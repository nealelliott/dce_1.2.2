# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/11  02:26:58  marty
# 	Update OSF copyright years
# 	[1996/03/10  19:36:07  marty]
#
# Revision 1.1.4.1  1995/12/11  15:15:36  root
# 	Submit
# 	[1995/12/11  14:32:09  root]
# 
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  23:17:05  root]
# 
# Revision 1.1.2.1  1994/09/28  21:46:00  melman
# 	moved admin tests to test/functional
# 	[1994/09/28  21:14:09  melman]
# 
# Revision 1.1.2.6  1994/09/19  19:51:00  baum
# 	Added DATAFILES
# 	[1994/09/19  19:50:30  baum]
# 
# Revision 1.1.2.5  1994/09/01  20:24:34  pwang
# 	Added secval_status_[NP].tcl
# 	[1994/09/01  20:14:12  pwang]
# 
# Revision 1.1.2.4  1994/06/10  20:16:02  devsrc
# 	cr10872 - fixed copyright
# 	[1994/06/10  16:56:25  devsrc]
# 
# Revision 1.1.2.3  1994/05/25  21:29:59  pwang
# 	Added secval_setup.tcl and secval_cleanup.tcl
# 	[1994/05/25  21:19:01  pwang]
# 
# Revision 1.1.2.2  1994/05/11  14:46:55  rousseau
# 	Updated to new install path and removed bad use of SCRIPTS (CR 10601).
# 	[1994/05/11  14:46:21  rousseau]
# 
# Revision 1.1.2.1  1994/05/04  22:37:41  pwang
# 	Initial Implementation
# 	[1994/05/04  22:10:45  pwang]
# 
# $EndLog$

SECVAL_FILES	= secval_activate_N.tcl secval_activate_P.tcl \
		  secval_deactivate_N.tcl secval_deactivate_P.tcl \
		  secval_ping_N.tcl secval_ping_P.tcl \
                  secval_status_N.tcl secval_status_P.tcl \
		  secval_update_N.tcl secval_update_P.tcl \
		  secval_setup.tcl secval_cleanup.tcl

IDIR		= /test/tet/functional/admin/dcecp/ts/secval/
ILIST		= ${SECVAL_FILES}
DATAFILES	= ${SECVAL_FILES}
IMODE		= 755

.include <${RULES_MK}>
