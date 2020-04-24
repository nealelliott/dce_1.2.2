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
 * $Log: dfagwtp.h,v $
 * Revision 1.1.2.2  1996/03/11  14:11:25  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:59:32  marty]
 *
 * Revision 1.1.2.1  1996/02/29  10:00:33  takano
 * 	Submit DFAM-client test
 * 	[1996/02/29  09:57:05  takano]
 * 
 * Revision 1.1.2.1  1996/02/22  10:41:46  takano
 * 	First Submit
 * 	[1996/02/22  10:41:30  takano]
 * 
 * $EndLog$
 */
/*EnglishComment**************************************************************/
/*                                                                           */
/*   Gateway Testsuite Header File                                           */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/**************************************************************EnglishComment*/

#ifndef DFAGWTP_H
#define DFAGWTP_H

#define DFAGWTPDIR          "SYS:\\SYSTEM\\DFAGWTP\\"
#define DFAGWTPTESTDIR      "SYS:\\SYSTEM\\DFAGWTP\\TEST\\"
#define LOGFILE             "SYS:\\SYSTEM\\DFAGWTP\\DFAGWTP.LOG"
#define CFGFILE             "SYS:\\SYSTEM\\DFAGWTP\\DFAGWTP.CFG"
#define FVTFILE             "SYS:\\SYSTEM\\DFAGWTP\\DFAGWTP.FVT"
#define VOLFILE             "SYS:\\SYSTEM\\DFAVOL.CFG"

#define DFANLM              "DFA.NLM"

#define CON_ECAT            "@CON_T  "

#define DEF_NWUSER0         "DFALOGIN"
#define DEF_NWUSER1         "DFAOWNER"
#define DEF_NWUSER2         "DFATRUST"
#define DEF_NWGROUP0        "DFALOGINGROUP"
#define DEF_NWGROUP1        "DFAOWNERGROUP"
#define DEF_NWGROUP2        "DFATRUSTGROUP"
#define DEF_DCEGROUP0       "dfagroup0"
#define DEF_DCEGROUP1       "dfagroup1"
#define DEF_DCEGROUP2       "dfagroup2"
#define DEF_OTHEROBJ        "DFAOTHER"
#define DEF_MASKOBJ         "DFAMASK"
#define DEF_GWVOLUME        "DFATP"
#define DEF_TOPDIR          "/.../sales.hitachi.co.jp/fs/dfatree"
#define DEF_FVTFILE         "DFAGWTP.FVT"

#define MSG_CONT            "CONT"
#define MSG_STOP            "STOP"

#define DFA_OTHER           "DFA_OTHER_GROUP"
#define DFA_MASK            "DFA_MASK_OBJ"

#define DCE_USER_PROPERTY   "DFA001"
#define DCE_PWD_PROPERTY    "DFA002"
#define DCE_GROUP_PROPERTY  "DFA004"

#define LOG_USER            "USER_CHECK  "
#define LOG_GROUP           "GROUP_CHECK "
#define LOG_VOLUME          "VOLUME_CHECK"
#define LOG_SUCCESS         "SUCCESSFUL"
#define LOG_FAILED          "FAILED"

#define FUNC_READPROPERTY   "ReadPropertyValue"
#define FUNC_FOPEN          "fopen"
#define FUNC_FGETS          "fgets"
#define FUNC_STAT           "stat"
#define FUNC_FPUTS          "fputs"
#define FUNC_FFLUSH         "fflush"
#define FUNC_FCLOSE         "fclose"

#define LOG_NULL            "NULL"

#define VOLFILEVER          "01.00"

#define SEGMENTNUM          128

#define MAX_NWUSERLEN       47
#define MAX_NWGROUPLEN      47
#define MAX_DCEGROUPLEN     1024
#define MAX_TOPDIRLEN       1022

#define MIN_TOPDIRLEN       11
#define MIN_VOLUME          2

#define READBUFLEN          1040

#define ECATCH_LEN          8
#define MAX_NWUSERNUM       10
#define MAX_GROUPNUM        10

#define MAX_MSGLEN          1280

#define LOG_YES             1
#define LOG_NO              0

struct ConfigInfo {
    BYTE asEcat[ECATCH_LEN];
    int  iErrorMode;

    char *pszNWUser[MAX_NWUSERNUM];
    char *pszNWGroup[MAX_GROUPNUM];
    char *pszDCEGroup[MAX_GROUPNUM];

    char aszOtherobj[MAX_NWGROUPLEN + 1];
    char aszMaskobj[MAX_NWGROUPLEN + 1];
    char aszGWVolume[_MAX_VOLUME];
    char aszTopdir[MAX_TOPDIRLEN + 1];
    char aszFVTFile[_MAX_NAME];
};


#define KDDS32000 "KDDS32000-I Verification of Gateway environment started.\n"
#define KDDS32001 "KDDS32001-I Gateway environment successfully verified.\n"
#define KDDS32002 "KDDS32002-I Gateway environment verification discontinued.\n"
#define KDDS32003 "KDDS32003-E Gateway is not running.\n"
#define KDDS32004 "KDDS32004-Q Test Suite Log File already exists.  Overwrite ? (Y/N)"
#define KDDS32005 "KDDS32005-E User %s is not a NetWare user.\n"
#define KDDS32006 "KDDS32006-E NetWare user %s is not a Gateway User.\n"
#define KDDS32007 "KDDS32007-E Group %s is not a NetWare group.\n"
#define KDDS32008 "KDDS32008-E NetWare group %s is not a Gateway Group.\n"
#define KDDS32009 "KDDS32009-E NetWare group %s is not related to DCE group %s.\n"
#define KDDS32010 "KDDS32010-E DCE group related to NetWare group %s is not defined.\n"
#define KDDS32011 "KDDS32011-E NetWare group related to DCE group %s is not defined.\n"
#define KDDS32012 "KDDS32012-E Volume %s is not a NetWare volume.\n"
#define KDDS32013 "KDDS32013-E NetWare volume %s is not a Gateway Volume.\n"
#define KDDS32014 "KDDS32014-E NetWare volume %s is not related to DFS Mount Point %s.\n"
#define KDDS32015 "KDDS32015-E No Configuration File.\n"
#define KDDS32016 "KDDS32016-E Configuration File has invalid data (line=%ld).\n"
#define KDDS32017 "KDDS32017-E Failed to access bindery.\n  function     : %s\n  return value : %d\n  errno        : %d\n  NetWareErrno : %d\n  name         : %s\n  property     : %s\n"
#define KDDS32018 "KDDS32018-E Invalid Gateway Volume Information.\n"
#define KDDS32100 "KDDS32100-I Gateway verification started.\n"
#define KDDS32101 "KDDS32101-I Gateway verification completed.\n"
#define KDDS32102 "KDDS32102-I %s started.\n"
#define KDDS32103 "KDDS32103-I %s successfully verified.\n"
#define KDDS32104 "KDDS32104-E %s failed.\n  function     : %s\n  return value : %d\n  errno        : %d\n  NetWareErrno : %d\n"
#define KDDS32105 "KDDS32105-Q Discontinue further verification ? (Y/N)"
#define KDDS32106 "KDDS32106-E Invalid data in Test Item Catalog File (line=%ld).\n"
#define KDDS32107 "KDDS32107-E No Test Item Catalog File.\n"
#define KDDS32108 "KDDS32108-E Invalid data in Test Item File %s (line=%ld).\n"
#define KDDS32109 "KDDS32109-E No Test Item File %s.\n"
#define KDDS32110 "KDDS32110-I All the test items (%ld) successfully verified.\n"
#define KDDS32111 "KDDS32111-I Successfully ended : %ld item(s)  Abnormally ended : %ld item(s)\n"
#define KDDS32200D "KDDS32200-E File access error.\n  file name    : %s\n  function     : %s\n  return value : %d\n  errno        : %d\n  NetWareErrno : %d\n"
#define KDDS32200S "KDDS32200-E File access error.\n  file name    : %s\n  function     : %s\n  return value : %s\n  errno        : %d\n  NetWareErrno : %d\n"
#define KDDS32201 "KDDS32201-E Shortage of memory.\n"
#define KDDS32202 "KDDS32202-E Internal error.\n  function     : %s\n  return value : %d\n  errno        : %d\n  NetWareErrno : %d\n"
#define    K32000 "  Gateway User Name   : %s\n"
#define    K32001 "                      : %s\n"
#define    K32002 "  Gateway Group Name  : %s\n"
#define    K32003 "  other_obj           : %s\n  mask_obj            : %s\n  Gateway Volume Name : %s\n"

void                main(void);
void                fnGWFVTTerm(int);
void                GWFVTTerm(void);
int                 InputYN(void);
int                 PutStartLog(void);
int                 PutEndLog(char *, char *);
int                 PutLog(char *);
int                 FVTMessage(int, char *, ... );

int                 GWFVTInit(void);
int                 LogfileExistCheck(void);
int                 FVTAllocMemory(void);
int                 ReadConfigFile(void);
int                 SetDefaultValue(void);
int                 DFALoadCheck(void);

int                 CheckGWEnv(void);
int                 CheckGWUser(void);
int                 CheckGWUserProperty(char *);
int                 CheckGWGroup(void);
int                 CheckGWGroupProprety(char *, char *);
int                 CheckGWVolume(void);
int                 DeleteServerNameFromVolume(char *);
void                PrintRegisteredInfo(void);

void                ExecFVT(void);
int                 CheckTXTFile(char *, LONG);
int                 ExecTest(char *);
int                 CheckTestName(char *, char *, LONG);
int                 MapTest(char *, char *);

#endif
