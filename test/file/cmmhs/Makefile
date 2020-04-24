# Copyright (C) 1996 Transarc Corporation - All rights reserved.

DATAFILES =	README mhsTestCDSAddr.pl mhsTestInit.pl mhsTestMSBind.pl \
		mhsTestSSBind.pl mhsTestUtil.pl mhsconfig.pl mhstest.pl \
		mhsTestInclude.pl

ILIST =	${DATAFILES}

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
