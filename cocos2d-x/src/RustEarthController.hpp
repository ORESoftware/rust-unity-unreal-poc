#pragma once

#include "rust_engine.h"

#include <span>

class RustEarthController final {
public:
    RustEarthController();
    ~RustEarthController();

    RustEarthController(const RustEarthController&) = delete;
    RustEarthController& operator=(const RustEarthController&) = delete;
    RustEarthController(RustEarthController&&) = delete;
    RustEarthController& operator=(RustEarthController&&) = delete;

    [[nodiscard]] EarthRenderState update(const ControlInput& input, float delta_seconds);
    [[nodiscard]] EarthRenderState state() const;
    [[nodiscard]] std::span<const SurfacePatch> surface_patches() const;

private:
    RustEngine* engine_;
};
