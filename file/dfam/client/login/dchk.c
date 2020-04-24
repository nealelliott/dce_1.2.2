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
 * $Log: dchk.c,v $
 * Revision 1.1.2.3  1996/03/11  14:06:49  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:24  marty]
 *
 * Revision 1.1.2.2  1996/02/21  06:26:10  takano
 * 	SI Miss
 * 	[1996/02/21  06:25:46  takano]
 * 
 * Revision 1.1.2.1  1996/02/21  05:41:30  takano
 * 	First Submit
 * 	[1996/02/21  05:41:24  takano]
 * 
 * $EndLog$
 */
#include "dfam_cli.h"
// EnglishComment
/*****************************************************************************/
/*  ID   = dchk()                                                            */
/*  name = DFAM login checking function                                      */
/*  func = Checks if the target DFAM Gateway has logged in to DFS, and       */
/*         returns a server connection handle and NCPExtension handle.       */
/*          - Gets server information                                        */
/*          - Searches an NCPExtension name                                  */
/*          - Sends a check command.                                         */
/*  i/o  = server        :char *         :inp: server name                   */
/*         client        :char *         :out: client name (user ID)         */
/*         connx         :NWCONN_HANDLE *:out: server connection handle      */
/*         NCPExtensionID:DWORD *        :out: NCPExtension handle           */
/*         connnumber    :NWCONN_NUM *   :out: connection number             */
/*                                                                           */
/*  return =  0: not logged in                   (DCHK_NON_LOGIN)            */
/*            1: already logged in               (DCHK_BEFORE_DCE_LOGIN)     */
/*           -1: not logged in to NetWare        (DCHK_NON_NETWARE_LOGIN)    */
/*           -2: DFAM Gateway not started        (DCHK_NON_DF_GW)            */
/*           -3: invalid NetWare server name     (DCHK_NW_SERVER_ERR)        */
/*           -4: other errors                    (DCHK_SEND_RECV_ERR)        */
/*           -5: login check error               (DCHK_LCHK_ERROR)           */
/*           -6: not a DFAM user                 (DCHK_NON_SET_DFAM)         */
/*           -7: bindery access error            (DCHK_GW_BINDRY_ERROR)      */
/*           -8: DFAM Gateway runs out of memory (DCHK_GW_MOMORY_OVER)       */
/*                                                                           */
/*  common = none                                                            */
/*  note = This function is invoked by caller functions for initialization.  */
/*  date = October 17, 1994 by Kentarou Chikura                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
// EnglishComment
int dchk( char *server, char *client, NWCONN_HANDLE *connx,
          DWORD *NCPExtensionID, NWCONN_NUM *connnumber )
{
    CONNECT_INFO    conninfo;
    NWSTRUCT_SIZE   conni_size;
    WORD            r_len;
    struct  LCHKSendFrame   s_data;
    struct  LCHKRecvFrame   r_data;
    NWCCODE         rcx;
    int             rc;
    char            s_flg;
    NWCONN_HANDLE   dconn;
    char            o_name[48];
    NWOBJ_ID        objid;
    char            objname[48];
    NWOBJ_TYPE      objtype;
    NWFLAGS         hasprops;
    NWFLAGS         objflag;
    NWFLAGS         objsec;
    char            *txt;

    strcpy( o_name, "*" );
    txt = _strupr( server );
    rcx = NWGetConnectionHandle( server,
                         (NWLOCAL_MODE)0,
                         connx,
                         (NWLOCAL_SCOPE NWFAR *)NULL );
    if ( rcx != 0 ) {
        rcx = NWGetDefaultConnectionID( &dconn );
        if ( rcx != (NWCCODE)0 ) {
            return( DCHK_SEND_RECV_ERR );
        }
        s_flg = F_OFF;
        rcx = (NWCCODE)0;
        objid = (NWOBJ_ID)-1;
        while( s_flg == F_OFF &&
               rcx == (NWCCODE)0 ) {
            rcx = NWScanObject( dconn, o_name,
                                  (NWOBJ_TYPE)OT_FILE_SERVER,
                                  &objid, objname,
                                  &objtype, &hasprops,
                                  &objflag, &objsec );
            if ( rcx == 0 ) {
                if ( !strcmp( server, objname ) )
                    s_flg = F_ON;
            }
        }
        if ( s_flg == F_ON )
            return( DCHK_NOT_NETWARE_LOGIN );
        else
            return( DCHK_NW_SERVER_ERR );
    }
    conni_size = sizeof( CONNECT_INFO );
    rcx = NWGetConnectionStatus( *connx, &conninfo,
                                 conni_size );
    if ( rcx != 0 )
        return( DCHK_SEND_RECV_ERR );
    if ( conninfo.clientName[0] == 0x00 ) {
        return( DCHK_NOT_NETWARE_LOGIN );
    }
    strcpy( client, conninfo.clientName );
    *connnumber = conninfo.connNumber;
    rcx = NWGetNCPExtensionInfoByName( *connx,
                               NCP_NAME,
                               NCPExtensionID,
                               0, 0, 0, 0 );
    if ( rcx == 0 ) {
        r_len = sizeof( struct LCHKRecvFrame );
        memcpy( s_data.command, DFAM_CHK_K, COMMAND_LEN );
        rcx = NWNCPExtensionRequest( *connx,
                                    *NCPExtensionID,
                                    (void *)&(s_data),
                                    (WORD)sizeof(struct  LCHKSendFrame),
                                    (void *)&(r_data),
                                    &r_len );
        if ( rcx != 0 ) {
            return( DCHK_SEND_RECV_ERR );
        }
        if ( memcmp( r_data.command,
                     DFAM_CHK_K, COMMAND_LEN ) ) {
            return( DCHK_SEND_RECV_ERR );
        }
        switch ( r_data.rc ) {
            case LCHK_NON_LOGIN:
                rc = DCHK_NON_LOGIN;
                break;
            case LCHK_LOGIN_ZUMI:
                rc = DCHK_BEFORE_DCE_LOGIN;
                break;
            case LCHK_ERROR:
                rc = DCHK_LCHK_ERROR;
                break;
            case LCHK_NON_SET_DFAM:
                rc = DCHK_NON_SET_DFAM;
                break;
            case LCHK_GW_BINDRY_ERROR:
                rc = DCHK_GW_BINDRY_ERROR;
                break;
            case LCHK_GW_MEMORY_ERROR:
                rc = DCHK_GW_MEMORY_ERROR;
                break;
            default:
                rc = DCHK_LCHK_ERROR;
                break;
        }
    } else {
        rc = DCHK_NON_DF_GW;
    }
    return( rc );
}
