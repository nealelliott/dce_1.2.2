/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: omGet.c,v $
 * Revision 1.1.10.2  1996/02/18  18:25:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:56  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:40:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:57:05  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:50:18  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:38:14  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:11:54  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:08:02  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:23:43  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:26:52  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:18:24  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:07:16  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:41:48  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:48:15  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:22:06  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:48:29  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: omGet.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:25:33 $";
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
/*	@(#)omGet.c	1.12 (Bull S.A)	6/26/92	*/

/****************************************************************/
/*								*/
/*	Function :	omP_get					*/
/*								*/
/****************************************************************/
/*								*/
/*	FUNCTION: omP_get()						*/
/*								*/
/*	SHORT DESCRIPTION:						*/
/*	omP_get	creates	a public copy of all or	particular parts*/
/*		of a private object					*/
/*								*/
/*	INPUT-PARAMETER:						*/
/*	original	private	object				*/
/*	exclusions	exclusions applied to the attributes of	*/
/*			the	object but not to those	of its	*/
/*			subobjects				*/
/*	included_types	types of the attributes	to be included	*/
/*			in the copy	(public	object)		*/
/*	local_strings	if true,translate strings representation*/
/*	initial_value	value position of the first	value to be	*/
/*			included in	the	copy			*/
/*	limiting_value	value position,	one	beyond that	of the	*/
/*			last value to be included in the copy	*/
/*								*/
/*	OUTPUT-PARAMETER:						*/
/*	copy		The	copy (public object). Present if	*/
/*			return code	is success and exclude_des-	*/
/*			criptor	is not specified		*/
/*	total_number	number of attribute	descriptors	returned*/
/*			in the public object			*/
/*								*/
/*	RETURN-VALUE:						*/
/*	OM_SUCCESS						*/
/*	OM_NOT_PRIVATE						*/
/*	OM_NO_SUCH_EXCLUSION					*/
/*	OM_NO_SUCH_TYPE						*/
/*								*/
/****************************************************************/

#ifdef THREADSAFE
#	include <pthread.h>
#endif

#include <xom.h>
#include <xomip.h>
#include <omGet.h>


OM_return_code
omP_alloc_sub_handle(
    OM_private_object original, 
    OM_object_identifier class,
    void *pri_obj,
    OM_private_object *handle
);

#define	OMP_EX_MASK		  ~(OM_EXCLUDE_ALL_BUT_THESE_TYPES |	\
				OM_EXCLUDE_ALL_BUT_THESE_VALUES	|	\
				OM_EXCLUDE_MULTIPLES	|		\
				OM_EXCLUDE_SUBOBJECTS	|		\
				OM_EXCLUDE_VALUES	|		\
				OM_EXCLUDE_DESCRIPTORS)

#define	OMP_VALID_EX(excl)	((excl & OMP_EX_MASK)?0:1)

OM_return_code
omP_xget(
	OM_private_object	original,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number
)

{

	OM_return_code	ret;
	OMX_workspace	workspace;
	OM_descriptor	 * desc_ptr;

	if (OMP_WORKSPACE(original)->status	 !=	WORKSPACE_INIT)
		return(OM_NO_SUCH_WORKSPACE);

	/* Check validity of the original object	*/
	if (!OMP_PRIVATE (original))
		return (OM_NOT_PRIVATE);

	/* Check validity of the requested exclusions	*/
	if (!OMP_VALID_EX(exclusions))
		return (OM_NO_SUCH_EXCLUSION);

	/* Check types parameters			*/
	if ((exclusions	 & OM_EXCLUDE_ALL_BUT_THESE_TYPES) &&  (included_types ==
		0))
		return(OM_NO_SUCH_TYPE);
	/* ignore included_types if	OM_EXCLUDE_ALL_BUT_THESE_TYPES absent*/
	if ((!(exclusions &		OM_EXCLUDE_ALL_BUT_THESE_TYPES)) &&	 (included_types	 !=
		0))
		included_types = 0;

	if ((exclusions	 & OM_EXCLUDE_ALL_BUT_THESE_VALUES)	&&	(initial_value >
		limiting_value))
		return(OM_WRONG_VALUE_POSITION);

	if (!(exclusions & OM_EXCLUDE_DESCRIPTORS))	 {
		if (!copy)
			return(OM_POINTER_INVALID);
		*copy =		0;
	}
	if (!total_number)
		return(OM_POINTER_INVALID);
	*total_number =		0;
	if ((ret = omP_get_switch(OMP_WORKSPACE(original), original, exclusions,
		 included_types, local_strings,		initial_value,
		limiting_value,	copy, total_number)) !=	 OM_SUCCESS)
		return (ret);

	if (!(exclusions & OM_EXCLUDE_DESCRIPTORS))	 {
		/* link	SPUB object	in workspace SPUB object list */
		workspace =		(OMX_workspace)OMP_WORKSPACE(original);
		desc_ptr = OMP_INTERNAL(*copy);
		if (workspace->om_spub_object_list)	 {
			((OMP_object_link *		)(OMP_private_object *	)workspace->om_spub_object_list)->private_back =
				(OMP_private_object	 * )desc_ptr;
		}
		((OMP_object_link *		)desc_ptr)->private_forw =	(OMP_private_object	 *
			)workspace->om_spub_object_list;
		((OMP_object_link *		)desc_ptr)->private_back =	(OMP_private_object	 *
			)0;
		workspace->om_spub_object_list = desc_ptr;
	}

	return(OM_SUCCESS);
} /* end omP_xget */


/********************************************************/
/*	omP_update_descriptor()				*/
/*		updates	descriptor -1st			*/
/*			descriptor 0th			*/
/********************************************************/
void
omP_update_descriptor(
	OM_private_object	original,
	OM_descriptor	*desc0)
{
	OM_descriptor *		desc_ptr;

	/* initialize descriptor -1st			*/
	desc_ptr = OMP_INTERNAL(desc0);
	desc_ptr->value.string.elements	 = OMP_WORKSPACE(original);
	desc0->syntax |= OM_S_SERVICE_GENERATED;
	return;
} /* end omP_update_descriptor */


/********************************************************/
/*							*/
/*	omP_alloc_sub_handle()				*/
/*		allocates and initializes the handle	*/
/*		updates	the	subobjects link		*/
/*		updates	OMP_S_SUBOBJECT	flag		*/
/*							*/
/********************************************************/
OM_return_code
omP_alloc_sub_handle(
	OM_private_object original,
	OM_object_identifier class,
	void *pri_obj,
	OM_private_object *handle)
{
	OMP_object_type	class_sub_id;
	OMP_private_object	 * handle_ptr;
	OMP_private_object	 * hand_orig;
	OM_return_code		ret;

	/* get class sub_id		*/
	if ((ret = omP_get_object_type(OMP_WORKSPACE(original),	class, &class_sub_id)) != OM_SUCCESS)
		return(ret);

	/* get object handle		*/
	if ((handle_ptr	= (OMP_private_object*)omP_get_object_handle(OMP_WORKSPACE(original), class, class_sub_id, pri_obj)) ==	0)
		return(OM_MEMORY_INSUFFICIENT);

	/* update object kind (subobject)	*/
	*handle	 = OMP_EXTERNAL(handle_ptr);
	OMP_SET_SUBOBJECT(*handle);

	/* update subobjects links	*/
	hand_orig =		(OMP_private_object	 * )	OMP_INTERNAL(original);

	handle_ptr->subobjects_list.private_forw =	hand_orig->subobjects_list.private_forw;
	if ( hand_orig->subobjects_list.private_forw !=		NULL)
		(hand_orig->subobjects_list.private_forw)->subobjects_list.private_back	 =
			handle_ptr;
	handle_ptr->subobjects_list.private_back = hand_orig;
	hand_orig->subobjects_list.private_forw	 = handle_ptr;

	return(OM_SUCCESS);
} /* end omP_alloc_sub_handle */



