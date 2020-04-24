# 
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.4.2  1996/03/09  23:14:40  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:31:59  marty]
#
# Revision 1.1.4.1  1995/12/08  16:14:14  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:39 UTC  dat
# 	Change sams dependancies.
# 
# 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:38 UTC  jrr
# 	Change sams dependancies.
# 
# 	HP revision /main/HPDCE02/2  1995/07/25  19:51 UTC  kline_s
# 	Merge changes for CHFts15806
# 
# 	HP revision /main/HPDCE02/kline_s_mothra/1  1995/07/21  22:51 UTC  kline_s
# 	Include dcegdsmsg.h in ILIST (CDS and XDS dependency on GDS messages).
# 
# 	HP revision /main/HPDCE02/1  1994/11/23  18:01 UTC  sommerfeld
# 	checkpoint (kernel & misc makefile cleanup)
# 	[1994/11/07  21:29 UTC  sommerfeld  /main/sommerfeld_mothmk_1/1]
# 
# Revision 1.1.2.9  1994/09/19  08:39:39  marrek
# 	Fixes for OT12176 (wrong incore tables.)
# 	[1994/09/19  08:33:52  marrek]
# 
# Revision 1.1.2.8  1994/07/06  15:08:10  marrek
# 	July 1994 code drop.
# 	[1994/07/06  12:02:57  marrek]
# 
# Revision 1.1.2.7  1994/06/21  14:47:56  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:43:41  marrek]
# 
# Revision 1.1.2.6  1994/06/10  21:15:37  devsrc
# 	cr10871 - fix copyright
# 	[1994/06/10  14:53:52  devsrc]
# 
# Revision 1.1.2.5  1994/05/10  16:02:56  marrek
# 	Merged with changes from 1.1.2.4
# 	[1994/05/10  15:44:32  marrek]
# 
# 	Bug fixes in April 1994 submission.
# 
# 	Bug fixes for April 1994 submission.
# 	[1994/05/02  13:21:01  marrek]
# 
# 	April 1994 code submission.
# 	[1994/04/27  13:09:56  marrek]
# 
# Revision 1.1.2.3  1994/03/23  15:46:42  keutel
# 	Merged with changes from 1.1.2.2
# 	[1994/03/23  15:45:35  keutel]
# 
# 	March 1994 code drop
# 	[1994/03/21  16:34:13  keutel]
# 
# Revision 1.1.2.2  1994/03/11  21:09:19  rsalz
# 	Use SAMSFLAGS variable to add the -f flag (OT CR 10122).
# 	[1994/03/11  14:13:16  rsalz]
# 
# Revision 1.1.2.1  1994/02/22  19:21:46  marrek
# 	February 1994 code drop
# 	[1994/02/17  16:20:09  marrek]
# 
# 	Creation for dce1.1.
# 	[1994/02/02  10:54:42  marrek]
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/09 23:14:40 $
#

SAMSFILES	= gds.sams
SAMIDL_INCLUDES	= dcegdssvc.h dcegdsmsg.h dcegdsmac.h
SAMS_TARGETS	= dcegdsmsg.h dcegdsmac.h dcegdssvc.c \
		  dcegds.cat dcegdsmsg.c

INCLUDES  = dcegdssvc.h dcegdsmsg.h dcegdsmac.h

EXPDIR = /usr/include/dce/

ILIST = dcegds.cat dcegdsmsg.h
dcegds.cat_IDIR = /nls/msg/en_US.ASCII/
dcegdsmsg.h_IDIR = /share/include/dce/

.include <${RULES_MK}>

