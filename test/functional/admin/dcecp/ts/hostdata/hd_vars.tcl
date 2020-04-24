# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: hd_vars.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:16:04  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:31:39  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:12  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/10/19  16:43 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts50/1  1995/10/19  16:39 UTC  truitt
# 	CHFts16529: Test changes to reflect new dced 'local' functionality.
# 	[1995/12/11  15:53:17  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:53  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:10  melman]
# 
# Revision 1.1.2.2  1994/08/10  19:22:11  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:30  annie]
# 
# Revision 1.1.2.1  1994/06/22  20:42:40  baum
# 	OT #11022 - Use tet_setup, use tetexec.cfg for cell/host
# 	[1994/06/22  20:40:13  baum]
# 
# $EndLog$

###
### hd_vars.tcl - Global variables for Hostdata tests
###

global DCP_HOSTDATA_HOST DCP_HOSTDATA_CELL

set HD1_SHORT_NAME hd1
set HD2_SHORT_NAME hd2

set CUR_HOST_NAME $DCP_HOSTDATA_HOST
set CUR_CELL_NAME $DCP_HOSTDATA_CELL

set HD1_LONG_NAME "$CUR_CELL_NAME/hosts/$CUR_HOST_NAME/config/hostdata/$HD1_SHORT_NAME"
set HD1_UUID 004fbfe7-7c8d-1dc9-941c-0000c0dc0d4b
set HD1_ANNOTATION Hostdata_1_Annotation
set HD1_STORAGE /tmp/hd1.hd

set HD2_LONG_NAME "$CUR_CELL_NAME/hosts/$CUR_HOST_NAME/config/hostdata/$HD2_SHORT_NAME"
set HD2_UUID 007c8635-7dbb-1dc9-b19a-0000c0dc0d4b
set HD2_ANNOTATION Hostdata_2_Annotation
set HD2_STORAGE /tmp/hd2.hd

set LINE1 {this_is_line1}
set LINE2 {this_is_line2}
set MODIFIED_LINE {this_is_the_modified_line}
