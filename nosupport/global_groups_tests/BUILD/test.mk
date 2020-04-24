#
#
#

.if !defined(_TEST_MK_)
_TEST_MK_=

#
# Set the IDIR variable to be the directory in which we are building.  This 
# affects the install build of the whole test tree.
#
IDIR ?= /${MAKESUB}

.if ${TARGET_MACHINE} == "RIOS"
  CCTYPE                ?= traditional
.endif
.endif


CFLAGS		?= ${${TARGET_MACHINE}_CFLAGS}
LDFLAGS		?= ${${TARGET_MACHINE}_LDFLAGS}
YFLAGS		?= ${${TARGET_MACHINE}_YFLAGS}
LIBS		?= ${LIBS_DCE} ${${TARGET_MACHINE}_LIBS}
