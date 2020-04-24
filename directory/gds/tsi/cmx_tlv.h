/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: cmx_tlv.h,v $
 * Revision 1.1.4.2  1996/03/11  13:24:11  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:37  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:47:19  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:25:07  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:16:32  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:30  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:15:12  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:50:35  marrek]
 * 
 * $EndLog$
 */
#ifndef _CMX_TLV_H
#define _CMX_TLV_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char cmx_tlv_rcsid[] = "@(#)$RCSfile: cmx_tlv.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:11 $";
#endif

/*
 * COPYRIGHT (C) Siemens Nixdorf Informationssysteme AG 1992
 *               All Rights Reserved
 *  
 * This software is furnished under a license and may be used
 * only in accordance with the terms of that license and with the
 * inclusion of the above copyright notice. This software may not
 * be provided or otherwise made available to, or used by, any
 * other person. No title to or ownership of the software is
 * hereby transferred.
 */

/* #ident "@(#)cmx_tlv.h	311.3 	93/06/02	CMX (NC1)" */

#if !defined(AIX) && !defined(__OSF__)
typedef unsigned char uchar;
#endif

/* define CX_VALUE_OFFSET indicates the number of information bytes	*/
/* in this case : type-byte and length-byte					*/

#define CX_VALUE_OFFSET	2


/* defines of errorvalues */

#define CX_FIELD_TOO_SHORT	-1
#define CX_WRONG_LENGTH	-2
#define CX_TYPE_NOT_FOUND	-3
#define CX_TLV_NOT_FOUND	-4
#define CX_LENGTH_TOO_LONG	-5

/* macros for tlv-infos	*/

#define CXTLV_TYPE(_tlv)		( (uchar) ( (uchar *)(_tlv) )[0]  )

#define CXTLV_TLV_LENGTH(_tlv)	( (int) (((uchar *)(_tlv) )[1] + 2) )

#define CXTLV_VAL_LENGTH(_tlv)	( (int) (((uchar *)(_tlv) )[1]) )

#define CXTLV_VALUE(_tlv)	( ( (uchar *)(_tlv) )[1]==0 ? \
					(uchar *)NULL : (uchar *)((uchar *)(_tlv)+2)  )

#ifndef CMX_TLV_C

/* functions prototypes   	*/

extern int cxtlv_create  (const uchar, const int, const uchar *,
			uchar *, const int);
extern int cxtlv_add     (const uchar *, const int, const uchar *, 
			const int, uchar *, const int);
extern int cxtlv_remove_elem  (const uchar *, const int, const uchar,
			uchar *, const int);
extern int cxtlv_remove_all	(const uchar *, const int, const uchar,
			uchar *, const int);
extern int cxtlv_first   (const uchar *, const int, 
			uchar *, const int);
extern int cxtlv_rest    (const uchar *, const int, 
			uchar *, const int);
extern int cxtlv_number_of(const uchar *, const int);
extern int cxtlv_find_type (const uchar *, const int, const uchar, 
			uchar *, const int);
extern int cxtlv_find_tlv (const uchar *, const int, const uchar *, 
			uchar *, const int);
extern int cxtlv_replace (const uchar *, const int, const uchar, 
			const uchar *, uchar *, const int);

#endif	/* CMX_TLV_C	*/
#endif /* _CMX_TLV_H */
