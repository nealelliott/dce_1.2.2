/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dua_ids.h,v $
 * Revision 1.1.10.2  1996/02/18  23:34:48  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:14  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:22:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:12  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:04:33  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:44:34  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  10:07:00  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  12:29:35  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  18:11:40  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:08:35  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:03:08  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:49:22  melman]
 * 
 * $EndLog$
 */
#ifndef _DUA_IDS_H
#define _DUA_IDS_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char dua_ids_rcsid[] = "@(#)$RCSfile: dua_ids.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:34:48 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/**********************************************************************/
/*                                                                    */
/*       D E K L A R A T I O N E N     Daten                          */
/*                                                                    */
/**********************************************************************/

/*****  Local  *****/

#define dua_psap_ids(tdua, tids) copy_pre_address(tdua, tids)
#define dua_octet_string_ids(tdua, tids) copy_octet_string((octet_string *) tdua, (octet_string *) tids)
#define dua_att_type_ids(tdua, tids)	att_name_to_att_oid (tdua, tids)
#define dua_name_ids(tdua, tids) 	dua_dist_name_ids (tdua, tids)
#define dua_bind_res_ids(tdua, tids)	dua_bind_arg_ids ((dua_bind_arg *) tdua, (ids_bind_arg *) tids)
#define dua_referral_ids(tdua, tids)	dua_cont_reference_ids(&(tdua->candidate), &(tids->candidate))
#define dua_sec_error_ids(tdua, tids)	(tids->pb = tdua->pb)
#define dua_serv_error_ids(tdua, tids)	(tids->pb = tdua->pb)
#define dua_upd_error_ids(tdua, tids)	(tids->pb = tdua->pb)

#endif /* _DUA_IDS_H */
