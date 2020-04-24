/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: getcellname.c,v $
 * Revision 1.1.7.2  1996/02/18  19:19:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:05:19  marty]
 *
 * Revision 1.1.7.1  1995/12/07  21:40:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  20:57:13  root]
 * 
 * Revision 1.1.5.2  1994/06/09  16:05:34  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:50:13  devsrc]
 * 
 * Revision 1.1.5.1  1993/12/29  16:23:46  rsalz
 * 	Rewrote in ANSI-C to call dce_cf_get_cell_name right.
 * 	[1993/12/29  14:28:17  rsalz]
 * 
 * Revision 1.1.2.2  1992/12/06  03:06:35  zeliff
 * 	Embedding copyrights
 * 	[1992/12/05  22:57:53  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:31:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
*/

#include <stdio.h>
#include <dce/dce.h>
#include <dce/dce_cf.h>

int
main(
    int			ac,
    char		*av[]
)
{
    char		*name;
    error_status_t	status;

    if (ac != 1) {
	fprintf(stderr, "Usage: %s\n", av[0]);
	exit(1);
    }

    dce_cf_get_cell_name(&name, &status);
    if (status != dce_cf_st_ok) {
	fprintf(stderr, "%s: cannot get cell name, 0x%8.8xl\n",
	    av[0], (long)status);
	return 1;
    }
    printf("%s\n", name);
    return 0;
}
