/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/* Copyright (C) 1996, 1995 Transarc Corporation - All rights reserved */
/*
 * HISTORY
 * $Log: as.h,v $
 * Revision 1.1.4.1  1996/10/02  17:50:09  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:25  damon]
 *
 * Revision 1.1.2.2  1994/08/08  18:54:38  mckeen
 * 	Change prototype for as_init
 * 	[1994/08/08  17:47:03  mckeen]
 * 
 * Revision 1.1.2.1  1994/07/26  19:13:18  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:57:55  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:05:37  mckeen]
 * 
 * $EndLog$
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/gateway/dfsgwd/as.h,v 1.1.4.1 1996/10/02 17:50:09 damon Exp $ */
/*
 * as.h - Header file for authentication service.
 */

#ifndef _AS_H_
#define _AS_H_

int as_init
#ifdef __STDC__
  	(char *service, char *server);
#else
	();
#endif /* __STDC__ */

int as();

#endif /* _AS_H_ */
