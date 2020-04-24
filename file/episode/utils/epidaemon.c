/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: epidaemon.c,v $
 * Revision 1.1.69.1  1996/10/02  17:42:23  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:35:03  damon]
 *
 * Revision 1.1.64.2  1994/06/09  14:04:28  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:39:11  annie]
 * 
 * Revision 1.1.64.1  1994/02/04  20:15:52  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:11:37  devsrc]
 * 
 * Revision 1.1.62.1  1993/12/07  17:21:48  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  15:02:34  jaffe]
 * 
 * Revision 1.1.4.4  1993/01/21  18:25:07  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  16:45:59  zeliff]
 * 
 * Revision 1.1.4.3  1992/11/24  16:58:01  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  17:52:07  bolinger]
 * 
 * Revision 1.1.4.2  1992/08/31  20:47:38  jaffe
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
 * 	[1992/08/30  03:00:26  jaffe]
 * 
 * Revision 1.1.2.2  1992/05/05  16:38:16  jdp
 * 	delta bwl-killable-daemons-2200 1.1
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/04  23:27:58  jdp]
 * 
 * Revision 1.1  1992/01/19  02:40:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <dcedfs/param.h>
#include <dcedfs/syscall.h>
#include <dcedfs/dir_buffer.h>
#include <dcedfs/episode/efs_opcode.h>

#define defaultNumDaemons 5

main(argc, argv)
    int argc;
    char *argv[];
{
    long numDaemons, i, code;
    osi_setlocale(LC_ALL, "");
    if (argc == 1) {
def:
	numDaemons = defaultNumDaemons;
	goto startem;
    }
    if (argc != 2) goto error;
    numDaemons = atoi (argv[1]);
    if (numDaemons < 0) goto error;
    if (numDaemons == 0) goto def;

startem:
#if !sun
    for (i=0; i<numDaemons; i++) {
	code = fork();
	if (code == 0) {
	    /* Child */
	    /* start a new process group to avoid signals */
	    (void) setpgrp(0,0);
	    argv[0] = "estratd";
	    afs_syscall (AFSCALL_EPISODE, EPIOP_STARTBIODAEMON,
			(int)numDaemons, 0, 0, 0);
	    exit(1);
	}
    }
#endif

    code = fork();
    if (code == 0) {
	/* Child */
	/* start a new process group to avoid signals */
	(void) setpgrp(0,0);
	argv[0] = "easyncd";
	afs_syscall (AFSCALL_EPISODE, EPIOP_STARTASYNCDAEMON, 0, 0, 0, 0);
	exit(1);
    }

    exit(0);

error:
    printf ("Too many arguments or negative argument.\n");
}
