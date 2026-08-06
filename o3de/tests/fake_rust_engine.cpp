#include "rust_engine.h"
struct RustEngine { EarthRenderState state{1,1.035F,-0.25F,0,0,4.2F,-0.35F,0.45F,-0.82F}; ControlInput input{}; };
extern "C" RustEngine* rust_engine_create(void) { return new RustEngine{}; }
extern "C" void rust_engine_destroy(RustEngine* value) { delete value; }
extern "C" void rust_engine_set_control_input(RustEngine* value, ControlInput input) { if(value) value->input=input; }
extern "C" void rust_engine_tick(RustEngine* value, float dt) { if(value) value->state.rotation_x += value->input.rotate_x * dt; }
extern "C" EarthRenderState rust_engine_render_state(const RustEngine* value) { return value ? value->state : EarthRenderState{}; }
extern "C" SurfacePatchView rust_engine_surface_patches(const RustEngine*) { static const SurfacePatch patch{0,0,10,1,1}; return {&patch,1}; }
