/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: crc.c,v $
 * Revision 1.1.6.2  1996/02/17  23:23:59  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:42:24  marty]
 *
 * Revision 1.1.6.1  1995/12/11  20:15:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:48:25  root]
 * 
 * Revision 1.1.4.2  1993/01/12  00:03:17  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:26:12  bbelch]
 * 
 * Revision 1.1.2.2  1992/05/01  15:42:18  rsalz
 * 	Part of rpc6 code drop.
 * 	[1992/05/01  01:24:35  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:41:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      crc.c 
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  CRC computation routines.
**
**
*/

#include <v2test.h>

/*****************************************************************************/ 

crc_bits crc_table[256];

/*****************************************************************************/ 

/*
 * Note that the msb of the "polynomial" is the least significant coefficient,
 * and vice versa.
 */
#ifdef CRC_16
/*
 * Coefficients of the CRC-CCITT polynomial, (x**16) + x**12 + x**5 + 1
 */
crc_bits gpoly = 0x8408;
#else
/*
 * Coefficients of the AUTODIN-II polynomial,
 * x**32 + x**26 + x**23 + x**22 + x**16 + x**12 + x**11 + x**10 +
 * x**8 + x**7 + x**5 + x**4 + x**2 + x**1 + 1
 */
crc_bits gpoly = 0xedb88320; 
#endif
	
crc_bits g[8];

/*****************************************************************************/ 

void crc_init()
{
	int i, m;

	g[0] = gpoly;
	
#ifdef CRC_DEBUG
	printf("G table:\n");
	printbin(gpoly);
	putchar('\n');
#endif
	
	/* Compute g[i] */
	for (i=1; i < 8; i++) {
		g[i] = g[i-1]>>1 ^ gpoly * (g[i-1] & 1);
#ifdef CRC_DEBUG
		printbin(g[i]);	putchar('\n');
#endif
	}
	/* compute CRC table. */
	for (i=0; i < 256; i++) {
		int t = 0;
		for (m = 7; m >=0; m--) {
			if (i & (1<<m)) {
				t ^= g[7-m];
			}
		}
#ifdef CRC_DEBUG
		printf("crc_table[%#x]=%#x (", i, t);
		printbin(t);
		printf(")\n");
#endif
		crc_table[i] = t;
	}
}

/*****************************************************************************/ 

crc_bits crc(buf, len)
	unsigned char *buf;
	int len;
{
	register crc_bits crc = 0;

	for ( ; len ; --len) {
		unsigned char t = (*buf++) ^ crc;
		crc >>= 8;
		crc ^= crc_table[t];
	}
	return crc;
}


