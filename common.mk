.SUFFIXES:

# Get GBDK commands under GBDK_HOME, if it's defined, otherwise rely on PATH
ifdef GBDK_HOME
	GBDK_BINS := $(GBDK_HOME)/bin/
endif

# Silent, idempotent commands to create and delete directories and files
# that work on Windows and *nix systems
ifeq '${OS}|${SHELL}' 'Windows_NT|sh.exe' # Detects Windows outside MSYS (Git Bash)
MKDIR = MD $(OBJDIR) 2>NUL || EXIT /B 0
RM    = DEL /S/Q/F 2>NUL
else
MKDIR = mkdir -p
RM    = rm -rf
endif


# This variable expands to a literal newline character
define newline =


endef
