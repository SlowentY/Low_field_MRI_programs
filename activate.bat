@echo off

REM .cpb paths

set PROJECT1=GRU\UDP_test.cbp
set PROJECT2=Picoscope_API\pico_test_00_second_copy.cbp
set PROJECT3=Syncronisation\Sync.cbp
set PROJECT4=Hackrf\hackrftrans00\hackrftrans00.cbp

REM .exe paths 

set EXE1=GRU\bin\Debug\UDP_test.exe
set EXE2=Picoscope_API\bin\Debug\pico_test_00_second_copy.exe
set EXE3=Syncronisation\bin\Debug\Sync.exe
set EXE4=Hackrf\hackrftrans00\bin\Debug\hackrftrans00.exe

REM building's function

:build_project
    echo Building project: %1
    codeblocks --build %1
    if %errorlevel% neq 0 (
        echo Failed to build project: %1
        exit /b %errorlevel%
    )
    goto :eof

REM running's function

:run_exe
    echo Running executable: %1
    if exist %1 (
        start "" "%1"
    ) else (
        echo Executable not found: %1
        exit /b 1
    )
    goto :eof

call :build_project "%PROJECT1%"
call :build_project "%PROJECT2%"
call :build_project "%PROJECT3%"
call :build_project "%PROJECT4%"

echo All projects built successfully.

call :run_exe "%EXE1%"
call :run_exe "%EXE2%"
call :run_exe "%EXE3%"
call :run_exe "%EXE4%"

echo All executables ran successfully.