/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: omInst.c,v $
 * Revision 1.1.10.2  1996/02/18  18:25:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:57  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:40:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:57:06  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:50:19  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:38:16  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:11:55  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:08:11  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:23:56  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:27:01  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:18:27  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:07:20  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:41:59  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:48:27  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:22:19  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:51:06  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: omInst.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:25:35 $";
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
/*	@(#)omInst.c	1.12 (Bull S.A)	6/12/92	*/

/*****************************************************************
 **	xom_instance.c -- OM function to test an object's class.	**
 **								**
 **	Author : J.Zrihen, DASD/SSD.				**
 *****************************************************************/

#ifdef THREADSAFE
#	include <pthread.h>
#endif

#include	<xom.h>
#include	<xomip.h>
#include <omInst.h>

/************************************************************************
 *									*
 * FUNCTION	: omP_instance()						*
 *									*
 *									*
 * DESCRIPTION :							*
 * This	function determines	whether	a service-generated	public or	*
 * private object,the subject,is an	instance of	a particular class	*
 * or any of its subclasses.						*
 *									*
 * ARGUMENTS :								*
 *									*
 * 1.- Subject (Object).						*
 *	   The subject,	which remains accessible.				*
 *									*
 * 2.- Class (Object Identifier).					*
 *	   Identifies the class	in question.				*
 *									*
 * RESULTS :								*
 *									*
 * 1.- Return Code (Return Code).					*
 *	   Whether the function	succeeded and, if not, why.	It is success	*
 *	   or one of the values	listed under Errors	below.			*
 *									*
 * 2.- Instance	(Boolean).						*
 *	   Whether the subject is an instance of the specified class or	*
 *	   any of its subclasses. This result is present if	and	only if	*
 *	   the Return Code result is success.				*
 *									*
 * ERRORS :									*
 *									*
 * function-interrupted, memory-insufficient, network-error,		*
 * no-such-class, no-such-object, no-such-representation,		*
 * permanent-error,	pointer-invalid, system-error, or temporary-error.	*
 *									*
 ************************************************************************/

OM_return_code
omP_xinstance(
	OM_object			subject,
	OM_object_identifier	class,
	OM_boolean			*instance
)
{
	OMP_private_object		 * xobject;
	OMP_object_type		subject_type;
	OMP_object_type		class_type;
	OM_boolean			flag = OM_FALSE;
	OM_return_code		ret;

	/*
	 * Set instance	argument to	false.
	 */
	*instance =		OM_FALSE;

	/*
	 * Check the subject argument.
	 */
	if ( subject ==		NULL )
		return(OM_POINTER_INVALID);

	if (OMP_SPUB(subject))
		if ( OMP_TYPE(subject) != OM_CLASS )
			return(OM_NO_SUCH_CLASS);

	/*
	 * Check the class argument, and get the class type.
	 */
	if ((ret = omP_get_object_type(OMP_WORKSPACE(subject), class, &class_type))	!= OM_SUCCESS)
		return(ret);

	/*
	 * Determine the subject type.
	 */
	/*
	 * Private object.
	 */
	if (OMP_PRIVATE(subject)) {
		/*
		 * Get descriptor -1st of private object.
		 * Convert pointer to the internal representation.
		 */
		xobject	 = (OMP_private_object *	)OMP_INTERNAL(subject);
		subject_type = xobject->object_type;
	} /*
	 * Public Object.
						 */	else if	(OMP_SPUB(subject))	 {
		if ((ret = omP_get_object_type(OMP_WORKSPACE(subject), subject->value.string, &subject_type))	 !=	OM_SUCCESS)
			return(ret);
	} else
		return(OM_NO_SUCH_OBJECT);

	/*
	 * Check class hierarchy (same class or	subclass)
	 * between subject_type	and	class_type.
	 */
	if ( OMP_PACKAGE(subject_type) == OMP_PACKAGE(class_type) &&  OMP_CLASS(subject_type)	  ==
		OMP_CLASS(class_type) )
		flag = OM_TRUE;
	else if	((ret =		omP_instance_switch(OMP_WORKSPACE(subject),	subject,
		 class,	class_type,	subject_type, &flag)) !=
		OM_SUCCESS)
		return(ret);

	*instance =		flag ? OM_TRUE : OM_FALSE;

	return(OM_SUCCESS);
} /* end omP_xinstance */


