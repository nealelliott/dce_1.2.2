/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rsdb_name_util.c,v $
 * Revision 1.1.13.4  1996/11/13  18:11:59  arvind
 * 	Add status parameter to rsdb_name_util_localize_pgo.
 * 	[1996/10/18  16:55 UTC  jrr  /main/DCE_1.2.2/jrr_122_6/1]
 *
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/10/05  16:59 UTC  aha  /main/HPDCE02/1]
 *
 * 	CHFts16436: collapse // to / in pgo names
 * 	HP 	[1995/10/03  21:39 UTC  aha  /main/aha_mothra10/4]
 *
 * 	CHFts16436: coding style improvements based on review
 * 	HP 	[1995/10/02  20:24 UTC  aha  /main/aha_mothra10/3]
 *
 * 	CHFts16436: add rsdb_canonicalize_components()
 * 	HP 	[1995/10/02  18:25 UTC  aha  /main/aha_mothra10/2]
 *
 * 	CHFts16436: add //->/ code to rsdb_canonicalize_and_convert
 * 	HP 	[1995/10/02  17:39 UTC  aha  /main/aha_mothra10/1]
 * 	[1996/11/13  17:52:31  arvind]
 *
 * Revision 1.1.13.3  1996/10/15  20:56:56  arvind
 * 	CHFts20196: don't use `index' on non-NULL-terminated strings!
 * 	[1996/10/08  20:05 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/1]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/10/05  16:59 UTC  aha  /main/HPDCE02/1]
 * 
 * 	CHFts16436: collapse // to / in pgo names
 * 	HP 	[1995/10/03  21:39 UTC  aha  /main/aha_mothra10/4]
 * 
 * 	CHFts16436: coding style improvements based on review
 * 	HP 	[1995/10/02  20:24 UTC  aha  /main/aha_mothra10/3]
 * 
 * 	CHFts16436: add rsdb_canonicalize_components()
 * 	HP 	[1995/10/02  18:25 UTC  aha  /main/aha_mothra10/2]
 * 
 * 	CHFts16436: add //->/ code to rsdb_canonicalize_and_convert
 * 	HP 	[1995/10/02  17:39 UTC  aha  /main/aha_mothra10/1]
 * 	[1996/10/15  20:22:49  arvind]
 * 
 * Revision 1.1.13.2  1996/07/08  18:38:49  arvind
 * 	Stylistic noise.
 * 	[1996/06/14  16:19 UTC  jrr  /main/DCE_1.2.2/jrr_122_1/2]
 * 
 * 	Fix rsdb_name_util_is_foreign_pgo to compare cell names
 * 	correctly. Create rsdb_name_util_localize_name to remove
 * 	the cell name from a local name.
 * 	[1996/06/14  06:10 UTC  jrr  /main/DCE_1.2.2/jrr_122_1/1]
 * 
 * 	merge in global group work for DCE 1.2.2
 * 	[1996/03/27  17:48 UTC  hanfei  /main/DCE_1.2/2]
 * 
 * 	fix typo.
 * 	[1996/03/15  17:03 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/2]
 * 
 * 	multi-cell group work: modify check_domain to check_domain_expand in
 * 	rsdb_name_util_complete_name() and add function rsdb_name_util_is_foreign_pgo().
 * 	[1996/02/02  16:32 UTC  hanfei  /main/DCE_1.2/hanfei_dce1.2_b1/1]
 * 
 * 	Update OSF copyright years
 * 	[1996/02/17  23:09:30  marty]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	HP 	[1995/10/05  16:59 UTC  aha  /main/HPDCE02/1]
 * 
 * 	CHFts16436: collapse // to / in pgo names
 * 	HP 	[1995/10/03  21:39 UTC  aha  /main/aha_mothra10/4]
 * 
 * 	CHFts16436: coding style improvements based on review
 * 	HP 	[1995/10/02  20:24 UTC  aha  /main/aha_mothra10/3]
 * 
 * 	CHFts16436: add rsdb_canonicalize_components()
 * 	HP 	[1995/10/02  18:25 UTC  aha  /main/aha_mothra10/2]
 * 
 * 	CHFts16436: add //->/ code to rsdb_canonicalize_and_convert
 * 	HP 	[1995/10/02  17:39 UTC  aha  /main/aha_mothra10/1]
 * 
 * 	CHFts16436: pgo name with // crashes secd
 * 	[1995/12/08  17:22:25  root]
 * 
 * Revision 1.1.13.1  1996/05/10  13:25:03  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/03/27  17:48 UTC  hanfei
 * 	merge in global group work for DCE 1.2.2
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/2  1996/03/15  17:03 UTC  hanfei
 * 	fix typo.
 * 
 * 	HP revision /main/DCE_1.2/hanfei_dce1.2_b1/1  1996/02/02  16:32 UTC  hanfei
 * 	multi-cell group work: modify check_domain to check_domain_expand in
 * 	rsdb_name_util_complete_name() and add function rsdb_name_util_is_foreign_pgo().
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:19 UTC  psn
 * 	CHFts16436: collapse // to / in pgo names
 * 	[1995/10/05  16:59 UTC  aha  /main/HPDCE02/1]
 * 
 * 	CHFts16436: coding style improvements based on review
 * 	[1995/10/03  21:39 UTC  aha  /main/aha_mothra10/4]
 * 
 * 	CHFts16436: add rsdb_canonicalize_components()
 * 	[1995/10/02  20:24 UTC  aha  /main/aha_mothra10/3]
 * 
 * 	CHFts16436: add //->/ code to rsdb_canonicalize_and_convert
 * 	[1995/10/02  18:25 UTC  aha  /main/aha_mothra10/2]
 * 
 * 	CHFts16436: pgo name with // crashes secd
 * 	[1995/10/02  17:39 UTC  aha  /main/aha_mothra10/1]
 * 
 * Revision 1.1.6.1  1994/07/15  15:02:54  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:23:32  mdf]
 * 
 * Revision 1.1.2.2  1992/12/29  16:39:08  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:28:44  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:44:40  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rsdb_name_util.c V=9 10/07/91 //littl/prgy/src/server/rsdb
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/* 
 *      Registry Server - rgy object name utilities
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)rsdb_name_util.c	9 - 10/07/91";
#endif

#include <dce/rpc.h>

#include <rsdb_util.h>
#include <rsdb_dir_util.h>
#include <rsdb_name_util.h>

#include <bstring.h>
#include <un_strng.h>
#include <u_str.h>

/* r s d b _ n a m e _ c a n o n i c a l i z e _ c o m p o n e n t s 
 *
 * CHFts16436: canonicalize components of a name by collapsing
 * any adjacent "//" to a single "/".  Modification takes place
 * in place (result will always be the same length or shorter).
 */

PUBLIC void rsdb_name_canonicalize_components
  (
    sec_rgy_name_t  namep
  )
{
    unsigned_char_t *charnext;
    unsigned_char_t *charlast;

    /* collapse any // to / */
    charnext=charlast=namep;
    while (*charlast) {
	*charlast = *charnext++;
        /* charnext now points to the character 
         * that will be copied on the next loop.
         * If both it and the character we just 
         * copied are "/", then we collapse them
         * by not advancing the "last copied" pointer.
         */
	if (*charnext != '/' || *charlast != '/') {
	    charlast++;
	}
    }
}

/* r s d b _ n a m e _ c a n o n i c a l i z e
 *
 * canonicalize a global name.  If the name begins with a /... or
 * krbtgt/ then the residual is canonicalized using the NSI entry_expand_name
 * function.  Expansion takes place in place.
 * CHFts16436: also collapse adjacent // to a single /
 */

PUBLIC void rsdb_name_canonicalize
#ifndef __STDC__
    (cell_namep, status)
    sec_rgy_name_t  cell_namep;
    error_status_t  *status;
#else
  (
    sec_rgy_name_t  cell_namep,
    error_status_t  *status
  )
#endif
{
    boolean32       is_krbtgt;
    boolean32       is_local;
    error_status_t  lst;
    unsigned_char_t *exp_name;

    /* CHFts16436: collapse any // to / */
    rsdb_name_canonicalize_components(cell_namep);

    /*
     * Handle krbtgt/ relative names.
     */
    is_krbtgt = strncmp((char *) cell_namep, KRB_DIR_ROOT, KRB_DIR_ROOT_LEN)
                                == 0;
    if (is_krbtgt) {
        rsdb_name_util_convert_to_cell(cell_namep, status);
    }

    /*
     * Handle global names /... based
     */
    if (is_krbtgt || strncmp((char *) cell_namep, GLOBAL_DIR_ROOT,
                                GLOBAL_DIR_ROOT_LEN) == 0) {

        rpc_ns_entry_expand_name(rpc_c_ns_syntax_dce, cell_namep, &exp_name,
                                    status);
        if (STATUS_OK(status)) {
            if (is_krbtgt) {
                /*
                 * convert back to krbtgt/ form
                 */
                rsdb_name_util_from_global(exp_name, cell_namep, &is_local,
                                                status);
            } else {
                if (u_strlen(exp_name) < sizeof(sec_rgy_name_t)) {
                    u_strcpy(cell_namep, exp_name);
                } else {
                    SET_STATUS(status, sec_rgy_bad_name);
                }
            }
            rpc_string_free(&exp_name, &lst);
        }
    }
}

/* r s d b _ n a m e _ c a n o n i c a l i z e _ a n d _ c o n v e r t
 *
 * canonicalize a global name.  If the name begins with a /... or
 * krbtgt/ then the residual is canonicalized using the NSI entry_expand_name
 * function.  /... relative names are converted into krbtgt relative names.
 * Expansion takes place in place.
 * CHFts16436: also collapse adjacent // to a single /
 */

PUBLIC void rsdb_name_canonicalize_and_convert
#ifndef __STDC__
    (cell_namep, status)
    sec_rgy_name_t  cell_namep;
    error_status_t  *status;
#else
  (
    sec_rgy_name_t  cell_namep,
    error_status_t  *status
  )
#endif
{
    boolean32       is_krbtgt;
    boolean32       is_local;
    error_status_t  lst;
    unsigned_char_t *exp_name;

    /* CHFts16436: collapse any // to / */
    rsdb_name_canonicalize_components(cell_namep);

    /*
     * Handle krbtgt/ relative names.
     */
    is_krbtgt = strncmp((char *) cell_namep, KRB_DIR_ROOT, KRB_DIR_ROOT_LEN)
                                == 0;
    if (is_krbtgt) {
        rsdb_name_util_convert_to_cell(cell_namep, status);
    }

    /*
     * Handle global names /... based
     */
    if (is_krbtgt || strncmp((char *) cell_namep, GLOBAL_DIR_ROOT,
                                GLOBAL_DIR_ROOT_LEN) == 0) {

        rpc_ns_entry_expand_name(rpc_c_ns_syntax_dce, cell_namep, &exp_name,
                                    status);
        if (STATUS_OK(status)) {
            /*
             * convert to krbtgt/ form
             */
            rsdb_name_util_from_global(exp_name, cell_namep, &is_local,
                                            status);
            rpc_string_free(&exp_name, &lst);
        }
    }
}

PUBLIC void rsdb_name_util_convert_to_cell
#ifndef __STDC__
    (cell_namep, status)
    sec_rgy_name_t  cell_namep;
    error_status_t  *status;
#else
  (
    sec_rgy_name_t  cell_namep,
    error_status_t  *status
  )
#endif
{
    /*
     * switch leading krbtgt/ into /.../
     */
    if (strncmp((char *) cell_namep, KRB_DIR_ROOT, KRB_DIR_ROOT_LEN) != 0) {
        SET_STATUS(status, sec_rgy_bad_name);
    } else {
        /*
         * And slide the residual over.
         */
        CLEAR_STATUS(status);
        strcpy((char *) cell_namep, GLOBAL_DIR_ROOT);
        strcpy((char *) &cell_namep[GLOBAL_DIR_ROOT_LEN], 
                (char *) &cell_namep[KRB_DIR_ROOT_LEN]);
    }
}

PUBLIC void rsdb_name_util_from_global
#ifndef __STDC__
    ( cell_namep, unresolved_namep, islocal, status )
    sec_rgy_name_t  cell_namep;
    sec_rgy_name_t  unresolved_namep;
    boolean32       *islocal;
    error_status_t  *status;
#else
  (
    sec_rgy_name_t  cell_namep,
    sec_rgy_name_t  unresolved_namep,
    boolean32       *islocal,
    error_status_t  *status
  )
#endif
{
    unsigned long   name_len;

    CLEAR_STATUS(status);
    *islocal = false;

    /*
     * switch leading /.../ into krbtgt
     */
    if (strncmp((char *) cell_namep, GLOBAL_DIR_ROOT, GLOBAL_DIR_ROOT_LEN) == 0) {
        /*
         * Make sure we have enough room in the buffer...
         */

        name_len = strlen((char *) cell_namep);
        if (name_len + (KRB_DIR_ROOT_LEN - GLOBAL_DIR_ROOT_LEN) 
                 > sec_rgy_name_max_len) {
            SET_STATUS(status, sec_rgy_bad_name);
        } else {
            strcpy((char *) unresolved_namep, KRB_DIR_ROOT);
            strcpy((char *) &unresolved_namep[KRB_DIR_ROOT_LEN], 
                        (char *) &cell_namep[GLOBAL_DIR_ROOT_LEN]);
        }
    } else {
        /*
         * Name was not a global name... must be relative to this cell.  Check
         * if it is Cell root relative - or if it is simply "/" relative.
         */
        *islocal = true;
        if (0 ==
             strncmp((char *) cell_namep, CELL_DIR_ROOT, CELL_DIR_ROOT_LEN-1)
               && (cell_namep[CELL_DIR_ROOT_LEN-1] == '\0'
                   || cell_namep[CELL_DIR_ROOT_LEN-1] 
                            == PATH_COMPONENT_SEPARATOR) ) {
            /*
             * strip off the Cell root prefix.
             */
            if (cell_namep[CELL_DIR_ROOT_LEN-1] == '\0') {
                unresolved_namep[0] = '\0';
            } else {
                strcpy((char *) unresolved_namep, 
                            (char *) &cell_namep[CELL_DIR_ROOT_LEN]);
            }
        } else if (cell_namep[0] == PATH_COMPONENT_SEPARATOR) {
            strcpy((char *) unresolved_namep, (char *) &cell_namep[1]);
        } else {
            strcpy((char *) unresolved_namep, (char *) cell_namep);
        }
    }
}

PUBLIC void rsdb_name_util_convert_from_global
#ifndef __STDC__
    ( cell_namep, unresolved_namep, islocal, status )
    sec_rgy_name_t  cell_namep;
    sec_rgy_name_t  unresolved_namep;
    boolean32       *islocal;
    error_status_t  *status;
#else
  (
    sec_rgy_name_t  cell_namep,
    sec_rgy_name_t  unresolved_namep,
    boolean32       *islocal,
    error_status_t  *status
  )
#endif
{
    rsdb_name_util_from_global(cell_namep, unresolved_namep, islocal, status);

    /*
     * Canonicalize name...
     */
    if (STATUS_OK(status)) {
        rsdb_name_canonicalize(unresolved_namep, status);
    }
}

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

PUBLIC boolean32 rsdb_name_util_is_prefix
#ifndef __STDC__
    (prefix, prefix_len, pathname, pathname_len, type)
    sec_rgy_name_t      prefix;                 /* [in] prefix              */
    long                prefix_len;             /* [in]                     */
    sec_rgy_name_t      pathname;               /* [in] pathname            */
    long                pathname_len;           /* [in]                     */
    short               *type;                  /* [out]                    */
#else
  (
    sec_rgy_name_t      prefix,                 /* [in] prefix              */
    long                prefix_len,             /* [in]                     */
    sec_rgy_name_t      pathname,               /* [in] pathname            */
    long                pathname_len,           /* [in]                     */
    short               *type
  )
#endif
{   
    if(prefix_len <= pathname_len) {
        if(strncmp((char *)prefix, (char *)pathname, prefix_len) == 0) {
            if(prefix_len == pathname_len) {
                *type = rsdb_identity;
                return true;
            }
            /*
            * Prefix is shorter than pathname.  Make sure prefix
            * is composed of complete pathname components by
            * checking the location of slash characters.
            */
            if(prefix[prefix_len - 1] == '/' ||
               pathname[prefix_len]   == '/'   ) { 
                *type = rsdb_prefix;
                return true;
            }
        }
    }   
    /*
    * Else prefix is longer than pathname.  
    * Is pathname a prefix of prefix?  
    * Prefix is over_qualified if pathname exactly matches left-most
    * whole components of prefix.
    */
    else {
        if(strncmp((char *)prefix, (char *)pathname, pathname_len) == 0) { 
            if(pathname[pathname_len - 1] == '/' ||
               prefix[pathname_len]   == '/'   ) { 
                *type = rsdb_over_qualified;
                return true;
            }                              
        }
    }
     
    return false;

}   /* End rsdb_dir_util_is_prefix. */

      
/* r s d b _ n a m e _ u t i l _ i s _ c e l l
 *
 *
 */
PUBLIC boolean32 rsdb_name_util_is_cell
#ifndef __STDC__
    ( pathname, st )
    sec_rgy_name_t          pathname;       /* [in] */
    error_status_t          *st;            /* [out] */
#else
    (
    sec_rgy_name_t          pathname,
    error_status_t          *st
    )
#endif
{ 
    CLEAR_STATUS(st);
    
    if(ustrncmp(pathname, KRB_DIR_ROOT, KRB_DIR_ROOT_LEN) == 0)
        return true;
    
    return false;
}
   
      
/* r s d b _ n a m e _ u t i l _ i t e m _ e x i s t s 
 *
 * Return true if name exists as a directory or pgo in given domain.
 * Return false otherwise.
 */
PUBLIC boolean32 rsdb_name_util_item_exists
#ifndef __STDC__
    ( domain, name, st )
    sec_rgy_domain_t        domain;         /* [in] */
    sec_rgy_name_t          name;           /* [in] */
    error_status_t          *st;            /* [out] */
#else
    (
    sec_rgy_domain_t        domain,
    sec_rgy_name_t          name,
    error_status_t          *st
    )
#endif
{                                         
    long                    name_len;
    rsdb_named_item_t       item;
                                     
    name_len = ustrlen(name);
    return( rsdb_util_get_by_name(domain, name, name_len, &item) );
}


                                           
/* r s d b _ n a m e _ u t i l _ g e t _ p a r e n t 
 *
 * If object_name has more than one slash-separated component,
 * copy the parent components (every component except right-most)
 * into parent_name and return true.  Otherwise, return false. 
 * Object name should neither begin, nor end, with a slash.
 */
PUBLIC boolean32 rsdb_name_util_get_parent
#ifndef __STDC__                 
    ( object_name, parent_name )
    sec_rgy_name_t      object_name;        /* [in] */
    sec_rgy_name_t      parent_name;        /* [out] */
#else
    (
    sec_rgy_name_t      object_name,
    sec_rgy_name_t      parent_name
    )
#endif
{
    char                *i_ptr;      /* index pointer */
    char                *tmp_ptr;
    long                parent_len;

    if(i_ptr = rindex((char *)object_name, '/')) {
        tmp_ptr = (char *)object_name;
        parent_len = i_ptr - tmp_ptr;
        ustrncpy(parent_name, object_name, parent_len);
        parent_name[parent_len] = '\0';
        return true;
    } else {
	parent_name[0] = '\0';
        return false;
    }
}
                                                         

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
 */  
PUBLIC boolean32 rsdb_name_util_get_exist_parent
#ifndef __STDC__
    ( domain, parent_name )
    sec_rgy_domain_t    domain;             /* [in] */
    sec_rgy_name_t      parent_name;        /* [in, out] */
#else
    (
    sec_rgy_domain_t    domain,
    sec_rgy_name_t      parent_name 
    )
#endif
{  
    long                    parent_len;
    rsdb_named_item_t       dir_item; 
    sec_rgy_name_t          tmp_name;
                                              
    parent_len = ustrlen(parent_name);

    while( !rsdb_dir_util_get_by_name( domain, parent_name,
                                    parent_len, &dir_item ) ) {
        if( !rsdb_name_util_get_parent(parent_name, tmp_name) ) {
            parent_name[0] = '\0';
            return true;
        }
        parent_len = ustrlen(tmp_name);
        parent_name[parent_len] = '\0';
    }                                                                
    return true;
} 

                          
 /*
  * r s d b _ n a m e _ u t i l _ c o m p l e t e _ n a m e
  *
  * Use input domain to get the name of the domain.
  * If domain does not correspond to any of the three domains, return error.
  * Concatenate object_name to the domain name in complete_name.
  * 
  * Database read lock required around this operation.
  * 
  * Status codes:
  *     sec_rgy_bad_domain - input domain invalid.
  *     error_status_ok.
  */
PUBLIC boolean32 rsdb_name_util_complete_name
#ifndef __STDC__
    ( domain, object_name, complete_name, st)
    sec_rgy_domain_t    domain;                 /* [in] */
    sec_rgy_name_t      object_name;            /* [in] */
    sec_rgy_name_t      complete_name;          /* [out] */
    error_status_t      *st;                    /* [out] */
#else
  (
    sec_rgy_domain_t    domain,                 
    sec_rgy_name_t      object_name,
    sec_rgy_name_t      complete_name,
    error_status_t      *st
  )
#endif
{
    rsdb_name_key_t name_key;
    long            name_key_len = sizeof(name_key);
    long            object_name_len;
                                     
    CLEAR_STATUS(st);               
    object_name_len = ustrlen(object_name);

    /* Retrieve the name_key record of the given domain directory.
     */                              
    if(!check_domain_expand(domain)) {
        SET_STATUS(st, sec_rgy_bad_domain);
        return false;
    }

    rsdb_util_get_key_of_id(domain, START_OF_LIST,
                            (Pointer)&name_key, &name_key_len);
    ustrncpy(complete_name, name_key.name, name_key.name_len);
    complete_name[name_key.name_len] = '\0'; 
    if(object_name_len > 0) {
        ustrcat(complete_name, "/");
        ustrcat(complete_name, object_name);
    }
    return true;
}



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

PUBLIC void rsdb_name_util_get_left_comp
#ifndef __STDC__
    (pathname, pathname_len, resolved, resolved_len, comp, comp_len)
    char            **pathname;             /* [in, out]    */
    long            *pathname_len;          /* [in, out]    */
    sec_rgy_name_t  resolved;               /* [in, out]    */
    long            *resolved_len;          /* [in, out]    */
    char            **comp;                 /* [out]        */
    long            *comp_len;              /* [out]        */
#else
  (
    char            **pathname,             /* [in, out]    */
    long            *pathname_len,          /* [in, out]    */
    sec_rgy_name_t  resolved,               /* [in, out]    */
    long            *resolved_len,          /* [in, out]    */
    char            **comp,                 /* [out]        */
    long            *comp_len
  )
#endif
{
    char           *i_ptr;      /* index pointer */
    char 	   *end_ptr;	/* end-of-buffer pointer */
    
    /*
     * Because pathname is not null-terminated, every update must include a
     * check of the pathname_len. 
     */

    /*
     * Move initial '/'s if present. 
     */
    while (**pathname == '/' && *pathname_len > 0) {
        ustrcat(resolved, "/");
        (*pathname)++;
        (*pathname_len)--;
        (*resolved_len)++;
    }

    /*
     * Determine length of next component (terminated by '/' or end of
     * pathname). 
     */
    for (i_ptr = *pathname, end_ptr = i_ptr + *pathname_len;
	 i_ptr < end_ptr;
	 i_ptr++)
    {
	if (*i_ptr == '/')
	    break;
    }
    
    *comp_len = i_ptr - *pathname;
    if (*comp_len == 0)
        return;

    /*
     * Make comp point after last character in resolved so it will point to
     * where the component is to be concatenated. Concatenate left component
     * of pathname to resolved. Advance pathname pointer to exclude the left
     * component. Adjust the lengths of pathname and resolved. 
     */
    *comp = (char *) &resolved[*resolved_len];
    ustrncat(resolved, *pathname, *comp_len);
    *pathname += *comp_len;
    *pathname_len -= *comp_len;
    *resolved_len += *comp_len;
    resolved[*resolved_len] = '\0';
}                               /* End of GET_LEFT_COMP */


 /*
  * r s d b _ n a m e _ u t i l _ s t r i p _ l e a f
  *
  * Strip the right-most component from pathname and copy it into leafname. 
  * Remove the trailing '/' from pathname (if it exists) and do not
  * include a leading '/' in leafname.  Both names are returned as
  * null-terminated strings. 
  */

PUBLIC void rsdb_name_util_strip_leaf
#ifndef __STDC__
    ( pathname, pathname_len, leafname, leafname_len )
    sec_rgy_name_t      pathname;       /* [in, out]                */ 
    long                *pathname_len;  /* [in, out]                */
    sec_rgy_name_t      leafname;       /* [out]                    */ 
    long                *leafname_len;  /* [out]                    */
#else
  (
    sec_rgy_name_t      pathname,       /* [in, out]                */ 
    long                *pathname_len,  /* [in, out]                */
    sec_rgy_name_t      leafname,       /* [out]                    */ 
    long                *leafname_len
  )
#endif
{
    char           *slash_pos;  /* last '/' in pathname     */

    pathname[*pathname_len] = '\0';
    /*
     * Strip trailing '/' characters from pathname. Strip the right-most
     * component from pathname. 
     */
    slash_pos = rindex((char *) pathname, '/');
    while (slash_pos == (char *) &pathname[*pathname_len - 1]) {
        /*
         * Slash is last character in pathname. Strip it and find next to
         * last slash. 
         */
        pathname[(*pathname_len)-- - 1] = '\0';
        slash_pos = rindex((char *) pathname, '/');
    }
    if (slash_pos != 0) {
        /*
         * There is at least one slash embedded in the pathname. Copy the
         * right-most component into leafname and remove it from the end
         * of pathname. Again, remove trailing '/' characters from
         * pathname. 
         */
        ustrcpy(leafname, (slash_pos + 1));
        *leafname_len = ustrlen(leafname);
        *slash_pos = '\0';
        *pathname_len -= *leafname_len + 1;

        slash_pos = rindex((char *) pathname, '/');
        while (slash_pos == (char *) &pathname[*pathname_len - 1]) {
            /*
             * Slash is last character in pathname. Strip it and find next
             * to last slash. 
             */
            pathname[(*pathname_len)-- - 1] = '\0';
            slash_pos = rindex((char *) pathname, '/');
        }
    } else {
        /*
         * There is no slash in pathname. Pathname contains only one
         * component; move it into leafname. 
         */
        ustrcpy(leafname, pathname);
        pathname[0] = '\0';
        *leafname_len = *pathname_len;
        *pathname_len = 0;
    }

}                      



 /*
  * r s d b _ n a m e _ u t i l _ r e a l m _ f r o m _ c e l l
  *
  * cell points to a cell name.  realm is simply a char pointer
  * with no storage associated with it.  If cell is a legal cell
  * name, make realm point to the substring of cell that names
  * the realm and return true.  If cell is not a legal cell
  * name, return false.
  */

PUBLIC boolean32 rsdb_name_util_realm_from_cell
#ifndef __STDC__
    ( cell, realm )
    char                *cell;          /* [in] */
    char                **realm;        /* [out] */
#else
  (
    char                *cell,
    char                **realm
  )
#endif
{
    if (strncmp(cell, GLOBAL_DIR_ROOT, GLOBAL_DIR_ROOT_LEN) == 0) {
        *realm = cell + GLOBAL_DIR_ROOT_LEN;
        return true;
    } else {
        return false;
    }
}


 /*
  * r s d b _ n a m e _ u t i l _ i s _ f o r e i g n _ p g o
  *
  * Return true if name start with "/.../", but not /.../<local_cell_name>,
  * else return false.
  *
  */

PUBLIC boolean32 rsdb_name_util_is_foreign_pgo
(
    sec_rgy_name_t      pgo    /* [in] */
)
{
    char *cell_name = NULL;
    error_status_t      st;

    if (strncmp((char *)pgo, GLOBAL_DIR_ROOT, GLOBAL_DIR_ROOT_LEN) == 0) {
	dce_cf_get_cell_name(&cell_name, &st);
	if ( cell_name ) {
	    if ( strncmp(cell_name, (char *)pgo, strlen(cell_name)) == 0 ) {
	        /* starts with "/.../<local_cell_name>", so is not foreign */
                free(cell_name);
	    	return false;
	    }
            free(cell_name);
        }
        return true;
    } else {
        return false;
    }
}
   
 /*
  * r s d b _ n a m e _ u t i l _ l o c a l i z e _ p g o
  *
  * To be called after rsdb_name_util_is_foreign_pgo returns false. This
  * will remove the local cell name prefix from a pgo if it exists.
  *
  */

PUBLIC void rsdb_name_util_localize_pgo
(
    sec_rgy_name_t      pgo,    /* [in, out] */
    error_status_t      *st
)
{
    char                *cell_name = NULL;
    sec_rgy_name_t      temp;
    unsigned32          index;

    SET_STATUS(st, error_status_ok);

    if (strncmp((char *)pgo, GLOBAL_DIR_ROOT, GLOBAL_DIR_ROOT_LEN) == 0) {
	dce_cf_get_cell_name(&cell_name, st);
        if (BAD_STATUS(st)) 
            return;

	if ( cell_name ) {
	    if ( strncmp(cell_name, (char *)pgo, strlen(cell_name)) == 0 ) {
	        /* It's a fully qualified name in our local cell */
                strcpy((char *)temp, (char *)pgo);
                index = strlen(cell_name) + 1;
                strcpy((char *)pgo, (char *)&temp[index]);
	    }
            free(cell_name);
        }
    }
    return;
}
   


