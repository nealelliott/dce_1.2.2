/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: xt_file.h,v $
 * Revision 1.1.4.2  1996/02/18  23:04:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:54  marty]
 *
 * Revision 1.1.4.1  1995/12/08  22:13:31  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:51  root]
 * 
 * Revision 1.1.2.3  1994/06/21  14:28:47  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  07:47:35  marrek]
 * 
 * Revision 1.1.2.2  1994/06/10  20:19:38  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:58:40  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:15:53  marrek
 * 	Beautification.
 * 	[1994/02/09  15:45:22  marrek]
 * 
 * 	Created for dce1.1 code drop.
 * 	[1994/02/09  11:30:23  marrek]
 * 
 * $EndLog$
 */
#ifndef _XT_FILE_H
#define _XT_FILE_H

#if !defined(lint) && defined(GDS_RCS_ID)
static char xt_file_rcsid[] = "@(#)$RCSfile: xt_file.h,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 23:04:08 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1994   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/


#include <xom.h>

OM_sint xt_openo(void);
OM_sint xt_opena(void);
OM_sint xt_closea(void);
OM_sint xt_opent(void);
OM_sint xt_closet(void);
OM_sint xt_openp(const char *filename);
OM_sint xt_closep(void);
OM_sint xt_rtest(void);
OM_sint xt_rtest_till_last(void);
void xt_log_head(const char *entry);    /* Write testcase header to log file */
OM_sint xt_log_func(const char *entry);     /* Write function name to log file   */
void xt_log_pass(const char *entry);    /* Write "PASSED" to log file        */
void xt_log_fail(const char *entry);           /* Write "FAILED" to log file        */
void xt_log_default_context (void);
void xt_log_default_session (void);
OM_sint xt_read_parm ( OM_sint parm_type,
	OM_sint table_type,
	OM_sint *r_table_index,
	char *r_value,
OM_sint *r_void );
OM_sint xt_read_object ( OM_sint parm_type,
	OM_sint *table_type,
	OM_sint *r_table_index,
	char *r_value,
	OM_sint *r_void,
	OM_object *object );
OM_sint xt_read_private_object ( OM_sint parm_type,
	OM_sint table_type,
	OM_sint *r_table_index,
	char *r_value,
	OM_sint *r_void,
	OM_private_object *private_object );

OM_sint xt_read_public_object ( OM_sint parm_type,
	OM_sint table_type,
	OM_sint *r_table_index,
	char *r_value,
	OM_sint *r_void,
OM_public_object *public_object );

OM_sint xt_read_string (char *r_value);
OM_sint xt_read_int (OM_sint *value);
OM_sint xt_read_equals (char **r_str, OM_sint *r_val);
OM_sint xt_read_quoted_string (char *return_string);
char    *xt_itoa (OM_sint i);
char    *xt_ptoa (char *p);      /* Pointer to Ascii.                 */
OM_sint xt_fnext(void);
void    xt_err(const char *func, const char *msg);
void    xt_err1(const char *func, const char *msg, const char *p1);
void    xt_err2(const char *func, const char *msg, const char *p1, const char *p2);
void    xt_errx(const char *func, const char *msg);
void    xt_errx1(const char *func, const char *msg, const char *p1);
void    xt_errx2(const char *func, const char *msg, const char *p1, const char *p2);
void    xt_perrx(const char *func, const char *msg);
void    xt_exit(OM_sint exitc);
void    xt_log_nocrlf(const char *msg);
void    xt_log_cat(const char *msg);
void    xt_log_newline(void);
void    xt_log(const char *msg);
void    xt_log1(const char *msg, const char *p1);
void    xt_log2(const char *msg, const char *p1, const char *p2);
void    xt_log_both_nocrlf(const char *msg);
void    xt_log_both_cat(const char *msg);
void    xt_log_both_newline(void);
void    xt_log_both(const char *msg);
void    xt_log_both1(const char *msg, const char *p1);
void    xt_log_both2(const char *msg, const char *p1, const char *p2);
#endif /* ifndef _XT_FILE_H */
