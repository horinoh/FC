@REM echo [ neslib.lib ]
@REM pushd neslib
@REM del /q neslib.lib *.o
@REM cl65 -t nes -Oisr -c crt0.s
@REM for %%i in (*.o) do @(ar65 a neslib.lib %%i)
@REM ar65 t neslib.lib
@REM popd

@echo [ neslib.lib ]
@pushd neslib
@del /q neslib.lib *.o
@for %%i in (*.s) do @(cl65 --verbose -t nes -Oisr -g -c %%i)
@for %%i in (*.o) do @(ar65 a neslib.lib %%i)
@ar65 t neslib.lib
@popd

@echo [ famitone ]
@del /q *.o
@for %%i in (*.s) do @(cl65 --verbose -t nes -Oisr -g -c %%i)

@pause