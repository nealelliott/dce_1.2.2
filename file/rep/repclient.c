/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: repclient.c,v $
 * Revision 1.1.137.1  1996/10/02  18:14:58  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:46:12  damon]
 *
 * $EndLog$
 */
/*
 * (C) Copyright 1990, 1996 Transarc Corporation
 * Licensed Materials - Property of Transarc
 * All Rights Reserved.
 */
#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>
#include <dcedfs/lock.h>
#include <dcedfs/rep_data.h>
#include <dcedfs/rep_proc.h>
#include <dcedfs/rep_errs.h>
#include <dcedfs/osi_dfserrors.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/rep/repclient.c,v 1.1.137.1 1996/10/02 18:14:58 damon Exp $")

static long scIndex;
static struct rpc_securityClass *sc;
static rpc_binding_handle_t RepSer;
/* static char confdir[100] = AFSCONF_CLIENTNAME; */
static char *(args[50]);

static GetArgs(line,args, nargs)
register char *line;
register char **args;
register int *nargs;
{
    char *sdum;

    for (sdum = line; *sdum != '\0'; ++sdum) if (*sdum == '\n') *sdum = ' ';
    *nargs = 0;
    while (*line != '\0') {
	register char *last = line;
	while (*line == ' ')
	    line++;
	if (*last == ' ')
	    *last = '\0';
	if (*line == '\0') {
	    *line = '\0';
	    break;
	}
	*args++  = line, (*nargs)++;
	while (*line && *line != ' ')
	    line++;
    }
}

static long rep_Initialize(auth, doServer, serverAddr)
int auth;
int doServer;
u_long serverAddr;
{
    long code;
    struct sockaddr_in srvSock;

    if (auth) {   /* we don't need tickets for null */
	scIndex = 0;
    }
    bzero((char *)&srvSock, sizeof(srvSock));
    srvSock.sin_family = AF_INET;
    srvSock.sin_addr.s_addr = serverAddr;
    rpcx_binding_from_sockaddr(&srvSock, &RepSer);
    if (RepSer == NULL) {
	fprintf(stderr, "rep_Initialize: can't connect to server\n");
	exit(1);
    }
    return 0;
}

static void PrintafsNetAddr(outfil, sockP)
FILE *outfil;
afsNetAddr *sockP;
{/* so print it in the same format. */
    int Ix, BigIx;

    fprintf(outfil, "%u:%u", (unsigned short) sockP->type, (unsigned char) sockP->data[0]);
    for (BigIx = sizeof(sockP->data)-1; BigIx > 0; --BigIx)
	if (sockP->data[BigIx] != 0) break;
    for (Ix = 1; Ix <= BigIx; ++Ix) fprintf(outfil, ".%u", (unsigned char) sockP->data[Ix]);
}

static void FillafsNetAddr(sockP, arg)
afsNetAddr *sockP;
char *arg;
{/* Fill in a struct afsNetAddr from the argument. */
    /* Make it be family:octet.octet.octet.octet.octet... all the way out. */
    int Ix, Family, Dum, AllOK;

    AllOK = 0;
    sockP->type = ((unsigned short) -1);
    bzero((char *)sockP->data, sizeof(sockP->data));
    Family = atoi(arg);
    if (Family > 0 && *arg != '-') {
	while (*arg >= '0' && *arg <= '9') ++arg;
	if (*arg == ':') {
	    ++arg;
	    AllOK = 1;
	    for (Ix = 0; Ix < sizeof(sockP->data); ++Ix) {
		if (*arg < '0' || *arg > '9') {AllOK = 0; break;}
		Dum = atoi(arg);
		while (*arg >= '0' && *arg <= '9') ++arg;
		if (Dum < 0 || Dum > 0xff || (*arg != '.' && *arg != '\0')) {AllOK = 0; break;}
		sockP->data[Ix] = Dum;
		if (*arg == '\0') break;
		++arg;		/* skip over the dot */
	    }
	}
	if (AllOK != 0) sockP->type = Family;
    }
}

static void display_nums(what, trackp)
char *what;
register struct repNumTrack *trackp;
{
    printf("%s: %lu done, %lu oversize; %lu tot overage, %lu,,%lu squared.\n",
	    what, trackp->Count, trackp->OverCount, trackp->SizeOverCount,
	    AFS_HGETBOTH(trackp->SizeOverCountSq));
    if (trackp->OverCount > 0) {
	printf("    Oversize avg: %ld\n", trackp->SizeOverCount / trackp->OverCount);
    }
    /* someday can calculate std dev with 64-bit quantity SizeOverCountSq. */
}

static void display_serverstat(ssp)
register struct repserverStatus *ssp;
{
    u_long nowSecs;
    nowSecs = osi_Time();
    printf("Replication server status:\n");
    printf("numReplicas: %lu; numHosts: %lu\n", ssp->numReplicas, ssp->numHosts);
    printf("numAllocVIDs: %lu; numUsedVIDs: %lu; numReusedVIDs: %lu\n",
	    ssp->numAllocVIDs, ssp->numUsedVIDs, ssp->numReusedVIDs);
    printf("nextForceKA: %lu (%ld secs from now)\n",
	    ssp->nextForceKA, ssp->nextForceKA - nowSecs);
    display_nums("Attns", &ssp->Attns);
    display_nums("KAs", &ssp->KAs);
    printf("spare1..9: %lu %lu %lu %lu %lu %lu %lu %lu %lu\n",
	    ssp->spare1, ssp->spare2, ssp->spare3,
	    ssp->spare4, ssp->spare5, ssp->spare6,
	    ssp->spare7, ssp->spare8, ssp->spare9);
    return;
}

static void display_repstat(ix, rsp)
u_long ix;
register struct repStatus *rsp;
{
    u_long nowSecs;
    nowSecs = osi_Time();
    printf("%lu: %s, cell %lu,,%lu; src %lu,,%lu/%lu (",
	    ix, rsp->volName, AFS_HGETBOTH(rsp->CellId),
	    AFS_HGETBOTH(rsp->srcVolId), rsp->srcAggr);
    PrintafsNetAddr(stdout, &rsp->srcAddr);
    printf(") to local %lu,,%lu/%lu\n",
	   AFS_HGETBOTH(rsp->volId), rsp->curAggr);
    printf("   flags %#lx, volstates %#lx.  NumKAs %lu; lastKA sweep=%lu.\n",
	    rsp->flags, rsp->volStates,
	    rsp->numKAs, rsp->lastKASweep);
    printf("   srcVV: %lu,,%lu; curVV: %lu,,%lu; WVT ID = %lu,,%lu\n",
	   AFS_HGETBOTH(rsp->srcVV),
	   AFS_HGETBOTH(rsp->curVV),
	   AFS_HGETBOTH(rsp->WVT_ID));
    printf("   tokenLoss %ld ago; tokenExpire %ld hence; lastPublish %ld ago\n",
	    nowSecs - rsp->tokenLossTime.sec,
	    rsp->tokenExpireTime.sec - nowSecs,
	    nowSecs - rsp->lastReplicaPublish.sec);
    printf("   vvCurr %lu.%06lu (%ld ago); vvPingCurr %lu.%06lu (%ld ago)\n",
	    rsp->vvCurr.sec, rsp->vvCurr.usec, nowSecs - rsp->vvCurr.sec,
	    rsp->vvPingCurr.sec, rsp->vvPingCurr.usec, nowSecs - rsp->vvPingCurr.sec);
    printf("   whenTried %lu.%06lu (%ld ago); whenToTryAgain %lu.%06lu (%ld hence)\n",
	    rsp->whenTried.sec, rsp->whenTried.usec,
	    nowSecs - rsp->whenTried.sec,
	    rsp->whenToTryAgain.sec, rsp->whenToTryAgain.usec,
	    rsp->whenToTryAgain.sec - nowSecs);
    printf("   Status msg: %s\n", rsp->RepMsg);
    printf("sh1..3: %lu,,%lu, %lu,,%lu, %lu,,%lu\n",
	   AFS_HGETBOTH(rsp->spareh1),
	   AFS_HGETBOTH(rsp->spareh2),
	   AFS_HGETBOTH(rsp->spareh3));
    printf("st1..2: %lu.%06lu, %lu.%06lu\n",
	    rsp->sparet1.sec, rsp->sparet1.usec,
	    rsp->sparet2.sec, rsp->sparet2.usec);
    printf("sl1..3: %lu %lu %lu\n", rsp->sparel1, rsp->sparel2, rsp->sparel3);
}

static printIfs(ifsp)
dfs_interfaceList *ifsp;
{
    register int ix;
    register struct dfs_interfaceDescription *ifp;
    char *uuidDesc;
    char ubuf[200];
    unsigned long st;

    if (ifsp->dfs_interfaceList_len == 0) printf("no interfaces");
    for (ix = 0; ix < ifsp->dfs_interfaceList_len; ++ix) {
	ifp = &ifsp->dfs_interfaceList_val[ix];
	uuid_to_string((uuid_t *)&(ifp->interface_uuid), (unsigned char **)&uuidDesc, &st);
	if (st == uuid_s_ok) {
	    strncpy(ubuf, uuidDesc, sizeof(ubuf));
	    rpc_string_free((unsigned char **)&uuidDesc, &st);
	} else {
	    sprintf(ubuf, "no translation: %s", dfs_dceErrTxt(st));
	}
	printf("[%d] u=%s M=%u m=%u; pv=%lu (%s)\n",
	       ix, ubuf, ifp->vers_major, ifp->vers_minor,
	       ifp->vers_provider, ifp->spareText);
    }
}

static print_usage()
{
    printf("Valid Commands:\n");
    printf("\tcrc (CheckReplicationConfig) cid.h cid.l vid.h vid.l flg\n");
    printf("\tacrc (AllCheckReplicationConfig) cid.h cid.l vid.h vid.l flg naddrs (addrs)*\n");
    printf("\tka (KeepFilesAlive) flg nfids (c.h c.l v.h v.l v u)*\n");
    printf("\tgc (GetVolChangedFiles) c.h c.l v.h v.l initvv.h initvv.l finvv.h finvv.l\n");
    printf("\tgs (GetRepStatus)\n");
    printf("\tgss (GetRepServerStatus)\n");
    printf("\tus (UpdateSelf) cid.h cid.l vid.h vid.l flg\n");
    printf("\tgi (GetServerInterfaces)\n");
    printf("\tmisc: q, quit, ?, h\n");
}

static void ScanHyper(char **argp, afs_hyper_t *h)
{
    long high, low;
    sscanf(argp[0], "%d", &high);
    sscanf(argp[1], "%d", &low);
    AFS_hset64(*h, high, low);
}

/* main program */
main(argc, argv)
int argc;
char **argv; {
    register long code;
    long i, imax;
    struct repserverStatus serverStat;
    struct repStatus repStat;
    char *hostname;
    struct sockaddr_in host;
    struct hostent *hp;
    int	noAuth = 1;	    /* Default is authenticated connections */
    int	isServer = 0;
    static char usage[] = "[-noauth] hostname";
    afs_hyper_t cellId, volId;
    u_long flg;
    struct afsNetAddrs addlAddrs;
    struct afsFids addlFids;

    osi_setlocale(LC_ALL, "");
    hostname = NULL;

    for (i=1; i<argc; i++) {
	if (strcmp(argv[i], "-noauth") == 0)
	    noAuth = 0;
	else if (argv[i][0] == '-') {
	    printf("repclient: Unrecognized arg '%s'\nusage: %s %s\n", argv[i], argv[0], usage);
	    exit(1);
	} else if (hostname == NULL) {
	    hostname = argv[i];
	} else {
	    printf("repclient: Unrecognized arg '%s'\nusage: %s %s\n", argv[i], argv[0], usage);
	    exit(1);
	}
    }
    if (hostname == NULL) {
	printf("repclient: missing hostname\nusage: %s %s\n", argv[0], usage);
	exit(1);
    }
    bzero((char *)&host, sizeof(struct sockaddr_in));
    host.sin_family = AF_INET;
    host.sin_addr.s_addr = inet_addr(hostname);
    if (host.sin_addr.s_addr == -1) {
	hp = gethostbyname(hostname);
	if (hp != NULL) {
	    host.sin_family = hp->h_addrtype;
	    bcopy(hp->h_addr, (caddr_t)&host.sin_addr, hp->h_length);
	    hostname = hp->h_name;
	    printf("Connecting to %s ([%s])\n", hostname, inet_ntoa(host.sin_addr));
	} else {
	    printf("unknown server host %s\nusage: %s %s\n", hostname, argv[0], usage);
	    exit(1);
	}
    }    
    if (code = rep_Initialize(noAuth, isServer, host.sin_addr.s_addr)) {
	printf("Couldn't initialize rep client library (code=%d).\n", code);
	exit(1);
    }
    osi_initDecodeTable();

    while (1) {
	char line[500];
	int nargs;
	unsigned long firstIx, nextIx, spare1, spare2, spare3, spare4, spare5, spare6;
	bzero((char *)&serverStat, sizeof(serverStat));
	bzero((char *)&repStat, sizeof(repStat));
	spare1 = spare2 = spare3 = spare4 = spare5 = spare6 = 0;
	printf("rep> ");
	if (fgets(line, sizeof(line), stdin) != NULL) {
	    char *oper;
	    register char **argp = args;
	    GetArgs(line, argp, &nargs);
	    if (nargs <= 0) continue;
	    oper = &argp[0][0];
	    ++argp, --nargs;
	    if (!strcmp(oper, "gss")) {
		code = REP_GetRepServerStatus(RepSer, 0, 0, 0, &serverStat, &spare4, &spare5);
		code = osi_errDecode(code);
		if (code == 0) {
		    printf("Returned OK; spare4=%lu; spare5=%lu\n", spare4, spare5);
		    display_serverstat(&serverStat);
		} else {
		    printf("Error: %ld (%ls)\n", code, dfs_dceErrTxt(code));
		}
	    } else if (!strcmp(oper, "gs")) {
		u_long nentries, i, bigi;
		struct repStatuses Stats;

		bigi = 1;
		for (firstIx = 0; 1; firstIx = nextIx) {
		    bzero((char *)&Stats, sizeof(struct repStatuses));
		    code = REP_GetRepStatus(RepSer, firstIx, 0, 0, 0, &nextIx, &Stats, &nentries, &spare1, &spare4, &spare5);
		    code = osi_errDecode(code);
		    if (code == 0) {
			printf("Returned OK; spare4=%lu; spare5=%lu\n", spare4, spare5);
			if (nentries > Stats.repStatuses_len) nentries = Stats.repStatuses_len;
			for (i = 0; i < nentries; ++i) {
			    display_repstat(bigi, &Stats.repStatuses_val[i]);
			    ++bigi;
			}
			if ((spare1 & 1) != 0) break;
		    } else {
			printf("Error: %ld (%ls)\n", code, dfs_dceErrTxt(code));
			break;
		    }
		}
	    } else if (!strcmp(oper, "crc")) {
		ScanHyper(argp, &cellId);
		argp+=2, nargs-=2;
		ScanHyper(argp, &volId);
		argp+=2, nargs-=2;
		sscanf(&(*argp)[0], "%x", &flg);
		code = REP_CheckReplicationConfig(RepSer, &cellId, &volId, flg, 0, 0, 0, &spare4, &spare5);
		code = osi_errDecode(code);
		printf("return code is %d\n",code);
	    } else if (!strcmp(oper, "acrc")) {
		ScanHyper(argp, &cellId);
		argp+=2, nargs-=2;
		ScanHyper(argp, &volId);
		argp+=2, nargs-=2;
		sscanf(&(*argp)[0], "%x", &flg);
		++argp, --nargs;
		sscanf(&(*argp)[0], "%x", &imax);
		if (imax > REP_ADDRBULKMAX) imax=  REP_ADDRBULKMAX;
		for (i = 0; i < imax; ++i) {
		    ++argp, --nargs;
		    FillafsNetAddr(&addlAddrs.afsNetAddrs_val[i], *argp);
		}
		addlAddrs.afsNetAddrs_len = imax;
		code = REP_AllCheckReplicationConfig(RepSer, &cellId, &volId, imax, &addlAddrs, flg, 0, 0, 0, &spare4, &spare5);
		code = osi_errDecode(code);
		printf("return code is %d\n",code);
	    } else if (!strcmp(oper, "ka")) {
		sscanf(&(*argp)[0], "%x", &flg);
		++argp, --nargs;
		sscanf(&(*argp)[0], "%x", &imax);
		if (imax > AFS_BULKMAX) imax = AFS_BULKMAX;
		for (i = 0; i < imax; ++i) {
		    ++argp, --nargs;
		    ScanHyper(argp, &addlFids.afsFids_val[i].Cell);
		    ++argp, --nargs;
		    ++argp, --nargs;
		    ScanHyper(argp, &addlFids.afsFids_val[i].Volume);
		    ++argp, --nargs;
		    ++argp, --nargs;
		    sscanf(&(*argp)[0], "%d", &addlFids.afsFids_val[i].Vnode);
		    ++argp, --nargs;
		    sscanf(&(*argp)[0], "%d", &addlFids.afsFids_val[i].Unique);
		}
		addlFids.afsFids_len = imax;
		code = REP_KeepFilesAlive(RepSer, &addlFids, imax, flg, 0, 0, 0, &spare4, &spare5);
		code = osi_errDecode(code);
		printf("return code is %d\n",code);
	    } else if (!strcmp(oper, "gc")) {
		afs_hyper_t initVV, finVV, realfinVV;
		u_long cookie, nextcookie, numfids, ctr;
		struct fidsInVol fiv;

		ScanHyper(argp, &cellId);
		argp+=2, nargs-=2;
		ScanHyper(argp, &volId);
		argp+=2, nargs-=2;
		ScanHyper(argp, &initVV);
		argp+=2, nargs-=2;
		ScanHyper(argp, &finVV);
		++argp, --nargs;
		for (cookie = flg = ctr = 0; (flg & 1) == 0; cookie = nextcookie) {
		    bzero((char *)&fiv, sizeof(struct fidsInVol));
		    code = REP_GetVolChangedFiles(RepSer, &cellId, &volId, &initVV, &finVV, cookie,
						  0, 0, 0, &realfinVV, &nextcookie, &fiv,
						  &numfids, &flg, &spare4, &spare5);
		    code = osi_errDecode(code);
		    if (code != 0) {
			printf("error: return code is %d\n",code);
			break;
		    }
		    if (cookie == 0) {
			printf("cell=%lu/%lu, vid=%lu,,%lu; going from %lu/%lu to %lu/%lu\n",
			       AFS_HGETBOTH(cellId), AFS_HGETBOTH(volId),
			       AFS_HGETBOTH(initVV), AFS_HGETBOTH(finVV));
		    }
		    printf("[%d] Really went to %lu/%lu\n",
			   AFS_HGETBOTH(realfinVV));
		    for (i = 0; i < numfids; ++i) {
			printf("[%d] vnode=%lu, unique=%lu\n", ctr,
			       fiv.fidsInVol_val[i].Vnode, fiv.fidsInVol_val[i].Unique);
			++ctr;
		    }
		}
	    } else if (!strcmp(oper, "us")) {
		ScanHyper(argp, &cellId);
		argp+=2, nargs-=2;
		ScanHyper(argp, &volId);
		argp+=2, nargs-=2;
		sscanf(&(*argp)[0], "%x", &flg);
		code = REP_UpdateSelf(RepSer, &cellId, &volId, flg, 0, 0, 0, &spare4, &spare5);
		code = osi_errDecode(code);
		printf("return code is %d\n",code);
	    } else if (!strcmp(oper, "gi")) {
		dfs_interfaceList ifs;
		bzero((char *)&ifs, sizeof(ifs));
		code = REP_GetServerInterfaces(RepSer, &ifs);
		code = osi_errDecode(code);
		printf("return code is %d\n",code);
		if (code == 0) printIfs(&ifs);
	    } else if ((!strcmp(oper,"?")) || !strcmp(oper,"h"))
		print_usage();
	    else if ((!strcmp(oper,"q")) || !strcmp(oper, "quit"))
		exit(0);
	    else
		printf("Unknown operation.\n");
	}
    }
}
