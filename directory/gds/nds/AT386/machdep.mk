# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.4.2  1996/03/09  23:14:15  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:31:33  marty]
#
# Revision 1.1.4.1  1995/12/08  16:04:46  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:15:55  root]
# 
# Revision 1.1.2.6  1994/07/21  14:01:43  marrek
# 	Use gnu sources in FSF directory.
# 	[1994/07/21  12:40:57  marrek]
# 
# Revision 1.1.2.5  1994/07/11  21:06:09  keutel
# 	OT 10732: ndscomp runs during installation
# 	[1994/07/11  21:04:08  keutel]
# 
# Revision 1.1.2.4  1994/07/06  15:07:49  marrek
# 	July 1994 code submission.
# 	[1994/07/04  13:17:39  marrek]
# 
# Revision 1.1.2.3  1994/06/09  18:46:32  devsrc
# 	cr10871 - expand copyright
# 	[1994/06/09  18:19:38  devsrc]
# 
# Revision 1.1.2.2  1994/05/10  15:56:58  marrek
# 	April 1994 submission.
# 	[1994/04/29  10:53:07  marrek]
# 
# Revision 1.1.2.1  1994/03/23  15:19:13  keutel
# 	creation
# 	[1994/03/21  15:53:12  keutel]
# 
# $EndLog$
# 
# $RCSfile: machdep.mk,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:14:15 $
#

SHARED_LIBRARIES	= libNDS.so
ILIST	= ${LIBRARIES} ${PROGRAMS} ${LOCALFILES} ${SHARED_LIBRARIES}

libNDS.so_EXPDIR	= /usr/shlib/
libNDS.so_IDIR	= /usr/shlib/
libNDS.so_LIBS  = -lc ${_GCC_EXEC_PREFIX_}gnulib -ldce
libNDS.so_LDFLAGS	+= -export_default libNDS:
LXFLAGS		= -v
ndscomp_LIBS	= -ly -ll ${LIBS}

EXPLIB_TARGETS	= ${SHARED_LIBRARIES:S/^/export_/g}


gnu_regex.o_CFLAGS = -DSTDC_HEADERS=1 -DHAVE_STRING_H=1 -DREGEX_MALLOC ${CFLAGS}
gnu_regex1.o_CFLAGS = -DSTDC_HEADERS=1 -DHAVE_STRING_H=1 -DREGEX_MALLOC ${CFLAGS}

libNDS.so_OFILES = \
		ndscheck.o   ndsxtimap.o  ndsreg.o \
		ndsgetopt.o  ndsmatch.o   ndsselect.o \
		ndsshmatt.o  ndssockmap.o ndsusrmap.o gnu_regex.o

gnu_regex.c:	FSF/gnu_regex.c
	rm -f $@
	cp $> $@

gnu_regex1.c:	FSF/gnu_regex.c
	rm -f $@
	cp $> $@

ndscomp_OFILES = \
		y.tab.o lex.yy.o ndscompil.o ndsreg1.o gnu_regex1.o

