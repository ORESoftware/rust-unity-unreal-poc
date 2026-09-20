#!/bin/sh
# Delegate nested project hook installation to the repository-root policy.
set -eu
ROOT=$(git rev-parse --show-toplevel 2>/dev/null) || { echo "ores-lint hook installer: not a git repo" >&2; exit 1; }
INSTALLER="$ROOT/.ores-lint/install-git-hooks.sh"
[ -f "$INSTALLER" ] || { echo "ores-lint hook installer: missing $INSTALLER" >&2; exit 1; }
exec sh "$INSTALLER" "$@"
