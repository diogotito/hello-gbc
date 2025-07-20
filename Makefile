#
# A Makefile that compiles all .c and .s files in "src" and "res" 
# subdirectories and places the output in a "obj" subdirectory
#

# If you move this project you can change the directory 
# to match your GBDK root directory (ex: GBDK_HOME = "C:/GBDK/"
ifndef GBDK_HOME
	GBDK_HOME = ../../../
endif

LCC = $(GBDK_HOME)bin/lcc 

# C23 standard
LCCFLAGS += -Wf--std-c23

# This is a Game Boy Color-only game
LCCFLAGS += -Wm-yC

# GBDK_DEBUG = ON
ifdef GBDK_DEBUG
	LCCFLAGS += -debug -v
endif


# You can set the name of the .gb ROM file here
PROJECTNAME    = Example

SRCDIR      = src
SCNDIR     := $(SRCDIR)/scene
# ENTITIESDIR := $(SRCDIR)/entities, for example
# ...
CDIRS      := $(SRCDIR) $(SCNDIR)

OBJDIR      = obj
RESDIR      = res
BINS	    = $(OBJDIR)/$(PROJECTNAME).gb

CSOURCES    = $(foreach dir,$(CDIRS),$(notdir $(wildcard $(dir)/*.c))) $(foreach dir,$(RESDIR),$(notdir $(wildcard $(dir)/*.c)))
ASMSOURCES  = $(foreach dir,$(SRCDIR),$(notdir $(wildcard $(dir)/*.s)))
OBJS        = $(CSOURCES:%.c=$(OBJDIR)/%.o) $(ASMSOURCES:%.s=$(OBJDIR)/%.o)

# $(info CSOURCES = $(CSOURCES))
# $(info OBJS     = $(OBJS))

all:	prepare $(BINS)

compile.bat: Makefile
	@echo "REM Automatically generated from Makefile" > compile.bat
	@make -sn | sed s,$(GBDK_HOME),C:/tools/gbdk/, | sed y/\\//\\\\/ | sed s/mkdir\ -p\/mkdir\/ | grep -v make >> compile.bat


# Canned recipe shared by all C compilation rules
define compile-c =
$(LCC) $(LCCFLAGS) -c -o $@ $<
endef

define newline =


endef

# Compile .c files in "src/", "src/scene/" and "res/ to .o object files
# by dynamically generating rules like these for each word in $(CDIRS):
# $(OBJDIR)/%.o:	src/somewhere_in_CDIRS/%.c ; $(compile-c)
$(eval $(foreach CDIR,$(CDIRS),$(newline)$\
$$(OBJDIR)/%.o: $(CDIR)/%.c ; $$(compile-c)))

$(OBJDIR)/%.o:	$(RESDIR)/%.c ; $(compile-c)

# Compile .s assembly files in "src/" to .o object files
$(OBJDIR)/%.o:	$(SRCDIR)/%.s
	$(LCC) $(LCCFLAGS) -c -o $@ $<

# If needed, compile .c files in "src/" to .s assembly files
# (not required if .c is compiled directly to .o)
$(OBJDIR)/%.s:	$(SRCDIR)/%.c
	$(LCC) $(LCCFLAGS) -S -o $@ $<

# Link the compiled object files into a .gb ROM file
$(BINS):	$(OBJS)
	$(LCC) $(LCCFLAGS) -o $(BINS) $(OBJS)

prepare:
	mkdir -p $(OBJDIR)

clean:
#	rm -f  *.gb *.ihx *.cdb *.adb *.noi *.map
	rm -f  $(OBJDIR)/*.*

