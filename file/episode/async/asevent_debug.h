/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: asevent_debug.h,v $
 * Revision 1.1.12.1  1996/10/02  17:20:11  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:29:27  damon]
 *
 * Revision 1.1.6.1  1994/06/09  13:59:53  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:35:08  annie]
 * 
 * Revision 1.1.4.3  1993/01/18  22:09:52  cjd
 * 	embedded copyright notice
 * 	[1993/01/18  21:50:45  cjd]
 * 
 * Revision 1.1.4.2  1992/08/31  19:05:37  jaffe
 * 	Transarc delta: jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	  Selected comments:
 * 	    This revision of this delta contains changes made to episode/async by the OSF
 * 	    between dce1.0 and dce1.0.1b9.  Subsequent revisions will contain merges
 * 	    for other episode subdirectories.
 * 	    Merged in changes made by the OSF between dce1.0 and dce1.0.1b9.
 * 	    This revision of this delta contains changes made to episode/dir by the OSF
 * 	    This revision of this delta contains changes made to episode/lobguf by the OSF
 * 	    This revision of this delta contains changes made to episode/salvage by the OSF
 * 	    This revision of this delta contains changes made to episode/sautils by the OSF
 * 	    This revision of this delta contains changes made to episode/tools by the OSF
 * 	    This revision of this delta contains changes made to episode/utils by the OSF
 * 	    This revision of this delta contains changes made to episode/vnops by the OSF
 * 	    between dce1.0 and dce1.0.1b9.
 * 	    This revision fixes some problems in the logbuf layer that occurred after
 * 	    the merge.
 * 	    This delta adds anode/epi_errs.msf.
 * 	    This delta fixes problems in the logbuf layer.
 * 	    This revision fixes some problems in the vnode layer.
 * 	    Fix more vnode layer problems.
 * 	    Add files that were introduced after DCE 1.0 to the .klib Makefiles.
 * 	    This delta fixes various problems that caused undefined references
 * 	    while building the AIX kernel extension.
 * 	    This revision fixes random problems encountered under OSF/1.
 * 	    This revision fixes some problems encountered when compiling test_vnodeops
 * 	    on OSF/1.
 * 	    This revision includes the OSF's changes to epimount.
 * 	Transarc delta: ota-ot3222-pin-core 1.1
 * 	  Selected comments:
 * 	    We need a way to simulate the behavior of system configurations with
 * 	    less memory.  The approach taken here is the write a system call that
 * 	    just allocates and pins a specified amount of memory.
 * 	    Define new opcode: EFS_DEBUG_ASYNC_PINCORE.
 * 	[1992/08/30  02:13:42  jaffe]
 * 
 * Revision 1.1.2.2  1992/05/05  20:05:20  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/04  21:09:24  jdp]
 * 
 * Revision 1.1  1992/01/19  02:38:59  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * Copyright (C) 1991 Transarc Corporation
 * All rights reserved.
 */

/*
 * these are the constants defined within the asevent_debug module.
 */

#define EFS_DEBUG_ASYNC_FREEZE 1
#define EFS_DEBUG_ASYNC_UNFREEZE 2
#define EFS_DEBUG_ASYNC_QUERY 3
#define EFS_DEBUG_ASYNC_TEST 4
#define EFS_DEBUG_ASYNC_PINCORE 5

EXPORT long asevent_efs_debug _TAKES((long, long, long));


