/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: omTrace.c,v $
 * Revision 1.1.8.2  1996/02/18  18:25:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:48:01  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:41:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:57:10  root]
 * 
 * Revision 1.1.6.1  1994/02/22  19:00:45  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  12:08:49  marrek]
 * 
 * Revision 1.1.4.2  1993/08/10  07:24:59  marrek
 * 	July 1993 code drop.
 * 	[1993/07/27  14:27:40  marrek]
 * 
 * Revision 1.1.2.3  1992/12/30  21:18:38  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/30  21:07:38  bbelch]
 * 
 * Revision 1.1.2.2  1992/11/30  08:42:44  marrek
 * 	November 1992 code drop
 * 	[1992/11/25  15:49:15  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: omTrace.c,v $ $Revision: 1.1.8.2 $ $Date: 1996/02/18 18:25:43 $";
#endif

/**********************************************************************/
/*					COPYRIGHT (C) BULL S.A.	1991 1992				  */
/*						  All Rights Reserved						  */
/**********************************************************************/
/*																	  */
/*	 * This	software is	furnished under	licence	and	may	be used	only  */
/*	 * in accordance with the terms	of that	licence	and	with the	  */
/*	 * inclusion of	the	above copyright	notice.						  */
/*	 * This	software may not be	provided or	otherwise made available  */
/*	 * to, or used by, any other person. No	title to or	ownership of  */
/*	 * the software	is hereby transferred.							  */
/*																	  */
/**********************************************************************/
/*		@(#)omTrace.c	1.1	(BULL S.A) 6/26/92		*/

/**********************************************************************/
/*	File name:	  omTrace.c											  */
/*	Project:	  XOM library										  */
/*	Author:		  Daniel LUTOFF										  */
/**********************************************************************/
/*			   M O D I F I C A T I O N	 P R O T O C O L			  */
/**********************************************************************/
/*	date		 !			description					 !	vs ! sc	  */
/*_______________!_______________________________________!_____!______*/
/* 04/09/91		 ! initial version						 !	01 !	  */
/*				 !										 !	   !	  */
/*				 !										 !	   !	  */
/**********************************************************************/
/*																	  */
/*				   f i l e	 d e s c r i p t i o n					  */
/*				   ===============================					  */
/*																	  */
/*																	  */
/**********************************************************************/

#ifdef THREADSAFE
#	include <pthread.h>
#endif

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include <string.h>
#include <xom.h>
#include <xomip.h>
#include <alloc.h>
#include <xomtrace.h>
#include <omTrace.h>

extern OM_sint	omP_trace_opt;
extern FILE	*omP_trace_file;


#ifdef OM_TRACE
#define	OMP_ILLEGAL_OPT_COMBINATION(opt1, opt2)			\
		  {											\
		  fprintf(omP_trace_file,"Warning :	illegal	options	combination	( (%s) and (%s))\n", opt1, opt2);\
		  }
#else
#define	OMP_ILLEGAL_OPT_COMBINATION(opt1, opt2)
#endif

#ifdef OM_TRACE
#define	OMP_OPEN_TRACE_FILE_FAILED()				\
		  {											\
	  extern int  errno;						\
		  fprintf(stderr,"Warning :	opening	OM trace file failed : %s\n", strerror(errno));\
		  }
#else
#define	OMP_OPEN_TRACE_FILE_FAILED()
#endif

#ifdef OM_TRACE
#define	OMP_CLOSE_TRACE_FILE_FAILED()				 \
		  {											\
	  extern int  errno;						\
		  fprintf(stderr,"Warning :	closing	OM trace file failed : %s\n", strerror(errno));\
		  }
#else
#define	OMP_CLOSE_TRACE_FILE_FAILED()
#endif













































/**********************************************************************/
/*																	  */
/* FUNCTION:  omP_set_trace()										  */
/*																	  */
/* PURPOSE:	 set the OM	trace options.								  */
/*																	  */
/* INPUT:  none.													  */
/*									  */
/* INPUT/OUTPUT: none							  */
/*									  */
/* OUTPUT:	none							  */
/*									  */
/* RETURN:	return_code: whether the function succeeded	or failed	  */
/*									  */
/* NOTE:	The	 environment variable "OM_TRACE_OPTIONS" is	used to	  */
/*			set	up OM trace	options.								  */
/*			The	syntax is :											  */
/*			OM_TRACE_OPTIONS=<option>[:<option>][@<file>]			  */
/*			The	possible value of <option> are :					  */
/*					 NONE :	none traces	will be	generated.			  */
/*					 ALL : all traces will be generated. Some of these*/
/*			   traces can not be understood	by application*/
/*			   programmer. There are for BULL technical	  */
/*			   support.									  */
/*					 SYNTAX	: OM object	syntax error will be traced	  */
/*					 ASSERT	: abort	the	process	when an	error occurs  */
/*									  */
/*			<file> specified the trace file. Default file is stderr	  */
/*									  */
/*			Default	value of <option> is "NONE"						  */
/*									  */
/*			Example:												  */
/*			  OM_TRACE_OPTIONS=ALL@stderr; export OM_TRACE_OPTIONS	  */
/*									  */
/**********************************************************************/

OM_return_code
omP_set_trace(void)
{
	char	*val, *s_val, *s_val_cpy; /* used to store value returned by "getenv" call */
	char	*tok;	   /* used to store	token value						*/
	char	*opt;	   /* used to store	the	string of the trace	options	*/
	OM_sint	cmp_res;	  /* used to store string comparison result	*/

	/*-----------------------------------------------------------*/
	/* Get environment variable									 */
	/*-----------------------------------------------------------*/
	val	 = getenv("OM_TRACE_OPTIONS");

	/*-----------------------------------------------------------*/
	/* If environment variable "OM_TRACE_OPTIONS" cannot be		 */
	/* found : there are no	trace set up.						 */
	/*-----------------------------------------------------------*/
	if (val	 !=	NULL) {
		/*---------------------------------------------------------*/
		/* store the OM_TRACE_OPTIONS value						   */
		/*---------------------------------------------------------*/
		s_val =		OM_MALLOC(strlen(val) +		1);
		if (s_val == NULL) {
			return(OM_MEMORY_INSUFFICIENT) ;
		}

		/*---------------------------------------------------------*/
		/* store the pointer value in order	to free	memory		   */
		/*---------------------------------------------------------*/
		s_val_cpy =		s_val;
		strcpy(s_val, val);

		/*---------------------------------------------------------*/
		/* isolate trace options : skip	the	trace file			   */
		/*---------------------------------------------------------*/
		opt	 = strtok(val, "@");

		/*---------------------------------------------------------*/
		/* parse the trace options : options consistency is	checked*/
		/*---------------------------------------------------------*/
		tok	 = strtok(opt, ":");
		while (tok != NULL)	 {

			cmp_res	 = strcmp(tok, "NONE");
			if (cmp_res	 ==	0) {
				if (omP_trace_opt &		OMP_TRACE_ALL) {
					OMP_ILLEGAL_OPT_COMBINATION( "NONE",
						 "ALL");
					OM_FREE(s_val);
					return(OM_TEMPORARY_ERROR) ;
				}
				omP_trace_opt =		OMP_TRACE_OFF;
			}

			cmp_res	 = strcmp(tok, "ALL");
			if (cmp_res	 ==	0) {
				if (omP_trace_opt &		OMP_TRACE_OFF) {
					OMP_ILLEGAL_OPT_COMBINATION("NONE",
						 "ALL");
					OM_FREE(s_val);
					return(OM_TEMPORARY_ERROR) ;
				}
				omP_trace_opt =		OMP_TRACE_ALL;
			}

			cmp_res	 = strcmp(tok, "OBJECT");
			if (cmp_res	 ==	0) {
				if (omP_trace_opt &		OMP_TRACE_OFF) {
					OMP_ILLEGAL_OPT_COMBINATION("NONE",
						 "OBJECT");
					OM_FREE(s_val);
					return(OM_TEMPORARY_ERROR) ;
				}
				omP_trace_opt += OMP_TRACE_OBJECT;
			}

			cmp_res	 = strcmp(tok, "SYNTAX");
			if (cmp_res	 ==	0) {
				omP_trace_opt |= OMP_TRACE_CHECK_SYNTAX;
			}

			cmp_res	 = strcmp(tok, "NO_CHECK");
			if (cmp_res	 ==	0) {
				omP_trace_opt |= OMP_DO_NOT_CHECK_SYNTAX;
			}

			cmp_res	 = strcmp(tok, "FATAL");
			if (cmp_res	 ==	0) {
				if (omP_trace_opt &		OMP_TRACE_OFF) {
					OMP_ILLEGAL_OPT_COMBINATION("NONE",
						 "FATAL");
					OM_FREE(s_val);
					return(OM_TEMPORARY_ERROR) ;
				}
				omP_trace_opt |= OMP_TRACE_FATAL_ERROR;
				omP_trace_opt |= OMP_TRACE_ASSERT_ON;
			}

			cmp_res	 = strcmp(tok, "BED");
			if (cmp_res	 ==	0) {
				if (omP_trace_opt &		OMP_TRACE_OFF) {
					OMP_ILLEGAL_OPT_COMBINATION("NONE",
						 "BED");
					OM_FREE(s_val);
					return(OM_TEMPORARY_ERROR) ;
				}
				omP_trace_opt |= OMP_TRACE_BED;
			}

			cmp_res	 = strcmp(tok, "ASSERT");
			if (cmp_res	 ==	0) {
				if (omP_trace_opt &		OMP_TRACE_OFF) {
					OMP_ILLEGAL_OPT_COMBINATION("NONE",
						 "ASSERT");
					OM_FREE(s_val);
					return(OM_TEMPORARY_ERROR) ;
				}
				omP_trace_opt |= OMP_TRACE_ASSERT_ON;
			}

			tok	 = strtok(NULL,	":");

		} /* while end */

		/*---------------------------------------------------------*/
		/* get trace file										   */
		/*---------------------------------------------------------*/
		tok	 = strtok(s_val,	"@");
		tok	 = strtok(NULL,	"@");
		if (tok	 !=	NULL) {
			omP_trace_file = fopen(tok,		"a");
			if (omP_trace_file == NULL)	 {
				omP_trace_file = stderr	;
				OMP_OPEN_TRACE_FILE_FAILED();
			}
		}

		/*---------------------------------------------------------*/
		/* free	up memory										   */
		/*---------------------------------------------------------*/
		OM_FREE(s_val_cpy);

	} /* if	end	*/

	/*-----------------------------------------------------------*/
	/* Processing end :	success									 */
	/*-----------------------------------------------------------*/
	return(OM_SUCCESS) ;


	/*------------------------------------------------------*/
} /* end of:  void omP_set_trace()				  */



/*------------------------------------------------------*/


/**********************************************************************/
/*																	  */
/* FUNCTION:  omP_trace_create()									  */
/*																	  */
/* PURPOSE:	 traces	the	creation of	OM_objects						  */
/*																	  */
/* INPUT:								  */
/*	trc_file : the trace file in which the trace is	written		  */
/*	file : the file	in which the create	function is	called		  */
/*	line : the line	in file	where the create function is called	  */
/*	object : the created OM_object.								  */
/*									  */
/* INPUT/OUTPUT: none							  */
/*									  */
/* OUTPUT:	none							  */
/*									  */
/* RETURN:	none							  */
/*									  */
/* NOTE:									  */
/*	This function is called	only if	the	trace is activated and the*/
/*	trace file trc_file	is opened						  */
/*									  */
/**********************************************************************/

void
omP_trace_create(
	FILE			*trc_file,
	char 			*file,
	OM_sint				line,
	OM_private_object	object)
{
	fprintf(trc_file, "O_CREATE	pt : %X, file :	%s,	line : %d\n",
            (unsigned int)object, file, line);
	fflush (trc_file);
} /* end omP_trace_create */


/**********************************************************************/
/*																	  */
/* FUNCTION:  omP_trace_encode()									  */
/*																	  */
/* PURPOSE:	 traces	the	creation of	OM_object result of	an encode	  */
/*		 operation									  */
/*																	  */
/* INPUT:								  */
/*	trc_file : the trace file in which the trace is	written		  */
/*	file : the file	in which the create	function is	called		  */
/*	line : the line	in file	where the create function is called	  */
/*	object : the created OM_object.								  */
/*									  */
/* INPUT/OUTPUT: none							  */
/*									  */
/* OUTPUT:	none							  */
/*									  */
/* RETURN:	none							  */
/*									  */
/* NOTE:									  */
/*	This function is called	only if	the	trace is activated and the*/
/*	trace file trc_file	is opened						  */
/*									  */
/**********************************************************************/

void
omP_trace_encode(
	FILE			*trc_file,
	char *file,
	OM_sint	line,
	OM_private_object	object)
{
	fprintf(trc_file, "O_CREATE	pt : %X, file :	%s,	line : %d\n",
            (unsigned int)object, file, line);
	fflush (trc_file);
} /* end omP_trace_encode */


/**********************************************************************/
/*																	  */
/* FUNCTION:  omP_trace_decode()									  */
/*																	  */
/* PURPOSE:	 traces	the	creation of	OM_object result of	a decode	  */
/*		 operation									  */
/*																	  */
/* INPUT:								  */
/*	trc_file : the trace file in which the trace is	written		  */
/*	file : the file	in which the create	function is	called		  */
/*	line : the line	in file	where the create function is called	  */
/*	object : the created OM_object.								  */
/*									  */
/* INPUT/OUTPUT: none							  */
/*									  */
/* OUTPUT:	none							  */
/*									  */
/* RETURN:	none							  */
/*									  */
/* NOTE:									  */
/*	This function is called	only if	the	trace is activated and the*/
/*	trace file trc_file	is opened						  */
/*									  */
/**********************************************************************/

void
omP_trace_decode(
	FILE			*trc_file,
	char *file,
	OM_sint	line,
	OM_private_object	object)
{
	fprintf(trc_file, "O_CREATE	pt : %X, file :	%s,	line : %d\n",
            (unsigned int)object, file, line);
	fflush (trc_file);
} /* end omP_trace_decode */


/**********************************************************************/
/*																	  */
/* FUNCTION:  omP_trace_copy()										  */
/*																	  */
/* PURPOSE:	 traces	the	creation of	OM_object result of	a copy		  */
/*		 operation									  */
/*																	  */
/* INPUT:								  */
/*	trc_file : the trace file in which the trace is	written		  */
/*	file : the file	in which the create	function is	called		  */
/*	line : the line	in file	where the create function is called	  */
/*	object : the created OM_object.								  */
/*									  */
/* INPUT/OUTPUT: none							  */
/*									  */
/* OUTPUT:	none							  */
/*									  */
/* RETURN:	none							  */
/*									  */
/* NOTE:									  */
/*	This function is called	only if	the	trace is activated and the*/
/*	trace file trc_file	is opened						  */
/*									  */
/**********************************************************************/

void
omP_trace_copy(
	FILE			*trc_file,
	char *file,
	OM_sint	line,
	OM_private_object	object)
{
	fprintf(trc_file, "O_CREATE	pt : %X, file :	%s,	line : %d\n",
            (unsigned int)object, file, line);
	fflush (trc_file);
} /* end omP_trace_copy	*/


/**********************************************************************/
/*																	  */
/* FUNCTION:  omP_trace_get()										  */
/*																	  */
/* PURPOSE:	 traces	the	creation of	the	SPUB OM_object result of the  */
/*		 get operation.						  */
/*																	  */
/* INPUT:                                                             */
/*	trc_file : the trace file in which the trace is	written		  */
/*	file : the file	in which the create	function is	called		  */
/*	line : the line	in file	where the create function is called	  */
/*	object : the created SPUB OM_object.						  */
/*									  */
/* INPUT/OUTPUT: none							  */
/*									  */
/* OUTPUT:	none							  */
/*									  */
/* RETURN:	none							  */
/*									  */
/* NOTE:									  */
/*	This function is called	only if	the	trace is activated and the*/
/*	trace file trc_file	is opened						  */
/*									  */
/**********************************************************************/

void
omP_trace_get(
	FILE			*trc_file,
	char *file,
	OM_sint	line,
	OM_public_object	object,
	OM_sint	att_nb)
{
	OM_sint	i;

	fprintf(trc_file, "SP_CREATE pt : %X, file : %s, line : %d\n",
            (unsigned int)object, file, line);
	for	(i = 0;	i <		att_nb;	i++)	 {
		if ((object[i].syntax &		OM_S_SYNTAX) ==		OM_S_OBJECT)	 {
			fprintf(trc_file, "H_CREATE	pt : %X, file :	%s,	line : %d\n",
				 (unsigned int)object[i].value.object.object,		file,
				line);
		}
	}

	fflush (trc_file);
} /* end omP_trace_get */


/**********************************************************************/
/*																	  */
/* FUNCTION:  omP_trace_delete()									  */
/*																	  */
/* PURPOSE:	 traces	the	deletion of	OM_objects						  */
/*																	  */
/* INPUT:								  */
/*	trc_file : the trace file in which the trace is	written		  */
/*	file : the file	in which the create	function is	called		  */
/*	line : the line	in file	where the create function is called	  */
/*	object : the deleted OM_object.								  */
/*									  */
/* INPUT/OUTPUT: none							  */
/*									  */
/* OUTPUT:	none							  */
/*									  */
/* RETURN:	none							  */
/*									  */
/* NOTE:									  */
/*	This function is called	only if	the	trace is activated and the*/
/*	trace file trc_file	is opened						  */
/*									  */
/**********************************************************************/

void
omP_trace_delete(
	FILE		*trc_file,
	char *file,
	OM_sint	line,
	OM_object	object)
{

	fprintf(trc_file, "O_FREE pt : %X, file : %s, line : %d\n",
            (unsigned int)object, file, line);

	/*
	if (!OMP_PRIVATE(object))	{
				for	(i=0,desc_ptr=object;desc_ptr->type!=OM_NO_MORE_TYPES;
								i++,desc_ptr++)
				{
			if (OMP_SYNTAX(desc_ptr) ==	OM_S_OBJECT)
				omP_trace_delete(
					trc_file,
					file,
					line,
					desc_ptr->value.object.object);
				}
	}

*/

	fflush (trc_file);
} /* end omP_trace_delete */


/*******************/
/*	END	 OF	 FILE  */
/*******************/
