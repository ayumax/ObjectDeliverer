#!/bin/bash
# プラグインテスト用スクリプト for Linux + UE5

# スクリプトの実行場所を取得
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

# プラグイン名を設定
PLUGIN_NAME="ObjectDeliverer"
echo "Starting $PLUGIN_NAME plugin tests with UE5..."

# プラグインファイルのパスを指定
PLUGIN_FILE="$PROJECT_DIR/Plugins/$PLUGIN_NAME/$PLUGIN_NAME.uplugin"
if [ ! -f "$PLUGIN_FILE" ]; then
    # 別の場所を確認
    PLUGIN_FILE="$PROJECT_DIR/$PLUGIN_NAME.uplugin"
    if [ ! -f "$PLUGIN_FILE" ]; then
        echo "ERROR: Could not find plugin file at: $PLUGIN_FILE"
        exit 1
    fi
fi

echo "Using plugin file: $PLUGIN_FILE"

# UE5でテストを実行
# -RunAutomationTest は特定のプラグインのテストを実行するために使用
UE_CMD="UnrealEditor-Cmd"

# テスト結果のログディレクトリを作成
mkdir -p "$PROJECT_DIR/TestResults"
LOGFILE="$PROJECT_DIR/TestResults/AutomationTest.log"

# プラグインテスト実行
"$UE_CMD" \
    "$PLUGIN_FILE" \
    -ExecCmds="Automation RunTests $PLUGIN_NAME" \
    -unattended \
    -NullRHI \
    -nopause \
    -waitfortest \
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
