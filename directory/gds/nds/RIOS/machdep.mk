# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.4.2  1996/03/09  23:14:25  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:31:42  marty]
#
# Revision 1.1.4.1  1995/12/08  16:05:51  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:16:16  root]
# 
# Revision 1.1.2.7  1994/07/21  14:01:52  marrek
# 	Use gnu sources in FSF directory.
# 	[1994/07/21  12:40:59  marrek]
# 
# Revision 1.1.2.6  1994/07/11  21:06:12  keutel
# 	OT 10732: ndscomp runs during installation
# 	[1994/07/11  21:04:10  keutel]
# 
# Revision 1.1.2.5  1994/07/06  15:07:50  marrek
# 	July 1994 code submission.
# 	[1994/07/04  13:17:43  marrek]
# 
# Revision 1.1.2.4  1994/06/21  14:47:37  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:42:39  marrek]
# 
# Revision 1.1.2.3  1994/06/09  18:46:47  devsrc
# 	cr10871 - expand copyright
# 	[1994/06/09  18:19:52  devsrc]
# 
# Revision 1.1.2.2  1994/05/10  15:57:09  marrek
# 	Bug fixes.
# 	[1994/04/29  09:30:19  marrek]
# 
# 	April 1994 code submission.
# 	[1994/04/27  12:42:17  marrek]
# 
# Revision 1.1.2.1  1994/03/23  15:25:44  keutel
# 	creation
# 	[1994/03/21  15:53:40  keutel]
# 
# $EndLog$
# 
# $RCSfile: machdep.mk,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:14:25 $
#

LIBRARIES	= libNDS.a
LOCALFILES	+= ndsshr.exp
ILIST	= ${LIBRARIES} ${PROGRAMS} ${LOCALFILES}

libNDS.a_EXPDIR	= /usr/lib/
libNDS.a_IDIR   = /usr/lib/
LXFLAGS		= -v
libNDS.a_LIBS	= -bE:ndsshr.exp -bM:SRE -lc -lPW -T512 -H512 -ldce
ndscomp_LIBS	= -ly -ll -lPW ${LIBS}

EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}

gnu_regex.o_CFLAGS = -DSTDC_HEADERS=1 -DHAVE_STRING_H=1 -DREGEX_MALLOC ${CFLAGS}
gnu_regex1.o_CFLAGS = -DSTDC_HEADERS=1 -DHAVE_STRING_H=1 -DREGEX_MALLOC ${CFLAGS}

libNDS.a_OFILES = ndsshr.o
ndsshr.o_OFILES = \
		ndscheck.o   ndsxtimap.o  ndsreg.o \
		ndsgetopt.o  ndsmatch.o   ndsselect.o \
		ndsshmatt.o  ndssockmap.o ndsusrmap.o gnu_regex.o
libNDS.a: ndsshr.o
	ar -rv libNDS.a ndsshr.o

ndsshr.o:	${ndsshr.o_OFILES} ndsshr.exp
	${_LD_} -o ndsshr.o ${ndsshr.o_OFILES} ${LIBFLAGS} ${libNDS.a_LIBS}

gnu_regex.c:	FSF/gnu_regex.c
	rm -f $@
	cp $> $@

gnu_regex1.c:	FSF/gnu_regex.c
	rm -f $@
	cp $> $@

ndscomp_OFILES = \
		y.tab.o lex.yy.o ndscompil.o ndsreg1.o gnu_regex1.o
