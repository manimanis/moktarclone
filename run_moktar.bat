@echo off
setlocal

set "SCRIPT_DIR=%~dp0"
set "BIN_EXE=%SCRIPT_DIR%bin\moktar.exe"

:: Verifier si le binaire existe, sinon compiler
if not exist "%BIN_EXE%" (
    echo [*] Executable bin\moktar.exe non trouve, declenchement de la compilation...
    call "%SCRIPT_DIR%build_windows.bat"
    if errorlevel 1 (
        echo [ERREUR] Impossible de compiler Moktar.
        pause
        exit /b 1
    )
)

:: Configuration des variables SDL12-compat pour un rendu optimal
set "SDL12COMPAT_SCALE_METHOD=nearest"
set "SDL12COMPAT_USE_GAME_CONTROLLERS=1"
set "SDL12COMPAT_SYNC_TO_VBLANK=1"

:: Lancement du jeu avec tous les arguments passes
"%BIN_EXE%" %*
