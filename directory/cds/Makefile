# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.71.1  1996/10/04  14:38:46  arvind
# 	OSF DCE 1.2.2 Drop 4
# 	[1996/10/04  14:38:26  arvind]
#
# Revision /main/DCE_1.2.2/3  1996/09/24  14:02 UTC  arvind
# 	Fix build errors on HP/UX for PKSS/gloria
# 
# Revision /main/DCE_1.2.2/2  1996/09/23  21:45 UTC  arvind
# 	Conditionalize  HP build not to use gloria/gdb
# 	[1996/09/12  18:24 UTC  arvind  /main/arvind_pkss/3]
# 
# 	Conditionalize  HP build not to use gloria
# 	[1996/09/11  19:47 UTC  arvind  /main/arvind_pkss/2]
# 
# Revision /main/DCE_1.2.2/1  1996/09/09  21:18 UTC  arvind
# 	PKSS drop from DEC (DCE1.2.2)
# 	[1996/08/30  15:37 UTC  arvind  /main/arvind_pkss/1]
# 
# Revision 1.1.69.3  1996/03/09  23:11:23  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:28:47  marty]
# 
# Revision 1.1.69.2  1996/02/23  19:32:22  root
# 	Endgame: flag build of cdsbrowser
# 	[1996/02/23  19:31:53  root]
# 
# Revision 1.1.69.1  1995/12/08  00:09:47  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  23:53:02  root]
# 
# Revision 1.1.64.2  1994/04/07  10:22:15  kevins
# 	CR 10304 Export some header files for cross component dependency issues
# 	[1994/04/07  10:17:59  kevins]
# 
# Revision 1.1.64.1  1994/01/13  16:07:34  peckham
# 	Add SAMIDL_SUBDIRS definition.
# 	[1994/01/12  01:28:06  peckham]
# 
# Revision 1.1.4.2  1992/12/30  12:34:21  zeliff
# 	Embedding copyright notices
# 	[1992/12/29  22:16:16  zeliff]
# 
# Revision 1.1.2.2  1992/04/30  20:59:06  john
# 	Added SETUP_SUBDIRS.
# 	[1992/04/10  14:31:34  john]
# 
# Revision 1.1  1992/01/19  15:27:35  devrcs
# 	Initial revision
# 
# $EndLog$
#
#
# COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991. ALL RIGHTS RESERVED.
#
# THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
# ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
# INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
# COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
# OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
# TRANSFERRED.
#
# THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
# AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
# CORPORATION.
#
# DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
# SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
#
#
# Local naming top level makefile
#

# Directories to visit
.if ${TARGET_MACHINE}=="HP800"
SUBDIRS		= includes stubs library cache child server \
                  adver gda control ${NO_CDSBROWSER:D:Ucdsbrowser}
.else 
SUBDIRS		= includes stubs library cache child server \
                  adver gda control ${NO_CDSBROWSER:D:Ucdsbrowser} \
		  gloria/gdb
.endif
SETUP_SUBDIRS	= control
SAMIDL_SUBDIRS	= includes stubs
EXPINC_SUBDIRS	= ${SUBDIRS}
EXPLIB_SUBDIRS	= ${SUBDIRS}

.include <${RULES_MK}>
