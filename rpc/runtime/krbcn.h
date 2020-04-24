/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: krbcn.h,v $
 * Revision 1.1.516.2  1996/02/18  22:56:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:15:31  marty]
 *
 * Revision 1.1.516.1  1995/12/08  00:20:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:59:35  root]
 * 
 * Revision 1.1.514.2  1994/09/21  19:07:41  sommerfeld
 * 	[OT10413] on client, retain & check timestamps for CN mutual authentication.
 * 	[1994/09/21  19:05:58  sommerfeld]
 * 
 * Revision 1.1.514.1  1994/01/21  22:37:52  cbrooks
 * 	RPC Code Cleanyp - Initial Submission
 * 	[1994/01/21  21:58:11  cbrooks]
 * 
 * Revision 1.1.4.4  1993/03/05  19:21:05  weisman
 * 	 09-jan-1993 burton      removed  rpc_g_krb_cn_epv
 * 	                              EXTERNAL declaration
 * 	[1993/03/05  19:09:40  weisman]
 * 
 * Revision 1.1.4.3  1993/01/03  23:25:55  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  20:07:58  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  20:51:52  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  15:39:44  zeliff]
 * 
 * Revision 1.1.2.3  1992/05/22  18:58:17  wei_hu
 * 	 28-mar-1992 sp      Add fields to rpc_krb_cn_info_t
 * 	 17-mar-1992 sp      Initial Version
 * 	[1992/05/22  18:45:23  wei_hu]
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**  NAME
**
**      krbcn.h
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  The Kerberos CN authentication module interface.
**
*/

#ifndef _KRBCN_H
#define _KRBCN_H	1

#include <cn.h> 

typedef struct 
{
    rpc_cn_auth_info_t  cn_info;

    /*
     * Kerberos specific fields here.
     */
    union
    {
        struct
        {
            sec_krb_timestamp   ctime;
            unsigned32          cusec;
        } server;
        struct
        {
	    sec_krb_timestamp	 ctime;	       /* client-side nonce */
	    unsigned32		 cusec;        /* client-side nonce */
        } client;
    } u;
    boolean32            encrypted_crc_valid;  /* for md checksum        */
    sec_des_block        encrypted_crc;/* encrypted CRC(assoc_uuid)      */
    sec_des_key          key;          /* cn session key                 */
    sec_des_key_schedule key_sched;    /* key schedule                   */
    sec_krb_cred         cred;         /* credentials                    */
    unsigned32           expiration;   /* expiration time of credentials */
    unsigned8            auth_subtype; /* indicates which checksum alg.  */
    unsigned8            auth_cksm_len;/* depends on auth_subtype        */
} rpc_krb_cn_info_t, *rpc_krb_cn_info_p_t;

typedef struct rpc_krb_cn_key_info_s_t
{
    boolean32            encrypted_crc_valid;
    sec_des_block        encrypted_crc;/* encrypted CRC(assoc_uuid)      */
    sec_des_key          key;          /* cn session key                 */
    sec_des_key_schedule key_sched;    /* key schedule                   */
    sec_krb_cred         cred;         /* credentials                    */
    unsigned32           expiration;   /* expiration time of credentials */
    unsigned8            auth_subtype; /* indicates which checksum alg.  */
    unsigned8            auth_cksm_len;/* depends on auth_subtype        */
} rpc_krb_cn_key_info_t, *rpc_krb_cn_key_info_p_t;

#endif /* _KRBCN_H */
