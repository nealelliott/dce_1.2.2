/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/* Copyright (C) 1996, 1995 Transarc Corporation - All rights reserved */
/*
 * HISTORY
 * $Log: dfsgw_userint.c,v $
 * Revision 1.1.6.1  1996/10/02  17:49:53  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:18  damon]
 *
 * Revision /main/HPDCE02/1  1995/02/14  22:42 UTC  maunsell_c
 * 	@sys @host changes for nfs/dfs gateway
 * 
 * Revision /main/maunsell_mothra1.45/1  1995/01/31  19:21 UTC  maunsell_c
 * 	add @sys @host support for dfs_login/dfsgwd/dfsgw
 * 
 * Revision 1.1.2.1  1994/07/26  19:13:05  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:57:45  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:01:51  mckeen]
 * 
 * $EndLog$
 */
/*
 * dfsgw_userint.c
*/
/*
 * uses dfs cmd parsing to setup user input data
 * for dfsgw commands
 * 
 */
#include <stdio.h>
#include <dcedfs/osi.h>
#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>
#include <dfsgw_userint.h>

#include <dcedfs/stds.h>
RCSID("$Header: /u0/rcs_trees/dce/rcs/file/gateway/dfsgw/dfsgw_userint.c,v 1.1.6.1 1996/10/02 17:49:53 damon Exp $")

/*
 * dfsgw add  -id <networkID>:<userID> [-dceid <login_name>[:<password>]] [-sysname <name>] [-remotehost <name>] [-af <address_family>]
 */
void
SetUpAdd()
{
    struct cmd_syndesc *dfsgwAddSyntax;

    dfsgwAddSyntax = cmd_CreateSyntax("add", dfsgw_AddCmd, (char *)NULL,
                        "add an entry to the AT");
    cmd_IndexedAddParm(dfsgwAddSyntax, "-id", CMD_SINGLE, CMD_REQUIRED,
                        "networkID:userID", NET_ID_OFFSET);
    cmd_IndexedAddParm(dfsgwAddSyntax, "-dceid", CMD_SINGLE, CMD_OPTIONAL,
                        "login_name[:<password>]", DCE_ID_OFFSET);
    cmd_IndexedAddParm(dfsgwAddSyntax, "-sysname", CMD_SINGLE, CMD_OPTIONAL,
                        "name", SYSNAME_OFFSET);
    cmd_IndexedAddParm(dfsgwAddSyntax, "-remotehost", CMD_SINGLE, CMD_OPTIONAL,
                        "name", REMOTEHOST_OFFSET); 
    cmd_IndexedAddParm(dfsgwAddSyntax, "-af", CMD_SINGLE, CMD_OPTIONAL,
                        "address_family", ADDR_FAMILY_OFFSET);
}


/*
 * dfsgw delete -id <networkID>:<userID> [-af <address_family>] 
 */
void
SetUpDelete()
{
    struct cmd_syndesc *dfsgwDeleteSyntax;

    dfsgwDeleteSyntax = cmd_CreateSyntax("delete", dfsgw_DeleteCmd, (char *)NULL,
                           "delete an entry from the AT");
    cmd_IndexedAddParm(dfsgwDeleteSyntax, "-id", CMD_SINGLE, CMD_REQUIRED,
                        "networkID:userID", NET_ID_OFFSET);
    cmd_IndexedAddParm(dfsgwDeleteSyntax, "-af", CMD_SINGLE, CMD_OPTIONAL,
                        "address_family", ADDR_FAMILY_OFFSET);
}

/*
 * dfsgw query -id <networkID>:<userID> [-af <address_family>]
 */
void
SetUpQuery()
{
    struct cmd_syndesc *dfsgwQuerySyntax;

    dfsgwQuerySyntax = cmd_CreateSyntax("query", dfsgw_QueryCmd, (char *)NULL,
                           "query the AT for relationship existence");
    cmd_IndexedAddParm(dfsgwQuerySyntax, "-id", CMD_SINGLE, CMD_REQUIRED,
                        "networkID:userID", NET_ID_OFFSET);
    cmd_IndexedAddParm(dfsgwQuerySyntax, "-af", CMD_SINGLE, CMD_OPTIONAL,
                        "address_family", ADDR_FAMILY_OFFSET);

}
/*
 * dfsgw list 
 */
void
SetUpList()
{
     struct cmd_syndesc *dfsgwListSyntax;

     dfsgwListSyntax = cmd_CreateSyntax("list", dfsgw_ListCmd, (char *)NULL,
                          "list all entries in the AT");
}

void
SetUpSyntax()
{
    SetUpAdd();
    SetUpDelete();
    SetUpList();
    SetUpQuery();
}

