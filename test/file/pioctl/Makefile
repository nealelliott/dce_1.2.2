#
#  Makefile --
#	For the test/file/pioctl directory.
#

DATAFILES = README

PROGRAMS = pioctl_test

CCTYPE = ansi

CFLAGS = ${USERCFLAGS} -DITL_FS_TEST

ILIST = ${DATAFILES} ${PROGRAMS}

SUBDIRS = scripts

DATA_SUBDIRS = scripts

pioctl_test_OFILES = pioctl_test.o
pioctl_test_LIBS   = -litlpioctl -litlFile -litlHubSpoke -litlUtils -litl \
                 -litlcommon -lafssys -licl -lafsutil -losi \
                 -lcom_err -ldce ${OS_LIBS}

.if ${TARGET_OS} == "SUNOS5"
pioctl_test_LIBS += -lsocket
.endif

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

