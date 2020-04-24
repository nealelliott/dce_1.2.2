/*
 *      SCCS:  @(#)inetlib_in.h	1.2 (92/09/30) 
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

SCCS:   	@(#)inetlib_in.h	1.2 92/09/30 DTET release 1.0b
NAME:		inetlib_in.h
PRODUCT:	DTET (Distributed Test Environment Toolkit)
AUTHOR:		Andrew Dingwall, UniSoft Ltd.
DATE CREATED:	April 1992

DESCRIPTION:
	declarations for extern inetlib functions not declared in other
	header files

	these functions are inet-specific; the transport-independent
	interfaces to the transport-specific library are declared in
	tslib.h

MODIFICATIONS:

************************************************************************/


#if __STDC__

extern struct in_addr *gethostaddr(char *);
extern struct in_addr *getlocalhostaddr();
extern int gettccdaddr(struct ptab *);
extern int gettccdport();
extern void ts_accept(int);
extern void ts_listen(int);
extern int ts_nbio(struct ptab *);

#else /* __STDC__ */

extern struct in_addr *gethostaddr();
extern struct in_addr *getlocalhostaddr();
extern int gettccdaddr();
extern int gettccdport();
extern void ts_accept();
extern void ts_listen();
extern int ts_nbio();

#endif /* __STDC__ */

