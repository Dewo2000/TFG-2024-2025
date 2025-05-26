@echo off
setlocal enabledelayedexpansion

:: Verificar que se pasó el argumento al .bat
if "%~1"=="" (
    echo Error: Debes proporcionar el argumento para el directorio principal.
    exit /b
)

:: Guardar el argumento pasado al .bat
set "arg=%~1"

:: Guardar el directorio de trabajo actual (el directorio donde se ejecuta el .bat)
set "current_dir=%cd%"

:: Iterar sobre todas las carpetas dentro de la carpeta actual
for /d %%i in (*) do (
    :: Comprobar si la carpeta contiene el script Jiwer_cer_2.py
    if exist "%%i\Jiwer_cer_2.py" (
        echo Cambiando al entorno de trabajo de %%i...
        
        :: Cambiar al directorio de la subcarpeta
        cd "%%i"

        echo Ejecutando Jiwer_cer_2.py en %%i con el argumento: ../%arg%/%%i
        
        :: Ejecutar el script Python dentro de la subcarpeta
        python "Jiwer_cer_2.py" "../%arg%/%%i"
        
        :: Volver al directorio de trabajo original
        cd "%current_dir%"
        
        echo Regresado al entorno del .bat
    )
)

echo Todos los scripts han sido ejecutados.
pause

