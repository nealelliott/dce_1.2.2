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
 * $Log: dfammsg.h,v $
 * Revision 1.1.2.3  1996/03/22  07:10:12  takano
 * 	Modified the English messages.
 * 	[1996/03/22  07:09:47  takano]
 *
 * Revision 1.1.2.2  1996/03/11  14:07:34  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:00  marty]
 * 
 * Revision 1.1.2.1  1996/02/21  07:22:01  takano
 * 	First Submit
 * 	[1996/02/21  07:21:56  takano]
 * 
 * $EndLog$
 */
/*EnglishComment**************************************************************
 * DFAMMSG.H                                                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/





#define KDDS11001 "KDDS11001-E DFA.NLM cannot run on this NetWare version.\n"

#define KDDS11002 "KDDS11002-E DFA.NLM has different interface version with %s.\n"

#define KDDS11003 "KDDS11003-I Gateway initialization started.\n"

#define KDDS11004 "KDDS11004-E Failed to start %s.\n"

#define KDDS11005 "KDDS11005-E NWAddFSMonitorHook failed: error code %d.\n"

#define KDDS11006 "KDDS11006-E NWRegisterNCPExtension failed: error code %d.\n"

#define KDDS11007 "KDDS11007-E BeginThreadGroup failed: error code %d.\n"

#define KDDS11008 "KDDS11008-W DFA.NLM requires 3.12g or newer version of CLIB.NLM.\n"

#define KDDS11009 "KDDS11009-E GetVolumeNumber failed: error code %d.\n"

#define KDDS11010 "KDDS11010-E AdvertiseService failed: error code %d.\n"

#define KDDS11011 "KDDS11011-E Failed to suspend MainThread.\n"

#define KDDS11012 "KDDS11012-E Failed to initialize Gateway: error code "

#define KDDS11015 "KDDS11015-E Volume %s is invalid or not mounted.\n"

#define KDDS11016 "KDDS11016-E Invalid syntax in DFS path.\n"

#define KDDS11017 "KDDS11017-E Invalid hostname.\n"

#define KDDS11018 "KDDS11018-E Failed to open logfile %s.\n"

#define KDDS11019 "KDDS11019-E Invalid Gateway backup directory %s.\n"

#define KDDS11034 "KDDS11034-E DFA.NLM detected self-contradiction.\n"

#define KDDS11035 "KDDS11035-E Failed to get memory dump.\n"

#define KDDS11036 "KDDS11036-E ResumeThread failed: error code %d.\n"

#define KDDS11037 "KDDS11037-E Insufficient memory.\n"

#define KDDS11039 "KDDS11039-E GetServerInformation failed: error code %d.\n"

#define KDDS11040 "KDDS11040-E GetFileServerID failed: error code %d.\n"

#define KDDS11041 "KDDS11041-E GetVolumeStatistics failed: error code %d.\n"

#define KDDS11046 "KDDS11046-E Failed to read Gateway Volume Options.\n"

#define KDDS11047 "KDDS11047-E Failed to read Agent Options.\n"

#define KDDS11048 "KDDS11048-E Failed to read Gateway Other Options.\n"

#define KDDS11049 "KDDS11049-I Gateway initialization normally completed.\n"

#define KDDS11050 "KDDS11050-I AdminThread detected timeout. AdminThread will be restarted.\n"

#define KDDS11051 "KDDS11051-I Gateway termination process started.\n"

#define KDDS11052 "KDDS11052-I Gateway process will be terminated.\n"

#define KDDS11053 "KDDS11053-I Gateway global resources are being freed.\n"



#define KDDS11201 "KDDS11201-E Gateway detected self-contradiction: error code %d.\n"

#define KDDS11202 "KDDS11202-E Invalid SYS:\\ETC\\HOSTS file.\n"

#define KDDS11203 "KDDS11203-E Invalid SYS:\\ETC\\SERVICES file.\n"

#define KDDS11204 "KDDS11204-E TCPIP.NLM has been unloaded.\n"

#define KDDS11205 "KDDS11205-E Gateway detected unknown error: error code %d.\n"

#define KDDS11206 "KDDS11206-E Agent is not registered with Gateway.\n"

#define KDDS11207 "KDDS11207-E Failed to modify Master Key.\n"

#define KDDS11208 "KDDS11208-E Gateway is not registered with Agent.\n"

#define KDDS11209 "KDDS11209-E Invalid user account. DCE user name: %s.\n"

#define KDDS11210 "KDDS11210-E Gateway and Agent have different Master Keys.\n"

#define KDDS11211 "KDDS11211-E DCE Security Server is unavailable.\n"

#define KDDS11214 "KDDS11214-E Gateway runs out of memory.\n"

#define KDDS11215 "KDDS11215-E Failed to access Bindery: error code %d.\n"

#define KDDS11216 "KDDS11216-E Error in communication with Agent.\n"

#define KDDS11217 "KDDS11217-E Agent is not up and running.\n"

#define KDDS11218 "KDDS11218-E Trustee Rights cannot be assigned to Gateway Volume %s.\n"

#define KDDS11219 "KDDS11219-E Trustee Rights cannot be removed from Gateway Volume %s.\n"

#define KDDS11220 "KDDS11220-I user name: %s.\n"

#define KDDS11221 "KDDS11221-I All the users will be logged out.\n"

#define KDDS11222 "KDDS11222-I All the users have been logged out.\n"

#define KDDS11223 "KDDS11223-E Agent failed to access Master Key.\n"

#define KDDS11224 "KDDS11224-I DFA.NLM was unloaded; Logged out from DCE.\n"

#define KDDS11225 "KDDS11225-E Communication error; Logged out from DCE.\n"

#define KDDS11226 "KDDS11226-I Gateway user %s has been logged out from DCE.\n"



#define KDDS11401 "KDDS11401-E Failed to copy back the file to DFS.\n"

#define KDDS11402 "KDDS11402-I Data lost: backup directory not defined.\n"

#define KDDS11403 "KDDS11403-I Following files are unchanged:\n"

#define KDDS11404 "KDDS11404-I File succesfully copied back to DFS.\n"

#define KDDS11405 "KDDS11405-I Following files have been copied back:\n"

#define KDDS11406 "KDDS11406-I Following files renamed after being backed up:\n"

#define KDDS11407 "KDDS11407-I Path names will be displayed only once.\n"

#define KDDS11408 "KDDS11408-I File names will be displayed only once.\n"

#define KDDS11409 "KDDS11409-I %s-\n"

#define KDDS11410 "KDDS11410-I -%s\n"

#define KDDS11411 "KDDS11411-I -%s\n"

#define KDDS11412 "KDDS11412-I -%s\n"

#define KDDS11413 "KDDS11413-E Communication error; Logged out from DCE.\n"

#define KDDS11414 "KDDS11414-E Gateway user %s will be logged out from DCE due to communication error: error code %d.\n"


#define KDDS11601 "KDDS11601-E Volume %s is invalid or not mounted.\n"

#define KDDS11603 "KDDS11603-E Failed to read Volume Information of %s.\n"

#define KDDS11604 "KDDS11604-E File-Access Administrator is not registered.\n"

#define KDDS11605 "KDDS11605-E File-Access Administrator failed to log in.\n"

#define KDDS11607 "KDDS11607-E Failed to connect with Agent.\n"

#define KDDS11613 "KDDS11613-E Insufficient memory.\n"

#define KDDS11624 "KDDS11624-E Directory Sync. detected self-contradiction: error code %d.\n"

#define KDDS11625 "\
KDDS11625-Q Delete files/directories remaining in Volume %s ?\n\
 Y: Delete them(Use the volume as a Gateway Volume).\n\
 N: Keep them  (Do not use it as a Gateway Volume).\n\
 (Y/N)\n"

#define KDDS11631 "KDDS11631-I Started Volume %s initialization.\n"

#define KDDS11632 "KDDS11632-I Volume %s succesfully initialized.\n"

#define KDDS11633 "KDDS11633-I Failed to initialize Volume %s.\n"

#define KDDS11634 "KDDS11634-I Started Volume %s cleanup.\n"

#define KDDS11635 "KDDS11635-I Volume %s successfully cleaned up.\n"

#define KDDS11636 "KDDS11636-I Started to reconnect with Agent.\n"

#define KDDS11637 "KDDS11637-I Succesfully re-connected with Agent.\n"

#define KDDS11638 "KDDS11638-I DCE login for Administrator started.\n"

#define KDDS11639 "KDDS11639-I Started to initialize directory synchronization.\n"

#define KDDS11640 "KDDS11640-I Directory synchronization successfully initialized.\n"

#define KDDS11641 "KDDS11641-I Failed to initialize directory synchronization.\n"

#define KDDS11642 "KDDS11642-I Started to terminate directory synchronization.\n"

#define KDDS11643 "KDDS11643-I Directory synchronization successfully terminated.\n"

#define KDDS11644 "KDDS11644-I Volume %s initialization started.\n"

#define KDDS11645 "KDDS11645-I Volume %s initialization terminated: return code %d.\n"

#define KDDS11646 "KDDS11646-I Directory Sync. started.\n"

#define KDDS11647 "KDDS11647-I Directory Sync. terminated: return code %d.\n"


#define KDDS11651 "KDDS11651-E NetWare server has invalid network environment.\n"

#define KDDS11652 "KDDS11652-E No response from Agent.\n"

#define KDDS11653 "KDDS11653-E Disconnection occurred in the communication with Agent.\n"

#define KDDS11654 "KDDS11654-E Timeout occurred in the communication with Agent.\n"


#define KDDS11661 "KDDS11661-E Administrator has invalid DCE account or password.\n"


#define KDDS11662 "KDDS11662-E Agent is not in root-mode.\n"


#define KDDS11663 "KDDS11663-E Agent has reported error: error code %d.\n"

#define KDDS11664 "KDDS11664-E Mount point %s does not exist.\n"

#define KDDS11665 "KDDS11665-E Failed to access Mount point %s.\n"

#define KDDS11666 "KDDS11666-E Mount point %s does not have access rights.\n"

#define KDDS11667 "KDDS11667-E Different encryption scheme is used.\n"

#define KDDS11668 "KDDS11668-E Different protocol is used.\n"

#define KDDS11671 "KDDS11671-E Gateway Volume %s runs out of disk space.\n"

#define KDDS11672 "KDDS11672-E Gateway Volume %s runs out of disk blocks.\n"

#define KDDS11681 "KDDS11681-E Gateway detected unknown error: error code %d.\n"





#define KDDS11801 "KDDS11801-E Access error while file is being dumped.\n"

#define KDDS11802 "KDDS11802-E Initialization error in Watchdog connection.\n"

#define KDDS11803 "KDDS11803-E Error in Watchdog connection.\n"

#define KDDS11804 "KDDS11804-E Failed to load DFALIB.NLM.\n"

#define KDDS11805 "KDDS11805-E Failed to register DFALIB.\n"

#define KDDS11807 "KDDS11807-E Failed to continue DFALIB handling.\n"

#define KDDS11808 "KDDS11808-I Agent will be terminated in %d sec.\n"

#define KDDS11809 "KDDS11809-I Agent will be killed.\n"

#define KDDS11810 "KDDS11810-E Failed to renew DCE login Period.\n"

#define KDDS11811 "KDDS11811-I Effective Period: %s.\n"

#define KDDS11812 "KDDS11812-I Cause: Security Server is not running.\n"

#define KDDS11813 "KDDS11813-I Cause: Login Period too small.\n"

#define KDDS11814 "KDDS11814-I Cause: Password changed while login.\n"


#define KDDS11816 "KDDS11816-I Cause: Account invalid or expired.\n"

#define KDDS11817 "KDDS11817-I Cause: Login Period expired before renewed.\n"

#define KDDS11818 "KDDS11818-I Unknown DCE error.\n"

#define KDDS11819 "KDDS11819-I Change DCE password.\n"

#define KDDS11820 "KDDS11820-I Gateway Watchdog initialization started.\n"

#define KDDS11821 "KDDS11821-I Gateway Watchdog initialization completed.\n"

#define KDDS11822 "KDDS11822-I Gateway Watchdog will be suspended.\n"

#define KDDS11823 "KDDS11823-I Gateway Watchdog will be restarted.\n"

#define KDDS11824 "KDDS11824-I Gateway Watchdog connection will be closed.\n"

#define KDDS11825 "KDDS11825-I Gateway Watchdog will be terminated.\n"

#define KDDS11826 "KDDS11826-I Dump will be started.\n"

#define KDDS11827 "KDDS11827-I Dump completed.\n"

#define KDDS11828 "KDDS11828-I Unknown message received.\n"


#define KDDS12001 "KDDS12001-E Communication error; Logged out from DCE.\n"

#define KDDS12002 "KDDS12002-E Gateway user %s will be logged out from DCE due to communication error: error code %d.\n"



#define KDDS12201 "KDDS12201-E Failed to record communication logging:"


#define KDDS12401 "KDDS12401-I Data was taken, and stored to message logging file.\n"

#define KDDS12402 "KDDS12402-I Data was taken, and stored to error logging file.\n"

#define KDDS12403 "KDDS12403-I Data was taken, and stored to communication logging file.\n"

#define KDDS12404 "KDDS12404-I Data was taken, and stored to trace logging file.\n"

#define KDDS12405 "KDDS12405-I Data was taken, and stored to dump file.\n"

#define KDDS12407 "KDDS12407-E DFARAS.NLM has different interface version from DFA.NLM.\n"
