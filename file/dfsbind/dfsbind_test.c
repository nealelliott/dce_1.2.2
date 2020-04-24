/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfsbind_test.c,v $
 * Revision 1.1.81.1  1996/10/02  17:14:57  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:06:40  damon]
 *
 * $EndLog$
*/
/*
 *      Copyright (C) 1996, 1991 Transarc Corporation
 *      All rights reserved.
 */


/*
 * dfsbind_test.c 
 * 
 * Test program is to test one of dfsbind functions, ie., resolving pathnames
 * with CDS, without invoking the kernel cache manager service. 
 * Note: Only one of functions of dfsbind, GetBind is tested in this program.
 */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/ioctl.h>
#include <ctype.h>

#include <dcedfs/common_data.h>
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <dcedfs/cmd.h>
#include <dce/rpc.h>
#include <dce/dce_cf.h>
#include <dcedfs/fldb_proc.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsdsbmac.h>
#include <dfsdsbsvc.h>
#include <dfsdsbmsg.h>

#include <dcedfs/icl.h>
#include <bind_trace.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/dfsbind/dfsbind_test.c,v 1.1.81.1 1996/10/02 17:14:57 damon Exp $")

struct icl_set *bind_iclSetp = 0;

#define AFSBIND_BUFSIZE		4096

extern do_GetBind();
int Debugging;


dce_svc_handle_t   dsb_svc_handle;

void initialize_svc()
{
  error_status_t st;

  dsb_svc_handle = dce_svc_register(dsb_svc_table,  (idl_char *)"dsb",  &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot register svc 0x%lx\n", st);
  dce_svc_routing((unsigned char *) "NOTICE:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route NOTICE messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "WARNING:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route WARNING messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "ERROR:STDERR:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route ERROR messages  0x%lx\n", st);

  dfs_define_dsb_message_tables();

}


void SetupLog()
{
    struct icl_log *logp;
    long code;

    code = icl_CreateLog("bindtest", 1024, &logp);
    if (code == 0)
        code = icl_CreateSet("bindtest", logp, (struct icl_log *) 0, &bind_iclSetp);
    if (code)
        dce_svc_printf(DSB_S_WARNING_MSG, code);
}

main(argc, argv)
  int argc;
  char *argv[];

{
    char *p;
    char InLine[400];
    long osize, Dum;
    char inBuf[AFSBIND_BUFSIZE], outBuf[AFSBIND_BUFSIZE];
    char *inP, *outP, *tmpoutP;
    int ix, princCount, addrCount;
    long buffer;
    long replyCode;
    u_long TTL, Flags;
    long nameLen;
    afs_hyper_t newCellID;
    char newCellName[300], realCellName[300];
    struct sockaddr taddr;
    uuid_t uuid;
    char *uuidStr;
    unsigned long code=0;
    unsigned long varLen = 0;
    sigset_t		signalBlockSet;


    initialize_svc();

    /* 
     * block SIGUSR1 for all threads. The ICL thread that wiil be
     * waiting for it will clear its mask by using sigwait()
     */
    (void)sigemptyset(&signalBlockSet);
    (void)sigaddset(&signalBlockSet, SIGUSR1);
    (void)sigprocmask(SIG_BLOCK, &signalBlockSet, (sigset_t *)NULL);  

    /* set up ICL */
    code = icl_StartCatcher(/*waitInKernel*/0);
    if (code < 0)
	(void) dce_svc_printf(DSB_S_NO_ICL_SIGNAL_WATCHER_MSG, code, errno);
    else
	SetupLog();

    dce_svc_printf(DSB_S_TEST_NAMES_MSG);
    for (;;) {
	dce_svc_printf(DSB_S_CELL_NAME_MSG); fflush(stdout);
	p = gets(InLine);
	if (!p)
	{
	    if (!ferror(stdin))
		exit(0);
	    clearerr(stdin);	
	    continue;
	}
	inP = inBuf;
	Dum = 0;
	bcopy ((char *)&Dum, inP, sizeof(long));
	inP += sizeof(long);
	Dum = strlen(InLine)+1;
	bcopy ((char *)&Dum, inP, sizeof(long));
	inP += sizeof(long);
	bcopy (InLine, inP, Dum);
	inP += Dum;

	Dum = do_GetBind (inBuf, inP - inBuf, outBuf, &osize);
	if (Dum) {
	    dce_svc_printf(DSB_S_ERR_CODE_MSG, InLine, Dum);
	    continue;
	}
	else {
	    outP = outBuf;
	    bcopy (outP, (char *)&replyCode, sizeof(long)); /* always 0 */
	    outP += sizeof(long); osize -= sizeof(long);
	    bcopy (outP, (char *)&replyCode, sizeof(long)); 
	    outP += sizeof(long); osize -= sizeof(long);
	}
	if (replyCode == EINVAL)
	    continue;
	if (replyCode != 0 && replyCode != ENOENT && replyCode != EISDIR) {
	    dce_svc_printf(DSB_S_GETBIND_REPLYCODE_MSG, InLine, replyCode);
	    continue;
	}
	if (replyCode == ENOENT || replyCode == EISDIR) {
	    bcopy (outP, (char *)&TTL, sizeof(TTL));
	    outP += sizeof(TTL); osize -= sizeof(TTL);
	    bcopy (outP, (char *)&Flags, sizeof(Flags));
	    outP += sizeof(Flags); osize -= sizeof(Flags);
	    bcopy (outP, (char *)&varLen, sizeof(varLen));
	    outP += sizeof(varLen); osize -= sizeof(varLen);
	    if (*outP != '\0') {
		sscanf (outP, "%s", newCellName);
	    }
	    else
		newCellName[0] = '\0';

/*	    printf("do_GetBind (%s): %s; really `/.../%s', TTL %d, flags %x\n",
 *		   InLine, 
 *		   (replyCode == ENOENT ? "no such domain" : "no cell here"),
 *		   newCellName, TTL, Flags);
 */
		        if (replyCode == ENOENT)   {
		            dce_svc_printf(DSB_S_NO_DOMAIN_MSG, InLine, newCellName,TTL, Flags);
		        }
		        else   {
		            dce_svc_printf(DSB_S_NO_CELL_MSG, InLine, newCellName,TTL, Flags);
		        }
	} else {
                bcopy(outP, (char *)&buffer, sizeof(buffer)); 
		outP += sizeof(buffer); osize -= sizeof(buffer);
                bcopy(outP, (char *)&newCellID, sizeof(newCellID)); 
		outP += sizeof(newCellID); osize -= sizeof(newCellID);
                bcopy(outP, (char *)&TTL, sizeof(TTL)); 
		outP += sizeof(TTL); osize -= sizeof(TTL);
                bcopy(outP, (char *)&Flags, sizeof(Flags)); 
		outP += sizeof(Flags); osize -= sizeof(Flags);

		/* locate end of address-sets */
		tmpoutP = outP;

		for (princCount = 0; princCount < buffer; princCount++) {
		    for (addrCount = 0; ; addrCount++) {
			if (((struct sockaddr_in *)
			     tmpoutP)->sin_addr.s_addr == 0) {
			    /* address-set terminator */
			    if (addrCount <= 0 || addrCount > UBIK_MAXADDRS) {
				/* bad address-set format */
				dce_svc_printf(DSB_S_TEST_ADDR_LIST_BAD_MSG,
					       princCount, addrCount);
			    }
			    tmpoutP += sizeof(struct sockaddr_in);
			    break;
			}
			tmpoutP += sizeof(struct sockaddr_in);
		    }
		}

		if (*(tmpoutP + sizeof(uuid) + sizeof(varLen)) != '\0') {
		    sscanf (tmpoutP + sizeof(uuid) + sizeof(varLen),
			    "%s", newCellName);
		} else {
		    newCellName[0] = '\0';
		}

		bcopy (tmpoutP, (char *)&uuid, sizeof(uuid));

		uuid_to_string(&uuid, (unsigned char **)&uuidStr, &code);
		dce_printf(dsb_s_bindtest_ok1,
			   InLine, newCellName, TTL, Flags);

		dce_printf(dsb_s_bindtest_ok2,
			   uuidStr, AFS_HGETBOTH(newCellID), buffer);

		rpc_string_free ((unsigned char **)&uuidStr, &code);

		/* print address-sets */
		for (princCount = 0; princCount < buffer; princCount++) {
		    for (addrCount = 0; ; addrCount++) {
			if (((struct sockaddr_in *)
			     outP)->sin_addr.s_addr == 0) {
			    /* address-set terminator */
			    outP  += sizeof(struct sockaddr_in);
			    osize -= sizeof(struct sockaddr_in);
			    break;
			}

			bcopy(outP, (char *)&taddr, sizeof(taddr));
			outP  += sizeof(struct sockaddr_in);
			osize -= sizeof(struct sockaddr_in);

			dce_printf(dsb_s_bindtest_ok3, princCount);

			if (taddr.sa_family == AF_INET) {
			    dce_printf(dsb_s_bindtest_ok4,
				       ntohs(((struct sockaddr_in *)
					      &taddr)->sin_port),
				       inet_ntoa(((struct sockaddr_in *)
						  &taddr)->sin_addr));
			} else {
			    dce_printf(dsb_s_bindtest_ok5,
				       (unsigned short) taddr.sa_family,
				       (unsigned char) taddr.sa_data[0]);

			    for (ix = 1; ix < sizeof(taddr.sa_data); ++ix) {
				dce_printf(dsb_s_bindtest_ok6,
					   (unsigned char) taddr.sa_data[ix]);
			    }
			    dce_printf(dsb_s_bindtest_ok7);
			}
		    }
		}

		outP  += (sizeof(uuid) + sizeof(varLen) +
			  ((strlen(newCellName) + 1) * sizeof(char)));
		osize -= (sizeof(uuid) + sizeof(varLen) +
			  ((strlen(newCellName) + 1) * sizeof(char)));

		if (*outP != '\0') {
		    sscanf (outP, "%s", realCellName);
		} else {
		    realCellName[0] = '\0';
		}

		outP  += (strlen(realCellName) + 1) * sizeof(char);
		osize -= (strlen(realCellName) + 1) * sizeof(char);

		dce_svc_printf(DSB_S_TEST_PRINC_NAME_MSG);
		for (Dum = 0; Dum < buffer; ++Dum) {
		    dce_svc_printf(DSB_S_TEST_CELLNAME_MSG,
				   Dum, realCellName, outP);
		    outP  += (strlen(outP) + 1) * sizeof(char);
		    osize -= (strlen(outP) + 1) * sizeof(char);
		}
	    }
    }
}
