/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: deif_err.c,v $
 * Revision 1.1.6.2  1996/03/11  13:26:10  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:14  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:55:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:28:37  root]
 * 
 * Revision 1.1.4.2  1992/12/29  12:37:04  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:32:56  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/08  15:27:30  burati
 * 	Add new functionality to support POSIX mask calculation
 * 	Moved error displaying rtns here.
 * 	[1992/05/07  14:58:24  burati]
 * 
 * Revision 1.1  1992/01/19  14:45:16  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  deif_err.c V=10 11/21/91 //littl/prgy/src/admin/dacl_edit
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      sec_acl editor error interface routines
** 
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)deif_err.c	10 - 11/21/91";
#endif

#include <stdio.h>
#include <macros.h>

#include <dce/nbase.h>
#include <dce/dce_error.h>

PRIVATE char *deif_get_error_text
#ifndef __STDC__
    (st)
    error_status_t st;
#else
  (
    error_status_t st
  )
#endif
{
    int                         tmp_stat;
    static dce_error_string_t   buf;

    dce_error_inq_text(st, buf, &tmp_stat);
    if (!GOOD_STATUS(&tmp_stat))
        sprintf((char *) buf, "SEC_ACL_EDIT status %x", st);
    return (char *) buf;
}

/*
 * d e i f _ s t a t u s _ p r i n t
 */
PUBLIC void deif_status_print
#ifndef __STDC__
    (st_p, use_gui)
    error_status_t      *st_p;
    boolean32           use_gui;
#else
  (
    error_status_t      *st_p,
    boolean32           use_gui
  )
#endif
{
    /* Just print the error string for now, worry about GUI later */
    fprintf(stderr, "ERROR: %s\n", deif_get_error_text(*st_p));
}


/*
 * d e i f _ m s g _ p r i n t
 */
PUBLIC void deif_msg_print
#ifndef __STDC__
    (string, use_gui)
    char                *string;
    boolean32           use_gui;
#else
  (
    char                *string,
    boolean32           use_gui
  )
#endif
{
    /* Just print the string for now, worry about GUI later */
    fprintf(stderr, "%s", string);
}


/*
 * d e i f _ m s g _ s t a t u s _ p r i n t
 */
PUBLIC void deif_msg_status_print
#ifndef __STDC__
    (string, st_p, use_gui)
    char                *string;
    error_status_t      *st_p;
    boolean32           use_gui;
#else
  (
    char                *string,
    error_status_t      *st_p,
    boolean32           use_gui
  )
#endif
{
    /* Just print the string for now, worry about GUI later */
    fprintf(stderr, "%s - ", string);
    fprintf(stderr, "%s\n", deif_get_error_text(*st_p));
}
