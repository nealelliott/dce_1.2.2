/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/* Copyright (C) 1996, 1995 Transarc Corporation - All rights reserved */
/*
 * HISTORY
 * $Log: dfsgw.h,v $
 * Revision 1.1.6.1  1996/10/02  17:50:11  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:26  damon]
 *
 * Revision /main/HPDCE02/2  1995/06/22  20:14 UTC  maunsell_c
 * 	mothra C&S fixes
 * 
 * Revision /main/HPDCE02/1  1995/02/14  22:42 UTC  maunsell_c
 * 	@sys @host changes for nfs/dfs gateway
 * 
 * Revision /main/maunsell_mothra1.45/2  1995/02/01  17:00 UTC  maunsell_c
 * 	fix lineup of VERSION define
 * 
 * Revision /main/maunsell_mothra1.45/1  1995/01/31  19:21 UTC  maunsell_c
 * 	add @sys @host support for dfs_login/dfsgwd/dfsgw
 * 
 * Revision 1.1.2.2  1994/08/08  18:54:38  mckeen
 * 	Set official port number
 * 	defined server name
 * 	[1994/08/08  17:47:06  mckeen]
 * 
 * Revision 1.1.2.1  1994/07/26  19:13:20  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:57:56  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:05:38  mckeen]
 * 
 * $EndLog$
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/gateway/dfsgwd/dfsgw.h,v 1.1.6.1 1996/10/02 17:50:11 damon Exp $ */
/*
 * dfsgw.h - Common definitions for the dfs remote authentication service.
 */

#ifndef _DFSGW_H
#define _DFSGW_H

/*
 * Defines.
 */
#define DFSGW_SERVICE		"dfsgw"
#define DFSGW_SERVICE_DEFAULT	438
#define DFSGW_SERVER            "dfsgw-server"

#define DFSGW_MAXPSIZE		(10240)		/* Max auth packet size */
#define DFSGW_MAXRSIZE		(1024)		/* Max return packet size */


/*
 * Format of header used in concatenating auth and priv messages.
 * This was originally defined as krb5_data.  However that structure changed
 * from one version of Kerberos to the next.  So we are now using our own
 * structure, which will change only at our discretion.
 */
typedef struct _dfsgw_data {
    int length;
    char *data;
} dfsgw_data;


/*
 * Messages sent between client and server have the following format:
 *
 *	unsigned char protocol_version
 *	unsigned char message_type
 *		  (encoded in the lsb of message type is the host byte order)
 *	roundup to <dfsgw_factor> boundary
 *	<message-specific data>
 */
#define dfsgw_factor		(sizeof(unsigned long) - 1) 
#define dfsgw_roundup(x)	(((int)(x) + dfsgw_factor) & (~dfsgw_factor))
#define DFSGW_PROTOCOL_VERSION   (2)		/* Protocol version 2 - @sys @host */
#define DFSGW_PROTOCOL_VERSION_1 (1)		/* Protocol version 1 */
#ifdef BAD_PROTOCOL_TEST
#define DFSGW_BAD_PROTOCOL_VERSION (0)          /* Bad Protocol version, build w -DBAD_PROTOCOL_TEST */
#endif
#define DFSGW_AUTH_PACKET	(0x1 << 1)	/* Authentication packet */
#define DFSGW_RAUTH_PACKET	(0x2 << 1)	/* Revoke authentication packet */
#define DFSGW_REPLY_PACKET	(0x3 << 1)	/* Reply packet */
#define DFSGW_REPLY_OK		(0)		/* Reply codes */
#define DFSGW_REPLY_ERROR	(1)	
#define DFSGW_REPLY_ERROR_BAD_VERSION (2)      
#define DFSGW_LITTLE_ENDIAN	(0)		/* Byte order types */
#define DFSGW_BIG_ENDIAN		(1)		
#define dfsgw_msg(x)		(x & ~0x1)
#define dfsgw_endian(x)		(x & 0x1)
#define dfsgw_sswap(x)		(((x >> 8) | (x << 8)) & 0xffff)
#define dfsgw_wswap(x)		((dfsgw_sswap(x & 0xffff) << 16) | dfsgw_sswap(x >> 16)) 
#endif /* _DFSGW_H */
