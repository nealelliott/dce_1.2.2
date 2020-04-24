/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xoisort.c,v $
 * Revision 1.1.4.2  1996/03/11  13:25:15  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:28  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:51:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:44  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:22:52  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:48  devsrc]
 * 
 * Revision 1.1.2.2  1994/03/23  15:50:33  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:39:12  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:24  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:23:47  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:58:52  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOI - XOM Object Info Module                      */
/* File Name   : xoisort.c					   */
/* Description : This file contains all the functions to sort      */
/*		 and perform binary search on the Object Class     */
/*		 and Attribute arrays.				   */
/* Date        : July 1, 1993                                      */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: xoisort.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:25:15 $";
#endif

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <xom.h>

#include <xoistruct.h>
#include <xoi.h>
#include <xoiinit.h>
#include <xoiutil.h>
#include <xoidebug.h>
#include <xoisort.h>

/*-- Static Variable(s) -------------------------------------------*/

OM_boolean	xoi_dup_abbrv;       /* Boolean tag to indicate duplicate */
				     /* Abbreviation	   		  */
OM_boolean	xoi_dup_objid;       /* Boolean tag to indicate duplicate */
				     /* Object Identifiers  		  */
OM_boolean	xoi_dup_omclass;     /* Boolean tag to indicate duplicate */
				     /* OM Class Identifiers  		  */

/* Array to store Object Class information sorted on abbreviation  */
static	xoi_obj_abbrv_ord 	*xoi_obj_abbrv_array;

/* Array to store Object Class information sorted on Obj.Identifier*/
static  xoi_obj_id_ord		*xoi_obj_id_array;

/* Array to store Attribute Information sorted on abbreviation     */
static  xoi_attr_abbrv_ord	*xoi_attr_abbrv_array;

/* Array to store Attribute Information sorted on Obj.Identifier   */
static  xoi_attr_id_ord		*xoi_attr_id_array;

/* Array to store OM Class Information sorted on OM Class identifier    */
static  xoi_omclass_ord		*xoi_omclass_array;

/*-- Static Function(s) -------------------------------------------*/
static int xoi__compare_obj_abbrv(
    const void *abbrv1,   /* IN - The first xoi_obj_abbrv_ord element  */
    const void *abbrv2);  /* IN - The second xoi_obj_abbrv_ord element */

static int xoi__compare_obj_id(
    const void *id1,      /* IN - The first xoi_obj_id_ord element  */
    const void *id2);     /* IN - The second xoi_obj_id_ord element */

static int xoi__compare_attr_abbrv(
    const void *abbrv1,   /* IN - The first xoi_attr_abbrv_ord element  */
    const void *abbrv2);  /* IN - The second xoi_attr_abbrv_ord element */

static int xoi__compare_attr_id(
    const void *id1,      /* IN - The first xoi_attr_id_ord element  */
    const void *id2);     /* IN - The second xoi_attr_id_ord element */

static int xoi__compare_omclass(
    const void *class1,   /* IN - The first xoi_omclass_ord element  */
    const void *class2);  /* IN - The second xoi_omclass_ord element */

static int xoi__compare_om_strings(
    OM_string *str1,      /* IN - The first OM_string to be compared  */
    OM_string *str2);     /* IN - The second OM_string to be compared */

static OM_integer xoi__get_class_ptr(
    OM_string  *omclass,  /* IN  - OM Class Identifier		      */
    xoi_class **class);   /* OUT - Pointer to the xoi_class structure */

static OM_integer xoi__sort_objclass(
    void);

static OM_integer xoi__sort_attribute(
    void);

static OM_integer xoi__sort_omclass(
    void);


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_sort_info				   */
/* Description   : This function is used to sort the Object Class, */
/*		   Attribute and OM Class structures. 	   	   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_sort_info(
    void)
{
OM_integer retval;

    /* Initialize all variables */
    xoi_obj_abbrv_array    = NULL;
    xoi_obj_id_array       = NULL;
    xoi_attr_abbrv_array   = NULL;
    xoi_attr_id_array      = NULL;
    xoi_omclass_array      = NULL;
    xoi_dup_abbrv          = OM_FALSE;
    xoi_dup_objid          = OM_FALSE;
    xoi_dup_omclass	   = OM_FALSE;

    /* Sort the Object Classes */
    if ((retval = xoi__sort_objclass()) != XOI_SUCCESS)
	return(retval);

    /* Sort the Attributes */
    if ((retval = xoi__sort_attribute()) != XOI_SUCCESS)
	return(retval);

    /* Sort the OM Classes */
    if ((retval = xoi__sort_omclass()) != XOI_SUCCESS)
	return(retval);

#ifdef XOI_DEBUG
    xoi_debug_obj_sortinfo(xoi_obj_abbrv_array, xoi_obj_id_array);
    xoi_debug_attr_sortinfo(xoi_attr_abbrv_array, xoi_attr_id_array); 
    xoi_debug_omclass_sortinfo(xoi_omclass_array);
#endif

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi__sort_objclass			   	   */
/* Description   : This function is used to sort the Object Class  */
/*		   structures.  The sorting is done Abbreviation   */
/*		   wise and also Object Identifier wise		   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_integer xoi__sort_objclass(
    void)
{
OM_integer	  retval;
OM_boolean	  completed;
OM_uint		  i;
xoi_object_class *objclass_ptr;

    completed = OM_FALSE;

    while (completed == OM_FALSE)
    {
	objclass_ptr = xoi_get_objclass_ptr();

	if (objclass_ptr == NULL || objclass_ptr->count == 0)
	{
	    retval = XOI_SUCCESS;
	    completed = OM_TRUE;
	    break;
	}

	xoi_obj_abbrv_array = (xoi_obj_abbrv_ord *) 
			malloc(sizeof(xoi_obj_abbrv_ord) * objclass_ptr->count);

	if (xoi_obj_abbrv_array == NULL)
	{
	    retval = XOI_MEMORY_INSUFFICIENT;
	    break;
	}

	xoi_obj_id_array = (xoi_obj_id_ord *) 
			   malloc(sizeof(xoi_obj_id_ord) * objclass_ptr->count);

	if (xoi_obj_id_array == NULL)
	{
	    retval = XOI_MEMORY_INSUFFICIENT;
	    break;
	}

	/* Load the Object Class Array */
	for (i = 0; i < objclass_ptr->count; ++i)
	{
	    xoi_obj_abbrv_array[i].abbrv = objclass_ptr->obj[i].abbrv;
	    xoi_obj_abbrv_array[i].index = i;
	    xoi_obj_id_array[i].e_objid  = objclass_ptr->obj[i].e_objid;
	    xoi_obj_id_array[i].index    = i;
	}
    
	/* Sort the Object Class Array by Abbreviation */
	qsort((void *) xoi_obj_abbrv_array, (size_t) objclass_ptr->count,
	      (size_t) sizeof(xoi_obj_abbrv_ord), xoi__compare_obj_abbrv);

	/* If no duplicate Abbreviations */
	if (xoi_dup_abbrv == OM_TRUE)
	{
	    retval = XOI_DUPLICATE_OBJ_ABBRV;
	    break;
	}

	/* Sort the Object Class Array by Object Identifier */
	qsort((void *) xoi_obj_id_array, (size_t) objclass_ptr->count,
	      (size_t) sizeof(xoi_obj_id_ord), xoi__compare_obj_id);

	/* If duplicate Object Identifiers */
	if (xoi_dup_objid == OM_TRUE)
	{
	    retval = XOI_DUPLICATE_OBJ_OBJ_ID;
	    break;
	}

        completed = OM_TRUE;
    }

    if (completed == OM_FALSE)
    {
	xoi_free_sort_structures();
	return(retval);
    }

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi__sort_attribute			   	   */
/* Description   : This function is used to sort the Attribute     */
/*		   structures. The sorting is done Abbreviation    */
/*		   wise and Object Identifier wise		   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_integer xoi__sort_attribute(
    void)
{
OM_integer     retval;
OM_boolean     completed;
OM_uint        i;
xoi_attribute *attr_ptr;
    
    completed = OM_FALSE;

    while (completed == OM_FALSE)
    {
	attr_ptr = xoi_get_attribute_ptr();

	if (attr_ptr == NULL || attr_ptr->count == 0)
	{
	    retval = XOI_SUCCESS;
	    completed = OM_TRUE;
	    break;
	}

	xoi_attr_abbrv_array = (xoi_attr_abbrv_ord *) 
			   malloc(sizeof(xoi_attr_abbrv_ord) * attr_ptr->count);

	if (xoi_attr_abbrv_array == NULL)
	{
	    retval = XOI_MEMORY_INSUFFICIENT;
	    break;
	}

	xoi_attr_id_array = (xoi_attr_id_ord *) 
			     malloc(sizeof(xoi_attr_id_ord) * attr_ptr->count);

	if (xoi_attr_id_array == NULL)
	{
	    retval = XOI_MEMORY_INSUFFICIENT;
	    break;
	}

	/* Load the Attribute Arrays */
	for (i = 0; i < attr_ptr->count; ++i)
	{
	    xoi_attr_abbrv_array[i].abbrv = attr_ptr->attr[i].abbrv;
	    xoi_attr_abbrv_array[i].index = i;
	    xoi_attr_id_array[i].e_objid  = attr_ptr->attr[i].e_objid;
	    xoi_attr_id_array[i].index    = i;
	}

	/* Sort the Attribute Array by Abbreviation */
	qsort((void *) xoi_attr_abbrv_array, (size_t) attr_ptr->count,
	      (size_t) sizeof(xoi_attr_abbrv_ord), xoi__compare_attr_abbrv);

	if (xoi_dup_abbrv == OM_TRUE)
	{
	    retval = XOI_DUPLICATE_ATTR_ABBRV;
	    break;
	}

	/* Sort the Attribute Array by Object Identifier */
	qsort((void *) xoi_attr_id_array, (size_t) attr_ptr->count,
	      (size_t) sizeof(xoi_attr_id_ord), xoi__compare_attr_id);

	if (xoi_dup_objid == OM_TRUE)
	{
	    retval = XOI_DUPLICATE_ATTR_OBJ_ID;
	    break;
	}

	completed = OM_TRUE;
    }

    if (completed == OM_FALSE)
    {
	xoi_free_sort_structures();
	return(retval);
    }

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi__sort_omclass			   	   */
/* Description   : This function is used to sort the OM Class      */
/*		   structures. The sorting is done OM Class        */
/*		   Identifier wise		   		   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_integer xoi__sort_omclass(
    void)
{
OM_integer	     retval;
OM_boolean 	     completed;
OM_uint		     i;
xoi_omclass_struct  *omclass_ptr;

    completed = OM_FALSE;

    while (completed == OM_FALSE)
    {
	omclass_ptr = xoi_get_omclass_ptr();

	if (omclass_ptr == NULL || omclass_ptr->count == 0)
	{
	    retval = XOI_SUCCESS;
	    completed = OM_TRUE;
	    break;
	}

	xoi_omclass_array = (xoi_omclass_ord *)
			   malloc(sizeof(xoi_omclass_ord) * omclass_ptr->count);

	if (xoi_omclass_array == NULL)
	{
	    retval = XOI_MEMORY_INSUFFICIENT;
	    break;
	}

	/* Load the OM Class Array */
	for (i = 0; i < omclass_ptr->count; ++i)
	{
	    xoi_omclass_array[i].omclass = omclass_ptr->class[i].omclass;
	    xoi_omclass_array[i].index   = i;
	}

	/* Sort the OM Class Array by Class Identifiers */
	qsort((void *) xoi_omclass_array,(size_t) omclass_ptr->count,
	      (size_t) sizeof(xoi_omclass_ord), xoi__compare_omclass);

	if (xoi_dup_omclass == OM_TRUE)
	{
	    retval = XOI_DUPLICATE_CLASS_BLOCK;
	    break;
	}

	completed = OM_TRUE;
    }

    if (completed == OM_FALSE)
    {
	xoi_free_sort_structures();
	return(retval);
    }

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_oinfo_from_abbrv		   	   */
/* Description   : This function is used to get a pointer to the   */
/*		   relevant xoi_oinfo structure.	   	   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_get_oinfo_from_abbrv(
    char       *abbrv,   /* IN  - Object Class Abbreviation	     */
    xoi_oinfo **obj)     /* OUT - Pointer to the xoi_oinfo structure */
{
xoi_object_class  *objclass_ptr;
xoi_obj_abbrv_ord  srch_obj, *p;
char		   c[XOI_MAX_ABBRV + 1];
OM_uint	           index;

    /* Get Object Class information pointer */
    objclass_ptr = xoi_get_objclass_ptr();

    /* Convert string to lowercase */
    xoi_strcpy_to_lower(c, abbrv);

    srch_obj.abbrv = c;

    /* Search the Abbreviation */
    p = (xoi_obj_abbrv_ord *) bsearch((void *) &srch_obj, 
				      (void *) xoi_obj_abbrv_array, 
				      (size_t) objclass_ptr->count, 
				      (size_t) sizeof(xoi_obj_abbrv_ord), 
				      xoi__compare_obj_abbrv);

    /* Abbreviation not found */
    if (p == NULL)
	return(XOI_UNKNOWN_ABBRV);

    index = p->index;

    *obj = &(objclass_ptr->obj[index]);

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_oinfo_from_objid		   	   */
/* Description   : This function is used to get a pointer to the   */
/*		   relevant xoi_oinfo structure.	   	   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_get_oinfo_from_objid(
    OM_string   *e_objid, /* IN  - Encoded Object Class Identifier    */
    xoi_oinfo  **obj)	  /* OUT - Pointer to the xoi_oinfo structure */
{
xoi_object_class *objclass_ptr;
xoi_obj_id_ord	  srch_obj, *p;
OM_uint	          index;

    /* Get Object Class information pointer */
    objclass_ptr = xoi_get_objclass_ptr();

    srch_obj.e_objid = e_objid;

    /* Search the Object Identifier */
    p = (xoi_obj_id_ord *) bsearch((void *) &srch_obj, 
			           (void *) xoi_obj_id_array, 
			           (size_t) objclass_ptr->count, 
			           (size_t) sizeof(xoi_obj_id_ord), 
			           xoi__compare_obj_id);

    /* Object Identifier not found */
    if (p == NULL)
	return(XOI_UNKNOWN_OBJ_ID);

    index = p->index;

    *obj = &(objclass_ptr->obj[index]);

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_oinfo_index		   	   	   */
/* Description   : This function is used to return the original    */
/*		   index into the object class structures.	   */
/* Return Value  : The original index				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_uint xoi_get_oinfo_system_index(
    OM_uint index)   /* IN - Index to the sorted array */
{
    return(xoi_obj_abbrv_array[index].index);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_ainfo_from_abbrv	   	   	   */
/* Description   : This function is used to get a pointer to the   */
/*		   relevant xoi_ainfo structure.	   	   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_get_ainfo_from_abbrv(
    char       *abbrv,   /* IN  - Attribute Abbreviation	     */
    xoi_ainfo **attr)    /* OUT - Pointer to the xoi_ainfo structure */
{
xoi_attr_abbrv_ord  srch_attr, *p;
xoi_attribute	   *attr_ptr;
char		    c[XOI_MAX_ABBRV + 1];
OM_uint	            index;

    /* Get Attribute Information pointer */
    attr_ptr = xoi_get_attribute_ptr();

    /* Convert string to lowercase */
    xoi_strcpy_to_lower(c, abbrv);

    srch_attr.abbrv = c;

    /* Search the Abbreviation */
    p = (xoi_attr_abbrv_ord *) bsearch((void *) &srch_attr, 
				       (void *) xoi_attr_abbrv_array, 
				       (size_t) attr_ptr->count, 
				       (size_t) sizeof(xoi_attr_abbrv_ord), 
				       xoi__compare_attr_abbrv);

    /* Abbreviation not found */
    if (p == NULL)
	return(XOI_UNKNOWN_ABBRV);

    index = p->index;

    *attr = &(attr_ptr->attr[index]);

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_ainfo_from_objid	   	   	   */
/* Description   : This function is used to get a pointer to the   */
/*		   relevant xoi_ainfo structure.	   	   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_get_ainfo_from_objid(
    OM_string   *e_objid, /* IN  - Encoded Attribute Object Identifier  */
    xoi_ainfo  **attr)    /* OUT - Pointer to the xoi_ainfo structure   */
{
xoi_attr_id_ord  srch_attr, *p;
xoi_attribute   *attr_ptr;
OM_uint	         index;

    /* Get Attribute Information pointer */
    attr_ptr = xoi_get_attribute_ptr();

    srch_attr.e_objid = e_objid;

    /* Search the Abbreviation */
    p = (xoi_attr_id_ord *) bsearch((void *) &srch_attr, 
			            (void *) xoi_attr_id_array, 
			            (size_t) attr_ptr->count, 
			            (size_t) sizeof(xoi_attr_id_ord), 
			            xoi__compare_attr_id);

    /* Abbreviation not found */
    if (p == NULL)
	return(XOI_UNKNOWN_OBJ_ID);

    index = p->index;

    *attr = &(attr_ptr->attr[index]);

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_ainfo_index		   	   	   */
/* Description   : This function is used to return the original    */
/*		   index into the attribute structures.	   	   */
/* Return Value  : The original index				   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_uint xoi_get_ainfo_system_index(
    OM_uint index)   /* IN - Index to the sorted array */
{
    return(xoi_attr_abbrv_array[index].index);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_cinfo_from_abbrv	   	   	   */
/* Description   : This function is used to get a pointer to the   */
/*		   relevant xoi_cinfo structure.	   	   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_get_cinfo_from_abbrv(
    OM_string  *omclass,   /* IN  - OM Class Identifier		       */
    char       *abbrv,     /* IN  - Abbreviation of the component      */
    xoi_cinfo **comp)	   /* OUT - Pointer to the xoi_cinfo structure */
{
OM_integer  retval;
OM_uint	    i;
xoi_class  *class;
char	    c[XOI_MAX_ABBRV + 1];

    if ((retval = xoi__get_class_ptr(omclass, &class)) != XOI_SUCCESS)
	return(retval);

    xoi_strcpy_to_lower(c, abbrv);

    for (i = 0; i < class->count; ++i)
    {
	if (strcmp(class->comps[i].abbrv, c) == 0)
	{
	    *comp = &(class->comps[i]);
	     return(XOI_SUCCESS);
	}
    }
   
    return(XOI_UNKNOWN_ABBRV);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_get_cinfo_from_omtype	   	   	   */
/* Description   : This function is used to get a pointer to the   */
/*		   relevant xoi_cinfo structure.	   	   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
OM_integer xoi_get_cinfo_from_omtype(
    OM_string  *omclass,   /* IN  - OM Class Identifier		       */
    OM_type     omtype,    /* IN  - OM_type of the component           */
    xoi_cinfo **comp)      /* OUT - Pointer to the xoi_cinfo structure */
{
OM_integer  retval;
OM_uint     i;
xoi_class  *class;

    if ((retval = xoi__get_class_ptr(omclass, &class)) != XOI_SUCCESS)
	return(retval);

    for (i = 0; i < class->count; ++i)
    {
	if (class->comps[i].omtype == omtype)
	{
	    *comp = &(class->comps[i]);
	     return(XOI_SUCCESS);
	}
    }
   
    return(XOI_UNKNOWN_OMTYPE);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi__get_class_ptr				   */
/* Description   : This function is used to get a pointer to the   */
/*		   relevant xoi_class structure.	   	   */
/* Return Value  : XOI_SUCCESS or a negative integer indicating    */
/*		   the type of error		   		   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static OM_integer xoi__get_class_ptr(
    OM_string  *omclass,   /* IN  - OM Class Identifier		       */
    xoi_class **class)     /* OUT - Pointer to the xoi_class structure */
{
xoi_omclass_struct  *omclass_ptr;
xoi_omclass_ord      srch_class, *p;
OM_uint		     index;

    /* Get the OM Class Information pointer */
    omclass_ptr = xoi_get_omclass_ptr();

    srch_class.omclass = omclass;

    /* Search the OM Class Identifier */
    p = (xoi_omclass_ord *) bsearch((void *) &srch_class, 
			            (void *) xoi_omclass_array, 
			            (size_t) omclass_ptr->count,
			            (size_t) sizeof(xoi_omclass_ord), 
			            xoi__compare_omclass);

    /* OM Class Identifier not found */
    if (p == NULL)
	return(XOI_INVALID_CLASS_NAME);

    index = p->index;

    *class = &(omclass_ptr->class[index]);

    return(XOI_SUCCESS);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi__compare_obj_abbrv			   */
/* Description   : This function is used by the qsort function for */
/*		   comparing Object Class Abbreviations.    	   */
/* Return Value  : An integer value less than, equal to or greater */
/*		   than zero depending on whether the first element*/
/*		   is less than, equal to or greater than the 	   */
/*		   second element.          			   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static int xoi__compare_obj_abbrv(
    const void *abbrv1,   /* IN - The first xoi_obj_abbrv_ord element  */
    const void *abbrv2)   /* IN - The second xoi_obj_abbrv_ord element */
{
int retval;

    retval = strcmp(((xoi_obj_abbrv_ord *) abbrv1)->abbrv, 
		    ((xoi_obj_abbrv_ord *) abbrv2)->abbrv);

    /* Duplicate Abbreviation */
    if (retval == 0)
	xoi_dup_abbrv = OM_TRUE;

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi__compare_obj_id			   	   */
/* Description   : This function is used by the qsort function for */
/*		   comparing Object Class Identifiers.      	   */
/* Return Value  : An integer value less than, equal to or greater */
/*		   than zero depending on whether the first element*/
/*		   is less than, equal to or greater than the 	   */
/*		   second element.          			   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static int xoi__compare_obj_id(
    const void *id1,   /* IN - The first xoi_obj_id_ord element  */
    const void *id2)   /* IN - The second xoi_obj_id_ord element */
{
int retval;

    retval = xoi__compare_om_strings(((xoi_obj_id_ord *) id1)->e_objid,
				     ((xoi_obj_id_ord *) id2)->e_objid);

    /* Duplicate Object Identifiers */
    if (retval == 0)
	xoi_dup_objid = OM_TRUE;

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi__compare_attr_abbrv			   */
/* Description   : This function is used by the qsort function for */
/*		   comparing Attribute Abbreviations.    	   */
/* Return Value  : An integer value less than, equal to or greater */
/*		   than zero depending on whether the first element*/
/*		   is less than, equal to or greater than the 	   */
/*		   second element.          			   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static int xoi__compare_attr_abbrv(
    const void *abbrv1,   /* IN - The first xoi_attr_abbrv_ord element  */
    const void *abbrv2)   /* IN - The second xoi_attr_abbrv_ord element */
{
int retval;

    retval = strcmp(((xoi_attr_abbrv_ord *) abbrv1)->abbrv, 
		    ((xoi_attr_abbrv_ord *) abbrv2)->abbrv);

    /* Duplicate Abbreviation */
    if (retval == 0)
	xoi_dup_abbrv = OM_TRUE;

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi__compare_attr_id			   	   */
/* Description   : This function is used by the qsort function for */
/*		   comparing Attribute Object Identifiers.  	   */
/* Return Value  : An integer value less than, equal to or greater */
/*		   than zero depending on whether the first element*/
/*		   is less than, equal to or greater than the 	   */
/*		   second element.          			   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static int xoi__compare_attr_id(
    const void *id1,   /* IN - The first xoi_attr_id_ord element  */
    const void *id2)   /* IN - The second xoi_attr_id_ord element */
{
int retval;

    retval = xoi__compare_om_strings(((xoi_attr_id_ord *) id1)->e_objid,
				     ((xoi_attr_id_ord *) id2)->e_objid);

    /* Duplicate Object Identifiers */
    if (retval == 0)
	xoi_dup_objid = OM_TRUE;

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi__compare_omclass			           */
/* Description   : This function is used by the qsort function for */
/*		   comparing OM Class Identifiers.	   	   */
/* Return Value  : An integer value less than, equal to or greater */
/*		   than zero depending on whether the first element*/
/*		   is less than, equal to or greater than the 	   */
/*		   second element.          			   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static int xoi__compare_omclass(
    const void *class1,   /* IN - The first xoi_omclass_ord element  */
    const void *class2)   /* IN - The second xoi_omclass_ord element */
{
int retval;

    retval = xoi__compare_om_strings(((xoi_omclass_ord *) class1)->omclass,
				     ((xoi_omclass_ord *) class2)->omclass);

    /* Duplicate Class Identifiers */
    if (retval == 0)
	xoi_dup_omclass = OM_TRUE;

    return(retval);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi__compare_om_strings		   	   */
/* Description   : This function is used to compare two OM_strings */
/* Return Value  : An integer value less than, equal to or greater */
/*		   than zero depending on whether the first element*/
/*		   is less than, equal to or greater than the 	   */
/*		   second element.          			   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
static int xoi__compare_om_strings(
    OM_string *str1,   /* IN - The first OM_string to be compared  */
    OM_string *str2)   /* IN - The second OM_string to be compared */
{
int retval;

    if (str1->length < str2->length)
    {
        retval = memcmp(str1->elements, str2->elements, str1->length);

        /* If they match, since length of str1 is less, str2 must be greater */
	if (retval == 0)
	    return(-1);

	return(retval);
    }

    if (str1->length > str2->length)
    {
        retval = memcmp(str1->elements, str2->elements, str2->length);

        /* If they match, since length of str2 is less, str1 must be greater */
	if (retval == 0)
	    return(1);

        return(retval);
    }

    /* Lengths of both are equal */
    return(memcmp(str1->elements, str2->elements, str1->length));

}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xoi_free_sort_structures                        */
/* Description   : This function is used to free the area occupied */
/*                 by the sorted structures.   			   */
/* Return Value  : None					   	   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.     	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void xoi_free_sort_structures(
    void)
{

    if (xoi_obj_abbrv_array != NULL)
    {
	free(xoi_obj_abbrv_array);
	xoi_obj_abbrv_array = NULL;
    }

    if (xoi_obj_id_array != NULL)
    {
	free(xoi_obj_id_array);
	xoi_obj_id_array = NULL;
    }

    if (xoi_attr_abbrv_array != NULL)
    {
	free(xoi_attr_abbrv_array);
	xoi_attr_abbrv_array = NULL;
    }

    if (xoi_attr_id_array != NULL)
    {
	free(xoi_attr_id_array);
	xoi_attr_id_array = NULL;
    }

    if (xoi_omclass_array != NULL)
    {
	free(xoi_omclass_array);
	xoi_omclass_array = NULL;
    }
}

/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
