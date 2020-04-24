/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: time_support.c,v $
 * Revision 1.1.6.2  1996/02/17  23:34:35  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:42  marty]
 *
 * Revision 1.1.6.1  1995/12/08  18:08:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:24:01  root]
 * 
 * Revision 1.1.4.3  1994/08/16  18:12:50  cbrooks
 * 	CR 11494
 * 	[1994/08/09  19:00:06  cbrooks]
 * 
 * Revision 1.1.4.2  1994/05/16  18:51:40  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:11  rhw]
 * 
 * Revision 1.1.4.1  1994/05/12  15:17:27  rhw
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:45  rhw]
 * 
 * Revision 1.1.2.2  1992/12/30  16:34:23  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:10:48  htf]
 * 
 * Revision 1.1  1992/01/19  15:32:01  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 *	Module: TIME_SUPPORT.C
 */
/*
 ****************************************************************************
 *									    *
 *  COPYRIGHT (c) 1988, 1989 BY						    *
 *  DIGITAL EQUIPMENT CORPORATION, MAYNARD, MASSACHUSETTS.		    *
 *  ALL RIGHTS RESERVED.						    *
 * 									    *
 *  THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND COPIED   *
 *  ONLY IN  ACCORDANCE WITH  THE  TERMS  OF  SUCH  LICENSE  AND WITH THE   *
 *  INCLUSION OF THE ABOVE COPYRIGHT NOTICE. THIS SOFTWARE OR  ANY  OTHER   *
 *  COPIES THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY   *
 *  OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE IS  HEREBY   *
 *  TRANSFERRED.							    *
 * 									    *
 *  THE INFORMATION IN THIS SOFTWARE IS  SUBJECT TO CHANGE WITHOUT NOTICE   *
 *  AND  SHOULD  NOT  BE  CONSTRUED AS  A COMMITMENT BY DIGITAL EQUIPMENT   *
 *  CORPORATION.							    *
 * 									    *
 *  DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE  OR  RELIABILITY OF ITS   *
 *  SOFTWARE ON EQUIPMENT WHICH IS NOT SUPPLIED BY DIGITAL.		    *
 * 									    *
 *									    *
 ****************************************************************************
 */

/*
 * FACILITY:	Distributed Time Synchronization Service (DTSS)
 *
 * ABSTRACT:
 *
 *	This module contains support routines for manipulating the
 *	UTC time format.
 *
 * ENVIRONMENT: 	portable
 *
 * AUTHOR:		Carol Frampton
 *
 * CREATION DATE: 	February 1989
 *
 */

#include <dts.h>
#include <dtss_includes.h>	/* DTSS top-level header file */

/*
 * 	Externals
 */

/*
 *	Statics
 */


/*
 * GetTDF () -
 *
 * Description:
 *
 *      Returns the TDF as an extended inaccuracy.  
 *
 * Inputs:
 *
 *      utcTime - The time that contains the tdf to return.  If it is
 *                a null pointer, than the current tdf is returned.
 *
 * Outputs:
 *
 *	none.
 *
 * Return Value:
 *
 *	none
 *
 * Side Effects:
 *	
 *	bugchecks if there is an error
 */
ExtInacc GetTDF(utcTime)
utc_t *utcTime;
{
    ExtTDF  extTDF;
    long seconds;
    
    utc_bintime((timespec_t *)NULL, (timespec_t *)NULL, &seconds,
		utcTime);
    
    emul( seconds, K_100NS_PER_SEC, &extTDF );
    
    return( extTDF );
}



/*
 * AddLSToInacc () -
 *
 * Description:
 *
 *	Adds a leap second to the inaccuracy.  Does not check for infinite
 *	inaccuracy.
 *
 * Inputs:
 *
 *	Iptr - pointer to the inaccuracy
 *
 * Outputs:
 *
 *	the inaccuracy, Iptr, had a leap second added to it
 *
 * Return Value:
 *
 *	none
 *
 */

void AddLSToInacc (Iptr)

ExtInacc 	*Iptr;

{
    if (! IsInfiniteInacc (Iptr))
	UTCiAdd (Iptr, 1*K_100NS_PER_SEC, Iptr);
}

/*
 * CvtUTCValueToStr() -
 *
 * Description:
 *
 *	Convert an 8-byte numeric UTC value to an ASCIZ character string.
 *	The buffer, strPtr, should be at least 20+1 characters long to hold 
 *	the largest character string which could result from the conversion.
 *
 *	This routine is for DEBUGGING ONLY.
 *
 * Inputs:
 *
 *	utcPtr - pointer to UTC time value to convert to a string
 *	strPtr - pointer to the ASCIZ character string
 *	bufLen - length of the buffer pointed to by strPtr; should be at
 *	         20+1 characters long to hold the longest possible string
 *
 * Outputs:
 *
 *	strPtr - the ASCIZ string representation of the UTC value
 *
 * Return Value:
 *
 *	none
 *
 * Side Effects:
 *	
 *	bugchecks if there is a buffer overflow
 */

void CvtUTCValueToStr (utcPtr, strPtr, bufLen)

UTCValue *utcPtr;
char *strPtr;
int bufLen;

{
    UTCValue utcTemp;
    unsigned long remd1, remd2;
    int negflag = 0;

    /*
     * Convert the time to decimal
     */

    if (IsUTCneg( utcPtr )) {
	UTCnegate( utcPtr, &utcTemp );
	negflag++;
    } else {
	utcTemp = *utcPtr;
    }
	
    UTCiDiv( &utcTemp, 10000000, &utcTemp, &remd1 );
    UTCiDiv( &utcTemp, 10000000, &utcTemp, &remd2 );

    /*
     * Make sure there is room for the string
     */
    if (bufLen > 20)
	if (negflag)
	    (void) sprintf( strPtr, "-%ld%07ld%07ld",
			    UTClow(&utcTemp), remd2, remd1 );
	else
            (void) sprintf( strPtr, "%ld%07ld%07ld",
			    UTClow(&utcTemp), remd2, remd1 );
    else
        BugCheck();

}
