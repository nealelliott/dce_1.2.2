/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: pxclient.c,v $
 * Revision 1.1.10.1  1996/10/02  18:13:14  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:45:41  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/afs4int.h>
#include <dcedfs/lock.h>
#include <ubik.h>


RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/px/pxclient.c,v 1.1.10.1 1996/10/02 18:13:14 damon Exp $")

struct ubik_client *cstruct;
handle_t sconn, serverconns[MAXSERVERS];
char *(args[50]);
afs_hyper_t hyp0;

extern int AFS_FetchData(), AFS_StoreData(), AFS_StoreACL();
extern int AFS_GetTime(), AFS_GetStatistics(), AFS_FetchStatus(), AFS_FetchACL();
extern int AFS_StoreStatus(), AFS_RemoveFile(), AFS_CreateFile();
extern int AFS_Rename(), AFS_Symlink(), AFS_HardLink(), AFS_MakeDir(), AFS_RemoveDir();
extern int AFS_Readdir(), AFS_MakeMountPoint(), AFS_ReleaseTokens(), AFS_GetToken();
extern int AFS_BulkStatus(), AFS_Lookup();
extern int AFS_BulkStatus(), AFS_BulkLookup();
extern int AFS_BulkFetchVV(), AFS_BulkKeepAlive();
extern int AFS_Spare1(), AFS_Spare2(), AFS_Spare3(), AFS_Spare4(), AFS_Spare5(), AFS_Spare6();

long pxclient_Initialize(auth, serverAddr)
int auth;
long serverAddr;
{
    /* Some ancient RX code used to be here. */

    return 0;
}

/* main program */
main(argc, argv)
    int argc;
    char **argv; {
    char **av = argv;
    struct sockaddr_in host;
    register long code;
    struct hostent *hp;
    char *hostname;
    char hnamebuf[200];
    int	noAuth = 1;	    /* Default is authenticated connections */

    argc--, av++;
    if (argc < 1) {
	printf("usage: pxclient <serverHost>\n");
	exit(1);
    }
    AFS_hzero(hyp0);
    bzero((char *)&host, sizeof(struct sockaddr_in));
    host.sin_family = AF_INET;
    host.sin_addr.s_addr = inet_addr(av[0]);
    if (host.sin_addr.s_addr != -1) {
	strcpy(hnamebuf, av[0]);
	hostname = hnamebuf;
    } else {
	hp = gethostbyname(av[0]);
	if (hp) {
	    host.sin_family = hp->h_addrtype;
	    bcopy(hp->h_addr, (caddr_t)&host.sin_addr, hp->h_length);
	    hostname = hp->h_name;
	} else {
	    printf("unknown server host %s\n", av[0]);
	    exit(1);
	}
    }    
    if (code = pxclient_Initialize(noAuth, host.sin_addr.s_addr)) {
	printf("Couldn't initialize fs library (code=%d).\n",code);
	exit(1);
    }

    while (1) {
	char line[500];
	int nargs;

	printf("fs> ");
	if (gets(line) != NULL) {
	    char *oper;
	    register char **argp = args;
	    GetArgs(line, argp, &nargs);
	    oper = &argp[0][0];
	    ++argp, --nargs;
	    if (!strcmp(oper, "probe")) {
		struct timeval tv;

		code = ubik_Call(AFS_GetTime, cstruct, 0, &tv.tv_sec, &tv.tv_usec);
		printf("return code is %d\n", code);
	        if (!code)
		    printf("sec=%d\n", tv.tv_sec);
	    } else if (!strcmp(oper, "fsstats")) {
		struct afsStatistics stats;
		
		code = ubik_Call(AFS_GetStatistics, cstruct, 0, &stats);
		printf("return code is %d\n", code);
	    } else if (!strcmp(oper, "fd")) {
	        code = FetchData(argp);
		printf("return code is %d\n", code);
	    } else if (!strcmp(oper, "fs")) {
	        code = FetchStatus(argp);
		printf("return code is %d\n", code);
	    } else if (!strcmp(oper, "fa")) {
	        code = FetchACL(argp);
		printf("return code is %d\n", code);
	    } else if (!strcmp(oper, "sd")) {
	        code = StoreData(argp);
		printf("return code is %d\n", code);
	    } else if (!strcmp(oper, "ss")) {
	        code = StoreStatus(argp);
		printf("return code is %d\n", code);
	    } else if (!strcmp(oper, "sa")) {
	        code = StoreACL(argp);
		printf("return code is %d\n", code);
	    } else if (!strcmp(oper, "cf")) {
	        code = CreateFile(argp);
		printf("return code is %d\n", code);
	    } else if (!strcmp(oper, "rf")) {
	        code = RemoveFile(argp);
		printf("return code is %d\n", code);
	    } else if (!strcmp(oper, "rn")) {
	        code = Rename(argp);
		printf("return code is %d\n", code);
	    } else if (!strcmp(oper, "sl")) {
	        code = Symlink(argp);
		printf("return code is %d\n", code);
	    } else if (!strcmp(oper, "hl")) {
	        code = HardLink(argp);
		printf("return code is %d\n", code);
	    } else if (!strcmp(oper, "md")) {
	        code = MakeDir(argp);
		printf("return code is %d\n", code);
	    } else if (!strcmp(oper, "rd")) {
	        code = RemoveDir(argp);
		printf("return code is %d\n", code);
	    } else if (!strcmp(oper, "rdd")) {
	        code = Readdir(argp);
		printf("return code is %d\n", code);
	    } else if (!strcmp(oper, "mm")) {
	        code = MakeMountPoint(argp);
		printf("return code is %d\n", code);
	    } else if (!strcmp(oper, "rt")) {
	        code = ReleaseTokens(argp);
		printf("return code is %d\n", code);
	    } else if (!strcmp(oper, "bs")) {
	        code = BulkStatus(argp);
		printf("return code is %d\n", code);
	    } else if (!strcmp(oper, "lk")) {
	        code = Lookup(argp);
		printf("return code is %d\n", code);
	    } else if (!strcmp(oper, "gt")) {
	        code = GetToken(argp);
		printf("return code is %d\n", code);
	    } else if (!strcmp(oper, "ka")) {
	        code = KeepAlive(argp);
		printf("return code is %d\n", code);
	    } else if ((!strcmp(oper,"q")) || !strcmp(oper, "quit")) 
		 exit(0);
	    else {
		 printf("Unknown oper! Available operations: \n\n");
		 printf("fd <vnode> <unique> <pos> <len>\n");		 
		 printf("fs <vnode> <unique>\n");
		 printf("fa <vnode> <unique>\n");
		 printf("sd <vnode> <unique> <pos> <len> <flen> [<mode>|-1] [<owner>|-1] [<length>|-1] <string>\n");
		 printf("ss <vnode> <unique> [<mode>|-1] [<owner>|-1] [<length>|-1]\n");
		 printf("sa <vnode> <unique> <string>\n");
		 printf("rf <vnode> <unique> <name>\n");
		 printf("cf <vnode> <unique> <name> [<mode>|-1] [<owner>|-1] [<length>|-1]\n");
		 printf("rn <ovnode> <ounique> <oname> <nvnode> <nunique> <nname>\n");
		 printf("sl <vnode> <unique> <name> <contents> [<mode>|-1] [<owner>|-1] [<length>|-1]\n");
		 printf("hl <dvnode> <dunique> <name> <evnode> <eunique>\n");
		 printf("md <vnode> <unique> <name> [<mode>|-1] [<owner>|-1] [<length>|-1]\n");
		 printf("rd <vnode> <unique> <name>\n");
		 printf("rdd <vnode> <unique> <pos> <len>\n");		 
		 printf("lk <vnode> <unique> <name>\n");
		 printf("gt <vnode> <unique> <tokenID>\n");
		 printf("ka <vol.l> <vnode> <unique> <isExec> <kaTime>\n");
	       }
	}
    }
}


GetArgs(line,args, nargs)
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


FetchData(argp)
    char **argp;
{
    struct afsFetchStatus OutStatus;
    afs_token_t Token;
    struct afsVolSync tsync;
    struct afsFid fid;
    int vnode, unique, position, length;
    int code;
    struct rx_call *tcall;

    sscanf(&(*argp)[0], "%d", &vnode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &unique);
    ++argp;    
    bzero(&fid, sizeof(struct afsFid));
    fid.Vnode = vnode;
    fid.Unique = unique;
    sscanf(&(*argp)[0], "%d", &position);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &length);
    ++argp;    
    /* There could be multiple IP addresses for a multi-homed machine.
       These should be handled in the right way. For now we just consider
       the first address
     */
    tcall = rx_NewCall(cstruct->conns[0][0]);
    code = StartAFS_FetchData(tcall, &fid, &hyp0, position, length, 0);
    if (!code) {
	code = FetchProc(tcall);
    }
    if (!code) {
	code = EndAFS_FetchData(tcall, &OutStatus, &Token, &tsync);
    }
    code = rx_EndCall(tcall, code);
}


static FetchProc(acall)
    register struct rx_call *acall;
{
    extern char *malloc();
    register char *tbuffer;
    long tlen, length, code;

    code = rx_Read(acall, &length, sizeof(long));
    length = ntohl(length);
    if (code != sizeof(long)) 
	return -1;
    tbuffer = malloc(256);
    while (length > 0) {
	tlen = (length > 256? 256 : length);
	code = rx_Read(acall, tbuffer, tlen);
	if (code != tlen) {
	    free(tbuffer);
	    return -1;
	}
	length -= tlen;
    }
    free(tbuffer);
    return 0;
}


FetchStatus(argp)
    char **argp;
{
    struct afsFetchStatus OutStatus;
    afs_token_t Token;
    struct afsVolSync tsync;
    struct afsFid fid;
    int vnode, unique;
    int code;

    sscanf(&(*argp)[0], "%d", &vnode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &unique);
    ++argp;    
    bzero(&fid, sizeof(struct afsFid));
    AFS_hset64(fid.Volume, 0, 10);
    fid.Vnode = vnode;
    fid.Unique = unique;
    code = ubik_Call(AFS_FetchStatus, cstruct, 0, &fid, &hyp0, 0,
		     &OutStatus, &Token, &tsync);
    return (code);
}


FetchACL(argp)
    char **argp;
{
    struct afsFetchStatus OutStatus;
    struct afsACL AccessList;
    afs_token_t Token;
    struct afsVolSync tsync;
    struct afsFid fid;
    int vnode, unique;
    int code;

    sscanf(&(*argp)[0], "%d", &vnode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &unique);
    ++argp;    
    bzero(&fid, sizeof(struct afsFid));
    AFS_hset64(fid.Volume, 0, 10);
    fid.Vnode = vnode;
    fid.Unique = unique;
    code = ubik_Call(AFS_FetchACL, cstruct, 0, &fid, &hyp0, 0,
		     &AccessList, &OutStatus, &tsync);
    return (code);
}


StoreData(argp)
    char **argp;
{
    struct afsStoreStatus InStatus;
    struct afsFetchStatus OutStatus;
    struct afsVolSync tsync;
    struct afsFid fid;
    int vnode, unique, position, length, filelength;
    int mode, owner, len;
    int code;
    char *string;
    struct rx_call *tcall;

    sscanf(&(*argp)[0], "%d", &vnode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &unique);
    ++argp;    
    bzero(&fid, sizeof(struct afsFid));
    AFS_hset64(fid.Volume, 0, 10);
    fid.Vnode = vnode;
    fid.Unique = unique;
    sscanf(&(*argp)[0], "%d", &position);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &length);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &filelength);
    ++argp;    
    bzero(&InStatus, sizeof(struct afsStoreStatus));
    sscanf(&(*argp)[0], "%d", &mode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &owner);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &len);
    ++argp;    
    if (mode != -1) {
	InStatus.mode = mode;
	InStatus.mask |= AFS_SETMODE;
    }
    if (owner != -1) {
	InStatus.owner = owner;
	InStatus.mask |= AFS_SETOWNER;
    }
    if (length != -1) {
	InStatus.length = length;
	InStatus.mask |= AFS_SETLENGTH;
    }
    string = &argp[0][0];
    ++argp;    

    /* There could be multiple IP addresses for a multi-homed machine.
       These should be handled in the right way. For now we just consider
       the first address
     */
    tcall = rx_NewCall(cstruct->conns[0][0]);
    code = StartAFS_StoreData(tcall, &fid, &InStatus, position, length, filelength, &hyp0, 0);
    if (!code) {
	code = StoreProc(tcall, string, length);
    }
    if (!code) {
	code = EndAFS_StoreData(tcall, &OutStatus, &tsync);
    }
    code = rx_EndCall(tcall, code);
    return (code);
}


static StoreProc(acall, string, length)
    register struct rx_call *acall;
    char *string;
    int length;
{
    long tlen, code;

    while (length > 0) {
	tlen = (length > 256? 256 : length);
	code = rx_Write(acall, string, tlen);
	if (code != tlen) {
	    return -1;
	}
	length -= tlen;
    }
    return 0;
}


StoreStatus(argp)
    char **argp;
{
    struct afsStoreStatus InStatus;
    struct afsFetchStatus OutStatus;
    struct afsVolSync tsync;
    struct afsFid fid;
    int vnode, unique, mode, owner, length;
    int code;

    sscanf(&(*argp)[0], "%d", &vnode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &unique);
    ++argp;    
    bzero(&fid, sizeof(struct afsFid));
    AFS_hset64(fid.Volume, 0, 10);
    fid.Vnode = vnode;
    fid.Unique = unique;
    bzero(&InStatus, sizeof(struct afsStoreStatus));
    sscanf(&(*argp)[0], "%d", &mode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &owner);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &length);
    ++argp;    
    if (mode != -1) {
	InStatus.mode = mode;
	InStatus.mask |= AFS_SETMODE;
    }
    if (owner != -1) {
	InStatus.owner = owner;
	InStatus.mask |= AFS_SETOWNER;
    }
    if (length != -1) {
	InStatus.length = length;
	InStatus.mask |= AFS_SETLENGTH;
    }
    code = ubik_Call(AFS_StoreStatus, cstruct, 0, &fid, &InStatus, &hyp0, 0,
		     &OutStatus, &tsync);
    return (code);
}


StoreACL(argp)
    char **argp;
{
    struct afsFetchStatus OutStatus;
    struct afsACL AccessList;
    afs_token_t Token;
    struct afsVolSync tsync;
    struct afsFid fid;
    char *string;
    int vnode, unique;
    int code;

    sscanf(&(*argp)[0], "%d", &vnode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &unique);
    ++argp;    
    bzero(&fid, sizeof(struct afsFid));
    AFS_hset64(fid.Volume, 0, 10);
    fid.Vnode = vnode;
    fid.Unique = unique;
    string = &argp[0][0];
    ++argp;    
    AccessList.afsACL_len = strlen(string)+1;
    AccessList.afsACL_val = string;
    code = ubik_Call(AFS_StoreACL, cstruct, 0, &fid, 
		     &AccessList, &hyp0, 0, &OutStatus, &tsync);
    return (code);
}


RemoveFile(argp)
    char **argp;
{
    struct afsFetchStatus OutDirStatus, OutFidStatus;
    struct afsVolSync tsync;
    struct afsFidName nameFid;
    struct afsFid fid, outFid;
    char *name;
    int vnode, unique;
    int code;

    sscanf(&(*argp)[0], "%d", &vnode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &unique);
    ++argp;    
    bzero(&fid, sizeof(struct afsFid));
    AFS_hset64(fid.Volume, 0, 10);
    fid.Vnode = vnode;
    fid.Unique = unique;
    name = &argp[0][0];
    ++argp;    
    bzero(&nameFid, sizeof(struct afsFidName));
    strcpy(nameFid.name, name);
    code = ubik_Call(AFS_RemoveFile, cstruct, 0, &fid, &nameFid, &hyp0, 0,
		     &OutDirStatus, &OutFidStatus, &outFid, &tsync);
    return (code);
}


CreateFile(argp)
    char **argp;
{
    struct afsFetchStatus OutDirStatus, OutFidStatus;
    struct afsStoreStatus InStatus;
    struct afsVolSync tsync;
    struct afsFid fid, outFid;
    afs_token_t Token;
    char *name;
    int vnode, unique, mode, owner, length;
    int code;

    sscanf(&(*argp)[0], "%d", &vnode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &unique);
    ++argp;    
    bzero(&fid, sizeof(struct afsFid));
    AFS_hset64(fid.Volume, 0, 10);
    fid.Vnode = vnode;
    fid.Unique = unique;
    name = &argp[0][0];
    ++argp;    
    bzero(&InStatus, sizeof(struct afsStoreStatus));
    sscanf(&(*argp)[0], "%d", &mode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &owner);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &length);
    ++argp;    
    if (mode != -1) {
	InStatus.mode = mode;
	InStatus.mask |= AFS_SETMODE;
    }
    if (owner != -1) {
	InStatus.owner = owner;
	InStatus.mask |= AFS_SETOWNER;
    }
    if (length != -1) {
	InStatus.length = length;
	InStatus.mask |= AFS_SETLENGTH;
    }
    code = ubik_Call(AFS_CreateFile, cstruct, 0, &fid, name, &InStatus, &hyp0, 0,
		     &outFid, &OutFidStatus, &OutDirStatus, &Token, &tsync);
    return (code);
}


Rename(argp)
    char **argp;
{
    struct afsFetchStatus OutOldDirStatus, OutNewDirStatus;
    struct afsFetchStatus OutOldFileStatus, OutNewFileStatus;
    struct afsVolSync tsync;
    struct afsFid OldDirFid, NewDirFid, OutOldFileFid, OutNewFileFid;
    struct afsFidName OldName, NewName;
    char *oname, *nname;
    int ovnode, ounique, nvnode, nunique;
    int code;

    sscanf(&(*argp)[0], "%d", &ovnode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &ounique);
    ++argp;    
    bzero(&OldDirFid, sizeof(struct afsFid));
    AFS_hset64(OldDirFid.Volume, 0, 10);
    OldDirFid.Vnode = ovnode;
    OldDirFid.Unique = ounique;
    oname = &argp[0][0];
    ++argp;    
    bzero(&OldName, sizeof(struct afsFidName));
    strcpy(OldName.name, oname);
    sscanf(&(*argp)[0], "%d", &nvnode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &nunique);
    ++argp;    
    bzero(&NewDirFid, sizeof(struct afsFid));
    AFS_hset64(NewDirFid.Volume, 0, 10);
    NewDirFid.Vnode = nvnode;
    NewDirFid.Unique = nunique;
    nname = &argp[0][0];
    ++argp;    
    bzero(&NewName, sizeof(struct afsFidName));
    strcpy(NewName.name, nname);
    code = ubik_Call(AFS_Rename, cstruct, 0, &OldDirFid, &OldName, &NewDirFid, &NewName, &hyp0, 0,
		     &OutOldDirStatus, &OutNewDirStatus,
		     &OutOldFileFid, &OutOldFileStatus,
		     &OutNewFileFid, &OutNewFileStatus, &tsync);
    return (code);
}


Symlink(argp)
    char **argp;
{
    struct afsFetchStatus OutDirStatus, OutFidStatus;
    struct afsStoreStatus InStatus;
    struct afsVolSync tsync;
    struct afsFid fid, outFid;
    afs_token_t Token;
    char *name, *linkcontents;
    int vnode, unique, mode, owner, length;
    int code;

    sscanf(&(*argp)[0], "%d", &vnode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &unique);
    ++argp;    
    bzero(&fid, sizeof(struct afsFid));
    AFS_hset64(fid.Volume, 0, 10);
    fid.Vnode = vnode;
    fid.Unique = unique;
    name = &argp[0][0];
    ++argp;    
    linkcontents = &argp[0][0];
    ++argp;    
    bzero(&InStatus, sizeof(struct afsStoreStatus));
    sscanf(&(*argp)[0], "%d", &mode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &owner);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &length);
    ++argp;    
    if (mode != -1) {
	InStatus.mode = mode;
	InStatus.mask |= AFS_SETMODE;
    }
    if (owner != -1) {
	InStatus.owner = owner;
	InStatus.mask |= AFS_SETOWNER;
    }
    if (length != -1) {
	InStatus.length = length;
	InStatus.mask |= AFS_SETLENGTH;
    }
    code = ubik_Call(AFS_Symlink, cstruct, 0, &fid, name, linkcontents, &InStatus, &hyp0, 0,
		     &outFid, &OutFidStatus, &OutDirStatus, &Token, &tsync);
    return (code);
}


HardLink(argp)
    char **argp;
{
    struct afsFetchStatus OutDirStatus, OutFidStatus;
    struct afsVolSync tsync;
    struct afsFid fid, existingFid;
    char *name;
    int vnode, unique;
    int code;

    sscanf(&(*argp)[0], "%d", &vnode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &unique);
    ++argp;    
    bzero(&fid, sizeof(struct afsFid));
    AFS_hset64(fid.Volume, 0, 10);
    fid.Vnode = vnode;
    fid.Unique = unique;
    name = &argp[0][0];
    ++argp;    
    sscanf(&(*argp)[0], "%d", &vnode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &unique);
    ++argp;    
    bzero(&existingFid, sizeof(struct afsFid));
    AFS_hset64(existingFid.Volume, 0, 10);
    existingFid.Vnode = vnode;
    existingFid.Unique = unique;
    code = ubik_Call(AFS_HardLink, cstruct, 0, &fid, name, &existingFid, &hyp0, 0,
		     &OutFidStatus, &OutDirStatus, &tsync);
    return (code);
}


MakeDir(argp)
    char **argp;
{
    struct afsFetchStatus OutDirStatus, OutFidStatus;
    struct afsStoreStatus InStatus;
    struct afsVolSync tsync;
    struct afsFid fid, outFid;
    afs_token_t Token;
    char *name;
    int vnode, unique, mode, owner, length;
    int code;

    sscanf(&(*argp)[0], "%d", &vnode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &unique);
    ++argp;    
    bzero(&fid, sizeof(struct afsFid));
    AFS_hset64(fid.Volume, 0, 10);
    fid.Vnode = vnode;
    fid.Unique = unique;
    name = &argp[0][0];
    ++argp;    
    bzero(&InStatus, sizeof(struct afsStoreStatus));
    sscanf(&(*argp)[0], "%d", &mode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &owner);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &length);
    ++argp;    
    if (mode != -1) {
	InStatus.mode = mode;
	InStatus.mask |= AFS_SETMODE;
    }
    if (owner != -1) {
	InStatus.owner = owner;
	InStatus.mask |= AFS_SETOWNER;
    }
    if (length != -1) {
	InStatus.length = length;
	InStatus.mask |= AFS_SETLENGTH;
    }
    code = ubik_Call(AFS_MakeDir, cstruct, 0, &fid, name, &InStatus, &hyp0, 0,
		     &outFid, &OutFidStatus, &OutDirStatus, &Token, &tsync);
    return (code);
}


RemoveDir(argp)
    char **argp;
{
    struct afsFetchStatus OutDirStatus;
    struct afsVolSync tsync;
    struct afsFid fid, outFid;
    struct afsFidName nameFid;
    char *name;
    int vnode, unique;
    int code;

    sscanf(&(*argp)[0], "%d", &vnode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &unique);
    ++argp;    
    bzero(&fid, sizeof(struct afsFid));
    AFS_hset64(fid.Volume, 0, 10);
    fid.Vnode = vnode;
    fid.Unique = unique;
    name = &argp[0][0];
    ++argp;    
    bzero(&nameFid, sizeof(struct afsFidName));
    strcpy(nameFid.name, name);
    code = ubik_Call(AFS_RemoveDir, cstruct, 0, &fid, &nameFid, &hyp0, 0,
		     &OutDirStatus, &outFid, &tsync);
    return (code);
}


Readdir(argp)
    char **argp;
{
    struct afsFetchStatus OutDirStatus;
    struct afsVolSync tsync;
    struct afsFid fid;
    afs_token_t Token;
    char *name;
    struct rx_call *tcall;
    int vnode, unique, offset, length, NextOffset;
    int code;

    sscanf(&(*argp)[0], "%d", &vnode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &unique);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &offset);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &length);
    ++argp;    
    bzero(&fid, sizeof(struct afsFid));
    AFS_hset64(fid.Volume, 0, 10);
    fid.Vnode = vnode;
    fid.Unique = unique;
    /* There could be multiple IP addresses for a multi-homed machine.
       These should be handled in the right way. For now we just consider
       the first address
     */
    tcall = rx_NewCall(cstruct->conns[0][0]);
    code = StartAFS_Readdir(tcall, &fid, offset, length, &hyp0, 0);
    if (!code) {
	code = FetchDir(tcall);
    }
    if (!code) {
	code = EndAFS_FetchData(tcall, &NextOffset, &OutDirStatus, &Token, &tsync);
    }
    code = rx_EndCall(tcall, code);
    return (code);
}


static FetchDir(acall)
    register struct rx_call *acall;
{
    extern char *malloc();
    register char *tbuffer;
    long tlen, length, code;
    struct dirent *dp;


    tbuffer = malloc(256);
    while (1) {
        code = rx_Read(acall, &length, sizeof(long));
	length = ntohl(length);
	if (code != sizeof(long)) 
	    return -1;
	if (length == 0) 
	    break;
	tlen = (length > 8192? 8192 : length);
	code = rx_Read(acall, tbuffer, tlen);
	if (code != tlen) {
	    free(tbuffer);
	    return -1;
	}
	length -= tlen;
    }
    dp = (struct dirent *)dp;
    free(tbuffer);
    return 0;
}


Lookup(argp)
    char **argp;
{
    struct afsFetchStatus OutDirStatus, OutFidStatus;
    struct afsVolSync tsync;
    struct afsFid fid, outFid;
    char *name;
    int vnode, unique;
    int code;

    sscanf(&(*argp)[0], "%d", &vnode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &unique);
    ++argp;    
    bzero(&fid, sizeof(struct afsFid));
    AFS_hset64(fid.Volume, 0, 10);
    fid.Vnode = vnode;
    fid.Unique = unique;
    name = &argp[0][0];
    ++argp;    
    code = ubik_Call(AFS_Lookup, cstruct, 0, &fid, name, &hyp0, 0,
		     &outFid, &OutFidStatus, &OutDirStatus, &tsync);
    return (code);
}


GetToken(argp)
    char **argp;
{
    struct afsFetchStatus OutStatus;
    struct afsVolSync tsync;
    afs_token_t MinToken, RealToken;
    struct afsFid fid;
    int vnode, unique, tokenId;
    int code;

    sscanf(&(*argp)[0], "%d", &vnode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &unique);
    ++argp;    
    bzero(&fid, sizeof(struct afsFid));
    AFS_hset64(fid.Volume, 0, 10);
    fid.Vnode = vnode;
    fid.Unique = unique;
    sscanf(&(*argp)[0], "%d", &tokenId);
    ++argp;    
    bzero(&MinToken, sizeof(afs_token_t));
    AFS_hset64(MinToken.tokenID, 0, tokenId);
    code = ubik_Call(AFS_GetToken, cstruct, 0, &fid, &MinToken, &hyp0, 0,
		     &RealToken, &OutStatus, &tsync);
    return (code);
}


MakeMountPoint(argp)
char **argp;
{ }


ReleaseTokens(argp)
char **argp;
{ }


BulkStatus(argp)
char **argp;
{ }

/*  printf("ka <vol.l> <vnode> <unique> <isExec> <kaTime>\n"); */
KeepAlive(argp)
    char **argp;
{
    struct afsBulkFEX fex;
    unsigned long numExec, spare4;
    struct afsFidExp fx;
    int code;
    int low;

    bzero(&fx, sizeof(struct afsFidExp));
    sscanf(&(*argp)[0], "%d", &low);
    ++argp;    
    AFS_hset64(fx.fid.Volume, 0, low);
    sscanf(&(*argp)[0], "%d", &fx.fid.Vnode);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &fx.fid.Unique);
    ++argp;    
    sscanf(&(*argp)[0], "%d", &numExec);
    ++argp;
    sscanf(&(*argp)[0], "%d", &fx.keepAliveTime);
    bzero(&fex, sizeof(struct afsBulkFEX));
    fex.afsBulkFEX_val = &fx;
    fex.afsBulkFEX_len = 1;
    code = ubik_Call(AFS_BulkKeepAlive, cstruct, 0, &fex, numExec, 0, 0, 0, &spare4);
    return (code);
}
