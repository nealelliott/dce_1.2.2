/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPCode.c,v $
 * Revision 1.1.6.2  1996/02/18  18:24:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:15  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:31:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:27  root]
 * 
 * Revision 1.1.4.1  1994/02/22  18:09:57  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:19:51  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:58:17  marrek]
 * 
 * Revision 1.1.2.2  1993/08/10  07:01:15  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:13:01  marrek]
 * 
 * Revision 1.1.4.2  1992/12/30  21:16:20  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:04:31  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  19:46:20  zeliff
 * 	Commenting out non-ANSI #ident
 * 	[1992/05/28  15:54:24  zeliff]
 * 
 * 	New files from GDS drop
 * 	[1992/05/27  19:45:13  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsPCode.c,v $ $Revision: 1.1.6.2 $ $Date: 1996/02/18 18:24:32 $";
#endif

/***************************************************************
*															   *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG	1991   *
*				 ALL RIGHTS	RESERVED						   *
*															   *
***************************************************************/

/******************************************************************/
/*																  */
/*	COPYRIGHT	(C)	 1991 BY BULL S.A							  */
/*	All	rights reserved											  */
/*																  */
/*	This software is furnished under licence and may be	used only */
/*	in accordance with the terms of	that licence and with the	  */
/*	inclusion of the above copyright notice.					  */
/*	This software may not be provided or otherwise made	available */
/*	to,	or used	by,	any	other person. No title to or ownership of */
/*	the	software is	hereby transferred.							  */
/*																  */
/******************************************************************/
/*	@(#)dsPCode.c	1.3	(Bull S.A) 4/23/92	*/

/******************************************************************/
/*																  */
/*				dsP_decode_switch()	function						  */
/*																  */
/*				dsP_encode_switch()	function						  */
/*																  */
/******************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <xom.h>
#include <xomip.h>
#include <dsPCode.h>

/******************************************************************/
/*																  */
/*			  dsP_decode_switch()  function							  */
/*			  called by	omP_decode() function					  */
/*																  */
/******************************************************************/
OM_return_code dsP_decode_switch (
	const OM_private_object		encoding,
	OM_private_object	*original
)
{
	return(OM_FUNCTION_DECLINED);
} /* end dsP_decode_switch	*/


/******************************************************************/
/*																  */
/*			  dsP_encode_switch()  function							  */
/*			  called by	omP_encode() function					  */
/*																  */
/******************************************************************/
OM_return_code dsP_encode_switch (
	const OM_private_object		original,
	const OM_object_identifier	rules,
	OM_private_object			*encoding
)
{
	return(OM_FUNCTION_DECLINED);
} /* end dsP_encode_switch	*/

