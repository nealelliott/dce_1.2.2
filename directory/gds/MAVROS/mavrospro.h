/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mavrospro.h,v $
 * Revision 1.1.10.2  1996/02/18  23:34:46  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:12  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:09:42  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:13  root]
 * 
 * Revision 1.1.8.1  1994/02/22  15:40:43  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:05:07  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  14:12:19  marrek
 * 	July 1993 code drop.
 * 	[1993/07/29  11:09:06  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:12:15  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:55:09  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:22:51  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:34:22  zeliff]
 * 
 * $EndLog$
 */
 
#ifndef _MAVROSPRO_H
#define _MAVROSPRO_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char mavrospro_rcsid[] ="@(#)$RCSfile: mavrospro.h,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 23:34:46 $";
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

/**************************************************************\
* 							       *
* These two structures are used to convey intermediate values  *
* during the parsing of the input.			       *
* 							       *
\**************************************************************/
struct tdcl{
	int 	l1,
		l2,
		c;
	char 	* s;
};

struct tstrd{
	char 	* s;
	int 	m;
	int 	n;
};

/**********************************************************\
* 							   *
* The component structure is used to defined the elements  *
* of the MAVROS module specification.			   *
* 							   *
\**********************************************************/
struct comp{
	char 	* c_id;		/* The text identifier */
	int 	c_mode,		/* The tag MODE */
		c_number,	/* The tag NUMBER */
		c_implicit,	/* IMPLICIT or not */
		c_type,		/* The TYPE code */
		c_u_tag,	/* The UNIVERSAL tag number, if implicit.. */
		c_optional,	/* OPTIONALITY */
		c_first_son,	/* the first child */
		c_brother,	/* Next brother or "-1" */
		c_father,	/* father */
		c_line_num,	/* Line number of the declaration */
		c_call_mode;	/* call mode, for arguments */
	char 	* c_name,	/* Name of the type */
		* c_index;	/* Name of index or variable */
};

/***************************************************************************\
* 									    *
* The structure ``exec_flag'' is used for defining the			    *
* encoding parameters for different types of routine:			    *
* coding, decoding, light weight, etc..					    *
* 									    *
* The structure ``basic_table_table'' is used for defining the 		    *
* arguments of the ``basic'' coding and decoding routines.		    *
* 									    *
* The table ``exec_table'' is indexed by the ``external routine generation  *
* flags'' defined underneath.						    *
* 									    *
* The tables ``exec_table'' and ``basic_table'' are defined in the 	    *
* module ``mavrosbase''.						    *
* 									    *
\***************************************************************************/
typedef struct {
	char	* suffix;
	char	* value;
	char	* common_args;
	int 	arg_mode;
	int 	comp_mode;
	char	* arg_proto;
	char	* arg_decl;
	char	* ret_type;
	char	* ret_arg;
}exec_flag;

struct basic_table_entry {
	char *	bt_name;
	int	bt_vcod;
	int	bt_vdec;
	int	bt_alloc;
	int	bt_nb_parms;
	int	* bt_parm_mode;};


extern exec_flag exec_table[];
extern struct basic_table_entry basic_table[];
/******************************************************************\
* 								   *
* These variables are used to compute the depth of a declaration,  *
* and to list the indexes that it will manipulate.		   *
* 								   *
\******************************************************************/
extern char ** storelist;
extern int storeindex;
extern int current_list_head;
extern int current_call_mode;

char * shortname();

/**********************\
* 		       *
* Types of components  *
* 		       *
\**********************/
#define T_EXTERN	0
#define T_SEQUENCE	1
#define T_SET		2
#define T_CHOICE	3
#define T_PARAMETER	5
#define T_IGNORE	6
#define T_SEQUENCE_OF	7
#define T_SET_OF 	8
#define T_ANY		9
#define T_INTEGER	10
#define T_BOOLEAN	11
#define T_OCTETS	12
#define T_BITS		13
#define T_CHARS		14
#define T_COPY		15
#define T_NULL		16
#define T_ARGUMENT	17
#define T_TYPE		18
#define T_INIT		24
#define T_ALLOC		25
#define T_INIT_IT	26
#define T_DEFAULT	27
#define T_OCTETS_ALLOC	28
#define T_VALUE		31
#define T_BITS_ALLOC	32
#define T_ANY_ALLOC	33
#define T_COMP		34
#define T_FLAGS		35
#define T_OID		36
#define T_OID_ALLOC	37
#define T_IMPORT	38
#define T_COPY_ALLOC	39
#define T_EXTERNAL	40
#define T_REAL		41
#define T_ANYDEF	42
#define T_FINAL		43
#define T_INIT_STR	44

/**************************\
* 			   *
* Program execution flags  *
* 			   *
\**************************/
#define PRETTY 		1
#define INCLUDE 	2
#define ASN1CODER 	4
#define MAKEFILE 	8
#define LIGHTWEIGHT 	16
#define ONE_FILE 	32
#define INOUT		64
#define COPYING		128

/************************************\
* 				     *
* External routine generation flags  *
* 				     *
\************************************/
#define DECODE		0
#define ENCODE		1
#define FREE		2
#define LENGTH		3
#define COMPCOD		4
#define	C_SEQ_D		5
#define	ERRCOD		6
#define LWCOD		7
#define LWDEC		8
#define MATCH		9
#define IN		10
#define OUT		11
#define OUTLEN  	12
#define C_IN		13
#define C_OUT		14
#define C_CPY		15

#define INCLUDE_FLAG  	(1<<7)

/***********************\
* 		        *
* Type generation flag  *
* 		        *
\***********************/
#define SET_MODE 	0
#define SEQUENCE_MODE 	1
#define COMPLEX_MODE	2
#define DEFINE_MODE 	3
#define HALF_MODE 	4

#endif /* _MAVROSPRO_H */
