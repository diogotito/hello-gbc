-include ../common.mk
PNG2ASSET := ${GBDK_BINS}png2asset

# png2asset flags for groups of assets
TILESET_FLAGS := -map -tiles_only -keep_palette_order -keep_duplicate_tiles
MAP_FLAGS     := -map -use_map_attributes
SPRITE_FLAGS  := -spr8x8 -px 0 -py 0

# This directory (whether this Makefile is used directly or included)
RESDIR        ?= .
R             := ${RESDIR}

# Process every .PNG in this directory by default
OUTPUT_C_FILES := $(foreach png,$(wildcard $R/*.png),${png:.png=.h} ${png:.png=.c})

# -------
# Targets
# -------

all: all_assets
all_assets: ${OUTPUT_C_FILES}

# Tilesets:

$R/ui_tiles.h $R/ui_tiles.c &: $R/ui_tiles.png
	${PNG2ASSET} $< ${TILESET_FLAGS} -tile_origin 13

# Screens and maps:

$R/title.h $R/title.c &: $R/title.png
	${PNG2ASSET} $< ${MAP_FLAGS} -tile_origin 0

$R/ruins.h $R/ruins.c &: $R/ruins.png
	${PNG2ASSET} $< ${MAP_FLAGS} -tile_origin 0 -max_palettes 6
	

# Sprites:

$R/dude-sheet.h $R/dude-sheet.c &: $R/dude-sheet.png
	${PNG2ASSET} $< ${SPRITE_FLAGS} -tile_origin 80  -sw 16 -sh 16

$R/cursor.h $R/cursor.c &: $R/cursor.png
	${PNG2ASSET} $< ${SPRITE_FLAGS} -tile_origin 100 -sw 16 -sh 16

# -----
# Tasks
# -----

# Cleanup task:
clean: clean_assets
clean_assets:
	-${RM} ${OUTPUT_C_FILES}

.PHONY: all all_assets clean clean_assets
