/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: flserver.p.h,v $
 * Revision 1.1.59.1  1996/10/02  17:46:19  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:37:08  damon]
 *
 * $EndLog$
 */

/* Copyright (C) 1996, 1991 Transarc Corporation - All rights reserved */
/* $Header: /u0/rcs_trees/dce/rcs/file/flserver/flserver.p.h,v 1.1.59.1 1996/10/02 17:46:19 damon Exp $ */

#ifndef	_FLSERVER_H_
#define	_FLSERVER_H_

#ifndef VLF_RWEXISTS
/* get vldb includes if not basically already provided */
#ifdef KERNEL
#include <dcedfs/afsvl_proc.h>
#else /* KERNEL */
#include <dcedfs/fldb_proc.h>
#endif /* KERNEL */
#endif

#include <dcedfs/fldb_data.h>

/* Order of entries in the volumeid[] array */
#define	RWVOL		0
#define	ROVOL		1
#define	BACKVOL		2
#define	VOLTIX_TO_VOLTYPE(vix)	((vix) << 16)

/* Vlentry's flags also used to stamp type of ongoing operation */
#define	VLOP_MOVE	0x10
#define	VLOP_RELEASE	0x20
#define	VLOP_BACKUP	0x40
#define	VLOP_DELETE	0x80
#define	VLOP_DUMP	0x100
#define	VLOP_RESTORE	0x200
#define	VLOP_ADDSITE	0x400
#define	VLOP_ALLOPERS	(VLOP_MOVE | VLOP_RELEASE | VLOP_BACKUP | VLOP_DELETE \
				   | VLOP_DUMP | VLOP_RESTORE | VLOP_ADDSITE)

/* Valid RelaseLock types */
#define	LOCKREL_TIMESTAMP   1
#define	LOCKREL_OPCODE	    2
#define	LOCKREL_AFSID	    4
#define	LOCKREL_ALL	(LOCKREL_TIMESTAMP | LOCKREL_OPCODE | LOCKREL_AFSID)

#define	MAXBUMPCOUNT	100000	/* Keep max bump count low for unauth access */

#endif	/* _FLSERVER_H_ */
