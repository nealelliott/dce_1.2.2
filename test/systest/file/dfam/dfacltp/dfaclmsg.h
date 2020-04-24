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
 * $Log: dfaclmsg.h,v $
 * Revision 1.1.2.2  1996/03/11  14:11:07  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:59:23  marty]
 *
 * Revision 1.1.2.1  1996/02/29  04:20:16  takano
 * 	Submit DFAM-client test
 * 	[1996/02/29  04:19:26  takano]
 * 
 * Revision 1.1.2.1  1996/02/22  10:54:32  takano
 * 	First Submit
 * 	[1996/02/22  10:54:27  takano]
 * 
 * $EndLog$
 */
/*****************************************************************************/
/*  Test Suite Client Message File                                             */
/*				                                                             */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/

#define KDDS33001 "KDDS33001-I DFA test started.\n\n"
#define KDDS33002 "KDDS33002-I %s is tested.\n"
#define KDDS33003 "KDDS33003-I %s successfully ended.\n\n"
#define KDDS33004 "KDDS33004-I %s abnormally ended.\n\n"
#define KDDS33005 "KDDS33005-Q Quit further test? (Y/N) "
#define KDDS33006 "KDDS33006-I All the test cases (%d item(s)) successfully ended.\n\n"
#define KDDS33007 "KDDS33007-I Successfully ended : %d item(s) Abnormally ended : %d item(s)\n\n"
#define KDDS33008 "KDDS33008-I DFA test ended.\n"
#define KDDS33100 "KDDS33100-E Failed to log in to NetWare.\n" \
                  "            function=%s, API=%s, error code=%04X\n"
#define KDDS33101 "KDDS33101-E Failed to log in to DCE.\n" \
                  "            function=%s, API=%s, error code=%04X\n"
#define KDDS33102 "KDDS33102-E File-Access Volume Mapping failed.\n" \
                  "            function=%s, API=%s, error code=%04X\n"
#define KDDS33103 "KDDS33103-E Syntax error in Configuration File.\n" \
                  "            line=%d\n"
#define KDDS33104 "KDDS33104-E Syntax error in Test Item Catalog File.\n" \
                  "            line=%d\n"
#define KDDS33105 "KDDS33105-E Syntax error in Test Item File.\n" \
                  "            line=%d\n"
#define KDDS33106 "KDDS33106-E Syntax error in Base File.\n" \
                  "            line=%d\n"
#define KDDS33107 "KDDS33107-E Access error.\n" \
                  "            file=%s, function=%s, API=%s, error code=%04X\n"
#define KDDS33108 "KDDS33108-E Failed to log out from DCE.\n" \
                  "            function=%s, API=%s, error code=%04X\n"
#define KDDS33109 "KDDS33109-E Failed to log out from NetWare.\n" \
                  "            function=%s, API=%s, error code=%04X\n"
#define KDDS33110 "KDDS33110-E Communication error.\n" \
                  "            function=%s, API=%s, error code=%04X\n"
#define KDDS33111 "KDDS33111-E Internal error.\n" \
                  "            function=%s, API=%s, error code=%04X\n"
#define KDDS33112 "KDDS33112-E Shortage of memory.\n" \
                  "            function=%s, API=%s, error code=%04X\n"
#define KDDS33113 "KDDS33113-E I/O error.\n" \
                  "            function=%s, API=%s, error code=%04X\n"
#define KDDS33114 "KDDS33114-Q Test Suite Log File already exists.\n" \
                  "            Overwrite? (Y/N) "
#define KDDS33115 "KDDS33115-E Line numbers of Test Item File and Base File don't match.\n"
#define KDDS33116 "KDDS33116-E The result doesn't match with Base File.\n" \
                  "            line=%d, output=%s\n"
