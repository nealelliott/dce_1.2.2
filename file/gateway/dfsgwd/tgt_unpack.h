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
 * $Log: tgt_unpack.h,v $
 * Revision 1.1.4.1  1996/10/02  17:50:20  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:35  damon]
 *
 * Revision 1.1.2.1  1994/07/26  19:13:30  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:58:03  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:05:43  mckeen]
 * 
 * $EndLog$
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/gateway/dfsgwd/tgt_unpack.h,v 1.1.4.1 1996/10/02 17:50:20 damon Exp $ */
/*
 * tgt_unpack.h - Prototypes for tgt_unpack.c
 */
int tgt_unpack
#ifdef __STDC__
	(char *packet, krb5_creds *tgt, int swap);
#else
	();
#endif /* __STDC__ */
