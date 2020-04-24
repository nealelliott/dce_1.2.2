/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPCreate.c,v $
 * Revision 1.1.6.2  1996/02/18  18:24:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:16  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:32:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:28  root]
 * 
 * Revision 1.1.4.5  1994/07/06  15:08:55  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:23:32  marrek]
 * 
 * Revision 1.1.4.4  1994/06/21  14:48:40  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:47:32  marrek]
 * 
 * Revision 1.1.4.3  1994/05/10  16:03:46  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:13:49  marrek]
 * 
 * Revision 1.1.4.2  1994/03/23  15:48:33  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:35:43  keutel]
 * 
 * Revision 1.1.4.1  1994/02/22  18:09:59  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:19:56  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:58:26  marrek]
 * 
 * Revision 1.1.2.2  1993/08/10  07:01:29  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:13:11  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:16:22  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:04:35  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:31:34  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:40:55  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:17:51  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:45:27  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsPCreate.c,v $ $Revision: 1.1.6.2 $ $Date: 1996/02/18 18:24:34 $";
#endif

/***************************************************************
*															   *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG	1991   *
*				 ALL RIGHTS	RESERVED						   *
*															   *
***************************************************************/

/* ****************************************************************** */
/*																	  */
/*	 * COPYRIGHT   (C)	1990 BY	BULL S.A					  */
/*	 * All rights reserved											  */
/*	 *																  */
/*	 * This	software is	furnished under	licence	and	may	be used	only  */
/*	 * in accordance with the terms	of that	licence	and	with the	  */
/*	 * inclusion of	the	above copyright	notice.						  */
/*	 * This	software may not be	provided or	otherwise made available  */
/*	 * to, or used by, any other person. No	title to or	ownership of  */
/*	 * the software	is hereby transferred.							  */
/*																	  */
/* ****************************************************************** */
/*	@(#)dsPCreate.c	1.23 (BULL S.A)	7/1/92	*/

/****************************************************************/
/*								*/
/*	Function :	dsP_create_switch()				*/
/*								*/
/****************************************************************/
/*								*/
/*	dsP_create_switch()	calls the function associated to the	*/
/*			object type								*/
/*								*/
/*	Each create_xxx() function allocates the corresponding	*/
/*			  XDS structure							*/
/*			  and initialise it	if necessary		*/
/*								*/
/*	The	object handle has been allocated					*/
/*								*/
/*	Allocate XDS object	according to the input class		*/
/*	parameter												*/
/*								*/
/*	Initialise,	if needed, initial values of the XDS object	*/
/*	parameter. For that	purpose, for each object that may	*/
/*	have initial values, a list	of descriptors is furnished	*/
/*	with these initial values, and the om_put function is	*/
/*	called with	this descriptor	list as	public object source*/
/*	argument.												*/
/*								*/
/*	Each function or subroutine	which needs	memory			*/
/*	allocation may return memory-insufficient code			*/
/*								*/
/*	RETURN_CODE:						*/
/*	OM_SUCCESS												*/
/*	OM_FUNCTION_DECLINED									*/
/*	OM_MEMORY_INSUFFICIENT									*/
/*	OM_NO_SUCH_CLASS										*/
/*	OM_NOT_CONCRETE											*/
/*								*/
/****************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <xom.h>
#include <d2dir.h>
#include <xdsiapl.h>
#include <xds.h>
#include <xomip.h>
#include <xdsgds.h>
#include <xdsimport.h>
#include <xdspackage.h>
#include <xdssap.h>
#include <alloc.h>
#include <xdsalloc.h>
#include <dsPPsap.h> /* for dsP_get_psap_length () */
#include <dsPDelete.h> /* for dsP_del_session() */
#include <dsPCreate.h>

extern xds_session	*default_xds_session;
extern OMP_mutex	mutex_session;

/************************************************************/
/*		static prototypes 									*/
/************************************************************/
static OM_return_code
init_default_session(
	OM_private_object destination);


static OM_descriptor	ext_init[] = {
	{ DS_CRIT, OM_S_BOOLEAN, OM_FALSE },
	OM_NULL_DESCRIPTOR
};


#if 0
static	OM_descriptor	op_init[] =	{
		OMP_OID_DESC(OM_CLASS,DS_C_OPERATION_PROGRESS),
		{DS_NAME_RESOLUTION_PHASE, OM_S_ENUMERATION,DS_NOT_STARTED},
		OM_NULL_DESCRIPTOR
		};
#endif /* 0 */
static OM_descriptor	ctx_init[] = {
	{ DS_OPERATION_PROGRESS, OM_S_OBJECT, {	0, DS_OPERATION_NOT_STARTED	 } },
	{ DS_ALIASED_RDNS, OM_S_INTEGER, 0 },
	{ DS_CHAINING_PROHIB, OM_S_BOOLEAN,		OM_TRUE	 },
	{ DS_DONT_USE_COPY,		OM_S_BOOLEAN, OM_TRUE },
	{ DS_PRIORITY, OM_S_ENUMERATION, DS_MEDIUM },
/*	{DS_SCOPE_OF_REFERRAL,OM_S_ENUMERATION,DS_WORLD_SCOPE},	*/
	{ DS_SIZE_LIMIT, OM_S_INTEGER, (OM_uint32)D2_S_UNLIMITED },
	{ DS_TIME_LIMIT, OM_S_INTEGER, (OM_uint32)D2_T_UNLIMITED },
	{ DS_AUTOMATIC_CONTINUATION, OM_S_BOOLEAN, OM_TRUE },
	{ DSX_DONT_STORE, OM_S_BOOLEAN,		OM_TRUE	 },
	{ DSX_USEDSA, OM_S_BOOLEAN,		OM_TRUE	 },
	OM_NULL_DESCRIPTOR
};


static OM_descriptor	eis_init[] = {
	{ DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_TRUE },
	{ DS_INFO_TYPE,		OM_S_ENUMERATION, DS_TYPES_AND_VALUES },
	OM_NULL_DESCRIPTOR
};


static OM_descriptor	em_init[] =		{
	{ DS_MOD_TYPE, OM_S_ENUMERATION, DS_ADD_ATTRIBUTE },
	OM_NULL_DESCRIPTOR
};


static OM_descriptor	fi_init[] =		{
	{ DS_FILTER_TYPE, OM_S_ENUMERATION,		DS_AND },
	OM_NULL_DESCRIPTOR
};


static OM_descriptor	sr_crit_init[] = {
	{ DS_FILTER_TYPE, OM_S_ENUMERATION,		DS_ITEM	 },
	OM_NULL_DESCRIPTOR
};

static OM_descriptor    cert_init[] = {
    { DS_VERSION,OM_S_ENUMERATION,DS_V1988},
    OM_NULL_DESCRIPTOR
};


OMP_object_type	ses_obj_type = {
	XDS_GDS_PACKAGE,
	XDS_GDS_SESSION};


OMP_object_type	ctx_obj_type = {
	XDS_GDS_PACKAGE,
	XDS_GDS_CONTEXT};


/********************************************************/
/*							*/
/*	init_default session called by omP_initialise()		*/
/*		initialize the pointer to the		*/
/*		initial	values of the session object	*/
/*							*/
/********************************************************/
static OM_return_code
init_default_session(
	OM_private_object destination)
{
	OMX_workspace workspace	 = (OMX_workspace)OMP_WORKSPACE(destination);
	OM_return_code	ret	 = OM_SUCCESS;
	xds_session	 * xds_object =		0;
	OM_boolean	while_flag = OM_TRUE;
	MUTEX_STATE		/* state of	thread asynchronous	cancelability */

	if (!(xds_object = (void *)OMP_XALLOC(workspace, AL_XDS_SESSION, 1)))
		return(OM_MEMORY_INSUFFICIENT);
	memset(xds_object, 0, sizeof(xds_session));

	/* lock	the	mutex on default_xds_session */
	MUTEX_LOCK(&mutex_session, "create session");

	while (	while_flag && ret == OM_SUCCESS	) {
		if (default_xds_session->dsa_name) {
			if (!( xds_object->dsa_name = (void *)OMP_XALLOC_STR(workspace,
			(char *)(default_xds_session->dsa_name),
			strlen((char *)default_xds_session->dsa_name)) )) {
				ret	 = OM_MEMORY_INSUFFICIENT;
				break;
			} /* endif */
		} /* endif */

		if (default_xds_session->dsa_address) {
			if (!( xds_object->dsa_address = (void *)OMP_XALLOC_STR(workspace,
			(char *)(default_xds_session->dsa_address),
			dsP_get_psap_length(default_xds_session->dsa_address)))) {
				ret	 = OM_MEMORY_INSUFFICIENT;
				break;
			} /* endif */
		} /* endif */

		if (default_xds_session->requestor) {
			if (!( xds_object->requestor = (void *)OMP_XALLOC_STR(workspace,
			(char *)(default_xds_session->requestor),
			strlen((char *)default_xds_session->requestor)) )) {
				ret	 = OM_MEMORY_INSUFFICIENT;
				break;
			} /* endif */
		} /* endif */

		if ( (default_xds_session->password.d2_p_pw) 
		&& (OMP_PACKAGE(OMP_OBJ_TYPE(destination)) == XDS_GDS_PACKAGE)) {
			if (!( xds_object->password.d2_p_pw =
			(void *)OMP_XALLOC_STR(workspace,
			(char *)(default_xds_session->password.d2_p_pw),
			default_xds_session->password.d2_p_len) ))	 {
				ret	 = OM_MEMORY_INSUFFICIENT;
				break;
			} /* endif */
		} /* endif */

		xds_object->password.d2_p_len = default_xds_session->password.d2_p_len;

		xds_object->dir_id = default_xds_session->dir_id;
		xds_object->auth_mech = default_xds_session->auth_mech;

		if ((default_xds_session->auth_info.d2_value)
		&& (OMP_PACKAGE(OMP_OBJ_TYPE(destination)) == XDS_GDS_PACKAGE)) {
			if (!(xds_object->auth_info.d2_value =
			(void *)OMP_XALLOC_STR( workspace,
			(char *)(default_xds_session->auth_info.d2_value),
			default_xds_session->auth_info.d2_size))) {
				ret	 = OM_MEMORY_INSUFFICIENT;
				break;
			} /* endif */
		} /* endif */


		/* fill	the	handle->priv_object	pointer	with the 
		xds_session structure * address */
		((OMP_private_object * )OMP_INTERNAL(destination))->priv_object	 =
			xds_object;
		while_flag = OM_FALSE;
	} /* endwhile */

	/* unlock the mutex	on default_xds_session */
	MUTEX_UNLOCK(&mutex_session,"create	session");

	if (ret != OM_SUCCESS)
		dsP_del_session(workspace, xds_object);

	return(ret);
} /* end init_default_session */


OM_return_code
dsP_create_switch (
	const OMP_object_type	class_sub_id,
	OM_private_object		destination,
	const OM_boolean		initialise)
{
	OM_return_code	ret	 = OM_SUCCESS;
	OM_object	initial_values = 0;
	OM_boolean	change_package = OM_FALSE;

	if ((!OMP_CHECK_CLOSURE(OMP_PACKAGE(class_sub_id))) && (!omP_check_negotiated_package(OMP_WORKSPACE(destination), OMP_PACKAGE(class_sub_id))))
		return(OM_NO_SUCH_CLASS);

	switch (OMP_PACKAGE(class_sub_id)) {
	case XDS_SERVICE_PACKAGE:

		switch (OMP_CLASS(class_sub_id)) {
		case XDS_PRESENTATION_ADDRESS:
		case XDS_DS_RDN:
		case XDS_DS_DN:
		case XDS_AVA:
		case XDS_ATTRIBUTE:
		case XDS_ATTRIBUTE_LIST:
		case XDS_ENTRY_INFO:
		case XDS_ENTRY_MOD_LIST:
		case XDS_FILTER_ITEM:
			break;
		case XDS_EXT:
			initial_values = &ext_init[0];
			break;
		case XDS_CONTEXT:
			change_package = OM_TRUE;
			OMP_OBJ_TYPE(destination) =		ctx_obj_type;
			initial_values = &ctx_init[0];
			break;
		case XDS_ENTRY_INFO_SELECTION:
			initial_values = &eis_init[0];
			break;
		case XDS_ENTRY_MOD:
			initial_values = &em_init[0];
			break;
		case XDS_FILTER:
			initial_values = &fi_init[0];
			break;
		case XDS_SESSION:
			if (initialise)
				ret	 = init_default_session(destination);
			break;

			/* abstract	object classes */
		case XDS_ADDRESS :
		case XDS_ERROR:
		case XDS_COMMON_RESULTS:
		case XDS_NAME :
		case XDS_RELATIVE_NAME :
			ret	 = (OM_NOT_CONCRETE);
			break;

		case XDS_ABANDON_FAILED	:
		case XDS_ACCESS_POINT :
		case XDS_ATTRIBUTE_PROBLEM :
		case XDS_COMPARE_RESULT:
		case XDS_CONTINUATION_REF:
		case XDS_LIST_INFO_ITEM:
		case XDS_LIST_INFO:
		case XDS_LIST_RESULT:
		case XDS_OPERATION_PROGRESS:
		case XDS_PARTIAL_OUTCOME_QUAL:
		case XDS_READ_RESULT:
		case XDS_REFERRAL :
		case XDS_SEARCH_INFO:
		case XDS_SEARCH_RESULT:
		case XDS_ATTRIBUTE_ERROR:
		case XDS_COMMUNICATIONS_ERROR:
		case XDS_SECURITY_ERROR:
		case XDS_NAME_ERROR:
		case XDS_LIBRARY_ERROR:
		case XDS_SERVICE_ERROR:
		case XDS_SYSTEM_ERROR:
		case XDS_UPDATE_ERROR:
			ret	 = (OM_FUNCTION_DECLINED);
			break;

		default:
			ret	 = (OM_NO_SUCH_CLASS);
			break;
		} /* endswitch */
		break;

	case XDS_GDS_PACKAGE:

		switch (OMP_CLASS(class_sub_id)) {
		case XDS_GDS_SESSION:
			if (initialise)
				ret	 = init_default_session(destination);
			break;
		case XDS_GDS_CONTEXT:
			initial_values = &ctx_init[0];
			break;
		case XDS_GDS_ACL:
		case XDS_GDS_ACL_ITEM:
			break;
		default:
			ret	 = (OM_NO_SUCH_CLASS);
			break;
		} /* endswitch */
		break;

	case XDS_BDCP:

		switch (OMP_CLASS(class_sub_id)) {
		case XDS_SEARCH_CRITERION:
			initial_values = &sr_crit_init[0];
			break;
		case XDS_FACSIMILE_PHONE_NBR:
		case XDS_POSTAL_ADDRESS:
		case XDS_SEARCH_GUIDE:
		case XDS_TELETEX_TERM_IDENT:
		case XDS_TELEX_NBR:
			break;
		default:
			ret	 = (OM_NO_SUCH_CLASS);
			break;
		} /* endswitch */
		break;
		CASE_CREATE_MDUP_PACKAGE 
#if 0
	case XDS_MDUP:
	switch (OMP_CLASS(class_sub_id)) {
		case MHS_DLSP:
		break;
		default:
		ret	= (OM_NO_SUCH_CLASS);
		break;
	} /* endswitch */
	break;
	case XMH_PACKAGE:
	switch (OMP_CLASS(class_sub_id)) {
		case XMH_OR_NAME:
			break;
		case XMH_OR_ADDRESS:
			ret	= (OM_NOT_CONCRETE);
			break;
		default:
			ret	= (OM_NO_SUCH_CLASS);
			break;
	} /* endswitch */
	break;
#endif /* 0 */
    case XDS_SAP_PACKAGE:
    switch (OMP_CLASS(class_sub_id)) {
        case XDS_CERTIFICATE:
            initial_values = &cert_init[0];
            break;
        case XDS_ALGORITHM_IDENT:
        case XDS_CERTIFICATE_LIST:
        case XDS_CERTIFICATE_PAIR:
        case XDS_CERTIFICATE_SUBLIST:
        case XDS_SIGNATURE:
            break;
        default:
           ret  = (OM_NO_SUCH_CLASS);
           break;
    } /* endswitch */
    break;

	default:
		ret	 = (OM_NO_SUCH_CLASS);
	} /* endswitch */
	if ((ret == OM_SUCCESS) && ((initialise) && (initial_values))) {
		ret = omP_put(destination,	OM_REPLACE_ALL,	initial_values, 0, 0, 0);
	} /* endif */
	if (change_package)
		OMP_OBJ_TYPE(destination) =	class_sub_id;
	return (ret);
} /* end dsP_create_switch */


/****************************************************************/
/*								*/
/*	Function :	dsP_cr_handle_switch()			*/
/*								*/
/****************************************************************/
/*								*/
/*	dsP_cr_handle_switch() checks the object type validity		*/
/*			 (package and class)					*/
/*								*/
/*	The	handle will	be allocated if	the	returned code is	*/
/*	OM_SUCCESS							*/
/*								*/
/*	RETURN_CODE:						*/
/*	OM_SUCCESS												*/
/*	OM_NO_SUCH_CLASS										*/
/*	OM_NOT_CONCRETE											*/
/*								*/
/****************************************************************/

OM_return_code
dsP_cr_handle_switch (
	const OMP_object_type	class_sub_id)
{
	switch (OMP_PACKAGE(class_sub_id)) {
	case XDS_SERVICE_PACKAGE:
		switch (OMP_CLASS(class_sub_id)) {
		case XDS_PRESENTATION_ADDRESS:
		case XDS_DS_RDN:
		case XDS_DS_DN:
		case XDS_AVA:
		case XDS_ATTRIBUTE:
		case XDS_ATTRIBUTE_LIST:
		case XDS_EXT:
		case XDS_CONTEXT:
		case XDS_ENTRY_INFO:
		case XDS_ENTRY_INFO_SELECTION:
		case XDS_ENTRY_MOD:
		case XDS_ENTRY_MOD_LIST:
		case XDS_FILTER:
		case XDS_FILTER_ITEM:
		case XDS_SESSION:
			break;

		case XDS_ABANDON_FAILED	:
		case XDS_ACCESS_POINT :
		case XDS_ATTRIBUTE_PROBLEM :
		case XDS_COMPARE_RESULT:
		case XDS_CONTINUATION_REF:
		case XDS_LIST_INFO_ITEM:
		case XDS_LIST_INFO:
		case XDS_LIST_RESULT:
		case XDS_OPERATION_PROGRESS:
		case XDS_PARTIAL_OUTCOME_QUAL:
		case XDS_READ_RESULT:
		case XDS_REFERRAL :
		case XDS_SEARCH_INFO:
		case XDS_SEARCH_RESULT:
		case XDS_ATTRIBUTE_ERROR:
		case XDS_COMMUNICATIONS_ERROR:
		case XDS_SECURITY_ERROR:
		case XDS_NAME_ERROR:
		case XDS_LIBRARY_ERROR:
		case XDS_SERVICE_ERROR:
		case XDS_SYSTEM_ERROR:
		case XDS_UPDATE_ERROR:
			break;

			/* abstract	object classes */
		case XDS_ADDRESS :
		case XDS_ERROR:
		case XDS_COMMON_RESULTS:
		case XDS_NAME :
		case XDS_RELATIVE_NAME :
			return (OM_NOT_CONCRETE);
			break;

		default:
			return (OM_NO_SUCH_CLASS);
		} /* endswitch */
		break;

	case XDS_GDS_PACKAGE:

		switch (OMP_CLASS(class_sub_id)) {
		case XDS_GDS_SESSION:
		case XDS_GDS_CONTEXT:
		case XDS_GDS_ACL:
		case XDS_GDS_ACL_ITEM:
			break;
		default:
			return (OM_NO_SUCH_CLASS);
		} /* endswitch */
		break;

	case XDS_BDCP:

		switch (OMP_CLASS(class_sub_id)) {
		case XDS_SEARCH_CRITERION:
		case XDS_FACSIMILE_PHONE_NBR:
		case XDS_POSTAL_ADDRESS:
		case XDS_SEARCH_GUIDE:
		case XDS_TELETEX_TERM_IDENT:
		case XDS_TELEX_NBR:
			break;
		default:
			return(OM_NO_SUCH_CLASS);
		} /* endswitch */
		break;
		CASE_CR_HANDLE_MDUP_PACKAGE	
#if 0
	case XDS_MDUP:
	switch (OMP_CLASS(class_sub_id)) {
		case MHS_DLSP:
			break;
		default:
			return (OM_NO_SUCH_CLASS);
			break;
	} /* endswitch */
	break;
	case XMH_PACKAGE:
	switch (OMP_CLASS(class_sub_id)) {
		case XMH_OR_NAME:
			break;
		default:
			return (OM_NO_SUCH_CLASS);
			break;
	} /* endswitch */
	break;
#endif /* 0 */
    case XDS_SAP_PACKAGE:
        switch (OMP_CLASS(class_sub_id)) {
        case XDS_CERTIFICATE:
		case XDS_ALGORITHM_IDENT:
		case XDS_CERTIFICATE_LIST:
		case XDS_CERTIFICATE_PAIR:
		case XDS_CERTIFICATE_SUBLIST:
		case XDS_SIGNATURE:
           break;
		default:
           return  (OM_NO_SUCH_CLASS);
           break;
       } /* endswitch */
       break;

	default:
		return (OM_NO_SUCH_CLASS);
	} /* endswitch */

	return (OM_SUCCESS);
} /* end dsP_cr_handle_switch  */
