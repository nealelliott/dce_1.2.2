/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpc.cds.3_err_str.c,v $
 * Revision 1.1.6.2  1996/02/17  23:27:41  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:59  marty]
 *
 * Revision 1.1.6.1  1995/12/11  22:42:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:19:59  root]
 * 
 * Revision 1.1.4.1  1994/06/10  20:50:05  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:19:33  devsrc]
 * 
 * Revision 1.1.2.3  1993/03/12  18:56:04  mhickey
 * 	Fixing bsubmit mystery meat
 * 	[1993/03/12  18:51:21  mhickey]
 * 
 * Revision 1.1.2.2  1993/03/12  04:27:33  mhickey
 * 	Initial check in of the rpc.cds.3  system test.
 * 	[1993/01/29  19:58:02  mhickey]
 * 
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
char *rpccds3_srv_check_auth_errs[] = { "", "", "",
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
char *rpccds3_nop_errs[] = {
                              "Success from rpcsec2_nop",
                              "Passed value was odd",
                              "Other error in rpcsec2_nop"
			   };

char *rpccds3_parse_cal_spec_errs[] = {
                                       "Success from rpccds3_parse_cal_spec",
                                       "No calendar specification passed",
                                       "No space for data structure",
                                       "Bad calendar data file or filename",
                                       "Bad data in calendar data file",
                            "Unspecified error from rpccds3_parse_cal_spec."
				      };

char *rpccds3_set_cal_uuid_errs[] = { 
                                      "Success from rpccds3_set_cal_uuid",
                                      "Bad calendar name.",
                                      "Couldn't create uuid for calendar",
                                      "Couldn't get uuid from namespace",
                               "Unspecified error from rpccds3_set_cal_uuid."
			            };

char *rpccds3_send_cal_errs[] = { 
                                  "Success from rpccds3_send_cal",
                                  "Bad binding handle passed",
                                  "No uuid in binding handle",
                                  "Bad binding handle passed",
                                  "No calendar found for uuid",
                                  "Unspecified error from rpccds3_send_cal"
				};

 
                     

