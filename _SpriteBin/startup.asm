.setcpu        "6502"
.autoimport    on
.IMPORTZP    sp

.global    _NesMain

.segment "HEADER"
    .byte   $4E, $45, $53, $1A 
    .byte   $02                
    .byte   $00                
    .byte   $01                
    .byte   $00                
    .byte   $00               
    .byte   $00,$00,$00,$00, $00, $00, $00

.segment "STARTUP"
.proc    Reset
    sei                
    ldx    #$ff        
    txs                

    lda    #$ff        
    sta    sp          
    lda    #$03       
    sta    sp + 1     

    jsr    _NesMain   
.endproc

.segment "VECINFO"    
    .word    $0000     
    .word    Reset     
    .word    $0000     