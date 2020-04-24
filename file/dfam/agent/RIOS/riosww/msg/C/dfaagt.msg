$ Trade Secret of Hitachi, Ltd.
$ Do not disclose without written concession from Hitachi, Ltd.
$ Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.

$
$ dfaagt -a or dfaagt -s 
$

1100	KDDS01100-I Started Agent termination.\n

1101	KDDS01101-I Started Agent emergency termination.\n

1102	KDDS01102-E Agent is already stopped.\n

1103	KDDS01103-E Agent is being initialized: No command taken.\n

1104	KDDS01104-E Agent is being terminated: No command taken.\n

1105	KDDS01105-E Agent is taking emergency exit: No command taken.\n

1106	KDDS01106-E Unexpected command error.\n            (%s)\n

1107	KDDS01107-E Failed to set locale: C locale is assumed.\n            %s\n

2000	KDDS02000-I Started Agent initialization.\n

2001	KDDS02001-I Agent successfully initialized.\n

2002	KDDS02002-I Agent will be terminated in %i sec.\n

2003	KDDS02003-I Agent will take an emergency exit.\n

2004	KDDS02004-I Agent successfully terminated.\n

2005	KDDS02005-E Agent is already up and running.\n

2006	KDDS02006-E Failed to start Agent.\n

2007	KDDS02007-E Agent terminated due to an unexpected error.\n            (%s)\n
2008	KDDS02008-E Start dfaagt with Supervisor mode.\n            (%s)\n

2030	KDDS02030-E Trace Process terminated due to an unexpected error.\n            (%s)\n

2031	KDDS02031-E System Communication Process terminated due to an unexpected error.\n            (%s)\n

2032	KDDS02032-E Administration Process terminated due to an unexpected error.\n            (%s)\n

2033	KDDS02033-E Watchdog Process terminated due to an unexpected error.\n            (%s)\n

2034	KDDS02034-E User Communication Process terminated due to an unexpected error.\n            (%s)\n

2100	KDDS02100-I Stopped outputting Trace Information to file.\n

2101	KDDS02101-I Stopped outputting Trace Information of Process %i.\n

2102	KDDS02102-I Failed to open Trace File.\n            %s\n

2103	KDDS02103-E Failed to access Trace File.\n            %s:%s\n            (%s)\n

2130	KDDS02130-E Cannot fork: Agent cannot take connection requests from Gateway.\n            %s:%s\n            (%s)\n

2131	KDDS02131-I Number of connections exceeds the limit: Agent cannot take connection requests from Gateway.\n            %s\n

2132	KDDS02132-I Termination of Agent is in progress: Agent cannot take connection requests from Gateway.\n            %s\n

2133	KDDS02133-I Connect request refused: unknown GATEWAY.\n            %s\n

2134	KDDS02134-I Fatal error occurred in Gateway.\n            %s\n

2135	KDDS02135-I Re-connection occurred in Gateway.\n            %s\n

2160	KDDS02160-E Failed to open Status Logging File.\n            %s\n

2161	KDDS02161-E Failed to access Status Logging File.\n            %s:%s\n            (%s)\n

2220	KDDS02220-E No sanity check response returned from Gateway.\n            %s\n

2250	KDDS02250-E Failed to change DCE Effective Login Period.\n            %s:%s            (%s)\n

2251	KDDS02251-E Effective Period of DCE password expired.\n            %s\n

2252	KDDS02252-E Failed to access Master Key File.\n            %s:%s\n            (%s)\n

2280	KDDS02280-E Failed to generate Trace Process.\n            %s\n

2281	KDDS02281-E Failed to generate System Communication Process.\n            %s\n

2282	KDDS02282-E DFS Client is not ready to run.\n

2283	KDDS02283-I TCP/IP Port Number %i is equal or smaller than 5000.\n

2300	KDDS02300-E Trace Process terminated: %08x.\n

2301	KDDS02301-E System Communication Process terminated: %08x.\n

2330	KDDS02330-E Failed to send a message: %s\n            %s\n

2500	KDDS02500-E Agent failed to get memory.\n            %s\n            (%s)\n

2520	KDDS02520-E Agent failed to get shared memory.\n            %s:%s\n            (%s)\n

2521	KDDS02521-I Shared memory already allocated: will be re-allocated.\n            (%s)\n

2540	KDDS02540-E Failed to access semaphore.\n            %s:%s\n            (%s)\n

2560	KDDS02560-E Failed to open socket.\n            %s\n            (%s)\n

2561	KDDS02561-E Failed to bind an address to the socket.\n            %s:%s\n            (%s)\n

2562	KDDS02562-E Network error.\n            %s:%s\n            (%s)\n

2563	KDDS02563-E Timeout occurred receiving data from Gateway.\n            %s\n

2564	KDDS02564-E Communication error.\n            %s:%s\n            (%s)\n

2580	KDDS02580-E File system full.\n            %s:%s\n            (%s)\n

2600	KDDS02600-E Failed to fork.\n            %s:%s\n            (%s)\n

2620	KDDS02620-E Agent error.\n            (%s)\n

2800	KDDS02800-E Invalid option.\n

2801	KDDS02801-E Invalid option value.\n            %s : invalid character detected\n

2900	KDDS02900-E Can not open configuration file.\n            %s\n            %s\n

2901	KDDS02901-E Invalid configuration option detected.\n            %s\n

2902	KDDS02902-E Invalid configuration option.\n            %s : missing option value\n

2903	KDDS02903-E Invalid configuration option.\n            %s : invalid character detected\n

2904	KDDS02904-E Invalid configuration option.\n            %s : out of range\n

2905	KDDS02905-E Invalid configuration option.\n            %s : undefined GATEWAY\n

20000	usage: dfaagt [-s [time] | -a]\n

20001	usage: dfaagt [-dfaID id] [-ConfigFile file]\n       [-s [time] | [-a] | -msg message -to gateway...]\n
