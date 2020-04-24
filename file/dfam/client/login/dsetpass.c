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
 * $Log: dsetpass.c,v $
 * Revision 1.1.2.2  1996/03/11  14:06:56  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:28  marty]
 *
 * Revision 1.1.2.1  1996/02/21  05:48:55  takano
 * 	First Submit
 * 	[1996/02/21  05:48:49  takano]
 * 
 * $EndLog$
 */
#include "dfam_cli.h"
// EnglishComment
/*****************************************************************************/
/*  ID   = dsetpass()                                                        */
/*  name = Setting password                                                  */
/*  func = Requests DFAM Gateway to check the current DCE password, and      */
/*         to change the DCE password.                                       */
/*          - Checks DFAM login.                                             */
/*          - Checks an old password.                                        */
/*          - Checks a new password.                                         */
/*  i/o  = server  :char  :inp: target server name                           */
/*         old_pass:char *:in : old password                                 */
/*         new_pass:char *:in : new password                                 */
/*         msg     :char  :out: result message                               */
/*         flg     :char  :inp: processing indicator                         */
/*                            0: check the old password                      */
/*                            1: set the new password                        */
/*                                                                           */
/*  return =  0: password successfully changed                               */
/*            1: old password invalid                                        */
/*            2: new password setting error                                  */
/*           -1: not logged in to NetWare                                    */
/*           -2: DFAM Gateway not started                                    */
/*           -4: other errors                                                */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = October 18, 1994 by Kentarou Chikura                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
// EnglishComment
int dsetpass(   char * server,
                char *old_pass,
                char *new_pass,
                char *msg,
                char flg )
{
    NWCONN_HANDLE   connx;
    DWORD           NCPExtensionID;
    int             rc;
    char            client[48];
    NWCONN_NUM      connnumber;
    int             len, len8;
    NWCCODE         rcx;
    WORD            r_len;
    struct  PWDOSendFrame   so_data;
    struct  PWDORecvFrame   ro_data;
    struct  PWDNSendFrame   sn_data;
    struct  PWDNRecvFrame   rn_data;
    union   Key_Format_Tbl  keyFormat;

    rc = dchk( server, client, &connx,
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
            return( rc );
            break;
        case DCHK_NON_DF_GW :
            dmsg( MSG_NON_DF_GW, msg, server );
            rc = -2;
            return( rc );
            break;
        case DCHK_NW_SERVER_ERR :
            dmsg( MSG_NW_SERVER_ERR, msg, server );
            rc = -4;
            return( rc );
            break;
        case DCHK_SEND_RECV_ERR :
            dmsg( MSG_SEND_RECV_ERR, msg );
            rc = -4;
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
    if ( flg == F_OFF ) {
        memcpy( keyFormat.oldPassword, 0x00, 8 );
        strncpy( keyFormat.oldPassword, old_pass, 8 );
        len = strlen( old_pass );
        if ( ( len8 = ( 8 - ( len % 8 ) ) ) != 8 ) {
            len += len8 ;
        }
        fnSetEncryptData( keyFormat.key, (BYTE *)old_pass,
                          len, so_data.oldpwd );
        r_len = sizeof( struct PWDORecvFrame );
        so_data.len = (unsigned short)len;
        memcpy( so_data.command, OLD_PASS_K, COMMAND_LEN );
        rcx = NWNCPExtensionRequest( connx,
                                    NCPExtensionID,
                                    (void *)&(so_data),
                                    (WORD)sizeof(struct  PWDOSendFrame),
                                    (void *)&(ro_data),
                                    &r_len );
        if ( rcx != 0 ) {
            dmsg( MSG_SEND_RECV_ERR, msg );
            return( -4 );
        }
        if ( memcmp( ro_data.command,
                     OLD_PASS_K, COMMAND_LEN ) ) {
            dmsg( MSG_SEND_RECV_ERR, msg );
            return( -4 );
        }
        switch( ro_data.rc ) {
            case PWDO_PASSWORD_OK:
                rc = 0;
                break;
            case PWDO_PASSWORD_ERR:
                dmsg( MSG_NOT_OLD_PASSWD, msg );
                rc = 1;
                break;
            case PWDO_GW_MEMORY_ERROR:
                dmsg( MSG_GW_MEMORY_ERROR, msg );
                rc = -4;
                break;
            case PWDO_GW_BINDRY_ERROR:
                dmsg( MSG_GW_BINDRY_ERROR, msg );
                rc = -4;
                break;
            case PWDO_NON_SET_DFAM:
                dmsg( MSG_NON_SET_DFAM, msg, client );
                rc = -4;
                break;
            case PWDO_ERROR:
                dmsg( MSG_PASS_SET_ERROR, msg );
                rc = -4;
                break;
            default:
                dmsg( MSG_PASS_SET_ERROR, msg );
                rc = -4;
                break;
        }
    } else {
        memcpy( keyFormat.oldPassword, 0x00, 8 );
        strncpy( keyFormat.oldPassword, old_pass, 8 );
        len = strlen( old_pass );
        if ( ( len8 = ( 8 - ( len % 8 ) ) ) != 8 ) {
            len += len8 ;
        }
        fnSetEncryptData( keyFormat.key, (BYTE *)old_pass,
                          len, sn_data.oldpwd );
        sn_data.olen = (unsigned short)len;
        len = strlen( new_pass );
        if ( ( len8 = ( 8 - ( len % 8 ) ) ) != 8 ) {
            len += len8 ;
        }
        fnSetEncryptData( keyFormat.key, (BYTE *)new_pass,
                          len, sn_data.newpwd );
        sn_data.nlen = (unsigned short)len;
        r_len = sizeof( struct PWDNRecvFrame );
        memcpy( sn_data.command, NEW_PASS_K, COMMAND_LEN );
        rcx = NWNCPExtensionRequest( connx,
                                    NCPExtensionID,
                                    (void *)&(sn_data),
                                    (WORD)sizeof(struct  PWDNSendFrame),
                                    (void *)&(rn_data),
                                    &r_len );
        if ( rcx != 0 ) {
            dmsg( MSG_SEND_RECV_ERR, msg );
            return( -4 );
        }
        if ( memcmp( rn_data.command,
                     NEW_PASS_K, COMMAND_LEN ) ) {
            dmsg( MSG_SEND_RECV_ERR, msg );
            return( -4 );
        }
        switch( rn_data.rc ) {
            case PWDN_PASSWORD_OK:
                dmsg( MSG_CHANGE_PASS_OK, msg, server );
                rc = 0;
                break;
            case PWDN_PASSWORD_ERR:
                dmsg( MSG_NOT_OLD_PASSWD, msg );
                rc = 1;
                break;
            case PWDN_GW_MEMORY_ERROR:
                dmsg( MSG_GW_MEMORY_ERROR, msg );
                rc = -4;
                break;
            case PWDN_GW_BINDRY_ERROR:
                dmsg( MSG_GW_BINDRY_ERROR, msg );
                rc = -4;
                break;
            case PWDN_NON_SET_DFAM:
                dmsg( MSG_NON_SET_DFAM, msg, client );
                rc = -4;
                break;
            case PWDN_ERROR:
                dmsg( MSG_PASS_SET_ERROR, msg );
                rc = -4;
                break;
            default:
                dmsg( MSG_PASS_SET_ERROR, msg );
                rc = -4;
                break;
        }
    }
    return( rc );
}
