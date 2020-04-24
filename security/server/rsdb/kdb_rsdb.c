/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: kdb_rsdb.c,v $
 * Revision 1.1.17.5  1996/11/13  18:11:30  arvind
 * 	CHFts20690..93: status code translation...
 * 	[1996/11/07  21:04 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_5/1]
 *
 * 	Remove misleading comment.
 * 	[1996/11/05  02:15 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_4/1]
 *
 * 	Fix server side of Third-Party preauth
 * 	[1996/10/15  17:30 UTC  aha  /main/DCE_1.2.2/aha_pk10/1]
 *
 * 	CHFts20163: Merge public key version support changes into
 * 	krb5_kdc_pk_client_key_retriever()
 * 	[1996/09/30  14:15 UTC  aha  /main/DCE_1.2.2/aha_pk9_5/1]
 *
 * 	Add case for public key to pre_auth_req
 * 	[1996/09/27  14:10 UTC  aha  /main/DCE_1.2.2/aha_pk9_4/1]
 *
 * 	Merge-up from sommerfeld_pk_kdc_1
 * 	[1996/09/16  23:18 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 *
 * 	Add support for retrieving user private keys.
 * 	[1996/06/11  17:57 UTC  aha  /main/DCE_1.2.2/2]
 *
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:51 UTC  psn  /main/DCE_1.2.2/DCE122_PK/1]
 *
 * Revision 1.1.17.4  1996/10/15  20:56:25  arvind
 * 	Fix alloc/free of public key data.
 * 	[1996/10/11  00:06 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/2]
 * 
 * 	From AHA: fix for CHFts20153/CHFts20163
 * 	[1996/10/04  22:04 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/1]
 * 
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:51 UTC  psn  /main/DCE_1.2.2/DCE122_PK/1]
 * 
 * Revision 1.1.17.3  1996/10/03  15:11:55  arvind
 * 	Add support for retrieving user private keys.
 * 	[1996/09/16  23:18 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 * 
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:51 UTC  psn  /main/DCE_1.2.2/DCE122_PK/1]
 * 
 * Revision 1.1.17.2  1996/07/08  18:38:38  arvind
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:51 UTC  psn  /main/DCE_1.2.2/DCE122_PK/1]
 * 
 * 	bugfix: good_since_date bogusly clobbered.
 * 	fix call to lm_get_login_info, and make sure it had a prototype in
 * 	scope..
 * 	[1996/05/08  19:43 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts17874/2]
 * 
 * 	Layering cleanup:  move OSF-DCE specific coding down here instead
 * 	of leaving it up in the KDC where it will impede merges.
 * 	[1996/04/26  22:52 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts17874/1]
 * 
 * 	[1994/12/15  03:27 UTC  rps  /main/HPDCE02/1]
 * 
 * 	[OT12669] cell renaming
 * 	[1994/12/15  00:35 UTC  rps  /main/rps_mothra/1]
 * 
 * Revision 1.1.17.1  1996/06/04  22:04:13  arvind
 * 	Merge u2u: If sec_rgy_acct_auth_user_to_user set on acct, then set
 * 	KRB5_KDB_REQUIRES_USER_TO_USER flag on the KRB entry returned.
 * 	[1996/05/06  21:31 UTC  burati  /main/DCE_1.2/2]
 * 
 * 	merge u2u work
 * 	[1996/04/30  15:09 UTC  burati  /main/DCE_1.2/mb_u2u/1]
 * 
 * 	u2u work off Mothra, before 1.2 branch is available
 * 	[1996/04/30  15:07 UTC  burati  /main/HPDCE02/mb_mothra8/1]
 * 
 * Revision 1.1.15.3  1996/02/18  00:20:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:08:38  marty]
 * 
 * Revision 1.1.15.2  1995/12/08  17:58:02  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/4  1995/05/15  21:10 UTC  greg
 * 	Mergeout from HPDCE02 to SEC_migrate_merge.
 * 	[1995/05/13  02:41 UTC  greg  /main/HPDCE02/SEC_migrate_merge/2]
 * 
 * 	Resolve CHFts14998
 * 	[1995/05/03  18:51 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_4/1]
 * 
 * 	Ceate SEC_migrate_merge mainline from SEC_migrate and HPDCE02
 * 	[1995/04/21  12:57 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
 * 
 * 	HP revision /main/HPDCE02/3  1995/05/03  19:10 UTC  mdf
 * 	Resolve CHFts14998
 * 	[1995/05/03  18:51 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_4/1]
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/01  18:59 UTC  mdf
 * 	"Resolve CHFts14998"
 * 	[1995/04/26  12:08 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_3/1]
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/15  03:27 UTC  rps
 * 
 * 	HP revision /main/rps_mothra/1  1994/12/15  00:35 UTC  rps
 * 	[OT12669] cell renaming
 * 	[1995/12/08  17:21:54  root]
 * 
 * Revision 1.1.9.5  1994/10/05  18:03:30  sekhar
 * 	[ OT 12466 ] Check return status from krb5_copy_principal() .
 * 	[1994/10/05  15:01:48  sekhar]
 * 
 * Revision 1.1.9.4  1994/08/29  15:51:49  hondo
 * 	OT 11919 - login activity idl change to support different address types
 * 	[1994/08/28  21:41:51  hondo]
 * 
 * Revision 1.1.9.3  1994/08/24  20:08:32  mullan_s
 * 	[OT11556] Update DCE preauth types to match Kerberos spec.
 * 	[1994/08/24  20:07:36  mullan_s]
 * 
 * Revision 1.1.9.2  1994/07/15  15:02:30  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:22:27  mdf]
 * 
 * Revision 1.1.9.1  1994/05/11  19:32:10  ahop
 * 	hp_sec_to_osf_2 drop
 * 	Update code to use new krb5_db_entry struct.
 * 	changes to pre_authentication for ERAs
 * 	[1994/04/29  21:47:22  ahop]
 * 
 * Revision 1.1.6.4  1992/12/29  16:38:07  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:27:40  zeliff]
 * 
 * Revision 1.1.6.3  1992/10/07  20:37:42  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:19:57  sekhar]
 * 
 * Revision 1.1.7.2  1992/10/06  16:01:27  burati
 * 	Second replication drop: bl5
 * 	remove READ_LOCK(lock_db,...).  Locks are now taken at entry points.
 * 
 * Revision 1.1.6.2  1992/09/29  20:13:32  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:38:53  sekhar]
 * 
 * Revision 1.1.2.7  1992/07/13  21:21:54  sommerfeld
 * 	[CR4513] reject null string as principal name (avoids confusion).
 * 	[1992/07/09  20:08:00  sommerfeld]
 * 
 * Revision 1.1.2.5  1992/05/29  15:01:12  sommerfeld
 * 	[CR2559] Don't make failure to get last_changer name fatal.
 * 	[1992/05/28  22:53:02  sommerfeld]
 * 
 * Revision 1.1.2.4  1992/05/28  20:59:31  sommerfeld
 * 	[CR3650] Move macros around so they can be used in more of the code.
 * 	If effective policy sez max lifetime is zero, set it to infinite.
 * 	[1992/05/27  22:29:18  sommerfeld]
 * 
 * Revision 1.1.2.3  1992/05/15  21:25:31  sommerfeld
 * 	Fix typo in last fix which set the default life equal to the
 * 	minimum life.
 * 	[1992/05/15  21:14:35  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/05/13  22:08:52  sommerfeld
 * 	Enforce ordering of policy timeouts.
 * 	[1992/05/13  18:29:37  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  14:44:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  
** Copyright (c) Hewlett-Packard Company 1991, 1992, 1994
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 * 
 *
 * Derived from MIT file kdb_dbm.c:
 * Copyright 1988,1989,1990 by the Massachusetts Institute of Technology. 
 * For copying and distribution information, please see the file
 * <krb5/copyright.h>.    
 */

#if !defined(lint) && !defined(SABER)
static char *VersionID = "@(#)kdb_rsdb.c	25 - 12/05/91";
static char rcsid_krb_dbm_c[] =
"$Id: kdb_rsdb.c,v 1.1.17.5 1996/11/13 18:11:30 arvind Exp $";
#endif  /* lint */

#include <krb5/copyright.h>
#include <krb5/krb5.h>
#include <krb5/kdb.h>
#include <krb5/kdb_dbm.h>
#include <krb5/ext-proto.h>

#include <dce/rgynbase.h>

#include <rs_util.h>
#include <rsdb_pvt.h>
#include <rsdb_jrnl.h>
#include <rsdb_login.h>
#include <rsdb_util.h>
#include <rsdb_acct.h>
#include <rsdb_auth.h>
#include <rsdb_policy.h>
#include <rsdb_pgo.h>
#include <rsdb_name_util.h>
#include <lm_login.h>
#include <rgymacro.h>
#include <bstring.h>

#include <rsdb_attr.h>
#include <sec_attr_tools.h>

#include <rs_ver.h>
#include <rs_ver_pvt.h>

#include <dce/passwd.h>
#include <sec_encode.h>
#include <dce/sec_pk.h>
                      
#ifndef	min
#define	min(a, b)	((a) < (b) ? (a) : (b))
#endif

#ifndef max
#define	max(a, b)	((a) > (b) ? (a) : (b))
#endif

#define zeroisinf(a)	((a) == 0? KRB5_INT32_MAX:(a))

enum pre_auth{
        none,
        timestamp,
        third_party,
        public_key
};

enum pwd_override{
        no_override,
        override
};

PUBLIC krb5_error_code princ_to_login_name(
#ifdef __STDC__
    krb5_principal          principal,
    sec_rgy_login_name_t    *login_name
#endif
);


PRIVATE void free_build_princ_entry_contents(
#ifdef __STDC__
    krb5_db_entry           *entry
#endif
);



/*
 * This module contains all of the code which directly interfaces to
 * the registry database.
 */


/*     
 * Return error in routines that shouldn't be called.
 * Do nothing in harmless routines likely to be called.
 */

static krb5_data local_realm;

krb5_error_code
krb5_dbm_db_init(void)
{
    error_status_t st;
    st = krb5_get_default_realm(&local_realm.data);
    if (st == 0) {
	local_realm.length = strlen(local_realm.data);
    }
    return st;
}


krb5_error_code
krb5_dbm_db_fini(void)
{
    return 0;
}

krb5_dbm_db_create (char *  ignored)
{
    return -1;
}

krb5_dbm_db_rename (char *  ignored1, char *  ignored)
{
    return -1;
}

krb5_dbm_db_set_name (char *  ignored)
{
    return 0;			/* used by caller */
}

krb5_dbm_db_get_age
  (
    char *  ignored,
    time_t *  ignored1
  )
{
    return 0;
}

krb5_dbm_db_put_principal
  (
    krb5_db_entry   *ignored,
    int * ignored1
  )
{
    return -1;
}

krb5_dbm_db_delete_principal
  (
    krb5_principal  ignored,
    int * ignored1
  )
{
    return -1;
}
           
/*
 * Function to transpose server name and realm name - used for cross-realm
 * ticket requests.
 */
PRIVATE void swap_foreign_tgs_name(krb5_principal principal)
{
    krb5_data   *t;

    t = principal[2];
    principal[2] = krb5_princ_realm(principal);
    krb5_princ_realm(principal) = t;
}

/*
 * princ_to_login_name: converts a krb5_principal to a sec_rgy_login_name_t.
 * This maps from the Kerberos namespace into the DCE namespace.
 */
PUBLIC krb5_error_code princ_to_login_name
  (
    krb5_principal          principal,
    sec_rgy_login_name_t    *login_name
  )
{   
    krb5_error_code     retval;            
    char                *name;
    char                *p;
    signed32            pname_len;
    int swap = 0;
    
    /*
     * Realm sanity checks:
     *  - Either the realm we are given is an acceptable alias, or
     *  - this is an interrealm principal (otherrealm, "krbtgt", localrealm)
     */

    if ((!krb5_realm_equiv(krb5_princ_realm(principal), &local_realm))) {
	if (!(krb5_is_tgs_principal(principal)) ||
	    !(krb5_realm_equiv(krb5_princ_component(principal, 1),
			       &local_realm)))
	    return KRB5_KDB_NOENTRY;

	/* definitely a foreign TGS entry.. swap.. */
	swap = 1;
	swap_foreign_tgs_name(principal);
    }
    
    /*
     * Name is allocated by unparse_name and must be freed when done.
     * Name will be of the format person_name@realm.
     */
    retval = krb5_unparse_name(principal, &name);
    
    if (swap)
	/* ... and swap back */
	swap_foreign_tgs_name(principal);
    
    if (retval)
        return(retval);
    
    if ((p = strchr(name, '@')) == NULL) 
	retval = KRB5_PARSE_MALFORMED;
    else {
        int i, j;
        
        pname_len = p - name;
        strncpy((char *)(login_name->pname), name, pname_len);
        /*
         * !!! replace "\/" with  "/".
         */
        for (i=0; i < pname_len; i++)
        {
            if ((login_name->pname[i] == '\\') &&
                (login_name->pname[i+1] == '/'))
            {
                for (j=i; j<pname_len; j++) {
                    login_name->pname[j] = login_name->pname[j+1];
                }
                pname_len--;
            }
        }
        
        login_name->pname[pname_len] = '\0';
        login_name->gname[0]         = '\0';
        login_name->oname[0]         = '\0';
        retval = 0;
        rsdb_name_canonicalize(login_name->pname, (error_status_t *)&retval);
    }
    free(name);
    return retval;
}



/*
 * build_princ_entry: Stuff the fields in entry with information from
 * account, and auth_policy.  Contruct a krb5_principal from the realm
 * and pname (principal name). 
 *
 * If this call returns successfully, the caller will have to free
 * principal, mod_name, and key.contents.  This is usually done in
 * krb5_dbm_db_free_principal called by the user of the kdb_rsdb 
 * interface.  If the call returns unsuccessfully, no cleanup is required.
 */

PRIVATE krb5_error_code build_princ_entry
  (
    krb5_principal          princ,          /* [in]      */
    sec_rgy_login_name_t    *login_name,          /* [in]      */
    sec_rgy_acct_user_t     *user_part,     /* [in]      */
    sec_rgy_acct_admin_t    *admin_part,    /* [in]      */
    sec_rgy_plcy_auth_t     *auth_policy,   /* [in]      */
    sec_rgy_plcy_t          *policy,        /* [in]      */
    krb5_keytype            key_type,       /* [in]      */
    krb5_kvno               key_version,    /* [in]      */
    krb5_db_entry           *entry          /* [in, out] */
  )
{
    krb5_principal          mod_princ; 
    char                    princ_name[(2 * sec_rgy_name_t_size) + 1];
    sec_rgy_name_t          *p;   
    krb5_error_code         retval; 
    rsdb_credentials_t      credentials;
    long                    name_len;              
    error_status_t          status, ignore_st;
    sec_rgy_login_activity_t login_activity;
    sec_rgy_cursor_t         cursor;
    uuid_t                  *last_changer_uuid;
    int                     tlen;
    krb5_encrypted_keyblock *key;
    krb5_data               pepper;
    boolean32               default_pepper;
    char *		    realm;
    rsdb_util_rgy_obj_info_t obj_info;       /* pre-auth check code */
    sec_attr_t               sec_attr;       /* pre-auth check code */
    sec_attr_t               override_attr;  /* pre-auth check code */
    sec_rgy_name_t           fullname;       /* pre-auth check code */
    uuid_t                   obj_uuid;       /* pre-auth check code */
    lm_upd_action_t          action;
    sec_timeval_sec_t        curtime;

    sec_rgy_acct_auth_flags_t  auth_flags = admin_part->authentication_flags;

    memset(&sec_attr,0,sizeof(sec_attr_t));
    memset(&action,0,sizeof(lm_upd_action_t));
    memset(&login_activity,0,sizeof(sec_rgy_login_activity_t));
               
    status = krb5_copy_principal(princ, &entry->principal); /* freed in krb5_dbm_db_free_principal */
    if (BAD_STATUS(&status))
        return status;

    last_changer_uuid = &admin_part->last_changer.principal;
    
    /* if last_changer_uuid is nil, say "rgy_create" */
    if (uuid_is_nil (last_changer_uuid, &status)) {
        strcpy (princ_name, "rgy_creator");
    } else {
	/* Prepare mod_name and put in entry. 
	 * Get last_changer UUID and lookup its name.
	 */ 
	if(!rsdb_get_credentials_by_uid(domain_info[sec_rgy_domain_person].db,
					last_changer_uuid, &credentials))
	{
	    strcpy(princ_name, "<deleted account>");
	} else 
	{
	    p = (sec_rgy_name_t *)princ_name;
	    rsdb_util_path_construct(sec_rgy_domain_person, credentials.id,
		*p, &name_len, &status);  
	    if(BAD_STATUS(&status)) {
		return status;
	    }
	}
    }
       
    strcat(princ_name, "@");
    tlen = strlen(princ_name);
    strncat(princ_name, princ[0]->data, princ[0]->length);
    /*
     * Null terminate the princ_name
     */
    princ_name[tlen+princ[0]->length] = '\0';
    if(retval = krb5_parse_name(princ_name, &mod_princ)) {
        return(retval);
    }
    entry->mod_name = mod_princ;/* freed in krb5_dbm_db_free_principal */
    
    /* Allocate for key and put in entry.
     */
    /* need to get the "pepper" from the security database and return
     * as the PW_SALT in the AS_REP pa-data.  If default pepper used, return 
     * KRB5_KDB_SALTTYPE_NORMAL in the salt_type field causing the  client
     * side to construct the salt using krb5_principal_to_salt
     */
    
    /* initialize to NULL key.  fill in if the acct has a key. */
    entry->key.keytype = 0;
    entry->key.length = 0;
    entry->key.contents = 0;
    
    /* called with lock, so don't need to lock here */
    rsdb_acct_get_auth_key_by_name(login_name->pname, princ_name+tlen,
                                   key_type, key_version,
				   (krb5_keyblock **) &key, 
                                   &default_pepper, &pepper, 
                                   &(entry->mkvno), &status);
    if (GOOD_STATUS(&status)) { 
	entry->key =  *key;
  	xfree(key);
        if(default_pepper) {
            entry->salt_type = KRB5_KDB_SALTTYPE_NORMAL;
            entry->salt_length = 0;
            entry->salt = NULL;
        } else {
            entry->salt_type = KRB5_KDB_SALTTYPE_SPECIAL;
            entry->salt_length = pepper.length;
            entry->salt = (krb5_octet *) pepper.data;
            pepper.data = NULL;
        }
    } else {
	if (status == sec_rgy_key_bad_type)
	    status = KRB5KDC_ERR_ETYPE_NOSUPP;
	krb5_free_principal(mod_princ);
	return status;
    }
    
    /* Copy remaining fields into entry.
     */
    if (key_version == 0) {  
         /* current version number */
         entry->kvno = user_part->passwd_version_number;
    } else {
        /* got whatever version number they asked for */
	entry->kvno = key_version;
    }

    entry->max_life           = zeroisinf((krb5_deltat)auth_policy->max_ticket_lifetime);
    entry->max_renewable_life = zeroisinf((krb5_deltat)auth_policy->max_renewable_lifetime);

    if (policy->acct_lifespan == 0 /* live forever */) {
        entry->expiration = (krb5_timestamp)admin_part->expiration_date;
    } else if (admin_part->expiration_date == 0  /* never expire */) {
        entry->expiration = (krb5_timestamp)admin_part->creation_date 
                                            + policy->acct_lifespan;
    } else {
        entry->expiration = (krb5_timestamp)MIN(admin_part->expiration_date, 
                                                (admin_part->creation_date 
                                                + policy->acct_lifespan));
    }                                                                    
    if (entry->expiration == 0) {
        entry->expiration = KRB5_INT32_MAX;
    }
                                          
        

    if (policy->passwd_lifetime == 0) {
        entry->pw_expiration = (policy->passwd_exp_date > user_part->passwd_dtm) 
                            ? (krb5_timestamp)policy->passwd_exp_date : 0;
    } else if (policy->passwd_exp_date == 0) {
        entry->pw_expiration = (krb5_timestamp)user_part->passwd_dtm
                                            + policy->passwd_lifetime;
    } else {
        sec_timeval_sec_t  exp;

        exp = (policy->passwd_exp_date > user_part->passwd_dtm) 
               ? policy->passwd_exp_date : 0;
        if (exp == 0) {      
            entry->pw_expiration = (krb5_timestamp)user_part->passwd_dtm
                                               + policy->passwd_lifetime;  
        } else {
  
            entry->pw_expiration = (krb5_timestamp)MIN(policy->passwd_exp_date, 
                                                    (user_part->passwd_dtm
                                                    + policy->passwd_lifetime));
        }
    }
    if (entry->pw_expiration == 0) {
        entry->pw_expiration = KRB5_INT32_MAX;
    }

    entry->mod_date   = (krb5_timestamp)admin_part->change_date;
    entry->good_since_date = (krb5_timestamp)admin_part->good_since_date;


    entry->attributes = 0;
    if (!FLAG_SET(auth_flags, sec_rgy_acct_auth_forwardable)) {
        SET(entry->attributes, KRB5_KDB_DISALLOW_FORWARDABLE);
    }
    if (!FLAG_SET(auth_flags, sec_rgy_acct_auth_post_dated)) {
        SET(entry->attributes, KRB5_KDB_DISALLOW_POSTDATED);
    }
    if (!FLAG_SET(auth_flags, sec_rgy_acct_auth_tgt)) {
        SET(entry->attributes, KRB5_KDB_DISALLOW_TGT_BASED);
    }
    if (!FLAG_SET(auth_flags, sec_rgy_acct_auth_renewable)) {
        SET(entry->attributes, KRB5_KDB_DISALLOW_RENEWABLE);
    }
    if (!FLAG_SET(auth_flags, sec_rgy_acct_auth_proxiable)) {
        SET(entry->attributes, KRB5_KDB_DISALLOW_PROXIABLE);
    }                                                     
    if (!FLAG_SET(auth_flags, sec_rgy_acct_auth_dup_skey)) {
        SET(entry->attributes, KRB5_KDB_DISALLOW_DUP_SKEY);
    }
    if (!FLAG_SET(admin_part->flags, sec_rgy_acct_admin_client)) {
        SET(entry->attributes, KRB5_KDB_DISALLOW_CLIENT_TIX);
    }
    if (!FLAG_SET(admin_part->flags, sec_rgy_acct_admin_server)) {
        SET(entry->attributes, KRB5_KDB_DISALLOW_SERVER_TIX);
    }
    if (!FLAG_SET(admin_part->flags, sec_rgy_acct_admin_valid)) {
        SET(entry->attributes, KRB5_KDB_DCE_ACCT_NOT_VALID);
    }
    if (FLAG_SET(auth_flags, sec_rgy_acct_auth_user_to_user)) {
        SET(entry->attributes, KRB5_KDB_REQUIRES_USER_TO_USER);
    }

    /*
     * Initialize login activity-related info.
     * By setting max_attempts to 0, we ensure that,
     * in the absence of an invalid login ERA indicating
     * otherwise, the validate_as_request validation
     * code will not engage in any login activity
     * tracking.
     */
    entry->max_attempts = 0;
    entry->disable = 0;

    rsdb_name_util_complete_name(sec_rgy_domain_person, login_name->pname, fullname, &status);
    if (STATUS_OK(&status)) {
	rsdb_util_get_obj_info_by_name(fullname, rsdb_pgo_type,
				       sec_acl_type_object, &obj_info, 
				       &obj_uuid, &status);
    }
    if (BAD_STATUS(&status)) {
	return KDC_ERR_C_PRINCIPAL_UNKNOWN;
    }

    /*
     * If the passwd expiration feature is suported by the registry 
     * server, we will subsequently enable or disable  password expiration 
     * override for this principal according to the value, if any, of the
     * passwd_override attribute instance attached to this principal.  If
     * no such attritubute instance is attached to this principal, then
     * passwd expiration will be enforced.
     * Otherwise, assume passwd expiration is overridden for this principal. 
     */
    if (rs_sw_feature_test_support(rs_c_sw_feature_passwd_exp, &ignore_st)) {
	signed32                 override_val;   /* pre-auth check code */
	/*  
	 *  NULL is passed to the rsdb_attr functions because the value
	 *  returned is an integer and the rsdb_attr functions only
	 *  allocate space if the value of the ERA is a pointer 
	 */
	rsdb_attr_lookup_by_name(&obj_info, "passwd_override", NULL, 
				 &override_attr, &status);
	if (BAD_STATUS(&status)) {
	    override_val = (signed32) no_override;
	    CLEAR_STATUS(&status);
	    
	} else {
	    override_val = SA_ATTR_INTEGER(&override_attr);
	}
	if (override_val != no_override)
	    entry->pw_expiration = 0;	    
    } else {
	entry->pw_expiration = 0;
    }
    

    /*   The kdc now needs to check for the
     *   existence of an instance of the ERA  "pre_auth_req"
     *   and if it is found, a flag is set so that
     *   functions called by process_as_req() have access to
     *   to the information
     *   Only the value "third_party" is important.
     *   We only disallow login requests if the ERA is more
     *   restrictive than the attempt.
     */

    /*  
     *  NULL is passed to the rsdb_attr functions because the value
     *  returned is an integer and the rsdb_attr functions only
     *  allocate space if the value of the ERA is a pointer 
     */
    rsdb_attr_lookup_by_name(&obj_info, "pre_auth_req", NULL, &sec_attr, &status);

    if (GOOD_STATUS(&status)) {
        switch(SA_ATTR_INTEGER(&sec_attr)) {
            case third_party:
                SET(entry->attributes, KRB5_KDB_PADATA_OSF_DCE);
                break;
            case timestamp:
                SET(entry->attributes, KRB5_KDB_PADATA_UNIX_TIME);
                break;
            case public_key:
                SET(entry->attributes, KRB5_KDB_PADATA_ROOT_CERT);
            default:
                break;
        }
    }
    /*   The kdc now needs to check for login activity
     *       so we need to get the current registry info
     *       and load up the client entry with this
     *       so decisions can be made later in login processing
     */

     rs_util_clear_cursor(&cursor);
     lm_get_login_info(login_name,&cursor,&login_activity,&status);

     entry->last_success = login_activity.last_good_time;
     entry->last_failed = login_activity.last_bad_time;
     entry->fail_auth_count = login_activity.cur_inv_attempts;
     /*
      * Disabling an account only happens if you have a
      * max_invalid_attempts ERA value so......
      * Check this first then......
      * Check if the account is currently disabled
      *        and if it should be re-enabled
      * Re-enabling requires an update to the login_activity_database.
      */
     memset(&sec_attr,0,sizeof(sec_attr_t));
    /*  
     *  NULL is passed to the rsdb_attr functions because the value
     *  returned is an integer and the rsdb_attr functions only
     *  allocate space if the value of the ERA is a pointer 
     */
     rsdb_attr_lookup_by_name(&obj_info, "max_invalid_attempts", NULL,
             &sec_attr, &status);
     if (GOOD_STATUS(&status)) {
             entry->max_attempts = SA_ATTR_INTEGER(&sec_attr);
             if (login_activity.dis_time){
                 rs_util_apply_timestamp(&curtime);
                 if (STILL_DISABLED(login_activity.dis_time,curtime))
                     entry->disable = login_activity.dis_time;
                 else{
                     entry->disable = 0;
                     action.type=lm_reset;
                     lm_update_login_info(entry->principal, &action, &status);
                 }
             }
             else
                 entry->disable = 0;
     }
     else{
        entry->max_attempts = 0;
        entry->disable = login_activity.dis_time;
     }
    return 0;
}


/*                           
 * krb5_dbm_db_get_principal:
 *
 * Look up principal information in the registry data base. 
 * Entries has been allocated by the caller.
 * Caller specifies number of krb5_db_entries allocated in nentries.
 * The number of entries found is returned in nentries.
 * More is set if more entries exist in the database than were
 * requested in the input nentries count.
 *
 * Current implementation never returns more than one entry;
 * thus, more is generally FALSE.
 *
 * Future implementation may allow wildcards in searchfor.
 */

PUBLIC krb5_error_code krb5_dbm_db_get_principal
  (
    krb5_principal searchfor,   /* [in]      */
    krb5_keytype key_type,      /* [in]      */
    krb5_kvno key_version,      /* [in]      */
    krb5_db_entry *entries,     /* [in, out] */
    int *nentries,              /* [in, out] */
    krb5_boolean *more          /* [out]     */
  )
{
    int                     found = 0;
    sec_rgy_properties_t    properties;
    sec_rgy_login_name_t    login_name;
    sec_rgy_plcy_auth_t     auth_policy;
    sec_rgy_plcy_t          policy;
    sec_rgy_cursor_t        cursor; 
    sec_rgy_acct_key_t      name_parts;
    sec_rgy_acct_admin_t    admin_part;
    sec_rgy_acct_user_t     user_part;
    sec_rgy_sid_t           sid;
    sec_rgy_unix_sid_t      unix_sid;
    uuid_t                  org_uuid;
    sec_rgy_name_t          org_name;
    error_status_t          status;
    error_status_t          lstatus;


    status = princ_to_login_name(searchfor, &login_name);

    if (status == KRB5_KDB_NOENTRY) {
	*more = FALSE;
	*nentries = 0;
	return 0;
    }
    
    if (BAD_STATUS(&status)) {
        return(status);
    }
    /*
     * Null string is not really there.
     */
    if (login_name.pname[0] == '\0') 
    {
	*more = FALSE;
	*nentries = 0;
	return 0;
    }

    rs_util_clear_cursor(&cursor);                 
    rsdb_acct_lookup(&login_name, &cursor, &sid, &unix_sid, &name_parts,
			 &user_part, &admin_part, &status);

    if(GOOD_STATUS(&status)) {
   	    rsdb_auth_policy_get_effective(&login_name, &auth_policy, &status);
    }
	
    /* need effective account policy */
    if (GOOD_STATUS(&status)) {
        rsdb_pgo_uid_to_name(sec_rgy_domain_org, &sid.org, org_name, &status);
        if (GOOD_STATUS(&status)) {
            rsdb_policy_get_effective(org_name, &policy, &status);
        }
    }     

    if (GOOD_STATUS(&status)) {
        /*MKEY - master key needs to be included  */  
        status = build_princ_entry(searchfor, &login_name, &user_part, 
				&admin_part, &auth_policy, 
				&policy, key_type, key_version, entries);
    }

    if (GOOD_STATUS(&status)) {
        /* In this implementation, no more than one entry is returned. */
        found = 1;
    }

    if (BAD_STATUS(&status)) {
        *more = FALSE;
        *nentries = 0;
        if (status == sec_rgy_object_not_found) {
            return 0;
        } else {
            return(status);
        }
    }                                         

    if(found > *nentries ) {
        *more = TRUE;
    } else {
        *more = FALSE;
    }
    *nentries = found;        

    return(0);
}

/*
  Free the "entries" memory allocated by krb5_dbm_db_get_principal.
 */
PUBLIC void krb5_dbm_db_free_principal
  (
    krb5_db_entry   *entries,
    int             nentries
  )
{
    register int i;
    for (i = 0; i < nentries; i++)
        free_build_princ_entry_contents(&entries[i]);
    return;
}

PRIVATE void free_build_princ_entry_contents
  (
    krb5_db_entry *entry
  )
{
    if (!entry)
        return;

    /* erase the key */
    if (entry->key.contents) {
        bzero((char *)entry->key.contents, entry->key.length);
        free((char *)entry->key.contents);
    }

    if (entry->principal) {
        krb5_free_principal(entry->principal);
    }

    if (entry->mod_name) {
        krb5_free_principal(entry->mod_name);
    }
    if (entry->salt) {
        bzero((char *)entry->salt, entry->salt_length);
        free((char *)entry->salt);
    }
    (void) bzero((char *)entry, sizeof(*entry));
    return;
}


/*                           
 * krb5_dbm_db_get_policy:
 *
 * Get realm-wide policy info
 */

PUBLIC krb5_error_code krb5_dbm_db_get_policy ( kdc_policy )
    krb5_db_policy  *kdc_policy;     
{
    sec_rgy_properties_t  properties; 
    sec_rgy_plcy_auth_t   auth_policy;
    sec_rgy_login_name_t  rgy; 
    error_status_t        status;
    krb5_timestamp min_life, max_life, max_renewable_life, default_life;

    rsdb_properties_get_info(&properties, &status);

    if (GOOD_STATUS(&status)) {   
        rgy.pname[0] = rgy.gname[0] = rgy.oname[0] = '\0';
        rsdb_auth_policy_get_info(&rgy, &auth_policy, &status);
    } 

    if (GOOD_STATUS(&status)) {
	max_life = zeroisinf(auth_policy.max_ticket_lifetime);
	max_renewable_life = zeroisinf(auth_policy.max_renewable_lifetime);
	default_life = zeroisinf(properties.default_certificate_lifetime);

        min_life = properties.minimum_ticket_lifetime;
	
	/*
	 * Enforce ordering of policy timeouts to prevent admins from
	 * screwing the database so badly it can't come back up and allow 
	 * them to change it back.
	 * 
	 * We enforce the ordering: 
	 * 
	 * 60 seconds <= default
	 * 60 seconds <= max
	 * 60 seconds <= max_renewable
	 * min <= default <= max <= max_renewable
	 */

	max_life = max (60, max_life);
	max_renewable_life = max (60, max_renewable_life);
	default_life = max(60, default_life);

	max_life = min (max_life, max_renewable_life);
	default_life = min (default_life, max_life);
	min_life = min (min_life, default_life);

	kdc_policy->max_life = max_life;
	kdc_policy->max_renewable_life = max_renewable_life;
	kdc_policy->min_life = min_life;
	kdc_policy->default_life = default_life;
    }

    return status;
}

/* !!! probable memory leaks here */
error_status_t krb5_kdc_pk_client_key_retriever (
    void *principal,
    sec_pk_domain_t *domain,
    sec_pk_usage_flags_t usage,
    unsigned32 *kvno,
    sec_pk_pubkey_t *pubkey_p
)
{
    error_status_t status;
    sec_attr_t returned_attr;
    uuid_t *attr_id;
    uuid_t obj_uuid;
    krb5_principal princ = (krb5_principal)principal;
    sec_rgy_login_name_t login_name;
    sec_rgy_name_t fullname;
    rsdb_util_rgy_obj_info_t obj_info;
    sec_passwd_rec_t         passwd;
    
    if ((usage & sec_pk_usage_keyEncipherment) &&
        !(usage & !sec_pk_usage_keyEncipherment))
        attr_id=&DCEPKKeyEncipherment_id;
    else if ((usage & sec_pk_usage_digitalSignature) &&
        !(usage & !sec_pk_usage_digitalSignature))
        attr_id=&DCEPKAuthentication_id;
    else
        return sec_pk_e_usage_unsupported;

    /* convert the principal name to a login name */
    status = princ_to_login_name(princ, &login_name);
    if (status)
	return status;

    /* canonicalize the login name */
    rsdb_name_util_complete_name(sec_rgy_domain_person, login_name.pname,
				 fullname, &status);
    if (status)
	return status;
    
    /* find the object in the database... */
    rsdb_util_get_obj_info_by_name(fullname, rsdb_pgo_type,
				   sec_acl_type_object, &obj_info, 
				   &obj_uuid, &status);
    if (status)
	return status;

    /* and now look for the attribute ... */
    rsdb_attr_lookup1_by_uuid(&obj_info,
			      attr_id,
			      rpc_ss_allocate, /* allocator */
			      &returned_attr,
			      &status);

    if (status)
	return status;

    /* Successfully read public key attribute */

    /* IDL-decode the attribute value */
    sec_pwd_decode(rpc_ss_allocate,
                   rpc_ss_free,
                   returned_attr.attr_value.tagged_union.bytes->length,
                   returned_attr.attr_value.tagged_union.bytes->data,
                   &passwd,
                   &status);
    if (status)
        return status;

    /* Check version number */
    if (kvno) {
        if ((*kvno != 0) && (passwd.version_number != *kvno)) {
            rpc_ss_free(PUBKEY_PASSWD_DATA(&passwd));
            return sec_pk_e_key_vers_unavailable;
        } else {
            *kvno=passwd.version_number;
        }
    }

    status = sec_pk_data_create (
	    PUBKEY_PASSWD_DATA(&passwd),
	    PUBKEY_PASSWD_LEN(&passwd),
	    pubkey_p);

    return status;
}

