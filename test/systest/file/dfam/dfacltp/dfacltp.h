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
 * $Log: dfacltp.h,v $
 * Revision 1.1.2.2  1996/03/11  14:11:13  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:59:26  marty]
 *
 * Revision 1.1.2.1  1996/02/29  04:24:07  takano
 * 	Submit DFAM-client test
 * 	[1996/02/29  04:23:53  takano]
 * 
 * Revision 1.1.2.1  1996/02/22  10:54:55  takano
 * 	First Submit
 * 	[1996/02/22  10:54:51  takano]
 * 
 * $EndLog$
 */
/*****************************************************************************/
/* Test Suite Client Header File                                               */
/*				                                                             */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>	 
#include <process.h>
#include <direct.h>
#include <io.h>
#include <fcntl.h>
#include <errno.h>
#include <time.h>
#include <ctype.h>
#include <sys\stat.h>
#include <sys\types.h>
#include <sys\utime.h>
#include "nwcalls.h"
#include "nwncpext.h"

#define LINE_MAX 150
#define KEYWORD 11

#define OFF 0
#define ON 1

#define TRUE 1
#define FALSE 0
#define LOGERR 2

#define BAT 0
#define TXT 1

struct {
		int FvtFile_flg    : 1;
		int NWUser_flg     : 1;
		int NWPassword_flg : 1;
		int NWServer_flg   : 1;
		int GWVolume_flg   : 1;
		int ErrorMode_flg  : 1;
       } Byte;

typedef int BOOL;

int		Errcode;
char	log_buf[LINE_MAX];
FILE	*log_fp;

/*** dfacltp.c ***/
BOOL LOG_Open();
BOOL CFG_Read();
int  FVT_Read();
void LOG_Close();

int  Test(char *fvt_buf);
int  FpFh_Free();
BOOL BAT_Verify(char *pcl_name, char *ans_name);
BOOL Pcl_Read(char *pcl_name, char *ans_name);
BOOL Disp(char *prm_p, int pcl_line);
BOOL API_Search(char *pcl_buf, int pcl_line);

BOOL T_Open(char *prm_p, int pcl_line);
BOOL T_Close(char *prm_p, int pcl_line);
BOOL T_Read(char *prm_p, int pcl_line);
BOOL T_Write(char *prm_p, int pcl_line);
BOOL T_Unlink(char *prm_p, int pcl_line);
BOOL T_Mkdir(char *prm_p, int pcl_line);
BOOL T_Rmdir(char *prm_p, int pcl_line);
BOOL T_Chdir(char *prm_p, int pcl_line);
BOOL T_Rename(char *prm_p, int pcl_line);
BOOL T_Stat(char *prm_p, int pcl_line);
BOOL T_Utime(char *prm_p, int pcl_line);
BOOL T_Fopen(char *prm_p, int pcl_line);
BOOL T_Fclose(char *prm_p, int pcl_line);
BOOL T_Fgetc(char *prm_p, int pcl_line);
BOOL T_Fgets(char *prm_p, int pcl_line);
BOOL T_Fread(char *prm_p, int pcl_line);
BOOL T_Fputc(char *prm_p, int pcl_line);
BOOL T_Fputs(char *prm_p, int pcl_line);
BOOL T_Fwrite(char *prm_p, int pcl_line);
BOOL T_Map();

void Time_Get();

/*** dfaclcom.c ***/
BOOL initialize();
BOOL nwLogin(const char* nwserver, const char* nwuser, const char* nwpassword);
BOOL nwLogout();
BOOL dceLogin();
BOOL dceLogout();
BOOL mapping(const int drive, const char* nwvolume);
BOOL comopen(const char* nwserver, const char* nwuser, const char* nwpassword, const char* nwvolume);
BOOL comclose();
