/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: osi_cred_mach.h,v $
 * Revision 1.1.47.1  1996/10/02  17:56:58  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:16  damon]
 *
 * Revision 1.1.42.1  1994/06/09  14:14:42  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:27:26  annie]
 * 
 * Revision 1.1.2.4  1993/01/21  14:50:00  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  14:51:46  cjd]
 * 
 * Revision 1.1.2.3  1992/09/15  20:31:25  jaffe
 * 	sync with Transarc
 * 	[92/09/15            jaffe]
 * 
 * Revision 1.1.2.2  1992/09/15  13:15:34  jaffe
 * 	Transarc delta: jaffe-ot4608-ucred-cleanup 1.3
 * 	  Selected comments:
 * 	    use osi macros for ucred access.  New macros are osi_GetNGroups,
 * 	    osi_SetNGroups, osi_GetUID, osi_GetRUID, and osi_GetGID.  New
 * 	    constants are OSI_NGROUPS, OSI_NOGROUP, and OSI_MAXGROUPS.
 * 	    add initial start at this file.  There is still more work to go before
 * 	    this file will compile.
 * 	    include osi_cred.h for appropriate definitions.
 * 	    fixed typo. GetGID takes only one argument.
 * 	[1992/09/14  20:14:39  jaffe]
 * 
 * $EndLog$
 */
/*
 *      Copyright (C) 1992 Transarc Corporation
 *      All rights reserved.
 */

#ifndef TRANSARC_OSI_CRED_MACH_H
#define	TRANSARC_OSI_CRED_MACH_H

#define osi_SetNGroups(c,n) MACRO_BEGIN \
    int i; \
    struct ucred *cred = (c); \
    for (i = (n); i < NGROUPS; i++) cred->cr_groups[i] = NOGROUP; \
MACRO_END
#error "Must define a function for GetNGroups, since it may be used inline"
#define osi_GetNGroups(c) ((c)->cr_ngroups)

#define osi_SetUID(c,u)     ((c)->cr_uid = u)
#define osi_GetUID(c)     ((c)->cr_uid)

#define osi_GetRUID(c)     ((c)->cr_ruid)

#define osi_SetGID(c,g)     ((c)->cr_gid = g)
#define osi_GetGID(c)     ((c)->cr_gid)
/*
 * Number of available group entries in the cred structure. 
 * NB: PAG is stored in the "pag" field of the cred structure.
 */
#define OSI_AVAILGROUPS (NGROUPS)
#define OSI_MAXGROUPS (NGROUPS)
#define OSI_NOGROUP NOGROUP
#error "need to define osi_getucred and osi_setucred"

#endif /* TRANSARC_OSI_CRED_MACH_H */
