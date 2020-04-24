/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * Copyright (C) Transarc Corporation 1995, 1990 -- All Rights Reserved.
 */
/*
 * HISTORY
 * $Log: afsd.c,v $
 * Revision 1.1.29.1  1996/10/02  16:56:54  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:03:22  damon]
 *
 * $EndLog$
 */
/*
 *       Cache Manager (AFS 4.0) Starter
 */

/*-----------------------------------------------------------------------------
  * Description:
  *	The AFS startup process.  It is responsible for reading and
  * parsing the various configuration files, starting up the kernel processes
  * required by AFS and feeding the configuration information to the kernel.
  *
  * Recognized flags are:
  *	-blocks	    The number of blocks available in the workstation cache.
  *			must be greater than 0
  *	-files	    The target number of files in the workstation cache
  *		    (Default: 1000).
  *			must be greater than 0
  *	-rootfileset The name of the root volume to use.
  *			must be a non empty string
  *	-stat	    The number of stat cache entries.
  *			must be greater than 0
  *	-memcache   Use an in-memory cache rather than disk.
  *	-cachedir   The base directory for the workstation cache.
  *	-mountdir   The directory on which the DFS is to be mounted.
  *	-mainprocs  The number of background daemons to start (Default: 2).
  *			must be greater than 0
  *	-settime    Keep comparing the local system time with server's (local
  *                 cell) to avoid drift.
  *	-verbose    Be chatty.
  *	-debug	    Print out additional debugging info.
  *	-chunk [n]  2^n is the chunksize to be used.  0 is default.
  *			must be greater than 0 and less than 20
  *	-dcache     The number of data cache entries.
  *			must be at least 50
  *	-rootcell   The name of the cell containing the root volume.
  *			must be a non empty string
  *	-tokenprocs Number of token process for the TKN manager.
  *			must be greater than 0
  *	-ioprocs    Number of bkg I/O daemons (AIX3.1 only)
  *			must be greater than 0
  *	-namecachesize Size of the name lookup hash; default is 256 entries.
  *	-initiallocalprotectlevel	Initial and minimum protection levels
  *	-minlocalprotectlevel	 for the local cell and for all other
  *	-initialremoteprotectlevel	  cells.
  *	-minremoteprotectlevel
  *	-help
  *
  *----------------------------------------------------------------------------
  */

#define VFS 1

#include <dcedfs/param.h>
#include <dcedfs/cmd.h>
#include <dcedfs/cmdAux.h>

/* The following double defines "removed" during internationalization:
 *#define VFS 1
 *
 *#include <dcedfs/param.h>
 */
#include <dcedfs/osi.h>
#include <dcedfs/osi_mount.h>
#include <dcedfs/ioctl.h>
#include <dcedfs/common_data.h>
#include <dcedfs/queue.h>
#include <dcedfs/cm.h>
#include <dcedfs/syscall.h>
#include <dcedfs/compat.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsdsdmac.h>
#include <dfsdsdsvc.h>
#include <dfsdsdmsg.h>
#include <dirent.h>

#define static	/* hack! to get symbols for adb */

#undef	VIRTUE
#undef	VICE

dce_svc_handle_t   dsd_svc_handle;

void initialize_svc()
{
  error_status_t st;

  dsd_svc_handle = dce_svc_register(dsd_svc_table,  (idl_char *)"dsd",  &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot register svc 0x%lx\n", st);
  dce_svc_routing((unsigned char *) "NOTICE:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route NOTICE messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "WARNING:STDOUT:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route WARNING messages  0x%lx\n", st);
  dce_svc_routing((unsigned char *) "ERROR:STDERR:--", &st);
  if (st != svc_s_ok)
        fprintf(stderr, "Cannot route ERROR messages  0x%lx\n", st);

  dfs_define_dsd_message_tables();

}

/* 
 * define argument indices 
 */
#define NUM_BLOCKS               0
#define NUM_CACHE_FILES          1
#define NUM_STATUS_ENTRIES       2
#define ROOT_FILESET             3
#define CACHE_DIR                4
#define MOUNT_DIR                5
#define ROOT_CELL                6
#define SET_TIME                 7
#define NUM_DAEMONS              8
#define NUM_TOKEN_PROCS          9
#define NUM_IO_PROCS             10
#define SIZE_MEMCACHE            11
#define NUM_DCACHE               12
#define CHUNK_SIZE               13
#define NUM_NAME_CACHE_ENTRIES   14
#define CM_D_INITLOCALAUTHN	15
#define CM_D_MINLOCALAUTHN	16
#define CM_D_INITREMOTEAUTHN	17
#define CM_D_MINREMOTEAUTHN	18
#define VERBOSE                  19
#define DEBUG                    20


static char dceLocal_Path[] = DCELOCAL_PATH;
#define CACHEINFOFILESUFFIX   "/etc/CacheInfo"
#define	DCACHEFILE	"CacheItems"
#define	VOLINFOFILE	"FilesetItems"

/* Constants for minimum values for values */
#define MIN_DCACHESIZE  50

/*
 *  the base DFS dir
 */
#define	BASEDIRSUFFIX	    "var/adm/dfs"
static char DFSBaseDir[1024];


#define MAX_CACHE_LOOPS 4
/*
 * Internet address (old style... should be updated).  This was pulled out of
 * the old 4.2 version of <netinet/in.h>, since it's still useful.
 */
struct in_addr_42 {
	union {
		struct { u_char s_b1,s_b2,s_b3,s_b4; } S_un_b;
		struct { u_short s_w1,s_w2; } S_un_w;
		u_long S_addr;
	} S_un;
#define	s_host	S_un.S_un_b.s_b2	/* host on imp */
#define	s_net	S_un.S_un_b.s_b1	/* network */
#define	s_imp	S_un.S_un_w.s_w2	/* imp */
#define	s_impno	S_un.S_un_b.s_b4	/* imp # */
#define	s_lh	S_un.S_un_b.s_b3	/* logical host */
};

#define	mPrintIPAddr(ipaddr)  dce_svc_printf(DSD_S_IPADDR_MSG, \
      ((struct in_addr_42 *) &(ipaddr))->S_un.S_un_b.s_b1,	\
      ((struct in_addr_42 *) &(ipaddr))->S_un.S_un_b.s_b2,	\
      ((struct in_addr_42 *) &(ipaddr))->S_un.S_un_b.s_b3,	\
      ((struct in_addr_42 *) &(ipaddr))->S_un.S_un_b.s_b4)

/*
 * Global configuration variables.
 */
static long cacheBlocks;		/* Num blocks in the cache*/
static long cacheFiles	= 1000;		/* Optimal # of files in cache */
static long cacheStatEntries = 300;	/* Number of stat cache entries*/
static char fullpn_DCacheFile[1024];	/* Full pathname of DCACHEFILE*/
static char fullpn_VolInfoFile[1024];	/* Full pathname of VOLINFOFILE*/
static char fullpn_VFile[1024];		/* Full pathname of data cache files*/
static char *vFileNumber;		/* Ptr to num part of above pathname */
static char dfsMountDir[1024];		/* Mount directory for DFS */
static char cacheBaseDir[1024];		/* Where the DFS cache lives */
static char localCellName[512];		/* local Cell Name */
static char *localMachinePrincipal;
static char rootFileset[200] = "root.dfs";	/* DFS root fileset name */
static char rootCell[200];
static int createAndTrunc = O_CREAT | O_TRUNC; /* Create & truncate on open */
static int ownerRWmode	= 0600;		/* Read/write OK by owner*/
static int nDaemons = 2;		/* Number of background daemons */
static int chunkSize = 0;               /* 2^chunkSize bytes per chunk */
static int dCacheSize = 100;            /* # of dcache entries */
static int vCacheSize = 50;             /* # of volume cache entries */
static int nIOprocs = 5;		/* AIX3.1 only */
static int nTKNprocs = 2;		/* Number of Token services */
static int cacheFlags = 0;		/* Flags to cache manager*/
static int namecachesize = 256;		/* Size of the name cache */
/*
 * The following flags are from cmd line switches 
 */
static int dfsd_dfsMountDir = 0;
static int dfsd_cacheBaseDir = 0;
static int dfsd_memCache = 0;
static int dfsd_cacheBlocks = 0;
static int dfsd_DCacheSize = 0;
static int dfsd_cacheSetTime = 0;       /* Don't set the time, by default */
static int dfsd_rootFileset = 0;	/* True if root fileset name set */
static int dfsd_cacheFiles = 0;		/* True if num of cache files set */
static int dfsd_rootCell = 0;		/* True if root cell name set */
static struct cm_security_bounds dfsd_localSec =
	{CM_DEFAULT_LCLINIT_LEVEL, CM_DEFAULT_LCLMIN_LEVEL};
static struct cm_security_bounds dfsd_remoteSec =
	{CM_DEFAULT_RMTINIT_LEVEL, CM_DEFAULT_RMTMIN_LEVEL};
static int dfsd_verbose = 0;		/* Are we being chatty? */
static int dfsd_debug = 0;		/* Are we printing debugging info? */

/*
 */
static u_long *flags_for_V;             /* Array of flagsfor desired
					 * cache files
					 */

static int missing_DCacheFile	= 1;	/* Is the DCACHEFILE missing? */
static int missing_VolInfoFile	= 1;	/* Is the VOLINFOFILE missing? */

/* Parameters passed to the CM */
static struct cm_cacheparams cparams;

/*-----------------------------------------------------------------------------
  * ParseCacheInfoFile
  *
  * Description:
  *	Open the file containing the description of the workstation's DFS cache
  *	and pull out its contents.  The format of this file is as follows:
  *
  *	    dfsMountDir:cacheBaseDir:cacheBlocks
  *
  * Arguments:
  *	None.
  *
  * Returns:
  *	0 if everything went well,
  *	1 otherwise.
  *
  * Environment:
  *	Nothing interesting.
  *
  *  Side Effects:
  *	Sets globals.
  *----------------------------------------------------------------------------
  */
static int ParseCacheInfoFile()
{
    FILE *cachefd;			/*Descriptor for cache info file*/
    int	parseResult;			/*Result of our fscanf()*/
    long tCacheBlocks;
    char tCacheBaseDir[1024], tdfsMountDir[1024], infoFile[1024];

    strncpy(infoFile, dceLocal_Path, sizeof(infoFile));
    strncat(infoFile, CACHEINFOFILESUFFIX, sizeof(infoFile)-sizeof(dceLocal_Path));
    infoFile[sizeof(infoFile)-1] = '\0';
    if (dfsd_debug)
	dce_svc_printf(DSD_S_OPENINGCACHE_MSG, infoFile);

    cachefd = fopen(infoFile, "r");
    if (!cachefd) {
	dce_svc_printf(DSD_S_NOCACHEFD_MSG, infoFile, errno);
        return(1);
    }
    /*
     * Parse the contents of the cache info file.  All chars up to the first
     * colon are the AFS mount directory, all chars to the next colon are the
     * full path name of the workstation cache directory and all remaining
     * chars represent the number of blocks in the cache.
     */
    tdfsMountDir[0] = tCacheBaseDir[0] = '\0';
    parseResult = fscanf(cachefd,
			  "%1024[^:]:%1024[^:]:%d",
			  tdfsMountDir, tCacheBaseDir, &tCacheBlocks);

    /*
     * Regardless of how the parse went, we close the cache info file.
     */
    fclose(cachefd);
    if (parseResult == EOF || parseResult < 3) {
       dce_svc_printf(DSD_S_FORMATERROR_MSG);
       if (parseResult == EOF)
	 dce_svc_printf(DSD_S_TOOSOONEOF_MSG);
       else
	 dce_svc_printf(DSD_S_GOODPARSE_MSG, parseResult);
       dce_svc_printf(DSD_S_PARSEFIELDS_MSG, dfsMountDir, cacheBaseDir, cacheBlocks);
       return(1);
    }
    
    /* 
     * now copy in the fields not explicitly overridden by cmd args 
     */
    if (!dfsd_dfsMountDir)
	strcpy(dfsMountDir, tdfsMountDir);
    /*
     * Setup the cacheBaseDir for both disk cache and mem cache.
     */
    if (!dfsd_cacheBaseDir)
	strcpy(cacheBaseDir, tCacheBaseDir);

    if  (!dfsd_cacheBlocks)
	cacheBlocks = tCacheBlocks;

    if (dfsd_debug) {
       dce_svc_printf(DSD_S_CACHEPARSE_MSG);
       dce_svc_printf(DSD_S_CACHEFIELDS_MSG, tdfsMountDir, tCacheBaseDir, tCacheBlocks);
    }
    return(0);
}

/*-----------------------------------------------------------------------------
  * GetVFileNumber
  *
  * Description:
  *	Given the final component of a filename expected to be a data cache
  *	file, return the integer corresponding to the file.  Note: we reject
  *	names that are not a ``V'' followed by an integer.  We also reject
  *	those names having the right format but lying outside the range
  *	[0..cacheFiles-1].
  *
  * Arguments:
  *	fname : Char ptr to the filename to parse.
  *
  * Returns:
  *	>= 0 iff the file is really a data cache file numbered from 0 to
  *	     cacheFiles-1, or
  *	-1      otherwise.
  *
  * Environment:
  *	Nothing interesting.
  *
  * Side Effects:
  *	None.
  *----------------------------------------------------------------------------
  */

static int GetVFileNumber(fname)
    char *fname;
{
    int	computedVNumber;    /*The computed file number we return*/
    int	filenameLen;	    /*Number of chars in filename*/
    int	currDigit;	    /*Current digit being processed*/

    /*
     * The filename must have at least two characters, the first of which must
     * be a ``V'' and the second of which cannot be a zero unless the file is
     * exactly two chars long.
     */
    filenameLen = strlen(fname);
    if (filenameLen < 2)
	return(-1);
    if (fname[0] != 'V')
	return(-1);
    if ((filenameLen > 2) && (fname[1] == 0))
	return(-1);

    /*
     * Scan through the characters in the given filename, failing immediately
     * if a non-digit is found.
     */
    for (currDigit = 1; currDigit < filenameLen; currDigit++)
	if (isdigit(fname[currDigit]) == 0)
	    return(-1);
    /*
     * All relevant characters are digits.  Pull out the decimal number they
     * represent.  Reject it if it's out of range, otherwise return it.
     */
    computedVNumber = atoi(++fname);
    if (computedVNumber < cacheFiles)
	return(computedVNumber);
    else
	return(-1);
}
/*-----------------------------------------------------------------------------
  * CreateCacheFile
  *
  * Description:
  *	Given a full pathname for a file we need to create for the workstation
  *	AFS cache, go ahead and create the file.
  *
  * Arguments:
  *	fname : Full pathname of file to create.
  *
  * Returns:
  *	0   iff the file was created,
  *	-1  otherwise.
  *
  * Environment:
  *	The given cache file has been found to be missing.
  *
  * Side Effects:
  *	As described.
  *----------------------------------------------------------------------------
  */
static int CreateCacheFile(fname)
    char *fname;
{
    int	cfd;			    /*File descriptor to AFS cache file*/
    int	closeResult;		    /*Result of close()*/

    if (dfsd_verbose)
       dce_svc_printf(DSD_S_CREATINGCFILE_MSG, fname);
    cfd = open(fname, createAndTrunc, ownerRWmode);
    if (cfd <= 0) {
	dce_svc_printf(DSD_S_NOCREATECFILE_MSG, fname, errno);
	return(-1);
    }
    closeResult = close(cfd);
    if (closeResult) {
	dce_svc_printf(DSD_S_NOCLOSECFILE_MSG, fname, errno);
	return(-1);
    }

    return(0);
}

/*-----------------------------------------------------------------------------
  * SweepDFSCache
  *
  * Description:
  *	Sweep through the AFS cache directory, recording the inode number/handle for
  *	each valid data cache file there.  Also, delete any file that doesn't
  *	belong in the cache directory during this sweep, and remember which of
  *	the other residents of this directory were seen.  After the sweep, we
  *	create any data cache files that were missing.
  *
  * Arguments:
  *	vFilesFound : Set to the number of data cache files found.
  *
  * Returns:
  *	0   if everything went well,
  *	-1 otherwise.
  *
  * Environment:
  *	This routine may be called several times.  If the number of data cache
  *	files found is less than the global cacheFiles, then the caller will
  *	need to call it again to record the inodes/handles of the missing zero-length
  *	data cache files created in the previous call.
  *
  * Side Effects:
  *	Fills up the global flags_for_V array, may create and/or delete files 
  *     as explained above.
  *----------------------------------------------------------------------------
  */

static int SweepDFSCache(int *vFilesFound)
{
    char fullpn_FileToDelete[1024];	/*File to be deleted from cache*/
    char *fileToDelete;			/*Ptr to last component of above*/
    DIR	*cdirp;				/*Ptr to cache directory structure*/
    osi_direct_t *currp;		/*Current directory entry*/
    int	vFileNum;			/*Data cache file's associated number*/

    /* 
     * Now scan the DFS Cache Files
     */
    if (dfsd_debug)
	dce_svc_printf(DSD_S_OPENINGCDIR_MSG, cacheBaseDir);

    if (chmod(cacheBaseDir, 0700)) {		/* force it to be 700 */
	dce_svc_printf(DSD_S_BADCHMOD0700_MSG, cacheBaseDir, errno);
	return (-1);
    }
    cdirp = opendir(cacheBaseDir);
    if (cdirp == (DIR *)0) {
	dce_svc_printf(DSD_S_BADOPENCDIR_MSG, cacheBaseDir, errno);
	return(-1);
    }
    /*
     * Scan the directory entries, remembering data cache file inodes/handles and the
     * existence of other important residents.  Delete all files that don't
     * belong here.
     */
    *vFilesFound = 0;
    sprintf(fullpn_FileToDelete, "%s/", cacheBaseDir);
    fileToDelete = fullpn_FileToDelete + strlen(fullpn_FileToDelete);

    for (currp = readdir(cdirp); currp; currp = readdir(cdirp)) {
	if (dfsd_debug) {
	    dce_svc_printf(DSD_S_CURRENTDENTRY_MSG);
	    dce_svc_printf(DSD_S_DENTRYSTATS_MSG, currp->d_ino, currp->d_reclen, currp->d_name);
	}
	/*
	 * Guess current entry is for a data cache file.
	 */
	vFileNum = GetVFileNumber(currp->d_name);
	if (vFileNum >= 0) {
	    /*
	     * Found a valid data cache filename.
	     * Bump the number of files found.
	     */
	    flags_for_V[vFileNum] = 1;	/* mark that we found the file */
	    (*vFilesFound)++;
	}
	else if (strcmp(currp->d_name, DCACHEFILE) == 0) {
	    /*
	     * Found the file holding the dcache entries.
	     */
	    missing_DCacheFile = 0;
	}
	else if (strcmp(currp->d_name, VOLINFOFILE) == 0) {
	    /*
	     * Found the file holding the volume info.
	     */
	    missing_VolInfoFile = 0;
	}
	else  if ((strcmp(currp->d_name,          ".") == 0) ||
		  (strcmp(currp->d_name,         "..") == 0) ||
		  (strcmp(currp->d_name, "lost+found") == 0)) {
	    /*
	     * Don't do anything - this file is legit, and is to be left alone.
	     */
	}
	else {
	    /*
	     * This file doesn't belong in the cache.  Nuke it.
	     */
	    strcpy(fileToDelete, currp->d_name);
	    if (dfsd_verbose)
		dce_svc_printf(DSD_S_DELETINGNUKED_MSG, fullpn_FileToDelete);
	    if (unlink(fullpn_FileToDelete)) {
		dce_svc_printf(DSD_S_BADUNLINKNUKED_MSG, fullpn_FileToDelete, errno);
	    }
	}
    }
    /*
     * Create all the cache files that are missing.
     */
    if (missing_DCacheFile) {
	if (dfsd_verbose)
	    dce_svc_printf(DSD_S_CREATEMISSINGCFILE_MSG, fullpn_DCacheFile);
	if (CreateCacheFile(fullpn_DCacheFile))
	    dce_svc_printf(DSD_S_NOCREATEMISSINGCFILE_MSG, fullpn_DCacheFile, errno);
    }
    if (missing_VolInfoFile) {
	if (dfsd_verbose)
	    dce_svc_printf(DSD_S_CREATEDATACFILE_MSG, fullpn_VolInfoFile);
	if (CreateCacheFile(fullpn_VolInfoFile))
	    dce_svc_printf(DSD_S_BADCREATEDATACFILE_MSG, fullpn_VolInfoFile, errno);
    }
    if (*vFilesFound < cacheFiles) {
	/*
	 * We came up short on the number of data cache files found.  Scan
	 * through the inode/handle list and create all missing files.
	 */
	for (vFileNum = 0; vFileNum < cacheFiles; vFileNum++)
	    if (flags_for_V[vFileNum] == (u_long)0) {
		sprintf(vFileNumber, "%d", vFileNum);
		if (dfsd_verbose)
		    dce_svc_printf(DSD_S_CREATINGVFILENO_MSG, fullpn_VFile);
		if (CreateCacheFile(fullpn_VFile))
		    dce_svc_printf(DSD_S_NOCREATEVFILENO_MSG, fullpn_VFile, errno);
	    }
    }
    /*
     * Close the directory, return success.
     */
    if (dfsd_debug)
	dce_svc_printf(DSD_S_CLOSINGVDIR_MSG);
    closedir(cdirp);
    return(0);
}

mainproc(as, arock)
  register struct cmd_syndesc *as;
  char *arock;
{
    long code;			/* Result of fork() */
    register int i;
    int	currVFile;		/* Current AFS cache file number passed in */
    int	cacheIteration;		/* num of times through cache verification */
    int	vFilesFound;		/* num of cache files were found in sweep */
    char *cellNamep, *cnp, *hostNamep, *localPrincNamep;
    int defaultChunkSize;		/* what we think kernel will use */
    long dum;
#ifdef AFS_HPUX_ENV
    char vfs_slot[MAXPATHLEN];
    int  ret;
#endif /* AFS_HPUX_ENV */

    if (as->parms[NUM_BLOCKS].items) {
	/* -blocks */
	cacheBlocks = atoi(as->parms[NUM_BLOCKS].items->data);
	if (cacheBlocks <= 0) {
	    dce_svc_printf(DSD_S_NOCBLOCKS_MSG);
	    exit(1);
	}
	dfsd_cacheBlocks = 1;
    }
    if (as->parms[NUM_CACHE_FILES].items) {
	/* -files */
	cacheFiles = atoi(as->parms[NUM_CACHE_FILES].items->data);
	if (cacheFiles <= 0) {
	    dce_svc_printf(DSD_S_NOCFILES_MSG);
	    exit(1);
	}
	dfsd_cacheFiles = 1;	/* set when spec'd on cmd line */
    }
    if (as->parms[ROOT_FILESET].items) {
	/* -rootfileset */
	strcpy(rootFileset, as->parms[ROOT_FILESET].items->data);
	if (*rootFileset == '\0') {
	    dce_svc_printf(DSD_S_NOROOTFILESET_MSG);
	    exit(1);
	}
	dfsd_rootFileset = 1;
    }
    if (as->parms[NUM_STATUS_ENTRIES].items) {
	/* -stat */

	cacheStatEntries = atoi(as->parms[NUM_STATUS_ENTRIES].items->data);
	if (cacheStatEntries <= 0) {
	    dce_svc_printf(DSD_S_NOSTATENTRIES_MSG);
	    exit(1);
	}
    }
    if (as->parms[SIZE_MEMCACHE].items) {
	/* -memcache,  for running in diskless mode */
	dfsd_memCache = 1;
	cacheFlags |= CM_INIT_MEMCACHE;
	if (chunkSize == 0)
	    chunkSize = 13;
    }
    if (as->parms[CACHE_DIR].items) {
	/* -cachedir */
	strcpy(cacheBaseDir, as->parms[CACHE_DIR].items->data);
	dfsd_cacheBaseDir = 1;
    }
    if (as->parms[MOUNT_DIR].items) {
	/* -mountdir */
	strcpy(dfsMountDir, as->parms[MOUNT_DIR].items->data);
	dfsd_dfsMountDir = 1;
    }
    if (as->parms[NUM_DAEMONS].items) {
	/* -mainprocs */
	nDaemons = atoi(as->parms[NUM_DAEMONS].items->data);
	if (nDaemons <= 0) {
	    dce_svc_printf(DSD_S_NOMAINPROCS_MSG);
	    exit(1);
	}
    }
    if (as->parms[SET_TIME].items) {
	/* -settime */
	dfsd_cacheSetTime = 1;
    }
    if (as->parms[VERBOSE].items) {
	/* -verbose */
	dfsd_verbose = 1;
    }
    if (as->parms[DEBUG].items) {
	/* -debug */
	dfsd_debug = 1;
	dfsd_verbose = 1;
    }
    if (as->parms[CHUNK_SIZE].items) {
	/* -chunk */
	chunkSize = atoi(as->parms[CHUNK_SIZE].items->data);
	if (chunkSize < 0 || chunkSize > 20) {
	    dce_svc_printf(DSD_S_BADCHUNKSIZE_MSG, chunkSize);
	    chunkSize = 0;
	}
    }
    if (as->parms[NUM_DCACHE].items) {
	/* -dcache */
	dCacheSize = atoi(as->parms[NUM_DCACHE].items->data);
	if (dCacheSize < MIN_DCACHESIZE) {
	    dce_svc_printf(DSD_S_BADDCACHESIZE_MSG, MIN_DCACHESIZE);
	    exit(1);
	}
	dfsd_DCacheSize = 1;
    }
    if (as->parms[ROOT_CELL].items) {
      /* -rootcell */
      strncpy(rootCell, as->parms[ROOT_CELL].items->data, sizeof(rootCell));
      if (*rootCell == '\0') {
	  dce_svc_printf(DSD_S_BADROOTCELL_MSG);
	  exit(1);
      }
      dfsd_rootCell = 1;
    }
    if (as->parms[NUM_TOKEN_PROCS].items) {
	/* -tokenprocs */
	nTKNprocs = atoi(as->parms[NUM_TOKEN_PROCS].items->data);
	if (nTKNprocs <= 0) {
	    dce_svc_printf(DSD_S_BADTOKENPROCS_MSG);
	    exit(1);
	}
    }
    if (as->parms[NUM_IO_PROCS].items) {
	/* -ioprocs */
	nIOprocs = atoi(as->parms[NUM_IO_PROCS].items->data);
	if (nIOprocs <= 0) {
	    dce_svc_printf(DSD_S_BADIOPROCS_MSG);
	    exit(1);
	}
    }
    if (as->parms[NUM_NAME_CACHE_ENTRIES].items) {
	namecachesize= atoi(as->parms[NUM_NAME_CACHE_ENTRIES].items->data);
	if (namecachesize <= 0) {
	    dce_svc_printf(DSD_S_BADNAMECSIZE_MSG);
	    exit(1);
	}
    }
    
    if (as->parms[CM_D_INITLOCALAUTHN].items) {
	if (compat_nameToAuthnLevel(
				    as->parms[CM_D_INITLOCALAUTHN].items->data,
				    &dfsd_localSec.initialProtectLevel)) {
	    dce_svc_printf(DSD_S_BADINITLCLAUTHN_MSG,
			   as->parms[CM_D_INITLOCALAUTHN].items->data);
	    exit(1);
	}
    }
    if (as->parms[CM_D_MINLOCALAUTHN].items) {
	if (compat_nameToAuthnLevel(
				    as->parms[CM_D_MINLOCALAUTHN].items->data,
				    &dfsd_localSec.minProtectLevel)) {
	    dce_svc_printf(DSD_S_BADMINLCLAUTHN_MSG,
			   as->parms[CM_D_MINLOCALAUTHN].items->data);
	    exit(1);
	}
    }
    if (as->parms[CM_D_INITREMOTEAUTHN].items) {
	if (compat_nameToAuthnLevel(
				    as->parms[CM_D_INITREMOTEAUTHN].items->data,
				    &dfsd_remoteSec.initialProtectLevel)) {
	    dce_svc_printf(DSD_S_BADINITRMTAUTHN_MSG,
			   as->parms[CM_D_INITREMOTEAUTHN].items->data);
	    exit(1);
	}
    }
    if (as->parms[CM_D_MINREMOTEAUTHN].items) {
	if (compat_nameToAuthnLevel(
				    as->parms[CM_D_MINREMOTEAUTHN].items->data,
				    &dfsd_remoteSec.minProtectLevel)) {
	    dce_svc_printf(DSD_S_BADMINRMTAUTHN_MSG,
			   as->parms[CM_D_MINREMOTEAUTHN].items->data);
	    exit(1);
	}
    }
    if (dfsd_localSec.minProtectLevel != rpc_c_protect_level_default
	&& dfsd_localSec.initialProtectLevel != rpc_c_protect_level_default
	&& dfsd_localSec.minProtectLevel > dfsd_localSec.initialProtectLevel) {
	dce_svc_printf(DSD_S_LCLPROTOVERLAP_MSG,
		       dfsd_localSec.minProtectLevel, dfsd_localSec.initialProtectLevel);
	exit(1);
    }
    if (dfsd_remoteSec.minProtectLevel != rpc_c_protect_level_default
	&& dfsd_remoteSec.initialProtectLevel != rpc_c_protect_level_default
	&& dfsd_remoteSec.minProtectLevel > dfsd_remoteSec.initialProtectLevel) {
	dce_svc_printf(DSD_S_RMTPROTOVERLAP_MSG);
	exit(1);
    }

    if (ParseCacheInfoFile()) {
	dce_svc_printf(DSD_S_BADOPENCINFOFILE_MSG);
	exit(1);
    }
    /* check for existence of mount point before we get too far into this */
    if (!CheckDirectory(dfsMountDir)) {
	dce_svc_printf(DSD_S_BADMOUNTPOINT_MSG, dfsMountDir);
	exit(1);
    }

    /* do some random computations in memcache case to get things to work
     * reasonably no matter which parameters you set.
     */
    if (cacheFlags & CM_INIT_MEMCACHE) {
	/* memory cache: size described either as blocks or dcache entries, but
	 * not both.
	 */
	defaultChunkSize = (chunkSize == 0 ? 13 : chunkSize);
	if (dfsd_DCacheSize) {
	    if (dfsd_cacheBlocks) {
		dce_svc_printf(DSD_S_NOSIMULTANEOUSSET_MSG );
		exit(1);
	    }
	    /* compute the cache size based on #of chunks times chunk size */
	    i = (1<<defaultChunkSize);	/* bytes per chunk */
	    cacheBlocks = (i * dCacheSize) >> 10;
	    /* so that ParseCacheInfoFile doesn't overwrite */
	    dfsd_cacheBlocks = 1;	
	}
	else {
	    /* compute the dcache size from overall cache size,  chunk size */
	    dCacheSize = (cacheBlocks << 10) / (1<<defaultChunkSize);
	    /* 
	     * don't have to set dfsd_DCacheSize here since it isn't 
	     * overwritten by ParseCacheInfoFile.
	     */
	}
	/* kernel computes # of dcache entries as min of cacheFiles and
	 * dCacheSize, so we now make them equal.
	 */
	cacheFiles = dCacheSize;
    }
    else {
	/* Disk cache:
	 * Compute the number of cache files based on cache size, but only if
	 * -files hasn't explicitly been done.
	 */
	defaultChunkSize = (chunkSize == 0 ? CM_DEFCHUNK : chunkSize);
	/* ... but first see that the directory exists */
	if (!CheckDirectory(cacheBaseDir)) {
	    dce_svc_printf(DSD_S_SEEBADCDIR_MSG, cacheBaseDir);
	    exit(1);
	}
	if (!dfsd_cacheFiles) {
	    /* Steal some heuristics from a modern AFS. */
	    /* Assume about 10k for an average V-file. */
	    cacheFiles = cacheBlocks / 10;
	    if (cacheFiles <  100) cacheFiles =  100;
	    /* Keep cacheFiles large enough so that 2/3-full V-files will
	     * fill the cache. */
	    /* Number of full files that will fill the cache: */
	    dum = cacheBlocks / (1<<(defaultChunkSize-10));
	    /* Now, 3/2 of that number: */
	    dum = (dum*3)>>1;
	    if (cacheFiles < dum) {
		if (dfsd_verbose)
		    fprintf(stderr, "dfsd: Bumping cacheFiles from %d to %d\n",
			    cacheFiles, dum);
		cacheFiles = dum;
	    }
	    /* Now keep cacheFiles small enough that if each V-file takes 1 K,
	     * it won't fill the cache. */
	    dum = cacheBlocks - 30; /* approx. size of FilesetItems */
	    /* Every cacheFile takes 1K plus a cm_fcache entry of 108 bytes */
	    dum = (dum * (1024+108)) / 1024;
	    if (cacheFiles > dum) {
		if (dfsd_verbose)
		    fprintf(stderr, "dfsd: Capping cacheFiles from %d to %d\n",
			    cacheFiles, dum);
		cacheFiles = dum;
	    }
	    /* a final check... */
	    if (cacheFiles > 32000) {
		if (dfsd_verbose)
		    fprintf(stderr, "dfsd: Finally capping cacheFiles from %d to %d\n",
			    cacheFiles, 32000);
		cacheFiles = 32000;
	    }
	}
    }
    /*
     * Now do some final sanity checks on the values that we were passed.
     */
    /*
     * number of bytes in the cacheBlocks (each block is 1024 bytes),
     * must be at least as large as the number of bytes in the chunkSize.
     */
    if ((cacheBlocks << 10) < (1<<defaultChunkSize)) {
	dce_svc_printf(DSD_S_MUSTBE_MSG,
		cacheBlocks, (1<<defaultChunkSize));
	exit(1);
    }
    /*
     * Create and zero the inode table for the desired cache files.
     */
    flags_for_V = (u_long *) malloc(cacheFiles * sizeof(u_long));
    if (flags_for_V == (u_long *)0) {
	dce_svc_printf(DSD_S_BADIMALLOC_MSG, cacheFiles);
	exit(1);
    }
    bzero((char *)flags_for_V, (cacheFiles * sizeof(u_long)));
    if (dfsd_debug)
	dce_svc_printf(DSD_S_VENTRYFLAGS_MSG, cacheFiles, flags_for_V, (cacheFiles * sizeof(u_long)));

    /*
     * Set up all the pathnames we'll need for later.
     */
    sprintf(DFSBaseDir, "%s/%s", dceLocal_Path, BASEDIRSUFFIX);
    sprintf(fullpn_DCacheFile,  "%s/%s", cacheBaseDir, DCACHEFILE);
    sprintf(fullpn_VolInfoFile, "%s/%s", cacheBaseDir, VOLINFOFILE);
    sprintf(fullpn_VFile,       "%s/V",  cacheBaseDir);
    vFileNumber = fullpn_VFile + strlen(fullpn_VFile);

#ifdef mac2
    setpgrp(osi_GetPid(), 0);
#endif /* mac2 */
    
    dce_cf_get_cell_name(&cellNamep, &code);
    if (code) {
	dce_svc_printf(DSD_S_NOLOCALCNAME_MSG, code);
	_exit(1);
    }
    /* canonicalize the cell name by eliminating a leading "/.../" */
    cnp = cellNamep;
    if (strncmp("/.../", cnp, 5) == 0) {
	cnp += 5;
    } else {
	dce_svc_printf(DSD_S_BADCNAMESTART_MSG, cnp);
    }
    strncpy(localCellName, cnp, sizeof(localCellName));

    /* Get the name of the machine principal, too */
    dce_cf_get_host_name(&hostNamep, &code);
    if (code) {
	dce_svc_printf(DSD_S_NOLOCALHNAME_MSG, code);
	_exit(1);
    }
    localPrincNamep = malloc(strlen(cellNamep)+strlen(hostNamep)+sizeof"//self");
    if (localPrincNamep == NULL) {
	dce_svc_printf(DSD_S_NOPRINCNAMEMEMORY_MSG);
	_exit(1);
    }
    sprintf(localPrincNamep, "%s/%s/self", cellNamep, hostNamep);
    free(cellNamep);
    free(hostNamep);

    if (dfsd_verbose)
       dce_svc_printf(DSD_S_STARTINGTKN_MSG);
    /*
     * Set up all the kernel processes needed for the TKN Manager and
     * Issue a system call to invoke and register TKN server(s) in kernel.
     */
    /* Invoke the TKN server in this addr space rather than via a "fork" */
    {
	errno = 0;
	if (call_syscall(CMOP_START_TKN, 0, nTKNprocs, localPrincNamep) != 0) {
	    dce_svc_printf(DSD_S_NOINVOKETKN_MSG , errno);
	    _exit(1);
	}
    }
    free(localPrincNamep);
    code = fork();
    if (code == 0)  { /* Child  */
	/* Create a multi-thread pool for TKN servers */
	call_syscall(CMOP_START_TKN, 1, nTKNprocs, NULL);
	exit(1);
    }

    if (dfsd_verbose)
	dce_svc_printf(DSD_S_FORKINGDAEMON_MSG);
    code = fork();
    if (code == 0) {
	/* Child */
	call_syscall(CMOP_START_MAIN);
	_exit(1);
    }

    if (dfsd_verbose)
	dce_svc_printf(DSD_S_FORKINGBKGNDDAEMONS_MSG, nDaemons);
    for (i = 0; i < nDaemons; i++) {
	code = fork();
	if (code == 0) {
	    /* Child */
#ifdef	AFS_AIX31_VM
	    call_syscall(CMOP_START_BKG, 0);
#else
	    call_syscall(CMOP_START_BKG);
#endif
	    _exit(1);
	}
    }
#ifdef	AFS_AIX31_VM
    for (i=0; i< nIOprocs;i++) {
	code = fork();
	if (code == 0) {	/* Child */
	    call_syscall(CMOP_START_BKG, nIOprocs);
	    exit(1);
	}
    }
#endif

    /*
     * If the root fileset has been explicitly set, tell the kernel.
     */
    if (dfsd_rootFileset) {
	if (dfsd_verbose)
	    dce_svc_printf(DSD_S_CALLINGCMOPRV_MSG, rootFileset);
	if (call_syscall(CMOP_ROOTVOLUME, rootFileset) != 0) {
	    dce_svc_printf(DSD_S_BADSETROOTFS_MSG, errno);
	    _exit(1);
	}
    }
    /*
      * If the root cell has been explicitly set, tell the kernel, too.
      * This is for the diskless client.
	  */
    if (dfsd_rootCell) {
	if (dfsd_verbose)
	    dce_svc_printf(DSD_S_CALLINGCMOPRC_MSG, rootCell);
	if (call_syscall(CMOP_ROOTCELL, rootCell) != 0) {
	    dce_svc_printf(DSD_S_SETROOTFAILED_MSG, errno);
	    _exit(1);
	}
    }
    /*
     * Tell the kernel some basic information about the workstation's cache.
     */
    if (dfsd_verbose) {
	dce_svc_printf(DSD_S_CALLINGCINIT_MSG, cacheStatEntries, cacheFiles);
	dce_svc_printf(DSD_S_OPTIMUMFB_MSG, cacheBlocks);
	dce_svc_printf(DSD_S_OPTIMUMFLAGS_MSG, cacheFlags, dCacheSize);
    }
    bzero((char *)&cparams, sizeof(cparams));
    cparams.cacheScaches = cacheStatEntries;
    cparams.cacheFiles = cacheFiles;
    cparams.cacheBlocks = cacheBlocks;
    cparams.cacheDcaches = dCacheSize;
    cparams.cacheVolumes = vCacheSize;
    cparams.chunkSize = chunkSize;
    cparams.setTimeFlag = dfsd_cacheSetTime;
    cparams.disklessFlag = cacheFlags & CM_INIT_MEMCACHE;
    cparams.namecachesize = namecachesize;
    cparams.localSec = dfsd_localSec;
    cparams.nonLocalSec = dfsd_remoteSec;
    /* 
     * if !memcache then pass in the cacheBaseDir, otherwise if there is
     * a cacheBaseDir pass it in to get the cache dev.
     */
    if (dfsd_memCache && !dfsd_cacheBaseDir) {
	/* allow the user to specify a cacheBaseDir even if we are memCache */
	code = call_syscall(CMOP_CACHEINIT, &cparams, localCellName,
			    (char *)NULL);
    } else {
	code = call_syscall(CMOP_CACHEINIT, &cparams, localCellName,
			    cacheBaseDir);
    } 
    if (code) {
	dce_svc_printf(DSD_S_ICACHEFAIL_MSG, errno);
	_exit(1);
    }

    /* IF Memory-cache based system, then it doesn't need this */

    if (!(cacheFlags & CM_INIT_MEMCACHE)) { 
	/*
	 * Sweep the workstation AFS cache directory, remembering the inodes of
	 * valid files and deleting extraneous files.  Keep sweeping until we
	 * have the right number of data cache files or we've swept too many
	    * times.
	    */
	if (dfsd_verbose)
	    dce_svc_printf(DSD_S_WSCDIRSWEEP_MSG );

	cacheIteration = 0;
	dce_svc_printf(DSD_S_CDIRSWEEPSTART_MSG);
	do {
	    cacheIteration++;
	    if (SweepDFSCache(&vFilesFound)) {
		dce_svc_printf(DSD_S_BADCDIRSWEEP_MSG, cacheIteration);
		exit(1);
	    }
	    if (dfsd_verbose)
		dce_svc_printf(DSD_S_NUMSWEEPCFILES_MSG, vFilesFound, cacheFiles, cacheIteration);
	} while ((vFilesFound < cacheFiles) &&
		 (cacheIteration < MAX_CACHE_LOOPS));
    } 
    
    /*
      * Pass the kernel the name of the workstation cache file holding the 
      * dcache entries.
      */
    if (dfsd_debug)
	dce_svc_printf(DSD_S_CALLINGCMOPCI2_MSG, fullpn_DCacheFile);
    /* once again, meaningless for a memory-based cache. */
    if(!(cacheFlags & CM_INIT_MEMCACHE))
	if (call_syscall(CMOP_CACHEINFO, fullpn_DCacheFile) != 0) {
		dce_svc_printf(DSD_S_ICACHEITEMSFAIL_MSG, 
			fullpn_DCacheFile, errno);
	    	_exit(1);
	}

    /*
      * Pass the kernel the name of the workstation cache file holding the
      * volume information.
      */
    if (dfsd_debug)
	dce_svc_printf(DSD_S_CALLINGCMOPVI_MSG, fullpn_VolInfoFile);
    if (!(cacheFlags & CM_INIT_MEMCACHE))
	if (call_syscall(CMOP_VOLUMEINFO, fullpn_VolInfoFile) != 0) {
	    dce_svc_printf(DSD_S_INITFSIFAILED_MSG, fullpn_VolInfoFile, errno);
	    _exit(1);
	}
    /*
      * Give the kernel the names of the AFS files cached on the workstation's
      * disk.
      */
    if (!(cacheFlags & CM_INIT_MEMCACHE)) { /* ... and again ... */
	if (dfsd_debug)
	    dce_svc_printf(DSD_S_CALLINGCMOPCINOD_MSG, cacheFiles, cacheBaseDir);

	for (currVFile = 0; currVFile < cacheFiles; currVFile++) {
            sprintf(vFileNumber, "%d", currVFile);
	    if (call_syscall(CMOP_CACHEFILE, fullpn_VFile) != 0) {
	        dce_svc_printf(DSD_S_BADSETUPCINOD_MSG, errno);
		_exit(1);
	    }
	} /*end for*/
    }
    /*
      * All the necessary info has been passed into the kernel to run an AFS
      * system.  Give the kernel our go-ahead.
      */
    if (dfsd_debug)
	dce_svc_printf(DSD_S_CALLINGCMOPGO_MSG);
    if (call_syscall(CMOP_GO) != 0) {
	dce_svc_printf(DSD_S_BADCMGRINIT_MSG); 
	exit(1);
    }

    /*
      * At this point, we have finished passing the kernel all the info 
      * it needs to set up the AFS.  Mount the AFS root.
      */
    dce_svc_printf(DSD_S_ALLDAEMONSSTARTED_MSG);

    if (dfsd_verbose)
	dce_svc_printf(DSD_S_MOUNTINGROOTON_MSG, dfsMountDir, 0);

    code = osi_mount("DFS", dfsMountDir, OSI_MOUNT_TYPE_DFS, 0, NULL, NULL, 0);
    if (code)
	dce_svc_printf(DSD_S_NOTSUNCANTMNT_MSG, dfsMountDir, code);

    /*
     * Exit successfully.
     */
    exit(0);
}

main(argc, argv)
int argc;
char **argv; {
    register struct cmd_syndesc *ts;

    osi_setlocale(LC_ALL, "");

    initialize_svc();

    ts = cmd_CreateSyntax((char *) 0, mainproc, (char *) 0, "start DFS");
    cmd_IndexedAddParm(ts, "-blocks", CMD_SINGLE, CMD_OPTIONAL, 
		"number_of_cache_blocks", NUM_BLOCKS);
    cmd_IndexedAddParm(ts, "-files", CMD_SINGLE, CMD_OPTIONAL, 
		"number_of_cache_files", NUM_CACHE_FILES);
    cmd_IndexedAddParm(ts, "-rootfileset", CMD_SINGLE, CMD_OPTIONAL, 
		"root_fileset", ROOT_FILESET);
    cmd_IndexedAddParm(ts, "-stat", CMD_SINGLE, CMD_OPTIONAL, 
		"number_of_status_cache_entries", NUM_STATUS_ENTRIES);
    cmd_IndexedAddParm(ts, "-memcache", CMD_FLAG, CMD_OPTIONAL, 
		"", SIZE_MEMCACHE);
    cmd_IndexedAddParm(ts, "-cachedir", CMD_SINGLE, CMD_OPTIONAL, 
		"cache_directory", CACHE_DIR);
    cmd_IndexedAddParm(ts, "-mountdir", CMD_SINGLE, CMD_OPTIONAL, 
		"DFS_mount_directory", MOUNT_DIR);
    cmd_IndexedAddParm(ts, "-mainprocs", CMD_SINGLE, CMD_OPTIONAL, 
		"number_of_background_daemons", NUM_DAEMONS);
    cmd_IndexedAddParm(ts, "-settime", CMD_FLAG, CMD_OPTIONAL, 
		"", SET_TIME);
    cmd_IndexedAddParm(ts, "-verbose", CMD_FLAG, CMD_OPTIONAL, 
		"", VERBOSE);
    cmd_IndexedAddParm(ts, "-debug", CMD_FLAG, CMD_OPTIONAL, 
		"", DEBUG);
    cmd_IndexedAddParm(ts, "-chunksize", CMD_SINGLE, CMD_OPTIONAL, 
		"chunk_exponent", CHUNK_SIZE);
    cmd_IndexedAddParm(ts, "-dcache", CMD_SINGLE, CMD_OPTIONAL, 
		"number_of_entries", NUM_DCACHE);
    cmd_IndexedAddParm(ts, "-rootcell", CMD_SINGLE, CMD_OPTIONAL, 
		"root_cell", ROOT_CELL);
    cmd_IndexedAddParm(ts, "-tokenprocs", CMD_SINGLE, CMD_OPTIONAL,
		"number_of_token_daemons", NUM_TOKEN_PROCS);
    cmd_IndexedAddParm(ts, "-ioprocs", CMD_SINGLE, CMD_OPTIONAL, 
		"number_of_background_I/O_daemons", NUM_IO_PROCS);
    cmd_IndexedAddParm(ts, "-namecachesize", CMD_SINGLE, CMD_OPTIONAL,
		       "number_of_name_cache_entries", NUM_NAME_CACHE_ENTRIES);
    cmd_IndexedAddParm(ts, "-initiallocalprotectlevel", CMD_SINGLE, CMD_OPTIONAL,
		       "level", CM_D_INITLOCALAUTHN);
    cmd_IndexedAddParm(ts, "-minlocalprotectlevel", CMD_SINGLE, CMD_OPTIONAL,
		       "level", CM_D_MINLOCALAUTHN);
    cmd_IndexedAddParm(ts, "-initialremoteprotectlevel",
		       CMD_SINGLE, CMD_OPTIONAL,
		       "level", CM_D_INITREMOTEAUTHN);
    cmd_IndexedAddParm(ts, "-minremoteprotectlevel", CMD_SINGLE, CMD_OPTIONAL,
		       "level", CM_D_MINREMOTEAUTHN);
    return (cmd_Dispatch(argc, argv));
}

call_syscall(param1, param2, param3, param4, param5)
int param1, param2, param3, param4, param5;
{
    int error;

    error = afs_syscall(AFSCALL_CM, param1, param2, param3, param4, param5);
    return (error);
}

/*
 * CheckDirectory() checks the existence of the specified directory.
 * It returns 1 for existence; 0 otherwise.
 */
int CheckDirectory(dirname)
char *dirname;
{
    int retval = 0;
    struct stat stbuf;

    if ((stat(dirname, &stbuf) == 0) && S_ISDIR(stbuf.st_mode))
	retval = 1;

    return retval;
}


	
