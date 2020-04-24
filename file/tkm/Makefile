# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# Copyright (c) 1994 Transarc Corporation
# 
# $EndLog$

PROGRAMS		= tkm_systest tkm_test_units tkm_adjust

INCLUDES		= tkm_tokens.h tkm_errs.h tkm_race.h tkm_trace.h

LIBRARIES		= libtkm.a

EXPDIR			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

tkm_tokens.h_EXPDIR	= /usr/include/dcedfs/
tkm_errs.h_EXPDIR	= /usr/include/dcedfs/
tkm_race.h_EXPDIR	= /usr/include/dcedfs/
tkm_trace.h_EXPDIR	= /usr/include/dcedfs/

OTHERS			= dfstkm.cat dfsztm.cat
ILIST			= ${OTHERS} tkm_tokens.h tkm_errs.h tkm_race.h \
			   tkm_trace.h
IDIR			= /share/include/dcedfs/
dfstkm.cat_IDIR		= /nls/msg/en_US.ASCII/
dfsztm.cat_IDIR		= /nls/msg/en_US.ASCII/

CFLAGS			= ${USERCFLAGS} -DTKM_DEBUG

libtkm.a_OFILES		= tkm_recycle.o \
			  tkm_misc.o tkm_asyncgrant.o tkm_tokens.o\
			  tkm_filetoken.o tkm_voltoken.o \
			  tkm_file.o tkm_volume.o \
			  tkm_revokes.o tkm_conflicts.o tkm_ctable.o\
			  tkm_tokenlist.o tkm_range.o tkm_race.o

GARBAGE			= tkm_errs.h tkm_trace.h

tkm_adjust_OFILES	= tkm_adjust.o tkm_userint.o libtkm.a
tkm_adjust_LIBS		= -ltpq -licl -lafssys -lncompat -lcmd -lcmdAux \
			  -lcom_err -losi -ldce ${OS_LIBS}
tkm_test_units_OFILES   =  tkm_test_units.o libtkm.a
tkm_test_units_LIBS     = -ltpq -licl -lafssys -lncompat -lcmd -lcmdAux \
			  -lcom_err -losi -ldce ${OS_LIBS}
tkm_systest_OFILES   =  tkm_systest_main.o tkm_systest_parconf.o \
			    tkm_systest_rpc.o tkm_systest_asyncgrant.o libtkm.a
tkm_systest_LIBS     = -ltpq -licl -lafssys -lncompat -lcmd -lcmdAux \
			  -lcom_err -losi -ldce ${OS_LIBS}
.if exists(../${TARGET_MACHINE}/machdep.mk)
.include "../${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(../${TARGET_OS}/machdep.mk)
.include "../${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

tkm_errs.h tkm_errs.msf: tkm_errs.et
	${COMPILE_ET} ${tkm_errs.et:P} -h tkm_errs

dfstkm.cat: tkm_errs.msf
	 ${GENCAT} $@ $>

tkm_trace.h tkm_trace.msg: tkm_trace.et
	${COMPILE_ET} ${tkm_trace.et:P} -h tkm_trace

dfsztm.cat: tkm_trace.msf
	${GENCAT} $@ $>








