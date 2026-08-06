#pragma once
#include "rust_engine.h"

class RustEngineSession final {
public:
    RustEngineSession();
    ~RustEngineSession();
    RustEngineSession(const RustEngineSession&) = delete;
    RustEngineSession& operator=(const RustEngineSession&) = delete;

    EarthRenderState tick(ControlInput input, float delta_seconds);
    EarthRenderState state() const;

private:
    RustEngine* engine_;
};
