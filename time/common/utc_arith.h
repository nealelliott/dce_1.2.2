/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: utc_arith.h,v $
 * Revision 1.1.7.2  1996/02/18  23:12:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:36:56  marty]
 *
 * Revision 1.1.7.1  1995/12/08  18:08:25  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1994/09/26  13:31 UTC  pare
 * 	Merge bl18 changes
 * 	[1995/12/08  17:24:05  root]
 * 
 * Revision 1.1.5.6  1994/09/23  20:00:03  tom
 * 	Back out change of unsigned/signed64 to typedef hyper
 * 	[1994/09/23  19:26:49  tom]
 * 
 * Revision 1.1.5.4  1994/08/23  20:32:02  cbrooks
 * 	Code Cleanup
 * 	[1994/08/19  17:52:35  cbrooks]
 * 
 * Revision 1.1.5.3  1994/08/16  18:12:52  cbrooks
 * 	Move U64IMUL, U64IDIV to utc_arith.h
 * 	[1994/08/16  18:07:50  cbrooks]
 * 
 * 	More 64bit changes for 32bit machines
 * 	[1994/08/15  18:50:22  cbrooks]
 * 
 * 	CR 11494
 * 	[1994/08/09  19:00:01  cbrooks]
 * 
 * 	HP revision /main/HPDCE02/1  1994/06/29  20:32 UTC  pare
 * 	Merge hpdce02 (DCE 1.1 beta) and HPDCE01 into HPDCE02
 * 
 * Revision 1.1.4.2  1993/11/29  13:58:04  pare
 * 	OT9081
 * 	The macro UTCabsVal should directly call UTCnegate instead of calling it
 * 	indirectly. This will avoid unnecessary allocatition of a automatic variables.
 * 	[1993/11/29  13:56:58  pare]
 * 
 * Revision 1.1.2.3  1993/09/15  17:46:51  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.5.2  1994/05/12  15:17:35  rhw
 * 	continued code cleanup & merging
 * 	[1994/05/09  18:46:33  rhw]
 * 
 * 	first pass at code cleanup (compiler warnings, function prototypes)
 * 	[1994/04/25  14:10:50  rhw]
 * 
 * Revision 1.1.5.1  1994/04/21  22:05:18  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  20:05:23  cbrooks]
 * 
 * Revision 1.1.2.2  1992/12/30  20:40:19  htf
 * 	Embedding copyright notice
 * 	[1992/12/30  19:08:51  htf]
 * 
 * Revision 1.1  1992/01/19  15:32:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 	Header file UTC_ARITH - Contains Macros to do extended arithmetic
 *				on UTCValues.
 */
/*
 * Copyright (c) 1986 by
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
 * FACILITY:	DSS (Distributed System Services) Time Synchronization Service
 *
 * ABSTRACT:	Contains macros to support extended addition, subtraction and
 *		comparisions of UTC values.
 *		
 * ENVIRONMENT:	all
 *		
 *		
 * AUTHOR:	Carol Frampton
 *		Jan. 16, 1989
 *
 *		adapted from QUAD.H
 *		
 */

#ifndef _UTC_ARITH_H
#define _UTC_ARITH_H 1
#include <limits.h>
#include <dce/dtssdef.h>
#include <dce/dce64.h>

/*
 * All macros work on the datatype UTCValue.  All macros require POINTERS
 * to UTCValues as parameters.  Pointers are used so these macros can be 
 * converted to functions at a later date if that's deemed desireable for the
 * underlying hardware.
 *
 * All macros, that are not defined here, are defined in 
 * src/dce/utils/misc/dce64.h file.
 */ 

#define IsUTCzero(utc)	U64ISZERO (*(utc))

#define IsUTCneg(utc)  	U64ISNEG (*(utc))

/*
 * Signed UTC value comparisons
 * The greater than and less than macros depend on the high-order
 * longword being declared a signed long. They cast the low word to be
 * unsigned in the case that the high words are signed to make the
 * quad-word comparisons work.
 */

#define IsUTCgt(a,b)	( U64CMP(*(a), *(b)) == 1 )

#define IsUTCge(a,b)	( U64CMP(*(a), *(b)) >= 0 )

#define IsUTClt(a,b)	( U64CMP(*(a), *(b)) < 0 )

#define IsUTCle(a,b)	( U64CMP(*(a), *(b)) <= 0 )

#define IsUTCeq(a,b) 	U64EQUAL(*(a), *(b))

#define UTCadd(add1,add2,sum)					\
{								\
        UTCValue _intermediate_result;				\
	U64SET( _intermediate_result, *(add1) );		\
	U64ADD( _intermediate_result, *(add2) );		\
	U64SET( *(sum), _intermediate_result) ;			\
}

#define UTCzero(dest)		U64ZERO(*(dest))

#define UTCnegate(source,dest)	U64NEGATE( *(dest), *(source) )

#define UTCsub(sub1,sub2,result)			\
{							\
        UTCValue _intermediate_result;			\
	U64SET((_intermediate_result), *(sub1));	\
	U64SUB((_intermediate_result), *(sub2));	\
	U64SET(*(result),_intermediate_result);		\
}

/* 
 * UTCValue Absolute Value.  result = sub1 if sub1 is positive
 * or the absolute value of sub1 if it was negative.
 */
#define UTCabsVal(sub1,result)				\
{							\
	UTCValue   _utcZero;				\
	U64ZERO(_utcZero);				\
	if (U64ISNEG(*(sub1)))				\
	   UTCsub(&_utcZero, (sub1), result)		\
	else						\
	   *(result) = *(sub1);				\
}

/*
 * Shifts a 64 bit value up to 31 bits right or left.
 * These shift macros assume that the distance is <= 31 bits.
 * K&R says that negative shift distances give undefined results.
 */
#define UTCshiftLeft(num,dist)	   U64LSHIFT(*(num), dist);
#define UTCshiftRight(num,dist)    U64RSHIFT(*(num), dist);

/* 
 * UTCValue Multiply By 2. result = num*2
 */
#define UTCmultiplyBy2(num,result)  			\
{							\
        UTCValue _intermediate_ ; 			\
	U64SET(_intermediate_, *(num));			\
	U64LSHIFT(_intermediate_, 1);			\
	U64SET(*(result),_intermediate_) 		\
}

/* 
 * UTCValue Dvide By 2. result = num/2
 */
#define UTCdivideBy2(num,result)  			\
{							\
	UTCValue _intermediate_; 			\
	U64SET(_intermediate_, *(num));			\
	U64RSHIFT(_intermediate_, 1);			\
        U64SET(*(result),_intermediate_);		\
}

/*
 * UTCiAdd - macro to add a signed integer to a UTC time
 *
 * See UTCadd macro above
 */

#define UTCiAdd(add1,I,sum)	\
{							\
     UTCValue _intermediate_;				\
     U64IADD(*(add1),I,_intermediate_);			\
     U64SET(*(sum), _intermediate_);			\
}							    
 

/*
 * UTCiMul - macro to mutiply a UTC value by a signed integer
 */

#define UTCiMul(num,I,result)		\
{					\
    UTCValue _intermediate_ ;		\
    U64IMUL(*(num),I,_intermediate_);	\
    U64SET(*(result), _intermediate_);	\
}    

/*
 * UTCiDiv - macro to divide a UTC value by a signed integer
 */

#define UTCiDiv(dividend,I,quot,remd)				\
	U64IDIV(*(dividend),I,*(quot),*(remd))

#define UTCassign(destination,source_lo,source_hi)		\
	U64SET64(*(destination), source_hi, source_lo)

#define UTCcopy(d,s)	U64COPY(*(d),*(s))

/*
 * 64, 32 bit differences
 */

#if LONG_BIT >= 64
#define UTCinit(lo,hi) 				\
	{((unsigned long)(hi)<<32)|lo}

#define UTClow(low)  (*(low))

#define U64IMUL(num,I,result)	((result) = (num) * I)

#define U64IDIV(dividend,I,quot,remd)				\
{								\
    (remd) = (long)(dividend) % (I);			\
    (quot) = (long)(dividend) / (I);				\
}

#else	/* LONG_BIT */
#if LONG_BIT == 32 

#define UTCinit(lo,hi) 	 {hi,lo}

#define UTClow(low)  ((low)->lo)

#define U64IMUL(num,I,result)					\
{ 								\
    UTCValue _iMul_intermediate; 				\
								\
    U64SET(_iMul_intermediate, (num));				\
    uemul((num).lo,(unsigned long)(I),&(result)); 		\
    (result).hi += (_iMul_intermediate.hi)*(I); 		\
    if ((long)(I) < 0) 						\
        (result).hi -= _iMul_intermediate.lo; 			\
}


#define U64IDIV(dividend,I,quot,remd)                                   \
    {                                                                   \
    UTCValue _iDiv_intermed, _iDiv_intermed2;				\
    int _negDividend = 0;						\
									\
    _iDiv_intermed.hi = 0;						\
    if ((dividend).hi & 0x80000000UL) {					\
	_negDividend = 1;						\
	_iDiv_intermed.lo = ~(dividend).hi;				\
	if (!((_iDiv_intermed2.lo = -(dividend).lo) & 0xffffffffUL))	\
	    _iDiv_intermed.lo++;					\
    } else {								\
	_iDiv_intermed.lo = (dividend).hi;				\
	_iDiv_intermed2.lo = (dividend).lo;				\
    }									\
    (void) uediv(&_iDiv_intermed,					\
		 (unsigned long)((long)(I) < 0 ? -(I) : (I)),		\
		 &((quot).hi), &_iDiv_intermed2.hi);			\
    (void) uediv(&_iDiv_intermed2,					\
		 (unsigned long)((long)(I) < 0 ? -(I) : (I)),		\
		 &((quot).lo), (unsigned long *)&(remd));		\
    if (_negDividend ^ (long)(I) < 0) {					\
	(quot).hi = ~(quot).hi;					\
	if (!(((quot).lo = -(quot).lo) & 0xffffffffUL))			\
	    (quot).hi++;						\
    }									\
    if (_negDividend)							\
	(remd) = -(remd);						\
    }
#else
#error "long integers must have at least 32 bits of precision"
#endif  /* LONG_BIT == 32 */
#endif	/* LONG_BIT >= 64 */

#endif  /* #ifndef _UTC_ARITH_H */
