/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: itest.c,v $
 * Revision 1.1.11.1  1996/10/02  18:48:12  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:49:05  damon]
 *
 * Revision 1.1.6.2  1994/07/13  22:30:00  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:55:56  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  16:03:21  mbs]
 * 
 * Revision 1.1.6.1  1994/06/09  14:22:57  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:34:33  annie]
 * 
 * Revision 1.1.2.2  1993/01/21  15:55:33  cjd
 * 	embedded copyright notice
 * 	[1993/01/20  17:38:56  cjd]
 * 
 * Revision 1.1  1992/01/19  02:50:48  devrcs
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

#include <stdio.h>
#include <dcedfs/compat.h>		/* dfs_dceErrTxt */

#include <cmd.h>

static cproc1 (as, arock)
char *arock;
struct cmd_syndesc *as; {
    printf("in the apple command\n");
    return 0;
}

static cproc2 (as, arock)
char *arock;
struct cmd_syndesc *as; {
    register struct cmd_item *ti;
    printf("in the pear command\n");
    printf("number is %s\n", as->parms[0].items->data);
    if (as->parms[1].items) printf("running unauthenticated\n");
    for(ti=as->parms[2].items; ti; ti=ti->next) {
	printf("spotspos %s\n", ti->data);
    }
    if (as->parms[8].items)
	printf("cell name %s\n", as->parms[8].items->data);
    return 0;
}

static cproc3(as, arock)
char *arock;
struct cmd_syndesc *as; {
    exit(0);
}

main(argc, argv)
int argc;
char **argv; {
    register struct cmd_syndesc *ts;
    char tline[1000];
    long tc;
    char *tp;
    long code;
    char *tv[100];
    
    initialize_cmd_error_table();

    ts = cmd_CreateSyntax("apple", cproc1, 0, "describe apple");

    ts = cmd_CreateSyntax("pear", cproc2, 1, "describe pear");
    cmd_AddParm(ts, "-num", CMD_LIST, 0, "number of pears");
    cmd_AddParm(ts, "-noauth", CMD_FLAG, CMD_OPTIONAL, "don't authenticate");
    cmd_AddParm(ts, "-spotpos", CMD_LIST, CMD_OPTIONAL | CMD_EXPANDS, 0);
    cmd_Seek(ts, 8);
    cmd_AddParm(ts, "-cell", CMD_SINGLE, CMD_OPTIONAL, "cell name");
    cmd_CreateAlias(ts, "alias");
    
    ts = cmd_CreateSyntax("quit", cproc3, 0, "quit");
    
    while (1) {
	printf("> ");
	tp = gets(tline);
	if (tp == (char *) 0) break;
	code = cmd_ParseLine(tline, tv, &tc, 100);
	if (code) {
	    printf("itest: parsing failure: %s\n", dfs_dceErrTxt(code));
	    exit(1);
	}
	code = cmd_Dispatch(tc, tv);
	cmd_FreeArgv(tv);
	if (code) {
	    printf("itest: execution failed: %s\n", dfs_dceErrTxt(code));
	}
    }
    exit(0);
}
