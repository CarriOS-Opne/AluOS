The AluOS library is based on the AluOS open source and is monolithic. 

Place BOOTX64.c, AluOS.C, and stb_truetype.h (https://github.com/nothings/stb/blob/master/stb_truetype.h) together. Then compile with Visual Studio.

 
@echo off
chcp 65001 > nul
echo ========== AluOS Build Script ==========

set "VCVARS="

if exist "C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat" (
    set "VCVARS=C:\Program Files\Microsoft Visual Studio\18\Community\VC\Auxiliary\Build\vcvars64.bat"
    goto found_compiler
)

set "vswhere=%ProgramFiles(x86)%\Microsoft Visual Studio\Installer\vswhere.exe"
if exist "%vswhere%" (
    for /f "usebackq tokens=*" %%i in (`"%vswhere%" -latest -products * -property installationPath`) do (
        if exist "%%i\VC\Auxiliary\Build\vcvars64.bat" (
            set "VCVARS=%%i\VC\Auxiliary\Build\vcvars64.bat"
            goto found_compiler
        )
    )
)

:no_compiler
echo.
echo [FATAL ERROR] vcvars64.bat not found!
set /p VCVARS="Enter full path to vcvars64.bat: "
if not exist "%VCVARS%" (
    echo [ERROR] File not found.
    pause
    exit /b 1
)

:found_compiler
echo [INFO] Compiler found: %VCVARS%
call "%VCVARS%"

echo.
echo [1/2] Compiling bootx64.c...
cl /utf-8 /nologo /c /std:c11 /O2 /GS- /Gs1048576 /Gy- /Zl /Gz /W4 /wd4100 /wd4201 /wd4214 /wd4057 /I. bootx64.c
if errorlevel 1 goto compile_error

echo.
echo [2/2] Linking BOOTX64.EFI...
link /nologo /subsystem:efi_application /entry:EfiMain /nodefaultlib /dll /ALIGN:4096 /out:BOOTX64.EFI bootx64.obj /FORCE:MULTIPLE
if errorlevel 1 goto link_error

echo.
echo ========== BUILD SUCCESS ==========
pause
exit /b 0

:compile_error
echo.
echo [ERROR] Compilation failed!
pause
exit /b 1

:link_error
echo.
echo [ERROR] Linking failed!
pause
exit /b 1
