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
 * $Log: dfaclcom.c,v $
 * Revision 1.1.2.3  1996/03/12  03:19:08  takano
 * 	Delete 2byte code.
 * 	[1996/03/12  03:18:20  takano]
 *
 * Revision 1.1.2.2  1996/03/11  14:11:04  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:59:23  marty]
 * 
 * Revision 1.1.2.1  1996/02/29  04:18:05  takano
 * 	Submit DFAM-client test
 * 	[1996/02/29  04:17:06  takano]
 * 
 * Revision 1.1.2.1  1996/02/22  10:49:04  takano
 * 	First Submit
 * 	[1996/02/22  10:48:58  takano]
 * 
 * $EndLog$
 */
#include <malloc.h>
#include "dfacltp.h"
#include "dfaclmsg.h"

NWCONN_HANDLE	nwConnectionHandle;
NWCONN_HANDLE	nwDefaultConnectionHandle;
DWORD			nwNCPExtensionHandle;
NWCCODE			Code;
char			nwNCPExtensionName[] = "DFA_Command_Listen";
char			LOGIN[] = "LGIN";
char			LOGOUT[] = "LGOT";

/*** EnglishComment **********************************************************/
/*  ID   = BOOL initialize()                                                 */
/*                                                                           */
/*  name = Initialization before using NetWare API                           */
/*  func = (1) Initializes NetWare API.                                      */
/*         (2) Gets a connection handle of the default NetWare server.       */
/*                                                                           */
/*  i/o  = nwDefaultConnectionHandle :NWCONN_HANDLE :OUT: connection handle  */
/*                                             of the default NetWare server */
/*                                                                           */
/*  return = TRUE/FALSE                                                      */
/*  note = Call this function only once (when FVT is started).               */
/*                                                                           */
/*  date = September 1, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/
BOOL initialize()
{
	NWCCODE	code;

	code = NWCallsInit(NULL, NULL);

	if (code != 0) {
		printf(KDDS33110, "initialize()", "NWCallsInit", code);
		if (fprintf(log_fp, KDDS33110, "initialize()", "NWCallsInit", code) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "initialize()", "fprintf", Errcode);
		}
		return FALSE;
	}

	code = NWGetDefaultConnectionID(&nwDefaultConnectionHandle);

	if (code != 0) {
		printf(KDDS33110, "initialize()", "NWGetDefaultConnectionID", code);
		if (fprintf(log_fp, KDDS33110, "initialize()", "NWGetDefaultConnectionID", code) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "initialize()", "fprintf", Errcode);
		}
		return FALSE;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID   = BOOL nwLogin(const char* nwserver, const char* nwuser,            */
/*         const char* nwpassword)                                           */
/*                                                                           */
/*  name = NetWare login                                                     */
/*  func = Gets the connection handle of the NetWare server to be logged in, */
/*         then logs in to NetWare.                                          */
/*                                                                           */
/* i/o = nwserver                  :const char*   :INP: NetWare server name  */
/*                                  (If a NULL character string or NULL      */
/*                                   pointer was set to nwserver, FVT        */
/*                                   attempts the NetWare login to a default */
/*                                   NetWare server)                         */
/*       nwuser                    :const char*   :INP: NetWare user name    */
/*       nwpassword                :const char*   :INP: NetWare password     */
/*       nwDefaultConnectionHandle :NWCONN_HANDLE :OUT: connection handle of */
/*                                  the default NetWare server               */
/*       nwConnectionHandle        :NWCONN_HANDLE :OUT: connection handle of */
/*                                  the logged-in NetWare server             */
/*                                                                           */
/*  return = TRUE/FALSE                                                      */
/*  note = none                                                              */
/*                                                                           */
/*  date = September 1, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/
BOOL nwLogin(const char* nwserver, const char* nwuser, const char* nwpassword)
{
	char			server[48], user[48], password[129];
	NWCCODE			code;
	CONNECT_INFO	connectInfo;
	struct {
		int		alreadyConnect : 1;
	} flag;

	if (nwserver == NULL || nwserver[0] == '\0') {
		nwConnectionHandle = nwDefaultConnectionHandle;
		flag.alreadyConnect = ON;
	}
	else {
		strcpy(server, nwserver);
		strupr(server);
		code = NWAttachToFileServer(server, (NWLOCAL_SCOPE)0, &nwConnectionHandle);

		switch (code) {
			case 0x0000:
				flag.alreadyConnect = OFF;
				break;
			case 0x8800:
				code = NWGetConnectionHandle((unsigned char*)server, (NWLOCAL_MODE)0,
								&nwConnectionHandle, NULL);

				if (code != 0) {
					printf(KDDS33110, "nwLogin()", "NWGetConnectionHandle", code);
					if (fprintf(log_fp, KDDS33110, "nwLogin()", "NWGetConnectionHandle", code) < 0){
						Errcode = errno;
						printf(KDDS33107, "DFACLTP.LOG", "nwLogin()", "fprintf", Errcode);
					}
					return FALSE;
				}

				flag.alreadyConnect = ON;
				break;
			default:
				printf(KDDS33110, "nwLogin()", "NWAttachToFileServer", code);
				if (fprintf(log_fp, KDDS33110, "nwLogin()", "NWAttachToFileServer", code) < 0){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "nwLogin()", "fprintf", Errcode);
				}
				return FALSE;
		}
	}

	code = NWGetConnectionStatus(nwConnectionHandle, &connectInfo, sizeof(CONNECT_INFO));

	if (code != 0) {
		printf(KDDS33110, "nwLogin()", "NWGetConnectionStatus", code);
		if (fprintf(log_fp, KDDS33110, "nwLogin()", "NWGetConnectionStatus", code) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "nwLogin()", "fprintf", Errcode);
		}

		if (flag.alreadyConnect == OFF && nwConnectionHandle != nwDefaultConnectionHandle)
			code = NWDetachFromFileServer(nwConnectionHandle);

		return FALSE;
	}

	if ((connectInfo.connectFlags & CONNECTION_LOGGED_IN) != 0) {
		printf(KDDS33110, "nwLogin()", "NWGetConnectionStatus", 0xF000);
		if (fprintf(log_fp, KDDS33110, "nwLogin()", "NWGetConnectionStatus", 0xF000) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "nwLogin()", "fprintf", Errcode);
		}

		if (flag.alreadyConnect == OFF)
			code = NWDetachFromFileServer(nwConnectionHandle);

		return FALSE;
	}

	strcpy(user, nwuser);
	strupr(user);
	strcpy(password, nwpassword);
	strupr(password);

	code = NWLoginToFileServer(nwConnectionHandle, user, OT_USER, password);

	if (code != 0) {
		printf(KDDS33110, "nwLogin()", "NWLoginToFileServer", code);
		if (fprintf(log_fp, KDDS33110, "nwLogin()", "NWLoginToFileServer", code) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "nwLogin()", "fprintf", Errcode);
		}

		if (flag.alreadyConnect == OFF)
			code = NWDetachFromFileServer(nwConnectionHandle);

		return FALSE;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID   = BOOL nwLogout()                                                   */
/*                                                                           */
/*  name = NetWare logout                                                    */
/*  func = Logs out from NetWare, and disconnects the communication with     */
/*         the NetWare server to be logged out.                              */
/*                                                                           */
/* i/o = nwConnectionHandle        :NWCONN_HANDLE :INP: connection handle of */
/*                                  the NetWare server to be logged out      */
/*                                                                           */
/*  return = TRUE/FALSE                                                      */
/*  note = none                                                              */
/*                                                                           */
/*  date = September 1, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/
BOOL nwLogout()
{
	NWNUMBER		connectionCount, maximumConnectionCount;
	NWCONN_HANDLE*	pConnectionList;
	NWCCODE			code;

	code = NWLogoutFromFileServer(nwConnectionHandle);

	NWGetMaximumConnections(&maximumConnectionCount);

	pConnectionList = malloc(sizeof(NWCONN_HANDLE)*maximumConnectionCount);

	if (pConnectionList == 0) {
		printf(KDDS33112, "nwLogout()", "malloc", 0x0000);
		if (fprintf(log_fp, KDDS33112, "nwLogout()", "malloc", 0x0000) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "nwLogout()", "fprintf", Errcode);
		}
		return FALSE;
	}

	code = NWGetConnectionList((NWLOCAL_MODE)0, pConnectionList,
				maximumConnectionCount, &connectionCount);

	if (code != 0) {
		printf(KDDS33110, "nwLogout()", "NWGetConnectionList", code);
		if (fprintf(log_fp, KDDS33110, "nwLogout()", "NWGetConnectionList", code) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "nwLogout()", "fprintf", Errcode);
		}
		free(pConnectionList);
		return FALSE;
	}

	if (connectionCount > 1)
		code = NWDetachFromFileServer(nwConnectionHandle);

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID   = BOOL dceLogin()                                                   */
/*                                                                           */
/*  name = DCE login                                                         */
/*  func = Gets the NCPExtension handle of DFAM Gateway on the NetWare       */
/*         server where the user made a NetWare login, and logs in to DCE.   */
/*                                                                           */
/* i/o = nwConnectionHandle   :NWCONN_HANDLE:INP: connection handle of the   */
/*                                                logged-in NetWare server   */
/*       nwNCPExtensionHandle :DWORD        :OUT: NCPExtension handle of DFAM*/
/*                                                                           */
/*  return = TRUE/FALSE                                                      */
/*  note = none                                                              */
/*                                                                           */
/*  date = September 1, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/
BOOL dceLogin()
{
	struct LoginSendFrame {
		unsigned char	command[4];
	} sendBuffer;

	struct LoginReceiveFrame {
		unsigned char	command[4];
		unsigned short	result;
		unsigned short	dceUserNameLength;
		unsigned char	dceUserName[1024];
	} receiveBuffer;

	NWCCODE	code;
	WORD	sendLength, receiveLength;

	code = NWGetNCPExtensionInfoByName(nwConnectionHandle, nwNCPExtensionName,
							&nwNCPExtensionHandle, NULL, NULL, NULL, NULL);

	if (code != 0) {
		printf(KDDS33110, "dceLogin()", "NWGetNCPExtensionInfoByName", 0xF002);
		if (fprintf(log_fp, KDDS33110, "dceLogin()", "NWGetNCPExtensionInfoByName", 0xF002) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "dceLogin()", "fprintf", Errcode);
		}
		return FALSE;
	}

	memcpy((void*)(sendBuffer.command), (void*)LOGIN, 4);
	sendLength = sizeof(struct LoginSendFrame);
	receiveLength = sizeof(struct LoginReceiveFrame);

	code = NWNCPExtensionRequest(nwConnectionHandle, nwNCPExtensionHandle,
							&sendBuffer, sendLength, &receiveBuffer, &receiveLength);

	if (code != 0) {
		printf(KDDS33110, "dceLogin()", "NWNCPExtensionRequest", code);
		if (fprintf(log_fp, KDDS33110, "dceLogin()", "NWNCPExtensionRequest", code) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "dceLogin()", "fprintf", Errcode);
		}
		return FALSE;
	}

	if (receiveLength != sizeof(struct LoginReceiveFrame) ||
		memcmp((void*)(receiveBuffer.command), (void*)LOGIN, 4) != 0) {
		printf(KDDS33110, "dceLogin()", "LGIN", 0x0100);
		if (fprintf(log_fp, KDDS33110, "dceLogin()", "LGIN", 0x0100) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "dceLogin()", "fprintf", Errcode);
		}
		return FALSE;
	}

	if (receiveBuffer.result != 0) {
		printf(KDDS33110, "dceLogin()", "LGIN", receiveBuffer.result);
		if (fprintf(log_fp, KDDS33110, "dceLogin()", "LGIN", receiveBuffer.result) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "dceLogin()", "fprintf", Errcode);
		}
		return FALSE;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID   = BOOL dceLogout()                                                  */
/*                                                                           */
/*  name = DCE logout                                                        */
/*  func = Logs out from DCE.                                                */
/*                                                                           */
/* i/o = nwConnectionHandle   :NWCONN_HANDLE:INP: connection handle of the   */
/*                                                logged-in NetWare server   */
/*       nwNCPExtensionHandle :DWORD        :INP: NCPExtension handle of DFAM*/
/*                                                                           */
/*  return = TRUE/FALSE                                                      */
/*  note = none                                                              */
/*                                                                           */
/*  date = September 1, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/
BOOL dceLogout()
{
	struct LogoutSendFrame {
		unsigned char	command[4];
	} sendBuffer;

	struct LogoutReceiveFrame {
		unsigned char	command[4];
		unsigned short	result;
		unsigned short	dceUserNameLength;
		unsigned char	dceUserName[1024];
	} receiveBuffer;

	NWCCODE	code;
	WORD	sendLength, receiveLength;

	memcpy((void*)(sendBuffer.command), (void*)LOGOUT, 4);
	sendLength = sizeof(struct LogoutSendFrame);
	receiveLength = sizeof(struct LogoutReceiveFrame);

	code = NWNCPExtensionRequest(nwConnectionHandle, nwNCPExtensionHandle,
							&sendBuffer, sendLength, &receiveBuffer, &receiveLength);

	if (code != 0) {
		printf(KDDS33110, "dceLogout()", "NWNCPExtensionRequest", code);
		if (fprintf(log_fp, KDDS33110, "dceLogout()", "NWNCPExtensionRequest", code) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "dceLogout()", "fprintf", Errcode);
		}
		return FALSE;
	}

	if (receiveLength != sizeof(struct LogoutReceiveFrame) ||
		memcmp((void*)(receiveBuffer.command), (void*)LOGOUT, 4)) {
		printf(KDDS33110, "dceLogout()", "LGOT", 0x0100);
		if (fprintf(log_fp, KDDS33110, "dceLogout()", "LGOT", 0x0100) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "dceLogout()", "fprintf", Errcode);
		}
		return FALSE;
	}

	if (receiveBuffer.result != 0) {
		printf(KDDS33110, "dceLogout()", "LGOT", receiveBuffer.result);
		if (fprintf(log_fp, KDDS33110, "dceLogout()", "LGOT", receiveBuffer.result) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "dceLogout()", "fprintf", Errcode);
		}
		return FALSE;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID   = BOOL mapping(const int drive, const char* dospath)                */
/*                                                                           */
/*  name = Mapping                                                           */
/*  func = Maps the path on the NetWare server to the network drive.  Then   */
/*         relocates the current drive to "drive," and the current directory */
/*         to the root of "drive."                                           */
/*                                                                           */
/*  i/o  = drive   :const int   :INP: a drive number to be mapped (A=1,B=2,..*/
/*                                    ..,Y=25,Z=26)                          */
/*         dospath :const char *:INP: the root path to be mapped             */
/*                 (dospath  can be either a MS-DOS full path starting from  */
/*                  a MS-DOS drive, or a NetWare full path starting from a   */
/*                  NetWare server.  If a NULL character string or NULL      */
/*                  pointer was set to dospath, FVT attempts to map the      */
/*                  current directory to the drive)                          */
/*                                                                           */
/*  return = TRUE/FALSE                                                      */
/*  note = none                                                              */
/*                                                                           */
/*  date = September 1, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/
BOOL mapping(const int drive, const char* dospath)
{
	static char		directory[] = "\\";
	char			path[80], nwVolumePath[304], *pbuf;
	NWCCODE			code;
	NWCONN_HANDLE	hConnection;
	NWDIR_HANDLE	hDirectory;
	WORD			status;
	int				ret, errno_save;

	if (dospath == NULL || dospath[0] == '\0') {
		pbuf = _getcwd(path, sizeof(path));
		errno_save = errno;

		if (pbuf == NULL) {
			printf(KDDS33113, "mapping()", "_getcwd", errno_save);
			sprintf(log_buf, KDDS33113, "mapping()", "_getcwd", errno_save);
			return FALSE;
		}
	}
	else
		strcpy (path, dospath);

	strupr(path);
	code = NWParseNetWarePath(path, &hConnection, &hDirectory, nwVolumePath);

	if (code != 0) {
		printf(KDDS33110, "mapping()", "NWParseNetWarePath", code);
		sprintf(log_buf, KDDS33110, "mapping()", "NWParseNetWarePath", code);
		return FALSE;
	}

	if (hConnection != nwConnectionHandle) {
		printf(KDDS33111, "mapping()", "NWParseNetWarePath", 0xF001);
		sprintf(log_buf, KDDS33111, "mapping()", "NWParseNetWarePath", 0xF001);
		return FALSE;
	}

	code = NWGetDriveStatus(drive, NW_FORMAT_SERVER_VOLUME, &status,
					NULL, NULL, NULL, NULL);

	switch (code) {
		case 0x0000:
			break;
		case 0x883C:
			status = NW_NETWORK_DRIVE;
			break;
		default:
			printf(KDDS33113, "mapping()", "NWGetDriveStatus", code);
			sprintf(log_buf, KDDS33113, "mapping()", "NWGetDriveStatus", code);
			return FALSE;
	}

	if (status != NW_UNMAPPED_DRIVE) {
		code = NWDeleteDriveBase(drive, (NWLOCAL_SCOPE)0);

		if (code != 0) {
			printf(KDDS33113, "mapping()", "NWDeleteDriveBase", code);
			sprintf(log_buf, KDDS33113, "mapping()", "NWDeleteDriveBase", code);
			return FALSE;
		}
	}

	Code = NWSetDriveBase(drive, nwConnectionHandle, 0, nwVolumePath, 0);

	if (Code == 0x8803){
		Errcode = -1;
		return TRUE;
	}else if (Code != 0) {
		printf(KDDS33110, "mapping()", "NWSetDriveBase", Code);
		sprintf(log_buf, KDDS33110, "mapping()", "NWSetDriveBase", Code);
		return FALSE;
	}

	ret = _chdrive(drive);
	errno_save = errno;

	if (ret != 0) {
		printf(KDDS33113, "mapping()", "_chdrive", errno_save);
		sprintf(log_buf, KDDS33113, "mapping()", "_chdrive", errno_save);
		return FALSE;
	}

	ret = _chdir(directory);
	errno_save = errno;

	if (ret != 0) {
		printf(KDDS33113, "mapping()", "_chdir", errno_save);
		sprintf(log_buf, KDDS33113, "mapping()", "_chdir", errno_save);
		return FALSE;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID   = BOOL comopen(const char* nwserver, const char* nwuser,            */
/*         const char* nwpassword, const char* nwvolume)                     */
/*                                                                           */
/*  name = Pre-processing for the Client test suite                          */
/*  func = (1) Logs in to the designated NetWare server.                     */
/*         (2) Logs in to DCE via DFAM Gateway on the designated NetWare     */
/*             server.                                                       */
/*         (3) Maps the designated NetWare volume (on the designated NetWare */
/*             server) to U drive.                                           */
/*         (4) Maps PUBLIC directory of SYS volume on the NetWare server to  */
/*             T drive.                                                      */
/*                                                                           */
/* i/o = nwserver  :const char* :INP: NetWare server name                    */
/*                                  (If a NULL character string or NULL      */
/*                                   pointer was set to nwserver, FVT        */
/*                                   attempts the NetWare login to a default */
/*                                   NetWare server)                         */
/*        nwuser   :const char* :INP: NetWare user name                      */
/*        nwpassword:const char*:INP:NetWare password                        */
/*        nwvolume :const char* :INP: root path to be mapped to U drive      */
/*                 (nwvolume can be either a MS-DOS full path starting from  */
/*                  a MS-DOS drive, or a NetWare full path starting from a   */
/*                  NetWare server.  If a NULL character string or NULL      */
/*                  pointer was set to nwvolume, FVT attempts to map the     */
/*                  current directory of the current drive to Z drive)       */
/*                                                                           */
/*  return = TRUE/FALSE                                                      */
/*  note = none                                                              */
/*                                                                           */
/*  date = September 1, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/
BOOL comopen(const char* nwserver, const char* nwuser, const char* nwpassword, const char* nwvolume)
{
	BOOL		result;
	const int	treeDrive = 21;
	const int	publicDrive = 20;
	static char nwpublic[] = "SYS:\\PUBLIC";
	static char pathdelimiter[] = "\\";
	static char volumedelimiter[] = ":";
	int			position;
	char		path[70];

	result = initialize();
	if (result == FALSE) {
		return FALSE;
	}

	result = nwLogin(nwserver, nwuser, nwpassword);
	if (result == FALSE) {
		return FALSE;
	}

	result = dceLogin();
	if (result == FALSE) {
		result = nwLogout();
		return FALSE;
	}

	strcpy(path, nwserver);
	strcat(path, pathdelimiter);
	position = strlen(path);

	Errcode = 0;

	strcpy(path+position, nwvolume);
	strcat(path, volumedelimiter);
	strcat(path, pathdelimiter);
	result = mapping(treeDrive, path);
	if (result == FALSE) {
		if (fprintf(log_fp, log_buf) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "comopen()", "fprintf", Errcode);
		}
		result = nwLogout();
		return FALSE;
	}else if (Errcode != 0){


		printf(KDDS33110, "mapping()", "NWSetDriveBase", Code);
		fprintf(log_fp, KDDS33110, "mapping()", "NWSetDriveBase", Code);

		result = nwLogout();
		return FALSE;
	}

	strcpy(path+position, nwpublic);
	result = mapping(publicDrive, path);
	if (result == FALSE) {
		if (fprintf(log_fp, log_buf) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "comopen()", "fprintf", Errcode);
		}
		result = nwLogout();
		return FALSE;
	}else if (Errcode != 0){


		printf(KDDS33110, "mapping()", "NWSetDriveBase", Code);
		fprintf(log_fp, KDDS33110, "mapping()", "NWSetDriveBase", Code);

		result = nwLogout();
		return FALSE;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID   = BOOL comclose()                                                   */
/*                                                                           */
/*  name = Post-processing for the Client test suite                         */
/*  func = (1) Logs out from DCE via DFAM Gateway.                           */
/*         (2) Logs out from NetWare server.                                 */
/*                                                                           */
/* i/o = none                                                                */
/*                                                                           */
/*  return = TRUE/FALSE                                                      */
/*  note = none                                                              */
/*                                                                           */
/*  date = September 1, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/
BOOL comclose()
{
	BOOL	result;

	dceLogout();

	result = nwLogout();
	if (result == FALSE) {
		return FALSE;
	}

	return TRUE;
}
