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
 * $Log: gcistruct.h,v $
 * Revision 1.1.4.2  1996/02/18  23:35:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:24:39  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:56:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:12:01  root]
 * 
 * Revision 1.1.2.3  1994/06/09  18:45:33  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:26  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  15:54:55  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:57:43  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:16:26  keutel
 * 	creation
 * 	[1994/03/21  12:54:28  keutel]
 * 
 * $EndLog$
 */

/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Project     : GDS Control Program                               */
/* File Name   : gcistruct.h                                       */
/* Description : This file contains all the defines and the	   */
/*		 data type definitions used by gdscp interpreter   */
/* Date        : September 24, 1993                                */
/* Author      : M. Kronenburg, SNI BU BA KP13                     */
/*                                                                 */
/*-----------------------------------------------------------------*/

#ifndef _GCISTRUCT_H
#define _GCISTRUCT_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char gcistruct_rcsid[] = "@(#)$RCSfile: gcistruct.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:35:01 $";
#endif

/*-- Data Types ---------------------------------------------------*/
typedef struct _gc_i_command_procedure
{                                                                               
    char         *name; /* command name string	    */
    Tcl_CmdProc  *proc; /* pointer to the procedure */
} gc_i_command_procedure;


/*-- Defines -------------------------------------------------------*/
#define GC_I_MAX_ERR_STRING       128   /* maximum error string 	     */
#define GC_T_MAX_CWO_ERR_STRING  1024   /* for current working object errors */

/*-- Defines for switches -----------------------------------*/
#define GC_I_VERBOSE_SWITCH			"-verbose"

/*-- Defines for commands -----------------------------------*/
#define GC_I_X500OBJ_COMMAND_STRING    		"x500obj"
#define GC_I_X500SVC_COMMAND_STRING    		"x500svc"
#define GC_I_X500ABBR_COMMAND_STRING   		"x500abbr"
#define GC_I_EXIT_COMMAND_STRING       		"exit"
#define GC_I_QUIT_COMMAND_STRING       		"quit"

/*-- Defines for operations ---------------------------------*/
#define GC_I_BIND_OPERATION_STRING		"bind"
#define GC_I_COMPARE_OPERATION_STRING		"compare"
#define GC_I_CREATE_OPERATION_STRING		"create"
#define GC_I_DELETE_OPERATION_STRING		"delete"
#define GC_I_LIST_OPERATION_STRING		"list"
#define GC_I_MODIFY_OPERATION_STRING		"modify"
#define GC_I_SEARCH_OPERATION_STRING		"search"
#define GC_I_SHOW_OPERATION_STRING		"show"
#define GC_I_HELP_OPERATION_STRING		"help"
#define GC_I_OPERATIONS_OPERATION_STRING	"operations"

#endif /* ifndef _GCISTRUCT_H */
