/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: instrument.h,v $
 * Revision 1.1.11.1  1996/10/02  17:42:07  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:34:53  damon]
 *
 * Revision 1.1.6.1  1994/06/09  14:04:02  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:38:52  annie]
 * 
 * Revision 1.1.4.2  1993/01/21  18:24:31  zeliff
 * 	Embedding copyright notices
 * 	[1993/01/19  14:14:54  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/05  17:45:41  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/04  23:24:26  jdp]
 * 
 * Revision 1.1  1992/01/19  02:39:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * instrument.h
 */

/*
 * (C) Copyright 1991 Transarc Corporation
 * All Rights Reserved
 */



#if !defined(TRANSARC_DE_INSTRUMENT_H)
#define TRANSARC_DE_INSTRUMENT_H

#define INS_GET 0x1
#define INS_SET 0x2
#define INS_GETNEXT 0x4

#define INS_DT_BYTES 0x1	/* uniterpreted byte stream */
#define INS_DT_LONG 0x2		/* a long */
#define INS_DT_ULONG 0x4	/* an unsigned long */
#define INS_DT_UINT16 0x8	/* an unsigned 16 bit integer */
#define INS_DT_CHAR 0x10	/* a character */
#define INS_DT_STRING 0x20	/* a character string */
#define INS_DT_GETNEXT_DROPOFF 0x40	/* data type for getnext error dropoff code */
#define INS_MAXOBJLEN 1024		/* maximum size of object allowed */
#define INS_MAXPROCS 100		/* total number of procedures allowed - not yet enforced */
#define INS_MAXVARS 100			/* maximum number of variables allowed per proc - not
					 yet enforced */

/* The generic object used in the system. The actual
 * data depends on the type. Everything is encoded as 
 * this tuple.
 */
struct ins_basicObject{		
    unsigned long type;		/* integer, string, uninterpreted etc. */
    unsigned long length;	/* how big is the value field that follows */
    char value[INS_MAXOBJLEN];
};

struct ins_varDesc{
    /* (procIdx,varIdx) tuple identifies a variable uniquely in the kernel*/
    long procIdx;	
    long varIdx;
};

struct ins_opDesc{
    long opcode;	/* INS_GET, INS_SET, and INS_GETNEXT */
    long count;		/* number of requests */
};

struct ins_sysOp{	/* used by the system call interface */
    struct ins_opDesc op;
    struct ins_varDesc var;
};

struct ins_basicObjectId{
    int totalComps;	/* total number of components in the Id */
    int progress;	/* how many components of the object ID have been consumed */
    struct ins_basicObject objId;
};

struct ins_varTableEntry{
    long *varAddr;	/* pointer to the variable */
    long varSize;	/* size of the variable */
};

struct ins_node{
    unsigned long type;		/* the type of the node */
    unsigned long id;		/* the id of the node */
    struct node *sibling;	/* used for tree walk, arrays and lists */
    struct node *parent;	/* pointer to the parent */
    struct node *child;		/* pointer to the child */
    long varIdx;		/* non zero for leaf nodes */
    long procIdx;		/* non zero for leaf-nodes, (procIdx,varIdx) identify a
				 variable uniquely in the kernel */
};

#endif /* TRANSARC_DE_INSTRUMENT_H */
