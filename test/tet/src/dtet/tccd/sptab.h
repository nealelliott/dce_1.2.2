/*
 *      SCCS:  @(#)sptab.h	1.2 (92/09/30) 
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

SCCS:   	@(#)sptab.h	1.2 92/09/30
NAME:		sptab.h
PRODUCT:	DTET (Distributed Test Environment Toolkit) release 1.0b
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	tccd server-specific process table data description

MODIFICATIONS:

************************************************************************/

/* tccd server-specific process table data - pointed to by ptab.pt_sdata */
struct sptab {
	int *sp_snames;		/* ptr to remote systems list */
	int sp_nsname;		/* number of remote systems */
	int sp_snlen;		/* # of bytes pointed to by sp_snames */
};

