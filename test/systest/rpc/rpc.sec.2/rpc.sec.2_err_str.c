/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpc.sec.2_err_str.c,v $
 * Revision 1.1.76.2  1996/02/17  23:28:11  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:17  marty]
 *
 * Revision 1.1.76.1  1995/12/11  22:43:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:20:39  root]
 * 
 * Revision 1.1.74.1  1993/09/09  19:12:52  cmckeen
 * 	 HP's TET'ized version
 * 	[1993/09/09  19:11:49  cmckeen]
 * 
 * Revision 1.1.6.2  1993/08/16  14:30:06  eheller
 * 	File tetified.
 * 	[1993/07/23  17:44:21  eheller]
 * 
 * Revision 1.1.2.6  1993/03/12  19:13:59  mhickey
 * 	Fixes for submit problems
 * 	[1993/03/12  19:13:24  mhickey]
 * 
 * Revision 1.1.2.5  1993/03/12  04:07:12  mhickey
 * 	Final checkin for comment fixes before final submit
 * 	[1993/03/12  04:00:14  mhickey]
 * 
 * Revision 1.1.2.4  1993/02/25  20:16:22  mhickey
 * 	Fixing bcs problem
 * 	[1993/02/25  20:02:43  mhickey]
 * 
 * Revision 1.1.2.3  1993/02/05  15:22:41  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:43:42  cjd]
 * 
 * Revision 1.1.4.2  1993/02/04  22:12:37  cjd
 * 	Embedded copyright notice
 * 
 * Revision 1.1.2.2  1992/12/03  19:54:08  mhickey
 * 	Initial check-in.
 * 
 * 	Defect 5887.
 * 	[1992/12/03  17:55:16  mhickey]
 * 
 * $EndLog$
 */

/*---*
 *---* rpc.sec.2_err_str.c - contains an array of messages for return
 *---*                       values from various functions
 *---*
 *---* ASSOCIATION
 *---*    This file is part of the rpc.sec.2 DCE System Test
 *---*
 *---*    The rpc.sec.2 system test corresponds to test rpc.sec.2 in the 
 *---*    DCE1.0.2 Core Component System Test plan.  For more information on 
 *---*    the operation and goals of the rpc.sec.2 system test, see the test 
 *---*    plan, or the README file in this directory.
 *---* 
 *---* FUNCTION
 *---*    Defines strings corresponding to error return values from 
 *---*    server functions
 *---* 
 *---* CALLED BY
 *---*    rpc.sec.2_smain.c, 
 *---*    rpc.sec.2_cmain.c
 *---* 
 *---* CALLS INTO 
 *---*   Nothing.
 *---*
 *---* PREREQUISITES
 *---*   None.
 *---*/


/*---*
 *---* strings for rpcsec2_srv_check_auth - note for the convience of the
 *---*                                      callers we start this array at
 *---*                                      7.
 *---*/
char *rpcsec2_srv_check_auth_errs[] = { "", "", "",
                                        "", "", "", "",
                       "Successful authorization",
                       "Incorrect server principal in binding handle",
                       "Unsupported authorization service in binding handle",
                       "Unsupported authation service in binding handle",
                       "Incorrect client principal in binding handle",
                       "Unsupported authentication protection level",
                       "Can't get auth information from binding handle"
		                };

/*---*
 *---* strings for rpcsec2_cnf_str()
 *---*/
char *rpcsec2_cnf_str_errs[] = { "Success from rpcsec2_cnf_str",
                                 "Null array in passed struct",
                                 "Checksum error",
                                 "Passed structure pointer is null",
                                 "No process/thread id in passed structure",
                                 "No size element in passed structure",
                                 "Unspecified error in rpcsec2_cnf_str"
			       };

/*---*
 *---* strings for rpcsec2_nop()
 *---*/
char *rpcsec2_nop_errs[] = {
                              "Success from rpcsec2_nop",
                              "Passed value was odd",
                              "Other error in rpcsec2_nop"
			   };
