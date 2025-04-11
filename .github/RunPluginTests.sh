#!/bin/bash
# Update the script to run with UE5.5 Docker image

# Get script directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

# Set plugin name
PLUGIN_NAME="ObjectDeliverer"
echo "Starting $PLUGIN_NAME plugin tests with UE5.5..."

# Generate project file name from plugin name
PROJECT_FILE="$PROJECT_DIR/${PLUGIN_NAME}Test.uproject"
if [ ! -f "$PROJECT_FILE" ]; then
    # Use plugin file if uproject doesn't exist
    PROJECT_FILE="$PROJECT_DIR/$PLUGIN_NAME.uplugin"
    echo "No ${PLUGIN_NAME}Test.uproject file found, using plugin file: $PROJECT_FILE"
else
    echo "Using project file: $PROJECT_FILE"
fi

# Use the Engine path from Docker image
# Adjust UE_ENGINE_DIR to match Docker image structure
UE_ENGINE_DIR="/Engine"
if [ ! -d "$UE_ENGINE_DIR" ]; then
    echo "Engine directory not found at $UE_ENGINE_DIR"
    # Try alternate location
    UE_ENGINE_DIR="/opt/unreal-engine/Engine"
    if [ ! -d "$UE_ENGINE_DIR" ]; then
        echo "Engine directory not found at alternate location $UE_ENGINE_DIR"
        exit 1
    fi
fi

# Run the test using the full path to UnrealEditor-Cmd
"$UE_ENGINE_DIR/Binaries/Linux/UnrealEditor-Cmd" "$PROJECT_FILE" \
    -ExecCmds="Automation RunTests $PLUGIN_NAME" \
    -unattended \
    -NullRHI \
    -nopause \
    -waitfortest \
    -log \
    -stdout

# Get test result exit code
TEST_RESULT=$?

# Output result summary
if [ $TEST_RESULT -eq 0 ]; then
    echo "Tests completed successfully!"
else
    echo "Tests failed with exit code: $TEST_RESULT"
fi

# Create test results directory
mkdir -p "$PROJECT_DIR/TestResults"

# Copy log files to TestResults directory
if [ -d "$PROJECT_DIR/Saved/Logs" ]; then
    cp -r "$PROJECT_DIR/Saved/Logs"/* "$PROJECT_DIR/TestResults/"
    echo "Log files copied to TestResults directory"
fi

exit $TEST_RESULT
