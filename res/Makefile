include ../common.mk
PNG2ASSET := ${GBDK_BINS}png2asset

# png2asset flags for groups of assets
TILESET_FLAGS := -map -tiles_only -keep_palette_order -keep_duplicate_tiles
MAP_FLAGS     := -map -use_map_attributes
SPRITE_FLAGS  := -spr8x8 -px 0 -py 0

# Process every .PNG in this directory by default
OUTPUT_C_FILES := $(foreach png,$(wildcard *.png),${png:.png=.h} ${png:.png=.c})

# -------
# Targets
# -------

all: ${OUTPUT_C_FILES}

# Tilesets:

ui_tiles.h ui_tiles.c &: ui_tiles.png
	${PNG2ASSET} $< ${TILESET_FLAGS} -tile_origin 13

# Screens and maps:

title.h title.c &: title.png
	${PNG2ASSET} $< ${MAP_FLAGS} -tile_origin 0

ruins.h ruins.c &: ruins.png
	${PNG2ASSET} $< ${MAP_FLAGS} -tile_origin 0 -max_palettes 6
	

# Sprites:

dude-sheet.h dude-sheet.c &: dude-sheet.png
	${PNG2ASSET} $< ${SPRITE_FLAGS} -tile_origin 80  -sw 16 -sh 16

cursor.h cursor.c &: cursor.png
	${PNG2ASSET} $< ${SPRITE_FLAGS} -tile_origin 100 -sw 16 -sh 16

# -----
# Tasks
# -----

# Cleanup task:
clean:
	-${RM} ${OUTPUT_C_FILES}

.PHONY: all clean
