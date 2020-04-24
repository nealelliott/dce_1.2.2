# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/09  23:12:45  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:55  marty]
#
# Revision 1.1.4.1  1995/12/08  15:20:51  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:52:49  root]
# 
# Revision 1.1.2.2  1994/09/19  08:39:13  marrek
# 	Fixes for OT12176 (wrong incore tables.)
# 	[1994/09/19  08:31:11  marrek]
# 
# Revision 1.1.2.1  1994/06/21  14:39:24  marrek
# 	Created in June 94 code submission.
# 	[1994/06/10  16:03:21  marrek]
# 
# $EndLog$
# 
# $RCSfile: Makefile,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:12:45 $
# 

PROGRAMS        = gdssetup
OBJECTS         = server.init client.init

ILIST         	= ${PROGRAMS} ${OBJECTS}

IDIR          	= /bin/

server.init_IDIR = /share/var/adm/directory/gds/adm/
client.init_IDIR = /share/var/adm/directory/gds/adm/

gdssetup_OFILES = gsuset.o gsusteps.o

INCFLAGS      	= -I../../h -I../../gdscp/gct/h

gdssetup_LIBS	= -lgct ${LIBS}

.include <${RULES_MK}>
