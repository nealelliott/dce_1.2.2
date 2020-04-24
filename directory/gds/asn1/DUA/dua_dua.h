/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dua_dua.h,v $
 * Revision 1.1.10.2  1996/02/18  23:34:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:13  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:22:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:08  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:04:26  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:44:03  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:04:02  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:29:00  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:11:29  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:08:24  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  16:26:18  marrek
 * 	November 1992 code drop
 * 	[1992/11/17  13:15:03  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:02:51  melman
 * 	Added cast in the following #define:
 * 	#define dua_name_free(tdua) ids_dist_name_free((ids_dist_name *)tdua)
 * 	[1992/06/01  18:10:52  melman]
 * 
 * 	New GDS merged drop
 * 	[1992/05/28  20:48:57  melman]
 * 
 * $EndLog$
 */
#ifndef _DUA_DUA_H
#define _DUA_DUA_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char dua_dua_rcsid[] = "@(#)$RCSfile: dua_dua.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:34:47 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/


#define dua_psap_dua(tids, tdua) copy_pre_address(tids, tdua)
#define dua_octet_string_dua(tids, tdua) copy_octet_string((octet_string *) tids, (octet_string *) tdua)
#define dua_att_type_dua(tids, tdua)	att_oid_to_att_name(tids, tdua)
#define dua_bind_res_dua(tids, tdua)	dua_bind_arg_dua((ids_bind_arg *) tids, (dua_bind_arg *) tdua)
#define dua_bind_res_free(tdua)	dua_bind_arg_free((dua_bind_arg *) tdua)
#define dua_referral_dua(tids, tdua)	dua_cont_reference_dua(&(tids->candidate), &(tdua->candidate))
#define dua_sec_error_dua(tids, tdua)	(tdua->pb = tids->pb)
#define dua_serv_error_dua(tids, tdua)	(tdua->pb = tids->pb)
#define dua_upd_error_dua(tids, tdua)	(tdua->pb = tids->pb)

#endif /* _DUA_DUA_H */
