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
### neslib を使用する場合
 - [neslib](https://github.com/clbr/neslib) を git サブモジュールとして追加した
    - neslib/crt0.s をアセンブル & リンクして使用する
 - oam_off でエラーが出るので、ソースコードに以下の記述を追加
    ~~~
    #include <stdint.h>
    #pragma bss-name (push,"ZEROPAGE")
    #pragma data-name (push,"ZEROPAGE")
    uint8_t oam_off;
    #pragma data-name(pop)
    #pragma bss-name (pop)
    ~~~
 - コンフィグファイルに以下の記述を追加
    ~~~
    FEATURES {
       CONDES:
            segment = INIT,
	        type = constructor,
	        label = __CONSTRUCTOR_TABLE__,
	        count = __CONSTRUCTOR_COUNT__;
        CONDES: 
            segment = RODATA,
	        type = destructor,
	        label = __DESTRUCTOR_TABLE__,
	        count = __DESTRUCTOR_COUNT__;
    }
    ~~~

## バイナリデータ
- コンフィグファイルに CHARS エントリを追加
    ~~~
    MEMORY {
        CHR: 		start = $0000, size = $2000, file = %O, fill = yes;
    }
    SEGMENTS {   
        CHARS:    load = CHR,            type = rw;
    }
    ~~~
 - バイナリデータ (XXX.s) を用意し。アセンブル & リンクして使用
    ~~~
    .segment "CHARS"
    .byte $00,$00,$00,$00,$00,$00,$00,$00
    ...
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
