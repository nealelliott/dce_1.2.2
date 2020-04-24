/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: init_et.c,v $
 * Revision 1.1.4.2  1996/02/18  00:08:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:59:50  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:40:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:56:41  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:56:20  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:51:01  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:53:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  init_et.c V=2 10/23/91 //littl/prgy/krb5/comerr
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Header: /u0/rcs_trees/dce/rcs/security/krb5/comerr/init_et.c,v 1.1.4.2 1996/02/18 00:08:33 marty Exp $
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/comerr/init_et.c,v $
 * $Locker:  $
 *
 * Copyright 1986, 1987, 1988 by MIT Information Systems and
 *      the MIT Student Information Processing Board.
 *
 * For copyright info, see mit-sipb-cr.h.
 */

#include <stdio.h>
#include "error_table.h"

#ifndef __STDC__
#define const
#endif

#ifndef lint
static const char rcsid_init_et_c[] =
    "$Header: /u0/rcs_trees/dce/rcs/security/krb5/comerr/init_et.c,v 1.1.4.2 1996/02/18 00:08:33 marty Exp $";
#endif

extern char *malloc(), *realloc();

struct foobar {
    struct et_list etl;
    struct error_table et;
};

extern struct et_list * _et_list;

int init_error_table(msgs, base, count)
    const char * const * msgs;
    int base;
    int count;
{
    struct foobar * new_et;

    if (!base || !count || !msgs)
        return 0;

    new_et = (struct foobar *) malloc(sizeof(struct foobar));
    if (!new_et)
        return errno;   /* oops */
    new_et->etl.table = &new_et->et;
    new_et->et.msgs = msgs;
    new_et->et.base = base;
    new_et->et.n_msgs= count;

    new_et->etl.next = _et_list;
    _et_list = &new_et->etl;
    return 0;
}
