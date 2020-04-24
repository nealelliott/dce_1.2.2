/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: omDelete.c,v $
 * Revision 1.1.10.2  1996/02/18  18:25:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:53  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:40:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:57:03  root]
 * 
 * Revision 1.1.8.2  1994/07/06  15:09:27  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:26:00  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  18:11:51  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:07:37  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:22:49  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:26:21  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:18:18  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:07:02  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:41:06  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:47:37  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  21:01:16  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:42:41  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  23:21:58  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:48:23  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: omDelete.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:25:29 $";
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
/*	@(#)omDelete.c	1.17 (BULL S.A)	7/1/92	*/

/****************************************************************/
/*								*/
/*	Function :	omP_delete				*/
/*								*/
/****************************************************************/

#ifdef THREADSAFE
#	include <pthread.h>
#endif

#include <xom.h>
#include <xomip.h>
#include <alloc.h>
#include <omDelete.h>


void
delete_desc_value(
    OMX_workspace	workspace,
    OM_descriptor *desc_ptr
);

/****************************************************************/
/*								*/
/*	FUNCTION: omP_delete()					*/
/*								*/
/*	SHORT DESCRIPTION:						*/
/*	omP_delete() deletes a private or a	service-generated	*/
/*		public object					*/
/*								*/
/*	case private object	:					*/
/*		free XDS object	and	subobjects (IAPL structures)*/
/*		free subobject handles (created	by om_get)	*/
/*		update the private objects list			*/
/*		free the object	handle				*/
/*								*/
/*	case public	object:					*/
/*		free resources occupied	by attribute values	*/
/*			(string, object	(only public) )		*/
/*		free descriptors				*/
/*								*/
/*	case private or	public	subobject :			*/
/*		function declined				*/
/*								*/
/*	INPUT-PARAMETER:						*/
/*	subject	= object to	delete				*/
/*								*/
/*	OUTPUT-PARAMETER:						*/
/*								*/
/*	RETURN-VALUE:						*/
/*	OM_NO_SUCH_OBJECT	subject	is not an object	*/
/*	OM_NO_SUCH_WORKSPACE	workspace shutdown		*/
/*	OM_SUCCESS		omP_delete successful		*/
/*								*/
/****************************************************************/


OM_return_code
omP_xdelete(
	OM_object  subject
)
{
	OM_sint32		nbdesc;
	OMP_object_link		 * link_xobj;
	OM_return_code		ret;
	OMP_private_object	 * object;
	OMP_private_object	 * xobject;
	OM_descriptor		 * desc_ptr;
	OMX_workspace		workspace =		OMP_WORKSPACE(subject);

	/*************************************/
	/* Delete a	private	object		*/
	/*************************************/
	if (OMP_PRIVATE(subject)) {

		/* Check if	the	workspace status is	initialized	*/
		if (OMP_WORKSPACE(subject)->status != WORKSPACE_INIT)
			return(OM_NO_SUCH_WORKSPACE);

		/* If it is	a subobject	do nothing	*/
		if (OMP_SUBOBJECT(subject))	 {
			return(OM_SUCCESS);
		}

		/* Free	class identifier		*/
		/*
			* Here we suppose that the length of the string
			* could	not	be OM_LENGTH_UNSPECIFIED :
			* An OID can contain a null	member
			* e.g 1.3.0.1
			*/
		OMP_XFREE_STR(workspace, (OMP_CLASS_ID_STR(subject)).elements,
			 (OMP_CLASS_ID_STR(subject)).length);

		xobject	 = (OMP_private_object *	)OMP_INTERNAL(subject);

		/* Check the magic number of subject:
			- if it	is equal to	OMP_S_INVALIDATED_SUBOBJECT, we	are
			  deleting a SPUB subobject	(recursive call	cf.
			  delete_desc_value) so	subject	is not chained in the
			  private objects list but we have to suppress it from
			  the subhandle	list
		 */
		if (((OMP_subobject	 * )xobject)->magic	== OMP_S_INVALIDATED_SUBOBJECT)	 {
			/* Update the subhandle	list */
			link_xobj =		&(xobject->subobjects_list);
			/* Update the forward link of the previous object		*/
			if (link_xobj->private_back)
				((OMP_private_object * )(link_xobj->private_back))->subobjects_list.private_forw =
					link_xobj->private_forw;
			else
				omP_error("omP_delete PRI :	subhanle list error\n");

			/* Update the backward link	of the forward object		*/
			if (link_xobj->private_forw)
				((OMP_private_object * )(link_xobj->private_forw))->subobjects_list.private_back =
					link_xobj->private_back;
		} else { /*	It is not a	subhandle */
			/* Free	the	object			*/
			if (xobject->priv_object) {
				if (( ret =		omP_delete_switch(OMP_WORKSPACE(subject),
					 subject)) != OM_SUCCESS)
					return(ret);
			}

			/* Invalidate the subobjects handles		*/
			object = xobject->subobjects_list.private_forw;	/* subobjects list header	*/
			while (object != NULL) {
				/* check if	it is a	private	subobject	*/

				if (!(OMP_PRIVATE(OMP_EXTERNAL(object))	 &&
					(OMP_SUBOBJECT(OMP_EXTERNAL(object)))))
					omP_error("omP_delete PRI :	subobject kind error\n");

				((OM_descriptor	 * )object)->value.string.elements =
					(void *)OMX_invalid_workspace;
				object = object->subobjects_list.private_forw;

			}

			/* Update the Private objects list	*/
			link_xobj =		(OMP_object_link * )OMP_INTERNAL(subject);
			/* Update the forward link of the previous object	*/
			if (link_xobj->private_back)
				((OMP_object_link *		)(link_xobj->private_back))->private_forw =
					link_xobj->private_forw;
			else
				OMP_WORKSPACE(subject)->om_private_object_list =
					link_xobj->private_forw;

			/* Update the backward link	of the forward object	*/
			if (link_xobj->private_forw)
				((OMP_object_link *		)(link_xobj->private_forw))->private_back =
					link_xobj->private_back;
		}

		/* Free	the	object header		*/
		OMP_XFREE(workspace, OMP_INTERNAL(subject),	AL_HANDLE, 1);

		return(OM_SUCCESS);

	}
	/*************************************/
	/* Delete a	Public object		*/
	/*************************************/
	if (OMP_SPUB(subject)) {
		/* If it is	a subobject	do nothing		*/
		if (OMP_SUBOBJECT(subject))
			return(OM_SUCCESS);
		/* loop	on all descriptors from	descriptor 0	*/
		/* delete resources	occupied by	class identifier*/
		/*			 and  attribute	values		*/
		for	(nbdesc	 = 0, desc_ptr =	subject; desc_ptr->type	 !=
			OM_NO_MORE_TYPES; nbdesc++,		desc_ptr++)	 {

			(void)delete_desc_value(workspace, desc_ptr);
		}

		/* Check the magic number of subject:
			- if it	is equal to	OMP_S_INVALIDATED_SUBOBJECT, we	are
			  deleting a SPUB subobject	(recursive call	cf.
			  delete_desc_value) so	subject	is not chained in the
			  SPUB objects list
			- else,	it is a	subobject that has to be removed from
			  the subobjects list
		 */
		if (((OMP_subobject	 * )OMP_INTERNAL(subject))->magic !=
			OMP_S_INVALIDATED_SUBOBJECT)	 {
			/* Update the SPUB objects list	*/
			link_xobj =		(OMP_object_link * )OMP_INTERNAL(subject);
			/* Update the forward link of the previous object		*/
			if (link_xobj->private_back)
				((OMP_object_link *		)(link_xobj->private_back))->private_forw =
					link_xobj->private_forw;
			else
				OMP_WORKSPACE(subject)->om_spub_object_list	 =
					(OM_descriptor * )link_xobj->private_forw;

			/* Update the backward link	of the forward object		*/
			if (link_xobj->private_forw)
				((OMP_object_link *) (link_xobj->private_forw))
				->private_back = link_xobj->private_back;
		}

		/* free	the	descriptors		*/
		nbdesc = nbdesc	 + 2; /* number of descriptors (+-1st and last NULL)*/
		OMP_XFREE(workspace, OMP_INTERNAL(subject), AL_DESCRIPTOR, nbdesc);
		if (((OMX_workspace)workspace)->status == WORKSPACE_SHUT) {
			/* if all public objects are freed,	free workspace memory
		   allocation */
			if ((workspace->om_private_object_list ==  NULL) &&
				(workspace->om_spub_object_list	==	NULL))
				omP_free_workspace((OMX_workspace)workspace);
		}
		return(OM_SUCCESS);

	} else
		return(OM_NO_SUCH_OBJECT);
} /* end omP_xdelete */


/****************************************************************/
/*								*/
/*	delete_desc_value()					*/
/*		frees ressources value in a	descriptor		*/
/****************************************************************/
void
delete_desc_value(
	OMX_workspace	workspace,
	OM_descriptor *desc_ptr)
{
	OM_return_code		ret;
	OM_object		object;
	switch (OMP_SYNTAX(desc_ptr)) {

		/* string syntax	*/
	case OM_S_BIT_STRING :
	case OM_S_ENCODING_STRING :
	case OM_S_OBJECT_IDENTIFIER_STRING:
	case OM_S_GENERAL_STRING:
	case OM_S_GENERALISED_TIME_STRING:
	case OM_S_GRAPHIC_STRING:
	case OM_S_IA5_STRING:
	case OM_S_NUMERIC_STRING:
	case OM_S_OBJECT_DESCRIPTOR_STRING:
	case OM_S_OCTET_STRING:
	case OM_S_PRINTABLE_STRING:
	case OM_S_TELETEX_STRING:
	case OM_S_UTC_TIME_STRING:
	case OM_S_VIDEOTEX_STRING:
	case OM_S_VISIBLE_STRING:
		if (desc_ptr->value.string.elements	 !=	OM_ELEMENTS_UNSPECIFIED)
			OMP_XFREE_STR(workspace, desc_ptr->value.string.elements,
				 (desc_ptr->value.string.length	 !=	OM_LENGTH_UNSPECIFIED)
				 ?				desc_ptr->value.string.length :	 strlen(desc_ptr->value.string.elements));
		break;

		/* object syntax	*/
	case OM_S_OBJECT:

		if ((object	 = desc_ptr->value.object.object) ==  0)
			break;

		if (OMP_SUBOBJECT(object)) {
			OMP_RESET_SUBOBJECT(object);
			if ((ret = omP_delete(object)) != OM_SUCCESS)
				omP_error("omP_delete :	delete subobject\n");
			break;
		} else
			omP_error("omP_delete SPUB : subobject kind	error\n");


	case OM_S_NULL:
	case OM_S_BOOLEAN:
	case OM_S_ENUMERATION:
	case OM_S_INTEGER:
		break;
	default:			/* other syntaxes	*/
		if ((ret = omP_delete_specific_value(workspace,	desc_ptr))	 !=
			OM_SUCCESS)
			omP_error("omP_delete :	incorrect syntax in	SPUB\n");
		break;
	}
	return ;
} /* end delete_desc_value */


