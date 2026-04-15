#!/usr/bin/env bash

set -euo pipefail

DEPS=deps

if [ -d "$DEPS" ]; then
    echo Dependencies already exist, skipping...
    exit 0
fi

echo Preparing to clone deps...
sleep 1
mkdir -p "$DEPS"
pushd $DEPS

echo Cloning miniaudio...
git clone --depth 1 https://github.com/mackron/miniaudio.git > /dev/null

echo Cloning Lua 5.2.3...
git clone --depth 1 --branch v5.2.3 https://github.com/lua/lua.git > /dev/null

popd