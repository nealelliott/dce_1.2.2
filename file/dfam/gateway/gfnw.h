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
 * $Log: gfnw.h,v $
 * Revision 1.1.2.2  1996/03/11  14:08:21  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:38  marty]
 *
 * Revision 1.1.2.1  1996/02/21  07:38:50  takano
 * 	First Submit
 * 	[1996/02/21  07:38:44  takano]
 * 
 * $EndLog$
 */
/*EnglishComment**************************************************************
 * GFNW.H                                                                    *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/

int                     AddToDelList(struct GName_t *);
struct GName_t *        AddToName_t( BYTE *);
struct GName_t *        AllocName_t();
void                    ClearFNWDellist(struct GName_t *);
int                     DeleteName_t( struct GName_t *p, int );
void                    DeleteAllName_t( struct GName_t *);
void                    DeleteName_tByName( BYTE * );
int                     DeleteName_t( struct GName_t *p, int );
int                     DevideFileName(BYTE *, BYTE *, BYTE *);
struct DFSFakeDirEnt *  DFSExistchk ( BYTE *, struct DFSFakeDirEnt *);
int                     EntSynChk( BYTE *);
struct GName_t *        FindName_t ( BYTE *);
int                     from36toi( char *);
int                     GetDFAMVolumeName( BYTE *, BYTE *);
int                     GetEntry(char *, char *, char **);
int                     isDFAMVolume( BYTE *, struct GVolume_t ** );
void                    ito36( int, char *);
int                     invalchk(BYTE *, int);
BYTE *                  NWExistchk ( BYTE *, BYTE *);
int                     SearchName_tByName ( BYTE *, struct GName_t *, BOOL, struct GName_t **);
int                     SearchUniqueFileName(BYTE *, BYTE *, BYTE *, BYTE *,BYTE *, struct DFSFakeDirEnt *);
int                     SetDefaultAttr(char *);
int                     upperchk(BYTE *);

#define	NW_DELIMITER	     "\\\/"
#define STR36                "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
#define DELETEDENTRYKEEPTIME 3600

#define MODE_DFS             1
#define MODE_NW              2

#define FOUND                1
#define NOTFOUND             0
#define FNWERR_NOT_DFAMVOL   0
#define FNWERR_NORMAL        0
#define FNWERR_IS_DFAMVOL    1
#define FNWERR_PTR_IS_NULL  -1
#define FNWERR_NO_SUBDIR    -2
#define FNWERR_NO_ENTRY     -3

#define DELLIST_MAX         10
#define VOLUMELEN_MAX       64
#define KEYNUMBER_MAX       1296

#define WILDCARD             "\\*.*"
#define DEFAULTATTR         0x00000020


#define FUNC_GETNWPATH                  "GetNWPath"
#define FUNC_GETDFSPATH                 "GetDFSPath"
#define FUNC_GETENTRYNAME               "GetEntryName"
#define FUNC_FINDNAME_T                 "FindName_t"
#define FUNC_INVALCHK                   "invalchk"
#define FUNC_UPPERCHK                   "upperchk"
#define FUNC_DEVIDEFILENAME             "DevideFilename"
#define FUNC_SEARCHUNIQUEFILENAME       "SearchUniqueFileName"
#define FUNC_ITO36                      "ito36"
#define FUNC_FROM36TOI                  "from36toi"
#define FUNC_ISCONVERTEDNAME            "isconvertedname"
#define FUNC_ISMATCHNAME                "ismatchname"
#define FUNC_ISSJIS1                    "isSJIS1"
#define FUNC_ISSJIS2                    "isSJIS2"
#define FUNC_ISDFAMVOL                  "isDFAMVolume"
#define FUNC_PURGEOLDDELETED            "PurgeOldDeletedEntry"
#define FUNC_DFSEXISTCHK                "DFSExistchk"
#define FUNC_NWEXISTCHK                 "NWExistchk"
#define FUNC_ADDTONAMET                 "AddToName_t"
#define FUNC_DELNAMETBYNAME             "DeleteName_tByName"
#define FUNC_SETDELMARKBYNAME           "SetDelMarkByName"
#define FUNC_ENTSYNCHK                  "EntSynChk"
#define FUNC_GETENTRY                   "GetEntry"

