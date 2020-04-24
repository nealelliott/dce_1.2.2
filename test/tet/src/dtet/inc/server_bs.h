/*
 *      SCCS:  @(#)server_bs.h	1.2 (92/09/30) 
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

SCCS:   	@(#)server_bs.h	1.2 92/09/30 DTET release 1.0b
NAME:		server_bs.h
PRODUCT:	DTET (Distributed Test Environment Toolkit)
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	declarations of bytestream-related server-specific functions that may
	be called by library routines

MODIFICATIONS:

************************************************************************/


#if __STDC__

extern int ss_bs2md(char *, struct ptab *);
extern int ss_md2bs(struct ptab *, char **, int *, int);

#else /* __STDC__ */

extern int ss_bs2md();
extern int ss_md2bs();

#endif /* __STDC__ */

