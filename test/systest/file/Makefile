# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
# src directory for the full copyright text.
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.16.2  1996/03/11  02:43:28  marty
# 	Update OSF copyright years
# 	[1996/03/10  20:07:10  marty]
#
# Revision 1.1.16.1  1995/12/11  21:59:10  root
# 	Submit OSF/DCE 1.2.1
# 
# 	HP revision /main/HPDCE02/2  1994/10/11  20:52 UTC  bissen
# 	Merge Hpdce02_04 to HPDCE02 branch
# 	[1995/12/11  20:58:32  root]
# 
# Revision 1.1.13.2  1994/09/01  19:37:33  rdl
# 	Changes to fix problems with RPC where multiple servers on one machine
# 	[1994/09/01  19:35:44  rdl]
# 
# 	HP revision /main/HPDCE02/1  1994/07/05  19:44 UTC  bissen
# 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
# 
# Revision 1.1.13.1  1994/02/04  20:48:17  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:25:59  devsrc]
# 
# Revision 1.1.11.1  1994/01/27  21:04:19  cmckeen
# 	Added dfs.repfs and repfs.data to the OTHERS list.
# 	[1994/01/27  20:59:32  cmckeen]
# 
# Revision 1.1.9.1  1993/10/04  14:56:08  gmd
# 	Add add_root_as_admin.ksh, README and README.RWALL to
# 	list of files for installation. (CR 8884)
# 	[1993/10/04  14:55:36  gmd]
# 
# Revision 1.1.7.4  1993/07/30  15:43:09  gmd
# 	Updated to install dfs.maxdir, dfs.maxfile,
# 	dfs.block_frag and their data files. Also
# 	check_RC.ksh, a common routine for these
# 	scripts.
# 	[1993/07/30  15:02:37  gmd]
# 
# Revision 1.1.7.3  1993/07/13  15:29:40  dassarma
# 	Fixed and added entries for the script dfs.read_write_all.main.
# 	[1993/07/13  15:29:04  dassarma]
# 
# Revision 1.1.7.2  1993/06/10  20:06:15  mhickey
# 	Added conditionals to allow specification of debugging
# 	(build -DDFS_ALL_DEBUGS)
# 	and use of perror() (build -DUSE_PERROR)
# 
# 	Defect 8138.
# 	[1993/06/10  19:54:29  mhickey]
# 
# Revision 1.1.4.6  1993/03/09  21:28:52  gmd
# 	Remove unnecessary references to systest/tools libraries.
# 	[1993/03/09  21:27:55  gmd]
# 
# Revision 1.1.4.5  1993/02/12  21:38:13  gmd
# 	Updated deliverables with new scripts and checklists.
# 	[1993/02/12  20:57:45  gmd]
# 
# Revision 1.1.4.4  1993/02/05  15:20:21  cjd
# 	Embedded copyright notice
# 	[1993/02/05  14:40:30  cjd]
# 
# Revision 1.1.5.2  1993/02/04  22:07:14  cjd
# 	Embedded copyright notice
# 
# Revision 1.1.4.3  1993/01/22  15:21:52  pellis
# 	Made many changes to prepare for moving unsupported files to the
# 	./src/nosupport/test/systest directory.
# 	[1993/01/21  22:14:09  pellis]
# 
# Revision 1.1.4.2  1993/01/03  23:31:25  gmd
# 	Added filewnr and dirread to PROGRAMS
# 	Added OTHERS, SUPPORTED_PROGRAMS and ILIST to populate
# 	new (with 1.0.2) test install tree.
# 	[1993/01/03  23:26:08  gmd]
# 
# Revision 1.1.2.3  1992/04/15  16:53:07  rps
# 	Use tools libs
# 	[1992/04/15  15:43:09  rps]
# 
# Revision 1.1.2.2  1992/03/18  01:16:56  rps
# 	Initial version
# 	[1992/03/18  00:06:39  rps]
# 
# $EndLog$

########################################################
# Handle Flags for Make                                #
########################################################
# 
# debugging symbols
#
.if defined(DFS_ALL_DEBUGS)
  DEBUG_FLAGS=-g
.endif
#
# actually use perror in filewnr
#
.if defined(USE_PERROR)
  PERROR_IS_PERROR = -DPERROR_is_perror
.endif


INCFLAGS	= -I../tools

RIOS_CFLAGS	= -qlanglvl=ansi
CFLAGS		= ${DEBUG_FLAGS}${PERROR_IS_PERROR} -D_BSD -Dunix -D_NONSTD_TYPES -U__STR__ \
		  -DIDL_PROTOTYPES ${${TARGET_MACHINE}_CFLAGS}

LIBFLAGS	= -L../tools
LIBS		= -ldce ${${TARGET_MACHINE}_LIBS}

PROGRAMS	= filewnr dirread fmul.server fmul.client

IDLFILES	= fmul.idl
IDLFLAGS	= -keep c_source


dfs.glue_IDIR 	= /tet/file/ts/dfstest/
glue.data_IDIR	= /tet/file/ts/dfstest/
dirwrite.sh_IDIR = /tet/file/ts/dfstest/
filewnr_IDIR	= /tet/file/ts/dfstest/
dirread_IDIR	= /tet/file/ts/dfstest/

DATAFILES	= dfs.glue glue.data \
                  dfs.lock lock.data \
                  dfs.read_write_all.main dfs.read.write.data \
                  do.ksh profile \
                  dirwrite.sh \
                  dfs.repfldb_checklist \
                  dfs.repfs_checklist \
                  dfs.wan_checklist \
                  dfs.sec.cross_bind_checklist \
                  dfs.maxfile maxfile.data \
                  dfs.maxdir maxdir.data \
                  dfs.block_frag block_frag.data \
                  check_RC.ksh \
                  README README.RWALL


OTHERS		= dfs.glue glue.data \
		  dirwrite.sh \
		  dfs.lock lock.data \
		  dfs.read_write_all.main dfs.read.write.data \
		  do.ksh profile add_root_as_admin.ksh check_RC.ksh \
		  dfs.repfldb_checklist \
		  dfs.repfs_checklist \
		  dfs.wan_checklist \
		  dfs.sec.cross_bind_checklist \
		  dfs.maxfile maxfile.data \
		  dfs.maxdir maxdir.data \
		  dfs.block_frag block_frag.data \
		  dfs.repfs repfs.data \
		  README README.RWALL

ILIST		= ${PROGRAMS} ${OTHERS}

filewnr_OFILES = filewnr.o
dirread_OFILES = dirread.o
fmul.server_OFILES = fmul.server.o fmul.routines.o fmul_sstub.o
fmul.client_OFILES = fmul.client.o fmul_cstub.o

${fmul.server_OFILES} ${fmul.client_OFILES} : fmul_sstub.c fmul_cstub.c

.if exists(${TARGET_MACHINE}/machdep.mk)
.include "${TARGET_MACHINE}/machdep.mk"
.endif

.include <${RULES_MK}>
