@echo off
REM This script creates a folder structure: STORAGE/GET and STORAGE/INJECT

set "BASE_FOLDER=STORAGE"
set "GET_FOLDER=%BASE_FOLDER%\GET"
set "INJECT_FOLDER=%BASE_FOLDER%\INJECT"

echo Creating folder structure...

REM Check if BASE_FOLDER already exists
if exist "%BASE_FOLDER%" (
    echo Folder "%BASE_FOLDER%" already exists.
) else (
    md "%BASE_FOLDER%"
    if exist "%BASE_FOLDER%" (
        echo Folder "%BASE_FOLDER%" created successfully.
    ) else (
        echo Failed to create folder "%BASE_FOLDER%". Exiting.
        goto :eof
    )
)

REM Create GET folder
if exist "%GET_FOLDER%" (
    echo Folder "%GET_FOLDER%" already exists.
) else (
    md "%GET_FOLDER%"
    if exist "%GET_FOLDER%" (
        echo Folder "%GET_FOLDER%" created successfully.
    ) else (
        echo Failed to create folder "%GET_FOLDER%".
    )
)

REM Create INJECT folder
if exist "%INJECT_FOLDER%" (
    echo Folder "%INJECT_FOLDER%" already exists.
) else (
    md "%INJECT_FOLDER%"
    if exist "%INJECT_FOLDER%" (
        echo Folder "%INJECT_FOLDER%" created successfully.
    ) else (
        echo Failed to create folder "%INJECT_FOLDER%".
    )
)

echo.
echo Folder structure creation complete.
pause

gcc *.c ..\common\*.c -lws2_32 -o main
