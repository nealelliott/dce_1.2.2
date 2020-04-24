/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dispatch.c,v $
 * Revision 1.1.9.1  1996/10/03  14:58:21  arvind
 * 	merge up stuff from beta6
 * 	[1996/09/16  20:40 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.7.2  1996/02/18  00:08:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:59:55  marty]
 * 
 * Revision 1.1.7.1  1995/12/08  17:44:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:28  root]
 * 
 * Revision 1.1.5.1  1993/10/14  20:02:19  sommerfeld
 * 	Workaround MIT-DCE incompatibility in tgs request
 * 	[1993/10/14  15:52:16  sommerfeld]
 * 
 * Revision 1.1.3.3  1992/12/29  14:02:21  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:54:24  zeliff]
 * 
 * Revision 1.1.3.2  1992/10/07  20:29:49  sekhar
 * 	[OT 5566]  Bl5 Replication code drop from HP.
 * 	[1992/10/07  19:13:54  sekhar]
 * 
 * Revision 1.1.1.2  1992/10/06  15:51:12  burati
 * 	 Second replication code drop: bl5
 * 	dispatch - use BEGIN_READ(lock_db, &status) to lock dbase for
 * 	read and to prevent state change.
 * 
 * Revision 1.1  1992/01/19  14:54:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1991, 1992
 * Unpublished work. All Rights Reserved.
 *
 */
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/kdc/dispatch.c,v $
 * $Author: arvind $
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
 * Dispatch an incoming packet.
 */

#include <krb5/copyright.h>
#include <krb5/krb5.h>
#include <krb5/asn1.h>
#include <krb5/kdb.h>
#include <syslog.h>
#include "kdc_util.h"
#include <rs_lock.h>
#include <rs_ch_lock.h>

krb5_error_code
dispatch(pkt, from, portnum, response)
    krb5_data *pkt;
    const krb5_fulladdr *from;
    int portnum;
    krb5_data **response;
{
    volatile krb5_error_code retval = 0;
    krb5_kdc_req *as_req;
    krb5_kdc_req *tgs_req;

    error_status_t  status;

    CLEAR_STATUS(&status);

    BEGIN_READ(lock_db, &status) {

        /* Single thread access to the KDC code */
        WRITE_LOCK(lock_kdc) {
    
            /* decode incoming packet, and dispatch */
    
            /* try the replay lookaside buffer */
            if (kdc_check_lookaside(pkt, response)) {
                /* a hit! */
                syslog(LOG_INFO, "DISPATCH: replay found and re-transmitted");
                retval = 0;
                goto exit_point;
            }
    
            /* try TGS_REQ first; they are more common! */
            if (krb5_is_tgs_req(pkt)) {
                if (!(retval = decode_krb5_tgs_req(pkt, &tgs_req))) {
                    retval = process_tgs_req(tgs_req, from, pkt, response);
                    krb5_free_kdc_req(tgs_req);
                }
            } else if (krb5_is_as_req(pkt)) {
                if (!(retval = decode_krb5_as_req(pkt, &as_req))) {
                    retval = process_as_req(as_req, from, portnum, response);
                    krb5_free_kdc_req(as_req);
                }
            }
#ifdef KRB4
            else if (pkt->data[0] == 4)         /* old version */
                retval = process_v4(pkt, from, response);
#endif
            else
                retval = KRB5KRB_AP_ERR_MSG_TYPE;
            /* put the response into the lookaside buffer */
            if (!retval)
                kdc_insert_lookaside(pkt, *response);
    
    exit_point:
            ;   /* C insists on having a statement after a label */
    
        } END_WRITE_LOCK;

    } END_READ

    if (BAD_STATUS(&status)) {
        retval = status;
    }

    return retval;
}

