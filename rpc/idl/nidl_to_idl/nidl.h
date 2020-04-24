/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: nidl.h,v $
 * Revision 1.1.4.2  1996/02/18  23:46:07  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:53  marty]
 *
 * Revision 1.1.4.1  1995/12/07  22:36:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:18:23  root]
 * 
 * Revision 1.1.2.3  1993/01/03  22:15:06  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:48:17  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  19:16:04  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:11:26  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:00:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/********************************************************************/
/*                                                                  */
/*                          N I D L . H                             */
/*                                                                  */
/*                  Mainline for IDL compilers.                     */
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

#include <ast.h>

#ifndef __PROTOTYPE
#ifdef __STDC__
#define __PROTOTYPE(x) x
#else
#define __PROTOTYPE(x) ()
#endif
#endif

    /********************************/
    /*  Initialize NIDL frontend    */
    /********************************/

void NIDL_init __PROTOTYPE((void)) ;

    /****************************************/
    /*  Parse arguments to NIDL compiler    */
    /****************************************/

char *NIDL_parse_args(
#ifdef __STDC__
    int   argc,
    char  **argv
#endif
    ) ;

    /****************************/
    /*  Parse a NIDL file       */
    /****************************/

binding_t *NIDL_parse(
#ifdef __STDC__
    char *source_file
#endif
    ) ;

    /****************************/
    /*  Generate header files   */
    /****************************/

void NIDL_gen_hdrs(
#ifdef  __STDC__
    binding_t *ast_p
#endif
    ) ;

    /****************************/
    /*  Generate stubs          */
    /****************************/

void  NIDL_gen_stubs(
#ifdef  __STDC__
    binding_t *ast_p
#endif
    ) ;
