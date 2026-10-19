#!/bin/bash
# ==============================================================================
# Script de lancement pour la réplique C++ Linux de Moktar
# ==============================================================================

SCRIPT_DIR="$(cd "$(dirname "${BASH_SOURCE[0]}")" && pwd)"
export LD_LIBRARY_PATH="${SCRIPT_DIR}/lib:${LD_LIBRARY_PATH}"
export SDL12COMPAT_SCALE_METHOD=nearest
export SDL12COMPAT_USE_GAME_CONTROLLERS=1
export SDL12COMPAT_SYNC_TO_VBLANK=1

# Détection du binaire Windows (.exe) ou Linux
if [[ "$OSTYPE" == "msys"* || "$OSTYPE" == "cygwin"* || "$OSTYPE" == "win32"* || -f "${SCRIPT_DIR}/bin/moktar.exe" ]]; then
    BIN_TARGET="${SCRIPT_DIR}/bin/moktar.exe"
else
    BIN_TARGET="${SCRIPT_DIR}/bin/moktar"
fi

# Si le binaire n'est pas encore compilé, lancer make
if [ ! -f "$BIN_TARGET" ]; then
    echo "[*] Compilation initiale du jeu Moktar..."
    make -C "${SCRIPT_DIR}" -j$(nproc 2>/dev/null || echo 2)
    if [ $? -ne 0 ]; then
        echo "[!] Erreur lors de la compilation."
        exit 1
    fi
fi

# Exécution du binaire avec tous les arguments transmis
exec "$BIN_TARGET" "$@"
