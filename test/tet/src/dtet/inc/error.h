/*
 *      SCCS:  @(#)error.h	1.2 (92/09/30) 
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

SCCS:   	@(#)error.h	1.2 92/09/30 DTET release 1.0b
NAME:		error.h
PRODUCT:	DTET (Distributed Test Environment Toolkit)
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	error message printing macros
	avoids cluttering source files with long function call lines

	each .c file that includes this file needs a line of the form

		static char srcFile[] = __FILE__;

	the file containing main() should contain pointers initialised to
	the Error and Fatal routines to use
	the functions generror() and genfatal() are provided in libdtet.a and
	are sufficient for use where no special error processing is required

MODIFICATIONS:

************************************************************************/


extern void (*Error)();
extern void (*Fatal)();

#define error(errnum, s1, s2)	(*Error)(errnum, srcFile, __LINE__, s1, s2)
#define fatal(errnum, s1, s2)	(*Fatal)(errnum, srcFile, __LINE__, s1, s2)

#ifndef NEEDsrcFile
#define NEEDsrcFile
#endif

/* test an assertion, exit if it fails */
extern char Assertmsg[];
#ifdef __STDC__
/* assume cpp understands #x whatever the value of __STDC__ */
#define ASSERT(x)	if (!(x)) fatal(0, Assertmsg, #x); else
#else
#define ASSERT(x)	if (!(x)) fatal(0, Assertmsg, "x"); else
#endif

