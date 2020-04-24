/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: bind_helper.c,v $
 * Revision 1.1.21.1  1996/10/02  17:14:50  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:06:37  damon]
 *
 * $EndLog$
 */
/*
 *      Copyright (C) 1996, 1991 Transarc Corporation
 *      All rights reserved.
 */

/*
 * bind_helper.c -- helper process resolves path names with CDS.
 *		    given the CDS flserver group path name, it
 *		    returns a packet containing the flserver
 *		    bindings and principal names along with the uuid.
 *
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
#include <dce/rgybase.h>
#include <dce/sec_login.h> /* used by LogTktInfo() */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsdsbmac.h>
#include <dfsdsbsvc.h>
#include <dfsdsbmsg.h>
#include <dcedfs/icl.h>
#include <bind_trace.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/dfsbind/bind_helper.c,v 1.1.21.1 1996/10/02 17:14:50 damon Exp $")

struct icl_set *bind_iclSetp;

#define FLDB_IFSPEC     (rpc_if_handle_t)VL__v4_0_c_ifspec

extern int Debugging;
unsigned long ttlmiss, ttlhit, ttldir;

#ifdef DEBUG
static void LogTktInfo();
#endif /* DEBUG */

void getFlags (cellnp, flags, exact)
char *cellnp;
long *flags;
int exact;
{
    static char *localCellName = 0;
    static error_status_t localCellNameStatus = 0;
    static int localCellNameLen = 0;
    int res;

    if (localCellName == 0 && localCellNameStatus == 0) {
	/* Initialize ourselves. */
	dce_cf_get_cell_name (&localCellName, &localCellNameStatus);
	if (localCellName != 0)
	    localCellNameLen = strlen(localCellName);
    }
    if (localCellNameStatus != dce_cf_st_ok) {
	*flags = 2;	/* assume not local */
    } else {
	if (exact) {
	    res = strcmp(cellnp, localCellName);
	} else {
	    res = strncmp(cellnp, localCellName, localCellNameLen);
	}
	if (res == 0) {
	    *flags = 1;
	} else {
	    *flags = 2;
	}
    }
}

int debugGetBind(outStr, printp, writep)
     char *outStr;
     char *printp;
     char *writep;
{
    long obufsz, Dum;
    char *outP;
    int ix;
    long buffer;
    long replyCode;
    u_long TTL, Flags;
    long nameLen;
    afs_hyper_t newCellID;
    char newCellName[300];
    char realCellName[300];
    struct sockaddr taddr;
    uuid_t uuid;
    char *uuidStr;
    unsigned long varLen;
    unsigned long code=0;
	    

    outP = outStr + sizeof(long);
    obufsz = writep - outStr;
    bcopy(outP, (char *)&replyCode, sizeof(long)); 
    outP += sizeof(long); obufsz -= sizeof(long);

    if (replyCode != 0 && replyCode != ENOENT && replyCode != EISDIR) {

	dce_svc_printf(DSB_S_SOME_REPLYCODE_MSG, printp, replyCode);

    } else if (replyCode == ENOENT || replyCode == EISDIR) {

	bcopy(outP, (char *)&TTL, sizeof(TTL)); 
	outP += sizeof(TTL); 
	obufsz -= sizeof(TTL);
	bcopy(outP, (char *)&Flags, sizeof(Flags)); 
	outP += sizeof(Flags); 
	obufsz -= sizeof(Flags);
	bcopy(outP, (char *)&varLen, sizeof(unsigned long));
	outP += sizeof(unsigned long);
	obufsz -= sizeof(unsigned long);
	if (*outP != '\0') {
	    sscanf (outP, "%s", newCellName);
	    outP += (strlen(newCellName)+1);
	    obufsz -= (strlen(newCellName)+1);
	}
	else
	    newCellName[0] = '\0';

/*	printf("do_GetBind (%s): really %s; really `%s',TTL %d, flags %x\n",
 *	       printp, (replyCode == ENOENT ? "no such domain" : 
 *			"no cell here"), newCellName, TTL, Flags);
 */
	if (replyCode == ENOENT)   {
	    dce_svc_printf(DSB_S_NO_SUCH_DOMAIN_MSG, printp, newCellName,TTL, Flags);
	}
	else   {
	    dce_svc_printf(DSB_S_NO_CELL_HERE_MSG, printp, newCellName,TTL, Flags);
	}

    } else if (replyCode == 0) {

	bcopy(outP, (char *)&buffer, sizeof(buffer)); 
	outP += sizeof(buffer); 
	obufsz -= sizeof(buffer);
	bcopy(outP, (char *)&newCellID, sizeof(newCellID)); 
	outP += sizeof(newCellID); 
	obufsz -= sizeof(newCellID);
	bcopy(outP, (char *)&TTL, sizeof(TTL)); 
	outP += sizeof(TTL); 
	obufsz -= sizeof(TTL);
	bcopy(outP, (char *)&Flags, sizeof(Flags)); 
	outP += sizeof(Flags); 
	obufsz -= sizeof(Flags);

	if (*(outP+(buffer*sizeof(struct sockaddr_in))+sizeof(uuid)+ \
sizeof(varLen)) != '\0') {
	    sscanf(outP + (buffer*sizeof(struct sockaddr_in)) + sizeof(uuid)+ \
sizeof(varLen), 
		   "%s", newCellName);
	}
	else
	   newCellName[0] = '\0';

	bcopy (outP + (buffer*sizeof(struct sockaddr_in)), 
	       (char *)&uuid, sizeof(uuid));
	
	uuid_to_string(&uuid, (unsigned char **)&uuidStr, (unsigned32 *)&code);

	dce_printf(dsb_s_bind_ok1, printp, newCellName, TTL, Flags, uuidStr);
	dce_printf(dsb_s_bind_ok2, AFS_HGETBOTH(newCellID), buffer);
	
	rpc_string_free ((unsigned char **)&uuidStr, (unsigned32 *)&code);

	for (Dum = 0; Dum < buffer; ++Dum) {
	    bcopy(outP, (char *)&taddr, sizeof(taddr)); 
	    outP += sizeof(taddr); 
	    obufsz -= sizeof(taddr);
	    dce_printf(dsb_s_bind_ok3, Dum);
	    if (taddr.sa_family == AF_INET) {
	      dce_printf(dsb_s_bind_ok4, ntohs(((struct sockaddr_in *) &taddr)->sin_port), 
		     inet_ntoa(((struct sockaddr_in *)&taddr)->sin_addr));
	    } 
	    else {
	       dce_printf(dsb_s_bind_ok5, (unsigned short) taddr.sa_family, 
		      (unsigned char) taddr.sa_data[0]);
	       for (ix = 1; ix < sizeof(taddr.sa_data); ++ix)
		   dce_printf(dsb_s_bind_ok6, (unsigned char) taddr.sa_data[ix]);
/*	       printf("\n");  */
	       dce_printf(dsb_s_bind_ok7);
	    } /* else */
	} /* for (Dum */

	outP += sizeof(uuid)+sizeof(varLen)+(strlen(newCellName)+1);
	if ( *outP != '\0') {
	     sscanf ( outP, "%s", realCellName);
	}
	else
	    realCellName[0] = '\0';

	outP += (strlen(realCellName)+1);
	obufsz -= (strlen(realCellName)+1);

	for (Dum = 0; Dum < buffer; ++Dum) {
	    dce_svc_printf(DSB_S_PRINC_NAME_MSG,Dum, realCellName, outP);
	    outP += (strlen(outP)+1);
	    obufsz -= (strlen(outP)+1);
	}
    } /* replyCode == 0 */
    return 0;
}


/* 
 * Request format of GetBind;
 *   long:   request size
 *   long:   opcode =  0
 *   long:   byteCountForCellName
 *   char:   cellName[]
 *
 */

/* 
 *  Response to get-cell opcode:
 *  long errcode
 *  long replyCode
 *
 * - when replyCode != 0 && replyCode != ENOENT && replyCode != EISDIR 
 *   it is returned as error code
 * - for replyCode == ENOENT or replyCode == EISDIR:
 *     long TTL (time to live)
 *     long Flags (future expansion)
 *     long nameLen (length of cell distinguished name)
 *     char cellName[] (the distinguished name, if any known)
 * - for replyCode == 0:
 *     long numAddrs (number of sockaddrs returned)
 *     afs_hyper_t newCellID (probably zero)
 *     long TTL (time to live)
 *     long Flags (afsd-style flags--local, nosetuid, etc.)
 *     long nameLen (length of cell distinguished name)
 *     all 'numAddrs' sockaddr_in structures
 *     uuid_t bindingUuid uuid from binding handle
 *     char cellName[] (the distinguished name)
 *
 *
 * Code semantics:
 * ENOENT means that there is no such domain as the probe, so the returned 
 * cellName will probably be null.
 * EISDIR means that there is such a domain as the probe, but it's not a cell. 
 *
 * The returned cellName will have any abbreviations expanded.
 * 0 means that the probe name corresponded to a cell.
 */

#include <dce/dnsclerk.h>
#include <dce/dnsmessage.h>
#define attrName_JunctionName           "DFSJunctionName"
#define attrName_JunctionVisible        "DFSJunctionVisible"
#define attrName_ServerId               "ServerId"
#define attrName_Test                   "DNS$Replicas"

static int BetterError(unsigned32 codeval, int defaultval)
{
    int retval;

    switch (codeval) {
	case rpc_s_unknown_authn_service:
	case rpc_s_binding_has_no_auth:
	case rpc_s_no_ns_permission:
	case rpc_s_authn_level_mismatch:
	case ept_s_cant_create:
	case ept_s_cant_access:
	case rpc_s_invalid_credentials:
	case rpc_s_auth_tkt_expired:
	case rpc_s_auth_tkt_nyv:
	case rpc_s_auth_repeat:
	case rpc_s_auth_not_us:
	case rpc_s_auth_badmatch:
	case rpc_s_auth_skew:
	case rpc_s_auth_badaddr:
	case rpc_s_auth_badversion:
	case rpc_s_auth_msg_type:
	case rpc_s_auth_modified:
	case rpc_s_auth_badorder:
	case rpc_s_auth_badkeyver:
	case rpc_s_auth_nokey:
	case rpc_s_auth_mut_fail:
	case rpc_s_auth_baddirection:
	case rpc_s_auth_method:
	case rpc_s_auth_badseq:
	case rpc_s_auth_inapp_cksum:
	case rpc_s_auth_field_toolong:
	case rpc_s_authn_challenge_malformed:
	case rpc_s_unsupported_auth_subtype:
	case sec_s_bad_nonce:
	case sec_priv_s_invalid_principal:
	case sec_priv_s_invalid_trust_path:
	    retval = EACCES;	/* Lots of auth failures: ``no permission'' */
	    break;
	case rpc_s_rpcd_comm_failure:
	    retval = ESRCH;	/* No rpcd: ``no such process'' */
	    break;
	case rpc_s_connect_timed_out:
	case rpc_s_call_timeout:
	    retval = ETIMEDOUT;	/* Known ``timed out'' */
	    break;
	case rpc_s_network_unreachable:
	    retval = ENETUNREACH;	/* Known ``net unreachable'' */
	    break;
	case rpc_s_connect_rejected:
	case rpc_s_loc_connect_aborted:
	    retval = ECONNABORTED;	/* Known ``connection aborted'' */
	    break;
	case rpc_s_rem_network_shutdown:
	case rpc_s_connect_closed_by_rem:
	    retval = ECONNRESET;	/* Known ``connection reset'' */
	    break;
	case rpc_s_rem_host_down:
	    retval = EHOSTDOWN;	/* Known ``host down'' */
	    break;
	case rpc_s_host_unreachable:
	    retval = EHOSTUNREACH;	/* Known ``host unreachable'' */
	    break;
	default:
	    retval = defaultval;
	    break;
    }
    icl_Trace3(bind_iclSetp, BIND_TRACE_MAYBE_BETTER_ERROR,
	       ICL_TYPE_LONG, (long) codeval,
	       ICL_TYPE_LONG, (long) defaultval,
	       ICL_TYPE_LONG, (long) retval);
    return (retval);
}

int do_GetBind(inStr, inSize, outStr, outSizeP)
     char *inStr;
     long inSize;
     char *outStr;
     long *outSizeP;
{
    struct ubik_client *aclient = (struct ubik_client *)0;
    uuid_t bindingUuid, newBindingUuid;
    unsigned_char_p_t resName = 0;
    unsigned_char_p_t unresName = 0;
    unsigned32 code = 0, Flags = 0, code2, unusedCode;
    afs_hyper_t cellId;
    struct sockaddr_in *sockArray=(struct sockaddr_in *)0;
    long  opCode, cellInfoSize, sockCount, princCount, attrBufLen, i, j, k;
    long ErrnoVal = 0, return_code = 0, gotUuid = 0;
    dns_handle dnsHandle;
    dnsFlagStat_t flags;
    enum dnsAttrType attrType;
    u_char attrValue[1000];
    char *writep, *printp, **princArray=(char **)0;
    unsigned long varLen;
    char *storep;
    sec_rgy_name_t cellName, prinName;
    char *altStr=(char *)0;
    char junct[BUFSIZ];
    short sizeJunct = BUFSIZ;
    char *newNameP, *fullNewNameP, *newCellNameP;
    static char preFix[] = "/.../";

    extern dnsFlagStat_t *init_flagstatus();

    AFS_hzero(cellId);

    writep = outStr;
    
    bcopy (inStr, (char *)&opCode, sizeof(long));
    inStr += sizeof(long); 
    inSize -= sizeof(long);

    bcopy (inStr, (char *)&cellInfoSize, sizeof(long)); 
    inStr += sizeof(long); 
    inSize -= sizeof(long); 

    if (Debugging) {
      printp = (char *)malloc(inSize);
      if (printp != (char *)0)
	bcopy (inStr, printp, inSize);
    }

    /* Default the value to be returned */
    newNameP = inStr;
    inStr[inSize-1] = '\0';	/* ensure null-termination */
    inStr -= strlen(preFix); 
    inSize += strlen(preFix);
    bcopy (preFix, inStr, strlen(preFix));
    fullNewNameP = inStr;	/* with preFix prepended */

    if (opCode != 0 && opCode != 1) {
	ErrnoVal = EINVAL;
	bcopy ((char *)&ErrnoVal, writep, sizeof(long)); 
	writep += sizeof(long);
	*outSizeP = writep - outStr;
	return EINVAL;
    }

    icl_Trace1(bind_iclSetp,BIND_TRACE_SECID_ENTRY, ICL_TYPE_STRING, inStr);
    sec_id_parse_name (sec_rgy_default_handle, inStr, cellName,
                       (uuid_t *)NULL, prinName,
                       (uuid_t *)NULL, &code);
    icl_Trace1(bind_iclSetp,BIND_TRACE_SECID_EXIT,ICL_TYPE_LONG,code);
    if (code == error_status_ok) { /*d*/
	if (strcmp((char *)prinName, "") == 0) { /*c*/
	    /* Catch cell-alias references */
	    if (strncmp((char *)cellName, inStr, inSize) != 0) {
		fullNewNameP = (char*)cellName;
		newNameP = &fullNewNameP[strlen(preFix)];
		inStr = fullNewNameP;
	    }
	    icl_Trace0(bind_iclSetp, BIND_TRACE_GETJUNC_ENTRY);
	    dfs_GetJunctionName ((char *)cellName, 1, junct, sizeJunct, &code);
	    icl_Trace1(bind_iclSetp, BIND_TRACE_GETJUNC_EXIT,
		       ICL_TYPE_LONG, code);
	    if (code) {
		ErrnoVal = BetterError(code, ETIMEDOUT);
	    }
	    else if (strcmp(junct, "") != 0) { /*b*/ /* wrong annotation */
		altStr = (char *)malloc(strlen((char *)cellName)+strlen(junct)+1);
		if (altStr == (char *)0)
		{
		    icl_Trace0(bind_iclSetp,BIND_TRACE_STR_MALLOC_ERR);
		    ErrnoVal = ENOMEM;
		}
		else { /*a*/
		    strcpy(altStr, (char *)cellName);
		    strcat(altStr, junct);
		    inStr = altStr;
		} /*a*/
	    } /*b*/
	} /*c*/
    } /*d*/
    else if (code != sec_rgy_object_not_found)
        ErrnoVal = BetterError(code, EINVAL);

    /* 
     * if sec_id_parse_name() fails with object not found,
     * try to resolve it as a partial name anyway.
     */
    if (ErrnoVal == 0) {
	icl_Trace0(bind_iclSetp, BIND_TRACE_NS_INQ_RES_ENTRY);
	rpc_ns_entry_inq_resolution ( rpc_c_ns_syntax_dce,
				     (unsigned_char_p_t)inStr,
				     &resName,
				     &unresName,
				     &code2);
	icl_Trace1(bind_iclSetp, BIND_TRACE_NS_INQ_RES_EXIT,
		   ICL_TYPE_LONG, code2);
	
	switch (code2) {
	case rpc_s_ok:
	    if (code == error_status_ok) /* code from sec_id_parse_name */
	        ErrnoVal = 0;
	    else
	        ErrnoVal = EISDIR;
	    break;
	case DNS_ROOTLOST:
	    /* 
	     * apparently this error code can be returned if GDAD isn't runnning.
	     * Might as well map it into ENOENT, as well.
	     */
	    ErrnoVal = ENOENT;
	    break;
	case rpc_s_partial_results:
	    if (Debugging)
		dce_svc_printf(DSB_S_PARTIAL_RESULTS_MSG, resName, unresName);
	    if ( strcmp ((char *)unresName, "") ) /* part of the name unresolved */
		ErrnoVal = ENOENT;	/* always return ENOENT here */
	    else if ( !strcmp ((char *)resName, inStr ))  /* full name resolved */
		ErrnoVal = EISDIR;
	    break;
	case rpc_s_underspecified_name:
	    if (Debugging)
		dce_svc_printf(DSB_S_UNDER_SPECIFIED_MSG, resName, unresName);
	    ErrnoVal = EISDIR;
	    break;
	case rpc_s_incomplete_name:
	case rpc_s_invalid_name_syntax:
	    ErrnoVal = EINVAL;
	    break;
	default:
	    ErrnoVal = BetterError(code2, ETIMEDOUT);
	    break;
	}
	if (resName) 
	    rpc_string_free (&resName, &unusedCode);
	if (unresName) 
	    rpc_string_free (&unresName, &unusedCode);
	resName = unresName = (unsigned_char_p_t)0;
    }

    if (ErrnoVal == 0) {
	/* Initialize local auth context here */
	icl_Trace0(bind_iclSetp, BIND_TRACE_UBIK_CLINIT_ENTRY);

	code = ubik_ClientInit ( FLDB_IFSPEC, (unsigned_char_t *)inStr, 
				(unsigned_char_t *)"subsys/dce/dfs-fs-servers", 
				0, 0, 1,
				(struct ubik_client **)&aclient );
	icl_Trace1(bind_iclSetp, BIND_TRACE_UBIK_CLINIT_EXIT,
		   ICL_TYPE_LONG, code);

	switch (code) {
	case rpc_s_ok:
	    ErrnoVal = 0;
	    break;
	case rpc_s_invalid_binding: 
	    ErrnoVal = EINVAL;
	    break;
	case rpc_s_no_protseqs:
	    ErrnoVal = ENOPROTOOPT;
	    break;
	case rpc_s_invalid_name_syntax:
	    ErrnoVal = EINVAL;
	    break;
	case rpc_s_unsupported_name_syntax:
	    ErrnoVal = EINVAL;
	    break;
	case rpc_s_no_more_bindings:
	case rpc_s_entry_not_found:
	case rpc_s_group_not_found:
	    init_flagstatus(&flags);
	    dnsHandle = dnsGetHandle();
	    if (!dnsHandle)
	    {
		icl_Trace0(bind_iclSetp,BIND_TRACE_DNSHANDLE_ERR);
		ErrnoVal = EINVAL;
		break;
	    }
	    icl_Trace0(bind_iclSetp, BIND_TRACE_DNSENUMATTR_ENTRY);
	    code2 = dnsEnumAttr ((dns_full_name_t *)inStr,
				 dnsHandle, dnsDir, &attrType,
				 0, 0, 0, &flags);
	    icl_Trace1(bind_iclSetp, BIND_TRACE_DNSENUMATTR_EXIT,
		       ICL_TYPE_LONG, code2);
	    switch (code2) {
	    case DNS_EMPTY:
		ErrnoVal = EISDIR;
		break;
	    case DNS_ERROR:
		ErrnoVal = ((code == rpc_s_no_more_bindings) ?
			    ETIMEDOUT : ENOENT);
		break;
	    case DNS_SUCCESS:
		ErrnoVal = EISDIR;
		break;
	    case DNS_PENDING:
		ErrnoVal = ETIMEDOUT;
		break;
	    default:
		ErrnoVal = BetterError(code2, ETIMEDOUT);
		break;
	    }
	    dnsFreeHandle(dnsHandle);
	    break;
	case rpc_s_no_memory:
	    ErrnoVal = ENOMEM;
	    break;
	case rpc_s_comm_failure:
	    ErrnoVal = ETIMEDOUT;
	    break;
	case rpc_s_auth_tkt_expired:
	    ErrnoVal = ETIMEDOUT;
	    break;
	case UNOSERVERS:
	    icl_Trace0(bind_iclSetp, BIND_TRACE_NO_SERVER_AVAILABLE);
	    ErrnoVal = EISDIR;
	    break;
	default:
	    ErrnoVal = BetterError(code, code);
	}
    }

    if (ErrnoVal == 0) {
	/* Alloc the sockaddr/principal arrays beforehand to avoid problems */
	sockArray = ((struct sockaddr_in *)
		     malloc((aclient->nServers * (UBIK_MAXADDRS + 1)) *
			    sizeof(struct sockaddr_in)));

	princArray = (char **) malloc(aclient->nServers * sizeof(char *));

	if (!sockArray || !princArray) {
	    ErrnoVal = ENOMEM;
	} else {
	    princCount = sockCount = 0;

	    for (i = 0; i < aclient->nServers && ErrnoVal == 0; i++)  {
		/* check obj-UUID for all princ's bindings; get sockaddrs */

		for (k = 0; k < UBIK_MAXADDRS && aclient->conns[i][k]; k++) {
		    /* at least one binding; record principal name */
		    if (k == 0) {
			princArray[princCount++] = aclient->principals[i];
		    }

		    icl_Trace1(bind_iclSetp, BIND_TRACE_RPC_BIND_INQ_OBJ_ENTRY,
			       ICL_TYPE_POINTER, aclient->conns[i][k]);

		    rpc_binding_inq_object(aclient->conns[i][k],
					   &newBindingUuid, &code);

		    icl_Trace2(bind_iclSetp, BIND_TRACE_RPC_BIND_INQ_OBJ_EXIT,
			       ICL_TYPE_POINTER, aclient->conns[i][k],
			       ICL_TYPE_LONG, code);

		    if (code != rpc_s_ok) {
			/* can't get object UUID, return error */
			ErrnoVal = BetterError(code, EINVAL);
			break;
		    }

		    if (!gotUuid) {
			bcopy((char *)&newBindingUuid, (char *)&bindingUuid,
			      sizeof(bindingUuid));
			gotUuid = 1;
		    }

		    if (bcmp((char *)&bindingUuid, (char *)&newBindingUuid,
			     sizeof(bindingUuid)) != 0) {
			/* UUIDs not the same for all bindings */
			ErrnoVal = EINVAL;
			break;
		    }

		    /* extract sockaddr from binding */
		    icl_Trace1(bind_iclSetp, BIND_TRACE_BIND_SOCKADDR_ENTRY,
			       ICL_TYPE_POINTER, aclient->conns[i][k]);

		    code = rpcx_binding_to_sockaddr(aclient->conns[i][k],
						    &sockArray[sockCount++]);

		    icl_Trace2(bind_iclSetp, BIND_TRACE_BIND_SOCKADDR_EXIT,
			       ICL_TYPE_POINTER, aclient->conns[i][k],
			       ICL_TYPE_LONG, code);

		    if (code) {
			/* error in sockaddr generation, abort */
			ErrnoVal = BetterError(code, EINVAL);
			break;
		    }
		} /* for k */

		/* null-terminate principal's address-set, if not empty */
		if (k > 0) {
		    bzero((char *)&sockArray[sockCount++],
			  sizeof(struct sockaddr_in));
		}
	    } /* for i */

	    /* indicate error if no bindings retrieved */
	    if (ErrnoVal == 0 && princCount <= 0) {
		ErrnoVal = ENOENT;
	    }
	}
    }

    /* Start writing output */

    bcopy ((char *)&return_code, writep, sizeof(long)); /* Always zero */
    writep += sizeof(long);

    if (ErrnoVal == 0) {
	bcopy ((char *)&ErrnoVal, writep, sizeof(long)); /* put replyCode */
	writep += sizeof(long);

	bcopy ((char *)&princCount, writep, sizeof(long)); /* put fl count */
	writep += sizeof(long);

	bcopy ((char *)&cellId, writep, sizeof(afs_hyper_t)); /* put Cell ID */
	writep += sizeof(afs_hyper_t);

	bcopy ((char *)&ttlhit, writep, sizeof(ttlhit)); /* put TTL */
	writep += sizeof(u_long);

	if (aclient->cellName && aclient->cellName[0] != '\0') {
	    newCellNameP = aclient->cellName;
	    getFlags (newCellNameP, &Flags, 1);
	} else {
	    newCellNameP = fullNewNameP;
	    getFlags (newCellNameP, &Flags, 0);
	}
	bcopy ((char *)&Flags, writep, sizeof(u_long)); /* put styles */
	writep += sizeof(u_long);

	for (i = 0; i < sockCount; i++) { /* put all sock-addr sets */
	    bcopy ((char *)&sockArray[i], writep, sizeof(struct sockaddr_in));
	    writep += sizeof(struct sockaddr_in);
	}

	if (gotUuid)
	    bcopy ((char *)&bindingUuid, writep, sizeof(bindingUuid));
	else
	    bzero(writep, sizeof(bindingUuid));
	writep += sizeof(bindingUuid);

	storep = writep;
	writep += sizeof(unsigned long); /* skip for varLen */

	bcopy (newNameP, writep, (strlen(newNameP)+1)); 
	writep += (strlen(newNameP)+1);

	bcopy (newCellNameP, writep, (strlen(newCellNameP)+1));
	writep += (strlen(newCellNameP)+1);

	for (i = 0; i < princCount; i++) {
	    bcopy (princArray[i], writep,
		   (strlen(princArray[i])+1));
	    writep += (strlen(princArray[i])+1);
	}
	varLen = writep - storep - sizeof(unsigned long);
	bcopy((char *)&varLen, storep, sizeof(unsigned long));

    } else if (ErrnoVal == ENOENT || ErrnoVal == EISDIR) {
#ifdef DEBUG
	LogTktInfo();
#endif /* DEBUG */
	bcopy ((char *)&ErrnoVal, writep, sizeof(long)); 
	writep += sizeof(long);

	if (ErrnoVal == EISDIR)
	    bcopy ((char *)&ttldir, writep, sizeof(ttldir)); 
	else
	    bcopy ((char *)&ttlmiss, writep, sizeof(ttlmiss));
	writep += sizeof(u_long);

	getFlags (fullNewNameP, &Flags, 0);
	bcopy ((char *)&Flags, writep, sizeof(u_long));
	writep += sizeof(u_long);

	storep = writep;
	writep += sizeof(unsigned long);

	bcopy (newNameP, writep, (strlen(newNameP)+1)); 
	writep += (strlen(newNameP)+1);

	varLen = writep - storep - sizeof(unsigned long);
	bcopy((char *)&varLen, storep, sizeof(unsigned long));

    } else {
#ifdef DEBUG
	LogTktInfo();
#endif /* DEBUG */

	bcopy ((char *)&ErrnoVal, writep, sizeof(long)); 
	writep += sizeof(long);
    }	

    *outSizeP = writep - outStr;

    /* carefully free up resources */
    if (altStr) (void)free(altStr);
    if (princArray) (void)free(princArray);
    if (sockArray) (void)free(sockArray);
    if ( aclient ) ubik_ClientFree(aclient);

    if (Debugging && printp != (char *)0) {
       debugGetBind(outStr, printp, writep);
       free(printp);
     }
    return 0;
}	

dns_opq_fname partialRes; /* partial results from the API returned here */

dnsFlagStat_t *init_flagstatus (fp)
  dnsFlagStat_t *fp;
{                                   /* -1 lines not needed. just tidy. */

    int secs;                           /* the seconds field of the wait */
    
    fp->fsMaybeMore     = 0;            /* no value here yet */
    fp->fsConf          = dnsConfLow;         /* current confidence */
    fp->fsWait          = secs;         /* current timeout wait */
    fp->fsDontCache     = 0;            /* clerk to cache or not */
    fp->fsUser[0]       = 0;            /* synchronous operation only */
    fp->fsUser[1]       = 0;            /* synchronous operation only */
    fp->fsUser[2]       = 0;            /* synchronous operation only */
    fp->fsUser[3]       = 0;            /* synchronous operation only */
    fp->fsUser[4]       = 0;            /* synchronous operation only */
    fp->fsNameType = dnsStrDCE;         /* default to opaque names */
    fp->fsUnAuth = 1;                   /* default to authenticated access */
    fp->fsTrustAll = 1;                 /* default to not trust any server */
    fp->fsCacheTimeout  = 0;            /* ok to use old stuff in cache */
    fp->fsPartialRes    = &partialRes;  /* return partial results */
    fp->fsResLength     = sizeof(partialRes);
    fp->fsOutLink       = -1;           /* no value here yet */
    fp->fsFromCache     = -1;           /* no value here yet */
    fp->fsLocStat       = -1;           /* no value here yet */
    fp->fsDNSStat       = -1;           /* no value here yet */
    fp->fsOpqDNS[0]     = -1;           /* no value here yet */
    fp->fsOpqDNS[1]     = -1;           /* no value here yet */
    fp->fsOpqDNS[2]     = -1;           /* no value here yet */
    fp->fsOpqDNS[3]     = -1;           /* no value here yet */
    fp->fsOpqDNS[4]     = -1;           /* no value here yet */
    return(fp);                         /* maybe the caller wants to use it */
}


#ifdef DEBUG
static void LogTktInfo()
{
    char *krb;
    error_status_t code;
    sec_login_handle_t login;
    sec_login_net_info_t info;
    extern char *getenv();

    sec_login_get_current_context(&login, &code);
    if (code != error_status_ok)
	return;

    sec_login_inquire_net_info(login, &info, &code);
    if (code != error_status_ok)
	return;

    krb = getenv("KRB5CCNAME");
    icl_Trace2(bind_iclSetp, BIND_TRACE_AUTH_INFO,
	       ICL_TYPE_STRING, (char *)info.pac.principal.name, 
	       ICL_TYPE_STRING, ctime((time_t *)&info.identity_expiration_date));
    if (krb && *krb)
	icl_Trace1(bind_iclSetp, BIND_TRACE_CRED_INFO, ICL_TYPE_STRING, krb);

    sec_login_free_net_info(&info);
    return;
}
#endif /* DEBUG */
