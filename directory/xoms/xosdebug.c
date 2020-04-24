/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xosdebug.c,v $
 * Revision 1.1.4.2  1996/03/11  13:25:23  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:13:35  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:51:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:26:56  root]
 * 
 * Revision 1.1.2.2  1994/06/10  21:23:03  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:56:58  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:01:33  marrek
 * 	Change rcsid string format.
 * 	[1994/02/09  08:31:10  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  11:00:01  marrek]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : XOM Convenience Functions                         */
/* File Name   : xosdebug.c					   */
/* Description : This file contains all the debug routines used    */
/*		 by the XOM convenience functions.		   */
/* Date	       : July 1, 1993					   */
/* Author      : B T Naik, SNI BU BA KP13, SISL India.             */
/*                                                                 */
/*-----------------------------------------------------------------*/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: xosdebug.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:25:23 $";
#endif

/* NOTE: This file is not THREADSAFE. It is not meant to be, since */
/*	 the routines included here are only debug routines and    */
/*	 will not be called by users of the convenience functions. */

#include <stdio.h>
#include <ctype.h>

#include <xom.h>

#include <xosstruct.h>
#include <xosdebug.h>


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_debug_string				   */
/* Description   : This function is used to print the debug        */
/*		   information to stderr.			   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void xos_debug_string(
    char      *str,  	   /* IN - The string to be printed 		   */
    OM_uint32  len,  	   /* IN - Length of the string     		   */
    OM_sint    indent_tag) /* IN - Tag to indicate the desired indentation */
{
OM_uint32  i;
static OM_sint spaces = 0;

    /* if NULL terminated string */
    if (len == 0)
    {
	switch (indent_tag)
	{
	    case (1):
		for (i = 0; i < spaces; ++i)
		    fputc(' ', stderr);
		fprintf(stderr, "%s\n", str);

		for (i = 0; i < spaces; ++i)
		    fputc(' ', stderr);
		fputc('[', stderr);

		spaces += 4;
		break;

	    case(-1):
		if (spaces > 0)
		    spaces -= 4;

		for (i = 0; i < spaces; ++i)
		    fputc(' ', stderr);
		fprintf(stderr, "]\n");

		for (i = 0; i < spaces; ++i)
		    fputc(' ', stderr);

		fprintf(stderr, "%s\n", str);
		break;

	    default:
		for (i = 0; i < spaces; ++i)
		    fputc(' ', stderr);
		fprintf(stderr, "%s", str);
		break;
	}
    }
    else
    {
	for (i = 0; i < spaces; ++i)
	    fputc(' ', stderr);
	fputc('<', stderr);
	for (i = 0; i < len; ++i)
        {
	    if (isprint(str[i]))
		fputc(str[i], stderr);
	    else
		fprintf(stderr, "\\%02x", str[i]);
        } 
	fputc('>', stderr);
    }
    fprintf(stderr, "\n");
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_debug_stack				   */
/* Description   : This function is used to print the contents of  */
/*		   the stack content during a string to filter	   */
/*		   operation.					   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void xos_debug_stack(
    xos_stack_item  *stack,      /* IN - The stack to be examined */
    OM_uint	     stack_index)/* IN - Index to top of stack    */
{
OM_uint  i;
char    *debug_msg;

    xos_debug_string("Stack Contents : ", 0, 1);

    for (i = stack_index - 1; stack[i].token != XOS_S_START; --i)
    {
	switch (stack[i].token)
	{
	    case (XOS_S_START):
		debug_msg = "XOS_S_START";
		break;
	    case (XOS_S_BRACKET_OPEN):
		debug_msg = "XOS_S_BRACKET_OPEN";
		break;
	    case (XOS_S_NOT):
		debug_msg = "XOS_S_NOT";
		break;
	    case (XOS_S_AND):
		debug_msg = "XOS_S_AND";
		break;
	    case (XOS_S_OR):
		debug_msg = "XOS_S_OR";
		break;
	    case (XOS_S_FILTER_ITEM):
		debug_msg = "XOS_S_FILTER_ITEM";
		break;
	    case (XOS_S_FILTER):
		debug_msg = "XOS_S_FILTER";
		break;
	    default:
		debug_msg = "UNKNOWN STACK TOKEN";
		break;
	}
	xos_debug_string(debug_msg, 0, 0);
    }

    xos_debug_string("XOS_S_START", 0, 0);

    xos_debug_string("End Stack Contents", 0, -1);
}


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name : xos_debug_read_token				   */
/* Description   : This function is used to print the enumerated   */
/*		   value of the read token during a string to      */
/*		   filter operation.				   */
/* Return Value  : NONE						   */
/* Author        : B T Naik, SNI BU BA KP13, SISL India.	   */
/*                                                                 */
/*-----------------------------------------------------------------*/
void xos_debug_read_token(
    xos_token read_token)  /* IN - The token read */
{
char *debug_msg;

    switch(read_token)
    {
	case (XOS_F_ABBRV):
	    debug_msg = "Read Token = XOS_F_ABBRV";
	    break;
	case (XOS_F_BRACKET_OPEN):
	    debug_msg = "Read Token = XOS_F_BRACKET_OPEN";
	    break;
	case (XOS_F_BRACKET_CLOSE):
	    debug_msg = "Read Token = XOS_F_BRACKET_CLOSE";
	    break;
	case (XOS_F_NOT):
	    debug_msg = "Read Token = XOS_F_NOT";
	    break;
	case (XOS_F_AND):
	    debug_msg = "Read Token = XOS_F_AND";
	    break;
	case (XOS_F_OR):
	    debug_msg = "Read Token = XOS_F_OR";
	    break;
	case (XOS_F_EOS):
	    debug_msg = "Read Token = XOS_F_EOS";
	    break;
	default:
	    debug_msg = "Unknown Read Token";
	    break;
    }
    xos_debug_string(debug_msg, 0, 0);
}


/*-----------------------------------------------------------------*/
/* End of file.                                                    */
/*-----------------------------------------------------------------*/
