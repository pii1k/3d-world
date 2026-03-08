#!/bin/bash
#
# Usage:
#   ./build.sh                   # Build the default game (snake_game)
#   ./build.sh --game=game_name  # Build a specific game
#   ./build.sh --clean           # Remove the build directory before building
#

set -e # Exit immediately if a command exits with a non-zero status.

# --- Default Configuration ---
BUILD_DIR="build"
DEFAULT_GAME="snake_game"
SRC_DIR="."
GAME_NAME=$DEFAULT_GAME
CLEAN_BUILD=false

# --- Parse Arguments ---
for arg in "$@"
do
    case $arg in
        --game=*)
        GAME_NAME="${arg#*=}"
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

# Configure the project with CMake
echo "--- Configuring for game: $GAME_NAME ---"
cmake -B "$BUILD_DIR" -S "$SRC_DIR" -DGAME_NAME="$GAME_NAME"

# Build the project
echo "--- Building project ---"
cmake --build "$BUILD_DIR"

echo
echo "Build complete!"
echo "Executable is located at: $BUILD_DIR/src/main_app"
