/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: errors.h,v $
 * Revision 1.1.12.1  1996/07/08  18:34:48  arvind
 * 	Add KDC_ERR_MUST_USE_USER2USER
 * 	[1996/01/03  19:13 UTC  psn  /main/DCE_1.2/1]
 *
 * Revision 1.1.10.3  1996/02/18  23:02:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:20:54  marty]
 * 
 * Revision 1.1.10.2  1995/12/08  17:42:07  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/15  00:00 UTC  rps
 * 	copyright
 * 	[1995/04/05  17:05 UTC  rps  /main/MOTHRA_panic/2]
 * 
 * 	more beta4 merge
 * 	[1995/02/28  22:43 UTC  rps  /main/rps_hc/2]
 * 
 * 	merge up to krb5beta4patch3
 * 	[1995/01/29  01:19 UTC  rps  /main/rps_hc/1]
 * 
 * Revision 1.1.6.1  1994/05/11  19:15:40  ahop
 * 	hp_sec_to_osf_2 drop
 * 	[1994/04/29  21:12:33  ahop]
 * 
 * Revision 1.1.4.2  1992/12/29  13:58:57  zeliff
 * 	     Embedding copyright notice
 * 	     [1992/12/28  20:52:22  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/13  22:06:56  sommerfeld
 * 	     Add krb5 draft 5 error statuses.
 * 	     [1992/05/11  21:39:43  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  14:48:38  devrcs
 * 	     Initial revision
 * 
 * $EndLog$
 */

/*
**
** Copyright (c) Hewlett-Packard Company 1991, 1994, 1995, 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * include/krb5/errors.h
 *
 * Copyright 1989,1990 by the Massachusetts Institute of Technology.
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
 *
 * Protocol error code definitions
 */


#ifndef KRB5_ERRORS__
#define KRB5_ERRORS__


/* Error codes used in KRB_ERROR protocol messages.
   Return values of library routines are based on a different error table
   (which allows non-ambiguous error codes between subsystems) */

/* KDC errors */
#define	KDC_ERR_NONE			0 /* No error */
#define	KDC_ERR_NAME_EXP		1 /* Client's entry in DB expired */
#define	KDC_ERR_SERVICE_EXP		2 /* Server's entry in DB expired */
#define	KDC_ERR_BAD_PVNO		3 /* Requested pvno not supported */
#define	KDC_ERR_C_OLD_MAST_KVNO		4 /* C's key encrypted in old master */
#define	KDC_ERR_S_OLD_MAST_KVNO		5 /* S's key encrypted in old master */
#define	KDC_ERR_C_PRINCIPAL_UNKNOWN	6 /* Client not found in Kerberos DB */
#define	KDC_ERR_S_PRINCIPAL_UNKNOWN	7 /* Server not found in Kerberos DB */
#define	KDC_ERR_PRINCIPAL_NOT_UNIQUE	8 /* Multiple entries in Kerberos DB */
#define	KDC_ERR_NULL_KEY		9 /* The C or S has a null key */
#define	KDC_ERR_CANNOT_POSTDATE		10 /* Tkt ineligible for postdating */
#define	KDC_ERR_NEVER_VALID		11 /* Requested starttime > endtime */
#define	KDC_ERR_POLICY			12 /* KDC policy rejects request */
#define	KDC_ERR_BADOPTION		13 /* KDC can't do requested opt. */
#define	KDC_ERR_ETYPE_NOSUPP		14 /* No support for encryption type */
#define KDC_ERR_SUMTYPE_NOSUPP		15 /* No support for checksum type */
#define KDC_ERR_PADATA_TYPE_NOSUPP	16 /* No support for padata type */
#define KDC_ERR_TRTYPE_NOSUPP		17 /* No support for transited type */
#define KDC_ERR_CLIENT_REVOKED		18 /* C's creds have been revoked */
#define KDC_ERR_SERVICE_REVOKED		19 /* S's creds have been revoked */
#define KDC_ERR_TGT_REVOKED		20 /* TGT has been revoked */
#define KDC_ERR_CLIENT_NOTYET		21 /* C not yet valid */
#define KDC_ERR_SERVICE_NOTYET		22 /* S not yet valid */
#define KDC_ERR_KEY_EXP			23 /* Password has expired */
#define KDC_ERR_PREAUTH_FAILED		24 /* Preauthentication failed */
#ifdef OSF_DCE
#define KDC_PREAUTH_FAILED		24 /* Preauthentication failed */
#endif /* OSF_DCE */			   /* tmp retained for source compat */
#define KDC_ERR_PREAUTH_REQUIRED	25 /* Additional preauthentication */
					   /* required */
#define KDC_ERR_SERVER_NOMATCH		26 /* Requested server and */
					   /* ticket don't match*/
#define KDC_ERR_MUST_USE_USER2USER      27 /* Server principal only valid for
       	                                    * user-to-user TGS requests
					    */

/* Application errors */
#define	KRB_AP_ERR_BAD_INTEGRITY 31	/* Decrypt integrity check failed */
#define	KRB_AP_ERR_TKT_EXPIRED	32	/* Ticket expired */
#define	KRB_AP_ERR_TKT_NYV	33	/* Ticket not yet valid */
#define	KRB_AP_ERR_REPEAT	34	/* Request is a replay */
#define	KRB_AP_ERR_NOT_US	35	/* The ticket isn't for us */
#define	KRB_AP_ERR_BADMATCH	36	/* Ticket/authenticator don't match */
#define	KRB_AP_ERR_SKEW		37	/* Clock skew too great */
#define	KRB_AP_ERR_BADADDR	38	/* Incorrect net address */
#define	KRB_AP_ERR_BADVERSION	39	/* Protocol version mismatch */
#define	KRB_AP_ERR_MSG_TYPE	40	/* Invalid message type */
#define	KRB_AP_ERR_MODIFIED	41	/* Message stream modified */
#define	KRB_AP_ERR_BADORDER	42	/* Message out of order */
#define	KRB_AP_ERR_ILL_CR_TXT	43	/* Illegal cross-realm ticket */
#define	KRB_AP_ERR_BADKEYVER	44	/* Key version is not available */
#define	KRB_AP_ERR_NOKEY	45	/* Service key not available */
#define	KRB_AP_ERR_MUT_FAIL	46	/* Mutual authentication failed */
#define KRB_AP_ERR_BADDIRECTION	47 	/* Incorrect message direction */
#define KRB_AP_ERR_METHOD	48 	/* Alternative authentication */
					/* method required */
#define KRB_AP_ERR_BADSEQ	49 	/* Incorrect sequence numnber */
					/* in message */
#define KRB_AP_ERR_INAPP_CKSUM	50	/* Inappropriate type of */
					/* checksum in message */

/* other errors */
#define KRB_ERR_GENERIC		60 	/* Generic error (description */
					/* in e-text) */
#define	KRB_ERR_FIELD_TOOLONG	61	/* Field is too long for impl. */

#endif /* KRB5_ERRORS__ */
