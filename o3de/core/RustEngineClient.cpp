#include "RustEngineClient.hpp"
#include <stdexcept>
namespace RustEarth {
RustEngineClient::RustEngineClient() : engine_(rust_engine_create()) {
    if (engine_ == nullptr) throw std::runtime_error("rust_engine_create returned null");
}
RustEngineClient::~RustEngineClient() { rust_engine_destroy(engine_); }
EarthRenderState RustEngineClient::Tick(ControlInput input, float deltaSeconds) {
    rust_engine_set_control_input(engine_, input);
    rust_engine_tick(engine_, deltaSeconds);
    return State();
}
EarthRenderState RustEngineClient::State() const { return rust_engine_render_state(engine_); }
std::span<const SurfacePatch> RustEngineClient::SurfacePatches() const {
    const auto view = rust_engine_surface_patches(engine_);
    return view.ptr == nullptr ? std::span<const SurfacePatch>{} : std::span<const SurfacePatch>{view.ptr, view.len};
}
}
