# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:37:52  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:05:08  marty]
#
# Revision 1.1.4.1  1995/12/11  21:54:58  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  20:55:09  root]
# 
# Revision 1.1.2.5  1994/06/27  19:01:25  baum
# 	added run_setup.sh
# 	[1994/06/27  19:01:16  baum]
# 
# Revision 1.1.2.4  1994/06/27  18:26:39  baum
# 	Fixed install path and moved setup and misc.tcl files to lib directory
# 	[1994/06/27  18:26:14  baum]
# 
# Revision 1.1.2.3  1994/06/24  19:53:57  fadden
# 	Merged with changes from 1.1.2.2
# 	[1994/06/24  19:53:14  fadden]
# 
# 	Remove use of tet stubs (except for tet_infoline)
# 	[1994/06/24  18:24:14  fadden]
# 
# Revision 1.1.2.2  1994/06/17  18:46:04  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:36:32  devsrc]
# 
# Revision 1.1.2.1  1994/05/26  22:44:08  fadden
# 	Move use of tet down a level
# 	Add non-tet version of tet_infoline for modules where
# 	  dtetapi_stubs can't be used
# 	Add shell scripts
# 	[1994/05/26  20:10:16  fadden]
# 
# 	Freeze working dtet version
# 	[1994/05/13  15:41:12  fadden]
# 
# 	Initial Version
# 	[1994/04/25  19:58:49  fadden]
# 
# $EndLog$

INCFLAGS= -I../include -I../../../../../include

DATAFILES	= dcd_st_utils.tcl dcd_st_setup.tcl run_setup.sh

ILIST		= run.dced dcdrel.functions common_functions ${DATAFILES}
IDIR		= /test/tet/system/dced/ts/rel/

LIBRARIES	= libdcdtst.a 
EXPDIR		= /usr/lib/

OFILES = dcd_st_utils.o dcd_st_dbg.o tet_info_stub.o

.include <${RULES_MK}>


