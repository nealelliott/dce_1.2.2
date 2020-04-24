/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: commondef.h,v $
 * Revision 1.1.6.2  1996/02/18  23:11:16  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:36:10  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:05:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:40  root]
 * 
 * Revision 1.1.4.2  1994/08/23  20:31:46  cbrooks
 * 	Code Cleanup
 * 	[1994/08/19  17:52:21  cbrooks]
 * 
 * Revision 1.1.4.1  1994/05/12  15:16:50  rhw
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:09  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  20:38:12  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:06:38  htf]
 * 
 * Revision 1.1  1992/01/19  15:31:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 	Module: COMMONDEF.H
 */
/*
 * Copyright (c) 1989,1990 by
 * Digital Equipment Corporation, Maynard, Mass.
 * 
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 *
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:    Top-level header file for the COMMON component of DTSS
 *
 * ENVIRONMENT:	all
 *
 * AUTHORS:	Carol Frampton
 *		Distributed Systems Services (DSS) 
 * 
 * DATE:	February 24, 1989
 *	
 */


#ifndef _COMMONDEF_H
#define _COMMONDEF_H

/*
 *	DEFINES for Common Routines
 */



/*
 *	TYPEDEFS for Common Routines
 */



/*
 * 	PROTOTYPES for Common Routines
 */

/*
 * DTSS_MALLOC.C
 */
pointer_t DTSSmalloc _DCE_PROTOTYPE_ ((unsigned ));
void  DTSSfree _DCE_PROTOTYPE_ ((pointer_t ));

/*
 * TIME_SUPPORT.C
 */
ExtInacc GetTDF _DCE_PROTOTYPE_ ((utc_t *));
void AddLSToInacc _DCE_PROTOTYPE_ ((ExtInacc *));

/*
 * ARITH.C
 */

void emul _DCE_PROTOTYPE_ ((long int , long int , Bits64 *));
void uemul _DCE_PROTOTYPE_ ((unsigned long int , unsigned long int ,
                        Bits64 *));
int ediv _DCE_PROTOTYPE_ ((Bits64 *, long int ,
                      long int *, long int *));
int uediv _DCE_PROTOTYPE_ ((Bits64 *, unsigned long int ,
                       unsigned long int *, unsigned long int *));

void CvtUTCValueToStr _DCE_PROTOTYPE_ ((UTCValue *utcPtr, char *strPtr,
                                   int bufLen));

/*
 * Internal hook in UTCAPI.C
 */

int utc_comptime _DCE_PROTOTYPE_ (
		(UTCValue *timevalue,
		 ExtInacc *inaccvalue,
		 long *tdf,
		 utc_t *utc) );

int utc_mkcomptime _DCE_PROTOTYPE_ (
		(utc_t *utc,
		 UTCValue *timevalue,
		 ExtInacc *inaccvalue,
		 long tdf) );

int utc_getnextzone _DCE_PROTOTYPE_ (
		(utc_t *next,
		 utc_t *utc) );

int utc_settdf _DCE_PROTOTYPE_ (
		(utc_t *result,
		 utc_t *utc1,
		 long tdf) );

#endif  /* #ifndef _COMMONDEF_H */
