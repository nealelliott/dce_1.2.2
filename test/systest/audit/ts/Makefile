# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:37:36  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:05:02  marty]
#
# Revision 1.1.4.1  1995/12/11  21:54:35  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:54:57  root]
# 
# Revision 1.1.2.4  1994/06/10  20:47:16  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  17:18:12  devsrc]
# 
# Revision 1.1.2.3  1994/05/23  14:38:20  bhaim
# 	Use dcecp
# 	[1994/05/23  14:37:23  bhaim]
# 
# Revision 1.1.2.2  1994/03/25  15:41:53  bhaim
# 	Add aud prefix
# 	[1994/03/25  15:41:33  bhaim]
# 
# Revision 1.1.2.1  1994/03/18  14:32:29  bhaim
# 	Initial drop
# 	[1994/03/18  14:29:46  bhaim]
# 
# $EndLog$
PROGRAMS		= try_logins audrel001
try_logins_OFILES	= try_logins.o
audrel001_OFILES	= audrel001.o
audrel001_LIBS		= -ltettcm -ltetapi -ldcetst -laudit -ldce
try_logins_LIBS		= -ldcetst -ldce
SCRIPTS			= audstr001 systest
ILIST			= ${PROGRAMS} ${SCRIPTS} 
IDIR			= /test/tet/system/audit/ts/


.include <${RULES_MK}>

