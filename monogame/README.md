# MonoGame Renderer

A runnable MonoGame DesktopGL host for the authoritative Rust simulation in [`unreal-unity-poc/rust-engine`](https://github.com/unreal-unity-poc/rust-engine).

## Hot path

```text
MonoGame keyboard -> ControlInput -> Rust C ABI -> EarthRenderState -> SpriteBatch
```

The sample renders a generated blue globe texture and applies the Rust-owned rotation and camera-distance state every frame. The native library is never reimplemented in C#: build `rust-engine` and place `rust_engine.dll`, `librust_engine.so`, or `librust_engine.dylib` beside the executable.

## Run

```bash
dotnet restore
dotnet run --project src/MonoGameRustEarth/MonoGameRustEarth.csproj
```

Controls: arrow keys rotate, Page Up/Page Down zoom, and R resets.
