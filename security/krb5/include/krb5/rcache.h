/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rcache.h,v $
 * Revision 1.1.4.2  1996/02/18  23:03:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:34  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:43:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:22  root]
 * 
 * Revision 1.1.2.2  1992/12/29  14:01:43  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:53:51  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:48:54  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rcache.h V=2 10/23/91 //littl/prgy/krb5/include/krb5
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/include/krb5/rcache.h,v $
 * $Author: marty $
 * $Id: rcache.h,v 1.1.4.2 1996/02/18 23:03:01 marty Exp $
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
 * All Rights Reserved.
 *
 * Export of this software from the United States of America is assumed
 *   to require a specific license from the United States Government.
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
 *
 * Replay detection cache definitions.
 */


#ifndef KRB5_RCACHE__
#define KRB5_RCACHE__

typedef struct krb5_rc_st {
    struct _krb5_rc_ops *ops;
    krb5_pointer data;
} *krb5_rcache;

typedef struct _krb5_donot_replay {
    char *server;			/* null-terminated */
    char *client;			/* null-terminated */
    krb5_int32 cusec;
    krb5_timestamp ctime;
} krb5_donot_replay;

typedef struct _krb5_rc_ops {
    char *type;
    krb5_error_code (*init)PROTOTYPE((krb5_rcache,krb5_deltat)); /* create */
    krb5_error_code (*recover)PROTOTYPE((krb5_rcache)); /* open */
    krb5_error_code (*destroy)PROTOTYPE((krb5_rcache));
    krb5_error_code (*close)PROTOTYPE((krb5_rcache));
    krb5_error_code (*store)PROTOTYPE((krb5_rcache,krb5_donot_replay *));
    krb5_error_code (*expunge)PROTOTYPE((krb5_rcache));
    krb5_error_code (*get_span)PROTOTYPE((krb5_rcache,krb5_deltat *));
    char *(*get_name)PROTOTYPE((krb5_rcache));
    krb5_error_code (*resolve)PROTOTYPE((krb5_rcache, char *));
} krb5_rc_ops;

krb5_error_code krb5_rc_default PROTOTYPE((krb5_rcache *));
krb5_error_code krb5_rc_register_type PROTOTYPE((krb5_rc_ops *));
krb5_error_code krb5_rc_resolve_type PROTOTYPE((krb5_rcache *,char *));
krb5_error_code krb5_rc_resolve_full PROTOTYPE((krb5_rcache *,char *));
char *krb5_rc_get_type PROTOTYPE((krb5_rcache));
char *krb5_rc_default_type PROTOTYPE((void));
char *krb5_rc_default_name PROTOTYPE((void));
krb5_error_code krb5_auth_to_rep PROTOTYPE((krb5_tkt_authent *,
					    krb5_donot_replay *));

#define krb5_rc_initialize(id, span) (*(id)->ops->init)(id, span)
#define krb5_rc_recover(id) (*(id)->ops->recover)(id)
#define krb5_rc_destroy(id) (*(id)->ops->destroy)(id)
#define krb5_rc_close(id) (*(id)->ops->close)(id)
#define krb5_rc_store(id, dontreplay) (*(id)->ops->store)(id, dontreplay)
#define krb5_rc_expunge(id) (*(id)->ops->expunge)(id)
#define krb5_rc_get_lifespan(id, spanp) (*(id)->ops->get_span)(id, spanp)
#define krb5_rc_get_name(id) (*(id)->ops->get_name)(id)
#define krb5_rc_resolve(id, name) (*(id)->ops->resolve)(id, name)

extern krb5_rc_ops krb5_rc_dfl_ops;

#endif /* KRB5_RCACHE__ */
