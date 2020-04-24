/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rca_pwd.h,v $
 * Revision 1.1.7.2  1996/03/11  13:28:11  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:57  marty]
 *
 * Revision 1.1.7.1  1995/12/08  18:07:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:32:29  root]
 * 
 * Revision 1.1.3.2  1992/12/29  13:03:31  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:13:42  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:47:14  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rca_pwd.h V=1 05/21/91 //littl/prgy/src/client/rca
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 *      Registry Client - Password pickling
 * 
 */

#ifndef __rca_pwd_h_included__
#define __rca_pwd_h_included__

#include <rsbase.h>
#include <dce/sec_login.h>
#include <dce/passwd.h>

/* 
 * rca_pwd_gen_transmit_rep
 *
 * generate a "pickled" version of the plain password suitable for
 * transmission to the server.  This routine allocates storage for
 * the pickled passwd which must be freed with a call to 
 * rgy_pwd_free_transmit_rep.  
 */ 
void rca_pwd_gen_transmit_rep ( 
#ifdef __STDC__
    sec_login_handle_t         context,          /* [in] */
    sec_passwd_rec_t           *caller_key,      /* [in]  */
    sec_passwd_rec_t           *new_key,         /* [in]  */
    rs_acct_key_transmit_t     **transmit_rep,   /* [out] */
    error_status_t             *stp              /* [out] */
#endif
);


/* 
 * rca_pwd_free_transmit_rep
 *
 * Free the storage allocated via rgy_pwd_gen_transmit_rep
 */ 
void rca_pwd_free_transmit_rep ( 
#ifdef __STDC__
    rs_acct_key_transmit_t  **transmit_rep  /* [in, out] */
#endif
);

#endif
