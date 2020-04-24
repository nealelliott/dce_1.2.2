/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: main.c,v $
 * Revision 1.1.4.2  1996/02/17  23:59:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:07  marty]
 *
 * 	KK merged upto DCE1_0_3b03.
 *
 * 	Added #include <hpdce_version.h>.
 * 	[1993/05/26  14:13:50  tatsu_s]
 *
 * Revision 1.1.4.1  1995/12/07  22:36:17  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/01/30  22:39 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:32 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	HP revision /main/HPDCE02/1  1994/08/03  16:33 UTC  tatsu_s
 * 	Merged mothra up to dce 1.1.
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/21  22:48  lmm
 * 	merge kk and hpdce01
 * 	[1995/12/07  21:18:11  root]
 * 
 * Revision 1.1.6.3  1993/09/15  15:23:56  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.5.2  1993/07/08  09:11:59  root
 * 	Initial King Kong branch
 * 	[1993/07/08  09:11:19  root]
 * 
 * Revision 1.1.2.3  1993/01/03  22:14:55  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:47:59  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  19:15:38  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:11:09  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:00:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/********************************************************************/
/*                                                                  */
/*                          M A I N                                 */
/*                                                                  */
/*                  Mainline for to2 translator                     */
/*                                                                  */
/********************************************************************/

/*
 * ==========================================================================
 * Confidential and Proprietary.  Copyright 1987 by Apollo Computer Inc.,
 * Chelmsford, Massachusetts.  Unpublished -- All Rights Reserved Under
 * Copyright Laws Of The United States.
 *
 * Apollo Computer Inc. reserves all rights, title and interest with respect
 * to copying, modification or the distribution of such software programs and
 * associated documentation, except those rights specifically granted by Apollo
 * in a Product Software Program License, Source Code License or Commercial
 * License Agreement (APOLLO NETWORK COMPUTING SYSTEM) between Apollo and
 * Licensee.  Without such license agreements, such software programs may not
 * be used, copied, modified or distributed in source or object code form.
 * Further, the copyright notice must appear on the media, the supporting
 * documentation and packaging as set forth in such agreements.  Such License
 * Agreements do not grant any rights to use Apollo Computer's name or trademarks
 * in advertising or publicity, with respect to the distribution of the software
 * programs without the specific prior written permission of Apollo.  Trademark
 * agreements may be obtained in a separate Trademark License Agreement.
 * ==========================================================================
 */

#include <stdio.h>
#include <signal.h>
#ifdef vms
#   include <types.h>
#   include <stat.h>
#else
#   include <sys/types.h>
#   include <sys/stat.h>
#   include <fcntl.h>
#endif
#include <ast.h>
#include <utils.h>
#include <frontend.h>
#include <emit.h>

extern void exit __PROTOTYPE((int));
#ifndef VMS
void main __PROTOTYPE((int argc, char **argv));
void main(argc, argv)
#else
int main __PROTOTYPE((int argc, char **argv));
int main(argc, argv)
#endif
    int  argc;
    char *argv[];
{
    char *source_file;
    binding_t *ast;
    FILE *dest_file;

    NIDL_init();
    source_file = NIDL_parse_args(argc-1, &argv[1]);
    ast = NIDL_parse(source_file);
    NIDL_translate(ast);

    exit (pgm_$ok);
}
