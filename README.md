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
    - 以下のようななバッチファイルでビルド
        ~~~
        @pushd neslib
        @cl65 -t nes -Oisr -c crt0.s
        @ar65 a neslib.lib crt0.o
        @popd
        ~~~
    - oam_off でエラーが出るので、ソースコードに以下の記述を追加
        ~~~
        #include <stdint.h>
        #pragma bss-name (push,"ZEROPAGE")
        #pragma data-name (push,"ZEROPAGE")
        uint8_t oam_off;
        #pragma data-name(pop)
        #pragma bss-name (pop)
    ~~~
 - [neslib2](https://github.com/sehugg/neslib.git) を git サブモジュールとして追加した
    - 以下のようなバッチファイルでビルド
        ~~~
        @pushd neslib2
        @for %%i in (*.s) do @(cl65 --verbose -t nes -Oisr -g -c %%i)
        @ar65 a neslib2.lib lz4vram.o pad.o rand.o memfill.o vram_read.o vram_unrle.o oam_meta_spr_clip.o oam_meta_spr_pal.o oam_meta_spr.o oam_spr.o oam_clear_fast.o split.o splitxy.o
        @popd
        ~~~

## バイナリデータ
 - XXX.s を用意し、アセンブル & リンクして使用
    ~~~
    ; XXX.s
    .segment "CHARS"
    .byte $00,$00,$00,$00,$00,$00,$00,$00
    ...
    ~~~
    - C から使えるようにする場合
        ~~~
        ; .s
        .export _hoge
        _hoge:
            .byte $00,$00,$00,$00,$00,$00,$00,$00
            ...
        ~~~
        ~~~
        // .c
        extern const uint8_t hoge[];
        ~~~
 - YYY.bin, YYY.s を用意し、アセンブル & リンクして使用
    ~~~
    ; YYY.s
    .segment "CHARS"
    .export _YYY
    _YYY:
    .incbin "YYY.bin"
    ~~~
    ~~~
    // .c
    extern const uint8_t YYY[];
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
 - ターミナル - タスクの実行 - 上記で作成したタスク名("Build FC"等)を選択して実行
