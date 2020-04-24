/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: proc3.3.c,v $
 * Revision 1.1.8.1  1996/10/17  18:16:45  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:49:24  damon]
 *
 * Revision 1.1.3.2  1993/01/11  17:37:55  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  16:54:31  htf]
 * 
 * Revision 1.1  1992/01/19  03:18:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>

main()
{
printf("Proc3.3 ready to call abort()\n");
abort();
}
