#include "dfam_cli.h"
// EnglishComment
/*****************************************************************************/
/*  ID   = pass_all_change()                                                 */
/*  name = Changing all the same passwords                                   */
/*  func = Changes all the passwords that are equivalent to the previous     */
/*         password (the target is only the DFAM Gateway running on the      */
/*         active servers).                                                  */
/*                                                                           */
/*  i/o  = server  :char  :inp: default server                               */
/*         old_pass:char *:inp: old password                                 */
/*         new_pass:char *:inp: new password                                 */
/*         msg     :char  :out: result message                               */
/*         key     :BYTE *:inp: key                                          */
/*                                                                           */
/*  return =  0: normal end                                                  */
/*           -4: other errors                                                */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = November 1, 1994 by Kentarou Chikura                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
// EnglishComment
int pass_all_change(    char    *def_server,
                        char    *old_pass,
                        char    *new_pass,
                        char    *msg    )
{
    int         rc;
    int         rcb;
    struct      DLIST   *lst_buf;
    int         sv_cnt;
    int         i;
    char        ps_flg;
    BYTE        key, inkey;

    lst_buf = (struct  DLIST  *)NULL;
    if ( ( rc = dlist( &lst_buf,
                         msg ) ) > 0 ) {
        sv_cnt = rc;
        for ( ps_flg=F_OFF,i=0 ; i < sv_cnt ; i++ ) {
            if ( lst_buf[i].NW_flg == F_ON &&
                 strcmp( def_server,
                         lst_buf[i].server ) ) {
                if (  !(rc = dsetpass(
                        lst_buf[i].server,
                        old_pass,
                        new_pass,
                        msg, F_OFF ) ) ) {
                    lst_buf[i].Y_flg = F_ON;
                    if ( ps_flg == F_OFF ) {
                        printf( " \n" );
                        ps_flg = F_ON;
                    }
                    printf( "%s\n",
                         lst_buf[i].server );
                } else if ( rc == -4 ) {
                    free( lst_buf );
                    return( rc );
                }
            }
        }
        if ( ps_flg == F_ON ) {
            printf( " \n" );
            dmsg( MSG_PASSWORD_ALL, msg,
                  def_server );
            printf( "%sY\x08", msg );
            key = 'Y';
            while ( ( inkey = (BYTE)_getch() )
                     != 0x0d ) {
                inkey = (BYTE)toupper( (int)inkey );
                if ( inkey=='Y' || inkey=='N' ) {
                    key = inkey;
                    printf( "%c\x08", key );
                }
            }
            strcpy( msg, "\n" );
            if ( key == 'Y' ) {
                for ( rcb=0,i=0 ; i < sv_cnt ; i++ ) {
                    if ( lst_buf[i].Y_flg == F_ON ) {
                        printf( msg );
                        rc = dsetpass( lst_buf[i].server,
                                    old_pass,
                                    new_pass,
                                    msg, F_ON );
                        if ( rcb == 0 || rc != 0 )
                            rcb = rc;
                    }
                }
                rc = rcb;
            } else {
                rc = 0;
                strcpy( msg, "\x00" );
            }
        } else {
            rc = 0;
            strcpy( msg, "\x00" );
        }
        free( lst_buf );
    }
    return( rc );
}

// EnglishComment
/*****************************************************************************/
/*  ID   = main()                                                            */
/*  name = DSETPASS main                                                     */
/*  func = DSETPASS.EXE main processing function                             */
/*          - Checks login with using dchk().                                */
/*          - Gets an old password.                                          */
/*          - Checks the old password with using dsetpass().                 */
/*          - Gets a new password.                                           */
/*          - Sets the new password with using dsetpass().                   */
/*                                                                           */
/*  i/o  = argc:int    :IN: the number of parameters                         */
/*         argv:char **:IN: the parameters                                   */
/*                                                                           */
/*  return =  0: password successfully changed                               */
/*            1: old password invalid                                        */
/*            2: new password setting error                                  */
/*           -1: not logged in to NetWare                                    */
/*           -2: DFAM Gateway not started                                    */
/*           -4: other errors                                                */
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
    char            old_pass[1025];
    char            new_pass[1025];
    char            re_new_pass[1025];
    char            def_server[48];
    int             rc;
    char            msg[1125];
    char            client[48];
    NWCONN_HANDLE   connx;
    DWORD           NCPExtensionID;
    NWCONN_NUM      connnumber;
    NWCCODE         rcx;
    CONNECT_INFO    conninfo;
    NWSTRUCT_SIZE   conni_size;
    int     targc;
    char    **targv;
    char    *chk_pot;
    int     cnt;

    rcx = NWCallsInit( NULL , NULL );
    if ( rcx != (NWCCODE)0 ) {
        dmsg( MSG_SEND_RECV_ERR, msg );
        printf( "%s", msg );
        return( -4 );
    }
    for( cnt=1 ; cnt < argc ; cnt++ ) {
        if( (chk_pot=strchr(argv[cnt],(int)0x5c)) != NULL )
            *chk_pot = 0x00;
        if( (chk_pot=strchr(argv[cnt],(int)0x2f)) != NULL )
            *chk_pot = 0x00;
    }
    if( (targc = parm_pru( argc, argv, &targv )) == -1 ) {
        dmsg( MSG_MEMORY_OVER, msg );
        printf( "%s", msg );
        return( -4 );
    }
    memset( old_pass, 0x00, 1025 );
    memset( new_pass, 0x00, 1025 );
    memset( re_new_pass, 0x00, 1025 );
    switch( targc ) {
        case 1:
            rcx = NWGetDefaultConnectionID( &connx );
            if ( rcx != (NWCCODE)0 ) {
				free(targv);
                dmsg( MSG_SEND_RECV_ERR, msg );
                printf( "%s", msg );
                return( -4 );
            }
            conni_size = sizeof( CONNECT_INFO );
            rcx = NWGetConnectionStatus( connx,
                                         &conninfo,
                                         conni_size );
            if ( rcx != (NWCCODE)0 ) {
				free(targv);
                dmsg( MSG_SEND_RECV_ERR, msg);
                printf( "%s", msg );
                return( -4 );
            }
            if ( conninfo.clientName[0] == 0x00 ) {
				free(targv);
                dmsg( MSG_NOT_NETWARE_LOGIN, msg);
                printf( "%s", msg );
                return( -1 );
            }
            strcpy( def_server, conninfo.serverName );
            break;
        case 2:
            if ( strlen( targv[1] ) > 47 ) {
				free(targv);
                dmsg( MSG_PARM_ERR, msg );
                printf( "%s", msg );
                return( -4 );
            }
            strcpy( def_server, targv[1] );
            break;
        default:
			free(targv);
            dmsg( MSG_PARM_ERR, msg );
            printf( "%s", msg );
            return( -4 );
            break;
    }
    rc = dchk( def_server, client, &connx,
               &NCPExtensionID, &connnumber );
    switch( rc ) {
        case DCHK_NON_LOGIN:
            rc = 0;
            break;
        case DCHK_BEFORE_DCE_LOGIN:
            rc = 0;
            break;
        case DCHK_NOT_NETWARE_LOGIN :
            dmsg( MSG_NOT_NETWARE_LOGIN, msg );
            rc = -1;
            break;
        case DCHK_NON_DF_GW :
            dmsg( MSG_NON_DF_GW, msg, def_server );
            rc = -2;
            break;
        case DCHK_NW_SERVER_ERR :
            dmsg( MSG_NW_SERVER_ERR, msg, def_server );
            rc = -4;
            break;
        case DCHK_SEND_RECV_ERR :
            dmsg( MSG_SEND_RECV_ERR, msg );
            rc = -4;
            break;
        case DCHK_LCHK_ERROR:
            dmsg( MSG_LCHK_ERROR, msg );
            rc = -4;
            break;
        case DCHK_NON_SET_DFAM:
            dmsg( MSG_NON_SET_DFAM, msg, client );
            rc = -4;
            break;
        case DCHK_GW_BINDRY_ERROR:
            dmsg( MSG_GW_BINDRY_ERROR, msg );
            rc = -4;
            break;
        case DCHK_GW_MEMORY_ERROR:
            dmsg( MSG_GW_MEMORY_ERROR, msg );
            rc = -4;
            break;
        default:
            dmsg( MSG_LCHK_ERROR, msg );
            rc = -4;
            break;
    }
    if ( rc == 0 ) {
        dmsg( MSG_CHANGE_PASS, msg );
        printf( msg );
        dmsg( MSG_OLD_PASS_SET, msg, def_server );
        printf( msg );
        rc = ps_input( old_pass, 1024 );
        printf( "\n" );
        if ( rc == -1 ) {
			free(targv);
            dmsg( MSG_MEMORY_OVER, msg );
            printf( msg );
            return( -4 );
        }
        rc = dsetpass( def_server, old_pass,
                       new_pass, msg, F_OFF );
        if ( rc == 0 ) {
			free(targv);
            dmsg( MSG_NEW_PASS_SET, msg, def_server );
            printf( msg );
            rc = ps_input( new_pass, 1024 );
            printf( "\n" );
            if ( rc == -1 ) {
				free(targv);
                dmsg( MSG_MEMORY_OVER, msg );
                printf( msg );
                return( -4 );
            }
            dmsg( MSG_RENEW_PASS_SET, msg, def_server );
            printf( msg );
            rc = ps_input( re_new_pass, 1024 );
            printf( "\n" );
            if ( rc == -1 ) {
				free(targv);
                dmsg( MSG_MEMORY_OVER, msg );
                printf( msg );
                return( -4 );
            }
            if ( strcmp( new_pass, re_new_pass ) ){
                dmsg( MSG_NOT_NEW_PASSWD, msg );
                rc = 2;
            } else {
                rc = dsetpass( def_server,
                               old_pass,
                               new_pass,
                               msg, F_ON );
                if ( rc == 0 ) {
                    printf( msg );
                    rc = pass_all_change( def_server,
                                     old_pass,
                                     new_pass,
                                     msg );
                }
            }
        }
    }
    free( targv );
    printf( "%s", msg );
    return( rc );
}
