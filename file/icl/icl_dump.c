/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* Copyright (C) 1996, 1992 Transarc Corporation - All rights reserved */
/*
 * HISTORY
 * $Log: icl_dump.c,v $
 * Revision 1.1.64.1  1996/10/02  17:52:05  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:40:48  damon]
 *
 * $EndLog$
 */

/* This file contains the command to dump out the ICL in a readable
 * fashion from the Unix kernel.
 */

#include <errno.h>
#include <stdio.h>
#include <icl.h>	/* includes standard stuff */
#include <icl_errs.h>
#include <dcedfs/syscall.h>
#include <dcedfs/hash.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/icl/icl_dump.c,v 1.1.64.1 1996/10/02 17:52:05 damon Exp $")

/* make it big enough to snapshot everything at once, since
 * decoding takes so long.
 */
#define IBSIZE		100000	/* default size */

void icl_DumpLog _TAKES((FILE *, struct icl_log *, int));

struct logInfo {
    struct logInfo *nextp;
    char *name;
} *allInfo = 0;


/* PRIVATE */
char dumpFileName[256] = "";

RegisterIclDumpFileName(name)
  char *name;
{
    (void) sprintf(dumpFileName, "icl.%.250s", name);
}

/* Macro used to compute size of parameter when in log, used by
 * icl_AppendRecord and dump, etc.  The t1 parameter is never zero.  We
 * evaluate this in most likely first order.
 */
#define GETSIZE(t1, p1) \
    (((t1) <= ICL_TYPE_POINTER) ? 1	/* t1 is never zero: long, ptr */ \
     : (((t1) == ICL_TYPE_FID) ? 4 \
	: (((t1) == ICL_TYPE_HYPER) ? 2 \
	   : (((t1) == ICL_TYPE_UUID) ? 4 \
	      : (((t1) == ICL_TYPE_STRING) \
		 ? (int)((unsigned)(strlen((char *)(p1)) + 4) >> 2) \
		 : 1)))))		/* unixdate (dump only) */

/* Check types in printf string "bufferp", making sure that each
 * is compatible with the corresponding parameter type described
 * by typesp.  Also watch for prematurely running out of parameters
 * before the string is gone.
 */
static CheckTypes(bufferp, typesp, typeCount)
  char *bufferp;
  int *typesp;
  int typeCount;
{
    register char tc;
    int inPercent;
    int tix;

    inPercent = 0;
    tix = 0;
    for (tc = *bufferp;; tc = *(++bufferp)) {
	if (tc == 0) {
	    /* hit end of string.  We win as long as we aren't
	     * in a '%'.
	     */
	    if (inPercent) return 0;
	    else return 1;
	}
	if (tc == '%') {
	    inPercent = 1 - inPercent;
	    continue;
	}
	if (inPercent) {
	    inPercent = 0;
	    if (tc >= '0' && tc <= '9') continue; /* skip digits in % string */
	    /* otherwise, we've finally gotten to the type-describing
	     * character.  Make sure there's a type descriptor, and then
	     * check the type descriptor.
	     */
	    if (tix > typeCount)
		return 0;	/* no more type descriptors left */
	    if (tc == 's' && typesp[tix] != 1)	/* not a string descriptor */
		return 0;
	    if ((tc == 'u' || tc == 'l' || tc == 'x' || tc == 'd')
		&& (typesp[tix] != 0))
		return 0;	/* not an integer descriptor */
	    /* otherwise we're fine, so eat this descriptor */
	    tix++;
	}
    }
    /* not reached */
}

#define MAX_ALLOCATED_CODE2STR_MAPPINGS 1000

static struct dfsh_hashTable codeHash;
struct codeEntry {
    struct codeEntry *next;
    unsigned long code;
    u_long hits;			/* times used */
    char msg[1];
};

static unsigned long CodeHashEntry(struct codeEntry *h)
{
    return h->code;
}

/* FindText -- is a wrapper for calls to dce_error_inq_text which caches
 *     results using a hash table.
 *
 * PARAMETERS -- msg must point to a string buffer at least as large as
 *     dce_c_error_string_len. */

static int FindText (unsigned long code, char *msg)
{
    int status;
    struct codeEntry *h;

    static int inited = 0;

    if (!inited) {
	inited = 1;
	dfsh_HashInit (&codeHash, CodeHashEntry, /*thread offset*/0);
    }
    DFSH_LOOKUP (struct codeEntry *, &codeHash, h, code, (h->code == code));
    if (h) {
	h->hits++;
	strcpy (msg, h->msg);
	return 0;
    }
    dce_error_inq_text(code, msg, &status);
    if (status)
	return status;
    if (codeHash.entries < MAX_ALLOCATED_CODE2STR_MAPPINGS) {
	h = osi_Alloc (sizeof(*h)-sizeof(h->msg)+strlen(msg)+1);
	h->hits = 1;
	h->code = code;
	strcpy (h->msg, msg);
	dfsh_HashIn (&codeHash, h);
    }
    return 0;
}

/* display a single record.
 * alp points at the first word in the array to be interpreted
 * rsize gives the # of words in the array
 */
static DisplayRecord(outFilep, alp, rsize, displayStyle)
  FILE *outFilep;
  register long *alp;
  long rsize;
  int displayStyle;
{
    char msgBuffer[1024];
    long printfParms[ICL_MAXEXPANSION * /* max parms */ 4];
    int printfTypes[ICL_MAXEXPANSION * 4];
    int i;
    long temp;
    int j;
    int type;
    int pix;			/* index in alp */
    int pfpix;			/* index in printfParms */
    int pftix;			/* index in printfTypes */
    int status;
    int printed = 0;		/* did we print the string yet? */
    long *tlp;

    /* decode parameters */
    temp = alp[0];	/* type encoded in low-order 24 bits, t0 high */
    pix = 4;
    pfpix = 0;
    pftix = 0;
    /* init things */

    for (i = 0; i < 4 * ICL_MAXEXPANSION; i++) 
        printfParms[i] = 0;
    /* decode each parameter, getting addrs for hypers and strings */
    for (i = 0; i < 4; i++) {
	type = (temp >> (18 - i*6)) & 0x3f;
	if (type == ICL_TYPE_NONE) break;
	else if (type == ICL_TYPE_LONG || type == ICL_TYPE_POINTER) {
	    printfTypes[pftix++] = 0;	/* integer */
	    printfParms[pfpix++] = alp[pix];
	} else if (type == ICL_TYPE_HYPER) {
	    printfTypes[pftix++] = 0;	/* integer */
	    printfParms[pfpix++] = alp[pix];
	    printfTypes[pftix++] = 0;	/* integer */
	    printfParms[pfpix++] = alp[pix+1];
	} else if (type == ICL_TYPE_FID || type == ICL_TYPE_UUID) {
	    printfTypes[pftix++] = 0;	/* integer */
	    printfParms[pfpix++] = alp[pix];
	    printfTypes[pftix++] = 0;	/* integer */
	    printfParms[pfpix++] = alp[pix+1];
	    printfTypes[pftix++] = 0;	/* integer */
	    printfParms[pfpix++] = alp[pix+2];
	    printfTypes[pftix++] = 0;	/* integer */
	    printfParms[pfpix++] = alp[pix+3];
	} else if (type == ICL_TYPE_STRING) {
	    printfTypes[pftix++] = 1;	/* string */
	    printfParms[pfpix++] = (long) &alp[pix];
	} else if (type == ICL_TYPE_UNIXDATE) {
	    printfParms[pfpix++] = (long)ctime(&alp[pix]);
	}
	pix += GETSIZE(type, (char *) &alp[pix]);
    }

    if (displayStyle == ICL_DUMP_FORMATTED) {
	/* next, try to decode the opcode into a printf string */
	/* assert (sizeof(msgBuffer) >= dce_c_error_string_len) */
	status = FindText (alp[1], msgBuffer);
	/* if we got a string back, and it is compatible with the
	 * parms we've got, then print it.
	 */
	if (status == 0) {
	    if (CheckTypes(msgBuffer, printfTypes, pftix)) {
		/* we have a string to use, but it ends "(dfs / zcm)",
		 * so we remove the extra gunk.
		 */
		j = strlen(msgBuffer);
		if (j > 12) msgBuffer[j-11] = 0;
		fprintf(outFilep, "time %d.%06d, pid %u: ", alp[3]/1000000,
			alp[3] % 1000000, alp[2]);
		fprintf(outFilep, msgBuffer, printfParms[0], printfParms[1],
			printfParms[2], printfParms[3], printfParms[4],
			printfParms[5], printfParms[6], printfParms[7],
			printfParms[8], printfParms[9], printfParms[10],
			printfParms[11], printfParms[12], printfParms[13],
			printfParms[14], printfParms[15]);
		fprintf(outFilep, "\n");
		printed = 1;
	    } else {
		fprintf(outFilep, "Type mismatch, using raw print.\n");
		fprintf(outFilep, "%s", msgBuffer);
	    }
	}
    }
    if (!printed) {
	if ((displayStyle == ICL_DUMP_RAW) && (alp[1] == ICL_INFO_TIMESTAMP)) {
	    fprintf(outFilep, "current time event, ");
	} else {
	    fprintf(outFilep, "raw op %x, ", alp[1]);
	}
	fprintf(outFilep, "time %d.%06d, pid %u\n",
		alp[3]/1000000, alp[3] % 1000000, alp[2]);
	/* now decode each parameter and print it */
	pix = 4;
	for (i = 0; i < 4; i++) {
	    type = (temp >> (18 - i*6)) & 0x3f;
	    if (type == ICL_TYPE_NONE) break;
	    else if (type == ICL_TYPE_LONG) {
		fprintf(outFilep, "p%d:%d ", i, alp[pix]);
	    } else if (type == ICL_TYPE_POINTER) {
		fprintf(outFilep, "p%d:0x%x ", i, alp[pix]);
	    } else if (type == ICL_TYPE_HYPER) {
		fprintf(outFilep, "p%d:%x.%x ", i, alp[pix], alp[pix+1]);
	    } else if (type == ICL_TYPE_FID || type == ICL_TYPE_UUID) {
		fprintf(outFilep, "p%d:%x.%x.%x.%x ",  i, alp[pix], alp[pix+1],
		       alp[pix+2], alp[pix+3]);
	    } else if (type == ICL_TYPE_STRING) {
		fprintf(outFilep, "p%d:%s ", i, (char *) &alp[pix]);
	    } else if (type == ICL_TYPE_UNIXDATE) {
		fprintf(outFilep, "p%d:%s ", i, ctime(&alp[pix]));
	    }
	    pix += GETSIZE(type, (char *) &alp[pix]);
	}
	fprintf(outFilep, "\n");	/* done with line */
    }
}

icl_DumpKernel(outFilep, type, name, dumpStyle)
  FILE *outFilep;
  int type;
  char *name;
  int dumpStyle;
{
    long bufferSize = 0;
    long *bufferp;
    long i;
    long code, retVal = 0;
    char tname[64];
    long nwords;
    long ix;
    long rlength;
    long dummy, dummy2;
    struct logInfo *lip;

    /* first, enumerate the logs we're interested in */
    if (type == ICL_DUMP_SET) {
	int found = 0;
	/* dump logs for a particular set */
	for (i = 0; i < ICL_LOGSPERSET; i++) {
	    code = afs_syscall(AFSCALL_ICL, ICL_OP_ENUMLOGSBYSET,
			       (int) name, i, (int) tname,
			       sizeof(tname));
	    if (code) {
		if (errno == EBADF) {
		    code = 0;
		    continue;   /* missing slot, nothing to worry about */
		}
		break;
	    }
	    code = afs_syscall(AFSCALL_ICL, ICL_OP_GETLOGINFO,
			       (int) tname, (int) &dummy,
			       (int) &dummy2, 0);
	    if (code)
		break;
	    found++;
	    if (dummy > bufferSize)	/* find biggest log */
		bufferSize = dummy;
	    lip = (struct logInfo *) malloc(sizeof(struct logInfo));
	    bzero((char *)lip, sizeof(*lip));
	    lip->nextp = allInfo;
	    allInfo = lip;
	    lip->name = (char *) malloc(strlen(tname)+1);
	    strcpy(lip->name, tname);
	}
	i = found;
    } else if (type == ICL_DUMP_LOG) {
	/* dump particular log */
	code = afs_syscall(AFSCALL_ICL, ICL_OP_GETLOGINFO,
			   (int) name, (int) &dummy,
			   (int) &dummy2, 0);
	if (code)
	    return -1;
	i = 1;
	if (dummy > bufferSize)   
	    bufferSize = dummy;
	lip = (struct logInfo *) malloc(sizeof(struct logInfo));
	bzero((char *)lip, sizeof(lip));
	lip->nextp = allInfo;
	allInfo = lip;
	lip->name = (char *) malloc(strlen(name)+1);
	strcpy(lip->name, name);
    } else {
	/* dump all logs */
	for (i = 0; i < ICL_MAXLOGS; i++) {
	    code = afs_syscall(AFSCALL_ICL, ICL_OP_ENUMLOGS,
			       i, (int) tname, sizeof(tname), (int) &dummy);
	    if (code) break;
	    if (dummy > bufferSize)	/* find biggest log */
		bufferSize = dummy;
	    lip = (struct logInfo *) malloc(sizeof(struct logInfo));
	    bzero((char *)lip, sizeof(*lip));
	    lip->nextp = allInfo;
	    allInfo = lip;
	    lip->name = (char *) malloc(strlen(tname)+1);
	    strcpy(lip->name, tname);
	}
    }

    if (bufferSize == 0) return -1;
    bufferp = (long *) malloc(sizeof(long) * bufferSize);
    if (!bufferp) return -1;

    if (type != ICL_DUMP_LOG) {
	fprintf(outFilep, "Found %d logs.\n", i);
    }

    /* now print out the contents of each log */
    for (lip = allInfo; lip; lip=lip->nextp) {
	fprintf(outFilep, "\nContents of log %s:\n", lip->name);
	/* read out everything first; gets a more consistent
	 * snapshot.
	 */
	nwords = 0;		/* total words copied out */
	for (i = 0;;) {
	    /* display all the entries in the log */
	    if (bufferSize - nwords <= 0) break; /* filled whole buffer */
	    code = afs_syscall(AFSCALL_ICL, ICL_OP_COPYOUT,
			       (int) lip->name, (int) (bufferp+nwords),
			       bufferSize - nwords, (int) &i);
	    if (code < 0) {
		/* otherwise we've got an error */
		fprintf(outFilep, "Returned error %d dumping log.\n", errno);
		break;
	    }
	    /* otherwise, we have flags in the high order byte, and
	     * a length (in words) in the remainder.
	     */
	    if ((code >> 24) & ICL_COPYOUTF_MISSEDSOME)
		fprintf(outFilep, "Log wrapped; data missing.\n");
	    code &= 0xffffff;
	    if (code == 0) {
		/* we're done */
		break;
	    }
	    nwords += code;
	    i += code;
	}	/* for loop over all cookies */

	/* otherwise we should display all of the log entries here.
	 * Note that a record may end in the middle, in which case
	 * we should start over with the cookie value of the start
	 * of that record.
	 */
	for(ix = 0; ix<nwords;) {
	    /* start of a record */
	    rlength = (bufferp[ix] >> 24) & 0xff;
	    if (rlength <= 0) {
		fprintf(outFilep, "Internal error: 0 length record\n");
		retVal = -1;
		goto done;
	    }
	    /* ensure that entire record fits */
	    if (ix + rlength > nwords) {
		/* doesn't fit, adjust cookie and break */
		break;
	    }
	    /* print the record */
	    DisplayRecord(outFilep, &bufferp[ix], rlength, dumpStyle);
	    ix += rlength;
#ifdef notdef
	    /* obsolete: read entire buffer first */
	    i += rlength;	/* update cookie value, too */
#endif
	}	/* for loop displaying buffer */
    }		/* for loop over all logs */

  done:
    free(bufferp);
    return(retVal);
}

FILE *icl_OpenLogFile(name, quiet)
char *name;
int quiet;
{
    char *endp;
    char logfile[512], oldLogfile[512];
    FILE *fp;

    if (name && *name) {
	char *p;
	p = strchr(name, '/');
	if (p) {
	    if (!quiet)
		(void) fprintf(stderr, "Slashes (/) not allowed in dump file names\n");
	    return NULL;
	}
    } else
	name = dumpFileName;

    /* create the directories if necessary */	
    (void) sprintf(oldLogfile, "%s/var/dfs", DCELOCAL_PATH);
    if ((mkdir(oldLogfile, 701) < 0) && (errno != EEXIST)) {
	if (!quiet)
	    (void) fprintf(stderr,
			   "Cannot create directory %s for dump file\n",
			   oldLogfile);
	return NULL;
    }
    endp = &oldLogfile[strlen(oldLogfile)];
    (void) strcpy(endp, "/adm");
    if ((mkdir(oldLogfile, 755) < 0) && (errno != EEXIST)) {
	if (!quiet)
	    (void) fprintf(stderr,
			   "Cannot create directory %s for dump file\n",
			   oldLogfile);
	return NULL;
    }
    endp += sizeof("/adm")-1;
    *endp++ = '/';

    /* generate full path names for logfile and old logfile */
    if (!*name) {
	register struct icl_log *lp;
	/* 
	 * dump file name hasn't been set -- intuit it from the
	 * name of the first log, which happens to be at the end
	 * of the list.
	 */
	if (!icl_allLogs)
	    return NULL;

	(void) strcpy(endp, "icl.");
	endp += sizeof("icl.")-1;
	osi_mutex_enter(&icl_lock);
	for (lp = icl_allLogs; lp->nextp; lp = lp->nextp)
	    ;	/* place commercial here */
	(void) strcpy(endp, lp->name);
	osi_mutex_exit(&icl_lock);
    } else
	(void) strcpy(endp, name);
    endp += strlen(endp);

    (void) strcpy(logfile, oldLogfile);
    (void) strcpy(endp, ".old");

    /* move existing logfile to .old */
    (void) rename(logfile, oldLogfile);

    if ((fp = fopen(logfile, "w")) == NULL) {
	if (!quiet)
	    fprintf(stderr, "Failed to open dump file %s\n", logfile);
    }

    return fp;
}    

/* dump all user-level logs */
void icl_DumpUserToFile(outFilep)
  FILE *outFilep;
{
    long i;
    struct icl_log *logp;
    time_t startTime;

    /* first open the log file */
    if (!outFilep && 
	((outFilep = icl_OpenLogFile(NULL, /*!quiet*/0)) == NULL))
	return;

    startTime = time((long *) 0);
    (void) fprintf(outFilep, "DFS Trace Dump -\n\n   Date: %s",
		   ctime((time_t *)&startTime));

    /* now print out the contents of each log */
    osi_mutex_enter(&icl_lock);
    for (logp = icl_allLogs; logp; logp=logp->nextp) {
	logp->refCount++;	/* hold this guy */
	osi_mutex_exit(&icl_lock);
	osi_mutex_enter(&logp->lock);
	fprintf(outFilep, "\nContents of log %s:\n", logp->name);
	/* Always dump in formatted mode */
	icl_DumpLog(outFilep, logp, ICL_DUMP_FORMATTED);
	osi_mutex_exit(&logp->lock);
	osi_mutex_enter(&icl_lock);
	if (--logp->refCount == 0)
	    icl_ZapLog_r(logp);
    }
    osi_mutex_exit(&icl_lock);

}

void icl_DumpUser() 
{
    FILE *outFilep;

    /* first open the log file */
    if ((outFilep = icl_OpenLogFile(NULL, /*!quiet*/0)) == NULL)
	return;
    (void) icl_DumpUserToFile(outFilep);
    (void) fclose(outFilep);
}

/* dump a particular log */
icl_DumpUserLog(outFilep, logname)
  FILE *outFilep;
  char *logname;
{
    struct icl_log *logp;

    logp = icl_FindLog(logname);
    if (!logp)
	return -1;
    osi_mutex_enter(&logp->lock);
    /* always formatted dump style, for now */
    icl_DumpLog(outFilep, logp, ICL_DUMP_FORMATTED);
    osi_mutex_exit(&logp->lock);
    icl_LogRele(logp);
    return 0;
}

/* dump a user-level log.  called with logp->lock held. */
void icl_DumpLog(outFilep, logp, dumpStyle)
  FILE *outFilep;
  struct icl_log *logp;
  int dumpStyle;
{
    register long ix;
    register long i;
    register long j;
    long nwords;
    long rlength;
    long buffer[255];
    long last;

    if (logp->firstUsed != 0)
	fprintf(outFilep, "Log wrapped; data missing.\n");

    if (logp->firstUsed <= logp->firstFree) {
	last = logp->firstFree;		/* no wrapping */
	nwords = last - logp->firstUsed;
    } else {
	last = logp->logSize;		/* end of the log buffer for now */
	nwords = last - logp->firstUsed + logp->firstFree;
    }

    ix = logp->firstUsed;
    while (nwords > 0) {
	/* beginning of record */
	rlength = (logp->datap[ix] >> 24) & 0xff;
	if (rlength < 4) {
	    int i;
	    fprintf(outFilep, "Following record has invalid length:%d\n", 
		    rlength);
	    if (rlength < 1)
		rlength = 1;		/* avoid infinite loop */
	    for (i = 0; i < rlength; i++) 
		fprintf(outFilep, "#%x ", logp->datap[ix + i]);
	    fprintf(outFilep, "\n");
	    ix += rlength;
	    if (ix >= logp->logSize)
		ix -= logp->logSize;
	    nwords -= rlength;
	    continue;
	}

	/* try to avoid the copy */
	if ((ix + rlength) <= logp->logSize) {
	    /* no wrapping, just print it */
	    DisplayRecord(outFilep, &logp->datap[ix], rlength, dumpStyle);
	} else {
	    /* wraps -- have to copy it into buffer */
	    for (i = 0, j = ix; i < rlength; i++) {
		buffer[i] = logp->datap[j];
		if (++j == logp->logSize)
		    j = 0;
	    }
	    DisplayRecord(outFilep, buffer, rlength, dumpStyle);
	}
	ix += rlength;
	if (ix >= logp->logSize)
	    ix -= logp->logSize;
	nwords -= rlength;
    }
}

/* clear out log 'name' */
icl_ClearLog(name)
  char *name;
{
    long code;

    code = afs_syscall(AFSCALL_ICL, ICL_OP_CLRLOG, (int)name, 0, 0, 0);
    return code;
}

/* clear out set 'name' */
icl_ClearSet(name)
  char *name;
{
    long code;

    code = afs_syscall(AFSCALL_ICL, ICL_OP_CLRSET, (int) name, 0, 0, 0);
    return code;
}

/* clear out all logs */
icl_ClearAll()
{
    long code;

    code = afs_syscall(AFSCALL_ICL, ICL_OP_CLRALL, 0, 0, 0, 0);
    return code;
}

/* list out all available sets to outFileP */
int icl_ListSets(outFileP)
  FILE *outFileP;
{
    int i;
    long code = 0;
    long states;
    char tname[64];

    for (i = 0; i < 1000; i++) {
	code = afs_syscall(AFSCALL_ICL, ICL_OP_ENUMSETS,
		       i, (int) tname, sizeof(tname), (int) &states);
	if (code) break;
	(void) fprintf(outFileP, "%s: %s%s\n", tname,
		       (states & ICL_SETF_ACTIVE) ? "active" : 
		       ((states & ICL_SETF_FREED) ? "dormant" : "inactive"),
		       (states & ICL_SETF_PERSISTENT) ? " persistent" : "");
    }

    if (code && (errno == ENOENT))
	code = 0;

    return code;
}

/* list out all available logs to outFileP */
int icl_ListLogs(outFileP, longflg)
  FILE *outFileP;
  int longflg;
{
    int i;
    int allocated;
    long code = 0;
    long logSize;
    char tname[64];

    for (i = 0; i < 1000; i++) {
	code = afs_syscall(AFSCALL_ICL, ICL_OP_ENUMLOGS,
		       i, (int) tname, sizeof(tname), (int) &logSize);
	if (code) break;
	if (longflg) {
	    /* get more information on the log */
	    code = afs_syscall(AFSCALL_ICL, ICL_OP_GETLOGINFO, (int) tname, 
			       (int) &logSize, (int) &allocated, 0);
	    if (code)
		break;
	    (void) fprintf(outFileP, "%s : %d kwords (%s)\n",
			   tname, logSize/1024,
			   allocated ? "allocated" : "unallocated");
	} else
	    (void) fprintf(outFileP, "%s\n", tname);
    }

    if (code && (errno == ENOENT))
	code = 0;

    return code;
}

/* list out all available logs to outFileP */
int icl_ListLogsBySet(outFileP, setname, longflg)
  FILE *outFileP;
  char *setname;
  int longflg;
{
    int i;
    long code = 0;
    long logSize;
    int allocated;
    char tname[64];

    for (i = 0; i < ICL_LOGSPERSET; i++) {
	code = afs_syscall(AFSCALL_ICL, ICL_OP_ENUMLOGSBYSET,
		       (int) setname, i, (int) tname, sizeof(tname));
	if (code) {
	    if (errno == EBADF) {
		code = 0;
		continue;	/* missing */
	    }
	    break;
	}
	if (longflg) {
	    /* get more information on the log */
	    code = afs_syscall(AFSCALL_ICL, ICL_OP_GETLOGINFO, (int) tname, 
			       (int) &logSize, (int) &allocated, 0);
	    if (code)
		break;
	    (void) fprintf(outFileP, "%s : %d kwords (%s)\n",
			   tname, logSize/1024,
			   allocated ? "allocated" : "unallocated");
	} else
	    (void) fprintf(outFileP, "%s\n", tname);
    }

    return code;
}

/* activate/deactivate/free specified set */
int icl_ChangeSetState(name, op)
  char *name;
  long op;
{
    long code;

    code = afs_syscall(AFSCALL_ICL, ICL_OP_SETSTAT, (int) name, op, 0, 0);
    return code;
}

/* activate/deactivate/free all sets */
int icl_ChangeAllSetState(op)
  long op;
{
    long code;

    code = afs_syscall(AFSCALL_ICL, ICL_OP_SETSTATALL, op, 0, 0, 0);
    return code;
}

/* set size if log */
int icl_ChangeLogSize(name, logSize)
  char *name;
  long logSize;
{
    long code;

    code = afs_syscall(AFSCALL_ICL, ICL_OP_SETLOGSIZE, (int)name, logSize,
		       0, 0);
    return code;
}

/* get logsize of specified log */
int icl_GetLogsize(logname, logSizeP, allocatedP)
  char *logname;
  long *logSizeP;
  int *allocatedP;
{
    long code;
    code = afs_syscall(AFSCALL_ICL, ICL_OP_GETLOGINFO, (int) logname,
		       (int) logSizeP, (int) allocatedP, 0);
    return code;
}

/* get state of specified set */
int icl_GetSetState(setname, stateP)
  char *setname;
  long *stateP;
{
    long code;
    code = afs_syscall(AFSCALL_ICL, ICL_OP_GETSETINFO, (int) setname,
		       (int) stateP, 0, 0);
    return code;
}

icl_TailKernel(outFilep, logname, waitTime, dumpStyle)
  FILE *outFilep;
  char *logname;
  long waitTime;
  int dumpStyle;
{
    long bufferSize = 0;
    long newBufferSize;
    long *bufferp;
    long i;
    long code, retVal = 0;
    long nwords;
    long ix;
    long rlength;
    int allocated;
    struct logInfo *lip;

    /* get information about the specified log */
    code = afs_syscall(AFSCALL_ICL, ICL_OP_GETLOGINFO,
		       (int) logname, (int) &bufferSize, (int) &allocated, 0);
    if (code) {
	return errno;
    }

    if (!allocated) {
	return ICL_LOG_UNALLOCATED;
    }

    if (bufferSize == 0) return -1;
    bufferp = (long *) malloc(sizeof(long) * bufferSize);
    if (!bufferp) {
	return ENOMEM;
    }

    /* start "infinite" loop */
    for (;;) {
	/* read out all that's currently there */
	nwords = 0;		/* total words copied out */
	i = 0;	/* initialize cookie */
	for (;;) {
	    /* display all the entries in the log */
	    if (bufferSize - nwords <= 0) 
		break;	/* filled whole buffer, clear when done */
	    code = afs_syscall(AFSCALL_ICL, ICL_OP_COPYOUTCLR,
			       (int) logname, (int) (bufferp+nwords),
			       bufferSize - nwords, (int) &i);
	    if (code < 0) {
		/* otherwise we've got an error */
		retVal = errno;
		goto tail_done;
	    }
	    /* otherwise, we have flags in the high order byte, and
	     * a length (in words) in the remainder.
	     */
	    code &= 0xffffff;
	    if (code == 0) {
		/* we're done */
		break;
	    }
	    nwords += code;
	    i += code;
	}	/* for loop over all cookies */

	/* otherwise we should display all of the log entries here.
	 * Note that a record may end in the middle, in which case
	 * we should start over with the cookie value of the start
	 * of that record.
	 */
	for (ix = 0; ix < nwords;) {
	    /* start of a record */
	    rlength = (bufferp[ix] >> 24) & 0xff;
	    /* ensure that entire record fits */
	    if (ix+rlength > nwords) {
		/* doesn't fit, adjust cookie and break */
		if (rlength <= 0) {
		    retVal = ICL_BOGUS_RECORD;
		    goto tail_done;
		}
		break;
	    }
	    /* print the record */
	    DisplayRecord(outFilep, &bufferp[ix], rlength, dumpStyle);
	    ix += rlength;
	}	/* for loop displaying buffer */

	if (waitTime)
	    sleep(waitTime);

	/* see if things have changed */
	code = afs_syscall(AFSCALL_ICL, ICL_OP_GETLOGINFO,
			   (int) logname, (int) &newBufferSize,
			   (int) &allocated, 0);
	if (code) {
	    retVal = errno;
	    goto tail_done;
	}
	
	if (!allocated)	{
	    retVal = ICL_LOG_UNALLOCATED;
	    goto tail_done;
	}
	
	if (bufferSize == 0) {
	    retVal = ICL_LOG_SIZE_ZERO;
	    goto tail_done;
	}
	if (bufferSize != newBufferSize) {
	    /* have to reallocate a buffer */
	    bufferSize = newBufferSize;
	    free(bufferp);
	    bufferp = (long *) malloc(sizeof(long) * bufferSize);
	    if (!bufferp) {
		retVal = ENOMEM;
		goto tail_done;
	    }
	}
    }		/* infinite loop */

  tail_done:
    free(bufferp);
    return(retVal);
}

