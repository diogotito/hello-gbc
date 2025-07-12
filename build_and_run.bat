set "LCC=%GBDK_HOME%\bin\lcc.exe"
set "PNG2ASSET=%GBDK_HOME%\bin\png2asset.exe"

cd /d "%~dp0"
rmdir /S/Q obj

pushd res
png2asset ruins.png -map -tile_origin 0 -use_map_attributes
png2asset dude-sheet.png -tile_origin 80 -sw 16 -sh 16 -px 0 -py 0
popd

mkdir obj
%LCC% -Wf--std-c23 -Wm-yC -c -o obj\dude.o res\dude.c
%LCC% -Wf--std-c23 -Wm-yC -c -o obj\dude-sheet.o res\dude-sheet.c
%LCC% -Wf--std-c23 -Wm-yC -c -o obj\ruins.o res\ruins.c
%LCC% -Wf--std-c23 -Wm-yC -c -o obj\main.o src\main.c
%LCC% -Wf--std-c23 -Wm-yC -o obj\Example.gb obj\main.o obj\dude.o obj\dude-sheet.o obj\ruins.o 

obj\Example.gb
