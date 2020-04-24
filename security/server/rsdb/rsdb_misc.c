/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_misc.c,v $
 * Revision 1.1.7.2  1996/02/18  00:21:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:09:28  marty]
 *
 * Revision 1.1.7.1  1995/12/08  17:59:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:22:24  root]
 * 
 * Revision 1.1.5.2  1994/08/04  16:14:49  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:13:55  mdf]
 * 
 * Revision 1.1.5.1  1994/04/22  19:14:18  rps
 * 	Support registry database polymorphism
 * 	[1994/04/21  00:24:35  rps]
 * 
 * Revision 1.1.3.2  94/03/29  18:44:26  rps
 * 	Handle rsdb_pgodir_type for rgy polymorphism
 * 
 * Revision 1.1.2.2  1992/12/29  16:39:00  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:28:40  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:44:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rsdb_misc.c V=11 10/22/91 //littl/prgy/src/server/rsdb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
 *      Registry Server - database interlude
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rsdb_misc.c	11 - 10/22/91";
#endif

#include <dce/dce.h>
#include <dce/uuid.h>

#include <rs_util.h>
#include <rsdb_util.h>
#include <rsdb_pgo_util.h>
#include <rsdb_pvt.h>
#include <rsdb_misc.h>
#include <rsdb_policy.h>

#include <stdio.h>
#include <sec_svc.h>
                                      
#define uuid_c_version_embedded_unix_id 2
#define SEC_UUID_VERSION_BITS   (uuid_c_version_embedded_unix_id << 12)
#define TIME_HIGH_MASK          0x0fff
#define VERSION_MASK            0xf000
#define SEC_UUID_VERSION_SET(uuid) \
    (((uuid)->time_hi_and_version & VERSION_MASK) == SEC_UUID_VERSION_BITS)


PRIVATE void get_next_free_unix_id (
#ifdef __STDC__     
    sec_rgy_properties_t    *properties,
    sec_rgy_domain_t        domain,
    int                     *unix_id,
    error_status_t          *st
#endif
);

                
/* g e t _ n e x t _ f r e e _ u n i x _ i d
 *
 * Get the next free unix id for the given domain.
 *
 * Keep the search bounded between low and max.  This version
 * remembers the last one given out, so wrap at max and continue
 * searching from low until you reach the starting point.
 *
 */
PRIVATE void get_next_free_unix_id 
#ifndef __STDC__
    ( properties, domain, unix_id, st )
    sec_rgy_properties_t    *properties;    /* [in]  */
    sec_rgy_domain_t        domain;         /* [in]  */
    int                     *unix_id;       /* [out]  */
    error_status_t          *st;            /* [out] */
#else
    (
    sec_rgy_properties_t    *properties,
    sec_rgy_domain_t        domain,
    int                     *unix_id,
    error_status_t          *st
    )
#endif
{
    rsdb_credentials_t      credentials; 
    int                     starting_uxid;
    rsdb_pvt_id_t           item_id;
    rsdb_named_item_t       item;

    CLEAR_STATUS(st);
    /* Look for a free unix id starting with the next assigned
     * unix id stored in the last "sentinal" pgo for the domain.
     */                                                  
    starting_uxid = RSDB_PGO(&domain_info[domain].last).unix_id;
    *unix_id = starting_uxid;
    while( *unix_id <= properties->max_unix_id &&
           rsdb_get_credentials_by_unix_id(domain_info[domain].db, *unix_id,
                                          &credentials) ) {
        (*unix_id)++;
    }   
    /* If max unix id is reached, start looking for a free unix id
     * at the low value for this domain.
     */
    if(*unix_id > properties->max_unix_id) {  
        switch( domain ) {
            case sec_rgy_domain_person:  
                *unix_id = properties->low_unix_id_person;
                break;
            case sec_rgy_domain_group:
                *unix_id = properties->low_unix_id_group;
                break;
            case sec_rgy_domain_org:
                *unix_id = properties->low_unix_id_org;
                break;                               
            default:
                SET_STATUS(st, sec_rgy_bad_domain);
                return;
        }  
        while( *unix_id < starting_uxid &&
               rsdb_get_credentials_by_unix_id(domain_info[domain].db, *unix_id,
                                              &credentials) ) {
            (*unix_id)++;
        }   
        /* If starting_uxid is reached, there are no free unix ids.
         */
        if(*unix_id == starting_uxid) {  
            SET_STATUS(st, sec_rgy_no_more_unix_ids);
            return;
        }
    }                                                    
    /* Store the next unix id to be assigned in this domain. */
    RSDB_PGO(&domain_info[domain].last).unix_id = *unix_id + 1;
    item_id = END_OF_LIST;
    if(!rsdb_pgo_util_get_by_id(domain, item_id, &item)) {
        dce_svc_printf(SEC_RSDB_CANT_GET_ITEM_MSG);
    }
    RSDB_PGO(&item).unix_id = *unix_id + 1;
    rsdb_util_store_item_by_id(domain, item_id, &item);
}


 /* r s d b _ u u i d _ c r e a t e
  *
  * Create a new uuid with the version number set to show that the low 32 bits of
  * time represents a 32 bit Unix id.  If embedded_unix_id is false, just
  * create a uuid without embedding the unix_id.
  *
  * This function assumes that an external mechanism will guarantee uniqueness.
  */

PUBLIC void rsdb_uuid_create
#ifndef __STDC__
    (domain, embedded_unix_id, unix_id, uuid, st)
    sec_rgy_domain_t    domain;             /* [in] */
    boolean32           embedded_unix_id;   /* [in] */
    int                 unix_id;            /* [in] */
    uuid_t              *uuid;              /* [out] */
    error_status_t      *st;                /* [out] */
#else
  (
    sec_rgy_domain_t    domain,
    boolean32           embedded_unix_id,
    int                 unix_id,
    uuid_t              *uuid,
    error_status_t      *st
  )
#endif
{
    do {
        uuid_create(uuid, st);
    } while (BAD_STATUS(st));
                                     
    if(embedded_unix_id) {
        uuid->time_low = unix_id;
        /* To avoid creating identical UUIDs when the 
         * the unix ids of objects in different domains match,
         * indicate domain in the clock_seq_low.
         */
        uuid->clock_seq_low = domain;
        /* Clear the UUID version bits (high four bits of 
         * uuid->time_hi_and_version) set by uuid_create.
         */
        uuid->time_hi_and_version &= TIME_HIGH_MASK;
        uuid->time_hi_and_version |= SEC_UUID_VERSION_BITS;
    }
}


                                
/* r s d b _ u n i x _ u u i d _ g e n _ c h e c k 
 *
 * Generate a unix id and/or uuid OR check that the
 * input unix id and uuid are consistent (uuid contains
 * unix id) if embedded_unix_id is true.  
 * For release DCE 1.0, the embedded_unix_id flag 
 * should be false ONLY when creating ids for a cell principal.
 * Future releases of DCE may not require embedded unix ids, 
 * in which case, this flag my be false.
 *
 * Handle four cases:  Note that the input domain is checked only
 * in the first case.
 * 1. Neither unix id nor uuid given: get the next free unix id
 *    for the given domain; use unix id to create uuid.
 * 2. Unix id given, uuid not given: if object exists for unix_id, copy
 *    its uuid; otherwise, use unix id to create new uuid.
 * 3. Unix id not given, uuid given: extract unix id from uuid.
 * 4. Both unix id and uuid given: extract unix id from uuid and
 *    compare with input unix id -- error if they don't match.    
 *
 * An input unix id of -1 indicates "not given."
 * An input uuid of uuid_nil indicates "not given."
 *
 * Return true if no error, false otherwise.
 *
 * WARNING:  Read_lock database before calling this operation.
 *
 *  Status Codes
 *  sec_rgy_unix_id_changed - (case 4) unix id doesn't match 
 *      unix id extracted from uuid. 
 *  sec_rgy_uuid_bad_version - uuid version bits didn't correspond to
 *      input boolean embedded_unix_id.
 *
 */
PUBLIC boolean32 rsdb_unix_uuid_gen_check
#ifndef __STDC__
    ( domain, embedded_unix_id, unix_id, uuid, st )
    sec_rgy_domain_t    domain;             /* [in]      */
    boolean32           embedded_unix_id;   /* [in]      */
    int                 *unix_id;           /* [in, out] */
    uuid_t              *uuid;              /* [in, out] */
    error_status_t      *st;                /* [out]     */
#else
    (
    sec_rgy_domain_t    domain,
    boolean32           embedded_unix_id,
    int                 *unix_id,
    uuid_t              *uuid,                             
    error_status_t      *st
    )
#endif
{      
    sec_rgy_properties_t    properties; 
    rsdb_credentials_t      credentials; 
    signed32                max_unix_id;

    CLEAR_STATUS(st);
    rsdb_properties_get_info ( &properties, st );
    if(BAD_STATUS(st)) {
        return false;
    }          

    if(*unix_id == sec_rgy_uxid_unknown) {
        if(uuid_is_nil(uuid, st)) {  
            /* Case 1. */
            /* get next free unix id for domain. create uuid */
            get_next_free_unix_id(&properties, domain, unix_id, st);
            if(BAD_STATUS(st)) {
                return false;
            }
            rsdb_uuid_create(domain, embedded_unix_id, *unix_id, uuid, st);
            if(BAD_STATUS(st)) {
                return false;
            }
            return true;
        } else {         
            /* Case 3. */
            if(embedded_unix_id) {
                if(!(SEC_UUID_VERSION_SET(uuid))) {
                    SET_STATUS(st, sec_rgy_uuid_bad_version);
                    return false;
                }
 
                /* extract unix id from uuid */
                *unix_id = uuid->time_low;
            } else {
/*
                if((SEC_UUID_VERSION_SET(uuid))) {
                    SET_STATUS(st, sec_rgy_uuid_bad_version);
                    return false;
                }  
*/
 
                get_next_free_unix_id(&properties, domain, unix_id, st);
            }
            if(BAD_STATUS(st)) {
                return false;
            }
            return true;
        }
    } else {    /* unix id is given */ 
        /* Check that input unix id doesn't exceed max.
         * Make max_unix_id a signed variable for the comparison.
         */
        max_unix_id = (int)properties.max_unix_id;
        if(*unix_id > max_unix_id) {
            SET_STATUS(st, sec_rgy_bad_data);
            return false;
        }
        if(uuid_is_nil(uuid, st)) {
            /* Case 2. */
            /* First, check to see if object exists for this unix_id.
             * If so, copy its uuid.  Otherwise create new uuid.
             */
            if(rsdb_get_credentials_by_unix_id(domain_info[domain].db, 
                                               *unix_id, &credentials) ) {
                *uuid = credentials.uid;
            } else {
                /* use unix id to create uuid */
                rsdb_uuid_create(domain, embedded_unix_id, *unix_id, uuid, st);
                if(BAD_STATUS(st)) {
                    return false;
                }
            }
            return true;
        } else {
            /* Case 4: unix id and uuid given. */
            /* extract unix id from uuid, compare with input unix id */
            if(embedded_unix_id) {
                if(!(SEC_UUID_VERSION_SET(uuid))) {
                    SET_STATUS(st, sec_rgy_uuid_bad_version);
                    return false;
                }
                if(*unix_id != uuid->time_low) {
                    SET_STATUS(st, sec_rgy_unix_id_changed);
                    return false;
                }                            
                return true;
            } else {
/*
                if((SEC_UUID_VERSION_SET(uuid))) {
                    SET_STATUS(st, sec_rgy_uuid_bad_version);
                    return false;
                }  
*/
            }
            return true;
        } 
    }
}                


                                
/* r s d b _ u n i x _ u u i d _ c h e c k 
 *
 * Check that the input unix id and uuid are consistent 
 * (uuid contains unix id) and that the SEC_UUID_VERSION_BITS
 * are set.
 *
 * If both unix id and uuid are given: extract unix id from uuid and
 * compare with input unix id -- error if they don't match.    
 * If only uuid given, make sure version bits are correct.
 * If only unix_id given, nothing to check -- return true.
 *
 * Return true if ids are consistent and no error, false otherwise.
 *
 *  Status Codes
 *  sec_rgy_unix_id_changed - (case 4) unix id doesn't match 
 *      unix id extracted from uuid. 
 *  sec_rgy_uuid_bad_version - uuid version bits incorrect.
 *
 */
PUBLIC boolean32 rsdb_unix_uuid_check
#ifndef __STDC__
    ( unix_id, uuid, st )
    int                 *unix_id;           /* [in] */
    uuid_t              *uuid;              /* [in] */
    error_status_t      *st;                /* [out] */
#else
    (
    int                 *unix_id,
    uuid_t              *uuid,                             
    error_status_t      *st
    )
#endif
{      
    CLEAR_STATUS(st);

    if( uuid_is_nil(uuid, st) ) {
        /* Consistency check unnecessary */
        return true;
    }
    if(!(SEC_UUID_VERSION_SET(uuid))) {
        SET_STATUS(st, sec_rgy_uuid_bad_version);
        return false;
    }
    if( *unix_id == sec_rgy_uxid_unknown ) {
        /* Consistency check unnecessary */
        return true;
    }

    /* extract unix id from uuid, compare with input unix id */
    if(*unix_id != uuid->time_low) {
        SET_STATUS(st, sec_rgy_unix_id_changed);
        return false;
    }                            
    return true;
}



                                
/* r s d b _ u u i d _ e m b e d s _ u n i x _ i d
 *
 * Return true if the SEC_UUID_VERSION_BITS are set on the
 * input uuid.  Return false otherwise.
 *
 */
PUBLIC boolean32 rsdb_uuid_embeds_unix_id
#ifndef __STDC__
    ( uuid )
    uuid_t              *uuid;              /* [in] */
#else
    (
    uuid_t              *uuid
    )
#endif
{      
    if(SEC_UUID_VERSION_SET(uuid)) {
        return true;
    }
    return false;
}

