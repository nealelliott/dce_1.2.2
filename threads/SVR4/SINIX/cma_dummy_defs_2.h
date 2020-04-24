/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA,USA
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: cma_dummy_defs_2.h,v $
 * Revision 1.1.35.2  1996/02/18  23:08:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:32:29  marty]
 *
 * Revision 1.1.35.1  1995/12/07  21:53:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:02:15  root]
 * 
 * Revision 1.1.33.1  1994/06/10  20:52:37  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:49:37  devsrc]
 * 
 * Revision 1.1.31.1  1993/10/05  21:38:41  ohara
 * 	SNI SVR4 ref port 8596
 * 	[1993/09/15  22:15:52  ohara]
 * 
 * Revision 1.1.2.4  93/07/06  17:23:10  maher
 * 	added __dummy_ioctl (see OT$ 621)
 * 
 * Revision 1.1.5.2  93/07/06  17:14:34  maher
 * 	added __dummy_ioctl (see OT$ 621)
 * 
 * Revision 1.1.2.3  93/02/09  09:01:38  hinman
 * 	Acceptance of OSF rev 1.1.2.2
 * 
 * Revision 1.2.1.2  93/02/04  10:04:25  root
 * 	Auto checkin of OSF rev 1.1.2.2
 * 
 * Revision 1.1.2.2  1993/02/01  19:27:18  hinman
 * 	[hinman] - Check in SNI revision 1.1.2.2
 * 	[1993/01/08  13:40:59  hinman]
 * 
 * $EndLog$
 */
#ifndef R_DUMMY_DEFS_2
#define R_DUMMY_DEFS_2

#define close __dummy_close

#define open  __dummy_open

#define read  __dummy_read

#define readv __dummy_readv

#define write __dummy_write

#define writev __dummy_writev

#define getmsg __dummy_getmsg

#define getpmsg __dummy_getpmsg

#define putmsg __dummy_putmsg

#define putpmsg __dummy_putpmsg

#define ioctl __dummy_ioctl



#endif /* R_DUMMYS_DEFS2*/

