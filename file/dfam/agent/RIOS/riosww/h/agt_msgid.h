/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.
 */
#ifdef HITACHI_REV_CHECK
# ifndef RCS_agt_msgid_h_Included
# define RCS_agt_msgid_h_Included
 static char *RCSID_agt_msgid_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_msgid.h,v 1.1.2.3 1996/03/22 05:14:38 takano Exp $";
# endif /* Not RCS_agt_msgid_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_msgid.h,v $
 * Revision 1.1.2.3  1996/03/22  05:14:38  takano
 * 	Modified the English messages.
 * 	[1996/03/22  05:12:50  takano]
 *
 * Revision 1.1.2.2  1996/03/11  14:06:12  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:55:56  marty]
 * 
 * Revision 1.1.2.1  1996/02/19  09:34:09  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:33:46  dce-sdc]
 * 
 * Revision 1.1.2.7  96/01/12  05:50:23  05:50:23  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:49:46  kishi]
 * 
 * Revision 1.1.2.6  1995/11/02  06:16:41  ichikawa
 * 	Modify KDDS02134,02135,02563 message.
 * 	[1995/11/02  06:16:28  ichikawa]
 * 
 * Revision 1.1.2.5  1995/10/26  02:24:40  kishi
 * 	Change Copyright.
 * 	[1995/10/26  02:24:24  kishi]
 * 
 * Revision 1.1.2.4  1995/10/25  01:51:33  takano
 * 	Add msg.
 * 	[1995/10/25  01:51:14  takano]
 * 
 * Revision 1.1.2.3  1995/10/09  02:49:01  ichikawa
 * 	KDDS01307,01308,02131,02250 is spell miss.
 * 	[1995/10/09  02:48:46  ichikawa]
 * 
 * Revision 1.1.2.2  1995/09/20  07:30:38  ichikawa
 * 	KDDS02282 is spell miss.
 * 	[1995/09/20  07:30:20  ichikawa]
 * 
 * Revision 1.1.2.1  1995/09/09  07:39:24  kishi
 * 	Initial revision
 * 	[1995/09/09  07:19:49  kishi]
 * 
 * $EndLog$
 */
#ifndef _AGT_MSGID_H
#define _AGT_MSGID_H

#define MSG01100           1100
#define KDDS01100          "KDDS01100-I Started Agent termination.\n"
#define MSG01101           1101
#define KDDS01101          "KDDS01101-I Started Agent emergency termination.\n"
#define MSG01102           1102
#define KDDS01102          "KDDS01102-E Agent is already stopped.\n"
#define MSG01103           1103
#define KDDS01103          "KDDS01103-E Agent is being initialized: No command taken.\n"
#define MSG01104           1104
#define KDDS01104          "KDDS01104-E Agent is being terminated: No command taken.\n"
#define MSG01105           1105
#define KDDS01105          "KDDS01105-E Agent is taking emergency exit: No command taken.\n"
#define MSG01106           1106
#define KDDS01106          "KDDS01106-E Unexpected command error.\n            (%s)\n"
#define KDDS01107          "KDDS01107-E Failed to set locale: C locale is assumed.\n            %s\n"

#define MSG01300           1300
#define KDDS01300          "KDDS01300-E Failed to open Master Key File: File has no access right.\n"
#define MSG01301           1301
#define KDDS01301          "KDDS01301-E Gateway %s is not registered with Master Key File.\n"
#define MSG01302           1302
#define KDDS01302          "KDDS01302-E Agent Administrator is not registered with Master Key File.\n"
#define MSG01303           1303
#define KDDS01303          "KDDS01303-E Cannot execute setdfakey command.\n"
#define MSG01304           1304
#define KDDS01304          "KDDS01304-E Cannot execute setdfakey command due to unmatched Master Keys.\n"
#define MSG01305           1305
#define KDDS01305          "KDDS01305-E Failed to write to Master Key File:\n            %s\n"
#define MSG01306           1306
#define KDDS01306          "KDDS01306-E Selected Gateway %s does not exist in /etc/hosts file.\n"
#define MSG01307           1307
#define KDDS01307          "KDDS01307-E Agent is running: Cannot execute setdfakey command.\n"
#define MSG01308           1308
#define KDDS01308          "KDDS01308-E Unexpected error occurred to setdfakey command.\n            (%s)\n"
#define MSG01309            1309
#define KDDS01309          "KDDS01309-E Setlocale failed, continuing with C locale.\n            %s\n"
#define MSG01310            1310
#define KDDS01310          "KDDS01310-E Invalid option.\n"

#define MSG02000           2000
#define KDDS02000          "KDDS02000-I Started Agent initialization.\n"
#define MSG02001           2001
#define KDDS02001          "KDDS02001-I Agent successfully initialized.\n"
#define MSG02002           2002
#define KDDS02002          "KDDS02002-I Agent will be terminated in %i sec.\n"
#define MSG02003           2003
#define KDDS02003          "KDDS02003-I Agent will take an emergency exit.\n"
#define MSG02004           2004
#define KDDS02004          "KDDS02004-I Agent successfully terminated.\n"
#define MSG02005           2005
#define KDDS02005          "KDDS02005-E Agent is already up and running.\n"
#define MSG02006           2006
#define KDDS02006          "KDDS02006-E Failed to start Agent.\n"
#define MSG02007           2007
#define KDDS02007          "KDDS02007-E Agent terminated due to an unexpected error.\n            (%s)\n"
#define MSG02008           2008
#define KDDS02008          "KDDS02008-E Start dfaagt with Supervisor mode.\n            (%s)\n"
#define MSG02030           2030
#define KDDS02030          "KDDS02030-E Trace Process terminated due to an unexpected error.\n            (%s)\n"
#define MSG02031           2031
#define KDDS02031          "KDDS02031-E System Communication Process terminated due to an unexpected error.\n            (%s)\n"
#define MSG02032           2032
#define KDDS02032          "KDDS02032-E Administration Process terminated due to an unexpected error.\n            (%s)\n"
#define MSG02033           2033
#define KDDS02033          "KDDS02033-E Watchdog Process terminated due to an unexpected error.\n            (%s)\n"
#define MSG02034           2034
#define KDDS02034          "KDDS02034-E User Communication Process terminated due to an unexpected error.\n            (%s)\n"
#define MSG02100           2100
#define KDDS02100          "KDDS02100-I Stopped outputting Trace Information to file.\n"
#define MSG02101           2101
#define KDDS02101          "KDDS02101-I Stopped outputting Trace Information of Process %i.\n"
#define MSG02102           2102
#define KDDS02102          "KDDS02102-I Failed to open Trace File.\n            %s\n"
#define MSG02103           2103
#define KDDS02103          "KDDS02103-E Failed to access Trace File.\n            %s:%s\n            (%s)\n"
#define MSG02130           2130
#define KDDS02130          "KDDS02130-E Cannot fork: Agent cannot take connection requests from Gateway.\n            %s:%s\n            (%s)\n"
#define MSG02131           2131
#define KDDS02131          "KDDS02131-I Number of connections exceeds the limit: Agent cannot take connection requests from Gateway.\n            %s\n"
#define MSG02132           2132
#define KDDS02132          "KDDS02132-I Termination of Agent is in progress: Agent cannot take connection requests from Gateway.\n            %s\n"
#define MSG02133           2133
#define KDDS02133          "KDDS02133-I Connect request refused: unknown GATEWAY.\n            %s\n"
#define MSG02134           2134
#define KDDS02134          "KDDS02134-I Fatal error occurred in Gateway.\n            %s\n"
#define MSG02135           2135
#define KDDS02135          "KDDS02135-I Re-connection occurred in Gateway.\n            %s\n"
#define MSG02160           2160
#define KDDS02160          "KDDS02160-E Failed to open Status Logging File.\n            %s\n"
#define MSG02161           2161
#define KDDS02161          "KDDS02161-E Failed to access Status Logging File.\n            %s:%s\n            (%s)\n"
#define MSG02220           2220
#define KDDS02220          "KDDS02220-E No sanity check response returned from Gateway.\n            %s\n"
#define MSG02250           2250
#define KDDS02250          "KDDS02250-E Failed to change DCE Effective Login Period.\n            %s:%s            (%s)\n"
#define MSG02251           2251
#define KDDS02251          "KDDS02251-E Effective Period of DCE password expired.\n            %s\n"
#define MSG02252           2252
#define KDDS02252          "KDDS02252-E Failed to access Master Key File.\n            %s:%s\n            (%s)\n"
#define MSG02280           2280
#define KDDS02280          "KDDS02280-E Failed to generate Trace Process.\n            %s\n"
#define MSG02281           2281
#define KDDS02281          "KDDS02281-E Failed to generate System Communication Process.\n            %s\n"
#define MSG02282           2282
#define KDDS02282          "KDDS02282-E DFS Client is not ready to run.\n"
#define MSG02283           2283
#define KDDS02283          "KDDS02283-I TCP/IP Port Number %i is equal or smaller than 5000.\n"
#define MSG02300           2300
#define KDDS02300          "KDDS02300-E Trace Process terminated: %08x.\n"
#define MSG02301           2301
#define KDDS02301          "KDDS02301-E System Communication Process terminated: %08x.\n"
#define MSG02330           2330
#define KDDS02330          "KDDS02330-E Failed to send a message: %s\n            %s\n"
#define MSG02500           2500
#define KDDS02500          "KDDS02500-E Agent failed to get memory.\n            %s\n            (%s)\n"
#define MSG02520           2520
#define KDDS02520          "KDDS02520-E Agent failed to get shared memory.\n            %s:%s\n            (%s)\n"
#define MSG02521           2521
#define KDDS02521          "KDDS02521-E Shared memory already allocated: will be re-allocated.\n            (%s)\n"
#define MSG02540           2540
#define KDDS02540          "KDDS02540-E Failed to access semaphore.\n            %s:%s\n            (%s)\n"
#define MSG02560           2560
#define KDDS02560          "KDDS02560-E Failed to open socket.\n            %s\n            (%s)\n"
#define MSG02561           2561
#define KDDS02561          "KDDS02561-E Failed to bind an address to the socket.\n            %s:%s\n            (%s)\n"
#define MSG02562           2562
#define KDDS02562          "KDDS02562-E Network error.\n            %s:%s\n            (%s)\n"
#define MSG02563           2563
#define KDDS02563          "KDDS02563-E Timeout occurred receiving data from Gateway.\n            %s\n"
#define MSG02564           2564
#define KDDS02564          "KDDS02564-E Communication error.\n            %s:%s\n            (%s)\n"
#define MSG02580           2580
#define KDDS02580          "KDDS02580-E File system full.\n            %s:%s\n            (%s)\n"
#define MSG02600           2600
#define KDDS02600          "KDDS02600-E Failed to fork.\n            %s:%s\n            (%s)\n"
#define MSG02620           2620
#define KDDS02620          "KDDS02620-E Agent error.\n            (%s)\n"
#define MSG02800           2800
#define KDDS02800          "KDDS02800-E Invalid option.\n"
#define MSG02801           2801
#define KDDS02801          "KDDS02801-E Invalid option value.\n            %s : invalid character detected\n"
#define MSG02900           2900
#define KDDS02900          "KDDS02900-E Can not open configuration file.\n            %s\n            %s\n"
#define MSG02901           2901
#define KDDS02901          "KDDS02901-E Invalid configuration option detected.\n            %s\n"
#define MSG02902           2902
#define KDDS02902          "KDDS02902-E Invalid configuration option.\n            %s : missing option value\n"
#define MSG02903           2903
#define KDDS02903          "KDDS02903-E Invalid configuration option.\n            %s : invalid character detected\n"
#define MSG02904           2904
#define KDDS02904          "KDDS02904-E Invalid configuration option.\n            %s : out of range\n"
#define MSG02905           2905
#define KDDS02905          "KDDS02905-E Invalid configuration option.\n            %s : undefined GATEWAY\n"

#define MSG20000           20000
#define KDDS20000          "usage: dfaagt [-s [time] | -a]\n"
#define MSG20001           20001
#define KDDS20001          "usage: dfaagt [-dfaID id] [-ConfigFile file]\n       [-s [time] | [-a] | -msg message -to gateway...]\n"
#define MSG20003           20003
#define KDDS20003          "usage: setdfakey [-a GATEWAY name | -d GATEWAY name | -l]\n"


#define MSG10000          10000
#define KDDS10000          "Enter master key:"
#define MSG10001          10001
#define KDDS10001          "Re-enter master key:"
#define MSG10002          10002
#define KDDS10002          "Not match. Enter master key:"
#define MSG10003          10003
#define KDDS10003          "Enter Server name:"
#define MSG10004          10004
#define KDDS10004          "%s is not in master key file. New entry (y/n)?:"

#endif
