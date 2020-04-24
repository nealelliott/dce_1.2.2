/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dtest.c,v $
 * Revision 1.1.11.1  1996/10/02  18:48:08  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:49:04  damon]
 *
 * Revision 1.1.6.2  1994/07/13  22:29:59  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:54:11  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:03:19  mbs]
 * 
 * Revision 1.1.6.1  1994/06/09  14:22:56  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:34:32  annie]
 * 
 * Revision 1.1.2.2  1993/01/21  15:55:27  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  17:38:49  cjd]
 * 
 * Revision 1.1  1992/01/19  02:50:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#include <cmd.h>

static cproc (as, arock)
char *arock;
struct cmd_syndesc *as; {
    register struct cmd_item *ti;
    printf("in the pear command\n");
    printf("number is %s\n", as->parms[0].items->data);
    if (as->parms[1].items) printf("running unauthenticated\n");
    for(ti=as->parms[2].items; ti; ti=ti->next) {
	printf("spotspos %s\n", ti->data);
    }
    return 0;
}

main(argc, argv)
int argc;
char **argv; {
    register struct cmd_syndesc *ts;
    
    ts = cmd_CreateSyntax((char *) 0, cproc, 1, "describe pear");
    cmd_AddParm(ts, "-num", CMD_SINGLE, 0, "number of pears");
    cmd_AddParm(ts, "-noauth", CMD_FLAG, CMD_OPTIONAL, "don't authenticate");
    cmd_AddParm(ts, "-spotpos", CMD_LIST, CMD_OPTIONAL, 0);
    
    return cmd_Dispatch(argc, argv);
}
