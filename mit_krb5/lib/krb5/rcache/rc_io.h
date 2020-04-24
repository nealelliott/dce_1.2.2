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
 * $Log: rc_io.h,v $
 * Revision 1.1.2.1  1996/06/05  21:22:00  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  20:12:21  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/
/*
 * lib/krb5/rcache/rc_io.h
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

typedef struct krb5_rc_iostuff
 {
  int fd;
#ifdef MSDOS_FILESYSTEM
  long mark;
#else
  int mark; /* on newer systems, should be pos_t */
#endif
  char *fn;
 }
krb5_rc_iostuff;

/* first argument is always iostuff for result file */

krb5_error_code krb5_rc_io_creat 
	PROTOTYPE((krb5_context,
		   krb5_rc_iostuff *,
		   char **));
krb5_error_code krb5_rc_io_open 
	PROTOTYPE((krb5_context,
		   krb5_rc_iostuff *,
		   char *));
krb5_error_code krb5_rc_io_move 
	PROTOTYPE((krb5_context,
		   krb5_rc_iostuff *,
		   krb5_rc_iostuff *));
krb5_error_code krb5_rc_io_write 
	PROTOTYPE((krb5_context,
		   krb5_rc_iostuff *,
		   krb5_pointer,
		   int));
krb5_error_code krb5_rc_io_read 
	PROTOTYPE((krb5_context,
		   krb5_rc_iostuff *,
		   krb5_pointer,
		   int));
krb5_error_code krb5_rc_io_close 
	PROTOTYPE((krb5_context,
		   krb5_rc_iostuff *));
krb5_error_code krb5_rc_io_destroy 
	PROTOTYPE((krb5_context,
		   krb5_rc_iostuff *));
krb5_error_code krb5_rc_io_mark 
	PROTOTYPE((krb5_context,
		   krb5_rc_iostuff *));
krb5_error_code krb5_rc_io_unmark 
	PROTOTYPE((krb5_context,
		   krb5_rc_iostuff *));
krb5_error_code krb5_rc_io_sync
	PROTOTYPE((krb5_context,
		   krb5_rc_iostuff *));
long krb5_rc_io_size
	PROTOTYPE((krb5_context,
		   krb5_rc_iostuff *));
#endif
