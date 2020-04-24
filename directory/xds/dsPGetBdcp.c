/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPGetBdcp.c,v $
 * Revision 1.1.6.2  1996/02/18  18:24:45  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:22  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:32:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:34  root]
 * 
 * Revision 1.1.4.4  1994/07/06  15:09:02  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:24:18  marrek]
 * 
 * Revision 1.1.4.3  1994/05/10  16:03:53  marrek
 * 	Bug fixes and I18N for April 1994 submission.
 * 	[1994/05/06  09:58:52  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  13:14:32  marrek]
 * 
 * Revision 1.1.4.2  1994/03/23  15:48:39  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:36:13  keutel]
 * 
 * Revision 1.1.4.1  1994/02/22  18:10:10  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:20:39  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:59:29  marrek]
 * 
 * Revision 1.1.2.2  1993/08/10  07:03:33  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:14:38  marrek]
 * 
 * Revision 1.1.4.2  1992/12/30  21:16:44  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:04:55  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  23:18:31  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:45:56  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsPGetBdcp.c,v $ $Revision: 1.1.6.2 $ $Date: 1996/02/18 18:24:45 $";
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
/*	@(#)dsPGetBdcp.c	1.11 (Bull S.A)	4/23/92	*/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <xom.h>
#include <xomip.h>
#include <d2dir.h>
#include <xds.h>
#include <xdsiapl.h>
#include <xdsbdcp.h>
#include <alloc.h>
#include <xdsdef.h>
#include <xomdef.h>
#include <xmhdef.h>
#include <xdsalloc.h>
#include <xdsimport.h>
#include <xdspackage.h>
#include <dsPGetBdcp.h>
#include <dsPGet.h>
#include <dsPPut.h>
#include <omGet.h> /* for omP_alloc_sub_handle() */
#include <d27conv.h> /* for d27_local_to_t61(), d27_t61_to_local() */


/***************************************************************/
/*		no static function prototypes							*/
/***************************************************************/

extern OM_sint	IAPL_TO_OM[];	/* defined in dsPGet.c */

/********************************************************/
/*							*/
/*	get	SEARCH GUIDE object				*/
/*							*/
/********************************************************/
OM_return_code
dsP_get_search_guide(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_search_guide	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number)
{
	OMP_type_list	types_list[] = SR_GUID_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint	nb_desc		 = 0;
	OM_descriptor	 * ptr_desc;
	OM_descriptor	 * desc;
	OM_descriptor	 * desc0;
	 OMP_type_list *pt_l;
	OM_boolean initialize;

	VALIDATE_TYPE_LIST(types_list, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {

		for	(pt_l =	types_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0)
				continue;


			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_SEARCH_GUIDE);
				break;

			case DS_OBJECT_CLASS:
				if ((!xobject) ||  (!xobject->d2_obj_defined))
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DS_OBJECT_CLASS,
					 OM_S_OBJECT_IDENTIFIER_STRING,	xobject->d2_obj.d2_typ_len,
															xobject->d2_obj.d2_type,
					1);

				break;

			case DS_CRITERIA:
				if (!xobject)
					break;
				INC_NBDESC_MONO;
				OBJECT_VALUE(search_criterion, dsP_DS_C_SEARCH_CRITERION,
				(xds_search_criterion *) &xobject->d2_crit, DS_CRITERIA, 1);
				break;
			} /* endswitch */
			if ((initialize) &&		(ret !=		OM_SUCCESS))
				clean_before_exit(desc0, ptr_desc, nb_desc,
					 ret);
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize)
			break;
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number =		nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc,	&desc))	!=
			OM_SUCCESS)
			return(ret);

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original,		desc0);
	*copy =		desc0;
	*total_number =		nb_desc;
end:
	return(ret);
} /* end dsP_get_search_guide */


/********************************************************/
/*							*/
/*	get	SEARCH CRITERION object			*/
/*							*/
/********************************************************/
OM_return_code
dsP_get_search_criterion(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_search_criterion	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number)
{
	OMP_type_list	types_list[] = SR_CRIT_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint	nb_desc		 = 0;
	OM_descriptor	 * ptr_desc;
	OM_descriptor	 * desc;
	OM_descriptor	 * desc0;
	 OMP_type_list *pt_l;
	OM_boolean initialize;
	OM_sint	i;
	xds_search_criterion	 * adr_obj;

	VALIDATE_TYPE_LIST(types_list, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {

		for	(pt_l = types_list; pt_l->type != OM_NO_MORE_TYPES; pt_l++) {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0)
				continue;


			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_SEARCH_CRITERION);
				break;

			case DS_ATTRIBUTE_TYPE:
				if ((!xobject) ||  (xobject->d2_c_mode !=
					D2_ITEM) ||		 (!xobject->d2_crit_val.type))
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DS_ATTRIBUTE_TYPE,
					 OM_S_OBJECT_IDENTIFIER_STRING,	xobject->d2_crit_val.type->d2_ci_type.d2_typ_len,
																																	 xobject->d2_crit_val.type->d2_ci_type.d2_type,
					 1);
				break;

			case DS_CRITERIA:
				if ((!xobject) ||  (xobject->d2_c_mode ==
					D2_ITEM))
					break;
				if (!initialize) {
					if (xobject->d2_c_mode ==  D2_NOT)	 {
						if ((OMP_EX_ALL_BUT_CERTAIN_VALUES)	 &&
							(!(OMP_REPLACE_SINGLE_VALUE)))
							return(OM_NOT_PRESENT);
						if (!xobject->d2_crit_val.not) {
							pt_l->max_val =
								0;
							break;
						} /* endif */
						nb_desc++;
						/* single value		*/
						pt_l->max_val =	 1;
						pt_l->nb_val =	1;
						pt_l->desc = (xds_search_criterion *
							)
						(xobject->d2_crit_val.not);
						break;
					} else /* D2_AND or	D2_OR  */		   {
						if ((!xobject->d2_crit_val.d2_ci_and_or) ||
							(!xobject->d2_crit_val.d2_ci_and_or->and_or)) {
							pt_l->max_val =
								0;
							break;
						} /* endif */
						/* multi valued		*/
						pt_l->max_val =	 xobject->d2_crit_val.d2_ci_and_or->and_or_nr;
						UPDATE_LIMIT_VALUE(pt_l->max_val);
						pt_l->desc = (D2_criteria *
							)
						(xobject->d2_crit_val.d2_ci_and_or->and_or)	 +
							initial_value;
						nb_desc	 +=	pt_l->nb_val;
						break;
					} /* endif */
				} /* endif */
				adr_obj	 = pt_l->desc;
				for	(i = 0;	i <		pt_l->nb_val; i++, adr_obj++)	 {

					OBJECT_VALUE(search_criterion, dsP_DS_C_SEARCH_CRITERION,
					(xds_search_criterion*)adr_obj, DS_CRITERIA, pt_l->max_val);
				} /* endfor */

				break;

			case DS_FILTER_ITEM_TYPE:
				if ((!xobject) ||  (xobject->d2_c_mode !=
					D2_ITEM) ||		 (!xobject->d2_crit_val.type))
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_ENUM(ptr_desc, DS_FILTER_ITEM_TYPE,
					 xobject->d2_crit_val.type->d2_ci_mode,
					1);
				break;

			case DS_FILTER_TYPE:
				if (!xobject)
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_ENUM(ptr_desc, DS_FILTER_TYPE,
					 xobject->d2_c_mode, 1);
				break;
			} /* endswitch */
			if ((initialize) &&		(ret !=		OM_SUCCESS))
				clean_before_exit(desc0, ptr_desc, nb_desc,
					 ret);
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize)
			break;
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number =		nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc,	&desc))	!=
			OM_SUCCESS)
			return(ret);

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original,		desc0);
	*copy =		desc0;
	*total_number =		nb_desc;
end:
	return(ret);
} /* end dsP_get_search_criterion */


/********************************************************/
/*							*/
/*	get	TELEX NUMBER object				*/
/*							*/
/********************************************************/
OM_return_code
dsP_get_telex_number(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_telex_number	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number)
{
	OMP_type_list	types_list[] = TX_NB_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint	nb_desc		 = 0;
	OM_descriptor	 * ptr_desc;
	OM_descriptor	 * desc;
	OM_descriptor	 * desc0;
	 OMP_type_list *pt_l;
	OM_boolean initialize;

	VALIDATE_TYPE_LIST(types_list, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {

		for	(pt_l = types_list; pt_l->type != OM_NO_MORE_TYPES; pt_l++) {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0)
				continue;


			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_TELEX_NBR);
				break;

			case DS_ANSWERBACK:
				if ((!xobject) ||  (!xobject->d2_answer.d2_size) ||
					(!xobject->d2_answer.d2_value))
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DS_ANSWERBACK,
					 OM_S_PRINTABLE_STRING,			 xobject->d2_answer.d2_size,
																																	xobject->d2_answer.d2_value,
					 1);
				break;

			case DS_COUNTRY_CODE:
				if ((!xobject) ||  (!xobject->d2_c_code.d2_size) ||
					(!xobject->d2_c_code.d2_value))
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DS_COUNTRY_CODE,
					 OM_S_PRINTABLE_STRING,			 xobject->d2_c_code.d2_size,
																																	xobject->d2_c_code.d2_value,
					 1);
				break;

			case DS_TELEX_NBR:
				if ((!xobject) ||  (!xobject->d2_t_number.d2_size) ||
					(!xobject->d2_t_number.d2_value))
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DS_TELEX_NBR,
					OM_S_PRINTABLE_STRING,		 xobject->d2_t_number.d2_size,
																																	xobject->d2_t_number.d2_value,
					 1);
				break;
			} /* endswitch */
			if ((initialize) &&		(ret !=		OM_SUCCESS))
				clean_before_exit(desc0, ptr_desc, nb_desc,
					 ret);
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize)
			break;
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number =		nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc,	&desc))	!=
			OM_SUCCESS)
			return(ret);

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original,		desc0);
	*copy =		desc0;
	*total_number =		nb_desc;
end:
	return(ret);
} /* end dsP_get_telex_number */


/********************************************************/
/*							*/
/*	get	FAX	NUMBER object				*/
/*							*/
/********************************************************/
OM_return_code
dsP_get_faxnumber(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_faxnumber		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number)
{
	OMP_type_list	types_list[] = FAX_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint	nb_desc		 = 0;
	OM_descriptor	 * ptr_desc;
	OM_descriptor	 * desc;
	OM_descriptor	 * desc0;
	 OMP_type_list *pt_l;
	OM_boolean initialize;

	VALIDATE_TYPE_LIST(types_list, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {

		for	(pt_l = types_list; pt_l->type != OM_NO_MORE_TYPES; pt_l++) {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0)
				continue;


			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_FACSIMILE_PHONE_NBR);
				break;

			case DS_PARAMETERS:
				if ((!xobject) ||  (!xobject->d2_par_defined))
					break;
				INC_NBDESC_MONO;
				OBJECT_VALUE(G3_Fax_NBP, dsP_MH_C_G3_FAX_NBPS,
				&xobject->d2_par, DS_PARAMETERS, 1);
				break;

			case DS_PHONE_NBR:
				if ((!xobject) ||  (!xobject->d2_tel_nr.d2_size) ||
					(!xobject->d2_tel_nr.d2_value))
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DS_PHONE_NBR,
					OM_S_PRINTABLE_STRING,		 xobject->d2_tel_nr.d2_size,
																																	xobject->d2_tel_nr.d2_value,
					 1);
				break;
			} /* endswitch */
			if ((initialize) &&		(ret !=		OM_SUCCESS))
				clean_before_exit(desc0, ptr_desc, nb_desc,
					 ret);
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize)
			break;
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number =		nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc,	&desc))	!=
			OM_SUCCESS)
			return(ret);

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original,		desc0);
	*copy =		desc0;
	*total_number =		nb_desc;
end:
	return(ret);
} /* end dsP_get_faxnumber */


/********************************************************/
/*							*/
/*	get	POSTAL ADDRESS object			*/
/*							*/
/********************************************************/
OM_return_code
dsP_get_postal_address(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_postal_address	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number)
{
	OMP_type_list	types_list[] = POST_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint	nb_desc		 = 0;
	OM_descriptor	 * ptr_desc;
	OM_descriptor	 * desc;
	OM_descriptor	 * desc0;
	 OMP_type_list *pt_l;
	OM_boolean initialize;
	OM_sint	i;
	 xds_str_list	**adr_obj;
	OM_sint	n_cur;
	OM_syntax			syntax;

	VALIDATE_TYPE_LIST(types_list, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {

		for	(pt_l =		types_list;	pt_l->type != OM_NO_MORE_TYPES;
			pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0)
				continue;


			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_POSTAL_ADDRESS);

				break;
			case DS_POSTAL_ADDRESS:
				if (!xobject)
					break;
				if (!initialize) {
					GET_NB_OBJ(xds_str_list, d2_next,
						 pt_l->max_val,	&xobject, adr_obj);
					UPDATE_LIMIT_VALUE(pt_l->max_val);
					JUMP_N_OBJ(xds_str_list, d2_next,
						 initial_value,	&xobject, adr_obj,
							n_cur);
					pt_l->desc = adr_obj;
					for	(i = 0;	i <		pt_l->nb_val; i++,
						 adr_obj = &(*adr_obj)->d2_next) {
						if ((!(*adr_obj)->d2_str.d2_size) ||
							(!(*adr_obj)->d2_str.d2_value))
							continue;
						nb_desc++;
					} /* end for */
					break;
				} /* endif */

				adr_obj	 = pt_l->desc;
				for	(i = 0;	i <		pt_l->nb_val; i++, adr_obj =
					&(*adr_obj)->d2_next) {
					if ((!(*adr_obj)->d2_str.d2_size) ||
						(!(*adr_obj)->d2_str.d2_value))
						continue;
					switch ((*adr_obj)->d2_rep)	 {
					case D2_PRINTABLE :
						syntax = IAPL_TO_OM[(*adr_obj)->d2_rep];
						break;
					case D2_T61	:
						syntax =  OM_S_TELETEX_STRING;
						break;
					} /* endswitch */

					UPDATE_DESC_STR(ptr_desc, pt_l->type,
						 syntax, (*adr_obj)->d2_str.d2_size,
						 (*adr_obj)->d2_str.d2_value,
						1);
				} /* endfor	*/
				break;
			} /* endswitch */
			if ((initialize) &&		(ret !=		OM_SUCCESS))
				clean_before_exit(desc0, ptr_desc, nb_desc,
					 ret);
		} /* endfor	*/
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize)
			break;
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number =		nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc,	&desc))	!=
			OM_SUCCESS)
			return(ret);

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original,		desc0);
	*copy =		desc0;
	*total_number =		nb_desc;
end:
	return(ret);
} /* end dsP_get_postal_address	*/


/********************************************************/
/*							*/
/*	get	TELETEX	TERMINAL ID	object			*/
/*							*/
/********************************************************/
OM_return_code
dsP_get_ttxid(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_ttxid		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number)
{
	OMP_type_list	types_list[] = TTXID_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint	nb_desc		 = 0;
	OM_descriptor	 * ptr_desc;
	OM_descriptor	 * desc;
	OM_descriptor	 * desc0;
	 OMP_type_list *pt_l;
	OM_boolean initialize;

	VALIDATE_TYPE_LIST(types_list, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {

		for	(pt_l = types_list; pt_l->type != OM_NO_MORE_TYPES; pt_l++) {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0)
				continue;


			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_TELETEX_TERM_IDENT);
				break;

			case DS_PARAMETERS:
				if (!xobject)
					break;
				INC_NBDESC_MONO;
				OBJECT_VALUE(Teletex_NBP, dsP_MH_C_TELETEX_NBPS,
				xobject, DS_PARAMETERS, 1);
				break;

			case DS_TELETEX_TERM:
				if ((!xobject) ||  (!xobject->d2_terminal.d2_size) ||
					(!xobject->d2_terminal.d2_value))
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, DS_TELETEX_TERM,
					 OM_S_PRINTABLE_STRING,			 xobject->d2_terminal.d2_size,
																																	xobject->d2_terminal.d2_value,
					 1);
				break;
			} /* endswitch */
			if ((initialize) &&		(ret !=		OM_SUCCESS))
				clean_before_exit(desc0, ptr_desc, nb_desc,
					 ret);
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize)
			break;
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number =		nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc,	&desc))	!=
			OM_SUCCESS)
			return(ret);

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original,		desc0);
	*copy =		desc0;
	*total_number =		nb_desc;
end:
	return(ret);
} /* end dsP_get_ttxid */


/********************************************************/
/*							*/
/*	get	G3-FAX-NBPs	object				*/
/*							*/
/********************************************************/
OM_return_code
dsP_get_G3_Fax_NBP(
	OMX_workspace		workspace,
	OM_private_object	original,
	OM_sint32			*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number)
{
	OMP_type_list	types_list[] = G3_FAX_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint	nb_desc		 = 0;
	OM_descriptor	 * ptr_desc;
	OM_descriptor	 * desc;
	OM_descriptor	 * desc0;
	 OMP_type_list *pt_l;
	OM_boolean initialize;

	VALIDATE_TYPE_LIST(types_list, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {

		for	(pt_l = types_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0)
				continue;


			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_MH_C_G3_FAX_NBPS);
				break;

			case MH_T_A3_WIDTH:
				if (!xobject)
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, MH_T_A3_WIDTH ,
					 (*xobject & D2_A3_WIDTH ? OM_TRUE :  OM_FALSE)	,
															1);
				break;
			case MH_T_B4_LENGTH:
				if (!xobject)
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, MH_T_B4_LENGTH ,
					 (*xobject & D2_B4_LENGTH ?		OM_TRUE	 :
					OM_FALSE) ,
					1);
				break;
			case MH_T_B4_WIDTH:
				if (!xobject)
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, MH_T_B4_WIDTH ,
					 (*xobject & D2_B4_WIDTH ? OM_TRUE :  OM_FALSE)	,
															1);
				break;
			case MH_T_FINE_RESOLUTION:
				if (!xobject)
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, MH_T_FINE_RESOLUTION	,
					 (*xobject & D2_FINE_RESOLUTION	 ?	OM_TRUE
					 :					OM_FALSE) ,		1);
				break;
			case MH_T_TWO_DIMENSIONAL:
				if (!xobject)
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, MH_T_TWO_DIMENSIONAL	,
					 (*xobject & D2_TWO_DIMENSIONAL	 ?	OM_TRUE
					 :					OM_FALSE) ,		1);
				break;
			case MH_T_UNCOMPRESSED:
				if (!xobject)
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, MH_T_UNCOMPRESSED ,
					 (*xobject & D2_UNCOMPRESSED ?	OM_TRUE	 :
					OM_FALSE) ,		1);
				break;
			case MH_T_UNLIMITED_LENGTH:
				if (!xobject)
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_BOOL(ptr_desc, MH_T_UNLIMITED_LENGTH ,
					 (*xobject & D2_UNLIMITED_LENGTH ?	OM_TRUE
					 :					OM_FALSE) ,		1);
				break;
			} /* endswitch */
			if ((initialize) &&		(ret !=		OM_SUCCESS))
				clean_before_exit(desc0, ptr_desc, nb_desc,
					 ret);
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize)
			break;
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number =		nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc,	&desc))	!=
			OM_SUCCESS)
			return(ret);

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original,		desc0);
	*copy =		desc0;
	*total_number =		nb_desc;
end:
	return(ret);
} /* end dsP_get_G3_Fax_NBP	*/


/********************************************************/
/*							*/
/*	get	Teletex-NBPs object				*/
/*							*/
/********************************************************/
OM_return_code
dsP_get_Teletex_NBP(
	OMX_workspace		workspace,
	OM_private_object	original,
	D2_ttxid		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number)
{

	OMP_type_list	types_list[] = TX_NBP_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint	nb_desc		 = 0;
	OM_descriptor	 * ptr_desc;
	OM_descriptor	 * desc;
	OM_descriptor	 * desc0;
	 OMP_type_list *pt_l;
	OM_boolean initialize;

	VALIDATE_TYPE_LIST(types_list, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {

		for	(pt_l = types_list; pt_l->type != OM_NO_MORE_TYPES; pt_l++) {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0)
				continue;


			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_MH_C_TELETEX_NBPS);
				break;

			case MH_T_CONTROL_CHARACTER_SETS:
				if ((!xobject) ||  (!xobject->d2_controls.d2_size) ||
					(!xobject->d2_controls.d2_value))
					break;
				INC_NBDESC_MONO;
				if (local_strings) {
					unsigned char *result;
					OM_uint32 result_len = 0;
					if (d27_t61_to_local( (char *)xobject->d2_controls.d2_value,
					xobject->d2_controls.d2_size,(char **)&result,&result_len) 
					!= D2_NOERROR) {
						ret	 = OM_WRONG_VALUE_SYNTAX;
						break;
					} else {
						xobject->d2_controls.d2_size = result_len;
						xobject->d2_controls.d2_value = result;
						/* possible memory leak here */
					} /* endif */
				} /* endif */
				UPDATE_DESC_STR(ptr_desc, MH_T_CONTROL_CHARACTER_SETS,
				local_strings?OM_S_TELETEX_STRING|OM_S_LOCAL_STRING:OM_S_TELETEX_STRING ,
				xobject->d2_controls.d2_size,
				xobject->d2_controls.d2_value, 1);
				break;
			case MH_T_GRAPHIC_CHARACTER_SETS:
				if ((!xobject) ||  (!xobject->d2_graphics.d2_size) ||
					(!xobject->d2_graphics.d2_value))
					break;
				INC_NBDESC_MONO;
				if (local_strings) {
					unsigned char *result;
					OM_uint32 result_len = 0;
					if (d27_t61_to_local( (char *)xobject->d2_graphics.d2_value,
					xobject->d2_graphics.d2_size,(char **)&result,&result_len) 
					!= D2_NOERROR) {
						ret	 = OM_WRONG_VALUE_SYNTAX;
						break;
					} else {
						xobject->d2_graphics.d2_size = result_len;
						xobject->d2_graphics.d2_value = result;
						/* possible memory leak here */
					} /* endif */
				} /* endif */
				UPDATE_DESC_STR(ptr_desc, MH_T_GRAPHIC_CHARACTER_SETS,
				local_strings?OM_S_TELETEX_STRING|OM_S_LOCAL_STRING:OM_S_TELETEX_STRING ,
				xobject->d2_graphics.d2_size,
				xobject->d2_graphics.d2_value, 1);
				break;
			case MH_T_MISCELLANEOUS_CAPABILITIES:
				if ((!xobject) ||  (!xobject->d2_miscel.d2_size) ||
					(!xobject->d2_miscel.d2_value))
					break;
				INC_NBDESC_MONO;
				if (local_strings) {
					unsigned char *result;
					OM_uint32 result_len = 0;
					if (d27_t61_to_local( (char *)xobject->d2_miscel.d2_value,
					xobject->d2_miscel.d2_size,(char **)&result,&result_len) 
					!= D2_NOERROR) {
						ret	 = OM_WRONG_VALUE_SYNTAX;
						break;
					} else {
						xobject->d2_miscel.d2_size = result_len;
						xobject->d2_miscel.d2_value = result;
						/* possible memory leak here */
					} /* endif */
				} /* endif */
				UPDATE_DESC_STR(ptr_desc, MH_T_MISCELLANEOUS_CAPABILITIES,
				local_strings?OM_S_TELETEX_STRING|OM_S_LOCAL_STRING:OM_S_TELETEX_STRING ,
				xobject->d2_miscel.d2_size,
				xobject->d2_miscel.d2_value, 1);
				break;
			case MH_T_PAGE_FORMATS:
				if ((!xobject) ||  (!xobject->d2_pages.d2_size)	 ||
					(!xobject->d2_pages.d2_value))
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, MH_T_PAGE_FORMATS,
					 OM_S_OCTET_STRING,			 xobject->d2_pages.d2_size,
																																	xobject->d2_pages.d2_value,
					 1);
				break;
			case MH_T_PRIVATE_USE:
				if ((!xobject) ||  (!xobject->d2_privates.d2_size) ||
					(!xobject->d2_privates.d2_value))
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, MH_T_PRIVATE_USE,
					 OM_S_OCTET_STRING,			 xobject->d2_privates.d2_size,
																																	xobject->d2_privates.d2_value,
					 1);
				break;
			} /* endswitch */
			if ((initialize) &&		(ret !=		OM_SUCCESS))
				clean_before_exit(desc0, ptr_desc, nb_desc,
					 ret);
		} /* endfor */
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize)
			break;
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number =		nb_desc;
			return(OM_SUCCESS);
		} /* endif */

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc,	&desc))	
		!= OM_SUCCESS)
			return(ret);

		ptr_desc = desc0 = desc	+ 1;
		initialize = OM_TRUE;
	} /* endwhile */

	omP_update_descriptor(original,	desc0);
	*copy =	desc0;
	*total_number =	nb_desc;
end:
	return(ret);
} /* end dsP_get_Teletex_NBP */


