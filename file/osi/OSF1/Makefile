# @OSF_COPYRIGHT@
# COPYRIGHT NOTICE
# Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
# ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
# the full copyright text.
#
# HISTORY
# $Log: Makefile,v $
# Revision 1.1.575.1  1996/10/02  17:58:11  damon
# 	Newest DFS from Transarc
# 	[1996/10/01  18:43:49  damon]
#
# Revision 1.1.569.2  1994/06/09  14:15:29  annie
# 	fixed copyright in src/file
# 	[1994/06/09  13:28:00  annie]
# 
# Revision 1.1.569.1  1994/02/04  20:24:40  devsrc
# 	Merged from 1.0.3a to 1.1
# 	[1994/02/04  15:15:41  devsrc]
# 
# Revision 1.1.567.1  1993/12/07  17:29:50  jaffe
# 	1.0.3a update from Transarc
# 	[1993/12/03  16:01:21  jaffe]
# 
# Revision 1.1.4.6  1993/01/21  14:50:21  cjd
# 	embedded copyright notice
# 	[1993/01/20  14:52:19  cjd]
# 
# Revision 1.1.4.5  1993/01/13  18:17:26  shl
# 	Transarc delta: kazar-ot6437-make-init-parent 1.1
# 	  Selected comments:
# 	    exiting threads weren't being wait3'd, and turned into zombies
# 	    add new file containing dummy for osi_MakeInitChild
# 	[1993/01/12  21:41:05  shl]
# 
# Revision 1.1.4.4  1992/12/10  22:27:30  marty
# 	Upgrade DFS to OSF/1 1.1.1.  [OT defect 5236]
# 	[1992/12/10  22:02:47  marty]
# 
# 	OSF 1.1.1 port
# 	[1992/11/30  23:07:01  marty]
# 
# Revision 1.1.4.3  1992/10/02  21:21:45  toml
# 	Added osi_sysconfig_mach.h.
# 	[1992/10/02  21:19:50  toml]
# 
# Revision 1.1.4.2  1992/08/31  20:24:10  jaffe
# 	Transarc delta: jaffe-ot4719-cleanup-gcc-Wall-in-osi 1.3
# 	  Selected comments:
# 	    Fixed many compiler warnings in the osi directory.
# 	    Reworked ALL of the header files.  All files in the osi directory now
# 	    have NO machine specific ifdefs.  All machine specific code is in the
# 	    machine specific subdirectories.  To make this work, additional flags
# 	    were added to the afs/param.h file so that we can tell if a particular
# 	    platform has any additional changes for a given osi header file.
# 	    changed osi_port_osf.h to osi_port_mach.h.
# 	    export osi_cred_mach.h, osi_net_mach.h, osi_kvnode_mach.h, and
# 	    osi_uio_mach.h
# 	    Corrected errors that appeared while trying to build everything on AIX3.2
# 	    cleanup for OSF1 compilation
# 	[1992/08/30  03:15:07  jaffe]
# 
# Revision 1.1.2.5  1992/05/05  03:55:39  mason
# 	delta jdp-ot2611-osi-add-copyrights 1.2
# 	[1992/04/24  22:10:16  mason]
# 
# Revision 1.1.2.4  1992/04/21  14:01:53  mason
# 	Transarc delta: tu-1143-error-mapping 1.8
# 	  Files modified:
# 	    cm: cm.h, cm_cell.c, cm_conn.c, cm_daemons.c, cm_dcache.c
# 	    cm: cm_dnamehash.c, cm_init.c, cm_pioctl.c, cm_rrequest.c
# 	    cm: cm_scache.c, cm_server.c, cm_subr.c, cm_tokens.c
# 	    cm: cm_vnodeops.c, cm_volume.c; fsprobe: fsprobe.c
# 	    libafs/RIOS: dfscore.exp; osi: Makefile, osi_dfserrors.h
# 	    osi.klib/OSF1: Makefile; osi.klib/RIOS: Makefile
# 	    osi/OSF1: Makefile, osi_dfs2osferr.c, osi_osf2dfserr.c
# 	    osi/RIOS: Makefile, osi_aix2dfserr.c, osi_dfs2aixerr.c
# 	    px: px_init.c, px_intops.c; rep: rep_main.c
# 	    userInt/fts: volc_tokens.c
# 	  Selected comments:
# 	    An enhancement to allow the DFS server to map returned kernel errors to the
# 	    canonical ones and to allow the CM to map the canonical ones to the
# 	    appropriate local kernel errors.
# 	    Please refer to the design memo, Error Conversion Protocol, for more detail.
# 	    The same as above.
# 	    Made osi_dfserrors.h and osi_dfs2aixerr.c osi_aix2dfserr.c osi_osf2dfserr.c and
# 	    osi_dfs2osferr.c avaliable in the user space. So, whoever references AFS_xxx
# 	    APIs should use the error conversion protocol.
# 	    Made osi_dfs2osferr.o and osi_osf2dfserr.o available to OSF1/libosi.a
# 	    fixed some typo errors and others
# 	    Changes did not compile on the PMAX.
# 	    DFS servers now maps errors to the canonical ones before returning to theirs
# 	    clients. Therefore, those clients who use AFS_xxx rpc calls must do the
# 	    error decoding work to get the appropriate error back.
# 	    CFE adds osi_initDecodeTable() and osi_errDecode() to repserver and fts.
# 	[1992/04/20  22:44:30  mason]
# 
# Revision 1.1.2.3  1992/04/14  04:09:54  mason
# 	Transarc delta: tu-1143-error-mapping 1.6
# 	  Files modified:
# 	    cm: cm.h, cm_cell.c, cm_conn.c, cm_daemons.c, cm_dcache.c
# 	    cm: cm_dnamehash.c, cm_init.c, cm_pioctl.c, cm_rrequest.c
# 	    cm: cm_scache.c, cm_server.c, cm_subr.c, cm_tokens.c
# 	    cm: cm_vnodeops.c, cm_volume.c; libafs/RIOS: dfscore.exp
# 	    osi: Makefile, osi_dfserrors.h; osi.klib/OSF1: Makefile
# 	    osi.klib/RIOS: Makefile
# 	    osi/OSF1: Makefile, osi_dfs2osferr.c, osi_osf2dfserr.c
# 	    osi/RIOS: Makefile, osi_aix2dfserr.c, osi_dfs2aixerr.c
# 	    px: px_init.c, px_intops.c
# 	  Selected comments:
# 	    An enhancement to allow the DFS server to map returned kernel errors to the
# 	    canonical ones and to allow the CM to map the canonical ones to the
# 	    appropriate local kernel errors.
# 	    Please refer to the design memo, Error Conversion Protocol, for more detail.
# 	    The same as above.
# 	    Made osi_dfserrors.h and osi_dfs2aixerr.c osi_aix2dfserr.c osi_osf2dfserr.c and
# 	    osi_dfs2osferr.c avaliable in the user space. So, whoever references AFS_xxx
# 	    APIs should use the error conversion protocol.
# 	    Made osi_dfs2osferr.o and osi_osf2dfserr.o available to OSF1/libosi.a
# 	    fixed some typo errors and others
# 	    Changes did not compile on the PMAX.
# 	[1992/04/13  17:02:32  mason]
# 
# Revision 1.1.2.2  1992/01/24  03:46:43  devsrc
# 	Fixed logs
# 	[1992/01/23  22:19:07  devsrc]
# 
# $EndLog$

# Copyright (C) 1992 Transarc Corporation - All rights reserved

VPATH			= ${TARGET_OS}
INCLUDES		+= osi_port_mach.h sysincludes.h \
	osi_cred_mach.h osi_net_mach.h osi_kvnode_mach.h \
	osi_uio_mach.h osi_sysconfig_mach.h osi_user_mach.h \
	osi_lock_mach.h osi_ufs_mach.h

libosi.a_OFILES         += osi_dfs2osferr.o osi_osf2dfserr.o osi_procutils.o

ILIST			+= osi_port_mach.h sysincludes.h \
	osi_cred_mach.h osi_net_mach.h \
	osi_uio_mach.h osi_sysconfig_mach.h osi_lock_mach.h
