/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: struct_ops.c,v $
 * Revision 1.1.75.1  1996/10/02  17:03:04  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:32  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1996, 1991 Transarc Corporation
 * All Rights Reserved
 */
/*
 * struct_ops.c -- structure printing utilities
 */ 
#include <dcedfs/stds.h>  /* include appropriate standards file */

#include <sys/types.h>
#include <dcedfs/stds.h>
#include <sys/file.h>
#include <dcedfs/param.h>
#include <dcedfs/compat.h>
#include <stdio.h>
#include <dce/rpc.h>
#include <netinet/in.h>
#include <string.h>
#include "budb_errs.h"
#include "database.h"

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/bakserver/struct_ops.c,v 1.1.75.1 1996/10/02 17:03:04 damon Exp $")

printDbHeader(ptr)
     struct DbHeader *ptr;
{
    printf("version = %d\n", ptr->dbversion);
    printf("created = %s", ctime(&ptr->created));
    printf("cell = %s\n",ptr->cell);
    printf("lastDumpId = %d\n", ptr->lastDumpId);
    printf("lastInstanceId = %d\n", ptr->lastInstanceId);
    printf("lastTapeId = %d\n", ptr->lastTapeId);
}

printDump(fid, dptr)
     FILE *fid;
     struct dump *dptr;
{
    fprintf(fid,"id = %d\n", dptr->id);
    fprintf(fid,"idHashChain = %d\n", dptr->idHashChain);
    fprintf(fid,"name = %s\n", dptr->dumpName);
    fprintf(fid,"vsname = %s\n", dptr->volumeSet);
    fprintf(fid,"dumpPath = %s\n", dptr->dumpPath);
    fprintf(fid,"nameHashChain = %d\n", dptr->nameHashChain);
    fprintf(fid,"flags = 0x%x\n", dptr->flags);
    fprintf(fid,"parent = %d\n", dptr->parent);
    fprintf(fid,"created = %s", ctime( (long *) &dptr->created));
/*  fprintf(fid,"incTime = %s", ctime( (long *) &dptr->incTime)); */
    fprintf(fid,"nVolumes = %d\n", dptr->nVolumes);
    /* printTapeSet(&dptr->tapes); */
    fprintf(fid,"firstTape = %d\n", dptr->firstTape);
    /* printKtcPrincipal(&dptr->dumper); */

}

printDumpEntry(deptr)
     budb_dumpEntry *deptr;
{
    printf("id = %d\n", deptr->id);
    printf("parent = %d\n", deptr->parent);
    printf("level = %d\n", deptr->level);
    printf("flags = 0x%x\n", deptr->flags);
    printf("Fileset family = %s\n", deptr->volumeSetName);
    printf("dump path = %s\n", deptr->dumpPath);
    printf("name = %s\n", deptr->name);
    printf("created = %s", ctime((long *)&deptr->created));
/*  printf("incTime = %s", ctime(&deptr->incTime)); */
    printf("nVolumes = %d\n", deptr->nVolumes);

    printTapeSet(&deptr->tapes);
}

/* printHashTable
 *	print the hash table structure, i.e. the header structure.
 */

printHashTable(fid, htptr)
     FILE *fid;
     struct hashTable *htptr;
{
    fprintf(fid,"functionType = %d\n", htptr->functionType);
    fprintf(fid,"threadOffset = %d\n", htptr->threadOffset);
    fprintf(fid,"entries = %d\n", htptr->entries);
    fprintf(fid,"length = %d\n", htptr->length);
    fprintf(fid,"table = %d\n", htptr->table);
    fprintf(fid,"progress = %d\n", htptr->progress);
    fprintf(fid,"oldLength = %d\n", htptr->oldLength);
    fprintf(fid,"oldTable = %d\n", htptr->oldTable);
}

/* printMemoryHashTable
 *	print the hash table structure, i.e. the header structure.
 */
printMemoryHashTable(fid, mhtptr)
     FILE *fid;
     struct memoryHashTable *mhtptr;
{
    fprintf(fid,"threadOffset = %d\n", mhtptr->threadOffset);
    fprintf(fid,"length = %d\n", mhtptr->length);
    fprintf(fid,"progress = %d\n", mhtptr->progress);
    fprintf(fid,"size = %d\n", mhtptr->size);
    fprintf(fid,"oldsize = %d\n", mhtptr->oldSize);
}

#ifdef notdef
printPrincipal(ptr)
     struct ktc_principal *ptr;
{
    printf("name = %s\n", ptr->name);
    printf("instance = %s\n", ptr->instance);
    printf("cell = %s\n", ptr->cell);
}
#endif

printStructDumpHeader(ptr)
     structDumpHeader *ptr;
{
    printf("type = %d\n", ptr->type);
    printf("structure version = %d\n", ptr->structversion);
    printf("size = %d bytes\n", ptr->size);
}

printTape(fid, tptr)
     FILE *fid;
     struct tape *tptr;
{
    fprintf(fid,"name = %s\n", tptr->name);
    fprintf(fid,"nameHashChain = %d\n", tptr->nameHashChain);
    fprintf(fid,"flags = 0x%x\n", tptr->flags);
    /* printTapeId(tptr->tapeid); */
    fprintf(fid,"written = %s", ctime( (long *) &tptr->written));
    fprintf(fid,"nMBytes = %d\n", tptr->nMBytes);
    fprintf(fid,"nBytes = %d\n", tptr->nBytes);
    fprintf(fid,"nFiles = %d\n", tptr->nFiles);
    fprintf(fid,"nVolumes = %d\n", tptr->nVolumes);
    fprintf(fid,"seq = %d\n", tptr->seq);
    fprintf(fid,"dump = %d\n", tptr->dump);
    fprintf(fid,"nextTape = %d\n", tptr->nextTape);
    fprintf(fid,"firstVol = %d\n", tptr->firstVol);
    fprintf(fid,"useCount = %d\n", tptr->useCount);
}

printTapeEntry(teptr)
     budb_tapeEntry *teptr;
{
    printf("name = %s\n", teptr->name);
    printf("flags = 0x%x\n", teptr->flags);
    printf("written = %s", ctime((long *)&teptr->written));
    printf("expires = %s", ctime((long *)&teptr->expires));
    printf("nMBytes = %d\n", teptr->nMBytes);
    printf("nBytes = %d\n", teptr->nBytes);
    printf("nFiles = %d\n", teptr->nFiles);
    printf("nVolumes = %d\n", teptr->nVolumes);
    printf("seq = %d\n", teptr->seq);
    printf("tapeid = %d\n", teptr->tapeid);
    printf("useCount = %d\n", teptr->useCount);
    printf("mediaType = %d\n", teptr->mediaType);
    printf("dump = %d\n", teptr->dump);
}

printTapeSet(tsptr)
     budb_tapeSet *tsptr;
{
    printf("id  = %d\n",tsptr->id );
    printf("tapeServer = %s\n",tsptr->tapeServer);
    printf("format = %s\n",tsptr->format);
    printf("maxTapes = %d\n",tsptr->maxTapes);
    printf("a  = %d\n",tsptr->a );
    printf("b = %d\n",tsptr->b);
}

printVolumeEntry(veptr)
     budb_volumeEntry *veptr;
{
    printf("name = %s\n", veptr->name);
    printf("flags = 0x%x\n", veptr->flags);
    printf("id = %u,,%u\n", AFS_HGETBOTH(veptr->id));
    printf("server = %s\n", veptr->server);
    printf("aggregate = %d\n", veptr->partition);
    printf("nFrags = %d\n", veptr->nFrags);

    printf("position = %d\n", 
	   (veptr->position==0)?0:(veptr->position-1));
    printf("clone = %s", ctime((long *)&veptr->clone));
    /* printf("incTime = %s", ctime(&veptr->incTime)); */
    printf("startByte = %d\n", veptr->startByte);
    printf("nBytes = %d\n", veptr->nBytes);
    printf("seq = %d\n", veptr->seq);

    printf("dump = %d\n", veptr->dump);
    printf("tape = %s\n", veptr->tape);
}
  
printVolFragment(fid, vfptr)
     FILE *fid;
     struct volFragment *vfptr;
{
    fprintf(fid,"vol = %d\n", vfptr->vol);
    fprintf(fid,"sameNameChain = %d\n", vfptr->sameNameChain);
    fprintf(fid,"tape = %d\n", vfptr->tape);
    fprintf(fid,"sameTapeChain = %d\n", vfptr->sameTapeChain);
    fprintf(fid,"position = %d\n", vfptr->position);
    fprintf(fid,"clone = %s", ctime((long *) &vfptr->clone));
    fprintf(fid,"incTime = %s", ctime((long *) &vfptr->incTime));
    fprintf(fid,"startByte = %d\n", vfptr->startByte);
    fprintf(fid,"nBytes = %d\n", vfptr->nBytes);
    fprintf(fid,"flags = %d\n", vfptr->flags);
    fprintf(fid,"sequence = %d\n", vfptr->sequence);
}

printVolInfo(fid, viptr)
     FILE *fid;
     struct volInfo *viptr;
{
    afs_hyper_t temp;

    DFSH_MemFromNetHyper(temp, viptr->id);

    fprintf(fid,"name = %s\n", viptr->name);
    fprintf(fid,"nameHashChain = %d\n", viptr->nameHashChain);
    fprintf(fid,"id = %u,,%u\n", AFS_HGETBOTH(temp));
    fprintf(fid,"server = %s\n", viptr->server);
    fprintf(fid,"partition = %d\n", viptr->partition);
    fprintf(fid,"flags = 0x%x\n", viptr->flags);
    fprintf(fid,"sameNameHead = %d\n", viptr->sameNameHead);
    fprintf(fid,"sameNameChain = %d\n", viptr->sameNameChain);
    fprintf(fid,"firstFragment = %d\n", viptr->firstFragment);
    fprintf(fid,"nFrags = %d\n", viptr->nFrags);
}


/* -----------------------------------------
 * structure xdr routines
 * ----------------------------------------- 
 */

/* utilities - network to host conversion
 *	currently used for debug only
 */

volFragment_ntoh(netVfPtr, hostVfPtr)
     struct volFragment *netVfPtr, *hostVfPtr;
{
    hostVfPtr->vol = ntohl(netVfPtr->vol);
    hostVfPtr->sameNameChain = ntohl(netVfPtr->sameNameChain);
    hostVfPtr->tape = ntohl(netVfPtr->tape);
    hostVfPtr->sameTapeChain = ntohl(netVfPtr->sameTapeChain);
    hostVfPtr->position = ntohl(netVfPtr->position);
    hostVfPtr->clone = ntohl(netVfPtr->clone);
    hostVfPtr->incTime = ntohl(netVfPtr->incTime);
    hostVfPtr->startByte = ntohl(netVfPtr->startByte);
    hostVfPtr->nBytes = ntohl(netVfPtr->nBytes);
    hostVfPtr->flags = ntohs(netVfPtr->flags);
    hostVfPtr->sequence = ntohs(netVfPtr->sequence);
}

volInfo_ntoh(netViPtr, hostViPtr)
     struct volInfo *netViPtr, *hostViPtr;
{
    strcpy(hostViPtr->name, netViPtr->name);
    hostViPtr->nameHashChain = ntohl(netViPtr->nameHashChain);
    hostViPtr->id = netViPtr->id;	/* leave unconverted */
    strcpy(hostViPtr->server, netViPtr->server);
    hostViPtr->partition = ntohl(netViPtr->partition);
    hostViPtr->flags = ntohl(netViPtr->flags);
    hostViPtr->sameNameHead = ntohl(netViPtr->sameNameHead);
    hostViPtr->sameNameChain = ntohl(netViPtr->sameNameChain);
    hostViPtr->firstFragment = ntohl(netViPtr->firstFragment);
    hostViPtr->nFrags = ntohl(netViPtr->nFrags);
}

tape_ntoh(netTapePtr, hostTapePtr)
     struct tape *netTapePtr, *hostTapePtr;
{
    strcpy(hostTapePtr->name, netTapePtr->name);
    hostTapePtr->nameHashChain = ntohl(netTapePtr->nameHashChain);
    hostTapePtr->flags = ntohl(netTapePtr->flags);

    /* tape id conversion here */
    hostTapePtr->written = ntohl(netTapePtr->written);
    hostTapePtr->nMBytes = ntohl(netTapePtr->nMBytes);
    hostTapePtr->nBytes = ntohl(netTapePtr->nBytes);
    hostTapePtr->nFiles = ntohl(netTapePtr->nFiles);
    hostTapePtr->nVolumes = ntohl(netTapePtr->nVolumes);
    hostTapePtr->seq = ntohl(netTapePtr->seq);
    hostTapePtr->dump = ntohl(netTapePtr->dump);
    hostTapePtr->nextTape = ntohl(netTapePtr->nextTape);
    hostTapePtr->firstVol = ntohl(netTapePtr->firstVol);
    hostTapePtr->useCount = ntohl(netTapePtr->useCount);
}

dump_ntoh(netDumpPtr, hostDumpPtr)
     struct dump *netDumpPtr, *hostDumpPtr;
{
    hostDumpPtr->id = ntohl(netDumpPtr->id);    
    hostDumpPtr->idHashChain = ntohl(netDumpPtr->idHashChain);
    strcpy(hostDumpPtr->dumpName, netDumpPtr->dumpName);
    strcpy(hostDumpPtr->dumpPath, netDumpPtr->dumpPath);
    strcpy(hostDumpPtr->volumeSet, netDumpPtr->volumeSet);
    hostDumpPtr->nameHashChain = ntohl(netDumpPtr->nameHashChain);
    hostDumpPtr->flags = ntohl(netDumpPtr->flags);
    hostDumpPtr->parent = ntohl(netDumpPtr->parent);
    hostDumpPtr->created = ntohl(netDumpPtr->created);
    hostDumpPtr->nVolumes = ntohl(netDumpPtr->nVolumes);

    tapeSet_ntoh(&netDumpPtr->tapes, &hostDumpPtr->tapes);

    hostDumpPtr->firstTape = ntohl(netDumpPtr->firstTape);
#ifdef notdef
    principal_ntoh(&netDumpPtr->dumper, &hostDumpPtr->dumper);
#endif
}

DbHeader_ntoh(netptr, hostptr)
     struct DbHeader *netptr, *hostptr;
{
    hostptr->dbversion = ntohl(netptr->dbversion);
    hostptr->created = ntohl(netptr->created);
    strcpy((char *)hostptr->cell, (char *)netptr->cell);
    hostptr->lastDumpId = ntohl(netptr->lastDumpId);
    hostptr->lastInstanceId = ntohl(netptr->lastInstanceId);
    hostptr->lastTapeId = ntohl(netptr->lastTapeId);
}

dumpEntry_ntoh(netptr, hostptr)
     budb_dumpEntry *netptr, *hostptr;
{
    hostptr->id = ntohl(netptr->id);
    hostptr->parent = ntohl(netptr->parent);
    hostptr->level = ntohl(netptr->level);
    hostptr->flags = ntohl(netptr->flags);
    strcpy((char *)hostptr->volumeSetName, (char *)netptr->volumeSetName);
    strcpy((char *)hostptr->dumpPath, (char *)netptr->dumpPath);
    strcpy((char *)hostptr->name, (char *)netptr->name);
    hostptr->created = ntohl(netptr->created);
    hostptr->incTime = ntohl(netptr->incTime);
    hostptr->nVolumes = ntohl(netptr->nVolumes);

    tapeSet_ntoh(&netptr->tapes, &hostptr->tapes);
}

#ifdef notdef
principal_hton(hostptr, netptr)
     struct ktc_principal *hostptr, *netptr;
{
    strcpy(netptr->name, hostptr->name);
    strcpy(netptr->instance, hostptr->instance);
    strcpy(netptr->cell, hostptr->cell);
}

principal_ntoh(netptr, hostptr)
     struct ktc_principal *netptr, *hostptr;
{
    strcpy(hostptr->name, netptr->name);
    strcpy(hostptr->instance, netptr->instance);
    strcpy(hostptr->cell, netptr->cell);
}

#endif
structDumpHeader_hton(hostPtr, netPtr)
     structDumpHeader *hostPtr, *netPtr;
{
    netPtr->type = htonl(hostPtr->type);
    netPtr->structversion = htonl(hostPtr->structversion);
    netPtr->size = htonl(hostPtr->size);
}

structDumpHeader_ntoh(netPtr, hostPtr)
     structDumpHeader *hostPtr, *netPtr;
{
    hostPtr->type = ntohl(netPtr->type);
    hostPtr->structversion = ntohl(netPtr->structversion);
    hostPtr->size = ntohl(netPtr->size);
}

tapeEntry_ntoh(netptr, hostptr)
     budb_tapeEntry *netptr, *hostptr;
{
    strcpy((char *)hostptr->name, (char *)netptr->name);
    hostptr->flags = ntohl(netptr->flags);
    hostptr->written = ntohl(netptr->written);
    hostptr->expires = ntohl(netptr->expires);
    hostptr->nMBytes = ntohl(netptr->nMBytes);
    hostptr->nBytes = ntohl(netptr->nBytes);
    hostptr->nFiles = ntohl(netptr->nFiles);
    hostptr->nVolumes = ntohl(netptr->nVolumes);
    hostptr->seq = ntohl(netptr->seq);
    /* hostptr->tapeid = ntohl(netptr->tapeid); */
    hostptr->useCount = ntohl(netptr->useCount);
    hostptr->mediaType = ntohl(netptr->mediaType);
    hostptr->dump = ntohl(netptr->dump);
}

tapeSet_hton(hostptr, netptr)
     budb_tapeSet *hostptr, *netptr;
{
    netptr->id = htonl(hostptr->id);
    strcpy((char *)netptr->tapeServer, (char *)hostptr->tapeServer);    
    strcpy((char *)netptr->format, (char *)hostptr->format);    
    netptr->maxTapes = htonl(hostptr->maxTapes);
    netptr->a = htonl(hostptr->a);
    netptr->b = htonl(hostptr->b);
}

tapeSet_ntoh(netptr, hostptr)
     budb_tapeSet *netptr, *hostptr;
{
    hostptr->id = ntohl(netptr->id);
    strcpy((char *)hostptr->tapeServer, (char *)netptr->tapeServer);    
    strcpy((char *)hostptr->format, (char *)netptr->format);    
    hostptr->maxTapes = ntohl(netptr->maxTapes);
    hostptr->a = ntohl(netptr->a);
    hostptr->b = ntohl(netptr->b);
}

textBlock_hton(hostptr, netptr)
     struct textBlock *hostptr, *netptr;
{
    netptr->version = htonl(hostptr->version);
    netptr->size = htonl(hostptr->size);
    netptr->textAddr = htonl(hostptr->textAddr);
    netptr->newsize = htonl(hostptr->newsize);
    netptr->newTextAddr = htonl(hostptr->newTextAddr);
}

textBlock_ntoh(netptr, hostptr)
     struct textBlock *netptr, *hostptr;
{
    hostptr->version = ntohl(netptr->version);
    hostptr->size = ntohl(netptr->size);
    hostptr->textAddr = ntohl(netptr->textAddr);
    hostptr->newsize = ntohl(netptr->newsize);
    hostptr->newTextAddr = ntohl(netptr->newTextAddr);
}

textLock_hton(hostptr, netptr)
     db_lockP	hostptr, netptr;
{
    netptr->type = htonl(hostptr->type);
    netptr->lockState = htonl(hostptr->lockState);
    netptr->lockTime = htonl(hostptr->lockTime);
    netptr->expires = htonl(hostptr->expires);
    netptr->instanceId = htonl(hostptr->instanceId);
    netptr->lockHost = htonl(hostptr->lockHost);
}

textLock_ntoh(netptr, hostptr)
     db_lockP	netptr, hostptr;
{
    hostptr->type = ntohl(netptr->type);
    hostptr->lockState = ntohl(netptr->lockState);
    hostptr->lockTime = ntohl(netptr->lockTime);
    hostptr->expires = ntohl(netptr->expires);
    hostptr->instanceId = ntohl(netptr->instanceId);
    hostptr->lockHost = ntohl(netptr->lockHost);
}

volumeEntry_ntoh(
  budb_dbVolume_t *netptr,
  budb_volumeEntry *hostptr)
{
    strcpy((char *)hostptr->name, (char *)netptr->name);
    hostptr->flags = ntohl(netptr->flags);
    DFSH_MemFromNetHyper(hostptr->id, netptr->id);
    strcpy((char *)hostptr->server, (char *)netptr->server);
    hostptr->partition = ntohl(netptr->partition);
    hostptr->nFrags = ntohl(netptr->nFrags);

    hostptr->position  = ntohl(netptr->position);
    hostptr->clone = ntohl(netptr->clone);
    hostptr->incTime = ntohl(netptr->incTime);
    hostptr->startByte = ntohl(netptr->startByte);
    hostptr->nBytes = ntohl(netptr->nBytes);
    hostptr->seq = ntohl(netptr->seq);

    hostptr->dump = ntohl(netptr->dump);
    strcpy((char *)hostptr->tape, (char *)netptr->tape);
}

/* -------------------------------------
 * structure conversion & copy routines 
 * -------------------------------------
 */

#ifdef notdef
copy_ktcPrincipal_to_budbPrincipal(ktcPtr, budbPtr)
     struct ktc_principal *ktcPtr;
     budb_principal *budbPtr;
{
    strncpy(budbPtr->name, ktcPtr->name, sizeof(budbPtr->name));
    strncpy(budbPtr->instance, ktcPtr->instance, sizeof(budbPtr->instance));
    strncpy(budbPtr->cell, ktcPtr->cell, sizeof(budbPtr->cell));
}
#endif

/* dumpToBudbDump
 * entry:
 *	dumpPtr - host format
 */

dumpToBudbDump(dumpPtr, budbDumpPtr)
     dbDumpP dumpPtr;
     budb_dumpEntry *budbDumpPtr;
{
    budbDumpPtr->id = dumpPtr->id;
    budbDumpPtr->parent = dumpPtr->parent;
    budbDumpPtr->level = dumpPtr->level;
    budbDumpPtr->flags = dumpPtr->flags;

    strcpy((char *)budbDumpPtr->volumeSetName, dumpPtr->volumeSet);
    strcpy((char *)budbDumpPtr->dumpPath, dumpPtr->dumpPath);
    strcpy((char *)budbDumpPtr->name, dumpPtr->dumpName);

    budbDumpPtr->created = dumpPtr->created;
    budbDumpPtr->nVolumes = dumpPtr->nVolumes;
    budbDumpPtr->tapes = dumpPtr->tapes;

    return(0);
}

tapeToBudbTape(tapePtr, budbTapePtr)
     struct tape *tapePtr;
     budb_tapeEntry *budbTapePtr;
{
    strcpy((char *)budbTapePtr->name, tapePtr->name);
    budbTapePtr->flags = tapePtr->flags;
    budbTapePtr->written = tapePtr->written;
    budbTapePtr->expires = tapePtr->expires;
    budbTapePtr->nMBytes = tapePtr->nMBytes;
    budbTapePtr->nBytes = tapePtr->nBytes;
    budbTapePtr->nFiles = tapePtr->nFiles;
    budbTapePtr->nVolumes = tapePtr->nVolumes;
    budbTapePtr->seq = tapePtr->seq;
    budbTapePtr->tapeid = tapePtr->tapeid;
    budbTapePtr->useCount = tapePtr->useCount;
    budbTapePtr->mediaType = tapePtr->mediaType;
    return(0);
}

volsToBudbVol(
  struct volFragment *volFragPtr,
  struct volInfo *volInfoPtr,
  budb_dbVolume_t *budbVolPtr)
{
    afs_hyper_t vid;

    strcpy((char *)budbVolPtr->name, volInfoPtr->name);
    budbVolPtr->flags = volInfoPtr->flags;

    /* Sadly, these structures are not identical, though they contain the same
     * fields. */
    DFSH_MemFromNetHyper(vid, volInfoPtr->id);
    DFSH_NetFromMemHyper(budbVolPtr->id, vid);

    strcpy((char *)budbVolPtr->server, volInfoPtr->server);
    budbVolPtr->partition = volInfoPtr->partition;
    budbVolPtr->nFrags = volInfoPtr->nFrags;

    budbVolPtr->position = volFragPtr->position;
    budbVolPtr->clone = volFragPtr->clone;
    budbVolPtr->incTime = volFragPtr->incTime;
    budbVolPtr->startByte = volFragPtr->startByte;
    budbVolPtr->nBytes = volFragPtr->nBytes;
    budbVolPtr->seq = volFragPtr->sequence;
    return(0);
}

/* ----------------------------------
 * structure initialization routines 
 *  ---------------------------------
 */

/* default_tapeset
 *	this fills in the tape set with the current default tapeset
 *	format;
 * entry:
 *	dumpname - volset.dumplevel
 * exit:
 *	0 - ok
 *	n - error
 */

default_tapeset(tapesetPtr, dumpname)
     budb_tapeSet *tapesetPtr;
     char *dumpname;
{
    bzero((char *)tapesetPtr, sizeof(*tapesetPtr));

    strcpy((char *)tapesetPtr->format, dumpname);
    strcat((char *)tapesetPtr->format, ".%d");
    tapesetPtr->a = 1;
    tapesetPtr->b = 1;
    tapesetPtr->maxTapes = 0;
}
