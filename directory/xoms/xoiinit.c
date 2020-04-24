/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xoiinit.c,v $
 * Revision 1.1.4.2  1996/03/11  13:25:12  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:25  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:51:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:40  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:22:47  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:45  devsrc]
 * 
 * Revision 1.1.2.2  1994/03/23  15:50:31  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:38:56  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:21  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:22:31  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:58:37  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOI - XOM Object Info Module                      */
/* File Name   : xoiinit.c                                         */
/* Description : This file contains modules that read the schema   */
/*		 file and initializes the private data structures  */
/*		 for use by the convenience functions		   */
/* Date        : July 1, 1993                                      */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: xoiinit.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:25:12 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <xom.h>

#include <xoistruct.h>
#include <xoi.h>
#include <xoisort.h>
#include <xoiutil.h>
#include <xoidebug.h>
#include <xoiinit.h>

/*-- Static Variable(s) -------------------------------------------*/
static	FILE		   *xoi_schema_file;/* Pointer to Schema file        */
static  xoi_object_class    xoi_obj;        /* To store Object Class Details */
static  xoi_attribute	    xoi_attr;       /* To store Attribute Details    */
static  xoi_omclass_struct  xoi_omclass;    /* To store OM Class Details     */
static  xoi_cinfo	   *xoi_cinfo_mem;  /* Array of Class Info.	     */
static  OM_uint		    xoi_cinfo_index;/* Index into xoi_cinfo_mem	     */
static	xoi_tclass	    xoi_cenc;	    /* Temporary Array to store      */
					    /* OM Class details encountered  */
					    /* while reading schema file     */

/*-- Static Function(s) -------------------------------------------*/
static OM_integer xoi__allocate_memory(
    OM_uint *line_no); /* OUT - Contains erroneous line No.in case of errors */

static OM_integer xoi__get_class_block_size(
    OM_uint     *comp_count,       /* OUT - Number of components of the Class */
    OM_uint	*comp_class_count, /* OUT - Number of encountered OM Classes  */
    OM_boolean  *eof_flag, 	   /* OUT - Value is OM_TRUE if End of File   */
    OM_uint     *line_no); 	   /* OUT - Contains erroneous line Number    */

static OM_integer xoi__process_objclass_block(
    OM_uint *line_no); /* OUT - Contains erroneous line No.in case of errors */

static OM_integer xoi__process_attribute_block(
    OM_uint *line_no); /* OUT - Contains erroneous line No.in case of errors */

static OM_integer xoi__process_omclass_blocks(
    OM_uint *line_no); /* OUT - Contains erroneous line No.in case of errors */

static OM_integer xoi__read_omclass_block(
    OM_uint     *line_no,   /* OUT - Erroneous line No.in case of error */
    OM_boolean  *eof_flag); /* OUT - Value is OM_TRUE if End of File    */

static OM_integer xoi__load_objclass_det(
    char *abbrv,      /* IN - Abbreviation	       */
    char *full_name,  /* IN - Full Name	  	       */
    char *objid);     /* IN - Object Identifier string */

static OM_integer xoi__load_attribute_det(
    OM_uint  line_no, 	  /* IN - Line Number in Schema file */
    char    *abbrv, 	  /* IN - Abbreviation		     */
    char    *full_name,   /* IN - Full Name		     */
    char    *objid, 	  /* IN - Object Identifier string   */
    char    *attr_syntax, /* IN - Attribute Syntax 	     */
    char    *class_str);  /* IN - Class Name string	     */

static OM_integer xoi__load_classcomp_det(
    OM_uint  line_no, 	  /* IN - Pointer to the Structure */
    char    *abbrv, 	  /* IN - Abbreviation	           */
    char    *full_name,   /* IN - Full Name		   */	
    OM_type  omtype,      /* IN - OM Type		   */
    char    *comp_syntax, /* IN - OM Syntax		   */	
    char    *comp_class); /* IN - OM Class Name	 	   */

static void xoi__free_tclass(
    void);

static void xoi__free_classcomp(
    xoi_class *class); /* IN - Pointer to Class structure */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_init                                   	   */
/* Description   : This function is used to read the Schema file   */
/*                 and build up the internal data structures       */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_init(
    OM_uint *line_no)  /* OUT - Contains erroneous line No.in case of errors */
{
OM_integer   retval;
OM_boolean   completed;
char	     file_name[XOI_MAX_LINE + 1];

    /* Initialize the structures */
    xoi_obj.count     = 0;
    xoi_obj.obj       = NULL;
    xoi_attr.count    = 0;
    xoi_attr.attr     = NULL;
    xoi_omclass.count = 0;
    xoi_omclass.class = NULL;
    xoi_cinfo_mem     = NULL;
    xoi_cinfo_index   = 0;
    xoi_cenc.count    = 0;
    xoi_cenc.tinfo    = NULL;
    xoi_schema_file   = NULL;

    *line_no = 0;

    /* get the file name */
    xoi_get_schema_file_name(file_name);

    if (*file_name != '\0')
    {
	xoi_schema_file = fopen(file_name, "r");
    }

    if (xoi_schema_file == NULL)
    {
	return(XOI_CANNOT_READ_SCHEMA);
    }

    completed = OM_FALSE;
    retval = XOI_SUCCESS;

    while (completed == OM_FALSE)
    {
	/* Read the file and allocate memory */
	if ((retval = xoi__allocate_memory(line_no)) != XOI_SUCCESS)
	    break;

	/* Read and process the Object Class Definition Block */
	if ((retval = xoi__process_objclass_block(line_no)) != XOI_SUCCESS)
	    break;

	/* Read and process the Attribute Definition Block */
	if ((retval = xoi__process_attribute_block(line_no)) != XOI_SUCCESS)
	    break;

	/* Read and process the OM Class Definition Blocks */
	if((retval = xoi__process_omclass_blocks(line_no)) != XOI_SUCCESS)
	    break;

#ifdef XOI_DEBUG
	xoi_debug_objclass_block();	
	xoi_debug_attribute_block();
#endif

	/* Since file successfully read, reset line_no */
	*line_no = 0;

	/* Sort the Information */
	if ((retval = xoi_sort_info()) != XOI_SUCCESS)
	    break;

	completed = OM_TRUE;
    }

    /* Free the temporarily allocated Class Name structures */
    xoi__free_tclass();

    /* Close the Schema file */
    fclose(xoi_schema_file);

    if (completed == OM_FALSE)
    {
	/* Free the Allocated Data Structures */
	xoi_free_blockinfo();
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi__allocate_memory                            */
/* Description   : This function is used to scan the Schema file   */
/*                 and compute the total number of Object Classes  */
/*		   Attributes and OM Class Names, and then allocate*/
/*		   the required size for the data structures.  	   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_integer xoi__allocate_memory(
    OM_uint *line_no)  /* OUT - Contains erroneous line No.in case of errors */
{
OM_integer  retval;
OM_uint     oinfo_alloc_size, ainfo_alloc_size, cinfo_alloc_size;
OM_uint	    class_alloc_size, tinfo_alloc_size;
OM_uint	    comp_class_count, comp_count;
OM_syntax   omsyntax;
OM_boolean  eof_flag;
char 	    input_line[XOI_MAX_LINE + 1]; 
char	    abbrv[XOI_MAX_ABBRV + 1]; 
char	    full_name[XOI_MAX_NAME + 1];
char 	    attr_syntax[XOI_MAX_FLD + 1]; 
char	    class_str[XOI_MAX_FLD + 1];
char 	    objid[XOI_MAX_OBJID + 1];
char	    format_str[XOI_MAX_NAME + 1];

    oinfo_alloc_size = 0;
    ainfo_alloc_size = 0;
    tinfo_alloc_size = 0;
    cinfo_alloc_size = 0;
    class_alloc_size = 0;

    /* Get the start of Object Class Definition Block */
    *line_no = xoi_get_next_line(xoi_schema_file, input_line);

    /* If no start of Object Class Definition Block */
    if (*input_line != XOI_START_BLOCK)
        return(XOI_NO_START_OBJ_BLOCK);

    /* Prepare the format string for sscanf */
    sprintf(format_str, "%%%ds %%%ds %%%ds", 
		        XOI_MAX_ABBRV, XOI_MAX_NAME, XOI_MAX_OBJID);

    while (OM_TRUE)
    {
	*line_no = xoi_get_next_line(xoi_schema_file, input_line);
	
	/* If end of file, start of block or end of block */
	if ((!*input_line) || (*input_line == XOI_END_BLOCK)
			   || (*input_line == XOI_START_BLOCK))
	    break;

	if (sscanf(input_line, format_str, abbrv, full_name, objid) 
		  != XOI_OBJ_BLOCK_FLD)
 	{
	    return(XOI_OBJ_FORMAT_ERROR);
	}

	++oinfo_alloc_size;
    }

    if (*input_line != XOI_END_BLOCK)
        return(XOI_NO_END_OBJ_BLOCK);

    if (oinfo_alloc_size == 0)
        return(XOI_EMPTY_OBJ_BLOCK);

    /* Get the start of Attribute Definition Block */
    *line_no = xoi_get_next_line(xoi_schema_file, input_line);

    /* If no start of Attribute Definition Block */
    if (*input_line != XOI_START_BLOCK)
	return(XOI_NO_START_ATTR_BLOCK);

    /* Prepare the format string for sscanf */
    sprintf(format_str, "%%%ds %%%ds %%%ds %%%ds %%%ds", 
		        XOI_MAX_ABBRV, XOI_MAX_NAME, XOI_MAX_OBJID,
		        XOI_MAX_FLD, XOI_MAX_FLD);
    while (OM_TRUE)
    {
	*line_no = xoi_get_next_line(xoi_schema_file, input_line);

	/* If end of file, start of block or end of block */
	if ((!*input_line) || (*input_line == XOI_END_BLOCK)
			   || (*input_line == XOI_START_BLOCK))
	    break;

	if (sscanf(input_line, format_str, 
		   abbrv, full_name, objid, attr_syntax, class_str) 
		!= XOI_ATTR_BLOCK_FLD)
	{
	    return(XOI_ATTR_FORMAT_ERROR);
	}

        /* Convert to XOM Syntax */
        if ((retval = xoi_to_omsyntax(attr_syntax, &omsyntax)) != XOI_SUCCESS)
	    return(retval);

	/* If structured attribute */
	if (omsyntax == OM_S_OBJECT)
	    ++tinfo_alloc_size;

	++ainfo_alloc_size;
    }

    if (*input_line != XOI_END_BLOCK)
	return(XOI_NO_END_ATTR_BLOCK);

    if (ainfo_alloc_size == 0)
	return(XOI_EMPTY_ATTR_BLOCK);

    /* If OM Class Names were encountered in the Attribute Definition Block */
    if (tinfo_alloc_size > 0)
    {
	while (OM_TRUE)
	{
	    /* Get the number of components and OM Class Names in the  */
	    /* OM Class Definition Block.			       */
	    retval = xoi__get_class_block_size(&comp_count, &comp_class_count,
					       &eof_flag, line_no);
	    if (retval != XOI_SUCCESS)
		return(retval);
	    if (eof_flag == OM_TRUE)
		break;
	    cinfo_alloc_size += comp_count;
	    tinfo_alloc_size += comp_class_count;
	    ++class_alloc_size;
	}
    }

    /* Allocate the memory for Object Classes*/
    xoi_obj.obj = (xoi_oinfo *) malloc(sizeof(xoi_oinfo) * oinfo_alloc_size);

    if (xoi_obj.obj == NULL)
	return(XOI_MEMORY_INSUFFICIENT);

    /* Allocate the memory for Attributes */
    xoi_attr.attr = (xoi_ainfo *) malloc(sizeof(xoi_ainfo) * ainfo_alloc_size);
    if (xoi_attr.attr == NULL)
	return(XOI_MEMORY_INSUFFICIENT);
 
    /* Allocate the memory for the temporary OM Class structures */
    if (tinfo_alloc_size > 0)
    {
	xoi_cenc.tinfo = (xoi_tinfo *) 
			  malloc(sizeof(xoi_tinfo) * tinfo_alloc_size);
	if (xoi_cenc.tinfo == NULL)
	    return(XOI_MEMORY_INSUFFICIENT);
    }

    if (cinfo_alloc_size > 0)
    {
	/* Allocate the memory for the components of the OM Class */
	xoi_cinfo_mem = (xoi_cinfo *)
			 malloc(sizeof(xoi_cinfo) * cinfo_alloc_size);
	if (xoi_cinfo_mem == NULL)
	    return(XOI_MEMORY_INSUFFICIENT);

	/* Allocate the memory for the OM Class structures */
	xoi_omclass.class = (xoi_class *)
			     malloc(sizeof(xoi_class) * class_alloc_size);
	if (xoi_omclass.class == NULL)
	    return(XOI_MEMORY_INSUFFICIENT);
    }
    
    /* Reposition file pointer to start of file */
    rewind(xoi_schema_file);

    /* Reset Line Number to zero */
    *line_no = 0;

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi__get_class_block_size                       */
/* Description   : This function is used to scan the OM Class      */
/*                 Definition Block in the Schema file and compute */
/*		   the total number of components of the Class. It */
/*		   also computes the total number of OM Classes    */
/*		   encountered in the block. 	   		   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_integer xoi__get_class_block_size(
    OM_uint     *comp_count,       /* OUT - Number of components of the Class */
    OM_uint	*comp_class_count, /* OUT - Number of encountered OM Classes  */
    OM_boolean  *eof_flag, 	   /* OUT - Value is OM_TRUE if End of File   */
    OM_uint     *line_no)  	   /* OUT - Contains erroneous line Number    */
{
OM_integer retval;
OM_syntax  omsyntax;
OM_type	   omtype;
char 	   input_line[XOI_MAX_LINE + 1]; 
char	   abbrv[XOI_MAX_ABBRV + 1]; 
char	   full_name[XOI_MAX_NAME + 1];
char       comp_syntax[XOI_MAX_FLD + 1];
char	   comp_class[XOI_MAX_FLD + 1];
char	   format_str[XOI_MAX_NAME + 1];

    *comp_count = 0;
    *comp_class_count = 0;
    *eof_flag = OM_FALSE;

    *line_no = xoi_get_next_line(xoi_schema_file, input_line);

    /* If End of file reached */
    if (!*input_line)
    {
	*eof_flag = OM_TRUE;
	return(XOI_SUCCESS);
    }

    if (*input_line != XOI_START_BLOCK)
	return(XOI_NO_START_CLASS_BLOCK);

    /* Get the OM Class Name */
    *line_no = xoi_get_next_line(xoi_schema_file, input_line);

    if (!*input_line)
	return(XOI_NO_CLASS_NAME);

    /* Prepare the format string for sscanf */
    sprintf(format_str, "%%%ds %%%ds %%hu %%%ds %%%ds", 
		        XOI_MAX_ABBRV, XOI_MAX_NAME, XOI_MAX_FLD, XOI_MAX_FLD);
    while (OM_TRUE)
    {
	*line_no = xoi_get_next_line(xoi_schema_file, input_line);
	if ((!*input_line) || (*input_line == XOI_END_BLOCK)
			   || (*input_line == XOI_START_BLOCK))
	    break;

	/* Break the line into its fields */
	if (sscanf(input_line, format_str, 
	    	   abbrv, full_name, &omtype, comp_syntax, comp_class) 
		   != XOI_CLASS_BLOCK_FLD)
        {
	    return(XOI_CLASS_FORMAT_ERROR);
        }

	/* Convert to XOM Syntax */
	if ((retval = xoi_to_omsyntax(comp_syntax, &omsyntax)) != XOI_SUCCESS)
	    return(retval);

	/* If component is structured */
	if (omsyntax == OM_S_OBJECT)
	    ++*comp_class_count;
	
	++*comp_count;
    }

    if (*input_line != XOI_END_BLOCK)
	return(XOI_NO_END_CLASS_BLOCK);

    if (*comp_count == 0)
	return(XOI_EMPTY_CLASS_BLOCK);

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi__process_objclass_block                     */
/* Description   : This function is used to read and process the   */
/*                 Object Class Definition Block in the Schema file*/
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_integer xoi__process_objclass_block(
    OM_uint *line_no)  /* OUT - Contains erroneous line No.in case of errors */
{
OM_integer  retval;
char        input_line[XOI_MAX_LINE + 1]; 
char        abbrv[XOI_MAX_ABBRV + 1]; 
char        full_name[XOI_MAX_NAME + 1];
char        objid[XOI_MAX_OBJID + 1];
char        format_str[XOI_MAX_NAME + 1];

    /* Get the start of Object Class Definition Block */
    *line_no = xoi_get_next_line(xoi_schema_file, input_line);

    retval = XOI_SUCCESS;

    /* Prepare the format string for sscanf */
    sprintf(format_str, "%%%ds %%%ds %%%ds", 
		        XOI_MAX_ABBRV, XOI_MAX_NAME, XOI_MAX_OBJID);
    while (OM_TRUE)
    {
	*line_no = xoi_get_next_line(xoi_schema_file, input_line);

	if (*input_line == XOI_END_BLOCK)
	    break;

	/* Break the line into its fields */
	sscanf(input_line, format_str, abbrv, full_name, objid);

	/* Load the Object Class Structures */
	retval = xoi__load_objclass_det(abbrv, full_name, objid);

	if (retval != XOI_SUCCESS)
	    break;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi__load_objclass_det                    	   */
/* Description   : This function validates and loads the Object    */
/*                 Class details into the relevant structures.     */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_integer xoi__load_objclass_det(
    char *abbrv,      /* IN - Abbreviation	       */
    char *full_name,  /* IN - Full Name	  	       */
    char *objid)      /* IN - Object Identifier string */
{
OM_integer   retval;
OM_string   *e_objid;
OM_uint	     index;
OM_boolean   completed;
char	    *p1, *p2;
char	     c[XOI_MAX_ABBRV + 1];

    /* Convert abbreviation to lower case */
    xoi_strcpy_to_lower(c, abbrv);

    p1 = p2 = NULL;
    e_objid = NULL;
    retval = XOI_SUCCESS;
    completed = OM_FALSE;

    while (completed == OM_FALSE)
    {
	/* Validate the abbreviation */
	if ((retval = xoi_validate_abbrv(c)) != XOI_SUCCESS)
	    break;

	/* Duplicate the Abbreviation */
	if ((p1 = xoi_strdup(c)) == NULL)
	{
	    retval = XOI_MEMORY_INSUFFICIENT;
	    break;
	}

	/* Duplicate the Full Name */
	if ((p2 = xoi_strdup(full_name)) == NULL)
	{
	    retval = XOI_MEMORY_INSUFFICIENT;
	    break;
	}

        /* Convert to BER Encoded Object Identifier */
        if ((retval = xoi_string_to_ber(objid, &e_objid)) != XOI_SUCCESS)
	    break;

	completed = OM_TRUE;
    }

    if (completed == OM_FALSE)
    {
	if (p1 != NULL)
	    free(p1);
	if (p2 != NULL)
	    free(p2);
	if (e_objid != NULL)
	    xoi_free_omstring(e_objid);
    }
    else
    {
	/* Data is error free. Now load all the structure members */
	index = xoi_obj.count;
	xoi_obj.obj[index].abbrv   = p1;
	xoi_obj.obj[index].name    = p2;
	xoi_obj.obj[index].e_objid = e_objid;
	++(xoi_obj.count);
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi__process_attribute_block                    */
/* Description   : This function is used to read and process the   */
/*                 Attribute Definition Block in the Schema file   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_integer xoi__process_attribute_block(
    OM_uint *line_no)  /* OUT - Contains erroneous line No.in case of errors */
{
OM_integer  retval;
char        input_line[XOI_MAX_LINE + 1]; 
char        abbrv[XOI_MAX_ABBRV + 1]; 
char        full_name[XOI_MAX_NAME + 1];
char        attr_syntax[XOI_MAX_FLD + 1]; 
char        class_str[XOI_MAX_FLD + 1];
char        objid[XOI_MAX_OBJID + 1];
char        format_str[XOI_MAX_NAME + 1];


    /* Get the start of Attribute Definition Block */
    *line_no = xoi_get_next_line(xoi_schema_file, input_line);

    /* Prepare the format string for sscanf */
    sprintf(format_str, "%%%ds %%%ds %%%ds %%%ds %%%ds", 
		        XOI_MAX_ABBRV, XOI_MAX_NAME, XOI_MAX_OBJID,
		        XOI_MAX_FLD, XOI_MAX_FLD);

    while (OM_TRUE)
    {
	*line_no = xoi_get_next_line(xoi_schema_file, input_line);

	if (*input_line == XOI_END_BLOCK)
	    break;

	/* Break the line into its fields */
	sscanf(input_line, format_str, 
			   abbrv, full_name, objid, attr_syntax, class_str);

	/* Load the Attribute Structures */
	retval = xoi__load_attribute_det(*line_no, abbrv, full_name, objid, 
					  attr_syntax, class_str);

	if (retval != XOI_SUCCESS)
	    return(retval);
    }

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi__load_attribute_det                   	   */
/* Description   : This function validates and loads the Attribute */
/*                 details into the relevant structures.           */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_integer xoi__load_attribute_det(
    OM_uint  line_no, 	  /* IN - Line Number in Schema file */
    char    *abbrv, 	  /* IN - Abbreviation		     */
    char    *full_name,   /* IN - Full Name		     */
    char    *objid, 	  /* IN - Object Identifier string   */
    char    *attr_syntax, /* IN - Attribute Syntax 	     */
    char    *class_str)	  /* IN - Class Name string	     */
{
OM_integer   retval;
OM_string   *e_objid;
OM_string   *omclass;
OM_syntax    omsyntax;
OM_uint	     attr_index, tclass_index;
OM_boolean   completed;
char	    *p1, *p2;
char	     c[XOI_MAX_ABBRV + 1];

    p1 = p2 = NULL;
    omclass = e_objid = NULL;
    completed = OM_FALSE;

    while (completed == OM_FALSE)
    {
	/* Convert abbreviation to lower case */
	xoi_strcpy_to_lower(c, abbrv);

	/* Validate the abbreviation */
	if ((retval = xoi_validate_abbrv(c)) != XOI_SUCCESS)
	    break;

	/* Duplicate the Abbreviation */
	if ((p1 = xoi_strdup(c)) == NULL)
	{
	    retval = XOI_MEMORY_INSUFFICIENT;
	    break;
	}

	/* Duplicate the Full Name */
	if ((p2 = xoi_strdup(full_name)) == NULL)
	{
	    retval = XOI_MEMORY_INSUFFICIENT;
	    break;
	}

	/* Convert to BER Encoded Object Identifier */
	if ((retval = xoi_string_to_ber(objid, &e_objid)) != XOI_SUCCESS)
	    break;

        /* Convert to XOM Syntax */
        if ((retval = xoi_to_omsyntax(attr_syntax, &omsyntax)) != XOI_SUCCESS)
	    break;

	/* If not a structured attribute */
	if (omsyntax != OM_S_OBJECT)
	{
	    /* For normal attributes, the OM Class should be 0 */
	    if (strcmp(class_str, "0") != 0)
	    {
		retval = XOI_ATTR_FORMAT_ERROR;
	        break;
	    }
	}
	else
	{
	    /* Convert to XOM Class Identifier */
	    if ((retval = xoi_to_omclass(class_str, &omclass)) != XOI_SUCCESS)
		break;
	}
	completed = OM_TRUE;
    }

    if (completed == OM_FALSE)
    {    
	if (p1 != NULL)
	    free(p1);
	if (p2 != NULL)
	    free(p2);
	if (e_objid != NULL)
	    xoi_free_omstring(e_objid);
	if (omclass != NULL)
	    xoi_free_omstring(omclass);
	return(retval);
    }

    /* Data is error free. Now initialize the structure members */
    attr_index = xoi_attr.count;

    xoi_attr.attr[attr_index].abbrv    = p1;
    xoi_attr.attr[attr_index].name     = p2;
    xoi_attr.attr[attr_index].e_objid  = e_objid;
    xoi_attr.attr[attr_index].omsyntax = omsyntax;

    /* Initialize the pointer to Class structure NULL. Will be */
    /* reinitialized later for structured attributes           */
    xoi_attr.attr[attr_index].class = NULL;

    ++(xoi_attr.count);

    /* If Structured Attribute */
    if (omsyntax == OM_S_OBJECT)
    {
	/* Now load the temporary Class Name structure */
        tclass_index = xoi_cenc.count;

	xoi_cenc.tinfo[tclass_index].omclass = omclass;
	xoi_cenc.tinfo[tclass_index].lineno  = line_no;
	xoi_cenc.tinfo[tclass_index].index   = attr_index;
	xoi_cenc.tinfo[tclass_index].if_attr = OM_TRUE;
	++(xoi_cenc.count);
    }

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi__process_omclass_blocks              	   */
/* Description   : This function is used to read and process the   */
/*                 OM Class Definition Blocks in the Schema file   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_integer xoi__process_omclass_blocks(
    OM_uint *line_no)  /* OUT - Contains erroneous line No.in case of errors */
{
OM_integer  retval;
OM_boolean  eof_flag;
OM_uint	    index, i, j;

    /* If no OM Class Names were encountered in the Attribute Definition */
    /* Block, there should be no OM Class Definition Block 		 */
    if (xoi_cenc.count == 0)
	return(XOI_SUCCESS);

    while(OM_TRUE)
    {
	/* Read the OM Class Definition Block */
	retval = xoi__read_omclass_block(line_no, &eof_flag);

	if (retval != XOI_SUCCESS)
	    return(retval);

	/* If End of File */
	if (eof_flag == OM_TRUE)
	    break;
    }

    /* Now set all the class pointers of xoi_ainfo and xoi_cinfo structures */
    /* to the appropriate Class Name structure.			    	    */
    for (i = 0; i < xoi_cenc.count; ++i)
    {
	for (j = 0; j < xoi_omclass.count; ++j)
	{
	    if (xoi_cenc.tinfo[i].omclass->length == 
		xoi_omclass.class[j].omclass->length)
	    {
		if (memcmp(xoi_cenc.tinfo[i].omclass->elements,
			   xoi_omclass.class[j].omclass->elements, 
			   xoi_omclass.class[j].omclass->length) == 0)
		{
		    index = xoi_cenc.tinfo[i].index;

		    if (xoi_cenc.tinfo[i].if_attr == OM_TRUE)
		    {
			/* This OM Class has been encountered in an Attribute*/
			/* Definition Block.				     */
			xoi_attr.attr[index].class = &(xoi_omclass.class[j]);
		    }
		    else
		    {
			/* This OM Class has been encountered in an OM Class */
			/* Definition Block.				     */
			xoi_cinfo_mem[index].comp_class = 
						   &(xoi_omclass.class[j]);
		    }
		    break;
		}
	    }
	}

	/* If OM Class Name not found */
	if (j == xoi_omclass.count)
	{
	    *line_no = xoi_cenc.tinfo[i].lineno;
	    return(XOI_CLASS_BLOCK_UNDEFINED);
	}
    }

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi__read_omclass_block                    	   */
/* Description   : This function is used to read and process a	   */
/*                 single OM Class Definition Block in Schema file */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_integer xoi__read_omclass_block(
    OM_uint     *line_no, /* OUT - Contains erroneous line No.in case of error*/
    OM_boolean  *eof_flag)/* OUT - Value is OM_TRUE if End of File	      */
{
OM_integer   retval;
OM_boolean   completed;
OM_string   *omclass;
OM_uint	     class_index;
OM_type	     omtype;
char 	     input_line[XOI_MAX_LINE + 1]; 
char	     abbrv[XOI_MAX_ABBRV + 1]; 
char	     full_name[XOI_MAX_NAME + 1];
char         comp_syntax[XOI_MAX_FLD + 1];
char         comp_class[XOI_MAX_FLD + 1];
char	     format_str[XOI_MAX_NAME + 1];

    *eof_flag = OM_FALSE;
    *line_no = xoi_get_next_line(xoi_schema_file, input_line);

    /* If End of file reached */
    if (!*input_line)
    {
	*eof_flag = OM_TRUE;
	return(XOI_SUCCESS);
    }

    completed = OM_FALSE;

    while (completed == OM_FALSE)
    {
	/* Get the OM Class Name */
	*line_no = xoi_get_next_line(xoi_schema_file, input_line);

	/* Convert to XOM Class Identifier*/
	if ((retval = xoi_to_omclass(input_line, &omclass)) != XOI_SUCCESS)
	    break;

	class_index = xoi_omclass.count;
	xoi_omclass.class[class_index].count = 0;
	xoi_omclass.class[class_index].comps= &(xoi_cinfo_mem[xoi_cinfo_index]);
	xoi_omclass.class[class_index].omclass = omclass;

	/* Prepare the format string for sscanf */
	sprintf(format_str, "%%%ds %%%ds %%hu %%%ds %%%ds", 
		         XOI_MAX_ABBRV, XOI_MAX_NAME, XOI_MAX_FLD, XOI_MAX_FLD);
	retval = XOI_SUCCESS;
	while (OM_TRUE)
	{
	    *line_no = xoi_get_next_line(xoi_schema_file, input_line);
	    if (*input_line == XOI_END_BLOCK)
		break;

	    /* Break the line into its fields */
	    sscanf(input_line, format_str, 
		   abbrv, full_name, &omtype, comp_syntax, comp_class);

	    /* Load the read data into the structure */
	    retval = xoi__load_classcomp_det(*line_no, abbrv, full_name, omtype,
					     comp_syntax, comp_class);

	    if (retval != XOI_SUCCESS)
		break;
	}

        if (retval != XOI_SUCCESS)
	    break;
    
	completed = OM_TRUE;
    }

    if (completed == OM_FALSE)
    {
	xoi__free_classcomp(&(xoi_omclass.class[class_index]));
	return(retval);
    }

    ++(xoi_omclass.count);

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi__load_classcomp_det                   	   */
/* Description   : This function validates and loads the Class     */
/*                 Component details into the relevant structures. */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_integer xoi__load_classcomp_det(
    OM_uint  line_no, 	  /* IN - Pointer to the Structure */
    char    *abbrv, 	  /* IN - Abbreviation	           */
    char    *full_name,   /* IN - Full Name		   */	
    OM_type  omtype,      /* IN - OM Type		   */
    char    *comp_syntax, /* IN - OM Syntax		   */	
    char    *comp_class)  /* IN - OM Class Name	 	   */
{
OM_integer   retval;
OM_boolean   completed;
OM_string   *omclass;
OM_syntax    omsyntax;
OM_uint	     class_index, tclass_index;
char	    *p1, *p2;
char	     c[XOI_MAX_ABBRV + 1];

    p1 = p2 = NULL;
    omclass = NULL;
    completed = OM_FALSE;

    while (completed == OM_FALSE)
    {
	/* Convert abbreviation to lower case */
	xoi_strcpy_to_lower(c, abbrv);

	if ((retval = xoi_validate_abbrv(c)) != XOI_SUCCESS)
	   break;

	/* Duplicate the Abbreviation */
	if ((p1 = xoi_strdup(c)) == NULL)
	{
	    retval = XOI_MEMORY_INSUFFICIENT;
	    break;
	}

	/* Duplicate the Full Name */
	if ((p2 = xoi_strdup(full_name)) == NULL)
	{
	    retval = XOI_MEMORY_INSUFFICIENT;
	    break;
	}

	/* Convert to XOM syntax */
	if ((retval = xoi_to_omsyntax(comp_syntax, &omsyntax)) != XOI_SUCCESS)
	    break;

	/* If component is not structured */
	if (omsyntax != OM_S_OBJECT)
	{
	    /* For normal components, the OM Class should be 0 */
	    if (strcmp(comp_class, "0") != 0)
	    {
		retval = XOI_CLASS_FORMAT_ERROR;
		break;
	    }
	}
	else
	{
	    /* Convert to XOM Class Identifier */
	    if ((retval = xoi_to_omclass(comp_class, &omclass)) != XOI_SUCCESS)
		break;
	}

	completed = OM_TRUE;
    }

    if (completed == OM_FALSE)
    {
	if (p1 != NULL)
	    free(p1);
	if (p2 != NULL)
	    free(p2);
	if (omclass != NULL)
	    xoi_free_omstring(omclass);
	return(retval);
    }

    /* Data is error free. Now load the structure members */
    xoi_cinfo_mem[xoi_cinfo_index].abbrv    = p1;
    xoi_cinfo_mem[xoi_cinfo_index].name     = p2;
    xoi_cinfo_mem[xoi_cinfo_index].omtype   = omtype;
    xoi_cinfo_mem[xoi_cinfo_index].omsyntax = omsyntax;

    /* Initialize the Pointer to Class Structure NULL. Will be */
    /* reinitialized later for structured components.	       */
    xoi_cinfo_mem[xoi_cinfo_index].comp_class = NULL;

    class_index = xoi_omclass.count;
    ++(xoi_omclass.class[class_index].count);

    /* If component is structured */
    if (omsyntax == OM_S_OBJECT)
    {
	/* Load the temporary Class Name structure */
        tclass_index = xoi_cenc.count;

	xoi_cenc.tinfo[tclass_index].omclass = omclass;
	xoi_cenc.tinfo[tclass_index].lineno  = line_no;
	xoi_cenc.tinfo[tclass_index].index   = xoi_cinfo_index;
	xoi_cenc.tinfo[tclass_index].if_attr = OM_FALSE;
	++(xoi_cenc.count);
    }

    /* Go to the next available memory block */
    ++xoi_cinfo_index;

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_objclass_ptr			   	   */
/* Description   : This function is used to return the pointer to  */
/*		   the Object Class Structures    		   */
/* Return Value  : Pointer to xoi_object_class or NULL in case of  */
/*		   error			   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
xoi_object_class *xoi_get_objclass_ptr(
    void)
{
    if (xoi_obj.count > 0)
	return(&xoi_obj);

    return(NULL);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_attribute_ptr			   */
/* Description   : This function is used to return the pointer to  */
/*		   the Attribute Structures       		   */
/* Return Value  : Pointer to xoi_attribute or NULL in case of     */
/*		   error					   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
xoi_attribute *xoi_get_attribute_ptr(
    void)
{
    if (xoi_attr.count > 0)
	return(&xoi_attr);

    return(NULL);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_omclass_ptr			   	   */
/* Description   : This function is used to return the pointer to  */
/*		   the OM Class Structures       		   */
/* Return Value  : Pointer to xoi_omclass_struct or NULL in case   */
/*		   of error					   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
xoi_omclass_struct *xoi_get_omclass_ptr(
    void)
{
    if (xoi_omclass.count > 0)
	return(&xoi_omclass);

    return(NULL);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_free_blockinfo			   	   */
/* Description   : This function is used to free the Allocated	   */
/*		   Object Class, Attribute and OM Class Structures */
/* Return Value  : None					   	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void xoi_free_blockinfo(
    void)
{
OM_uint i;

    /* First free the Object Class Structures */
    if (xoi_obj.obj != NULL)
    {
    	for (i = 0; i < xoi_obj.count; ++i)
    	{
	    free(xoi_obj.obj[i].abbrv);
	    free(xoi_obj.obj[i].name);
	    xoi_free_omstring(xoi_obj.obj[i].e_objid);
    	}

    	free(xoi_obj.obj);
	xoi_obj.obj = NULL;
    	xoi_obj.count = 0;
    }

    /* Now free the Attribute Structures */
    if (xoi_attr.attr != NULL)
    {
    	for (i = 0; i < xoi_attr.count; ++i)
    	{
	    free(xoi_attr.attr[i].abbrv);
	    free(xoi_attr.attr[i].name);
	    xoi_free_omstring(xoi_attr.attr[i].e_objid);
    	}

    	free(xoi_attr.attr);
	xoi_attr.attr = NULL;
    	xoi_attr.count = 0;
    }

    /* Now free the OM Class Structures */
    if (xoi_omclass.class != NULL)
    {
	for (i = 0; i < xoi_omclass.count; ++i)
	{
	    xoi__free_classcomp(&(xoi_omclass.class[i]));
	}
	free(xoi_omclass.class);

	xoi_omclass.class = NULL;
	xoi_omclass.count = 0;
    }

    /* Now free the Class Info. Structures */
    if (xoi_cinfo_mem != NULL)
    {
	free(xoi_cinfo_mem);
	xoi_cinfo_mem = NULL;
	xoi_cinfo_index = 0;
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi__free_classcomp			   	   */
/* Description   : This function is used to free the Class   	   */
/*                 structures	   		   	   	   */
/* Return Value  : None					   	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void xoi__free_classcomp(
    xoi_class *class)  /* IN - Pointer to Class structure */
{
OM_uint i;

    xoi_free_omstring(class->omclass);

    for (i = 0; i < class->count; ++i)
    {
	free(class->comps[i].abbrv);
	free(class->comps[i].name);
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi__free_tclass				   */
/* Description   : This function is used to free the temporarily   */
/*                 allocated Class Name Structure		   */
/* Return Value  : None					   	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void xoi__free_tclass(
    void)
{
OM_uint i;

    if (xoi_cenc.tinfo != NULL)
    {
	for (i = 0; i < xoi_cenc.count; ++i)
	{
	    xoi_free_omstring(xoi_cenc.tinfo[i].omclass);
	}

	free(xoi_cenc.tinfo);
	xoi_cenc.tinfo = NULL;
	xoi_cenc.count = 0;
    }
}
/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
