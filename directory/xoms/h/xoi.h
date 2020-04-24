/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xoi.h,v $
 * Revision 1.1.4.2  1996/03/11  13:24:52  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:09  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:49:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:11  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:21:50  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:29  devsrc]
 * 
 * Revision 1.1.2.2  1994/03/23  15:50:25  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:38:39  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:02  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:13:47  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:55:54  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOI - XOM Object Info Module                      */
/* File Name   : xoi.h                                       	   */
/* Description : This file contains all the defines and function   */
/* 		 prototypes for the public XOI routines            */
/* Date        : July 1, 1993                                      */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _XOI_H
#define _XOI_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xoi_rcsid[] = "@(#)$RCSfile: xoi.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:52 $";
#endif


/*-- Defines for Error Returns -----------------------------------------------*/
#define XOI_SUCCESS                ((OM_integer)  0)
#define XOI_CANNOT_READ_SCHEMA     ((OM_integer) -1)
#define XOI_SCHEMA_NOT_READ	   ((OM_integer) -2)
#define XOI_NO_START_OBJ_BLOCK	   ((OM_integer) -3)
#define XOI_NO_END_OBJ_BLOCK	   ((OM_integer) -4)
#define XOI_EMPTY_OBJ_BLOCK	   ((OM_integer) -5)
#define XOI_OBJ_FORMAT_ERROR	   ((OM_integer) -6)
#define XOI_DUPLICATE_OBJ_ABBRV    ((OM_integer) -7)
#define XOI_DUPLICATE_OBJ_OBJ_ID   ((OM_integer) -8)
#define XOI_NO_START_ATTR_BLOCK	   ((OM_integer) -9)
#define XOI_NO_END_ATTR_BLOCK	   ((OM_integer) -10)
#define XOI_EMPTY_ATTR_BLOCK	   ((OM_integer) -11)
#define XOI_ATTR_FORMAT_ERROR	   ((OM_integer) -12)
#define XOI_DUPLICATE_ATTR_ABBRV   ((OM_integer) -13)
#define XOI_DUPLICATE_ATTR_OBJ_ID  ((OM_integer) -14)
#define XOI_NO_START_CLASS_BLOCK   ((OM_integer) -15)
#define XOI_NO_END_CLASS_BLOCK	   ((OM_integer) -16)
#define XOI_EMPTY_CLASS_BLOCK	   ((OM_integer) -17)
#define XOI_CLASS_FORMAT_ERROR     ((OM_integer) -18)
#define XOI_NO_CLASS_NAME          ((OM_integer) -19)
#define XOI_DUPLICATE_CLASS_BLOCK  ((OM_integer) -20)
#define XOI_CLASS_BLOCK_UNDEFINED  ((OM_integer) -21)
#define XOI_INVALID_ABBRV          ((OM_integer) -22)
#define XOI_INVALID_OBJ_ID	   ((OM_integer) -23)
#define XOI_INVALID_CLASS_NAME     ((OM_integer) -24)
#define XOI_INVALID_SYNTAX     	   ((OM_integer) -25)
#define XOI_MEMORY_INSUFFICIENT    ((OM_integer) -26)
#define XOI_INVALID_PARAMETER	   ((OM_integer) -27)
#define XOI_UNKNOWN_ABBRV	   ((OM_integer) -28)
#define XOI_UNKNOWN_OBJ_ID	   ((OM_integer) -29)
#define XOI_UNKNOWN_OMTYPE	   ((OM_integer) -30)


/*-- Function Prototypes ------------------------------------------*/
void xoi_free(
    void);

OM_integer xoi_initialize(
    OM_uint *line_no); /* OUT - Contains erroneous line No.in case of errors */

OM_integer xoi_get_objclass_count(
    OM_uint *obj_count); /* OUT - Count of number of object classes */

OM_integer xoi_get_obj_info_from_index(
    OM_uint  index,     /* IN  - Index into the object class array */
    char    *obj_id,    /* OUT - Object identifier		   */
    char    *abbrv,     /* OUT - Object Class abbreviation	   */
    char    *full_name);/* OUT - Object Class full name		   */

OM_integer xoi_get_objclass_objid(
    char      *objclass,     /* IN  - Object Class Abbreviation or string */
    void     **e_objid,      /* OUT - The Encoded identifier		  */
    OM_uint32 *e_objid_len); /* OUT - Length of  the identifier		  */

OM_integer xoi_get_objclass_name(
    OM_string *e_objid,    /* IN  - Encoded Object Identifier */
    char      *abbrv,      /* OUT - Object Class Abbreviation */
    char      *full_name); /* OUT - Object Class Full Name    */

OM_integer xoi_get_attribute_count(
    OM_uint *attr_count); /* OUT - Count of number of attributes */

OM_integer xoi_get_attr_info_from_index(
    OM_uint  index,     /* IN  - Index into the attribute array */
    char    *obj_id,    /* OUT - Attribute Object identifier    */
    char    *abbrv,     /* OUT - Attribute abbreviation	        */
    char    *full_name);/* OUT - Attribute full name	        */

OM_integer xoi_get_attr_details(
    char       *attribute,   /* IN  - Attribute as an Abbreviation or a string*/
    OM_syntax  *omsyntax,    /* OUT - XOM syntax			      */
    void      **e_objid,     /* OUT - The Encoded identifier.		      */
    OM_uint32  *e_objid_len, /* OUT - Length of the identifier		      */
    void      **omclass,     /* OUT - XOM Class				      */
    OM_uint32  *omclass_len);/* OUT - Length of the Class identifier          */

OM_integer xoi_get_attr_name(
    OM_string *e_objid,    /* IN  - Encoded Object Identifier */
    char      *abbrv,      /* OUT - Object Class Abbreviation */
    char      *full_name); /* OUT - Object Class Full Name    */

OM_integer xoi_get_class_count(
    OM_uint *class_count); /* OUT - Count of number of classes */

OM_integer xoi_get_class_info_from_index(
    OM_uint  index,       /* IN  - Index into the OM Class array      */
    char    *class_name,  /* OUT - Class Name		              */
    OM_uint *comp_count); /* OUT - Number of components of this class */

OM_integer xoi_get_comp_info_from_index(
    OM_uint  class_index, /* IN  - Index into the OM Class array  */
    OM_uint  comp_index,  /* IN  - Index into the component array */
    char    *abbrv,       /* OUT - Component abbreviation 	  */
    char    *full_name);  /* OUT - Component full name		  */

OM_integer xoi_get_comp_details(
    OM_string  *omclass,         /* IN  - XOM Class  		        */
    char       *abbrv,           /* IN  - Abbreviation of the component */
    OM_type    *omtype,          /* OUT - XOM Type		        */
    OM_syntax  *omsyntax,        /* OUT - XOM Syntax		        */
    void      **comp_class,      /* OUT - Class Name of the Component   */
    OM_uint32  *comp_class_len); /* OUT - Length of the Class Name      */

OM_integer xoi_get_comp_name(
    OM_string *omclass,    /* IN  - XOM Class		   */
    OM_type    omtype,     /* IN  - XOM Type		   */
    char      *abbrv,      /* OUT - Component Abbreviation */
    char      *full_name); /* OUT - Component Full Name    */

OM_integer xoi_get_schema_file_name(
    char  *file_name); /* OUT - Name of the schema file */

OM_integer xoi_string_to_ber(
    char       *objid,    /* IN  - Object Identifier String      */
    OM_string **e_objid); /* OUT - BER Encoded Object Identifier */

OM_integer xoi_ber_to_string(
    OM_string *e_objid, /* IN  - Encoded Object Identifier */
    char      *objid);  /* OUT - Converted string	   */

#endif  /* ifndef _XOI_H */
