/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mavcod.h,v $
 * Revision 1.1.10.2  1996/02/18  23:34:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:09  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:08:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:48:48  root]
 * 
 * Revision 1.1.8.1  1994/02/22  15:37:13  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:02:08  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  13:54:29  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  11:05:34  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:11:01  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:54:05  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:21:01  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:22:15  zeliff]
 * 
 * $EndLog$
 */
 
#ifndef _MAVCOD_H
#define _MAVCOD_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char mavcod_rcsid[] = "@(#)$RCSfile: mavcod.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:34:44 $";
#endif

/* ****************************************************************** */
/*                                                                    */
/*   COPYRIGHT  (C)  1990 BY SIEMENS NIXDORF INFORMATIONSSYSTEME AG   */
/*                           AND INRIA                                */
/*   All rights reserved                                              */
/*                                                                    */
/*   This software is furnished under licence and may be used only    */
/*   in accordance with the terms of that licence and with the        */
/*   inclusion of the above copyright notice.                         */
/*   This software may not be provided or otherwise made available    */
/*   to, or used by, any other person. No title to or ownership of    */
/*   the software is hereby  transferred.                             */
/*                                                                    */
/* ****************************************************************** */

/*******************************************************\
* 						        *
* A type to indicate the various possible structures.   *
* 						        *
\*******************************************************/

struct asn1_tree {
	int	a_father;
	int	a_brother;
	int	a_first_son;
	int	a_last_son;

	char	* a_id;
	int	a_mode;
	int	a_num;
	int	a_implicit;

	int	a_type;
	char *	a_name;

	int	a_option;
	char *	a_default;

	char *	a_ext_id;
};

extern struct asn1_tree * tree_element;
extern int tree_element_nb;

/**********************************************\
* 					       *
* Some codes for the possible structure types  *
* 					       *
\**********************************************/
struct element {
	int t;
	char * st;
};

#define T_UNDEF		-1
#define T_EXTERN	0
#define T_SET		1
#define T_SEQUENCE	2
#define T_CHOICE	3
#define T_SEQUENCE_OF	4
#define T_SET_OF	5
#define T_NULL		6
#define T_INTEGER	7
#define T_BOOLEAN	8
#define T_REAL		9
#define T_OCTETS	10
#define T_OID		11
#define T_EXTERNAL	12
#define T_BITS		13
#define T_ANYDEF	14
#define T_ANY		15
#define T_INT_VALUE	16
#define T_TYPE		17
#define T_COMP		18
#define T_IGNORE        19     /*AF*/
#define T_FLAGS         20     /*AF ebenfalls erg.*/
#define T_COPY          21     /*AF ebenfalls erg.*/
#define T_ENUMERATED    22

extern struct element element_error;
/***************************************************\
* 						    *
* The possible values of the ``optionality'' code.  *
* 						    *
\***************************************************/
struct options {
	int t;
	char * n;
};

#define O_DEFAULT	1
#define O_OPTION	2
#define O_ALWAYS	0

extern struct options option_always;	

/****************************************\
* 					 *
* The possible values of the class tag.	 *
* 					 *
\****************************************/
struct tags {
	int m;
	int n;
	int i;
};

#define C_UNIVERSAL	0
#define C_APPLICATION	1
#define C_CONTEXT	2
#define C_PRIVATE	3
#define C_UNSPEC	-1

#define F_IMPLICIT	0
#define F_EXPLICIT	1
#define F_UNDEF		-1

extern struct tags tags_unspec;

/*****************************************************\
* 						      *
* This structure is used during the yacc processing,  *
* in order to pass the component description.	      *
* 						      *
\*****************************************************/

struct struct_element {
	char * id;
	struct tags tag;
	struct element el;};

/**************************************\
* 				       *
* Values of the ``argument flag'' for  *
* printing arguments.		       *
* 				       *
\**************************************/
#define A_STD		0
#define A_CHOICE_INDEX	1
#define A_CHOICE	2
#define A_ARRAY_INDEX	3
#define A_ARRAY		4
#define A_OPTION	5

/*********************************\
* 				  *
* A limited number of constants.  *
* 				  *
\*********************************/
#define ALLOCATION_STEP ((unsigned) 128)
#define STD_INPUT_NAME	"standard-input"

#endif /* _MAVCOD_H */
