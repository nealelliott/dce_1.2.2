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
 * $Log: dfacltp.c,v $
 * Revision 1.1.2.2  1996/03/11  14:11:09  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:59:25  marty]
 *
 * Revision 1.1.2.1  1996/02/29  04:21:58  takano
 * 	Submit DFAM-client test
 * 	[1996/02/29  04:21:43  takano]
 * 
 * Revision 1.1.2.1  1996/02/22  10:49:55  takano
 * 	First Submit
 * 	[1996/02/22  10:49:49  takano]
 * 
 * $EndLog$
 */
#include "dfacltp.h"
#include "dfaclmsg.h"

int		FVT_Read_ret, PclType;
int		fh1 = 32767, fh2 = 32767, fh3 = 32767, fh4 = 32767;
int		fh1_flg = 0, fh2_flg = 0, fh3_flg = 0, fh4_flg = 0;
int		s, t;
char	FvtFile[13], NWUser[48], NWPassword[129], NWServer[48], GWVolume[16], ErrorMode[5];
char	time_buf[26];
FILE	*fp1 = NULL, *fp2 = NULL, *fp3 = NULL, *fp4 = NULL;
time_t	nowtime;

/*** EnglishComment **********************************************************/
/*  ID   = main                                                              */
/*                                                                           */
/*  name = Main routine 						                             */
/*  func = (1) Calls a function to open the logging file.                    */
/*         (2) Calls a function to check the contents of the                 */
/*             configuration file.                                           */
/*         (3) Calls a function for mapping the NetWare login, DCE login,    */
/*             Network (MS-DOS) drive.                                       */
/*         (4) Displays a test suite starting message on a console.          */
/*         (5) Calls a function to check the contents of the test item       */
/*             registration file.                                            */
/*         (6) Initializes the current drive and current directory used when */
/*             test suite is executed                                        */
/*         (7) Displays a test suite end message on a console.               */
/*         (8) Calls a function to close the logging file.                   */
/*                                                                           */
/*  i/o    = none							                                 */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

void main(void)
{
	if (LOG_Open() == FALSE)
		exit(0);

	if (CFG_Read() == FALSE)
		exit(0);


	if (comopen(NWServer, NWUser, NWPassword, GWVolume) == FALSE)
		exit(0);


	printf(KDDS33001);

	FVT_Read_ret = FVT_Read();


	_chdrive('c'-'a'+1);
	_chdir("\\DFACLTP");


	printf(KDDS33008);


	comclose();

	LOG_Close();
}

/*** EnglishComment **********************************************************/
/*  ID   = LOG_Open   						                                 */
/*                                                                           */
/*  name = Opening the logging file                			                 */
/*  func = (1) Checks if the logging file exists.                            */
/*         (2) Opens the logging file (generation).                          */
/*         (3) Displays a test suite starting message on the console.        */
/*                                                                           */
/*  i/o    = none							                                 */
/*  return = TRUE : normal end                                               */
/*           FALSE: abnormal end                                             */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL LOG_Open()
{
	char	q[128];


	if (_access("C:\\DFACLTP\\DFACLTP.LOG", 0) != -1){
		while(1){
			rewind(stdin);
			printf(KDDS33114);
			scanf("%s", q);

			if(toupper(q[0]) == 'Y' && q[1] == '\0'){
				break;
			}else if(toupper(q[0]) == 'N' && q[1] == '\0'){
				return FALSE;
			}
		}
	}


	if ((log_fp = fopen("C:\\DFACLTP\\DFACLTP.LOG", "wt")) == NULL){
		Errcode = errno;
		printf(KDDS33107, "DFACLTP.LOG", "LOG_Open()", "fopen", Errcode);
		return FALSE;
	}


	Time_Get();


	if (fprintf(log_fp, "%sSTART\n", time_buf) < 0){
		Errcode = errno;
		printf(KDDS33107, "DFACLTP.LOG", "LOG_Open()", "fprintf", Errcode);
		if (fclose(log_fp) == EOF){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "LOG_Open()", "fclose", Errcode);
		}
		return FALSE;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = CFG_Read   						                             */
/*                                                                           */
/*  name = Reading the configuration file             			             */
/*  func = (1) Sets the default values.                                      */
/*         (2) Opens the configuration file.                                 */
/*         (3) Checks the key words and values.                              */
/*         (4) Closes the configuration file.                                */
/*                                                                           */
/*  i/o    = none							                                 */
/*  return = TRUE : normal end                                               */
/*           FALSE: abnormal end                                             */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL CFG_Read()
{
	int		cfg_len, value_len;
	int		cfg_line = 0;
	char	key[KEYWORD], value[LINE_MAX];
	char	cfg_buf[LINE_MAX];
	FILE	*cfg_fp;
	char	*equal_p;


	Byte.FvtFile_flg = OFF;
	Byte.NWUser_flg = OFF;
	Byte.NWPassword_flg = OFF;
	Byte.NWServer_flg = OFF;
	Byte.GWVolume_flg = OFF;
	Byte.ErrorMode_flg = OFF;


	strcpy(FvtFile, "DFACLTP.FVT");
	strcpy(NWUser, "DFALOGIN");
	strcpy(NWPassword, "DFALOGIN");
	NWServer[0] = '\0';
	strcpy(GWVolume, "DFATP");
	strcpy(ErrorMode, "STOP");


	if ((cfg_fp = fopen("C:\\DFACLTP\\DFACLTP.CFG", "r")) == NULL){
		Errcode = errno;
		printf(KDDS33107, "DFACLTP.CFG", "CFG_Read()", "fopen", Errcode);
		if (fprintf(log_fp, KDDS33107, "DFACLTP.CFG", "CFG_Read()", "fopen", Errcode) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fprintf", Errcode);
			if (fclose(log_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fclose", Errcode);
			}
		}
		return FALSE;
	}


	while (1){

		if (fgets(cfg_buf, LINE_MAX, cfg_fp) == NULL){
			Errcode = errno;
			if (feof(cfg_fp) == 0){
				printf(KDDS33107, "DFACLTP.CFG", "CFG_Read()", "fgets", Errcode);
				if (fprintf(log_fp, KDDS33107, "DFACLTP.CFG", "CFG_Read()", "fgets", Errcode) < 0){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fprintf", Errcode);
					if (fclose(log_fp) == EOF){
						Errcode = errno;
						printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fclose", Errcode);
					}
				}
				if (fclose(cfg_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.CFG", "CFG_Read()", "fclose", Errcode);
				}
				return FALSE;
			}
			break;
		}else{
			cfg_line++;


			if (feof(cfg_fp) != 0){
				printf(KDDS33103, cfg_line);
				if (fprintf(log_fp, KDDS33103, cfg_line) < 0){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fprintf", Errcode);
					if (fclose(log_fp) == EOF){
						Errcode = errno;
						printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fclose", Errcode);
					}
				}
				if (fclose(cfg_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.CFG", "CFG_Read()", "fclose", Errcode);
				}
				return FALSE;
			}
		}


		cfg_len = strlen(cfg_buf);
		if (cfg_buf[cfg_len-1] == '\n'){
			cfg_buf[cfg_len-1] = '\0';
			cfg_len--;
		}else{
			printf(KDDS33103, cfg_line);
			if (fprintf(log_fp, KDDS33103, cfg_line) < 0){
				Errcode = errno;
				printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fprintf", Errcode);
				if (fclose(log_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fclose", Errcode);
				}
			}
			if (fclose(cfg_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, "DFACLTP.CFG", "CFG_Read()", "fclose", Errcode);
			}
			return FALSE;
		}


		if (_strnicmp(cfg_buf, "REM ", 4) == 0)
			continue;


		if ((equal_p = strchr(cfg_buf, '=')) == NULL){
			printf(KDDS33103, cfg_line);
			if (fprintf(log_fp, KDDS33103, cfg_line) < 0){
				Errcode = errno;
				printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fprintf", Errcode);
				if (fclose(log_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fclose", Errcode);
				}
			}
			if (fclose(cfg_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, "DFACLTP.CFG", "CFG_Read()", "fclose", Errcode);
			}
			return FALSE;
		}


		if (equal_p - cfg_buf > KEYWORD){
			printf(KDDS33103, cfg_line);
			if (fprintf(log_fp, KDDS33103, cfg_line) < 0){
				Errcode = errno;
				printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fprintf", Errcode);
				if (fclose(log_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fclose", Errcode);
				}
			}
			if (fclose(cfg_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, "DFACLTP.CFG", "CFG_Read()", "fclose", Errcode);
			}
			return FALSE;
		}else{

			strncpy(key, cfg_buf, equal_p - cfg_buf);
			key[equal_p-cfg_buf] = '\0';
		}


		strcpy(value, equal_p+1);


		if ((value_len = strlen(value)) == 0){
			printf(KDDS33103, cfg_line);
			if (fprintf(log_fp, KDDS33103, cfg_line) < 0){
				Errcode = errno;
				printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fprintf", Errcode);
				if (fclose(log_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fclose", Errcode);
				}
			}
			if (fclose(cfg_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, "DFACLTP.CFG", "CFG_Read()", "fclose", Errcode);
			}
			return FALSE;
		}


		if(_stricmp(key, "FVTFILE") == 0 && Byte.FvtFile_flg == OFF){
			if (value_len < 1 || value_len > 12){
				printf(KDDS33103, cfg_line);
				if (fprintf(log_fp, KDDS33103, cfg_line) < 0){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fprintf", Errcode);
					if (fclose(log_fp) == EOF){
						Errcode = errno;
						printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fclose", Errcode);
					}
				}
				if(fclose(cfg_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.CFG", "CFG_Read()", "fclose", Errcode);
				}
				return FALSE;
			}
			strcpy(FvtFile, value);
			Byte.FvtFile_flg = ON;
		}else if(_stricmp(key, "NWUSER") == 0 && Byte.NWUser_flg == OFF){
			if (value_len < 1 || value_len > 47){
				printf(KDDS33103, cfg_line);
				if (fprintf(log_fp, KDDS33103, cfg_line) < 0){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fprintf", Errcode);
					if (fclose(log_fp) == EOF){
						Errcode = errno;
						printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fclose", Errcode);
					}
				}
				if (fclose(cfg_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.CFG", "CFG_Read()", "fclose", Errcode);
				}
				return FALSE;
			}
			strcpy(NWUser, value);
			Byte.NWUser_flg = ON;
		}else if(_stricmp(key, "NWPASSWORD") == 0 && Byte.NWPassword_flg == OFF){
			if (value_len < 1 || value_len > 128){
				printf(KDDS33103, cfg_line);
				if (fprintf(log_fp, KDDS33103, cfg_line) < 0){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fprintf", Errcode);
					if (fclose(log_fp) == EOF){
						Errcode = errno;
						printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fclose", Errcode);
					}
				}
				if (fclose(cfg_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.CFG", "CFG_Read()", "fclose", Errcode);
				}
				return FALSE;
			}
			strcpy(NWPassword, value);
			Byte.NWPassword_flg = ON;
		}else if(_stricmp(key, "NWSERVER") == 0 && Byte.NWServer_flg == OFF){
			if (value_len < 1 || value_len > 47){
				printf(KDDS33103, cfg_line);
				if (fprintf(log_fp, KDDS33103, cfg_line) < 0){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fprintf", Errcode);
					if (fclose(log_fp) == EOF){
						Errcode = errno;
						printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fclose", Errcode);
					}
				}
				if (fclose(cfg_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.CFG", "CFG_Read()", "fclose", Errcode);
				}
				return FALSE;
			}
			strcpy(NWServer, value);
			Byte.NWServer_flg = ON;
		}else if(_stricmp(key, "GWVOLUME") == 0 && Byte.GWVolume_flg == OFF){
			if (value_len < 2 || value_len > 15){
				printf(KDDS33103, cfg_line);
				if (fprintf(log_fp, KDDS33103, cfg_line) < 0){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fprintf", Errcode);
					if (fclose(log_fp) == EOF){
						Errcode = errno;
						printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fclose", Errcode);
					}
				}
				if (fclose(cfg_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.CFG", "CFG_Read()", "fclose", Errcode);
				}
				return FALSE;
			}
			strcpy(GWVolume, value);
			Byte.GWVolume_flg = ON;
		}else if(_stricmp(key, "ERRORMODE") == 0 && Byte.ErrorMode_flg == OFF){
			if (!(_stricmp(value, "STOP") == 0 || _stricmp(value, "CONT") == 0)){
				printf(KDDS33103, cfg_line);
				if (fprintf(log_fp, KDDS33103, cfg_line) < 0){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fprintf", Errcode);
					if (fclose(log_fp) == EOF){
						Errcode = errno;
						printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fclose", Errcode);
					}
				}
				if (fclose(cfg_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.CFG", "CFG_Read()", "fclose", Errcode);
				}
				return FALSE;
			}
			strcpy(ErrorMode, value);
			Byte.ErrorMode_flg = ON;
		}else{
			printf(KDDS33103, cfg_line);
			if (fprintf(log_fp, KDDS33103, cfg_line) < 0){
				Errcode = errno;
				printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fprintf", Errcode);
				if (fclose(log_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "CFG_Read()", "fclose", Errcode);
				}
			}
			if (fclose(cfg_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, "DFACLTP.CFG", "CFG_Read()", "fclose", Errcode);
			}
			return FALSE;
		}
	}


	if (fclose(cfg_fp) == EOF){
		Errcode = errno;
		printf(KDDS33107, "DFACLTP.CFG", "CFG_Read()", "fclose", Errcode);
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = FVT_Read   						                             */
/*                                                                           */
/*  name = Reading the test item registration file    			             */
/*  func = (1) Opens the test item registration file.                        */
/*         (2) Checks the name of the test item file.                        */
/*         (3) Initializes the current drive and current directory for test. */
/*         (4) Calls a function to execute the testing.                      */
/*         (5) Releases a file handle and file pointer.                      */
/*         (6) Closes the test item registration file.                       */
/*         (7) Displays the test summary on the console.                     */
/*                                                                           */
/*  i/o    = none							                                 */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*           LOGERR: failed to write on the logging file                     */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

int FVT_Read()
{
	int		i, fvt_len, Test_ret, FpFh_Free_ret;
	int     fvt_line = 0;
	FILE	*fvt_fp;
	char	pcl_name[LINE_MAX], f_fvt_name[80];


	s = t = 0;


	strcpy(f_fvt_name, "C:\\DFACLTP\\");
	strcat(f_fvt_name, FvtFile);
	if ((fvt_fp = fopen(f_fvt_name, "r")) == NULL){
		Errcode = errno;
		printf(KDDS33107, _strupr(FvtFile), "FVT_Read()", "fopen", Errcode);
		if (fprintf(log_fp, KDDS33107, _strupr(FvtFile), "FVT_Read()", "fopen", Errcode) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "FVT_Read()", "fprintf", Errcode);
			return LOGERR;
		}
		return FALSE;
	}

	while (1){

		if (fgets(pcl_name, LINE_MAX, fvt_fp) == NULL){
			Errcode = errno;
			if (feof(fvt_fp) == 0){
				printf(KDDS33107, _strupr(FvtFile), "FVT_Read()", "fgets", Errcode);
				if (fclose(fvt_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(FvtFile), "FVT_Read()", "fclose", Errcode);
				}
				if (fprintf(log_fp, KDDS33107, _strupr(FvtFile), "FVT_Read()", "fgets", Errcode) < 0){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "FVT_Read()", "fprintf", Errcode);
					return LOGERR;
				}
				return FALSE;
			}
			break;
		}else{
			fvt_line++;


			if (feof(fvt_fp) != 0){
				printf(KDDS33104, fvt_line);
				if (fclose(fvt_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(FvtFile), "FVT_Read()", "fclose", Errcode);
				}
				if (fprintf(log_fp, KDDS33104, fvt_line) < 0){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "FVT_Read()", "fprintf", Errcode);
					return LOGERR;
				}
				return FALSE;
			}
		}

		fvt_len = strlen(pcl_name);


		if (pcl_name[fvt_len-1] == '\n'){
			pcl_name[fvt_len-1] = '\0';
			fvt_len--;
		}else{
			printf(KDDS33104, fvt_line);
			if (fclose(fvt_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(FvtFile), "FVT_Read()", "fclose", Errcode);
			}
			if (fprintf(log_fp, KDDS33104, fvt_line) < 0){
				Errcode = errno;
				printf(KDDS33107, "DFACLTP.LOG", "FVT_Read()", "fprintf", Errcode);
				return LOGERR;
			}
			return FALSE;
		}


		if (_strnicmp(pcl_name, "REM ", 4) == 0)
			continue;


		if (fvt_len != 12){
			printf(KDDS33104, fvt_line);
			if (fclose(fvt_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(FvtFile), "FVT_Read()", "fclose", Errcode);
			}
			if (fprintf(log_fp, KDDS33104, fvt_line) < 0){
				Errcode = errno;
				printf(KDDS33107, "DFACLTP.LOG", "FVT_Read()", "fprintf", Errcode);
				return LOGERR;
			}
			return FALSE;
		}



		if(toupper(pcl_name[0]) != 'C'){
			printf(KDDS33104, fvt_line);
			if (fclose(fvt_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(FvtFile), "FVT_Read()", "fclose", Errcode);
			}
			if (fprintf(log_fp, KDDS33104, fvt_line) < 0){
				Errcode = errno;
				printf(KDDS33107, "DFACLTP.LOG", "FVT_Read()", "fprintf", Errcode);
				return LOGERR;
			}
			return FALSE;
		}


        for(i=1; i<8; i++){
			if (isdigit(pcl_name[i]) == 0){
				printf(KDDS33104, fvt_line);
				if (fclose(fvt_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(FvtFile), "FVT_Read()", "fclose", Errcode);
				}
				if (fprintf(log_fp, KDDS33104, fvt_line) < 0){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "FVT_Read()", "fprintf", Errcode);
					return LOGERR;
				}
				return FALSE;
			}
		}

		if (strncmp(pcl_name+1, "00", 2) == 0){
			printf(KDDS33104, fvt_line);
			if (fclose(fvt_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(FvtFile), "FVT_Read()", "fclose", Errcode);
			}
			if (fprintf(log_fp, KDDS33104, fvt_line) < 0){
				Errcode = errno;
				printf(KDDS33107, "DFACLTP.LOG", "FVT_Read()", "fprintf", Errcode);
				return LOGERR;
			}
			return FALSE;
		}

		if (strncmp(pcl_name+3, "00", 2) == 0){
			printf(KDDS33104, fvt_line);
			if (fclose(fvt_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(FvtFile), "FVT_Read()", "fclose", Errcode);
			}
			if (fprintf(log_fp, KDDS33104, fvt_line) < 0){
				Errcode = errno;
				printf(KDDS33107, "DFACLTP.LOG", "FVT_Read()", "fprintf", Errcode);
				return LOGERR;
			}
			return FALSE;
		}

		if (strncmp(pcl_name+5, "000", 3) == 0){
			printf(KDDS33104, fvt_line);
			if (fclose(fvt_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(FvtFile), "FVT_Read()", "fclose", Errcode);
			}
			if (fprintf(log_fp, KDDS33104, fvt_line) < 0){
				Errcode = errno;
				printf(KDDS33107, "DFACLTP.LOG", "FVT_Read()", "fprintf", Errcode);
				return LOGERR;
			}
			return FALSE;
		}


		if (_stricmp(pcl_name+8, ".BAT") == 0){
			PclType = BAT;
		}else if (_stricmp(pcl_name+8, ".TXT") == 0){
			PclType = TXT;
		}else{
			printf(KDDS33104, fvt_line);
			if (fclose(fvt_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(FvtFile), "FVT_Read()", "fclose", Errcode);
			}
			if (fprintf(log_fp, KDDS33104, fvt_line) < 0){
				Errcode = errno;
				printf(KDDS33107, "DFACLTP.LOG", "FVT_Read()", "fprintf", Errcode);
				return LOGERR;
			}
			return FALSE;
		}


		if (_chdrive('u'-'a'+1) == -1){
			Errcode = errno;
			printf(KDDS33113, "FVT_Read()", "_chdrive", Errcode);
			if (fclose(fvt_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(FvtFile), "FVT_Read()", "fclose", Errcode);
			}
			if (fprintf(log_fp, KDDS33113, "FVT_Read()", "_chdrive", Errcode) < 0){
				Errcode = errno;
				printf(KDDS33107, "DFACLTP.LOG", "FVT_Read()", "fprintf", Errcode);
				return LOGERR;
			}
			return FALSE;
		}

		if (_chdir("\\") == -1){
			Errcode = errno;
			printf(KDDS33113, "FVT_Read()", "_chdir", Errcode);
			if (fclose(fvt_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(FvtFile), "FVT_Read()", "fclose", Errcode);
			}
			if (fprintf(log_fp, KDDS33113, "FVT_Read()", "_chdir", Errcode) < 0){
				Errcode = errno;
				printf(KDDS33107, "DFACLTP.LOG", "FVT_Read()", "fprintf", Errcode);
				return LOGERR;
			}
			return FALSE;
		}


		Test_ret = Test(pcl_name);


		if ((FpFh_Free_ret = FpFh_Free()) != TRUE){
			return FpFh_Free_ret;
		}

		if (Test_ret != TRUE){
			break;
		}
    }


	if (fclose(fvt_fp) == EOF){
		Errcode = errno;
		printf(KDDS33107, _strupr(FvtFile), "FVT_Read()", "fclose", Errcode);
	}


	if (t == 0){
		printf(KDDS33006, s);
	}else{
		printf(KDDS33007, s, t);
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = FpFh_Free   						                             */
/*                                                                           */
/*  name = Releasing the file handle and file pointer                        */
/*  func = (1) Releases a file handle and file pointer.                      */
/*                                                                           */
/*  i/o    = none							                                 */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*           LOGERR: failed to write on the logging file                     */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

int FpFh_Free()
{
	int		log_flg = 0, err_flg = 0;


	if (fh1_flg == 1){
		if (_close(fh1) == -1){
			Errcode = errno;
			printf(KDDS33107, "FH1", "FpFh_Free()", "_close", Errcode);
			if (fprintf(log_fp, KDDS33107, "FH1", "FpFh_Free()", "_close", Errcode) < 0){
				Errcode = errno;
				printf(KDDS33107, "DFACLTP.LOG", "FpFh_Free()", "fprintf", Errcode);
				log_flg = 1;
			}
			err_flg = 1;
		}
		fh1 = 32767;
		fh1_flg = 0;
	}

	if (fh2_flg == 1){
		if (_close(fh2) == -1){
			Errcode = errno;
			printf(KDDS33107, "FH2", "FpFh_Free()", "_close", Errcode);
			if (log_flg != 1){
				if (fprintf(log_fp, KDDS33107, "FH2", "FpFh_Free()", "_close", Errcode) < 0){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "FpFh_Free()", "fprintf", Errcode);
					log_flg = 1;
				}
			}
			err_flg = 1;
		}
		fh2 = 32767;
		fh2_flg = 0;
	}

	if (fh3_flg == 1){
		if (_close(fh3) == -1){
			Errcode = errno;
			printf(KDDS33107, "FH3", "FpFh_Free()", "_close", Errcode);
			if (log_flg != 1){
				if (fprintf(log_fp, KDDS33107, "FH3", "FpFh_Free()", "_close", Errcode) < 0){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "FpFh_Free()", "fprintf", Errcode);
					log_flg = 1;
				}
			}
			err_flg = 1;
		}
		fh3 = 32767;
		fh3_flg = 0;
	}

	if (fh4_flg == 1){
		if (_close(fh4) == -1){
			Errcode = errno;
			printf(KDDS33107, "FH4", "FpFh_Free()", "_close", Errcode);
			if (log_flg != 1){
				if (fprintf(log_fp, KDDS33107, "FH4", "FpFh_Free()", "_close", Errcode) < 0){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "FpFh_Free()", "fprintf", Errcode);
					log_flg = 1;
				}
			}
			err_flg = 1;
		}
		fh4 = 32767;
		fh4_flg = 0;
	}

	if (fp1 != NULL){
		if (fclose(fp1) == EOF){
			Errcode = errno;
			printf(KDDS33107, "FP1", "FpFh_Free()", "fclose", Errcode);
			if (log_flg != 1){
				if (fprintf(log_fp, KDDS33107, "FP1", "FpFh_Free()", "fclose", Errcode) < 0){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "FpFh_Free()", "fprintf", Errcode);
					log_flg = 1;
				}
			}
			err_flg = 1;
		}
		fp1 = NULL;
	}

	if (fp2 != NULL){
		if (fclose(fp2) == EOF){
			Errcode = errno;
			printf(KDDS33107, "FP2", "FpFh_Free()", "fclose", Errcode);
			if (log_flg != 1){
				if (fprintf(log_fp, KDDS33107, "FP2", "FpFh_Free()", "fclose", Errcode) < 0){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "FpFh_Free()", "fprintf", Errcode);
					log_flg = 1;
				}
			}
			err_flg = 1;
		}
		fp2 = NULL;
	}

	if (fp3 != NULL){
		if (fclose(fp3) == EOF){
			Errcode = errno;
			printf(KDDS33107, "FP3", "FpFh_Free()", "fclose", Errcode);
			if (log_flg != 1){
				if (fprintf(log_fp, KDDS33107, "FP3", "FpFh_Free()", "fclose", Errcode) < 0){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "FpFh_Free()", "fprintf", Errcode);
					log_flg = 1;
				}
			}
			err_flg = 1;
		}
		fp3 = NULL;
	}

	if (fp4 != NULL){
		if (fclose(fp4) == EOF){
			Errcode = errno;
			printf(KDDS33107, "FP4", "FpFh_Free()", "fclose", Errcode);
			if (log_flg != 1){
				if (fprintf(log_fp, KDDS33107, "FP4", "FpFh_Free()", "fclose", Errcode) < 0){
					Errcode = errno;
					printf(KDDS33107, "DFACLTP.LOG", "FpFh_Free()", "fprintf", Errcode);
					log_flg = 1;
				}
			}
			err_flg = 1;
		}
		fp4 = NULL;
	}

	if (log_flg == 1)
		return LOGERR;

	if (err_flg == 1)
		return FALSE;

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = LOG_Close   						                             */
/*                                                                           */
/*  name = Closing the logging file                                          */
/*  func = (1) Outputs the test suite completion message to the logging file.*/
/*         (2) Closes the logging file.                                      */
/*                                                                           */
/*  i/o    = none							                                 */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

void LOG_Close()
{
	if (FVT_Read_ret != LOGERR){

		Time_Get();


		if (fprintf(log_fp, "%sEND\n", time_buf) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "LOG_Close()", "fprintf", Errcode);
		}
	}


	if (fclose(log_fp) == EOF){
		Errcode = errno;
		printf(KDDS33107, "DFACLTP.LOG", "LOG_Close()", "fclose", Errcode);
	}
}

/*** EnglishComment **********************************************************/
/*  ID     = Test   							                             */
/*                                                                           */
/*  name = Test execution                                                    */
/*  func = (1) Displays the starting (each test item) message on the         */
/*             console, as well as outputs the message to the logging file.  */
/*         (2) Calls a function to check if the batch file format is correct.*/
/*         (3) Calls a function to check if the text file format is correct. */
/*         (4) Displays the passed/failed message on the console, as well as */
/*             outputs the message to the logging file.                      */
/*                                                                           */
/*  i/o    = pcl_name :char * :IN: pointer to the buffer storing the test    */
/*                                 item file name                            */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*           LOGERR: failed to write on the logging file                     */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

int Test(char *pcl_name)
{
	int		ret;
	char	q;
	char	ans_name[13], f_pcl_name[80];


	printf(KDDS33002, _strupr(pcl_name));


	Time_Get();


	if (fprintf(log_fp, "%s", time_buf) < 0){
		Errcode = errno;
		printf(KDDS33107, "DFACLTP.LOG", "Test()", "fprintf", Errcode);
		return LOGERR;
	}


	strncpy(ans_name, pcl_name, 9);
	strcpy(ans_name+9, "ANS");

	if (PclType == BAT){
		strcpy(f_pcl_name, "C:\\DFACLTP\\TEST\\");
		strcat(f_pcl_name, pcl_name);
		system(f_pcl_name);

		ret = BAT_Verify(pcl_name, ans_name);
	}else{
		ret = Pcl_Read(pcl_name, ans_name);
	}


	Time_Get();

	if (ret == TRUE){
		s++;

		printf(KDDS33003, _strupr(pcl_name));

		if (fprintf(log_fp, "%s%s SUCCESSFUL\n", time_buf, _strupr(pcl_name)) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "Test()", "fprintf", Errcode);
			return LOGERR;
		}
	}else {
		t++;

		printf(KDDS33004, _strupr(pcl_name));

		if (fprintf(log_fp, "%s%s FAILED\n", time_buf, _strupr(pcl_name)) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "Test()", "fprintf", Errcode);
			return LOGERR;
		}

		if (fprintf(log_fp, log_buf) < 0){
			Errcode = errno;
			printf(KDDS33107, "DFACLTP.LOG", "Test()", "fprintf", Errcode);
			return LOGERR;
		}

		if (_stricmp(ErrorMode, "STOP") == 0){
			while(1){
				rewind(stdin);
				printf(KDDS33005);
				scanf("%c", &q);
				printf("\n");
				if(toupper(q) == 'N'){
					break;
				}else if(toupper(q) == 'Y'){
					return FALSE;
				}
			}
		}
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = BAT_Verify   						                             */
/*                                                                           */
/*  name = Comparison of the actual result and expected value                */
/*  func = Compares the actual result with the expected value (examination   */
/*         of the batch file format).                                        */
/*         (1) Opens the expected result file.                               */
/*         (2) Opens the outputted result file.                              */
/*         (3) Checks and compares the both files.                           */
/*         (4) Closes the outputted result file.                             */
/*         (5) Closes the expected result file.                              */
/*                                                                           */
/*  i/o    = pcl_name :char * :IN: pointer to the buffer storing the test    */
/*                                 item file name                            */
/*           ans_name :char * :IN: pointer to the buffer storing the         */
/*                                 expected result file name                 */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL BAT_Verify(char *pcl_name, char *ans_name)
{
	int		ans_len, Line, Count, word_len, word_no, i;
	int		ans_line = 0, out_line = 0, output_line = 0;
	char	out_name[13], f_ans_name[80], f_out_name[80], ans_buf[LINE_MAX], out_buf[LINE_MAX];
	char	line_buf[4], count_buf[3], Word[141], keep[LINE_MAX];
	char	*txt_p, *next_p, *Out;
	FILE	*ans_fp, *out_fp;


	strncpy(out_name, pcl_name, 9);
	strcpy(out_name+9, "OUT");


	strcpy(f_ans_name, "C:\\DFACLTP\\ANSWER\\");
	strcat(f_ans_name, ans_name);
	if ((ans_fp = fopen(f_ans_name, "r")) == NULL){
		Errcode = errno;
		printf(KDDS33107, _strupr(ans_name), "BAT_Verify()", "fopen", Errcode);
		sprintf(log_buf, KDDS33107, _strupr(ans_name), "BAT_Verify()", "fopen", Errcode);
		return FALSE;
	}


	strcpy(f_out_name, "C:\\DFACLTP\\OUTPUT\\");
	strcat(f_out_name, out_name);
	if ((out_fp = fopen(f_out_name, "r")) == NULL){
		Errcode = errno;
		printf(KDDS33107, _strupr(out_name), "BAT_Verify()", "fopen", Errcode);
		sprintf(log_buf, KDDS33107, _strupr(out_name), "BAT_Verify()", "fopen", Errcode);
		if (fclose(ans_fp) == EOF){
			Errcode = errno;
			printf(KDDS33107, _strupr(ans_name), "BAT_Verify()", "fclose", Errcode);
		}
		return FALSE;
	}

	while (1){

		if (fgets(ans_buf, LINE_MAX, ans_fp) == NULL){
			Errcode = errno;
			if (feof(ans_fp) == 0){
				printf(KDDS33107, _strupr(ans_name), "BAT_Verify()", "fgets", Errcode);
				sprintf(log_buf, KDDS33107, _strupr(ans_name), "BAT_Verify()", "fgets", Errcode);
				if (fclose(ans_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(ans_name), "BAT_Verify()", "fclose", Errcode);
				}
				if (fclose(out_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(out_name), "BAT_Verify()", "fclose", Errcode);
				}
				return FALSE;
			}


			break;
		}else{
			ans_line++;


			if (feof(ans_fp) != 0){
				printf(KDDS33106, ans_line);
				sprintf(log_buf, KDDS33106, ans_line);
				if (fclose(ans_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(ans_name), "BAT_Verify()", "fclose", Errcode);
				}
				if (fclose(out_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(out_name), "BAT_Verify()", "fclose", Errcode);
				}
				return FALSE;
			}

			ans_len = strlen(ans_buf);


			if (ans_buf[ans_len-1] == '\n'){
				ans_buf[ans_len-1] = '\0';
				ans_len--;
			}else{
				printf(KDDS33106, ans_line);
				sprintf(log_buf, KDDS33106, ans_line);
				if (fclose(ans_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(ans_name), "BAT_Verify()", "fclose", Errcode);
				}
				if (fclose(out_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(out_name), "BAT_Verify()", "fclose", Errcode);
				}
				return FALSE;
			}


			if (_strnicmp(ans_buf, "REM ", 4) == 0)
				continue;
		}


		if ((txt_p = strchr(ans_buf, ' ')) == NULL){
			printf(KDDS33106, ans_line);
			sprintf(log_buf, KDDS33106, ans_line);
			if (fclose(ans_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(ans_name), "BAT_Verify()", "fclose", Errcode);
			}
			if (fclose(out_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(out_name), "BAT_Verify()", "fclose", Errcode);
			}
			return FALSE;
		}


		if (txt_p-ans_buf > 3){
			printf(KDDS33106, ans_line);
			sprintf(log_buf, KDDS33106, ans_line);
			if (fclose(ans_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(ans_name), "BAT_Verify()", "fclose", Errcode);
			}
			if (fclose(out_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(out_name), "BAT_Verify()", "fclose", Errcode);
			}
			return FALSE;
		}

		strncpy(line_buf, ans_buf, txt_p-ans_buf);
		line_buf[txt_p-ans_buf] = '\0';


		for (i = 1; line_buf[i] != '\0'; i++){
			if (isdigit(line_buf[i]) == 0){
				printf(KDDS33106, ans_line);
				sprintf(log_buf, KDDS33106, ans_line);
				if (fclose(ans_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(ans_name), "BAT_Verify()", "fclose", Errcode);
				}
				if (fclose(out_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(out_name), "BAT_Verify()", "fclose", Errcode);
				}
				return FALSE;
			}
		}


		Line = atoi(line_buf);

		next_p = txt_p+1;


		if ((txt_p = strchr(next_p, ' ')) == NULL){
			printf(KDDS33106, ans_line);
			sprintf(log_buf, KDDS33106, ans_line);
			if (fclose(ans_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(ans_name), "BAT_Verify()", "fclose", Errcode);
			}
			if (fclose(out_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(out_name), "BAT_Verify()", "fclose", Errcode);
			}
			return FALSE;
		}


		if (txt_p-next_p > 2){
			printf(KDDS33106, ans_line);
			sprintf(log_buf, KDDS33106, ans_line);
			if (fclose(ans_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(ans_name), "BAT_Verify()", "fclose", Errcode);
			}
			if (fclose(out_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(out_name), "BAT_Verify()", "fclose", Errcode);
			}
			return FALSE;
		}

		strncpy(count_buf, next_p, txt_p-next_p);
		count_buf[txt_p-next_p] = '\0';


		for (i = 1; count_buf[i] != '\0'; i++){
			if (isdigit(count_buf[i]) == 0){
				printf(KDDS33106, ans_line);
				sprintf(log_buf, KDDS33106, ans_line);
				if (fclose(ans_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(ans_name), "BAT_Verify()", "fclose", Errcode);
				}
				if (fclose(out_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(out_name), "BAT_Verify()", "fclose", Errcode);
				}
				return FALSE;
			}
		}


		Count = atoi(count_buf);

		next_p = txt_p+1;


		if ((txt_p = strchr(next_p, ' ')) != NULL){
			printf(KDDS33106, ans_line);
			sprintf(log_buf, KDDS33106, ans_line);
			if (fclose(ans_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(ans_name), "BAT_Verify()", "fclose", Errcode);
			}
			if (fclose(out_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(out_name), "BAT_Verify()", "fclose", Errcode);
			}
			return FALSE;
		}


		if ((word_len = strlen(next_p)) < 1 || word_len > 140){
			printf(KDDS33106, ans_line);
			sprintf(log_buf, KDDS33106, ans_line);
			if (fclose(ans_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(ans_name), "BAT_Verify()", "fclose", Errcode);
			}
			if (fclose(out_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(out_name), "BAT_Verify()", "fclose", Errcode);
			}
			return FALSE;
		}


		strcpy(Word, next_p);

		if (output_line != Line){

			rewind(out_fp);


			for (output_line = 1; output_line <= Line; output_line++){
				if (fgets(out_buf, LINE_MAX, out_fp) == NULL){
					Errcode = errno;
					if (feof(out_fp) == 0){
						printf(KDDS33107, _strupr(out_name), "BAT_Verify()", "fgets", Errcode);
						sprintf(log_buf, KDDS33107, _strupr(out_name), "BAT_Verify()", "fgets", Errcode);
						if (fclose(ans_fp) == EOF){
							Errcode = errno;
							printf(KDDS33107, _strupr(ans_name), "BAT_Verify()", "fclose", Errcode);
						}
						if (fclose(out_fp) == EOF){
							Errcode = errno;
							printf(KDDS33107, _strupr(out_name), "BAT_Verify()", "fclose", Errcode);
						}
						return FALSE;
					}else{

						printf(KDDS33106, ans_line);
						sprintf(log_buf, KDDS33106, ans_line);
						if (fclose(ans_fp) == EOF){
							Errcode = errno;
							printf(KDDS33107, _strupr(ans_name), "BAT_Verify()", "fclose", Errcode);
						}
						if (fclose(out_fp) == EOF){
							Errcode = errno;
							printf(KDDS33107, _strupr(out_name), "BAT_Verify()", "fclose", Errcode);
						}
						return FALSE;
					}
				}
			}


			strcpy(keep, out_buf);

			output_line--;
		}

		strcpy(out_buf, keep);

		Out = strtok(out_buf, " \n");


		for  (word_no = 2; word_no <= Count && Out != NULL; word_no++){
			Out = strtok(NULL, " \n");
		}


		if (strcmp(Word, Out) != 0){
			printf(KDDS33116, ans_line, Out);
			sprintf(log_buf, KDDS33116, ans_line, Out);
			if (fclose(ans_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(ans_name), "BAT_Verify()", "fclose", Errcode);
			}
			if (fclose(out_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(out_name), "BAT_Verify()", "fclose", Errcode);
			}
			return FALSE;
		}
	}


	if (fclose(out_fp) == EOF){
		Errcode = errno;
		printf(KDDS33107, _strupr(out_name), "BAT_Verify()", "fclose", Errcode);
	}


	if (fclose(ans_fp) == EOF){
		Errcode = errno;
		printf(KDDS33107, _strupr(ans_name), "BAT_Verify()", "fclose", Errcode);
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = Pcl_Read   						                             */
/*                                                                           */
/*  name = Reading and checking the test item file                           */
/*  func = Reads the test item file, and checks the text file format of the  */
/*         file.                                                             */
/*         (1) Opens the test item file.                                     */
/*         (2) Opens the outputted result file.                              */
/*         (3) Checks the numbers of lines of the test item file and the     */
/*             expected result file.                                         */
/*         (4) Calls a function to search for an API name.                   */
/*         (5) Compares the actual results with the expected result file.    */
/*         (6) Closes the test item file.                                    */
/*         (7) Closes the expected result file.                              */
/*                                                                           */
/*  i/o    = pcl_name :char * :IN: pointer to the buffer storing the test    */
/*                                 item file name                            */
/*           ans_name :char * :IN: pointer to the buffer storing the         */
/*                                 expected result file name                 */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL Pcl_Read(char *pcl_name, char *ans_name)
{
	int		pcl_len, ans_len, i;
	int		pcl_break_flg = 0, ans_break_flg = 0, pcl_line = 0, ans_line = 0;
	FILE	*pcl_fp, *ans_fp;
	char	pcl_buf[LINE_MAX], ans_buf[LINE_MAX], err_buf[3], f_pcl_name[80], f_ans_name[80];


	strcpy(f_pcl_name, "C:\\DFACLTP\\TEST\\");
	strcat(f_pcl_name, pcl_name);
	if ((pcl_fp = fopen(f_pcl_name, "r")) == NULL){
		Errcode = errno;
		printf(KDDS33107, _strupr(pcl_name), "Pcl_Read()", "fopen", Errcode);
		sprintf(log_buf, KDDS33107, _strupr(pcl_name), "Pcl_Read()", "fopen", Errcode);
		return FALSE;
	}


	strcpy(f_ans_name, "C:\\DFACLTP\\ANSWER\\");
	strcat(f_ans_name, ans_name);
	if ((ans_fp = fopen(f_ans_name, "r")) == NULL){
		Errcode = errno;
		printf(KDDS33107, _strupr(ans_name), "Pcl_Read()", "fopen", Errcode);
		sprintf(log_buf, KDDS33107, _strupr(ans_name), "Pcl_Read()", "fopen", Errcode);
		if (fclose(pcl_fp) == EOF){
			Errcode = errno;
			printf(KDDS33107, _strupr(pcl_name), "Pcl_Read()", "fclose", Errcode);
		}
		return FALSE;
	}

	while (1){

		if (fgets(pcl_buf, LINE_MAX, pcl_fp) == NULL){
			Errcode = errno;
			if (feof(pcl_fp) == 0){
				printf(KDDS33107, _strupr(pcl_name), "Pcl_Read()", "fgets", Errcode);
				sprintf(log_buf, KDDS33107, _strupr(pcl_name), "Pcl_Read()", "fgets", Errcode);
				if (fclose(pcl_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(pcl_name), "Pcl_Read()", "fclose", Errcode);
				}
				if (fclose(ans_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(ans_name), "Pcl_Read()", "fclose", Errcode);
				}
				return FALSE;
			}


			pcl_break_flg = 1;
		}else{
			pcl_line++;


			if (feof(pcl_fp) != 0){
				printf(KDDS33105, pcl_line);
				sprintf(log_buf, KDDS33105, pcl_line);
				if (fclose(pcl_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(pcl_name), "Pcl_Read()", "fclose", Errcode);
				}
				if (fclose(ans_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(ans_name), "Pcl_Read()", "fclose", Errcode);
				}
				return FALSE;
			}

			pcl_len = strlen(pcl_buf);


			if (pcl_buf[pcl_len-1] == '\n'){
				pcl_buf[pcl_len-1] = '\0';
				pcl_len--;
			}else{
				printf(KDDS33105, pcl_line);
				sprintf(log_buf, KDDS33105, pcl_line);
				if (fclose(pcl_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(pcl_name), "Pcl_Read()", "fclose", Errcode);
				}
				if (fclose(ans_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(ans_name), "Pcl_Read()", "fclose", Errcode);
				}
				return FALSE;
			}


			if (_strnicmp(pcl_buf, "REM ", 4) == 0)
				continue;

			if (_strnicmp(pcl_buf, "DISP ", 5) == 0){
				if (Disp(pcl_buf+5, pcl_line) == FALSE){
					if (fclose(pcl_fp) == EOF){
						Errcode = errno;
						printf(KDDS33107, _strupr(pcl_name), "Pcl_Read()", "fclose", Errcode);
					}
					return FALSE;
				}
				continue;
			}
		}

		while(1){

			if (fgets(ans_buf, LINE_MAX, ans_fp) == NULL){
				Errcode = errno;
				if (feof(ans_fp) == 0){
					printf(KDDS33107, _strupr(ans_name), "Pcl_Read()", "fgets", Errcode);
					sprintf(log_buf, KDDS33107, _strupr(ans_name), "Pcl_Read()", "fgets", Errcode);
					if (fclose(pcl_fp) == EOF){
						Errcode = errno;
						printf(KDDS33107, _strupr(pcl_name), "Pcl_Read()", "fclose", Errcode);
					}
					if (fclose(ans_fp) == EOF){
						Errcode = errno;
						printf(KDDS33107, _strupr(ans_name), "Pcl_Read()", "fclose", Errcode);
					}
					return FALSE;
				}


				ans_break_flg = 1;
				break;
			}else{
				ans_line++;


				if (feof(ans_fp) != 0){
					printf(KDDS33106, ans_line);
					sprintf(log_buf, KDDS33106, ans_line);
					if (fclose(pcl_fp) == EOF){
						Errcode = errno;
						printf(KDDS33107, _strupr(pcl_name), "Pcl_Read()", "fclose", Errcode);
					}
					if (fclose(ans_fp) == EOF){
						Errcode = errno;
						printf(KDDS33107, _strupr(ans_name), "Pcl_Read()", "fclose", Errcode);
					}
					return FALSE;
				}
			}

			ans_len = strlen(ans_buf);


			if (ans_buf[ans_len-1] == '\n'){
				ans_buf[ans_len-1] = '\0';
				ans_len--;
			}else{
				printf(KDDS33106, ans_line);
				sprintf(log_buf, KDDS33106, ans_line);
				if (fclose(pcl_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(pcl_name), "Pcl_Read()", "fclose", Errcode);
				}
				if (fclose(ans_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(ans_name), "Pcl_Read()", "fclose", Errcode);
				}
				return FALSE;
			}


			if (_strnicmp(ans_buf, "REM ", 4) == 0)
				continue;


			if (ans_buf[0] == '-'){
				i = 1;
			}else{
				i = 0;
			}

			for (; ans_buf[i] != '\0'; i++){
				if (isdigit(ans_buf[i]) == 0){
					printf(KDDS33106, ans_line);
					sprintf(log_buf, KDDS33106, ans_line);
					if (fclose(pcl_fp) == EOF){
						Errcode = errno;
						printf(KDDS33107, _strupr(pcl_name), "Pcl_Read()", "fclose", Errcode);
					}
					if (fclose(ans_fp) == EOF){
						Errcode = errno;
						printf(KDDS33107, _strupr(ans_name), "Pcl_Read()", "fclose", Errcode);
					}
					return FALSE;
				}
			}


			break;
		}


		if (pcl_break_flg == 1){
			if (ans_break_flg == 0){
				printf(KDDS33115);
				sprintf(log_buf, KDDS33115);
				if (fclose(pcl_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(pcl_name), "Pcl_Read()", "fclose", Errcode);
				}
				if (fclose(ans_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(ans_name), "Pcl_Read()", "fclose", Errcode);
				}
				return FALSE;
			}

			break;
		}else{
			if (ans_break_flg == 1){
				printf(KDDS33115);
				sprintf(log_buf, KDDS33115);
				if (fclose(pcl_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(pcl_name), "Pcl_Read()", "fclose", Errcode);
				}
				if (fclose(ans_fp) == EOF){
					Errcode = errno;
					printf(KDDS33107, _strupr(ans_name), "Pcl_Read()", "fclose", Errcode);
				}
				return FALSE;
			}
		}


		if (API_Search(pcl_buf, pcl_line) == FALSE){
			if (fclose(pcl_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(pcl_name), "Pcl_Read()", "fclose", Errcode);
			}
			if (fclose(ans_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(ans_name), "Pcl_Read()", "fclose", Errcode);
			}
			return FALSE;
		}


		_itoa(Errcode, err_buf, 10);


		if (strcmp(ans_buf, err_buf) != 0){
			printf(KDDS33116, ans_line, err_buf);
			sprintf(log_buf, KDDS33116, ans_line, err_buf);
			if (fclose(pcl_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(pcl_name), "Pcl_Read()", "fclose", Errcode);
			}
			if (fclose(ans_fp) == EOF){
				Errcode = errno;
				printf(KDDS33107, _strupr(ans_name), "Pcl_Read()", "fclose", Errcode);
			}
			return FALSE;
		}

	}


	if (fclose(pcl_fp) == EOF){
		Errcode = errno;
		printf(KDDS33107, _strupr(pcl_name), "Pcl_Read()", "fclose", Errcode);
	}


	if (fclose(ans_fp) == EOF){
		Errcode = errno;
		printf(KDDS33107, _strupr(ans_name), "Pcl_Read()", "fclose", Errcode);
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = Disp   							                             */
/*                                                                           */
/*  name = Message display and output to the logging file                    */
/*  func = Displays the message, and outputs the message to the logging file.*/
/*         file.                                                             */
/*         (1) Checks the parameters.                                        */
/*         (2) Displays the outputting message to the console, and outputs   */
/*             the message to the logging file as well.                      */
/*                                                                           */
/*  i/o    = prm_p   :char * :IN: pointer to parameters                      */
/*           pcl_line:int    :IN: the line number of the test item file      */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL Disp(char *prm_p, int pcl_line)
{

	if(strlen(prm_p) >= 80){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	printf("%s\n", prm_p);

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = API_Search   						                             */
/*                                                                           */
/*  name = Searching for the API name                                        */
/*  func = Searching the test item file for the API name in the currently-   */
/*         executed line.                                                    */
/*                                                                           */
/*  i/o    = pcl_buf :char * :IN: pointer to the buffer storing a single     */
/*                                line of the test item file (for the text   */
/*                                format only)                               */
/*           pcl_line:int    :IN: the line number of the test item file      */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL API_Search(char *pcl_buf, int pcl_line)
{
	char	api_buf[LINE_MAX];
	char	*txt_p;


	if (_stricmp(pcl_buf, "MAP") == 0){
		if (T_Map() == FALSE){
			return FALSE;
		}
		return TRUE;
	}


	if ((txt_p = strchr(pcl_buf, ' ')) == NULL){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}
	txt_p++;


	strncpy(api_buf, pcl_buf, txt_p-pcl_buf);
	api_buf[txt_p-pcl_buf] = '\0';
	_strupr(api_buf);

	if (strcmp(api_buf, "OPEN ") == 0){
		if (T_Open(txt_p, pcl_line) == FALSE){
			return FALSE;
		}
	}else if (strcmp(api_buf, "CLOSE ") == 0){
		if (T_Close(txt_p, pcl_line) == FALSE){
			return FALSE;
		}
	}else if (strcmp(api_buf, "READ ") == 0){
		if (T_Read(txt_p, pcl_line) == FALSE){
			return FALSE;
		}
	}else if (strcmp(api_buf, "WRITE ") == 0){
		if (T_Write(txt_p, pcl_line) == FALSE){
			return FALSE;
		}
	}else if (strcmp(api_buf, "UNLINK ") == 0){
		if (T_Unlink(txt_p, pcl_line) == FALSE){
			return FALSE;
		}
	}else if (strcmp(api_buf, "MKDIR ") == 0){
		if (T_Mkdir(txt_p, pcl_line) == FALSE){
			return FALSE;
		}
	}else if (strcmp(api_buf, "RMDIR ") == 0){
		if (T_Rmdir(txt_p, pcl_line) == FALSE){
			return FALSE;
		}
	}else if (strcmp(api_buf, "CHDIR ") == 0){
		if (T_Chdir(txt_p, pcl_line) == FALSE){
			return FALSE;
		}
	}else if (strcmp(api_buf, "RENAME ") == 0){
		if (T_Rename(txt_p, pcl_line) == FALSE){
			return FALSE;
		}
	}else if (strcmp(api_buf, "STAT ") == 0){
		if (T_Stat(txt_p, pcl_line) == FALSE){
			return FALSE;
		}
	}else if (strcmp(api_buf, "UTIME ") == 0){
		if (T_Utime(txt_p, pcl_line) == FALSE){
			return FALSE;
		}
	}else if (strcmp(api_buf, "FOPEN ") == 0){
		if (T_Fopen(txt_p, pcl_line) == FALSE){
			return FALSE;
		}
	}else if (strcmp(api_buf, "FCLOSE ") == 0){
		if (T_Fclose(txt_p, pcl_line) == FALSE){
			return FALSE;
		}
	}else if (strcmp(api_buf, "FGETC ") == 0){
		if (T_Fgetc(txt_p, pcl_line) == FALSE){
			return FALSE;
		}
	}else if (strcmp(api_buf, "FGETS ") == 0){
		if (T_Fgets(txt_p, pcl_line) == FALSE){
			return FALSE;
		}
	}else if (strcmp(api_buf, "FREAD ") == 0){
		if (T_Fread(txt_p, pcl_line) == FALSE){
			return FALSE;
		}
	}else if (strcmp(api_buf, "FPUTC ") == 0){
		if (T_Fputc(txt_p, pcl_line) == FALSE){
			return FALSE;
		}
	}else if (strcmp(api_buf, "FPUTS ") == 0){
		if (T_Fputs(txt_p, pcl_line) == FALSE){
			return FALSE;
		}
	}else if (strcmp(api_buf, "FWRITE ") == 0){
		if (T_Fwrite(txt_p, pcl_line) == FALSE){
			return FALSE;
		}
	}else{
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = T_Open   							                             */
/*                                                                           */
/*  name = Library function: _open                                           */
/*  func = (1) Checks the parameters.                                        */
/*         (2) Executes _open.                                               */
/*                                                                           */
/*  i/o    = prm_p   :char * :IN: pointer to parameters                      */
/*           pcl_line:int    :IN: the line number of the test item file      */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL T_Open(char *prm_p, int pcl_line)
{
	int		ret, len;
	int		Mode = 0;
	char	FileHandle[4], FileName[80], AccessMode[3], FileMode[2], CreateMode[3];
	char	*fh_p, *fn_p, *am_p, *fm_p;


	fh_p = prm_p;


	if ((prm_p = strchr(fh_p, ' ')) == NULL){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	if(prm_p-fh_p != 3){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strncpy(FileHandle, fh_p, prm_p-fh_p);
	FileHandle[prm_p-fh_p] = '\0';


	fn_p = prm_p+1;


	if ((prm_p = strchr(fn_p, ' ')) == NULL){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	if(prm_p-fn_p >= 80){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strncpy(FileName, fn_p, prm_p-fn_p);
	FileName[prm_p-fn_p] = '\0';


	am_p = prm_p+1;


	if ((prm_p = strchr(am_p, ' ')) == NULL){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	if(prm_p-am_p != 2){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strncpy(AccessMode, am_p, prm_p-am_p);
	AccessMode[prm_p-am_p] = '\0';


	fm_p = prm_p+1;


	if ((prm_p = strchr(fm_p, ' ')) == NULL){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	if(prm_p-fm_p != 1){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strncpy(FileMode, fm_p, prm_p-fm_p);
	FileMode[prm_p-fm_p] = '\0';


	prm_p++;


	if((len = strlen(prm_p)) == 0 || len > 2){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strcpy(CreateMode, prm_p);


	if (!((_stricmp(FileHandle, "FH1") == 0) || (_stricmp(FileHandle, "FH2") == 0) ||
	    (_stricmp(FileHandle, "FH3") == 0) || (_stricmp(FileHandle, "FH4") == 0))){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	if(_stricmp(AccessMode, "RO") == 0){
		Mode |= _O_RDONLY;
	}else if(_stricmp(AccessMode, "WO") == 0){
		Mode |= _O_WRONLY;
	}else if(_stricmp(AccessMode, "RW") == 0){
		Mode |= _O_RDWR;
	}else{
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	if(toupper(FileMode[0]) == 'B'){
		Mode |= _O_BINARY;
	}else if(toupper(FileMode[0]) == 'T'){
		Mode |= _O_TEXT;
	}else{
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	if(_stricmp(CreateMode, "C") == 0){
		Mode |= _O_CREAT;
	}else if(_stricmp(CreateMode, "T") == 0){
		Mode |= _O_TRUNC;
	}else if(_stricmp(CreateMode, "CE") == 0){
		Mode |= (_O_CREAT | _O_EXCL);
	}else if(_stricmp(CreateMode, "N") == 0){
		;
	}else{
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	ret = _open(FileName, Mode, _S_IREAD | _S_IWRITE);

	if (ret == -1){
		Errcode = errno;
	}else{
		if (_stricmp(FileHandle, "FH1") == 0){
			fh1 = ret;
			fh1_flg = 1;
		}else if (_stricmp(FileHandle, "FH2") == 0){
			fh2 = ret;
			fh2_flg = 1;
		}else if (_stricmp(FileHandle, "FH3") == 0){
			fh3 = ret;
			fh3_flg = 1;
		}else if (_stricmp(FileHandle, "FH4") == 0){
			fh4 = ret;
			fh4_flg = 1;
		}

		Errcode = 0;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = T_Close   							                             */
/*                                                                           */
/*  name = Library function: _close                                          */
/*  func = (1) Checks the parameters.                                        */
/*         (2) Executes _close.                                              */
/*                                                                           */
/*  i/o    = prm_p   :char * :IN: pointer to parameters                      */
/*           pcl_line:int    :IN: the line number of the test item file      */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL T_Close(char *prm_p, int pcl_line)
{
	int		ret, handle;
	char	FileHandle[4];


	if (strlen(prm_p) != 3){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strcpy(FileHandle, prm_p);


	if (_stricmp(FileHandle, "FH1") == 0){
		handle = fh1;

		fh1 = 32767;
		fh1_flg = 0;
	}else if (_stricmp(FileHandle, "FH2") == 0){
		handle = fh2;

		fh2 = 32767;
		fh2_flg = 0;
	}else if (_stricmp(FileHandle, "FH3") == 0){
		handle = fh3;

		fh3 = 32767;
		fh3_flg = 0;
	}else if (_stricmp(FileHandle, "FH4") == 0){
		handle = fh4;

		fh4 = 32767;
		fh4_flg = 0;
	}else{
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	ret = _close(handle);

	if (ret == -1){
		Errcode = errno;
	}else{
		Errcode = 0;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = T_Read   							                             */
/*                                                                           */
/*  name = Library function: _read                                           */
/*  func = (1) Checks the parameters.                                        */
/*         (2) Executes _read.                                               */
/*                                                                           */
/*  i/o    = prm_p   :char * :IN: pointer to parameters                      */
/*           pcl_line:int    :IN: the line number of the test item file      */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL T_Read(char *prm_p, int pcl_line)
{
	int		ret, handle;
	char	FileHandle[4], ReadBuf[16];


	if (strlen(prm_p) != 3){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strcpy(FileHandle, prm_p);


	if (_stricmp(FileHandle, "FH1") == 0){
		handle = fh1;
	}else if (_stricmp(FileHandle, "FH2") == 0){
		handle = fh2;
	}else if (_stricmp(FileHandle, "FH3") == 0){
		handle = fh3;
	}else if (_stricmp(FileHandle, "FH4") == 0){
		handle = fh4;
	}else{
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	ret = _read(handle, ReadBuf, 16);

	if (ret == -1){
		Errcode = errno;
	}else{
		Errcode = 0;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = T_Write   							                             */
/*                                                                           */
/*  name = Library function: _write                                          */
/*  func = (1) Checks the parameters.                                        */
/*         (2) Executes _write.                                              */
/*                                                                           */
/*  i/o    = prm_p   :char * :IN: pointer to parameters                      */
/*           pcl_line:int    :IN: the line number of the test item file      */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL T_Write(char *prm_p, int pcl_line)
{
	int		ret, handle;
	char	FileHandle[4];
	char	WriteBuf[17] = "0123456789ABCDEF";


	if (strlen(prm_p) != 3){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strcpy(FileHandle, prm_p);


	if (_stricmp(FileHandle, "FH1") == 0){
		handle = fh1;
	}else if (_stricmp(FileHandle, "FH2") == 0){
		handle = fh2;
	}else if (_stricmp(FileHandle, "FH3") == 0){
		handle = fh3;
	}else if (_stricmp(FileHandle, "FH4") == 0){
		handle = fh4;
	}else{
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	ret = _write(handle, WriteBuf, 16);

	if (ret == -1){
		Errcode = errno;
	}else{
		Errcode = 0;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = T_Unlink   						                             */
/*                                                                           */
/*  name = Library function: _unlink                                         */
/*  func = (1) Checks the parameters.                                        */
/*         (2) Executes _unlink.                                             */
/*                                                                           */
/*  i/o    = prm_p   :char * :IN: pointer to parameters                      */
/*           pcl_line:int    :IN: the line number of the test item file      */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL T_Unlink(char *prm_p, int pcl_line)
{
	int 	ret, len;
	char	FileName[80];


	if((len = strlen(prm_p)) == 0 || len >= 80){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strcpy(FileName, prm_p);


	if (strchr(FileName, ' ') != NULL){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	ret = _unlink(FileName);

	if (ret == -1){
		Errcode = errno;
	}else{
		Errcode = 0;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = T_Mkdir   							                             */
/*                                                                           */
/*  name = Library function: _mkdir                                          */
/*  func = (1) Checks the parameters.                                        */
/*         (2) Executes _mkdir.                                              */
/*                                                                           */
/*  i/o    = prm_p   :char * :IN: pointer to parameters                      */
/*           pcl_line:int    :IN: the line number of the test item file      */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL T_Mkdir(char *prm_p, int pcl_line)
{
	int		ret, len;
	char	DirName[80];


	if((len = strlen(prm_p)) == 0 || len >= 80){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strcpy(DirName, prm_p);


	if (strchr(DirName, ' ') != NULL){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	ret = _mkdir(DirName);

	if (ret == -1){
		Errcode = errno;
	}else{
		Errcode = 0;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = T_Rmdir   							                             */
/*                                                                           */
/*  name = Library function: _rmdir                                          */
/*  func = (1) Checks the parameters.                                        */
/*         (2) Executes _rmdir.                                              */
/*                                                                           */
/*  i/o    = prm_p   :char * :IN: pointer to parameters                      */
/*           pcl_line:int    :IN: the line number of the test item file      */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL T_Rmdir(char *prm_p, int pcl_line)
{
	int		ret, len;
	char	DirName[80];


	if((len = strlen(prm_p)) == 0 || len >= 80){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strcpy(DirName, prm_p);


	if (strchr(DirName, ' ') != NULL){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	ret = _rmdir(DirName);

	if (ret == -1){
		Errcode = errno;
	}else{
		Errcode = 0;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = T_Chdir   							                             */
/*                                                                           */
/*  name = Library function: _chdir                                          */
/*  func = (1) Checks the parameters.                                        */
/*         (2) Executes _chdir.                                              */
/*                                                                           */
/*  i/o    = prm_p   :char * :IN: pointer to parameters                      */
/*           pcl_line:int    :IN: the line number of the test item file      */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL T_Chdir(char *prm_p, int pcl_line)
{
	int		ret, len;
	char	DirName[80];


	if((len = strlen(prm_p)) == 0  || len >= 80){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strcpy(DirName, prm_p);


	if (strchr(DirName, ' ') != NULL){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	ret = _chdir(DirName);

	if (ret == -1){
		Errcode = errno;
	}else{
		Errcode = 0;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = T_Rename   						                             */
/*                                                                           */
/*  name = Library function: rename                                          */
/*  func = (1) Checks the parameters.                                        */
/*         (2) Executes "rename."                                            */
/*                                                                           */
/*  i/o    = prm_p   :char * :IN: pointer to parameters                      */
/*           pcl_line:int    :IN: the line number of the test item file      */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL T_Rename(char *prm_p, int pcl_line)
{
	int		ret, len;
	char	OldName[13], NewName[13];
	char	*on_p;


	on_p = prm_p;


	if ((prm_p = strchr(on_p, ' ')) == NULL){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	if(prm_p-on_p == 0 || prm_p-on_p >= 13){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strncpy(OldName, on_p, prm_p-on_p);
	OldName[prm_p-on_p] = '\0';


	prm_p++;


	if((len = strlen(prm_p)) == 0 || len >= 13){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strcpy(NewName, prm_p);


	if (strchr(NewName, ' ') != NULL){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	ret = rename(OldName, NewName);

	if (ret == 0){
		Errcode = 0;
	}else{
		Errcode = errno;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = T_Stat   							                             */
/*                                                                           */
/*  name = Library function: _stat                                           */
/*  func = (1) Checks the parameters.                                        */
/*         (2) Executes _stat.                                               */
/*                                                                           */
/*  i/o    = prm_p   :char * :IN: pointer to parameters                      */
/*           pcl_line:int    :IN: the line number of the test item file      */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL T_Stat(char *prm_p, int pcl_line)
{
	int 	ret, len;
	char	FileName[80];

	struct _stat stat_buf;


	if((len = strlen(prm_p)) == 0 || len >= 80){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf,KDDS33105, pcl_line);
		return FALSE;
	}


	strcpy(FileName, prm_p);


	if (strchr(FileName, ' ') != NULL){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf,KDDS33105, pcl_line);
		return FALSE;
	}


	ret = _stat(FileName, &stat_buf);

	if (ret == -1){
		Errcode = errno;
	}else{
		Errcode = 0;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = T_Utime   							                             */
/*                                                                           */
/*  name = Library function: _utime                                          */
/*  func = (1) Checks the parameters.                                        */
/*         (2) Executes _utime.                                              */
/*                                                                           */
/*  i/o    = prm_p   :char * :IN: pointer to parameters                      */
/*           pcl_line:int    :IN: the line number of the test item file      */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL T_Utime(char *prm_p, int pcl_line)
{
	int 	ret, len;
	char	FileName[80];


	if((len = strlen(prm_p)) == 0 || len >= 80){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strcpy(FileName, prm_p);


	if (strchr(FileName, ' ') != NULL){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	ret = _utime(FileName, NULL);

	if (ret == -1){
		Errcode = errno;
	}else{
		Errcode = 0;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = T_Fopen   							                             */
/*                                                                           */
/*  name = Library function: fopen                                           */
/*  func = (1) Checks the parameters.                                        */
/*         (2) Executes fopen.                                               */
/*                                                                           */
/*  i/o    = prm_p   :char * :IN: pointer to parameters                      */
/*           pcl_line:int    :IN: the line number of the test item file      */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL T_Fopen(char *prm_p, int pcl_line)
{
	char	FilePointa[4], FileName[80], AccessMode[3], ChangeMode[2], Mode[4];
	char	*fp_p, *fn_p, *am_p;
	FILE	*ret;


	fp_p = prm_p;


	if ((prm_p = strchr(fp_p, ' ')) == NULL){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	if(prm_p-fp_p != 3){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strncpy(FilePointa, fp_p, prm_p-fp_p);
	FilePointa[prm_p-fp_p] = '\0';


	fn_p = prm_p+1;


	if ((prm_p = strchr(fn_p, ' ')) == NULL){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	if(prm_p-fn_p >= 80){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strncpy(FileName, fn_p, prm_p-fn_p);
	FileName[prm_p-fn_p] = '\0';


	am_p = prm_p+1;


	if ((prm_p = strchr(am_p, ' ')) == NULL){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	if(prm_p-am_p == 0 || prm_p-am_p > 2){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strncpy(AccessMode, am_p, prm_p-am_p);
	AccessMode[prm_p-am_p] = '\0';


	_strlwr(AccessMode);


	prm_p++;


	if(strlen(prm_p) != 1){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strcpy(ChangeMode, prm_p);


	_strlwr(ChangeMode);


	if (!(_stricmp(FilePointa, "FP1") == 0 || _stricmp(FilePointa, "FP2") == 0 ||
	      _stricmp(FilePointa, "FP3") == 0 || _stricmp(FilePointa, "FP4") == 0)){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	if(strcmp(AccessMode, "r") == 0){
		strcpy(Mode, AccessMode);
	}else if(strcmp(AccessMode, "w") == 0){
		strcpy(Mode, AccessMode);
	}else if(strcmp(AccessMode, "a") == 0){
		strcpy(Mode, AccessMode);
	}else if(strcmp(AccessMode, "r+") == 0){
		strcpy(Mode, AccessMode);
	}else if(strcmp(AccessMode, "w+") == 0){
		strcpy(Mode, AccessMode);
	}else if(strcmp(AccessMode, "a+") == 0){
		strcpy(Mode, AccessMode);
	}else{
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	if(ChangeMode[0] == 'b'){
		strcat(Mode, ChangeMode);
	}else if(ChangeMode[0] == 't'){
		strcat(Mode, ChangeMode);
	}else{
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	ret = fopen(FileName, Mode);

	if (ret == NULL){
		Errcode = -1;
	}else{
		if (_stricmp(FilePointa, "FP1") == 0){
			fp1 = ret;
		}else if (_stricmp(FilePointa, "FP2") == 0){
			fp2 = ret;
		}else if (_stricmp(FilePointa, "FP3") == 0){
			fp3 = ret;
		}else if (_stricmp(FilePointa, "FP4") == 0){
			fp4 = ret;
		}

		Errcode = 0;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = T_Fclose   						                             */
/*                                                                           */
/*  name = Library function: fclose                                          */
/*  func = (1) Checks the parameters.                                        */
/*         (2) Executes fclose.                                              */
/*                                                                           */
/*  i/o    = prm_p   :char * :IN: pointer to parameters                      */
/*           pcl_line:int    :IN: the line number of the test item file      */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL T_Fclose(char *prm_p, int pcl_line)
{
	char	FilePointa[4];
	FILE	*fp;


	if (strlen(prm_p) != 3){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strcpy(FilePointa, prm_p);


	if (_stricmp(FilePointa, "FP1") == 0){
		fp = fp1;
		fp1 = NULL;
	}else if (_stricmp(FilePointa, "FP2") == 0){
		fp = fp2;
		fp2 = NULL;
	}else if (_stricmp(FilePointa, "FP3") == 0){
		fp = fp3;
		fp3 = NULL;
	}else if (_stricmp(FilePointa, "FP4") == 0){
		fp = fp4;
		fp4 = NULL;
	}else{
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	if (fclose(fp) == EOF){
		Errcode = -1;
	}else{
		Errcode = 0;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = T_Fgetc   							                             */
/*                                                                           */
/*  name = Library function: fgetc                                           */
/*  func = (1) Checks the parameters.                                        */
/*         (2) Executes fgetc.                                               */
/*                                                                           */
/*  i/o    = prm_p   :char * :IN: pointer to parameters                      */
/*           pcl_line:int    :IN: the line number of the test item file      */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL T_Fgetc(char *prm_p, int pcl_line)
{
	char	FilePointa[4];
	FILE	*fp;


	if(strlen(prm_p) != 3){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strcpy(FilePointa, prm_p);


	if (_stricmp(FilePointa, "FP1") == 0){
		fp = fp1;
	}else if (_stricmp(FilePointa, "FP2") == 0){
		fp = fp2;
	}else if (_stricmp(FilePointa, "FP3") == 0){
		fp = fp3;
	}else if (_stricmp(FilePointa, "FP4") == 0){
		fp = fp4;
	}else{
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	fgetc(fp);

	if (ferror(fp) != 0){
		Errcode = -1;
	}else{
		Errcode = 0;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = T_Fgets   							                             */
/*                                                                           */
/*  name = Library function: fgets                                           */
/*  func = (1) Checks the parameters.                                        */
/*         (2) Executes fgets.                                               */
/*                                                                           */
/*  i/o    = prm_p   :char * :IN: pointer to parameters                      */
/*           pcl_line:int    :IN: the line number of the test item file      */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL T_Fgets(char *prm_p, int pcl_line)
{
	char	FilePointa[4], fgets_buf[17];
	FILE	*fp;


	if(strlen(prm_p) != 3){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strcpy(FilePointa, prm_p);


	if (_stricmp(FilePointa, "FP1") == 0){
		fp = fp1;
	}else if (_stricmp(FilePointa, "FP2") == 0){
		fp = fp2;
	}else if (_stricmp(FilePointa, "FP3") == 0){
		fp = fp3;
	}else if (_stricmp(FilePointa, "FP4") == 0){
		fp = fp4;
	}else{
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	fgets(fgets_buf, 17, fp);

	if (ferror(fp) != 0){
		Errcode = -1;
	}else{
		Errcode = 0;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = T_Fread   							                             */
/*                                                                           */
/*  name = Library function: fread                                           */
/*  func = (1) Checks the parameters.                                        */
/*         (2) Executes fread.                                               */
/*                                                                           */
/*  i/o    = prm_p   :char * :IN: pointer to parameters                      */
/*           pcl_line:int    :IN: the line number of the test item file      */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL T_Fread(char *prm_p, int pcl_line)
{
	char	FilePointa[4], fread_buf[16];
	FILE	*fp;


	if(strlen(prm_p) != 3){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strcpy(FilePointa, prm_p);


	if (_stricmp(FilePointa, "FP1") == 0){
		fp = fp1;
	}else if (_stricmp(FilePointa, "FP2") == 0){
		fp = fp2;
	}else if (_stricmp(FilePointa, "FP3") == 0){
		fp = fp3;
	}else if (_stricmp(FilePointa, "FP4") == 0){
		fp = fp4;
	}else{
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	fread(fread_buf, 1, 16, fp);

	if (ferror(fp) != 0){
		Errcode = -1;
	}else{
		Errcode = 0;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = T_Fputc   							                             */
/*                                                                           */
/*  name = Library function: fputc                                           */
/*  func = (1) Checks the parameters.                                        */
/*         (2) Executes fputc.                                               */
/*                                                                           */
/*  i/o    = prm_p   :char * :IN: pointer to parameters                      */
/*           pcl_line:int    :IN: the line number of the test item file      */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL T_Fputc(char *prm_p, int pcl_line)
{
	char	FilePointa[4];
	FILE	*fp;


	if (strlen(prm_p) != 3){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strcpy(FilePointa, prm_p);


	if (_stricmp(FilePointa, "FP1") == 0){
		fp = fp1;
	}else if (_stricmp(FilePointa, "FP2") == 0){
		fp = fp2;
	}else if (_stricmp(FilePointa, "FP3") == 0){
		fp = fp3;
	}else if (_stricmp(FilePointa, "FP4") == 0){
		fp = fp4;
	}else{
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	fputc('R', fp);

	if (ferror(fp) != 0){
		Errcode = -1;
	}else{
		Errcode = 0;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = T_Fputs  							                             */
/*                                                                           */
/*  name = Library function: fputs                                           */
/*  func = (1) Checks the parameters.                                        */
/*         (2) Executes fputs.                                               */
/*                                                                           */
/*  i/o    = prm_p   :char * :IN: pointer to parameters                      */
/*           pcl_line:int    :IN: the line number of the test item file      */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL T_Fputs(char *prm_p, int pcl_line)
{
	char	FilePointa[4];
	char	fputs_buf[17] = "0123456789ABCDEF";
	FILE	*fp;


	if (strlen(prm_p) != 3){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strcpy(FilePointa, prm_p);


	if (_stricmp(FilePointa, "FP1") == 0){
		fp = fp1;
	}else if (_stricmp(FilePointa, "FP2") == 0){
		fp = fp2;
	}else if (_stricmp(FilePointa, "FP3") == 0){
		fp = fp3;
	}else if (_stricmp(FilePointa, "FP4") == 0){
		fp = fp4;
	}else{
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	fputs(fputs_buf, fp);

	if (ferror(fp) != 0){
		Errcode = -1;
	}else{
		Errcode = 0;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = T_Fwrite   						                             */
/*                                                                           */
/*  name = Library function: fwrite                                          */
/*  func = (1) Checks the parameters.                                        */
/*         (2) Executes fwrite.                                              */
/*                                                                           */
/*  i/o    = prm_p   :char * :IN: pointer to parameters                      */
/*           pcl_line:int    :IN: the line number of the test item file      */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL T_Fwrite(char *prm_p, int pcl_line)
{
	char	FilePointa[4];
	char	fwrite_buf[17] = "0123456789ABCDEF";
	FILE	*fp;


	if (strlen(prm_p) != 3){
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	strcpy(FilePointa, prm_p);


	if (_stricmp(FilePointa, "FP1") == 0){
		fp = fp1;
	}else if (_stricmp(FilePointa, "FP2") == 0){
		fp = fp2;
	}else if (_stricmp(FilePointa, "FP3") == 0){
		fp = fp3;
	}else if (_stricmp(FilePointa, "FP4") == 0){
		fp = fp4;
	}else{
		printf(KDDS33105, pcl_line);
		sprintf(log_buf, KDDS33105, pcl_line);
		return FALSE;
	}


	fwrite(fwrite_buf, 1, 16, fp);

	if (ferror(fp) != 0){
		Errcode = -1;
	}else{
		Errcode = 0;
	}

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = T_Map   							                             */
/*                                                                           */
/*  name = Mapping                                                           */
/*  func = Calls a function for mapping.                                     */
/*         (1) Maps the current directory to V drive (maps the directory as  */
/*             root.                                                         */
/*         (2) Makes the root of V drive the current directory.              */
/*                                                                           */
/*  i/o    = none                                                            */
/*                                                                           */
/*  return = TRUE  : normal end                                              */
/*           FALSE : abnormal end                                            */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

BOOL T_Map()
{
 	if (mapping('v'-'a'+1, NULL) == FALSE)
		return FALSE;

	return TRUE;
}

/*** EnglishComment **********************************************************/
/*  ID     = Time_Get   						                             */
/*                                                                           */
/*  name = Getting the current time                                          */
/*  func = Gets the present time.                                            */
/*                                                                           */
/*  i/o    = none                                                            */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = August 10, 1995  by Riko Ogura                                  */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/********************************************************** EnglishComment ***/

void Time_Get()
{
	nowtime = time(NULL);
	strcpy(time_buf, ctime(&nowtime));
	time_buf[24] = ' ';
}
