/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: gciprint.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:34  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:26  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:56:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:11:47  root]
 * 
 * Revision 1.1.2.4  1994/09/19  08:39:24  marrek
 * 	Fixes for OT12176 (wrong incore tables.)
 * 	[1994/09/19  08:32:27  marrek]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:23  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:14  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:54:46  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:56:56  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:07  keutel
 * 	March 1994 code drop
 * 	[1994/03/22  11:31:25  keutel]
 * 
 * 	creation
 * 	[1994/03/21  12:54:57  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gciprint.c                                        */
/* Description : This file contains all the modules for printing   */
/*		 the results received from the gdscp Translator    */
/*		 in a structured format.			   */
/* Date        : Oct 04, 1993                                      */
/* Author      : M. Kronenburg, SNI BU BA KP13                     */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gciprint.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:34 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <termios.h>

#include <gds.h>

#include <dce/tcl.h>

#include <gdscp.h>
#include <gctconv.h>
#include <gctstruct.h>

#include <gcistruct.h>
#include <gciutil.h>
#include <gciprint.h>

/*-- Type definitions ------------------------------------------*/
/* structure for one line */
typedef struct _gc_i_output_line
{
    boolean     is_header;   /* indicates if this line is the header line */
    signed16    indent;      /* indentation of this line                  */
    signed16    parts_count; /* number of parts in this line              */
    char      **line_parts;  /* the parts of this line                    */
} gc_i_output_line;

/* structure for all lines of one output */
typedef struct _gc_i_output_struct
{
    signed16           max_parts;      /* max. number of parts in one line  */
    char              *div_sym;        /* symols dividing the several parts */
    signed16          *div_sym_indent; /* columns of the dividing symbols   */
    unsigned32         line_count;     /* number of lines                   */
    gc_i_output_line  *out_vect;       /* the several lines                 */
} gc_i_output_struct;

typedef enum _gc_i_page_option_type
{
    GC_I_NTH_PAGE	= 0, /* To advance to the nth page  */
    GC_I_LAST_PAGE	= 1, /* To advance to the last page */
    GC_I_NEXT_LINE	= 2, /* To advance to the next line */
    GC_I_SKIP_FORWARD   = 3, /* To skip forward		    */
    GC_I_SKIP_BACKWARD  = 4, /* To skip backward	    */
    GC_I_QUIT	        = 5  /* To quit		    	    */
} gc_i_page_option_type;


/*-- Local Defines -------------------------------------------*/
/* For the level of indentation in the output */
#define GC_I_FIRST_LEVEL_INDENT 	4
#define GC_I_NORMAL_LEVEL_INDENT 	2

/* defines to indicate the type of result to be printed */
#define GC_I_DIR_RESULT 		((signed16) 0)
#define GC_I_ABBR_SVC_RESULT    	((signed16) 1)

/* maximum lines printed on the terminal including the prompt */
#define GC_I_TERMINAL_SIZE      	24

/* maximum number of characters printed on the terminal */
#define GC_I_COLUMN_SIZE         	80

/* Define for the prompt */
#define GC_I_PROMPT              	":"

/* Define to indicate end of result */
#define GC_I_EOL_STRING       	        "(EOL)"

/* maximum length of the commandline */
#define GC_I_CMD_LINE_LEN         	8

/*-- Static variables ----------------------------------------*/
/* output stucture for the x500obj commands show, search and list */
static gc_i_output_struct  gc_i_dir_result = { 0, NULL, NULL, 0, NULL };

/* output stucture for the show command of x500abbr and x500svc */
static gc_i_output_struct  gc_i_abbr_svc_result = { 0, NULL, NULL, 0, NULL };

/* to store the message in case of incomplete results */
static char *gc_i_incomplete_result_message;

/* first line which should be printed */
static signed32 gc_i_first_line;

/* last line which should be printed */
static signed32 gc_i_last_line;

/* option the user has chosen */
static gc_i_page_option_type gc_i_page_option = GC_I_NTH_PAGE;

/* number of pages to be skipped or number of pages to be viewed */
static signed32 gc_i_skip_pages;

/* flag to indicate which output structure should be printed */
static signed16 gc_i_output_flag;

/* to store the prompt string */
static char gc_i_prompt_string[sizeof(GC_I_PROMPT) + sizeof(GC_I_EOL_STRING)];

extern boolean  gc_i_interactive_mode;


/*-- Static Functions -------------------------------------------*/
static signed16 gc_i__prepare_attr(
    signed16      indent,     /* IN  - indent 		    */
    gc_t_vector  *in_vect,    /* IN  - Vector to convert    */
    unsigned32   *index);     /* I/O - index of this vector */

static signed16 gc_i__prepare_struct_attr(
    signed16      indent,     /* IN  - indent for the attribute name */
    gc_t_vector  *in_vect,    /* IN  - Vector to convert 	     */
    unsigned32   *index);     /* I/O - index of this vector 	     */

static signed16 gc_i__init_out_struct(
    signed16    max_part,    /* IN  - max. parts in a line   */
    char       *div_symb,    /* IN  - the dividing symbols   */
    unsigned32  line_count); /* IN  - number of lines	     */

static signed16 gc_i__prepare_normal_abbr(
    gc_t_vect_type  sym1,       /* IN  - 1. symbol in the gct_vector     */
    gc_t_vect_type  sym2,       /* IN  - 2. symbol in the gct_vector     */
    gc_t_vect_type  sym3,       /* IN  - 3. symbol in the gct_vector     */
    char           *head_string,/* IN  - string at the head of the block */
    gc_t_vector    *in_vect,    /* IN  - Vector to convert 	         */
    unsigned32     *in_index);  /* I/O - index of this vector 	         */

static signed16 gc_i__prepare_struct_attr_abbr(
    gc_t_vector  *in_vect,     /* IN  - Vector to convert    */
    unsigned32   *in_index);   /* I/O - index of this vector */

static signed16 gc_i__int_length(
    signed32 number);  /* IN - number */

static void gc_i__check_indentation(
    signed16  indent,  /* IN  - The indentation */
    signed16  column); /* IN  - Column Number   */

static signed16 gc_i__print_page(
    boolean  print_flag);/* IN - Indicates if printing reqd or not */

static void gc_i__read_cmd_line(
    void);

static void gc_i__compute_next_page(
    void);

static void gc_i__free_out_struct( 
    void);


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i_prepare_print_result                       */
/* Description   : This function is used to convert the result     */
/*		   received after a list, show or search operation.*/
/*		   It processes the results and prepares the       */
/*		   data structures for printing the result in a    */
/*		   structured format.				   */
/*		   In the case of errors, the TCL interpreter      */
/*		   result is set to the error message.		   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*                 the type of error.				   */
/* Author        : M. Kronenburg, SNI BU BA KP13                   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_i_prepare_print_result(
    gc_t_vector  *in_vect,  /* IN  - Vector to convert 		    	   */
    Tcl_Interp   *interp)   /* OUT - Tcl-interpreter set in case of errors */
{
unsigned32  line_count;
unsigned32  in_idx;
unsigned32  out_idx;
unsigned32  i;
unsigned32  dn_number;
signed16    total_len;
signed16    retval;

    gc_i_incomplete_result_message = NULL;
    gc_i_output_flag = GC_I_DIR_RESULT;
    in_idx = 0;
    out_idx = 0;
    dn_number = 1;
    line_count = 0;
    retval = GC_SUCCESS;

    i = 0;

    /* count vector items */
    while (in_vect[i].type != GC_T_EOL)
    {
	switch(in_vect[i].type)
	{
	    case (GC_T_DN):
	    case (GC_T_SATTRNAME):
	    case (GC_T_SCOMPNAME):
	    case (GC_T_COMPNAME):
		/* one line is needed for all these vectors */
		++line_count;
		++i;
		break;
	    case (GC_T_ATTRNAME):
		++line_count;
		++i;

		if (in_vect[i].type == GC_T_ATTRVAL)
		{
		    ++i;
		    /* count all the other attribute values, if any */
		    while (in_vect[i].type == GC_T_ATTRVAL)
		    {
			++line_count;
			++i;
		    }
		}
		break;
	    default:
		++i;
		break;
	}
    }

    /* initialize the print structure */
    retval = gc_i__init_out_struct(2, ":", line_count);

    if (retval == GC_SUCCESS)
    {
	/* start conversion */
	while (in_vect[in_idx].type != GC_T_EOL)
	{
	    while (in_vect[in_idx].type == GC_T_DN)
	    {
		gc_i_dir_result.out_vect[out_idx].is_header = TRUE;

		/* handle the DN */
		gc_i_dir_result.out_vect[out_idx].indent = 0;
		gc_i_dir_result.out_vect[out_idx].parts_count = 1;

		/* allocate the memory for all parts of the line */
		gc_i_dir_result.out_vect[out_idx].line_parts = 
					    (char **) malloc(sizeof(char *));

		if (gc_i_dir_result.out_vect[out_idx].line_parts == NULL)
		{
		    retval = GC_I_MEMORY_INSUFFICIENT;
		    break;
		}

		/* allocate the memory for the dn-headstring */
		total_len = gc_i__int_length(dn_number) +
			    strlen(in_vect[in_idx].value) + 5;

		gc_i_dir_result.out_vect[out_idx].line_parts[0] = 
				    (char *) malloc(sizeof(char) * total_len);

		if (gc_i_dir_result.out_vect[out_idx].line_parts[0] ==NULL)
		{
		    retval = GC_I_MEMORY_INSUFFICIENT;
		    free(gc_i_dir_result.out_vect[out_idx].line_parts);
		    break;
		}
	    
		sprintf(gc_i_dir_result.out_vect[out_idx].line_parts[0],
			"%2d) %s", dn_number, in_vect[in_idx].value);

		gc_i_dir_result.line_count = out_idx + 1;

		++out_idx;
		++in_idx;
		++dn_number;

		/* handle now the attributes of the DN */
		while (in_vect[in_idx].type == GC_T_ATTRNAME ||
		       in_vect[in_idx].type == GC_T_SATTRNAME)
		{
		    switch(in_vect[in_idx].type)
		    {
			case (GC_T_ATTRNAME): 
			    retval = gc_i__prepare_attr(GC_I_FIRST_LEVEL_INDENT,
							in_vect, &in_idx);
			    if (retval == GC_SUCCESS)
			    {
				out_idx = gc_i_dir_result.line_count;
			    }
			    break; 
			case (GC_T_SATTRNAME):
			    retval = gc_i__prepare_struct_attr(
							GC_I_FIRST_LEVEL_INDENT,
							in_vect, &in_idx);
			    if (retval == GC_SUCCESS)
			    {
				out_idx = gc_i_dir_result.line_count;
			    }
			    break; 
			default: 
			    break;
		    }

		    if (retval != GC_SUCCESS)
		    {
			break;
		    }
		}

		if (retval != GC_SUCCESS)
		{
		    break;
		}
	    }

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }

	    /* if it is any of these vectors, it means that the result */
	    /* is partial.					       */
	    if (in_vect[in_idx].type == GC_T_REF_DSA ||
		in_vect[in_idx].type == GC_T_REF_ADDR ||
		in_vect[in_idx].type == GC_T_LIMIT_PROBLEM ||
		in_vect[in_idx].type == GC_T_NO_CRIT_EXT)
	    {
		gc_i_incomplete_result_message = in_vect[in_idx].value;
		break;
	    }
	}
    }

    if (retval != GC_SUCCESS)
    {
	/* free the allocated memory */
	gc_i__free_out_struct();

	/* set the interpreter result to the appropriate error message */
	gc_i_set_interpreter_error(retval, NULL, interp);
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__prepare_attr                              */
/* Description   : This function looks for an attribute name or a  */
/*		   component name and its values in the result     */
/*		   vector and prepares the print structure to print*/
/*		   in a structured format.			   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*                 the type of error.				   */
/* Author        : M.Kronenburg, SNI BU BA KP13                    */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_i__prepare_attr(
    signed16      indent,     /* IN  - indent 		    */
    gc_t_vector  *in_vect,    /* IN  - Vector to convert    */
    unsigned32   *index)      /* I/O - index of this vector */
{
unsigned32  in_idx;    
unsigned32  out_idx;
signed16    attr_name_len;
signed16    parts_count;
signed16    retval;

    retval = GC_SUCCESS;
    in_idx = *index;
    out_idx = gc_i_dir_result.line_count;

    gc_i_dir_result.out_vect[out_idx].indent = indent;

    /* check if there is an attribute value */
    if (in_vect[in_idx+1].type == GC_T_ATTRVAL ||
        in_vect[in_idx+1].type == GC_T_COMPVAL)
    {
	parts_count = 2;
    }
    else
    {
	parts_count = 1;
    }

    gc_i_dir_result.out_vect[out_idx].parts_count = parts_count;
    gc_i_dir_result.out_vect[out_idx].is_header = FALSE;;

    /* malloc the space for the pointers of the line-parts */
    gc_i_dir_result.out_vect[out_idx].line_parts = 
				(char **) malloc(parts_count * sizeof(char *));

    if (gc_i_dir_result.out_vect[out_idx].line_parts == NULL)
    {
	retval = GC_I_MEMORY_INSUFFICIENT;
    }
    else
    {
	/* first the attribute name */
	gc_i_dir_result.out_vect[out_idx].line_parts[0] = in_vect[in_idx].value;

	gc_i__check_indentation(indent + strlen(in_vect[in_idx].value) + 1, 0);

	/* store the length of the attribute name */
	attr_name_len = strlen(in_vect[in_idx].value) + 1;

	++in_idx;

	/* now the first attribute value, if there is one */
	if (in_vect[in_idx].type == GC_T_ATTRVAL ||
	    in_vect[in_idx].type == GC_T_COMPVAL)
	{
	    gc_i_dir_result.out_vect[out_idx].line_parts[1] = 
							in_vect[in_idx].value; 
	    ++in_idx;
	}

	gc_i_dir_result.line_count = out_idx + 1;;
	++out_idx;

	/* if there are more than one value, they are written */
	/* in a separate line 				      */
	while (in_vect[in_idx].type == GC_T_ATTRVAL ||
	       in_vect[in_idx].type == GC_T_COMPVAL)
	{
	    gc_i_dir_result.out_vect[out_idx].indent = indent;
	    gc_i_dir_result.out_vect[out_idx].parts_count = 2;
	    gc_i_dir_result.out_vect[out_idx].is_header = FALSE;

	    /* malloc the space for the pointers of the line-parts */
	    gc_i_dir_result.out_vect[out_idx].line_parts = 
					(char **) malloc(sizeof(char *) * 2);

	    if (gc_i_dir_result.out_vect[out_idx].line_parts == NULL)
	    {
		retval = GC_I_MEMORY_INSUFFICIENT;
		break;
	    }

	    /* for multiple values the attribute name should not be repeated */
	    /* Hence create a blank name.			             */
	    gc_i_dir_result.out_vect[out_idx].line_parts[0] = ""; 

	    /* now the value */
	    gc_i_dir_result.out_vect[out_idx].line_parts[1] = 
							in_vect[in_idx].value;

	    ++in_idx;
	    gc_i_dir_result.line_count = out_idx + 1;;
	    ++out_idx;
	}
    }

    if (retval == GC_SUCCESS)
    {
	*index = in_idx;
    }

    return(retval);

}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__prepare_struct_attr                       */
/* Description   : This function looks for a structured attribute  */
/*		   name and its values in the result vector and    */
/*		   prepares the structure to print in a structured */
/*		   format.				   	   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*                 the type of error.				   */
/* Author        : M.Kronenburg, SNI BU BA KP13                    */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_i__prepare_struct_attr(
    signed16      indent,     /* IN  - indent for the attribut name  */
    gc_t_vector  *in_vect,    /* IN  - Vector to convert 	     */
    unsigned32   *index)      /* I/O - index of this vector 	     */
{
unsigned32  in_idx;
unsigned32  out_idx;
signed16    retval;

    retval = GC_SUCCESS;
    in_idx = *index;
    out_idx = gc_i_dir_result.line_count;

    gc_i_dir_result.out_vect[out_idx].indent = indent;
    gc_i_dir_result.out_vect[out_idx].parts_count = 1;
    gc_i_dir_result.out_vect[out_idx].is_header = FALSE;;

    gc_i_dir_result.out_vect[out_idx].line_parts = 
					(char **) malloc(sizeof(char *));

    if (gc_i_dir_result.out_vect[out_idx].line_parts == NULL)
    {
	retval = GC_I_MEMORY_INSUFFICIENT;
    }
    else
    {
	/* first the attribute name */
	gc_i_dir_result.out_vect[out_idx].line_parts[0] = in_vect[in_idx].value;

	gc_i__check_indentation(indent + strlen(in_vect[in_idx].value) + 1, 0);

	++in_idx;
	gc_i_dir_result.line_count = out_idx + 1;
	++out_idx;

	/* now the components */
	while (in_vect[in_idx].type == GC_T_COMPNAME ||
	       in_vect[in_idx].type == GC_T_SCOMPNAME)
	{
	    switch(in_vect[in_idx].type)
	    {
		case (GC_T_COMPNAME):
		    retval = gc_i__prepare_attr(indent + 
						GC_I_NORMAL_LEVEL_INDENT,
						in_vect, &in_idx);
		    if (retval == GC_SUCCESS)
		    {
			out_idx = gc_i_dir_result.line_count;
		    }
		    break;
		case (GC_T_SCOMPNAME):
		    retval = gc_i__prepare_struct_attr(indent + 
						       GC_I_NORMAL_LEVEL_INDENT,
						       in_vect, &in_idx);
		    if (retval == GC_SUCCESS)
		    {
			/* skip past GC_T_SCOMPEND */
			++in_idx;
			out_idx = gc_i_dir_result.line_count;
		    }
		    break;
		default:
		    break;
	    }

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }
	}
    }

    if (retval == GC_SUCCESS)
    {
	*index = in_idx;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i_prepare_showabbr_print_result              */
/* Description   : This function scans the result vector received  */
/*		   after a show operation on an x500abbr object    */
/*		   and prepares the print structure to print the   */
/*		   result in a structured format.	   	   */
/*		   In the case of errors, the TCL interpreter      */
/*		   result is set to the error message.		   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*                 the type of error.				   */
/* Author        : M. Kronenburg, SNI BU BA KP13                   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_i_prepare_showabbr_print_result(
    gc_t_vector  *in_vect,  /* IN  - Vector to convert 		            */
    Tcl_Interp   *interp)   /* OUT - Tcl-interpreter set in case of errors  */
{
unsigned32  vect_idx;
unsigned32  line_count;
unsigned32  i;
signed16    retval;
char  	   *header;

    vect_idx = 0;
    line_count = 0;
    gc_i_output_flag = GC_I_ABBR_SVC_RESULT;
    retval = GC_SUCCESS;

    i = 0;
    /* count vector items */
    while (in_vect[i].type != GC_T_EOL)
    {
	switch (in_vect[i].type)
	{
	    case (GC_T_OCLABBRV):
		/* one line is required for the header and one for */
		/* object class information.			   */
		line_count += 2;

		/* skip past the abbrv, full name and objid */
		i += 3;

		while (in_vect[i].type == GC_T_OCLABBRV)
		{
		    ++line_count;
		    i += 3;
		}
		break;
	    case (GC_T_ATTRABBRV):
		/* one line is required for the header and one for */
		/* attribute information.			   */
		line_count += 2;

		/* skip past the abbrv, full name and objid */
		i += 3;

		while (in_vect[i].type == GC_T_ATTRABBRV)
		{
		    ++line_count;
		    i += 3;
		}
		break;
	    case (GC_T_SATTRNAME):
		/* one line is required for the header */
		++line_count;
		++i;

		while (in_vect[i].type == GC_T_COMPABBRV)
		{
		    /* skip past the abbrv and full name */
		    i += 2;
		    ++line_count;
		}
		break;
	    default:
		break;
	}
    }

    retval = gc_i__init_out_struct(3, "  ", line_count);

    if (retval == GC_SUCCESS)
    {
	/* start conversion */
	while (in_vect[vect_idx].type != GC_T_EOL)
	{
	    switch(in_vect[vect_idx].type)
	    {
		case (GC_T_OCLABBRV):
		    /* get header string from catalog */
		    header = (char *) dce_sprintf(gc_i_obj_header);
		    retval = gc_i__prepare_normal_abbr(GC_T_OCLABBRV,
						       GC_T_OCLNAME,
						       GC_T_OBJID,
						       header,
						       in_vect, &vect_idx);
		    free(header);
		    break;
		case (GC_T_ATTRABBRV):
		    /* get header string from catalog */
		    header = (char *) dce_sprintf(gc_i_attr_header);
		    retval = gc_i__prepare_normal_abbr(GC_T_ATTRABBRV,
						       GC_T_ATTRNAME,
						       GC_T_OBJID,
						       header,
						       in_vect, &vect_idx);
		    free(header);
		    break;
		case (GC_T_SATTRNAME):
		    retval = gc_i__prepare_struct_attr_abbr(in_vect, &vect_idx);
		    break;
		default:
		    break;
	    }

	    if (retval != GC_SUCCESS)
	    {
		break;
	    }
	}
    }

    if (retval != GC_SUCCESS)
    {
	/* free the allocated memory */
	gc_i__free_out_struct();

	/* set the interpreter result to the appropriate error message */
	gc_i_set_interpreter_error(retval, NULL, interp);
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__prepare_normal_abbr                       */
/* Description   : This function scans the result vector received  */
/*		   after a show operation on an x500abbr object    */
/*		   It looks for the object class and attribute     */
/*		   abbreviation and prepares the print structure   */
/*		   to print the result in a structured format.     */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*                 the type of error.				   */
/* Author        : M. Kronenburg, SNI BU BA KP13                   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_i__prepare_normal_abbr(
    gc_t_vect_type  sym1,       /* IN  - 1. symbol in the gct_vector     */
    gc_t_vect_type  sym2,       /* IN  - 2. symbol in the gct_vector     */
    gc_t_vect_type  sym3,       /* IN  - 3. symbol in the gct_vector     */
    char           *head_string,/* IN  - string at the head of the block */
    gc_t_vector    *in_vect,    /* IN  - Vector to convert 	         */
    unsigned32     *in_index)   /* I/O - index of this vector 	         */
{
unsigned32  vect_idx;
unsigned32  out_idx;
signed16    retval;

    vect_idx = *in_index;
    out_idx = gc_i_abbr_svc_result.line_count;
    retval = GC_SUCCESS;
    
    /* headline for the abbreviations */
    gc_i_abbr_svc_result.out_vect[out_idx].indent = 0;
    gc_i_abbr_svc_result.out_vect[out_idx].parts_count = 1;
    gc_i_abbr_svc_result.out_vect[out_idx].is_header = TRUE;
    
    /* allocate the memory for the several parts */
    gc_i_abbr_svc_result.out_vect[out_idx].line_parts = 
					    (char **) malloc(sizeof(char *));

    if (gc_i_abbr_svc_result.out_vect[out_idx].line_parts == NULL)
    {
	retval = GC_I_MEMORY_INSUFFICIENT;
    }
    else
    {
	gc_i_abbr_svc_result.out_vect[out_idx].line_parts[0] = 
		    (char *) malloc(sizeof(char) * (strlen(head_string) + 1));

	if (gc_i_abbr_svc_result.out_vect[out_idx].line_parts[0] == NULL)
	{
	    retval = GC_I_MEMORY_INSUFFICIENT;
	    free(gc_i_abbr_svc_result.out_vect[out_idx].line_parts);
	}
	else
	{
	    strcpy(gc_i_abbr_svc_result.out_vect[out_idx].line_parts[0], 
		   head_string) ;

	    gc_i_abbr_svc_result.line_count = out_idx + 1;
	    ++out_idx;

	    /* scan all abbreviations */
	    while (in_vect[vect_idx].type == sym1)
	    {
		gc_i_abbr_svc_result.out_vect[out_idx].indent = 
							GC_I_FIRST_LEVEL_INDENT;
		gc_i_abbr_svc_result.out_vect[out_idx].parts_count = 3;
		gc_i_abbr_svc_result.out_vect[out_idx].is_header = FALSE;

		/* allocate the memory for all the parts of the line */
		gc_i_abbr_svc_result.out_vect[out_idx].line_parts = 
					(char **) malloc(sizeof(char *) * 3);

		if (gc_i_abbr_svc_result.out_vect[out_idx].line_parts == NULL)
		{
		    retval = GC_I_MEMORY_INSUFFICIENT;
		    break;
		}

		gc_i_abbr_svc_result.out_vect[out_idx].line_parts[0] = 
							in_vect[vect_idx].value;

		gc_i__check_indentation(strlen(in_vect[vect_idx].value) + 1 +
			         gc_i_abbr_svc_result.out_vect[out_idx].indent,
				 0);
	
		++vect_idx;

		/* the second part */
		gc_i_abbr_svc_result.out_vect[out_idx].line_parts[1] = 
							in_vect[vect_idx].value;

		gc_i__check_indentation(strlen(in_vect[vect_idx].value) + 1 +
			      gc_i_abbr_svc_result.out_vect[out_idx].indent +
			      gc_i_abbr_svc_result.div_sym_indent[0], 1);
	
		++vect_idx;

		/* the third part */
		gc_i_abbr_svc_result.out_vect[out_idx].line_parts[2] = 
							in_vect[vect_idx].value;

		gc_i_abbr_svc_result.line_count = out_idx + 1;

		++vect_idx;

		++out_idx;
	    }
	}
    }

    if (retval == GC_SUCCESS)
    {
	*in_index = vect_idx;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__prepare_struct_attr_abbr                  */
/* Description   : This function scans the result vector received  */
/*		   after a show operation on an x500abbr object    */
/*		   It looks for the structured attribute 	   */
/*		   abbreviations and prepares the print structure  */
/*		   to print the result in a structured format.     */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*                 the type of error.				   */
/* Author        : M. Kronenburg, SNI BU BA KP13                   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_i__prepare_struct_attr_abbr(
    gc_t_vector  *in_vect,     /* IN  - Vector to convert    */
    unsigned32   *in_index)    /* I/O - index of this vector */
{
unsigned32  vect_idx;
unsigned32  out_idx;
signed16    retval;

    retval = GC_SUCCESS;
    vect_idx = *in_index;
    out_idx = gc_i_abbr_svc_result.line_count;

    /* scan all component abbreviations */
    while (in_vect[vect_idx].type == GC_T_SATTRNAME)
    {
	/* the headline */
	gc_i_abbr_svc_result.out_vect[out_idx].parts_count = 1;
	gc_i_abbr_svc_result.out_vect[out_idx].indent = 0;
	gc_i_abbr_svc_result.out_vect[out_idx].is_header = TRUE;

	/* allocate the memory for the several parts */
	gc_i_abbr_svc_result.out_vect[out_idx].line_parts = 
					     (char **) malloc(sizeof(char *));

	if (gc_i_abbr_svc_result.out_vect[out_idx].line_parts == NULL)
	{
	    retval = GC_I_MEMORY_INSUFFICIENT;
	    break;
	}
	
	/* get the header string from the catalog */
	gc_i_abbr_svc_result.out_vect[out_idx].line_parts[0] =
				 (char *) dce_sprintf(gc_i_sattr_header,
						      in_vect[vect_idx].value);

	if (gc_i_abbr_svc_result.out_vect[out_idx].line_parts[0] == NULL)
	{
	    free(gc_i_abbr_svc_result.out_vect[out_idx].line_parts);
	    retval = GC_I_MEMORY_INSUFFICIENT;
	    break;
	}

	gc_i_abbr_svc_result.line_count = out_idx + 1;
	++vect_idx;
	++out_idx;
		 
	/* scan all component abbreviations for one component */
	while (in_vect[vect_idx].type == GC_T_COMPABBRV)
	{
	    gc_i_abbr_svc_result.out_vect[out_idx].parts_count = 2;
	    gc_i_abbr_svc_result.out_vect[out_idx].indent = 
						   GC_I_FIRST_LEVEL_INDENT;
	    gc_i_abbr_svc_result.out_vect[out_idx].is_header = FALSE;

	    /* allocate the memory for the several parts of the line */
	    gc_i_abbr_svc_result.out_vect[out_idx].line_parts = 
					(char **) malloc(sizeof(char *) * 2);

	    if (gc_i_abbr_svc_result.out_vect[out_idx].line_parts == NULL)
	    {
		retval = GC_I_MEMORY_INSUFFICIENT;
		break;
	    }

	    /* the first part, the component abbreviation */
	    gc_i_abbr_svc_result.out_vect[out_idx].line_parts[0] = 
							in_vect[vect_idx].value;

	    gc_i__check_indentation(strlen(in_vect[vect_idx].value) + 1 +
				 gc_i_abbr_svc_result.out_vect[out_idx].indent,
				 0);

	    ++vect_idx;

	    /* now the COMPNAME */
	    gc_i_abbr_svc_result.out_vect[out_idx].line_parts[1] = 
							in_vect[vect_idx].value;
	
	    gc_i__check_indentation(strlen(in_vect[vect_idx].value) + 1 +
			      gc_i_abbr_svc_result.out_vect[out_idx].indent +
			      gc_i_abbr_svc_result.div_sym_indent[0], 1);

	    gc_i_abbr_svc_result.line_count = out_idx + 1;
	    ++vect_idx;
	    ++out_idx;
	}

	if (retval != GC_SUCCESS)
	{
	    break;
	}
    }

    if (retval == GC_SUCCESS)
    {
	*in_index = vect_idx;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i_prepare_showdsc_print_result               */
/* Description   : This function scans the result vector received  */
/*		   after a show operation on an x500svc object and */
/*		   prepares the print structure to print the       */
/*		   result in a structured format.	   	   */
/*		   In the case of errors, the TCL interpreter      */
/*		   result is set to the error message.		   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*                 the type of error.				   */
/* Author        : M. Kronenburg, SNI BU BA KP13                   */
/*                                                                 */
/*-----------------------------------------------------------------*/
signed16 gc_i_prepare_showdsc_print_result(
    gc_t_vector  *in_vect,  /* IN  - Vector to convert 		    */
    Tcl_Interp   *interp)   /* OUT - Tcl-interpreter set in case of errors */
{
unsigned32  line_count;
signed16    vect_idx;
signed16    out_idx;
signed16    retval;
signed16    i;

    vect_idx = 0;
    out_idx = 0;
    line_count = 0;
    gc_i_output_flag = GC_I_ABBR_SVC_RESULT;
    retval = GC_SUCCESS;

    /* count vector items */
    for (i = 0; in_vect[i].type != GC_T_EOL; ++i)
    {
	if (in_vect[i].type == GC_T_ATTRNAME)
	{
	    ++line_count;
	}
    }

    retval = gc_i__init_out_struct(2, "-", line_count);

    if (retval == GC_SUCCESS)
    {
	/* start conversion */
	while (in_vect[vect_idx].type == GC_T_ATTRNAME)
	{
	    gc_i_abbr_svc_result.out_vect[out_idx].indent = 0;
	    gc_i_abbr_svc_result.out_vect[out_idx].parts_count = 2;
	    gc_i_abbr_svc_result.out_vect[out_idx].is_header = FALSE;

	    /* allocate the memory for the parts of the line */
	    gc_i_abbr_svc_result.out_vect[out_idx].line_parts = 
					(char **) malloc(sizeof(char *) * 2);

	    if (gc_i_abbr_svc_result.out_vect[out_idx].line_parts == NULL)
	    {
		retval = GC_I_MEMORY_INSUFFICIENT;
		break;
	    }

	    /* the first part */
	    gc_i_abbr_svc_result.out_vect[out_idx].line_parts[0] = 
						      in_vect[vect_idx].value;
	
	    gc_i__check_indentation(strlen(in_vect[vect_idx].value) + 1 +
			        gc_i_abbr_svc_result.out_vect[out_idx].indent, 
				0);
	
	    ++vect_idx;

	    /* the second part */
	    gc_i_abbr_svc_result.out_vect[out_idx].line_parts[1] = 
						       in_vect[vect_idx].value;
	    gc_i_abbr_svc_result.line_count = out_idx + 1;
	
	    ++vect_idx;

	    ++out_idx;
	}
    }

    if (retval != GC_SUCCESS)
    {
	/* free the allocated memory */
	gc_i__free_out_struct();

	/* set the interpreter result to the appropriate error message */
	gc_i_set_interpreter_error(retval, NULL, interp);
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__init_out_struct                           */
/* Description   : Initialize the global structures gc_i_dir_result*/
/*		   or gc_i_abbr_svc_result.    			   */
/* Return Value  : GC_SUCCESS or a negative integer indicating     */
/*                 the type of error.				   */
/* Author        : M. Kronenburg, SNI BU BA KP13                   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_i__init_out_struct(
    signed16     max_part,    /* IN  - max. parts in a line   */
    char        *div_symb,    /* IN  - the dividing symbols   */
    unsigned32   line_count)  /* IN  - number of lines	      */
{
gc_i_output_struct  *out_struct;
unsigned32  	     i;
signed16  	     retval;

    retval = GC_SUCCESS;

    if (gc_i_output_flag == GC_I_DIR_RESULT)
    {
	out_struct = &gc_i_dir_result;
    }
    else
    {
	out_struct = &gc_i_abbr_svc_result;
    }

    out_struct->div_sym = (char *) malloc(sizeof(char) * (max_part - 1));

    if (out_struct->div_sym == NULL)
    {
	retval = GC_I_MEMORY_INSUFFICIENT;
    }
    else
    {
	out_struct->div_sym_indent = (signed16 *) 
				      malloc(sizeof(signed16) * (max_part - 1));

	if (out_struct->div_sym_indent == NULL)
	{
	    retval = GC_I_MEMORY_INSUFFICIENT;
	    free(out_struct->div_sym);
	}
	else
	{
	    /* allocate the memory for all lines */
	    out_struct->out_vect = (gc_i_output_line *) 
				    malloc(sizeof(gc_i_output_line) * 
					   line_count);
	    if (out_struct->out_vect == NULL)
	    {
		retval = GC_I_MEMORY_INSUFFICIENT;
		free(out_struct->div_sym);
		free(out_struct->div_sym_indent);
	    }
	    else
	    {
		for (i = 0; i < max_part - 1; ++i)
		{
		    out_struct->div_sym[i] = div_symb[i];
		    out_struct->div_sym_indent[i] = 0;
		}
		out_struct->max_parts = max_part;
	    }
	}
    }
    
    if (retval != GC_SUCCESS)
    {
	out_struct->max_parts = 0;
	out_struct->div_sym = NULL;
	out_struct->div_sym_indent = NULL;
	out_struct->out_vect = NULL;
    }

    out_struct->line_count = 0;

    return(retval);
}



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__free_out_struct                           */
/* Description   : This function is used to free the allocated     */
/*		   memory of output print structure. 		   */
/* Return Value  : None                                            */
/* Author        : M. Kronenburg, SNI BU BA KP13                   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void gc_i__free_out_struct( 
    void)
{
gc_i_output_struct  *out_struct;
unsigned32  	     i;

    if (gc_i_output_flag == GC_I_DIR_RESULT)
    {
	out_struct = &gc_i_dir_result;
    }
    else
    {
	out_struct = &gc_i_abbr_svc_result;
    }

    if (out_struct->out_vect != NULL)
    {
	for (i = 0; i < out_struct->line_count; ++i)
	{
	    /* Only the header string is allocated by the interpreter. */
	    /* So only that should be freed.		               */ 
	    if (out_struct->out_vect[i].is_header == TRUE)
	    {
		free(out_struct->out_vect[i].line_parts[0]);
	    }

	    free(out_struct->out_vect[i].line_parts);
	}

	free(out_struct->out_vect);
    }

    if (out_struct->div_sym != NULL)
    {
	free(out_struct->div_sym);
    }

    if (out_struct->div_sym_indent != NULL)
    {
	free(out_struct->div_sym_indent);
    }

    out_struct->max_parts = 0;
    out_struct->div_sym = NULL;
    out_struct->div_sym_indent = NULL;
    out_struct->line_count = 0;
    out_struct->out_vect = NULL;
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__int_length                                */
/* Description   : This function computes the length of an integer.*/
/* Return Value  : length of the integer.                          */
/* Author        : M.Kronenburg, SNI BU BA KP13                    */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_i__int_length(
    signed32 number)  /* IN - number */
{
signed16 length = 1;

    number = abs(number);

    while (number > 9)
    {
        ++length;
        number /= 10;
    }

    /* if the number is less than 10, it should be printed with a preceding */
    /* space.								    */
    if (length == 1)
	length = 2;

    return length;
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__check_indentation                         */
/* Description   : This function checks the indentation specified  */
/*		   against the current indentation in the print    */
/*		   structure.  If the indentation is greater, then */
/*		   the print structure indentation is updated.     */
/* Return Value  : NONE						   */
/* Author        : M.Kronenburg, SNI BU BA KP13                    */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void gc_i__check_indentation(
    signed16  indent,  /* IN  - The indentation     */
    signed16  column)  /* IN  - Column Number       */
{
gc_i_output_struct  *out_struct;

    if (gc_i_output_flag == GC_I_DIR_RESULT)
    {
	out_struct = &gc_i_dir_result;
    }
    else
    {
	out_struct = &gc_i_abbr_svc_result;
    }

    if (indent > out_struct->div_sym_indent[column])
    {
	out_struct->div_sym_indent[column] = indent;
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i_print_result                               */
/* Description   : Displays the result of an read, search, list,   */
/*		   showabbrv or showdsc command and allows paging. */
/*		   The memory of the result structures are freed.  */
/* Return Value  : NONE                                            */
/* Author        : M. Kronenburg, SNI BU BA KP13                   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void gc_i_print_result(
    void)
{
gc_i_output_struct  *out_struct;

    /* check which structure should be printed */
    if (gc_i_output_flag == GC_I_DIR_RESULT)
    {
	out_struct = &gc_i_dir_result;
    }
    else
    {
	out_struct = &gc_i_abbr_svc_result;
    }

    /* check if there is anything to do */
    if (out_struct->line_count <= 0)
    {
	return;
    }

    /* if the session is not interactive output should not be paged */
    if (gc_i_interactive_mode == FALSE)
    {
	gc_i_first_line = 1;
	gc_i_last_line = out_struct->line_count;
	gc_i__print_page(TRUE);
    }
    else
    {
	/* set the first range to be printed */
	if (out_struct->line_count > GC_I_TERMINAL_SIZE - 1)
	{
	    gc_i_page_option = GC_I_NTH_PAGE;
	    gc_i_skip_pages = 1;
	    gc_i__compute_next_page();
        }
        else
        {
	    gc_i_first_line = 1;
	    gc_i_last_line = out_struct->line_count;
        }

        while (gc_i_page_option != GC_I_QUIT)
        {
	    gc_i__print_page(TRUE);
    
	    /* only if there is more than one page , paging is supported */
	    if (out_struct->line_count > GC_I_TERMINAL_SIZE - 1)
	    {
	        gc_i__read_cmd_line();
	        gc_i__compute_next_page();
	    }
	    else
	    {
	        gc_i_page_option = GC_I_QUIT;
	    }
        }
    }

    /* reset page_option */
    gc_i_page_option = GC_I_NTH_PAGE;

    /* now free the result structures */
    gc_i__free_out_struct();

    /* if there is any message about incomplete result, print it */
    if (gc_i_incomplete_result_message != NULL)
    {
	/* if the referral DSA name has been returned */
	if (*gc_i_incomplete_result_message == '/')
	{
	    dce_fprintf(stdout, gc_t_incomplete_referral_returned, 
			gc_i_incomplete_result_message);
	}
	else
	{
	    dce_fprintf(stdout, gc_t_incomplete_operation, 
			gc_i_incomplete_result_message);
	}
	gc_i_incomplete_result_message = NULL;
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__print_page                                */
/* Description   : Displays the selected page.                     */
/* Return Value  : The number of extra lines lines that exceed     */
/*		   GC_I_COLUMN_SIZE characters.			   */
/* Author        : M. Kronenburg, SNI BU BA KP13                   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static signed16 gc_i__print_page(
    boolean  print_flag) /* IN - Indicates if printing reqd or not */
{
gc_i_output_struct  *out_struct;
signed32	     i;
signed16  	     j, k;
signed16	     sym_indent;
signed16  	     col_count;
signed16  	     extra_lines;

    extra_lines = 0;

    if (gc_i_output_flag == GC_I_DIR_RESULT)
    {
	out_struct = &gc_i_dir_result;
    }
    else
    {
	out_struct = &gc_i_abbr_svc_result;
    }

    for (i = gc_i_first_line; i <= gc_i_last_line; ++i)
    {
	/* first the indent */
	if (print_flag == TRUE)
	{
	    for (j = 0; j < out_struct->out_vect[i-1].indent; ++j)
	    {
	        printf(" ");
	    }
	}

	col_count = out_struct->out_vect[i-1].indent;

	/* the parts of the line */
	for (j = 0; j < out_struct->out_vect[i-1].parts_count; ++j)
	{
	    if (print_flag == TRUE)
	    {
	        printf("%s", out_struct->out_vect[i-1].line_parts[j]);
	    }

	    col_count += strlen(out_struct->out_vect[i-1].line_parts[j]);

	    /* the dividing symbol if there is a following part */
	    if (out_struct->out_vect[i-1].parts_count-1 > j)
	    {
		sym_indent = out_struct->div_sym_indent[j] -
			     strlen(out_struct->out_vect[i-1].line_parts[j]) -
			     out_struct->out_vect[i-1].indent;

		/* if it isn't the first part, there is already an indent */
		if (j != 0)
		{
		    sym_indent -= out_struct->div_sym_indent[j-1];
		}
		    
		if (print_flag == TRUE)
		{
		    for (k = 0; k < sym_indent; ++k)
		    {
		        printf(" ");
		    }
		    printf("%c ", out_struct->div_sym[j]);
		}
		col_count = col_count + sym_indent + 2;
	    }
	}

	if (print_flag == TRUE)
	    printf("\n");

	extra_lines += (col_count/GC_I_COLUMN_SIZE);
    }

    /* if the session is not interactive, there is no paging */
    if  (gc_i_interactive_mode == TRUE)
    {
        if  (print_flag == TRUE)
        {
            /* print prompt & empty lines only if there is more than one page */
            if (out_struct->line_count > GC_I_TERMINAL_SIZE - 1)
            {
	        if (gc_i_last_line == out_struct->line_count)
	        {
	            sprintf(gc_i_prompt_string, "%s%s", 
			    GC_I_EOL_STRING, GC_I_PROMPT);
	        }
	        else
	        {
	            strcpy(gc_i_prompt_string, GC_I_PROMPT);
	        }
	        printf("%s", gc_i_prompt_string);
            }
        }
    }

    return(extra_lines);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__read_cmd_line                             */
/* Description   : This function reads the next commandline and    */
/*		   interprets it.				   */
/* Return Value  : None                                            */
/* Author        : M. Kronenburg, SNI BU BA KP13                   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void gc_i__read_cmd_line( 
    void)
{
struct termios   ttynew;
struct termios   ttysave_old;
signed16         cmd_line_pos;
signed16	 len;
signed16         i;
boolean          end;
char             c;
char             cmd_line[GC_I_CMD_LINE_LEN];

    end = FALSE;
    cmd_line_pos = 0;

    /* init the commandline */
    for (i = 0; i < GC_I_CMD_LINE_LEN; ++i)
    {
	cmd_line[i] = '\0';
    }

    fflush(stdin);

    /* store the old modus */
    tcgetattr(0, &ttysave_old);

    /* copy the old modus */
    ttynew = ttysave_old;

    /* set the new modus */
    ttynew.c_lflag = ttynew.c_lflag & ~(ICANON | ECHO);

    /* only one character is read */
    ttynew.c_cc[VMIN] = 1;

    /* no time */
    ttynew.c_cc[VTIME] = 0;

    tcsetattr(0, TCSANOW, &ttynew);

    while (end == FALSE)
    {
	switch(c = getchar())
	{
	    case ('$'):
		gc_i_page_option = GC_I_LAST_PAGE;
		end = TRUE;
		break;
	    case (' '):
		gc_i_page_option = GC_I_SKIP_FORWARD;
		gc_i_skip_pages = 1;
		end = TRUE;
		break;
	    case ('q'):
		gc_i_page_option = GC_I_QUIT;
		end = TRUE;
		break;
	    case ('\n'):
		if (cmd_line_pos == 0)
		{
		    gc_i_page_option = GC_I_NEXT_LINE;
		}
		else
		{
		    /* first case : only '+' or '-' was inserted */
		    if ((cmd_line_pos == 1) && 
			(cmd_line[0] == '+' || cmd_line[0] == '-'))
		    {
			if (cmd_line[0] == '+')
			{
			    gc_i_page_option = GC_I_SKIP_FORWARD;
			}
			else
			{
			    gc_i_page_option = GC_I_SKIP_BACKWARD;
			}

			gc_i_skip_pages = 1;
		    }
		    else 
		    {
			if (cmd_line[0] == '+' || cmd_line[0] == '-')
			{
			    if (cmd_line[0] == '+')
			    {
				gc_i_page_option = GC_I_SKIP_FORWARD;
			    }
			    else
			    {
				gc_i_page_option = GC_I_SKIP_BACKWARD;
			    }
			    gc_i_skip_pages = atoi(cmd_line + 1);
			}
			else
			{
			    gc_i_page_option = GC_I_NTH_PAGE;
			    gc_i_skip_pages = atoi(cmd_line);
			}
		    }
		}

		end = TRUE;
		break;
	    case ('-'):
	    case ('+'): 
		if (cmd_line_pos == 0)
		{
		    cmd_line[0] = c;
		    printf("%c",c);
		    ++cmd_line_pos;
		}
		/* if not the first character forget it ! */
		else
		{
		    putchar(7);
		}
		break;

	    case ('0'):
	    case ('1'):
	    case ('2'):
	    case ('3'):
	    case ('4'):
	    case ('5'):
	    case ('6'):
	    case ('7'):
	    case ('8'):
	    case ('9'):  
		/* a null at the beginning is ignored */
		if (cmd_line_pos == 0 && c == '0')
		{
		    putchar(7);
		    break;
		}

		/* '-1' because the last character must be the NULL-char. */
		if (cmd_line_pos < GC_I_CMD_LINE_LEN - 1)
		{
		    cmd_line[cmd_line_pos] = c;
		    ++cmd_line_pos;
		    printf("%c",c);
		}
		/* if the commandline is full, forget it */
		break;

	    case ('\b'):
		if (cmd_line_pos != 0)
		{
		    cmd_line_pos--;
		    cmd_line[cmd_line_pos] = '\0';
		    printf("%c",c);
		}
		else
		{
		    putchar(7);
		}
		break;
	    
	    default:
		putchar(7);
		/* ignore all other characters */
		break;
	}
    }

    len = strlen(gc_i_prompt_string) + strlen(cmd_line);

    for (i = 0; i < len ; ++i)
	printf("\b");

    for (i = 0; i < len; ++i)
	printf(" ");

    for (i = 0; i < len ; ++i)
	printf("\b");

    if (c == 'q')
	printf("\n");

    /* set the old modus */
    tcsetattr(0, TCSANOW, &ttysave_old);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : gc_i__compute_next_page                         */
/* Description   : This function computes next "first" & "lastline"*/
/* Return Value  : None                                            */
/* Author        : M. Kronenburg, SNI BU BA KP13                   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void gc_i__compute_next_page( 
    void)
{
gc_i_output_struct  *out_struct;
signed16  	     extra_lines;

    if (gc_i_output_flag == GC_I_DIR_RESULT)
    {
	out_struct = &gc_i_dir_result;
    }
    else
    {
	out_struct = &gc_i_abbr_svc_result;
    }

    switch(gc_i_page_option)
    {
	case (GC_I_NTH_PAGE):
	    gc_i_first_line = ((gc_i_skip_pages - 1) * (GC_I_TERMINAL_SIZE - 1))
			       + 1;
	    gc_i_last_line  = gc_i_first_line + GC_I_TERMINAL_SIZE - 2;
            break;
	case (GC_I_SKIP_BACKWARD):
	    gc_i_first_line = gc_i_first_line - 
			      (gc_i_skip_pages * (GC_I_TERMINAL_SIZE - 1));

	    /* To show one line from previous page */
	    ++gc_i_first_line;

	    if (gc_i_first_line < 1)
	    {
		gc_i_first_line = 1;
	    }

	    gc_i_last_line = gc_i_first_line + GC_I_TERMINAL_SIZE - 2; 
            break;
	case (GC_I_SKIP_FORWARD):
	    gc_i_last_line = gc_i_last_line + 
			     (gc_i_skip_pages * (GC_I_TERMINAL_SIZE - 1));
	    gc_i_first_line = gc_i_last_line - GC_I_TERMINAL_SIZE + 2;

	    /* To show one line from previous page */
	    --gc_i_last_line;
            break;
	case (GC_I_LAST_PAGE):
	    gc_i_last_line  = out_struct->line_count;
	    gc_i_first_line = gc_i_last_line - GC_I_TERMINAL_SIZE + 2;
            break;
	case (GC_I_QUIT):
            break;
	case (GC_I_NEXT_LINE):
	    gc_i_first_line = gc_i_last_line + 1;
	    gc_i_last_line  = gc_i_first_line;
            break;
	default:
            break;
    }

    if (gc_i_last_line >= out_struct->line_count || gc_i_last_line < 1)
    {
	gc_i_last_line = out_struct->line_count;
	gc_i_first_line = MAX(gc_i_last_line - GC_I_TERMINAL_SIZE + 2, 1);
    }

    if (gc_i_first_line > out_struct->line_count)
    {
	gc_i_first_line = MAX(out_struct->line_count - GC_I_TERMINAL_SIZE + 2, 
			      1);
	gc_i_last_line = out_struct->line_count;
    }
    else
    {
	if (gc_i_first_line < 1)
	{
	    gc_i_first_line = 1;
	    gc_i_last_line = MIN(gc_i_first_line + GC_I_TERMINAL_SIZE - 2, 
				 out_struct->line_count);
	}
    }

    if (gc_i_last_line != out_struct->line_count)
    {
        /* compute number of lines that exceed GC_I_COLUMN_SIZE characters */
        extra_lines = gc_i__print_page(FALSE);

	if (gc_i_last_line - extra_lines >= gc_i_first_line)
	{
	    gc_i_last_line -= extra_lines;
	}
    }
}
