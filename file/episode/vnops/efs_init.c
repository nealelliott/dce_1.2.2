/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: efs_init.c,v $
 * Revision 1.1.22.1  1996/10/02  17:44:01  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:35:50  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1996, 1990 Transarc Corporation
 * All rights reserved.
 */

/*			Episode File System
			Syscall Interface
 */

#include <dcedfs/param.h>
#include <dcedfs/osi_device.h>
/* XXX FIX ME XXX */
#undef	PRIVATE
#undef	SHARED

#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_ufs.h>
#include <dcedfs/osi_cred.h>
#include <dcedfs/queue.h>
#include <dcedfs/dir_buffer.h>
#include <efs_opcode.h>
#include <dcedfs/xcred.h>		/* xcred_Init */
#include <dcedfs/tkc.h>			/* tkc_Init */
#include <dcedfs/volreg.h>		/* volreg_Init */
#include <dcedfs/com_locks.h>		/* vnl_init */
#include <dcedfs/episode/aggregate.h>	/* epig_ functions */
#include <dcedfs/episode/anode.h>	/* epia_ functions */
#include <dcedfs/dir_buffer.h>		/* dirb_Init */
#include <efs_evnode.h>			/* struct evnode & ICL dcls */
#include <efs_misc.h>			/* vnm_Init */
#include <efs_volio.h>
#include <dcedfs/xvfs_export.h>		/* struct xvfs_vnodeops */
#include <dcedfs/episode/instrument.h>	/* monitoring and tuning stuff */
#if defined(AFS_OSF_ENV) && defined(_KERNEL)
#include <sys/sysconfig.h>
#endif /* AFS_OSF_ENV */

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/vnops/efs_init.c,v 1.1.22.1 1996/10/02 17:44:01 damon Exp $")

extern long efs_debug(long, long, long, long);

extern struct xvfs_xops efs_xops;

static struct efs_params efsParams;

extern long efs_allowSuserDirLinkOps;	/* root can't link/unlink dirs */

extern long afsdb_episode_vnops;

#define DEBUG_THIS_FILE EFS_DEBUG_INIT

static int NullProc()
{
    return 0;
}

/*
 * instrumentation related routines.
 */
/*
 * the list of monitor variables in the vnops package. These are supported by
 * ins_vnops.
 */
/* This structure should also include a R/O bit! -ota 930914 */
static struct ins_varTableEntry varTable[] = {
    {(long *)&EFS_ELBB_PARAMS(efsParams).byteCount,			/*  0 */
	 sizeof EFS_ELBB_PARAMS(efsParams).byteCount},
    {(long *)&EFS_EPIG_PARAMS(efsParams).maxHandles,			/*  1 */
	 sizeof EFS_EPIG_PARAMS(efsParams).maxHandles},
    {(long *)&EFS_DIRB_PARAMS(efsParams).abuffers,			/*  2 */
	 sizeof EFS_DIRB_PARAMS(efsParams).abuffers},
    {(long *)&efsParams.vnodePoolSize,sizeof efsParams.vnodePoolSize}, /*  3 */
    {(long *)&efsParams.flockPoolSize,sizeof efsParams.flockPoolSize}, /*  4 */
    {(long *)&efsParams.standalone,sizeof efsParams.standalone},	/*  5 */
    {(long *)&efs_allowSuserDirLinkOps, sizeof(long)},			/*  6 */
};

static long varTableSize = sizeof(varTable)/sizeof(struct ins_varTableEntry);


/*
 * The procedures below are declared and defined in the individual packages
 * responsible for providing the information.
 */
extern int ins_anode_anode();
extern int ins_anode_file();
extern int ins_anode_volume();
extern int ins_anode_aggregate();
extern int ins_log();
extern int ins_log_tune();
extern int ins_dirbuf();
extern int ins_async();
extern int ins_trace();
/*
extern int ins_acl();
 */
int ins_acl()
{
    /* dummy */
    return 0;
}

int ins_vnops();

static int (*procTable[])() = {	/* module_submodule */
    ins_anode_anode,		/* index 0 */
    ins_anode_file,		/* index 1 */
    ins_anode_volume,		/* index 2 */
    ins_anode_aggregate,	/* index 3 */
    NullProc,		/* index 4 */
    NullProc,		/* index 5 */
    NullProc,		/* index 6 */
    NullProc,		/* index 7 */
    ins_log,		/* index 8 */
    ins_elbb,		/* index 9 */
    ins_vnops,		/* index 10 */
    ins_dirbuf,		/* index 11 */
    ins_async,		/* index 12 */
    ins_acl,		/* index 13 */
    ins_log_tune,	/* index 14 */
    ins_elbb_tune,	/* index 15 */
    ins_trace		/* index 16 */
    };

static long procArraySize = (sizeof(procTable)/sizeof(long));


/* Forward definitions */
int epii_Init(struct efs_params *params);

/*
 * Main fs_syscall kernel entry point
 */

#ifdef KERNEL

/* EXPORT */
int afscall_episode(
    long code,
    long parm1, long parm2, long parm3, long parm4,
    int *retvalP)
{
    struct episode_args episode_args;
    struct ins_varDesc vars;
    struct ins_opDesc opcode;
    struct ins_sysOp sysop;
    struct vnode *bdevp;
    dev_t dev;
    struct async_device *adevp;
    epi_anode_t aggh;
    int rc, rc2;

    osi_MakePreemptionRight();

    /* Must be super-user */
    if (!osi_suser(osi_getucred())) {
	rc = EPERM;
	goto bad;
    }

    *retvalP = 0;

    rc = 0;
    switch (code) {

	case EPIOP_INIT:		/* Initialize Episode */
	    rc = osi_copyin_r((caddr_t) parm1, (caddr_t) &episode_args,
			 sizeof (episode_args));
	    if (rc)
		goto bad;

	    EFS_ELBB_PARAMS(efsParams).byteCount =
		episode_args.buffer_pool_size;
	    EFS_EPIG_PARAMS(efsParams).maxHandles =
		episode_args.anode_pool_size;
	    EFS_DIRB_PARAMS(efsParams).abuffers =
		episode_args.dir_buffer_pool_size;
	    efsParams.vnodePoolSize = episode_args.vnode_pool_size;
	    efsParams.flockPoolSize = episode_args.flock_pool_size;
	    efsParams.standalone = episode_args.standalone;

	    rc = epii_Init (&efsParams);

	    break;

	case EPIOP_STARTBIODAEMON:	/* Start buffer I/O daemon(s) */
#ifdef	AFS_AIX31_VM
	    efs_PageInDaemon ();
	    efs_PageOutDaemon();
	    efs_PageUnprotectDaemon();
#endif
	    break;

	case EPIOP_STARTASYNCDAEMON:	/* Start I/O async daemon */
#if	defined(AFS_AIX31_VM)
	    epia_StartAsyncDaemon();
#endif
	    break;

	case EPIOP_GROWAGGR:

	    /* Grow an aggregate to the size specified by parm2 (which is the
             * number of 1K blocks desired). */

	    osi_UnmakePreemptionRight();

	    rc = osi_getvdev ((caddr_t)parm1, &dev, &bdevp);
	    if (rc) return rc;

	    OSI_OP_OPEN (bdevp, FREAD | FWRITE, osi_getucred(), rc);
	    if (rc) {
		goto closed;
	    }

	    osi_MakePreemptionRight();
	    rc = epig_InitAggregate(osi_major(dev), osi_minor(dev), 1,
					&adevp, &aggh);
	    if (rc) {
		EFS_TRACE_VNOPS(DEBUG_THIS_FILE, rc, EIO);
		rc = EIO;
		goto grown;
	    }
	    rc = epig_GrowAggregate (aggh, parm2 / (adevp->blkSize / 1024),
				     0, 0);
	    if (rc) {
		EFS_TRACE_VNOPS(DEBUG_THIS_FILE, rc, EIO);
		rc = EIO;
	    }

	    rc2 = epig_CloseAggregate (aggh);
	    osi_assert(!rc2);
	    osi_UnmakePreemptionRight();

grown:
	    OSI_OP_CLOSE (bdevp, FREAD | FWRITE, osi_getucred());

closed:
	    OSI_VN_RELE (bdevp);

	    return rc;

	case EPIOP_NEWAGGR:		/* Create a new aggregate */

	case EPIOP_NEWVOL:		/* Create a new volume */

	case EPIOP_STATS: {		/* Get performance statistics or tune the system*/
	    struct ins_basicObject *resultValuep;
	    struct ins_basicObjectId *argObjIdp, *resultObjIdp;
	    char *buf, *bp;

	    rc = osi_copyin_r((caddr_t)parm1, (caddr_t)&sysop,
			 sizeof (struct ins_sysOp));
	    if (rc) goto bad;

	    vars = sysop.var;		/* structure assignment */
	    opcode = sysop.op;		/* structure assignment */
	    if (opcode.count > 1) {
		/* array operation not supported yet */
		rc = EINVAL;
		goto bad;
	    }

	    bp = buf = (char *)osi_AllocBufferSpace_r();
	    resultValuep = (struct ins_basicObject *)bp;
	    bp += sizeof (*resultValuep);
	    argObjIdp = (struct ins_basicObjectId *)bp;
	    bp += sizeof (*argObjIdp);
	    resultObjIdp = (struct ins_basicObjectId *)bp;
	    bp += sizeof (*resultObjIdp);

	    rc = osi_copyin_r((caddr_t)parm2, (caddr_t)argObjIdp,
			sizeof (struct ins_basicObjectId));
	    if (rc) goto dealloc;

	    rc = osi_copyin_r((caddr_t)parm3, (caddr_t)resultObjIdp,
			sizeof (struct ins_basicObjectId));
	    if (rc) goto dealloc;

	    rc = osi_copyin_r((caddr_t)parm4, (caddr_t)resultValuep,
			sizeof (struct ins_basicObject));
	    if (rc) goto dealloc;

	    /* all the parameters have been copied in */
	    rc = ins_SelectProc(&opcode,&vars,argObjIdp,resultObjIdp,
		resultValuep);
	    if (rc) goto dealloc;

	    if(opcode.opcode != INS_SET) {
		/* it is GET or GETNEXT operation, therefore result of
		 * lookup is required
		 */
		rc = osi_copyout_r((caddr_t)resultValuep, (caddr_t)parm4,
				sizeof (struct ins_basicObject));
		if (rc) goto dealloc;
	    }
	    rc = osi_copyout_r((caddr_t)resultObjIdp, (caddr_t)parm3,
			 sizeof(struct ins_basicObjectId));
	    if (rc) goto dealloc;
dealloc:
	    osi_FreeBufferSpace_r((struct osi_buffer *)buf);
	    if (rc)
		goto bad;
	    }
	    break;

	  case EPIOP_DEBUG:
	    rc = efs_debug(parm1, parm2, parm3, parm4);
	    break;

	default:
	    rc = EINVAL;
    }
bad:
    osi_UnmakePreemptionRight();
    return (rc);
}

#endif /* KERNEL */

/*
 * epii_GetEPIOps -- Export the EPI ops to the vfs+ layer
 */

struct xvfs_vnodeops *xvfs_ops1;		/* EPI vfs ops */
#ifdef AFS_OSF_ENV
struct xvfs_vnodeops *xvfs_ops2;		/* EPI device vfs ops */
struct xvfs_vnodeops *xvfs_ops3;		/* EPI fifo vfs ops */
#endif /* AFS_OSF_ENV */

static void InitEPIOps(void)
{
#ifdef	AFS_OSF_ENV
    extern struct osi_vnodeops efs_specops;
    extern struct osi_vnodeops efs_fifoops;
    extern struct xvfs_xops efs_deadxops;
#endif /* AFS_OSF_ENV */

    xvfs_ops1 = osi_Alloc_r(sizeof (struct xvfs_vnodeops));
    xvfs_InitFromXOps (&efs_xops, xvfs_ops1);

#ifdef AFS_OSF_ENV
    xvfs_ops2 = osi_Alloc_r(sizeof (struct xvfs_vnodeops));
    xvfs_InitFromXOps(&efs_xops, xvfs_ops2);
    xvfs_ops2->nops = efs_specops;
    xvfs_ops2->xops = efs_deadxops;
    xvfs_ops3 = osi_Alloc_r(sizeof (struct xvfs_vnodeops));
    xvfs_InitFromXOps(&efs_xops, xvfs_ops3);
    xvfs_ops3->nops = efs_fifoops;
    xvfs_ops3->xops = efs_deadxops;
#endif /* AFS_OSF_ENV */
}

/* EXPORT */
struct xvfs_vnodeops *epii_GetEPIOps(void)
{
    afsl_Assert (xvfs_ops1 != NULL);
    return xvfs_ops1;
}


/*
 * epii_Init -- initialize Episode
 *
 * This can be used both inside and outside the kernel.
 */

int epi_Initted = 0;
extern int efs_vfsgetvolume(struct osi_vfs *, struct volume **);
extern struct xvfs_vfsops efs_vfsops;

#ifdef	KERNEL
struct icl_set *efs_iclSet;		/* Put only critical traces here */
struct icl_set *efs_iclSet2;		/* All other traces */
struct icl_set *efs_iclSetalloc;	/* All other traces */
#endif	/* KERNEL */

/* EXPORT */
int epii_Init(struct efs_params *params)
{
    int rc;				/* error return code */

    if (epi_Initted)
	return 0;
    epi_Initted = 1;
    osi_MakePreemptionRight();

    /*
     * If we didn't come in through the syscall, the parameters need to
     * be copied to `efsParams' so that epistat will work.
     */
    if (params != &efsParams)
	efsParams = *params;		/* structure assignment */

#ifdef KERNEL
    {
	struct icl_log *logp;
	icl_CreateLogWithFlags_r("disk", 0, 0, &logp);
	icl_CreateSetWithFlags_r("episode/vnopsBasic", logp, 0,
				  ICL_CRSET_FLAG_DEFAULT_ON, &efs_iclSet);
	icl_CreateSetWithFlags_r("episode/vnopsVerbose", logp, 0,
				  ICL_CRSET_FLAG_DEFAULT_OFF, &efs_iclSet2);
	icl_CreateSetWithFlags_r("episode/allocation", logp, 0,
				  ICL_CRSET_FLAG_DEFAULT_OFF,
				  &efs_iclSetalloc);
    }
#endif /* KERNEL */

    EFS_ELBB_PARAMS(*params).bufferPool =
	(caddr_t)osi_Alloc_r(EFS_ELBB_PARAMS(*params).byteCount);
    /* XXX This is dubious-looking code. */
    if (rc = dirb_Init (&params->dirbParams)) {
	if (rc == BUF_REINIT || rc == BUF_TOOSMALL) {
	    osi_Free_r(EFS_ELBB_PARAMS(*params).bufferPool,
		     EFS_ELBB_PARAMS(*params).byteCount);
	    rc = (rc == BUF_REINIT) ? 0 : EINVAL;
	} else if (rc == EPI_E_REINIT) {
	    rc = 0;
	} else if (rc == -1) {
	    panic("Internal error");
	}
	goto badinit;
    }

    if (rc = vnm_Init (params->vnodePoolSize))
	goto badinit;

    osi_UnmakePreemptionRight();
    rc = vnl_init (params->flockPoolSize);
    osi_MakePreemptionRight();
    if (rc)
	goto badinit;

    epia_InitStrategy();
    vol_efsInitIO(VOL_EFS_NBUFS);	/* XXX Parameterize */
    InitEPIOps();

    osi_UnmakePreemptionRight();
    volreg_Init();
    xcred_Init();			/* Init the xcred module */
    tkc_Init();
    if (!params->standalone)
	/* Exportable */
	xvfs_InitFromVFSOps ((struct osi_vfsops *) &efs_vfsops,
			     (struct xvfs_vfsops *) &efs_vfsops,
			     efs_vfsgetvolume);
#if defined(AFS_SUNOS5_ENV) && defined(_KERNEL)
    /*
     * Start the async I/O cleanup thread and the inactive daemon.
     * XXX We should probably do this on AIX also, instead of relying
     * on a separate utility.  XXX We must hold the Big Lock when
     * calling osi_ThreadCreate.
     */
    epia_StartAsyncDaemon();
#endif /* AFS_SUNOS5_ENV && _KERNEL */
    if (params->standalone) {
	afsUUID sysAdminGroupID;
	u_long bogusgid = (u_long) -2;
	bzero((caddr_t) &sysAdminGroupID, sizeof (epi_principal_id_t));
	*(u_long *)&sysAdminGroupID = bogusgid;
	dacl_SetSysAdminGroupID(&sysAdminGroupID);
    }
    eacl_Init();

    return (0);

badinit:
    osi_UnmakePreemptionRight();
    epi_Initted = 0;
    return (rc);
}

/*
 * check the validity of the parameters, select the right procedure
 * to call on the basis of vars->procIdx.
 */
/* EXPORT */
int ins_SelectProc(opcode,vars,argObjIds,resultObjIds,resultValues)
  IN struct ins_opDesc *opcode;
  IN struct ins_varDesc *vars;
  IN struct ins_basicObjectId *argObjIds;
  OUT struct ins_basicObjectId *resultObjIds;
  OUT struct ins_basicObject *resultValues;
{
    if (opcode->count > 1) {
	/* not yet implemented */
	return EINVAL;
    }
    if (!(opcode->opcode == INS_SET || opcode->opcode == INS_GET || opcode->opcode == INS_GETNEXT)) {
	/* illegal operation */
	return EINVAL;
    }
    if (vars->procIdx > (procArraySize - 1)) {
	/* illegal procedure index */
	return EINVAL;
    }
    return((*procTable[vars->procIdx])(opcode,vars,argObjIds,resultObjIds,resultValues));

}


/* EXPORT */
int ins_GenericTransfer(aTable,aSize,opcode,vars,argObjIds,resultObjIds,resultValues)
IN struct ins_varTableEntry *aTable;
IN long aSize;
IN struct ins_opDesc *opcode;
IN struct ins_varDesc *vars;
IN struct ins_basicObjectId *argObjIds;
OUT struct ins_basicObjectId *resultObjIds;
OUT struct ins_basicObject *resultValues;
{

    if (vars->varIdx >= 0 && vars->varIdx < aSize) {
	struct ins_varTableEntry *var = &aTable[vars->varIdx];

	/* the variable index is within range */

	if (opcode->opcode == INS_GET) {
	    if (var->varAddr == NULL)
		bzero (resultValues->value, var->varSize);
	    else bcopy ((caddr_t) var->varAddr,
			(caddr_t) resultValues->value,
			var->varSize);
	}
	else if (opcode->opcode == INS_SET) {
	    if (var->varAddr != NULL)
		bcopy ((caddr_t) resultValues->value,
			(caddr_t) var->varAddr,
			var->varSize);
	}
	else if (opcode->opcode == INS_GETNEXT) {
	    /* not yet supported */
	}
	return 0;
    }
    else {
	return EINVAL;
    }
}


/*
 * get or set the value of variable indicated by <vars>->varIdx. The
 * result of lookup is stored in <resultValues>. Object Id parameters
 * are ignored for now. --This proc should be generalized for common
 * use by all pacakages.
 */
/* EXPORT */
int ins_vnops(opcode,vars,argObjIds,resultObjIds,resultValues)
IN struct ins_opDesc *opcode;
IN struct ins_varDesc *vars;
IN struct ins_basicObjectId *argObjIds;
OUT struct ins_basicObjectId *resultObjIds;
OUT struct ins_basicObject *resultValues;
{
    if ((opcode->opcode == INS_SET) && (vars->varIdx != 6)) {
	/* not writeable */
	return EINVAL;
    }
    return(ins_GenericTransfer(varTable,varTableSize,opcode,vars,argObjIds,resultObjIds,resultValues));
}
