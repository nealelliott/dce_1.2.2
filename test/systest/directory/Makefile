# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.11.2  1996/03/11  02:38:51  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:05:35  marty]
#
# Revision 1.1.11.1  1995/12/11  21:56:09  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/1  1994/06/29  14:37 UTC  bissen
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 
# 	HP revision /main/HPDCE01/2  1994/04/01  18:04 UTC  truitt
# 
# 	HP revision /main/HPDCE01/truitt_junk/1  1994/04/01  17:57 UTC  truitt
# 	move files to proper subdirectories
# 
# 	HP revision /main/HPDCE01/1  1994/01/24  16:18  bissen
# 	merge kk and hpdce01
# 	[1995/12/11  20:56:01  root]
# 
# Revision 1.1.9.3  1993/10/15  21:15:51  ohara
# 	SVR4 porting changes - 8596
# 	[1993/10/15  16:35:09  ohara]
# 
# Revision 1.1.9.2  1993/09/21  14:24:47  pellis
# 	Added gds back to SUBDIRS line.
# 	[1993/09/21  14:17:35  pellis]
# 
# Revision 1.1.9.1  1993/09/10  13:24:23  cmckeen
# 	Temporarily removed gds from SUBDIRS line
# 	[1993/09/10  13:24:05  cmckeen]
# 
# Revision 1.1.9.6  1993/08/27  21:09:22  giza
# 	fixed IDIRs for TET
# 	[1993/08/27  21:09:03  giza]
# 
# Revision 1.1.9.5  1993/08/25  16:30:02  giza
# 	Removed gds files.
# 	[1993/08/25  16:29:39  giza]
# 
# Revision 1.1.9.4  1993/08/25  14:32:59  giza
# 	Defuncted dcecdnam
# 	[1993/08/25  14:32:31  giza]
# 
# Revision 1.1.9.3  1993/08/24  19:58:07  bissen
# 	Added SUBDIRS = cds for adding dce1.0.2a system tests
# 	[1993/08/24  19:57:31  bissen]
# 
# Revision 1.1.9.2  1993/07/09  10:53:16  root
# 	Initial King Kong branch
# 	[1993/07/09  10:52:36  root]
# 
# Revision 1.1.7.5  1993/05/04  18:09:11  giza
# 	*** empty log message ***
# 
# Revision 1.1.7.4  1993/04/28  15:38:03  eheller
# 	Updated to reflect changes to dcecdnam.
# 	[1993/04/28  15:37:07  eheller]
# 
# Revision 1.1.7.3  1993/04/21  19:31:06  giza
# 	removed defuncted tests
# 	[1993/04/21  19:29:02  giza]
# 
# Revision 1.1.7.2  1993/03/30  18:54:05  mgm
# 	Checking into GAMERA
# 	[1993/03/30  18:53:11  mgm]
# 
# Revision 1.1.5.12  1993/02/01  20:24:16  mgm
# 	Added cdsabuse and dcesx.
# 	[1993/02/01  20:19:47  mgm]
# 
# Revision 1.1.5.11  1993/01/08  20:12:01  mgm
# 	Added ifdef for instrumented build.
# 	[1993/01/08  20:09:36  mgm]
# 
# Revision 1.1.5.10  1992/11/25  19:41:06  mgm
# 	Added tet* to install
# 	[1992/11/25  19:40:36  mgm]
# 
# Revision 1.1.5.9  1992/11/25  16:18:48  mgm
# 	Added tet* files to source control
# 	[1992/11/25  16:18:02  mgm]
# 
# Revision 1.1.5.8  1992/11/23  17:34:35  mgm
# 	Removed -lm from LIBS (not needed for shared libraries)
# 	[1992/11/23  12:30:49  mgm]
# 
# Revision 1.1.5.7  1992/11/20  21:17:22  mgm
# 	Added tet_scen to install
# 	[1992/11/20  21:15:53  mgm]
# 
# 	Fixed comment in prior check-in.
# 	[1992/11/17  12:26:14  mgm]
# 
# Revision 1.1.5.6  1992/11/17  12:26:36  mgm
# 	*** empty log message ***
# 
# Revision 1.1.5.5  1992/11/17  12:23:20  mgm
# 	Added run.cds and rpt.cds to install
# 	[1992/11/17  12:21:51  mgm]
# 
# Revision 1.1.5.4  1992/11/09  13:49:09  mgm
# 	Updated for HPUX
# 	[1992/11/09  13:48:34  mgm]
# 
# Revision 1.1.5.3  1992/11/03  17:10:23  mgm
# 	Added ILIST to keep track of what needs to get installed for TET
# 	[1992/11/03  17:09:45  mgm]
# 
# Revision 1.1.5.2  1992/11/03  15:59:22  mgm
# 	DCE 1.0.1 Release
# 	[1992/11/03  15:58:26  mgm]
# 
# Revision 1.1.1.6  1992/09/21  18:35:48  osfrcs
# 	    Loading drop Dce1_0_1fnl
# 
# $EndLog$

#
# Subdirectories in which system tests are built
#
.ifdef NO_GDS
SUBDIRS		= cds 
.else
SUBDIRS         = cds gds
.endif

.include <${RULES_MK}>
