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
 * Revision 1.1.7.2  1996/02/18  22:58:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:17:01  marty]
 *
 * Revision 1.1.7.1  1995/12/13  16:24:40  root
 * 	Submit
 * 	[1995/12/11  15:14:23  root]
 * 
 * Revision 1.1.5.1  1994/07/15  14:58:59  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:15:39  mdf]
 * 
 * Revision 1.1.3.3  1992/12/29  13:07:02  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:43:21  zeliff]
 * 
 * Revision 1.1.3.2  1992/10/07  20:24:23  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:10:28  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:29:15  burati
 * 	 Second replication code drop: bl5
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1991
 * Unpublished work. All Rights Reserved.
 */
/*
 *      Registry Client - Password pickling
 *
 */

#ifndef __rca_pwd_h_included__
#define __rca_pwd_h_included__

#include <dce/rsbase.h>
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
