/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: omRead.c,v $
 * Revision 1.1.10.2  1996/02/18  18:25:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:59  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:41:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:57:08  root]
 * 
 * Revision 1.1.8.3  1994/05/10  16:04:19  marrek
 * 	Bug fixes and I18N for April 1994 submission.
 * 	[1994/05/06  09:59:47  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:50:21  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:38:18  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  19:00:41  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:08:28  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:24:24  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:27:20  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:18:32  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:07:30  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:42:22  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:48:50  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  21:01:20  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:42:47  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  23:22:35  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:51:21  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: omRead.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:25:39 $";
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
/*	@(#)omRead.c	1.9	(BULL S.A) 1/28/92	*/

#ifdef THREADSAFE
#	include <pthread.h>
#endif

#include <xom.h>
#include <xomip.h>
#include <omRead.h>

/************************************************************************
 *									*
 * FUNCTION	: omP_read()						*
 *									*
 *									*
 * DESCRIPTION :							*
 * This	function reads a segment of	an attribute value in a	private	*
 * object, the subject.							*
 *									*
 * Note	: This function	enables	the	client to read an arbitrarily	*
 * long	value without requiring	that the service place a copy of	*
 * the entire value	in memory.						*
 *									*
 * ARGUMENTS :								*
 *									*
 * 1.- Subject (Private	Object).					*
 *	   The subject,	which remains accessible.				*
 *									*
 * 2.- Type	(Type).							*
 *	   Identifies the type of the attribute	one	of whose values			*
 *	   is to be	read.							*
 *									*
 * 3.- Value Position (Value Position).					*
 *	   The position	within the above attribute of the value				*
 *	   to be read.							*
 *									*
 * 4.- Local-String	(Boolean).						*
 *	   If true,	indicates that the value is	to be translated into	*
 *	   the implementation-defined local	character set representation	*
 *	   (which may entail the loss of some information).			*
 *									*
 * 5.- First Element Position (Element Position).			*
 *	   The position, p,	within the value, of the first element			*
 *	   to be read.							*
 *	   If it exceeds the number	of elements	present	in the value,	*
 *	   the argument	is taken to	be equal to	that number.		*
 *									*
 *	   In the C	interface, the First Element Position and the		*
 *	   Next	Element	Position result	of the generic interface are	*
 *	   realised	as the Element Position	argument.			*
 *									*
 * 6.- Elements	(String).						*
 *	   The space the client	provides for the elememts to be	read.	*
 *	   The string's	elements initially are unspecified.	The		*
 *	   string's	length initially is	the	maximum	number,	nm,		*
 *	   of elements the function	is to read.				*
 *									*
 *	   The service modifies	this argument. The string's	elements	*
 *	   are made	the	elements actually read,	those having positions,	*
 *	   within the value, in	the	interval [p, p + na]. The string's	*
 *	   length is made the number, na, of elements read,	i.e.,		*
 *	   minimum {nt - p,	nm}, nt	being the number of	elements		*
 *	   present.								*
 *									*
 *	   If Local-String is true,	the	number and positions of	the		*
 *	   elements	present	will be	determined by, but may not be the	*
 *	   same	as,	those requested, according to the characteristics	*
 *	   of the implementation-defined translation into the local		*
 *	   character set.							*
 *									*
 * RESULTS :								*
 *									*
 * 1.- Return Code (Return Code).					*
 *	   Whether the function	succeeded and, if not, why.	It is success	*
 *	   or one of the values	listed under Errors	below.			*
 *									*
 * 2.- Next	Element	Position (Element Position).			*
 *	   The position	within the value of	the	element	following the		*
 *	   last	of the elements	read, or zero if the value's final		*
 *	   element was read. This result is	present	if and only	if the	*
 *	   Return Code result is success.					*
 *									*
 *	   In the C	interface, the First Element Position argument and	*
 *	   the Next	Element	Position result	of the generic interface are	*
 *	   realised	as the Element Position	argument.			*
 *									*
 * ERRORS :									*
 *									*
 * function-interrupted, memory-insufficient, network-error,		*
 * no-such-object, no-such-syntax, not-present,	not-private,		*
 * permanent-error,	pointer-invalid, system-error, or temporary-error,	*
 * or wrong-value-syntax.												*
 *									*
 ************************************************************************/

/**
 **	omP_check_value_syntax -- Check	that syntax	of the value is	a string.
 **/
OM_return_code
omP_check_value_syntax(
	OMX_workspace	workspace,
	OM_syntax syntax)
{
	switch (syntax)	 {
	case OM_S_BIT_STRING:
	case OM_S_ENCODING_STRING:
	case OM_S_GENERAL_STRING:
	case OM_S_GENERALISED_TIME_STRING:
	case OM_S_GRAPHIC_STRING:
	case OM_S_IA5_STRING:
	case OM_S_NUMERIC_STRING:
	case OM_S_OBJECT_DESCRIPTOR_STRING:
	case OM_S_OBJECT_IDENTIFIER_STRING:
	case OM_S_OCTET_STRING:
	case OM_S_PRINTABLE_STRING:
	case OM_S_TELETEX_STRING:
	case OM_S_UTC_TIME_STRING:
	case OM_S_VIDEOTEX_STRING:
	case OM_S_VISIBLE_STRING:
		return(OM_SUCCESS);

	default:
		return(omP_check_specific_syntax(workspace,		syntax));
	} /* endswitch */
} /* end omP_check_value_syntax	*/


OM_return_code
omP_xread(
	OM_private_object		subject,
	OM_type					type,
	OM_value_position		value_position,
	OM_boolean				local_string,
	OM_string_length		*element_position,
	OM_string				*elements
)
{
	OM_return_code			ret;
	OM_string_length		len;
	OM_public_object		pub;
	OM_value_position		nb;
	OM_type					types[2];
	char					*str_ptr;

	/*
	 * Check the subject argument.
	 */

	if (!OMP_PRIVATE(subject)) {
		return(OM_NOT_PRIVATE);
	} /* endif */

	/*
	 * Check value position.
	 */
	if ( value_position	< (OM_value_position) 0) {
		return(OM_NOT_PRESENT);
	} /* endif */

	/*
	 * Check string's length of	elements the function is to	read.
	 */
	if ( elements->length == 0) {
		return(OM_SUCCESS);
	} /* endif */

	/*
	 * Read	a segment of a string in a private object.
	 */

	types[0] = type;
	types[1] = 0;
	if ((ret = omP_get(
						subject,
						OM_EXCLUDE_ALL_BUT_THESE_TYPES | OM_EXCLUDE_ALL_BUT_THESE_VALUES,
						types,
						local_string,
						value_position,
						value_position + 1,
						&pub, &nb))	 !=	OM_SUCCESS) {
		return(ret);
	} /* endif */

	if (!nb) {
		omP_delete(pub);
		return(OM_NOT_PRESENT);
	} /* endif */

	/* check value syntax is a string	*/
	if ((ret = omP_check_value_syntax(OMP_WORKSPACE(subject),
	OMP_SYNTAX(pub))) != OM_SUCCESS) {
		omP_delete(pub);
		return(ret);
	} /* endif */

	/*
	 * If the position of the first	element	to be read,
	 * exceeds the number of elements present in the value,
	 * the argument	is taken to	be equal to	that number.
	 */
	if (pub->value.string.length ==	OM_LENGTH_UNSPECIFIED) {
		len	 = strlen(pub->value.string.elements);
	} else {
		len	 = pub->value.string.length;
	} /* endif */
	if ( *element_position >= len )	 {
		*element_position =		0;			/* Next	Element	Position */
		elements->length = 0;			/* Nothing was read	 */
		omP_delete(pub);
		return(OM_SUCCESS);
	} /* endif */

	elements->length = (elements->length <= len - *element_position)
	? elements->length : len - *element_position;

	str_ptr	 = (char *)pub->value.string.elements +	*element_position ;

	memcpy(elements->elements, str_ptr, elements->length);


	/*
	 * Update Next Element Position
	 *
	 */
	*element_position += elements->length;
	/*
	 * Check if	the	value's	final element was read.
	 */
	if ( len ==	*element_position ) {
		*element_position =	0;
	} /* endif */

	omP_delete(pub);
	return(OM_SUCCESS);
} /* end omP_xread */
