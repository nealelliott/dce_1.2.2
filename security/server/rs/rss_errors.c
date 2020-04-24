/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rss_errors.c,v $
 * Revision 1.1.6.2  1996/02/18  00:20:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:31  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:57:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:21:48  root]
 * 
 * Revision 1.1.4.1  1994/07/15  15:02:24  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:22:18  mdf]
 * 
 * Revision 1.1.2.2  1992/12/29  16:37:51  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:27:17  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:43:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rss_errors.c V=12 08/22/91 //littl/prgy/src/server/rs
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server Surrogate - error handling
 * 
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rss_errors.c	12 - 08/22/91";
#endif

#include <stdio.h>

#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/dce_error.h>
#include <dce/rplbase.h>

#include <un_io.h>

#include <rss_errors.h>
#include <rs_base.h>


PUBLIC void rs_util_fatal
#ifndef __STDC__
    ( string ) 
    char *string;
#else
  (
    char *string
  )
#endif
{
    error_status_t  status;

    fprintf(stderr, "Registry: Fatal Error - %s\n", string);
    exit(3);
}

PUBLIC void rs_util_fatal_st
#ifndef __STDC__
    ( status, string ) 
    error_status_t  status;
    char            *string;
#else
  (
    error_status_t  status,
    char            *string
  )
#endif
{

    if (string && *string) {
        fprintf(stderr, "Registry: Fatal Error - %s - ", string);
    } else {
        fprintf(stderr, "Registry: Fatal Error");
    }
    print_st(status);

    exit(3);
}

void error
#ifndef __STDC__
    (string)
    char *string;
#else
  (
    char *string
  )
#endif
{
    fprintf(stderr, "Registry: Error - %s\n", string);
}

PUBLIC void error_st
#ifndef __STDC__
    ( status, string ) 
    error_status_t  status;
    char            *string;
#else
  (
    error_status_t  status,
    char            *string
  )
#endif
{
    if (string && *string) {
        fprintf(stderr, "Registry: Error - %s - ", string);
    } else {
        fprintf(stderr, "Registry: Error");
    }
    print_st(status);
}

void warning
#ifndef __STDC__
    ( string ) 
    char *string;
#else
  (
    char *string
  )
#endif
{
    fprintf(stderr, "Registry: Warning - %s\n", string);
}

PUBLIC void warning_st
#ifndef __STDC__
    ( status, string ) 
    error_status_t  status;
    char            *string;
#else
  (
    error_status_t  status,
    char            *string
  )
#endif
{
    if (string && *string) {
        fprintf(stderr, "Registry: Warning - %s - ", string);
    } else {
        fprintf(stderr, "Registry: Warning");
    }
    print_st(status);
}

PUBLIC void print_st
#ifndef __STDC__
    ( status ) 
    error_status_t  status;
#else
  (
    error_status_t  status
  )
#endif
{
    unsigned char error_text[dce_c_error_string_len];
    char message[dce_c_error_string_len+20];

    dce_error_inq_text(status, error_text, 0);

    sprintf(message, "0x%08x - %s", STATUS(&status), error_text);
    fprintf(stderr, " - %s\n", message);
}

PUBLIC void print_rgy_st
#ifndef __STDC__
    ( status ) 
    error_status_t  status;
#else
  (
    error_status_t  status
  )
#endif
{
    unsigned char error_text[dce_c_error_string_len];
    char message[dce_c_error_string_len+20];

    dce_error_inq_text(status, error_text, 0);

    if (error_text[0] != '\0') {
        sprintf(message, "0x%08x - %s", STATUS(&status), error_text);
        fprintf(stderr, " - %s\n", message);
    }
}

