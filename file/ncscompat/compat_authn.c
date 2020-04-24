/*
 * HISTORY
 * $Log: compat_authn.c,v $
BINRevision 1.1.2.1  1996/10/02  17:54:14  damon
BIN	Newest DFS from Transarc
BIN
 * $EndLog$
 */
/*
*/
/* Copyright (C) 1995 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <dcedfs/osi.h>
#include <dce/rpc.h>
#include <compat.h>
#include <pthread.h>
#include <string.h>

char *compat_authnToString(lvl, buff, bufflen)
unsigned32 lvl;
char *buff;
int bufflen;
{
    char *namep = NULL;

    /*
     * Do these as static strings because they're the same strings that
     * the commands accept as input.
     */

    switch (lvl) {
	case rpc_c_protect_level_none:
	    namep = "rpc_c_protect_level_none";
	    break;
	case rpc_c_protect_level_connect:
	    namep = "rpc_c_protect_level_connect";
	    break;
	case rpc_c_protect_level_call:
	    namep = "rpc_c_protect_level_call";
	    break;
	case rpc_c_protect_level_pkt:
	    namep = "rpc_c_protect_level_pkt";
	    break;
	case rpc_c_protect_level_pkt_integ:
	    namep = "rpc_c_protect_level_pkt_integ";
	    break;
	case rpc_c_protect_level_pkt_privacy:
	    namep = "rpc_c_protect_level_pkt_privacy";
	    break;
    }
    if (namep) {
	strncpy(buff, namep, bufflen);
	buff[bufflen-1] = '\0';
    } else {
	sprintf(buff, "level %d", lvl);
    }
    return &buff[0];	/* as a convenience */
}

int compat_nameToAuthnLevel(namestring, valp)
char *namestring;
unsigned32 *valp;
{
    signed32 thisVal;
    char *ptr;
    static char pref1[] = "rpc_c_protect_level_";
    static char pref2[] = "rpc_c_authn_level_";

    if (isdigit(namestring[0])) {
	thisVal = atoi(namestring);
	if (thisVal < 0)
	    return -1;
	switch (thisVal) {
	    case rpc_c_protect_level_default:
	    case rpc_c_protect_level_none:
	    case rpc_c_protect_level_connect:
	    case rpc_c_protect_level_call:
	    case rpc_c_protect_level_pkt:
	    case rpc_c_protect_level_pkt_integ:
	    case rpc_c_protect_level_pkt_privacy:
		*valp = (unsigned32)thisVal;
		return 0;
	    default:
		return -1;
	}
    } else {
	ptr = namestring;
	if (strncmp(ptr, pref1, sizeof(pref1)-1) == 0) {
	    ptr += (sizeof(pref1)-1);
	} else if (strncmp(ptr, pref2, sizeof(pref2)-1) == 0) {
	    ptr += (sizeof(pref2)-1);
	}
	if (strcmp(ptr, "default") == 0)
	    *valp = rpc_c_protect_level_default;
	else if (strcmp(ptr, "none") == 0)
	    *valp = rpc_c_protect_level_none;
	else if (strcmp(ptr, "connect") == 0)
	    *valp = rpc_c_protect_level_connect;
	else if (strcmp(ptr, "call") == 0)
	    *valp = rpc_c_protect_level_call;
	else if (strcmp(ptr, "pkt") == 0)
	    *valp = rpc_c_protect_level_pkt;
	else if (strcmp(ptr, "pkt_integ") == 0)
	    *valp = rpc_c_protect_level_pkt_integ;
	else if (strcmp(ptr, "pkt_integrity") == 0)
	    *valp = rpc_c_protect_level_pkt_integ;
	else if (strcmp(ptr, "pkt_privacy") == 0)
	    *valp = rpc_c_protect_level_pkt_privacy;
	else
	    return -1;
	return 0;
    }
}
