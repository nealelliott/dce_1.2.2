# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.3  1996/03/11  02:09:41  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:27:59  marty]
#
# Revision 1.1.4.2  1995/12/13  15:24:20  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1995/03/02  19:23 UTC  truitt
# 	Initial HPDCE02 branch.
# 
# 	HP revision /main/truitt_dcecp_chfts17/1  1995/03/02  19:22 UTC  truitt
# 	Add aud rewind tests.
# 	[1995/12/11  15:51:17  root]
# 
# Revision 1.1.2.1  1994/09/28  21:37:44  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:06:27  melman]
# 
# Revision 1.1.2.4  1994/06/10  20:02:22  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:52:30  devsrc]
# 
# Revision 1.1.2.3  1994/05/23  18:09:20  rousseau
# 	Various updates.
# 	[1994/05/23  18:08:53  rousseau]
# 
# Revision 1.1.2.2  1994/05/11  14:46:50  rousseau
# 	Updated to new install path and removed bad use of SCRIPTS (CR 10601).
# 	[1994/05/11  14:46:16  rousseau]
# 
# Revision 1.1.2.1  1994/04/21  20:28:24  rousseau
# 	Initial drop of audit tests (CR 9938).
# 	[1994/04/21  20:27:06  rousseau]
# 
# $EndLog$

VPATH		= ${TARGET_MACHINE}

AUD_FILES       = aud_disable_N.tcl aud_disable_P.tcl \
		  aud_modify_N.tcl aud_modify_P.tcl \
		  aud_stop_N.tcl aud_stop_P.tcl \
		  aud_enable_N.tcl aud_enable_P.tcl \
		  aud_rewind_N.tcl aud_rewind_P.tcl \
		  aud_show_N.tcl aud_show_P.tcl \
		  audevents_catalog_N.tcl audevents_catalog_P.tcl \
		  audevents_show_N.tcl audevents_show_P.tcl \
		  audtrail_show_N.tcl audtrail_show_P.tcl \
		  audfilter_catalog_N.tcl audfilter_catalog_P.tcl \
		  audfilter_create_N.tcl audfilter_create_P.tcl \
		  audfilter_delete_N.tcl audfilter_delete_P.tcl \
		  audfilter_modify_N.tcl audfilter_modify_P.tcl \
		  audfilter_show_N.tcl audfilter_show_P.tcl
TRAIL_FILES	= trail1 trail1.md_index trail2 trail2.md_index
IDIR		= /test/tet/functional/admin/dcecp/ts/aud/
ILIST		= ${AUD_FILES} ${TRAIL_FILES}
DATAFILES	= ${AUD_FILES} ${TRAIL_FILES}
IMODE		= 755

.include <${RULES_MK}>
