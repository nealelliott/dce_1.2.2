/*
 *      SCCS:  @(#)ltoa.h	1.2 (92/09/30) 
 *
 *	UniSoft Ltd., London, England
 *
 * (C) Copyright 1992 X/Open Company Limited
 *
 * All rights reserved.  No part of this source code may be reproduced,
 * stored in a retrieval system, or transmitted, in any form or by any
 * means, electronic, mechanical, photocopying, recording or otherwise,
 * except as stated in the end-user licence agreement, without the prior
 * permission of the copyright owners.
 *
 * X/Open and the 'X' symbol are trademarks of X/Open Company Limited in
 * the UK and other countries.
 */

/************************************************************************

SCCS:   	@(#)ltoa.h	1.2 92/09/30 DTET release 1.0b
NAME:		ltoa.h
PRODUCT:	DTET (Distributed Test Environment Toolkit)
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	a header file for use with the num-to-ascii conversion functions

MODIFICATIONS:

************************************************************************/


/* avoid conflict with the HP/UX library routine */
#ifdef __hpux
#define ltoa	dtltoa
#endif


#define itoa(x)	(ltoa((long) (x)))	/* convert int to ascii */
#define LNUMSZ	12			/* length of largest decimal long:
					   sign, 10 digits + null */

#define itoo(x)	(ltoo((long) (x)))	/* convert int to octal ascii */
#define LONUMSZ	13			/* length of largest octal long:
					   "O", 11 digits + null */

#define itox(x)	(ltox((long) (x)))	/* convert int to hex ascii */
#define LXNUMSZ	11			/* length of largest hex long:
					   "Ox", 8 digits + null */

#define NLBUF	5			/* no of times each function may be
					   called before re-using static
					   buffers */


/* extern function declarations */

#if __STDC__

extern char *ltoa(long);
extern char *ltoo(long);
extern char *ltox(long);

#else /* __STDC__ */

extern char *ltoa();
extern char *ltoo();
extern char *ltox();

#endif /* __STDC__ */

