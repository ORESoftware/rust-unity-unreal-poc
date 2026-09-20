#!/bin/sh
set -eu

REPO_ROOT=$(CDPATH= cd -- "$(dirname -- "$0")/.." && pwd)
TMP=$(mktemp -d)
trap 'rm -rf "$TMP"' EXIT HUP INT TERM

fail() {
  echo "git-hook-installer contract: $*" >&2
  exit 1
}

new_repo() {
  name=$1
  repo="$TMP/$name"
  git init -q "$repo"
  mkdir -p "$repo/.ores-lint"
  cp "$REPO_ROOT/.ores-lint/install-git-hooks.sh" "$repo/.ores-lint/install-git-hooks.sh"
  cat > "$repo/.ores-lint/lint.sh" <<'LINT'
#!/bin/sh
set -eu
printf '%s\n' ran > .ores-lint/lint-ran
LINT
  chmod +x "$repo/.ores-lint/lint.sh"
  printf '%s\n' "$repo"
}

run_installer() {
  repo=$1
  shift
  (cd "$repo" && "$@" sh .ores-lint/install-git-hooks.sh)
}

# Default Git hook directory.
repo=$(new_repo default)
run_installer "$repo" env
hook="$(git -C "$repo" rev-parse --absolute-git-dir)/hooks/pre-push"
[ -x "$hook" ] || fail "default pre-push hook was not installed"
grep -Fq 'installed by rust-unity-unreal-poc/.ores-lint/install-git-hooks.sh' "$hook" || fail "default hook marker missing"
(cd "$repo" && "$hook")
[ -f "$repo/.ores-lint/lint-ran" ] || fail "installed hook did not execute root ores-lint"

# Relative core.hooksPath is honored without rewriting the config.
repo=$(new_repo relative)
git -C "$repo" config core.hooksPath .githooks
run_installer "$repo" env
[ -x "$repo/.githooks/pre-push" ] || fail "relative hooksPath was ignored"
[ "$(git -C "$repo" config --get core.hooksPath)" = ".githooks" ] || fail "relative hooksPath was rewritten"

# Absolute core.hooksPath is honored.
repo=$(new_repo absolute)
absolute_hooks="$TMP/absolute-hooks"
git -C "$repo" config core.hooksPath "$absolute_hooks"
run_installer "$repo" env
[ -x "$absolute_hooks/pre-push" ] || fail "absolute hooksPath was ignored"

# Literal ~/ paths are expanded against HOME by the installer.
repo=$(new_repo tilde)
home="$TMP/home"
mkdir -p "$home"
git -C "$repo" config core.hooksPath '~/custom-hooks'
run_installer "$repo" env HOME="$home"
[ -x "$home/custom-hooks/pre-push" ] || fail "tilde hooksPath was not expanded against HOME"
[ "$(git -C "$repo" config --get core.hooksPath)" = "~/custom-hooks" ] || fail "tilde hooksPath was rewritten"

# An unrelated hook is never overwritten.
repo=$(new_repo conflict)
git -C "$repo" config core.hooksPath .githooks
mkdir -p "$repo/.githooks"
printf '%s\n' '#!/bin/sh' 'echo keep-me' > "$repo/.githooks/pre-push"
chmod +x "$repo/.githooks/pre-push"
if run_installer "$repo" env >"$TMP/conflict.out" 2>&1; then
  fail "installer clobbered or accepted an unrelated hook"
fi
grep -Fq 'refusing to clobber existing hook' "$TMP/conflict.out" || fail "conflict refusal was not explicit"
grep -Fq 'keep-me' "$repo/.githooks/pre-push" || fail "existing hook content changed"

# Every nested copy is a byte-identical delegate to the root policy.
canonical="$REPO_ROOT/cef/.ores-lint/install-git-hooks.sh"
for project in wasm bevy tauri unity leptos dioxus webview2 v8-blink rust-engine electron-abi electron-wasm; do
  cmp -s "$canonical" "$REPO_ROOT/$project/.ores-lint/install-git-hooks.sh" || fail "$project installer drifted from the canonical delegate"
done

# A nested invocation still installs the root hook and therefore the root lint.
repo=$(new_repo nested)
mkdir -p "$repo/cef/.ores-lint"
cp "$canonical" "$repo/cef/.ores-lint/install-git-hooks.sh"
(cd "$repo/cef" && sh .ores-lint/install-git-hooks.sh)
hook="$(git -C "$repo" rev-parse --absolute-git-dir)/hooks/pre-push"
(cd "$repo" && "$hook")
[ -f "$repo/.ores-lint/lint-ran" ] || fail "nested delegate did not route to root ores-lint"

printf '%s\n' 'git-hook-installer contract: PASS'
