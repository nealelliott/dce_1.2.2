#include "dfam_cli.h"
#include "cli_msg.h"                                            

static  struct Msg_list {
    unsigned char    code;
    unsigned char    txt[150];
    char    flg;
} msg_list[] = {
    1,      MSG_TXT_01,     2,
    2,      MSG_TXT_02,     1,
    3,      MSG_TXT_03,     0,
    4,      MSG_TXT_04,     0,
    5,      MSG_TXT_05,     1,
    6,      MSG_TXT_06,     2,
    7,      MSG_TXT_07,     1,
    8,      MSG_TXT_08,     0,
    9,      MSG_TXT_09,     0,
    10,     MSG_TXT_10,     0,
    11,     MSG_TXT_11,     0,
    12,     MSG_TXT_12,     1,
    13,     MSG_TXT_13,     0,
    14,     MSG_TXT_14,     0,
    15,     MSG_TXT_15,     0,
    16,     MSG_TXT_16,     0,
    17,     MSG_TXT_17,     1,
    18,     MSG_TXT_18,     1,
    19,     MSG_TXT_19,     1,
    20,     MSG_TXT_20,     0,
    21,     MSG_TXT_21,     1,
    22,     MSG_TXT_22,     1,
    23,     MSG_TXT_23,     0,
    24,     MSG_TXT_24,     0,
    25,     MSG_TXT_25,     0,
    26,     MSG_TXT_26,     0,
    27,     MSG_TXT_27,     0,
    28,     MSG_TXT_28,     0,
    29,     MSG_TXT_29,     0,
    30,     MSG_TXT_30,     1,
    31,     MSG_TXT_31,     1,
    32,     MSG_TXT_32,     0,
    33,     MSG_TXT_33,     0,
    34,     MSG_TXT_34,     0,
    35,     MSG_TXT_35,     0,
    36,     MSG_TXT_36,     0,
    37,     MSG_TXT_37,     0,
    0xfd,   MSG_TXT_fd,     2,
    0xfe,   MSG_TXT_fe,     -1,
    0xff,   MSG_TXT_ff,     1 };

// EnglishComment
/*****************************************************************************/
/*  ID   = dmsg()                                                            */
/*  name = Generating a message for DFAM Client                              */
/*  func = Generates a DFAM Client message.                                  */
/*          - Searches message codes.                                        */
/*          - If there is a variable field in the message, puts the field    */
/*            into the message.                                              */
/*          - Returns the message.                                           */
/*  i/o  = code:char *:inp: message ID number                                */
/*         msg :char *:out: message                                          */
/*         pr1 :char *:inp: 1st variable field in the message                */
/*         pr2 :char *:inp: 2nd variable field in the message                */
/*                                                                           */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note = If the target message does not carry a variable field, the        */
/*         parameter for the variable data is not necessary to set.          */
/*  date = October 19, 1994 by Kentarou Chikura                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
// EnglishComment
void dmsg( BYTE code, char *msg , char *pr1 , char *pr2 )
{
    int i, j, l, k, m;
    unsigned char *pt1, *pt2;
    char    no[4];

    for ( i=0 ; msg_list[i].code != code &&
                msg_list[i].code != MSG_NO_ERROR
              ; i++ ) ;
    l = i;
    switch ( msg_list[l].flg ) {
        case 0 :
            strcpy( msg, msg_list[l].txt );
            break;
        case 1 :
            pt1 = strchr( msg_list[l].txt, 0x01 );              
            j = (int)(pt1 - msg_list[l].txt);
            memcpy( msg, msg_list[l].txt, j );
            strcpy( &(msg[j]), pr1 );                           
            m = strlen( msg );
            pt1++;
            strcpy( &(msg[m]), pt1 );
            break;
        case 2 :
            pt1 = strchr( msg_list[l].txt, 0x01 );              
            pt2 = strchr( pt1+1, 0x01 );                        
            j = (int)(pt1 - msg_list[l].txt);
            memcpy( msg, msg_list[l].txt, j );
            strcpy( &(msg[j]), pr1 );                           
            m = strlen(msg);
            pt1++;
            k = (int)(pt2 - pt1);
            memcpy( &(msg[m]), pt1, k );
            strcpy( &(msg[m+k]), pr2 );                         
            pt2++;
            m = strlen(msg);
            strcpy( &(msg[m]), pt2 );
            break;
        default:
            sprintf( no, "%d", code );
            pt1 = strchr( msg_list[l].txt, 0x01 );              
            j = (int)(pt1 - msg_list[l].txt);
            memcpy( msg, msg_list[l].txt, j );
            strcpy( &(msg[j]), no );                            
            m = strlen( msg );
            pt1++;
            strcpy( &(msg[m]), pt1 );
            break;
    }
    return ;
}
