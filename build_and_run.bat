set "LCC=%GBDK_HOME%\bin\lcc.exe"
set "PNG2ASSET=%GBDK_HOME%\bin\png2asset.exe"

cd /d "%~dp0"
rmdir /S/Q obj

pushd res
png2asset ui_tiles.png -map -tile_origin 13 -tiles_only -keep_palette_order -keep_duplicate_tiles
png2asset ruins.png -map -tile_origin 0 -use_map_attributes
png2asset dude-sheet.png -tile_origin 80 -sw 16 -sh 16 -px 0 -py 0
popd

mkdir obj
%LCC% -Wf--std-c23 -Wm-yC -c -o obj\dude.o src\dude.c
%LCC% -Wf--std-c23 -Wm-yC -c -o obj\input.o src\input.c
%LCC% -Wf--std-c23 -Wm-yC -c -o obj\main.o src\main.c
%LCC% -Wf--std-c23 -Wm-yC -c -o obj\my_sprites.o src\my_sprites.c
%LCC% -Wf--std-c23 -Wm-yC -c -o obj\ui.o src\ui.c
%LCC% -Wf--std-c23 -Wm-yC -c -o obj\dude-sheet.o res\dude-sheet.c
%LCC% -Wf--std-c23 -Wm-yC -c -o obj\ruins.o res\ruins.c
%LCC% -Wf--std-c23 -Wm-yC -c -o obj\ui_tiles.o res\ui_tiles.c
%LCC% -Wf--std-c23 -Wm-yC -o obj\Example.gb obj\dude.o obj\input.o obj\main.o obj\my_sprites.o obj\ui.o obj\dude.o obj\dude-sheet.o obj\ruins.o obj\ui_tiles.o 

obj\Example.gb
