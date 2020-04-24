/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: do_tgs_req.c,v $
 * Revision 1.1.17.4  1996/11/13  18:05:42  arvind
 * 	CHFts20690..93: fix retry logic.
 * 	[1996/11/07  21:04 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_5/1]
 *
 * 	bugfix: don't bit-bucket requests with bogus etypes..
 * 	[1996/11/05  02:12 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_4/1]
 *
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:51 UTC  psn  /main/DCE_1.2.2/DCE122_PK/1]
 *
 * 	Fix OT13224.
 * 	[1996/01/19  21:28 UTC  psn  /main/DCE_1.2/final_1.2.1_merge/1]
 *
 * 	Fix OT 13224.
 * 	[1995/12/19  14:32 UTC  hinxman  /main/HPDCE02/hinxman_ot13224/1]
 *
 * 	OT13224 - Bad call to decode_krb5_authdata
 * 	[1996/01/03  19:13 UTC  psn  /main/DCE_1.2/1]
 *
 * Revision 1.1.17.3  1996/10/03  14:58:50  arvind
 * 	with fix to coder, undo kludged workaround.
 * 	[1996/09/16  20:42 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 * 
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:51 UTC  psn  /main/DCE_1.2.2/DCE122_PK/1]
 * 
 * 	Fix OT13224.
 * 	[1996/01/19  21:28 UTC  psn  /main/DCE_1.2/final_1.2.1_merge/1]
 * 
 * 	Fix OT 13224.
 * 	[1995/12/19  14:32 UTC  hinxman  /main/HPDCE02/hinxman_ot13224/1]
 * 
 * 	OT13224 - Bad call to decode_krb5_authdata
 * 	[1996/01/03  19:13 UTC  psn  /main/DCE_1.2/1]
 * 
 * Revision 1.1.17.2  1996/07/08  18:35:53  arvind
 * 	Merge PSM, sec_pvtkey, sec_pubkey, dced, base defs for Public Key Login
 * 	[1996/06/04  18:51 UTC  psn  /main/DCE_1.2.2/DCE122_PK/1]
 * 
 * 	Implement transited path policy checking in the KDC; reject attempts
 * 	to create tickets which our realm's services would reject anyway due
 * 	to the transited path.
 * 	[1996/05/02  20:57 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts17874/3]
 * 
 * 	reinsert some OSF_DCE #ifdef's..
 * 	[1996/05/02  18:38 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts17874/2]
 * 
 * 	transited path cleanup.
 * 	realm alias cleanup.
 * 	remove some DCE-specific grunge.
 * 	[1996/04/26  22:56 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts17874/1]
 * 
 * 	Fix OT13224.
 * 	[1996/01/19  21:28 UTC  psn  /main/DCE_1.2/final_1.2.1_merge/1]
 * 
 * 	Fix OT 13224.
 * 	[1995/12/19  14:32 UTC  hinxman  /main/HPDCE02/hinxman_ot13224/1]
 * 
 * 	OT13224 - Bad call to decode_krb5_authdata
 * 	[1996/01/03  19:13 UTC  psn  /main/DCE_1.2/1]
 * 
 * 	Flush minimum ticket lifetime enforcement from TGS server. it's the
 * 	wrong thing to do here.
 * 	[1995/08/11  20:06 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothpatch/2]
 * 
 * 	clean up minimum ticket lifetime enforcement; always allow sender to
 * 	request ticket that expires when the TGT expires..
 * 	[1995/07/30  19:17 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts15483/1]
 * 
 * 	Merge doubly freed tickets fix from mb_mothra5
 * 	[1995/07/21  19:39 UTC  burati  /main/HPDCE02/3]
 * 
 * 	Don't free ticket twice on prepare_error_tgs
 * 	[1995/07/21  15:27 UTC  burati  /main/HPDCE02/mb_mothra5/1]
 * 
 * 	WP merge
 * 	[1995/05/17  23:56 UTC  rps  /main/HPDCE02/2]
 * 
 * 	Fix #ifdef inside macro call problem for AIX.
 * 	[1995/04/25  15:46 UTC  jrr  /main/DCE1_1_WP/2]
 * 
 * 	fuzzy realm compares
 * 	[1995/04/03  18:08 UTC  rps  /main/MOTHRA_panic/2]
 * 
 * 	aliases
 * 	[1995/03/14  01:09 UTC  rps  /main/rps_hc/2]
 * 
 * 	krb5b4p3 merge
 * 	[1995/03/08  21:41 UTC  rps  /main/rps_hc/1]
 * 
 * Revision 1.1.17.1  1996/06/04  22:00:35  arvind
 * 	Merge u2u:  Apply SKEY/TGT endtime  to generated tkt endtime in
 * 	process_tgs_req()
 * 	[1996/05/06  21:05 UTC  burati  /main/DCE_1.2/3]
 * 
 * 	merge  u2u work
 * 	[1996/04/30  15:00 UTC  burati  /main/DCE_1.2/mb_u2u/1]
 * 
 * 	u2u work off Mothra, before 1.2 branch is available
 * 	Apply SKEY/TGT endtime to generated ticket endtime in process_tgs_req()
 * 	[1996/04/29  21:34 UTC  burati  /main/HPDCE02/mb_mothra8/1]
 * 
 * Revision 1.1.13.4  1996/02/18  00:08:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:00:00  marty]
 * 
 * Revision 1.1.13.3  1996/02/07  17:48:20  parul
 * 	DCE 1.2.1 final drop from HP
 * 	[1996/02/07  16:14:06  parul]
 * 
 * 	DCE 1.2.1 final drop from HP
 * 
 * Revision 1.1.9.4  1994/10/07  18:30:43  rps
 * 	[OT12150] process_tgs_req:  call realm_compare() rather than own check
 * 	[1994/10/07  15:20:55  rps]
 * 
 * Revision 1.1.9.3  1994/07/15  15:00:29  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:17:47  mdf]
 * 
 * Revision 1.1.9.2  1994/05/11  19:19:24  ahop
 * 	hp_sec_to_osf_2 drop
 * 	remove include of int-proto.h since prototypes moved to func-proto.h
 * 	[1994/05/03  21:46:47  ahop]
 * 
 * Revision 1.1.9.1  1994/01/28  23:11:02  burati
 * 	Fix bug in decode_authdata where it was only copying data on error
 * 	[1994/01/20  04:32:25  burati]
 * 
 * Revision 1.1.7.1  1993/10/14  20:02:20  sommerfeld
 * 	Workaround MIT-DCE incompatibility in tgs request
 * 	[1993/10/14  15:52:26  sommerfeld]
 * 
 * Revision 1.1.5.2  1992/12/29  14:02:38  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:54:34  zeliff]
 * 
 * Revision 1.1.2.5  1992/05/28  20:58:31  sommerfeld
 * 	[CR3650] Use ADD macro rather than just "+" when adding timestamps.
 * 	Rearrange code to avoid nested "min's" to make life easier on
 * 	compilers.
 * 	[1992/05/27  22:27:51  sommerfeld]
 * 
 * Revision 1.1.2.4  1992/05/20  21:21:49  sommerfeld
 * 	Make all comparisons vs. good_since date include implicit clock skew
 * 	[1992/05/20  15:08:15  sommerfeld]
 * 
 * Revision 1.1.2.3  1992/05/13  22:07:33  sommerfeld
 * 	Change checks of good_since date to return official krb5 status codes.
 * 	Add check of server's good_since date.
 * 	[1992/05/13  17:39:47  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/04/01  17:12:42  ahop
 * 	 fix CR 2460: in process_tgs_req, set req_authdat to 0 after freeing
 * 	[1992/03/31  18:56:16  ahop]
 * 
 * Revision 1.1  1992/01/19  14:54:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*
**
** Copyright (c) Hewlett-Packard Company 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/kdc/do_tgs_req.c,v $
 * $Author: arvind $
 *
 * Copyright 1990,1991 by the Massachusetts Institute of Technology.
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
 * KDC Routines to deal with TGS_REQ's
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_do_tgs_req_c[] =
"$Id: do_tgs_req.c,v 1.1.17.4 1996/11/13 18:05:42 arvind Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/kdb.h>
#include <krb5/los-proto.h>
#include <krb5/asn1.h>
#include <krb5/osconf.h>
#include <krb5/ext-proto.h>
#include <com_err.h>

#include <syslog.h>
#ifdef KRB5_USE_INET
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#endif

#if defined(DCE_AUD_POINTS)
#include <dce/audit.h>
#include <sec_aud_evts.h>
#endif	/* defined(DCE_AUD_POINTS) */

#include <kdc_util.h>
#include <policy.h>
#include <extern.h>


static void find_alternate_tgs PROTOTYPE((krb5_kdc_req *,
#ifdef OSF_DCE
					  krb5_keytype *,
#endif
					  krb5_db_entry *,
					  krb5_boolean *,
					  int *));

static krb5_error_code prepare_error_tgs PROTOTYPE((krb5_kdc_req *,
						    krb5_ticket *,
						    int,
						    const char *,
						    krb5_data **));

#ifdef OSF_DCE
#include <dce/dce_cf_const.h>

#define has_null_key(dbe) ((dbe)->key.keytype == 0)

extern krb5_deltat krb5_clockskew;

#endif

#if defined(DCE_AUD_POINTS)
extern dce_aud_trail_t sec_aud_trail;
#endif	/* defined(DCE_AUD_POINTS) */
    
/*ARGSUSED*/
krb5_error_code
process_tgs_req(request, from, pkt, response)
krb5_kdc_req *request;
const krb5_fulladdr *from;		/* who sent it ? */
krb5_data *pkt;
krb5_data **response;			/* filled in with a response packet */
{
#ifdef OSF_DCE
    krb5_db_entry client;
    krb5_db_policy  kdc_policy;
    krb5_timestamp endtime;
    krb5_keytype usekeytype;
    int ncprincs = 0;
#endif
    krb5_db_entry server;
    krb5_kdc_rep reply;
    krb5_enc_kdc_rep_part reply_encpart;
    krb5_ticket ticket_reply, *header_ticket = 0;
    krb5_tkt_authent *req_authdat = 0;
    int st_idx = 0;
    krb5_enc_tkt_part enc_tkt_reply;
    krb5_transited enc_tkt_transited;
    int newtransited = 0;
    int nprincs = 0;
    krb5_boolean more;
    krb5_timestamp kdc_time, authtime;
    krb5_keyblock *session_key = 0;
    krb5_timestamp until, rtime;
    krb5_keyblock encrypting_key;
    char *cname = 0, *sname = 0, *tmp = 0, *fromstring = 0;
    krb5_last_req_entry *nolrarray[2], nolrentry;
/*    krb5_address *noaddrarray[1]; */
    krb5_enctype useetype;
    int	errcode, errcode2;
    register int i;
    int firstpass = 1;
    char	*status = 0;

#ifdef KRB5_USE_INET
    if (from->address->addrtype == ADDRTYPE_INET)
	fromstring =
	    (char *) inet_ntoa(*(struct in_addr *)from->address->contents);
#endif
#ifdef OSF_DCE
    if (from->address->addrtype == ADDRTYPE_DCE) {
        fromstring = (char *) from->address->contents;
    }
#endif /* OSF_DCE */
    if (!fromstring)
	fromstring = "<unknown>";

    if (errcode = krb5_unparse_name(request->server, &sname)) {
	status = "UNPARSING SERVER";
	goto cleanup;
    }

    errcode = kdc_process_tgs_req(request, from, pkt, &req_authdat);
    if (req_authdat)
	header_ticket = req_authdat->ticket;

    if (header_ticket && header_ticket->enc_part2 &&
	(errcode2 = krb5_unparse_name(header_ticket->enc_part2->client,
				      &cname))) {
	status = "UNPARSING CLIENT";
	errcode = errcode2;
	goto cleanup;
    }

    if (errcode) {
	status = "PROCESS_TGS";
	goto cleanup;
    }

    if (!header_ticket) {
	status="UNEXPECTED NULL in header_ticket";
	goto cleanup;
    }
    
    /*
     * We've already dealt with the AP_REQ authentication, so we can
     * use header_ticket freely.  The encrypted part (if any) has been
     * decrypted with the session key.
     */

    authtime = header_ticket->enc_part2->times.authtime;

    /* XXX make sure server here has the proper realm...taken from AP_REQ
       header? */

    for (i = 0; i < request->netypes; i++) {
	if (valid_etype(request->etype[i])) {

	    useetype = request->etype[i];
	    usekeytype = krb5_csarray[useetype]->system->proto_keytype;
	    
	    nprincs = 1;
	    if (errcode = krb5_db_get_principal(request->server,
						usekeytype,
						0,
						&server,
						&nprincs,
						&more)) {
		if (errcode == KRB5KDC_ERR_ETYPE_NOSUPP)
		    continue;
		status = "GET_PRINCIPAL";
		nprincs = 0;
		goto cleanup;
	    }
	    break;
	}
    }
    
    if (i == request->netypes) {
	/* unsupported etype */
	status = "BAD_ENCRYPTION_TYPE";
	errcode = KRB5KDC_ERR_ETYPE_NOSUPP;
	goto cleanup;
    }

tgt_again:
    if (more) {
	status = "NON_UNIQUE_PRINCIPAL";
	errcode = KRB5KDC_ERR_PRINCIPAL_NOT_UNIQUE;
	goto cleanup;
    } else if (nprincs != 1) {
	/*
	 * might be a request for a TGT for some other realm; we
	 * should do our best to find such a TGS in this db
	 */
	if (firstpass && krb5_princ_size(request->server) == 2) {
	    krb5_data *server_1 = krb5_princ_component(request->server, 1);
	    krb5_data *tgs_1 = krb5_princ_component(tgs_server, 1);

	    if (server_1->length != tgs_1->length ||
		memcmp(server_1->data, tgs_1->data, tgs_1->length)) {
		krb5_db_free_principal(&server, nprincs);
#ifdef OSF_DCE
		find_alternate_tgs(request, &usekeytype, &server, &more, &nprincs);
#else
		find_alternate_tgs(request, &server, &more, &nprincs);
#endif
		firstpass = 0;
		goto tgt_again;
	    }
	}
#ifndef OSF_DCE
	krb5_db_free_principal(&server, nprincs);
#endif
	status = "UNKNOWN_SERVER";
	errcode = KRB5KDC_ERR_S_PRINCIPAL_UNKNOWN;
	goto cleanup;
    }

    if (errcode = krb5_timeofday(&kdc_time)) {
	status = "TIME_OF_DAY";
	goto cleanup;
    }
    
    if (errcode = validate_tgs_request(request, server, header_ticket,
				      kdc_time, &status)) {
	if (!status)
	    status = "UNKNOWN_REASON";
	errcode = errcode + ERROR_TABLE_BASE_krb5;
	goto cleanup;
    }

#ifdef OSF_DCE
    if (has_null_key(&server)) {
	status = "NULL_KEY";
	errcode = KRB5KDC_ERR_NULL_KEY;
	goto cleanup;
    }

    if (isflagset(server.attributes, KRB5_KDB_DISALLOW_SERVER_TIX)
     && !dce_security_service_principal(server.principal)) {
	status = "DISALLOW_SERVER_TIX";
	errcode = KRB5KDC_ERR_POLICY;
	goto cleanup;
    }

    /*
     * Check good-since date of server principal.
     * 
     * Note that we do *not* check the client's authtime against the
     * server good since date, since the server could have been created 
     * since we logged in..
     */

    if ((server.good_since_date - kdc_time) > krb5_clockskew)
    {
	status = "SERVER_NOT_YET";
	errcode = KRB5KDC_ERR_SERVICE_NOTYET;
	goto cleanup;
    }

    /*
     * If the client is from this realm, then apply client policy checks
     */
    if (krb5_realm_equiv(krb5_princ_realm(tgs_server),
			 krb5_princ_realm(header_ticket->enc_part2->client))) {
        ncprincs = 1;
        if (errcode = krb5_db_get_principal(header_ticket->enc_part2->client,
					   usekeytype,
					   0,   /* use current key */
					   &client,
					   &ncprincs,
					   &more)) {
	    status = "GET_CLIENT_PRINCIPAL";
	    errcode = errcode + ERROR_TABLE_BASE_krb5;
	    goto cleanup;
        }

        if (more) {
	    status = "NON_UNIQUE_CLIENT_PRINCIPAL";
	    errcode = KRB5KDC_ERR_PRINCIPAL_NOT_UNIQUE;
	    goto cleanup;
        } else if (ncprincs != 1) {
	    status = "CLIENT_UNKNOWN";
	    errcode = KRB5KDC_ERR_C_PRINCIPAL_UNKNOWN;
	    goto cleanup;
	}

	
        if ((client.good_since_date - kdc_time) > krb5_clockskew) 
	{
	    status = "CLIENT_NOTYET";
	    errcode = KRB5KDC_ERR_CLIENT_NOTYET;
	    goto cleanup;
        }

        if ((client.good_since_date - 
	    header_ticket->enc_part2->times.authtime) > krb5_clockskew) 
	{
	    status = "CLIENT_REVOKED";
	    errcode = KRB5KDC_ERR_CLIENT_REVOKED;
	    goto cleanup;
        }
	
        if (isflagset(client.attributes, KRB5_KDB_DISALLOW_CLIENT_TIX)
	    && !dce_security_service_principal(client.principal)) {
	    status = "CLIENT_DISALLOWED";
	    errcode = KRB5KDC_ERR_POLICY;
	    goto cleanup;
        }
    }

    /*
     * get policy for realm--it's needed below
     */
    if (errcode = krb5_db_get_policy(&kdc_policy)) {
	status = "KDC_POLICY";
	errcode = errcode + ERROR_TABLE_BASE_krb5;
	goto cleanup;
    }

#else /* OSF_DCE */
    for (i = 0; i < request->netypes; i++)
	if (valid_etype(request->etype[i]))
	    break;
    if (i == request->netypes) {
	/* unsupported etype */
	status = "BAD_ENCRYPTION_TYPE";
	errcode = KRB5KDC_ERR_ETYPE_NOSUPP;
	goto cleanup;
    }
    useetype = request->etype[i];
#endif /* OSF_DCE */

    if (errcode = (*(krb5_csarray[useetype]->system->random_key))(krb5_csarray[useetype]->random_sequence, &session_key)) {
	/* random key failed */
	status = "RANDOM_KEY_FAILED";
	goto cleanup;
    }

    ticket_reply.server = request->server; /* XXX careful for realm... */
    ticket_reply.enc_part.etype = useetype;
    ticket_reply.enc_part.kvno = server.kvno;

    enc_tkt_reply.flags = 0;
    enc_tkt_reply.times.starttime = 0;

    /*
     * Fix header_ticket's starttime; if it's zero, fill in the
     * authtime's value.
     */
    if (!(header_ticket->enc_part2->times.starttime))
	header_ticket->enc_part2->times.starttime =
	    header_ticket->enc_part2->times.authtime;

    /* don't use new addresses unless forwarded, see below */

    enc_tkt_reply.caddrs = header_ticket->enc_part2->caddrs;
    /* noaddrarray[0] = 0; */
    reply_encpart.caddrs = 0;		/* optional...don't put it in */

    /* It should be noted that local policy may affect the  */
    /* processing of any of these flags.  For example, some */
    /* realms may refuse to issue renewable tickets         */

    if (isflagset(request->kdc_options, KDC_OPT_FORWARDABLE))
	setflag(enc_tkt_reply.flags, TKT_FLG_FORWARDABLE);

    if (isflagset(request->kdc_options, KDC_OPT_FORWARDED)) {
	setflag(enc_tkt_reply.flags, TKT_FLG_FORWARDED);

	/* include new addresses in ticket & reply */

	enc_tkt_reply.caddrs = request->addresses;
	reply_encpart.caddrs = request->addresses;
    }	
    if (isflagset(header_ticket->enc_part2->flags, TKT_FLG_FORWARDED))
	setflag(enc_tkt_reply.flags, TKT_FLG_FORWARDED);

    if (isflagset(request->kdc_options, KDC_OPT_PROXIABLE))
	setflag(enc_tkt_reply.flags, TKT_FLG_PROXIABLE);

    if (isflagset(request->kdc_options, KDC_OPT_PROXY)) {
	setflag(enc_tkt_reply.flags, TKT_FLG_PROXY);

	/* include new addresses in ticket & reply */

	enc_tkt_reply.caddrs = request->addresses;
	reply_encpart.caddrs = request->addresses;
    }

    if (isflagset(request->kdc_options, KDC_OPT_ALLOW_POSTDATE))
	setflag(enc_tkt_reply.flags, TKT_FLG_MAY_POSTDATE);

    if (isflagset(request->kdc_options, KDC_OPT_POSTDATED)) {
	setflag(enc_tkt_reply.flags, TKT_FLG_POSTDATED);
	setflag(enc_tkt_reply.flags, TKT_FLG_INVALID);
	enc_tkt_reply.times.starttime = request->from;
    } else
	enc_tkt_reply.times.starttime = kdc_time;

    if (isflagset(request->kdc_options, KDC_OPT_VALIDATE)) {
	/* BEWARE of allocation hanging off of ticket & enc_part2, it belongs
	   to the caller */
	ticket_reply = *(header_ticket);
	enc_tkt_reply = *(header_ticket->enc_part2);
	clear(enc_tkt_reply.flags, TKT_FLG_INVALID);
    }

    if (isflagset(request->kdc_options, KDC_OPT_RENEW)) {
	krb5_deltat old_life;

	/* BEWARE of allocation hanging off of ticket & enc_part2, it belongs
	   to the caller */
	ticket_reply = *(header_ticket);
	enc_tkt_reply = *(header_ticket->enc_part2);

	old_life = enc_tkt_reply.times.endtime - enc_tkt_reply.times.starttime;

	enc_tkt_reply.times.starttime = kdc_time;
	
#ifdef OSF_DCE
	endtime = ADD(kdc_time, old_life);
	
	enc_tkt_reply.times.endtime =
	    min(header_ticket->enc_part2->times.renew_till, endtime);
#else
	enc_tkt_reply.times.endtime =
	    min(header_ticket->enc_part2->times.renew_till,
		kdc_time + old_life);
#endif
    } else {
	/* not a renew request */
	enc_tkt_reply.times.starttime = kdc_time;
	until = (request->till == 0) ? kdc_infinity : request->till;
#ifdef OSF_DCE
	endtime = ADD(server.max_life, enc_tkt_reply.times.starttime);
	endtime = min (endtime, header_ticket->enc_part2->times.endtime);
	enc_tkt_reply.times.endtime = min(until, endtime);
	
#else
	enc_tkt_reply.times.endtime =
	    min(until, min(enc_tkt_reply.times.starttime + server.max_life,
			   min(enc_tkt_reply.times.starttime + max_life_for_realm,
			       header_ticket->enc_part2->times.endtime)));
#endif
	if (isflagset(request->kdc_options, KDC_OPT_RENEWABLE_OK) &&
	    (enc_tkt_reply.times.endtime < request->till) &&
	    isflagset(header_ticket->enc_part2->flags,
		  TKT_FLG_RENEWABLE)) {
	    setflag(request->kdc_options, KDC_OPT_RENEWABLE);
	    request->rtime =
		min(request->till,
		    header_ticket->enc_part2->times.renew_till);
	}
    }
    rtime = (request->rtime == 0) ? kdc_infinity : request->rtime;

    if (isflagset(request->kdc_options, KDC_OPT_RENEWABLE)) {
	/* already checked above in policy check to reject request for a
	   renewable ticket using a non-renewable ticket */
	setflag(enc_tkt_reply.flags, TKT_FLG_RENEWABLE);
#ifdef OSF_DCE
	endtime = ADD(enc_tkt_reply.times.starttime, server.max_renewable_life);
	endtime = min (endtime, header_ticket->enc_part2->times.renew_till);
	enc_tkt_reply.times.renew_till = min(rtime,endtime);
#else
	enc_tkt_reply.times.renew_till =
	    min(rtime,
		min(header_ticket->enc_part2->times.renew_till,
		    enc_tkt_reply.times.starttime +
		    min(server.max_renewable_life,
			max_renewable_life_for_realm)));
#endif
    } else {
	enc_tkt_reply.times.renew_till = 0;
    }
    
    /*
     * Set authtime to be the same as header_ticket's
     */
    enc_tkt_reply.times.authtime = header_ticket->enc_part2->times.authtime;
    
    /*
     * Propagate the preauthentication flags through to the returned ticket.
     */
    if (isflagset(header_ticket->enc_part2->flags, TKT_FLG_PRE_AUTH))
	setflag(enc_tkt_reply.flags, TKT_FLG_PRE_AUTH);

    if (isflagset(header_ticket->enc_part2->flags, TKT_FLG_HW_AUTH))
	setflag(enc_tkt_reply.flags, TKT_FLG_HW_AUTH);
    
    /* starttime is optional, and treated as authtime if not present.
       so we can nuke it if it matches */
    if (enc_tkt_reply.times.starttime == enc_tkt_reply.times.authtime)
	enc_tkt_reply.times.starttime = 0;

    /* assemble any authorization data */
#ifdef OSF_DCE
    /*
     * authorization data, if present, is encrypted under the client-supplied
     * subkey in the authentication header
     */
#endif
    if (request->authorization_data.ciphertext.data) {
	krb5_encrypt_block eblock;
	krb5_data scratch;

	/* decrypt the authdata in the request */
	if (!valid_etype(request->authorization_data.etype)) {
	    status = "BAD_AUTH_ETYPE";
	    errcode = KRB5KDC_ERR_ETYPE_NOSUPP;
	    goto cleanup;
	}
	/* put together an eblock for this encryption */

	krb5_use_cstype(&eblock, request->authorization_data.etype);

	scratch.length = request->authorization_data.ciphertext.length;
	if (!(scratch.data =
	      malloc(request->authorization_data.ciphertext.length))) {
	    status = "AUTH_NOMEM";
	    goto cleanup;
	}
	/* do any necessary key pre-processing */



#ifdef OSF_DCE
	if (errcode = krb5_process_key(&eblock,
				      req_authdat->authenticator->subkey ?
				      req_authdat->authenticator->subkey :
				      header_ticket->enc_part2->session)) {
#else
	if (errcode = krb5_process_key(&eblock,
				      header_ticket->enc_part2->session)) {
#endif
	    status = "AUTH_PROCESS_KEY";
	    free(scratch.data);
	    goto cleanup;
	}

	/* call the encryption routine */
	if (errcode = krb5_decrypt((krb5_pointer) request->authorization_data.ciphertext.data,
				  (krb5_pointer) scratch.data,
				  scratch.length, &eblock, 0)) {
	    status = "AUTH_ENCRYPT_FAIL";
	    (void) krb5_finish_key(&eblock);
	    free(scratch.data);
	    goto cleanup;
	}
	if (errcode = krb5_finish_key(&eblock)) {
	    status = "AUTH_FINISH_KEY";
	    free(scratch.data);
	    goto cleanup;
	}
	/* scratch now has the authorization data, so we decode it */
	errcode = decode_krb5_authdata(&scratch, &request->unenc_authdata);
	free(scratch.data);
	if (errcode) {
	    status = "AUTH_DECODE";
	    goto cleanup;
	}

	if (errcode =
	    concat_authorization_data(request->unenc_authdata,
				      header_ticket->enc_part2->authorization_data, 
				      &enc_tkt_reply.authorization_data)) {
	    status = "CONCAT_AUTH";
	    goto cleanup;
	}
    } else
	enc_tkt_reply.authorization_data =
	    header_ticket->enc_part2->authorization_data;

    enc_tkt_reply.session = session_key;
    enc_tkt_reply.client = header_ticket->enc_part2->client;
    enc_tkt_reply.transited.tr_type = KRB5_DOMAIN_X500_COMPRESS;
    enc_tkt_reply.transited.tr_contents = empty_string; /* equivalent of "" */

    /*
     * Only add the realm of the presented tgt to the transited list if 
     * it is different than the local realm (cross-realm) and it is different
     * than the realm of the client (since the realm of the client is already
     * implicitly part of the transited list and should not be explicitly
     * listed).
     */

    /* realm compare is like strcmp, but knows how to deal with these args */

    /* if either the granting realm is one of ours,
       or the granting realm and the client are equal (not just equivalent),
       then we need not add anything to the transited path; everything
       we would need to add is in there already */
#ifdef OSF_DCE
    if (krb5_realm_equiv(krb5_princ_realm(header_ticket->server),
			 krb5_princ_realm(tgs_server)) ||
	realm_compare(header_ticket->server, enc_tkt_reply.client) ) {
#else
    if (realm_compare(header_ticket->server, tgs_server) ||
	realm_compare(header_ticket->server, enc_tkt_reply.client)) {
#endif
	/* tgt issued by local realm or issued by realm of client */
	enc_tkt_reply.transited = header_ticket->enc_part2->transited;
    } else {
	/* tgt issued by some other realm and not the realm of the client */
	/* assemble new transited field into allocated storage */
	if (header_ticket->enc_part2->transited.tr_type !=
	    KRB5_DOMAIN_X500_COMPRESS) {
	    status = "BAD_TRTYPE";
	    errcode = KRB5KDC_ERR_TRTYPE_NOSUPP;
	    goto cleanup;
	}
	enc_tkt_transited.tr_type = KRB5_DOMAIN_X500_COMPRESS;
	enc_tkt_transited.tr_contents.data = 0;
	enc_tkt_transited.tr_contents.length = 0;
	enc_tkt_reply.transited = enc_tkt_transited;
	if (errcode =
	    add_to_transited(&header_ticket->enc_part2->transited.tr_contents,
			       &enc_tkt_reply.transited.tr_contents,
			       header_ticket->server,
			       enc_tkt_reply.client,
			       request->server)) {
	    status = "ADD_TR_FAIL";
	    goto cleanup;
	}
	/* check that the new transited-patch is acceptable to us */
	errcode = krb5_check_transited_list
	    (&enc_tkt_transited, krb5_princ_realm(enc_tkt_reply.client),
	     krb5_princ_realm(request->server));
	if (errcode != 0) {
	    status = "ADD_TR_POLICY_FAIL";
	    errcode = KRB5KDC_ERR_POLICY;
	    goto cleanup;
	}
	newtransited = 1;
    }

    ticket_reply.enc_part2 = &enc_tkt_reply;

    if (isflagset(request->kdc_options, KDC_OPT_ENC_TKT_IN_SKEY)) {
	krb5_keyblock *st_sealing_key;
	krb5_kvno st_srv_kvno;

	if (errcode = kdc_get_server_key(request->second_ticket[st_idx],
					&st_sealing_key,
					&st_srv_kvno)) {
	    status = "2ND_TKT_SERVER";
	    goto cleanup;
	}

	/* decrypt the ticket */
	errcode = krb5_decrypt_tkt_part(st_sealing_key,
				       request->second_ticket[st_idx]);
	krb5_free_keyblock(st_sealing_key);
	if (errcode) {
	    status = "2ND_TKT_DECRYPT";
	    goto cleanup;
	}

	/*
	 * Make sure the client for the second ticket matches
	 * requested server.
	 */
	if (!krb5_principal_compare(request->server,
				    request->second_ticket[st_idx]->enc_part2->client)) {
		if (errcode = krb5_unparse_name(request->second_ticket[st_idx]->enc_part2->client, &tmp))
			tmp = 0;
		syslog(LOG_INFO, "TGS_REQ: 2ND_TKT_MISMATCH: authtime %d, host %s, %s for %s, 2nd tkt client %s",
		       authtime, fromstring, cname, sname,
		       tmp ? tmp : "<unknown>");
		goto cleanup;
	}
#ifdef OSF_DCE
	/* Don't grant a useless ticket */
	if (request->second_ticket[st_idx]->enc_part2->times.endtime <
            enc_tkt_reply.times.starttime) {

            errcode = KRB5KDC_ERR_NEVER_VALID;
            goto cleanup;
        } 

	/* Apply the endtime of the second ticket to the ticket_reply */
        enc_tkt_reply.times.endtime = min(enc_tkt_reply.times.endtime,
            request->second_ticket[st_idx]->enc_part2->times.endtime);

#endif /* OSF_DCE */

	if (errcode = krb5_encrypt_tkt_part(request->second_ticket[st_idx]->enc_part2->session,
					   &ticket_reply)) {
	    status = "2ND_TKT_ENCRYPT";
	    goto cleanup;
	}
	st_idx++;
    } else {
	/* convert server.key into a real key (it may be encrypted
	   in the database) */
	if (errcode = KDB_CONVERT_KEY_OUTOF_DB(&server.key, &encrypting_key)) {
	    status = "CONV_KEY";
	    goto cleanup;
	}

	errcode = krb5_encrypt_tkt_part(&encrypting_key, &ticket_reply);

	memset((char *)encrypting_key.contents, 0, encrypting_key.length);
	krb5_xfree(encrypting_key.contents);

	if (errcode) {
	    status = "TKT_ENCRYPT";
	    goto cleanup;
	}
    }

    /* Start assembling the response */
    reply.msg_type = KRB5_TGS_REP;
    reply.padata = 0;		/* always */
    reply.client = header_ticket->enc_part2->client;
    reply.enc_part.etype = useetype;
    reply.enc_part.kvno = 0;		/* We are using the session key */
    reply.ticket = &ticket_reply;

    reply_encpart.session = session_key;
    reply_encpart.nonce = request->nonce;

    /* copy the time fields EXCEPT for authtime; its location
       is used for ktime */
    reply_encpart.times = enc_tkt_reply.times;
#ifdef KRB5_BETA1
    reply_encpart.times.authtime = kdc_time;
#else
    reply_encpart.times.authtime = header_ticket->enc_part2->times.authtime;
#endif

    /* starttime is optional, and treated as authtime if not present.
       so we can nuke it if it matches */
    if (enc_tkt_reply.times.starttime == enc_tkt_reply.times.authtime)
	enc_tkt_reply.times.starttime = 0;

    nolrentry.lr_type = KRB5_LRQ_NONE;
    nolrentry.value = 0;
    nolrarray[0] = &nolrentry;
    nolrarray[1] = 0;
    reply_encpart.last_req = nolrarray;	/* not available for TGS reqs */
    reply_encpart.key_exp = 0;		/* ditto */
    reply_encpart.flags = enc_tkt_reply.flags;
    reply_encpart.server = ticket_reply.server;
    
    /* use the session key in the ticket, unless there's a subsession key
       in the AP_REQ */

    errcode = krb5_encode_kdc_rep(KRB5_TGS_REP, &reply_encpart,
				 req_authdat->authenticator->subkey ?
				 req_authdat->authenticator->subkey :
				 header_ticket->enc_part2->session,
				 &reply, response);
    if (errcode) {
	status = "ENCODE_KDC_REP";
    } else {
	status = "ISSUE";
    }

    memset(ticket_reply.enc_part.ciphertext.data, 0,
	   ticket_reply.enc_part.ciphertext.length);
    free(ticket_reply.enc_part.ciphertext.data);
    /* these parts are left on as a courtesy from krb5_encode_kdc_rep so we
       can use them in raw form if needed.  But, we don't... */
    memset(reply.enc_part.ciphertext.data, 0,
	   reply.enc_part.ciphertext.length);
    free(reply.enc_part.ciphertext.data);

#if defined(DCE_AUD_POINTS)
    /* 
     * Full audit logic is included here as there are just too many
     * points of returns in the code above where a dangling dce_aud_start()
     * would never have a matching dce_aud_commit().  Handle the
     * "success" audit conditions here before returning.
     * 
     * There are three possible audit events: ticket request, renew request
     * and validate request.
     */

    unsigned32 aud_event_type;
    
    if (isflagset(request->kdc_options, KDC_OPT_RENEW))
        aud_event_type = TGS_RenewReq;
    else if (isflagset(request->kdc_options, KDC_OPT_VALIDATE))
        aud_event_type = TGS_ValidateReq;
    else
        aud_event_type = TGS_TicketReq;
    
    dce_aud_start(aud_event_type, h, 0, aud_c_esl_cond_success,
                  &ard, &aud_status);

    /* No event specific info associated with the TGS_*Req events. */

    if (ard) {    
        dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                       aud_c_esl_cond_success, &aud_status); 
    }     
#endif	/* defined(DCE_AUD_POINTS) */
    
cleanup:
    if (status)
        syslog(LOG_INFO, "TGS_REQ%c %s: authtime %d, host %s, %s for %s%s%s",
#ifdef OSF_DCE
	       ':', status, authtime, fromstring,
#else
	       secondary_ch, status, authtime, fromstring,
#endif
	       cname ? cname : "<unknown client>",
	       sname ? sname : "<unknown server>",
	       errcode ? ", " : "",
	       errcode ? error_message(errcode) : "");
    if (errcode) {
	errcode -= ERROR_TABLE_BASE_krb5;
	if (errcode < 0 || errcode > 128)
	    errcode = KRB_ERR_GENERIC;
	    
	errcode = prepare_error_tgs(request, header_ticket, errcode,
				   fromstring, response);
    }

#ifndef KRB5_BETA1
    if (request)
	krb5_free_kdc_req(request);
#endif
    if (req_authdat)
	krb5_free_tkt_authent(req_authdat);
    if (cname)
	free(cname);
    if (sname)
	free(sname);
    if (nprincs)
	krb5_db_free_principal(&server, 1);
#ifdef OSF_DCE
    if (ncprincs)
	krb5_db_free_principal(&client, 1);
#endif
    if (session_key)
	krb5_free_keyblock(session_key);
    if (newtransited)
	free(enc_tkt_reply.transited.tr_contents.data); 

    return errcode;
}

static krb5_error_code
prepare_error_tgs (request, ticket, error, ident, response)
register krb5_kdc_req *request;
krb5_ticket *ticket;
int error;
const char *ident;
krb5_data **response;
{
    krb5_error errpkt;
    krb5_error_code retval;
    krb5_data *scratch;

#if defined(DCE_AUD_POINTS)
    /* 
     * Handle the "error" audit conditions here.
     * 
     * There are three possible audit events: ticket request, renew request
     * and validate request.
     */

    unsigned32 aud_event_type;

    if (isflagset(request->kdc_options, KDC_OPT_RENEW))
        aud_event_type = TGS_RenewReq;
    else if (isflagset(request->kdc_options, KDC_OPT_VALIDATE))
        aud_event_type = TGS_ValidateReq;
    else
        aud_event_type = TGS_TicketReq;
    
    dce_aud_start(aud_event_type, h, 0, aud_c_esl_cond_failure,
                  &ard, &aud_status);

    /* No event specific info associated with the TGS_*Req events. */

    if (ard) {    
        dce_aud_commit(sec_aud_trail, ard, aud_c_evt_commit_sync_no_wait, 1, 
                       aud_c_esl_cond_failure, &aud_status); 
    }     
#endif	/* defined(DCE_AUD_POINTS) */

    errpkt.ctime = request->nonce;
    errpkt.cusec = 0;

    if (retval = krb5_us_timeofday(&errpkt.stime, &errpkt.susec))
	return(retval);
    errpkt.error = error;
    errpkt.server = request->server;
    if (ticket && ticket->enc_part2)
	errpkt.client = ticket->enc_part2->client;
    else
	errpkt.client = 0;
    errpkt.text.length = strlen(error_message(error+KRB5KDC_ERR_NONE))+1;
    if (!(errpkt.text.data = malloc(errpkt.text.length)))
	return ENOMEM;
    (void) strcpy(errpkt.text.data, error_message(error+KRB5KDC_ERR_NONE));

    if (!(scratch = (krb5_data *)malloc(sizeof(*scratch)))) {
	free(errpkt.text.data);
	return ENOMEM;
    }
    errpkt.e_data.length = 0;
    errpkt.e_data.data = 0;

    retval = krb5_mk_error(&errpkt, scratch);
    free(errpkt.text.data);
    *response = scratch;
    return retval;
}

/*
 * The request seems to be for a ticket-granting service somewhere else,
 * but we don't have a ticket for the final TGS.  Try to give the requestor
 * some intermediate realm.
 */
static void
find_alternate_tgs(request, keytype, server, more, nprincs)
krb5_kdc_req *request;
krb5_keytype *keytype;
krb5_db_entry *server;
krb5_boolean *more;
int *nprincs;
{
    krb5_error_code retval;
    krb5_principal *plist, *pl2;
    krb5_data tmp;

    *nprincs = 0;
    *more = FALSE;

    if (retval = krb5_walk_realm_tree(krb5_princ_realm(request->server),
				      krb5_princ_component(request->server, 1),
#ifdef OSF_DCE
				      &plist, PATH_COMPONENT_SEPARATOR))
#else
				      &plist, KRB5_REALM_BRANCH_CHAR))
#endif
	return;

    /* move to the end */
    for (pl2 = plist; *pl2; pl2++);

    /* the first entry in this array is for krbtgt/local@local, so we
       ignore it */
    while (--pl2 > plist) {
	*nprincs = 1;
	tmp = *krb5_princ_realm(*pl2);
	krb5_princ_set_realm(*pl2, krb5_princ_realm(tgs_server));
	retval = krb5_db_get_principal(*pl2, *keytype, 0, server, nprincs, more);
	krb5_princ_set_realm(*pl2, &tmp);
	if (retval) {
	    *nprincs = 0;
	    *more = FALSE;
	    krb5_free_realm_tree(plist);
	    return;
	}
	if (*more) {
	    krb5_db_free_principal(server, *nprincs);
	    continue;
	} else if (*nprincs == 1) {
	    /* Found it! */
	    krb5_principal tmpprinc;
	    char *sname;

	    tmp = *krb5_princ_realm(*pl2);
	    krb5_princ_set_realm(*pl2, krb5_princ_realm(tgs_server));
	    if (retval = krb5_copy_principal(*pl2, &tmpprinc)) {
		krb5_db_free_principal(server, *nprincs);
		krb5_princ_set_realm(*pl2, &tmp);
		continue;
	    }
	    krb5_princ_set_realm(*pl2, &tmp);

	    krb5_free_principal(request->server);
	    request->server = tmpprinc;
	    if (krb5_unparse_name(request->server, &sname)) {
		syslog(LOG_INFO,
		       "TGS_REQ: issuing alternate <un-unparseable> TGT");
	    } else {
		syslog(LOG_INFO,
		       "TGS_REQ: issuing TGT %s", sname);
		free(sname);
	    }
#ifdef OSF_DCE_XXX
	    krb5_free_realm_tree(plist);
#endif
	    return;
	}
	krb5_db_free_principal(server, *nprincs);
	continue;
    }

    *nprincs = 0;
    *more = FALSE;
    krb5_free_realm_tree(plist);
    return;
}

