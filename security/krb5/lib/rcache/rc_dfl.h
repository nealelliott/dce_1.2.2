/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rc_dfl.h,v $
 * Revision 1.1.4.2  1996/02/18  23:03:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:07  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:44:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:50  root]
 * 
 * Revision 1.1.2.2  1992/12/29  16:32:08  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:19:51  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:52:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rc_dfl.h V=4 10/24/91 //littl/prgy/krb5/lib/rcache
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/rcache/rc_dfl.h,v $
 * $Author: marty $
 * $Id: rc_dfl.h,v 1.1.4.2 1996/02/18 23:03:34 marty Exp $
 *
 * This file of the Kerberos V5 software is derived from public-domain code
 * contributed by Daniel J. Bernstein, <brnstnd@acf10.nyu.edu>.
 *
 */

/*
 * Declarations for the default replay cache implementation.
 */

#ifndef KRB5_RC_DFL_H
#define KRB5_RC_DFL_H

extern krb5_rc_ops krb5_rc_dfl_ops; /* initialized to the following */

krb5_error_code krb5_rc_dfl_init PROTOTYPE((krb5_rcache,krb5_deltat));
krb5_error_code krb5_rc_dfl_recover PROTOTYPE((krb5_rcache)); 
krb5_error_code krb5_rc_dfl_destroy PROTOTYPE((krb5_rcache));
krb5_error_code krb5_rc_dfl_close PROTOTYPE((krb5_rcache));
krb5_error_code krb5_rc_dfl_store PROTOTYPE((krb5_rcache,krb5_donot_replay *));
krb5_error_code krb5_rc_dfl_expunge PROTOTYPE((krb5_rcache));
krb5_error_code krb5_rc_dfl_get_span PROTOTYPE((krb5_rcache,krb5_deltat *));
char *krb5_rc_dfl_get_name PROTOTYPE((krb5_rcache));
krb5_error_code krb5_rc_dfl_resolve PROTOTYPE((krb5_rcache,char *));

#endif
