/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: et_name.c,v $
 * Revision 1.1.4.2  1996/02/18  00:08:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:59:47  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:39:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:56:40  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:56:11  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:50:57  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:53:57  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  et_name.c V=3 10/29/91 //littl/prgy/krb5/comerr
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * Copyright 1987 by MIT Student Information Processing Board
 *
 * For copyright info, see mit-sipb-cr.h.
 */

#include <error_table.h>
#include <internal.h>

#ifndef lint
static const char copyright[] =
    "Copyright 1987,1988 by Student Information Processing Board, Massachusetts Institute of Technology";
static const char rcsid_et_name_c[] =
    "$Header: /u0/rcs_trees/dce/rcs/security/krb5/comerr/et_name.c,v 1.1.4.2 1996/02/18 00:08:31 marty Exp $";
#endif

static const char char_set[] =
        "abcdefghijklmnopqrstuvwxyz_0123456789";

static char buf[20];

const char * error_table_name(num)
    int num;
{
    int ch;
    int i;
    char *p;

    /* num = aa aaa abb bbb bcc ccc cdd ddd d?? ??? ??? */
    p = buf;

    /* generate the basic package name */
    ch = (num>>28) & 3;
    if (ch == 0) {
        /* return null string here for reserved error codes */
        *p = 0;
        return (buf);
    }
    else if (ch == 1) {
        strcpy(p, "DCE:");
        p += 4;
    }
    else if (ch == 2) {
        strcpy(p, "DFS:");
        p += 4;
    }

    /* now decode the package name */
    num >>= 12;
    /* num = ?? ??? ??? aaa aaa bbb bbb ccc ccc ddd ddd */
    num &= 0xffff;
    /* num = 00 000 000 aaa aaa bbb bbb ccc ccc ddd ddd */
    if (num >= 40*40) {
        ch = num / (40*40);
        *p++ = char_set[ch];
        num -= ch  * 40 * 40;
    }
    if (num >= 40) {
        ch = num / 40;
        *p++ = char_set[ch];
        num -= ch * 40;
    }
    if (num >= 1) {
        ch = num;
        *p++ = char_set[ch];
    }

    /* finally, null terminate the result */
    *p = '\0';
    return(buf);
}
