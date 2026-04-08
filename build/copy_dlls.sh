#!/usr/bin/env bash

EXE="$1"
if [ -z "$EXE" ]; then
    echo "Usage: $0 <pe.exe>"
    exit 1
fi

# Le dossier ou on va enregistrer les DLLs (le dossier ou est l'exécutable)
DEST=$(dirname "$(realpath "$EXE")")

# On récupère le sysroot de mingw (le dossier avec les DLLs)
SYSROOT=$(x86_64-w64-mingw32-gcc -print-sysroot)
MINGW_BIN="$SYSROOT/mingw/bin"

BUILTINS=(
    kernel32.dll
    msvcrt.dll
)

copy_dependencies() {
    local target="$1"
    
    local dlls=$(x86_64-w64-mingw32-objdump -p "$target" | grep "DLL Name" | awk '{print $3}' |  grep -ivE $BUILTINS)

    for dll in $dlls; do
        if [ -f "$MINGW_BIN/$dll" ]; then
            if [ ! -f "$DEST/$dll" ]; then
                cp "$MINGW_BIN/$dll" "$DEST/"
                echo "Copied: $dll -> $DEST/"
                
                copy_dependencies "$MINGW_BIN/$dll"
            fi
        fi
    done
}

echo "Analyzing $EXE..."
if [ -f "$EXE" ]; then
    copy_dependencies "$EXE"
    echo "Successfully moved dependencies to: $DEST"
else
    echo "Error: $EXE not found."
    exit 1
fi
