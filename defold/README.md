# Defold Renderer

This repository owns the Defold native-extension adapter for the shared Rust simulation in [`unreal-unity-poc/rust-engine`](https://github.com/unreal-unity-poc/rust-engine).

## Hot path

```text
Defold input -> Lua module -> native extension -> rust_engine_tick -> state table -> render script
```

The native extension keeps the Rust engine opaque, accepts normalized control input, and returns the authoritative earth render state. The C++ ownership layer is tested independently from the Defold SDK; `extension/rust_engine/src/rust_engine_ext.cpp` is the Defold binding.

## Validate the SDK-independent core

```bash
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
```

To package the actual extension, make the `rust_engine` dynamic/static library available to Defold's build servers or vendor an approved prebuilt per target platform.
