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
 * dump_sched.c -- Dump Schedule management functions
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */

#include <dcedfs/compat.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>
#include <pthread.h>
#include <dcedfs/ktime.h>
#include <dcedfs/cmd.h>
#include <com_err.h>
#include <dcedfs/budb_errs.h>
#include <bc.h>
#include <error_macros.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbcmmac.h>
#include <dfsbcmsvc.h>
#include <dfsbcmmsg.h>

/* code to manage dump schedules
 * specific to the ubik database implementation
 */

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/bak/RCS/dump_sched.c,v 4.33 1995/07/26 16:07:33 blake Exp $")

extern struct bc_config *bc_globalConfig;
extern struct udbHandleS udbHandle;
extern char *whoami;
static long ListDumpSchedule();
long bc_UpdateDumpSchedule();

/* ------------------------------------
 * command level routines
 * ------------------------------------
 */

/* bc_AddDumpCmd
 *	add a dump schedule
 * params:
 *	parm 0: list of dump names
 *	parm 1: expiration date (list)
 */

bc_AddDumpCmd(as, arock)
struct cmd_syndesc *as;
char *arock; {
    register char *dname;			/* dump schedule name */
    char *dateString;				/* expiration date */
    register long code;
    long expType, expDate;
    register struct cmd_item *ti, *expItem;
    udbClientTextP ctPtr;
    long bc_ParseExpiration();

    /* if an expiration date has been specified */
    if ( as->parms[1].items != 0 )
    {
	code = bc_ParseExpiration(&as->parms[1], &expType, &expDate);
	if ( code )
	{
	  com_err(whoami, code, "; Unable to parse expiration date %s",
		  as->parms[1]);
	  return(code);
	}
    } /* end if parm[1].items */
    else
    {
	/* no expiration date specified */
	expDate = 0;
	expType = BC_NO_EXPDATE;
    }

    /* lock schedules and check validity */
    ctPtr = &bc_globalConfig->configText[TB_DUMPSCHEDULE];

    code = bc_LockText(ctPtr);
    if ( code == BUDB_LOCKED ) {
      com_err(whoami, code,
	   "; Unable to lock dump schedule info in database, try again later");
      ERROR(code);
    }
    else if (code) {
      com_err(whoami, code,
       "; Unable to add dump schedule to backup database");
      ERROR(code);
    }

    code = bc_UpdateDumpSchedule(ctPtr);
    if (code) {
      com_err(whoami, code,
	      "; Unable to sync dump schedule info in local cache and backup database");
      ERROR(code);
    }

    /* process each dump name using the expiration date computed above */
    for( ti=as->parms[0].items; ti != 0; ti = ti->next )
    {

	/* get next dump name to process */
	dname = ti->data;

	/* validate the name dump name length */
	if ( strlen(dname) >= (unsigned)BU_MAX_DUMP_PATH )
	{
	    com_err(whoami, 0, "Dump names must be < %d characters",
		    BU_MAX_DUMP_PATH);
	    com_err(whoami, 0, "Dump %s not added", dname);
	    continue;
	}

	code = bc_CreateDumpSchedule(bc_globalConfig, dname, expDate, expType);
	if (code)
	{
	  com_err(whoami, code, "; Failed to create dump schedule '%s'",
		  dname);
	  continue;
	}

	/* save the new schedule item */

	code = bc_SaveDumpSchedule();
	if (code == 0)
		com_err(whoami,0,"Created new dump schedule %s", dname);
	else
	{
	    com_err(whoami, code, "; Unable to add dump level to database");
	    com_err(whoami,0,"Changes are temporary - for this session only");
	}

	/* stop if any errors occurred */
	if ( code )
		break;
    }

error_exit:
    if ( ctPtr->lockHandle != 0 )
	bc_UnlockText(ctPtr);
    return(code);
}


/* bc_DeleteDumpCmd
 *	delete a dump schedule
 */

bc_DeleteDumpCmd(as, arock)
struct cmd_syndesc *as;
char *arock; {
    /* parm 0 is vol set name
     * parm 1 is dump schedule name
     */
    register char  *dname;
    register long code;
    udbClientTextP ctPtr;

    /* lock schedules and check validity */
    ctPtr = &bc_globalConfig->configText[TB_DUMPSCHEDULE];

    code = bc_LockText(ctPtr);
    if ( code == BUDB_LOCKED ) {
      com_err(whoami, code,
	   "; Unable to lock dump schedule info in database, try again later");
      ERROR(code);
    }
    else if (code) {
      com_err(whoami, code,
       "; Unable to delete dump schedule from backup database");
      ERROR(code);
    }
    code = bc_UpdateDumpSchedule(ctPtr);
    if (code) {
      com_err(whoami, code,
	      "; Unable to sync dump schedule info in local cache and backup database");
      ERROR(code);
    }

    dname = as->parms[0].items->data;
    code = bc_DeleteDumpSchedule(bc_globalConfig, dname);
    if (code) {
      com_err(whoami, code,
	      "; Failed to delete dump schedule '%s'", dname);
      goto error_exit;
    }
    code = bc_SaveDumpSchedule();
    if (code == 0)
    	dce_svc_printf(BAK_S_DELETE_SCHEDULE_MSG, dname);
    else
    {
	com_err(whoami, code,
		"; Unable to delete dump level from database");
	com_err(whoami, 0,
		"Deletion is temporary - for this session only");
    }

error_exit:
    if ( ctPtr->lockHandle != 0 )
	bc_UnlockText(ctPtr);
    return code;
}



/* bc_ListDumpScheduleCmd
 *      list the (internally held) dump schedule tree
 * parameters:
 *      ignored
 */

bc_ListDumpScheduleCmd(as, arock)
struct cmd_syndesc *as;
char *arock;
{
    /* no parms */
    register struct bc_dumpSchedule *tdump;
    udbClientTextP ctPtr;
    register long code;

    /* lock schedules and check validity */
    ctPtr = &bc_globalConfig->configText[TB_DUMPSCHEDULE];

    /* first check to see if schedules must be updated */
    code = bc_LockText(ctPtr);
    if ( code )
      com_err(whoami, code,
       "; Unable to fetch dump schedules from bakserver; Using info in cache");
    else
      (void)bc_UpdateDumpSchedule(ctPtr);
    /* go through entire list, displaying trees for root-level dump
     * schedules
     */
    lock_ObtainRead(&(bc_globalConfig->dschedLock));
    for(tdump = bc_globalConfig->dsched; tdump; tdump=tdump->next)
    {
        /* if this is a root-level dump, show it and its kids */
        if (!tdump->parent)
            (void)ListDumpSchedule(tdump, 0);
    }
    lock_ReleaseRead(&(bc_globalConfig->dschedLock));
    if ( ctPtr->lockHandle )
	bc_UnlockText(ctPtr);
    return 0;
}


/* bc_SetExpCmd
 *	Set/clear expiration date on existing dump node
 * params:
 *	parm 0: list of dump names
 *	parm 1: expiration date (list)
 */

bc_SetExpCmd(as, arock)
     struct cmd_syndesc *as;
     char *arock;
{
    register char *dname;			/* dump schedule name */
    register struct cmd_item *ti;
    struct bc_dumpSchedule *node, *parent;
    long expType, expDate;
    udbClientTextP ctPtr;
    register long code;
    long bc_ParseExpiration();

    /* if an expiration date has been specified */
    if ( as->parms[1].items != 0 )
    {
	code = bc_ParseExpiration(&as->parms[1], &expType, &expDate);
	if ( code )
	{
	  com_err(whoami, code, "; Unable to parse expiration date %s",
		  as->parms[1]);
	  return(code);
	}
    } /* end if parm[1].items */
    else
    {
	/* no expiration date specified */
	expDate = 0;
	expType = BC_NO_EXPDATE;
    }

    /* lock schedules and check validity */
    ctPtr = &bc_globalConfig->configText[TB_DUMPSCHEDULE];

    code = bc_LockText(ctPtr);
    if ( code == BUDB_LOCKED ) {
      com_err(whoami, code,
	   "; Unable to lock dump schedule info in database, try again later");
      ERROR(code);
    }
    else if (code) {
      com_err(whoami, code,
       "; Unable to set expiration date on dump schedule in backup database");
      ERROR(code);
    }
    code = bc_UpdateDumpSchedule(ctPtr);
    if (code) {
      com_err(whoami, code,
	      "; Unable to sync dump schedule info in local cache and backup database");
      ERROR(code);
    }

    /* process each dump name using the expiration date computed above */
    for( ti = as->parms[0].items; ti != 0; ti = ti->next)
    {
	/* get next dump name to process */
	dname = ti->data;

	/* validate the name dump name length */
	if ( strlen(dname) >= (unsigned)BU_MAX_DUMP_PATH )
	{
	    com_err(whoami, 0, "; Dump names must be < %d characters",
		    BU_MAX_DUMP_PATH);
	    com_err(whoami, 0, "Dump %s not added", dname);
	    continue;
	}
	lock_ObtainRead(&(bc_globalConfig->dschedLock));
	code = FindDump(bc_globalConfig, dname, &parent, &node);
	if ( code )
	{
	    com_err(whoami, code, "; Unable to find dump level %s", dname);
	    lock_ReleaseRead(&(bc_globalConfig->dschedLock));
	    continue;
	}
	lock_ReleaseRead(&(bc_globalConfig->dschedLock));
	node->expDate = expDate;
	node->expType = expType;
    }

    code = bc_SaveDumpSchedule();
    if ( code )
    {
	com_err(whoami, code,
		"; Unable to set expiration date to dump level in database");
	com_err(whoami,0,"Expiration changes effective for this session only");
    }

error_exit:
    if ( ctPtr->lockHandle != 0 )
        bc_UnlockText(ctPtr);
    return(code);
}



/* ------------------------------------
 * general dump schedule handling routines
 * ------------------------------------
 */

bc_GetDumpSchedule()
{
    extern struct bc_config *bc_globalConfig;
    udbClientTextP ctPtr;
    struct udbHandleS *uhptr = &udbHandle;
    long code;

    /* setup the right ptr */
    ctPtr = &bc_globalConfig->configText[TB_DUMPSCHEDULE];

    /* lock the text */
    code = bc_LockText(ctPtr);
    if ( code )
    	ERROR(code);

    /* fetch the file */
    code = bcdb_GetTextFile(ctPtr);
    if ( code )
    	ERROR(code);

     /* fetch the version number */
    code = ubik_Call(BUDB_GetTextVersion,  uhptr->uh_client, 0,
                     ctPtr->textType, &ctPtr->textVersion);
    if ( code )
        ERROR(code);

    /* parse the file */
    code = bc_ParseDumpSchedule();
    if ( code )
    	ERROR(code);

    code = bc_ProcessDumpSchedule(bc_globalConfig);
    if ( code )
    	ERROR(code);

error_exit:
    if ( ctPtr->lockHandle )
	code = bc_UnlockText(ctPtr);
    return(code);
}

bc_ParseDumpSchedule()
{
    char tbuffer[1024];
    char dsname[256],  period[64];
    char *tp;
    long code;
    udbClientTextP ctPtr;
    register struct bc_dumpSchedule *tds;
    struct bc_dumpSchedule **ppds, *pds;
    long  expDate, expType;

    register FILE *stream;

    /* initialize locally used variables */
    ctPtr = &bc_globalConfig->configText[TB_DUMPSCHEDULE];
    stream = ctPtr->textStream;

    if ( ctPtr->textSize == 0 )			/* nothing defined yet */
	return(0);

    if ( stream == NULL )
    	return(BC_INTERNALERROR);

    rewind(stream);

    /* check the magic number and version */
    tp = fgets(tbuffer, sizeof(tbuffer), stream);
    if ( tp == 0 )
	/* can't read first line - error */
	return(BC_INTERNALERROR);
    else
    {
	long dsmagic, dsversion;

	/* read the first line, and then check magic # and version */

	code = sscanf(tbuffer, "%d %d", &dsmagic, &dsversion);
	if ( (code != 2)
	||   (dsmagic != BAK_SCHEDULE_MAGIC)
	||   (dsversion != CUR_BAK_SCHEDULE_VERSION)
	   )
	{
	    /* invalid or unexpected header - error */
	    com_err(whoami, 0, "Unable to understand dump schedule file");
	    return(BC_INTERNALERROR);
	}
    }

    while(1) {
      /* read all of the lines out */
      tp = fgets(tbuffer, sizeof(tbuffer), stream);
      if ( tp == 0 )
	break;	/* hit eof? */
      code = sscanf(tbuffer, "%s %s %d %d", dsname, period,
		    &expDate, &expType);
      if (code != 4)
	{
	  com_err(whoami,0,"Syntax error in dump schedule file, line is: %s",
		    tbuffer);
	  return (BC_INTERNALERROR);
	}
      tds=(struct bc_dumpSchedule *)osi_Alloc(sizeof(struct bc_dumpSchedule));
      bzero((char *)tds, sizeof(*tds));

      tds->next = (struct bc_dumpSchedule *) 0;
      tds->name = (char *)osi_Alloc(strlen(dsname)+1);
      strcpy(tds->name, dsname);

      tds->expDate = expDate;
      tds->expType = expType;

      /* find the end of the schedule list, and append the new item to it */
      lock_ObtainWrite(&(bc_globalConfig->dschedLock));
      ppds = &bc_globalConfig->dsched;
      pds = *ppds;
      while ( pds != 0 )
	{
	  ppds = &pds->next;
	  pds = *ppds;
	}
      *ppds = tds;
      lock_ReleaseWrite(&(bc_globalConfig->dschedLock));
    }
    return 0;
}


bc_SaveDumpSchedule()
{
    struct bc_dumpSchedule *tdump;
    udbClientTextP ctPtr;
    long code;

    extern struct bc_config *bc_globalConfig;
    extern long filesize();

    /* setup the right ptr */
    ctPtr = &bc_globalConfig->configText[TB_DUMPSCHEDULE];

    /* must be locked */
    if ( ctPtr->lockHandle == 0 )
    	return(BC_INTERNALERROR);

    /* truncate the file */
    code = ftruncate(fileno(ctPtr->textStream), 0);
    if ( code )
    	ERROR(errno);

    rewind(ctPtr->textStream);

    /* write the new information */
    fprintf(ctPtr->textStream, "%d %d\n",
	    BAK_SCHEDULE_MAGIC, CUR_BAK_SCHEDULE_VERSION);

    lock_ObtainRead(&(bc_globalConfig->dschedLock));
    for(tdump = bc_globalConfig->dsched; tdump; tdump = tdump->next)
    {
        fprintf(ctPtr->textStream, "%s %s %d %d\n",tdump->name, "any",
                tdump->expDate, tdump->expType);
    }
    lock_ReleaseRead(&(bc_globalConfig->dschedLock));
    if (ferror(ctPtr->textStream))
    	return(BC_INTERNALERROR);

    fflush(ctPtr->textStream);			/* debug */

    /* send to server */
    code = bcdb_SaveTextFile(ctPtr);
    if ( code )
    	ERROR(code);

    /* increment local version number */
    ctPtr->textVersion++;

    /* update locally stored file size */
    ctPtr->textSize = filesize(ctPtr->textStream);
error_exit:
    return(code);
}


/* ------------------------------------
 * misc. support routines - specific to dump schedules
 * ------------------------------------
 */

long bc_UpdateDumpSchedule(ctPtr)
    udbClientTextP ctPtr;
{
    struct bc_dumpSchedule *dumpPtr, *nextDumpPtr;
    struct udbHandleS *uhptr = &udbHandle;
    long code;

    code = bc_CheckTextVersion(ctPtr);
    if ( code == 0 )
    {
	/* version matches, nothing to do */
	goto error_exit;
    }
    else if ( code != BC_VERSIONMISMATCH ) {
      /* some other error */
      com_err(whoami, code,
	   "; Unable to contact bakserver; Using dump schedule info in cache");
      ERROR(code);
    }

    /* must update the dump schedules */
    dce_svc_printf(BAK_S_OBSOLETE_DUMP_SCHED_MSG );
    lock_ObtainWrite(&(bc_globalConfig->dschedLock));
    /* clear all old schedule information */
    dumpPtr = bc_globalConfig->dsched;
    while ( dumpPtr )   {
      nextDumpPtr = dumpPtr->next;
      (void)osi_Free(dumpPtr, sizeof(struct bc_dumpSchedule));
      dumpPtr = nextDumpPtr;
    }
    bc_globalConfig->dsched = 0;
    lock_ReleaseWrite(&(bc_globalConfig->dschedLock));
    /* now get a fresh set of information from the database */
    code = bcdb_GetTextFile(ctPtr);
    if ( code )
        ERROR(code);

     /* fetch the version number */
    code = ubik_Call(BUDB_GetTextVersion,  uhptr->uh_client, 0,
                     ctPtr->textType, &ctPtr->textVersion);
    if ( code )
      return(code);

    /* parse the file */
    code = bc_ParseDumpSchedule();
    if ( code )
    	ERROR(code);

    /* rebuild the tree */
    code = bc_ProcessDumpSchedule(bc_globalConfig);
    if ( code )
    	ERROR(code);

error_exit:
    return(code);
}

/* ListDumpSchedule
 *	Print out the dump schedule tree whose root is adump. Alevel should
 *	be passed in as 0, and is incremented for the recursive calls
 * entry:
 *	adump - ptr to the root node of a dump schedule
 *	alevel - 0
 */

static long ListDumpSchedule(adump, alevel)
int alevel;
register struct bc_dumpSchedule *adump; {
    register int i;
    register struct bc_dumpSchedule *child;

    char *tailCompPtr();

    /* sanity check for loops */
    if (alevel > 100) {
	dce_svc_printf(BAK_S_TOO_MANY_SCHED_LEVELS_MSG);
	return 0;
    }

    /* move to appropriate indentation level */
    for(i=0; i<alevel; i++)
	dce_printf(bak_s_indent);

    /* name is a pathname style name, determine trailing name and only print
     * it
     */

    dce_printf(bak_s_slash_string, tailCompPtr(adump->name));


    /* list expiration time */
    switch ( adump->expType )
    {
      case BC_ABS_EXPDATE:
	/* absolute expiration date. Never expires if date is 0 */
	if ( adump->expDate )
	{
	    dce_printf(bak_s_expires_at, ktime_DateOf(adump->expDate));
	}
	break;

      case BC_REL_EXPDATE:
	{
	    struct ktime_date kt;

	    /* expiration date relative to the time that the dump is done */
	    LongTo_ktimeRelDate(adump->expDate, &kt);
	    dce_printf(bak_s_expires_in, RelDatetoString(&kt));
	}
	break;

      default:
	break;
    }
    dce_printf(bak_s_newline);
    for(child = adump->firstChild; child; child = child->nextSibling)
	(void)ListDumpSchedule(child, alevel+1);

    return 0;
}

/* bc_ParseExpiration
 *
 * Notes:
 *	dates are specified as absolute or relative, the syntax is:
 *	absolute:	at %d/%d/%d [%d:%d]	where [..] is optional
 *	relative:	in [%dy][%dm][%dd]	where at least one component
 *						must be specified
 */

long
bc_ParseExpiration(paramPtr, expType, expDate)
     struct cmd_parmdesc *paramPtr;
     long *expType;
     long *expDate;
{
  struct cmd_item *itemPtr, *tempPtr;
  struct ktime_date kt;
  char *dateString=0;
  long length = 0;
  long code = 0;

  if ( paramPtr->items == 0 ) {
    /* no expiration specified */
    *expType = BC_NO_EXPDATE;
    *expDate = 0;
    return(0);
  }
  /* some form of expiration date specified. First validate the prefix */
  itemPtr = paramPtr->items;
  if ( strcmp(itemPtr->data, "at") == 0 )
    *expType = BC_ABS_EXPDATE;
  else if ( strcmp(itemPtr->data, "in") == 0 )
    *expType = BC_REL_EXPDATE;
  else  {
    code=BC_BADEXPDATE;
    goto error;
  }
  /* before parsing the date string - concatenate all the pieces */
  itemPtr = itemPtr->next;
  tempPtr = itemPtr;

  /* compute the length of string required */
  while ( tempPtr != 0 ) {
    length += strlen(tempPtr->data);
    tempPtr = tempPtr->next;
    length++;				/* space or null terminator */
  }
  if ( length == 0 )	{			/* no actual date string */
    code = BC_BADEXPDATE;
    goto error;
  }
  dateString = (char *)osi_Alloc(length);
  if ( dateString == 0 ) {
    code = BC_NOMEM;
    goto error;
  }
  /* now assemble the date string */
  dateString[0] = 0;
  while ( itemPtr != 0 )  {
    strcat(dateString, itemPtr->data);
    itemPtr = itemPtr->next;
    if ( itemPtr != 0 )
      strcat(dateString, " ");
  }
  switch ( *expType ) {
  case BC_ABS_EXPDATE:
    code = ktime_DateToLong(dateString, expDate);
    if ( code )
      goto error;
    break;
  case BC_REL_EXPDATE:
    code = ParseRelDate(dateString, &kt);
    if ( code )
      goto error;
    *expDate = ktimeRelDate_ToLong(&kt);
    break;
  default:
    code = BC_BADEXPDATE;
    goto error;
  }

exit:
  /* normal exit */
  if ( dateString )
    osi_Free(dateString, length);
  return(code);

error:
  /* assign default values */
  *expType = BC_NO_EXPDATE;
  *expDate = 0;
  goto exit;
}
