/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: priv_krb_util.h,v $
 * Revision 1.1.6.2  1996/02/18  22:58:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:59  marty]
 *
 * Revision 1.1.6.1  1995/12/13  16:24:38  root
 * 	Submit
 * 
 * 	HP revision /main/HPDCE02/3  1995/08/22  21:07 UTC  mullan_s
 * 	Merge fix for CHFts15637
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts15637/6  1995/08/21  21:01 UTC  mullan_s
 * 	Change comments regarding on-disk authdata format.
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts15637/5  1995/08/15  17:10 UTC  mullan_s
 * 	In v1 authdata, store/parse UUID as bytes instead of string.
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts15637/4  1995/08/11  15:27 UTC  mullan_s
 * 	Add more comments to describe the PTGT/STKT authdata.
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts15637/3  1995/08/10  18:20 UTC  mullan_s
 * 	Add priv_krb_util_compare_authdata_uuids.
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts15637/2  1995/08/09  14:24 UTC  mullan_s
 * 	Add more comments to describe format of authdata.
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts15637/1  1995/07/26  18:04 UTC  mullan_s
 * 	Add priv_krb_util_epac_search
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/22  17:04 UTC  mullan_s
 * 	Merge
 * 
 * 	HP revision /main/HPDCE02/mullan_mothra_bin_compat9/1  1995/05/22  15:41 UTC  mullan_s
 * 	Add login context parameter to priv_krb_util_fetch_v1_1_ccache_authdata.
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/03  22:03 UTC  mullan_s
 * 	Binary Compatibility Merge
 * 	[1995/12/11  15:14:21  root]
 * 
 * Revision 1.1.4.3  1994/08/18  20:25:25  greg
 * 	Add new authorization data processing routines and remove
 * 	obsolete FAKE-EPAC routines.
 * 	[1994/08/16  18:52:18  greg]
 * 
 * Revision 1.1.4.2  1994/06/10  15:07:14  greg
 * 	intercell fixes for DCE1.1 beta
 * 	[1994/06/03  20:48:54  greg]
 * 
 * Revision 1.1.4.1  1994/01/28  23:10:31  burati
 * 	Delegation/EPAC changes (dlg_bl1)
 * 	[1994/01/18  21:52:23  burati]
 * 
 * Revision 1.1.2.2  1992/12/29  13:06:55  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:14:31  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:42:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  priv_krb_util.h
**
** Copyright (c) Hewlett-Packard Company 1991, 1993
** Unpublished work. All Rights Reserved.
**
*/
/* 
 * priv_krb_util.h: common kerberos-based priv server routines
 */ 

#ifndef __priv_krb_util_h__included
#define __priv_krb_util_h__included

#include <dce/id_base.h>
#include <dce/lbase.h>
#include <dce/pklbase.h>
#include <dce/authz_base.h>
#include <dce/sec_login.h>
#include <krb5/base-defs.h>
#include <krb5/krb5.h>
#include <sec_login_db.h>

/*
 * These are 8 bit version numbers used in the authorization
 * data of EPTGTs and service tickets to identify what release
 * of DCE they are associated with. There is currently only
 * one version for each. In an EPTGT, the version is placed immediately 
 * after a v1 pac in the authorization data. This distinguishes
 * it as DCE 1.1 PTGT, as opposed to DCE 1.0 PTGT. In a service ticket,
 * it is placed at the very beginning of the authorization data. Again, 
 * this distinguishes it as a DCE 1.1 service ticket, as opposed to
 * a DCE 1.0 service ticket.
 *
 * Anytime we need to distinguish service tickets or PTGTs from one
 * release to the next, a new version needs to be added.
 *
 * Format:		       {  optional      } 
 *	  --------------------------------------
 * PTGT: |V1 PAC (optional)|vno| buffer    |uuid| 
 *	  --------------------------------------
 *       ('+   +'==1byte)  +   +   +   +   +   
 *	 if vno == 1, the vno is followed by a optional uuid.
 *
 *	     { optional       }
 *        --------------------
 * STKT: |vno| buffer    |uuid|
 *	  --------------------
 *       +   +   +   +   +   
 *	 if vno == 1, the vno is followed by a optional uuid.
 *
 * The uuid is used to correctly match a PTGT/STKT pair to
 * avoid mismatched seal errors.
 */
#define PRIV_KRB_PTGT_AUTHDATA_VNO_1 0x01	/* DCE 1.1 */
#define PRIV_KRB_STKT_AUTHDATA_VNO_1 0x01	/* DCE 1.1 */
extern unsigned8 priv_krb_def_stkt_authdata_vno;
extern unsigned8 priv_krb_def_ptgt_authdata_vno;

/* p r i v _  k r b _ u t i l _  p a c _ t o _  a u t h d a t a
 *
 * take a pickled pac and tranform it into a krb5_auth_data array
 * suitable for inclusion in a krb5 ticket.  The result includes 
 * any "extra" authdata passed in.  Passing NULL in the extra
 * auhtdata parameter will prevent this from occuring.
 *
 * E.G.
 *
 *   sec_id_pickled_pac_t  ppac;
 *   krb5_authdata         **authdata;
 *   error_status_t        st;
 *
 *  priv_krb_util_pac_to_authdata(&ppac, NULL, &authdata, &st);
 *
 */
void  priv_krb_util_pac_to_authdata ( 
#ifdef __STDC__
    idl_pkl_t             *ppac,              /* [in] */
    unsigned32            ppac_len,           /* [in] */
    krb5_authdata         **extra_authdata,   /* [in] */
    krb5_authdata         ***authdata,        /* [out] */
    error_status_t        *stp                /* [out] */
#endif
);

/* p r i v _ k r b _ u t i l _ b y t e s _ t o _ a u t h d a t a
 *
 * take a sec_bytes_t and transform it into a krb5_auth_data array
 * suitable for inclusion in a krb5 ticket.  The result includes
 * any "extra" authdata passed in.  passing NULL for in the extra
 * auhtdata parameter will prevent this from occuring
 */
void priv_krb_util_bytes_to_authdata(
    sec_bytes_t           *authbytes,        /* [in]  */
    krb5_authdata         **extra_authdata,  /* [in]  */
    krb5_authdata         ***authdata,       /* [out] */
    error_status_t        *stp               /* [out] */
);

/* p r i v _ k r b _ u t i l _  a u t h d a t a _ t o _ p a c
 *
 * Basically the inverse of priv_util_krb_pac_to_authdata.
 * Storage is allocated for a copy of the authdata pac (if any)
 * and must be freed by the caller.  If there is no authdata
 * of the the OSF DCE registered type in the input authdata
 * vector, the returned pointer is NULL.
 */

void  priv_krb_util_authdata_to_pac ( 
#ifdef __STDC__
    krb5_authdata         **authdata,   /* [in] */
    idl_pkl_t             **ppac,       /* [out] */
    error_status_t        *stp
#endif
);

void  priv_krb_util_pac_to_enc_authdata ( 
#ifdef __STDC__
    idl_pkl_t             *ppac,              /* [in] */
    unsigned32            ppac_len,           /* [in] */
    krb5_authdata         **extra_authdata,   /* [in] */
    krb5_encrypt_block    *eblock,            /* [in] */
    krb5_enc_data         *enc_authdata,      /* [out] */
    error_status_t        *stp
#endif
);

void priv_krb_util_delete_dce_authdata(
    krb5_authdata       **in_authdata,
    krb5_authdata       ***out_authdata,
    error_status_t      *stp
);

/*
 * takes an encrypted data string and converts it to a decrypted, decoded
 * krb5_authdata array.  If one the authdata entries is of the OSF DCE
 * registered type, the results are unpickled and returned in the
 * ppac parameter.  Passing NULL for either the ppac or authdata parameter
 * will prevent that value from being returned.
 */
void  priv_krb_util_enc_authdata_to_pac ( 
#ifdef __STDC__
    krb5_enc_data         *enc_authdata,  /* [in] */
    krb5_encrypt_block    *eblock,        /* [in] */
    idl_pkl_t             **ppac,         /* [out] */
    krb5_authdata         ***authdata,    /* [out] */
    error_status_t        *stp
#endif
);

krb5_authdata *priv_krb_util_get_dce_authdata (
    krb5_authdata **authdata,
    boolean32     *more
);

/*
 * a flag set indicating which of the possible types 
 * of DCE authorization data are actually present
 * in a kerberos V5 ticket.
 */
#define priv_ad_part_none     0x1  /* no dce authdata whatsoever */
#define priv_ad_part_unknown  0x2  /* dce authdata that we don't understand,
                                      presumably from some newer version
                                      of security that has chosen not to
                                      maintain backward compatibility with 
                                      version 1.1 */
#define priv_ad_part_pac      0x4  /* a pickled 1.0 PAC */
#define priv_ad_part_v1_1     0x8  /* a sec_v1_1_authz_data_t structure */

/*
 * Check krb5_authdata for the presence of v1_1-style
 * authorization data as well as pre1-1 compatibility 
 * info (a PAC).  If the caller passes NULL for either
 * the v1_1_ad or ppac parameters, the routine will
 * not look for authdata of that type.
 *
 * The function return value is a flag set with the
 * appropriate flag bits set according to the 
 * type(s) of authorization data actually found.
 * 
 * The caller MUST provide valid alloc and dealloc
 * functions.  The prototypes for these function
 * parameters are equivalent to the prototypes for
 * the idl stub support memory routines.  If there is
 * any possibility of type conflicts between the
 * prototypes used here and the prototypes of the
 * memory management functions being passed, then
 * the caller should write wrapper functions that
 * use the correct prototype as specified here,
 * and call the actual functtion from within the 
 * wrapper (using the appropriate type coercion).  
 */
unsigned32  priv_krb_util_parse_v0_authdata (
    idl_void_p_t           (*alloc)(idl_size_t size),     /* [in] */
    void                   (*dealloc)(idl_void_p_t ptr),  /* [in] */
    krb5_authdata          **authdata,                    /* [in] */
    sec_v1_1_authz_data_t  *v1_1_ad,                      /* [out] */
    idl_pkl_t              **ppac,                        /* [out] */
    error_status_t         *stp                           /* [out] */
);

void  priv_krb_util_gen_v0_authdata (
    idl_void_p_t           (*alloc)(idl_size_t size),     /* [in] */
    void                   (*dealloc)(idl_void_p_t ptr),  /* [in] */
    sec_v1_1_authz_data_t  *v1_1_authz_data,              /* [in] */
    sec_id_pac_t           *v1_pac,                       /* [in] */
    krb5_authdata          ***authdata,                   /* [out] */
    error_status_t         *stp                           /* [out] */
);

void priv_krb_util_gen_v1_pac(
    idl_void_p_t           (*alloc)(idl_size_t size),     /* [in] */
    void                   (*dealloc)(idl_void_p_t ptr),  /* [in] */
    sec_bytes_t            *epac_chain,                   /* [in] */
    idl_pkl_t              **v1_pac,                      /* [out] */
    unsigned32             *v1_pac_len,                   /* [out] */
    error_status_t         *stp                           /* [out] */
);

/*
 * Client-only routines
 * 
 * Used for generating DCE authorization data of the sort
 * that goes into krb5 credential cache (E)PTGT entries.
 *
 * Definitely NOT of any use for manipulating the DCE authorization
 * data found in service tickets.
 *
 * See function headers in priv_krb_util.c for details.
 */
void  priv_krb_util_gen_v1_1_ccache_authdata(
    sec_bytes_t            *epac_chain,                   /* [in] */
    boolean32              gen_v1_pac,                    /* [in] */
    sec_bytes_t		   *privs,			  /* [out] */
    krb5_authdata          ***authdata,                   /* [out] */
    error_status_t         *stp                           /* [out] */
);
 
/* 
 * boolean return value is meaningful only if the error status is
 * OK.  If false, then the credential cache either contains no
 * DCE authdata at all, or the DCE authdata that is there contains
 * no 1.1 credentials (this would be the case if the security servers
 * in the cell were pre 1.1 servers)
 */
boolean32  priv_krb_util_parse_v1_1_ccache_authdata(
    idl_void_p_t           (*alloc)(idl_size_t size),     /* [in] */
    void                   (*dealloc)(idl_void_p_t ptr),  /* [in] */
    krb5_authdata          **authdata,                    /* [in] */
    sec_bytes_t            *epac_chain,                   /* [out] */
    error_status_t         *stp                           /* [out] */
);

boolean32 priv_krb_util_fetch_v1_1_ccache_authdata(
    sec_login_handle_t	   login_context,		  /* [in] */
    idl_void_p_t           (*alloc)(idl_size_t size),     /* [in] */
    void                   (*dealloc)(idl_void_p_t ptr),  /* [in] */
    char                   *datafile,                     /* [in] */
    unsigned_char_t        *target_cell,                  /* [in] */
    sec_bytes_t            *epac_chain,                   /* [out] */
    sec_bytes_t		   *authbytes,			  /* [out] */
    error_status_t         *stp                           /* [out] */
);

boolean32 priv_krb_util_epac_search(
    sec_login_db_entry_t   db_entry,			  /* [in]  */
    idl_void_p_t           (*alloc)(idl_size_t size),     /* [in]  */
    void                   (*dealloc)(idl_void_p_t ptr),  /* [in]  */
    unsigned_char_t	   *target_cell,		  /* [in]  */
    sec_bytes_t		   *epac_chain,			  /* [out] */
    sec_bytes_t		   *authbytes,			  /* [out] */
    error_status_t         *stp                           /* [out] */
);

boolean32 priv_krb_util_match_dce_authdata(
    register const krb5_authdata        *in_authdata,	  /* [in] */
    register const krb5_authdata        *fetched_authdata /* [in] */
);

void priv_krb_util_replace_dce_authdata(
    krb5_authdata   			*dce_authdata,	  /* [in] */
    krb5_authdata   			**authdata,	  /* [in,out] */
    error_status_t  			*stp		  /* [out] */
);

boolean32 priv_krb_util_parse_v1_ptgt_authdata(
    krb5_authdata       **v1_ptgt_authdata,     /* [in]  */
    idl_pkl_t           **ppac,                 /* [out] */
    unsigned32		*ppac_len,		/* [out] */
    uuid_t              *ptgt_uuid,             /* [out] */
    error_status_t      *stp                    /* [out] */
);

void priv_krb_util_construct_v1_ptgt_authdata(
    idl_pkl_t           *ppac,                  /* [in]  */
    unsigned32          ppac_len,               /* [in]  */
    uuid_t              *ptgt_uuid,             /* [in]  */
    krb5_authdata	**extra_authdata,	/* [in]  */
    krb5_authdata       ***v1_ptgt_authdata,    /* [out] */
    sec_bytes_t		*v1_ptgt_authbytes,	/* [out] */
    error_status_t      *stp                    /* [out] */
);

boolean32 priv_krb_util_parse_v1_stkt_authdata(
    krb5_authdata       **v1_stkt_authdata,     /* [in]  */
    uuid_t              *stkt_uuid,             /* [out] */
    error_status_t      *stp                    /* [out] */
);

void priv_krb_util_construct_v1_stkt_authdata(
    uuid_t              *stkt_uuid, 	        /* [in]  */
    krb5_authdata	**extra_authdata,	/* [in]  */
    krb5_authdata       ***v1_stkt_authdata,    /* [out] */
    error_status_t      *stp                    /* [out] */
);

boolean32 priv_krb_util_compare_authdata_uuids(
    krb5_authdata	**ptgt_authdata,
    krb5_authdata	**dce_authdata,
    boolean32		both_ptgts,
    error_status_t	*stp
);

#endif
