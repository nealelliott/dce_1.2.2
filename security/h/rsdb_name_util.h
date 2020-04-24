/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_name_util.h,v $
 * Revision 1.1.8.3  1996/11/13  18:00:39  arvind
 * 	Fix typo. Add status parameter to rsdb_name_util_localize_pgo().
 * 	[1996/10/18  16:56 UTC  jrr  /main/DCE_1.2.2/jrr_122_6/1]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/10/05  16:59 UTC  aha  /main/HPDCE02/1]
 *
 * 	CHFts16436: collapse // to / in pgo names
 * 	HP 	[1995/10/04  12:13 UTC  aha  /main/aha_mothra10/2]
 *
 * 	CHFts16436: collapse // to / in pgo names;
 * 	eliminate non-ANSI code
 * 	HP 	[1995/10/02  20:23 UTC  aha  /main/aha_mothra10/1]
 * 	[1996/11/13  17:42:37  arvind]
 *
 * Revision 1.1.8.2  1996/07/08  18:33:35  arvind
 * 	Add rsdb_name_util_localize_pgo.
 * 	[1996/06/14  06:09 UTC  jrr  /main/DCE_1.2.2/jrr_122_1/1]
 * 
 * 	merge in global group work for DCE 1.2.2
 * 	[1996/03/27  17:46 UTC  hanfei  /main/DCE_1.2/2]
 * 
 * 	take out __STDC__ for rsdb_name_util_is_foreign_pgo()
 * 	[1996/02/21  15:39 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/2]
 * 
 * 	add header for rsdb_name_util_is_foreign_pgo().
 * 	[1996/02/02  16:36 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/1]
 * 
 * 	Update OSF copyright years
 * 	[1996/02/18  22:18:52  marty]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/10/05  16:59 UTC  aha  /main/HPDCE02/1]
 * 
 * 	CHFts16436: collapse // to / in pgo names
 * 	HP 	[1995/10/04  12:13 UTC  aha  /main/aha_mothra10/2]
 * 
 * 	CHFts16436: collapse // to / in pgo names;
 * 	eliminate non-ANSI code
 * 	HP 	[1995/10/02  20:23 UTC  aha  /main/aha_mothra10/1]
 * 
 * 	CHFts16436: add rsdb_name_canonicalize_components()
 * 	[1995/12/08  16:53:54  root]
 * 
 * Revision 1.1.8.1  1996/05/10  13:17:17  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:46 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/02/21  15:39 UTC  hanfei
 * 	take out __STDC__ for rsdb_name_util_is_foreign_pgo()
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/02/02  16:36 UTC  hanfei
 * 	add header for rsdb_name_util_is_foreign_pgo().
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:11 UTC  psn
 * 	CHFts16436: collapse // to / in pgo names
 * 	[1995/10/05  16:59 UTC  aha  /main/HPDCE02/1]
 * 
 * 	CHFts16436: collapse // to / in pgo names;
 * 	eliminate non-ANSI code
 * 	[1995/10/04  12:13 UTC  aha  /main/aha_mothra10/2]
 * 
 * 	CHFts16436: add rsdb_name_canonicalize_components()
 * 	[1995/10/02  20:23 UTC  aha  /main/aha_mothra10/1]
 * 
 * Revision 1.1.2.2  1992/12/29  13:26:20  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:15:27  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rsdb_name_util.h V=5 10/07/91 //littl/prgy/src/server/rsdb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
 *      Registry Server - rgy object name utilities
 */

#ifndef rsdb_name_util__included
#define rsdb_name_util__included
 
#include <dce/nbase.h>
#include <dce/rgynbase.h>  

#include <rsdb_pvt.h>

/*
*   Used for rsdb_name_util_is_prefix prefix_type parameter
*/
enum prefix_type_t { rsdb_prefix, rsdb_identity,
                        rsdb_over_qualified } prefix_type_t;


PUBLIC void rsdb_name_canonicalize_components (
    sec_rgy_name_t  namep
);

PUBLIC void rsdb_name_canonicalize (
#ifdef __STDC__
    sec_rgy_name_t  cell_namep,
    error_status_t  *status
#endif
);

PUBLIC void rsdb_name_canonicalize_and_convert (
#ifdef __STDC__
    sec_rgy_name_t  cell_namep,
    error_status_t  *status
#endif
);

PUBLIC void rsdb_name_util_convert_to_cell (
#ifdef __STDC__
    sec_rgy_name_t  cell_namep,
    error_status_t  *status
#endif
);


PUBLIC void rsdb_name_util_from_global (
#ifdef __STDC__
    sec_rgy_name_t  cell_namep,
    sec_rgy_name_t  unresolved_namep,
    boolean32       *islocal,
    error_status_t  *status
#endif
);

PUBLIC void rsdb_name_util_convert_from_global (
#ifdef __STDC__
    sec_rgy_name_t  cell_namep,
    sec_rgy_name_t  unresolved_namep,
    boolean32       *islocal,
    error_status_t  *status
#endif
);

   
/*  r s d b _ n a m e _ u t i l _ i s _ p r e f i x
*
*   Compare prefix and pathname.  The input prefix
*   is a legitimate prefix if every component in prefix exactly matches the
*   leftmost components in pathname.  The match must include whole
*   (/-separated) components (i.e., the rightmost component of a prefix may
*   not match a  partial component in the pathname).  If the prefix exactly
*   matches the left-most components of pathname, return TRUE and set type to 
*   one of the following:
*
*   rsdb_prefix         - the pathname has more components than the prefix,
*   rsdb_identity       - the prefix exactly matches the entire pathname, and
*   rsdb_over_qualified - the prefix has more components than the pathname
*                             and the pathname is a prefix of 'prefix.'
*
*   If prefix does not exactly match the left-most components of pathname,
*   return FALSE.   
*
*   Prefix and pathname may contain leading and trailing slash (/) characters.
*/                               

PUBLIC boolean32 rsdb_name_util_is_prefix(
#ifdef __STDC__  
    sec_rgy_name_t  prefix,                     /* [in] prefix              */
    long            prefix_len,                 /* [in]                     */
    sec_rgy_name_t  pathname,                   /* [in] pathname            */
    long            pathname_len,               /* [in]                     */
    short           *type                       /* [out]                    */
#endif
);                          

      
/* r s d b _ n a m e _ u t i l _ i s _ c e l l
 *
 *
 */
PUBLIC boolean32 rsdb_name_util_is_cell (
#ifdef __STDC__
    sec_rgy_name_t          pathname,           /* [in] */
    error_status_t          *st                 /* [out] */
#endif
);

      
/* r s d b _ n a m e _ u t i l _ i t e m _ e x i s t s 
 *
 * Return true if name exists as a directory or pgo in given domain.
 * Return false otherwise.
 */
PUBLIC boolean32 rsdb_name_util_item_exists (
#ifdef __STDC__
    sec_rgy_domain_t        domain,
    sec_rgy_name_t          name,
    error_status_t          *st
#endif
);


/* r s d b _ n a m e _ u t i l _ g e t _ p a r e n t 
 *
 * If object_name has more than one slash-separated component,
 * copy the parent components (every component except right-most)
 * into parent_name and return true.  Otherwise, return false. 
 * Object name should neither begin, nor end, with a slash.
 */
PUBLIC boolean32 rsdb_name_util_get_parent (
#ifdef __STDC__                 
    sec_rgy_name_t      object_name,
    sec_rgy_name_t      parent_name
#endif
);
                                                         

/* r s d b _ n a m e _ u t i l _ g e t _ e x i s t _ p a r e n t
 *
 * Look up input parent_name.  If dir object doesn't exist, look
 * up the parent directory of parent_name (by stripping leaf from
 * parent_name).  Continue in this fashion until a directory
 * lookup is successful.  Return the name of this directory -- the
 * lowest-level existing directory in parent_name.  If none of the
 * directories exist in the given domain, return null string in parent_name.
 *
 * Database Read Lock required around this operation.
 *
 */
PUBLIC boolean32 rsdb_name_util_get_exist_parent (
#ifdef __STDC__
    sec_rgy_domain_t    domain,
    sec_rgy_name_t      parent_name
#endif
);


 /*
  * r s d b _ n a m e _ u t i l _ c o m p l e t e _ n a m e  
  *
  * Use input domain to return the name of the domain.
  * If name does not correspond to any of the three domains, return error.
  * Concatenate object_name to the domain name in complete_name.
  * 
  * Status codes:
  *     sec_rgy_bad_domain - input name doesn't identify any domain.
  *     error_status_ok.
  */
PUBLIC boolean32 rsdb_name_util_complete_name (
#ifdef __STDC__
    sec_rgy_domain_t    domain,
    sec_rgy_name_t      object_name,
    sec_rgy_name_t      complete_name,
    error_status_t      *st
#endif
);



 /* r s d b _ n a m e _ u t i l _ g e t _ l e f t _ c o m p
  *
  * Concatenate the left-most component from pathname (without
  * preceeding "/" characters, if any) to resolved.    Advance pathname
  * pointer to exclude component and "/" characters (both preceeding and
  * following the component). Update the pathname pointer without overwriting
  * the name pointed to. Update pathname_len and resolved_len to reflect the
  * modifications. Set comp to point to the beginning of the component within
  * the resolved string.  Set comp_len equal to the length of the component
  * (without "/" characters).  Return comp_len. 
  *
  *     Params: pathname     - pointer to pathname string whose leftmost
  *                            component is copied and excluded.
  *             pathname_len - updated length of pathname.
  *             resolved     - string into which the component is concatenated. 
  *             resolved_len - updated length of resolved.
  *             comp         - pointer (into resolved) to leftmost component.
  *             comp_len     - length of comp.
  */
PUBLIC void rsdb_name_util_get_left_comp (
#ifdef __STDC__
    char            **pathname,             /* [in, out]    */
    long            *pathname_len,          /* [in, out]    */
    sec_rgy_name_t  resolved,               /* [in, out]    */
    long            *resolved_len,          /* [in, out]    */
    char            **comp,                 /* [out]        */
    long            *comp_len
#endif
);


 /* r s d b _ n a m e _ u t i l _ s t r i p _ l e a f
  *
  * Strip the right-most component from
  * pathname and copy it into leafname.  Remove the trailing '/' from
  * pathname (if it exists) and do not include a leading '/' in leafname. 
  * Both names are returned as null-terminated strings. 
  *
  */
PUBLIC void rsdb_name_util_strip_leaf(
#ifdef __STDC__  
    sec_rgy_name_t      pathname,
    long                *pathname_len,
    sec_rgy_name_t      leafname,
    long                *leafname_len
#endif
);


 /*
  * r s d b _ n a m e _ u t i l _ r e a l m _ f r o m _ c e l l
  *
  * cell points to a cell name.  realm is simply a char pointer
  * with no storage associated with it.  If cell is a legal cell
  * name, make realm point to the substring of cell that names
  * the realm and return true.  If cell is not a legal cell
  * name, return false.
  */

PUBLIC boolean32 rsdb_name_util_realm_from_cell (
#ifdef __STDC__
    char                *cell,
    char                **realm
#endif
);      

 /*
  * r s d b _ n a m e _ u t i l _ i s _ f o r e i g n _ p g o
  *
  * Return true if name start with "/.../", but not /.../<local_cell_name>,
  * else return false.
  *
  */

PUBLIC boolean32 rsdb_name_util_is_foreign_pgo (
    sec_rgy_name_t      pgo
);

 /*
  * r s d b _ n a m e _ u t i l _ l o c a l i z e _ p g o
  *
  * To be called after rsdb_name_util_is_foreign_pgo returns false. This
  * will remove the local cell name prefix from a pgo if it exists.
  *
  */

PUBLIC void rsdb_name_util_localize_pgo (
    sec_rgy_name_t      pgo,
    error_status_t      *st
);

#endif      /* rsdb_name_util__included */
