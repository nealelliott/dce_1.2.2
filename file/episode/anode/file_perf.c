/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: file_perf.c,v $
 * Revision 1.1.61.1  1996/10/02  17:17:25  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:27:14  damon]
 *
 * $EndLog$
 */
/* Copyright (C) 1990, 1994 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>		/* AIX: make sure _BSD is set */
#include <sys/param.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/file.h>
#include <sys/dir.h>

	/* Sun NFS Standard */
#include <sys/vfs.h>
#define STATFS struct statfs
#define FS_BTOT f_blocks
#define FS_BSIZE f_bsize
#define FS_BFREE f_bfree
#define STATFS_OK 0

#ifndef DST_USA
#include <sys/time.h>
#endif

#include <sys/resource.h>
#include <stdio.h>
#include <errno.h>

static char rcsid[] = "$Header: /u0/rcs_trees/dce/rcs/file/episode/anode/file_perf.c,v 1.1.61.1 1996/10/02 17:17:25 damon Exp $";

static char *whoami = "file_perf";

struct use {
    long time, cpu, ins, outs;
};

static struct {
    int size;				/* file size */
    int max;				/* number to create */
    char *prefix;			/* name prefix */
    int n;				/* number created so far */
    struct use read;			/* usage for reads */
    struct use write;			/*  ditto  writes */
    struct use delete;			/*  ditto  delete */
} limits[] =
#if DEBUG
   {{0, 10/*0*/, "z"},
    {1, 10/*0*/, "o"},
    {10, 10/*0*/, "t"},
    {100, 10/*0*/, "h"},
    {1000, 30/*0*/, "k"},
    {10000, 10/*0*/, "tk"},
    {100000, 10, "hk"},
    {1000000, 1/*3*/, "m"}};
#else
   {{0, 100, "z"},
    {1, 100, "o"},
    {10, 100, "t"},
    {100, 100, "h"},
    {1000, 300, "k"},
    {10000, 100, "tk"},
    {100000, 10, "hk"},
    {1000000, 3, "m"},
    {10000000, 1, "tm"}};
#endif

#define NLIMITS (sizeof(limits)/sizeof(limits[0]))

static struct {
    char *name;				/* name of directory */
    int percent;			/* fraction of files */
    int n;				/* number actually present */
    struct use stat;			/* time to stat contents */
    char *path;				/* full path of directory */
} dirs[] = {
    {"a", 50},
    {"b", 25},
    {"c", 20},
    {"d", 4},
    {"e", 1}};
#define NDIRS (sizeof(dirs)/sizeof(dirs[0]))

struct fileData { /* info for each file */
    int limit;				/* index of category */
    int state;				/* (e.g. created, deleted) */
    char path[1];			/* allocated big enough for pathname */
};
int *randomCreate;			/* order to create files in */
int *randomDelete;			/* order to delete files in */

/* time I/O to individual files this big or bigger */
#define FILESIZE_TO_TIME ((int)100e3)

int totalFiles;				/* total files to create */
int totalSize;				/* total size of all files */
int nFiles;				/* files created so far */
struct fileData **fileData;		/* osi_Alloc'd array of all files */

#define BUFFERSIZE (50*1024)		/* big buffer */
char buffer[BUFFERSIZE];

long blockSize;				/* block size of file system */
extern char *osi_Alloc();

/* All times stored in longs as microseconds */
#define TIME_UNIT 1e6

long TimeUsed (start, end)
  struct timeval *start, *end;
{
    int  sec = end->tv_sec - start->tv_sec;
    int usec = end->tv_usec - start->tv_usec;
    return sec*(int)TIME_UNIT + usec;
}

long CPUTimeUsed (start, end)
  struct rusage *start, *end;
{
    return TimeUsed (&start->ru_utime, &end->ru_utime) +
	   TimeUsed (&start->ru_stime, &end->ru_stime);
}

struct edge {
    struct timeval w;			/* wall clock */
    struct rusage u;			/* CPU and I/O usage */
};
struct record {
    struct edge start,end;
};

#if defined(AFS_AIX_ENV) || defined (AFS_HPUX_ENV)
#include <sys/times.h>
getrusage (flag, usage)
  int flag;
  struct rusage *usage;
{
    struct tms vt;

    times (&vt);
    usage->ru_utime.tv_sec = vt.tms_utime/HZ;
    usage->ru_utime.tv_usec = (vt.tms_utime % HZ) * 1000000 / HZ;
    usage->ru_stime.tv_sec = vt.tms_stime/HZ;
    usage->ru_stime.tv_usec = (vt.tms_stime % HZ) * 1000000 / HZ;
    usage->ru_inblock = 0;
    usage->ru_oublock = 0;
}
#endif

#define RECORD_EDGE(e) gettimeofday(&e.w,0), getrusage(RUSAGE_SELF,&e.u)
#define RECORD_SUM(use,r) \
    ((use).time += TimeUsed(&((r).start.w), &((r).end.w))), \
    ((use).cpu += CPUTimeUsed(&((r).start.u), &((r).end.u))), \
    ((use).ins += ((r).end.u.ru_inblock - (r).start.u.ru_inblock)), \
    ((use).outs += ((r).end.u.ru_oublock - (r).start.u.ru_oublock))

int PickADir ()
{
    int i;
    int r = random() % 100;
    for (i=0; i<NDIRS; i++) {
	if (r < dirs[i].percent) return i;
	r -= dirs[i].percent;
    }
    fprintf ("shouldn't get here\n");
    exit (3);
}

int CreateFile (i)
  int i;
{
    struct fileData *f = fileData[i];
    int fd;				/* file descriptor of new file */
    long code;
    int size;
    int n;
    int count;
    struct record r;

    fd = open (f->path, O_RDWR+O_CREAT+O_TRUNC, 0777);
    if (fd == -1) {
	perror ("creating file");
	return -1;
    }
    f->state = 1;
    limits[f->limit].n++;

    size = limits[f->limit].size;	/* target file size */
    if (size >= FILESIZE_TO_TIME) RECORD_EDGE (r.start);
    for (n = size; n>0; n -= count) {
	count = write (fd, buffer, ((n > BUFFERSIZE) ? BUFFERSIZE : n));
	if (count < 0) {
	    perror ("write fails");
	    break;			/* defer failure */
	}
    }
    if (size >= FILESIZE_TO_TIME) {
	RECORD_EDGE(r.end);
	RECORD_SUM (limits[f->limit].write, r);
    }

    code = close (fd);
    if (code) {
	perror ("failed to close");
	return -1;
    }
    return (count < 0);			/* if write failed */
}

int DeleteFile (i)
  int i;
{
    struct fileData *f = fileData[i];
    int fd;				/* file descriptor of file */
    long code;
    int size;
    int n;
    int count;
    struct record r;

    if (f->state != 1) return 0;	/* this file skipped */
    fd = open (f->path, O_RDWR, 0);
    if (fd == -1) {
	fprintf (stderr, "error opening %s\n", f->path);
	perror ("opening for read file");
	return -1;
    }
    f->state = 2;			/* reading */

    size = limits[f->limit].size;	/* target file size */
    if (size >= FILESIZE_TO_TIME) RECORD_EDGE (r.start);
    for (n = size; n>0; n -= count) {
	count = read (fd, buffer, BUFFERSIZE);
	if (count < 0) {
	    perror ("read fails");
	    return count;
	}
	if (count == 0) {
	    fprintf (stderr, "premature eof reading %s\n", f->path);
	    break;
	}
    }
    if (n < 0) {
	fprintf (stderr, "read unexpected amount from %s\n", f->path);
	return -1;
    }
    if (size >= FILESIZE_TO_TIME) {
	RECORD_EDGE (r.end);
	RECORD_SUM (limits[f->limit].read, r);
	r.start = r.end;		/* start timing delete */
    }

    unlink (f->path);			/* since file is open this is no-op */
    code = close (fd);			/* actually do removal work here */
    if (code) perror ("failed to close");

    if (size >= FILESIZE_TO_TIME) {
	RECORD_EDGE (r.end);
	RECORD_SUM (limits[f->limit].delete, r);
    }
    return 0;
}

int StatDir (i)
  int i;
{
    DIR *d;
    struct direct *de;
    long code;
    char path[100];
    int dirlen;
    struct stat info;
    struct record r;

    RECORD_EDGE (r.start);

    strcpy (path, dirs[i].path);
    d = opendir(path);
    if (d == 0) {
	perror ("Can't open dir");
	return -1;
    }
    dirlen = strlen(path);
    path[dirlen++] = '/';
    while (de = readdir (d)) {
	path[dirlen] = 0;
	strcat (path, de->d_name);
	code = stat (path, &info);
	if (code) {
	    fprintf (stderr, "in stat'ing dir contents failed on %s\n", path);
	    perror ("stat dirent");
	}
    }
    closedir(d);

    RECORD_EDGE (r.end);
    RECORD_SUM (dirs[i].stat, r);

    return 0;
}

PrintUse (msg, u, n)
  char *msg;
  struct use *u;
  int n;
{
    double bytes = n;
    double rw, rc;
    char ios[2][16];

    if (msg) printf ("%s ", msg);
    if (n == 0) { /* just print out raw numbers */
	rw = u->time / TIME_UNIT;
	rc = u->cpu / TIME_UNIT;
	printf ("%.3g sec, %.3g cpu sec, I/O = %d/%d %d byte blocks\n",
		rw, rc, u->ins, u->outs, blockSize);
    } else if (n < 1000) { /* n isn't bytes */
	/* wall clock and cpu rates in files/second */
	rw = bytes / (u->time / TIME_UNIT);
	rc = bytes / (u->cpu / TIME_UNIT);
	/* Kbytes per block I/O */
	strcpy (ios[0], "0");
	strcpy (ios[1], "");
	if (u->ins) sprintf (ios[0],"%.3g", bytes / u->ins);
	if (u->outs) sprintf (ios[1],"%d WRITES", u->outs);
	printf ("%.3g files/sec, %.3g f/cpu, I/O %s f/blk %s\n",
		rw, rc, ios[0], ios[1]);
    }
    else {
	/* wall clock rates in Kilobytes/second */
	rw = (bytes / 1e3) / (u->time / TIME_UNIT);
	/* cpu time rates in Megabytes/second */
	rc = (bytes / 1e6) / (u->cpu / TIME_UNIT);
	/* Kbytes per block I/O */
	strcpy (ios[1], strcpy (ios[0], "0"));
	if (u->ins) sprintf (ios[0],"%.3g", (bytes / blockSize) / u->ins);
	if (u->outs) sprintf (ios[1],"%.3g", (bytes / blockSize) / u->outs);
	/* bf => block fraction: (data read in blocks) / (block reads) */
	printf ("%.3g Kb/sec, %.3g Mb/cpuSec, I/O %s/%s bf\n",
		rw, rc, ios[0], ios[1]);
    }
}

/* This procedure was taken from afs/src/vol/vol-salvage.c */
#define assert(a)

/* Return the name of the indicated block device */
char * DevName(dev)
  unsigned int dev;
{
#ifdef	AFS_SUNOS5_ENV
    struct dirent *dp;
#else
    struct direct *dp;
#endif
    static char name[32];
    DIR *dirp;
    int rootLen;
#ifdef AFS_HPUX_ENV
    strcpy(name, "/dev/dsk");
#else
    strcpy(name, "/dev");
#endif /* AFS_HPUX_ENV */
    dirp = opendir(name);
    assert(dirp != NULL);
    rootLen = strlen(name);
    name[rootLen++] = '/';
    while (dp=readdir(dirp)) {
	struct stat status;
	name[rootLen]=0;
	strcat (name, dp->d_name);
	if (stat (name, &status)) perror ("can't stat device");
	if ((status.st_mode & S_IFMT) == S_IFBLK &&
	    (status.st_rdev == dev)) {
		closedir(dirp);
		return name;
	    }
    }
    closedir(dirp);
    return NULL;
}

main(argc, argv)
  int   argc;
  char *argv[];
{
    long code;
    int i,j;
    char *startPath;
    char *outPath;
    int   forkCopies;			/* fork to execute parallel tests */
    struct record r;			/* record whole process */
    struct record cr, sr, dr;		/* record creates, stats and deletes */
    struct use u;
    STATFS fsinfo;
    struct stat finfo;
    char *devName;
    double startFreePercentage;		/* file system free at beginning */
    double minFreePercentage;		/* file system free at peak usage */

    startPath = 0;
    outPath = 0;
    forkCopies = 0;
    /* parse arguments */
    i = 1;
    while (i<argc) {
	int arglen = strlen(argv[i]);
	char arg[256];
	lcstring (arg, argv[i], sizeof(arg));
#define IsArg(a) (strncmp (arg,a, arglen) == 0)
	if (IsArg("-startpath")) startPath = argv[++i];
	else if (IsArg("-output")) outPath = argv[++i];
	else if (IsArg("-fork")) forkCopies = atoi(argv[++i]);
	else if ((arg[0] != '-') && (startPath == 0)) startPath = argv[i];
	else {
	    fprintf (stderr, "unexpected arg '%s'\n", arg);
	  usage:
	    fprintf (stderr, "Usage is: '%s [[-startPath <path>] [-output <path>] [-fork <# copies>]\n", whoami);
	    exit (1);
	}
	i++;
    }
    /* do defaults */
    if (startPath == 0) startPath = ".";

    if (outPath) {
	if (! freopen (outPath, "w+", stdout)) {
	    fprintf (stderr, "Failed to reopen stdout to %s\n", outPath);
	    perror ("reopen stdout");
	    exit(6);
	}
	fprintf (stderr, "Redirecting output to %s\n", outPath);
    }

    time(&code);
    printf ("%s", ctime(&code));
    code = stat (startPath, &finfo);
    if (code) {
	fprintf (stderr, "stat call failed on %s\n", startPath);
	perror ("stat'ing start path");
	exit (4);
    }
    devName = DevName(finfo.st_dev);
    if (devName == 0) {
	fprintf (stderr, "Can't get device name of %0.4x\n", finfo.st_dev);
	devName = "unknown";
    }
    code = statfs (startPath, &fsinfo);
    if (code != STATFS_OK) {
	fprintf (stderr, "statfs call failed on %s\n", startPath);
	perror ("statfs'ing start path");
	exit (4);
    }
    blockSize = fsinfo.FS_BSIZE;
    startFreePercentage = (double)fsinfo.FS_BFREE / (double)fsinfo.FS_BTOT;

    fflush (stdout);
    system ("uptime");

    for (i=0; i<NDIRS; i++) {
	int len = strlen(startPath) + strlen(dirs[i].name) + 3;
	int firstTime = 1;
	dirs[i].path = (char *)osi_Alloc (len);
	sprintf (dirs[i].path, "%s/%s", startPath, dirs[i].name);
      mkdir_again:
	code = mkdir (dirs[i].path, 0777);
	if (code) {
	    if (firstTime && (errno == EEXIST)) {
		char temp [100];
		fprintf (stderr,
			 "Directory %s exists: deleting it...", dirs[i].path);
		fflush (stderr);
		fflush (stdout);
		sprintf (temp, "rm -rf %s", dirs[i].path);
		code = system (temp);
		if (code) {
		    fprintf (stderr, "failed code=%d\n", code);
		    exit (2);
		}
		fprintf (stderr, "done.\n");
		firstTime = 0;
		goto mkdir_again;
	    }
	    perror ("making dirs");
	    exit (2);
	}
	dirs[i].n = 0;
	bzero (&dirs[i].stat, sizeof(struct use));
    }

    srandom(1);
    nFiles = totalSize = totalFiles = 0;
    for (i=0; i<NLIMITS; i++) {
	totalFiles += limits[i].max;
	totalSize += limits[i].size*limits[i].max;
	limits[i].n = 0;
	bzero (&limits[i].read, sizeof (struct use));
	bzero (&limits[i].write, sizeof (struct use));
	bzero (&limits[i].delete, sizeof (struct use));
    }
    fileData = (struct fileData **)
	osi_Alloc (totalFiles * sizeof(struct fileData *));
    for (i=0; i<NLIMITS; i++) {
	int n = limits[i].max;
	for (j=0; j<n; j++) {
	    char path[100];
	    struct fileData *f;
	    int k = PickADir ();
	    sprintf (path, "%s/%s%d", dirs[k].path, limits[i].prefix, j);
	    dirs[k].n++;
	    f = (struct fileData *)
		osi_Alloc (sizeof(struct fileData) + strlen (path));
	    f->limit = i;
	    f->state = 0;
	    strcpy (f->path, path);
	    fileData[nFiles++] = f;
	}
    }

    /* create two random sequences in for creating, then deleting, the files */
    randomCreate = (int *) osi_Alloc (totalFiles * sizeof(int));
    randomDelete = (int *) osi_Alloc (totalFiles * sizeof(int));
    for (i=0; i<totalFiles; i++) randomCreate[i] = randomDelete[i] = i;
    for (i=0; i<totalFiles-1; i++) {
	int temp, r;
	r = random() % (totalFiles-i);
	temp = randomCreate[i];
	randomCreate[i] = randomCreate[i+r];
	randomCreate[i+r] = temp;
	r = random() % (totalFiles-i);
	temp = randomDelete[i];
	randomDelete[i] = randomDelete[i+r];
	randomDelete[i+r] = temp;
    }

    for (i=0; i<BUFFERSIZE; i++)
	buffer[i] = -1;

    printf ("  %d files totalling %g Mbytes\n",
	    totalFiles, (double)totalSize / 1e6);

    RECORD_EDGE (r.start);
    cr.start = r.start;

    for (i=0; i<totalFiles; i++) {
	code = CreateFile (randomCreate[i]);
	if (code) break;
    }

    RECORD_EDGE (cr.end);
    sr.start = cr.end;

    for (i=0; i<NDIRS; i++)
	if (StatDir (i)) break;

    RECORD_EDGE (sr.end);
    dr.start = sr.end;

    statfs (startPath, &fsinfo);
    minFreePercentage = (double)fsinfo.FS_BFREE / (double)fsinfo.FS_BTOT;

    for (i=0; i<totalFiles; i++)
	if (DeleteFile (randomDelete[i])) break;

    RECORD_EDGE (r.end);
    dr.end = r.end;

    /* delete dirs */
    for (i=0; i<NDIRS; i++) {
	code = rmdir (dirs[i].path);
	if (code) perror ("removing dirs");
    }

    printf ("  File system on %s was initially %.3g%% free, min was %.3g%%\n",
	    devName, startFreePercentage*100.0, minFreePercentage*100.0);
    bzero (&u, sizeof(u));
    RECORD_SUM (u, cr);
    PrintUse ("Creates", &u, 0);
    bzero (&u, sizeof(u));
    RECORD_SUM (u, sr);
    PrintUse ("Stats", &u, 0);
    bzero (&u, sizeof(u));
    RECORD_SUM (u, dr);
    PrintUse ("Deletes", &u, 0);
    for (i=0; i<NLIMITS; i++) if (limits[i].size >= FILESIZE_TO_TIME) {
	int bytes = limits[i].n * limits[i].size;
	printf ("  %d Files w/ %.4g bytes:\n",
		limits[i].n, (double)limits[i].size);
	PrintUse ("    reads", &limits[i].read, bytes);
	PrintUse ("    writes", &limits[i].write, bytes);
	PrintUse ("    deletes", &limits[i].delete, bytes);
    }
    for (i=0; i<NDIRS; i++) {
	printf ("stat of %s, %d files ", dirs[i].path, dirs[i].n);
	PrintUse (0, &dirs[i].stat, dirs[i].n);
    }

    fflush (stdout);
    system ("uptime");

    exit (0);
}
