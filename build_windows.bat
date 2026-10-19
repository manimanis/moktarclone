@echo off
setlocal

echo ====================================================================
echo  Compilation de Moktar pour Windows (MSYS2 / UCRT64)
echo ====================================================================

:: Detection de msys2_shell.cmd
set "MSYS2_SHELL="
if exist "C:\MSYS2\msys2_shell.cmd" (
    set "MSYS2_SHELL=C:\MSYS2\msys2_shell.cmd"
) else if exist "C:\msys64\msys2_shell.cmd" (
    set "MSYS2_SHELL=C:\msys64\msys2_shell.cmd"
) else if defined MSYS2_ROOT if exist "%MSYS2_ROOT%\msys2_shell.cmd" (
    set "MSYS2_SHELL=%MSYS2_ROOT%\msys2_shell.cmd"
)

if not defined MSYS2_SHELL (
    echo [ERREUR] Impossible de trouver msys2_shell.cmd.
    echo Verifiez que MSYS2 est bien installe dans C:\MSYS2 ou C:\msys64.
    exit /b 1
)

echo [*] Utilisation de MSYS2 : %MSYS2_SHELL%
echo [*] Lancement de la compilation avec UCRT64...

call "%MSYS2_SHELL%" -defterm -here -no-start -ucrt64 -c "make -j%NUMBER_OF_PROCESSORS%"

if %ERRORLEVEL% equ 0 (
    echo ====================================================================
    echo  Compilation terminee avec succes !
    echo  Executable : bin\moktar.exe
    echo  Vous pouvez lancer le jeu avec : run_moktar.bat
    echo ====================================================================
) else (
    echo ====================================================================
    echo [ERREUR] La compilation a echoue.
    echo ====================================================================
    exit /b %ERRORLEVEL%
)
