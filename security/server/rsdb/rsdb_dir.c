/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_dir.c,v $
 * Revision 1.1.10.3  1996/02/18  00:21:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:09:17  marty]
 *
 * Revision 1.1.10.2  1995/12/08  17:59:24  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/17  23:52 UTC  rps
 * 	migration: conditionally repress pgodir creation
 * 	[1995/05/08  17:13 UTC  rps  /main/rps_wp/1]
 * 
 * Revision 1.1.6.4  1994/08/04  16:14:46  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/26  19:13:47  mdf]
 * 
 * Revision 1.1.6.2  1994/04/22  19:14:15  rps
 * 	Support registry database polymorphism
 * 	[1994/04/21  00:23:30  rps]
 * 
 * 	Moved location of num_attrs and attr_list_id
 * 	[1994/03/30  16:46:25  rps]
 * 
 * Revision 1.1.4.2  1994/03/29  23:46:31  rps
 * 	Support rgy polymorphism
 * 
 * Revision 1.1.3.2  1992/12/29  16:38:48  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:28:23  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:44:37  devrcs
 * 	Initial revision
 * 
 * Revision 1.1.6.1  1994/03/02  17:56:06  hanfei
 * 	Initial security drop
 * 	[1994/02/18  20:52:40  hanfei]
 * 
 * 	HP revision /main/ODESSA_2/1  1994/02/10  04:51  ahop
 * 	 merge ODESSA_ERA4 up to ODESSA_2
 * 
 * 	HP revision /main/ODESSA_ERA4/1  1994/02/07  19:43  mdf
 * 	merge from ODESSA_ERA3 to ODESSA_ERA4.
 * 
 * 	HP revision /main/ODESSA_ERA3/1  1994/02/07  01:48  ahop
 * 	 merge it up
 * 
 * 	HP revision /main/ahop_odss_era_bl8/1  1994/02/05  22:16  ahop
 * 	initialize num_attrs and attr_list_id before storing new item
 * 
 * Revision 1.1.3.2  1992/12/29  16:38:48  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:28:23  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:44:37  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rsdb_dir.c V=8 05/30/91 //littl/prgy/src/server/rsdb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server - database directory operations
 * 
 */

#include <stdio.h>
#include <Pointer.h>
#include <un_strng.h> 

#include <dce/dce.h>
#include <dce/uuid.h>

#include <rsdb_pvt.h>   
#include <rsdb_util.h>
#include <rsdb_acl.h>
#include <rsdb_dir_util.h>  
#include <rsdb_name_util.h>  
#include <rsdb_pgo_util.h>  
#include <rsdb_dir.h>

#include <rs_ver.h>
#include <rs_ver_pvt.h>

#include <sec_svc.h>

/* 
* Types and Defines
*/



 /*
  * RSDB_DIR_CREATE: In the given domain database, create the named
  * directory as a child of the parent directory implied by the pathname. 
  * The new directory inherits the ACLs of the parent.  If the parent, or
  * any higher-level directory, doesn't exist, return
  * sec_rgy_dir_not_found. If a pgo_item or directory of the same name
  * already exists, return sec_rgy_name_exists. 
  *
  * The pathname must be relative to the domain and must not begin with a
  * slash ('/').  For example, to create the new directory whose
  * "absolute" pathname is /auth/persons/r_d/sfwr, pass to rsdb_dir_create
  * "r_d/sfwr" in the name parameter. 
  *
  * Status: error_status_ok, sec_rgy_dir_not_found, sec_rgy_name_exists,
  * sec_rgy_bad_name 
  *
  */

PUBLIC void rsdb_dir_create
#ifndef __STDC__
    ( domain, name, status ) 
    sec_rgy_domain_t    domain;   /* [in]                     */
    sec_rgy_name_t      name;     /* [in] new dir pathname    */
    error_status_t      *status;  /* [out]                    */
#else
  (
    sec_rgy_domain_t    domain,   /* [in]                     */
    sec_rgy_name_t      name,     /* [in] new dir pathname    */
    error_status_t      *status
  )
#endif
{                        
    long                name_len;
    rsdb_named_item_t	item;                   /* used to lookup dir */
    rsdb_named_item_t	newitem;
    rsdb_dir_item_t	*dirp;
    sec_rgy_name_t      component;              /* new dir component name   */
    long                component_len;        
    sec_rgy_name_t      parent_name;
    long                parent_name_len;
    rsdb_named_item_t   parent;                 /* parent dir_item          */
    rsdb_pvt_id_t       parent_id;           
                              
    CLEAR_STATUS(status);
    name_len = ustrlen(name);  
    if(name_len <= 0) {
        SET_STATUS(status, sec_rgy_bad_name);
        return;
    }   
    /*     
     * Make sure a dir by the same name doesn't already exist.
    */
    if (rsdb_dir_util_get_by_name(domain, name, name_len, &item)) {
        SET_STATUS(status, sec_rgy_name_exists);
        return;
    }
    /*
    * Get dir_item of the parent directory implied by the pathname.
    * Strip the right-most component from name
    * and put it in component with rsdb_name_util_strip_leaf.
    * Call rsdb_dir_util_path_resolve to get directory item.
    */     
    ustrcpy(parent_name, name);                                      
    parent_name_len = ustrlen(parent_name);  
    rsdb_name_util_strip_leaf(parent_name, &parent_name_len,
                              component, &component_len);

    if(parent_name_len == 0) {
        /*                                     
        * Name contained only one component, implying that the
        * parent directory is the root of this domain.                                   
        */     
        parent_id = START_OF_LIST;
        if(!rsdb_util_get_item_by_id(domain, parent_id, &parent)) {
            SET_STATUS(status, sec_rgy_dir_not_found);
            return;     
        }
    } else {                         
        /* 
        * Use name to retrieve parent dir_item.   
        * Copy pertinent info from parent to new dir_item.
        */                            
        if(!rsdb_dir_util_get_by_name(domain, parent_name,
                                      parent_name_len, &parent)) {
            SET_STATUS(status, sec_rgy_dir_not_found);
            return;
        }                                                 
        parent_id                   = parent.body.threads.id;
    }                      

    /*     
     * If it's already a pgo node, convert to aggregate form
     */
    if (rsdb_pgo_util_get_by_name(domain, name, name_len, &item)) {
	if (!rs_sw_feature_test_support(rs_c_sw_feature_polymorph, status))
	    return;

	newitem.type = rsdb_pgo_type | rsdb_dir_type;
	memcpy(&RSDB_DIR(&newitem).threads, &item.body.threads,
		sizeof(item.body.threads));
	memcpy(&RSDB_PGO(&newitem), &RSDB_PGO(&item),
		PGO_LEN(&RSDB_PGO(&item)));
    }
    else {
	newitem.type = rsdb_dir_type;
    }

    dirp = &RSDB_DIR(&newitem);
    /*
     * Get a UUID for the new directory.
    */
    uuid_create(&dirp->uid, status);
    if(BAD_STATUS(status)) {
	SET_STATUS(status, sec_rgy_dir_could_not_create);
	return;                 
    }
    dirp->acl         = rsdb_acl_copy(RSDB_DIR(&parent).def_dir_acl, status);
    dirp->def_dir_acl = rsdb_acl_copy(RSDB_DIR(&parent).def_dir_acl, status);
    dirp->def_obj_acl = rsdb_acl_copy(RSDB_DIR(&parent).def_obj_acl, status);

    dirp->num_attrs = 0;
    dirp->attr_list_id = END_OF_LIST;

    if (RSDB_IS_PGO(&newitem)) {

	rsdb_uid_key_t		uid_key;
	rsdb_name_key_t		name_key;
	long			name_key_len;

	rsdb_util_store_item_by_id(domain, newitem.body.threads.id, &newitem);

	/* Since item is newly a directory, also store dir uid key */
	memset(&name_key, 0, sizeof(name_key));
	name_key.record_type = NAME_TYPE;
	name_key.parent_id = parent_id;
	name_key.name_len = component_len;
	strncpy((char *) name_key.name, (char *) component, component_len);
	name_key_len = NAME_LEN(&name_key);

	memset(&uid_key, 0, sizeof(uid_key));
	uid_key.record_type = DIRECTORY_UUID_TYPE;
	uid_key.uid = dirp->uid;

	rsdb_store(domain_info[domain].db, (Pointer) &uid_key, sizeof(uid_key),
		   (Pointer) &name_key, name_key_len, status);
	if (BAD_STATUS(status)) {
            dce_svc_printf(DCE_SVC(sec_svc_handle, ""),
                                   sec_s_rs_rsdb,
                                   svc_c_sev_fatal | svc_c_action_abort,
                                   *status);
	}
    }
    else
	rsdb_util_store_new_named_item(domain, parent_id, component,
					component_len, &newitem);

}   /* End rsdb_dir_create */
        

/*
 *   RSDB_DIR_DELETE: Delete the directory, identified by name,
 *   from the given domain database.  If the directory is not empty, 
 *   return sec_rgy_dir_not_empty and do not delete the directory.                                           
 *
 *   Status: error_status_ok, sec_rgy_dir_not_found, sec_rgy_dir_not_empty
 */

PUBLIC void rsdb_dir_delete
#ifndef __STDC__
    (domain, name, status) 
    sec_rgy_domain_t    domain;                 /* [in]                     */
    sec_rgy_name_t      name;                   /* [in] directory pathname  */
    error_status_t      *status;                /* [out]                    */
#else
  (
    sec_rgy_domain_t    domain,                 /* [in]                     */
    sec_rgy_name_t      name,                   /* [in] directory pathname  */
    error_status_t      *status
  )
#endif
{
    rsdb_named_item_t   dir;
    long                name_len; 

    CLEAR_STATUS(status);
    name_len = strlen( (char *) name);
    
    /* 
    *   Return if directory by that name does not exist.
    */
    if(!rsdb_dir_util_get_by_name(domain, name, name_len, &dir)) {
        SET_STATUS(status, sec_rgy_dir_not_found);
        return;
    } 
    /*
    *   Return if directory is not empty.
    */
    if(!rsdb_dir_util_is_empty(domain, dir.body.threads.id)) {
        SET_STATUS(status, sec_rgy_dir_not_empty);
        return;
    } 

    /* Delete the acls associated with this dir. */
    rsdb_acl_delete(RSDB_DIR(&dir).acl, status);
    rsdb_acl_delete(RSDB_DIR(&dir).def_obj_acl, status);
    rsdb_acl_delete(RSDB_DIR(&dir).def_dir_acl, status);

    /*
    *   Delete from sequential chain if not leaving behind pgo.
    */
    if (RSDB_IS_PGO(&dir)) {
	rsdb_named_item_t	pgo;

	pgo.type = rsdb_pgo_type;
	memcpy(&RSDB_PGO(&pgo), &RSDB_PGO(&dir),
		PGO_LEN(&RSDB_PGO(&dir)));
	memcpy(&pgo.body.threads, &dir.body.threads,
		sizeof(dir.body.threads));
	rsdb_util_store_item_by_id(domain, pgo.body.threads.id, &pgo);
    }
    else {
	rsdb_util_reset_next_item(domain, dir.body.threads.prev,
				  dir.body.threads.next);
	rsdb_util_reset_previous_item(domain, dir.body.threads.next,
				  dir.body.threads.prev);
    }
    /*
    *   Delete database records keyed by name key, ID, and UID as appropriate.
    */
    rsdb_dir_util_purge_dir( domain, &dir );                  

}   /* End rsdb_dir_delete. */

 /* 
  * RSDB_DIR_MOVE: Move the directory (and implicitly all its
  * subdirectories), identified by old_name, to a new directory, new_name,
  * within a given domain. If old_name doesn't exist, return
  * sec_rgy_dir_not_found.  If new_name exists, but identifies a PGO item,
  * return sec_rgy_name_exists. If new_name exists and identifies a
  * directory, move the old directory into the new directory maintaining
  * the old directory component name. If new_name doesn't exist, but its
  * parent directory does exist, move the old directory to the parent
  * directory and rename it new_name. Else if neither new_name, nor its
  * implied parent, exist return sec_rgy_dir_not_found.                                  
  * If new_name is a descendant of old_name, return
  * sec_rgy_dir_move_illegal. 
  *
  * Directory pathnames, old_name and new_name, must be fully qualified
  * relative to the given domain.  Pathnames must not begin with a slash
  * (/). 
  *
  * Status: error_status_ok, sec_rgy_dir_not_found, sec_rgy_name_exists,
  * sec_rgy_dir_move_illegal 
  *
  */

PUBLIC void rsdb_dir_move
#ifndef __STDC__
    ( domain, old_name, new_name, status )
    sec_rgy_domain_t    domain;                 /* [in]                     */
    sec_rgy_name_t      old_name;               /* [in] old dir pathname    */
    sec_rgy_name_t      new_name;               /* [in] new directory name  */
    error_status_t      *status;                /* [out]                    */
#else
  (
    sec_rgy_domain_t    domain,                 /* [in]                     */
    sec_rgy_name_t      old_name,               /* [in] old dir pathname    */
    sec_rgy_name_t      new_name,               /* [in] new directory name  */
    error_status_t      *status
  )
#endif
{                        
    long                old_name_len;
    long                new_name_len;  
    sec_rgy_name_t      parent_name;
    long                parent_name_len; 
    rsdb_pvt_id_t       parent_id;
    rsdb_named_item_t   old_dir;
    rsdb_named_item_t   new_dir;
    rsdb_name_key_t     old_key; 
    long                old_key_len = sizeof(old_key); 
    short               prefix_type;

    CLEAR_STATUS(status);
    old_name_len    = ustrlen(old_name);
    new_name_len    = ustrlen(new_name);
                                                                       
    /*
    *   Check to see if old_name is a parent of new_name.
    *   This case is illegal: can't make a parent directory the
    *   child of one of its descendants. 
    *   If old_name is the same as new_name, return without processing.
    */
    if(rsdb_name_util_is_prefix(old_name, old_name_len, new_name,
                               new_name_len, &prefix_type)) {
        if(prefix_type == rsdb_identity) {
            return;
        }
        if(prefix_type == rsdb_prefix) { 
            SET_STATUS(status, sec_rgy_dir_move_illegal);
            return;
        }
    }
    /*
    *   Return if old_name doesn't exist or if new_name identifies a PGO.
    */
    if (!rsdb_dir_util_get_by_name(domain, old_name, old_name_len, &old_dir)) {
        SET_STATUS(status, sec_rgy_dir_not_found);
        return;
    }
    if (rsdb_pgo_util_get_by_name(domain, new_name, new_name_len, &new_dir)) {
        SET_STATUS(status, sec_rgy_name_exists);
        return;
    }                       
    /*
    *   Get name_key of old directory.
    */              
    rsdb_util_get_key_of_id(domain, old_dir.body.threads.id, 
                            (Pointer)&old_key, &old_key_len);            
    /*
    *   If new_name identifies an existing directory, move the old 
    *   directory into the new directory.
    *   The ID of the new directory becomes the parent_id of the 
    *   old directory.
    */
    if (rsdb_dir_util_get_by_name(domain, new_name, new_name_len, &new_dir)) {
        rsdb_util_change_name_key(domain, &old_dir, new_dir.body.threads.id,  
                                  old_key.name, old_key.name_len, status);
    }
    /*
    *   Else, does the parent of new_name identify a directory?
    *   If not, return sec_rgy_dir_not_found.
    *   If so, move the old directory into the parent directory and 
    *   rename it with the last component of new_name.
    */
    else {                              
        ustrcpy(parent_name, new_name);
        parent_name_len = ustrlen(parent_name);
        rsdb_name_util_strip_leaf(parent_name, &parent_name_len,
                                  old_key.name, &old_key.name_len);  
        if(parent_name_len == 0) {
            parent_id = START_OF_LIST;
        }
        else {
            if (!rsdb_dir_util_get_by_name(domain, parent_name,
                                           parent_name_len, &new_dir)) {
                SET_STATUS(status, sec_rgy_dir_not_found);
                return; 
            }
            parent_id = new_dir.body.threads.id;
        }
        rsdb_util_change_name_key(domain, &old_dir, parent_id,  
                                  old_key.name, old_key.name_len, status);
    }                     
}   /* End rsdb_dir_move. */                  



/* 
 * r s d b _ d i r _ i s _ e m p t y
 *
 * Return true if dir_name names a directory that contains 
 * no other subdirectories or PGOs.
 *
 * Status codes:
 * sec_rgy_dir_not_found - dir_name didn't identify an existing directory.
 *
 */
PUBLIC boolean32 rsdb_dir_is_empty
#ifndef __STDC__
    ( domain, dir_name, status )
    sec_rgy_domain_t    domain;                 /* [in] */
    sec_rgy_name_t      dir_name;               /* [in] dir pathname */
    error_status_t      *status;                /* [out] */
#else
  (
    sec_rgy_domain_t    domain,           
    sec_rgy_name_t      dir_name,         
    error_status_t      *status
  )
#endif
{                          
    rsdb_named_item_t   dir_item;                
    long                dir_name_len;

    CLEAR_STATUS(status);
    dir_name_len = ustrlen(dir_name);
             
    if(!rsdb_dir_util_get_by_name(domain, dir_name, dir_name_len, &dir_item)) {
        SET_STATUS(status, sec_rgy_dir_not_found);
        return false;
    }
    return(rsdb_dir_util_is_empty(domain, dir_item.body.threads.id));
}




