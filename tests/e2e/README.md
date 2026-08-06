# Native host ABI E2E

This directory validates the exported Rust engine from the perspective of a
plain C host, rather than calling Rust internals directly.

`run_ffi_contract.sh` builds the release `cdylib`, checks every symbol declared
by `rust-engine/include/rust_engine.h`, compiles `ffi_contract.c` with strict C
warnings, dynamically links it to `librust_engine.so`, and executes three
journeys:

1. null-safe lifecycle behavior and the documented default render state;
2. clamped bidirectional input, synchronous callback delivery, callback
   clearing, and reset behavior;
3. the zero-copy surface-patch pointer, count, field bounds, and stability
   across a tick.

Run from the repository root:

```sh
bash tests/e2e/run_ffi_contract.sh
```

## Ownership and lifetime

Only `rust_engine_create` allocates an engine. A non-null pointer must be passed
to `rust_engine_destroy` exactly once. Host code must serialize mutable access
to an engine and must keep callback code and `user_data` alive until the
callback is cleared or the engine is destroyed.

`rust_engine_render_state` returns a value copy. `rust_engine_surface_patches`
returns a borrowed, read-only view into Rust-owned memory; the host must not
free or mutate it and should reacquire the view after any engine mutation.
