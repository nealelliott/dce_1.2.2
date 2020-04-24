#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.9.2  1996/03/11  02:28:11  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:00:05  marty]
#
# Revision 1.1.9.1  1995/12/11  20:13:31  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/11  19:46:34  root]
# 
# Revision 1.1.7.2  1993/01/13  19:51:30  bbelch
# 	Embedding copyright notice
# 	[1993/01/06  18:18:58  bbelch]
# 
# Revision 1.1.2.6  1992/07/09  13:24:14  rsalz
# 	Remove comtest and nstest from subdir list; added by mistake in previous fix.
# 	[1992/07/09  13:22:15  rsalz]
# 
# Revision 1.1.2.5  1992/06/30  13:08:28  rsalz
# 	Add dependencies so IDL files get built when needed.
# 	[1992/06/30  13:08:13  rsalz]
# 
# Revision 1.1.2.4  1992/05/07  14:13:43  mishkin
# 	Remove "kdriver" from KSUBDIRS since whether to build it is platform-specific
# 	and it's just easier to leave it out, at least for now, and let people manually
# 	build it if they want it.
# 	[1992/05/07  14:12:28  mishkin]
# 
# Revision 1.1.2.3  1992/05/01  15:50:34  rsalz
# 	Edited as part of rpc6 drop.
# 	[1992/05/01  00:57:12  rsalz]
# 
# Revision 1.1.2.2  1992/02/05  17:42:58  eperkins
# 	*** empty log message ***
# 	[1992/02/05  17:42:00  eperkins]
# 
# Revision 1.1  1992/01/19  03:42:07  devrcs
# 	Initial revision
# 
# $EndLog$
#

.ifndef NO_KRPC
KEXPINC_SUBDIRS =
KSUBDIRS        = kncstest uncstest kv2test uv2test
.endif

SUBDIRS		= v2test_lib perf ncstest ${KSUBDIRS} 
EXPINC_SUBDIRS  =

.include <${RULES_MK}>
