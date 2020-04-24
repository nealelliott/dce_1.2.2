# DCE GlobalGroup testing
# test/globalgroup/ts/ggapi Makefile
#

# Compiler flags for this directory
CFLAGS = ${${TARGET_MACHINE}_CFLAGS}

# Programs to build
PROGRAMS = 	do_ggapi

do_ggapi_OFILES	= do_ggapi.o

# Libraries the programs need to link with
LIBS = -ldce ${${TARGET_MACHINE}_LIBS}

ILIST		= ${PROGRAMS}

.include <${RULES_MK}>
