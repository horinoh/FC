@REM echo [ neslib.lib ]
@REM pushd neslib
@REM del /q neslib.lib *.o
@REM popd

@echo [ neslib.lib ]
@pushd neslib
@del /q neslib.lib *.o > nul 2>&1
@popd

@echo [ famitone ]
@del /q *.o > nul 2>&1

@pause