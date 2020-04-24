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
 * $Log: dlist.c,v $
 * Revision 1.1.2.2  1996/03/11  14:06:51  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:25  marty]
 *
 * Revision 1.1.2.1  1996/02/21  05:42:00  takano
 * 	First Submit
 * 	[1996/02/21  05:41:52  takano]
 * 
 * $EndLog$
 */
#include "dfam_cli.h"

struct      SERVER_LIST     {
        char    server[48];
        char    add_node[10];
        struct  SERVER_LIST     *back;
        struct  SERVER_LIST     *next;
};

// EnglishComment
/*****************************************************************************/
/*  ID   = dlist()                                                           */
/*  name = Getting a list of servers that DFAM Gateway started               */
/*  func = Gets a list of servers that were started by DFAM Gateway in       */
/*         the internetwork.                                                 */
/*          - Gets a default handle.                                         */
/*          - Uses dchk() to check the DFAM login.                           */
/*          - Searches the bindery.                                          */
/*          - Assigns area for a "server list."                              */
/*          - Issues dchk() to each server for DFAM login checking.          */
/*          - Retuns the number of servers if successfully ended.            */
/*  i/o  = buf:struct DLIST **:in : list data buffer                         */
/*         msg:char *         :out: result message                           */
/*                                                                           */
/*  return =  0 : no target servers                                          */
/*            >0: number of the target servers                               */
/*            -1:                                                            */
/*            -2:                                                            */
/*            -3:                                                            */
/*            -4: other errors                                               */
/*              :                                                            */
/*  common = none                                                            */
/*  note = The format of "list data buffer" is as follows (stored in         */
/*         dfam_cli.h):                                                      */
/*           struct DLIST {                                                  */
/*               char server[48];   // server name                           */
/*               char network[8];   // network address                       */
/*               char node[12];     // node address                          */
/*               char dfl_flg;      // default indicator                     */
/*                                       1: default server                   */
/*               char DCE_flg;      // DCE login indicator                   */
/*                                       1: being logged in to DCE           */
/*               char NW_flg;       // NetWare login indicator               */
/*                                       1: being logged in to NetWare       */
/*              };                                                           */
/*                                                                           */
/*  date = October 18, 1994 by Kentarou Chikura                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
// EnglishComment
int dlist( struct DLIST **buf, char *msg )
{
    int             rc;
    NWCONN_HANDLE   connx;
    CONNECT_INFO    conninfo;
    NWSTRUCT_SIZE   conni_size;
    DWORD           NCPExtensionID;
    int             i;
    char            client[48];
    char            cc_server[20];

    NWOBJ_TYPE  objtype;
    NWOBJ_ID    objid;
    NWCCODE     ccode;

    NWFLAGS     hasprops;
    NWFLAGS     objflag;
    NWFLAGS     objsec;
    char        objname[48];
    struct      SERVER_LIST     *s_l;
    struct      SERVER_LIST     *s_l_s;
    struct      SERVER_LIST     *s_l_b;


    int         c_server;
    char        def_server[48];
    NWCONN_NUM  connnumber;

    s_l = s_l_s = s_l_b = (struct SERVER_LIST *)NULL;
    objid = (NWOBJ_ID)-1;
    rc = 0;
    ccode = NWGetDefaultConnectionID( &connx );
    if ( ccode != (NWCCODE)0 ) {
        dmsg( MSG_SEND_RECV_ERR, msg, def_server );
        return( -4 );
    }
    conni_size = sizeof( CONNECT_INFO );
    ccode = NWGetConnectionStatus( connx, &conninfo,
                           conni_size );
    if ( ccode != (NWCCODE)0 ) {
        dmsg( MSG_SEND_RECV_ERR, msg, def_server );
        return( -4 );
    }
    strcpy( def_server, conninfo.serverName );
    rc = dchk( def_server, client, &connx,
               &NCPExtensionID, &connnumber );
    switch( rc ) {
        case DCHK_NON_LOGIN:
        case DCHK_BEFORE_DCE_LOGIN:
        case DCHK_NOT_NETWARE_LOGIN :
        case DCHK_NON_DF_GW :
        case DCHK_NON_SET_DFAM:
            break;
        case DCHK_NW_SERVER_ERR :
            dmsg( MSG_LCHK_ERROR, msg );
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
    for( ccode=0, i=0 ; !ccode ; ) {
        if ( !( ccode = NWScanObject( connx, "*", (NWOBJ_TYPE)SAP_TYPE,
                                      &objid, objname, &objtype, &hasprops, &objflag, &objsec ) ) ) {
            if ( (s_l = (struct SERVER_LIST *)malloc(
                  sizeof( struct SERVER_LIST ) ) ) == NULL ) {
                dmsg( MSG_MEMORY_OVER, msg );
                s_l = s_l_s;
                while( s_l != (struct SERVER_LIST *)NULL ) {
                    s_l_b = s_l;
                    s_l = s_l_b->next;
                    free ( s_l_b );
                }
                return( -4 );
            }
            memset( s_l, 0x00, sizeof( struct SERVER_LIST ) );
            if ( s_l_s == (struct SERVER_LIST *)NULL ) {
                s_l_s = s_l;
                s_l_b = s_l;
                strcpy( s_l->server, objname );
                s_l->next = s_l->back = (struct SERVER_LIST *)NULL;
            } else {
                s_l_b->next = s_l;
                strcpy( s_l->server, objname );
                s_l->back = s_l_b;
                s_l->next = (struct SERVER_LIST *)NULL;
                s_l_b = s_l;
            }
            i++;
        }
    }
    c_server = i;
    if ( c_server != 0 ) {
        if ( ( *buf = (struct DLIST *)malloc(
                 sizeof(struct DLIST)*(c_server) ) ) == NULL ) {
            dmsg( MSG_MEMORY_OVER, msg );
            s_l = s_l_s;
            while( s_l != (struct SERVER_LIST *)NULL ) {
                s_l_b = s_l;
                s_l = s_l_b->next;
                free ( s_l_b );
            }
            return( -4 );
        }
        s_l = s_l_s;
        for( i=0 ; i < c_server ; i++ ){
            strcpy( (*buf)[i].server, s_l->server );
            memset( (*buf)[i].network, 0x20, 8 );
            memset( (*buf)[i].node,    0x20, 12 );
            (*buf)[i].dfl_flg = F_OFF;
            (*buf)[i].NW_flg  = F_OFF;
            (*buf)[i].DCE_flg = F_OFF;
            (*buf)[i].Y_flg = F_OFF;
            s_l_b = s_l;
            s_l = s_l->next;
            free( s_l_b );
        }
        for( i=0 ; i < c_server ; i++ ){
            if ( !strcmp( def_server, (*buf)[i].server ) )
                (*buf)[i].dfl_flg = F_ON;
            else
                (*buf)[i].dfl_flg = F_OFF;
            rc = dchk( (*buf)[i].server, client,  &connx,
                        &NCPExtensionID, &connnumber );
            switch ( rc ) {
                case DCHK_NON_LOGIN:
                case DCHK_NON_SET_DFAM:
                    (*buf)[i].NW_flg    = F_ON;
                    (*buf)[i].DCE_flg   = F_OFF;
                    break;
                case DCHK_BEFORE_DCE_LOGIN:
                    (*buf)[i].NW_flg    = F_ON;
                    (*buf)[i].DCE_flg   = F_ON;
                    break;
                case DCHK_NOT_NETWARE_LOGIN :
                    (*buf)[i].NW_flg    = F_OFF;
                    (*buf)[i].DCE_flg   = F_OFF;
                    break;
                case DCHK_NON_DF_GW :
                    break;
                case DCHK_NW_SERVER_ERR :
                    dmsg( MSG_NW_SERVER_ERR, msg,
                          (*buf)[i].server );
                    free( *buf );
                    return( -4 );
                    break;
                case DCHK_SEND_RECV_ERR :
                    dmsg( MSG_SEND_RECV_ERR, msg );
                    free( *buf );
                    return( rc );
                    break;
                case DCHK_LCHK_ERROR:
                    dmsg( MSG_LCHK_ERROR, msg );
                    free( *buf );
                    return( -4 );
                    break;
                case DCHK_GW_BINDRY_ERROR:
                    dmsg( MSG_GW_BINDRY_ERROR, msg );
                    free( *buf );
                    rc = -4;
                    return( rc );
                    break;
                case DCHK_GW_MEMORY_ERROR:
                    dmsg( MSG_GW_MEMORY_ERROR, msg );
                    free( *buf );
                    rc = -4;
                    return( rc );
                    break;
                default:
                    dmsg( MSG_LCHK_ERROR, msg );
                    free( *buf );
                    return( -4 );
                    break;
            }
        }
        sprintf( cc_server, "%d", c_server );
        dmsg( MSG_SERVER_NUMBER, msg, cc_server );
    } else {
        dmsg( MSG_NON_SERVER, msg );
    }
    return(c_server);
}
