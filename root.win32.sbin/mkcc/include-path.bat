@echo off

call setup.bat --quiet

for /f "tokens=* delims=" %%i in ('g++ -E -v -x c++ - ^< nul 2^>^&1 ^
    ^| gawk -f %SBIN%/include-list.awk' ^
    ^| sed -e 's/[ \r\n\t]\+/ /g' ^
    ^| sed -e 's/ \. / /g' ') do (
        set "GCC_INCLUDE_PATH=%%i"
)

if "%1" == "--list" (
echo %GCC_INCLUDE_PATH% ^
    | sed 's;\\\\\\\\;\\\\;g' ^
    | sed 's;\\\\;/;g' ^
    | sed 's/ /\\n/g' ^
    | sed '/^^ *$/d'
) else (
echo %GCC_INCLUDE_PATH%
)
