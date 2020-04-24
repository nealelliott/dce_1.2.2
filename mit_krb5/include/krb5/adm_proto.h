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
 * $Log: adm_proto.h,v $
 * Revision 1.1.2.1  1996/06/05  20:26:04  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:26:05  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * include/krb5/adm_proto.h
 *
 * Copyright 1995 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America may
 *   require a specific license from the United States Government.
 *   It is the responsibility of any person or organization contemplating
 *   export to obtain such a license before exporting.
 *
 * WITHIN THAT CONSTRAINT, permission to use, copy, modify, and
 * distribute this software and its documentation for any purpose and
 * without fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright notice and
 * this permission notice appear in supporting documentation, and that
 * the name of M.I.T. not be used in advertising or publicity pertaining
 * to distribution of the software without specific, written prior
 * permission.  M.I.T. makes no representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 *
 */
#ifndef	KRB5_ADM_PROTO_H__
#define	KRB5_ADM_PROTO_H__

/*
 * Function prototypes.
 */

/* adm_conn.c */
krb5_error_code krb5_adm_connect
	PROTOTYPE((krb5_context,
		   char *,
		   char *,
		   char *,
		   int *,
		   krb5_auth_context **,
		   krb5_ccache *));
void krb5_adm_disconnect
	PROTOTYPE((krb5_context,
		   int *,
		   krb5_auth_context *,
		   krb5_ccache));

/* adm_proto.c */
void krb5_free_adm_data
	PROTOTYPE((krb5_context,
		   krb5_int32,
		   krb5_data *));

krb5_error_code krb5_send_adm_cmd
	PROTOTYPE((krb5_context,
		   krb5_pointer,
		   krb5_auth_context *,
		   int,
		   krb5_data *));
krb5_error_code krb5_send_adm_reply
	PROTOTYPE((krb5_context,
		   krb5_pointer,
		   krb5_auth_context *,
		   krb5_int32,
		   int,
		   krb5_data *));
krb5_error_code krb5_read_adm_cmd
	PROTOTYPE((krb5_context,
		   krb5_pointer,
		   krb5_auth_context *,
		   krb5_int32 *,
		   krb5_data **));
krb5_error_code krb5_read_adm_reply
	PROTOTYPE((krb5_context,
		   krb5_pointer,
		   krb5_auth_context *,
		   krb5_int32 *,
		   krb5_int32 *,
		   krb5_data **));
#endif	/* KRB5_ADM_PROTO_H__ */
