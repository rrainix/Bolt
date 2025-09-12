#!/usr/bin/env bash
set -euo pipefail

ROOT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
cd "$ROOT_DIR"

CMD="${1:-gen}"

detect_platform() {
  local uname_s="${OS:-$(uname -s)}"
  case "$uname_s" in
    MINGW*|MSYS*|CYGWIN*|Windows_NT) echo "windows" ;;
    Darwin) echo "macos" ;;
    *) echo "linux" ;;
  esac
}

have() { command -v "$1" >/dev/null 2>&1; }

gen() {
  local plat
  plat="$(detect_platform)"
  if have premake5; then
    case "$plat" in
      windows) echo "[premake] Generating Visual Studio 2022 solution (no build folder)..." ; premake5 vs2022 ;;
      macos)   echo "[premake] Generating Xcode project..." ; premake5 xcode ;;
      *)       echo "[premake] Generating GNU Makefiles..." ; premake5 gmake2 ;;
    esac
    echo "Done."
  else
    echo "premake5 not found."
    if [[ "$plat" == "windows" ]]; then
      echo "[cmake fallback] Generating Visual Studio 2022 solution in-place..."
      cmake -S . -B . -G "Visual Studio 17 2022"
      echo "Done."
    else
      echo "Please install premake5: https://premake.github.io/download/  (or run on Windows with Visual Studio + cmake fallback)."
      exit 1
    fi
  fi
}

clean() {
  if have premake5; then
    echo "[premake] Cleaning generated projects..."
    premake5 clean || true
  fi
  echo "[cleanup] Removing common CMake artifacts (if any)..."
  rm -rf CMakeCache.txt CMakeFiles cmake_install.cmake Makefile \
         BoltCore/CMakeFiles BoltGame/CMakeFiles \
         BoltCore/cmake_install.cmake BoltGame/cmake_install.cmake || true
  echo "Clean complete."
}

case "$CMD" in
  gen|generate) gen ;;
  clean) clean ;;
  help|-h|--help)
    cat <<EOF
Usage: ./build.sh [gen|clean]

gen   : Generate IDE project files without creating a separate build folder.
        - Windows: premake5 vs2022 (or CMake fallback -> VS 2022 .sln in repo root)
        - macOS  : premake5 xcode
        - Linux  : premake5 gmake2
clean : Remove generated project files (Premake clean) and common CMake artifacts.

Prereqs:
  - Preferred: premake5 in PATH
  - Windows fallback: CMake + Visual Studio 2022
EOF
  ;;
  *) echo "Unknown command: $CMD"; exit 1 ;;
esac
