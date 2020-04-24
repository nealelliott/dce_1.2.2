/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: export.c,v $
 * $EndLog$
 */
/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/common_data.h>
#include <dcedfs/queue.h>
#include <dcedfs/syscall.h>
#include <ag_init.h>
#include <dcedfs/cmdAux.h>
#include <dcedfs/ftutil.h>
#include <dcedfs/osi_net.h>
#include <dcedfs/afs4int.h>
#include <dcedfs/tkm_errs.h>
#include <dcedfs/vol_errs.h>
#include <dce/dce_cf.h>

#include <astab.h>
#include <aggrDesc.h>
#include <volDesc.h>
#include <fcntl.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsxagmac.h>
#include <dfsxagsvc.h>
#include <dfsxagmsg.h>

#include <syslog.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/xaggr/RCS/export.c,v 4.100 1996/07/15 18:17:52 gait Exp $")

#define FSET_STATUS_CHUNK_SIZE	32

#if defined(AFS_AIX_ENV) || defined(AFS_SUNOS5_ENV)
PRIVATE struct flock fileRlock = { F_RDLCK, 0, 0, 0, 0, 0 };
#endif
#ifdef AFS_HPUX_ENV
PRIVATE struct flock fileRlock = { F_RDLCK, 0, 0, 0, 0 };
#endif

dce_svc_handle_t   xag_svc_handle;

void initialize_svc()
{
  error_status_t st;
  static char programName[] = "dfsexport";

  xag_svc_handle = dce_svc_register(xag_svc_table,  (idl_char *)"xag",  &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot register svc 0x%lx\n", st);
  dce_svc_set_progname(programName, &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot set program name 0x%lx\n", st);
  dce_svc_routing((unsigned char *) "NOTICE:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route NOTICE messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "WARNING:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route WARNING messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "ERROR:STDERR:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route ERROR messages  0x%lx\n", st);

  dfs_define_xag_message_tables();

}

struct attlist {
    struct attlist *next;
    struct astab *astabp;
    struct ufs_astab *fstabp;
}; 

PRIVATE int exitStat = 0;
PRIVATE int parm_unattach;
PRIVATE int parm_all;
PRIVATE int parm_verbose;
PRIVATE char parm_type[128];
int parm_force;

extern char *AstabFileName;

PRIVATE int exportDriver _TAKES((struct cmd_syndesc *as,
				 char *arock));

PRIVATE int attach _TAKES((struct astab *stabp,
			   struct ufs_astab *fstabp));

extern int attached _TAKES((struct astab *astabp,
			     struct attlist **attlistpp,
			     int *errp));


extern void printastab _TAKES((struct astab *stabp,
			       struct ufs_astab *fstabp,
			       FILE *stream));
extern unattachl _TAKES((char *mntp, int parm_all, char * prg_name, char *parm_type, int parm_force));

PRIVATE int usage(exstat)
int exstat;
{
    dce_svc_printf(XAG_S_USAGE_MSG);
    exit(exstat);
}

main(argc, argv)
    int argc;
    char **argv;
{
    struct cmd_syndesc *ts;

    osi_setlocale(LC_ALL, "");


    initialize_svc();


    as_init();				/* initialize Astab name */

    ts = cmd_CreateSyntax((char *)0, exportDriver, (char *)0, "dfsexport" );
    /* '-all' at offset 0 */
    cmd_AddParm(ts, "-all", CMD_FLAG, CMD_OPTIONAL,
			"export_all_aggregates");
    /* '-aggregate' at offset 1 */
    cmd_AddParm(ts, "-aggregate", CMD_SINGLE, CMD_OPTIONAL,
			"name");
    (void)cmd_MakeAlternate(ts, "-all", "-aggregate");
    /* '-type' at offset 2 */
    cmd_AddParm(ts, "-type", CMD_SINGLE, CMD_OPTIONAL,
			"name");
    /* '-detach' at offset 3 */
    cmd_AddParm(ts, "-detach", CMD_FLAG, CMD_OPTIONAL,
			"detach_rather_than_attach");
    /* '-force' at offset 4 */
    cmd_AddParm(ts, "-force", CMD_FLAG, CMD_OPTIONAL,
			"force_detach");
    /* '-verbose' at offset 5 */
    cmd_AddParm(ts, "-verbose", CMD_FLAG, CMD_OPTIONAL,
			"print boring stuff");

    return cmd_Dispatch(argc, argv);
}


PRIVATE int exportDriver(as, arock)
  IN struct cmd_syndesc *as;
  IN char *arock;
{
    FILE *filep;
    char *mntp;
    static char buffer[256];
    struct astab astab;
    struct ufs_astab ufstab;
    int code, olen, found;
    int ctr;

    exitStat = 0;

    /* '-all' at offset 0 */
    /* '-aggregate' at offset 1 */
    /* '-type' at offset 2 */
    /* '-detach' at offset 3 */
    /* '-force' at offset 4 */
    /* '-verbose' at offset 5 */

    parm_unattach = 0;
    parm_all = 0;
    parm_verbose = 0;
    parm_force = 0;
    parm_type[0] = '\0';
    mntp = 0;

    if (as->parms[0].items)
	parm_all = 1;
    if (as->parms[1].items)
	mntp = as->parms[1].items->data;
    if (as->parms[2].items)
	strncpy(parm_type, as->parms[2].items->data, sizeof(parm_type));
    if (as->parms[3].items)
	parm_unattach = 1;
    if (as->parms[4].items)
	parm_force = 1;
    if (as->parms[5].items)
	parm_verbose = 1;

    if (!parm_all && !mntp) {
	/*
	 * Print all attached aggregates
	 */
	attached((struct astab *)0, (struct attlist **)0, 0);
	exit(exitStat);
    }
    if (parm_all && mntp) {
	dce_svc_printf(XAG_S_BAD_USAGE_MSG);
	usage(1);
    }
    if (parm_force && !parm_unattach) {
	dce_svc_printf(XAG_S_FORCE_USAGE_MSG);
	usage(1);
    }
    if (geteuid() != 0) {
	dce_svc_printf(XAG_S_PERMISSIONS_MSG);
	exit(1);
    }
    if (parm_unattach) {
	exit(unattachl(mntp,parm_all,"dfsexport",parm_type,parm_force));
    }
    if ((filep = fopen(AstabFileName, "r")) == NULL) {
	perror(AstabFileName);
	exit(1);
    }	    
#if defined (AFS_AIX_ENV) || defined (AFS_HPUX_ENV) || defined(AFS_SUNOS5_ENV)
    if (fcntl(fileno(filep), F_SETLK, &fileRlock) < 0)
#else
    if (flock(fileno(filep), LOCK_SH) < 0)
#endif
    {
	dce_svc_printf(XAG_S_LOCK_NOT_GOTTEN_MSG, AstabFileName, errno);
	fclose(filep);	
	exit(1);
    }
    found = 0;
    while (as_getent(filep, buffer, &astab, &ufstab)) {
	    
	if (!parm_all) {
	    if (strcmp(astab.as_spec, mntp) != 0
		&& strcmp(astab.as_aggrName, mntp) != 0
		&& strcmp(ufstab.uas_mountedon, mntp) != 0)
		continue;
	}
	if (parm_type[0] != 0 && strncmp(astab.as_type, parm_type, ASTABSIZE_TYPE) != 0)
	    continue;
	found = 1;
#ifdef	_EPISODE_LIC_MGMT_
	/* If exporting Episode aggregate grab license */
	if  ((strcmp(astab.as_type,"lfs") == 0) && (getEpisodeLicense() == 0)) {
	    fprintf(stderr, "dfsexport: can not export aggregate %s \n",
		astab.as_aggrName);
	  	exitStat = 1;
		continue;
	}
#endif
	code = attach(&astab, &ufstab);
	if (code == FTU_E_TRY_RECOVERY)
	    exitStat = 2;
	else if (code == FTU_E_ALREADY_ATTACHED)
	    exitStat = 3;
	else if (code)
	    exitStat = 1;
    }
    if (!found) {
	if (parm_all)
	    dce_svc_printf(XAG_S_NO_AGGRS_EXPORTED_MSG);
	else
	    dce_svc_printf(XAG_S_NO_AGGR_FOUND_MSG, mntp);
	exitStat = 1;
    }
    fclose(filep);
    exit(exitStat);
}


/*
 * Try to attach the specified entry
 */
PRIVATE int attach(stabp, fstabp)
    struct astab *stabp;
    struct ufs_astab *fstabp;
{
#ifdef notdef
    struct aggrDesc aggrDesc;
    struct volDesc volDesc;
#endif /* notdef */
    int code;

    /*
     * Read the "./aggrDesc" file to make sure it's consistent with the given entry.
     */
#ifdef	notdef
    code = readAggrDesc(stabp->as_dir, &aggrDesc);	
    if (!code) {
	dce_svc_printf(XAG_S_NO_READ_AGGRDESC_MSG, errno);
	exitStat = 1;
	return -1;
    }
    if (aggrDesc.astab.as_aggrId != stabp->as_aggrId ||
	!AFS_hsame(aggrDesc.ufstab.uas_volId, fstabp->uas_volId) ||
	strcmp(aggrDesc.astab.as_type, stabp->as_type, ASTABSIZE_TYPE) ||
	strcmp(aggrDesc.astab.as_aggrName, stabp->as_aggrName) ||
	strcmp(aggrDesc.astab.as_spec, stabp->as_spec)) {
	dce_fprintf(stderr, xag_s_entry_mismatch1, AstabFileName);
	printastab(astabp, fstabp);
	dce_fprintf(stderr, xag_s_entry_mismatch2, stabp->as_dir, AGGRDESC);
	printastab(&aggrDesc.astab, &aggrDesc.fstab);
	dce_fprintf(stderr, xag_s_entry_mismatch3);
	exitStat = 1;
	return -1;
    }

    /*
     * XXX Should do a similar test with the VOLDESC file too! XXX
     */
    code = readVolDesc(stabp->as_dir, &volDesc);	
    if (!code) {
	dce_svc_printf(XAG_S_RDVOLDESC_FAILED_MSG, errno);
	return -1;
    }
    if (attached(stabp, (struct attlist **)0, &code)) {
	if (code == 0) {
	dce_svc_printf(XAG_S_THIS_ID_NOTATT1_MSG, stabp->as_spec, 
		stabp->as_aggrName, stabp->as_aggrId);
	}
	else {
	dce_svc_printf(XAG_S_THIS_ID_NOTATT2_MSG, stabp->as_spec, 
		stabp->as_aggrName, stabp->as_aggrId);
	}
	return -1;
    }
#endif /* notdef */ 

   code = ftu_AttachAggrWithAstab(stabp, FTU_ATTACH_FLAGS_MOUNTAGGR,
				   (caddr_t)fstabp);
    if (code) {
	dce_svc_printf(XAG_S_ATTACH_IGNORED_MSG, 
		stabp->as_spec, stabp->as_aggrName, dfs_dceErrTxt(code));
	return code;
    }
    if (parm_verbose) {
	dce_svc_printf(XAG_S_GOOD_ATTACH_MSG, 
	    stabp->as_spec, stabp->as_aggrName);
    }
    return 0;
}


/*
 * AggrTypeToString - convert the numeric type to a character string
 * Note: this is taken out of newft.c. It should become part of a libray
 * in the next release.
 */
PRIVATE char *AggrTypeToString(type)
    unsigned type;
{
    static char *ag_types[MAX_AG_TYPE] = {
	"unknown",
	"ufs",
	"lfs", /* was "episode" */
	"jfs",
	"vxfs",
	"dmepi",
    };

    return (type < MAX_AG_TYPE) ? ag_types[type] : ag_types[0];
}
