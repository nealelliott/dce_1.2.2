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
#
# HISTORY
# $Log: osf.dce.mk,v $
# Revision 1.2.38.2  1996/03/09  23:25:15  marty
# 	Update OSF copyright year
# 	[1996/03/09  22:40:53  marty]
#
# Revision 1.2.38.1  1995/12/07  22:09:32  root
# 	Submit OSF/DCE 1.2.1
# 	[1995/12/07  21:08:27  root]
# 
# Revision 1.2.34.4  1994/05/31  16:06:53  rsalz
# 	config.mk is gone (OT CR 9663)
# 	[1994/05/31  16:06:02  rsalz]
# 
# Revision 1.2.34.3  1993/12/13  14:52:25  mario
# 	Made cosmetic changes
# 	[1993/12/13  14:51:42  mario]
# 
# Revision 1.2.34.2  1993/12/07  17:54:34  mario
# 	Changed for ode2.3 - < and > becomes this except for RULES_MK
# 	[1993/12/07  17:25:22  mario]
# 
# Revision 1.2.31.1  1993/10/15  21:19:05  ohara
# 	create a tet.mk
# 	[1993/10/12  21:25:56  ohara]
# 
# Revision 1.2.28.4  1993/09/22  20:25:55  zeliff
# 	Removed .s.o translation rule
# 	[1993/09/22  20:24:49  zeliff]
# 
# Revision 1.2.28.3  1993/07/30  18:03:04  zeliff
# 	Merging dce1.0.3i version to dce1.1
# 	[1993/07/30  18:02:36  zeliff]
# 
# Revision 1.2.28.2  1993/07/07  20:05:36  cjd
# 	Added SAMS to project tools definitions
# 	[1993/07/07  20:04:38  cjd]
# 
# Revision 1.2.26.2  1993/06/10  19:18:20  sommerfeld
# 	      Don't define _PICLIB_ or _GLUE_ on HP-UX
# 		(perhaps should be test for == "OSF1" instead?)
# 	      Don't include assembly rule for .s if NO_RECOMPILE defined.
# 	      [1993/06/07  16:09:00  sommerfeld]
# 
# 	Include file.mk for HP800 just like on PMAX.
# 	[1992/10/16  21:58:59  toml]
# 
# 	Removed setting of CC_OPT_LEVEL ... it
# 	should be set in osf.std.mk
# 	[1992/06/18  20:50:27  hopkins]
# 
# Revision 1.2.17.6  1993/01/26  21:12:10  john
# 	Moved the config specific platform specific stuff to the config.mk file.
# 	[1993/01/26  20:55:26  john]
# 
# Revision 1.2.17.5  1992/12/31  16:47:03  zeliff
# 	Embedding copyright notices
# 	[1992/12/30  19:50:55  zeliff]
# 
# Revision 1.2.17.4  1992/12/10  17:18:23  marty
# 	Set the variable NO_KRPC is NO_DFS is defined (OT# 5018).
# 	[1992/12/10  17:13:50  marty]
# 
# 	Fix for OT # 5102.  The environment variable USE_SHARED_LIBRARIES
# 	is not checked to see if it is defined before being
# 	referenced.
# 	[1992/12/03  16:38:06  marty]
# 
# Revision 1.2.17.3  1992/10/20  19:07:15  comuzzi
# 	Add public domain timezone code
# 	[1992/10/20  18:53:54  comuzzi]
# 
# Revision 1.2.17.2  1992/09/29  20:02:19  devsrc
# 	[OT 5373]  SNI/SVR4 Merge UPDATE: Moved  the line "PARSER_AID?=parser_aid"
# 	           from osf.std.mk to here.
# 	[1992/09/25  14:28:31  sekhar]
# 
# Revision 1.2.2.16  1992/06/29  14:22:20  zeliff
# 	Fixing syntax of .if statements
# 	[1992/06/29  14:05:28  zeliff]
# 
# Revision 1.2.2.15  1992/06/26  07:20:16  jim
# 	Removed setting of OPT_LEVEL for AIX 3.2
# 	[1992/06/10  17:33:02  jim]
# 
# 	Last change for AIX 3.2.
# 	[1992/06/05  16:30:17  jim]
# 
# 	Added -DAIX32 flag for AIX 3.2.
# 	[1992/06/04  16:55:40  jim]
# 
# 	Changes for AIX 3.2.
# 	[1992/05/29  18:33:03  jim]
# 
# Revision 1.2.2.14  1992/06/05  21:47:52  john
# 	Changed CC_OPT_LEVEL back to ?= from = to allow command line
# 	overriding.
# 	[1992/06/05  21:46:20  john]
# 
# Revision 1.2.2.13  1992/05/29  18:59:06  marty
# 	The "-nostdinc" option is GNU compiler specific. Get rid
# 	of it for now.
# 	[1992/05/29  18:58:14  marty]
# 
# Revision 1.2.2.12  1992/05/28  18:38:23  marty
# 	Added definitions for dfs kernel builds.
# 	[1992/05/28  18:36:36  marty]
# 
# Revision 1.2.2.11  1992/05/11  18:05:02  mhickey
# 	Moved shared library compiler defs to the bottom of the file, so
# 	that components can decide whether USE_SHARED_LIBRARIES should be set.
# 	Defect 2717.
# 
# 	Moved conditionals that turned off USE_SHARED_LIBRARIES for some subtrees
# 	into the appropriate component makefiles.
# 	Defect 3332.
# 	[1992/05/11  17:27:00  mhickey]
# 
# Revision 1.2.2.10  1992/04/16  14:32:05  marty
# 	Fix copyright marker and hardwired copyright notices
# 	[1992/04/16  14:29:24  marty]
# 
# Revision 1.2.2.9  1992/04/13  19:45:07  rec
# 	Added test/rpc/runtime/kncstest to list of directories not to be
# 	build 'shared'.
# 	[1992/04/13  18:51:57  rec]
# 
# Revision 1.2.2.8  1992/04/08  18:00:02  rec
# 	Added libafs to list of directories not to be built 'shared'.
# 	[1992/04/08  17:17:14  rec]
# 
# Revision 1.2.2.7  1992/04/07  19:16:56  mckeen
# 	DEFTOOLBASE includes trailing slash
# 	[1992/03/31  16:36:53  mckeen]
# 
# 	Added definitions for DCE tools
# 	[1992/03/13  15:40:32  mckeen]
# 
# Revision 1.2.2.6  1992/04/07  13:52:48  toml
# 	Force off USE_SHARED_LIBRARIES directories for file/episode/libefs.
# 	[1992/04/07  13:50:31  toml]
# 
# Revision 1.2.2.5  1992/04/02  21:59:21  toml
# 	Force off USE_SHARED_LIBRARIES for PMAX kernel components, including DFS, KRPC, KRPCTEST.
# 	[1992/04/02  21:58:31  toml]
# 
# Revision 1.2.2.4  1992/01/23  14:19:59  jim
# 	Change syntax of whether to include file.mk.
# 	[1992/01/23  14:19:44  jim]
# 
# Revision 1.2.2.3  1992/01/22  22:33:31  jim
# 	file.mk does not exist on AIX.  Temporary change until DFS merge.
# 	[1992/01/22  22:29:26  jim]
# 
# Revision 1.2.2.2  1992/01/21  20:33:44  mhickey
# 	Removed the project - specific definitions from this
# 	directory, and put them in Makeconf.  Removed the conditionals
# 	for LIBDCE and NEED_PROJECT_MAKE_DEFS from this file as well.
# 	[1992/01/21  20:33:07  mhickey]
# 
# Revision 1.2  1992/01/19  22:13:19  devrcs
# 	Dropping DCE1.0 OSF1_misc port archive
# 
# $EndLog$
#
#
#  The following history section comes from the work done in DCE1.0.
#  Although these revisions don't exist under rcs control; the 
#  comments give a good idea about how this file has developed over time. 
#
#
#  OSF DCE Version 1.0 
# Revision 1.3.5.2  91/07/15  15:38:38  zeliff
# 	Redirected make to look in local lbe/mk directory for
# 	component makefiles first.
# 
# Revision 1.3.4.5  91/07/11  15:49:55  melman
# 	Added components/ to the path of where to find each .mk file.
# 	[91/07/11  15:49:18  melman]
# 
# Revision 1.3.4.4  91/07/11  13:45:35  melman
# 	Fixed typo in the rpc section.
# 	[91/07/11  13:45:07  melman]
# 
# Revision 1.3.4.3  91/07/11  13:42:31  melman
# 	Applied first OSF changes to this file.
# 	It was originally the dce.50 version.
# 	I added a .if for threads.
# 	Also changed all the files to look in src/lbe/mk
# 	for the component .mk files instead of throughout
# 	the source tree.
# 	[91/07/10  16:42:41  melman]
# 
# Revision 1.3.4.2  91/07/11  13:07:08  zeliff
# 	Added "-I-" to GENINC lines.
# 	[91/07/11  12:49:01  zeliff]
# 
# Revision 1.3  91/07/10  15:01:53  devrcs
# 	Adding dce.50
# 
# Revision 1.1.2.3  91/04/09  08:55:00  mckeen
# 	Combined DCE and ODE Common Makefiles
# 	[91/04/09  08:48:00  mckeen]
# 
# Revision 1.1.2.2  91/03/22  19:34:08  mckeen
# 	Changes for Reno make
# 	[91/03/22  16:35:38  mckeen]
# 


.if !defined(_OSF_DCE_MK_) 
_OSF_DCE_MK_=

#
# Project tools definitions
#
COMPILE_ET?=${DEFTOOLBASE}compile_et
IDL?=${DEFTOOLBASE}idl
SAMS?=${DEFTOOLBASE}sams
MAVROS?=${DEFTOOLBASE}mavros
MAVCOD?=${DEFTOOLBASE}mavcod
MSG?=${DEFTOOLBASE}msg
PRS?=${DEFTOOLBASE}prs
PARSER_AID?=./parser_aid
ZIC?=${DEFTOOLBASE}zic

# Removed setting of CC_OPT_LEVEL ... it should be set in osf.std.mk ...
# Don't want -O because of bug potential, and don't want -g because of lack
# of disk
#CC_OPT_LEVEL?=

DCESHARED_PATH=/opt/dce
DCELOCAL_PATH=/opt/dcelocal
DCEPATHS ?= -DDCESHARED_PATH=\"/opt/dce\" -DDCELOCAL_PATH=\"/opt/dcelocal\"

#
# Apparently need this to find "local" imports for idl
#
.if ${TARGET_MACHINE} == "PMAX"
IDLINCFLAGS ?= -I.
.endif

####################################################
# project specific rules                           #
####################################################

_host_GENINC_=
_ansi_GENINC_=${x:L:!${GENPATH} -I.!} 
_traditional_GENINC_=${x:L:!${GENPATH} -I.!}
_writable_strings_GENINC_=${x:L:!${GENPATH} -I.!}
_CC_GENINC_=${_${_CCTYPE_}_GENINC_}

####################################################
# project specific  component modifications        #
####################################################
.if !empty(MAKESUB:Mlbe*)
.if ${TARGET_MACHINE} == "RIOS"
TARGET_FLAGS ?= -D_ALL_SOURCE -DAIX32
.endif
.endif

.if !empty(MAKESUB:Mdirectory/cds*)
.include "${MAKETOP}/directory/cds/cds.mk"
.endif

.if !empty(MAKESUB:Mdirectory/gds*)
.include "${MAKETOP}/directory/gds/gds.mk"
.endif

.if !empty(MAKESUB:Mdirectory/xds*)
.include "${MAKETOP}/directory/xds/xds.mk"
.endif

.if !empty(MAKESUB:Mdirectory/xom*)
.include "${MAKETOP}/directory/xom/xom.mk"
.endif

.if !empty(MAKESUB:Mdiskless*)
.if ${TARGET_MACHINE} == "RIOS"
CCTYPE?=traditional
.endif
.endif

.if !empty(MAKESUB:Mrpc*)
.include "${MAKETOP}/rpc/rpc.mk"
.endif

.if !empty(MAKESUB:Msecurity*)
.include "${MAKETOP}/security/security.mk"
.endif

.if !empty(MAKESUB:Mtest*)
.include "${MAKETOP}/test/test.mk"
.endif

.if !empty(MAKESUB:Mtest/tet*)
.include "${MAKETOP}/test/tet/tet.mk"
.endif

.if !empty(MAKESUB:Mtime*)
.include "${MAKETOP}/time/time.mk"
.endif

.if !empty(MAKESUB:Mthreads*)
.include "${MAKETOP}/threads/threads.mk"
.endif

.if ${TARGET_MACHINE}=="PMAX" || ${TARGET_MACHINE}=="HP800"
.if !empty(MAKESUB:Mfile*)
.include "${MAKETOP}/file/file.mk"
.endif
.endif

#
#  dfs kernel definitions
#  For setting "-G 18" and "-nostdinc" options.
#
_O_F_CFLAGS_=${_${_T_M_}_${OBJECT_FORMAT}_CFLAGS_}


.if ${TARGET_MACHINE} == "RIOS"
TARGET_FLAGS ?= -D_ALL_SOURCE -DAIX32
.endif

#
# Shared library definitions
#
#
.if !defined(USE_SHARED_LIBRARIES)
.if defined(NO_SHARED_LIBRARIES) || defined(USE_STATIC_LIBRARIES)
USE_SHARED_LIBRARIES=0
.else
USE_SHARED_LIBRARIES=1
.endif
.endif 


.if ${USE_SHARED_LIBRARIES}
_SHCCDEFS_=-D_SHARED_LIBRARIES
.if ${TARGET_OS} != "HPUX"
_PICLIB_=-pic-lib ${_GENLIB_} ${_CC_LIBDIRS_} ${_LIBS_}
_GLUE_=-%ld," -warn_nopic -glue"
.endif
.else
_SHCCDEFS_=
_PICLIB_=
_GLUE_=
.endif

_host_PICLIB_=
_ansi_PICLIB_=${_PICLIB_}
_traditional_PICLIB_=${_PICLIB_}
_writable_strings_PICLIB_=${_PICLIB_}
_CC_PICLIB_=${_${_CCTYPE_}_PICLIB_}

_host_GLUE_=
_ansi_GLUE_=${_GLUE_}
_traditional_GLUE_=${_GLUE_}
_writable_strings_GLUE_=${_GLUE_}
_CC_GLUE_=${_${_CCTYPE_}_GLUE_}


# The rest of the shared lib defs.  This has to go here to avoid
# stomping on per/component-directory definitions for LIBS.  Basically,
# If LIBS is not defined, make it -ldce.  
#
.if ${USE_SHARED_LIBRARIES}
LIBS ?= -ldce
.endif

# Setting for building without DFS.
.if defined (NO_DFS)
NO_KRPC=1
.endif
.endif


