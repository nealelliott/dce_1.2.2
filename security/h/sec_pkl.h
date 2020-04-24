/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_pkl.h,v $
 * Revision 1.1.6.1  1996/08/09  12:05:57  arvind
 * 	Merge Registry support for KDC private key storage
 * 	[1996/07/13  20:49 UTC  aha  /main/aha_pk6/1]
 *
 * 	Merge-out and bugfixes and Registry work
 * 	[1996/02/18  23:01:15  marty  1.1.4.2]
 *
 * Revision 1.1.4.2  1996/02/18  23:01:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:19:36  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  17:29:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:54:23  root]
 * 
 * Revision 1.1.2.2  1992/12/29  13:27:50  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:16:12  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:43:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  sec_pkl.h V=2 11/22/91 //littl/prgy/src/utils
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**  Copyright (c) 1991 by
**      Hewlett-Packard Company, Palo Alto, Ca.
**
**   Copyright (c) Hewlett-Packard Company 1991
**   Unpublished work. All Rights Reserved.
**  
**
**  NAME:
**
**      sec_pkl.h
**
**  FACILITY:
**
**      DCE Security
**
**  ABSTRACT:
**
**      Pickling routine prototypes for three sec_ types.
**
**  
**
*/

#include <dce/pklbase.h>
#include <dce/rpcbase.h>
#include <dce/passwd.h>
#include <dce/id_base.h>
#include <dce/sec_base.h>

/*
 * type IDs for the two pickled types
 */

static uuid_t sec_chksum_t_typeid = {
    /* d20b05c8-49da-11ca-996d-08001e022936 */
    0xd20b05c8,
    0x49da,
    0x11ca,
    0x99,
    0x6d,
    {0x08, 0x00, 0x1e, 0x02, 0x29, 0x36}
};

static uuid_t sec_passwd_rec_t_typeid = {
    /* d52ef390-49da-11ca-b2ac-08001e022936 */
    0xd52ef390,
    0x49da,
    0x11ca,
    0xb2,
    0xac,
    {0x08, 0x00, 0x1e, 0x02, 0x29, 0x36}
};

static uuid_t sec_id_pac_t_typeid = {
    /* d9f3bd98-567d-11ca-9ec6-08001e022936 */
    0xd9f3bd98,
    0x567d,
    0x11ca,
    0x9e,
    0xc6,
    {0x08, 0x00, 0x1e, 0x02, 0x29, 0x36}
};

/*
 * pickling routine for sec_chksum_t
 */
void sec_chksum_t_pickle(
#ifdef __STDC__
    sec_chksum_t *data,
    rpc_syntax_id_t *syntax,
    void *(*allocator)(unsigned32),
    unsigned32 stream_alloc_mod,
    idl_pkl_t **pickle,
    unsigned32 *pickle_len,
    error_status_t *st
#endif
);

/*
 * unpickling routine for sec_chksum_t
 */
void sec_chksum_t_unpickle(
#ifdef __STDC__
    idl_pkl_t *pickle,
    void *(*allocator)(unsigned32),
    sec_chksum_t *data,
    error_status_t *st
#endif
);

/*
 * pickling routine for sec_passwd_rec_t
 */
void sec_passwd_rec_t_pickle(
#ifdef __STDC__
    sec_passwd_rec_t *data,
    rpc_syntax_id_t *syntax,
    void *(*allocator)(unsigned32),
    unsigned32 stream_alloc_mod,
    idl_pkl_t **pickle,
    unsigned32 *pickle_len,
    error_status_t *st
#endif
);

/* Routine that can route an encoded passwd_rec_t to
 * the appropriate decode function: either
 * sec_pwd_decode or sec_passwd_rec_t_unpickle
 */
void sec_passwd_rec_t_decode(
#ifdef __STDC__
    unsigned32        num_bytes,
    idl_byte          *bytes_p,
    idl_void_p_t      (*alloc)(idl_size_t size),
    void              (*dealloc)(idl_void_p_t ptr),
    sec_passwd_rec_t  *pwd_p,
    error_status_t    *st_p
#endif
);

/*
 * unpickling routine for sec_passwd_rec_t
 */
void sec_passwd_rec_t_unpickle(
#ifdef __STDC__
    idl_pkl_t *pickle,
    void *(*allocator)(unsigned32),
    sec_passwd_rec_t *data,
    error_status_t *st
#endif
);

/*
 * pickling routine for sec_id_pac_t
 */
void sec_id_pac_t_pickle(
#ifdef __STDC__
    sec_id_pac_t *data,
    rpc_syntax_id_t *syntax,
    void *(*allocator)(unsigned32),
    unsigned32 stream_alloc_mod,
    idl_pkl_t **pickle,
    unsigned32 *pickle_len,
    error_status_t *st
#endif
);

/*
 * unpickling routine for sec_id_pac_t
 */
void sec_id_pac_t_unpickle(
#ifdef __STDC__
    idl_pkl_t *pickle,
    void *(*allocator)(unsigned32),
    sec_id_pac_t *data,
    error_status_t *st
#endif
);


