# FC

- [NESDEV](https://www.nesdev.org/)

## ビルド
- [コンパイラ(cc65)](https://cc65.github.io/)
    - 解凍して cc65-snapshot-win32 を Cドライブ直下に配置
    - C:\cc65-snapshot-win32\bin\ を環境変数 Path に通しておく
    - ビルド
        ~~~
        $cl65 -t none -c startup.asm
        $cl65 -t none -c -O *.c
        $ld65 -o nes.rom --config build.cfg --obj *.o --lib nes.lib
        ~~~

## 実行
- [エミュレータ(nestopia)](http://nestopia.sourceforge.net/)
    - C:\Nestopia140bin を環境変数 Path に通しておく
    ~~~
    $nestopia .\nes.rom
    ~~~

- 初期設定のキー配置
    - 方向 = Arrow Key
    - B, A = Commna(,), Dot(.)
    - セレクト, スタート = RightShift, Enter

## VS Code からのビルド、実行
 - ファイル - フォルダを開く で対象のフォルダを開く
 - ターミナル - タスクの構成 - テンプレートから tasks.json を生成 - Others - tasks.json を編集する
    ~~~
    "version": "2.0.0",
    "tasks": [
        {
            "label": "Asm FC",
            "type": "shell",
            "command": "cl65 -t none -c startup.asm"
        },
        {
            "label": "Compile FC",
            "type": "shell",
            "command": "cl65 -t none -c -O main.c"
        },
        {
            "label": "Link FC",
            "type": "shell",
            "command": "ld65 -o nes.rom --config build.cfg --obj startup.o main.o --lib nes.lib"
        },
        {
            "label": "Execute FC",
            "type": "shell",
            "command": "nestopia .\\nes.rom"
        },
        {
            "label": "Build FC",
            "type": "shell",
            "dependsOrder": "sequence",
            "dependsOn": [
                "Asm FC",
                "Compile FC",
                "Link FC"
            ]
        },
        {
            "label": "Build and execute FC",
            "type": "shell",
            "dependsOrder": "sequence",
            "dependsOn": [
                "Build FC",
                "Execute FC"
            ]
        }
    ]
    ~~~
 - ターミナル - タスクの実行 - 上記で作成したタスク名("Build FC"等)を選択して実行
