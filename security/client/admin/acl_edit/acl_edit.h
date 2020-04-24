/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: acl_edit.h,v $
 * Revision 1.1.6.2  1996/03/11  13:26:06  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:10  marty]
 *
 * Revision 1.1.6.1  1995/12/08  17:55:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:28:27  root]
 * 
 * Revision 1.1.4.2  1992/12/29  12:36:47  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:32:42  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/08  15:24:46  burati
 * 	Add new functionality to support POSIX mask calculation
 * 	[1992/05/06  21:46:10  burati]
 * 
 * Revision 1.1  1992/01/19  14:45:14  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  acl_edit.h V=5 04/24/91 //littl/prgy/src/admin/dacl_edit
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      SEC_ACL Editor Base declarations
**
** Copyright Hewlett-Packard Company 1991
** 
*/

#ifndef dacl_edit__h_included
#define dacl_edit__h_included 

typedef enum {
    TTY_OUTPUT,
    GUI_OUTPUT
} sec_acl_edit_output_t;

typedef enum {
    POSIX_MASK_OBJ_CALC,
    CALC_MASK_OBJ_AFTER,
    NO_MASK_OBJ_CALC
} sec_acl_edit_mask_calc_t;

#endif
