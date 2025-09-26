#!/usr/bin/env bash
set -euo pipefail

VERSION=""
PREFIX=""
USER_INSTALL="0"
SUDO_STDIN="0"

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

TMP_DIR="$(mktemp -d)"
trap 'rm -rf "$TMP_DIR"' EXIT

REF="osx/${VERSION}"
BASE_URL="https://raw.githubusercontent.com/mlc-tools/mlc-binary-app-versions/${REF}"

echo "Downloading ${BASE_URL}/mlc ..."
curl -fsSL -o "${TMP_DIR}/mlc" "${BASE_URL}/mlc"
chmod +x "${TMP_DIR}/mlc"

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
mkdir -p "$TARGET_DIR" 2>/dev/null || true

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
  install -m 755 "${TMP_DIR}/mlc" "$TARGET_PATH"
else
  if [ "$SUDO_STDIN" = "1" ]
  then
    sudo -S install -m 755 "${TMP_DIR}/mlc" "$TARGET_PATH"
  else
    sudo install -m 755 "${TMP_DIR}/mlc" "$TARGET_PATH"
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
