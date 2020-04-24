/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: policy.c,v $
 * Revision 1.1.4.2  1996/02/18  00:08:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:00:21  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:45:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:41  root]
 * 
 * Revision 1.1.2.2  1992/12/29  14:18:49  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:55:09  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:54:22  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  policy.c V=4 10/24/91 //littl/prgy/krb5/kdc
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/kdc/policy.c,v $
 * $Author: marty $
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
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
 * Policy decision routines for KDC.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_policy_c[] =
"$Id: policy.c,v 1.1.4.2 1996/02/18 00:08:58 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/copyright.h>


#include <krb5/krb5.h>
#include <krb5/kdb.h>

#include "kdc_util.h"

/*ARGSUSED*/
krb5_boolean
against_postdate_policy(fromtime)
krb5_timestamp fromtime;
{
    return FALSE;
}

krb5_boolean
against_flag_policy_as(request)
const register krb5_kdc_req *request;
{
    if (isflagset(request->kdc_options, KDC_OPT_FORWARDED) ||
	isflagset(request->kdc_options, KDC_OPT_PROXY) ||
	isflagset(request->kdc_options, KDC_OPT_RENEW) ||
	isflagset(request->kdc_options, KDC_OPT_VALIDATE) ||
	isflagset(request->kdc_options, KDC_OPT_ENC_TKT_IN_SKEY))
	return TRUE;			/* against policy */

    return FALSE;			/* not against policy */
}

krb5_boolean
against_flag_policy_tgs(request, ticket)
const register krb5_kdc_req *request;
const register krb5_ticket *ticket;
{

    if (((isflagset(request->kdc_options, KDC_OPT_FORWARDED) ||
	  isflagset(request->kdc_options, KDC_OPT_FORWARDABLE)) &&
	 !isflagset(ticket->enc_part2->flags,
		TKT_FLG_FORWARDABLE)) || /* TGS must be forwardable to get
					    forwarded or forwardable ticket */

	((isflagset(request->kdc_options, KDC_OPT_PROXY) ||
	  isflagset(request->kdc_options, KDC_OPT_PROXIABLE)) &&
	 !isflagset(ticket->enc_part2->flags,
		TKT_FLG_PROXIABLE)) ||	/* TGS must be proxiable to get
					   proxiable ticket */

	((isflagset(request->kdc_options, KDC_OPT_ALLOW_POSTDATE) ||
	  isflagset(request->kdc_options, KDC_OPT_POSTDATED)) &&
	 !isflagset(ticket->enc_part2->flags,
		TKT_FLG_MAY_POSTDATE)) || /* TGS must allow postdating to get
					   postdated ticket */
	 
	(isflagset(request->kdc_options, KDC_OPT_VALIDATE) &&
	 !isflagset(ticket->enc_part2->flags,
		TKT_FLG_INVALID)) || 	/* can only validate invalid tix */

	((isflagset(request->kdc_options, KDC_OPT_RENEW) ||
	  isflagset(request->kdc_options, KDC_OPT_RENEWABLE)) &&
	 !isflagset(ticket->enc_part2->flags,
		TKT_FLG_RENEWABLE))) 	/* can only renew renewable tix */

	return TRUE;			/* against policy */

    return FALSE;			/* XXX not against policy */
}
