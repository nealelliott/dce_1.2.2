/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: omXext.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:32:04  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:39:27  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:53  root]
 * 
 * Revision 1.1.2.4  1994/06/10  21:21:42  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:24  devsrc]
 * 
 * Revision 1.1.2.3  1994/05/10  16:04:16  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:16:07  marrek]
 * 
 * Revision 1.1.2.2  1994/03/23  15:49:21  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:38:36  keutel]
 * 
 * Revision 1.1.2.1  1994/02/22  19:00:32  marrek
 * 	Beautification.
 * 	[1994/02/09  15:41:58  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  11:03:04  marrek]
 * 
 * $EndLog$
 */
#ifndef _OMXEXT_H
#define _OMXEXT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char omxext_rcsid[] = "@(#)$RCSfile: omXext.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:38:29 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code omX_fill(
    OM_type	      type,         /* IN  - Type of Object	      */
    OM_syntax	      syntax,       /* IN  - Syntax of the object     */
    OM_uint32	      length,       /* IN  - Data length	      */
    void	     *elements,     /* IN  - Data Value 	      */
    OM_descriptor    *destination); /* OUT - The filled up descriptor */

OM_return_code omX_fill_oid(
    OM_type	          type,         /* IN  - Type of Object	          */
    OM_object_identifier  object_id,    /* IN  - Value of the object      */
    OM_descriptor        *destination); /* OUT - The filled up descriptor */

OM_return_code omX_extract(
    OM_private_object  object,          /* IN  - Extract from this object    */
    OM_type_list       navigation_path, /* IN  - Leads to the target object  */
    OM_exclusions      exclusions,      /* IN  - Scope of extraction         */
    OM_type_list       included_types,  /* IN  - Objects to be extracted     */
    OM_boolean         local_strings,   /* IN  - Local strings required      */
    OM_value_position  initial_value,   /* IN  - First value to be extracted */
    OM_value_position  limiting_value,  /* IN  - Last value to be extracted  */
    OM_public_object  *values,	 	/* OUT - Array of extracted objects  */
    OM_value_position *total_number);	/* OUT - Count of extracted objects  */

OM_return_code omX_string_to_object(
    OM_workspace          workspace,     /* IN  - The workspace		      */
    OM_string            *string,        /* IN  - The string to be converted  */
    OM_object_identifier  class,         /* IN  - The OM Class to be created  */
    OM_boolean            local_strings, /* IN  - Local strings specified     */
    OM_private_object    *object,        /* OUT - The converted Object	      */
    OM_integer	         *error_position,/* OUT - Error Position in I/P string*/
    OM_integer	         *error_type);   /* OUT - Type of error 	      */

OM_return_code omX_object_to_string(
    OM_object    object,        /* The Object to be converted          */
    OM_boolean	 local_strings, /* To indicate local string conversion */
    OM_string	*string);       /* The converted DN string 	       */

#endif /* ifndef _OMXEXT_H */
