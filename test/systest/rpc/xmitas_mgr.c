/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xmitas_mgr.c,v $
 * Revision 1.1.10.2  1996/03/09  20:51:51  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:29:34  marty]
 *
 * Revision 1.1.10.1  1995/12/13  21:56:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:35  root]
 * 
 * Revision 1.1.6.2  1993/07/09  13:59:28  root
 * 	Initial King Kong branch
 * 	[1993/07/09  13:58:56  root]
 * 
 * Revision 1.1.4.2  1993/03/31  12:44:46  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.2  1992/06/11  20:01:30  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:08:14  mcfarland]
 * 
 * Revision 1.1.1.2  1992/05/28  15:15:41  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:36:13  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <xmitas.h>

ndr_long_int xmitas$sum(h, list)
handle_t h;
list_t list;
{
    ndr_long_int sum = list.value;    /* every list has at least one element */

    while (list.next)
    {
        list = *(list.next);
        sum += list.value;
    }

    return sum;
}

globaldef xmitas_v0_0_epv_t xmitas_v0_0_m_epv = {xmitas$sum};
