#pragma once
#include "rust_engine.h"
#include <span>
namespace RustEarth {
class RustEngineClient final {
public:
    RustEngineClient();
    ~RustEngineClient();
    RustEngineClient(const RustEngineClient&) = delete;
    RustEngineClient& operator=(const RustEngineClient&) = delete;
    EarthRenderState Tick(ControlInput input, float deltaSeconds);
    EarthRenderState State() const;
    std::span<const SurfacePatch> SurfacePatches() const;
private:
    RustEngine* engine_{};
};
}
