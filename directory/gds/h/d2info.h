/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d2info.h,v $
 * Revision 1.1.10.2  1996/02/18  23:36:05  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:25  marty]
 *
 * Revision 1.1.10.1  1995/12/08  16:01:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:14:14  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:53:01  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:47:44  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  12:52:59  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  11:43:04  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  20:41:57  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:30:24  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/02  04:11:11  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:43:18  zeliff]
 * 
 * $EndLog$
 */
#ifndef _D2INFO_H
#define _D2INFO_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char d2info_rcsid[] = "@(#)$RCSfile: d2info.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:36:05 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*****************************************************************************/
/*               Administration <-> IAPL-interface header-file               */
/*****************************************************************************/

#define	D20_RESLNG	16000		/* max. size of the result area */
					/* used at the IAPL-interface */
#define D20_MATSIZE     40000           /* max. size of the attribute value*/
					/* area used at the IAPL-interface */
		    /* ATTENTION:   maximum size of attribute values  */
		    /*              depends on the length of one ADT  */
		    /*              entry (78 Bytes * D20_AT_MAX)     */


	/* common parameter block used at the IAPL-interface */

typedef struct
	{
	signed16		bind_id ;	/* bind Identifier */
	signed16		dir_id ;	/* directory Identifier */
	D2_c_arg	serv_cntrl ;	/* service controls */
	D2_credent	user_a_pwd ;	/* user name and password */
	D2_name_string	entry ;		/* dist. name of object */
	D2_name_string	alias ;		/* alias name of object */
	D2_name_string	rdn ;		/* new RDN of object */
	D2_subset	srchop ;	/* search operation mode */

	signed16           filter_present; /* indicates wether filter is present */
	D2_Fi_filter    filter ;	/* search filter information */

	D2_req_info	reqinfo ;	/* req. attribute types */
	D2_a_info	*attrinfo ;	/* attribute information queue */
	D2_a_value	*avalinfo ;	/* attribute informations */
	D2_a_modinf	*modinfo ;	/* modify attribute information */
	Octet_string    avalues ;       /* attribute value area */
	D2_error	*error;
	D2_ei_info	*resentry ;	/* result entry information queue */
	D2_ava		*resattr ;	/* result attribute information */
	String          filename ;      /* file for search result */
	} Ds_v2_info;

#endif /* _D2INFO_H */
