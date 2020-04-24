/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPMdup.c,v $
 * Revision 1.1.6.2  1996/02/18  18:24:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:25  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:32:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:36  root]
 * 
 * Revision 1.1.4.4  1994/07/06  15:09:04  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:24:28  marrek]
 * 
 * Revision 1.1.4.3  1994/05/10  16:03:56  marrek
 * 	Bug fix for April 1994 submission.
 * 	[1994/05/09  15:26:50  marrek]
 * 
 * 	Bug fixes and I18N for April 1994 submission.
 * 	[1994/05/06  09:59:04  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  13:14:46  marrek]
 * 
 * Revision 1.1.4.2  1994/03/23  15:48:41  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:36:23  keutel]
 * 
 * Revision 1.1.4.1  1994/02/22  18:10:13  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:20:56  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  11:59:55  marrek]
 * 
 * Revision 1.1.2.2  1993/08/10  07:04:23  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:15:12  marrek]
 * 
 * Revision 1.1.4.4  1992/12/30  21:16:50  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:05:06  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/04  17:38:12  marrek
 * 	November 1992 code drop
 * 	Replace cmx.h by cmx_addr.h
 * 	[1992/12/04  12:21:54  marrek]
 * 
 * Revision 1.1.4.2  1992/11/30  08:34:05  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:42:51  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  19:46:31  zeliff
 * 	Commenting out non-ANSI #ident
 * 	[1992/05/28  15:54:32  zeliff]
 * 
 * 	New files from GDS drop
 * 	[1992/05/27  19:45:21  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsPMdup.c,v $ $Revision: 1.1.6.2 $ $Date: 1996/02/18 18:24:49 $";
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
/*	@(#)dsPMdup.c	1.7	(Bull S.A) 4/23/92	*/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <xom.h>
#include <d2dir.h>
#include <xdsiapl.h>
#include <xds.h>
#include <xomip.h>
#include <alloc.h>
#include <xdsgds.h>
#include <xdsbdcp.h>
#include <osiaddr.h>
#include <cmx_addr.h>
#include <xdsdef.h>
#include <xomdef.h>
#include <xmhdef.h>
#ifndef	_MDUP_PKG_
#define	_MDUP_PKG_
#endif

#include <xdsalloc.h>
#include <xdsimport.h>
#include <xdspackage.h>
#include <omGet.h>
#include <dsPDelete.h>
#include <dsPPut.h> /* for validate_type_and_mod() ... */
#include <dsPGet.h>
#include <dsPMdup.h>
#include <d27conv.h>

/***************************************************************/
/*		static function prototypes							*/
/***************************************************************/



extern OM_sint	OM_TO_IAPL[];
extern OM_sint	IAPL_TO_OM[];

OMP_enum_limit	terminal_type_limit	 = TERMINAL_TYPE_LIMITS;
OMP_enum_limit	perm_type_limit	 = PERM_TYPE_LIMITS;

OMP_enum_list	terminal_type_list[] = TERMINAL_TYPE_LIST;
OMP_enum_list	perm_type_list[] = PERM_TYPE_LIST;

#define	DEL_STR_LIST(NAME)						\
{									\
	dsP_del_str_list(workspace,xobject->NAME);				\
}
#define	DELETE_STRING(NAME)						\
{									\
	if (xobject->NAME)							\
	OMP_XFREE_STR(workspace,xobject->NAME, 	\
					strlen((char *)xobject->NAME));	\
}

#define	DEL_PRT_STRING	DELETE_STRING
#define	DEL_NUM_STRING	DELETE_STRING
#define	DEL_TTX_STRING	DELETE_STRING

#define	CHECK_PERM_TYPE(TYPE,PERM)			\
{							\
	if (check)					\
	{						\
		if (TYPE != PERM)				\
		{						\
		ret	= OM_NO_SUCH_TYPE;			\
		break;					\
		}						\
	}						\
}


/********************************************************/
/*							*/
/*	del	a STR from a STR_LIST						*/
/*							*/
/********************************************************/
OM_return_code
dsP_del_a_str(
	OMX_workspace	workspace,
	xds_str_list *xobject)
{
	dsP_delete_str(workspace, &xobject->d2_str);
	/* free	the	object		*/
	OMP_XFREE(workspace, xobject, AL_XDS_STR_LIST, 1);
	return(OM_SUCCESS);
} /* end dsP_del_a_str */


/********************************************************/
/*							*/
/*	del	STR_LIST									*/
/*							*/
/********************************************************/
OM_return_code
dsP_del_str_list(
	OMX_workspace workspace,
	xds_str_list	*xobject)
{
	 xds_str_list *str_ptr;
	 xds_str_list *str_ptrnext;

	if (!xobject)
		return(OM_SUCCESS);
	/* free	all	the	modifications	*/

	str_ptr	 = xobject;
	while (str_ptr)	 {
		str_ptrnext	 = str_ptr->d2_next;
		dsP_del_a_str(workspace, str_ptr);
		str_ptr	 = str_ptrnext;
	}
	return(OM_SUCCESS);
} /* end dsP_del_str_list */


/********************************************************/
/*														*/
/*	delete OR_address object							*/
/*														*/
/********************************************************/
OM_return_code
dsP_del_OR_address(
	OMX_workspace workspace,
	xds_OR_address *xobject)
{
	if (!xobject)
		return(OM_SUCCESS);
	DEL_STR_LIST (d2_admd_name);
	DEL_STR_LIST (d2_common_name);
	DEL_STR_LIST (d2_country_name);
	DEL_STR_LIST (d2_domain_type1);
	DEL_STR_LIST (d2_domain_type2);
	DEL_STR_LIST (d2_domain_type3);
	DEL_STR_LIST (d2_domain_type4);
	DEL_STR_LIST (d2_domain_value1);
	DEL_STR_LIST (d2_domain_value2);
	DEL_STR_LIST (d2_domain_value3);
	DEL_STR_LIST (d2_domain_value4);
	DEL_STR_LIST (d2_generation);
	DEL_STR_LIST (d2_given_name);
	DEL_STR_LIST (d2_initials);
	DEL_NUM_STRING (d2_isdn_number);
	DEL_NUM_STRING (d2_isdn_subaddress);
	DEL_NUM_STRING (d2_numeric_user_id);
	DEL_STR_LIST (d2_org_name);
	DEL_STR_LIST (d2_org_unit_name1);
	DEL_STR_LIST (d2_org_unit_name2);
	DEL_STR_LIST (d2_org_unit_name3);
	DEL_STR_LIST (d2_org_unit_name4);
	DEL_STR_LIST (d2_post_addr_details);
	DEL_TTX_STRING (d2_post_addr_in_full);
	DEL_STR_LIST (d2_post_code);
	DEL_STR_LIST (d2_post_country_name);
	DEL_STR_LIST (d2_post_deliv_pt_name);
	DEL_PRT_STRING (d2_post_deliv_sys_name);
	DEL_STR_LIST (d2_post_gen_deliv_addr);
	DEL_STR_LIST (d2_post_locale);
	DEL_STR_LIST (d2_post_office_box_nbr);
	DEL_STR_LIST (d2_post_office_name);
	DEL_STR_LIST (d2_post_office_nbr);
	DEL_STR_LIST (d2_post_org_name);
	DEL_STR_LIST (d2_post_patron_details);
	DEL_STR_LIST (d2_post_patron_name);
	DEL_STR_LIST (d2_post_street_addr);
	dsP_del_psap(workspace, xobject->d2_presentation_addr);
	DEL_STR_LIST (d2_prmd_name);
	DEL_STR_LIST (d2_surname);
	DEL_PRT_STRING (d2_terminal_id);
	DEL_NUM_STRING (d2_x121_addr);
	DEL_STR_LIST (d2_post_addr_in_lines);

	/* free	the	object				*/
	OMP_XFREE(workspace, xobject, AL_XDS_OR_ADDRESS, 1);
	return(OM_SUCCESS);
} /* end dsP_del_OR_address	*/


/********************************************************/
/*														*/
/*	delete OR_name object								*/
/*														*/
/********************************************************/
OM_return_code
dsP_del_OR_name(
	OMX_workspace workspace,
	xds_OR_name *xobject)
{
	if (!xobject)
		return(OM_SUCCESS);
	/* free	directory name		*/
	dsP_del_dist_name(workspace, xobject->d2_dir_name);

	/* free	OR address		*/
	dsP_del_OR_address(workspace, xobject->d2_or_address);

	/* free	the	object				*/
	OMP_XFREE(workspace, xobject, AL_XDS_OR_NAME, 1);
	return(OM_SUCCESS);
} /* end dsP_del_OR_name */


/********************************************************/
/*														*/
/*	delete DL_SUBMIT_PERM object						*/
/*														*/
/********************************************************/
OM_return_code
dsP_del_dlsp(
	OMX_workspace workspace,
	xds_dl_submit_perm *xobject)
{
	if (!xobject)
		return(OM_SUCCESS);
	switch (xobject->d2_perm_type) {
	case DS_PERM_INDIVIDUAL	:
	case DS_PERM_MEMBER_OF_DL :
	case DS_PERM_PATTERN_MATCH :
		/* free	OR_name			*/
		dsP_del_OR_name(workspace, xobject->d2_choice.d2_individual);
		break;

	case DS_PERM_MEMBER_OF_GROUP :
		/* free	name list		*/
		dsP_del_dist_name(workspace, xobject->d2_choice.d2_member_of_group);
		break;
	}
	/* free	the	object				*/
	OMP_XFREE(workspace, xobject, AL_XDS_DL_SUBMIT_PERMS, 1);
	return(OM_SUCCESS);
} /* end dsP_del_dlsp */


/********************************************************/
/*							*/
/*	put	STR_LIST									*/
/*							*/
/********************************************************/
OM_return_code
dsP_put_str_list(
	OMX_workspace	workspace,
	void **xds_object,
	OM_modification	modification,
	OMP_type_list	*pt_l,
	void **pt_obj
)
{

	OM_return_code	ret;
	OM_descriptor	 * desc;
	void	*link;
	D2_str_list	 * pt;
	OM_sint	i;
	D2_str	 * str_pt;

	ret	 = OM_SUCCESS;
	if (OMP_REMOVE)
		pt_l->nb_val = 0;
	for	(i = 0, desc = (OM_descriptor *		)pt_l->desc; i < pt_l->nb_val;
		i++, desc++, pt_obj	 = (void *)	& ((xds_str_list
		*		) *		pt_obj)->d2_next) {
		/* allocate	XDS	structure */
		if ((pt	 = (xds_str_list	 * )OMP_XALLOC(workspace, AL_XDS_STR_LIST,
			 1)) == (void *)0)	 {
			ret	 = OM_MEMORY_INSUFFICIENT;
			break;
		}
		/* clear XDS structure */
		memset (pt, 0, sizeof(xds_str_list));
		/* call	PUT	function
		 * with	REPLACE_ALL	modification
		 */
		str_pt = (void *) &		pt->d2_str;
		UPDATE_STRING (	(pt->d2_str.d2_size), (pt->d2_str.d2_value),
			 desc->value.string.length, desc->value.string.elements);
		if (ret	 != OM_SUCCESS)
			break;
		pt->d2_rep = OM_TO_IAPL[OMP_SYNTAX(desc)];
		link = *pt_obj;
		*pt_obj	 = pt;
		pt->d2_next	 = link;
	}

#if 0
	if (!*xds_object)
	{
		if ((*xds_object = OMP_XALLOC(workspace,AL_XDS_STR_LIST,1))
						== (void *)0)
		return(OM_MEMORY_INSUFFICIENT);
/* clear xds structure */
		memset (*xds_object, 0, sizeof(xds_str_list));
	}
#endif /* 0 */
	return(ret);
} /* end dsP_put_str_list */


/********************************************************/
/*							*/
/*	put	OR_ADDRESS object							*/
/*							*/
/********************************************************/
OM_return_code
dsP_put_OR_address(
	OMX_workspace	workspace,
	void **xds_object,
	OM_object	source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value

)
{
#define	ORA_OBJ	((xds_OR_address *)	*xds_object)

	OM_return_code	ret = OM_SUCCESS;
	OM_sint			i;
	OMP_type_list	 * pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_sint	check;
	OM_descriptor	 * desc_ptr;
	void	**name_obj;

	void	*link;
	void	**pt_obj;
	short	nb_val_d;
	short	n_cur;
	OM_sint	nb_res, nb_end;
	OM_sint	k, len;
	Print_string	 * prt_pt;
	Numeric_string	 * num_pt;
	T61_string		 * tlx_pt;
	OM_value_position	init_val;
	OM_value_position	limit_val;
	OM_descriptor	 * desc_pt;
	OM_sint	j;
	OM_string_length	str_len;
	void	*str_elem;
	OM_string_length	min_len;

	/* syntaxes	to check on	multivalued	attributes defined as
	 * D2_str_list type	in D2_OR_address structure
	 */
	/* the variable	k will be initialised with :
	 *	- 0	: for POSTAL_ADDRESS_IN_LINES
	 *	- 1	: for ADMD_NAME
	 *		  COUNTRY_NAME
	 *		  POSTAL_CODE
	 *		  POSTAL_COUNTRY_NAME
	 *		  PRMD_NAME
	 *	- 2	: for the other	ones
	 * and will	be used	as index in	the	following array	to get the
	 * allowed syntaxes	for	the	specific attribute type
	 */
	OM_syntax	str_stx[3][2] =  {
		/* for POSTAL_ADDRESS_IN_LINES (one	syntax twice) */
		{ OM_S_PRINTABLE_STRING, OM_S_PRINTABLE_STRING },
		/* for ADMD_NAME
			 *	   COUNTRY_NAME
			 *	   POSTAL_CODE
			 *	   POSTAL_COUNTRY_NAME
			 *	   PRMD_NAME
			 * (two	syntaxes)
			 */
		{ OM_S_PRINTABLE_STRING, OM_S_NUMERIC_STRING },
		/* for the other ones */
		{ OM_S_PRINTABLE_STRING, OM_S_TELETEX_STRING }
	};

	OMP_type_list	type_list[]	 = OR_ADDR_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		ALLOCATE_OBJECT(OR_ADDRESS, xds_OR_address);

		init_val = initial_value;
		limit_val = limiting_value;
		CHECK_INIT;
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES;
			pt_l++)	 {
			if (VALUE_NOT_PRESENT)
				continue;

			initial_value = init_val;
			limiting_value = limit_val;

			len	 = 0;
			/* initialise index	in the str_stx array for muli-valued
		 * attributes of D2_str_list type
		 */
			k = 0;
			switch (pt_l->type)	 {
			case MH_T_TERMINAL_TYPE	:
				CHECK_VALID_OPT_ENUM_AND_UPDATE((OM_descriptor *
					)pt_l->desc, ORA_OBJ->d2_terminal_type,
					terminal_type_limit);
				/*
			CHECK_ENUM_AND_UPDATE((OM_descriptor *)pt_l->desc,
						 ORA_OBJ->d2_terminal_type);
*/
				break;

			case MH_T_POSTAL_DELIV_SYSTEM_NAME :
			case MH_T_TERMINAL_IDENTIFIER :
				switch (pt_l->type)	 {
				case MH_T_POSTAL_DELIV_SYSTEM_NAME :
					prt_pt = &ORA_OBJ->d2_post_deliv_sys_name;
					break;
				case MH_T_TERMINAL_IDENTIFIER :
					prt_pt = &ORA_OBJ->d2_terminal_id;
					break;
				}
				CHECK_OPT_SYNTAX_AND_LENGTH(pt_l->desc,
					OM_S_PRINTABLE_STRING, MIN_VL_1, pt_l->max_length);
				UPDATE_STRING(len, (*prt_pt),
				((OM_descriptor *)pt_l->desc)->value.string.length,
				((OM_descriptor *)pt_l->desc)->value.string.elements);
				break;

			case MH_T_ISDN_NUMBER :
			case MH_T_ISDN_SUBADDRESS :
			case MH_T_NUMERIC_USER_IDENTIFIER :
			case MH_T_X121_ADDRESS :
				switch (pt_l->type)	 {
				case MH_T_ISDN_NUMBER :
					num_pt = &ORA_OBJ->d2_isdn_number;
					break;
				case MH_T_ISDN_SUBADDRESS :
					num_pt = &ORA_OBJ->d2_isdn_subaddress;
					break;
				case MH_T_NUMERIC_USER_IDENTIFIER :
					num_pt = &ORA_OBJ->d2_numeric_user_id;
					break;
				case MH_T_X121_ADDRESS :
					num_pt = &ORA_OBJ->d2_x121_addr;
					break;
				}
				CHECK_OPT_SYNTAX_AND_LENGTH(pt_l->desc,
					OM_S_NUMERIC_STRING, MIN_VL_1, pt_l->max_length);
				UPDATE_STRING(len, (*num_pt), ((OM_descriptor *
					)pt_l->desc)->value.string.length, ((OM_descriptor *
					)pt_l->desc)->value.string.elements);
				break;

			case MH_T_POSTAL_ADDRESS_IN_FULL :
				CHECK_OPT_SYNTAX_AND_LENGTH(pt_l->desc,
				OM_S_TELETEX_STRING, MIN_VL_1, pt_l->max_length);
				tlx_pt = &(ORA_OBJ->d2_post_addr_in_full);

				if (OMP_SYNTAX(pt_l->desc) & OM_S_LOCAL_STRING) {
					unsigned char * result; OM_uint32 result_len;
					if ( d27_local_to_t61(
					((OM_descriptor *)pt_l->desc)->value.string.elements,
					((OM_descriptor *)pt_l->desc)->value.string.length,
					(char **)&result, &result_len) != D2_NOERROR) {
						return(OM_WRONG_VALUE_SYNTAX);
					} else {
						((OM_descriptor *)pt_l->desc)->value.string.length 
						= result_len;
						((OM_descriptor *)pt_l->desc)->value.string.elements 
						= result;
						/* possible memory leak */
					} /* endif */
				} /* endif */

				UPDATE_STRING(len, (*tlx_pt),
				((OM_descriptor *) pt_l->desc)->value.string.length,
				((OM_descriptor *)pt_l->desc)->value.string.elements);
				break;

			case MH_T_PRESENTATION_ADDRESS :
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				name_obj = (void **) & (ORA_OBJ->d2_presentation_addr);
				REMOVE_OBJ(psap, name_obj);
				ret	 = dsP_put_psap	(workspace, name_obj,
				((OM_descriptor *)pt_l->desc)->value.object.object,
				object_type, OM_REPLACE_ALL, 0, 0, OM_ALL_VALUES);
				break;

				/* case	for	all	multi-valued attributes	whose
		 * type	is D2_str_list (see	str_stx	array above)
		 *
		 * The first group of these	attributes has 2 possible values
		 * and 2 allowed syntaxes:
		 *	OM_S_PRINTABLE_STRING
		 *	OM_S_TELETEX_STRING
		 * The second group	has	one	possible value
		 * and 2 allowed syntaxes:
		 *	OM_S_PRINTABLE_STRING
		 *	OM_S_NUMERIC_STRING
		 * The third group has 6 possible values
		 * and one allowed syntax:
		 *	OM_S_PRINTABLE_STRING
		 */
			case MH_T_COMMON_NAME :
			case MH_T_DOMAIN_TYPE_1	:
			case MH_T_DOMAIN_TYPE_2	:
			case MH_T_DOMAIN_TYPE_3	:
			case MH_T_DOMAIN_TYPE_4	:
			case MH_T_DOMAIN_VALUE_1 :
			case MH_T_DOMAIN_VALUE_2 :
			case MH_T_DOMAIN_VALUE_3 :
			case MH_T_DOMAIN_VALUE_4 :
			case MH_T_GENERATION :
			case MH_T_GIVEN_NAME :
			case MH_T_INITIALS :
			case MH_T_ORGANIZATION_NAME	:
			case MH_T_ORGANIZATIONAL_UNIT_NAME_1 :
			case MH_T_ORGANIZATIONAL_UNIT_NAME_2 :
			case MH_T_ORGANIZATIONAL_UNIT_NAME_3 :
			case MH_T_ORGANIZATIONAL_UNIT_NAME_4 :
			case MH_T_POSTAL_ADDRESS_DETAILS :
			case MH_T_POSTAL_DELIVERY_POINT_NAME :
			case MH_T_POSTAL_GENERAL_DELIV_ADDR	:
			case MH_T_POSTAL_LOCALE	:
			case MH_T_POSTAL_OFFICE_BOX_NUMBER :
			case MH_T_POSTAL_OFFICE_NAME :
			case MH_T_POSTAL_OFFICE_NUMBER :
			case MH_T_POSTAL_ORGANIZATION_NAME :
			case MH_T_POSTAL_PATRON_DETAILS	:
			case MH_T_POSTAL_PATRON_NAME :
			case MH_T_POSTAL_STREET_ADDRESS	:
			case MH_T_SURNAME :
				/* k will be set to	2 for all the attributes above */
				k++;
			case MH_T_ADMD_NAME	:
			case MH_T_COUNTRY_NAME :
			case MH_T_POSTAL_CODE :
			case MH_T_POSTAL_COUNTRY_NAME :
			case MH_T_PRMD_NAME	:
				/* k will be set to	1 for all the 5	attributes above */
				k++;
			case MH_T_POSTAL_ADDRESS_IN_LINES :
				/* k will be set to	0 for the attribute	above */
				/* The minimum value string	length is 1	for
			 * all attributes except for:
			 * MH_T_ADMD_NAME:		0
			 * MH_T_COUNTRY_NAME:		2
			 * MH_T_POSTAL_COUNTRY_NAME:	2
			 */
				min_len	 = MIN_VL_1;
				switch (pt_l->type)	 {
				case MH_T_COMMON_NAME :
					name_obj = (void *)	 & ORA_OBJ->d2_common_name;
					break;
				case MH_T_DOMAIN_TYPE_1	:
					name_obj = (void *)	 & ORA_OBJ->d2_domain_type1;
					break;
				case MH_T_DOMAIN_TYPE_2	:
					name_obj = (void *)	 & ORA_OBJ->d2_domain_type2;
					break;
				case MH_T_DOMAIN_TYPE_3	:
					name_obj = (void *)	 & ORA_OBJ->d2_domain_type3;
					break;
				case MH_T_DOMAIN_TYPE_4	:
					name_obj = (void *)	 & ORA_OBJ->d2_domain_type4;
					break;
				case MH_T_DOMAIN_VALUE_1 :
					name_obj = (void *)	 & ORA_OBJ->d2_domain_value1;
					break;
				case MH_T_DOMAIN_VALUE_2 :
					name_obj = (void *)	 & ORA_OBJ->d2_domain_value2;
					break;
				case MH_T_DOMAIN_VALUE_3 :
					name_obj = (void *)	 & ORA_OBJ->d2_domain_value3;
					break;
				case MH_T_DOMAIN_VALUE_4 :
					name_obj = (void *)	 & ORA_OBJ->d2_domain_value4;
					break;
				case MH_T_GENERATION :
					name_obj = (void *)	 & ORA_OBJ->d2_generation;
					break;
				case MH_T_GIVEN_NAME :
					name_obj = (void *)	 & ORA_OBJ->d2_given_name;
					break;
				case MH_T_INITIALS :
					name_obj = (void *)	 & ORA_OBJ->d2_initials;
					break;
				case MH_T_ORGANIZATION_NAME	:
					name_obj = (void *)	 & ORA_OBJ->d2_org_name;
					break;
				case MH_T_ORGANIZATIONAL_UNIT_NAME_1 :
					name_obj = (void *)	 & ORA_OBJ->d2_org_unit_name1;
					break;
				case MH_T_ORGANIZATIONAL_UNIT_NAME_2 :
					name_obj = (void *)	 & ORA_OBJ->d2_org_unit_name2;
					break;
				case MH_T_ORGANIZATIONAL_UNIT_NAME_3 :
					name_obj = (void *)	 & ORA_OBJ->d2_org_unit_name3;
					break;
				case MH_T_ORGANIZATIONAL_UNIT_NAME_4 :
					name_obj = (void *)	 & ORA_OBJ->d2_org_unit_name4;
					break;
				case MH_T_POSTAL_ADDRESS_DETAILS :
					name_obj = (void *)	 & ORA_OBJ->d2_post_addr_details;
					break;
				case MH_T_POSTAL_DELIVERY_POINT_NAME :
					name_obj = (void *)	 & ORA_OBJ->d2_post_deliv_pt_name;
					break;
				case MH_T_POSTAL_GENERAL_DELIV_ADDR	:
					name_obj = (void *)	 & ORA_OBJ->d2_post_gen_deliv_addr;
					break;
				case MH_T_POSTAL_LOCALE	:
					name_obj = (void *)	 & ORA_OBJ->d2_post_locale;
					break;
				case MH_T_POSTAL_OFFICE_BOX_NUMBER :
					name_obj = (void *)	 & ORA_OBJ->d2_post_office_box_nbr;
					break;
				case MH_T_POSTAL_OFFICE_NAME :
					name_obj = (void *)	 & ORA_OBJ->d2_post_office_name;
					break;
				case MH_T_POSTAL_OFFICE_NUMBER :
					name_obj = (void *)	 & ORA_OBJ->d2_post_office_nbr;
					break;
				case MH_T_POSTAL_ORGANIZATION_NAME :
					name_obj = (void *)	 & ORA_OBJ->d2_post_org_name;
					break;
				case MH_T_POSTAL_PATRON_DETAILS	:
					name_obj = (void *)	 & ORA_OBJ->d2_post_patron_details;
					break;
				case MH_T_POSTAL_PATRON_NAME :
					name_obj = (void *)	 & ORA_OBJ->d2_post_patron_name;
					break;
				case MH_T_POSTAL_STREET_ADDRESS	:
					name_obj = (void *)	 & ORA_OBJ->d2_post_street_addr;
					break;
				case MH_T_SURNAME :
					name_obj = (void *)	 & ORA_OBJ->d2_surname;
					break;

				case MH_T_ADMD_NAME	:
					name_obj = (void *)	 & ORA_OBJ->d2_admd_name;
					min_len	 = MIN_VL_ADMD_NAME;
					break;
				case MH_T_COUNTRY_NAME :
					name_obj = (void *)	 & ORA_OBJ->d2_country_name;
					min_len	 = MIN_VL_COUNTRY_NAME;
					break;
				case MH_T_POSTAL_CODE :
					name_obj = (void *)	 & ORA_OBJ->d2_post_code;
					break;
				case MH_T_POSTAL_COUNTRY_NAME :
					name_obj = (void *)	 & ORA_OBJ->d2_post_country_name;
					min_len	 = MIN_VL_POSTAL_COUNTRY_NAME;
					break;
				case MH_T_PRMD_NAME	:
					name_obj = (void *)	 & ORA_OBJ->d2_prmd_name;
					break;

				case MH_T_POSTAL_ADDRESS_IN_LINES :
					name_obj = (void *)	 & ORA_OBJ->d2_post_addr_in_lines;
					break;
				}
				/* check all the values	to put in the OR_address */
				for	(j = 0, desc_pt	 = pt_l->desc; j <
					pt_l->nb_val; j++, desc_pt++) {
					CHECK_OPT_TWO_SYNTAXES_AND_LENGTH(desc_pt,
						 str_stx[k][0], str_stx[k][1],
						min_len,
						pt_l->max_length);
				}
				if (check)
					break;
				/* if necessary,
			 * update initial and limiting values with
			 * the existing	number of values
			 */
				GET_NB_OBJ(xds_str_list, d2_next, nb_val_d,
					 name_obj, pt_obj);
				UPDATE_LIMIT_VAL(nb_val_d, break);
				nb_res = nb_val_d -		limiting_value;
				if (OMP_REMOVE)
					pt_l->nb_val = 0;
				/* check if	the	number of values of	the	attribute
			 * after modification is applied would not exceed the
			 * max allowed number
			 */
				if ((nb_end	 = initial_value	 + pt_l->nb_val	+
					nb_res)	 > pt_l->max_val) {
					ret	 = OM_WRONG_VALUE_NUMBER;
					break;
				}

				/* jump	values from	the	begining to	initial	value */
				JUMP_N_OBJ(xds_str_list, d2_next, initial_value,
						name_obj, pt_obj, n_cur);

				/* copy	the	new	objects	*/
				if (OMP_INSERT)	 {
					ret	 = dsP_put_str_list(workspace, xds_object,
						 modification, pt_l, pt_obj);
				} else if (OMP_REPLACE)	 {
					/*delete the objects which have	to be replaced*/
					DELETE_OBJ(xds_str_list, limiting_value	 -
						initial_value, d2_next, dsP_del_a_str);

					/* replace them	by the new ones	*/
					ret	 = dsP_put_str_list(workspace, xds_object,
						 modification, pt_l, pt_obj);
				}
				break;

			}
			if (ret	 != OM_SUCCESS)
				break;
		}
		CHECK_END;
	}
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_OR_address	*/


/********************************************************/
/*							*/
/*	put	OR_NAME	object								*/
/*							*/
/********************************************************/
OM_return_code
dsP_put_OR_name(
	OMX_workspace	workspace,
	void **xds_object,
	OM_object	source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value
)
{
#define	ORN_OBJ	((xds_OR_name *) *xds_object)

	OM_return_code	ret = OM_SUCCESS;
	OM_descriptor	 * desc_ptr;
	OMP_type_list	 * pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_sint	check;
	void	**name_obj;
	OM_sint	check_add = 1;
	OM_sint	list_cnt, k;
	OM_type_list	list, new_list;

	OMP_type_list	type_list[]	 = OR_NAME_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		ALLOCATE_OBJECT(OR_NAME, xds_OR_name);
		CHECK_INIT;
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES;
			pt_l++)	 {
			if (VALUE_NOT_PRESENT)
				continue;

			switch (pt_l->type)	 {
			case MH_T_DIRECTORY_NAME :
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				name_obj = (void **) & ORN_OBJ->d2_dir_name;
				REMOVE_OBJ(dist_name, name_obj);
				ret	 = dsP_put_dist_name	(workspace, name_obj,
					 ((OM_descriptor * )pt_l->desc)->value.object.object,
																																			 object_type,
					 OM_REPLACE_ALL, 0, 0, OM_ALL_VALUES);
				break;

			default	:
				/* case	for	all	OR_address attributes type */
				if ((check)	 &&	(check_add))	 {
					/* call	dsP_put_OR_address one time	with the complete
			 * source object
			 */
					/* build an	included types list	without	the
				 * MH_T_DIRECTORY_NAME type
				 */
					list_cnt = 0;
					new_list = 0;
					if (included_types)	 {
						/* first calculate the list	length */
						list = included_types;
						while (*list !=  OM_NO_MORE_TYPES) {
							if (*list++	 !=
								MH_T_DIRECTORY_NAME)
								list_cnt++;
						}
						/* + 1 for the end of list */
						list_cnt++;
						/* allocate	a STRING buffer	*/
						if ((new_list =  (OM_type_list)OMP_XALLOC(workspace,
							 AL_STRING, ROUNDUP(workspace,
																																																list_cnt *
							sizeof(OM_type)))) ==  0) {
							ret	 = OM_MEMORY_INSUFFICIENT;
							break;
						}
						/* constitute the new list for dsP_put_OR_address */
						for	(k = 0, list = included_types;
							*list != OM_NO_MORE_TYPES;
							list++)	 {
							if (*list != MH_T_DIRECTORY_NAME)
								new_list[k++] =
									*list;
						}
						/* set the end of the list */
						new_list[k]	 = OM_NO_MORE_TYPES;
					}
					name_obj = (void **) &	ORN_OBJ->d2_or_address;
					ret	 = dsP_put_OR_address (workspace,
						 name_obj, source, object_type,
						modification, new_list,
						initial_value, limiting_value);
					check_add = 0;
					/* free	the	previous allocated STRING buffer */
					if (new_list)
						OMP_XFREE(workspace, new_list,
							 AL_STRING, ROUNDUP(workspace,
							 list_cnt *
							sizeof(OM_type)));
				}
				break;

			}
			if (ret	 != OM_SUCCESS)
				break;
		}
		CHECK_END;
	}
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_OR_name */


/********************************************************/
/*							*/
/*	put	DL_SUBMIT_PERMISSION object					*/
/*							*/
/********************************************************/
OM_return_code
dsP_put_dlsp(
	OMX_workspace	workspace,
	void **xds_object,
	OM_object	source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value

)
{
#define	DL_OBJ	((xds_dl_submit_perm *)	*xds_object)

	OM_return_code	ret = OM_SUCCESS;
	OMP_type_list	 * pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_sint	check;
	OM_descriptor	 * desc_ptr;
	void	**name_obj;
	OM_sint	old_type, new_type;

	OMP_type_list	type_list[]	 = DL_SP_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		ALLOCATE_OBJECT(DL_SUBMIT_PERMS, xds_dl_submit_perm);
		CHECK_INIT;
		new_type = old_type	 = DL_OBJ->d2_perm_type;
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES;
			pt_l++)	 {
			if (VALUE_NOT_PRESENT)
				continue;

			switch (pt_l->type)	 {
			case DS_PERM_TYPE :
				new_type = ((OM_descriptor * )pt_l->desc)->value.enumeration;
				CHECK_VALID_ENUM_AND_UPDATE((OM_descriptor *
					)pt_l->desc, DL_OBJ->d2_perm_type, perm_type_limit);
				/*
			CHECK_ENUM_AND_UPDATE((OM_descriptor *)pt_l->desc,
						DL_OBJ->d2_perm_type);
*/
				if ((old_type == DS_PERM_MEMBER_OF_GROUP) &&
					(new_type != DS_PERM_MEMBER_OF_GROUP)) {
					dsP_del_dist_name(workspace, DL_OBJ->d2_choice.d2_member_of_group);
					DL_OBJ->d2_choice.d2_member_of_group =
						0;
				} else if ((new_type == DS_PERM_MEMBER_OF_GROUP) &&
					(old_type != DS_PERM_MEMBER_OF_GROUP)) {
					dsP_del_OR_name(workspace, DL_OBJ->d2_choice.d2_individual);
					DL_OBJ->d2_choice.d2_individual	 =
						0;
				}

				break;
			case DS_INDIVIDUAL :
				CHECK_PERM_TYPE(new_type, DS_PERM_INDIVIDUAL);
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				dsP_del_OR_name(workspace, DL_OBJ->d2_choice.d2_individual);
				DL_OBJ->d2_choice.d2_individual	 = 0;
				if (OMP_REMOVE)	 {
					ret	 = OM_SUCCESS;
					break;
				}
				name_obj = (void **)
				&DL_OBJ->d2_choice.d2_individual;
				ret	 = dsP_put_OR_name (workspace, name_obj,
					 ((OM_descriptor * )pt_l->desc)->value.object.object,
																																			 object_type,
					 OM_REPLACE_ALL, 0, 0, OM_ALL_VALUES);
				break;
			case DS_MEMBER_OF_DL :
				CHECK_PERM_TYPE(new_type, DS_PERM_MEMBER_OF_DL);
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				dsP_del_OR_name(workspace, DL_OBJ->d2_choice.d2_member_of_dl);
				DL_OBJ->d2_choice.d2_member_of_dl =  0;
				if (OMP_REMOVE)	 {
					ret	 = OM_SUCCESS;
					break;
				}
				name_obj = (void **)
				&DL_OBJ->d2_choice.d2_member_of_dl;
				ret	 = dsP_put_OR_name (workspace, name_obj,
					 ((OM_descriptor * )pt_l->desc)->value.object.object,
																																			 object_type,
					 OM_REPLACE_ALL, 0, 0, OM_ALL_VALUES);
				break;
			case DS_PATTERN_MATCH :
				CHECK_PERM_TYPE(new_type, DS_PERM_PATTERN_MATCH);
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				dsP_del_OR_name(workspace, DL_OBJ->d2_choice.d2_pattern_match);
				DL_OBJ->d2_choice.d2_pattern_match =  0;
				if (OMP_REMOVE)	 {
					ret	 = OM_SUCCESS;
					break;
				}
				name_obj = (void **)
				&DL_OBJ->d2_choice.d2_pattern_match;
				ret	 = dsP_put_OR_name (workspace, name_obj,
					 ((OM_descriptor * )pt_l->desc)->value.object.object,
																																			 object_type,
					 OM_REPLACE_ALL, 0, 0, OM_ALL_VALUES);
				break;
			case DS_MEMBER_OF_GROUP	:
				CHECK_PERM_TYPE(new_type, DS_PERM_MEMBER_OF_GROUP);
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				name_obj = (void **)
				&DL_OBJ->d2_choice.d2_member_of_group;
				REMOVE_OBJ(dist_name, name_obj);
				ret	 = dsP_put_dist_name	(workspace, name_obj,
					 ((OM_descriptor * )pt_l->desc)->value.object.object,
																																			 object_type,
					 OM_REPLACE_ALL, 0, 0, OM_ALL_VALUES);
				break;
			}
			if (ret	 != OM_SUCCESS)
				break;
		}
		CHECK_END;
	}
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_dlsp */


/********************************************************/
/*							*/
/*	get	OR_ADDRESS object							*/
/*							*/
/********************************************************/
OM_return_code
dsP_get_OR_address(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_OR_address		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number)
{
	OMP_type_list	types_list[] = OR_ADDR_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint	nb_desc		 = 0;
	OM_descriptor	 * ptr_desc;
	OM_descriptor	 * desc;
	OM_descriptor	 * desc0;
	 OMP_type_list *pt_l;
	OM_boolean initialize;
	 xds_str_list	**adr_obj;
	OM_sint	n_cur, i;
	Print_string	prt_obj;
	Numeric_string	num_obj;
	D2_str_list		 * *str_obj;
	OM_syntax		syntax;
	OM_value_position	init_val;
	OM_value_position	limit_val;

	VALIDATE_TYPE_LIST(types_list, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {
		init_val = initial_value;
		limit_val = limiting_value;

		for	(pt_l = types_list;	pt_l->type != OM_NO_MORE_TYPES;
			pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0)
				continue;
			initial_value = init_val;
			limiting_value = limit_val;

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_MH_C_OR_ADDRESS);

				break;
			case MH_T_TERMINAL_TYPE	:
				if ((!xobject) || (!xobject->d2_terminal_type))
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_ENUM(ptr_desc, MH_T_TERMINAL_TYPE,
					 xobject->d2_terminal_type, 1);
				break;

			case MH_T_POSTAL_DELIV_SYSTEM_NAME :
			case MH_T_TERMINAL_IDENTIFIER :
				if (!xobject)
					break;
				switch (pt_l->type)	 {
				case MH_T_POSTAL_DELIV_SYSTEM_NAME :
					prt_obj	 = xobject->d2_post_deliv_sys_name;
					break;
				case MH_T_TERMINAL_IDENTIFIER :
					prt_obj	 = xobject->d2_terminal_id;
					break;
				}
				if (!prt_obj)
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, pt_l->type, OM_S_PRINTABLE_STRING,
				strlen((char *)prt_obj), prt_obj, 1);
				break;

			case MH_T_ISDN_NUMBER :
			case MH_T_ISDN_SUBADDRESS :
			case MH_T_NUMERIC_USER_IDENTIFIER :
			case MH_T_X121_ADDRESS :
				if (!xobject)
					break;
				switch (pt_l->type)	 {
				case MH_T_ISDN_NUMBER :
					num_obj	 = xobject->d2_isdn_number;
					break;
				case MH_T_ISDN_SUBADDRESS :
					num_obj	 = xobject->d2_isdn_subaddress;
					break;
				case MH_T_NUMERIC_USER_IDENTIFIER :
					num_obj	 = xobject->d2_numeric_user_id;
					break;
				case MH_T_X121_ADDRESS :
					num_obj	 = xobject->d2_x121_addr;
					break;
				}
				if (!num_obj)
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, pt_l->type, OM_S_NUMERIC_STRING,
						 strlen((char
				*)num_obj), num_obj, 1);
				break;

			case MH_T_PRESENTATION_ADDRESS :
				if ((!xobject) || (!xobject->d2_presentation_addr))
					break;
				INC_NBDESC_MONO;
				OBJECT_VALUE(psap, dsP_DS_C_PRESENTATION_ADDRESS,
				xobject->d2_presentation_addr, pt_l->type, 1);
				break;

			case MH_T_POSTAL_ADDRESS_IN_FULL :
				if ((!xobject) || (!xobject->d2_post_addr_in_full))
					break;
				INC_NBDESC_MONO;
				if (local_strings) {
					char *result;
					OM_uint32 result_len = 0;
					if (d27_t61_to_local(
					(char *)xobject->d2_post_addr_in_full,
					(unsigned32)strlen((char*)xobject->d2_post_addr_in_full),
					&result,&result_len) != D2_NOERROR) {
						ret	 = OM_WRONG_VALUE_SYNTAX;
						break;
					} else {
						xobject->d2_post_addr_in_full = (unsigned char*)result;
						/* possible memory leak here */
					} /* endif */
				} /* endif */
				UPDATE_DESC_STR(ptr_desc, pt_l->type,
				local_strings?OM_S_TELETEX_STRING | OM_S_LOCAL_STRING:
				OM_S_TELETEX_STRING,
				strlen((char *)xobject->d2_post_addr_in_full),
				xobject->d2_post_addr_in_full, 1);
				break;

			case MH_T_ADMD_NAME	:
			case MH_T_COUNTRY_NAME :
			case MH_T_POSTAL_CODE :
			case MH_T_POSTAL_COUNTRY_NAME :
			case MH_T_PRMD_NAME	:
			case MH_T_COMMON_NAME :
			case MH_T_DOMAIN_TYPE_1	:
			case MH_T_DOMAIN_TYPE_2	:
			case MH_T_DOMAIN_TYPE_3	:
			case MH_T_DOMAIN_TYPE_4	:
			case MH_T_DOMAIN_VALUE_1 :
			case MH_T_DOMAIN_VALUE_2 :
			case MH_T_DOMAIN_VALUE_3 :
			case MH_T_DOMAIN_VALUE_4 :
			case MH_T_GENERATION :
			case MH_T_GIVEN_NAME :
			case MH_T_INITIALS :
			case MH_T_ORGANIZATION_NAME	:
			case MH_T_ORGANIZATIONAL_UNIT_NAME_1 :
			case MH_T_ORGANIZATIONAL_UNIT_NAME_2 :
			case MH_T_ORGANIZATIONAL_UNIT_NAME_3 :
			case MH_T_ORGANIZATIONAL_UNIT_NAME_4 :
			case MH_T_POSTAL_ADDRESS_DETAILS :
			case MH_T_POSTAL_DELIVERY_POINT_NAME :
			case MH_T_POSTAL_GENERAL_DELIV_ADDR	:
			case MH_T_POSTAL_LOCALE	:
			case MH_T_POSTAL_OFFICE_BOX_NUMBER :
			case MH_T_POSTAL_OFFICE_NAME :
			case MH_T_POSTAL_OFFICE_NUMBER :
			case MH_T_POSTAL_ORGANIZATION_NAME :
			case MH_T_POSTAL_PATRON_DETAILS	:
			case MH_T_POSTAL_PATRON_NAME :
			case MH_T_POSTAL_STREET_ADDRESS	:
			case MH_T_SURNAME :
			case MH_T_POSTAL_ADDRESS_IN_LINES :
				if (!xobject)
					break;
				switch (pt_l->type)	 {
				case MH_T_ADMD_NAME	:
					str_obj	 = &xobject->d2_admd_name;
					break;
				case MH_T_COUNTRY_NAME :
					str_obj	 = &xobject->d2_country_name;
					break;
				case MH_T_POSTAL_CODE :
					str_obj	 = &xobject->d2_post_code;
					break;
				case MH_T_POSTAL_COUNTRY_NAME :
					str_obj	 = &xobject->d2_post_country_name;
					break;
				case MH_T_PRMD_NAME	:
					str_obj	 = &xobject->d2_prmd_name;
					break;
				case MH_T_COMMON_NAME :
					str_obj	 = &xobject->d2_common_name;
					break;
				case MH_T_DOMAIN_TYPE_1	:
					str_obj	 = &xobject->d2_domain_type1;
					break;
				case MH_T_DOMAIN_TYPE_2	:
					str_obj	 = &xobject->d2_domain_type2;
					break;
				case MH_T_DOMAIN_TYPE_3	:
					str_obj	 = &xobject->d2_domain_type3;
					break;
				case MH_T_DOMAIN_TYPE_4	:
					str_obj	 = &xobject->d2_domain_type4;
					break;
				case MH_T_DOMAIN_VALUE_1 :
					str_obj	 = &xobject->d2_domain_value1;
					break;
				case MH_T_DOMAIN_VALUE_2 :
					str_obj	 = &xobject->d2_domain_value2;
					break;
				case MH_T_DOMAIN_VALUE_3 :
					str_obj	 = &xobject->d2_domain_value3;
					break;
				case MH_T_DOMAIN_VALUE_4 :
					str_obj	 = &xobject->d2_domain_value4;
					break;
				case MH_T_GENERATION :
					str_obj	 = &xobject->d2_generation;
					break;
				case MH_T_GIVEN_NAME :
					str_obj	 = &xobject->d2_given_name;
					break;
				case MH_T_INITIALS :
					str_obj	 = &xobject->d2_initials;
					break;
				case MH_T_ORGANIZATION_NAME	:
					str_obj	 = &xobject->d2_org_name;
					break;
				case MH_T_ORGANIZATIONAL_UNIT_NAME_1 :
					str_obj	 = &xobject->d2_org_unit_name1;
					break;
				case MH_T_ORGANIZATIONAL_UNIT_NAME_2 :
					str_obj	 = &xobject->d2_org_unit_name2;
					break;
				case MH_T_ORGANIZATIONAL_UNIT_NAME_3 :
					str_obj	 = &xobject->d2_org_unit_name3;
					break;
				case MH_T_ORGANIZATIONAL_UNIT_NAME_4 :
					str_obj	 = &xobject->d2_org_unit_name4;
					break;
				case MH_T_POSTAL_ADDRESS_DETAILS :
					str_obj	 = &xobject->d2_post_addr_details;
					break;
				case MH_T_POSTAL_DELIVERY_POINT_NAME :
					str_obj	 = &xobject->d2_post_deliv_pt_name;
					break;
				case MH_T_POSTAL_GENERAL_DELIV_ADDR	:
					str_obj	 = &xobject->d2_post_gen_deliv_addr;
					break;
				case MH_T_POSTAL_LOCALE	:
					str_obj	 = &xobject->d2_post_locale;
					break;
				case MH_T_POSTAL_OFFICE_BOX_NUMBER :
					str_obj	 = &xobject->d2_post_office_box_nbr;
					break;
				case MH_T_POSTAL_OFFICE_NAME :
					str_obj	 = &xobject->d2_post_office_name;
					break;
				case MH_T_POSTAL_OFFICE_NUMBER :
					str_obj	 = &xobject->d2_post_office_nbr;
					break;
				case MH_T_POSTAL_ORGANIZATION_NAME :
					str_obj	 = &xobject->d2_post_org_name;
					break;
				case MH_T_POSTAL_PATRON_DETAILS	:
					str_obj	 = &xobject->d2_post_patron_details;
					break;
				case MH_T_POSTAL_PATRON_NAME :
					str_obj	 = &xobject->d2_post_patron_name;
					break;
				case MH_T_POSTAL_STREET_ADDRESS	:
					str_obj	 = &xobject->d2_post_street_addr;
					break;
				case MH_T_SURNAME :
					str_obj	 = &xobject->d2_surname;
					break;
				case MH_T_POSTAL_ADDRESS_IN_LINES :
					str_obj	 = &xobject->d2_post_addr_in_lines;
					break;
				}
				if (!(*str_obj))
					break;
				if (!initialize) {
					GET_NB_OBJ(xds_str_list, d2_next,
						 pt_l->max_val, (xds_str_list *
						*)str_obj, adr_obj);
					UPDATE_LIMIT_VALUE(pt_l->max_val);
					JUMP_N_OBJ(xds_str_list, d2_next,
						 initial_value, str_obj, adr_obj,
						 n_cur);
					pt_l->desc = adr_obj;
					for	(i = 0;	i <		pt_l->nb_val; i++,
						 adr_obj = &(*adr_obj)->d2_next) {
						if ((!(*adr_obj)->d2_str.d2_size) ||
							(!(*adr_obj)->d2_str.d2_value))
							continue;
						nb_desc++;
					}
					break;
				}
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
					case D2_NUMERIC	:
						syntax = IAPL_TO_OM[(*adr_obj)->d2_rep];
						break;
					}
					UPDATE_DESC_STR(ptr_desc, pt_l->type,
						 syntax, (*adr_obj)->d2_str.d2_size,
						 (*adr_obj)->d2_str.d2_value,
						1);
				}
				break;

			}
			if ((initialize) &&		(ret != OM_SUCCESS))
				clean_before_exit(desc0, ptr_desc, nb_desc,
					 ret);
		}
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize)
			break;
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		}

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS)
			return(ret);

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	}

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;

end:
	return(ret);
} /* end dsP_get_OR_address	*/


/********************************************************/
/*							*/
/*	get	OR_NAME	object								*/
/*							*/
/********************************************************/
OM_return_code
dsP_get_OR_name(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_OR_name		*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number)
{
	OMP_type_list	types_list[] = OR_NAME_TYPE_LIST;
	OM_return_code	ret	 = OM_SUCCESS;
	OM_sint	nb_desc		 = 0;
	OM_descriptor	 * ptr_desc;
	OM_descriptor	 * desc;
	OM_descriptor	 * desc0;
	 OMP_type_list *pt_l;
	OM_boolean initialize;
	 xds_str_list	**adr_obj;
	OM_sint	n_cur, i;
	Print_string	prt_obj;
	Numeric_string	num_obj;
	D2_str_list		 * *str_obj;
	OM_syntax		syntax;

	VALIDATE_TYPE_LIST(types_list, included_types);

	/* update number of	values for each	included attribute type	*/
	initialize = OM_FALSE;
	while (1) {

		for	(pt_l = types_list;	pt_l->type != OM_NO_MORE_TYPES;
			pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0)
				continue;


			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_MH_C_OR_NAME);

				break;
			case MH_T_DIRECTORY_NAME :
				if ((!xobject) || (!xobject->d2_dir_name))
					break;
				INC_NBDESC_MONO;
				OBJECT_VALUE(dn, dsP_DS_C_DS_DN, xobject->d2_dir_name,
				MH_T_DIRECTORY_NAME, 1)
				break;
			case MH_T_TERMINAL_TYPE	:
				if ((!(xobject->d2_or_address))	 ||	 (!(xobject->d2_or_address->d2_terminal_type)))
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_ENUM(ptr_desc, MH_T_TERMINAL_TYPE,
					 (xobject->d2_or_address)->d2_terminal_type,
																														 1);
				break;

			case MH_T_POSTAL_DELIV_SYSTEM_NAME :
			case MH_T_TERMINAL_IDENTIFIER :
				if (!(xobject->d2_or_address))
					break;
				switch (pt_l->type)	 {
				case MH_T_POSTAL_DELIV_SYSTEM_NAME :
					prt_obj	 = (xobject->d2_or_address)->d2_post_deliv_sys_name;
					break;
				case MH_T_TERMINAL_IDENTIFIER :
					prt_obj	 = (xobject->d2_or_address)->d2_terminal_id;
					break;
				}
				if (!prt_obj)
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, pt_l->type, OM_S_PRINTABLE_STRING,
							 strlen((char
				*)prt_obj), prt_obj, 1);
				break;

			case MH_T_ISDN_NUMBER :
			case MH_T_ISDN_SUBADDRESS :
			case MH_T_NUMERIC_USER_IDENTIFIER :
			case MH_T_X121_ADDRESS :
				if (!(xobject->d2_or_address))
					break;
				switch (pt_l->type)	 {
				case MH_T_ISDN_NUMBER :
					num_obj	 = (xobject->d2_or_address)->d2_isdn_number;
					break;
				case MH_T_ISDN_SUBADDRESS :
					num_obj	 = (xobject->d2_or_address)->d2_isdn_subaddress;
					break;
				case MH_T_NUMERIC_USER_IDENTIFIER :
					num_obj	 = (xobject->d2_or_address)->d2_numeric_user_id;
					break;
				case MH_T_X121_ADDRESS :
					num_obj	 = (xobject->d2_or_address)->d2_x121_addr;
					break;
				}
				if (!num_obj)
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_STR(ptr_desc, pt_l->type, OM_S_NUMERIC_STRING,
						 strlen((char
				*)num_obj), num_obj, 1);
				break;

			case MH_T_PRESENTATION_ADDRESS :
				if ((!(xobject->d2_or_address))	 ||	 (!(xobject->d2_or_address)->d2_presentation_addr))
					break;
				INC_NBDESC_MONO;
				OBJECT_VALUE(psap, dsP_DS_C_PRESENTATION_ADDRESS,
				(xobject->d2_or_address)->d2_presentation_addr, pt_l->type, 1);
				break;

			case MH_T_POSTAL_ADDRESS_IN_FULL :
				if ((!(xobject->d2_or_address))	 
				|| (!(xobject->d2_or_address)->d2_post_addr_in_full)) {
					break;
				} /* endif */
				INC_NBDESC_MONO;
				if (local_strings) {
					char *result;
					OM_uint32 result_len = 0;
					if (d27_t61_to_local(
					(char *)(xobject->d2_or_address)->d2_post_addr_in_full,
					(unsigned32)strlen((char*)(xobject->d2_or_address)->
					d2_post_addr_in_full),
					&result,&result_len) != D2_NOERROR) {
						ret	 = OM_WRONG_VALUE_SYNTAX;
						break;
					} else {
						(xobject->d2_or_address)->d2_post_addr_in_full
						= (unsigned char*)result;
						/* possible memory leak here */
					} /* endif */
				} /* endif */
				UPDATE_DESC_STR(ptr_desc, pt_l->type,
				local_strings?OM_S_TELETEX_STRING|OM_S_LOCAL_STRING:OM_S_TELETEX_STRING ,
				strlen((char *)(xobject->d2_or_address)->d2_post_addr_in_full),
				(char *)(xobject->d2_or_address)->d2_post_addr_in_full, 1);
				break;

			case MH_T_ADMD_NAME	:
			case MH_T_COUNTRY_NAME :
			case MH_T_POSTAL_CODE :
			case MH_T_POSTAL_COUNTRY_NAME :
			case MH_T_PRMD_NAME	:
			case MH_T_COMMON_NAME :
			case MH_T_DOMAIN_TYPE_1	:
			case MH_T_DOMAIN_TYPE_2	:
			case MH_T_DOMAIN_TYPE_3	:
			case MH_T_DOMAIN_TYPE_4	:
			case MH_T_DOMAIN_VALUE_1 :
			case MH_T_DOMAIN_VALUE_2 :
			case MH_T_DOMAIN_VALUE_3 :
			case MH_T_DOMAIN_VALUE_4 :
			case MH_T_GENERATION :
			case MH_T_GIVEN_NAME :
			case MH_T_INITIALS :
			case MH_T_ORGANIZATION_NAME	:
			case MH_T_ORGANIZATIONAL_UNIT_NAME_1 :
			case MH_T_ORGANIZATIONAL_UNIT_NAME_2 :
			case MH_T_ORGANIZATIONAL_UNIT_NAME_3 :
			case MH_T_ORGANIZATIONAL_UNIT_NAME_4 :
			case MH_T_POSTAL_ADDRESS_DETAILS :
			case MH_T_POSTAL_DELIVERY_POINT_NAME :
			case MH_T_POSTAL_GENERAL_DELIV_ADDR	:
			case MH_T_POSTAL_LOCALE	:
			case MH_T_POSTAL_OFFICE_BOX_NUMBER :
			case MH_T_POSTAL_OFFICE_NAME :
			case MH_T_POSTAL_OFFICE_NUMBER :
			case MH_T_POSTAL_ORGANIZATION_NAME :
			case MH_T_POSTAL_PATRON_DETAILS	:
			case MH_T_POSTAL_PATRON_NAME :
			case MH_T_POSTAL_STREET_ADDRESS	:
			case MH_T_SURNAME :
			case MH_T_POSTAL_ADDRESS_IN_LINES :
				if (!(xobject->d2_or_address))
					break;
				switch (pt_l->type)	 {
				case MH_T_ADMD_NAME	:
					str_obj	 = &(xobject->d2_or_address)->d2_admd_name;
					break;
				case MH_T_COUNTRY_NAME :
					str_obj	 = &(xobject->d2_or_address)->d2_country_name;
					break;
				case MH_T_POSTAL_CODE :
					str_obj	 = &(xobject->d2_or_address)->d2_post_code;
					break;
				case MH_T_POSTAL_COUNTRY_NAME :
					str_obj	 = &(xobject->d2_or_address)->d2_post_country_name;
					break;
				case MH_T_PRMD_NAME	:
					str_obj	 = &(xobject->d2_or_address)->d2_prmd_name;
					break;
				case MH_T_COMMON_NAME :
					str_obj	 = &(xobject->d2_or_address)->d2_common_name;
					break;
				case MH_T_DOMAIN_TYPE_1	:
					str_obj	 = &(xobject->d2_or_address)->d2_domain_type1;
					break;
				case MH_T_DOMAIN_TYPE_2	:
					str_obj	 = &(xobject->d2_or_address)->d2_domain_type2;
					break;
				case MH_T_DOMAIN_TYPE_3	:
					str_obj	 = &(xobject->d2_or_address)->d2_domain_type3;
					break;
				case MH_T_DOMAIN_TYPE_4	:
					str_obj	 = &(xobject->d2_or_address)->d2_domain_type4;
					break;
				case MH_T_DOMAIN_VALUE_1 :
					str_obj	 = &(xobject->d2_or_address)->d2_domain_value1;
					break;
				case MH_T_DOMAIN_VALUE_2 :
					str_obj	 = &(xobject->d2_or_address)->d2_domain_value2;
					break;
				case MH_T_DOMAIN_VALUE_3 :
					str_obj	 = &(xobject->d2_or_address)->d2_domain_value3;
					break;
				case MH_T_DOMAIN_VALUE_4 :
					str_obj	 = &(xobject->d2_or_address)->d2_domain_value4;
					break;
				case MH_T_GENERATION :
					str_obj	 = &(xobject->d2_or_address)->d2_generation;
					break;
				case MH_T_GIVEN_NAME :
					str_obj	 = &(xobject->d2_or_address)->d2_given_name;
					break;
				case MH_T_INITIALS :
					str_obj	 = &(xobject->d2_or_address)->d2_initials;
					break;
				case MH_T_ORGANIZATION_NAME	:
					str_obj	 = &(xobject->d2_or_address)->d2_org_name;
					break;
				case MH_T_ORGANIZATIONAL_UNIT_NAME_1 :
					str_obj	 = &(xobject->d2_or_address)->d2_org_unit_name1;
					break;
				case MH_T_ORGANIZATIONAL_UNIT_NAME_2 :
					str_obj	 = &(xobject->d2_or_address)->d2_org_unit_name2;
					break;
				case MH_T_ORGANIZATIONAL_UNIT_NAME_3 :
					str_obj	 = &(xobject->d2_or_address)->d2_org_unit_name3;
					break;
				case MH_T_ORGANIZATIONAL_UNIT_NAME_4 :
					str_obj	 = &(xobject->d2_or_address)->d2_org_unit_name4;
					break;
				case MH_T_POSTAL_ADDRESS_DETAILS :
					str_obj	 = &(xobject->d2_or_address)->d2_post_addr_details;
					break;
				case MH_T_POSTAL_DELIVERY_POINT_NAME :
					str_obj	 = &(xobject->d2_or_address)->d2_post_deliv_pt_name;
					break;
				case MH_T_POSTAL_GENERAL_DELIV_ADDR	:
					str_obj	 = &(xobject->d2_or_address)->d2_post_gen_deliv_addr;
					break;
				case MH_T_POSTAL_LOCALE	:
					str_obj	 = &(xobject->d2_or_address)->d2_post_locale;
					break;
				case MH_T_POSTAL_OFFICE_BOX_NUMBER :
					str_obj	 = &(xobject->d2_or_address)->d2_post_office_box_nbr;
					break;
				case MH_T_POSTAL_OFFICE_NAME :
					str_obj	 = &(xobject->d2_or_address)->d2_post_office_name;
					break;
				case MH_T_POSTAL_OFFICE_NUMBER :
					str_obj	 = &(xobject->d2_or_address)->d2_post_office_nbr;
					break;
				case MH_T_POSTAL_ORGANIZATION_NAME :
					str_obj	 = &(xobject->d2_or_address)->d2_post_org_name;
					break;
				case MH_T_POSTAL_PATRON_DETAILS	:
					str_obj	 = &(xobject->d2_or_address)->d2_post_patron_details;
					break;
				case MH_T_POSTAL_PATRON_NAME :
					str_obj	 = &(xobject->d2_or_address)->d2_post_patron_name;
					break;
				case MH_T_POSTAL_STREET_ADDRESS	:
					str_obj	 = &(xobject->d2_or_address)->d2_post_street_addr;
					break;
				case MH_T_SURNAME :
					str_obj	 = &(xobject->d2_or_address)->d2_surname;
					break;
				case MH_T_POSTAL_ADDRESS_IN_LINES :
					str_obj	 = &(xobject->d2_or_address)->d2_post_addr_in_lines;
					break;
				}
				if (!(*str_obj))
					break;
				if (!initialize) {
					GET_NB_OBJ(xds_str_list, d2_next,
						 pt_l->max_val, (xds_str_list *
						*)str_obj, adr_obj);
					UPDATE_LIMIT_VALUE(pt_l->max_val);
					JUMP_N_OBJ(xds_str_list, d2_next,
						 initial_value, str_obj, adr_obj,
						 n_cur);
					pt_l->desc = adr_obj;
					for	(i = 0;	i <		pt_l->nb_val; i++,
						 adr_obj = &(*adr_obj)->d2_next) {
						if ((!(*adr_obj)->d2_str.d2_size) ||
							(!(*adr_obj)->d2_str.d2_value))
							continue;
						nb_desc++;
					}
					break;
				}
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
					case D2_NUMERIC	:
						syntax = IAPL_TO_OM[(*adr_obj)->d2_rep];
						break;
					}
					UPDATE_DESC_STR(ptr_desc, pt_l->type,
						 syntax, (*adr_obj)->d2_str.d2_size,
						 (*adr_obj)->d2_str.d2_value,
						1);
				}
				break;
			}
			if ((initialize) &&		(ret != OM_SUCCESS))
				clean_before_exit(desc0, ptr_desc, nb_desc,
					 ret);
		}
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize)
			break;
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		}

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS)
			return(ret);

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	}

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;

end:
	return(ret);
} /* end dsP_get_OR_name */


/********************************************************/
/*							*/
/*	get	DL SUBMIT PERMISSIONS object				*/
/*							*/
/********************************************************/
OM_return_code
dsP_get_dlsp(
	OMX_workspace		workspace,
	OM_private_object	original,
	xds_dl_submit_perm	*xobject,
	OM_exclusions		exclusions,
	OM_type_list		included_types,
	OM_boolean		local_strings,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_public_object		*copy,
	OM_value_position		*total_number)
{
	OMP_type_list	types_list[] = DL_SP_TYPE_LIST;
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

		for	(pt_l = types_list;	pt_l->type != OM_NO_MORE_TYPES;
			pt_l++)	 {
			/* the type	doesn't	belong to included_types list	*/
			if (pt_l->max_val == 0)
				continue;

			switch (pt_l->type)	 {
			case OM_CLASS:
				INC_NBDESC_MONO;
				UPDATE_DESC_CLASS(dsP_DS_C_DL_SUBMIT_PERMS);
				break;
			case DS_PERM_TYPE :
				if (!xobject)
					break;
				INC_NBDESC_MONO;
				UPDATE_DESC_ENUM(ptr_desc, DS_PERM_TYPE,
					 xobject->d2_perm_type, 1);
				break;
			case DS_INDIVIDUAL :
				if ((!xobject) 
				||  (xobject->d2_perm_type != DS_PERM_INDIVIDUAL)	 
				||	(!xobject->d2_choice.d2_individual))
					break;
				INC_NBDESC_MONO;
				OBJECT_VALUE(OR_name, dsP_MH_C_OR_NAME,
				xobject->d2_choice.d2_individual, DS_INDIVIDUAL, 1)
				break;
			case DS_MEMBER_OF_DL :
				if ((!xobject) 
				||  (xobject->d2_perm_type != DS_PERM_MEMBER_OF_DL) 
				||  (!xobject->d2_choice.d2_member_of_dl))
					break;
				INC_NBDESC_MONO;
				OBJECT_VALUE(OR_name, dsP_MH_C_OR_NAME,
				xobject->d2_choice.d2_member_of_dl, DS_MEMBER_OF_DL, 1)
				break;
			case DS_PATTERN_MATCH :
				if ((!xobject) 
				||  (xobject->d2_perm_type != DS_PERM_PATTERN_MATCH) 
				||  (!xobject->d2_choice.d2_pattern_match))
					break;
				INC_NBDESC_MONO;
				OBJECT_VALUE(OR_name, dsP_MH_C_OR_NAME,
				xobject->d2_choice.d2_pattern_match, DS_PATTERN_MATCH, 1)
				break;
			case DS_MEMBER_OF_GROUP	:
				if ((!xobject) 
				||  (xobject->d2_perm_type != DS_PERM_MEMBER_OF_GROUP) 
				||		 (!xobject->d2_choice.d2_member_of_group))
					break;
				INC_NBDESC_MONO;
				OBJECT_VALUE(dn, dsP_DS_C_DS_DN,
				xobject->d2_choice.d2_member_of_group, DS_MEMBER_OF_GROUP, 1)
				break;
			}
			if ((initialize) &&	(ret != OM_SUCCESS)) {
				clean_before_exit(desc0, ptr_desc, nb_desc, ret);
			} /* endif */
		}
		/* if exclude_descriptors return the number	of descriptors	*/
		if (initialize)
			break;
		if (OMP_EXCLUDE_DESCRIPTORS) {
			*total_number = nb_desc;
			return(OM_SUCCESS);
		}

		/* allocate	the	descriptors					*/
		if ((ret = dsP_alloc_descriptor(workspace, nb_desc, &desc))	!=
			OM_SUCCESS)
			return(ret);

		ptr_desc = desc0 = desc	 + 1;
		initialize = OM_TRUE;
	}

	omP_update_descriptor(original, desc0);
	*copy = desc0;
	*total_number = nb_desc;

end:
	return(ret);
} /* end dsP_get_dlsp */
