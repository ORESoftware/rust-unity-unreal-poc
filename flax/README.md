# Flax Renderer

This repository owns the Flax C# adapter for the authoritative simulation in [`unreal-unity-poc/rust-engine`](https://github.com/unreal-unity-poc/rust-engine).

## Hot path

```text
Flax input -> RustEngineSession -> C ABI -> Rust tick -> EarthRenderState -> Flax actor/materials
```

`src/RustEngine.Interop` is a standalone, SDK-independent .NET library containing exact `StructLayout` definitions, native imports, safe-handle ownership, input clamping, and render-state access. `samples/Flax/RustEarthActor.cs` shows the Flax script boundary without forcing CI to download the Flax editor SDK.

## Validate

```bash
dotnet build src/RustEngine.Interop/RustEngine.Interop.csproj --configuration Release
dotnet run --project tests/InteropContract/InteropContract.csproj --configuration Release
```

The contract test verifies every native struct size and field offset expected by Rust/C/C++ hosts. Runtime smoke tests require the platform-specific `rust_engine` dynamic library beside the Flax executable.
