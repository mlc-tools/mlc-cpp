#!/usr/bin/env bash
set -euo pipefail

VERSION=""
while [ $# -gt 0 ]
do
case "$1" in
    --version) VERSION="$2"; shift 2;;
    *) shift;;
esac
done

if [ -z "$VERSION" ]
then
echo "Usage: mlc update --version <x.y.z>" >&2
exit 2
fi

TMP="$(mktemp -d)"
trap 'rm -rf "$TMP"' EXIT

ARCH="$(uname -m)"
TAG="osx/${VERSION}"  # при разделении по архитектурам используйте osx-${ARCH}/${VERSION}
BASE="https://raw.githubusercontent.com/mlc-tools/mlc-binary-app-versions/${TAG}"
echo "Downloading ${BASE}/mlc ..."
curl -fsSL -o "${TMP}/mlc" "${BASE}/mlc"

# Опциональная проверка целостности

if curl -fsSL -o "${TMP}/mlc.sha256" "${BASE}/mlc.sha256"
then
(cd "${TMP}" && shasum -a 256 -c mlc.sha256)
fi

chmod +x "${TMP}/mlc"

if command -v mlc >/dev/null 2>&1
then
TARGET="$(command -v mlc)"
else
PREFIX="$(brew --prefix 2>/dev/null || echo /usr/local)"
TARGET="${PREFIX}/bin/mlc"
fi

if [ -f "${TARGET}" ]
then
sudo cp "${TARGET}" "${TARGET}.bak.$(date +%s)"
fi

sudo install -m 755 "${TMP}/mlc" "${TARGET}"
echo "Updated mlc -> $(${TARGET} -v 2>/dev/null || echo "${VERSION}")"