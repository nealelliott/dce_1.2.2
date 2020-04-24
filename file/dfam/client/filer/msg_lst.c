#include "cli_filr.h"
unsigned    short   user_group;
unsigned    short   flg_progrm;

// EnglishComment
/*****************************************************************************/
/*  ID   = msg_lst()                                                         */
/*  name = Message display                                                   */
/*  func = Displays messages.                                                */
/*           Gets a return code and sets a message text.                     */
/*  i/o  = no   :int    :IN : return code                                    */
/*         m_txt:char * :OUT: message text                                   */
/*                                                                           */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = March 7, 1995 by Kousuke Takano and Kentarou Chikura              */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void    msg_lst( int no, char *m_txt )
{
    switch( no ) {
        case    ERROPT:
            strcpy( m_txt, MSG_PARM_ERR );
            break;
        case    MEMERR:
            strcpy( m_txt, MSG_MEMORY_OVER );
            break;
        case    LOCALDRIVE:
            strcpy( m_txt, MSG_LOCALE_DRV );
            break;
        case    HEADONLY:
            if( flg_progrm == FLG_DLISTDIR )
                strcpy( m_txt, MSG_NON_DIR ) ;
            else
                strcpy( m_txt, MSG_NON_PASSNAME ) ;
            break;
        case    PATHERR:
        case    NOENT:
        case    INVALENT:
        case    NOSERVER:
            if( flg_progrm == FLG_DLISTDIR )
                strcpy( m_txt, MSG_NON_PASS ) ;
            else
                strcpy( m_txt, MSG_NON_PASSNAME ) ;
            break;
        case    NOUSER:
            strcpy( m_txt, MSG_NON_USERNAME );
            break;
        case    NOGRP:
            strcpy( m_txt, MSG_NON_GROUPNAME );
            break;
        case    ERIGHT:
            strcpy( m_txt, MSG_ERR_RIGHT );
            break;
        case    NORIGHT:
            strcpy( m_txt, MSG_NON_RIGHT );
            break;
        case    TOOLONG:
            strcpy( m_txt, MSG_PASS_MAX );
            break;
        case    NODFAMUSER:
            if ( user_group == DEFAULT_USER )
                strcpy( m_txt, MSG_NON_DFAM_USER );
            else
                strcpy( m_txt, MSG_NON_DFAM_GROUP );
            break;
        case    UNLOGIN:
            strcpy( m_txt, MSG_NON_DLOGIN );
            break;
        case    CONERR:
            strcpy( m_txt, MSG_CONN_ERR );
            break;
        case    SYSTEM:
            strcpy( m_txt, MSG_SYSTEM_ERR );
            break;
        case    NOTDEL:
            strcpy( m_txt, MSG_NOT_DEL );
            break;
        case    ENOTLFS:
            strcpy( m_txt, MSG_ENOTLFS );
            break;
        case    NOVOL:
            strcpy( m_txt, MSG_NON_DFAM_VOL );
            break;
        case    NODCEENT:
            strcpy( m_txt, MSG_NODCEENT );
            break;
        case    NOTRLST:
            if( flg_progrm != FLG_DGRANT )
                strcpy( m_txt, MSG_NOTRLST );
            else
                sprintf( m_txt, MSG_PROGRM_ERR2, no );
            break;
        default:
            if ( no < GTHRESHLD ) {
                sprintf( m_txt, MSG_PROGRM_ERR1, no );
            } else {
                sprintf( m_txt, MSG_PROGRM_ERR2, no );
            }
    }
}
