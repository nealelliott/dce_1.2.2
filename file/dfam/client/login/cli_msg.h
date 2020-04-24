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
 * $Log: cli_msg.h,v $
 * Revision 1.1.2.3  1996/03/22  05:44:01  takano
 * 	Modified the English messages.
 * 	[1996/03/22  05:43:41  takano]
 *
 * Revision 1.1.2.2  1996/03/11  14:06:47  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:23  marty]
 * 
 * Revision 1.1.2.1  1996/02/21  05:55:20  takano
 * 	First Submit
 * 	[1996/02/21  05:55:14  takano]
 * 
 * $EndLog$
 */
// EnglishComment
/*****************************************************************************/
/* cli_msge.h :                                                              */
/*                                                                           */
/*  date = October 18, 1994 by Kentarou Chikura                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
// EnglishComment
#define     MSG_TXT_01      "KDDS22001-I  (\x01 \x01) successfully logged in.\n"
#define     MSG_TXT_02      "KDDS22101-I  (\x01) successfully logged out.\n"
#define     MSG_TXT_03      "KDDS22402-E  Communication error\n"
#define     MSG_TXT_04      "KDDS22002-E  Cannot log in to DCE\n"
#define     MSG_TXT_05      "KDDS22003-W  User \x01 is not a legitimate File-Access user.\n"
#define     MSG_TXT_06      "KDDS22004-W  Designated \x01(\x01) does not exist in DCE.\n"
#define     MSG_TXT_07      "KDDS22403-W  Gateway is not running on \x01.\n"
#define     MSG_TXT_08      "KDDS22005-E  Invalid DCE password\n"
#define     MSG_TXT_09      "KDDS22102-E  Cannot log out from DCE\n"
#define     MSG_TXT_10      "KDDS22404-W  Not logged in to DCE\n"
#define     MSG_TXT_11      "KDDS22405-W  Not logged in to NetWare server\n"
#define     MSG_TXT_12      "KDDS22301-I  Geteway is active on \x01 file servers in the network.\n"
#define     MSG_TXT_13      "KDDS22302-W  No Gateway is active in the network.\n"
#define     MSG_TXT_14      "KDDS22201-W  Invalid old password\n"
#define     MSG_TXT_15      "KDDS22202-W  Invalid re-entered password\n"
#define     MSG_TXT_16      "KDDS22406-W  Parameter error: Set one server at a time\n"
#define     MSG_TXT_17      "KDDS22407-E  Cannot communicate with NetWare server \x01\n"
#define     MSG_TXT_18      "KDDS22205-I  Enter old password for \x01:"
#define     MSG_TXT_19      "KDDS22206-I  Enter new password for \x01:"
#define     MSG_TXT_20      "KDDS22204-I  Password change is in progress.\n"
#define     MSG_TXT_21      "KDDS22207-I  Re-enter new password for \x01:"
#define     MSG_TXT_22      "KDDS22208-I  Password for \x01 was changed.\n"
#define     MSG_TXT_23      "KDDS22408-W  NetWare Client runs out of memory.\n"
#define     MSG_TXT_24      "KDDS22006-W  Already logged in to DCE\n"
#define     MSG_TXT_25      "KDDS22409-E  Gateway or Agent runs out of memory.\n"
#define     MSG_TXT_26      "KDDS22410-E  Gateway failed to access file 'bindery'\n"
#define     MSG_TXT_27      "KDDS22411-E  System error occurred in Gateway\n"
#define     MSG_TXT_28      "KDDS22412-E  Connection error occurred between Gateway and Agent.\n"
#define     MSG_TXT_29      "KDDS22414-W  Login/logout in progress: No jobs taken.\n"
#define     MSG_TXT_30      "KDDS22209-Q  The above Gateway shares the same old password as \x01. "
#define     MSG_TXT_31      "KDDS22105-I  Server \x01 is processing DCE logout.\n"
#define     MSG_TXT_32      "KDDS22210-E  Cannot change or check password\n"
#define     MSG_TXT_33      "KDDS22415-E  Cannot access the DCE login status\n"
#define     MSG_TXT_34      "KDDS22416-W  Agent is not running.\n"
#define     MSG_TXT_35      "KDDS22417-E  System error occurred in Agent\n"
#define     MSG_TXT_36      "KDDS22418-E  System error occurred in DCE\n"
#define     MSG_TXT_37      "KDDS22007-W  The number of Gateway login users exceeds the limit.\n"
#define     MSG_TXT_fd      "KDDS22991-I  Parm1 (\x01) Parm2 (\x01).\n"
#define     MSG_TXT_fe      "KDDS22992-E  Messages no error (\x01).\n"
#define     MSG_TXT_ff      "KDDS22993-I  \x01.\n"
