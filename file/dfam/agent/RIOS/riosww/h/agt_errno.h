/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Trade Secret of Hitachi, Ltd.
 * Do not disclose without written concession from Hitachi, Ltd.
 * Copyright 1996 Hitachi, Ltd. All Rights Reserved.  Derechos Reservados.
 */
#ifdef HITACHI_REV_CHECK
# ifndef RCS_agt_errno_h_Included
# define RCS_agt_errno_h_Included
 static char *RCSID_agt_errno_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/agt_errno.h,v 1.1.2.2 1996/03/11 14:06:04 marty Exp $";
# endif /* Not RCS_agt_errno_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: agt_errno.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:04  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:55:50  marty]
 *
 * Revision 1.1.2.1  1996/02/19  09:49:25  dce-sdc
 * 	First Submit
 * 	[1996/02/19  09:48:57  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  05:30:55  05:30:55  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  05:30:46  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  01:58:01  kishi
 * 	Change Copyright.
 * 	[1995/10/26  01:57:51  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:41:55  kishi
 * 	Initial revision
 * 	[1995/09/09  07:17:06  kishi]
 * 
 * $EndLog$
 */

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

#define ECOM            1000
#define DENOTDEL        1001

#define DEUNAVAILSRV    1002
#define DEUNNOWNPRI     1003
#define DENOMASK	1004
#define DENOACLPRI	1005

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
#define DENOAGTKF   	1112
#define DEAGTKSIZE   	1113
#define DENOACNT	1114
#define DENOGWENT	1115
