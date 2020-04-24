/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (c) 1995, 1996 Transarc Corporation
 * All rights reserved
 */
/*
 * HISTORY
 * $Log: udebug.c,v $
 * Revision 1.1.71.1  1996/10/02  17:56:20  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:43:00  damon]
 *
 * $EndLog$
*/
/*
 * P_R_P_Q_# (C) COPYRIGHT IBM CORPORATION 1987, 1988
 * Copyright TRANSARC CORPORATION 1989
 * LICENSED MATERIALS - PROPERTY OF IBM
 * REFER TO COPYRIGHT INSTRUCTIONS FORM NUMBER G120-2083
 */

#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/cmd.h>
#include <dcedfs/osi_net.h>
#include <dce/rpc.h>
#include <pthread.h>
#include <dcedfs/compat.h>

#define UBIK_INTERNALS
#include <dcedfs/nubik.h>
#include <dcedfs/ubikvote_proc.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/ncsubik/udebug.c,v 1.1.71.1 1996/10/02 17:56:20 damon Exp $")

extern struct hostent *hostutil_GetHostByName();

#ifndef uabs
static uabs(a)
    int a; {
    return (a<0? -a : a);
}
#endif /* !defined(uabs) */

static short PortNumber(aport)
char *aport; {
    int tc;
    long total;

    total = 0;
    while (tc = *aport++) {
	if (tc < '0' || tc > '9') return -1;	/* bad port number */
	total *= 10;
	total += tc - (int) '0';
    }
    return htons(total);
}

static short PortName(aname)
char *aname; {
    struct servent *ts;
    ts = getservbyname(aname, (char *) 0);
    if (!ts) return -1;
    return ts->s_port;	/* returns it in network byte order */
}

static CommandProc(as, arock)
char *arock;
struct cmd_syndesc *as; {
    char *hostName, *portName;
    unsigned_char_t *uuid_str;
    long hostAddr;
    struct in_addr inhostAddr;
    long i, code;
    short port;
    int longp;
    rpc_binding_handle_t thandle;
    struct sockaddr_in taddr;
    error_status_t st;
    long now;
    struct hostent *th;
    rpc_binding_handle_t tconn;
    struct ubik_debug_v2 udebug_v2;
    struct ubik_debug udebug;
    struct ubik_sdebug usdebug;
    struct ubik_sdebug_v2 usdebug_v2;
    int pVers;
    uuid_t nil_uuid;

    /* cds */
    char localHostName[MAXHOSTNAMELEN];
    char *nsGroupP = 0,
	 *entryNameP = 0;
    uuid_t object_uuid;

    if (as->parms[0].items)
	nsGroupP = as->parms[0].items->data;
    else
	nsGroupP = (char *) 0;

    if (as->parms[1].items)
	entryNameP = as->parms[1].items->data;
    else
	entryNameP = (char *) 0;

    /* -long flag - print all information */
    longp = (as->parms[2].items ? 1 : 0);

    /* CDS */
    /* get the object uuid from the group */
    compat_get_group_object_uuid((unsigned char *)nsGroupP,
				 &object_uuid, &st);
    if ( st )
    {
	fprintf(stderr, "udebug: compat_get_group_object_uuid failed; %s\n",
	       dfs_dceErrTxt(st));
	exit(1);
    }

    if ( entryNameP == 0 )
    {
	/* local host - since no host specified */

	code = gethostname(localHostName, MAXHOSTNAMELEN);
	if ( code )
	{
	    fprintf(stderr, "udebug: ");
	    perror("gethostbyname failed");
	    exit(1);
	}
	entryNameP = &localHostName[0];
    }

    uuid_create_nil(&nil_uuid, (unsigned32 *)&st);
    if (st != uuid_s_ok) {
      fprintf(stderr, "udebug: failed to create nil uuid; %s\n",
	      dfs_dceErrTxt(st));
      exit(1);
    }
    /* get the bindings to the host */
    dfs_locate_rpc_server((unsigned char *)entryNameP, &tconn,
			  (uuid_t *)&nil_uuid,
			  0, 	/* localauth */
			  0, 	/* noauth */
			  &st);
    if ( (st != rpc_s_ok) && (st != COMPAT_ERROR_SHORTNAME_NOAUTH) )
    {
	fprintf(stderr, "udebug: failed to locate ubik server %s; %s\n",
		entryNameP, dfs_dceErrTxt(st));
	exit(1);
    }

    /* set the object uuid in the binding */
    rpc_binding_set_object(tconn, &object_uuid, &st);
    if ( st != rpc_s_ok )
    {
	fprintf(stderr, "udebug: rpc_binding_set_object failed; %s\n",
		dfs_dceErrTxt(st));
	exit(1);
    }

    /* now do the main call */
    code = UBIKVOTE_DebugV2(tconn, &udebug, &udebug_v2);
    if (code == rpc_s_op_rng_error) {
        /* host address var. needs to be set */
        code = rpcx_binding_to_sockaddr(tconn, &taddr);
        if ( code )
        {
          if (code < 0)
	    fprintf(stderr, "udebug: rpcx_binding_to_sockaddr failed\n");
          else
	    fprintf(stderr, "udebug: rpcx_binding_to_sockaddr failed; %s\n",
		    dfs_dceErrTxt(code));
          exit(1);
        }
        code = rpcx_ipaddr_from_sockaddr(&taddr, (char *)&hostAddr);
        if ( code )
        {
          if (code < 0)
	    fprintf(stderr, "udebug: rpcx_ipaddr_from_sockaddr failed\n");
          else
	    fprintf(stderr, "udebug: rpcx_ipaddr_from_sockaddr failed; %s\n",
		    dfs_dceErrTxt(code));
	    exit(1);
        }
	pVers = 1;
        code = UBIKVOTE_Debug(tconn, &udebug);
    } else if (!code) {
	hostAddr = htons(udebug_v2.myAddr);
	pVers = udebug_v2.providerVers;
    }
    if (code) {
      fprintf(stderr, "udebug: failed to obtain ubik server information; %s\n",
	      dfs_dceErrTxt(code));
      exit(1);
    }

    /* now print the main info */
    now = time(0);
    inhostAddr.s_addr = hostAddr;
    printf("Host %s, his time is %d\n", inet_ntoa(inhostAddr), udebug.now-now);
    if (uabs(udebug.now-now) >= MAXSKEW)
	printf("****clock may be bad\n");
    inhostAddr.s_addr = htonl(udebug.lastYesHost);
    printf("Vote: Last yes vote for %s at %d ", inet_ntoa(inhostAddr),
	   udebug.lastYesTime - now);
    if (udebug.lastYesState) printf("(sync site)\n");
    else printf("(not sync site)\n");
    if (pVers >= 2) {
        uuid_to_string(&udebug_v2.lastYesPrinc, &uuid_str, &st);
	printf("      Last vote was for principal %s\n", uuid_str);
	rpc_string_free(&uuid_str, &st);
        uuid_to_string(&udebug_v2.lastYesRealm, &uuid_str, &st);
	printf("      Last vote was for realm %s\n", uuid_str);
	rpc_string_free(&uuid_str, &st);
    }
    printf("      Last vote started at %d\n", udebug.lastYesClaim-now);
    printf("Local db version is %d.%d\n", udebug.localVersion.epoch,
	   udebug.localVersion.counter);
    if (udebug.amSyncSite) {
	printf("I am sync site until %d (%d servers)\n",
	       udebug.syncSiteUntil - now, udebug.nServers);
	printf("Recovery state %x\n", udebug.recoveryState);
	if (udebug.activeWrite) {
	    printf("I am currently managing write trans %d.%d\n",
		   udebug.epochTime, udebug.tidCounter);
	}
    }
    else {
	printf("I am not sync site\n");
	inhostAddr.s_addr = htonl(udebug.lowestHost);
	printf("Lowest host %s at %d\n", inet_ntoa(inhostAddr),
	       udebug.lowestTime - now);
	inhostAddr.s_addr = htonl(udebug.syncHost);
	printf("Sync host %s at %d\n", inet_ntoa(inhostAddr),
	       udebug.syncTime - now);
    }
    printf("Sync site's db version is %d.%d\n", udebug.syncVersion.epoch,
	   udebug.syncVersion.counter);

    printf("%d locked pages, %d of them for write\n", udebug.lockedPages,
	   udebug.writeLockedPages);

    if (udebug.anyReadLocks) printf("There are read locks held\n");
    if (udebug.anyWriteLocks) printf("There are write locks held\n");

    if (udebug.currentTrans) {
	if (udebug.writeTrans) printf("There is an active write transaction\n");
	else printf("There is at least one active read transaction\n");
	printf("Transaction tid is %d.%d\n", udebug.syncTid.epoch,
	       udebug.syncTid.counter);
    }
    if (udebug.epochTime == 0) {
	/* this host never a sync site */
	printf("This server has never been sync site\n");
    }
    else {
	printf("This server last became sync site at %d\n",
	       udebug.epochTime - now);
    }
    if (pVers >= 2) {
	printf("Provider version %d\n", pVers);
        uuid_to_string(&udebug_v2.myPrinc, &uuid_str, &st);
	printf("Principal %s\n", uuid_str);
	rpc_string_free(&uuid_str, &st);
        uuid_to_string(&udebug_v2.myRealm, &uuid_str, &st);
	printf("Realm %s\n", uuid_str);
	rpc_string_free(&uuid_str, &st);
    }

    now	= time(0);  /* reevaluate */

    if (longp || udebug.amSyncSite) {
	/* now do the subcalls */
	for(i=0;;i++) {
	    if (pVers >= 2) {
	        code = UBIKVOTE_SDebugV2(tconn, i, &usdebug, &usdebug_v2);
	    } else {
	        code = UBIKVOTE_SDebug(tconn, i, &usdebug);
	    }
	    if (code > 0) break;	/* done */
	    if (code < 0) {
		printf("error code %d from UBIKVOTE_SDebug\n", code);
		break;
	    }
	    inhostAddr.s_addr = htonl(usdebug.addr);
	    /* otherwise print the structure */
	    printf("\nServer %s: (db %d.%d)\n", inet_ntoa(inhostAddr),
		   usdebug.remoteVersion.epoch, usdebug.remoteVersion.counter);
	    printf("    last vote rcvd at %d, last beacon sent at %d, last vote was ",
		   usdebug.lastVoteTime-now, usdebug.lastBeaconSent-now);
	    if (usdebug.lastVote) printf("yes\n");
	    else printf("no\n");
	    printf("    dbcurrent=%d, up=%d, beaconSince=%d\n",
		   usdebug.currentDB, usdebug.up, usdebug.beaconSinceDown);
	    if (pVers >= 2) {
	        printf("    Provider version %d\n",usdebug_v2.providerVers);
                uuid_to_string(&usdebug_v2.principal, &uuid_str, &st);
	        printf("    Principal %s\n", uuid_str);
	        rpc_string_free(&uuid_str, &st);
                uuid_to_string(&usdebug_v2.realm, &uuid_str, &st);
	        printf("    Realm %s\n", uuid_str);
	        rpc_string_free(&uuid_str, &st);
	    }
	}
    }
}

main(argc, argv)
    int argc;
    char **argv; {
    struct cmd_syndesc *ts;

    osi_setlocale(LC_ALL, "");
    ts = cmd_CreateSyntax((char *) 0, CommandProc, 0, "probe ubik server");
/* cmd_AddParm(ts, "-servers", CMD_SINGLE, CMD_REQUIRED, "server machine"); */
    cmd_AddParm(ts, "-rpcgroup", CMD_SINGLE, CMD_REQUIRED, "RPC_server_group");

/*  cmd_AddParm(ts, "-port", CMD_SINGLE, CMD_OPTIONAL, "IP port"); */
    cmd_AddParm(ts, "-server", CMD_SINGLE, CMD_OPTIONAL, "machine");
    cmd_AddParm(ts, "-long", CMD_FLAG, CMD_OPTIONAL, "print all info");

    cmd_Dispatch(argc, argv);
    exit(0);
}
