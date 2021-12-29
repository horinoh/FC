; A, X, Y レジスタを持つ、それぞれサイズは 8 ビット
.setcpu        "6502"
.autoimport    on
.IMPORTZP    sp

.global    _NesMain

; iNESヘッダ(16 Bytes)
.segment "HEADER"
    .byte   $4E, $45, $53, $1A ; "NES" 
    .byte   $02                ; PRG-ROMバンク数(16KB x 2)
    .byte   $00                ; CHR-ROMバンク数(8KB x 0)
    .byte   $01                ; Mapper 0(low)/Vetrical Mirroring
    .byte   $00                ; Mapper 0(high)
    .byte   $00                ; PRG-RAMサイズ
    .byte   $00,$00,$00,$00, $00, $00, $00

.segment "STARTUP"
; リセット割り込み
.proc    Reset
    sei                ; IRQ割り込みを禁止
    ldx    #$ff        ; 0xff を X レジスタへ代入
    txs                ; X レジスタの内容を 6502 のスタックポインタへ代入

    lda    #$ff        ; 0xff を A レジスタへ代入
    sta    sp          ; A レジスタの内容を sp(C言語のスタックポインタ) へ代入
    lda    #$03        ; 0x03 を A レジスタへ代入
    sta    sp + 1      ; A レジスタの内容を sp + 1 へ代入

    jsr    _NesMain    ; 指定のサブルーチン (NesMain) 呼び出し
.endproc

; 割り込み処理時呼び出すアドレス (ここではリセット割込みだけを使用している)
.segment "VECINFO"     ; ベクタテーブル
    .word    $0000     ; NMI割り込みアドレス
    .word    Reset     ; リセット割り込みアドレス
    .word    $0000     ; IRQ割り込みアドレス