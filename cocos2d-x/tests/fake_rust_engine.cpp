#include "rust_engine.h"

#include <algorithm>

struct RustEngine {
    EarthRenderState state{1.0F, 1.035F, -0.25F, 0.0F, 0.0F, 4.2F, -0.35F, 0.45F, -0.82F};
    ControlInput input{};
    uint64_t frame_index{};
    RustEngineEventCallback callback{};
    void* user_data{};
};

extern "C" RustEngine* rust_engine_create(void) { return new RustEngine{}; }
extern "C" void rust_engine_destroy(RustEngine* engine) { delete engine; }
extern "C" void rust_engine_set_control_input(RustEngine* engine, ControlInput input) {
    if (engine != nullptr) engine->input = input;
}
extern "C" void rust_engine_tick(RustEngine* engine, float delta_seconds) {
    if (engine == nullptr) return;
    const float dt = std::clamp(delta_seconds, 0.0F, 0.1F);
    if (engine->input.reset != 0U) {
        engine->state = EarthRenderState{1.0F, 1.035F, -0.25F, 0.0F, 0.0F, 4.2F, -0.35F, 0.45F, -0.82F};
    } else {
        engine->state.rotation_x += engine->input.rotate_x * dt;
        engine->state.rotation_y += engine->input.rotate_y * dt;
        engine->state.camera_distance = std::clamp(
            engine->state.camera_distance - engine->input.zoom * dt,
            2.15F,
            9.5F);
    }
    ++engine->frame_index;
    if (engine->callback != nullptr) {
        engine->callback(engine->user_data, EngineEvent{1U, engine->frame_index, engine->state});
    }
}
extern "C" void rust_engine_set_event_callback(RustEngine* engine, RustEngineEventCallback callback, void* user_data) {
    if (engine != nullptr) {
        engine->callback = callback;
        engine->user_data = user_data;
    }
}
extern "C" void rust_engine_clear_event_callback(RustEngine* engine) {
    if (engine != nullptr) engine->callback = nullptr;
}
extern "C" EarthRenderState rust_engine_render_state(const RustEngine* engine) {
    return engine == nullptr ? EarthRenderState{} : engine->state;
}
extern "C" SurfacePatchView rust_engine_surface_patches(const RustEngine*) {
    static const SurfacePatch patches[] = {{46.0F, -102.0F, 18.0F, 1.45F, 0.85F}};
    return SurfacePatchView{patches, 1U};
}
