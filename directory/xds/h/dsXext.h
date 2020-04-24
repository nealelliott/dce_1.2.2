/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dsXext.h,v $
 * Revision 1.1.4.2  1996/02/18  23:37:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:30:00  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:35:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:56:11  root]
 * 
 * Revision 1.1.2.3  1994/06/10  21:20:19  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:55:57  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:04:03  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  13:15:27  marrek]
 * 
 * Revision 1.1.2.1  1994/02/22  19:23:35  marrek
 * 	February 1994 code drop
 * 	[1994/02/18  09:10:28  marrek]
 * 
 * 	Beautification.
 * 	[1994/02/09  15:39:46  marrek]
 * 
 * 	Created for DCE1.1 code drop.
 * 	[1994/02/09  10:58:21  marrek]
 * 
 * $EndLog$
 */
#ifndef _DSXEXT_H
#define _DSXEXT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char dsxext_rcsid[] = "@(#)$RCSfile: dsXext.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:37:42 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


OM_return_code 
dsX_extract_attr_values(
    OM_private_object     object,	  /* IN  - The source object 	      */
    OM_object_identifier  attribute_type, /* IN  - Attribute to be extracted  */
    OM_boolean            local_strings,  /* IN  - Local strings required     */
    OM_public_object     *values,	  /* OUT - Extracted Attribute Values */
    OM_value_position    *total_number);  /* OUT - Number of extracted values */
#endif /* ifndef _DSXEXT_H */
