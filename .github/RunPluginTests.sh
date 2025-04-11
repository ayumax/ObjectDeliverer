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

# ステップ1: プロジェクトのビルド（テスト実行なし）
echo "Building project without running tests..."
"$BUILD_SCRIPT" ObjectDelivererTestEditor Linux Development "$PROJECT_FILE" -notools -ModuleWithSuffix=$PLUGIN_NAME,$PLUGIN_NAME -log="$BUILD_LOG"

# ビルド結果の確認
BUILD_RESULT=$?
if [ $BUILD_RESULT -ne 0 ]; then
    echo "Build failed with exit code: $BUILD_RESULT"
    cat "$BUILD_LOG"
    exit $BUILD_RESULT
fi

echo "Build completed successfully!"

# ステップ2: ObjectDelivererプラグインのテストのみを実行
echo "Running ObjectDeliverer plugin tests only..."
"$UE_CMD" "$PROJECT_FILE" \
    -ExecCmds="Automation RunTests $PLUGIN_NAME" \
    -unattended \
    -NullRHI \
    -nopause \
    -log \
    -stdout \
    -abslog="$LOGFILE" \
    -ReportExportPath="$PROJECT_DIR/TestResults"

# テスト結果の終了コードを取得
TEST_RESULT=$?

# 結果のサマリーを出力
if [ $TEST_RESULT -eq 0 ]; then
    echo "Tests completed successfully!"
else
    echo "Tests failed with exit code: $TEST_RESULT"
fi

# ログファイルを TestResults ディレクトリにコピー
if [ -d "$PROJECT_DIR/Saved/Logs" ]; then
    cp -r "$PROJECT_DIR/Saved/Logs"/* "$PROJECT_DIR/TestResults/"
    echo "Log files copied to TestResults directory"
fi

exit $TEST_RESULT
