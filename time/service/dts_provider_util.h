/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: dts_provider_util.h,v $
 * Revision 1.1.4.2  1996/02/18  23:12:21  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:37:19  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:13:26  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:25:36  root]
 * 
 * Revision 1.1.2.3  1994/07/20  20:24:43  cbrooks
 * 	CR10945 fix Time Provider errors
 * 	[1994/07/20  18:26:04  cbrooks]
 * 
 * Revision 1.1.2.2  1994/06/09  19:28:01  annie
 * 	cr10871 - expand copyright
 * 	[1994/06/09  19:27:24  annie]
 * 
 * Revision 1.1.2.1  1994/05/18  19:49:17  tom
 * 	Use provider specific define so this can be built stand-alone.
 * 	[1994/05/16  16:06:03  tom]
 * 
 * 	Initial checkin.
 * 	[1994/05/12  22:41:59  tom]
 * 
 * $EndLog$
 */

/*
 * Declarations for utility routines
 */

#define NOCHDIR		0
#define CHDIR		1

#define NOCLOSEFD	0
#define CLOSEFD		1

extern int daemon(int do_chdir, int do_close);

extern void signal_catcher( pthread_addr_t );

/*
 * Directory where Time Provider will chdir() to,
 * prefixed by libdce provided dcelocal_path global.
 * (usually dcelocal_path="/opt/dcelocal")
 */
#define K_PROVIDER_HOME_DIR	"/var/adm/time"
