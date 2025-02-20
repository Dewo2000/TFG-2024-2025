@echo off
for /L %%i in (1,1,13) do (
    if exist %%i\ (
	del %%i\merged.txt
        copy /b %%i\*.txt %%i.txt
    )
)
echo Proceso completado.
pause

