@pushd resource
@for %%i in (*.s) do @(cl65 -t nes -Oisr -c %%i)
@popd

@pause