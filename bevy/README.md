# Bevy Renderer

Bevy renders the shared Rust-owned earth state through a Rust-native ECS/rendering path.

## Integration path

```text
Bevy input system -> rust-engine tick -> Bevy systems/resources -> Bevy renderer
```

This target is the Rust-to-Rust comparison in the `unreal-unity-poc` portfolio: no managed boundary, dynamic-library loading, or C callback shim is required for the default path. An optional ABI-backed mode can be added later for direct comparison with the C/C++ and C# hosts.

The authoritative simulation and FFI contracts live in [`unreal-unity-poc/rust-engine`](https://github.com/unreal-unity-poc/rust-engine). The coordinated integration workspace lives in [`unreal-unity-poc/unreal-unity-monorepo`](https://github.com/unreal-unity-poc/unreal-unity-monorepo).

## Expected output

- Blue earth mesh driven by Rust-owned transform state.
- Green Rust-owned surface patches.
- Atmosphere shell or glow.
- Keyboard input routed into the authoritative Rust simulation before rendering.

## Run

```bash
cargo run
```

Controls: arrow keys rotate, Page Up/Page Down zoom, and R resets the shared simulation.

## Repository role

This standalone repository is the canonical Bevy deployable/demo unit. The monorepo may mirror or pin it for cross-renderer comparison, but releases and issues for this renderer belong here.
