#
# HISTORY
# This revision checks in a working set of scripts to test the DFS backup
# system. The files checked in for this delta are high level scripts to
# configure and run the test suite. Separate deltas would export the scripts
# to test individual bak commands.
#
# The README file provides a description on how to configure and run these tests.
#

DATAFILES	= 	restoredb-1b restoredb-2.1b restoredb-2.3b \
			restoredb-2.1.1e restoredb-2.2b restoredb-2.3e \
			restoredb-2.1.2e restoredb-2.2e restoredb-cmd \
			README

ILIST     	= 	${DATAFILES}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>
