/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: omCopy.c,v $
 * Revision 1.1.10.2  1996/02/18  18:25:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:50  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:40:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:57:00  root]
 * 
 * Revision 1.1.8.2  1994/09/19  08:50:17  marrek
 * 	Fixes for OT 12198.
 * 	[1994/09/19  08:49:47  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  18:11:47  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:07:09  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:22:00  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:25:50  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:18:10  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:06:46  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:40:30  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:46:59  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:21:29  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:48:00  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: omCopy.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:25:25 $";
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
/*	@(#)omCopy.c	1.8	(BULL S.A) 6/9/92	*/


#ifdef THREADSAFE
#	include <pthread.h>
#endif

#include <xom.h>
#include <xomip.h>
#include <omCopy.h>

/************************************************************************
 *									*
 * FUNCTION	: omP_copy()						*
 *									*
 *									*
 * DESCRIPTION :							*
 * This	function creates a new private object, the copy, that is	*
 * an exact	but	independent	copy of	an existing	private	object,		*
 * the original. The function is recursive in that copying the		*
 * original	also copies	its	subobjects.					*
 *									*
 * ARGUMENTS :								*
 *									*
 * 1.- Original	(Private Object).					*
 *	   The original, which remains accessible.				*
 *									*
 * 2.- Workspace (Workspace	).						*
 *	   The workspace in	which the copy is to be	created. the			*
 *	   original's class	shall be in	a package associated with			*
 *	   this	workspace.													*
 *									*
 * RESULTS :								*
 *									*
 * 1.- Return Code (Return Code).					*
 *	   Whether the function	succeeded and, if not, why.	It is success	*
 *	   or one of the values	listed under Errors	below.			*
 *									*
 * 2.- Copy	(Private Object).						*
 *	   The copy. This result is	present	if and only	if the Return		*
 *	   Code	result is success.						*
 *									*
 * ERRORS :									*
 *									*
 * function-interrupted, memory-insufficient, network-error,		*
 * no-such-class, no-such-object, no-such-workspace, not-private,	*
 * permanent-error,	pointer-invalid, system-error, or temporary-error,	*
 * or too-many-values.													*
 *									*
 ************************************************************************/

OM_return_code
omP_xcopy(
	OM_private_object		original,
	OM_workspace		workspace,
	OM_private_object		*copy
)
{
	OM_return_code		ret;
	OM_private_object		destination;
	OM_object			source;
	OM_public_object		pub	 = 0;
	OM_value_position		nb;

	/*
	 * Check the original argument.
	 */
	if ( original == NULL )
		return(OM_POINTER_INVALID);

	if (!OMP_PRIVATE(original))
		return(OM_NOT_PRIVATE);

	/*
	 * Check the workspace in which	the	copy is	to be created.
	 */
	if (((OMX_workspace) workspace)->status	 ==	OM_FALSE	)
		return(OM_NO_SUCH_WORKSPACE);

	/*
	 * Compare the original's and copy's workspace.
	 */
	if (OMP_WORKSPACE(original)	 !=	workspace )	{
		/* check valid handle */
		if (!OMP_VALID_OBJECT(original))
			return(OM_NO_SUCH_OBJECT);

		/* get the source object		*/
		if ((ret = om_get(original,		0, 0, 0, 0,		0, &pub, &nb)) !=
			OM_SUCCESS)
			return(ret);

		/* patch due to bug found by TEIS */
		if ((ret = omP_create(OMP_CLASS_ID_STR(pub), OM_FALSE,	workspace,
			&destination)) != OM_SUCCESS) {
			if(pub)
				om_delete(pub);
			return(ret);

		} /* endif */
	} else {
		/*
		 * Create the copy object (class = original	class)
		 */
		if ((ret = omP_create(OMP_CLASS_ID_STR(original), OM_FALSE,	workspace,
				&destination)) != OM_SUCCESS) {
			if (pub)
				om_delete(pub);
			return(ret);
		}
	}

	/*
	 * Update  the copy	object.
	 */
	source = (pub) ? pub : original;
	if ((ret = omP_put(destination,		OM_REPLACE_ALL,		source,	0,	0,
		 0)) !=	OM_SUCCESS)	{
		omP_delete(destination);
		if (pub)
			om_delete(pub);
		return(ret);
	}

	if (pub)
		om_delete(pub);
	*copy =		destination;
	return(OM_SUCCESS);
} /* end omP_xcopy */


