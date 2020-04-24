/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: omRemove.c,v $
 * Revision 1.1.10.2  1996/02/18  18:25:41  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:48:00  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:41:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:57:09  root]
 * 
 * Revision 1.1.8.1  1994/02/22  19:00:43  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:08:37  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:24:37  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:27:30  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:18:35  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:07:34  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:42:32  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:49:02  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:22:44  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:51:32  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: omRemove.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:25:41 $";
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
/*	@(#)omRemove.c	1.7	(BULL S.A) 1/28/92	*/

#ifdef THREADSAFE
#	include <pthread.h>
#endif

#include <xom.h>
#include <xomip.h>
#include <omRemove.h>


/************************************************************************
 *									*
 * FUNCTION	: omP_remove()						*
 *									*
 *									*
 * DESCRIPTION :							*
 * This	function removes and discards particular values	of an		*
 * attribute of	a private object, the subject. If no values	remain,	*
 * the attribute itself	is removed also. If	the	value is a subobject,	*
 * the value is	first removed and then the Delete function is applied	*
 * to it, thus destroying the object.					*
 *									*
 * ARGUMENTS :								*
 *									*
 * 1.- Subject (Private	Object).					*
 *	   The subject,	which remains accessible. The subject's	class	*
 *	   is unaffected.							*
 *									*
 * 2.- Type	(Type).							*
 *	   Identifies the type of the attribute	some of	whose values are	*
 *	   to be removed. The type shall not be	Class.			*
 *									*
 * 3.- Initial Value (Value	Position).					*
 *	   The position	within the above attribute of the first	value		*
 *	   to be removed.							*
 *	   If it is	all-values or exceeds the number of	values present	*
 *	   in the attribute, the argument is taken to be equal to that	*
 *	   number.								*
 *									*
 * 4.- Limiting	Value (Value Position).					*
 *	   The position	within the above attribute one beyond that of	*
 *	   the last	value to be	removed. If	this argument is not		*
 *	   greater than	the	Initial	Value argument,	no values are		*
 *	   removed.								*
 *	   If it is	all-values or exceeds the number of	values present	*
 *	   in an attribute,	the	argument is	taken to be	equal to that	*
 *	   number.								*
 *									*
 * RESULTS :								*
 *									*
 * 1.- Return Code (Return Code).					*
 *	   Whether the function	succeeded and, if not, why.	It is success	*
 *	   or one of the values	listed under Errors	below.			*
 *									*
 * ERRORS :									*
 *									*
 * function-declined, function-interrupted,	memory-insufficient,	*
 * network-error, no-such-object, not-private, permanent-error,		*
 * pointer-invalid,	system-error or	 temporary-error			*
 *									*
 ************************************************************************/

OM_return_code
omP_xremove(
	OM_private_object		subject,
	OM_type			type,
	OM_value_position		initial_value,
	OM_value_position		limiting_value
)
{
	OM_return_code		ret;
	OM_type				types[2];
	OM_public_object		pub	 = 0;
	OM_value_position		nb;
	OM_descriptor		descr[2];

	/*
	 * Check arguments.
	 */
	if ( subject ==		NULL )
		return(OM_POINTER_INVALID);

	if ( !OMP_PRIVATE(subject) )
		return(OM_NOT_PRIVATE);

	if (type ==		OM_CLASS)
		return(OM_FUNCTION_DECLINED);

	/*
	 * If initial_value	is greater than	limiting_value
	 * no values are removed.
	 */
	if ( initial_value > limiting_value	)
		return(OM_WRONG_VALUE_POSITION);

	/*
	 * Remove and discard values of	an attribute of	a private object.
	 */

	types[0] = type;
	types[1] = 0;

	/* check if	attribute values are present	*/
	if ((ret = omP_get(subject,		OM_EXCLUDE_ALL_BUT_THESE_TYPES |  OM_EXCLUDE_VALUES	|
		OM_EXCLUDE_ALL_BUT_THESE_VALUES,
		types, 0, initial_value, limiting_value, &pub, &nb)) !=	 OM_SUCCESS)
		return(ret);

	if (!nb) {
		omP_delete(pub);
		return(OM_NOT_PRESENT);
	}

	descr[0].type =		type;
	descr[0].syntax	 = OMP_SYNTAX(pub);
	descr[0].value.string.length = 0;
	descr[0].value.string.elements = 0;
	descr[1].type =		OM_NO_MORE_TYPES;

	ret	 = omP_put_switch(OMP_WORKSPACE(subject), OMP_OBJ_TYPE(subject),
		 subject, OMP_REMOVE_VALUES, &descr[0],		types,
		initial_value, limiting_value);
	omP_delete(pub);
	return(ret);
} /* end omP_xremove */


