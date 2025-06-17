# FREE-ROBUX Remote Access Tool

Sebuah aplikasi remote access (akses jarak jauh) yang ditulis dalam bahasa C dengan kemampuan operasi sistem file, enkripsi, dan fitur kontrol jarak jauh.

## 🛠️ Struktur Proyek

```plaintext
FREE-ROBUX/
├── common/             # Fungsi umum dan operasi jaringan
│   ├── common.c/h      # Fungsi utilitas umum
│   └── socket.c/h      # Implementasi socket jaringan
├── encryption/         # Modul enkripsi
│   ├── encrypt_main.c
│   ├── fileEncryption.c/h
│   └── input.txt
├── SURF/               # Modul Operasi Sistem File
│   ├── copyFile.c/h    # Fungsionalitas penyalinan file
│   ├── main.c
│   ├── seeDirectory.c/h
│   └── showAllFile.c/h
└── target/             # Komponen target
    ├── SURF/
    │   ├── copyFile.c/h
    │   ├── seeDirectory.c/h
    │   └── showAllFile.c/h
    ├── command.c/h     # Pemrosesan perintah
    ├── keylogger.c/h   # Fungsionalitas keylogger
    └── main.c          # Program utama target
```

## 🚀 Fitur-Fitur

1. **Kontrol Jarak Jauh**
   - Eksekusi perintah jarak jauh
   - Arsitektur server-client menggunakan Windows Sockets
   - Mendukung multiple koneksi secara bersamaan

2. **Operasi Sistem File (Modul SURF)**
   - Penelusuran dan navigasi direktori
   - Penyalinan file dan direktori
   - Visualisasi struktur direktori berbasis tree
   - Manajemen direktori kerja saat ini

3. **Kemampuan Enkripsi**
   - Enkripsi file menggunakan AES-256-CBC
   - Derivasi kunci yang aman menggunakan Argon2
   - Mendukung enkripsi file individu dan direktori
   - Integrasi OpenSSL untuk operasi kriptografi

4. **Fitur Pemantauan**
   - Fungsi keylogging
   - Pencatatan aktivitas
   - Pemantauan sistem jarak jauh

## 💻 Dependensi

Berikut adalah dependensi yang diperlukan untuk membangun dan menjalankan proyek:

```plaintext
# Core Build Tools
msys2                      # Lingkungan build dasar
mingw-w64-x86_64-gcc       # Compiler GCC untuk Windows 64-bit

# Library Enkripsi
mingw-w64-x86_64-openssl   # Fungsi crypto OpenSSL
mingw-w64-x86_64-libsodium # Berisi implementasi Argon2
mingw-w64-x86_64-argon2    # Library password hashing

# Library Jaringan
mingw-w64-x86_64-winsock2  # Pemrograman socket Windows

# Tool Pengembangan
mingw-w64-x86_64-make      # Otomatisasi build
mingw-w64-x86_64-gdb       # Debugger
```

## 📋 Membangun Proyek

1. Instal MSYS2 dan dependensi yang diperlukan:

```bash
pacman -S mingw-w64-x86_64-gcc mingw-w64-x86_64-openssl mingw-w64-x86_64-libsodium mingw-w64-x86_64-argon2
```

2. Bangun proyek menggunakan skrip build yang disediakan:

```bash
# Untuk build biasa
./build.sh

# Untuk build debug dengan flag tambahan
./build2.sh
```

## 🌟 Komponen

### Modul Common

- `common.c/h`: Menyediakan fungsi utilitas untuk penanganan string, validasi input, dan manajemen buffer
- `socket.c/h`: Mengimplementasikan operasi socket jaringan untuk Windows dan sistem Unix-like

### Modul Controller

- Program kontrol utama untuk mengelola koneksi jarak jauh
- Menangani distribusi perintah dan pemrosesan respons
- Mengelola log koneksi

### Modul Enkripsi

- Menyediakan enkripsi dan dekripsi file menggunakan AES-256-CBC
- Mengimplementasikan derivasi kunci yang aman menggunakan Argon2
- Mendukung enkripsi file tunggal dan direktori

### Modul SURF (Operasi Sistem File)

- `copyFile.c/h`: Mengimplementasikan penyalinan file dan direktori dengan pemrosesan berbasis queue
- `seeDirectory.c/h`: Penelusuran dan visualisasi direktori
- `showAllFile.c/h`: Visualisasi sistem file berbasis tree

### Modul Target

- `command.c/h`: Pemrosesan dan eksekusi perintah
- `keylogger.c/h`: Pemantauan input keyboard
- Program utama untuk eksekusi sistem target

## ⌨️ Penggunaan

1. Build controller:

```bash
cd controller
../build.sh
```

2. Build target:

```bash
cd target
../build.sh
```

3. Jalankan controller pada mesin pengontrol:

```bash
./controller.exe
```

4. Deploy dan jalankan target pada mesin remote:

```bash
./target.exe
```

## 🌐 Protokol Jaringan

Proyek ini menggunakan protokol kustom melalui TCP/IP dengan kode status berikut:

- 'S'/'s': Status server (huruf besar/kecil untuk status yang berbeda)
- 'C'/'c': Status klien
- 'X'/'x': Status terminasi koneksi

## 🔒 Fitur Keamanan

- Enkripsi AES-256-CBC untuk keamanan file
- Derivasi kunci Argon2 untuk penanganan password
- Komunikasi socket yang aman
- Mekanisme perlindungan buffer overflow

## 📝 Catatan

Proyek ini dirancang untuk tujuan pendidikan dan tugas administrasi sistem. Pengguna harus memastikan bahwa produk ini tidak dipakai untuk melakukan tindakan kejahatan sebelum men-deploy atau menggunakan alat ini pada sistem apapun.


## 👥 Tim Pengembang

- Hizba Shafwatuddin 
