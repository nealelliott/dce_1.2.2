# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.3  1996/03/11  02:14:53  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:30:34  marty]
#
# Revision 1.1.4.2  1995/12/13  15:25:46  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  15:52:54  root]
# 
# Revision 1.1.2.1  1994/09/28  21:39:27  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:08:32  melman]
# 
# Revision 1.1.2.5  1994/06/10  20:12:28  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:53:54  devsrc]
# 
# Revision 1.1.2.4  1994/05/11  14:46:51  rousseau
# 	Updated to new install path and removed bad use of SCRIPTS (CR 10601).
# 	[1994/05/11  14:46:18  rousseau]
# 
# Revision 1.1.2.3  1994/04/21  13:57:34  rousseau
# 	Merged with changes from 1.1.2.2
# 	[1994/04/21  13:57:26  rousseau]
# 
# 	Initial drop of dts and clock tests (CR 9938).
# 	[1994/04/21  13:55:46  rousseau]
# 
# Revision 1.1.2.2  1994/03/10  17:35:14  kevins
# 	CR10118 Changed SCRIPTS to TCL_CODE
# 	[1994/03/10  17:10:36  kevins]
# 
# Revision 1.1.2.1  1994/02/10  23:17:05  rousseau
# 	Initial test drop.
# 	[1994/02/10  23:09:05  rousseau]
# 
# $EndLog$

DTS_FILES	= dts_activate_N.tcl dts_activate_P.tcl \
		  dts_configure_N.tcl dts_configure_P.tcl \
		  dts_deactivate_N.tcl dts_deactivate_P.tcl \
		  dts_modify_N.tcl dts_modify_P.tcl \
		  dts_show_N.tcl dts_show_P.tcl \
		  dts_stop_N.tcl dts_stop_P.tcl \
		  dts_synchronize_N.tcl dts_synchronize_P.tcl \
		  clock_set_N.tcl clock_set_P.tcl \
		  clock_show_N.tcl clock_show_P.tcl \
		  clock_synchronize_N.tcl clock_synchronize_P.tcl
IDIR		= /test/tet/functional/admin/dcecp/ts/dts/
ILIST		= ${DTS_FILES}
DATAFILES	= ${DTS_FILES}
IMODE		= 755

.include <${RULES_MK}>
