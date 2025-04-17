# ObjectDeliverer

[![master](https://github.com/ayumax/ObjectDeliverer/actions/workflows/unreal-plugin-test.yml/badge.svg?branch=master)](https://github.com/ayumax/ObjectDeliverer/actions/workflows/unreal-plugin-test.yml)
[![UE 5.5](https://github.com/ayumax/ObjectDeliverer/actions/workflows/unreal-plugin-test.yml/badge.svg?branch=UE5.5)](https://github.com/ayumax/ObjectDeliverer/actions/workflows/unreal-plugin-test.yml)
[![UE 5.4](https://github.com/ayumax/ObjectDeliverer/actions/workflows/unreal-plugin-test.yml/badge.svg?branch=UE5.4)](https://github.com/ayumax/ObjectDeliverer/actions/workflows/unreal-plugin-test.yml)
[![UE 5.3](https://github.com/ayumax/ObjectDeliverer/actions/workflows/unreal-plugin-test.yml/badge.svg?branch=UE5.3)](https://github.com/ayumax/ObjectDeliverer/actions/workflows/unreal-plugin-test.yml)

ObjectDelivererは、Unreal Engine向けの柔軟なデータ通信ライブラリです。C++とBlueprintの両方で利用可能です。

## 目次
- [概要](#概要)
- [サポートするUEバージョン](#サポートするueバージョン)
- [入手方法](#入手方法)
- [インストール手順](#インストール手順)
- [クイックスタート](#クイックスタート)
- [機能詳細](#機能詳細)
  - [通信プロトコル](#通信プロトコル)
  - [データ分割ルール](#データ分割ルール)
  - [シリアライズ方法](#シリアライズ方法)
- [使用例](#使用例)
  - [C++での使用例](#cでの使用例)
  - [Blueprintでの使用例](#blueprintでの使用例)
- [詳細なドキュメント](#詳細なドキュメント)
- [ライセンス](#ライセンス)
- [貢献方法](#貢献方法)

## 概要

ObjectDelivererは、特に以下の点で優れたデータ通信ライブラリです：

- **プロトコル切替が容易** - TCP/IP、UDP、共有メモリなど複数の通信プロトコルを簡単に切り替えられます
- **柔軟なデータ分割ルール** - 固定サイズ、ヘッダ＋ボディ、終端記号など様々なデータ分割ルールに対応
- **多様なシリアライズ方法** - バイト配列、UTF-8文字列、JSONオブジェクトなどに対応
- **C++とBlueprintの両方をサポート** - 好みの開発方法で利用可能

ネットワーク通信のロジックを簡素化し、アプリケーション開発に集中できるようになります。

## サポートするUEバージョン

| UEバージョン | サポート状況 | ブランチ名 |
|------------|------------|----------|
| UE 5.5     | ✅ サポート中 | UE5.5    |
| UE 5.4     | ✅ サポート中 | UE5.4    |
| UE 5.3     | ✅ サポート中 | UE5.3    |
| その他     | 🔄 要確認   | master   |

### ブランチ構成について

```
master (最新UEバージョン用) ──┐
                          ├─ UE5.5 (UE5.5専用)
                          ├─ UE5.4 (UE5.4専用)
                          └─ UE5.3 (UE5.3専用)
```

- **master**ブランチ: 常に最新バージョンのUnreal Engineに対応
- **UEX.X**ブランチ: 特定のUEバージョン向けの安定版
- **注意事項**: 古いバージョンのブランチには最新の機能が含まれていない場合があります

## 入手方法

### UE Marketplace

[https://www.fab.com/ja/listings/b6ffd7d7-80da-483f-a7fa-09cb46b72651](https://www.fab.com/ja/listings/b6ffd7d7-80da-483f-a7fa-09cb46b72651)

Marketplaceから入手する場合は、指定された料金が請求されます。

### GitHub

このリポジトリをクローンして無料で使用できます。

## インストール手順

### Marketplaceからの場合
1. Marketplaceからプラグインを購入・ダウンロード
2. UE Launcherの「ライブラリ」からプロジェクトに追加
3. プロジェクトを開き、「編集」→「プラグイン」からObjectDelivererを有効化

### GitHubからの場合
1. このリポジトリをクローン：`git clone https://github.com/ayumax/ObjectDeliverer.git`
2. クローンしたリポジトリの`Plugins`ディレクトリをプロジェクトフォルダにコピー
3. プラグインをビルド(C++コードをビルドする環境が必要です)
4. プロジェクトを開き、「編集」→「プラグイン」からObjectDelivererを有効化



### 互換性の確認
使用するUEバージョンに合わせて適切なブランチを選択してください：
```bash
# UE5.4を使用する場合の例
git checkout UE5.4
```

## クイックスタート

### 基本的な使用手順
1. ObjectDelivererManagerを作成
2. イベントハンドラを設定（接続、切断、データ受信）
3. 通信プロトコルとパケットルールを設定してスタート

![セットアップ例](https://user-images.githubusercontent.com/8191970/52522481-48075700-2cc9-11e9-92a0-067992f56042.png)

## 機能詳細

### 通信プロトコル
以下のプロトコルが標準で利用可能です（独自プロトコルの追加も可能）：

- **TCP/IPサーバー** - 複数クライアント接続対応
- **TCP/IPクライアント** - サーバーへの接続
- **UDP(送信側)** - UDPデータ送信
- **UDP(受信側)** - UDPデータ受信
- **共有メモリ** - Windows環境でのプロセス間通信
- **ファイル書き込み** - データのファイル保存
- **ファイル読み込み** - ファイルからのデータ読み込み

### データ分割ルール
受信データの適切なパケット分割と再構築のためのルール：

#### FixedSize（固定サイズ）
例）1024バイト固定の場合
![固定長](https://user-images.githubusercontent.com/8191970/56475737-7d999f00-64c7-11e9-8e9e-0182f1af8156.png)

#### Header(BodySize) + Body（ヘッダ＋ボディ）
例）サイズ領域が4バイトの場合
![サイズとボディ](https://user-images.githubusercontent.com/8191970/56475796-6e672100-64c8-11e9-8cf0-6524f2899be0.png)

#### Split by terminal symbol（終端記号による分割）
例）0x00を終端とする場合
![終端](https://user-images.githubusercontent.com/8191970/56475740-82f6e980-64c7-11e9-91a6-05d77cfdbd60.png)

#### No division（分割なし）
受信バッファをそのまま使用します。

### シリアライズ方法
- **バイト配列** - 生のバイナリデータ
- **UTF-8文字列** - テキストデータ
- **オブジェクト(JSON)** - 構造化データ

## 使用例

### C++での使用例

```cpp
void UMyClass::Start()
{
    auto deliverer = UObjectDelivererManager::CreateObjectDelivererManager();

    // イベントハンドラを設定
    deliverer->Connected.AddDynamic(this, &UMyClass::OnConnect);
    deliverer->Disconnected.AddDynamic(this, &UMyClass::OnDisConnect);
    deliverer->ReceiveData.AddDynamic(this, &UMyClass::OnReceive);

    // 通信開始
    // プロトコル：TCP/IPサーバー
    // データ分割ルール：ヘッダ(サイズ) + ボディ
    // シリアライズ方法：バイト配列
    deliverer->Start(UProtocolFactory::CreateProtocolTcpIpServer(9099),
                     UPacketRuleFactory::CreatePacketRuleSizeBody());
}

void UMyClass::OnConnect(UObjectDelivererProtocol* ClientSocket)
{
    // データ送信
    TArray<uint8> buffer;
    deliverer->Send(buffer);
}

void UMyClass::OnDisConnect(UObjectDelivererProtocol* ClientSocket)
{
    // 切断時処理
    UE_LOG(LogTemp, Log, TEXT("closed"));
}

void UMyClass::OnReceive(UObjectDelivererProtocol* ClientSocket, const TArray<uint8>& Buffer)
{
    // データ受信時処理
}
```

### Blueprintでの使用例

![Blueprint例](https://user-images.githubusercontent.com/8191970/52522481-48075700-2cc9-11e9-92a0-067992f56042.png)

## 詳細なドキュメント

各機能の詳細な使い方はWikiをご覧ください：

[ObjectDeliverer Wiki](https://github.com/ayumax/ObjectDeliverer/wiki)

## ライセンス

- このプラグインはMITライセンスで提供されています
- ただし、Epic Games Marketplaceからダウンロードして使用する場合は、Epic Gamesのライセンス条項が適用されます

## 貢献方法

ObjectDelivererの改善にご協力いただける方を歓迎します！

### 貢献の流れ

1. リポジトリをフォーク
2. `master`ブランチから機能ブランチを作成
3. 変更を加える
4. `master`ブランチを対象にプルリクエストを送信

### 貢献のガイドライン

- **すべてのプルリクエスト**は`master`ブランチに向けて作成してください
- 大きな変更を行う前には、まずIssueを開いて議論することをお勧めします(English or Japanese)
- コードスタイルは既存のコードベースに合わせてください
- テストを追加またはアップデートしてください

### サポートするUEバージョン

新しい機能を追加する場合は、できるだけ複数のUEバージョンで動作するように考慮してください。特定のUEバージョンでしか動作しない機能の場合は、その旨を明記してください。

バグ修正、機能追加、ドキュメント改善、提案など、あらゆる貢献を歓迎します！ 