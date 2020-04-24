/*
 * Copyright (C) 1994 Transarc Corporation
 * All rights reserved.
 */
/* $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/episode/vnops/RCS/efs_tmpcopy.h,v 1.21 1994/08/04 14:34:04 cml Exp $ */

#ifndef TRANSARC_EFS_TMPCOPY_H
#define TRANSARC_EFS_TMPCOPY_H

#include <dcedfs/sysincludes.h>
#include <dcedfs/episode/logbuf.h>
#include "efs_evnode.h"

#define NEXT_BOUNDARY(x, size) (((x) + (size) - 1) & ~((size) - 1))
#define NEXT_PAGE(x) NEXT_BOUNDARY(x, PAGESIZE)
#define NEXT_FRAG(l, a) NEXT_BOUNDARY(l, a->device->fragSize)
#define NEXT_BLOCK(l, a) NEXT_BOUNDARY(l, a->device->blkSize)
#define PAGE_START(x) ((x) & ~(PAGESIZE - 1))

#define	DISK_SIZE(l, ap) \
    (((l) <= epix_GetMaxInlineData(ap)) ? (l) : \
     ((l) <= (ap)->device->blkSize - (ap)->device->fragSize) ? \
	NEXT_FRAG(l, ap) : NEXT_BLOCK(l, ap))

extern int efs_DiscardVM(
  struct evnode *evp,
  u_long oldLen,
  u_long newLen,
  osi_cred_t *credp);

/*
 * Solaris and AIX require have different interfaces to their VM systems
 * so the following will be used to avoid putting ifdefs all over the place
 * The user space code follows the AIX model.
 */
#if defined(AFS_SUNOS5_ENV) && defined(KERNEL)
#define TMPBUF_DECL page_t *plist;
#define EFS_GETCONTENTS(vp, off, len, rblk) \
    efs_GetContents(vp, off, len, rblk, &plist)
#define EFS_PUTCONTENTS(vp, wblk, len, justUnlock) \
    efs_PutContents(plist, justUnlock)
#define EFS_PUTCONTENTS_TO_INLINE(trans, vp, len) \
    efs_PutContentsToInline(trans, vp, plist, len)
#define EFS_HOLD_CONTENTS(vp, off, len) \
    efs_HoldContents(vp, off, len, &plist)
#define EFS_RELEASE_CONTENTS(vp) \
    efs_PutContents(plist, 1)
#define EFS_ZEROFILL_LAST_PAGE(evp, len) \
    efs_ZeroLastPage(evp, len);

extern int efs_GetContents(
  struct evnode *vp,
  long off,
  long len,
  daddr_t rblkno,
  page_t **pl);

extern void efs_PutContents(page_t *pl, int justUnlock);

extern void efs_PutContentsToInline(
  elbb_tranRec_t trans,
  struct evnode *vp,
  page_t *pp,
  long length);

extern void efs_HoldContents(
  struct evnode *evp,
  long off,
  long len,
  page_t **plist);

extern void efs_ZeroLastPage(struct evnode *evp, long len);

#elif defined(AFS_AIX_ENV) || !defined(KERNEL)

#define TMPBUF_DECL	char *mem = NULL; int memlen = 0;

#define EFS_GETCONTENTS(vp, off, len, rblk)   \
    efs_GetContents(vp, off, len, rblk, &mem, &memlen)
#define EFS_PUTCONTENTS(vp, wblk, len, justUnlock) \
    efs_PutContents(vp, wblk, len, justUnlock, mem, memlen)
#define EFS_PUTCONTENTS_TO_INLINE(trans, vp, len) \
    efs_PutContentsToInline(trans, vp, len, mem, memlen)
#define EFS_HOLD_CONTENTS(vp, off, len) /* NULL */
#define EFS_RELEASE_CONTENTS(vp) /* NULL */
#define EFS_ZEROFILL_LAST_PAGE(evp, len) \
    efs_ZeroLastBlock(evp, len)

extern int efs_GetContents(
  struct evnode *vp,
  long off,
  long len,
  daddr_t rblkno,
  char **mem,
  int *memlen);

extern void efs_PutContents(
  struct evnode *vp,
  daddr_t wblk,
  long len,
  int justUnlock,
  char *mem,
  int memlen);

extern void efs_PutContentsToInline(
  elbb_tranRec_t trans,
  struct evnode *vp,
  long length,
  char *mem,
  int memlen);

extern int efs_ZeroLastPage(struct evnode *evp, long len);

#endif /*!KERNEL || AFS_AIX_ENV*/

extern void efs_CreateDirtyZeroPages(
  struct evnode *evp,
  long start,
  long finish);

#endif /* TRANSARC_EFS_TMPCOPY_H */
