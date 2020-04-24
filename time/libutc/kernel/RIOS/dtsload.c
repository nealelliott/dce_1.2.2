/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtsload.c,v $
 * Revision 1.1.7.2  1996/02/17  23:35:02  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:13  marty]
 *
 * Revision 1.1.7.1  1995/12/08  18:12:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:25:02  root]
 * 
 * Revision 1.1.5.2  1994/05/16  17:27:32  cbrooks
 * 	CR 10651
 * 	[1994/05/16  15:16:07  cbrooks]
 * 
 * Revision 1.1.5.1  1994/05/12  15:17:44  rhw
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:11:02  rhw]
 * 
 * Revision 1.1.2.3  1993/01/18  23:26:59  jwade
 * 	Change method of checking to see if the DTS kernel extension is loaded
 * 	into the machine.  Since non-root users cannot read /dev/kmem then use
 * 	the knlist library function to look for the extension.
 * 	[1993/01/18  19:17:08  jwade]
 * 
 * Revision 1.1.2.2  1992/12/30  16:36:33  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:18:56  htf]
 * 
 * Revision 1.1  1992/01/19  15:35:02  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <dce/dce.h>
#include <sys/types.h>
#include <sys/errno.h>
#include <sys/stat.h>
#include <nlist.h>
/*
 * Functions to load DFS kernel extensions, and determine if they are loaded
 */

#define LOADED 1
#define NOT_LOADED 0

int dts_syscall_installed(void);
int dts_check_load(void);


/*
 * See if kernel extension containing kfs_syscall is installed.
 * Return a 1 (TRUE) if system call is installed.
 */

int
dts_syscall_installed(void)
{
    char  loadpath[256];
	
    strcpy(loadpath, dcelocal_path);
    strcat(loadpath,"/ext/dtsloadobj");

    if (dts_check_load() && 
        (int) load(loadpath, 0, 0) )
	return (1);
    else
	return (0);
}

/*
 * Search for symbol too see if kernel extension is loaded
 */

int
dts_check_load(void)
{
	struct nlist nl;
	int numel = 1;

	nl.n_name = "kutc_gettime";
	nl.n_value = 0;

	knlist(&nl, numel, sizeof(struct nlist));
	if (nl.n_value != 0)
		return(LOADED);
	else
		return(NOT_LOADED);
}




