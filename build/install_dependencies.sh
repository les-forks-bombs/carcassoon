#!/bin/bash
set -e

export DEBIAN_FRONTEND=noninteractive

dpkg --add-architecture i386
apt-get update
apt-get install -y --no-install-recommends \
    bear clang clang-format clang-tidy cmake curl doxygen git llvm make ninja-build \
    mingw-w64 python3-pip plantuml wine wine64 ca-certificates pkg-config xz-utils \
    libclang-rt-18-dev libfreetype-dev libharfbuzz-dev libasound2-dev libpulse-dev \
    libaudio-dev libjack-dev libsndio-dev libx11-dev libxext-dev libxrandr-dev \
    libxcursor-dev libxfixes-dev libxi-dev libxss-dev libxtst-dev libwayland-dev \
    wayland-protocols libxkbcommon-dev libdrm-dev libgbm-dev libgl1-mesa-dev \
    libgles2-mesa-dev libegl1-mesa-dev libvulkan-dev libdbus-1-dev libibus-1.0-dev \
    libudev-dev libusb-1.0-0-dev libfribidi-dev libthai-dev wine32 lld lld-18 node

sudo curl -Lo /usr/bin/appimagetool https://github.com/AppImage/appimagetool/releases/download/continuous/appimagetool-x86_64.AppImage
sudo chmod +x /usr/bin/appimagetool

sudo pip3 install --break-system-packages lcov_cobertura

cat << 'EOF' > /tmp/mingw64.cmake
set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_C_COMPILER x86_64-w64-mingw32-gcc)
set(CMAKE_CXX_COMPILER x86_64-w64-mingw32-g++)
set(CMAKE_RC_COMPILER x86_64-w64-mingw32-windres)
set(CMAKE_FIND_ROOT_PATH /usr/x86_64-w64-mingw32)
set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM NEVER)
set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)
EOF

sudo git clone https://github.com/emscripten-core/emsdk.git /opt/emsdk
cd /opt/emsdk
sudo ./emsdk install latest
sudo ./emsdk activate latest
source /opt/emsdk/emsdk_env.sh

# SDL3

git clone --depth 1 https://github.com/libsdl-org/SDL.git /tmp/SDL3
cd /tmp/SDL3

# Natif
cmake -B build-native -DCMAKE_BUILD_TYPE=Release -DSDL_TESTS=OFF -DSDL_EXAMPLES=OFF
cmake --build build-native --parallel $(nproc)
sudo cmake --install build-native

# Windows
cmake -B build-w64 -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=/tmp/mingw64.cmake -DCMAKE_INSTALL_PREFIX=/usr/x86_64-w64-mingw32 -DSDL_TESTS=OFF -DSDL_EXAMPLES=OFF
cmake --build build-w64 --parallel $(nproc)
sudo cmake --install build-w64

# WASM
emcmake cmake -B build-wasm -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/wasm32-unknown-emscripten -DSDL_TESTS=OFF -DSDL_EXAMPLES=OFF
cmake --build build-wasm --parallel $(nproc)
sudo cmake --install build-wasm

# Cleanup
cd /tmp
rm -rf /tmp/SDL3

# SDL3_image
git clone --depth 1 https://github.com/libsdl-org/SDL_image.git /tmp/SDL3_image
cd /tmp/SDL3_image

# Natif
cmake -B build-native -DCMAKE_BUILD_TYPE=Release -DSDLIMAGE_SAMPLES=OFF -DSDLIMAGE_TESTS=OFF
cmake --build build-native --parallel $(nproc)
sudo cmake --install build-native

# Windows
cmake -B build-w64 -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=/tmp/mingw64.cmake -DCMAKE_INSTALL_PREFIX=/usr/x86_64-w64-mingw32 -DSDLIMAGE_SAMPLES=OFF -DSDLIMAGE_TESTS=OFF
cmake --build build-w64 --parallel $(nproc)
sudo cmake --install build-w64

# WASM
emcmake cmake -B build-wasm -DCMAKE_BUILD_TYPE=Release -DSDL3_DIR=/usr/wasm32-unknown-emscripten/lib/cmake/SDL3 -DCMAKE_INSTALL_PREFIX=/usr/wasm32-unknown-emscripten -DSDLIMAGE_SAMPLES=OFF -DSDLIMAGE_TESTS=OFF
cmake --build build-wasm --parallel $(nproc)
sudo cmake --install build-wasm

# Cleanup
cd /tmp
rm -rf /tmp/SDL3_image

# SDL3_ttf
git clone --depth 1 https://github.com/libsdl-org/SDL_ttf.git /tmp/SDL3_ttf
cd /tmp/SDL3_ttf
sh external/download.sh

# Natif
cmake -B build-native -DCMAKE_BUILD_TYPE=Release -DSDLTTF_SAMPLES=OFF -DSDLTTF_TESTS=OFF
cmake --build build-native --parallel $(nproc)
sudo cmake --install build-native

# Windows
cmake -B build-w64 -DCMAKE_BUILD_TYPE=Release -DSDLTTF_VENDORED=ON -DCMAKE_TOOLCHAIN_FILE=/tmp/mingw64.cmake -DCMAKE_INSTALL_PREFIX=/usr/x86_64-w64-mingw32 -DSDLTTF_SAMPLES=OFF -DSDLTTF_TESTS=OFF
cmake --build build-w64 --parallel $(nproc)
sudo cmake --install build-w64

# WASM
emcmake cmake -B build-wasm -DCMAKE_BUILD_TYPE=Release -DSDLTTF_VENDORED=ON -DSDL3_DIR=/usr/wasm32-unknown-emscripten/lib/cmake/SDL3 -DCMAKE_INSTALL_PREFIX=/usr/wasm32-unknown-emscripten -DSDLTTF_SAMPLES=OFF -DSDLTTF_TESTS=OFF
cmake --build build-wasm --parallel $(nproc)
sudo cmake --install build-wasm

# Cleanup
cd /tmp
rm -rf /tmp/SDL3_ttf

# CMocka
git clone --depth 1 https://gitlab.com/cmocka/cmocka.git /tmp/cmocka
cd /tmp/cmocka

# Native
cmake -B build-native -DCMAKE_BUILD_TYPE=Release -DWITH_TESTING=OFF -DWITH_EXAMPLES=OFF
cmake --build build-native --parallel $(nproc)
sudo cmake --install build-native

# Windows
cmake -B build-w64 -DCMAKE_BUILD_TYPE=Release -DCMAKE_TOOLCHAIN_FILE=/tmp/mingw64.cmake -DCMAKE_INSTALL_PREFIX=/usr/x86_64-w64-mingw32 -DWITH_TESTING=OFF -DWITH_EXAMPLES=OFF
cmake --build build-w64 --parallel $(nproc)
sudo cmake --install build-w64

# WASM
emcmake cmake -B build-wasm -DCMAKE_BUILD_TYPE=Release -DCMAKE_INSTALL_PREFIX=/usr/wasm32-unknown-emscripten -DWITH_TESTING=OFF -DWITH_EXAMPLES=OFF
cmake --build build-wasm --parallel $(nproc)
sudo cmake --install build-wasm

# Cleanup
cd /tmp
rm -rf /tmp/cmocka

# Emscripten pre-compile

echo "int main() { return 0; }" > /tmp/dummy.c

# Lto thin
emcc /tmp/dummy.c -flto=thin -O3 \
     -s USE_ZLIB=1 -s USE_LIBPNG=1 -s USE_FREETYPE=1 -s USE_HARFBUZZ=1 \
     -o /tmp/dummy.js
rm -f /tmp/dummy.*

# Regular
echo "int main() { return 0; }" > /tmp/dummy.c
emcc /tmp/dummy.c -O3 \
     -s USE_ZLIB=1 -s USE_LIBPNG=1 -s USE_FREETYPE=1 -s USE_HARFBUZZ=1 \
     -o /tmp/dummy.js
rm -f /tmp/dummy.*
