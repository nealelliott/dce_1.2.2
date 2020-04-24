#include "cli_filr.h"

struct  dtlist_data     send_tbl;
struct  add_data        add_data;

struct rdata {
    struct  RetHeader       head;
    struct  RetTlistData    body[DATNUM];
};

struct  rdata r_data;
char    path_name[NW_PATHSIZE];

void Terminate(int code);
void PrintInformation();

// EnglishComment
/*****************************************************************************/
/*  ID   = main()                                                            */
/*  name = DTLIST main routine                                               */
/*  func = DTLIST.EXE main processing routine                                */
/*           Displays trustee owners and owned trustees as a DFAM Volume     */
/*           trustee list.                                                   */
/*  i/o  = argc :int    :IN : the number of parameters                       */
/*         argv :char **:IN : parameters                                     */
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
void main( int argc , char **argv )
{
    short rtn;
    short retcode;
    int     r_len;
    unsigned short v_type;
    unsigned short v_err;
    NWCONN_HANDLE v_conn;
    char v_vol[VOLLEN];
	NWCCODE nwrc;

    flg_progrm = FLG_DTLIST;
    user_group = DEFAULT_USER;

    rtn = initialize();
    if ( rtn == FALSE ) Terminate(CONERR);

    switch ( argc ) {
        case 3:
            retcode = chkpath( argv[1], send_tbl.path_name );
            if ( retcode != NORMAL ) Terminate(retcode);

            if ( !stricmp( argv[2], "groups" ) ) {
                send_tbl.ug_class = DEFAULT_GROUP;
            } else if( !stricmp( argv[2], "users" ) ) {
                send_tbl.ug_class = DEFAULT_USER;
            } else
				Terminate(ERROPT);
            break;
        case 2:
            retcode = chkpath( argv[1], send_tbl.path_name );
            if ( retcode != NORMAL ) Terminate(retcode);

            send_tbl.ug_class = DEFAULT_BOTH;
            break;
        case 1:
            retcode = chkpath( CURRENT, send_tbl.path_name );
            if ( retcode != NORMAL ) Terminate(retcode);

            send_tbl.ug_class = DEFAULT_BOTH;
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

    strncpy(send_tbl.command_name, DTLIST_ID, sizeof(send_tbl.command_name));
    r_len = sizeof(struct rdata);

    rtn = ncpaccess((void *)&send_tbl, sizeof(struct dtlist_data), (void *)&r_data, &r_len);
    if ( rtn == FALSE ) Terminate(CONERR);
    if ( r_data.head.ret_value == FALSE ) Terminate(r_data.head.DFAM_errno);
    if ( r_data.head.DFAM_errno == HEADONLY ) Terminate(HEADONLY);

    path_name[0] = '\0';
	PrintInformation();

    strncpy(add_data.command_name, DTLIST_ID, sizeof(add_data.command_name));
    while ( r_data.head.next_frame != FALSE ) {
        r_len = sizeof(struct rdata);
        rtn = ncpaccess((void *)&add_data, sizeof(struct add_data), (void *)&r_data, &r_len);
        if ( rtn == FALSE ) Terminate(CONERR);
        if ( r_data.head.ret_value == FALSE ) Terminate(r_data.head.DFAM_errno);

		PrintInformation();
    }

    Terminate(NORMAL);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = Terminate                                                         */
/*  name = DTLIST error routine                                              */
/*  func = DTLIST.EXE error processing and exit this command.                */
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
	if (code == ERROPT) printf(DTLIST_MSG);

	if (code == HEADONLY) exit(NORMAL);
	exit(code);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = PrintInformation                                                  */
/*  name = Print trustee information                                         */
/*  func = (1) print title of trustee type. (user or group)                  */
/*         (2) list trustee name and trustee right.                          */
/*  i/o  = none                                                              */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = June 24, 1996                                                     */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void PrintInformation()
{
    BOOL    u_noinfo;
    BOOL    g_noinfo;
    int     i = -1;
    char    right_chr[10];

    do {
        i++;

        if ( strcmp( r_data.body[i].path_name , path_name ) ) {
            strcpy(path_name, r_data.body[i].path_name);
            printf("\n%s\n", path_name);
            u_noinfo = TRUE;
            g_noinfo = TRUE;
        }

        switch( send_tbl.ug_class ) {
            case DEFAULT_USER:
                if (r_data.body[i].user_name[0] != '\0') {
                    if ( u_noinfo == TRUE ) printf( DEFINE_USER_T );

                    trst_chr( right_chr, r_data.body[i].t_info );
                    printf( MSG_DTLIST_OK, r_data.body[i].user_name , right_chr );
                    u_noinfo = FALSE;
                } else {
                    if ( u_noinfo == TRUE ) printf( MSG_NON_USER_TR );

                    u_noinfo = TRUE;
                }
                break;
            case DEFAULT_GROUP:
                if (r_data.body[i].user_name[0] != '\0') {
                    if ( g_noinfo == TRUE ) printf( DEFINE_GROUP_T );

                    trst_chr( right_chr, r_data.body[i].t_info );
                    printf( MSG_DTLIST_OK, r_data.body[i].user_name , right_chr );
                    g_noinfo = FALSE;
                } else {
                    if ( g_noinfo == TRUE ) printf( MSG_NON_GRP_TR );

                    g_noinfo = TRUE;
                }
                break;
            case DEFAULT_BOTH:
                if (r_data.body[i].user_name[0] != '\0') {
                    if (u_noinfo == TRUE && r_data.body[i].ug_info < DTHRESHLD) {
                        printf( DEFINE_USER_T );
                        u_noinfo = FALSE;
                    }

                    if (g_noinfo == TRUE && r_data.body[i].ug_info > DTHRESHLD) {
                        printf( DEFINE_GROUP_T );
                        g_noinfo = FALSE;
                    }

                    trst_chr( right_chr, r_data.body[i].t_info );
                    printf( MSG_DTLIST_OK, r_data.body[i].user_name , right_chr );
                } else {
                    if (u_noinfo == TRUE && r_data.body[i].ug_info < DTHRESHLD) {
                        printf( MSG_NON_USER_TR );
                        u_noinfo = TRUE;
                    }

                    if (g_noinfo == TRUE && r_data.body[i].ug_info > DTHRESHLD) {
                        printf( MSG_NON_GRP_TR );
                        g_noinfo = TRUE;
                    }
                }
        }
    } while( r_data.body[i].next_data != FALSE );
}
