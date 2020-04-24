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
 * $Log: dfamdef.h,v $
 * Revision 1.1.2.3  1996/03/22  07:34:48  takano
 * 	modified the table deletion method in the POST routine.
 * 	[1996/03/22  07:31:04  takano]
 *
 * Revision 1.1.2.2  1996/03/11  14:07:31  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:57  marty]
 * 
 * Revision 1.1.2.1  1996/02/21  07:13:09  takano
 * 	First Submit
 * 	[1996/02/21  07:13:02  takano]
 * 
 * $EndLog$
 */
/*EnglishComment**************************************************************
 * DFAMDEF.H                                                                 *
 *                                                                           *
 * Trade Secret of Hitachi, Ltd.                                             *
 * Do not disclose without written concession from Hitachi, Ltd.             *
 * Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  *
 *EnglishComment**************************************************************/
#ifndef DFAMDEF_H
#define DFAMDEF_H

#include <stdio.h>
#include <stdlib.h>
#include <stddef.h>
#include <time.h>
#include <nwtypes.h>
#include <nwalias.h>
#include <sys/types.h>
#include <fshooks.h>


typedef short BOOL;

#ifndef D_MODE_T
#define D_MODE_T
    typedef unsigned short dmode_t;
#endif

#ifndef D_SSIZE_T
#define D_SSIZE_T
    typedef int dssize_t;
#endif

#ifndef D_SIZE_T
#define D_SIZE_T
    typedef unsigned int dsize_t;
#endif

#ifndef D_OFF_T
#define D_OFF_T
    typedef long doff_t;
#endif

#ifndef D_TIME_T
#define D_TIME_T
    typedef long dtime_t;
#endif


#define DFAM_NLM_INTERFACE_VER  "01.01"
#define ALLOWED_OS_VER          "3.12"
#define FILEVERSION_DFAVOL     "01.00"
#define FILEVERSION_DFAAGT     "01.00"
#define FILEVERSION_DFAOPT     "01.01"
#define CFGVER_0100            "01.00"
#define ALLOWED_CLIB_MAJ_VER	3
#define ALLOWED_CLIB_MIN_VER	12
#define ALLOWED_CLIB_REV		'g'

#define NLM_DFALIB             "DFALIB.NLM"
#define NLM_DFADCE             "DFADCE.NLM"

#define OT_DFA                 0x0805

#define CIPHER_NONE             0
#define CIPHER_MULTI2           1
#define CIPHER_MULTI0           2
#define CIPHER_OSF              3

#define THREAD_ADMIN            fnAdminThread
#define THREAD_WATCHDOG         fnWatchDogThread

#define THREADNAME_ADMIN        "AdminThread"
#define THREADNAME_WATCHDOG     "WatchDogThread"

#define VOLFILENAME     "SYS:SYSTEM\\DFAVOL.CFG"
#define AGTFILENAME     "SYS:SYSTEM\\DFAAGT.CFG"
#define OPTFILENAME     "SYS:SYSTEM\\DFAOPT.CFG"

#define TRCFNAME_FUNC   "SYS:\\DFAFUNC.TRC"
#define TRCFNAME_MAIN   "SYS:\\MAIN.TRC"
#define TRCFNAME_ADMIN  "SYS:\\ADMIN.TRC"
#define TRCFNAME_WATCH  "SYS:\\WATCHDOG.TRC"

#define NCPEXTNAME              "DFA_Command_Listen"
#define NCP_MAJOR_VERSION   1
#define NCP_MINOR_VERSION   0
#define NCP_REVISION        0

#define     THREAD_ABENDED      -1
#define     THREAD_NACTIVE       0
#define     THREAD_NOW_INIT      1
#define     THREAD_WORKING       2
#define     THREAD_NOW_TERM      3
#define     THREAD_SLEEP         4
#define     THREAD_TIMEOUT       5

#define		ACCESSLIST_THREAD_IS_NOT_CREATED	0
#define		ACCESSLIST_THREAD_IS_CREATED		1
#define		ACCESSLIST_THREAD_TERMINATE_REQUEST	2
#define		ACCESSLIST_THREAD_IS_TERMINATED		3

#define		COPYBACK_THREAD_IS_NOT_CREATED		0
#define		COPYBACK_THREAD_IS_CREATED			1
#define		COPYBACK_THREAD_TERMINATE_REQUEST	2
#define		COPYBACK_THREAD_IS_TERMINATED		3

#define		RENAME_ACCESS		1
#define		ERASE_ACCESS		2
#define		CREATEDIR_ACCESS	3
#define		DELETEDIR_ACCESS	4

#define LOAD_TCPIP   0
#define UNLOAD_TCPIP 1

#define DFAM_CON        0x01
#define DFAM_NWC_ALL    0x02
#define DFAM_NWC        0x04
#define DFAM_NWCS       0x08


#define LOG_NO       0x00
#define LOG_YES      0x01


#define DFAM_GMEMT001 0x01
#define DFAM_GMEMT002 0x02

#define DFAMT001     0x01
#define DFAMT002     0x02

#define DFAMWALLWAIT    1000

#define NOT_FULL        0
#define FULL            1
#define CASE_1          1
#define CASE_2          2
#define CASE_3          3
#define CASE_4          4
#define YES             1
#define NO              0

#define OPENMODE        1
#define CHECKMODE       2

#define     O_mask          0x00000003
#define     S_mask          0x0000001c

#define     O_READ_MODE     0x0001
#define     O_WRITE_MODE    0x0002

#define     GW_FILEOPEN             1
#define     GW_FILECREATEOPEN       2
#define     GW_FILEERASE            3
#define     GW_FILERENAME           4
#define     GW_FILECLOSE            5
#define     GW_DIRCREATE            6
#define     GW_DIRDEL               7


#define  T_READ     0x00000001
#define  T_WRITE    0x00000002
#define  T_RESERV   0x00000004
#define  T_CREATE   0x00000008
#define  T_ERASE    0x00000010
#define  T_ACCESS   0x00000020
#define  T_FIND     0x00000040
#define  T_MODIFY   0x00000080
#define  T_SUPER    0x00000100
#define  T_Q        0x00000200

#define  A_READ     0X00000001
#define  A_WRITE    0X00000002
#define  A_EXECUTE  0X00000004
#define  A_CONTROL  0X00000008
#define  A_INSERT   0X00000010
#define  A_DELETE   0X00000020


#define AGENTNAME_MAX           64
#define BUFF_SIZE               4096
#define DFAMGROUPNAME_MAX       47
#define DFAMGRP_MAX             47
#define DFSENTRY_MAX            255
#define DFSGRP_MAX              1024
#define DFSPATH_MAX             1024
#define DGW_OWNER_MAX           1024
#define DGW_OWNER_MIN           0
#define DGW_PATH_MAX            1024
#define DGW_PATH_MIN            0
#define LINEBUFLEN              128
#define LINE_MAX                80
#define MAP_DFS_PATH_MAX        1022
#define MASTERKEYLEN            17
#define MAXARRNUM               203
#define MAXDFSIZE               1250000
#define MAXSIZE32               0xFFFFFFFF
#define MAX_DCEPASSWORD         1025
#define MAX_DCEUSERNAME         1025
#define MAXMSGSIZE              2048
#define MAXNWCLEN               60
#define MAXTRFLEN               262144
#define MAX_NWUSERNAME          48
#define NWVOLUME_MAX            16
#define PROP_MAX                15
#define SEGMENTNUM              128
#define SEG_SIZE                128
#define SERV_INFO_SIZE          256
#define TG_STACK_SIZE          16384
#define WATCHDOGMSGLEN_MAX      60
#define DFS_MAX_FILE_NAME       1024
#define ICATCH_LEN              8
#define TIMEOUT_WAIT_TIME       10000
#define SLEEP_WAIT_TIME         1000
#define SHORTWAITTIME           50
#define MIN_TO_MILISEC          60000
#define GWMSG_STR_MAXSIZE       1024
#define SEMAPHORE_INIVALUE      1

#define ERLOGMAX         65536
#define ERRECLEN         64
#define MSGLOGMAX        65536
#define MSGRECLEN        256
#define TRHEADEND        100
#define FTRHEADEND       100
#define OPEN_ERROR       0X01
#define WRITE_ERROR      0X02
#define CLOSE_ERROR      0X04
#define NO_TR_AREA       0X08
#define GW_FUNC_START    0X00
#define GW_FUNC_END      0X01
#define GW_FUNC_FLUSH    0X02
#define GW_MAIN_TRACE_FLUSH	401
#define GW_ADMIN_TRACE_FLUSH	402
#define GW_WATCHDOG_TRACE_FLUSH	403
#define FUNC_TR_SIZE     4000
#define COM_TR_SIZE      2660
#define ADM_TR_SIZE      8192
#define WDG_TR_SIZE      65536
#define NO_FUNCTR_AREA   0X01
#define LOG_TIME         16
#define LOG_WEEK         4
#define OBJNAMESIZE      18
#define GW_TR_ERR        5
#define FT_OPEN_ERROR    0X01
#define FT_WRITE_ERROR   0X02
#define FT_NAME_LEN      20
#define CONN_MAX         250


#define GWERR_UNLOAD            3
#define GWERR_RECYCLE           2
#define GWERR_NORMAL            0
#define GWERR_ABEND             -1
#define GWERR_EXCEED            -2
#define GWERR_LESS              -3
#define GWERR_NOMEM             -4
#define GWERR_ISDEVICE          -5
#define GWERR_ISSYS             -6
#define GWERR_EOF               -7
#define GWERR_UNKNOWN           -8
#define GWERR_DOUBLEDEF         -9
#define GWERR_FAILURE           -10
#define GWERR_INVAL             -11
#define GWERR_NOTNUM            -12
#define GWERR_KEY_IS_FULL       -13
#define GWERR_LONG              -14
#define GWERR_IS_DFAMVOL        -15
#define GWERR_NOT_DFAMVOL       -16
#define GWERR_NUSE              -17
#define GWERR_NW_NOENT          -18

#define GWERR_BUG               -100

#define LGIN_SUCCESS              0
#define LGIN_NOT_DCE_USER         1
#define LGIN_OVER_USERS_NUM       2
#define LGIN_INVALID_PASSWORD     4
#define LGIN_ALREADY_LOGIN       10

#define LGOT_SUCCESS             0

#define PWDO_SUCCESS             0
#define PWDO_NOT_OLDPASSWORD    10
#define PWDN_SUCCESS             0
#define PWDN_NOT_OLDPASSWORD    10

#define DCHK_NOT_LOGIN           0
#define DCHK_ALREADY_LOGIN      10

#define NCP_COM_ERROR              203
#define NCP_INVALID_GW_ENV         204
#define NCP_INVALID_AGT_ENV        205
#define NCP_INVALID_DCE_ENV        206

#define NCP_NOT_OT_USER            246
#define NCP_NOT_DFAM_USER          247
#define NCP_BINDERY_ACCESS_FAILURE 248
#define NCP_OUT_OF_MEMORY          249
#define NCP_NOT_DCE_LOGIN          250
#define NCP_AGT_DOWN               251
#define NCP_BUSY                   252
#define NCP_FAILURE                254

#define DRFAILURE      (-1)
#define DRNOERR          0
#define DRUNKNOWLERR     1
#define DRINTERNALERR    2
#define DRCLOSEERR       3
#define DRREMOVEERR      4
#define DROPENERR        5
#define DRWRITEERR       6
#define DRNOGETMODE      7
#define DRNORESOURCE     8
#define DRNODATA         9
#define DRNOMEM         10

#define DUMP_REGULAR	0
#define DUMP_DFARAS		1

#ifndef DFAM_CODE
#define DFAM_CODE

#define DENOERR         0
#define DEPERM          1
#define DENOENT         2
#define DESRCH          3
#define DEINTR          4
#define DEIO            5
#define DENXIO          6
#define DE2BIG          7
#define DENOEXEC        8
#define DEBADF          9
#define DECHILD         10
#define DEAGAIN         11
#define DENOMEM         12
#define DEACCES         13
#define DEFAULT         14
#define DENOTBLK        15
#define DEBUSY          16
#define DEEXIST         17
#define DEXDEV          18
#define DENODEV         19
#define DENOTDIR        20
#define DEISDIR         21
#define DEINVAL         22
#define DENFILE         23
#define DEMFILE         24
#define DENOTTY         25
#define DETXTBSY        26
#define DEFBIG          27
#define DENOSPC         28
#define DESPIPE         29
#define DEROFS          30
#define DEMLINK         31
#define DEPIPE          32
#define DEDOM           33
#define DERANGE         34
#define DENOMSG         35
#define DEIDRM          36
#define DECHRNG         37
#define DEL2NSYNC       38
#define DEL3HLT         39
#define DEL3RST         40
#define DELNRNG         41
#define DEUNATCH        42
#define DENOCSI         43
#define DEL2HLT         44
#define DEDEADLK        45
#define DENOLCK         46
#define DENOSTR         50
#define DENODATA        51
#define DETIME          52
#define DENOSR          53
#define DENONET         54
#define DENOPKG         55
#define DENOLINK        57
#define DEADV           58
#define DESRMNT         59
#define DECOMM          60
#define DEPROTO         61
#define DEMULTIHOP      64
#define DEDOTDOT        66
#define DEBADMSG        67
#define DEUSERS         68
#define DEDQUOT         69
#define DESTALE         70
#define DEREMOTE        71
#define DEMIA           77
#define DEMNPWD         78
#define DEMZOMB         79
#define DENOSYM         215
#define DENOTSOCK       216
#define DEDESTADDRREQ   217
#define DEMSGSIZE       218
#define DEPROTOTYPE     219
#define DENOPROTOOPT    220
#define DEPROTONOSUPPORT    221
#define DESOCKTNOSUPPORT    222
#define DEOPNOTSUPP     223
#define DEPFNOSUPPORT   224
#define DEAFNOSUPPORT   225
#define DEADDRINUSE     226
#define DEADDRNOTAVAIL  227
#define DENETDOWN       228
#define DENETUNREACH    229
#define DENETRESET      230
#define DECONNABORTED   231
#define DECONNRESET     232
#define DENOBUFS        233
#define DEISCONN        234
#define DENOTCONN       235
#define DESHUTDOWN      236
#define DETOOMANYREFS   237
#define DETIMEDOUT      238
#define DECONNREFUSED   239
#define DEREFUSED       DECONNREFUSED
#define DEREMOTERELEASE 240
#define DEHOSTDOWN      241
#define DEHOSTUNREACH   242
#define DEALREADY       244
#define DEINPROGRESS    245
#define DEWOULDBLOCK    246
#define DENOTEMPTY      247
#define DENAMETOOLONG   248
#define DELOOP          249
#define DENOSYS         251
#define DEFDNMT         319
#define DEFDIOE         320
#define DEFDWPT         321
#define DEFDDLS         332
#define DEPRNRDY        349
#define DEPRFLT         350
#define DEPRPE          351
#define DEPRPWOFF       352
#define DEPRFUSE        353
#define DEPRLD          354
#define DEPRTMOUT       355
#define DEPRPARITY      356
#define DEPRCANCEL      357
#define DEPRREADY       358
#define DEPRBMPERR      360
#define DEPRBUSERR      361
#define DEPREDATA       362
#define DEPRCWTMOUT     363

#define DECOM           1000
#define DENOTDEL        1001
#define DEUNAVAILSRV    1002
#define DEUNNOWNPRI     1003
#define DENOMASK        1004
#define DENOACLPRI      1005

#define DENOKFILE       1100
#define DENOKEY         1101
#define DEKEYSIZE       1102
#define DEINVALAGT      1103
#define DENOENTRY       1104
#define DEINVALPSWD     1105
#define DEINVALACNT     1106
#define DELOCAL         1107
#define DEINVALGTWY     1108
#define DEUNKNOWN       1109
#define DEUPDATEKEY     1110
#define DENULLPSWD      1111
#define DENOAGTKF       1112
#define DEAGTKSIZE      1113
#define DENOACNT        1114
#define DENOGWENT       1115

#define DEHOSTFILE      1200
#define DESERVICESFILE  1201
#define DEREFUSEAGT     1202
#define DENONBKIO       1203
#define DECOMNOMEM      1204
#define DETIMEOUTAGT    1205
#define DERESPONCEDATA  1206
#define DEUNLOADTCPIP   1207
#define DEDFAMDCESYSTEM 1208
#define DEDFAMTONOVELL  1209
#define DEINVALSOCKET   1210
#define DENETWORKENV    1211
#define DEDFAMTERM      1212
#define DECIPHER        1213
#define DENOSUPPORT     1214
#endif

#define DIRSYNCINTERVAL_MIN         1
#define DIRSYNCINTERVAL_MAX         1440
#define DIRSYNCINTERVAL_DFL         300000
#define COPYBACKDELAYTIME_MIN       0
#define COPYBACKDELAYTIME_MAX       10000
#define COPYBACKDELAYTIME_DFL       0
#define RESPONSEWAITTIME_MIN        1
#define RESPONSEWAITTIME_MAX        32767
#define RESPONSEWAITTIME_DFL        120
#define SENDBUFFERLENGTH_MIN        512
#define SENDBUFFERLENGTH_MAX        52000
#define SENDBUFFERLENGTH_DFL        4096
#define WATCHDOGWAITTIME_MIN        1
#define WATCHDOGWAITTIME_MAX        30
#define WATCHDOGWAITTIME_DFL        5
#define	CONNECTTIME_MIN				1
#define	CONNECTTIME_MAX				32767
#define	CONNECTTIME_DFL				45
#define	FILESPARSEDELAYTIME_MIN		0
#define	FILESPARSEDELAYTIME_MAX		10
#define	FILESPARSEDELAYTIME_DFL		0
#define MESSAGELOGFILE1_DFL         "SYS:\\DFAERM.001"
#define MESSAGELOGFILE2_DFL         "SYS:\\DFAERM.002"
#define ERRORLOGFILE1_DFL           "SYS:\\DFAERL.001"
#define ERRORLOGFILE2_DFL           "SYS:\\DFAERL.002"
#define COPYBACKDIRLEN_MAX          242
#define TRACEFLUSHCOUNT_DFL         20

#define INVALCHAR_NWENTRY           ".,+[]\*?:\\\/;=<>\| "
#define VOL_SYS                     "SYS"
#define CHAR_WILDCARD   "\*?"


#define FUNC_FNCOMMANDLISTEN        "fnCommandListen"
#define FUNC_FNCONNDOWN             "fnConnDown"

#define FUNC_NCP_DCELOGIN           "NCP_DCELogin"
#define FUNC_FNLOGINTHREAD          "fnLoginThread"
#define FUNC_NCP_DCELOGINCHK        "NCP_DCELoginchk"
#define FUNC_ADDGWVOLTRUSTEE        "AddGWVolTrustee"
#define FUNC_DELGWVOLTRUSTEE        "DelGWVolTrustee"

#define FUNC_NCP_DCELOGOUT          "NCP_DCELogout"
#define FUNC_NCP_DCELOGOUTDOWN      "NCP_DCELogoutDown"
#define FUNC_RESUMEALLUSERTHREAD    "ResumeAllUserThread"

#define FUNC_NCP_DCEOLDPASSWD       "NCP_DCEOldPasswd"
#define FUNC_NCP_DCENEWPASSWD       "NCP_DCENewPasswd"
#define FUNC_NCP_GETDCEPASSWORD     "NCP_GetDCEPassWord"

#define FUNC_ADDTGRPID              "AddTGrpID"
#define FUNC_DELTGRPID              "DelTGrpID"

#define FUNC_GETDFAMUSERNAME        "GetDFAMUserName"
#define FUNC_GETDCEUSERNAME         "GetDCEUserName"
#define FUNC_GETDCEPASSWORD         "GetDCEPassWord"

#define FUNC_WRITETOPROP            "WriteToProperty"
#define FUNC_REGDFAMGRP             "RegisterDFAMGroup"

#define GW_CONTROL_CON  0x0001
#define GW_WATCHDOG_CON 0x0002
#define GW_USER_CON     0x0003

#define DGW_CONSOLE     0x0001
#define DGW_STOP        0x0002
#define DGW_ABORT       0x0003

#define HEALTHCHK_REQ   0x0012
#define MESSAGE_REQ     0x0013

#define DO_RDONLY       0000000
#define DO_WRONLY       0000001
#define DO_RDWR         0000002

#define DS_IRUSR        00400
#define DS_IWUSR        00200
#define DS_IXUSR        00100
#define DS_IRGRP        00040
#define DS_IWGRP        00020
#define DS_IXGRP        00010
#define DS_IROTH        00004
#define DS_IWOTH        00002
#define DS_IXOTH        00001

#define DS_IFDIR        0x4000
#define DS_GETSUBDIR    0x0000
#define DS_GETFILENAME  0x0001
#define DS_GETTIME      0x0002
#define DS_GETOWN_SIZE  0x0004

#define dsec_acl_perm_read      0x0001
#define dsec_acl_perm_write     0x0002
#define dsec_acl_perm_excute    0x0004
#define dsec_acl_perm_control   0x0008
#define dsec_acl_perm_insert    0x0010
#define dsec_acl_perm_delete    0x0020

#define DS_USER         0x0001
#define DS_GROUP        0x0002

#define ON  1
#define OFF 0

#define ST_SIZE 16384
#define TRACEAREASIZE     1380

#define DCE_USER_PROPERTY "DFA001"
#define DCE_PWD_PROPERTY  "DFA002"
#define DCE_GROUP_PROPERTY "DFA004"

#define TSH_RECV    -1
#define TSH_SEND    -2

#define UNLOAD_DFA          0
#define BAD_ADMIN_CONN     -1


#define COMLEN 4
#define NCPLOGIN     "LGIN"
#define NCPLOGOUT    "LGOT"
#define NCPOLDPASSWD "PWDO"
#define NCPNEWPASSWD "PWDN"
#define NCPLOGINCHK  "LCHK"
#define NCPGRANT     "GRNT"
#define NCPREVOKE    "RVOK"
#define NCPREMOVE    "RMOV"
#define NCPTLIST     "TLST"
#define NCPRIGHT     "RGHT"
#define NCPLISTDIR   "LSTD"
#define NCPNDIR      "NDIR"
#define NCPNCOPY     "NCPY"
#define NCPCHGDATE   "CHGD"
#define NCPCHGOWN    "CHGO"
#define NCPCHKVOL    "CHKV"

#endif
