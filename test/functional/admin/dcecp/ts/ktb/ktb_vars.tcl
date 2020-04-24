# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: ktb_vars.tcl,v $
# Revision 1.1.4.3  1996/03/11  02:17:14  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:32:22  marty]
#
# Revision 1.1.4.2  1995/12/13  15:26:38  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:53:41  root]
# 
# Revision 1.1.2.1  1994/09/28  21:40:40  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:09:55  melman]
# 
# Revision 1.1.2.2  1994/08/10  19:22:12  annie
# 	expand copyright with OSF copyright text
# 	[1994/08/10  17:16:31  annie]
# 
# Revision 1.1.2.1  1994/06/22  20:37:20  baum
# 	OT #11022 - Use tetexec.cfg for cell/host.  Use tet_setup
# 	[1994/06/22  20:36:14  baum]
# 
# $EndLog$

###
### ktb_vars.tcl - Global variables for use of ktb tests.
###

global DCP_KEYTAB_HOST DCP_KEYTAB_CELL

set TEST_PRINC1 test_princ1
set TEST_PRINC2 test_princ2
set KTB1_SHORT_NAME ktb1
set KTB2_SHORT_NAME ktb2

set KTB_CATALOG_OUTPUT "$KTB1_SHORT_NAME
$KTB2_SHORT_NAME"

set CUR_HOST_NAME $DCP_KEYTAB_HOST
set CUR_CELL_NAME $DCP_KEYTAB_CELL

set KTB1_LONG_NAME "$CUR_CELL_NAME/hosts/$CUR_HOST_NAME/config/keytab/$KTB1_SHORT_NAME"
set KTB2_LONG_NAME "$CUR_CELL_NAME/hosts/$CUR_HOST_NAME/config/keytab/$KTB2_SHORT_NAME"

set KTB1_LIST_OUTPUT "$CUR_CELL_NAME/$TEST_PRINC1
$CUR_CELL_NAME/$TEST_PRINC2"

set KTB1_UUID 004fbfe7-7c8d-1dc9-941c-0000c0dc0d4b
set KTB1_ANNOTATION Keytab_1_Annotation
set KTB1_STORAGE /tmp/ktb1.ktb

set KTB1_SHOW_ENTRY_OUTPUT "UUID: $KTB1_UUID
Object Name: $KTB1_SHORT_NAME
Annotation: $KTB1_ANNOTATION
Storage: $KTB1_STORAGE"

set KTB1_SHOW_MBR_OUTPUT "$CUR_CELL_NAME/$TEST_PRINC1    des    1
$CUR_CELL_NAME/$TEST_PRINC2    des    1
"

set KTB1_SHOW_OUTPUT "$KTB1_SHOW_ENTRY_OUTPUT
$KTB1_SHOW_MBR_OUTPUT"

set KTB2_UUID 007c8635-7dbb-1dc9-b19a-0000c0dc0d4b
set KTB2_ANNOTATION Keytab_2_Annotation
set KTB2_STORAGE /tmp/ktb2.ktb

set KTB2_LIST_OUTPUT "$KTB1_LIST_OUTPUT $CUR_CELL_NAME/$TEST_PRINC1"

set KTB2_LIST_AFTER_DEL_OUTPUT "$CUR_CELL_NAME/$TEST_PRINC1"
