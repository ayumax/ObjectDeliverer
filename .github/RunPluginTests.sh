#!/bin/bash
# プラグインテスト用スクリプト for Linux + UE5

# スクリプトの実行場所を取得
SCRIPT_DIR="$( cd "$( dirname "${BASH_SOURCE[0]}" )" &> /dev/null && pwd )"
PROJECT_DIR="$(dirname "$SCRIPT_DIR")"

# プラグイン名を設定
PLUGIN_NAME="ObjectDeliverer"
echo "Starting $PLUGIN_NAME plugin tests with UE5..."

# プラグイン名からプロジェクトファイル名を動的に生成
PROJECT_FILE="$PROJECT_DIR/${PLUGIN_NAME}Test.uproject"
if [ ! -f "$PROJECT_FILE" ]; then
    # 対応するuprojectファイルがない場合はプラグインファイルを使用
    PROJECT_FILE="$PROJECT_DIR/$PLUGIN_NAME.uplugin"
    echo "No ${PLUGIN_NAME}Test.uproject file found, using plugin file: $PROJECT_FILE"
else
    echo "Using project file: $PROJECT_FILE"
fi

# UE5でテストを実行
# UE4の -testexit="Automation Test Queue Empty" はUE5では -waitfortest に変更
UnrealEditor-Cmd "$PROJECT_FILE" \
    -ExecCmds="Automation RunTests $PLUGIN_NAME" \
    -unattended \
    -NullRHI \
    -nopause \
    -waitfortest \
    -log \
    -stdout

# テスト結果の終了コードを取得
TEST_RESULT=$?

# 結果のサマリーを出力
if [ $TEST_RESULT -eq 0 ]; then
    echo "Tests completed successfully!"
else
    echo "Tests failed with exit code: $TEST_RESULT"
fi

# テスト結果のログディレクトリを作成
mkdir -p "$PROJECT_DIR/TestResults"

# ログファイルを TestResults ディレクトリにコピー
if [ -d "$PROJECT_DIR/Saved/Logs" ]; then
    cp -r "$PROJECT_DIR/Saved/Logs"/* "$PROJECT_DIR/TestResults/"
    echo "Log files copied to TestResults directory"
fi

exit $TEST_RESULT