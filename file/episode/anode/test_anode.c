/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: test_anode.c,v $
 * Revision 1.1.82.1  1996/10/02  17:19:17  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:28:54  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1996, 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/osi_device.h>
/* XXX FIX ME XXX */
#undef	PRIVATE
#undef	SHARED
#include <dcedfs/stds.h>
#include <dcedfs/osi.h>
#include <dcedfs/debug.h>

#include <ctype.h>
#include <string.h>
#include <time.h>
#include <signal.h>

#include <com_err.h>
#include <dcedfs/hash.h>
#include <dcedfs/episode/fifo.h>
#include <dcedfs/episode/critical.h>
#include <dcedfs/episode/epi_thread.h>
#include <dcedfs/episode/async.h>
#include <dcedfs/episode/instrument.h>
#include <dcedfs/episode/logbuf.h>
#include <volreg.h>

/* user space I/O simulation */
#include <dcedfs/episode/us.h>

/* user space VM simulation */
#include <dcedfs/osi_buf.h>
#include <test_vm.h>

#include <anode.h>
#include <fixed_anode.h>
#include <block_alloc.h>		/* coverage fcns */
#include <fid.h>
#include <volume.h>
#include <file.h>
#include <aggregate.h>
#include <verify.h>
#include <salvage.h>
#include <boot.h>

#include <runtest.h>

RCSID ("$Header: /u0/rcs_trees/dce/rcs/file/episode/anode/test_anode.c,v 1.1.82.1 1996/10/02 17:19:17 damon Exp $")

#if defined(AFS_AIX_ENV) || defined(AFS_HPUX_ENV)
#define sscanf_r(c,a) (cma_lock_global(), (c=sscanf a), cma_unlock_global(), c)
#define fscanf_r(c,a) (cma_lock_global(), (c=fscanf a), cma_unlock_global(), c)
#else
#define sscanf_r(c,a) (c=sscanf a)
#define fscanf_r(c,a) (c=fscanf a)
#endif

#ifdef DOBTOOL
#define Exit BtoolExit
extern char *btool_log_name;
extern char *btool_test_name;
static char BtoolLogname[1024];
#define BTOOLSETUP \
  { btool_test_name = "test_anode"; \
    (void) sprintf(BtoolLogname, "btool.dmp.%d", getpid()); \
    btool_log_name = BtoolLogname; \
  }
#else
#define Exit exit
#define BTOOLSETUP {}
#endif /*DOBTOOL*/

/* EXPORT */
int afsdb_episode_anode_test = 0;
#define TestPrintStatus() (afsdb_episode_anode_test & 2) /* file/vol status */
#define TestPrintCoverage() (afsdb_episode_anode_test & 4) /* test coverage */
#define TestPrintSleep() (afsdb_episode_anode_test & (1<<(32-1)))

/* this overrides DBprint from fixed_anode.h */
#undef DBprint
#define DBprint(p) dprintf (afsdb_episode_anode_test, p)
#define ALWAYSprint(p) dprintf (1, p)

static int os_supports_vm = OS_SUPPORTS_VM;

/* Knuth claims multiplier in LCG ((a*X+c) mod m) should be = 5 mod 8, with c =
 * 1 or a.  With m a power of two.  Note that high bits are "more" random than
 * low bits. */
#define BIG_PRIME 1257056893		/* 0x4AED2A7d */
#define NextRandom(p) \
    ((p)->randomSeed = (p)->randomSeed * BIG_PRIME + BIG_PRIME)
#define NewRandom(p, v) \
    ((p)->randomSeed = (p)->randomSeed * BIG_PRIME + (v))

/* This is a global database describing the partitions we're working with */
static struct partition {
    u_long nBlocks;
    u_long fsize;
    u_long bsize;
} partition[US_MAX_MINOR_DEV];

/* vm_lock -- synchronizes access to the VM system so that WriteFile (and to a
 *     smaller extent ReadFile) can see a consistent view.  This is
 *     particularly important for obtaining error codes.  An invalidate that
 *     intervenes in the middle of a WriteFile that checks for an error code
 *     can lose the error.  Thus invalidates on the whole VM require an
 *     exclusive lock while WriteFile obtains a sharable lock. */

static osi_dlock_t vm_lock;

#define ObtainRVM() do {if (useVM) lock_ObtainRead (&vm_lock); } while (0)
#define ReleaseRVM() do {if (useVM) lock_ReleaseRead (&vm_lock); } while (0)
#define ObtainWVM() do {if (useVM) lock_ObtainWrite (&vm_lock); } while (0)
#define ReleaseWVM() do {if (useVM) lock_ReleaseWrite (&vm_lock); } while (0)

/* for now these are just random constants */
#define FIRST_BLOCK 4			/* first legal block number */
#if 0  /* call elbb_ComputeMinLogSize instead */
#define MinLogSize(bsize)		/* See log_Init for details. */ \
    (((35<<10/*max tran size*/)/(bsize) +1) /* funny round-up */	\
     + 2/*log full margin*/						\
     + 1/*must be strictly bigger than this*/				\
     + 1/*to make it work!!!!*/)
#endif /*0*/

#if defined(AFS_HPUX_ENV) || defined(AFS_SUNOS5_ENV)
/* the name file is already taken on some platforms. */
#define file epi_test_anode_file
#endif
/* Sorta like a vnode, describes a file and remembers its open state. */
struct file {
    u_int devi;
    afs_hyper_t volId;
    epi_volFileId_t fid;
    struct async_device *device;
    epi_anode_t avl;
    epi_volume_t vol;
    epi_anode_t file;
    long flags;				/* currently EPIF_CFLAGS_DIRECTORY */
};

#define FileIsDir(file) ((file)->flags & EPIF_CFLAGS_DIRECTORY)

struct fileName {
    struct fileName *next;
    char name[32];
    epi_volFileId_t fid;
    afs_hyper_t volId;
    u_int devi;
};

#define RANDOM_DATA_WORDS 64

struct processData {
    char buf[64];			/* print buffer */
    char buf2[64];			/* auxiliary print buffer */
    /* This is the name of the test that was run.  Sorta an ouput parameter */
    char testName[EPIR_MAX_TESTNAME];	/* note: this is inline */
    /* fields from opData */
    long errorCode;
    char *whereami;
    char *testFile;
    long version;
    pthread_t pid;
    struct processData *parent;		/* parent pointer */
    int nChildren;
    /* for doing filename / fid translations */
    struct dfsh_hashTable fileNameHT;
    /* per process random number and random data state */
    u_long randomSeed;			/* random number state */
    u_long words[RANDOM_DATA_WORDS];	/* random sequence */
    u_long wordsOffset;			/* offset of last used byte */
    u_long wordsBase;			/* offset of current words array */
    u_long wordsRandom;			/* file portion of random sequence */
    /* buffers for file xfer */
    u_long buflen;
    char *databuf;			/* for reading and writing */
    char *cmpbuf;			/* for comparing result of reading */
    /* current file identification and open handles */
    struct file cur;
    /* status for future comparison */
    struct epig_status oldAggrStatus;
    struct epiv_status oldVolStatus;	/* for change comparisons */
    struct epia_info oldInfo;		/* for checking changes */
    struct epif_status oldStatus;
    /* for recursion, we'll need the operation table and other stuff */
    struct epir_operationTable *ops;
};

/* external */
extern char *ucstring ();
extern char *lcstring ();

/* forward */
static void PushVMWrites _TAKES((
  IN struct processData *p));
static long DoWriteFile _TAKES((
  IN struct processData *p,
  IN u_long offset,
  IN u_long length,
  IN int useRandomValues));
static struct fileName *LookupFile _TAKES((
  IN struct processData *p,
  IN struct file *context,		/* context for resolving ambiguities */
  IN char *name));

char whoami[] = "test_anode";

#define PrintedFid(fid, len, buf) epid_PrintedFid(fid, len, buf)

#define PrintedHyper(h, len,buf) epiz_PrintedHyper(h, len, buf)

static long Pause(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    char buf[10];
    (void) fprintf(stderr, "[%s] Continue? ", o->whereami);
    fgets(buf, sizeof (buf), stdin);
    return(0);
}

long GetCurrentDevice (p, file)
  IN struct processData *p;
  INOUT struct file *file;
{
    long code;

    struct async_device *device = 0;
    long bsize;

    if (file->devi == -1) {
	com_err (p->whereami, ENXIO, "No device currently open");
	return -1;
    }
    bsize = partition[file->devi].bsize;
    code = asevent_InitDevice (US_MAJOR_DEV, file->devi, 0, &device);
    if (code == 0) {
	if (bsize && device->blkSize != bsize) {
	    code = asevent_CloseDevice(device);
	    afsl_MBZ (code);
	    device = 0;
	}
    }

    if (!device) {
	code = asevent_InitDevice (US_MAJOR_DEV, file->devi, bsize, &device);
	if (code) {
	    com_err (p->whereami, code, "calling async_InitDevice");
	}
    }

    if (device) {
	partition[file->devi].bsize = device->blkSize;
    }

    file->device = device;
    return code;
}

long GetCurrentAggr (p, file)
  IN struct processData *p;
  INOUT struct file *file;
{
    long code;
    struct async_device *device;

    if (file->avl) {
	afsl_Assert(file->device &&
		(file->devi == file->device->mindev));
	afsl_Assert(epia_AnodeDevice(file->avl) == file->device);
	afsl_Assert(file->device->blkSize);
	return 0;
    }

    if (file->devi == -1) {
	com_err (p->whereami, ENXIO, "No device currently open");
	return -1;
    }
    code = epig_InitAggregate (US_MAJOR_DEV, file->devi,1,
			       &device, &file->avl);
    if (code) {
	file->device = 0;
	if (p->errorCode == code) return code;
	com_err (p->whereami, code, "initializing aggregate");
	return code;
    }
    file->device = device;
    afsl_Assert(!partition[file->devi].fsize ||
	    ((file->device)->fragSize == partition[file->devi].fsize));
    afsl_Assert(device->blkSize);
    code = epig_AttachAggregate (file->avl);
    if (code) {
	if (p->errorCode == code) return code;
	com_err (p->whereami, code, "attaching aggregate");
	return code;
    }
    return 0;
}

long GetCurrentVol (p, file)
  IN struct processData *p;
  INOUT struct file *file;
{
    long code;
    epi_anode_t otherAvl;
    u_long volIndex;
    char buf[32];

    if (code = GetCurrentAggr (p, file)) return code;
    if (file->vol) return 0;

    if (AFS_hiszero(file->volId)) {
	com_err (p->whereami, code = EPI_E_NOENT, "No current volume");
	return code;
    }

    code = vr_LookupVolumeId (file->volId, &otherAvl, &volIndex);
    if (code) {
	if (p->errorCode == code) return code;
	com_err (p->whereami, code, "looking up current volume %s",
		 PrintedHyper (file->volId, sizeof (buf), buf));
	return code;
    }
    if (otherAvl != file->avl) {
	fprintf (stderr, "Volume id %s in unexpected aggregate\n",
		 PrintedHyper(file->volId, sizeof (buf), buf));
	return EPI_E_VOLIDEXISTS;
    }

    code = epig_OpenVolume (file->avl, volIndex, &file->vol);
    if (code) {
	if (p->errorCode == code) return code;
	com_err (p->whereami, code, "opening volume %s at index %d",
		 PrintedHyper(file->volId, sizeof (buf), buf), volIndex);
	return code;
    }
    return 0;
}

long GetCurrentFile (p, file)
  IN struct processData *p;
  INOUT struct file *file;
{
    long code;
    struct epia_info info;
    char buf[32];

    if (code = GetCurrentVol (p, file)) return code;
    if (file->file) return 0;

    if (epid_IsInvalid(&file->fid)) {
	com_err (p->whereami, code = EPI_E_NOENT, "No current file");
	return code;
    }
    code = epif_Open (file->vol, &file->fid, &file->file);
    if (code) {
	if (p->errorCode == code) return code;
	com_err (p->whereami, code, "opening an anode in a volume");
	return code;
    }
    code = epia_GetInfo (file->file, &info);
    if (code) {
	if (p->errorCode == code) return code;
	com_err (p->whereami, code, "getting info on file %s",
		 PrintedFid (&file->fid, sizeof (buf), buf));
	return code;
    }
    file->flags = (info.flags&EPIA_CFLAGS_LOGGED) ? EPIF_CFLAGS_DIRECTORY : 0;
    return 0;
}

#define WITH_TRANS(service, file, a) \
    BEGIN_LCRIT (elbb_tranRec_t trans, \
		 code = epia_StartTran (service, (file)->avl, &trans), \
		 code = epia_EndTran ((file)->avl, trans)) { \
        a; \
    } END_CRIT

long ClosePreviousFile (p, file)
  IN struct processData *p;
  INOUT struct file *file;
{
    long code = 0;
    char buf[32];

    if (file->file) {
	BEGIN_CRIT (ObtainRVM(),
		    ReleaseRVM()) {
	    PushVMWrites (p);		/* if any delayed writes */
	} END_CRIT /* vm lock */;

	code = epif_Close (file->file);
	if (code) {
	    com_err (p->whereami, code, "closing previous file %s",
		     PrintedFid(&file->fid, sizeof (buf), buf));
	}
    }
    file->file = 0;
    epid_MakeInvalid (&file->fid);
    return code;
}

long ClosePreviousVol (p, file)
  IN struct processData *p;
  INOUT struct file *file;
{
    long code = 0;
    char buf[32];

    if (code = ClosePreviousFile (p, file)) return code;
    if (file->vol) {
	code = epig_CloseVolume (elbb_NullTranId, file->vol);
	if (code) {
	    com_err (p->whereami, code, "closing previous volume %s",
		     PrintedHyper(file->volId, sizeof (buf), buf));
	}
    }
    AFS_hzero (file->volId);
    file->vol = 0;
    return code;
}

static long ClosePreviousAggr(
  IN struct processData *p,
  INOUT struct file *file)
{
    long code;
    struct async_device *device;

    if (code = ClosePreviousVol (p, file)) return code;

    if (useVM && (&p->cur == file)) {
	/* invalidate VM if we're closing the default context */
	ObtainWVM();
	code = vm_Write (VM_WRITE|VM_SYNC|VM_AWAIT|VM_INVALIDATE, 0);
	if (code) {
	    com_err (p->whereami, code, "I/O error closing aggregate");
	    return code;
	}
	ReleaseWVM();
    }

    if (file->avl == 0) return 0;
    code = GetCurrentDevice(p, file);
    if (code) {
	com_err (p->whereami, code, "Device %d never opened", file->devi);
	return code;
    }
    if (epia_AnodeDevice (file->avl) != file->device) {
	com_err (p->whereami, 0, "aggregate not closed: wrong device");
	return -1;
    }

    code = epig_CloseAggregate (file->avl);
    if (code) {
	com_err (p->whereami, code, "closing avl");
	return code;
    }
    file->avl = 0;
    file->device = 0;
    return 0;
}

static unsigned long FileNameHash(
  IN struct fileName *entry)
{
    unsigned char *str = (unsigned char *)entry->name;
    unsigned long hash = 1000003;	/* big prime to make "" hash nicely */
    /* The multiplicative constant should be odd and have a goodly number of
     * one bits. */
    while (*str) hash = (*str++) + hash * 0x10204081;
    return hash;
}

/* NameAsFid -- convert a name to a fid, filling in output fid and volid and
 *     returning true if possible, return zero otherwise. */

static int NameAsFid(
  IN struct processData *p,
  IN char *name,
  OUT epi_volFileId_t *fid,
  OUT afs_hyper_t *volidP)
{
    int count;
    u_long i, u;			/* fid parts */
    char vol[32];			/* volume/file name */

    sscanf_r (count, (name, " (%lu,%lu)[%31s]", &i, &u, vol));
    if (count == 2) {
	epid_Assemble (fid, i, u);
	AFS_hzero (*volidP);
	return 1;			/* regular fid, no volume */
    }
    else if (count == 3) {

	/* Check for qualifying volume for fid.  Either a volume id or a file
         * name whose volume to use. */

	int len = strlen(vol);
	u_long h,l;			/* hi,low parts of afs_hyper_t */
	struct fileName *fn;		/* result of file name lookup */

	if (vol[len-1] != ']')
	    return 0;			/* must include trailing bracket */
	vol[--len] = 0;

	sscanf_r (count, (vol, "%lu,,%lu", &h, &l));
	if (count == 2) {
	    epid_Assemble (fid, i, u);
	    AFS_hset64 (*volidP, h,l);
	    return 1;
	} else {
	    fn = LookupFile (p, 0, vol);
	    if (!fn) return 0;
	    epid_Assemble (fid, i, u);
	    *volidP = fn->volId;
	    return 1;
	}
    }
    return 0;
}

static struct fileName *LookupFile(
  IN struct processData *p,
  IN struct file *context,		/* context for resolving ambiguities */
  IN char *name)
{
    struct fileName *f;
    epi_volFileId_t fid;
    afs_hyper_t volid;
    long cnt = 0;

    AFS_hzero (volid);

    if (name) {
	struct fileName fn;

	strncpy (fn.name, name, sizeof (fn.name));
	f = dfsh_HashLookup (&p->fileNameHT, &fn);
	if (f) return f;

	if (!NameAsFid (p, name, &fid, &volid))
	    return 0;
    } else
	fid = context->fid;

    /* locate file with fid and optional volid, the hard way... */
    if (AFS_hiszero(volid))
	volid = context->volId;
    for (f = dfsh_HashNext (&p->fileNameHT, NULL);
	 f;
	 f = dfsh_HashNext(&p->fileNameHT, f)) {
	if ((epid_Equal (&fid, &f->fid) ||
	     ((epid_ToUnique(&fid) == EPID_DONTCARE_UNIQUE) &&
	      (epid_ToIndex(&fid) == epid_ToIndex(&f->fid)))) &&
	    AFS_hsame (volid, f->volId) && (context->devi == f->devi))
	    return f;
	afsl_Assert(cnt++ < p->fileNameHT.entries);
    }
    /* none found */
    return 0;
}

static void RemoveFile(
  IN struct processData *p,
  IN struct file *context)
{
    long code;
    struct fileName *f;
    f = LookupFile (p, context, 0);
    if (!f) return;			/* already removed */
    code = dfsh_HashOut_r (&p->fileNameHT, f);
    afsl_MBZ (code);
    epid_MakeInvalid (&context->fid);
    context->file = 0;
    osi_Free (f, sizeof (*f));
}

static long RestoreFileNameHT(
  IN struct processData *p,
  FILE *stream)
{
    long code;
    while (!feof(stream)) {
	struct fileName *f;
	u_long vlo, vhi, fa, fb;
	int count;

	f = (struct fileName *) osi_Alloc (sizeof (*f));
	bzero ((char *)f, sizeof (*f));
	fscanf_r (count, (stream, "%31s (%lu,%lu) %lu,,%lu %d",
			  f->name, &fa, &fb, &vhi, &vlo, &f->devi));
	if ((count < 0) && feof(stream)) break;
	if (count != 6) {
	    fprintf (stderr, "Error reading file name info\n");
	    return -1;
	}
	epid_Assemble(&f->fid, fa, fb);
	AFS_hset64 (f->volId, vhi, vlo);
	code = dfsh_HashIn_r (&p->fileNameHT, f);
	if (code) {
	    com_err (p->whereami, code,
		     "hashing in file name (%s) during restore", f->name);
	    return -1;
	}
    }
    return 0;
}

static long DumpFileNameHT(
  IN struct processData *p,
  FILE *stream)
{
    struct fileName *f;
    char buff[32], bufh[32];
    long cnt = 0;

    f = 0;
    while (1) {
	f = dfsh_HashNext (&p->fileNameHT, f);
	if (!f) break;
	fprintf (stream, "%s %s %s %d\n", f->name,
		 PrintedFid (&f->fid, sizeof (buff), buff),
		 PrintedHyper(f->volId, sizeof (bufh), bufh), f->devi);
	afsl_Assert(cnt++ < p->fileNameHT.entries);
    }
    return 0;

}

static long CopyFileNameHT(
  IN struct processData *p,
  IN struct processData *newp)
{
    long code;
    struct fileName *f;
    struct fileName *newf;
    long cnt = 0;

    code = dfsh_HashInit (&newp->fileNameHT, FileNameHash,
			  offsetof (struct fileName, next));
    if (code) return code;
    code = dfsh_HashInitKeyString (&newp->fileNameHT,
				   offsetof (struct fileName, name[0]));
    if (code) return code;

    f = 0;
    while (1) {
	f = dfsh_HashNext (&p->fileNameHT, f);
	if (!f) break;
	afsl_Assert(cnt++ < p->fileNameHT.entries);
	newf = (struct fileName *) osi_Alloc (sizeof (*newf));
	bzero ((char *)newf, sizeof (*newf));
	*newf = *f;
#if 0
	strcpy (newf->name, f->name);
	newf->fid = f->fid;
	newf->volId = f->volId;
#endif
	code = dfsh_HashIn_r (&newp->fileNameHT, newf);
	if (code) {
	    com_err (p->whereami, code,
		     "hashing in file name (%s) during copy", newf->name);
	    return -1;
	}
    }
    return 0;
}

#define BYTES_IN_WORDS_MASK (RANDOM_DATA_WORDS*sizeof (u_long) - 1)

static void InitRandomValues(
  IN struct processData *p,
  IN afs_hyper_t volId,
  IN epi_volFileId_t *fid,
  IN u_long offset)
{
    u_long saveSeed = p->randomSeed;

    p->randomSeed = AFS_hgethi(volId);
    NewRandom (p, AFS_hgetlo(volId));
    if (fid) {
	NewRandom (p, epid_ToIndex(fid));
	NewRandom (p, epid_ToUnique(fid));
    }
    p->wordsRandom = p->randomSeed;
    p->randomSeed = saveSeed;
    p->wordsOffset = offset;
    p->wordsBase = 1;			/* an illegal value */
}

static void GetRandomValues(
  IN struct processData *p,
  IN int len,
  IN char *buf)
{
#if 0 /* this actually performs worse */
    if (!TestGoodRandomValues()) {
	int i;
	int v = p->wordsRandom + p->wordsOffset;
	for (i=0; i<len; i++) *buf++ = v++;
	p->wordsOffset += len;
	return;
    }
#endif
    while (len > 0) {
	int off = p->wordsOffset & BYTES_IN_WORDS_MASK;
	int base = p->wordsOffset & ~BYTES_IN_WORDS_MASK;
	int bytesLeft = sizeof (p->words) - off;
	int l;
	if (base != p->wordsBase) {	/* re-fill words array */
	    int i;
	    u_long saveSeed = p->randomSeed;
	    p->randomSeed = p->wordsRandom; /* restore random sequence */
	    p->wordsBase = base;	/* new base */
	    NewRandom (p, p->wordsBase);
	    for (i = 0; i < 64; i++) p->words[i] = NextRandom(p);
	    p->randomSeed = saveSeed;
	}
	l = MIN(len, bytesLeft);
	bcopy((char *)(p->words) + off, buf, l);
	len -= l;
	buf += l;
	p->wordsOffset += l;
    }
}

typedef struct {
    char type;
    union {
	long l;
	struct timeval t;
	afs_hyper_t h;
    } v;
} value_t;
#define VALUE_HYPERS "lVv"		/* l = file Length */
					/* V = volume's volume Version */
					/* v = file's volume Version */
#define VALUE_TIMES "mac"		/* m = file Mod time */
					/* a = file Access time */
					/* c = file Change file */
#define VALUE_LONGS "zZdADFPUuqgQGf"	/* z = container allocated siZe */
					/* Z = container visible siZe */
					/* d = file Data version number */
					/* A = file Acl length */
					/* D = file Initial Dir Acl length */
					/* F = file Initial File Acl length */
					/* P = file Plist length */
					/* U = file User Plist length */
					/* u = volume Uniquifier */
					/* q = volume allocated Quota limit */
					/* g = volume allocated Quota usage */
					/* Q = volume visiBle quota limit */
					/* G = volume visiBle quota usage */
					/* f = aggregate Free space */
#define VALUE_SHORTS "htpo"		/* h = file Hard link count */
					/* t = file Type */
					/* p = file Permission bits */
					/* o = Outstanding copies */
long ScanValue (c, s, value)
  char c;
  char *s;
  value_t *value;
{
    char *sp;
    long l;
    bzero ((char *)value, sizeof (*value));
    value->type = c;
    if (strchr (VALUE_HYPERS, c)) {
	int code = dfsh_StrToHyper(s, &value->v.h, &sp);
	if (code || *sp) return -1;
	return 0;
    } else if (strchr (VALUE_TIMES, c)) {
	return -1;			/* NYI */
    } else if (!strchr (VALUE_LONGS, c) && !strchr (VALUE_SHORTS, c))
	return -1;			/* unknown type */
    l = strtoul (s, &sp, 0);
    if (*sp) return -1;
    value->v.l = l;
    return 0;
}

char *PrintedValue (v, len, buf)
  IN value_t *v;
  IN int len;
  OUT char *buf;
{
    char c = v->type;
    buf[len-1] = 0;
    if (strchr (VALUE_HYPERS, c)) {
	return PrintedHyper (v->v.h, len, buf);
    }
    if (strchr (VALUE_TIMES, c)) {
	char *time = ctime ((const time_t *)&v->v.t.tv_sec);
	time[19] = 0;
	time[24] = 0;
	sprintf (buf, "%s.%06d %s", time, v->v.t.tv_usec, time + 20);
    } else if (!strchr (VALUE_LONGS, c) && !strchr (VALUE_SHORTS, c))
	return "XXX";			/* unknown type */
    else sprintf (buf, "%d", v->v.l);
    afsl_Assert(buf[len-1] == 0);
    return buf;
}

/* LookupField -- finds a field given a one letter indicator.  The field is
 *     either set from or fetched to the address given in valueP.  The return
 *     value is positive if the value was inconsistent, and negative if the
 *     usage was erroneous.  The output flags parameter provides the
 *     appropriate mask bit for calling epif_SetStatus.  The vflags output
 *     parameter is the corresponding mask for epiv_SetStatus. */

long LookupField (c, op, info, status, vstatus, value, flagsP, vflagsP)
  IN char c;
  IN int op;				/* 0=>get, 1=>set */
  INOUT struct epia_info *info;
  INOUT struct epif_status *status;
  INOUT struct epiv_status *vstatus;
  INOUT value_t *value;
  OUT long *flagsP;
  OUT long *vflagsP;
{
    long flags, vflags;

    if (op == 0) {
	bzero ((char *)value, sizeof (*value));
	value->type = c;
    }
    if (flagsP) *flagsP = 0;
    if (vflagsP) *vflagsP = 0;
    vflags = flags = 0;

    if (strchr(VALUE_HYPERS, c)) {
	afs_hyper_t *addr = 0;
	switch (c) {
	  case 'l':
	    if (!info) return -1;
	    addr = &info->length;
	    break;
	  case 'v':
	    if (!status) return -1;
	    addr = &status->volumeVersionNumber;
	    break;
	  case 'V':
	    if (!vstatus) return -1;
	    addr = &vstatus->version;
	    vflags = EPIV_STATUS_VERSION;
	    break;
	  default:
	    afsl_PAssert (0, ("unknown afs_hyper_t type '%c'", c));
	}
	if (op) {
	    *addr = value->v.h;
	} else {
	    value->v.h = *addr;
	}
    } else if (strchr (VALUE_TIMES, c)) {
	struct timeval *addr = 0;
	if (!status) return -1;
	switch (c) {
	  case 'm':
	    flags = EPIF_SFLAGS_MTIME;
	    addr = &status->mTime;
	    break;
	  case 'a':
	    flags = EPIF_SFLAGS_ATIME;
	    addr = &status->aTime;
	    break;
	  case 'c':
	    flags = EPIF_SFLAGS_CTIME;
	    addr = &status->cTime;
	    break;
	  default:
	    afsl_PAssert (0, ("unknown timeval type '%c'", c));
	}
	if (op) *addr = value->v.t;
	else value->v.t = *addr;
    } else if (strchr (VALUE_LONGS, c)) {
	long *addr = 0;
	switch (c) {
	  case 'z':			/* siZe */
	    if (!info) return -1;
	    addr = (long *)&info->allocated;
	    break;
	  case 'Z':			/* siZe */
	    if (!info) return -1;
	    addr = (long *)&info->visible;
	    break;
	  case 'd':
	    if (!status) return -1;
	    addr = (long *)&status->dataVersionNumber;
	    break;
	  case 'A':			/* acl length */
	    if (!status) return -1;
	    addr = (long *)&status->acl;
	    break;
	  case 'D':			/* initial dir acl length */
	    if (!status) return -1;
	    addr = (long *)&status->initialDirAcl;
	    break;
	  case 'F':			/* initial file acl length */
	    if (!status) return -1;
	    addr = (long *)&status->initialFileAcl;
	    break;
	  case 'P':			/* plist length */
	    if (!status) return -1;
	    addr = (long *)&status->plist;
	    break;
	  case 'U':			/* user plist length */
	    if (!status) return -1;
	    addr = (long *)&status->reservedForUserPlist;
	    break;
	  case 'u':			/* volume Uniquifier */
	    if (!vstatus) return -1;
	    addr = (long *)&vstatus->unique;
	    vflags = EPIV_STATUS_UNIQUE;
	    break;
	  case 'q':			/* volume allocated Quota limit */
	    if (!vstatus) return -1;
	    addr = (long *)&vstatus->allocated.limit;
	    vflags = EPIV_STATUS_Q_ALLOCATED;
	    break;
	  case 'g':			/* volume allocated quota usaGe */
	    if (!vstatus) return -1;
	    addr = (long *)&vstatus->allocated.usage;
	    /* can't set quota usage */
	    break;
	  case 'Q':			/* volume visible Quota limit */
	    if (!vstatus) return -1;
	    addr = (long *)&vstatus->visible.limit;
	    vflags = EPIV_STATUS_Q_VISIBLE;
	    break;
	  case 'G':			/* volume visible quota usaGe */
	    if (!vstatus) return -1;
	    addr = (long *)&vstatus->visible.usage;
	    /* can't set quota usage */
	    break;
	  default:
	    afsl_PAssert (0, ("unknown long type '%c'", c));
	}
	if (op) *addr = value->v.l;
	else value->v.l = *addr;
    } else if (strchr (VALUE_SHORTS, c)) {
	u_short *addr = 0;
	switch (c) {
	  case 'h':			/* Hard link count */
	    if (!status) return -1;
	    flags = EPIF_SFLAGS_LINK;
	    addr = (u_short *)&status->linkCount;
	    break;
	  case 'p':			/* Protection bits */
	    if (!status) return -1;
	    flags = EPIF_SFLAGS_MODE;
	    addr = (u_short *)&status->mode;
	    break;
	  case 'o':
	    if (!info) return -1;
	    addr = (u_short *)&info->copies;
	    break;
	  default:
	    afsl_PAssert (0, ("unknown short type '%c'", c));
	}
	if (op) *addr = value->v.l;
	else value->v.l = *addr;
    } else {
	fprintf (stderr, "unknown field indicator '%c'\n", c);
	return -1;
    }
    if (flagsP) *flagsP = flags;
    if (vflagsP) *vflagsP = vflags;
    return 0;
}

/* GetAuxFile -- works like the open_file command except the result is packaged
 *     up and returned in a struct file.  It is different from GetCurrentFile
 *     in that it takes a name and looks it up the current processes' name
 *     table.  It also interacts appropriately with the CHECK_CODE mechanism.
 *
 * PARAMETERS -- It silently does nothing if passes a null string or a name
 *     equal to "". */

static long GetAuxFile(
  IN struct processData *p,
  IN struct file *context,
  IN char *name,
  OUT struct file *file,
  IN char *msg)
{
    long code;
    int nameLen;
    struct fileName *f;

    bzero ((char *)file, sizeof (*file));
    nameLen = strlen(name);
    afsl_Assert(nameLen < sizeof (f->name));
    if (nameLen && (strcmp (name, "\"\"") != 0)) {
	f = LookupFile (p, context, name);
	if (!f) {
	    code = EPI_E_NOENT;
not_found:
	    if (code == p->errorCode) return code;
	    com_err (p->whereami, code,
		     "getting file %s containing %s", name, msg);
	    return code;
	}
	file->fid = f->fid;
	file->volId = f->volId;
	file->devi = f->devi;
	code = GetCurrentFile (p, file);
	if (code) goto not_found;
    }
    return 0;
}

long InitProcessData (oldp, p)
  IN struct processData *oldp;
  OUT struct processData *p;
{
    long code;

    if (oldp) {
	*p = *oldp;
	code = CopyFileNameHT (oldp, p);
	if (code) return code;
	bzero ((char *)&p->cur, sizeof (p->cur));
	p->cur.devi = oldp->cur.devi;
	p->cur.volId = oldp->cur.volId;
	p->cur.fid = oldp->cur.fid;
	oldp->nChildren++;
    } else {
	bzero ((char *)p, sizeof (*p));
	code = dfsh_HashInit (&p->fileNameHT, FileNameHash,
			      offsetof (struct fileName, next));
	if (code) return code;
	code = dfsh_HashInitKeyString
	    (&p->fileNameHT, offsetof (struct fileName, name[0]));
	if (code) return code;
	p->buflen = 1234;
	epid_MakeInvalid (&p->cur.fid);
    }
    p->nChildren = 0;
    p->parent = oldp;

    p->databuf = osi_Alloc (p->buflen);
    p->cmpbuf = osi_Alloc (p->buflen);
    return 0;
}

/* MappedCodeOK -- returns a boolean; true if an expected code was returned and
 *     we should return it immediately.  False if no error or unexpected error.
 *     Since the VM sustem must return standard errno codes via the struct
 *     bufs, we map these back to Episode codes for comparison purposes. */

static int MappedCodeOK(
  IN struct processData *p,
  INOUT long *codeP)
{
    if (!*codeP) return 0;
    if (*codeP == ENOSPC) *codeP = EPI_E_AGGREGATEFULL;
#ifndef	AFS_SUNOS5_ENV
    /* No EDQUOT on SunOS 5 */
    else if (*codeP == EDQUOT) *codeP = EPI_E_VOLUMEFULL;
#endif
    if (*codeP == p->errorCode) return 1;
    return 0;
}

#define IsOp(a) (strncmp(o->op, (a), strlen(o->op)) == 0)
#define NO_ARGS \
    if (o->line[0] != 0) { \
        com_err (p->whereami, 0, "command doesn't take any arguments"); \
	return -1; }
#define CHECK_CODE if (MappedCodeOK (p, &code)) return code

static long OpenDevice(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    u_long nBlocks;
    u_long bsize;
    u_long fsize;
    char devName[500];
    dev_t dev;
    osi_off_t size;

    static char *tmpDevPrefix;
    static char *userName;
    static struct {
	char *suffix;
	int   set;
    } minor[US_MAX_MINOR_DEV + 1] =
	{{"one", 0}, {"two", 0}, {"three", 0}, {"four", 0}};
    extern char *getenv();

    if (!userName) {
	userName = getenv("USER");
	afsl_PAssert (userName, ("No 'USER' environment variable"));
	tmpDevPrefix = getenv("TEST_ANODE_TMPDEV");
    }

    if (code = ClosePreviousAggr (p, &p->cur)) return code;

    bsize = 0;
    fsize = 0;
    nBlocks = 0;
    devName[0] = '\0';
    sscanf_r (count, (o->line, "%d %lu %lu %lu %s", &p->cur.devi,
		      &bsize, &fsize, &nBlocks, devName));
    if (count < 1) {
od_usage:
	com_err (p->whereami, 0,
		 "Usage error: %s <minor dev number> [<block size> [<fragment \
size> [<n blocks> [<dev name>]]]]", o->op);
	return -1;
    }
    if ((p->cur.devi == -1) && (count == 1))
	return 0;			/* short hand for "close device" */
    if ((p->cur.devi < 0) || (p->cur.devi > US_MAX_MINOR_DEV)) {
	com_err (p->whereami, 0,
		 "minor device number %d out of range", p->cur.devi);
	return -1;
    }

    dev = osi_makedev(US_MAJOR_DEV, p->cur.devi);
    if (!minor[p->cur.devi].set) {
	if (count > 4) {
	    /* using specified path */ ;
	} else if (tmpDevPrefix) {
	    /* build name from specified prefix */
	    sprintf (devName,
		     "%s-%s", tmpDevPrefix, minor[p->cur.devi].suffix);
	} else {
	    /* build name from user name */
	    sprintf (devName,
		     "/tmp/dev-%s-%s", userName, minor[p->cur.devi].suffix);
	}
	if (devName[0] == '/') {
	    code = us_ioctl (dev, EPIU_IOCTL_SET_DIR, "/");
	    if (code) {
		com_err (p->whereami, code, "calling us_ioctl to set path");
		return code;
	    }
	}
	code = us_ioctl(dev, EPIU_IOCTL_SET_FILE, &(devName[1]));
	if (code != 0) {
	    com_err (o->whereami, code,
		     "calling us_ioctl to set file to ``%s''", &(devName[1]));
	    return code;
	}
	minor[p->cur.devi].set = 1;
    } else if (count > 4) {
	com_err (p->whereami, 0, "device name already set");
	return -1;
    }

    code = us_open (dev, US_OFLAGS_ASYNC/*simulate async I/O completes*/);
    if (code) {
	com_err (o->whereami, code, "calling us_open");
	return code;
    }
    size = ((osi_off_t) nBlocks) * ((osi_off_t) bsize);
    if (nBlocks || bsize) {
	if (size == 0) {
	    com_err (o->whereami, 0, "Must specify both nBlocks and bsize");
	    goto od_usage;
	}
	code = us_ioctl (dev, EPIU_IOCTL_SET_MAX_OFFSET, (caddr_t)&size);
	if (code) {
	    com_err (p->whereami, code, "calling us_ioctl to size");
	    return code;
	}
	partition[p->cur.devi].nBlocks = nBlocks;
    }
    if (fsize) partition[p->cur.devi].fsize = fsize;
    if (bsize) partition[p->cur.devi].bsize = bsize;

    if (bsize && fsize) {		/* create new device */
	if (((osi_off_t) bsize * nBlocks) < (EPISODE_SUPERBLOCK_ADDR + bsize)) {
	    com_err (o->whereami, 0,
		     "Device too small for superblock at %lu: \
need to have %lu bytes on device, or %lu %lu-byte blocks.",
		     EPISODE_SUPERBLOCK_ADDR, EPISODE_SUPERBLOCK_ADDR + bsize,
		     (EPISODE_SUPERBLOCK_ADDR+bsize)/bsize, bsize);
	    goto od_usage;
	}
	if (code = GetCurrentDevice(p, &p->cur)) return code;
    }
    if (useVM) {
	/* this resets the page size if the new block size is too small. */
	code = GetCurrentDevice (p, &p->cur); /* ensure device is opened */
	if (code) return code;
	code = vm_SetSizes (o->whereami, p->cur.device->blkSize, 0, 0);
	afsl_MBZ (code);
    }
    return 0;
}

static long GrowDevice(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    u_long nBlocks;
    u_long bsize;
    osi_off_t size;
    dev_t dev;

    if (code = ClosePreviousAggr (p, &p->cur)) return code;

    dev = makedev(US_MAJOR_DEV, p->cur.devi);
    bsize = partition[p->cur.devi].bsize;
    sscanf_r (count, (o->line, "%d %lu", &p->cur.devi, &nBlocks));
    if (count < 2) {
	com_err (p->whereami, 0,
		 "Usage error: %s <minor dev number> <n blocks>", o->op);
	return -1;
    }

    size = ((osi_off_t) nBlocks) * ((osi_off_t) bsize);
    if (nBlocks < partition[p->cur.devi].nBlocks)
    {
	com_err (p->whereami, 0,
		 "%s: Cannot shrink aggregate: current = %d, proposed = %d",
		 o->op, partition[p->cur.devi].nBlocks, nBlocks);
	return -1;
    }
    code = us_ioctl (dev, EPIU_IOCTL_SET_MAX_OFFSET, (caddr_t)&size);
    if (code) {
	com_err (p->whereami, code, "calling us_ioctl to size");
	return code;
    }
    partition[p->cur.devi].nBlocks = nBlocks;
    if (code = GetCurrentDevice(p, &p->cur)) return code;

    return(0);
}

static long CheckDevice(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    struct epiz_salvageState ss;
    int somePaths;			/* true if damaged anodes */
    int exitCode;

    NO_ARGS;
    if (code = GetCurrentAggr(p, &p->cur)) return code;
    bzero((char *)&ss, sizeof (ss) );
    if (TestPrintStatus()) ss.flags |= EPIZ_SS_VERBOSE;
    ss.flags |= EPIZ_SS_NODIRSCAN;
    code = epig_VerifyExistingAggregate (p->cur.device, &ss);
    epiz_DisplayAllNames (&ss);
    CHECK_CODE;
    if (code)
	/* code of -1 means that the err msg was already issued. */
	com_err (o->whereami, code, "verifying an aggregate");

    if (code) exitCode = EXIT_CRASH;
    else exitCode = epiz_GetExitCode (&ss);

    if (exitCode) {
	fprintf (stderr, "Verify failed at %s: %s\n",
		 o->whereami, epiz_GetExitMessage(exitCode));
    }
    return exitCode;
}

static long SalvageDevice(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    struct epiz_salvageState ss;
    NO_ARGS;
    if (code = GetCurrentAggr(p, &p->cur)) return code;
    bzero((char *)&ss, sizeof (ss) );
    if (TestPrintStatus()) ss.flags |= EPIZ_SS_VERBOSE;
    ss.flags |= EPIZ_SS_SALVAGE;
    ss.flags |= EPIZ_SS_NODIRSCAN;
    code = epig_VerifyExistingAggregate (p->cur.device, &ss);
    if (code && (code != -1)) {
	/* code of -1 means that the err msg was already issued. */
	com_err (o->whereami, code, "salvaging an aggregate");
    }
    return code;
}

static long BashDevice(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int bash = IsOp("bash_device");
    struct us_ioctl_rw rw;
    static char buf[1000];		/* we can't do this in MP mode... */
    dev_t dev;
    NO_ARGS;

    code = GetCurrentDevice(p, &p->cur);
    if (code) {
	com_err (o->whereami, code, "calling async_InitDevice");
	return code;
    }
    dev = makedev(US_MAJOR_DEV, p->cur.devi);
    rw.block = async_ToBufferBlocks
	(p->cur.device, EPISODE_SUPERBLOCK_ADDR / (p->cur.device->blkSize));
    rw.offset = 0;
    rw.length = sizeof (buf);
    rw.buffer = (caddr_t) buf;
    if (bash) { /* save old value */
	char ranBuf[sizeof (buf)];
	code = us_ioctl (dev, EPIU_IOCTL_PEEK, (caddr_t)&rw);
	if (code) {
	    com_err (o->whereami, code, "saving old superblock");
	    return code;
	}
	InitRandomValues (p, p->cur.volId, 0, 0);
	GetRandomValues (p, sizeof (ranBuf), ranBuf);
	rw.buffer = ranBuf;
    }
    code = us_ioctl (dev, EPIU_IOCTL_POKE, (caddr_t)&rw);
    if (code) {
	com_err (o->whereami, code, "bashing superblock");
	return code;
    }
    return 0;
}

static long CreateAggregate(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    u_long logsize;
    u_int nExceptions;
    daddr_t *excepts = 0;
    u_int exceptSize;
    u_long nBlocks;
    u_long firstBlock;
    u_long superBlock;
    extern int epig_creatingProgram;

    if (code = ClosePreviousAggr (p, &p->cur)) return code;

    sscanf_r (count, (o->line, "%lu %u", &logsize, &nExceptions));
    if (count < 1)
	logsize = 0;
    if (count < 2)
	nExceptions = 0;

    nBlocks = partition[p->cur.devi].nBlocks;
    if (!nBlocks) {
	com_err (o->whereami, 0, "No block size specified for device");
	return -1;
    }
    superBlock = EPISODE_SUPERBLOCK_ADDR / partition[p->cur.devi].bsize;
    firstBlock = MIN(FIRST_BLOCK, superBlock);
    nBlocks -= firstBlock;

    if (nExceptions > 0) {
	exceptSize = nExceptions * sizeof (*excepts);
	excepts = (daddr_t *)osi_Alloc (exceptSize);
	excepts[0] = nBlocks/2+firstBlock;
	/* try some out of bounds conditions */
	if (nExceptions > 1) excepts[1] = (u_long)-1;
	if (nExceptions > 2) excepts[2] = nBlocks/10+firstBlock;
	if (nExceptions > 3) excepts[3] = nBlocks+firstBlock;
	if (nExceptions > 4) excepts[4] = 2*nBlocks/3+firstBlock;
	if (nExceptions > 5) excepts[5] = nBlocks/3+firstBlock;
	if (nExceptions > 6) excepts[6] = firstBlock-1;
	if (nExceptions > 7) nExceptions = 7;
    }

    epig_creatingProgram = EPIG_CREATOR_TEST_ANODE; /* no real dirs */
    if (code = GetCurrentDevice(p, &p->cur)) return code;
    if (!logsize)
	logsize = elbb_ComputeMinLogSize (p->cur.device, /*minload*/1);
    code = epig_CreateAggregate
	(p->cur.device, partition[p->cur.devi].fsize,
	 firstBlock, superBlock, nBlocks, logsize, nExceptions, excepts);
    if (excepts) osi_Free ((opaque)excepts, exceptSize);
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code, "creating new aggregate, log size is %d",
		 logsize);
	return code;
    }
    if (code = GetCurrentAggr(p, &p->cur)) return code;
    return 0;
}

static long GrowAggregate(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    u_int nExceptions;
    daddr_t *excepts = 0;
    u_int exceptSize;
    u_long firstBlock;
    u_long nBlocks;

    if (code = GetCurrentAggr(p, &p->cur)) return code;

    sscanf_r (count, (o->line, "%lu %u", &nBlocks, &nExceptions));
    if (count < 1)
    {
	com_err (o->whereami, 0,
		 "Usage error: %s blocks [ exceptions ]", o->op);
	return -1;
    }
    if (count < 2)
	nExceptions = 0;

    if (nExceptions > 0) {
	firstBlock = 4;
	exceptSize = nExceptions * sizeof (*excepts);
	excepts = (daddr_t *)osi_Alloc (exceptSize);
	excepts[0] = nBlocks/2+firstBlock;
	/* try some out of bounds conditions */
	if (nExceptions > 1) excepts[1] = (u_long)-1;
	if (nExceptions > 2) excepts[6] = nBlocks/10+firstBlock;
	if (nExceptions > 3) excepts[3] = nBlocks+firstBlock;
	if (nExceptions > 4) excepts[4] = 2*nBlocks/3+firstBlock;
	if (nExceptions > 5) excepts[5] = nBlocks/3+firstBlock;
	if (nExceptions > 6) excepts[2] = firstBlock-1;
	if (nExceptions > 7) nExceptions = 7;
    }
    code = epig_GrowAggregate (p->cur.avl, nBlocks, nExceptions, excepts);
    if (excepts) osi_Free ((opaque)excepts, exceptSize);
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code, "extending aggregate, blocks = %d",
		 nBlocks);
	return code;
    }
    if (code = GetCurrentAggr(p, &p->cur)) return code;
    return 0;
}

static long CloseAggregate(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    NO_ARGS;
    return ClosePreviousAggr (p, &p->cur);
}

static long CreateVolume(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    int existOkay = IsOp("exist_volume");
    struct epiv_status status;
    epi_anode_t ap;
    u_long volIndex;
    u_long hi, lo;
    afs_hyper_t id;
    epia_quota_t quota;
    struct epif_status fileStatus;
    char volRootName[32];

    if (code = ClosePreviousVol (p, &p->cur)) return code;

    bzero((char *)&status, sizeof (status));
    sscanf_r (count, (o->line,
		      " = %s %lu,,%lu %lu %lu %s", volRootName, &hi, &lo,
		      &quota.a, &quota.v, status.statusMsg));
    if (count == 0) {
	volRootName[0] = 0;
	sscanf_r (count, (o->line, "%s %lu,,%lu %lu %lu %s",
			  status.ident.name, &hi, &lo, &quota.a, &quota.v,
			  status.statusMsg));
    }
    if ((count > 6) || (count == 2)) {
	com_err (o->whereami, 0,
		 "Usage error: %s [<name> [hi,,lo [<q_alloc> [<q_visible> [<status msg>]]]]]",
		 o->op);
	return -1;
    }
    if (count < 4) quota.a = 100;
    if (count < 5) quota.v = quota.a;
    if (count < 6)
	strncpy (status.statusMsg, o->line, sizeof (status.statusMsg));
    else if (strcmp (status.statusMsg, "\"\"") == 0)
	/* we need this for testing OT 6376 */
	bzero (status.statusMsg, sizeof (status.statusMsg));
    if (count < 2) AFS_hzero(id);
    else AFS_hset64(id, hi, lo);

    if (status.ident.name[0] == 0)
	sprintf (status.ident.name, "V%u", NextRandom(p));
    if (volRootName[0] == 0) {
	if (strlen (status.ident.name) +5 >= sizeof (volRootName)) {
	    com_err (o->whereami, 0,
		     "volume name (%s) too long to append '.root'",
		     status.ident.name);
	    return -1;
	}
	sprintf (volRootName, "%s.root", status.ident.name);
    }
    if (AFS_hiszero(id)) {			/* find unused id */
	while (code == 0) {
	    AFS_hset64 (id, 0, NextRandom(p));
	    code = vr_LookupVolumeId (id, 0, &volIndex);
	}
	if (code != EPI_E_NOENT) {
	    com_err (o->whereami, code, "looking up volume %s",
		     PrintedHyper(id, sizeof (p->buf), p->buf));
	    return code;
	}
    }

    if (code = GetCurrentAggr(p, &p->cur)) return code;

    status.allocated.limit = quota.a;
    status.visible.limit = quota.v;
    status.ident.id = id;
    WITH_TRANS ("create_volume", &p->cur,
		code = epig_CreateVolume (trans, p->cur.avl, 0,
					  &status, &volIndex));
    CHECK_CODE;
    if ((code == EPI_E_VOLIDEXISTS) && existOkay) {
	if (TestPrintStatus())
	    printf ("Volume %s now exists\n",
		    PrintedHyper(status.ident.id, sizeof (p->buf), p->buf));
    } else if (code) {
	com_err (o->whereami, code, "creating a volume %s with id %s",
		 status.ident.name,
		 PrintedHyper(status.ident.id, sizeof (p->buf), p->buf));
	return code;
    } else if (TestPrintStatus())
	printf ("Created volume %s with id %s\n", status.ident.name,
		PrintedHyper(status.ident.id, sizeof (p->buf), p->buf));

    p->cur.volId = status.ident.id;

    if (code = GetCurrentVol(p, &p->cur)) return code;

    bzero((char *)&fileStatus, sizeof (fileStatus));
    WITH_TRANS ("create_volroot", &p->cur,
		code = epif_Create (trans, p->cur.vol,
				    EPIF_CFLAGS_VOLUMEROOT,
				    /*estLength*/0, &fileStatus,
				    /*acl,plist*/0, 0,
				    &p->cur.fid, &ap));
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code, "creating root of volume");
	return code;
    }
    afsl_Assert(epid_IsVolumeRoot (&p->cur.fid));
    code = epif_Close (ap);
    afsl_MBZ (code);
    {   struct fileName *f =
	    (struct fileName *) osi_Alloc (sizeof (struct fileName));
	f->fid = p->cur.fid;
	f->volId = p->cur.volId;
	f->devi = p->cur.devi;
	strcpy (f->name, volRootName);
	code = dfsh_HashIn_r (&p->fileNameHT, f);
	if (code) {
	    com_err (o->whereami, code,
		     "adding file %s to hash table", f->name);
	    return code;
	}
    }
    if (TestPrintStatus())
	printf ("volume root is %s\n",
		PrintedFid (&p->cur.fid, sizeof (p->buf), p->buf));
    return 0;
}

/* dv_DeleteAFile --
 *
 * PARAMETERS -- We are passed a pointer to a file handle.  If we are
 *     successful at deleting the file we zero the handle so our caller knows
 *     not to.  Otherwise the caller closes it and we go on.
 *
 *     The fstat pointer may also be null if the caller hasn't obtained the
 *     file's status. */

static long dv_DeleteAFile(
  IN struct processData *p,
  IN epi_volFileId_t *fid,
  IN epi_anode_t *fP,                   /* pointer to file handle */
  IN struct epif_status *fstat)
{
    long code;
    struct epia_info info;

    /* The fstat arg will be filled in when called from iterator ForAllFiles */
    if (fstat && epid_IsVolumeRoot(fid))
	return 0;
    code = epia_GetInfo(*fP, &info);
    if (code) return code;
    if (info.copies) return 0;		/* delete everything else first */

    if (useVM) {
	/* is there any reason to write out the pages? */
	code = vm_Write(VM_INVALIDATE, *fP);
	afsl_MBZ (code);
    }

    code = epif_Delete(*fP, EPIF_DELETE_IGNORE_LINKCNT, NULL);
    afsl_MBZ(code);

    *fP = 0;				/* closed by delete */
    p->cur.fid = *fid;
    RemoveFile (p, &p->cur);
    return 0;
}

static long ForAllFiles(
  IN epi_volume_t v,
  IN long (*proc)(),			/* call for each entry */
  IN opaque rock)			/* pass through parameter */
{
    long code;
    u_long index;
    u_long lastIndex;
    int someReturned;			/* some file not handled */
    int someDeleted;			/* some file was deleted */

    do {
	code = epiv_LastIndex (v, &lastIndex);
	if (code) return code;
	lastIndex = epid_LastIndex(lastIndex, epia_AnodeDevice(v));
	someReturned = someDeleted = 0;
	for (index=1; index<=lastIndex; index++) {
	    epi_volFileId_t fid;
	    struct epif_status fstat;
	    epid_MakeIndexOnly (&fid, index);
	    BEGIN_LCRIT (epi_anode_t f = 0,
			 code = epif_Open (v, &fid, &f),
			 if (f) code = epif_Close (f)) {
		code = epif_GetStatus (f, &fstat);
		if (code) continue;
		epid_Assemble (&fid, index, fstat.unique);
		code = (*proc) (rock, &fid, &f, &fstat);
		if (code) continue;
		if (f) someReturned++;
		else someDeleted++;
	    } END_CRIT;
	    if (code == EPI_E_NOENT) continue;
	    if (code) return code;
	}
    } while (someReturned && someDeleted);
    if (someReturned > 1)
	return EPI_E_NOTEMPTY;		/* could delete all anodes */
    return 0;
}

static long DeleteVolume(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    char options[32];
    int force;

    if (code = ClosePreviousFile (p, &p->cur)) return code;

    if (o->line[0] == 0) {
	/* no args so delete current volume */
    } else {
	u_long hi, lo;

	options[0] = 0;
	sscanf_r (count, (o->line, "%lu,,%lu %31s", &hi, &lo, options));
	if (count >= 2) {
	    if (code = ClosePreviousVol (p, &p->cur)) return code;
	    AFS_hset64 (p->cur.volId, hi, lo);
	    if (AFS_hiszero(p->cur.volId)) {
  usage:
		com_err (o->whereami, 0,
			 "Usage error: %s [{hi,,lo|<filename>}] [-force]",
			 o->op);
		return -1;
	    }
	} else {
	    char name[32];
	    struct fileName *found;

	    sscanf_r (count, (o->line, "%31s %31s", name, options));
	    if (count < 1) goto usage;
	    found = LookupFile (p, &p->cur, name);
	    if (found == 0) {
		com_err (o->whereami, 0, "No such file found %s", name);
		return -1;
	    }
	    if (found->devi != p->cur.devi) {
		if (code = ClosePreviousAggr (p, &p->cur)) return code;
	    } else if (!AFS_hsame(found->volId, p->cur.volId)) {
		if (code = ClosePreviousVol (p, &p->cur)) return code;
	    }
	    p->cur.devi = found->devi;
	    p->cur.volId = found->volId;
	    p->cur.fid = found->fid;
	}
    }
    if (strcmp(options, "-force") == 0)
	force = 1;			/* don't empty volume first */
    else force = 0;

    if (code = GetCurrentVol (p, &p->cur)) return code;

    if (!force) {
	code = ForAllFiles (p->cur.vol, dv_DeleteAFile, p);
	if (code) {
	    com_err (o->whereami, code,
		     "Failed to delete contents of volume id %s",
		     PrintedHyper(p->cur.volId, sizeof (p->buf), p->buf));
	    return code;
	}

	/* now get the root */
	epid_MakeVolumeRoot (&p->cur.fid);
	{   long tmp = p->errorCode;
	    p->errorCode = EPI_E_NOENT;
	    code = GetCurrentFile(p, &p->cur);
	    p->errorCode = tmp;
	    if (code == 0) {
		code = dv_DeleteAFile (p, &p->cur.fid, &p->cur.file, 0);
		if (code) {
		    com_err (o->whereami, code,
			     "Failed to delete root of volume id %s",
			     PrintedHyper(p->cur.volId,sizeof(p->buf),p->buf));
		    return code;
		}
	    } else if (code != EPI_E_NOENT) {
		com_err (o->whereami, code,
			 "Failed to find root of volume id %s",
			 PrintedHyper(p->cur.volId, sizeof (p->buf), p->buf));
		return code;
	    }
	}
    }

    code = epiv_Deflate (p->cur.vol);
    if (code) {
	com_err (o->whereami, code, "calling epit_Deflate");
	return code;
    }
    WITH_TRANS ("delete_volume", &p->cur,
		code = epig_DeleteVolume (trans, p->cur.vol));
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code, "calling epiv_Delete");
	return code;
    }
    AFS_hzero (p->cur.volId);
    p->cur.vol = 0;
    return 0;
}

static long OpenVolume(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    u_long hi, lo;

    if (code = ClosePreviousVol (p, &p->cur)) return code;

    sscanf_r (count, (o->line, "%lu,,%lu", &hi, &lo));
    if (count != 2) {
ov_usage:
	com_err (o->whereami, 0, "Usage error: %s [hi,,lo]", o->op);
	return -1;
    }
    AFS_hset64 (p->cur.volId, hi, lo);
    if (AFS_hiszero(p->cur.volId)) goto ov_usage;

    code = GetCurrentVol(p, &p->cur);
    if (code) return code;
    return 0;
}

static long GarbageCollectVolume(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    struct file file;
    u_int newLink;			/* link count of file being deleted */
    u_long iter;			/* iteration cookie */
    u_long newIter;			/* next iteration cookie */
    int nZero;				/* count of zero link count files */
    int nGCed;				/* count of files deleted */

    NO_ARGS;

    if (code = ClosePreviousFile (p, &p->cur)) return code;
    if (code = GetCurrentVol (p, &p->cur)) return code;

    bzero((char *)&file, sizeof (file));
    nZero = 0;
    for (iter = 0; iter != 0xffffffff; iter=newIter) {
	code = epif_NextDeletedFile (p->cur.vol, iter, &newIter, &file.fid);
	if (code == EPI_E_ITERATIONDONE) break;
	if (code) {
	    com_err (o->whereami, code,
		     "Failed to get next deleted file (iter = %d)", iter);
	    return code;
	}
	nZero++;
    }

    nGCed = 0;
    for (iter = 0; iter != 0xffffffff; iter = newIter) {
	code = epif_NextDeletedFile (p->cur.vol, iter, &newIter, &file.fid);
	if (code == EPI_E_ITERATIONDONE) break;
	if (code) {
	    com_err (o->whereami, code,
		     "Failed to get next deleted file (iter = %d)", iter);
	    return code;
	}
	code = epif_Open (p->cur.vol, &file.fid, &file.file);
	if (code) {
	    com_err (o->whereami, code,
		     "Failed to open deleted file (fid = %s)",
		     PrintedFid (&file.fid, sizeof (p->buf), p->buf));
	    return code;
	}

	if (useVM) {
	    /* is there any reason to write out the pages? */
	    code = vm_Write (VM_INVALIDATE, file.file);
	    afsl_MBZ (code);
	}
	code = epif_Delete(file.file, 0 /* flags */, &newLink);
	if (code) {
	    long code2;
	    code2 = epif_Close(file.file);
	    afsl_MBZ(code2);
	    if (code == EPI_E_HASLINKS) {
		com_err (o->whereami, 0,
			 "Failed to delete file (fid = %s) with linkcount=%d",
			 PrintedFid(&file.fid, sizeof (p->buf), p->buf), 
			 newLink);
		code = -1;
	    } else {
		com_err (o->whereami, code,
			 "Failed to delete deleted file (fid = %s)",
			 PrintedFid(&file.fid, sizeof (p->buf), p->buf));
	    }
	    return code;
	}
	if (TestPrintStatus())
	    printf ("garbage collected %s\n",
		    PrintedFid(&file.fid, sizeof (p->buf), p->buf));
	RemoveFile (p, &file);
	nGCed++;
    }
    if (nZero != nGCed) {
	printf ("Found %d files w/ zero link count but actually deleted %d\n",
		nZero, nGCed);
    }
    return 0;
}

static long VolumeInfo(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    char s[10][16];
    int i;
    struct epiv_status status;

    sscanf_r (count, (o->line, "%s %s %s %s %s %s %s %s %s %s",
		      s[0], s[1], s[2], s[3], s[4],
		      s[5], s[6], s[7], s[8], s[9]));

    if (code = GetCurrentVol (p, &p->cur)) return code;

    code = epiv_GetStatus (p->cur.vol, &status);
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code, "calling epiv_GetStatus from vol_info");
	return code;
    }
    for (i=0; i<count; i++) {
	char c = s[i][0];		/* field indicator */
	char o = s[i][1];		/* operation */
	int cmp;
	value_t testValue;
	value_t actual;

	if ((o == '=') || (o == '!')) {
	    code = ScanValue (c, s[i]+2, &testValue);
	    if (code) goto vi_usage;
	    code = LookupField (c, 0, 0,0, &status, &actual, 0,0);
	    if (code) goto vi_usage;
	    cmp = bcmp((char *)&actual,(char *)&testValue, sizeof (actual));
	    if ((o == '=') != (cmp == 0)) {
		com_err (p->whereami, 0,
			 "test %s fails, actual was %s", s[i],
			 PrintedValue (&actual, sizeof (p->buf), p->buf));
		code = -1;
		break;
	    }
	} else if ((c == '!') || (c == '=')) {
	    char tmp = c;
	    c = o;
	    o = tmp;
	    code = LookupField (c, 0, 0,0, &p->oldVolStatus, &testValue, 0,0);
	    if (code) goto vi_usage;
	    code = LookupField (c, 0, 0,0, &status, &actual, 0,0);
	    if (code) goto vi_usage;
	    cmp = bcmp((char *)&actual, (char *)&testValue, sizeof (actual));
	    if ((o == '=') != (cmp == 0)) {
		fprintf (stderr, "%s: test %s fails, ", p->whereami, s[i]);
		if (cmp == 0)
		    fprintf (stderr, "both are %s\n",
			     PrintedValue (&actual, sizeof (p->buf), p->buf));
		else {
		    fprintf (stderr, "actual is %s, ",
			     PrintedValue(&actual, sizeof (p->buf), p->buf));
		    fprintf (stderr, "old value was %s\n",
			     PrintedValue (&testValue,
					   sizeof (p->buf), p->buf));
		}
		code = -1;
		break;
	    }
	} else {
	  vi_usage:
	    com_err (p->whereami, 0,
		     "Illegal format in test string %s", s[i]);
	    return -1;
	}
    }					/* for each test */
    if (code || (count < 1) || (TestPrintStatus())) {
	printf ("For vol %s (#%s): ",
		status.ident.name, PrintedHyper(status.ident.id,
						sizeof (p->buf), p->buf));
	printf ("VV %s, unique %d,\n",
		PrintedHyper (status.version, sizeof (p->buf), p->buf),
		status.unique);
	printf ("  alloc/vis quota used %d(%.1f%% of %d) / %d(%.1f%% of %d)\n",
		status.allocated.usage,
		100.0*status.allocated.usage/status.allocated.limit,
		status.allocated.limit,
		status.visible.usage,
		100.0*status.visible.usage/status.visible.limit,
		status.visible.limit);
    }
    if (code) return code;

    /* save old values */
    p->oldVolStatus = status;
    return 0;
}

static long AggregateInfo(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    char s[10][16];
    int i;
    struct epig_status status;

    sscanf_r (count, (o->line, "%s %s %s %s %s %s %s %s %s %s",
		      s[0], s[1], s[2], s[3], s[4],
		      s[5], s[6], s[7], s[8], s[9]));

    if (code = GetCurrentAggr (p, &p->cur)) return code;

    code = epig_GetAggregateStatus (p->cur.avl, &status);
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code,
		 "calling epig_GetAggregateStatus from aggr_info");
	return code;
    }
    for (i=0; i<count; i++) {
	char c = s[i][0];		/* field indicator */
	char o = s[i][1];		/* operation */
	int cmp;
	value_t testValue;
	value_t actual;

	if ((o == '=') || (o == '!')) {
	    if (c != 'f') goto usage;
	    code = ScanValue (c, s[i]+2, &testValue);
	    bzero((char *)&actual, sizeof (actual));
	    actual.type = 'f';
	    actual.v.l = status.fragmentsFreeReally;
	    cmp = bcmp((char *)&actual, (char *)&testValue, sizeof (actual));
	    if ((o == '=') != (cmp == 0)) {
		com_err (p->whereami, 0,
			 "test %s fails, actual was %s", s[i],
			 PrintedValue (&actual, sizeof (p->buf), p->buf));
		code = -1;
		break;
	    }
	} else if ((c == '!') || (c == '=')) {
	    char tmp = c;
	    c = o;
	    o = tmp;
	    if (c != 'f') goto usage;
	    bzero ((char *)&testValue, sizeof (testValue));
	    testValue.type = 'f';
	    testValue.v.l = p->oldAggrStatus.fragmentsFree;
	    bzero ((char *)&actual, sizeof (actual));
	    actual.type = 'f';
	    actual.v.l = status.fragmentsFree;

	    cmp = bcmp((char *)&actual, (char *)&testValue, sizeof (actual));
	    if ((o == '=') != (cmp == 0)) {
		fprintf (stderr, "%s: test %s fails, ", p->whereami, s[i]);
		if (cmp == 0)
		    fprintf (stderr, "both are %s\n",
			     PrintedValue (&actual, sizeof (p->buf), p->buf));
		else {
		    fprintf (stderr, "actual is %s, ",
			     PrintedValue(&actual, sizeof (p->buf), p->buf));
		    fprintf (stderr, "old value was %s\n",
			     PrintedValue (&testValue,
					   sizeof (p->buf), p->buf));
		}
		code = -1;
		break;
	    }
	} else {
  usage:
	    com_err (p->whereami, 0,
		     "Illegal format in test string %s", s[i]);
	    return -1;
	}
    }					/* for each test */
    if (code || (count < 1) || (TestPrintStatus())) {
	printf ("For aggr on device %x/%x: \n",
		status.deviceMajor, status.deviceMinor);
	printf ("  %d fragments free.\n", status.fragmentsFree);
    }
    if (code) return code;

    /* save old values */
    p->oldAggrStatus = status;
    return 0;
}

static long CreateFile(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    int spliceFile;
    int byfid;
    int createFile;
    struct epif_status fileStatus;
    epi_anode_t ap;
    struct fileName fn;
    struct fileName *f;
    long flags;
    char aclFile[32], plistFile[32], copyFile[32];
    struct file acl, plist, copy;

    spliceFile = createFile = byfid = 0;
    if (IsOp("create_file")) createFile = 1;
    else if (IsOp("create_fid")) createFile = byfid = 1;
    else if (IsOp("splice_file")) spliceFile = createFile = 1;
    else if (IsOp("splice_fid")) spliceFile = createFile = byfid = 1;
    else /* create_dir */;

    if (code = ClosePreviousFile (p, &p->cur)) return code;

    aclFile[0] = plistFile[0] = copyFile[0] = 0;
    sscanf_r (count, (o->line, "%31s %31s %31s %31s",
		      fn.name, aclFile, plistFile, copyFile));
    if (count < 1) {
usage:
	com_err (o->whereami, 0,
		 "Usage error: %s <name|fid> [<acl name|fid|\"\">\
 [<plist name|fid|\"\"> [<copy name|fid>]]]", o->op);
	return (-1);
    }

    afsl_Assert(strlen (fn.name) < sizeof (fn.name));

    if (byfid) {

	/* bypass internal checks and call epif_Create with this fid... */

	afs_hyper_t volid;

	if (!NameAsFid (p, o->line, &p->cur.fid, &volid)) goto usage;
	if (!AFS_hiszero(volid)) {
	    if (code = ClosePreviousVol (p, &p->cur)) return code;
	    p->cur.volId = volid;
	}

    } else {
	f = LookupFile (p, &p->cur, fn.name); /* this checks fid syntax too */
	if (f) {
	    com_err (o->whereami, 0, "File %s already exists", fn.name);
	    return -1;
	}
    }

    if (code = GetCurrentVol(p, &p->cur)) return code;

    code = GetAuxFile (p, &p->cur, aclFile, &acl, "acl");
    if (code) return code;
    code = GetAuxFile (p, &p->cur, plistFile, &plist, "property list");
    if (code) return code;
    code = GetAuxFile (p, &p->cur, copyFile, &copy, "copy-on-write source");
    if (code) return code;

    flags = 0;
    if (!createFile)
	flags |= EPIF_CFLAGS_DIRECTORY;
    if (copy.file)
	flags |= (!spliceFile ? EPIA_CFLAGS_COPYONWRITE : EPIA_CFLAGS_BACKING);
    if (byfid)
	flags |= EPIF_CFLAGS_USEFID;

    bzero ((char *)&fileStatus, sizeof (fileStatus));
    WITH_TRANS ("create_file", &p->cur,
        {   code = epif_CreateE (trans, p->cur.vol, flags,
				 /*estLength*/0, &fileStatus,
				 acl.file, plist.file, copy.file,
				 &p->cur.fid, &ap);
	} );
    afsl_MBZ (ClosePreviousAggr (p, &acl));
    afsl_MBZ (ClosePreviousAggr (p, &plist));
    afsl_MBZ (ClosePreviousAggr (p, &copy));
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code, "creating an anode in a volume");
	return code;
    }
    code = epif_Close (ap);
    afsl_MBZ (code);
    f = (struct fileName *) osi_Alloc (sizeof (fn));
    f->fid = p->cur.fid;
    f->volId = p->cur.volId;
    f->devi = p->cur.devi;
    if (byfid) {
	sprintf (f->name, "%s[%s]",
		 PrintedFid(&f->fid, sizeof(p->buf), p->buf),
		 PrintedHyper (f->volId, sizeof (p->buf2), p->buf2));
    } else {
	strcpy (f->name, fn.name);
    }
    code = dfsh_HashIn_r (&p->fileNameHT, f);
    if (code) {
	com_err (o->whereami, code, "adding file %s to hash table", fn.name);
	return code;
    }
    if (TestPrintStatus())
	printf ("fid is %s\n",
		PrintedFid (&p->cur.fid, sizeof (p->buf), p->buf));
    return 0;
}

static long OpenFile(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    int byfid = IsOp("open_fid");
    char name[32];
    struct fileName *found;
    epi_anode_t file;			/* file being opened */

    if (code = ClosePreviousFile (p, &p->cur)) return code;

    if (byfid) {

	/* bypass internal checks and call epif_Open with this fid... */

	afs_hyper_t volid;

	if (!NameAsFid (p, o->line, &p->cur.fid, &volid)) goto of_usage;
	if (!AFS_hiszero(volid)) {
	    if (code = ClosePreviousVol (p, &p->cur)) return code;
	    p->cur.volId = volid;
	}

    } else {
	sscanf_r (count, (o->line, "%31s", name));
	if (count != 1) {
	  of_usage:
	    com_err (o->whereami, 0,
		     "Usage error: %s {(<n>,<n>)|<name>}", o->op);
	    return (-1);
	}
	found = LookupFile (p, &p->cur, name);
	if (found == 0) {
	    com_err (o->whereami, 0, "No such file found %s", name);
	    return -1;
	}
	if (found->devi != p->cur.devi) {
	    if (code = ClosePreviousAggr (p, &p->cur)) return code;
	} else if (!AFS_hsame(found->volId, p->cur.volId)) {
	    if (code = ClosePreviousVol (p, &p->cur)) return code;
	}
	p->cur.devi = found->devi;
	p->cur.volId = found->volId;
	p->cur.fid = found->fid;
    }

    if (code = GetCurrentVol(p, &p->cur)) return code;

    /* Just open it here for error checking purposes.  Don't supplant call to
     * GetCurrentFile since it does other initialization. */

    code = epif_Open (p->cur.vol, &p->cur.fid, &file);
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code,
		 "opening file %s",
		 PrintedFid(&p->cur.fid, sizeof (p->buf), p->buf));
	return code;
    }
    code = epif_Close (file);
    afsl_MBZ (code);
    return 0;
}

/* GetNBSValues -- fill data buffer NBS pattern.  Each word contains its word
 *     offset in the file. */

static void GetNBSValues (
  u_long offset,
  u_int len,
  char *buf)
{
    int i;
    u_int32 *array = (u_int32 *)buf;

    afsl_Assert (((len|offset) & 3) == 0);
    len >>= 2;				/* convert to words */
    offset >>= 2;			/*  ditto  */

    for (i=0; i<len; i++)
	*array++ = offset++;
}

static int IsZero (
  char *buf,
  int len)
{
    int i;
    while (len-- > 0)
	if (*buf++)
	    return 0;
    return 1;
}

static long WriteFile(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    int delayOp = IsOp("delayed_write_file");
    char offStr[20];
    u_long offset;
    u_long length;
    int useRandomValues;		/* 0=>no, 1=>random, 2=>NBS */
    char valStr[12];
    u_long value = 0;			/* value to check for */
    int i;
    int log;				/* is data logged: a dir */
    char *sp;				/* for calling strtoul */

    afsl_Assert(p->databuf);
    sscanf_r (count, (o->line, "%19s %lu %11s", offStr, &length, valStr));
    if (count < 2) {
usage:
	com_err (o->whereami, 0,
		 "Usage error: %s <offset|RANDOM:m> <length> [<value|NBS>]",
		 o->op);
	return (-1);
    }
    useRandomValues = 1;
    if (count == 3) {
	value = strtoul (valStr, &sp, 0);
	if (*sp != '\0') {
	    lcstring (valStr, valStr, sizeof (valStr));
	    if (strcmp(valStr, "nbs") == 0) {
		useRandomValues = 2;
		value = 0;
	    } else
		goto usage;
	} else if (value <= 0xff)
	    useRandomValues = 0;
    }

    if (code = GetCurrentFile (p, &p->cur)) return code;
    log = FileIsDir(&p->cur);

    offset = strtoul (offStr, &sp, 0);
    if (*sp != '\0') {
	lcstring (offStr, offStr, sizeof (offStr));
	if (strncmp(offStr, "random", 6) == 0) {
	    u_long maxLen;
	    if (offStr[6] == '\0') {
		maxLen = AFS_hgetlo(epia_GetLength (p->cur.file));
	    } else if (offStr[6] == ':') {
		maxLen = strtoul (offStr+7, &sp, 0);
		if (*sp != '\0')
		    goto usage;
	    } else
		goto usage;

	    offset = NextRandom(p) % howmany(maxLen, length);
	    offset *= length;
	} else
	    goto usage;
    }

    if (!useRandomValues) {
	if (value == 0)
	    bzero (p->databuf, MIN(p->buflen,length));
	else
	    for (i=0; i<MIN(p->buflen,length); i++)
		p->databuf[i] = (char)value;
    } else if (useRandomValues == 1) {
	if (value > 0xff) {
	    afs_hyper_t seed;
	    AFS_hset64 (seed, 0, value);
	    InitRandomValues (p, seed, 0, offset);
	}
	else InitRandomValues (p, p->cur.volId, &p->cur.fid, offset);
    } else /* NBS values */;

    BEGIN_CRIT (ObtainRVM(),
		ReleaseRVM()) {
	code = DoWriteFile (p, offset, length, useRandomValues);
	if ((!code || MappedCodeOK (p, &code)) && !delayOp)
	    PushVMWrites (p);
    } END_CRIT /* vm lock */;

#if 0
    if (!log && useVM && MappedCodeOK (p, &code) &&
	((code == EPI_E_AGGREGATEFULL) || (code == EPI_E_VOLUMEFULL))) {
	u_long len = p->buflen;

	/* We got an expected space error using VM.  To get precise data xfer
         * try iterating in smaller pieces. */

	code = 0;
	while (length > 0) {
	    if (length < len) len = length;
	    code = DoWriteFile (p, offset, len, useRandomValues);
	    if (code) break;
	    offset += len;
	    length -= len;
	}
    }
#endif
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code, "writing for file %s",
		 PrintedFid (&p->cur.fid, sizeof(p->buf), p->buf));
    }
    return code;
}

/* PushVMWrites -- We need to push the updates through so that subsequent
 *     GetInfo calls see the expected things.  On the otherhand we sometimes
 *     want to delay this to test reservation code, but intervening status info
 *     may be anomolous.  If delayed this is called from ClosePreviousFile.
 *
 * CAUTIONS -- We must not call this with a running transaction because a
 *     synchronous putpage may need to start a transaction, which would be
 *     nested otherwise. */

static void PushVMWrites(IN struct processData *p)
{
    int log = FileIsDir(&p->cur);	/* is data logged: a dir */
    long code;

    if (!log && useVM && p->cur.file) {
	code = vm_Write (VM_WRITE|VM_AWAIT, p->cur.file);
	afsl_MBZ(code);
    }
}

static long DoWriteFile(
  IN struct processData *p,
  IN u_long offset,
  IN u_long length,
  IN int useRandomValues)
{
    long code;
    int i;
    int log = FileIsDir(&p->cur);	/* is data logged: a dir */

    BEGIN_LCRIT (elbb_tranRec_t trans,
		 trans = elbb_NullTranId,
		 ((trans != elbb_NullTranId) &&
		  (code = epia_EndTran (p->cur.file, trans)))) {
	/* do this at least once, in case length==0 specified expicitly */

	if (!log && useVM) {
	    u_long bufLen, bufOff;
	    u_long cmpLen, cmpOff;
	    u_long len;
	    struct vm_buf b;		/* for vm_GetBuf and vm_Release */

	    bufLen = 0, cmpLen = 0;
	    while (length > 0) {
		if (cmpLen == 0) {	/* get the reference values */

		    /* Also reserve at this time because we must do it
		     * before grabbing a buffer (which dirties it). */

		    struct uio uio;
		    struct iovec iov;

		    cmpLen = MIN(length,p->buflen);
		    if (useRandomValues == 1)
			GetRandomValues (p, cmpLen, p->databuf);
		    else if (useRandomValues == 2)
			GetNBSValues (offset, cmpLen, p->databuf);
		    cmpOff = 0;

		    epia_InitUio (&uio, &iov, offset, cmpLen,
				  /*don't care*/p->databuf);

		}
		if (bufLen == 0) {	/* get the data */
		    code = vm_GetBuf (p->cur.file, VM_FORWRITE,
				    offset, length, &b);
		    afsl_PAssert (code == 0,
				  ("%s: vm_GetBuf failed", p->whereami));
		    bufOff = offset - b.base;
		    afsl_Assert (bufOff < b.length);
		    bufLen = b.length - bufOff;
		}
		len = MIN (cmpLen, bufLen);
		afsl_Assert (len <= length);

		bcopy(p->databuf + cmpOff, b.addr + bufOff, len);
		bufOff += len, cmpOff += len, offset += len;
		bufLen -= len, cmpLen -= len, length -= len;
		if (bufLen == 0)
		    vm_Release (&b);
	    }
	    if (bufLen > 0)
		vm_Release (&b);
	} else
	    do {
		struct uio uio;
		struct iovec iov;
		u_long len = p->buflen;

		if (len > length) len = length;
		if (useRandomValues == 1)
		    GetRandomValues (p, len, p->databuf);
		else if (useRandomValues == 2)
		    GetNBSValues (offset, len, p->databuf);

		if (log && (trans == elbb_NullTranId)) {
		    code = epia_StartTran ("write_dir", p->cur.file,
					   &trans);
		    afsl_MBZ (code);
		}

		epia_InitUio (&uio, &iov, offset, len, p->databuf);
		code = epia_Write (trans, p->cur.file, 0, &uio);
		if (code) break;
		length -= len;
		offset += len;
	    } while (length > 0);
	if (code) continue;
	if (trans == elbb_NullTranId) {
	    /* create one at this point in either file or dir case */
	    code = epia_StartTran ("write_file", p->cur.file, &trans);
	    if (code) continue;
	}
	code = epif_Mark (trans, p->cur.file,
			  EPIF_SFLAGS_MTIME | EPIF_SFLAGS_CTIME);
	if (code) continue;
    } END_CRIT /* tran */;
    return code;
}

static long ReadFile(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    int useRandomValues;		/* 0=>no, 1=>random, 2=>NBS */
    char valStr[12];
    u_long value = 0;			/* value to check for */
    int i;
    u_long offset;
    char lenStr[12];
    u_long length;
    int log;				/* file is a directory */
    char *sp;				/* for calling strtoul */

    caddr_t addr;
    struct vm_buf b;			/* for vm_GetBuf and vm_Release */
    u_long bufLen, bufOff;
    u_long cmpLen, cmpOff;
    u_long len;

    afsl_Assert(p->databuf && p->cmpbuf);
    sscanf_r (count, (o->line, "%lu %11s %11s", &offset, lenStr, valStr));
    if (count < 2) {
usage:
	com_err (o->whereami, 0,
		 "Usage error: %s <offset> <length|EOF> [<value>|NBS]", o->op);
	return (-1);
    }
    useRandomValues = 1;
    if (count == 3) {
	value = strtoul (valStr, &sp, 0);
	if (*sp != '\0') {
	    lcstring (valStr, valStr, sizeof (valStr));
	    if (strcmp(valStr, "nbs") == 0) {
		useRandomValues = 2;
		value = 0;
	    } else
		goto usage;
	} else if (value <= 0xff)
	    useRandomValues = 0;
    }

    if (code = GetCurrentFile (p, &p->cur)) return code;
    log = FileIsDir(&p->cur);

    length = strtoul (lenStr, &sp, 0);
    if (*sp != '\0') {
	lcstring (lenStr, lenStr, sizeof (lenStr));
	if (strcmp(lenStr, "eof") == 0) {
	    length = AFS_hgetlo(epia_GetLength (p->cur.file)) - offset;
	} else
	    goto usage;
    }

    if (!useRandomValues) {
	if (value == 0)
	    bzero (p->cmpbuf, MIN(p->buflen, length));
	else
	    for (i = 0; i <MIN(p->buflen, length); i++)
		p->cmpbuf[i] = (char)value;
    } else if (useRandomValues == 1) {
	if (value > 0xff) {
	    afs_hyper_t seed;
	    AFS_hset64 (seed, 0, value);
	    InitRandomValues (p, seed, 0, offset);
	} else
	    InitRandomValues (p, p->cur.volId, &p->cur.fid, offset);
    } else /* NBS values */;

    if (!log && useVM) {
	u_long fileLen;
	fileLen = AFS_hgetlo(epia_GetLength (p->cur.file));
	if (offset > fileLen) code = EPI_E_EOF;
	else if (offset+length > fileLen) code = EPI_E_EOF;
	else code = 0;
	if (code) {
	    CHECK_CODE;
	    com_err (o->whereami, code,
		     "reading past EOF (@%u) from file %s", fileLen,
		     PrintedFid (&p->cur.fid, sizeof (p->buf), p->buf));
	    return code;
	}
    }

    ObtainRVM();
    bufLen = 0, cmpLen = 0;
    while (length > 0) {
	if (bufLen == 0) { /* get the data */
	    if (!log && useVM) {
		code = vm_GetBuf(p->cur.file, VM_FORREAD, offset, length, &b);
		if (code) return -1;
		bufOff = offset - b.base;
		afsl_Assert (bufOff < b.length);
		bufLen = b.length - bufOff;
		addr = b.addr;
	    } else {
		struct uio uio;
		struct iovec iov;

		bufLen = MIN (length, p->buflen);
		epia_InitUio (&uio, &iov, offset, bufLen, p->databuf);
		code = epia_Read (p->cur.file, 0, &uio);
		if (code) ReleaseRVM();
		CHECK_CODE;
		if (code) {
		    com_err (o->whereami, code,
			     "reading %d bytes at offset %u from file %s",
			     bufLen, offset,
			     PrintedFid (&p->cur.fid, sizeof (p->buf), p->buf));
		    return code;
		}
		bufOff = 0;
		addr = p->databuf;
	    }
	}
	if (cmpLen == 0) { /* get the reference values */
	    cmpLen = MIN(length, p->buflen);
	    if (useRandomValues == 1)
		GetRandomValues (p, cmpLen, p->cmpbuf);
	    else if (useRandomValues == 2)
		GetNBSValues (offset, cmpLen, p->cmpbuf);
	    cmpOff = 0;
	}
	len = MIN (length, MIN (cmpLen, bufLen));
	if ((bcmp (addr + bufOff, p->cmpbuf+cmpOff, len) != 0) &&
	    ((useRandomValues == 2) && !IsZero (addr + bufOff, len))) {
	    com_err (o->whereami, 0, "error at offset=%d in %d bytes.",
		     offset, len);
	    code = -1;
	    break;
	}
	bufOff += len, cmpOff += len, offset += len;
	bufLen -= len, cmpLen -= len, length -= len;
	if (!log && useVM) {
	    if (bufLen == 0)
		vm_Release (&b);
	}
    }
    if (!log && useVM) {
	if (bufLen > 0)
	    vm_Release (&b);
    }
    ReleaseRVM();
    return code;
}

static long TruncateFile(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    u_long length;
    afs_hyper_t l;

    sscanf_r (count, (o->line, "%lu", &length));
    if (count != 1) {
	com_err (o->whereami, 0, "Usage error: %s <length>", o->op);
	return (-1);
    }
    if (code = GetCurrentFile (p, &p->cur)) return code;

    if (useVM) {

	/* Ideally we should write/invalidate the page containing the MIN(new
         * length, old length) and invalidate pages past that point.  But it is
         * correct, if potentially slower, to purify and kill the whole file.
         * We also want to take care of any delayed writes here and verify
         * quota reservation operation so call PushVMWrites first. */

	BEGIN_CRIT (ObtainRVM(),
		    ReleaseRVM()) {
	    PushVMWrites (p);		/* if any delayed writes */
	    code = vm_Write (VM_WRITE|VM_SYNC|VM_AWAIT|VM_INVALIDATE,
			     p->cur.file);
	} END_CRIT /* vm_lock */;
	CHECK_CODE;
	if (code) {
	    com_err (p->whereami, code,
		     "I/O error flushing VM before truncate");
	    return code;
	}
    }

    AFS_hset64 (l, 0, length);
    code = epia_Truncate (elbb_NullTranId, p->cur.file,
			  (useVM ? EPIA_RWFLAGS_UNBUFFERED : 0), l);
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code, "truncating file %s to %s",
		 PrintedFid (&p->cur.fid, sizeof (p->buf), p->buf),
		 PrintedHyper (l, sizeof (p->buf2), p->buf2));
	return code;
    }
    WITH_TRANS ("truncate_file", &p->cur,
		code = epif_Mark (trans, p->cur.file,
				  EPIF_SFLAGS_MTIME | EPIF_SFLAGS_CTIME));
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code,
		 "updating mtime for file %s",
		 PrintedFid (&p->cur.fid, sizeof (p->buf), p->buf));
	return code;
    }

    return 0;
}

static long ChangeLink(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    int delta = 0;
    int delete = 0;
    char dstr[8];
    u_int newLink;

    sscanf_r (count, (o->line, "%d %7s", &delta, dstr));
    if (count < 1) {
      cl_usage:
	com_err (o->whereami, 0,
		 "Usage error: %s <delta> [delete]", o->op);
	return (-1);
    }
    if (count == 2) {
	lcstring (dstr, dstr, sizeof (dstr));
	if (strncmp (dstr, "delete", strlen(dstr)) == 0) delete++;
	else goto cl_usage;
    }

    if (code = GetCurrentFile (p, &p->cur)) return code;

    if (delta && !delete) {
	WITH_TRANS("change_link", &p->cur, 
	       { code = epif_ChangeLink(trans, p->cur.file, delta, 0, &newLink);
		 if (code) continue;
		 code = epif_Mark(trans, p->cur.file, EPIF_SFLAGS_CTIME);
  	       });
	CHECK_CODE;
	if (code) {
	    com_err(o->whereami, code, 
		    "so failed to change link count of %s by %d",
		    PrintedFid(&p->cur.fid, sizeof(p->buf), p->buf), delta);
	    return code;
	}
	if (TestPrintStatus())
	    printf("link count for %s is %d\n",
		   PrintedFid(&p->cur.fid, sizeof (p->buf), p->buf), newLink);
    }
    if (delete) {
	epi_volume_t vol = p->cur.vol;
	epi_anode_t avl = p->cur.avl;
	elbb_tranRec_t trans;
	afs_hyper_t vv;

	if (useVM) {
	    /* is there any reason to write out the pages? */
	    code = vm_Write(VM_INVALIDATE, p->cur.file);
	    afsl_MBZ(code);
	}
	code = epif_ChangeLink(elbb_NullTranId, p->cur.file,
			       delta, 1 /* delete */, &newLink);
	CHECK_CODE;
	if (code == 0) {
	    if (TestPrintStatus())
		printf("deleted %s\n",
		       PrintedFid(&p->cur.fid, sizeof (p->buf), p->buf));
	    RemoveFile(p, &p->cur);
	    p->cur.file = 0;		
	    code = epia_StartTran("delete_update_vv", avl, &trans);
	    afsl_MBZ(code);
	    code = epiv_NewVolumeVersion(trans, vol, &vv);
	    afsl_MBZ(code);
	    code = epia_EndTran(avl, trans);
	    afsl_MBZ(code);
	} else {
	    long code2;
	    com_err(o->whereami, code,
		    "so failed to change link count of %s by %d and delete it",
		    PrintedFid(&p->cur.fid, sizeof (p->buf), p->buf), delta);
	    if (code == EPI_E_HASLINKS) {
		code2 = epif_Close(p->cur.file);
		afsl_MBZ(code2);
		p->cur.file = 0;		
	    }
	    return code;
	}
    }
    return 0;
}

static long DeleteFile(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code = 0;
    int count;
    char force[8];
    int useForce;
    u_int newLink;
    int link;				/* amount to change link by */
    epi_volume_t vol; 
    epi_anode_t avl;
    elbb_tranRec_t trans;
    afs_hyper_t vv;

    useForce = 0;
    sscanf_r (count, (o->line, "%7s", force));
    if (count == 1) {
	if ((strcmp (force, "force") == 0) || (strcmp (force, "-force") == 0))
	    useForce = 1;
	else {
	    /* If not "[-]force" assume it is a filename so open it first. */
	    OpenFile (p, o);
	}
    } else if (count > 0) {
	com_err (o->whereami, 0,
		 "Usage error: %s [-force]", o->op);
	return (-1);
    }

    if (code = GetCurrentFile (p, &p->cur)) return code;

    if (useVM) {
	/* is there any reason to write out the pages? */
	code = vm_Write (VM_INVALIDATE, p->cur.file);
	afsl_MBZ (code);
    }

    if (useForce) {
	code = epif_Delete(p->cur.file, EPIF_DELETE_IGNORE_LINKCNT, NULL);
	CHECK_CODE;
	afsl_Assert(code != EPI_E_HASLINKS);
	if (code) {
	    com_err(o->whereami, code, "Failed to delete %s",
		    PrintedFid(&p->cur.fid, sizeof (p->buf), p->buf));
	    return code;
	}
    } else {
	code = epif_ChangeLink(elbb_NullTranId, p->cur.file, -1 /* delta */,
			       1 /* delete */, &newLink);
	CHECK_CODE;
	if (code) {
	    if (code != EPI_E_HASLINKS) {
		com_err(o->whereami, code,
			"Failed to decrement link count and delete %s",
			PrintedFid(&p->cur.fid, sizeof (p->buf), p->buf));
	    } else {
		com_err(o->whereami, 0, 
			"Didn't delete %s because linkCount == %d\n",
			PrintedFid(&p->cur.fid, sizeof (p->buf), p->buf), 
			newLink);
	    }
	    return code;
	}
    }

    avl = p->cur.avl;
    vol = p->cur.vol;

    code = epia_StartTran("delete2_update_vv", avl, &trans);
    afsl_MBZ(code);
    code = epiv_NewVolumeVersion(trans, vol, &vv);
    afsl_MBZ(code);
    code = epia_EndTran(avl, trans);
    afsl_MBZ(code);

    if (TestPrintStatus())
	printf ("deleted %s\n",
		PrintedFid(&p->cur.fid, sizeof (p->buf), p->buf));
    RemoveFile (p, &p->cur);
    return code;
}

static long FsyncFile(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;

    NO_ARGS;
    if (code = GetCurrentFile (p, &p->cur)) return code;

    if (useVM) {
	BEGIN_CRIT (ObtainRVM(),
		    ReleaseRVM()) {
	    PushVMWrites (p);		/* if any delayed writes */
	    code = vm_Write (VM_WRITE|VM_SYNC|VM_AWAIT, p->cur.file);
	} END_CRIT /* vm_lock */;
	CHECK_CODE;
	if (code) {
	    com_err (p->whereami, code, "I/O error flushing VM before fsync");
	    return code;
	}
    }
    code = epia_Fsync (p->cur.file);
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code, "Failed to fsync file");
	return code;
    }
    return 0;
}

/* MakeDir_Sub -- some code from MakeDir (see below), pulled out into a
 *     separate function to placate HP's limited C preprocessor. */

static long MakeDirSub(
  IN elbb_tranRec_t trans,
  IN struct processData *p,
  IN struct file *parentP,
  IN u_int *parentLinkP)
{
    long code;
    struct epif_status fileStatus;
    epi_anode_t ap;
    long flags;
    u_int newLink;			/* new link count of new dir */
    struct uio uio;
    struct iovec iov;
    int i;
    static u_char byte = 8;

#define BUMP_BYTE (byte++, ((byte > 0xf8) ? (byte = 0x8) : 0))

    flags = EPIF_CFLAGS_DIRECTORY;
    bzero ((char *)&fileStatus, sizeof (fileStatus));
    code = epif_CreateE (trans, p->cur.vol, flags,
			 /*estLength*/0, &fileStatus,
			 parentP->file, 0, 0,
			 &p->cur.fid, &ap);
    if (code) return code;
    code = epif_Close (ap);
    afsl_MBZ (code);
    code = GetCurrentFile(p, &p->cur);
    if (code) return code;

    /* new dir header */
    for (i=0; i<192; i++) p->databuf[i] = byte;
    epia_InitUio (&uio, &iov, 0, 192, p->databuf);
    code = epia_Write (trans, p->cur.file, 0, &uio);
    if (code) return code;
    BUMP_BYTE;

    /* Add "." */

    /* himark */
    epia_InitUio (&uio, &iov, 5, 1, &byte);
    code = epia_Write (trans, p->cur.file, 0, &uio);
    if (code) return code;
    BUMP_BYTE;

    /* free */
    for (i=0; i<32; i++) p->databuf[i] = byte;
    epia_InitUio (&uio, &iov, 192, 32, p->databuf);
    code = epia_Write (trans, p->cur.file, 0, &uio);
    if (code) return code;
    BUMP_BYTE;

    /* freelist */
    epia_InitUio (&uio, &iov, 4, 1, &byte);
    code = epia_Write (trans, p->cur.file, 0, &uio);
    if (code) return code;
    BUMP_BYTE;

    /* "." */
    for (i=0; i<32; i++) p->databuf[i] = byte;
    epia_InitUio (&uio, &iov, 160, 32, p->databuf);
    code = epia_Write (trans, p->cur.file, 0, &uio);
    if (code) return code;
    BUMP_BYTE;

    /* hash bucket */
    epia_InitUio (&uio, &iov, 78, 1, &byte);
    code = epia_Write (trans, p->cur.file, 0, &uio);
    if (code) return code;
    BUMP_BYTE;

    /* Add ".." */

    /* himark */
    epia_InitUio (&uio, &iov, 5, 1, &byte);
    code = epia_Write (trans, p->cur.file, 0, &uio);
    if (code) return code;
    BUMP_BYTE;

    /* free */
    for (i=0; i<32; i++) p->databuf[i] = byte;
    epia_InitUio (&uio, &iov, 224, 32, p->databuf);
    code = epia_Write (trans, p->cur.file, 0, &uio);
    if (code) return code;
    BUMP_BYTE;

    /* freelist */
    epia_InitUio (&uio, &iov, 4, 1, &byte);
    code = epia_Write (trans, p->cur.file, 0, &uio);
    if (code) return code;
    BUMP_BYTE;

    /* ".." */
    for (i=0; i<32; i++) p->databuf[i] = byte;
    epia_InitUio (&uio, &iov, 192, 32, p->databuf);
    code = epia_Write (trans, p->cur.file, 0, &uio);
    if (code) return code;
    BUMP_BYTE;

    /* hash bucket */
    epia_InitUio (&uio, &iov, 100, 1, &byte);
    code = epia_Write (trans, p->cur.file, 0, &uio);
    if (code) return code;
    BUMP_BYTE;

    /* This finishes formatting new directory */

    if (parentP->file) {
	u_long parentLen;

	/* Now add "fn.name" to parent */

	/* himark */
	epia_InitUio (&uio, &iov, 5, 1, &byte);
	code = epia_Write (trans, parentP->file, 0, &uio);
	if (code) return code;
	BUMP_BYTE;

	/* free */
	parentLen = AFS_hgetlo(epia_GetLength(parentP->file));
	for (i=0; i<32; i++) p->databuf[i] = byte;
	epia_InitUio (&uio, &iov, parentLen, 32, p->databuf);
	code = epia_Write (trans, parentP->file, 0, &uio);
	if (code) return code;
	BUMP_BYTE;

	/* freelist */
	epia_InitUio (&uio, &iov, 4, 1, &byte);
	code = epia_Write (trans, parentP->file, 0, &uio);
	if (code) return code;
	BUMP_BYTE;

	/* "fn.name" */
	for (i=0; i<32; i++) p->databuf[i] = byte;
	epia_InitUio (&uio, &iov, parentLen-32, 32, p->databuf);
	code = epia_Write (trans, parentP->file, 0, &uio);
	if (code) return code;
	BUMP_BYTE;

	/* hash bucket */
	epia_InitUio (&uio, &iov, 99, 1, &byte);
	code = epia_Write (trans, parentP->file, 0, &uio);
	if (code) return code;
	BUMP_BYTE;

	code = epif_ChangeLink (trans, parentP->file, 1, 0, parentLinkP);
	if (code) return code;
	code = epif_Mark (trans, parentP->file,
			  EPIF_SFLAGS_MTIME | EPIF_SFLAGS_CTIME);
	if (code) return code;
    } else *parentLinkP = 0;

    code = epif_ChangeLink (trans, p->cur.file, 1, 0, &newLink);
    if (code) return code;
    afsl_PAssert (newLink == 2,
		  ("%s: New directory's linkCount should be 2",
		   p->whereami));
    return code;
}

/* MakeDir -- a command which tries to simulate the horrible behavior of the
 *     real mkdir vnodeop. */

static long MakeDir(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    struct fileName fn;
    struct fileName *f;
    char parentDir[32];
    struct file parent;
    u_int parentLink;			/* new link count of parent */

    if (code = ClosePreviousFile (p, &p->cur)) return code;

    parentDir[0];
    sscanf_r (count, (o->line, "%31s %31s", fn.name, parentDir));
    if (count != 2) {
	com_err (o->whereami, 0, "Usage error: %s <name> <parent>", o->op);
	return (-1);
    }

    afsl_Assert(strlen (fn.name) < sizeof (fn.name));
    if (f = dfsh_HashLookup (&p->fileNameHT, &fn)) {
	com_err (o->whereami, 0, "File %s already exists", fn.name);
	return -1;
    }

    if (code = GetCurrentVol(p, &p->cur)) return code;

    code = GetAuxFile (p, &p->cur, parentDir, &parent, "parent directory");
    if (code) return code;

    WITH_TRANS ("mkdir", &p->cur,
        {
	    code = MakeDirSub (trans, p, &parent, &parentLink);
	} );
    afsl_MBZ (ClosePreviousAggr (p, &parent));
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code, "creating an anode in a volume");
	return code;
    }
    f = (struct fileName *) osi_Alloc (sizeof (fn));
    f->fid = p->cur.fid;
    f->volId = p->cur.volId;
    f->devi = p->cur.devi;
    strcpy (f->name, fn.name);
    code = dfsh_HashIn_r (&p->fileNameHT, f);
    if (code) {
	com_err (o->whereami, code, "adding file %s to hash table", fn.name);
	return code;
    }
    if (TestPrintStatus())
	printf ("fid is %s, parent's linkCount is %d\n",
		PrintedFid (&p->cur.fid, sizeof (p->buf), p->buf), parentLink);
    return 0;
}

static long FileInfo(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    char s[10][16];
    struct epia_info info;
    struct epif_status status;
    struct epiv_status vstatus;
    int i;

    sscanf_r (count, (o->line, "%s %s %s %s %s %s %s %s %s %s",
		      s[0], s[1], s[2], s[3], s[4],
		      s[5], s[6], s[7], s[8], s[9]));

    if (code = GetCurrentFile (p, &p->cur)) return code;

    code = epia_GetInfo (p->cur.file, &info);
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code, "calling epia_GetInfo from get_info");
	return code;
    }
    code = epif_GetStatus (p->cur.file, &status);
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code,
		 "calling epif_GetStatus from get_info");
	return code;
    }
    code = epiv_GetStatus (p->cur.vol, &vstatus);
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code, "calling epiv_GetStatus from get_info");
	return code;
    }
    for (i=0; i<count; i++) {
	char c = s[i][0];		/* field indicator */
	char o = s[i][1];		/* operation */
	int cmp;
	value_t testValue;
	value_t actual;

	if ((o == '=') || (o == '!')) {
	    code = ScanValue (c, s[i]+2, &testValue);
	    if (code) {
		/* not a number, maybe it's another field name */
		code = LookupField (*(s[i]+2), 0, &info, &status, &vstatus,
				    &testValue, 0,0);
		if (code) goto gi_usage;
	    }
	    code = LookupField (c, 0, &info, &status, &vstatus, &actual, 0,0);
	    if (code) goto gi_usage;
	    /* Type fields may be different. */
	    cmp = bcmp ((char *)&actual.v, (char *)&testValue.v,
			sizeof (actual.v));
	    if ((o == '=') != (cmp == 0)) {
		com_err (p->whereami, 0,
			 "test %s fails, actual was %s, test was %s", s[i],
			 PrintedValue (&actual, sizeof (p->buf), p->buf),
			 PrintedValue (&testValue, sizeof (p->buf2), p->buf2));
		code = -1;
		break;
	    }
	} else if ((c == '!') || (c == '=')) {
	    char tmp = c;
	    c = o;
	    o = tmp;
	    code = LookupField (c, 0,
				&p->oldInfo, &p->oldStatus, &p->oldVolStatus,
				&testValue, 0,0);
	    if (code) goto gi_usage;
	    code = LookupField (c, 0, &info, &status, &vstatus, &actual, 0,0);
	    if (code) goto gi_usage;
	    cmp = bcmp ((char *)&actual, (char *)&testValue, sizeof (actual));
	    if ((o == '=') != (cmp == 0)) {
		fprintf (stderr,
			 "%s: test %s fails, ", p->whereami, s[i]);
		if (cmp == 0)
		    fprintf (stderr, "both are %s\n",
			     PrintedValue(&actual, sizeof (p->buf), p->buf));
		else {
		    fprintf (stderr, "actual is %s, ",
			     PrintedValue(&actual, sizeof (p->buf), p->buf));
		    fprintf (stderr, "old value was %s\n",
			     PrintedValue (&testValue,
					   sizeof (p->buf), p->buf));
		}
		code = -1;
		break;
	    }
	} else {
	  gi_usage:
	    com_err (p->whereami, 0,
		     "Illegal format in test string %s", s[i]);
	    return -1;
	}
    }					/* for each test */
    if (code || (count < 1) || (TestPrintStatus())) {
	value_t time;
	printf ("For %s: length=%s, allocated/visible=%d/%d, copies=%d, dv=%d",
		PrintedFid (&p->cur.fid, sizeof (p->buf), p->buf),
		PrintedHyper (info.length, sizeof (p->buf2), p->buf2),
		info.allocated, info.visible, info.copies,
		status.dataVersionNumber);
	printf (" VV=%s\n", PrintedHyper (status.volumeVersionNumber,
					  sizeof (p->buf), p->buf));
	printf ("  linkCount=%d, mode=%d, Acl=%d bytes",
		status.linkCount, status.mode, status.acl);
	if (FileIsDir(&p->cur)) {
	    printf (", Initial Dir/File Acl=%d/%d bytes",
		status.initialDirAcl, status.initialFileAcl);
	}
	printf (", Plist/UserPlist=%d/%d bytes\n",
		status.plist, status.reservedForUserPlist);
	time.type = 'm';
	time.v.t = status.mTime;
	printf ("  modifyTime: %s\n",
		PrintedValue (&time, sizeof (p->buf), p->buf));
	time.v.t = status.cTime;
	printf ("  anodeTime: %s\n",
		PrintedValue (&time, sizeof (p->buf), p->buf));
	time.v.t = status.aTime;
	printf ("  accessTime: %s\n",
		PrintedValue (&time, sizeof (p->buf), p->buf));
    }
    if (code) return code;

    /* save old values */
    p->oldInfo = info;
    p->oldStatus = status;
    return 0;
}

static long SetStatus(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    char s[10][16];
    int fileOK;				/* allow setting file attributes */
    struct epif_status status;
    struct epiv_status vstatus;
    long flags, vflags;
    int i;

    sscanf_r (count, (o->line, "%s %s %s %s %s %s %s %s %s %s",
		      s[0], s[1], s[2], s[3], s[4],
		      s[5], s[6], s[7], s[8], s[9]));

    if (code = GetCurrentVol (p, &p->cur)) return code;
    code = epiv_GetStatus (p->cur.vol, &vstatus);
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code, "calling epiv_GetStatus from set_status");
	return code;
    }

    /* Test if the file is available carefully.  If not just disable file
     * attribute setting. */

    fileOK = 0;
    if (!epid_IsInvalid(&p->cur.fid)) {
	epi_anode_t file;
	if (!epif_Open (p->cur.vol, &p->cur.fid, &file) &&
	    !epif_Close (file)) {
	    if (code = GetCurrentFile (p, &p->cur)) return code;
	    code = epif_GetStatus (p->cur.file, &status);
	    CHECK_CODE;
	    if (code) {
		com_err (o->whereami, code,
			 "calling epif_GetStatus from set_status");
		return code;
	    }
	    fileOK = 1;
	}
    }

    bzero ((char *)&status, sizeof(status));
    bzero ((char *)&vstatus, sizeof(vstatus));
    vflags = flags = 0;
    for (i=0; i<count; i++) {
	char c = s[i][0];		/* field indicator */
	char o = s[i][1];		/* operation */
	long f,v;
	value_t newValue;

	if (o != '=') {
  ss_usage:
	    com_err (p->whereami, 0, "Illegal field specification %s", s[i]);
	    return -1;
	}
	code = ScanValue (c, s[i]+2, &newValue);
	if (code) goto ss_usage;
	code = LookupField (c, 1, 0, &status, &vstatus, &newValue, &f, &v);
	if (code) goto ss_usage;

	if (f) flags |= f;
	else if (v) vflags |= v;
	else goto ss_usage;
    }					/* for each field */
    if (!flags && !vflags) goto ss_usage;
    if (flags && !fileOK) {
	com_err (o->whereami, 0, "No current file, can't set file attributes");
	return -1;
    }

    WITH_TRANS("SetStatus", &p->cur,
        {
	    if (flags) {
		code = epif_SetStatus (trans, p->cur.file, flags, &status);
		if (code) continue;
		code = epif_Mark (trans, p->cur.file, EPIF_SFLAGS_CTIME);
	    } else if (vflags) {
		code = epiv_SetStatus (trans, p->cur.vol, vflags, &vstatus);
	    }
	});
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code, "setting status (%s) for file %s",
		 o->line, PrintedFid(&p->cur.fid, sizeof (p->buf), p->buf));
	return code;
    }
    return 0;
}

static int GetAuxType(IN char *name)
{
    if (strcmp(name, "acl") == 0) return EPIF_AUX_IS_ACL;
    else if (strcmp(name, "idacl") == 0) return EPIF_AUX_IS_INITIAL_DIR_ACL;
    else if (strcmp(name, "ifacl") == 0) return EPIF_AUX_IS_INITIAL_FILE_ACL;
    else if (strcmp(name, "plist") == 0) return EPIF_AUX_IS_PLIST;
    else if (strcmp(name, "uplist") == 0) return EPIF_AUX_IS_USER_PLIST;
    else {
	com_err ("GetAuxType", 0, "Unknown aux type %s", name);
	return 0;
    }
}

static long ReadAux(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    char type[10];
    int auxType;
    u_long offset;
    u_long length;
    int value;				/* value to check for */
    int i;
    u_long auxLen;
    char *auxIs;
    char *auxShould = 0;
    int auxShouldLen;
    struct epif_status status;
    int str;				/* compare to string */

    sscanf_r (count, (o->line, "%9s %lu %lu %d",
		      type, &offset, &length, &value));
    if (count < 2) {
	auxShouldLen = 64;
	auxShould = osi_Alloc (auxShouldLen);
	sscanf_r (count, (o->line, "%9s %63s", type, auxShould));
	if (count != 2) {
  ra_usage:
	    com_err (o->whereami, 0,
		     "Usage error: %s <type> {<off> <len> [<value>] | <string>}",
		     o->op);
	    return (-1);
	}
	if (strcmp (auxShould, "\"\"") == 0) auxShould[0] = 0;
	str = 1;
    } else {
	if (count < 3) goto ra_usage;
	str = 0;
    }
    auxType = GetAuxType(type);
    if (!auxType) return -1;

    if (code = GetCurrentFile (p, &p->cur)) return code;

    code = epif_GetStatus (p->cur.file, &status);
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code,
		 "calling epif_GetStatus from read_aux");
	return code;
    }
    switch (auxType) {
      case EPIF_AUX_IS_ACL: auxLen = status.acl;
	break;
      case EPIF_AUX_IS_INITIAL_DIR_ACL: auxLen = status.initialDirAcl;
	break;
      case EPIF_AUX_IS_INITIAL_FILE_ACL: auxLen = status.initialFileAcl;
	break;
      case EPIF_AUX_IS_PLIST: auxLen = status.plist;
	break;
      case EPIF_AUX_IS_USER_PLIST: auxLen = status.reservedForUserPlist;
	break;
      default: auxLen = 0;
    }

    if ((auxLen > 8192) && (auxType != EPIF_AUX_IS_USER_PLIST)) {
	com_err (o->whereami, 0, "aux for file %s too large",
		 PrintedFid (&p->cur.fid, sizeof (p->buf), p->buf));
	return -1;
    }
    afsl_Assert(auxLen < 100000);		/* for safety */
    if (str) {
	offset = 0;
	length = auxLen;
    }
    auxIs = osi_Alloc (length);
    code = epif_ReadAux
	(p->cur.file, auxType, offset, length, auxIs);
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code, "calling epif_ReadAux from read_aux");
	return code;
    }
    if (str) {
	if ((auxLen != strlen (auxShould)) ||
	    (bcmp (auxIs, auxShould, length) != 0)) {
	  ra_badcmp:
	    com_err (o->whereami, 0, "Aux contents not as expected");
	    return -1;
	}
    } else {
	auxShouldLen = length;
	auxShould = osi_Alloc (length);
	if (count == 3) {
	    afs_hyper_t zero;
	    AFS_hzero(zero);
	    /* Can't use fid, since several files may share same acl.
	     * Also since they can be copied across volumes the volId
	     * is no good either. */
	    InitRandomValues (p, zero, 0, offset);
	    GetRandomValues (p, length, auxShould);
	} else
	    for (i = 0; i < length; i++)
		auxShould[i] = (char)value;
	if (bcmp (auxIs, auxShould, length) != 0) goto ra_badcmp;
    }
    osi_Free (auxIs, length);
    osi_Free (auxShould, auxShouldLen);
    return 0;
}

static long WriteAux(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    int auxType;
    char type[10];
    u_long offset;
    u_long length;
    int value;				/* value to check for */
    int i;
    long flags;
    char *auxShould = 0;
    int auxShouldLen;

    sscanf_r (count, (o->line, "%9s %lu %lu %d",
		      type, &offset, &length, &value));
    if (count < 2) {
	auxShouldLen = 64;
	auxShould = osi_Alloc (auxShouldLen);
	sscanf_r (count, (o->line, "%9s %63s", type, auxShould));
	if (count != 2) {
  wa_usage:
	    com_err (o->whereami, 0,
		     "Usage error: %s <type> {<off> <len> [<value>] | <string>}",
		     o->op);
	    return -1;
	}
	if (strcmp (auxShould, "\"\"") == 0) auxShould[0] = 0;
	offset = 0;
	length = strlen(auxShould);
    } else {
	if (count < 3) goto wa_usage;
	auxShouldLen = length;
	auxShould = osi_Alloc (length);
	if (count == 3) {
	    afs_hyper_t zero;
	    AFS_hzero(zero);
	    InitRandomValues (p, zero, 0, offset);
	    GetRandomValues (p, length, auxShould);
	} else
	    for (i = 0; i < length; i++)
		auxShould[i] = (char)value;
    }
    auxType = GetAuxType(type);
    if (!auxType) return -1;

    if (code = GetCurrentFile (p, &p->cur)) return code;

    flags = auxType | EPIF_FLAGS_NEW_AUX|EPIF_FLAGS_TRUNCATE_AUX;

    WITH_TRANS("WriteAux", &p->cur,
	       {
		   code = epif_WriteAux (trans, p->cur.file, flags,
					 offset, length, auxShould);
		   if (code) continue;
		   code = epif_Mark (trans, p->cur.file, EPIF_SFLAGS_CTIME);
	       });
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code,
		 "calling epif_WriteAux from write_aux");
	return code;
    }
    osi_Free (auxShould, auxShouldLen);
    return 0;
}

static long ReplaceAux(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    int fileType;
    int auxType;
    char typeFile[10];			/* dest aux */
    char typeAux[10];			/* source aux */
    char auxFile[32];
    struct file aux;
    long flags;
    struct fileName *f;

    sscanf_r (count, (o->line, "%9s %31s %9s", typeFile, auxFile, typeAux));
    if (count < 2) {
	com_err (o->whereami, 0,
		 "Usage error: %s <type> <source file> [<source type>]",
		 o->op);
	return (-1);
    }
    fileType = GetAuxType(typeFile);
    if (count == 3) {
	auxType = GetAuxType(typeAux);
	if (!auxType || !auxFile) {
	    com_err (o->whereami, 0,
		     "Bogus aux types '%s' or '%s'", typeFile, typeAux);
	    return -1;
	}
	flags = (auxType<<4) | fileType;
    } else {
	flags = fileType;		/* test defaulting */
    }

    if (code = GetCurrentFile (p, &p->cur)) return code;

    bzero ((char *)&aux, sizeof (aux));
    f = LookupFile (p, &p->cur, auxFile);
    if (!f) {
	code = EPI_E_NOENT;
      ra_noaux:
	com_err (o->whereami, code,
		 "getting file %s from which to copy the aux",
		 auxFile);
	return code;
    }
    aux.fid = f->fid;
    aux.volId = f->volId;
    aux.devi = f->devi;
    code = GetCurrentFile (p, &aux);
    if (code) goto ra_noaux;

    WITH_TRANS("replace_aux", &p->cur,
	       {
		   code = epif_ReplaceAux (trans,
					   p->cur.file, flags, aux.file);
		   afsl_MBZ (ClosePreviousAggr (p, &aux));
		   if (code) continue;
		   code = epif_Mark (trans, p->cur.file, EPIF_SFLAGS_CTIME);
	       });
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code, "calling epif_ReplaceAux");
	return code;
    }
    return 0;
}

static long CloneFile(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int recloneOp = IsOp("reclone_file");
    long flags = (recloneOp ? 1 : 0);
    struct fileName *back;
    struct file backing;

    if (code = GetCurrentFile (p, &p->cur)) return code;

    if (useVM || recloneOp) {
	/* We are going to bash the backing anode with this command so we
         * better clear out VM.  Actually we can probably just invalidate, but
         * surely this is safer and since the backing file is R/O it can't cost
         * much extra. */
	BEGIN_LLCRIT (epi_volume_t volB,
		      epi_anode_t b,
		      code = epiv_OpenBackingAnode (p->cur.file, &volB, &b),
		      (code = epiv_CloseAnode (b),
		       code = epiv_Close(elbb_NullTranId, volB))) {
	    struct epiv_ident ident;
	    if (useVM) {
		BEGIN_CRIT (ObtainRVM(),
			    ReleaseRVM()) {
		    PushVMWrites (p);		/* if any delayed writes */
		    code = vm_Write (VM_WRITE|VM_SYNC|VM_AWAIT|VM_INVALIDATE,
				     b);
		} END_CRIT /* vm_lock */;
		CHECK_CODE;
		if (code) {
		    com_err (p->whereami, code,
			     "I/O error flushing backing file before clone");
		    return code;
		}
	    }
	    /* We know that the device, volId and the index of the backing file
             * will not change, and are sufficient to uniquely determine the
             * file. */
	    epid_MakeIndexOnly(&backing.fid,
			       epid_IndexForFid(epiv_GetAnodeIndex(b),
						p->cur.device));
	    code = epiv_GetIdent(volB, &ident);
	    if (code) com_err (p->whereami, code, "Can't get backing volId");
	    backing.volId = ident.id;
	    backing.devi = p->cur.devi;
	} END_CRIT;
    }

    code = epif_Clone (elbb_NullTranId, p->cur.file, flags);
    CHECK_CODE;
    if (code) {
	com_err (o->whereami, code, "%scloning file %s",
		 (recloneOp ? "re" : "un"),
		 PrintedFid(&p->cur.fid, sizeof (p->buf), p->buf));
	return code;
    }
    if (recloneOp) {
	/* Since the reclone operation changes the unique of the backing file
         * to that of the COW file the fid will change, so we must update out
         * name table. */
	struct fileName *back;
	struct fileName *new;
	back = LookupFile (p, &backing, 0);
	afsl_PAssert (back,
	    ("%s: Couldn't find backing file: volid %s, fid index %d",
	     o->whereami,
	     PrintedHyper(backing.volId, sizeof(p->buf), p->buf),
	     epid_ToIndex(&backing.fid)));
	new = (struct fileName *)osi_Alloc (sizeof (*back));
	new->fid = back->fid;		/* save name and info of back file */
	new->volId = back->volId;
	new->devi = back->devi;
	strcpy (new->name, back->name);
	RemoveFile (p, &backing);	/* this invalidates back pointer */
	back = 0;
	/* copy unique, then reinstall back file */
	epid_UniquePart(&new->fid) = epid_UniquePart(&p->cur.fid);
	code = dfsh_HashIn_r (&p->fileNameHT, new);
	if (code) {
	    com_err (p->whereami, code,
		     "re-adding backing file %s to hash table", new->name);
	    return code;
	}
	if (TestPrintStatus())
	    printf ("reclone changes fid of %s to %s\n",
		    new->name, PrintedFid(&new->fid, sizeof (p->buf), p->buf));
    }
    return 0;
}

struct mountEntry {
    struct fifo_Thread thread;
    struct file file;
};
static struct fifo mountTable;

/* MountFile -- works kinda like mount does in that it pins a file open,
 *     especially including its associated volume, aggregate and device.  It
 *     just remembers these files in a static table.  See also the UnmountFile
 *     command.
 *
 *     It does NOT change the currently open file. */

static long MountFile(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    char name[32];
    struct mountEntry *vfs;

    sscanf_r (count, (o->line, "%31s", name));
    if (count != 1) {
	com_err (o->whereami, 0, "Usage error: %s <name>", o->op);
	return (-1);
    }

    if (code = GetCurrentVol(p, &p->cur)) return code;

    vfs = (struct mountEntry *)osi_Alloc (sizeof (struct mountEntry));

    code = GetAuxFile (p, &p->cur, name, &vfs->file, "mountfile");
    if (code) {
	osi_Free (vfs, sizeof (struct mountEntry));
	return code;
    }
    FIFO_ADD (&mountTable, &vfs->thread);

    return 0;
}

/* UnmountFile -- reverses the action of a mount file command.  Basically it
 *     tracks down the specified name in the global mount table, closes the
 *     file, and removes the entry.  If no name is specified all entries in the
 *     mount table are removed.
 *
 *     This procedure can be called directly from the shutdown code.  In that
 *     case don't check for arguments just unmount everything. */

static long UnmountFile(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    int shutdown = IsOp("end");
    char name[32];
    struct fileName *f;
    struct mountEntry *vfs;

    if (shutdown) count = 0;
    else {
	sscanf_r (count, (o->line, "%31s", name));
	if (count > 1) {
	    com_err (o->whereami, 0, "Usage error: %s [<name>]", o->op);
	    return (-1);
	}
    }

    if (count == 0) {

	/* Close all files in the mount table */

	while (vfs = FIFO_LookAtOldest(&mountTable,
			offsetof(struct mountEntry, thread))) {
	    code = ClosePreviousAggr (p, &vfs->file);
	    afsl_MBZ (code);
	    FIFO_REMOVE (&mountTable, &vfs->thread);
	    osi_Free (vfs, sizeof (struct mountEntry));
	}

    } else {

	/* Close the specified entry in the mount table. */

	if (code = GetCurrentVol(p, &p->cur)) return code;

	f = LookupFile (p, &p->cur, name);
	if (!f)  {
	    com_err (o->whereami, 0, "No such file found %s", name);
	    return -1;
	}

	/* look up the file in the mountTable by dev, volId and fid. */

	for (vfs = FIFO_LookAtOldest(&mountTable,
			offsetof(struct mountEntry, thread));
	     vfs;
	     vfs = FIFO_NextOldest (&mountTable, vfs, &vfs->thread))
	    if (epid_Equal (&vfs->file.fid, &f->fid) &&
		AFS_hsame (vfs->file.volId, f->volId) &&
		(vfs->file.devi == f->devi))
		break;

	if (!vfs) {
	    code = EPI_E_NOENT;
	    com_err (o->whereami, code, "File %s not mounted", name);
	    return code;
	}

	/* Actually close the file and remove the entry from the mount table */

	code = ClosePreviousAggr (p, &vfs->file);
	afsl_MBZ (code);
	FIFO_REMOVE (&mountTable, &vfs->thread);
	osi_Free (vfs, sizeof (struct mountEntry));
    }

    return 0;
}

static long ParallelTest(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    char file[EPIR_MAX_TESTFILENAME];
    struct processData *newp;

    file[sizeof (file)-1] = 0;
    sscanf_r (count, (o->line, "%s", file));
    afsl_MBZ (file[sizeof (file)-1]);
    if (count < 1) {
	com_err (o->whereami, 0, "Usage error: %s <pathname>", o->op);
	return (-1);
    }
    newp = (struct processData *) osi_Alloc (sizeof (*p));
    code = InitProcessData (p, newp);
    if (code) return code;
    newp->testFile = osi_Alloc (strlen(file)+1);
    strcpy (newp->testFile, file);

    code = epir_RunTest (newp->testFile, newp->ops, (opaque)newp,
			 EPIR_NEW_PROCESS|EPIR_NEW_TEST);
    afsl_MBZ(code);
    return 0;
}

static long SaveFileNames(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    int saveOp = IsOp("save");
    char file[EPIR_MAX_TESTFILENAME];
    FILE *f;

    file[sizeof (file)-1] = 0;
    sscanf_r (count, (o->line, "%s", file));
    afsl_MBZ (file[sizeof (file)-1]);
    if (count < 1) {
	com_err (o->whereami, 0, "Usage error: %s <pathname>", o->op);
	return (-1);
    }
    f = fopen (file, (saveOp ? "w" : "r"));
    if (!f) {
	com_err (o->whereami, errno,
		 "opening %s for %s operation", file, o->op);
	return -1;
    }
    code = (*(saveOp ? DumpFileNameHT : RestoreFileNameHT)) (p, f);
    return code;			/* msg printed already */
}

static long SetTransferSize(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    int count;
    u_long size;

    sscanf_r (count, (o->line, "%u", &size));
    if (count != 1) {
usage:
	com_err (o->whereami, 0, "Usage error: %s <buffer size>", o->op);
	return (-1);
    }
    if (p->buflen == size) return 0;	/* no change */
    if (p->buflen > 0) {
	if (p->databuf) osi_Free (p->databuf, p->buflen);
	if (p->cmpbuf) osi_Free (p->cmpbuf, p->buflen);
    }
    p->buflen = size;
    p->databuf = osi_Alloc (p->buflen);
    p->cmpbuf = osi_Alloc (p->buflen);
    if (TestPrintStatus())
	printf ("New file transfer size is %d bytes.\n", p->buflen);
    return 0;
}

static long SetWithholding(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    int count;
    long lo, hi, target;

    lo = hi = target = 0;
    sscanf_r (count, (o->line, "%d %d %d", &lo, &hi, &target));
    if (count < 1) {
usage:
	com_err (o->whereami, 0, "Usage error: %s <low millage> [<high mills> [<target K>]] ", o->op);
	return (-1);
    }
    {   extern u_long epib_leastWithheld, epib_mostWithheld, epib_withholding;

	if (lo >= 0)
	    epib_leastWithheld = lo;
	if (hi >= 0)
	    epib_mostWithheld = hi;
	if (target >= 0)
	    epib_withholding = target;
    }
    return 0;
}

/*
 * StartAsync -- forks a thread to run the async daemon.
 *
 * XXX Disable this for now
 */

static void StartAsync(void)
{
#if 0
    static pthread_t *async = NULL;
    long code;

    if (async != NULL)
	return;

    async = osi_Alloc(sizeof (*async));
    code = epi_ThreadCreate((pthread_func_t)epia_Async, 4000,
			    EPI_NORMAL_PRIORITY, (void *)0, async);
    afsl_PAssert (code==0, ("Can't start thread for Async: code=%d", code));
    fprintf (stdout, "Starting async daemon to complete VM I/O.\n");
#endif /* 0 */
}

/* SetVM -- switch to/from using VM paradigm for read/write.  If going "on" for
 *     the first time fork an async daemon for use with strategy. */

static long SetVM(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    char state[10];
    u_long pageSize = 0;
    u_long extentSize = 0;
    int use;				/* desired state of useVM flag */

    sscanf_r (count, (o->line, "%9s %u %u", state, &pageSize, &extentSize));
    if (count < 1)
	goto usage;

    if (strcmp (state, "on") == 0)
	use = 1;
    else if (strcmp (state, "off") == 0)
	use = 0;
    else if (strcmp (state, "\"\"") == 0)
	use = -1;			/* just set page size */
    else
	goto usage;

    if (use != 0) {
	u_long bsize = (p->cur.device ? p->cur.device->blkSize : 0);
	code = vm_SetSizes (o->whereami, bsize, pageSize, extentSize);
	if (code) return -1;
	if (use == -1) return 0;	/* we're done */
    }
    if (use == 0) {
	if (pageSize || extentSize) goto usage;
	if (useVM) {
	    ObtainWVM();
	    code = vm_Write (VM_WRITE|VM_SYNC|VM_AWAIT|VM_INVALIDATE, 0);
	    CHECK_CODE;
	    if (code) {
		com_err (p->whereami, code, "I/O error flushing VM");
		return code;
	    }
	    ReleaseWVM();
	    /* invalidate buffer pool */
	}
    } else /* use == 1 */ {
	if (!os_supports_vm) {
	    printf("%s: system does not support VM, hence not enabled.\n",
		   o->whereami);
	    use = -1;
	} else {
	    StartAsync ();
	    if (!useVM) {
		/* flush dirty buffers */
		vm_Write (VM_INVALIDATE, 0);
	    }
	}
    }
    if (use >= 0) {
	useVM = (use != 0);
    } /* else don't change setting */
    return 0;

usage:
    com_err (o->whereami, 0,
	     "Usage error: %s {on [<page size> [<extent size>]] | off}",
	     o->op);
    return (-1);
}

static long ModifyAFSDB(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    int count;
    char vbl[32];			/* name of afsdb variable */
    char op[32];			/* operation to perform */
    int bit;				/* bit number to modify */
    static struct vbl {
	char *name;
	int *vbl;
    } vbls[] = {
	{"runtest", &afsdb_episode_runtest},
	{"test", &afsdb_episode_anode_test},
	{"anode_test", &afsdb_episode_anode_test},
	{"anode_verify", &afsdb_episode_anode_verify},
	{"verify", &afsdb_episode_anode_verify},
	{"anode", &afsdb_episode_anode},
	{"logbuf", &afsdb_episode_logbuf},
	{"async", (int *) &afsdb_episode_async},
    };
    int *v;
    int mask;			/* mask corresponding to bit */
    int i;

    sscanf_r (count, (o->line, "%31s %31s %d", vbl, op, &bit));
    if (count != 3) {
usage:
	com_err (o->whereami, 0,
		 "Usage error: %s <variable> <operation> <bit number>", o->op);
	return (-1);
    }
    mask = 1 << (bit-1);
    if ((bit < 1) || (bit > 32) || (mask == 0)) {
	fprintf (stderr, "bit number must be between 1 and 32, not %d\n", bit);
	goto usage;
    }
    lcstring (vbl, vbl, sizeof (vbl));
    v = 0;
    for (i=0; i<(sizeof (vbls)/sizeof(vbls[0])); i++) {
	if (strcmp (vbls[i].name, vbl) == 0) {
	    v = vbls[i].vbl;
	    break;
	}
    }
    if (v == 0) {
	fprintf (stderr, "unknown variable name %s\n", vbl);
	goto usage;
    }
    if (strcmp (op, "on") == 0) *v |= mask;
    else if (strcmp (op, "off") == 0) *v &= ~mask;
    else if (strcmp (op, "xor") == 0) *v ^= mask;
    else {
	fprintf (stderr, "\
unknown operation; must be 'on', 'off', or 'xor', not '%s'\n", op);
	goto usage;
    }
    if (TestPrintStatus()) printf ("afsdb variable %s is %#x\n", vbl, *v);
    return 0;
}

static long TestUniqueTime(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    NO_ARGS;
    afsdb_episode_anode |= (1<<(EPIA_DEBUG_UNIQUETIME-1));
    code = epif_Init();
    afsdb_episode_anode &= ~(1<<(EPIA_DEBUG_UNIQUETIME-1));
    return code;
}

static long Coverage(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    NO_ARGS;
    if (TestPrintCoverage()) {
#ifdef AFS_DEBUG
	extern int epix_mv_allStates[];
	extern long epix_mv_allOps;
	int i;
	int insertNewline;
	if (epix_mv_allOps) {
	    printf ("clone coverage %x\n", epix_mv_allOps);
	    insertNewline = 1;
	    for (i=0; i<36; i++) {
		if (insertNewline == 1) insertNewline=0, printf (" %2d:", i);
		printf (" %2d", epix_mv_allStates[i]);
		if (i == 19) insertNewline=1, printf ("\n");
	    }
	    if (insertNewline == 0) printf ("\n");
	}
	{
#define pfc epib_PercentFragmentCoverage
	    printf ("\
allocator %%: (1) %.2g (2) %.2g %.2g (4) %.2g %.2g %.2g %.2g\n",
		    pfc(0), pfc(1), pfc(2), pfc(3), pfc(4), pfc(5), pfc(6));
	    printf ("\
  (8) %.2g %.2g %.2g %.2g %.2g %.2g %.2g %.2g (edges) %.2g\n",
		    pfc(7), pfc(8), pfc(9), pfc(10), pfc(11), pfc(12),
		    pfc(13), pfc(14), pfc(15));

	}
	for (i=0;i<EPIA_DEBUG_NFILES;i++) {
	    int limit;
	    int j;
	    /* index zero gives the max in use for each file */
	    for (limit=MIN(epia_codeBlock[i][0], EPIA_MAX_FILE_CODE_BLOCKS);
		 limit>0 && (epia_codeBlock[i][limit] == 0);
		 limit--);
	    if (limit <= 0) continue;
	    printf ("Code coverage for %s:\n", epia_debugFilename[i]);
	    insertNewline = 1;
	    for (j=1; j<=limit; j++) {
		if (insertNewline == 1) insertNewline=0, printf (" %2d:", j);
		printf (" %2d", epia_codeBlock[i][j]);
		if ((j % 20) == 19) insertNewline=1, printf ("\n");
	    }
	    if (insertNewline == 0) printf ("\n");
	}
	for (i=0; i<US_MAX_MINOR_DEV; i++) {
	    int reads, writes;
	    dev_t dev;

	    dev = osi_makedev(US_MAJOR_DEV, i);
	    reads = writes = 0;
	    us_ioctl (dev, EPIU_IOCTL_GET_READCOUNT, (caddr_t)&reads);
	    us_ioctl (dev, EPIU_IOCTL_GET_WRITECOUNT, (caddr_t)&writes);
	    if (reads || writes) {
		printf ("For minor dev %d: %d reads %d writes\n",
			i, reads, writes);
	    }
	}
#else
	printf ("No coverage info available\n");
#endif
    }
    /* else be quiet */
    return 0;
}

static long when = 0;

static long Sleep (
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    int count;
    int seconds;

    sscanf_r (count, (o->line, "%d", &seconds));
    if (count != 1) {
usage:
	com_err (o->whereami, 0, "Usage error: %s <seconds>", o->op);
	return (-1);
    }
    if (seconds < 0) {
	long now = time(0);
	if (when == 0) {
	    when = now-seconds;
	}
	if (when < now) {
	    printf ("Over shot join time by %d seconds.\n", now-when);
	    return 0;
	}
	seconds = when-now;
    }
    osi_Pause (seconds);
    return 0;
}

static long WaitAll(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    NO_ARGS;
    while (p->nChildren > 0) {
	code = epir_WaitSome();
	if (code) return code;
    }
    return 0;
}

static long ShutdownAll(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    NO_ARGS;
    if (code = ClosePreviousAggr (p, &p->cur)) return code;
    o->endProcessing = 1;
    if (p->parent)
	return 0;		/* sub-processes don't really finish */
    code = epir_WaitAll();
    if (code) return code;

    /* This should be the main process and all others should have exited by
     * now.  Do all unmounts and close all other devices. */

    UnmountFile (p, o);			/* notices it is called from "end" */

    for (p->cur.devi=0; p->cur.devi<US_MAX_MINOR_DEV; p->cur.devi++) {
	code = epig_InitAggregate (US_MAJOR_DEV, p->cur.devi, 1,
				   &p->cur.device, &p->cur.avl);
	if ((code == ASEVENT_NODEVICE) ||
	    (code == EPI_E_NOENT) ||
	    (code == EPI_E_NOTANAGGREGATE) ||
	    (code == ENODEV) ||
	    (code == ENXIO)) continue;
	if (code) {
	    com_err (o->whereami, code,
		     "checking for initialized aggregates");
	    return code;
	}
	if (code = ClosePreviousAggr (p, &p->cur)) return code;
    }
    Coverage (p, o);			/* dump coverage numbers if enabled */
    return 0;
}

static long AbortImmediately(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
/*    NO_ARGS; */
    Exit(129);				/* abort immediately */
    return 0;				/* quiet GCC */
}

static long Sync(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;

    NO_ARGS;
    if (code = GetCurrentDevice(p, &p->cur)) return code;

    if (useVM) {
	ObtainWVM();
	code = vm_Write (VM_WRITE, 0);
	if (code) {
	    com_err (p->whereami, code, "syncing vm");
	    return code;
	}
	ReleaseWVM();
    }
    elbb_SyncForce();
    return 0;
}

static long Quiesce(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    int count;
    int i;
    long code;
    char consist[4][32];		/* type of consistency */
    int commit;				/* parameter to quiesce */
    int header;				/* quiesce volume body or header? */

    sscanf_r (count, (o->line, "%31s %31s %31s %31s",
		      consist[0], consist[1], consist[2], consist[3]));
    if ((count < 1) || (count > 4)) {
usage:
	com_err (o->whereami, 0,
		 "Usage error: %s [header] {serial|commit|write|fsync}+",
		 o->op);
	return (-1);
    }

    commit = 0;
    header = 0;
    for (i=0; i<count; i++) {
	lcstring (consist[i], consist[i], sizeof(consist[0]));
	if (strncmp (consist[i], "serial", 6) == 0)
	    commit |= ELBB_QUIESCE_SERIALIZE;
	else if (strcmp (consist[i], "commit") == 0)
	    commit |= ELBB_QUIESCE_COMMIT;
	else if (strcmp (consist[i], "write") == 0)
	    commit |= ELBB_QUIESCE_WRITE;
	else if (strcmp (consist[i], "fsync") == 0)
	    commit |= ELBB_QUIESCE_FSYNC;
	else if (strcmp (consist[i], "header") == 0)
	    header++;
	else {
	    com_err (o->whereami, 0, "Unexpected consistency specifier: %s",
		     consist[i]);
	    goto usage;
	}
    }
    if (code = GetCurrentDevice(p, &p->cur)) return code;

    if (useVM && !header &&
	(commit & (ELBB_QUIESCE_WRITE|ELBB_QUIESCE_FSYNC))) {
	int force = ((commit & ELBB_QUIESCE_FSYNC) ? (VM_SYNC|VM_AWAIT) : 0);
	ObtainWVM();
	code = vm_Write (VM_WRITE|force, 0);
	if (code) {
	    com_err (p->whereami, code, "syncing vm");
	    return code;
	}
	ReleaseWVM();
    }

    if (header) {
	code = epiv_QuiesceHeader (p->cur.vol, commit);
	if (code) {
	    com_err (p->whereami, code, "quiescing header");
	    return code;
	}
    } else {
	elbb_QuiesceSystem (p->cur.device, commit);
    }
    return 0;
}

static long SetTracing(
  IN struct processData *p,
  IN struct epir_operationData *o)
{
    long code;
    int count;
    char state[10];
    int trace = 0;			/* new state of tracing */
    struct icl_set *setp = icl_FindSet("disk/lock");

    /* Try to remember the current state of tracing.  This isn't thread-safe
     * but should cut down on spurious messages. */
#ifdef OSI_TRACE_SYNC
    static traceOn = 1;
#else
    static traceOn = 0;
#endif

    sscanf_r (count, (o->line, "%9s", state));
    if (count != 1)
	goto usage;

    if (strcmp (state, "on") == 0)
	trace = 1;
    else if (strcmp (state, "off") == 0)
	trace = 0;
    else if (strcmp (state, "stay-off") == 0)
	trace = -1;			/* off and stay off */
    else goto usage;
    
    if (!setp) {
	traceOn = 0;
    } else if ((traceOn != -1) && (trace != traceOn)) {
	long code;
	traceOn = trace;
	code = icl_SetSetStat(setp, ((trace == 1) ? ICL_OP_SS_ACTIVATE
				     : ICL_OP_SS_DEACTIVATE));
	afsl_MBZ (code);
	icl_SetRele(setp);
	printf("%sabling sync tracing at %s.\n",
	       ((trace == 1) ? "En" :
		((trace == 0) ? "Dis" :
		 "Permanently dis")), o->whereami);
    }
    return 0;

usage:
    com_err (o->whereami, 0, "Usage error: %s {on | off}", o->op);
    return (-1);
}

/*
 * The procedures below are declared and defined in the individual packages
 * responsible for providing the information.
 */
extern int ins_anode_anode();
extern int ins_anode_file();
extern int ins_anode_volume();
extern int ins_anode_aggregate();
extern int ins_log();
#if 0
int ins_vnops();
extern int ins_dirbuf();
#endif
extern int ins_async();
#if 0
extern int ins_acl();
#endif
extern int ins_log_tune();

static int NullProc()
{
   return 0;
}

static int (*procTable[])() = {	/* module_submodule */
    ins_anode_anode,			/* index 0 */
    ins_anode_file,			/* index 1 */
    ins_anode_volume,			/* index 2 */
    ins_anode_aggregate,		/* index 3 */
    NullProc,				/* index 4 */
    NullProc,				/* index 5 */
    NullProc,				/* index 6 */
    NullProc,				/* index 7 */
    ins_log,				/* index 8 */
    ins_elbb,				/* index 9 */
    NullProc /*ins_vnops*/,		/* index 10 */
    NullProc /*ins_dirbuf*/,		/* index 11 */
    ins_async,				/* index 12 */
    NullProc /*ins_acl*/,		/* index 13 */
    ins_log_tune,			/* index 14 */
    ins_elbb_tune			/* index 15 */
    };

static long procArraySize = (sizeof (procTable)/sizeof(long));

static long EpiStat(p, o)
  IN struct processData *p;
  IN struct epir_operationData *o;
{
    int count;
    int error = 0;
    struct ins_varDesc vars;
    struct ins_opDesc opcode;
    struct ins_basicObject resultValues;
    struct ins_basicObjectId argObjIds, resultObjIds;

    bzero((char *)&resultValues, sizeof (resultValues));
    bzero((char *)&argObjIds, sizeof (argObjIds));
    bzero((char *)&resultObjIds, sizeof (resultObjIds));

    sscanf_r (count, (o->line, "%ld %ld %ld %ld",
		      &opcode.opcode, &vars.procIdx, &vars.varIdx,
		      (long *)resultValues.value));
    if (count < 3) {
	com_err(o->whereami, 0, "Usage error: %s <opcode> <procIdx> <varIdx>",
		o->op);
	return -1;
    }
    if (opcode.opcode != INS_SET &&
	opcode.opcode != INS_GET &&
	opcode.opcode != INS_GETNEXT) {
	com_err(o->whereami, 0, "Invalid opcode: %ld", opcode.opcode);
	return EINVAL;
    }
    opcode.count = 1;

    if (vars.procIdx > procArraySize - 1) {
	com_err(o->whereami, 0, "Invalid pocedure index: %ld", vars.procIdx);
	return EINVAL;
    }

    error = (*procTable[vars.procIdx])(&opcode, &vars, &argObjIds,
				       &resultObjIds, &resultValues);
    if (opcode.opcode == INS_GET || opcode.opcode == INS_GETNEXT) {
	printf("result %ld nextInstance %ld\n",
	       *(long *)resultValues.value,
	       *(long *)resultObjIds.objId.value);
    }

    return error;
}

static struct epir_operation anodeTestOps [] = {
    {"abort", AbortImmediately},
    {"aggr_info", AggregateInfo},
    {"bash_device", BashDevice},
    {"change_link", ChangeLink},
    {"check_device", CheckDevice},
    {"close_aggregate", CloseAggregate},
    {"coverage", Coverage},
    {"create_aggregate", CreateAggregate},
    {"create_dir", CreateFile},
    {"create_fid", CreateFile},
    {"create_file", CreateFile},
    {"create_volume", CreateVolume},
    {"delayed_write_file", WriteFile},
    {"delete_file", DeleteFile},
    {"delete_volume", DeleteVolume},
    {"end", ShutdownAll},
    {"epistat", EpiStat},
    {"exist_volume", CreateVolume},
    {"fork", ParallelTest},
    {"fsync", FsyncFile},
    {"gc_volume", GarbageCollectVolume},
    {"get_info", FileInfo},
    {"grow_aggregate", GrowAggregate},
    {"grow_device", GrowDevice},
    {"mkdir", MakeDir},
    {"modify_afsdb", ModifyAFSDB},
    {"mount_file", MountFile},
    {"open_device", OpenDevice},
    {"open_fid", OpenFile},
    {"open_file", OpenFile},
    {"open_volume", OpenVolume},
    {"pause", Pause},
    {"quiesce", Quiesce},
    {"read_aux", ReadAux},
    {"read_file", ReadFile},
    {"reclone_file", CloneFile},
    {"replace_aux", ReplaceAux},
    {"restore", SaveFileNames},
    {"salvage_device", SalvageDevice},
    {"save", SaveFileNames},
    {"set_status", SetStatus},
    {"set_tracing", SetTracing},
    {"set_transfer_size", SetTransferSize},
    {"set_withholding", SetWithholding},
    {"set_vm", SetVM},
    {"splice_fid", CreateFile},
    {"splice_file", CreateFile},
    {"sleep", Sleep},
    {"sync", Sync},
    {"truncate_file", TruncateFile},
    {"unbash_device", BashDevice},
    {"unclone_file", CloneFile},
    {"unique_time", TestUniqueTime},
    {"unmount_file", UnmountFile},
    {"vm", SetVM},
    {"vol_info", VolumeInfo},
    {"wait", WaitAll},
    {"write_aux", WriteAux},
    {"write_file", WriteFile},
};

static long InitProcessFromOp (p, o)
  IN struct processData *p;
  IN struct epir_operationData *o;
{
    p->testFile = o->testFile;
    strncpy(p->testName, o->testName, sizeof (p->testName));
    p->version = o->version;
    p->pid = o->pid;
    NewRandom (p, (o->procId));		/* diverge random number generators */
    return 0;
}

static long FinishProcess (p, o)
  IN struct processData *p;
  IN struct epir_operationData *o;
{
    if (p->parent) p->parent->nChildren--;
    return 0;
}

static long CopyOpToProcess (p, o)
  IN struct processData *p;
  IN struct epir_operationData *o;
{
    p->errorCode = o->errorCode;
    p->whereami = o->whereami;
    return 0;
}

/* looks like a CMA bug makes bogus sigaction definition */
#undef sigaction

void main (argc, argv)
  IN int   argc;
  IN char *argv[];
{
    long code;
    struct processData pData;
    struct epir_operationTable ops;
    pthread_t junk;
    struct sigaction oldAction;		/* original SIGHUP handling info */
    long maxHandles = 1000;		/* number of anode's to allocate */
    int a;
    int startAsync = 0;			/* start async daemon after LWP init */

    BTOOLSETUP
    for (a = 1; a < argc; a++) {
	int arglen = strlen(argv[a]);
	char arg[256];
	char *sp;
	int count = 0;
	int useA = 0;

/* test argument for match and bump count on each */
#define IsArg(a) ((strncmp (arg, a, arglen) == 0) && (++count == 1))

	if (arglen == 0) continue;      /* already processed */

	if (arglen >= sizeof (arg)) {
	    fprintf(stderr, "argument too long\n");
	    goto usage;
	}
	if (arglen == 1)
	    continue;			/* process bare '-' later */

	lcstring (arg, argv[a], sizeof (arg));

	if (IsArg("-maxhandles")) {
	    if (a + 1 == argc)
		goto usage;
	    maxHandles = strtoul(argv[a + 1], &sp, 0);
	    if (*sp) goto usage;
	    useA = 1;
	}
	if (IsArg("-usevm")) {
	    useVM=1;
	    startAsync = 1;
	}
	if (IsArg("-usesavm")) {
	    /* stand alone VM: no async daemon */
	    useVM=1;
	    startAsync = 0;
	}
	if (count > 1) {
	    fprintf (stderr, "ambiguous argument '%s'\n", arg);
	    goto usage;
	}
	if (count == 1) argv[a][0] = 0;	/* don't process this later */
	if (useA) argv[++a][0] = 0;	/*  ditto  */
    }

    if (useVM && !os_supports_vm ) {
	useVM = 0;
	printf("%s: system does not support VM, hence -usevm ignored.\n",
	       whoami);
    }
    {
	/* Disable aggregate free space withholding for now.  It only affects
         * VM which we aren't testing now. */

	extern u_long epib_mostWithheld;
	epib_mostWithheld = 0;
    }

    {   struct sigaction act;
	act.sa_handler = (void (*)(int))AbortImmediately;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;
	code = sigaction (SIGHUP, &act, &oldAction);
	if (code) {
	    perror ("calling sigaction");
	    exit (1);
	}
    }

    {
	struct epig_params epigParams;

#define ONE_MEG (1024*1024)
	epigParams.elbbParams.bufferPool = (char *) osi_Alloc (ONE_MEG);
	epigParams.elbbParams.byteCount = ONE_MEG;
	epigParams.maxHandles = maxHandles;
	if (code = epig_Init(&epigParams)) {
	    com_err (whoami, code, "during module initialization");
	    Exit (2);
	}
    }

    bzero ((char *)partition, sizeof (partition));
    lock_Init (&vm_lock);
    FIFO_INIT (&mountTable, offsetof(struct mountEntry, thread));

    ops.table = anodeTestOps;
    ops.nOperations = sizeof (anodeTestOps) / sizeof (anodeTestOps[0]);
    ops.sorted = 1;			/* statically... */
    ops.testProlog = InitProcessFromOp;
    ops.testEpilog = FinishProcess;
    ops.opProlog = CopyOpToProcess;
    ops.opEpilog = 0;

    code = InitProcessData (0, &pData);
    if (code != 0)
	Exit(1);
    pData.ops = &ops;			/* for later recursion */

    /* Start up the user-space I/O simulator.  Also start asynchronous I/O
     * completer thread running at low priority */

    code = us_Init();
    if (code) {
	com_err (whoami, code, "Can't initialize user-space I/O");
	Exit (1);
    }
    {	pthread_t junk;
	code = epi_ThreadCreate(us_Complete, 4000, EPI_NORMAL_PRIORITY - 1,
				(void *)0, &junk);
	if (code) {
	    com_err (whoami, code, "Can't start user-space I/O completer");
	    Exit (1);
	}
    }

    vm_Init();
    if (startAsync) StartAsync();

    code = epir_StartTest (argc, argv, &ops, (opaque)&pData);
    printf("Test %s %s\n", pData.testName,
	(code == 0) ? "completed successfully" : "FAILED");

    /* restore SIGHUP handler to avoid recursive calls to Exit. */
    (void) sigaction (SIGHUP, &oldAction, NULL);

    Exit (code != 0);

usage:
    fprintf (stderr,
	"Usage is: '%s <run test args> [-maxhandles <n>] [-useVM]'\n",
	whoami);
    Exit (1);
}
