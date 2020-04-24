/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: isasmb.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:06  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:29:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:54:47  root]
 * 
 * Revision 1.1.8.1  1994/02/22  16:03:58  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:57:18  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  12:56:50  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  14:04:06  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  18:49:43  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:15:15  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/07  13:19:01  zeliff
 * 	Removed duplicate OSF copyright markers
 * 	[1992/12/06  00:42:56  zeliff]
 * 
 * Revision 1.1.4.2  1992/11/27  17:22:45  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  17:47:48  marrek]
 * 
 * Revision 1.1.2.3  1992/06/01  23:36:14  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:40:01  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: isasmb.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:05 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/***************************************************************************
 *
 *			   INFORMIX SOFTWARE, INC.
 *
 *			      PROPRIETARY DATA
 *
 *	THIS DOCUMENT CONTAINS TRADE SECRET DATA WHICH IS THE PROPERTY OF 
 *	INFORMIX SOFTWARE, INC.  THIS DOCUMENT IS SUBMITTED TO RECIPIENT IN
 *	CONFIDENCE.  INFORMATION CONTAINED HEREIN MAY NOT BE USED, COPIED OR 
 *	DISCLOSED IN WHOLE OR IN PART EXCEPT AS PERMITTED BY WRITTEN AGREEMENT 
 *	SIGNED BY AN OFFICER OF INFORMIX SOFTWARE, INC.
 *
 *	THIS MATERIAL IS ALSO COPYRIGHTED AS AN UNPUBLISHED WORK UNDER
 *	SECTIONS 104 AND 408 OF TITLE 17 OF THE UNITED STATES CODE. 
 *	UNAUTHORIZED USE, COPYING OR OTHER REPRODUCTION IS PROHIBITED BY LAW.
 *
 *
 *  Title:	isasmb.c
 *  Description:
 *		Low level utility routines.
 *		bycopy(src, dst, cnt)
 *		byfill(dst, cnt, chr)
 *		bycmpr(src, dst, cnt)
 *		ldlong(ptr)
 *		stlong(lng, ptr)
 *
 ***************************************************************************
 */


/*
 * utility routines which may be written in assembly language
 *
 * Relational Database Systems, Inc.
 * Roy Harrington	November 4, 1981
 *
 */

#include <machine.h>

/*
 * copy "cnt" bytes from "src" to "dst"
 */

bycopy(src, dst, cnt)
register char *src, *dst;
register int cnt;
{
	if (cnt >= 8 && 	/* big copy on word boundary ? */
	   (((int)src|(int)dst|(int)cnt) & ((sizeof(short))-1)) == 0)
	     wdcopy((short*) src, (short*) dst, cnt);
	else while(cnt-- > 0) *dst++ = *src++;
}

/*
 * copy "cnt"/sizeof(short) words from "src" to "dst"
 */

wdcopy(src, dst, cnt)
register short *src, *dst;
register int cnt;
{
	for ( ; cnt > 0; cnt -= sizeof(short))
	    *dst++ = *src++;
}

/*
 * fill "cnt" bytes starting at "dst" with "chr"
 */

byfill(dst, cnt, chr)
register char *dst;
register int cnt, chr;
{
	while(cnt-- > 0) *dst++ = chr;
}

/*
 * compare character (byte) strings
 *  returns:
 *	-1  => s1 <  s2
 *	 0  => s1 == s2
 *	 1  => s1 >  s2
 */

bycmpr(s1, s2, cnt)
register char *s1, *s2;
register int cnt;
{
	if (cnt <= 0) return(0);
	for ( ; *s1 == *s2; s1++, s2++)
	    if (--cnt == 0) return(0);

	if ((*s1 & BYTEMASK) < (*s2 & BYTEMASK))
	    return(-1);
	else return(1);
}


/*
 * load 4 byte number into a long
 */

#ifdef ldlong
#undef ldlong
#endif /* ldlong */

long ldlong(p)
register char *p;
{
	return((((((((long) p[0]
		<< BYTESHFT) + (p[1] & BYTEMASK))
		<< BYTESHFT) + (p[2] & BYTEMASK))
		<< BYTESHFT) + (p[3] & BYTEMASK)));
}

/*
 * store long into 4 byte number
 */

#ifdef stlong
#undef stlong
#endif /* stlong */

stlong(l,p)
register long l;
register char *p;
{
	p[0] = (l >> (BYTESHFT*3));
	p[1] = (l >> (BYTESHFT*2));
	p[2] = (l >> BYTESHFT);
	p[3] = l;
}

#ifdef PCDOS
peek(segment, offset, buffer, nbytes)
 
int segment;
int offset;
char *buffer;
unsigned nbytes;

{
char far *ptr;
	(*((unsigned *)&(ptr) + 1)) = segment;
	(*((unsigned *)&(ptr))) = offset;
	while (nbytes--)
		*buffer++ = *ptr++;
}
#endif /* PCDOS */

