#
# A Makefile that compiles all .c and .s files in "src" and "res" 
# subdirectories and places the output in a "obj" subdirectory
#

.SUFFIXES:
.PHONY: all prepare assets check_for_new_OBJS_prerequisites clean

ifdef GBDK_HOME
	GBDK_BINS = $(GBDK_HOME)/bin/
endif

LCC = $(GBDK_BINS)lcc

# C23 standard
LCCFLAGS += -Wf--std-c23

# This is a Game Boy Color-only game
LCCFLAGS += -Wm-yC

# GBDK_DEBUG = ON
ifdef GBDK_DEBUG
	LCCFLAGS += -debug -v
else
	LCCFLAGS += -Wf--max-allocs-per-node50000
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

PARTIALS    = $(foreach dir,$(CDIRS) $(RESDIR),$(wildcard $(dir)/*__*__.c))
CSOURCES    = $(notdir $(filter-out $(PARTIALS),\
                $(foreach dir,$(CDIRS),$(wildcard $(dir)/*.c))\
				$(foreach dir,$(RESDIR),$(wildcard $(dir)/*.c))))
ASMSOURCES  = $(foreach dir,$(CDIRS),$(notdir $(wildcard $(dir)/*.s)))
OBJS        = $(CSOURCES:%.c=$(OBJDIR)/%.o) $(ASMSOURCES:%.s=$(OBJDIR)/%.o)

$(info PARTIALS = $(PARTIALS))
$(info CSOURCES = $(CSOURCES))
$(info OBJS     = $(OBJS))

all:    initial_OBJS := ${OBJS}
all:	prepare assets check_for_new_OBJS_prerequisites $(BINS)

compile.bat: Makefile
	@echo "REM Automatically generated from Makefile" > compile.bat
	@make -sn | sed s,$(GBDK_BINS),C:/tools/gbdk/, | sed y/\\//\\\\/ | sed s/mkdir\ -p\/mkdir\/ | grep -v make >> compile.bat


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

# Add each __partial__.c file to the list of prerequisites of its parent's
# .o object file
$(eval $(foreach partial,${PARTIALS},$\
         $(let basename,$(notdir $(firstword $(subst __, ,${partial}))),$\
		 ${OBJDIR}/${basename}.o: ${partial}${newline})))

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
	@mkdir -p $(OBJDIR)

assets:
	@echo ----- ASSETS -----
	$(MAKE) -C res
	@echo ------------------

check_for_new_OBJS_prerequisites:
	$(eval new_OBJS := $(filter-out ${initial_OBJS},${OBJS}))
	$(if ${new_OBJS},$\
	    $(info There are new OBJS to compile!)$\
		$(MAKE) ${new_OBJS})

clean:
#	rm -f  *.gb *.ihx *.cdb *.adb *.noi *.map
	-rm -f $(OBJDIR)/*.*
	-$(MAKE) -C res clean
