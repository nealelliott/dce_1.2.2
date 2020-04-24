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
 * $Log: dfamprt.h,v $
 * Revision 1.1.2.3  1996/03/22  07:02:27  takano
 * 	Fixed the confliction between the Directory Synchronization and the File-Access.
 * 	[1996/03/22  06:59:27  takano]
 *
 * Revision 1.1.2.2  1996/03/11  14:07:35  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:57:01  marty]
 * 
 * Revision 1.1.2.1  1996/02/21  07:25:18  takano
 * 	First Submit
 * 	[1996/02/21  07:25:12  takano]
 * 
 * $EndLog$
 */
/*EnglishComment**************************************************************
 * DFAMPRT.H                                                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/

#ifndef DFAMPRT_H
#define DFAMPRT_H

int			AccessListTableDel(struct FileAccessList *pAcs_t);
int                     AddTGrpID(LONG, struct GTGroup_t **);
struct GName_t   *      AddtoName_t(BYTE *);
int						AddGWVolTrustee(long);
extern int              AgentDisConnectReq(void *pComTbl);
extern int              AgtConnectReq(struct GAgent_t *pAgent,unsigned short ushConAssort,void *pComTbl);
extern  void            CopyBack( struct OpenFile_t *pOF );
extern int              DCELogFileFlush(void);
int                     DCE_Login( void *, BYTE *, BYTE *, int *);
void                    DeleteAllName_t( struct GName_t * );
void                    DeleteFromGVolume_t( struct GVolume_t *);
void                    DeleteName_tByName( BYTE * );
void					DelGWVolTrustee(long, char *);
int                     DelTGrpID(LONG);
int						DFARASdump( void );
int						DFARASlog( void );
int						DFARASmessage( void );
int						DFARAStrace( void );
extern int              DFSchownReq(char *path,char *owner,char *group,int *DFSErrno,void *pComTbl);
extern int              DFSclosedirReq(DDIR *dirp,int *DFSErrno,void *pComTbl);
extern int              DFScloseReq(int fildes,int *DFSErrno,void *pComTbl);
extern int              DFSCpbkFirstWriteReq(int fildes,void *buf,dsize_t nbyte,int *DFSErrno,void *pComTbl);
extern int              DFSCpbkMiddleWriteReq(int fildes,void *buf,dsize_t nbyte,int *DFSErrno,void *pComTbl);
extern int              DFSCpbkLastWriteReq(int fildes,int *DFSErrno,void *pComTbl);
extern int              DFSCpFirstReadReq(int fildes,dsize_t nbyte,int *DFSErrno,void *pComTbl);
extern int              DFSCpMiddleReadReq(void *buf,int *DFSErrno,void *pComTbl);
extern int              DFSCpLastReadReq(int *DFSErrno,void *pComTbl);
extern int              DFScreatReq(char *path,dmode_t mode,int *DFSErrno,void *pComTbl);
extern int              DFSdelacl_syscallReq(char *pathname,unsigned short option,char *acl,int *length,int *DFSErrno,void *pComTbl);
extern int              DFSdirsyncReq(dtime_t time,char *mpname,int *DFSErrno,struct ddirsync *pDsync,void *pComTbl);
extern int              DFSgetacl_syscallReq(char *pathname,unsigned short option,char *acl,int *length,int *DFSErrno,void *pComTbl);
extern long             DFSLinGetIdReq(char *pcIndata,long iIndataLen,char *pcOutdata,int *DFSErrno,void *pComTbl);
extern long             DFSLinGetPasswdReq(char *pcData,long iDataLen,int *DFSErrno,void *pComTbl);
extern long             DFSLinResetKeyReq(int *DFSErrno,void *pComTbl);
extern long             DFSLoutReq(int *DFSErrno,void *pComTbl);
extern int              DFSlseekReq(int fildes,doff_t lseekp,dsize_t length,int *DFSErrno,void *pComTbl);
extern int              DFSmkdirReq(char *path,dmode_t mode,int *DFSErrno,void *pComTbl);
extern int              DFSopendirReq(char *dirname,DDIR *dirp,int *DFSErrno,void *pComTbl);
extern int              DFSopenReq(char *path,int oflag,long smode,int *DFSErrno,void *pComTbl);
extern dssize_t         DFSreadReq(int fildes,void *buf,dsize_t nbyte,int *DFSErrno,void *pComTbl);
extern int              DFSreadstatReq(DDIR *dirp,short statmode,struct ddirent *direntry,int *DFSErrno,void *pComTbl);
extern int              DFSremoveReq(char *path,int *DFSErrno,void *pComTbl);
extern int              DFSrenameReq(char *source,char *target,int *DFSErrno,void *pComTbl);
extern unsigned int     DFSrightsReq(char *pathname,int *DFSErrno,void *pComTbl);
extern int              DFSrmdirReq(char *path,int *DFSErrno,void *pComTbl);
extern int              DFSsetacl_syscallReq(char *pathname,unsigned short option,char *acl,int *length,int *DFSErrno,void *pComTbl);
extern int              DFSstatReq(char *path,struct dstat *buf,int *DFSErrno,void *pComTbl);
extern dtime_t          DFStimeReq(dtime_t *tloc,int *DFSErrno,void *pComTbl);
extern int              DFSutimeReq(char *path,const struct dutimbuf *times,int *DFSErrno,void *pComTbl);
extern dssize_t         DFSwriteReq(int fildes,void *buf,dsize_t nbyte,int *DFSErrno,void *pComTbl);
int                     DMmain( void );
struct DMAccessFile	*DMAddAccessFileList( char *pszPath, int ThID );
int			DMDeleteAccessFileList( int ThID );
int			DMFreeAccessFileList( void );
int			DMCheckAccessFileList( char *pszPath );
char *                  D_strchr( char *, int );
size_t                  D_strcspn( char *, char *);
char *                  D_strlwr( char *);
char *                  D_strpbrk ( char *, char * );
char *                  D_strrchr( char *, int);
char *                  D_strupr( char *);
int                     EraseDFAMDir ( char *, int *, int * );
int                     EraseDFAMFile ( char *, int * );
void                    FAR_Terminate(void);
int						fdDTlist_sort( struct TableKeep *, unsigned long *,	unsigned char **, unsigned char	*, short);
void                    fnAdminThread();
void                    fnChgdate(struct GTGroup_t *,int,unsigned char *,unsigned long,unsigned char *,unsigned long *);
void                    fnChgowner(struct GTGroup_t *,int,unsigned char *,unsigned long,unsigned char *,unsigned long *);
void                    fnChkvol(unsigned char *,unsigned long,unsigned char *,unsigned long *);
BYTE                    fnCommandListen( struct NCPExtensionClient *, void *, LONG, void *, LONG *);
void					fnCommnErrLog( unsigned char * );
void                    fnConnDown( LONG, LONG );
void                    fnCreatRandKey( LONG * );
void                    fnDelAllTask(struct GTGroup_t *);
void                    fnDelTask(struct GTGroup_t *,struct TableKeep *);
void                    fnDGrant(struct GTGroup_t *,int,unsigned char *,unsigned long,unsigned char *,unsigned long *);
void                    fnDGrantContinue(struct TableKeep *,unsigned char *,unsigned char *,unsigned long *);
void                    fnDGrantNew(struct GTGroup_t *,struct TableKeep *,unsigned char *,unsigned char *,unsigned long *);
void					fnDGrant_err(struct TableKeep *,unsigned long *,int);
int						fnDGrant_main(struct GTGroup_t *,struct TableKeep *,char *,char *,unsigned short,unsigned short,unsigned long,struct dstat *,char *,struct RetGrantData *);
int                     fnDGrant_op(struct GTGroup_t *,struct TableKeep *,char *,struct dgrant_data *,unsigned char *,unsigned long *);
int                     fnDGrant_set(struct GTGroup_t *,struct TableKeep *,char *,char *,unsigned char *,unsigned long *,unsigned short,unsigned short,unsigned long,struct dstat *,char *);
void                    fnDListdir(struct GTGroup_t *,int,unsigned char *,unsigned long,unsigned char *,unsigned long * );
void                    fnDListdirContinue(struct GTGroup_t *,struct TableKeep *,unsigned char *,unsigned char *,unsigned long *);
void                    fnDListdirNew(struct GTGroup_t *,struct TableKeep *,unsigned char *,unsigned char *,unsigned long *);
int                     fnDListdir_dirwd(struct GTGroup_t *,struct TableKeep *,struct dlistdir_data *,unsigned char *,unsigned long *);
void                    fnDListdir_err( struct TableKeep *,unsigned long *,int);
int                     fnDListdir_main(struct GTGroup_t *,char *,struct RetListdirData *,struct dlistdir_data *);
int                     fnDListdir_op(struct GTGroup_t *,struct TableKeep *,char *,struct dlistdir_data *,unsigned char *,unsigned long *);
void                    fnDListdir_wd(struct GTGroup_t *,struct TableKeep *,char *,unsigned char *,unsigned long *,struct dlistdir_data *);
void                    fnDNdir(struct GTGroup_t *,int,unsigned char *,unsigned long,unsigned char *,unsigned long *);
void                    fnDNdirContinue(struct GTGroup_t *,struct TableKeep *,unsigned char *,unsigned long,unsigned char *,unsigned long *);
void                    fnDNdirNew(struct GTGroup_t *,struct TableKeep *,unsigned char *,unsigned long,unsigned char *,unsigned long *);
int                     fnDNdir_dirwd(char *,struct GTGroup_t *,struct TableKeep *,unsigned char *,unsigned long *);
int                     fnDNdir_main(struct GTGroup_t *,char *,struct RetNdirData *);
int                     fnDNdir_op(struct GTGroup_t *,struct TableKeep *,char *,unsigned char *,unsigned long *);
void                    fnDNdir_wd(struct GTGroup_t *,unsigned char *,struct TableKeep *,unsigned char *,unsigned long *);
void                    fnDRemove(struct GTGroup_t *,int,unsigned char *,unsigned long,unsigned char *,unsigned long *);
void                    fnDRemoveContinue(struct TableKeep *,unsigned char *,unsigned char *,unsigned long *);
void                    fnDRemoveNew(struct GTGroup_t *,struct TableKeep *,unsigned char *,unsigned char *,unsigned long *);
void					fnDRemove_err(struct TableKeep *,unsigned long *,int);
int                     fnDRemove_op(struct GTGroup_t *,struct TableKeep *,char *,struct dremove_data *,unsigned char *,unsigned long *);
int                     fnDRemove_set(struct GTGroup_t *,struct TableKeep *,char *,unsigned short,char *,unsigned char *,unsigned long *);
void                    fnDRevoke(struct GTGroup_t *,int,unsigned char *,unsigned long,unsigned char *,unsigned long *);
void                    fnDRevokeContinue(struct TableKeep *,unsigned char *,unsigned char *,unsigned long *);
void                    fnDRevokeNew(struct GTGroup_t *,struct TableKeep *,unsigned char *,unsigned char *,unsigned long *);
void					fnDRevoke_err(struct TableKeep *,unsigned long *,int);
int	        	        fnDRevoke_main(struct GTGroup_t *,struct TableKeep *,char *,char *,unsigned short,unsigned short,unsigned long,struct dstat *,char *,struct RetRevokeData *);
int	                    fnDRevoke_op(struct GTGroup_t *,struct TableKeep *,char *,struct drevoke_data *,unsigned char *,unsigned long *);
int	        	        fnDRevoke_set(struct GTGroup_t *,struct TableKeep *,char *,char *,unsigned char *,unsigned long *,unsigned short,unsigned short,unsigned long,struct dstat *,char *);
void                    fnDRights(struct GTGroup_t *,int,unsigned char *,unsigned long,unsigned char *,unsigned long *);
void                    fnDTlist(struct GTGroup_t *,int,unsigned char *,unsigned long,unsigned char *,unsigned long *);
void                    fnDTlistContinue(struct GTGroup_t *,struct TableKeep *,unsigned char *,unsigned long,unsigned char *,unsigned long *);
void                    fnDTlistNew(struct GTGroup_t *,struct TableKeep *,unsigned char *,unsigned char *,unsigned long *);
void                    fnDtlist_err(struct TableKeep *, unsigned long *,int);
int                     fnDTlist_main(struct GTGroup_t *,struct TableKeep *,char *,struct RetTlistData *,unsigned char *,char *);
int                     fnDTlist_sort(struct TableKepp *,unsigned long *,unsigned char **,unsigned char *,short);
unsigned short          fnErrChange(int, struct GTGroup_t *);
void                    fnExec_Filer(struct GTGroup_t *,int,unsigned char *, unsigned long,unsigned char *,unsigned long *);
void                    fnGetDecryptData( LONG *, BYTE *, int, BYTE * );
int                     fnGetMasterKey( BYTE *, LONG * );
int                     fnGetTrustee(struct GTGroup_t *,struct TableKeep *,char *,struct dtlist_data *,unsigned char *,unsigned long *);
int                     fnKenriBitmap(unsigned char *,WORD *);
int                     fnKenriMap(WORD,unsigned char *);
void                    fnLoginThread(void *);
LONG                    fnLongSwap( LONG );
unsigned short          fnNW_change_errno(int);
void					fnResumeThread(struct GTGroup_t *, int);
void                    fnSetEncryptData( LONG *, BYTE *, int, BYTE * );
int                     fnSetMasterKey( BYTE *, LONG * );
int                     fnTrustee_op(struct GTGroup_t *,struct TableKeep *,char *,char *,unsigned char *,unsigned long *);
void                    fnWatchDogThread();
void                    FreeGlobalResource(void);
void					Free_Task(struct TableKeep *);
void                    FreeVolumeTableMember(struct GVolume_t *);
int                     GetDCEPassword(char *, char *);
int                     GetDCEUserName(char *, char *);
int                     GetDFAMUserName(char *, char *);
int                     GetDFSPath (BYTE *, BYTE *, int);
int                     GetEntry(char *, char *, char **);
int                     GetNWPath (BYTE *, BYTE *, BYTE *, struct DFSFakeDirEnt *);
struct GTGroup_t *      GetTGrp(LONG);
int                     GW_Clear_GMemTbl( struct GMemManage_t *, int );
int                     GW_DCEGroup_Map(char *,char *);
int                     GW_DFAMGroup_Map(char *,char *);
int                     GW_free( void *,int );
void                    GW_free_all( void );
void *                  GW_malloc( size_t );
int                     GW_Message( int, int* , int , char*,...);
int                     GW_Rights_Map(struct RightsMapping *);
int                     GW_tsh_dump( int iMode );
int                     GW_tsh_functrace( char *, int, int );
int                     GW_tsh_log ( int , char * );
int                     GW_tsh_trace( void *, size_t, int, int, int, char * );
int                     isDevice( BYTE * );
int                     isDFAMVolume( BYTE *, struct GVolume_t ** );
int                     isdigits( char *);
void                    MainCloseSemaphore( LONG * );
void                    MakeMasterKey( BYTE * , BYTE * );
void                    NCP_DCELogin(LONG, struct LGINRecvFrame *, LONG *);
void                    NCP_DCELoginChk(LONG, char *, struct LCHKRecvFrame *, LONG *);
void                    NCP_DCELogout(struct GTGroup_t *, struct LGOTRecvFrame *, LONG *);
int                     NCP_DCELogoutDown(struct GTGroup_t *);
void                    NCP_DCENewPasswd(LONG, struct PWDNSendFrame *, LONG , struct PWDNRecvFrame *, LONG *);
void                    NCP_DCEOldPasswd(LONG, struct PWDOSendFrame *, LONG , struct PWDORecvFrame *, LONG *);
void                    NCP_GetDCEPassword(BYTE *, unsigned short, BYTE *, BYTE *);
void                    NCP_LogComment(LONG, char *);
extern  int             Post_Close(CloseFileCallBackStruct *pcfcbs, LONG uiNWerrno_result);
extern  int             Post_Createdir( CreateDirCallBackStruct *pcdcbs, LONG uiNWerrno_result);
extern  int             Post_CreateOpen( xCreateAndOpenCallBackStruct *pofcbs, LONG uiNWerrno_result );
extern  int             Post_Deletedir( DeleteDirCallBackStruct *pddcbs, LONG uiNWerrno_result);
extern  int             Post_Erase( EraseFileCallBackStruct *pefcbs, LONG uiNWerrno_result);
extern  int             Post_Open( xOpenFileCallBackStruct *pofcbs, LONG uiNWerrno_result );
extern  int             Post_Rename_Move( RenameMoveEntryCallBackStruct *prmecbs, LONG uiNWerrno_result);
extern  int             Pre_Createdir( CreateDirCallBackStruct *pcdcbs);
extern  int             Pre_CreateOpen( xCreateAndOpenCallBackStruct *pcaocbs);
extern  int             Pre_Deletedir( DeleteDirCallBackStruct *pddcbs);
extern  int             Pre_Erase( EraseFileCallBackStruct *pefcbs);
extern  int             Pre_Open( xOpenFileCallBackStruct *pofcbs);
extern  int             Pre_Rename_Move( RenameMoveEntryCallBackStruct *prmecbs);
void                    PurgeOldDeletedEntry(BYTE *);
int                     RangeCheck( char *, int, int);
void                    RemoveHooks(void);
int                     RestoreName_t(struct GName_t * );
void                    ResumeAllUserThread(int);
int                     SetDelmarkByName( BYTE *);
void                    StrCut( char *, size_t);
extern  void            Timesync(void *pCur_TG);
extern int              WatchDogStatRecvReq(unsigned long *puiInfo,size_t *puiMsgSize,char *pcMsg,int *piErrno,void *pComTbl);



#endif
