IF NOT EXIST zlib      (powershell -ExecutionPolicy Bypass %~dp0..\EZTools\load-library.ps1 zlib %1 %2)
IF NOT EXIST libpng    (powershell -ExecutionPolicy Bypass %~dp0..\EZTools\load-library.ps1 libpng %1 %2)
IF NOT EXIST libjpeg   (powershell -ExecutionPolicy Bypass %~dp0..\EZTools\load-library.ps1 libjpeg %1 %2)
IF NOT EXIST freetype  (powershell -ExecutionPolicy Bypass %~dp0..\EZTools\load-library.ps1 freetype %1 %2)
IF NOT EXIST podofo    (powershell -ExecutionPolicy Bypass %~dp0..\EZTools\load-library.ps1 podofo %1 %2) 