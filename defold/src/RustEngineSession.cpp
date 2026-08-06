#include "RustEngineSession.hpp"
#include <stdexcept>

RustEngineSession::RustEngineSession() : engine_(rust_engine_create()) {
    if (engine_ == nullptr) throw std::runtime_error("rust_engine_create returned null");
}
RustEngineSession::~RustEngineSession() { rust_engine_destroy(engine_); }
EarthRenderState RustEngineSession::tick(ControlInput input, float delta_seconds) {
    rust_engine_set_control_input(engine_, input);
    rust_engine_tick(engine_, delta_seconds);
    return state();
}
EarthRenderState RustEngineSession::state() const { return rust_engine_render_state(engine_); }
