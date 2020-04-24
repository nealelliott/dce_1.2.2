# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  13:19:51  marty
# 	Update OSF copyright years
# 	[1996/03/11  13:08:59  marty]
#
# Revision 1.1.4.1  1995/12/08  17:31:36  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  16:18:01  root]
# 
# Revision 1.1.2.3  1994/06/10  21:06:56  devsrc
# 	cr10871 - fix copyright
# 	[1994/06/10  14:49:40  devsrc]
# 
# Revision 1.1.2.2  1994/05/10  15:59:31  marrek
# 	April 1994 code submission.
# 	[1994/04/27  12:53:46  marrek]
# 
# Revision 1.1.2.1  1994/03/23  15:34:01  keutel
# 	creation
# 	[1994/03/21  13:23:59  keutel]
# 
# $EndLog$
#
#
#
# @SNI_COPYRIGHT@
#
# $RCSfile: Makefile,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:19:51 $
#

PROGRAMS = gdsrosstep
ILIST	 = gdsrosstep

IDIR	 = /bin/

INCFLAGS = -I../../../h -I../../incl -I../roincl

gdsrosstep_LIBS = -lrosstep -lcomstep -lrosstep -lcomstep \
	       -lrtrosasn1 -lrosstep -lcomstep -lrtrosasn1 ${LIBS}

gdsrosstep_OFILES = trstep.o usrstep.o

.include <${RULES_MK}>
