# FC

- 参考
    - [NESDEV](https://www.nesdev.org/)
    - [8bitworkshop](https://8bitworkshop.com/v3.9.0/?file=hello.c&platform=nes)
    
## ビルド
<!--
- [コンパイラ(cc65)](https://cc65.github.io/)
    - 解凍して cc65-snapshot-win32 を Cドライブ直下に配置
    - C:\cc65-snapshot-win32\bin\ を環境変数 Path に通しておく
    - ビルド
        ~~~
        $cl65 -t none -c startup.asm
        $cl65 -t none -c -O *.c
        $ld65 -o nes.rom --config build.cfg --obj *.o --lib nes.lib
        ~~~
-->

### neslib
<!--
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
-->
 - [neslib](https://github.com/sehugg/neslib.git) を git サブモジュールとして追加した
    - 以下のようなバッチファイルでビルド
        ~~~
        @pushd neslib
        @del /q neslib.lib *.o
        @for %%i in (*.s) do @(cl65 --verbose -t nes -Oisr -g -c %%i)
        @for %%i in (*.o) do @(ar65 a neslib2.lib %%i)
        @ar65 t neslib.lib
        @popd
        ~~~
## Famitone2
 - [Famitone2](https://shiru.untergrund.net/code.shtml)
 
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
    $nestopia .\main.nes
    ~~~

    - 初期設定のキー配置
        - 方向 = Arrow Key
        - B, A = Commna(,), Dot(.)
        - セレクト, スタート = RightShift, Enter

- [エミュレータ(fceux)](https://fceux.com/web/home.html)
    - C:\qfceux-2.6.4-win64\bin を環境変数 Path に通しておく
    ~~~
    $qfceux main.nes
    ~~~

    - 初期設定のキー配置
        - 方向 = Arrow Key
        - B, A = D, F
        - セレクト, スタート = S, Enter

## VS Code からのビルド、実行
 - ファイル - フォルダを開く で対象のフォルダを開く
 - ターミナル - タスクの構成 - テンプレートから tasks.json を生成 - Others - tasks.json を編集する
 - ターミナル - タスクの実行 - 上記で作成したタスク名("Build FC"等)を選択して実行
