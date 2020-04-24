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
 * $Log: exp_rename.h,v $
 * Revision 1.1.2.1  1996/08/09  19:13:31  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  18:57:21  arvind]
 *
 * $EndLog$
 */

/* translate.h - preface globals that appear in the expect library
with "exp_" so we don't conflict with the user.  This saves me having
to use exp_XXX throughout the expect program itself, which was written
well before the library when I didn't have to worry about name conflicts.

Written by: Don Libes, NIST, 12/3/90

Design and implementation of this program was paid for by U.S. tax
dollars.  Therefore it is public domain.  However, the author and NIST
would appreciate credit if this program or parts of it are used.
*/

#define errorlog	exp_errorlog
#define debuglog	exp_debuglog
#define is_debugging	exp_is_debugging
#define logfile		exp_logfile
#define debugfile	exp_debugfile
#define loguser		exp_loguser
#define logfile_all	exp_logfile_all

#define getptymaster	exp_getptymaster
#define getptyslave	exp_getptyslave
