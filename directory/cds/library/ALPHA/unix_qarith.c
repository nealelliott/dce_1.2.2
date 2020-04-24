/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: unix_qarith.c,v $
 * Revision 1.1.4.2  1996/02/18  19:31:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:12:42  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:19:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:44:12  root]
 * 
 * Revision 1.1.2.4  1994/08/03  19:02:57  mccann
 * 	includes file cleanup
 * 	[1994/07/19  18:07:13  mccann]
 * 
 * Revision 1.1.2.3  1994/06/30  19:15:52  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:57:21  mccann]
 * 
 * Revision 1.1.2.2  1994/06/09  18:39:54  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:38  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/12  22:04:59  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:22:09  peckham]
 * 
 * $EndLog$
 */
/*
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
 * MODIFICATION HISTORY:
 *
 */

#include <dce/dce.h>

typedef unsigned long int UTCValue;

/*
 * All macros work on the datatype UTCValue.  All macros require POINTERS
 * to UTCValues as paramters.  Pointers are used so these macros can be 
 * converted to functions at a later date if that's deemed desireable for the
 * underlying hardware.
 */ 

/*
 * UTCadd - macro to add two UTC times
 *
 */
#define UTCadd(add1,add2,sum)    *(sum) = *(add1) + *(add2)

/*
 * Negate the UTC value.  We take the one's complement of both the lo and high 
 * order longwords separately.  Then we do quad add of one.  
 */
#define UTCnegate(source,dest)		    \
    {					    \
	UTCValue _utcOne;		    \
	_utcOne = 1;		    	    \
					    \
	*(dest) = ~(*(source));	            \
	UTCadd((dest), &_utcOne, (dest));   \
    }

/* 
 * UTCValue Subtract.  result = sub1 - sub2.  
 */
#define UTCsub(sub1,sub2,result)    *(result) = *(sub1) - *(sub2)

/* 
 * UTCValue Absolute Value.  result = sub1 if sub1 is positive
 * or the absolute value of sub1 if it was negative.
 */
#define UTCabsVal(sub1,result)			\
    {						\
        UTCValue _utcZero;		        \
        _utcZero = 0;	   	                \
						\
        if (*(sub1) & 0x8000000000000000)	\
           UTCsub(&_utcZero, (sub1), (result))	\
	else					\
	   *(result) = *(sub1);			\
    }

/*
 * Shifts a 64 bit value up to 31 bits right or left.
 * These shift macros assume that the distance is <= 31 bits.
 * K&R says that negative shift distances give undefined results.
 */
#define UTCshiftLeft(num,dist)     *(num)  = *(num) << (dist)
#define UTCshiftRight(num,dist)    *(num)  = *(num) >> (dist);

/* 
 * UTCValue Multiply By 2. result = num*2
 */
#define UTCmultiplyBy2(num,result)    *(result) = *(num)*2

/* 
 * UTCValue Dvide By 2. result = num/2
 */
#define UTCdivideBy2(num,result)    *(result) = *(num)/2

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
 *	assumes that xxx are 64 bits wide.
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
UTCmult (UTCValue  *a,
         UTCValue  *b,
         UTCValue  *result)
{
    UTCValue la, lb;

    la = *a;
    lb = *b;
    *(result) = 0L;
    while (lb)
    {
        if (lb & 1)
            UTCadd (&la, (result), (result));
        if (la & 0x8000000000000000)
            return (TRUE);
        UTCshiftLeft  (&la, 1);
        UTCshiftRight (&lb, 1);
    }
    return (FALSE);
}


void 
dns_qadd (void	*add1,
          void	*add2,
          void	*sum)
{
	UTCadd((UTCValue *)add1, (UTCValue *)add2, (UTCValue *)sum);
	return;
}

void 
dns_qsub (void	*min,
          void	*sub,
          void	*result)
{
	UTCsub((UTCValue *)min, (UTCValue *)sub, (UTCValue *)result);
	return;
}

void 
dns_emul (int	*mult1,
          int	*mult2,
          void	*prod)
{
	UTCValue quad1, quad2;

	quad1 = *mult1;
	quad2 = *mult2;
	(void)UTCmult (&quad1, &quad2, (UTCValue *)prod);
	return;
}

void 
dns_ediv (int	*divisor,
          void	*dividend,
          int	*quotient,
          int	*remainder)
{
    UTCValue *ldividend = (UTCValue *)dividend;

    *quotient  = *ldividend / *divisor;
    *remainder = *ldividend % *divisor;
}

void 
dns_qmul (void	*mult1,
          void	*mult2,
          void	*prod)
{
	(void)UTCmult ((UTCValue *)mult1, (UTCValue *)mult2, (UTCValue *)prod);
	return;
}

