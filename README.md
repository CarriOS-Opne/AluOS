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

for %%D in (C D E F) do (
    for %%V in (18 2026 2022 2019) do (
        for %%E in (Community Professional Enterprise BuildTools) do (
            if exist "%%D:\Program Files\Microsoft Visual Studio\%%V\%%E\VC\Auxiliary\Build\vcvars64.bat" (
                set "VCVARS=%%D:\Program Files\Microsoft Visual Studio\%%V\%%E\VC\Auxiliary\Build\vcvars64.bat"
                goto found_compiler
            )
            if exist "%%D:\Program Files (x86)\Microsoft Visual Studio\%%V\%%E\VC\Auxiliary\Build\vcvars64.bat" (
                set "VCVARS=%%D:\Program Files (x86)\Microsoft Visual Studio\%%V\%%E\VC\Auxiliary\Build\vcvars64.bat"
                goto found_compiler
            )
        )
    )
)


:no_compiler
echo.
echo [FATAL ERROR] Visual Studio C++ Compiler (vcvars64.bat) not found!
echo 컴파일러를 자동으로 찾을 수 없습니다.
echo.
set /p VCVARS="Visual Studio가 설치된 'vcvars64.bat'의 정확한 전체 경로를 입력해주세요: "
if not exist "%VCVARS%" (
    echo [ERROR] 입력하신 경로에도 파일이 없습니다. C++ 데스크톱 개발 워크로드가 설치되었는지 확인해주세요.
    pause
    exit /b
)

:found_compiler
echo [INFO] Found Compiler: %VCVARS%
call "%VCVARS%"

echo.
echo [1/2] Compiling OS Core (bootx64.c)...
cl /utf-8 /nologo /c /O2 /GS- /Gs1048576 /Gy- /Wall /wd4053 /wd4116 /wd4201 /wd4214 /wd4255 /wd4820 /wd4668 /wd4100 /wd4047 /wd4028 /wd4018 /wd4133 /wd4090 /wd4024 /wd4115 /wd4087 /I. bootx64.c
if errorlevel 1 goto compile_error

echo.
echo [2/2] Linking into BOOTX64.EFI...
link /nologo /subsystem:efi_application /entry:EfiMain /nodefaultlib /dll /out:BOOTX64.EFI bootx64.obj
if errorlevel 1 goto link_error

echo.
echo ========== BUILD SUCCESS ==========
pause
exit /b

:compile_error
echo.
echo [ERROR] Compilation failed! See errors above.
pause
exit /b

:link_error
echo.
echo [ERROR] Linking failed! See errors above.
pause
exit /b
