/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: authz_util.h,v $
 * Revision 1.1.4.2  1996/02/18  22:57:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:16:46  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:27:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:53:18  root]
 * 
 * Revision 1.1.2.1  1994/08/18  20:25:23  greg
 * 	Header file for authz_util.c
 * 	[1994/08/17  17:24:26  greg]
 * 
 * 	HP revision /main/greg_fc1/1  1994/07/31  21:08 UTC  greg
 * 	first cut at real authz data: compiled but not tested
 * 
 * 	HP revision /main/greg_intercell_bugs/1  1994/07/14  20:13 UTC  greg
 * 	first cut at real dce 1.1 authorization data
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1994
 * Unpublished work. All Rights Reserved.
 */

#ifndef _AUTHZ_UTIL_H_
#define _AUTHZ_UTIL_H_

/* 
 * We need to do a bit of byte swapping now and again
 * for those pieces of authorization data that are
 * always big-endian regardless of local architecture,
 * so we provide some definitions  in a central place
 */

#define AUTHZ_UTIL_ALIGN_MP(mp, alignment) \
    rpc_align_mp((mp), (alignment))

#define AUTHZ_UTIL_ADVANCE_MP(mp, delta) \
    rpc_advance_mp((mp), (delta))

#define AUTHZ_UTIL_MARSH_BYTES(mp, bytes, len) \
    memcpy((mp), (bytes), (len)); \
    AUTHZ_UTIL_ADVANCE_MP(mp, (len))

#define AUTHZ_UTIL_UNMARSH_BYTES(mp, bytes, len) \
    memcpy((bytes), (mp), (len)); \
    AUTHZ_UTIL_ADVANCE_MP(mp, (len))

#define SWAP_UNSIGNED16(src, dst)\
    ((dst)[0])=((src)[1]);\
    ((dst)[1])=((src)[0])

#define SWAP_UNSIGNED32(src, dst)\
    ((dst)[0])=((src)[3]);\
    ((dst)[1])=((src)[2]);\
    ((dst)[2])=((src)[1]);\
    ((dst)[3])=((src)[0])

#define SWAP_UUID(src, dst)\
    SWAP_UNSIGNED32((src), (dst));\
    SWAP_UNSIGNED16((&(src)[4]),(&(dst)[4]));\
    SWAP_UNSIGNED16((&(src)[6]),(&(dst)[6]));\
    {int i; for (i=8; i<16; i++) (dst)[i]=(src)[i];}

#if (NDR_LOCAL_INT_REP == ndr_c_int_big_endian)

#  define AUTHZ_UTIL_MARSH_BE_UNSIGNED32(mp, uint32) \
       (*((unsigned32 *)(&(mp)[0]))) = (uint32)

#  define AUTHZ_UTIL_UNMARSH_BE_UNSIGNED32(mp, uint32) \
       (uint32) = (*((unsigned32 *)(&(mp)[0])))

#  define AUTHZ_UTIL_MARSH_BE_UUID(mp, uuid) \
       (*((uuid_t *)(&(mp)[0]))) = (uuid)

#  define AUTHZ_UTIL_UNMARSH_BE_UUID(mp, uuid) \
       (uuid) = (*((uuid_t *)(&(mp)[0])))

#else  /* little endian */

#  define AUTHZ_UTIL_MARSH_BE_UNSIGNED32(mp, uint32) \
       SWAP_UNSIGNED32((sec_authz_mp_t)(&(uint32)), (sec_authz_mp_t)(&(mp)[0]))

#  define AUTHZ_UTIL_UNMARSH_BE_UNSIGNED32(mp, uint32) \
       SWAP_UNSIGNED32((sec_authz_mp_t)(&(mp)[0]), (sec_authz_mp_t)(&(uint32)))

#  define AUTHZ_UTIL_MARSH_BE_UUID(mp, uuid) \
       SWAP_UUID((sec_authz_mp_t)(&(uuid)), (sec_authz_mp_t)(&(mp)[0]))

#  define AUTHZ_UTIL_UNMARSH_BE_UUID(mp, uuid) \
       SWAP_UUID( (sec_authz_mp_t)(&(mp)[0]), (sec_authz_mp_t)(&(uuid)))

#endif


/* authorization data version number is at offset 0 */  
#define AUTHZ_DATA_VNO(authz_bytes) (*((unsigned8 *)&((authz_bytes)[0])))

/* v0 authorization data type uuid begins at offset 24 */
#define AUTHZ_UTIL_V0_TYPE_OFFSET 24 

typedef rpc_mp_t sec_authz_mp_t;

/*
 * Global variables 
*/

/* 
 * v0 authorization data type uuids
 */
extern uuid_t  sec__g_v0_authzd_compat;
               /* d9f3bd98-567d-11ca-9ec6-08001e022936 */
extern uuid_t  sec__g_v0_authzd_no_compat;  
               /* 9b143cec-9a90-11cd-8b27-0800097086bf */

/* 
 * Routines
 */

void  sec_authz_util_gen_seal (
    idl_void_p_t (*alloc)(idl_size_t size),     /* [in] */
    void         (*dealloc)(idl_void_p_t ptr),  /* [in] */
    sec_bytes_t   *delegation_chain,            /* [in] */
    sec_id_seal_t *seal                         /* [out] */
);

int sec_authz_util_eq_seal (
    sec_id_seal_t *a,
    sec_id_seal_t *b
);

void sec_id_seal_free (
    void         (*dealloc)(idl_void_p_t ptr),  /* [in] */
    sec_id_seal_t *seal                         /* [in/out] */
);


void sec__authz_util_get_authzd_type (
    sec_bytes_t  *authz_data,   /* [in] */
    uuid_t       *type          /* [out] */
);

void sec__authz_util_marsh_v0_authzd (
    idl_void_p_t           (*alloc)(idl_size_t size),     /* [in] */
    void                   (*dealloc)(idl_void_p_t ptr),  /* [in] */
    sec_bytes_t            *encoded_v1_1_ad,               /* [in] */
    unsigned32             encoded_pac_len,               /* [in] */
    idl_pkl_t              *encoded_pac,                  /* [in] */
    sec_bytes_t            *authz_data,                   /* [out] */
    error_status_t         *stp                           /* [out] */
);

void sec__authz_util_unmarsh_v0_authzd (
    sec_bytes_t            *authz_data,                   /* [in] */
    sec_bytes_t            *encoded_v1_1_ad,              /* [out] */
    unsigned32             *encoded_pac_len,              /* [out] */
    idl_pkl_t              **encoded_pac,                 /* [out] */
    error_status_t         *stp                           /* [out] */
);

/*
 * The following routine converts an EPAC to a PAC by
 * aliasing storage in the EPAC wherever possible.
 * NEVER, EVER use anything other than the destructor
 * function for aliased PACs to free pointers in
 * an aliased PAC.  Just don't.  Use  
 * sec__authz_util_pac_alias_free().
 *
 * And be careful about disposing of an EPAC that has
 * an aliased PAC associated with it.  When you free such an
 * EPAC, you orphan pointers in the aliased PAC.
 */
void  sec__authz_util_epac_to_pac_alias (
    idl_void_p_t        (*alloc)(idl_size_t size),     /* [in] */
    void                (*dealloc)(idl_void_p_t ptr),  /* [in] */
    sec_id_epac_data_t  *epac_data,     /* [in] */
    boolean32           authenticated,  /* [in] */
    sec_id_pac_t        *pac,           /* [out] */
    error_status_t      *stp            /* [out] */
);

void  sec__authz_util_pac_alias_free (
    void                (*dealloc)(idl_void_p_t ptr),  /* [in] */
    sec_id_pac_t        *pac                           /* [in/out] */
);

#endif  
