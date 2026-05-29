#!/usr/bin/env bash

EXE="$1"
if [ -z "$EXE" ]; then
    echo "Usage: $0 <pe.exe>"
    exit 1
fi

# Le dossier ou on va enregistrer les DLLs (le dossier ou est l'exécutable)
DEST=$(dirname "$(realpath "$EXE")")

# On récupère le sysroot de mingw (le dossier avec les DLLs)
SYSROOT="/usr/x86_64-w64-mingw32"
MINGW_BIN="$SYSROOT/bin"

BUILTINS="^(kernel32|msvcrt|user32|gdi32|advapi32|shell32|ole32|ws2_32|setupapi|imm32|oleaut32|version|winmm|shlwapi|rpcrt4|comdlg32|ucrtbase)\.dll$"

copy_dependencies() {
    local target="$1"
    
    local dlls=$(x86_64-w64-mingw32-objdump -p "$target" | grep "DLL Name" | awk '{print $3}' |  grep -ivE $BUILTINS)

    for dll in $dlls; do
        if [ -f "$MINGW_BIN/$dll" ]; then
            if [ ! -f "$DEST/$dll" ]; then
                cp "$MINGW_BIN/$dll" "$DEST/"
                
                copy_dependencies "$MINGW_BIN/$dll"
            fi
        fi
    done
}

if [ -f "$EXE" ]; then
    copy_dependencies "$EXE"
else
    echo "Error: $EXE not found."
    exit 1
fi
