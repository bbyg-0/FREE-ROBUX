#!/bin/bash
# This script creates a folder structure: STORAGE/GET and STORAGE/INJECT

BASE_FOLDER="STORAGE"
GET_FOLDER="$BASE_FOLDER/GET"
INJECT_FOLDER="$BASE_FOLDER/INJECT"

echo "Creating folder structure..."

# Use mkdir -p to create parent directories if they don't exist
# and suppress errors if directories already exist.

mkdir -p "$GET_FOLDER"
if [ $? -eq 0 ]; then
    echo "Folder '$GET_FOLDER' created/exists."
else
    echo "Failed to create folder '$GET_FOLDER'."
fi

mkdir -p "$INJECT_FOLDER"
if [ $? -eq 0 ]; then
    echo "Folder '$INJECT_FOLDER' created/exists."
else
    echo "Failed to create folder '$INJECT_FOLDER'."
fi

echo ""
echo "Folder structure creation complete."

gcc *.c ../common/*.c -o main
