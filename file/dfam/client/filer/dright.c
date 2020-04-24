#include "cli_filr.h"

struct dright_data  send_tbl;

struct rdata{
    struct  RetHeader       head;
    struct  RetRightsData   body;
};

struct rdata  r_data;

void Terminate(int code);
void PrintDirectoryInformation();
void PrintFileInformation();

// EnglishComment
/*****************************************************************************/
/*  ID   = main()                                                            */
/*  name = DRIGHT main routine                                               */
/*  func = DRIGHT.EXE main processing routine                                */
/*           Displays effective rights of the DFAM Volume file/directory.    */
/*                                                                           */
/*  i/o  = argc :int    :IN : the number of parameters                       */
/*         argv :char **:IN : parameters                                     */
/*  return =   0 (NORMAL) : normal end                                       */
/*             4 (ERROPT) : parameter error                                  */
/*            10 (NOVOL)  : volume error                                     */
/*            97 (CABNORM): internal logic error                             */
/*           254 (CONERR) : connecton error                                  */
/*           others       : return value from chkpath()                      */
/*                        : return value from chkvol()                       */
/*                        : return value from Gateway                        */
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
    short retcode;
    int r_len;
    unsigned short v_type;
    unsigned short v_err;
    NWCONN_HANDLE v_conn;
    char v_vol[VOLLEN];
	NWCCODE nwrc;

    flg_progrm = FLG_DRIGHT;
    user_group = DEFAULT_USER;

    rtn = initialize();
    if ( rtn == FALSE ) Terminate(CONERR);

    switch ( argc ){
        case 2:
            retcode = chkpath( argv[1] , send_tbl.path_name );
            if ( retcode != NORMAL ) Terminate(retcode);
            break;
        case 1:
            retcode = chkpath( CURRENT , send_tbl.path_name );
            if ( retcode != NORMAL ) Terminate(retcode);
            break;
        default:
  			Terminate(ERROPT);
	}

    nwrc = NWParsePath(send_tbl.path_name, NULL, &v_conn, v_vol, NULL);
	if (nwrc != 0) Terminate(CONERR);

    rtn = selectNCPext( v_conn, NCP_NAME );
    if ( rtn == FALSE ) Terminate(NOVOL);

    rtn = chkvol( v_vol , v_conn , &v_type , &v_err );
    if ( rtn == FALSE ) Terminate(v_err);
    if ( v_type == NETWARE ) Terminate(NOVOL);

    strncpy(send_tbl.command_name, DRIGHT_ID, sizeof(send_tbl.command_name));
    r_len = sizeof( struct rdata );
    rtn = ncpaccess((void *)&send_tbl, sizeof(struct dright_data), (void *)&r_data, &r_len);
    if ( rtn == FALSE ) Terminate(CONERR);
    if ( r_data.head.ret_value == FALSE ) Terminate(r_data.head.DFAM_errno);

    switch ( r_data.body.dir_info ){
        case C_DIR:
			PrintDirectoryInformation();
            break;
        case C_FILE:
			PrintFileInformation();
            break;
        default:
            Terminate(CABNORM);
    }

    Terminate(NORMAL);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = Terminate                                                         */
/*  name = DRIGHT error routine                                              */
/*  func = DRIGHT.EXE error processing and exit this command.                */
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
	if (code == ERROPT) printf(DRIGHT_MSG);

	exit(code);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = PrintDirectoryInformation                                         */
/*  name = Print Directory effective Rights.                                 */
/*  func = print detailed message of effective rights for directory.         */
/*  i/o  = none                                                              */
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
void PrintDirectoryInformation()
{
    int     i;
    char    r_flg = FALSE;
    char    w_flg = FALSE;
    char    c_flg = FALSE;
    char    e_flg = FALSE;
    char    m_flg = FALSE;
    char    f_flg = FALSE;
    char    a_flg = FALSE;
    char    q_flg = FALSE;
    char    right_chr[10];

    puts(send_tbl.path_name);

    trst_chr(right_chr, r_data.body.rights_list);
    printf("\n" MSG_DRIGHT_T_LST "\n", DEFINE_DLEC_H, right_chr);

    for (i = 1; i < 9; i++) {
        switch (right_chr[i]) {
            case 'R':
                printf(MSG_DRIGHT_READ_D);
                r_flg = TRUE;
                break;
            case 'W':
                printf(MSG_DRIGHT_WRITE_D);
                w_flg = TRUE;
                break;
            case 'C':
                printf(MSG_DRIGHT_ADD_D);
                c_flg = TRUE;
                break;
            case 'E':
                printf(MSG_DRIGHT_DEL_D);
                e_flg = TRUE;
                break;
            case 'M':
                printf(MSG_DRIGHT_CHANGE_D);
                m_flg = TRUE;
                break;
            case 'F':
                printf(MSG_DRIGHT_FIND_D);
                f_flg = TRUE;
                break;
            case 'A':
                printf(MSG_DRIGHT_ACC);
                a_flg = TRUE;
                break;
            case 'Q':
                printf(MSG_DRIGHT_CH_D);
                q_flg = TRUE;
        }
    }

    if (r_flg == TRUE || w_flg == TRUE ||
		m_flg == TRUE || q_flg == TRUE) {
        printf("\n" MSG_DRIGHT_D_NON);
    }

    if (w_flg == TRUE && c_flg == TRUE && e_flg == TRUE &&
        m_flg == TRUE && f_flg == TRUE && a_flg == TRUE    ) {
        printf("\n" MSG_DRIGHT_ALL_R, DEFINE_DLEC_H);
    }
}

// EnglishComment
/*****************************************************************************/
/*  ID   = PrintFileInformation                                              */
/*  name = Print File effective Rights.                                      */
/*  func = print detailed message of effective rights for file.              */
/*  i/o  = none                                                              */
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
void PrintFileInformation()
{
    int     i;
    char    r_flg = FALSE;
    char    w_flg = FALSE;
    char    c_flg = FALSE;
    char    e_flg = FALSE;
    char    m_flg = FALSE;
    char    f_flg = FALSE;
    char    a_flg = FALSE;
    char    q_flg = FALSE;
    char    right_chr[10];

    puts(send_tbl.path_name);

    trst_chr(right_chr, r_data.body.rights_list);
    printf("\n" MSG_DRIGHT_T_LST "\n", DEFINE_FILE_H, right_chr);

    for (i = 1; i < 9; i++) {
        switch (right_chr[i]) {
            case 'R':
                printf(MSG_DRIGHT_READ_F);
                r_flg = TRUE;
                break;
            case 'W':
                printf(MSG_DRIGHT_WRITE_F);
                w_flg = TRUE;
                break;
            case 'C':
                printf(MSG_DRIGHT_ADD_F);
                c_flg = TRUE;
                break;
            case 'E':
                printf(MSG_DRIGHT_DEL_F);
                e_flg = TRUE;
                break;
            case 'M':
                printf(MSG_DRIGHT_CHANGE_F);
                m_flg = TRUE;
                break;
            case 'F':
                printf(MSG_DRIGHT_FIND_F);
                f_flg = TRUE;
                break;
            case 'A':
                printf(MSG_DRIGHT_ACC);
                a_flg = TRUE;
                break;
            case 'Q':
                printf(MSG_DRIGHT_CH_F);
                q_flg = TRUE;
        }
    }

    if (c_flg == TRUE || e_flg == TRUE || m_flg == TRUE ||
		f_flg == TRUE || q_flg == TRUE) {
        printf("\n" MSG_DRIGHT_F_NON);
    }

    if (r_flg == TRUE && w_flg == TRUE && c_flg == TRUE &&
        f_flg == TRUE && a_flg == TRUE && q_flg == TRUE    ) {
        printf("\n" MSG_DRIGHT_ALL_R, DEFINE_FILE_H);
    }
}
