@set "PATH=%GBDK_HOME%\bin;%PATH%"

REM NOTE TO SELF: Maybe -keep_palette_order

png2asset ruins.png -map -tile_origin 0 -use_map_attributes
png2asset dude-sheet.png -tile_origin 80 -sw 16 -sh 16 -px 0 -py 0
