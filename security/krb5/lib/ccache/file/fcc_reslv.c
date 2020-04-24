/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: fcc_reslv.c,v $
 * Revision 1.1.6.2  1996/02/18  00:09:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:01:12  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:47:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:58:00  root]
 * 
 * Revision 1.1.4.1  1994/08/26  16:01:43  sommerfeld
 * 	Krb5beta4 merge.
 * 	Add ODE header & copyright
 * 	[1994/08/26  15:54:05  sommerfeld]
 * 
 * $EndLog$
 */
/*
**
** Copyright (c) Hewlett-Packard Company 1991, 1992
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/ccache/file/fcc_reslv.c,v $
 * $Author: marty $
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
 * This file contains the source code for krb5_fcc_resolve.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_fcc_reslve_c[] =
"$Id: fcc_reslv.c,v 1.1.6.2 1996/02/18 00:09:37 marty Exp $";
#endif /* !lint && !SABER */

#ifdef OSF_DCE

/*
 * The DCE environment runs in a multi-threaded POSIX world.  We
 * must be sure to pick up pthread.h to work with mutexes.
 */

#   include <pthread.h>

#endif

#include "fcc.h"

extern krb5_cc_ops krb5_fcc_ops;

/*
 * Requires:
 * residual is a legal path name, and a null-terminated string
 *
 * Modifies:
 * id
 * 
 * Effects:
 * creates a file-based cred cache that will reside in the file
 * residual.  The cache is not opened, but the filename is reserved.
 * 
 * Returns:
 * A filled in krb5_ccache structure "id".
 *
 * Errors:
 * KRB5_CC_NOMEM - there was insufficient memory to allocate the
 * 		krb5_ccache.  id is undefined.
 * permission errors
 */
krb5_error_code
krb5_fcc_resolve (id, residual)
   krb5_ccache *id;
   char *residual;
{
     krb5_ccache lid;
     krb5_error_code    retval = KRB5_CC_NOMEM;
     krb5_fcc_data      *data = NULL;
#ifdef OSF_DCE
     pthread_mutex_t    *mutex = NULL;
#endif
     
     lid = (krb5_ccache) malloc(sizeof(struct _krb5_ccache));
     if (lid == NULL)
	  goto err_out;

     lid->ops = &krb5_fcc_ops;
     
     lid->data = (krb5_pointer) malloc(sizeof(krb5_fcc_data));
     if (lid->data == NULL) {
          goto err_out;
     }

     data = (krb5_fcc_data *) lid->data;

     data->filename = (char *) malloc(strlen(residual) + 1);

     if (data->filename == NULL) {
          goto err_out;
     }

#ifdef OSF_DCE          
     /*
      * Initialize the mutex.
      */
     data->mutex = (void *) malloc(sizeof(pthread_mutex_t));
     if (data->mutex == NULL) {
            goto err_out;
     }
     mutex = (pthread_mutex_t *) data->mutex;
     if (pthread_mutex_init(mutex, pthread_mutexattr_default) == -1) {
        retval = krb5_fcc_interpret(errno);
        goto err_out;
     }
#endif

     /* default to open/close on every trn */
     data->flags = KRB5_TC_OPENCLOSE;
     data->fd = -1;
     
     /* Set up the filename */
     strcpy(data->filename, residual);

     /* other routines will get errors on open, and callers must expect them,
	if cache is non-existent/unusable */
     *id = lid;
     return KRB5_OK;

err_out:
     if (lid) {
        if (data) {
            if (data->filename) {
                krb5_xfree(data->filename);
            }
#ifdef OSF_DCE
            if (data->mutex) {
                krb5_xfree(data->mutex);
            }
#endif
            krb5_xfree(data);
        }
        krb5_xfree(lid);
     }

     return retval;
}
