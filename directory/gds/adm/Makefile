#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.12.2  1996/03/09  23:12:33  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:29:44  marty]
#
# Revision 1.1.12.1  1995/12/08  15:10:03  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/08  14:49:21  root]
# 
# Revision 1.1.10.3  1994/06/21  14:43:08  marrek
# 	June 1994 code submission.
# 	[1994/06/16  09:22:06  marrek]
# 
# Revision 1.1.10.2  1994/05/10  15:49:07  marrek
# 	April 1994 code submission.
# 	[1994/04/27  12:08:40  marrek]
# 
# Revision 1.1.10.1  1994/02/22  15:50:34  marrek
# 	Code drop for dce1.1.
# 	[1994/02/08  09:11:15  marrek]
# 
# Revision 1.1.8.3  1993/08/10  08:04:55  marrek
# 	July 1993 code drop.
# 	[1993/07/30  08:02:04  marrek]
# 
# Revision 1.1.8.2  1993/07/02  15:06:06  marrek
# 	Changes for port to HP800.
# 	[1993/07/02  14:34:33  marrek]
# 
# Revision 1.1.4.5  1993/01/06  20:51:30  zeliff
# 	Added -f as a flag to CP of datafiles
# 	[1993/01/06  20:48:44  zeliff]
# 
# Revision 1.1.4.4  1992/12/31  17:43:15  bbelch
# 	Embedding copyright notice
# 	[1992/12/31  12:57:24  bbelch]
# 
# Revision 1.1.4.3  1992/11/27  15:32:50  marrek
# 	Introduce .dat files and remove dummy target.
# 	November 1992 code drop
# 	[1992/11/16  16:37:21  marrek]
# 
# Revision 1.1.4.2  1992/11/06  21:25:02  john
# 	Added if statements to check for existence before coping.
# 	[1992/10/23  02:17:33  john]
# 
# 	Added tests for file existence before doing the copy.
# 	      This prevents errors in subsequent builds of this directory.
# 	[1992/10/20  01:30:34  john]
# 
# Revision 1.1.2.3  1992/06/01  20:25:07  zeliff
# 	New version of file from GDS drop
# 	[1992/05/28  18:35:46  zeliff]
# 
# Revision 1.1.2.2  1992/04/07  19:07:10  mckeen
# 	Commented out DATAFILES
# 	[1992/03/13  15:39:19  mckeen]
# 
# Revision 1.1  1992/01/19  15:01:03  devrcs
# 	Initial revision
# 
# $EndLog$
#
# $RCSfile: Makefile,v $ $Revision: 1.1.12.2 $ $Date: 1996/03/09 23:12:33 $
#

EXPLIB_SUBDIRS	= adm_iapl adm_dump adm_sch admin delupd adm_tree usradmin
SUBDIRS		= admin usradmin delupd setup

LOCALFILES	= admscheme.dat common.dat newscheme.dat countries.dat adm_term_cap.dat nsapmacros.dat
DATAFILES	= ${LOCALFILES:S/.dat//g} 
ILIST		= ${LOCALFILES:S/.dat//g}
IDIR		= /share/var/adm/directory/gds/adm/

.include <${RULES_MK}>
admscheme:	admscheme.dat
	-${RM} ${_RMFLAGS_} admscheme
	-${CP} ${admscheme.dat:P} admscheme

common:		common.dat
	-${RM} ${_RMFLAGS_} common
	-${CP} ${common.dat:P} common

newscheme:	newscheme.dat
	-${RM} ${_RMFLAGS_} newscheme
	-${CP} ${newscheme.dat:P} newscheme

countries:	countries.dat
	-${RM} ${_RMFLAGS_} countries
	-${CP} ${countries.dat:P} countries

adm_term_cap:	adm_term_cap.dat
	-${RM} ${_RMFLAGS_} adm_term_cap
	-${CP} ${adm_term_cap.dat:P} adm_term_cap

nsapmacros:	${TARGET_MACHINE}/nsapmacros.dat
	-${RM} ${_RMFLAGS_} nsapmacros
	-${CP} ${${TARGET_MACHINE}/nsapmacros.dat:P} nsapmacros
