#include "dfam_cli.h"

// EnglishComment
/*****************************************************************************/
/*  ID   = main()                                                            */
/*  name = DLOGIN main                                                       */
/*  func = DLOGIN.EXE main processing function                               */
/*          - Checks parameters.                                             */
/*          - Handles login with using dlogin().                             */
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
    NWCONN_HANDLE   connx;
    CONNECT_INFO    conninfo;
    NWSTRUCT_SIZE   conni_size;
    char msg[1125];
    int rc;
    NWCCODE         rcx;
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
    if ( targc == 1 ) {
        rcx = NWGetDefaultConnectionID( &connx );
        if ( rcx != (NWCCODE)0 ) {
			free(targv);
            dmsg( MSG_SEND_RECV_ERR, msg );
            printf( "%s", msg );
            return( -4 );
        }
        conni_size = sizeof( CONNECT_INFO );
        rcx = NWGetConnectionStatus( connx, &conninfo,
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
        rc = dlogin( conninfo.serverName, msg );
    } else if ( targc == 2 ) {
        if( strlen( targv[1] ) > 47 ) {
            dmsg( MSG_PARM_ERR, msg );
            rc = -4;
        } else
            rc = dlogin( targv[1], msg );
    } else {
        dmsg( MSG_PARM_ERR, msg );
        rc = -4;
    }
    free( targv );
    printf( "%s", msg );
    return( rc );
}

