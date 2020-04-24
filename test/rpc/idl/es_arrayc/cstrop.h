/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cstrop.h,v $
 * Revision 1.1.4.2  1996/02/18  23:05:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:25:54  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:04:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:30:59  root]
 * 
 * Revision 1.1.2.2  1993/08/11  16:25:13  ganni
 * 	tests for new IDL functionality
 * 	[1993/08/11  16:13:47  ganni]
 * 
 * $EndLog$
 */
/*
*/

/* Make names consistent with FORTRAN on U*x systems */
#ifndef VMS
#define in_fn_sub in_fn_sub_
#define out_fn_sub out_fn_sub_
#define in_cfmt_sub in_cfmt_sub_
#define out_cfmt_sub out_cfmt_sub_
#define in_open_sub in_open_sub_
#define out_open_sub out_open_sub_
#endif
