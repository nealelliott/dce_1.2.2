/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rs_pwd.h,v $
 * Revision 1.1.7.2  1996/02/18  22:59:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:55  marty]
 *
 * Revision 1.1.7.1  1995/12/13  16:25:37  root
 * 	Submit
 * 	[1995/12/11  15:14:59  root]
 * 
 * Revision 1.1.5.1  1994/07/15  14:59:30  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:16:46  mdf]
 * 
 * Revision 1.1.3.4  1993/03/19  22:19:49  emartin
 * 	CR 7472 more new routines,
 * 	rs_pwd_get_key => rs_pwd_get_key_by_prnc - returns principal's key
 * 	[1993/03/19  00:07:42  emartin]
 * 
 * 	CR 7472  some new routines for unix passwd mgmt
 * 	[1993/03/17  17:16:52  emartin]
 * 
 * Revision 1.1.3.3  1992/12/29  13:24:56  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:45:04  zeliff]
 * 
 * Revision 1.1.3.2  1992/10/07  20:26:55  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:12:05  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:41:35  burati
 * 	 Second replication code drop: bl5
 * 
 * Revision 1.1  1992/01/19  14:42:49  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/* 
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 */
/*
 * Registry Server - Password management
 */

#ifndef __rs_pwd_h_included__
#define __rs_pwd_h_included__ 

#include <dce/passwd.h>
#include <dce/nbase.h>
#include <dce/rsbase.h>


/*
 *  rs_pwd_get_key
 *
 * Retrieve a password rec from the pickled network rep.  Perform the decryption 
 * using the key for the principal identified in the "principal" parameter.
 * Also, retrieve the principal's key.
 */
void rs_pwd_get_key_by_prnc (
#ifdef __STDC__
    rs_acct_key_transmit_t     *transmit_rep,  /* [in] */
    uuid_t                     *principal,     /* [in] calling principal's id */
    sec_passwd_rec_t           *new_key,       /* [out] */
    sec_passwd_rec_t           **princ_key,    /* [out] */
    error_status_t             *stp
#endif
);
  
/*
 *  rs_pwd_get_key_by_session_key
 *
 * Retrieve a password rec from the pickled network rep.  Perform the decryption 
 * using the session key provided by the caller
 */
void rs_pwd_get_key_by_session_key (
#ifdef __STDC__
    rs_acct_key_transmit_t     *transmit_rep,  /* [in] */
    sec_passwd_rec_t           *session_key,   /* [in] */
    sec_passwd_rec_t           *new_key,       /* [out] */
    error_status_t             *stp
#endif
);

/*
 *  rs_pwd_unix_pwd_to_xmit
 *
 * Generate a pickled network rep from a unix password
 * using the "encryption_key" argument.
 */
void rs_pwd_unix_pwd_to_xmit (
#ifdef __STDC__
    sec_passwd_rec_t           *encryption_key,
    sec_rgy_unix_passwd_buf_t  unix_passwd,    
    rs_acct_key_transmit_t     **xmit_rep,     
    error_status_t             *stp
#endif
);

/*
 *  rs_pwd_unix_pwd_fr_xmit
 *
 * Retrieve a unix password from its pickled network rep.
 * Perform the decryption using the "encryption_key" 
 * argument.
 */
void rs_pwd_unix_pwd_fr_xmit (
#ifdef __STDC__
    rs_acct_key_transmit_t     *xmit_rep,     
    sec_passwd_rec_t           *encryption_key,
    sec_rgy_unix_passwd_buf_t  unix_passwd,    
    error_status_t             *stp
#endif
);

/*
 *  rs_pwd_to_unix_pwd
 *
 * Convert a generic sec_passwd to a unix_passwd_buffer
 */
void rs_pwd_to_unix_pwd (
#ifdef __STDC__
    sec_passwd_rec_t            *passwd,
    sec_rgy_unix_passwd_buf_t   unix_passwd,      /* [out] */
    error_status_t              *stp
#endif
);

#endif
