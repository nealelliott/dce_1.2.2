# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
# @HP_COPYRIGHT@
# 
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.19.1  1996/10/02  17:53:37  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:41:38  damon]
#
# Revision 1.1.14.7  1994/08/23  19:30:42  rsarbo
# 	remove -u sec_cred_get_initiator from dfs_server.ext
# 	link line.
# 	[1994/08/23  19:30:14  rsarbo]
# 
# Revision 1.1.14.6  1994/08/23  18:58:44  rsarbo
# 	remove -lksec from dfs_server.ext, already linked in dce_krpc.ext
# 	[1994/08/23  16:42:47  rsarbo]
# 
# 	DFS delegation changes
# 	[1994/08/22  22:23:51  rsarbo]
# 
# Revision 1.1.14.5  1994/08/09  17:32:25  burati
# 	Merged with changes from 1.1.14.4
# 	[1994/08/09  17:32:10  burati]
# 
# 	DFS/EPAC/KRPC/dfsbind changes
# 	[1994/08/08  21:51:41  burati]
# 
# Revision 1.1.14.4  1994/08/08  18:54:45  mckeen
# 	Linking libgwauth with dfs_client.ext
# 	[1994/08/08  17:52:50  mckeen]
# 
# Revision 1.1.14.3  1994/06/09  14:12:28  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:25:53  annie]
# 
# Revision 1.1.14.2  1994/05/20  22:03:23  sommerfeld
# 	fix genpath invocation
# 	[1994/05/17  20:21:53  sommerfeld]
# 
# Revision 1.1.14.1  1994/02/04  20:21:58  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:14:36  devsrc]
# 
# Revision 1.1.12.1  1993/12/07  17:27:34  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  15:43:43  jaffe]
# 
# Revision 1.1.6.2  1993/07/19  19:31:52  zeliff
# 	HP port of DFS
# 	[1993/07/19  18:24:14  zeliff]
# 
# Revision 1.1.4.5  1993/07/16  19:50:32  kissel
# 	Move -laggr to dce_core.exp
# 	[1993/07/02  19:48:00  tmm]
# 
# 	Take out lilbexauth.a
# 	[1993/06/30  20:39:34  tmm]
# 
# 	Don't bother with building static kernels any more.
# 	Use :P syntax to find "exp" files.
# 	[1993/05/04  20:13:29  sommerfeld]
# 
# 	GAMERA Merge.
# 	[1993/04/09  20:23:33  toml]
# 
# 	*** empty log message ***
# 	[1993/06/21  14:30:34  kissel]
# 
# Revision 1.1.2.2  1993/06/04  14:52:00  kissel
# 	Initial HPUX RP version.
# 	[1993/06/03  21:39:38  kissel]
# 
# Revision 1.1.2.7  1993/03/15  16:13:53  toml
# 	Link with KRPC test suites by default.
# 	Add kernel extensions.
# 	[1993/03/15  16:09:31  toml]
# 
# Revision 1.1.2.6  1993/01/14  19:52:15  toml
# 	Add libagfs.a, and re-order libraries more like OSF1 platform.
# 	[1993/01/14  19:51:49  toml]
# 
# Revision 1.1.2.5  1992/11/23  19:41:31  kinney
# 	Added -lkzlc to afs_libs list (long comment in OSF1 version of this file)
# 	[1992/11/23  19:40:54  kinney]
# 
# Revision 1.1.2.4  1992/11/23  17:52:19  tmm
# 	10/23/92 	tmm	Add libexauth.a
# 	[1992/11/23  17:12:17  tmm]
# 
# Revision 1.1.2.3  1992/10/15  21:44:06  toml
# 	Initial coding for HPUX.  Re-order libraries in afs_libs for HPUX.
# 	[1992/10/15  21:40:12  toml]
# 
# Revision 1.1.2.2  1992/06/18  21:02:45  tmm
# 	06/17/92 tmm Created.
# 	[1992/06/18  18:56:33  tmm]
# 
# $EndLog$

VPATH			= ${TARGET_OS}:../../../export/${target_machine}/usr/lib

LIBRARIES		= libafs.a

KERNEL_CC		=

EXPDIR			= /usr/lib/
EXPLIB_TARGETS          = ${LIBRARIES:S/^/export_/g}
CFLAGS			= ${KERNELCFLAGS}
OFILES			= cm_config.o px_config.o dcebase_config.o

BUILD_EPISODE = true
.ifdef BUILD_EPISODE
EPI_OTHERS = dfs_episode.ext
.else
EPI_OTHERS =
.endif
OTHERS		= dfs_core.ext dfs_client.ext dfs_server.ext ${EPI_OTHERS} # afsvmunix efsvmunix

ILIST		= ${OTHERS}
IDIR		= /ext/

# DATAFILES	= dfs_core.exp

CONFIG		= ${KERNEL_${TARGET_MACHINE}_CONFIG:U${KERNEL_CONFIG:UDCE}}
LIBGENPATH	= ${_LIBFLAGS_:!${GENPATH} ${${.TARGET}_LIBFLAGS}!}

#
# Link the monolithic kernels.
#
#	afsvmunix == hp-ux + DFS
#
#	efsvmunix == hp-ux + DFS + EPISODE
#

#
# The order of libraries is carefully crafted so that all the code will be pulled
# by a single pass linker, when the kernel has only a single unresolved reference
# to DFS - dfs_link.
#
# dfs_link    is resolved in libafs (libafs/HPUX/dcebase_config.c)      and references afs_syscall.
# afs_syscall is resolved in libkdfskutil (kutils/syscall.c)            and references cm_configure(), px_configure()
# [px&cm]_configure is resolved in libafs (libafs/HPUX/[cm&px]_config.c)and cm_configure() references
#	afs_vfsops, and afscall_cm_*. These are resolved in libcm, and pull in most of libcm,
#	and from there, most of the references are resolved naturally.
#
afs_libs	= -lafs -lkdfskutil -lafs -lcm -laggr -lkdfsncs -lkdfskutil \
		-lpx -lhost -lkzlc -lktkset -lkxcred -lkrepcli \
		-lkosi -lkcommondata -lkicl -lkfldb -lkafs4srv \
		-lkafs4clt -lufsops -lxvnode -lagfs \
		-lktkc -lktkm -lktpq -lvolreg -lvolume -lkdacl -lkdacllfs 

afs_repeats	= -lkosi -lkdacl -lkzlc

afs_libs_dep	= libcm.a libkdfskutil.a libafs.a libaggr.a libkdfsncs.a \
		libpx.a libhost.a libktkm.a libktpq.a libktkset.a \
		libkxcred.a libkrepcli.a libkcommondata.a \
		libkosi.a libvolreg.a libkfldb.a libkafs4srv.a \
		libkafs4clt.a libufsops.a libxvnode.a \
		libagfs.a libktkc.a libvolume.a libkdacl.a libkdacllfs.a 

# Unset NO_KRPC_TESTS in your build environment to link KRPC test driver into kernel.
# (this requires a different version of conf.c)
.ifndef	NO_KRPC_TESTS
rpc_test_libs	= -lkncsdev -lkv2test
.else
rpc_test_libs   =
.endif

rpc_libs 	= ${rpc_test_libs} -lknck -lkidl -lkdes

rpc_libs_dep	= libknck.a libkidl.a libkdes.a

efs_libs 	= -lefs -lkefsops -lkdir -lkeacl -lkanode \
		-lklogbuf -lkasync  -lkdacllfs -lktools -lkolddacl \

efs_libs_dep	= libefs.a libkefsops.a libkdir.a libkeacl.a libkanode.a \
		libklogbuf.a libkasync.a libkdacllfs.a libktools.a \
		libkolddacl.a 

#
# The monolithic DFS kernel (without Episode).
#
afsvmunix: ${MAKETOP}/${OBJECTDIR}/kernel/${CONFIG}/hp-ux.sys  ${KERNEL_LDDEP}
	${CC_EXEC_PREFIX}/bin/ld ${KERNEL_LDOPTS} -o afsvmunix ${LIBDIRS} \
	-u dce_krpc_config -u dfs_core_config -u dfs_client_config -u dfs_server_config \
	${${MAKETOP}/${OBJECTDIR}/kernel/${CONFIG}/hp-ux.sys:P}  ${KERNEL_LDDEP} ${afs_libs} -lepidum ${afs_repeats} ${rpc_libs}

#
# The monolithic DFS kernel (with Episode).
#
efsvmunix: ${MAKETOP}/${OBJECTDIR}/kernel/${CONFIG}/hp-ux.sys  ${KERNEL_LDDEP} ${afs_libs_dep} ${rpc_libs_dep} ${efs_libs_dep}
	${CC_EXEC_PREFIX}/bin/ld ${KERNEL_LDOPTS} -o efsvmunix ${LIBDIRS} \
	-u dce_krpc_config -u dfs_core_config -u dfs_client_config -u dfs_server_config \
	-u dfs_episode_config \
	${${MAKETOP}/${OBJECTDIR}/kernel/${CONFIG}/hp-ux.sys:P} ${KERNEL_LDDEP} ${afs_libs} ${efs_libs} ${afs_repeats} ${rpc_libs} 

#
# DFS kernel extensions.
#
# Dependency tree is as follows:
#
#                          ---- dfs_client
#                         /
#                        /
#                       /
# dce_krpc <-- dfs_core <------ dfs_server
#                       \
#                        \
#                          ---- dfs_episode
#
#
# DFS_CORE
#
dfs_core_ext_LIBS	= ${libvolreg.a:P} ${libvolume.a:P} ${libkxcred.a:P} \
			  ${libkdfskutil.a:P} ${libkosi.a:P} \
			  ${libxvnode.a:P} ${libkcommondata.a:P} \
			  ${libktkc.a:P} ${libktkm.a:P} ${libkbomb.a:P} \
			  ${libkafsutil.a:P} ${libkolddacl.a:P} \
			  ${libkdacl.a:P} ${libkdacllfs.a:P} \
			  ${libktpq.a:P} ${libkzlc.a:P} ${libkicl.a:P} \
			  ${libkdfsncs.a:P} \
			  ${libkafs4clt.a}  ${libkfldb.a:P} ${libaggr.a:P}

dfs_core_LIBS            = -lvolreg -lvolume -lkxcred -lkdfskutil -lkosi \
			  -lkcommondata
			  -lkicl -lxvnode -lktkc -lktkm -lkbomb \
			  -lkafsutil -lkolddacl -lkdacl -lkdacllfs -lktpq \
			  -lkzlc -lkdfsncs -lkfldb -laggr

dfs_core.ext_LIBS       = ${dfs_core_LIBS} 
dfs_core.ext: ${dfs_core_ext_LIBS} ${OFILES} ${dfs_core.exp:P}
	${_LD_} -N -r -o $@ ${$@_LDFLAGS} \
	dcebase_config.o \
	-c ${dfs_core.exp:P} \
	${LIBGENPATH} ${LIBDIRS} ${$@_LIBS}

#
# DFS_CLIENT
#
dfs_client_ext_LIBS	= ${libcm.a:P} ${libkrepcli.a:P} \
			  ${libkafs4clt.a:P} ${libgwauth.a:P}

# Note that both dfs_core and dfs_client pull object files from libkafs4clt.a .
# 'dfscore' is pulling the marshalling routines from the IDL-generated '*aux' object file.
# 'dfs_client' is pulling the client stubs from the IDL-generated '*cstub' object file.
dfs_client_LIBS		= -lcm -lkrepcli -lgwauth

dfs_client.ext_LIBS	= ${dfs_client_LIBS}
dfs_client.ext: ${dfs_client_ext_LIBS} ${OFILES}
	${_LD_} -N -r -o $@ ${$@_LDFLAGS} \
	cm_config.o ../fsint.klib/afs4int_cstub.o ../fsint.klib/tkn4int_sstub.o \
	${LIBGENPATH} ${LIBDIRS} ${$@_LIBS}


#
# DFS_SERVER
#
# Include px, tkset, fshost, fsint[server], and config(RPC runtime support for pipes).
#
dfs_server_ext_LIBS	= ${libpx.a:P} ${libktkset.a:P} ${libhost.a:P} ${libagfs.a:P} ${libkafs4srv.a:P} ${libufsops.a:P} 

dfs_server_LIBS		= -lpx -lktkset -lhost -lkdacllfs -lagfs -lkafs4srv -lufsops

dfs_server.ext_LIBS	= ${dfs_server_LIBS}
dfs_server.ext: ${dfs_server_ext_LIBS} ${OFILES}
	${_LD_} -N -r -o $@ ${$@_LDFLAGS} px_config.o ${LIBGENPATH} ${LIBDIRS} ${$@_LIBS}

.ifdef BUILD_EPISODE
#
# DFS_EPISODE
#
# Include all of vnops, and let that drag in eacl, logbuf, dir, anode, and async.
#
dfs_episode.ext_LIBS	= ${efs_libs}
dfs_episode.ext: ${efs_libs_dep} ${OFILES}
	${_LD_} -N -r -o $@ ${$@_LDFLAGS} -u dfs_episode_config ${LIBGENPATH} ${LIBDIRS} ${$@_LIBS}
.endif

.if exists(${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk)
.include "${MAKETOP}/file/${TARGET_MACHINE}/machdep.mk"
.endif

.if exists (${TARGET_MACHINE}/Makefile)
.include "${TARGET_MACHINE}/Makefile"
.endif

.include <${RULES_MK}>


