@echo off
for /f %%a in ('echo prompt $E^| cmd') do set "ESC=%%a"
@echo on

@set "PATH=%GBDK_HOME%\bin;%PATH%"


@echo. & echo %ESC%[31m=== UI ^& MISC TILESETS ===

set "TILESET_ARGS=-map -use_map_attributes"
png2asset frames.png %TILESET_ARGS% -tile_origin 192 -tiles_only -keep_palette_order


@echo. & echo %ESC%[32m=== MAPS ===

set "MAP_ARGS=-map -use_map_attributes"
png2asset ruins.png %MAP_ARGS% -tile_origin 0 -max_palettes 6


@echo. & echo %ESC%[34m=== OBJS ===

set "SPRITE_ARGS=-spr8x8 -px 0 -py 0"
png2asset dude-sheet.png %SPRITE_ARGS% -tile_origin 80 -sw 16 -sh 16

@echo %ESC%[0m
@echo _
