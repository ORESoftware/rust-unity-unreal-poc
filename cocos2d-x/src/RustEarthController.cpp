#include "RustEarthController.hpp"

#include <stdexcept>

RustEarthController::RustEarthController() : engine_(rust_engine_create()) {
    if (engine_ == nullptr) {
        throw std::runtime_error("rust_engine_create returned null");
    }
}

RustEarthController::~RustEarthController() {
    rust_engine_destroy(engine_);
}

EarthRenderState RustEarthController::update(const ControlInput& input, float delta_seconds) {
    rust_engine_set_control_input(engine_, input);
    rust_engine_tick(engine_, delta_seconds);
    return state();
}

EarthRenderState RustEarthController::state() const {
    return rust_engine_render_state(engine_);
}

std::span<const SurfacePatch> RustEarthController::surface_patches() const {
    const SurfacePatchView view = rust_engine_surface_patches(engine_);
    return view.ptr == nullptr ? std::span<const SurfacePatch>{}
                               : std::span<const SurfacePatch>{view.ptr, view.len};
}
