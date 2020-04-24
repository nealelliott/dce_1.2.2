/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 *      Copyright (C) 1995, 1992 Transarc Corporation
 *      All rights reserved.
 */
/*
 * $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/osi/OSF1/RCS/osi_cred_mach.h,v 1.9 1995/07/18 22:58:12 blake Exp $
 */
#ifndef TRANSARC_OSI_CRED_MACH_H
#define	TRANSARC_OSI_CRED_MACH_H

#include <sys/param.h>
#include <sys/cred.h>

typedef struct ucred osi_cred_t;

#define OSI_MAXGROUPS	NGROUPS
#define OSI_NOGROUP	NOGROUP

/*
 * Set true if the cred struct has a member containing the length of the
 * cr_groups list; otherwise, the list is assumed to be terminated by
 * the special value OSI_NOGROUP.
 */
#define OSI_CR_GROUPS_COUNTED	1

/*
 * Set true if the cred struct has a member that records the PAG; if
 * false, the PAG is stored in the first two cr_groups entries.
 */
#define OSI_HAS_CR_PAG		1

#define osi_SetNGroups(c,n) ((c)->cr_ngroups = (n))
#define osi_GetNGroups(c) ((c)->cr_ngroups)

#define osi_SetUID(c,u)     ((c)->cr_uid = (u))
#define osi_GetUID(c)     ((c)->cr_uid)

#define osi_GetRUID(c)     ((c)->cr_ruid)

#define osi_SetGID(c,g)     ((c)->cr_gid = (g))
#define osi_GetGID(c)     ((c)->cr_gid)

#ifdef KERNEL
#define osi_pcred_lock(p)
#define osi_pcred_unlock(p)
#define osi_getucred()	u.u_cred
#define osi_setucred(c)	\
    substitute_real_creds(u.u_procp, NOUID, NOUID, NOGID, NOGID, c)
#define osi_set_thread_creds(p, cr)
#endif /* KERNEL */

#endif /* TRANSARC_OSI_CRED_MACH_H */
