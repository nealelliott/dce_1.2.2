/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: std.h,v $
 * Revision 1.1.10.2  1996/02/18  23:34:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:18  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:22:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:17  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:35:36  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:45:13  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:11:28  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:30:28  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:11:56  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:08:55  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:03:40  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:49:53  melman]
 * 
 * $EndLog$
 */
#ifndef _STD_H
#define _STD_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char std_rcsid[] = "@(#)$RCSfile: std.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:34:50 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/**********************************************************************/
/*                                                                    */
/*       D E K L A R A T I O N E N     Daten                          */
/*                                                                    */
/**********************************************************************/

/*****  Local  *****/

#define Boolean int

typedef char * string;

typedef struct {
	int		nb;
	unsigned char   * contents;
} octet_string;

#define UC unsigned char

#ifndef TRUE
#define TRUE 1
#endif

#ifndef FALSE
#define FALSE 0
#endif

#ifndef NULL
#define NULL 0
#endif

#endif /* _STD_H */
