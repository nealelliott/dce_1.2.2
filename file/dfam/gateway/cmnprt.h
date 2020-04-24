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
 * $Log: cmnprt.h,v $
 * Revision 1.1.2.3  1996/03/22  07:01:37  takano
 * 	Fixed the confliction between the Directory Synchronization and the File-Access.
 * 	[1996/03/22  06:58:52  takano]
 *
 * Revision 1.1.2.2  1996/03/11  14:07:06  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:36  marty]
 * 
 * Revision 1.1.2.1  1996/02/21  07:11:34  takano
 * 	First Submit
 * 	[1996/02/21  07:11:29  takano]
 * 
 * $EndLog$
 */
/*EnglishComment**************************************************************/
/* ID	= CMDPRT.H										                     */
/*                                                                           */
/* date = September 27, 1995  by Yoshiaki Okada                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*EnglishComment**************************************************************/

#ifndef	CMDPRT_H

extern	struct GTGroup_t *GetGTGroupTable(LONG uiConnNum);

extern	struct OpenFile_t 	*SearchOpenFile_t_cb(struct GTGroup_t	*pCur_TG,
												 xOpenFileCallBackStruct *pofcbs);
extern	struct OpenFile_t 	*SearchOpenFile_t_cbCO(struct GTGroup_t	*pCur_TG,
												 xCreateAndOpenCallBackStruct *pofcbs);

extern	struct OpenFile_t 	*SearchOpenFile_t_fd(struct GTGroup_t	*pCur_TG,
											 	 LONG		iNW_fd);

extern	short 	SearchrequestedAccessRights(struct GTGroup_t	*pCur_TG,
											char	*pszNWpath,
											int		*iRecAcsRgt_dmy);

extern	int		DelOpenFile_t(struct OpenFile_t *pOF,struct GTGroup_t *pCur_TG);

extern	struct GCopyBack_t *SearchCopyBack_t(BYTE *pszNWFullpath);

extern	int 	AddCopyBack_t(BYTE *pszNWFullpath, LONG uiReqdAcsRights);

extern	int		DelCopyBack_t(BYTE *pszNWFullpath);

extern	int DecCopyBackCounter(BYTE *pszNWFullpath, LONG uiReqdAcsRights);

extern	int		MapNWFullpath(	LONG	uiVolnum,
								LONG 	uiDirBase,
								LONG 	uiPathCompCnt,
								BYTE 	*psPathString,
								BYTE 	*pszNWFullpath,
								BYTE	*pszNWPath,
								int		*piWildCard_flg);

extern	int	IsWildChar(BYTE *pChar);

extern 	int FileCopyFromDFS(BYTE				*pszNWFullpath,
							BYTE				*pszDFSFullpath,
							struct	GTGroup_t	*pCur_TG,
							int					iO_mode,
							int					iS_mode,
							int					*iNWerrno);

extern	int FileCopyFromNW(	BYTE				*pszNWFullpath,
							long				iDFSHandle,
							struct	GTGroup_t	*pCur_TG);

extern	void	FAR_Terminate(void);
extern	void	Terminate_Grp(struct	GTGroup_t 	*pCur);
extern	int GW_FileBackUP(struct OpenFile_t 	*pOF );
extern	int MakeSparseFile(struct OpenFile_t *pOF);

extern	void	ConvertErrno(int	AcsMode,
							 int	iDFSerrno,
							 int	*iNWerrno);
extern	void					 AddAccessList(struct	FileAccessList	*pAcs_t);
extern	void					 DelAccessList(LONG uiConNo,char *pPathName_b,char *pPathName_a);
extern	void					 DelAccessListConNoAll(LONG	uiConNo);
extern	struct FileAccessList	*SerchAccessList(LONG uiConNo, LONG uiTaskNum, int iAccessType);
extern	void DelayCutoff( struct	GTGroup_t	*pTGroup );
#define	CMDPRT_H
#endif
