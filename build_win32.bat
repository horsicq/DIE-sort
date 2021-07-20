set VSVARS_PATH="C:\Program Files (x86)\Microsoft Visual Studio\2019\Community\VC\Auxiliary\Build\vcvars32.bat"
set QMAKE_PATH="C:\Qt\5.15.2\msvc2019\bin\qmake.exe"
set SEVENZIP_PATH="C:\Program Files\7-Zip\7z.exe"

set X_SOURCE_PATH=%~dp0
set X_BUILD_NAME=xapkdetector_win32_portable
set /p X_RELEASE_VERSION=<%X_SOURCE_PATH%\release_version.txt

call %X_SOURCE_PATH%\build_tools\windows.cmd check_file %VSVARS_PATH%
call %X_SOURCE_PATH%\build_tools\windows.cmd check_file %QMAKE_PATH%
call %X_SOURCE_PATH%\build_tools\windows.cmd check_file %SEVENZIP_PATH%

IF NOT [%X_ERROR%] == [] goto exit

call %X_SOURCE_PATH%\build_tools\windows.cmd make_init
call %X_SOURCE_PATH%\build_tools\windows.cmd make_build %X_SOURCE_PATH%\DIE_sort_source.pro

call %X_SOURCE_PATH%\build_tools\windows.cmd check_file %X_SOURCE_PATH%\build\release\die-sort.exe

IF NOT [%X_ERROR%] == [] goto exit

copy %X_SOURCE_PATH%\build\release\die-sort.exe %X_SOURCE_PATH%\release\%X_BUILD_NAME%\
xcopy %X_SOURCE_PATH%\Detect-It-Easy\db %X_SOURCE_PATH%\release\%X_BUILD_NAME%\db /E /I

call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5Widgets
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5Gui
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5Core
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5Widgets
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_library Qt5Sql
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_plugin platforms qwindows
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_qt_plugin sqldrivers qsqlite
call %X_SOURCE_PATH%\build_tools\windows.cmd deploy_vc_redist

call %X_SOURCE_PATH%\build_tools\windows.cmd make_release

:exit
call %X_SOURCE_PATH%\build_tools\windows.cmd make_clear