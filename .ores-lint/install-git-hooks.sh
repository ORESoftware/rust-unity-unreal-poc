#!/bin/sh
# Optional: install a pre-push hook that runs the repository-root ores-lint.
# This root copy is authoritative; nested project copies delegate here.
set -eu

ROOT=$(git rev-parse --show-toplevel 2>/dev/null) || {
  echo "ores-lint hook installer: not a git repo" >&2
  exit 1
}

HOOKS_PATH=$(git -C "$ROOT" config --get core.hooksPath 2>/dev/null || true)
case "$HOOKS_PATH" in
  "")
    GIT_DIR=$(git -C "$ROOT" rev-parse --absolute-git-dir)
    HOOK_DIR="$GIT_DIR/hooks"
    ;;
  /*)
    HOOK_DIR="$HOOKS_PATH"
    ;;
  '~/'*)
    # Git stores a literal ~/ prefix. POSIX sh does not expand it after the
    # value is read from config, so strip exactly those first two bytes.
    HOOK_DIR="$HOME/${HOOKS_PATH#??}"
    ;;
  *)
    # Relative core.hooksPath values are resolved from the worktree root for
    # ordinary client-side hooks.
    HOOK_DIR="$ROOT/$HOOKS_PATH"
    ;;
esac

mkdir -p "$HOOK_DIR"
HOOK="$HOOK_DIR/pre-push"
MARKER='installed by rust-unity-unreal-poc/.ores-lint/install-git-hooks.sh'

if [ -e "$HOOK" ] && ! grep -Fq "$MARKER" "$HOOK"; then
  echo "ores-lint hook installer: refusing to clobber existing hook: $HOOK" >&2
  exit 1
fi

cat > "$HOOK" <<'INNER'
#!/bin/sh
# installed by rust-unity-unreal-poc/.ores-lint/install-git-hooks.sh
set -eu
ROOT=$(git rev-parse --show-toplevel 2>/dev/null) || {
  echo "ores-lint pre-push: not a git repo" >&2
  exit 1
}
LINT="$ROOT/.ores-lint/lint.sh"
if [ ! -f "$LINT" ]; then
  echo "ores-lint pre-push: missing $LINT" >&2
  exit 1
fi
exec sh "$LINT"
INNER
chmod +x "$HOOK"
printf 'installed %s\n' "$HOOK"
