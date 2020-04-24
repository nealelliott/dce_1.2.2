#include "cli_filr.h"

struct  dgrant_data     send_tbl;
struct  add_data        adddata;

struct Recieve_data {
    struct RetHeader        head;
    struct RetGrantData     body[DATNUM];
} r_data;

int     argc;
char    **argv = NULL;
int		i = 1;

void Terminate(int code);
void OperandTrusteeRight();
void OperandPath();
void OperandTrusteeName();
void PrintInformation();

// EnglishComment
/*****************************************************************************/
/*  ID   = main()                                                            */
/*  name = DGRANT main routine                                               */
/*  func = DGRANT.EXE main processing                                        */
/*           Grants/deletes the user/group trustee to file/directory.        */
/*  i/o  = targc :int    :IN : the number of parameters                      */
/*         targv :char **:IN : parameters                                    */
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
void main( int targc , char *targv[] )
{
    BOOL rtn;
    short retcode;
    WORD    r_len;
    char    msg_bak[335];                                       
    unsigned short v_type;
    unsigned short v_err;
    NWCONN_HANDLE v_conn;
    char v_vol[VOLLEN];
	NWCCODE nwrc;

    flg_progrm = FLG_DGRANT;
    argc = parm_pru(targc, targv, &argv);
    if (argc == -1) Terminate(MEMERR);

    user_group = DEFAULT_USER;

    if (argc == 1 || argc > 17) Terminate(ERROPT);

    rtn = initialize();
    if ( rtn == FALSE ) Terminate(CONERR);

	OperandTrusteeRight();

    memset(send_tbl.option, 0x00, sizeof(send_tbl.option));

    if (!stricmp(argv[i], "for")) {
		OperandPath();
        if (++i >= argc) Terminate(ERROPT);
        if (stricmp(argv[i], "to")) Terminate(ERROPT);

		OperandTrusteeName();
        if (++i < argc) Terminate(ERROPT);
    } else {
        if (stricmp(argv[i], "to")) Terminate(ERROPT);

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

    strncpy(send_tbl.command_name, DGRANT_ID, sizeof(send_tbl.command_name));
    r_len = (WORD)sizeof(struct Recieve_data);
    puts(send_tbl.path_name);

    if ( send_tbl.ug_class == DEFAULT_BOTH ) {
        send_tbl.ug_class = DEFAULT_USER;
        rtn = ncpaccess((void *)&send_tbl, (WORD)sizeof(struct dgrant_data), (void *)&r_data, &r_len);
        if ( rtn == FALSE ) Terminate(CONERR);

        if ( r_data.head.ret_value == FALSE ) {
            if (r_data.head.DFAM_errno != NOUSER   &&
                r_data.head.DFAM_errno != NODFAMUSER) Terminate(r_data.head.DFAM_errno);

            user_group = send_tbl.ug_class;
            msg_lst(r_data.head.DFAM_errno, msg_bak);

            send_tbl.ug_class = DEFAULT_GROUP;
            user_group = send_tbl.ug_class;
            rtn = ncpaccess((void *)&send_tbl, (WORD)sizeof(struct dgrant_data), (void *)&r_data, &r_len);
            if (rtn == FALSE ) Terminate(CONERR);
            if (r_data.head.ret_value == FALSE &&
                 (r_data.head.DFAM_errno == NOGRP ||
                  r_data.head.DFAM_errno == NODFAMUSER)) printf(msg_bak);
        }
    } else {
        user_group = send_tbl.ug_class;
        rtn = ncpaccess((void *)&send_tbl, (WORD)sizeof(struct dgrant_data), (void *)&r_data, &r_len);
        if ( rtn == FALSE ) Terminate(CONERR);
    }

    if ( r_data.head.ret_value == FALSE ) Terminate(r_data.head.DFAM_errno);
    if ( r_data.head.DFAM_errno == HEADONLY ) Terminate(HEADONLY);

    user_group = DEFAULT_USER;
	PrintInformation();

    strncpy(adddata.command_name, DGRANT_ID, sizeof(adddata.command_name));
    while ( r_data.head.next_frame != FALSE ) {
        r_len = (WORD)sizeof(struct Recieve_data);
        rtn = ncpaccess((void *)&adddata, (WORD)sizeof(struct add_data), (void *)&r_data, &r_len);
        if ( rtn == FALSE ) Terminate(CONERR);
        if ( r_data.head.ret_value == FALSE ) Terminate(r_data.head.DFAM_errno);

		PrintInformation();
    }

    Terminate(NORMAL);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = Terminate                                                         */
/*  name = DGRANT error routine                                              */
/*  func = DGRANT.EXE error processing and exit this command.                */
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
	if (code == ERROPT) printf(DGRANT_MSG);

	if (code == HEADONLY) exit(NORMAL);
	exit(code);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = OperandTrusteeRight                                               */
/*  name = analyze command line operand of trustee right.                    */
/*  func = operand "all only allbut n r w c e f m a q" process.              */
/*  i/o  = none.                                                             */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = June 27, 1996                                                     */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void OperandTrusteeRight()
{
	static char		kenri_lst[] = "rwcemfaq";
	char    t_list[TLENGTH+1];
	char    all_nright;

    if (!stricmp(argv[1], "for") || !stricmp(argv[1], "to")) Terminate(ERROPT);

    memset(t_list, 0x00, sizeof(t_list));

    if (!stricmp( argv[i], "only")) {
        send_tbl.right_mask = C_ONLY;
        i++;
        if (!stricmp( argv[i], "for") || !stricmp(argv[i], "to")) Terminate(ERROPT);
    } else if ( !stricmp( argv[i], "allbut" ) ) {
        send_tbl.right_mask = C_ALLBUT;
        i++;
        if (!stricmp(argv[i], "for") || !stricmp(argv[i], "to")) Terminate(ERROPT);
    } else if (!stricmp(argv[i], "all") && !stricmp(argv[i+1], "but")) {
        send_tbl.right_mask = C_ALLBUT;
        i+=2;
        if (!stricmp(argv[i], "for") || !stricmp(argv[i], "to")) Terminate(ERROPT);
    } else if (!stricmp(argv[i], "all")) {
        send_tbl.right_mask = C_NONE;
        strcpy(t_list, kenri_lst);
        i++;
        if (stricmp(argv[i], "for") && stricmp(argv[i], "to")) Terminate(ERROPT);
    } else if (!stricmp(argv[i], "no") && !stricmp(argv[i+1], "rights")) {
        send_tbl.right_mask = C_ALLBUT;
        strcpy(t_list, kenri_lst);
        i+=2;
        if (stricmp(argv[i], "for") && stricmp(argv[i], "to")) Terminate(ERROPT);
    } else {
	    all_nright = FALSE;
        send_tbl.right_mask = C_NONE;

	    for (; stricmp(argv[i],"for") && stricmp(argv[i],"to") && i < argc; i++) {
	        if (strlen( argv[i] ) != 1) Terminate(ERROPT);

			argv[i][0] = tolower(argv[i][0]);
    	    if (argv[i][0] == 'n') {
	            if (send_tbl.right_mask != C_NONE || t_list[0] != 0x00) Terminate(ERROPT);

            	send_tbl.right_mask = C_ALLBUT;
        	    strcpy(t_list, kenri_lst);
    	        all_nright = TRUE;
	        } else if (strchr(kenri_lst, argv[i][0]) != NULL) {
        	    if (strlen(t_list)+1 > TLENGTH || all_nright == TRUE) Terminate(ERROPT);

    	        strcat(t_list, argv[i]);
	        } else
				Terminate(ERROPT);
	    }
    }

    if (i >= argc) Terminate(ERROPT);

    memcpy(send_tbl.t_list, t_list, sizeof(send_tbl.t_list));
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
/*  date = June 27, 1996                                                     */
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
}

// EnglishComment
/*****************************************************************************/
/*  ID   = OperandTrusteeName                                                */
/*  name = analyze command line operand of trustee name.                     */
/*  func = operand "to xxx" process.                                         */
/*  i/o  = none.                                                             */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = June 27, 1996                                                     */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void OperandTrusteeName()
{
    if (++i >= argc) Terminate(ERROPT);

    if (!stricmp(argv[i], "group")) {
        if (++i >= argc) Terminate(ERROPT);
        send_tbl.ug_class = DEFAULT_GROUP;
    } else if (!stricmp(argv[i], "user")) {
        if (++i >= argc) Terminate(ERROPT);
        send_tbl.ug_class = DEFAULT_USER;
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
    char	*p_file;
    char	*p_file2;
    char	right_chr[10];

    do {
        i++;

        p_file = strrchr(r_data.body[i].path_name, ':');
        if ((p_file2 = strrchr(p_file, '\\')) != NULL) p_file = p_file2;

        trst_chr(right_chr, r_data.body[i].t_list);
        printf(MSG_DGRANT_OK, p_file+1, right_chr);
    } while (r_data.body[i].next_data != FALSE);
}
