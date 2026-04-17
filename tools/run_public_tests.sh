#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="${1:-build}"

./tools/configure.sh "$BUILD_DIR"
cmake --build "$BUILD_DIR" -j
ctest --test-dir "$BUILD_DIR" --output-on-failure