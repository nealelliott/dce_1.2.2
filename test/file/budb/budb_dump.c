/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright(c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED(DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * Copyright(C) 1991, 1995 Transarc Corporation
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: budb_test.c,v $
	* $EndLog$
 */
/*
 * budb_test.c - Test program for the Backup Database
 */

#include <sys/types.h>
#include <unistd.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <dcedfs/stds.h>
#include <dcedfs/param.h>
#include <dcedfs/osi.h>
#include <dcedfs/osi_net.h>
#include <dce/rpc.h>
#include <pthread.h>
#include <dcedfs/compat.h>
#include <dcedfs/nubik.h>
#include <dcedfs/lock.h>
#include <com_err.h>
#include <dcedfs/cmd.h>
#include <dcedfs/tcdata.h>
#include <dcedfs/butc.h>
#include <dcedfs/bc.h>
#include <dcedfs/flserver.h>
#include <dcedfs/flclient.h>
#include <dcedfs/budb.h>
#include <dce/dce.h>
#include <dce/utc.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <database.h>

#define DB_FILE "/opt/dcelocal/var/dfs/backup/bkdb.DB0"

int print_dbHeader(int addr, struct dbHeader *ptr)
{
    printf("\nDATABASE HEADER: addr:\t%x\n\n",addr);
    printf("\tversion:\t%d\n",ntohl(ptr->version));
    printf("\tlastDumpId:\t%x\n",ntohl(ptr->lastDumpId));
    printf("\tlastInstanceId:\t%x\n",ntohl(ptr->lastInstanceId));
    printf("\teofPtr:\t\t%x\n",ntohl(ptr->eofPtr));
    printf("\tlastUpdate:\t%d\n",ntohl(ptr->lastUpdate));
    printf("\tcheckVersion:\t%d\n",ntohl(ptr->checkVersion));
    printf("\n");
    printf("\tfreePtrs\n");
    printf("\t  Free:\t\t%x\n",ntohl(ptr->freePtrs[0]));
    printf("\t  VolFrag:\t%x\n",ntohl(ptr->freePtrs[1]));
    printf("\t  VolInfo:\t%x\n",ntohl(ptr->freePtrs[2]));
    printf("\t  Tape:\t\t%x\n",ntohl(ptr->freePtrs[3]));
    printf("\t  Dump:\t\t%x\n",ntohl(ptr->freePtrs[4]));
    printf("\t  Hash:\t\t%x\n",ntohl(ptr->freePtrs[5]));
    printf("\n");
    printf("\tVolume Name Hash Table\n");
    printf("\t  functionType:\t%d\n",ntohl(ptr->volName.functionType));
    printf("\t  threadOffset:\t%d\n",ntohl(ptr->volName.threadOffset));
    printf("\t  entries:\t%d\n",ntohl(ptr->volName.entries));
    printf("\t  length:\t%d\n",ntohl(ptr->volName.length));
    printf("\t  table:\t%x\n",ntohl(ptr->volName.table));
    printf("\t  progress:\t%d\n",ntohl(ptr->volName.progress));
    printf("\t  oldLength:\t%d\n",ntohl(ptr->volName.oldLength));
    printf("\t  oldTable:\t%x\n",ntohl(ptr->volName.oldTable));
    printf("\n");
    printf("\tTape Name Hash Table\n");
    printf("\t  functionType:\t%d\n",ntohl(ptr->tapeName.functionType));
    printf("\t  threadOffset:\t%d\n",ntohl(ptr->tapeName.threadOffset));
    printf("\t  entries:\t%d\n",ntohl(ptr->tapeName.entries));
    printf("\t  length:\t%d\n",ntohl(ptr->tapeName.length));
    printf("\t  table:\t%x\n",ntohl(ptr->tapeName.table));
    printf("\t  progress:\t%d\n",ntohl(ptr->tapeName.progress));
    printf("\t  oldLength:\t%d\n",ntohl(ptr->tapeName.oldLength));
    printf("\t  oldTable:\t%x\n",ntohl(ptr->tapeName.oldTable));
    printf("\n");
    printf("\tDump Name Hash Table\n");
    printf("\t  functionType:\t%d\n",ntohl(ptr->dumpName.functionType));
    printf("\t  threadOffset:\t%d\n",ntohl(ptr->dumpName.threadOffset));
    printf("\t  entries:\t%d\n",ntohl(ptr->dumpName.entries));
    printf("\t  length:\t%d\n",ntohl(ptr->dumpName.length));
    printf("\t  table:\t%x\n",ntohl(ptr->dumpName.table));
    printf("\t  progress:\t%d\n",ntohl(ptr->dumpName.progress));
    printf("\t  oldLength:\t%d\n",ntohl(ptr->dumpName.oldLength));
    printf("\t  oldTable:\t%x\n",ntohl(ptr->dumpName.oldTable));
    printf("\n");
    printf("\tDump ID Hash Table\n");
    printf("\t  functionType:\t%d\n",ntohl(ptr->dumpIden.functionType));
    printf("\t  threadOffset:\t%d\n",ntohl(ptr->dumpIden.threadOffset));
    printf("\t  entries:\t%d\n",ntohl(ptr->dumpIden.entries));
    printf("\t  length:\t%d\n",ntohl(ptr->dumpIden.length));
    printf("\t  table:\t%x\n",ntohl(ptr->dumpIden.table));
    printf("\t  progress:\t%d\n",ntohl(ptr->dumpIden.progress));
    printf("\t  oldLength:\t%d\n",ntohl(ptr->dumpIden.oldLength));
    printf("\t  oldTable:\t%x\n",ntohl(ptr->dumpIden.oldTable));
}

int print_volFragBlock(int addr, struct vfBlock *ptr)
{
    int i;
    for ( i = 0 ; i < NvolFragmentS ; i++ )
    {
	printf("\n%x:",addr+sizeof(ptr->h)+i*sizeof(ptr->a[0]));
	printf("\tvol:\t\t%x\n",ntohl(ptr->a[i].s.vol));
	printf("\tsameNameChain:\t%x\n",ntohl(ptr->a[i].s.sameNameChain));
	printf("\ttape:\t\t%x\n",ntohl(ptr->a[i].s.tape));
	printf("\tsameTapeChain:\t%x\n",ntohl(ptr->a[i].s.sameTapeChain));
	printf("\tposition:\t%d\n",ntohl(ptr->a[i].s.position));
	printf("\tclone:\t\t%d\n",ntohl(ptr->a[i].s.clone));
	printf("\tincTime:\t%d\n",ntohl(ptr->a[i].s.incTime));
	printf("\tstartByte:\t%d\n",ntohl(ptr->a[i].s.startByte));
	printf("\tnBytes:\t\t%d\n",ntohl(ptr->a[i].s.nBytes));
	printf("\tflags:\t\t%x\n",ntohs(ptr->a[i].s.flags));
	printf("\tsequence:\t%d\n",ntohs(ptr->a[i].s.sequence));
    }
}

int print_volInfoBlock(int addr, struct viBlock *ptr)
{
    int i;
    for ( i = 0 ; i < NvolInfoS ; i++ )
    {
	afs_hyper_t id;
	DFSH_MemFromNetHyper(id, ptr->a[i].s.id);

	printf("\n%x:",addr+sizeof(ptr->h)+i*sizeof(ptr->a[0]));
	printf("\tname:\t\t'%s'\n",ptr->a[i].s.name);
	printf("\tflags:\t\t%x\n",ntohl(ptr->a[i].s.flags));
	printf("\tid:\t\t(%u,,%u)\n",AFS_HGETBOTH(id));
	printf("\tserver:\t\t'%s'\n",ptr->a[i].s.server);
	printf("\tpartition:\t%d\n",ntohl(ptr->a[i].s.partition));
	printf("\tnFrags:\t\t%d\n",ntohl(ptr->a[i].s.nFrags));
	printf("\tnameHashChain:\t%x\n",ntohl(ptr->a[i].s.nameHashChain));
	printf("\tsameNameHead:\t%x\n",ntohl(ptr->a[i].s.sameNameHead));
	printf("\tsameNameChain:\t%x\n",ntohl(ptr->a[i].s.sameNameChain));
	printf("\tfirstFragment:\t%x\n",ntohl(ptr->a[i].s.firstFragment));
    }
}

int print_tapeBlock(int addr, struct tBlock *ptr)
{
    int i;
    for ( i = 0 ; i < NtapeS ; i++ )
    {
	printf("\n%x:",addr+sizeof(ptr->h)+i*sizeof(ptr->a[0]));
	printf("\tname:\t\t'%s'\n",ptr->a[i].s.name);
	printf("\twritten:\t%d\n",ntohl(ptr->a[i].s.written));
	printf("\texpires:\t%d\n",ntohl(ptr->a[i].s.expires));
	printf("\tnMBytes:\t%d\n",ntohl(ptr->a[i].s.nMBytes));
	printf("\tnBytes:\t\t%d\n",ntohl(ptr->a[i].s.nBytes));
	printf("\tnFiles:\t\t%d\n",ntohl(ptr->a[i].s.nFiles));
	printf("\tnVolumes:\t%d\n",ntohl(ptr->a[i].s.nVolumes));
	printf("\tseq:\t\t%d\n",ntohl(ptr->a[i].s.seq));
	printf("\ttapeid:\t\t%x\n",ntohl(ptr->a[i].s.tapeid));
	printf("\tnameHashChain:\t%x\n",ntohl(ptr->a[i].s.nameHashChain));
	printf("\tdump:\t\t%x\n",ntohl(ptr->a[i].s.dump));
	printf("\tnextTape:\t%x\n",ntohl(ptr->a[i].s.nextTape));
	printf("\tfirstVol:\t%x\n",ntohl(ptr->a[i].s.firstVol));
    }
}

int print_dumpBlock(int addr, struct dBlock *ptr)
{
    int i;
    for ( i = 0 ; i < NdumpS ; i++ )
    {
	printf("\n%x:",addr+sizeof(ptr->h)+i*sizeof(ptr->a[0]));
	printf("\tid:\t\t%x\n",ntohl(ptr->a[i].s.id));
	printf("\tparent:\t\t%x\n",ntohl(ptr->a[i].s.parent));
	printf("\tlevel:\t\t%d\n",ntohl(ptr->a[i].s.level));
	printf("\tflags:\t\t%x\n",ntohl(ptr->a[i].s.flags));
	printf("\tvolumeSet:\t'%s'\n",ptr->a[i].s.volumeSet);
	printf("\tdumpPath:\t'%s'\n",ptr->a[i].s.dumpPath);
	printf("\tdumpName:\t'%s'\n",ptr->a[i].s.dumpName);
	printf("\tcreated:\t%d\n",ntohl(ptr->a[i].s.created));
	printf("\tnVolumes:\t%d\n",ntohl(ptr->a[i].s.nVolumes));
	printf("\ttapes.id:\t%x\n",ntohl(ptr->a[i].s.tapes.id));
	printf("\tidHashChain:\t%x\n",ntohl(ptr->a[i].s.idHashChain));
	printf("\tnameHashChain:\t%x\n",ntohl(ptr->a[i].s.nameHashChain));
	printf("\tfirstTape:\t%x\n",ntohl(ptr->a[i].s.firstTape));
    }
}

int print_hashBlock(int addr, struct htBlock *ptr)
{
    int i,j;
    int printFlag;
    char line[128];

    printf("\n");
    printFlag = 1;
    for ( i = 0 ; i < NhtBucketS ; i+= 4 )
    {
	if ( i == 0 || i+4 >= NhtBucketS ||
	     ptr->bucket[i] != 0 || ptr->bucket[i+1] != 0 ||
	     ptr->bucket[i+2] != 0 || ptr->bucket[i+3] != 0 )
	{
	    if ( printFlag == 0 ) printf("\t*\n");
	    printf("\t%x:\t",addr+sizeof(ptr->h)+i*sizeof(ptr->bucket[0]));
	    for ( j = i ; j < i + 4 && j < NhtBucketS ; j++ )
	    {
		printf(" %8x",ntohl(ptr->bucket[j]));
	    }
	    printf("\n");
	    printFlag = 1;
	}
	else printFlag = 0;
    }
}

int print_block(int addr, struct blockHeader *ptr)
{
    switch(ptr->type)
    {
    case free_BLOCK:
        printf("\nFREE BLOCK\n\n");
	break;
    case volFragment_BLOCK:
        printf("\nVOLUME FRAGMENT BLOCK\n\n");
	break;
    case volInfo_BLOCK:
        printf("\nVOLUME INFORMATION BLOCK\n\n");
	break;    case tape_BLOCK:
        printf("\nTAPE BLOCK\n\n");
	break;
    case dump_BLOCK:
        printf("\nDUMP BLOCK\n\n");
	break;
    case hashTable_BLOCK:
	printf("\nHASH TABLE BLOCK\n\n");
	break;
    case text_BLOCK:
	printf("\nTEXT BLOCK\n\n");
	break;
    default:
	printf("\n<ERROR> UNEXPECTED BLOCK TYPE <ERROR>\n\n");
    }

    printf("\taddr:\t\t%x\n",addr);
    printf("\tflags:\t\t%x\n",ntohl(ptr->flags));
    printf("\tnFree:\t\t%x\n",ntohl(ptr->nFree));
    printf("\tnext:\t\t%x\n",ntohl(ptr->next));
    switch(ptr->type)
    {
    case volFragment_BLOCK:
	print_volFragBlock(addr,(struct vfBlock *)ptr);
	break;
    case volInfo_BLOCK:
	print_volInfoBlock(addr,(struct viBlock *)ptr);
	break;
    case tape_BLOCK:
	print_tapeBlock(addr,(struct tBlock *)ptr);
	break;
    case dump_BLOCK:
	print_dumpBlock(addr,(struct dBlock *)ptr);
	break;
    case hashTable_BLOCK:
	print_hashBlock(addr,(struct htBlock *)ptr);
	break;
    }
}

main(int argc, char *argv[])
{
    char *ptr;
    int fd;
    int rc;
    int addr;

    ptr = (char *)malloc(BLOCKSIZE);
    if ( ptr == NULL )
    {
	perror("malloc");
	exit(1);
    }

    fd = open(DB_FILE,O_RDONLY,0);
    if ( fd < 0 )
    {
	perror(DB_FILE);
	exit(1);
    }

    rc = read(fd,ptr,64);
    if ( rc == 0 ) exit(0);
    else if ( rc < 0 )
    {
        perror("read");
        exit(1);
    }

    addr = 0;

    rc = read(fd,ptr,sizeof(struct dbHeader));
    if ( rc == 0 ) exit(0);
    else if ( rc < 0 )
    {
        perror("read");
        exit(1);
    }
    print_dbHeader(addr,(struct dbHeader *)ptr);
    addr += rc;

    while (1)
    {
	rc = read(fd,ptr,BLOCKSIZE);
	if ( rc == 0 ) break;
	else if ( rc < 0 )
	{
	    perror("read");
	    exit(1);
	}

	print_block(addr,(struct blockHeader *)ptr);
	addr += rc;
    }

    close(fd);
    exit(0);
}
