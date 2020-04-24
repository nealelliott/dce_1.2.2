/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: critical.h,v $
 * Revision 1.1.5.1  1996/10/02  17:28:57  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:34:48  damon]
 *
 * $EndLog$
 */
/* This file defines macros that manage critical regions. */

/* $Header: /u0/rcs_trees/dce/rcs/file/episode/tools/critical.h,v 1.1.5.1 1996/10/02 17:28:57 damon Exp $ */

/* Copyright (C) 1994, 1990 Transarc Corporation - All rights reserved */

#ifndef TRANSARC_CRITICAL_H
#define TRANSARC_CRITICAL_H

/*
 * Macros for handling critical regions.  A critical region is introduced by
 * the BEGIN_CRIT macro which takes two statements as parameters.  The first is
 * executed before beginning and if it sets code to a non-zero value the region
 * is never entered.  If an error is detected within the critical region
 * premature exit should be effected using ABORT_CRIT or EXIT_CRIT.  The second
 * statement is executed on exit, premature or otherwise.  It may set code, but
 * if code is already non-zero, this value is preserved.  Following the
 * critical region, a non-zero value of code indicates the first error that
 * occurred.
 *
 * The critical region itself should consist of one or more statements.  The
 * region is terminated by an END_CRIT, which does not require a semi-colon.
 * The variable "code" is assumed to be declared as a long and is initialized
 * to zero at the beginning of the critical region.
 *
 * The BEGIN_LCRIT and BEGIN_LLCRIT macros are the same as BEGIN_CRIT except
 * that they allow for the declaration of variables local to the critical
 * region.
 */

#define BEGIN_LLCRIT(d1,d2,a,b)					\
{   long _code = 0; int _once = 0; d1;d2; code = 0;		\
    a;								\
    if (code == 0)	/* if "a" fails don't enter crit. */	\
        while (/*CONSTCOND*/1) {				\
	    if (_once > 0) { _code = code; b; break; }		\
	    _once = 1;



 /* critical region goes here */

#define END_CRIT						\
    } /* break, continue and normal exit get us here */		\
    if (_code) code = _code;	/* restore code */		\
}

#define BEGIN_CRIT(a,b) BEGIN_LLCRIT(0,0,a,b)
#define BEGIN_LCRIT(d,a,b) BEGIN_LLCRIT(d,0,a,b)

/*
 * This provides a convenient way to exit a critical region with a specific
 * code value.  It is not needed if the variable "code" is already set
 * correctly.  It can only be used where multiple statements are allowed
 * and so it should usually be surrounded with braces.  Also, since a critical
 * region is exited using "continue", this macro cannot be used within a loop.
 */

#define ABORT_CRIT(c) code = (c); continue;

/*
 * Exit a critical region, with the variable "code" already set correctly.
 * This is an expression and should NOT be surrounded with braces.
 */

#define EXIT_CRIT continue

/*
 * Macros for handling locked regions.  These are analogous to the critical
 * region macros, but specialized for the case in which the entry and exit
 * statements are simple obtains and releases of locks.  Locked regions in
 * which the entry and exit statements do not fit this paradigm can use the
 * more general CRIT macros above.  A locked region should be ended with
 * END_LOCK.
 *
 * Users of tlocks will find analogous macros defined in ../anode/tlock.h.
 *
 * The caller can use macros ABORT_CRIT and EXIT_CRIT.  In the case of
 * ABORT_CRIT, however, the BEGIN_LOCK macro does not initialize variable
 * "code" or even assume that it exists.  Thus, the caller must take care of
 * initializing it.
 */

#define BEGIN_LOCK(a,b)			\
{   int _once = 0;			\
    a;					\
    while (/*CONSTCOND*/1) {		\
	if (_once > 0) { b; break; }	\
	_once = 1;

#define END_LOCK						\
    } /* break, continue, and normal exit get us here */	\
}

#define BEGIN_LOCK_READ(lock)		\
    BEGIN_LOCK (lock_ObtainRead_r (lock),	\
		lock_ReleaseRead_r (lock))

#define BEGIN_LOCK_WRITE(lock)			\
    BEGIN_LOCK (lock_ObtainWrite_r (lock),	\
		lock_ReleaseWrite_r (lock))

#define BEGIN_MUTEX(lock)		\
    BEGIN_LOCK (epi_mutex_lock (lock),	\
		epi_mutex_unlock (lock))

#endif
