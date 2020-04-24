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
 * $Log: m_dlist.c,v $
 * Revision 1.1.2.2  1996/03/11  14:06:58  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:30  marty]
 *
 * Revision 1.1.2.1  1996/02/21  05:49:49  takano
 * 	First Submit
 * 	[1996/02/21  05:49:43  takano]
 * 
 * $EndLog$
 */
#include "dfam_cli.h"

// EnglishComment
/*****************************************************************************/
/*  ID   = main()                                                            */
/*  name = DLIST main                                                        */
/*  func = DLIST.EXE main processing function                                */
/*          - Gets a server list.                                            */
/*          - Displays a server list.                                        */
/*                                                                           */
/*  i/o  = argc:int    :IN: the number of parameters                         */
/*         argv:char **:IN: the parameters                                   */
/*                                                                           */
/*  return =  0: target server                                               */
/*           >0: the number of target servers                                */
/*           -4: other errors                                                */
/*                                                                           */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = October 24, 1994 by Kentarou Chikura                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
// EnglishComment
int main( int argc, char **argv )
{
    int             rc, i;
    char            msg[1125];
    struct  DLIST   *lst_buf;
    int             maxNum;
    NWCCODE         rcx;
    char            *midashi1_1 =
         DLIST_T1
         DLIST_T2;

    struct  SLIST1  {
            char    server[47];
            char    dm1[1];
            char    deflt[7];
            char    dm2[1];
            char    n_log[5];
            char    dm3[3];
            char    d_log[5];
            char    n2[3];
        }   s_list = { "                                               ", " ",
                       "       ", " ", "     ", "   ", "     ", "\x0d\x0a\x00"    };

    rcx = NWCallsInit( NULL , NULL );
    if ( rcx != (NWCCODE)0 ) {
        dmsg( MSG_SEND_RECV_ERR, msg );
        printf( "%s", msg );
        return( -4 );
    }
    lst_buf = (struct  DLIST  *)NULL;
    if ( ( rc = dlist( &lst_buf, msg ) ) > 0 ) {
        maxNum = rc;
        printf( midashi1_1 );
        for ( i=0 ; i < maxNum ; i++ ) {
            memset( s_list.server, 0x20, 47 );
            memcpy( s_list.server, lst_buf[i].server,
                    strlen(lst_buf[i].server) );
            if ( lst_buf[i].dfl_flg )
                memcpy( s_list.deflt, DLIST_DEFAULT,
                        strlen(DLIST_DEFAULT) );
            else
                memcpy( s_list.deflt, DLIST_DEFAULT_NON,
                        strlen(DLIST_DEFAULT) );
            if ( lst_buf[i].DCE_flg )
                memcpy( s_list.d_log, DLIST_LOGIN,
                        strlen(DLIST_LOGIN) );
            else
                memcpy( s_list.d_log, DLIST_LOGIN_NON,
                        strlen(DLIST_LOGIN) );
            if ( lst_buf[i].NW_flg )
                memcpy( s_list.n_log, DLIST_LOGIN,
                        strlen(DLIST_LOGIN) );
            else
                memcpy( s_list.n_log, DLIST_LOGIN_NON,
                        strlen(DLIST_LOGIN) );
            printf( "%s", &(s_list) );
        }
        free( lst_buf );
    }
    printf( "%s", msg );
    return( rc );
}
