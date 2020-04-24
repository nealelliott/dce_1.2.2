# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
libbos.a_OFILES		= bbos_ncs_interface_cstub.o \
			  bbos_util.o bbos_pathnames.o \
			  dfsbsssvc.o dfsbssmsg.o bss_svc.o

.if !defined(LIBDCEDFS_LIBRARY_MAKEFILE)
#
# Information to build components when we are not building libdcedfs.a
#
INCLUDES		= bbos_pathnames.h bbos_util.h bbos_bnode.h \
			  bbos_ncs_interface.h bossvr_trace.h \
			  dfsbssmsg.h dfsbssmac.h dfsbsssvc.h

LIBRARIES		= libbos.a
PROGRAMS		= bosserver

EXPDIR			= /usr/include/dcedfs/
dfsbssmsg.h_EXPDIR	= /usr/include/dcedfs/
dfsbsssvc.h_EXPDIR	= /usr/include/dcedfs/
libbos.a_EXPDIR		= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

OTHERS			= dfsbbs.cat dfszbb.cat
TECHNOLOGY=dfs

SAMSFILES = bss.sams
SAMS_TARGETS = dfsbssmsg.h dfsbssmsg.c dfsbssmac.h dfsbsssvc.c dfsbsssvc.h \
    dfsbss.cat

ILIST			= ${OTHERS} bosserver bbos_ncs_interface.h \
			   bbos_util.h dfsbss.cat
IDIR			= /share/include/dcedfs/
dfsbss.cat_IDIR         = /nls/msg/en_US.ASCII/
dfsbbs.cat_IDIR		= /nls/msg/en_US.ASCII/
dfszbb.cat_IDIR		= /nls/msg/en_US.ASCII/
bosserver_IDIR		= /bin/

.if ${TARGET_MACHINE} != "RIOS"
bosserver_ILINKS	= /bin/dcebosserver
.endif

# Include -DDFS_BOSSERVER_ALLOW_SETDEBUG to get ``bos setdebug'' impl.
CFLAGS			= -Dunix ${USERCFLAGS} -D_BSD

.if ${TARGET_SYS} == "SPARC_SOL25" || ${TARGET_SYS} == "I386_SOL25"  
bossvr_thread_childWatch.o_CFLAGS = -DPOSIX_SUNOS5_SIGWAIT ${CFLAGS}
.endif

bosserver_OFILES	= bossvr_main.o bossvr_ncs_procs.o bbos_bnode.o \
			  bbos_ncs_interface_sstub.o \
			  bossvr_thread_childWatch.o \
			  bossvr_thread_reBossvrWatch.o \
			  bossvr_thread_bnodeTimeout.o \
			  bossvr_bnode_ezops.o bossvr_bnode_cronops.o \
			  bossvr_afsFiles.o bossvr_sleep.o bossvr_debug.o \
			  dfsbsssvc.o dfsbssmsg.o bss_svc.o

bosserver_LIBS		= -lbos -lcmd -lcmdAux -lafsutil -lncompat \
                          -ldacl -ldauth -ldacllfs -ldacl -ldauth -ldacllfs \
                          -licl -lncompat -lafssys -lafsutil -losi -lcom_err \
			  -ldce ${OS_LIBS}

IDLFILES		= bbos_ncs_interface.idl

IDLFLAGS		= -keep c_source -no_cpp

IDLINCFLAGS		= -I.

# On some platforms the optimizer is very expensive relative to the
# actual gain.  We disable it on a per-platform basis.
OPT_LEVEL		+= ${STUB_OPT_LEVEL}

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

bbos_util.h bbos_err.msf: bbos_util.p.h bbos_err.et
	-${CP} ${bbos_util.p.h:P} .
	${COMPILE_ET} ${bbos_err.et:P} -h bbos_util
	-${CP} bbos_util.h bbos_err.h
	${RM} -f bbos_util.p.h

bossvr_trace.h bossvr_trace.msf: bossvr_trace.et
	${COMPILE_ET} ${bossvr_trace.et:P} -h bossvr_trace

dfsbbs.cat: bbos_err.msf
	 ${GENCAT} $@ $>

dfszbb.cat: bossvr_trace.msf
	${GENCAT} $@ $>

.endif
