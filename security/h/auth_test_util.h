/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: auth_test_util.h,v $
 * Revision 1.1.4.2  1996/02/18  22:57:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:45  marty]
 *
 * Revision 1.1.4.1  1995/12/13  16:24:25  root
 * 	Submit
 * 	[1995/12/11  15:14:11  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:05:58  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:14:10  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:35  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  auth_test_util.h V=3 04/30/91 //littl/prgy/src/test/util
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
** 
*/

#ifndef _auth_test_util_h__included_
#define _auth_test_util_h__included_

extern unsigned_char_p_t  server_princ_name;  /* defaults to NULL */

extern  unsigned32  authn_level;  /* defaults to rpc_c_authn_level_connect */
extern  unsigned32  authn_svc;    /* defaults to rpc_c_authn_dce_private   */
extern  unsigned32  authz_svc;    /* defaults to rpc_c_authz_dce           */

/* type, tables, and routines for translating from authentication string to 
 *  constant values, and vice versa
 */

typedef struct _auth_table {
    char        *string;
    unsigned32  constant;
} auth_table_entry_t;
                
extern  auth_table_entry_t  authn_level_table[];
extern  auth_table_entry_t  authn_svc_table[];
extern  auth_table_entry_t  authz_svc_table[];

boolean32  auth_table_string_to_const ( 
#ifdef __STDC__
    auth_table_entry_t  table[],
    char                *string,
    unsigned32          *auth_const_p
#endif
);

boolean32  auth_table_const_to_string ( 
#ifdef __STDC__
    auth_table_entry_t  table[],
    unsigned32          auth_const,
    char                **string    /* address of a character pointer, not an array */
#endif
);

void auth_util_print_status ( 
#ifdef __STDC__
    char            *preamble,
    error_status_t  *stp
#endif
);

#endif  /* nested include guard */
