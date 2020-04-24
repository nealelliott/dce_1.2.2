#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.67.2  1996/03/11  01:46:12  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:16:33  marty]
#
# Revision 1.1.67.1  1995/12/08  22:12:17  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  21:45:07  root]
# 
# Revision 1.1.65.5  1994/10/05  16:44:10  keutel
# 	OT 12431
# 	[1994/10/05  16:36:19  keutel]
# 
# Revision 1.1.65.4  1994/09/14  09:46:33  marrek
# 	Add acl2 and use -lcdsutil .
# 	[1994/09/13  13:05:30  marrek]
# 
# Revision 1.1.65.3  1994/07/06  15:09:32  marrek
# 	July 1994 code drop.
# 	[1994/07/06  12:03:00  marrek]
# 
# Revision 1.1.65.2  1994/03/23  16:06:34  keutel
# 	March 1994 code drop
# 	[1994/03/21  18:14:18  keutel]
# 
# Revision 1.1.65.1  1994/02/22  16:39:53  marrek
# 	Code drop for dce1.1.
# 	[1994/02/07  13:33:20  marrek]
# 
# Revision 1.1.63.1  1993/10/14  19:24:34  keutel
# 	October 1993 code drop
# 	[1993/10/14  19:23:26  keutel]
# 
# 	October 1993 code drop
# 	[1993/10/13  12:29:05  keutel]
# 
# 	October 1993 code drop
# 	[1993/10/11  12:42:45  keutel]
# 
# Revision 1.1.4.4  1993/01/05  19:59:53  htf
# 	Embedding copyright notice
# 	[1993/01/05  18:17:23  htf]
# 
# Revision 1.1.4.3  1992/12/01  14:58:06  marrek
# 	November 1992 code drop
# 	[1992/12/01  13:17:26  marrek]
# 
# Revision 1.1.4.2  1992/11/02  15:34:01  root
# 	Added ILIST for install pass
# 	[1992/10/28  20:14:44  root]
# 
# Revision 1.1.2.4  1992/07/28  20:12:39  melman
# 	Moved CFLAGS=-Xc to inside svr4
# 	[1992/07/28  19:47:31  melman]
# 
# Revision 1.1.2.3  1992/07/01  03:25:20  melman
# 	GDS drop of 6/16/92
# 	[1992/06/30  23:03:54  melman]
# 
# Revision 1.1.2.2  1992/06/11  21:52:46  melman
# 	New GDS test code drop.
# 	[1992/06/11  21:46:12  melman]
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.67.2 $ $Date: 1996/03/11 01:46:12 $
#

PROGRAMS          = example teldir acl acl2 cds thradd example_gds example_gds2
LIST              = example teldir acl acl2 cds thradd example_gds example_gds2
ILIST             = ${PROGRAMS}
DATAFILES         = thradd.in

EXPDIR            = /usr/lib/
EXPLIB_TARGETS    = ${LIBRARIES/^/export_}

example_gds_LIBS  = -ldce
example_gds2_LIBS = -ldce
example_LIBS      = -ldce
teldir_LIBS       = -ldce
acl_LIBS          = -ldce
acl2_LIBS         = -ldce
cds_LIBS          = -lcdsutil -ldce
thradd_LIBS       = -ldce

thradd.in: thradd.dat
	rm -f $@
	cp $> $@


.include <${RULES_MK}>

