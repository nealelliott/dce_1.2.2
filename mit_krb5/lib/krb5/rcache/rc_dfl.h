/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: rc_dfl.h,v $
 * Revision 1.1.2.1  1996/06/05  21:21:38  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:11:59  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/rcache/rc_dfl.h
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

krb5_error_code krb5_rc_dfl_init 
    	PROTOTYPE((krb5_context,
		   krb5_rcache,
		   krb5_deltat));
krb5_error_code krb5_rc_dfl_recover 
	PROTOTYPE((krb5_context,
		   krb5_rcache)); 
krb5_error_code krb5_rc_dfl_destroy 
	PROTOTYPE((krb5_context,
		   krb5_rcache));
krb5_error_code krb5_rc_dfl_close 
	PROTOTYPE((krb5_context,
		   krb5_rcache));
krb5_error_code krb5_rc_dfl_store 
	PROTOTYPE((krb5_context,
		   krb5_rcache,
		   krb5_donot_replay *));
krb5_error_code krb5_rc_dfl_expunge 
	PROTOTYPE((krb5_context,
		   krb5_rcache));
krb5_error_code krb5_rc_dfl_get_span 
	PROTOTYPE((krb5_context,
		   krb5_rcache,
		   krb5_deltat *));
char * krb5_rc_dfl_get_name 
	PROTOTYPE((krb5_context,
		   krb5_rcache));
krb5_error_code krb5_rc_dfl_resolve 
	PROTOTYPE((krb5_context,
		   krb5_rcache,
		   char *));
krb5_error_code krb5_rc_dfl_close_no_free
	PROTOTYPE((krb5_context,
		   krb5_rcache));
void krb5_rc_free_entry 
	PROTOTYPE((krb5_context,
		   krb5_donot_replay **));
#endif

