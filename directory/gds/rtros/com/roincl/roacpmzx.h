/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: roacpmzx.h,v $
 * Revision 1.1.4.2  1996/02/18  23:36:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:27:21  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:10:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:18:39  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:04:26  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:48:14  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:58:25  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:48:33  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:31:31  keutel
 * 	creation
 * 	[1994/03/21  13:18:51  keutel]
 * 
 * $EndLog$
 */
#ifndef _ROACPMZX_H
#define _ROACPMZX_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char _roacpmzx_rcsid[] = { "@(#) $RCSfile: roacpmzx.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:36:38 $" } ;
#endif

/*
roacpmzz.h,
Thu Jan 23 16:54:08 1992
*/

/* This header file contains only the function proto types for ACSE En/De    */
/* functions. Since the roacpmzz.h contains the local info also.             */


/* CODING AND DECODING PROCEDURES */


int PASCAL ACSE_apdu_dec PROTO ((STOREHANDLE,unsigned long,rT0646_acseApdus FAR *,asn1_pvoid));
int PASCAL ACSE_apdu_cod PROTO ((STOREHANDLE,int,long,rT0646_acseApdus FAR *,asn1_pvoid));

 /*
int PASCAL ACSE_apdu_dec PROTO((STOREHANDLE,unsigned long,int FAR *,asn1_pvoid));
int PASCAL ACSE_apdu_cod PROTO((STOREHANDLE,int,long,int FAR *,asn1_pvoid));
*/

#endif	/* _ROACPMZX_H */
