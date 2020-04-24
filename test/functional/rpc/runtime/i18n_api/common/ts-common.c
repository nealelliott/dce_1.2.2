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
 * $Log: ts-common.c,v $
 * Revision 1.1.4.2  1996/02/18  00:32:15  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:17:49  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:47:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:25:10  root]
 * 
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  18:16:26  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:42:33  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:14:55  devsrc]
 * 
 * Revision 1.1.2.1  1994/05/04  21:53:11  mori_m
 * 	CR 9701: RPC runtime I18N extension functional test drop.
 * 	[1994/05/04  21:37:25  mori_m]
 * 
 * $EndLog$
 */

#include <stdio.h>
#include <varargs.h>

/*
 * prints information to stdout, and to the tet journal, this is needed
 * to meet the HP TET requirements as well as the OSF TET requirements.
 */
void
mtet_infoline(va_alist)
va_dcl
{
        va_list vargs;
        char msg[1024];                 /* buffer for info lines */
        char *fmt;

        va_start(vargs);
        fmt = va_arg(vargs, char *);
        (void) vsprintf(msg, fmt, vargs);
        va_end(vargs);
        printf (msg);
        tet_infoline (msg);
}
