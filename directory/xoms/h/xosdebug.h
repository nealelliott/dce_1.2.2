/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xosdebug.h,v $
 * Revision 1.1.4.2  1996/03/11  13:25:01  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:16  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:50:34  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:24  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:22:34  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:37  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:14  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:15:16  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:57:13  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOM Convenience Functions                         */
/* File Name   : xosdebug.h					   */
/* Description : This file contains all the function prototypes    */
/*		 for the routines in xosdebug.c 		   */
/* Date        : July 1, 1993                                      */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _XOSDEBUG_H
#define _XOSDEBUG_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xosdebug_rcsid[] = "@(#)$RCSfile: xosdebug.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:25:01 $";
#endif

/*-- Function Prototypes ------------------------------------------*/
void xos_debug_string(
    char      *str,         /* IN - The string to be printed                */
    OM_uint32  len,         /* IN - Length of the string                    */
    OM_sint    indent_tag); /* IN - Tag to indicate the desired indentation */

void xos_debug_stack(
    xos_stack_item  *stack,         /* IN - The stack to be examined */
    OM_uint          stack_index);  /* IN - Index to top of stack    */

void xos_debug_read_token(
    xos_token read_token); /* IN - The token read */

#endif  /* ifndef _XOSDEBUG_H */
