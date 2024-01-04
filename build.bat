@ECHO OFF
SET COMPILE=0
SET VERBOSE=0
SET RUN=0
SET DEBUG=0

REM -c compile, only to run make
REM -v verbose make
REM -e execute program after building
REM -d debug settings for cmake
FOR %%x IN (%*) DO (
    IF /I "%%x"=="-c" SET COMPILE=1
    IF /I "%%x"=="-v" SET VERBOSE=1
    IF /I "%%x"=="-e" SET RUN=1
    IF /I "%%x"=="-d" SET DEBUG=1
)

IF %COMPILE% EQU 1 (
    ECHO Running only compilation
    cd build
    CALL :make_func
) ELSE (
    ECHO Rebuilding..
    RD /S /Q build
    mkdir build && cd build
    CALL :cmake_func
    CALL :make_func
    xcopy /e /i ..\assets\ assets\
    XCOPY /S c:\sdl2\SDL2.dll .
    XCOPY /S c:\sdl2\SDL2_ttf.dll .
)

REM run only if needed
IF %RUN% EQU 1 (
    ECHO Run
    START feditor.exe
)

:make_func
REM verbose mode for make
IF %VERBOSE% EQU 1 (
    make VERBOSE=1
) ELSE (
    make
)
GOTO :EOF

:cmake_func
IF %DEBUG% EQU 1 (
    ECHO Debug build
    cmake .. -G "MinGW Makefiles" --log-level=VERBOSE -DDEBUG_FLAG=ON -DCMAKE_BUILD_TYPE=Debug
) ELSE (
    ECHO Release build
    cmake .. -G "MinGW Makefiles" --log-level=VERBOSE -DCMAKE_BUILD_TYPE=Release
)
GOTO :EOF