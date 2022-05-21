.segment "XRAM"

.segment "PBANK00" ; 0x8000 ; バンク切替番号(0)
.segment "PBANK01" ; 0x8000 ; バンク切替番号(1)
.segment "PBANK02" ; 0x8000 ; バンク切替番号(2)
.segment "PBANK03" ; 0xA000 ; バンク切替番号(3)
.segment "PBANK04" ; 0xA000 ; バンク切替番号(4)
.segment "PBANK05" ; 0xA000 ; バンク切替番号(5)

; 直前までのデータサイズによってバンク切替番号(オフセット)が決まる
.segment "CBANK00" ; バンク切替番号(0) 
  .incbin	"../resource/ascii.chr" ; 8KB (0x2000, 256 tiles)
.segment "CBANK01" ; バンク切替番号(8)
  .incbin	"../resource/character.chr" ; 8KB
.segment "CBANK02" ; バンク切替番号(16 = 8 + 8) 
  .incbin	"../resource/jroatch.chr" ; 4KB 
.segment "CBANK03" ; バンク切替番号(20 = 8 + 8 + 4)
  .incbin	"../resource/bg.chr" ; 4KB ... アルファベットは大文字のみ
.segment "CBANK04"
.segment "CBANK05"
.segment "CBANK06" 
.segment "CBANK07"
 
