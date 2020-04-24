/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: proc3.1.c,v $
 * Revision 1.1.8.1  1996/10/17  18:16:43  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:49:21  damon]
 *
 * Revision 1.1.2.3  1993/01/11  17:37:46  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  16:54:23  htf]
 * 
 * Revision 1.1.2.2  1992/11/24  21:44:14  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:43:59  bolinger]
 * 
 * Revision 1.1  1992/01/19  03:18:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *  test3.1.c -- A procedure that generates exceptions
 *
 *
 *	Copyright (C) 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/test/file/bos/tests/procs/proc3.1.c,v 1.1.8.1 1996/10/17 18:16:43 damon Exp $")

#include <stdio.h>

   /* Exception raised: Exceed available File Descriptors */

int 
main(argc, argv)
     int	argc;
     char *	argv[];
{
    char str[BUFSIZ];
    int i;
    FILE *fp, *fopen();

    for (i=0; i<70; i++) 
	fp = fopen("/tmp/test", "w");

    /* The file pointer should be NULL. Check how NULL
       pointer accesses are handled */

    fgets (str, BUFSIZ, fp);


}


