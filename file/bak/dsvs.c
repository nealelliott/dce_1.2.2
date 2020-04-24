/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (C) 1995, 1991 Transarc Corporation
 * All Rights Reserved
 */
/*
 * dsvs.c -- maintain dump schedule and fileset family abstractions
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */
#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>
#include <dcedfs/cmd.h>
#include <com_err.h>
#include <dcedfs/compat.h>
#include <pthread.h>
#include <bc.h>
#include <dfsbcmmac.h>
#include <dfsbcmsvc.h>
#include <dfsbcmmsg.h>

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/bak/RCS/dsvs.c,v 4.47 1995/07/26 16:07:31 blake Exp $")

extern char *whoami;
static int FreeVolumeEntry();

static char db_dsvs = 0;    /*Assume debugging output turned off*/
static char mn[] = "dsvs";  /*Module name*/

struct ubik_client *cstructp;	/*Ptr to Ubik client structure*/

/* Code to maintain dump schedule and volume set abstractions.
 * A volume set looks like this:
 *	vsname: servername partition-name <volume list>*
 * A dump schedule looks like this:
 *	dsname: vsname period parent-ds
 */

/*
 * bc_FindVolumeSet
 *	Return a pointer to the desired fileset family.
 *	Call with aconfig->vsetLock held for read
 */
int bc_FindVolumeSet(aconfig, aname, volSetP)
    struct bc_config *aconfig;
    char *aname;
    struct bc_volumeSet **volSetP;
{ /*bc_FindVolumeSet*/

    register struct bc_volumeSet *tvs;
    *volSetP = (struct bc_volumeSet *)NULL;

    if ((strcspn(aname, " ") == 0) || (strcmp(aname, "") == 0))
      return BC_BADVOLSET;
    for(tvs = aconfig->vset; tvs; tvs=tvs->next) {
      if (!strcmp(tvs->name, aname)) {
	*volSetP = tvs;
	return 0;
      }
    }
    return BC_NOVOLSET;
}/*bc_FindVolumeSet*/

/* get partition id from a name */
long bc_GetPartitionID(aname, servName, aval)
    long *aval;
    char *servName;
    char *aname; {

    /*bc_GetPartitionID */

    register char tc;
    char ascii[3];
    long aggrId;
    struct sockaddr_in servAddr;
    struct hostent *he;

    /* special-case "anything" */
    if (strcmp(aname, ".*") == 0) {
	*aval = -1;
	return 0;
    }
    /* convert servName to servAddr */
    if (!bc_hostNameToAddr(servName, &servAddr))
      return (BC_NOHOST);

    if (!GetAggr(aname, &servAddr, 1, &aggrId, 0))
	return(BC_NOPARTITION);
    else {
	*aval = aggrId;
	return 0;
    }

} /*bc_GetPartitionID*/

/* create an empty volume set, new items are added via bc_AddVolumeItem */
bc_CreateVolumeSet(aconfig, avolName)
struct bc_config *aconfig;
char *avolName; {
    register long code;
    register struct bc_volumeSet **tlast;
    struct bc_volumeSet *tset;

    if (aconfig == (struct bc_config *)NULL)
      return (BC_INTERNALERROR);
    lock_ObtainShared(&(aconfig->vsetLock));
    code = bc_FindVolumeSet(aconfig, avolName, &tset);
    if (code == 0) {
      lock_ReleaseShared(&(aconfig->vsetLock));
      if (tset == (struct bc_volumeSet *)NULL)
	return BC_INTERNALERROR;
      else
	return BC_VOLSETEXISTS;
    }
    else if (code != BC_NOVOLSET) {
      lock_ReleaseShared(&(aconfig->vsetLock));
      return code;
    }
    else {
      if (tset) {
	lock_ReleaseShared(&(aconfig->vsetLock));
	return BC_INTERNALERROR;
      }
    }
    lock_UpgradeSToW(&(aconfig->vsetLock));
    /* move to end of the list */
    for(tlast = &aconfig->vset, tset = *tlast; tset;
	tlast = &tset->next, tset = *tlast);
    tset = (struct bc_volumeSet *)osi_Alloc(sizeof(struct bc_volumeSet));
    if (tset == (struct bc_volumeSet *)NULL) {
      lock_ReleaseWrite(&(aconfig->vsetLock));
      return BC_NOMEM;
    }
    bzero((char *)tset, sizeof(*tset));
    tset->name = (char *)osi_Alloc(strlen(avolName)+1);
    if (tset->name == (char *)NULL) {
      lock_ReleaseWrite(&(aconfig->vsetLock));
      osi_Free(tset, sizeof(struct bc_volumeSet));
      return BC_NOMEM;
    }
    strcpy(tset->name, avolName);
    *tlast = tset;
    lock_ReleaseWrite(&(aconfig->vsetLock));
    return 0;
}

static FreeVolumeEntryList(aentry)
register struct bc_volumeEntry *aentry; {
    register struct bc_volumeEntry *tnext;

    while(aentry) {
	tnext = aentry->next;
	FreeVolumeEntry(aentry);
	aentry = tnext;
    }
    return 0;
}

static FreeVolumeEntry(aentry)
register struct bc_volumeEntry *aentry; {
    (void)osi_Free(aentry->name, strlen(aentry->name)+1);
    (void)osi_Free(aentry->serverName, strlen(aentry->serverName)+1);
    (void)osi_Free(aentry->partname, strlen(aentry->partname)+1);
    (void)osi_Free(aentry, sizeof(struct bc_volumeEntry));
    return 0;
}

bc_DeleteVolumeSet(aconfig, avolName)
struct bc_config *aconfig;
char *avolName; {
    register struct bc_volumeSet **tlast, *tset;

    lock_ObtainWrite(&(aconfig->vsetLock));
    tlast = &aconfig->vset;
    for(tset = *tlast; tset; tlast = &tset->next, tset = *tlast) {
	if (strcmp(avolName, tset->name) == 0) {
	    /* found the item we're looking for */
	    *tlast = tset->next;
	    (void)osi_Free(tset->name, strlen(tset->name)+1);
	    FreeVolumeEntryList(tset->ventries);
	    (void)osi_Free(tset, sizeof(struct bc_volumeSet));
	    lock_ReleaseWrite(&(aconfig->vsetLock));
	    return 0;
	}
    }

    lock_ReleaseWrite(&(aconfig->vsetLock));
    /* if we get here, we didn't find the item */
    return BC_NOVOLSET;
}

bc_DeleteVolumeItem(aconfig, avolName, anumber)
struct bc_config *aconfig;
char *avolName;
long anumber; {
    register long i;
    register struct bc_volumeEntry *tentry, **tlast;
    struct bc_volumeSet *tset;
    register long code;

    if (aconfig == (struct bc_config *)NULL)
      return BC_INTERNALERROR;
    lock_ObtainShared(&(aconfig->vsetLock));
    code = bc_FindVolumeSet(aconfig, avolName, &tset);
    if (code || !tset) {
      lock_ReleaseShared(&(aconfig->vsetLock));
      return BC_NOVOLSET;
    }
    lock_UpgradeSToW(&(aconfig->vsetLock));
    tlast = &tset->ventries;
    for(i=1, tentry = *tlast; tentry; tlast = &tentry->next, tentry = *tlast, i++) {
	if (anumber == i) {
	    /* found entry we want */
	    *tlast = tentry->next;
	    FreeVolumeEntry(tentry);
	    lock_ReleaseWrite(&(aconfig->vsetLock));
	    return 0;
	}
    }
    lock_ReleaseWrite(&(aconfig->vsetLock));
    return BC_NOVOLENTRY;/* not found */
}

bc_AddVolumeItem(aconfig, avolName, ahost, apart, avol)
     struct bc_config *aconfig;
     char *avolName;
     char *ahost, *apart, *avol;
{
    struct bc_volumeSet *tset;
    register struct bc_volumeEntry **tlast, *tentry;
    register long code;

    if (aconfig == (struct bc_config *)NULL)
      return BC_INTERNALERROR;
    lock_ObtainShared(&(aconfig->vsetLock));
    code = bc_FindVolumeSet(aconfig, avolName, &tset);
    if (code || !tset) {
      lock_ReleaseShared(&(aconfig->vsetLock));
      return(BC_NOVOLSET);
    }
    /* otherwise append this item to the end of the real list */
    tlast = &tset->ventries;
    /* move to end of the list */
    for(tentry = *tlast; tentry; tlast = &tentry->next, tentry = *tlast);
    lock_UpgradeSToW(&(aconfig->vsetLock));
    tentry = (struct bc_volumeEntry *)osi_Alloc(sizeof(struct bc_volumeEntry));
    if (tentry == (struct bc_volumeEntry *)NULL) {
      lock_ReleaseWrite(&(aconfig->vsetLock));
      return (code);
    }
    bzero((char *)tentry, sizeof(*tentry));
    /* Expand the host name before we store it in the fileset family */
    code = bc_validateTapeHostName(ahost, &(tentry->serverName));
    if (code) {
      lock_ReleaseWrite(&(aconfig->vsetLock));
      osi_Free(tentry, sizeof(struct bc_volumeEntry));
      return (code);
    }
    tentry->partname = (char *)osi_Alloc(strlen(apart)+1);
    if (tentry->partname == (char *)NULL) {
      lock_ReleaseWrite(&(aconfig->vsetLock));
      osi_Free(tentry, sizeof(struct bc_volumeEntry));
      return (code);
    }
    strcpy(tentry->partname, apart);
    tentry->name = (char *)osi_Alloc(strlen(avol)+1);
    if (tentry->name == (char *)NULL) {
      lock_ReleaseWrite(&(aconfig->vsetLock));
      osi_Free(tentry->partname, strlen(apart)+1);
      osi_Free(tentry, sizeof(struct bc_volumeEntry));
      return (code);
    }
    strcpy(tentry->name, avol);

    /* For now, we only handle the .* pattern. This will have to change if
       we accept regular expressions for the server name */
    if ( strcmp(tentry->serverName, ".*") != 0 &&
	!bc_hostNameToAddr(tentry->serverName,
			(struct sockaddr_in *)&(tentry->server))) {
      lock_ReleaseWrite(&(aconfig->vsetLock));
      osi_Free(tentry->partname, strlen(apart)+1);
      osi_Free(tentry->name, strlen(avol)+1);
      osi_Free(tentry, sizeof(struct bc_volumeEntry));
      return(BC_NOHOST);
    }
    code = bc_GetPartitionID(tentry->partname, tentry->serverName,
			     &tentry->partition);
    if (code) {
      lock_ReleaseWrite(&(aconfig->vsetLock));
      osi_Free(tentry->partname, strlen(apart)+1);
      osi_Free(tentry->name, strlen(avol)+1);
      osi_Free(tentry, sizeof(struct bc_volumeEntry));
      return(code);
    }
    *tlast = tentry;	/* thread on the list */
    lock_ReleaseWrite(&(aconfig->vsetLock));
    return 0;
}

/* ------------------------------------
 * dumpschedule management code
 * ------------------------------------
 */

/* bc_CreateDumpSchedule
 *	Add another node to the dump schedule.
 * entry:
 *	aconfig - in core configuration structures
 *	adumpName - name of new dump node
 *	expDate - expiration date
 *	expType - absolute or relative
 */

bc_CreateDumpSchedule(aconfig,adumpName, expDate, expType)
struct bc_config *aconfig;
char *adumpName;
long	expDate;
long	expType;
{
    register struct bc_dumpSchedule **tlast, *tdump;
    struct bc_dumpSchedule *parent, *node;
    long code;

    if (aconfig == (struct bc_config *)NULL)
      return BC_INTERNALERROR;
    if(adumpName == (char *)NULL || ((int)strlen(adumpName) < 2) ||
       (strcmp(adumpName, "none") == 0) ||
       (adumpName[0] != '/'))
	return BC_BADDUMPLVL;
    lock_ObtainShared(&(aconfig->dschedLock));
    code = bc_FindDumpSchedule(aconfig, adumpName, &node);
    if (code == 0) {
      lock_ReleaseShared(&(aconfig->dschedLock));
      if (node == (struct bc_dumpSchedule *)NULL)
	return BC_INTERNALERROR;
      else
	return BC_DUMPLVLEXISTS;
    }
    else if (code != BC_NODUMPLVL) {
      lock_ReleaseShared(&(aconfig->dschedLock));
      return code;
    }
    else {
      if (node) {
	lock_ReleaseShared(&(aconfig->dschedLock));
	return BC_INTERNALERROR;
      }
    }
    code =  FindDump(aconfig, adumpName, &parent, &node);
    if ( code == 0 ) {
      lock_ReleaseShared(&(aconfig->dschedLock));
      return BC_DUMPLVLEXISTS;		/* node already exists */
    }
    else if (code != BC_NODUMPLVL) {
      lock_ReleaseShared(&(aconfig->dschedLock));
      return code;				/* name specification error */
    }

    /*
     * dump level does not exist, create one
     */
    tdump=(struct bc_dumpSchedule *)osi_Alloc(sizeof(struct bc_dumpSchedule));
    if (tdump == (struct bc_dumpSchedule *)NULL) {
      lock_ReleaseShared(&(aconfig->dschedLock));
      return BC_NOMEM;
    }
    bzero((char *)tdump, sizeof(*tdump));
    lock_UpgradeSToW(&(aconfig->dschedLock));
    /* prepend this node to the dump schedule list */
    tdump->next = aconfig->dsched;
    aconfig->dsched = tdump;

    /* save the name of this dump node */
    tdump->name = (char *)osi_Alloc(strlen(adumpName)+1);
    strcpy(tdump->name, adumpName);

    /* expiration information */
    tdump->expDate = expDate;
    tdump->expType = expType;
    lock_ReleaseWrite(&(aconfig->dschedLock));
    bc_ProcessDumpSchedule(aconfig);	/* redo tree */
    return 0;
}


/* Recursively remove this node and all of its children from aconfig's
 * list of dumps.  Note that this leaves the sibling pointers damaged (pointing
 * to strange places), so we must call bc_ProcessDumpSchedule when we're done.
 *
 *	Called with aconfig->dschedLock held for write
 */

bc_DeleteDumpScheduleAddr(aconfig, adumpAddr)
struct bc_config *aconfig;
struct bc_dumpSchedule *adumpAddr; {
    register struct bc_dumpSchedule **tlast, *tdump;
    register struct bc_dumpSchedule *tnext;

    if (aconfig == (struct bc_config *)NULL)
      return BC_INTERNALERROR;
    /* knock off all children first */
    for(tdump = adumpAddr->firstChild; tdump; tdump = tnext) {
	/* extract next ptr now, since will be freed by recursive call below */
	tnext = tdump->nextSibling;
	bc_DeleteDumpScheduleAddr(aconfig, tdump);
    }

    /* finally, remove us from the list of good dudes */
    for(tlast = &aconfig->dsched, tdump = *tlast; tdump;
	tlast = &tdump->next, tdump = *tlast) {
	if (tdump == adumpAddr) {
	    /* found the one we're looking for */
	    *tlast = tdump->next;	/* remove us from basic list */
	    (void)osi_Free(tdump->name, strlen(tdump->name)+1);
       	    (void)osi_Free(tdump, sizeof(struct bc_dumpSchedule));
	    return 0;
	}
    }
    return BC_NODUMPLVL;
}

/* bc_FindDumpSchedule
 *	Finds dump schedule aname by doing a linear search
 *
 *	Call this routine with aconfig->dschedLock held for read
 * entry:
 *	aconfig - handle for incore configuration tables
 *	aname - (path)name to match on
 *	dumpSP - returned dump schedule ptr
 * exit:
 *	return error code, dump schedule ptr is NULL for failure
 *      return 0 and ptr to dumpschedule non-NULL for success
 */

bc_FindDumpSchedule(aconfig, aname, dumpSP)
struct bc_config *aconfig;
char *aname;
struct bc_dumpSchedule **dumpSP;
{
    register struct bc_dumpSchedule *tds;
    if (aconfig == (struct bc_config *)NULL)
      return BC_INTERNALERROR;
    *dumpSP = (struct bc_dumpSchedule *)NULL;
    if ((strcspn(aname, " ") == 0) || (strcmp(aname, "") == 0))
      return BC_BADDUMPLVL;
    for(tds=aconfig->dsched; tds; tds=tds->next) {
	if (!strcmp(tds->name, aname)) {
	    *dumpSP = tds;
	    return 0;
	  }
    }
    return BC_NODUMPLVL;
}

/* bc_DeleteDumpSchedule
 *	Delete dump node adumpName from the dump schedule
 *	Called with aconfig->dschedLock held for read
 */

bc_DeleteDumpSchedule(aconfig, adumpName)
struct bc_config *aconfig;
char *adumpName; {
    register struct bc_dumpSchedule *tdump;

    /* does a linear search of the dump schedules in order to find
     * the one to delete
     */
    if (aconfig == (struct bc_config *)NULL)
      return BC_INTERNALERROR;
    lock_ObtainWrite(&(aconfig->dschedLock));
    for(tdump = aconfig->dsched; tdump; tdump=tdump->next) {
	if (strcmp(tdump->name, adumpName)==0) {
	    /* found it, we can zap recursively */
	    bc_DeleteDumpScheduleAddr(aconfig, tdump);
	    /* tree is pruned, but we have to recompute the internal pointers
	       from first principles, since we didn't bother to maintain
	       the sibling and children pointers during the call to delete
	       the nodes */
	    lock_ReleaseWrite(&(aconfig->dschedLock));
	    bc_ProcessDumpSchedule(aconfig);
	    return 0;
	}
    }
    /* if we make it here, there's no such dump schedule entry */
    lock_ReleaseWrite(&(aconfig->dschedLock));
    return BC_NODUMPLVL;
}


/* bc_ProcessDumpSchedule
 *	Walk over the dump schedule list, building it into a tree.  This
 * algorithm is simple, but takes O(N*2) operations to run, with N=number
 * of dump schedule nodes. It probably will never matter
 *
 *	Called with aconfig->dschedLock held for write
 */

bc_ProcessDumpSchedule(aconfig)
register struct bc_config *aconfig;
{
    register struct bc_dumpSchedule *tds, *uds;
    struct bc_dumpSchedule *parentptr, *nodeptr;
    int retval;

    if (aconfig == (struct bc_config *)NULL)
      return BC_INTERNALERROR;
    /* first, clear all the links on all entries so that this function
     * may be called any number of times with no ill effects
     */
    for(tds = aconfig->dsched; tds; tds=tds->next)
    {
	tds->parent = (struct bc_dumpSchedule *) 0;
	tds->nextSibling = (struct bc_dumpSchedule *) 0;
	tds->firstChild = (struct bc_dumpSchedule *) 0;
    }

    for(tds = aconfig->dsched; tds; tds=tds->next)
    {
	retval = FindDump(aconfig, tds->name, &parentptr, &nodeptr);
	if ( retval != 0 )
	  return retval;

	/* only need to do work if it is not a root node */
	if ( parentptr != 0 )
	{
	    nodeptr->parent = parentptr;
	    nodeptr->nextSibling = parentptr->firstChild;
	    parentptr->firstChild = nodeptr;
	}
    }
    return 0;
}


/* FindDump
 * entry:
 *	Called with aconfig->dschedLock held for read
 * exit:
 *	parentptr - set to parent node, if one exists
 *	nodeptr - set to node requested
 *
 *	return values are:
 *	0 - success, parentptr and nodeptr set appropriately
 * notes:
 *	pathname checking should be done externally. In particular, trailing
 *	/ symbols may return confusing error codes.
 */

int
FindDump(aconfig, name, parentptr, nodeptr)
  struct bc_config *aconfig;
  char *name;
  struct bc_dumpSchedule **parentptr;
  struct bc_dumpSchedule **nodeptr;
{
    struct bc_dumpSchedule *dsptr;
    char *curptr, *nodeString;

    if (aconfig == (struct bc_config *)NULL)
      return BC_INTERNALERROR;
    *parentptr = (struct bc_dumpSchedule *)0;
    *nodeptr = (struct bc_dumpSchedule *)0;
    if ((strcspn(name, " ") == 0) || (strcmp(name, "") == 0))
      return BC_BADDUMPLVL;

    /*
     * this routine does inline manipulation of name,
     * better to have our own copy
     */
    nodeString = (char *)osi_Alloc(strlen(name)+1);
    if (nodeString == (char *)NULL)
	return BC_NOMEM;
    strcpy(nodeString, name);

    curptr = nodeString;
    while (curptr) {
	*parentptr = *nodeptr;
	*nodeptr = (struct bc_dumpSchedule *)0;
	*curptr = '/';
	curptr = strchr((char *)curptr+1, '/');
	if (curptr)
	    *curptr = '\0';
	/* now search all the nodes for this name */
	for ( dsptr = aconfig->dsched; dsptr != 0; dsptr = dsptr->next)
	    if ( strcmp(dsptr->name, nodeString) == 0) {
		*nodeptr = dsptr;
		break;
	    }
	if(! *nodeptr)
	    break;
    }/* while */

    (void)osi_Free(nodeString, strlen(name)+1);
    if (curptr) 		/* parent node not present */
	return BC_NOPARENTLVL;
    if (! *nodeptr)		/* child node not present */
	return BC_NODUMPLVL;
    return 0;			/* both child and parent (if any) present */
}
