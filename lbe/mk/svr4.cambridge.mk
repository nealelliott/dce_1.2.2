#
# @OSF_FREE_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1993, 1992, 1991, 1990, 1996 Open Software Foundation, Inc.
# Permission is hereby granted to use, copy, modify and freely distribute
# the software in this file and its documentation for any purpose without
# fee, provided that the above copyright notice appears in all copies and
# that both the copyright notice and this permission notice appear in
# supporting documentation.  Further, provided that the name of Open
# Software Foundation, Inc. ("OSF") not be used in advertising or
# publicity pertaining to distribution of the software without prior
# written permission from OSF.  OSF makes no representations about the
# suitability of this software for any purpose.  It is provided "as is"
# without express or implied warranty.
#
# (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
# All Rights Reserved
#
#	SNI sinix (MX300i) local makefile
#		defines variables local to site
#	@(#)svr4.cambridge.mk	1.10
# 
# HISTORY
# $Log: svr4.cambridge.mk,v $
# Revision 1.1.8.2  1996/03/09  23:25:31  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:41:07  marty]
#
# Revision 1.1.8.1  1995/12/07  22:11:10  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:09:04  root]
# 
# Revision 1.1.6.2  1993/06/24  20:08:16  hinman
# 	[hinman@sni] - Check in SNI merged version (these files have no tainted or private code
# 	[1993/06/17  14:37:33  hinman]
# 
# Revision 1.1.3.5  1993/02/01  20:27:04  hinman
# 	[hinman@sni] - Final merge before bsubmit
# 	[1993/01/31  15:58:52  hinman]
# 
# Revision 1.1.3.4  1992/12/31  16:48:11  zeliff
# 	Embedding copyright notices
# 	[1992/12/30  19:52:24  zeliff]
# 	Revision 1.1.4.2  1993/01/11  16:10:03  hinman
# 	[hinman] - Check in merged SNI version
# 
# Revision 1.1.3.3  1992/12/23  17:35:12  john
# 	Fixed the copyright marker to be OSF_FREE_COPYRIGHT.
# 	[1992/12/23  17:14:52  john]
# 
# Revision 1.1.3.2  1992/09/29  20:02:39  devsrc
# 	New file for SVR4 OSF merge
# 
# 	Submitted by STeve Gilbane
# 	[1992/09/11  15:32:48  rec]
# 
# $EndLog$
# 

.if !defined(_CAMBRIDGE_DCE_MK_)
_CAMBRIDGE_DCE_MK_=

#
# To add things to svr4.cambridge.mk:
#	Put HARDWIRED variable assignments BEFORE <svr4.sinix.mk>
#	Put variable assignments that depend on other variables AFTER <svr4.sinix.mk>
#	Put new rules AFTER including svr4.sinix.mk 
#

.if defined(WORKON)
ODEINCDIRS      !=      sbinfo incdirs
INCDIRS         +=      ${ODEINCDIRS}
ODELIBDIRS      !=      sbinfo libdirs
LIBDIRS         +=      ${ODELIBDIRS}
.endif

SNILIBS		?=

# this is still required for some GDS builds - prevents use of NFS lockd
SNI_CAMBRIDGE	= true

ZIC		?= ${DEFTOOLBASE}zic

.if ${TARGET_OS_PLATFORM} == "SINIX"
.include <svr4.sinix.mk>
.elif ${TARGET_OS_PLATFORM} == "SVR4V3"
.include <svr4.v3.mk>
.elif ${TARGET_OS_PLATFORM} == "PYRMIPS"
.include <svr4.rm600.mk>
.endif

.endif
