/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1996, 1991 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/common_data.h>
#include <time.h>
#include <dcedfs/osi_net.h>

#include <dce/rpc.h>

#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>

#include <fldb_proc.h>

#include <flserver.h>
#include <flclient.h>
#include <fl_opcodes.h>
#include "flinternal.h"

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfslsvmac.h>
#include <dfslsvsvc.h>
#include <dfslsvmsg.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/flserver/RCS/flclient.c,v 4.20 1996/03/04 19:07:20 ota Exp $")

dce_svc_handle_t   lsv_svc_handle;

void initialize_svc()
{
  error_status_t st;

  lsv_svc_handle = dce_svc_register(lsv_svc_table,  (idl_char *)"lsv",  &st);
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

  dce_msg_define_msg_table(lsv__table,
        sizeof lsv__table / sizeof lsv__table[0], &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Warning Cannot register lsv incore message table 0x%lx\n", st);
}



static char *opcode_names[VL_OPCODE_RANGE] = {
    /* 501 */    "CreateEntry",
    /* 502 */    "DeleteEntry",
    /* 503 */    "GetEntryByID",
    /* 504 */    "GetEntryByName",
    /* 505 */    "GetNewVolumeId",
    /* 506 */    "ReplaceEntry",
    /* 507 */    "(OLD)UpdateEntry",
    /* 508 */    "SetLock",
    /* 509 */    "ReleaseLock",
    /* 510 */    "ListEntry",
    /* 511 */    "(OLD)ListAttributes",
    /* 512 */    "(OLD)LinkedList",
    /* 513 */    "GetStats",
    /* 514 */    "Probe",
    /* 515 */    "AddAddress",
    /* 516 */    "RemoveAddress",
    /* 517 */    "ChangeAddress",
    /* 518 */    "(OLD)SetPrincipal",
    /* 519 */    "GetCellInfo",
    /* 520 */    "GetNextServersByID",
    /* 521 */    "GetNextServersByName",
    /* 522 */    "GetSiteInfo",
    /* 523 */    "GenerateSites",
    /* 524 */    "ListByAttributes",
    /* 525 */    "GetNewVolumeIds",
    /* 526 */    "CreateServer",
    /* 527 */    "GetCEntryByID",
    /* 528 */    "GetCEntryByName",
    /* 529 */    "GetCNextServersByID",
    /* 530 */    "GetCNextServersByName",
    /* 531 */    "ExpandSiteCookie",
    /* 532 */    "AlterServer",
    /* 533 */    "GetServerInterfaces",
};

struct ubik_client *cstruct;
struct sockaddr_in *serverAddrs[UBIK_MAXSERVERS];
char *(args[50]);

static long fl_Initialize(auth, nsGroupP)
     int auth;
     char *nsGroupP;
{
    long code = 0;
    long i;

    /*
     ************ Init the rpc stuff here
     */

    code = ubik_ClientInit((rpc_if_handle_t) VL__v4_0_c_ifspec,
                           (unsigned_char_t *)nsGroupP,
			   (unsigned_char_t *)"subsys/dce/dfs-fs-servers",
			   0, 0, 1,
			   (struct ubik_client **)&cstruct);

    if (code)
	dce_fprintf(stderr, lsv_s_ubik_client_init_failed);

    return(code);;
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

static void ScanHyper(char **argp, afs_hyper_t *h)
{
    long high, low;
    sscanf(argp[0], "%d", &high);
    sscanf(argp[1], "%d", &low);
    AFS_hset64(*h, high, low);
}

static void fill_entry(entry, argp, nargs)
vldbentry *entry;
char **argp;
int nargs;
{
    char *name;
    int i;

    name = &argp[0][0];
    ++argp, --nargs;
    sscanf(&(*argp)[0], "%d", &entry->volumeType);
    ++argp, --nargs;
    sscanf(&(*argp)[0], "%d", &entry->nServers);
    strcpy((char *)entry->name, name);
    for (i=0; i < entry->nServers; i++) {
	++argp, --nargs;
	FillafsNetAddr(&entry->siteAddr[i], *argp);
    }
    for (i=0; i < entry->nServers; i++) {
	++argp, --nargs;
	sscanf(&(*argp)[0], "%d", &entry->sitePartition[i]);
    }
    for (i=0; i < entry->nServers; i++) {
	++argp, --nargs;
	sscanf(&(*argp)[0], "%x", &entry->siteFlags[i]);
    }
    for (i=0; i < entry->nServers; i++) {
	++argp, --nargs;
	sscanf(&(*argp)[0], "%d", &entry->sitemaxReplicaLatency[i]);
    }
    for (i=0; i < MAXVOLTYPES; i++) {
	++argp, --nargs;
	sscanf(&(*argp)[0], "%x", &entry->VolTypes[i]);
	if (entry->VolTypes[i] == ((unsigned long) -1)) {
	    entry->VolTypes[i] = 0;
	    AFS_hzero(entry->VolIDs[i]);
	    break;
	}
	++argp, --nargs;
	ScanHyper(argp, &entry->VolIDs[i]);
	++argp, --nargs;
    }
    ++argp, --nargs;
    sscanf(&(*argp)[0], "%x", &entry->flags);
    ++argp, --nargs;
    ScanHyper(argp, &entry->cloneId);
    ++argp, --nargs;
    ++argp, --nargs;
    sscanf(&(*argp)[0], "%d", &entry->maxTotalLatency);
    ++argp, --nargs;
    sscanf(&(*argp)[0], "%d", &entry->hardMaxTotalLatency);
    ++argp, --nargs;
    sscanf(&(*argp)[0], "%d", &entry->minimumPounceDally);
    ++argp, --nargs;
    sscanf(&(*argp)[0], "%d", &entry->defaultMaxReplicaLatency);
    ++argp, --nargs;
    sscanf(&(*argp)[0], "%d", &entry->reclaimDally);
}

static void PrintafsNetAddr(sockP)
afsNetAddr *sockP;
{/* so print it in the same format. */
    int Ix, BigIx;

    dce_printf( lsv_s_sock_type, (unsigned short) sockP->type, (unsigned char) sockP->data[0]);
    for (BigIx = sizeof(sockP->data)-1; BigIx > 0; --BigIx)
	if (sockP->data[BigIx] != 0) break;
    for (Ix = 1; Ix <= BigIx; ++Ix) dce_printf( lsv_s_sock_data, (unsigned char) sockP->data[Ix]);
}

static fill_listbyattributes_entry(entry, argp, nargs)
VldbListByAttributes *entry;
char **argp;
int nargs;
{
    entry->Mask = 0;
    FillafsNetAddr(&entry->site, *argp);
    if (entry->site.type != ((unsigned short) -1))
	entry->Mask |= VLLIST_SITE;
    ++argp, --nargs;
    sscanf(&(*argp)[0], "%d", &entry->partition);
    if (entry->partition != -1)
	entry->Mask |= VLLIST_PARTITION;
    ++argp, --nargs;
    sscanf(&(*argp)[0], "%x", &entry->volumetype);
    if (entry->volumetype != -1)
	entry->Mask |= VLLIST_VOLUMETYPE;
    ++argp, --nargs;
    ScanHyper(argp, &entry->volumeid);
    ++argp, --nargs;
    if (!AFS_hiszero(entry->volumeid))
	entry->Mask |= VLLIST_VOLUMEID;
    ++argp, --nargs;
    sscanf(&(*argp)[0], "%x", &entry->flag);
    if (entry->flag != -1)
	entry->Mask |= VLLIST_FLAG;
 }

static void get_uuid(uuidp, ptr)
afsUUID *uuidp;
register char *ptr;
{
    register char *x = (char *) uuidp;
    register char y;
    register int res, state;
    register unsigned char buf;

    bzero((char *)uuidp, sizeof(afsUUID));
    state = 0; buf = 0;
    for (; *ptr != '\0'; ++ptr) {
	if (isascii(*ptr)) {
	    y = *ptr;
	    if (isdigit(y) || isalpha(y)) {
		if (isdigit(y)) {
		    res = y - '0';
		} else {
		    if (isupper(y)) y = tolower(y);
		    if (y >= 'a' && y <= 'f') {
			res = y - 'a' + 0xa;
		    } else res = -1;
		}
		if (res >= 0) {
		    buf <<= 4;
		    buf |= y;
		    ++state;
		    if (state == 2) {
			*x++ = buf;
			buf = 0; state = 0;
		    }
		}
	    }
	}
    }
}

static void fill_siteAlter_entry(entry, argp, nargs)
struct siteAlter *entry;
char **argp;
int nargs;
{
    entry->Mask = 0;
    get_uuid(&entry->Owner, *argp);
    if (entry->Owner.time_low != 0 || entry->Owner.time_mid != 0)
	entry->Mask |= SITEALTER_OWNER;
    ++argp, --nargs;
    get_uuid(&entry->ObjID, *argp);
    if (entry->ObjID.time_low != 0 || entry->ObjID.time_mid != 0)
	entry->Mask |= SITEALTER_OBJID;
    ++argp, --nargs;
    sscanf(&(*argp)[0], "%d", &entry->CreationQuota);
    if (entry->CreationQuota != (unsigned long) -1)
	entry->Mask |= SITEALTER_CREATIONQUOTA;
    ++argp, --nargs;
    sscanf(&(*argp)[0], "%d", &entry->CreationUses);
    if (entry->CreationUses != (unsigned long) -1)
	entry->Mask |= SITEALTER_CREATIONUSES;
    ++argp, --nargs;
    if (nargs > 0) {
	strncpy((char *)&entry->KerbPrin[0], *argp, MAXKPRINCIPALLEN);
	entry->Mask |= SITEALTER_PRINCIPAL;
    }
}

static void fill_siteDesc_entry(entry, argp, nargs)
struct siteDesc *entry;
char **argp;
int nargs;
{
    int nadr, ix;

    bzero((char *)entry, sizeof(struct siteDesc));
    for (ix = 0; ix < ADDRSINSITE; ++ix)
	entry->Addr[ix].type = ((unsigned short) -1);
    sscanf(&(*argp)[0], "%d", &nadr);
    if (nadr > ADDRSINSITE) {
	dce_printf( lsv_s_too_many_addresses, nadr, ADDRSINSITE, nadr - ADDRSINSITE);
    }
    ++argp, --nargs;
    for (ix = 0; ix < nadr; ++ix) {
	if (ix < ADDRSINSITE) FillafsNetAddr(&entry->Addr[ix], *argp);
	++argp, --nargs;
    }
    for (; ix < ADDRSINSITE; ++ix) entry->Addr[ix].type = (unsigned short) -1;
    strncpy((char *)&entry->KerbPrin[0], *argp, MAXKPRINCIPALLEN);
    ++argp, --nargs;
    get_uuid(&entry->Owner, *argp);
    ++argp, --nargs;
    get_uuid(&entry->ObjID, *argp);
    ++argp, --nargs;
    sscanf(&(*argp)[0], "%d", &entry->CreationQuota);
 }

static void printUuid(uuidp)
afsUUID *uuidp;
{
    register unsigned char *x = (unsigned char *) uuidp;
    register int ctr;

    for (ctr = sizeof(afsUUID); ctr > 0; --ctr) {
	if (*x++ != 0) break;
    }
    if (ctr == 0) {
	fputs("<uuidNil>", stdout);
    } else {
	x = (unsigned char *) uuidp;
	for (ctr = sizeof(afsUUID); ctr > 0; --ctr) {
	    dce_printf( lsv_s_afsuuid, *x++);
	}
    }
}

static void display_siteAlter(entryp)
struct siteAlter *entryp;
{
    if (entryp->Mask & SITEALTER_PRINCIPAL) {
	dce_printf( lsv_s_new_principal_prefix, &entryp->KerbPrin[0]);
    }
    if (entryp->Mask & SITEALTER_OWNER) {
	dce_printf( lsv_s_newowner_id); printUuid(&entryp->Owner); putchar('\n');
    }
    if (entryp->Mask & SITEALTER_OBJID) {
	dce_printf( lsv_s_newobject_id); printUuid(&entryp->ObjID); putchar('\n');
    }
    if (entryp->Mask & SITEALTER_CREATIONQUOTA) {
	dce_printf( lsv_s_newquota_total, entryp->CreationQuota);
    }
    if (entryp->Mask & SITEALTER_CREATIONUSES) {
	dce_printf( lsv_s_new_usecount, entryp->CreationUses);
    }
}

static void display_siteDesc(descp)
struct siteDesc *descp;
{/* Display a siteDesc structure */
    int i;

    for (i = 0; i < ADDRSINSITE && descp->Addr[i].type != ((unsigned short) -1); ++i) {
	dce_printf( lsv_s_site_descriptor, i);
	PrintafsNetAddr(&descp->Addr[i]);
	putchar('\n');
    }
    dce_printf( lsv_s_display_principal, &descp->KerbPrin[0]);
    dce_printf( lsv_s_display_total_quota, descp->CreationQuota);
    dce_printf( lsv_s_display_current_uses, descp->CreationUses);
    dce_printf( lsv_s_display_owner); printUuid(&descp->Owner); putchar('\n');
    dce_printf( lsv_s_display_objid); printUuid(&descp->ObjID);  putchar('\n');
}

static char *voltype_string(type)
u_long type;
{
    static char tbuff[20];
    int hitype;
    char *dcesptr;

    hitype = (type >> 16) & 0xffff;
    if ((type & 0xffff) == 0) {
	switch (hitype) {
	    case RWVOL:
		return "read/write";
	    case ROVOL:
		return "readonly";
	    case BACKVOL:
		return "backup";
	    default:
		break;
	}
    }

    dcesptr = (char *)dce_sprintf(lsv_s_type_not_known, type);
    strcpy(tbuff , dcesptr);
    free(dcesptr);
    return tbuff;
}

static display_listbyattributes_entry(entry, error)
VldbListByAttributes *entry;
int			    error;
{
    if (error) return;
    dce_printf( lsv_s_entry_values_listed, entry->Mask);
    if (entry->Mask & VLLIST_SITE) {
	dce_printf( lsv_s_entry_site_header_prefix);
	PrintafsNetAddr(&entry->site);
	dce_printf( lsv_s_justa_newline);
    }
    if (entry->Mask & VLLIST_PARTITION)
	dce_printf( lsv_s_list_partition_prefix, entry->partition);
    if (entry->Mask & VLLIST_VOLUMETYPE)
	dce_printf( lsv_s_volume_type_prefix, entry->volumetype, voltype_string(entry->volumetype));
    if (entry->Mask & VLLIST_VOLUMEID)
	dce_printf( lsv_s_volid_prefix, AFS_HGETBOTH(entry->volumeid));
    if (entry->Mask & VLLIST_FLAG)
	dce_printf( lsv_s_flag_prefix, entry->flag);
}

static display_entry(entry)
vldbentry    *entry;
{
    int i;

    dce_printf( lsv_s_volume_display_start, voltype_string(entry->volumeType), entry->name, entry->flags);
    for (i = 0; i < MAXVOLTYPES && !AFS_hiszero(entry->VolIDs[i]); ++i) {
	dce_printf( lsv_s_volume_entry_types, voltype_string(entry->VolTypes[i]),
		   AFS_HGETBOTH(entry->VolIDs[i]));
    }
    if (entry->WhenLocked != 0 || entry->LockerName[0] != '\0' || (entry->flags & VLOP_ALLOPERS) != 0) {
	dce_printf( lsv_s_lock_info_prefix);
	if (entry->WhenLocked != 0) dce_printf( lsv_s_locked_at_time, entry->WhenLocked);
	if (entry->LockerName[0] != '\0') dce_printf( lsv_s_lock_owner, entry->LockerName);
	if ((entry->flags & VLOP_ALLOPERS) != 0)
	    dce_printf( lsv_s_lock_op, (entry->flags & VLOP_ALLOPERS));
	dce_printf( lsv_s_yet_another_newline);
    }
    if (!AFS_hiszero(entry->cloneId)) dce_printf( lsv_s_volumes_clone_id, AFS_HGETBOTH(entry->cloneId));
    dce_printf( lsv_s_type_and_servers, voltype_string(entry->volumeType), entry->nServers);
    dce_printf( lsv_s_latency_and_other_data, entry->maxTotalLatency, entry->hardMaxTotalLatency,
	    entry->minimumPounceDally, entry->defaultMaxReplicaLatency,
	    entry->reclaimDally);
    dce_printf( lsv_s_site_addr_site_relp_principal);
    for (i=0; i < entry->nServers; i++) {
	PrintafsNetAddr(&entry->siteAddr[i]);
	dce_printf( lsv_s_some_final_data, entry->sitePartition[i], entry->siteFlags[i],
	       entry->sitemaxReplicaLatency[i], &entry->sitePrincipal[i].text[0]);
    }
}

static display_comentry(entry)
compactvldbentry    *entry;
{
    int i;

    dce_printf( lsv_s_comentry_display_head, voltype_string(entry->volumeType), entry->name, entry->flags);
    for (i = 0; i < MAXVOLTYPES && !AFS_hiszero(entry->VolIDs[i]); ++i) {
	dce_printf( lsv_s_comentry_id_type, voltype_string(entry->VolTypes[i]),
		   AFS_HGETBOTH(entry->VolIDs[i]));
    }
    if (entry->WhenLocked != 0 || entry->LockerName[0] != '\0' || (entry->flags & VLOP_ALLOPERS) != 0) {
	dce_printf( lsv_s_comentry_lock_info);
	if (entry->WhenLocked != 0) dce_printf( lsv_s_comentry_locked_time, entry->WhenLocked);
	if (entry->LockerName[0] != '\0') dce_printf( lsv_s_comentry_locker, entry->LockerName);
	if ((entry->flags & VLOP_ALLOPERS) != 0)
	    dce_printf( lsv_s_comentry_lock_ops, (entry->flags & VLOP_ALLOPERS));
	dce_printf( lsv_s_comentry_newline);
    }
    if (!AFS_hiszero(entry->cloneId))
	dce_printf( lsv_s_comentry_clone_id, AFS_HGETBOTH(entry->cloneId));
    dce_printf( lsv_s_comentry_voltype_and_svrs, voltype_string(entry->volumeType), entry->nServers);
    dce_printf( lsv_s_comentry_latency_etc, entry->maxTotalLatency, entry->hardMaxTotalLatency,
	    entry->minimumPounceDally, entry->defaultMaxReplicaLatency,
	    entry->reclaimDally);
    dce_printf( lsv_s_comentry_site_cookies_stmt, entry->nServers);
    for (i=0; i < entry->nServers; i++) {
	if (i != 0) fputs(", ", stdout);
	dce_printf( lsv_s_comentry_cookie_server, entry->siteCookies[i]);
    }
    putchar('\n');
}

static dump_stats(stats, vital_header)
vldstats	*stats;
vital_vlheader	*vital_header;
{   int	i;
    char strg[30];

    strncpy(strg, ctime((time_t *)&stats->start_time), sizeof(strg));
    strg[strlen(strg)-1] = 0;
    dce_svc_printf(LSV_S_DYNAMIC_STATS_TIME_MSG, strg);
    dce_svc_printf(LSV_S_DYNAMIC_STATS_OPCODE_MSG);
    for (i=0; i < VL_OPCODE_RANGE; i++)
	dce_svc_printf(LSV_S_DYNAMIC_STATS_CODENAMES_MSG, opcode_names[i], stats->requests[i], stats->aborts[i]);
    dce_svc_printf(LSV_S_BLDB_HEADER_STATS_MSG, vital_header->vldbversion);
    dce_svc_printf(LSV_S_HEADER_ALLOCS_AND_FREES_MSG,
		   vital_header->headersize, vital_header->allocs,
		   vital_header->frees,
		   AFS_HGETBOTH(vital_header->MaxVolumeId));
    for (i=0; i < MAXTYPES; i++)
	dce_svc_printf(LSV_S_TOTAL_VOL_ENTRIES_IS_MSG, voltype_string(VOLTIX_TO_VOLTYPE(i)),
	       vital_header->totalEntries[i]);
    dce_svc_printf(LSV_S_POINTER_INFO_MSG, vital_header->freePtr, vital_header->eofPtr);
    dce_svc_printf(LSV_S_SITEPTR_AND_NMBR_SITES_MSG, vital_header->sitesPtr, vital_header->numSites);
}

static GetArgs(line, args, nargs)
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

static print_usage()
{
    dce_svc_printf(LSV_S_VALID_COMMAND_USAGE_MSG);
    dce_svc_printf(LSV_S_CREATE_ENTRY_MSG);
    dce_svc_printf(LSV_S_REPLACE_ENTRY_MSG);
    dce_svc_printf(LSV_S_MASK_AND_PARMS_MSG);
    dce_svc_printf(LSV_S_LIST_ATTRIBUTES_MSG);
    dce_svc_printf(LSV_S_DISPLAY_ID_MSG);
    dce_svc_printf(LSV_S_DISPLAY_NAME_MSG);
    dce_svc_printf(LSV_S_SHOW_LOCK_STUFF_MSG);
    dce_svc_printf(LSV_S_ADD_ADDRESS_MSG);
    dce_svc_printf(LSV_S_REMOVE_ADDRESS_MSG);
    dce_svc_printf(LSV_S_NEXT_ID_MSG);
    dce_svc_printf(LSV_S_NEXT_SVRNAME_MSG);
    dce_svc_printf(LSV_S_GET_CELL_SITE_INFO_MSG);
    dce_svc_printf(LSV_S_GET_GEN_STATS_MSG);
    dce_svc_printf(LSV_S_GETC_ID_MSG);
    dce_svc_printf(LSV_S_GETC_NAME_MSG);
    dce_svc_printf(LSV_S_CREATE_SERVER_MSG);
    dce_svc_printf(LSV_S_ALTER_SERVER_MSG);
    dce_svc_printf(LSV_S_NEW_VOLIDS_MSG);
    dce_svc_printf(LSV_S_CMD_MISC_MSG);
}


/* main program */
main(argc, argv)
int argc;
char **argv; {
    register long code;
    long i, voltype, voldum;
    afs_hyper_t id;
    vldbentry entry;
    compactvldbentry centry;
    VldbListByAttributes listbyattributes;

    int	noAuth = 1;	    /* Default is authenticated connections */
    int	isServer = 0;

    /* cds */
    char *cdsGroupP = "/.:/fs";			/* default */
    char *rpcDebugFlags = 0;
    unsigned32 st;


    initialize_svc();


    for (i=1; i<argc; i++) {
	if (strcmp(argv[i], "-noauth") == 0)
	    noAuth = 0;
	else
        if ( strcmp(argv[i], "-group") == 0)
	{
	    /* name service group containing flservers */
	    i++;
	    cdsGroupP = argv[i];
	}
	else
	if ( strcmp(argv[i], "-rpcdebug") == 0 )
	{
	    /* rpc runtime debug */
	    i++;
            rpcDebugFlags = argv[i];
            rpc__dbg_set_switches(rpcDebugFlags, &st);
            if ( st != rpc_s_ok )
                dce_svc_printf(LSV_S_BEBUG_SET_FAILED_MSG);
	} else {
	    dce_svc_printf(LSV_S_UNKNOWN_FLC_FLAG_MSG, argv[i]);
	    exit(1);
	}
    }
    if (code = fl_Initialize(noAuth, cdsGroupP)) {
	dce_svc_printf(LSV_S_LIBRARY_NOT_INITED_MSG, code);
	exit(1);
    }

    while (1) {
	char line[500];
	int nargs, releasetype;
	unsigned long firstIx, nextIx;
	bzero((char *)&entry, sizeof(entry));
	bzero((char *)&centry, sizeof(centry));
	bzero((char *)&listbyattributes, sizeof(listbyattributes));
	dce_printf( lsv_s_fl_input_prompt);
	if (fgets(line, sizeof(line), stdin) != NULL) {
	    char *oper;
	    volumeName VNbuff;
	    register char **argp = args;
	    GetArgs(line, argp, &nargs);
	    if (nargs <= 0) continue;
	    oper = &argp[0][0];
	    ++argp, --nargs;
	    if (!strcmp(oper, "cr")) {
		fill_entry(&entry, argp, nargs);
		display_entry(&entry);
		code = ubik_Call(VL_CreateEntry, cstruct, 0, &entry);
		if (code != error_status_ok){
		    dce_svc_printf(LSV_S_CREATE_VL_ENTRY_MSG, code);
		}
	    } else if (!strcmp(oper, "rm")) {
		ScanHyper(argp, &id);
		argp+=2; nargs-=2;
		sscanf(&(*argp)[0], "%x", &voltype);
		code = ubik_Call(VL_DeleteEntry, cstruct, 0, &id, voltype);
		if (code != error_status_ok){
		    dce_svc_printf(LSV_S_DELETE_VL_ENTRY_MSG, code);
		}
	    } else if (!strcmp(oper, "re")) {
		ScanHyper(argp, &id);
		argp+=2; nargs-=2;
		sscanf(&(*argp)[0], "%x", &voltype);
		++argp, --nargs;
		sscanf(&(*argp)[0], "%d", &releasetype);
		++argp, --nargs;
		fill_entry(&entry, argp, nargs);
		display_entry(&entry);
		code = ubik_Call(VL_ReplaceEntry, cstruct, 0, &id, voltype, &entry, releasetype);
		if (code != error_status_ok){
		    dce_svc_printf(LSV_S_REPLACE_VL_ENTRY_MSG, code);
		}
	    } else if (!strcmp(oper, "ls")) {
		long index, count, next_index;
		for (index = 0; 1; index = next_index) {
		    bzero((char *)&entry, sizeof(entry));
		    code = ubik_Call(VL_ListEntry, cstruct, 0, index, &count, &next_index, &entry);
		    if (code != error_status_ok){
			dce_svc_printf(LSV_S_LIST_VL_ENTRY_MSG, code);
			break;
		    }
		    if (!next_index) break;
		    display_entry(&entry);
		}
	    } else if (!strcmp(oper, "la")) {
		int nentries, i;
		bulkentries entries;
		u_long cookie, nextcookie, flg;
		vldbentry *entry;

		fill_listbyattributes_entry(&listbyattributes, argp, nargs);
		display_listbyattributes_entry(&listbyattributes, 0);
		for (cookie = flg = 0; (flg & 1) == 0; cookie = nextcookie) {
		    bzero((char *)&entries, sizeof(entries));
		    code = ubik_Call(VL_ListByAttributes, cstruct, 0, &listbyattributes, cookie, &nentries, &entries, &nextcookie, &flg);
		    if (code != error_status_ok){
			dce_svc_printf(LSV_S_LIST_VL_ATTRIBUTES_MSG, code);
			break;
		    }
		    entry = &entries.bulkentries_val[0];
		    for (i=0; i < nentries; i++, entry++) {
			display_entry(entry);
		    }
		}
	    } else if (!strcmp(oper, "di")) {
		ScanHyper(argp, &id);
		argp+=2; nargs-=2;
		sscanf(&(*argp)[0], "%x", &voltype);
		code = ubik_Call(VL_GetEntryByID, cstruct, 0,
				 &id, voltype, &entry);
		if (code == error_status_ok)
		    display_entry(&entry);
		else
		    dce_svc_printf(LSV_S_GET_VL_ID_MSG, code);
	    } else if (!strcmp(oper, "cdi")) {
		ScanHyper(argp, &id);
		argp+=2; nargs-=2;
		sscanf(&(*argp)[0], "%x", &voltype);
		code = ubik_Call(VL_GetCEntryByID, cstruct, 0,
				 &id, voltype, &centry);
		if (code == error_status_ok)
		    display_comentry(&centry);
		else
		    dce_svc_printf(LSV_S_GETC_VL_ID_MSG, code);
	    } else if (!strcmp(oper, "sl")) {
		ScanHyper(argp, &id);
		argp+=2; nargs-=2;
		sscanf(&(*argp)[0], "%x", &voltype);
		++argp, --nargs;
		sscanf(&(*argp)[0], "%d", &voldum);
		code = ubik_Call(VL_SetLock, cstruct, 0, &id, voltype, voldum);
		if (code != error_status_ok)
		    dce_svc_printf(LSV_S_SET_VL_LOCK_FAAILED_MSG, code);
	    } else if (!strcmp(oper, "rl")) {
		ScanHyper(argp, &id);
		argp+=2; nargs-=2;
		sscanf(&(*argp)[0], "%x", &voltype);
		++argp, --nargs;
		sscanf(&(*argp)[0], "%d", &voldum);
		code = ubik_Call(VL_ReleaseLock, cstruct, 0, &id, voltype, voldum);
		if (code != error_status_ok)
		  dce_svc_printf(LSV_S_RELEASE_VL_LOCK_FAILED_MSG, code);
	    } else if (!strcmp(oper, "dn")) {
		strncpy((char *)&VNbuff[0], &argp[0][0], sizeof(VNbuff));
		code = ubik_Call(VL_GetEntryByName, cstruct, 0, &VNbuff[0], &entry);
		if (code == error_status_ok)
		    display_entry(&entry);
		else
		    dce_svc_printf(LSV_S_GET_VL_NAME_FAILED_MSG, code);
	    } else if (!strcmp(oper, "cdn")) {
		strncpy((char *)&VNbuff[0], &argp[0][0], sizeof(VNbuff));
		code = ubik_Call(VL_GetCEntryByName, cstruct, 0, &VNbuff[0], &centry);
		if (code == error_status_ok)
		    display_comentry(&centry);
		else
		    dce_svc_printf(LSV_S_GETC_VL_NAME_FAILED_MSG, code);
	    } else if (!strcmp(oper, "nv")) {
		afs_hyper_t newvolid;
		unsigned long count;
		sscanf(&(*argp)[0], "%d", &count);
		code = ubik_Call(VL_GetNewVolumeId, cstruct, 0, count, 0, &newvolid);
		if (code == error_status_ok)
		    dce_svc_printf(LSV_S_CURRENT_MAX_VOLID_MSG,
				   AFS_HGETBOTH(newvolid));
		else
		    dce_svc_printf(LSV_S_GET_VL_NEWVOL_FAILED_MSG, code);
	    } else if (!strcmp(oper, "xsc")) {
		siteDesc desc;
		unsigned long cookie;
		sscanf(&(*argp)[0], "%d", &cookie);
		code = ubik_Call(VL_ExpandSiteCookie, cstruct, 0, cookie, &desc);
		if (code == error_status_ok)
		    display_siteDesc(&desc);
		else
		    dce_svc_printf(LSV_S_EXPAND_VL_SITE_COOKIE_FAILED_MSG, code);
	    } else if (!strcmp(oper, "nvs")) {
		bulkIDs IDs;
		int i;
		unsigned long count;
		sscanf(&(*argp)[0], "%d", &count);
		code = ubik_Call(VL_GetNewVolumeIds, cstruct, 0, count, 0, &IDs);
		if (code == error_status_ok) {
		  i = sizeof(IDs.bulkIDs_val) / sizeof(IDs.bulkIDs_val[0]);
		  if (count > i) count = i;
		  for (i = 0; i < count; ++i) {
		    dce_svc_printf(LSV_S_NEWVOL_ID_VALUE_MSG, i,
				   AFS_HGETBOTH(IDs.bulkIDs_val[i]));
		  }
		}
		else
		  dce_svc_printf(LSV_S_GETNEW_VL_VOLID_FAILED_MSG, code);
	    } else if (!strcmp(oper, "gs")) {
		vldstats stats;
		vital_vlheader vital_header;
		code = ubik_Call(VL_GetStats, cstruct, 0, &stats, &vital_header);
		if (code == error_status_ok)
		    dump_stats(&stats, &vital_header);
		else
		    dce_svc_printf(LSV_S_GET_VL_STATS_FAILED_MSG, code);
	    } else if (!strcmp(oper, "aa")) {
		afsNetAddr Old, New;
		FillafsNetAddr(&Old, *argp);
		++argp, --nargs;
		FillafsNetAddr(&New, *argp);
		code = ubik_Call(VL_AddAddress, cstruct, 0, &Old, &New);
		if (code != error_status_ok)
		    dce_svc_printf(LSV_S_ADD_VL_ADDRESS_FAILED_MSG, code);
	    } else if (!strcmp(oper, "ca")) {
		afsNetAddr Old, New;
		FillafsNetAddr(&Old, *argp);
		++argp, --nargs;
		FillafsNetAddr(&New, *argp);
		code = ubik_Call(VL_ChangeAddress, cstruct, 0, &Old, &New);
		if (code != error_status_ok)
		    dce_svc_printf(LSV_S_CHANGE_VL_ADDRESS_FAILED_MSG, code);
	    } else if (!strcmp(oper, "ra")) {
		afsNetAddr Old;
		FillafsNetAddr(&Old, *argp);
		code = ubik_Call(VL_RemoveAddress, cstruct, 0, &Old);
		if (code != error_status_ok)
		    dce_svc_printf(LSV_S_RM_VL_ADDRESS_FAILED_MSG, code);
	    } else if (!strcmp(oper, "cs")) {
		siteDesc desc;
		fill_siteDesc_entry(&desc, argp, nargs);
		display_siteDesc(&desc);
		code = ubik_Call(VL_CreateServer, cstruct, 0, &desc);
		if (code != error_status_ok)
		    dce_svc_printf(LSV_S_CREATE_VL_SERVER_FAILED_MSG, code);
	    } else if (!strcmp(oper, "as")) {
		siteAlter desc;
		afsNetAddr Old;
		FillafsNetAddr(&Old, *argp);
		++argp, --nargs;
		fill_siteAlter_entry(&desc, argp, nargs);
		dce_printf(lsv_s_entry_being_altered);
		PrintafsNetAddr(&Old);
		dce_svc_printf(LSV_S_ALTERED_ENTRY_BEG_MSG);
		display_siteAlter(&desc);
		code = ubik_Call(VL_AlterServer, cstruct, 0, &Old, &desc);
		if (code != error_status_ok)
		    dce_svc_printf(LSV_S_ALTER_VL_SERVER_FAILED_MSG, code);
#if 0
	    } else if (!strcmp(oper, "sp")) {
		afsNetAddr Old;
		kerb_princ_name NewName;
		FillafsNetAddr(&Old, *argp);
		++argp, --nargs;
		bzero(&NewName.text[0], sizeof(NewName.text));
		if (nargs > 0) strcpy((char *)&NewName.text[0], *argp);
		code = ubik_Call(VL_SetPrincipal, cstruct, 0, &Old, &NewName);
		if (code != error_status_ok)
		    dce_svc_printf(LSV_S_SET_VL_PRINCIPAL_FAILED_MSG, code);
#endif /* 0 */
	    } else if (!strcmp(oper, "gc")) {
		vlconf_cell MyCell;
		int Ix;
		code = ubik_Call(VL_GetCellInfo, cstruct, 0, &MyCell);
		if (code == error_status_ok) {
		    dce_printf( lsv_s_get_vl_cell_info, MyCell.name,
			       AFS_HGETBOTH(MyCell.CellID), MyCell.numServers);
		    for (Ix = 0; Ix < MAXVLHOSTSPERCELL && (MyCell.hostName[Ix].hstnam[0] != '\0' || (MyCell.hostAddr[Ix].type != 0 && MyCell.hostAddr[Ix].type != (unsigned short) -1)); ++Ix) {
			dce_printf(lsv_s_vlcell_info_hostname, Ix, &(MyCell.hostName[Ix].hstnam[0]));
			PrintafsNetAddr(&(MyCell.hostAddr[Ix]));
			dce_printf( lsv_s_cellinfo_hostname_newline);
		    }
		}
		else
		  dce_svc_printf(LSV_S_GETVL_CELLINFO_FAILED_MSG, code);
	    } else if (!strcmp(oper, "ni")) {
		ScanHyper(argp, &id);
		argp+=2; nargs-=2;
		sscanf(&(*argp)[0], "%x", &voltype);
		++argp, --nargs;
		sscanf(&(*argp)[0], "%d", &firstIx);
		nextIx = 0;
		code = ubik_Call(VL_GetNextServersByID, cstruct, 0, &id,
				 voltype, firstIx, &nextIx, &entry, &voldum);
		if (code == error_status_ok) {
		  display_entry(&entry);
		  dce_svc_printf(LSV_S_FLAGE_FOR_DISPLAY_ENTRY_MSG, voldum, nextIx);
		}
		else
		  dce_svc_printf(LSV_S_GETNEXT_VL_SERVERS_FAILED_MSG, code);
	    } else if (!strcmp(oper, "nn")) {
		strncpy((char *)&VNbuff[0], &argp[0][0], sizeof(VNbuff));
		++argp, --nargs;
		sscanf(&(*argp)[0], "%d", &firstIx);
		nextIx = 0;
		code = ubik_Call(VL_GetNextServersByName, cstruct, 0, &VNbuff[0], firstIx, &nextIx, &entry, &voldum);
		if (code == error_status_ok) {
		  display_entry(&entry);
		  dce_svc_printf(LSV_S_NEXT_ENTRY_DISPLAY_FLAGS_MSG, voldum, nextIx);
		}
		else
		  dce_svc_printf(LSV_S_GETNEXT_VLSERVER_FAILED_MSG, code);
	    } else if (!strcmp(oper, "cni")) {
		ScanHyper(argp, &id);
		argp+=2; nargs-=2;
		sscanf(&(*argp)[0], "%x", &voltype);
		++argp, --nargs;
		sscanf(&(*argp)[0], "%d", &firstIx);
		nextIx = 0;
		code = ubik_Call(VL_GetCNextServersByID, cstruct, 0, &id, voltype, firstIx, &nextIx, &centry, &voldum);
		if (code == error_status_ok) {
		  display_comentry(&centry);
		  dce_svc_printf(LSV_S_DISPLAY_COMENTRY_FLAGS_MSG, voldum, nextIx);
		}
		else
		  dce_svc_printf(LSV_S_GET_COMENTRY_NEXTID_FAILED_MSG, code);
	    } else if (!strcmp(oper, "cnn")) {
		strncpy((char *)&VNbuff[0], &argp[0][0], sizeof(VNbuff));
		++argp, --nargs;
		sscanf(&(*argp)[0], "%d", &firstIx);
		nextIx = 0;
		code = ubik_Call(VL_GetCNextServersByName, cstruct, 0, &VNbuff[0], firstIx, &nextIx, &centry, &voldum);
		if (code == error_status_ok) {
		  display_comentry(&centry);
		  dce_svc_printf(LSV_S_GETCOMENTRY_NEXTID_DATA_MSG, voldum, nextIx);
		}
		else
		  dce_svc_printf(LSV_S_GETNEXT_COMENTRY_VLID_FAILED_MSG,code);
	    } else if (!strcmp(oper, "gn")) {	/* GenerateSites */
		bulkSites bS;
		int nSites, Ctr, j, k;
		firstIx = 0;
		nextIx = 0;
		for (Ctr = 1; ; ++Ctr) {
		    code = ubik_Call(VL_GenerateSites, cstruct, 0, firstIx, &nextIx, &bS, &nSites);
		    if (code != error_status_ok)
			dce_svc_printf(LSV_S_GENERATE_VLSITES_FAILED_MSG, code);
		    else
		      dce_svc_printf(LSV_S_NEXT_VLSITES_INFO_MSG, Ctr,
			     firstIx, nextIx, nSites);

		    if (code != 0 || nSites == 0) break;
		    for (j = 0; j < nSites; ++j) {
			dce_printf( lsv_s_site_principals_prefix, Ctr, j, bS.Sites[j].KerbPrin);
			for (k = 0; k < ADDRSINSITE; ++k) {
			    if (bS.Sites[j].Addr[k].type != ((unsigned short) -1)) {
				dce_printf( lsv_s_site_addr_type_info2, Ctr, j, k);
				PrintafsNetAddr(&(bS.Sites[j].Addr[k]));
				dce_printf( lsv_s_site_addrs_info_newline);
			    }
			}
		    }
		    firstIx = nextIx;
		}
	    } else if (!strcmp(oper, "si")) {	/* GetSiteInfo */
		afsNetAddr Old;
		siteDesc SD;
		int j;
		FillafsNetAddr(&Old, *argp);
		code = ubik_Call(VL_GetSiteInfo, cstruct, 0, &Old, &SD);
		if (code != error_status_ok)
		  dce_svc_printf(LSV_S_GET_VL_SITEINFO_FAILURE_MSG, code);
		else
		  display_siteDesc(&SD);
	    } else if (!strcmp(oper, "pr")) {
		code = ubik_Call(VL_Probe, cstruct, 0);
		if (code != error_status_ok)
		    dce_svc_printf(LSV_S_VL_PROBE_FAILURE_MSG, code);
	    } else if ((!strcmp(oper, "?")) || !strcmp(oper, "h"))
		print_usage();
	    else if ((!strcmp(oper, "q")) || !strcmp(oper, "quit"))
		exit(0);
	    else if (!strcmp(oper, "#")) {
		dce_printf( lsv_s_three_pounds);
		for( ; nargs; --nargs) {dce_printf( lsv_s_usage_args, *argp++);}
		dce_printf( lsv_s_site_info_newline);
		}
	    else
		dce_svc_printf(LSV_S_UNKNOWN_OPER_MSG);
	}
    }
}
