/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rutcops.c,v $
 * Revision 1.1.2.2  1996/02/18  19:16:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:43  marty]
 *
 * Revision 1.1.2.1  1995/12/08  20:54:06  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/07  17:13 UTC  mk
 * 	Merge in initial version mk_mothra_dced/2.
 * 
 * 	HP revision /main/mk_mothra_dced/2  1994/12/07  17:10 UTC  mk
 * 	Add history and log keywords.
 * 	[1995/12/08  20:49:19  root]
 * 
 * $EndLog$
 */

/*
**  Copyright (c) 1994 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**
**  Operations for lightweight remote UTC interface.
*/

#include <dce/utc.h>

/*
**  Manager routine for rutc_gettime.
*/
long int
rutc_gettime(
    handle_t            h,
    utc_t               *utc
)
{
    return (utc_gettime(utc));
}
