/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_dir.h,v $
 * Revision 1.1.4.2  1996/02/18  23:00:12  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:39  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:28:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:48  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:26:03  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:15:19  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rsdb_dir.h V=6 05/30/91 //littl/prgy/src/server/rsdb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Server - database directory operations
 * 
 */

#ifndef rsdb_dir_h__included  
#define rsdb_dir_h__included


#include <dce/nbase.h>
#include <dce/rgynbase.h>

#include <rsdb_pvt.h>   

/* 
* Types and Defines
*/


/*
*   RSDB_DIR_CREATE: In the given domain database, create the named directory
*   as a child of the parent directory implied by the pathname.  The new 
*   directory inherits the ACLs of the parent.  If the parent, or any 
*   higher-level directory, doesn't exist, return sec_rgy_dir_not_found.
*
*   Status: error_status_ok, sec_rgy_dir_not_found
*
*/                          

PUBLIC void rsdb_dir_create(
#ifdef __STDC__    
    sec_rgy_domain_t    domain,                 /* [in]                     */
    sec_rgy_name_t      name,                   /* [in] new dir pathname    */
    error_status_t      *status                 /* [out]                    */
#endif
); 
        

/*
*   RSDB_DIR_DELETE: Delete the directory, identified by name,
*   from the given domain database.  If the directory is not empty, 
*   return sec_rgy_dir_not_empty and do not delete the directory.                                           
*
*   Status: error_status_ok, sec_rgy_dir_not_found, sec_rgy_dir_not_empty
*/

PUBLIC void rsdb_dir_delete(
#ifdef __STDC__    
    sec_rgy_domain_t    domain,                 /* [in]                     */
    sec_rgy_name_t      name,                   /* [in] directory pathname  */
    error_status_t      *status                 /* [out]                    */
#endif
);                                                    



/*      
*
*   RSDB_DIR_MOVE: Move the directory (and implicitly all its subdirectories), 
*   identified by old_name, to a new directory, new_name, within a given 
*   domain.  If the new_name doesn't exist, but its parent directory
*   does exist, move the old directory to the parent directory and
*   rename it new_name.  Else if neither new_name, nor its implied
*   parent, exist return sec_rgy_dir_not_found.  Else if the new directory
*   already contains an entry with the same name as the old directory,
*   return sec_rgy_name_exists.  If old_name doesn't exist, return
*   sec_rgy_dir_not_found.  
*
*   Directory pathnames, old_name and new_name, must be fully qualified
*   (i.e., no relative pathnames).                            
*
*   Status: error_status_ok, sec_rgy_dir_not_found, sec_rgy_name_exists     
*
*/

PUBLIC void rsdb_dir_move(                                             
#ifdef __STDC__  
    sec_rgy_domain_t    domain,                /* [in]                     */
    sec_rgy_name_t      old_name,              /* [in] old dir pathname    */
    sec_rgy_name_t      new_name,              /* [in] new directory name  */
    error_status_t      *status                /* [out]                    */
#endif
);                                 


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
PUBLIC boolean32 rsdb_dir_is_empty (
#ifdef __STDC__
    sec_rgy_domain_t    domain,           
    sec_rgy_name_t      dir_name,         
    error_status_t      *status
#endif
);

#endif /* rsdb_dir_h__included */
