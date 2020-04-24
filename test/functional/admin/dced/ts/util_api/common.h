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
 * $Log: common.h,v $
 * Revision 1.1.7.3  1996/02/18  23:05:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:27  marty]
 *
 * Revision 1.1.7.2  1995/12/11  15:20:03  root
 * 	Submit
 * 	[1995/12/11  14:33:36  root]
 * 
 * Revision 1.1.2.2  1994/10/27  20:07:14  rrizzo
 * 	expand copyright
 * 	[1994/10/27  19:39:30  rrizzo]
 * 
 * Revision 1.1.2.1  1994/10/18  20:55:43  max
 * 	Initial submission of FVTs for dced utility API
 * 	[1994/10/18  20:47:55  max]
 * 
 * $EndLog$
 */



int common_setup( void);
int common_cleanup( void);
int common_fifo_to_tet_info ( int fd );
int common_server_func ( char * func);

