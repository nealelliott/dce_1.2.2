#
# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.61.1  1996/10/02  16:56:41  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:03:16  damon]
#
# Revision 1.1.57.2  1994/06/09  13:49:31  annie
# 	fixed copyright in src/file
# 	[1994/06/08  21:23:00  annie]
# 
# Revision 1.1.57.1  1994/02/04  20:03:47  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:05:31  devsrc]
# 
# Revision 1.1.55.1  1993/12/07  17:10:46  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/02  22:37:02  jaffe]
# 
# Revision 1.1.4.2  1993/01/18  19:48:22  cjd
# 	Embedded copyright notice
# 	[1993/01/18  19:21:27  cjd]
# 
# 	a few more fixes
# 	[92/01/07  18:35:33  garyf]
# 
# 	Merging dfs5.11 to dfs6.3
# 
# Revision 1.1.2.2  1992/01/24  01:47:19  devsrc
# 	Bring in dfs6.3 sources
# 	[1992/01/22  19:28:33  devsrc]
# 
# Revision 1.1.2.3  91/11/21  09:30:52  garyf
# 	fix a few problems
# 	[91/11/21  09:30:19  garyf]
# 
# Revision 1.1.2.2  91/11/20  17:38:18  garyf
# 	Initial coding.
# 	[91/11/20  17:35:19  garyf]
# 
# $EndLog$
#
# Makefile for DFS kernel extensions.
#
# Dependency tree is as follows:
#
#                          _ dcecm
#                         /
#                        /
#                       /
# dcerpc <--- dcebase  <---- dcefx
#                       \
#                        \
#                         \_ dceepi
#

SHARED_LIBRARIES= libdcebase.so libdcecm.so libdcefx.so libdceepi.so
OTHERS		= ${SHARED_LIBRARIES}

EXPLIB_TARGETS  = ${SHARED_LIBRARIES:S/^/export_/g}
EXPDIR		= /sbin/subsys/

ILIST		= ${SHARED_LIBRARIES}
IDIR		= /sbin/subsys/

KERNEL_CC	=
CFLAGS		= ${KERNELCFLAGS}

LDARGS		= -nostdlib -L${EXPORTBASE}/sbin/subsys

#
# DCE BASE
#
# Force inclusion of all contents of osi, xvnode, xcred, kutils, xaggr, ufsops, flserver.
# Let them drag in most/all of xvolume, tkc, tkm, volreg, and fp.
#
osi_OFILES	= ../osi.klib/afsl_trace.o    ../osi.klib/osi_pag.o         \
		  ../osi.klib/lock.o          ../osi.klib/osi_port_osf.o    \
		  ../osi.klib/osi_fio.o       ../osi.klib/osi_printf.o      \
		  ../osi.klib/osi_misc.o      ../osi.klib/osi_ufs.o         \
		  ../osi.klib/osi_net.o       ../osi.klib/uiomove.o
xvnode_OFILES   = ../xvnode/xvfs_osf2vfs.o   ../xvnode/xvfs_vfs2osf.o ../xvnode/xvfs_vnode.o \
		  ../xvnode/xvfs_osfglue.o   ../xvnode/xvfs_volume.o
xcred_OFILES	= ../xcred.klib/xcred.o 
kutils_OFILES	= ../kutils.klib/syscall.o   ../kutils.klib/com_lockctl.o
xaggr_OFILES	= ../xaggr.klib/ag_init.o    ../xaggr.klib/ag_registry.o
ufsops_OFILES   = ../ufsops/ufs_volops.o     ../ufsops/ufs_agops.o
flserver_OFILES = ../flserver.klib/afsvl_proc_cstub.o
xvolume_OFILES	= ../xvolume.klib/vol_init.o

libdcebase.so:		${libdcebase.so_OFILES}
libdcebase.so_OFILES	= dcebase_config.o ${osi_OFILES} ${xvnode_OFILES} ${xcred_OFILES} \
			  ${kutils_OFILES} ${xvolume_OFILES} ${xaggr_OFILES} \
			  ${ufsops_OFILES} ${flserver_OFILES}
libdcebase.so_LDFLAGS	= -export dcebase: -e dcebase_config
libdcebase.so_LIBS	= -lktkc -lktkm -lvolreg -lvolume -lkfp -lkolddacl -ldcerpc -lvmunix	

#
# DCE CM
#
# Force inclusion of cm, fsint[client], and ktc.
#
libdcecm.so_OFILES	= \
			../cm/cm_aclent.o      ../cm/cm_dkload.o      ../cm/cm_pcache.o      ../cm/cm_tokens.o \
			../cm/cm_bkg.o         ../cm/cm_dnamehash.o   ../cm/cm_pioctl.o      ../cm/cm_vcache.o \
			../cm/cm_cell.o        ../cm/cm_exporter.o    ../cm/cm_rrequest.o    ../cm/cm_vdirent.o \
			../cm/cm_chunk.o       ../cm/cm_express.o     ../cm/cm_scache.o      ../cm/cm_vfsops.o \
			../cm/cm_conn.o        ../cm/cm_expressops.o  ../cm/cm_server.o      ../cm/cm_vnodeops.o \
			../cm/cm_cuser.o       ../cm/cm_init.o        ../cm/cm_shutdown.o    ../cm/cm_volume.o \
			../cm/cm_daemons.o     ../cm/cm_lockf.o       ../cm/cm_subr.o        ../cm/cm_xsyscalls.o \
			../cm/cm_dcache.o      ../cm/cm_mariner.o     ../cm/cm_tknimp.o

libdcecm.so:		$(libdcecm.so_OFILES}
libdcecm.so_LDFLAGS	= -export dcecm: -e cm_configure
# libkafs4srv.a really belongs in libdcefx.so, but libkdfsncs.a (krpc_misc.o) requires it here.  
libdcecm.so_LIBS	= -lkdfsncs -lkafs4clt -lkrepcli -lkafs4srv -ldcebase -ldcerpc -lvmunix

#
# DCE FX
#
# Include px, tkset, fshost, fsint[server], and config(RPC runtime support for pipes).
#
libdcefx.so_OFILES	= px_config.o ../px/px_intops.o ../px/px_init.o ../px/px_subr.o ../px/px_remove.o \
			  ../px/px_repops.o  ../px/px_vlutils.o \
			  ../fsint.klib/tkn4int_cstub.o

libdcefx.so:		$(libdcefx.so_OFILES}
libdcefx.so_LDFLAGS	= -e fx_configure
libdcefx.so_LIBS	= -ltkset -lhost -ldcecm -ldcebase -ldcerpc -lvmunix

#
# DCE EPI
#
# Include all of vnops, and let that drag in eacl, logbuf, dir, anode, and async.
#
vnops_OFILES		= ../episode/vnops.klib/efs_dhops.o  ../episode/vnops.klib/efs_access.o \
			  ../episode/vnops.klib/efs_vattr.o  ../episode/vnops.klib/efs_dir.o    \
			  ../episode/vnops.klib/efs_misc.o   ../episode/vnops.klib/efs_vnodeops.o \
			  ../episode/vnops.klib/efs_vfsops.o ../episode/vnops.klib/efs_init.o \
			  ../episode/vnops.klib/efs_volops.o ../episode/vnops.klib/efs_agops.o

sysacl_OFILES		= ../sysacl.klib/xvnops_call.o

epi_libs		= ../episode/vnops.klib/libkefsops.a  ../episode/dir.klib/libkdir.a \
			  ../episode/eacl.klib/libkeacl.a     ../episode/anode.klib/libkanode.a \
			  ../episode/logbuf.klib/libklogbuf.a ../episode/async.klib/libkasync.a

libdceepi.so_OFILES	= epi_config.o ${vnops_OFILES} ${sysacl_OFILES}

libdceepi.so:		${libdceepi.so_OFILES} ${epi_libs}
libdceepi.so_LDFLAGS	= -e epi_configure
libdceepi.so_LIBS	= ${epi_libs} -lkdacl -lktools -ldcerpc -ldcebase -lvmunix

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists(${MAKETOP}/file/${TARGET_OS}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_OS}/machdep.mk"
.endif

.include <${RULES_MK}>
