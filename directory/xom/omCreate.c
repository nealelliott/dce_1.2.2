/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: omCreate.c,v $
 * Revision 1.1.10.2  1996/02/18  18:25:27  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:52  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:40:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:57:02  root]
 * 
 * Revision 1.1.8.3  1994/07/06  15:09:25  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:25:57  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:49:26  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:38:11  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:11:50  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:07:27  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:22:31  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:26:10  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:18:15  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:06:56  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:40:53  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:47:23  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:21:49  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:48:17  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: omCreate.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:25:27 $";
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
/*		@(#)omCreate.c	1.20 (BULL S.A)	7/1/92		*/

#ifdef THREADSAFE
#	include <pthread.h>
#endif

#include <stdio.h>
#include <string.h>
#include <xom.h>
#include <xomip.h>
#include <alloc.h>
#include <omCreate.h>


/* local functions */

static void
link_handle(
    OMP_private_object	*handle_ptr,
    OMX_workspace		workspace);

static OM_boolean
check_package(
    OM_object_identifier class,
    OMP_package	*packages,
    OM_sint max_packages,
    OM_sint	*pack_id, 
    OM_sint *len_pack);

static OM_sint32
get_class_id(
	OM_object_identifier class,
	OM_sint len_pack);

/********************************************************************/
/*																	*/
/* link_handle()													*/
/*																	*/
/*	  insert a new OM private object in	the	private	objects	chain	*/
/*																	*/
/********************************************************************/

static void
link_handle	(
	OMP_private_object	*handle_ptr,
	OMX_workspace		workspace
)
{
	if (workspace->om_private_object_list == (OMP_private_object *	)0)	 {
		/* the chain is	empty */
		((OMP_object_link *		)handle_ptr)->private_forw =
		(OMP_private_object	 * )0;
	} else
	{
		/* chain not empty */
		((OMP_object_link *		)handle_ptr)->private_forw =
		workspace->om_private_object_list;
		((OMP_object_link *		)workspace->om_private_object_list)->private_back =
			(OMP_private_object	 * )handle_ptr;
	}
	((OMP_object_link *		)handle_ptr)->private_back = (OMP_private_object *
		)0;
	workspace->om_private_object_list =		(OMP_private_object	 *	)handle_ptr;
	return;
} /* end link_handle  */


/****************************************************************/
/*								*/
/*	Function :	omP_create				*/
/*								*/
/****************************************************************/
/*								*/
/*	omP_create() creates a private object					*/
/*								*/
/*	Check class	validity									*/
/*		- abstract class								*/
/*		not-concrete code is returned					*/
/*								*/
/*		- class	of result object						*/
/*		function-declined code is returned				*/
/*								*/
/*		- unknown class									*/
/*		no-such-class code is returned					*/
/*								*/
/*	Get	the	package	and	class numbers						*/
/*								*/
/*	Create an object handle									*/
/*								*/
/*	Call the create_switch function	which allocates	the		*/
/*	private	object and initialise it, if needed.			*/
/*								*/
/*	If create_switch return	OM_SUCCESS,	then the object		*/
/*	is linked in the private object	chain					*/
/*								*/
/*	Each function or subroutine	which needs	memory			*/
/*	allocation may return memory-insufficient code			*/
/*								*/
/****************************************************************/


OM_return_code
omP_xcreate(
	OM_object_identifier	class,
	OM_boolean		initialise,
	OM_workspace		workspace,
	OM_private_object	*object
)

{
	OMP_object_type	class_sub_id;
	OMP_private_object	 * handle_ptr;
	OM_return_code	ret;

	/* Is workspace	initialised?
	 * yes : search	package
	 *	no : return	error code
	 */
	if (((OMX_workspace) workspace)->status	 ==	OM_FALSE)
		return (OM_NO_SUCH_WORKSPACE);

	if ((ret = omP_get_object_type ((OMX_workspace)workspace, class, &class_sub_id)) != OM_SUCCESS)
		return (ret);

	/*
	 * get a free object handle
	 * and initialise it without object	pointer
	 */

	if ((handle_ptr	= omP_get_object_handle	((OMX_workspace)workspace,
	class,	class_sub_id, 0)) == (OMP_private_object *) 0)
		return (OM_MEMORY_INSUFFICIENT);


	if ((ret = omP_create_switch (((OMX_workspace)workspace), class_sub_id,
		 OMP_EXTERNAL(handle_ptr), initialise))	!=
		OM_SUCCESS)	 {
		/* free	allocated handle */
		OMP_XFREE ((OMX_workspace)workspace, handle_ptr, AL_HANDLE,
				1);
		return (ret);
	}

	(void)link_handle(handle_ptr, (OMX_workspace)workspace);

	/* return handle (0th descriptor) into object parameter	*/
	*object	 = (OM_private_object) (OMP_EXTERNAL	(handle_ptr));

	return (OM_SUCCESS);
} /* end omP_xcreate */


/****************************************************************/
/*								*/
/*	omP_create_handle()	creates	a private object handle		*/
/*								*/
/*	Check class	validity									*/
/*		- abstract class								*/
/*		not-concrete code is returned					*/
/*								*/
/*		- unknown class									*/
/*		no-such-class code is returned					*/
/*								*/
/*	Allocate a handle from the heap	of the object handles	*/
/*								*/
/*	Initialise,	if needed, initial values of the XDS object	*/
/*	parameter												*/
/*								*/
/*								*/
/*	Each function or subroutine	which needs	memory			*/
/*	allocation may return memory-insufficient code			*/
/*								*/
/****************************************************************/

OM_return_code
omP_xcreate_handle(
	OM_object_identifier	class,
	OM_workspace		workspace,
	void *priv_object,
	OM_private_object	*object
)

{
	OMP_object_type	class_sub_id;
	OMP_private_object	 * handle_ptr;
	OM_return_code	ret;

	/* Is workspace	initialised?
	 * yes : search	package
	 *	no : return	error code
	 */
	if ( ((OMX_workspace )workspace	 ==	(OMX_workspace )0) ||  (((OMX_workspace	)workspace)->status	==
		OM_FALSE) )
		return (OM_NO_SUCH_WORKSPACE);

	if ((ret = omP_get_object_type ((OMX_workspace )workspace, class, &class_sub_id)) != OM_SUCCESS)
		return (ret);

	if ((ret = omP_cr_handle_switch	(((OMX_workspace )workspace), class_sub_id))	 !=
		OM_SUCCESS)
		return (ret);

	/* get a free object handle
	 * and initialise it
	 */
	if ((handle_ptr = omP_get_object_handle ((OMX_workspace )workspace,
		 class,		class_sub_id, priv_object))	 ==	(OMP_private_object	*) 0)
		return (OM_MEMORY_INSUFFICIENT);

	(void)link_handle(handle_ptr, (OMX_workspace )workspace);

	/* return handle (0th descriptor) into object parameter	*/
	*object	 = (OM_private_object)(OMP_EXTERNAL(handle_ptr));

	return (OM_SUCCESS);
} /* end omP_xcreate_handle	*/


/********************************************************************/
/*									*/
/*	check_package()							*/
/*									*/
/*	checks that	the	package	identifier belongs to the		*/
/*	packages table given in	argument				*/
/********************************************************************/
static OM_boolean
check_package(OM_object_identifier class,
	OMP_package	*packages, OM_sint max_packages,
	OM_sint	*pack_id, OM_sint *len_pack)
{
	OM_sint	pack;
	OMP_package	 * package_ptr;

	/* check if	package	idendifier is present in packages table	*/
	for	(pack =		0, package_ptr = packages; pack	 < max_packages; pack++,
			package_ptr++) {
		if (package_ptr->feature.length	 ==	0)
			continue;
		/*
			* Here we suppose that the length of the string
			* could	not	be OM_LENGTH_UNSPECIFIED :
			* An OID can contain a null	member
			* e.g 1.3.0.1
			*/
		if (package_ptr->feature.length	 >=	class.length)
			continue;

		if (memcmp(package_ptr->feature.elements, class.elements,
			 package_ptr->feature.length) == 0)	 {
			*len_pack =		package_ptr->feature.length;
			*pack_id = pack;
			return(OM_TRUE);
		}
	}
	return(OM_FALSE);
} /* end check_package */


/********************************************************/
/*														*/
/*		omP_get_object_type()								*/
/*														*/
/*		gets the object	type from a	class identifier	*/
/*				(package number,class sub-id)			*/
/*		The	package	identifier must	belong to the		*/
/*		packages known by XOM. The package may be		*/
/*		negotiated or not.								*/
/********************************************************/
OM_return_code
omP_get_object_type	(
	OMX_workspace			workspace,
	OM_object_identifier	class,
	OMP_object_type	*class_id
)
{
	OM_sint	len_pack;
	OM_sint	pack_id;

	if (!check_package(class, workspace->packages_table, NBMAXPACKAGES,
		 &pack_id, &len_pack))
		if (check_package(class, workspace->closure_packages_table,
			 NBMAX_CLOSURE_PACKAGES, &pack_id, &len_pack)) {
			pack_id	++;
			pack_id	 = -pack_id;		/* negative	number for closure	*/
		}
		else
			return(OM_NO_SUCH_CLASS);

	class_id->package =		pack_id	;
	class_id->class	 = get_class_id(class, len_pack);
	return(OM_SUCCESS);
} /* end omP_get_object_type  */


/********************************************************************/
/*									*/
/*	get_class_id()							*/
/*									*/
/*	extracts the class sub-identifier from the class		*/
/*			identifier					*/
/********************************************************************/

static OM_sint32
get_class_id(
	OM_object_identifier class,
	OM_sint len_pack)
{

	OM_sint	i;
	OM_sint32 sub_id;
	char	*sub_id_ptr;

	/* get sub-identifier class	*/
	sub_id = 0;
	sub_id_ptr = class.elements;
	sub_id_ptr += len_pack;

	/*
		* Here we suppose that the length of the string
		* could	not	be OM_LENGTH_UNSPECIFIED :
		* An OID can contain a null	member
		* e.g 1.3.0.1
		*/
	for	(i = len_pack; i < class.length	; i++) {
		sub_id <<= 7;
		sub_id += (OM_sint32)(*sub_id_ptr++	 & 0x7f);
	}

	return (sub_id);
} /* end get_class_id */


/********************************************************************/
/*									*/
/*	omP_check_negotiated_package()					*/
/*									*/
/*	checks that	the	package	class number has been negotiated	*/
/*									*/
/********************************************************************/
OM_boolean
omP_check_negotiated_package(
	OM_workspace workspace,
	OM_sint	package_no)
{
	OM_boolean * feature = ((OMX_workspace )workspace)->feature;
	if (feature[package_no]	 ==	OM_FALSE	)
		return(OM_FALSE);
	return(OM_TRUE);
} /* end omP_check_negotiated_package */


/********************************************************************/
/*																	*/
/* get_class_subid()												*/
/*																	*/
/* get the package and the class numbers corresponding				*/
/* to the OM_object_identifier class								*/
/*																	*/
/* returns an OMP_object_type										*/
/*																	*/
/*																	*/
/********************************************************************/

OM_return_code
omP_get_class_sub_id (
	OMX_workspace	workspace,
	OM_object_identifier	class,
	OMP_object_type	*class_sub_id
)
{

	OM_sint	len_pack;
	OM_sint	pack_id;

	if ((!check_package(class, workspace->packages_table, NBMAXPACKAGES,
		 &pack_id, &len_pack)) ||  (!omP_check_negotiated_package((OM_workspace)workspace,
																pack_id)))
		return(OM_NO_SUCH_CLASS);

	class_sub_id->package =		pack_id	;
	class_sub_id->class	 = get_class_id(class, len_pack);
	return(OM_SUCCESS);
} /* end omP_get_class_sub_id  */


/********************************************************************/
/*																	*/
/* omP_get_object_handle()												*/
/*																	*/
/* get a free object handle											*/
/* initialise -1st and 0th descriptors:								*/
/* - workspace pointer												*/
/* - attribute type	= OM_CLASS										*/
/* - syntax	= OBJECT_IDENTIFIER	|| PRIVATE + SERVICE GENERATED		*/
/* - object	type = sub-identifier class								*/
/* - private object	pointer											*/
/*																	*/
/********************************************************************/

OMP_private_object *
omP_get_object_handle (
	OMX_workspace	workspace,
	OM_object_identifier	class,
	OMP_object_type		class_sub_id,
	void *priv_object
)
{
	OM_descriptor	 * header_ptr;
	OMP_private_object	 * handle_ptr;

	if ((handle_ptr	 = (OMP_private_object *	)OMP_XALLOC(workspace,
		 AL_HANDLE,		1))	 ==		(OMP_private_object	 * )	0 )
		return (handle_ptr);

	/* erase allocated memory */
	memset (handle_ptr,		0, sizeof(OMP_private_object));

	header_ptr = handle_ptr->OMP_object_header;
	header_ptr->value.string.elements =		(void *)workspace;
	header_ptr++;
	header_ptr->type = OM_PRIVATE_OBJECT;
	header_ptr->syntax = OM_S_PRIVATE |		OM_S_SERVICE_GENERATED |  OM_S_OBJECT_IDENTIFIER_STRING;

	/*
		* Here we suppose that the length of the string
		* could	not	be OM_LENGTH_UNSPECIFIED :
		* An OID can contain a null	member
		* e.g 1.3.0.1
		*/
	header_ptr->value.string.length	 = class.length;
	if ((header_ptr->value.string.elements = (void *)OMP_XALLOC_STR(workspace,
		 class.elements, class.length))	 ==	NULL) {
		OMP_XFREE(workspace, handle_ptr, AL_HANDLE,		1);
		return ( (OMP_private_object * ) 0 );
	}
	handle_ptr->object_type	 = class_sub_id;
	handle_ptr->priv_object	 = priv_object;

	handle_ptr->subobjects_list.private_forw = (OMP_private_object *)NULL;
	handle_ptr->subobjects_list.private_back = (OMP_private_object *)NULL;

	return (handle_ptr);
} /* end omP_get_object_handle	*/
