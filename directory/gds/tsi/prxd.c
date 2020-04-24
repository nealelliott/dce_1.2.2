/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: prxd.c,v $
 * Revision 1.1.4.2  1996/03/11  13:24:48  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:06  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:49:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:04  root]
 * 
 * Revision 1.1.2.3  1994/07/06  15:08:47  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:23:11  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  21:18:41  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:09  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/10  16:16:01  marrek
 * 	Created for April Submission.
 * 	[1994/04/26  12:52:35  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: prxd.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:48 $";
#endif

/*
 * COPYRIGHT (C) 1987 BY SIEMENS AG
 * All rights reserved
 *  
 * This software is furnished under a license and may be used
 * only in accordance with the terms of that license and with the
 * inclusion of the above copyright notice. This software may not
 * be provided or otherwise made available to, or used by, any
 * other person. No title to or ownership of the software is
 * hereby transferred.
 */

#include <stdio.h>
/*
 * prxd -- preparation in xd format
 *
 * Syntax:
 * 	prx(ip, l, o, func)
 *      char *ip;       pointer to data to be prepared
 *      int l;          length itself
 *      int o;          start address
 *      int (*func)();  function for specifying a line
 *
 * Defines
 */
int prx(register char *ip, int l, int o, int (*func)(char *));

/*
static char sccsid[] = "@(#)prxd.c	311.2 93/08/24 CMX (NC1)";
*/

/*
 * output of a text in hexadecimal representation, such as
 *
 *   0      7 10 DW1   19     26 29       38      |  48 BAR1          65 BAR2
 *   |      |  |        |      |  |        |      |  |                |66 LEND
 *   V      v  v        v      v  v        v      v  v                vv
 *   _______0  48404030 67404366  003a3a43 20484f4c  |H@@0g0Cf ::C HOL|\n0
 *   <-adr ->  <--------- hexchar fields --------->   <-- printchar ->
 *             <------------- CMPLEN ------------->
 *
 * The prepatation from ip is carried out in l length in xd format.
 * if o can be divided by 16, the number up to the next 16 limit is stored
 * in the first lines of the first part (right justified) the second part
 * then starts at a 16 limit, the last part is only partly filled if necessary
 * (left justified). After a line, additional same lines are represented by
 * "*** same ***" dargestellt.
 * e.g. with prx(ip, 42, 9), ip is prepared as
 *
 *     9                       3a3a43 20484f4c  |         ::C HOL|\n
 *    10  48404030 67404366  003a3a43 20484f4c  |H@@0g0Cf ::C HOL|\n
 *    20              *** same ***\n
 *    30  484040                                |H@@             |\n
 *
 * For each generated line, prxd calls (*func)(line).
 */

/*
 * Two exchange buffers prev(ious) and curr(ent) for comparing lines
 * for "*** same ***"
 */
#define DW1	10
#define CMPLEN	38
#define BAR1 	48
#define BAR2	65
#define LEND	66
#define LINE	80
static char line1[LINE];
static char line2[LINE];

#define FALSE	0
#define TRUE	1
static char same[] = "                      *** same ***\n";
static prsame = FALSE;

static char seziffer[] = "0123456789abcdef";

int prx(register char *ip, int l, int o, int (*func)(char *))
{
	register char 	*hp;	/* hexchar fields in line */
	register char 	*pp;	/* printchar field in line */
	register 	k, hexcnt;
	char 		*prev;
	char 		*curr;
	
	for (k = 0, hp = line1, pp = line2; k < LINE; k++)
		hp[k] = pp[k] = ' ';
	hp[BAR1] = hp[BAR2] = pp[BAR1] = pp[BAR2] = '|';
	hp[LEND] = pp[LEND] = '\n';
	hp[LEND + 1] = pp[LEND + 1] = '\0';
	prev = line1;
	curr = line2;
	hp = curr;

	while (l > 0) {

		hexcnt = 1;
		if ((k = o % 16) != 0) {
			sprintf(&hp[0], "%8x  ", o);
			o &= 0xfffffff0;
			pp = &hp[BAR1 + 1];
			hp = &hp[DW1];
			while (k--) {
				*hp++ = ' ';
				*hp++ = ' ';
				*pp++ = ' ';
				if (hexcnt % 4 == 0)
					*hp++ = ' ';
				if (hexcnt % 8 == 0)
					*hp++ = ' ';
				hexcnt++;
			}
		} else {
			sprintf(&hp[0], "%8x  ", o);
			pp = &hp[BAR1 + 1];
			hp = &hp[DW1];
		}

		for (k = 0; k < l && hexcnt <= 16; k++, hexcnt++) {

			*hp++ = seziffer[ip[k] >> 4 & 0x0f];
			*hp++ = seziffer[ip[k] & 0x0f];

			if (ip[k] < 0x7f  &&  ip[k] > 0x1f)
				*pp++ = ip[k];
			else  
				*pp++ = ' ';

			if (hexcnt % 4 == 0)
				*hp++ = ' ';
			if (hexcnt % 8 == 0)
				*hp++ = ' ';
		}

		while (hexcnt <= 16) {
			*hp++ = ' ';
			*hp++ = ' ';
			*pp++ = ' ';
			if (hexcnt % 4 == 0)
				*hp++ = ' ';
			if (hexcnt % 8 == 0)
				*hp++ = ' ';
			hexcnt++;
		}

		/*
		 * line ready, next
		 */
		o += 16;
		l -= k;
		ip += k;
		k = memcmp(&prev[DW1], &curr[DW1], CMPLEN);
		if (k != 0) {
			(*func)(curr);
			hp = prev;
			prev = curr;
			curr = hp;
			prsame = FALSE;
		} else {
			if (!prsame) {
				(*func)(same);
				prsame = TRUE;
			}
			hp = curr;
		}
	}
}
