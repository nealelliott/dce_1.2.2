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
 * $Log: dlogin.c,v $
 * Revision 1.1.2.2  1996/03/11  14:06:52  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:26  marty]
 *
 * Revision 1.1.2.1  1996/02/21  05:42:26  takano
 * 	First Submit
 * 	[1996/02/21  05:42:21  takano]
 * 
 * $EndLog$
 */
#include "dfam_cli.h"
// EnglishComment
/*****************************************************************************/
/*  ID   = dlogin()                                                          */
/*  name = DFS login                                                         */
/*  func = Requests DFS login to DFAM Gateway.                               */
/*          - Checks DFAM login with using dchk().                           */
/*          - Sends a logout command.                                        */
/*          - Checks the result.                                             */
/*  i/o  = server:char:inp: server name                                      */
/*         msg   :char:out: result message                                   */
/*                                                                           */
/*  return =  0: login successfully ended                                    */
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
int dlogin( char *server, char *msg )
{
    NWCONN_HANDLE   connx;
    DWORD           NCPExtensionID;
    int             rc;
    char            client[48];
    NWCONN_NUM      connnumber;
    NWCCODE         rcx;
    WORD            r_len;
    struct  LGINSendFrame   s_data;
    struct  LGINRecvFrame   r_data;

    rc = dchk( server, client,&connx,
               &NCPExtensionID, &connnumber );
    switch( rc ) {
        case DCHK_NON_LOGIN:
            rc = 0;
            break;
        case DCHK_BEFORE_DCE_LOGIN:
            dmsg( MSG_BEFORE_DCE_LOGIN, msg );
            return( -3 );
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
    r_len = sizeof( struct LGINRecvFrame );
    memcpy( s_data.command, LOGIN_K, COMMAND_LEN );
    rcx = NWNCPExtensionRequest( connx,
                                NCPExtensionID,
                                (void *)&(s_data),
                                (WORD)sizeof(struct LGINSendFrame),
                                (void *)&(r_data),
                                &r_len );
    if ( rcx != 0 ) {
        dmsg( MSG_SEND_RECV_ERR, msg );
        return( -4 );
    }
    if ( memcmp( r_data.command,
                 LOGIN_K, COMMAND_LEN ) ) {
        dmsg( MSG_SEND_RECV_ERR, msg );
        return( -4 );
    }
    switch ( r_data.rc ) {
        case LGIN_LOGIN_OK:
            r_data.dce_uid[r_data.len] = 0x00;
            dmsg( MSG_LOGIN_OK, msg, client, r_data.dce_uid );
            break;
        case LGIN_NON_SET_DCE:
            r_data.dce_uid[r_data.len] = 0x00;
            dmsg( MSG_NON_SET_DCE, msg,
                     client, r_data.dce_uid );
            rc = -4;
            break;
        case LGIN_NOT_DCE_PASSWD:
            dmsg( MSG_NOT_DCE_PASSWD, msg );
            rc = -4;
            break;
        case LGIN_NON_SET_DFAM:
            dmsg( MSG_NON_SET_DFAM, msg, client );
            rc = -4;
            break;
        case LGIN_BEFORE_DCE_LOGIN:
            dmsg( MSG_BEFORE_DCE_LOGIN, msg );
            rc = -3;
            break;
        case LGIN_GW_MEMORY_ERROR:
            dmsg( MSG_GW_MEMORY_ERROR, msg );
            rc = -4;
            break;
        case LGIN_GW_BINDRY_ERROR:
            dmsg( MSG_GW_BINDRY_ERROR, msg );
            rc = -4;
            break;
        case LGIN_GW_EGT_CONN:
            dmsg( MSG_GW_EGT_CONN, msg );
            rc = -4;
            break;
        case LGIN_GW_SYSTEM_ERR:
            dmsg( MSG_GW_SYSTEM_ERROR, msg );
            rc = -4;
            break;
        case LGIN_EGT_SYSTEM_ERROR:
            dmsg( MSG_EGT_SYSTEM_ERROR, msg );
            rc = -4;
            break;
        case LGIN_DCE_SYSTEM_ERROR:
            dmsg( MSG_DCE_SYSTEM_ERROR, msg );
            rc = -4;
            break;
        case LGIN_NON_DFAM_EGT:
            dmsg( MSG_NON_DFAM_EGT, msg );
            rc = -4;
            break;
        case LGIN_GW_ERROR_NH:
            dmsg( MSG_GW_ERROR_NH, msg );
            rc = -4;
            break;
        case LGIN_NOT_LOGIN:
            dmsg( MSG_NOT_LOGIN, msg );
            rc = -4;
            break;
        case LGIN_DLOGIN_NUM_OVER:
            dmsg( MSG_DLOGIN_NUM_OVER, msg );
            rc = -4;
            break;
        default:
            dmsg( MSG_NON_LOGIN, msg );
            rc = -4;
            break;
    }
    return( rc );
}
