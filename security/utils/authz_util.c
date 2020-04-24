/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: authz_util.c,v $
 * Revision 1.1.6.3  1996/02/18  00:22:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:10:09  marty]
 *
 * Revision 1.1.6.2  1995/12/08  18:01:53  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/15  21:12 UTC  greg
 * 	Mergeout from HPDCE02 to SEC_migrate_merge.
 * 	[1995/05/13  02:41 UTC  greg  /main/SEC_migrate_merge/2]
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/08  21:12 UTC  mullan_s
 * 	HPDCE02 branch
 * 
 * 	HP revision /main/mullan_mothra_bin_compat4/1  1995/05/08  19:45 UTC  mullan_s
 * 	Use sec_svc_handle instead of NULL when calling dce_assert
 * 	[1995/12/08  17:22:52  root]
 * 
 * Revision 1.1.2.3  1994/08/18  20:25:41  greg
 * 	Add routines for (un)marshalling authorization
 * 	data according to the real 1.1 authorization
 * 	data protocol.
 * 	[1994/08/16  20:16:34  greg]
 * 
 * Revision 1.1.2.2  1994/07/06  20:27:13  sommerfeld
 * 	[OT10607] pick up greg's change to the seal creation functions.
 * 	[1994/07/06  19:38:38  sommerfeld]
 * 
 * Revision 1.1.2.1  1994/06/02  21:17:54  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:20:48  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * $EndLog$
 */

/*
 *  OSF DCE Version 1.1
 */

/*
** Copyright (c) Hewlett-Packard Company 1994
** Unpublished work. All Rights Reserved.
*/

#include <pthread.h>
#include <dce/sec_authn.h>
#include <dce/assert.h>
#include <sec_cred_internal.h>
#include <rgymacro.h>
#include <sec_encode.h>
#include <pkl.h>
#include <sec_pkl.h>
#include <sec_id_pac_util.h>

#define PTHREAD_EXC        /* make one version of pthread_exc.h harmless */
#define PTHREAD_EXC_H      /* make another version of pthread_exc.h harmless */
#include <dce/stubbase.h>  /* includes pthread_exc.h */

#include <authz_util.h> 

/*
 * The following authorization data type uuids are 
 * architectural.  They cannot be changed
 * without destroying interoperability
 */
/*
 * compat type uuid *MUST* be the same as the
 * sec_id_pac_t_typeid defined in sec_pkl.h.
 */ 
uuid_t  sec__g_v0_authzd_compat =  {
    /* d9f3bd98-567d-11ca-9ec6-08001e022936 */
    0xd9f3bd98,
    0x567d,
    0x11ca,
    0x9e,
    0xc6,
    {0x08, 0x00, 0x1e, 0x02, 0x29, 0x36}
};

uuid_t  sec__g_v0_authzd_no_compat = { 
    /* 9b143cec-9a90-11cd-8b27-0800097086bf */
    0x9b143cec, 
    0x9a90, 
    0x11cd, 
    0x8b, 
    0x27,
    { 0x08, 0x00, 0x09, 0x70, 0x86, 0xbf }
};

/*
**++
**
**  ROUTINE NAME:      authz_padding
**
**  SCOPE:  static
**
**  DESCRIPTION: 
**
**      Given a message length, and an aligment requirement, calulate the 
**      number of padding bytes needed to pad the message out to the next
**      byte boundary satisfying the alignment requirements 
**      
**  INPUTS:
**
**      length          message length
**
**      align           alignment requirements
**   
**  INPUTS/OUTPUTS:
**
**  OUTPUTS:            
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     unsigned32
**  
**      The number of bytes needed to pad a message of length bytes to the
**      byte boundary satisfying the caller's aligment requirenments.
**
**  SIDE EFFECTS:       none
**
**--
**/

static unsigned32  authz_padding (
    unsigned32 len,
    unsigned32 align
)
{
    unsigned32 pad;

    pad = len % align;
    return ((pad == 0) ? pad : (align - pad));
}

/*
 * Caller must pass in a valid and consistent
 * memory allocator/deallocator pair.  We use
 * prototypes for the idl versions of memory
 * management routines, so beware of argument
 * precision differences between the idl
 * protoypes and the prototypes of "native" 
 * malloc()/free() functions.  E.G. if the
 * native malloc() routine takes an argument
 * of different precision than an idl_size_t
 * (and it does, at least on HP/UX, then pass
 * in a "wrapper" allocator routine that 
 * takes in a idl_size_t and coerces it
 * to the appropriate type for the native
 * call.  Take a look at the malloc_shim
 * routine declared/defined in sec_encode.[hc].
 * It's provided for this very purpose.
 */
PUBLIC  void  sec_authz_util_gen_seal (
    idl_void_p_t (*alloc)(idl_size_t size),     /* [in] */
    void         (*dealloc)(idl_void_p_t ptr),  /* [in] */
    sec_bytes_t   *delegation_chain,            /* [in] */
    sec_id_seal_t *seal                         /* [out] */
)
{
    sec_md_struct             mds;
    
    sec_md_begin(&mds);
    sec_md_update(&mds, delegation_chain->bytes, delegation_chain->num_bytes);
    sec_md_final(&mds);

    seal->seal_type = sec_id_seal_type_md5;
    seal->seal_len = sizeof(mds.digest);
    seal->seal_data = (*alloc)(seal->seal_len);
    memcpy(seal->seal_data, mds.digest, seal->seal_len);
}

PUBLIC int sec_authz_util_eq_seal (
    sec_id_seal_t *a,
    sec_id_seal_t *b
)
{
    return ((a->seal_type == b->seal_type) &&
	    (a->seal_len == b->seal_len) &&
	    (memcmp(a->seal_data, b->seal_data, a->seal_len) == 0));
}

/*
 * Make sure the deallocator is consistent with whatever
 * allocator was used to allocate the seal.
 */
void sec_id_seal_free (
    void         (*dealloc)(idl_void_p_t ptr),  /* [in] */
    sec_id_seal_t *seal                         /* [in/ out] */
) 
{
    if (seal && seal->seal_len && seal->seal_data) {
	(*dealloc) (seal->seal_data);
    }

}

/*
**++
**
**  ROUTINE NAME:      sec__authz_util_get_authzd_type
**
**  DESCRIPTION:   
**  
**      Extract the authorization data type uuid from 
**      a DCE authorization data stream.
**
**  INPUTS:
**
**      authz_data      the authorization data stream.
**
**  INPUTS/OUTPUTS:
**
**  OUTPUTS:            
**
**      type            the authorization data type uuid.
**                      In the unlikely event that the
**                      uuid cannot be extracted, the returned
**                      uuid will be NIL.  More likely, if the
**                      caller hands us garbage, we will just
**                      convert that garbage to a uuid.
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

void sec__authz_util_get_authzd_type (
    sec_bytes_t  *authz_data,   /* [in] */
    uuid_t       *type          /* [out] */
)
{
    error_status_t  ignore_st;
    idl_pkl_t       *pkl = (idl_pkl_t *)authz_data->bytes;

    uuid_create_nil(type, &ignore_st);

    /* NULL or zero-length authorization data can't have a type */
    if (!(authz_data && authz_data->num_bytes && authz_data->bytes)) {
	return;
    }
    
    /* We only know about version 0 authorization data */
    if (AUTHZ_DATA_VNO(authz_data->bytes) != 0) {
	return;
    }

    /* 
     * Make sure that the v0 authorization data are at least
     * as long as a v0 header, otherwise, there's no point 
     * in trying to get the type uuid
     */
    if (authz_data->num_bytes < v0_authz_header_size) {
	return;
    }

    /* 
     * As we've spec'd DCE authorization data to contain
     * a type uuid at exactly the same location, and in the
     * same format, as a V0 pickle type uuid, we can use 
     * the version 0 pickle routine to extract the type uuid
     * from DCE authorization data
     */
    idl_pkl_get_type_uuid(*pkl, type);
}


/*
**++
**
**  ROUTINE NAME:      sec__authz_util_marsh_v0_authzd
**
**  DESCRIPTION:   
**  
**  Given either a v0 PAC, v1_1 authorization data, or both,
**  generate version 0 DCE authorization data appropriate for inclusion in
**  a kerberos V5 ticket.   See the description of authorization
**  data in security/idl/authz_base.idl for a detailed specification
**  of the v1_1 authorization data format.
**      
**  INPUTS:
**
**      alloc            memory allocation routine
**      dealloc          memory deallocation routine
**      encoded_v1_1_ad  pointer to encoded v1.1 authorization data.  May be
**                           NULL, in which case it is ignored.
**      encoded_pac_len  length of pre-1.1 PAC pickle.  If 0, then no pre-1.1
**                          PAC is included in the marshalled authz_data
**      encoded_pac      a pickled pre-1.1 Privilege Attribute Certificate.  If
**                       pickled_pac_len is zero, this field is ignored
**   
**  INPUTS/OUTPUTS:
**
**  OUTPUTS:            
**
**      authz_data      the resulting authorization data stream.
**      status          A value indicating the return status of the routine:
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
void sec__authz_util_marsh_v0_authzd (
    idl_void_p_t   (*alloc)(idl_size_t size),      /* [in] */
    void           (*dealloc)(idl_void_p_t ptr),   /* [in] */
    sec_bytes_t     *encoded_v1_1_ad,              /* [in] */
    unsigned32       encoded_pac_len,              /* [in] */
    idl_pkl_t       *encoded_pac,                  /* [in] */
    sec_bytes_t     *authz_data,                   /* [out] */
    error_status_t  *stp                           /* [out] */
) 
{
    unsigned32      len = 0, pad = 0, authz_header_len = 0;
    sec_authz_mp_t   mp;
    idl_char         *authz_bytes = NULL,  *authz_header = NULL,
                     no_compat_header[v0_authz_header_size];


    CLEAR_STATUS(stp);
    authz_data->bytes = 0;
    authz_data->num_bytes = 0;

    if (!((encoded_v1_1_ad && encoded_v1_1_ad->num_bytes)
    || (encoded_pac_len && encoded_pac))) {
	/* Caller gave us nothing to marshall (?) so just return */
	return;
    }

    /*
     * we need to construct a V0 authorization data header.
     * If we've been given a pac, the task is simple: the
     * v0 authz data header is just the v0 idl pickle header 
     * in the  encoded pac.  If not, we've got a bit more work to do.
     * See authz_base.idl for a complete description
     * of the v0 authorization data format.
     */

    /*
     * First calculate the required length of the final authz data
     * byte stream.
     */
    if (encoded_pac_len) {
	/* 
	 * Must accomodate the encoded v1_pac and (possibly) 
	 * the encoded v1_1_ad beginning at the first 8 byte
         * offset beyond the encoded v1_pac.  Don't need to
	 * account for the authorization data header separately
	 * because it is part of the encoded v1_pac.
         */
	len = authz_header_len = encoded_pac_len;
        authz_header = &(*encoded_pac)[0];

	if (encoded_v1_1_ad->num_bytes) {
	    pad = authz_padding(len, v0_authz_alignment);
	    len = len + pad + encoded_v1_1_ad->num_bytes;
	}

    } else if (encoded_v1_1_ad->num_bytes) {
	/*
         * No v1 encoded pac so we need to generate a no-compat 
	 * v0 authz header
	 */
	len = authz_header_len = sizeof(no_compat_header);
	pad = authz_padding(sizeof(no_compat_header), v0_authz_alignment);
	len = len + pad + encoded_v1_1_ad->num_bytes;

	memset(no_compat_header, 0 ,sizeof(no_compat_header) );
        
	/*
	 * All we care about in the header is the version field
	 * (byte 0), which should be zero (and is, due to the above memset(),
	 * and the type uuid field (bytes 24-39), which we assign the
	 * no-compat type uuid.  The authorization data protocol
	 * requires that the type  uuid be big-endian,  regardless of local 
	 * architecture.
	 */
	mp = (sec_authz_mp_t) &no_compat_header[AUTHZ_UTIL_V0_TYPE_OFFSET];
	AUTHZ_UTIL_MARSH_BE_UUID(mp, sec__g_v0_authzd_no_compat);
        
	authz_header_len =  sizeof(no_compat_header);
        authz_header = &no_compat_header[0];
    }

    if (len != 0) {

	if (encoded_v1_1_ad->num_bytes) {
	    /* 
	     * add an alignments worth of bytes to hold the encoded epac 
	     * chain length.  We'll use the first 4 bytes for that.
	     */
	    len = len + v0_authz_alignment;
	}

	authz_data->bytes = (*alloc)(len);
	if (authz_data->bytes  == NULL) {
	    SET_STATUS(stp, sec_s_no_memory);
	} 
	   
	if (STATUS_OK(stp)) {
	    mp =  (sec_authz_mp_t) &(authz_data->bytes[0]);
	    memset(mp, 0, len);
	    authz_data->num_bytes = len;

	    /* 
	     * copy the header into the authz byte stream.  The 
	     * header is either an entire encoded v1_pac, or an 
             * an explicitly constructed v0 authorization data header
	     */
	    if (authz_header_len) {
		AUTHZ_UTIL_MARSH_BYTES(mp, authz_header, authz_header_len);
	    }

	    /* advance past padding, if any */
	    AUTHZ_UTIL_ADVANCE_MP(mp, pad);

	    if (encoded_v1_1_ad->num_bytes) {
		AUTHZ_UTIL_MARSH_BE_UNSIGNED32(mp, encoded_v1_1_ad->num_bytes);
		AUTHZ_UTIL_ADVANCE_MP(mp, v0_authz_alignment);

		/* 
		 * copy the encoded v1_1_authorization data after
		 * pad bytes
		 */
		AUTHZ_UTIL_MARSH_BYTES(mp, encoded_v1_1_ad->bytes, encoded_v1_1_ad->num_bytes);
	    }
	}
    }
}

	
/*
**++
**
**  ROUTINE NAME:      sec__authz_util_unmarsh_v0_authzd
**
**  DESCRIPTION:   
**  
**  Given a version 0 DCE authorization data stream, decompose it
**  into it's constituent parts.  There may be an encoded v1 PAC
**  (for compatibility), encoded v1.1 authorization data or both.
**  This routine does not check the type of the pickles found.  It's
**  up to the caller do that.
**      
**  INPUTS:
**
**      alloc            memory allocation routine
**      dealloc          memory deallocation routine
**      authz_data       a v0 authorization data stream.
**   
**  INPUTS/OUTPUTS:
**
**  OUTPUTS:            
**      encoded_v1_1_ad  pointer to encoded v1.1 authorization data (sec_bytes_t).
**                          If the num_bytes field is zero, then no v1.1 
**                          authorization data were present in authz_data.
**                          in which case it is ignored.
**      encoded_pac_len  length of pre-1.1 PAC pickle.  If 0, then no pre-1.1
**                          PAC is present authz_data
**      encoded_pac      a pickled pre-1.1 Privilege Attribute Certificate.
**                          NULL if no pre-1.1 PAC is present authz_data
**      status           A value indicating the return status of the routine
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       
**
**  It is a fatal programming error to call this
**  routine with anything other tnan a v0 authorization
**  data stream.  This is enforced with dce_assert.
**
**  Not exactly a side effect, but, we eliminate the
**  the need for memory allocation and cleanup by
**  handing back pointers directly into the input authorization
**  data stream.  If the caller alters the pointed to storage
**  in any way, very obscure, very bad, side effects will almost
**  certainly ensue.
**--
**/

void sec__authz_util_unmarsh_v0_authzd (
    sec_bytes_t            *authz_data,                   /* [in] */
    sec_bytes_t            *encoded_v1_1_ad,              /* [out] */
    unsigned32             *encoded_pac_len,              /* [out] */
    idl_pkl_t              **encoded_pac,                 /* [out] */
    error_status_t         *stp                           /* [out] */
)
{
    uuid_t                 authzd_type;
    error_status_t         ignore_st;
    sec_authz_mp_t         mp;

    CLEAR_STATUS(stp);
    encoded_v1_1_ad->num_bytes = 0;
    encoded_v1_1_ad->bytes = NULL;
    *encoded_pac_len = 0;
    *encoded_pac = NULL;

    /*
     * If the caller must give us valid version 0 authorization data
     */
	
    dce_assert(sec_svc_handle, 
	       (!authz_data && authz_data->num_bytes && authz_data->bytes));
    dce_assert(sec_svc_handle, (AUTHZ_DATA_VNO(authz_data->bytes) != 0));

    /* 
     * Get the type uuid from the authorization data header
     */
    sec__authz_util_get_authzd_type(authz_data, &authzd_type);

    if (uuid_equal(&authzd_type, &sec__g_v0_authzd_compat, &ignore_st)) {
	/* 
	 * there's a version 0 idl pickle containing a version 1 
	 * compatibility PAC beginning at byte 0 of the
	 * authorization data stream.  The version 1 idl pickle
	 * containing version 1.1 authorization data, if present,
	 * starts at the first 8 byte-aligned boundary after the
	 * the version 0 PAC pickle.
	 */

	unsigned8       version;
	unsigned32      data_len;
	rpc_syntax_id_t syntax;
        uuid_t          ignore_uuid;
	
	*encoded_pac =  (idl_pkl_t *)authz_data->bytes;

	idl_pkl_get_header(authz_data->bytes, &version, &data_len,
			   &syntax, &ignore_uuid);

	/* 
	 * If our understanding of the version 0 IDL PAC pickling
	 * code in sec_pkl.c is is correct, the length of a
	 * version 0 pickle is the version 0 pickle header
	 * + the pickle data + pad bytes to extend the pickle out
         * to a boundary required for alignment purposes (8-byte).
	 * If that length accounts for all of the authorization data,
	 * then there is no version 1.1 authorization data, otherwise,
	 * there is.
	 */
	*encoded_pac_len = idl_pkl_data_offset + data_len;
        *encoded_pac_len = (*encoded_pac_len + (7)) & ~(7);

	if (*encoded_pac_len < authz_data->num_bytes) {
	    mp =  (sec_authz_mp_t) (authz_data->bytes + *encoded_pac_len);
	    AUTHZ_UTIL_UNMARSH_BE_UNSIGNED32(mp, encoded_v1_1_ad->num_bytes);
	    AUTHZ_UTIL_ADVANCE_MP(mp, v0_authz_alignment);
	    encoded_v1_1_ad->bytes = (idl_byte *) mp;
	}

    } else if (uuid_equal(&authzd_type, &sec__g_v0_authzd_no_compat, &ignore_st)) {
	/*
	 * There is no version 0 idl pickle containing a version
	 * 1 compatibility PAC, so a v1_1 pickle begins right after
	 * the authorization data header.
	 */

	mp = (sec_authz_mp_t) &(authz_data->bytes[v0_authz_header_size]);
        AUTHZ_UTIL_ADVANCE_MP(mp, authz_padding(v0_authz_header_size,
                                                v0_authz_alignment));
	AUTHZ_UTIL_UNMARSH_BE_UNSIGNED32(mp, encoded_v1_1_ad->num_bytes);
	AUTHZ_UTIL_ADVANCE_MP(mp, v0_authz_alignment);
	encoded_v1_1_ad->bytes = (idl_byte *) mp;
    } /* default: return nothing cause we don't undertand the type */
}


/*  sec__authz_util_epac_to_pac_alias
 *
 * !!BEWARE!!
 *
 * We optimize wherever possible by aliassng the PAC storage
 * back into the EPAC wherver possible.  Due to differences in 
 * the layout of foreign groups in pacs and epacs, we can't do
 * any straightforward aliasing.  We are forced to dynamically
 * allocate foreign group storage in the pac.   The caller should
 * always use the sec__authz_util_pac_alias_free() routine
 * to free up storage for PACs created with 
 * sec__authz_util_epac_to_pac_alias() 
 */
extern void sec__authz_util_epac_to_pac_alias (
    idl_void_p_t        (*alloc)(idl_size_t size),     /* [in] */
    void                (*dealloc)(idl_void_p_t ptr),  /* [in] */
    sec_id_epac_data_t  *epac_data,     /* [in] */
    boolean32           authenticated,  /* [in] */
    sec_id_pac_t        *pac,           /* [out] */
    error_status_t      *stp            /* [out] */
)
{
    sec_id_pa_t   pa = epac_data->pa;
    unsigned16    i, j, cur_group;

    memset(pac, 0, sizeof(sec_id_pac_t));

    pac->authenticated = authenticated;
    pac->pac_type = sec_id_pac_format_v1;
    pac->realm = pa.realm;
    pac->principal = pa.principal;
    pac->group = pa.group;
    pac->num_groups = pa.num_groups;
    pac->groups = pa.groups;

    /* Need to "flatten" the epac foreign groupset
     * structure into a v1_pac structure
     */

    /* count the total number of foreign groups in the epac */
    pac->num_foreign_groups = 0;
    for (i = 0; i < pa.num_foreign_groupsets; i++)
	pac->num_foreign_groups += pa.foreign_groupsets[i].num_groups;

    if (pac->num_foreign_groups > 0) {
	pac->foreign_groups = (sec_id_foreign_t *)
	    (*alloc)(pac->num_foreign_groups * sizeof(sec_id_foreign_t));

	if (pac->foreign_groups == NULL) {
	    SET_STATUS(stp, sec_s_no_memory);
	    return;
	}

	/* now do the "copy" */
	cur_group = 0;
	for (i = 0; i < pa.num_foreign_groupsets; i++) {
	    for (j = 0; j < pa.foreign_groupsets[i].num_groups; j++) {
		pac->foreign_groups[cur_group].realm
		    = pa.foreign_groupsets[i].realm;
		pac->foreign_groups[cur_group].id
		    = pa.foreign_groupsets[i].groups[j];
		cur_group++;
	    }
	}
    }
}

/* sec__authz_util_pac_alias_free
 *
 * Free whatever pointers are not aliases back into an epac.
 *
 * BEWARE.
 *
 * This routine should be used only to dispose of PACs
 * created with sec_authz_util_epac_to_pac_alias().
 * If you free such a PAC any othr way, you will be
 * freeing pointers in some EPAC.  Likewise, should
 * you dispose of an EPAC that has been used to create
 * an aliased PAC, pointer in the aliased PAC will be orphaned.
 */

extern void sec__authz_util_pac_alias_free (
    void                (*dealloc)(idl_void_p_t ptr),  /* [in] */
    sec_id_pac_t        *pac                           /* [in/out] */
)
{
    if (pac && pac->num_foreign_groups > 0 && pac->foreign_groups != NULL)
	(*dealloc)(pac->foreign_groups);
    pac->num_foreign_groups = 0;
    pac->foreign_groups = NULL;
}
