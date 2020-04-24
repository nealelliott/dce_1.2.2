/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xoistruct.h,v $
 * Revision 1.1.4.2  1996/03/11  13:24:56  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:12  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:50:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:16  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:21:55  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:32  devsrc]
 * 
 * Revision 1.1.2.2  1994/03/23  15:50:26  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:38:41  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:09  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:14:17  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:56:30  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOI - XOM Object Info Module                      */
/* File Name   : xoistruct.h					   */
/* Description : This file contains the structures and data type   */
/*		 definitions used by XOI			   */
/* Date        : July 1, 1993                                      */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _XOISTRUCT_H
#define _XOISTRUCT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xoistruct_rcsid[] = "@(#)$RCSfile: xoistruct.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:24:56 $";
#endif

/*-- Defines ------------------------------------------------------*/
#define XOI_START_BLOCK         '{'   /* Start of Block character  */
#define XOI_END_BLOCK           '}'   /* End of Block character    */
#define XOI_COMMENT_CHAR        '#'   /* For Comment character     */

#define XOI_MAX_OBJID        	 64   /* Max size of Obj.Id.string            */
#define XOI_MAX_LINE		256   /* Max size of line in Schema file      */
#define XOI_MAX_ABBRV	 	 16   /* Max size of Abbrv. in Schema file    */
#define XOI_MAX_NAME	 	 64   /* Max size of Full Name in Schema file */
#define XOI_MAX_FLD	 	 64   /* Max size of other fields in Schema   */
				      /* file.				      */
#define XOI_OBJ_BLOCK_FLD	  3   /* Total Number of fields in Object     */
				      /* Class Definition Block.	      */
#define XOI_ATTR_BLOCK_FLD	  5   /* Total Number of fields in Attribute  */
				      /* Definition Block.	      	      */
#define XOI_CLASS_BLOCK_FLD	  5   /* Total Number of OM Class Definition  */
				      /* Blocks.			      */
 

/*-- Data Structures ----------------------------------------------*/

/* Structures used to store Object Class Details */
typedef struct _xoi_oinfo
{
   char         *abbrv;		/* Object Class Abbreviation         	    */
   char         *name;		/* Full Name of the Object Class     	    */
   OM_string    *e_objid;	/* BER Encoded Obj.Identifier string 	    */
} xoi_oinfo;

typedef struct _xoi_object_class
{
   OM_uint	 count;		/* Total Number of Object Classes	    */
   xoi_oinfo	*obj;		/* Pointer to the Object Class Array	    */
} xoi_object_class;


/* Structures used to store details of Components of the OM Class */
typedef struct _xoi_cinfo
{
   char		     *abbrv;	  /* Abbreviation of the component	    */
   char		     *name;	  /* Full Name of the component		    */
   OM_type	      omtype;	  /* OM Type as defined by XOM		    */
   OM_syntax	      omsyntax;	  /* Syntax as defined by XOM		    */
   struct _xoi_class *comp_class; /* Pointer to OM Class structure. For     */
				  /* normal components it is NULL.	    */
} xoi_cinfo;

typedef struct _xoi_class
{
   OM_string	*omclass;	/* OM Class Name as defined by XOM	    */
   OM_uint	 count;	        /* Total Number of Components 		    */
   xoi_cinfo	*comps;		/* Pointer to the Component Array	    */
} xoi_class;

/* Structure used to store OM Class details */
typedef struct _xoi_omclass_struct
{
   OM_uint	 count;		/* Total number of OM Classes		    */
   xoi_class	*class;		/* Pointer to the OM Class array	    */
} xoi_omclass_struct;


/* Structures used to store Attribute Details */
typedef struct _xoi_ainfo
{
   char          *abbrv;	/* Attribute Abbreviation 		    */
   char          *name;         /* Full Name of the Attribute		    */
   OM_string     *e_objid;      /* BER Encoded Obj.Identifier string        */
   OM_syntax      omsyntax;     /* Attribute Syntax 			    */
   xoi_class 	 *class;	/* Pointer to the Class structure.          */
				/* For normal Attributes it is NULL.	    */
} xoi_ainfo;

typedef struct _xoi_attribute
{
   OM_uint	 count;		/* Total Number of Attributes		    */
   xoi_ainfo	*attr;		/* Pointer to the Attribute Array	    */
} xoi_attribute;


/* Structures used to temporarily store details read from a Definition	    */
/* Block when an OM Class Name is encountered. 				    */
typedef struct _xoi_tinfo
{
   OM_string	*omclass;	/* OM Class Name as defined by XOM          */
   OM_uint	 lineno;	/* Line Number in the Schema File where the */
				/* OM Class Name was encountered.    	    */
   OM_uint	 index;		/* Index to the Attribute Info. or          */
			  	/* Class Info array.	  		    */
   OM_boolean    if_attr;	/* Indicates if this Class Name corresponds */
				/* to an attribute or a component.	    */
				/* If OM_TRUE, then it is for an attribute. */
} xoi_tinfo;

typedef struct _xoi_tclass
{
   OM_uint	 count;		/* Total Number of encountered OM Class Names*/
   xoi_tinfo	*tinfo;		/* Pointer to an array of xoi_tinfo structure*/
} xoi_tclass;


/* Structure to store Object Class information sorted by Abbreviation  */
typedef struct _xoi_obj_abbrv_ord 
{ 
   char       *abbrv;    /* Object Class Abbreviation  */
   OM_uint     index;    /* Index into xoi_oinfo array */
} xoi_obj_abbrv_ord;

/* Structure to store Object Class information sorted by Object Identifiers */
typedef struct _xoi_obj_id_ord
{
   OM_string  *e_objid;  /* BER Encoded Object Identifier string  */
   OM_uint     index;    /* Index into xoi_oinfo array 		  */
} xoi_obj_id_ord;

/* Structure to store Attribute information sorted by Abbreviation */
typedef struct _xoi_attr_abbrv_ord
{
   char       *abbrv;    /* Attribute Abbreviation     */
   OM_uint     index;    /* Index into xoi_ainfo array */
} xoi_attr_abbrv_ord;

/* Structure to store Attribute information sorted by Object Identifiers */
typedef struct _xoi_attr_id_ord
{
   OM_string  *e_objid;  /* BER Encoded Object Identifier string  */
   OM_uint     index;    /* Index into xoi_ainfo array 	          */
} xoi_attr_id_ord;

/* Structure to store OM Class information sorted by OM Class Identifiers */
typedef struct _xoi_omclass_ord
{
   OM_string  *omclass;  /* OM Class Name as defined by XOM        */
   OM_uint     index;    /* Index into xoi_class array 		   */
} xoi_omclass_ord;

#endif  /* ifndef _XOISTRUCT_H */
