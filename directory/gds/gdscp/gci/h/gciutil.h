/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: gciutil.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:40  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:56:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:05  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:34  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:27  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:54:56  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:57:46  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:28  keutel
 * 	creation
 * 	[1994/03/21  12:54:30  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gciutil.h                                         */
/* Description : File contains function prototypes for converting  */
/*		 results     					   */
/* Date        : September 24, 1993                                */
/* Author      : M. Kronenburg, SNI BU BA KP13                     */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _GCIUTIL_H
#define _GCIUTIL_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char gciutil_rcsid[] = "@(#)$RCSfile: gciutil.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:02 $";
#endif

/*-- Function Prototypes ------------------------------------------*/           
signed16 gc_i_convert_result(
    boolean        is_show_result, /* IN  - Result is from show operation */
    gc_t_vector   *vector,         /* IN  - Vector to convert 		  */
    Tcl_Interp    *interp);        /* OUT - The Tcl-interpreter           */

signed16 gc_i_convert_list_result(
    gc_t_vector  *vector,  /* IN  - Vector to convert   */
    Tcl_Interp   *interp); /* OUT - The Tcl-interpreter */

signed16 gc_i_convert_showabbrv_result(
    gc_t_vector  *vector,    /* IN  - Vector to convert   */
    Tcl_Interp   *interp);   /* OUT - The Tcl-interpreter */

signed16 gc_i_convert_showdsc_result(
    gc_t_vector  *vector,    /* IN  - Vector to convert   */
    Tcl_Interp   *interp);   /* OUT - The Tcl-interpreter */

signed16 gc_i_repack_arguments(
    char           **argv,              /* IN  - The argument vector          */
    int              argc,              /* IN  - Number of arguments          */
    gc_t_op_type     op_type,           /* IN  - Type of operation            */
    char          ***new_argv,          /* OUT - The created vector           */
    signed16        *new_argc,          /* OUT - Count of created vectors     */
    boolean         *structured_result, /* OUT - struc.result switch specified*/
    Tcl_Interp      *interp);           /* OUT - The Tcl-interpreter          */

char *gc_i_strdup(
    char *source); /* IN - The input string to be copied */

void gc_i_free_created_args(
    char **vector); /* IN - The error to be freed */

signed16 gc_i_check_operation(
    gc_i_command_procedure  *op_table,    /* IN  - The operation table        */
    char                    *user_string, /* IN  - The user supplied switch   */
    signed16                *table_index, /* OUT - Index into operation table */
    Tcl_Interp              *interp);     /* OUT - The Tcl-interpreter        */

signed16 gc_i_create_list_of_operations(
    gc_i_command_procedure  *op_table, /* IN  - The operation table */
    Tcl_Interp     	    *interp);  /* OUT - The Tcl-interpreter */

void gc_i_set_interpreter_error(
    signed16      retval,     /* IN  - The error code                  */
    char         *append_str, /* IN  - String to be appended in result */
    Tcl_Interp   *interp);    /* OUT - The Tcl-interpreter             */

void gc_i_set_interp_to_trans_error(
    gc_t_vector  *error_vector,  /* IN  - The Translator error vector      */
    signed16      retval,        /* IN  - The return value from Translator */
    Tcl_Interp   *interp);       /* OUT - The Tcl-interpreter              */

#endif /* ifndef _GCIUTIL_H */
