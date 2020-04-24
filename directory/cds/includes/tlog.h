/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: tlog.h,v $
 * Revision 1.1.6.2  1996/02/18  23:34:30  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:40  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:18:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:57  root]
 * 
 * Revision 1.1.4.3  1994/08/03  19:02:49  mccann
 * 	include file cleanup
 * 	[1994/07/19  18:01:22  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:39:40  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:21  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:04:38  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:20:38  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:51:11  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:55:16  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:16:10  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _TLOG_H
#define _TLOG_H
/*
 * Module: tlog.h
 * Version: X02.10
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 */


#define TLOG_VERSION_MAJOR	2
#define	TLOG_VERSION_MINOR	10

#include <dbdef.h>

typedef struct tlog_dsc
	{
	int		cnt;	/* Number of updates */
	int		len;	/* Size of buffer used in buf_p */
	byte_t		*buf_p;	/* Buffer of updates */
	} tlog_dsc_t;

/* This buffer needs to be big enough to hold all data from one log request.
 * Since the maximum input comes from do_updates, the buffer needs to be
 * big enough to hold 16k bytes plus the header.
 */

#define TLOG_BUF_BLOCKS (32 + 1)

/* Header on each transaction */
typedef struct tlog_hdr 
	{
	unsigned long	t_checksum; 	/* Checksum of data following hdr*/
	unsigned long	t_len;		/* Length in bytes of log entry */
	unsigned long	t_seq;		/* File header sequence number 
					 * at time of log */
	int		t_type;	   	/* tlog_create or tlog_update */
	int		t_operation;	/* transaction code generating log */	
	int		t_rec;		/* Record type */
	int		t_cnt;		/* Block count of log entry */
	ObjUID_u	t_diruid;	/* Directory uid */
	unsigned short	t_align_mask;	/* Alignment mask */
	} tlog_hdr_t;

#define TLOG_CREATE 1	/* Log contains entire new record */
#define TLOG_UPDATE 2	/* The log contains an update */

#endif  /* #ifndef _TLOG_H */
