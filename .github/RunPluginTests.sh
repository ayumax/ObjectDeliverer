#!/bin/bash
# Plugin test script for Linux + UE5

# Get the script execution directory
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

# Set the plugin name
PLUGIN_NAME="ObjectDeliverer"
echo "Starting $PLUGIN_NAME plugin tests with UE5..."

# Specify the uproject file path
PROJECT_FILE="$PROJECT_DIR/ObjectDelivererTest.uproject"
PLUGIN_FILE="$PROJECT_DIR/Plugins/$PLUGIN_NAME/$PLUGIN_NAME.uplugin"

if [ ! -f "$PROJECT_FILE" ]; then
    echo "ERROR: Could not find ObjectDelivererTest.uproject at: $PROJECT_FILE"
    ls -la "$PROJECT_DIR"
    exit 1
fi

echo "Using project file: $PROJECT_FILE"

UE_ENGINE_DIR="$HOME/UnrealEngine"
UE_BUILD_CMD="$UE_ENGINE_DIR/Engine/Build/BatchFiles/RunUAT.sh"
UE_TEST_CMD="$UE_ENGINE_DIR/Engine/Binaries/Linux/UnrealEditor-Cmd"
UE_GENERATE_CMD="$UE_ENGINE_DIR/Engine/Binaries/Linux/UnrealEditor-Cmd"

# Clean up existing build artifacts
echo "Cleaning up previous build artifacts..."
rm -rf "$PROJECT_DIR/Binaries"
rm -rf "$PROJECT_DIR/Intermediate"
rm -rf "$PROJECT_DIR/Plugins/$PLUGIN_NAME/Binaries"
rm -rf "$PROJECT_DIR/Plugins/$PLUGIN_NAME/Intermediate"

# First, generate project files
echo "Generating project files..."
"$UE_GENERATE_CMD" -projectfiles -project="$PROJECT_FILE" -game -engine -DDC-ForceMemoryCache

# Build the plugin
echo "Building $PLUGIN_NAME plugin..."
"$UE_BUILD_CMD" BuildPlugin -Plugin="$PLUGIN_FILE" -Package="$PROJECT_DIR/Plugins/$PLUGIN_NAME" -TargetPlatforms=Linux

# Build the project - using standard UBT
echo "Building project with $PLUGIN_NAME plugin..."
"$UE_ENGINE_DIR/Engine/Build/BatchFiles/Linux/Build.sh" ObjectDelivererTestEditor Linux Development -Project="$PROJECT_FILE" -TargetType=Editor

# Create a directory for test results logs
mkdir -p "$PROJECT_DIR/TestResults"
LOGFILE="$PROJECT_DIR/TestResults/AutomationTest.log"

# Set variables for test execution
AUTOMATION_FLAGS="-unattended -NullRHI -nosound -nopause -log -stdout -DDC-ForceMemoryCache"
AUTOMATED_TESTS_TO_RUN="$PLUGIN_NAME"
AUTOMATION_CMDS="-ExecCmds=\"Automation RunTests $AUTOMATED_TESTS_TO_RUN; quit\""
AUTOMATION_EXIT="-TestExit=\"Automation Test Queue Empty\""

# Run the ObjectDeliverer plugin tests
echo "Running ObjectDeliverer plugin tests..."
eval "$UE_TEST_CMD $PROJECT_FILE $AUTOMATION_FLAGS $AUTOMATION_CMDS $AUTOMATION_EXIT -abslog=$LOGFILE -ReportExportPath=$PROJECT_DIR/TestResults"

# Get the exit code from the test results
TEST_RESULT=$?

# 結果のサマリーを出力
if [ $TEST_RESULT -eq 0 ]; then
    echo "Tests completed successfully!"
else
    echo "Tests failed with exit code: $TEST_RESULT"
    
    # エラーが発生した場合、ログファイルの最後の部分を表示
    if [ -f "$LOGFILE" ]; then
        echo "=== Last 50 lines of log file ==="
        tail -n 50 "$LOGFILE"
        echo "=================================="
    fi
fi

# Copy log files to the TestResults directory
if [ -d "$PROJECT_DIR/Saved/Logs" ]; then
    cp -r "$PROJECT_DIR/Saved/Logs"/* "$PROJECT_DIR/TestResults/"
    echo "Log files copied to TestResults directory"
fi

exit $TEST_RESULT
