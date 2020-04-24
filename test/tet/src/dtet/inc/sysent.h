/*
 *      SCCS:  @(#)sysent.h	1.2 (92/09/30) 
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

SCCS:   	@(#)sysent.h	1.2 92/09/30 DTET release 1.0b
NAME:		sysent.h
PRODUCT:	DTET (Distributed Test Environment Toolkit)
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	a header file for use with the systems file access functions

MODIFICATIONS:

************************************************************************/

/*
**	DTET systems are known internally by (numeric) sysid
**	a sysid is mapped to a system (or host) name by an entry in the
**	systems file
**	a system name may be used by transport-specific routines to
**	determine how to connect to that system; however, the systems file
**	itself does not contain transport-specific information
*/

/* structure of a systems table entry */
struct sysent {
	int sy_sysid;		/* system id */
	char *sy_name;		/* system name */
};


/* extern function declarations */

#if __STDC__

extern void endsysent();
extern struct sysent *getsysbyid(int);
extern struct sysent *getsysent();
extern int setsysent();

#else /* __STDC__ */

extern void endsysent();
extern struct sysent *getsysbyid();
extern struct sysent *getsysent();
extern int setsysent();

#endif /* __STDC__ */

