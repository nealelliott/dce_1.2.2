/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xosstruct.h,v $
 * Revision 1.1.4.2  1996/03/11  13:25:04  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:19  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:50:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:29  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:22:38  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:41  devsrc]
 * 
 * Revision 1.1.2.2  1994/03/23  15:50:27  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:38:42  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:17  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:17:29  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:57:38  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOM Convenience Functions                         */
/* File Name   : xosstruct.h					   */
/* Description : This file contains the structures and data type   */
/*		 definitions used by the Convenience Functions 	   */
/* Date        : July 1, 1993                                      */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _XOSSTRUCT_H
#define _XOSSTRUCT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xosstruct_rcsid[] = "@(#)$RCSfile: xosstruct.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:25:04 $";
#endif

/*-- Defines ------------------------------------------------------*/
#define	XOS_RDN_DELIMITER_OP  '/'   /* Delimiter between RDNs	             */
#define XOS_ESCAPE_OP	      '\\'  /* Escape character		             */
#define XOS_AVA_SEPARATOR_OP  ','   /* To represent multiple AVAs            */
#define XOS_AVA_EQUALITY_OP   '='   /* Equality operator for AVAs            */
#define XOS_AVA_QUOTE_OP      '\''  /* Quote character to enclose AVAs       */
#define XOS_HEX_OP	      'x'   /* To specifiy hexadecimal values        */
#define XOS_ATTR_EQUALITY_OP  '='   /* Equality operator for Attributes      */
#define XOS_RECUR_OP          ';'   /* To specify recurring values 	     */
#define XOS_ATTR_QUOTE_OP     '\''  /* Quote character to enclose attribute  */
				    /* values.				     */
#define XOS_WILD_CARD_OP      '*'   /* Wild card character to specify        */
				    /* substring operations in filter strings*/
#define XOS_SATTR_START_OP    '{'   /* Start of Structured Attribute Value   */
#define XOS_SATTR_END_OP      '}'   /* End of Structured Attribute Value     */
#define XOS_COMP_DELIMITER_OP ','   /* Delimiter for components of a         */
				    /* structured attribute.		     */
#define XOS_AND_OP	      '&'   /* AND operator used in filter strings   */
#define XOS_OR_OP	      '|'   /* OR operator used in filter strings    */
#define XOS_GREATER_OP	      '>'   /* Greater than  operator used in filter */
#define XOS_LESSER_OP	      '<'   /* Less than  operator used in filter    */
#define XOS_APPROX_OP	      '~'   /* Approximate match operator 	     */
#define XOS_NOT_OP	      '!'   /* NOT operator used in filter strings   */
#define XOS_BRACKET_OPEN_OP   '('   /* Open brackets used for nesting filters*/
#define XOS_BRACKET_CLOSE_OP  ')'   /* Close brackets for nesting filters    */


#define XOS_MAX_FILE_NAME 1024	    /* Maximum size of schema file name	     */
#define XOS_MAX_OBJID     64	    /* Maximum size of Obj.Identifier string */
#define XOI_MAX_NAME      64        /* Maximum size of Attribute Full Name   */
#define XOS_MAX_ABBRV     16        /* Maximum size of Abbreviation	     */
#define XOS_MAX_VAR       16	    /* Maximum size for other variables	     */

/*-- Utility macros -----------------------------------------------*/
/* To initialize a NULL descriptor */
#define XOS_FILL_OMD_NULL(desc)	\
	omX_fill(OM_NO_MORE_TYPES, OM_S_NO_MORE_SYNTAXES, 0,		\
		 OM_ELEMENTS_UNSPECIFIED, desc)

/* To access the value of the syntax in the descriptor structure */
#define XOS_SYNTAX_VAL(desc) (((OM_descriptor *)(desc))->syntax)

/* To check if the descriptor is of type OM_S_OBJECT */
#define XOS_OBJECT(desc)     (((XOS_SYNTAX_VAL(desc)) & OM_S_SYNTAX) ==	\
			      OM_S_OBJECT)

/* To check for client generated public object */
#define XOS_CPUB(obj)  ((XOS_SYNTAX_VAL(obj) & 				\
			(OM_S_PRIVATE | OM_S_SERVICE_GENERATED)) == 0)

/* To check for private object */
#define XOS_PRIVATE(obj)  ((OMP_TYPE(obj) == OM_PRIVATE_OBJECT) &&	\
			   ((XOS_SYNTAX_VAL(obj) &			\
			   (OM_S_PRIVATE | OM_S_SERVICE_GENERATED)) ==  \
			   (OM_S_PRIVATE | OM_S_SERVICE_GENERATED)))


/*-- Data Structures ----------------------------------------------*/
/* Enumerated type for the input tokens in a filter string */
typedef enum
{
   XOS_F_ABBRV         = 0,    /* Attribute Abbreviation */
   XOS_F_BRACKET_OPEN  = 1,    /* Open Bracket 		 */
   XOS_F_BRACKET_CLOSE = 2,    /* Close Bracket 	 */
   XOS_F_NOT           = 3,    /* NOT operator 		 */
   XOS_F_AND           = 4,    /* AND operator 		 */
   XOS_F_OR            = 5,    /* OR operator  		 */
   XOS_F_EOS           = 6     /* End of String		 */
} xos_token;

/* Enumerated type for the stack tokens */
typedef enum
{
   XOS_S_START	       = 0,    /* Start of input string  */
   XOS_S_BRACKET_OPEN  = 1,    /* Open Bracket		 */
   XOS_S_NOT	       = 2,    /* NOT operator		 */
   XOS_S_AND	       = 3,    /* AND operator		 */
   XOS_S_OR	       = 4,    /* OR operator		 */
   XOS_S_FILTER_ITEM   = 5,    /* DS_C_FILTER_ITEM object*/
   XOS_S_FILTER        = 6     /* DS_C_FILTER object	 */
} xos_stack_token;

/* Structure for the stack items */
typedef struct _xos_stack_item
{
   xos_stack_token  token;
   OM_object	    object;
} xos_stack_item;

#endif  /* ifndef _XOSSTRUCT_H */
