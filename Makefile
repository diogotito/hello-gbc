#
# A Makefile that compiles all .c and .s files in "src" and "res" 
# subdirectories and places the output in a "obj" subdirectory
#
include common.mk
LCC = ${GBDK_BINS}lcc

# ----------------
# Project metadata
# ----------------

PROJECTNAME = Dark-SciFi-TRPG

# --------------
# Compiler flags
# --------------

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

# -----------
# Directories
# -----------

SRCDIR      = src
SCNDIR     := ${SRCDIR}/scene
GAMEDIR    := ${SRCDIR}/game
DATADIR    := ${SRCDIR}/data
# ENTITIESDIR := ${SRCDIR}/entities, for example
# Maybe I should bring the "rwildcard" hack from RGBDS's template project here
# ...
CDIRS      := ${SRCDIR} ${SCNDIR} ${GAMEDIR} ${DATADIR}

OBJDIR      = obj
RESDIR      = res

# ---------------------------------------
# Variables for targets and prerequisites
# ---------------------------------------

BINS	    = ${OBJDIR}/${PROJECTNAME}.gbc

PARTIALS    = $(foreach dir,${CDIRS} ${RESDIR},$(wildcard ${dir}/*__*__.c))
CSOURCES    = $(notdir $(filter-out ${PARTIALS},\
				$(filter ${RESDIR}/%.c,${OUTPUT_C_FILES})\
                $(foreach dir,${CDIRS},$(wildcard ${dir}/*.c))\
				$(foreach dir,${RESDIR},$(wildcard ${dir}/*.c))))
ASMSOURCES  = $(foreach dir,${CDIRS},$(notdir $(wildcard ${dir}/*.s)))
OBJS        = $(CSOURCES:%.c=${OBJDIR}/%.o) $(ASMSOURCES:%.s=${OBJDIR}/%.o)

# $(info PARTIALS = ${PARTIALS})
# $(info OUTPUT_C_FILES = ${OUTPUT_C_FILES})
# $(info CSOURCES = ${CSOURCES})
# $(info OBJS     = ${OBJS})

# --------
# Includes
# --------

include res/Makefile

# ----------
# Full build
# ----------

.DEFAULT_GOAL := all
all:	${BINS}

${OBJS}: | ${OBJDIR}

${OBJDIR}:
	-${MKDIR} ${OBJDIR}

# -------
# Targets
# -------

# Canned recipe shared by all C compilation rules
define compile-c =
${LCC} ${LCCFLAGS} -c -o $@ $< \
$(if $(filter-out undefined,$(origin SHUT_UP_EVELYN)),| sed /EVELYN/d)
endef

# Compile .c files in "src/", "src/scene/" and "res/ to .o object files
# by dynamically generating rules like these for each word in ${CDIRS}:
# ${OBJDIR}/%.o:	src/somewhere_in_CDIRS/%.c ; ${compile-c}
$(eval $(foreach CDIR,${CDIRS},${newline}$\
$${OBJDIR}/%.o: ${CDIR}/%.c ; $${compile-c}))

# Add each __partial__.c file to the list of prerequisites of its parent's
# .o object file
$(eval $(foreach partial,${PARTIALS},$\
         $(let basename,$(notdir $(firstword $(subst __, ,${partial}))),$\
		 ${OBJDIR}/${basename}.o: ${partial}${newline})))

${OBJDIR}/%.o:	${RESDIR}/%.c ; ${compile-c}

# Compile .s assembly files in "src/" to .o object files
${OBJDIR}/%.o:	${SRCDIR}/%.s
	${LCC} ${LCCFLAGS} -c -o $@ $<

# If needed, compile .c files in "src/" to .s assembly files
# (not required if .c is compiled directly to .o)
${OBJDIR}/%.s:	${SRCDIR}/%.c
	${LCC} ${LCCFLAGS} -S -o $@ $<

# Link the compiled object files into a .gb ROM file
${BINS}:	${OBJS}
	${LCC} ${LCCFLAGS} -o ${BINS} ${OBJS}

# --------------------
# Tasks and misc files
# --------------------

clean: clean_build
clean_build:
	${RM} ${OBJDIR}

.PHONY: all clean clean_build