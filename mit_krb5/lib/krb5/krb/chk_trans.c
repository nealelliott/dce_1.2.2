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
 * $Log: chk_trans.c,v $
 * Revision 1.1.2.1  1996/06/05  20:57:26  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:55:15  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * Copyright (c) 1994 CyberSAFE Corporation.
 * All rights reserved.
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
 * permission.  Neither M.I.T., the Open Computing Security Group, nor
 * CyberSAFE Corporation make any representations about the suitability of
 * this software for any purpose.  It is provided "as is" without express
 * or implied warranty.
 */

#include <stdio.h>
#include "k5-int.h"

#define MAX_REALM_LN 500

krb5_error_code
krb5_check_transited_list(context, trans, realm1, realm2)
    krb5_context context;
krb5_data      *trans;
krb5_data      *realm1;
krb5_data      *realm2;
{
  char            prev[MAX_REALM_LN+1];
  char            next[MAX_REALM_LN+1];
  char            *nextp;
  int             i, j;
  int             trans_length;
  krb5_error_code retval = 0;
  krb5_principal  *tgs_list;

  if (!trans || !trans->data)  return(0);
  trans_length = trans->data[trans->length-1] ?
                 trans->length : trans->length - 1;

  if (retval = krb5_walk_realm_tree(context, realm1, realm2, &tgs_list,
                                    KRB5_REALM_BRANCH_CHAR)) {
    return(retval);
  }

  memset(prev, 0, MAX_REALM_LN + 1);
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
    if (strlen(next) > 0) {
      if (next[0] != '/') {
        if (*(nextp-1) == '.')  strcat(next, prev);
        retval = KRB5KRB_AP_ERR_ILL_CR_TKT;
        for (j = 0; tgs_list[j]; j++) {
          if (strlen(next) == (size_t) krb5_princ_realm(context, tgs_list[j])->length &&
              !memcmp(next, krb5_princ_realm(context, tgs_list[j])->data,
                      strlen(next))) {
            retval = 0;
            break; 
          }
        }
        if (retval)  goto finish;
      }
      if (i+1 < trans_length && trans->data[i+1] == ' ') {
        i++;
        memset(next, 0, MAX_REALM_LN + 1), nextp = next;
        continue;
      }
      if (i+1 < trans_length && trans->data[i+1] != '/') {
        strcpy(prev, next);
        memset(next, 0, MAX_REALM_LN + 1), nextp = next;
        continue;
      }
    }
  }

finish:
  krb5_free_realm_tree(context, tgs_list);
  return(retval);
}
