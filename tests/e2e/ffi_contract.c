#include "rust_engine.h"

#include <math.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CHECK(condition, message)                                                \
    do {                                                                         \
        if (!(condition)) {                                                       \
            fprintf(stderr, "FAIL %s:%d: %s\n", __FILE__, __LINE__, (message)); \
            exit(EXIT_FAILURE);                                                   \
        }                                                                         \
    } while (0)

static unsigned callback_count = 0;
static EngineEvent last_event;

static int nearly_equal(float left, float right) {
    return fabsf(left - right) <= 0.0001f;
}

static void capture_event(void* user_data, EngineEvent event) {
    unsigned* observed = (unsigned*)user_data;
    *observed += 1;
    callback_count += 1;
    last_event = event;
}

static void test_default_lifecycle_and_null_contract(void) {
    EarthRenderState null_state = rust_engine_render_state(NULL);
    SurfacePatchView null_patches = rust_engine_surface_patches(NULL);

    CHECK(nearly_equal(null_state.radius, 1.0f),
          "null render_state must return the documented default");
    CHECK(nearly_equal(null_state.camera_distance, 4.2f),
          "null render_state must preserve the default camera distance");
    CHECK(null_patches.ptr == NULL, "null engine must return a null patch pointer");
    CHECK(null_patches.len == 0, "null engine must return zero patches");

    rust_engine_destroy(NULL);
    rust_engine_tick(NULL, 0.016f);
    rust_engine_set_control_input(NULL, (ControlInput){0});
    rust_engine_clear_event_callback(NULL);

    RustEngine* engine = rust_engine_create();
    CHECK(engine != NULL, "rust_engine_create returned NULL");

    EarthRenderState state = rust_engine_render_state(engine);
    CHECK(nearly_equal(state.radius, 1.0f), "default radius drifted");
    CHECK(state.atmosphere_radius > state.radius,
          "atmosphere must enclose the earth radius");
    CHECK(state.camera_distance > state.atmosphere_radius,
          "camera must begin outside the atmosphere");
    CHECK(isfinite(state.light_x) && isfinite(state.light_y) &&
              isfinite(state.light_z),
          "default light vector must be finite");

    rust_engine_destroy(engine);
    puts("ok 1 - default lifecycle and null-safe C ABI");
}

static void test_control_callback_clear_and_reset(void) {
    RustEngine* engine = rust_engine_create();
    CHECK(engine != NULL, "rust_engine_create returned NULL");

    unsigned user_observed = 0;
    callback_count = 0;
    memset(&last_event, 0, sizeof(last_event));
    rust_engine_set_event_callback(engine, capture_event, &user_observed);

    EarthRenderState before = rust_engine_render_state(engine);
    ControlInput extreme = {
        .rotate_x = 99.0f,
        .rotate_y = -99.0f,
        .zoom = 99.0f,
        .reset = 0,
    };
    rust_engine_set_control_input(engine, extreme);
    rust_engine_tick(engine, 9.0f);

    EarthRenderState after = rust_engine_render_state(engine);
    CHECK(after.rotation_x > before.rotation_x,
          "positive rotate_x must advance the authoritative state");
    CHECK(after.rotation_x <= 1.35f,
          "rotate_x input and state must remain clamped");
    CHECK(after.rotation_y >= 0.0f && after.rotation_y < 6.284f,
          "rotation_y must be wrapped into one turn");
    CHECK(after.camera_distance < before.camera_distance,
          "positive zoom must move the camera inward");
    CHECK(after.camera_distance >= 2.15f,
          "camera distance must respect the lower bound");
    CHECK(callback_count == 1 && user_observed == 1,
          "one successful tick must emit exactly one host callback");
    CHECK(last_event.kind == 1, "tick callback kind must be 1");
    CHECK(last_event.frame_index == 1, "first callback frame index must be 1");
    CHECK(nearly_equal(last_event.state.rotation_x, after.rotation_x),
          "callback state must match pull-based render_state");
    CHECK(nearly_equal(last_event.state.camera_distance, after.camera_distance),
          "callback camera state must match pull-based render_state");

    rust_engine_clear_event_callback(engine);
    rust_engine_tick(engine, 0.016f);
    CHECK(callback_count == 1 && user_observed == 1,
          "clearing the callback must stop subsequent notifications");

    ControlInput reset = {.reset = 1};
    rust_engine_set_control_input(engine, reset);
    rust_engine_tick(engine, 0.016f);
    EarthRenderState restored = rust_engine_render_state(engine);
    CHECK(nearly_equal(restored.rotation_x, -0.25f),
          "reset must restore default rotation_x");
    CHECK(nearly_equal(restored.rotation_y, 0.0f),
          "reset must restore default rotation_y");
    CHECK(nearly_equal(restored.camera_distance, 4.2f),
          "reset must restore default camera distance");

    rust_engine_destroy(engine);
    puts("ok 2 - input clamp, callback delivery, clear, and reset");
}

static void test_surface_patch_pointer_and_data_contract(void) {
    RustEngine* engine = rust_engine_create();
    CHECK(engine != NULL, "rust_engine_create returned NULL");

    SurfacePatchView first_view = rust_engine_surface_patches(engine);
    CHECK(first_view.ptr != NULL, "live engine must expose a patch pointer");
    CHECK(first_view.len == 12, "surface patch count must remain twelve");

    for (size_t index = 0; index < first_view.len; ++index) {
        const SurfacePatch* patch = &first_view.ptr[index];
        CHECK(isfinite(patch->lat_degrees) && patch->lat_degrees >= -90.0f &&
                  patch->lat_degrees <= 90.0f,
              "patch latitude is invalid");
        CHECK(isfinite(patch->lon_degrees) && patch->lon_degrees >= -180.0f &&
                  patch->lon_degrees <= 180.0f,
              "patch longitude is invalid");
        CHECK(isfinite(patch->radius_degrees) && patch->radius_degrees > 0.0f,
              "patch radius must be finite and positive");
        CHECK(isfinite(patch->stretch_x) && patch->stretch_x > 0.0f,
              "patch stretch_x must be finite and positive");
        CHECK(isfinite(patch->stretch_y) && patch->stretch_y > 0.0f,
              "patch stretch_y must be finite and positive");
    }

    const SurfacePatch* original_ptr = first_view.ptr;
    SurfacePatch first_patch = first_view.ptr[0];
    rust_engine_tick(engine, 0.016f);
    SurfacePatchView after_tick = rust_engine_surface_patches(engine);

    CHECK(after_tick.ptr == original_ptr,
          "static surface patch storage must keep a stable pointer across ticks");
    CHECK(after_tick.len == first_view.len,
          "surface patch count must remain stable across ticks");
    CHECK(memcmp(&first_patch, &after_tick.ptr[0], sizeof(first_patch)) == 0,
          "surface patch data must not mutate during a simulation tick");

    rust_engine_destroy(engine);
    puts("ok 3 - surface patch pointer, count, bounds, and tick stability");
}

int main(void) {
    test_default_lifecycle_and_null_contract();
    test_control_callback_clear_and_reset();
    test_surface_patch_pointer_and_data_contract();
    puts("1..3");
    return EXIT_SUCCESS;
}
