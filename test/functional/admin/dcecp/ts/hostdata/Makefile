# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.3  1996/03/11  02:15:49  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:28  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:06  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1995/03/03  15:14 UTC  truitt
# 	Add cleanup files.
# 
# 	HP revision /main/HPDCE02/truitt_dcecp_chfts17/1  1995/03/03  15:13 UTC  truitt
# 	Add hostdata cleanup.
# 
# 	HP revision /main/HPDCE02/1  1994/11/07  20:41 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_01/1  1994/11/07  20:37 UTC  truitt
# 	Add the new file for hostdata setup.
# 	[1995/12/11  15:53:11  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:48  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:05  melman]
# 
# Revision 1.1.2.4  1994/06/22  20:42:38  baum
# 	Merged with changes from 1.1.2.3
# 	[1994/06/22  20:42:11  baum]
# 
# 	OT #11022 - Use tet_setup, use tetexec.cfg for cell/host
# 	[1994/06/22  20:40:10  baum]
# 
# Revision 1.1.2.3  1994/06/17  18:43:09  devsrc
# 	cr10872 - fix copyright
# 	[1994/06/17  18:25:03  devsrc]
# 
# Revision 1.1.2.2  1994/05/25  16:51:00  baum
# 	ACL's on hostdata entry now setup automatically for tests
# 	[1994/05/25  16:48:24  baum]
# 
# Revision 1.1.2.1  1994/05/19  22:52:30  baum
# 	Initial checkin
# 	[1994/05/19  22:48:45  baum]
# 
# $EndLog$

HD_FILES	= hostdata_delete_N.tcl hostdata_delete_P.tcl \
                  hostdata_show_N.tcl hostdata_show_P.tcl \
                  hostdata_catalog_N.tcl hostdata_catalog_P.tcl \
                  hostdata_create_N.tcl hostdata_create_P.tcl \
                  hostdata_modify_N.tcl hostdata_modify_P.tcl \
		  hd_misc.tcl hd_vars.tcl hd_setup.tcl hd_cleanup.tcl
HD_DATA		= hd_setup.sh hd_cleanup.sh

IDIR		= /test/tet/functional/admin/dcecp/ts/hostdata/
ILIST		= ${HD_FILES} ${HD_DATA}
DATAFILES	= ${HD_FILES} ${HD_DATA}
IMODE		= 755

.include <${RULES_MK}>
