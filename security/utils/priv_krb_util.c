/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: priv_krb_util.c,v $
 * Revision 1.1.9.2  1996/10/03  15:14:12  arvind
 * 	Silence compiler warnings.
 * 	Deal with API change to authdata encode/decode.
 * 	[1996/09/16  23:32 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * 	last endgame sec fix fron hp -mcg
 * 	[1996/04/16  21:00:21  root]
 *
 * 	construct_v1_ptgt_authdata, construct_v1_stkt_authdata: memset
 * 	authorization data to NULL before using to clear out garbage bytes
 * 	which may make match_dce_authdata() fail.
 * 	[1995/08/22  21:07 UTC  mullan_s  /main/HPDCE02/7]
 *
 * 	Merge fix for CHFts15637
 * 	[1995/08/15  17:10 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/7]
 *
 * 	In v1 authdata, store UUID as bytes instead of string.
 * 	[1995/08/11  15:27 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/6]
 *
 * 	Add more comments.
 * 	[1995/08/10  18:20 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/5]
 *
 * 	Add priv_krb_util_compare_authdata_uuids
 * 	[1995/08/09  14:12 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/4]
 *
 * 	work.
 * 	[1995/08/09  13:43 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/3]
 *
 * 	Convert uuid to string before adding to authdata.
 * 	[1995/08/02  15:26 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/2]
 *
 * 	Add support for parsing out uuid from DCE 1.1 authorization data.
 * 	[1995/07/26  19:33 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/1]
 *
 * 	Add priv_krb_util_epac_search.
 * 	[1995/05/26  19:37 UTC  mullan_s  /main/HPDCE02/6]
 *
 * 	Merge
 * 	[1995/05/26  18:49 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat11/1]
 *
 * 	fetch_v1_1_ccache_authdata: If login context == NULL, do not lock .data.db file.
 * 	[1995/05/22  17:05 UTC  mullan_s  /main/HPDCE02/5]
 *
 * 	Merge
 * 	[1995/05/22  15:49 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat9/1]
 *
 * 	priv_krb_util_fetch_v1_1_ccache_authdata: call thread-safe
 * 	sec_login_util_fetch_db_entry.
 * 	[1995/05/08  21:09 UTC  mullan_s  /main/HPDCE02/4]
 *
 * 	Merge
 * 	[1995/05/08  19:46 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat4/1]
 *
 * 	Use sec_svc_handle when calling dce_assert().
 * 	[1995/04/21  20:15 UTC  mullan_s  /main/HPDCE02/3]
 *
 * 	priv_krb_parse_v1_1_authdata: simplify greatly so that this routine
 * 	looks for an epac chain in the OSF_DCE authorization data.
 * 	[1995/04/03  22:14 UTC  mullan_s  /main/HPDCE02/2]
 *
 * 	Binary Compatibility Merge
 * 	[1995/01/11  14:32 UTC  greg  /main/HPDCE02/1]
 *
 * 	Fix memory leaks per OSF CR # 12774
 * 	[1994/08/18  20:25:43  greg  1.1.4.5]
 *
 * Revision 1.1.9.1  1996/05/17  15:14:44  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/2  1996/05/16  22:05 UTC  psn
 * 	Merge fix for CHFts15637
 * 	[1995/08/22  21:07 UTC  mullan_s  /main/HPDCE02/7]
 * 
 * 	In v1 authdata, store UUID as bytes instead of string.
 * 	[1995/08/15  17:10 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/7]
 * 
 * 	Add more comments.
 * 	[1995/08/11  15:27 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/6]
 * 
 * 	Add priv_krb_util_compare_authdata_uuids
 * 	[1995/08/10  18:20 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/5]
 * 
 * 	work.
 * 	[1995/08/09  14:12 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/4]
 * 
 * 	Convert uuid to string before adding to authdata.
 * 	[1995/08/09  13:43 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/3]
 * 
 * 	Add support for parsing out uuid from DCE 1.1 authorization data.
 * 	[1995/08/02  15:26 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/2]
 * 
 * 	Add priv_krb_util_epac_search.
 * 	[1995/07/26  19:33 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/1]
 * 
 * 	Merge
 * 	[1995/05/26  19:37 UTC  mullan_s  /main/HPDCE02/6]
 * 
 * 	fetch_v1_1_ccache_authdata: If login context == NULL, do not lock .data.db file.
 * 	[1995/05/26  18:49 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat11/1]
 * 
 * 	Merge
 * 	[1995/05/22  17:05 UTC  mullan_s  /main/HPDCE02/5]
 * 
 * 	priv_krb_util_fetch_v1_1_ccache_authdata: call thread-safe
 * 	sec_login_util_fetch_db_entry.
 * 	[1995/05/22  15:49 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat9/1]
 * 
 * 	Merge
 * 	[1995/05/08  21:09 UTC  mullan_s  /main/HPDCE02/4]
 * 
 * 	Use sec_svc_handle when calling dce_assert().
 * 	[1995/05/08  19:46 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat4/1]
 * 
 * 	priv_krb_parse_v1_1_authdata: simplify greatly so that this routine
 * 	looks for an epac chain in the OSF_DCE authorization data.
 * 	[1995/04/21  20:15 UTC  mullan_s  /main/HPDCE02/3]
 * 
 * 	Binary Compatibility Merge
 * 	[1995/04/03  22:14 UTC  mullan_s  /main/HPDCE02/2]
 * 
 * 	Fix memory leaks per OSF CR # 12774
 * 	[1995/01/11  14:32 UTC  greg  /main/HPDCE02/1]
 * 
 * 	construct_v1_ptgt_authdata, construct_v1_stkt_authdata: memset
 * 	authorization data to NULL before using to clear out garbage bytes
 * 	which may make match_dce_authdata() fail.
 * 	[1996/04/14  20:22 UTC  mullan_s  /main/HPDCE02/bartf_chfts16961/1]
 * 
 * 	Merge fix for CHFts15637
 * 	[1995/08/22  21:07 UTC  mullan_s  /main/HPDCE02/7]
 * 
 * 	In v1 authdata, store UUID as bytes instead of string.
 * 	[1995/08/15  17:10 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/7]
 * 
 * 	Add more comments.
 * 	[1995/08/11  15:27 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/6]
 * 
 * 	Add priv_krb_util_compare_authdata_uuids
 * 	[1995/08/10  18:20 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/5]
 * 
 * 	work.
 * 	[1995/08/09  14:12 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/4]
 * 
 * 	Convert uuid to string before adding to authdata.
 * 	[1995/08/09  13:43 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/3]
 * 
 * 	Add support for parsing out uuid from DCE 1.1 authorization data.
 * 	[1995/08/02  15:26 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/2]
 * 
 * 	Add priv_krb_util_epac_search.
 * 	[1995/07/26  19:33 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/1]
 * 
 * 	Merge
 * 	[1995/05/26  19:37 UTC  mullan_s  /main/HPDCE02/6]
 * 
 * 	fetch_v1_1_ccache_authdata: If login context == NULL, do not lock .data.db file.
 * 	[1995/05/26  18:49 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat11/1]
 * 
 * 	Merge
 * 	[1995/05/22  17:05 UTC  mullan_s  /main/HPDCE02/5]
 * 
 * 	priv_krb_util_fetch_v1_1_ccache_authdata: call thread-safe
 * 	sec_login_util_fetch_db_entry.
 * 	[1995/05/22  15:49 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat9/1]
 * 
 * 	Merge
 * 	[1995/05/08  21:09 UTC  mullan_s  /main/HPDCE02/4]
 * 
 * 	Use sec_svc_handle when calling dce_assert().
 * 	[1995/05/08  19:46 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat4/1]
 * 
 * 	priv_krb_parse_v1_1_authdata: simplify greatly so that this routine
 * 	looks for an epac chain in the OSF_DCE authorization data.
 * 	[1995/04/21  20:15 UTC  mullan_s  /main/HPDCE02/3]
 * 
 * 	Binary Compatibility Merge
 * 	[1995/04/03  22:14 UTC  mullan_s  /main/HPDCE02/2]
 * 
 * 	Fix memory leaks per OSF CR # 12774
 * 	[1995/01/11  14:32 UTC  greg  /main/HPDCE02/1]
 * 
 * 	HP revision /main/DCE_1.2/1  1996/01/03  19:20 UTC  psn
 * 	Merge fix for CHFts15637
 * 	[1
 * 
 * Revision 1.1.4.5  1994/08/18  20:25:43  greg
 * 	Flush FAKE-EPAC authorization data scheme and institute
 * 	authorization data processing according to the real
 * 	1.1 authorization data protocol.
 * 	[1994/08/16  20:22:55  greg]
 * 
 * Revision 1.1.4.4  1994/08/04  16:14:57  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/08/02  17:17:09  mdf]
 * 
 * Revision 1.1.2.2  1992/12/29  16:40:47  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:32:01  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:41:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1994, 1995, 1996
** Unpublished work. All Rights Reserved.
**
*/
/* 
 * priv_krb_util.c: common priv server routines
 */ 

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)priv_krb_util.c	6 - 10/03/91";
#endif

#include <dce/prvnbase.h>
#include <bstring.h>
#include <un_maloc.h>
#include <macros.h>
#include <sec_pkl.h>
#include <sec_encode.h>
#include <sec_svc.h>
#include <pkl.h>
#include <priv_krb_util.h>
#include <u_str.h>
#include <krb5/asn1.h>
#include <dce/assert.h>

#define PTHREAD_EXC        /* make one version of pthread_exc.h harmless */
#define PTHREAD_EXC_H      /* make another version of pthread_exc.h harmless */
#include <dce/stubbase.h>  /* includes pthread_exc.h */
#include <authz_util.h>

/* DCE "registered" kerberos authorization data type */
#define OSF_DCE_KRB_AUTHDATA  64

/* Default version numbers for DCE authdata */
unsigned8 priv_krb_def_stkt_authdata_vno = PRIV_KRB_STKT_AUTHDATA_VNO_1;
unsigned8 priv_krb_def_ptgt_authdata_vno = PRIV_KRB_PTGT_AUTHDATA_VNO_1;

static boolean32 get_uuid(
    sec_bytes_t         authbytes,
    uuid_t              *auth_uuid
);

/*
 * delete any instances of DCE authorization data from the authdata
 * array and return a new array without the entries. If there is
 * no instance of DCE authorization data, it returns NULL for the
 * out_authdata.
 *
 * This routine does not free any entries from in_authdata, it is
 * up to the caller to do that after the call is finished.
 *
 * If there is no DCE authdata in in_authdata, this routine makes
 * a copy of in_authdata to out_authdata.
 *
 * NOTE : there should really only be at most one instance of DCE 
 * authorization, but this routine is ultra picky and looks for more
 * than one and throws them all out.
 */
void priv_krb_util_delete_dce_authdata(
    krb5_authdata 	**in_authdata,		/* [in]  */
    krb5_authdata 	***out_authdata,	/* [out] */
    error_status_t	*stp			/* [out] */
)
{
    int num_entries, num_dce_entries, i;
    krb5_authdata **ptr, **retdata;

    CLEAR_STATUS(stp);
    *out_authdata = NULL;

    if (!in_authdata)
        return;

    /* count up the instances of DCE auth data */
    num_entries = num_dce_entries = 0;
    for (ptr = in_authdata; *ptr; ptr++, num_entries++) {
        if ((*ptr)->ad_type == OSF_DCE_KRB_AUTHDATA)
            num_dce_entries++;
    }

    /*
     * Just return, if in_authdata only contains DCE authdata.
     * The caller should recognize that a NULL out_authdata means
     * this.
     */
    if (num_dce_entries == num_entries)
        return;

    num_entries = (num_entries + 1) - num_dce_entries;
    retdata = (krb5_authdata **) malloc(num_entries * sizeof(*retdata));
    if (!retdata) {
        SET_STATUS(stp, sec_priv_s_no_mem);
        return;
    }
    memset(retdata, 0, num_entries * sizeof(*retdata));

    i = 0;
    for (ptr = in_authdata; (ptr && *ptr); ptr++) {
        if ((*ptr)->ad_type != OSF_DCE_KRB_AUTHDATA) {
            retdata[i] = (krb5_authdata *)malloc(sizeof(*retdata[i]));
            if (!retdata[i]) {
                SET_STATUS(stp, sec_priv_s_no_mem);
                krb5_free_authdata(retdata);
                return;
            }
            *retdata[i] = **ptr;

            if (!(retdata[i]->contents =
                  (krb5_octet *)malloc(retdata[i]->length))) {
                SET_STATUS(stp, sec_priv_s_no_mem);
                krb5_free_authdata(retdata);
                return;
            }
            memcpy((char *) retdata[i]->contents,
                   (char *)(*ptr)->contents,
                   retdata[i]->length);
            i++;
        }
    }

    retdata[i] = 0;                     /* null-terminated array */
    *out_authdata = retdata;
}

/*
 * concat authorization data was lifted from kdc_util.c in the kerberos
 * source distribution, and modified slightly
 */  
/*
 * concatenate first two authdata arrays, returning an allocated replacement.
 * The replacement should be freed with krb5_free_authdata().
 * This routine is called from within DCE-specific code in the kerberos
 * library, so if you change the prototype, or the semantics, be sure
 * you find all references to this routine and take the appropriate
 * action.
 */
void priv_krb_util_concat_authdata(
    krb5_authdata   **dce_authdata,
    krb5_authdata   **extra_authdata,
    krb5_authdata   ***output,
    error_status_t  *stp
)
{
    register int i, j;
    register krb5_authdata **ptr, **retdata;

    CLEAR_STATUS(stp);

    /* count up the entries */
    i = 0;
    if (dce_authdata)
        for (ptr = dce_authdata; *ptr; ptr++,i++);
    if (extra_authdata)
        for (ptr = extra_authdata; *ptr; ptr++,i++);
    
    retdata = (krb5_authdata **)malloc((i+1)*sizeof(*retdata));
    if (!retdata) {
        SET_STATUS(stp, sec_priv_s_no_mem);
        return;
    }
    memset(retdata, 0, (i+1)*sizeof(*retdata));

    i = 0;
    for (j = 0, ptr = dce_authdata; j < 2 ; ptr = extra_authdata, j++) {
        while (ptr && *ptr) {
            /* now walk & copy */
            retdata[i] = (krb5_authdata *)malloc(sizeof(*retdata[i]));
            if (!retdata[i]) {
                SET_STATUS(stp, sec_priv_s_no_mem);
                krb5_free_authdata(retdata);
                return;
            }
	    
	    /*
             * There can only be one instance of DCE authorization
             * data in a privilege ticket granting ticket, so if we're 
             * copying from the extra_authdata array (i.e., j = 1) then
             * be sure to throw out any entries of type OSF_DCE_KRB_AUTHDATA;
             */
	    if ((j==0) || ((*ptr)->ad_type != OSF_DCE_KRB_AUTHDATA)) {
		*retdata[i] = **ptr;
		
		if (!(retdata[i]->contents =
		      (krb5_octet *)malloc(retdata[i]->length))) {
		    SET_STATUS(stp, sec_priv_s_no_mem);
                    krb5_free_authdata(retdata);
		    return;
		}
		memcpy((char *) retdata[i]->contents,
		       (char *)(*ptr)->contents,
		       retdata[i]->length);
		i++;
	    }
	    ptr++;
	}
    }

    retdata[i] = 0;                     /* null-terminated array */
    
    *output = retdata;
    return;
}

/*
 * Replaces DCE authorization data contained in the authdata
 * array with the DCE authorization data in dce_authdata.
 *
 * Assumes there is only one instance of DCE authorization
 * data in the input array and returns control to caller after
 * replacement.
 *
 * If there is no DCE authdata in the authdata array, this
 * routine does not replace anything!
 */
void priv_krb_util_replace_dce_authdata(
    krb5_authdata   *dce_authdata,		/* [in] */
    krb5_authdata   **authdata,			/* [in,out] */
    error_status_t  *stp			/* [out] */
)
{
    register krb5_authdata **ptr;

    CLEAR_STATUS(stp);

    if ((dce_authdata == NULL) || (dce_authdata->length == 0) ||
        (dce_authdata->contents == NULL) ||
        (dce_authdata->ad_type != OSF_DCE_KRB_AUTHDATA))
	return;

    for (ptr = authdata; *ptr; ptr++) {
        if ((*ptr)->ad_type == OSF_DCE_KRB_AUTHDATA) {
	   if ((*ptr)->contents != NULL && (*ptr)->contents != 0)
		free((*ptr)->contents); 
           (*ptr)->contents = malloc(dce_authdata->length);
           if ((*ptr)->contents != NULL) {
		memcpy((*ptr)->contents, dce_authdata->contents,
		       dce_authdata->length);
                (*ptr)->length = dce_authdata->length;
           } else 
		SET_STATUS(stp, sec_priv_s_no_mem);
           break;
	}
    }
}

/*
 * Retrieves instance of DCE authorization data from the
 * authdata array. If there is more than one instance, it
 * sets more to true.
 */
krb5_authdata *priv_krb_util_get_dce_authdata (
    krb5_authdata 	**authdata,		/* [in]  */	
    boolean32     	*more			/* [out] */
)
{
    krb5_authdata *dce_authdata;
    int i;

    *more = false;
    for (dce_authdata = NULL, i = 0; authdata[i] != NULL; i++) {
	
        if (authdata[i]->ad_type == OSF_DCE_KRB_AUTHDATA) {
	    if (dce_authdata != NULL) {
		/* 
		 * only one instance of DCE authorization data
		 * allowed in a ticket and we just found a second
		 * one, so set more to true and bail out.
		 */
		*more = true;
		break;
	    } else {
		dce_authdata = authdata[i];
	    }
	}
    }

    return dce_authdata;
}

static void dce_authz_data_convert (
    idl_void_p_t   (*alloc)(idl_size_t size),     /* [in] */
    void           (*dealloc)(idl_void_p_t ptr),  /* [in] */
    sec_bytes_t    *encoded_dce_authz_data,       /* [in] */
    krb5_authdata  ***authdata,                   /* [out] */
    error_status_t *stp                           /* [out] */
)
{
    krb5_authdata  **dce_authdata;
 
    *authdata = NULL;

    /*
     * stuff the encoded bytes into krb5_authdata of the
     * registered DCE type.
     */
   
    dce_authdata = (krb5_authdata **) (*alloc)(2*sizeof(*dce_authdata));
    if (dce_authdata == NULL) {
        SET_STATUS(stp, sec_priv_s_no_mem);
        return;
    }
     
    (dce_authdata)[0] = (krb5_authdata *) (*alloc)(sizeof(krb5_authdata));
    if ((dce_authdata)[0] == NULL) {
        SET_STATUS(stp, sec_priv_s_no_mem);
        (*dealloc)( (char *) dce_authdata);
        return;
    }
                 
    (dce_authdata)[0]->ad_type = OSF_DCE_KRB_AUTHDATA;
    (dce_authdata)[0]->length = encoded_dce_authz_data->num_bytes;
    (dce_authdata)[0]->contents = 
        (krb5_octet *) (*alloc)((dce_authdata)[0]->length);
    
    if ((dce_authdata)[0]->contents == NULL) {
        SET_STATUS(stp, sec_priv_s_no_mem);
        (*dealloc)( (char *) (dce_authdata)[0]);
        (*dealloc)( (char *) dce_authdata);
        return;
    }
    
    memcpy((dce_authdata)[0]->contents, 
	   encoded_dce_authz_data->bytes,
	   encoded_dce_authz_data->num_bytes);

    (dce_authdata)[1] = NULL;  /* NULL terminated array */

    *authdata = dce_authdata;
}


/* p r i v _  k r b _u t i l _  p a c _ t o _  a u t h d a t a
 *
 * take a pickled pac and transform it into a krb5_auth_data array
 * suitable for inclusion in a krb5 ticket.  The result includes 
 * any "extra" authdata passed in.  passing NULL for in the extra
 * authdata parameter will prevent this from occuring
 */
 void  priv_krb_util_pac_to_authdata(
    idl_pkl_t             *ppac,             /* [in]  */
    unsigned32            ppac_len,          /* [in]  */
    krb5_authdata         **extra_authdata,  /* [in]  */
    krb5_authdata         ***authdata,       /* [out] */
    error_status_t        *stp		     /* [out] */
)
{
    krb5_authdata         **dce_authdata;
    sec_bytes_t           dce_authz_data;

    CLEAR_STATUS(stp);
    *authdata = NULL;
    dce_authz_data.num_bytes = ppac_len;
    dce_authz_data.bytes = &(*ppac)[0];

    priv_krb_util_bytes_to_authdata(&dce_authz_data,
                                    extra_authdata,
                                    &dce_authdata,
                                    stp);
    if (GOOD_STATUS(stp)) {
        *authdata = dce_authdata;
    }
}

/* p r i v _ k r b _ u t i l _ b y t e s _ t o _ a u t h d a t a
 *
 * take a sec_bytes_t and transform it into a krb5_auth_data array
 * suitable for inclusion in a krb5 ticket.  The result includes
 * any "extra" authdata passed in.  passing NULL for in the extra
 * authdata parameter will prevent this from occuring
 */
void priv_krb_util_bytes_to_authdata(
    sec_bytes_t           *authbytes,        /* [in]  */
    krb5_authdata         **extra_authdata,  /* [in]  */
    krb5_authdata         ***authdata,       /* [out] */
    error_status_t        *stp               /* [out] */
)
{
    krb5_authdata         **dce_authdata;

    CLEAR_STATUS(stp);
    *authdata = NULL;

    dce_authz_data_convert(malloc_shim, free, authbytes, 
			   &dce_authdata, stp);
    
    if (!STATUS_OK(stp)) {
	return;
    }

    if (extra_authdata != NULL) {
        priv_krb_util_concat_authdata(dce_authdata, extra_authdata, authdata, stp);
        
        /* 
         * we've either copied dce_authdata, or encountered a failure
         * condition.  In either case, we're done with it.
         */
        krb5_free_authdata(dce_authdata);
        if (BAD_STATUS(stp)) {
            *authdata = NULL;
        }
         
    } else {
        *authdata = dce_authdata;
    }
}


/* p r i v _ k r b _ u t i l _  a u t h d a t a _ t o _ p a c
 *
 * Basically the inverse of priv_util_krb_pac_to_authdata.
 * Storage is allocated for a copy of the authdata pac (if any)
 * and must be freed by the caller.  If there is no authdata
 * of the DCE registered type in the input authdata array, the returned
 * pointer is NULL.
 */
void  priv_krb_util_authdata_to_pac(
    krb5_authdata         **authdata,   /* [in] */
    idl_pkl_t             **ppac,       /* [out] */
    error_status_t        *stp
)
{
    int i;
    krb5_authdata *dce_authdata;
    boolean32     more;

    *ppac = NULL;
    CLEAR_STATUS(stp);

    if (authdata == NULL) {
        return;
    }
    
    dce_authdata = priv_krb_util_get_dce_authdata(authdata, &more);
    if (dce_authdata != NULL
    && dce_authdata->length != 0
    && dce_authdata->contents != NULL
    && !more) {
	sec_bytes_t   dce_authz_data, encoded_v1_1_ad;
	idl_pkl_t     *encoded_pac;
	unsigned32    encoded_pac_len;
	
	dce_authz_data.num_bytes = dce_authdata->length;
	dce_authz_data.bytes = dce_authdata->contents;
	sec__authz_util_unmarsh_v0_authzd(&dce_authz_data,
					  &encoded_v1_1_ad,
					  &encoded_pac_len,
					  &encoded_pac,
					  stp);
	if (STATUS_OK(stp)) {
	    if (encoded_pac_len && encoded_pac) {
		*ppac = malloc(encoded_pac_len);
		
		if (*ppac == 0) {
		    SET_STATUS(stp, sec_priv_s_no_mem);
		}
		
		memcpy(*ppac, encoded_pac, encoded_pac_len);
	    }
	}
    }
}


/* p r i v _  k r b _ u t i l _  p a c _ t o _ e n c _  a u t h d a t a
 *
 * create an authdata item of the DCE registered type, and encode and encrypt
 * it, along with any "extra" authdata, and return the result in the
 * enc_authdata parameter.
 * 
 * This routine assigns an appropriate etype to the returned encrypted data,
 * but it's up the the caller to set the key version number field.
 */
 void  priv_krb_util_pac_to_enc_authdata(
    idl_pkl_t             *ppac,              /* [in] */
    unsigned32            ppac_len,           /* [in] */
    krb5_authdata         **extra_authdata,   /* [in] */
    krb5_encrypt_block    *eblock,            /* [in] */
    krb5_enc_data         *enc_authdata,      /* [out] */
    error_status_t        *stp
)
{
    krb5_data             *scratch;
    krb5_authdata         **authdata;
    error_status_t        st;

    CLEAR_STATUS(stp);
    
    priv_krb_util_pac_to_authdata(ppac, ppac_len, extra_authdata,
                                    &authdata, stp);
    if (BAD_STATUS(stp)) {
        return;
    }

    st = encode_krb5_authdata(authdata, &scratch);
    krb5_free_authdata(authdata);
    if (st != 0) {
        *stp = st;
        return;
    }
    
    enc_authdata->ciphertext.length =
        krb5_encrypt_size(scratch->length, eblock->crypto_entry);
    /* add padding area, and zero it */
    scratch->data = realloc(scratch->data, enc_authdata->ciphertext.length);
    if (scratch->data == NULL) {
        /* may destroy scratch->data */
        xfree(scratch);
        SET_STATUS(stp, sec_priv_s_no_mem);
        return;
    }
    
    memset(scratch->data + scratch->length, 0,
               enc_authdata->ciphertext.length - scratch->length);
    enc_authdata->ciphertext.data = malloc(enc_authdata->ciphertext.length);
    if (enc_authdata->ciphertext.data == NULL) {
        krb5_free_data(scratch);
        SET_STATUS(stp, sec_priv_s_no_mem);
        return;
        }
    
    /* call the encryption routine */
    st =  krb5_encrypt((krb5_pointer) scratch->data,
                        (krb5_pointer) enc_authdata->ciphertext.data,
                         scratch->length, eblock, 0);
    
    krb5_free_data(scratch);

    if (st != 0)
    {
        *stp = st;
        return;
    }
    enc_authdata->etype = eblock->crypto_entry->proto_enctype;
}


void  priv_krb_util_enc_authdata_to_pac(
    krb5_enc_data         *enc_authdata,  /* [in] */
    krb5_encrypt_block    *eblock,        /* [in] */
    idl_pkl_t             **ppac,         /* [out] */
    krb5_authdata         ***authdata,    /* [out] */
    error_status_t        *stp
)
{
    krb5_data             scratch;
    krb5_authdata         **ap;
    error_status_t        st;

    scratch.length = enc_authdata->ciphertext.length;
    if (!(scratch.data =
          malloc(enc_authdata->ciphertext.length))) {
        SET_STATUS(stp, sec_priv_s_no_mem);
        return;
    }

    /* call the encryption routine */
    st = krb5_decrypt((krb5_pointer) enc_authdata->ciphertext.data,
                              (krb5_pointer) scratch.data,
                              scratch.length, eblock, 0);
    if (st != 0) {
        free(scratch.data);
        *stp = st;
        return;
    }

    st = decode_krb5_authdata(&scratch, &ap);
    free(scratch.data);
    if (st != 0) {
        *stp = st;      
        return;
    }

    if (ppac != NULL) {
        priv_krb_util_authdata_to_pac(ap, ppac, stp);
        if (BAD_STATUS(stp)) {
            krb5_free_authdata(ap);
            return;
        }
    }

    if (authdata != NULL) {
        *authdata = ap;
    } else {
        krb5_free_authdata(ap);
    }                       

    xfree(ap);
}


void  priv_krb_util_enc_dlg_token(
    krb5_encrypt_block      *eblock,
    sec_dlg_token_set_t     *dlg_token_set,
    sec_encrypted_bytes_t   *enc_dlg_token,
    error_status_t          *stp
)
{
    krb5_data  in, out;
    unsigned32 num_bytes;
    idl_byte   bytes;

    sec_dlg_token_set_encode((void *(*)(unsigned32))malloc, free,
        dlg_token_set, (unsigned32 *)&in.length, (idl_byte **) &in.data, stp);
    if (GOOD_STATUS(stp)) {
        sec_login_util_encrypt(eblock, &in, &out, stp);
        if (BAD_STATUS(stp)) {
            sec_encode_buffer_free(free, (idl_byte **)&in.data);
        } else {
            enc_dlg_token->ebytes.num_bytes = out.length;
            enc_dlg_token->ebytes.bytes = (idl_byte *) out.data;
        }
    }
}


unsigned32  priv_krb_util_parse_v0_authdata (
    idl_void_p_t           (*alloc)(idl_size_t size),     /* [in] */
    void                   (*dealloc)(idl_void_p_t ptr),  /* [in] */
    krb5_authdata          **authdata,                    /* [in] */
    sec_v1_1_authz_data_t  *v1_1_ad,                      /* [out] */
    idl_pkl_t              **ppac,                        /* [out] */
    error_status_t         *stp                           /* [out] */
) 
{
    krb5_authdata  *dce_authdata = NULL;
    unsigned32     ad_parts = 0;
    sec_bytes_t    authz_data, v1_pickle;
    idl_pkl_t      *encoded_pac;
    unsigned32     encoded_pac_len = 0;
    uuid_t         type;
    error_status_t ignore_st;
    boolean32      more;
    
    CLEAR_STATUS(stp);

    if (authdata == NULL) {
        return  priv_ad_part_none;
    }
    
    /* 
     * caller can tell us not to bother with either of the
     * the two types of authorization data by passing NULL
     * in the respective parameters
     */
    
    dce_authdata = priv_krb_util_get_dce_authdata(authdata, &more);
    
    if (dce_authdata == NULL) {
	return  priv_ad_part_none;
    }

    /*
     * treat a zero-length krb5_authdata entry of type OSF_DCE
     * as "unknown" because it's there and we don't know
     * what it means.  Treat multiple entries of type OSF_DCE 
     * as "unknown"
     */
    if (dce_authdata->length == 0
    || dce_authdata->contents == NULL
    || more) {
	return  priv_ad_part_unknown;
    }

    /*
     * There is a non-NULL entry of type OSF_DCE in the krb5_authdata field
     * of our ticket.  Make sure that it contains version 0 DCE authorization
     * data of a type that we undertand. If not, return "unknown." 
     * Presumably this state of affairs arises only if we've received
     * a ticket generated by some newer version of security that has 
     * dispensed with v1.1 compatibility altogether.
     */
    
    authz_data.num_bytes = dce_authdata->length;
    authz_data.bytes = dce_authdata->contents;

    sec__authz_util_get_authzd_type(&authz_data, &type);
    if (!(uuid_equal(&type, &sec__g_v0_authzd_compat, &ignore_st)
    || uuid_equal(&type, &sec__g_v0_authzd_no_compat, &ignore_st))) {
	return priv_ad_part_unknown;
    }

    sec__authz_util_unmarsh_v0_authzd(&authz_data, &v1_pickle,
				      &encoded_pac_len, &encoded_pac,
				      stp);
    if (!STATUS_OK(stp)) {
	return priv_ad_part_unknown;
    }
    
    if (v1_pickle.num_bytes) {
	sec_encode_type_t  encoding_type;
	
	/* 
	 * Make sure the v1_pickle is a version 1 idl 
	 * encoding of version 1.1 authorization data.
	 * If not, ignore it.
	 */
	encoding_type = 
	    sec_encode_get_encoding_type(v1_pickle.num_bytes,
					 v1_pickle.bytes);
	
	if (encoding_type == sec_encode_type_v1_1_authz_data) {
	    /* 
	     * If the caller asked for a copy of the version 1.1
	     * authorization data, then decode it for them.
	     */
	    CLEAR_STATUS(stp);
	    if (v1_1_ad) {
		sec_v1_1_authz_data_decode(alloc, dealloc,
					   v1_pickle.num_bytes,
					   v1_pickle.bytes,
					   v1_1_ad,
					   stp);
		
	    }
	    if (STATUS_OK(stp)) { 
		SET(ad_parts, priv_ad_part_v1_1);
	    }
	}
    }
    
    if (STATUS_OK(stp) && encoded_pac_len) {
	/* 
	 * The v0 authorization data contain a v1 PAC, so
         * give the caller a copy of it if they've asked for
         * one.
	 */
	if (ppac) {
	    *ppac = (*alloc)(encoded_pac_len);
	
	    if (*ppac != NULL) {
		memcpy(*ppac, encoded_pac, encoded_pac_len);
	    } else {
		SET_STATUS(stp, sec_s_no_memory);
	    }
	}
	if (STATUS_OK(stp)) { 
	    SET(ad_parts, priv_ad_part_pac);
	}
    }
    
    if (!STATUS_OK(stp)) {
	if (FLAG_SET(ad_parts, priv_ad_part_v1_1)
	&& v1_1_ad) {
	    sec_encode_v1_1_authz_data_free(dealloc, v1_1_ad);
	}
	if (FLAG_SET(ad_parts, priv_ad_part_pac) && ppac) {
	    (*dealloc)(ppac);
	}
	/*
	 * If we've gotten this far and encountered a bad status
	 * it's because we were unable to succesfully process 
	 * what we originally thought was legitimate version 0
	 * DCE authorization data.  That being so, we return
         * "unknown."  This only matters if the caller
         * isn't checking the return status.  We want such a badly
	 * behaved caller to think there was soemthing there that
         * wasn't understandable rather than thinking there
         * was nothing at all.
	 */
	ad_parts = 0 | priv_ad_part_unknown;
    }

    return ad_parts;
}

/*
 * NOTE WELL!
 *
 * The caller must provide memory management routines.
 * This is primarily so that server routines may use
 * rpc_ss_allocate() and rpc_ss_free() and thereby take
 * advantage of the RPC garbage collection mechanism.
 * BUT, the thing being allocated is krb5_authdata,
 * and the kerberos krb5_free_authdata() routine always
 * uses free().  So, if you pass anything but the system
 * malloc() routine, make absolutely certain that you
 * are never handing the resulting krb5_authdata off
 * to the krb5_free_authdata() routine (This can happen
 * in subtle ways, such as the freeing of a structure that
 * has authdata nested within it e.g.,  krb5_req/rep structures.
 */
void  priv_krb_util_gen_v0_authdata (
    idl_void_p_t           (*alloc)(idl_size_t size),     /* [in] */
    void                   (*dealloc)(idl_void_p_t ptr),  /* [in] */
    sec_v1_1_authz_data_t  *v1_1_authz_data,              /* [in] */
    sec_id_pac_t           *v1_pac,                       /* [in] */
    krb5_authdata          ***authdata,                   /* [out] */
    error_status_t         *stp                           /* [out] */
) 
{
    sec_bytes_t    encoded_v1_1_authz_data = {0, 0}, 
                   encoded_v0_authz_data = {0, 0};
    idl_pkl_t      *encoded_v1_pac = NULL;
    unsigned32     encoded_v1_pac_len = 0;

    CLEAR_STATUS(stp);
    *authdata = NULL;

    /* if caller gives us nothing, then do nothing */
    if (!(v1_1_authz_data || v1_pac)) {
	return;
    }

    if (v1_1_authz_data) {
	sec_v1_1_authz_data_encode(alloc, dealloc,
				   v1_1_authz_data,
				   &encoded_v1_1_authz_data.num_bytes,
				   &encoded_v1_1_authz_data.bytes,
				   stp); 
    }

    if (STATUS_OK(stp) && v1_pac) {
	sec_id_pac_t_pickle(v1_pac, NULL, (void *(*)(unsigned32)) malloc, 8,
			    &encoded_v1_pac, &encoded_v1_pac_len, stp);
    }

    if (STATUS_OK(stp)) {
	sec__authz_util_marsh_v0_authzd(alloc, dealloc, 
					&encoded_v1_1_authz_data,
					encoded_v1_pac_len,
					encoded_v1_pac, 
					&encoded_v0_authz_data,
					stp);
    }
					
    /*
     * We're done with v1.1 and v1 encoding buffers, regardless.
     */
    if (encoded_v1_pac) {
	free(encoded_v1_pac);
    }
    sec_encode_buffer_free(dealloc, &encoded_v1_1_authz_data.bytes);

    if (!STATUS_OK(stp)) {
	return;
    }

    dce_authz_data_convert(alloc, dealloc, &encoded_v0_authz_data,
			   authdata, stp);

    /*
     * We're done with the encoded v0 authz data, regardless
     */
    sec_encode_buffer_free(dealloc, &encoded_v0_authz_data.bytes);

}

/*
 * Converts an epac chain into a pickled pac
 */
void priv_krb_util_gen_v1_pac(
    idl_void_p_t           (*alloc)(idl_size_t size),     /* [in] */
    void                   (*dealloc)(idl_void_p_t ptr),  /* [in] */
    sec_bytes_t		   *epac_chain,			  /* [in] */
    idl_pkl_t   	   **v1_pac,			  /* [out] */
    unsigned32	           *v1_pac_len,			  /* [out] */
    error_status_t	   *stp				  /* [out] */
)
{
    sec_id_epac_set_t	epac_set;
    sec_id_epac_data_t  last_caller_epac_data, compat_epac_data,
                        initiator_epac_data;
    sec_id_pac_t	compat_pac;

    CLEAR_STATUS(stp);

    dce_assert(sec_svc_handle, epac_chain != NULL);

    *v1_pac = NULL;
    *v1_pac_len = 0;
    memset(&epac_set, 0, sizeof(epac_set));
    memset(&last_caller_epac_data, 0, sizeof(last_caller_epac_data));
    memset(&initiator_epac_data, 0, sizeof(initiator_epac_data));
    memset(&compat_pac, 0, sizeof(compat_pac));

    sec_id_epac_set_decode(alloc, dealloc, epac_chain->num_bytes,
                           epac_chain->bytes, &epac_set, stp);

    if (STATUS_OK(stp)) {
        /*
         * decode the last epac in the chain to determine the
         * compat mode.
         */
        sec_id_epac_data_decode(alloc, dealloc,
            epac_set.epacs[epac_set.num_epacs-1].pickled_epac_data.num_bytes,
            epac_set.epacs[epac_set.num_epacs-1].pickled_epac_data.bytes,
            &last_caller_epac_data, stp);
    }

    if (STATUS_OK(stp)) {
        switch(last_caller_epac_data.compat_mode) {

        case sec_id_compat_mode_caller:
            /* generate v1 PAC from the last caller's epac data */
            compat_epac_data = last_caller_epac_data;
            break;

        case sec_id_compat_mode_initiator:
            /*
             * generate v1 PAC from the initiator's epac data ( initiator
             * and last caller will be the same if num_epacs = 1
             */
            if (epac_set.num_epacs != 1) {
                sec_id_epac_data_decode(alloc, dealloc,
                                        epac_set.epacs[0].pickled_epac_data.num_bytes,
                                        epac_set.epacs[0].pickled_epac_data.bytes,
                                        &initiator_epac_data, stp);

                if (STATUS_OK(stp)) {
                    compat_epac_data = initiator_epac_data;
                }
            } else {
                compat_epac_data = last_caller_epac_data;
            }
            break;
    
        case sec_id_compat_mode_none:
            /*
             * generate a NIL EPAC to provide consistency
             * with what a server process sees after a call to
             * rpc_binding_inq_auth_client on V1_1 credentials
             * in which compatibility has been turned off (it's
             * a long story, having to do with a bug in pre-1.1
             * libraries that required us to always place a
             * pickled PAC of some sort in the authdata in the
             * client's credential cache--otherwise statically
             * linked pre-1.1 applications that called
             * sec_login_inquire_net_info on a credential cache
             * generated by a 1.1 library would crash).
             */
            memset(&compat_epac_data, 0, sizeof(compat_epac_data));
            break;

        default:
            /*
             * Can only imagine this happening if we've gotten our
             * epac chain from some post 1.1 version of security with
             * different compat modes, but we don't know what to do, so
             * return an error.
             */
            SET_STATUS(stp, sec_priv_s_bad_compat_mode);
            break;
        }
    }

    if (STATUS_OK(stp)) {
        /* convert compat_epac_data to a v1_PAC */
        sec__authz_util_epac_to_pac_alias(alloc, dealloc,
                                          &compat_epac_data, true,
                                          &compat_pac, stp);
    }

    if (STATUS_OK(stp)) {
        sec_id_pac_t_pickle(&compat_pac, NULL, (void *(*)(unsigned32)) malloc,
                            (unsigned32) 8, v1_pac, v1_pac_len, stp);

        /* done with compat_pac, in any event */
        sec__authz_util_pac_alias_free(dealloc, &compat_pac);
    }

    /*
     * The following destructor routines are bulletproofed
     * against NULL pointers, and we were careful to initialize
     * all the data structures involved,  so we don't need to check
     * before calling them.
     */
    sec_encode_epac_set_free(dealloc, &epac_set);
    sec_encode_epac_data_free(dealloc, &last_caller_epac_data);
    sec_encode_epac_data_free(dealloc, &initiator_epac_data);
}


/*
 * Client-only routines
 *
 * The following routines are used to generate and
 * parse the DCE authdata stored in the authdata vector of
 * DCE (E)PTGT credential cache entries.  The format of that
 * data is NOT the same as authorization data sealed in 
 * krb5 service tickets. 
 *
 * Client ccache authdata consists of an optional pickled v1 PAC
 * (specified if the gen_v1_pac flag is set to true in the
 * dce_cf.db file), followed by a 8-bit version number. The epac 
 * chain is stored in the .data.db file. The version number is 
 * used to locate the correct ticket in the creds file which it 
 * belongs to.
 */


/*
**++
**
**  ROUTINE NAME:  priv_krb_util_gen_v1_1_ccache_authdata   
**
**  SCOPE:              public
**
**  DESCRIPTION:   
**
**  Given a version 1.1 epac chain, construct an authorization
**  data buffer suitable for use in the authorization data
**  field of a krb5 DCE EPTGT credential cache entry.  The
**  caller may request either the "raw" privileges, the
**  krb5_authdata containing those raw privileges, the v1 pac, or
**  all three, by passing NULL pointers for those out parameters
**  that should be ignored.
**
**  This routine is for use only by security client library code
**  that is maintaining DCE credential cache entries.  It is of
**  no use for processing the DCE authorization data sealed in
**  kerberos v5 service tickets.
**
**  INPUTS:
**
**      epac_chain              encoded epac set representing a clients
**                                 version 1.1 privileges
**      gen_v1_pac              a boolean indicating whether or not
**                                 a pickled v1 PAC should be generated
**                              
**  INPUTS/OUTPUTS:
**
**  OUTPUTS:            
**
**      privs           the raw client authorization data stream.  If NULL,
**                         this parameter is ignored.
**      authdata        the raw authorization data expressed as krb5 authdata.
**                         If NULL, this parameter is ignored.
**      status          A value indicating the return status of the routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
**/

void  priv_krb_util_gen_v1_1_ccache_authdata(
    sec_bytes_t            *epac_chain,                   /* [in] */
    boolean32              gen_v1_pac,                    /* [in] */
    sec_bytes_t            *privs,                        /* [out] */
    krb5_authdata          ***authdata,                   /* [out] */
    error_status_t         *stp                           /* [out] */
) 
{
    krb5_authdata       **a_d, **a_d2;
    idl_pkl_t           *ppac;
    unsigned32          ppac_len = 0;
    sec_bytes_t         tmp_privs;
    krb5_authdata       **tmp_authdata;

    CLEAR_STATUS(stp);

    /* 
     * It's senseless to call this routine with a NULL epac chain,
     * or both privs and authdata NULL, and we make any such caller
     * pay a very heavy price indeed.
     */
    dce_assert(sec_svc_handle, epac_chain != NULL);
    dce_assert(sec_svc_handle, !(privs == NULL && authdata == NULL));

    tmp_privs.num_bytes = 0;
    tmp_privs.bytes = NULL;

    if (privs) {
	privs->num_bytes = 0;
	privs->bytes = NULL;
    }

    /* 
     * Generate a v1 pickled PAC for inclusion at the head of the ccache
     * authorization data strem only if the caller indicated that we
     * should do so.
     */
    if (gen_v1_pac) {
        priv_krb_util_gen_v1_pac(malloc_shim, free, epac_chain, 
                                 &ppac, &ppac_len, stp);
    }
	
    if (STATUS_OK(stp)) {
	uuid_t 			ptgt_uuid;
	/*
	 * We need a buffer that contains the pickled pac, if any, followed
         * by a 8-bit version number, which will be used to match the correct
         * ticket with the epac chain stored in the .data.db file, followed
         * by a uuid, which will be used to match with the correct service ticket.
         */

	/* Generate a uuid */
        uuid_create(&ptgt_uuid, stp); /* any failures are really bad */
	if (STATUS_OK(stp)) {
	    priv_krb_util_construct_v1_ptgt_authdata(ppac, ppac_len,
		&ptgt_uuid, NULL, &tmp_authdata, &tmp_privs, stp);
	}
	if (STATUS_OK(stp)) {
            if (authdata) {
                *authdata = tmp_authdata;
            } else {
		krb5_free_authdata(tmp_authdata);
            }
        }
    }

    /* done with pickled pac, in any event */
    if (ppac_len > 0 && ppac != NULL) {
        free(ppac);
    }

    if (STATUS_OK(stp)) {
        if (privs) {
            *privs = tmp_privs;
        }
    }

    /* cleanup as necessary */
    if (!privs && (tmp_privs.bytes && tmp_privs.num_bytes)) {
	free(tmp_privs.bytes);
    }

    /* clean up any allocated memory */
    if (BAD_STATUS(stp)) {
        if (privs && (tmp_privs.bytes && tmp_privs.num_bytes))
            free(tmp_privs.bytes);
    }
}

/*
**++
**
**  ROUTINE NAME:  priv_krb_util_parse_v1_1_ccache_authdata
**
**  SCOPE:              public
**
**  DESCRIPTION:
**
**  Given authdata constructed by sec_krb_get_cred, extract the 
**  v1.1 epac_chain. If there is no authorization data of type
**  OSF_DCE, this routine returns false. This means that the server
**  is most likely 1.0.x based.
**
**  NOTE!!!: this routine should not be used to parse the authorization
**  data stored on disk in the krb5 creds file. It should only be used
**  to parse the in-memory authdata constructed by the sec_krb_get_cred
**  routine.
**
**  INPUTS:
**
**      alloc           memory allocator function pointer
**      dealloc         memory deallocator function pointer
**      authdata        krb5 authdata from a DCE (E)PTGT credential
**                         cache entry.  The contents field of any DCE
**                         authorization data contained therein must be
**                         aligned on an 8-byte boundary.
**      stp             error status.  The boolean32 function return
**                      value is meaningful only if the stp returns
**                      with error_status_ok.  The error status is
**                      used to report basic failures (out of memory,
**                      etc).
**
**  INPUTS/OUTPUTS:
**
**  OUTPUTS:
**
**      epac_chain      the raw client authorization data stream.  If NULL,
**                         this parameter is ignored.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      True if the authorization data stream contains an encoded
**      v1.1 epac chain, false otherwise.  Meaningful only if the
**      "stp" return parameter is error_status_ok.
**
**
**  SIDE EFFECTS:       none
**
**--
**/

boolean32  priv_krb_util_parse_v1_1_ccache_authdata(
    idl_void_p_t           (*alloc)(idl_size_t size),     /* [in] */
    void                   (*dealloc)(idl_void_p_t ptr),  /* [in] */
    krb5_authdata          **authdata,                    /* [in] */
    sec_bytes_t            *epac_chain,                   /* [out] */
    error_status_t         *stp                           /* [out] */
)
{
    boolean32         found = false;
    krb5_authdata     *dce_authdata = NULL;
    sec_bytes_t       tmp_epac_chain;
    sec_encode_type_t encoding_type;
    boolean32         more;

    CLEAR_STATUS(stp);

    if (authdata == NULL || *authdata == NULL || epac_chain == NULL) {
        return false;
    }

    dce_authdata = priv_krb_util_get_dce_authdata(authdata, &more);
    if (dce_authdata == NULL
    || dce_authdata->length == 0
    || dce_authdata->contents == NULL
    || more) {
        return false;
    }

    tmp_epac_chain.num_bytes = dce_authdata->length;

    /*
     * Check the encoding type to be sure we really
     * do have an encoded epac chain.  If so, we'll
     * allocate and memcpy it.
     */
    encoding_type = sec_encode_get_encoding_type(tmp_epac_chain.num_bytes,
			    (idl_byte *)dce_authdata->contents);

    if (encoding_type == sec_encode_type_epac_set) {
        tmp_epac_chain.bytes = (*alloc)(tmp_epac_chain.num_bytes);
        if (tmp_epac_chain.bytes != NULL) {
            memcpy(tmp_epac_chain.bytes, dce_authdata->contents, 
                   tmp_epac_chain.num_bytes);
            found = true;
            *epac_chain = tmp_epac_chain;
        } else {
            SET_STATUS(stp, sec_s_no_memory);
        }
    }

    return found;
}

/*
**++
**
**  ROUTINE NAME:  priv_krb_util_fetch_v1_1_ccache_authdata
**
**  SCOPE:              public
**
**  DESCRIPTION:
**
**  Given the new (1.1+) login ".db" datafile pathname, fetch the 
**  v1.1 epac chain which matches the target cell.
**
**  INPUTS:
**
**	login_context	handle to the login context the .db file refers to.
**			If NULL, then no thread locking of .db file is performed.
**      alloc           memory allocator function pointer
**      dealloc         memory deallocator function pointer
**      datafile	pathname of ".db" login storage file
**	target_cell	the cell which the epac chain we are requesting
**			   was granted from.
**
**  OUTPUTS:
**
**      epac_chain      the v1.1 epac chain data stream.  If NULL,
**                         this parameter is ignored.
**      authbytes	the raw client authorization data stream as stored
**                      in the krb5 credentials cache. Typically this would
**                      contain an optional v1 PAC followed by an 8-bit
**                      version number. If NULL, this parameter is ignored. 
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:
**
**      True if the epac chain is successfully fetched from the
**      datafile. Meaningful only if the "stp" return parameter
**      is error_status_ok.
**
**  SIDE EFFECTS:       none
**
**--
**/

boolean32 priv_krb_util_fetch_v1_1_ccache_authdata(
    sec_login_handle_t	   login_context,		  /* [in] */
    idl_void_p_t           (*alloc)(idl_size_t size),     /* [in] */
    void                   (*dealloc)(idl_void_p_t ptr),  /* [in] */
    char		   *datafile,			  /* [in] */
    unsigned_char_t        *target_cell,		  /* [in] */
    sec_bytes_t            *epac_chain,                   /* [out] */
    sec_bytes_t		   *authbytes,			  /* [out] */
    error_status_t         *stp                           /* [out] */
)
{
    sec_login_db_entry_t 	db_entry;
    boolean32       		found = false;

    CLEAR_STATUS(stp);

    if (target_cell == NULL || datafile == NULL) {
        return false;
    }

    memset(&db_entry, 0, sizeof(db_entry));
    if (epac_chain) {
        epac_chain->num_bytes = 0;
        epac_chain->bytes = NULL;
    }
    if (authbytes) {
        authbytes->num_bytes = 0;
        authbytes->bytes = NULL;
    }

    /*
     * fetch authdata from ".data.db" file
     */
    db_entry.entry_type = epac_enum;
    if (login_context == NULL)
	sec_login_db_fetch_entry(datafile, &db_entry, stp);
    else
        sec_login_util_fetch_db_entry(login_context, datafile, &db_entry, stp);
    if (BAD_STATUS(stp)) {
        /* not found isn't bad */
        if (STATUS_EQUAL(stp, db_s_key_not_found) ||
            STATUS_EQUAL(stp, db_s_open_failed_enoent)) {
            CLEAR_STATUS(stp);
        }
        return found;
    }

    if (!priv_krb_util_epac_search(db_entry, alloc, dealloc, target_cell, 
				   epac_chain, authbytes, stp)) {
        /* not found isn't bad */
        sec_login_db_free_entry_contents(rpc_ss_client_free, &db_entry, -1);
        return found;
    }
    if (STATUS_OK(stp)) {
        found = true;
    }

    sec_login_db_free_entry_contents(rpc_ss_client_free, &db_entry, -1);
    return found;
}

/*
 * priv_krb_util_epac_search
 *
 * Search the login storage .db credential file for a matching epac
 * chain for the target cell provided.
 *
 * Returns true if found, false otherwise.
 */
boolean32 priv_krb_util_epac_search(
    sec_login_db_entry_t	db_entry,			/* [in]  */
    idl_void_p_t 	        (*alloc)(idl_size_t size),  	/* [in]  */
    void                        (*dealloc)(idl_void_p_t ptr),  	/* [in]  */
    unsigned_char_t		*target_cell,			/* [in]  */
    sec_bytes_t			*epac_chain,			/* [out] */
    sec_bytes_t			*authbytes,			/* [out] */
    error_status_t		*stp				/* [out] */
)
{
    unsigned32			index;
    boolean32			found = false;
    krb5_timestamp  currenttime = 0; /* so we don't find expired credentials */

    CLEAR_STATUS(stp);
    if (epac_chain) {
        epac_chain->num_bytes = 0;
        epac_chain->bytes = NULL;
    }
    if (authbytes) {
        authbytes->num_bytes = 0;
        authbytes->bytes = NULL;
    }

    /*
     * if getting the current time won't work, then the process
     * must be completely hosed.  Don't bother trying to recover.
     */
    (void) krb5_timeofday(&currenttime);

    /*
     * Search for correct authdata in array
     */
    if (!sec_login_db_authdata_search((void *) DB_V1_AUTHDATA(&db_entry),
                                      (void *) target_cell, &index, stp)) {
        /* not found isn't bad */
        sec_login_db_free_entry_contents(rpc_ss_client_free, &db_entry, -1);
        return found;
    }

    /* don't want a ticket that's expired. */
    if (DB_V1_AUTHDATA_ARRAY(&db_entry, index).endtime < currenttime) {
        sec_login_db_free_entry_contents(rpc_ss_client_free, &db_entry, -1);
        return found;
    }

    if (STATUS_OK(stp)) {
        if (epac_chain && DB_V1_AUTHDATA_ARRAY(&db_entry, index).epac_chain.num_bytes
                       && DB_V1_AUTHDATA_ARRAY(&db_entry, index).epac_chain.bytes) {
            sec_bytes_t tmp_privs;
            tmp_privs.num_bytes = DB_V1_AUTHDATA_ARRAY(&db_entry, index).epac_chain.num_bytes;
            tmp_privs.bytes = (byte *)
                    (*alloc)(DB_V1_AUTHDATA_ARRAY(&db_entry, index).epac_chain.num_bytes);
            if (tmp_privs.bytes == NULL) {
                SET_STATUS(stp, sec_priv_s_no_mem);
            } else {
                memcpy(tmp_privs.bytes, DB_V1_AUTHDATA_ARRAY(&db_entry, index).epac_chain.bytes,
                       tmp_privs.num_bytes);
                *epac_chain = tmp_privs;
            }
        }
    }
    if (STATUS_OK(stp)) {
        if (authbytes && DB_V1_AUTHDATA_ARRAY(&db_entry, index).authbytes.num_bytes
                      && DB_V1_AUTHDATA_ARRAY(&db_entry, index).authbytes.bytes) {
            sec_bytes_t tmp_bytes;
            tmp_bytes.num_bytes = DB_V1_AUTHDATA_ARRAY(&db_entry, index).authbytes.num_bytes;
            tmp_bytes.bytes = (byte *)
                    (*alloc)(DB_V1_AUTHDATA_ARRAY(&db_entry, index).authbytes.num_bytes);
            if (tmp_bytes.bytes == NULL) {
                SET_STATUS(stp, sec_priv_s_no_mem);
                if (epac_chain && epac_chain->bytes)
                    (*dealloc)(epac_chain->bytes);
            } else {
                memcpy(tmp_bytes.bytes, DB_V1_AUTHDATA_ARRAY(&db_entry, index).authbytes.bytes,
                       tmp_bytes.num_bytes);
                *authbytes = tmp_bytes;
            }
        }
    }
    if (STATUS_OK(stp)) {
	found = true;
    }
    return found;
}

/*
 * This routine compares in_authdata with fetched_authdata. If a V1
 * PAC exists at the very beginning of fetched_authdata, it ignores
 * it and does a bit comparison of the remaining bits after the
 * V1 PAC.
 *
 * It returns TRUE if they match and FALSE otherwise.
 *
 * This routine is called by the low-level krb5 match authdata
 * routines. 
 *
 * This routine allows the caller to be as selective as they choose.
 * For example, if the caller only wants to match the version number,
 * the in_authdata should only contain the version number they wish
 * to match against. All other fetched authdata after the vno will
 * be ignored. 
 */
boolean32 priv_krb_util_match_dce_authdata(
    register const krb5_authdata	*in_authdata,
    register const krb5_authdata        *fetched_authdata
)
{
    unsigned char 	*mp;
    error_status_t 	ignore_st;
    uuid_t		type_uuid;
    rpc_syntax_id_t	syntax;
    unsigned32		data_len;
    unsigned8		version;

    if (in_authdata == NULL || fetched_authdata == NULL ||
        (in_authdata->ad_type != OSF_DCE_KRB_AUTHDATA) ||
        (fetched_authdata->ad_type != OSF_DCE_KRB_AUTHDATA))
        goto false_out;

    /*
     * If they are the same length, they must be an exact match!
     */
    if ((in_authdata->length == fetched_authdata->length) &&
        (memcmp((char *)in_authdata->contents,
                (char *)fetched_authdata->contents,
                in_authdata->length) == 0))
        return true;

    /*
     * Take a first shot at comparing the authdata. If this fails,
     * see if a V1 pac exists at the front of the authdata and
     * scan past it before comparing again.
     */
    if (memcmp((char *)in_authdata->contents,
               (char *)fetched_authdata->contents, in_authdata->length) == 0)
        return true;

    /* 
     * scan past V1 authdata, if any 
     * First we have to figure out the length of the pickled v1 pac, if
     * any, at the head of the authorization data.  The first 4 bytes beyond
     * that contain the length of the encoded epac chain, and the encoded
     * epac chain itself begins at the next 8-byte boundary.
     */

    /*
     * If there is a v1 PAC at the head of the authorization data, then
     * the authorization data would have to be at least as long as a
     * v1 pickle header.
     */
    if (fetched_authdata->length > idl_pkl_data_offset) {
        idl_pkl_t  *pkl = (idl_pkl_t *)fetched_authdata->contents;
        unsigned32 pkl_len;

        /*
         * Treat the authorization data stream as a v1 pickled
         * PAC and pull out the v1 pickle header.
         */
        idl_pkl_get_header(*pkl, &version,
                           &data_len,  &syntax, &type_uuid);

        /*
         * make sure that we really are dealing with a v1 pickle
         * header by checking the type uuid.  If so, advance the
         * marshalling pointer to the first 8-byte offset beyond
         * the pickled pac, otherwise return false.
         */
        if (uuid_equal(&type_uuid, &sec__g_v0_authzd_compat, &ignore_st)) {
            unsigned32 pkl_len;

            /*
             * find the end of the v1 pickled PAC contents, adjusted
             * for 8-byte alignment
             */
            pkl_len = idl_pkl_data_offset + data_len;
            pkl_len = (pkl_len + 7) & ~7;
            mp = fetched_authdata->contents + pkl_len;

        } else {
            /* 
             * DCE authdata we just don't understand.
             */
            goto false_out;
        }
    } else {
        /*
         * There is definitely not enough room for a V1 PAC, so this is
         * DCE authdata we just don't understand.
         */
        goto false_out;
    }

    /*
     * Assuming the contents field of the krb5 authdata is 8-byte
     * aligned, then our marshalling pointer will be too.
     */

    /*
     * if the marshalling pointer points past the end of
     * krb5_authdata, then something is badly wrong, so just
     * return false
     */
    if (mp > fetched_authdata->contents + fetched_authdata->length) {
        goto false_out;
    }

    /* 
     * Now, if we have gotten this far, we have scanned past the V1 PAC.
     * We now memcmp what is left.
     */
    if (memcmp((char *)in_authdata->contents,
               (char *)mp, in_authdata->length) == 0)
        return true;

false_out:
    DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
        "priv_krb_util_match_dce_authdata: match failed\n"));
    return false;
}

/*
 * priv_krb_util_parse_v1_ptgt_authdata
 *
 * Parse an on-disk PTGT credentials cache authorization
 * data of type DCE, version 1.
 *
 * Input:
 *	v1_ptgt_authdata: krb5 authorization data
 * Output:
 *	ppac: The V1 pac, if it exists, stored at the head of
 *		the DCE authdata.
 *	ppac_len: the length of the V1 PAC.
 *	ptgt_uuid: the uuid, if found. If not found, a NIL uuid
 *		   is returned.
 *	stp: error status
 *
 * Returns: TRUE, if the DCE authdata is of version 1 format,
 *		FALSE otherwise.
 */
boolean32 priv_krb_util_parse_v1_ptgt_authdata(
    krb5_authdata	**v1_ptgt_authdata,	/* [in]  */
    idl_pkl_t		**ppac,			/* [out] */
    unsigned32		*ppac_len,		/* [out] */
    uuid_t		*ptgt_uuid,		/* [out] */
    error_status_t	*stp			/* [out] */
)
{
    boolean32		more;
    krb5_authdata	*dce_authdata;
    uuid_t		type_uuid;
    rpc_syntax_id_t	syntax;
    unsigned32		data_len;
    unsigned8		vno, version;
    error_status_t	ignore_st;
    boolean32		free_ppac = false;
    unsigned char	*mp;

    CLEAR_STATUS(stp);
    if (v1_ptgt_authdata == NULL || *v1_ptgt_authdata == NULL)
	return false;
    if (ppac && ppac_len) {
        *ppac = NULL;
        *ppac_len = 0;
    }
    if (ptgt_uuid)
	uuid_create_nil(ptgt_uuid, &ignore_st);

    dce_authdata = priv_krb_util_get_dce_authdata(v1_ptgt_authdata, &more);
    if (dce_authdata == NULL)
	return false;
    else
	mp = dce_authdata->contents;

    /*
     * If there is a v1 PAC at the head of the authorization data, then
     * the authorization data would have to be at least as long as a
     * v1 pickle header.
     */
    if (dce_authdata->length > idl_pkl_data_offset) {
        idl_pkl_t  *pkl = (idl_pkl_t *)dce_authdata->contents;
        unsigned32 pkl_len;

        /*
         * Treat the authorization data stream as a v1 pickled
         * PAC and pull out the v1 pickle header.
         */
        idl_pkl_get_header(*pkl, &version,
                           &data_len,  &syntax, &type_uuid);

        /*
         * make sure that we really are dealing with a v1 pickle
         * header by checking the type uuid.  If so, advance the
         * marshalling pointer to the first 8-byte offset beyond
         * the pickled pac, otherwise return false.
         */
        if (uuid_equal(&type_uuid, &sec__g_v0_authzd_compat, &ignore_st)) {
            unsigned32 pkl_len;

            /*
             * find the end of the v1 pickled PAC contents, adjusted
             * for 8-byte alignment
             */
            pkl_len = idl_pkl_data_offset + data_len;
            pkl_len = (pkl_len + 7) & ~7;
	    if (ppac && ppac_len) {
		*ppac_len = pkl_len;
		pkl = malloc(pkl_len);
		if (pkl == NULL) {
		    SET_STATUS(stp, sec_s_no_memory);
		    return false;
		}
		memcpy(pkl, mp, pkl_len);
		*ppac = pkl;
		free_ppac = true;
	    }
            mp = dce_authdata->contents + pkl_len;
        }
    }

    /*
     * if the marshalling pointer points past the end of
     * krb5_authdata, then something is badly wrong, so just
     * return false
     */
    if (mp > dce_authdata->contents + dce_authdata->length)
	goto ptgt_out;

    /* Check if we are dealing with version 1 ptgt authorization data */
    vno = (*(unsigned8 *)mp);
    if (vno != priv_krb_def_ptgt_authdata_vno)
	goto ptgt_out;
    mp += sizeof(priv_krb_def_ptgt_authdata_vno);

    /*
     * if the marshalling pointer points at or past the end of
     * krb5_authdata, then there is no uuid.
     */
    if (mp >= dce_authdata->contents + dce_authdata->length) {
        return true;
    }

    if (ptgt_uuid) {
        /*
         * The rest of the data should contain a uuid.
         * The length begins at the next 4-byte boundary.
         */
	mp += 3;
	memcpy(ptgt_uuid, mp, sizeof(*ptgt_uuid));
    }
    return true;
ptgt_out:
    if (free_ppac) {
        free(*ppac);
	*ppac = NULL;
        *ppac_len = 0;
    }
    return false;
}

/*
 * priv_krb_util_construct_v1_ptgt_authdata
 *
 * Construct an on-disk PTGT credentials cache authorization
 * data of type DCE, version 1.
 *
 * Input:
 *	ppac: The V1 pac (optional) stored at the head of
 *		the DCE authdata.
 *	ppac_len: the length of the V1 PAC.
 *	ptgt_uuidr: the uuid (optional)
 *	extra_authdata: any additional krb5 authdata
 * Output:
 *	v1_ptgt_authdata: the krb5_authdata array, containing
 *		the DCE v1 ptgt authdata and any additional krb5 authdata
 *		specified in extra_authdata.
 *	v1_ptgt_authbytes: the DCE v1 ptgt authdata in sec_bytes_t format
 *	stp: error status
 */
void priv_krb_util_construct_v1_ptgt_authdata(
    idl_pkl_t           *ppac,			/* [in]  */
    unsigned32          ppac_len,		/* [in]  */
    uuid_t		*ptgt_uuid,		/* [in]  */
    krb5_authdata	**extra_authdata,	/* [in]  */
    krb5_authdata	***v1_ptgt_authdata,	/* [out] */
    sec_bytes_t		*v1_ptgt_authbytes,	/* [out] */
    error_status_t	*stp			/* [out] */
)
{
    sec_bytes_t		authbytes;
    unsigned char	*mp;

    CLEAR_STATUS(stp);

    memset(&authbytes, 0, sizeof(authbytes));

    authbytes.num_bytes = sizeof(priv_krb_def_ptgt_authdata_vno);
    if (ppac != NULL && ppac_len > 0)
	authbytes.num_bytes += ppac_len;
    if (ptgt_uuid != NULL) {
	authbytes.num_bytes += sizeof(*ptgt_uuid) + 3;
    }
	
    authbytes.bytes = malloc(authbytes.num_bytes);
    if (authbytes.bytes == NULL) {
	SET_STATUS(stp, sec_s_no_memory);
	return;
    }
    memset(authbytes.bytes, 0, authbytes.num_bytes);

    mp = authbytes.bytes;
    if (ppac != NULL && ppac_len > 0) {
        memcpy(mp, ppac, ppac_len);
        mp += ppac_len;
    }
    memcpy(mp, &priv_krb_def_ptgt_authdata_vno, sizeof(priv_krb_def_ptgt_authdata_vno));
    if (ptgt_uuid != NULL) {
        mp += sizeof(priv_krb_def_ptgt_authdata_vno) + 3;
	memcpy(mp, ptgt_uuid, sizeof(*ptgt_uuid));
    }
 
    priv_krb_util_bytes_to_authdata(&authbytes, extra_authdata, v1_ptgt_authdata, stp);
    if (STATUS_OK(stp) && v1_ptgt_authbytes != NULL)
	*v1_ptgt_authbytes = authbytes;
    else
        free(authbytes.bytes);
}

/*
 * priv_krb_util_parse_v1_stkt_authdata
 *
 * Parse an on-disk STKT credentials cache authorization
 * data of type DCE, version 1.
 *
 * Input:
 *	v1_stkt_authdata: krb5 authorization data
 * Output:
 *	stkt_uuid: the uuid, if found. Returns a nil uuid if
 *		   not found.
 *	stp: error status
 *
 * Returns: TRUE, if the DCE STKT authdata is of version 1 format,
 *		FALSE otherwise.
 */
boolean32 priv_krb_util_parse_v1_stkt_authdata(
    krb5_authdata	**v1_stkt_authdata,	/* [in]  */
    uuid_t		*stkt_uuid,		/* [out] */
    error_status_t	*stp			/* [out] */
)
{
    krb5_authdata	*dce_authdata;
    boolean32		more;
    unsigned8		vno;
    unsigned char	*mp;
    error_status_t	lst;	

    CLEAR_STATUS(stp);
    if (v1_stkt_authdata == NULL || *v1_stkt_authdata == NULL)
	return false;
    if (stkt_uuid)
	uuid_create_nil(stkt_uuid, &lst);

    dce_authdata = priv_krb_util_get_dce_authdata(v1_stkt_authdata, &more);
    if (dce_authdata == NULL)
	return false;
    else
	mp = dce_authdata->contents;

    /* Check if we are dealing with version 1 stkt authorization data */
    vno = (*(unsigned8 *)mp);
    if (vno != priv_krb_def_stkt_authdata_vno)
	return false;
    mp += sizeof(priv_krb_def_stkt_authdata_vno);

    /*
     * if the marshalling pointer points at or past the end of
     * krb5_authdata, then there is no uuid.
     */
    if (mp >= dce_authdata->contents + dce_authdata->length) {
        return true;
    }

    if (stkt_uuid) {
        /*
         * The rest of the data should contain a uuid. 
         * The length starts at the next 4-byte boundary.
         */
	mp += 3;
	memcpy(stkt_uuid, mp, sizeof(*stkt_uuid));
    }
    return true;
}

/*
 * priv_krb_util_construct_v1_stkt_authdata
 *
 * Construct an on-disk STKT credentials cache authorization
 * data of type DCE, version 1.
 *
 * Input:
 *	stkt_uuid_str: the uuid (optional)
 *	extra_authdata: any additional krb5 authdata
 * Output:
 *	v1_stkt_authdata: the krb5_authdata array, containing
 *		the DCE v1 stkt authdata and any additional krb5 authdata
 *		specified in extra_authdata.
 *	stp: error status
 */
void priv_krb_util_construct_v1_stkt_authdata(
    uuid_t		*stkt_uuid,		/* [in]  */
    krb5_authdata	**extra_authdata,	/* [in]  */
    krb5_authdata	***v1_stkt_authdata,	/* [out] */
    error_status_t	*stp			/* [out] */
)
{
    sec_bytes_t		authbytes;
    unsigned char	*mp;

    CLEAR_STATUS(stp);

    memset(&authbytes, 0, sizeof(authbytes));

    authbytes.num_bytes = sizeof(priv_krb_def_stkt_authdata_vno);
    if (stkt_uuid != NULL) {
	authbytes.num_bytes += sizeof(*stkt_uuid) + 3;
    }

    authbytes.bytes = malloc(authbytes.num_bytes);
    if (authbytes.bytes == NULL) {
	SET_STATUS(stp, sec_s_no_memory);
	return;
    }
    memset(authbytes.bytes, 0, authbytes.num_bytes);

    mp = authbytes.bytes;
    memcpy(mp, &priv_krb_def_stkt_authdata_vno, sizeof(priv_krb_def_stkt_authdata_vno));
    if (stkt_uuid != NULL) {
	mp += sizeof(priv_krb_def_stkt_authdata_vno) + 3;
	memcpy(mp, stkt_uuid, sizeof(*stkt_uuid));
    }

    priv_krb_util_bytes_to_authdata(&authbytes, extra_authdata, v1_stkt_authdata, stp);
    /* done with authbytes, in any event */
    free(authbytes.bytes);
}

/*
 * priv_krb_util_compare_authdata_uuids
 *
 * Given version 1 DCE authdata, parse out the uuid contained
 * within (if it exists) and compare them. If equal, return true,
 * otherwise return false.
 *
 * Input:
 *	ptgt_authdata: version 1 DCE ptgt authdata
 *	dce_authdata: version 1 DCE ptgt or stkt authdata
 *	both_ptgts: flag indicating whether dce_authdata is PTGT authdata
 * Output:
 *	stp: error status
 *
 * NOTE: If neither authdata contains a uuid, true will be returned
 * because this case is considered to be a match (nil uuids).
 */ 
boolean32 priv_krb_util_compare_authdata_uuids(
    krb5_authdata	**ptgt_authdata,	/* ptgt authdata */
    krb5_authdata	**dce_authdata,		/* ptgt or stkt authdata */
    boolean32		both_ptgts,
    error_status_t	*stp
)
{
    boolean32		v1_found = false, match = false;
    uuid_t		ptgt_uuid, dce_uuid;

    CLEAR_STATUS(stp);

    if (priv_krb_util_parse_v1_ptgt_authdata(ptgt_authdata, NULL,
		NULL, &ptgt_uuid, stp) && STATUS_OK(stp)) {

	if (both_ptgts)
	    v1_found = priv_krb_util_parse_v1_ptgt_authdata(dce_authdata,
		NULL, NULL, &dce_uuid, stp);
	else
	    v1_found = priv_krb_util_parse_v1_stkt_authdata(dce_authdata,
		&dce_uuid, stp);
    }

    if (v1_found && STATUS_OK(stp)) {
    	if (uuid_equal(&ptgt_uuid, &dce_uuid, stp))
	    match = true;
    }

    return match;
}
