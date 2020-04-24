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
 * $Log: gmai.h,v $
 * Revision 1.1.2.2  1996/03/11  14:08:31  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:47  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:39:28  takano
 * 	First Submit
 * 	[1996/02/21  07:39:17  takano]
 * 
 * $EndLog$
 */
/*EnglishComment**************************************************************
 * GMAI.H                                                                    *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/

int AddHooks(void);
struct GVolume_t *      AddToGVolume_t(void);
struct GAgent_t	 * 		AddToGAgent_t();
int                     CheckAgentFile(void);
int                     CheckOptionFile(void);
int                     CheckVolumeFile(void);
int                     CopyBackDirCheck(BYTE *);
void                    DeleteFromGAgent_t( struct GAgent_t *);
int                     DFAMInit(void);
void                    fnDFAMTerm(int);
int 					isFileAbleToOpen(char *, time_t *);
int                     isDFSPath(char *);
int						LineRead ( FILE *, char ** );
int						LogFileCheck( char *, int *, char **);
int                     vol_exist_chk( char *);

#define	FUNC_RANGECHECK				"RangeCheck"
#define FUNC_ISDIGITS				"isdigits"
#define FUNC_ADDHOOKS               "AddHooks"
#define FUNC_ADDTOVOLUME            "AddToGVolume_t"
#define FUNC_ADDTOAGENT             "AddToGAgent_t"
#define FUNC_CHKVOLFILE             "CheckVolumeFile"
#define FUNC_CHKAGTFILE             "CheckAgentFile"
#define FUNC_CHKOPTFILE             "CheckOptionFile"
#define FUNC_DELETEFROMVOLUME       "DeleteFromGVolume_t"
#define FUNC_DELETEFROMAGENT        "DeleteFromGAgent_t"
#define FUNC_FILEABLEOPEN           "IsFileAbleToOpen"
#define FUNC_FREEGLOBAL             "FreeGlobalResource"
#define FUNC_DFAMINIT               "DFAMInit"
#define FUNC_LINEREAD               "LineRead"
#define FUNC_LOGFILECHECK           "LogFileCheck"
#define FUNC_REMOVEHOOKS            "RemoveHooks"
#define FUNC_VOLEXISTCHK            "vol_exist_chk"
#define FUNC_ISDFSPATH              "isDFSPath"
#define FUNC_COPYBACKDIRCHECK       "CopyBackDirCheck"

#define LOG_RH_PREOPEN "RmHookPrOp"
#define LOG_RH_POSOPEN "RmHookPoOp"
#define LOG_RH_PRECLOS "RmHookPrCl"
#define LOG_RH_POSCLOS "RmHookPoCl"
#define LOG_RH_PRECREF "RmHookPrCr"
#define LOG_RH_PRECROP "RmHookPrCO"
#define LOG_RH_POSCROP "RmHookPoCO"
#define LOG_RH_PREERAF "RmHookPrEr"
#define LOG_RH_POSERAF "RmHookPoEr"
#define LOG_RH_PREREN  "RmHookPrRe"
#define LOG_RH_POSREN  "RmHookPoRe"
#define LOG_RH_PRECRD  "RmhookPrCD"
#define LOG_RH_POSCRD  "RmhookPoCD"
#define LOG_RH_PREDELD "RmhookPrDD"
#define LOG_RH_POSDELD "RmhookPoDD"


#define INIER_MAIN		0x0000
#define INIER_VOLCHK	0x1000
#define INIER_AGTCHK	0x2000
#define INIER_OPTCHK	0x3000


#define INIER_VOL_NWVOL	0x0100
#define INIER_VOL_DFS	0x0200


#define INIER_AGT_HOST	0x0100
#define INIER_AGT_KEY	0x0200


#define INIER_OPT_SYN	0x0100
#define INIER_OPT_CPT	0x0200
#define INIER_OPT_SBL	0x0300
#define INIER_OPT_RWT	0x0400
#define INIER_OPT_WWT	0x0500
#define INIER_OPT_TS	0x0600
#define INIER_OPT_MW	0x0700
#define INIER_OPT_CBD	0x0800
#define INIER_OPT_MLF	0x0900
#define INIER_OPT_ELF	0x0a00
#define INIER_OPT_CWT	0x0b00
#define INIER_OPT_FSD	0x0c00


#define INIERC_NOT		0x0001
#define INIERC_NDF		0x0002
#define INIERC_INV		0x0003
#define INIERC_OPN		0x0004
#define INIERC_SYS		0x0005
#define INIERC_GWV		0x0006
#define INIERC_DBL		0x0007
#define INIERC_PAIR		0x0008
#define INIERC_NNM		0x0009
#define INIERC_EXC		0x000a
#define INIERC_LES		0x000b


#define	INIERC_ADM		0x0041
#define	INIERC_WAT		0x0042
#define INIERC_VER		0x0043
#define INIERC_INT		0x0044


#define INIERC_BUG		0x0061


#define INIERC_MEM		0x0081
#define INIERC_API		0x0082
#define INIERC_UNK		0x0083



