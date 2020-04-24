#
# HISTORY
# This revision checks in a working set of scripts to test the DFS backup
# system. The files checked in for this delta are high level scripts to
# configure and run the test suite. Separate deltas would export the scripts
# to test individual bak commands.
#
# The README file provides a description on how to configure and run these tests.
#

DATAFILES	=	restoredisk-1.2.5.2.1b restoredisk-11.2e \
			restoredisk-1.2.5.2.2b  restoredisk-1e \
			restoredisk-1.1.1b restoredisk-1.3.1.1b \
			restoredisk-2.1e restoredisk-1.1.2b \
			restoredisk-1.3.1.2b restoredisk-2.2e \
			restoredisk-1.2.1b restoredisk-1.3.2.1b \
			restoredisk-7e restoredisk-1.2.2b \
			restoredisk-1.3.2.2b restoredisk-8.1e \
			restoredisk-1.2.3b restoredisk-1.4.1b \
			restoredisk-8.2e restoredisk-1.2.4.1.1b \
			restoredisk-1.4.2b restoredisk-9.1.1e \
			restoredisk-1.2.4.1.2b restoredisk-1.4.3b \
			restoredisk-9.1.2e restoredisk-1.2.4.2.1b \
			restoredisk-1.4.4b restoredisk-9.2e \
			restoredisk-1.2.4.2.2b restoredisk-1.5b \
			restoredisk-9.3e restoredisk-1.2.5.1.1b \
			restoredisk-10e restoredisk-cmd \
			restoredisk-1.2.5.1.2b restoredisk-11.1e \
			README

ILIST   	= 	${DATAFILES}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>
