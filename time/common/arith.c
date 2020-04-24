/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: arith.c,v $
 * Revision 1.1.7.2  1996/02/17  23:34:32  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:39  marty]
 *
 * Revision 1.1.7.1  1995/12/08  18:05:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:23:39  root]
 * 
 * Revision 1.1.5.7  1994/09/23  20:00:00  tom
 * 	Back out change of unsigned/signed64 to typedef hyper
 * 	[1994/09/23  19:26:47  tom]
 * 
 * Revision 1.1.5.5  1994/08/23  20:31:45  cbrooks
 * 	Code Cleanup
 * 	[1994/08/19  17:52:20  cbrooks]
 * 
 * Revision 1.1.5.4  1994/08/16  18:12:43  cbrooks
 * 	CR 11494
 * 	[1994/08/09  18:59:49  cbrooks]
 * 
 * Revision 1.1.5.3  1994/05/16  18:51:35  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:07  rhw]
 * 
 * Revision 1.1.5.2  1994/05/12  15:16:49  rhw
 * 	continued code cleanup & merging
 * 	[1994/05/09  18:46:15  rhw]
 * 
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:08  rhw]
 * 
 * Revision 1.1.5.1  1994/04/07  10:22:33  kevins
 * 	CR 10304 Export some header files for cross component dependency issues
 * 	[1994/04/07  10:18:19  kevins]
 * 
 * Revision 1.1.2.2  1992/12/30  16:33:34  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:09:47  htf]
 * 
 * Revision 1.1  1992/01/19  15:31:41  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 	Module arith.c
 *	Version X0.1
 */
/*
 * Copyright (c) 1989 by
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
 * ABSTRACT:
 *
 * ENVIRONMENT: Portable
 *
 * AUTHORS:
 *		Joe Comuzzi
 *		Distributed Systems Services (DSS) 
 * 
 * DATE:	August 11, 1989
 *
 */

/*
 * If INCLUDED is defined, this file is being included by portable_api.c
 * in that case, declare all the routines static and don't re-include
 * dtss_includes.h. Otherwise, declare all the routines as externally
 * available.
 */

#ifdef INCLUDED
#define STATIC static
#else
#include <dts.h>
#include <dce/dtss_includes.h>	/* DECdts top-level header file which */
                                /* defines Bits64 */
#define STATIC
#endif

STATIC void emul(long int u, long int v, Bits64 *prodPtr);
STATIC void ueemul(Bits64 *uPtr, Bits64 *vPtr, Bits128 *prodPtr);
STATIC int uediv(Bits64 *dividendPtr, unsigned long divisor, unsigned long *quotPtr, unsigned long *remdPtr);
STATIC void uemul(unsigned long u, unsigned long v, Bits64 *prodPtr);
STATIC void ueemul( Bits64 *uPtr, Bits64 *vPtr, Bits128 *prodPtr);


/*
 *++
 *  uemul()
 *
 *  Functional Description:
 *        32-bit unsigned quantity * 32-bit unsigned quantity
 *        producing 64-bit unsigned result. This routine assumes
 *        long's contain at least 32 bits. It makes no assumptions
 *        about byte orderings.
 *  
 *  Inputs:
 *
 *        u, v       Are the numbers to be multiplied passed by value
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *        prodPtr    is a pointer to the 64-bit result 
 *
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *
 *  Side Effects:
 *
 *
 *  Note:
 *        This algorithm is taken from: "The Art of Computer
 *        Programming", by Donald E. Knuth. Vol 2. Section 4.3.1
 *        Pages: 253-255.
 *--
 */

STATIC void uemul( unsigned long u, unsigned long v, Bits64 *prodPtr)

{
    /* Following the notation in Knuth, Vol. 2 */

    unsigned long int   u1, u2, v1, v2, temp;

#if LONG_BIT >= 64
    *prodPtr = u * v;
#else 	/* LONG_BIT */
    u1 = u >> 16;
    u2 = u & 0xffff;
    v1 = v >> 16;
    v2 = v & 0xffff;

    temp = u2 * v2;
    prodPtr->lo = temp & 0xffff;
    temp = u1 * v2 + (temp >> 16);
    prodPtr->hi = temp >> 16;
    temp = u2 * v1 + (temp & 0xffff);
    prodPtr->lo += (temp & 0xffff) << 16;
    prodPtr->hi += u1 * v1 + (temp >> 16);
#endif	/* LONG_BIT */

}
/* End of routine uemul */

/*
 *++
 *  emul()
 *
 *  Functional Description:
 *        32-bit signed quantity * 32-bit signed quantity
 *        producing 64-bit signed result. This routine assumes
 *        long's contain at least 32 bits. It makes no assumptions
 *        about byte orderings.
 *  
 *  Inputs:
 *
 *        u, v       Are the numbers to be multiplied passed by value
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *        prodPtr    is a pointer to the 64-bit result 
 *
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *
 *  Side Effects:
 *
 *
 *--
 */

STATIC void emul( long int u, long int v, Bits64 *prodPtr)

{
#if LONG_BIT >= 64
    *prodPtr = u * v;
#else	/* LONG_BIT */
    uemul((unsigned long)u, (unsigned long)v, prodPtr);

    /* Correct for signed multiply */
    if (u < 0)
        prodPtr->hi -= v;
    if (v < 0)
        prodPtr->hi -= u;
#endif	/* LONG_BIT */

}
/* End of routine emul */

/*
 *++
 *  uediv()
 *
 *  Functional Description:
 *        64-bit unsigned quantity / 32-bit unsigned quantity
 *        producing 32-bit unsigned quotient and 32-bit unsigned
 *        remainder. This routine assumes long's contain at least
 *        32 bits. It makes no assumptions about byte orderings.
 *  
 *  Inputs:
 *
 *        DividendPtr   Pointer to number to be divided.
 *        Divisor       Divisor
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *        QuotPtr       Pointer to quotient
 *        RemdPtr       Pointer to remainder
 *
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *        0          success
 *        -1         overflow or divison by zero.
 *
 *  Side Effects:
 *
 *
 *  Note:
 *        This algorithm is taken from: "The Art of Computer
 *        Programming", by Donald E. Knuth. Vol 2. Section 4.3.1
 *        Pages: 257-261.
 *--
 */

STATIC int uediv( Bits64 *dividendPtr, unsigned long divisor, unsigned long *quotPtr, unsigned long *remdPtr)

{
#if LONG_BIT >= 64
    if (remdPtr)
        *remdPtr = *dividendPtr % divisor;
    *quotPtr = *dividendPtr / divisor;
    return(0);
#else	/* LONG_BIT */
    /* Following the notation in Knuth, Vol. 2 */

    unsigned long int u1, u2, u3, u4, v1, v2, temp, trial;
    unsigned long int q1, q2;
    unsigned long int v, uhigh, ulow;

    /* D1 */
    if (!(v = divisor))
        return(-1);

    uhigh = dividendPtr->hi;
    ulow = dividendPtr->lo;
    while (!(v & 0x80000000UL)) {
        v = v << 1;
        if (uhigh & 0x80000000UL)
            return(-1);

        uhigh = (uhigh << 1) | ulow >> 31;
        ulow = ulow << 1;
    }        

    v1 = v >> 16;
    v2 = v & 0xffff;

    u1 = uhigh >> 16;
    u2 = uhigh & 0xffff;
    u3 = ulow >> 16;
    u4 = ulow & 0xffff;

    if (v <= uhigh)
        return(-1);

    /* D3                        Calculate Trial divisor */
    if (u1 == v1)
        trial = 0xffff;
    else
        trial = uhigh / v1;

    while (!((uhigh - v1 * trial) & 0xffff0000UL) &&
           ((v2 * trial) > (((uhigh - v1 * trial) << 16) + u3)))
        trial--;

    /* D4                        Multiply and subtract      */
    temp = v2 * trial;        /* Replace (u1,u2,u3) by      */
    u3 -= temp & 0xffff;      /* (u1,u2,u3) - trial*(v1,v2) */
    if (u3 & 0xffff0000UL) {
        u2--;
        u3 &= 0xffff;
    }
    u2 -= (v1 * trial + (temp >> 16));
    u2 &= 0xffff;

    /* D5 */
    q1 = trial;

    /* The complication of step D6 can not occur, since n = 2! */

    /* D3                        Calculate Trial divisor */
    uhigh = (u2 << 16) + u3;
    if (u2 == v1)
        trial = 0xffff;
    else
        trial = uhigh / v1;

    while (!((uhigh - v1 * trial) & 0xffff0000UL) &&
           ((v2 * trial) > (((uhigh - v1 * trial) << 16) + u4)))
        trial--;

    /* D4                        Multiply and subtract      */
    temp = v2 * trial;        /* Replace (u2,u3,u4) by      */
    u4 -= temp & 0xffff;      /* (u2,u3,u4) - trial*(v1,v2) */
    if (u4 & 0xffff0000UL) {
        u3--;
        u4 &= 0xffff;
    }
    u3 -= (v1 * trial + (temp >> 16));
    u3 &= 0xffff;

    /* D5 */
    q2 = trial;

    /* The complication of step D6 can not occur, since n = 2! */

    /* D8                        Unnormalize */
    *quotPtr = (q1 << 16) + q2;

    if (remdPtr == NULL)
        return(0);

    v = divisor;
    uhigh = (u3 << 16) + u4;

    while (!(v & 0x80000000UL)) {
        v = v << 1;
        uhigh = uhigh >> 1;
    }        
    *remdPtr = uhigh;
    return(0);
#endif	/* LONG_BIT */

}
/* End of routine uediv */

#ifndef INCLUDED
/*
 *++
 *  ediv()
 *
 *  Functional Description:
 *        64-bit signed quantity / 32-bit signed quantity
 *        producing 32-bit signed quotient and 32-bit signed
 *        remainder. The division is performed such that the
 *        remainder has the same sign as the dividend. This
 *        routine assumes long's contain at least 32 bits.
 *        It makes no assumptions about byte orderings.
 *  
 *  Inputs:
 *
 *        DividendPtr   Pointer to number to be divided.
 *        Divisor       Divisor
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *        QuotPtr       Pointer to quotient
 *        RemdPtr       Pointer to remainder
 *
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *
 *  Side Effects:
 *
 *
 *--
 */

STATIC int ediv(dividendPtr, divisor, quotPtr, remdPtr)

Bits64                *dividendPtr;
long int              divisor;
long int              *quotPtr, *remdPtr;

{
#if LONG_BIT >= 64
    if (remdPtr)
        *remdPtr = *dividendPtr % divisor;
    *quotPtr = *dividendPtr / divisor;
    return(0);
#else	/* LONG_BIT */
    Bits64            tempDividend;
    long int          tempDivisor;
    int               negDividend, negDivisor;
    int               overflow;
    unsigned long int quotient, remainder;

    if (dividendPtr->hi & 0x80000000UL) {
        negDividend = 1;
        if (tempDividend.lo = -dividendPtr->lo)
            tempDividend.hi = ~(dividendPtr->hi);
        else
            tempDividend.hi = -(dividendPtr->hi);
    } else {
        negDividend = 0;
        tempDividend = *dividendPtr;
    }
        

    if (negDivisor = (divisor < 0))
        tempDivisor = - divisor;
    else
        tempDivisor = divisor;

    overflow = uediv(&tempDividend, (unsigned long)tempDivisor,
               &quotient, (remdPtr == NULL ? NULL : &remainder));

    *quotPtr = (long) quotient;

    if (remdPtr != NULL) {
	*remdPtr = (long) remainder;
	if (negDividend)
	    *remdPtr = - *remdPtr;
    }

    if (overflow)
        return(-1);

    if (negDivisor ^ negDividend) {
        *quotPtr = - *quotPtr;
        if (*quotPtr > 0)
            return(-1);
    } else {
        if (*quotPtr < 0)
            return(-1);
    }

    return(0);
#endif /* LONG_BIT */
}
/* End of routine ediv */
#endif

/*
 *++
 *  ueemul()
 *
 *  Functional Description:
 *        64-bit unsigned quantity * 64-bit unsigned quantity
 *        producing 128-bit unsigned result. This routine assumes
 *        long's contain at least 32 bits. It makes no assumptions
 *        about byte orderings.
 *  
 *  Inputs:
 *
 *        uPtr, vPtr  Are pointer to the numbers to be multiplied
 *
 *  Implicit Inputs:
 *
 *
 *  Outputs:
 *
 *        prodPtr    is a pointer to the 128-bit result 
 *
 *  Implicit Outputs:
 *
 *      
 *  Value Returned:
 *
 *
 *  Side Effects:
 *
 *
 *  Note:
 *        This algorithm is taken from: "The Art of Computer
 *        Programming", by Donald E. Knuth. Vol 2. Section 4.3.1
 *        Pages: 253-255.
 *--
 */

STATIC void ueemul( Bits64 *uPtr, Bits64 *vPtr, Bits128 *prodPtr)

{
#if LONG_BIT >= 64
    Bits64              u1, u2, v1, v2, temp;

    u1 = *uPtr >> 32;
    u2 = *uPtr & 0xffffffff;
    v1 = *vPtr >> 32;
    v2 = *vPtr & 0xffffffff;

    temp = u2 * v2;
    prodPtr->lo = temp & 0xffffffff;
    temp = u1 * v2 + (temp >> 32);
    prodPtr->hi = temp >> 32;
    temp = u2 * v1 + (temp & 0xffffffff);
    prodPtr->lo += (temp & 0xffffffff) << 32;
    prodPtr->hi += u1 * v1 + (temp >> 32);
#else	/* LONG_BIT */
    Bits64		temp, temp2;

    uemul(uPtr->lo, vPtr->lo, &temp);
    prodPtr->lolo = temp.lo;
    uemul(uPtr->hi, vPtr->lo, &temp2);
    UTCiAdd(&temp2, temp.hi, &temp2);
    prodPtr->hilo = temp2.hi;
    uemul(uPtr->lo, vPtr->hi, &temp);
    UTCiAdd(&temp, temp2.lo, &temp);
    prodPtr->lohi = temp.lo;
    uemul(uPtr->hi, vPtr->hi, &temp2);
    UTCiAdd(&temp2, temp.hi, &temp2);
    UTCiAdd(&temp2, prodPtr->hilo, &temp2);
    prodPtr->hilo = temp2.lo;
    prodPtr->hihi = temp2.hi;
#endif	/* LONG_BIT */
}
/* End of routine ueemul */
