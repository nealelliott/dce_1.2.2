/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: omWkspace.c,v $
 * Revision 1.1.10.2  1996/02/18  18:25:47  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:48:05  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:41:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:57:12  root]
 * 
 * Revision 1.1.8.3  1994/07/06  15:09:29  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:26:14  marrek]
 * 
 * Revision 1.1.8.2  1994/06/21  14:49:01  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:49:21  marrek]
 * 
 * Revision 1.1.8.1  1994/02/22  19:00:49  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:09:12  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  07:25:38  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:28:04  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:18:43  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:07:46  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:43:19  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:49:45  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:23:01  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:51:46  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: omWkspace.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:25:47 $";
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
/*	@(#)omWkspace.c	1.20 (BULL S.A)	7/1/92	*/

/****************************************************************/
/*								*/
/*		Functions :		omP_initialize()		*/
/*				omP_shutdown ()			*/
/*				omP_version()			*/
/*								*/
/****************************************************************/

#ifdef THREADSAFE
#	include <pthread.h>
#endif

#include <stdio.h>
#include <string.h>
#include <assert.h>
#include <xom.h>
#include <xomip.h>
#include <xomtrace.h>
#include <alloc.h>
#include <omWkspace.h>



OM_return_code
omP_free_package_table (
    OM_workspace	workspace);

/*	number of functions		*/
#define	NBFUNC	(sizeof(OMP_functions)/sizeof(long)	-1)


static OMP_functions	om_functions	 =
{
	NBFUNC,
	(OMP_copy)omP_copy,
	(OMP_copy_value)omP_copy_value,
	(OMP_create)omP_create,
	(OMP_decode)omP_decode,
	(OMP_delete)omP_delete,
	(OMP_encode)omP_encode,
	(OMP_get)omP_get,
	(OMP_instance)omP_instance,
	(OMP_put)omP_put,
	(OMP_read)omP_read,
	(OMP_remove)omP_remove,
	(OMP_write)omP_write
};


static OMP_functions	om_iw_functions		 =
{
	NBFUNC,
	(OMP_copy)omP_iw_copy,
	(OMP_copy_value)omP_iw_copy_value,
	(OMP_create)omP_iw_create,
	(OMP_decode)omP_iw_decode,
	(OMP_delete)omP_iw_delete,
	(OMP_encode)omP_iw_encode,
	(OMP_get)omP_iw_get,
	(OMP_instance)omP_iw_instance,
	(OMP_put)omP_iw_put,
	(OMP_read)omP_iw_read,
	(OMP_remove)omP_iw_remove,
	(OMP_write)omP_iw_write
};


OMP_mutex	*deadlock_mutex	 = (OMP_mutex *) NULL;

struct OMX_workspace_body OMX_invalid_workspace_struct = {
	&om_iw_functions,	/* pointer to jump table	*/
	WORKSPACE_INIT,				/* workspace status		*/
	NULL,			/* mutex pointer		*/
	NULL,			/* private objects list		*/
	NULL,			/* pointer to switch table	*/
	NULL,			/* pointer to package table	*/
	NULL,			/* pointer to closure package table	*/
	NULL,			/* pointer to generic workspace	*/
	NULL,			/* pointer to alloc	table	*/
	{ OM_FALSE },		/* feature array		*/
	NULL			/* pointer to spub list		*/
};


OMX_workspace	OMX_invalid_workspace =	&OMX_invalid_workspace_struct;

OM_sint	omP_trace_opt = OMP_TRACE_OFF ;
FILE	*omP_trace_file	 = stderr;

/****************************************************************/
/*																*/
/*	FUNCTION: omP_package_present								*/
/*																*/
/*	SHORT DESCRIPTION:											*/
/*	   This	new	function test if a package is present in the	*/
/*	   workspace												*/
/*																*/
/*	INPUT-PARAMETER:											*/
/*		workspace		= The workspace	pointer					*/
/*		package_oid		= The package oid						*/
/*																*/
/*	OUTPUT-PARAMETER:											*/
/*		package_present	= boolean which	inducates if the		*/
/*						  package in known in the workspace		*/
/*																*/
/*	RETURN-VALUE:												*/
/*		OM_POINTER_INVALID		not	a valid	workspace pointer	*/
/*		OM_SUCCESS				successful						*/
/*																*/
/****************************************************************/
OM_return_code
omP_xpackage_present (
	OM_workspace		workspace ,
	OM_object_identifier	package_oid	,
	OM_boolean		*package_present)
{
	OM_sint	nb ;
	OMX_workspace	work_ptr ;


	if (workspace == (OM_workspace)	NULL ) {
		return(OM_POINTER_INVALID) ;
	} /* endif */

	work_ptr = (OMX_workspace) workspace ;
	*package_present = OM_FALSE	;

	for	(nb	 = 0; (nb <	NBMAXPACKAGES);	nb++) {
		/*
		* Here we suppose that the length of the string
		* could	not	be OM_LENGTH_UNSPECIFIED :
		* An OID can contain a null	member
		* e.g 1.3.0.1
		*/
		if (work_ptr->packages_table[nb].feature.length == package_oid.length)	 {
			if (!(memcmp(work_ptr->packages_table[nb].feature.elements,
				 package_oid.elements, package_oid.length ))) {
				*package_present = OM_TRUE;
				break;
			} /* endif */
		} /* endif */
	} /* endfor */
	return(OM_SUCCESS);
} /* end omP_xpackage_present  */



/****************************************************************/
/*																*/
/*	FUNCTION: omP_free_package_table()							*/
/*																*/
/*	SHORT DESCRIPTION:											*/
/*	   This	new	function free the workspace	package	table		*/
/*																*/
/*	INPUT-PARAMETER:											*/
/*		workspace		= The workspace	pointer					*/
/*																*/
/*	OUTPUT-PARAMETER:											*/
/*																*/
/*	RETURN-VALUE:												*/
/*		OM_SUCCESS				successful						*/
/*																*/
/****************************************************************/
OM_return_code
omP_free_package_table (
	OM_workspace	workspace)
{
	OMX_workspace	work_ptr ;
	OM_sint	i ;

	work_ptr = (OMX_workspace) workspace ;
	for	(i = 0 ; i < NBMAXPACKAGES ; i++) {
		if ( work_ptr->packages_table[i].feature.length	 != 0 )	 {
			OM_FREE(work_ptr->packages_table[i].feature.elements) ;
		} /* endif */
	} /* endfor */

	OM_FREE(work_ptr->packages_table) ;

	return(OM_SUCCESS) ;
} /* end omP_free_package_table	 */


/****************************************************************/
/*																*/
/*	FUNCTION: omP_add_package()									*/
/*																*/
/*	SHORT DESCRIPTION:											*/
/*	   This	new	function adds a	package	in the workspace		*/
/*																*/
/*	INPUT-PARAMETER:											*/
/*		workspace		= The workspace	pointer					*/
/*		package_feature	= The new package feature				*/
/*																*/
/*	OUTPUT-PARAMETER:											*/
/*																*/
/*	RETURN-VALUE:												*/
/*		OM_POINTER_INVALID		not	a valid	workspace pointer	*/
/*		OM_TOO_MANY_VALUES		too	many packages				*/
/*		OM_MEMORY_INSUFFICIENT	insufficient memory				*/
/*		OM_SUCCESS				successful						*/
/*																*/
/****************************************************************/
OM_return_code
omP_xadd_package (
	OM_workspace		workspace ,
	OMP_package		package_feature)
{
	OMX_workspace	work_ptr ;
	OM_sint	nb ;
	OM_boolean	package_present	;
	OM_return_code	ret;


	if (workspace == (OM_workspace)	NULL ) {
		return(OM_POINTER_INVALID) ;
	} /* endif */

	ret	= omP_package_present(workspace, package_feature.feature,
	&package_present) ;
	if (ret	 != OM_SUCCESS)	{
		return(ret)	;
	} /* endif */
	if ( package_present == OM_TRUE) {
		return(OM_SUCCESS) ;
	} /* endif */

	work_ptr = (OMX_workspace) workspace ;

	/* look	for	the	last position the number of	package	*/
	/*
		* Here we suppose that the length of the string
		* could	not	be OM_LENGTH_UNSPECIFIED :
		* An OID can contain a null	member
		* e.g 1.3.0.1
		*/
	for	(nb = 0;
	(nb < NBMAXPACKAGES) && (work_ptr->packages_table[nb].feature.length != 0);
	nb++) {
		;
	} /* endfor */

	if (nb == NBMAXPACKAGES) {
		return(OM_TOO_MANY_VALUES) ;
	} /* endif */

	/*
		* Here we suppose that the length of the string
		* could	not	be OM_LENGTH_UNSPECIFIED :
		* An OID can contain a null	member
		* e.g 1.3.0.1
		*/
	work_ptr->packages_table[nb].feature.elements = 
	OM_MALLOC(package_feature.feature.length) ;

	if ( work_ptr->packages_table[nb].feature.elements == NULL) {
		return(OM_MEMORY_INSUFFICIENT) ;
	} /* endif */

	/*
		* Here we suppose that the length of the string
		* could	not	be OM_LENGTH_UNSPECIFIED :
		* An OID can contain a null	member
		* e.g 1.3.0.1
		*/
	work_ptr->packages_table[nb].feature.length	 = package_feature.feature.length ;
	memcpy (work_ptr->packages_table[nb].feature.elements ,	package_feature.feature.elements ,
			package_feature.feature.length ) ;

	work_ptr->packages_table[nb].activated = package_feature.activated ;

	if (work_ptr->packages_table[nb].activated == OMP_DEFAULT) {
		work_ptr->feature[nb] = OM_TRUE;
	} /* endif */

	return(OM_SUCCESS) ;

} /* end omP_xadd_package  */


/****************************************************************/
/*																*/
/*	FUNCTION: omP_delete_package()								*/
/*																*/
/*	SHORT DESCRIPTION:											*/
/*	   omP_delete_package allows to	remove a package from		*/
/*	   the package table										*/
/*																*/
/*	INPUT-PARAMETER:											*/
/*		workspace		= The workspace	pointer					*/
/*		package_oid		= The new package oid					*/
/*																*/
/*	OUTPUT-PARAMETER:											*/
/*																*/
/*	RETURN-VALUE:												*/
/*		OM_POINTER_INVALID		not	a valid	workspace pointer	*/
/*		OM_MEMORY_INSUFFICIENT	insufficient memory				*/
/*		OM_SUCCESS				successful						*/
/*																*/
/****************************************************************/
OM_return_code
omP_xdelete_package	(
	OM_workspace			workspace ,
	OM_object_identifier	package_oid)
{
	OMX_workspace	work_ptr ;
	OM_sint	nb ;
	OM_boolean		package_present	;

	package_present	 = OM_FALSE	;
	if (workspace == (OM_workspace)	NULL ) {
		return(OM_POINTER_INVALID) ;
	} /* endif */

	work_ptr = (OMX_workspace) workspace ;

	/*
		* Here we suppose that the length of the string
		* could	not	be OM_LENGTH_UNSPECIFIED :
		* An OID can contain a null	member
		* e.g 1.3.0.1
		*/
	for	(nb = 0;
	(nb < NBMAXPACKAGES) && (work_ptr->packages_table[nb].feature.length != 0);
	nb++) {
		if ( work_ptr->packages_table[nb].feature.length == package_oid.length)	 {
			if (!(memcmp(work_ptr->packages_table[nb].feature.elements ,
				 package_oid.elements ,		package_oid.length ))) {
				package_present	 = OM_TRUE ;
				break ;
			} /* endif */
		} /* endif */
	} /* endfor */

	if ( package_present == OM_FALSE) {
		return(OM_SUCCESS) ;
	} /* endif */

	if (work_ptr->packages_table[nb].activated != OMP_ADDED) {
		return(OM_POINTER_INVALID);
	} /* endif */

	/* Delete the package oid */
	OM_FREE(work_ptr->packages_table[nb].feature.elements) ;

	/*
		* Here we suppose that the length of the string
		* could	not	be OM_LENGTH_UNSPECIFIED :
		* An OID can contain a null	member
		* e.g 1.3.0.1
		*/
	/* Update package_table	*/
	work_ptr->packages_table[nb].feature.length	 = 0	;
	work_ptr->packages_table[nb].feature.elements = NULL ;
	work_ptr->packages_table[nb].activated = OM_FALSE ;

	/* Update feature table	*/
	work_ptr->feature[nb] = OM_FALSE ;

	return(OM_SUCCESS) ;
} /* end omP_xdelete_package  */



/****************************************************************/
/*								*/
/*	FUNCTION: omP_set_generic_workspace							*/
/*								*/
/*	SHORT DESCRIPTION:						*/
/*	   set	  the generic workspace								*/
/*								*/
/*	INPUT-PARAMETER:						*/
/*	workspace		= the  workspace						*/
/*	generic_workspace = the	generic	workspace pointer	*/
/*								*/
/*	OUTPUT-PARAMETER:						*/
/*	NONE												*/
/*								*/
/*	RETURN-VALUE:						*/
/*	OM_INVALID_POINTER		the	workspace pointer is NULL	*/
/*	OM_SUCCESS		omP_initialize successful	*/
/*								*/
/****************************************************************/

OM_return_code
omP_xset_generic_workspace (
	OM_workspace	workspace ,
	void *generic_workspace)
{
	OMX_workspace	work_ptr ;

	if ( workspace == (OM_workspace) NULL) {
		return(OM_POINTER_INVALID) ;
	} /* endif */

	work_ptr = (OMX_workspace) workspace ;
	work_ptr->generic_workspace	 = generic_workspace	;
	return(OM_SUCCESS) ;
} /* end omP_xset_generic_workspace	 */



/****************************************************************/
/*								*/
/*	FUNCTION: omP_get_generic_workspace							*/
/*								*/
/*	SHORT DESCRIPTION:						*/
/*	   return the generic workspace								*/
/*								*/
/*	INPUT-PARAMETER:						*/
/*	workspace		= the  workspace						*/
/*								*/
/*	OUTPUT-PARAMETER:						*/
/*	generic_workspace = the	generic	workspace pointer	*/
/*								*/
/*	RETURN-VALUE:						*/
/*	OM_POINTER_INVALID		the	workspace pointer is NULL	*/
/*	OM_SUCCESS		omP_initialize successful	*/
/*								*/
/****************************************************************/

OM_return_code
omP_xget_generic_workspace (
	OM_workspace	workspace ,
	void **generic_workspace)
{
	OMX_workspace	work_ptr ;

	if ( workspace == (OM_workspace) NULL) {
		*generic_workspace = NULL ;
		return(OM_POINTER_INVALID) ;
	} /* endif */

	work_ptr = (OMX_workspace) workspace ;
	*generic_workspace = work_ptr->generic_workspace ;
	return(OM_SUCCESS) ;
} /* end omP_xget_generic_workspace	 */


/****************************************************************/
/*								*/
/*	FUNCTION: omP_initialize()					*/
/*								*/
/*	SHORT DESCRIPTION:						*/
/*	   omP_initialize is called	by ds_initialize()		*/
/*		either at initialization step				*/
/*		or after a shutdown					*/
/*								*/
/*	   omP_initialize initialize the workspace structure	*/
/*		 and returns a workspace handle				*/
/*								*/
/*	INPUT-PARAMETER:						*/
/*	default_session	= initial values of	session		*/
/*								*/
/*	OUTPUT-PARAMETER:						*/
/*	workspace_return = workspace handle			*/
/*								*/
/*	RETURN-VALUE:						*/
/*	OM_PERMANENT_ERROR	workspace already initialized	*/
/*	OM_MEMORY_INSUFFICIENT	insufficient memory		*/
/*	OM_SUCCESS		omP_initialize successful	*/
/*								*/
/****************************************************************/

OM_return_code
omP_xinitialize	(
	struct OMP_switch_functions_body *sw_functions,
	OMP_package	*packages_table,
	OMP_package	*closure_packages_table,
	void *generic_workspace	,
	struct alloc_table *alloc_table,
	void *default_session,
	OM_workspace	*workspace_return)
{
	OMX_workspace	workspace;		/* workspace handle	*/
	OM_sint	i;
	OM_return_code	ret;
	OMP_package	 * local_packages_table	;

	workspace = (OMX_workspace)OM_MALLOC( sizeof(struct	OMX_workspace_body ));
	memset(workspace, 0, sizeof(struct OMX_workspace_body ));
	if ( workspace == (OMX_workspace) NULL)	 {
		return(OM_MEMORY_INSUFFICIENT) ;
	} /* endif */
	/* initialize Jump table and Package identifier	*/
	workspace->functions = &om_functions;
	workspace->sw_functions	 = sw_functions;

	local_packages_table = (OMP_package	 * )
	OM_MALLOC(sizeof(OMP_package) *		NBMAXPACKAGES);
	if (local_packages_table == (OMP_package * ) NULL )	 {
		OM_FREE(workspace);
		return(OM_MEMORY_INSUFFICIENT);
	} /* endif */
	workspace->packages_table = local_packages_table ;

	/* Initialise packages and feature tables */
	for	(i = 0 ; i < NBMAXPACKAGES ; i++) {
		workspace->packages_table[i].feature.length	 = 0;
		workspace->packages_table[i].activated = OMP_BASIC;
		workspace->feature[i] = OM_FALSE;
	} /* endfor */
	/* Add packages	*/
	for ( i = 0; packages_table[i].feature.length != 0; i++) {
		ret = omP_add_package (workspace, packages_table[i] );
		if ( ret != OM_SUCCESS) {
			(void) omP_free_package_table (workspace);
			OM_FREE(workspace);
			return(ret);
		} /* endif */
	} /* endfor */

	workspace->closure_packages_table = closure_packages_table;
	workspace->generic_workspace = generic_workspace;
	workspace->alloc_table = alloc_table;

	workspace->om_private_object_list = NULL;
	/*---------------------------------------------------------------------------*/
	/* Initialise the list of the SPUB of the workspace					 */
	/*---------------------------------------------------------------------------*/
	workspace->om_spub_object_list = NULL;

	workspace->status = WORKSPACE_INIT;
	if ((ret = omP_init_switch(workspace, default_session))	 == OM_SUCCESS)
		*workspace_return = workspace;

	return(ret);
} /* end omP_xinitialize */


/****************************************************************/
/*								*/
/*	FUNCTION: omP_free_workspace()				*/
/*								*/
/*	SHORT DESCRIPTION:						*/
/*	 omP_free_workspace	free memory	which is used by	*/
/*	 the workspace and the memory management				*/
/*	 The only kept space is	the	switch_function_table		*/
/*	 which was first allocated.								*/
/*								*/
/*	INPUT-PARAMETER:						*/
/*								*/
/*	OUTPUT-PARAMETER:						*/
/*								*/
/*	RETURN-VALUE:						*/
/*								*/
/****************************************************************/
void
omP_free_workspace(
	OM_workspace	workspace)
{
	OMX_workspace	wk_ptr;						/* workspace handle	*/
	OMP_mutex	 * mutex_save;

	wk_ptr = (OMX_workspace) workspace;

	/* Remove package table	*/
	(void) omP_free_package_table(workspace);

	/*
 * DON'T REMOVE	THE	WORKSPACE
 *	OM_FREE(wk_ptr);
 */
	/* save	and	restore	the	mutex pointer, if any */
	mutex_save = wk_ptr->mutex_wk;
	/* initialise workspace	with invalid workspace */
	memcpy(wk_ptr, OMX_invalid_workspace, sizeof(struct	OMX_workspace_body ));
	wk_ptr->mutex_wk = mutex_save;
} /* end omP_free_workspace	*/


/****************************************************************/
/*								*/
/*	FUNCTION: omP_shutdown()					*/
/*								*/
/*	SHORT DESCRIPTION:						*/
/*	 omP_shutdown disables the status flag in the workspace	*/
/*		structure and deletes all the existing private	*/
/*		objects						*/
/*		This function does not delete the spub objects	*/
/*								*/
/*	INPUT-PARAMETER:						*/
/*								*/
/*	OUTPUT-PARAMETER:						*/
/*								*/
/*	RETURN-VALUE:						*/
/*								*/
/****************************************************************/
void
omP_xshutdown(
	OM_workspace	workspace)
{
	OM_return_code	ret;
	register OMP_private_object	*subject;

	/* delete all the private objects				*/
	/* om_private_object_list is the header	private	object list	*/
	/* omP_delete updates this list	with the next object		*/

	while ((subject = ((OMX_workspace)workspace)->om_private_object_list) != 0) {
		if ((ret = omP_delete(OMP_EXTERNAL(subject))) !=  OM_SUCCESS ) {
			if (omP_trace_opt &	OMP_TRACE_FATAL_ERROR) {
				fprintf(omP_trace_file,	"XOM COMMON	(omP_shutdown) : Delete private object error ret = %d\n", ret);
				fprintf(omP_trace_file,	"   FATAL ERROR\n");
				OMP_ASSERT(0) ;
			} /* endif */
		} /* endif */
	} /* endwhile */

	/* disable the workspace  */
	((OMX_workspace)workspace)->status = WORKSPACE_SHUT;

	if (((OMX_workspace)workspace)->om_spub_object_list	 == NULL)
		omP_free_workspace((OM_workspace)workspace);
} /* end omP_xshutdown */


/****************************************************************/
/*								*/
/*	FUNCTION: omP_complete_shutdown()				*/
/*								*/
/*	SHORT DESCRIPTION:						*/
/*	 omP_complete_shutdown free	memory which is	used by		*/
/*	 the workspace and the memory management				*/
/*	 We	DO NOT CARE	about handle used by the user :			*/
/*	 when this function	is called any handle to	public or	*/
/*	 private object	are	destroyed. If the user used	one	of	*/
/*	 them after	the	function called, the effect	is			*/
/*	 UNSPECIFIED											*/
/*								*/
/*	INPUT-PARAMETER:						*/
/*								*/
/*	OUTPUT-PARAMETER:						*/
/*								*/
/*	RETURN-VALUE:						*/
/*								*/
/****************************************************************/
void
omP_xcomplete_shutdown(
	OM_workspace	workspace)
{
	OMX_workspace					wk_ptr ;			/* workspace handle	*/
	register OMP_private_object		*subject;
	register OM_descriptor			*spub;
	OM_return_code					ret;

	wk_ptr = (OMX_workspace) workspace ;

	/*-----------------------------------------------------------------------*/
	/* Delete the spub and the private objects								 */
	/*-----------------------------------------------------------------------*/
	/* delete all the spub objects				*/
	/* om_spub_object_list is the header of	the	spub object	list	*/
	/* omP_delete removes the object from the list		*/

	while ((spub = ((OMX_workspace)workspace)->om_spub_object_list)	!= 0) {
		if ((ret = omP_delete(OMP_EXTERNAL(spub))) != OM_SUCCESS ) {
			if (omP_trace_opt &		OMP_TRACE_FATAL_ERROR) {
				fprintf(omP_trace_file,		"XOM COMMON	(omP_shutdown) : Delete	private	object error ret = %d\n", ret);
				fprintf(omP_trace_file,		"			FATAL ERROR\n")	;
				OMP_ASSERT(0) ;
			} /* endif */
		} /* endif */
	} /* endwhile */


	/* delete all the private objects				*/
	/* om_private_object_list is the header	private	object list	*/
	/* omP_delete updates this list	with the next object		*/

	while ((subject=((OMX_workspace)workspace)->om_private_object_list) != 0) {
		if ((ret = omP_delete(OMP_EXTERNAL(subject))) !=  OM_SUCCESS ) {
			if (omP_trace_opt &		OMP_TRACE_FATAL_ERROR) {
				fprintf(omP_trace_file,"XOM COMMON (omP_shutdown) : Delete private object error ret = %d\n", ret);
				fprintf(omP_trace_file,	"			FATAL ERROR\n")	;
				OMP_ASSERT(0) ;
			} /* endif */
		} /* endif */
	} /* endwhile */

	/*
* Remove package table
*/
	(void) omP_free_package_table(workspace) ;

	/*
* Remove the workspace
*/
	OM_FREE(wk_ptr)	;

} /* end omP_xcomplete_shutdown	*/


/**********************************************************************/
/*																	  */
/* FUNCTION: omP_X_find_package										  */
/*																	  */
/* PURPOSE:	 This function look	for	a package in internal structure	  */
/*																	  */
/* INPUT:															  */
/*			OMX_workspace workspace	: The workspace					  */
/*			OM_object_identifiuer package :	the	package	oid			  */
/*																	  */
/* INPUT/OUTPUT:													  */
/*																	  */
/* OUTPUT:															  */
/*			OM_boolean * found : Indicate if the package is	found	  */
/*			OM_sint	*		 packno: The index of the package			  */
/*																	  */
/* RETURN:	void													  */
/*																	  */
/* NOTE:															  */
/*																	  */
/**********************************************************************/
static void
omP_X_find_package (
OMX_workspace		workspace,
	OM_object_identifier	package	,
	OM_boolean		*found ,
	OM_sint	*packno)
{
	OMP_package	 * package_ptr ;

	for ((*packno) = 0 , (*found) = OM_FALSE ,
	package_ptr =  workspace->packages_table; (*packno) <	NBMAXPACKAGES ;
	(*packno)++, package_ptr++) {
		/*
			* Here we suppose that the length of the string
			* could	not	be OM_LENGTH_UNSPECIFIED :
			* An OID can contain a null	member
			* e.g 1.3.0.1
			*/
		if (package.length == package_ptr->feature.length) {
			if (!memcmp(	package.elements, package_ptr->feature.elements,
				 package.length)) {
				(*found) = OM_TRUE;
				break;
			} /* endif */
		} /* endif */
	} /* endfor */

} /* end omP_X_find_package	*/


/**********************************************************************/
/*																	  */
/* FUNCTION: omP_negotiate											  */
/*																	  */
/* PURPOSE:	 Negotiation of	the	feature								  */
/*																	  */
/* INPUT:															  */
/*			OM_workspace workspace : The workspace					  */
/*																	  */
/* INPUT/OUTPUT:													  */
/*			OM_negotiate_feature  feature_list : the list of feature  */
/*																	  */
/* OUTPUT:															  */
/*			NONE													  */
/*																	  */
/* RETURN:															  */
/*			OM_SUCCESS												  */
/*			OM_POINTER_INVALID										  */
/*			OM_MEMORY_INSUFFICIENT									  */
/*			OM_WRONG_VALUE_TYPE	  :	The	"request" input	parameter has */
/*									is not correct					  */
/*																	  */
/* NOTE:															  */
/*																	  */
/**********************************************************************/
OM_return_code
omP_xnegotiate (
	OM_workspace			workspace ,
	OMP_negotiate_feature	feature_list[])
{
	OMP_negotiate_feature	*feature_element ;
	OMX_workspace			work_ptr ;
	OM_sint					packno ;
	OM_boolean				found ;


	/*
* we fisrt verify input	parameters
*/
	if ( workspace == (OM_workspace) NULL) {
		return(OM_POINTER_INVALID) ;
	} /* endif */

	if ( feature_list == (OMP_negotiate_feature	 * )	NULL ) {
		return(OM_POINTER_INVALID) ;
	} /* endif */

	/*
* Now, we can performe the request
*/
	work_ptr = (OMX_workspace) workspace ;

	/*
		* Here we suppose that the length of the string
		* could	not	be OM_LENGTH_UNSPECIFIED :
		* An OID can contain a null	member
		* e.g 1.3.0.1
		*/
	for	(feature_element = feature_list; feature_element->feature.length != 0 ;
	feature_element++) {
		/*
		* NOTE : The default case is an	error :	The	input parameter	is
		* not a	good one
		*/
		switch (feature_element->request) {
		case OMP_ACTIVATED :
			/*
			* Look for the package
			*/
			omP_X_find_package (work_ptr, feature_element->feature,
				 &found, &packno) ;

			if ( found == OM_TRUE) {
				/*
				* We find the package
				* Now, we have to look if the package has
				* been already activated
				* or not
				*/
				if (work_ptr->feature[packno] ==  OM_TRUE) {
					/*
					* package already activated:
					* don't	do anything
					*/
				} else {
					/*
					* Activate the package
					*/
					work_ptr->feature[packno] = OM_TRUE	;
				} /* endif */

				/*
				* We find the package and activate
				* set the return parameter
				*/
				feature_element->response = OM_TRUE	;
			} else {
				/*
				* We don't find	the	package
				*/
				feature_element->response = OM_FALSE;
			} /* endif */
			break ;
		case OMP_DEACTIVATED :
			/*
			* Look for the package
			*/
			omP_X_find_package (work_ptr, feature_element->feature,
				 &found, &packno) ;

			if ( found == OM_TRUE) {
				/*
				* We find the package
				* Now, we desactivate the package
				*/
				if (work_ptr->feature[packno] ==  OM_TRUE) {
					/*
				* package activated:
				* we have to desactivate it
				*/
					if (work_ptr->packages_table[packno].activated ==
						OMP_DEFAULT) {
						feature_element->response = OM_FALSE ;
					} else {
						if ( work_ptr->packages_table[packno].activated	 ==
							OMP_ADDED) {
							OM_FREE(work_ptr->packages_table[packno].
											feature.elements) ;
							work_ptr->packages_table[packno].feature.length =0;
							work_ptr->packages_table[packno].feature.elements =
							NULL ;
						} /* endif */
						work_ptr->feature[packno] = OM_FALSE ;
						feature_element->response = OM_TRUE;
					} /* endif */
				} else {
					/*
				* The package is already desactivaed
				*/
					feature_element->response = OM_TRUE	;
				} /* endif */
			} else {
				/*
				* We don't find	the	package
				*/
				feature_element->response = OM_FALSE;
			} /* endif */
			break ;
		case OMP_QUERY_STATE :
			/*
			* Look for the package
			*/
			omP_X_find_package (work_ptr, feature_element->feature ,
				 &found, &packno) ;

			if ( found == OM_TRUE) {
				/*
				* We find the package
				* Now, we have to look if the package has
				* been already activated
				* or not
				*/
				if (work_ptr->feature[packno] ==  OM_TRUE) {
					/*
					* package activated
					*/
					feature_element->response = OM_TRUE	;
				} else {
					/*
					* package not activated
					*/
					feature_element->response = OM_FALSE ;
				} /* endif */
			} else {
				/*
				* We don't find	the	package
				*/
				feature_element->response = OM_FALSE;
			} /* endif */
			break ;
		case OMP_QUERY_SUPPORTED :
			/*
			* Look for the package
			*/
			omP_X_find_package (work_ptr, feature_element->feature ,
				 &found, &packno) ;

			if ( found == OM_TRUE) {
				/*
				* We find the package, so it is	supported
				*/
				feature_element->response = OM_TRUE	;
			} else {
				/*
				* We don't find	the	package:
				* so it	is not supported
				*/
				feature_element->response = OM_FALSE;
			} /* endif */
			break ;
		default	:
			feature_element->request  = OMP_INVALID	;
			feature_element->response = OM_FALSE ;
			break ;
		} /* endswitch */
	} /* endfor */

	return(OM_SUCCESS) ;
} /* end omP_xnegotiate	 */


/****************************************************************/
/*								*/
/*	FUNCTION: omP_version()					*/
/*								*/
/*	SHORT DESCRIPTION:						*/
/*		omP_version	is called by ds_version()			*/
/*								*/
/*	 omP_version checks	the	validity of	the	features	*/
/*			(Package identifier	) and updates the features	*/
/*			in the workspace structure			*/
/*								*/
/*								*/
/*	INPUT-PARAMETER:						*/
/*	feature_list (elements)	 = package ids	to negotiate	*/
/*								*/
/*	OUTPUT-PARAMETER:						*/
/*	feature_list (activated) = TRUE	if activated		*/
/*								*/
/*	RETURN-value:						*/
/*	OM_PERMANENT_ERROR		max	number of packages reached	*/
/*	OM_POINTER_INVALID		feature_list pointer invalid	*/
/*	OM_POINTER_INVALID		workspace pointer invalid		*/
/*	OM_SUCCESS		omP_version	successful		*/
/*								*/
/****************************************************************/


OM_return_code
omP_xversion(
	OM_workspace	workspace,
	OMP_feature	feature_list[])
{
	OM_return_code		om_return_code ;
	OMP_negotiate_feature	 * feature_nego	;
	OM_sint	nb_feature ;

	/*
* Calculate	the	number of features
*/
	/*
		* Here we suppose that the length of the string
		* could	not	be OM_LENGTH_UNSPECIFIED :
		* An OID can contain a null	member
		* e.g 1.3.0.1
		*/
	for	(nb_feature = 0 ; feature_list[nb_feature].feature.length != 0 ;
	nb_feature++) {
		;
	} /* endfor */

	if ( nb_feature	 == 0 )	{
		return(OM_SUCCESS) ;
	} /* endif */

	/*
* Alloc	memory for the new feature list
*/
	feature_nego = (OMP_negotiate_feature *		)
	OM_MALLOC(sizeof(OMP_negotiate_feature)	 * (nb_feature +  1));
	if ( feature_nego == (OMP_negotiate_feature	 * )	NULL) {
		return(OM_MEMORY_INSUFFICIENT) ;
	} /* endif */

	/*
* We translate the feature information
* in order to call omP_negotiate
*/

	for	(nb_feature = 0 ; feature_list[nb_feature].feature.length != 0 ;
	nb_feature++) {
		feature_nego[nb_feature].feature.length =
		feature_list[nb_feature].feature.length;
		feature_nego[nb_feature].feature.elements =
		feature_list[nb_feature].feature.elements;
		feature_nego[nb_feature].request = OMP_ACTIVATED ;
	} /* endfor */

	/*
	* Initialize the last feature
	*/
	feature_nego[nb_feature].feature.length	  = 0 ;
	feature_nego[nb_feature].feature.elements = NULL ;

	/*
	* Class	omP_negotiate
	*/
	om_return_code	 = omP_negotiate	( workspace	, feature_nego ) ;
	if (om_return_code != OM_SUCCESS) {
		OM_FREE(feature_nego) ;
		return(om_return_code) ;
	} /* endif */

   /*
	* Translate	the	result into	feature_list parameter
	*/
	for (nb_feature = 0 ; feature_list[nb_feature].feature.length != 0 ;
	nb_feature++) {
		feature_list[nb_feature].activated =
		feature_nego[nb_feature].response ;
	} /* endfor */

	OM_FREE(feature_nego) ;
	return(OM_SUCCESS) ;
} /* end omP_xversion */


