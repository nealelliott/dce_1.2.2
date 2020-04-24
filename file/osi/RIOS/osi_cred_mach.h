/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/osi/RIOS/RCS/osi_cred_mach.h,v 1.17 1995/08/02 15:56:33 blake Exp $
 */
/*
 *      Copyright (C) 1992, 1995 Transarc Corporation
 *      All rights reserved.
 */

#ifndef TRANSARC_OSI_CRED_MACH_H
#define	TRANSARC_OSI_CRED_MACH_H

#include <sys/param.h>
#include <sys/cred.h>

typedef struct ucred osi_cred_t;

#define OSI_MAXGROUPS	NGROUPS
#define OSI_NOGROUP	(-1)

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
#define OSI_HAS_CR_PAG		0

#ifdef AFS_AIX32_ENV
#undef crhold
#define crhold(cr) { \
	extern lock_t cred_lock; \
	int  _crhold_waslocked; \
	_crhold_waslocked = lockl(&cred_lock, LOCK_SHORT); \
	(cr)->cr_ref++; \
	if (_crhold_waslocked != LOCK_NEST) \
	    unlockl(&cred_lock); \
	}
#endif /* AFS_AIX32_ENV */

#define osi_SetNGroups(c, n)	((c)->cr_ngrps = (n))
#define osi_GetNGroups(c)	((c)->cr_ngrps)

#define osi_SetUID(c, u)	((c)->cr_uid = (u))
#define osi_GetUID(c)		((c)->cr_uid)

#define osi_GetRUID(c)		((c)->cr_ruid)

#define osi_SetGID(c, g)	((c)->cr_gid = (g))
#define osi_GetGID(c)		((c)->cr_gid)

#ifdef KERNEL
#define osi_pcred_lock(p)
#define osi_pcred_unlock(p)
#define osi_set_thread_creds(p, cr)
extern osi_cred_t *osi_aix_getucred(void);
extern void osi_aix_setucred(osi_cred_t *);
extern int osi_crequal(osi_cred_t *cred1p, osi_cred_t *cred2p);
#define osi_getucred()	osi_aix_getucred()
#define osi_setucred(c)	osi_aix_setucred(c)
#else
#define osi_getucred()	u.u_cred
#define osi_setucred(c)	u.u_cred = c
#endif /* KERNEL */


#endif /* TRANSARC_OSI_CRED_MACH_H */
