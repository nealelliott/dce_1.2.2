/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsPPut.c,v $
 * Revision 1.1.44.2  1996/02/18  18:24:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:47:29  marty]
 *
 * Revision 1.1.44.1  1995/12/08  15:32:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:55:39  root]
 * 
 * Revision 1.1.42.8  1994/09/19  08:50:09  marrek
 * 	Fixes for OT 12198.
 * 	[1994/09/19  08:49:34  marrek]
 * 
 * Revision 1.1.42.7  1994/08/24  08:35:41  marrek
 * 	Fix warnings. (OT 11824)
 * 	[1994/08/23  14:37:22  marrek]
 * 
 * Revision 1.1.42.6  1994/08/10  08:25:43  marrek
 * 	Fix for SAP errors, OT11467.
 * 	[1994/08/08  14:15:56  marrek]
 * 
 * Revision 1.1.42.5  1994/07/06  15:09:07  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:24:58  marrek]
 * 
 * Revision 1.1.42.4  1994/06/21  14:48:49  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:48:42  marrek]
 * 
 * Revision 1.1.42.3  1994/05/10  16:03:58  marrek
 * 	Bug fixes and I18N for April 1994 submission.
 * 	[1994/05/06  09:59:38  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  13:15:16  marrek]
 * 
 * Revision 1.1.42.2  1994/03/23  15:48:44  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:36:48  keutel]
 * 
 * Revision 1.1.42.1  1994/02/22  18:10:17  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:21:31  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:00:43  marrek]
 * 
 * Revision 1.1.40.2  1993/10/22  12:29:28  keutel
 * 	CR 9102
 * 	[1993/10/22  12:28:31  keutel]
 * 
 * Revision 1.1.40.1  1993/10/14  19:19:29  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:17:40  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  09:45:33  keutel]
 * 
 * Revision 1.1.2.2  1993/08/10  07:05:55  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:16:04  marrek]
 * 
 * Revision 1.1.6.2  1993/07/22  07:19:31  marrek
 * 	Add test for OM_LENGTH_UNSPECIFIED in update_dist_name().
 * 	[1993/05/28  10:35:40  marrek]
 * 
 * Revision 1.1.4.4  1992/12/30  21:16:59  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:05:22  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/04  17:39:15  marrek
 * 	November 1992 code drop
 * 	Replace cmx.h by cmx_addr.h
 * 	Replace T_NO_NSAP by T_MAXNSAP
 * 	[1992/12/04  12:26:27  marrek]
 * 
 * Revision 1.1.4.2  1992/11/30  08:35:23  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:43:40  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  21:00:57  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:42:18  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  23:19:08  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:46:23  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: dsPPut.c,v $ $Revision: 1.1.44.2 $ $Date: 1996/02/18 18:24:54 $";
#endif

/***************************************************************
*															   *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG	1991   *
*				 ALL RIGHTS RESERVED						   *
*															   *
***************************************************************/

/**********************************************************************/
/*				 COPYRIGHT (C) BULL S.A. 1990 1991 1992				  */
/*						  All Rights Reserved						  */
/**********************************************************************/
/*																	  */
/*	 * This software is furnished under licence and may be used only  */
/*	 * in accordance with the terms of that licence and with the	  */
/*	 * inclusion of the above copyright notice.						  */
/*	 * This software may not be provided or otherwise made available  */
/*	 * to, or used by, any other person. No title to or ownership of  */
/*	 * the software is hereby transferred.							  */
/*																	  */
/**********************************************************************/
/*	@(#)dsPPut.c 1.49 (Bull S.A) 6/26/92 */

/****************************************************************/
/*								*/
/*	Function :	dsP_put_switch				*/
/*								*/
/****************************************************************/
/*								*/
/*	dsP_put_switch() called by om_put()	function				*/
/*			 calls for each XDS object destination		*/
/*			 a specific put_xxx() function				*/
/*								*/
/* Each function checks the attribute types validity		*/
/*		and the associated syntax values				*/
/*								*/
/*		If the source object is private the om_get() function	*/
/*		is called with the included_types list as argument to	*/
/*		obtain a public object. So, the source code can work	*/
/*		on only source public objects.							*/
/*								*/
/*	The om_put specification is not very clear :			*/
/*	- we have a list of types of attribute and				*/
/*	- only one kind of modification with the associated		*/
/*	  initial and limiting values							*/
/*															*/
/*	=> the implementation needed a choice:					*/
/*	- for a single-valued attribute type, the only			*/
/*	  allowed modifications are	REPLACE_ALL or				*/
/*	  REPLACE_CERTAIN_VALUES with:							*/
/*		   initial_value = 0								*/
/*		   limiting_value = 1								*/
/*	  the attributes of	these types	will be	analysed first	*/
/*	- for each multi-valued	attribute type,					*/
/*	  the modification will	be applied separately			*/
/*	  i.e. an INSERT_AT_CERTAIN_POINT with:					*/
/*		   initial_value = 3 (for example)					*/
/*		   the attribute of	type type1 has 5 values			*/
/*		   the attribute of	type type2 has 2 values			*/
/*	  => in	type1 the values will be inserted at position 3	*/
/*		 in	type2 the values will be inserted at position 2	*/
/*															*/
/*	When an	included_types list	is present,					*/
/*	the	only types of attributes which will	be get from		*/
/*	the	source will	be the ones	that appear	in the list		*/
/*	and	which can be allowed in	the	destination.			*/
/*	The	other ones will	be ignored.							*/
/*															*/
/*	When no	included_types list	is present,					*/
/*	the	only types of attributes which will	be get from		*/
/*	the	source will	be the ones	which can be allowed in		*/
/*	the	destination. The other ones	will be	ignored			*/
/*															*/
/*	Each function or subroutine	which needs	memory			*/
/*	allocation may return memory-insufficient code			*/
/*								*/
/*	RETURN_CODE:						*/
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
#include <xdssap.h>
#include <xdsdef.h>
#include <xomdef.h>
#include <sapdef.h>
#include <xmhdef.h>
#include <xdsalloc.h>
#include <xdsimport.h>
#include <xdspackage.h>
#include <dsPDelete.h>
#include <dsPPsap.h>
#include <dsPAcl.h> /* for *_acl(), *_aci(), dsP_get_acl_length() */
#include <dsPPut.h>
#include <dsPMdup.h> /* for get_OR_name() ... */
#include <d27conv.h> /* for d27_local_to_t61(), d27_t61_to_local()... */

/***************************************************************/
/*		external function prototypes							*/
/***************************************************************/
OM_return_code
dsP_del_certificate_sublist(
	OMX_workspace workspace,
	xds_certificate_sublist *xobject);

OM_return_code
dsP_del_algorithm_ident(
	OMX_workspace				workspace,
	xds_algorithm_ident			*xobject);

/***************************************************************/
/*		static function prototypes							*/
/***************************************************************/

static OM_boolean
type_in_list(
	OM_type		type,
	OM_type_list	list
);
#if 0 /* seems to be unused: if confirmed, remove it. CC 2.2.94 */
static void
priv_cp_type(
	D2_a_type	**new_val,
	D2_a_type	*old_val,
	OM_sint16	nb_val
);
static void
priv_cp_val(
	D2_a_value	**new_val,
	D2_a_value	*old_val,
	OM_sint16	nb_val
);
#endif /* 0 - seems to be unused */

static OM_return_code
update_array (
	OMX_workspace	workspace,
	short *nb_val_d,
	void **val_d_p,
	OM_modification	modification,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OMP_type_list	*pt_l,
	OM_sint	type);

static OM_return_code
update_dist_name (
	xds_dist_name	dn_ptr,
	OM_sint		len_src,
	void 		*str_src,
	char 		separator,
	OM_boolean	convert,
	OM_boolean	next_ava);

static OM_return_code
pub_cp_crit(
	OMX_workspace	workspace,
	D2_criteria		**a_val,
	OM_modification	modification,
	OM_descriptor	*desc,
	OM_sint16		nb_val);


static OM_return_code
pub_cp_str(
	OMX_workspace	workspace,
	D2_str			**a_val,
	OM_modification	modification,
	OM_descriptor	*desc,
	OM_sint16		nb_val);


static OM_return_code
pub_cp_type(
	OMX_workspace	workspace,
	D2_a_type		**a_val,
	OM_modification	modification,
	OM_descriptor	*desc,
	OM_sint16		nb_val);


static OM_return_code
pub_cp_val(
	OMX_workspace	workspace,
	D2_a_value		**a_val,
	OM_modification	modification,
	OM_descriptor	*desc,
	OM_sint16		nb_val);


static OM_return_code
dsP_put_attribute (
	OMX_workspace	workspace,
	void 			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_attr_list(
	OMX_workspace	workspace,
	void 			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_ava	(
	OMX_workspace	workspace,
	void 			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_context(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_rdn1 (
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	xds_dist_name		dn_ptr);

static OM_return_code
dsP_put_rdn	(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_entry_info(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_entry_info_sel(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_entry_mod(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_entry_mod_list(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_extension(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_filter_item(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_filter(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_session(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_faxnumber(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_postal_address(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_search_criterion(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_search_guide(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_ttxid(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_telex_number(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_aci	(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_acl	(
	OMX_workspace	workspace,
	void 			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_algorithm_ident(
	OMX_workspace		workspace,
	void				**xds_object,
	OM_object			source,
	OMP_object_type		object_type,
	OM_modification		modification,
	OM_type_list		included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_certificate(
	OMX_workspace		workspace,
	void 				**xds_object,
	OM_object			source,
	OMP_object_type		object_type,
	OM_modification		modification,
	OM_type_list		included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_certificate_list(
	OMX_workspace		workspace,
	void 				**xds_object,
	OM_object			source,
	OMP_object_type		object_type,
	OM_modification		modification,
	OM_type_list		included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_certificate_pair(
	OMX_workspace		workspace,
	void 				**xds_object,
	OM_object			source,
	OMP_object_type		object_type,
	OM_modification		modification,
	OM_type_list		included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

static OM_return_code
dsP_put_certificate_sublist(
	OMX_workspace		workspace,
	void 				**xds_object,
	OM_object			source,
	OMP_object_type		object_type,
	OM_modification		modification,
	OM_type_list		included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value);

/* define different	kind of	array type allowed in update_array() function */
enum array_type	{VAL_ARRAY, TYP_ARRAY, STR_ARRAY, CRIT_ARRAY};


/* define macros to	call the specific XDS put function according the
 * object type
 */
#define	CALL_PUT(class)							\
	dsP_put_##class(workspace,xds_object,source,				\
			object_type,modification,			\
			included_types,initial_value,limiting_value)

static OMP_object_type dummy_object = {0,0};
#define	PUT_VAL(class,VAL,REP)						\
{									\
	dest = (void *)&((*a_val)->d2_at_value.VAL);			\
	if ((ret = dsP_put_##class(workspace, dest, source,			\
			dummy_object, OM_REPLACE_ALL, 0, 0, OM_ALL_VALUES))	\
							!= OM_SUCCESS)	\
		break;							\
	(*a_val)->d2_a_rep = REP;					\
}

#define	PUT_VAL_NULL_LENGTH(class,VAL,REP)				\
{									\
	dest = (void *)&((*a_val)->d2_at_value.VAL);			\
	if ((ret = dsP_put_##class(workspace,	dest, source,			\
			dummy_object, OM_REPLACE_ALL, 0, 0, OM_ALL_VALUES))	\
							!= OM_SUCCESS)	\
		break;							\
	(*a_val)->d2_a_v_len = 0;					\
	(*a_val)->d2_a_rep = REP;					\
}

#define	CHECK_UPDATE_VALUE_ARRAY(NVAL,VALPT)					\
{									\
	if (check)							\
		break;							\
	UPDATE_VALUE_ARRAY(NVAL,VALPT);					\
}


/* define macros to	call update_array function according to	the	kind of
 * array to	update
 */
#define	UPDATE_VALUE_ARRAY(NVAL,VALPT)					\
{									\
	if (OMP_REMOVE)							\
		pt_l->nb_val = 0;					\
	ret = update_array(workspace,NVAL, VALPT,			\
			modification,					\
			initial_value,limiting_value,pt_l,		\
			VAL_ARRAY);					\
}


#define	UPDATE_TYPE(ATTR)						\
{									\
	if (OMP_REMOVE)							\
		pt_l->nb_val = 0;					\
	ret = update_array(workspace,					\
			&(ATTR->d2_r_no_at),				\
			(void *)&(ATTR->d2_r_type_at),			\
			modification,					\
			initial_value,limiting_value,pt_l,		\
			TYP_ARRAY);					\
}

#define	UPDATE_STR(NVAL,VALPT)						\
{									\
	if (OMP_REMOVE)							\
		pt_l->nb_val = 0;					\
	ret = update_array(workspace,NVAL,VALPT,			\
			modification,					\
			initial_value,limiting_value,pt_l,		\
			STR_ARRAY);					\
}

#define	UPDATE_CRIT(NVAL,VALPT)						\
{									\
	if (OMP_REMOVE)							\
		pt_l->nb_val = 0;					\
	ret = update_array(workspace,NVAL, VALPT,			\
			modification,					\
			initial_value,limiting_value,pt_l,		\
			CRIT_ARRAY);					\
}

#define	PUB_CP_STR_SRC(SRC,VALPT,NBVAL)					\
{									\
	if ((OMP_REMOVE) &&	(NBVAL))					\
	{								\
		dsP_delete_str(workspace,VALPT);				\
		memset(VALPT, 0, sizeof(D2_str));			\
		ret = OM_SUCCESS;					\
		goto end;					\
	}								\
	ret = pub_cp_str (workspace, &(VALPT), modification,		\
					SRC,NBVAL);		\
}



/* macros called in	update_array function to copy an array of a	specific
 * type
 */
#define	PUB_CP_VAL(VALPT,NBVAL)						\
{									\
	if ((OMP_REMOVE) &&	(NBVAL))					\
	{								\
		dsP_delete_a_value(workspace,VALPT);			\
		memset(VALPT, 0, sizeof(D2_a_value));			\
		ret = OM_SUCCESS;					\
		goto end;							\
	}								\
	ret = pub_cp_val (workspace,&(VALPT), modification,		\
			 (OM_descriptor	*)pt_l->desc,NBVAL);		\
}


#define	PUB_CP_TYPE(VALPT,NBVAL)					\
{									\
	if ((OMP_REMOVE) &&	(NBVAL))					\
	{								\
		dsP_delete_type(workspace,VALPT);				\
		memset(VALPT, 0, sizeof(D2_a_type));			\
		ret =  OM_SUCCESS;					\
		goto end;							\
	}								\
	ret = pub_cp_type (workspace,					\
			 &(VALPT), modification,			\
			 (OM_descriptor	*)pt_l->desc,NBVAL);		\
}


#define	PUB_CP_STR(VALPT,NBVAL)						\
{									\
	if ((OMP_REMOVE) &&	(NBVAL))					\
	{								\
		dsP_delete_str(workspace,VALPT);				\
		memset(VALPT, 0, sizeof(D2_str));			\
		ret = OM_SUCCESS;					\
		goto end;							\
	}								\
	ret = pub_cp_str (workspace,					\
			 &(VALPT), modification,			\
			 (OM_descriptor	*)pt_l->desc,NBVAL);		\
}


#define	PUB_CP_CRIT(VALPT,NBVAL)					\
{									\
	if ((OMP_REMOVE) &&	(NBVAL))					\
	{								\
		dsP_delete_criteria(workspace,VALPT);			\
		memset(VALPT, 0, sizeof(D2_criteria));			\
		ret = OM_SUCCESS;					\
		goto end;							\
	}								\
	ret = pub_cp_crit (workspace,					\
			 &(VALPT), modification,			\
			 (OM_descriptor	*)pt_l->desc,NBVAL);		\
}

#define	FREE_ARRAY							\
{									\
	switch(type){							\
	case VAL_ARRAY:							\
		OMP_XFREE (workspace,old_val_p,	AL_VALUE, *nb_val_d);	\
		break;							\
	case TYP_ARRAY:							\
		OMP_XFREE (workspace,old_val_p,	AL_TYPE, *nb_val_d);	\
		break;							\
	case STR_ARRAY:							\
		OMP_XFREE (workspace,old_val_p,	AL_STR,	*nb_val_d);		\
		break;							\
	case CRIT_ARRAY:						\
		OMP_XFREE (workspace,					\
			old_val_p, AL_XDS_SEARCH_CRITERION,	*nb_val_d);	\
		break;							\
	}									\
}

#define	ARRAY_VAR							\
	D2_a_value	*val_p;							\
	D2_a_type	*typ_p;							\
	D2_str	*str_p;							\
	D2_criteria	*crit_p;

#define	COPY_PUB_ELEMENTS						\
{									\
	switch(type){							\
	case VAL_ARRAY:							\
		val_p = (D2_a_value	*)new_val_p;				\
		PUB_CP_VAL(val_p, pt_l->nb_val);				\
		new_val_p = (void *)val_p;					\
		break;							\
	case TYP_ARRAY:							\
		typ_p = (D2_a_type *)new_val_p;				\
		PUB_CP_TYPE(typ_p, pt_l->nb_val);				\
		new_val_p = (void *)typ_p;					\
		break;							\
	case STR_ARRAY:							\
		str_p = (D2_str	*)new_val_p;				\
		PUB_CP_STR(str_p, pt_l->nb_val);				\
		new_val_p = (void *)str_p;					\
		break;							\
	case CRIT_ARRAY:						\
		crit_p = (D2_criteria *)new_val_p;				\
		PUB_CP_CRIT(crit_p,	pt_l->nb_val);				\
		new_val_p = (void *)crit_p;					\
		break;							\
	}									\
}


/* macro to	copy elements from old to new allocated	array.
 * pointers, if	any, are copied	too, but not the objects
 * which are pointed to
 */
#define	PRI_CP_ELEM(DEST,SRC,NB,D2TYP)					\
{														\
	memcpy (DEST, SRC, NB *	sizeof(D2TYP));				\
	ptr = (char	*)DEST + NB	* sizeof(D2TYP);			\
	DEST = (void *)ptr;									\
}


#define	COPY_PRI_ELEMENTS(DPT,INIT,NB)					\
{									\
	switch(type){							\
	case VAL_ARRAY:							\
		PRI_CP_ELEM(DPT,						\
			(char *)old_val_p +	(INIT)*sizeof(D2_a_value),	\
			(NB),D2_a_value);				\
		break;							\
	case TYP_ARRAY:							\
		PRI_CP_ELEM(DPT,						\
			(char *)old_val_p +	(INIT)*sizeof(D2_a_type),	\
			(NB),D2_a_type);				\
		break;							\
	case STR_ARRAY:							\
		PRI_CP_ELEM(DPT,						\
			(char *)old_val_p +	(INIT)*sizeof(D2_str),	\
			(NB),D2_str);					\
		break;							\
	case CRIT_ARRAY:						\
		PRI_CP_ELEM(DPT,						\
			(char *)old_val_p +	(INIT)*sizeof(D2_criteria),	\
			(NB),D2_criteria);				\
		break;							\
	}									\
}


/* macro to	delete elements	from allocated array
 * and to clear	the	structures.
 */
#define	DELETE_AND_CLEAR(D2TYPE,FUNCT)					\
{									\
	for	(i = 0;	i <	limiting_value - initial_value;	i++)		\
	{									\
	FUNCT (workspace,&(((D2TYPE	*)new_val_p)[initial_value + i]));	\
	memset(&(((D2TYPE *)new_val_p)[initial_value + i]),			\
						0, sizeof(D2TYPE));	\
	}									\
	new_val_p = (void *)&(((D2TYPE *)new_val_p)[initial_value]);	\
}

#define	CLEAR_ELEMENTS							\
{									\
	switch(type){							\
	case VAL_ARRAY:							\
		DELETE_AND_CLEAR(D2_a_value, dsP_delete_a_value);		\
		break;							\
	case TYP_ARRAY:							\
		DELETE_AND_CLEAR(D2_a_type,	dsP_delete_type);			\
		break;							\
	case STR_ARRAY:							\
		DELETE_AND_CLEAR(D2_str, dsP_delete_str);			\
		break;							\
	case CRIT_ARRAY:						\
		DELETE_AND_CLEAR(D2_criteria, dsP_delete_criteria);		\
		break;							\
	}									\
}

/* macro to	allocate an	array of elements.
 */
#define	ALLOC_AND_CLEAR(D2TYPE,ALTYPE)					\
{									\
	if ((new_val_p = *val_d_p = (void *)				\
			OMP_XALLOC(workspace,ALTYPE,new_nb_val)) == 0) {	\
		ret = OM_MEMORY_INSUFFICIENT;			\
		goto end;								\
	} /* endif */								\
	/* clear all the values	*/					\
	memset (new_val_p, 0, new_nb_val * sizeof(D2TYPE));		\
}

#define	ALLOC_ARRAY							\
{									\
	switch(type){							\
	case VAL_ARRAY:							\
		ALLOC_AND_CLEAR(D2_a_value,	AL_VALUE);			\
		break;							\
	case TYP_ARRAY:							\
		ALLOC_AND_CLEAR(D2_a_type, AL_TYPE);			\
		break;							\
	case STR_ARRAY:							\
		ALLOC_AND_CLEAR(D2_str,	AL_STR);				\
		break;							\
	case CRIT_ARRAY:						\
		ALLOC_AND_CLEAR(D2_criteria, AL_XDS_SEARCH_CRITERION);	\
		break;							\
	}									\
}

/* macro to	delete and to free an array	of elements.
 */
#define	DELETE_ARRAY							\
{									\
	switch(type){							\
	case VAL_ARRAY:							\
		dsP_delete_values(workspace,(D2_a_value	*)old_val_p, *nb_val_d);\
		break;							\
	case TYP_ARRAY:							\
		for	(i = 0;	i <	*nb_val_d; i++)				\
		dsP_delete_type	(workspace,&(((D2_a_type *)old_val_p)[i]));	\
		OMP_XFREE (workspace,old_val_p,	AL_TYPE, *nb_val_d);	\
		break;							\
	case STR_ARRAY:							\
		for	(i = 0;	i <	*nb_val_d; i++)				\
		dsP_delete_str (workspace,&(((D2_str *)old_val_p)[i]));	\
		OMP_XFREE (workspace,old_val_p,	AL_STR,	*nb_val_d);		\
		break;							\
	case CRIT_ARRAY:						\
		for	(i = 0;	i <	*nb_val_d; i++)				\
		dsP_delete_criteria	(workspace,				\
				&(((D2_criteria	*)old_val_p)[i]));	\
		OMP_XFREE (workspace,					\
			old_val_p, AL_XDS_SEARCH_CRITERION,	*nb_val_d);	\
		break;							\
	}									\
}
/* macro to	convert	OM syntax to IAPL representation */
#define	SYNTAX_TO_REP(SYNT)						\
{									\
	(*a_val)->d2_a_rep = 				\
		(SYNTAX_ARRAY_ADDR[SYNT/10])[SYNT -	(SYNT/10)*10];	\
}

/* convert OM syntax to	IAPL representation	*/
OM_sint
OM_TO_IAPL[] = {
	0,			/* OM_S_NO_MORE_SYNTAX */
	D2_BOOLEAN,				/* OM_S_BOOLEAN	*/
	D2_INTEGER,				/* OM_S_INTEGER	*/
	0,			/* OM_S_BIT_STRING */
	D2_OCTET,		/* OM_S_OCTET_STRING */
	0,			/* OM_S_NULL */
	D2_OBJ_IDENT,		/* OM_S_OBJECT_IDENTIFIER_STRING */
	0,			/* OM_S_OBJECT_DESCRIPTOR_STRING */
	D2_ASN1,		/* OM_S_ENCODING_STRING	*/
	0,			/* ****	 FREE  ****	*/
	D2_ENUM,		/* OM_S_ENUMERATION	*/
	0,			/* ****	 FREE  ****	*/
	0,			/* ****	 FREE  ****	*/
	0,			/* ****	 FREE  ****	*/
	0,			/* ****	 FREE  ****	*/
	0,			/* ****	 FREE  ****	*/
	0,			/* ****	 FREE  ****	*/
	0,			/* ****	 FREE  ****	*/
	D2_NUMERIC,				/* OM_S_NUMERIC_STRING */
	D2_PRINTABLE,		/* OM_S_PRINTABLE_STRING */
	D2_T61,					/* OM_S_TELETEX_STRING */
	0,			/* OM_S_VIDEOTEX_STRING	*/
	0,			/* OM_S_IA5_STRING */
	D2_TIME,		/* OM_S_UTC_TIME_STRING	*/
	0,			/* OM_S_GENERALISED_TIME_STRING	*/
	0,			/* OM_S_GRAPHIC_STRING */
	0,			/* OM_S_VISIBLE_STRING */
	0,			/* OM_S_GENERAL_STRING */
	0,			/* ****	 FREE  ****	*/
	0			/* ****	 FREE  ****	*/
};


/* OM_S_OBJECT is not converted	*/

OM_sint
OMX_TO_IAPL[] = {
	D2_INT_LIST,		/* DSX_S_INTEGER_LIST */
	0,			/* DSX_S_RESERVED */
	0,
	0,
	0,
	0
};


/* array used by SYNTAX_TO_REP macro					   */
/* to convert OM syntax	to IAPL	representation			   */

OM_sint	*
SYNTAX_ARRAY_ADDR[]	 = {
	OM_TO_IAPL,				/* OM syntax from 0	to 9 */
	&OM_TO_IAPL[10],	/* OM syntax from 10 to	19 */
	&OM_TO_IAPL[20],	/* OM syntax from 20 to	29 */
	0,
	0,
	OMX_TO_IAPL		/* OM syntax from 50 to	59 */
};


OMP_enum_limit	filter_item_type_limit = FILTER_ITEM_TYPE_LIMITS;
OMP_enum_limit	filter_type_limit = FILTER_TYPE_LIMITS;
OMP_enum_limit	info_type_limit	 = INFO_TYPE_LIMITS;
OMP_enum_limit	mod_type_limit = MOD_TYPE_LIMITS;
OMP_enum_limit	nr_phase_type_limit	 = NAME_RESOLUTION_PHASE_TYPE_LIMITS;
OMP_enum_limit	priority_type_limit	 = PRIORITY_TYPE_LIMITS;
OMP_enum_limit	scope_ref_type_limit = SCOPE_OF_REFERRAL_TYPE_LIMITS;
OMP_enum_limit	inter_type_limit = INTERPRETATION_TYPE_LIMITS;
OMP_enum_limit	auth_mech_type_limit = AUTH_MECH_TYPE_LIMITS;
OMP_enum_limit	prot_req_type_limit = PROT_REQ_TYPE_LIMITS;

OMP_enum_list	filter_item_type_list[]	 = FILTER_ITEM_TYPE_LIST;
OMP_enum_list	filter_type_list[] = FILTER_TYPE_LIST;
OMP_enum_list	info_type_list[] = INFO_TYPE_LIST;
OMP_enum_list	mod_type_list[]	 = MOD_TYPE_LIST;
OMP_enum_list	nr_phase_type_list[] = NAME_RESOLUTION_PHASE_TYPE_LIST;
OMP_enum_list	priority_type_list[] = PRIORITY_TYPE_LIST;
OMP_enum_list	scope_ref_type_list[] = SCOPE_OF_REFERRAL_TYPE_LIST;
OMP_enum_list	inter_type_list[] = INTERPRETATION_TYPE_LIST;
OMP_enum_list	auth_mech_type_list[] = AUTH_MECH_TYPE_LIST;
OMP_enum_list	prot_req_type_list[] = PROT_REQ_TYPE_LIST;

/************************************************************************/
/*																		*/
/* dsP_test_source_priv	function											*/
/*																		*/
/*	 If	the	source object is private, this function	calls				*/
/*	 om_get	function with exclude subobject	according to the last		*/
/*	 parameter,	and	with an	included_types list	to get the only	types	*/
/*	 wished	for.														*/
/*																		*/
/* INPUT :																*/
/*	 source	: source object	pointer										*/
/*	 included_types	: list of types	of attributes to be	got				*/
/*	 src_pub : pointer to receive the handle to	the	public object		*/
/*	 all : EXCLUDE_SUB_OBJECTS (OM_FALSE) to exclude the sub_objects	*/
/*		   NO_EXCLUDE_SUB_OBJECTS (OM_TRUE)	to get the sub_objects too	*/
/*																		*/
/* OUTPUT :																*/
/*	 *src_pub :	handle to the public object	if flag == OM_TRUE			*/
/*	 flag :	OM_TRUE	if the source object is	private						*/
/*																		*/
/* RETURN_CODE ERROR:													*/
/*	 om_get() function return_code errors.								*/
/*																		*/
/************************************************************************/

OM_return_code
dsP_test_source_priv(
	OM_object		source,
	OM_type_list	included_types,
	OM_public_object	*src_pub,
	OM_value_position	*desc_nb,
	OM_boolean		*flag,
	OM_boolean		all
)
{
	OM_exclusions	exclusions;
	OM_return_code	ret = OM_SUCCESS;

	*flag = OM_FALSE;
	*src_pub = (OM_public_object )0;
	if (OMP_PRIVATE(source)) {
		/* check valid handle */
		if (!OMP_VALID_OBJECT(source)) {
			ret = OM_NO_SUCH_OBJECT;
			goto end;
		} /* endif */

		*flag = OM_TRUE;
		if (all) {
			exclusions = OM_NO_EXCLUSIONS;
		} else {
			exclusions = OM_EXCLUDE_SUBOBJECTS;
		} /* endif */
		if (included_types != OM_NO_MORE_TYPES) {
			exclusions |= OM_EXCLUDE_ALL_BUT_THESE_TYPES;
		} /* endif */
		ret = om_get(source, exclusions, included_types, 0,	0,
			 0,	src_pub, desc_nb);
		goto end;
	} /* endif */
end:
	return(ret);
} /* end dsP_test_source_priv */


/********************************************************/
/*							*/
/*	dsP_put_switch()					*/
/*							*/
/********************************************************/
OM_return_code
dsP_put_switch (
	const OMP_object_type	object_type,
	OM_private_object		destination,
	const OM_modification	modification,
	const OM_object			source,
	const OM_type_list		included_types,
	const OM_value_position	initial_value,
	const OM_value_position	limiting_value
)
{
	OMP_private_object	 * handle_ptr;
	void	*xds_object;
	OM_return_code	ret = OM_SUCCESS;
	OMX_workspace	workspace;

	handle_ptr = (OMP_private_object * ) (OMP_INTERNAL (destination));
	xds_object = &OMP_DATA(destination);
	workspace = OMP_WORKSPACE(destination);

	/* at this point, initial_value	and	limiting_value are significant
	 *
	 * initial_value == limiting_value
	 * => insert:
	 *		at begining = 0
	 *		at end = OM_ALL_VALUES
	 *		at certain point = n
	 * initial_value < limiting_value
	 * => replace:
	 *		all	: init = 0
	 *			  limit = OM_ALL_VALUES
	 *		certain	values : init = i
	 *				 limit = l or OM_ALL_VALUES
	 */


	/* If destination and source objects are instances	   */
	/* of the same class,and the modification is OM_REPLACE_ALL*/
	/* delete the xds destination object and copy the source   */
	switch (OMP_PACKAGE(object_type)) {
	case XDS_SERVICE_PACKAGE:

		switch (OMP_CLASS(object_type))	 {
		case XDS_ATTRIBUTE:
			ret	 = CALL_PUT(attribute);
			break;
		case XDS_ATTRIBUTE_LIST:
			ret	 = CALL_PUT(attr_list);
			break;
		case XDS_AVA:
			ret	 = CALL_PUT(ava);
			break;
		case XDS_CONTEXT:
			ret	 = CALL_PUT(context);
			break;
		case XDS_DS_DN:
			ret	 = CALL_PUT(dist_name);
			break;
		case XDS_DS_RDN:
			ret	 = CALL_PUT(rdn);
			break;
		case XDS_EXT:
			ret	 = CALL_PUT(extension);
			break;
		case XDS_ENTRY_INFO:
			ret	 = CALL_PUT(entry_info);
			break;
		case XDS_ENTRY_INFO_SELECTION:
			ret	 = CALL_PUT(entry_info_sel);
			break;
		case XDS_ENTRY_MOD:
			ret	 = CALL_PUT(entry_mod);
			break;
		case XDS_ENTRY_MOD_LIST:
			ret	 = CALL_PUT(entry_mod_list);
			break;
		case XDS_FILTER:
			ret	 = CALL_PUT(filter);
			break;
		case XDS_FILTER_ITEM:
			ret	 = CALL_PUT(filter_item);
			break;
		case XDS_PRESENTATION_ADDRESS:
			ret	 = CALL_PUT(psap);
			break;
		case XDS_SESSION:
			ret	 = CALL_PUT(session);
			break;

		case XDS_ADDRESS :
		case XDS_COMMON_RESULTS:
		case XDS_ERROR:
		case XDS_NAME :
		case XDS_RELATIVE_NAME :
			ret = OM_NOT_CONCRETE;
			goto end;

		case XDS_ABANDON_FAILED	:
		case XDS_ACCESS_POINT :
		case XDS_ATTRIBUTE_PROBLEM :
		case XDS_COMMUNICATIONS_ERROR:
		case XDS_COMPARE_RESULT:
		case XDS_CONTINUATION_REF:
		case XDS_LIST_INFO:
		case XDS_LIST_INFO_ITEM:
		case XDS_LIST_RESULT:
		case XDS_OPERATION_PROGRESS:
		case XDS_PARTIAL_OUTCOME_QUAL:
		case XDS_READ_RESULT:
		case XDS_REFERRAL :
		case XDS_SEARCH_INFO:
		case XDS_SEARCH_RESULT:
		case XDS_ATTRIBUTE_ERROR:
		case XDS_LIBRARY_ERROR:
		case XDS_NAME_ERROR:
		case XDS_SECURITY_ERROR:
		case XDS_SERVICE_ERROR:
		case XDS_SYSTEM_ERROR:
		case XDS_UPDATE_ERROR:
			ret = OM_FUNCTION_DECLINED;
			goto end;

		default:
			ret = OM_NO_SUCH_CLASS;
			goto end;
		} /* endswitch */
		break;

	case XDS_GDS_PACKAGE:

		switch (OMP_CLASS(object_type))	 {
		case XDS_GDS_CONTEXT:
			ret	 = CALL_PUT(context);
			break;
		case XDS_GDS_SESSION:
			ret	 = CALL_PUT(session);
			break;
		case XDS_GDS_ACL:
			ret	 = CALL_PUT(acl);
			break;

		case XDS_GDS_ACL_ITEM:
			ret	 = CALL_PUT(aci);
			break;

		default:
			ret	 = (OM_NO_SUCH_CLASS);
			break;
		} /* endswitch */

		break;

	case XDS_BDCP:
		switch (OMP_CLASS(object_type))	 {
		case XDS_FACSIMILE_PHONE_NBR:
			ret	 = CALL_PUT(faxnumber);
			break;
		case XDS_POSTAL_ADDRESS:
			ret	 = CALL_PUT(postal_address);
			break;
		case XDS_SEARCH_CRITERION:
			ret	 = CALL_PUT(search_criterion);
			break;
		case XDS_SEARCH_GUIDE:
			ret	 = CALL_PUT(search_guide);
			break;
		case XDS_TELETEX_TERM_IDENT:
			ret	 = CALL_PUT(ttxid);
			break;
		case XDS_TELEX_NBR:
			ret	 = CALL_PUT(telex_number);
			break;
		default:
			ret	 = (OM_NO_SUCH_CLASS);
			break;
		} /* endswitch */

		break;

		CASE_PUT_MDUP_PACKAGE 
#if 0
		case XDS_MDUP:
		switch (OMP_CLASS(object_type))	{
			case MHS_DLSP:
				ret = CALL_PUT(dlsp);
				break;
			default:
				ret = (OM_NO_SUCH_CLASS);
				break;
		} /* endswitch */
		break;
		case XMH_PACKAGE:
		switch (OMP_CLASS(object_type))	{
			case XMH_OR_NAME:
				ret = CALL_PUT(OR_name);
			break;
			case XMH_OR_ADDRESS:
			ret = (OM_NO_SUCH_CLASS);
			break;
			default:
			ret = (OM_NO_SUCH_CLASS);
			break;
		} /* endswitch */
		break;
#endif /* 0 */
    case XDS_SAP_PACKAGE:
        switch (OMP_CLASS(object_type))  {
		case XDS_ALGORITHM_IDENT:
			ret = CALL_PUT(algorithm_ident);
			break;
		case XDS_CERTIFICATE:
            ret = CALL_PUT(certificate);
			break;
		case XDS_CERTIFICATE_LIST:
			ret = CALL_PUT(certificate_list);
			break;
		case XDS_CERTIFICATE_PAIR:
			ret = CALL_PUT(certificate_pair);
			break;
		case XDS_CERTIFICATE_SUBLIST:
			ret = CALL_PUT(certificate_sublist);
			break;
        default:
            ret  = (OM_NO_SUCH_CLASS);
            break;
        } /* endswitch */
        break;

	default:
		ret	 = OM_NO_SUCH_CLASS;
		break;
	} /* endswitch */

end:
	if (!ret) {
		ret = OM_SUCCESS;
	} /* endif */

	return (ret);

} /* end dsP_put_switch	*/


/* search a	type in	a list */
/* return TRUE if found	   */
/* else	return FALSE	   */
static OM_boolean
type_in_list(
	OM_type		type,
	OM_type_list	list
)
{
	while (*list != OM_NO_MORE_TYPES) {
		if (type == *list++) {
			return (OM_TRUE);
		} /* endif */
	} /* endwhile */
	return (OM_FALSE);
} /* end type_in_list */


/************************************************************************/
/*																		*/
/* update a	string														*/
/*																		*/
/* if a	buffer string was previously allocated :						*/
/*	  if the new string	needs a	different number of	STRING UNIT			*/
/*		  the old buffer string	is freed								*/
/*		  a	new	buffer string is allocated								*/
/*		  the new string is	copied										*/
/*	  else																*/
/*		  the new string is	copied in the convenient old buffer	string	*/
/* else																	*/
/*	  a	new	buffer string is allocated									*/
/*	  the new string is	copied											*/
/*																		*/
/* INPUT :																*/
/*	  len :	pointer	to current string length							*/
/*	  str :	address	of pointer to current string						*/
/*	  new_len :	new	string length										*/
/*	  new_str :	pointer	to new string									*/
/*	  modification : kind of modification								*/
/*																		*/
/* OUTPUT :																*/
/*	  *len : new string	length											*/
/*	  **str	: new string pointer										*/
/*																		*/
/* RETURN_CODE ERROR:													*/
/*	  OM_WRONG_VALUE_MAKEUP: the new string	is not defined				*/
/*	  OM_MEMORY_INSUFFICIENT: no free string to	allocate				*/
/*																		*/
/************************************************************************/
OM_return_code
dsP_update_string(
	OM_sint	*len,
	void **str,
	OM_uint32	new_len,
	void *new_str,
	OM_modification	modification,
	OMX_workspace	workspace
)
{
	OM_sint32	old_l;
	OM_sint32	new_l;
	OM_return_code	ret = OM_SUCCESS;

	if (((old_l = (((*len == 0)
	|| (*len == OM_LENGTH_UNSPECIFIED)) ? 0 : *len)) == 0)
	&& (*str != 0)) {
		old_l = strlen (*str);
	} /* endif */
	if (((new_l =(((new_len == 0)
	|| (new_len == OM_LENGTH_UNSPECIFIED)) ? 0 : new_len)) == 0)
	&& (new_str != 0)) {
		new_l = strlen (new_str);
	} /* endif */
	if (OMP_REMOVE) {
		if (old_l) {
			OMP_XFREE_STR(workspace, *str, old_l);
		} /* endif */
		*len = 0;
		*str = 0;
		ret =  OM_SUCCESS;
		goto end;
	} /* endif */
	if (new_l == 0) {
		ret = OM_WRONG_VALUE_MAKEUP;
		goto end;
	} /* endif */
	if ((old_l != 0) && (ROUNDUP(workspace, new_l + 1)
	!= ROUNDUP(workspace, old_l + 1))) {
		OMP_XFREE_STR(workspace, *str, old_l);
		old_l = 0;
	} /* endif */
	if (old_l == 0) {
		if ((*str = (unsigned char *)OMP_XALLOC_STR
		(workspace, new_str, new_l)) == 0) {
			ret = OM_MEMORY_INSUFFICIENT;
			goto end;
		} /* endif */
	} else {
		memcpy (*str, new_str, new_l + 1);
	} /* endif */

	*len = new_l;
end:
	return(ret);
} /* end dsP_update_string */


/************************************************************************/
/*																		*/
/* validate_type_and_mod												*/
/*																		*/
/*	 check the validity	of the types of	the	included_types list	and		*/
/*	 of	the	kind of	modification for each requested	type				*/
/*																		*/
/* INPUT :																*/
/*	 t_l : structure OMP_type_list for all types of	attributes allowed	*/
/*		   in the destination object									*/
/*	 list :	include_types list											*/
/*	 initial_value : om_put	parameter									*/
/*	 limiting_value	: om_put parameter									*/
/*																		*/
/* OUTPUT :																*/
/*	 t_l : each	entry corresponding	to an allowed type of attribute		*/
/*		   not present in the included_types list is modified:			*/
/*		   max_val = 0 => the corresponding	type is	not	requested		*/
/*																		*/
/* RETURN_CODE ERROR:													*/
/*	 OM_WRONG_VALUE_TYPE: a	type in	included_types list	is not allowed	*/
/*	 OM_NOT_PRESENT	: initial or/and limiting values is/are	incompatible*/
/*					  with the constraint of the type of an	attribute	*/
/*					  (kind	of modification	not	appropriate)			*/
/*																		*/
/************************************************************************/
OM_return_code
dsP_validate_type_and_mod(
	OMP_type_list	*t_l,
	OM_type_list	list,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
	OMP_type_list	 * pt_l;
	OM_type			type;
	OM_type			typs;
	OM_sint			single,	multi;
	OM_return_code	ret = OM_SUCCESS;

	single = multi = 0;

	/* increment the OMP_type_list pointer to skip the OM_CLASS	type */
	if (t_l->type == OM_CLASS) {
		t_l->max_val = 0;
		t_l++;
	} /* endif */

	if (list == OM_NO_MORE_TYPES) {
		ret = OM_SUCCESS;
		goto end;
	} else {
		for	(; (type = *list) != OM_NO_MORE_TYPES; list++) {
			for	(pt_l = t_l; (typs = pt_l->type) != OM_NO_MORE_TYPES; pt_l++) {
				if (typs == type) {
					pt_l->nb_val++;
					break;
				} /* endif */
			} /* endfor */
			if (typs == OM_NO_MORE_TYPES) {
				ret = OM_WRONG_VALUE_TYPE;
				goto end;
			} /* endif */
		} /* endfor */
	} /* endif */
	for	(pt_l = t_l; pt_l->type	 != OM_NO_MORE_TYPES; pt_l++) {
		if (pt_l->nb_val == 0) {
			pt_l->max_val = 0;
		} else {
			pt_l->nb_val = 0;
			if (pt_l->max_val) {
				if (pt_l->max_val == 1) {
					single++;
				} else {
					multi++;
				} /* endif */
			} /* endif */
		} /* endif */
	} /* endfor */
	if ((single) &&	!(OMP_REPLACE_SINGLE_VALUE))
		ret =  OM_WRONG_VALUE_NUMBER;

end:
	return(ret);
} /* end dsP_validate_type_and_mod	*/


/************************************************************************/
/*																		*/
/* dsP_pub_type_in_list														*/
/*																		*/
/* - check the validity	of the types of	attributes of a	list of			*/
/*	 descriptors in	comparison with	the	allowed	types, if no			*/
/*	 included_types	list is	provided,									*/
/* - select	in the descriptor list the descriptors related to the		*/
/*	 requested attribute types											*/
/* - in	case of	multi-valued attribute type, check if the number of		*/
/*	 values	doesn't	exceed the max number allowed, and if the values	*/
/*	 are contiguous														 */
/* - check the validity	of the kind	of modification	for	each			*/
/*	 requested type														*/
/*																		*/
/* INPUT :																*/
/*	 t_l : structure OMP_type_list for all types of	attributes allowed	*/
/*		   in the destination object									*/
/*	 included_types	: list of requested	types							*/
/*	 initial_value : om_put	parameter									*/
/*	 limiting_value	: om_put parameter									*/
/*	 desc_ptr :	pointer	to a descriptor	list ended by a	NULL_DESCRIPTOR	*/
/*																		*/
/* OUTPUT :																*/
/*	 *t_l :	each entry of the OMP_type_list	related	to a requested		*/
/*			attribute type which appears in	the	descriptor is modified:	*/
/*			nb_val = number	of descriptors for the type					*/
/*			desc = pointer to the first	of the nb_val descriptors		*/
/*																		*/
/* RETURN_CODE ERROR:													*/
/*	 OM_WRONG_VALUE_TYPE: an attribute type	in descriptor list is not	*/
/*					  allowed											*/
/*	 OM_TOO_MANY_VALUES: the number	of values in the descriptor	list	*/
/*						 for an	attribute type exceed the max number	*/
/*						 allowed for this type of attribute				*/
/*	 OM_VALUES_NOT_ADJACENT: the descriptors for the values	of a		*/
/*							 particular	attribute are not adjacent		*/
/*	 OM_NOT_PRESENT	: initial or/and limiting values is/are	incompatible*/
/*					  with the constraint of the type of an	attribute	*/
/*					  (kind	of modification	not	appropriate)			*/
/*																		*/
/************************************************************************/

OM_return_code
dsP_pub_type_in_list(
	OMP_type_list	*t_l,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OM_descriptor	*desc_ptr)
{
	OMP_type_list	 * pt_l;
	OM_type	type;
	OM_type	new_type;
	OM_type	prev_type = 0;
	OM_sint	single = 0;
	OM_sint	multi = 0;
	OM_return_code	ret = OM_SUCCESS;


	for	(new_type = desc_ptr->type;	desc_ptr->type != OM_NO_MORE_TYPES;
		desc_ptr++,	new_type = desc_ptr->type) {
		/* for each	type of	the	source check if	it is requested
		 * in the included_types list */

		if ((included_types	 == OM_NO_MORE_TYPES)
		|| (type_in_list(new_type, included_types))) {
			/* the type	is in the included types list
			 * check if	it is allowed for the destination object
			 */
			for	(pt_l = t_l; (type = pt_l->type) != OM_NO_MORE_TYPES; pt_l++) {
				if ((new_type != type) || (pt_l->max_val == 0)) {
					continue;
				} /* endif */

				if (pt_l->nb_val >= pt_l->max_val) {
					ret =  OM_TOO_MANY_VALUES;
					goto end;
				} /* endif */
				if (pt_l->nb_val == 0) {
					pt_l->desc = (void *)desc_ptr;
				} else if ((pt_l->nb_val) && (new_type != prev_type)) {
					ret = OM_VALUES_NOT_ADJACENT;
					goto end;
				} /* endif */
				/* increment the number	of values found
				 * in the source for that type
				 */
				pt_l->nb_val++;
				break;
			} /* endfor */
			if ((type == OM_NO_MORE_TYPES)
			&& (included_types != OM_NO_MORE_TYPES)) {
				ret = OM_WRONG_VALUE_TYPE;
				goto end;
			} /* endif */
		} /* endif */
		prev_type = new_type;
	} /* endfor */
	if (OMP_REMOVE) {
		ret = OM_SUCCESS;
		goto end;
	} /* endif */
	for	(pt_l = t_l; pt_l->type != OM_NO_MORE_TYPES; pt_l++) {
		if (pt_l->nb_val) {
			if (pt_l->max_val == 1) {
				single++;
			} else {
				multi++;
			} /* endif */
		} else {
			pt_l->max_val = 0;
		} /* endif */
	} /* endfor */
	if (single && !OMP_REPLACE_SINGLE_VALUE && !OMP_INSERT_AT_END) {
		ret = OM_NOT_PRESENT;
		goto end;
	} /* endif */
end:
	return(ret);
} /* end dsP_pub_type_in_list */


#if 0 /* seems to be unused: if confirmed, remove it. CC 2.2.94 */
/* copy	n types	from an	old	types array	to a new types array */
static void
priv_cp_type(
	D2_a_type	**new_val,
	D2_a_type	*old_val,
	OM_sint16	nb_val)
{
	/* copy	type from old to new allocated array
	 * pointers, if	any, are copied	too, but not the objects
	 * which are pointed to
	 */
	memcpy (*new_val, old_val, nb_val * sizeof(D2_a_type));
	*new_val = *new_val	 + nb_val;
} /* end priv_cp_type */


/* copy	n values from an old values	array to a new values array */
static void
priv_cp_val(
	D2_a_value	**new_val,
	D2_a_value	*old_val,
	OM_sint16	nb_val)
{
	/* copy	values from	old	to new allocated array
	 * pointers, if	any, are copied	too, but not the objects
	 * which are pointed to
	 */
	memcpy (*new_val, old_val, nb_val * sizeof(D2_a_value));
	*new_val = *new_val	 + nb_val;
} /* end priv_cp_val */

#endif /* 0 - seems to be unused */

/* copy	n criterias	from a list	of descriptors in a	criterias array	*/
static OM_return_code
pub_cp_crit(
	OMX_workspace	workspace,
	D2_criteria	**a_val,
	OM_modification	modification,
	OM_descriptor	*desc,
	OM_sint16	nb_val)
{
	OM_sint	i;
	OM_descriptor	 * pt_d;
	OM_return_code	ret	 = OM_SUCCESS;
	void	*dest;
	OM_object		source;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OMP_object_type		src_type;

	for	(i = 0,	pt_d = desc; i < nb_val; i++, pt_d++, (*a_val)++) {
		if (OMP_REMOVE) {
			break;
		} /* endif */
		source = pt_d->value.object.object;
		if (source == 0) {
			ret =  OM_WRONG_VALUE_MAKEUP;
			goto end;
		} /* endif */

		src_pub	 = (OM_public_object	)0;
		if (OMP_PRIVATE(source)) {
			/* check valid handle */
			if (!OMP_VALID_OBJECT(source)) {
				ret = OM_NO_SUCH_OBJECT;
				goto end;
			} /* endif */

			if ((ret = om_get(
					source,
					OM_NO_EXCLUSIONS,
					0,
					0, 
					0,
					0,
					&src_pub,
					&desc_nb))	!= OM_SUCCESS) {
				goto end;
			} /* endif */
			if (desc_nb	 == 0) {
				/*delete NULL descriptor*/
				om_delete(src_pub);
				ret =  OM_NO_SUCH_OBJECT;
				goto end;
			} /* endif */
			source = src_pub;
		} /* endif */

		/* test	OM_CLASS of	the	sub_object */
		if (OMP_TYPE(source) != OM_CLASS) {
			ret	 = OM_NO_SUCH_OBJECT;
		} /* endif */
		/* source public object	is in the current workspace	*/
		if ((ret != OM_SUCCESS)	 ||	((ret = omP_get_class_sub_id (workspace,
		source->value.string, &src_type)) != OM_SUCCESS)) {
			if (source == src_pub) {
				om_delete (src_pub);
			} /* endif */
			goto end;
		} /* endif */

		switch (OMP_PACKAGE(src_type)) {
		case XDS_BDCP:
			switch (OMP_CLASS(src_type)) {
			case XDS_SEARCH_CRITERION:
				dest = (void *)	 & (*a_val);
				ret	 = dsP_put_search_criterion(workspace, dest,
					 source, src_type, OM_REPLACE_ALL, 0,	0,
					 OM_ALL_VALUES);
				break;
			default:
				ret	 = OM_WRONG_VALUE_TYPE;
			} /* endswitch */
			break;
		default:
			ret	 = OM_NO_SUCH_CLASS;
		} /* endswitch */
		if (source == src_pub) {
			om_delete (src_pub);
		} /* endif */
		if (ret	 != OM_SUCCESS) {
			break;
		} /* endif */
	} /* endif */
end:
	return(ret);
} /* end pub_cp_crit */


/* copy	n strings from a list of descriptors in	a strings array	*/
static OM_return_code
pub_cp_str(
	OMX_workspace	workspace,
	D2_str		**a_val,
	OM_modification	modification,
	OM_descriptor	*desc,
	OM_sint16	nb_val)
{
	OM_sint	i;
	OM_descriptor	 * pt_d;
	OM_return_code	ret = OM_SUCCESS;
	for	(i = 0,	pt_d = desc; i < nb_val; i++, pt_d++, (*a_val)++) {
		UPDATE_STRING (((*a_val)->d2_size), ((*a_val)->d2_value),
		pt_d->value.string.length, pt_d->value.string.elements);
		if (ret	 != OM_SUCCESS) {
			goto end;
		} /* endif */

	} /* endfor */
end:
	return(ret);
} /* end pub_cp_str	*/



/* copy	n types	from a list	of descriptors in a	types array	*/
static OM_return_code
pub_cp_type(
	OMX_workspace	workspace,
	D2_a_type	**a_val,
	OM_modification	modification,
	OM_descriptor	*desc,
	OM_sint16	nb_val)
{
	OM_sint	i;
	OM_descriptor	 * pt_d;
	OM_return_code	ret = OM_SUCCESS;
	for	(i = 0,	pt_d = desc; i < nb_val; i++, pt_d++, (*a_val)++) {
		UPDATE_STRING (((*a_val)->d2_typ_len),((*a_val)->d2_type),
		pt_d->value.string.length,pt_d->value.string.elements);
		if (ret	 != OM_SUCCESS) {
			goto end;
		} /* endif */

	} /* endfor */
end:
	return(ret);
} /* end pub_cp_type */



/* copy	n values from a	list of	descriptors	in a values	array */
static OM_return_code
pub_cp_val(
	OMX_workspace	workspace,
	D2_a_value	**a_val,
	OM_modification	modification,
	OM_descriptor	*desc,
	OM_sint16	nb_val)
{
	OM_sint	i;
	OM_descriptor	 * pt_d;
	OM_return_code	ret = OM_SUCCESS;
	OM_sint	len;
	void	*dest;
	OM_object		source;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OMP_object_type		src_type;

	for	(i = 0,	pt_d = desc; i < nb_val; i++, pt_d++, (*a_val)++) {
		switch (OMP_SYNTAX(pt_d)) {
		case OM_S_BOOLEAN:
			if (OMP_REMOVE) {
				break;
			} /* endif */
			(*a_val)->d2_at_value.d2_bool_a_v = pt_d->value.boolean;
			(*a_val)->d2_a_rep = OM_TO_IAPL[OMP_SYNTAX(pt_d)];
			break;
		case OM_S_INTEGER:
			if (OMP_REMOVE) {
				break;
			} /* endif */
			(*a_val)->d2_at_value.d2_int_a_v = pt_d->value.integer;
			(*a_val)->d2_a_rep = OM_TO_IAPL[OMP_SYNTAX(pt_d)];
			break;
		case OM_S_ENUMERATION:
			if (OMP_REMOVE) {
				break;
			} /* endif */
			(*a_val)->d2_at_value.d2_int_a_v = pt_d->value.enumeration;
			(*a_val)->d2_a_rep = OM_TO_IAPL[OMP_SYNTAX(pt_d)];
			break;
		case OM_S_NULL:
			break;
		case OM_S_OBJECT:
			if (OMP_REMOVE) {
				break;
			} /* endif */
			source = pt_d->value.object.object;
			if (source == 0) {
				ret =  OM_WRONG_VALUE_MAKEUP;
				goto end;
			} /* endif */

			src_pub	 = (OM_public_object	)0;
			if (OMP_PRIVATE(source)) {
				/* check valid handle */
				if (!OMP_VALID_OBJECT(source)) {
					ret = OM_NO_SUCH_OBJECT;
					goto end;
				} /* endif */

				if ((ret = om_get(
								source,
								OM_NO_EXCLUSIONS,
								0,
								0,
								0,
								0,
								&src_pub,
								&desc_nb)) !=  OM_SUCCESS) {
					goto end;
				} /* endif */
				if (desc_nb	 == 0) {
					/*delete NULL descriptor*/
					om_delete(src_pub);
					ret = OM_NO_SUCH_OBJECT;
					goto end;
				} /* endif */
				source = src_pub;
			} /* endif */
			ret	 = OM_SUCCESS;
			/* test	OM_CLASS of	the	sub_object */
			if (OMP_TYPE(source) != OM_CLASS) {
				ret	 = OM_NO_SUCH_OBJECT;
			} /* endif */
			/* source public object	is in the current workspace	*/
			if ((ret != OM_SUCCESS)	 ||	((ret = omP_get_object_type	(workspace,
			source->value.string, &src_type)) != OM_SUCCESS)) {
				if (source == src_pub) {
					om_delete (src_pub);
				} /* endif */
				goto end;
			} /* endif */

			switch (OMP_PACKAGE(src_type)) {
			case XDS_SERVICE_PACKAGE:
				switch (OMP_CLASS(src_type)) {
				case XDS_PRESENTATION_ADDRESS:
					/* put_psap	*/
					PUT_VAL(psap, d2_psap_a_v, D2_PSAP_STX);
					(*a_val)->d2_a_v_len = dsP_get_psap_length
					((*a_val)->d2_at_value.d2_psap_a_v);
					break;
				case XDS_DS_DN:
					/* dsP_put_dist_name */
					PUT_VAL(dist_name, d2_obj_name_a_v, D2_DISTNAME);
					if ((*a_val)->d2_at_value.d2_obj_name_a_v) {
						(*a_val)->d2_a_v_len = 1 +
						strlen((char *)(*a_val)->d2_at_value.d2_obj_name_a_v) ;
					} /* endif */
#if 0
					if (((*a_val)->d2_a_v_len = strlen((char *)
					(*a_val)->d2_at_value.d2_obj_name_a_v)))
						((*a_val)->d2_a_v_len)++;
#endif /* 0 */
					break;
				default:
					ret	 = OM_WRONG_VALUE_TYPE;
				} /* endswitch */
				break;
			case XDS_GDS_PACKAGE:
				switch (OMP_CLASS(src_type)) {
				case XDS_GDS_ACL:
					/* put_acl */
					PUT_VAL(acl, d2_oct_a_v, D2_ACCL);
					(*a_val)->d2_a_v_len 
					= dsP_get_acl_length((*a_val)->d2_at_value.d2_oct_a_v);
					break;
				default:
					ret	 = OM_WRONG_VALUE_TYPE;
				} /* endswitch */
				break;
			case XDS_BDCP:
				switch (OMP_CLASS(src_type)) {
				case XDS_FACSIMILE_PHONE_NBR:
					/* put_faxnumber */
					PUT_VAL_NULL_LENGTH(faxnumber, d2_faxnr_a_v, D2_FAXNR_STX);
					break;
				case XDS_POSTAL_ADDRESS:
					/* put_postal_address */
					PUT_VAL_NULL_LENGTH(postal_address, d2_post_a_v,
					D2_T61_PR_LIST);
					break;
				case XDS_SEARCH_GUIDE:
					/* put_search_guide	*/
					PUT_VAL_NULL_LENGTH(search_guide, d2_guide_a_v, D2_GUIDE);
					break;
				case XDS_TELETEX_TERM_IDENT:
					/* put_ttxid */
					PUT_VAL_NULL_LENGTH(ttxid, d2_ttxid_a_v, D2_TTXID_STX);
					break;
				case XDS_TELEX_NBR:
					/* put_telex_number	*/
					PUT_VAL_NULL_LENGTH(telex_number,d2_tlxnr_a_v,D2_TLXNR_STX);
					break;
				default:
					ret	 = OM_WRONG_VALUE_TYPE;
				} /* endswitch */
				break;
				/*	CASE_PUT_VALUE_MDUP_PACKAGE	*/
#ifdef	_MDUP_PKG_
			case XDS_MDUP:
				switch (OMP_CLASS(src_type)) {
				case MHS_DLSP:
					/* put_dlsp	*/
					PUT_VAL_NULL_LENGTH(dlsp, d2_dl_s_p_av, D2_MHS_DLSP_STX);
					break;
				default:
					ret	 = OM_WRONG_VALUE_TYPE;
				} /* endswitch */
				break;
			case XMH_PACKAGE:
				switch (OMP_CLASS(src_type)) {
				case XMH_OR_NAME:
					/* put_OR_name */
					PUT_VAL_NULL_LENGTH(OR_name, d2_or_name_av,
					D2_MHS_ORNAME_STX);
					break;
				case XMH_OR_ADDRESS:
					/* put_OR_address */
					PUT_VAL_NULL_LENGTH(OR_address, d2_or_addr_av,
					D2_MHS_ORADR_STX);
					break;
				default:
					ret	 = OM_WRONG_VALUE_TYPE;
				} /* endswitch */
				break;
#endif	/* _MDUP_PKG_ */
			case XDS_SAP_PACKAGE:
					switch (OMP_CLASS(src_type)) {
				case XDS_CERTIFICATE:
					PUT_VAL_NULL_LENGTH(certificate,d2_cert_a_v,
					D2_CERTIFICATE);
					break;
				case XDS_CERTIFICATE_LIST:
					PUT_VAL_NULL_LENGTH(certificate_list,d2_ctlist_a_v,
					D2_CERT_LIST);
					break;
				case XDS_CERTIFICATE_PAIR:
					PUT_VAL_NULL_LENGTH(certificate_pair,d2_ctpair_a_v,
					D2_CERT_PAIR);
					break;
				default:
					ret  = (OM_NO_SUCH_CLASS);
					break;
				} /* endswitch */
				break;
			default:
					ret	 = OM_NO_SUCH_CLASS;
			} /* endswitch */
			if (source == src_pub) {
				om_delete (src_pub);
			} /* endif */
			if (ret	 != OM_SUCCESS) {
				goto end;
			} /* endif */
			break;
		case OM_S_GENERAL_STRING:
		case OM_S_GENERALISED_TIME_STRING:
		case OM_S_GRAPHIC_STRING:
		case OM_S_IA5_STRING:
		case OM_S_OBJECT_DESCRIPTOR_STRING:
		case OM_S_VIDEOTEX_STRING:
		case OM_S_VISIBLE_STRING:
			ret =  OM_WRONG_VALUE_SYNTAX;
			goto end;
		case OM_S_BIT_STRING:
			len	 = (((*a_val)->d2_a_v_len +7) & -8) /8;
			UPDATE_STRING (len, ((*a_val)->d2_at_value),
			((pt_d->value.string.length + 7) & -8) /8,
			pt_d->value.string.elements);
			if (ret != OM_SUCCESS) {
				goto end;
			} /* endif */
			(*a_val)->d2_a_v_len = pt_d->value.string.length;
			(*a_val)->d2_a_rep = OM_TO_IAPL[OMP_SYNTAX(pt_d)];
			break;
		case OM_S_ENCODING_STRING:
		case OM_S_NUMERIC_STRING:
		case OM_S_OBJECT_IDENTIFIER_STRING:
		case OM_S_OCTET_STRING:
		case OM_S_PRINTABLE_STRING:
		case OM_S_UTC_TIME_STRING:
			len	 = (*a_val)->d2_a_v_len;
			UPDATE_STRING (len, ((*a_val)->d2_at_value),
			pt_d->value.string.length, pt_d->value.string.elements);
			if (ret != OM_SUCCESS) {
				goto end;
			} /* endif */
			(*a_val)->d2_a_v_len = len;
			(*a_val)->d2_a_rep = OM_TO_IAPL[OMP_SYNTAX(pt_d)];
			break;
		case OM_S_TELETEX_STRING:
			len	 = (*a_val)->d2_a_v_len;
			if (pt_d->syntax & OM_S_LOCAL_STRING) {
				unsigned char * result; OM_uint32 result_len = 0;
				if ( d27_local_to_t61( pt_d->value.string.elements,
				pt_d->value.string.length,
				(char **)&result, &result_len) != D2_NOERROR) {
					ret = OM_WRONG_VALUE_SYNTAX;
					goto end;
				} else {
					pt_d->value.string.length = result_len;
					pt_d->value.string.elements = result;
					/* possible memory leak */
				} /* endif */
			} /* endif */

			UPDATE_STRING (len, ((*a_val)->d2_at_value),
			pt_d->value.string.length, pt_d->value.string.elements);
			if (ret != OM_SUCCESS) {
				goto end;
			} /* endif */
			(*a_val)->d2_a_v_len = len;
			(*a_val)->d2_a_rep = OM_TO_IAPL[OMP_SYNTAX(pt_d)];
			break;

#if 0
			case DSX_S_INTEGER_LIST:
			len = (*a_val)->d2_a_v_len;
			UPDATE_STRING (len, ((*a_val)->d2_at_value),
			pt_d->value.string.length, pt_d->value.string.elements);
			if (ret	!= OM_SUCCESS) {
				goto end;
			} /* endif */
			(*a_val)->d2_a_v_len = len;
			(*a_val)->d2_a_rep = D2_INT_LIST;
			break;
#endif /* 0 */

		default:
			ret = OM_WRONG_VALUE_SYNTAX;
			goto end;
		} /* endswitch */

	} /* endfor */
end:
	return (ret);
} /* end pub_cp_val	*/




/************************************************************************/
/*																		*/
/* Function	update_array()						*/
/*																		*/
/* INPUT :																*/
/*	 workspace :														*/
/*	 nb_val_d :	pointer	to number of values	in the array of	values		*/
/*	 val_d_p  :	pointer	to the address of an array of values			 */
/*	 modification :														*/
/*	 initial_value :													*/
/*	 limiting_value:													*/
/*	 pt_l :	pointer	to an element of an	OMP_type_List					*/
/*	 type :	type of	the	values to update (value, type, str,	criteria,..)*/
/*																		*/
/* OUTPUT :																*/
/*	 *nb_val_d : updated with the new number of	values					*/
/*	 *val_d_p  : updated with the new array	of values					*/
/*																		*/
/* RETURN_CODE ERROR:													*/
/*	 OM_WRONG_VALUE_NUMBER:	an attribute would have	a value	that		*/
/*							violates the value number constraints		*/
/*							in force.									*/
/*																		*/
/************************************************************************/

static OM_return_code
update_array (
	OMX_workspace	workspace,
	short *nb_val_d,
	void **val_d_p,
	OM_modification	modification,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	OMP_type_list	*pt_l,
	OM_sint	type)
{
	OM_return_code	ret = OM_SUCCESS;
	void	*old_val_p;
	void	*new_val_p;
	OM_sint	new_nb_val;
	OM_sint	n_cur;
	OM_sint	i;
	char	*ptr;
	ARRAY_VAR;
	/* if necessary,
	 * update initial and limiting values with
	 * the existing	number of values
	 */
	UPDATE_LIMIT_VAL(*nb_val_d,		return(ret));

	/* if the number of	values in the destination object
	 * after update	is unchanged, no space allocation.
	 * otherwise allocate the exact	number of attribute
	 * value structures
	 */
	old_val_p = *val_d_p;
	if ((new_nb_val	 = initial_value	 +	pt_l->nb_val	 +	(*nb_val_d -
		limiting_value)) != *nb_val_d) {
		/* if the number of	values is greater than	*/
		/* the max number allowed, return error		*/
		if (new_nb_val > pt_l->max_val) {
			ret = OM_WRONG_VALUE_NUMBER;
			goto end;
		} /* endif */

		/* if remove all the values					*/
		/* remove the array	of values				*/
		if (new_nb_val == 0) {
			DELETE_ARRAY;
			*val_d_p = 0;
			*nb_val_d = 0;
			ret = OM_SUCCESS;
			goto end;
		} /* endif */
		/* clear the values	which will be replaced or removed */
		if (new_val_p = old_val_p) {
			CLEAR_ELEMENTS;
		} /* endif */
		ALLOC_ARRAY;
	} else {
		new_val_p = old_val_p;
		/* clear the values	which will be replaced */
		CLEAR_ELEMENTS;
	} /* endif */
	n_cur = 0;
	/* copy	values from	the	begining to	initial	value */
	if ((new_nb_val	 != *nb_val_d) && (initial_value != 0)) {
		COPY_PRI_ELEMENTS(new_val_p, 0,	initial_value);
	} /* endif */
	n_cur += initial_value;

	/* copy	the	new	values */
	COPY_PUB_ELEMENTS;

	if (ret	 != OM_SUCCESS) {
		goto end;
	} /* endif */
	n_cur += pt_l->nb_val;

	/* copy	values from	limiting value to the last value */
	if ((new_nb_val	 != *nb_val_d) &&  (limiting_value != *nb_val_d)) {
		COPY_PRI_ELEMENTS(new_val_p,  limiting_value, *nb_val_d	 
		- limiting_value);
	} /* endif */

	n_cur += (*nb_val_d	 - limiting_value);
	/* free	the	old	array of values	*/
	if ((new_nb_val	 != *nb_val_d) && (*nb_val_d)) {
		FREE_ARRAY;
	} /* endif */
	*nb_val_d = n_cur;
end:
	return(ret);
} /* end update_array */



/********************************************************/
/*							*/
/*	put	ATTRIBUTE object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_put_attribute (
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
#define	ATTR_OBJ	((xds_attribute	*)*xds_object)

	OM_return_code	ret = OM_SUCCESS;
	OMP_type_list	 * pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_descriptor	 * desc_ptr;
	xds_attribute	*attribute_obj = *xds_object;

	OMP_type_list	type_list[]	 = A_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
	limiting_value);
	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		ALLOCATE_OBJECT(ATTRIBUTE, xds_attribute);
		attribute_obj = *xds_object;
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case DS_ATTRIBUTE_TYPE :
				VALIDATE_SYNTAX(pt_l->desc,	OM_S_OBJECT_IDENTIFIER_STRING);
				UPDATE_STRING (	(ATTR_OBJ->d2_a_type.d2_typ_len),
				(ATTR_OBJ->d2_a_type.d2_type),
				((OM_descriptor	*)pt_l->desc)->value.string.length,
				((OM_descriptor *)pt_l->desc)->value.string.elements);
				break;

			case DS_ATTRIBUTE_VALUES :
				UPDATE_VALUE_ARRAY ( &(ATTR_OBJ->d2_a_no_val),
				(void *) &(ATTR_OBJ->d2_a_val));
				break;
			} /* endswitch */
			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	attribute_obj = (xds_attribute *)*xds_object; /* for debugging */
	return(ret);
} /* end dsP_put_attribute */


/********************************************************/
/*							*/
/*	put	ATTRIBUTE LIST object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_put_attr_list(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{

	OM_return_code	ret = OM_SUCCESS;
	OM_sint	i;
	OMP_type_list	 * pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_descriptor	 * desc_ptr;
	OM_descriptor	 * desc;
	xds_attr_list	*attr_list_obj = *xds_object;

	void	*link;
	void	**pt_obj;
	void	*pt;
	short	nb_val_d;
	short	n_cur;

	OMP_type_list	type_list[]	 = A_L_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
	limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *	)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case DS_ATTRIBUTES :
				VALIDATE_SYNTAX(pt_l->desc,	OM_S_OBJECT);

				/* if necessary,
			 * update initial and limiting values with
			 * the existing	number of values
			 */
				GET_NB_OBJ(xds_attribute, d2_a_next, nb_val_d,
				xds_object, pt_obj);
				UPDATE_LIMIT_VAL(nb_val_d, break);

				/* jump	values from	the	begining to	initial	value */
				JUMP_N_OBJ(xds_attribute, d2_a_next, initial_value,
				xds_object,pt_obj,n_cur);

				/* copy	the	new	objects	*/
				if (OMP_INSERT)	 {
					INSERT_OBJ(ATTRIBUTE, xds_attribute,
					pt_l->nb_val, d2_a_next, dsP_put_attribute);
				} else if (OMP_REPLACE)	 {
					/*delete the objects which have	to be replaced*/
					DELETE_OBJ(xds_attribute, limiting_value - initial_value,
					d2_a_next, dsP_del_attribute);

					/* replace them	by the new ones	*/
					INSERT_OBJ(ATTRIBUTE, xds_attribute,
					pt_l->nb_val, d2_a_next, dsP_put_attribute);
				} /* endif */
				break;
			} /* endswitch */
			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	attr_list_obj = (xds_attr_list *)*xds_object; /* for debugging */
	return(ret);
} /* end dsP_put_attr_list */


/********************************************************/
/*							*/
/*	put	AVA	object					*/
/*							*/
/********************************************************/
static OM_return_code
dsP_put_ava	(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
#define	AVA_OBJ	((xds_ava *)*xds_object)
	OM_return_code	ret = OM_SUCCESS;
	OMP_type_list	 * pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_descriptor	 * desc_ptr;
	D2_a_value	 * val_ptr;
	xds_ava		*ava_obj = *xds_object;

	OMP_type_list	type_list[]	 = AVA_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		ALLOCATE_OBJECT(AVA, xds_ava);
		ava_obj = *xds_object; /* repeated after ALLOCATE_OBJECT */
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case DS_ATTRIBUTE_TYPE :
				VALIDATE_SYNTAX(pt_l->desc,	OM_S_OBJECT_IDENTIFIER_STRING);
				UPDATE_STRING (	(AVA_OBJ->d2_avatype.d2_typ_len),
				(AVA_OBJ->d2_avatype.d2_type),
				((OM_descriptor	*)pt_l->desc)->value.string.length,
				((OM_descriptor *)pt_l->desc)->value.string.elements);
				break;

			case DS_ATTRIBUTE_VALUES :
				/* copy	the	new	value */
				val_ptr	 = &(AVA_OBJ->d2_avaval);
				PUB_CP_VAL (val_ptr, 1);
				break;
			} /* endswitch */
			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	ava_obj = (xds_ava *)*xds_object; /* for debugging */
	return(ret);
} /* end dsP_put_ava */


/********************************************************/
/*							*/
/*	put	CONTEXT	object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_put_context(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
	#define	CHECK_AND_UPDATE_CNTRLS(DESC,BIT_VAL)				\
{									\
	CHECK_SYNTAX(DESC,OM_S_BOOLEAN);				\
	if ((DESC)->value.boolean == OM_TRUE)				\
		((xds_context *)*xds_object)->d2_serv_cntrls |= BIT_VAL;	\
	else								\
		((xds_context *)*xds_object)->d2_serv_cntrls &= ~BIT_VAL;	\
}

	#define	UPDATE_CTX_CNTRLS(DESC,BIT_VAL)					\
{									\
	VALIDATE_SYNTAX(DESC,OM_S_BOOLEAN);				\
	if ((DESC)->value.boolean == OM_TRUE)				\
		((xds_context *)*xds_object)->d2_serv_cntrls |= BIT_VAL;	\
	else								\
		((xds_context *)*xds_object)->d2_serv_cntrls &= ~BIT_VAL;	\
}

#define	CTX_OBJ	((xds_context *) *xds_object)

	OM_return_code	ret = OM_SUCCESS;
	OM_sint	i;
	OMP_type_list	 * pt_l;
	OM_value_position	desc_nb;
	OM_public_object	src_pub;
	OM_boolean		priv_flag;
	OM_sint	check;
	OM_descriptor	 * desc_ptr;
	OM_descriptor	 * desc;
	OMP_object_type	src_type;
	OMP_type_list	 * type_list;
	OM_public_object	src_puba;
	OM_object	src;
	OM_descriptor	 * src_pt;

	void	*link;
	void	**pt_obj;
	void	*pt;
	short	nb_val_d;
	short	n_cur;
	OM_boolean	empty_obj = OM_FALSE;

	OMP_type_list	put_ctx_type_list[]	 = CTX_TYPE_LIST;

	OMP_type_list	put_cgx_type_list[]	 = CGX_TYPE_LIST;

	switch (OMP_PACKAGE(object_type)) {
	case XDS_SERVICE_PACKAGE:
		type_list = put_ctx_type_list;
		break;

	case XDS_GDS_PACKAGE:
		type_list = put_cgx_type_list;
		break;
	} /* endswitch */

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		if (!*xds_object) {
			empty_obj = OM_TRUE;
		} /* endif */
		ALLOCATE_OBJECT(CONTEXT, xds_context);
		if (empty_obj) {
			CTX_OBJ->d2_scope_ref = D2_WORLD_SCOPE;
			CTX_OBJ->d2_size_limit = D2_S_UNLIMITED;
			CTX_OBJ->d2_time_limit = D2_T_UNLIMITED;
		} /* endif */

		CHECK_INIT;
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case DS_EXT	:
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				GET_NB_OBJ(xds_extension, d2_next_ext, nb_val_d,
					 (void *)	 & CTX_OBJ->d2_extension, pt_obj);
				UPDATE_LIMIT_VAL(nb_val_d, break);
				JUMP_N_OBJ(xds_extension, d2_next_ext, initial_value,
						(void *) & CTX_OBJ->d2_extension,
					pt_obj,		n_cur);


				/* copy	the	new	objects	*/
				if (OMP_INSERT)	 {
					INSERT_OBJ(EXT,		xds_extension,
						pt_l->nb_val, d2_next_ext, dsP_put_extension);
				} else if (OMP_REPLACE)	 {
					/*delete the objects which have	to be replaced*/
					DELETE_OBJ(xds_extension, limiting_value	 -
						initial_value, d2_next_ext,	dsP_del_extension);

					/* replace them	by the new ones	*/
					INSERT_OBJ(EXT,		xds_extension,
						pt_l->nb_val, d2_next_ext, dsP_put_extension);
				} /* endif */
				break;

			case DS_OPERATION_PROGRESS :
				ret	 = OM_SUCCESS;
				if (check) {
					VALIDATE_SYNTAX(pt_l->desc,	OM_S_OBJECT);
					if (OMP_REMOVE)	 {
						ret	 = OM_WRONG_VALUE_NUMBER;
						break;
					} /* endif */
					desc = (OM_descriptor *	 )pt_l->desc;
					src	 = desc->value.object.object;
#if	0 /* qad 1.7.93	CC */
					if ((src != DS_OPERATION_NOT_STARTED) &&
						!(OMP_SPUB(src)	 ||	OMP_PRIVATE(src))) {
						ret	 = OM_NO_SUCH_OBJECT;
						break;
					} /* endif */
#endif /* 0	*/
					break;
				} /* endif */
#if 0
			CHECK_SYNTAX(pt_l->desc,OM_S_OBJECT);
			if (OMP_REMOVE) {
				CTX_OBJ->d2_op_progress.d2_nr_phase = D2_NOT_STARTED;
				CTX_OBJ->d2_op_progress.d2_next_rdn = 0;
				break;
			} /* endif */
#endif /* 0 */
				desc = (OM_descriptor *		)pt_l->desc;
				src	 = desc->value.object.object;
				if (src	 == DS_OPERATION_NOT_STARTED) {
					CTX_OBJ->d2_op_progress.d2_nr_phase	 =
						D2_NOT_STARTED;
					CTX_OBJ->d2_op_progress.d2_next_rdn	 =
						0;
					break;
				} /* endif */
				src_puba = (OM_public_object )0;
				if (OMP_PRIVATE(src)) {
					/* check valid handle */
					if (!OMP_VALID_OBJECT(src))	 {
						ret	 = OM_NO_SUCH_OBJECT;
						break;
					} /* endif */

					if ((ret = om_get(
									src,
									OM_NO_EXCLUSIONS,
									0,
									0,
									0,
									0,
									&src_puba,
									&desc_nb)) != OM_SUCCESS) {
						break;
					} /* endif */
					if (desc_nb	 == 0) {
						/*delete NULL descriptor*/
						om_delete(src_puba);
						ret	 = OM_NO_SUCH_OBJECT;
						break;
					} /* endif */
					src	 = src_puba;
				} /* endif */
				/* test	OM_CLASS of	the	sub_object */
				if (OMP_TYPE(src) != OM_CLASS) {
					ret	 = OM_NO_SUCH_OBJECT;
				} /* endif */
				/* src public object is	in the current workspace */
				if ((ret != OM_SUCCESS)
				||	((ret = omP_get_class_sub_id ( workspace,
				src->value.string,	&src_type)) != OM_SUCCESS)) {
					if (src	 == src_puba) {
						om_delete (src_puba);
					} /* endif */
					break;
				} /* endif */
				src_pt = src;
				src_pt++;

				if (((OMP_PACKAGE(src_type)) == XDS_SERVICE_PACKAGE) &&
					((OMP_CLASS(src_type))	 == XDS_OPERATION_PROGRESS)) {
					for	(; src_pt->type	 != OM_NO_MORE_TYPES; src_pt++) {
						switch (OMP_TYPE(src_pt)) {
						case DS_NAME_RESOLUTION_PHASE:
							VALIDATE_SYNTAX(src_pt, OM_S_ENUMERATION);
							UPDATE_ENUM(src_pt,
							CTX_OBJ->d2_op_progress.d2_nr_phase);
							break;
						case DS_NEXT_RDN_TO_BE_RESOLVED:
							VALIDATE_SYNTAX(src_pt, OM_S_INTEGER);
							UPDATE_INTEGER(src_pt,
							CTX_OBJ->d2_op_progress.d2_next_rdn);
							break;
						default:
							ret	 = OM_NO_SUCH_SYNTAX;
						} /* endswitch */
						if (ret	 != OM_SUCCESS) {
							break;
						} /* endif */
					} /* endfor */
				} else {
					ret	 = OM_WRONG_VALUE_TYPE;
				} /* endif */
				if (src	 == src_puba) {
					om_delete (src_puba);
				} /* endif */
				break;

			case DS_ALIASED_RDNS :
				CHECK_INTEGER_AND_UPDATE((OM_descriptor*)pt_l->desc,
				CTX_OBJ->d2_al_rdn);
				break;

			case DS_CHAINING_PROHIB	:
				CHECK_AND_UPDATE_CNTRLS((OM_descriptor*)pt_l->desc,
				D2_NOCHAINING);
				break;

			case DS_DONT_DEREFERENCE_ALIASES :
				CHECK_AND_UPDATE_CNTRLS((OM_descriptor *)pt_l->desc,
				D2_DREFALIAS);
				break;

			case DS_DONT_USE_COPY :
				CHECK_AND_UPDATE_CNTRLS((OM_descriptor *)pt_l->desc,
				D2_NOCACHE);
				break;

			case DS_LOCAL_SCOPE	:
				CHECK_AND_UPDATE_CNTRLS((OM_descriptor *)pt_l->desc,
				D2_LOCALSCOPE);
				break;

			case DS_PREFER_CHAINING	:
				CHECK_AND_UPDATE_CNTRLS((OM_descriptor *)pt_l->desc,
				D2_PREF_CHAINING);
				break;

			case DS_PRIORITY :
				CHECK_VALID_ENUM_AND_UPDATE((OM_descriptor *)pt_l->desc,
				CTX_OBJ->d2_priority, priority_type_limit);
				/*
			CHECK_ENUM_AND_UPDATE((OM_descriptor *)pt_l->desc,
			CTX_OBJ->d2_priority);
*/
				break;
			case DS_SCOPE_OF_REFERRAL :
				CHECK_VALID_OPT_ENUM(pt_l->desc, scope_ref_type_limit);
				/*
			CHECK_SYNTAX(pt_l->desc,OM_S_ENUMERATION);
*/
				if (OMP_REMOVE) {
					CTX_OBJ->d2_scope_ref = D2_WORLD_SCOPE;
				} else {
					UPDATE_ENUM((OM_descriptor *  )pt_l->desc,
						 CTX_OBJ->d2_scope_ref);
				} /* endif */
				break;
			case DS_SIZE_LIMIT :
				CHECK_SYNTAX(pt_l->desc, OM_S_INTEGER);
				if (OMP_REMOVE) {
					CTX_OBJ->d2_size_limit =  D2_S_UNLIMITED;
				} else {
					UPDATE_INTEGER((OM_descriptor *)pt_l->desc,
					CTX_OBJ->d2_size_limit);
				} /* endif */
				break;
			case DS_TIME_LIMIT :
				CHECK_SYNTAX(pt_l->desc, OM_S_INTEGER);
				if (OMP_REMOVE) {
					CTX_OBJ->d2_time_limit =  D2_T_UNLIMITED;
				} else {
					UPDATE_INTEGER((OM_descriptor *)pt_l->desc,
					CTX_OBJ->d2_time_limit);
				} /* endif */
				break;
			case DS_ASYNCHRONOUS :
				CHECK_SYNTAX(pt_l->desc, OM_S_BOOLEAN);
				break;
			case DS_AUTOMATIC_CONTINUATION :
				CHECK_AND_UPDATE_CNTRLS((OM_descriptor *)pt_l->desc,
				D2_NOREFERRAL);
				break;
			case DSX_DUAFIRST :
				CHECK_AND_UPDATE_CNTRLS((OM_descriptor *)pt_l->desc,
				D2_DUAFIRST);
				break;

			case DSX_DONT_STORE	:
				CHECK_AND_UPDATE_CNTRLS((OM_descriptor *)pt_l->desc,
				D2_DONT_STORE);
				break;

			case DSX_NORMAL_CLASS :
				CHECK_AND_UPDATE_CNTRLS((OM_descriptor *)pt_l->desc,
				D2_NORMAL_CLASS);
				break;

			case DSX_PRIV_CLASS	:
				CHECK_AND_UPDATE_CNTRLS((OM_descriptor *)pt_l->desc,
				D2_PRIVILEGED_CLASS);
				break;

			case DSX_RESIDENT_CLASS	:
				CHECK_AND_UPDATE_CNTRLS((OM_descriptor *)pt_l->desc,
				D2_RESIDENT_CLASS);
				break;

			case DSX_USEDSA	:
				CHECK_AND_UPDATE_CNTRLS((OM_descriptor *)pt_l->desc,
				D2_USEDSA);
				break;

			case DSX_DUA_CACHE :
				CHECK_AND_UPDATE_CNTRLS((OM_descriptor *)pt_l->desc,
				D2_DUACACHE);
				break;

			case DSX_PREFER_ADM_FUNCS :
				CHECK_AND_UPDATE_CNTRLS((OM_descriptor *)pt_l->desc,
				D2_PREF_ADM_FUN);
				break;

			case DSX_SIGN_MECHANISM :
			case DSX_PROT_REQUEST :
				break;  /* for future use */
			} /* endswitch */
			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
		CHECK_END;
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_context */


/************************************************************************/
/*																		*/
/* update_dist_name														*/
/*																		*/
/*	 add in	a distinguished	name either:								*/
/*		an object identifier to	convert	in ASCII format	(convert=TRUE)	*/
/*	  or:																*/
/*		a value	which is a string (convert=FALSE)						*/
/*																		*/
/* INPUT :																*/
/*	 dn_ptr	: pointer to the beginning of the Distingished Name			*/
/*	 len_src : string length											*/
/*	 str_src : string pointer											*/
/*	 separator : type/value	separator (^C or ^D	or ^E or ^F)
*/

/*	 convert : OM_TRUE => the string has to	be converted				*/
/*			   OM_FALSE => the string is not to	be converted			*/
/*	 next_ava: OM_TRUE => the separator	is ^B in a multivalued RDN		*/
/*			   OM_FALSE => the separator is	^A							*/
/*																		*/
/* RETURN_CODE ERROR:													*/
/*	 OM_WRONG_VALUE_LENGTH:	the	final string length	would exceed the	*/
/*							max	length allowed							*/
/*																		*/
/************************************************************************/
static OM_return_code
update_dist_name (
	xds_dist_name	dn_ptr,
	OM_sint	len_src,
	void *str_src,
	char separator,
	OM_boolean	convert,
	OM_boolean	next_ava)
{
	char	*cur_ptr;
	OM_sint	i;
	OM_sint	count ;
	unsigned char	*str_ptr;
	unsigned char	c;
	long	num;
	OM_return_code ret = OM_SUCCESS;

	cur_ptr	 = (char *)dn_ptr +	strlen((char *)dn_ptr);
	if (strlen((char *)dn_ptr)) {
		memset (cur_ptr, convert ? (next_ava ? D2_AVA_SEP :	 D2_RDN_SEP):
		separator ,		1);
	} /* endif */
	if (len_src	 == OM_LENGTH_UNSPECIFIED) {
		len_src	 = strlen(str_src);
	} /* endif */

	if (strlen((char *)dn_ptr) + (convert ?	3 *	len_src : len_src ) + 1 >(unsigned int)D2_DNL_MAX) {
		ret = OM_WRONG_VALUE_LENGTH;
		goto end;
	} /* endif */
	count = strlen((char *)dn_ptr);
	if (convert) {
		str_ptr	 = (unsigned char *)str_src;
		num	 = 0;
		for	(i = 0;	i <	len_src; i++, str_ptr++) {
			c = *str_ptr;
			num	 = num *	128	 + (c &	0x7f);
			if ( c < 0x80) {
				sprintf((char *) & dn_ptr[count], "%d",
					num);
				count += strlen((char *) & dn_ptr[count]);
				dn_ptr[count++]	 = '.';
				num	 = 0;
			} /* endif */
		} /* endfor */
		dn_ptr[--count]	 = '\0';
	} else {
		memcpy (dn_ptr + strlen((char *)dn_ptr), str_src, len_src);
	} /* endif */
end:
	return(ret);
} /* end update_dist_name */


/************************************************************************/
/*																		*/
/* dsP_get_nb_rdn_ava														*/
/*																		*/
/* get the number of RDNs in a Distinguished Name  or					*/
/* get the number of AVAs in a Relative	Distinguished Name				*/
/* and build an	OM_string array	to describe	each RDN (or AVA) from the	*/
/* XDS dist_name structure												*/
/*																		*/
/* Each	RDN	is described with a	pointer	and	a length:					*/
/* For the first RDN:													*/
/*	   elements	points to the beginning	of one of the following	strings	*/
/*				type+^C+value											*/
/*			or	type+^C+value+^B+type+^C+value							*/
/*	   length is the length	of that	string								*/
/* For the next	RDNs:													*/
/*	   elements	points to the beginning	of one of the following	strings	*/
/*				^A+type+^C+value										*/
/*			or	^A+type+^C+value+^B+type+^C+value						*/
/*	   length is the length	of that	string								*/
/*																		*/
/* INPUT :																*/
/*	 dn_ptr	: XDS DN structure pointer									*/
/*	 desc_dn : pointer to an OM_string structures array					*/
/*	 rdn_nb	: pointer to the number	of RDN to return					*/
/*	 sep	: value	of the separator (^C or	^D or ^E or	^F)				*/
/*																		*/
/* OUTPUT:																*/
/*	 *rdn_nb : number of RDNs of the DN	(or	AVAs of	the	RDN)			*/
/*																		*/
/* RETURN_CODE ERROR:													*/
/*	 OM_WRONG_VALUE_MAKEUP:	the	DN structure is	incorrect:				*/
/*							an object identifier is	not	followed by		*/
/*							a value										*/
/*																		*/
/************************************************************************/

OM_return_code
dsP_get_nb_rdn_ava (
	char *dn_ptr,
	OM_string	*desc_dn,
	OM_sint	*rdn_nb,
	OM_sint	sep)
{
	unsigned char	c;
	char	*rdn_ptr;
	OM_sint	rdn_len;
	OM_return_code ret = OM_SUCCESS;

	*rdn_nb	 = 0;
	desc_dn->length	 = 0;
	desc_dn->elements = (char *) 0;
	if ((dn_ptr) &&	(dn_ptr[0] != '\0')) {
		for	(rdn_ptr = dn_ptr, c = D2_RDN_SEP, rdn_len =  -1;
		c != '\0'; (*rdn_nb)++,	desc_dn++, rdn_len =  0) {
			while (((c = rdn_ptr[++rdn_len]) < D2_T61_REP) && (c > D2_IA5_REP)){
				; /* not ^C^D^E^F */
			} /* endwhile */
			if (c == '\0') {
				ret = OM_WRONG_VALUE_MAKEUP;
				goto end;
			} /* endif */
			if (sep	 == D2_AVA_SEP) {
				while (((c = rdn_ptr[++rdn_len]) != sep)
				&& ((c	!= D2_RDN_SEP)	 &&	 (c	!= '\0'))) {
					;
				} /* endwhile */
			} else {
				while (((c = rdn_ptr[++rdn_len]) != sep) && (c != '\0')) {
					;
				} /* endwhile */
			} /* endif */
			desc_dn->length	 = rdn_len;
			desc_dn->elements = rdn_ptr;
			rdn_ptr	 += rdn_len;
			if ((sep == D2_AVA_SEP)	 &&	(c == D2_RDN_SEP)) {
				c = '\0';
			} /* endif */
		} /* endfor */
		desc_dn->elements = &dn_ptr[strlen(dn_ptr)];
	} /* endif */
end:
	return(ret);
} /* end dsP_get_nb_rdn_ava	*/


/************************************************************************/
/*																		*/
/* dsP_put_rdn1																*/
/*																		*/
/* function	called by dsP_put_dist_name() and dsP_put_rdn()	function process	*/
/* RDNS																	*/
/*																		*/
/************************************************************************/
static OM_return_code
dsP_put_rdn1 (
	OMX_workspace		workspace,
	void 				**xds_object,
	OM_object			source,
	OMP_object_type		object_type,
	OM_modification		modification,
	OM_type_list		included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value,
	xds_dist_name		dn_ptr)
{
	OM_return_code		ret = OM_SUCCESS;
	OM_sint				i;
	OMP_type_list	 	*pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean			priv_flag;
	OM_descriptor	 	*desc_ptr;
	OM_descriptor	 	*desc;
	OM_object			src;
	OM_descriptor	 	*desc_ptra;
	OMP_type_list	 	*pt_la;
	OM_public_object	src_puba;
	OM_boolean			priv_flg;
	OM_boolean			convert;
	OM_sint				nb_val_d;
	OM_sint				nb_res;
	OM_sint				nb_end = 0;
	OM_sint				cp_len;
	OM_sint				ct, cv;
	OM_string			desc_rdn[D2_NO_AVA+1] = { { 0, NULL } };
	OM_string		 	*drdn_ptr;
	unsigned char		*in_ptr;
	OM_boolean			next_ava = OM_FALSE;
	OM_value_position	init_val;
	OM_value_position	limit_val;
	OM_value_position	init_vals;
	OM_value_position	limit_vals;
	OMP_type_list		type_list[]	 = RDN_TYPE_LIST;
	OMP_type_list		put_ava_type_list[]	 = AVA_TYPE_LIST;
	OMP_type_list		put_ava_typ_list[] = AVA_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
	limiting_value);

	TEST_SOURCE_PRIV(NO_EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *)source;

	init_val = initial_value;
	limit_val = limiting_value;
	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		memset(desc_rdn, 0,	D2_NO_AVA * sizeof(OM_string));
		/* get the number of AVAs in the destination object	*/
		if ((ret = dsP_get_nb_rdn_ava(*xds_object, desc_rdn, &nb_val_d,
		D2_AVA_SEP)) != OM_SUCCESS) {
			goto end;
		} /* endif */
		UPDATE_LIMIT_VAL (nb_val_d,	goto end;);
		init_vals = initial_value;
		limit_vals = limiting_value;

		/* copy	AVAs from beginning	to initial_value */
		drdn_ptr = desc_rdn;
		if (initial_value) {
			for	(i = 0,	cp_len = 0,	in_ptr = desc_rdn[i].elements;
			i <	initial_value; cp_len += (desc_rdn[i].length), i++) {
				;
			} /* endfor */
			memcpy (dn_ptr,		in_ptr,		cp_len);
			next_ava = OM_TRUE;
		} /* endif */
		/* skip	replaced values	*/
		drdn_ptr = &desc_rdn[limiting_value];
		nb_res = nb_val_d -	limiting_value;

		ret	 = OM_SUCCESS;

		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			if (OMP_REMOVE) {
				pt_l->nb_val = 0;
			} /* endif */
			/* check if	the	number of AVAs of the RDN after	the
		 * modification	is applied would not exceed	the
		 * max allowed number
		 */
			if ((nb_end	 = initial_value + pt_l->nb_val + nb_res) > D2_NO_AVA) {
				ret	 = OM_WRONG_VALUE_NUMBER;
				break;
			} /* endif */
			if ((OMP_REMOVE) && (nb_end  == 0))	{
				if (*xds_object) {
					OMP_XFREE_STR(workspace, *xds_object,strlen(*xds_object));
					*xds_object	 = 0;
				} /* endif */
				break;
			} /* endif */

			switch (pt_l->type)	 {
			case DS_AVAS :
				for	(i = 0,	desc = (OM_descriptor *)pt_l->desc;
				i <	pt_l->nb_val; i++, desc++) {
					VALIDATE_SYNTAX(desc, OM_S_OBJECT);
					src	 = desc->value.object.object;
					ret	 = dsP_test_source_priv(src, 0, &src_puba, &desc_nb,
					&priv_flg, NO_EXCLUDE_SUB_OBJECTS);
					if (priv_flg) {
						if (ret	 != OM_SUCCESS) {
							break;
						} /* endif */
						if (desc_nb	 ==  0)	{
							/*delete NULL descriptor*/
							om_delete(src_puba);
							break;
						} /* endif */
						src	 = src_puba;
					} /* endif */

					desc_ptra = (OM_descriptor * )src;
					included_types = 0;	/* for the following macro */
					initial_value = 0;
					limiting_value = OM_ALL_VALUES;
					memcpy(put_ava_type_list, put_ava_typ_list,
					sizeof(put_ava_type_list));

					if ((ret = PUB_TYPE_IN_LIST(put_ava_type_list,
					desc_ptra)) == OM_SUCCESS)	 {

					/* an AVA must have	one	ATTRIBUTE TYPE and
					 * one ATTRIBUTE VALUE
					 */
						pt_la = put_ava_type_list;
						pt_la++;
						ct = pt_la->nb_val;
						pt_la++;
						cv = pt_la->nb_val;
						if (ct != cv) {
							ret	 = OM_NOT_PRESENT;
							break;
						} /* endif */
						for (pt_la = put_ava_type_list;
						pt_la->type != OM_NO_MORE_TYPES; pt_la++) {
							if (pt_la->nb_val == 0) {
								continue;
							} /* endif */

							if (((OM_descriptor *)pt_la->desc)->syntax 
							& OM_S_LOCAL_STRING) {
								unsigned char* result; OM_uint32 result_len = 0;
								if ( d27_local_to_t61(
								((OM_descriptor *)pt_la->desc)->value.string.elements,
								((OM_descriptor *)pt_la->desc)->value.string.length,
								(char **)&result, &result_len) != D2_NOERROR) {
									ret = OM_WRONG_VALUE_SYNTAX;
									goto end;
								} else {
									((OM_descriptor *)pt_la->desc)
									-> value.string.elements = result;
									((OM_descriptor *)pt_la->desc)
									-> value.string.length = result_len;
									/* possible memory leak */
								} /* endif */
							} /* endif */

							convert = OM_FALSE;
							switch (pt_la->type) {
							case DS_ATTRIBUTE_TYPE :
								VALIDATE_SYNTAX(pt_la->desc,
								OM_S_OBJECT_IDENTIFIER_STRING);
								convert = OM_TRUE;
							case DS_ATTRIBUTE_VALUES :
								{
								char	sep;

								switch (((OM_descriptor *)pt_la->desc)
								->syntax & OM_S_SYNTAX) {

								case OM_S_TELETEX_STRING:
									sep	 = D2_T61_REP;
									break;
								case OM_S_PRINTABLE_STRING:
									sep	 = D2_PRTBL_REP;
									break;
								case OM_S_IA5_STRING:
									sep	 = D2_IA5_REP;
									break;
								case OM_S_NUMERIC_STRING:
									sep	 = D2_NUM_REP;
									break;
								default:
									sep	 = D2_T61_REP;
									break;
								} /* endswitch */

								ret  = update_dist_name (dn_ptr,
								((OM_descriptor *)pt_la->desc)->value.
								string.length,
								((OM_descriptor*)pt_la->desc)->value.
								string.elements, sep,convert, next_ava);
								next_ava = OM_TRUE;
								break;
								} /* endswitch */
							} /* endswitch */
						} /* endfor */
						if (ret	 != OM_SUCCESS) {
							break;
						} /* endif */
					} /* endif */
					initial_value = init_vals;
					limiting_value = limit_vals;
					if (src	 = src_puba) {
						om_delete (src_puba);
					} /* endif */
				} /* endfor */
				break;
			} /* endswitch */
			if (ret != OM_SUCCESS) {
				break;
			} /* endif */
			if (limiting_value < nb_val_d) {
				/* copy	residual values	*/
				for	(i = limiting_value, cp_len =0,
				in_ptr = desc_rdn[i].elements; i < nb_val_d; cp_len
				+= (desc_rdn[i].length), i++) {
					;
				} /* endfor */
				if (strlen((char *)dn_ptr))	 {
					if ((*in_ptr == D2_RDN_SEP)	 
					|| (*in_ptr == D2_AVA_SEP)) {
						*in_ptr	 = next_ava	? D2_AVA_SEP : D2_RDN_SEP;
					} else {
						dn_ptr[strlen((char	*)dn_ptr)] = next_ava ?
						D2_AVA_SEP : D2_RDN_SEP;
					} /* endif */
				} else if ((*in_ptr == D2_RDN_SEP) || (*in_ptr == D2_AVA_SEP)) {
					in_ptr++;
					cp_len--;
				} /* endif */
				memcpy (&dn_ptr[strlen((char *)dn_ptr)], in_ptr, cp_len	 + 1);
			} /* endif */
		} /* endfor */
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_rdn1 */


/********************************************************/
/*							*/
/*	put	DS DN object				*/
/*							*/
/********************************************************/
OM_return_code
dsP_put_dist_name (
	OMX_workspace		workspace,
	void				**xds_object,
	OM_object			source,
	OMP_object_type		object_type,
	OM_modification		modification,
	OM_type_list		included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
	OM_return_code		ret = OM_SUCCESS;
	OM_sint				i;
	OMP_type_list	 	*pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean			priv_flag;
	OM_descriptor	 	*desc_ptr;
	OM_descriptor	 	*desc;
	xds_dist_name		dn_ptr;
	OM_sint				nb_val_d;
	OM_sint				nb_res;
	OM_sint				nb_end = 0;
	OM_sint				cp_len;
	OM_string			desc_rdn[D2_NP_MAX+1] = { { 0, NULL } };
	OM_string		 	*drdn_ptr;
	unsigned char		*in_ptr;
	OMP_type_list		type_list[]	 = DN_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	while ((ret	 = PUB_TYPE_IN_LIST(type_list, desc_ptr)) ==  OM_SUCCESS) {
		memset(desc_rdn, 0,		D2_NP_MAX *		sizeof(OM_string));
		/* get the number of RDNs in the destination object	*/
		if ((ret = dsP_get_nb_rdn_ava(*xds_object, desc_rdn, &nb_val_d,
		D2_RDN_SEP)) != OM_SUCCESS) {
			break;
		} /* endif */
		UPDATE_LIMIT_VAL (nb_val_d, break);

		/* allocate	MAX	length Distinguished Name string */
		if ((dn_ptr = (xds_dist_name )OMP_XALLOC (workspace, AL_STRING,
		ROUNDUP(workspace, D2_DNL_MAX + 1))) == 0) {
			ret	 = OM_MEMORY_INSUFFICIENT;
			break;
		} /* endif */
		memset(dn_ptr, 0, D2_DNL_MAX + 1);

		/* copy	RDNs from beginning	to initial_value */
		drdn_ptr = desc_rdn;
		if (initial_value) {
			for	(i = 0,	cp_len = 0,	in_ptr = desc_rdn[i].elements;
			i <	initial_value; cp_len += (desc_rdn[i].length), i++) {
				;
			} /* endfor */
			memcpy (dn_ptr,	in_ptr,	cp_len);
		} /* endif */
		/* skip	replaced values	*/
		drdn_ptr = &desc_rdn[limiting_value];
		nb_res = nb_val_d -		limiting_value;

		ret	 = OM_SUCCESS;

		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */
			if (OMP_REMOVE) {
				pt_l->nb_val = 0;
			} /* endif */
			/* check if	the	number of RDNs of the DN after the
		 * modification	is applied would not exceed	the
		 * max allowed number
		 */
			if ((nb_end	 = initial_value + pt_l->nb_val + nb_res) > D2_NP_MAX) {
				ret	 = OM_WRONG_VALUE_NUMBER;
				break;
			} /* endif */
			if ((OMP_REMOVE) && (nb_end == 0)) {
				if (*xds_object) {
					OMP_XFREE_STR(workspace, *xds_object, strlen(*xds_object));
					*xds_object	 = 0;
				} /* endif */
				break;
			} /* endif */

			switch (pt_l->type)	 {
			case DS_RDNS :
				for	(i = 0,	desc = (OM_descriptor *)pt_l->desc;
				i <	pt_l->nb_val; i++, desc++) {
					VALIDATE_SYNTAX(desc, OM_S_OBJECT);
					if (strlen((char *)dn_ptr)) {
						dn_ptr[strlen((char	*)dn_ptr)] = D2_RDN_SEP;
					} /* endif */
					ret	 = dsP_put_rdn1(workspace,
					&(desc_rdn[nb_val_d].elements), desc->value.object.object,
					object_type, modification, 0, 0, OM_ALL_VALUES,
					&dn_ptr[strlen((char *)dn_ptr)]);
				} /* endfor */
				break;
			} /* endswitch */
		} /* endfor */
		if ((ret != OM_SUCCESS) || (nb_end == 0)) {
			OMP_XFREE (workspace, dn_ptr, AL_STRING, ROUNDUP (workspace,
			D2_DNL_MAX	 + 1));
		} else {
			if (limiting_value < nb_val_d) {
				/* copy	residual values	*/
				for	(i = limiting_value, cp_len	 = 0,
				in_ptr = desc_rdn[i].elements; i < nb_val_d;
				cp_len += (desc_rdn[i].length), i++) {
					;
				} /* endfor */
				if (!strlen((char *)dn_ptr)	 &&	 (*in_ptr == D2_RDN_SEP)) {
					in_ptr++;
					cp_len--;
				} else if (strlen((char	*)dn_ptr) && (*in_ptr != D2_RDN_SEP)) {
					dn_ptr[strlen((char	*)dn_ptr)] = D2_RDN_SEP;
				} /* endif */
				memcpy (&dn_ptr[strlen((char *)dn_ptr)], in_ptr, cp_len	 + 1);
			} /* endif */

			if ((*xds_object == 0)
			||  (ROUNDUP(workspace, strlen((char *)dn_ptr) + 1)
			!= ROUNDUP(workspace, strlen	(*xds_object) + 1))) {
				if (*xds_object) {
					OMP_XFREE_STR(workspace, *xds_object, strlen(*xds_object));
				} /* endif */

				if (ROUNDUP(workspace, strlen((char *)dn_ptr) +	1)
				!=  ROUNDUP(workspace, D2_DNL_MAX + 1))	{
					if ((*xds_object = (void *)OMP_XALLOC_STR (workspace, 
					(char *)dn_ptr, strlen((char *)dn_ptr))) == 0) {
						ret = OM_MEMORY_INSUFFICIENT;
						goto end;
					} /* endif */

					OMP_XFREE (workspace, dn_ptr, AL_STRING,
					ROUNDUP (workspace, D2_DNL_MAX	 + 1));
				} else {
					*xds_object = dn_ptr;
				} /* endif */
			} else {
				memcpy (*xds_object, dn_ptr, strlen((char *)dn_ptr) + 1);
				OMP_XFREE (workspace, dn_ptr, AL_STRING,
				ROUNDUP(workspace,	D2_DNL_MAX +  1));
			} /* endif */
		} /* endif */
		if (1) {	/* just to suppress warning on sinix */
			break;
		} /* endif */
	} /* endwhile */
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_dist_name */


/********************************************************/
/*							*/
/*	put	DS RDN object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_put_rdn	(
	OMX_workspace		workspace,
	void 			**xds_object,
	OM_object			source,
	OMP_object_type		object_type,
	OM_modification		modification,
	OM_type_list		included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
	OM_return_code		ret = OM_SUCCESS;
	xds_dist_name		dn_ptr = 0;


	/* allocate	MAX	length Distinguished Name string */
	if ((dn_ptr	 = (xds_dist_name )	OMP_XALLOC (workspace, AL_STRING,
	ROUNDUP(workspace, D2_DNL_MAX + 1))) == 0) {
		ret = OM_MEMORY_INSUFFICIENT;
		goto end;
	} /* endif */
	memset(dn_ptr, 0, D2_DNL_MAX + 1);

	ret	 = dsP_put_rdn1(
					workspace, 
					xds_object, 
					source, 
					object_type,
					modification ,
					included_types,
					initial_value ,
					limiting_value,
					dn_ptr);
	if (ret	 != OM_SUCCESS)	{
		OMP_XFREE (workspace, dn_ptr, AL_STRING, ROUNDUP (workspace,
		D2_DNL_MAX	 + 1));
	} else {
		if ((*xds_object == 0)
		||  (ROUNDUP(workspace, strlen((char *)dn_ptr) +1)
		!= ROUNDUP(workspace, strlen	(*xds_object) + 1)))	 {
			if (*xds_object) {
				OMP_XFREE_STR(workspace, *xds_object, strlen(*xds_object));
			} /* endif */

			if (ROUNDUP(workspace, strlen((char	*)dn_ptr) + 1) 
			!=  ROUNDUP(workspace, D2_DNL_MAX +	1))	 {
				if ((*xds_object = (void *)OMP_XALLOC_STR(workspace,
				(char *)dn_ptr, strlen((char *)dn_ptr))) == 0) {
					ret = OM_MEMORY_INSUFFICIENT;
					goto end;
				} /* endif */
				OMP_XFREE (workspace, dn_ptr, AL_STRING,
					 ROUNDUP (workspace, D2_DNL_MAX	 +	1));
			} else {
				*xds_object	 = dn_ptr;
			} /* endif */
		} else {
			memcpy (*xds_object, dn_ptr, strlen((char *)dn_ptr) +		1);
			OMP_XFREE (workspace, dn_ptr, AL_STRING, ROUNDUP (workspace,
			D2_DNL_MAX	 + 1));
		} /* endif */
	} /* endif */
end:
	return(ret);
} /* end dsP_put_rdn */




/********************************************************/
/*							*/
/*	put	ENTRY INFORMATION object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_put_entry_info(
	OMX_workspace		workspace,
	void 			**xds_object,
	OM_object			source,
	OMP_object_type		object_type,
	OM_modification		modification,
	OM_type_list		included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{

#define	EI_OBJ	((xds_entry_info *)	*xds_object)

	OM_return_code		ret = OM_SUCCESS;
	OM_sint				i;
	OMP_type_list	 *pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean			priv_flag;
	OM_sint				check;
	OM_descriptor	 *desc_ptr;
	OM_descriptor	 *desc;
	void				*link;
	void				**pt_obj;
	void				*pt;
	short				nb_val_d;
	short				n_cur;
	void				**name_obj;

	OMP_type_list		type_list[]	 = E_I_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		ALLOCATE_OBJECT(ENTRY_INFO,	xds_entry_info);
		CHECK_INIT;
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case DS_FROM_ENTRY :
				CHECK_BOOLEAN_AND_UPDATE((OM_descriptor	 *)pt_l->desc,
				EI_OBJ->d2_ei_fromentry);
				break;
			case DS_OBJECT_NAME	:
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				name_obj = (void **) & EI_OBJ->d2_ei_entry;
				REMOVE_OBJ(dist_name, name_obj);
				ret = dsP_put_dist_name(workspace,name_obj,
				((OM_descriptor * )pt_l->desc)->value.object.object,
				object_type, OM_REPLACE_ALL, 0,0, OM_ALL_VALUES);
				break;
			case DS_ATTRIBUTES :
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				GET_NB_OBJ(xds_attribute, d2_a_next, nb_val_d,
				(void *) & EI_OBJ->d2_ei_ai, pt_obj);
				UPDATE_LIMIT_VAL(nb_val_d, break);

				/* jump	values from	the	begining to	initial	value */
				JUMP_N_OBJ(xds_attribute, d2_a_next, initial_value,
				(void *) & EI_OBJ->d2_ei_ai, pt_obj, n_cur);

				/* copy	the	new	objects	*/
				if (OMP_INSERT)	 {
					INSERT_OBJ(ATTRIBUTE, xds_attribute, pt_l->nb_val,
					d2_a_next, dsP_put_attribute);
				} else if (OMP_REPLACE)	 {
					/*delete the objects which have	to be replaced*/
					DELETE_OBJ(xds_attribute, limiting_value - initial_value,
					d2_a_next, dsP_del_attribute);

					/* replace them	by the new ones	*/
					INSERT_OBJ(ATTRIBUTE, xds_attribute, pt_l->nb_val,
					d2_a_next, dsP_put_attribute);
				} /* endif */
				break;
			} /* endswitch */
			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
		CHECK_END;
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_entry_info	*/


/********************************************************/
/*							*/
/*	put	ENTRY INFORMATION SELECTION	 object		*/
/*							*/
/********************************************************/
static OM_return_code
dsP_put_entry_info_sel(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
	OM_return_code	ret = OM_SUCCESS;
	OM_sint	i;
	OMP_type_list	 * pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_sint	check;
	OM_descriptor	 * desc_ptr;
	OM_boolean		old_all, new_all;

#define	EIS_OBJ	((xds_entry_info_sel *)*xds_object)

	OMP_type_list	type_list[]	 = EIS_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		ALLOCATE_OBJECT(ENTRY_INFO_SELECTION, xds_entry_info_sel);
		CHECK_INIT;
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case DS_ALL_ATTRIBUTES :
				CHECK_SYNTAX(pt_l->desc, OM_S_BOOLEAN);
				old_all	 = EIS_OBJ->d2_r_no_at;
				if (((OM_descriptor	 * )pt_l->desc)->value.boolean == OM_TRUE) {
					new_all	 = EIS_OBJ->d2_r_no_at =
						D2_ALL_ATT_REQ;
					if (((old_all != new_all) &&  (new_all == D2_ALL_ATT_REQ)) 
					&&	 (old_all != 0)) {
						for	(i = 0;	i <	 old_all; i++) {
							dsP_delete_type	(workspace,
							&(EIS_OBJ->d2_r_type_at[i]));
						} /* endfor */
						OMP_XFREE (workspace, EIS_OBJ->d2_r_type_at,
						AL_TYPE, old_all);
					} /* endif */
				} else if (old_all	 == D2_ALL_ATT_REQ) {
						EIS_OBJ->d2_r_no_at	 = 0;
				} /* endif */

				break;
			case DS_ATTRIBUTES_SELECTED	:
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT_IDENTIFIER_STRING);
				if (((xds_entry_info_sel *) *xds_object)->d2_r_no_at 
				!= D2_ALL_ATT_REQ) {
					UPDATE_TYPE(EIS_OBJ);
				} /* endif */
				break;
			case DS_INFO_TYPE :
				CHECK_VALID_ENUM_AND_UPDATE((OM_descriptor *)pt_l->desc,
				EIS_OBJ->d2_r_val_at, info_type_limit);
				/*
			CHECK_ENUM_AND_UPDATE((OM_descriptor *)pt_l->desc,
					EIS_OBJ->d2_r_val_at);
*/
				break;
			} /* endswitch */
			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
		CHECK_END;
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_entry_info_sel	*/


/********************************************************/
/*							*/
/*	put	ENTRY MODIFICATION object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_put_entry_mod(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
#define	EM_OBJ	((xds_entry_mod	*) *xds_object)

	OM_return_code	ret = OM_SUCCESS;
	OMP_type_list	 * pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_sint	check;
	OM_descriptor	 * desc_ptr;

	OMP_type_list	type_list[]	 = E_M_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		ALLOCATE_OBJECT(ENTRY_MOD, xds_entry_mod);
		CHECK_INIT;
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case DS_MOD_TYPE :
				CHECK_VALID_ENUM_AND_UPDATE((OM_descriptor *)pt_l->desc,
				EM_OBJ->d2_m_mod, mod_type_limit);
				/*
			CHECK_ENUM_AND_UPDATE((OM_descriptor *)pt_l->desc,
						EM_OBJ->d2_m_mod);
*/
				break;
			case DS_ATTRIBUTE_TYPE :
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT_IDENTIFIER_STRING);
				UPDATE_STRING ((EM_OBJ->d2_m_attr.d2_a_type.d2_typ_len),
				(EM_OBJ ->d2_m_attr.d2_a_type.d2_type),
				((OM_descriptor *)pt_l->desc)->value.string.length,
				((OM_descriptor *)pt_l->desc)->value.string.elements);
				break;

			case DS_ATTRIBUTE_VALUES :
				CHECK_UPDATE_VALUE_ARRAY ( &(EM_OBJ->d2_m_attr.d2_a_no_val),
				(void *) &	(EM_OBJ->d2_m_attr.d2_a_val));
				break;
			} /* endswitch */
			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
		CHECK_END;
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_entry_mod */


/********************************************************/
/*							*/
/*	put	ENTRY MODIFICATION LIST	object		*/
/*							*/
/********************************************************/
static OM_return_code
dsP_put_entry_mod_list(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
	OM_return_code	ret = OM_SUCCESS;
	OM_sint	i;
	OMP_type_list	 * pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_descriptor	 * desc_ptr;
	OM_descriptor	 * desc;

	void	*link;
	void	**pt_obj;
	void	*pt;
	short	nb_val_d;
	short	n_cur;

	OMP_type_list	type_list[]	 = EML_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case DS_CHANGES	:
				VALIDATE_SYNTAX(pt_l->desc,		OM_S_OBJECT);

				/* if necessary,
			 * update initial and limiting values with
			 * the existing	number of values
			 */
				GET_NB_OBJ(xds_entry_mod, d2_m_next, nb_val_d,
					 xds_object, pt_obj);
				UPDATE_LIMIT_VAL(nb_val_d, break);

				/* jump	values from	the	begining to	initial	value */
				JUMP_N_OBJ(xds_entry_mod, d2_m_next, initial_value,
						xds_object,		pt_obj,	n_cur);

				/* copy	the	new	objects	*/
				if (OMP_INSERT)	 {
					INSERT_OBJ(ENTRY_MOD, xds_entry_mod,
						 pt_l->nb_val, d2_m_next, dsP_put_entry_mod);
				} else if (OMP_REPLACE)	 {
					/*delete the objects which have	to be replaced*/
					DELETE_OBJ(xds_entry_mod, limiting_value	 -
						initial_value, d2_m_next,	dsP_del_entry_mod);

					/* replace them	by the new ones	*/
					INSERT_OBJ(ENTRY_MOD, xds_entry_mod,
						 pt_l->nb_val, d2_m_next, dsP_put_entry_mod);
				} /* endif */
				break;

			} /* endswitch */
			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_entry_mod_list	*/


/********************************************************/
/*							*/
/*	put	EXTENSION object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_put_extension(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
#define	EXT_OBJ	((xds_extension	*) *xds_object)
	OM_return_code	ret = OM_SUCCESS;
	OMP_type_list	 * pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_sint	check;
	OM_descriptor	 * desc_ptr;

	OMP_type_list	type_list[]	 = EXT_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		ALLOCATE_OBJECT(EXT, xds_extension);
		CHECK_INIT;
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case DS_CRIT :
				CHECK_BOOLEAN_AND_UPDATE((OM_descriptor	 *)pt_l->desc,
				EXT_OBJ->d2_critical);
				break;

			case DS_IDENT :
				CHECK_INTEGER_AND_UPDATE((OM_descriptor	 *)pt_l->desc,
				EXT_OBJ->d2_ext_id);
				break;

			case DS_ITEM_PARAMETERS	:
				/* not defined today */
				break;

			} /* endswitch */
			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
		CHECK_END;
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_extension */


/********************************************************/
/*							*/
/*	put	FILTER ITEM	object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_put_filter_item(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
#define	FILTER_ITEM_OBJ	((xds_fi_item *) *xds_object)
	OM_return_code	ret = OM_SUCCESS;
	OMP_type_list	 * pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_sint	check;
	OM_descriptor	 * desc_ptr;

	D2_a_value	 * val_ptr;
	OM_string_length	str_len;
	void	*str_elem;

	OMP_type_list	type_list[]	 = F_I_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		ALLOCATE_OBJECT(FILTER_ITEM, xds_fi_item);
		CHECK_INIT;
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case DS_FILTER_ITEM_TYPE :
				CHECK_VALID_ENUM_AND_UPDATE((OM_descriptor *)pt_l->desc,
				FILTER_ITEM_OBJ->match_type, filter_item_type_limit);
				break;
			case DS_FINAL_SUBSTRING	:
				if (check) {
					if (OMP_REMOVE) {
						break;
					} /* endif */
					VALIDATE_LENGTH(pt_l->desc,	MIN_VL_FINAL_SUBSTRING,
					MAX_LENGTH);
					break;
				} /* endif */
				val_ptr	 = &(FILTER_ITEM_OBJ->f_lst_sstr);
				PUB_CP_VAL(val_ptr,	1);
				break;
			case DS_INITIAL_SUBSTRING :
				if (check) {
					if (OMP_REMOVE) {
						break;
					} /* endif */
					VALIDATE_LENGTH(pt_l->desc,	MIN_VL_INITIAL_SUBSTRING,
						 MAX_LENGTH);
#if 0
				str_len = ((OM_descriptor *)pt_l->desc)->value.string.length;
				str_elem = ((OM_descriptor *)pt_l->desc)->value.string.elements;
				if ((str_len == 0) ||
				(str_len == OM_LENGTH_UNSPECIFIED &&
				 (str_elem == 0	|| strlen(str_elem) == 0))) {
					ret = OM_WRONG_VALUE_LENGTH;
					break;
				} /* endif */
#endif /* 0 */
					break;
				} /* endif */
				val_ptr	 = &(FILTER_ITEM_OBJ->f_ini_sstr);
				PUB_CP_VAL(val_ptr,	1);
				break;
			case DS_ATTRIBUTE_TYPE :
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT_IDENTIFIER_STRING);
				UPDATE_STRING ((FILTER_ITEM_OBJ->f_item_att.d2_a_type.d2_typ_len),
				(FILTER_ITEM_OBJ ->f_item_att.d2_a_type.d2_type),
				((OM_descriptor *)pt_l->desc)->value.string.length,
				((OM_descriptor *)pt_l->desc)->value.string.elements);
				break;

			case DS_ATTRIBUTE_VALUES :
				CHECK_UPDATE_VALUE_ARRAY ( &(FILTER_ITEM_OBJ->f_item_att.d2_a_no_val),
				(void *) &(FILTER_ITEM_OBJ->f_item_att.d2_a_val));
				break;
			} /* endswitch */
			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
		CHECK_END;
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_filter_item */


/********************************************************/
/*							*/
/*	put	FILTER object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_put_filter(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
#define	FILTER_OBJ	((xds_filter *)	*xds_object)
	OM_return_code	ret = OM_SUCCESS;
	OM_sint	i;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_sint	check;
	OM_descriptor	 * desc_ptr;
	OMP_type_list	 * pt_l;
	OM_descriptor	 * desc;

	void	*link;
	void	**pt_obj;
	void	*pt;
	short	nb_val_d;
	short	n_cur;
	OM_value_position	init_val;
	OM_value_position	limit_val;

	OMP_type_list	type_list[]	 = F_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		ALLOCATE_OBJECT(FILTER,		xds_filter);
		init_val = initial_value;
		limit_val = limiting_value;
		CHECK_INIT;
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */
			initial_value = init_val;
			limiting_value = limit_val;

			switch (pt_l->type)	 {
			case DS_FILTERS	:
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				GET_NB_OBJ(xds_filter, filter_next,	nb_val_d,
				(void *) & FILTER_OBJ->filter_first, pt_obj);
				UPDATE_LIMIT_VAL(nb_val_d, break);

				/* jump	values from	the	begining to	initial	value */
				JUMP_N_OBJ(xds_filter, filter_next,initial_value,
				(void *) & FILTER_OBJ->filter_first, pt_obj, n_cur);

				/* copy	the	new	objects	*/
				if (OMP_INSERT)	 {
					INSERT_OBJ(FILTER, xds_filter, pt_l->nb_val,
					filter_next, dsP_put_filter);
				} else if (OMP_REPLACE)	 {
					/*delete the objects which have	to be replaced*/
					DELETE_OBJ(xds_filter, limiting_value - initial_value,
					filter_next, dsP_del_a_filter);

					/* replace them	by the new ones	*/
					INSERT_OBJ(FILTER, xds_filter, pt_l->nb_val,
					filter_next, dsP_put_filter);
				} /* endif */
				break;

			case DS_FILTER_ITEMS :
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				GET_NB_OBJ(xds_fi_item,	f_item_next, nb_val_d,
				(void *)& FILTER_OBJ->fil_item, pt_obj);
				UPDATE_LIMIT_VAL(nb_val_d, break);

				/* jump	values from	the	begining to	initial	value */
				JUMP_N_OBJ(xds_fi_item,	f_item_next, initial_value,
				(void *) & FILTER_OBJ->fil_item, pt_obj, n_cur);

				/* copy	the	new	objects	*/
				if (OMP_INSERT)	 {
					INSERT_OBJ(FILTER_ITEM,	xds_fi_item,
					pt_l->nb_val, f_item_next,	dsP_put_filter_item);
				} else if (OMP_REPLACE)	 {
					/*delete the objects which have	to be replaced*/
					DELETE_OBJ(xds_fi_item,	limiting_value - initial_value,
					f_item_next, dsP_del_fi_item);

					/* replace them	by the new ones	*/
					INSERT_OBJ(FILTER_ITEM,	xds_fi_item, pt_l->nb_val,
					f_item_next, dsP_put_filter_item);
				} /* endif */
				break;

			case DS_FILTER_TYPE	:
				CHECK_VALID_ENUM_AND_UPDATE((OM_descriptor *)pt_l->desc,
				FILTER_OBJ->fi_type, filter_type_limit);
				break;
			} /* endswitch */
			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
		CHECK_END;
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_filter	*/


/********************************************************/
/*							*/
/*	put	PRESENTATION ADDRESS object			*/
/*							*/
/********************************************************/
OM_return_code
dsP_put_psap (
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
#define	PSAP_OBJ	((xds_psap) *xds_object)

	OM_return_code	ret = OM_SUCCESS;
	OM_sint	i;
	OMP_type_list	 * pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_sint			check;
	OM_descriptor	 * desc_ptr;
	OM_descriptor	 * src_ptr;

	xds_psap		 * psap_ptr;
	Psap_info		 * src;
	Psap_info		 * nsap_ptr;
	Psap_info		 * dest;
	Psap_info		p_sel;
	Psap_info		s_sel;
	Psap_info		t_sel;
	Psap_info		nsap_info[T_MAXNSAP];
	OM_sint	n_ins;
	OM_sint	n_res;
	OM_sint	n_rep;
	OM_sint	no_nsap;
	OM_sint	new_no_nsap;
	OM_sint	psap_len;
	OM_sint	new_psap_len;

    /* vars for local-string conversion */
    char      no_sel[] = "";
#define MAXNSAPSIZE  sizeof (struct t_adrdesc) + T_ALIGN4(T_NSAPSIZE)
    unsigned char  psapstr[T_MAXNSAP][MAXNSAPSIZE];   /* 8x28 */
	Psap_info empty_pi;
    OM_sint       nsap_num;
    OM_sint       res;


#define	CLEAR_PSAP_INFO(PSAP_INFO)				\
	PSAP_INFO.psap_infolng = 0;					\
	PSAP_INFO.psap_infoval = (char *)0;

	OMP_type_list	type_list[]	 = PSAP_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	while ((ret	 = PUB_TYPE_IN_LIST(type_list, desc_ptr)) ==  OM_SUCCESS)	 {
		/* clear nsap_info array */
		memset(nsap_info, 0, sizeof(Psap_info) * T_MAXNSAP);
		if ((psap_len = dsP_get_psap_length	(PSAP_OBJ))	 ==  0)	{
			no_nsap	 = 0;
			CLEAR_PSAP_INFO(p_sel);
			CLEAR_PSAP_INFO(s_sel);
			CLEAR_PSAP_INFO(t_sel);
		} /* decode	psap address and get the number	of nsap	in the object */ 
		else if (dsP_decode_psap_addr (&p_sel, &s_sel, &t_sel, &no_nsap,
		nsap_info, PSAP_OBJ) < 0) {
				ret	 = OM_NO_SUCH_OBJECT;
				break;
		} /* endif */
		UPDATE_LIMIT_VAL (no_nsap, break);

		/* initialise new nsap number with the old one */
		new_no_nsap	 = no_nsap;

		ret	 = OM_SUCCESS;
		CHECK_INIT;
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case DS_N_ADDRESSES	:
				CHECK_SYNTAX(pt_l->desc, OM_S_OCTET_STRING);

				if (OMP_REMOVE) {
					pt_l->nb_val = 0;
				} /* endif */
				n_ins = pt_l->nb_val;
				n_rep = limiting_value - initial_value;
				n_res = no_nsap	 - limiting_value;
				new_no_nsap	 = initial_value	 + n_ins	 + n_res;
				if (new_no_nsap	 > T_MAXNSAP) {
					ret	 = OM_WRONG_VALUE_NUMBER;
					break;
				} /* endif */
				if (new_no_nsap != no_nsap) {
					if (n_res) {
						if (n_rep >	n_ins) {
							memcpy (&nsap_info [initial_value+n_ins],
								    &nsap_info[limiting_value], 
                                    n_res * sizeof(Psap_info));
						} else {
							for	(dest = &nsap_info[new_no_nsap - 1],
							 src = &nsap_info[no_nsap - 1];
							 (n_res > 0);
                             dest--, src--,	n_res--) {
                                memcpy(dest, src, sizeof(Psap_info));
							} /* endfor */
						} /* endif */
					} /* endif */
				} /* endif */

				/* update Psap_info	structures with	new	nsap info */
				for	(i = 0,	src_ptr = (OM_descriptor *)pt_l->desc,
                nsap_ptr = &nsap_info[initial_value];
				(i < pt_l->nb_val); 
                i++, src_ptr++, nsap_ptr++) {

                /*
                 * if OM_S_LOCAL_STRING is set then NSAP contains macros
                 * convert NSAP to hex using nsapmacros file
                 */
                 if (src_ptr->syntax & OM_S_LOCAL_STRING) {

                     /* expand the macros, build a PSAP */

                     if (src_ptr->value.string.length == OM_LENGTH_UNSPECIFIED){

                         if (d2p_p01_str_to_bin_psap(no_sel, no_sel, no_sel, 1,
				                              (char **)&(src_ptr->value.string.elements),
                                              psapstr[i]) < 0) {
					         ret = OM_WRONG_VALUE_MAKEUP;
					         break;
                         } /* endif */
                     } else {
                        char *tmp;

                        if ((tmp = OMP_XALLOC_STR(
						workspace, src_ptr->value.string.elements,
						src_ptr->value.string.length)) == 0) {
                             ret =  OM_MEMORY_INSUFFICIENT;
							 goto end;
						} /* endif */

                         if (d2p_p01_str_to_bin_psap(no_sel, no_sel, no_sel, 1,
				                                    &tmp, psapstr[i]) < 0) {
                             OMP_XFREE_STR(workspace, tmp,
                                           src_ptr->value.string.length +1);
					         ret = OM_WRONG_VALUE_MAKEUP;
					         break;
                         } /* endif */
                         OMP_XFREE_STR(workspace, tmp,
                                       src_ptr->value.string.length +1);
                     } /* endif */

			         CLEAR_PSAP_INFO(empty_pi);

                     /* locate the NSAP */
                     res = dsP_decode_psap_addr(&empty_pi, &empty_pi, &empty_pi,
                                              &nsap_num, &empty_pi, psapstr[i]);
                     if (res < 0 || (nsap_num != 1)) {
					     ret = OM_WRONG_VALUE_MAKEUP;
			             break;
                     } /* endif */
					nsap_ptr->psap_infolng = empty_pi.psap_infolng;
					nsap_ptr->psap_infoval = empty_pi.psap_infoval;
                 } else {
					nsap_ptr->psap_infolng = src_ptr->value.string.length;
					nsap_ptr->psap_infoval = src_ptr->value.string.elements;
                 } /* endif */
			} /* endfor */

				break;
			case DS_P_SELECTOR :
				CHECK_SYNTAX(pt_l->desc, OM_S_OCTET_STRING);
				p_sel.psap_infolng = ((OM_descriptor *)pt_l->desc)
				->value.string.length;
				p_sel.psap_infoval = ((OM_descriptor *)pt_l->desc)
				->value.string.elements;
				break;
			case DS_S_SELECTOR :
				CHECK_SYNTAX(pt_l->desc, OM_S_OCTET_STRING);
				s_sel.psap_infolng = ((OM_descriptor *)pt_l->desc)
				->value.string.length;
				s_sel.psap_infoval = ((OM_descriptor *)pt_l->desc)
				->value.string.elements;
				break;
			case DS_T_SELECTOR :
				CHECK_SYNTAX(pt_l->desc, OM_S_OCTET_STRING);
				t_sel.psap_infolng = ((OM_descriptor *)pt_l->desc)
				->value.string.length;
				t_sel.psap_infoval = ((OM_descriptor *)pt_l->desc)
				->value.string.elements;
				break;
			} /* endswitch */
			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
		CHECK_END;
		if ((ret != OM_SUCCESS) ||	(ret = dsP_encode_psap_addr	(workspace,
			 &p_sel, &s_sel, &t_sel, new_no_nsap,  nsap_info,
			(xds_psap *)&psap_ptr,	&new_psap_len))	 !=  OM_SUCCESS)
			break;
		if (*xds_object) {
			/* delete the old psap string	*/
			OMP_XFREE_STR(workspace, *xds_object, psap_len);
		} /* endif */
		*xds_object	 = psap_ptr;
		if (1) {	/* just to suppress warning on sinix */
			break;
		} /* endif */
	} /* endwhile */
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_psap */


/********************************************************/
/*							*/
/*	put	SESSION	object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_put_session(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
#define	SES_OBJ	((xds_session *) *xds_object)
	OM_return_code	ret = OM_SUCCESS;
	OMP_type_list	 * pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_sint	check;
	OM_descriptor	 * desc_ptr;

	OMP_type_list	 * type_list;
	void	**name_obj;
	OM_sint	len;
	OM_boolean	empty_obj = OM_FALSE;

	OMP_type_list	put_ses_type_list[]	 = SES_TYPE_LIST;

	OMP_type_list	put_sgs_type_list[]	 = SGS_TYPE_LIST;

	switch (OMP_PACKAGE(object_type)) {
	case XDS_SERVICE_PACKAGE:
		type_list = put_ses_type_list;
		break;

	case XDS_GDS_PACKAGE:
		type_list = put_sgs_type_list;
		break;
	} /* endswitch */


	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		if (!*xds_object) {
			empty_obj = OM_TRUE;
		} /* endif */
		ALLOCATE_OBJECT(SESSION, xds_session);
		if (empty_obj) {
			SES_OBJ->dir_id	 = D2MINDIR_ID;
		} /* endif */
		CHECK_INIT;
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case DS_DSA_ADDRESS	:
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				name_obj = (void **) & SES_OBJ->dsa_address;
				REMOVE_OBJ(psap, name_obj);
				ret = dsP_put_psap(workspace,name_obj,
				((OM_descriptor *)pt_l->desc)->value.object.object,
				object_type, OM_REPLACE_ALL, 0,0, OM_ALL_VALUES);
				break;
			case DS_DSA_NAME :
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				name_obj = (void **) & SES_OBJ->dsa_name;
				REMOVE_OBJ(dist_name, name_obj);
				ret = dsP_put_dist_name(workspace,name_obj,
				((OM_descriptor * )pt_l->desc)->value.object.object,
				object_type, OM_REPLACE_ALL, 0,0, OM_ALL_VALUES);
				break;
			case DS_FILE_DESCRIPTOR	:
				CHECK_SYNTAX(pt_l->desc, OM_S_INTEGER);
				break;
			case DS_REQUESTOR :
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				name_obj = (void **) & SES_OBJ->requestor;
				REMOVE_OBJ(dist_name, name_obj);
				ret = dsP_put_dist_name(workspace,name_obj,
				((OM_descriptor * )pt_l->desc)->value.object.object,
				object_type, OM_REPLACE_ALL, 0,0, OM_ALL_VALUES);
				break;
			case DSX_PASSWORD :
				CHECK_SYNTAX(pt_l->desc, OM_S_OCTET_STRING);
				len	 = SES_OBJ->password.d2_p_len;
				UPDATE_STRING (len,	(SES_OBJ->password.d2_p_pw),
				((OM_descriptor *)pt_l->desc)->value.string.length,
				((OM_descriptor *)pt_l->desc)->value.string.elements);
				if (ret	 == OM_SUCCESS) {
					SES_OBJ->password.d2_p_len =  len;
				} /* endif */
				break;
			case DSX_DIR_ID	:
				CHECK_INTEGER_AND_UPDATE((OM_descriptor *)pt_l->desc,
				SES_OBJ->dir_id);
				break;
			case DSX_AUTH_MECHANISM	:
				CHECK_VALID_ENUM_AND_UPDATE((OM_descriptor *)pt_l->desc,
				SES_OBJ->auth_mech, auth_mech_type_limit);
				break;
			case DSX_AUTH_INFO	:
				CHECK_SYNTAX(pt_l->desc, OM_S_OCTET_STRING);
				len	 = SES_OBJ->auth_info.d2_size;
				UPDATE_STRING (len,	(SES_OBJ->auth_info.d2_value),
				((OM_descriptor *)pt_l->desc)->value.string.length,
				((OM_descriptor *)pt_l->desc)->value.string.elements);
				if (ret	 == OM_SUCCESS) {
					SES_OBJ->auth_info.d2_size =  len;
				} /* endif */
				break;
			} /* endswitch */
			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
		CHECK_END;
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_session */


/********************************************************/
/*							*/
/*	put	FAX	NUMBER object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_put_faxnumber(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
#define	FAX_OBJ	((xds_faxnumber	*) *xds_object)

	#define	UPDATE_FAX_PAR(DESC,BIT_VAL)					\
{									\
	if (OMP_SYNTAX(DESC) != OM_S_BOOLEAN)				\
	ret = OM_NO_SUCH_SYNTAX;					\
	else								\
	{									\
	if ((DESC)->value.boolean == OM_TRUE)				\
		((xds_faxnumber	*)*xds_object)->d2_par |= BIT_VAL;		\
	else								\
		((xds_faxnumber	*)*xds_object)->d2_par &= ~BIT_VAL;		\
	FAX_OBJ->d2_par_defined = TRUE;				\
	}									\
} 

	OM_return_code	ret = OM_SUCCESS;
	OMP_type_list	 * pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_sint	check;
	OM_descriptor	 * desc_ptr;
	OM_descriptor	 * src_pt;

	OM_descriptor	 * desc;
	OM_sint	len;
	OM_object		src;
	OM_public_object	src_puba;
	OMP_object_type	src_type;
	OM_string_length	str_len;
	void	*str_elem;

	OMP_type_list	type_list[]	 = FAX_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		ALLOCATE_OBJECT(FACSIMILE_PHONE_NBR, xds_faxnumber);
		CHECK_INIT;
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case DS_PARAMETERS:
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				ret	 = OM_SUCCESS;
				if (OMP_REMOVE)	 {
					FAX_OBJ->d2_par_defined	= FALSE;
					FAX_OBJ->d2_par	 = 0;
					break;
				} /* endif */
				desc = (OM_descriptor *	)pt_l->desc;
				src	 = desc->value.object.object;
				if (src	 == 0) {
					ret	 = OM_WRONG_VALUE_MAKEUP;
					break;
				} /* endif */
				src_puba = (OM_public_object )0;
				if (OMP_PRIVATE(src)) {
					/* check valid handle */
					if (!OMP_VALID_OBJECT(src))	 {
						ret	 = OM_NO_SUCH_OBJECT;
						break;
					} /* endif */

					if ((ret = om_get(src, OM_NO_EXCLUSIONS,
						 0,	0, 0,	0, &src_puba, &desc_nb)) != OM_SUCCESS) {
						break;
					} /* endif */
					if (desc_nb	 == 0) {
						/*delete NULL descriptor*/
						om_delete(src_puba);
						ret	 = OM_NO_SUCH_OBJECT;
						break;
					} /* endif */
					src	 = src_puba;
				} /* endif */
				/* test	OM_CLASS of	the	sub_object */
				if (OMP_TYPE(src) != OM_CLASS) {
					ret	 = OM_NO_SUCH_OBJECT;
				} /* endif */

				/* src public object is	in the current workspace */
				if (ret	 != OM_SUCCESS)	{
					if (src	 == src_puba) {
						om_delete (src_puba);
					} /* endif */
					break;
				} /* endif */
				src_pt = src;
				src_pt++;

				if (((ret = omP_get_object_type(workspace, src->value.string,
				&src_type))	 ==  OM_SUCCESS)
				&&(OMP_PACKAGE(src_type) == XMH_PACKAGE) 
				&& (OMP_CLASS(src_type) == XMH_G3_FAX_NBPS)) {
					for	(; src_pt->type != OM_NO_MORE_TYPES; src_pt++) {
						switch (OMP_TYPE(src_pt)) {
						case MH_T_A3_WIDTH:
							UPDATE_FAX_PAR(src_pt,
								 D2_A3_WIDTH);
							break;
						case MH_T_B4_LENGTH:
							UPDATE_FAX_PAR(src_pt,
								 D2_B4_LENGTH);
							break;
						case MH_T_B4_WIDTH:
							UPDATE_FAX_PAR(src_pt,
								 D2_B4_WIDTH);
							break;
						case MH_T_FINE_RESOLUTION:
							UPDATE_FAX_PAR(src_pt,
								 D2_FINE_RESOLUTION);
							break;
						case MH_T_TWO_DIMENSIONAL:
							UPDATE_FAX_PAR(src_pt,
								 D2_TWO_DIMENSIONAL);
							break;
						case MH_T_UNCOMPRESSED:
							UPDATE_FAX_PAR(src_pt,
								 D2_UNCOMPRESSED);
							break;
						case MH_T_UNLIMITED_LENGTH:
							UPDATE_FAX_PAR(src_pt,
								 D2_UNLIMITED_LENGTH);
							break;
						default:
							ret	 = OM_NO_SUCH_SYNTAX;
						} /* endswitch */
						if (ret	 != OM_SUCCESS) {
							break;
						} /* endif */
					} /* endif */
				} else {
					ret	 = OM_WRONG_VALUE_TYPE;
				} /* endif */	

				if (src	 == src_puba) {
					om_delete (src_puba);
				} /* endif */
				break;

			case DS_PHONE_NBR:
				CHECK_SYNTAX_AND_LENGTH(pt_l->desc,	OM_S_PRINTABLE_STRING,
				MIN_VL_PHONE_NBR, DS_VL_PHONE_NBR);
				len	 = FAX_OBJ->d2_tel_nr.d2_size;
				UPDATE_STRING (len,(FAX_OBJ->d2_tel_nr.d2_value),
				((OM_descriptor * )pt_l->desc)->value.string.length,
				((OM_descriptor *)pt_l->desc)->value.string.elements);
				if (ret	 == OM_SUCCESS) {
					FAX_OBJ->d2_tel_nr.d2_size =  len;
				} /* endif */
				break;

			} /* endswitch */
			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
		CHECK_END;
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_faxnumber */


/********************************************************/
/*							*/
/*	put	POSTAL ADDRESS object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_put_postal_address(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
	OM_return_code	ret = OM_SUCCESS;
	OM_sint	i;
	OMP_type_list	 * pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_sint	check;
	OM_descriptor	 * desc_ptr;
	void	*link;
	void	**pt_obj;
	short	nb_val_d;
	short	n_cur;
	OM_sint	nb_res,		nb_end;

	void	**name_obj;
	OM_descriptor	 * desc_pta;
	OM_sint	j;
	OM_string_length	str_len;
	void	*str_elem;

	OMP_type_list	type_list[]	 = POST_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		/* allocated by	dsP_put_str_list()
		ALLOCATE_OBJECT(POSTAL_ADDRESS,xds_postal_address);	*/
		CHECK_INIT;
		name_obj = xds_object;

		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case DS_POSTAL_ADDRESS:
				for	(j = 0,	desc_pta = pt_l->desc; j < pt_l->nb_val;
				j++, desc_pta++) {
					CHECK_TWO_SYNTAXES_AND_LENGTH(desc_pta, OM_S_TELETEX_STRING,
					OM_S_PRINTABLE_STRING, MIN_VL_POSTAL_ADDRESS,
					pt_l->max_length);
				} /* endfor */
				if (check) {
					break;
				} /* endif */

				/* if necessary,
			 * update initial and limiting values with
			 * the existing	number of values
			 */
				GET_NB_OBJ(xds_str_list, d2_next, nb_val_d, name_obj, pt_obj);
				UPDATE_LIMIT_VAL(nb_val_d, break);
				nb_res = nb_val_d -	limiting_value;
				if (OMP_REMOVE) {
					pt_l->nb_val = 0;
				} /* endif */
				/* check if	the	number of values of	the	attribute
			 * after modification is applied would not exceed the
			 * max allowed number
			 */
				if ((nb_end	 = initial_value	 + pt_l->nb_val	+
					nb_res)	 > pt_l->max_val) {
					ret	 = OM_WRONG_VALUE_NUMBER;
					break;
				} /* endif */

				/* jump	values from	the	begining to	initial	value */
				JUMP_N_OBJ(xds_str_list, d2_next, initial_value, name_obj,
				pt_obj, n_cur);

				/* copy	the	new	objects	*/
				if (OMP_INSERT)	 {
					ret	 = dsP_put_str_list(workspace, xds_object,
						 modification, pt_l, pt_obj);
				} else if (OMP_REPLACE)	 {
					/*delete the objects which have	to be replaced*/
					DELETE_OBJ(xds_str_list, limiting_value	 -
						initial_value, d2_next,	dsP_del_a_str);

					/* replace them	by the new ones	*/
					ret	 = dsP_put_str_list(workspace, xds_object,
						 modification, pt_l, pt_obj);
				} /* endif */
				break;
			} /* endswitch */
			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
		CHECK_END;
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_postal_address	*/


/********************************************************/
/*							*/
/*	put	SEARCH CRITERION object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_put_search_criterion(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
#define	SRC_OBJ	((xds_search_criterion *) *xds_object)
	OM_return_code	ret = OM_SUCCESS;
	OM_sint	i;
	OMP_type_list	 * pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_sint	check;
	OM_descriptor	 * desc_ptr;

	OM_sint	new_f_typ;
	OM_sint	filter_type;
	D2_criteria		 * crit_pt;
	D2_crit_item	 * type;
	OM_sint	not_nb = 1;
	OM_uint	*crit_nb;
	short	nb_crit;

	OMP_type_list	type_list[]	 = SR_CRIT_TYPE_LIST;

	crit_nb	 = (OM_uint *)&not_nb;
	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		ALLOCATE_OBJECT(SEARCH_CRITERION, xds_search_criterion);
		CHECK_INIT;
		filter_type	 = SRC_OBJ->d2_c_mode;
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			if (!check) {
				filter_type	 = SRC_OBJ->d2_c_mode;
			} /* endif */
			switch (pt_l->type)	 {
			case DS_FILTER_TYPE	:
				if (check) {
					VALIDATE_SYNTAX(pt_l->desc,	OM_S_ENUMERATION);
					if (OMP_REMOVE)	 {
						ret	 = OM_WRONG_VALUE_NUMBER;
						break;
					} /* endif */
					VALIDATE_ENUM(pt_l->desc, filter_type_limit);
					filter_type	 = ((OM_descriptor *
						)pt_l->desc)->value.enumeration;
					break;
				} /* endif */
				UPDATE_ENUM((OM_descriptor * )pt_l->desc,
					 SRC_OBJ->d2_c_mode);
				new_f_typ = SRC_OBJ->d2_c_mode;
				if (new_f_typ == filter_type) {
					break;
				} /* endif */
				crit_pt	 = (D2_criteria	* )0;
				if (filter_type	 == D2_ITEM)	 {
					if ((type = SRC_OBJ->d2_crit_val.type) !=
						0) {
						dsP_delete_type(workspace, &(type->d2_ci_type));
						OMP_XFREE(workspace, type,
							 AL_CRIT_ITEM, 1);
						SRC_OBJ->d2_crit_val.type =
							0;
					} /* endif */
				} else if (((new_f_typ == D2_ITEM) ||  (new_f_typ !=
					D2_NOT)) &&	(filter_type ==  D2_NOT)) {
					if ((crit_pt = SRC_OBJ->d2_crit_val.not) != 0) {
						dsP_delete_criteria(workspace, crit_pt);
						OMP_XFREE(workspace, crit_pt,
						AL_XDS_SEARCH_CRITERION, 1);
						SRC_OBJ->d2_crit_val.not = 0;
					} /* endif */
				} else if (((new_f_typ == D2_ITEM) ||  (new_f_typ ==
					D2_NOT)) &&	(filter_type !=  D2_NOT)
					 &&		(SRC_OBJ->d2_crit_val.d2_ci_and_or)) {
					crit_nb	 = (OM_uint *)&(SRC_OBJ->d2_crit_val.d2_ci_and_or->and_or_nr);
					crit_pt	 = SRC_OBJ->d2_crit_val.d2_ci_and_or->and_or;
					if (*crit_nb) {
						for	(i = 0;	i <	 *crit_nb; i++) {
							dsP_delete_criteria(workspace, &(crit_pt[i]));
						} /* endfor */
						OMP_XFREE(workspace, crit_pt,
							 AL_XDS_SEARCH_CRITERION,
							 *crit_nb);
					} /* endif */
					OMP_XFREE(workspace, SRC_OBJ->d2_crit_val.d2_ci_and_or,
						 AL_CRIT_SET, 1);
					SRC_OBJ->d2_crit_val.d2_ci_and_or =
						0;
				} /* endif */

				break;
			case DS_ATTRIBUTE_TYPE :
			case DS_FILTER_ITEM_TYPE :
				if (check) {
					if ((filter_type != D2_ITEM) ||
						((filter_type == D2_ITEM) &&  (OMP_REMOVE))) {
						ret	 = OM_WRONG_VALUE_NUMBER;
						break;
					} /* endif */
				} /* endif */
				if (pt_l->type == DS_ATTRIBUTE_TYPE	) {
					CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT_IDENTIFIER_STRING);
				} else {
					CHECK_VALID_ENUM(pt_l->desc, filter_item_type_limit);
				} /* endif */
				if (!SRC_OBJ->d2_crit_val.type)	 {
					if ((SRC_OBJ->d2_crit_val.type =
						(D2_crit_item *		)OMP_XALLOC(workspace,
						 AL_CRIT_ITEM,
						1))	 == (void *)0) {
						ret	 = OM_MEMORY_INSUFFICIENT;
						break;
					} /* endif */
					/* clear xds structure */
					memset (SRC_OBJ->d2_crit_val.type,
						 0,		 sizeof(D2_crit_item));
				} /* endif */
				if (pt_l->type == DS_ATTRIBUTE_TYPE	) {
					UPDATE_STRING (
					((SRC_OBJ->d2_crit_val.type)->d2_ci_type.d2_typ_len),
					((SRC_OBJ->d2_crit_val.type)-> d2_ci_type.d2_type),
					((OM_descriptor *)pt_l->desc)->value.string.length,
					((OM_descriptor *)pt_l->desc)->value.string.elements);
				} else {
					UPDATE_ENUM((OM_descriptor *  )pt_l->desc,
						 (SRC_OBJ->d2_crit_val.type)->d2_ci_mode);
				} /* endif */
				break;

			case DS_CRITERIA:
				if (check) {
					VALIDATE_SYNTAX(pt_l->desc,	OM_S_OBJECT);
					if (filter_type	 == D2_ITEM)	 {
						ret	 = OM_WRONG_VALUE_NUMBER;
						break;
					} /* endif */
				} /* endif */
				if (filter_type	 == D2_NOT)	{
					if (check) {
						if (!OMP_REPLACE_SINGLE_VALUE) {
							ret	 = OM_WRONG_VALUE_NUMBER;
							break;
						} /* endif */
					} else {
						if (!(crit_pt =  SRC_OBJ->d2_crit_val.not))	{
							if ((crit_pt = SRC_OBJ->d2_crit_val.not =
							(D2_criteria *)OMP_XALLOC(workspace,
							AL_XDS_SEARCH_CRITERION, 1)) == 0) {
								ret	 = OM_MEMORY_INSUFFICIENT;
								break;
							} /* endif */
						} else {
							dsP_delete_criteria(workspace,
								 crit_pt);
						} /* endif */
						memset(crit_pt,	0,	sizeof(D2_criteria));
						PUB_CP_CRIT(crit_pt, 1);
					} /* endif */
				} else {
					if (!check)	 {
						if (!SRC_OBJ->d2_crit_val.d2_ci_and_or)	 {
							if ((SRC_OBJ->d2_crit_val.d2_ci_and_or =
								(D2_crit_set * )OMP_XALLOC(workspace,
																		AL_CRIT_SET,
								 1)) == 0) {
								ret	 = OM_MEMORY_INSUFFICIENT;
								break;
							} /* endif */
							memset(SRC_OBJ->d2_crit_val.d2_ci_and_or,
								 0,	sizeof(D2_crit_set));
						} /* endif */
						nb_crit	 = SRC_OBJ->d2_crit_val.d2_ci_and_or->and_or_nr;
						UPDATE_CRIT(&nb_crit, (void
							*) &	(SRC_OBJ->d2_crit_val.d2_ci_and_or->and_or));
						SRC_OBJ->d2_crit_val.d2_ci_and_or->and_or_nr =
							nb_crit;
					} /* endif */
				} /* endif */
				break;

			} /* endswitch */
			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
		CHECK_END;
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_search_criterion */


/********************************************************/
/*							*/
/*	put	SEARCH GUIDE object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_put_search_guide(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
#define	SRG_OBJ	((xds_search_guide *) *xds_object)
	OM_return_code	ret = OM_SUCCESS;
	OMP_type_list	 * pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_sint	check;
	OM_descriptor	 * desc_ptr;

	D2_criteria		 * crit_pt;

	OMP_type_list	type_list[]	 = SR_GUID_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		ALLOCATE_OBJECT(SEARCH_GUIDE, xds_search_guide);
		CHECK_INIT;
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case DS_OBJECT_CLASS:
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT_IDENTIFIER_STRING);
				UPDATE_STRING ((SRG_OBJ->d2_obj.d2_typ_len),
				(SRG_OBJ->d2_obj.d2_type),
				((OM_descriptor *)pt_l->desc)->value.string.length,
				((OM_descriptor * )pt_l->desc)->value.string.elements);
				if (SRG_OBJ->d2_obj.d2_type) {
					SRG_OBJ->d2_obj_defined	 = TRUE;
				} else {
					SRG_OBJ->d2_obj_defined	 = FALSE;
				} /* endif */
				break;

			case DS_CRITERIA:
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				/* copy	the	new	value */
				crit_pt	 = &(SRG_OBJ->d2_crit);
				dsP_delete_criteria(workspace, crit_pt);
				memset(crit_pt,	0, sizeof(D2_criteria));
				PUB_CP_CRIT(crit_pt, 1);
				break;

			} /* endswitch */
			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
		CHECK_END;
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_search_guide */


/********************************************************/
/*							*/
/*	put	TELETEX	TERMINAL ID	object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_put_ttxid(
	OMX_workspace		workspace,
	void 				**xds_object,
	OM_object			source,
	OMP_object_type		object_type,
	OM_modification		modification,
	OM_type_list		included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
#define	TXI_OBJ	((xds_ttxid	*) *xds_object)
	OM_return_code	ret = OM_SUCCESS;
	OMP_type_list	 * pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_sint	check;
	OM_descriptor	 * desc_ptr;

	OM_descriptor	 * src_pt;
	OM_descriptor	 * desc;
	OM_public_object	src_puba;
	OM_object		src;
	OMP_object_type	src_type;

	D2_str		 * str_pt;
	OM_string_length	str_len;
	void	*str_elem;

	OMP_type_list	type_list[]	 = TTXID_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		ALLOCATE_OBJECT(TELETEX_TERM_IDENT,		xds_ttxid);
		CHECK_INIT;
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case DS_PARAMETERS:
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				ret	 = OM_SUCCESS;
				if (OMP_REMOVE)	 {
					dsP_delete_str(workspace, &(TXI_OBJ->d2_controls));
					memset(&TXI_OBJ->d2_controls, 0,
							sizeof(D2_str));
					dsP_delete_str(workspace, &(TXI_OBJ->d2_graphics));
					memset(&TXI_OBJ->d2_graphics, 0,
							sizeof(D2_str));
					dsP_delete_str(workspace, &(TXI_OBJ->d2_miscel));
					memset(&TXI_OBJ->d2_miscel,	0,	sizeof(D2_str));
					dsP_delete_str(workspace, &(TXI_OBJ->d2_pages));
					memset(&TXI_OBJ->d2_pages, 0,	sizeof(D2_str));
					dsP_delete_str(workspace, &(TXI_OBJ->d2_privates));
					memset(&TXI_OBJ->d2_privates, 0,
							sizeof(D2_str));
					break;
				} /* endif */
				desc = (OM_descriptor *		)pt_l->desc;
				src	 = desc->value.object.object;
				if (src	 == 0) {
					ret	 = OM_WRONG_VALUE_MAKEUP;
					break;
				} /* endif */
				src_puba = (OM_public_object )0;
				if (OMP_PRIVATE(src)) {
					/* check valid handle */
					if (!OMP_VALID_OBJECT(src))	 {
						ret	 = OM_NO_SUCH_OBJECT;
						break;
					} /* endif */

					if ((ret = om_get(src, OM_NO_EXCLUSIONS,
						 0,	0, 0,	0, &src_puba, &desc_nb)) != OM_SUCCESS) {
						break;
					} /* endif */
					if (desc_nb	 == 0) {
						/*delete NULL descriptor*/
						om_delete(src_puba);
						ret	 = OM_NO_SUCH_OBJECT;
						break;
					} /* endif */
					src	 = src_puba;
				} /* endif */
				/* test	OM_CLASS of	the	sub_object */
				if (OMP_TYPE(src) != OM_CLASS) {
					if (src	 == src_puba) {
						om_delete (src_puba);
					} /* endif */
					ret	 = OM_NO_SUCH_OBJECT;
					break;
				} /* endif */
				src_pt = src;
				src_pt++;

				if (((ret = omP_get_object_type(workspace,
				src->value.string,	&src_type))	 ==  OM_SUCCESS) 
				&& (OMP_PACKAGE(src_type) == XMH_PACKAGE) 
				&& (OMP_CLASS(src_type) == XMH_TELETEX_NBPS)) {
					for	(; src_pt->type	 !=  OM_NO_MORE_TYPES; src_pt++) {
						switch (OMP_TYPE(src_pt)) {
						case MH_T_CONTROL_CHARACTER_SETS:
							VALIDATE_SYNTAX(src_pt, OM_S_TELETEX_STRING);
							str_pt = &(TXI_OBJ->d2_controls);
							if (src_pt->syntax & OM_S_LOCAL_STRING) {
								unsigned char* result; OM_uint32 result_len = 0;
								if ( d27_local_to_t61(
								(char *)str_pt->d2_value, str_pt->d2_size,
								(char **)&result, &result_len) != D2_NOERROR) {
									ret = OM_WRONG_VALUE_SYNTAX;
									goto end;
								} else {
									str_pt->d2_value = result;
									str_pt->d2_size = result_len;
									/* possible memory leak */
								} /* endif */
							} /* endif */
							break;
						case MH_T_GRAPHIC_CHARACTER_SETS:
							VALIDATE_SYNTAX(src_pt, OM_S_TELETEX_STRING);
							str_pt = &(TXI_OBJ->d2_graphics);
							if (src_pt->syntax & OM_S_LOCAL_STRING) {
								unsigned char* result; OM_uint32 result_len = 0;
								if ( d27_local_to_t61(
								(char *)str_pt->d2_value, str_pt->d2_size,
								(char **)&result, &result_len) != D2_NOERROR) {
									ret = OM_WRONG_VALUE_SYNTAX;
									goto end;
								} else {
									str_pt->d2_value = result;
									str_pt->d2_size = result_len;
									/* possible memory leak */
								} /* endif */
							} /* endif */
							break;
						case MH_T_MISCELLANEOUS_CAPABILITIES:
							VALIDATE_SYNTAX(src_pt, OM_S_TELETEX_STRING);
							str_pt = &(TXI_OBJ->d2_miscel);
							if (src_pt->syntax & OM_S_LOCAL_STRING) {
								unsigned char* result; OM_uint32 result_len = 0;
								if ( d27_local_to_t61(
								(char *)str_pt->d2_value, str_pt->d2_size,
								(char **)&result, &result_len) != D2_NOERROR) {
									ret = OM_WRONG_VALUE_SYNTAX;
									goto end;
								} else {
									str_pt->d2_value = result;
									str_pt->d2_size = result_len;
									/* possible memory leak */
								} /* endif */
							} /* endif */
							break;
						case MH_T_PAGE_FORMATS:
							VALIDATE_SYNTAX(src_pt, OM_S_OCTET_STRING);
							str_pt = &(TXI_OBJ->d2_pages);
							break;
						case MH_T_PRIVATE_USE:
							VALIDATE_SYNTAX(src_pt, OM_S_OCTET_STRING);
							str_pt = &(TXI_OBJ->d2_privates);
							break;
						default:
							ret	 = OM_NO_SUCH_SYNTAX;
						} /* endswitch */
						if (ret	 != OM_SUCCESS) {
							break;
						} /* endif */
						PUB_CP_STR_SRC(src_pt, str_pt, 1);
					} /* endfor */
				} else {
					ret	 = OM_WRONG_VALUE_TYPE;
				} /* endif */

				if (src	 == src_puba) {
					om_delete (src_puba);
				} /* endif */
				break;

			case DS_TELETEX_TERM:
				CHECK_SYNTAX_AND_LENGTH(pt_l->desc,	OM_S_PRINTABLE_STRING,
						MIN_VL_TELETEX_TERM, DS_VL_TELETEX_TERM);
				str_pt = &(TXI_OBJ->d2_terminal);
				PUB_CP_STR(str_pt, 1);
				break;

			} /* endswitch */
			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
		CHECK_END;
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_ttxid */


/********************************************************/
/*							*/
/*	put	TELEX NUMBER object				*/
/*							*/
/********************************************************/
static OM_return_code
dsP_put_telex_number(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
#define	TXN_OBJ	((xds_telex_number *) *xds_object)
	OM_return_code	ret = OM_SUCCESS;
	OMP_type_list	 * pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_sint	check;
	OM_descriptor	 * desc_ptr;

	D2_str		 * str_pt;
	OM_string_length	str_len;
	void	*str_elem;

	OMP_type_list	type_list[]	 = TX_NB_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		ALLOCATE_OBJECT(TELEX_NBR, xds_telex_number);
		CHECK_INIT;
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case DS_ANSWERBACK:
				CHECK_SYNTAX_AND_LENGTH(pt_l->desc,	OM_S_PRINTABLE_STRING,
						MIN_VL_ANSWERBACK, DS_VL_ANSWERBACK);
				str_pt = &(TXN_OBJ->d2_answer);
				PUB_CP_STR(str_pt, 1);
				break;

			case DS_COUNTRY_CODE:
				CHECK_SYNTAX_AND_LENGTH(pt_l->desc,	OM_S_PRINTABLE_STRING,
						MIN_VL_COUNTRY_CODE, DS_VL_COUNTRY_CODE);
				str_pt = &(TXN_OBJ->d2_c_code);
				PUB_CP_STR(str_pt, 1);
				break;

			case DS_TELEX_NBR:
				CHECK_SYNTAX_AND_LENGTH(pt_l->desc,	OM_S_PRINTABLE_STRING,
				MIN_VL_TELEX_NBR, DS_VL_TELEX_NBR);
				str_pt = &(TXN_OBJ->d2_t_number);
				PUB_CP_STR(str_pt, 1);
				break;

			} /* endswitch */
			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
		CHECK_END;
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_telex_number */


/********************************************************/
/*							*/
/*	put	ACCESS CONTROL ITEM	object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_put_aci	(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
#define	ACI_OBJ	((xds_acl_item ) *xds_object)

	OM_return_code	ret = OM_SUCCESS;
	OMP_type_list	 * pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_sint	check;
	OM_descriptor	 * desc_ptr;

	xds_acl_item	aci_ptr;
	xds_acl_item	aci_str;
	Aci_info		aci_info;
	OM_sint	no_acl;
	OM_sint	new_no_acl;
	OM_sint	aci_len;
	OM_sint	no_aci;
	xds_dist_name	dname;
	void	**name_obj;

#define EMPTY_ACI	"30"

#define EMPTY_ACI_LENGTH strlen(EMPTY_ACI)

#define ACI_LENGTH(ACI)	strlen(ACI.ai_p) + 2

#define UPDATE_ACI_LENGTH(ACI)	ACI.ai_len = strlen(ACI.ai_p) +	2

	#define CLEAR_ACL_INFO(ACL_INFO)					\
{									\
	OM_sint		iaci;							\
	ACL_INFO.ar_val_nb = 0;						\
	ACL_INFO.ar_len = D2_NOIDX_ACL * EMPTY_ACI_LENGTH;			\
	ACL_INFO.ar_p = (char *)0;						\
	for	(iaci = 0; iaci	< D2_NOIDX_ACL;	iaci++)				\
	{									\
	ACL_INFO.ar_info[iaci].ai_len = EMPTY_ACI_LENGTH;		\
	ACL_INFO.ar_info[iaci].ai_inter = D2_IGNORE;			\
	ACL_INFO.ar_info[iaci].ai_p = "";				\
	}									\
}

	OMP_type_list	type_list[]	 = ACI_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	dname = (xds_dist_name )0;
	while ((ret	 = PUB_TYPE_IN_LIST(type_list, desc_ptr)) ==  OM_SUCCESS)	 {
		if (!ACI_OBJ) {
			aci_info.ai_inter = D2_IGNORE;
			aci_info.ai_p = "";
			UPDATE_ACI_LENGTH(aci_info);
			no_aci = 0;
			aci_len	 = aci_info.ai_len;
		} /* decode acl string and get the number of usable info in the object*/
		else if ((ret = dsP_decode_aci (&aci_info, ACI_OBJ, &aci_len,
		&no_aci)) != OM_SUCCESS) {
				break;
		} /* endif */

		aci_ptr	 = ACI_OBJ;
		/* initialise new acl number with the old one */
		new_no_acl = no_acl;

		ret	 = OM_SUCCESS;
		CHECK_INIT;
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {
			case DSX_INTERPRETATION:
				if (check) {
					VALIDATE_SYNTAX(pt_l->desc,	OM_S_ENUMERATION);
					if (!OMP_REMOVE) {
						VALIDATE_ENUM(pt_l->desc, inter_type_limit);
					} /* endif */
					break;
				} /* endif */
/*				 CHECK_SYNTAX(pt_l->desc,OM_S_ENUMERATION); */
				if (OMP_REMOVE)	 {
					aci_info.ai_inter = D2_IGNORE;
					aci_info.ai_p = "";
					UPDATE_ACI_LENGTH(aci_info);
				} else {
					aci_info.ai_inter = DSX_TO_IAPL_INTER(
					((OM_descriptor *)pt_l->desc)->value.enumeration);
				} /* endif */
				break;
			case DSX_USER:
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				if (OMP_REMOVE)	 {
					aci_info.ai_inter = D2_IGNORE;
					aci_info.ai_p = "";
					UPDATE_ACI_LENGTH(aci_info);
				} else {
					name_obj = (void **) & dname;
					ret = dsP_put_dist_name (workspace, name_obj,
					((OM_descriptor *)pt_l->desc)->value.object.object,
					object_type, OM_REPLACE_ALL, 0,0, OM_ALL_VALUES);
				} /* endif */
				break;
			} /* endswitch */
			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
		CHECK_END;
		if (ret	 != OM_SUCCESS) {
			break;
		} /* endif */
		if (dname) {
			aci_info.ai_len = strlen((char*)dname)+ 2; /* inter+ name + \0 */
			aci_info.ai_p = (char *)dname;
		} /* endif */
		if ((aci_ptr = (xds_acl_item )OMP_XALLOC(workspace,	AL_STRING,
		ROUNDUP(workspace, aci_info.ai_len))) == 0) {
			ret	 = OM_MEMORY_INSUFFICIENT;
			break;
		} /* endif */
		aci_str	 = aci_ptr;
		if ((ret = dsP_encode_aci (&aci_info, &aci_str)) !=  OM_SUCCESS) {
			break;
		} /* endif */
		if (*xds_object) {
			/* delete the old acl string	*/
			OMP_XFREE_STR(workspace, *xds_object, aci_len);
		} /* endif */
		*xds_object	 = aci_ptr;
		if (1) {	/* just to suppress warning on sinix */
			break;
		} /* endif */
	} /* endwhile */
	if (dname) {
		OMP_XFREE_STR(workspace, dname, strlen((char *)dname));
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_aci  */


/********************************************************/
/*							*/
/*	put	ACCESS CONTROL LIST	 object			*/
/*							*/
/********************************************************/
static OM_return_code
dsP_put_acl	(
	OMX_workspace	workspace,
	void			**xds_object,
	OM_object		source,
	OMP_object_type	object_type,
	OM_modification	modification,
	OM_type_list	included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
#define	ACL_OBJ	((xds_acl )	*xds_object)

	OM_return_code	ret = OM_SUCCESS;
	OM_sint			i;
	OMP_type_list	 * pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean		priv_flag;
	OM_sint	check;
	OM_descriptor	 * desc_ptr;

	OM_descriptor	 * src_ptr;
	OM_value_position	init_val;
	OM_value_position	limit_val;

	xds_acl		acl_ptr;
	xds_acl		acl_str;
	Aci_info		 * aci_info;
	xds_acl_item	acl_item;
	void	**acl_i_obj;
	Aci_info		 * src;
	Aci_info		 * dest;
	Acr_info		acl[D2_NOACCESS_RIGHTS];
	Acr_info		 * acl_info;
	char	*str_array[D2_ACLIDX_MAX];
	OM_sint	str2free = 0;
	OM_sint	n_ins;
	OM_sint	n_res;
	OM_sint	n_rep;
	OM_sint	no_acl;
	OM_sint	new_no_acl;
	OM_sint	acl_len;
	OM_sint	new_acl_len;


	OMP_type_list	type_list[]	 = ACL_TYPE_LIST;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
		 limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *		)source;

	while ((ret	 = PUB_TYPE_IN_LIST(type_list, desc_ptr)) ==  OM_SUCCESS)	 {
		acl_info = acl;
		if (!ACL_OBJ) {
			/* clear acl_info array	*/
			for	(i = 0;	i <	D2_NOACCESS_RIGHTS;	i++) {
				CLEAR_ACL_INFO(acl[i]);
			} /* endfor */
			acl_len	 = EMPTY_ACI_LENGTH	* D2_ACLIDX_MAX;
			no_acl = 0;
		}
		/* decode acl string and get the number of usable info in the object*/
		else if ((ret = dsP_decode_acl (&acl_info, ACL_OBJ, &acl_len, &no_acl))
		!= OM_SUCCESS) {
				break;
		} /* endif */

		/* initialise new acl info number with the old one */
		new_no_acl = no_acl;

		ret	 = OM_SUCCESS;
		init_val = initial_value;
		limit_val = limiting_value;
		str2free = 0;
		/* clear new ACL_ITEM string pointer array */
		for	(i = 0;	i <	D2_ACLIDX_MAX; str_array[i++] = NULL) {
			;
		} /* endfor */

		CHECK_INIT;
		for	( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {
			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */
			initial_value = init_val;
			limiting_value = limit_val;
			acl_info = acl;
			switch (pt_l->type)	 {
			case DSX_MODIFY_SENSITIVE:
				acl_info++;
			case DSX_READ_SENSITIVE:
				acl_info++;
			case DSX_MODIFY_STANDARD:
				acl_info++;
			case DSX_READ_STANDARD:
				acl_info++;
			case DSX_MODIFY_PUBLIC:
				no_acl = acl_info->ar_val_nb;
				UPDATE_LIMIT_VAL (no_acl, break);
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				if (OMP_REMOVE) {
					pt_l->nb_val = 0;
				} /* endif */
				n_ins = pt_l->nb_val;
				n_rep = limiting_value - initial_value;
				n_res = no_acl - limiting_value;
				new_no_acl = initial_value + n_ins +  n_res;
				if (new_no_acl > D2_NOIDX_ACL) {
					ret	 = OM_WRONG_VALUE_NUMBER;
					break;
				} /* endif */
				if (new_no_acl != no_acl) {
					if (n_res) {
						if (n_rep > n_ins) {
							memcpy(&(acl_info->ar_info [initial_value+n_ins]),
							&(acl_info->ar_info[limiting_value]),
							n_res * sizeof(Aci_info));
						} else {
							for	(dest =  &(acl_info->ar_info[new_no_acl-1]),
							src = &(acl_info->ar_info[no_acl -	1]);
							n_res >	0; dest--, src--,n_res--) {
								memcpy(dest, src, sizeof(Aci_info));
							} /* endfor */
						} /* endif */
					} /* endif */
				} /* endif */
				/* update acl_info structures with new acl info	*/
				for	(i = 0,
				src_ptr	 = (OM_descriptor *)pt_l->desc,
				aci_info = &(acl_info->ar_info[initial_value]);
				i <	pt_l->nb_val; i++, src_ptr++, aci_info++,
				str2free++) {
					acl_item = (xds_acl_item )0;
					acl_i_obj = (void **) &	 acl_item;
					if ((ret = dsP_put_aci (workspace, acl_i_obj,
					src_ptr->value.object.object, object_type,
					OM_REPLACE_ALL,	0, 0, OM_ALL_VALUES)) != OM_SUCCESS) {
						break;
					} /* endif */
					/* save acl_item string pointer to free the string
				 * after ACL encoding
				 */
					str_array[str2free]	 = (char *)acl_item;
					/* init Aci_info structure for the new ACL_ITEM	*/
					aci_info->ai_inter = *((char *)acl_item);
					aci_info->ai_p = (char *)acl_item + 1;
					aci_info->ai_len = strlen(aci_info->ai_p) + 2;
				} /* endfor */
				if (ret	 != OM_SUCCESS) {
					break;
				} /* endif */
				for	(i = new_no_acl; i < D2_NOIDX_ACL; i++)	{
					/* ignore the last ACL_ITEMs */
					acl_info->ar_info[i].ai_len	 = EMPTY_ACI_LENGTH;
					acl_info->ar_info[i].ai_inter = D2_IGNORE;
					acl_info->ar_info[i].ai_p =  "";
				} /* endfor */
				/* update Acr_info structure: ACL_ITEM#	and length */
				acl_info->ar_val_nb	 = new_no_acl;
				for	(i = 0,	acl_info->ar_len = 0; i < D2_NOIDX_ACL; i++) {
					acl_info->ar_len += acl_info->ar_info[i].ai_len;
				} /* endfor */

				break;
			} /* endswitch */
			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
			/* update new ACL string length	*/
			for	(i = 0,	new_acl_len	 = 0; i	< D2_NOACCESS_RIGHTS; i++) {
				new_acl_len	 += acl[i].ar_len;
			} /* endfor */
		} /* endfor */
		CHECK_END;
		if ((acl_ptr = (xds_acl	)OMP_XALLOC(workspace, AL_STRING,
			 ROUNDUP(workspace,	new_acl_len))) ==  0) {
			ret	 = OM_MEMORY_INSUFFICIENT;
			break;
		} /* endif */
		acl_str	 = acl_ptr;
		acl_info = acl;
		/* encode the new ACL */
		if ((ret = dsP_encode_acl(&acl_info, &acl_str))	 !=  OM_SUCCESS) {
			break;
		} /* endif */
		if (*xds_object) {
			/* delete the old acl string	*/
			OMP_XFREE_STR(workspace, *xds_object, acl_len);
		} /* endif */
		*xds_object	 = acl_ptr;
		if (1) {	/* just to suppress warning on sinix */
			break;
		} /* endif */
	} /* endwhile */
	/* free	strings allocated for each new acl info inserted or replaced */
	for	(i = 0;	i <	str2free; i++) {
		OMP_XFREE_STR(workspace, str_array[i], strlen(str_array[i])	 + 1);
	} /* endfor */
	TEST_DEL_PUB(ret);
end:
	return(ret);
} /* end dsP_put_acl  */

static OM_return_code
/********************************************************/
				dsP_put_algorithm_ident(
/********************************************************/
	OMX_workspace		workspace,
	void				**xds_object,
	OM_object			source,
	OMP_object_type		object_type,
	OM_modification		modification,
	OM_type_list		included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
	xds_algorithm_ident	*alg_id_obj = *xds_object;
	OM_return_code		ret = OM_SUCCESS;
	OMP_type_list	 	*pt_l;
	OM_public_object	src_pub;
	OM_value_position	 desc_nb;
	OM_boolean			priv_flag;
	OM_sint				check;
	OM_descriptor	 	*desc_ptr;
	OMP_type_list		type_list[]	 = ALGORITHM_IDENT_TYPE_LIST;
	OM_boolean			empty_obj = OM_FALSE;

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
	limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		if (!xds_object) {
			empty_obj = OM_TRUE;
		} /* endif */
		ALLOCATE_OBJECT(ALGORITHM_IDENT, xds_algorithm_ident);
		/* need to repeat follwing assignment, because macro 
		changes xds_object */
		alg_id_obj = *xds_object;
		CHECK_INIT;

		for ( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {

			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type) {

			case DS_ALGORITHM:
				VALIDATE_SYNTAX(pt_l->desc,	OM_S_OBJECT_IDENTIFIER_STRING);
				UPDATE_STRING (	(alg_id_obj->algorithm.d2_typ_len),
				(alg_id_obj->algorithm.d2_type),
				((OM_descriptor	*)pt_l->desc)->value.string.length,
				((OM_descriptor *)pt_l->desc)->value.string.elements);
				break;
			case DS_ALGORITHM_PARAMETERS:
				/* must be extended when not integer */
				alg_id_obj->parameters = malloc(sizeof(*(alg_id_obj->parameters)));
				CHECK_INTEGER_AND_UPDATE((OM_descriptor	*)pt_l->desc,
				*(alg_id_obj->parameters));
				break;
			default:
				ret = OM_NO_SUCH_SYNTAX;

			} /* endswitch */

			if (ret != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
		CHECK_END;
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	*xds_object = alg_id_obj; 
	return(ret);
} /* end dsP_put_algorithm_ident */

static OM_return_code
/********************************************************/
				dsP_put_certificate(
/********************************************************/
	OMX_workspace		workspace,
	void 				**xds_object,
	OM_object			source,
	OMP_object_type		object_type,
	OM_modification		modification,
	OM_type_list		included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
	xds_certificate 	*cert_obj = *xds_object;
	OM_return_code		ret = OM_SUCCESS;
	OMP_type_list		*pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean			priv_flag;
	OM_sint				check;
	OM_descriptor		*desc_ptr;
	OMP_type_list		type_list[]	 = CERTIFICATE_TYPE_LIST;
	void				**name_obj;
	OM_sint				len;
	OM_boolean			empty_obj = OM_FALSE;
	#define	CERT_OBJ	((xds_certificate *)*xds_object)


	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
	limiting_value);


	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		if (!*xds_object) {
			empty_obj = OM_TRUE;
		} /* endif */
		ALLOCATE_OBJECT(CERTIFICATE, xds_certificate);
		/* need to repeat follwing assignment, because macro 
		changes xds_object */
		cert_obj = *xds_object;
		if (empty_obj) {
			cert_obj->to_be_signed.version = IAPL_V1988;
		} /* endif */
		CHECK_INIT;

		for ( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {

			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {

			case DS_ISSUER:
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				name_obj = (void **) & CERT_OBJ->to_be_signed.issuer;
				REMOVE_OBJ(dist_name, name_obj);
				ret = dsP_put_dist_name(workspace,name_obj,
				((OM_descriptor * )pt_l->desc)->value.object.object,
				object_type, OM_REPLACE_ALL, 0,0, OM_ALL_VALUES);
				break;

			case DS_SIGNATURE:
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				name_obj = (void **) & CERT_OBJ->to_be_signed.signature;
				REMOVE_OBJ(algorithm_ident, name_obj);
				ret	 = dsP_put_algorithm_ident (workspace, (void **)&name_obj,
				((OM_descriptor *)pt_l->desc)->value.object.object,
				object_type, OM_REPLACE_ALL, 0,	0, OM_ALL_VALUES);
				break;

			case DS_SIGNATURE_VALUE:
				CHECK_SYNTAX(pt_l->desc, OM_S_OCTET_STRING);
				len	 = cert_obj->encr_str.d2_size;
				UPDATE_STRING (len,	(cert_obj->encr_str.d2_value),
				((OM_descriptor *)pt_l->desc)->value.string.length,
				((OM_descriptor *)pt_l->desc)->value.string.elements);
				if (ret	 == OM_SUCCESS) {
					cert_obj->encr_str.d2_size =  len * 8;
				} /* endif */
				break;
				
			case DS_SERIAL_NBR :
				CHECK_INTEGER_AND_UPDATE((OM_descriptor	*)pt_l->desc,
				cert_obj->to_be_signed.serial_number);
				break;

			case DS_SUBJECT	:
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				name_obj = (void **) & CERT_OBJ->to_be_signed.subject;
				REMOVE_OBJ(dist_name, name_obj);
				ret = dsP_put_dist_name(workspace,name_obj,
				((OM_descriptor * )pt_l->desc)->value.object.object,
				object_type, OM_REPLACE_ALL, 0,0, OM_ALL_VALUES);
				break;

			case DS_SUBJECT_ALGORITHM	:
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				name_obj = (void **)
				&cert_obj->to_be_signed.subj_public_key_info.algorithm;
				REMOVE_OBJ(algorithm_ident, name_obj);
				ret	 = dsP_put_algorithm_ident (workspace,	(void **)&name_obj,
				((OM_descriptor *)pt_l->desc)->value.object.object,
				object_type, OM_REPLACE_ALL, 0,	0, OM_ALL_VALUES);
				name_obj = (void **)
				&cert_obj->alg.algorithm;
				REMOVE_OBJ(algorithm_ident, name_obj);
				ret	 = dsP_put_algorithm_ident (workspace,	(void **)&name_obj,
				((OM_descriptor *)pt_l->desc)->value.object.object,
				object_type, OM_REPLACE_ALL, 0,	0, OM_ALL_VALUES);
				break;

			case DS_SUBJECT_PUBLIC_KEY	:
				CHECK_SYNTAX(pt_l->desc, OM_S_BIT_STRING);
				len	 = ((cert_obj->to_be_signed.subj_public_key_info.
				subject_key.d2_size + 7) & -8 ) /8;
				UPDATE_STRING (len, cert_obj->to_be_signed.
				subj_public_key_info.subject_key.d2_value,
				((((OM_descriptor *)pt_l->desc)->value.string.length +7)& -8)/8,
				((OM_descriptor *)pt_l->desc)->value.string.elements);
				if (ret	 == OM_SUCCESS) {
					cert_obj->to_be_signed.subj_public_key_info
					.subject_key.d2_size = ((OM_descriptor *)pt_l->desc)
					->value.string.length; 
				} /* endif */
				break;

			case DS_VALIDITY_NOT_AFTER :
				CHECK_SYNTAX(pt_l->desc, OM_S_UTC_TIME_STRING);
				if (cert_obj->to_be_signed.validity.not_after != NULL) {
					len	= strlen(cert_obj->to_be_signed.validity.not_after);
				} else {
					len = 0;
				} /* endif */
				UPDATE_STRING (len, cert_obj->to_be_signed.validity.not_after,
				((OM_descriptor *)pt_l->desc)->value.string.length,
				((OM_descriptor *)pt_l->desc)->value.string.elements);
				break;

			case DS_VALIDITY_NOT_BEFORE :
				CHECK_SYNTAX(pt_l->desc, OM_S_UTC_TIME_STRING);
				if (cert_obj->to_be_signed.validity.not_before != NULL) {
					len	= strlen(cert_obj->to_be_signed.validity.not_before);
				} else {
					len = 0;
				} /* endif */
				UPDATE_STRING (len, cert_obj->to_be_signed.validity.not_before,
				((OM_descriptor *)pt_l->desc)->value.string.length,
				((OM_descriptor *)pt_l->desc)->value.string.elements);
				break;

			case DS_VERSION	:
				/* values of enumerations do not correspond directly
					IAPL values start at 0, XOM at 1
					assume Viapl = Vxom - 1 */
				VALIDATE_SYNTAX(pt_l->desc, OM_S_ENUMERATION);
				UPDATE_ENUM((OM_descriptor *)pt_l->desc,
				cert_obj->to_be_signed.version);
				cert_obj->to_be_signed.version -= 1;
				break;

			default:
				ret	 = OM_NO_SUCH_SYNTAX;
			} /* endswitch */

			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
		CHECK_END;
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	*xds_object = cert_obj; 
	return(ret);
} /* end dsP_put_certificate */

static OM_return_code
/********************************************************/
					dsP_put_certificate_pair(
/********************************************************/
	OMX_workspace		workspace,
	void 				**xds_object,
	OM_object			source,
	OMP_object_type		object_type,
	OM_modification		modification,
	OM_type_list		included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
	xds_certificate_pair 	*cert_pair_obj = *xds_object;
	OM_return_code		ret = OM_SUCCESS;
	OMP_type_list		*pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean			priv_flag;
	OM_sint				check;
	OM_descriptor		*desc_ptr;
	OMP_type_list		type_list[]	 = CERTIFICATE_PAIR_TYPE_LIST;
	void				**name_obj;
	OM_boolean			empty_obj = OM_FALSE;
	#define	CERT_PAIR_OBJ	((xds_certificate_pair *)*xds_object)


	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
	limiting_value);


	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		if (!*xds_object) {
			empty_obj = OM_TRUE;
		} /* endif */
		ALLOCATE_OBJECT(CERTIFICATE_PAIR, xds_certificate_pair);
		/* need to repeat follwing assignment, because macro 
		changes xds_object */
		cert_pair_obj = *xds_object;
		CHECK_INIT;

		for ( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {

			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {

			case DS_FORWARD:
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				name_obj =  (void **)&CERT_PAIR_OBJ->forward;
				REMOVE_OBJ(certificate, name_obj);
				ret = dsP_put_certificate(workspace,name_obj,
				((OM_descriptor * )pt_l->desc)->value.object.object,
				object_type, OM_REPLACE_ALL, 0,0, OM_ALL_VALUES);
				break;
			case DS_REVERSE:
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				name_obj = (void **)&CERT_PAIR_OBJ->reverse;
				REMOVE_OBJ(certificate, name_obj);
				ret = dsP_put_certificate(workspace,name_obj,
				((OM_descriptor * )pt_l->desc)->value.object.object,
				object_type, OM_REPLACE_ALL, 0,0, OM_ALL_VALUES);
				break;
			default:
				ret	 = OM_NO_SUCH_SYNTAX;
			} /* endswitch */

			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
		CHECK_END;
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	*xds_object = cert_pair_obj; 
	return(ret);
} /* end dsP_put_certificate_pair */


static OM_return_code
/********************************************************/
				dsP_put_certificate_sublist(
/********************************************************/
	OMX_workspace		workspace,
	void 				**xds_object,
	OM_object			source,
	OMP_object_type		object_type,
	OM_modification		modification,
	OM_type_list		included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
	xds_certificate_sublist 	*cert_sublist_obj = *xds_object;
	xds_certificate_subsublist 	*cert_subsublist_obj = NULL;
	OM_return_code		ret = OM_SUCCESS;
	OMP_type_list		*pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean			priv_flag;
	OM_sint				check;
	OM_descriptor		*desc_ptr;
	OMP_type_list		type_list[]	 = CERTIFICATE_SUBLIST_TYPE_LIST;
	void				**name_obj;
	OM_sint				len;
	OM_boolean			empty_obj = OM_FALSE;
	OM_uint				number_of_revoc_dates = 0;
	OM_uint				number_of_serial_nbrs = 0;
	OM_object 			kept_signature, kept_issuer;
	OM_sint				i;
	#define	CERT_SUBLIST_OBJ	((xds_certificate_sublist *)*xds_object)


	if (cert_sublist_obj == NULL) {
		cert_subsublist_obj = NULL;
	} else {
		cert_subsublist_obj = cert_sublist_obj->to_be_signed.rev_cert;
	} /* endif */

	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
	limiting_value);

	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		if (!*xds_object) {
			empty_obj = OM_TRUE;
			ALLOCATE_OBJECT(CERTIFICATE_SUBLIST, xds_certificate_sublist);
			/* following assignment repeated because macro changes xds_object */
			cert_sublist_obj = *xds_object;
		} /* endif */
		CHECK_INIT;

		for ( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {

			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {

			case DS_ISSUER:
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				kept_issuer = ((OM_descriptor*)pt_l->desc)->value.object.object;
				break;

			case DS_SIGNATURE:
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				kept_signature = 
				((OM_descriptor *)pt_l->desc)->value.object.object;
				name_obj = (void **) &cert_sublist_obj->alg;
				REMOVE_OBJ(algorithm_ident, name_obj);
				ret = dsP_put_algorithm_ident (workspace, (void **)&name_obj,
				(OM_descriptor *)kept_signature,
				object_type, OM_REPLACE_ALL, 0,	0, OM_ALL_VALUES);
				break;

			case DS_SIGNATURE_VALUE:
				CHECK_SYNTAX(pt_l->desc, OM_S_OCTET_STRING);
				len	 = cert_sublist_obj->encr_str.d2_size;
				UPDATE_STRING (len,	(cert_sublist_obj->encr_str.d2_value),
				((OM_descriptor *)pt_l->desc)->value.string.length,
				((OM_descriptor *)pt_l->desc)->value.string.elements);
				if (ret	 == OM_SUCCESS) {
					cert_sublist_obj->encr_str.d2_size =  len * 8;
				} /* endif */
				break;

			case DS_REVOC_DATE :
				{
				OM_descriptor	 * pt_desc;
				/* allocate array */
				if ( cert_subsublist_obj == NULL) {
					cert_subsublist_obj = calloc(pt_l->nb_val,
					sizeof(xds_certificate_subsublist));
				} /* endif */

				for	(i = 0,	pt_desc = pt_l->desc; i < pt_l->nb_val;
				i++, pt_desc++) {
					CHECK_SYNTAX(pt_desc, OM_S_UTC_TIME_STRING);
					if (OMP_REMOVE) {
						OM_FREE(cert_subsublist_obj[i].rev_date);
					} else {
						char **string = &(cert_subsublist_obj[i].rev_date);
						len = 0;
						UPDATE_STRING (len, string,
						pt_desc->value.string.length,
						pt_desc->value.string.elements);
						cert_subsublist_obj[i].rev_date = (char *)string;
					} /* endif */
				} /* endfor */
				} /* endblock */
				number_of_revoc_dates =  pt_l->nb_val;
				break;

			case DS_SERIAL_NBRS:
				{
				OM_descriptor	 * pt_desc;
				/* allocate array */
				if ( cert_subsublist_obj == NULL) {
					cert_subsublist_obj = calloc(pt_l->nb_val,
					sizeof(xds_certificate_subsublist));
				} /* endif */
				for	(i = 0,	pt_desc = pt_l->desc; i < pt_l->nb_val;
				i++, pt_desc++) {
					CHECK_SYNTAX(pt_desc, OM_S_INTEGER);
					if (OMP_REMOVE) {
						; /* allocated as block */
					} else {
						UPDATE_INTEGER((OM_descriptor *)pt_desc,
						cert_subsublist_obj[i].user_cert);
					} /* endif */
				} /* endfor */
				} /* endblock */
				number_of_serial_nbrs =  pt_l->nb_val;
				break;

			default:
				ret	 = OM_NO_SUCH_SYNTAX;
			} /* endswitch */

			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
		CHECK_END;
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	if (ret == OM_SUCCESS) {
		cert_sublist_obj->to_be_signed.rev_cert = cert_subsublist_obj;
		if ( number_of_revoc_dates == number_of_serial_nbrs) {
			cert_sublist_obj->to_be_signed.no_rev_certs = number_of_revoc_dates;
			for ( i = 0; i < number_of_revoc_dates; ++i) {
				
				/* replicate signature */
				name_obj = (void **) &cert_subsublist_obj[i].signature;
				REMOVE_OBJ(algorithm_ident, name_obj);
				ret = dsP_put_algorithm_ident (workspace, (void **)&name_obj,
				(OM_descriptor *)kept_signature,
				object_type, OM_REPLACE_ALL, 0,	0, OM_ALL_VALUES);

				/* replicate issuer */
				name_obj = (void **) &cert_subsublist_obj[i].issuer;
				REMOVE_OBJ(dist_name, name_obj);
				ret = dsP_put_dist_name(workspace,name_obj,
				(OM_descriptor * )kept_issuer,
				object_type, OM_REPLACE_ALL, 0,0, OM_ALL_VALUES);
			} /* endfor */
			*xds_object = cert_sublist_obj; 
		} else {
			OM_FREE(cert_sublist_obj);
			OM_FREE(cert_subsublist_obj);
			ret = OM_WRONG_VALUE_NUMBER;
		} /* endif */
	} /* endif */
	return(ret);
} /* end dsP_put_certificate_sublist */


static OM_return_code
/********************************************************/
					dsP_put_certificate_list(
/********************************************************/
	OMX_workspace		workspace,
	void 				**xds_object,
	OM_object			source,
	OMP_object_type		object_type,
	OM_modification		modification,
	OM_type_list		included_types,
	OM_value_position	initial_value,
	OM_value_position	limiting_value)
{
	xds_certificate_list 	*cert_list_obj = *xds_object;
	OM_return_code		ret = OM_SUCCESS;
	OMP_type_list		*pt_l;
	OM_public_object	src_pub;
	OM_value_position	desc_nb;
	OM_boolean			priv_flag;
	OM_sint				check;
	OM_descriptor		*desc_ptr;
	OM_sint				i;
	OMP_type_list		type_list[]	 = CERTIFICATE_LIST_TYPE_LIST;
	void				**name_obj;
	void				**sublist_obj;
	OM_sint				len;
	OM_boolean			empty_obj = OM_FALSE;
	OM_uint				number_of_sublists = 0;
	#define	CERT_LIST_OBJ	((xds_certificate_list *)*xds_object)


	VALIDATE_TYPE_AND_MOD (type_list, included_types, initial_value,
	limiting_value);


	TEST_SOURCE_PRIV(EXCLUDE_SUB_OBJECTS);

	/* initialise pointer to the first useful descriptor */
	desc_ptr = (OM_descriptor *)source;

	if ((ret = PUB_TYPE_IN_LIST(type_list, desc_ptr)) == OM_SUCCESS) {
		if (!*xds_object) {
			empty_obj = OM_TRUE;
		} /* endif */
		ALLOCATE_OBJECT(CERTIFICATE_LIST, xds_certificate_list);
		/* need to repeat follwing assignment, because macro 
		changes xds_object */
		cert_list_obj = *xds_object;
		CHECK_INIT;

		for ( pt_l = type_list;	pt_l->type != OM_NO_MORE_TYPES; pt_l++)	 {

			if (VALUE_NOT_PRESENT) {
				continue;
			} /* endif */

			switch (pt_l->type)	 {

			case DS_ISSUER:
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				name_obj = (void **) & CERT_LIST_OBJ->to_be_signed.issuer;
				REMOVE_OBJ(dist_name, name_obj);
				ret = dsP_put_dist_name(workspace,name_obj,
				((OM_descriptor * )pt_l->desc)->value.object.object,
				object_type, OM_REPLACE_ALL, 0,0, OM_ALL_VALUES);
				break;

			case DS_SIGNATURE:
				CHECK_SYNTAX(pt_l->desc, OM_S_OBJECT);
				name_obj = (void **) & CERT_LIST_OBJ->to_be_signed.signature;
				REMOVE_OBJ(algorithm_ident, name_obj);
				ret	 = dsP_put_algorithm_ident (workspace, (void **)&name_obj,
				((OM_descriptor *)pt_l->desc)->value.object.object,
				object_type, OM_REPLACE_ALL, 0,	0, OM_ALL_VALUES);
				/* duplicate alg id because X.509 redundant */
				name_obj = (void **) & CERT_LIST_OBJ->alg;
				REMOVE_OBJ(algorithm_ident, name_obj);
				ret	 = dsP_put_algorithm_ident (workspace, (void **)&name_obj,
				((OM_descriptor *)pt_l->desc)->value.object.object,
				object_type, OM_REPLACE_ALL, 0,	0, OM_ALL_VALUES);
				break;

			case DS_SIGNATURE_VALUE:
				CHECK_SYNTAX(pt_l->desc, OM_S_OCTET_STRING);
				len	 = cert_list_obj->encr_str.d2_size;
				UPDATE_STRING (len,	(cert_list_obj->encr_str.d2_value),
				((OM_descriptor *)pt_l->desc)->value.string.length,
				((OM_descriptor *)pt_l->desc)->value.string.elements);
				if (ret	 == OM_SUCCESS) {
					cert_list_obj->encr_str.d2_size =  len * 8;
				} /* endif */
				break;
				
			case DS_LAST_UPDATE:
				CHECK_SYNTAX(pt_l->desc, OM_S_UTC_TIME_STRING);
				if (cert_list_obj->to_be_signed.last_update != NULL) {
					len	= strlen(cert_list_obj->to_be_signed.last_update);
				} else {
					len = 0;
				} /* endif */
				UPDATE_STRING (len, cert_list_obj->to_be_signed.last_update,
				((OM_descriptor *)pt_l->desc)->value.string.length,
				((OM_descriptor *)pt_l->desc)->value.string.elements);
				break;

			case DS_REVOKED_CERTS:
				{
				OM_descriptor	 * pt_desc;

				for	(i = 0,	pt_desc = pt_l->desc; i < pt_l->nb_val;
				i++, pt_desc++) {
					CHECK_SYNTAX(pt_desc, OM_S_OBJECT);
					if ( cert_list_obj->to_be_signed.rev_certs == NULL) {
						cert_list_obj->to_be_signed.rev_certs  = calloc(
						pt_l->nb_val, sizeof(xds_certificate_sublist));
					} /* endif */
					sublist_obj = (void **) &cert_list_obj
					-> to_be_signed.rev_certs[i];
					REMOVE_OBJ(certificate_sublist, sublist_obj);
					ret = dsP_put_certificate_sublist(workspace,
					(void **)&sublist_obj,
					((OM_descriptor * )pt_desc)->value.object.object,
					object_type, OM_REPLACE_ALL, 0,0, OM_ALL_VALUES);
				} /* endfor */
				} /* endblock */
				number_of_sublists += 1;
				break;

			default:
				ret	 = OM_NO_SUCH_SYNTAX;
			} /* endswitch */

			if (ret	 != OM_SUCCESS) {
				break;
			} /* endif */
		} /* endfor */
		CHECK_END;
	} /* endif */
	TEST_DEL_PUB(ret);
end:
	if (ret == OM_SUCCESS && number_of_sublists > 1) {
		/* combine sublists into first one */
		OM_sint number_of_rev_certs_so_far = 
		cert_list_obj->to_be_signed.rev_certs[0].to_be_signed.no_rev_certs;
		xds_certificate_sublist *to = cert_list_obj->to_be_signed.rev_certs;
		xds_certificate_sublist *fixed_to = to;
		/* loop over sublists after first one */
		for ( i = 1; i < number_of_sublists; i++) {
			OM_uint j;
			OM_uint this_sublist_len = 
			cert_list_obj->to_be_signed.rev_certs[i].to_be_signed.no_rev_certs;
			xds_certificate_sublist *from = &cert_list_obj
			->to_be_signed.rev_certs[i];
			to->to_be_signed.rev_cert = realloc(to->to_be_signed.rev_cert,
			(number_of_rev_certs_so_far + this_sublist_len)
			* sizeof(xds_certificate_subsublist));
			for ( j = 0 ; j < from->to_be_signed.no_rev_certs; j++ ) {
				to->to_be_signed.rev_cert[number_of_rev_certs_so_far]
				= from->to_be_signed.rev_cert[j];
				number_of_rev_certs_so_far++;
			} /* endfor */
		} /* endfor */
		cert_list_obj->to_be_signed.rev_certs[0].to_be_signed.rev_cert 
		= fixed_to->to_be_signed.rev_cert;
		cert_list_obj->to_be_signed.rev_certs[0].to_be_signed.no_rev_certs 
		= number_of_rev_certs_so_far;
	} /* endif */
	*xds_object = cert_list_obj; 
	return(ret);
} /* end dsP_put_certificate_list */

