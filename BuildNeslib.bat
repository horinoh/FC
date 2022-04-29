@echo [ neslib.lib ]
@pushd neslib
@del /q neslib.lib *.o
@cl65 -t nes -Oisr -c crt0.s
@for %%i in (*.o) do @(ar65 a neslib.lib %%i)
@ar65 t neslib.lib
@popd

@echo [ neslib2.lib ]
@pushd neslib2
@del /q neslib2.lib *.o
@for %%i in (*.s) do @(cl65 --verbose -t nes -Oisr -g -c %%i)
@for %%i in (*.o) do @(ar65 a neslib2.lib %%i)
@ar65 t neslib2.lib
@popd

@echo [ famitone ]
@del /q *.o
@for %%i in (*.s) do @(cl65 --verbose -t nes -Oisr -g -c %%i)

@pause