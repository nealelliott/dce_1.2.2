/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_mkey.h,v $
 * Revision 1.1.6.2  1996/02/18  22:59:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:49  marty]
 *
 * Revision 1.1.6.1  1995/12/13  16:25:28  root
 * 	Submit
 * 	[1995/12/11  15:14:54  root]
 * 
 * Revision 1.1.3.5  1992/12/29  13:08:28  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:44:48  zeliff]
 * 
 * Revision 1.1.3.4  1992/12/18  22:10:30  sekhar
 * 	[OT 6540] Replication code drop bl6c
 * 	[1992/12/18  21:46:15  sekhar]
 * 
 * Revision 1.1.4.2  1992/12/11  21:09:43  emartin
 * 	bl6c: remove duplicate rs_mkey_random prototype
 * 
 * Revision 1.1.3.3  1992/11/20  15:50:31  ahop
 * 	Add rs_mkey_store_in_db.
 * 	[1992/11/10  23:02:20  ahop]
 * 
 * Revision 1.1.3.2  1992/10/07  20:26:32  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:11:52  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:38:58  burati
 * 	 Second replication code drop: bl5
 * 
 * $EndLog$
 */

/*
 * Copyright (c) Hewlett-Packard Company 1992
 * Unpublished work. All Rights Reserved.
 */

#ifndef rs_mkey__included
#define rs_mkey__included


#include <rsdb_pvt.h>
#include <krb5/kdb.h>


void rs_mkey_create (
#ifdef __STDC__
    sec_id_t            *rgy_local_cell,
    char                *keyseed
#endif
);

void rs_mkey_init (
#ifdef __STDC__
    unsigned32          rep_state,
    error_status_t      *st_p
#endif
);

void rs_mkey_store_in_db (
#ifdef __STDC__
    error_status_t      *st_p
#endif
);

void rs_mkey_verify (
#ifdef __STDC__
    krb5_keyblock           *mkey,
    krb5_kvno               mkey_version,
    krb5_encrypt_block      *eblock,
    error_status_t          *st_p
#endif
);

void rs_mkey_encrypt_with_master (
#ifdef __STDC__
    krb5_keyblock               *in,
    krb5_encrypted_keyblock     *out,
    error_status_t              *st_p
#endif
);

void rs_mkey_decrypt_with_master (
#ifdef __STDC__
    krb5_kvno                   mkey_version,
    krb5_encrypted_keyblock     *in,
    krb5_keyblock               *out,
    error_status_t              *st_p
#endif
);
      
void rs_mkey_random (
#ifdef __STDC__
    sec_passwd_rec_t    *passwd,
    error_status_t      *st_p
#endif
);

void rs_mkey_string_to_key (
#ifdef __STDC__
    krb5_keytype        keytype,
    krb5_data           *pwd_data,
    krb5_data           *pepper,
    krb5_keyblock       *key,
    error_status_t      *st_p
#endif
);

void rs_mkey_change (
#ifdef __STDC__
    error_status_t      *st_p
#endif
);


void rs_mkey_purge (
#ifdef __STDC__
    void
#endif
);


#endif

