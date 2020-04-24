/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xoiutil.c,v $
 * Revision 1.1.4.2  1996/03/11  13:25:17  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:30  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:51:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:46  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:22:54  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:49  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:25  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:24:54  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:59:04  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOI - XOM Object Info Module                      */
/* File Name   : xoiutil.c					   */
/* Description : This file contains all the utility functions      */
/*		 used by XOI. These include functions for  	   */
/*		 validation checks, conversion functions to	   */
/*		 OM Class strings, OM_syntax, etc.		   */
/* Date        : July 1, 1993                                      */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: xoiutil.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:25:17 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <ctype.h>
#include <string.h>

#include <xom.h>

/* The files below are included to obtain the OM Class Names */
#include <xds.h>
#include <xdsbdcp.h>
#include <xdsgds.h>
#include <xmhp.h>
#include <xdsmdup.h>
#include <xdssap.h>

#include <xoi.h>
#include <xoisort.h>
#include <xoiutil.h>


/*-- Static Variable(s) -------------------------------------------*/
/* Note: omclass_len member is introduced because sizeof(omclass_str) */
/*       will give the size of "char *" and not the array size.	      */
static struct { char      *str;
	 	char      *omclass_str;
		OM_uint    omclass_len;
       	      } xoi_omclass_table[]  =  
{
    { "DS_C_FACSIMILE_PHONE_NBR",		OMP_O_DS_C_FACSIMILE_PHONE_NBR,
	sizeof(OMP_O_DS_C_FACSIMILE_PHONE_NBR) },
    { "DS_C_POSTAL_ADDRESS",		OMP_O_DS_C_POSTAL_ADDRESS,
	sizeof(OMP_O_DS_C_POSTAL_ADDRESS) },
    { "DS_C_PRESENTATION_ADDRESS",	OMP_O_DS_C_PRESENTATION_ADDRESS,
	sizeof(OMP_O_DS_C_PRESENTATION_ADDRESS) },
    { "DS_C_TELETEX_TERM_IDENT",		OMP_O_DS_C_TELETEX_TERM_IDENT,
	sizeof(OMP_O_DS_C_TELETEX_TERM_IDENT) },
    { "DS_C_TELEX_NBR",			OMP_O_DS_C_TELEX_NBR,
	sizeof(OMP_O_DS_C_TELEX_NBR) },
    { "DSX_C_GDS_ACL",			OMP_O_DSX_C_GDS_ACL,
	sizeof(OMP_O_DSX_C_GDS_ACL) },
    { "DSX_C_GDS_ACL_ITEM",		OMP_O_DSX_C_GDS_ACL_ITEM,
 	sizeof(OMP_O_DSX_C_GDS_ACL_ITEM) },
    { "DS_C_DL_SUBMIT_PERMS",		OMP_O_DS_C_DL_SUBMIT_PERMS,
	sizeof(OMP_O_DS_C_DL_SUBMIT_PERMS) },
    { "MH_C_OR_ADDRESS",	        	OMP_O_MH_C_OR_ADDRESS,
	sizeof(OMP_O_MH_C_OR_ADDRESS) },
    { "MH_C_OR_NAME",	        	OMP_O_MH_C_OR_NAME,
	sizeof(OMP_O_MH_C_OR_NAME) },
    { "MH_C_TELETEX_NBPS",	       	OMP_O_MH_C_TELETEX_NBPS,
	sizeof(OMP_O_MH_C_TELETEX_NBPS) },
    { "MH_C_G3_FAX_NBPS",	        	OMP_O_MH_C_G3_FAX_NBPS,
	sizeof(OMP_O_MH_C_G3_FAX_NBPS) },
    { "DS_C_SEARCH_CRITERION",		OMP_O_DS_C_SEARCH_CRITERION,
	sizeof(OMP_O_DS_C_SEARCH_CRITERION) },
    { "DS_C_SEARCH_GUIDE",		OMP_O_DS_C_SEARCH_GUIDE,
	sizeof(OMP_O_DS_C_SEARCH_GUIDE) },
    { "DS_C_ALGORITHM_IDENT",	        OMP_O_DS_C_ALGORITHM_IDENT,
	sizeof(OMP_O_DS_C_ALGORITHM_IDENT) },
    { "DS_C_CERT",	        	OMP_O_DS_C_CERT,
	sizeof(OMP_O_DS_C_CERT) },
    { "DS_C_CERT_LIST",	        	OMP_O_DS_C_CERT_LIST,
	sizeof(OMP_O_DS_C_CERT_LIST) },
    { "DS_C_CERT_PAIR",	        	OMP_O_DS_C_CERT_PAIR,
	sizeof(OMP_O_DS_C_CERT_PAIR) },
    { "DS_C_CERT_SUBLIST",	        OMP_O_DS_C_CERT_SUBLIST,
	sizeof(OMP_O_DS_C_CERT_SUBLIST) },
    { "DS_C_SIGNATURE",	        	OMP_O_DS_C_SIGNATURE,
	sizeof(OMP_O_DS_C_SIGNATURE) },
    { "DSX_C_GDS_CONTEXT",	        OMP_O_DSX_C_GDS_CONTEXT,
	sizeof(OMP_O_DSX_C_GDS_CONTEXT) },
    { "DSX_C_GDS_SESSION",	        OMP_O_DSX_C_GDS_SESSION,
	sizeof(OMP_O_DSX_C_GDS_SESSION) },
    { "DS_C_DS_DN",			OMP_O_DS_C_DS_DN,
	sizeof(OMP_O_DS_C_DS_DN) },
    { "DS_C_DS_RDN",			OMP_O_DS_C_DS_RDN,
	sizeof(OMP_O_DS_C_DS_RDN) },
    { "DS_C_AVA",				OMP_O_DS_C_AVA,
	sizeof(OMP_O_DS_C_AVA) },
    { "DS_C_ATTRIBUTE",	        	OMP_O_DS_C_ATTRIBUTE,
	sizeof(OMP_O_DS_C_ATTRIBUTE) },
    { "DS_C_ATTRIBUTE_LIST",	        OMP_O_DS_C_ATTRIBUTE_LIST,
	sizeof(OMP_O_DS_C_ATTRIBUTE_LIST) },
    { "DS_C_CONTEXT",	        	OMP_O_DS_C_CONTEXT,
	sizeof(OMP_O_DS_C_CONTEXT) },
    { "DS_C_ENTRY_INFO",	        	OMP_O_DS_C_ENTRY_INFO,
	sizeof(OMP_O_DS_C_ENTRY_INFO) },
    { "DS_C_ENTRY_INFO_SELECTION",	OMP_O_DS_C_ENTRY_INFO_SELECTION,
	sizeof(OMP_O_DS_C_ENTRY_INFO_SELECTION) },
    { "DS_C_ENTRY_MOD",		        OMP_O_DS_C_ENTRY_MOD,
	sizeof(OMP_O_DS_C_ENTRY_MOD) },
    { "DS_C_ENTRY_MOD_LIST",		OMP_O_DS_C_ENTRY_MOD_LIST,
	sizeof(OMP_O_DS_C_ENTRY_MOD_LIST) },
    { "DS_C_EXT",				OMP_O_DS_C_EXT,
	sizeof(OMP_O_DS_C_EXT) },
    { "DS_C_FILTER",			OMP_O_DS_C_FILTER,
	sizeof(OMP_O_DS_C_FILTER) },
    { "DS_C_FILTER_ITEM",			OMP_O_DS_C_FILTER_ITEM,
	sizeof(OMP_O_DS_C_FILTER_ITEM) },
    { "DS_C_OPERATION_PROGRESS",		OMP_O_DS_C_OPERATION_PROGRESS,
	sizeof(OMP_O_DS_C_OPERATION_PROGRESS) },
    { "DS_C_SESSION",			OMP_O_DS_C_SESSION,
	sizeof(OMP_O_DS_C_SESSION) }
};

static struct { char	  *str;
		OM_syntax  omsyntax;
	      } xoi_omsyntax_table[] = 
{
    { "OM_S_OBJECT_IDENTIFIER_STRING",  OM_S_OBJECT_IDENTIFIER_STRING },
    { "OM_S_PRINTABLE_STRING",		OM_S_PRINTABLE_STRING },
    { "OM_S_IA5_STRING",		OM_S_IA5_STRING },
    { "OM_S_OBJECT",			OM_S_OBJECT },
    { "OM_S_NUMERIC_STRING",		OM_S_NUMERIC_STRING },
    { "OM_S_TELETEX_STRING",		OM_S_TELETEX_STRING },
    { "OM_S_INTEGER",			OM_S_INTEGER },
    { "OM_S_OCTET_STRING",		OM_S_OCTET_STRING },
    { "OM_S_OBJECT_DESCRIPTOR_STRING",	OM_S_OBJECT_DESCRIPTOR_STRING },
    { "OM_S_VISIBLE_STRING",		OM_S_VISIBLE_STRING },
    { "OM_S_BIT_STRING", 		OM_S_BIT_STRING },
    { "OM_S_BOOLEAN",			OM_S_BOOLEAN },
    { "OM_S_ENCODING_STRING",		OM_S_ENCODING_STRING },
    { "OM_S_ENUMERATION",		OM_S_ENUMERATION },
    { "OM_S_GENERAL_STRING",		OM_S_GENERAL_STRING },
    { "OM_S_GENERALISED_TIME_STRING",	OM_S_GENERALISED_TIME_STRING },
    { "OM_S_GRAPHIC_STRING",		OM_S_GRAPHIC_STRING },
    { "OM_S_UTC_TIME_STRING",		OM_S_UTC_TIME_STRING },
    { "OM_S_VIDEOTEX_STRING",		OM_S_VIDEOTEX_STRING }
};


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_next_line				   */
/* Description   : This function is used to read a line from the   */
/*                 Schema file. The leading and trailing blanks in */
/*		   the line are removed. The newline character is  */
/*		   also removed.		  		   */
/* Return Value  : An integer value indicating the line number     */
/*		   read from the file		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_uint xoi_get_next_line(
    FILE *fileptr,     /* IN  - File Pointer		 */
    char *data_buffer) /* OUT - Line read from the file  */
{
static OM_uint  line_no = 0;
OM_uint	        len, ret_line_no;
char	       *p;
char	       *str = data_buffer;


    while ((p = fgets(str, XOI_MAX_LINE, fileptr)) != NULL)
    {
	++line_no;

	/* Skip over white spaces */
	while (*str && *str != '\n' && isspace(*str))
	    ++str;

	/* Ignore Blank lines and comment lines */
	if ((!*str) || (*str == '\n') || (*str == XOI_COMMENT_CHAR))
	    continue;
	else
	    break;
    }

    if (p != NULL)
    {
	/* Remove the trailing spaces and newline */
	len = strlen(str) - 1;
	if (str[len] == '\n')
	    --len;
	while (isspace(str[len]))
	    --len;
	++len;
	str[len] = 0;

	/* Copy to data_buffer.  "str" points to the first  */
	/* non space character.				    */
	strcpy(data_buffer, str);
    }
    else
    {
	*data_buffer = 0;
    }

    ret_line_no = line_no;
   
    /* If end of file has been reached reset line_no */
    if (p == NULL)
	line_no = 0;

    return(ret_line_no);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_validate_abbrv			   	   */
/* Description   : This function is used to validate the 	   */
/*		   Abbreviations used for Object Class and 	   */
/* 		   Attributes.				   	   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_validate_abbrv(
    char *abbrv)  /* IN - Abbreviation */
{
OM_integer  retval;

    retval = XOI_SUCCESS;

    if (!abbrv)
    {
	retval = XOI_INVALID_ABBRV;
    }
    else
    {
	/* First character of abbreviation should be an alphabet */
	if (!isalpha(*abbrv))
	{
	    retval = XOI_INVALID_ABBRV;
	}
	else
	{
	    for (++abbrv; *abbrv; ++abbrv)
	    {
		if (!isalnum(*abbrv))
		{
		    retval = XOI_INVALID_ABBRV;
		    break;
		}
	    }
	}
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_to_omclass			   	   */
/* Description   : This function converts the Class Name in the    */
/*		   string format to OM_string form. The memory 	   */
/* 		   space for the OM_string is provided by this     */
/*		   function.	 	   		           */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_to_omclass(
    char       *str,      /* IN  - String to be converted */
    OM_string **omclass)  /* OUT - Converted OM_string    */
{
OM_integer   retval;
OM_string   *p;
OM_uint32    len;
OM_uint      n, i;

    if (omclass == NULL)
	return(XOI_INVALID_PARAMETER);

    retval = XOI_SUCCESS;
    n = sizeof(xoi_omclass_table)/sizeof(xoi_omclass_table[0]);

    for (i = 0; i < n; ++i)
    {
	/* if Class Name found */
	if (strcmp(xoi_omclass_table[i].str, str) == 0)
	{
	    len = (OM_uint32) (xoi_omclass_table[i].omclass_len - 1);

	    p = xoi_create_omstring(xoi_omclass_table[i].omclass_str, len);
	    if (p == NULL)
	    {
		retval = XOI_MEMORY_INSUFFICIENT;
		break;
	    }

	    *omclass = p;
	    break;
	}
    }

    if (i == n)
	retval = XOI_INVALID_CLASS_NAME;

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_omclass_define		   	   */
/* Description   : This function returns the name of the OM Class  */
/*		   in the form of a string. Eg: DS_C_POSTAL_ADDRESS*/
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_get_omclass_define(
    OM_string  *omclass,  /* IN  - OM Class string      */
    char       *str)      /* OUT - OM Class Name string */
{
OM_integer  retval;
OM_uint     n, i;

    if (omclass == NULL || str  == NULL)
	return(XOI_INVALID_PARAMETER);

    retval = XOI_SUCCESS;

    n = sizeof(xoi_omclass_table)/sizeof(xoi_omclass_table[0]);

    for (i = 0; i < n; ++i)
    {
	/* if OM Class found */
	if (xoi_omclass_table[i].omclass_len - 1 == omclass->length &&
	    memcmp(xoi_omclass_table[i].omclass_str, omclass->elements,
		   omclass->length) == 0)
	{
	    strcpy(str, xoi_omclass_table[i].str);
	    break;
	}
    }

    if (i == n)
	retval = XOI_INVALID_CLASS_NAME;

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_to_omsyntax			   	   */
/* Description   : This function is used to convert the Syntax in  */
/*		   the string format to	OM_syntax form.		   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_to_omsyntax(
    char      *str,       /* IN  - String to be converted */
    OM_syntax *omsyntax)  /* OUT - Converted OM_syntax    */
{
OM_integer  retval;
OM_uint     i, n;

    if (omsyntax == NULL)
	return(XOI_INVALID_PARAMETER);

    retval = XOI_SUCCESS;

    n = sizeof(xoi_omsyntax_table)/sizeof(xoi_omsyntax_table[0]);

    for (i = 0; i < n; ++i)
    {
	if (strcmp(xoi_omsyntax_table[i].str, str) == 0)
	{
	    *omsyntax = xoi_omsyntax_table[i].omsyntax;
	    break;
	}
    }

    if (i == n)
    {
	retval = XOI_INVALID_SYNTAX;
    }

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_strcpy_to_lower			   	   */
/* Description   : This function is used to convert and copy a     */
/*		   string to lowercase.		   		   */
/* Return Value  : None					   	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void xoi_strcpy_to_lower(
    char *dst,    /* OUT - Converted String */
    char *src)    /* IN  - Input String     */
{
    for (; *src; ++src, ++dst)
	*dst = tolower(*src);
    *dst = 0;
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_strcpy_to_upper			   	   */
/* Description   : This function is used to convert and copy a     */
/*		   string to uppercase.		   		   */
/* Return Value  : None					   	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void xoi_strcpy_to_upper(
    char *dst,    /* OUT - Converted String */
    char *src)    /* IN  - Input String     */
{
    for (; *src; ++src, ++dst)
	*dst = toupper(*src);
    *dst = 0;
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_strdup				   	   */
/* Description   : This function is used to duplicate a string. The*/
/*		   memory space for the duplicated string is 	   */
/*		   provided by this function. Note that same       */
/*		   functionality is provided by the "strdup" 	   */
/*		   function, but is not used since it is not POSIX */
/*		   conformant 					   */
/* Return Value  : A pointer to the duplicated string or a NULL    */
/*		   pointer in the case of error.         	   */	
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
char *xoi_strdup(
    char *str)   /* IN - Input String to be duplicated */
{
char *s;

    if (str == NULL)
	return(NULL);

    s = (char *) malloc(sizeof(char) * (strlen(str) + 1));

    if (s != NULL)
    {
        strcpy(s, str);
    }

    return(s);
}	


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_create_omstring			   	   */
/* Description   : This function is used to convert a normal 	   */
/*		   character string to OM_string. The memory space */
/* 		   for the OM_string is provided by this funciton  */
/* Return Value  : A pointer to the converted OM_string or a NULL  */
/*		   pointer in the case of error.	           */	
/* Author        : B T Naik, SNI BU BA KP13, SISL India.  	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_string *xoi_create_omstring(
    char       *str,   /* IN - Input String		  */
    OM_uint32   len)   /* IN - Length of the input string */
{
OM_string *p;

    if (str == NULL || len <= 0)
	return(NULL);

    if ((p = (OM_string *) malloc(sizeof(OM_string))) != NULL)
    {
	p->elements = (char *) malloc(sizeof(char) * len);

	if (p->elements == NULL)
	{
	    free(p);
	    p = NULL;
	}
	else
	{
	    memcpy(p->elements, str, len);
	    p->length = len;
	}
    }

    return(p);
}
    

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_free_omstring			   	   */
/* Description   : This function is used to free the OM_string	   */
/*		   structure.			   	           */
/* Return Value  : None					   	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void xoi_free_omstring(
    OM_string *omstring) /* IN - String to be freed  */
{

    if (omstring == NULL)
	return;

    free(omstring->elements);
    free(omstring);
}

/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
