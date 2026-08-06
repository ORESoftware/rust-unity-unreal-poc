#include "rust_engine.h"
#include <algorithm>
struct RustEngine { EarthRenderState state{1,1.035F,-0.25F,0,0,4.2F,-0.35F,0.45F,-0.82F}; ControlInput input{}; };
extern "C" RustEngine* rust_engine_create(void) { return new RustEngine{}; }
extern "C" void rust_engine_destroy(RustEngine* value) { delete value; }
extern "C" void rust_engine_set_control_input(RustEngine* value, ControlInput input) { if(value) value->input=input; }
extern "C" void rust_engine_tick(RustEngine* value, float dt) { if(!value) return; dt=std::clamp(dt,0.0F,0.1F); value->state.rotation_y += value->input.rotate_y*dt; }
extern "C" void rust_engine_set_event_callback(RustEngine*, RustEngineEventCallback, void*) {}
extern "C" void rust_engine_clear_event_callback(RustEngine*) {}
extern "C" EarthRenderState rust_engine_render_state(const RustEngine* value) { return value ? value->state : EarthRenderState{}; }
extern "C" SurfacePatchView rust_engine_surface_patches(const RustEngine*) { return {nullptr,0}; }
