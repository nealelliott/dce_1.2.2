/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: replay.c,v $
 * Revision 1.1.4.2  1996/02/18  00:09:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:00:23  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:45:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:43  root]
 * 
 * Revision 1.1.2.2  1992/12/29  14:18:59  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:55:16  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:54:16  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  replay.c V=1 06/06/91 //littl/prgy/krb5/kdc
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/kdc/replay.c,v $
 * $Author: marty $
 *
 * Copyright 1991 by the Massachusetts Institute of Technology.
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
 * Replay lookaside cache for the KDC, to avoid extra work.
 *
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_replay_c[] =
"$Id: replay.c,v 1.1.4.2 1996/02/18 00:09:00 marty Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <krb5/los-proto.h>
#include <krb5/kdb.h>
#include "kdc_util.h"

typedef struct _krb5_kdc_replay_ent {
    struct _krb5_kdc_replay_ent *next;
    int num_hits;
    krb5_int32 timein;
    krb5_data *req_packet;
    krb5_data *reply_packet;
} krb5_kdc_replay_ent;

static krb5_kdc_replay_ent root_ptr = {0};

static int hits = 0;
static int calls = 0;
static int max_hits_per_entry = 0;
static int num_entries = 0;

#define STALE_TIME	2*60		/* two minutes */
#define STALE(ptr) (abs((ptr)->timein - timenow) >= STALE_TIME)

#define MATCH(ptr) (((ptr)->req_packet->length == inpkt->length) && \
		    !memcmp((ptr)->req_packet->data, inpkt->data, inpkt->length))

/* XXX
   Todo:  quench the size of the queue...
 */

/* return TRUE if outpkt is filled in with a packet to reply with,
   FALSE if the caller should do the work */

krb5_boolean
kdc_check_lookaside(inpkt, outpkt)
register krb5_data *inpkt;
register krb5_data **outpkt;
{
    krb5_int32 timenow;
    register krb5_kdc_replay_ent *eptr, *last, *hold;

    if (krb5_timeofday(&timenow))
	return FALSE;

    calls++;

    /* search for a replay entry in the queue, possibly removing
       stale entries while we're here */

    if (root_ptr.next) {
	for (last = &root_ptr, eptr = root_ptr.next;
	     eptr;
	     eptr = eptr->next) {
	    if (MATCH(eptr)) {
		eptr->num_hits++;
		hits++;

		if (krb5_copy_data(eptr->reply_packet, outpkt))
		    return FALSE;
		else
		    return TRUE;
		/* return here, don't bother flushing even if it is stale.
		   if we just matched, we may get another retransmit... */
	    }
	    if (STALE(eptr)) {
		/* flush it and collect stats */
		max_hits_per_entry = max(max_hits_per_entry, eptr->num_hits);
		krb5_free_data(eptr->req_packet);
		krb5_free_data(eptr->reply_packet);
		hold = eptr;
		last->next = eptr->next;
		eptr = last;
		xfree(hold);
	    } else {
		/* this isn't it, just move along */
		last = eptr;
	    }
	}
    }
    return FALSE;
}

/* insert a request & reply into the lookaside queue.  assumes it's not
   already there, and can fail softly due to other weird errors. */

void
kdc_insert_lookaside(inpkt, outpkt)
register krb5_data *inpkt;
register krb5_data *outpkt;
{
    register krb5_kdc_replay_ent *eptr;    
    krb5_int32 timenow;

    if (krb5_timeofday(&timenow))
	return;

    /* this is a new entry */
    eptr = (krb5_kdc_replay_ent *)calloc(1, sizeof(*eptr));
    if (!eptr)
	return;
    eptr->timein = timenow;
    if (krb5_copy_data(inpkt, &eptr->req_packet)) {
	xfree(eptr);
	return;
    }
    if (krb5_copy_data(outpkt, &eptr->reply_packet)) {
	krb5_free_data(eptr->req_packet);
	xfree(eptr);
	return;
    }
    eptr->next = root_ptr.next;
    root_ptr.next = eptr;
    num_entries++;
    return;
}
