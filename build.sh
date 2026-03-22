#!/bin/bash
#
# Usage:
#   ./build.sh                                         # Build the default game (snake_game)
#   ./build.sh --game=game_name                        # Build a specific game
#   ./build.sh --os=linux (linux, mac, windows)        # Build for a specific target OS
#   ./build.sh --clean                                 # Remove the build directory before building

set -e # Exit immediately if a command exits with a non-zero status.

# --- Default Configuration ---
BUILD_DIR="build"
DEFAULT_GAME="snake_game"
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

case "$TARGET_OS" in mac|windows|linux)
        ;;
        *)
        echo "Unsupported or unknown target OS: '$TARGET_OS'"
        echo "Suported OS: mac, window, linux"
        exit 1
        ;;
esac

# Configure the project with CMake
echo "--- Configuring for game: $GAME_NAME (target OS: $TARGET_OS) ---"
cmake -B "$BUILD_DIR" -S "$SRC_DIR" -DGAME_NAME="$GAME_NAME" -DTARGET_OS="$TARGET_OS"

# Build the project
echo "--- Building project ---"
cmake --build "$BUILD_DIR"

echo
echo "Build complete!"
echo "Executable is located at: $BUILD_DIR/src/main_app"