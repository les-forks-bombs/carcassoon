#!/usr/bin/env bash

EXE="$1"
if [ -z "$EXE" ]; then
    echo "Usage: $0 <pe.exe>"
    exit 1
fi

# Le dossier ou on va enregistrer les DLLs (le dossier ou est l'exécutable)
DEST=$(dirname "$(realpath "$EXE")")

GCC_LIB_DIR=$(dirname "$(x86_64-w64-mingw32-gcc -print-libgcc-file-name)")
SEARCH_PATHS=(
    "/usr/x86_64-w64-mingw32/bin"
    "$GCC_LIB_DIR"
)

BUILTINS="^(kernel32|msvcrt|user32|gdi32|advapi32|shell32|ole32|ws2_32|setupapi|imm32|oleaut32|version|winmm|shlwapi|rpcrt4|comdlg32|ucrtbase|ntdll)\.dll$"

copy_dependencies() {
    local target="$1"
    local dlls=$(x86_64-w64-mingw32-objdump -p "$target" 2>/dev/null | grep "DLL Name" | awk '{print $3}' | grep -ivE "$BUILTINS")

    for dll in $dlls; do
        if [ ! -f "$DEST/$dll" ]; then
            local found=0
            
            for path in "${SEARCH_PATHS[@]}"; do
                if [ -f "$path/$dll" ]; then
                    echo "Copying $dll from $path..."
                    cp "$path/$dll" "$DEST/"
                    found=1
                    copy_dependencies "$path/$dll"
                    break
                fi
            done
            
            if [ $found -eq 0 ]; then
                echo "Warning: Could not find $dll in search paths."
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