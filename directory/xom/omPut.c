/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: omPut.c,v $
 * Revision 1.1.10.2  1996/02/18  18:25:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:58  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:40:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:57:07  root]
 * 
 * Revision 1.1.8.1  1994/02/22  18:11:56  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:08:19  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:24:09  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:27:11  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:18:29  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:07:26  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:42:09  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:48:38  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:22:27  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:51:14  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: omPut.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:25:38 $";
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
/*	@(#)omPut.c	1.9	(Bull S.A) 6/9/92	*/

/****************************************************************/
/*								*/
/*	Function :	omP_put					*/
/*								*/
/****************************************************************/
/*								*/
/*	omP_put() place	or replace in one private object		*/
/*			  copies of	the	attribute values of	another		*/
/*			  public or	private	object						*/
/*								*/
/*	Check objects validity									*/
/*		- destination is a public object				*/
/*		not-private	code is	returned					*/
/*								*/
/*		- source is	neither	public nor private object	*/
/*		no-such-object code	is returned					*/
/*								*/
/*	Check existing modification								*/
/*		- unknown modification							*/
/*		no-such-modification code is returned			*/
/*								*/
/*	Call the put_switch	function which calls the put_xxx	*/
/*	function associated	to the xxx type	of object			*/
/*								*/
/*	Each function or subroutine	which needs	memory			*/
/*	allocation may return memory-insufficient code			*/
/*								*/
/*	RETURN_CODE:						*/
/*								*/
/*	from omP_put:											*/
/*	OM_NO_SUCH_MODIFICATION									*/
/*	OM_NO_SUCH_OBJECT										*/
/*	OM_NO_SUCH_WORKSPACE									*/
/*	OM_NOT_PRIVATE											*/
/*								*/
/*	from omP_put_switch:									*/
/*	OM_SUCCESS												*/
/*	OM_FUNCTION_DECLINED									*/
/*	OM_MEMORY_INSUFFICIENT									*/
/*	OM_NO_SUCH_CLASS										*/
/*	OM_NO_SUCH_OBJECT										*/
/*	OM_NO_SUCH_SYNTAX										*/
/*	OM_NO_SUCH_TYPE											*/
/*	OM_NOT_CONCRETE											*/
/*	OM_NOT_PRESENT											*/
/*	OM_TOO_MANY_VALUES										*/
/*	OM_VALUES_NOT_ADJACENT									*/
/*	OM_WRONG_VALUE_LENGTH									*/
/*	OM_WRONG_VALUE_MAKEUP									*/
/*	OM_WRONG_VALUE_NUMBER									*/
/*	OM_WRONG_VALUE_SYNTAX									*/
/*	OM_WRONG_VALUE_TYPE										*/
/*								*/
/****************************************************************/

#ifdef THREADSAFE
#	include <pthread.h>
#endif

#include <xom.h>
#include <xomip.h>
#include <alloc.h>
#include <omPut.h>


OM_return_code
omP_xput(
	OM_private_object	destination,
	OM_modification		modification,
	OM_object		source,
	OM_type_list		included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value
)

{
	OM_descriptor	 * header_ptr;
	OMP_private_object	 * handle_ptr;
	OM_return_code	ret;
	OMX_workspace	workspace;

	/* Check validity of the destination object	*/
	if (OMP_PUBLIC (destination))
		return (OM_NOT_PRIVATE);

	/* Check validity of the source	object */
	if ( (!OMP_PRIVATE(source))	 &&	(!OMP_PUBLIC	(source)) )
		return (OM_NO_SUCH_OBJECT);

	if (OMP_SPUB(source) &&		(!OMP_VALID_OBJECT(source)))
		return (OM_NO_SUCH_OBJECT);

	/* check valid handle */
	if (OMP_PRIVATE(source)	 &&	(!OMP_VALID_OBJECT(source)))
		return(OM_NO_SUCH_OBJECT);

	/* check if	destination	is a subobject */
	if (OMP_SUBOBJECT(destination))	 {
		return(OM_FUNCTION_DECLINED);
	} /* endif */

	/* Check validity of the requested modification	*/
	if (!OMP_VALID_MOD(modification))
		return (OM_NO_SUCH_MODIFICATION);

	handle_ptr = (OMP_private_object * ) (OMP_INTERNAL (destination));
	header_ptr = (OM_descriptor	 * )	handle_ptr;

	/* Is the workspace	initialised?
	 * yes : search	package
	 *	no : return	error code
	 */
	workspace =		(OMX_workspace)	header_ptr->value.string.elements;
	if ( workspace->status != WORKSPACE_INIT)
		return (OM_NO_SUCH_WORKSPACE);

	switch ( modification)	 {
	case OM_INSERT_AT_BEGINNING:
			initial_value =		(OM_value_position)	0;
	case OM_INSERT_AT_CERTAIN_POINT:
			limiting_value = initial_value;
		break;
	case OM_INSERT_AT_END:
			initial_value =		OM_ALL_VALUES;
		limiting_value = initial_value;
		break;
	case OM_REPLACE_ALL:
			initial_value =		(OM_value_position)	0;
		limiting_value = OM_ALL_VALUES;
		break;
	case OM_REPLACE_CERTAIN_VALUES:
			if ((initial_value == OM_ALL_VALUES) ||	 ((limiting_value !=
				OM_ALL_VALUES) &&  initial_value >=
				limiting_value))
				return (OM_WRONG_VALUE_POSITION);
		break;

	default:
		return (OM_NO_SUCH_MODIFICATION);
	} /* endswitch */
	/* at this point, initial_value	and	limiting_value are significant
	 *
	 * initial_value ==	limiting_value
	 *	=> insert:
	 *		at begining	= 0
	 *		at end = OM_ALL_VALUES
	 *		at certain point = n
	 * initial_value < limiting_value
	 *	=> replace:
	 *		all	: init = 0
	 *			  limit	= OM_ALL_VALUES
	 *		certain	values : init =	i
	 *				 limit = l or OM_ALL_VALUES
	 */

	if ((ret = omP_put_switch (OMP_WORKSPACE(destination), handle_ptr->object_type,
		 destination, modification,		source,
		included_types,	 initial_value,	limiting_value)) !=		 OM_SUCCESS)
		return (ret);

	return (OM_SUCCESS);
} /* end omP_xput */


