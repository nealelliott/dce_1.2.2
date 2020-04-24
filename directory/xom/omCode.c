/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: omCode.c,v $
 * Revision 1.1.10.2  1996/02/18  18:25:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:49  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:40:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:57:00  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:11:46  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:07:01  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:21:51  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:25:42  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:18:08  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:06:41  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:40:20  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:46:48  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:21:21  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:47:55  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: omCode.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:25:23 $";
#endif

/***************************************************************
*															   *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG	1991   *
*				 ALL RIGHTS	RESERVED						   *
*															   *
***************************************************************/

/**********************************************************************/
/*				 COPYRIGHT (C) BULL	S.A. 1990 1991 1992				  */
/*						  All Rights Reserved						  */
/**********************************************************************/
/*																	  */
/*	 * This	software is	furnished under	licence	and	may	be used	only  */
/*	 * in accordance with the terms	of that	licence	and	with the	  */
/*	 * inclusion of	the	above copyright	notice.						  */
/*	 * This	software may not be	provided or	otherwise made available  */
/*	 * to, or used by, any other person. No	title to or	ownership of  */
/*	 * the software	is hereby transferred.							  */
/*																	  */
/**********************************************************************/
/*	@(#)omCode.c	1.7	(Bull S.A) 6/26/92	*/

#ifdef THREADSAFE
#	include <pthread.h>
#endif

#include <xom.h>
#include <xomip.h>
#include <omCode.h>

/************************************************************************
 *									*
 * FUNCTION	: omP_encode()						*
 *		  omP_decode()						*
 *									*
 *									*
 * DESCRIPTION :							*
 *	omP_encode encodes an existing private object,creates a	new	*
 *	private	object of class	Encoding				*
 *									*
 *	omP_decode decodes a private object	of class Encoding, creates	*
 *	a new private object						*
 *									*
 * ARGUMENTS :								*
 *									*
 *	omP_encode:							*
 *		original = private object to encode			*
 *		rules	 = set of rules	to produce the encoding		*
 *		encoding = instance	of class Encoding			*
 *									*
 *	omP_decode:							*
 *		encoding = instance	of class Encoding			*
 *		original = private object decoded			*
 *									*
 * ERRORS :									*
 *									*
 *	function-declined						*
 *									*
 ************************************************************************/

OM_return_code
omP_xencode	(
	OM_private_object	original,
	OM_object_identifier	rules,
	OM_private_object		*encoding
)
{
	OM_return_code		ret	;


	/* Check validity of the destination object	*/
	if ( original == NULL )	 {
		*encoding =		(OM_private_object)	NULL ;
		return(OM_POINTER_INVALID);
	}

	if (OMP_PUBLIC (original)) {
		*encoding =		(OM_private_object)	NULL ;
		return (OM_NOT_PRIVATE);
	}

	/* check the valiidy of	the	workspace */
	if (OMP_WORKSPACE(original)->status	 !=	WORKSPACE_INIT)	{
		*encoding =		(OM_private_object)	NULL ;
		return(OM_NO_SUCH_WORKSPACE);
	}

	/*
* Encode the object
*/

	ret	 = omP_encode_switch(OMP_WORKSPACE(original), original,	rules,
			encoding);
	if (ret	 !=	OM_SUCCESS)	{
		return(ret);
	}
	return(OM_SUCCESS);


} /* end omP_xencode  */


OM_return_code
omP_xdecode	(
	OM_private_object		encoding,
	OM_private_object		*original
)
{
	OM_return_code		ret;

	/*
* Check	validity of	the	source object
*/
	if ( encoding == NULL )	 {
		return(OM_POINTER_INVALID);
	}

	/*
* Is workspace initialised?
*/

	if ( OMP_WORKSPACE(encoding)->status !=		WORKSPACE_INIT)	 {
		return (OM_NO_SUCH_WORKSPACE);
	}

	ret	 = omP_decode_switch(OMP_WORKSPACE(encoding)	, encoding,	original )	;
	if (ret	 !=	OM_SUCCESS)	{
		*original =		(OM_private_object)	NULL ;
		return(ret);
	}

	return(OM_SUCCESS);
} /* end omP_xdecode  */


/***************/
/* END OF FILE */
/***************/
