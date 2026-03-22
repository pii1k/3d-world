#!/bin/bash
#
# Usage:
#   ./build.sh                                         # Build the default game for the host OS
#   ./build.sh --game=game_name                        # Build a specific game
#   ./build.sh --os=linux (linux, mac, window/windows) # Build for a specific target OS
#   ./build.sh --clean                                 # Remove the build directory before building

set -e # Exit immediately if a command exits with a non-zero status.

# --- Default Configuration ---
BUILD_DIR="build"
DEFAULT_GAME="bat_game"
SRC_DIR="."
GAME_NAME=$DEFAULT_GAME
CLEAN_BUILD=false
TARGET_OS=""

detectHostOs() {
    case "$(uname -s)" in
        Darwin)
            echo "mac"
            ;;
        Linux)
            echo "linux"
            ;;
        MINGW*|MSYS*|CYGWIN*)
            echo "windows"
            ;;
        *)
            echo ""
            ;;
    esac
}

# --- Parse Arguments ---
for arg in "$@"
do
    case $arg in
        --game=*)
        GAME_NAME="${arg#*=}"
        shift
        ;;
        --os=*)
        TARGET_OS="${arg#*=}"
        shift
        ;;
        --clean)
        CLEAN_BUILD=true
        shift
        ;;
        *)
        # a no-op for unknown arguments
        ;;
    esac
done

# --- Main Build Logic ---

# If --clean is passed, remove the build directory
if [ "$CLEAN_BUILD" = true ] && [ -d "$BUILD_DIR" ]; then
    echo "Cleaning build directory: $BUILD_DIR"
    rm -rf "$BUILD_DIR"
fi

if [ -z "$TARGET_OS" ]; then
    TARGET_OS="$(detectHostOs)"
fi

case "$TARGET_OS" in
    window)
        TARGET_OS="windows"
        ;;
esac

if [ -n "$TARGET_OS" ]; then
    case "$TARGET_OS" in
        mac|windows|linux)
            ;;
        *)
        echo "Unsupported or unknown target OS: '$TARGET_OS'"
        echo "Supported OS: mac, linux, window/windows"
        exit 1
        ;;
    esac
fi

CONFIGURE_PRESET="host-debug"
BUILD_PRESET="host-debug"

case "$TARGET_OS" in
    linux)
        CONFIGURE_PRESET="linux-debug"
        BUILD_PRESET="linux-debug"
        ;;
    mac)
        CONFIGURE_PRESET="mac-debug"
        BUILD_PRESET="mac-debug"
        ;;
    windows)
        CONFIGURE_PRESET="windows-debug"
        BUILD_PRESET="windows-debug"
        ;;
esac

# Configure the project with CMake
echo "--- Configuring for game: $GAME_NAME (preset: $CONFIGURE_PRESET) ---"
cmake -S "$SRC_DIR" --preset "$CONFIGURE_PRESET" -DGAME_NAME="$GAME_NAME"

# Build the project
echo "--- Building project (preset: $BUILD_PRESET) ---"
cmake --build --preset "$BUILD_PRESET"

echo
echo "Build complete!"
echo "Executable is located at: $BUILD_DIR/$BUILD_PRESET/src/main_app"
