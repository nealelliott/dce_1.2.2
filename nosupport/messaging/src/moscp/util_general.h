/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: util_general.h,v $
 * Revision 1.1  1996/10/07  16:40:43  wwang
 * Initial revision
 *
 * Revision 1.1  1996/10/01  17:07:12  wwang
 * Initial revision
 *
 * Revision 1.1.2.5  1994/08/19  22:15:52  rousseau
 * 	Made aud and dts attributes and values abbreviateable (CR 11025).
 * 	[1994/08/19  22:15:04  rousseau]
 *
 * Revision 1.1.2.4  1994/08/19  18:04:11  salamone
 * 	CR10683 - Provide generic routines to handle 1 sbin in TCL syntax.
 * 	[1994/08/19  17:50:17  salamone]
 * 
 * Revision 1.1.2.3  1994/06/09  16:02:36  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:12  devsrc]
 * 
 * Revision 1.1.2.2  1994/02/03  17:24:25  kevins
 * 	added a couple of interesting routines
 * 	[1994/02/03  17:18:49  kevins]
 * 
 * Revision 1.1.2.1  1993/12/20  20:08:11  kevins
 * 	New file for general utilities
 * 	[1993/12/20  18:49:19  kevins]
 * 
 * $EndLog$
 */

#ifndef _UTIL_GENERAL_H
#define _UTIL_GENERAL_H

#define GOOD_STATUS(stp)          ((stp)==NULL || (*stp) == error_status_ok)
#define CLEAR_STATUS(stp)         ((*stp) = error_status_ok)

char * expand_cell_name(char *, error_status_t *);
int isllist(char *);
int decode_uuid(uuid_t *, char *);
int input_parameter_check(Tcl_Interp *, unsigned32, int, char *);
int dcp_general_split_list(Tcl_Interp *, char *, int *, char ***);
int dcp_general_allocate_argv_for_one(Tcl_Interp *, char *, int *, char ***);
int dcp_general_find_attr_index(Tcl_Interp *, char *, int, char *[], int *);

#define NUM_PARAM 2    /* average number of parameters expected in commands */

#endif
