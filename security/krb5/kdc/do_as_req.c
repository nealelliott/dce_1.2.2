/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: do_as_req.c,v $
 * Revision 1.1.21.5  1996/11/13  18:05:27  arvind
 * 	CHFts20690..93: fix retry logic.
 * 	[1996/11/07  21:03 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_5/1]
 *
 * 	Add special case for KDC_ERR_POLICY in errout: block of
 * 	process_as_req()
 * 	[1996/11/05  22:09 UTC  aha  /main/DCE_1.2.2/aha_pk11/3]
 *
 * 	Fix incredibly embarrassing string comparison error in errout: block
 * 	of process_as_req
 * 	[1996/11/05  21:31 UTC  aha  /main/DCE_1.2.2/aha_pk11/2]
 *
 * 	CHFts20619: if check_padata() returns KDC_ERR_S_PRINCIPAL_UNKNOWN,
 * 	then always call prepare_enc_tp_error_edata() so intercell case can
 * 	be handled.
 * 	[1996/11/05  18:43 UTC  aha  /main/DCE_1.2.2/aha_pk11/1]
 *
 * 	Fix server side of Third-Party preauth
 * 	[1996/10/28  21:39 UTC  aha  /main/DCE_1.2.2/7]
 *
 * 	CHFts20575: initialize status in process_as_req()
 * 	[1996/10/30  16:09 UTC  aha  /main/DCE_1.2.2/aha_pk10/13]
 *
 * 	CHFts20576: initialize authtime in process_as_req()
 * 	[1996/10/28  20:11 UTC  aha  /main/DCE_1.2.2/aha_pk10/12]
 *
 * 	Allow check_padata to return an alternate client encrypting_key
 * 	[1996/10/24  20:26 UTC  aha  /main/DCE_1.2.2/aha_pk10/11]
 *
 * 	Remove unnecessary diffs
 * 	[1996/10/24  16:54 UTC  aha  /main/DCE_1.2.2/aha_pk10/10]
 *
 * 	Move hook for invalid login to depend on check_padata
 * 	[1996/10/23  19:05 UTC  aha  /main/DCE_1.2.2/aha_pk10/9]
 *
 * 	Move verify_preauth_policy() into check_padata() and, for OSF_DCE,
 * 	call check_padata() whether padata present or not.
 * 	[1996/10/22  19:00 UTC  aha  /main/DCE_1.2.2/aha_pk10/8]
 *
 * 	Cleanup.
 * 	[1996/10/21  20:22 UTC  aha  /main/DCE_1.2.2/aha_pk10/7]
 *
 * 	Fix errcode handling between prepare_enc_tp_error_edate and prepare_error_as
 * 	[1996/10/21  18:35 UTC  aha  /main/DCE_1.2.2/aha_pk10/6]
 *
 * 	Remove #if 0
 * 	[1996/10/21  14:46 UTC  aha  /main/DCE_1.2.2/aha_pk10/5]
 *
 * 	Change signature on prepare_enc_tp_error_edata()
 * 	[1996/10/18  20:21 UTC  aha  /main/DCE_1.2.2/aha_pk10/4]
 *
 * 	Re-insert calls to prepare_enc_tp_error_edata() in the OSF_DCE 1.1
 * 	backwards compatibility case.  OSF DCE 1.2.2 e_data structure is not
 * 	backwards compatible with pre-OSF DCE 1.2.2 e_data.
 * 	[1996/10/17  14:38 UTC  aha  /main/DCE_1.2.2/aha_pk10/3]
 *
 * 	Put change passwords code back in
 * 	[1996/10/16  14:37 UTC  aha  /main/DCE_1.2.2/aha_pk10/2]
 *
 * 	Fix build errors
 * 	[1996/10/15  17:30 UTC  aha  /main/DCE_1.2.2/aha_pk10/1]
 *
 * 	CHFts20121: make Third-Party Pre-Authentication work under the KRB5
 * 	Beta 6 preauth framework; second pass
 * 	[1996/10/14  17:09 UTC  aha  /main/DCE_1.2.2/aha_pk9_5/5]
 *
 * 	CHFts20121: get Third-Party Pre-Authentication working again; first pass
 * 	[1996/10/08  21:06 UTC  aha  /main/DCE_1.2.2/aha_pk9_5/4]
 *
 * 	Fix build errors
 * 	[1996/09/30  19:13 UTC  aha  /main/DCE_1.2.2/aha_pk9_5/2]
 *
 * 	fix obvious double-free error.
 * 	[1996/09/27  19:35 UTC  sommerfeld  /main/DCE_1.2.2/3]
 *
 * 	Add pre-auth policy checks
 * 	[1996/09/30  13:42 UTC  aha  /main/DCE_1.2.2/aha_pk9_4/2]
 *
 * 	Merge-up from sommerfeld_pk_kdc_1
 * 	[1996/09/27  14:05 UTC  aha  /main/DCE_1.2.2/aha_pk9_4/1]
 *
 * 	CHFts20121: make Third-Party Pre-Authentication work under the KRB5
 * 	Beta 6 preauth framework; second pass
 * 	[1996/10/08  21:06 UTC  aha  /main/DCE_1.2.2/aha_pk9_5/4]
 *
 * 	CHFts20121: get Third-Party Pre-Authentication working again; first pass
 * 	[1996/10/02  18:32 UTC  aha  /main/DCE_1.2.2/aha_pk9_5/3]
 *
 * 	Fix build errors
 * 	[1996/09/30  19:13 UTC  aha  /main/DCE_1.2.2/aha_pk9_5/2]
 *
 * 	fix obvious double-free error.
 * 	[1996/09/27  19:35 UTC  sommerfeld  /main/DCE_1.2.2/3]
 *
 * 	Add pre-auth policy checks
 * 	[1996/09/30  13:42 UTC  aha  /main/DCE_1.2.2/aha_pk9_4/2]
 *
 * 	Merge-up from sommerfeld_pk_kdc_1
 * 	[1996/09/27  14:05 UTC  aha  /main/DCE_1.2.2/aha_pk9_4/1]
 *
 * 	process_as_req(): initialize session key to NULL to avoid freeing
 * 	uninitialized memory in errout.
 * 	[1996/09/30  14:22 UTC  mullan_s  /main/DCE_1.2.2/mullan_pk_bugs/1]
 *
 * 	Fix more potential memory corruption problems in process_as_req().
 * 	[1996/09/27  19:35 UTC  sommerfeld  /main/DCE_1.2.2/3]
 *
 * Revision 1.1.21.4  1996/10/15  20:50:43  arvind
 * 	Plug memory leak through reply.padata.
 * 	[1996/10/11  00:02 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/2]
 * 
 * Revision 1.1.21.3  1996/10/04  16:57:00  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  16:56:45  arvind]
 * 
 * Revision /main/DCE_1.2.2/5  1996/10/03  18:46 UTC  sommerfeld
 * 	CHFts20137: avoid blackholing failure status.
 * 	[1996/10/03  17:13 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/1]
 * 
 * Revision /main/DCE_1.2.2/4  1996/10/01  19:59 UTC  mullan_s
 * 	process_as_req(): initialize session key to NULL to avoid freeing
 * 	uninitialized memory in errout.
 * 	[1996/10/01  14:53 UTC  mullan_s  /main/DCE_1.2.2/mullan_pk_bugs/2]
 * 
 * 	Fix more potential memory corruption problems in process_as_req().
 * 	[1996/09/30  14:22 UTC  mullan_s  /main/DCE_1.2.2/mullan_pk_bugs/1]
 * 
 * Revision /main/DCE_1.2.2/3  1996/09/27  19:35 UTC  sommerfeld
 * 	fix obvious double-free error.
 * 
 * Revision /main/DCE_1.2.2/2  1996/09/26  23:25 UTC  sommerfeld
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/11  17:57 UTC  aha  /main/DCE_1.2.2/1]
 * 
 * 	Pick up new preauth module interface from KRB5beta6 (sort of).
 * 
 * Revision /main/DCE_1.2.2/1  1996/06/11  17:57 UTC  aha
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 
 * Revision /main/DCE122_PK/1  1996/06/04  18:51 UTC  psn
 * 	Update OSF copyright years
 * 	[1996/02/17  22:59:57  marty]
 * 	 *
 * 
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/4  1995/05/03  19:10 UTC  mdf
 * 	Resolve CHFts14998
 * 	[1995/05/03  17:44 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_4/1]
 * 
 * 	HP revision /main/HPDCE02/3  1995/05/01  18:51 UTC  mdf
 * 	"Resolve CHFts14998"
 * 	[1995/04/26  12:07 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_3/1]
 * 
 * 	HP revision /main/HPDCE02/2  1995/04/15  00:00 UTC  rps
 * 	copyright
 * 	[1995/04/05  17:17 UTC  rps  /main/HPDCE02/MOTHRA_panic/2]
 * 
 * 	new type for TT
 * 	[1995/03/08  21:40 UTC  rps  /main/HPDCE02/rps_hc/1]
 * 
 * 	HP revision /main/HPDCE02/1  1994/12/14  20:22 UTC  mullan_s
 * 
 * 	HP revision /main/mullan_mothra_chfts13565/1  1994/12/14  20:13 UTC  mullan_s
 * 	Disallow logins with no padata if pre_auth_req ERA=1 (timestamps).
 * 	[1995/12/08  16:57:29  root]
 * 
 * Revision /main/DCE_1.2/DCE12_PK/1  1996/05/30  21:54 UTC  sommerfeld
 * 	Add prototype here for check_padata to silence a compiler warning.
 * 	[1996/05/08  19:44 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts17874/1]
 * 
 * 	A few more review changes.
 * 	[1996/03/15  22:04 UTC  jrr  /main/HPDCE02/jrr_davis_1/3]
 * 
 * 	Code cleanup from review for last fix.
 * 	[1996/03/15  14:58 UTC  jrr  /main/HPDCE02/jrr_davis_1/2]
 * 
 * 	Modify the code to more accurately track invalid login attempts.
 * 	[1996/03/06  16:31 UTC  jrr  /main/HPDCE02/jrr_davis_1/1]
 * 
 * Revision /main/DCE_1.2/1  1996/01/03  19:13 UTC  psn
 * 	Resolve CHFts14998
 * 	[1995/05/03  17:44 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_4/1]
 * 
 * 	"Resolve CHFts14998"
 * 	[1995/04/26  12:07 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_3/1]
 * 
 * 	copyright
 * 	[1995/04/05  17:17 UTC  rps  /main/HPDCE02/MOTHRA_panic/2]
 * 
 * 	new type for TT
 * 	[1995/03/08  21:40 UTC  rps  /main/HPDCE02/rps_hc/1]
 * 
 * 	[1994/12/14  20:22 UTC  mullan_s  /main/HPDCE02/1]
 * 
 * 	Disallow logins with no padata if pre_auth_req ERA=1 (timestamps).
 * 	[1994/12/14  20:13 UTC  mullan_s  /main/mullan_mothra_chfts13565/1]
 * 
 * Revision 1.1.7.10  1994/09/19  15:00:28  burati
 * 	CR12234 Fix 3 memory corruption problems in process_as_req()
 * 	[1994/09/16  22:45:35  burati]
 * 
 * Revision 1.1.7.9  1994/09/06  19:14:08  mullan_s
 * 	[OT11812] Partial fix for memory leaks causing secd to crash.
 * 	[1994/09/06  19:13:41  mullan_s]
 * 
 * Revision 1.1.7.8  1994/08/31  20:00:03  desai
 * 	Merged with changes from 1.1.7.7
 * 	[1994/08/31  19:59:56  desai]
 * 
 * 	Fix for OT CR 11976. Remove unnecessary memset from dce_padata.
 * 	[1994/08/31  19:51:11  desai]
 * 
 * 	Fix for CR 11949. Added return for the default path in a switch statement and initialized variable.
 * 	[1994/08/31  16:36:59  desai]
 * 
 * Revision 1.1.7.7  1994/08/31  16:37:26  desai
 * 	Fix for CR 11949. Added return for the default path in a switch statement and initialized variable.
 * 	[1994/08/31  16:36:59  desai]
 * 
 * Revision 1.1.7.6  1994/08/29  15:51:38  hondo
 * 	OT 11919 --login activity idl change to support different address types
 * 	[1994/08/28  21:50:12  hondo]
 * 
 * Revision 1.1.7.5  1994/08/26  18:44:30  mullan_s
 * 	[OT11557] Subtype DCE preauth data
 * 	[1994/08/25  18:35:06  mullan_s]
 * 
 * Revision 1.1.7.4  1994/08/24  20:14:43  mullan_s
 * 	[OT11556] Update DCE preauth types to match Kerberos spec.
 * 	[1994/08/24  20:13:59  mullan_s]
 * 
 * Revision 1.1.9.2  1994/08/16  17:40:09  mullan_s
 * 	[OT11489] 1.03 dce_login crashes 1.1 secd replica. Check
 * 	that request->padata is not NULL before inspecting contents.
 * 
 * Revision 1.1.7.2  1994/07/15  15:00:28  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:17:43  mdf]
 * 
 * Revision 1.1.5.3  1993/04/16  19:55:27  sommerfeld
 * 	[CR7733] don't die if request->addresses is NULL (as it might be..)
 * 	[1993/04/16  02:05:09  sommerfeld]
 * 
 * Revision 1.1.5.2  1992/12/29  14:02:29  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:54:29  zeliff]
 * 
 * Revision 1.1.2.5  1992/05/28  20:58:13  sommerfeld
 * 	[CR3650] Use ADD macro (which handles overflow correctly)
 * 	rather than just "+".
 * 	Rearrange use of "min" to make life easier on the compiler.
 * 	[1992/05/27  22:27:23  sommerfeld]
 * 
 * Revision 1.1.2.4  1992/05/20  21:21:44  sommerfeld
 * 	Make all comparisons vs. good_since date include implicit clock skew.
 * 	[1992/05/20  15:08:06  sommerfeld]
 * 
 * Revision 1.1.2.3  1992/05/13  22:07:14  sommerfeld
 * 	[CR3389] Return appropriate "good_since"-related error statuses
 * 	[CR3398] Clip min, max lifetime appropriately.
 * 	[1992/05/13  17:39:18  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/05/08  17:05:20  sommerfeld
 * 	[CR3229] Don't issue initial tickets if "good_since" time is in the future.
 * 	[1992/05/05  02:55:49  sommerfeld]
 * 
 * Revision 1.1  1992/01/19  14:54:27  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  do_as_req.c V=6 11/14/91 //littl/prgy/krb5/kdc
**
** Copyright (c) Hewlett-Packard Company 1991, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/kdc/do_as_req.c,v $
 * $Author: arvind $
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
 * KDC Routines to deal with AS_REQ's
 */

#ifdef OSF_DCE
#define OSF_DCE_ARCHAISM
#define OSF_DCE_FEATURE
#endif

#include <krb5/krb5.h>
#include <krb5/kdb.h>
#include <krb5/preauth.h>
#include <krb5/los-proto.h>
#include <krb5/asn1.h>
#include <krb5/osconf.h>
#include <com_err.h>

#include <krb5/ext-proto.h>
#include <dce/nbase.h>
#include <dce/rgynbase.h>
#include <dce/lm_login.h>
#include <dce/rs_login_activity.h>
#include <macros.h>

#include <syslog.h>
#ifdef KRB5_USE_INET
#include <sys/types.h>
#include <netinet/in.h>
#ifndef hpux
#include <arpa/inet.h>
#endif	/* hpux */
#endif /* KRB5_USE_INET */

#include "kdc_util.h"
#include "policy.h"
#include "extern.h"

#if defined(DCE_AUD_POINTS)
#include <dce/audit.h>
#include <sec_aud_evts.h>
#endif	/* defined(DCE_AUD_POINTS) */

static krb5_error_code prepare_error_as PROTOTYPE((krb5_kdc_req *,
						   int,
						   krb5_data *,
						   krb5_data **));



#ifdef KRB5_BETA1
/*
 * Do all the processing required for a AS_REQ
 */

/* XXX needs lots of cleanup and modularizing */
#define has_null_key(dbe) ((dbe)->key.keytype == 0)

extern krb5_deltat krb5_clockskew;
#endif /* KRB5_BETA1 */

#if defined(DCE_AUD_POINTS)
extern dce_aud_trail_t sec_aud_trail;
#endif	/* defined(DCE_AUD_POINTS) */

/* check_padata() has moved into kdc_preauth.c */

/*ARGSUSED*/
krb5_error_code
process_as_req(request, from, portnum, response)
register krb5_kdc_req *request;
const krb5_fulladdr *from;		/* who sent it ? */
int	portnum;
krb5_data **response;			/* filled in with a response packet */
{

    krb5_db_entry client, server;
#ifdef KRB5_BETA1              
    krb5_db_policy  kdc_policy; 
#endif /* KRB5_BETA1 */
    krb5_kdc_rep reply;
    krb5_enc_kdc_rep_part reply_encpart;
    krb5_ticket ticket_reply;
    krb5_enc_tkt_part enc_tkt_reply;
    krb5_error_code errcode=0, sub_err=0;
    int c_nprincs = 0, s_nprincs = 0;
    char *cpw_service;
    int pwreq, pa_id, pa_flags;
    krb5_boolean more;
#ifdef KRB5_BETA1
    krb5_timestamp kdc_time=0, endtime=0;
    krb5_keytype usekeytype;
#endif /* KRB5_BETA1 */
    krb5_timestamp authtime=0;
    krb5_keyblock *session_key = NULL;
#ifdef OSF_DCE_FEATURE
    krb5_keyblock client_enc_key;
    krb5_keyblock svr_enc_key;
#else
    krb5_keyblock encrypting_key;
#endif
    krb5_enctype useetype;
    krb5_pa_data *padat_tmp[2], padat_local;
    const char *status=NULL;
    unsigned long st;
    krb5_data e_data;
#ifdef OSF_DCE_FEATURE
    lm_upd_action_t action;
    krb5_address *fromp;
    boolean32               invalid_login = FALSE;
    boolean32               use_enc_tp_edata=TRUE;
    boolean32               do_salt_padata=FALSE;
#endif /* OSF_DCE */

    register int i;
    krb5_data *data_foo=NULL, data_bar;
    krb5_timestamp until, rtime;

    char *cname = 0, *sname = 0, *fromstring = 0;

    memset(&reply, 0, sizeof(reply));
    ticket_reply.enc_part.ciphertext.data = 0;
    e_data.data = 0;
#ifdef OSF_DCE_FEATURE
    client_enc_key.contents = 0;
    svr_enc_key.contents = 0;
#else
    encrypting_key.contents = 0;
#endif

#ifdef OSF_DCE_FEATURE
    memset((char*)&action,0,sizeof(lm_upd_action_t));
    CLEAR_STATUS(&st);
#endif /* OSF_DCE */
    memset((char*)&data_bar,0,sizeof(krb5_data));

#ifdef KRB5_USE_INET
    if (from->address->addrtype == ADDRTYPE_INET)
	fromstring = (char *) inet_ntoa(*(struct in_addr *)from->address->contents);
#endif
#ifdef OSF_DCE
    if (from->address->addrtype == ADDRTYPE_DCE) {
        fromstring = (char *) from->address->contents;
    }
#endif /* OSF_DCE */

    if (!fromstring)
	fromstring = "<unknown>";

    if (!request->client) {
	status = "NULL_CLIENT";
	errcode = KRB5KDC_ERR_C_PRINCIPAL_UNKNOWN;
	goto errout;
    }

    if ((errcode = krb5_unparse_name(/*kdc_context,*/request->client, &cname))) {
	status = "UNPARSING_CLIENT";
	goto errout;
    }
    if (!request->server) {
	status = "NULL_SERVER";
	errcode = KRB5KDC_ERR_S_PRINCIPAL_UNKNOWN;
	goto errout;
    }
    if ((errcode = krb5_unparse_name(/*kdc_context,*/ request->server, &sname))) {
	status = "UNPARSING_SERVER";
	goto errout;
    }

#ifdef OSF_DCE
     fromp=from->address;
     lm_get_origin_from_addr(&action,fromp);
#endif /* OSF_DCE */

    /*
     * Special considerations are allowed when changing passwords. Is
     * this request for changepw?
     *
     * XXX This logic should be moved someplace else, perhaps the
     * site-specific policiy file....
     */
    pwreq = 0;

    cpw_service = malloc(krb5_princ_realm(request->server)->length + 19);
    strcpy(cpw_service, "changepw/kerberos@");
    memcpy(cpw_service, krb5_princ_realm(request->server)->data,
        krb5_princ_realm(request->server)->length);
    cpw_service[18+krb5_princ_realm(request->server)->length] = '\0';

    if (strcmp(sname, cpw_service) == 0) pwreq++;
    free(cpw_service);

    for (i = 0; i < request->netypes; i++) {
        if (valid_etype(request->etype[i])) {
	    useetype = request->etype[i];
	    usekeytype = krb5_csarray[useetype]->system->proto_keytype;

	    c_nprincs = 1;
	    if (errcode = krb5_db_get_principal(request->client, 
#ifdef KRB5_BETA1
						usekeytype,
						0,   /* use current key */
#endif /* KRB5_BETA1 */
						&client, 
						&c_nprincs,
						&more)) {
		if (errcode == KRB5KDC_ERR_ETYPE_NOSUPP)
		    continue;
		status = "LOOKING_UP_CLIENT";
		c_nprincs = 0;
		goto errout;
	    }
	    break;
	}
    }

    
    if (i == request->netypes) {
        /* unsupported etype */
	status = "BAD_ENCRYPTION_TYPE";
	errcode = KRB5KDC_ERR_ETYPE_NOSUPP;
	goto errout;
    }

    if (more) {
	status = "NON-UNIQUE_CLIENT";
	errcode = KRB5KDC_ERR_PRINCIPAL_NOT_UNIQUE;
	goto errout;
    } else if (c_nprincs != 1) {
	status = "CLIENT_NOT_FOUND";
#ifdef KRBCONF_VAGUE_ERRORS
	errcode = KRB5KRB_ERR_GENERIC;
#else
	errcode = KRB5KDC_ERR_C_PRINCIPAL_UNKNOWN;
#endif
	goto errout;
    }
    
    if (isflagset(client.attributes, KRB5_KDB_DCE_ACCT_NOT_VALID)) {
        errcode=KDC_ERR_POLICY;
#ifdef OSF_DCE
        sub_err=OSF_DCE_ERR_ADMIN_VALID;
#endif /* OSF_DCE */
        goto errout;
    }

#ifdef KRB5_BETA1
    if (isflagset(client.attributes, KRB5_KDB_DISALLOW_CLIENT_TIX)
    && !dce_security_service_principal(client.principal)) {
       status = "CLIENT_TIX_DISALLOWED";
       errcode = KRB5KDC_ERR_POLICY;
       goto errout;
    }
#endif /* KRB5_BETA1 */

    s_nprincs = 1;
    if (errcode = krb5_db_get_principal(request->server, 
#ifdef KRB5_BETA1
                                       krb5_csarray[useetype]->system->proto_keytype,
                                       0,   /* use current key */
#endif /* KRB5_BETA1 */
                                       &server, 
                                       &s_nprincs,
				       &more)) {
	status = "LOOKING_UP_SERVER";
	goto errout;
    }
    if (more) {
	status = "NON-UNIQUE_SERVER";
	errcode = KRB5KDC_ERR_PRINCIPAL_NOT_UNIQUE;
	goto errout;
    } else if (s_nprincs != 1) {
	status = "SERVER_NOT_FOUND";
	errcode = KRB5KDC_ERR_S_PRINCIPAL_UNKNOWN;
	goto errout;
    }

#ifdef OSF_DCE_FEATURE
    if (has_null_key(&client) || has_null_key(&server)) {
	status = "NULL_KEY";
	errcode = KRB5KDC_ERR_NULL_KEY;
	goto errout;
    }

    if (isflagset(server.attributes, KRB5_KDB_DISALLOW_SERVER_TIX)
    && !dce_security_service_principal(server.principal)) {
       status = "SERVER_TIX_DISALLOWED";
       errcode = KRB5KDC_ERR_POLICY;
       goto errout;
    }
#endif /* OSF_DCE_FEATURE */

    if (errcode = krb5_timeofday(&kdc_time)) {
	status = "TIMEOFDAY";
	goto errout;
    }

#ifdef OSF_DCE_FEATURE
    if ((client.good_since_date - kdc_time) > krb5_clockskew)
    {
	errcode = KRB5KDC_ERR_CLIENT_NOTYET;
	status = "CLIENT_NOTYET";
	goto errout;
    }

    if (errcode = krb5_db_get_policy(&kdc_policy)) {
	status = "NO_POLICY";
	goto errout;
    }
#endif /* KRB5_BETA1 */
    status = "UNKNOWN REASON";
    if (errcode = validate_as_request(request, &client, server,
				     kdc_time, &status)) {
	if (!status) 
	    status = "UNKNOWN_REASON";
	errcode += ERROR_TABLE_BASE_krb5;
	goto errout;
    }
      
    if (errcode = (*(krb5_csarray[useetype]->system->random_key))
	(krb5_csarray[useetype]->random_sequence, &session_key)) {
	/* random key failed */
	status = "RANDOM_KEY_FAILED";
	goto errout;
    }

    ticket_reply.server = request->server;
    ticket_reply.enc_part.etype = useetype;
    ticket_reply.enc_part.kvno = server.kvno;

    enc_tkt_reply.flags = 0;
    setflag(enc_tkt_reply.flags, TKT_FLG_INITIAL);

    	/* It should be noted that local policy may affect the  */
        /* processing of any of these flags.  For example, some */
        /* realms may refuse to issue renewable tickets         */

#ifdef KRB5_BETA1
    if (against_flag_policy_as(request)) {
	errcode = KRB5KDC_ERR_BADOPTION;
	status = "AGAINST_AS_FLAG_POLICY";
	goto errout;
    }
#endif /* KRB5_BETA1 */

    if (isflagset(request->kdc_options, KDC_OPT_FORWARDABLE))
	setflag(enc_tkt_reply.flags, TKT_FLG_FORWARDABLE);

    if (isflagset(request->kdc_options, KDC_OPT_PROXIABLE))
	    setflag(enc_tkt_reply.flags, TKT_FLG_PROXIABLE);

    if (isflagset(request->kdc_options, KDC_OPT_ALLOW_POSTDATE))
	    setflag(enc_tkt_reply.flags, TKT_FLG_MAY_POSTDATE);

    enc_tkt_reply.session = session_key;
    enc_tkt_reply.client = request->client;
    enc_tkt_reply.transited.tr_type = KRB5_DOMAIN_X500_COMPRESS;
    enc_tkt_reply.transited.tr_contents = empty_string; /* equivalent of "" */

    enc_tkt_reply.times.authtime = kdc_time;

    if (isflagset(request->kdc_options, KDC_OPT_POSTDATED)) {
#ifdef KRB5_BETA1
	if (against_postdate_policy(request->from)) {
	    errcode = KRB5KDC_ERR_POLICY;
	    status = "AGAINST_AS_POSTDATE_POLICY";
	    goto errout;
	}
#endif /* KRB5_BETA1 */
	setflag(enc_tkt_reply.flags, TKT_FLG_INVALID);
	enc_tkt_reply.times.starttime = request->from;
    } else
	enc_tkt_reply.times.starttime = kdc_time;

#ifdef OSF_DCE_FEATURE
    until = (request->till == 0) 
            ? ADD(enc_tkt_reply.times.starttime, kdc_policy.default_life)
            : request->till;

    /* Logic to enforce minimum ticket lifetimes removed here; it
     * caused too many problems, yet had no significant benefit
     */
    
    endtime = min (server.max_life, kdc_policy.max_life);
    endtime = min (client.max_life, endtime);
    endtime = ADD (enc_tkt_reply.times.starttime, endtime);
    enc_tkt_reply.times.endtime = min(until, endtime);

    /* XXX why && request->till ? */
    if (isflagset(request->kdc_options, KDC_OPT_RENEWABLE_OK) && 
	request->till && (enc_tkt_reply.times.endtime < request->till)) 
#else
    until = (request->till == 0) ? kdc_infinity : request->till;

    enc_tkt_reply.times.endtime =
        min(until,
            min(enc_tkt_reply.times.starttime + client.max_life,
                  min(enc_tkt_reply.times.starttime + server.max_life,
                    enc_tkt_reply.times.starttime + max_life_for_realm)));

    if (isflagset(request->kdc_options, KDC_OPT_RENEWABLE_OK) &&
        !isflagset(client.attributes, KRB5_KDB_DISALLOW_RENEWABLE) &&
        (enc_tkt_reply.times.endtime < request->till))
#endif /* OSF_DCE_FEATURE */   
    {
	/* we set the RENEWABLE option for later processing */

	setflag(request->kdc_options, KDC_OPT_RENEWABLE);
	request->rtime = request->till;
    }

    rtime = (request->rtime == 0) ? kdc_infinity : request->rtime;

    if (isflagset(request->kdc_options, KDC_OPT_RENEWABLE)) {
	/*
	 * XXX Should we squelch the output renew_till to be no
	 * earlier than the endtime of the ticket? 
	 */
	setflag(enc_tkt_reply.flags, TKT_FLG_RENEWABLE);
#ifdef KRB5_BETA1
	endtime = min (client.max_renewable_life, server.max_renewable_life);
	endtime = min (endtime, kdc_policy.max_renewable_life);
	endtime = ADD (enc_tkt_reply.times.starttime, endtime);
	
	enc_tkt_reply.times.renew_till = min(rtime, endtime);
#else
        enc_tkt_reply.times.renew_till =
            min(rtime, enc_tkt_reply.times.starttime +
                min(client.max_renewable_life,
                    min(server.max_renewable_life,
                        max_renewable_life_for_realm)));
#endif /* KRB5_BETA1 */

    } else
	enc_tkt_reply.times.renew_till = 0; /* XXX */

    /* starttime is optional, and treated as authtime if not present.
       so we can nuke it if it matches */
    if (enc_tkt_reply.times.starttime == enc_tkt_reply.times.authtime)
	enc_tkt_reply.times.starttime = 0;

    enc_tkt_reply.caddrs = request->addresses;
    enc_tkt_reply.authorization_data = 0;

    /*
     * Check the disable field for invalid login attempts
     */

#ifdef OSF_DCE
    if(client.disable) {
	status = "Client locked out";
	errcode = KRB5KRB_AP_ERR_BAD_INTEGRITY;
	goto errout;
    }
#endif /* OSF_DCE */

    /*
     * convert client.key into a real key (it may be encrypted in the database) 
     */
#ifdef OSF_DCE_FEATURE
    if (errcode = KDB_CONVERT_KEY_OUTOF_DB(&client.key, &client_enc_key)) {
#else
    if (errcode = KDB_CONVERT_KEY_OUTOF_DB(&client.key, &encrypting_key)) {
#endif
#if 0   /* last_req is currently static data */
	if (reply_encpart.last_req)
	    krb5_free_last_req(reply_encpart.last_req);
#endif
	status = "DECRYPT_CLIENT_KEY";
	goto errout;
    }

    /* 
     * Check preauthentication, assuming it's there.
     */

#ifdef OSF_DCE_FEATURE
    /* After this point, we know enough about the client to generate
     * a salt_padata if there are errors
     */
    do_salt_padata=TRUE;
#endif

#ifndef OSF_DCE_FEATURE
    if (request->padata) {
#else
    if (TRUE) {
        /* check_padata_type() logic is now part of check_padata() */
#endif /* OSF_DCE_FEATURE */
#ifdef OSF_DCE_FEATURE
	errcode = check_padata(kdc_context, &client, request, &enc_tkt_reply, &client_enc_key);
#else
	errcode = check_padata(kdc_context, &client, request, &enc_tkt_reply);
#endif
	if (errcode) {
#ifdef OSF_DCE_FEATURE
            /* invalid_login logic moved to errout label */
            invalid_login=TRUE;
#endif
	    status = "PREAUTH_FAILED";
#ifdef KRBCONF_VAGUE_ERRORS
	    errcode = KRB5KRB_ERR_GENERIC;
#endif /* KRBCONF_VAGUE_ERRORS */
	    goto errout;
        }
    }

    /*
     * Final check before handing out ticket: If the client requires
     * Hardware authentication, verify ticket flag is set
     */  
    
    status = missing_required_preauth(&client, &server, &enc_tkt_reply);
    if (status) {
	errcode = KRB5KDC_ERR_PREAUTH_REQUIRED;
	goto errout;
    }

    /* KRB5_KDB_REQUIRES_HW_AUTH check moved into
     * missing_required_preauth() function
     */

    ticket_reply.enc_part2 = &enc_tkt_reply;

    /* convert server.key into a real key (it may be encrypted
       in the database) */
#ifdef OSF_DCE_FEATURE
    if (errcode = KDB_CONVERT_KEY_OUTOF_DB(&server.key, &svr_enc_key)) {
#else
    if (errcode = KDB_CONVERT_KEY_OUTOF_DB(&server.key, &encrypting_key)) {
#endif
	errcode = KRB5KDC_ERR_NULL_KEY;
	status = "SERVER_NULL_KEY";
	goto errout;
    }
#ifdef OSF_DCE_FEATURE
    errcode = krb5_encrypt_tkt_part(&svr_enc_key, &ticket_reply);
    memset((char *)svr_enc_key.contents, 0, svr_enc_key.length);
    xfree(svr_enc_key.contents);
    svr_enc_key.contents = NULL;
#else
    errcode = krb5_encrypt_tkt_part(&encrypting_key, &ticket_reply);
    memset((char *)encrypting_key.contents, 0, encrypting_key.length);
    xfree(encrypting_key.contents);
    encrypting_key.contents = NULL;
#endif
    
    if (errcode) {
	status = "ENCRYPT_FAIL";
	goto errout;
    }

    /* Start assembling the response */
    reply.msg_type = KRB5_AS_REP;

    /* Salt padata is now generated in return_padata() */

    reply.client = request->client;
    /* XXX need separate etypes for ticket encryption and kdc_rep encryption */
    reply.enc_part.etype = useetype;
    reply.enc_part.kvno = client.kvno;
    reply.ticket = &ticket_reply;

    reply_encpart.session = session_key;
    if (errcode = fetch_last_req_info(&client, &reply_encpart.last_req)) {
	status = "FETCH_LAST_REQ";
	goto errout;
    }

    /* client.key processing moved to precede call to check_padata() */

    reply_encpart.nonce = request->nonce;
    reply_encpart.key_exp = client.expiration;
    reply_encpart.flags = enc_tkt_reply.flags;
    reply_encpart.server = ticket_reply.server;

    /* copy the time fields EXCEPT for authtime; it's location
       is used for ktime */
    reply_encpart.times = enc_tkt_reply.times;
    reply_encpart.times.authtime = authtime = kdc_time;

    reply_encpart.caddrs = enc_tkt_reply.caddrs;

    /* Fetch the padata to be returned */
    reply.padata = 0;

#ifdef OSF_DCE_FEATURE
    errcode = return_padata(kdc_context, &client, request, &reply, &client.key,
			    &client_enc_key);
#else
    errcode = return_padata(kdc_context, &client, request, &reply, &client.key,
			    &encrypting_key);
#endif
    if (errcode) {
	status = "KDC_RETURN_PADATA";
	goto errout;
    }

    /* now encode/encrypt the response */

#ifdef OSF_DCE_FEATURE
    errcode = krb5_encode_kdc_rep(KRB5_AS_REP, &reply_encpart,
				 &client_enc_key,  &reply, response);
    memset((char *)client_enc_key.contents, 0, client_enc_key.length);
    xfree(client_enc_key.contents);
    client_enc_key.contents = NULL;
#else
    errcode = krb5_encode_kdc_rep(KRB5_AS_REP, &reply_encpart,
				 &encrypting_key,  &reply, response);
    memset((char *)encrypting_key.contents, 0, encrypting_key.length);
    xfree(encrypting_key.contents);
    encrypting_key.contents = NULL;
#endif
#if 0   /* last_req is currently static data */
    if (reply_encpart.last_req)
        krb5_free_last_req(reply_encpart.last_req);
#endif

    if (errcode) {
	syslog(LOG_INFO, "AS_REQ; ENCODE_KDC_REP: host %s, %s for %s failed: %s",
	       fromstring, cname, sname, error_message(errcode));
    } else {
#ifndef KRB5_BETA1
	if (is_secondary)
	    syslog(LOG_INFO, "AS_REQ; ISSUE: authtime %d, host %s, %s for %s",
		   authtime, fromstring, cname, sname);
	else
#endif /* KRB5_BETA1 */
	    syslog(LOG_INFO, "AS_REQ: ISSUE: authtime %d, host %s, %s for %s",
		   authtime, fromstring, cname, sname);
    }
    /* these parts are left on as a courtesy from krb5_encode_kdc_rep so we
       can use them in raw form if needed.  But, we don't... */
    memset(reply.enc_part.ciphertext.data, 0,
	   reply.enc_part.ciphertext.length);
    free(reply.enc_part.ciphertext.data);

#ifdef OSF_DCE
    /* Since this was a valid login, clear the
     * invalid login counter if invalid logins
     * are being controlled for this client
     */
    if (client.max_attempts) {
        action.type=lm_valid_attempt;
        lm_update_login_info(client.principal, &action, &st);
    }
#endif /* OSF_DCE */

#if defined(DCE_AUD_POINTS)
    /* 
     * Full audit logic is included here as there are just too many
     * points of returns in the code above where a dangling dce_aud_start()
     * would never have a matching dce_aud_commit().  Handle the
     * "success" audit conditions here before returning.
     */
    
    dce_aud_start(AS_Request, h, 0, aud_c_esl_cond_success,
                  &ard, &aud_status);

    /* No event specific info associated with a AS_REQUEST event. */

    if (ard) {    
        dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1,
                       aud_c_esl_cond_success, &aud_status); 
    }     
#endif	/* defined(DCE_AUD_POINTS) */

  errout:
#ifdef OSF_DCE
    /* All invalid login handling previously scattered above
     * is now done here
     */
    if (invalid_login) {
        if (client.max_attempts) {
            if ((client.fail_auth_count + 1) < client.max_attempts){
                action.type=lm_invalid_attempt;
                lm_update_login_info(client.principal,
                                     &action, &st);
            }
            else{
                action.type=lm_disable;
                lm_update_login_info(client.principal,
                                     &action, &st);
            }
        }
    }
#endif /* OSF_DCE */
    if (status)
        krb5_klog_syslog(LOG_INFO, "AS_REQ %s(%d): %s: %s for %s%s%s",
	       fromstring, portnum, status, 
	       cname ? cname : "<unknown client>",
	       sname ? sname : "<unknown server>",
	       errcode ? ", " : "",
	       errcode ? error_message(errcode) : "");
    if (errcode) {
#ifdef OSF_DCE_ARCHAISM
        /* Determine whether client is definitely DCE 1.2.2 or later.
         * use_enc_tp_edata may also have been set FALSE above, if some other
         * DCE 1.2.2-specific input was noticed there.
         */
        if ((errcode == KDC_ERR_S_PRINCIPAL_UNKNOWN) && (strcmp(status, "PREAUTH_FAILED") == 0)) {
            /* This indicates an intercell preauth failure.  Return
             * the enc_tp_error_edata, since there is no KRB5 error to
             * use in this case
             */
            use_enc_tp_edata=TRUE;
        } else if (errcode == KDC_ERR_POLICY) {
            /* OSF_DCE has a special case for invalid account; we need to
             * return tp_edata in this case, but not return any e_data
             * otherwise for this error type
             */
            use_enc_tp_edata=(sub_err == OSF_DCE_ERR_ADMIN_VALID);
            do_salt_padata=FALSE;
	} else if (request->padata && use_enc_tp_edata) {
            krb5_pa_data **padata;
	    for (padata = request->padata; *padata && use_enc_tp_edata; padata++) {
                switch ((*padata)->pa_type) {
                case KRB5_PADATA_ENC_UNIX_TIME:
                case KRB5_PADATA_OSF_DCE:
                    /* No information */
                    break;
                default:
                    use_enc_tp_edata=FALSE;
                    break;
		}
	    }
	}
        
        /* DCE 1.1 returns a special e_data structure that is not
         * compatible with preauth_hint_list.  For backwards
         * compatibility with pre-1.1 clients, we generate the
         * old e_data
         */
        if (use_enc_tp_edata) {
            krb5_error_code errcode1;
            errcode1=prepare_enc_tp_error_edata(request,errcode,
                    sub_err, &e_data, &client);
            /* If prepare_enc_tp_error_edata() itself has an
             * error, then reset the errcode to the new error;
             * otherwise, leave the current errcode as is
             */
            if (errcode1)
                errcode=errcode1;
        } else if (do_salt_padata)
#endif /* OSF_DCE_ARCHAISM */
        get_preauth_hint_list(request, &client, &server, &e_data);

	errcode -= ERROR_TABLE_BASE_krb5;
	if (errcode < 0 || errcode > 128)
	    errcode = KRB_ERR_GENERIC;
	    
	errcode = prepare_error_as(request, errcode, &e_data, response);
    }
    if (reply.padata) {
	krb5_free_pa_data(/*context, */reply.padata);
    }
#ifdef OSF_DCE_FEATURE
    if (svr_enc_key.contents) {
	memset((char *)svr_enc_key.contents, 0, svr_enc_key.length);
	krb5_xfree(svr_enc_key.contents);
    }
    if (client_enc_key.contents) {
	memset((char *)client_enc_key.contents, 0, client_enc_key.length);
	krb5_xfree(client_enc_key.contents);
    }
#else
    if (encrypting_key.contents) {
	memset((char *)encrypting_key.contents, 0, encrypting_key.length);
	krb5_xfree(encrypting_key.contents);
    }
#endif
    if (cname)
	    free(cname);
    if (sname)
	    free(sname);
    if (c_nprincs) {
#if 0
#ifdef	KRBCONF_KDC_MODIFIES_KDB
	if (update_client) {
	    krb5_db_put_principal(kdc_context, &client, &c_nprincs);
	    /*
	     * ptooey.  We want krb5_db_sync() or something like that.
	     */
	    krb5_db_fini(kdc_context);
	    if (kdc_active_realm->realm_dbname)
		krb5_db_set_name(kdc_active_realm->realm_context,
				 kdc_active_realm->realm_dbname);
	    krb5_db_init(kdc_context);
	}
#endif	/* KRBCONF_KDC_MODIFIES_KDB */
#endif
	krb5_db_free_principal(/*kdc_context,*/ &client, c_nprincs);
    }
    if (s_nprincs)
	krb5_db_free_principal(/*kdc_context,*/ &server, s_nprincs);
    if (session_key)
	krb5_free_keyblock(/*kdc_context,*/ session_key);
    if (ticket_reply.enc_part.ciphertext.data) {
	memset(ticket_reply.enc_part.ciphertext.data , 0,
	       ticket_reply.enc_part.ciphertext.length);
	free(ticket_reply.enc_part.ciphertext.data);
    }
    if (e_data.data)
	krb5_xfree(e_data.data);

    return errcode;
}

static krb5_error_code
prepare_error_as (request, error, e_data, response)
register krb5_kdc_req *request;
int error;
krb5_data *e_data;
krb5_data **response;
{
    krb5_error errpkt;
    krb5_error_code retval;
    krb5_data *scratch;
    
    errpkt.ctime = request->nonce;
    errpkt.cusec = 0;

#if defined(DCE_AUD_POINTS)
    /* 
     * Handle the "error" audit conditions here.
     */
    
    dce_aud_start(AS_Request, h, 0, aud_c_esl_cond_failure,
                  &ard, &aud_status);

    /* No event specific info associated with a AS_REQUEST event. */

    if (ard) {    
        dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                       aud_c_esl_cond_failure, &aud_status); 
    }     
#endif	/* defined(DCE_AUD_POINTS) */

    if ((retval = krb5_us_timeofday(/* kdc_context, */&errpkt.stime,
				    &errpkt.susec)))
	return(retval);
    errpkt.error = error;
    errpkt.server = request->server;
    errpkt.client = request->client;
    errpkt.text.length = strlen(error_message(error+KRB5KDC_ERR_NONE))+1;
    if (!(errpkt.text.data = malloc(errpkt.text.length)))
	return ENOMEM;
    (void) strcpy(errpkt.text.data, error_message(error+KRB5KDC_ERR_NONE));

    if (!(scratch = (krb5_data *)malloc(sizeof(*scratch)))) {
	free(errpkt.text.data);
	return ENOMEM;
    }
    if (e_data && e_data->data) {
	errpkt.e_data = *e_data;
    } else {
	errpkt.e_data.length = 0;
	errpkt.e_data.data = 0;
    }

    retval = krb5_mk_error(/* kdc_context, */&errpkt, scratch);
    free(errpkt.text.data);
    *response = scratch;
    return retval;
}
