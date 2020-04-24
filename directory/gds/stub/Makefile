#
#  @OSF_COPYRIGHT@
#  COPYRIGHT NOTICE
#  Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
#  ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
#  src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.2.103.2  1996/03/09  23:14:49  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:32:08  marty]
#
# Revision 1.2.103.1  1995/12/08  16:14:42  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  15:20:31  root]
# 
# Revision 1.2.101.5  1994/06/21  14:48:10  marrek
# 	June 1994 code submission.
# 	[1994/06/17  11:40:01  marrek]
# 
# Revision 1.2.101.4  1994/05/10  16:03:17  marrek
# 	Bug fixes and I18N for April 1994 submission.
# 	[1994/05/06  09:57:57  marrek]
# 
# 	Bug fixes for April 1994 submission.
# 	[1994/05/02  13:23:18  marrek]
# 
# Revision 1.2.101.3  1994/03/23  15:47:18  keutel
# 	March 1994 code drop
# 	[1994/03/21  16:33:26  keutel]
# 
# Revision 1.2.101.2  1994/02/22  19:21:51  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  11:50:19  marrek]
# 
# Revision 1.2.101.1  1993/12/08  03:47:56  mario
# 	Changes in order to build with ode2.3 make.
# 	[1993/12/08  03:47:16  mario]
# 
# Revision 1.2.99.5  1993/10/20  14:47:31  ohara
# 	svr4 changes
# 	[1993/10/20  02:20:21  ohara]
# 
# Revision 1.2.99.4  1993/10/19  17:12:44  keutel
# 	CR 9126
# 	[1993/10/19  16:57:42  keutel]
# 
# Revision 1.2.99.3  1993/10/14  19:13:09  keutel
# 	October 1993 code drop
# 	[1993/10/14  19:11:01  keutel]
# 
# 	Merged with changes from 1.2.99.2
# 	[1993/10/13  17:26:33  keutel]
# 
# 	October 1993 code drop
# 	[1993/10/09  17:57:54  keutel]
# 
# Revision 1.2.99.2  1993/10/05  16:03:34  treff
# 	DCT @ OSF
# 	Always make libshth.a, whther USE_SHARED_LIBRARIES or
# 	not, all platforms.  C and S stubs need it, but DON'T
# 	want to link with -ldce.
# 	.
# 	[1993/10/05  16:03:12  treff]
# 
# Revision 1.2.99.1  1993/10/04  17:37:51  keutel
# 	use of libdua instead of libdce for gds processes
# 	[1993/10/04  17:35:48  keutel]
# 
# Revision 1.2.7.2  1993/08/11  08:17:29  marrek
# 	July 1993 code drop.
# 	[1993/08/02  12:17:23  marrek]
# 
# Revision 1.2.5.4  1992/12/31  22:03:09  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  13:47:20  bbelch]
# 
# Revision 1.2.5.3  1992/11/27  18:26:34  marrek
# 	November 1992 code drop
# 	[1992/11/23  15:07:28  marrek]
# 
# Revision 1.2.5.2  1992/09/29  21:12:05  devsrc
# 	[hinman@sni] - Check in merges from SNI
# 	[1992/09/14  20:27:29  treff]
# 
# Revision 1.2.2.3  1992/06/30  21:00:10  melman
# 	GDS drop of 6/16/92
# 	[1992/06/30  20:41:18  melman]
# 
# Revision 1.2.2.2  1992/06/01  23:07:53  melman
# 	Added to d25evmgt.o_CFLAGS:
# 	    d25evmgt.o_CFLAGS       = ${${TARGET_MACHINE}_CFLAGS}
# 	[1992/06/01  19:03:41  melman]
# 
# 	New GDS merged drop
# 	[1992/05/29  12:29:41  melman]
# 
# Revision 1.2  1992/01/19  22:13:17  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.2.103.2 $ $Date: 1996/03/09 23:14:49 $
#

PROGRAMS	= gdscstub gdssstub
ILIST		= gdscstub gdssstub
INCFLAGS	= -I../h 
GARBAGE		= d24aaction.c d24conv.c d24evmgt.c d24iaction.c d24stub.c \
		  d24uaction.c

IDIR		= /bin/

d24stub.o_CFLAGS	=  -DCSTUB ${CFLAGS}
d24uaction.o_CFLAGS	=  -DCSTUB  ${CFLAGS}
d24iaction.o_CFLAGS	=  -DCSTUB ${CFLAGS}
d24aaction.o_CFLAGS	=  -DCSTUB ${CFLAGS}
d24conv.o_CFLAGS	=  -DCSTUB ${CFLAGS}
d24signal.o_CFLAGS	=  -DCSTUB ${CFLAGS}
d25cup.o_CFLAGS         =  -DCSTUB ${CFLAGS}

LDFLAGS			= ${${TARGET_MACHINE}_LDFLAGS}

gdscstub_OFILES	= \
		  d24stub.o d24uaction.o d24iaction.o \
		  d24aaction.o d24conv.o d24signal.o \
        	  d25cup.o d25raction.o d25ievent.o d25revent.o d25evmgt.o

gdssstub_OFILES = \
		  d25stub.o \
		  d25conv.o d25uaction.o d25iaction.o \
		  d25raction.o d25ievent.o d25revent.o d25evmgt.o d25signal.o

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>

d24uaction.c:	d25uaction.c
	rm -f $@
	cp $> $@

d24iaction.c:	d25iaction.c
	rm -f $@
	cp $> $@

d24aaction.c:	d25aaction.c
	rm -f $@
	cp $> $@

d24conv.c:	d25conv.c
	rm -f $@
	cp $> $@

d24signal.c:	d25signal.c
	rm -f $@
	cp $> $@

d24stub.c:	d25stub.c
	rm -f $@
	cp $> $@

