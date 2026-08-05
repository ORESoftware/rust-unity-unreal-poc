#!/usr/bin/env bash
set -euo pipefail

ROOT=$(cd "$(dirname "${BASH_SOURCE[0]}")/../.." && pwd)
cd "$ROOT"

cargo build --locked --workspace --release

LIB="$ROOT/target/release/librust_engine.so"
BIN="$ROOT/target/ffi-contract-e2e"
test -f "$LIB"

for symbol in \
  rust_engine_create \
  rust_engine_destroy \
  rust_engine_set_control_input \
  rust_engine_tick \
  rust_engine_set_event_callback \
  rust_engine_clear_event_callback \
  rust_engine_render_state \
  rust_engine_surface_patches; do
  nm -D --defined-only "$LIB" | awk '{print $3}' | grep -Fxq "$symbol"
done

cc \
  -std=c11 \
  -Wall \
  -Wextra \
  -Werror \
  -pedantic \
  -I "$ROOT/rust-engine/include" \
  "$ROOT/tests/e2e/ffi_contract.c" \
  -L "$ROOT/target/release" \
  -lrust_engine \
  -lm \
  -o "$BIN"

LD_LIBRARY_PATH="$ROOT/target/release${LD_LIBRARY_PATH:+:$LD_LIBRARY_PATH}" "$BIN"
