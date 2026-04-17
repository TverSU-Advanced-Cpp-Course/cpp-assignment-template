#!/usr/bin/env bash
set -euo pipefail

if ! command -v clang-format >/dev/null 2>&1; then
  echo "Error: clang-format not found"
  exit 1
fi

mapfile -t files < <(find solution -type f \( \
    -name '*.h' -o -name '*.hh' -o -name '*.hpp' -o -name '*.hxx' -o \
    -name '*.c' -o -name '*.cc' -o -name '*.cpp' -o -name '*.cxx' \
\) | sort)

if [ ${#files[@]} -eq 0 ]; then
  echo "No C++ files found in solution/"
  exit 0
fi

clang-format --dry-run -Werror "${files[@]}"
echo "clang-format check passed"