/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: sec_login_db.c,v $
 * Revision 1.1.4.1  1996/10/03  14:44:39  arvind
 * 	A second attempt to fix the compiler warning.
 * 	[1996/09/17  05:52 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/2]
 *
 * 	Silence compiler warnings.
 * 	NB: gcc still warns about unsigned compares;
 * 	ANSI is silent as to whether enum types are signed or unsigned,
 * 	so this warning appears to be bogus.
 * 	[1996/09/17  00:20 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.2.2  1996/03/11  14:09:25  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:30  marty]
 * 
 * Revision 1.1.2.1  1995/12/08  18:04:36  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/9  1995/10/04  19:18 UTC  mullan_s
 * 	Merge fixes for CHFts16546.
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts16496/1  1995/10/04  18:29 UTC  mullan_s
 * 	Call pac_to_epac_chain_bytes instead of pac_to_epac
 * 
 * 	HP revision /main/HPDCE02/8  1995/09/06  19:17 UTC  mullan_s
 * 	Merge fix for CHFts16255
 * 
 * 	HP revision /main/HPDCE02/mullan_bc_errtable_skew/1  1995/09/01  19:24 UTC  mullan_s
 * 	open_file: do not check for db_s_file_lock_fail error.
 * 
 * 	HP revision /main/HPDCE02/7  1995/08/22  21:08 UTC  mullan_s
 * 	Merge fix for CHFts15637
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts15637/3  1995/08/21  21:01 UTC  mullan_s
 * 	sec_login_db_import_context_data: memset fixed and varying before
 * 	filling them in.
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts15637/2  1995/08/09  14:18 UTC  mullan_s
 * 	Fix RCS header to allow hsitory comments to be added.
 * 	[1995/12/08  16:31:25  root]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1995
 * Unpublished work. All Rights Reserved.
 */
/*
 * sec_login_db.c - this file contains routines to read
 * and write data to the credential cache files located
 * in the security creds subdirectory.
 *
 * It consists of APIs which interact with the DCE Backing
 * Store APIs.
 *
 * See src/security/idl/sec_login_encode.idl for more
 * details.
 *
 * NOTE!!! These APIs are not thread-safe! It is up to the
 * caller to provide thread-locking. See sec_login_util.c for
 * examples of thread-safe wrappers.
 */
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <sys/types.h>

#include <dce/assert.h>
#include <dce/dce.h>
#include <dce/nbase.h>
#include <dce/pklbase.h>
#include <dce/rpc.h>
#include <dce/uuid.h>

#include <db.h>
#include <macros.h>
#include <sec_login_db.h>
#include <sec_svc.h>
#include <u_str.h>

#define NUM_DB_TYPES		3
#define DB_BACKEND_TYPE		"bsd4.4-btree"
#define DB_BTREE_PAGE_SIZE 	2048

#define PERMISSIONS(_M)		((_M) & (S_IRWXU | S_IRWXG | S_IRWXO))
#define WANTED_PERMISSIONS	(S_IRUSR | S_IWUSR)

#define INVALID_ENTRY_TYPE(et) ((((int)et)<(int)(generation_id_enum)) || (((int)et)>=(int)(NUM_DB_TYPES)))

/*
 * These MUST remain in the same order as the entries for 
 * sec_login_db_enum_t!!! (See sec_login_encode.idl)
 */
PRIVATE uuid_t sec_db_uuid[] = {
    /* uuid for generation_id_enum */
    { /* 8e9c6e36-6bae-11ce-a0c0-080009242444 */
        0x8e9c6e36,
        0x6bae,
        0x11ce,
        0xa0,
        0xc0,
        {0x08, 0x00, 0x09, 0x24, 0x24, 0x44}
    },
    /* uuid for v1_1_info_enum */
    { /* 8e9c7098-6bae-11ce-a0c0-080009242444 */
        0x8e9c7098,
        0x6bae,
        0x11ce,
        0xa0,
        0xc0,
        {0x08, 0x00, 0x09, 0x24, 0x24, 0x44}
    },
    /* uuid for epac_enum */
    { /* 8e9c723c-6bae-11ce-a0c0-080009242444 */
        0x8e9c723c,
        0x6bae,
        0x11ce,
        0xa0,
        0xc0,
        {0x08, 0x00, 0x09, 0x24, 0x24, 0x44}
    }
};

PUBLIC sec_login_db_enum_t lc_db_types[] = {
    generation_id_enum,
    v1_1_info_enum
};

/*
 * This routine prepares a login context to be saved to
 * disk.
 */
PUBLIC void sec_login_db_import_context_data(
    sec_login_context_t  	*lcp,      	/* [in]  */
    lc_fixed_data_t      	*fixed,    	/* [out] */
    Pointer              	*varying,  	/* [out] */
    sec_login_db_entry_t 	**db_entry, 	/* [out] */
    error_status_t       	*stp   		/* [out] */
)
{
    unsigned32          	var_len;
    idl_pkl_t           	*ppac;
    unsigned32          	ppac_len;
    char                	*p;
    int                 	i;
    sec_login_db_entry_t 	*tmp_db_entry;

    CLEAR_STATUS(stp);
    var_len = 0;
    ppac = NULL;
    memset(fixed, 0, sizeof(*fixed));

    /* first, things that don't affect the length of the varying data */
    fixed->state            = lcp->state;
    fixed->local            = lcp->local;
    fixed->auth_src         = lcp->auth_src;
    fixed->policy           = lcp->identity->policy;
    fixed->info_state       = lcp->identity->state;
    fixed->flags            = lcp->flags;
    fixed->unix_info_valid  = UNIX_CREDS(lcp).pwent != NULL;
    if (fixed->unix_info_valid) {
        fixed->pw_uid           = UNIX_CREDS(lcp).pwent->pw_uid;
        fixed->pw_gid           = UNIX_CREDS(lcp).pwent->pw_gid;
        fixed->pw_change        = UNIX_CREDS(lcp).pwent->pw_change;
        fixed->pw_expire        = UNIX_CREDS(lcp).pwent->pw_expire;
    }
    fixed->num_prev_caches = KRB_INFO(lcp).num_caches;

    /* start tallying up the length of varying data */
    var_len += fixed->cell_name_len   = u_strlen(lcp->cell);
    var_len += fixed->princ_name_len  = u_strlen(lcp->principal);
    var_len += fixed->pepper_len      = KRB_INFO(lcp).pepper.length;
    var_len += fixed->num_prev_caches * sizeof(int);
    for (i=0; i<fixed->num_prev_caches; i++) {
        var_len += strlen(KRB_INFO(lcp).prev_caches[i]);
    }
    if (fixed->unix_info_valid) {
        var_len += fixed->pw_name_len   = strlen(UNIX_CREDS(lcp).pwent->pw_name);
        var_len += fixed->pw_passwd_len = strlen(UNIX_CREDS(lcp).pwent->pw_passwd);
        var_len += fixed->pw_class_len  = strlen(UNIX_CREDS(lcp).pwent->pw_class);
        var_len += fixed->pw_gecos_len  = strlen(UNIX_CREDS(lcp).pwent->pw_gecos);
        var_len += fixed->pw_dir_len    = strlen(UNIX_CREDS(lcp).pwent->pw_dir);
        var_len += fixed->pw_shell_len  = strlen(UNIX_CREDS(lcp).pwent->pw_shell);
        fixed->num_unix_groups  = UNIX_CREDS(lcp).num_groups;
        var_len += fixed->num_unix_groups * sizeof(signed32);
    }

    /* pickle the context pac */
    if (fixed->info_state != info_none) {
        sec_id_pac_t_pickle(&NET_CREDS(lcp), NULL, (void *(*)(unsigned32)) malloc,
                            (unsigned32) 8, &ppac, &ppac_len, stp);
        if (BAD_STATUS(stp))
	    return;
    } else {
       ppac_len = 0;
    }

    var_len += fixed->ppac_len = ppac_len;
    fixed->var_len = var_len;

    /* allocate and populate varying portion */
    *varying = malloc(var_len);
    if (*varying == NULL) {
        SET_STATUS(stp, sec_login_s_no_memory);
        if (ppac != NULL)
            free(ppac);
        return;
    }
    memset(*varying, 0, var_len);

    p = *varying;
    memcpy(p, lcp->cell, fixed->cell_name_len);
    p += fixed->cell_name_len;
    memcpy(p, lcp->principal, fixed->princ_name_len);
    p += fixed->princ_name_len;
    memcpy(p, KRB_INFO(lcp).pepper.contents, fixed->pepper_len);
    p += fixed->pepper_len;
    for (i=0; i<fixed->num_prev_caches; i++) {
        int  len;
        len = strlen(KRB_INFO(lcp).prev_caches[i]);
        memcpy(p, &len, sizeof(int));
        p += sizeof(int);
        memcpy(p, KRB_INFO(lcp).prev_caches[i], len);
        p += len;
    }

    if (fixed->unix_info_valid) {
        memcpy(p, UNIX_CREDS(lcp).pwent->pw_name, fixed->pw_name_len);
        p += fixed->pw_name_len;
        memcpy(p, UNIX_CREDS(lcp).pwent->pw_passwd, fixed->pw_passwd_len);
        p += fixed->pw_passwd_len;
        memcpy(p, UNIX_CREDS(lcp).pwent->pw_class, fixed->pw_class_len);
        p += fixed->pw_class_len;
        memcpy(p, UNIX_CREDS(lcp).pwent->pw_gecos, fixed->pw_gecos_len);
        p += fixed->pw_gecos_len;
        memcpy(p, UNIX_CREDS(lcp).pwent->pw_dir, fixed->pw_dir_len);
        p += fixed->pw_dir_len;
        memcpy(p, UNIX_CREDS(lcp).pwent->pw_shell, fixed->pw_shell_len);
        p += fixed->pw_shell_len;
        memcpy(p, UNIX_CREDS(lcp).groups, fixed->num_unix_groups * sizeof(signed32));
        p += (fixed->num_unix_groups * sizeof(signed32));
    }

    if (ppac != NULL) {
        memcpy(p, ppac, fixed->ppac_len);
        free(ppac);
    }

    /* 
     * DCE 1.1 has 2 distinct pickles to be stored, one is for
     * the generation id and the other is for the rest of the 
     * data. The generation id is pickled separately so that it
     * can be retrieved without retrieving the other DCE 1.1
     * data. This is useful in the sec_login_export_context routine.
     *
     * As new login context data becomes necessary in future revisions
     * of DCE, they should be added to this code. Under no circumstances
     * should any current pickle types be modified or deleted in any
     * way. This would break binary compatibility!!
     */

    tmp_db_entry = (sec_login_db_entry_t *) 
                  malloc(sizeof(sec_login_db_entry_t) * NUM_LC_DB_TYPES);
    if (!tmp_db_entry) {
        SET_STATUS(stp, sec_login_s_no_memory);
        if (*varying)
            free(*varying);
        return;
    }
    memset(tmp_db_entry, 0, NUM_LC_DB_TYPES * sizeof(sec_login_db_entry_t));

    for (i = 0; i < NUM_LC_DB_TYPES; i++) {

        sec_login_db_copy_entries_from_lc(lcp,
                                          lc_db_types[i],
                                          &tmp_db_entry[i],
                                          stp);
        if (BAD_STATUS(stp)) {
            sec_login_db_free_entries(free, tmp_db_entry, NUM_LC_DB_TYPES);
            if (*varying)
                free(*varying);
            return;
        }
    }

    *db_entry = tmp_db_entry;
}

/*
 * This routine reconstructs a login context.
 *
 * NOTE WELL! It is the responsibility of the caller to free
 * the login context (ala sec_login_pvt_free_login_context)
 * if an error is returned. This routine does not clean up 
 * any memory it allocates in the login context.
 */
PUBLIC void sec_login_db_export_context_data(
    lc_fixed_data_t      	*fixed,   	/* [in]  */
    Pointer              	varying,  	/* [in]  */
    sec_login_db_data_t 	*db_data, 	/* [in]  */
    sec_login_context_t  	*lcp,     	/* [out] */
    error_status_t       	*stp      	/* [out] */
)
{
    unsigned32           	var_len;
    char                 	*p;
    int                  	i;
    sec_login_db_entry_t 	*dbp, *dbi;
    error_status_t              lst;

    CLEAR_STATUS(stp);

    lcp->state           = fixed->state;
    lcp->local           = fixed->local;
    lcp->auth_src        = fixed->auth_src;
    lcp->flags           = fixed->flags;
    lcp->identity->policy = fixed->policy;
    lcp->identity->state  = fixed->info_state;
    if (fixed->unix_info_valid) {
        UNIX_CREDS(lcp).pwent->pw_uid     = fixed->pw_uid;
        UNIX_CREDS(lcp).pwent->pw_gid     = fixed->pw_gid;
        UNIX_CREDS(lcp).pwent->pw_change  = fixed->pw_change;
        UNIX_CREDS(lcp).pwent->pw_expire  = fixed->pw_expire;
        UNIX_CREDS(lcp).num_groups        = fixed->num_unix_groups;
    }

    p = varying;
    memcpy(lcp->cell, p, fixed->cell_name_len);
    lcp->cell[fixed->cell_name_len] = '\0';
    p += fixed->cell_name_len;
    memcpy(lcp->principal, p, fixed->princ_name_len);
    lcp->principal[fixed->princ_name_len] = '\0';
    p += fixed->princ_name_len;

    KRB_INFO(lcp).pepper.length = fixed->pepper_len;
    if (fixed->pepper_len > 0) {
        KRB_INFO(lcp).pepper.contents = malloc(fixed->pepper_len);
        if (KRB_INFO(lcp).pepper.contents == NULL) {
            SET_STATUS(stp, sec_login_s_no_memory);
	    return;
        }
        memcpy(KRB_INFO(lcp).pepper.contents, p, fixed->pepper_len);
    } else {
        KRB_INFO(lcp).pepper.contents = NULL;
    }
    p += fixed->pepper_len;

    KRB_INFO(lcp).num_caches = fixed->num_prev_caches;
    if (fixed->num_prev_caches > 0) {
        KRB_INFO(lcp).prev_caches = (char **) malloc(fixed->num_prev_caches * sizeof(char *));
        if (KRB_INFO(lcp).prev_caches == NULL) {
	    SET_STATUS(stp, sec_login_s_no_memory);
            return;
        }
        for(i=0; i<fixed->num_prev_caches; i++) {
            int  len;
            char *cp;

            memcpy((char*)&len, p, sizeof(int));
            p += sizeof(int);
            cp = malloc(len + 1);
            if (cp == NULL) {
                SET_STATUS(stp, sec_login_s_no_memory);
                return;
            }
            memcpy(cp, p, len);
            cp[len] = '\0';
            p += len;
            KRB_INFO(lcp).prev_caches[i] = cp;
        }
    }

    if (fixed->unix_info_valid) {
        memcpy(UNIX_CREDS(lcp).pwent->pw_name, p, fixed->pw_name_len);
        UNIX_CREDS(lcp).pwent->pw_name[fixed->pw_name_len] = '\0';
        p += fixed->pw_name_len;
        memcpy(UNIX_CREDS(lcp).pwent->pw_passwd, p, fixed->pw_passwd_len);
        UNIX_CREDS(lcp).pwent->pw_passwd[fixed->pw_passwd_len] = '\0';
        p += fixed->pw_passwd_len;
        memcpy(UNIX_CREDS(lcp).pwent->pw_class, p, fixed->pw_class_len);
        UNIX_CREDS(lcp).pwent->pw_class[fixed->pw_class_len] = '\0';
        p += fixed->pw_class_len;
        memcpy(UNIX_CREDS(lcp).pwent->pw_gecos, p, fixed->pw_gecos_len);
        UNIX_CREDS(lcp).pwent->pw_gecos[fixed->pw_gecos_len] = '\0';
        p += fixed->pw_gecos_len;
        memcpy(UNIX_CREDS(lcp).pwent->pw_dir, p, fixed->pw_dir_len);
        UNIX_CREDS(lcp).pwent->pw_dir[fixed->pw_dir_len] = '\0';
        p += fixed->pw_dir_len;
        memcpy(UNIX_CREDS(lcp).pwent->pw_shell, p, fixed->pw_shell_len);
        UNIX_CREDS(lcp).pwent->pw_shell[fixed->pw_shell_len] = '\0';
        p += fixed->pw_shell_len;

        if (fixed->num_unix_groups > 0) {
            var_len = fixed->num_unix_groups * sizeof(signed32);
            UNIX_CREDS(lcp).groups = (signed32 *) malloc(var_len);
            if (UNIX_CREDS(lcp).groups == NULL) {
                SET_STATUS(stp, sec_login_s_no_memory);
                return;
            }
            memcpy(UNIX_CREDS(lcp).groups, p, var_len);
            p += var_len;
        } else {
            UNIX_CREDS(lcp).groups = NULL;
        }
    }

    if (fixed->info_state != info_none) {
        char  *temp;

        temp = malloc (fixed->ppac_len);
        if (temp == NULL) {
            SET_STATUS(stp, sec_login_s_no_memory);
            return;
        }
        memcpy (temp, p, fixed->ppac_len);
        sec_id_pac_t_unpickle( (idl_pkl_t *)temp, (void *(*)(unsigned32)) malloc,
                                &NET_CREDS(lcp), stp);
        free(temp);
    }

    if (db_data->num_fetched > 0) {

        dbi = ((sec_login_db_entry_t *) (((char *) &db_data->db_entry[0]) +
                  (sizeof(sec_login_db_entry_t) * db_data->num_fetched)));

        for (dbp = &db_data->db_entry[0]; dbp < dbi; dbp++) {

            sec_login_db_copy_entries_to_lc(dbp,
                                            lcp,
                                            stp);
            if (BAD_STATUS(stp)) {
                /* 
                 * caller will free allocated memory in login context.
                 */
                return;
            }
        }
    }

    /* initialize the rest */
    for (i = 0; i < db_data->num_not_fetched; i++) {
        switch (db_data->not_fetched[i]) {
           
            case generation_id_enum:
                uuid_create_nil(&lcp->generation_id, &lst);
                break;
            case v1_1_info_enum:
                {
                    sec_bytes_t   epac_chain;

                    pac_to_epac_chain_bytes(&NET_CREDS(lcp), &epac_chain, 
			    0, NULL, sec_id_deleg_type_none,
                            sec_id_compat_mode_initiator, stp);

                    if (GOOD_STATUS(stp)) {
                        sec_login_pvt_init_v1_1_info(&epac_chain,
                        	krb5_cc_get_name(KRB_INFO(lcp).cache),
                                &lcp->v1_1_info);
                    }
                }
                break;
            default: /* PROGRAMMING ERROR!!! */
                DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
                    "Invalid database information type (%d)\n", 
                    db_data->not_fetched[i]));
                SET_STATUS(stp, sec_login_s_internal_error);
                return;
        }
    }
}

/*
 * Given the type of a sec_login_db_entry_t,
 * this routine copies the appropriate fields of a login
 * context to the sec_login_db_entry_t.
 */
PRIVATE void sec_login_db_copy_entries_from_lc(
    sec_login_context_t		*lcp,		/* [in]  */
    sec_login_db_enum_t		db_type,	/* [in]  */
    sec_login_db_entry_t	*db_entry,	/* [out] */
    error_status_t		*stp		/* [out] */
)
{
    CLEAR_STATUS(stp);

    switch (db_type) {
        case generation_id_enum:
            db_entry->contents.generation_id = lcp->generation_id;
            break;
        case v1_1_info_enum:
            sec_login_pvt_copy_v1_1_info(&db_entry->contents.v1_1_info, 
                                         &lcp->v1_1_info, stp);
            break;
        default:
            DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
                "Invalid database information type (%d)\n", db_type));
            SET_STATUS(stp, sec_login_s_internal_error);
            return;
    }

    db_entry->entry_type = db_type;

}

/*
 * This routine copies a sec_login_db_entry_t to the appropriate
 * location in a login context.
 */
PRIVATE void sec_login_db_copy_entries_to_lc(
    sec_login_db_entry_t	*db_entry,	/* [in]  */
    sec_login_context_t		*lcp,		/* [out] */
    error_status_t		*stp		/* [out] */
)
{
    CLEAR_STATUS(stp);

    switch(db_entry->entry_type) {
        case generation_id_enum:
            lcp->generation_id = db_entry->contents.generation_id;
            break;
        case v1_1_info_enum:
            sec_login_pvt_copy_v1_1_info(&lcp->v1_1_info, 
                                         &db_entry->contents.v1_1_info,
                                         stp);
            break;
        default:
            DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
                "Invalid database information type (%d)\n", db_entry->entry_type));
            SET_STATUS(stp, sec_login_s_internal_error);
            return;
    }
}

/*
 * This routine stores an array of sec_login_db_entry_t in the
 * ".db" file named datafile.
 */
PUBLIC void sec_login_db_store_entries(
    char                        *datafile,      /* [in]  */
    unsigned32			num_db_entries, /* [in]  */
    sec_login_db_entry_t	*db_entry,	/* [in]  */
    error_status_t		*stp		/* [out] */
)
{
    sec_login_db_entry_t	*dbp, *dbi;
    dce_db_handle_t		dbh;
    error_status_t		lst;

    CLEAR_STATUS(stp);
   
    sec_login_db_open_and_lock(datafile,
                               db_c_index_by_uuid,
                               &dbh, stp);
    if (BAD_STATUS(stp)) {
        DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
            "sec_login_db_open_and_lock failed: filename = %s, status = %d\n", datafile, *stp));
	SET_STATUS(stp, sec_login_s_internal_error);
        return;
    }

    dbi = ((sec_login_db_entry_t *) (((char *) &db_entry[0]) + 
              (sizeof(sec_login_db_entry_t) * num_db_entries)));

    for (dbp = &db_entry[0]; dbp < dbi; dbp++) {

        dce_db_store_by_uuid(dbh,
                             &sec_db_uuid[dbp->entry_type],
                             dbp,
                             stp);
        if (BAD_STATUS(stp)) {
            DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
                "dce_db_store_by_uuid failed, status (%d)\n", *stp));
            SET_STATUS(stp, sec_login_s_internal_error);
	    sec_login_db_unlock_and_close(&dbh, &lst);
            return;
        }
    }

    sec_login_db_unlock_and_close(&dbh, stp);
}

/*
 * This routine fetches an array of sec_login_db_entry_t.
 *
 * Input:
 *	datafile : pathname of ".db" file
 *	num_db_entries : number of entries requested
 *	db_type : an array of the types of entries requested
 * Output:
 *	db_entry : an array of fetched entries
 *	num_fetched : the number of entries fetched
 *	num_not_fetched : the number of entries that the caller
 *		requested which were not fetched
 *	not_fetched : an array of the types of entries which
 *		were not fetched
 */
PUBLIC void sec_login_db_fetch_entries(
    char			*datafile,		/* [in]  */
    unsigned32			num_db_entries,		/* [in]  */
    sec_login_db_enum_t		db_type[],		/* [in]  */
    sec_login_db_entry_t        **db_entry,      	/* [out] */
    unsigned32                  *num_fetched,   	/* [out] */
    unsigned32			*num_not_fetched,	/* [out] */
    sec_login_db_enum_t		**not_fetched, 	 	/* [out] */
    error_status_t              *stp            	/* [out] */
)
{
    int				i;
    dce_db_handle_t		dbh;
    error_status_t		lst;
    int				nf = 0;
    sec_login_db_entry_t	*tmp_db_entry;
    sec_login_db_enum_t		*tmp_not_fetched;

    CLEAR_STATUS(stp);
    *db_entry = NULL;
    *not_fetched = NULL;
    *num_fetched = 0;
    *num_not_fetched = num_db_entries;

    tmp_not_fetched = (sec_login_db_enum_t *)
                  malloc(sizeof(sec_login_db_enum_t) * num_db_entries);
    if (!tmp_not_fetched) {
        SET_STATUS(stp, sec_login_s_no_memory);
        return;
    }
    memset(tmp_not_fetched, 0, num_db_entries * sizeof(sec_login_db_enum_t));

    sec_login_db_open_and_lock(datafile,
	                       (db_c_index_by_uuid | db_c_readonly),
                	       &dbh, stp);
    if (STATUS_EQUAL(stp, db_s_open_failed_enoent)) {
        /*
         * Well, it doesn't exist. This means a DCE 1.0 library
         * created the login context and we ended up inheriting it
         * without any DCE 1.1 information. This means we should
         * ignore this status and return. When the login context
         * is recreated, it will fill the fields in with appropriate
         * values.
         */
        CLEAR_STATUS(stp);
        DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
            "sec_login_db_fetch_entries: filename (%s) does not exist\n",
            datafile));
        for (i = 0; i < num_db_entries; i++)
            tmp_not_fetched[i] = db_type[i];
        *not_fetched = tmp_not_fetched;
        return;
    }
    if (BAD_STATUS(stp)) {
        DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
            "sec_login_db_open_and_lock failed: filename = %s, status = %d\n", datafile, *stp));
        SET_STATUS(stp, sec_login_s_internal_error);
        if (tmp_not_fetched)
            free(tmp_not_fetched);
        return;
    }

    tmp_db_entry = (sec_login_db_entry_t *)
                  malloc(sizeof(sec_login_db_entry_t) * num_db_entries);
    if (!tmp_db_entry) {
        SET_STATUS(stp, sec_login_s_no_memory);
        sec_login_db_unlock_and_close(&dbh, &lst);
        if (tmp_not_fetched)
            free(tmp_not_fetched);
        return;
    }
    memset(tmp_db_entry, 0, num_db_entries * sizeof(sec_login_db_entry_t));

    for (i = 0; i < num_db_entries; i++) {

        dce_db_fetch_by_uuid(dbh,
                             &sec_db_uuid[db_type[i]],
                             &tmp_db_entry[*num_fetched],
                             stp);
        if (STATUS_EQUAL(stp, db_s_key_not_found)) {
            /*
             * This means an entry was not found for the key.
             * This should not be fatal because we may be
             * dealing with a file that does not contain this
             * uuid. (Highly unlikely in this case because this
             * is the first time we are using this file).
             * When the login context is recreated, it will fill 
             * the fields in with appropriate values.
             */
             CLEAR_STATUS(stp);
             DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
                 "sec_login_db_fetch_entries: key (%d) lookup failed\n", i));
             tmp_not_fetched[nf++] = db_type[i];
        } else if (BAD_STATUS(stp)) {
             DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
                 "dce_db_fetch_by_uuid failed, status (%d)\n", *stp));
             SET_STATUS(stp, sec_login_s_internal_error);
	     sec_login_db_unlock_and_close(&dbh, &lst);
             goto clean_fetch_entries;
        } else {
            (*num_fetched)++;
            (*num_not_fetched)--;
        }
    }

    sec_login_db_unlock_and_close(&dbh, stp);
    if (BAD_STATUS(stp))
	goto clean_fetch_entries;

    *db_entry = tmp_db_entry;
    *not_fetched = tmp_not_fetched;
    return;

clean_fetch_entries:
    sec_login_db_free_entries(rpc_ss_client_free, tmp_db_entry, num_db_entries);
    if (tmp_not_fetched)
        free(tmp_not_fetched);
}

/*
 * This routine fetches a sec_login_db_entry_t.
 *
 * Input:
 *      datafile : pathname of ".db" file
 * Output:
 *      db_entry : an array of fetched entries
 * Assumptions:
 *	db_entry->entry_type is set by the caller to the
 *	requested entry type to be fetched.
 */
PUBLIC void sec_login_db_fetch_entry(
    char			*datafile,	/* [in]  */
    sec_login_db_entry_t	*db_entry,	/* [out] */
    error_status_t		*stp		/* [out] */
)
{
    dce_db_handle_t		dbh;
    error_status_t		lst;

    CLEAR_STATUS(stp);

    sec_login_db_open_and_lock(datafile,
                               (db_c_index_by_uuid | db_c_readonly),
                               &dbh, stp);
    if (STATUS_EQUAL(stp, db_s_open_failed_enoent)) {
        /*
         * Well, it doesn't exist. This means a DCE 1.0 library
         * created the login context and we ended up inheriting it
         * without any DCE 1.1 information. Since we don't always
         * know how the caller wants the data initialized, we
         * return db_s_open_failed_enoent and leave it up to the
         * caller to initialize.
         */
        DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
            "sec_login_db_fetch_entry: filename (%s) does not exist\n",
            datafile));
        return;
    }
    if (BAD_STATUS(stp)) {
        DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
            "sec_login_db_open_and_lock failed: filename = %s, status = %d\n", datafile, *stp));
        SET_STATUS(stp, sec_login_s_internal_error);
        return;
    }

    sec_login_db_handle_fetch_entry(dbh, db_entry, stp);
    if (BAD_STATUS(stp)) {
        sec_login_db_unlock_and_close(&dbh, &lst);
        return;
    }
    sec_login_db_unlock_and_close(&dbh, stp);
    if (BAD_STATUS(stp))
        sec_login_db_free_entry_contents(rpc_ss_client_free, db_entry, -1);
}

/*
 * This routine fetches a sec_login_db_entry_t.
 *
 * Input:
 *      dbh : A handle to a ".db" file
 * Output:
 *      db_entry : an array of fetched entries
 * Assumptions:
 *      db_entry->entry_type is set by the caller to the
 *      requested entry type to be fetched.
 */
PUBLIC void sec_login_db_handle_fetch_entry(
    dce_db_handle_t		dbh,		/* [in]  */
    sec_login_db_entry_t	*db_entry,	/* [out] */
    error_status_t		*stp		/* [out] */
)
{
    CLEAR_STATUS(stp);

    dce_assert(sec_svc_handle, dbh != NULL);


    if (INVALID_ENTRY_TYPE(db_entry->entry_type)) {
        DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
            "Invalid cache database type: %d\n", db_entry->entry_type));
        SET_STATUS(stp, sec_login_s_internal_error);
        return;
    }

    dce_db_fetch_by_uuid(dbh,
                         &sec_db_uuid[db_entry->entry_type],
                         db_entry,
                         stp);
    if (BAD_STATUS(stp)) {
        if (STATUS_EQUAL(stp, db_s_key_not_found)) {
            /*
             * This means an entry was not found for the key.
             * This should not be fatal because we may be
             * dealing with a file that does not contain this
             * uuid. (Highly unlikely in this case because this
             * is the first time we are using this file).
             * Since we don't always know how the caller wants
             * the data initialized, we return key not found and
             * leave it up to the caller to initialize.
             */
            DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
                 "sec_login_db_handle_fetch_entry: key (%d) lookup failed\n",
                 db_entry->entry_type));
        } else {
            DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
                "dce_db_fetch_by_uuid failed, status (%d)\n", *stp));
            SET_STATUS(stp, sec_login_s_internal_error);
        }
        return;
    }
}

/*
 * This routine free the contents of a sec_login_db_entry_t.
 *
 * NOTE we do not free the pointer to db_entry. This routine's
 * purpose is to free the contents of db_entry.
 *
 * Sometimes, there is only one entry in the authdata array which has
 * been allocated by free, and all others have been allocated
 * by rpc_ss_client_free. This can happen if an array is fetched
 * from the db layer and changed and re-stored. The authdata_index parameter
 * contains the index of the array that *MUST* use free() to deallocate
 * its memory. authdata_index should be -1 if this does not apply.
 */
PUBLIC void sec_login_db_free_entry_contents(
    void			(*dealloc)(idl_void_p_t ptr),
    sec_login_db_entry_t	*db_entry,	/* [in/out] */
    unsigned32			authdata_index
)
{
    unsigned32			x;

    if (!db_entry)
        return;

    switch (db_entry->entry_type) {
        case generation_id_enum:
            /* nothing to free */
            break;
        case v1_1_info_enum:
            sec_encode_v1_1_lc_info_free(dealloc, &db_entry->contents.v1_1_info);
            break;
        case epac_enum:
            if (DB_V1_AUTHDATA(db_entry)) {
                for (x = 0; x < DB_V1_AUTHDATA(db_entry)->num_ad; x++) {
                    if (authdata_index == x)
                        sec_db_free_authdata_contents(free, 
                        	&DB_V1_AUTHDATA_ARRAY(db_entry, x));
                    else
                        sec_db_free_authdata_contents(dealloc, 
                        	&DB_V1_AUTHDATA_ARRAY(db_entry, x));
                }
                (*dealloc)(DB_V1_AUTHDATA(db_entry));
            }
            break;
        default:
            /*
             * should never get this, because we should have received
             * an internal error long before this.
             */
            break;
    }
}

/*
 * This routine frees the contents of an array of sec_login_db_entry_t
 * The caller must specify how many entries are in the array.
 */
PUBLIC void sec_login_db_free_entries(
    void			(*dealloc)(idl_void_p_t ptr),
    sec_login_db_entry_t	*db_entry,	/* [in/out] */
    unsigned32			num_db_entries
)
{
    sec_login_db_entry_t	*dbp, *dbi;

    if (!db_entry)
        return;

    dbi = ((sec_login_db_entry_t *) (((char *) &db_entry[0]) +
              (sizeof(sec_login_db_entry_t) * num_db_entries)));

    for (dbp = &db_entry[0]; dbp < dbi; dbp++) {
    
        sec_login_db_free_entry_contents(dealloc, dbp, -1);

    }
    free(db_entry);
}

/*
 * sec_login_db_search_and_store_entry
 *
 * IMPORTANT!:
 * 1) Currently only supports entries of type epac_enum.
 * 2) Assumes the entry is of type array.
 * 3) Caller is responsible for opening and exclusively 
 *    locking the db file beforehand by calling 
 *    sec_login_db_open_and_lock().
 * 4) Caller is responsible for unlocking and closing file
 *    by calling sec_login_db_unlock_and_close().
 *
 * This routine retrieves the requested entry array from a db file 
 * and calls a caller-provided search routine which searches for 
 * caller-provided entry in the retrieved array. If found, it  
 * replaces the fetched entry with the caller-provided entry,
 * and stores the new array. If not found, it appends the new 
 * data to the end of the array and stores the new array in the 
 * db file.
 */
PUBLIC void sec_login_db_search_and_store_entry(
    dce_db_handle_t		dbh,		/* [in]  */
    sec_login_db_enum_t         entry_type,     /* [in]  */
    void                        *entry,         /* [in]  */
    boolean32
        (*search) (void *, void *, unsigned32 *, error_status_t *),
                                                /* [in]  */
    error_status_t              *stp            /* [out] */
)
{
    boolean32			need_to_search = true;
    sec_login_db_entry_t	db_entry;
    unsigned32			index;
    error_status_t		lst;
    unsigned32			index_to_free = -1;
    
    CLEAR_STATUS(stp);

    dce_assert(sec_svc_handle, dbh != NULL);

    if (INVALID_ENTRY_TYPE(entry_type)) {
        DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
            "Invalid database type: %d\n", entry_type));
        SET_STATUS(stp, sec_login_s_internal_error);
        return;
    }

    memset(&db_entry, 0, sizeof(db_entry));

    dce_db_fetch_by_uuid(dbh,
                         &sec_db_uuid[entry_type],
                         &db_entry,
                         stp);
    if (STATUS_EQUAL(stp, db_s_key_not_found)) {
        need_to_search = false;
    } else if (BAD_STATUS(stp)) {
        DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
            "dce_db_fetch_by_uuid failed, status (%d)\n", *stp));
        SET_STATUS(stp, sec_login_s_internal_error);
        return;
    }

    switch (entry_type) {
        case epac_enum:
            if (!DB_V1_AUTHDATA(&db_entry))
                need_to_search = false;
            if (need_to_search) {
                sec_db_authdata_contents_t *db_authdata =
				 (sec_db_authdata_contents_t *) entry;
                if ((*search) ((void *) DB_V1_AUTHDATA(&db_entry),
                               (void *) db_authdata->cell, &index, stp)) {
                    sec_db_free_authdata_contents(rpc_ss_client_free, 
                                                  &DB_V1_AUTHDATA_ARRAY(&db_entry, index));
                    sec_db_copy_authdata_contents(&DB_V1_AUTHDATA_ARRAY(&db_entry, index), 
				entry, stp);
                    if (GOOD_STATUS(stp))
                        index_to_free = index;
		} else {
                    DB_V1_AUTHDATA(&db_entry) = (sec_db_authdata_t *) 
                                                 realloc(DB_V1_AUTHDATA(&db_entry),
                                                 sizeof(sec_db_authdata_t) +
                                                  (sizeof(sec_db_authdata_contents_t) *
                                                   DB_V1_AUTHDATA(&db_entry)->num_ad));
                    if (DB_V1_AUTHDATA(&db_entry) == NULL) {
                        SET_STATUS(stp, sec_login_s_no_memory);
                        sec_login_db_free_entry_contents(rpc_ss_client_free, 
                                                         &db_entry, index_to_free);
                        return;
                    }
                    /* 
                     * we increment after the realloc because it is a conformant
                     * IDL array and has an array size of 1 to begin with.
                     */
                    DB_V1_AUTHDATA(&db_entry)->num_ad++;
                    sec_db_copy_authdata_contents(&DB_V1_AUTHDATA_ARRAY(&db_entry,
                                DB_V1_AUTHDATA(&db_entry)->num_ad-1),
			        entry, stp);
                    if (GOOD_STATUS(stp))
                        index_to_free = DB_V1_AUTHDATA(&db_entry)->num_ad-1;
                }
            } else {
                DB_V1_AUTHDATA(&db_entry) = (sec_db_authdata_t *) malloc(sizeof(sec_db_authdata_t));
                if (DB_V1_AUTHDATA(&db_entry) == NULL) {
                    SET_STATUS(stp, sec_login_s_no_memory);
                    return;
                }
                db_entry.entry_type = epac_enum;
                DB_V1_AUTHDATA(&db_entry)->num_ad = 1;
                sec_db_copy_authdata_contents(&DB_V1_AUTHDATA_ARRAY(&db_entry, 0),
                                entry, stp);
            }
            break;
    }

    if (BAD_STATUS(stp)) {
        sec_login_db_free_entry_contents((need_to_search) ? rpc_ss_client_free : free, 
                                         &db_entry, index_to_free);
        return;
    }

    dce_db_store_by_uuid(dbh,
                         &sec_db_uuid[entry_type],
                         &db_entry,
                         stp);
    if (BAD_STATUS(stp)) {
        DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
            "dce_db_store_by_uuid failed, status (%d)\n", *stp));
        SET_STATUS(stp, sec_login_s_internal_error);
        sec_login_db_free_entry_contents((need_to_search) ? rpc_ss_client_free : free, 
                                         &db_entry, index_to_free);
        return;
    }

    sec_login_db_free_entry_contents((need_to_search) ? rpc_ss_client_free : free, 
                                     &db_entry, index_to_free);
}

/*
 * This routine searches an array of stored DCE v1.1 authorization
 * data entries, and returns TRUE if an entry is found for the
 * given cell.
 */
PUBLIC boolean32 sec_login_db_authdata_search(
    void			*v1_authdata,	/* [in]  */
    void			*cell,		/* [in]  */
    unsigned32			*index,		/* [out] */
    error_status_t		*stp		/* [out] */ 
)
{
    sec_db_authdata_t		*db_authdata = 
                                (sec_db_authdata_t *) v1_authdata;
    unsigned_char_t		*tmp_cell = (unsigned_char_t *) cell;
    unsigned32			x;

    if (!v1_authdata || !cell)
        return false;

    /* a linear search should be fine for now */
    for (x = 0; x < db_authdata->num_ad; x++) {
        if (u_strcmp(tmp_cell, db_authdata->ad[x].cell) == 0) {
            *index = x;
            return true;
        }
    }

    return false;
}

/*
 * sec_login_db_open_and_lock
 *
 * This routine opens a db file and locks it. It returns
 * a handle to the db file.
 *
 * The flags argument is a combination of bit values passed
 * as the "flags" argument to dce_db_open. If the flag
 * "dce_db_c_readonly" is specified, the file is shared locked.
 * Otherwise the file is exclusively locked.
 *
 * The caller is responsible for unlocking and closing the file.
 */
PUBLIC void sec_login_db_open_and_lock(
    char		*datafile,	/* [in]  */
    unsigned32		flags,		/* [in]  */
    dce_db_handle_t	*dbh,		/* [out] */
    error_status_t	*stp		/* [out] */
)
{
    CLEAR_STATUS(stp);
    *dbh = NULL;

    /* Lock is requested by specifying the db_c_lock flag */
    sec_login_db_open_file(datafile,
                           (flags | db_c_lock),
                           (S_IRUSR | S_IWUSR),
                           (dce_db_convert_func_t) sec_login_db_entry_encode,
                           dbh, stp);
    if (BAD_STATUS(stp)) {
        DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
            "sec_login_db_open_file failed: filename = %s, status = %d\n", datafile, *stp));
        return;
    }
}

    
/*
 * sec_login_db_unlock_and_close
 *
 * This routine unlocks a db file and closes it.
 * It assumes the file is locked.
 */
PUBLIC void sec_login_db_unlock_and_close(
    dce_db_handle_t		*dbh,		/* [in]  */
    error_status_t		*stp		/* [out] */
)
{
    CLEAR_STATUS(stp);

    /* 
     * unlock and close file - the file will be unlocked because
     * the db_c_lock flag was specified when opening.
     */
    dce_db_close(dbh, stp);
    if (BAD_STATUS(stp)) {
        /* abort on unlock failures */
	if (STATUS_EQUAL(stp, db_s_not_locked)) {
            dce_svc_printf(DCE_SVC(sec_svc_handle, 
	    	           "%s%s"),
                           sec_s_login,
                           svc_c_sev_fatal | svc_c_action_abort,
			   sec_sys_errno_text, "fcntl",
			   strerror(errno));
        } else {
            DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
                          "dce_db_close failed: status = %d\n", *stp));
        }
    }
}


/*
 * This routine frees the contents of a sec_db_authdata_contents_t
 */
PRIVATE void sec_db_free_authdata_contents(
    void				(*dealloc)(idl_void_p_t ptr),
    sec_db_authdata_contents_t		*ac
)
{
    if (!ac)
        return;
    if (ac->cell) 
	(*dealloc)(ac->cell);
    if (ac->authbytes.bytes)
        (*dealloc)(ac->authbytes.bytes);
    if (ac->epac_chain.bytes)
        (*dealloc)(ac->epac_chain.bytes);

    memset(ac, 0, sizeof(sec_db_authdata_contents_t));
}

/*
 * This routine copies the contents of from_ac to to_ac.
 */
PRIVATE void sec_db_copy_authdata_contents(
    sec_db_authdata_contents_t		*to_ac,    /* [out] */
    sec_db_authdata_contents_t		*from_ac,  /* [in]  */
    error_status_t			*stp       /* [out] */
)
{
    sec_db_authdata_contents_t		tmp_ac;

    CLEAR_STATUS(stp);

    memset(&tmp_ac, 0, sizeof(sec_db_authdata_contents_t));

    tmp_ac.endtime = from_ac->endtime;

    if (from_ac->cell) {
        tmp_ac.cell = (unsigned_char_t *) malloc(u_strlen(from_ac->cell)+1);
        if (tmp_ac.cell == NULL) {
            SET_STATUS(stp, sec_login_s_no_memory);
            return;
        }
        u_strcpy(tmp_ac.cell, from_ac->cell);
    } 

    if (from_ac->authbytes.num_bytes && from_ac->authbytes.bytes) {
        tmp_ac.authbytes.num_bytes = from_ac->authbytes.num_bytes;
        tmp_ac.authbytes.bytes = (byte *) malloc(from_ac->authbytes.num_bytes);
        if (tmp_ac.authbytes.bytes == NULL) {
            SET_STATUS(stp, sec_login_s_no_memory);
            if (tmp_ac.cell)
                free(tmp_ac.cell);
            return;
        }
        memcpy(tmp_ac.authbytes.bytes, from_ac->authbytes.bytes,
               from_ac->authbytes.num_bytes);
    }

    if (from_ac->epac_chain.num_bytes && from_ac->epac_chain.bytes) {
        tmp_ac.epac_chain.num_bytes = from_ac->epac_chain.num_bytes;
        tmp_ac.epac_chain.bytes = (byte *) malloc(from_ac->epac_chain.num_bytes);
        if (tmp_ac.epac_chain.bytes == NULL) {
            SET_STATUS(stp, sec_login_s_no_memory);
            if (tmp_ac.cell)
                free(tmp_ac.cell);
            if (tmp_ac.authbytes.bytes)
                free(tmp_ac.authbytes.bytes);
            return;
        }
        memcpy(tmp_ac.epac_chain.bytes, from_ac->epac_chain.bytes, 
               from_ac->epac_chain.num_bytes);
    }

    *to_ac = tmp_ac;
}

PRIVATE void sec_login_db_open_file(
    char 			*datafile, 
    unsigned32 			flags,
    int				mode,
    dce_db_convert_func_t 	convert,
    dce_db_handle_t     	*dbh,
    error_status_t      	*stp
)
{
    BTREEINFO                   btreeinfo;
    struct stat			db_stat;
    boolean32			bad_permissions = false;
    error_status_t		lst;

    CLEAR_STATUS(stp);

    /* set access method specific parameters */
    memset(&btreeinfo, 0, sizeof(BTREEINFO));
    btreeinfo.psize = DB_BTREE_PAGE_SIZE;

    dce_db_open_file(datafile, DB_BACKEND_TYPE, flags, mode,
		     (flags & db_c_create) ? (void *)&btreeinfo : NULL,
                     convert, dbh, stp);

    /*
     * If file was not opened readonly, check that
     * permissions are read/write user only.
     */
    if (!(flags & db_c_readonly) && GOOD_STATUS(stp)) {
        dce_db_stat(*dbh, &db_stat, stp);
        if (GOOD_STATUS(stp) && 
	    PERMISSIONS(db_stat.st_mode) != WANTED_PERMISSIONS) {
	    dce_db_close(dbh, &lst);
	    (void) unlink(datafile);
	    bad_permissions = true;
        } else if (BAD_STATUS(stp)) {
            dce_db_close(dbh, &lst);
            return;
    	}
    }

    /* 
     * If file was not opened readonly and create,
     * reopen with create flag if file does not exist or
     * previous file had bad permissions.
     */
    if (!(flags & db_c_readonly) && 
        !(flags & db_c_create) &&
        (STATUS_EQUAL(stp, db_s_open_failed_enoent) ||
         bad_permissions)) {

        CLEAR_STATUS(stp);
	dce_db_open_file(datafile, DB_BACKEND_TYPE, (flags | db_c_create),
			 mode, (void *)&btreeinfo, convert, dbh, stp);
    }
}
