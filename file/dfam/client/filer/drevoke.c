#include "cli_filr.h"

struct  drevoke_data    send_tbl;
struct  add_data        add_data;

struct rdata{
    struct  RetHeader       head;
    struct RetRevokeData    body[DATNUM];
} r_data;

int     argc;
char    **argv = NULL;
int     i = 1;
unsigned long fcount = 0;
unsigned long dcount = 0;
char	wildcard = FALSE;

void Terminate(int code);
void OperandTrusteeRight();
void OperandPath();
void OperandTrusteeName();
void PrintInformation();

// EnglishComment
/*****************************************************************************/
/*  ID   = main()                                                            */
/*  name = DREVOKE main routine                                              */
/*  func = DREVOKE main processing routine                                   */
/*           Frees trustee rights (with remaining the user/group ACL of the  */
/*           DFAM Volume file/directory).                                    */
/*                                                                           */
/*  i/o  = targc :int    :IN : the number of parameters                      */
/*         targv :char **:IN : parameters                                    */
/*  return =   0 (NORMAL) : normal end                                       */
/*             4 (ERROPT) : parameter error                                  */
/*            10 (NOVOL)  : volume error                                     */
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
    int r_len;
    char    msg_bak[335];                                       
    unsigned short  bak_DFAM_errno;
    unsigned short v_type;
    unsigned short v_err;
    NWCONN_HANDLE v_conn;
    char v_vol[VOLLEN];
	NWCCODE nwrc;

    flg_progrm = FLG_DREVOKE;
	argc = parm_pru(targc, targv, &argv);
    if (argc == -1) Terminate(MEMERR);

    user_group = DEFAULT_USER;

    if (argc == 1 || argc > 15) Terminate(ERROPT);

    rtn = initialize();
    if ( rtn == FALSE ) Terminate(CONERR);

	OperandTrusteeRight();

    if (!stricmp(argv[i], "for")) {
		OperandPath();
        if (++i >= argc) Terminate(ERROPT);
        if (stricmp(argv[i], "from")) Terminate(ERROPT);

		OperandTrusteeName();
        if (++i < argc) Terminate(ERROPT);
    } else {
        if (stricmp(argv[i], "from")) Terminate(ERROPT);

		OperandTrusteeName();
        if (++i < argc) {
            if (stricmp(argv[i], "for")) Terminate(ERROPT);

			OperandPath();
            if (++i < argc) Terminate(ERROPT);
        } else {
            retcode = chkpath(CURRENT, send_tbl.path_name);
            if (retcode != NORMAL) Terminate(retcode);
        }
    }

    nwrc = NWParsePath(send_tbl.path_name, NULL, &v_conn, v_vol, NULL);
	if (nwrc != 0) Terminate(CONERR);

    rtn = selectNCPext( v_conn, NCP_NAME );
    if ( rtn == FALSE ) Terminate(NOVOL);

    rtn = chkvol( v_vol , v_conn , &v_type , &v_err );
    if ( rtn == FALSE ) Terminate(v_err);
    if ( v_type == NETWARE ) Terminate(NOVOL);

    strncpy(send_tbl.command_name, DREVOKE_ID, sizeof(send_tbl.command_name));
    r_len = sizeof(struct rdata);

    if ( send_tbl.ug_class == DEFAULT_BOTH ) {
        send_tbl.ug_class = DEFAULT_USER;
        rtn = ncpaccess((void *)&send_tbl, sizeof(struct drevoke_data), (void *)&r_data, &r_len);
	    if ( rtn == FALSE ) Terminate(CONERR);

        if ( r_data.head.ret_value == FALSE ) {
            if (r_data.head.DFAM_errno != NOUSER   &&
                r_data.head.DFAM_errno != NOTRLST  &&
                r_data.head.DFAM_errno != NODFAMUSER) Terminate(r_data.head.DFAM_errno);

            bak_DFAM_errno = r_data.head.DFAM_errno;
            user_group = send_tbl.ug_class;
            msg_lst( r_data.head.DFAM_errno, msg_bak );

            send_tbl.ug_class = DEFAULT_GROUP;
            user_group = send_tbl.ug_class;
            rtn = ncpaccess((void *)&send_tbl, sizeof(struct drevoke_data), (void *)&r_data, &r_len);
		    if (rtn == FALSE) Terminate(CONERR);
            if (r_data.head.ret_value == FALSE &&
                 (r_data.head.DFAM_errno == NOGRP ||
                  r_data.head.DFAM_errno == NODFAMUSER)) {
                if (bak_DFAM_errno == NOTRLST)
                    r_data.head.DFAM_errno = NOTRLST;
                else
                    printf( msg_bak );
			}
        }
    } else {
        user_group = send_tbl.ug_class;
        rtn = ncpaccess((void *)&send_tbl, sizeof(struct drevoke_data), (void *)&r_data, &r_len);
	    if ( rtn == FALSE ) Terminate(CONERR);
    }

    if ( r_data.head.ret_value == FALSE ) Terminate(r_data.head.DFAM_errno);
    if ( r_data.head.DFAM_errno == HEADONLY ) Terminate(HEADONLY);

    user_group = DEFAULT_USER;
    strncpy(add_data.command_name, DREVOKE_ID, sizeof(add_data.command_name));
	PrintInformation();

    while ( r_data.head.next_frame != FALSE ) {
        r_len = sizeof(struct rdata);
        rtn = ncpaccess((void *)&add_data, sizeof(struct add_data), (void *)&r_data, &r_len);
	    if ( rtn == FALSE ) Terminate(CONERR);
        if ( r_data.head.ret_value == FALSE ) Terminate(r_data.head.DFAM_errno);

		PrintInformation();
    }

    if (fcount > 0 || wildcard != FALSE) printf(MSG_REVOKE_F_CNT, fcount, send_tbl.name);
    if (dcount > 0 || wildcard != FALSE) printf(MSG_REVOKE_D_CNT, dcount, send_tbl.name);

    Terminate(NORMAL);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = Terminate                                                         */
/*  name = DREVOKE error routine                                             */
/*  func = DREVOKE.EXE error processing and exit this command.               */
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
	if (code == ERROPT) printf(DREVOKE_MSG);

	if (code == HEADONLY) exit(NORMAL);
	exit(code);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = OperandTrusteeRight                                               */
/*  name = analyze command line operand of trustee right.                    */
/*  func = operand "all r w c e f m a q" process.                            */
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
void OperandTrusteeRight()
{
    char opt_char1[] = "rwcemfaq";
    unsigned char t_list[TLENGTH+1];
    char    all_nright;

    all_nright = FALSE;
    if (!stricmp(argv[1], "for") || !stricmp(argv[1], "from")) Terminate(ERROPT);

    memset(t_list, 0x00, sizeof(t_list));

    if (!stricmp( argv[i], "all")) {
        strcpy(t_list, opt_char1);
        i++;
        all_nright = TRUE;
    }

    for (; stricmp(argv[i],"for") && stricmp(argv[i],"from") && i < argc; i++) {
        if (strlen(argv[i]) != 1) Terminate(ERROPT);

		argv[i][0] = tolower(argv[i][0]);
        if (strchr(opt_char1, argv[i][0]) == NULL) Terminate(ERROPT);
        if (strlen(t_list)+1 > TLENGTH || all_nright == TRUE) Terminate(ERROPT);

        strcat(t_list, argv[i]);
    }

    memcpy(send_tbl.t_list, t_list, sizeof(send_tbl.t_list));

    if (i >= argc) Terminate(ERROPT);
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
/*  func = operand "from xxx" process.                                       */
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
    if (++i >= argc) Terminate(ERROPT);

    if (!stricmp(argv[i], "user")) {
        if (++i >= argc) Terminate(ERROPT);
        send_tbl.ug_class = DEFAULT_USER;
    } else if (!stricmp( argv[i], "group")) {
        if (++i >= argc) Terminate(ERROPT);
        send_tbl.ug_class = DEFAULT_GROUP;
    } else {
        send_tbl.ug_class = DEFAULT_BOTH;
    }

    if (strlen( argv[i] ) > 47) Terminate(ERROPT);

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
    char    right_chr[10];

    do {
        i++;
        puts(r_data.body[i].path_name);

        if (r_data.body[i].dir_info == C_FILE) fcount++;
        if (r_data.body[i].dir_info == C_DIR) dcount++;

        trst_chr(right_chr, r_data.body[i].t_list);
        printf(MSG_REVOKE_T_LST , right_chr);
    } while (r_data.body[i].next_data != FALSE);
}
