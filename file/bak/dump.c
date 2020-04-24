/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright (C) 1996, 1991 Transarc Corporation
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: dump.c,v $
 * Revision 1.1.95.1  1996/10/02  16:58:39  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:03:52  damon]
 *
 * $EndLog$
 */
/*
 * dump.c -- Fileset dumps
 */ 

#include <dcedfs/stds.h>  /* include appropriate standards file */

#include <dcedfs/param.h>
#include <sys/param.h>
#include <sys/types.h>
#include <dcedfs/cmd.h>
#include <dce/rpc.h>
#include <pthread.h>
#include <dcedfs/compat.h>
#include <com_err.h>
#include <dcedfs/ftserver_data.h>
#include <dcedfs/tcdata.h>		  /* for checking TC_ABORTFAILED. PA */
#include <dcedfs/fldb_data.h>
#include <dcedfs/flclient.h>
#include <dcedfs/flserver.h>
#include <dcedfs/volc.h>
#include <dcedfs/volume.h>
#include <dcedfs/butc.h>
#include <error_macros.h>
#include <bc.h>
#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsbcmmac.h>
#include <dfsbcmsvc.h>
#include <dfsbcmmsg.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bak/dump.c,v 1.1.95.1 1996/10/02 16:58:39 damon Exp $")

#define dfprintf(x)

/* Can the volume be dumped  ? */

#define DUMP_VOL_NOTOK (VOL_DELONSALVAGE)

struct bc_dumpTask bc_dumpTasks[BC_MAXSIMDUMPS];

extern int nonInteractiveBak;
extern struct bc_config *bc_globalConfig;
extern char *bc_CopyString();
extern rpc_binding_handle_t bc_GetConn();
extern char *whoami;
extern int nonInteractiveBak;
extern statusP createStatusNode();

/* bc_Dumper
 *	called (indirectly) to make a dump
 * entry:
 *	dumpTaskPtr - ptr into dumpTask array, contains all the information 
 *		relevant to the dump
 */

bc_Dumper(dumpTaskPtr)
    struct bc_dumpTask * volatile dumpTaskPtr;
{
    volatile rpc_binding_handle_t tconn=(rpc_binding_handle_t)NULL;
    register struct bc_volumeDump *tde;
    volatile long count;
    struct tc_dumpArray * volatile volArray = (struct tc_dumpArray *)NULL;
    char * volatile baseNamePtr;
    volatile statusP statusPtr;
    struct tc_dumpInterface dumpInterface;
    struct tc_dumpInterface *tcdiPtr = &dumpInterface;
    volatile register long code=0;
    extern char *tailCompPtr();
    extern long bc_jobNumber();
    struct ftserver_status status;
    register struct bc_volumeDump *tde_1, *tde_2;
    int removefileset;
    
    /* create status monitor block */
    statusPtr = createStatusNode(BC_REFCOUNT);
    tconn = bc_GetConn( dumpTaskPtr->config, dumpTaskPtr->portOffset);
    if ( tconn == (rpc_binding_handle_t)NULL )
    {
	code = BC_NOCONN;
	com_err(whoami, code,
		"; Unable to connect to tape coordinator at tcid %d",
		dumpTaskPtr->portOffset);
	goto error_exit;
    }

    code = CheckTCVersion(tconn);
    if ( code )
    {
	com_err(whoami, code, 
		"; Unable to contact tape coordinator at tcid %d",
		dumpTaskPtr->portOffset);
	goto error_exit;
    }
    
    /* count volumes to be dumped  and throw out inconsistent volumes */
    count = 0;

    tde_1 = NULL;
    for( tde=dumpTaskPtr->volumes; tde; tde = tde_2) {
      
      /* Check for validity */
      tde_2 = tde->next;
      removefileset = 0;
      code = volc_Call(VC_VolumeStatus, (struct sockaddr *)&(tde->server), NULL, 
		       tde->partition, &(tde->vid), &status);
      if (code) {
	com_err(whoami, code, "; Unable to get fileset status for '%s' : not dumping", tde->name);
	removefileset = 1;
      }

      if (!removefileset)
	if (status.vss.states & DUMP_VOL_NOTOK) {
	  printf("Fileset '%s' in inconsistent state : not dumping\n", tde->name); 
	  removefileset = 1;
	}

      if (removefileset) {
	/* remove fileset from list */
	if (tde_1 == NULL)
	  dumpTaskPtr->volumes = tde->next;
	else {
	  tde_1->next  = tde->next;
	}
	(void) osi_Free(tde->name, BU_MAXNAMELEN + 1 );
	(void) osi_Free(tde, sizeof (struct bc_volumeDump));
      } else {
	tde_1 = tde;
	count++;
      }
    }

    volArray = (struct tc_dumpArray *)osi_Alloc(sizeof(struct tc_dumpArray)+((count-1)*sizeof(struct tc_dumpDesc)));
    if (volArray == (struct tc_dumpArray *)NULL)
      ERROR(BC_NOMEM);
    bzero((char *)volArray, 
	  sizeof(struct tc_dumpArray)+((count-1)*sizeof(struct tc_dumpDesc)));
    volArray->tc_dumpArray_len=count;
    /* build array of volumes to be sent to backup system */
    for( code=0,tde=dumpTaskPtr->volumes; tde; tde=tde->next, code++ ) {
      /* pass two over this thing */
      volArray->tc_dumpArray_val[code].vid = tde->vid;
      volArray->tc_dumpArray_val[code].partition = tde->partition;
      strcpy((char *)volArray->tc_dumpArray_val[code].name, tde->name);
      /* pass just the internet address to butc */
      volArray->tc_dumpArray_val[code].hostAddr = tde->server;
      volArray->tc_dumpArray_val[code].date = tde->date;
      volArray->tc_dumpArray_val[code].cloneDate = tde->cloneDate;
    }
    code = 0;
    baseNamePtr = tailCompPtr(dumpTaskPtr->dumpName);
    /* setup the interface structure */
    bzero((char *)tcdiPtr, sizeof(*tcdiPtr));

    /* general */
    strcpy((char *)tcdiPtr->dumpPath, dumpTaskPtr->dumpName);
    strcpy((char *)tcdiPtr->volumeSetName, dumpTaskPtr->volSetName);

    /* tapeset */
    strcpy((char *)tcdiPtr->tapeSet.format, dumpTaskPtr->volSetName);
    strcat((char *)tcdiPtr->tapeSet.format, ".");
    strcat((char *)tcdiPtr->tapeSet.format, (char *)baseNamePtr);
    strcat((char *)tcdiPtr->tapeSet.format, ".%d");
    tcdiPtr->tapeSet.a = 1;
    tcdiPtr->tapeSet.b = 1;
    tcdiPtr->tapeSet.maxTapes = 1000000000;
    tcdiPtr->tapeSet.expDate = dumpTaskPtr->expDate;		/* PA */
    tcdiPtr->tapeSet.expType = dumpTaskPtr->expType;

    /* construct dump set name */
    strcpy((char *)tcdiPtr->dumpName, dumpTaskPtr->volSetName);
    strcat((char *)tcdiPtr->dumpName, ".");
    strcat((char *)tcdiPtr->dumpName, (char *)baseNamePtr);

    tcdiPtr->parentDumpId = dumpTaskPtr->parentDumpID;
    tcdiPtr->dumpLevel = dumpTaskPtr->dumpLevel;

    /* start the dump on the tape coordinator */
    code = BUTC_PerformDump(tconn, tcdiPtr,
		(tc_dumpArray *) volArray, (long *) &dumpTaskPtr->dumpID);
    if ( code )
    {
	com_err(whoami, code, "; Unable to start dump at tape coordinator %d",
		dumpTaskPtr->portOffset);
	if ( dumpTaskPtr->dumpID != 0 )
	{
	    (void)BUTC_RequestAbort(tconn, dumpTaskPtr->dumpID);
	    (void)BUTC_EndStatus(tconn, dumpTaskPtr->dumpID);
	}
	goto error_exit;
    }

    com_err(whoami,0,"Dump ID of dump %s : %u", tcdiPtr->dumpName,
	    dumpTaskPtr->dumpID);

    lock_ObtainWrite(&statusQueueLock);
    TRY {
      statusPtr->taskId = dumpTaskPtr->dumpID;
      statusPtr->port = dumpTaskPtr->portOffset;
      statusPtr->jobNumber = bc_jobNumber();
      statusPtr->volsTotal = volArray->tc_dumpArray_len;
      statusPtr->opType = TCOP_DUMP;
      sprintf(statusPtr->taskName, "Dump (%s.%s)", dumpTaskPtr->volSetName, 
	      baseNamePtr);
    } FINALLY {
      lock_ReleaseWrite(&statusQueueLock);
    } ENDTRY
    clearStatus(dumpTaskPtr->dumpID, STARTING);
    lock_ObtainWrite(&statusQueueLock);
    TRY {
      deleteStatusNode(statusPtr);		/* drop the refCount */
    } FINALLY {
      lock_ReleaseWrite(&statusQueueLock);
    } ENDTRY

error_exit:
    bc_FreeConn(dumpTaskPtr->config, tconn);
    /* now free up resources used */
    dumpTaskPtr->dumpID = 0;
    /* dump task related structures */
    if (dumpTaskPtr->dumpName) 
      osi_Free(dumpTaskPtr->dumpName, strlen(dumpTaskPtr->dumpName)+1);
    if (dumpTaskPtr->volSetName) 
      osi_Free(dumpTaskPtr->volSetName, strlen(dumpTaskPtr->volSetName)+1);
    /* free the list of volumes */
    freeDumpTaskVolumeList(dumpTaskPtr->volumes);
    /* shouldn't volArray be freed too? */
    if (volArray)
      osi_Free(volArray,
	   sizeof(struct tc_dumpArray)+((count-1)*sizeof(struct tc_dumpDesc)));
    dumpTaskPtr->flags &= ~BC_DI_INUSE;
    /* locally allocated resources */
    if (code) {
	dumpTaskPtr->flags |= BC_DI_ERROR;
	zapStatusNode(statusPtr);
    }
    return (code) ;
}

/* freeDumpTaskVolumeList
 *	free the list of volumes used for dumps
 */

freeDumpTaskVolumeList(vdptr)
     struct bc_volumeDump *vdptr;
{
     struct bc_volumeDump *nextVdPtr;

     while (vdptr != 0)
     {
	 nextVdPtr = vdptr->next;

	 if ( vdptr->name )
	   osi_Free(vdptr->name, BU_MAXNAMELEN+1);
	 osi_Free(vdptr, sizeof(struct bc_volumeDump));

	 vdptr = nextVdPtr;
     }
}

/*
 * bc_InitDumpSlots
 *	Just zero out for now
 */
bc_InitDumpSlots()
{
    bzero((char *)&(bc_dumpTasks[0]), 
	  BC_MAXSIMDUMPS*sizeof(struct bc_dumpTask));
}

/* bc_LabelTape
 *	opens a connection to the tape coordinator and requests that it
 *	label a tape
 */
bc_LabelTape(name,size,config,port)
char *name;
struct bc_config * volatile config;
long port;
long size;
{
    volatile rpc_binding_handle_t tconn;
    long st = 0;
    long code = 0;
    int len;
    tc_tapeLabel label;
    statusP statusPtr;
    u_long taskId;
    extern long bc_jobNumber();

    /* 
     * create status monitor block first, otherwise we may not wait for this
     * in non-interactive bak
     */
    statusPtr = createStatusNode(BC_REFCOUNT);
    bzero((char *)&label,sizeof(label));
    tconn= bc_GetConn(config,port);
    if(tconn == (rpc_binding_handle_t)NULL)
    {
        code = BC_NOCONN;
	com_err(whoami, code,
		"; Unable to connect to tape coordinator at tcid %d",
		port);
	zapStatusNode(statusPtr);
	return code;
    }

    code = CheckTCVersion(tconn);
    if ( code )
    {
	com_err(whoami, code, 
		"; Unable to contact tape coordinator at tcid %d",
		port);
	bc_FreeConn(config, tconn);
	zapStatusNode(statusPtr);
	return code;
    }

    len = strlen(name) + 1;
    if (len <= TC_MAXTAPELEN) {
      label.nameLen = len;
      strcpy((char *)label.name, name);
    } else {
      label.nameLen = TC_MAXTAPELEN - 1;
      strncpy((char *)label.name, name, TC_MAXTAPELEN - 1);
      label.name[TC_MAXTAPELEN-1] = '\0';
    };

    label.size = size;
    code = BUTC_LabelTape(tconn,&label, &taskId);
    if(code)
    {
	com_err(whoami,code,"; Unable to start labeltape at tcid %d",
		port);
	bc_FreeConn(config, tconn);
	zapStatusNode(statusPtr);
	return code;
    }

    lock_ObtainWrite(&statusQueueLock);
    statusPtr->taskId = taskId;
    statusPtr->port = port;
    statusPtr->jobNumber =  bc_jobNumber();
    statusPtr->opType = TCOP_LABELTAPE;
    sprintf(statusPtr->taskName, "Labeltape (");

    if (strcmp(name, "")) 
      strncat(statusPtr->taskName, name, 40);
    else
      strcat(statusPtr->taskName, "<NULL>");

    strcat(statusPtr->taskName, ")");

    strncpy(statusPtr->tapeName, name, BU_MAXTAPELEN);

    if ((int)(strlen(name) + 1) > BU_MAXTAPELEN)
      statusPtr->tapeName[BU_MAXTAPELEN] = '\0';

    statusPtr->tapeSize = size;
    lock_ReleaseWrite(&statusQueueLock);
    TRY {
      /* In the new scheme of things, labeltape would report termination
	 and some details about what it labelled the tape as */
      clearStatus(taskId, STARTING);		/* ok to examine */
      lock_ObtainWrite(&statusQueueLock);
      dfprintf(("bc_LabelTape: ref count before deleting is %d\n", 
		statusPtr->refCount));
      deleteStatusNode(statusPtr);		/* drop the refCount */
      dfprintf(("bc_LabelTape: ref count after deleting is %d\n", 
		statusPtr->refCount));
    } FINALLY {
      lock_ReleaseWrite(&statusQueueLock);
    } ENDTRY
    bc_FreeConn(config, tconn);
    return 0;
}

/* bc_ReadLabel
 *	open a connection to the tape coordinator and read the label on
 *	a tape
 */

bc_ReadLabel(config,port)
struct bc_config * volatile config;
long port;
{
    volatile rpc_binding_handle_t tconn;
    volatile unsigned long taskId;
    struct timespec delayTime;
    volatile statusP statusPtr;
    long code = 0;
    extern long bc_jobNumber();

    /* create status monitor block */
    statusPtr = createStatusNode(BC_REFCOUNT);
    tconn= bc_GetConn(config,port);
    if(tconn == (rpc_binding_handle_t)NULL)
    {
        code = BC_NOCONN;
	com_err(whoami, code,
		"; Unable to connect to tape coordinator at tcid %d",
		port);
	zapStatusNode(statusPtr);
	return code;
    }

    code = CheckTCVersion(tconn);
    if ( code )
    {
	com_err(whoami, code, 
		"; Unable to contact tape coordinator at tcid %d",
		port);
	bc_FreeConn(config, tconn);
	zapStatusNode(statusPtr);
	return code;
    }
    
    code = BUTC_ReadLabel(tconn, (unsigned long *)&taskId);
    if(code){
	com_err(whoami,code,"; Unable to read tape label at tcid %d", port);
	bc_FreeConn(config, tconn);
	zapStatusNode(statusPtr);
	return code;
    }
    lock_ObtainWrite(&statusQueueLock);
    TRY {
      statusPtr->taskId = taskId;
      statusPtr->port = port;
      statusPtr->jobNumber =  bc_jobNumber();
      statusPtr->opType = TCOP_READLABEL;
      strcpy(statusPtr->taskName, "ReadLabel");
    } FINALLY {
      lock_ReleaseWrite(&statusQueueLock);
    } ENDTRY
    clearStatus(taskId, STARTING);		/* ok to examine */
    
    /* In the new scheme of things, this job is asynchronous, and the status
     * watcher will report termination, and the label of the tape. We could
     * call a function in the status watcher to tell us what the label of the
     * tape is when the reading is done, and report it to the user ourselves 
     */
    while (1) {
      lock_ObtainRead(&statusQueueLock);
      if ((statusPtr->flags & (TASK_DONE|ABORT_DONE)) || statusPtr->defunct) {
	if (statusPtr->flags & TASK_DONE)
	  dfprintf(("bc_ReadLabel: task %s:%d done\n", 
		    statusPtr->taskName, statusPtr->taskId));
	else
	  dfprintf(("bc_ReadLabel: task %s:%d defunct\n", 
		    statusPtr->taskName, statusPtr->taskId));
	break;
      }
      lock_ReleaseRead(&statusQueueLock);
      delayTime.tv_sec = 2;
      delayTime.tv_nsec = 0;
      pthread_delay_np(&delayTime);
    }
    if (statusPtr->defunct || (statusPtr->flags & (TASK_ERROR|ABORT_DONE)))
      dce_svc_printf(BAK_S_UNABLE_TO_RED_LABEL_MSG);
    else if(strcmp((char *)statusPtr->tapeName, "") == 0)
      dce_svc_printf(BAK_S_TAPE_READ_WAS_MSG, statusPtr->tapeSize);
    else
      dce_svc_printf(BAK_S_TAPE_READ_WAS_LABELLED_MSG, statusPtr->tapeName, statusPtr->tapeSize);
    lock_ReleaseRead(&statusQueueLock);
    lock_ObtainWrite(&statusQueueLock);
    TRY {
      dfprintf(("bc_ReadLabel: ref count before deleting is %d\n", 
		statusPtr->refCount));
      deleteStatusNode(statusPtr);
    } FINALLY {
      lock_ReleaseWrite(&statusQueueLock);    
    } ENDTRY
    bc_FreeConn(config, tconn);
    return 0;
}

bc_ScanDumps(config, dbAddFlag, port)
    struct bc_config * volatile config;
    long dbAddFlag;
    long port;
{
    volatile rpc_binding_handle_t tconn;
    volatile statusP statusPtr;
    volatile u_long taskId;
    volatile long code = 0;
    extern long bc_jobNumber();

    /* create status monitor block */
    statusPtr = createStatusNode(BC_REFCOUNT);
    tconn = bc_GetConn(config, port);
    if ( tconn == (rpc_binding_handle_t)NULL )
    {
        code = BC_NOCONN;
	com_err(whoami, code,
		"; Unable to connect to tape coordinator at tcid %d",
		port);
	zapStatusNode(statusPtr);
        return code;
    }

    code = CheckTCVersion(tconn);
    if ( code )
    {
	com_err(whoami, code,
		"; Unable to contact tape coordinator at tcid %d",
		port);
	bc_FreeConn(config, tconn);
	zapStatusNode(statusPtr);
	return(code);
    }
    
    code = BUTC_ScanDumps(tconn, dbAddFlag, (u_long *)&taskId);
    if ( code )
    {
	com_err(whoami, code, "; Failed to start scantape at tcid %d", port);
	bc_FreeConn(config, tconn);
	zapStatusNode(statusPtr);
	return(code);
    }

    lock_ObtainWrite(&statusQueueLock);
    TRY {
      statusPtr->taskId = taskId;
      statusPtr->port = port;
      statusPtr->jobNumber =  bc_jobNumber();
      statusPtr->opType = TCOP_SCANTAPE;
      sprintf(statusPtr->taskName, "Scantape");
    } FINALLY {
      lock_ReleaseWrite(&statusQueueLock);
    } ENDTRY
    clearStatus(taskId, STARTING);		/* ok to examine */
    lock_ObtainWrite(&statusQueueLock);
    TRY {
      deleteStatusNode(statusPtr);		/* drop the refCount */
    } FINALLY {
      lock_ReleaseWrite(&statusQueueLock);
    } ENDTRY
    bc_FreeConn(config, tconn);
    return(code);
}
