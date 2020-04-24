/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: proto.h,v $
 * Revision 1.1.9.2  1996/10/03  14:57:55  arvind
 * 	Add PADATA types from pkinit draft, krb5b6.
 * 	[1996/09/16  21:25 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 *
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:50 UTC  psn  /main/DCE122_PK/1]
 *
 * 	Add KRB5_PADATA types for pk-init
 * 	[1994/08/24  20:10:29  mullan_s  1.1.4.2]
 *
 * Revision 1.1.9.1  1996/07/08  18:35:25  arvind
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:50 UTC  psn  /main/DCE122_PK/1]
 * 
 * 	Add KRB5_PADATA types for pk-init
 * 	[1994/08/24  20:10:29  mullan_s  1.1.4.2]
 * 
 * Revision 1.1.7.2  1996/02/18  23:03:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:21:33  marty]
 * 
 * Revision 1.1.7.1  1995/12/08  17:43:52  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:21  root]
 * 
 * Revision 1.1.4.2  1994/08/24  20:10:29  mullan_s
 * 	[OT11556] Update DCE preauth types to match Kerberos spec.
 * 	[1994/08/24  20:09:50  mullan_s]
 * 
 * Revision 1.1.5.2  94/08/19  16:42:24  mullan_s
 * 	[OT 11556] Update preauth types to match Kerberos spec.
 * 
 * Revision 1.1.4.1  1994/05/11  19:18:21  ahop
 * 	hp_sec_to_osf_2 drop
 * 	add PADATA_ENC_THIRD_PARTY definition
 * 	[1994/04/29  21:16:36  ahop]
 * 
 * Revision 1.1.2.2  1992/12/29  14:01:38  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:53:47  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:48:53  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
**
** Copyright (c) Hewlett-Packard Company 1991, 1994
** Unpublished work. All Rights Reserved.
**
*/
/*
 *
 * Copyright 1989,1990 by the Massachusetts Institute of Technology.
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
 * Protocol message definitions.
 */


#ifndef	KRB5_PROTO__
#define KRB5_PROTO__

/* For Kerberos V5 Beta 1 compatibility */
#ifndef KRB5_BETA1
#define KRB5_BETA1
#endif

/* Protocol version number */
#define	KRB5_PVNO	5

/* Message types */

#define	KRB5_AS_REQ	((krb5_msgtype)10) /* Req for initial authentication */
#define	KRB5_AS_REP	((krb5_msgtype)11) /* Response to KRB_AS_REQ request */
#define	KRB5_TGS_REQ	((krb5_msgtype)12) /* TGS request to server */
#define	KRB5_TGS_REP	((krb5_msgtype)13) /* Response to KRB_TGS_REQ req */
#define	KRB5_AP_REQ	((krb5_msgtype)14) /* application request to server */
#define	KRB5_AP_REP	((krb5_msgtype)15) /* Response to KRB_AP_REQ_MUTUAL */
#define	KRB5_SAFE	((krb5_msgtype)20) /* Safe application message */
#define	KRB5_PRIV	((krb5_msgtype)21) /* Private application message */
#define	KRB5_CRED	((krb5_msgtype)22) /* Credential forwarding message */
#define	KRB5_ERROR	((krb5_msgtype)30) /* Error response */

/* LastReq types */
#define KRB5_LRQ_NONE			0
#define KRB5_LRQ_ALL_LAST_TGT		1
#define KRB5_LRQ_ONE_LAST_TGT		(-1)
#define KRB5_LRQ_ALL_LAST_INITIAL	2
#define KRB5_LRQ_ONE_LAST_INITIAL	(-2)
#define KRB5_LRQ_ALL_LAST_TGT_ISSUED	3
#define KRB5_LRQ_ONE_LAST_TGT_ISSUED	(-3)
#define KRB5_LRQ_ALL_LAST_RENEWAL	4
#define KRB5_LRQ_ONE_LAST_RENEWAL	(-4)
#define KRB5_LRQ_ALL_LAST_REQ		5
#define KRB5_LRQ_ONE_LAST_REQ		(-5)

/* PADATA types */
#define KRB5_PADATA_NONE                0
#define	KRB5_PADATA_AP_REQ		1
#define	KRB5_PADATA_TGS_REQ		KRB5_PADATA_AP_REQ
#define KRB5_PADATA_ENC_TIMESTAMP	2
#define	KRB5_PADATA_PW_SALT		3
#if 0				/* Not used */
#define KRB5_PADATA_ENC_ENCKEY          4  /* Key encrypted within itself */
#endif
#define KRB5_PADATA_ENC_UNIX_TIME       5  /* timestamp encrypted in key */
#define KRB5_PADATA_ENC_SANDIA_SECURID  6  /* SecurId passcode */
#define KRB5_PADATA_SESAME		7  /* Sesame project */
#define KRB5_PADATA_OSF_DCE		8  /* OSF DCE */

#define KRB5_CYBERSAFE_SECUREID		9  /* Cybersafe */
#define	KRB5_PADATA_AFS3_SALT		10 /* Cygnus */

#define KRB5_PADATA_ETYPE_INFO		11 /* Etype info for preauth */
#define KRB5_PADATA_SAM_CHALLENGE	12 /* draft challenge system */
#define KRB5_PADATA_SAM_RESPONSE	13 /* draft challenge system response */

#define KRB5_PADATA_ROOT_CERT     17  /* PA-PK-AS-ROOT */
#define KRB5_PADATA_PUBLIC_REP    18  /* PA-PK-AS-REP */

#define KRB5_PADATA_PUBLIC_REQ    19  /* PA-PK-AS-REQ */
#define KRB5_PADATA_PRIVATE_REP   20  /* PA-PK-USER-KEY */
#define KRB5_PADATA_PUBLIC_EXT    21  /* PA-PK-AS-EXT-CERT */
#define KRB5_PADATA_PUBLIC_SIGN   22  /* PA-PK-AS-SIGNED */



/* Reserved for SPX pre-authentication. */
#define KRB5_PADATA_DASS		16

/* Transited encoding types */
#define	KRB5_DOMAIN_X500_COMPRESS	1

/* alternate authentication types */
#define	KRB5_ALTAUTH_ATT_CHALLENGE_RESPONSE	64

/* authorization data types */
#define	KRB5_AUTHDATA_OSF_DCE	64

#ifdef KRB5_BETA1
/* principal name types */
#define KRB5_NT_UNKNOWN     0   /* Name type not known */
#define KRB5_NT_PRINCIPAL   1   /* Just the name of the principal as in DCE,
                                 * or for users */
#define KRB5_NT_SRV_INST    2   /* Service and other unique instance 
                                 * (krbtgt) */
#define KRB5_NT_SRV_HST     3   /* Service with host name as instance 
                                 * (telnet, rcommands) */
#define KRB5_NT_SRV_XHST    4   /* Service with host as remaining components */
#define KRB5_NT_UID         5   /* Unique ID */
#endif /* KRB5_BETA1 */

#endif /* KRB5_PROTO__ */
