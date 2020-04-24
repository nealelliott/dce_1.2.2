/*
 * Copyright (c) 1996, 1994, Transarc Corp.
 */

#include "tkm_systest.h"

RCSID("$Header: /afs/tr/project/fs/dev/dfs-dev/src/file/tkm/RCS/tkm_systest_parconf.c,v 1.4 1996/03/04 19:13:20 ota Exp $")

/****************************************************
  The following code is needed to generate simultaneous conflicting
  token requests.
  *****************************************************/



MakeOverlappingRanges( afs_hyper_t *beginRange1, afs_hyper_t *endRange1,
		      afs_hyper_t *beginRange2, afs_hyper_t *endRange2)
{
    int sl = lrand48() % 3;

    /*
     * We want to test the following possibilities :
     * 		Ranges are such that we get : 0, 1, or 2 slice & dice tokens
     */

    switch (sl) {
      case 0:

	AFS_hzero(*beginRange1);
	AFS_hset64(*endRange1, 0, 10);
	*beginRange2 = *beginRange1;
	*endRange2 = *endRange1;
	break;
      case 1:
	AFS_hzero(*beginRange1);
	AFS_hset64(*endRange1, 0, 10);
	AFS_hset64(*beginRange2, 0, 5);
	*endRange2 = *endRange1;
	break;
      case 2:
	AFS_hzero(*beginRange1);
	AFS_hset64(*endRange1, 0, 10);
	AFS_hset64(*beginRange2, 0, 5);
	AFS_hset64(*endRange2, 0, 9);
	break;
      default:
	osi_assert(0); /* can't happen */
    }
}

#define ALLCONFLICTS 18 /*
			 * this will need to change if the conflict array
			 * in ctable.c changes size
			 */

extern long conflict[ALLCONFLICTS][2];

MakeConflictingTypes(long *type1, long *type2)
{
    int cnum = lrand48() % ALLCONFLICTS;
    int order = osi_Time() % 2;

    *type1 = TKM_TOKENTYPE_MASK(conflict[cnum][order]);
    *type2 = TKM_TOKENTYPE_MASK(conflict[cnum][!order]);
}

static void PardoGetToken (void *argP,  void *ignoredTpqOpHandle)
{
    long type = ((struct pardoArg *) argP)->types;
    afs_hyper_t *begin = ((struct pardoArg *) argP)->beginRange;
    afs_hyper_t *end = ((struct pardoArg *) argP)->endRange;
    afsFid *fidP = ((struct pardoArg *) argP)->fidP;
    int h = ((struct pardoArg *) argP)->hostId;

    GetToken(fidP, h, 0, 0, type, begin, end, NULL, 0, 0, NULL);
}


/* thread that makes 2 parallel gettoken requests for conflicting tokens */
ParallelConflict()
{
    long t1, t2;
    afs_hyper_t b1, b2, e1, e2;
    char spare[1024];
    tpq_pardo_clientop_t clientOpsP[2];
    struct pardoArg argsP[2];
    struct systest_file *fileP ;

    bzero(spare,1024);
    bzero( (caddr_t) clientOpsP, 2 * sizeof(tpq_pardo_clientop_t));
    clientOpsP[0].op = clientOpsP[1].op = PardoGetToken;
    clientOpsP[0].argP = &(argsP[0]);
    clientOpsP[1].argP = &(argsP[1]);

    fileP = GetRandomFile();
    MakeConflictingTypes(&t1, &t2);
    MakeOverlappingRanges(&b1, &e1, &b2,&e2);

    argsP[0].fidP = argsP[1].fidP = fileP->fid;
    argsP[0].types = t1;
    argsP[1].types = t2;
    argsP[0].beginRange = &b1;
    argsP[0].endRange = &e1;
    argsP[1].beginRange = &b2;
    argsP[1].endRange = &e2;
    argsP[0].hostId = 0;
    argsP[1].hostId = 1;
    tpq_Pardo(systest_threadPoolHandle, clientOpsP, 2, TPQ_HIGHPRI, 0);
    if (spare[0] != 0)
	printf("YIIIIIIIIIKES\n");
}

GetTwoConflicting()
{
    afsFid fid, volFid;
    afs_hyper_t begin;
    afs_hyper_t end;
    afsRevokeDesc desc;
    struct systest_file * fileP;
    struct systest_file *vol;
    struct systest_token *revoked;
    long code;

    FidCopy((&volFid), (&fid));
    volFid.Vnode = TKM_AFSFID_WHOLE_VOL_VNODE;
    volFid.Unique = TKM_AFSFID_WHOLE_VOL_UNIQUE;
    AddFile(&fid);
    AddFile(&volFid);
    AFS_hset64(begin, 0, 1);
    AFS_hset64(end, 0, 2);
    if (GetToken(&fid, 1, 1, 0, 2, &begin, &end, NULL, 0, 0, NULL))
	return(1);
    fileP = FindFile(&fid);
    osi_assert(fileP!= NULL);
    tkm_dprintf(("got %x id %u,,%u\n", fileP->granted,
		 AFS_HGETBOTH(fileP->granted->info->tokenID)));
    if (GetToken(&fid, 2, 1, 0, 2, &begin, &end, NULL, 0, 0, NULL))
	return(1);
    fileP = FindFile(&fid);
    osi_assert(fileP!= NULL);
    tkm_dprintf(("got %x id %u,,%u\n", fileP->granted,
		 AFS_HGETBOTH(fileP->granted->info->tokenID)));
    return(0);
}

ParallelConflictLoop()
{
    systest_threadPoolHandle = tpq_Init(2,2,2,30, "systest_parconf");
    while (1) {
	ParallelConflict();
	osi_Pause(15);
    }
}
