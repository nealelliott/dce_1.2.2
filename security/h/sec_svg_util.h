/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_svg_util.h,v $
 * Revision 1.1.7.2  1996/02/18  23:01:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:52  marty]
 *
 * Revision 1.1.7.1  1995/12/08  17:29:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:30  root]
 * 
 * Revision 1.1.5.1  1994/06/17  18:42:14  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:52  devsrc]
 * 
 * Revision 1.1.3.2  1993/03/05  18:30:33  frisco
 * 	initial salvager functionality
 * 	[1993/03/02  21:14:59  frisco]
 * 
 * Revision 1.1.1.2  1992/11/25  21:10:27  frisco
 * 	Initial Revision
 * 
 * $EndLog$
 */

/*  
** Copyright (c) Hewlett-Packard Company 1992
** Unpublished work. All Rights Reserved.
*/

#ifdef MODULE_VERSION_ID
static char *VersionID =
    "$Id: sec_svg_util.h,v 1.1.7.2 1996/02/18 23:01:30 marty Exp $";
#endif

/*
 *      Registry Database Salvager Include file
 */

#ifndef sec_salvage_util__included
#define sec_salvage_util__included

#include <dce/rgynbase.h>
#include <rsdb_pvt.h>

#define BUFSIZE 1024

#define MAX_PERIOD_ELEMENT_LEN 100

#define SECS_IN_A_WEEK   604800
#define SECS_IN_A_DAY    86400
#define SECS_IN_AN_HOUR  3600
#define SECS_IN_A_MINUTE 60

#define WEEK_INDEX 0
#define DAY_INDEX 1
#define HOUR_INDEX 2
#define MIN_INDEX 3
#define SEC_INDEX 4
#define MAX_INDEX 5

/* rgy_edit/acl_edit borrowed types */
#define ek_lim_max_date_len       16  /* max length of an input date string */
#define ek_lim_max_date_time_len  16  /* max length of an output date/time
                                       * string
                                       */
#define ek_null_date             "0"  /* use this for assigning/checking-for 
                                       *  expiration dates that never take
                                       * effect
                                       */
typedef char   ek_date_buf_t[ek_lim_max_date_time_len + 1];
#define ek_live_forever 0       /* infinite lifespan     */


/* Table for parsing of entry_types */
typedef struct {
        char                    *name;
        sec_acl_entry_type_t    entry_type;
} sec_acl_entry_type_table_t;

void sec_to_dt_string (
#ifdef __STDC__
    sec_timeval_sec_t  secs,     /* IN - date-time  */
    char               *dt_str
#endif
);

error_status_t dt_to_sec (
#ifdef __STDC__
    char               *date,     /* IN - date string  */
    sec_timeval_sec_t  *clockh
#endif
);

void period_to_dt_string (
#ifdef __STDC__
    sec_timeval_period_t  secs,     /* IN - period  */
    char                  *dt_str
#endif
);

PRIVATE boolean32 IS_LIFESPAN_STRING (
#ifdef __STDC__
    char    *lifespan_str
#endif
);

error_status_t dt_to_period (
#ifdef __STDC__
    char                  *lifespan_str,
    sec_timeval_period_t  *lifespan_p 
#endif
);

PUBLIC void rsdb_acl_entry_to_str (
#ifdef __STDC__
    boolean32           cell_is_foreign,
    rsdb_acl_entry_t    acl_entry,
    char                *entry_string,
    sec_acl_printstring_t printstrings[],
    unsigned32          num_pstrings
#endif
);

PRIVATE void deif_permset_encode (
#ifdef __STDC__
    sec_acl_permset_t   perms,
    char                *string,
    sec_acl_printstring_t printstrings[],
    unsigned32          num_strings
#endif
);

PRIVATE void svg_util_parse_foreign_id (
#ifdef __STDC__
    unsigned char    *string,
    unsigned char    *string1,
    unsigned char    *string2
#endif
);

PRIVATE void make_lowercase (
#ifdef __STDC__
    char    *buffer
#endif
);

PRIVATE error_status_t deif_get_key (
#ifdef __STDC__
    rsdb_acl_entry_t     *acl_entry_p,
    sec_acl_entry_type_t entry_type,
    char                 *string
#endif
);

PUBLIC void rsdb_str_to_acl_entry (
#ifdef __STDC__
    char                *string,
    rsdb_acl_entry_t    *acl_entry,
    boolean32           cell_is_foreign,
    sec_acl_printstring_t printstrings[],
    unsigned32          num_pstrings,
    error_status_t      *status
#endif
);

#endif
