/*
 * (C) Copyright 1992 Transarc Corporation.
 * All Rights Reserved.
 */
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: openFlags.h,v $
 * Revision 1.1.8.1  1996/10/17  18:18:03  damon
 * 	Submitting on behalf of Transarc
 * 	[1996/10/17  16:49:54  damon]
 *
 * Revision 1.1.2.3  1993/01/11  18:25:26  htf
 * 	Embedded copyright notice
 * 	[1993/01/11  16:57:18  htf]
 * 
 * Revision 1.1.2.2  1992/11/04  19:22:06  jaffe
 * 	Transarc delta: fred-cc-fix-socket-port 1.3
 * 	  Selected comments:
 * 	    Additional fixes were necessary.  A couple calls to htons were added and
 * 	    also normalized the optn() flags.  There values are different on AIX and
 * 	    OSF/1.
 * 	    Forgot to include new .h file.
 * 	[1992/11/04  17:37:27  jaffe]
 * 
 * $EndLog$
 */
/*
 * openFlags.h -- 
 *    Maps open flags between the various platforms.
 * 
 * $Header: /u0/rcs_trees/dce/rcs/test/file/cache_mgr/openFlags.h,v 1.1.8.1 1996/10/17 18:18:03 damon Exp $
 */
#ifndef OPEN_FLAGS_H
#define OPEN_FLAGS_H

#define WLR_O_CREAT    0x0001
#define WLR_O_RDONLY   0x0002
#define WLR_O_RDWR     0x0004
#define WLR_O_TRUNC    0x0008

#endif  /* OPEN_FLAGS_H */
