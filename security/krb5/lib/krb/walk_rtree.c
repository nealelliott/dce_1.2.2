/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: walk_rtree.c,v $
 * Revision 1.1.10.1  1996/10/03  15:04:17  arvind
 * 	fix compiler warnings.
 * 	[1996/09/16  21:18 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.8.3  1996/02/18  00:14:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:04:19  marty]
 * 
 * Revision 1.1.8.2  1995/12/08  18:02:07  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/15  00:04 UTC  rps
 * 	copyright
 * 	[1995/04/05  17:19 UTC  rps  /main/MOTHRA_panic/3]
 * 
 * 	rework check_transited
 * 	[1995/03/21  23:55 UTC  rps  /main/MOTHRA_panic/2]
 * 
 * 	finish transit path check
 * 	[1995/03/21  23:49 UTC  rps  /main/rps_hc/2]
 * 
 * 	krb5b4p3 merge & DCE-flavored realm-walker
 * 	[1995/03/08  21:45 UTC  rps  /main/rps_hc/1]
 * 
 * Revision 1.1.4.1  1994/05/11  19:24:06  ahop
 * 	hp_sec_to_osf_1 drop
 * 	include int-proto.h with <> instead of ""
 * 	[1994/05/03  19:35:26  ahop]
 * 
 * Revision 1.1.2.2  1992/12/29  15:59:27  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:16:12  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:52:14  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  walk_rtree.c V=3 10/24/91 //littl/prgy/krb5/lib/krb
**
** Copyright (c) Hewlett-Packard Company 1991, 1995
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/krb/walk_rtree.c,v $
 * $Author: arvind $
 *
 * Copyright (c) 1994 CyberSAFE Corporation.
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
 * krb5_walk_realm_tree()
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_walk_rtree_c[] =
"$Id: walk_rtree.c,v 1.1.10.1 1996/10/03 15:04:17 arvind Exp $";
#endif	/* !lint & !SABER */

#include <krb5/krb5.h>
#include <krb5/ext-proto.h>
#include <int-proto.h>

/* internal function, used by krb5_get_cred_from_kdc() */

/*XXXrps
 Needs to canonicalize aliases; direct trust shortcut optimization
*/
 /*
  * k r b 5 _ w a l k _ r e a l m _ t r e e 
  *
  * Cloned fairly literally from original Kerberos, modified for
  * hierarchy orderings that are "parent/child" rather than "child.parent"
  */

krb5_error_code
#ifdef NARROW_PROTOTYPES
krb5_walk_realm_tree(const krb5_data *client, const krb5_data *server,
		     krb5_principal **tree, char realm_branch_char)
#else
krb5_walk_realm_tree(client, server, tree, realm_branch_char)
const krb5_data *client, *server;
krb5_principal **tree;
char realm_branch_char;
#endif
{
    krb5_error_code retval;
    krb5_principal *rettree;
    register char *ccp, *scp;
    register char *prevccp = 0, *prevscp = 0;
    char *com_ssep = 0, *com_csep = 0;
    register int i, links = 0;
    int clen, slen;
    krb5_data tmpcrealm, tmpsrealm;
    int nocommon = 1;

    clen = client->length;
    slen = server->length;

    for (com_csep = ccp = client->data,
	 com_ssep = scp = server->data;
	 clen && slen && *ccp == *scp ;
	 ccp++, scp++, 	clen--, slen--) {
	if (*ccp == realm_branch_char) {
	    com_csep = ccp;
	    com_ssep = scp;
	    nocommon = 0;
	}
    }

    /* ccp, scp point to common root.
       com_csep, com_ssep point to common components. */
    /* handle case of one ran out */
    if (!clen) {
	/* construct path from client to server, down the tree */
	if (!slen)
	    /* in the same realm--this means there is no ticket
	       in this realm. */
	    return KRB5_NO_TKT_IN_RLM;
	if (*scp == realm_branch_char) {
	    /* one is a subdomain of the other */
	    com_csep = client->data;
	    com_ssep = scp;
	} /* else normal case of two sharing parents */
    }
    if (!slen) {
	/* construct path from client to server, up the tree */
	if (*ccp == realm_branch_char) {
	    /* one is a subdomain of the other */
	    com_ssep = server->data;
	    com_csep = ccp;
	} /* else normal case of two sharing parents */
    }
    /* determine #links to/from common ancestor */
    if (nocommon)
	links = 1;
    else
	links = 2;
    /* if no common ancestor, artificially set up common root at the last
       component, then join with special code */
    for (ccp = client->data + client->length - 1; ccp >= com_csep; ccp--) {
	if (*ccp == realm_branch_char) {
	    links++;
	    if (nocommon)
		prevccp = ccp;
	}
    }

    for (scp = server->data + server->length - 1; scp >= com_ssep; scp--) {
	if (*scp == realm_branch_char) {
	    links++;
	    if (nocommon)
		prevscp = scp;
	}
    }
    if (nocommon) {
	if (prevccp)
	    com_csep = prevccp;
	if (prevscp)
	    com_ssep = prevscp;

	if(com_csep == client->data)
	   com_csep = client->data + client->length ;
	if(com_ssep == server->data)
	   com_ssep = server->data + server->length ;
    }

    if (!(rettree = (krb5_principal *)calloc(links+2,
					     sizeof(krb5_principal)))) {
	return ENOMEM;
    }
    i = 1;
    if (retval = krb5_tgtname(client,
			      client, &rettree[0])) {
	krb5_xfree(rettree);
	return retval;
    }
    for (prevccp = ccp = client->data + client->length - 1;
	 ccp >= com_csep;
	 ccp--) {
	if (*ccp != realm_branch_char)
	    continue;
	ccp--;				/* back over separator */
	tmpcrealm.data = client->data;
	tmpcrealm.length = prevccp - client->data + 1;
	tmpsrealm.data = client->data;
	tmpsrealm.length = ccp - client->data + 1;

	if (retval = krb5_tgtname(&tmpsrealm, &tmpcrealm, &rettree[i])) {
	    while (i) {
		krb5_free_principal(rettree[i-1]);
		i--;
	    }
	    krb5_xfree(rettree);
	    return retval;
	}
	prevccp = ccp;
	i++;
    }
    if (nocommon) {
	tmpcrealm.data = client->data;
	tmpcrealm.length = com_csep - client->data;
	tmpsrealm.data = server->data;
	tmpsrealm.length = com_ssep - server->data;

	if (retval = krb5_tgtname(&tmpsrealm, &tmpcrealm, &rettree[i])) {
	    while (i) {
		krb5_free_principal(rettree[i-1]);
		i--;
	    }
	    krb5_xfree(rettree);
	    return retval;
	}
	i++;
    }

    for (prevscp = com_ssep - 1, scp = com_ssep + 1;
	 scp < server->data + server->length - 1;
	 scp++) {
	if (*scp != realm_branch_char)
	    continue;
	tmpcrealm.data = server->data;
	tmpcrealm.length = prevscp + 1 - server->data;
	tmpsrealm.data = server->data;
	tmpsrealm.length = scp - server->data;
	if (retval = krb5_tgtname(&tmpsrealm, &tmpcrealm, &rettree[i])) {
	    while (i) {
		krb5_free_principal(rettree[i-1]);
		i--;
	    }
	    krb5_xfree(rettree);
	    return retval;
	}
	prevscp = scp - 1;
	i++;
    }
    if (slen && com_ssep < server->data + server->length) {
	/* only necessary if building down tree from ancestor or client */
	/* however, we can get here if we have only one component
	   in the server realm name, hence we make sure we found a component
	   separator there... */
	tmpcrealm.data = server->data;
	tmpcrealm.length = prevscp + 1 - server->data;
	if (retval = krb5_tgtname(server, &tmpcrealm,
				  &rettree[i])) {
	    while (i) {
		krb5_free_principal(rettree[i-1]);
		i--;
	    }
	    krb5_xfree(rettree);
	    return retval;
	}
    }
    *tree = rettree;
    return 0;
}


#include <dce/dce_cf_const.h>

#define MAX_REALM_LN 500

krb5_error_code krb5_check_transited_list(trans, realm1, realm2)
krb5_data      *trans;
krb5_data      *realm1;
krb5_data      *realm2;
{
    char curr[MAX_REALM_LN+1];
    char next[MAX_REALM_LN+1];
    char *nextp;
    int i, j;
    int trans_length;
    krb5_error_code retval = 0;
    krb5_principal *tgs_list;

#ifdef KRB5_BETA1
    if (!trans || !trans->length)
#else
    if (!trans || !trans->data)
#endif
	return(0);

    trans_length = trans->data[trans->length-1] ?
		    trans->length : trans->length - 1;

    if (retval = krb5_walk_realm_tree(realm1, realm2, &tgs_list,
					PATH_COMPONENT_SEPARATOR)) {
	return(retval);
    }

    *curr = 0;
    memset(next, 0, MAX_REALM_LN + 1), nextp = next;

    for (i = 0; i <= trans_length; i++) {
	if (i < trans_length-1 && trans->data[i] == '\\') {
	    i++;
	    *nextp++ = trans->data[i];
	    continue;
	}
	if (i < trans_length && trans->data[i] != ',') {
	    *nextp++ = trans->data[i];
	    continue;
	}
	if (*next == 0) {
	    continue;
	}

	retval = KRB5KRB_AP_ERR_ILL_CR_TKT;
	strcat(curr, next);

	for (j = 1; tgs_list[j]; j++) {
	    if (strlen(curr) == krb5_princ_realm(tgs_list[j])->length &&
		!memcmp(curr, krb5_princ_realm(tgs_list[j])->data,
			strlen(curr))) {
		retval = 0;
		break; 
	    }
	}
	if (retval)
	    goto finish;

	if (i+1 < trans_length && trans->data[i+1] == ' ') {
	    i++;
	    *curr = 0;
	    memset(next, 0, MAX_REALM_LN + 1), nextp = next;
	    continue;
	}
	if (i+1 < trans_length && trans->data[i+1] == PATH_COMPONENT_SEPARATOR) {
	    memset(next, 0, MAX_REALM_LN + 1), nextp = next;
	    continue;
	}
    }

finish:
    krb5_free_realm_tree(tgs_list);
    return(retval);
}
