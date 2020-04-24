/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: file_stats.c,v $
 * Revision 1.1.72.1  1996/10/02  17:17:26  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:27:15  damon]
 *
 * Revision 1.1.66.2  1994/06/09  13:57:43  annie
 * 	fixed copyright in src/file
 * 	[1994/06/08  21:32:54  annie]
 * 
 * Revision 1.1.66.1  1994/02/04  20:11:31  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:09:17  devsrc]
 * 
 * Revision 1.1.64.1  1993/12/07  17:17:13  jaffe
 * 	1.0.3a update from Transarc
 * 	[1993/12/03  13:42:40  jaffe]
 * 
 * Revision 1.1.5.4  1993/01/18  22:04:05  cjd
 * 	embedded copyright notice
 * 	[1993/01/18  21:43:42  cjd]
 * 
 * Revision 1.1.5.3  1992/11/24  16:20:43  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  16:18:37  bolinger]
 * 
 * Revision 1.1.5.2  1992/08/31  18:40:37  jaffe
 * 	Transarc delta: mason-add-hp800-osi-routines 1.15
 * 	  Selected comments:
 * 	    This delta is misnamed - these are really porting fixes.
 * 	    Need to checkpoint to fix a different bug.
 * 	    Checkpoint the first system where test_anode works.  Only one change is not
 * 	    included here - the link line in anode/Makefile uses -lbsd.  On HP-UX, this
 * 	    is -lBSD.  This should be fixed more generally.
 * 	    More HP-UX changes.
 * 	    More HP-UX related changes.
 * 	    Fix compile problem on the RIOS.
 * 	    Fix more compilation problems.
 * 	    Checkpoint.
 * 	    Checpoint.
 * 	    In this snap we turned all malloc() calls into osi_Alloc calls.
 * 	    Also turned free calls into osi_Free.
 * 	    s/malloc/osi_Alloc/
 * 	    HP-UX EOF detection fix.
 * 	    Another checkpoint.
 * 	    This is a checkpoint of the first working HP-UX Episode system.
 * 	    Needed to add the vnops routines specific to HP-UX.
 * 	    Fix compilation problems.
 * 	[1992/08/30  01:51:00  jaffe]
 * 
 * Revision 1.1.3.5  1992/05/05  20:42:08  jdp
 * 	delta jdp-epimerge-1.0-to-1.0.1b9-into-2.17 1.20
 * 	[1992/05/04  20:25:46  jdp]
 * 
 * Revision 1.1.3.4  1992/01/25  20:45:56  zeliff
 * 	dfs6.3 merge, part2
 * 	[1992/01/25  20:02:52  zeliff]
 * 
 * $EndLog$
 */
/* Copyright (C) 1990 Transarc Corporation - All rights reserved */

#include <dcedfs/param.h>
#include <dcedfs/stds.h>
#include <sys/types.h>
#include <sys/dir.h>
#include <ctype.h>
#include <errno.h>
#include <stdio.h>
#include <math.h>
#include <dcedfs/ioctl.h>
#include <dcedfs/afsint.h>
#include <com_err.h>


#ifndef DST_USA
#include <sys/time.h>
#endif

#ifndef M_LN2
/* these don't exist on the RT */
#define M_LN2	0.69314718055994530942
#define M_E	2.7182818284590452354
#define M_SQRT2	1.41421356237309504880
#endif

static char *whoami;
static int   verbose = 1;
static FILE *statsFILE = 0;		/* stats file */

#define MAXNAME 100
#define	MAXSIZE	2048

static char space[MAXSIZE];

static struct volStats {
    char name[32];			/* volume name */
    unsigned long date;			/* date data gathered */
    int someSkipped;			/* some dirs skipped due to access */
    float percentFound;		/* ratio of blocks found to quota */
    int dirCount;			/* for each type: count, size, blks */
    long dirSize;
    long dirBlocks;
    int dirHist[20];
    int fileCount;
    long fileSize;
    long fileBlocks;
    int fileHist[40];
    int linkCount;
    long linkSize;
    long linkBlocks;
    int linkHist[15];
} *volStats;				/* current vol */
static struct volStats **vols;		/* array of all vol stats */
static int maxVols;			/* alloc'ed size of vols array */
static int nVols;			/* current number of volumes */

#define MAXBUCKET(field) (sizeof(volStats->field)/sizeof(int) - 1)

/* The definition of the contents of each bucket is as follows:
   0      <= b[0] <= sqrt(2)
   sqrt(2) < b[1] <= 2
   2       < b[2] <= sqrt(8)
   sqrt(8) < b[3] <= 4
   ... etc... */

static int Bucket (value, max)
  double value;
{   int bucket;
    if (value <= 1.0) bucket = 0;
    else {
	bucket = (int)(2.0*log(value)/M_LN2 - 1.0e-5);
	if (bucket >= max) bucket = max;
    }
    return bucket;
}

#define BUMP(field,value) \
  volStats->field[Bucket((double)(value), MAXBUCKET(field))]++

static int TestBuckets()
{
    struct volStats vs;
    int i;
    static int answer[20] =
	{2,  1, 0, 2,  1,   3,     3,          4, 0,0,0,0,0,0,0,0,0,0,0,0};
     /* 0,1, 2,   3,4, 5, 6,7,8, 9,10,11, 12,13,14,15 */
    if ((Bucket(0.0, 5) != 0) ||
	(Bucket(0.5, 5) != 0) ||
	(Bucket(1.0, 5) != 0) ||
	(Bucket(1.4, 5) != 0) ||
	(Bucket(1.5, 5) != 1) ||
	(Bucket(2.0, 5) != 1) ||
	(Bucket(2.8, 5) != 2) ||
	(Bucket(2.9, 5) != 3) ||
	(Bucket(4.0, 5) != 3) ||
	(Bucket(5.6, 5) != 4) ||
	(Bucket(7.9, 5) != 5) ||
	(Bucket(8.0, 5) != 5) ||
	(Bucket(8.1, 5) != 5)) return -1;
    bzero (&vs, sizeof(vs));
    volStats = &vs;
    for (i=0; i<16; i++) {
	BUMP(dirHist,i);
    }
    if (verbose > 1) {
	printf ("hist of 0-15:");
	for (i=0; i<MAXBUCKET(dirHist); i++)
	    printf (" %d", vs.dirHist[i]);
	printf ("\n");
    }
    if (bcmp (answer, vs.dirHist) != 0) return -1;
    return 0;
}

static int   permErrors = 0;		/* too prevent copious error output */
	
static void AddPermissionDenied (path)
  char *path;
{
    if (verbose) {
	if (permErrors < 2) {
	    fprintf (stderr,
		     "skipping %s because of insufficient access\n", path);
	} else if (permErrors == 2) {
	    fprintf (stderr,
		     "...omitting further error messages for this directory\n");
	}
    }
    permErrors++;
    volStats->someSkipped++;
}

#if defined(AFS_AIX_ENV)
#define ST_BLOCKS(s) (s->st_size/512);
#else
#define ST_BLOCKS(s) (s->st_blocks)
#endif

static void AddDirStats (path, info)
  char *path;
  struct stat *info;
{
    long size = info->st_size;
    long blks = ST_BLOCKS(info);

    if (verbose > 1) {
	fprintf (stderr, "Dir %s is %d bytes in %d blocks\n",
		 path, size, blks);
    }
    volStats->dirCount++;
    volStats->dirSize += size;
    volStats->dirBlocks += blks;
    BUMP (dirHist, size/2048.0);
}

static void AddFileStats (path, info)
  char *path;
  struct stat *info;
{
    long size = info->st_size;
    long blks = ST_BLOCKS(info);

    if (verbose > 2) {
	fprintf (stderr, "File %s is %d bytes in %d blocks\n",
		 path, size, blks);
    }
    volStats->fileCount++;
    volStats->fileSize += size;
    volStats->fileBlocks += blks;
    BUMP (fileHist, size/32.0);
}

static void AddLinkStats (path, info)
  char *path;
  struct stat *info;
{
    long size = info->st_size;
    long blks = ST_BLOCKS(info);

    if (verbose > 2) {
	fprintf (stderr, "Link %s is %d bytes in %d blocks\n",
		 path, size, blks);
    }
    volStats->linkCount++;
    volStats->linkSize += size;
    volStats->linkBlocks += blks;
    BUMP (linkHist, size);
}

static long ProcessEntry (path)
  char *path;
{
    long code;
    struct stat info;
    int type;

    code = lstat (path, &info);
    if (code) {
	if (errno == EACCES) {
	    AddPermissionDenied (path);
	}
	else com_err (whoami, errno, "Skipping %s", path);
	return 0;
    }
    type = info.st_mode & S_IFMT;
    if (type == S_IFDIR) {
	/* If AFS directory appears to have an even inode number then if is
         * really a mount point since the inode number refers to the funny
         * symlink that is the mount point not the volume root directory. */
	if ((info.st_ino & 1) == 0) return 0; /* don't cross mount points */
	AddDirStats (path, &info);
	return ScanDir (path);
    }
    else if (type == S_IFLNK) {
	AddLinkStats (path, &info);
    }
    else if (type == S_IFREG) {
	AddFileStats (path, &info);
    }
    else {
	fprintf (stderr, "skipping %s with funny type %07o\n", path, type);
	return 0;
    }
    return 0;
}

static int ScanDir (path)
  char *path;
{
    long code;
    DIR *d;
    struct direct *de;
    int pathLen;
    
    permErrors = 0;			/* allow 2 per directory */
    pathLen = strlen (path);
    d = opendir(path);
    if (d == 0) {
	if (errno == EACCES) {
	    AddPermissionDenied (path);
	    return 0;
	}
	return errno;
    }
    while (errno = 0, de = readdir(d)) {
	char newPath[256];
	if (de->d_namlen == 0) {
	    fprintf (stderr, "in dir %s entry w/ zero length name!!!\n", path);
	    continue;
	}
	if ((strcmp(de->d_name, ".") == 0) ||
	    (strcmp(de->d_name, "..") == 0)) continue;
	if (pathLen + 1 + de->d_namlen >= sizeof(newPath)) {
	    fprintf (stderr,
		     "pathname would be too long: %s/%s\n", path, de->d_name);
	    return 1;
	}
	sprintf (newPath, "%s/%s", path, de->d_name);
	code = ProcessEntry (newPath);
	if (code) return code;
    }
    if (errno) {			/* check for readdir failure */
	closedir (d);
	return errno;
    }
  done:
    closedir (d);
    return 0;
}

static long AppendVolStats (f)
  FILE *f;
{
    long  code;
    int   i;

    if (!f) return 0;
    fprintf (f, "%s %d %.1f%%",
	     volStats->name, volStats->date, volStats->percentFound);
    fprintf (f, "\n D");
    for (i=0; i<=MAXBUCKET(dirHist); i++)
	fprintf (f, " %d", volStats->dirHist[i]);
    fprintf (f, "\n F");
    for (i=0; i<=MAXBUCKET(fileHist); i++)
	fprintf (f, " %d", volStats->fileHist[i]);
    fprintf (f, "\n L");
    for (i=0; i<=MAXBUCKET(linkHist); i++)
	fprintf (f, " %d", volStats->linkHist[i]);
    fprintf (f, "\n");
    code = fflush (f);
    return code;
}

static long ProcessVolumes (path, nPaths, volRootLinks)
  char **path;				/* array of paths of volumes roots */
  int   nPaths;
  int   volRootLinks;			/* use stat instead of lstat on root */
{
    int   i;
    long  code;
    char *name, *offmsg, *motd;
    struct afs_ioctl blob;
    struct VolumeStatus *status;
    int   namelen;
    struct stat info;
    int type;
    long totalSize, totalBlocks;

    for (i=0; i<nPaths; i++) {
	blob.out_size = MAXSIZE;
	blob.in_size = 0;
	blob.out = space;
	code = pioctl(path[i], VIOCGETVOLSTAT, &blob, 1);
	if (code) {
	    com_err (whoami, errno, "getting volume status for %s", path[i]);
	    continue;
	}
	status = (VolumeStatus *)space;
	name = (char *)status + sizeof(*status);
	offmsg = name + (namelen = strlen(name)) + 1;
	motd = offmsg + strlen(offmsg) + 1;

	/* initialize global statistics structure */
	volStats = (struct volStats *)osi_Alloc (sizeof(*volStats));
	bzero (volStats, sizeof(*volStats));

	if (namelen >= sizeof (volStats->name)) {
	    fprintf (stderr,
		     "volume name %s is too long, truncated to %d chars\n",
		     name, sizeof(volStats->name));
	    bcopy (volStats->name, name, sizeof(volStats->name));
	    volStats->name[sizeof(volStats->name)-1] = 0; /* terminate */
	}
	else strcpy (volStats->name, name);

	if (volRootLinks) code = stat (path[i], &info);
	else code = lstat (path[i], &info);
	if (code) {
	    com_err (whoami, errno, "stat'ing volume root %s", path[i]);
	  skip:
	    osi_Free (volStats, sizeof(*volStats));
	    continue;
	}
	type = info.st_mode & S_IFMT;
	/* Use special hack to detect AFS mount points */
	if ((type != S_IFDIR) || ((info.st_ino & 1) != 0)) {
	    fprintf (stderr, "%s is not root of a volume\n", path[i]);
	    goto skip;
	}
	AddDirStats (path[i], &info);
	code = ScanDir (path[i]);
	if (code) goto skip;

	volStats->date = time(0);
	totalBlocks = volStats->dirBlocks + volStats->fileBlocks +
	    volStats->linkBlocks;
	totalBlocks *= 512;		/* convert to bytes */
	volStats->percentFound =
	    100.0*totalBlocks/(status->BlocksInUse*1024);

	code = AppendVolStats (statsFILE);
	if (code) {
	    com_err (whoami, code, "couldn't append stats");
	    continue;
	}

	if (verbose) {
	    printf ("In volume %s:\n", volStats->name);
	    printf ("  %d dirs, %d bytes total in %d blocks\n",
		    volStats->dirCount, volStats->dirSize,
		    volStats->dirBlocks);
	    printf ("  %d files, %d bytes total in %d blocks\n",
		    volStats->fileCount, volStats->fileSize,
		    volStats->fileBlocks);
	    printf ("  %d links, %d bytes total in %d blocks\n",
		    volStats->linkCount, volStats->linkSize,
		    volStats->linkBlocks);
	    totalSize =
		volStats->dirSize + volStats->fileSize + volStats->linkSize;
	    printf ("  volume totals %d bytes (%.1f%% utilization); blocks in use is %d (found %.1f%%)\n",
		    totalSize, 100.0*totalSize/totalBlocks,
		    status->BlocksInUse, volStats->percentFound);
	}

	if (nVols >= maxVols) {
	    maxVols += 10;
	    vols = (struct volStats **)
		(vols ? realloc (vols, maxVols*sizeof(char *))
		 : osi_Alloc (maxVols*sizeof(char *)));
	}
	vols[nVols++] = volStats;
    }
    return 0;
}

static void FloatToKM (s, f)
  char *s;
  double f;
{
    f += 0.0001;			/* we expect mostly round numbers */
    if (f < 1024.0) sprintf (s, "%.3g", f);
    else if (f < 1024.0*1024.0) sprintf (s, "%.3gK", f/1024.0);
    else sprintf (s, "%.3gM", f/(1024.0*1024.0));
}

static void PrintHist (f, hist, num, min, seq)
  FILE *f;
  int   hist[];
  int   num;				/* number of buckets */
  int   min;				/* min value of X coordinate */
  int   seq;				/* seqence of plots */
{
    int i;
    double multiplier;
    double firstBucket, lastBucket;
    char fb[20], lb[20];
    int indent;
    char *screen;
    char *letter;
    char *name;
    int   mode = 0;			/* find mode of curve */
    int   modei = -1;
    int   total = 0;

    if (min) multiplier = (double)min;
    else multiplier = 1.0;
    firstBucket = multiplier*exp(0.5*M_LN2),
    lastBucket = multiplier*exp(0.5*(num-1)*M_LN2);
    FloatToKM (fb, firstBucket);
    FloatToKM (lb, lastBucket);

    indent = (seq-1)*80/3;
    if (seq == 1) screen = "";
    else screen = "-s";

    switch (seq) {
      case 1:
	letter = "D";
	name = "dirs";
	break;
      case 2:
	letter = "F", name = "files";
	break;
      case 3:
	letter = "L", name = "links";
	break;
      default:
	fprintf (stderr, "Unexpected sequence number %d\n", seq);
    }

    fprintf (f, "graph -l \"%*s %s <%s >%s\" -c %s %s -g 0<<EOF|plot -T4014\n",
	     indent, "", name, fb, lb, letter, screen);
    for (i=0; i<=num; i++) {
	total += hist[i];
	fprintf (f, "%d %d\n", i, hist[i]);
	if (hist[i] > mode) mode = hist[i], modei = i;
    }
    fprintf (f, "EOF\n");
    if (modei >= 0) {
	double f;
	char l[20], u[20];
	double eFold = 1.0/M_E;
	double cutOff;
	int soFar = 0;

	f = multiplier*exp(0.5*(modei)*M_LN2);
	FloatToKM (l, f);
	FloatToKM (u, f*M_SQRT2);
	printf ("For %s (%s < mode <= %s)\n", name, l, u);
	cutOff = 1.0 - eFold;
	printf ("    percentiles ");
	for (i=0; i<num; i++) {
	    soFar += hist[i];
	    if (((double)soFar)/total > cutOff) {
		f = multiplier*exp(0.5*i*M_LN2);
		FloatToKM (l, f);
		FloatToKM (u, f*M_SQRT2);
		printf ("%.1f%% <= %s, ", cutOff*100.0, u);
		eFold /= M_E;
		cutOff = 1.0 - eFold;
		if (cutOff > 0.99) break;
	    }
	}
	printf ("\n");
    }
}

static long ReadVols (f)
  FILE *f;
{
    long  code;
    struct volStats vs;
    char *doing;
    char  letter[10];
    char  line[256];
#define READLINE if (!fgets (line, sizeof(line), f)) return -1
    int   i;

    while (!feof(f)) {
	char *l;
	READLINE;
	for (l=line; isspace(*l); l++);
	if (strlen (l) == 0) continue;
      scan_volume:
	code = sscanf (line, "%s %d %f", vs.name, &vs.date, &vs.percentFound);
	if (code != 3) {
	    doing = "volume line";
	  skip:
	    fprintf (stderr, "processing error in %s on '%s'\n", doing, line);
	    while (1) {
		READLINE;
		if (isalpha(line[0])) goto scan_volume;
	    }
	}
	doing = "dir hist";
	code = fscanf (f, "%s", letter);
	if ((code != 1) && (strcmp(letter, "D") != 0)) goto skip;
	for (i=0; i<=MAXBUCKET(dirHist); i++) {
	    code = fscanf (f, "%d", &vs.dirHist[i]);
	    if (code != 1) goto skip;
	}
	doing = "file hist";
	code = fscanf (f, "%s", letter);
	if ((code != 1) && (strcmp(letter, "D") != 0)) goto skip;
	for (i=0; i<=MAXBUCKET(fileHist); i++) {
	    code = fscanf (f, "%d", &vs.fileHist[i]);
	    if (code != 1) goto skip;
	}
	doing = "link hist";
	code = fscanf (f, "%s", letter);
	if ((code != 1) && (strcmp(letter, "D") != 0)) goto skip;
	for (i=0; i<=MAXBUCKET(linkHist); i++) {
	    code = fscanf (f, "%d", &vs.linkHist[i]);
	    if (code != 1) goto skip;
	}

	if (nVols >= maxVols) {
	    maxVols += 10;
	    vols = (struct volStats **)
		(vols ? realloc (vols, maxVols*sizeof(char *))
		 : osi_Alloc (maxVols*sizeof(char *)));
	}
	volStats = (struct volStats *)osi_Alloc (sizeof(*volStats));
	*volStats = vs;
	vols[nVols++] = volStats;
    }
    return 0;
}

static int SortVols (a, b)
  struct volStats **a, **b;
{
    int cmp = strcmp ((*a)->name, (*b)->name);
    if (cmp == 0) {			/* put highest percentage last */
	if ((*a)->percentFound < (*b)->percentFound) cmp = -1;
	else if ((*a)->percentFound > (*b)->percentFound) cmp = 1;
    }
    return cmp;
}

static int TotalVols (limit)
  int limit;
{
    int v,i;
    int volSkipped = 0;
    float foundLimit = (float)limit;

    volStats = (struct volStats *)osi_Alloc (sizeof(*volStats));
    bzero (volStats, sizeof(*volStats));
    volStats->percentFound = 0.0;
    strcpy (volStats->name, "volume totals");

    qsort(vols, nVols, sizeof(char *), SortVols);

    for (v=0; v<nVols; v++) {
	if (vols[v]->percentFound < foundLimit) {
	    volSkipped++;
	    continue;
	}
	/* skip duplicates; highest percentage is last */
	while ((v < nVols-1) &&
	       (strcmp (vols[v]->name, vols[v+1]->name) == 0)) {
	    if (verbose)
		printf ("Skipping duplicate volume %s w/ %g%%\n",
			vols[v]->name, vols[v]->percentFound);
	    v++;
	}

	volStats->someSkipped += vols[v]->someSkipped;
	volStats->percentFound += vols[v]->percentFound;
	volStats->dirCount += vols[v]->dirCount;
	volStats->dirSize += vols[v]->dirSize;
	volStats->dirBlocks += vols[v]->dirBlocks;
	for (i=0; i<=MAXBUCKET(dirHist); i++)
	    volStats->dirHist[i] += vols[v]->dirHist[i];
	volStats->fileCount += vols[v]->fileCount;
	volStats->fileSize += vols[v]->fileSize;
	volStats->fileBlocks += vols[v]->fileBlocks;
	for (i=0; i<=MAXBUCKET(fileHist); i++)
	    volStats->fileHist[i] += vols[v]->fileHist[i];
	volStats->linkCount += vols[v]->linkCount;
	volStats->linkSize += vols[v]->linkSize;
	volStats->linkBlocks += vols[v]->linkBlocks;
	for (i=0; i<=MAXBUCKET(linkHist); i++)
	    volStats->linkHist[i] += vols[v]->linkHist[i];
    }
    if (volSkipped >= nVols) {
	fprintf (stderr, "All %d volumes skipped because limit was %.0f%%\n",
		 nVols, foundLimit);
	return -1;
    }
    else if (volSkipped)
	fprintf (stderr,
		 "Skipped %d volumes below %.0f%%\n", volSkipped, foundLimit);
    volStats->percentFound /= (nVols-volSkipped); /* average percentage */
    printf ("Totaled %d volumes\n", nVols-volSkipped);
    return 0;
}

main (argc, argv)
  int   argc;
  char *argv[];
{
    int   i;
    long  code;
    char **path;			/* array of volume root pathnames */
    int   nPaths;
    char *plotfile = 0;			/* for graph | plot output */
    char *statsfile = 0;		/* accumulate volume statistics */
    FILE *pf;				/* plot file */
    int   foundLimit = 0;		/* limit volumes w/ incomplete stats */
    int   volRootLinks = 0;		/* follow links to find vol root */

    whoami = argv[0];
    /* Make room for all the pathnames */
    path = (char **) osi_Alloc (sizeof(char *)*argc);
    nPaths = 0;

    /* parse arguments */
    i = 1;
    while (i<argc) {
	int arglen = strlen(argv[i]);
	char arg[256];
	lcstring (arg, argv[i], sizeof(arg));
#define IsArg(a) (strncmp (arg,a, arglen) == 0)
	if (IsArg("-quiet")) ((verbose>0) ? verbose-- : 0);
	else if (IsArg("-verbose")) verbose++;
	else if (IsArg("-plotfile")) plotfile = argv[++i];
	else if (IsArg("-foundlimit")) foundLimit = atoi(argv[++i]);
	else if (IsArg("-statsfile")) statsfile = argv[++i];
	else if (IsArg("-lstat")) volRootLinks++;
	else if (argv[i][0] != '-')
	    path[nPaths++] = argv[i];	/* add this to list of pathnames */
	else {
	    fprintf (stderr, "unexpected arg '%s'\n", arg);
	  usage:
	    fprintf (stderr, "Usage is: '%s [-quiet] [-verbose] [-plotfile <path>] [-statsfile <path>]",
		     whoami);
	    fprintf (stderr, " [-foundlimit <percentage>] [-Lstat] <volume root pathname>*\n");
	    exit (1);
	}
	i++;
    }

    if (TestBuckets ()) {
	fprintf (stderr, "Test fails\n");
	exit (1);
    }
    
    if (plotfile) {
	pf = fopen (plotfile, "w");
	if (pf == 0) {
	    com_err (whoami, errno, "opening plot data to %s", plotfile);
	    exit (1);
	}
    }

    maxVols = nVols = 0;
    if (nPaths) {
	/* get room to keep all the volume info structures */
	maxVols = nPaths;
	vols = (struct volStats **) osi_Alloc (sizeof(char *)*maxVols);
    }

    if (statsfile) {
	statsFILE = fopen (statsfile, "r");
	if (statsFILE) {
	    /* try to read in old volumes */
	    code = ReadVols (statsFILE);
	    fclose (statsFILE);
	}
	if (nPaths) {
	    /* the append new entries, if there is at least one volume */
	    statsFILE = fopen (statsfile, "a");
	    if (statsFILE == 0) {
		com_err (whoami, errno,
			 "opening %s for appending statistics", statsfile);
		exit (1);
	    }
	}
	else statsFILE = 0;
    }
    
    code = ProcessVolumes (path, nPaths, volRootLinks);
    if (code) exit (1);

    TotalVols(foundLimit);		/* calculate totals for all volumes */

    if (plotfile) {
	PrintHist (pf, volStats->dirHist, MAXBUCKET(dirHist), 2048, 1);
	PrintHist (pf, volStats->fileHist, MAXBUCKET(fileHist), 32, 2);
	PrintHist (pf, volStats->linkHist, MAXBUCKET(linkHist), 0, 3);
	code = fclose (pf);
	if (code) com_err (whoami, errno, "closing plot file %s", plotfile);
    }

    if (statsFILE) {
	code = fclose (statsFILE);
	if (code) com_err (whoami, errno, "closing stats file %s", statsfile);
    }

    exit (0);
}
