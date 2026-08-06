#include <dmsdk/sdk.h>
#include "../../../include/rust_engine.h"

namespace {
RustEngine* g_engine = nullptr;

int Tick(lua_State* L) {
    const float dt = static_cast<float>(luaL_checknumber(L, 1));
    ControlInput input{};
    if (lua_istable(L, 2)) {
        lua_getfield(L, 2, "rotate_x"); input.rotate_x = static_cast<float>(luaL_optnumber(L, -1, 0.0)); lua_pop(L, 1);
        lua_getfield(L, 2, "rotate_y"); input.rotate_y = static_cast<float>(luaL_optnumber(L, -1, 0.0)); lua_pop(L, 1);
        lua_getfield(L, 2, "zoom"); input.zoom = static_cast<float>(luaL_optnumber(L, -1, 0.0)); lua_pop(L, 1);
        lua_getfield(L, 2, "reset"); input.reset = lua_toboolean(L, -1) ? 1U : 0U; lua_pop(L, 1);
    }
    rust_engine_set_control_input(g_engine, input);
    rust_engine_tick(g_engine, dt);
    const EarthRenderState state = rust_engine_render_state(g_engine);
    lua_newtable(L);
    lua_pushnumber(L, state.radius); lua_setfield(L, -2, "radius");
    lua_pushnumber(L, state.atmosphere_radius); lua_setfield(L, -2, "atmosphere_radius");
    lua_pushnumber(L, state.rotation_x); lua_setfield(L, -2, "rotation_x");
    lua_pushnumber(L, state.rotation_y); lua_setfield(L, -2, "rotation_y");
    lua_pushnumber(L, state.cloud_rotation_y); lua_setfield(L, -2, "cloud_rotation_y");
    lua_pushnumber(L, state.camera_distance); lua_setfield(L, -2, "camera_distance");
    return 1;
}

const luaL_Reg kMethods[] = {{"tick", Tick}, {nullptr, nullptr}};

dmExtension::Result Initialize(dmExtension::Params* params) {
    g_engine = rust_engine_create();
    if (g_engine == nullptr) return dmExtension::RESULT_INIT_ERROR;
    luaL_register(params->m_L, "rust_engine", kMethods);
    lua_pop(params->m_L, 1);
    return dmExtension::RESULT_OK;
}

dmExtension::Result Finalize(dmExtension::Params*) {
    rust_engine_destroy(g_engine);
    g_engine = nullptr;
    return dmExtension::RESULT_OK;
}
}

DM_DECLARE_EXTENSION(RustEngineExt, "RustEngineExt", nullptr, nullptr, Initialize, nullptr, nullptr, Finalize)
