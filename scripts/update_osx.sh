#!/usr/bin/env bash
set -euo pipefail

VERSION=""
PREFIX=""
USER_INSTALL="0"
SUDO_STDIN="0"
REPO_OWNER="mlc-tools"
REPO_NAME="mlc-cpp"

fail() {
  echo "Error: $*" >&2
  exit 1
}

need_cmd() {
  if ! command -v "$1" >/dev/null 2>&1
  then
    fail "Required command '$1' is not installed."
  fi
}

need_any_cmd() {
  local label="$1"
  shift
  local cmd
  for cmd in "$@"
  do
    if command -v "$cmd" >/dev/null 2>&1
    then
      return 0
    fi
  done
  fail "$label is not installed. Expected one of: $*"
}

detect_re2_prefix() {
  if command -v brew >/dev/null 2>&1
  then
    local brew_prefix
    brew_prefix="$(brew --prefix re2 2>/dev/null || true)"
    if [ -n "$brew_prefix" ] && [ -f "${brew_prefix}/lib/cmake/re2/re2Config.cmake" ]
    then
      printf '%s\n' "$brew_prefix"
      return 0
    fi
  fi

  local prefix
  for prefix in /usr/local /opt/homebrew
  do
    if [ -f "${prefix}/lib/cmake/re2/re2Config.cmake" ]
    then
      printf '%s\n' "$prefix"
      return 0
    fi
  done

  return 1
}

check_dependencies() {
  need_cmd curl
  need_cmd tar
  need_cmd cmake
  need_cmd make
  need_any_cmd "C++ compiler" c++ g++ clang++ gcc

  if ! RE2_PREFIX="$(detect_re2_prefix)"
  then
    fail "RE2 library is not installed or CMake cannot find it. Install it first, for example: brew install re2"
  fi
}

download_source_archive() {
  local archive_path="$1"
  local url
  for url in \
    "https://codeload.github.com/${REPO_OWNER}/${REPO_NAME}/tar.gz/refs/tags/v/${VERSION}" \
    "https://codeload.github.com/${REPO_OWNER}/${REPO_NAME}/tar.gz/refs/tags/${VERSION}"
  do
    if curl -fsSL -o "$archive_path" "$url"
    then
      echo "Downloaded sources from ${url}"
      return 0
    fi
  done

  fail "Failed to download source archive for version '${VERSION}'. Checked tags '${VERSION}' and 'v${VERSION}'."
}

while [ $# -gt 0 ]
do
  case "$1" in
    --version)
      VERSION="$2"
      shift 2
      ;;
    --prefix)
      PREFIX="$2"
      shift 2
      ;;
    --user)
      USER_INSTALL="1"
      shift 1
      ;;
    -S)
      # pass password on stdin to sudo
      SUDO_STDIN="1"
      shift 1
      ;;
    *)
      # ignore unknown flags to keep script tolerant
      shift 1
      ;;
  esac
done

if [ -z "$VERSION" ]
then
  echo "Usage: mlc update --version <x.y.z> [--user|--prefix <path>] [-S]" >&2
  exit 2
fi

check_dependencies

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

ARCHIVE_PATH="${TMP_DIR}/mlc-cpp-${VERSION}.tar.gz"
download_source_archive "$ARCHIVE_PATH"

echo "Extracting sources ..."
tar -xzf "$ARCHIVE_PATH" -C "$TMP_DIR"
SOURCE_DIR="$(find "$TMP_DIR" -mindepth 1 -maxdepth 1 -type d -name "${REPO_NAME}-*" | head -n 1)"
[ -n "$SOURCE_DIR" ] || fail "Unable to locate extracted source directory."

BUILD_DIR="${TMP_DIR}/build"
CPU_COUNT="$(sysctl -n hw.ncpu 2>/dev/null || echo 4)"

echo "Configuring build in ${BUILD_DIR} ..."
if ! cmake -S "$SOURCE_DIR" -B "$BUILD_DIR" -DCMAKE_BUILD_TYPE=Release -DCMAKE_PREFIX_PATH="${RE2_PREFIX}"
then
  fail "CMake configuration failed. Verify that build tools are installed and RE2 is available to CMake."
fi

echo "Building mlc ..."
if ! cmake --build "$BUILD_DIR" -j"${CPU_COUNT}"
then
  fail "Build failed for version '${VERSION}'."
fi

BUILT_BINARY="${BUILD_DIR}/mlc"
[ -x "$BUILT_BINARY" ] || fail "Built binary was not found at ${BUILT_BINARY}"

# Decide target path
if [ -n "$PREFIX" ]
then
  TARGET_PATH="${PREFIX%/}/bin/mlc"
elif [ "$USER_INSTALL" = "1" ]
then
  TARGET_PATH="${HOME}/.local/bin/mlc"
else
  if command -v mlc >/dev/null 2>&1
  then
    TARGET_PATH="$(command -v mlc)"
  else
    BREW_PREFIX="$(brew --prefix 2>/dev/null || true)"
    if [ -n "$BREW_PREFIX" ]
    then
      TARGET_PATH="${BREW_PREFIX}/bin/mlc"
    else
      TARGET_PATH="/usr/local/bin/mlc"
    fi
  fi
fi

TARGET_DIR="$(dirname "$TARGET_PATH")"
if [ ! -d "$TARGET_DIR" ]
then
  if mkdir -p "$TARGET_DIR" 2>/dev/null
  then
    :
  else
    if [ "$SUDO_STDIN" = "1" ]
    then
      sudo -S mkdir -p "$TARGET_DIR"
    else
      sudo mkdir -p "$TARGET_DIR"
    fi
  fi
fi

BACKUP_PATH="${TARGET_PATH}.bak"
if [ -f "$TARGET_PATH" ]
then
  echo "Backing up current binary to ${BACKUP_PATH}"
  if [ -w "$TARGET_DIR" ]
  then
    cp "$TARGET_PATH" "$BACKUP_PATH" || true
  else
    if [ "$SUDO_STDIN" = "1" ]
    then
      echo "" | sudo -S true >/dev/null 2>&1 || true
      sudo -S cp "$TARGET_PATH" "$BACKUP_PATH"
    else
      sudo cp "$TARGET_PATH" "$BACKUP_PATH"
    fi
  fi
fi

echo "Installing to ${TARGET_PATH} ..."
if [ -w "$TARGET_DIR" ]
then
  install -m 755 "${BUILT_BINARY}" "$TARGET_PATH"
else
  if [ "$SUDO_STDIN" = "1" ]
  then
    sudo -S install -m 755 "${BUILT_BINARY}" "$TARGET_PATH"
  else
    sudo install -m 755 "${BUILT_BINARY}" "$TARGET_PATH"
  fi
fi

echo "mlc updated to version: $("$TARGET_PATH" -v 2>/dev/null || echo "$VERSION")"
if [ "$USER_INSTALL" = "1" ]
then
  case ":$PATH:" in
    *:"${HOME}/.local/bin":*) ;;
    *) echo "Note: add ${HOME}/.local/bin to PATH" ;;
  esac
fi
