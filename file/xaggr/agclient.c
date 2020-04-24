/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: agclient.c,v $
 * Revision 1.1.427.1  1996/10/02  21:11:15  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:51:23  damon]
 *
 * $EndLog$
 */
/*
*/
/* Copyright (C) 1996, 1989 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/lock.h>
#include <dcedfs/common_data.h>
#include <dcedfs/queue.h>
#include <dcedfs/xvfs_vnode.h>
#include <dcedfs/syscall.h>
#include <aggr.h>
#include <dcedfs/volume.h>
#include <ag_init.h>
#include <astab.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsxagmac.h>
#include <dfsxagsvc.h>
#include <dfsxagmsg.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/xaggr/agclient.c,v 1.1.427.1 1996/10/02 21:11:15 damon Exp $")

dce_svc_handle_t   xag_svc_handle;

void initialize_svc()
{
  error_status_t st;
  static char programName[] = "agclient";

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

static int GetArgs(line,args, nargs)
    register char *line;
    register char **args;
    register int *nargs;
{
    *nargs = 0;
    while (*line) {
	register char *last = line;
	while (*line == ' ')
	    line++;
	if (*last == ' ')
	    *last = 0;
	if (!*line)
	    break;
	*args++  = line, (*nargs)++;
	while (*line && *line != ' ')
	    line++;
    }
}

static int ag_Stat(argp)
    char **argp;
{
    int code;
    struct ag_status stat;
    int aggrId;

    sscanf(&(*argp)[0], "%d", &aggrId);
    code = afs_syscall(AFSCALL_AGGR, AGOP_STAT, aggrId, (int) &stat, 0, 0);
    return code;
}


static int ag_VolCreate(argp)
    char **argp;
{
    int code;	
    struct vol_status vstat;
    int aggrId;
    u_long high, low;
    afs_hyper_t volid;
    unsigned flags;

    bzero((char *)&vstat, sizeof(struct vol_status));
    sscanf(&(*argp)[0], "%d", &aggrId);
    ++argp;
    sscanf(&(*argp)[0], "%lu", &high);
    ++argp;
    sscanf(&(*argp)[0], "%lu", &low);
    ++argp;
    AFS_hset64(volid, high, low);
    bzero((char *)&vstat, sizeof(struct vol_status));
    vstat.vol_st.volId = volid;
    strncpy(vstat.vol_st.volName, *argp, sizeof(vstat.vol_st.volName));
    AFS_hset64(vstat.vol_dy.visQuotaLimit, 0, 10000*1024);
    AFS_hset64(vstat.vol_dy.allocLimit, 0, 10000*1024);
    vstat.vol_st.type = VOL_RW;
    vstat.vol_st.states = VOL_IS_COMPLETE | VOL_RW;
    /* 
     * XXXX Setup vstat!! XXXX
     */
    flags = AGGR_CREATE_ROOT;
    code = afs_syscall(AFSCALL_AGGR, AGOP_VOLCREATE, aggrId, (int) &volid,
		       (int) &vstat, flags);	/* XXX */
    return code;
}



static int ag_VolInfo(argp)
    char **argp;
{
    int code, aggrId, index;
    struct volume vol;

    sscanf(&(*argp)[0], "%d", &aggrId);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &index);
    ++argp;    
    code = afs_syscall(AFSCALL_AGGR, AGOP_VOLINFO, aggrId, index,
		       (int) &vol, 0);
    return code;
}


static int ag_Detach(argp)
    char **argp;
{
    int code;
    int aggrId;

    sscanf(&(*argp)[0], "%d", &aggrId);
    code = afs_syscall(AFSCALL_AGGR, AGOP_DETACH, aggrId, 0, 0, 0);
    return code;
}


static int ag_Attach(argp)
    char **argp;
{
    struct astab astab;
    int code, flags;
    struct ufs_astab fstab;

    bzero((char *)&fstab, sizeof(struct ufs_astab));
    bzero((char *)&astab, sizeof(struct astab));
    sscanf(&(*argp)[0], "%d", &flags);
    ++argp;    
    /* 
     * XXXX Setup fstab!! XXXX
     */
    code = afs_syscall(AFSCALL_AGGR, AGOP_ATTACH, (int) &astab,
		       flags, (int) &fstab, 0);	/* XXX */
    return code;
}


static int ag_Scan(argp)
    char **argp;
{
    int code, aggrId;
    struct aggr aggr;

    bzero((char *)&aggr, sizeof(struct aggr));
    sscanf(&(*argp)[0], "%d", &aggrId);
    ++argp;    
    code = afs_syscall(AFSCALL_AGGR, AGOP_SCAN, aggrId, (int) &aggr, 0, 0);
    return code;
}


#define VOLS_MAXAGGR	26
static int ag_AgEnum(argp)
    char **argp;
{
    int i, code, inlen, outlen;
    struct aggrList aggrList[VOLS_MAXAGGR];
    
    inlen = VOLS_MAXAGGR * sizeof(struct aggrList);
    code = afs_syscall(AFSCALL_AGGR, AGOP_AGGENUMERATE, inlen, (int) aggrList,
		       (int) &outlen, 0);
    if (!code) {
	dce_svc_printf(XAG_S_OUTLEN_MSG, outlen);
	inlen = outlen / sizeof(struct aggrList);
	for (i = 0; i < inlen; i++) 
	    dce_svc_printf(XAG_S_AGGR_ENTRY_MSG, i, aggrList[i].name,
		   aggrList[i].Id, aggrList[i].type);
    }
    return code;
}

static int ag_AgInfo(argp)
    char **argp;
{
    int code, aggrId;
    struct aggrInfo aggrInfo;
    struct aggr aggr;

    bzero((char *)&aggr, sizeof(struct aggr));
    sscanf(&(*argp)[0], "%d", &aggrId);
    ++argp;    
    code = afs_syscall(AFSCALL_AGGR, AGOP_AGGINFO, aggrId, (int) &aggrInfo, 0, 0);
    if (!code) {
	dce_svc_printf(XAG_S_AGGR_INFO_MSG, aggrId);
	dce_svc_printf(XAG_S_MORE_AGGR_INFO_MSG, aggrInfo.name, aggrInfo.devName,
	       aggrInfo.type, aggrInfo.totalUsable, aggrInfo.realFree, aggrInfo.minFree);
    }
    return code;
}


static int ag_VolEnum(argp)
    char **argp;
{
    int code, aggrId, max, outindex, outlen, position, len, i;
    struct vol_status status[30];
    struct io_rwDesc iodesc;

    bzero((char *)&iodesc, sizeof(struct io_rwDesc));
    sscanf(&(*argp)[0], "%d", &aggrId);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &max);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &position);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &len);
    ++argp;    
    iodesc.descId = aggrId;
    iodesc.position = position;
    iodesc.length = len;
    iodesc.bufferp = (char *)status;
    code = afs_syscall(AFSCALL_AGGR, AGOP_VOLENUMERATE, max,
		       (int) &iodesc, (int) &outindex, (int) &outlen);
    if (!code) {
	dce_svc_printf(XAG_S_OUTLEN2_MSG, outlen);
	len = outlen / sizeof(struct vol_status);
	for (i = 0; i < len; i++)
	    dce_svc_printf(XAG_S_OUTLEN_INFO_MSG, i, status[i].vol_st.volName,
			   AFS_HGETBOTH(status[i].vol_st.volId),
		   status[i].vol_st.type, status[i].vol_st.states);
    }
    return code;
}

static char *(args[50]);

main(argc, argv)
    int argc;
    char **argv; {
    register long code = 0;

    osi_setlocale(LC_ALL, "");


    initialize_svc();


    while (1) {
	char line[500];
	int nargs;

	dce_printf(xag_s_ag_redir);
	if (gets(line) != NULL) {
	    char *oper;
	    register char **argp = args;
	    GetArgs(line, argp, &nargs);
	    oper = &argp[0][0];
	    ++argp, --nargs;
	    if (!strcmp(oper, "stat")) {
		code = ag_Stat(argp);
	    } else if (!strcmp(oper, "volcr")) {
		code = ag_VolCreate(argp);
	    } else if (!strcmp(oper, "volin")) {
		code = ag_VolInfo(argp);
	    } else if (!strcmp(oper, "attach")) {
		code = ag_Attach(argp);
	    } else if (!strcmp(oper, "detach")) {
		code = ag_Detach(argp);
	    } else if (!strcmp(oper, "scan")) {
		code = ag_Scan(argp);
	    } else if (!strcmp(oper, "agenum")) {
		code = ag_AgEnum(argp);
	    } else if (!strcmp(oper, "aginfo")) {
		code = ag_AgInfo(argp);
	    } else if (!strcmp(oper, "volenum")) {
		code = ag_VolEnum(argp);
	    } else if ((!strcmp(oper,"q")) || !strcmp(oper, "quit")) {
		exit(0);
	    } else {
		dce_svc_printf(XAG_S_UNKNOWN_OPER_MSG);
		dce_svc_printf(XAG_S_STAT_INFO_MSG);
		dce_svc_printf(XAG_S_VOLCR_INFO_MSG);
		dce_svc_printf(XAG_S_VOLIN_INFO_MSG);
		dce_svc_printf(XAG_S_ATTACH_INFO_MSG);
		dce_svc_printf(XAG_S_DETACH_INFO_MSG);
		dce_svc_printf(XAG_S_SCAN_INFO_MSG);
		dce_svc_printf(XAG_S_AGENUM_INFO_MSG);
		dce_svc_printf(XAG_S_AGINFO_MSG);
		dce_svc_printf(XAG_S_VOLENUM_INFO_MSG);
	    }
	    if (code == 0) dce_svc_printf(XAG_S_RET_CODE_ZERO_MSG);
	    else dce_svc_printf(XAG_S_ERRNO_INFO_MSG, code, errno);
	}
    }
}
