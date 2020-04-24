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
 * $Log: uadmmlh.h,v $
 * Revision 1.1.2.2  1996/03/11  14:09:15  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:21  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:42:49  takano
 * 	First Submit
 * 	[1996/02/21  07:42:38  takano]
 * 
 * $EndLog$
 */
/*EnglishComment**************************************************************
 * UADMMLH.H                                                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/



#define NOTHING                0x0000
#define ADMIN                  0x0001
#define VERSION                0x0002
#define USERNAME               0x0003
#define PASSWORD               0x0004
#define ADMINSCREEN            0x0005
#define MENU                   0x0006
#define DFAMVOLUME             0x0007
#define DFAMAGENT              0x0008
#define DFAMUSER               0x0009
#define DFAMGROUP              0x000a
#define DFAMOTHER              0x000b
#define EXITADMIN              0x000c
#define EXITADMIN2             0x000d
#define DFAMVOLUMELIST         0x000e
#define NOTDFAMVOLUMELIST      0x000f
#define MOUNTPOINTNAME         0x0010
#define DELDFAMVOLUME          0x0011
#define AGENTNAME              0x0012
#define NOTDFAMAGENTLIST       0x0013
#define MASTERKEY              0x0014
#define REMASTERKEY            0x0015
#define DFAMUSERLIST           0x0016
#define NOTDFAMUSERLIST        0x0017
#define DCEUSER                0x0018
#define DCEPASSWD              0x0019
#define REENTERDCEPASSWD       0x001a
#define DELDFAMUSER            0x001b
#define DFAMUSERINFO           0x001c
#define CHANGEDCEUSER          0x001d
#define SETADMINEXEC           0x001e
#define ADMINEXECUTION         0x001f
#define EXECUTION              0x0020
#define DFAMGROUPLIST          0x0021
#define NOTGROUPLIST           0x0022
#define DCEGROUPNAME           0x0023
#define DELDFAMGROUP           0x0024
#define OTHEROPTION            0x0025
#define TIMESYNC               0x0026
#define MOUNTWARNING           0x0027
#define DELAYTIME              0x0028
#define DIRSYNC                0x0029
#define TIMEOUT                0x002a
#define CONNECT                0x002b
#define FILESPERS              0x002c
#define COPYBACKDIR            0x002d
#define SEEBOX                 0x002e
#define COPYBACKDIRPATH        0x002f



#define RESOURCETAGNOTALLOC     0x0030
#define LOGINFAILED             0x0031
#define VOLUMENOTFOUND          0x0032
#define DEFINECOPYBACKDIR       0x0033
#define DFSMOUNTPOINTERR        0x0034
#define DFAMLOADED              0x0035
#define DFAMAGENTNOTFOUND       0x0036
#define MASTERKEYFAILURE        0x0037
#define MASTERKEYNOTSAME        0x0038
#define DCEUSERNAMEFAILURE      0x0039
#define DCEPASSWORDFAILURE      0x003a
#define DCEPASSWORDNOTSAME      0x003b
#define DCEGROUPNAMEFAILURE     0x003c
#define COPYDIRNOTEXIST         0x003d
#define COPYDIRNOTEXIST2        0x003e
#define DEFINEVOLUME            0x003f
#define BINDERYERROR            0x0040
#define FILEACCESSERROR         0x0041
#define OMITTED1                0x0042
#define OMITTED2                0x0043
#define VOLUMEFILENOTUSE        0x0044
#define AGENTFILENOTUSE         0x0045
#define OPTIONFILENOTUSE        0x0046
#define VOLUMEFILEINVALID       0x0047
#define AGENTFILEINVALID        0x0048
#define OPTIONFILEINVALID       0x0049
#define OUTOFMEMORY             0x004a
#define SCREENPREFAILED         0x004b
#define INVALSERVVER            0x004c
#define SCREENFAILED            0x004d
#define ADMINERROR              0x004e
