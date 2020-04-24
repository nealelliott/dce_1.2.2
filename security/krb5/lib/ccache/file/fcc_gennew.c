/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: fcc_gennew.c,v $
 * Revision 1.1.9.1  1996/06/04  22:01:25  arvind
 * 	Merge krb5 work to DCE1.2.2 mainline
 * 	[1996/05/29  20:30 UTC  mullan_s  /main/mullan_dce1.2.2_krb5_drop/1]
 *
 * 	Kerb5Beta5 KRB5_FCC_FVNO_3 merge.
 * 	[1996/01/03  19:13 UTC  psn  /main/DCE_1.2/1]
 *
 * 	Binary Compatibility Merge
 * 	[1995/04/03  22:10 UTC  mullan_s  /main/HPDCE02/1]
 *
 * 	Call sec_login_krb_get_ccache_version to determine format
 * 	version of cache files.
 * 	[1995/01/24  15:14 UTC  mullan_s  /main/mullan_mothra_bin_compat/mullan_mothra_bin_compat2/1]
 *
 * Revision 1.1.7.2  1996/02/18  00:09:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:00:51  marty]
 * 
 * Revision 1.1.7.1  1995/12/08  17:46:42  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/03  22:10 UTC  mullan_s
 * 	Binary Compatibility Merge
 * 
 * 	HP revision /main/mullan_mothra_bin_compat/mullan_mothra_bin_compat2/1  1995/01/24  15:14 UTC  mullan_s
 * 	Call sec_login_krb_get_ccache_version to determine format
 * 	version of cache files.
 * 	[1995/12/08  16:57:54  root]
 * 
 * Revision 1.1.4.1  1994/08/26  16:01:32  sommerfeld
 * 	Krb5beta4 merge.
 * 	Add ODE header & copyright
 * 	[1994/08/26  15:52:49  sommerfeld]
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
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/ccache/file/fcc_gennew.c,v $
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
 * This file contains the source code for krb5_fcc_generate_new.
 */

#if !defined(lint) && !defined(SABER)
static char rcsid_fcc_resolve_c[] =
"$Id: fcc_gennew.c,v 1.1.9.1 1996/06/04 22:01:25 arvind Exp $";
#endif /* !lint && !SABER */

#ifdef OSF_DCE

/*
 * The DCE environment runs in a multi-threaded POSIX world.  We
 * must be sure to pick up pthread.h to work with mutexes.
 */

#   include <pthread.h>

#include <sec_login_pvt.h>

#endif

#include <errno.h>
#include <krb5/osconf.h>
#include "fcc.h"

#ifdef KRB5_USE_INET
#include <netinet/in.h>
#else
 #error find some way to use net-byte-order file version numbers.
#endif

extern krb5_cc_ops krb5_fcc_ops;

/*
 * Effects:
 * Creates a new file cred cache whose name is guaranteed to be
 * unique.  The name begins with the string TKT_ROOT (from fcc.h).
 * The cache is not opened, but the new filename is reserved.
 *  
 * Returns:
 * The filled in krb5_ccache id.
 *
 * Errors:
 * KRB5_CC_NOMEM - there was insufficient memory to allocate the
 * 		krb5_ccache.  id is undefined.
 * system errors (from open)
 */
krb5_error_code
krb5_fcc_generate_new (id)
   krb5_ccache *id;
{
     krb5_ccache lid;
     int ret;
     krb5_error_code    retcode = 0;
     krb5_fcc_data      *data = NULL;
#ifdef OSF_DCE
     pthread_mutex_t    *mutex = NULL;
#endif
     char scratch[sizeof(TKT_ROOT)+6+1]; /* +6 for the scratch part, +1 for
					    NUL */
     
     /* Allocate memory */
     lid = (krb5_ccache) malloc(sizeof(struct _krb5_ccache));
     if (lid == NULL)
	  return KRB5_CC_NOMEM;

     lid->ops = &krb5_fcc_ops;

     (void) strcpy(scratch, TKT_ROOT);
     (void) strcat(scratch, "XXXXXX");
     mktemp(scratch);

     lid->data = data = (krb5_pointer) malloc(sizeof(krb5_fcc_data));
     if (lid->data == NULL) {
	  krb5_xfree(lid);
	  return KRB5_CC_NOMEM;
     }

#ifdef OSF_DCE
     /*
      * The mutex doesn't exist until later...
      */
     data->mutex = NULL;
#endif

     /*
      * The file is initially closed at the end of this call...
      */
     data->fd = -1;

     data->filename = (char *)
	  malloc(strlen(scratch) + 1);
     if (data->filename == NULL) {
	  krb5_xfree(data);
	  krb5_xfree(lid);
	  return KRB5_CC_NOMEM;
     }

     data->flags = 0;
     
     /* Set up the filename */
     strcpy(data->filename, scratch);

     /* Make sure the file name is reserved */
     ret = open(data->filename,	O_CREAT | O_EXCL | O_WRONLY, 0);
     if (ret == -1) {
	  retcode = krb5_fcc_interpret(errno);
          goto err_out;
     } else {
	  krb5_int16 fcc_fvno = htons(KRB5_FCC_DEFAULT_FVNO);
	  int errsave, cnt;

#ifdef OSF_DCE
         int version;
         /*
          * Retrieve krb5 file format vno from dce_cf.db
          */
         sec_login_pvt_get_krb5_ccache_vno(&version);
         fcc_fvno = htons(version);
#endif /* OSF_DCE */

	  /* Ignore user's umask, set mode = 0600 */
#ifdef NOFCHMOD
	  chmod(data->filename, S_IRUSR | S_IWUSR);
#else
	  fchmod(ret, S_IRUSR | S_IWUSR);
#endif
	  if ((cnt = write(ret, (char *)&fcc_fvno, sizeof(fcc_fvno)))
	      != sizeof(fcc_fvno)) {
	      errsave = errno;
	      (void) close(ret);
	      (void) unlink(data->filename);
	      retcode = (cnt == -1) ? krb5_fcc_interpret(errsave) : KRB5_CC_IO;
              goto err_out;
	  }
	  if (close(ret) == -1) {
	      errsave = errno;
	      (void) unlink(data->filename);
	      retcode = krb5_fcc_interpret(errsave);
              goto err_out;
	  }

#ifdef OSF_DCE          
          data->mutex = (void *) malloc(sizeof(pthread_mutex_t));
          if (data->mutex == NULL) {
            goto err_out;
          }
          mutex = (pthread_mutex_t *) data->mutex;
          if (pthread_mutex_init(mutex, pthread_mutexattr_default) == -1) {
	      retcode = krb5_fcc_interpret(errno);
              goto err_out;
          }
#endif

	  *id = lid;
	  return KRB5_OK;
     }

err_out:
#ifdef OSF_DCE
     if (data->mutex) {
        krb5_xfree(data->mutex);
     }
#endif
     krb5_xfree(data->filename);
     krb5_xfree(data);
     krb5_xfree(lid);
     return retcode;
}
