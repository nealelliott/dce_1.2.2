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
 * $Log: util_var.h,v $
 * Revision 1.1  1996/10/07  16:40:43  wwang
 * Initial revision
 *
 * Revision 1.1  1996/10/01  17:07:12  wwang
 * Initial revision
 *
 * Revision 1.1.4.6  1994/08/31  23:03:57  salamone
 * 	CR11916 - Add _b(sec) support.
 * 	[1994/08/31  23:02:21  salamone]
 *
 * Revision 1.1.4.5  1994/08/01  20:41:27  rousseau
 * 	Add _h support (CR 11285).
 * 	[1994/08/01  20:41:04  rousseau]
 * 
 * Revision 1.1.4.4  1994/06/09  16:02:44  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:48:20  devsrc]
 * 
 * Revision 1.1.4.3  1994/03/31  22:07:09  salamone
 * 	CR10259 - Remove global C variables for dcecp convenience variable support.
 * 	[1994/03/31  21:14:33  salamone]
 * 
 * Revision 1.1.4.2  1994/03/29  17:48:44  salamone
 * 	CR9854  - Remove malloc of fixed length convenience variables.
 * 	CR10087 - Provide list support of $_n
 * 	[1994/03/29  17:47:19  salamone]
 * 
 * Revision 1.1.4.1  1993/12/20  15:53:08  salamone
 * 	Remove length parameter from dcp_general_set_namelist()
 * 	[1993/12/17  20:04:33  salamone]
 * 
 * Revision 1.1.2.1  1993/12/14  17:15:30  salamone
 * 	Initial version of util_var.h
 * 	[1993/12/14  17:12:01  salamone]
 * 
 * $EndLog$
 */

/*
 * MODULE: 
 * util_var.h
 *
 * DESCRIPTION:
 * External declarations for dcecp convenience variables.
 *
 */

#ifndef _UTIL_VAR_H
#define _UTIL_VAR_H

/*
 * DECLARATIONS
 */
#define DCP_C_GENERAL_NULL                              "\0"

/*
 * Names of Convenience Variables
 */
#define DCP_C_GENERAL_VARNAME_PREFIX                    "_old"
#define DCP_C_GENERAL_NAME_LIST				"_n"
#define DCP_C_GENERAL_PARENT_NAME			"_p"
#define DCP_C_GENERAL_OBJECT				"_o"
#define DCP_C_GENERAL_SERVER_NAME			"_s"
#define DCP_C_GENERAL_BOUND_SERVER			"_b"
#define DCP_C_GENERAL_RETURN_VALUE			"_r"
#define DCP_C_GENERAL_PRINCIPAL			        "_u"
#define DCP_C_GENERAL_PRINCIPAL_CELL			"_c"
#define DCP_C_GENERAL_HOSTNAME 			        "_h"

/*
 * PROTOTYPES
 */
char *dcp_general_set_var(Tcl_Interp *, char *, char *, char *, int);  
void  dcp_general_unset_var(Tcl_Interp *, char *, char *, int);
void *dcp_general_init_var(Tcl_Interp *);
void  dcp_general_cleanup_var(Tcl_Interp *);

#endif /* _UTIL_VAR_H */
