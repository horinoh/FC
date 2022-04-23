@pushd neslib
@cl65 -t nes -Oisr -c crt0.s
@del /q neslib.lib
@ar65 a neslib.lib crt0.o
@echo [ neslib.lib ]
@ar65 t neslib.lib
@popd

@pushd neslib2
@for %%i in (*.s) do @(cl65 --verbose -t nes -Oisr -g -c %%i)
@del /q neslib2.lib
@ar65 a neslib2.lib crt0.o lz4vram.o pad.o rand.o memfill.o vram_read.o vram_unrle.o oam_meta_spr_clip.o oam_meta_spr_pal.o oam_meta_spr.o oam_spr.o oam_clear_fast.o split.o splitxy.o
@echo [ neslib2.lib ]
@ar65 t neslib2.lib
@popd

@cl65 --verbose -t nes -Oisr -g -c famitone2.s

@pause