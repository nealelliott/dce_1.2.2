/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: frontend.h,v $
 * Revision 1.1.4.2  1996/02/18  23:46:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:44:47  marty]
 *
 * Revision 1.1.4.1  1995/12/07  22:35:48  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:18:00  root]
 * 
 * Revision 1.1.2.3  1993/01/03  22:14:46  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:47:42  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  19:15:14  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  01:10:53  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:00:04  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
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

binding_t *parse(
#ifdef __STDC__
    STRTAB_str_t    source_file,
    void            (*error_proc)(char *filename),
    char            **impdirs,
    char            **def_strings
#endif
);

void NIDL_init(
#ifdef __STDC__
    void
#endif
) ;

char *NIDL_parse_args(
#ifdef __STDC__
    int argc,
    char **argv
#endif
) ;

binding_t *NIDL_parse(
#ifdef __STDC__
    char    *source_file
#endif
) ;

void NIDL_translate(
#ifdef __STDC__
        binding_t *ast
#endif
) ;

#ifdef __STDC__
void warn(char *mesg);
void fatal_error(char *fmt, char *arg);
void unimp_type_exit(type_kind kind, char *label);
#else
void warn();
void fatal_error();
void unimp_type_exit();
#endif

void terminate(
#ifdef __STDC__
    void
#endif
) ;
