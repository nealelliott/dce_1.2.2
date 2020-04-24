# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.148.1  1996/10/02  17:15:41  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:06:59  damon]
#
# Revision 1.1.142.3  1994/06/09  13:56:10  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:30:43  annie]
# 
# Revision 1.1.142.2  1994/02/04  20:10:03  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:08:31  devsrc]
# 
# Revision 1.1.14.1  1993/12/07  17:15:54  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/06  13:35:07  jaffe]
# 
# $EndLog$
#
INCLUDES 		= tlock.h fid.h anode.h file.h volume.h boot.h \
			  aggregate.h salvage.h sal_errors.h volume_table.h \
			  block_alloc.h

LIBRARIES		= libedsk.a libanode.a
PROGRAMS		= newaggr findlog test_anode calcLogSize

EXPDIR			= /usr/include/dcedfs/episode/
libedsk.a_EXPDIR	= /usr/lib/
libanode.a_EXPDIR	= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}

OTHERS			= dfsepi.cat dfszea.cat newaggr findlog calcLogSize
ILIST			= ${OTHERS}
IDIR			= /bin/
findlog_IDIR		= /unsupported/bin/
dfsepi.cat_IDIR		= /nls/msg/en_US.ASCII/
dfszea.cat_IDIR		= /nls/msg/en_US.ASCII/

#
# Set CARGS on build cmd line to enable more debugging
#     (e.g. "build CARGS=-DAFSL_DEBUG_LEVEL=5")
#
CFLAGS			= ${USERCFLAGS} -D_TRANSARC_PTHREAD_PROTO

libedsk.a_OFILES	= edbasic.o ederoot.o edbcheck.o edecheck.o
libanode.a_OFILES	= anode_block.o fixed_anode.o \
			  block_alloc.o anode.o volume_table.o \
			  volume.o file.o aggregate.o strategy.o\
			  epia_volreg.o tlock.o salvage.o fid.o

.ifdef PLUMBER
PLUMBER			= plumber.o
.else
PLUMBER			=
.endif

.if exists(${TARGET_MACHINE}/Makefile)
.include "${TARGET_MACHINE}/Makefile"
.endif
.if exists(${TARGET_OS}/Makefile)
.include "${TARGET_OS}/Makefile"
.endif

test_anode_OFILES	= test_anode.o runtest.o ${TESTVM} epia_volreg.o \
			  ${PLUMBER}

LIBS			= libanode.a libedsk.a -llogbuf -lasync -lbomb \
			  -lafsutil \
			  -lcom_err -ltools -llwp -ldfstab -lcmd -lncompat \
			  -lftutil -licl -lafssys -losi -ldce ${OS_LIBS}

test_anode_LIBS		= libanode.a -llogbuf -lasync -ltools \
			  -lbomb -lafsutil -lcom_err -ldfstab -lncompat -lcmd \
			  -llwp -licl -lafssys -losi \
			  -ldce ${OS_LIBS}

PROGRAMS		+= test_sscanf
test_sscanf_LIBS	= -llwp -licl -losi -ldce ${OS_LIBS}

calcLogSize_LIBS	= -llogbuf -lasync \
			  -lafsutil -lcom_err -ltools -llwp -licl -losi \
			  -ldfstab -lcmd -ldce ${OS_LIBS}
traverseAggrs_LIBS	= -lelf

PROGRAMS		+= spin
spin_LIBS		= -llwp -losi -ldce ${OS_LIBS}

#
# test anode is a test simulation program and should always be compiled
# with symbols.
#
test_anode.o_OPT_LEVEL	= -g
runtest.o_OPT_LEVEL	= -g
test_vm.o_OPT_LEVEL	= -g
test_sunvm.o_OPT_LEVEL	= -g
test_anode_LDFLAGS	= -g

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif
.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif
.include <${RULES_MK}>

#
# Temporary, until make depend is provided
#
aggregate.o: anode.h epi_trace.h fixed_anode.h aggregate.h
anode.o: anode.h fixed_anode.h
anode_block.o: anode.h
block_alloc.o: anode.h fixed_anode.h
edbasic.o: anode.h
edecheck.o: anode.h
epia_volreg.o: anode.h
fid.o: anode.h
file.o: anode.h fixed_anode.h
findlog.o: anode.h fixed_anode.h aggregate.h
fixed_anode.o: anode.h fixed_anode.h
runtest.o: anode.h
salvage.o: anode.h fixed_anode.h sal_errors.h
strategy.o: anode.h epi_trace.h fixed_anode.h strategy.h
strat_sunos.o: anode.h epi_trace.h fixed_anode.h strategy.h
strat_aix.o: anode.h epi_trace.h fixed_anode.h strategy.h
test_anode.o: anode.h fixed_anode.h aggregate.h
test_sunvm.o: anode.h aggregate.h
test_vm.o: anode.h aggregate.h
tlock.o: anode.h
verify.o: anode.h fixed_anode.h aggregate.h
volume.o: anode.h fixed_anode.h
volume_table.o: anode.h fixed_anode.h

test_anode: libanode.a

newaggr: libedsk.a libanode.a
newaggr.o: anode.h fixed_anode.h aggregate.h

anode.h epi_errs.msf: anode.p.h epi_errs.et
	-${RM} -f ./anode.p.h
	-${CP} ${anode.p.h:P} .
	${COMPILE_ET} ${epi_errs.et:P} -h anode
	${RM} -f anode.p.h

dfsepi.cat: epi_errs.msf
	${GENCAT} $@ $>

epi_trace.h epi_trace.msf: epi_trace.et
	${COMPILE_ET} ${epi_trace.et:P} -h epi_trace

dfszea.cat: epi_trace.msf
	${GENCAT} $@ $>
