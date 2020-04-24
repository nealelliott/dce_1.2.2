/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPGet.c,v $
 * Revision 1.1.43.2  1996/02/18  18:24:39  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:19  marty]
 *
 * Revision 1.1.43.1  1995/12/08  15:32:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:31  root]
 * 
 * Revision 1.1.41.7  1994/08/24  08:35:37  marrek
 * 	Fix warnings. (OT 11824)
 * 	[1994/08/23  14:36:35  marrek]
 * 
 * Revision 1.1.41.6  1994/08/10  08:25:39  marrek
 * 	Fix for SAP errors, OT11467.
 * 	[1994/08/08  14:15:12  marrek]
 * 
 * Revision 1.1.41.5  1994/07/06  15:08:58  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:24:08  marrek]
 * 
 * Revision 1.1.41.4  1994/06/21  14:48:43  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:48:08  marrek]
 * 
 * Revision 1.1.41.3  1994/05/10  16:03:50  marrek
 * 	Bug fixes and I18N for April 1994 submission.
 * 	[1994/05/06  09:58:43  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  13:14:24  marrek]
 * 
 * Revision 1.1.41.2  1994/03/23  15:48:35  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:36:06  keutel]
 * 
 * Revision 1.1.41.1  1994/02/22  18:10:05  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:20:29  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:59:12  marrek]
 * 
 * Revision 1.1.39.1  1993/10/14  19:19:25  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:17:11  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  09:45:58  keutel]
 * 
 * Revision 1.1.2.2  1993/08/10  07:02:55  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:14:05  marrek]
 * 
 * Revision 1.1.4.4  1992/12/30  21:16:31  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:04:47  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/04  17:37:35  marrek
 * 	November 1992 code drop
 * 	Replace cmx.h by cmx_addr.h
 * 	Replace T_NO_NSAP by T_MAXNSAP
 * 	[1992/12/04  12:20:42  marrek]
 * 
 * Revision 1.1.4.2  1992/11/30  08:32:59  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:41:53  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  21:00:40  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:42:02  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  23:18:17  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:45:43  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsPGet.c,v $ $Revision: 1.1.43.2 $ $Date: 1996/02/18 18:24:39 $";
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
/*	@(#)dsPGet.c	1.39 (Bull S.A)	6/26/92	*/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <xom.h>
#include <xomip.h>
#include <d2dir.h>
#include <xdsiapl.h>
#include <xds.h>
#include <xdsgds.h>
#include <alloc.h>
#include <xdsdef.h>
#include <xomdef.h>
#include <xmhdef.h>
#include <xdsalloc.h>
#include <xdsimport.h>
#include <xdspackage.h>
#include <cmx_addr.h>
#include <osiaddr.h>
#include <omDelete.h> /* for dsP_delete_desc_value() */
#include <omGet.h> /* for omP_alloc_sub_handle() */
#include <dsPGetBdcp.h> /* for get_*() */
#include <dsPAcl.h> /* for dsP_decode_acl() */
#include <dsPPut.h> /* for dsP_get_nb_rdn_ava() */
#include <xdssap.h> 
#include <sapdef.h> 
#include <dsPPsap.h> 
#include <dsPMdup.h>  /* for dsP_get_disp(), dsP_get_OR_*() */
#include <dsPGet.h>
#include <d27conv.h> /* for d27_local_to_t61(), d27_t61_to_local() */
#include <assert.h>

/***************************************************************/
/*		nonstatic function prototypes							*/
/***************************************************************/

int compare_issuer(
	const byte *first,
	const byte *second);

int compare_signature(
	const Alg_id *first,
	const Alg_id *second);

/***************************************************************/
/*		static function prototypes							*/
/***************************************************************/

static OM_return_code
dsP_convert_oid(
	OMX_workspace	workspace,
	OM_sint	len,
	char *ptr,
	OM_string	*str);

static OM_return_code
dsP_update_desc_value(OM_private_object	original,
	OM_descriptor	**adr_desc,
	D2_a_value	*value_ptr,
	OM_exclusions exclusions,
	OM_boolean 		local_strings,
	OM_sint nb_val,
	OM_sint max_val,
	OM_type type);

static OM_return_code
dsP_get_rdn(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_rdn				xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_ava(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_ava				*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_attribute(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_attribute		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_attr_list(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_attr_list		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_compare_result(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_compare_result	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_list_res(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_list_res		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_list_info(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_list_info		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_list_info_item(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_list_item		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_extension(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_extension		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_context(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_context			*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_entry_info(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_entry_info		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_entry_info_sel(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_entry_info_sel	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_entry_mod(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_entry_mod		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_entry_mod_list(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_entry_mod_list	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_fi_item(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_fi_item			*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_filter(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_filter			*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_read_result(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_read_result		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_search_result(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_search_result	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_search_info(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_search_info		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_session(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_session			*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_error(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_error			*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_n_error(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_error			*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_a_error(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_error			*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_referral(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_error			*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_ap_error(
	OMX_workspace		workspace,
	OM_private_object	original,
	D2_a_problem		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_acc_point(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_acc_point		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_op_progress(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_op_progress		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_part_oq(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_part_oq			*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_cont_ref(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_cont_ref		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_aci(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_acl_item		xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_acl(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_acl				xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_algorithm_ident(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_algorithm_ident	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);


static OM_return_code
dsP_get_certificate(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_certificate		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_certificate_sublist(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_certificate_sublist		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_certificate_list(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_certificate_list		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

static OM_return_code
dsP_get_certificate_pair(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_certificate_pair *xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number);

#define	IS_INVALID	-1

OM_sint	IAPL_TO_OM[] =
{
	IS_INVALID,
	IS_INVALID,
	OM_S_OBJECT_IDENTIFIER_STRING, /*D2_OBJ_IDENT		*/
	IS_INVALID,
	IS_INVALID,
	OM_S_PRINTABLE_STRING, /* D2_PRINTABLE		*/
	OM_S_NUMERIC_STRING, /* D2_NUMERIC		*/
	IS_INVALID,
	OM_S_BOOLEAN, 	/* D2_BOOLEAN		*/
	OM_S_INTEGER, 	/* D2_INTEGER		*/
	OM_S_OCTET_STRING, 	/* D2_OCTET		*/
	OM_S_UTC_TIME_STRING, /* D2_TIME		*/
	IS_INVALID,

};


#define	CALL_GET(object_class)						\
	dsP_get_##object_class(	workspace, 			\
		 original,OMP_DATA(original),exclusions, \
		included_types, 				\
			local_strings, initial_value,limiting_value,copy,total_number)

#define	UPDATE_SELECTOR(SEL)					\
{								\
		UPDATE_DESC_STR(ptr_desc, 	\
		SEL,OM_S_OCTET_STRING, 		\
		((Psap_info	*)pt_l->desc)->psap_infolng, \
		((Psap_info	*)pt_l->desc)->psap_infoval,1);	\
}


OM_return_code
dsP_check_specific_syntax(
	OM_syntax syntax)
{
	switch (syntax)	 {
		/* GDS syntaxes		*/
#if 0 /* VR */
	case DSX_S_INTEGER_LIST	:
		return(OM_SUCCESS);
#endif /* 0 VR */
	default:
		return(OM_WRONG_VALUE_SYNTAX);
	} /* endswitch */
} /* end dsP_check_specific_syntax */


OM_return_code
dsP_validate_type_list(
	OMP_type_list	*legal_types,
	OM_type_list	included_types)
{
	OMP_type_list	*plegal_types;
	OM_type			included_type;
	OM_type			legal_type;

	if (included_types == OM_NO_MORE_TYPES) {
		return (OM_SUCCESS);
	} /* endif */
	for(;(included_type = *included_types)!=OM_NO_MORE_TYPES;included_types++) {
		for (plegal_types = legal_types;
		(legal_type = plegal_types->type) != OM_NO_MORE_TYPES; plegal_types++) {
			if (legal_type == included_type) {
				plegal_types->nb_val++;
				break;
			} /* endif */
		} /* endfor */
		if (legal_type == OM_NO_MORE_TYPES) {
			return(OM_NO_SUCH_TYPE);
		} /* endif */
	} /* endfor */
	for (plegal_types = legal_types;
	plegal_types->type != OM_NO_MORE_TYPES; plegal_types++) {
		if (plegal_types->nb_val == 0) {
			plegal_types->max_val = 0;
		} else {
			plegal_types->nb_val = 0;
		} /* endif */
	} /* endfor */
	return (OM_SUCCESS);
} /* end dsP_validate_type_list */


/********************************************************/
/*							*/
/*	convert_oid()					*/
/*		convert	an object identifier in	Hexa	*/
/*		from an	ASCII string			*/
/*		used for Distinguished name		*/
/********************************************************/
static OM_return_code
dsP_convert_oid(
	OMX_workspace	workspace,
	OM_sint			len,
	char 			*ptr,
	OM_string		*str)
{
	long	val[D2_NP_ID_LEN];	/* value converted from	ascii
											   to binary */
	OM_sint		val_len[D2_NP_ID_LEN];/* byte count of the converted value */
	OM_sint		n_val;
	OM_sint		str_len;
	OM_sint		number;
	OM_sint		i, j;
	OM_sint		rest;
	OM_sint		quot;
	unsigned char	c;
	unsigned char	bit;
	unsigned char	*pt;

	/* convert each	value between two "." into binary */
	/* in the val array	*/
	for (i = 0, pt = (unsigned char	*)ptr, n_val = 0; i	 < len;	n_val++) {
		val[n_val] = 0;
		while ((i <	len) &&	((c = pt[i++]) != '.'))	 {
			val[n_val] = val[n_val]	 * 10 +	(c -	'0');
		} /* endwhile */
	} /* endfor */

	/* get the global string size before allocating	STRING units */
	for (i = 0, str_len	 = 0; i	< n_val; i++) {
		number = val[i];
		val_len[i] = 1;
		while ((number = (number / 128)) > 0) {
			val_len[i]++;
			str_len++;
		} /* endwhile */
		str_len++;
	} /* endfor */
	if ((str->elements = (void *)OMP_XALLOC(workspace, AL_STRING,
	ROUNDUP(workspace, str_len + 1))) == 0) {
		return(OM_MEMORY_INSUFFICIENT);
	} /* endif */

	/* convert into	the	external representation	*/
	for (i = 0, pt = str->elements;	i <	n_val; i++)	 {
		number = val[i];
		j = val_len[i];
		while (j) {
			quot = number /	128;
			rest = number -	(quot *	128);
			bit	 = (((val_len[i] > 1) && (j != val_len[i])) ? 0x80 : 0);
			pt[--j]	 = (unsigned char)rest | bit;
			number = quot;
		} /* endwhile */
		pt += val_len[i];
	} /* endfor */
	*pt	 = '\0';
	str->length	 = str_len;
	return (OM_SUCCESS);
} /* end dsP_convert_oid */


/********************************************************/
/*							*/
/*	dsP_alloc_descriptor()				*/
/*		allocates the descriptors		*/
/*		initializes	-1st and last descriptors	*/
/*							*/
/********************************************************/

OM_return_code
dsP_alloc_descriptor(
	OMX_workspace	workspace,
	OM_sint			nb_desc, OM_descriptor **desc)
{
	/* add -1st	and	last NULL descriptor	*/
	if ((*desc = (OM_descriptor	*)OMP_XALLOC(workspace, AL_DESCRIPTOR,
		 nb_desc + 2)) == 0) {
		return(OM_MEMORY_INSUFFICIENT);
	} /* endif */
	memset(*desc, 0, (nb_desc +	2) * (sizeof(OM_descriptor)));

	return(OM_SUCCESS);

} /* end dsP_alloc_descriptor */


/********************************************************/
/*							*/
/*	free_resources()				*/
/*		free resources (values and descriptors)	*/
/*		in case	of error			*/
/********************************************************/
OM_return_code
dsP_free_resources(
	OMX_workspace workspace,
	OM_descriptor *desc0, OM_descriptor	*cur_desc,
	OM_sint			nb_desc)
{
	OM_descriptor *	ptr;

	/* free	values		*/
	for (ptr = desc0; ptr <	cur_desc; ptr++) {
		delete_desc_value(workspace, ptr);
	} /* endfor */
	/* free	descriptors	*/
	ptr	 = OMP_INTERNAL(desc0);
	OMP_XFREE(workspace, ptr, AL_DESCRIPTOR, nb_desc + 2);

} /* end dsP_free_resources	*/


/********************************************************/
/*							*/
/*	dsP_update_desc_value()				*/
/*		updates	descriptor (type,syntax,value)	*/
/*		from D2_a_value	structure		*/
/*							*/
/********************************************************/
static OM_return_code
dsP_update_desc_value(
	OM_private_object	original,
	OM_descriptor		**adr_desc,
	D2_a_value			*value_ptr,
	OM_exclusions 		exclusions,
	OM_boolean 			local_strings,
	OM_sint 			nb_val,
	OM_sint 			max_val,
	OM_type 			type)
{

	OM_syntax		syntax;
	OM_sint			i;
	OM_descriptor	*ptr_desc;
	OMX_workspace	workspace = OMP_WORKSPACE(original);
	OM_return_code	ret = OM_SUCCESS;

	ptr_desc = *adr_desc;
	for (i = 0;	i <	nb_val;	i++, value_ptr++) {
		switch (value_ptr->d2_a_rep) {
		case D2_BOOLEAN	:
			UPDATE_DESC_BOOL(ptr_desc, type, value_ptr->d2_at_value.d2_bool_a_v,
			max_val);
			break;
		case D2_INTEGER	:
			UPDATE_DESC_INT(ptr_desc, type, value_ptr->d2_at_value.d2_int_a_v,
			max_val);
			break;
		case D2_ENUM	:
			UPDATE_DESC_ENUM(ptr_desc, type, value_ptr->d2_at_value.d2_int_a_v,
			max_val);
			break;
		case D2_OCTET	:
		case D2_PRINTABLE	:
		case D2_OBJ_IDENT	:
		case D2_NUMERIC		:
		case D2_TIME	:
			UPDATE_DESC_STR(ptr_desc, type, IAPL_TO_OM[value_ptr->d2_a_rep],
			value_ptr->d2_a_v_len, value_ptr->d2_at_value.d2_oct_a_v, max_val);
			break;
		case D2_ASN1	:
			UPDATE_DESC_STR(ptr_desc, type, OM_S_ENCODING_STRING,
			value_ptr->d2_a_v_len, value_ptr->d2_at_value.d2_oct_a_v, max_val);
			break;
		case D2_T61		:
			if (local_strings) {
				char *result;
				OM_uint32 result_len = 0;
				if (d27_t61_to_local( (char *)value_ptr->d2_at_value.d2_oct_a_v,
				value_ptr->d2_a_v_len,&result,&result_len) != D2_NOERROR) {
					ret	 = OM_WRONG_VALUE_SYNTAX;
					break;
				} else {
					value_ptr->d2_a_v_len = result_len;
					value_ptr->d2_at_value.d2_oct_a_v = (void *)result;
					/* possible memory leak here */
				} /* endif */
			} /* endif */
			UPDATE_DESC_STR(ptr_desc, type,
			local_strings?OM_S_TELETEX_STRING|OM_S_LOCAL_STRING:OM_S_TELETEX_STRING ,
			value_ptr->d2_a_v_len, value_ptr->d2_at_value.d2_oct_a_v, max_val);
			break;
		case D2_DISTNAME	:	/*	Object Name		*/
			OBJECT_VALUE(dn, dsP_DS_C_DS_DN, 
			value_ptr->d2_at_value.d2_obj_name_a_v, type, max_val);
			break;
		case D2_PSAP_STX	:	/*	Object Presentation	Address*/
			OBJECT_VALUE(psap, dsP_DS_C_PRESENTATION_ADDRESS,
			value_ptr->d2_at_value.d2_psap_a_v, type, max_val);
			break;
		case D2_GUIDE	:	/*	Object Search-guide	(BDCP)*/
			OBJECT_VALUE(search_guide, dsP_DS_C_SEARCH_GUIDE,
			value_ptr->d2_at_value.d2_guide_a_v, type, max_val);
			break;
		case D2_T61_PR_LIST	:	 /*	Object Postal-Address (BDCP)*/
		case D2_POST_AD_STX	:	 /*	Object Postal-Address (BDCP)*/
			OBJECT_VALUE(postal_address, dsP_DS_C_POSTAL_ADDRESS,
			value_ptr->d2_at_value.d2_post_a_v, type, max_val);
			break;
		case D2_TLXNR_STX	:	/*	Object Telex-number	(BDCP)*/
			OBJECT_VALUE(telex_number, dsP_DS_C_TELEX_NBR,
			value_ptr->d2_at_value.d2_tlxnr_a_v, type, max_val);
			break;
		case D2_TTXID_STX	:	/*	Object Teletex-Terminal-Id */
			OBJECT_VALUE(ttxid, dsP_DS_C_TELETEX_TERM_IDENT,
			value_ptr->d2_at_value.d2_ttxid_a_v, type, max_val);
			break;
		case D2_FAXNR_STX	:	/*	Object Facsimile-Telephone */
			OBJECT_VALUE(faxnumber, dsP_DS_C_FACSIMILE_PHONE_NBR,
			value_ptr->d2_at_value.d2_faxnr_a_v, type, max_val);
			break;
        case D2_CERTIFICATE :   /*  Object Certificate (SAP)*/
			OBJECT_VALUE(certificate, dsP_DS_C_CERT,
			value_ptr->d2_at_value.d2_cert_a_v, type, max_val);
			break;
        case D2_CERT_PAIR   :   /*  Object Certificate Pair (SAP)*/
			OBJECT_VALUE(certificate_pair, dsP_DS_C_CERT_PAIR,
			value_ptr->d2_at_value.d2_ctpair_a_v, type, max_val);
			break;
        case D2_CERT_LIST   :   /*  Object Certificate List (SAP)*/
			OBJECT_VALUE(certificate_list, dsP_DS_C_CERT_LIST,
			value_ptr->d2_at_value.d2_ctlist_a_v, type, max_val);
            break;
		case D2_ACCL	:	/*	Object Access Control List (GDS) */
			OBJECT_VALUE(acl, dsP_DSX_C_GDS_ACL,
			value_ptr->d2_at_value.d2_oct_a_v, type, max_val);
			break;
			/*	CASE_GET_VALUE_MDUP_PKG	*/
#ifdef	_MDUP_PKG_
		case D2_MHS_DLSP_STX	:	/*	Object DL-submit-permissions */
			OBJECT_VALUE(dlsp, dsP_DS_C_DL_SUBMIT_PERMS,
			value_ptr->d2_at_value.d2_dl_s_p_av, type, max_val);
			break;
		case D2_MHS_ORNAME_STX :	/*	Object OR-Name */
			OBJECT_VALUE(OR_name, dsP_MH_C_OR_NAME,
			value_ptr->d2_at_value.d2_or_name_av, type, max_val);
			break;
		case D2_MHS_ORADR_STX :	/*	Object OR-Address */
			OBJECT_VALUE(OR_address, dsP_MH_C_OR_ADDRESS,
			value_ptr->d2_at_value.d2_or_addr_av, type, max_val);
			break;
		case D2_MHS_PR_DM_STX :	/*	Preferred-Delivery-Methods */
			UPDATE_DESC_ENUM(ptr_desc, type,
			value_ptr->d2_at_value.d2_int_a_v, max_val);
			break;
#endif	/* _MDUP_PKG_ */

		default:			/*	GDS	syntaxes		*/
			switch (value_ptr->d2_a_rep) {
#if 0 	/* VR */
		case D2_INT_LIST	:
		  syntax = DSX_S_INTEGER_LIST;
		  break;
#endif /* 0 VR */
			default:
				ret	 = OM_WRONG_VALUE_SYNTAX;
				break;
			} /* endswitch */
			if (ret	 == OM_SUCCESS) {
				UPDATE_DESC_STR(ptr_desc, type, syntax, value_ptr->d2_a_v_len,
				value_ptr->d2_at_value.d2_oct_a_v, max_val);
			} /* endif */
			break;
		} /* endswitch */
		if (ret) {
			*adr_desc = ptr_desc;
			return(ret);
		} /* endif */
	} /* endfor */
	*adr_desc = ptr_desc;
	return(OM_SUCCESS);
} /* dsP_update_desc_value */



/********************************************************/
/*							*/
/*	dsP_get_switch()					*/
/*							*/
/********************************************************/
OM_return_code
dsP_get_switch(
	const OM_private_object	original,
	const OM_exclusions		exclusions,
	const OM_type_list		included_types,
	const OM_boolean		local_strings,
	const OM_value_position	initial_value,
	const OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number)
{
	OM_sint				ret;
	OMP_object_type		obj_type;
	OMX_workspace		workspace = OMP_WORKSPACE(original);

	obj_type = OMP_OBJ_TYPE(original);
	switch (OMP_PACKAGE(obj_type))	 {
		/* Directory Service Package classes	*/
	case XDS_SERVICE_PACKAGE :
		switch (OMP_CLASS(obj_type)) {
		case XDS_PRESENTATION_ADDRESS:
			ret	 = CALL_GET(psap);
			break;
		case XDS_DS_RDN:
			ret	 = CALL_GET(rdn);
			break;
		case XDS_DS_DN:
			ret	 = CALL_GET(dn);
			break;
		case XDS_AVA:
			ret	 = CALL_GET(ava);
			break;
		case XDS_ATTRIBUTE:
			ret	 = CALL_GET(attribute);
			break;
		case XDS_ATTRIBUTE_LIST:
			ret	 = CALL_GET(attr_list);
			break;
		case XDS_EXT:
			ret	 = CALL_GET(extension);
			break;
		case XDS_CONTEXT:
			ret	 = CALL_GET(context);
			break;
		case XDS_ENTRY_INFO:
			ret	 = CALL_GET(entry_info);
			break;
		case XDS_ENTRY_INFO_SELECTION:
			ret	 = CALL_GET(entry_info_sel);
			break;
		case XDS_ENTRY_MOD:
			ret	 = CALL_GET(entry_mod);
			break;
		case XDS_ENTRY_MOD_LIST:
			ret	 = CALL_GET(entry_mod_list);
			break;
		case XDS_FILTER:
			ret	 = CALL_GET(filter);
			break;
		case XDS_FILTER_ITEM:
			ret	 = CALL_GET(fi_item);
			break;
		case XDS_SESSION:
			ret	 = CALL_GET(session);
			break;
		case XDS_COMPARE_RESULT:
			ret	 = CALL_GET(compare_result);
			break;
		case XDS_LIST_RESULT:
			ret	 = CALL_GET(list_res);
			break;
		case XDS_READ_RESULT:
			ret	 = CALL_GET(read_result);
			break;
		case XDS_SEARCH_RESULT:
			ret	 = CALL_GET(search_result);
			break;
		case XDS_ABANDON_FAILED:
		case XDS_UPDATE_ERROR:
		case XDS_SYSTEM_ERROR:
		case XDS_SERVICE_ERROR:
		case XDS_LIBRARY_ERROR:
		case XDS_SECURITY_ERROR:
		case XDS_COMMUNICATIONS_ERROR:
			ret	 = CALL_GET(error);
			break;
		case XDS_ATTRIBUTE_ERROR:
			ret	 = CALL_GET(a_error);
			break;
		case XDS_ATTRIBUTE_PROBLEM:
			ret	 = CALL_GET(ap_error);
			break;
		case XDS_NAME_ERROR:
			ret	 = CALL_GET(n_error);
			break;
		case XDS_REFERRAL:
			ret	 = CALL_GET(referral);
			break;
		case XDS_CONTINUATION_REF:
			ret	 = CALL_GET(cont_ref);
			break;
		case XDS_ACCESS_POINT :
			ret	 = CALL_GET(acc_point);
			break;
		case XDS_OPERATION_PROGRESS:
			ret	 = CALL_GET(op_progress);
			break;
		case XDS_PARTIAL_OUTCOME_QUAL:
			ret	 = CALL_GET(part_oq);
			break;
		case XDS_LIST_INFO_ITEM:
			ret	 = CALL_GET(list_info_item);
			break;
		case XDS_LIST_INFO:
			ret	 = CALL_GET(list_info);
			break;
		case XDS_SEARCH_INFO:
			ret	 = CALL_GET(search_info);
			break;
		default:
			ret	 = OM_NO_SUCH_OBJECT;
		} /* endswitch */
		break;
		/* Gds Directory Service Package classes	*/
	case XDS_GDS_PACKAGE:
		switch (OMP_CLASS(obj_type)) {
		case XDS_GDS_SESSION:
			ret	 = CALL_GET(session);
			break;
		case XDS_GDS_CONTEXT:
			ret	 = CALL_GET(context);
			break;
		case XDS_GDS_ACL:
			ret	 = CALL_GET(acl);
			break;
		case XDS_GDS_ACL_ITEM:
			ret	 = CALL_GET(aci);
			break;
		default:
			ret	 = OM_NO_SUCH_OBJECT;
		} /* endswitch */
		break;
		/* Basic Directory Contents	Package		*/
	case XDS_BDCP:
		switch (OMP_CLASS(obj_type)) {
		case XDS_FACSIMILE_PHONE_NBR:
			ret	 = CALL_GET(faxnumber);
			break;
		case XDS_POSTAL_ADDRESS:
			ret	 = CALL_GET(postal_address);
			break;
		case XDS_SEARCH_CRITERION:
			ret	 = CALL_GET(search_criterion);
			break;
		case XDS_SEARCH_GUIDE :
			ret	 = CALL_GET(search_guide);
			break;
		case XDS_TELETEX_TERM_IDENT:
			ret	 = CALL_GET(ttxid);
			break;
		case XDS_TELEX_NBR:
			ret	 = CALL_GET(telex_number);
			break;
		default:
			ret	 = OM_NO_SUCH_OBJECT;
		} /* endswitch */
		break;
	case XMH_PACKAGE:
		switch (OMP_CLASS(obj_type)) {
		case XMH_G3_FAX_NBPS:
			ret	 = CALL_GET(G3_Fax_NBP);
			break;
		case XMH_TELETEX_NBPS:
			ret	 = CALL_GET(Teletex_NBP);
			break;
			CASE_GET_MDUP_PACKAGE_CLOSURE_OBJECT
#if 0
		case XMH_OR_NAME:
			ret = CALL_GET(OR_name);
		break;
		case XMH_OR_ADDRESS:
			ret = OM_NO_SUCH_OBJECT;
			break;
		default:
			ret = OM_NO_SUCH_OBJECT;
		break;
#endif /* 0 */
		} /* endswitch */
		break;
		CASE_GET_MDUP_PACKAGE 
#if 0
	case XDS_MDUP:
	switch(OMP_CLASS(obj_type))	{
		case MHS_DLSP:
			ret = CALL_GET(dlsp);
			break;
		default:
			ret = OM_NO_SUCH_OBJECT;
			break;
		} /* endswitch */
		break;
#endif /* 0 */
    case XDS_SAP_PACKAGE:
        switch (OMP_CLASS(obj_type)) {
		case XDS_ALGORITHM_IDENT:
            ret = CALL_GET(algorithm_ident);
			break;
        case XDS_CERTIFICATE:
            ret = CALL_GET(certificate);
            break;
		case XDS_CERTIFICATE_PAIR:
            ret = CALL_GET(certificate_pair);
            break;
		case XDS_CERTIFICATE_LIST:
            ret = CALL_GET(certificate_list);
            break;
		case XDS_CERTIFICATE_SUBLIST:
            ret = CALL_GET(certificate_sublist);
            break;
        default:   
            ret = OM_NO_SUCH_OBJECT;
            break;
        } /* endswitch */
        break;
	default:
		ret	 = OM_NO_SUCH_OBJECT;
	} /* endswitch */
	return(ret);
} /* end dsP_get_switch	*/


/********************************************************/
/*							*/
/*	get	PRESENTATION ADDRESS object			*/
/*							*/
/********************************************************/
OM_return_code
dsP_get_psap(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_psap			xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{

	OM_return_code	ret	 = OM_SUCCESS;
	OMP_type_list	legal_types[] = PSAP_TYPE_LIST;
	OM_sint			nb_desc	 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	Psap_info 		p_sel;
	Psap_info 		t_sel;
	Psap_info 		s_sel;
	OM_sint			no_nsap;
	Psap_info 		nsap_info[T_MAXNSAP];
	Psap_info	 	* nsap_ptr;
	OM_boolean 		initialize;
	OM_sint			i;

    /* vars for local-string conversion */
	OM_sint	j;
#define MAXNSAPSTR  MAX_DSPNM * MAX_DSP_PARTS  /* 80 */
    OM_sint32 nlens[T_MAXNSAP]= {MAXNSAPSTR,MAXNSAPSTR,MAXNSAPSTR,MAXNSAPSTR,
                           MAXNSAPSTR,MAXNSAPSTR,MAXNSAPSTR,MAXNSAPSTR};
    char *nsaps[T_MAXNSAP]; 
    byte *nbuf = NULL;
    char selbuf[(2 * T_PSELSIZE) + 4]; 
    OM_syntax syntax;

	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	if (xobject) {
		if (dsP_decode_psap_addr(&p_sel, &s_sel, &t_sel, &no_nsap, nsap_info,
				xobject)) {
			omP_error("	Error psap decode");
		} /* endif */

        if (local_strings == OM_TRUE) {
            if ((nbuf = (byte *) malloc(no_nsap * MAXNSAPSTR)) == NULL) {
		        return(OM_MEMORY_INSUFFICIENT);
			} /* endif */

            for (i=j=0; i < no_nsap; i++, j += MAXNSAPSTR) {
                nsaps[i] = (char*)nbuf + j;
            } /* endfor */

            if (d2p_d01_bin_to_str_psap(xobject, selbuf, selbuf, selbuf, 
									no_nsap, nsaps, nlens) < 0) {
                if (nbuf) {
                    free(nbuf);
				} /* endif */
                return(OM_WRONG_VALUE_MAKEUP);
            } /* endif */

            for (i=j=0; i < no_nsap; i++, j += MAXNSAPSTR) {
                nsap_info[i].psap_infolng = strlen((char *)&nbuf[j]);
                nsap_info[i].psap_infoval = (char *)&nbuf[j];
            } /* endfor */
        } /* endif */
	} /* endif */

	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_PRESENTATION_ADDRESS);
				break;
			case DS_N_ADDRESSES:
				if ((!xobject) || (!(pt_l->max_val =  no_nsap))) {
					break;
				} /* endif */
				if (!initialize) {
					UPDATE_LIMIT_VALUE(no_nsap);
					pt_l->desc = &nsap_info[initial_value];
					nb_desc	 += pt_l->nb_val;
					break;
				} /* endif */
				nsap_ptr = (Psap_info *		)pt_l->desc;
                if (local_strings == OM_TRUE) {
                    syntax = OM_S_OCTET_STRING | OM_S_LOCAL_STRING;
				} else {
                    syntax = OM_S_OCTET_STRING;
				} /* endif */

				for (i = 0;	i <		pt_l->nb_val; i++, nsap_ptr++) {
					UPDATE_DESC_STR(ptr_desc, DS_N_ADDRESSES, syntax,
					nsap_ptr->psap_infolng, nsap_ptr->psap_infoval, no_nsap);
				} /* endfor */
				break;
			case DS_P_SELECTOR:
				if ((!xobject) || (!p_sel.psap_infoval)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				pt_l->desc = &p_sel;
				UPDATE_SELECTOR(DS_P_SELECTOR);
				break;
			case DS_S_SELECTOR:
				if ((!xobject) || (!s_sel.psap_infoval)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				pt_l->desc = &s_sel;
				UPDATE_SELECTOR(DS_S_SELECTOR);
				break;
			case DS_T_SELECTOR:
				if ((!xobject) || (!t_sel.psap_infoval)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				pt_l->desc = &t_sel;
				UPDATE_SELECTOR(DS_T_SELECTOR);
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
                if (nbuf) {
                    free (nbuf);
				} /* endif */
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
            } /* endif */
		} /* endfor */

		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
            if (nbuf) {
                free (nbuf);
			} /* endif */
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!= OM_SUCCESS) {
            if (nbuf) {
                free (nbuf);
			} /* endif */
			return(ret);
        } /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */


	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
    if (nbuf) {
        free (nbuf);
	} /* endif */
	return(ret);
} /* end dsP_get_psap */


/********************************************************/
/*							*/
/*	get	DS RDN object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_rdn(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_rdn				xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)

{

	OMP_type_list	legal_types[] = RDN_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor 	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OM_syntax		syntax;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;
	OM_string 		desc_rdn[D2_NO_AVA + 1] =  { { 0, (char *)0 } };
	OM_string 		*drdn_ptr;
	OM_sint			i, nb_ava, len, len_val, nb_val_d;
	OM_object		ptr_han;
	char			c;
	xds_ava	 		*ava;
	OM_string		oid;

	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	if (xobject) {
		if ((ret = dsP_get_nb_rdn_ava ((char *)xobject, desc_rdn, &nb_val_d,
		D2_AVA_SEP)) != OM_SUCCESS) {
			return (ret);
		} /* endif */
		for (i = 1;	i <	nb_val_d; i++) {
			desc_rdn[i].length--;
			desc_rdn[i].elements = ((char *)desc_rdn[i].elements)+1;
		} /* endfor */
	} /* endif */
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_DS_RDN);
				break;
			case DS_AVAS:
				if (!xobject) {
					break;
				} /* endif */
				if (!initialize) {
					UPDATE_LIMIT_VALUE(nb_val_d);
					pt_l->desc = &desc_rdn[initial_value];
					nb_desc	 += pt_l->nb_val;
					break;
				} /* endif */
				drdn_ptr = pt_l->desc;
				for (i = 0;	i <	pt_l->nb_val; i++, drdn_ptr++) {
					if ((!OMP_EXCLUDE_VALUES) 
					&&  (!OMP_EXCLUDE_MULTIPLES(nb_val_d)))	 {
						if ((ava = (xds_ava * )OMP_XALLOC(workspace,
						AL_XDS_AVA, 1)) == NULL) {
							clean_before_exit(desc0, ptr_desc, nb_desc, ret);
						} /* endif */
						len	 = 0;
						while (((c = ((char *)drdn_ptr->elements)[len++])
						!= D2_T61_REP) && (c != D2_PRTBL_REP)
						&& (c != D2_IA5_REP) 
						&& (c != D2_NUM_REP) &&  /* not ^C^D^E^F	*/
						(c != '\0')) {
							;
						} /* endwhile */

						/*	Syntax initialisation				*/
						switch (c) {
						case D2_T61_REP:
							ava->d2_avaval.d2_a_rep	 = D2_T61;
							break;
						case D2_PRTBL_REP:
							ava->d2_avaval.d2_a_rep	 = D2_PRINTABLE;
							break;
						case D2_IA5_REP:
							ava->d2_avaval.d2_a_rep	 = D2_IA5;
							break;
						case D2_NUM_REP:
							ava->d2_avaval.d2_a_rep	 = D2_NUMERIC;
							break;
						default:
							ava->d2_avaval.d2_a_rep	 = D2_T61;
							break;
						} /* endswitch */

						/* convert attribute type in Hexa	*/
						if (( dsP_convert_oid(workspace, len - 1,
						drdn_ptr->elements, &oid)) != OM_SUCCESS) {
							clean_before_exit(desc0, ptr_desc, nb_desc, ret);
						} /* endif */

						ava->d2_avatype.d2_typ_len = oid.length;
						ava->d2_avatype.d2_type	 = oid.elements;
						ava->d2_avaval.d2_at_value.d2_prt_a_v =
							((char *)drdn_ptr->elements	 + len);
						len_val	 = 0;
						while (((c = ((char *)drdn_ptr->elements)[len++]) 
						!= D2_RDN_SEP)	 &&	 (c	!= D2_AVA_SEP) && (c != '\0')) {
							len_val++;
						} /* endwhile */
						ava->d2_avaval.d2_a_v_len = len_val;


						if (OMP_EXCLUDE_SUBOBJECTS)	 {
							if ((ret = omP_alloc_sub_handle(original,
							dsP_DS_C_AVA, ava, &ptr_han)) != OM_SUCCESS) {
								clean_before_exit(desc0, ptr_desc, nb_desc,ret);
							} /* endif */
							syntax = OM_S_OBJECT | OM_S_PRIVATE;
						} else {
							if ((ret = dsP_get_ava(workspace, original, ava,
							0, 0, local_strings, 0, 0, &ptr_han,
							(OM_value_position *)&nb_ava)) != OM_SUCCESS) {
								clean_before_exit(desc0, ptr_desc, nb_desc,ret);
							} /* endif */
							OMP_XFREE(workspace, ava, AL_XDS_AVA, 1);
							OMP_XFREE_STR(workspace, oid.elements, oid.length);
							syntax = OM_S_OBJECT ;
						} /* endif */
						UPDATE_DESC_OBJ(ptr_desc, DS_AVAS, ptr_han, syntax);
					} else {
						UPDATE_DESC_TYPE_AND_SYNTAX(ptr_desc, DS_AVAS,
						OM_S_OBJECT | OM_S_NO_VALUE);
					} /* endif */
				} /* endfor */
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_rdn */


/********************************************************/
/*							*/
/*	get	DS DN object				*/
/*							*/
/********************************************************/
OM_return_code
dsP_get_dn(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_dist_name		xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list		legal_types[] = DN_TYPE_LIST;
	OM_return_code		ret	 = OM_SUCCESS;
	OM_sint				nb_desc		 = 0;
	OM_descriptor		*ptr_desc;
	OM_descriptor	 	*desc;
	OM_descriptor	 	*desc0;
	OMP_type_list 		*pt_l;
	OM_boolean 			initialize;
	OM_string  			desc_rdn[D2_NP_MAX + 1] =  { { 0, (char *)0 } };
	OM_string  			*drdn_ptr;
	OM_sint				nb_val_d, i;

	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of values for each included attribute type	*/
	initialize = OM_FALSE;
	if (xobject) {
		if ((ret = dsP_get_nb_rdn_ava ((char *)xobject, desc_rdn, &nb_val_d,
			 D2_RDN_SEP)) != OM_SUCCESS) {
			return (ret);
		} /* endif */
		for (i = 1;	i < nb_val_d; i++) {
			desc_rdn[i].length--;
			desc_rdn[i].elements = ((char *)desc_rdn[i].elements)+1;
		} /* endfor */
	} /* endif */
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_DS_DN);
				break;
			case DS_RDNS:
				if (!xobject) {
					break;
				} /* endif */
				if (!initialize) {
					UPDATE_LIMIT_VALUE(nb_val_d);
					pt_l->desc = &desc_rdn[initial_value];
					nb_desc	 += pt_l->nb_val;
					break;
				} /* endif */
				drdn_ptr = pt_l->desc;
				for (i = 0;	i <	pt_l->nb_val; i++, drdn_ptr++) {
					OBJECT_VALUE(rdn, dsP_DS_C_DS_RDN,
					(xds_rdn)drdn_ptr->elements, DS_RDNS, nb_val_d);
				} /* endfor */
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_dn	*/


/********************************************************/
/*							*/
/*	get	AVA	object					*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_ava(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_ava				*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list		legal_types[] = AVA_TYPE_LIST;
	OM_return_code		ret	 = OM_SUCCESS;
	OM_sint				nb_desc		 = 0;
	OM_descriptor	 	*ptr_desc;
	OM_descriptor	 	*desc;
	OM_descriptor	 	*desc0;
	OMP_type_list 		*pt_l;
	OM_boolean 			initialize;

	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types; pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_AVA);
				break;
			case DS_ATTRIBUTE_TYPE:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DS_ATTRIBUTE_TYPE,
				OM_S_OBJECT_IDENTIFIER_STRING, xobject->d2_avatype.d2_typ_len,
				xobject->d2_avatype.d2_type, 1);
				break;
			case DS_ATTRIBUTE_VALUES:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				ret	 = dsP_update_desc_value(original, &ptr_desc,
				&xobject->d2_avaval, exclusions, local_strings, 1, 1,
				DS_ATTRIBUTE_VALUES);
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors	*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	+ 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_ava */


/********************************************************/
/*							*/
/*	get	ATTRIBUTE object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_attribute(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_attribute		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = A_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;

	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_ATTRIBUTE);
				break;
			case DS_ATTRIBUTE_TYPE:
				if ((!xobject) || (!xobject->d2_a_type.d2_type)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DS_ATTRIBUTE_TYPE,
				OM_S_OBJECT_IDENTIFIER_STRING, xobject->d2_a_type.d2_typ_len,
				xobject->d2_a_type.d2_type, 1);
				break;
			case DS_ATTRIBUTE_VALUES:
				if ((!xobject) ||  (!(pt_l->max_val	 = xobject->d2_a_no_val))) {
					break;
				} /* endif */
				if (!initialize) {
					UPDATE_LIMIT_VALUE(pt_l->max_val);
					pt_l->desc = (D2_a_value *)(xobject->d2_a_val)
					+ initial_value;
					nb_desc	 += pt_l->nb_val;
					break;
				} /* endif */
				if (pt_l->nb_val) {
					ret	 = dsP_update_desc_value(original, &ptr_desc,
					pt_l->desc, exclusions, local_strings, pt_l->nb_val,
					pt_l->max_val, DS_ATTRIBUTE_VALUES);
				} /* endif */
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_attribute */




/********************************************************/
/*							*/
/*	get	ATTRIBUTE LIST object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_attr_list(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_attr_list		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = A_L_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list	*pt_l;
	OM_boolean 		initialize;
	xds_attribute	**adr_obj;
	OM_sint			n_cur, i;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */
			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_ATTRIBUTE_LIST);
				break;
			case DS_ATTRIBUTES:
				if (!xobject) {
					break;
				} /* endif */
				if (!initialize) {
					GET_NB_OBJ(xds_attribute, d2_a_next, pt_l->max_val,
					(xds_attribute **) & xobject, adr_obj);
					UPDATE_LIMIT_VALUE(pt_l->max_val);
					JUMP_N_OBJ(xds_attribute, d2_a_next, initial_value,
					&xobject, adr_obj, n_cur);
					pt_l->desc = adr_obj;
					nb_desc	 += pt_l->nb_val;
					break;
				} /* endif */

				adr_obj	 = pt_l->desc;
				for (i = 0;	i < pt_l->nb_val;
				i++, adr_obj = &(*adr_obj)->d2_a_next)	{
					OBJECT_VALUE(attribute, dsP_DS_C_ATTRIBUTE,
					(xds_attribute *) *adr_obj, DS_ATTRIBUTES, pt_l->max_val);
				} /* endfor */
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */
	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_attr_list */


/********************************************************/
/*							*/
/*	get	COMPARE	RESULT object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_compare_result(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_compare_result	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = CMP_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */
			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_COMPARE_RESULT);
				break;
			case DS_FROM_ENTRY:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DS_FROM_ENTRY, xobject->from_entry,
				1);
				break;
			case DS_MATCHED:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DS_MATCHED, xobject->match, 1);
				break;
			case DS_OBJECT_NAME:
				if ((!xobject) || (!xobject->dist_name)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(dn, dsP_DS_C_DS_DN,
				(xds_dist_name)xobject->dist_name, DS_OBJECT_NAME, 1);
				break;
				/* Common Results	*/
			case DS_ALIAS_DEREFERENCED:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DS_ALIAS_DEREFERENCED,
				xobject->com_res->d2_al_deref, 1);
				break;
			case DS_PERFORMER:
				if ((!xobject) || (!xobject->com_res->d2_performer)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(dn, dsP_DS_C_DS_DN,
				(xds_dist_name)xobject->com_res->d2_performer, DS_PERFORMER, 1);
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_compare_result	*/


/********************************************************/
/*							*/
/*	get	LIST RESULT	object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_list_res(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_list_res		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = LIST_RES_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_LIST_RESULT);

				break;
			case DS_LIST_INFO:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(list_info, dsP_DS_C_LIST_INFO,
				(xds_list_info * )xobject, DS_LIST_INFO, 1);
				break;
			case DS_UNCORRELATED_LIST_INFO:
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_list_res */


/********************************************************/
/*							*/
/*	get	LIST INFO object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_list_info(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_list_info		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = LIST_INF_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;
	 xds_list_item 	**adr_obj;
	OM_sint			n_cur, i;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_LIST_INFO);
				break;
			case DS_OBJECT_NAME:
				if ((!xobject) || (!xobject->t_object)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(dn, dsP_DS_C_DS_DN,
				(xds_dist_name)xobject->t_object, DS_OBJECT_NAME, 1);
				break;
			case DS_PARTIAL_OUTCOME_QUAL:
				if ((!xobject) || (!xobject->l_part_oq)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(part_oq, dsP_DS_C_PARTIAL_OUTCOME_QUAL,
				(xds_part_oq *)xobject->l_part_oq, DS_PARTIAL_OUTCOME_QUAL, 1);
				break;
			case DS_SUBORDINATES:
				if ((!xobject) || (!xobject->subordinates)) {
					break;
				} /* endif */
				if (!initialize) {
					GET_NB_OBJ(xds_list_item, d2_l_next, pt_l->max_val,
					(xds_list_item	 * *) &  xobject->subordinates, adr_obj);
					UPDATE_LIMIT_VALUE(pt_l->max_val);
					JUMP_N_OBJ(xds_list_item, d2_l_next, initial_value,
					&xobject->subordinates, adr_obj, n_cur);
					pt_l->desc = adr_obj;
					nb_desc	 += pt_l->nb_val;
					break;
				} /* endswitch */

				adr_obj	 = pt_l->desc;
				for (i = 0;	i < pt_l->nb_val;
				i++, adr_obj = &(*adr_obj)->d2_l_next)	{
					OBJECT_VALUE(list_info_item, dsP_DS_C_LIST_INFO_ITEM,
					(xds_list_item*) *adr_obj, DS_SUBORDINATES, pt_l->max_val);
				} /* endfor */
				break;
				/* Common Results	*/
			case DS_ALIAS_DEREFERENCED:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DS_ALIAS_DEREFERENCED,
				xobject->com_res->d2_al_deref, 1);
				break;
			case DS_PERFORMER:
				if ((!xobject) ||  (!xobject->com_res->d2_performer)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(dn, dsP_DS_C_DS_DN,
				(xds_dist_name)xobject->com_res->d2_performer, DS_PERFORMER, 1);
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_list_info */


/********************************************************/
/*							*/
/*	get	LIST INFO ITEM	object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_list_info_item(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_list_item		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = LIST_INF_I_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_LIST_INFO_ITEM);
				break;
			case DS_ALIAS_ENTRY:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DS_ALIAS_ENTRY, xobject->d2_l_alias,
				1);
				break;
			case DS_FROM_ENTRY:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DS_FROM_ENTRY,
				xobject->d2_l_fromentry, 1);
				break;
			case DS_RDN:
				if ((!xobject) || (!xobject->d2_l_rdn)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(rdn, dsP_DS_C_DS_RDN, (xds_rdn	)xobject->d2_l_rdn,
				DS_RDN, 1);
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_list_info_item	*/


/********************************************************/
/*							*/
/*	get	EXTENSION object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_extension(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_extension		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = EXT_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;

	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */


			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_EXT);
				break;
			case DS_CRIT:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DS_CRIT, xobject->d2_critical, 1);
				break;
			case DS_IDENT:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_INT(ptr_desc, DS_IDENT, xobject->d2_ext_id, 1);
				break;
			case DS_ITEM_PARAMETERS:
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_extension */


/********************************************************/
/*							*/
/*	get	CONTEXT	object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_context(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_context			*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types_ctx[] = CTX_TYPE_LIST;

	OMP_type_list	legal_types_cgx[] = CGX_TYPE_LIST;

	OMP_type_list	*legal_types;
	OM_string		class;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;
	xds_extension	**adr_obj;
	OM_sint			n_cur, i;

	switch (OMP_PACKAGE(((OMP_private_object * )(OMP_INTERNAL(original)))->object_type)) {
	case XDS_SERVICE_PACKAGE:
		legal_types = legal_types_ctx;
		class = dsP_DS_C_CONTEXT;
		break;
	case XDS_GDS_PACKAGE:
		legal_types = legal_types_cgx;
		class = dsP_DSX_C_GDS_CONTEXT;
		break;
	} /* endswitch */


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(class);
				break;
			case DS_EXT	:
				if (!xobject) {
					break;
				} /* endif */
				if (!initialize) {
					GET_NB_OBJ(xds_extension, d2_next_ext, pt_l->max_val,
					(void *) &	xobject->d2_extension, adr_obj);
					UPDATE_LIMIT_VALUE(pt_l->max_val);
					JUMP_N_OBJ(xds_extension, d2_next_ext, initial_value,
					(void *) &	xobject->d2_extension, adr_obj, n_cur);
					pt_l->desc = adr_obj;
					nb_desc	 += pt_l->nb_val;
					break;
				} /* endif */
				adr_obj	 = pt_l->desc;
				for (i = 0;	i <		pt_l->nb_val; i++, adr_obj =
					&(*adr_obj)->d2_next_ext) {
					OBJECT_VALUE(extension, dsP_DS_C_EXT,
					(xds_extension*)*	adr_obj, DS_EXT, pt_l->max_val);
				} /* endfor */
				break;
			case DS_OPERATION_PROGRESS :
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				if (xobject->d2_op_progress.d2_nr_phase	 ==
					D2_NOT_STARTED)	 {
					if ((!OMP_EXCLUDE_VALUES) &&  (!OMP_EXCLUDE_MULTIPLES(pt_l->max_val))) {
						ptr_desc->value.object.object= DS_OPERATION_NOT_STARTED;
					} /* endif */
					ptr_desc->type = DS_OPERATION_PROGRESS;
					ptr_desc->syntax = OM_S_OBJECT;
					ptr_desc++;
				} else {
					OBJECT_VALUE(op_progress, dsP_DS_C_OPERATION_PROGRESS,
					&xobject->d2_op_progress, DS_OPERATION_PROGRESS, 1)
				} /* endif */
				break;
			case DS_ALIASED_RDNS :
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_INT(ptr_desc, DS_ALIASED_RDNS, xobject->d2_al_rdn,
				1);
				break;
			case DS_CHAINING_PROHIB	:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DS_CHAINING_PROHIB,
				(xobject->d2_serv_cntrls&D2_NOCHAINING ? OM_TRUE :OM_FALSE),1);
				break;
			case DS_DONT_DEREFERENCE_ALIASES :
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DS_DONT_DEREFERENCE_ALIASES,
				(xobject->d2_serv_cntrls&D2_DREFALIAS ? OM_TRUE	: OM_FALSE), 1);
				break;
			case DS_DONT_USE_COPY :
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DS_DONT_USE_COPY,
				(xobject->d2_serv_cntrls & D2_NOCACHE ? OM_TRUE	:OM_FALSE), 1);
				break;
			case DS_LOCAL_SCOPE	:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DS_LOCAL_SCOPE,
				(xobject->d2_serv_cntrls&D2_LOCALSCOPE? OM_TRUE : OM_FALSE), 1);
				break;
			case DS_PREFER_CHAINING	:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DS_PREFER_CHAINING,
				(xobject->d2_serv_cntrls&D2_PREF_CHAINING ? OM_TRUE : OM_FALSE),1);
				break;
			case DS_PRIORITY :
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_ENUM(ptr_desc, DS_PRIORITY,xobject->d2_priority, 1);
				break;
			case DS_SCOPE_OF_REFERRAL :
				if ((!xobject) || (xobject->d2_scope_ref ==
					D2_WORLD_SCOPE)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_ENUM(ptr_desc, DS_SCOPE_OF_REFERRAL,
				xobject->d2_scope_ref, 1);
				break;
			case DS_SIZE_LIMIT :
				if ((!xobject) || (xobject->d2_size_limit ==
					D2_S_UNLIMITED)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_INT(ptr_desc, DS_SIZE_LIMIT, xobject->d2_size_limit,
				1);
				break;
			case DS_TIME_LIMIT :
				if ((!xobject) || (xobject->d2_time_limit ==
					D2_T_UNLIMITED)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_INT(ptr_desc, DS_TIME_LIMIT, xobject->d2_time_limit,
				1);
				break;
			case DS_ASYNCHRONOUS :
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DS_ASYNCHRONOUS , OM_FALSE , 1);
				break;
			case DS_AUTOMATIC_CONTINUATION :
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DS_AUTOMATIC_CONTINUATION,
				(xobject->d2_serv_cntrls&D2_NOREFERRAL ? OM_TRUE : OM_FALSE),1);
				break;
			case DSX_DUAFIRST :
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DSX_DUAFIRST,
				(xobject->d2_serv_cntrls & D2_DUAFIRST ? OM_TRUE :OM_FALSE), 1);
				break;
			case DSX_DONT_STORE	:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DSX_DONT_STORE ,
				(xobject->d2_serv_cntrls &D2_DONT_STORE ? OM_TRUE:OM_FALSE), 1);
				break;
			case DSX_NORMAL_CLASS :
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DSX_NORMAL_CLASS,
				(xobject->d2_serv_cntrls&D2_NORMAL_CLASS ?OM_TRUE :OM_FALSE),1);
				break;
			case DSX_PRIV_CLASS	:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DSX_PRIV_CLASS,
				(xobject->d2_serv_cntrls & D2_PRIVILEGED_CLASS ? OM_TRUE 
				:OM_FALSE) , 1);
				break;
			case DSX_RESIDENT_CLASS	:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DSX_RESIDENT_CLASS ,
				(xobject->d2_serv_cntrls&D2_RESIDENT_CLASS ? OM_TRUE 
				:OM_FALSE) , 1);
				break;
			case DSX_USEDSA	:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DSX_USEDSA ,
				(xobject->d2_serv_cntrls &	D2_USEDSA ? OM_TRUE : OM_FALSE), 1);
				break;
			case DSX_DUA_CACHE :
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DSX_DUA_CACHE ,
				(xobject->d2_serv_cntrls &D2_DUACACHE? OM_TRUE : OM_FALSE) , 1);
				break;
			case DSX_PREFER_ADM_FUNCS :
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DSX_PREFER_ADM_FUNCS	,
				(xobject->d2_serv_cntrls &D2_PREF_ADM_FUN?OM_TRUE:OM_FALSE), 1);
				break;
			case DSX_SIGN_MECHANISM :
			case DSX_PROT_REQUEST :
				break;  /* for future use */
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_context */


/********************************************************/
/*							*/
/*	get	ENTRY INFORMATION object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_entry_info(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_entry_info		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = E_I_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;
	xds_attr_list	**adr_obj;
	OM_sint			n_cur, i;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_ENTRY_INFO);
				break;
			case DS_FROM_ENTRY:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DS_FROM_ENTRY,
				xobject->d2_ei_fromentry, 1);
				break;
			case DS_OBJECT_NAME:
				if ((!xobject) ||  (!xobject->d2_ei_entry)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(dn, dsP_DS_C_DS_DN,
				(xds_dist_name	)xobject->d2_ei_entry, DS_OBJECT_NAME, 1);
				break;
			case DS_ATTRIBUTES:
				if (!xobject) {
					break;
				} /* endif */
				if (!initialize) {
					GET_NB_OBJ(xds_attribute, d2_a_next, pt_l->max_val,
					(xds_attribute **) &  xobject->d2_ei_ai, adr_obj);
					UPDATE_LIMIT_VALUE(pt_l->max_val);
					JUMP_N_OBJ(xds_attribute, d2_a_next, initial_value,
					&xobject->d2_ei_ai, adr_obj, n_cur);
					pt_l->desc = adr_obj;
					nb_desc	 += pt_l->nb_val;
					break;
				} /* endif */

				adr_obj	 = pt_l->desc;
				for (i = 0;	i < pt_l->nb_val; i++,
				adr_obj = &(*adr_obj)->d2_a_next)	{

					OBJECT_VALUE(attribute, dsP_DS_C_ATTRIBUTE,
					(xds_attribute*) *	adr_obj, DS_ATTRIBUTES, pt_l->max_val);
				} /* endfor */
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_entry_info	*/


/********************************************************/
/*							*/
/*	get	ENTRY INFORMATION SELECTION	 object		*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_entry_info_sel(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_entry_info_sel	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = EIS_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;
	D2_a_type		*adr_obj;
	OM_sint 		i;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */


			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_ENTRY_INFO_SELECTION);
				break;
			case DS_ALL_ATTRIBUTES:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DS_ALL_ATTRIBUTES,
				(xobject->d2_r_no_at == D2_ALL_ATT_REQ?OM_TRUE :OM_FALSE), 1);
				break;
			case DS_ATTRIBUTES_SELECTED:
				if ((!xobject) ||  (xobject->d2_r_no_at	 ==
					D2_ALL_ATT_REQ)	 ||		(!(pt_l->max_val =
					xobject->d2_r_no_at))) {
					break;
				} /* endif */
				if (!initialize) {
					UPDATE_LIMIT_VALUE(pt_l->max_val);
					pt_l->desc = (D2_a_type	*) (xobject->d2_r_type_at)
					+ initial_value;
					nb_desc	 += pt_l->nb_val;
					break;
				} /* endif */
				for (i = 0, adr_obj = pt_l->desc; i < pt_l->nb_val;
				i++, adr_obj++) {
					UPDATE_DESC_STR(ptr_desc, DS_ATTRIBUTES_SELECTED,
					OM_S_OBJECT_IDENTIFIER_STRING, adr_obj->d2_typ_len,
					adr_obj->d2_type, 1);
				} /* endfor */
				break;
			case DS_INFO_TYPE:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_ENUM(ptr_desc, DS_INFO_TYPE, xobject->d2_r_val_at,
				1);
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;

end:
	return(ret);
} /* end dsP_get_entry_info_sel	*/


/********************************************************/
/*							*/
/*	get	ENTRY MODIFICATION object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_entry_mod(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_entry_mod		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = E_M_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;

	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_ENTRY_MOD);

				break;
			case DS_MOD_TYPE:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_ENUM(ptr_desc, DS_MOD_TYPE, xobject->d2_m_mod, 1);
				break;
			case DS_ATTRIBUTE_TYPE:
				if ((!xobject) ||  (!xobject->d2_m_attr.d2_a_type.d2_type)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DS_ATTRIBUTE_TYPE,
				OM_S_OBJECT_IDENTIFIER_STRING,
				xobject->d2_m_attr.d2_a_type.d2_typ_len,
				xobject->d2_m_attr.d2_a_type.d2_type, 1);
				break;
			case DS_ATTRIBUTE_VALUES:
				if ((!xobject) ||  (!(pt_l->max_val	 = xobject->d2_m_attr.d2_a_no_val))) {
					break;
				} /* endif */
				if (!initialize) {
					UPDATE_LIMIT_VALUE(pt_l->max_val);
					pt_l->desc = (D2_a_value *) (xobject->d2_m_attr.d2_a_val)
					+ initial_value;
					nb_desc	 += pt_l->nb_val;
					break;
				} /* endif */
				if (pt_l->nb_val) {
					ret	 = dsP_update_desc_value(original, &ptr_desc,
					pt_l->desc, exclusions, local_strings, pt_l->nb_val,
					pt_l->max_val, DS_ATTRIBUTE_VALUES);
				} /* endif */
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;

end:
	return(ret);
} /* end dsP_get_entry_mod */


/********************************************************/
/*							*/
/*	get	ENTRY MODIFICATION LIST	object		*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_entry_mod_list(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_entry_mod_list	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = EML_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;
	xds_entry_mod	**adr_obj;
	OM_sint			n_cur, i;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_ENTRY_MOD_LIST);
				break;
			case DS_CHANGES:
				if (!xobject) {
					break;
				} /* endif */
				if (!initialize) {
					GET_NB_OBJ(xds_entry_mod, d2_m_next, pt_l->max_val,
					(xds_entry_mod **) &  xobject, adr_obj);
					UPDATE_LIMIT_VALUE(pt_l->max_val);
					JUMP_N_OBJ(xds_entry_mod, d2_m_next, initial_value,
					&xobject, adr_obj, n_cur);
					pt_l->desc = adr_obj;
					nb_desc	 += pt_l->nb_val;
					break;
				} /* endif */

				adr_obj	 = pt_l->desc;
				for (i = 0;	i <		pt_l->nb_val; i++, adr_obj =
					&(*adr_obj)->d2_m_next)	{

					OBJECT_VALUE(entry_mod, dsP_DS_C_ENTRY_MOD,
					(xds_entry_mod *)*adr_obj, DS_CHANGES, pt_l->max_val);
				} /* endfor */
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;

end:
	return(ret);
} /* end dsP_get_entry_mod_list	*/


/********************************************************/
/*							*/
/*	get	FILTER ITEM	object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_fi_item(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_fi_item			*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = F_I_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_FILTER_ITEM);
				break;
			case DS_FILTER_ITEM_TYPE :
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_ENUM(ptr_desc, DS_FILTER_ITEM_TYPE,
				xobject->match_type, 1);
				break;
			case DS_FINAL_SUBSTRING	:
				if ((!xobject) 
				|| ((void *)xobject->f_lst_sstr.d2_at_value.d2_oct_a_v== NULL)){
					break;
				} /* endif */
				INC_NBDESC_MONO;
				ret	 = dsP_update_desc_value(original, &ptr_desc,
				&xobject->f_lst_sstr, exclusions,local_strings, 1, 1,
				DS_FINAL_SUBSTRING);
				break;
			case DS_INITIAL_SUBSTRING :
				if ((!xobject) ||  
				((void *)xobject->f_ini_sstr.d2_at_value.d2_oct_a_v	 == NULL)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				ret	 = dsP_update_desc_value(original, &ptr_desc,
				&xobject->f_ini_sstr, exclusions,local_strings, 1, 1,
				DS_INITIAL_SUBSTRING);
				break;
			case DS_ATTRIBUTE_TYPE:
				if ((!xobject) || (!xobject->f_item_att.d2_a_type.d2_type)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DS_ATTRIBUTE_TYPE,
				OM_S_OBJECT_IDENTIFIER_STRING,
				xobject->f_item_att.d2_a_type.d2_typ_len,
				xobject->f_item_att.d2_a_type.d2_type, 1);
				break;
			case DS_ATTRIBUTE_VALUES:
				if ((!xobject) ||  
				(!(pt_l->max_val = xobject->f_item_att.d2_a_no_val))) {
					break;
				} /* endif */
				if (!initialize) {
					UPDATE_LIMIT_VALUE(pt_l->max_val);
					pt_l->desc = (D2_a_value *	)(xobject->f_item_att.d2_a_val)
					 + initial_value;
					nb_desc	 += pt_l->nb_val;
					break;
				} /* endif */
				ret	 = dsP_update_desc_value(original, &ptr_desc, pt_l->desc,
				exclusions, local_strings, pt_l->nb_val, pt_l->max_val,
				DS_ATTRIBUTE_VALUES);
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;

end:
	return(ret);
} /* end dsP_get_fi_item */


/********************************************************/
/*							*/
/*	get	FILTER object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_filter(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_filter			*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = F_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;
	xds_filter		**adr_obj;
	xds_fi_item		**adr_obji;
	OM_sint			n_cur, i;
	OM_value_position	init_val;
	OM_value_position	limit_val;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			init_val = initial_value;
			limit_val = limiting_value;

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_FILTER);
				break;
			case DS_FILTER_ITEMS :
				if ((!xobject) || (!xobject->fil_item)) {
					break;
				} /* endif */
				if (!initialize) {
					GET_NB_OBJ(xds_fi_item, f_item_next, pt_l->max_val,
					(xds_fi_item **) & xobject->fil_item, adr_obji);
					UPDATE_LIMIT_VALUE(pt_l->max_val);
					JUMP_N_OBJ(xds_fi_item, f_item_next, initial_value,
					&xobject->fil_item, adr_obji, n_cur);
					pt_l->desc = adr_obji;
					nb_desc	 += pt_l->nb_val;
					break;
				} /* endif */
				adr_obji = pt_l->desc;
				for (i = 0;	i <		pt_l->nb_val; i++, adr_obji =
					&(*adr_obji)->f_item_next) {
					OBJECT_VALUE(fi_item, dsP_DS_C_FILTER_ITEM,
					(xds_fi_item *) *adr_obji, DS_FILTER_ITEMS, pt_l->max_val);
				} /* endfor */
				break;
			case DS_FILTERS	:
				if ((!xobject) || (!xobject->filter_first)) {
					break;
				} /* endif */
				if (!initialize) {
					GET_NB_OBJ(xds_filter, filter_next, pt_l->max_val,
					(xds_filter	**) & xobject->filter_first, adr_obj);
					UPDATE_LIMIT_VALUE(pt_l->max_val);
					JUMP_N_OBJ(xds_filter, filter_next, initial_value,
					&xobject->filter_first, adr_obj, n_cur);
					pt_l->desc = adr_obj;
					nb_desc	 += pt_l->nb_val;
					break;
				} /* endif */
				adr_obj	 = pt_l->desc;
				for (i = 0;	i <		pt_l->nb_val; i++, adr_obj =
				&(*adr_obj)->filter_next) {
					OBJECT_VALUE(filter, dsP_DS_C_FILTER,
					(xds_filter *)*adr_obj, DS_FILTERS, pt_l->max_val);
				} /* endfor */
				break;
			case DS_FILTER_TYPE	:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_ENUM(ptr_desc, DS_FILTER_TYPE, xobject->fi_type, 1);
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_filter	*/


/********************************************************/
/*							*/
/*	get	READ RESULT	object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_read_result(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_read_result		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = READ_RES_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_READ_RESULT);
				break;
			case DS_ENTRY:
				if ((!xobject) ||  (!xobject->read_info)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(entry_info, dsP_DS_C_ENTRY_INFO,
				(xds_entry_info * )xobject->read_info, DS_ENTRY, 1);
				break;
				/* Common Results	*/
			case DS_ALIAS_DEREFERENCED:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DS_ALIAS_DEREFERENCED,
				xobject->com_res->d2_al_deref, 1);
				break;
			case DS_PERFORMER:
				if ((!xobject) ||  (!xobject->com_res->d2_performer)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(dn, dsP_DS_C_DS_DN,
				(xds_dist_name)xobject->com_res->d2_performer, DS_PERFORMER, 1);
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_read_result */


/********************************************************/
/*							*/
/*	get	SEARCH RESULT object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_search_result(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_search_result	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = SRCH_RES_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_SEARCH_RESULT);
				break;
			case DS_SEARCH_INFO:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(search_info, dsP_DS_C_SEARCH_INFO,
				(xds_search_info *)xobject, DS_SEARCH_INFO, 1);
				break;
			case DS_UNCORRELATED_SEARCH_INFO:
				pt_l->max_val = 0;
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_search_result */


/********************************************************/
/*							*/
/*	get	SEARCH INFO	object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_search_info(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_search_info		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = SRCH_INF_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;
	xds_entry_info	**adr_obj;
	OM_sint			n_cur, i;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_SEARCH_INFO);

				break;
			case DS_ENTRIES:
				if (!xobject) {
					break;
				} /* endif */
				if (!initialize) {
					GET_NB_OBJ(xds_entry_info, d2_ei_next, pt_l->max_val,
					(xds_entry_info	* *) &  xobject->search_entries, adr_obj);
					UPDATE_LIMIT_VALUE(pt_l->max_val);
					JUMP_N_OBJ(xds_entry_info, d2_ei_next, initial_value,
					&xobject->search_entries, adr_obj, n_cur);
					pt_l->desc = adr_obj;
					nb_desc	 += pt_l->nb_val;
					break;
				} /* endif */

				adr_obj	 = pt_l->desc;
				for (i = 0;	i <		pt_l->nb_val; i++, adr_obj =
					&(*adr_obj)->d2_ei_next)	 {

					OBJECT_VALUE(entry_info, dsP_DS_C_ENTRY_INFO,
					(xds_entry_info *) *adr_obj, DS_ENTRIES, pt_l->max_val);
				} /* endfor */
				break;
			case DS_OBJECT_NAME:
				if ((!xobject)	 ||	(!xobject->b_object)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(dn, dsP_DS_C_DS_DN,
				(xds_dist_name	)xobject->b_object, DS_OBJECT_NAME, 1);

				break;
			case DS_PARTIAL_OUTCOME_QUAL:
				if ((!xobject) ||  (!xobject->s_part_oq)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(part_oq, dsP_DS_C_PARTIAL_OUTCOME_QUAL,
				(xds_part_oq *)xobject->s_part_oq, DS_PARTIAL_OUTCOME_QUAL, 1);

				break;
				/* Common Results	*/
			case DS_ALIAS_DEREFERENCED:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DS_ALIAS_DEREFERENCED,
				xobject->com_res->d2_al_deref, 1);
				break;
			case DS_PERFORMER:
				if ((!xobject) ||  (!xobject->com_res->d2_performer)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(dn, dsP_DS_C_DS_DN,
				(xds_dist_name)xobject->com_res->d2_performer, DS_PERFORMER, 1);

				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_search_info */


/********************************************************/
/*							*/
/*	get	SESSION	object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_session(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_session			*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types_ses[] = SES_TYPE_LIST;
	OMP_type_list	legal_types_sgs[] = SGS_TYPE_LIST;
	OMP_type_list	*legal_types;
	OM_string		class;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;

	switch (OMP_PACKAGE(((OMP_private_object * )(OMP_INTERNAL(original)))->object_type)) {
	case XDS_SERVICE_PACKAGE:
		legal_types = legal_types_ses;
		class = dsP_DS_C_SESSION;
		break;
	case XDS_GDS_PACKAGE:
		legal_types = legal_types_sgs;
		class = dsP_DSX_C_GDS_SESSION;
		break;
	} /* endswitch */


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(class);
				break;
			case DS_DSA_ADDRESS	:
				if ((!xobject) || (!xobject->dsa_address)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(psap, dsP_DS_C_PRESENTATION_ADDRESS,
				xobject->dsa_address, DS_DSA_ADDRESS, 1)
				break;
			case DS_DSA_NAME :
				if ((!xobject) || (!xobject->dsa_name)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(dn, dsP_DS_C_DS_DN, xobject->dsa_name,
				DS_DSA_NAME, 1)
				break;
			case DS_FILE_DESCRIPTOR	:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_INT(ptr_desc, DS_FILE_DESCRIPTOR,
				DS_NO_VALID_FILE_DESCRIPTOR, 1);
				break;
			case DS_REQUESTOR :
				if ((!xobject) || (!xobject->requestor)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(dn, dsP_DS_C_DS_DN, xobject->requestor,
				DS_REQUESTOR, 1)
				break;
			case DSX_PASSWORD :
				if ((!xobject) || (!xobject->password.d2_p_pw)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DSX_PASSWORD, OM_S_OCTET_STRING,
				xobject->password.d2_p_len, xobject->password.d2_p_pw, 1);
				break;
			case DSX_DIR_ID	:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_INT(ptr_desc, DSX_DIR_ID , xobject->dir_id, 1);
				break;
			case DSX_AUTH_MECHANISM	:
				if ((!xobject) || (xobject->auth_mech == D2_ANONYMOUS)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_ENUM(ptr_desc,DSX_AUTH_MECHANISM,xobject->auth_mech,
				1);
				break;
			case DSX_AUTH_INFO	:
				if((!xobject) || (!xobject->auth_info.d2_value)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DSX_AUTH_INFO, OM_S_OCTET_STRING,
				xobject->auth_info.d2_size, xobject->auth_info.d2_value, 1);

			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_session */


/********************************************************/
/*							*/
/*	get	ERROR (DS_problem) object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_error(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_error			*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = ERR_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(OMP_CLASS_ID_STR(original));
				break;
			case DS_PROBLEM:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_ENUM(ptr_desc, DS_PROBLEM, xobject->val, 1);
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_error */


/********************************************************/
/*							*/
/*	get	ERROR NAME object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_n_error(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_error			*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = N_ERR_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_NAME_ERROR);

				break;
			case DS_MATCHED:
				if ((!xobject)
				|| (!xobject->ptr) 
				||  (!xobject->ptr->d2_e_problem.d2_e_nmproblem)
				|| (!xobject->ptr->d2_e_problem.d2_e_nmproblem->d2_ne_match)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(dn, dsP_DS_C_DS_DN,
				xobject->ptr->d2_e_problem.d2_e_nmproblem->d2_ne_match,
				DS_MATCHED, 1)
				break;
			case DS_PROBLEM:
				if ((!xobject) || (!xobject->ptr) ||  (!xobject->ptr->d2_e_problem.d2_e_nmproblem)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_ENUM(ptr_desc, DS_PROBLEM,
				xobject->ptr->d2_e_problem.d2_e_nmproblem->d2_ne_prob, 1);
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_n_error */


/********************************************************/
/*							*/
/*	get	ATTRIBUTE ERROR	object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_a_error(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_error			*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = A_ERR_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;
	 D2_a_problem	**adr_obj;
	OM_sint			n_cur, i;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_ATTRIBUTE_ERROR);
				break;
			case DS_OBJECT_NAME:
				if ((!xobject) 
				|| (!xobject->ptr) 
				||  (!xobject->ptr->d2_e_problem.d2_e_atproblem) 
				|| (!xobject->ptr->d2_e_problem.d2_e_atproblem->d2_ae_name)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(dn, dsP_DS_C_DS_DN,
				xobject->ptr->d2_e_problem.d2_e_atproblem->d2_ae_name,
				DS_OBJECT_NAME, 1)
				break;
			case DS_PROBLEMS:
				if ((!xobject) 
				|| (!xobject->ptr) 
				||  (!xobject->ptr->d2_e_problem.d2_e_atproblem)	
				|| (!xobject->ptr->d2_e_problem.d2_e_atproblem->d2_ae_prob)) {
					break;
				} /* endif */
				if (!initialize) {
					GET_NB_OBJ(D2_a_problem, d2_ap_next, pt_l->max_val,
					(D2_a_problem**)&xobject->ptr->d2_e_problem.d2_e_atproblem
					->d2_ae_prob, adr_obj);
					UPDATE_LIMIT_VALUE(pt_l->max_val);
					JUMP_N_OBJ(D2_a_problem, d2_ap_next, initial_value,
					(D2_a_problem**) &xobject->ptr->
					d2_e_problem.d2_e_atproblem->d2_ae_prob, adr_obj, n_cur);
					pt_l->desc = adr_obj;
					nb_desc	 += pt_l->nb_val;
					break;
				} /* endif */
				adr_obj	 = pt_l->desc;
				for (i = 0;	i <		pt_l->nb_val; i++, adr_obj =
					&(*adr_obj)->d2_ap_next)	 {

					OBJECT_VALUE(ap_error, dsP_DS_C_ATTRIBUTE_PROBLEM,
					(D2_a_problem * ) * adr_obj, DS_PROBLEMS, pt_l->max_val);
				} /* endfor */
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_a_error */


/********************************************************/
/*							*/
/*	get	REFERRAL object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_referral(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_error			*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OM_return_code	ret	 = OM_SUCCESS;
	xds_referral	*adr_obj;

	if ((!xobject)
	|| (!xobject->ptr)
	||  (!xobject->ptr->d2_e_problem.d2_e_referral)) {
		adr_obj	 = NULL;
	} else {
		adr_obj	 = xobject->ptr->d2_e_problem.d2_e_referral;
	} /* endif */
	ret = dsP_get_cont_ref(workspace, original, adr_obj,
	exclusions, included_types, local_strings, initial_value,
	limiting_value, copy, total_number);

	return(ret);
} /* end dsP_get_referral */


/********************************************************/
/*							*/
/*	get	ATTRIBUTE PROBLEM object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_ap_error(
	OMX_workspace		workspace,
	OM_private_object	original,
	D2_a_problem		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = A_PB_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_ATTRIBUTE_PROBLEM);
				break;
			case DS_ATTRIBUTE_TYPE:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DS_ATTRIBUTE_TYPE,
				OM_S_OBJECT_IDENTIFIER_STRING,
				xobject->d2_ap_info.d2_a_type.d2_typ_len,
				xobject->d2_ap_info.d2_a_type.d2_type, 1);
				break;
			case DS_ATTRIBUTE_VALUE:
				if ((!xobject) || (!xobject->d2_ap_info.d2_a_no_val)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				ret	 = dsP_update_desc_value(original, &ptr_desc,
				xobject->d2_ap_info.d2_a_val, exclusions, local_strings, 1, 1,
				DS_ATTRIBUTE_VALUE);
				break;
			case DS_PROBLEM:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_ENUM(ptr_desc, DS_PROBLEM, xobject->d2_ap_prob, 1);
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_ap_error */


/********************************************************/
/*							*/
/*	get	ACCESS POINT object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_acc_point(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_acc_point		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = ACC_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_ACCESS_POINT);

				break;
			case DS_ADDRESS:
				if ((!xobject) ||  (!xobject->d2_adr_ap)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(psap, dsP_DS_C_PRESENTATION_ADDRESS,
				(xds_psap )xobject->d2_adr_ap, DS_ADDRESS, 1);
				break;
			case DS_AE_TITLE:
				if ((!xobject) ||  (!xobject->d2_dsa_ap)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(dn, dsP_DS_C_DS_DN,
				(xds_dist_name	)xobject->d2_dsa_ap, DS_AE_TITLE, 1);
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_acc_point */


/********************************************************/
/*							*/
/*	get	OPERATION PROGRESS object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_op_progress(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_op_progress		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = OP_PROG_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;

	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_OPERATION_PROGRESS);

				break;
			case DS_NAME_RESOLUTION_PHASE:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_ENUM(ptr_desc, DS_NAME_RESOLUTION_PHASE,
				xobject->d2_nr_phase, 1);
				break;
			case DS_NEXT_RDN_TO_BE_RESOLVED:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_INT(ptr_desc, DS_NEXT_RDN_TO_BE_RESOLVED,
				xobject->d2_next_rdn, 1);
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_op_progress */


/********************************************************/
/*							*/
/*	get	PARTIAL	OUTCOME	QUALIFIER object		*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_part_oq(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_part_oq			*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{

	OMP_type_list	legal_types[] = PART_OUT_Q_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES;
			pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_PARTIAL_OUTCOME_QUAL);
				break;
			case DS_LIMIT_PROBLEM:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_ENUM(ptr_desc, DS_LIMIT_PROBLEM,
				xobject->limit_prob, 1);
				break;
			case DS_UNAVAILABLE_CRIT_EXT:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, DS_UNAVAILABLE_CRIT_EXT,
				xobject->unav_crit_ext, 1);
				break;
			case DS_UNEXPLORED:
				if ((!xobject) || (!xobject->cont_ref)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(cont_ref, dsP_DS_C_CONTINUATION_REF,
				(xds_cont_ref * )xobject->cont_ref, DS_UNEXPLORED, 1);
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endwhile */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_part_oq */


/********************************************************/
/*							*/
/*	get	CONTINUATION REFERENCE object		*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_cont_ref(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_cont_ref		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = CONT_REF_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;
	xds_acc_point	**adr_obj;
	OM_sint			n_cur, i;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				if (OMP_CLASS(OMP_OBJ_TYPE(original)) ==
					XDS_REFERRAL) {
					UPDATE_DESC_CLASS(dsP_DS_C_REFERRAL);
				} else {
					UPDATE_DESC_CLASS(dsP_DS_C_CONTINUATION_REF);
				} /* endif */
				break;
			case DS_ACCESS_POINTS:
				if (!xobject) {
					break;
				} /* endif */
				if (!initialize) {
					GET_NB_OBJ(xds_acc_point, d2_next_ap, pt_l->max_val,
					(xds_acc_point**) &  xobject->d2_acc_point, adr_obj);
					UPDATE_LIMIT_VALUE(pt_l->max_val);
					JUMP_N_OBJ(xds_acc_point, d2_next_ap, initial_value,
					&xobject->d2_acc_point, adr_obj, n_cur);
					pt_l->desc = adr_obj;
					nb_desc	 += pt_l->nb_val;
					break;
				} /* endif */

				adr_obj	 = pt_l->desc;
				for (i = 0;	i <	pt_l->nb_val;
				i++, adr_obj = &(*adr_obj)->d2_next_ap)	 {
					OBJECT_VALUE(acc_point, dsP_DS_C_ACCESS_POINT,
					(xds_acc_point *)*adr_obj, DS_ACCESS_POINTS, pt_l->max_val);
				} /* endfor */
				break;
			case DS_ALIASED_RDNS:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_INT(ptr_desc, DS_ALIASED_RDNS,
				xobject->d2_al_rdns, 1);
				break;
			case DS_OPERATION_PROGRESS:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(op_progress, dsP_DS_C_OPERATION_PROGRESS,
				(xds_op_progress *) &xobject->d2_op_progr,
				DS_OPERATION_PROGRESS, 1);
				break;
			case DS_RDNS_RESOLVED:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_INT(ptr_desc, DS_RDNS_RESOLVED,
				xobject->d2_rdn_res, 1);
				break;
			case DS_TARGET_OBJECT:
				if ((!xobject) ||  (!xobject->d2_target)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(dn, dsP_DS_C_DS_DN,
				(xds_dist_name	)xobject->d2_target, DS_TARGET_OBJECT, 1);
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_cont_ref */


/********************************************************/
/*							*/
/*	get	ACL_ITEM object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_aci(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_acl_item		xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = ACI_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	* desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;
	Aci_info		aci;

	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	if (xobject) {
		aci.ai_len = strlen((char *)xobject) + 1;	/* aci string length */
		aci.ai_inter = *((char *)xobject);	/* IAPL	INTERPRETATION */
		aci.ai_p = (char *)xobject + 1;		/* dname pointer */
	} /* endif */
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DSX_C_GDS_ACL_ITEM);

				break;
			case DSX_INTERPRETATION:
				if ((!xobject) || (aci.ai_inter	 ==  D2_IGNORE))	 {
					pt_l->max_val = 0;
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_ENUM(ptr_desc, DSX_INTERPRETATION,
				(OM_enumeration)(IAPL_TO_DSX_INTER(aci.ai_inter)), 1);
				break;
			case DSX_USER:
				if ((!xobject) || (aci.ai_inter	 ==  D2_IGNORE))	 {
					pt_l->max_val = 0;
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(dn, dsP_DS_C_DS_DN,
				(xds_dist_name	) & (aci.ai_p[0]), DSX_USER, 1);
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_aci */


/********************************************************/
/*							*/
/*	get	ACL	object					*/
/*							*/
/********************************************************/
static OM_return_code
dsP_get_acl(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_acl				xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = ACL_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc		 = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;
	Acr_info		acl[D2_NOACCESS_RIGHTS];
	Acr_info		*aclp;
	Aci_info		*aci_ptr;
	OM_sint			nb_val_d, acl_len;
	OM_sint			i;

	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	if (xobject) {
		aclp = acl;
		if ((ret = dsP_decode_acl(&aclp, xobject, &nb_val_d, &acl_len))	 
		!= OM_SUCCESS) {
			return (ret);
		} /* endif */
	} /* endif */
	while (1) {
		for (pt_l = legal_types;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			aclp = acl;

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DSX_C_GDS_ACL);

				break;
			case DSX_MODIFY_SENSITIVE:
				aclp++;
			case DSX_READ_SENSITIVE:
				aclp++;
			case DSX_MODIFY_STANDARD:
				aclp++;
			case DSX_READ_STANDARD:
				aclp++;
			case DSX_MODIFY_PUBLIC:
				if ((!xobject) || (!(pt_l->max_val =  aclp->ar_val_nb))) {
					break;
				} /* endif */
				if (!initialize) {
					UPDATE_LIMIT_VALUE(aclp->ar_val_nb);
					pt_l->desc = &(aclp->ar_info[initial_value]);
					nb_desc	 += pt_l->nb_val;
					break;
				} /* endif */
				aci_ptr	 = (Aci_info *	)pt_l->desc;
				for (i = 0;	i <		pt_l->nb_val; i++, aci_ptr++)	 {
					OBJECT_VALUE(aci, dsP_DSX_C_GDS_ACL_ITEM,
					(xds_acl_item)(aci_ptr->ai_p - 1), pt_l->type, nb_val_d);
				} /* endfor */
				break;
			} /* endswitch */
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_acl */


static OM_return_code
/********************************************************/
			dsP_get_algorithm_ident(
/********************************************************/
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_algorithm_ident	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = ALGORITHM_IDENT_TYPE_LIST;
	OM_return_code	ret = OM_SUCCESS;
	OM_sint			nb_desc = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types; pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
/* from OM class Object: */
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_ALGORITHM_IDENT);
				break;

			case DS_ALGORITHM:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DS_ALGORITHM,
				OM_S_OBJECT_IDENTIFIER_STRING, xobject->algorithm.d2_typ_len,
				xobject->algorithm.d2_type, 1);
				break;

			case DS_ALGORITHM_PARAMETERS:
				if (!xobject || !xobject->parameters) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_INT(ptr_desc, DS_ALGORITHM_PARAMETERS,
				*(xobject->parameters), 1);
				break;
			} /* endswitch */
			if ((initialize) && (ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			ret = OM_SUCCESS;
		} /* endif */

		/* allocate	the	descriptors */
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc,	&desc))	
		!= OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc + 1;
		initialize = OM_TRUE;
	} /* endwhile */
	omP_update_descriptor(original,	desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_algorithm_ident */



static OM_return_code
/********************************************************/
			dsP_get_certificate(
/********************************************************/
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_certificate		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = CERTIFICATE_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types; pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
/* from OM class Object: */
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_CERT);
				break;

/* from OM class Signature: */
			case DS_ISSUER:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(dn, dsP_DS_C_DS_DN,
				xobject->to_be_signed.issuer, DS_ISSUER, 1)
				break;
				
			case DS_SIGNATURE:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(algorithm_ident, dsP_DS_C_ALGORITHM_IDENT,
				&xobject->to_be_signed.signature, DS_SIGNATURE, 1)
				break;

			case DS_SIGNATURE_VALUE	:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DS_SIGNATURE_VALUE,
				OM_S_OCTET_STRING,
				xobject->encr_str.d2_size / 8,
				xobject->encr_str.d2_value,
				1);
				break;

/* from OM class Certificate: */
			case DS_SERIAL_NBR:
				if ((!xobject) ) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_INT(ptr_desc, DS_SERIAL_NBR,
				xobject->to_be_signed.serial_number, 1);
				break;

			case DS_SUBJECT	:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(dn, dsP_DS_C_DS_DN,
				xobject->to_be_signed.subject, DS_SUBJECT, 1)
				break;

			case DS_SUBJECT_ALGORITHM	:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(algorithm_ident, dsP_DS_C_ALGORITHM_IDENT,
				&xobject->alg, DS_SUBJECT_ALGORITHM, 1)
				break;

			case DS_SUBJECT_PUBLIC_KEY	:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DS_SUBJECT_PUBLIC_KEY,
				OM_S_BIT_STRING,
				((xobject->to_be_signed.subj_public_key_info.subject_key.d2_size
				+7) & -8) /8,
				xobject->to_be_signed.subj_public_key_info.subject_key.d2_value,
				1);
				ptr_desc[-1].value.string.length = 
				xobject->to_be_signed.subj_public_key_info.subject_key.d2_size;
				break;

			case DS_VALIDITY_NOT_AFTER :
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DS_VALIDITY_NOT_AFTER,
				OM_S_UTC_TIME_STRING, DS_VL_VALIDITY_NOT_AFTER,
				xobject->to_be_signed.validity.not_after, 1);
				break;
			case DS_VALIDITY_NOT_BEFORE :
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DS_VALIDITY_NOT_BEFORE,
				OM_S_UTC_TIME_STRING, DS_VL_VALIDITY_NOT_BEFORE,
				xobject->to_be_signed.validity.not_before, 1);
				break;
			case DS_VERSION	:
				if ((!xobject) ) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				/* values of enumerations do not correspond directly
					IAPL values start at 0, XOM at 1
					assume Vxom = Viapl + 1 */
				UPDATE_DESC_ENUM(ptr_desc, DS_VERSION,
				xobject->to_be_signed.version+1, 1);
				break;
			} /* endswitch */
			if ((initialize) && (ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc,	&desc))	!= OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc + 1;
		initialize = OM_TRUE;
	} /* endwhile */
	omP_update_descriptor(original,	desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_certificate */



static OM_return_code
/********************************************************/
			dsP_get_certificate_pair(
/********************************************************/
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_certificate_pair *xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = CERTIFICATE_PAIR_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types; pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
/* from OM class Object: */
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_CERT_PAIR);
				break;

			case DS_FORWARD:
				if (!xobject || xobject->forward == NULL) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(certificate, dsP_DS_C_CERT,
				xobject->forward, DS_FORWARD, 1)
				break;

			case DS_REVERSE:
				if (!xobject || xobject->reverse == NULL) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(certificate, dsP_DS_C_CERT,
				xobject->reverse, DS_REVERSE, 1)
				break;
			} /* endswitch */
			if ((initialize) && (ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc,	&desc))	!= OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc + 1;
		initialize = OM_TRUE;
	} /* endwhile */
	omP_update_descriptor(original,	desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_certificate_pair */

static OM_return_code
/********************************************************/
			dsP_get_certificate_sublist(
/********************************************************/
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_certificate_sublist	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = CERTIFICATE_SUBLIST_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		for (pt_l = legal_types; pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */


			switch (pt_l->type)	 {
/* from OM class Object: */
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_CERT_SUBLIST);
				break;

/* from OM class Signature: */
			case DS_ISSUER:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(dn, dsP_DS_C_DS_DN,
				xobject->to_be_signed.rev_cert->issuer, DS_ISSUER, 1)
				break;

			case DS_SIGNATURE:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(algorithm_ident, dsP_DS_C_ALGORITHM_IDENT,
				&xobject->alg, DS_SIGNATURE, 1)
				break;

			case DS_SIGNATURE_VALUE	:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DS_SIGNATURE_VALUE,
				OM_S_OCTET_STRING,
				xobject->encr_str.d2_size / 8,
				xobject->encr_str.d2_value,
				1);
				break;


			case DS_REVOC_DATE :
				{
				OM_uint			i;
				if (!initialize) {
					if ((OMP_EX_ALL_BUT_CERTAIN_VALUES)
					&& (!(OMP_REPLACE_SINGLE_VALUE))) {
						ret = OM_NOT_PRESENT;
						goto end;
					} /* endif */
					nb_desc += xobject->to_be_signed.no_rev_certs;
					break;
				} /* endif */
				for (i = 0; i < xobject->to_be_signed.no_rev_certs; i++) {
					UPDATE_DESC_STR(ptr_desc, DS_REVOC_DATE,
					OM_S_UTC_TIME_STRING, DS_VL_REVOC_DATE,
					xobject->to_be_signed.rev_cert[i].rev_date,1)
				} /* endfor */
				} /* endblock */
				break;

			case DS_SERIAL_NBRS:
				{
				OM_uint			i;
				if (!initialize) {
					if ((OMP_EX_ALL_BUT_CERTAIN_VALUES)
					&& (!(OMP_REPLACE_SINGLE_VALUE))) {
						ret = OM_NOT_PRESENT;
						goto end;
					} /* endif */
					nb_desc += xobject->to_be_signed.no_rev_certs;
					break;
				} /* endif */
				for (i = 0; i < xobject->to_be_signed.no_rev_certs; i++) {
					UPDATE_DESC_INT(ptr_desc, DS_SERIAL_NBRS,
					xobject->to_be_signed.rev_cert[i].user_cert, 1);
				} /* endfor */
				} /* endblock */
				break;

			default:
				ret	 = OM_NO_SUCH_SYNTAX;
			} /* endswitch */

			if ((initialize) && (ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		} /* endfor */
		/* if exclude_descriptors return the number of descriptors	*/
		if (initialize) {
			break;
		} /* endif */
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc,	&desc))	!= OM_SUCCESS) {
			return(ret);
		} /* endif */

		ptr_desc = desc0 = desc + 1;
		initialize = OM_TRUE;
	} /* endwhile */
	omP_update_descriptor(original,	desc0);
	*copy = desc0;
	*total_number = nb_desc;
end:
	return(ret);
} /* end dsP_get_certificate_sublist */


/********************************************************/
int compare_issuer(
/********************************************************/
	const byte *first,
	const byte *second)
{
	int result;
	if (result =strncmp((char*)first,(char*)second,
	MIN(strlen((char *)first), strlen((char *)second))) == 0) {
		int s1, s2;
		if ((s1 = strlen((char *)first)) == (s2 = strlen((char *)second))) {
			return(0);
		} else {
			return (s1 - s2);
		} /* endif */
	} else {
		return(result);
	} /* endif */
	return(strncmp((char *)first,(char *)second, MIN(strlen((char *)first), strlen((char *)second))));
} /* end compare_issuer */

/********************************************************/
int compare_signature(
/********************************************************/
	const Alg_id *first,
	const Alg_id *second)
{
	if ( first->algorithm.d2_typ_len != second->algorithm.d2_typ_len) {
		return(first->algorithm.d2_typ_len -
		second->algorithm.d2_typ_len);
	} else if (memcmp(first->algorithm.d2_type,second->algorithm.d2_type,
	first->algorithm.d2_typ_len) != 0) {
		if ( first->parameters) {
			if (second->parameters) {
				return (*(first->parameters) - *(second->parameters));
			} else {
				return(1);
			} /* endif */
		} else {
			if (second->parameters) {
				return(-1);
			} else {
				return(0);
			} /* endif */
		} /* endif */
	} else {
		return(0);
	} /* endif */
	assert(0);
} /* end compare_signature */

static int
/********************************************************/
			compare_rev_certs(
/********************************************************/
	const void *first_generic,
	const void *second_generic)
{
	const xds_certificate_subsublist *first = (const xds_certificate_subsublist *)first_generic;
	const xds_certificate_subsublist *second = (const xds_certificate_subsublist *)second_generic;
	int issuer_comparison=compare_issuer(first->issuer,second->issuer);
	if (issuer_comparison != 0) {
		return (issuer_comparison);
	} else {
		int signature_comparison = compare_signature(
		&(first->signature),&(second->signature));
		if (signature_comparison != 0) {
			return (signature_comparison);
		} else {
			return(0);
		} /* endif */
	} /* endif */
	assert(0);
} /* end compare_rev_certs */

static void
/********************************************************/
				sort_rev_certs(
/********************************************************/
	xds_certificate_subsublist *list,
	size_t	list_size)
{
	qsort((void *)list,list_size,sizeof(xds_certificate_subsublist),
	compare_rev_certs);
} /* end sort_rev_certs */


static OM_return_code
/********************************************************/
			dsP_get_certificate_list(
/********************************************************/
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_certificate_list	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean			local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object	*copy,
	OM_value_position	*total_number)
{
	OMP_type_list	legal_types[] = CERTIFICATE_LIST_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint			nb_desc = 0;
	OM_descriptor	*ptr_desc;
	OM_descriptor	*desc;
	OM_descriptor	*desc0;
	OMP_type_list 	*pt_l;
	OM_boolean 		initialize;


	VALIDATE_TYPE_LIST(legal_types, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;

	/* sort revoked certs in order to be able to */
	/* partition into sublists */
	if (xobject && xobject->to_be_signed.rev_certs &&
	xobject->to_be_signed.rev_certs->to_be_signed.no_rev_certs) {
		sort_rev_certs( xobject->to_be_signed.rev_certs->to_be_signed.rev_cert,
			xobject->to_be_signed.rev_certs->to_be_signed.no_rev_certs);
	} /* endif */

	while (1) {
		for (pt_l = legal_types; pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
/* from OM class Object: */
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_CERT_LIST);
				break;

/* from OM class Signature: */
			case DS_SIGNATURE:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(algorithm_ident, dsP_DS_C_ALGORITHM_IDENT,
				&xobject->alg, DS_SIGNATURE, 1)
				break;

			case DS_SIGNATURE_VALUE	:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DS_SIGNATURE_VALUE,
				OM_S_OCTET_STRING,
				xobject->encr_str.d2_size /8,
				xobject->encr_str.d2_value,
				1);
				break;

			case DS_ISSUER:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				OBJECT_VALUE(dn, dsP_DS_C_DS_DN,
				xobject->to_be_signed.issuer, DS_ISSUER, 1)
				break;

			case DS_LAST_UPDATE:
				if (!xobject) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DS_LAST_UPDATE,
				OM_S_UTC_TIME_STRING, DS_VL_LAST_UPDATE,
				xobject->to_be_signed.last_update, 1);
				break;

    		
            case DS_REVOKED_CERTS:
                if (!xobject || !xobject->to_be_signed.rev_certs) {
                    break;
                } /* endif */
				if (!initialize) {
					if ((OMP_EX_ALL_BUT_CERTAIN_VALUES)
					&& (!(OMP_REPLACE_SINGLE_VALUE))) {
						ret = OM_NOT_PRESENT;
						goto end;
					} /* endif */ /* nb_desc increment in loop */
				} /* endif */
                {
                OM_uint total_rev_certs = xobject->to_be_signed
                .rev_certs->to_be_signed.no_rev_certs;
                xds_certificate_subsublist *prev_cert = xobject->to_be_signed
                .rev_certs->to_be_signed.rev_cert;

				OM_uint start = 0, i;
				for (i = 0; i < total_rev_certs; ++i) {
					if (((i == total_rev_certs - 1) && (start <= i))
					|| (compare_rev_certs(
					(const void *)
					&xobject->to_be_signed.rev_certs
					->to_be_signed.rev_cert[i],
					(const void *)
					&xobject->to_be_signed.rev_certs
					->to_be_signed.rev_cert[i+1]) != 0)) {
						if (initialize) {
							xobject->to_be_signed.rev_certs->to_be_signed
							.no_rev_certs = i - start +1;
							xobject->to_be_signed.rev_certs->to_be_signed
							.rev_cert = &prev_cert[start];
							OBJECT_VALUE(certificate_sublist,
							dsP_DS_C_CERT_SUBLIST,
							xobject->to_be_signed.rev_certs,
							DS_REVOKED_CERTS, 1)
							start = i + 1;
						} else {
							nb_desc++;
						} /* endif */
					} /* endif */
				} /* endfor */

                /* restore xobject */
				if (initialize) {
					xobject->to_be_signed.rev_certs->to_be_signed.no_rev_certs 
					= total_rev_certs;
					xobject->to_be_signed.rev_certs->to_be_signed.rev_cert
					= prev_cert;
				} /* endif */
                break;
                }

            } /* endswitch */
            if ((initialize) && (ret != OM_SUCCESS)) {
                clean_before_exit(desc0, ptr_desc, nb_desc, ret);
            } /* endif */
        } /* endfor */
        /* if exclude_descriptors return the number of descriptors */
        if (initialize) {
            break;
        } /* endif */
        if (OMP_EXCLUDE_DESCRIPTORS) {
            *total_number = nb_desc;
            return(OM_SUCCESS);
        } /* endif */

        /* allocate    the    descriptors                    */
        if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc)) != OM_SUCCESS) {
            return(ret);
        } /* endif */

        ptr_desc = desc0 = desc + 1;
        initialize = OM_TRUE;
    } /* endwhile */
    omP_update_descriptor(original, desc0);
    *copy = desc0;
    *total_number = nb_desc;
end:
    return(ret);
} /* end dsP_get_certificate_list */
