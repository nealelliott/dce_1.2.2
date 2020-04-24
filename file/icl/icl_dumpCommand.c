/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */

/*
 * Copyright (C) 1992, 1996 Transarc Corporation.
 * All Rights Reserved.
 */

/* 
 * HISTORY
 * $Log: icl_dumpCommand.c,v $
 * Revision 1.1.65.1  1996/10/02  17:52:08  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:40:50  damon]
 *
 * $EndLog$
 */

/*
 * icl_dumpCommand.c -- 
 *    This file contains the command to dump out the DFS cache manager
 *    trace log (ICL) in a readable fashion.  It simply calls the 
 *    icl_DumpKernel() function.
 */

#include <stdio.h>
#include <time.h>
#include <errno.h>
#include <dce/dce_error.h>
#include <dce/rpc.h>
#include <dcedfs/stds.h>
#include <dcedfs/syscall.h>
#include <dcedfs/icl.h>
#include <icl_rpc.h>
#include <icl_errs.h>
#include <icl_userint.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#define BUFFER_MULTIPLIER     1024
#define CDSENTRYPRESENT(as) cmd_IsParamPresent((as), COMMON_ARG_CDSENTRY_INDEX)

extern char *sys_errlist[];
static void DCE_ERROR(FILE *fp, char *call, unsigned32 status)
{
    int __st;
    dce_error_string_t __error_buffer;
    unsigned char *__ep;
    __error_buffer[0] = '\0';
    if (status < 256) {
	__ep = (unsigned char *)sys_errlist[status];
    }
    else {
	dce_error_inq_text(status, __error_buffer, &__st);
	__ep = __error_buffer;
    }
    fprintf(fp,"dfstrace: %s failed: %s\n", call, __ep);
}

static void DCE_WARNING(FILE *fp, char *call, unsigned32 status)
{
    int __st;
    dce_error_string_t __error_buffer;
    unsigned char *__ep;
    __error_buffer[0] = '\0';
    if (status < 256) {
	__ep = (unsigned char *)sys_errlist[status];
    }
    else {
	dce_error_inq_text(status, __error_buffer, &__st);
	__ep = __error_buffer;
    }
    fprintf(fp,"dfstrace: warning in %s: %s\n", call, __ep);
}

#define MAP_ENOENT_SET(e) (((e) == ENOENT) ? ICL_ERROR_NOSET : e)
#define MAP_ENOENT_LOG(e) (((e) == ENOENT) ? ICL_ERROR_NOLOG : e)

/* define globals to use for bulk info */
PRIVATE icl_bulkSetinfo_t *setInfo = (icl_bulkSetinfo_t *)0;
PRIVATE icl_bulkLoginfo_t *logInfo = (icl_bulkLoginfo_t *)0;
PRIVATE rpc_binding_handle_t bindingHandle = (rpc_binding_handle_t)0;

PRIVATE void SetUpBindingHandle(cdsentry)
char *cdsentry;
{
    unsigned32 code;

    if (bindingHandle)
	return;		/* already set up */

    /* connect to server */
    code = icl_ConnectToServer(cdsentry, &bindingHandle);
    if (code)
    {
	DCE_ERROR(stderr, "connection to server", code);
	exit(1);
    }
}

PRIVATE icl_bulkSetinfo_t *GetBulkSetInfo()
{
    unsigned int infoSize;

    infoSize = sizeof(icl_bulkSetinfo_t) + 
	(ICL_RPC_MAX_SETS-1) * sizeof(icl_setinfo_t);
    if (!setInfo) 
    {
	setInfo = (icl_bulkSetinfo_t *)malloc(infoSize);
	if (!setInfo)
	{
	    DCE_ERROR(stderr, "initialization", ICL_EMEM_BULK_SET);
	    exit (1);
	}
    }
    bzero((char *)setInfo, infoSize);

    return setInfo;
}

PRIVATE icl_bulkLoginfo_t *GetBulkLogInfo()
{
    unsigned int infoSize;

    infoSize = sizeof(icl_bulkLoginfo_t) + 
	(ICL_RPC_MAX_LOGS-1) * sizeof(icl_loginfo_t);
    if (!logInfo)
    {
	logInfo = (icl_bulkLoginfo_t *)malloc(infoSize);
	if (!logInfo)
	{
	    DCE_ERROR(stderr, "initialization", ICL_EMEM_BULK_LOG);
	    exit (1);
	}
    }

    bzero((char *)logInfo, infoSize);
    return logInfo;
}

PRIVATE void SetUpCdsEntry(aSyntax)
struct cmd_syndesc *	aSyntax;
{
    (void)cmd_Seek(aSyntax, COMMON_ARG_CDSENTRY_INDEX);
    (void)cmd_AddParm(aSyntax, "-cdsentry", CMD_SINGLE, CMD_OPTIONAL, "server_entry_in_CDS");
}

PRIVATE DoDump(as, arock)
  register struct cmd_syndesc *as;
  char *arock;
{
    long code = 0;
    long tcode;
    long waitTime = 10 /* seconds */;
    char *logname;
    char *filename;
    idl_char fnBuff[ICL_RPC_MAX_FILE_NAME];
    FILE *outfp = stdout;
    time_t startTime;
    struct cmd_item *	itemp;
    int dumpStyle = ICL_DUMP_FORMATTED;

    if (CDSENTRYPRESENT(as)) {
	int i;
	icl_bulkSetinfo_t *setinfo;

	if (cmd_IsParamPresent(as, DUMP_ARG_FOLLOW_INDEX)) {
	    DCE_ERROR(stderr, "dump", ICL_BAD_FOLLOW_ARG);
	    code = 1;
	}
	if (cmd_IsParamPresent(as, DUMP_ARG_SLEEP_INDEX)) {
	    DCE_ERROR(stderr, "dump", ICL_BAD_SLEEP_ARG);
	    code = 1;
	}

	if (cmd_IsParamPresent(as, DUMP_ARG_LOG_INDEX)) {
	    DCE_ERROR(stderr, "dump", ICL_BAD_LOG_ARG);
	    code = 1;
	}

	if (cmd_IsParamPresent(as, DUMP_ARG_RAW_INDEX)) {
	    DCE_ERROR(stderr, "dump", ICL_BAD_RAW_ARG);
	    code = 1;
	}

	if (cmd_IsParamPresent(as, DUMP_ARG_FILE_INDEX)) {
	    char *p;
	    filename = as->parms[DUMP_ARG_FILE_INDEX].items->data;
	    if (p = strchr(filename, '/')) {
		DCE_ERROR(stderr, "dump", ICL_BAD_FILENAME_ARG);
		code = 1;
	    }
	} else {
	    filename = "";
	}

	if (!code) {
	    int all = 0;
	    setinfo = GetBulkSetInfo();

	    if (cmd_IsParamPresent(as, DUMP_ARG_SET_INDEX)) {
		int i;

		for (i = 0, itemp = as->parms[DUMP_ARG_SET_INDEX].items; 
		     itemp && (i < ICL_RPC_MAX_SETS); 
		     itemp = itemp->next, i++) {
		    (void) strncpy((char *)(setinfo->setinfo[i].setName),
				   itemp->data,
				   sizeof(setinfo->setinfo[i].setName)-1);
		}
		if (itemp) {
		    DCE_WARNING(stderr, "dump", ICL_W_MAX_SETS_EXCEEDED);
		    code = 1;
		}
		setinfo->count = i;
	    } else {
		all = 1;	/* do 'em all */
		setinfo->count = ICL_RPC_MAX_SETS;
	    }

	    strncpy((char *)&fnBuff[0], filename, ICL_RPC_MAX_FILE_NAME);
	    fnBuff[ICL_RPC_MAX_FILE_NAME-1] = '\0';
	    SetUpBindingHandle(as->parms[COMMON_ARG_CDSENTRY_INDEX].items->data);
	    tcode = DFSTRACE_DumpSet(bindingHandle, fnBuff, all, setinfo);
	    if (tcode)
	    {
		DCE_ERROR(stderr, "dump", tcode);
		exit(1);
	    }

	    /* check status */
	    for (i = 0; i < setinfo->count; i++) {
		if (setinfo->setinfo[i].states != 0) {
		    char errbuf[64];
		    (void) sprintf(errbuf, "dumping of %s", 
				   setinfo->setinfo[i].setName);
		    DCE_WARNING(stderr, errbuf, setinfo->setinfo[i].states);
		    code = 1;
		}
	    }
	}
	return (code);
    }

    if (cmd_IsParamPresent(as, DUMP_ARG_SLEEP_INDEX))  {
	if (!cmd_IsParamPresent(as, DUMP_ARG_FOLLOW_INDEX)) {
	    DCE_ERROR(stderr, "dump", ICL_SLEEP_WO_FOLLOW);
	    return 1;
	}
	waitTime = strtol(as->parms[DUMP_ARG_SLEEP_INDEX].items->data, 
			  (char **)0, 0);
    }

    if (cmd_IsParamPresent(as, DUMP_ARG_FILE_INDEX)) {
	/* try to open the specified output file */
	if ((outfp = fopen(as->parms[DUMP_ARG_FILE_INDEX].items->data, "w"))
	    == NULL) {
	    char errbuf[64];
	    (void) sprintf(errbuf, "opening output file '%s'", 
			   as->parms[DUMP_ARG_FILE_INDEX].items->data);
	    DCE_ERROR(stderr, errbuf, errno);
	    return 1;
	}
    }

    if (cmd_IsParamPresent(as, DUMP_ARG_RAW_INDEX)) {
	dumpStyle = ICL_DUMP_RAW;
    }

    startTime = time((long *) 0);
    (void) fprintf(outfp, "DFS Trace Dump -\n\n   Date: %s\n",
		   ctime(&startTime));

    if (cmd_IsParamPresent(as, DUMP_ARG_SET_INDEX)) {
	for (itemp = as->parms[DUMP_ARG_SET_INDEX].items;
	     itemp;
	     itemp = itemp->next) {
	    tcode = icl_DumpKernel(outfp, ICL_DUMP_SET, itemp->data,
				   dumpStyle);
	    if (tcode) {
		char errbuf[64];
		(void) sprintf(errbuf, "dumping set '%s'", itemp->data);
		DCE_ERROR(stderr, errbuf, MAP_ENOENT_SET(errno));
		code = tcode;
	    }
	}
    } else if (cmd_IsParamPresent(as, DUMP_ARG_LOG_INDEX)) {
	for (itemp = as->parms[DUMP_ARG_LOG_INDEX].items;
	     itemp;
	     itemp = itemp->next) 
	{
	    tcode = icl_DumpKernel(outfp, ICL_DUMP_LOG, itemp->data, 
				   dumpStyle);
	    if (tcode) {
		char errbuf[64];
		(void) sprintf(errbuf, "dumping log '%s'", itemp->data);
		DCE_ERROR(stderr, errbuf, MAP_ENOENT_LOG(errno));
		code = tcode;
	    }
	}
    } else if (cmd_IsParamPresent(as, DUMP_ARG_FOLLOW_INDEX)) {
	logname = as->parms[DUMP_ARG_FOLLOW_INDEX].items->data;
	code = icl_TailKernel(outfp, logname, waitTime, dumpStyle);
	if (code) {
	    char errbuf[64];
	    (void) sprintf(errbuf, "following log '%s'", logname);
	    DCE_ERROR(stderr, errbuf, MAP_ENOENT_LOG(code));
	}
    } else {
	code = icl_DumpKernel(outfp, ICL_DUMP_ALL, NULL, dumpStyle);
    }

    (void) fprintf(outfp, "\nDFS Trace Dump - %s\n",
		   code ? "FAILED" : "Completed");

    if (outfp != stdout)
	(void) fclose(outfp);

    return code;
}

PRIVATE void SetUpDump()
{
    struct cmd_syndesc *dumpSyntax;
    
    dumpSyntax = cmd_CreateSyntax("dump", DoDump, (char *)NULL,
				  "dump DFS trace logs");
    
    (void)cmd_IndexedAddParm(dumpSyntax, "-set", CMD_LIST, CMD_OPTIONAL,
			     "set_name", DUMP_ARG_SET_INDEX);
    (void)cmd_IndexedAddParm(dumpSyntax, "-follow", CMD_SINGLE, CMD_OPTIONAL,
			     "log_name", DUMP_ARG_FOLLOW_INDEX);
    (void)cmd_IndexedAddParm(dumpSyntax, "-log", CMD_LIST, CMD_OPTIONAL,
			     "log_name", DUMP_ARG_LOG_INDEX);

    (void)cmd_IndexedAddParm(dumpSyntax, "-file", CMD_SINGLE, CMD_OPTIONAL,
			     "output_filename", DUMP_ARG_FILE_INDEX);
    (void)cmd_IndexedAddParm(dumpSyntax, "-sleep", CMD_SINGLE, 
			     CMD_OPTIONAL | CMD_PARM_NUMERIC,
			     "seconds_between_reads", DUMP_ARG_SLEEP_INDEX);

    (void)cmd_IndexedAddParm(dumpSyntax, "-raw", CMD_FLAG, CMD_OPTIONAL,
			     "dump raw log", DUMP_ARG_RAW_INDEX);


    (void)cmd_MakeAlternate(dumpSyntax, "-set", "-follow");
    (void)cmd_MakeAlternate(dumpSyntax, "-set", "-log");

    SetUpCdsEntry(dumpSyntax);
}

PRIVATE DoShowLog(as, arock)
  register struct cmd_syndesc *as;
  char *arock;
{
    long retVal = 0;
    long code = 0;
    long logSize;
    int allocated;
    int longflg;
    idl_char snbuff[32];
    struct cmd_item *	itemp;

    longflg = cmd_IsParamPresent(as, SHOWL_ARG_LONG_INDEX);

    if (CDSENTRYPRESENT(as))
    {
	char errbuf[128];
	icl_bulkLoginfo_t *loginfo;
	int i;
	int all = 0;

	SetUpBindingHandle(as->parms[COMMON_ARG_CDSENTRY_INDEX].items->data);
	if (cmd_IsParamPresent(as, SHOWL_ARG_SET_INDEX))  
	{
	    /* enumerate logs for the specified sets */
	    for (itemp = as->parms[SHOWL_ARG_SET_INDEX].items; itemp; itemp = itemp->next) 
	    {
		strncpy((char *)&snbuff[0], itemp->data, sizeof(snbuff));
		snbuff[sizeof(snbuff)-1] = '\0';
		loginfo = GetBulkLogInfo();
		loginfo->count = ICL_LOGSPERSET;
		code = DFSTRACE_GetLogInfo(bindingHandle, snbuff, /* !all */0, loginfo);
		if (code) 
		{
		    (void) sprintf(errbuf, "listing logs for set '%s'", itemp->data);
		    DCE_ERROR(stderr, errbuf, code);
		    retVal = 1;
		    continue;
		}
		
		(void) fprintf(stdout, "Logs for set '%s':\n", itemp->data);
		for(i = 0; i < loginfo->count; i++)
		{
		    if (longflg)
		    {
			(void) fprintf(stdout, "%s : %d kwords (%s)\n", 
				       loginfo->loginfo[i].logName,
				       loginfo->loginfo[i].logSize/1024,
				       (loginfo->loginfo[i].logElements >
				       loginfo->loginfo[i].logSize) ?
				       "unallocated" : "allocated");
		    }
		    else
			(void) fprintf(stdout, "%s\n", loginfo->loginfo[i].logName);
		}
	    }
	}
	else 
	{
	    loginfo = GetBulkLogInfo();
	    if (cmd_IsParamPresent(as, SHOWL_ARG_LOG_INDEX))  
	    {
		/* print out log information */
		for (i = 0, itemp = as->parms[SHOWL_ARG_LOG_INDEX].items; 
		     itemp && (i < ICL_RPC_MAX_LOGS);
		     itemp = itemp->next, i++) 
		{
		    (void) strncpy((char *)(loginfo->loginfo[i].logName), itemp->data,
				   sizeof(loginfo->loginfo[i].logName)-1);
		}
		if (itemp)
		{
		    DCE_WARNING(stderr, "lslog", ICL_W_MAX_LOGS_EXCEEDED);
		    retVal = 1;
		}
		loginfo->count = i;
	    }
	    else
	    {
		/* show all logs */
		(void) fprintf(stdout, "Available logs:\n");
		all = 1;	/* do 'em all */
		loginfo->count = ICL_RPC_MAX_SETS;
	    }
	    
	    strncpy((char *)&snbuff[0], "", sizeof(snbuff));
	    code = DFSTRACE_GetLogInfo(bindingHandle, snbuff, all, loginfo);
	    if (code) 
	    {
		DCE_ERROR(stderr, "GetLogInfo", code);
		exit(1);
	    }
	    
	    /* check status */
	    for(i = 0; i < loginfo->count; i++)
	    {
		if (loginfo->loginfo[i].states > 16)
		{
		    char errbuf[64];
		    (void) sprintf(errbuf, "listing of log '%s'", 
				   loginfo->loginfo[i].logName);
		    DCE_WARNING(stderr, errbuf, loginfo->loginfo[i].states);
		    retVal = 1;
		}
		else
		{
		    if (longflg)
		    {
			(void) fprintf(stdout, "%s : %d kwords (%s)\n", 
				       loginfo->loginfo[i].logName,
				       loginfo->loginfo[i].logSize/1024,
				       (loginfo->loginfo[i].logElements >
				       loginfo->loginfo[i].logSize) ?
				       "unallocated" : "allocated");
		    }
		    else
			(void) fprintf(stdout, "%s\n", loginfo->loginfo[i].logName);
		}
	    }
	}	    
	return (retVal);
    }

    if (cmd_IsParamPresent(as, SHOWL_ARG_SET_INDEX))  {
	/* enumerate logs for the specified sets */
	for (itemp = as->parms[SHOWL_ARG_SET_INDEX].items; itemp; itemp = itemp->next) {
	    (void) fprintf(stdout, "Logs for set '%s':\n", itemp->data);
	    code = icl_ListLogsBySet(stdout, itemp->data, longflg);
	    if (code) {
		char errbuf[64];
		(void) sprintf(errbuf, "listing of logs for set '%s'", itemp->data);
		DCE_ERROR(stderr, errbuf, MAP_ENOENT_SET(errno));
		retVal = code;
	    }
	}
    }
    else if (cmd_IsParamPresent(as, SHOWL_ARG_LOG_INDEX))  {
	/* print out log information */
	for (itemp = as->parms[SHOWL_ARG_LOG_INDEX].items; itemp; itemp = itemp->next) {
	    code = icl_GetLogsize(itemp->data, &logSize, &allocated);
	    if (!code) {
		if (longflg) 
		    (void) fprintf(stdout, "%s : %d kwords (%s)\n", itemp->data,
				   logSize/1024, allocated ? "allocated" : "unallocated");
		else
		    (void) fprintf(stdout, "%s\n", itemp->data);
	    }
	    else {
		char errbuf[64];
		(void) sprintf(errbuf, "listing of log '%s'", itemp->data);
		DCE_ERROR(stderr, errbuf, MAP_ENOENT_LOG(errno));
		retVal = code;
	    }
	}
    }
    else {
	/* show all logs */
	(void) fprintf(stdout, "Available logs:\n");
	code = icl_ListLogs(stdout, longflg);
	if (code) {
	    DCE_ERROR(stderr, "listing logs", MAP_ENOENT_LOG(errno));
	    retVal = code;
	}
    }

    return retVal;
}

PRIVATE void SetUpShowLog()
{
    struct cmd_syndesc *showSyntax;
    
    showSyntax = cmd_CreateSyntax("lslog", DoShowLog, (char *)NULL,
				  "list available logs");
    
    (void)cmd_IndexedAddParm(showSyntax, "-set", CMD_LIST, CMD_OPTIONAL,
			     "set_name", SHOWL_ARG_SET_INDEX);
    (void)cmd_IndexedAddParm(showSyntax, "-log", CMD_LIST, CMD_OPTIONAL,
			     "log_name", SHOWL_ARG_LOG_INDEX);
    (void)cmd_IndexedAddParm(showSyntax, "-long", CMD_FLAG, CMD_OPTIONAL,
			     "", SHOWL_ARG_LONG_INDEX);

    (void)cmd_MakeAlternate(showSyntax, "-set", "-log");
    SetUpCdsEntry(showSyntax);
}

PRIVATE DoShowSet(as, arock)
  register struct cmd_syndesc *as;
  char *arock;
{
    long retVal = 0;
    long code = 0;
    long state;
    struct cmd_item *	itemp;

    if (CDSENTRYPRESENT(as))
    {
	char errbuf[128];
	icl_bulkSetinfo_t *setinfo;
	int i;
	int all = 0;

	SetUpBindingHandle(as->parms[COMMON_ARG_CDSENTRY_INDEX].items->data);
	setinfo = GetBulkSetInfo();
	if (cmd_IsParamPresent(as, SHOWS_ARG_SET_INDEX))  {
	    /* print information on the specified sets */
	    for (i = 0, itemp = as->parms[SHOWS_ARG_SET_INDEX].items; 
		 itemp && (i < ICL_RPC_MAX_SETS);
		 itemp = itemp->next, i++) 
	    {
		(void) strncpy((char *)(setinfo->setinfo[i].setName), itemp->data,
			       sizeof(setinfo->setinfo[i].setName)-1);
	    }
	    if (itemp)
	    {
		DCE_WARNING(stderr, "lsset", ICL_W_MAX_SETS_EXCEEDED);
		retVal = 1;
	    }
	    setinfo->count = i;
	}
	else
	{
	    /* show all sets */
	    (void) fprintf(stdout, "Available sets:\n");
	    all = 1;	/* do 'em all */
	    setinfo->count = ICL_RPC_MAX_SETS;
	}
	
	code = DFSTRACE_GetSetInfo(bindingHandle, all, setinfo);
	if (code) 
	{
	    DCE_ERROR(stderr, "listing sets", code);
	    exit(1);
	}
	
	/* check status */
	for(i = 0; i < setinfo->count; i++)
	{
	    if (setinfo->setinfo[i].states > 16)
	    {
		char errbuf[64];
		(void) sprintf(errbuf, "listing of set '%s'", 
			       setinfo->setinfo[i].setName);
		DCE_WARNING(stderr, errbuf, setinfo->setinfo[i].states);
		retVal = 1;
	    }
	    else
	    {
		unsigned32 states;
		states = setinfo->setinfo[i].states;
		(void) fprintf(stdout, "%s: %s%s\n", 
			       setinfo->setinfo[i].setName,
			       (states & ICL_SETF_ACTIVE) ? "active" : 
			       ((states & ICL_SETF_FREED) ? "dormant" : "inactive"),
			       (states & ICL_SETF_PERSISTENT) ? " persistent" : "");
	    }
	}
	return (retVal);
    }

    if (cmd_IsParamPresent(as, SHOWS_ARG_SET_INDEX))  {
	/* print information on the specified sets */
	for (itemp = as->parms[SHOWS_ARG_SET_INDEX].items; itemp; itemp = itemp->next) {
	    code = icl_GetSetState(itemp->data, &state);
	    if (code) {
		char errbuf[64];
		(void) sprintf(errbuf, "getting status on set '%s'", itemp->data);
		DCE_ERROR(stderr, errbuf, MAP_ENOENT_SET(errno));
		retVal = code;
	    }
	    else
		(void) fprintf(stdout, "%s: %s%s\n", itemp->data,
			       (state & ICL_SETF_ACTIVE) ? "active" : 
			       ((state & ICL_SETF_FREED) ? "dormant" : "inactive"),
			       (state & ICL_SETF_PERSISTENT) ? " persistent" : "");
	}
    }
    else {
	/* show all sets */
	(void) fprintf(stdout, "Available sets:\n");
	code = icl_ListSets(stdout);
	if (code) {
	    DCE_ERROR(stderr, "listing all sets", MAP_ENOENT_SET(errno));
	    retVal = code;
	}
    }

    return retVal;
}

PRIVATE void SetUpShowSet()
{
    struct cmd_syndesc *showSyntax;
    
    showSyntax = cmd_CreateSyntax("lsset", DoShowSet, (char *)NULL,
				  "list available event sets");
    
    (void)cmd_IndexedAddParm(showSyntax, "-set", CMD_LIST, CMD_OPTIONAL,
			     "set_name", SHOWS_ARG_SET_INDEX);

    SetUpCdsEntry(showSyntax);
}

PRIVATE DoClear(as, arock)
  register struct cmd_syndesc *as;
  char *arock;
{
    long retVal = 0;
    long code = 0;
    struct cmd_item *	itemp;

    if (CDSENTRYPRESENT(as))
    {
	char errbuf[128];
	icl_bulkSetinfo_t *setinfo;
	icl_bulkLoginfo_t *loginfo;
	int i;
	int all = 0;

	SetUpBindingHandle(as->parms[COMMON_ARG_CDSENTRY_INDEX].items->data);
	if (cmd_IsParamPresent(as, CLR_ARG_SET_INDEX))  
	{
	    /* clear logs for the specified sets */
	    setinfo = GetBulkSetInfo();
	    for (i = 0, itemp = as->parms[CLR_ARG_SET_INDEX].items; 
		 itemp && (i < ICL_RPC_MAX_SETS);
		 itemp = itemp->next, i++) 
	    {
		(void) strncpy((char *)(setinfo->setinfo[i].setName), itemp->data,
			       sizeof(setinfo->setinfo[i].setName)-1);
	    }
	    if (itemp)
	    {
		DCE_WARNING(stderr, "clear", ICL_W_MAX_SETS_EXCEEDED);
		retVal = 1;
	    }
	    setinfo->count = i;

	    code = DFSTRACE_ClearSet(bindingHandle, /*!all*/0, setinfo);
	    if (code) 
	    {
		DCE_ERROR(stderr, "clear", code);
		exit(1);
	    }
	    
	    /* check status */
	    for(i = 0; i < setinfo->count; i++)
	    {
		if (setinfo->setinfo[i].states != 0)
		{
		    char errbuf[64];
		    (void) sprintf(errbuf, "clearing set '%s'", 
				   setinfo->setinfo[i].setName);
		    DCE_WARNING(stderr, errbuf, setinfo->setinfo[i].states);
		    retVal = 1;
		}
	    }
	}
	else 
	{
	    loginfo = GetBulkLogInfo();
	    if (cmd_IsParamPresent(as, CLR_ARG_LOG_INDEX))  
	    {
		/* clear specified log */
		for (i = 0, itemp = as->parms[CLR_ARG_LOG_INDEX].items; 
		     itemp && (i < ICL_RPC_MAX_LOGS);
		     itemp = itemp->next, i++) 
		{
		    (void) strncpy((char *)(loginfo->loginfo[i].logName), itemp->data,
				   sizeof(loginfo->loginfo[i].logName)-1);
		}
		if (itemp)
		{
		    DCE_WARNING(stderr, "clear", ICL_W_MAX_LOGS_EXCEEDED);
		    retVal = 1;
		}
		loginfo->count = i;
	    }
	    else
	    {
		/* clear all logs */
		all = 1;	/* do 'em all */
		loginfo->count = 0;
	    }
	
	    code = DFSTRACE_ClearLog(bindingHandle, all, loginfo);
	    if (code) 
	    {
		DCE_ERROR(stderr, "clear", code);
		exit(1);
	    }
	    
	    /* check status */
	    for(i = 0; i < loginfo->count; i++)
	    {
		if (loginfo->loginfo[i].states != 0)
		{
		    char errbuf[64];
		    (void) sprintf(errbuf, "clearing log '%s'", 
				   loginfo->loginfo[i].logName);
		    DCE_WARNING(stderr, errbuf, loginfo->loginfo[i].states);
		    retVal = 1;
		}
	    }
	}

	return (retVal);
    }

    if (cmd_IsParamPresent(as, CLR_ARG_SET_INDEX))  {
	/* clear logs for the specified sets */
	for (itemp = as->parms[CLR_ARG_SET_INDEX].items; itemp; itemp = itemp->next) {
	    code = icl_ClearSet(itemp->data);
	    if (code) {
		char errbuf[64];
		(void) sprintf(errbuf, "clearing set '%s'", itemp->data);
		DCE_ERROR(stderr, errbuf, MAP_ENOENT_SET(errno));
		retVal = code;
	    }
	}
    }
    else if (cmd_IsParamPresent(as, CLR_ARG_LOG_INDEX))  {
	/* clear specified log */
	for (itemp = as->parms[CLR_ARG_LOG_INDEX].items; itemp; itemp = itemp->next) {
	    code = icl_ClearLog(itemp->data);
	    if (code) {
		char errbuf[64];
		(void) sprintf(errbuf, "clearing log '%s'", itemp->data);
		DCE_ERROR(stderr, errbuf, MAP_ENOENT_LOG(errno));
		retVal = code;
	    }
	}
    }
    else {
	/* clear all logs */
	code = icl_ClearAll();
	if (code) {
	    DCE_ERROR(stderr, "clearing all logs", MAP_ENOENT_LOG(errno));
	    retVal = code;
	}
    }

    return retVal;
}

PRIVATE void SetUpClear()
{
    struct cmd_syndesc *clearSyntax;
    
    clearSyntax = cmd_CreateSyntax("clear", DoClear, (char *)NULL,
				   "clear logs by logname or by event set");
    
    (void)cmd_IndexedAddParm(clearSyntax, "-set", CMD_LIST, CMD_OPTIONAL,
			     "set_name", CLR_ARG_SET_INDEX);
    (void)cmd_IndexedAddParm(clearSyntax, "-log", CMD_LIST, CMD_OPTIONAL,
			     "log_name", CLR_ARG_LOG_INDEX);

    (void)cmd_MakeAlternate(clearSyntax, "-set", "-log");
    SetUpCdsEntry(clearSyntax);
}

PRIVATE DoSet(as, arock)
  register struct cmd_syndesc *as;
  char *arock;
{
    long retVal = 0;
    long code = 0;
    int op;
    int doFree = 0;
    char *operation;
    struct cmd_item *	itemp;

    if (cmd_IsParamPresent(as, SET_ARG_ACT_INDEX)) {
	op = ICL_OP_SS_ACTIVATE;
	operation = "active";
    }
    else if (cmd_IsParamPresent(as, SET_ARG_DEACT_INDEX)) {
	op = ICL_OP_SS_DEACTIVATE;
	operation = "inactive";
    }
    else if (cmd_IsParamPresent(as, SET_ARG_FREE_INDEX)) {
	op = ICL_OP_SS_DEACTIVATE;
	operation = "inactive";
	doFree = 1;
    }
    else {
	/* assume active" */
	op = ICL_OP_SS_ACTIVATE;
	operation = "active";
    }

    if (CDSENTRYPRESENT(as))
    {
	char errbuf[128];
	icl_bulkSetinfo_t *setinfo;
	int i;
	int all = 0;

	SetUpBindingHandle(as->parms[COMMON_ARG_CDSENTRY_INDEX].items->data);
	setinfo = GetBulkSetInfo();
	if (cmd_IsParamPresent(as, SET_ARG_SET_INDEX))  {
	    /* set state on the specified sets */
	    for (i = 0, itemp = as->parms[SET_ARG_SET_INDEX].items; 
		 itemp && (i < ICL_RPC_MAX_SETS);
		 itemp = itemp->next, i++) 
	    {
		(void) strncpy((char *)(setinfo->setinfo[i].setName), itemp->data,
			       sizeof(setinfo->setinfo[i].setName)-1);
		setinfo->setinfo[i].states = op;
	    }
	    if (itemp)
	    {
		DCE_WARNING(stderr, "setset", ICL_W_MAX_SETS_EXCEEDED);
		retVal = 1;
	    }
	    setinfo->count = i;
	}
	else
	{
	    /* set all sets */
	    all = 1;	/* do 'em all */
	    setinfo->setinfo[0].states = op;
	    setinfo->count = 1;
	}
	
	code = DFSTRACE_SetSetInfo(bindingHandle, all, setinfo);
	if (code) 
	{
	    DCE_ERROR(stderr, "setset", code);
	    exit(1);
	}
	
	/* check status */
	for(i = 0; i < setinfo->count; i++)
	{
	    if (setinfo->setinfo[i].states == EINVAL)
	    {
		char errbuf[64];
		(void) sprintf(errbuf, "setting state of set '%s'", 
			       setinfo->setinfo[i].setName);
		DCE_ERROR(stderr, errbuf, ICL_INVALID_SET_OP);
		retVal = 1;
	    }
	    else if (setinfo->setinfo[i].states != 0)
	    {
		char errbuf[64];
		(void) sprintf(errbuf, "setting state of set '%s'", 
			       setinfo->setinfo[i].setName);
		DCE_WARNING(stderr, errbuf, setinfo->setinfo[i].states);
		retVal = 1;
	    }

	    /* set states to free opcode in case we need to free */
	    setinfo->setinfo[i].states = ICL_OP_SS_FREE;
       }

	if (doFree)
	{
	    if (all)
	    {
		/* reset bulk structure for 'all' */
		setinfo->count = 1;
		setinfo->setinfo[0].states = ICL_OP_SS_FREE;
	    }

	    code = DFSTRACE_SetSetInfo(bindingHandle, all, setinfo);
	    if (code) 
	    {
		DCE_ERROR(stderr, "setset", code);
		exit(1);
	    }
	    
	    /* check status */
	    for(i = 0; i < setinfo->count; i++)
	    {
		if (setinfo->setinfo[i].states == EINVAL)
		{
		    char errbuf[64];
		    (void) sprintf(errbuf, "setting state of set '%s'", 
				   setinfo->setinfo[i].setName);
		    DCE_ERROR(stderr, errbuf, ICL_INVALID_SET_OP);
		    retVal = 1;
		}
		else if (setinfo->setinfo[i].states != 0)
		{
		    char errbuf[64];
		    (void) sprintf(errbuf, "setting state of set '%s'", 
				   setinfo->setinfo[i].setName);
		    DCE_WARNING(stderr, errbuf, setinfo->setinfo[i].states);
		    retVal = 1;
		}
	    }
	}

	return (retVal);
    }

    if (cmd_IsParamPresent(as, SET_ARG_SET_INDEX))  {
	/* activate specified sets */
	for (itemp = as->parms[SET_ARG_SET_INDEX].items; itemp; itemp = itemp->next) {
	    code = icl_ChangeSetState(itemp->data, op);
	    if (code) {
		char errbuf[64];
		(void) sprintf(errbuf, "setting state of set '%s' to %s", 
			       itemp->data, operation);
		DCE_ERROR(stderr, errbuf, MAP_ENOENT_SET(errno));
		retVal = code;
	    }
	    else if (doFree) {
		/* try to make it dormant as well */
		code = icl_ChangeSetState(itemp->data, ICL_OP_SS_FREE);
		if (code) {
		    char errbuf[64];
		    (void) sprintf(errbuf, "setting state of set '%s' to dormant", 
				   itemp->data);
		    DCE_ERROR(stderr, errbuf, MAP_ENOENT_SET(errno));
		    retVal = code;
		}
	    }
	}
    }
    else {
	/* show all sets */
	code = icl_ChangeAllSetState(op);
	if (code) {
	    char errbuf[64];
	    (void) sprintf(errbuf, "setting state of all sets to %s", operation);
	    DCE_ERROR(stderr, errbuf, MAP_ENOENT_SET(errno));
	    retVal = code;
	}
	else if (doFree) {
	    /* try to make it dormant as well */
	    code = icl_ChangeAllSetState(ICL_OP_SS_FREE);
	    if (code) {
		DCE_ERROR(stderr, "setting state of all sets to dormant", MAP_ENOENT_SET(errno));
		retVal = code;
	    }
	}
    }

    return retVal;
}

PRIVATE void SetUpSet()
{
    struct cmd_syndesc *setSyntax;
    
    setSyntax = cmd_CreateSyntax("setset", DoSet, (char *)NULL,
				 "set state of event sets");
    
    (void)cmd_IndexedAddParm(setSyntax, "-set", CMD_LIST, CMD_OPTIONAL,
			     "set_name", SET_ARG_SET_INDEX);

    (void)cmd_IndexedAddParm(setSyntax, "-active", CMD_FLAG, CMD_OPTIONAL,
			     "", SET_ARG_ACT_INDEX);
    (void)cmd_IndexedAddParm(setSyntax, "-inactive", CMD_FLAG, CMD_OPTIONAL,
			     "", SET_ARG_DEACT_INDEX);
    (void)cmd_IndexedAddParm(setSyntax, "-dormant", CMD_FLAG, CMD_OPTIONAL,
			     "", SET_ARG_FREE_INDEX);

    (void)cmd_MakeAlternate(setSyntax, "-active", "-inactive");
    (void)cmd_MakeAlternate(setSyntax, "-active", "-dormant");
    (void)cmd_MakeAlternate(setSyntax, "-inactive", "-dormant");

    SetUpCdsEntry(setSyntax);
}

PRIVATE DoResize(as, arock)
  register struct cmd_syndesc *as;
  char *arock;
{
    long retVal = 0;
    long code = 0;
    long bufferSize;
    struct cmd_item *	itemp;

    /* get buffer size */
    bufferSize = strtol(as->parms[SSIZE_ARG_SIZE_INDEX].items->data, 
			(char **)0, 0);
    bufferSize *= BUFFER_MULTIPLIER;
    if (bufferSize == 0)
	bufferSize = ICL_DEFAULT_LOGSIZE;

    if (CDSENTRYPRESENT(as))
    {
	char errbuf[128];
	icl_bulkLoginfo_t *loginfo;
	int i;
	int all = 0;

	SetUpBindingHandle(as->parms[COMMON_ARG_CDSENTRY_INDEX].items->data);
	loginfo = GetBulkLogInfo();
	if (cmd_IsParamPresent(as, SSIZE_ARG_LOG_INDEX))  
	{
	    /* set log information on sepcified logs */
	    for (i = 0, itemp = as->parms[SSIZE_ARG_LOG_INDEX].items; 
		 itemp && (i < ICL_RPC_MAX_LOGS);
		 itemp = itemp->next, i++) 
	    {
		(void) strncpy((char *)(loginfo->loginfo[i].logName), itemp->data,
			       sizeof(loginfo->loginfo[i].logName)-1);
		loginfo->loginfo[i].logSize = bufferSize;
	    }
	    if (itemp)
	    {
		DCE_WARNING(stderr, "setlog", ICL_W_MAX_LOGS_EXCEEDED);
		retVal = 1;
	    }
	    loginfo->count = i;
	}
	else
	{
	    all = 1;	/* do 'em all */
	    loginfo->loginfo[0].logSize = bufferSize;
	    loginfo->count = 1;
	}
	
	code = DFSTRACE_SetLogInfo(bindingHandle, all, loginfo);
	if (code)
	{
	    DCE_ERROR(stderr, "setlog", code);
	    exit(1);
	}
	
	/* check status */
	for(i = 0; i < loginfo->count; i++)
	{
	    if (loginfo->loginfo[i].states != 0)
	    {
		(void) sprintf(errbuf, "setting size of log '%s'", 
			       loginfo->loginfo[i].logName);
		DCE_WARNING(stderr, errbuf, loginfo->loginfo[i].states);
		retVal = 1;
	    }
	}	    
	return (retVal);
    }

    /* set the size of the specified logs */
    for (itemp = as->parms[SSIZE_ARG_LOG_INDEX].items; itemp; itemp = itemp->next) {
	code = icl_ChangeLogSize(itemp->data, bufferSize);
	if (code) {
	    char errbuf[64];
	    (void) sprintf(errbuf, "setting size of log '%s'", itemp->data);
	    DCE_ERROR(stderr, errbuf, MAP_ENOENT_LOG(errno));
	    retVal = code;
	}
    }

    return retVal;
}

PRIVATE void SetUpResize()
{
    struct cmd_syndesc *setsizeSyntax;
    
    setsizeSyntax = cmd_CreateSyntax("setlog", DoResize, (char *)NULL,
				     "set the size of a log");
    
    (void)cmd_IndexedAddParm(setsizeSyntax, "-log", CMD_SINGLE, CMD_REQUIRED,
			     "log_name", SSIZE_ARG_LOG_INDEX);
    (void)cmd_IndexedAddParm(setsizeSyntax, "-buffersize", CMD_SINGLE, 
			     CMD_REQUIRED | CMD_PARM_NUMERIC,
			     "4-kilobyte_units", SSIZE_ARG_SIZE_INDEX);
    SetUpCdsEntry(setsizeSyntax);
}

main(argc, argv)
  IN int argc;
  IN char *argv[];
{
    osi_setlocale(LC_ALL, "");
    /* set up user interface then dispatch */
    SetUpDump();
    SetUpShowLog();
    SetUpShowSet();
    SetUpClear();
    SetUpSet();
    SetUpResize();

    return(cmd_Dispatch(argc, argv));
}

