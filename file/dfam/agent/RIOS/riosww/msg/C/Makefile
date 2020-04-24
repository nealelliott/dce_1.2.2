#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# Trade Secret of Hitachi, Ltd.
# Do not disclose without written concession from Hitachi, Ltd.
# Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.
#
# $Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/msg/C/Makefile,v 1.1.2.4 1996/03/11 14:06:31 marty Exp $
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.2.4  1996/03/11  14:06:31  marty
# 	Update OSF copyright years.
# 	[1996/03/11  13:56:10  marty]
#
# Revision 1.1.2.3  1996/03/07  06:53:46  takano
# 	modify IDIR
# 	[1996/03/07  06:53:26  takano]
# 
# Revision 1.1.2.2  1996/03/06  06:51:44  takano
# 	modify environment valiable for install_all
# 	[1996/03/06  06:51:27  takano]
# 
# Revision 1.1.2.1  1996/02/19  11:10:00  dce-sdc
# 	First Submit
# 	[1996/02/19  11:09:31  dce-sdc]
# 
# Revision 1.1.2.4  96/01/12  06:46:19  06:46:19  kishi (Y.Kishi)
# 	... ODE test ... and Change Copyright
# 	[1996/01/12  06:46:11  kishi]
# 
# Revision 1.1.2.3  1995/10/25  12:20:44  kishi
# 	Change Copyright.
# 	[1995/10/25  12:20:34  kishi]
# 
# Revision 1.1.2.2  1995/09/16  04:45:54  kishi
# 	Change IDIR. /usr/lib/nls/msg/C/ -> /riosww/usr/lib/nls/msg/C/
# 	[1995/09/16  04:45:25  kishi]
# 
# 	Change OTHERS.
# 	[1995/09/14  05:16:54  kishi]
# 
# Revision 1.1.2.1  1995/09/09  07:51:25  kishi
# 	Initial revision
# 	[1995/09/09  07:50:13  kishi]
# 
# $EndLog$
#

OTHERS	= dfaagt.cat setdfakey.cat

ILIST   = ${OTHERS}
IDIR    = /nls/msg/C/
IMODE   = 444
IOWNER	= root
IGROUP	= bin

${OTHERS}: $${.TARGET:.cat=.msg}
	gencat ${.TARGET} ${${.TARGET:.cat=.msg}:P}

.include<${RULES_MK}>
