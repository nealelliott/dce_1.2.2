/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: pgo.c,v $
 * Revision 1.1.65.1  1996/10/03  14:46:39  arvind
 * 	shut up purify on error return path.
 * 	[1996/09/16  20:30 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.63.2  1996/03/11  13:28:08  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:54  marty]
 * 
 * Revision 1.1.63.1  1995/12/08  18:07:01  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/10/30  15:22 UTC  mullan_s
 * 	HPDCE02 branch for pgo.c
 * 
 * 	HP revision /main/mullan_chfts15792/1  1995/10/30  14:45 UTC  mullan_s
 * 	rgy_pgo_get: check result->status for binding retries.
 * 	[1995/12/08  16:32:15  root]
 * 
 * Revision 1.1.59.2  1994/05/06  20:18:31  mdf
 * 	Merged with changes from 1.1.59.1
 * 	[1994/05/06  20:18:18  mdf]
 * 
 * 	fix for OT9575: check to see if pgo name is legal.
 * 	[1994/05/05  11:55:42  mdf]
 * 
 * Revision 1.1.59.1  1994/05/05  20:44:32  hasbrouc
 * 	add group override support (RFC 55)
 * 	[1994/05/05  20:16:35  hasbrouc]
 * 
 * Revision 1.1.56.1  1993/10/05  22:30:07  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:05:17  mccann]
 * 
 * Revision 1.1.4.3  1993/02/05  22:44:36  sommerfeld
 * 	[OT5061] Deal with change to CHECK_CACHE macro.
 * 	[1993/02/04  22:31:34  sommerfeld]
 * 
 * Revision 1.1.4.2  1992/12/29  13:03:08  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:40:45  zeliff]
 * 
 * Revision 1.1.2.2  1992/06/29  21:07:52  burati
 * 	CR4491 Don't use NULL as end of string char.
 * 	[1992/06/29  18:08:08  burati]
 * 
 * Revision 1.1  1992/01/19  14:47:03  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  pgo.c V=16 05/03/91 //littl/prgy/src/client/rca
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      Registry Client Agent Interface - Naming management
**
** Copyright Hewlett-Packard Company 1989, 1990
** Copyright Apollo Computer Inc. 1987, 1988
** 
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)pgo.c	16 - 05/03/91";
#endif

#include <rca_pvt.h>  /* must be first cause it includes pbase.h */
#include <rgymacro.h>

#include <pgo.h>
#include <rs_pgo.h>

#include <sysdep.h>

#include <un_strng.h>

#define Crs_pgo_add(CH,b,c,d,CI,e)\
        (*rs_pgo_v1_0_c_epv.rs_pgo_add) (HANDLE(CH),b,c,d,CI,e)

#define Crs_pgo_delete(CH,b,c,CI,d)\
        (*rs_pgo_v1_0_c_epv.rs_pgo_delete) (HANDLE(CH),b,c,CI,d)

#define Crs_pgo_replace(CH,b,c,d,CI,e)\
        (*rs_pgo_v1_0_c_epv.rs_pgo_replace) (HANDLE(CH),b,c,d,CI,e)

#define Crs_pgo_rename(CH,b,c,d,CI,e)\
        (*rs_pgo_v1_0_c_epv.rs_pgo_rename) (HANDLE(CH),b,c,d,CI,e)

#define Crs_pgo_get(CH,b,c,d,e,CI,f,STATUS)\
        (*rs_pgo_v1_0_c_epv.rs_pgo_get) (HANDLE(CH),b,c,d,e,CI,f,STATUS)

#define Crs_pgo_key_transfer(CH,b,c,d,CI,e)\
        (*rs_pgo_v1_0_c_epv.rs_pgo_key_transfer) (HANDLE(CH),b,c,d,CI,e)

#define Crs_pgo_add_member(CH,b,c,d,CI,e)\
        (*rs_pgo_v1_0_c_epv.rs_pgo_add_member) (HANDLE(CH),b,c,d,CI,e)

#define Crs_pgo_delete_member(CH,b,c,d,CI,e)\
        (*rs_pgo_v1_0_c_epv.rs_pgo_delete_member) (HANDLE(CH),b,c,d,CI,e)

#define Crs_pgo_is_member(CH,b,c,d,CI,e)\
        (*rs_pgo_v1_0_c_epv.rs_pgo_is_member) (HANDLE(CH),b,c,d,CI,e)

#define Crs_pgo_get_members(CH,b,c,d,e,f,g,h,CI,i)\
        (*rs_pgo_v1_0_c_epv.rs_pgo_get_members) (HANDLE(CH),b,c,d,e,f,g,h,CI,i)

#ifdef GLOBAL_LIBRARY
#   include <set_sect_pvt.c>
#endif


PUBLIC void sec_rgy_pgo_add
#ifndef __STDC__
    ( context, name_domain, name, pgo_item, status ) 
    sec_rgy_handle_t    context;        /* [in] */
    sec_rgy_domain_t    name_domain;    /* [in] */
    sec_rgy_name_t      name;           /* [in] */
    sec_rgy_pgo_item_t  *pgo_item;      /* [in] */
    error_status_t      *status;        /* [out] */
#else
  (
    sec_rgy_handle_t    context,        /* [in] */
    sec_rgy_domain_t    name_domain,    /* [in] */
    sec_rgy_name_t      name,           /* [in] */
    sec_rgy_pgo_item_t  *pgo_item,      /* [in] */
    error_status_t      *status
  )
#endif
{
    rs_cache_data_t        cache_info;

    if (!LEGAL_PGO_NAME(name) || !CHECK_PGO_NAME(name) ||
	!CHECK_PGO_FULLNAME(pgo_item->fullname)) {
        SET_STATUS(status, sec_rgy_bad_data);
        return;
    }

    SETUP_RETRY(rca_op_write, status) {
        Crs_pgo_add(context, name_domain, name, pgo_item, &cache_info, status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_pgo_delete
#ifndef __STDC__
    ( context, name_domain, name, status ) 
    sec_rgy_handle_t    context;        /* [in] */
    sec_rgy_domain_t    name_domain;    /* [in] */
    sec_rgy_name_t      name;           /* [in] */
    error_status_t      *status;        /* [out] */
#else
  (
    sec_rgy_handle_t    context,        /* [in] */
    sec_rgy_domain_t    name_domain,    /* [in] */
    sec_rgy_name_t      name,           /* [in] */
    error_status_t      *status
  )
#endif
{
    rs_cache_data_t        cache_info;

    if (!CHECK_PGO_NAME(name)) {
        SET_STATUS(status, sec_rgy_bad_data);
        return;
    }

    SETUP_RETRY(rca_op_write, status) {
        Crs_pgo_delete(context, name_domain, name, &cache_info, status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_pgo_replace
#ifndef __STDC__
    ( context, name_domain, pgo_name, item, status ) 
    sec_rgy_handle_t    context;        /* [in] */
    sec_rgy_domain_t    name_domain;    /* [in] */
    sec_rgy_name_t      pgo_name;       /* [in] */
    sec_rgy_pgo_item_t  *item;          /* [in] */
    error_status_t      *status;        /* [out] */
#else
  (
    sec_rgy_handle_t    context,        /* [in] */
    sec_rgy_domain_t    name_domain,    /* [in] */
    sec_rgy_name_t      pgo_name,       /* [in] */
    sec_rgy_pgo_item_t  *item,          /* [in] */
    error_status_t      *status
  )
#endif
{
    rs_cache_data_t        cache_info;

    if (!CHECK_PGO_NAME(pgo_name) ||
	!CHECK_PGO_FULLNAME(item->fullname)) {
        SET_STATUS(status, sec_rgy_bad_data);
        return;
    }

    SETUP_RETRY(rca_op_write, status) {
        Crs_pgo_replace(context, name_domain, pgo_name, item, &cache_info,
                        status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_pgo_rename
#ifndef __STDC__
    ( context, name_domain, old_name, new_name, status ) 
    sec_rgy_handle_t    context;        /* [in] */
    sec_rgy_domain_t    name_domain;    /* [in] */
    sec_rgy_name_t      old_name;       /* [in] */
    sec_rgy_name_t      new_name;       /* [in] */
    error_status_t      *status;        /* [out] */
#else
  (
    sec_rgy_handle_t    context,        /* [in] */
    sec_rgy_domain_t    name_domain,    /* [in] */
    sec_rgy_name_t      old_name,       /* [in] */
    sec_rgy_name_t      new_name,       /* [in] */
    error_status_t      *status
  )
#endif
{
    rs_cache_data_t        cache_info;

    if (!CHECK_PGO_NAME(old_name) || !CHECK_PGO_NAME(new_name)) {
        SET_STATUS(status, sec_rgy_bad_data);
        return;
    }

    SETUP_RETRY(rca_op_write, status) {
        Crs_pgo_rename(context, name_domain, old_name, new_name, &cache_info,
                        status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

/*
** Temporary macros to access variant part of query unions.
** Someday NIDL will support a better mechanism for specifying
** unions.
*/
#define QUERY_NAME(Q) (Q).tagged_union.name
#define QUERY_ID(Q) (Q).tagged_union.id_key
#define QUERY_UNIX_NUM(Q) (Q).tagged_union.unix_num_key
#define QUERY_NEXT_SCOPE(Q) (Q).tagged_union.scope
#define RESULT_ITEM(R) (R).tagged_union.result.item
#define RESULT_NAME(R) (R).tagged_union.result.name

PRIVATE void rgy_pgo_get
#ifndef __STDC__
    ( context, domain, key, allow_aliases, cursor, result, status )
    sec_rgy_handle_t        context;        /* [in] */
    sec_rgy_domain_t        domain;         /* [in] */
    rs_pgo_query_key_t      *key;           /* [in] */
    boolean32               allow_aliases;  /* [in] */
    sec_rgy_cursor_t        *cursor;        /* [in, out] */
    rs_pgo_query_result_t   *result;        /* [out] */
    error_status_t          *status;        /* [out] */
#else
  (
    sec_rgy_handle_t        context,        /* [in] */
    sec_rgy_domain_t        domain,         /* [in] */
    rs_pgo_query_key_t      *key,           /* [in] */
    boolean32               allow_aliases,  /* [in] */
    sec_rgy_cursor_t        *cursor,        /* [in, out] */
    rs_pgo_query_result_t   *result,        /* [out] */
    error_status_t          *status
  )
#endif
{
    rs_cache_data_t        cache_info;

    SETUP_RETRY(rca_op_read, status) {
        Crs_pgo_get(context, domain, key, allow_aliases, cursor, 
                    &cache_info, result, status);
        /* 
         * If status is good (which means there were no exceptions,
         * including RPC communication failures), check if 
         * result->status is an error status that we should try to 
         * rebind on, such as secd being in a bad state 
         * (sec_rgy_rep_bad_state).
         */
	if (*status == error_status_ok)
	    SET_STATUS(status, result->status);
        NORMAL_CASE {
            /*
             * No communication failure, so capture the operation status
             */
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}


PUBLIC void sec_rgy_pgo_get_by_name
#ifndef __STDC__
    ( context, name_domain, pgo_name, item_cursor, pgo_item, status )
    sec_rgy_handle_t    context;        /* [in] */
    sec_rgy_domain_t    name_domain;    /* [in] */
    sec_rgy_name_t      pgo_name;       /* [in] */
    sec_rgy_cursor_t    *item_cursor;   /* [in, out] */
    sec_rgy_pgo_item_t  *pgo_item;      /* [out] */
    error_status_t      *status;        /* [out] */
#else
  (
    sec_rgy_handle_t    context,        /* [in] */
    sec_rgy_domain_t    name_domain,    /* [in] */
    sec_rgy_name_t      pgo_name,       /* [in] */
    sec_rgy_cursor_t    *item_cursor,   /* [in, out] */
    sec_rgy_pgo_item_t  *pgo_item,      /* [out] */
    error_status_t      *status
  )
#endif
{
    rs_pgo_query_key_t      key;
    rs_pgo_query_result_t   result;

    if (!CHECK_PGO_NAME(pgo_name)) {
        SET_STATUS(status, sec_rgy_bad_data);
	return;
    }

    key.query = rs_pgo_query_name;
    strcpy((char *) QUERY_NAME(key), (char *) pgo_name);

    rgy_pgo_get(context, name_domain, &key, false, item_cursor, &result,
		status);

    if (STATUS_OK(status)) {
        *pgo_item = RESULT_ITEM(result);
    }
}

PUBLIC void sec_rgy_pgo_get_by_id
#ifndef __STDC__
    ( context, name_domain, scope, item_id,  
		  allow_aliases, item_cursor, pgo_item, name, status )
    sec_rgy_handle_t    context;        /* [in] */
    sec_rgy_domain_t    name_domain;    /* [in] */
    sec_rgy_name_t      scope;          /* [in] */   
    uuid_t              *(item_id);     /* [in] */
    boolean32           allow_aliases;  /* [in] */
    sec_rgy_cursor_t    *(item_cursor); /* [in, out] */
    sec_rgy_pgo_item_t  *(pgo_item);    /* [out] */
    sec_rgy_name_t      name;           /* [out] */
    error_status_t      *(status);      /* [out] */
#else
  (
    sec_rgy_handle_t    context,        /* [in] */
    sec_rgy_domain_t    name_domain,    /* [in] */
    sec_rgy_name_t      scope,          /* [in] */   
    uuid_t              *(item_id),     /* [in] */
    boolean32           allow_aliases,  /* [in] */
    sec_rgy_cursor_t    *(item_cursor), /* [in, out] */
    sec_rgy_pgo_item_t  *(pgo_item),    /* [out] */
    sec_rgy_name_t      name,           /* [out] */
    error_status_t      *(status)
  )
#endif
{
    rs_pgo_query_key_t     key;
    rs_pgo_query_result_t  result;

    key.query = rs_pgo_query_id;
    QUERY_ID(key).id = *item_id;
    strcpy( (char *)QUERY_ID(key).scope, (char *) scope);

    rgy_pgo_get(context, name_domain, &key, allow_aliases, item_cursor,
		&result, status);

    if (STATUS_OK(status)) {
        *pgo_item = RESULT_ITEM(result); 
	strcpy( (char *) name, (char *) RESULT_NAME(result));
    }
}

PUBLIC void sec_rgy_pgo_get_by_unix_num
#ifndef __STDC__
    ( context, name_domain, scope, unix_id, 
				        allow_aliases, item_cursor, pgo_item,
				        name, status )
    sec_rgy_handle_t    context;         /* [in] */   
    sec_rgy_domain_t    name_domain;     /* [in] */
    sec_rgy_name_t      scope;           /* [in] */
    signed32            unix_id;         /* [in] */
    boolean32           allow_aliases;   /* [in] */
    sec_rgy_cursor_t    *(item_cursor);  /* [in, out] */
    sec_rgy_pgo_item_t  *(pgo_item);     /* [out] */
    sec_rgy_name_t      name;            /* [out] */
    error_status_t      *(status);       /* [out] */
#else
  (
    sec_rgy_handle_t    context,         /* [in] */   
    sec_rgy_domain_t    name_domain,     /* [in] */
    sec_rgy_name_t      scope,           /* [in] */
    signed32            unix_id,         /* [in] */
    boolean32           allow_aliases,   /* [in] */
    sec_rgy_cursor_t    *(item_cursor),  /* [in, out] */
    sec_rgy_pgo_item_t  *(pgo_item),     /* [out] */
    sec_rgy_name_t      name,            /* [out] */
    error_status_t      *(status)
  )
#endif
{
    rs_pgo_query_key_t     key;
    rs_pgo_query_result_t  result;

    key.query = rs_pgo_query_unix_num;
    QUERY_UNIX_NUM(key).unix_num = unix_id;
    strcpy( (char *) QUERY_UNIX_NUM(key).scope, (char *) scope);

    rgy_pgo_get(context, name_domain, &key, allow_aliases, item_cursor,
		&result, status);
    
    if (STATUS_OK(status)) {
        *pgo_item = RESULT_ITEM(result);
	strcpy( (char *) name, (char *) RESULT_NAME(result));
    }
}


PUBLIC void sec_rgy_pgo_get_by_eff_unix_num
#ifndef __STDC__
    ( context, name_domain, scope, unix_id, allow_aliases, 
     item_cursor, pgo_item, name, overriden, status )
    sec_rgy_handle_t    context;         /* [in] */   
    sec_rgy_domain_t    name_domain;     /* [in] */
    sec_rgy_name_t      scope;           /* [in] */
    signed32            unix_id;         /* [in] */
    boolean32           allow_aliases;   /* [in] */
    sec_rgy_cursor_t    *(item_cursor);  /* [in, out] */
    sec_rgy_pgo_item_t  *(pgo_item);     /* [out] */
    sec_rgy_name_t      name;            /* [out] */
    boolean32           *overriden;      /* [out] */
    error_status_t      *(status);       /* [out] */
#else
  (
    sec_rgy_handle_t    context,         /* [in] */   
    sec_rgy_domain_t    name_domain,     /* [in] */
    sec_rgy_name_t      scope,           /* [in] */
    signed32            unix_id,         /* [in] */
    boolean32           allow_aliases,   /* [in] */
    sec_rgy_cursor_t    *(item_cursor),  /* [in, out] */
    sec_rgy_pgo_item_t  *(pgo_item),     /* [out] */
    sec_rgy_name_t      name,            /* [out] */
    boolean32           *overriden,      /* [out] */
    error_status_t      *(status)
  )
#endif
{
    if (override_get_by_unix_num (name_domain, unix_id, name, status) ){
	*overriden = true;
	sec_rgy_pgo_get_by_name (context, name_domain, name, item_cursor, pgo_item, status);
    } else {
	*overriden = false;
	sec_rgy_pgo_get_by_unix_num (context, name_domain, scope, unix_id, allow_aliases, item_cursor,
				     pgo_item, name, status);
    }

}


PUBLIC void sec_rgy_pgo_get_next
#ifndef __STDC__
    ( context, name_domain,scope, item_cursor, pgo_item, name, status )
    sec_rgy_handle_t    context;         /* [in] */
    sec_rgy_domain_t    name_domain;     /* [in] */
    sec_rgy_name_t      scope;           /* [in] */
    sec_rgy_cursor_t    *(item_cursor);  /* [in, out] */
    sec_rgy_pgo_item_t  *(pgo_item);     /* [out] */
    sec_rgy_name_t      name;            /* [out] */
    error_status_t      *(status);       /* [out] */
#else
  (
    sec_rgy_handle_t    context,         /* [in] */
    sec_rgy_domain_t    name_domain,     /* [in] */
    sec_rgy_name_t      scope,           /* [in] */
    sec_rgy_cursor_t    *(item_cursor),  /* [in, out] */
    sec_rgy_pgo_item_t  *(pgo_item),     /* [out] */
    sec_rgy_name_t      name,            /* [out] */
    error_status_t      *(status)
  )
#endif
{
    rs_pgo_query_key_t     key;
    rs_pgo_query_result_t  result;

    key.query = rs_pgo_query_next;
    strcpy( (char *) QUERY_NEXT_SCOPE(key), (char *) scope);
    rgy_pgo_get(context, name_domain, &key, false, item_cursor, &result,
		status );

    if (STATUS_OK(status)) {
        *pgo_item = RESULT_ITEM(result);
	strcpy( (char *) name, (char *) RESULT_NAME(result));
    }
}

PRIVATE void rgy_pgo_key_transfer
#ifndef __STDC__
    ( context, domain, result_type, key, status ) 
    sec_rgy_handle_t    context;        /* [in] */
    sec_rgy_domain_t    domain;         /* [in] */
    rs_pgo_query_t      result_type;    /* [in] */
    rs_pgo_query_key_t  *key;           /* [in, out] */
    error_status_t      *(status);      /* [out] */
#else
  (
    sec_rgy_handle_t    context,        /* [in] */
    sec_rgy_domain_t    domain,         /* [in] */
    rs_pgo_query_t      result_type,    /* [in] */
    rs_pgo_query_key_t  *key,           /* [in, out] */
    error_status_t      *(status)
  )
#endif
{
    rs_cache_data_t        cache_info;

    SETUP_RETRY(rca_op_read, status) {
        Crs_pgo_key_transfer(context, domain, result_type, key, &cache_info,
                                status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_pgo_name_to_id
#ifndef __STDC__
    ( context, name_domain, pgo_name, item_id, status ) 
    sec_rgy_handle_t   context;      /* [in] */
    sec_rgy_domain_t   name_domain;  /* [in] */
    sec_rgy_name_t     pgo_name;     /* [in] */
    uuid_t             *(item_id);   /* [out] */
    error_status_t     *(status);    /* [out] */
#else
  (
    sec_rgy_handle_t   context,      /* [in] */
    sec_rgy_domain_t   name_domain,  /* [in] */
    sec_rgy_name_t     pgo_name,     /* [in] */
    uuid_t             *(item_id),   /* [out] */
    error_status_t     *(status)
  )
#endif
{
    rs_pgo_query_key_t    key;
    error_status_t xst;

    key.query = rs_pgo_query_name;
    strcpy((char *) QUERY_NAME(key), (char *) pgo_name);

    rgy_pgo_key_transfer(context, name_domain, rs_pgo_query_id,
			 &key, status);

    if (STATUS_OK(status)) {
	*item_id = QUERY_ID(key).id;
    } else {
	uuid_create_nil(item_id, &xst);
    }
    
}

PUBLIC void sec_rgy_pgo_id_to_name
#ifndef __STDC__
    ( context, name_domain, item_id, pgo_name, status )
    sec_rgy_handle_t    context;        /* [in] */
    sec_rgy_domain_t    name_domain;    /* [in] */
    uuid_t              *item_id;       /* [in] */
    sec_rgy_name_t      pgo_name;       /* [out] */
    error_status_t      *status;        /* [out] */
#else
  (
    sec_rgy_handle_t    context,        /* [in] */
    sec_rgy_domain_t    name_domain,    /* [in] */
    uuid_t              *item_id,       /* [in] */
    sec_rgy_name_t      pgo_name,       /* [out] */
    error_status_t      *status
  )
#endif
{
    rs_pgo_query_key_t    key;

    key.query = rs_pgo_query_id;
    QUERY_ID(key).id = *item_id;
    strcpy( (char *) QUERY_ID(key).scope, "");

    rgy_pgo_key_transfer(context, name_domain, rs_pgo_query_name,
			 &key, status);

    if (STATUS_OK(status)) {
        strcpy((char *) pgo_name, (char *) QUERY_NAME(key));
    } else {
        *pgo_name = '\0';
    }
}

PUBLIC void sec_rgy_pgo_name_to_unix_num
#ifndef __STDC__
    ( context, name_domain, pgo_name, item_id, status ) 
    sec_rgy_handle_t   context;      /* [in] */
    sec_rgy_domain_t   name_domain;  /* [in] */
    sec_rgy_name_t     pgo_name;     /* [in] */
    signed32           *(item_id);   /* [out] */
    error_status_t     *(status);    /* [out] */
#else
  (
    sec_rgy_handle_t   context,      /* [in] */
    sec_rgy_domain_t   name_domain,  /* [in] */
    sec_rgy_name_t     pgo_name,     /* [in] */
    signed32           *(item_id),   /* [out] */
    error_status_t     *(status)
  )
#endif
{
    rs_pgo_query_key_t    key;

    key.query = rs_pgo_query_name;
    strcpy((char *) QUERY_NAME(key), (char *) pgo_name);

    rgy_pgo_key_transfer(context, name_domain, rs_pgo_query_unix_num,
			 &key, status);

    if (STATUS_OK(status)) {
	*item_id = QUERY_UNIX_NUM(key).unix_num;
    } else {
	*item_id = -1;		/* safer than zero.. */
    }
}

PUBLIC void sec_rgy_pgo_unix_num_to_name
#ifndef __STDC__
    ( context, name_domain, item_id, pgo_name, status )
    sec_rgy_handle_t    context;        /* [in] */
    sec_rgy_domain_t    name_domain;    /* [in] */
    signed32            item_id;        /* [in] */
    sec_rgy_name_t      pgo_name;       /* [out] */
    error_status_t      *status;        /* [out] */
#else
  (
    sec_rgy_handle_t    context,        /* [in] */
    sec_rgy_domain_t    name_domain,    /* [in] */
    signed32            item_id,        /* [in] */
    sec_rgy_name_t      pgo_name,       /* [out] */
    error_status_t      *status
  )
#endif
{
    rs_pgo_query_key_t    key;

    key.query = rs_pgo_query_unix_num;
    QUERY_UNIX_NUM(key).unix_num = item_id;
    strcpy( (char *) QUERY_UNIX_NUM(key).scope, "");

    rgy_pgo_key_transfer(context, name_domain, rs_pgo_query_name,
			 &key, status);

    if (STATUS_OK(status)) {
        strcpy((char *) pgo_name, (char *) QUERY_NAME(key));
    } else {
        *pgo_name = '\0';
    }
}

PUBLIC void sec_rgy_pgo_id_to_unix_num
#ifndef __STDC__
    ( context, name_domain, item_id, item_unix_id, status )
    sec_rgy_handle_t    context;        /* [in] */
    sec_rgy_domain_t    name_domain;    /* [in] */
    uuid_t              *item_id;       /* [in] */
    signed32            *item_unix_id;  /* [out] */
    error_status_t      *status;        /* [out] */
#else
  (
    sec_rgy_handle_t    context,        /* [in] */
    sec_rgy_domain_t    name_domain,    /* [in] */
    uuid_t              *item_id,       /* [in] */
    signed32            *item_unix_id,  /* [out] */
    error_status_t      *status
  )
#endif
{
    rs_pgo_query_key_t    key;

    key.query = rs_pgo_query_id;
    QUERY_ID(key).id = *item_id;
    strcpy( (char *) QUERY_ID(key).scope, "");

    rgy_pgo_key_transfer(context, name_domain, rs_pgo_query_unix_num,
			 &key, status);

    if (STATUS_OK(status)) {
	*item_unix_id = QUERY_UNIX_NUM(key).unix_num;
    } else {
	*item_unix_id = -1;
    }
}

PUBLIC void sec_rgy_pgo_unix_num_to_id
#ifndef __STDC__
    ( context, name_domain, item_unix_id, item_id, status )
    sec_rgy_handle_t    context;        /* [in] */
    sec_rgy_domain_t    name_domain;    /* [in] */
    signed32            item_unix_id;   /* [in] */
    uuid_t              *item_id;       /* [out] */
    error_status_t      *status;        /* [out] */
#else
  (
    sec_rgy_handle_t    context,        /* [in] */
    sec_rgy_domain_t    name_domain,    /* [in] */
    signed32            item_unix_id,   /* [in] */
    uuid_t              *item_id,       /* [out] */
    error_status_t      *status
  )
#endif
{
    rs_pgo_query_key_t    key;
    error_status_t xst;

    key.query = rs_pgo_query_unix_num;
    QUERY_UNIX_NUM(key).unix_num = item_unix_id;
    strcpy( (char *) QUERY_UNIX_NUM(key).scope, "");

    rgy_pgo_key_transfer(context, name_domain, rs_pgo_query_id, 
			 &key, status);

    if (STATUS_OK(status)) {
	*item_id = QUERY_ID(key).id;
    } else {
	uuid_create_nil(item_id, &xst);
    }
}

PUBLIC void sec_rgy_pgo_add_member
#ifndef __STDC__
    ( context, name_domain, go_name, person_name, status )
    sec_rgy_handle_t    context;        /* [in] */
    sec_rgy_domain_t    name_domain;    /* [in] */
    sec_rgy_name_t      go_name;        /* [in] */
    sec_rgy_name_t      person_name;    /* [in] */
    error_status_t      *(status);      /* [out] */
#else
  (
    sec_rgy_handle_t    context,        /* [in] */
    sec_rgy_domain_t    name_domain,    /* [in] */
    sec_rgy_name_t      go_name,        /* [in] */
    sec_rgy_name_t      person_name,    /* [in] */
    error_status_t      *(status)
  )
#endif
{
    rs_cache_data_t        cache_info;

    if (!CHECK_PGO_NAME(go_name) || !CHECK_PGO_NAME(person_name)) {
        SET_STATUS(status, sec_rgy_bad_data);
        return;
    }

    SETUP_RETRY(rca_op_write, status) {
        Crs_pgo_add_member(context, name_domain, go_name, person_name,
                            &cache_info, status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC void sec_rgy_pgo_delete_member
#ifndef __STDC__
    ( context, domain, go_name, person_name, status ) 
    sec_rgy_handle_t   context;        /* [in] */
    sec_rgy_domain_t   domain;         /* [in] */
    sec_rgy_name_t     go_name;        /* [in] */
    sec_rgy_name_t     person_name;    /* [in] */
    error_status_t     *(status);      /* [out] */
#else
  (
    sec_rgy_handle_t   context,        /* [in] */
    sec_rgy_domain_t   domain,         /* [in] */
    sec_rgy_name_t     go_name,        /* [in] */
    sec_rgy_name_t     person_name,    /* [in] */
    error_status_t     *(status)
  )
#endif
{
    rs_cache_data_t        cache_info;

    if (!CHECK_PGO_NAME(go_name) || !CHECK_PGO_NAME(person_name)) {
        SET_STATUS(status, sec_rgy_bad_data);
        return;
    }

    SETUP_RETRY(rca_op_write, status) {
        Crs_pgo_delete_member(context, domain, go_name, person_name,
                                &cache_info, status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

PUBLIC boolean32 sec_rgy_pgo_is_member
#ifndef __STDC__
    ( context, name_domain, go_name, person_name, status )
    sec_rgy_handle_t    context;
    sec_rgy_domain_t    name_domain;          /* [in] */
    sec_rgy_name_t      go_name;              /* [in] */
    sec_rgy_name_t      person_name;          /* [in] */
    error_status_t      *(status);            /* [out] */
#else
  (
    sec_rgy_handle_t    context,
    sec_rgy_domain_t    name_domain,          /* [in] */
    sec_rgy_name_t      go_name,              /* [in] */
    sec_rgy_name_t      person_name,          /* [in] */
    error_status_t      *(status)
  )
#endif
{
    rs_cache_data_t     cache_info;
    volatile boolean32  result = false;

    if (!CHECK_PGO_NAME(go_name) || !CHECK_PGO_NAME(person_name)) {
        SET_STATUS(status, sec_rgy_bad_data);
        return result;
    }

    SETUP_RETRY(rca_op_read, status) {
        result = Crs_pgo_is_member(context, name_domain, go_name, person_name,
                            &cache_info, status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;

    return result;
}

PUBLIC void sec_rgy_pgo_get_members
#ifndef __STDC__
    ( context, name_domain, go_name, member_cursor,  
                                    max_members, member_list, number_supplied, 
                                    number_members, status )
    sec_rgy_handle_t    context;
    sec_rgy_domain_t    name_domain;          /* [in] */
    sec_rgy_name_t      go_name;              /* [in] */
    sec_rgy_cursor_t    *(member_cursor);     /* [in, out] */
    signed32            max_members;          /* [in] */
    sec_rgy_member_t    member_list[];        /* [out] */
    signed32            *(number_supplied);   /* [out] */
    signed32            *(number_members);    /* [out] */
    error_status_t      *(status);            /* [out] */
#else
  (
    sec_rgy_handle_t    context,
    sec_rgy_domain_t    name_domain,          /* [in] */
    sec_rgy_name_t      go_name,              /* [in] */
    sec_rgy_cursor_t    *(member_cursor),     /* [in, out] */
    signed32            max_members,          /* [in] */
    sec_rgy_member_t    member_list[],        /* [out] */
    signed32            *(number_supplied),   /* [out] */
    signed32            *(number_members),    /* [out] */
    error_status_t      *(status)
  )
#endif
{
    rs_cache_data_t        cache_info;

    if (!CHECK_PGO_NAME(go_name)) {
        SET_STATUS(status, sec_rgy_bad_data);
        return;
    }

    SETUP_RETRY(rca_op_read, status) {
        Crs_pgo_get_members(context, name_domain, go_name,
                            member_cursor, max_members, member_list,
                            number_supplied, number_members, &cache_info,
                            status);
        NORMAL_CASE {
            CHECK_CACHE(context, &cache_info);
        }
        RETRY_CASE;
    }
    RETRY_END;
}

