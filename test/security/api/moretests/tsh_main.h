/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: tsh_main.h,v $
 * Revision 1.1.4.2  1996/02/18  23:07:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:45  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:50:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:53:08  root]
 * 
 * Revision 1.1.2.2  1992/12/31  17:33:50  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:58:46  htf]
 * 
 * Revision 1.1  1992/01/19  04:07:46  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/* 
*/ 

#ifndef TSH_MAIN_INCLUDED
#define TSH_MAIN_INCLUDED
/*
**  Copyright (c) 1990 by
**      Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass. &
**      Locus Computing Corporation, Burlington, Mass.
**
**
**  $Log: tsh_main.h,v $
 * Revision 1.1.4.2  1996/02/18  23:07:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:45  marty]
 *
 * Revision 1.1.5.2  1996/02/18  22:28:45  marty
 * 	Update OSF copyright years
 *
 * Revision 1.1.4.1  1995/12/11  21:50:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:53:08  root]
 *
 * Revision 1.1.3.2  1995/12/11  20:53:08  root
 * 	Submit OSF/DCE 1.2.1
 *
 * Revision 1.1.2.2  1992/12/31  17:33:50  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:58:46  htf]
 *
 * Revision 1.1.1.2  1992/12/31  16:58:46  htf
 * 	Embedding copyright notice
 *
 * Revision 1.1  1992/01/19  04:07:46  devrcs
 * Initial revision
 *
 * Revision 1.1  91/08/22  11:07:18  finger
 * Initial check-in
 * 
**
** 
**  TSH LIBRARY HEADER 
**	Extracted	1/11/91 18:18:17
**	from delta on	11/12/90 15:59:13
*/
#ifndef LINT
static char tsh_main_sccs_id[] = "@(#)tsh_main.h	1.1";
#endif


#include <dce/rpc.h>


/*  Here is the context block that holds various bits of info relative to each 
 *  test program that gets executed from the TSH.  These are in effect global vars
 *  because of the way the context block is used.  It is typoically the first
 *  parameter to any function that needs the context info that it contains.
 */
typedef struct ctx
    {
    char_p_t       *ProgramName;        /* program name is argv[0] off command line */
    ndr_long_int   stderrSet;           /* set if -s encountered on command line */
    ndr_long_int   PerformanceSet;      /* set if -p encountered on command line */
    ndr_long_int   OptionsMask;         /* set to value specified by -d on cmd line */
    ndr_long_int   InfoUsed;            /* set when info block is printed */
    ndr_long_int   ErrorSet;            /* set when error is found during test */
    char_p_t       *InfoString;         /* info string about test from .tsh file */
    } CtxBlock;

#endif
