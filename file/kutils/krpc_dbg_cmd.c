/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: krpc_dbg_cmd.c,v $
 * Revision 1.1.11.1  1996/10/02  17:53:12  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:41:30  damon]
 *
 * Revision 1.1.6.3  1994/07/13  22:28:34  devsrc
 * 	merged with bl-10
 * 	[1994/06/29  11:38:14  devsrc]
 * 
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:47  mbs]
 * 
 * Revision 1.1.6.2  1994/06/09  14:12:09  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:25:37  annie]
 * 
 * Revision 1.1.6.1  1994/02/04  20:21:33  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:14:27  devsrc]
 * 
 * Revision 1.1.4.1  1994/01/28  20:43:46  annie
 * 	expand OSF copyright
 * 	[1994/01/28  20:42:19  annie]
 * 
 * Revision 1.1.2.2  1993/03/12  20:56:03  delgado
 * 	Add support for krpc to use dfs icl logs
 * 	[1993/03/10  23:08:03  delgado]
 * 
 * $EndLog$
 */

#include <stdio.h>

#include <dcedfs/osi.h>
#include <dcedfs/param.h>
#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>

#include <dcedfs/syscall.h>

#include <krpcdbg.h>


#define ON_LOGNAME_OFFSET 0
#define ON_LEVEL_OFFSET  1

#define SET_LEVEL_OFFSET  0

krpc_dbg_set(syndesc, a)
    struct cmd_syndesc *syndesc;
    char *a;
{
    char *debug_level;
    int code;
    
    debug_level = syndesc->parms[SET_LEVEL_OFFSET].items->data;
    code = afs_syscall(AFSCALL_KRPCDBG, KRPC_DBG_SET, debug_level, NULL,
                       NULL, NULL);
    if (code){
	perror("can't set debug level\n");
    }

    
}

krpc_dbg_on(syndesc, a)
    struct cmd_syndesc *syndesc;
    char *a;
{
    char *logname=0;
    char *debug_level=0;
    int  code=0;

    logname = syndesc->parms[ON_LOGNAME_OFFSET].items->data;
    code = afs_syscall(AFSCALL_KRPCDBG, KRPC_DBG_INIT,logname, NULL,
                       NULL, NULL);
    if (code){
        perror("can't initialize krpc logging\n");
        return;
    }
    if ( syndesc->parms[ON_LEVEL_OFFSET].items){
        debug_level =  syndesc->parms[ON_LEVEL_OFFSET].items->data;
        code = afs_syscall(AFSCALL_KRPCDBG, KRPC_DBG_SET, debug_level,
                           NULL, NULL, NULL);
        if (code)
           perror("can't set requested debug level \n"); 
    }
}

/* make sure this corresponds to the value in rpcdbg.h
 * (rpcdbg.h isn't exported)
 */
char dbg_off_switch[]="0-39.0";

krpc_dbg_off(syndesc, a)
    struct cmd_syndesc *syndesc;
    char *a;
{
    int code;

    code = afs_syscall(AFSCALL_KRPCDBG, KRPC_DBG_SET, dbg_off_switch);
}

main(argc, argv)

int argc;
char **argv;

{
   register struct cmd_syndesc *on, *off, *set;

   osi_setlocale(LC_ALL, "");

   on = (struct cmd_syndesc *)cmd_CreateSyntax("on", krpc_dbg_on, 0, 
                        "initialize and turn on krpc logging");
   (void)cmd_IndexedAddParm(on, "-logname", CMD_SINGLE, CMD_REQUIRED,
                        "logname", ON_LOGNAME_OFFSET);
   (void)cmd_IndexedAddParm(on, "-level", CMD_SINGLE, CMD_OPTIONAL,
                            "rpc_debug_level", ON_LEVEL_OFFSET);
   
   set = (struct cmd_syndesc *)cmd_CreateSyntax("set", krpc_dbg_set, 0, 
                        "set debug levels for krpc");
   (void)cmd_IndexedAddParm(set, "-level",  CMD_SINGLE, CMD_REQUIRED,
                            "rpc_debug_level", SET_LEVEL_OFFSET);
   off = (struct cmd_syndesc *)cmd_CreateSyntax("off", krpc_dbg_off, 0, 
                        "turn off logging for krpc");
   return (cmd_Dispatch(argc, argv));
         
}





