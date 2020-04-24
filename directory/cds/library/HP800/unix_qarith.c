/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: unix_qarith.c,v $
 * Revision 1.1.4.2  1996/02/18  19:31:56  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:12:47  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:19:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:44:17  root]
 * 
 * Revision 1.1.2.2  1994/08/03  19:02:59  mccann
 * 	includes file cleanup
 * 	[1994/07/19  18:07:46  mccann]
 * 
 * Revision 1.1.2.1  1994/06/30  19:15:58  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:11:05  mccann]
 * 
 * $EndLog$
 */
#if !defined(lint) && !defined(SABER)
static char *SccsID = "@(#)qarith_lib.c\t7\t(DECdns)\t11/9/1990";
#endif	/* neither lint nor saber-C */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990. ALL RIGHTS RESERVED.
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
 */
#include <dce/dce.h>

typedef struct Bits64
{
  unsigned32 lo;
  unsigned32 hi;
} UTCValue;

/*
 * All macros work on the datatype UTCValue.  All macros require POINTERS
 * to UTCValues as paramters.  Pointers are used so these macros can be
 * converted to functions at a later date if that's deemed desireable for the
 * underlying hardware.
 */

/*
 * UTCadd - macro to add two UTC times
 *
 * add lo and high order longword separately, using sign bits of the low-order
 * longwords to determine carry.  sign bits are tested before addition in two
 * cases - where sign bits match. when the addend sign bits differ the sign of
 * the result is also tested:
 *
 *	  sign		  sign
 *	addend 1	addend 2	carry?
 *
 *	    1		    1		 true
 *	    1		    0		 true if sign of sum clear
 *	    0		    1		 true if sign of sum clear
 *	    0		    0		 false
 */
#define UTCadd(add1,add2,sum)						\
    if (!(((add1)->lo&0x80000000) ^ ((add2)->lo&0x80000000)))		\
    {									\
        if (((add1)->lo&0x80000000))					\
    	{								\
	    (sum)->lo = (add1)->lo + (add2)->lo ;			\
	    (sum)->hi = (add1)->hi + (add2)->hi+1 ;			\
	} 								\
	else								\
    	{								\
	    (sum)->lo  = (add1)->lo + (add2)->lo ;			\
	    (sum)->hi = (add1)->hi + (add2)->hi ;			\
    	} 								\
    }									\
    else								\
    {									\
	(sum)->lo = (add1)->lo + (add2)->lo ;				\
	(sum)->hi = (add1)->hi + (add2)->hi ;				\
        if (!((sum)->lo&0x80000000)) 					\
	    (sum)->hi++ ;						\
    }

/*
 * Negate the UTC value.  We take the one's complement of both the lo and high
 * order longwords separately.  Then we do quad add of one.
 */
#define UTCnegate(source,dest)		    \
    {					    \
	UTCValue _utcOne;		    \
	_utcOne.hi = 0;		    	    \
	_utcOne.lo = 1;		    	    \
					    \
	(dest)->lo = ~((source)->lo);	    \
	(dest)->hi = ~((source)->hi);	    \
	UTCadd((dest), &_utcOne, dest);	    \
    }

/*
 * UTCValue Subtract.  result = sub1 - sub2.  First negates sub2 then
 * adds the result to sub1.
 */
#define UTCsub(sub1,sub2,result)                \
    {                                           \
	UTCValue _sub_intermediate;             \
						\
        UTCnegate((sub2), &_sub_intermediate)   \
        UTCadd((sub1), &_sub_intermediate, result)\
    }

/*
 * UTCValue Absolute Value.  result = sub1 if sub1 is positive
 * or the absolute value of sub1 if it was negative.
 */
#define UTCabsVal(sub1,result)			\
    {						\
        UTCValue _utcZero;		        \
        _utcZero.lo = _utcZero.hi = 0;	   	\
						\
        if ((sub1)->hi & 0x80000000)		\
           UTCsub(&_utcZero, (sub1), (result))	\
	else					\
	   *(result) = *(sub1);			\
    }

/*
 * Shifts a 64 bit value up to 31 bits right or left.
 * These shift macros assume that the distance is <= 31 bits.
 * K&R says that negative shift distances give undefined results.
 */
#define UTCshiftLeft(num,dist)			\
{                                               \
    (num)->hi  = (num)->hi << (dist);       	\
    (num)->hi |= (num)->lo >> (32-(dist));  	\
    (num)->lo  = (num)->lo << (dist);       	\
}

#define UTCshiftRight(num,dist)			\
{                                               \
    (num)->lo  = (num)->lo >> (dist);       	\
    (num)->lo |= (num)->hi << (32-(dist));  	\
    (num)->hi  = (num)->hi >> (dist);       	\
}

/*
 * UTCValue Multiply By 2. result = num*2
 */
#define UTCmultiplyBy2(num,result)			\
{                                               \
    (result)->hi  = (num)->hi << (1);       	\
    (result)->hi |= (num)->lo >> (32-(1));  	\
    (result)->lo  = (num)->lo << (1);       	\
}

/*
 * UTCValue Dvide By 2. result = num/2
 */
#define UTCdivideBy2(num,result)		\
{                                               \
    (result)->lo  = (num)->lo >> (1);       	\
    (result)->lo |= (num)->hi << (32-(1));  	\
    (result)->hi  = (num)->hi >> (1);       	\
}


/*
 * UTCmult () -
 *
 * Description:
 *
 *	Multiplies two UTC time values together to produce another UTC time
 *	value.  TRUE is returned if an overflow occurs.
 *
 * Inputs:
 *
 *	a, b	- pointer to UTC times to multiply together
 *
 *	assumes that xxx.hi and xxx.lo are 32 bits wide.
 *
 * Outputs:
 *
 *	result	- pointer to the result of a * b
 *
 * Return Value:
 *
 *	FALSE	- no overflow
 *	TRUE	- overflow occurred
 *
  */

int
UTCmult(
    UTCValue *a,
    UTCValue *b,
    UTCValue *result)
{
    UTCValue la;
    UTCValue lb;

    la = *a;
    lb = *b;
    result->hi = result->lo = 0;
    while (lb.hi || lb.lo)
    {
        if (lb.lo & 1)
            UTCadd (&la, result, result);
        if (la.hi & 0x80000000)
            return (TRUE);
        UTCshiftLeft  (&la, 1);
        UTCshiftRight (&lb, 1);
    }
    return (FALSE);
}


void
dns_qadd (
    void *add1,
    void *add2,
    void *sum)
{
	UTCadd((UTCValue *)add1, (UTCValue *)add2, (UTCValue *)sum);
	return;
}

void
dns_qsub (
    void *min,
    void *sub,
    void *result)
{
	UTCsub((UTCValue *)min, (UTCValue *)sub, (UTCValue *)result);
	return;
}

void
dns_emul (
    long *mult1,
    long *mult2,
    void *prod)
{
	UTCValue quad1, quad2;

	quad1.hi = 0;
	quad1.lo = *mult1;
	quad2.hi = 0;
	quad2.lo = *mult2;
	(void)UTCmult (&quad1, &quad2, (UTCValue *)prod);
	return;
}

void
dns_ediv (
    long *divisor,
    void *dividend,
    long *quotient,
    long *remainder)
{
    unsigned long rem = 0;
    long divh = ((UTCValue *)dividend)->hi;
    long divl = ((UTCValue *)dividend)->lo;
    unsigned long div = *divisor;
    int rep = 64;

    do {
	rem <<= 1; if (divh < 0) rem++;
	divh <<= 1; if (divl < 0) divh++;
	divl <<= 1;

	if (rem >= div) {
	    rem -= div;
	    divl++;
	}
    } while (--rep);

    if (quotient) *quotient = divl;
    if (remainder) *remainder = rem;
}

void
dns_qmul (
    void *mult1,
    void *mult2,
    void *prod)
{
	(void)UTCmult ((UTCValue *)mult1, (UTCValue *)mult2, (UTCValue *)prod);
}





