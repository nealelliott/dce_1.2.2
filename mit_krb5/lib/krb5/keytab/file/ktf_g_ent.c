/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: ktf_g_ent.c,v $
 * Revision 1.1.2.1  1996/06/05  20:53:00  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:51:52  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: ktf_g_ent.c,v $
 * Revision 1.1.2.1  1996/06/05  20:53:00  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:51:52  arvind]
 *
 * Revision 1.1.1.2  1996/06/05  19:51:52  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 *
 * Revision /main/DCE_1.2.2/2  1996/05/30  21:19 UTC  mullan_s
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 
 * Revision /main/DCE_1.2.2/mullan_dce1.2.2_krb5_drop/1  1996/05/29  20:44 UTC  mullan_s
 * 	Merge krb5 work from DCE_1.2 to DCE_1.2.2
 * 
 * Revision /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/2  1996/05/17  14:18 UTC  mullan_s
 * 	Remove #ifdef OSF_DCE, just a bugfix not an enhancement.
 * 
 * Revision /main/DCE_1.2/mullan_dce_k5_bsd44_rutils/1  1996/05/03  20:07 UTC  mullan_s
 * 	Add OSF/HP copyrights & RCS header.
 * 
 * $EndLog$
 */
/*
 * lib/krb5/keytab/file/ktf_get_en.c
 *
 * Copyright 1990 by the Massachusetts Institute of Technology.
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
 * This is the get_entry routine for the file based keytab implementation.
 * It opens the keytab file, and either retrieves the entry or returns
 * an error.
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/

#include "k5-int.h"
#include "ktfile.h"

krb5_error_code
krb5_ktfile_get_entry(context, id, principal, kvno, keytype, entry)
   krb5_context context;
   krb5_keytab id;
   krb5_principal principal;
   krb5_kvno kvno;
   krb5_keytype keytype;
   krb5_keytab_entry * entry;
{
    krb5_keytab_entry cur_entry, new_entry;
    krb5_error_code kerror = 0;

    /* Open the keyfile for reading */
    if (kerror = krb5_ktfileint_openr(context, id))
	return(kerror);
    
    /* 
     * For efficiency and simplicity, we'll use a while true that 
     * is exited with a break statement.
     */
    cur_entry.principal = 0;
    cur_entry.vno = 0;
    cur_entry.key.contents = 0;
    while (TRUE) {
	if (kerror = krb5_ktfileint_read_entry(context, id, &new_entry))
	    break;

	if (krb5_principal_compare(context, principal, new_entry.principal)) {
		if (kvno == IGNORE_VNO) {
			if (cur_entry.vno < new_entry.vno) {
				krb5_kt_free_entry(context, &cur_entry);
				cur_entry = new_entry;
			}
		} else {
			if (new_entry.vno == kvno) {
			    krb5_kt_free_entry(context, &cur_entry);

			    cur_entry = new_entry;
			    break;
			}
		}
	} else {
		krb5_kt_free_entry(context, &new_entry);
	}
    }
    if (kerror == KRB5_KT_END)
	    kerror = cur_entry.principal ? 0 : KRB5_KT_NOTFOUND;
    if (kerror) {
	(void) krb5_ktfileint_close(context, id);
	krb5_kt_free_entry(context, &cur_entry);
	return kerror;
    }
    if ((kerror = krb5_ktfileint_close(context, id)) != 0) {
	krb5_kt_free_entry(context, &cur_entry);
	return kerror;
    }
    *entry = cur_entry;
    return 0;
}
