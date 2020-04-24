/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xomdef.h,v $
 * Revision 1.1.22.2  1996/02/18  23:38:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:31:13  marty]
 *
 * Revision 1.1.22.1  1995/12/08  15:37:28  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:32  root]
 * 
 * Revision 1.1.20.6  1994/07/06  15:09:20  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:25:39  marrek]
 * 
 * Revision 1.1.20.5  1994/06/10  21:20:39  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:06  devsrc]
 * 
 * Revision 1.1.20.4  1994/05/10  16:04:14  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:16:02  marrek]
 * 
 * Revision 1.1.20.3  1994/03/23  15:49:16  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:37:20  keutel]
 * 
 * Revision 1.1.20.2  1994/02/24  15:03:38  marrek
 * 	Change rcs string format.
 * 	[1994/02/24  12:14:15  marrek]
 * 
 * Revision 1.1.20.1  1994/02/22  19:00:03  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:11:52  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:04:11  marrek]
 * 
 * Revision 1.1.18.1  1993/10/14  19:23:56  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:22:54  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  09:54:26  keutel]
 * 
 * $EndLog$
 */
#ifndef _XOMDEF_H
#define _XOMDEF_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xomdef_rcsid[] = "@(#)$RCSfile: xomdef.h,v $ $Revision: 1.1.22.2 $ $Date: 1996/02/18 23:38:07 $";
#endif
/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/
/* @(#)xomdef.h	10.2.1.1 91/12/19 (SNI) */

/**********************************************************************/
/*               COPYRIGHT (C) BULL S.A. 1990 1991 1992               */
/*                        All Rights Reserved                         */
/**********************************************************************/
/*                                                                    */
/*   * This software is furnished under licence and may be used only  */
/*   * in accordance with the terms of that licence and with the      */
/*   * inclusion of the above copyright notice.                       */
/*   * This software may not be provided or otherwise made available  */
/*   * to, or used by, any other person. No title to or ownership of  */
/*   * the software is hereby transferred.                            */
/*                                                                    */
/**********************************************************************/
/*	@(#)xomdef.h	1.13 (Bull S.A) 7/1/92	*/

/**********************************************************************/
/*                                                                    */
/* TYP          : INCLUDE                                             */
/*                                                                    */
/* NAME         : xomdef.h                                            */
/*                                                                    */
/* AUTHOR       : Bull S.A.                                           */
/* DATE         : 1. 10. 1990                                         */
/*                                                                    */
/* COMPONENT    : XOM/XDS                                             */
/*                                                                    */
/* DOC.-REF.    : OSI Object Management API Specification ver. 2.0    */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : declarations of functions and macros used           */
/*                internally by the XDS specific part of the          */ 
/*                implementation of om_get and om_put functions.      */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)@(#)xomdef.h	1.13 (Bull S.A) 7/1/92 */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       |    |         */
/*   1.0    | 90-10-01| Original                       |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/**********************************************************************/


/****************************************************************/
/* Define macros for om_put() function				*/
/****************************************************************/
typedef struct	{
	OM_type		type;
	OM_sint16	max_val;
	OM_sint16	nb_val;
	OM_value_length	max_length;
	void		*desc;
} OMP_type_list;

/* must occur after definition of OMP_type_list */
#include <dsPGet.h>	/* for dsP_validate_type_list */

typedef struct	{
	OM_enumeration	min;
	OM_enumeration	max;
} OMP_enum_limit;

typedef OM_enumeration	OMP_enum_list;

/* initialise initial and limiting values according to the type of     */
/* modification and the number of values (MAX_VAL) of the attribute    */

#define	UPDATE_LIMIT_VAL(MAX_VAL,EXEC)					\
{									\
	ret = OM_SUCCESS;						\
	if (((OMP_REPLACE) && !(OMP_REPLACE_ALL)) && 			\
					(initial_value >= MAX_VAL))	\
		ret = OM_WRONG_VALUE_POSITION;				\
	else if ((OMP_INSERT_AT_END) ||		 			\
		    ((OMP_INSERT) && (initial_value > MAX_VAL)))	\
		initial_value = limiting_value = MAX_VAL;		\
	else if ((limiting_value == OM_ALL_VALUES) ||			\
					(limiting_value > MAX_VAL))	\
		limiting_value = MAX_VAL;				\
	if (ret != OM_SUCCESS)						\
		EXEC;							\
}

/* get the number of objects of a certain type linked in an attribute  */
/* XDS   : XDS structure type                                          */
/* NEXT  : pointer in the structure to the next object                 */
/* NBOBJ : objects number in return                                    */
/* PTINIT: initial pointer to the first object of the list             */
/* PTOBJ : pointer to the current object                               */

#define	GET_NB_OBJ(XDS,NEXT,NBOBJ,PTINIT,PTOBJ)				\
		for (NBOBJ = 0, PTOBJ = PTINIT;				\
		     *PTOBJ != 0 ;					\
		     NBOBJ++,						\
		     PTOBJ = (void *)&((XDS *)*PTOBJ)->NEXT);

/* set the current object pointer to the nth object of a list          */
/* XDS   : XDS structure type                                          */
/* NEXT  : pointer in the structure to the next object                 */
/* NBOBJ : number of object of the list                                */
/* PTINIT: initial pointer to the first object of the list             */
/* PTOBJ : pointer to the current object                               */
/* CNT   : range of the object to get the pointer to                   */

#define	JUMP_N_OBJ(XDS,NEXT,NBOBJ,PTINIT,PTOBJ,CNT)			\
		for (CNT = 0, PTOBJ = PTINIT;				\
		     CNT != NBOBJ;					\
		     CNT++,						\
		     PTOBJ = (void *)&((XDS *)*PTOBJ)->NEXT);

#define INCR_PT_OBJ(XDS,NEXT,NBOBJ,PTOBJ,CNT)				\
		for (CNT = 0; CNT < NBOBJ;				\
		     CNT++,						\
	     	     PTOBJ = (void *)&((XDS *)*PTOBJ)->NEXT);

/* update a boolean value                                              */
/* BOOL : address of the value to update                               */
/* DESC : pointer to the descriptor to get the value from              */

#define UPDATE_BOOLEAN(DESC,BOOL)			\
		BOOL = (DESC)->value.boolean;

/* update an integer value                                             */
/* BOOL : address of the value to update                               */
/* DESC : pointer to the descriptor to get the value from              */

#define UPDATE_INTEGER(DESC,INTG)			\
		INTG = (DESC)->value.integer;

/* update an enumeration value                                         */
/* BOOL : address of the value to update                               */
/* DESC : pointer to the descriptor to get the value from              */

#define UPDATE_ENUM(DESC,ENUM)				\
		ENUM = (DESC)->value.enumeration;

/* check syntax consistency and                                        */
/* update a boolean value                                              */

#define IS_BOOLEAN_AND_UPDATE(DESC,BOOL)		\
	if (OMP_SYNTAX(DESC) != OM_S_BOOLEAN)		\
		ret = OM_WRONG_VALUE_SYNTAX;		\
	else						\
		UPDATE_BOOLEAN (DESC,BOOL);

/* check syntax consistency and                                        */
/* update an integer value                                             */

#define IS_INTEGER_AND_UPDATE(DESC,INTG)		\
	if (OMP_SYNTAX(DESC) != OM_S_INTEGER)		\
		ret = OM_WRONG_VALUE_SYNTAX;		\
	else						\
		UPDATE_INTEGER (DESC,INTG);

/* check syntax consistency and                                        */
/* update an enumeration value                                         */

#define IS_ENUM_AND_UPDATE(DESC,ENUM)			\
	if (OMP_SYNTAX(DESC) != OM_S_ENUMERATION)	\
		ret = OM_WRONG_VALUE_SYNTAX;		\
	else						\
		UPDATE_ENUM (DESC,ENUM);

/***********************************************************************/

#if 0 /* seems to be unused. if confirmed, remove later CC 10.3.94 */
/* Macro to declare variables used in om_put macros */
#define COM_VAR				\
    OM_return_code	ret;		\
    OM_sint			i;		\
    OMP_type_list	*pt_l;		\
    OM_public_object	src_pub;	\
    OM_value_position	desc_nb;	\
    OM_boolean		priv_flag;	\
    OM_sint			check;

#define OBJ_VAR				\
    void	*link;			\
    void	**pt_obj;		\
    void	*pt;

#endif /* 0  seems to be unused */

#define	OMP_REMOVE	(modification == OMP_REMOVE_VALUES)

#define	VALUE_NOT_PRESENT	(pt_l->max_val == 0)

/* check syntax consistency and                                        */
/* update a boolean value                                              */

#define CHECK_BOOLEAN_AND_UPDATE(DESC,BOOL)		\
{							\
	CHECK_SYNTAX(DESC, OM_S_BOOLEAN);		\
	UPDATE_BOOLEAN (DESC,BOOL);			\
}

/* check syntax consistency and                                        */
/* update an integer value                                             */

#define CHECK_INTEGER_AND_UPDATE(DESC,INTG)		\
{							\
	CHECK_SYNTAX(DESC, OM_S_INTEGER);		\
	UPDATE_INTEGER (DESC,INTG);			\
}

/* check syntax consistency and                                        */
/* update an enumeration value                                         */

#define CHECK_ENUM_AND_UPDATE(DESC,ENUM)		\
{							\
	CHECK_SYNTAX(DESC, OM_S_ENUMERATION);		\
	UPDATE_ENUM (DESC,ENUM);			\
}

#define	VALIDATE_ENUM(DESC,LIMIT)					\
{									\
	if (((OM_descriptor *)DESC)->value.enumeration < LIMIT.min ||	\
	    ((OM_descriptor *)DESC)->value.enumeration > LIMIT.max)	\
	{								\
		ret = OM_WRONG_VALUE_MAKEUP;				\
		break;							\
	}								\
}

/* check enumeration syntax and value validity for an optional         */
/* attribute (remove permitted)                                        */

#define CHECK_VALID_OPT_ENUM(DESC,LIMIT)				\
{									\
    if (check)								\
    {									\
	VALIDATE_SYNTAX(DESC,OM_S_ENUMERATION);				\
	if (!OMP_REMOVE)						\
		VALIDATE_ENUM(DESC,LIMIT);				\
    break;								\
    }									\
}

/* check syntax consistency, enumeration validity and                  */
/* update an enumeration value (optional attribute)                    */

#define CHECK_VALID_OPT_ENUM_AND_UPDATE(DESC,ENUM,LIMIT)	\
{								\
	CHECK_VALID_OPT_ENUM(DESC, LIMIT);			\
	UPDATE_ENUM (DESC,ENUM);				\
}

/* check enumeration syntax and value validity for a mandatory         */
/* attribute (remove not permitted)                                    */

#define CHECK_VALID_ENUM(DESC,LIMIT)					\
{									\
    if (check)								\
    {									\
	VALIDATE_SYNTAX(DESC,OM_S_ENUMERATION);				\
	if (OMP_REMOVE)							\
	{								\
		ret = OM_WRONG_VALUE_NUMBER;				\
		break;							\
	}								\
	VALIDATE_ENUM(DESC,LIMIT);					\
    break;								\
    }									\
}

/* check syntax consistency, enumeration validity and                  */
/* update an enumeration value (mandatory attribute)                   */

#define CHECK_VALID_ENUM_AND_UPDATE(DESC,ENUM,LIMIT)	\
{							\
	CHECK_VALID_ENUM(DESC, LIMIT);			\
	UPDATE_ENUM (DESC,ENUM);			\
}

#define UPDATE_STRING(LEN,STRPT,NLEN,NSTR)				\
{									\
	ret = dsP_update_string((OM_sint *)&LEN,(void *)&STRPT,NLEN,NSTR,	\
			modification,workspace);			\
}

/* remove an object from a linked list of object */
#define	REMOVE_OBJ(DEL,OBJ)						\
{									\
	if (OMP_REMOVE)							\
	{								\
		dsP_del_##DEL(workspace,*OBJ);				\
		*OBJ = 0;						\
		ret = OM_SUCCESS;					\
		break;							\
	}								\
}

#define	TEST_DEL_PUB(RET)						\
{									\
	if (priv_flag)							\
		om_delete (src_pub);					\
	ret =  RET;							\
	goto end;				\
}

/* allocate a new object structure */
#define	ALLOCATE_OBJECT(CLASS,TYPE)					\
{									\
    if (!*xds_object)							\
    {									\
	if ((*xds_object = (void *)OMP_XALLOC(workspace,AL_XDS_##CLASS,1)) \
					== (void *)0)			\
	{								\
		TEST_DEL_PUB(OM_MEMORY_INSUFFICIENT);			\
	}								\
	/* clear xds structure */					\
	memset (*xds_object, 0, sizeof(TYPE));				\
    }									\
}

/* macros to check correctness of syntaxes before beginning
 * the modification of the destination object
 */

#define	CHECK_INIT							\
	check = 2;							\
	while (check--)							\
	{

#define	CHECK_END							\
	    if (ret != OM_SUCCESS)					\
		break;							\
	}

#define CHECK								\
{									\
	if (check)							\
		break;							\
}

#define VALIDATE_SYNTAX(DESC,SYNTAX)					\
{									\
	if (OMP_SYNTAX(DESC) != SYNTAX)					\
	{								\
		ret = OM_WRONG_VALUE_SYNTAX;				\
		break;							\
	}								\
}

#define	VALIDATE_LENGTH(DESC,MIN,LENGTH)				\
{									\
	str_len = ((OM_descriptor *)DESC)->value.string.length;		\
	str_elem = ((OM_descriptor *)DESC)->value.string.elements;	\
	if (str_len == OM_LENGTH_UNSPECIFIED)				\
	    str_len = (str_elem == NULL ? 0 : strlen(str_elem));	\
	if ((str_len > (LENGTH == 0 ? MAX_LENGTH : LENGTH)) ||		\
	    (str_len < MIN))						\
	{								\
	    ret = OM_WRONG_VALUE_LENGTH;				\
	    break;							\
	}								\
}

/*
#define	VALIDATE_LENGTH(DESC,LENGTH)					\
{									\
	if ( LENGTH != 0 && LENGTH != MAX_LENGTH )			\
	{								\
	    if (((OM_descriptor *)DESC)->value.string.length > LENGTH)	\
	    {								\
		ret = OM_WRONG_VALUE_LENGTH;				\
		break;							\
	    }								\
	}								\
}
*/

#define VALIDATE_SYNTAX_AND_LENGTH(DESC,SYNTAX,MIN,LENGTH)		\
{									\
	VALIDATE_SYNTAX(DESC,SYNTAX);					\
	VALIDATE_LENGTH(DESC,MIN,LENGTH);				\
}

#define VALIDATE_SYNTAX_MUL_VAL(PT,SYNTAX)				\
{									\
	OM_sint		loc_i;						\
	OM_descriptor	*loc_desc;					\
    for (loc_i = 0, loc_desc = PT->desc;				\
	 loc_i < PT->nb_val; loc_i++, loc_desc++)			\
    {									\
	VALIDATE_SYNTAX(loc_desc,SYNTAX);				\
    }									\
    if (ret != OM_SUCCESS)						\
	break;								\
}

#define VALIDATE_SYNTAX_AND_LENGTH_MUL_VAL(PT,SYNTAX,MIN,LENGTH)	\
{									\
OM_sint		loc_i;						\
OM_descriptor	*loc_desc;					\
    for (loc_i = 0, loc_desc = PT->desc;				\
	 loc_i < PT->nb_val; loc_i++, loc_desc++)			\
    {									\
	VALIDATE_SYNTAX(loc_desc,SYNTAX);				\
	VALIDATE_LENGTH(loc_desc,MIN,LENGTH);				\
    }									\
    if (ret != OM_SUCCESS)						\
	break;								\
}

#define CHECK_SYNTAX(DESC,SYNTAX)					\
{									\
    if (check)								\
    {									\
	VALIDATE_SYNTAX(DESC,SYNTAX);					\
    break;								\
    }									\
}

/* check syntax and length for optional attributes */

#define CHECK_OPT_SYNTAX_AND_LENGTH(DESC,SYNTAX,MIN,LENGTH)		\
{									\
    if (check)								\
    {									\
	VALIDATE_SYNTAX(DESC,SYNTAX);					\
	if (!OMP_REMOVE)						\
	    VALIDATE_LENGTH(DESC,MIN,LENGTH);				\
    break;								\
    }									\
}

/* check syntax and length for mandatory attributes */

#define CHECK_SYNTAX_AND_LENGTH(DESC,SYNTAX,MIN,LENGTH)			\
{									\
    if (check)								\
    {									\
	VALIDATE_SYNTAX(DESC,SYNTAX);					\
	if (OMP_REMOVE)							\
	{								\
		ret = OM_WRONG_VALUE_NUMBER;				\
		break;							\
	}								\
	VALIDATE_LENGTH(DESC,MIN,LENGTH);				\
    break;								\
    }									\
}

#define CHECK_SYNTAX_MUL_VAL(PT,SYNTAX)					\
{									\
    if (check)								\
    {									\
	OM_sint		loc_i;					\
	OM_descriptor	*loc_desc;				\
	for (loc_i = 0, loc_desc = PT->desc;				\
		 loc_i < PT->nb_val; loc_i++, loc_desc++)		\
	{								\
	    VALIDATE_SYNTAX(loc_desc,SYNTAX);				\
	}								\
    break;								\
    }									\
}

#define CHECK_SYNTAX_AND_LENGTH_MUL_VAL(PT,SYNTAX,MIN,LENGTH)		\
{									\
    if (check)								\
    {									\
	OM_sint		loc_i;					\
	OM_descriptor	*loc_desc;				\
	for (loc_i = 0, loc_desc = PT->desc;				\
		 loc_i < PT->nb_val; loc_i++, loc_desc++)		\
	{								\
	    VALIDATE_SYNTAX(loc_desc,SYNTAX);				\
	    VALIDATE_LENGTH(loc_desc,MIN,LENGTH);			\
	}								\
    break;								\
    }									\
}

/* check two syntaxes and length for optional attributes */

#define CHECK_OPT_TWO_SYNTAXES_AND_LENGTH(DESC,STX1,STX2,MIN,LENGTH)	\
{									\
    if (check)								\
    {									\
	if ((OMP_SYNTAX(DESC) != STX1) && (OMP_SYNTAX(DESC) != STX2))	\
	{								\
		ret = OM_WRONG_VALUE_SYNTAX;				\
		break;							\
	}								\
	if (!OMP_REMOVE)						\
	    VALIDATE_LENGTH(DESC,MIN,LENGTH);				\
    break;								\
    }									\
}

/* check two syntaxes and length for mandatory attributes */

#define CHECK_TWO_SYNTAXES_AND_LENGTH(DESC,STX1,STX2,MIN,LENGTH)	\
{									\
    if (check)								\
    {									\
	if ((OMP_SYNTAX(DESC) != STX1) && (OMP_SYNTAX(DESC) != STX2))	\
	{								\
		ret = OM_WRONG_VALUE_SYNTAX;				\
		break;							\
	}								\
	VALIDATE_LENGTH(DESC,MIN,LENGTH);				\
    break;								\
    }									\
}

#define	VALIDATE_TYPE_LIST(XDSLIST,INCTYPE)				\
	if ((ret = dsP_validate_type_list(XDSLIST,INCTYPE)) != OM_SUCCESS) {	\
			goto end;								\
		} /* endif */												\

#define	VALIDATE_TYPE_AND_MOD(XDSLIST,INCTYPE,INIT,LIMIT)		\
	if ((ret = dsP_validate_type_and_mod(XDSLIST,INCTYPE,INIT,LIMIT))	\
							!= OM_SUCCESS) {	\
		goto end;			\
	} /* endif */

#define PUB_TYPE_IN_LIST(LIST,DESC)					\
	dsP_pub_type_in_list(LIST, 						\
		    		modification,				\
		    		included_types,				\
				initial_value,				\
				limiting_value,				\
				DESC)


/* insert a new object structure in a linked object list */
#define INSERT_OBJ(TYPE,OBJ,NBOBJ,NEXT,PUT)				\
    {									\
		/* allocate new structures 				\
		 * put the source values in them			\
		 * chain in the destination				\
		 */							\
		ret = OM_SUCCESS;					\
		if (OMP_REMOVE)						\
			pt_l->nb_val = 0;				\
		for (i = 0, desc = (OM_descriptor *)pt_l->desc;		\
		     i < NBOBJ; i++, desc++,				\
		     pt_obj = (void *)&((OBJ *)*pt_obj)->NEXT)		\
		{							\
			/* allocate XDS structure */			\
			if ((pt = (void *)OMP_XALLOC(workspace,		\
					AL_XDS_##TYPE,1))		\
						== (void *)0)		\
			{						\
				ret = OM_MEMORY_INSUFFICIENT;		\
				break;					\
			}						\
			/* clear XDS structure */			\
			memset (pt, 0, sizeof(OBJ));			\
			/* call PUT function				\
			 * with REPLACE_ALL modification 		\
			 */						\
			if ((ret = PUT (workspace,&pt,			\
				     desc->value.object.object,		\
				     object_type, OM_REPLACE_ALL,	\
				     0, 0, OM_ALL_VALUES)) 		\
					!= OM_SUCCESS)			\
				break;					\
			link = *pt_obj;					\
			*pt_obj = pt;					\
			((OBJ *)pt)->NEXT = link;			\
		}							\
		if (ret != OM_SUCCESS)					\
			break;						\
    }

/* delete an object structure from a linked object list */
#define DELETE_OBJ(OBJ,NBOBJ,NEXT,DEL)					\
    {									\
	ret = OM_SUCCESS;						\
	for (i = 0; i < NBOBJ; i++)					\
	{								\
		link = (void *) ((OBJ *)*pt_obj)->NEXT;			\
		if ((ret = DEL (workspace,*pt_obj)) != OM_SUCCESS)	\
			break;						\
		*pt_obj = link;						\
	}								\
	if (ret != OM_SUCCESS)						\
		break;							\
    }


/* macro to change, if necessary, a private object to a public object,
 * calling om_get() function with included_types list
 */
#define	EXCLUDE_SUB_OBJECTS	OM_FALSE
#define	NO_EXCLUDE_SUB_OBJECTS	OM_TRUE

#define	TEST_SOURCE_PRIV(EXCLUDE)					\
{									\
    ret = dsP_test_source_priv(source, included_types, 			\
			&src_pub, &desc_nb, &priv_flag,EXCLUDE);	\
    if (priv_flag)							\
    {									\
	if (ret != OM_SUCCESS) {						\
		goto end;							\
	} /* endif */				\
	if ((desc_nb == 0) || 						\
		((desc_nb == 1) && (OMP_TYPE(src_pub) == OM_CLASS)))	\
	{								\
	    om_delete(src_pub);						\
		goto end;					\
	}								\
	source = src_pub;						\
    }									\
}
/***********************************************************************/

/********************************************************/
/*		om_get() macros				*/
/********************************************************/
#define	OMP_EXCLUDE_DESCRIPTORS	(exclusions & OM_EXCLUDE_DESCRIPTORS)

#define	OMP_EXCLUDE_MULTIPLES(nbvalues)					\
		((exclusions & OM_EXCLUDE_MULTIPLES) && (nbvalues >1))	

#define	OMP_EXCLUDE_VALUES	(exclusions & OM_EXCLUDE_VALUES)

#define	OMP_EX_ALL_BUT_CERTAIN_VALUES				\
			  (exclusions & OM_EXCLUDE_ALL_BUT_THESE_VALUES)

#define	OMP_EX_ALL_BUT_CERTAIN_TYPES				\
			  (exclusions & OM_EXCLUDE_ALL_BUT_THESE_TYPES)

#define	OMP_EXCLUDE_SUBOBJECTS	(exclusions & OM_EXCLUDE_SUBOBJECTS)


#define	UPDATE_DESC_TYPE_AND_SYNTAX(DESC,TYPE,SYNTAX)	\
{						\
	DESC->type = TYPE;			\
	DESC->syntax = SYNTAX;			\
	DESC++;					\
}

#define	UPDATE_DESC_OBJ(DESC,TYPE,VALUE,SYNTAX)	\
{						\
	DESC->type = TYPE;			\
	DESC->syntax = SYNTAX;			\
	DESC->value.object.object = VALUE;	\
	DESC++;					\
	OMP_SET_SUBOBJECT(VALUE);		\
}
#define	UPDATE_LIMIT_VALUE(MAX_VAL)					\
{									\
   if (OMP_EXCLUDE_MULTIPLES(MAX_VAL)) 					\
   {									\
  	initial_value = 0;						\
	pt_l->nb_val = 1;						\
   }									\
   else									\
   {									\
	if (OMP_EX_ALL_BUT_CERTAIN_VALUES ) 			\
	{								\
	   if ((initial_value == OM_ALL_VALUES) || (initial_value >= MAX_VAL)) {\
			ret = OM_NOT_PRESENT;					\
			goto end;									\
		} /* endif */									\
	   if ((limiting_value == OM_ALL_VALUES) ||	(limiting_value > MAX_VAL))	\
			limiting_value = MAX_VAL;				\
    	pt_l->nb_val = limiting_value - initial_value;		\
	}								\
	else								\
	{								\
	  pt_l->nb_val = MAX_VAL;					\
  	  initial_value = 0;						\
	  limiting_value = MAX_VAL;					\
	}								\
   }									\
}
#define	UPDATE_DESC_BOOL(DESC,TYPE,VALUE,MAX_VAL)	\
{							\
	if ((!OMP_EXCLUDE_VALUES) &&			\
		(!OMP_EXCLUDE_MULTIPLES(MAX_VAL)))	\
		DESC->value.boolean = VALUE;		\
	else DESC->syntax = OM_S_NO_VALUE;		\
	DESC->type = TYPE;				\
	DESC->syntax |= OM_S_BOOLEAN;			\
	DESC++;						\
}

#define	UPDATE_DESC_ENUM(DESC,TYPE,VALUE,MAX_VAL)	\
{							\
	if ((!OMP_EXCLUDE_VALUES) &&			\
		(!OMP_EXCLUDE_MULTIPLES(MAX_VAL)))	\
		DESC->value.enumeration = VALUE;	\
	else DESC->syntax = OM_S_NO_VALUE;		\
	DESC->type = TYPE;				\
	DESC->syntax |= OM_S_ENUMERATION;		\
	DESC++;						\
}

#define	UPDATE_DESC_INT(DESC,TYPE,VALUE,MAX_VAL)	\
{							\
	if ((!OMP_EXCLUDE_VALUES) &&			\
		(!OMP_EXCLUDE_MULTIPLES(MAX_VAL)))	\
		DESC->value.integer = VALUE;		\
	else DESC->syntax = OM_S_NO_VALUE;		\
	DESC->type = TYPE;				\
	DESC->syntax |= OM_S_INTEGER;			\
	DESC++;						\
}
#define	UPDATE_DESC_STR(DESC,TYPE,SYNTAX,LEN,PTR,MAX_VAL)		\
{									\
    ret = OM_SUCCESS;							\
    if ((!OMP_EXCLUDE_VALUES) &&					\
		(!OMP_EXCLUDE_MULTIPLES(MAX_VAL)))			\
    {									\
 	if ((DESC->value.string.elements = 				\
		(void *)OMP_XALLOC_STR(workspace,(char *)PTR,LEN)) != 0) \
	    DESC->value.string.length = LEN;				\
	else 								\
	    ret = OM_MEMORY_INSUFFICIENT;				\
    }									\
    else DESC->syntax = OM_S_NO_VALUE;					\
    if (ret == OM_SUCCESS)						\
    {									\
    	DESC->type = TYPE;						\
	DESC->syntax |= SYNTAX;						\
    	DESC++;								\
    }									\
}


#define UPDATE_DESC_CLASS(CLASS)				\
{								\
		UPDATE_DESC_STR(ptr_desc,			\
		OM_CLASS,OM_S_OBJECT_IDENTIFIER_STRING,		\
		CLASS.length,					\
		CLASS.elements,1);				\
}



#define OBJECT_VALUE(GET,CLASS,XDS_OBJ,TYPE,MAX_VAL)			\
{									\
	OM_object	ptr_han;					\
	OM_syntax	syntax;						\
	OM_value_position nb;						\
	if ((!OMP_EXCLUDE_VALUES) && (!OMP_EXCLUDE_MULTIPLES(MAX_VAL)))	{ \
		if (OMP_EXCLUDE_SUBOBJECTS) {								\
			ret = omP_alloc_sub_handle(original,CLASS,			\
			XDS_OBJ,&ptr_han);					\
			syntax = OM_S_OBJECT | OM_S_PRIVATE;			\
		} else {								\
			ret = dsP_get_##GET(workspace,original,XDS_OBJ,		\
			0,0,local_strings,0,0,&ptr_han,&nb);		\
			syntax = OM_S_OBJECT ;					\
		} /* endif */								\
		if (ret == OM_SUCCESS) {					\
			UPDATE_DESC_OBJ(ptr_desc,TYPE,ptr_han,syntax);		\
		} /* endif */								\
	} else {								\
        UPDATE_DESC_TYPE_AND_SYNTAX(ptr_desc,TYPE, OM_S_OBJECT | OM_S_NO_VALUE);	\
	} /* endif */								\
}

#define INC_NBDESC_MONO					\
{							\
	if (!initialize) {						\
		if ((OMP_EX_ALL_BUT_CERTAIN_VALUES) && (!(OMP_REPLACE_SINGLE_VALUE))) {\
			ret = OM_NOT_PRESENT;		\
			goto end;						\
		} /* endif */				\
	    nb_desc++;					\
	    break;					\
	} /* endif */						\
}

#define clean_before_exit(DESC0,CURDESC,NBDESC,RET)	\
{							\
	dsP_free_resources(workspace,DESC0,CURDESC,NBDESC);	\
	ret = RET;					\
	goto end;						\
}

#endif /* _XOMDEF_H */
