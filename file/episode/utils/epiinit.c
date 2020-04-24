/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: epiinit.c,v $
 * Revision 1.1.60.1  1996/10/02  17:42:26  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:35:04  damon]
 *
 * Revision 1.1.55.2  1994/06/09  14:04:29  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:39:12  annie]
 * 
 * Revision 1.1.55.1  1994/02/04  20:15:55  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:11:38  devsrc]
 * 
 * Revision 1.1.53.1  1993/12/07  17:21:51  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:02:52  jaffe]
 * 
 * Revision 1.1.4.4  1993/01/21  18:25:09  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  16:46:03  zeliff]
 * 
 * Revision 1.1.4.3  1992/11/24  16:58:04  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  17:52:13  bolinger]
 * 
 * Revision 1.1.4.2  1992/08/31  20:48:08  jaffe
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
 * 	    Merged in changes made by the OSF between dce1.0 and dce1.0.1b9.
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
 * 	    Added include of <dcedfs/dir_buffer.h> since efs_opcode.h requires it.
 * 	    Fix a couple of remaining compilation problems.
 * 	    Fix compilation problems on OSF/1.
 * 	[1992/08/30  03:01:05  jaffe]
 * 
 * Revision 1.1.2.2  1992/05/05  16:38:25  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/04  23:28:51  jdp]
 * 
 * Revision 1.1  1992/01/19  02:40:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <errno.h>
#include <dcedfs/param.h>
#include <dcedfs/syscall.h>
#include <dcedfs/episode/efs_opcode.h>


struct episode_args episode_args = {
    EFS_DEFAULT_BUFFER_POOL_SIZE,
    EFS_DEFAULT_ANODE_TABLE_SIZE,
    EFS_DEFAULT_DIR_BUFFER_POOL_SIZE,
    EFS_DEFAULT_VNODE_TABLE_SIZE,
    EFS_DEFAULT_FLOCK_POOL_SIZE,
    0					/* standalone */
};

main(argc, argv)
{
    int code = 0;

#if !defined(AFS_AIX_ENV) && !defined(AFS_SUNOS5_ENV)
    osi_setlocale(LC_ALL, "");
    code = getargs (argc, argv);
    if (code)
	exit (code);
    code = syscall(AFS_SYSCALL, AFSCALL_EPISODE, EPIOP_INIT, &episode_args);
    printf ("initializing Episode:  syscall return value = %d\n", code);
    if (code)
	printf ("errno = %d\n", errno);
#endif
    return code;
}

/*
 * getargs -- decipher the argument list
 *
 * Return codes:
 *  2 -- syntax error
 *  0 -- success
 */

getargs(argc, argv)
    int argc;
    char *argv[];
{
    int ac;				/* argument counter */
    int anyminus = 0;                   /* minus sign seen yet? */

    static char UsageArgs[] =
"[-single] [-remote] [-bcache <1K blocks>] [-vnodes <num vnodes>] \
[-anodes <num anodes>] [-dircache <buffers>] [-locks <num locks>]";

    for (ac = 1; ac < argc; ++ac) {
	if (anyminus && argv[ac][0] != '-' && ac < (argc - 1)) {
	    printf("Unidentified argument: ``%s''\nUsage: %s %s\n",
		   argv[ac], argv[0], UsageArgs);
	    return (2);
	}

	if (argv[ac][0] == '-') anyminus = 1;

	if (strcmp (argv[ac], "-bcache") == 0) {
	    ++ac;
	    episode_args.buffer_pool_size = atoi(argv[ac]) << 10;
	} else if (strcmp (argv[ac], "-dircache") == 0) {
	    ++ac;
	    episode_args.dir_buffer_pool_size = atoi(argv[ac]);
	} else if (strcmp (argv[ac], "-vnodes") == 0) {
	    ++ac;
	    episode_args.vnode_pool_size = atoi(argv[ac]);
	} else if (strcmp (argv[ac], "-locks") == 0) {
	    ++ac;
	    episode_args.flock_pool_size = atoi(argv[ac]);
	} else if (strcmp (argv[ac], "-anodes") == 0) {
	    ++ac;
	    episode_args.anode_pool_size = atoi(argv[ac]);
	} else if (strcmp (argv[ac], "-single") == 0) {
	    episode_args.standalone = 1;
	} else if (strcmp (argv[ac], "-remote") == 0) {
	    episode_args.standalone = 0;
	} else {
	    printf("Bad option: ``%s''. Usage: %s %s\n",
		   argv[ac], argv[0], UsageArgs);
	    return (2);
	}
    }
    return (0);
}
