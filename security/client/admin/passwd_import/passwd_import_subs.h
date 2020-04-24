/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: passwd_import_subs.h,v $
 * Revision 1.1.6.2  1996/03/11  13:26:26  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:27  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:57:11  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:29:06  root]
 * 
 * Revision 1.1.4.2  1992/12/29  12:38:11  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:34:12  zeliff]
 * 
 * Revision 1.1.2.2  1992/04/23  14:15:09  burati
 * 	Initial version
 * 	[1992/04/23  01:31:39  burati]
 * 
 * $EndLog$
 */

/*  passwd_import_subs.h, greg, 07/27/89
**    declarations for import subroutines
**
** Copyright (c) Hewlett-Packard Company 1992
** Unpublished work. All Rights Reserved.
**
** Changes:
**  07/13/91    burati  Changes for NCS2.0 & DCE Security,
**  07/27/89    greg    ported to C from import_passwd.ins.pas source
*/ 

#ifndef import_passwd_subs_included
#define import_passwd_subs_included

#include <dce/nbase.h>
#include <dce/rgynbase.h>

typedef struct imp_tbl_ent_t {
    char        orig_name[sec_rgy_name_t_size];
    signed32    orig_num;
    char        new_name[sec_rgy_name_t_size];
    signed32    new_num;
} imp_tbl_ent_t;

/*
 * imp_get_err_text
 */
char *imp_get_err_text ( 
#ifdef __STDC__
    error_status_t  status
#endif
);


/*
 * imp_init - performs required initilaizations.
 */
void imp_init (
#ifdef __STDC__
void
#endif
);


/*
 * imp_add - adds an entry to the cache.
 */
void imp_add (
#ifdef __STDC__
   sec_rgy_domain_t domain,
   imp_tbl_ent_t    *entry,
   boolean32        *is_alias,
   error_status_t   *status
#endif
);


/*
 * imp_lookup_name - search for a matching orig_name
 *                  entry in the specified domain.
 */
void imp_lookup_name (
#ifdef __STDC__
    sec_rgy_domain_t    domain,
    char                *orig_name,
    imp_tbl_ent_t       *entry,
    error_status_t      *status
#endif
);


/*
 * imp_lookup_num - search for a matching orig_num
 *                  entry in the specified domain.
 */
void imp_lookup_num ( 
#ifdef __STDC__
    sec_rgy_domain_t    domain,
    signed32            orig_num,
    imp_tbl_ent_t       *entry,
    error_status_t      *status
#endif
);


/*
 * imp_get_next - get next sequential entry from specified domain.
 *                To get first entry, pass in NIL for imp_ptr
 */
void imp_get_next (
#ifdef __STDC__
    sec_rgy_domain_t    domain,
    void                **imp_ptr,
    imp_tbl_ent_t       *entry,
    error_status_t      *status
#endif
);


/*
 * imp_cleanup - release allocated memory
 */
void imp_cleanup (
#ifdef __STDC__
void
#endif
);

#endif

