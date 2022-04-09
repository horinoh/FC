@pushd neslib
@cl65 -t nes -Oisr -c crt0.s
@ar65 a neslib.lib crt0.o
@popd

@pushd neslib2
@for %%i in (*.s) do @(cl65 --verbose -t nes -Oisr -g -c %%i)
@ar65 a neslib2.lib lz4vram.o pad.o rand.o memfill.o vram_read.o vram_unrle.o oam_meta_spr_clip.o oam_meta_spr_pal.o oam_meta_spr.o oam_spr.o oam_clear_fast.o split.o splitxy.o
@popd

@pause