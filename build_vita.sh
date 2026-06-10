#!/bin/bash
# Build Wagic for PS Vita
#
# Usage:
#   ./build_vita.sh              # lightweight VPK (no card art)
#   ./build_vita.sh --full /path/to/card_images   # full VPK with card art
#   ./build_vita.sh --both /path/to/card_images   # build both VPKs

export VITASDK=/usr/local/vitasdk
export PATH="$VITASDK/bin:/usr/local/sbin:/usr/local/bin:/usr/sbin:/usr/bin:/sbin:/bin"

SRCDIR="/mnt/c/Users/Brendon Moncada/wagic_test"
BUILDDIR="$SRCDIR/build_vita"

BUILD_MODE="vpk"        # default: lightweight
CARD_IMAGES_DIR=""

# Parse arguments
while [[ $# -gt 0 ]]; do
    case "$1" in
        --full)
            BUILD_MODE="vpk-full"
            CARD_IMAGES_DIR="$2"
            shift 2
            ;;
        --both)
            BUILD_MODE="both"
            CARD_IMAGES_DIR="$2"
            shift 2
            ;;
        *)
            echo "Unknown option: $1"
            echo "Usage: $0 [--full /path/to/images] [--both /path/to/images]"
            exit 1
            ;;
    esac
done

# Validate card images dir when needed
if [ "$BUILD_MODE" != "vpk" ] && [ -z "$CARD_IMAGES_DIR" ]; then
    echo "ERROR: --full and --both require a card images directory path"
    echo "Usage: $0 --full /path/to/card_images"
    exit 1
fi

echo "Creating build directory"
rm -rf "$BUILDDIR"
mkdir -p "$BUILDDIR"
cd "$BUILDDIR"

echo "Running CMake"
CMAKE_ARGS="-DCMAKE_TOOLCHAIN_FILE=$VITASDK/share/vita.toolchain.cmake"
if [ -n "$CARD_IMAGES_DIR" ]; then
    CMAKE_ARGS="$CMAKE_ARGS -DCARD_IMAGES_DIR=$CARD_IMAGES_DIR"
fi
cmake $CMAKE_ARGS "$SRCDIR" 2>&1

if [ $? -ne 0 ]; then
    echo "CMake FAILED"
    exit 1
fi

echo "Building (make -j$(nproc))"

case "$BUILD_MODE" in
    vpk)
        make -j$(nproc) vpk 2>&1
        ;;
    vpk-full)
        make -j$(nproc) wagic.self-self 2>&1 && make vpk-full 2>&1
        ;;
    both)
        make -j$(nproc) vpk 2>&1 && make vpk-full 2>&1
        ;;
esac

if [ $? -ne 0 ]; then
    echo "Build FAILED"
    exit 1
fi

echo "Build SUCCEEDED"
ls -lh *.vpk 2>/dev/null || echo "No VPK found"
