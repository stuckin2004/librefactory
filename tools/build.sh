#!/usr/bin/env sh
set -e

# This weirdness ensures proper paths on *unix
SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
ROOT="$(cd "$SCRIPT_DIR/.." && pwd)"
SRC="$ROOT/src"
DEPS="$ROOT/deps"
OUT="$ROOT/out"

INC="-I$ROOT/include -I$DEPS/miniaudio -I$DEPS/lua"
DEFINES="-DLUA_USE_POSIX"
CFLAGS="-O2 -Wall $DEFINES $INC"

case "$(uname -s)" in
    Darwin) GL_LIBS="-framework OpenGL" ;;
    *)      GL_LIBS="-lGL" ;;
esac

mkdir -p "$OUT"

echo "Building Lua..."
for f in "$DEPS"/lua/*.c; do
    base="$(basename "$f" .c)"
    case "$base" in lua|luac) continue ;; esac
    clang $CFLAGS -c "$f" -o "$OUT/$base.o"
done

echo "Building game sources..."
for f in "$SRC"/*.cpp; do
    base="$(basename "$f" .cpp)"
    clang++ $CFLAGS -c "$f" -o "$OUT/$base.o"
done

echo "Linking..."
clang++ -o "$OUT/librefactory" "$OUT"/*.o \
    -lm -ldl $GL_LIBS -lminiaudio

echo "Done."