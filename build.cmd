rem Build script for librefactory
rem Despite the nature of being the "engine" this builds a binary app
rem And the end user (you) can develop Lua extensions to give this app
rem a life! Just run '' build '' in a Developer Command Prompt and you'll
rem have a working game in moments!

@echo off
setlocal
cls

set "ROOT=%~dp0"
set "SRC=%ROOT%src"
set "DEPS=%ROOT%deps"
set "OUT=%ROOT%out"
set "INC=/I"%ROOT%include" /I"%DEPS%\miniaudio" /I"%DEPS%\lua""

set "DEFINES=/D_WIN32_WINNT=0x0501 /DWINVER=0x0501"
set "CFLAGS=/O2 /W3 /nologo %DEFINES% %INC%"
  
if not exist "%OUT%" md "%OUT%"

echo Building Lua...
pushd "%DEPS%\lua"
for /f "delims=" %%f in ('dir /b /a-d *.c ^| findstr /v /i "lua.c luac.c"') do (
    cl %CFLAGS% /MT /c %%f /Fo"%OUT%\%%~nf.obj"
)
lib /nologo "%OUT%\lua*.obj" /OUT:"%OUT%\lua52.lib"
popd

echo Building game sources...
for /f "delims=" %%f in ('dir /b /a-d "%SRC%\*.c"') do (
    cl %CFLAGS% /MT /c "%SRC%\%%f" /Fo"%OUT%\%%~nf.obj"
)

echo Linking...
link /nologo /OUT:"%OUT%\librefactory.exe" /subsystem:windows ^
    "%OUT%\*.obj" "%OUT%\lua52.lib" ^
    kernel32.lib user32.lib winmm.lib opengl32.lib glu32.lib

echo Done.
