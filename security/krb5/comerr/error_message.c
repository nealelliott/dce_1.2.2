/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: error_message.c,v $
 * Revision 1.1.6.2  1996/02/18  00:08:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:59:45  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:39:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:56:35  root]
 * 
 * Revision 1.1.4.1  1993/10/14  17:45:14  mccann
 * 	CR8651 64bit port
 * 	[1993/10/14  17:42:28  mccann]
 * 
 * Revision 1.1.2.2  1992/12/29  13:55:52  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:50:41  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:54:05  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  error_message.c V=3 11/26/91 //littl/prgy/krb5/comerr
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Header: /u0/rcs_trees/dce/rcs/security/krb5/comerr/error_message.c,v 1.1.6.2 1996/02/18 00:08:29 marty Exp $
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/comerr/error_message.c,v $
 * $Locker:  $
 *
 * Copyright 1987 by the Student Information Processing Board
 * of the Massachusetts Institute of Technology
 *
 * For copyright info, see "mit-sipb-cr.h".
 */

#include <stdio.h>
#include "error_table.h"
#include "internal.h"
#include <dce/dce_error.h>

static const char rcsid[] =
    "$Header: /u0/rcs_trees/dce/rcs/security/krb5/comerr/error_message.c,v 1.1.6.2 1996/02/18 00:08:29 marty Exp $";
static const char copyright[] =
    "Copyright 1986, 1987, 1988 by the Student Information Processing Board\nand the department of Information Systems\nof the Massachusetts Institute of Technology";

static char buffer[256];

struct et_list * _et_list = (struct et_list *) NULL;

const char * error_message (code)
  int code;
{
    int offset;
    struct et_list *et;
    int table_num;
    int started = 0;
    char *cp;
    int status = 0;
    
    offset = code & ((1<<ERRCODE_RANGE)-1);
    table_num = code - offset;
    if (!table_num) {
        if (offset < sys_nerr)
            return(sys_errlist[offset]);
        else
            goto oops;
    }
    for (et = _et_list; et; et = et->next) {
        if (et->table->base == table_num) {
            /* This is the right table */
            if (et->table->n_msgs <= offset)
                goto oops;
            return(et->table->msgs[offset]);
        }
    }
oops:
    /* non-registered message - try dce error codes */
    dce_error_inq_text (code, buffer, &status);
    if (status == 0)
        return buffer;

    strcpy (buffer, "Unknown code ");
    if (table_num) {
        strcat (buffer, error_table_name (table_num));
        strcat (buffer, " ");
    }
    for (cp = buffer; *cp; cp++)
        ;
    if (offset >= 100) {
        *cp++ = '0' + offset / 100;
        offset %= 100;
        started++;
    }
    if (started || offset >= 10) {
        *cp++ = '0' + offset / 10;
        offset %= 10;
    }
    *cp++ = '0' + offset;
    if (code > -10000) sprintf (cp, " (%d)", code);
    else *cp = '\0';
    return(buffer);
}
