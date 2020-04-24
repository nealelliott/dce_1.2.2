/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: name.h,v $
 * Revision 1.1.11.1  1996/10/02  17:27:21  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:33:56  damon]
 *
 * Revision 1.1.6.1  1994/06/09  14:03:00  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:38:02  annie]
 * 
 * Revision 1.1.4.3  1993/01/19  15:33:01  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  13:55:01  cjd]
 * 
 * Revision 1.1.4.2  1992/08/31  20:10:36  jaffe
 * 	Fixed missing Header
 * 	[1992/08/30  14:20:14  jaffe]
 * 
 * Revision 1.1.2.2  1992/05/05  18:31:35  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/04  22:50:10  jdp]
 * 
 * Revision 1.1  1992/01/19  02:38:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/* name.h -- includes definition needed by dir scan phase of the salvager. */

/* Copyright (C) 1991, 1990 Transarc Corporation - All rights reserved */

/* $Header: /u0/rcs_trees/dce/rcs/file/episode/salvage/name.h,v 1.1.11.1 1996/10/02 17:27:21 damon Exp $ */

#ifndef	TRANSARC_SAL_NAME_H
#define	TRANSARC_SAL_NAME_H

struct	epis_path {
    struct epis_path * prev;
    char *	       name;
    char *	       pathname;	/* pathname of this entry, if any */
    int		       refCount;	/* pathname users */
};

/* epis_ToIndex -- provides the epia-style index corresponding to a fid. */

#define epis_ToIndex(fid,ss) epiz_ToEPIAIndex(epid_ToEPIVIndex(fid,ss->device))
#define EPIS_FIRSTINDEX 2
#define epis_MakeIndexOnly(fid,i,ss) \
    epid_Make(fid,ss->device,epiz_ToEPIVIndex(i),EPID_DONTCARE_UNIQUE)

/* SHARED */
struct epis_path * epis_AllocNameElem _TAKES((
  IN char * elem,
  IN struct epis_path * path));

/* SHARED */
void epis_FreeNameElem _TAKES((
  IN struct epis_path * path));

/* SHARED */
char * epis_MakeNamePath _TAKES((
  IN struct epis_path * path));

/* SHARED */
void epis_ReleaseNamePath _TAKES((
  IN struct epis_path * path));

/* SHARED */
long epis_DecorateNames _TAKES((
  IN u_long anodeIx,
  IN u_long volIx,
  IN struct epis_path * path));

#endif	/* TRANSARC_SAL_NAME_H */
