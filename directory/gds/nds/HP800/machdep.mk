# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.4.2  1996/03/09  23:14:19  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:31:37  marty]
#
# Revision 1.1.4.1  1995/12/08  16:05:30  root
# 	Submit OSF/DCE 1.2.1
# 
# 	Remove FSF software and add in HP's regcomp/regexec locally
# 	[1995/08/02  17:04 UTC  dtruong  /main/HPDCE02/dtruong_mothra1.1d/1]
# 
# 	HP revision /main/HPDCE02/3  1995/07/14  19:31 UTC  rmm
# 	merge back ux_release changes
# 
# 	HP revision /main/HPDCE02/rmm_wc_work/1  1995/06/29  22:41 UTC  rmm
# 	Change 1000 cond to work for Walnut Creek
# 
# 	HP revision /main/HPDCE02/2  1994/12/19  22:33 UTC  rmm
# 	merge back tab additions
# 
# 	HP revision /main/HPDCE02/rmm_mothra10/1  1994/12/19  22:29 UTC  rmm
# 	add tabs for translate rule commands
# 
# 	HP revision /main/HPDCE02/1  1994/12/13  20:34 UTC  dtruong
# 	10.0 merge back to main line
# 
# 	HP revision /main/dtruong_mothra1.1b/1  1994/10/10  21:16 UTC  dtruong
# 	Added hpux1000p for HP-UX 10.0
# 	[1995/12/08  15:31:17  root]
# 
# Revision 1.1.2.5  1994/07/11  21:06:09  keutel
# 	OT 10732: ndscomp runs during installation
# 	[1994/07/11  21:04:09  keutel]
# 
# Revision 1.1.2.4  1994/06/21  14:47:35  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:42:36  marrek]
# 
# Revision 1.1.2.3  1994/06/09  18:46:38  devsrc
# 	cr10871 - expand copyright
# 	[1994/06/09  18:19:45  devsrc]
# 
# Revision 1.1.2.2  1994/05/10  15:57:02  marrek
# 	April 1994 submission.
# 	[1994/04/29  10:53:09  marrek]
# 
# Revision 1.1.2.1  1994/03/23  15:19:20  keutel
# 	creation
# 	[1994/03/21  15:53:27  keutel]
# 
# $EndLog$
# 
# $RCSfile: machdep.mk,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:14:19 $
#

LIBRARIES	= libNDS.sl
ILIST	= ${LIBRARIES} ${PROGRAMS} ${LOCALFILES}

libNDS.sl_EXPDIR= /usr/lib/
libNDS.sl_IDIR  = /usr/lib/
libNDS.sl_LIBS  = -ldce
LXFLAGS		= -v -Xs100
ndscomp_LIBS	= -ly -ll  ${LIBS}

EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}
.if ${ux_release} >= 1000

libNDS.sl_OFILES = \
		ndscheck.o   ndsxtimap.o  ndsreg.o \
		ndsgetopt.o  ndsmatch.o   ndsselect.o \
		ndsshmatt.o  ndssockmap.o ndsusrmap.o hp_regex.o
libNDS.sl: ${libNDS.sl_OFILES}
	echo *.o
	${_LD_} -b -o ${@} ${libNDS.sl_OFILES} ${LIBFLAGS} ${libNDS.sl_LIBS}
	ar -rv libNDS.a ${libNDS.sl_OFILES}

hp_regex.c:   HP800/hp_regex.c
	rm -f $@
	cp $> $@

hp_regex1.c:  HP800/hp_regex.c
	rm -f $@
	cp $> $@

ndscomp_OFILES = \
		ndscompil.o y.tab.o lex.yy.o ndsreg1.o hp_regex1.o
.else
libNDS.sl_OFILES = \
                ndscheck.o   ndsxtimap.o  ndsreg.o \
                ndsgetopt.o  ndsmatch.o   ndsselect.o \
                ndsshmatt.o  ndssockmap.o ndsusrmap.o
libNDS.sl: ${libNDS.sl_OFILES}
	echo *.o
	${_LD_} -b -o ${@} ${libNDS.sl_OFILES} ${LIBFLAGS} ${libNDS.sl_LIBS}
	ar -rv libNDS.a ${libNDS.sl_OFILES}

ndscomp_OFILES = \
		ndscompil.o y.tab.o lex.yy.o ndsreg1.o
.endif
