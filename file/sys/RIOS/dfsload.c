/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsload.c,v $
 * Revision 1.1.11.1  1996/10/02  21:00:51  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:47:53  damon]
 *
 * Revision 1.1.6.1  1994/06/09  14:20:09  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:31:47  annie]
 * 
 * Revision 1.1.4.2  1993/01/21  15:20:27  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  15:59:26  cjd]
 * 
 * Revision 1.1.2.2  1992/04/21  14:38:17  mason
 * 	Transarc delta: cburnett-ot2591-rios_afs_syscall_fails_for_nonroot 1.1
 * 	  Files modified:
 * 	    sys/RIOS: dfsload.c
 * 	  Selected comments:
 * 	    [ The code for determining DFS kernel extension presence did not
 * 	      work for nonroot callers because it was opening /dev/kmem.  This
 * 	      step was not necessary so it was removed.  It should be pointed
 * 	      out that this change will not take place globally until libdce.a
 * 	      is relinked.  This is because libdce.a picks up libafssys.a]
 * 	    [ removed /dev/kmem open]
 * 	[1992/04/20  23:03:20  mason]
 * 
 * Revision 1.1  1992/01/19  02:56:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <sys/types.h>
#include <sys/errno.h>
#include <sys/nlist.h>
/*
 * Functions to load DFS kernel extensions, and determine if they are loaded
 */

/*
 * See if kernel extension containing kfs_syscall is installed.
 * Return a 1 (TRUE) if system call is installed.
 */

int
afs_syscall_installed()
{
    char  loadpath[256];
	
    strcpy(loadpath, DCELOCAL_PATH);
    strcat(loadpath,"/ext/dfsloadobj");

    if (dfs_check_load("dfscore_kmid") && 
        (int) load(loadpath, 0, 0) )
	return (1);
    else
	return (0);
}

/*
 * Search for symbol too see if kernel extension is loaded
 */

int
dfs_check_load(sym)
	char *sym;
{
	struct nlist nl;

	nl.n_name = sym;
	if ((knlist(&nl, 1, sizeof(struct nlist)))) {
		return(0);
	}
	return(1);
}
