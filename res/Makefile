.SUFFIXES:
.PHONY: all clean

# Get png2asset command under GBDK_HOME, if it's defined, otherwise rely on PATH
ifdef GBDK_HOME
	GBDK_BINS := $(GBDK_HOME)/bin/
endif

PNG2ASSET := $(GBDK_BINS)png2asset

# png2asset flags for groups of assets
TILESET_FLAGS := -map -tiles_only -keep_palette_order -keep_duplicate_tiles
MAP_FLAGS     := -map -use_map_attributes
SPRITE_FLAGS  := -spr8x8 -px 0 -py 0

# Process every .PNG in this directory by default
OUTPUT_C_FILES := $(foreach png,$(wildcard *.png),${png:.png=.h} ${png:.png=.c})
all: $(OUTPUT_C_FILES)

# Tilesets:
ui_tiles.h ui_tiles.c &: ui_tiles.png
	$(PNG2ASSET) $< $(TILESET_FLAGS) -tile_origin 13

# Screens and maps:
title.h title.c &: title.png
	$(PNG2ASSET) $< $(MAP_FLAGS) -tile_origin 0
ruins.h ruins.c &: ruins.png
	$(PNG2ASSET) $< $(MAP_FLAGS) -tile_origin 0 -max_palettes 6

# Sprites:
dude-sheet.h dude-sheet.c &: dude-sheet.png
	$(PNG2ASSET) $< $(SPRITE_FLAGS) -tile_origin 80  -sw 16 -sh 16
cursor.h cursor.c &: cursor.png
	$(PNG2ASSET) $< $(SPRITE_FLAGS) -tile_origin 100 -sw 16 -sh 16

# Cleanup task:
clean:
	-$(if $(findstring Windows_NT,$(OS)),DEL /Q/F 2>NUL,rm -f) $(OUTPUT_C_FILES)
