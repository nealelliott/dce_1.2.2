/* Copyright (C) 1996 Transarc Corporation - All rights reserved. */
/* $Header: /u0/rcs_trees/dce/rcs/file/util/hyper.h,v 1.1.2.1 1996/10/02 21:13:17 damon Exp $ */

#ifndef TRANSARC_UTIL_HYPER_H
#define TRANSARC_UTIL_HYPER_H

int dfsh_StrToHyper(const char *numString, afs_hyper_t *hyperP, char **cp);
char *dfsh_HyperToStr(afs_hyper_t *h, char *s);

/* When you absolutely have to have the bits of a hyper laid out they way they
 * used to be, use a dfsh_diskHyper_t.  Used to represent a hyper on disk or
 * tape.  It only needs to be 32-bit aligned and isn't affected by the
 * alignment requirements of the 64-bit scalar type. */

typedef struct {
    u_int32 dh_high;
    u_int32 dh_low;
} dfsh_diskHyper_t;

/* To convert back and forth two sets of to/from hyper macros are provided, the
 * first set preserves host order, this is used by Episode.  The second set
 * uses ntohl/htonl on the halves and is suitable when architecture neutrality
 * is needed: ubik databases and tapes. */

#define DFSH_MemFromDiskHyper(h,dh) AFS_hset64(h, (dh).dh_high, (dh).dh_low)
#define DFSH_DiskFromMemHyper(dh,h) \
    ((dh).dh_high = AFS_hgethi(h), (dh).dh_low = AFS_hgetlo(h))

#define DFSH_MemFromNetHyper(h,dh) \
    AFS_hset64(h, ntohl((dh).dh_high), ntohl((dh).dh_low))
#define DFSH_NetFromMemHyper(dh,h) \
    ((dh).dh_high = htonl(AFS_hgethi(h)), (dh).dh_low = htonl(AFS_hgetlo(h)))

#endif
