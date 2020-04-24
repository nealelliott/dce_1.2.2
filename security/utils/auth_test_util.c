/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: auth_test_util.c,v $
 * Revision 1.1.4.2  1996/02/18  00:22:28  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:08  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:01:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:22:51  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:40:24  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:31:43  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:41:39  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  auth_test_util.c V=3 04/30/91 //littl/prgy/src/test/util
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * 
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)auth_test_util.c	3 - 04/30/91";
#endif

#include <dce/nbase.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>

#include <stdio.h>
#include <un_strng.h>
#include <macros.h>
#include <auth_test_util.h>

PUBLIC unsigned_char_p_t   server_princ_name = NULL;
 
PUBLIC  unsigned32  authn_level  = rpc_c_authn_level_connect;
PUBLIC  unsigned32  authn_svc    = rpc_c_authn_dce_private;
PUBLIC  unsigned32  authz_svc    = rpc_c_authz_dce;
  
#define SENTINEL_CONST 0xfff

PUBLIC auth_table_entry_t authn_level_table[] = {
    { "default",    rpc_c_authn_level_default       },
    { "none",       rpc_c_authn_level_none          },
    { "connect",    rpc_c_authn_level_connect       },
    { "call",       rpc_c_authn_level_call          },
    { "packet",     rpc_c_authn_level_pkt           },
    { "integrity",  rpc_c_authn_level_pkt_integrity },
    { "privacy",    rpc_c_authn_level_pkt_privacy   },
    { NULL,         SENTINEL_CONST                  }
};
       

PUBLIC auth_table_entry_t  authn_svc_table[] = {
    { "none",     rpc_c_authn_none        },
    { "private",  rpc_c_authn_dce_private },
    { "public",   rpc_c_authn_dce_public  },
    { NULL,       SENTINEL_CONST          }
};
       

PUBLIC auth_table_entry_t  authz_svc_table[] = {
    { "none",  rpc_c_authz_none },
    { "name",  rpc_c_authz_name },
    { "dce",   rpc_c_authz_dce  },
    { NULL,    SENTINEL_CONST   }         
};
       
PUBLIC  boolean32  auth_table_string_to_const
#ifndef __STDC__
    ( table, string, auth_const_p ) 
    auth_table_entry_t  table[];
    char                *string;
    unsigned32          *auth_const_p;
#else
  (
    auth_table_entry_t  table[],
    char                *string,
    unsigned32          *auth_const_p
  )
#endif
{
    boolean32           found     = false;
    boolean32           exhausted = false; 
    int                 i;

    for (i = 0; !(found || exhausted); i++) {
       if (table[i].string == NULL) {
           *auth_const_p  = SENTINEL_CONST;
           exhausted = true;
       } else if (strcmp(table[i].string, string) == 0) {
           *auth_const_p = table[i].constant;
           found = true;
       }
    }

    return found;
}


PUBLIC  boolean32  auth_table_const_to_string
#ifndef __STDC__
    ( table, auth_const, string_p ) 
    auth_table_entry_t  table[];
    unsigned32          auth_const;
    char                **string_p;
#else
  (
    auth_table_entry_t  table[],
    unsigned32          auth_const,
    char                **string_p
  )
#endif
{
    boolean32           found     = false;
    boolean32           exhausted = false; 
    int                 i;

    for (i = 0; !(found || exhausted); i++) {
       if (table[i].constant == SENTINEL_CONST) {
           exhausted = true;
           *string_p = '\0';
       } else if (table[i].constant == auth_const) {
           found = true;
           *string_p = table[i].string;
       }
    }

    return found;
}
    

PUBLIC  void auth_util_print_status
#ifndef __STDC__
    ( preamble, stp ) 
    char            *preamble;
    error_status_t  *stp;      
#else
  (
    char            *preamble,
    error_status_t  *stp
  )
#endif
{
    dce_error_string_t  est;
    int                 lst;

    if (! (preamble == NULL || *preamble == '\0') ) {
        printf("%s: ", preamble);
    }     

    dce_error_inq_text(*stp, est, &lst);
    if (BAD_STATUS(&lst)) {
        strcpy((char *) est, "Unknown status");
    }

    printf("(%0x) %s\n", *stp, (char *) est);
}    

