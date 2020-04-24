/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: omWrite.c,v $
 * Revision 1.1.10.2  1996/02/18  18:25:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:48:07  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:41:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:57:13  root]
 * 
 * Revision 1.1.8.3  1994/05/10  16:04:20  marrek
 * 	Bug fixes and I18N for April 1994 submission.
 * 	[1994/05/06  09:59:50  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:50:22  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:38:22  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  19:00:51  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:09:27  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:25:57  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:28:20  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:18:48  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:07:51  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:43:36  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:50:10  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:23:12  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:53:08  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: omWrite.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:25:50 $";
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
/*	@(#)omWrite.c	1.8	(BULL S.A) 2/11/92	*/

#ifdef THREADSAFE
#	include <pthread.h>
#endif

#include <xom.h>
#include <xomip.h>
#include <alloc.h>
#include <omWrite.h>



OM_return_code
append_bit_strings(
    char *result,
    OM_sint	*result_length,
    char *s1,
    OM_sint	l1,
    char *s2,
    OM_sint	l2
);

/************************************************************************
 *									*
 * FUNCTION	: append_bit_strings()					*
 *									*
 *									*
 * DESCRIPTION :							*
 * This	function builds	a bit string that is the concatenation of two	*
 * bit strings.	This function does not allocate	the	memory for the		*
 * and assumes that	it has already be done.				*
 *									*
 * ARGUMENTS :								*
 *									*
 * 3.- s1 (char	*) : a pointer to a	the	bit	string that	will constitute	*
 *			the	beginning of the new bit string			*
 * 4.- l1 (OM_sint)	   : the length	of s1					*
 * 5.- s2 (char	*) : a pointer to a	the	bit	string that	will constitute	*
 *			 the end of	the	new	bit	string			*
 * 6.- l2 (OM_sint)	   : the length	of s2					*
 *									*
 * RESULTS :								*
 *									*
 * 1.- result (char	*) : a pointer to the result ( the concatenation of	*
 *			 s1	and	s2)					*
 * 2.- result_length (OM_sint *) : the length of result			*
 *									*
 ************************************************************************/
OM_return_code
append_bit_strings(
	char *result,
	OM_sint	*result_length,
	char *s1,
	OM_sint	l1,
	char *s2,
	OM_sint	l2)
{
	OM_sint	i;
	OM_sint	offset;
	OM_sint	d;
	OM_sint	msk	 = 0x00ff;
	char	c1;
	char	c2;

	/* Copy	the	content	of s1 into result */
	memcpy(result, s1, l1);

	/* Initialise offset: the number of	unused bits	in the last	byte
	   of s1
	 */
	offset = (OM_sint) s1[0];
	if ((offset	 > 7) ||(offset	 < 0)) {
		return(	OM_WRONG_VALUE_SYNTAX);
	} /* endif */

	/* Initialise c1 with the used bits	of the last	byte of	s1 and set
	   the other bits with 0
	 */
	c1 = s1[l1 - 1]	 >>	offset;
	c1 = c1	 <<	offset;

	/* the loop	begins with	s2[1] to skip the byte containing the number
	   of unused bits of the last byte of s2
	 */
	for	(i = 1;	i <		l2;	i++)	 {
		d =		(((OM_sint)	s2[i]) & msk);
		d =		d >> (8	 - offset);
		c2 = (char)	d;
		c2 |= c1;
		result[i + l1 -	2] = c2;
		c1 = s2[i] << offset;
	} /* endfor */
	result[l1 +	l2 - 2]	 = c1;

	/* Here	result contains	the	concatenation of s1	and	s2
	   we still	have to	update the number of unused	bits in	the
	   last	byte of	result
	 */
	result[0] =	(char)(( s1[0] + s2[0])	 % 8);

	/* Update result_length	*/
	if ((OM_sint) ((s1[0] +	s2[0]) / 8)	 ==	0 ) {
		/* result_length is	equal to the length	of l1 plus the length
		   of l2 minus one ( only one byte to give the number of unused
		   bits	in the last	byte is	needed)	minus one */

		*result_length = l1	 + l2 -	1;
	} else {
		*result_length = l1	 + l2 ;
	} /* endif */

	return(OM_SUCCESS);
} /* end append_bit_strings	*/


/************************************************************************
 *									*
 * FUNCTION	: omP_write()						*
 *									*
 *									*
 * DESCRIPTION :							*
 * This	function writes	a segment of an	attribute value	in a private	*
 * object, the subject.							*
 *									*
 * The written segment is made the value's last; the function		*
 * discards	any	elements of	the	value whose	positions equal	or exceed	*
 * the Next	Element	Position result. If	the	value being	written	is in	*
 * the local representation, it	is converted to	the	non-local		*
 * representation (which may entail	the	loss of	information	and		*
 * which may yield a different number of elements than that	provided).	*
 *									*
 * Note	: This function	enables	the	client to write	an arbitrarily	*
 * long	value without having to	place a	copy of	the	entire value	*
 * in memory.								*
 *									*
 * ARGUMENTS :								*
 *									*
 * 1.- Subject (Private	Object).					*
 *	   The subject,	which remains accessible.				*
 *									*
 * 2.- Type	(Type).							*
 *	   Identifies the type of the attribute	one	of whose values			*
 *	   is to be	written.						*
 *									*
 * 3.- Value Position (Value Position).					*
 *	   The position	within the above attribute of the value				*
 *	   to be written. The value	position shall neither be negative	*
 *	   nor exceed the number of	values present.	If it equals the	*
 *	   number of values	present, the segment is	inserted into the	*
 *	   attribute as	a new value.					*
 *									*
 * 4.- Syntax (Syntax).							*
 *	   If the value	being written was not already present in the	*
 *	   subject,	this identifies	the	syntax that	the	value is to	have.	*
 *	   It must be permissible syntax for the attribute of which	this	*
 *	   is a	value. If the value	being written was already present in	*
 *	   the subject,	then that value's syntax is	preserved and this	*
 *	   argument	is ignored.						*
 *									*
 * 5.- First Element Position (Element Position).			*
 *	   The position, p,	within the value, of the first element			*
 *	   to be written.							*
 *	   If it exceeds the number	of elements	present	in the value,	*
 *	   the argument	is taken to	be equal to	that number.		*
 *									*
 *	   In the C	interface, the First Element Position and the		*
 *	   Next	Element	Position result	of the generic interface are	*
 *	   realised	as the Element Position	argument.			*
 *									*
 * 6.- Elements	(String).						*
 *	   The elememts	to be written, na in number. Copies	of these	*
 *	   elements	will occupy	the	positions, within the value,		*
 *	   in the interval [p, p + na].	Any	elements already at	any	of	*
 *	   those positions are overlaid.					*
 *									*
 * RESULTS :								*
 *									*
 * 1.- Return Code (Return Code).					*
 *	   Whether the function	succeeded and, if not, why.	It is success	*
 *	   or one of the values	listed under Errors	below.			*
 *									*
 * 2.- Next	Element	Position (Element Position).			*
 *	   The position	within the value of	the	element	following the		*
 *	   last	element	written. This result is	present	if and only	if the	*
 *	   Return Code result is success.					*
 *									*
 *	   In the C	interface, the First Element Position argument and	*
 *	   the Next	Element	Position result	of the generic interface are	*
 *	   realised	as the Element Position	argument.			*
 *									*
 * ERRORS :									*
 *									*
 * function-declined, function-interrupted,	memory-insufficient,	*
 * network-error, no-such-object, no-such-syntax, no-such-type,		*
 * not-present,	not-private, permanent-error, pointer-invalid,		*
 * system-error, or	temporary-error, wrong-value-length,		*
 * wrong-value-makeup, or wrong-value-syntax.				*
 *									*
 ************************************************************************/

OM_return_code
omP_xwrite(
	OM_private_object		subject,
	OM_type					type,
	OM_value_position		value_position,
	OM_syntax				syntax,
	OM_string_length		*element_position,
	OM_string				elements
)
{
	OM_return_code			ret;
	OM_string				elts;
	OM_syntax				val_syntax;
	OM_type					types[2];
	OM_boolean				local_string;
	OM_modification			modification ;
	OM_public_object		pub	 = 0;
	OM_value_position		nb;
	OM_descriptor			descr[2];
	OM_string_length		starting_position;

	/*
	 * Check the subject argument.
	 */

	if (!OMP_PRIVATE(subject)) {
		return(OM_NOT_PRIVATE);
	} /* endif */

	/*
	 * Check value position.
	 */
	if ( value_position	 < (OM_value_position) 0) {
		return(OM_NOT_PRESENT);
	} /* endif */

	/*
	 * Check string's length of	elements the function is to	write.
	 */
	if ( elements.length ==	0 ) {
		return(OM_SUCCESS);
	} /* endif */

	/*
	 * Write a segment of a	string into	a private object.
	 */

	types[0] = type;
	types[1] = 0;

	local_string = (syntax & OM_S_LOCAL_STRING)	 ? OM_TRUE :  OM_FALSE;
	/* read	the	value			*/
	if ((ret = omP_get(subject, OM_EXCLUDE_ALL_BUT_THESE_TYPES 
	|  OM_EXCLUDE_ALL_BUT_THESE_VALUES, types, local_string ,
	value_position,	value_position + 1,		&pub, &nb))	 !=	OM_SUCCESS)	{
		if (ret	 !=	OM_NOT_PRESENT) {
			return(ret);
		} /* endif */
		/* determine how many values has the attribute	*/
		if ((ret = omP_get(subject,	OM_EXCLUDE_ALL_BUT_THESE_TYPES 
		| OM_EXCLUDE_DESCRIPTORS, types, local_string, 0, 0, 0, &nb)) 
		!= OM_SUCCESS) {
			return(ret);
		} /* endif */
		if (value_position != nb) {
			return(OM_NOT_PRESENT);
		} /* endif */
		modification = OM_INSERT_AT_END;
		val_syntax = syntax	 & OM_S_SYNTAX;
	} else {
		if (!nb) {
			modification = OM_INSERT_AT_END;	/* TEMPORARY  */
			val_syntax = syntax	 & OM_S_SYNTAX;
		} else {
			modification = OM_REPLACE_CERTAIN_VALUES;
			val_syntax = OMP_SYNTAX(pub);
		} /* endif */
	} /* endif */

	/* check value syntax is a string	*/
	if ((ret = omP_check_value_syntax(OMP_WORKSPACE(subject), val_syntax)) 
	!= OM_SUCCESS)	 {
		if (pub)
			omP_delete(pub);
		return(ret);
	} /* endif */

	/* Specify the length of elements to avoid problems
	 */
	if (elements.length	 ==	OM_LENGTH_UNSPECIFIED) {
		elements.length	 = strlen(elements.elements);
	} /* endif */

	starting_position =	*element_position;

	/*
	 * If the position of the first	element	to be written,
	 * return exceeds the number of	elements present in	the	value,
	 * the argument	is taken to	be equal to	that number.
	 */
	if (modification ==	OM_REPLACE_CERTAIN_VALUES) {
		if ( pub->value.string.length == OM_LENGTH_UNSPECIFIED)	 {
			if ( starting_position > strlen(pub->value.string.elements)) {
				starting_position =		strlen(pub->value.string.elements);
			} /* endif */
		} else if ( starting_position > pub->value.string.length )	 {
				starting_position =		pub->value.string.length ;
		} /* endif */

		elts.length	 = elements.length +	starting_position;

		/* allocate	a new string		*/
		if ((elts.elements = (void *)OMP_XALLOC(OMP_WORKSPACE(subject),
		AL_STRING,	elts.length	 + 1)) == 0)	 {
			if (pub) {
				omP_delete(pub);
			} /* endif */
			return(OM_MEMORY_INSUFFICIENT);
		} /* endif */
		/* update new string	*/
		if (val_syntax != OM_S_BIT_STRING)	 {
			memcpy(elts.elements, pub->value.string.elements,
			starting_position);
			memcpy((char *)elts.elements + starting_position,
			elements.elements,	elements.length);
		} else {

			/* if the string is	a bit string, there	is a bit more work to do */
			/* if element_position < pub->value.string.length, set the
		   number of unused	bits in	the	last octet of
		   pub->value.string.elements to 0
		 */
			if (starting_position <	pub->value.string.length) {
				((char *)(pub->value.string.elements))[0] = (char) 0;
			} /* endif */

			ret	 = append_bit_strings( elts.elements, (OM_sint *)(&elts.length),
			pub->value.string.elements,	starting_position, elements.elements,
			elements.length);
			if (ret	 !=	OM_SUCCESS)	{
				if (pub) {
					omP_delete(pub);
				} /* endif */
				OMP_XFREE( OMP_WORKSPACE(subject), elts.elements, AL_STRING,
				ROUNDUP(OMP_WORKSPACE(subject), elts.length + 1));
				return(ret);
			} /* endif */
		} /* endif */
	} else {
		elts.length	 = elements.length;
		elts.elements =	elements.elements;
	} /* endif */

	descr[0].type =	type;
	descr[0].syntax	 = val_syntax;
	if (local_string) {
		descr[0].syntax	 |=	OM_S_LOCAL_STRING;
	} /* endif */
	descr[0].value.string.length = elts.length;
	descr[0].value.string.elements = elts.elements;
	descr[1].type =	OM_NO_MORE_TYPES;

	if ((ret = omP_put(subject, modification, &descr[0], types,	value_position,
	value_position + 1)) != OM_SUCCESS) {
		if (pub) {
			omP_delete(pub);
			if (modification ==	OM_REPLACE_CERTAIN_VALUES) {
				OMP_XFREE(OMP_WORKSPACE(subject), elts.elements, AL_STRING,
				ROUNDUP(OMP_WORKSPACE(subject), elts.length + 1));
			} /* endif */
		} /* endif */
		return(ret);
	} /* endif */

	/*
	 * Update Next Element Position.
	 */
	*element_position = elts.length;

	if (pub) {
		omP_delete(pub);
		if (modification ==	OM_REPLACE_CERTAIN_VALUES) {
			OMP_XFREE(OMP_WORKSPACE(subject), elts.elements,
				 AL_STRING,	ROUNDUP(OMP_WORKSPACE(subject),	elts.length
				 + 1));
		} /* endif */
	} /* endif */

	return(OM_SUCCESS);
} /* end omP_xwrite	*/
