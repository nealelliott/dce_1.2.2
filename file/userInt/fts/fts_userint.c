/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: fts_userint.c,v $
 * $EndLog$
 */
/*
*/
/*
 * fts_userint.c -- user interface routines for fts
 *
 *
 *	Copyright (C) 1996, 1991, 1990 Transarc Corporation
 *	All rights reserved.
 */

#include <stdio.h>

#include <dcedfs/param.h>
#include <dcedfs/stds.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsftimac.h>
#include <dfsftisvc.h>
#include <dfsftimsg.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/userInt/fts/RCS/fts_userint.c,v 4.54 1996/10/31 19:02:34 madhuc Exp $")

#include <dcedfs/cmdAux.h>
#include <fts_userint.h>

static  error_status_t             st;
static  unsigned char  		   *cmd_explanation;

static void fts_ui_SetUpFilesetSwitch(aSyntax, type, flags, index)
struct cmd_syndesc *	aSyntax;
int			type;
unsigned long		flags;
int			index;
{
    (void)cmd_Seek(aSyntax, index);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_name, &st);
    (void)cmd_AddParm(aSyntax, "-fileset", type, flags, (char *) cmd_explanation);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_fileset_id, &st);
    (void)cmd_AddParamHelp(aSyntax, "-fileset", (char *) cmd_explanation, 0);
    free(cmd_explanation);
}

static void fts_ui_SetUpServerAndAggregate(aSyntax, flags, index)
struct cmd_syndesc *	aSyntax;
unsigned long		flags;
int			index;
{
    (void)cmd_Seek(aSyntax, index);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_server, &st);
    (void)cmd_AddParm(aSyntax, "-server", CMD_SINGLE, flags, (char *) cmd_explanation);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_aggregate, &st);
   (void)cmd_AddParm(aSyntax, "-aggregate", CMD_SINGLE, flags, (char *) cmd_explanation);
    free(cmd_explanation);
}

static void fts_ui_SetUpNormalDefaults(aSyntax, includeFileset)
struct cmd_syndesc *	aSyntax;
int			includeFileset;
{
    if (includeFileset) {
	fts_ui_SetUpFilesetSwitch(aSyntax, CMD_SINGLE, CMD_REQUIRED,
				  FTS_NORMAL_FILESET_OFFSET);
    }

    (void)cmd_Seek(aSyntax, FTS_COMMON_OFFSET);


    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_cell, &st);
    (void)cmd_AddParm(aSyntax, "-cell", CMD_SINGLE, CMD_OPTIONAL, (char *) cmd_explanation);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_noauth, &st);
    (void)cmd_AddParm(aSyntax, "-noauth", CMD_FLAG, CMD_OPTIONAL, (char *) cmd_explanation);
    free(cmd_explanation);

    (void)cmd_AddParm(aSyntax, "-localauth", CMD_FLAG, CMD_OPTIONAL, "");
    (void)cmd_MakeAlternate(aSyntax, "-noauth", "-localauth");
    (void)cmd_AddParm(aSyntax, "-verbose", CMD_FLAG, CMD_OPTIONAL, "");
}

static void fts_ui_SetUpAddSite()
{
    register struct cmd_syndesc * addSiteSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_addsite, &st);
    addSiteSyntax = cmd_CreateSyntax("addsite", fts_DoAddSite, 0, (char *) cmd_explanation);
    free(cmd_explanation);

    fts_ui_SetUpFilesetSwitch(addSiteSyntax, CMD_SINGLE, CMD_REQUIRED,
			       ADDSITE_FILESET_OFFSET);
    fts_ui_SetUpServerAndAggregate(addSiteSyntax, CMD_REQUIRED, ADDSITE_SERVER_OFFSET);

    /* target lazy-replication latency for replica, in seconds */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_addsitesyntax, &st);
    cmd_IndexedAddParm(addSiteSyntax, "-maxsiteage", CMD_SINGLE, CMD_OPTIONAL,
	 (char *) cmd_explanation, ADDSITE_MAXREPL_OFFSET);
    free(cmd_explanation);

    fts_ui_SetUpNormalDefaults(addSiteSyntax, 0);
}

static void fts_ui_SetUpAggrInfo()
{
    register struct cmd_syndesc * aggrInfoSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_aggrinfo, &st);
    aggrInfoSyntax = cmd_CreateSyntax("aggrinfo", fts_DoAggregateInfo, 0,
	  (char *) cmd_explanation );
    free(cmd_explanation);


    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_server, &st);
    cmd_IndexedAddParm(aggrInfoSyntax, "-server", CMD_SINGLE, CMD_REQUIRED,
  	  (char *) cmd_explanation, AGGRINFO_SERVER_OFFSET);
    free(cmd_explanation);


    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_ag_name, &st);
    cmd_IndexedAddParm(aggrInfoSyntax, "-aggregate", CMD_SINGLE, CMD_OPTIONAL,
  	(char *) cmd_explanation, AGGRINFO_AGGREGATE_OFFSET);
    free(cmd_explanation);

    fts_ui_SetUpNormalDefaults(aggrInfoSyntax, 0);
}

static void fts_ui_SetUpClone()
{
    register struct cmd_syndesc * cloneSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_clone, &st);
    cloneSyntax = cmd_CreateSyntax("clone", fts_DoBackupVolume, 0,
	 (char *) cmd_explanation );
    free(cmd_explanation);

    fts_ui_SetUpNormalDefaults(cloneSyntax, 1);
}

static void fts_ui_SetUpCloneSys()
{
    register struct cmd_syndesc * cloneSysSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_clonesyntax, &st);
    cloneSysSyntax = cmd_CreateSyntax("clonesys", fts_DoBackSys, (char *)NULL,
  	(char *) cmd_explanation );
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_prefix, &st);
    (void)cmd_IndexedAddParm(cloneSysSyntax, "-prefix", CMD_SINGLE, CMD_OPTIONAL,
  	(char *) cmd_explanation, CLONESYS_PREFIX_OFFSET);
    free(cmd_explanation);

    fts_ui_SetUpServerAndAggregate(cloneSysSyntax, CMD_OPTIONAL, CLONESYS_SERVER_OFFSET);

    fts_ui_SetUpNormalDefaults(cloneSysSyntax, 0);
}

static void fts_ui_SetUpCreate()
{
    register struct cmd_syndesc * createSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_create_fileset, &st);
    createSyntax = cmd_CreateSyntax("create", fts_DoCreateVolume, 0, 
  	  (char *) cmd_explanation);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_ftname, &st);
    (void)cmd_IndexedAddParm(createSyntax, "-ftname", CMD_SINGLE, CMD_REQUIRED,
  	(char *) cmd_explanation, CREATE_FSNAME_OFFSET);
    free(cmd_explanation);

    fts_ui_SetUpServerAndAggregate(createSyntax, CMD_REQUIRED, CREATE_SERVER_OFFSET);
    fts_ui_SetUpNormalDefaults(createSyntax, 0);
}

static void fts_ui_SetUpCrFLDBEntry()
{
    register struct cmd_syndesc * crFLDBEntrySyntax;


    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_crfldbentry, &st);
    crFLDBEntrySyntax = cmd_CreateSyntax("crfldbentry", fts_DoCrFLDBEntry, 0,
	  (char *) cmd_explanation );
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_crftname, &st);
    (void)cmd_IndexedAddParm(crFLDBEntrySyntax, "-ftname", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation, CRFLDB_FTNAME_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_server, &st);
    (void)cmd_IndexedAddParm(crFLDBEntrySyntax, "-server", CMD_SINGLE, CMD_REQUIRED,
  	(char *) cmd_explanation, CRFLDB_SERVER_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_cr_aggrid, &st);
    (void)cmd_IndexedAddParm(crFLDBEntrySyntax, "-aggrid", CMD_SINGLE, CMD_REQUIRED,
    (char *) cmd_explanation, CRFLDB_AGGRID_OFFSET);
    free(cmd_explanation);

    fts_ui_SetUpNormalDefaults(crFLDBEntrySyntax, 0);
}

static void fts_ui_SetUpCrMount()
{
    struct cmd_syndesc *	crMountSyntax;


    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_crmount, &st);
	    crMountSyntax = cmd_CreateSyntax("crmount", fts_DoMkMount, (char *)NULL,
    (char *) cmd_explanation );
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_cr_dir, &st);
    (void)cmd_IndexedAddParm(crMountSyntax, "-dir", CMD_SINGLE, CMD_REQUIRED,
	    (char *) cmd_explanation , CRMOUNT_DIR_OFFSET);
    free(cmd_explanation);

    fts_ui_SetUpFilesetSwitch(crMountSyntax, CMD_SINGLE, CMD_REQUIRED, 
			       CRMOUNT_FILESET_OFFSET);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_cr_global, &st);
    (void)cmd_IndexedAddParm(crMountSyntax, "-global", CMD_FLAG, CMD_REQUIRED, 
	    (char *) cmd_explanation, CRMOUNT_GLOBAL_OFFSET);
    free(cmd_explanation);

    (void)cmd_MakeAlternate(crMountSyntax, "-fileset", "-global");
#if 0
    /* Pull this option until the CM can use cellular mount points correctly. */

  cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_cr_cell, &st);
    (void)cmd_IndexedAddParm(crMountSyntax, "-cell", CMD_SINGLE, CMD_OPTIONAL,
  (char *) cmd_explanation, CRMOUNT_CELL_OFFSET);
  free(cmd_explanation);

#endif /* 0 */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_cr_rw, &st);
    (void)cmd_IndexedAddParm(crMountSyntax, "-rw", CMD_FLAG, CMD_OPTIONAL, 
	    (char *) cmd_explanation, CRMOUNT_RW_OFFSET);
    free(cmd_explanation);


    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_cr_fast, &st);
    (void)cmd_IndexedAddParm(crMountSyntax, "-fast", CMD_FLAG, CMD_OPTIONAL, 
  	  (char *) cmd_explanation, CRMOUNT_FAST_OFFSET);
    free(cmd_explanation);

}

static void fts_ui_SetUpCrServer()
{
    register struct cmd_syndesc * crServerSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_crserverentry, &st);
    crServerSyntax = cmd_CreateSyntax("crserverentry", fts_DoCrServer, 0,
	  (char *) cmd_explanation );
    free(cmd_explanation);

    /* first address to be added */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_server, &st);
    cmd_IndexedAddParm(crServerSyntax, "-server", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation , NEWSRV_ADDRESS_OFFSET);
    free(cmd_explanation);

    /* principal for that server */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_upprincipal, &st);
    cmd_IndexedAddParm(crServerSyntax, "-principal", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation , NEWSRV_PRINCIPAL_OFFSET);
    free(cmd_explanation);

    /* entry-creation quota for that server */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_upquota, &st);
    cmd_IndexedAddParm(crServerSyntax, "-quota", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation , NEWSRV_QUOTA_OFFSET);
    free(cmd_explanation);

    /* owner for that server */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_up_owner, &st);
    cmd_IndexedAddParm(crServerSyntax, "-owner", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation , NEWSRV_OWNER_OFFSET);
    free(cmd_explanation);

#if 0
    /* obj-id for that server */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_up_fxdid, &st);
    cmd_IndexedAddParm(crServerSyntax, "-fxdid", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation , NEWSRV_OBJID_OFFSET);
    free(cmd_explanation);

#endif /* 0 */

    fts_ui_SetUpNormalDefaults(crServerSyntax, 0);
}

static void fts_ui_SetUpDelServer()
{
  register struct cmd_syndesc * delServerSyntax;

  cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_delserverentry, &st);
  delServerSyntax = cmd_CreateSyntax("delserverentry", fts_DoDelServer, 0,
	  (char *) cmd_explanation );
  free(cmd_explanation);


  cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_server, &st);
  cmd_IndexedAddParm(delServerSyntax, "-server", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation , DELSRV_ADDRESS_OFFSET);
  free(cmd_explanation);

  fts_ui_SetUpNormalDefaults(delServerSyntax, 0);
}

static void fts_ui_SetUpDelete()
{
    register struct cmd_syndesc * deleteSyntax;


    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_delete_fileset, &st);
    deleteSyntax = cmd_CreateSyntax("delete", fts_DoDeleteVolume, 0, 
	  (char *) cmd_explanation);
    free(cmd_explanation);


    fts_ui_SetUpFilesetSwitch(deleteSyntax, CMD_SINGLE, CMD_REQUIRED, DELETE_FILESET_OFFSET);
    fts_ui_SetUpServerAndAggregate(deleteSyntax, CMD_REQUIRED, DELETE_SERVER_OFFSET);
    fts_ui_SetUpNormalDefaults(deleteSyntax, 0);
}

static void fts_ui_SetUpDelEntry()
{
    register struct cmd_syndesc * delFldbSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_delfldbentry, &st);
    delFldbSyntax = cmd_CreateSyntax("delfldbentry", fts_DoDeleteEntry, 0,
	  (char *) cmd_explanation );
    free(cmd_explanation);


    fts_ui_SetUpFilesetSwitch(delFldbSyntax, CMD_SINGLE, CMD_REQUIRED,
			       DELFLDB_FILESET_OFFSET);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_prefix, &st);
    (void)cmd_IndexedAddParm(delFldbSyntax, "-prefix", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation, DELFLDB_PREFIX_OFFSET);
    free(cmd_explanation);

    (void)cmd_MakeAlternate(delFldbSyntax, "-fileset", "-prefix");
    fts_ui_SetUpServerAndAggregate(delFldbSyntax, CMD_OPTIONAL, DELFLDB_SERVER_OFFSET);
    fts_ui_SetUpNormalDefaults(delFldbSyntax, 0);
}

static void fts_ui_SetUpDelMount()
{
    struct cmd_syndesc *	delMountSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_delmount, &st);
    delMountSyntax = cmd_CreateSyntax("delmount", fts_DoDelMount, (char *)NULL,
	  (char *) cmd_explanation );
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_delmount_dir, &st);
    (void)cmd_IndexedAddParm(delMountSyntax, "-dir", CMD_LIST, CMD_REQUIRED,
	  (char *) cmd_explanation , RMMOUNT_DIR_OFFSET);
    free(cmd_explanation);
}

static void fts_ui_SetUpDump()
{
    register struct cmd_syndesc * dumpSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_setupdump, &st);
    dumpSyntax = cmd_CreateSyntax("dump", fts_DoDumpVolume, (char *)NULL, 
	  (char *) cmd_explanation);
    free(cmd_explanation);

    fts_ui_SetUpFilesetSwitch(dumpSyntax, CMD_SINGLE, CMD_REQUIRED, DUMP_FILESET_OFFSET);

    (void)cmd_IndexedAddParm(dumpSyntax, "-time", CMD_SINGLE, CMD_REQUIRED,
			      "date",
			      DUMP_TIME_OFFSET);
    (void)cmd_AddParamHelp(dumpSyntax, "-time", "0", CMD_PARM_LITERAL);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_dump_number, &st);
    (void)cmd_IndexedAddParm(dumpSyntax, "-version", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation, DUMP_VERSION_OFFSET);
    free(cmd_explanation);

    (void)cmd_MakeAlternate(dumpSyntax, "-time", "-version");

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_server, &st);
    (void)cmd_IndexedAddParm(dumpSyntax, "-server", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation, DUMP_SERVER_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_filename, &st);
    (void)cmd_IndexedAddParm(dumpSyntax, "-file", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation, DUMP_FILE_OFFSET);
    free(cmd_explanation);

#if !defined(OSF_NO_SOCKET_DUMP)
    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_sockflag, &st);
    (void)cmd_IndexedAddParm(dumpSyntax, "-socket", CMD_FLAG, CMD_OPTIONAL, 
	  (char *) cmd_explanation, DUMP_SOCKFLAG_OFFSET);
    free(cmd_explanation);
#endif /* OSF_NO_SOCKET_DUMP */

    fts_ui_SetUpNormalDefaults(dumpSyntax, 0);
}

static void fts_ui_SetUpEdServer()
{
    register struct cmd_syndesc * edServerSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_edserverentry, &st);
    edServerSyntax = cmd_CreateSyntax("edserverentry", fts_DoAlterServer, 0,
	  (char *) cmd_explanation );
    free(cmd_explanation);

    /* address of server to be altered */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_server, &st);
    cmd_IndexedAddParm(edServerSyntax, "-server", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation , ALTSRV_ADDRESS_OFFSET);
    free(cmd_explanation);

    /* flag to delete that address */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_rmaddr, &st);
    cmd_IndexedAddParm(edServerSyntax, "-rmaddr", CMD_FLAG, CMD_OPTIONAL,
	  (char *) cmd_explanation , ALTSRV_DELADDR_OFFSET);
    free(cmd_explanation);

    /* address to be added to server */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_addaddr, &st);
    cmd_IndexedAddParm(edServerSyntax, "-addaddr", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation , ALTSRV_ADDADDR_OFFSET);
    free(cmd_explanation);

    /* address to change that to */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_changeaddr, &st);
    cmd_IndexedAddParm(edServerSyntax, "-changeaddr", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation , ALTSRV_CHANGEADDR_OFFSET);
    free(cmd_explanation);

    (void)cmd_MakeAlternate(edServerSyntax, "-rmaddr", "-addaddr");
    (void)cmd_MakeAlternate(edServerSyntax, "-addaddr", "-changeaddr");
    /* set principal */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_principal, &st);
    cmd_IndexedAddParm(edServerSyntax, "-principal", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation , ALTSRV_SETPRINCIPAL_OFFSET);
    free(cmd_explanation);

    /* set quota */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_quota, &st);
    cmd_IndexedAddParm(edServerSyntax, "-quota", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation , ALTSRV_SETQUOTA_OFFSET);
    free(cmd_explanation);

    /* set owner */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_owner_group, &st);
    cmd_IndexedAddParm(edServerSyntax, "-owner", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation , ALTSRV_SETOWNER_OFFSET);
    free(cmd_explanation);

    /* set no owner */
    cmd_IndexedAddParm(edServerSyntax, "-noowner", CMD_FLAG, CMD_OPTIONAL,
			"", ALTSRV_NOOWNER_OFFSET);
#if 0
    /* set objid */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_fxuid, &st);
    cmd_IndexedAddParm(edServerSyntax, "-fxdid", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation , ALTSRV_SETOBJID_OFFSET);
    free(cmd_explanation);

#endif /* 0 */

    (void)cmd_MakeAlternate(edServerSyntax, "-owner", "-noowner");
    fts_ui_SetUpNormalDefaults(edServerSyntax, 0);
}

static void fts_ui_SetUpGetRepServerStatus()
{
    register struct cmd_syndesc * getRepServerStatusSyntax;


    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_statrepserver, &st);
    getRepServerStatusSyntax = cmd_CreateSyntax("statrepserver",
						 fts_DoGetRepServerStatus, 
	   					 0,
                                                 (char *) cmd_explanation );
    free(cmd_explanation);

    /* which server to contact */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_server, &st);
    cmd_IndexedAddParm(getRepServerStatusSyntax, "-server", CMD_SINGLE,
				CMD_REQUIRED,
    (char *) cmd_explanation , GETRSSTAT_SERVER_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_long, &st);
    (void)cmd_IndexedAddParm(getRepServerStatusSyntax, "-long", CMD_FLAG, 
	     CMD_OPTIONAL, (char *) cmd_explanation, GETRSSTAT_LONG_OFFSET);
    free(cmd_explanation);

    fts_ui_SetUpNormalDefaults(getRepServerStatusSyntax, 0);
}

static void fts_ui_SetUpGetRepStatus()
{
    register struct cmd_syndesc * getRepStatusSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_lsreplicas, &st);
    getRepStatusSyntax = cmd_CreateSyntax("lsreplicas", fts_DoGetRepStatus, 0,
	  (char *) cmd_explanation );
    free(cmd_explanation);

    fts_ui_SetUpFilesetSwitch(getRepStatusSyntax, CMD_SINGLE, CMD_REQUIRED,
				  GETRSTAT_FILESET_OFFSET);

    /* flag if want status from all servers */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_all_replicas, &st);
    cmd_IndexedAddParm(getRepStatusSyntax, "-all", CMD_FLAG, CMD_REQUIRED,
	  (char *) cmd_explanation , GETRSTAT_ALLFLAG_OFFSET);
    free(cmd_explanation);

    /* which server/aggr to contact, or all servers (via flag) */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_server, &st);
    (void)cmd_IndexedAddParm(getRepStatusSyntax, "-server", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation , GETRSTAT_SERVER_OFFSET);
    free(cmd_explanation);

    (void)cmd_MakeAlternate(getRepStatusSyntax, "-all", "-server");
    fts_ui_SetUpNormalDefaults(getRepStatusSyntax, 0);
}

static void fts_ui_SetUpLockEntry()
{
    register struct cmd_syndesc * lockSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_lock_fileset, &st);
    lockSyntax = cmd_CreateSyntax("lock", fts_DoLockEntry, 0,
	  (char *) cmd_explanation );
    free(cmd_explanation);

    fts_ui_SetUpNormalDefaults(lockSyntax, 1);
}

static void fts_ui_SetUpLsAggr()
{
    register struct cmd_syndesc * lsAggrSyntax;


    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_list_aggr, &st);
    lsAggrSyntax = cmd_CreateSyntax("lsaggr", fts_DoListAggregates, 0,
	  (char *) cmd_explanation );
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_server, &st);
    (void)cmd_IndexedAddParm(lsAggrSyntax, "-server", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation, LSAGGR_SERVER_OFFSET);
    free(cmd_explanation);

    fts_ui_SetUpNormalDefaults(lsAggrSyntax, 0);
}

static void fts_ui_SetUpLsFldb()
{
    register struct cmd_syndesc * lsFldbSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_lsfldb, &st);
    lsFldbSyntax = cmd_CreateSyntax("lsfldb", fts_DoListFLDB, 0, 
	  (char *) cmd_explanation);
    free(cmd_explanation);

    fts_ui_SetUpFilesetSwitch(lsFldbSyntax, CMD_SINGLE, CMD_OPTIONAL, LSFLDB_FILESET_OFFSET);
    fts_ui_SetUpServerAndAggregate(lsFldbSyntax, CMD_OPTIONAL, LSFLDB_SERVER_OFFSET);
    (void)cmd_IndexedAddParm(lsFldbSyntax, "-locked", CMD_FLAG, CMD_OPTIONAL, "",
			      LSFLDB_LOCKED_OFFSET);
    fts_ui_SetUpNormalDefaults(lsFldbSyntax, 0);
}

static void fts_ui_SetUpLSFT()
{   
    register struct cmd_syndesc * lsftSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_lsft, &st);
    lsftSyntax = cmd_CreateSyntax("lsft", fts_DoLSFT, 0,
	  (char *) cmd_explanation );
    free(cmd_explanation);

    (void)cmd_Seek(lsftSyntax, EXAMINE_PATH_OFFSET);
    SetUpPathSwitch(lsftSyntax, CMD_SINGLE, CMD_OPTIONAL);
    fts_ui_SetUpFilesetSwitch(lsftSyntax, CMD_SINGLE, CMD_OPTIONAL,
			       EXAMINE_FILESET_OFFSET);
    (void)cmd_MakeAlternate(lsftSyntax, "-path", "-fileset");

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_server, &st);
    (void)cmd_IndexedAddParm(lsftSyntax, "-server", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation, EXAMINE_SERVER_OFFSET);
    free(cmd_explanation);

    fts_ui_SetUpNormalDefaults(lsftSyntax, 0);
}

static void fts_ui_SetUpLsHeader()
{
    register struct cmd_syndesc * lsHeaderSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_lsheader, &st);
    lsHeaderSyntax = cmd_CreateSyntax("lsheader", fts_DoListVolumes, 0,
	  (char *) cmd_explanation );
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_server, &st);
    (void)cmd_IndexedAddParm(lsHeaderSyntax, "-server", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation, LSFS_SERVER_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_aggregate, &st);
    (void)cmd_IndexedAddParm(lsHeaderSyntax, "-aggregate", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation, LSFS_AGGREGATE_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_fast_list, &st);
    (void)cmd_IndexedAddParm(lsHeaderSyntax, "-fast", CMD_FLAG, CMD_OPTIONAL,
	  (char *) cmd_explanation, LSFS_FAST_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_long_list, &st);
    (void)cmd_IndexedAddParm(lsHeaderSyntax, "-long", CMD_FLAG, CMD_OPTIONAL,
	  (char *) cmd_explanation, LSFS_LONG_OFFSET);
    free(cmd_explanation);

    (void)cmd_MakeAlternate(lsHeaderSyntax, "-fast", "-long");
    fts_ui_SetUpNormalDefaults(lsHeaderSyntax, 0);
}

static void fts_ui_SetUpLsMount()
{
    struct cmd_syndesc *	lsMountSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_lsmount, &st);
    lsMountSyntax = cmd_CreateSyntax("lsmount", fts_DoLsMount, (char *)NULL,
	  (char *) cmd_explanation );
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_dir_name, &st);
    (void)cmd_IndexedAddParm(lsMountSyntax, "-dir", CMD_LIST, CMD_REQUIRED,
	  (char *) cmd_explanation, LSMOUNT_DIR_OFFSET);
    free(cmd_explanation);
}

static void fts_ui_SetUpLsQuota()
{
    struct cmd_syndesc *	lsQuotaSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_lsquota, &st);
    lsQuotaSyntax = cmd_CreateSyntax("lsquota", fts_DoLsQuota, (char *)NULL,
	  (char *) cmd_explanation );
    free(cmd_explanation);

    (void)cmd_Seek(lsQuotaSyntax, LSQUOTA_PATH_OFFSET);
    SetUpPathSwitch(lsQuotaSyntax, CMD_LIST, CMD_OPTIONAL);
    fts_ui_SetUpFilesetSwitch(lsQuotaSyntax, CMD_LIST, CMD_OPTIONAL,
			       LSQUOTA_FILESET_OFFSET);
    (void)cmd_MakeAlternate(lsQuotaSyntax, "-path", "-fileset");
    fts_ui_SetUpNormalDefaults(lsQuotaSyntax, 0);
}

static void fts_ui_SetUpLsServer()
{
    register struct cmd_syndesc * lsServerSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_lsserverentry, &st);
    lsServerSyntax = cmd_CreateSyntax("lsserverentry", fts_DoLsServer, 0,
	  (char *) cmd_explanation );
    free(cmd_explanation);

    /* any one address for the server */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_server, &st);
    cmd_IndexedAddParm(lsServerSyntax, "-server", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation , SHOWSRV_ADDRESS_OFFSET);
    free(cmd_explanation);

    /* flag to show all servers */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_show_all_servers, &st);
    cmd_IndexedAddParm(lsServerSyntax, "-all", CMD_FLAG, CMD_REQUIRED,
	  (char *) cmd_explanation , SHOWSRV_ALL_OFFSET);
    free(cmd_explanation);

    (void)cmd_MakeAlternate(lsServerSyntax, "-server", "-all");
    fts_ui_SetUpNormalDefaults(lsServerSyntax, 0);
}

static void fts_ui_SetUpMove()
{
    register struct cmd_syndesc * moveSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_move_fileset, &st);
    moveSyntax = cmd_CreateSyntax("move", fts_DoMoveVolume, 0, 
	  (char *) cmd_explanation);
    free(cmd_explanation);

    fts_ui_SetUpFilesetSwitch(moveSyntax, CMD_SINGLE, CMD_REQUIRED, MOVE_FILESET_OFFSET);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_fromserver, &st);
    (void)cmd_IndexedAddParm(moveSyntax, "-fromserver", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation , MOVE_FROMSERVER_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_fromaggregate, &st);
    (void)cmd_IndexedAddParm(moveSyntax, "-fromaggregate", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation , MOVE_FROMAGGREGATE_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_toserver, &st);
    (void)cmd_IndexedAddParm(moveSyntax, "-toserver", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation , MOVE_TOSERVER_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_toaggregate, &st);
    (void)cmd_IndexedAddParm(moveSyntax, "-toaggregate", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation , MOVE_TOAGGREGATE_OFFSET);
    free(cmd_explanation);

#if !defined(OSF_NO_SOCKET_DUMP)
    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_sockflag, &st);
    (void)cmd_IndexedAddParm(moveSyntax, "-socket", CMD_FLAG, CMD_OPTIONAL, 
  	  (char *) cmd_explanation, MOVE_SOCKFLAG_OFFSET);
    free(cmd_explanation);
#endif /* OSF_NO_SOCKET_DUMP */

    fts_ui_SetUpNormalDefaults(moveSyntax, 0);
}

static void fts_ui_SetUpRelease()
{
    register struct cmd_syndesc * releaseSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_release, &st);
    releaseSyntax = cmd_CreateSyntax("release", fts_DoReleaseVolume, 0, 
	  (char *) cmd_explanation);
    free(cmd_explanation);

    fts_ui_SetUpFilesetSwitch(releaseSyntax, CMD_SINGLE, CMD_REQUIRED,
			       RELEASE_FILESET_OFFSET);
    (void)cmd_IndexedAddParm(releaseSyntax, "-wait", CMD_FLAG,
			     CMD_OPTIONAL, "", RELEASE_WAIT_OFFSET);
    fts_ui_SetUpNormalDefaults(releaseSyntax, 0);
}

static void fts_ui_SetUpRename()
{
    register struct cmd_syndesc * renameSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_rename, &st);
    renameSyntax = cmd_CreateSyntax("rename", fts_DoRenameVolume, 0, 
	  (char *) cmd_explanation);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_oldname, &st);
    (void)cmd_IndexedAddParm(renameSyntax, "-oldname", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation, RENAME_OLDNAME_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_newname, &st);
    (void)cmd_IndexedAddParm(renameSyntax, "-newname", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation, RENAME_NEWNAME_OFFSET);
    free(cmd_explanation);

    fts_ui_SetUpNormalDefaults(renameSyntax, 0);
}

#if 0
static void fts_ui_SetUpReserveIDs()
{
    register struct cmd_syndesc * reserveIDsSyntax;

    reserveIDsSyntax = cmd_CreateSyntax("reserveids", fts_DoReserveIDs, 0,
					  "reserve some fileset IDs");

    /* number to reserve */
    cmd_IndexedAddParm(reserveIDsSyntax, "-count", CMD_SINGLE, CMD_OPTIONAL,
			"number", RSVIDS_COUNT_OFFSET);
    /* server to refer to */
    cmd_IndexedAddParm(reserveIDsSyntax, "-server", CMD_SINGLE, CMD_OPTIONAL,
			"machine", RSVIDS_SERVER_OFFSET);

    fts_ui_SetUpNormalDefaults(reserveIDsSyntax, 0);
}
#endif /* 0 */

static void fts_ui_SetUpRestore()
{
    register struct cmd_syndesc * restoreSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_restore, &st);
    restoreSyntax = cmd_CreateSyntax("restore", fts_DoRestoreVolume, 0, 
	  (char *) cmd_explanation);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_ftname, &st);
    (void)cmd_IndexedAddParm(restoreSyntax, "-ftname", CMD_SINGLE, CMD_REQUIRED, 
	  (char *) cmd_explanation, RESTORE_FSNAME_OFFSET);
    free(cmd_explanation);

    fts_ui_SetUpServerAndAggregate(restoreSyntax, CMD_REQUIRED, RESTORE_SERVER_OFFSET);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_file, &st);
    (void)cmd_IndexedAddParm(restoreSyntax, "-file", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation, RESTORE_FILE_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_ftid, &st);
    (void)cmd_IndexedAddParm(restoreSyntax, "-ftid", CMD_SINGLE, CMD_OPTIONAL, 
	  (char *) cmd_explanation, RESTORE_FSID_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_overwrite, &st);
    (void)cmd_IndexedAddParm(restoreSyntax, "-overwrite", CMD_FLAG, CMD_OPTIONAL,
	  (char *) cmd_explanation, RESTORE_OVERWRITE_OFFSET);
    free(cmd_explanation);

#if !defined(OSF_NO_SOCKET_DUMP)
    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_sockflag, &st);
    (void)cmd_IndexedAddParm(restoreSyntax, "-socket", CMD_FLAG, CMD_OPTIONAL, 
	  (char *) cmd_explanation, RESTORE_SOCKFLAG_OFFSET);
    free(cmd_explanation);
#endif /* OSF_NO_SOCKET_DUMP */

#if 0

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_disjoint, &st);
    (void)cmd_IndexedAddParm(restoreSyntax, "-disjoint", CMD_FLAG, CMD_OPTIONAL,
	  (char *) cmd_explanation, RESTORE_DISJOINT_OFFSET);
    free(cmd_explanation);

#endif /* 0 */
    fts_ui_SetUpNormalDefaults(restoreSyntax, 0);
}

static void fts_ui_SetUpRmSite()
{
    register struct cmd_syndesc * rmSiteSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_rmsite, &st);
    rmSiteSyntax = cmd_CreateSyntax("rmsite", fts_DoRemoveSite, 0,
	  (char *) cmd_explanation );
    free(cmd_explanation);

    fts_ui_SetUpFilesetSwitch(rmSiteSyntax, CMD_SINGLE, CMD_REQUIRED, RMSITE_FILESET_OFFSET);
    fts_ui_SetUpServerAndAggregate(rmSiteSyntax, CMD_REQUIRED, RMSITE_SERVER_OFFSET);
    fts_ui_SetUpNormalDefaults(rmSiteSyntax, 0);
}

static void fts_ui_SetUpSetQuota()
{
    struct cmd_syndesc *	setQuotaSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_setquota, &st);
    setQuotaSyntax = cmd_CreateSyntax("setquota", fts_DoSetQuota, (char *)NULL,
	  (char *) cmd_explanation );
    free(cmd_explanation);

    (void)cmd_Seek(setQuotaSyntax, SETQUOTA_PATH_OFFSET);
    SetUpPathSwitch(setQuotaSyntax, CMD_SINGLE, CMD_REQUIRED);
    fts_ui_SetUpFilesetSwitch(setQuotaSyntax, CMD_SINGLE, CMD_REQUIRED,
			       SETQUOTA_FILESET_OFFSET);
    (void)cmd_MakeAlternate(setQuotaSyntax, "-path", "-fileset");

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_path_fileset, &st);
    (void)cmd_IndexedAddParm(setQuotaSyntax, "-size", CMD_SINGLE, CMD_REQUIRED, 
	  (char *) cmd_explanation, SETQUOTA_SIZE_OFFSET);
    free(cmd_explanation);

    fts_ui_SetUpNormalDefaults(setQuotaSyntax, 0);
}

static void fts_ui_SetUpSetRepInfo()
{
    register struct cmd_syndesc * setRepInfoSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_setrepinfo, &st);
    setRepInfoSyntax = cmd_CreateSyntax("setrepinfo", fts_DoSetRepInfo, 0,
	  (char *) cmd_explanation );
    free(cmd_explanation);
    fts_ui_SetUpFilesetSwitch(setRepInfoSyntax, CMD_SINGLE, CMD_REQUIRED,
			       SETREPL_FILESET_OFFSET);


    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_release_replication, &st);
    cmd_IndexedAddParm(setRepInfoSyntax, "-release", CMD_FLAG, CMD_OPTIONAL,
	  (char *) cmd_explanation , SETREPL_RELEASE_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_scheduled_replication, &st);
    cmd_IndexedAddParm(setRepInfoSyntax, "-scheduled", CMD_FLAG, CMD_OPTIONAL,
	  (char *) cmd_explanation , SETREPL_SCHEDULED_OFFSET);
    free(cmd_explanation);

    (void)cmd_MakeAlternate(setRepInfoSyntax, "-release", "-scheduled");

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_change_replication, &st);
    cmd_IndexedAddParm(setRepInfoSyntax, "-change", CMD_FLAG, CMD_OPTIONAL,
	  (char *) cmd_explanation , SETREPL_CHANGE_OFFSET);
    free(cmd_explanation);

    /* max (target) total latency */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_maxage, &st);
    cmd_IndexedAddParm(setRepInfoSyntax, "-maxage", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation , SETREPL_MAXAGE_OFFSET);
    free(cmd_explanation);

    /* hard max total latency */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_failage, &st);
    cmd_IndexedAddParm(setRepInfoSyntax, "-failage", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation , SETREPL_FAILAGE_OFFSET);
    free(cmd_explanation);

    /* dally period before reclaiming storage */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_reclaimwait, &st);
    cmd_IndexedAddParm(setRepInfoSyntax, "-reclaimwait", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation , SETREPL_RECLAIMWAIT_OFFSET);
    free(cmd_explanation);

    /* minimum pounce dally */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_minrepdelay, &st);
    cmd_IndexedAddParm(setRepInfoSyntax, "-minrepdelay", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation , SETREPL_MINREPDELAY_OFFSET);
    free(cmd_explanation);

    /* target lazy-replication latency for all replicas, in seconds */

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_defaultsiteage, &st);
    cmd_IndexedAddParm(setRepInfoSyntax, "-defaultsiteage", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation , SETREPL_DFLTSITEAGE_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_clear_old, &st);
    cmd_IndexedAddParm(setRepInfoSyntax, "-clear", CMD_FLAG, CMD_OPTIONAL,
	  (char *) cmd_explanation , SETREPL_CLEAR_OFFSET);
    free(cmd_explanation);

#if !defined(OSF_NO_SOCKET_DUMP)
    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_sockflag, &st);
    cmd_IndexedAddParm(setRepInfoSyntax, "-socket", CMD_FLAG, CMD_OPTIONAL, 
  	  (char *) cmd_explanation, SETREPL_SOCKFLAG_OFFSET);
    free(cmd_explanation);
#endif /* OSF_NO_SOCKET_DUMP */

    fts_ui_SetUpNormalDefaults(setRepInfoSyntax, 0);
}

static void fts_ui_SetUpStatFtServer()
{
    register struct cmd_syndesc * statFtServerSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_statftserver, &st);
    statFtServerSyntax = cmd_CreateSyntax("statftserver", fts_DoVolserStatus, 0,
	  (char *) cmd_explanation );
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_server, &st);
    (void)cmd_IndexedAddParm(statFtServerSyntax, "-server", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation, STATUS_SERVER_OFFSET);
    free(cmd_explanation);

    fts_ui_SetUpNormalDefaults(statFtServerSyntax, 0);
}

static void fts_ui_SetUpSyncFldb()
{
    register struct cmd_syndesc * syncFldbSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_syncfldb, &st);
    syncFldbSyntax = cmd_CreateSyntax("syncfldb", fts_DoSyncVldb, 0,
	  (char *) cmd_explanation );
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_server, &st);
    (void)cmd_IndexedAddParm(syncFldbSyntax, "-server", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation, SYNCFLDB_SERVER_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_aggregate, &st);
    (void)cmd_IndexedAddParm(syncFldbSyntax, "-aggregate", CMD_SINGLE, CMD_OPTIONAL, 
	  (char *) cmd_explanation, SYNCFLDB_AGGREGATE_OFFSET);
    free(cmd_explanation);

    fts_ui_SetUpNormalDefaults(syncFldbSyntax, 0);
}

static void fts_ui_SetUpSyncServer()
{
    register struct cmd_syndesc * syncServSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_syncserv, &st);
    syncServSyntax = cmd_CreateSyntax("syncserv", fts_DoSyncServer, 0,
	  (char *) cmd_explanation );
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_server, &st);
    (void)cmd_IndexedAddParm(syncServSyntax, "-server", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation, SYNCSERV_SERVER_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_aggregate, &st);
    (void)cmd_IndexedAddParm(syncServSyntax, "-aggregate", CMD_SINGLE, CMD_OPTIONAL, 
	  (char *) cmd_explanation, SYNCSERV_AGGREGATE_OFFSET);
    free(cmd_explanation);

    fts_ui_SetUpNormalDefaults(syncServSyntax, 0);
}

static void fts_ui_SetUpUnlock()
{
    register struct cmd_syndesc * unlockSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_unlock, &st);
    unlockSyntax = cmd_CreateSyntax("unlock", fts_DoReleaseEntry, 0,
	  (char *) cmd_explanation );
    free(cmd_explanation);

    fts_ui_SetUpNormalDefaults(unlockSyntax, 1);
}

static void fts_ui_SetUpUnlockFLDB()
{
    register struct cmd_syndesc * unlockFldbSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_unlockfldb, &st);
    unlockFldbSyntax = cmd_CreateSyntax("unlockfldb", fts_DoUnlockFLDB, 0,
	  (char *) cmd_explanation );
    free(cmd_explanation);

    fts_ui_SetUpServerAndAggregate(unlockFldbSyntax, CMD_OPTIONAL,
				    UNLOCKFLDB_SERVER_OFFSET);
    fts_ui_SetUpNormalDefaults(unlockFldbSyntax, 0);
}

static void fts_ui_SetUpUpdate()
{
    register struct cmd_syndesc *updateSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_update_replica, &st);
    updateSyntax = cmd_CreateSyntax("update", fts_DoRepUpdate, 0, 
	  (char *) cmd_explanation);
    free(cmd_explanation);

    fts_ui_SetUpFilesetSwitch(updateSyntax, CMD_SINGLE, CMD_REQUIRED, UPDATE_FILESET_OFFSET);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_all_servers, &st);
    cmd_IndexedAddParm(updateSyntax, "-all", CMD_FLAG, CMD_REQUIRED,
	  (char *) cmd_explanation , UPDATE_ALL_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_server, &st);
    cmd_IndexedAddParm(updateSyntax, "-server", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation , UPDATE_SERVER_OFFSET);
    free(cmd_explanation);

    (void)cmd_MakeAlternate(updateSyntax, "-server", "-all");
    fts_ui_SetUpNormalDefaults(updateSyntax, 0);
}

static void fts_ui_SetUpZapVolume()
{
    register struct cmd_syndesc * zapSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_zap, &st);
    zapSyntax = cmd_CreateSyntax("zap", fts_DoVolumeZap, 0,
	  (char *) cmd_explanation );
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_ftid, &st);
    (void)cmd_IndexedAddParm(zapSyntax, "-ftid", CMD_SINGLE, CMD_REQUIRED, 
	  (char *) cmd_explanation, ZAP_FSID_OFFSET);
    free(cmd_explanation);

    fts_ui_SetUpServerAndAggregate(zapSyntax, CMD_REQUIRED, ZAP_SERVER_OFFSET);
    fts_ui_SetUpNormalDefaults(zapSyntax, 0);
}

static void fts_ui_SetUpSetProtectLevels()
{
    register struct cmd_syndesc * setProtectLevelsSyntax;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_setprotectlevels, &st);
    setProtectLevelsSyntax = cmd_CreateSyntax("setprotectlevels",
					      fts_DoSetProtectLevels, 0,
					      (char *) cmd_explanation);
    free(cmd_explanation);
    fts_ui_SetUpFilesetSwitch(setProtectLevelsSyntax, CMD_SINGLE, CMD_REQUIRED,
			       SETPROT_FILESET_OFFSET);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_minlclprotect, &st);
    cmd_IndexedAddParm(setProtectLevelsSyntax, "-minlocalprotectlevel", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation, SETPROT_MINLCLAUTHN_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_maxlclprotect, &st);
    cmd_IndexedAddParm(setProtectLevelsSyntax, "-maxlocalprotectlevel", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation, SETPROT_MAXLCLAUTHN_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_minrmtprotect, &st);
    cmd_IndexedAddParm(setProtectLevelsSyntax, "-minremoteprotectlevel", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation, SETPROT_MINRMTAUTHN_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_maxrmtprotect, &st);
    cmd_IndexedAddParm(setProtectLevelsSyntax, "-maxremoteprotectlevel", CMD_SINGLE, CMD_OPTIONAL,
	  (char *) cmd_explanation, SETPROT_MAXRMTAUTHN_OFFSET);
    free(cmd_explanation);

    fts_ui_SetUpNormalDefaults(setProtectLevelsSyntax, 0);
}

#if 0
static void fts_ui_SetUpEndTrans()
{
    register struct cmd_syndesc *ts;

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_etrans, &st);
    ts = cmd_CreateSyntax("etrans", fts_DoEndTrans, 0,
	  (char *) cmd_explanation );
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_server, &st);
    (void)cmd_IndexedAddParm(ts, "-server", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation, ETRANS_SERVER_OFFSET);
    free(cmd_explanation);

    cmd_explanation = dce_msg_get_msg(fts_s_uint_cmd_tansId, &st);
    (void)cmd_IndexedAddParm(ts, "-transId", CMD_SINGLE, CMD_REQUIRED,
	  (char *) cmd_explanation, ETRANS_TRANID_OFFSET);
    free(cmd_explanation);

    fts_ui_SetUpNormalDefaults(ts, 0);
}
#endif /* 0 */

void fts_ui_SetUpSyntax()
{
    fts_ui_SetUpAddSite();
    fts_ui_SetUpAggrInfo();
    fts_ui_SetUpClone();
    fts_ui_SetUpCloneSys();
    fts_ui_SetUpCreate();
    fts_ui_SetUpCrFLDBEntry();
    fts_ui_SetUpCrMount();
    fts_ui_SetUpCrServer();
    fts_ui_SetUpDelServer();
    fts_ui_SetUpDelete();
    fts_ui_SetUpDelEntry();
    fts_ui_SetUpDelMount();
    fts_ui_SetUpDump();
    fts_ui_SetUpEdServer();
    fts_ui_SetUpGetRepServerStatus();
    fts_ui_SetUpGetRepStatus();
    fts_ui_SetUpLockEntry();
    fts_ui_SetUpLsAggr();
    fts_ui_SetUpLsFldb();
    fts_ui_SetUpLSFT();
    fts_ui_SetUpLsHeader();
    fts_ui_SetUpLsMount();
    fts_ui_SetUpLsQuota();
    fts_ui_SetUpLsServer();
    fts_ui_SetUpMove();
    fts_ui_SetUpRelease();
    fts_ui_SetUpRename();
    /* fts_ui_SetUpReserveIDs(); */
    fts_ui_SetUpRestore();
    fts_ui_SetUpRmSite();
    fts_ui_SetUpSetQuota();
    fts_ui_SetUpSetRepInfo();
    fts_ui_SetUpStatFtServer();
    fts_ui_SetUpSyncFldb();
    fts_ui_SetUpSyncServer();
    fts_ui_SetUpUnlock();
    fts_ui_SetUpUnlockFLDB();
    fts_ui_SetUpUpdate();
    fts_ui_SetUpZapVolume();
    fts_ui_SetUpSetProtectLevels();
    /* fts_ui_SetUpEndTrans(); */
}
