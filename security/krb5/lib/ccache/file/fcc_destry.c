/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: fcc_destry.c,v $
 * Revision 1.1.7.2  1996/02/18  00:09:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:00:42  marty]
 *
 * Revision 1.1.7.1  1995/12/08  17:46:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:57:51  root]
 * 
 * Revision 1.1.5.1  1994/08/26  16:01:28  sommerfeld
 * 	Krb5beta4 merge.
 * 	Add ODE header & copyright
 * 	[1994/08/26  15:52:13  sommerfeld]
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
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/ccache/file/fcc_destry.c,v $
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
 * This file contains the source code for krb5_fcc_destroy.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_fcc_destry_c[] =
"$Id: fcc_destry.c,v 1.1.7.2 1996/02/18 00:09:16 marty Exp $";
#endif /* !lint && !SABER */

#ifdef OSF_DCE

/*
 * The DCE environment runs in a multi-threaded POSIX world.  We
 * must be sure to pick up pthread.h to work with mutexes.
 */

#   include <pthread.h>

#endif

#include <errno.h>
#include "fcc.h"

/*
 * Effects:
 * Destroys the contents of id.
 *
 * Errors:
 * system errors
 */
krb5_error_code krb5_fcc_destroy(id)
   krb5_ccache id;
{
     struct stat buf;
     unsigned long size;
     char zeros[BUFSIZ];
     register int ret, i;
     krb5_fcc_data      *data = (krb5_fcc_data *) id->data;
#ifdef OSF_DCE
     pthread_mutex_t    *mutex;
#endif
     
     if (OPENCLOSE(id)) {
	  ret = open(data->filename, O_RDWR, 0);
	  if (ret < 0)
	       return krb5_fcc_interpret(errno);
	  data->fd = ret;
     }
     else
	  lseek(data->fd, 0, SEEK_SET);

     ret = unlink(data->filename);
     if (ret < 0) {
	 ret = krb5_fcc_interpret(errno);
	 if (OPENCLOSE(id)) {
	     (void) close(((krb5_fcc_data *)id->data)->fd);
	     data->fd = -1;
	 }
	 return ret;
     }
     
     ret = fstat(data->fd, &buf);
     if (ret < 0) {
	 ret = krb5_fcc_interpret(errno);
	 if (OPENCLOSE(id)) {
	     (void) close(((krb5_fcc_data *)id->data)->fd);
	     data->fd = -1;
	 }
	 return ret;
     }

     /* XXX This may not be legal XXX */
     size = (unsigned long) buf.st_size;

     memset(zeros, 0, BUFSIZ);
     for (i=0; i < size / BUFSIZ; i++)
	  if (write(data->fd, zeros, BUFSIZ) < 0) {
	      ret = krb5_fcc_interpret(errno);
	      if (OPENCLOSE(id)) {
		  (void) close(((krb5_fcc_data *)id->data)->fd);
		  data->fd = -1;
	      }
	      return ret;
	  }

     if (write(data->fd, zeros, size % BUFSIZ) < 0) {
	 ret = krb5_fcc_interpret(errno);
	 if (OPENCLOSE(id)) {
	     (void) close(((krb5_fcc_data *)id->data)->fd);
	     data->fd = -1;
	 }
	 return ret;
     }

     /*
      * Normal termination.  Close the file here and make sure we unlock
      * and destroy the mutex.
      */
#ifdef OSF_DCE
     mutex = (pthread_mutex_t *)data->mutex;
     if (pthread_mutex_destroy(mutex) == -1) {
        if (!ret) {
            ret = krb5_fcc_interpret(errno);
        }
     }
#endif
     ret = close(data->fd);
     data->fd = -1;

     if (ret) 
     {
	 ret = krb5_fcc_interpret(errno);
     } else if (data) {
        /*
         * Release storage used by this credential cache
         */
        if (data->filename) {
            krb5_xfree(data->filename);
        }
#ifdef OSF_DCE
        if (data->mutex) {
            krb5_xfree(data->mutex);
        }
#endif

        krb5_xfree(data);
        krb5_xfree(id);
     }

     return ret;
}
