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

# Try to find the UnrealEditor-Cmd executable in various possible locations
UE_CMD=""
POSSIBLE_PATHS=(
    "/home/ue5/UnrealEngine/Engine/Binaries/Linux/UnrealEditor-Cmd"
    "/opt/unreal/Engine/Binaries/Linux/UnrealEditor-Cmd"
    "/UnrealEngine/Engine/Binaries/Linux/UnrealEditor-Cmd"
    "/Engine/Binaries/Linux/UnrealEditor-Cmd"
    "/home/ue4/UnrealEngine/Engine/Binaries/Linux/UnrealEditor-Cmd"
)

for CMD_PATH in "${POSSIBLE_PATHS[@]}"; do
    echo "Checking for UnrealEditor-Cmd at: $CMD_PATH"
    if [ -f "$CMD_PATH" ]; then
        UE_CMD="$CMD_PATH"
        echo "Found UnrealEditor-Cmd at: $UE_CMD"
        break
    fi
done

# If we didn't find it in the known locations, try to search for it
if [ -z "$UE_CMD" ]; then
    echo "Searching for UnrealEditor-Cmd in the filesystem..."
    UE_CMD=$(find / -name "UnrealEditor-Cmd" -type f 2>/dev/null | head -1)
    
    if [ -n "$UE_CMD" ]; then
        echo "Found UnrealEditor-Cmd at: $UE_CMD"
    else
        echo "ERROR: Could not find UnrealEditor-Cmd executable. Please specify the correct path."
        exit 1
    fi
fi

# Create directories if needed
mkdir -p "$PROJECT_DIR/Saved/Logs"

# Run the test using the found UnrealEditor-Cmd path with DDC-ForceMemoryCache option
echo "Running tests with: $UE_CMD"
"$UE_CMD" "$PROJECT_FILE" \
    -ExecCmds="Automation RunTests $PLUGIN_NAME" \
    -unattended \
    -NullRHI \
    -nopause \
    -waitfortest \
    -log \
    -stdout \
    -DDC-ForceMemoryCache \
    -abslog="$PROJECT_DIR/Saved/Logs/AutomationTest.log"

# Get test result exit code
TEST_RESULT=$?

# Output result summary
if [ $TEST_RESULT -eq 0 ]; then
    echo "Tests completed successfully!"
else
    echo "Tests failed with exit code: $TEST_RESULT"
    echo "See logs for details at: $PROJECT_DIR/Saved/Logs/AutomationTest.log"
    
    # Display the end of the log file for quick diagnosis
    if [ -f "$PROJECT_DIR/Saved/Logs/AutomationTest.log" ]; then
        echo "Last 50 lines of log:"
        tail -n 50 "$PROJECT_DIR/Saved/Logs/AutomationTest.log"
    fi
fi

# Create test results directory
mkdir -p "$PROJECT_DIR/TestResults"

# Copy log files to TestResults directory
if [ -d "$PROJECT_DIR/Saved/Logs" ]; then
    cp -r "$PROJECT_DIR/Saved/Logs"/* "$PROJECT_DIR/TestResults/"
    echo "Log files copied to TestResults directory"
fi

exit $TEST_RESULT
