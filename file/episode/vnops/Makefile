#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.180.1  1996/10/02  17:43:00  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:35:21  damon]
#
# $EndLog$
#

#
# NOTE - NETWORK ROOT ACCESS RIGHTS
#
# By default, a network root now is a superuser. This can be turned off
# at runtime by setting kernel global variable efs_networkRootSuperUser
# to zero.
#
# To turn off availability of this feature, set CFLAGS to include
# "-DNETWORK_ROOT_NOT_SUPERUSER" in this Makefile or in the top level
#  makefile.
#

INCLUDES		= efs_opcode.h efsmount.h efs_debug.h efs_trace.h \
			  efs_vnops_debug.h
PROGRAMS		= growaggr test_vnodeops
LIBRARIES		= libefsops.a
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}
EXPDIR			= /usr/include/dcedfs/episode/
libefsops.a_EXPDIR	= /usr/lib/
OTHERS			= dfszev.cat

libefsops.a_OFILES	= efs_dhops.o efs_access.o efs_vattr.o efs_dir.o \
			  efs_misc.o efs_vnode.o \
			  efs_vnodeops.o efs_vfsops.o efs_init.o \
			  efs_volvnops.o efs_debug.o efs_vnops_debug.o \
			  efs_promote.o efs_newalloc.o efs_volio.o

# newvol_OFILES		= efs_newvol.o stubs.o

# newvol_LIBS		= -lefsops -ldir -lanode -llogbuf -lasync -leacl \
#			  -lxcred -lbomb -lafsutil -ldfskutil \
#			  -ledsk -lftutil \
#			  -lcom_err -ltools \
#			  -ldacl -ldacllfs -ldacl -losi -licl \
#			  -ldce ${OS_LIBS}


growaggr_OFILES		= efs_growaggr.o stubs.o vnops_svc.o
growaggr_LIBS		= ${LIBRARIES} -lafssys -ldir -ledsk -lftutil -lanode \
			  -llogbuf -lasync -leacl -lxcred -lbomb -lafsutil \
		          -lcom_err -ltools -llwp -losi -ldfstab \
			  -ldacl -ldacllfs \
			  -licl -ldfskutil -lcmd -lncompat -ldce \
			  ${OS_LIBS}

test_vnodeops_OFILES	= test_vnodeops.o stubs.o vnops_svc.o

test_vnodeops_LIBS	= ${LIBRARIES} -ldir -lftutil -lanode -llogbuf -lasync -leacl \
			  -lxcred -llwp -lbomb -lafsutil -ldfskutil \
			  -lcom_err -ltools \
			  -ldacl -ldacllfs -ldacl -losi -licl \
                          -lncompat -ldce ${OS_LIBS}

efs_access.o_CFLAGS	= ${USERCFLAGS} -DEPI_USE_FULL_ID \
			  -DEPI_SOURCE_IDS_ARE_32BITS

CFLAGS			= ${USERCFLAGS} -DEPI_USE_FULL_ID

ILIST			= growaggr ${OTHERS}
IDIR			= /bin/
dfszev.cat_IDIR		= /nls/msg/en_US.ASCII/

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

efs_misc.o: efs_newalloc.h efs_tmpcopy.h efs_promote.h efs_volvnops.h
efs_newalloc.o: efs_newalloc.h efs_tmpcopy.h efs_evnode.h efs_misc.h
efs_promote.o: efs_newalloc.h efs_tmpcopy.h efs_misc.h efs_evnode.h
efs_tmpcopy.o: efs_tmpcopy.h efs_evnode.h efs_misc.h efs_newalloc.h
efs_volops.o: efs_volio.h efs_evnode.h efs_misc.h efs_volvnops.h

efs_trace.h efs_trace.msf: efs_trace.et
	${COMPILE_ET} ${efs_trace.et:P} -h efs_trace

dfszev.cat: efs_trace.msf
	${GENCAT} $@ $>
