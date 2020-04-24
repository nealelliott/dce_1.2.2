/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: xdsdef.h,v $
 * Revision 1.1.35.2  1996/02/18  23:37:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:31  marty]
 *
 * Revision 1.1.35.1  1995/12/08  15:36:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:20  root]
 * 
 * Revision 1.1.33.3  1994/05/10  16:04:07  marrek
 * 	Bug fix for April 1994 submission.
 * 	[1994/05/09  15:26:55  marrek]
 * 
 * Revision 1.1.33.2  1994/03/23  15:49:07  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:37:03  keutel]
 * 
 * Revision 1.1.33.1  1994/02/22  19:23:54  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:11:07  marrek]
 * 
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:02:31  marrek]
 * 
 * Revision 1.1.31.1  1993/10/14  19:23:44  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  19:22:38  keutel]
 * 
 * 	October 1993 code drop
 * 	[1993/10/11  10:02:38  keutel]
 * 
 * Revision 1.1.2.2  1993/08/10  07:10:31  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:20:03  marrek]
 * 
 * Revision 1.1.4.3  1992/12/30  21:17:29  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:05:56  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/30  08:38:08  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:45:13  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:20:00  melman
 * 	New GDS merged drop
 * 	[1992/05/29  12:47:01  melman]
 * 
 * $EndLog$
 */
#ifndef _XDSDEF_H
#define _XDSDEF_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xdsdef_rcsid[] = "@(#)$RCSfile: xdsdef.h,v $ $Revision: 1.1.35.2 $ $Date: 1996/02/18 23:37:52 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/
/* @(#)xdsdef.h	10.2.1.1 91/12/19 (SNI) */

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
/*	@(#)xdsdef.h	1.9 (Bull S.A) 4/27/92	*/

/**********************************************************************/
/*                                                                    */
/* TYP          : INCLUDE                                             */
/*                                                                    */
/* NAME         : xdsdef.h                                            */
/*                                                                    */
/* AUTHOR       : Bull S.A.                                           */
/* DATE         : 1. 10. 1990                                         */
/*                                                                    */
/* COMPONENT    : XOM/XDS                                             */
/*                                                                    */
/* DOC.-REF.    : X/Open Directory Services API Specification ver. 1.0*/
/*              : OSI Object Management API Specification ver. 2.0    */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  : Definition, for all OM classes that constitute XDS, */
/*                of all OM attributes that may be found              */
/*                in an instance of an OM class.                      */
/*                                                                    */
/*                                                                    */
/* SYSTEM DEPENDENCIES:                                               */
/*                                                                    */
/* HISTORY      : sccsid  =  @(#)@(#)xdsdef.h	1.7 (Bull S.A) 8/29/91 */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       |    |         */
/*   1.0    | 90-10-01| Original                       |    |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/**********************************************************************/

/* the following _TYPE_LIST defines are of type OMP_type_list	*/
/* defined in xomdef.h, which has the following structure: */

/*			typedef struct  {									*/
/*				OM_type     type;								*/
/*				OM_sint16   max_val;							*/
/*				OM_sint16   nb_val;								*/
/*				OM_value_length max_length;						*/
/*				void        *desc;								*/
/*			} OMP_type_list;									*/


/********************************************************/
/*	SERVICE PACKAGE attributes names		*/
/********************************************************/
#define	PSAP_TYPE_LIST 					\
		{{OM_CLASS,1,0,0},			\
		{DS_N_ADDRESSES,MAX_VALUES,0,0},	\
		{DS_P_SELECTOR,1,0,0},			\
		{DS_S_SELECTOR,1,0,0},			\
		{DS_T_SELECTOR,1,0,0},			\
		{OM_NO_MORE_TYPES,0,0,0}}

#define	RDN_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DS_AVAS,D2_NO_AVA,0,0},		\
		{OM_NO_MORE_TYPES,0,0,0}}		
			
#define	DN_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DS_RDNS,D2_NP_MAX,0,0},		\
		{OM_NO_MORE_TYPES,0,0,0}}		
			
#define	AVA_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DS_ATTRIBUTE_TYPE,1,0,0},		\
		{DS_ATTRIBUTE_VALUES,1,0,0},		\
		{OM_NO_MORE_TYPES,0,0,0}}		
			
#define	A_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DS_ATTRIBUTE_TYPE,1,0,0},		\
		{DS_ATTRIBUTE_VALUES,MAX_VALUES,0,0},	\
		{OM_NO_MORE_TYPES,0,0,0}}		
			
#define	A_L_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DS_ATTRIBUTES,MAX_VALUES,0,0},		\
		{OM_NO_MORE_TYPES,0,0,0}}		
			
#define	EXT_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DS_CRIT,1,0,0},			\
		{DS_IDENT,1,0,0},			\
		{DS_ITEM_PARAMETERS,1,0,0},		\
		{OM_NO_MORE_TYPES,0,0,0}}		
			
#define	CTX_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
	        {DS_EXT,MAX_VALUES,0,0},		\
	        {DS_OPERATION_PROGRESS,1,0,0},		\
	        {DS_ALIASED_RDNS,1,0,0},		\
	        {DS_CHAINING_PROHIB,1,0,0},		\
	        {DS_DONT_DEREFERENCE_ALIASES,1,0,0},	\
	        {DS_DONT_USE_COPY,1,0,0},		\
	        {DS_LOCAL_SCOPE,1,0,0},			\
	        {DS_PREFER_CHAINING,1,0,0},		\
	        {DS_PRIORITY,1,0,0},			\
	        {DS_SCOPE_OF_REFERRAL,1,0,0},		\
	        {DS_SIZE_LIMIT,1,0,0},			\
	        {DS_TIME_LIMIT,1,0,0},			\
	        {DS_ASYNCHRONOUS,1,0,0},		\
	        {DS_AUTOMATIC_CONTINUATION,1,0,0},	\
		{OM_NO_MORE_TYPES,0,0,0}}		
			
#define	E_I_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DS_FROM_ENTRY,1,0,0},			\
		{DS_OBJECT_NAME,1,0,0},			\
		{DS_ATTRIBUTES,MAX_VALUES,0,0},		\
		{OM_NO_MORE_TYPES,0,0,0}}		
				
#define	EIS_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DS_ALL_ATTRIBUTES,1,0,0},		\
		{DS_ATTRIBUTES_SELECTED,MAX_VALUES,0,0},\
		{DS_INFO_TYPE,1,0,0},		\
		{OM_NO_MORE_TYPES,0,0,0}}		
			
#define	E_M_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DS_MOD_TYPE,1,0,0},		\
		{DS_ATTRIBUTE_TYPE,1,0,0},		\
		{DS_ATTRIBUTE_VALUES,MAX_VALUES,0,0},	\
		{OM_NO_MORE_TYPES,0,0,0}}		
			
#define	EML_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DS_CHANGES,MAX_VALUES,0,0},		\
		{OM_NO_MORE_TYPES,0,0,0}}		
			
#define	F_I_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DS_FILTER_ITEM_TYPE,1,0,0},		\
		{DS_FINAL_SUBSTRING,1,0,0},		\
		{DS_INITIAL_SUBSTRING,1,0,0},		\
		{DS_ATTRIBUTE_TYPE,1,0,0},		\
		{DS_ATTRIBUTE_VALUES,MAX_VALUES,0,0},	\
		{OM_NO_MORE_TYPES,0,0,0}}		
			
#define	F_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DS_FILTER_ITEMS,MAX_VALUES,0,0},	\
		{DS_FILTERS,MAX_VALUES,0,0},		\
		{DS_FILTER_TYPE,1,0,0},			\
		{OM_NO_MORE_TYPES,0,0,0}}		
			
#define	SES_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DS_DSA_ADDRESS,1,0,0},			\
		{DS_DSA_NAME,1,0,0},			\
		{DS_FILE_DESCRIPTOR,1,0,0},		\
		{DS_REQUESTOR,1,0,0},			\
		{OM_NO_MORE_TYPES,0,0,0}}		
			
#define	ACC_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DS_ADDRESS,1,0,0},			\
		{DS_AE_TITLE,1,0,0},			\
		{OM_NO_MORE_TYPES,0,0,0}}		

#define COM_RES_TYPE_LIST				\
		{DS_ALIAS_DEREFERENCED,1,0,0},		\
		{DS_PERFORMER,1,0,0}			

#define CMP_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DS_FROM_ENTRY,1,0,0},			\
		{DS_MATCHED,1,0,0},			\
		{DS_OBJECT_NAME,1,0,0},			\
		COM_RES_TYPE_LIST,			\
		{OM_NO_MORE_TYPES,0,0,0}}		

#define CONT_REF_TYPE_LIST				\
		{{OM_CLASS,1,0,0},			\
		{DS_ACCESS_POINTS,MAX_VALUES,0,0},	\
		{DS_ALIASED_RDNS,1,0,0},		\
		{DS_OPERATION_PROGRESS,1,0,0},		\
		{DS_RDNS_RESOLVED,1,0,0},		\
		{DS_TARGET_OBJECT,1,0,0},		\
		{OM_NO_MORE_TYPES,0,0,0}}		

#define OP_PROG_TYPE_LIST				\
		{{OM_CLASS,1,0,0},			\
		{DS_NAME_RESOLUTION_PHASE,1,0,0},	\
		{DS_NEXT_RDN_TO_BE_RESOLVED,1,0,0},	\
		{OM_NO_MORE_TYPES,0,0,0}}		

#define LIST_RES_TYPE_LIST					\
		{{OM_CLASS,1,0,0},				\
		{DS_LIST_INFO,1,0,0},				\
		{DS_UNCORRELATED_LIST_INFO,MAX_VALUES,0,0},	\
		{OM_NO_MORE_TYPES,0,0,0}}		

#define LIST_INF_TYPE_LIST				\
		{{OM_CLASS,1,0,0},			\
		{DS_OBJECT_NAME,1,0,0},			\
		{DS_PARTIAL_OUTCOME_QUAL,1,0,0},	\
		{DS_SUBORDINATES,MAX_VALUES,0,0},	\
		COM_RES_TYPE_LIST,			\
		{OM_NO_MORE_TYPES,0,0,0}}		

#define LIST_INF_I_TYPE_LIST				\
		{{OM_CLASS,1,0,0},			\
		{DS_ALIAS_ENTRY,1,0,0},			\
		{DS_FROM_ENTRY,1,0,0},			\
		{DS_RDN,1,0,0},				\
		{OM_NO_MORE_TYPES,0,0,0}}		

#define	PART_OUT_Q_TYPE_LIST					\
		{{OM_CLASS,1,0,0},				\
		{DS_LIMIT_PROBLEM,1,0,0},			\
		{DS_UNAVAILABLE_CRIT_EXT,1,0,0},	\
		{DS_UNEXPLORED,MAX_VALUES,0,0},			\
		{OM_NO_MORE_TYPES,0,0,0}}		

#define READ_RES_TYPE_LIST				\
		{{OM_CLASS,1,0,0},			\
		{DS_ENTRY,1,0,0},			\
		COM_RES_TYPE_LIST,			\
		{OM_NO_MORE_TYPES,0,0,0}}		

#define SRCH_RES_TYPE_LIST					\
		{{OM_CLASS,1,0,0},				\
		{DS_SEARCH_INFO,MAX_VALUES,0,0},		\
		{DS_UNCORRELATED_SEARCH_INFO,MAX_VALUES,0,0},	\
		{OM_NO_MORE_TYPES,0,0,0}}		

#define SRCH_INF_TYPE_LIST				\
		{{OM_CLASS,1,0,0},			\
		{DS_ENTRIES,MAX_VALUES,0,0},		\
		{DS_OBJECT_NAME,1,0,0},			\
		{DS_PARTIAL_OUTCOME_QUAL,1,0,0},	\
		COM_RES_TYPE_LIST,			\
		{OM_NO_MORE_TYPES,0,0,0}}		

#define ERR_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DS_PROBLEM,1,0,0},			\
		{OM_NO_MORE_TYPES,0,0,0}}		

#define A_ERR_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DS_OBJECT_NAME,1,0,0},			\
		{DS_PROBLEMS,MAX_VALUES,0,0},		\
		{OM_NO_MORE_TYPES,0,0,0}}		

#define A_PB_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DS_ATTRIBUTE_TYPE,1,0,0},		\
		{DS_ATTRIBUTE_VALUE,1,0,0},		\
		{DS_PROBLEM,1,0,0},			\
		{OM_NO_MORE_TYPES,0,0,0}}		

#define	N_ERR_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DS_MATCHED,1,0,0},			\
		{DS_PROBLEM,1,0,0},			\
		{OM_NO_MORE_TYPES,0,0,0}}		

/********************************************************/
/*		 DSX attributes names			*/
/********************************************************/
#define	CGX_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
	        {DS_EXT,MAX_VALUES,0,0},		\
	        {DS_OPERATION_PROGRESS,1,0,0},		\
	        {DS_ALIASED_RDNS,1,0,0},		\
	        {DS_CHAINING_PROHIB,1,0,0},		\
	        {DS_DONT_DEREFERENCE_ALIASES,1,0,0},	\
	        {DS_DONT_USE_COPY,1,0,0},		\
	        {DS_LOCAL_SCOPE,1,0,0},			\
	        {DS_PREFER_CHAINING,1,0,0},		\
	        {DS_PRIORITY,1,0,0},			\
	        {DS_SCOPE_OF_REFERRAL,1,0,0},		\
	        {DS_SIZE_LIMIT,1,0,0},			\
	        {DS_TIME_LIMIT,1,0,0},			\
	        {DS_ASYNCHRONOUS,1,0,0},		\
	        {DS_AUTOMATIC_CONTINUATION,1,0,0},	\
		{DSX_DUAFIRST,1,0,0},			\
		{DSX_DONT_STORE,1,0,0},			\
		{DSX_NORMAL_CLASS,1,0,0},		\
		{DSX_PRIV_CLASS,1,0,0},			\
		{DSX_RESIDENT_CLASS,1,0,0},		\
		{DSX_USEDSA,1,0,0},			\
		{DSX_DUA_CACHE,1,0,0},			\
		{DSX_PREFER_ADM_FUNCS,1,0,0},           \
		{DSX_SIGN_MECHANISM,1,0,0},             \
		{DSX_PROT_REQUEST,1,0,0},               \
		{OM_NO_MORE_TYPES,0,0,0}}		
			
#define	SGS_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DS_DSA_ADDRESS,1,0,0},			\
		{DS_DSA_NAME,1,0,0},			\
		{DS_FILE_DESCRIPTOR,1,0,0},		\
		{DS_REQUESTOR,1,0,0},			\
		{DSX_PASSWORD,1,0,0},			\
		{DSX_DIR_ID,1,0,0},			\
		{DSX_AUTH_MECHANISM,1,0,0},		\
		{DSX_AUTH_INFO,1,0,0},			\
		{OM_NO_MORE_TYPES,0,0,0}}		
			
#define	ACL_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DSX_MODIFY_PUBLIC,4,0,0},		\
		{DSX_READ_STANDARD,4,0,0},		\
		{DSX_MODIFY_STANDARD,4,0,0},		\
		{DSX_READ_SENSITIVE,4,0,0},		\
		{DSX_MODIFY_SENSITIVE,4,0,0},		\
		{OM_NO_MORE_TYPES,0,0,0}}		
			
#define	ACI_TYPE_LIST					\
		{{OM_CLASS,1,0,0},			\
		{DSX_INTERPRETATION,1,0,0},		\
		{DSX_USER,1,0,0},			\
		{OM_NO_MORE_TYPES,0,0,0}}		
			
/********************************************************/
/*		BDCP attributes names			*/
/********************************************************/
#define	FAX_TYPE_LIST							\
		{{OM_CLASS,1,0,0},					\
		{DS_PARAMETERS,1,0,0},					\
		{DS_PHONE_NBR,1,0,DS_VL_PHONE_NBR,0},			\
		{OM_NO_MORE_TYPES,0,0,0}}		

#define	POST_TYPE_LIST							\
		{{OM_CLASS,1,0,0},					\
		{DS_POSTAL_ADDRESS,DS_VN_POSTAL_ADDRESS,0,DS_VL_POSTAL_ADDRESS,0},		\
		{OM_NO_MORE_TYPES,0,0,0}}		

#define	SR_CRIT_TYPE_LIST						\
		{{OM_CLASS,1,0,0},					\
		{DS_FILTER_TYPE,1,0,0},					\
		{DS_ATTRIBUTE_TYPE,1,0,0},				\
		{DS_CRITERIA,MAX_VALUES,0,0},				\
		{DS_FILTER_ITEM_TYPE,1,0,0},				\
		{OM_NO_MORE_TYPES,0,0,0}}		

#define	SR_GUID_TYPE_LIST						\
		{{OM_CLASS,1,0,0},					\
		{DS_OBJECT_CLASS,1,0,0},				\
		{DS_CRITERIA,1,0,0},					\
		{OM_NO_MORE_TYPES,0,0,0}}		

#define	TTXID_TYPE_LIST							\
		{{OM_CLASS,1,0,0},					\
		{DS_PARAMETERS,1,0,0},					\
		{DS_TELETEX_TERM,1,0,DS_VL_TELETEX_TERM,0},		\
		{OM_NO_MORE_TYPES,0,0,0}}		

#define	TX_NB_TYPE_LIST							\
		{{OM_CLASS,1,0,0},					\
		{DS_ANSWERBACK,1,0,DS_VL_ANSWERBACK,0},			\
		{DS_COUNTRY_CODE,1,0,DS_VL_COUNTRY_CODE,0},		\
		{DS_TELEX_NBR,1,0,DS_VL_TELEX_NBR,0},			\
		{OM_NO_MORE_TYPES,0,0,0}}		

/* macro to convert interpretation field of ACL string from IAPL to 
   DSX interpretation and vice versa */
#define IAPL_TO_DSX_INTER(x)	x - '0'
#define DSX_TO_IAPL_INTER(x)	x + '0'


#define	FILTER_ITEM_TYPE_LIMITS {				\
    {DS_EQUALITY},						\
    {DS_APPROXIMATE_MATCH}					\
    }

#define	FILTER_ITEM_TYPE_LIST {					\
    DS_EQUALITY,						\
    DS_SUBSTRINGS,						\
    DS_GREATER_OR_EQUAL,					\
    DS_LESS_OR_EQUAL,						\
    DS_PRESENT,							\
    DS_APPROXIMATE_MATCH					\
    }

#define	FILTER_TYPE_LIMITS {					\
    {DS_ITEM},							\
    {DS_NOT}							\
    }

#define	FILTER_TYPE_LIST {					\
    DS_ITEM,							\
    DS_AND,							\
    DS_OR,							\
    DS_NOT							\
    }

#define	INFO_TYPE_LIMITS {					\
    {DS_TYPES_ONLY},						\
    {DS_TYPES_AND_VALUES}					\
    }

#define	INFO_TYPE_LIST {					\
    DS_TYPES_ONLY,						\
    DS_TYPES_AND_VALUES					\
    }

#define	MOD_TYPE_LIMITS {					\
    {DS_ADD_ATTRIBUTE},						\
    {DS_REMOVE_VALUES}						\
    }

#define	MOD_TYPE_LIST {						\
    DS_ADD_ATTRIBUTE,						\
    DS_REMOVE_ATTRIBUTE,					\
    DS_ADD_VALUES,						\
    DS_REMOVE_VALUES						\
    }

#define	NAME_RESOLUTION_PHASE_TYPE_LIMITS {			\
    {DS_NOT_STARTED},						\
    {DS_COMPLETED}						\
    }

#define	NAME_RESOLUTION_PHASE_TYPE_LIST {			\
    DS_NOT_STARTED,						\
    DS_PROCEEDING,						\
    DS_COMPLETED						\
    }

#define	PRIORITY_TYPE_LIMITS {					\
    {DS_LOW},							\
    {DS_HIGH}							\
    }

#define	PRIORITY_TYPE_LIST {					\
    DS_LOW,							\
    DS_MEDIUM,							\
    DS_HIGH							\
    }

#define	SCOPE_OF_REFERRAL_TYPE_LIMITS {				\
    {DS_DMD},							\
    {DS_COUNTRY}						\
    }

#define	SCOPE_OF_REFERRAL_TYPE_LIST {				\
    DS_DMD,							\
    DS_COUNTRY							\
    }

#define	INTERPRETATION_TYPE_LIMITS {				\
    {DSX_SINGLE_OBJECT},					\
    {DSX_ROOT_OF_SUBTREE}					\
    }

#define	INTERPRETATION_TYPE_LIST {				\
    DSX_SINGLE_OBJECT,						\
    DSX_ROOT_OF_SUBTREE						\
    }

#define	AUTH_MECH_TYPE_LIMITS {					\
    {DSX_NONE_AT_ALL},						\
    {DSX_STRONG}						\
    }

#define	AUTH_MECH_TYPE_LIST {					\
	DSX_NONE_AT_ALL,					\
    DSX_DEFAULT,						\
    DSX_SIMPLE,							\
    DSX_SIMPLE_PROT1,						\
    DSX_SIMPLE_PROT2,						\
    DSX_DCE_AUTH,						\
    DSX_STRONG							\
    }

#define	PROT_REQ_TYPE_LIMITS {					\
    {DSX_NONE},							\
    {DSX_SIGNED}						\
    }

#define	PROT_REQ_TYPE_LIST {					\
    DSX_NONE,							\
    DSX_SIGNED							\
    }


/* define default string value length		*/
#define	MIN_VL_0			((OM_value_length) 0)
#define	MIN_VL_1			((OM_value_length) 1)
#define	MIN_VL_TELETEX_TERM		((OM_value_length) 1)
#define	MIN_VL_ANSWERBACK		((OM_value_length) 1)
#define	MIN_VL_COUNTRY_CODE		((OM_value_length) 1)
#define	MIN_VL_TELEX_NBR		((OM_value_length) 1)
#define	MIN_VL_PHONE_NBR		((OM_value_length) 1)
#define	MIN_VL_FINAL_SUBSTRING		((OM_value_length) 1)
#define	MIN_VL_INITIAL_SUBSTRING	((OM_value_length) 1)
#define	MIN_VL_POSTAL_ADDRESS	((OM_value_length) 1)

#endif /* _XDSDEF_H */
