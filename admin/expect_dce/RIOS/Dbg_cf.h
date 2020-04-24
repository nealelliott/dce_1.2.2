/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: Dbg_cf.h,v $
 * Revision 1.1.2.2  1996/10/03  17:54:39  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  17:22:46  arvind]
 *
 * Revision 1.1.2.1  1996/08/09  19:18:04  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  19:01:28  arvind]
 * 
 * $EndLog$
 */
/* Dbg_cf.h.  Generated automatically by configure.  */
/* This file is only to be included by the Debugger itself. */
/* Applications should only include Dbg.h. */

/*
 * Check for headers
 */
#ifndef __NIST_DBG_CF_H__
#define __NIST_DBG_CF_H__

/* #undef NO_STDLIB_H */		/* Tcl requires this name */

/*
 * Check for functions
 */
#define HAVE_STRCHR 1

#ifndef HAVE_STRCHR
#define strchr(s,c) index(s,c)
#endif /* HAVE_STRCHR */

#endif	/* __NIST_DBG_CF_H__ */
