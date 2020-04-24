# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: machdep.mk,v $
# Revision 1.1.4.2  1996/03/09  23:14:31  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:31:48  marty]
#
# Revision 1.1.4.1  1995/12/08  16:06:08  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:16:25  root]
# 
# Revision 1.1.2.7  1994/07/21  14:01:53  marrek
# 	Use gnu sources in FSF directory.
# 	[1994/07/21  12:41:00  marrek]
# 
# Revision 1.1.2.6  1994/07/11  21:06:12  keutel
# 	OT 10732: ndscomp runs during installation
# 	[1994/07/11  21:04:11  keutel]
# 
# Revision 1.1.2.5  1994/07/06  15:07:52  marrek
# 	July 1994 code submission.
# 	[1994/07/04  13:17:46  marrek]
# 
# Revision 1.1.2.4  1994/06/21  14:47:39  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:42:45  marrek]
# 
# Revision 1.1.2.3  1994/06/09  18:46:54  devsrc
# 	cr10871 - expand copyright
# 	[1994/06/09  18:20:00  devsrc]
# 
# Revision 1.1.2.2  1994/05/10  15:57:14  marrek
# 	April 1994 submission.
# 	[1994/04/29  14:21:18  marrek]
# 
# Revision 1.1.2.1  1994/03/23  15:26:01  keutel
# 	creation
# 	[1994/03/21  15:53:55  keutel]
# 
# $EndLog$
# 
# $RCSfile: machdep.mk,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:14:31 $
#

LIBRARIES	= libNDS.so
ILIST	= ${LIBRARIES} ${PROGRAMS} ${LOCALFILES}

libNDS.so_EXPDIR= /usr/lib/
libNDS.so_IDIR  = /usr/lib/
libNDS.so_LIBS  = -lc -ly -ll -ldce
LXFLAGS		= -v
ndscomp_LIBS	= -ly -ll ${LIBS}

EXPLIB_TARGETS	= ${LIBRARIES:S/^/export_/g}

ndsxtimap.o_CFLAGS = -D SNI_SINIX ${CFLAGS}
lex.yy.o_CFLAGS = -D SNI_SINIX ${CFLAGS}
gnu_regex.o_CFLAGS = -DSTDC_HEADERS=1 -DHAVE_STRING_H=1 -DREGEX_MALLOC ${CFLAGS}
gnu_regex1.o_CFLAGS = -DSTDC_HEADERS=1 -DHAVE_STRING_H=1 -DREGEX_MALLOC ${CFLAGS}

libNDS.so_OFILES = \
		ndscheck.o   ndsxtimap.o  ndsreg.o \
		ndsgetopt.o  ndsmatch.o   ndsselect.o \
		ndsshmatt.o  ndssockmap.o ndsusrmap.o gnu_regex.o
libNDS.so: ${libNDS.so_OFILES}
	echo *.o
	${_CC_} -G -o ${@} ${libNDS.so_OFILES} ${LIBFLAGS} ${libNDS.so_LIBS}
	ar -rv libNDS.a ${libNDS.so_OFILES}

gnu_regex.c:	FSF/gnu_regex.c
	rm -f $@
	cp $> $@

gnu_regex1.c:	FSF/gnu_regex.c
	rm -f $@
	cp $> $@

ndscomp_OFILES = \
		y.tab.o lex.yy.o ndscompil.o ndsreg1.o gnu_regex1.o
