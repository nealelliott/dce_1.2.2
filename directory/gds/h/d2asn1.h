/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d2asn1.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:15  marty]
 *
 * Revision 1.1.4.1  1995/12/08  16:01:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:04  root]
 * 
 * Revision 1.1.2.3  1994/06/21  14:47:15  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:41:27  marrek]
 * 
 * Revision 1.1.2.2  1994/06/09  18:46:19  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:19:23  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:21:16  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:11:23  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:52:36  marrek]
 * 
 * $EndLog$
 */
#ifndef _D2ASN1_H
#define _D2ASN1_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d2asn1_rcsid[] = "@(#)$RCSfile: d2asn1.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:58 $";
#endif

/* includes */

#include <dce/dce.h>
#include <dce/asn1.h>

#define  D2A_APDU_MEM           4096   /* size of apdu to be allocated */
#define  D2A_ERR_APDU_SIZE      20     /* size of error apdu */

#define D2A_MORE_MEM    16384       /* memory that is additionally    */
				    /* allocated in case of           */
				    /* reallocation                   */

/***********************/
/* function prototypes */
/***********************/

/* from d24asn1.c */

signed32 d2a047_asn_error_apdu (byte **,signed32 *,signed32,signed16,asn1_field *,byte *);

#ifndef  DSA /* in d24asn1.c */

signed32 d2a000_apdu_asn1 (signed16,byte *,signed32 *,asn1_field *,byte *);
signed32 d2a001_asn1_apdu (signed16,byte **,signed32 *,asn1_field *,byte *);

#endif

/* from d25asn1.c */

#ifdef DSA /* set for d25asn1.c and d24asn1n.c */

signed32 d2a000_apdu_asn1 (signed16,signed16,byte *,signed32,asn1_field *,byte *);
signed32 d2a001_asn1_apdu (signed16,signed16,byte **,signed32 *,asn1_field *,byte *);
signed32 d2a027_apdu_error_asn (byte *,signed32,signed32 *,asn1_field *,signed16);
signed32 d2a026_apdu_bind_error_asn (byte *,signed32,asn1_field *);

#endif /* DSA */

#endif /* _D2ASN1_H */
