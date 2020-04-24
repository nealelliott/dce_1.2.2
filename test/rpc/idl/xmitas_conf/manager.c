/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: manager.c,v $
 * Revision 1.1.4.2  1996/02/17  23:18:37  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:37:24  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:32:02  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:54  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:58:20  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:47:25  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:33:27  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <xmitas_conf.h>

ndr_long_int _xmitas_sum(h, list)
handle_t h;
list_t *list;
{
    ndr_long_int sum = 0;

    while (list)
    {
        sum += list->value;
        list = list->next;
    }

    return sum;
}

globaldef xmitas_conf_v0_0_epv_t xmitas_conf_v0_0_m_epv = {_xmitas_sum};
