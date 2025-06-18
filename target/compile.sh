#!/bin/bash

# Periksa apakah argumen IP address diberikan
if [ -z "$1" ]; then
    echo "Penggunaan: ./compile.sh <IP_ADDRESS>"
    echo "Contoh: ./compile.sh 127.0.0.1"
    exit 1
fi

IP_ADDRESS="$1"

echo "Mulai kompilasi dengan ADDRESS=\"$IP_ADDRESS\"..."

# Kompilasi program dengan mengganti nilai ADDRESS
gcc -o surf.exe \
    *.c \
    ../common/*.c \
    ./encryption/*.c \
    ./SURF/*.c \
    -DADDRESS="\"$IP_ADDRESS\"" \
    -I ../encryption/ \
    -I /mingw64/include/ \
    -L /mingw64/lib/ \
    -Wl,-Bstatic -lssl -lcrypto -largon2 \
    -Wl,-Bdynamic -lws2_32 -lbcrypt -lcrypt32 -lversion -lwinhttp \
    -static-libgcc -static-libstdc++

# Periksa apakah kompilasi berhasil
if [ $? -eq 0 ]; then
    echo "Kompilasi berhasil! File surf.exe telah dibuat."
else
    echo "Kompilasi gagal. Silakan periksa pesan kesalahan di atas."
fi