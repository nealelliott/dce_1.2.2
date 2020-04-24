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
# ifndef RCS_hi_err_h_Included
# define RCS_hi_err_h_Included
 static char *RCSID_hi_err_h="$Header: /u0/rcs_trees/dce/rcs/file/dfam/agent/RIOS/riosww/h/hi_err.h,v 1.1.2.2 1996/03/11 14:06:30 marty Exp $";
# endif /* Not RCS_hi_err_h_Included */
#endif /* HITACHI_REV_CHECK */
/*
 * HISTORY
 * $Log: hi_err.h,v $
 * Revision 1.1.2.2  1996/03/11  14:06:30  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:09  marty]
 *
 * Revision 1.1.2.1  1996/02/19  08:51:17  dce-sdc
 * 	First Submit
 * 	[1996/02/19  08:50:20  dce-sdc]
 * 
 * Revision 1.1.2.3  96/01/12  06:45:05  06:45:05  kishi (Y.Kishi)
 * 	... ODE test ... and Change Copyright
 * 	[1996/01/12  06:44:55  kishi]
 * 
 * Revision 1.1.2.2  1995/10/26  03:07:19  kishi
 * 	Change Copyright.
 * 	[1995/10/26  03:06:55  kishi]
 * 
 * Revision 1.1.2.1  1995/09/09  07:25:12  kishi
 * 	Initial revision
 * 	[1995/09/09  07:24:46  kishi]
 * 
 * $EndLog$
 */
#define NORMAL				0

#define HI_EIO				5
#define HI_ENXIO			6
#define HI_EAGAIN			11
#define HI_EACCES			13
#define HI_ENOLCK			46
#define HI_EILSEQ			47
#define HI_ENOSTR			50
#define HI_ENODATA			51
#define HI_ETIME			52
#define HI_ENOSR			53
#define HI_EPROTO			61
#define HI_EBADMSG			67
#define HI_EUSERS			68
#define HI_EDQUOT			69
#define HI_ESTALE			70
#define HI_EREMOTE			71
#define HI_ENOTSOCK			216
#define HI_EDESTADDRREQ		217
#define HI_EMSGSIZE			218
#define HI_EPROTOTYPE		219
#define HI_ENOPROTOOPT		220
#define HI_EPROTONOSUPPORT	221
#define HI_ESOCKTNOSUPPORT	222
#define HI_EOPNOTSUPP		223
#define HI_EPFNOSUPPORT		224
#define HI_EAFNOSUPPORT		225
#define HI_EADDRINUSE		226
#define HI_EADDRNOTAVAIL	227
#define HI_ENETDOWN			228
#define HI_ENETUNREACH		229
#define HI_ENETRESET		230
#define HI_ECONNABORTED		231
#define HI_ECONNRESET		232
#define HI_ENOBUFS			233
#define HI_EISCONN			234
#define HI_ENOTCONN			235
#define HI_ESHUTDOWN		236
#define HI_ETOOMANYREFS		237
#define HI_ETIMEDOUT		238
#define HI_ECONNREFUSED		239
#define HI_EHOSTDOWN		241
#define HI_EHOSTUNREACH		242
#define HI_EALREADY			244
#define HI_EINPROGRESS 		245
#define HI_EWOULDBLOCK		246
#define HI_ENOTEMPTY		247
#define HI_ENAMETOOLONG		248
#define HI_ELOOP			249
#define HI_ENOSYS		  	251
#define HI_EFDIOE			320
#define HI_EROFS			30
