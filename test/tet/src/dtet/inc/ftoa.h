/*
 *      SCCS:  @(#)ftoa.h	1.2 (92/09/30) 
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

SCCS:   	@(#)ftoa.h	1.2 92/09/30 DTET release 1.0b
NAME:		ftoa.h
PRODUCT:	DTET (Distributed Test Environment Toolkit)
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	June 1992

DESCRIPTION:
	a header file for use with ftoa()

MODIFICATIONS:

************************************************************************/


/* flag names structure */
struct flags {
	int fl_value;		/* flag value */
	char *fl_name;		/* flag name */
};


/* extern function declarations */

#if __STDC__

extern char *ftoa(int, struct flags *, int);

#else /* __STDC__ */

extern char *ftoa();

#endif /* __STDC__ */

