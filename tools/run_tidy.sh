#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="${1:-build}"

if ! command -v clang-tidy >/dev/null 2>&1; then
  echo "Error: clang-tidy not found"
  exit 1
fi

if [ ! -f "$BUILD_DIR/compile_commands.json" ]; then
  echo "compile_commands.json not found, run ./tools/configure.sh first"
  exit 1
fi

mapfile -t files < <(find solution -type f \( \
    -name '*.cpp' -o -name '*.cc' -o -name '*.cxx' \
\) | sort)

if [ ${#files[@]} -eq 0 ]; then
  echo "No translation units found in solution/"
  exit 0
fi

clang-tidy -p "$BUILD_DIR" "${files[@]}"