# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  13:22:43  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:11:25  marty]
#
# Revision 1.1.4.1  1995/12/08  17:42:16  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:22:44  root]
# 
# Revision 1.1.2.3  1994/06/21  14:47:54  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:43:38  marrek]
# 
# Revision 1.1.2.2  1994/06/10  21:12:06  devsrc
# 	cr10871 - fix copyright
# 	[1994/06/10  14:52:45  devsrc]
# 
# Revision 1.1.2.1  1994/03/23  15:39:30  keutel
# 	creation
# 	[1994/03/21  13:38:20  keutel]
# 
# $EndLog$
#
#
#
# @SNI_COPYRIGHT@
#
# $RCSfile: Makefile,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:22:43 $
#

LIBRARIES		= librosx.a librosstep.a libdriv25.a

VPATH			= ../rodpat:../roexif:../roloif:../roropm:../rosepm:../rostep:../rouctr

librosx.a_EXPDIR	= /usr/lib/
librosstep.a_EXPDIR	= /usr/lib/
libdriv25.a_EXPDIR	= /usr/lib/

EXPLIB_TARGETS  	= ${LIBRARIES:S/^/export_/g}

INCFLAGS		= -I../roincl -I../../incl  -I../../../h

.if (${TARGET_MACHINE}=="HP800")
CFLAGS			+= -DRP0003_HP -DRC0008_LOG_V1 -DRC0010_CMXV4 \
			  -DRC0043_LuRosSpm ${${TARGET_MACHINE}_CFLAGS}
.else
CFLAGS			+= -DRC0008_LOG_V1 -DRC0010_CMXV4 \
			  -DRC0043_LuRosSpm ${${TARGET_MACHINE}_CFLAGS}
.endif

librosx.a_OFILES = \
	rospm00.o  rospm01.o  rospm02.o  rospm03.o  rospm04.o  roropm0.o  \
	rospm05.o  rospm06.o  rospm07.o  rouctr0.o  rouctr1.o  rouctr2.o  \
	rodpat02.o rodpat03.o rodpat04.o rodpat32.o roexif00.o roexif01.o \
	roexif04.o roexif05.o roexifx2.o roloif02.o roloif12.o roloif22.o \
	roropm2.o  roropm3.o  roropm4.o  roropmzy.o roropzz.o

librosstep.a_OFILES = \
	sproif0.o sploif02.o spcmx00.o  spuctr02.o spuctr1.o spuctr2.o \
	spropm1.o spropm2.o  spropmzy.o spropzz.o  spropm0.o spspm2.o  \
	spspm0.o  spstep22.o

libdriv25.a_OFILES = \
	roexif20.o roexif21.o

.include <${RULES_MK}>
