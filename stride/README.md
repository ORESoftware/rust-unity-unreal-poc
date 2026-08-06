# Stride Renderer

This repository owns the Stride adapter for the authoritative simulation in [`unreal-unity-poc/rust-engine`](https://github.com/unreal-unity-poc/rust-engine).

## Hot path

```text
Stride input -> RustEarthSyncScript -> RustEngineSession -> C ABI -> EarthRenderState -> entity transforms
```

`src/RustEngine.Interop` is an SDK-independent managed library with exact native struct layouts, safe ownership, bounded inputs, and state access. `samples/Stride/RustEarthSyncScript.cs` is the Stride `SyncScript` boundary. CI validates the interop contract without requiring a graphical editor installation.

## Validate

```bash
dotnet build src/RustEngine.Interop/RustEngine.Interop.csproj --configuration Release
dotnet run --project tests/InteropContract/InteropContract.csproj --configuration Release
```

The stable Stride 4.3 line is the initial integration target. Runtime tests require a Stride project plus the target-platform `rust_engine` native library.
