/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: omCopyValu.c,v $
 * Revision 1.1.10.2  1996/02/18  18:25:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:51  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:40:16  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:57:01  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:49:25  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:38:08  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:11:48  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:07:18  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:22:13  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:26:00  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:18:12  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:06:51  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:40:40  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:47:10  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:21:37  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:48:08  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: omCopyValu.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:25:26 $";
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
/*	@(#)omCopyValu.c	1.8	(BULL S.A) 6/9/92	*/


#ifdef THREADSAFE
#	include <pthread.h>
#endif

#include <xom.h>
#include <xomip.h>
#include <omCopyValu.h>

/************************************************************************
 *									*
 * FUNCTION	: omP_copy_value()						*
 *									*
 *									*
 * DESCRIPTION :							*
 * This	function places	or replaces	an attribute value in one		*
 * private object, the destination,with	a copy of an attribute		*
 * value in	another	private	object,	the	source.	The	source value	*
 * shall be	a string. The copy's syntax	is that	of the original.	*
 *									*
 * ARGUMENTS :								*
 *									*
 * 1.- Source (Private Object).						*
 *	   The source, which remains accessible.				*
 *									*
 * 2.- Source Type (Type).						*
 *	   Identifies the type of the attribute	one	of whose values			*
 *	   is to be	copied.							*
 *									*
 * 3.- Source Value	Position (Value	Position).				*
 *	   The position	within the above attribute of the value				*
 *	   to be copied.							*
 *									*
 * 4.- Destination (Private	Object).					*
 *	   The destination,	which remains accessible.			*
 *									*
 * 5.- Destination Type	(Type).						*
 *	   Identifies the type of the attribute	one	of whose values			*
 *	   is to be	placed or replaced.					*
 *									*
 * 6.- Destination Value Position (Value Position).			*
 *	   The position	within the above attribute of the value				*
 *	   to be placed	or replaced.					*
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
 * pointer-invalid,	system-error, temporary-error, wrong-value-length,	*
 * wrong-value-syntax, wrong-value-type.								*
 *									*
 ************************************************************************/

OM_return_code
omP_xcopy_value(
	OM_private_object		source,
	OM_type			source_type,
	OM_value_position		source_value_position,
	OM_private_object		destination,
	OM_type			destination_type,
	OM_value_position		destination_value_position
)
{
	OM_return_code		ret;
	OM_type				types[2];
	OM_modification			modification ;
	OM_public_object		pub	 = 0;
	OM_value_position		nb;
	OM_descriptor		descr[2];

	/*
	 * Check arguments.
	 */
	if ( source	 ==	NULL	 ||	destination	== NULL	)
		return(OM_POINTER_INVALID);

	if ( !OMP_PRIVATE(source) || !OMP_PRIVATE(destination) )
		return(OM_NOT_PRIVATE);

	/* check valid handle */
	if (!OMP_VALID_OBJECT(source))
		return(OM_NO_SUCH_OBJECT);

	if ( source_value_position < (OM_value_position) 0 ||  destination_value_position < (OM_value_position)	0 )
		return(OM_NOT_PRESENT);

	/*
	 * Read	the	string in the source private object.
	 */
	types[0] = source_type;
	types[1] = 0;
	if ((ret = om_get(
					source,
					OM_EXCLUDE_ALL_BUT_THESE_TYPES|OM_EXCLUDE_ALL_BUT_THESE_VALUES,
					&types[0],
					0,
					source_value_position,
					source_value_position +	1,
					&pub,
					&nb)) != OM_SUCCESS )
		return(ret);
	if (!nb) {
		om_delete(pub);
		return(OM_NOT_PRESENT);
	}

	/* check value syntax is a string	*/
	if ((ret = omP_check_value_syntax(OMP_WORKSPACE(destination),
	OMP_SYNTAX(pub)))	!= OM_SUCCESS)	{
		om_delete(pub);
		return(ret);
	}

	/* check if	the	destination	value is present or	not	*/
	types[0] = destination_type;
	types[1] = 0;
	if ((ret = omP_get(
						destination,
						OM_EXCLUDE_ALL_BUT_THESE_TYPES | OM_EXCLUDE_DESCRIPTORS,
						types,
						0,
						destination_value_position,
						destination_value_position +  1,
						0,
						&nb))	 !=		OM_SUCCESS ) {
		om_delete(pub);
		return(ret);
	}

	if (!nb)
		modification = OM_REPLACE_ALL;
	else if	(nb	 > destination_value_position)
		modification = OM_REPLACE_CERTAIN_VALUES;
	else
		modification = OM_INSERT_AT_END;

	descr[0].type =		destination_type;
	descr[0].syntax	 = OMP_SYNTAX(pub);
	descr[0].value.string.length = pub->value.string.length;
	descr[0].value.string.elements = pub->value.string.elements;
	descr[1].type =		OM_NO_MORE_TYPES;


	ret	 = omP_put(
					destination,
					modification,
					&descr[0],
					types,
					destination_value_position,
					destination_value_position + 1);
	om_delete(pub);
	return(ret);
} /* end omP_xcopy_value */


