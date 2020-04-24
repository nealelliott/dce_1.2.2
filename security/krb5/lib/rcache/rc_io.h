/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rc_io.h,v $
 * Revision 1.1.4.2  1996/02/18  23:03:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:09  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:44:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:18:52  root]
 * 
 * Revision 1.1.2.3  1993/03/03  23:32:47  sommerfeld
 * 	[7403] Add prototype for krb5_rc_io_size
 * 	[1993/03/03  20:31:08  sommerfeld]
 * 
 * Revision 1.1.2.2  1992/12/29  16:32:18  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:19:59  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:52:31  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  rc_io.h V=3 10/24/91 //littl/prgy/krb5/lib/rcache
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * $Source: /u0/rcs_trees/dce/rcs/security/krb5/lib/rcache/rc_io.h,v $
 * $Author: marty $
 * $Id: rc_io.h,v 1.1.4.2 1996/02/18 23:03:36 marty Exp $
 *
 * This file of the Kerberos V5 software is derived from public-domain code
 * contributed by Daniel J. Bernstein, <brnstnd@acf10.nyu.edu>.
 *
 */

/*
 * Declarations for the I/O sub-package of the replay cache
 */

#ifndef KRB5_RC_IO_H
#define KRB5_RC_IO_H
#include <krb5/osconf.h>

typedef struct krb5_rc_iostuff
 {
  int fd;
  int mark; /* on newer systems, should be pos_t */
  char *fn;
 }
krb5_rc_iostuff;

/* first argument is always iostuff for result file */

krb5_error_code krb5_rc_io_creat PROTOTYPE((krb5_rc_iostuff *,char **));
krb5_error_code krb5_rc_io_open PROTOTYPE((krb5_rc_iostuff *,char *));
krb5_error_code krb5_rc_io_move PROTOTYPE((krb5_rc_iostuff *,krb5_rc_iostuff *));
krb5_error_code krb5_rc_io_write PROTOTYPE((krb5_rc_iostuff *,krb5_pointer,int));
krb5_error_code krb5_rc_io_read PROTOTYPE((krb5_rc_iostuff *,krb5_pointer,int));
krb5_error_code krb5_rc_io_close PROTOTYPE((krb5_rc_iostuff *));
krb5_error_code krb5_rc_io_destroy PROTOTYPE((krb5_rc_iostuff *));
krb5_error_code krb5_rc_io_mark PROTOTYPE((krb5_rc_iostuff *));
krb5_error_code krb5_rc_io_unmark PROTOTYPE((krb5_rc_iostuff *));
int krb5_rc_io_size PROTOTYPE((krb5_rc_iostuff *));
    
#endif
