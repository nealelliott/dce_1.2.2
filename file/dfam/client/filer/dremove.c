#include "cli_filr.h"

struct  dremove_data    send_tbl;
struct  add_data        adddata;

struct rdata{
    struct  RetHeader       head;
    struct  RetRemoveData   body[DATNUM];
} r_data;

int     argc;
char    **argv = NULL;
int     i = 1;
unsigned long fcount = 0;
unsigned long dcount = 0;
char	wildcard = FALSE;

void Terminate(int code);
void OperandTrusteeName();
void OperandPath();
void PrintInformation();

// EnglishComment
/*****************************************************************************/
/*  ID   = main()                                                            */
/*  name = DREMOVE main routine                                              */
/*  func = DREMOVE.EXE main processing routine                               */
/*           Removes trustee rights from the trustee list in the DFAM        */
/*           Volume file/directory.                                          */
/*                                                                           */
/*  i/o  = targc :int    :IN : the number of parameters                      */
/*         targv :char **:IN : parameters                                    */
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
void main( int targc , char *targv[] )
{
    BOOL rtn;
    short retcode;
    int     r_len;
    char    msg_bak[335];                                       
    unsigned short v_type;
    unsigned short v_err;
    NWCONN_HANDLE v_conn;
    char v_vol[VOLLEN];
	NWCCODE nwrc;

    flg_progrm = FLG_DREMOVE;
	argc = parm_pru(targc, targv, &argv);
    if (argc == -1) Terminate(MEMERR);

    user_group = DEFAULT_USER;

    if (argc == 1 || argc > 6) Terminate(ERROPT);

    rtn = initialize();
    if ( rtn == FALSE ) Terminate(CONERR);

	OperandTrusteeName();

    if (++i < argc) {
        if (stricmp(argv[i], "from")) Terminate(ERROPT);

		OperandPath();
        if (++i < argc) Terminate(ERROPT);
    } else {
        retcode = chkpath( CURRENT, send_tbl.path_name );
        if (retcode != NORMAL) Terminate(retcode);
    }

    nwrc = NWParsePath(send_tbl.path_name, NULL, &v_conn, v_vol, NULL);
	if (nwrc != 0) Terminate(CONERR);

    rtn = selectNCPext( v_conn, NCP_NAME );
    if ( rtn == FALSE ) Terminate(NOVOL);

    rtn = chkvol( v_vol , v_conn , &v_type , &v_err );
    if ( rtn == FALSE ) Terminate(v_err);
    if ( v_type == NETWARE ) Terminate(NOVOL);

    strncpy(send_tbl.command_name, DREMOVE_ID, sizeof(send_tbl.command_name));
    r_len = sizeof(struct rdata);

    if ( send_tbl.ug_class == DEFAULT_BOTH ) {
        send_tbl.ug_class = DEFAULT_USER;
        rtn = ncpaccess((void *)&send_tbl, sizeof(struct dremove_data), (void *)&r_data, &r_len);
	    if ( rtn == FALSE ) Terminate(CONERR);

        if ( r_data.head.ret_value == FALSE ) {
            if (r_data.head.DFAM_errno != NOUSER   &&
                r_data.head.DFAM_errno != NODFAMUSER) Terminate(r_data.head.DFAM_errno);

            user_group = send_tbl.ug_class;
            msg_lst( r_data.head.DFAM_errno, msg_bak );

            send_tbl.ug_class = DEFAULT_GROUP;
            user_group = send_tbl.ug_class;
            rtn = ncpaccess((void *)&send_tbl, sizeof(struct dremove_data), (void *)&r_data, &r_len);
		    if (rtn == FALSE) Terminate(CONERR);
            if (r_data.head.ret_value == FALSE &&
                 (r_data.head.DFAM_errno == NOGRP ||
                  r_data.head.DFAM_errno == NODFAMUSER)) printf(msg_bak);
        }
    } else {
        user_group = send_tbl.ug_class;
        rtn = ncpaccess((void *)&send_tbl, sizeof(struct dremove_data), (void *)&r_data, &r_len);
	    if ( rtn == FALSE ) Terminate(CONERR);
    }

    if ( r_data.head.ret_value == FALSE ) Terminate(r_data.head.DFAM_errno);
    if ( r_data.head.DFAM_errno == HEADONLY ) Terminate(HEADONLY);

    user_group = DEFAULT_USER;
    fcount = dcount = 0;

    strncpy(adddata.command_name, DREMOVE_ID, sizeof(adddata.command_name));
	PrintInformation();

    while ( r_data.head.next_frame != FALSE ) {
        r_len = sizeof(struct rdata);
        rtn = ncpaccess((void *)&adddata, sizeof(struct add_data), (void *)&r_data, &r_len);
        if ( rtn == FALSE ) Terminate(CONERR);
        if ( r_data.head.ret_value == FALSE ) Terminate(r_data.head.DFAM_errno);

        PrintInformation();
    }

    if (fcount > 0 || wildcard != FALSE) printf(MSG_REMOVE_F_CNT, send_tbl.name, fcount);
    if (dcount > 0 || wildcard != FALSE) printf(MSG_REMOVE_D_CNT, send_tbl.name, dcount);

    Terminate(NORMAL);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = Terminate                                                         */
/*  name = DREMOVE error routine                                             */
/*  func = DREMOVE.EXE error processing and exit this command.               */
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

	free((void*)argv);
	if (code == NORMAL) exit(NORMAL);

	msg_lst(code, text);
	printf(text);
	if (code == ERROPT) printf(DREMOVE_MSG);

	if (code == HEADONLY) exit(NORMAL);
	exit(code);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = OperandPath                                                       */
/*  name = analyze command line operand of path.                             */
/*  func = operand "for xxx" process.                                        */
/*  i/o  = none.                                                             */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = July 12, 1996                                                     */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void OperandPath()
{
    short retcode;

    if (++i >= argc) Terminate(ERROPT);

    retcode = chkpath(argv[i], send_tbl.path_name);
    if (retcode != NORMAL) Terminate(retcode);

	if (strchr(argv[i], '*') != NULL || strchr(argv[i], '?') != NULL) wildcard = TRUE;
}

// EnglishComment
/*****************************************************************************/
/*  ID   = OperandTrusteeName                                                */
/*  name = analyze command line operand of trustee name.                     */
/*  func = operand "user xxx", "group xxx", "xxx" process.                   */
/*  i/o  = none.                                                             */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = July 12, 1996                                                     */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void OperandTrusteeName()
{
    if (!stricmp(argv[i], "group")) {
        send_tbl.ug_class = DEFAULT_GROUP;
        if (++i >= argc) Terminate(ERROPT);
    } else if (!stricmp(argv[i], "user")) {
        send_tbl.ug_class = DEFAULT_USER;
        if (++i >= argc) Terminate(ERROPT);
    } else {
        send_tbl.ug_class = DEFAULT_BOTH;
    }

    if (strlen(argv[i]) > 47) Terminate(ERROPT);

    strupr(argv[i]);

    strcpy(send_tbl.name, argv[i]);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = PrintInformation                                                  */
/*  name = Print trustee information.                                        */
/*  func = print path and trustee right.                                     */
/*  i/o  = none.                                                             */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = July 15, 1996                                                     */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void PrintInformation()
{
	int		i = -1;
    char	*ug_str;
    char	*dir_str;

    switch ( send_tbl.ug_class ){
        case DEFAULT_USER:
            ug_str = DEFINE_USER_H;
            break;
        case DEFAULT_GROUP:
            ug_str = DEFINE_GROUP_H;
            break;
        default:
            Terminate(CABNORM);
    }

    do {
        i++;
        switch (r_data.body[i].dir_info) {
            case C_DIR:
                dir_str = DEFINE_DLEC_H;
                dcount++;
                break;
            case C_FILE:
                dir_str = DEFINE_FILE_H;
                fcount++;
                break;
            default:
	            Terminate(CABNORM);
        }

        puts(r_data.body[i].path_name);
        printf(MSG_REMOVE_T_DEL, ug_str, send_tbl.name, dir_str);
    } while (r_data.body[i].next_data != FALSE);
}
