/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rdconf_err_str.c,v $
 * Revision 1.1.6.2  1996/02/17  23:27:31  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:53  marty]
 *
 * Revision 1.1.6.1  1995/12/11  22:42:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:19:45  root]
 * 
 * Revision 1.1.4.1  1994/06/10  20:49:51  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:19:24  devsrc]
 * 
 * Revision 1.1.2.2  1993/03/12  04:26:23  mhickey
 * 	Final checkin for initial submit to 1.0.2 backing tree
 * 
 * 	Defect 5887.
 * 	[1993/03/11  21:38:46  mhickey]
 * 
 * $EndLog$
 */

/*---*
 *---* rdconf_err_str.c - contains an array of messages for return
 *---*                       values from rd_conf()
 *---*
 *---* ASSOCIATION
 *---*    This file is part of the DCE 1.0.2 System Tests
 *---*
 *---*    The DCE 1.0.2 system tests corresponds to the 
 *---*    DCE1.0.2 Core Component System Test plan.  For more information on 
 *---*    the operation and goals of the DCE 1.0.2 system tests, see the test 
 *---* 
 *---* FUNCTION
 *---*    Defines an array of error strings, each positionally corresponding
 *---*    to a return value from rd_conf()
 *---*
 *---* CALLED BY
 *---*    noone.
 *---* 
 *---* CALLS INTO 
 *---*    'C' library
 *---*
 *---* PREREQUISITES
 *---*   None.
 *---*/

/*---*
 *---* strings for rpcsec2_rd_conf()
 *---*/
char *rd_conf_errs[] = { 
                                 "Success from rpcsec2_rd_conf", 
                                 "Syntax error in configuration file",
                                 "Config file not found",
                                 "Could not find values for all specifiers",
                                 "Could not get space!"
			       };
