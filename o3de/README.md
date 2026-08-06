# O3DE Rust Earth Gem

This repository owns the Open 3D Engine adapter for the authoritative simulation in `unreal-unity-poc/rust-engine`.

The checked-in C++ client owns one opaque Rust engine instance, forwards normalized input, ticks the Rust simulation, and exposes the resulting earth state and surface-patch view to an O3DE Gem component. The core layer is tested without downloading the full O3DE SDK.

```bash
cmake -S . -B build -G Ninja
cmake --build build
ctest --test-dir build --output-on-failure
```

Register this repository as a Gem with `o3de register --gem-path "$PWD"`, then link the platform-specific `rust_engine` library in the consuming project.
