/*
 * Copyright (c) 1996 Transarc Corporation
 */
#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>

#include <dcedfs/common_data.h>
#include <dcedfs/tkn4int.h>
#include <dcedfs/cmd.h>
#include <dcedfs/compat.h>		/* dfs_dceErrTxt */
#include <dcedfs/compat_errs.h>
#include <dcedfs/lock.h>
#include <dce/rpc.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/userInt/cmdebug/RCS/cm_debug.c,v 1.5 1996/03/04 19:14:32 ota Exp $")

#define		DFS_PROTSEQ		"ncadg_ip_udp"

static IsLocked(alock)
  struct afsDBLockDesc *alock;
{
    if (alock->waitStates || alock->exclLocked
	|| alock->numWaiting || alock->readersReading)
	return 1;
    return 0;
}

static PrintLock(alock)
  struct afsDBLockDesc *alock;
{
    printf("(");
    if (alock->waitStates) {
	if (alock->waitStates & READ_LOCK)
	    printf("reader_waiting");
	if (alock->waitStates & WRITE_LOCK)
	    printf("writer_waiting");
	if (alock->waitStates & SHARED_LOCK)
	    printf("upgrade_waiting");
    }
    else
	printf("none_waiting");
    if (alock->exclLocked) {
	if (alock->exclLocked & WRITE_LOCK)
	    printf(", write_locked");
	if (alock->exclLocked & SHARED_LOCK)
	    printf(", upgrade_locked");
    }
    if (alock->readersReading)
	printf(", %d read_locks", alock->readersReading);
    if (alock->numWaiting) printf(", %d waiters", alock->numWaiting);
    printf(")");
    return 0;
}

static PrintLocks(h, along)
  handle_t h;
  int along;
{
    register int i;
    struct afsDBLock lock;
    long code;

    for(i=0;i<1000;i++) {
	code = TKN_GetLock(h, i, &lock);
	if (code) {
	    if (code == 1) break;
	    /* otherwise we have an unrecognized error */
	    printf("cmdebug: error checking locks: %s\n", dfs_dceErrTxt(code));
	    return code;
	}
	/* here we have the lock information, so display it, perhaps */
	if (along || IsLocked(&lock.lock)) {
	    printf("Lock %s status: ", lock.name);
	    PrintLock(&lock.lock);
	    printf("\n");
	}
    }
    return 0;
}

static PrintCacheEntries(h, along)
  handle_t h;
  int along;
{
    register int i;
    register long code;
    struct afsDBCacheEntry centry;
    char *str;

    for(i=0;i<10000;i++) {
	code = TKN_GetCE(h, i, &centry);
	if (code) {
	    if (code == 1) break;
	    printf("cmdebug: failed to get cache entry %d (%s)\n", i,
		   dfs_dceErrTxt(code));
	    return code;
	}

	if (!along && !IsLocked(&centry.lock)) continue;
        if ( centry.refCount == 0 || centry.tokenExpiration == 0 ) continue;
	/* otherwise print this entry */
	printf("** Cache entry @ 0x%08x \n\t cell %u,,%u \n\t volume %u,,%u \n\t vnode %d\t fid.unique %d\n",
               centry.addr,
               AFS_HGETBOTH(centry.fid.Cell),
	       AFS_HGETBOTH(centry.fid.Volume),
               centry.fid.Vnode, centry.fid.Unique);
	if (IsLocked(&centry.lock)) {
	    printf("\t locks: ");
	    PrintLock(&centry.lock);
	    printf("\n");
	}
	printf("\t length %u,,%u\t data_v %u,,%u\t refcnt %d\n",
                AFS_HGETBOTH(centry.length), AFS_HGETBOTH(centry.dataVersion),
	       (short)centry.refCount);
	printf("\t %d opens\t %d readers\t %d writers\n",
                    centry.opens, centry.readers, centry.writers);
/*
        str = ctime((time_t*)&centry.tokenExpiration);
	printf("\t token expires at %s", str);
*/

	/* now display states */
	printf("\t states (0x%x)", centry.states);
	if (centry.states & 1) printf(", stat'd");
	if (centry.states & 2) printf(", read-only");
	if (centry.states & 4) printf(", mount pt valid");
	if (centry.states & 0x20) printf(", mapped file");
	if (centry.states & 0x40) printf(", mount pt for a volume");
	if (centry.states & 0x80) printf(", root of volume entry");
	if (centry.states & 0x100) printf(", return open tokens on close");
	if (centry.states & 0x200) printf(", a virtual bind dir");
	if (centry.states & 0x400) printf(", this dir is a cell root");
	if (centry.states & 0x800) printf(", this vnod is spec. dev socket or fifo");
	if (centry.states & 0x1000) printf(", failed to store the data");
	if (centry.states & 0x2000) printf(", storing status back");
	if (centry.states & 0x4000) printf(", fetching status now");
	if (centry.states & 0x4000) printf(", waiting for fetch or store flag");
	printf("\n");
	printf("\n");
    }
    return 0;
}

static void get_dfs_server(entry_nameP, bindingP, stp)
 char *entry_nameP;
 handle_t *bindingP;
 error_status_t *stp;
{
   unsigned_char_t *stringBindingP = 0;
   struct hostent *hostP;
   struct in_addr inetAddr;
   char *inAddrString;
   char **ptr;

   hostP = gethostbyname((char *)entry_nameP);
   if ( hostP == 0 ) {
     printf("Can not get host name %s\n", entry_nameP);
     exit(1);
   }
   ptr = hostP->h_addr_list;
   inetAddr = *((struct in_addr *) *ptr);
   inAddrString = (char *) inet_ntoa(inetAddr);
   rpc_string_binding_compose(NULL,                /* obj uuid */
          (unsigned char *)   DFS_PROTSEQ,         /* prot seq */
          (unsigned char *)   inAddrString,        /* addr */
                              NULL,                /* endpoint */
                              NULL,                /* options */
                              &stringBindingP,
                              stp);
   if (*stp != rpc_s_ok) {
      printf( "Can not get string binding.\n");
      exit(1);
   }
   rpc_binding_from_string_binding(stringBindingP, bindingP, stp);
   if (*stp != rpc_s_ok) {
      rpc_string_free(&stringBindingP, stp);
      printf("Can not get binding handle.\n");
      exit(1);
   }
}

static cmdebugProc(as)
  struct cmd_syndesc *as;
{
    handle_t h;
    register char *hostName;
    register struct hostent *thp;
    int longp;
    long addr;
    rpc_ns_handle_t import_context;
    error_status_t st;
    uuid_t nil_uuid;
    char *stringBinding;
    char cellName[100];
    long code;
    char myStringBinding[256];
    char *port;
    char *s;

    hostName = as->parms[0].items->data;
    uuid_create_nil(&nil_uuid, (unsigned32 *)&st);
    get_dfs_server(hostName, &h, &st); /* Get tkn server. */
    if ( st ) {
      printf("cmdebug: failed to locate dfs server\n");
      exit(1);
    }
    rpc_binding_to_string_binding(h, &stringBinding, &st);

    printf("Stringbinding is %s\n", stringBinding);
    sprintf(myStringBinding, "%s", stringBinding);
    s = strchr(myStringBinding,']');
    port = as->parms[1].items->data;
    strcpy(s,port);
    s +=4;
    *s++ = ']';
    *s = '\0';
    printf("Stringbinding is %s\n", myStringBinding);

    rpc_binding_from_string_binding(myStringBinding, &h, &st);

    if (as->parms[2].items) longp = 1;
    else longp = 0;
    code = PrintLocks(h, longp);
    if ( code ) {
      printf("PrintLocks returns non zero code\n");
    }
    code = PrintCacheEntries(h, longp);
    if ( code ) {
      printf("PrintCacheEntry returns non zero code\n");
    }
    return 0;
}

main(argc, argv)
int argc;
char **argv; {
    register struct cmd_syndesc *ts;

#ifdef	AFS_AIX32_ENV
    /*
     * The following signal action for AIX is necessary so that in case of a
     * crash (i.e. core is generated) we can include the user's data section
     * in the core dump. Unfortunately, by default, only a partial core is
     * generated which, in many cases, isn't too useful.
     */
    struct sigaction nsa;

    sigemptyset(&nsa.sa_mask);
    nsa.sa_handler = SIG_DFL;
    nsa.sa_flags = SA_FULLDUMP;
    sigaction(SIGSEGV, &nsa, NULL);
#endif

    ts = cmd_CreateSyntax((char *) 0, cmdebugProc, 0, "probe Cache Manager");
    cmd_AddParm(ts, "-servers", CMD_SINGLE, CMD_REQUIRED, "machine name");
    cmd_AddParm(ts, "-port", CMD_SINGLE, CMD_REQUIRED, "IP port");
    cmd_AddParm(ts, "-long", CMD_FLAG, CMD_OPTIONAL, "print all info");

    cmd_Dispatch(argc, argv);
    exit(0);
}
