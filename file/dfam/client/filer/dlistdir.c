#include "cli_filr.h"

struct  dlistdir_data   send_tbl;
struct  add_data        adddata;

struct rdata {
    struct  RetHeader       head;
    struct  RetListdirData  body[DATNUM];
};

struct recv_tbl {
    struct  RetListdirData  body;
    struct  recv_tbl        *u_next;
    struct  recv_tbl        *d_next;
    struct  recv_tbl        *t_next;
};

int		argc;
char	**argv = NULL;
struct recv_tbl *top_rtbl = NULL;
BOOL right_info = FALSE;
BOOL time_info  = FALSE;

void Terminate(int code);
void tbl_free(struct recv_tbl* pTop);
void PrintTitle();
void PrintNode(struct recv_tbl* pNode);

// EnglishComment
/*****************************************************************************/
/*  ID   = main()                                                            */
/*  name = DLISTDIR Main routine                                             */
/*  func = DLISTDIR.EXE main processing                                      */
/*           Displays the subdirectories information under the root or       */
/*           directories in the DFAM Volume.                                 */
/*  i/o  = targc :int    :IN : the number of parameters                      */
/*         targv :char **:IN : parameters                                    */
/*  return =   0 (NORMAL): normal end                                        */
/*             4 (ERROPT): parameter error                                   */
/*             7 (MEMERR): memory shortage                                   */
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
    register int i;
    int     d_cnt = 0;
    BOOL sub_info = FALSE;
    BOOL rtn;
    struct rdata recv_data;
    struct recv_tbl *topd_rtbl;
    struct recv_tbl *r_tbl = NULL;
    struct recv_tbl *rs_tbl;
    struct recv_tbl *nexttbl;
    short retcode;
    WORD    r_len;
    char    kai_flg;
    char    path_flg = FALSE;
    unsigned short v_type;
    unsigned short v_err;
    NWCONN_HANDLE v_conn;
    char v_vol[VOLLEN];
	NWCCODE nwrc;

    flg_progrm = FLG_DLISTDIR;
	argc = parm_pru(targc, targv, &argv);
    if (argc == -1) Terminate(MEMERR);

    user_group = DEFAULT_USER;

    rtn = initialize();
    if ( rtn == FALSE ) Terminate(CONERR);

    for(i = 1; i < argc; i++) {
        if (!stricmp(argv[i], "/a")) {
            right_info = TRUE;
            time_info  = TRUE;
            sub_info   = TRUE;
		} else if (!stricmp(argv[i], "/d") || !stricmp(argv[i], "/t")) {
            time_info  = TRUE;
		} else if (!stricmp(argv[i], "/s")) {
            sub_info   = TRUE;
		} else if (!stricmp(argv[i], "/e")) {
            right_info = TRUE;
        } else {
            if (path_flg != FALSE) Terminate(ERROPT);

            retcode = chkpath(argv[i], send_tbl.path_name);
            if (retcode != NORMAL) {
                if (retcode == NOENT ||
                    retcode == PATHERR ||
                    retcode == NOSERVER ||
                    retcode == INVALENT) PrintTitle();

               	Terminate(retcode);
            }

            path_flg = TRUE;
        }
    }

    if( path_flg == FALSE ) {
        retcode = chkpath( CURRENT, send_tbl.path_name );
        if ( retcode != NORMAL ) {
            if ( retcode == NOENT ||
                 retcode == PATHERR ||
                 retcode == NOSERVER ||
                 retcode == INVALENT)
				PrintTitle();

           	Terminate(retcode);
        }
    }

    nwrc = NWParsePath(send_tbl.path_name, NULL, &v_conn, v_vol, NULL);
	if (nwrc != 0) Terminate(CONERR);

    rtn = selectNCPext( v_conn, NCP_NAME );
    if ( rtn == FALSE ) Terminate(NOVOL);

    rtn = chkvol( v_vol , v_conn , &v_type , &v_err );
    if ( rtn == FALSE ) Terminate(v_err);
    if ( v_type == NETWARE ) Terminate(NOVOL);

    memset(send_tbl.option, 0x00, sizeof(send_tbl.option));
    send_tbl.option[0] = 'a';
    strncpy(send_tbl.command_name, DLISTDIR_ID, sizeof(send_tbl.command_name));
    r_len = (WORD)sizeof(struct rdata);

    rtn = ncpaccess((void *)&send_tbl, sizeof(struct dlistdir_data), (void *)&recv_data, &r_len);
    if ( rtn == FALSE ) Terminate(CONERR);
    if ( recv_data.head.ret_value == FALSE ) {
        if ( recv_data.head.DFAM_errno == NOENT ||
             recv_data.head.DFAM_errno == PATHERR ||
             recv_data.head.DFAM_errno == NOSERVER ||
             recv_data.head.DFAM_errno == INVALENT)
			PrintTitle();

		Terminate(recv_data.head.DFAM_errno);
    }

	PrintTitle();

    if ( recv_data.head.DFAM_errno == HEADONLY ) Terminate(HEADONLY);

    strncpy(adddata.command_name, DLISTDIR_ID, sizeof(adddata.command_name));
    top_rtbl = r_tbl = (struct recv_tbl *)NULL;                 
    i = -1;
    do {
        i++;
        nexttbl = (struct recv_tbl*)malloc(sizeof(struct recv_tbl));
        if (nexttbl == NULL) Terminate(MEMERR);

        if( top_rtbl == (struct recv_tbl *)NULL ) {             
            r_tbl = top_rtbl = nexttbl;
            r_tbl->u_next = r_tbl->d_next = r_tbl->t_next = (struct recv_tbl *)NULL; 
        } else {
            r_tbl->t_next = nexttbl;
            r_tbl = nexttbl;
            r_tbl->u_next = r_tbl->d_next = r_tbl->t_next = (struct recv_tbl *)NULL; 
        }

        recv_data.body[i].level = 0;
        memcpy( &(r_tbl->body), &(recv_data.body[i]), sizeof( struct  RetListdirData ) );
    } while( r_tbl->body.next_data != FALSE );

    while ( recv_data.head.next_frame != FALSE ) {
        r_len = (WORD)sizeof(struct rdata);
        rtn = ncpaccess((void *)&adddata, sizeof(struct add_data), (void *)&recv_data, &r_len);
        if ( rtn == FALSE ) Terminate(CONERR);
        if ( recv_data.head.ret_value == FALSE ) Terminate(recv_data.head.DFAM_errno);

        i = -1;
        do {
            i++;
			nexttbl = (struct recv_tbl *)malloc(sizeof(struct recv_tbl));
            if (nexttbl == NULL) Terminate(MEMERR);

            r_tbl->t_next = nexttbl;
            r_tbl = nexttbl;
            r_tbl->u_next = r_tbl->d_next = r_tbl->t_next = (struct recv_tbl *)NULL; 

            recv_data.body[i].level = 0;
            memcpy( &(r_tbl->body), &(recv_data.body[i]), sizeof( struct  RetListdirData ) );
        } while( r_tbl->body.next_data != FALSE );
    }

    r_tbl = top_rtbl;
    while ( r_tbl != ( struct recv_tbl * )NULL ) {              
        if ( sub_info == TRUE ) {
			PrintNode(r_tbl);
            d_cnt ++;

            strcpy( send_tbl.path_name, r_tbl->body.path_name );
			if (strlen(send_tbl.path_name) > sizeof(send_tbl.path_name)-5) continue;

            strcat( send_tbl.path_name, "\\*.*" );
            r_len = (WORD)sizeof(struct rdata);
            rtn = ncpaccess((void *)&send_tbl, sizeof(struct dlistdir_data), (void *)&recv_data, &r_len);
            if ( rtn == FALSE ) Terminate(CONERR);

            if ( recv_data.head.ret_value == TRUE &&
                 recv_data.head.DFAM_errno != HEADONLY ) {
                strncpy(adddata.command_name, DLISTDIR_ID, sizeof(adddata.command_name));
                topd_rtbl = rs_tbl = (struct recv_tbl * )NULL;  
                i = -1;
                do {
                    i++;
                    nexttbl = (struct recv_tbl*)malloc(sizeof(struct recv_tbl));
                    if (nexttbl == NULL) Terminate(MEMERR);

                    if( topd_rtbl == (struct recv_tbl *)NULL ) {    
                        rs_tbl = topd_rtbl = nexttbl;
                        rs_tbl->u_next = r_tbl;
                        rs_tbl->d_next = rs_tbl->t_next = (struct recv_tbl *)NULL; 
                        r_tbl->d_next = rs_tbl;
                    } else {
                        rs_tbl->t_next = nexttbl;
                        rs_tbl = nexttbl;
                        rs_tbl->u_next = r_tbl;
                        rs_tbl->d_next = rs_tbl->t_next = (struct recv_tbl *)NULL; 
                    }

                    memcpy( &(rs_tbl->body), &(recv_data.body[i]), sizeof( struct  RetListdirData ) );
                    rs_tbl->body.level = r_tbl->body.level+1;
                } while( rs_tbl->body.next_data != FALSE );

                while ( recv_data.head.next_frame != FALSE ) {
                    r_len = (WORD)sizeof(struct rdata);
                    rtn = ncpaccess((void *)&adddata, sizeof(struct add_data), (void *)&recv_data, &r_len);
                    if ( rtn == FALSE ) Terminate(CONERR);
                    if ( recv_data.head.ret_value == FALSE ) Terminate(recv_data.head.DFAM_errno);

                    i = -1;
                    do {
                        i++;
                        nexttbl = (struct recv_tbl*)malloc(sizeof(struct recv_tbl));
                        if (nexttbl == NULL) Terminate(MEMERR);

                        rs_tbl->t_next = nexttbl;
                        rs_tbl = nexttbl;
                        rs_tbl->u_next = r_tbl;
                        rs_tbl->d_next = rs_tbl->t_next = (struct recv_tbl *)NULL; 

                        memcpy( &(rs_tbl->body), &(recv_data.body[i]), sizeof( struct  RetListdirData ) );
                        rs_tbl->body.level = r_tbl->body.level+1;
                    } while( rs_tbl->body.next_data != FALSE );
                }
                r_tbl = topd_rtbl;
            } else if( ( recv_data.head.ret_value == FALSE &&
                         recv_data.head.DFAM_errno == NOENT ) ||
                       ( recv_data.head.ret_value == FALSE &&
                         recv_data.head.DFAM_errno == NORIGHT ) ||
                       ( recv_data.head.ret_value == TRUE &&
                         recv_data.head.DFAM_errno == HEADONLY ) ) {
                kai_flg = FALSE;
                while( kai_flg == FALSE ) {
                    if ( r_tbl->t_next == (struct recv_tbl *)NULL ) { 
                        if ( r_tbl->body.level == 0 ) {
                            kai_flg = TRUE;
                            nexttbl = r_tbl->t_next;
                            free ( r_tbl );
                            r_tbl = nexttbl;
                        } else {
                            nexttbl = r_tbl->u_next;
                            free ( r_tbl );
                            r_tbl = nexttbl;
                            r_tbl->d_next = (struct recv_tbl *)NULL; 
                        }
                    } else {
                        nexttbl = r_tbl->t_next;
                        if( r_tbl->u_next != (struct recv_tbl *)NULL ) 
                            (r_tbl->u_next)->d_next = nexttbl;
                        if( r_tbl == top_rtbl )
                            top_rtbl = nexttbl;
                        free ( r_tbl );
                        r_tbl = nexttbl;
                        kai_flg = TRUE;
                    }
                }
            } else
            	Terminate(recv_data.head.DFAM_errno);
        } else {
			PrintNode(r_tbl);
            d_cnt++;

            nexttbl = r_tbl->t_next;
            if( r_tbl == top_rtbl ) top_rtbl = nexttbl;
            free ( r_tbl );
            r_tbl = nexttbl;
        }
    }

    printf( MSG_DLISTDIR_OK, d_cnt );
    Terminate(NORMAL);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = Terminate                                                         */
/*  name = DLISTDIR error routine                                            */
/*  func = DLISTDIR.EXE error processing and exit this command.              */
/*  i/o  = code :int    :IN : error code                                     */
/*  return = none                                                            */
/*  common = argv :char** :IN :command line parameter list.                  */
/*           top_rtbl :struct recv_tbl* :IN :pointer of top directory node.  */
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

	tbl_free(top_rtbl);

	msg_lst(code, text);
	printf(text);
	if (code == ERROPT) printf(DLISTDIR_MSG);

	if (code == HEADONLY) exit(NORMAL);
	exit(code);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = tbl_free                                                          */
/*  name = free table memory                                                 */
/*  func = free table tree of directory node.                                */
/*  i/o  = t_tbl :struct recv_tbl* :IN :pointer of top directory node.       */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = March 7, 1995                                                     */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void    tbl_free( struct recv_tbl *t_tbl )
{
    struct recv_tbl *w_tbl;
    struct recv_tbl *b_tbl;

    w_tbl = t_tbl;
    while( w_tbl != (struct recv_tbl *)NULL ) {                 
        if( w_tbl->d_next == (struct recv_tbl *)NULL ) {        
            b_tbl = w_tbl;
            if( b_tbl->t_next == (struct recv_tbl *)NULL ) {    
                w_tbl = b_tbl->u_next;
                if( w_tbl != (struct recv_tbl *)NULL )          
                    w_tbl->d_next = (struct recv_tbl *)NULL;    
            } else {
                w_tbl = b_tbl->t_next;
            }
            free( b_tbl );
        } else {
            w_tbl = w_tbl->d_next;
        }
    }
}

// EnglishComment
/*****************************************************************************/
/*  ID   = PrintTitle                                                        */
/*  name = print title.                                                      */
/*  func = print title.                                                      */
/*  i/o  = none                                                              */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = June 6, 1996                                                      */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void PrintTitle()
{
    printf(MSG_DLISTDIR_READY, send_tbl.path_name);

    if (time_info == TRUE) printf(DLISTDIR_TIME_T1);
    if (right_info == TRUE) printf(DLISTDIR_RIGHT_T1);
    puts(DLISTDIR_DIR_T1);

    if (time_info == TRUE) printf(DLISTDIR_TIME_T2);
    if (right_info == TRUE) printf(DLISTDIR_RIGHT_T2);
    puts(DLISTDIR_DIR_T2);
}

// EnglishComment
/*****************************************************************************/
/*  ID   = PrintNode                                                         */
/*  name = print node.                                                       */
/*  func = print node (directory informations)                               */
/*  i/o  = pNode :struct recv_tbl* :IN :pointer to node                      */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = June 6, 1996                                                      */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void PrintNode(struct recv_tbl* pNode)
{
    struct  tm  *time_tbl;
    char    time_chr[20];
    char    *p_file;
    char    *p_file2;
    char    right_chr[10];

    if ( time_info == TRUE ){
        time_tbl = localtime( &(pNode->body.change_date ) ) ;
        if ( time_tbl->tm_hour == 0 )
            strftime( time_chr, 20, DLISTDIR_TIME_00, time_tbl );
        else
            strftime( time_chr, 20, DLISTDIR_TIME, time_tbl );

        if ( time_chr[11] == '0' ) time_chr[11] = ' ';
        printf( "%s  ", time_chr );
    }

    if ( right_info == TRUE ){
        trst_chr( right_chr, pNode->body.rights_list );
        printf( "[%s]  " , right_chr );
    }

    p_file = strrchr(pNode->body.path_name, ':');
    if ((p_file2 = strrchr(p_file, '\\')) != NULL) p_file = p_file2;

	p_file++;
    printf("->%*s\n", pNode->body.level*2+strlen(p_file), p_file);
}
