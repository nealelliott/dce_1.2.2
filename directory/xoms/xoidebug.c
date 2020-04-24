/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xoidebug.c,v $
 * Revision 1.1.4.2  1996/03/11  13:25:11  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:24  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:51:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:38  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:22:45  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:44  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:20  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:21:23  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:58:21  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOI - XOM Object Info Module                      */
/* File Name   : xoidebug.c					   */
/* Description : This file contains modules that prints the debug  */
/*		 information					   */
/* Date        : July 1, 1993                                      */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: xoidebug.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:25:11 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>

#include <xom.h>

#include <xoistruct.h>
#include <xoi.h>
#include <xoiinit.h>
#include <xoidebug.h>

/*-- Static Function(s) -------------------------------------------*/
static void xoi_debug_omstring(
    OM_string *omstring); /* IN - The string to be printed */

static void xoi_debug_class(
    xoi_class *class,     /* IN - Pointer to the class structure */
    OM_uint    space);    /* IN - Number of spaces to be printed */


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_debug_string                            	   */
/* Description   : This function is used to print the string 	   */
/*                 details					   */
/* Return Value  : None					   	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void xoi_debug_string(
    char *message) 	/* IN - The string to be printed   */
{
    fprintf(stderr, "%s", message);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_debug_omstring                          	   */
/* Description   : This function is used to print the details of   */
/*                 OM_string				   	   */
/* Return Value  : None					   	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void xoi_debug_omstring(
    OM_string *omstring) 	/* IN - The string to be printed */
{
OM_uint i, len;

    len = (OM_uint) (omstring->length);
    for (i = 0; i < len; ++i)
    {
	fprintf(stderr, "\\x%02x", ((unsigned char *) omstring->elements)[i]);
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_debug_objclass_block                    	   */
/* Description   : This function is used to print the details of   */
/*                 the Object Class Definition Block in the Schema */
/*		   file.			   		   */
/* Return Value  : None					   	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void xoi_debug_objclass_block(
    void)
{
OM_uint 	  i;
xoi_object_class *obj;
char		  c[XOI_MAX_OBJID + 1];

    if ((obj = xoi_get_objclass_ptr()) == NULL)
	return;

    fprintf(stderr, "XOI : Object Class Information :\n");

    for (i = 0; i < obj->count; ++i)
    {
	xoi_ber_to_string(obj->obj[i].e_objid, c);
	fprintf(stderr, "%3d. %-4s %-24s %-12s ", 
			i+1, obj->obj[i].abbrv, obj->obj[i].name, c);

	xoi_debug_omstring(obj->obj[i].e_objid);
	fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_debug_attribute_block			   */
/* Description   : This function is used to print the details of   */
/*                 the Attribute & OM Class Definition blocks in   */
/*		   the Schema file.	   		   	   */
/* Return Value  : None					   	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void xoi_debug_attribute_block(
    void)
{
OM_uint        i;
OM_uint	       space = 5;
xoi_attribute *attr;
char	       c[XOI_MAX_OBJID + 1];

    if ((attr = xoi_get_attribute_ptr()) == NULL)
	return;

    fprintf(stderr,"XOI : Attribute & OM Class Information :\n");

    for (i = 0; i < attr->count ; ++i)
    {
	xoi_ber_to_string(attr->attr[i].e_objid, c);
	fprintf(stderr, "%3d. %-4s %-24s %-12s ", 
			 i+1, attr->attr[i].abbrv, attr->attr[i].name, c);

	xoi_debug_omstring(attr->attr[i].e_objid);
	fprintf(stderr, "%5hu\n", attr->attr[i].omsyntax);

	/* If structured attribute */
	if (attr->attr[i].omsyntax == OM_S_OBJECT)
	{
	    xoi_debug_class(attr->attr[i].class, space);
	    fprintf(stderr, "\n");
	}
    }
    fprintf(stderr, "\n");
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_debug_class			           */
/* Description   : This function is used to print the details of   */
/*                 OM Class Name. 				   */
/* Return Value  : None					   	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static void xoi_debug_class(
    xoi_class *class,  /* IN - Pointer to the class structure */
    OM_uint    space)  /* IN - Number of spaces to be printed */
{
OM_uint i, k;

    for (i = 0; i < space; ++i)
	fprintf(stderr, " ");

    fprintf(stderr, "OM Class Name : ");

    xoi_debug_omstring(class->omclass);

    fprintf(stderr, "\n");
    for (i = 0; i < space; ++i)
	fprintf(stderr, " ");

    fprintf(stderr, "Components :\n");

    for (k = 0; k < class->count; ++k)
    {
	for (i = 0; i < space; ++i)
	    fprintf(stderr, " ");

	fprintf(stderr, "(%2d) %-4s %-24s %4hu %4hu\n",
			k + 1,
			class->comps[k].abbrv,
			class->comps[k].name,
			class->comps[k].omtype,
			class->comps[k].omsyntax);

	/* if Component is structured */
	if (class->comps[k].omsyntax == OM_S_OBJECT)
	{
	    space += 5;
	    xoi_debug_class(class->comps[k].comp_class, space);
	    space -= 5;
	}
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_debug_obj_sortinfo			   */
/* Description   : This function is used to print the details of   */
/*                 the sorted Object Classes		   	   */
/* Return Value  : None					   	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void xoi_debug_obj_sortinfo(
    xoi_obj_abbrv_ord *abbrv_array, /* IN - Pointer to sorted Abbreviations */ 	
    xoi_obj_id_ord    *objid_array) /* IN - Pointer to sorted Object Ids.   */
{
xoi_object_class *obj;
OM_uint 	  i, index;

    if ((obj = xoi_get_objclass_ptr()) == NULL)
	return;

    if (obj->count != 0)
    {
	fprintf(stderr, "XOI : Object Class sorted by Abbreviations :\n");

	for (i = 0; i < obj->count; ++i)
	{
	    index = abbrv_array[i].index;
	    fprintf(stderr, "\t%2d. %s\n", i+1, obj->obj[index].abbrv);
	}

	fprintf(stderr, "\n");
	fprintf(stderr,"XOI : Object Class sorted by Object Identifiers :\n");
	for (i = 0; i < obj->count; ++i)
	{
	    index = objid_array[i].index;
	    fprintf(stderr, "\t%2d. ", i+1);
	    xoi_debug_omstring(obj->obj[index].e_objid);
	    fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_debug_attr_sortinfo			   */
/* Description   : This function is used to print the details of   */
/*                 the sorted Attributes		   	   */
/* Return Value  : None					   	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void xoi_debug_attr_sortinfo(
    xoi_attr_abbrv_ord *abbrv_array, /* IN - Pointer to sorted Abbreviations  */
    xoi_attr_id_ord    *objid_array) /* IN - Pointer to sorted Object Ids.    */
{
OM_uint        i, index;
xoi_attribute *attr;

    if ((attr = xoi_get_attribute_ptr()) == NULL)
	return;

    if (attr->count != 0)
    {
	fprintf(stderr, "XOI : Attributes sorted by Abbreviations :\n");

	for (i = 0; i < attr->count; ++i)
	{
	    index = abbrv_array[i].index;
	    fprintf(stderr, "\t%2d. %s\n", i+1, attr->attr[index].abbrv);
	}

	fprintf(stderr, "\n");
	fprintf(stderr, "XOI : Attributes sorted by Object Identifiers :\n");
	for (i = 0; i < attr->count; ++i)
	{
	    index = objid_array[i].index;
	    fprintf(stderr, "\t%2d. ", i+1);
	    xoi_debug_omstring(attr->attr[index].e_objid);
	    fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
    }
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_debug_omclass_sortinfo			   */
/* Description   : This function is used to print the details of   */
/*                 the sorted OM Classes		   	   */
/* Return Value  : None					   	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void xoi_debug_omclass_sortinfo(
    xoi_omclass_ord  *omclass_array)   /* IN - Pointer to sorted OM Classes */
{
OM_uint	             index, i;
xoi_omclass_struct  *omclass_ptr;

    if ((omclass_ptr = xoi_get_omclass_ptr()) == NULL)
	return;

    if (omclass_ptr->count != 0)
    {
	fprintf(stderr, "XOI : OM Classes sorted by Class Identifiers :\n");
	for (i = 0; i < omclass_ptr->count; ++i)
	{
	    index = omclass_array[i].index;
	    fprintf(stderr, "\t%2d. ", i+1);
	    xoi_debug_omstring(omclass_ptr->class[index].omclass);
	    fprintf(stderr, "\n");
	}
	fprintf(stderr, "\n");
    }
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
