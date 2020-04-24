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
 * $Log: uadmdef.h,v $
 * Revision 1.1.2.2  1996/03/11  14:09:10  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:58:17  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:40:20  takano
 * 	First Submit
 * 	[1996/02/21  07:40:15  takano]
 * 
 * $EndLog$
 */
/*EnglishComment**************************************************************
 * UADMDEF.H                                                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
#define ON  1
#define OFF 0

#define INPUTUSER  0
#define INPUTPASS  1

#define FROM_VOLUME_INIT  0
#define FROM_OTHER_INIT   1

#define	DEFAULTATTRIBUTE   0x00000020

#define OPTFILEVER "01.01"
#define VOLFILEVER "01.00"
#define AGTFILEVER "01.00"
#define CFGVER_0100 "01.00"

#define MAX_NWUSERNAME 48
#define MAX_NWPASSWORD 128
#define MAX_DCEUSERNAME 1025
#define MAX_DCEGROUPNAME 1025
#define MAX_DCEPASSWD    1025
#define MAX_DFAMGROUPNAME 48
#define MAX_DFAMAGENTNAME 65
#define MAX_DFSMOUNTPOINT 1023
#define MAX_MASTERKEY 18

#define MAX_COPYBACKBUFLEN  6
#define MAX_DIRSYNCBUFLEN   5
#define MAX_TIMEOUTBUFLEN   6
#define MAX_CONNECTBUFLEN   6
#define MAX_FILESPERSBUFLEN 3
#define MAX_COPYBACKDIRNAME 304
#define MAX_COPYBACKDIRNAMESTRING 242
#define MAX_SENDBUFLEN 6
#define MAX_MESBUFLEN 300
#define MAX_ERRBUFLEN 300
#define MAX_WATCHDOGBUFLEN 3

#define MIN_COPYBACKDELAYTIME     0
#define DEF_COPYBACKDELAYTIME     0
#define MAX_COPYBACKDELAYTIME 10000
#define MIN_DIRSYNCINTERVAL       1
#define DEF_DIRSYNCINTERVAL       5
#define MAX_DIRSYNCINTERVAL    1440
#define MIN_TIMEOUT_TIME          1
#define DEF_TIMEOUT_TIME        120
#define MAX_TIMEOUT_TIME      32767
#define MIN_CONNECT_TIME          1
#define DEF_CONNECT_TIME         45
#define MAX_CONNECT_TIME      32767
#define MIN_SPERSDELAYTIME        0
#define DEF_SPERSDELAYTIME        0
#define MAX_SPERSDELAYTIME       10

#define MIN_MASTERKEYLEN   1
#define MAX_MASTERKEYLEN   8

#define SEGMENTNUM   128

#define MENUROW 8
#define MENUCOL 40
#define MENUHEIGHT 6

#define EXITROW 12
#define EXITCOL 40
#define EXITHEIGHT 1

#define OTHERROW 8
#define OTHERCOL 40
#define OTHERHEIGHT 13
#define OTHERWIDTH 62
#define OTHERFIELD 40

#define COPYDIRROW 18
#define COPYDIRCOL 40
#define COPYDIRHEIGHT 1
#define COPYDIRWIDTH 78

#define USERLISTROW 5
#define USERLISTCOL 20
#define USERLISTHEIGHT 15
#define MAX_USERLISTWIDTH 47
#define MIN_USERLISTWIDTH 24

#define NOTUSERLISTROW 5
#define NOTUSERLISTCOL 60
#define NOTUSERLISTHEIGHT 15
#define MAX_NOTUSERLISTWIDTH 47
#define MIN_NOTUSERLISTWIDTH 24

#define DELUSERROW 10
#define DELUSERCOL 40
#define DELUSERHEIGHT 1

#define DCEUSERROW 12
#define DCEUSERCOL 40
#define DCEUSERHEIGHT 1
#define DCEUSERWIDTH 78

#define DCEPASSROW 18
#define DCEPASSCOL 40
#define DCEPASSHEIGHT 1
#define DCEPASSWIDTH 78

#define USERINFOROW  5
#define USERINFOCOL 60
#define USERINFOHEIGHT 2
#define USERINFOWIDTH 30

#define ADMINEXECROW  18
#define ADMINEXECCOL  60
#define ADMINEXECHEIGHT 1
#define ADMINEXECWIDTH 32
#define ADMINEXECFIELD 28

#define GROUPLISTROW 5
#define GROUPLISTCOL 20
#define GROUPLISTHEIGHT 15
#define MAX_GROUPLISTWIDTH 47
#define MIN_GROUPLISTWIDTH 24

#define DELGROUPROW 10
#define DELGROUPCOL 40
#define DELGROUPHEIGHT 1

#define NOTGROUPLISTROW 5
#define NOTGROUPLISTCOL 60
#define NOTGROUPLISTHEIGHT 15
#define MAX_NOTGROUPLISTWIDTH 47
#define MIN_NOTGROUPLISTWIDTH 24

#define DCEGROUPROW 18
#define DCEGROUPCOL 40
#define DCEGROUPHEIGHT 1
#define DCEGROUPWIDTH 78

#define VOLLISTROW 5
#define VOLLISTCOL 20
#define VOLLISTHEIGHT 10
#define VOLLISTWIDTH 16

#define NOTVOLUMELISTROW 5
#define NOTVOLUMELISTCOL 60
#define NOTVOLUMELISTHEIGHT 10
#define NOTVOLUMELISTWIDTH 16

#define DELVOLUMEROW 10
#define DELVOLUMECOL 40
#define DELVOLUMEHEIGHT 1

#define MOUNTPOINTROW 18
#define MOUNTPOINTCOL 40
#define MOUNTPOINTHEIGHT 1
#define MOUNTPOINTWIDTH 78

#define AGENTNAMEROW 16
#define AGENTNAMECOL 40
#define AGENTNAMEHEIGHT 3
#define MAX_AGENTNAMEWIDTH 64
#define MIN_AGENTNAMEWIDTH 32

#define NOTAGENTLISTROW 5
#define NOTAGENTLISTCOL 20
#define NOTAGENTLISTHEIGHT 15
#define MIN_NOTAGENTLISTWIDTH 32
#define MAX_NOTAGENTLISTWIDTH 64

#define MASTERKEYROW 18
#define MASTERKEYCOL 60
#define MASTERKEYHEIGHT 1
#define MASTERKEYWIDTH 8

#define WAITAGENTROW        16
#define WAITAGENTCOL        40

#define WAITNOTAGENTROW     10
#define WAITNOTAGENTCOL     20

#define WAITVOLROW          10
#define WAITVOLCOL          20

#define WAITNOTVOLROW       10
#define WAITNOTVOLCOL       60

#define WAITUSERROW         10
#define WAITUSERCOL         20

#define WAITNOTUSERROW      10
#define WAITNOTUSERCOL      60

#define WAITGROUPROW        10
#define WAITGROUPCOL        20

#define WAITNOTGROUPROW     10
#define WAITNOTGROUPCOL     60

#define WAITOTHERROW         8
#define WAITOTHERCOL        40


#define DCE_USER_PROPERTY   "DFA001"
#define DCE_PWD_PROPERTY    "DFA002"
#define ADMIN_EXEC_PROPERTY "DFA003"
#define DCE_GROUP_PROPERTY  "DFA004"

#define VOLFILE     "sys:\\system\\dfavol.cfg"
#define AGTFILE     "sys:\\system\\dfaagt.cfg"
#define GRPFILE     "sys:\\system\\dfagrp.cfg"
#define OPTFILE     "sys:\\system\\dfaopt.cfg"
#define TMPVOLFILE  "sys:\\system\\dfavol.tmp"
#define TMPAGTFILE  "sys:\\system\\dfaagt.tmp"
#define TMPGRPFILE  "sys:\\system\\dfagrp.tmp"
#define TMPOPTFILE  "sys:\\system\\dfaopt.tmp"

#define DFAMNLM   "DFA.NLM"

#define CHAR_OF_BACKUPDIR "!..~"
#define CHAR_OF_STRING    " ..~"


struct VolumeInfo {
    struct VolumeInfo *pNext;
    char aszDFAMVolume[_MAX_VOLUME + 2];
    char aszDFSMountPoint[MAX_DFSMOUNTPOINT + 1];
};

struct AgentInfo {
    char aszDFAMAgent[MAX_DFAMAGENTNAME + 1];
    char aszMasterKey[MAX_MASTERKEY + 1];
};

struct GroupInfo {
    struct GroupInfo *pNext;
    char aszDFAMGroup[MAX_DFAMGROUPNAME + 1];
    char aszDCEGroup[MAX_DCEGROUPNAME + 1];
};

struct OtherInfo {
    LONG uiCopyback;
    LONG uiDirSync;
    LONG uiTimeOut;
    LONG uiConnect;
    LONG uiFileSpers;
    BYTE byTimeSync;
    BYTE byMountWarning;
    char aszCopyDir[MAX_COPYBACKDIRNAME];
    char aszSendBuf[MAX_SENDBUFLEN];
    char aszMesFile[MAX_MESBUFLEN];
    char aszLogFile[MAX_ERRBUFLEN];
    char aszWatchdog[MAX_WATCHDOGBUFLEN];
};

struct OptFileData {

    unsigned int bIsTimeSync    : 1;
    unsigned int bIsMountWarn   : 1;
    unsigned int bIsCopyBack    : 1;
    unsigned int bIsDirSync     : 1;
    unsigned int bIsTimeOut     : 1;
    unsigned int bIsConnect     : 1;
    unsigned int bIsFileSpers   : 1;
    unsigned int bIsBackup      : 1;
    unsigned int bIsSendBuf     : 1;
    unsigned int bIsMesLog      : 1;
    unsigned int bIsErrLog      : 1;
    unsigned int bIsWatchdog    : 1;
    unsigned int bRest          : 6;
};



void adminUnload(int);
void adminUnload2(void);
int adminLogin(void);
int MenuAction(LONG, LIST **, LONG *, void *);
void getVerInfo(void);
int CFGVerCheck(FILE *, char *);
int ConsoleInput(char *, int);
int isDigitString(char *);
int SkipReadCFGFile(FILE *);
void AlertErrorCode(LONG, char *);

void otherOption(void);
int OtherInit(void);
int writeOptFile(void);
LONG OtherOptAction(FIELD *, int, int *, NUTInfo *);
int CopyDirAction(LONG, BYTE *, void *);
void delAllOther(void);
int EscapeDirNameCheck(char *);
int CompareEscapeDirNameToDFAMVolume(char *);
int EscapeDirExistCheck(char *);
int DeleteServerNameFromBackupDir(char *);
int SetDefaultFileAttribute(char *);

void DFAMUserList(void);
int DFAMUserAction(LONG, LIST **, LONG *, void *);
int NOTDFAMUserAction(LONG, LIST **, LONG *, void *);
int DCEUserAction(LONG, BYTE *, void *);
int DCEPasswdAction(LONG, BYTE *, void *);
int DCEReEnterPasswdAction(LONG, BYTE *, void *);
int DFAMInfoAction(LONG, LIST **, LONG *, void *);
int GetDCEUserName(char *, char *, int *);

void DFAMGroupList(void);
int DFAMGroupAction(LONG, LIST **, LONG *, void *);
int NotDFAMGroupAction(LONG, LIST **, LONG *, void *);
int DCEGroupNameAction(LONG, BYTE *, void *);
int GetDCEGroupName(char *, char *, int *);

void DFAMVolumeList(void);
int DFAMVolumeAction(LONG ,LIST **, LONG *, void *);
int NOTDFAMVolumeAction(LONG, LIST **, LONG *, void *);
int DFSMountPointAction(LONG, BYTE *, void *);
int DFSPathCheck(char *);
int VolumeInit(int);
int addVolume(char *, char *);
int delVolume(char *);
struct VolumeInfo *getVolume(char *);
void delAllVolume(void);
int writeVolumeFile(void);
int DFAMVolumeNameCheck(char *);
int GetEscapeDirName(char *);

void DFAMAgent(void);
int DFAMAgentAction(LONG, LIST **, LONG *, void *);
int NOTDFAMAgentAction(LONG, LIST **, LONG *, void *);
int MasterKeyAction(LONG, BYTE *, void *);
int ReMasterKeyAction(LONG, BYTE *, void *);
int AgentInit(void);
int addAgent(char *, char *);
int AgentExistCheck(char *);
int writeAgentFile(void);
int DFAMLoadCheck(void);
void delAllAgent(void);
int DFAMAgentNameCheck(char *);
int DFAMMasterKeyCheck(char *);

void fnSetEncryptData( LONG *, BYTE *, int, BYTE * );
void fnGetDecryptData( LONG *, BYTE *, int, BYTE * );
void MakeMasterKey(BYTE *, BYTE *);
