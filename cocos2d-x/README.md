# Cocos2d-x Renderer

This repository owns the Cocos2d-x adapter for the shared Rust simulation in [`unreal-unity-poc/rust-engine`](https://github.com/unreal-unity-poc/rust-engine).

## Hot path

```text
Cocos2d-x input -> RustEarthController -> rust_engine_tick -> callback/state -> Cocos2d-x draw/update
```

The checked-in C++ controller is engine-independent and fully testable without downloading Cocos2d-x. A Cocos scene/layer can own one controller, forward keyboard/touch input, and render the returned `EarthRenderState` and `SurfacePatchView`.

## Validate

```bash
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
```

The test target links a deterministic fake implementation of the Rust C ABI. Production builds link the dynamic/static library built by `unreal-unity-poc/rust-engine`.
