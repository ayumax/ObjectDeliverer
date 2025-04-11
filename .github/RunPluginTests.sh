#!/bin/bash
# プラグインテスト用スクリプト for Linux + UE5

# スクリプトの実行場所を取得
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

# プラグイン名を設定
PLUGIN_NAME="ObjectDeliverer"
echo "Starting $PLUGIN_NAME plugin tests with UE5..."

# uprojectファイルのパスを指定
PROJECT_FILE="$PROJECT_DIR/ObjectDelivererTest.uproject"

if [ ! -f "$PROJECT_FILE" ]; then
    echo "ERROR: Could not find ObjectDelivererTest.uproject at: $PROJECT_FILE"
    ls -la "$PROJECT_DIR"
    exit 1
fi

echo "Using project file: $PROJECT_FILE"

# UnrealEngine関連のコマンドの場所を検索
UE_DIR="/home/ue4/UnrealEngine"
UE_CMD=$(find "$UE_DIR" -name "UnrealEditor-Cmd" -type f | head -1)
BUILD_SCRIPT=$(find "$UE_DIR" -name "Build.sh" -type f | grep -i "Linux/Build.sh" | head -1)

if [ -z "$UE_CMD" ]; then
    echo "ERROR: Could not find UnrealEditor-Cmd executable"
    exit 1
fi

if [ -z "$BUILD_SCRIPT" ]; then
    echo "ERROR: Could not find Build.sh script"
    exit 1
fi

echo "Using UnrealEditor-Cmd at: $UE_CMD"
echo "Using Build.sh at: $BUILD_SCRIPT"

# テスト結果のログディレクトリを作成
mkdir -p "$PROJECT_DIR/TestResults"
LOGFILE="$PROJECT_DIR/TestResults/AutomationTest.log"
BUILD_LOG="$PROJECT_DIR/TestResults/BuildLog.log"

echo "=== Building project (including plugin) for Linux ==="
# プロジェクトをビルド（プラグインも一緒にビルドされる）
chmod +x "$BUILD_SCRIPT"

PROJECT_NAME=$(basename "$PROJECT_FILE" .uproject)
"$BUILD_SCRIPT" \
    "${PROJECT_NAME}Editor" Linux Development \
    -Project="$PROJECT_FILE" \
    -TargetType=Editor > "$BUILD_LOG" 2>&1
    
BUILD_RESULT=$?

if [ $BUILD_RESULT -ne 0 ]; then
    echo "Build failed with code: $BUILD_RESULT"
    echo "Last 30 lines of build log:"
    tail -n 30 "$BUILD_LOG"
    exit $BUILD_RESULT
else
    echo "Build completed successfully!"
fi

echo "=== Running plugin tests ==="
# テスト実行
"$UE_CMD" \
    "$PROJECT_FILE" \
    -ExecCmds="Automation RunTest $PLUGIN_NAME;Quit" \
    -unattended \
    -NullRHI \
    -nopause \
    -log \
    -stdout \
    -DDC-ForceMemoryCache \
    -abslog="$LOGFILE" \
    -ReportExportPath="$PROJECT_DIR/TestResults"

# テスト結果の終了コードを取得
TEST_RESULT=$?

# 結果のサマリーを出力
if [ $TEST_RESULT -eq 0 ]; then
    echo "Tests completed successfully!"
else
    echo "Tests failed with exit code: $TEST_RESULT"
    
    # エラーの詳細を表示
    if [ -f "$LOGFILE" ]; then
        echo "Last 30 lines of log:"
        tail -n 30 "$LOGFILE"
    fi
fi

exit $TEST_RESULT
