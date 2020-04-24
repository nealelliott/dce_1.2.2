#include "cli_filr.h"

struct  dndir_data  send_tbl;
struct  add_data    add_data;

struct rdata{
    struct  RetHeader       head;
    struct  RetNdirData     body[DNDIR_DATNUM];
} r_data;

BOOL opt_info = FALSE;

void Terminate(int code);
void size_set(unsigned long sumsize, char *size_chr);
void PrintFileTitle();
void PrintFileInformation(int index);
void PrintDirectoryTitle();
void PrintDirectoryInformation(int index);

// EnglishComment
/*****************************************************************************/
/*  ID   = main()                                                            */
/*  name = DNDIR main routine                                                */
/*  func = DNDIR.EXE main processing routine                                 */
/*           Displays the information of DFAM Volume file/directory.         */
/*                                                                           */
/*  i/o  = argc :int    :IN : the number of parameters                       */
/*         argv :char **:IN : parameters                                     */
/*  return =   0 (NORMAL): normal end                                        */
/*             4 (ERROPT): parameter error                                   */
/*            10 (NOVOL) : volume error                                      */
/*           254 (CONERR): connecton error                                   */
/*           others      : return value from chkpath()                       */
/*                       : return value from chkvol()                        */
/*                       : return value from Gateway                         */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = March 7, 1995 by Kousuke Takano and Kentarou Chikura              */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void main( int argc , char *argv[] )
{
    BOOL rtn;
    int i ;
    short retcode;
    unsigned long sumsize = 0;
    unsigned long fcount = 0;
    unsigned long dcount = 0;
    WORD    r_len;
    unsigned short v_type;
    unsigned short v_err;
    NWCONN_HANDLE v_conn;
    char v_vol[VOLLEN];
    char    path_flg = FALSE;
    char    size_chr[14];
	NWCCODE nwrc;

    flg_progrm = FLG_DNDIR;
    user_group = DEFAULT_USER;

    rtn = initialize();
    if ( rtn == FALSE ) Terminate(CONERR);

    for(i = 1; i < argc ; i ++ ) {
        if (!stricmp(argv[i], "/n")) {
            opt_info = TRUE;
        } else {
            if (path_flg != FALSE) Terminate(ERROPT);

            retcode = chkpath(argv[i], send_tbl.path_name);
            if (retcode != NORMAL) Terminate(retcode);

            path_flg = TRUE;
        }
    }

    if( path_flg == FALSE ) {
        retcode = chkpath( CURRENT, send_tbl.path_name );
        if ( retcode != NORMAL ) Terminate(retcode);
    }

    nwrc = NWParsePath(send_tbl.path_name, NULL, &v_conn, v_vol, NULL);
    if (nwrc != 0) Terminate(CONERR);

    rtn = selectNCPext( v_conn, NCP_NAME );
    if ( rtn == FALSE ) Terminate(NOVOL);

    rtn = chkvol( v_vol , v_conn , &v_type , &v_err );
    if ( rtn == FALSE ) Terminate(v_err);
    if ( v_type == NETWARE ) Terminate(NOVOL);

    memset(send_tbl.option, 0x00, sizeof(send_tbl.option));
    strncpy(send_tbl.command_name, DNDIR_ID, sizeof(send_tbl.command_name));
    puts(send_tbl.path_name);

    r_len = sizeof(struct rdata);
    rtn = ncpaccess((void *)&send_tbl, sizeof(struct dndir_data), (void *)&r_data, &r_len);
    if ( rtn == FALSE ) Terminate(CONERR);

    strcpy(size_chr, "            0");

    if ( r_data.head.ret_value == FALSE ) {
        if (r_data.head.DFAM_errno != NOENT) Terminate(r_data.head.DFAM_errno);

        printf(MSG_DNDIR_OK, size_chr, fcount);
        Terminate(NORMAL);
    }

    if ( r_data.head.DFAM_errno == HEADONLY ) {
        printf( MSG_DNDIR_OK, size_chr, fcount );
        Terminate(NORMAL);
    }

    i = -1;
    do {
        i++;
        if ( r_data.body[i].dir_info == C_FILE ){
            sumsize += r_data.body[i].file_size;
            fcount ++;
            if (fcount == 1) PrintFileTitle(opt_info);
			PrintFileInformation(i);
        } else {
            dcount ++;
            if (dcount == 1) PrintDirectoryTitle();
			PrintDirectoryInformation(i);
        }
    } while( r_data.body[i].next_data != FALSE );

    strncpy(add_data.command_name, DNDIR_ID, sizeof(add_data.command_name));
    while ( r_data.head.next_frame != FALSE ) {
        r_len = sizeof(struct rdata);
        rtn = ncpaccess((void *)&add_data, sizeof(struct add_data), (void *)&r_data, &r_len);
	    if ( rtn == FALSE ) Terminate(CONERR);
        if ( r_data.head.ret_value == FALSE ) Terminate(r_data.head.DFAM_errno);

        i = -1;
        do {
            i++;
            if ( r_data.body[i].dir_info == C_FILE ){
                sumsize += r_data.body[i].file_size;
                fcount ++;
                if (fcount == 1) PrintFileTitle(opt_info);
				PrintFileInformation(i);
            } else {
                dcount ++;
                if (dcount == 1) PrintDirectoryTitle();
				PrintDirectoryInformation(i);
            }
        } while( r_data.body[i].next_data != FALSE );
    }

    size_set( sumsize, size_chr );
    printf("\n" MSG_DNDIR_OK, size_chr, fcount);

    Terminate(NORMAL);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = Terminate                                                         */
/*  name = DNDIR error routine                                               */
/*  func = DNDIR.EXE error processing and exit this command.                 */
/*  i/o  = code :int    :IN : error code                                     */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = May 27, 1996                                                      */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void Terminate(int code)
{
	char text[335];

	if (code == NORMAL) exit(NORMAL);

	msg_lst(code, text);
	printf(text);
	if (code == ERROPT) printf(DNDIR_MSG);

	exit(code);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = size_set                                                          */
/*  name = format number                                                     */
/*  func = comma is inserted to number every 3 figures.                      */
/*  i/o  = sumsize  :unsigned long :IN : number                              */
/*         size_chr :char*         :OUT: formated number (string)            */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = June 17, 1996                                                     */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void size_set(unsigned long sumsize, char* size_chr)
{
    char    size[11];

    sprintf(size, "%10lu", sumsize);
    size_chr[0] = size[0];
    size_chr[1] = (size[0] == ' ') ? ' ' : ',';
    strcpy(size_chr+2,  size+1);
    size_chr[5] = (size[3] == ' ') ? ' ' : ',';
    strcpy(size_chr+6,  size+4);
    size_chr[9] = (size[6] == ' ') ? ' ' : ',';
    strcpy(size_chr+10, size+7);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = PrintFileTitle()                                                  */
/*  name = print file title.                                                 */
/*  func = Displays the title of directory information in DFAM volume.       */
/*  i/o  = none                                                              */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = June 18, 1996                                                     */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void PrintFileTitle()
{
    puts("\n" DEFINE_FILE_H ":");

    if (opt_info == TRUE) {
        printf(DNDIR_FILE_DFS_T1);
        printf(DNDIR_FILE_DFS_T2);
    } else {
        printf(DNDIR_FILE_NODFS_T1);
        printf(DNDIR_FILE_NODFS_T2);
    }
}

// EnglishComment
/*****************************************************************************/
/*  ID   = PrintFileInformation()                                            */
/*  name = print file information.                                           */
/*  func = Displays the directory information in DFAM volume.                */
/*  i/o  = none                                                              */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = June 18, 1996                                                     */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void PrintFileInformation(int i)
{
    struct  tm  *time_tbl;
    char    time_chr[20];
    char    us_name[14];
    char    *p_file;
    char    *p_file2;
    char    p_dfile[24];
    char    *p_dce_p;
    char    size_chr[14];

    time_tbl = localtime(&(r_data.body[i].change_date)) ;
    if (time_tbl->tm_hour == 0)
        strftime(time_chr, sizeof(time_chr), DNDIR_TIME_00, time_tbl);
    else
        strftime(time_chr, sizeof(time_chr), DNDIR_TIME, time_tbl);

    if (time_chr[9] == '0') time_chr[9] = ' ';

    if (r_data.body[i].user_name[0] == 0x01)
        strcpy(r_data.body[i].user_name, NOT_DCE_USER);
    else if (r_data.body[i].user_name[0] == 0x02)
        strcpy(r_data.body[i].user_name, NOT_GW_USER);

    memset(us_name, 0x00, sizeof(us_name));

    p_file = strrchr(r_data.body[i].path_name, ':');
    if ((p_file2 = strrchr(p_file, '\\')) != NULL) p_file = p_file2;

    strncpy(us_name, r_data.body[i].user_name, 13);

    size_set(r_data.body[i].file_size, size_chr);

    if (opt_info == TRUE) {
	    memset(p_dfile, 0x00, sizeof(p_dfile));

        p_dce_p = strrchr(r_data.body[i].dfs_name, '/');
        strncpy(p_dfile, p_dce_p+1, 21);

        printf("%-12s %-21s %13s %16s %-13s\n", p_file+1, p_dfile, size_chr, time_chr, us_name);
    } else {
        printf("%-12s %13s %16s %-13s\n", p_file+1, size_chr, time_chr, us_name);
    }
}

// EnglishComment
/*****************************************************************************/
/*  ID   = PrintDirectoryTitle()                                             */
/*  name = print directory title.                                            */
/*  func = Displays the title of directory information in DFAM volume.       */
/*  i/o  = none                                                              */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = June 18, 1996                                                     */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void PrintDirectoryTitle()
{
    puts("\n" DEFINE_DLEC_H ":");

    if (opt_info == TRUE) {
        printf(DNDIR_DIR_DFS_T1);
        printf(DNDIR_DIR_DFS_T2);
    } else {
        printf(DNDIR_DIR_NODFS_T1);
        printf(DNDIR_DIR_NODFS_T2);
    }
}

// EnglishComment
/*****************************************************************************/
/*  ID   = PrintDirectoryInformation()                                       */
/*  name = print directory information.                                      */
/*  func = Displays the directory information in DFAM volume.                */
/*  i/o  = none                                                              */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = June 18, 1996                                                     */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void PrintDirectoryInformation(int i)
{
    struct  tm  *time_tbl;
    char    time_chr[20];
    char    us_name[14];
    char    right_chr[10];
    char    *p_file;
    char    *p_file2;
    char    p_dfile[24];
    char    *p_dce_p;

    time_tbl = localtime( &(r_data.body[i].change_date) ) ;
    if (time_tbl->tm_hour == 0)
        strftime(time_chr, sizeof(time_chr), DNDIR_TIME_00, time_tbl);
    else
        strftime(time_chr, sizeof(time_chr), DNDIR_TIME, time_tbl);

    if (time_chr[9] == '0') time_chr[9] = ' ';

    if (r_data.body[i].user_name[0] == 0x01)
        strcpy(r_data.body[i].user_name, NOT_DCE_USER);
    else if (r_data.body[i].user_name[0] == 0x02)
        strcpy(r_data.body[i].user_name, NOT_GW_USER);

    memset(us_name, 0x00, sizeof(us_name));

    p_file = strrchr(r_data.body[i].path_name, ':');
    if ((p_file2 = strrchr(p_file, '\\')) != NULL) p_file = p_file2;

    strncpy(us_name, r_data.body[i].user_name, 13);

    trst_chr(right_chr, r_data.body[i].rights_list);

    if (opt_info == TRUE) {
	    memset(p_dfile, 0x00, sizeof(p_dfile));

        p_dce_p = strrchr(r_data.body[i].dfs_name, '/');
        strncpy(p_dfile, p_dce_p+1, 23);

        printf("%-12s %-23s [%s] %-13s %s\n", p_file+1, p_dfile, right_chr, us_name, time_chr);
    } else {
        printf("%-12s [%s] %-13s %s\n", p_file+1, right_chr, us_name, time_chr);
    }
}
