#!/bin/bash

# build_windows.sh
# This script compiles a C program for Windows using the MinGW-w64 cross-compiler.
# It automatically finds source files from specified directories and includes headers.
# It links against OpenSSL's libcrypto and libsodium (which includes Argon2).
#
# IMPORTANT:
# This script assumes you have MinGW-w64, OpenSSL development libraries,
# and libsodium development libraries for the x86_64-w64-mingw32 target
# already installed and accessible in your system's PATH or standard library paths.
# Refer to the "Prerequisites" section in the previous response for installation.

echo "--- Starting C Program Build for Windows (MinGW-w64) ---"
echo " "

# --- Configuration ---
OUTPUT_EXECUTABLE="my_multi_program.exe" # The name of the Windows executable
COMPILER="x86_64-w64-mingw32-gcc"       # MinGW-w64 GCC compiler for 64-bit Windows

# Define the directories containing your C source files (.c)
# This includes the current directory (.), ../common/, and ./SURF/
SOURCE_DIRS=(
    "."           # Current directory
    "../common"   # Parent's common directory
    "./SURF"      # SURF directory within the current directory
)

# Define a primary directory for shared header files.
# If you have a central 'include' folder, specify it here.
# If headers are mostly co-located with source files, you can leave this empty
# or set it to an existing directory if headers are *also* there.
PRIMARY_INCLUDE_DIR="include"


# --- Check for Compiler ---
if ! command -v "$COMPILER" &> /dev/null; then
    echo "ERROR: MinGW-w64 compiler '$COMPILER' not found in your PATH."
    echo "Please install MinGW-w64 and ensure it's accessible."
    echo "If using MSYS2, you can install it with: pacman -S mingw-w64-x86_64-gcc"
    exit 1
fi
echo "MinGW-w64 compiler '$COMPILER' found."

# --- Gather Source Files ---
# Using an array to store source files for better handling of spaces/special characters in filenames.
declare -a ALL_SOURCE_FILES_ARRAY=()
echo " "
echo "Gathering source files from:"
for dir in "${SOURCE_DIRS[@]}"; do
    # Check if the directory exists and is a directory
    if [ ! -d "$dir" ]; then
        echo "WARNING: Source directory '$dir' not found or is not a directory. Skipping."
        continue
    fi
    echo "- $dir"
    # Find all .c files recursively in the source directory
    # -print0: Prints the full file name on the standard output, followed by a null character.
    # xargs -0: Reads items from standard input separated by null characters.
    # This approach is more robust against filenames containing spaces or special characters.
    while IFS= read -r -d $'\0' file; do
        ALL_SOURCE_FILES_ARRAY+=("$file")
    done < <(find "$dir" -type f -name "*.c" -print0)
done

if [ ${#ALL_SOURCE_FILES_ARRAY[@]} -eq 0 ]; then
    echo "ERROR: No C source files found in the specified directories: ${SOURCE_DIRS[@]}"
    exit 1
fi
echo " "
echo "Found C source files:"
for file in "${ALL_SOURCE_FILES_ARRAY[@]}"; do
    echo "- $file"
done

# --- Prepare Include Paths ---
# Start with the primary include directory if it exists
INCLUDE_FLAGS=""
if [ -n "$PRIMARY_INCLUDE_DIR" ] && [ -d "$PRIMARY_INCLUDE_DIR" ]; then
    INCLUDE_FLAGS="-I$PRIMARY_INCLUDE_DIR"
    echo "Adding primary include directory: $PRIMARY_INCLUDE_DIR"
else
    echo "Primary include directory '$PRIMARY_INCLUDE_DIR' not found or not specified. Skipping."
fi

# Add all source directories to the include paths as well,
# in case header files are co-located with source files.
echo "Adding source directories to include paths:"
for dir in "${SOURCE_DIRS[@]}"; do
    if [ -d "$dir" ]; then
        INCLUDE_FLAGS+=" -I$dir"
        echo "- $dir"
    fi
done
echo "Final include flags: $INCLUDE_FLAGS"


# --- Compilation Command ---
# -o: Specify the output executable name.
# -Wall -Wextra: Enable extensive warnings for better code quality.
# -s: Strip all symbol tables from the executable (reduces file size).
# -static: Link libraries statically. This makes the executable larger but removes
#          the dependency on external DLL files (like libcrypto-*.dll, libsodium-*.dll)
#          on the target Windows machine. This is generally preferred for simple deployments.
# -lcrypto: Link against OpenSSL's libcrypto library.
# -lsodium: Link against the libsodium library (which provides Argon2).

echo " "
echo "Compiling all source files into '$OUTPUT_EXECUTABLE'..."
# Pass the array of source files to the compiler.
# "${ALL_SOURCE_FILES_ARRAY[@]}" expands each element of the array as a separate argument.
echo "Command: $COMPILER ${ALL_SOURCE_FILES_ARRAY[@]} -o $OUTPUT_EXECUTABLE $INCLUDE_FLAGS -Wall -Wextra -s -static -lcrypto -lsodium"
echo " "

"$COMPILER" "${ALL_SOURCE_FILES_ARRAY[@]}" -o "$OUTPUT_EXECUTABLE" \
    $INCLUDE_FLAGS \
    -Wall -Wextra -s \
    -static \
    -lcrypto -lsodium

# --- Check Compilation Result ---
if [ $? -eq 0 ]; then
    echo " "
    echo "--------------------------------------------------------"
    echo "SUCCESS: Compilation complete!"
    echo "The Windows executable '$OUTPUT_EXECUTABLE' has been created."
    echo "You can now copy this file to a Windows machine and run it."
    echo "--------------------------------------------------------"
else
    echo " "
    echo "--------------------------------------------------------"
    echo "ERROR: Compilation failed."
    echo "Please review the error messages above."
    echo "Common issues:"
    echo "  - Missing MinGW-w64 development libraries (for OpenSSL or libsodium)."
    echo "    (e.g., install them via MSYS2 pacman as shown in Prerequisites)."
    echo "  - Incorrect header includes in your C files (e.g., #include \"myheader.h\" where myheader.h is not found)."
    echo "    Ensure all headers are either in your `PRIMARY_INCLUDE_DIR` or in one of your `SOURCE_DIRS`."
    echo "  - Syntax errors in your C code."
    echo "--------------------------------------------------------"
    exit 1
fi

