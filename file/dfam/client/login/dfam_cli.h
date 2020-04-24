// EnglishComment
/*****************************************************************************/
/* dfam_cli.h :                                                              */
/*                                                                           */
/*  date = October 18, 1994 by Kentarou Chikura                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
// EnglishComment
#include <stdio.h>
#include <stddef.h>
#include <stdlib.h>
#include <string.h>
#include <io.h>
#include <fcntl.h>
#include <conio.h>
#include <process.h>
#include <signal.h>
#include <errno.h>
#include <memory.h>
#include <malloc.h>
#include <ctype.h>

#include "nwcaldef.h"
#include "nwncpext.h"
#include "nwcalls.h"
#include "nwipxspx.h"

struct DLIST {
        char server[48];
        char network[8];
        char node[12];
        char dfl_flg;
        char DCE_flg;
        char NW_flg;
        char Y_flg;
};

struct  LGINSendFrame   {
    unsigned char   command[4];
};
struct  LGOTSendFrame   {
    unsigned char   command[4];
};
struct  PWDOSendFrame   {
    unsigned char   command[4];
    unsigned short  len;
    unsigned char   oldpwd[1024];
};
struct  PWDNSendFrame   {
    unsigned char   command[4];
    unsigned short  olen;
    unsigned char   oldpwd[1024];
    unsigned short  nlen;
    unsigned char   newpwd[1024];
};
struct  LCHKSendFrame   {
    unsigned char   command[4];
};
struct  LLSTSendFrame   {
    unsigned char   command[4];
    char            flg;
};

struct  LGINRecvFrame   {
    unsigned char   command[4];
    unsigned short  rc;
    unsigned short  len;
    unsigned char   dce_uid[1025];
};
struct  LGOTRecvFrame   {
    unsigned char   command[4];
    unsigned short  rc;
    unsigned short  len;
    unsigned char   dce_uid[1025];
};
struct  PWDORecvFrame   {
    unsigned char   command[4];
    unsigned short  rc;
};
struct  PWDNRecvFrame   {
    unsigned char   command[4];
    unsigned short  rc;
};
struct  LCHKRecvFrame   {
    unsigned char   command[4];
    unsigned short  rc;
};
struct  LLSTRecvFrame   {
    unsigned char   command[4];
    struct      {
        unsigned char   server[48];
        unsigned char   address[10];
    }   serverlist[8];
    unsigned short  cnt;
    char            flg;
};
union   Key_Format_Tbl  {
    LONG            key[2];
    char            oldPassword[8];
};
#define     NCP_NAME    "DFA_Command_Listen"

#define     F_ON        1
#define     F_OFF       0

#define     LOGIN_K     "LGIN"
#define     LOGOUT_K    "LGOT"
#define     OLD_PASS_K  "PWDO"
#define     NEW_PASS_K  "PWDN"
#define     DFAM_CHK_K  "LCHK"
#define     DFAM_LST_K  "LLST"
#define     COMMAND_LEN 4
#define     LLST_S_START  0
#define     LLST_S_ADD    1
#define     LLST_R_OFF    0
#define     LLST_R_ON     1

#define     MSG_LOGIN_OK            (unsigned char)1
#define     MSG_LOGOUT_OK           (unsigned char)2
#define     MSG_SEND_RECV_ERR       (unsigned char)3
#define     MSG_NOT_LOGIN           (unsigned char)4
#define     MSG_NON_SET_DFAM        (unsigned char)5
#define     MSG_NON_SET_DCE         (unsigned char)6
#define     MSG_NON_DF_GW           (unsigned char)7
#define     MSG_NOT_DCE_PASSWD      (unsigned char)8
#define     MSG_NOT_LOGOUT          (unsigned char)9
#define     MSG_NON_LOGIN           (unsigned char)10
#define     MSG_NOT_NETWARE_LOGIN   (unsigned char)11
#define     MSG_SERVER_NUMBER       (unsigned char)12
#define     MSG_NON_SERVER          (unsigned char)13
#define     MSG_NOT_OLD_PASSWD      (unsigned char)14
#define     MSG_NOT_NEW_PASSWD      (unsigned char)15
#define     MSG_PARM_ERR            (unsigned char)16
#define     MSG_NW_SERVER_ERR       (unsigned char)17
#define     MSG_OLD_PASS_SET        (unsigned char)18
#define     MSG_NEW_PASS_SET        (unsigned char)19
#define     MSG_CHANGE_PASS         (unsigned char)20
#define     MSG_RENEW_PASS_SET      (unsigned char)21
#define     MSG_CHANGE_PASS_OK      (unsigned char)22
#define     MSG_MEMORY_OVER         (unsigned char)23
#define     MSG_BEFORE_DCE_LOGIN    (unsigned char)24
#define     MSG_GW_MEMORY_ERROR     (unsigned char)25
#define     MSG_GW_BINDRY_ERROR     (unsigned char)26
#define     MSG_GW_SYSTEM_ERROR     (unsigned char)27
#define     MSG_GW_EGT_CONN         (unsigned char)28
#define     MSG_GW_ERROR_NH         (unsigned char)29
#define     MSG_PASSWORD_ALL        (unsigned char)30
#define     MSG_DLOGOUT_EXEC        (unsigned char)31
#define     MSG_PASS_SET_ERROR      (unsigned char)32
#define     MSG_LCHK_ERROR          (unsigned char)33
#define     MSG_NON_DFAM_EGT        (unsigned char)34
#define     MSG_EGT_SYSTEM_ERROR    (unsigned char)35
#define     MSG_DCE_SYSTEM_ERROR    (unsigned char)36
#define     MSG_DLOGIN_NUM_OVER     (unsigned char)37
#define     MSG_TEST_DATA           (unsigned char)0xfd
#define     MSG_NO_ERROR            (unsigned char)0xfe
#define     MSG_END                 (unsigned char)0xff

#define     DLIST_DEFAULT       "default"
#define     DLIST_DEFAULT_NON   "       "
#define     DLIST_LOGIN         "Login"
#define     DLIST_LOGIN_NON     "     "

#define     SAP_TYPE            0x0508
#define     QUERY_TYPE          (WORD)1

#define     BLKSIZE     8


#define     LCHK_NON_LOGIN          0
#define     LCHK_LOGIN_ZUMI         10
#define     LCHK_NON_SET_DFAM       247
#define     LCHK_GW_BINDRY_ERROR    248
#define     LCHK_GW_MEMORY_ERROR    249
#define     LCHK_ERROR              254

#define     LGIN_LOGIN_OK           0
#define     LGIN_NON_SET_DCE        1
#define     LGIN_DLOGIN_NUM_OVER    2
#define     LGIN_NOT_DCE_PASSWD     4
#define     LGIN_BEFORE_DCE_LOGIN   10
#define     LGIN_GW_EGT_CONN        203
#define     LGIN_GW_SYSTEM_ERR      204
#define     LGIN_EGT_SYSTEM_ERROR   205
#define     LGIN_DCE_SYSTEM_ERROR   206
#define     LGIN_NON_SET_DFAM       247
#define     LGIN_GW_BINDRY_ERROR    248
#define     LGIN_GW_MEMORY_ERROR    249
#define     LGIN_NON_DFAM_EGT       251
#define     LGIN_GW_ERROR_NH        252
#define     LGIN_NOT_LOGIN          254

#define     LGOT_LOGOUT_OK          0
#define     LGOT_GW_EGT_CONN        203
#define     LGOT_NON_SET_DFAM       247
#define     LGOT_GW_BINDRY_ERROR    248
#define     LGOT_GW_MEMORY_ERROR    249
#define     LGOT_NON_LOGIN          250
#define     LGOT_NON_DFAM_EGT       251
#define     LGOT_GW_ERROR_NH        252
#define     LGOT_NOT_LOGOUT         254

#define     PWDO_PASSWORD_OK        0
#define     PWDO_PASSWORD_ERR       10
#define     PWDO_NON_SET_DFAM       247
#define     PWDO_GW_BINDRY_ERROR    248
#define     PWDO_GW_MEMORY_ERROR    249
#define     PWDO_ERROR              254

#define     PWDN_PASSWORD_OK        0
#define     PWDN_PASSWORD_ERR       10
#define     PWDN_NON_SET_DFAM       247
#define     PWDN_GW_BINDRY_ERROR    248
#define     PWDN_GW_MEMORY_ERROR    249
#define     PWDN_ERROR              254


#define     DCHK_NON_LOGIN          0
#define     DCHK_BEFORE_DCE_LOGIN   1
#define     DCHK_NOT_NETWARE_LOGIN  -1
#define     DCHK_NON_DF_GW          -2
#define     DCHK_NW_SERVER_ERR      -3
#define     DCHK_SEND_RECV_ERR      -4
#define     DCHK_LCHK_ERROR         -5
#define     DCHK_NON_SET_DFAM       -6
#define     DCHK_GW_BINDRY_ERROR    -7
#define     DCHK_GW_MEMORY_ERROR    -8

int dlogin(
            char            *   ,
            char            *       );
int dchk(
            char            *   ,
            char            *   ,
            NWCONN_HANDLE   *   ,
            DWORD           *   ,
            NWCONN_NUM      *   );

int dlogout(
            char            *   ,
            char                ,
            char            *       );
int dlist(
             struct DLIST   **  ,
             char           *       );
int dsetpass(
            char            *   ,
            char            *   ,
            char            *   ,
            char            *   ,
            char                    );
void dmsg(
            BYTE                ,
            char            *   ,
            ... );
int ps_input(
            char            *   ,
            int                     );
int pass_sec(
            BYTE            *   ,
            BYTE            *   ,
            BYTE            *       );
void    fnSetEncryptData(
             LONG           *,
             BYTE           *,
             int             ,
             BYTE           *        );
int parm_pru( int , char **, char *** );

// EnglishComment
                     /* 0----+----1----+----2----+----3----+----4----+----5----+----6----+----7----+----8----+----9 */
#define     DLIST_T1    "Gateway-active NetWare Servers                  Status  NetWare DCE  \n"
#define     DLIST_T2    "----------------------------------------------- ------- -----   -----\n"
                     /*  xxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxxx default Login   Login     */
// EnglishComment
