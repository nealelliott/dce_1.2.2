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
 * $Log: dlogout.c,v $
 * Revision 1.1.2.2  1996/03/11  14:06:53  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:27  marty]
 *
 * Revision 1.1.2.1  1996/02/21  05:42:58  takano
 * 	First Submit
 * 	[1996/02/21  05:42:47  takano]
 * 
 * $EndLog$
 */
#include "dfam_cli.h"
// EnglishComment
/*****************************************************************************/
/*  ID   = dlogout()                                                         */
/*  name = DFAM logout (DFS logout)                                          */
/*  func = Requests DFAM Gateway to log out DFS users.                       */
/*          - Checks DFAM login with using dchk().                           */
/*          - Sends a logout command.                                        */
/*          - Checks the result.                                             */
/*  i/o  = server:char:inp: server name                                      */
/*         flg   :char:inp: processing indicator                             */
/*                            1: NetWare logout                              */
/*         msg   :char:out: result message                                   */
/*                                                                           */
/*  return =  0: normal end                                                  */
/*           -1: not logged in to NetWare                                    */
/*           -2: DFAM Gateway not started                                    */
/*           -3: not logged in to DCE                                        */
/*           -4: other errors                                                */
/*                                                                           */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = October 17, 1994 by Kentarou Chikura                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
// EnglishComment
int dlogout( char *server, char flg, char *msg )
{
    NWCONN_HANDLE   connx;
    DWORD           NCPExtensionID;
    int             rc;
    char            client[48];
    NWCCODE         rcx;
    NWCONN_NUM      connnumber;
    WORD            r_len;
    struct  LGOTSendFrame   s_data;
    struct  LGOTRecvFrame   r_data;
    char            dce_uid[1025];

    memset( dce_uid, 0x00, 1025 );
    rc = dchk( server, client, &connx,
               &NCPExtensionID, &connnumber );
    switch( rc ) {
        case DCHK_NON_LOGIN:
            dmsg( MSG_NON_LOGIN, msg );
            return( -3 );
            break;
        case DCHK_BEFORE_DCE_LOGIN:
            rc = 0;
            break;
        case DCHK_NOT_NETWARE_LOGIN :
            dmsg( MSG_NOT_NETWARE_LOGIN, msg );
            return( rc );
            break;
        case DCHK_NON_DF_GW :
            dmsg( MSG_NON_DF_GW, msg, server );
            return( rc );
            break;
        case DCHK_NW_SERVER_ERR :
            dmsg( MSG_NW_SERVER_ERR, msg, server );
            return( -4 );
            break;
        case DCHK_SEND_RECV_ERR :
            dmsg( MSG_SEND_RECV_ERR, msg );
            return( rc );
            break;
        case DCHK_LCHK_ERROR:
            dmsg( MSG_LCHK_ERROR, msg );
            rc = -4;
            return( rc );
            break;
        case DCHK_NON_SET_DFAM:
            dmsg( MSG_NON_SET_DFAM, msg, client );
            rc = -4;
            return( rc );
            break;
        case DCHK_GW_BINDRY_ERROR:
            dmsg( MSG_GW_BINDRY_ERROR, msg );
            rc = -4;
            return( rc );
            break;
        case DCHK_GW_MEMORY_ERROR:
            dmsg( MSG_GW_MEMORY_ERROR, msg );
            rc = -4;
            return( rc );
            break;
        default:
            dmsg( MSG_LCHK_ERROR, msg );
            rc = -4;
            return( rc );
            break;
    }
    r_len = sizeof( struct LGOTRecvFrame );
    memcpy( s_data.command, LOGOUT_K, COMMAND_LEN );
    rcx = NWNCPExtensionRequest( connx,
                                NCPExtensionID,
                                (void *)&(s_data),
                                (WORD)sizeof(struct LGOTSendFrame),
                                (void *)&(r_data),
                                &r_len );
    if ( rcx != 0 ) {
        dmsg( MSG_SEND_RECV_ERR, msg );
        return( -4 );
    }
    if ( memcmp( r_data.command,
                 LOGOUT_K, COMMAND_LEN ) ) {
        dmsg( MSG_SEND_RECV_ERR, msg );
        return( -4 );
    }
    switch ( r_data.rc ) {
        case LGOT_LOGOUT_OK:
            r_data.dce_uid[r_data.len] = 0x00;
            strcpy( dce_uid, r_data.dce_uid );
            dmsg( MSG_LOGOUT_OK, msg, dce_uid );
            rc= 0;
            break;
        case LGOT_GW_MEMORY_ERROR:
            dmsg( MSG_GW_MEMORY_ERROR, msg );
            rc = -4;
            break;
        case LGOT_NON_SET_DFAM:
            dmsg( MSG_NON_SET_DFAM, msg, client );
            rc = -4;
            break;
        case LGOT_GW_BINDRY_ERROR:
            dmsg( MSG_GW_BINDRY_ERROR, msg );
            rc = -4;
            break;
        case LGOT_GW_EGT_CONN:
            dmsg( MSG_GW_EGT_CONN, msg );
            rc = -4;
            break;
        case LGOT_NON_LOGIN:
            dmsg( MSG_NON_LOGIN, msg );
            rc = -3;
            break;
        case LGOT_NON_DFAM_EGT:
            dmsg( MSG_NON_DFAM_EGT, msg );
            rc = -4;
            break;
        case LGOT_GW_ERROR_NH:
            dmsg( MSG_GW_ERROR_NH, msg );
            rc = -4;
            break;
        case LGOT_NOT_LOGOUT:
            dmsg( MSG_NON_LOGIN, msg );
            rc = -4;
            break;
        default:
            dmsg( MSG_NON_LOGIN, msg );
            rc = -4;
            break;
    }
    return( rc );
}
