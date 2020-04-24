/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 *  bos_hostutils.h -- prototypes for functions useful for dealing with host
 *  names and addresses
 *
 *
 *	Copyright (C) 1995, 1990 Transarc Corporation
 *	All rights reserved.
 */
/*
 * $Header: /afs/tr/project/fs/dev/dfs-dev/src/file/userInt/bos/RCS/bos_hostutils.h,v 4.9 1995/07/26 16:09:27 blake Exp $
 */
#ifndef TRANSARC_BOS_HOSTUTIL_H
#define TRANSARC_BOS_HOSTUTIL_H

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#include <dcedfs/osi_net.h>

extern char *hostutil_NetAddrStr(char *hostStr);

#endif /* TRANSARC_BOS_HOSTUTIL_H */
