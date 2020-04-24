/*
 *      SCCS:  @(#)server_in.h	1.2 (92/09/30) 
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

SCCS:   	@(#)server_in.h	1.2 92/09/30 DTET release 1.0b
NAME:		server_in.h
PRODUCT:	DTET (Distributed Test Environment Toolkit)
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	declarations of server-specific functions that may be called by
	inetlib routines

MODIFICATIONS:

************************************************************************/


#if __STDC__

extern void ss_tsaccept();
extern int ss_tsafteraccept(struct ptab *);
extern int ss_tsconnect(struct ptab *);

#else /* __STDC__ */

extern void ss_tsaccept();
extern int ss_tsafteraccept();
extern int ss_tsconnect();

#endif /* __STDC__ */

