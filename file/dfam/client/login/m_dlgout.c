#include "dfam_cli.h"

// EnglishComment
/*****************************************************************************/
/*  ID   = main()                                                            */
/*  name = DLOGOUT main                                                      */
/*  func = DLOGOUT.EXE main processing function                              */
/*          - Handles parameters                                             */
/*          - Handles logout with using dlogout().                           */
/*                                                                           */
/*  i/o  = argc:int    :IN: the number of parameters                         */
/*         argv:char **:IN: the parameters                                   */
/*                                                                           */
/*  return =  0: successfully logged out                                     */
/*           -1: not logged in to NetWare                                    */
/*           -2: DFAM Gateway not started                                    */
/*           -3: not logged in to DCE                                        */
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
    char            msg[1125];
    int             rc, rc_b;
    struct  DLIST   *lst_buf;
    int             i;
    char            l_flg;
    int             sl_cnt;
    int             dfl_pot;
    int     targc;
    char    **targv;
    char    *chk_pot;
    int     cnt;
    NWCCODE         rcx;                                        

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
    switch ( targc ) {
        case 1:
            lst_buf = (struct  DLIST  *)NULL;                   
            if ( ( sl_cnt = dlist( &lst_buf, msg ) ) > 0) {
                dfl_pot = -1;
                for ( i=0, l_flg = F_OFF, rc_b = 0 ;
                      i < (int)sl_cnt ;
                      i++                       ) {
                    if ( !( lst_buf[i].dfl_flg ) ) {
                        if ( lst_buf[i].DCE_flg &&
                             lst_buf[i].NW_flg ) {
                            dmsg( MSG_DLOGOUT_EXEC, msg,
                                    lst_buf[i].server );
                            printf( "%s", msg );
                            rc = dlogout(
                                     lst_buf[i].server,
                                     F_OFF, msg );
                            printf( "%s", msg );
                            l_flg = F_ON;
                            if ( rc != 0 )
                                rc_b = rc;
                        }
                    } else {
                        dfl_pot = i;
                    }
                }
                if ( dfl_pot != -1 &&
                     lst_buf[dfl_pot].DCE_flg &&
                     lst_buf[dfl_pot].NW_flg ) {
                    dmsg( MSG_DLOGOUT_EXEC, msg,
                            lst_buf[dfl_pot].server );
                    printf( "%s", msg );
                    rc = dlogout(
                             lst_buf[dfl_pot].server,
                             F_OFF, msg );
                    printf( "%s", msg );
                    l_flg = F_ON;
                    if ( rc != 0 )
                        rc_b = rc;
                }
                free( lst_buf );
                if ( l_flg == F_OFF ) {
                    dmsg( MSG_NON_LOGIN, msg );
                    printf( "%s", msg );
                    rc = -3;
                } if ( rc_b != 0 )
                    rc = rc_b;
            } else {
                if ( sl_cnt == 0 ) {
                    dmsg( MSG_NON_DF_GW, msg, "all servers" );  
                    printf( "%s", msg );
                    rc = -2;
                } else {
                    printf( "%s", msg );
                    rc = sl_cnt;
                }
            }
            break;
        case 2:
            if( strlen( targv[1] ) > 47 ) {
                dmsg( MSG_PARM_ERR, msg );
                rc = -4;
            } else
                rc = dlogout( targv[1], F_OFF, msg );
            printf( "%s", msg );
            break;
        default:
            dmsg( MSG_PARM_ERR, msg );
            printf( "%s", msg );
            rc = -4;
            break;
    }
    free( targv );
    return( rc );
}
