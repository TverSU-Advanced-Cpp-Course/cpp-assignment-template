#!/usr/bin/env bash
set -euo pipefail

BUILD_DIR="${1:-build}"

cmake_args=(
  -S .
  -B "$BUILD_DIR"
  -DCMAKE_BUILD_TYPE=Debug
  -DCMAKE_EXPORT_COMPILE_COMMANDS=ON
  -DENABLE_SANITIZERS=ON
)

if command -v ninja >/dev/null 2>&1; then
  cmake_args+=(-G Ninja)
fi

if command -v clang-tidy >/dev/null 2>&1; then
  cmake_args+=(-DENABLE_CLANG_TIDY=ON)
else
  cmake_args+=(-DENABLE_CLANG_TIDY=OFF)
  echo "Warning: clang-tidy not found, configuring without clang-tidy"
fi

cmake "${cmake_args[@]}"