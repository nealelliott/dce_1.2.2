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
 * $Log: tst_tet_util.c,v $
 * Revision 1.1.4.2  1996/02/17  23:12:03  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:33:29  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:54:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:27:42  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:44:50  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:16:30  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/10  22:38:20  bhaim
 * 	Common TET functions
 * 	[1994/03/10  22:36:58  bhaim]
 * 
 * $EndLog$
 */

#include <stdio.h>              /* for printf's         */
#include <stdarg.h>             /* for va_arg macros    */
#include <tet_api.h>            /* TET api header file  */

/*
 *  ROUTINE NAME:       tst_tet_printf
 *
 *  SCOPE:              PUBLIC (intended for use by DME system tests)
 *
 *  DESCRIPTION:
 *  tst_tet_printf is a wrapper around tet_infoline that allows the user
 *  to provide information to print in the same form that printf takes.  See
 *  the printf() manpage for more information on the input and output
 *  parameters
 *
 */

int
tst_tet_printf(const char *format,...)
{

  va_list ap;
  char buf[1024];
  int retcode;

  va_start(ap, format);
  retcode=vsprintf(buf, format, ap);
  tet_infoline(buf);
  return(retcode);

}

