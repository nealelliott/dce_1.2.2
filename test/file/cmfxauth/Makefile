# Copyright (C) 1996 Transarc Corporation - All rights reserved.

PROGRAMS = fxsecadvice execunauth

fxsecadvice_OFILES = fxsecadvice.o
fxsecadvice_LIBS = -lafssys -ldce ${OS_LIBS}

execunauth_OFILES = execunauth.o
execunauth_LIBS = -lafssys -ldce ${OS_LIBS}

DATAFILES =	README secTestBounds.pl secTestBoundsWorker.pl \
		secTestFlAuth.pl secTestInclude.pl secTestRepAuth.pl \
		secTestRevokeAuth.pl secTestSetProt.pl secTestStartup.pl \
		secTestUtil.pl secconfig.pl sectest.pl

ILIST =	fxsecadvice execunauth ${DATAFILES}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.if exists (${TARGET_MACHINE}/Makefile)
.include "${TARGET_MACHINE}/Makefile"
.endif

.if exists (${TARGET_OS}/Makefile)
.include "${TARGET_OS}/Makefile"
.endif

.include <${RULES_MK}>
