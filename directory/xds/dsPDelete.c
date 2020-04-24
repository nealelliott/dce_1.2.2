/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPDelete.c,v $
 * Revision 1.1.42.2  1996/02/18  18:24:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:17  marty]
 *
 * Revision 1.1.42.1  1995/12/08  15:32:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:29  root]
 * 
 * Revision 1.1.40.5  1994/08/10  08:25:36  marrek
 * 	Fix for SAP errors, OT11467.
 * 	[1994/08/08  14:14:39  marrek]
 * 
 * Revision 1.1.40.4  1994/07/06  15:08:56  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:23:40  marrek]
 * 
 * Revision 1.1.40.3  1994/06/21  14:48:41  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:47:40  marrek]
 * 
 * Revision 1.1.40.2  1994/05/10  16:03:48  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:13:57  marrek]
 * 
 * Revision 1.1.40.1  1994/02/22  18:10:01  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:20:04  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:58:41  marrek]
 * 
 * Revision 1.1.38.1  1993/10/14  19:19:23  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:16:46  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  09:46:04  keutel]
 * 
 * Revision 1.1.2.2  1993/08/10  07:01:55  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:13:28  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:16:26  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:04:40  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:32:00  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:41:15  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:18:02  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:45:33  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsPDelete.c,v $ $Revision: 1.1.42.2 $ $Date: 1996/02/18 18:24:36 $";
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
/*	@(#)dsPDelete.c	1.30 (BULL S.A)	6/26/92	*/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <xom.h>
#include <d2dir.h>
#include <xdsiapl.h>
#include <xds.h>
#include <xdsgds.h>
#include <xomip.h>
#include <alloc.h>
#include <xdsalloc.h>
#include <xdsimport.h>
#include <xdspackage.h>
#include <dsPMdup.h> 	/* for del_dlsp(),del_OR_name()			*/
						/* del_OR_address(),del_str_list()  	*/
#include <dsPPsap.h> /* for dsP_get_psap_length() */
#include <dsPDelete.h>
#include <dsPAcl.h>		/* for dsP_get_acl_length() */


/***************************************************************/
/*		static function prototypes							*/
/***************************************************************/

static OM_return_code
dsP_del_rdn(
	OMX_workspace workspace,
	xds_dist_name xobject);

static OM_return_code
dsP_del_ava(
	OMX_workspace workspace,
	xds_ava *xobject);

static OM_return_code
dsP_del_attr_list(
	OMX_workspace	workspace,
	xds_attr_list *xobject);

static OM_return_code
dsP_del_compare_result(
	OMX_workspace workspace,
	xds_compare_result *xobject);

static OM_return_code
dsP_del_context(
	OMX_workspace workspace,
	xds_context *xobject);

static OM_return_code
dsP_del_entry_info(
	OMX_workspace workspace,
	xds_entry_info *xobject);

static OM_return_code
dsP_del_entry_info_sel(
	OMX_workspace workspace,
	xds_entry_info_sel *xobject);

static OM_return_code
dsP_del_entry_mod_list(
	OMX_workspace workspace,
	xds_entry_mod_list	*xobject);

static OM_return_code
dsP_del_filter(
	OMX_workspace workspace,
	xds_filter *xobject);

static OM_return_code
dsP_del_list_res(
	OMX_workspace workspace,
	xds_list_res *xobject);

static OM_return_code
dsP_del_read_result(
	OMX_workspace workspace,
	xds_read_result *xobject);

static OM_return_code
dsP_del_search_result(
	OMX_workspace	workspace,
	xds_search_result *xobject);

static OM_return_code
dsP_del_error(
	OMX_workspace	workspace,
	xds_error *xobject);

static OM_return_code
dsP_del_xerror(
	OMX_workspace workspace,
	xds_error *xobject);

static OM_return_code
dsP_del_acl(
	OMX_workspace workspace,
	xds_acl xobject);

static OM_return_code
dsP_del_aci(
	OMX_workspace workspace,
	xds_acl_item xobject);

static OM_return_code
dsP_del_search_guide(
	OMX_workspace workspace,
	xds_search_guide *xobject);

static OM_return_code
dsP_del_search_criterion(
	OMX_workspace workspace,
	xds_search_criterion *xobject);

static OM_return_code
dsP_del_telex_number(
	OMX_workspace workspace,
	xds_telex_number *xobject);

static OM_return_code
dsP_del_faxnumber(
	OMX_workspace	workspace,
	xds_faxnumber *xobject);

static OM_return_code
dsP_del_postal_address(
	OMX_workspace workspace,
	xds_postal_address *xobject);

OM_return_code
dsP_del_certificate(
	OMX_workspace workspace,
	Certificate *xobject);

static OM_return_code
dsP_del_certificate_list(
	OMX_workspace workspace,
	xds_certificate_list *xobject);

static OM_return_code
dsP_del_certificate_pair(
	OMX_workspace workspace,
	xds_certificate_pair *xobject);

/* not static, since used in dsPPut.c */
OM_return_code
dsP_del_certificate_sublist(
	OMX_workspace workspace,
	xds_certificate_sublist *xobject);

/* not static, since used in dsPPut.c */
OM_return_code
dsP_del_algorithm_ident(
	OMX_workspace				workspace,
	xds_algorithm_ident			*xobject);

static OM_return_code
dsP_del_ttxid(
	OMX_workspace	workspace,
	xds_ttxid *xobject);


#define	CALL_DEL(object_class)						\
	dsP_del_##object_class(	workspace, xobject)

/********************************************************/
/*	dsP_delete_specific_value()	deletes	values whose	*/
/*		syntax is specific to GDS		*/
/********************************************************/
OM_return_code
dsP_delete_specific_value(
	OMX_workspace	workspace,
	OM_descriptor	*desc_ptr)
{
	switch (OMP_SYNTAX(desc_ptr)) {
		/* GDS syntaxes	*/
#if 0 /*  VR */
	case DSX_S_INTEGER_LIST	:
		 if	(desc_ptr->value.string.elements !=	OM_ELEMENTS_UNSPECIFIED)
		OMP_XFREE_STR(workspace, desc_ptr->value.string.elements,
		(desc_ptr->value.string.length != OM_LENGTH_UNSPECIFIED)?
			desc_ptr->value.string.length:
			strlen(desc_ptr->value.string.elements));
		return(OM_SUCCESS);
#endif /* 0 VR */

	default:
		return(OM_WRONG_VALUE_SYNTAX);
	} /* endswitch */
} /* end dsP_delete_specific_value */


/********************************************************/
/*							*/
/*	dsP_delete_switch()	called by omP_delete()		*/
/*		calls for each XDS object		*/
/*		a specific dsP_del_xxxx	function		*/
/*							*/
/********************************************************/
OM_return_code
dsP_delete_switch(
	OM_object	subject)
{
	OM_return_code	ret;
	void	*xobject;
	OMP_object_type	obj_type;
	OMX_workspace	workspace =		OMP_WORKSPACE(subject);
	obj_type = OMP_OBJ_TYPE(subject);
	xobject	 = OMP_DATA(subject);

	switch (OMP_PACKAGE(obj_type))	 {

		/* Diectory	Service	Package	classes	*/
	case XDS_SERVICE_PACKAGE :
		switch (OMP_CLASS(obj_type)) {

			/* Objects created by om_create()			*/
			/*---------------------------------------------------*/
		case XDS_PRESENTATION_ADDRESS:
			ret	 = CALL_DEL(psap);
			break;
		case XDS_DS_RDN:
			ret	 = CALL_DEL(rdn);
			break;
		case XDS_DS_DN:
			ret	 = CALL_DEL(dist_name);
			break;
		case XDS_AVA:
			ret	 = CALL_DEL(ava);
			break;
		case XDS_ATTRIBUTE:
			ret	 = CALL_DEL(attribute);
			break;
		case XDS_ATTRIBUTE_LIST:
			ret	 = CALL_DEL(attr_list);
			break;
		case XDS_EXT:
			ret	 = CALL_DEL(extension);
			break;
		case XDS_CONTEXT:
			ret	 = CALL_DEL(context);
			break;
		case XDS_ENTRY_INFO:
			ret	 = CALL_DEL(entry_info);
			break;
		case XDS_ENTRY_INFO_SELECTION:
			ret	 = CALL_DEL(entry_info_sel);
			break;
		case XDS_ENTRY_MOD:
			ret	 = CALL_DEL(entry_mod);
			break;
		case XDS_ENTRY_MOD_LIST:
			ret	 = CALL_DEL(entry_mod_list);
			break;
		case XDS_FILTER:
			ret	 = CALL_DEL(filter);
			break;
		case XDS_FILTER_ITEM:
			ret	 = CALL_DEL(fi_item);
			break;
		case XDS_SESSION:
			ret	 = CALL_DEL(session);
			break;

			/* Result objects created by omP_create_handle()	*/
			/*---------------------------------------------------*/
		case XDS_COMPARE_RESULT:
			ret	 = CALL_DEL(compare_result);
			break;
		case XDS_LIST_RESULT:
			ret	 = CALL_DEL(list_res);
			break;
		case XDS_READ_RESULT:
			ret	 = CALL_DEL(read_result);
			break;
		case XDS_SEARCH_RESULT:
			ret	 = CALL_DEL(search_result);
			break;
		case XDS_ABANDON_FAILED:
		case XDS_UPDATE_ERROR:
		case XDS_SYSTEM_ERROR:
		case XDS_SERVICE_ERROR:
		case XDS_LIBRARY_ERROR:
		case XDS_SECURITY_ERROR:
		case XDS_COMMUNICATIONS_ERROR:
		case XDS_ERROR:
			ret	 = CALL_DEL(error);
			break;
		case XDS_NAME_ERROR:
		case XDS_ATTRIBUTE_ERROR:
		case XDS_ATTRIBUTE_PROBLEM:
		case XDS_CONTINUATION_REF:
		case XDS_REFERRAL:
			ret	 = CALL_DEL(xerror);
			break;

			/* Objects not allowed to created by om_create()	*/
			/*---------------------------------------------------*/
		case XDS_ACCESS_POINT :
		case XDS_OPERATION_PROGRESS:
		case XDS_PARTIAL_OUTCOME_QUAL:
		case XDS_LIST_INFO_ITEM:
		case XDS_LIST_INFO:
		case XDS_SEARCH_INFO:
		default:
			ret	 = OM_NO_SUCH_OBJECT;
		} /* endswitch */
		break;

		/* Siemens Directory Service Package classes	*/
	case XDS_GDS_PACKAGE:
		switch (OMP_CLASS(obj_type)) {
		case XDS_GDS_SESSION:
			ret	 = CALL_DEL(session);
			break;

		case XDS_GDS_CONTEXT:
			ret	 = CALL_DEL(context);
			break;

		case XDS_GDS_ACL:
			ret	 = CALL_DEL(acl);
			break;

		case XDS_GDS_ACL_ITEM:
			ret	 = CALL_DEL(aci);
			break;
		default:
			ret	 = OM_NO_SUCH_OBJECT;

		} /* endswitch */
		break;

		/* Basic Directory Contents	Package		*/
	case XDS_BDCP:
		switch (OMP_CLASS(obj_type)) {
		case XDS_FACSIMILE_PHONE_NBR:
			ret	 = CALL_DEL(faxnumber);
			break;
		case XDS_POSTAL_ADDRESS:
			ret	 = CALL_DEL(postal_address);
			break;
		case XDS_SEARCH_CRITERION:
			ret	 = CALL_DEL(search_criterion);
			break;
		case XDS_SEARCH_GUIDE :
			ret	 = CALL_DEL(search_guide);
			break;
		case XDS_TELETEX_TERM_IDENT:
			ret	 = CALL_DEL(ttxid);
			break;
		case XDS_TELEX_NBR:
			ret	 = CALL_DEL(telex_number);
			break;

		default:
			ret	 = OM_NO_SUCH_OBJECT;
		} /* endswitch */
		break;
		CASE_DELETE_MDUP_PACKAGE 
#if 0
	case XDS_MDUP:
	switch(OMP_CLASS(obj_type))	{
		case MHS_DLSP:
			ret	= CALL_DEL(dlsp);
			break;
		default:
			ret	= OM_NO_SUCH_OBJECT;
			break;
		} /* endswitch */
		break;
	case XMH_PACKAGE:
	switch(OMP_CLASS(obj_type))	{
		case XMH_OR_NAME:
		ret	= CALL_DEL(OR_name);
		break;

		case XMH_OR_ADDRESS:
		ret	= OM_NO_SUCH_OBJECT;
		break;
		default:
		ret	= OM_NO_SUCH_OBJECT;
		break;
		} /* endswitch */
		break;
#endif /* 0 */
    case XDS_SAP_PACKAGE:
        switch(OMP_CLASS(obj_type)) {
            case XDS_CERTIFICATE:
                ret = CALL_DEL(certificate);
                break;
            case XDS_ALGORITHM_IDENT:
                ret = CALL_DEL(algorithm_ident);
                break;
            case XDS_CERTIFICATE_LIST:
                ret = CALL_DEL(certificate_list);
                break;
            case XDS_CERTIFICATE_PAIR:
                ret = CALL_DEL(certificate_pair);
                break;
            case XDS_CERTIFICATE_SUBLIST:
                ret = CALL_DEL(certificate_sublist);
                break;
            case XDS_SIGNATURE:
                /* xxxx
                ret = CALL_DEL(signature);
                */
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
} /* end dsP_delete_switch */


/********************************************************/
/*							*/
/*	delete PRESENTATION	ADDRESS	object		*/
/*							*/
/********************************************************/
OM_return_code
dsP_del_psap(
	OMX_workspace workspace,
	xds_psap xobject)
{
	OM_sint	len;

	if (xobject) {
		len	 = dsP_get_psap_length(xobject);
		OMP_XFREE_STR(workspace, xobject, len);
	} /* endif */
	return(OM_SUCCESS);
} /* end dsP_del_psap */


/********************************************************/
/*							*/
/*	delete DS RDN object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_del_rdn(
	OMX_workspace workspace,
	xds_dist_name xobject)
{


	if (xobject)
		OMP_XFREE_STR(workspace, xobject, strlen(xobject));

	return(OM_SUCCESS);
} /* end dsP_del_rdn */


/********************************************************/
/*							*/
/*	delete DS DN object				*/
/*							*/
/********************************************************/
OM_return_code
dsP_del_dist_name(
	OMX_workspace	workspace,
	xds_dist_name xobject)
{
	if (xobject)
		OMP_XFREE_STR(workspace, xobject, strlen(xobject));
	return(OM_SUCCESS);
} /* end dsP_del_dist_name */


/********************************************************/
/*							*/
/*	delete D2_a_value values			*/
/*							*/
/********************************************************/
/*						*/
/*	delete	a value	(string,object..)	*/
/*						*/
void
dsP_delete_a_value(
	OMX_workspace workspace,
	D2_a_value *value_ptr)
{
	if (!value_ptr->d2_a_rep)
		return;			/* non-initialised value */
	switch (value_ptr->d2_a_rep)	 {

	case D2_DISTNAME	:
		if (value_ptr->d2_at_value.d2_oct_a_v)
			OMP_XFREE_STR(workspace,(value_ptr->d2_at_value.d2_oct_a_v),
			(value_ptr->d2_a_v_len ? value_ptr->d2_a_v_len -1 :
			strlen(value_ptr->d2_at_value.d2_oct_a_v)));
		break;
	case D2_T61		:
	case D2_OCTET	:
	case D2_PRINTABLE	:
	case D2_OBJ_IDENT	:
	case D2_NUMERIC		:
	case D2_TIME	:
	case D2_ASN1	:
	case D2_ACCL		:
	case D2_COUNTRY_STX	:
	case D2_PASSWD_STX	:
	case D2_INT_LIST	:
		if (value_ptr->d2_at_value.d2_oct_a_v)
			OMP_XFREE_STR(workspace, (value_ptr->d2_at_value.d2_oct_a_v),
			(value_ptr->d2_a_v_len ? value_ptr->d2_a_v_len 
			: strlen(value_ptr->d2_at_value.d2_oct_a_v)));
		break;
	case D2_PSAP_STX	:	/* Object Presentation Address */
		dsP_del_psap(workspace, value_ptr->d2_at_value.d2_psap_a_v);
		break;
	case D2_GUIDE	:	/* Object Search-guide (BDCP)	*/
		dsP_del_search_guide(workspace, value_ptr->d2_at_value.d2_guide_a_v);
		break;
	case D2_T61_PR_LIST	:	/* Object Postal-Address (BDCP)	*/
	case D2_POST_AD_STX	:	/* Object Postal-Address (BDCP)	*/
		dsP_del_postal_address(workspace, value_ptr->d2_at_value.d2_post_a_v);
		break;
	case D2_TLXNR_STX	:	/* Object Telex-number (BDCP)	*/
		dsP_del_telex_number(workspace, value_ptr->d2_at_value.d2_tlxnr_a_v);
		break;
	case D2_TTXID_STX	:	/* Object Teletex-Terminal-Idendifier */
		dsP_del_ttxid(workspace, value_ptr->d2_at_value.d2_ttxid_a_v);
		break;
	case D2_FAXNR_STX	:	/* Object Facsimile-Telephone (BDCP)  */
		dsP_del_faxnumber(workspace, value_ptr->d2_at_value.d2_faxnr_a_v);
		break;
    case D2_CERTIFICATE :   /* Object Certificate (SAP)       */
    case D2_CERT_PAIR   :   /* Object Certificate Pair (SAP)  */
    case D2_CERT_LIST   :   /* Object Certificate List (SAP)  */
        break;

	case D2_INTEGER	:
	case D2_ENUM		:
	case D2_BOOLEAN	:
		break;
		/*	CASE_DELETE_VALUE_MDUP_PKG	*/
#ifdef	_MDUP_PKG_
	case D2_MHS_DLSP_STX	:	/* Object DL-submit-permissions	(MDUP)*/
		dsP_del_dlsp(workspace, value_ptr->d2_at_value.d2_dl_s_p_av);
		break;
	case D2_MHS_ORNAME_STX:	/* Object OR-Name (MDUP)*/
		dsP_del_OR_name(workspace, value_ptr->d2_at_value.d2_or_name_av);
		break;
	case D2_MHS_ORADR_STX :	/* Object OR-Address (MDUP)*/
		dsP_del_OR_address(workspace, value_ptr->d2_at_value.d2_or_addr_av);
		break;
	case D2_MHS_PR_DM_STX :
		break;
#endif	/* _MDUP_PKG_ */
	default:
		omP_error("Delete D2_a_value : unknown syntax");
		break;
	} /* endswitch */
} /* end dsP_delete_a_value	*/


/*						*/
/*	delete n values	and	values array	*/
/*						*/
void
dsP_delete_values(
	OMX_workspace	workspace,
	D2_a_value *val_array,
	OM_sint	val_nb)
{
	 OM_sint	i;
	 D2_a_value	*val_ptr;

	/* if values pointer or	values number =	0 -> return	*/
	if (val_array == (D2_a_value * )0  || val_nb ==		0)
		return;

	/* free	string values		*/
	for	(i = 0, val_ptr	 = val_array; i	< val_nb; i++, val_ptr++) {
		dsP_delete_a_value(workspace, val_ptr);
	} /* endfor */
	/* free	values D2_a_value array		*/
	OMP_XFREE(workspace, val_array, AL_VALUE, val_nb);
} /* end dsP_delete_values */


/********************************************************/
/*							*/
/*	delete attribute type				*/
/*							*/
/********************************************************/
void
dsP_delete_type(
	OMX_workspace workspace,
	D2_a_type *type)
{
	/* free	object identifier type	*/
	if (type->d2_type)
		OMP_XFREE_STR(workspace, (type->d2_type), (type->d2_typ_len	
		? type->d2_typ_len : strlen(type->d2_type)));
} /* end dsP_delete_type */


/********************************************************/
/*							*/
/*	delete AVA object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_del_ava(
	OMX_workspace workspace,
	xds_ava *xobject)
{
	if (!xobject)
		return(OM_SUCCESS);
	/* free	attribute type			*/
	dsP_delete_type(workspace, &xobject->d2_avatype);

	/* free	string or object attribute values	 */
	dsP_delete_a_value(workspace, &xobject->d2_avaval);

	/*free the object			*/
	OMP_XFREE(workspace, xobject, AL_XDS_AVA, 1);
	return(OM_SUCCESS);
} /* end dsP_del_ava */


/********************************************************/
/*							*/
/*	delete ATTRIBUTE object				*/
/*							*/
/********************************************************/
OM_return_code
dsP_del_attribute(
	OMX_workspace	workspace,
	xds_attribute *xobject)
{
	if (!xobject)
		return(OM_SUCCESS);
	/* free	attribute type			*/
	dsP_delete_type(workspace, &xobject->d2_a_type);

	/* free	string attribute values	 and D2_a_value	array */
	dsP_delete_values(workspace, xobject->d2_a_val, xobject->d2_a_no_val);

	/*free the object			*/
	OMP_XFREE(workspace, xobject, AL_XDS_ATTRIBUTE, 1);
	return(OM_SUCCESS);
} /* end dsP_del_attribute */


/********************************************************/
/*							*/
/*	delete ATTRIBUTE LIST object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_del_attr_list(
	OMX_workspace	workspace,
	xds_attr_list *xobject)
{
	 xds_attribute *attr_ptr;
	 xds_attribute *attr_ptrnext;

	if (!xobject)
		return(OM_SUCCESS);
	/* free	all	the	attributes	*/
	attr_ptr = (xds_attribute *		)xobject;
	while (attr_ptr) {
		attr_ptrnext = attr_ptr->d2_a_next;
		dsP_del_attribute(workspace, attr_ptr);
		attr_ptr = attr_ptrnext;
	} /* endwhile */

	return(OM_SUCCESS);
} /* end dsP_del_attr_list */


/********************************************************/
/*							*/
/*	delete COMPARE RESULT object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_del_compare_result(
	OMX_workspace workspace,
	xds_compare_result *xobject)
{
	/* free	the	buffer allocated by	the	DUA		*/
	/*	with malloc() function			*/

	if (!xobject)
		return(OM_SUCCESS);
	if (xobject->dist_name)
		free(xobject->dist_name);
	if (xobject->com_res)
		free(xobject->com_res);

	/* free	the	object				*/
	OMP_XFREE(workspace, xobject, AL_XDS_COMPARE_RESULT, 1);
	return(OM_SUCCESS);
} /* end dsP_del_compare_result	*/


/********************************************************/
/*							*/
/*	delete EXTENSION object				*/
/*							*/
/********************************************************/
OM_return_code
dsP_del_extension(
	OMX_workspace	workspace,
	xds_extension *xobject)
{
	if (xobject) {
		OMP_XFREE(workspace, xobject, AL_XDS_EXT, 1);
	} /* endif */
	return(OM_SUCCESS);
} /* end dsP_del_extension */



/********************************************************/
/*							*/
/*	delete CONTEXT object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_del_context(
	OMX_workspace workspace,
	xds_context *xobject)
{

	 xds_extension	*ext_ptr;
	 xds_extension	*ext_ptrnext;
	OM_sint	ret;

	if (!xobject)
		return(OM_SUCCESS);
	ext_ptr	 = xobject->d2_extension;
	while (ext_ptr)	 {
		ext_ptrnext	 = ext_ptr->d2_next_ext;
		/* free	extensions			*/
		if ((ret = dsP_del_extension(workspace, ext_ptr)) !=  OM_SUCCESS)
			return(ret);
		ext_ptr	 = ext_ptrnext;
	} /* endwhile */

	/* free	the	object			*/
	OMP_XFREE(workspace, xobject, AL_XDS_CONTEXT, 1) ;
	return(OM_SUCCESS);
} /* end dsP_del_context */


/********************************************************/
/*							*/
/*	delete ENTRY INFORMATION object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_del_entry_info(
	OMX_workspace workspace,
	xds_entry_info *xobject)
{
	if (!xobject)
		return(OM_SUCCESS);
	/* free	distinguished name	*/
	if (xobject->d2_ei_entry)
		OMP_XFREE_STR(workspace, (xobject->d2_ei_entry),
		strlen(xobject->d2_ei_entry));

	/* free	attribute list		*/
	dsP_del_attr_list(workspace, xobject->d2_ei_ai);

	/* free	the	object		*/
	OMP_XFREE(workspace, xobject, AL_XDS_ENTRY_INFO, 1);


	return(OM_SUCCESS);
} /* end dsP_del_entry_info	*/


/********************************************************/
/*							*/
/*	delete ENTRY INFORMATION SELECTION	object	*/
/*							*/
/********************************************************/
static OM_return_code
dsP_del_entry_info_sel(
	OMX_workspace workspace,
	xds_entry_info_sel *xobject)
{
	 D2_a_type *typ_ptr;
	 OM_sint	i;

	if (!xobject)
		return(OM_SUCCESS);
	/* free	attribute types	if all_attributes is FALSE		*/
	/* In this case	d2_r_no_at = number	of attributes selected	*/
	/*		d2_r_type_at = pointer to an attributes	array	*/

	if ((xobject->d2_r_no_at !=		D2_ALL_ATT_REQ)	 &&	(xobject->d2_r_no_at) &&
		(xobject->d2_r_type_at)) {
		for	(i = 0, 	typ_ptr	 = xobject->d2_r_type_at; i	<  xobject->d2_r_no_at;
			i++, typ_ptr++)	{
			dsP_delete_type(workspace, typ_ptr);
		} /* endfor */
		/* free	the	attributes types array	*/
		OMP_XFREE(workspace, xobject->d2_r_type_at, 	AL_TYPE	, xobject->d2_r_no_at);
	} /* endif */

	/* free	the	object			*/
	OMP_XFREE(workspace, xobject, AL_XDS_ENTRY_INFO_SELECTION, 1);

	return(OM_SUCCESS);
} /* end dsP_del_entry_info_sel	*/


/********************************************************/
/*							*/
/*	delete ENTRY MODIFICATION object		*/
/*							*/
/********************************************************/
OM_return_code
dsP_del_entry_mod(
	OMX_workspace	workspace,
	xds_entry_mod *xobject)
{
	if (!xobject)
		return(OM_SUCCESS);
	/* free	attribute type		*/
	dsP_delete_type(workspace, &xobject->d2_m_attr.d2_a_type);

	/* free	attribute values	*/
	dsP_delete_values(workspace, xobject->d2_m_attr.d2_a_val, xobject->d2_m_attr.d2_a_no_val);

	/* free	the	object		*/
	OMP_XFREE(workspace, xobject, AL_XDS_ENTRY_MOD, 1);
	return(OM_SUCCESS);
} /* end dsP_del_entry_mod */


/********************************************************/
/*							*/
/*	delete ENTRY MODIFICATION LIST object		*/
/*							*/
/********************************************************/
static OM_return_code
dsP_del_entry_mod_list(
	OMX_workspace workspace,
	xds_entry_mod_list	*xobject)
{

	 xds_entry_mod *mod_ptr;
	 xds_entry_mod *mod_ptrnext;

	if (!xobject)
		return(OM_SUCCESS);
	/* free	all	the	modifications	*/

	mod_ptr	 =	xobject;
	while (mod_ptr)	 {
		mod_ptrnext	 =	mod_ptr->d2_m_next;
		dsP_del_entry_mod(workspace, mod_ptr);
		mod_ptr	 = mod_ptrnext;
	} /* endwhile */
	return(OM_SUCCESS);
} /* end dsP_del_entry_mod_list	*/


/********************************************************/
/*							*/
/*	delete FILTER ITEM object			*/
/*							*/
/********************************************************/
OM_return_code
dsP_del_fi_item(
	OMX_workspace workspace,
	xds_fi_item *xobject)
{
	if (!xobject)
		return(OM_SUCCESS);
	/* free	attribute type and values	*/
	dsP_delete_type(workspace, &xobject->f_item_att.d2_a_type);
	dsP_delete_values(workspace, xobject->f_item_att.d2_a_val, xobject->f_item_att.d2_a_no_val);

	/* free	initial	substrings	*/
	dsP_delete_a_value(workspace, &xobject->f_ini_sstr);

	/* free	final substrings	*/
	dsP_delete_a_value(workspace, &xobject->f_lst_sstr);

	/* free	the	object		*/
	OMP_XFREE(workspace, xobject, AL_XDS_FILTER_ITEM, 1);
	return(OM_SUCCESS);
} /* end dsP_del_fi_item */


/********************************************************/
/*							*/
/*	delete FILTER object				*/
/*							*/
/********************************************************/
OM_return_code
dsP_del_a_filter(
	OMX_workspace workspace,
	xds_filter *xobject)
{
	 xds_fi_item *fi_ptr;
	 xds_fi_item *fi_ptrnext;
	 xds_filter	*fi_ptrfirst;
	OM_sint	ret	 = OM_SUCCESS;

	if (!xobject)
		return(OM_SUCCESS);
	fi_ptr = xobject->fil_item;
	while (fi_ptr) {
		fi_ptrnext = fi_ptr->f_item_next;

		/* free	filter item		*/
		if ((ret = dsP_del_fi_item(workspace, fi_ptr)) != OM_SUCCESS)
			return(ret);

		fi_ptr = fi_ptrnext;
	} /* endwhile */

	if ((fi_ptrfirst = xobject->filter_first) != NULL)
		if ((ret = dsP_del_filter(workspace, fi_ptrfirst)) !=  OM_SUCCESS)
			return(ret);

	/* free	the	object		*/
	OMP_XFREE(workspace, xobject, AL_XDS_FILTER, 1);
	return(OM_SUCCESS);

} /* end dsP_del_a_filter */


static OM_return_code
dsP_del_filter(
	OMX_workspace workspace,
	xds_filter *xobject)
{
	 xds_filter	*fi_next;
	OM_sint	ret	 = OM_SUCCESS;

	if (!xobject)
		return(OM_SUCCESS);
	fi_next	 = xobject->filter_next;
	if ((ret = dsP_del_a_filter(workspace, xobject)) !=		OM_SUCCESS)
		return(ret);

	if (fi_next)
		ret	 = dsP_del_filter(workspace, fi_next);
	return(ret);
} /* end dsP_del_filter	*/


/********************************************************/
/*							*/
/*	delete LIST	RESULT object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_del_list_res(
	OMX_workspace workspace,
	xds_list_res *xobject)
{
	/* free	the	buffers	allocated by the DUA	*/
	/*	with malloc() function			*/

	if (!xobject)
		return(OM_SUCCESS);
	if (xobject->t_object)
		free(xobject->t_object);
	if (xobject->l_part_oq)	 {
		if (xobject->l_part_oq->cont_ref)
			free(xobject->l_part_oq->cont_ref);
		/* free	the	xds_part_oq	object		*/
		OMP_XFREE(workspace, xobject->l_part_oq, AL_XDS_PARTIAL_OUTCOME_QUAL,1);
	} /* endif */
	if (xobject->subordinates)
		free(xobject->subordinates);
	if (xobject->com_res)
		free(xobject->com_res);

	/* free	the	object				*/
	OMP_XFREE(workspace, xobject, AL_XDS_LIST_RESULT, 1);
	return(OM_SUCCESS);
} /* end dsP_del_list_res */


/********************************************************/
/*							*/
/*	delete READ	RESULT object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_del_read_result(
	OMX_workspace workspace,
	xds_read_result *xobject)
{
	if (!xobject)
		return(OM_SUCCESS);
	/* free	the	buffer allocated by	the	DUA		*/
	/*	with malloc() function			*/

	if (xobject->read_info)
		free(xobject->read_info);
	if (xobject->com_res)
		free(xobject->com_res);

	/* free	the	object				*/
	OMP_XFREE(workspace, xobject, AL_XDS_READ_RESULT, 1);
	return(OM_SUCCESS);
} /* end dsP_del_read_result */





/********************************************************/
/*							*/
/*	delete SEARCH RESULT object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_del_search_result(
	OMX_workspace	workspace,
	xds_search_result *xobject)
{
	/* free	the	buffers	allocated by the DUA	*/
	/*	with malloc() function			*/

	if (!xobject)
		return(OM_SUCCESS);
	if (xobject->search_entries)
		free(xobject->search_entries);
	if (xobject->b_object)
		free(xobject->b_object);
	if (xobject->s_part_oq)	 {
		if (xobject->s_part_oq->cont_ref)
			free(xobject->s_part_oq->cont_ref);
		/* free	the	xds_part_oq	object		*/
		OMP_XFREE(workspace, xobject->s_part_oq, AL_XDS_PARTIAL_OUTCOME_QUAL,1);
	} /* endif */
	if (xobject->com_res)
		free(xobject->com_res);

	/* free	the	object				*/
	OMP_XFREE(workspace, xobject, AL_XDS_SEARCH_RESULT, 1);
	return(OM_SUCCESS);
} /* end dsP_del_search_result */


/********************************************************/
/*							*/
/*	delete SESSION object				*/
/*							*/
/********************************************************/
OM_return_code
dsP_del_session(
	OMX_workspace workspace,
	xds_session *xobject)
{
	if (!xobject)
		return(OM_SUCCESS);
	/* free	psap			*/
	dsP_del_psap(workspace, 	xobject->dsa_address);

	/* free	dsa	name		*/
	dsP_del_dist_name(workspace, xobject->dsa_name);

	/* free	requestor		*/
	dsP_del_dist_name(workspace, xobject->requestor);

	/* free	password		*/
	if (xobject->password.d2_p_pw)
		OMP_XFREE_STR(workspace, (xobject->password.d2_p_pw),
		(xobject->password.d2_p_len ? xobject->password.d2_p_len :
		strlen(xobject->password.d2_p_pw)));
	/* free	auth_info		*/
	if (xobject->auth_info.d2_value)
		OMP_XFREE_STR(workspace, (xobject->auth_info.d2_value),
		xobject->auth_info.d2_size);
	/* free	the	object				*/
	OMP_XFREE(workspace, xobject, AL_XDS_SESSION, 1);
	return(OM_SUCCESS);
} /* end dsP_del_session */


/********************************************************/
/*							*/
/*	delete ERROR (DS_problem) object		*/
/*							*/
/********************************************************/
static OM_return_code
dsP_del_error(
	OMX_workspace	workspace,
	xds_error *xobject)
{
	/* free	the	object				*/
	if (xobject) {
		OMP_XFREE(workspace, xobject, AL_XDS_ERROR, 1);
	} /* endif */
	return(OM_SUCCESS);
} /* end dsP_del_error */


/********************************************************/
/*							*/
/*	delete ERROR (NAME,ATTRIBUTE,REFERR..) object	*/
/*							*/
/********************************************************/
static OM_return_code
dsP_del_xerror(
	OMX_workspace workspace,
	xds_error *xobject)
{
	if (!xobject)
		return(OM_SUCCESS);
	/* free	error buffer allocated by malloc	*/
	/* case	name_error,referral,attribute problem	*/
	if (xobject->ptr)
		free(xobject->ptr);

	/* free	the	object				*/
	OMP_XFREE(workspace, xobject, AL_XDS_ERROR, 1);
	return(OM_SUCCESS);
} /* end dsP_del_xerror	*/


/********************************************************/
/*							*/
/*	delete ACCESS_CONTROL_LIST	object		*/
/*							*/
/********************************************************/
static OM_return_code
dsP_del_acl(
	OMX_workspace workspace,
	xds_acl xobject)
{
	if (xobject) {
		OMP_XFREE_STR(workspace, xobject, dsP_get_acl_length(xobject));
	} /* endif */
	return(OM_SUCCESS);
} /* end dsP_del_acl */


/********************************************************/
/*							*/
/*	delete ACCESS_CONTROL_LIST_ITEM	object		*/
/*							*/
/********************************************************/
static OM_return_code
dsP_del_aci(
	OMX_workspace workspace,
	xds_acl_item xobject)
{
	if (xobject) {
		OMP_XFREE_STR(workspace, xobject, strlen(xobject));
	} /* endif */
	return(OM_SUCCESS);
} /* end dsP_del_aci */


/********************************************************/
/*							*/
/*	delete SEARCH GUIDE	object			*/
/*							*/
/********************************************************/
void
dsP_delete_criteria(
	OMX_workspace workspace,
	D2_criteria *xobject)
{
	OM_sint	i;
	D2_criteria	 * pt_criteria;

	if (xobject) {
		switch (xobject->d2_c_mode)	 {
		case D2_ITEM:
			if (xobject->d2_crit_val.type) {
				dsP_delete_type(workspace, &(xobject->d2_crit_val.type->d2_ci_type));
				OMP_XFREE(workspace, xobject->d2_crit_val.type,AL_CRIT_ITEM, 1);
			} /* endif */
			break;
		case D2_NOT:
			if (xobject->d2_crit_val.not) {
				dsP_delete_criteria(workspace, xobject->d2_crit_val.not);
				OMP_XFREE(workspace, xobject->d2_crit_val.not,
					 AL_XDS_SEARCH_CRITERION, 1);
			} /* endif */
			break;
		case D2_AND:
		case D2_OR:
			if ( (xobject->d2_crit_val.d2_ci_and_or) &&	 (xobject->d2_crit_val.d2_ci_and_or->and_or_nr)	&&
				(xobject->d2_crit_val.d2_ci_and_or->and_or)) {
				pt_criteria	 = xobject->d2_crit_val.d2_ci_and_or->and_or;
				for	(i = 0;	i <		xobject->d2_crit_val.d2_ci_and_or->and_or_nr;
					i++, pt_criteria++)
					dsP_delete_criteria(workspace, pt_criteria);

				OMP_XFREE(workspace, xobject->d2_crit_val.d2_ci_and_or->and_or,
					 AL_XDS_SEARCH_CRITERION,
					xobject->d2_crit_val.d2_ci_and_or->and_or_nr);

				OMP_XFREE(workspace, xobject->d2_crit_val.d2_ci_and_or,
					 AL_CRIT_SET, 1);
			} /* endif */
			break;
		} /* endswitch */
	} /* endif */
} /* end dsP_delete_criteria */


static OM_return_code
dsP_del_search_guide(
	OMX_workspace workspace,
	xds_search_guide *xobject)
{
	if (xobject) {
		dsP_delete_type(workspace, &xobject->d2_obj);
		dsP_delete_criteria(workspace, &xobject->d2_crit);
		OMP_XFREE(workspace, xobject, AL_XDS_SEARCH_GUIDE, 1);
	} /* endif */
	return(OM_SUCCESS);
} /* end dsP_del_search_guide */


/********************************************************/
/*							*/
/*	delete SEARCH CRITERION	object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_del_search_criterion(
	OMX_workspace workspace,
	xds_search_criterion *xobject)
{
	if (xobject) {
		dsP_delete_criteria(workspace, xobject);
		OMP_XFREE(workspace, xobject, AL_XDS_SEARCH_CRITERION, 1);
	} /* endif */
	return(OM_SUCCESS);
} /* end dsP_del_search_criterion */


/********************************************************/
/*							*/
/*	delete TELEX NUMBER	object			*/
/*							*/
/********************************************************/
void
dsP_delete_str(
	OMX_workspace workspace,
	D2_str *xobject)
{
	if (xobject) {
		if ((xobject->d2_size) && (xobject->d2_value))
			OMP_XFREE_STR(workspace, (xobject->d2_value), xobject->d2_size);
	} /* endif */
} /* end dsP_delete_str	*/


static OM_return_code
dsP_del_telex_number(
	OMX_workspace workspace,
	xds_telex_number *xobject)
{
	if (xobject) {
		dsP_delete_str(workspace, &xobject->d2_t_number);
		dsP_delete_str(workspace, &xobject->d2_c_code);
		dsP_delete_str(workspace, &xobject->d2_answer);
		OMP_XFREE(workspace, xobject, AL_XDS_TELEX_NBR, 1);
	} /* endif */
	return(OM_SUCCESS);
} /* end dsP_del_telex_number */


/********************************************************/
/*							*/
/*	delete FAX NUMBER object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_del_faxnumber(
	OMX_workspace	workspace,
	xds_faxnumber *xobject)
{
	if (xobject) {
		dsP_delete_str(workspace, &xobject->d2_tel_nr);
		OMP_XFREE(workspace, xobject, AL_XDS_FACSIMILE_PHONE_NBR, 1);
	} /* endif */
	return(OM_SUCCESS);
} /* end dsP_del_faxnumber */


/********************************************************/
/*							*/
/*	delete POSTAL ADDRESS object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_del_postal_address(
	OMX_workspace workspace,
	xds_postal_address *xobject)
{
	if (xobject) {
		dsP_del_str_list(workspace, xobject);
	} /* endif */
	return(OM_SUCCESS);
} /* end dsP_del_postal_address	*/


/********************************************************/
/*							*/
/*	delete TELETEX TERMINAL	ID object		*/
/*							*/
/********************************************************/
static OM_return_code
dsP_del_ttxid(
	OMX_workspace	workspace,
	xds_ttxid *xobject)
{
	if (xobject) {
		dsP_delete_str(workspace, &xobject->d2_terminal);
		dsP_delete_str(workspace, &xobject->d2_controls);
		dsP_delete_str(workspace, &xobject->d2_graphics);
		dsP_delete_str(workspace, &xobject->d2_miscel);
		dsP_delete_str(workspace, &xobject->d2_pages);
		dsP_delete_str(workspace, &xobject->d2_privates);

		OMP_XFREE(workspace, xobject, AL_XDS_TELETEX_TERM_IDENT, 1);
	} /* endif */
	return(OM_SUCCESS);
} /* end dsP_del_ttxid */

/* not static, since used in dsPPut.c */
OM_return_code
/********************************************************/
				dsP_del_algorithm_ident(
/********************************************************/
	OMX_workspace					workspace,
	xds_algorithm_ident				*xobject)
{

	if (xobject) {
		if ( xobject->parameters) {
			OM_FREE(xobject->parameters);
		} /* endif */
		OM_FREE(xobject->algorithm.d2_type);
	} /* endif */
	return(OM_SUCCESS);
} /* end dsP_del_algorithm_ident */

OM_return_code
/********************************************************/
		dsP_del_certificate(
							OMX_workspace workspace,
							xds_certificate *xobject)
/********************************************************/
{
	if (!xobject) {
		return(OM_SUCCESS);
	} /* endif */

	/* free    signature */
	dsP_del_algorithm_ident(workspace, &(xobject->to_be_signed.signature));

	/* free D2_name_string				issuer */
	OM_FREE(xobject->to_be_signed.issuer);

	/* free validity */
	OM_FREE(xobject->to_be_signed.validity.not_before);
	OM_FREE(xobject->to_be_signed.validity.not_after);

	/* free    D2_name_string          subject; */
	OM_FREE(xobject->to_be_signed.subject);

	/* free    subj_public_key_info.algorithm */
	dsP_del_algorithm_ident(workspace,
	&(xobject->to_be_signed.subj_public_key_info.algorithm));

	/* free    Subj_public_key_info.subject_key.d2_value; */
	OM_FREE(xobject->to_be_signed.subj_public_key_info.subject_key.d2_value);

	/* free    Alg_id	alg; */
	dsP_del_algorithm_ident(workspace,&(xobject->alg));

	/* free    Octet_string            signature_value; */
	OM_FREE(xobject->encr_str.d2_value);

	/* free	the	object				*/
	OM_FREE(xobject);

	return(OM_SUCCESS);
} /* end dsP_del_certificate */

static OM_return_code
/********************************************************/
dsP_del_certificate_list(OMX_workspace workspace, xds_certificate_list *xobject)
/********************************************************/
{
	if (!xobject) {
		return(OM_SUCCESS);
	} /* endif */

	/* free    UTC_time_string         last_update; */
		if (xobject->to_be_signed.last_update != NULL) {
			OM_FREE( xobject->to_be_signed.last_update);
		} /* endif */

	/* free    xds_certificate_sublist         revoked_certificates; */
		dsP_del_certificate_sublist(workspace, xobject->to_be_signed.rev_certs);

	/* free	the	object				*/
	OM_FREE(xobject);

	return(OM_SUCCESS);
} /* end dsP_del_certificate_list */

static OM_return_code
/********************************************************/
dsP_del_certificate_pair(OMX_workspace workspace, xds_certificate_pair *xobject)
/********************************************************/
{
	if (!xobject) {
		return(OM_SUCCESS);
	} /* endif */

	/* free    Certificate		*forward; */
	dsP_del_certificate(workspace, xobject->forward);

	/* free    Certificate		*reverse; */
	dsP_del_certificate(workspace, xobject->reverse);

	/* free	the	object				*/
	OM_FREE(xobject);

	return(OM_SUCCESS);
} /* end dsP_del_certificate_pair */

OM_return_code
/********************************************************/
dsP_del_certificate_sublist(OMX_workspace workspace, xds_certificate_sublist *xobject)
/********************************************************/
{
	if (!xobject) {
		return(OM_SUCCESS);
	} /* endif */

/* following assumes that each array is kept	*/
/* in contiguous memory							*/

	/* free    UTC_time_string     */
	/* xobject->to_be_signed.rev_certs->to_be_signed.rev_cert->rev_date */
	if (xobject->to_be_signed.rev_cert->rev_date!= NULL) {
		OM_FREE(xobject->to_be_signed.rev_cert->rev_date);
	} /* endif */

#if FALSE /* temp CC 14.3.94 */
	/* free		int		*serial_numbers */
	if (xobject->serial_numbers != NULL) {
		OM_FREE( xobject->serial_numbers);
	} /* endif */
#endif /* FALSE */

	/* free	the	object				*/
	OM_FREE(xobject);

	return(OM_SUCCESS);
} /* end dsP_del_certificate_sublist */
