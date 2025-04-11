#!/bin/bash
# プラグインテスト用スクリプト for Linux + UE5

# スクリプトの実行場所を取得
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

# プラグイン名を設定
PLUGIN_NAME="ObjectDeliverer"
echo "Starting $PLUGIN_NAME plugin tests with UE5..."

# uprojectファイルのパスを明示的に指定
PROJECT_FILE="$PROJECT_DIR/ObjectDelivererTest.uproject"

if [ ! -f "$PROJECT_FILE" ]; then
    echo "ERROR: Could not find ObjectDelivererTest.uproject at: $PROJECT_FILE"
    ls -la "$PROJECT_DIR"
    exit 1
fi

echo "Using project file: $PROJECT_FILE"

# UnrealEditor-Cmd の場所を検索
UE_CMD=$(find /home/ue4/UnrealEngine -name "UnrealEditor-Cmd" -type f | head -1)

if [ -z "$UE_CMD" ]; then
    echo "ERROR: Could not find UnrealEditor-Cmd executable"
    exit 1
fi

echo "Using UnrealEditor-Cmd at: $UE_CMD"

# テスト結果のログディレクトリを作成
mkdir -p "$PROJECT_DIR/TestResults"
LOGFILE="$PROJECT_DIR/TestResults/AutomationTest.log"
REPORT_PATH="$PROJECT_DIR/TestResults"

# 公式ドキュメントに基づくテスト実行コマンド
# RunTest (単数形) を使用し、終了するために ;Quit を追加
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
    -ReportExportPath="$REPORT_PATH"

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
