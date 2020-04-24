# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.32.2  1994/10/26  15:45:45  sasala
# 	UHYP: CR 12618 Implement S12Y in DFS flserver
# 	[1994/10/26  15:43:31  sasala]
#
# Revision 1.1.32.1  1994/10/20  19:29:30  sasala
# 	Implement S12Y in bosserver subcomponent -UYHP: 12618
# 	[1994/10/20  18:15:09  sasala]
# 
# Revision 1.1.27.10  1994/09/02  13:11:18  ruby
# 	Add ftserver to SAMIDL_SUBIDRS
# 	[1994/09/02  13:10:17  ruby]
# 
# Revision 1.1.27.9  1994/08/25  17:34:22  mckeen
# 	Added to samidl_subdirs
# 	[1994/08/25  17:22:01  mckeen]
# 
# Revision 1.1.27.8  1994/08/18  19:59:40  ruby
# 	Add episode, ftutil, ncscompat, and  scout to the SAMIDL_SUBDIRS list
# 	[1994/08/18  19:58:57  ruby]
# 
# Revision 1.1.27.7  1994/08/08  18:54:21  mckeen
# 	Cleaned up history log
# 	Added gateway
# 	[1994/08/08  18:54:03  mckeen]
# 
# Revision 1.1.27.6  1994/08/08  17:05:08  sasala
# 	Implement partial S12Y (Internationalization) for cm command,
# 	including general Makefile changes
# 	[1994/08/08  15:29:56  sasala]
# 
# Revision 1.1.27.5  1994/08/03  21:39:26  sasala
# 	Add cmd and gtx to the AUTOGEN pass for S12Y
# 	[1994/08/03  21:37:51  sasala]
# 
# Revision 1.1.27.4  1994/08/02  20:27:26  sasala
# 	Add dfs to SAMIDL_SUBDIRS
# 	[1994/08/02  20:25:44  sasala]
# 
# Revision 1.1.27.3  1994/07/13  22:19:15  devsrc
# 	merged with bl-10
# 	[1994/06/28  17:57:48  devsrc]
# 
# 	Added some subdirectories to SUBDIRS for proper in
# 	stall of .h files (ot 9837)
# 	[1994/03/08  21:19:56  mckeen]
# 
# Revision 1.1.27.2  1994/06/09  13:49:29  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:22:58  annie]
# 
# Revision 1.1.27.1  1994/02/04  21:45:11  devsrc
# 	Merged 1.0.3a into 1.1
# 	[1994/02/04  21:44:55  devsrc]
# 
# Revision 1.1.24.1  1993/12/07  17:10:43  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/02  22:36:42  jaffe]
# 
# $EndLog$
#
# This list should include all nodes in the source tree that build any 
# programs, or that install any files.  If they just build a library, or
# an include file, then put them under EXPLIB_SUBDIRS or EXPINC_SUBDIRS.

EPISODE_SUBDIRS = episode

SUBDIRS        = tools osi icl config kutils sysacl util bomb \
                 security fsint sys export zlc \
                 xaggr ncscompat ncsubik flserver \
                 xvolume ufsops xcred xvnode host fshost tpq tkm tkc \
		 cm px afsd pxd dfsbind ftutil ftserver rep \
                 bosserver userInt update bubasics bakserver \
		 bak butc bakutil gtx fsprobe scout gateway ${EPISODE_SUBDIRS} libafs

#
# This list should only contain directories which generate or export .h files
#
EPISODE_EXPINC = episode
EXPINC_SUBDIRS = tools config util osi kutils icl sysacl bomb security fsint \
                 export xaggr ncscompat ncsubik flserver \
                 xvolume ufsops agfs xcred volreg host fshost \
                 tpq tkm zlc tkset tkc xvnode cm px rep ftutil ftserver \
		 bosserver userInt update bubasics bakserver \
		 bak gtx fsprobe gateway ${EPISODE_EXPINC} dfsbind
#
# This list should contain dirs which build and/or export libraries
#
EPISODE_EXPLIB=episode

.if defined(NO_DFS)
EXPLIB_SUBDIRS = tools
SAMIDL_SUBDIRS	= tools
.else
EXPLIB_SUBDIRS = tools config config.klib util util.klib \
                 osi osi.klib kutils kutils.klib icl icl.klib \
                 sysacl bomb bomb.klib security fsint fsint.klib \
                 sys export \
                 xaggr xaggr.klib ncscompat ncsubik flserver flserver.klib \
                 xvolume.klib agfs ufsops xcred xcred.klib volreg fshost \
                 tpq tpq.klib tkm tkm.klib zlc tkset tkc xvnode cm px libafs \
                 ftutil ftserver bosserver userInt \
                 bubasics butm bakserver bak gtx fsprobe gateway \
                 ${EPISODE_EXPLIB} epidummy

SAMIDL_SUBDIRS	= xaggr dfsbind afsd pxd update fsprobe tools gtx ftserver \
		  userInt gateway episode ftutil ncscompat scout bak butc  \
		  bosserver flserver rep
.endif
.include <${RULES_MK}>



