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
 * $Log: exp_win.h,v $
 * Revision 1.1.2.1  1996/08/09  19:15:53  arvind
 * 	DCE 1.2.2 intermediate drop (post bl3/pre bl4)
 * 	[1996/08/09  18:59:25  arvind]
 *
 * $EndLog$
 */

/* exp_win.h - window support

Written by: Don Libes, NIST, 10/25/93

This file is in the public domain.  However, the author and NIST
would appreciate credit if you use this file or parts of it.
*/

int exp_window_size_set();
int exp_window_size_get();

void exp_win_rows_set();
void exp_win_rows_get();
void exp_win_columns_set();
void exp_win_columns_get();

void exp_win2_rows_set();
void exp_win2_rows_get();
void exp_win2_columns_set();
void exp_win2_columns_get();
