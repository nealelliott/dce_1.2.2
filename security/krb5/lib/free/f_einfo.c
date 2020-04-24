/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: f_einfo.c,v $
 * Revision 1.1.2.1  1996/10/03  15:01:51  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  13:45:11  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/26  23:31 UTC  sommerfeld
 * 	Imported from MIT krb5 beta 6
 * 
 * $EndLog$
 */
/*
 * lib/krb5/free/f_einfo.c
 *
 * Copyright 1995 by the Massachusetts Institute of Technology.
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
 * krb5_free_etype_info()
 */

#ifdef OSF_DCE
#define OSF_DCE_ARCHAISM
#endif

#ifdef OSF_DCE_ARCHAISM
#include <krb5/krb5.h>
#include <krb5/func-proto.h>
#else
#include "k5-int.h"
#endif

void krb5_free_etype_info(context, info)
    krb5_context context;
    krb5_etype_info info;
{
  int i;

  for(i=0; info[i] != NULL; i++) {
      if (info[i]->salt)
	  free(info[i]->salt);
      free(info[i]);
  }
  free(info);
}
    
