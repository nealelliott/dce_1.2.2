# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.6.2  1996/03/09  23:27:29  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:42:58  marty]
#
# Revision 1.1.6.1  1995/12/08  17:42:23  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  17:18:19  root]
# 
# Revision 1.1.2.4  1994/08/05  13:49:20  wray
# 	Rename gss_msgs.sms to gss.sams
# 	[1994/08/05  13:47:26  wray]
# 
# Revision 1.1.2.3  1994/06/28  20:00:32  wray
# 	GSSAPI BL3
# 	[1994/06/28  19:59:47  wray]
# 
# Revision 1.1.2.2  1994/06/17  18:41:49  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:08:27  devsrc]
# 
# Revision 1.1.2.1  1994/03/09  15:50:42  wray
# 	Initial submission (OT#10019)
# 	[1994/03/09  15:39:07  wray]
# 
# $EndLog$
INCLUDES=gssapi.h dcegssmsg.h
EXPDIR = /usr/include/dce/

OTHERS = dcegss.cat
ILIST=${OTHERS} ${INCLUDES}


gssapi.h_EXPDIR = /usr/include/dce/
gssapi.h_IDIR = /share/include/dce/
dcegssmsg.h_EXPDIR = /usr/include/dce/
dcegssmsg.h_IDIR = /share/include/dce/
dcegss.cat_IDIR = /nls/msg/en_US.ASCII/

 
.include <${RULES_MK}>


dcegssmsg.h: gss.sams
	${DEFTOOLBASE}sams -o thmcsSu $>

