set VS_PATH="C:\Program Files (x86)\Microsoft Visual Studio 12.0"
set SEVENZIP_PATH="C:\Program Files\7-Zip"
set QT_PATH32="C:\Qt\Qt5.6.3\5.6.3\msvc2013"

set BUILD_NAME=diesort_win32_portable
set SOURCE_PATH=%~dp0
mkdir %SOURCE_PATH%\build
mkdir %SOURCE_PATH%\build\loader
mkdir %SOURCE_PATH%\release
set /p RELEASE_VERSION=<%SOURCE_PATH%\release_version.txt

set QT_PATH=%QT_PATH32%
set QT_SPEC=win32-msvc2013
call %VS_PATH%\VC\bin\vcvars32.bat
set GUIEXE=die-sort.exe
set ZIP_NAME=%BUILD_NAME%_%RELEASE_VERSION%
set RES_FILE=rsrc

cd gui_source
%QT_PATH%\bin\qmake.exe gui_source.pro -r -spec %QT_SPEC% "CONFIG+=release"

nmake Makefile.Release clean
nmake
del Makefile
del Makefile.Release
del Makefile.Debug

cd ..

cd winloader_source

cl.exe /c winloader.cpp /D_USING_V110_SDK71_
Rc.exe /v %RES_FILE%.rc
link.exe winloader.obj %RES_FILE%.res user32.lib kernel32.lib shell32.lib /SUBSYSTEM:WINDOWS /SUBSYSTEM:WINDOWS,5.01 /ENTRY:entry /OUT:%SOURCE_PATH%\build\loader\%GUIEXE%
del /s winloader.obj
del /s %RES_FILE%.res

cd ..

mkdir %SOURCE_PATH%\release\%BUILD_NAME%
mkdir %SOURCE_PATH%\release\%BUILD_NAME%\base
mkdir %SOURCE_PATH%\release\%BUILD_NAME%\base\platforms
mkdir %SOURCE_PATH%\release\%BUILD_NAME%\base\sqldrivers

copy %SOURCE_PATH%\build\loader\%GUIEXE% %SOURCE_PATH%\release\%BUILD_NAME%\
copy %SOURCE_PATH%\build\release\%GUIEXE% %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\bin\Qt5Widgets.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\bin\Qt5Gui.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\bin\Qt5Core.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\bin\Qt5Sql.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %QT_PATH%\plugins\platforms\qwindows.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\platforms\
copy %QT_PATH%\plugins\sqldrivers\qsqlite.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\sqldrivers\

copy %VS_PATH%\VC\redist\x86\Microsoft.VC120.CRT\msvcp120.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\
copy %VS_PATH%\VC\redist\x86\Microsoft.VC120.CRT\msvcr120.dll %SOURCE_PATH%\release\%BUILD_NAME%\base\

cd %SOURCE_PATH%\release
if exist %ZIP_NAME%.zip del %ZIP_NAME%.zip
%SEVENZIP_PATH%\7z.exe a %ZIP_NAME%.zip %BUILD_NAME%\*
rmdir /s /q %SOURCE_PATH%\release\%BUILD_NAME%
cd ..