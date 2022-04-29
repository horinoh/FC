@echo [ bank ]
@pushd bank
@del /q *.o
@for %%i in (*.s) do @(cl65 --verbose -t nes -Oisr -g -c %%i)
@popd

@pause