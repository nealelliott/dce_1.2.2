/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: efs_opcode.h,v $
 * Revision 1.1.63.1  1996/10/02  17:44:17  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:36:01  damon]
 *
 * Revision 1.1.58.2  1994/06/09  14:05:39  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:21:12  annie]
 * 
 * Revision 1.1.58.1  1994/02/04  20:16:57  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:12:14  devsrc]
 * 
 * Revision 1.1.56.1  1993/12/07  17:22:48  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:10:09  jaffe]
 * 
 * Revision 1.1.4.3  1993/01/21  18:27:12  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  16:50:23  zeliff]
 * 
 * Revision 1.1.4.2  1992/08/31  21:11:48  jaffe
 * 	Fix Header, Sync with Transarc
 * 	[1992/08/30  15:00:26  jaffe]
 * 
 * 	Transarc delta: jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	  Selected comments:
 * 	    This revision of this delta contains changes made to episode/async by the OSF
 * 	    between dce1.0 and dce1.0.1b9.  Subsequent revisions will contain merges
 * 	    for other episode subdirectories.
 * 	    This revision of this delta contains changes made to episode/dir by the OSF
 * 	    This revision of this delta contains changes made to episode/lobguf by the OSF
 * 	    This revision of this delta contains changes made to episode/salvage by the OSF
 * 	    This revision of this delta contains changes made to episode/sautils by the OSF
 * 	    This revision of this delta contains changes made to episode/tools by the OSF
 * 	    This revision of this delta contains changes made to episode/utils by the OSF
 * 	    This revision of this delta contains changes made to episode/vnops by the OSF
 * 	    between dce1.0 and dce1.0.1b9.
 * 	    Merged in changes made by the OSF between dce1.0 and dce1.0.1b9.
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
 * 	Transarc delta: jdp-ot4179-lfs-encapsulate-initialization 1.3
 * 	  Selected comments:
 * 	    This delta simplifies Episode user-space initialization.  There are two parts
 * 	    to this change.  The first simplifies the process of calling the *_Init
 * 	    routines.  Previously, you typically had to call dirb_Init(), N anode init
 * 	    routines, and logbuf_Init().  With this change, you need only specify the
 * 	    initialization parameters to the necessary layers in one structure and call
 * 	    the top-most init routine.  Since each initialization routine is now
 * 	    responsible for calling the lower layers' initialization routines, only
 * 	    one call is necessary.  The second change made in this delta was the
 * 	    encapsulation of the user-space device initialization code in one routine,
 * 	    us_InitDevice().  This routine is declared in tools/us.h and defined in
 * 	    tools/us_io.c.
 * 	    Added definition of struct efs_params.
 * 	    Fix a couple of remaining compilation problems.
 * 	    Fix compilation problems on OSF/1.
 * 	[1992/08/30  03:28:33  jaffe]
 * 
 * Revision 1.1.2.3  1992/06/01  19:21:17  jdp
 * 	Transarc delta: bwl-ot3216-non-aix-growaggr 1.2
 * 	  Files modified:
 * 	    anode: Makefile, growaggr.c
 * 	    vnops: Makefile, efs_growaggr.c, efs_init.c, efs_opcode.h
 * 	  Selected comments:
 * 	    Fix growaggr to use afs_syscall instead of AIX-only fscntl syscall.
 * 	    Add GROWAGGR opcode.
 * 	    Fix problems that showed up on OSF/1.
 * 	[1992/06/01  17:12:04  jdp]
 * 
 * Revision 1.1.2.2  1992/05/05  16:03:03  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/05  14:52:32  jdp]
 * 
 * Revision 1.1  1992/01/19  02:39:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* $Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/efs_opcode.h,v 1.1.63.1 1996/10/02 17:44:17 damon Exp $ */

/*
 * Copyright (C) 1990 Transarc Corporation
 * All rights reserved.
 */

#ifndef EFS_OPCODE_H
#define EFS_OPCODE_H

#include <dcedfs/dir_buffer.h>		/* struct dirb_params */

/*
 * Opcodes for the AFSCALL_EPISODE syscall
 */

#define EPIOP_INIT		0
#define EPIOP_STARTBIODAEMON	1
#define	EPIOP_STARTASYNCDAEMON	2
#define EPIOP_NEWAGGR		3
#define EPIOP_NEWVOL		4
#define EPIOP_STATS		5
#define EPIOP_DEBUG             6
#define EPIOP_GROWAGGR		7

/*
 * Structure used for argument of the EPIOP_INIT syscall
 */

struct episode_args {
    long buffer_pool_size;		/* size of buffer pool, in bytes */
    long anode_pool_size;		/* number of anodes */
    long dir_buffer_pool_size;		/* number of directory buffers */
    long vnode_pool_size;		/* number of Episode vnodes */
    long flock_pool_size;		/* number of file locks */
    long standalone;			/* true iff running without glue */
};

struct efs_params {
    long vnodePoolSize;
    long flockPoolSize;
    long standalone;
    long spare[5];
    struct dirb_params dirbParams;
};
#define EFS_DIRB_PARAMS(x) ((x).dirbParams)
#define EFS_EPIG_PARAMS(x) ((x).dirbParams.epigParams)
#define EFS_ELBB_PARAMS(x) ((x).dirbParams.epigParams.elbbParams)

/* Default values for initialization parameters 
 *
 * Does this make sense?  Perhaps initialization parameters should all be
 * derived from one big mega-parameter (such as "maxusers").  Until we
 * design something better, we have default values, and they are defined
 * in one place (here).
 *
 * N.B. Vnode table size should be larger than TKC table size (which is 256),
 *	to avoid running out of vnodes due to TKC holding them.
 */
#define EFS_DEFAULT_BUFFER_POOL_SIZE		384 * 1024
#define EFS_DEFAULT_VNODE_TABLE_SIZE		1024
#define	EFS_DEFAULT_ANODE_TABLE_SIZE		1024
#define EFS_DEFAULT_DIR_BUFFER_POOL_SIZE	15
#define EFS_DEFAULT_FLOCK_POOL_SIZE		40

#endif /* EFS_OPCODE_H */
