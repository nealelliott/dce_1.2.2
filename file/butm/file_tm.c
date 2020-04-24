/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: file_tm.c,v $
 * Revision 1.1.51.1  1996/10/02  17:06:27  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:05:34  damon]
 *
 * $EndLog$
 */
/*
 * Copyright (C) 1991, 1995 Transarc Corporation
 * All Rights Reserved
 */
/*
 * file_tm.c -- Tape handling routines
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */
RCSID("$Header: /u0/rcs_trees/dce/rcs/file/butm/file_tm.c,v 1.1.51.1 1996/10/02 17:06:27 damon Exp $")

#include <dcedfs/param.h>
#include <dcedfs/sysincludes.h>
#include <ctype.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <pthread.h>
#include <dcedfs/compat.h>
#include <dcedfs/butm.h>

extern int errno;
extern int isafile; 

#ifdef notdef
#define dfprintf(x) {fprintf x;fflush(stdout);}
#else
#define dfprintf(x)
#endif

/* Notes: (PA)
 *
 * 1) When a volume is dumped, if the volume is locked, the dump will produce
 *	an anomalous tape format of the form:
 *		s/w file begin mark
 *		volume header
 *		s/w file end mark
 *	file_ReadFileData returns BUTM_ENDVOLUME if it encounters the
 *      s/w filemark instead of the volume data.
 */

/* data organization on tape:
 * all writes are in blocks of BUTM_BLOCKSIZE bytes. Each block has
 * BUTM_HDRSIZE bytes of header + BUTM_DATASIZE bytes of data. The header
 * (struct markStruct) contains a magic number and counts of real data bytes
 * written out in the block.
 *
 * each file is preceeded by a fileMark, which acts as the file 
 * delimiter. A file consists of contigous data blocks. TM does not
 * understand or interrpet the data in data blocks. 
 *
 * The tape begins with a tape label and ends with EOF file markers
 * in succession (2 or 4 of them ).
 */

static char *whoami = "file_tm";
static tapeConfig config;

/*
 * fork a child process to perform an IOCTL call. This avoids
 * blocking the entire process during a tape operation
 */

static ForkIoctl(fd,op,count)
int fd,op,count;
{
    int rc;				/* return code from system calls */
    int i;				/* loop index */
    int pid;				/* process ID of child process */
    int status;				/* exit status of child process */
    int ioctl_rc;			/* return codes from close */
    int ioctl_err;			/* error numbers from close */
    int pipefd[2];			/* pipe for child return status */
    int forkflag;			/* flag set when ready to fork */

    forkflag = 1;

    /* create pipe for getting return code from child */
    rc = pipe(pipefd);
    if ( rc < 0 )
    {
	LogError("Cannot create pipe for IOCTL process",errno);
	forkflag = 0;
    }

    if ( forkflag )
    {
	pid = fork();
        if ( pid < 0 )
        {
	    close(pipefd[0]);
	    close(pipefd[1]);
	    LogError("Cannot create child process for IOCTL",errno);
	    forkflag = 0;
        }
    }

    if ( !forkflag ) /* problem starting child process */
    {
	/* do the ioctl anyway, it will probably work */
	GENCOM.GENOP = op;
	GENCOM.GENCOUNT = count;
	ioctl_rc = GENCALL(fd,&GENCOM);
	if ( ioctl_rc < 0 )
	{
	    ioctl_err = errno;
	}
	else
	{
	    ioctl_err = 0;
	}
    }
    else if ( pid == 0 ) /* child process */
    {
	/* close all unneccessary file descriptors */
	for ( i = 3 ; i < OPEN_MAX ; i++ )
	{
	    if ( i != fd && i != pipefd[1] )
	    {
		close(i);
	    }
	}

	/* do the ioctl call */
	GENCOM.GENOP = op;
	GENCOM.GENCOUNT = count;
	ioctl_rc = GENCALL(fd,&GENCOM);
	if ( ioctl_rc < 0 ) ioctl_err = errno;
	else ioctl_err = 0;

	/* send the return code and errno back to the parent */
	write(pipefd[1],&ioctl_rc,sizeof(int));
	write(pipefd[1],&ioctl_err,sizeof(int));

	exit(0);
    }
    else /* parent process */
    {
	close(pipefd[1]);

	/* read the result from the child process */
	rc = read(pipefd[0],&ioctl_rc,sizeof(int));
	if ( rc != sizeof(int) )
	{
	    /* tape is now in unknown state */
	    LogError("No response from IOCTL process",errno);
	    ioctl_rc = -1;
	    ioctl_err = EFAULT;
	}
	else
	{
	    rc = read(pipefd[0],&ioctl_err,sizeof(int));
	    if ( rc != sizeof(int) )
	    {
	        LogError("No response from IOCTL process",errno);
	        if ( ioctl_rc < 0 )
		{
		    /* set the error code */
		    ioctl_err = EFAULT;
		}
		else
		{
		    /* the return code was good */
		    ioctl_err = 0;
		}
	    }
	}
	close(pipefd[0]);

	/* get the completion status from the child process */
	rc = waitpid(pid,&status,0);
	while (rc < 0 && errno == EINTR )
	{
	    rc = waitpid(pid,&status,0);
	}
	if ( rc < 0 )
	{
	    LogError("Waitpid, cannot get status of IOCTL process",errno);
	}
	else if ( status != 0 )
	{
	    LogError("Unexpected IOCTL exit status: 0x%04X\n",errno,status);
	}
    }

    errno = ioctl_err;
    return(ioctl_rc);
}

/* Forward space file */
static ForwardSpace(fid,count)
int fid,count;
{
    int error;

    if (isafile)
      return(0);

    error = ForkIoctl(fid,GENFSF,count);
    return error;

}

/* Backward space file */
static BackwardSpace(fid,count)
int fid,count;
{
    int error;

    if (isafile)
      return(0);

    error = ForkIoctl(fid,GENBSF,count);
    return(error);

}

/* write end of file mark */
static WriteEOF(info, fid, count)
     struct butm_tapeInfo *info;
     int fid,count;
{
    int error;

    /* account for the size of the file mark on tape */
    incSize(info,config.fileMarkSize);
    
    if (isafile)
      return(0);

    dfprintf((stdout, "WriteEOF: writing EOF mark\n"));

    error = ForkIoctl(fid,GENWEOF,count);
    return(error);
}

/* rewind tape */
static Rewind(fid,count)
int fid,count;
{
    int code;

    dfprintf((stdout, "Rewind: rewinding tape\n"));

    if (isafile)    {
        code = lseek(fid, 0L, SEEK_SET);
    } else {

    code = ForkIoctl(fid,GENREW,count);
  };

    code = (code == -1) ? errno : code;
    return code;

}

static GetStatus(fid)
int fid;
{
    if (isafile)
      return(0);

#ifndef TESTINGTM
	pthread_yield();
#endif

#ifdef AFS_AIX31_ENV
    return 0;
#else
    return(GENCALL(fid,(char *)&GENGET));
#endif
}


static
WriteFileMark(info, fid,markPtr)
     struct butm_tapeInfo *info;
int fid;
struct markStruct *markPtr;
{
   long code;
   long retval = 0;
   struct markStruct mark;
   char *pad;
   long size;

#ifndef TESTINGTM
	pthread_yield();
#endif
   dfprintf((stdout, "WriteFileMark: writing file mark\n"));
   size = BUTM_BLOCKSIZE;
   pad = (char *)malloc(size);
   bzero((char *)pad,size);
   bzero((char *)&mark,BUTM_HDRSIZE);
   mark.count = htonl(markPtr->count);
   mark.magic = htonl(markPtr->magic);
   bcopy((char *)&mark,(char *)pad,BUTM_HDRSIZE);
   code = write(fid,pad,size);

   if(code != size)
   {
       /* set the return value */
       retval = (code == -1) ? errno : BUTM_EOT;
   }
   incSize(info,size);
   if (isafile)
     info->position++;

   free(pad);
   return(retval);
}

/* ReadFileMark
 * notes:
 *	routine should read directly into the user supplied mark.
 *	should sanity check the information coming from tape
 */

static
ReadFileMark(info, fid,markPtr)
     struct butm_tapeInfo *info;
     int fid;
     struct markStruct *markPtr;
{
    long code;
    struct markStruct mark;
    char *pad;
    long size;
    
#ifndef TESTINGTM
	pthread_yield();
#endif

    dfprintf((stdout, "ReadFileMark: reading file mark\n"));
    size = BUTM_BLOCKSIZE;
    pad = (char *)malloc(size);
    bzero((char *)pad,size);
    bzero((char *)&mark,BUTM_HDRSIZE);
    code = read(fid,pad,size);
    if(code != size)
    {
	dfprintf((stderr, "ReadFileMark: error on read - return val %d\n",
		code));
	free(pad);
	return ( (code == -1) ? errno : BUTM_EOT);
    }
    bcopy((char *)pad,(char *)&mark,BUTM_HDRSIZE);
    markPtr->count = ntohl(mark.count);
    markPtr->magic = ntohl(mark.magic);
    dfprintf((stderr, "ReadFileMark: magic %u bytes %u\n",
		markPtr->magic, markPtr->count));
   if (isafile)
     info->position++;
    
    dfprintf((stdout, "READING : Position = %d\n", info->position));
    free(pad);
    return 0;
}

/* check
 *	check version numbers and permissions in the info structure
 */

static long check (info, write)
  struct butm_tapeInfo *info;
  int   write;				/* write operation requested */
{
    struct progress *p;
    if ((info == (struct butm_tapeInfo *)NULL) || 
	(info->structVersion != CUR_BUTM_VERSION))
    	return BUTM_OLDINTERFACE;
    p = (struct progress *)info->tmRock;
    if ( (p == (struct progress *)NULL) || (p->fid == 0))
      return BUTM_NOMOUNT;
    if (write && (info->flags & BUTM_FLAGS_READONLY))
    	return BUTM_READONLY;
    return 0;
}

/*
 * fork a child process to attempt to open the drive. We want to make
 * certain there is tape in the drive before trying to open the device
 * in the main process
 */

static ForkOpen(device)
char *device;
{
    int rc;				/* return code from system calls */
    int i;				/* loop index */
    int pid;				/* process ID of child process */
    int status;				/* exit status of child process */
    int open_rc;			/* return codes from close */
    int open_err;			/* error numbers from close */
    int pipefd[2];			/* pipe for child return status */
    int forkflag;			/* flag set when ready to fork */

    forkflag = 1;

    /* create pipe for getting return code from child */
    rc = pipe(pipefd);
    if ( rc < 0 )
    {
	LogError("Cannot create pipe for OPEN process",errno);
	forkflag = 0;
    }

    if ( forkflag )
    {
	pid = fork();
        if ( pid < 0 )
        {
	    close(pipefd[0]);
	    close(pipefd[1]);
	    LogError("Cannot create child process for OPEN",errno);
	    forkflag = 0;
        }
    }

    if ( !forkflag ) /* problem starting child process */
    {
	/*
	 *return success, the caller will discover any problems
	 * when it opens the device.
	 */

	open_rc = 0;
	open_err = 0;
    }
    else if ( pid == 0 ) /* child process */
    {
	/* close all unneccessary file descriptors */
	for ( i = 3 ; i < OPEN_MAX ; i++ )
	{
	    if ( i != pipefd[1] )
	    {
		close(i);
	    }
	}

	/* try the open */
	open_rc = open(device,O_RDONLY,0);
	if ( open_rc < 0 )
	{
	    open_err = errno;
	}
	else
	{
	    open_err = 0;
	    close(open_rc);
	}

	/* send the return code and errno back to the parent */
	write(pipefd[1],&open_rc,sizeof(int));
	write(pipefd[1],&open_err,sizeof(int));

	exit(0);
    }
    else /* parent process */
    {
	close(pipefd[1]);

	/* read the result from the child process */
	rc = read(pipefd[0],&open_rc,sizeof(int));
	if ( rc != sizeof(int) )
	{
	    /* this is not a problem since we will reopen the device anyway */
	    LogError("No response from OPEN process",errno);
	    open_rc = 0;
	    open_err = 0;
	}
	else
	{
	    rc = read(pipefd[0],&open_err,sizeof(int));
	    if ( rc != sizeof(int) )
	    {
	        LogError("No response from OPEN process",errno);
	        open_rc = 0;
	        open_err = 0;
	    }
	}
	close(pipefd[0]);

	/* get the completion status from the child process */
	rc = waitpid(pid,&status,0);
	while (rc < 0 && errno == EINTR )
	{
	    rc = waitpid(pid,&status,0);
	}
	if ( rc < 0 )
	{
	    LogError("Waitpid, cannot get status of OPEN process",errno);
	}
	else if ( status != 0 )
	{
	    LogError("Unexpected OPEN exit status: 0x%04X\n",errno,status);
	}
    }

    errno = open_err;
    return(open_rc);
}

static long
file_Mount (info, tape)
  struct butm_tapeInfo *info;
  char *tape;
{   struct progress *p;
    char  filename[64];
    int   fid;
    int xflags; 

#ifndef TESTINGTM
	pthread_yield();
#endif
    dfprintf((stdout, "butm_Mount: Mounting tape\n"));
    if ((info == (struct butm_tapeInfo *)NULL) || 
	(info->structVersion != CUR_BUTM_VERSION))
    	return BUTM_OLDINTERFACE;
    if (info->tmRock != (char *)NULL)
    	return BUTM_PARALLELMOUNTS;
    if (strlen (tape) >= sizeof(info->name))
    	return BUTM_BADARGUMENT;
    strcpy (info->name, tape);
    strcpy (filename, config.device);	/* the name of the tape device*/
    info->position          = (isafile ? 0 : 1);
    dfprintf((stderr, "MOUNTING : Position = %d\n", info->position));
    info->nBytes = info->nRecords = info->nFiles = info->kBytes = 0;
    info->recordSize = 0;
    info->tapeSize = config.capacity;
    info->coefBytes = 1;
    info->coefRecords = 0;
    info->coefFiles = BUTM_HDRSIZE;
    info->simultaneousTapes = 1;
    info->status = 0;
    info->flags = BUTM_FLAGS_SEQUENTIAL;

    xflags = 0;
    if (isafile) xflags |= O_CREAT;
    else
    {
	/*
	 * try to open in a child process first so nothing will
	 * time out should the process block because the device
	 * isn't ready.
	 */

	fid = ForkOpen(filename);
	if ( fid < 0 ) return(BUTM_MOUNTFAIL);
    }

    fid = open (filename, (O_RDWR | xflags), 0777);
    if (fid < 0) {
 	fid = open (filename, O_RDONLY, 0);
	if (fid < 0) {
	    return BUTM_MOUNTFAIL;
	} else {
	    info->flags |= BUTM_FLAGS_READONLY;
	}
    }
    p = (struct progress *) malloc (sizeof(struct progress));
    info->tmRock = (char *)p;
    p->fid = fid;
    p->reading = p->writing = (char)0;
    return 0;
}

/*
 * fork a child process to close the drive. If the drive rewinds on close
 * it could cause the process to block.
 */

static ForkClose(fd)
int fd;
{
    int rc;				/* return code from system calls */
    int i;				/* loop index */
    int pid;				/* process ID of child process */
    int status;				/* exit status of child process */
    int close_rc, parent_close_rc;	/* return codes from close */
    int close_err, parent_close_err;	/* error numbers from close */
    int pipefd[2];			/* pipe for child return status */
    int ctlpipe[2];			/* pipe for message to child */
    int forkflag;			/* flag set when ready to fork */

    forkflag = 1;

    /* create pipe for getting return code from child */
    rc = pipe(pipefd);
    if ( rc < 0 )
    {
	LogError("Cannot create pipe for CLOSE process",errno);
	forkflag = 0;
    }

    /* create pipe for notifying child when to close */
    if ( forkflag )
    {
        rc = pipe(ctlpipe);
        if ( rc < 0 )
        {
	    close(pipefd[0]);
	    close(pipefd[1]);
	    LogError("Cannot create pipe for CLOSE process",errno);
	    forkflag = 0;
        }
    }

    if ( forkflag )
    {
	pid = fork();
        if ( pid < 0 )
        {
	    close(pipefd[0]);
	    close(pipefd[1]);
	    close(ctlpipe[0]);
	    close(ctlpipe[1]);
	    LogError("Cannot create child process for CLOSE",errno);
	    forkflag = 0;
        }
    }

    if ( !forkflag ) /* problem starting child process */
    {
	close_rc = close(fd);
	if ( close_rc < 0 )
	{
	    close_err = errno;
	}
	else
	{
	    close_err = 0;
	}
	parent_close_rc = close_rc;
	parent_close_err = close_err;
    }
    else if ( pid == 0 ) /* child process */
    {
	/* close all unneccessary file descriptors */
	for ( i = 3 ; i < OPEN_MAX ; i++ )
	{
	    if ( i != fd && i != ctlpipe[0] && i != pipefd[1] )
	    {
		close(i);
	    }
	}

	/* the parent writes the control pipe after it closes the device */
	read(ctlpipe[0],&close_rc,sizeof(int));
	close(ctlpipe[0]);

	/* do the close */
	close_rc = close(fd);
	if ( close_rc < 0 )
	{
	    close_err = errno;
	}
	else
	{
	    close_err = 0;
	}

	/* send the return code and errno back to the parent */
	write(pipefd[1],&close_rc,sizeof(int));
	write(pipefd[1],&close_err,sizeof(int));

	exit(0);
    }
    else /* parent process */
    {
	close(pipefd[1]);
	close(ctlpipe[0]);

	/*
	 * close the device, this should have no effect as long as the
	 * child has not closed
	 */

	parent_close_rc = close(fd);
	if ( parent_close_rc < 0 )
	{
	    parent_close_err = errno;
	}
	else
	{
	    parent_close_err = 0;
	}

	/* notify the child to do its close */
	rc = write(ctlpipe[1],&close_rc,sizeof(int));
	if ( rc != sizeof(int) )
	{
	    LogError("Error communicating with CLOSE process\n",errno);
	}
	close(ctlpipe[1]);

	/* read the result from the child process */
	rc = read(pipefd[0],&close_rc,sizeof(int));
	if ( rc != sizeof(int) )
	{
	    /* logging is enough, since we wrote a file mark the */
	    /* return code from the close doesn't really matter  */
	    LogError("No response from CLOSE process",errno);
	    close_rc = 0;
	    close_err = 0;
	}
	else
	{
	    rc = read(pipefd[0],&close_err,sizeof(int));
	    if ( rc != sizeof(int) )
	    {
	        LogError("No response from CLOSE process",errno);
	        close_rc = 0;
	        close_err = 0;
	    }
	}
	close(pipefd[0]);

	/* get the completion status from the child process */
	rc = waitpid(pid,&status,0);
	while (rc < 0 && errno == EINTR )
	{
	    rc = waitpid(pid,&status,0);
	}
	if ( rc < 0 )
	{
	    LogError("Waitpid, cannot get status of OPEN process",errno);
	}
	else if ( status != 0 )
	{
	    LogError("Unexpected CLOSE exit status: 0x%04X\n",errno,status);
	}

	/* if either process received an error, then return an error */
	if ( parent_close_rc < 0 )
	{
	    close_rc = parent_close_rc;
	    close_err = parent_close_err;
	}
    }

    errno = close_err;
    return(close_rc);
}

static long file_Dismount (info)
  struct butm_tapeInfo *info;
{   struct progress *p;
    long  code;

#ifndef TESTINGTM
	pthread_yield();
#endif
    dfprintf((stdout, "butm_Dismount: dismounting tape\n"));
    if(info->tmRock == (char *)NULL) 
      return 0;/* repeated dismount are a no op */
    if (code = check (info, READ_OP)) return code;
    p = (struct progress *)info->tmRock;

    /* close the device */
    if (ForkClose(p->fid)) {
	LogError("Warning: error closing tape device\n",errno);
	dfprintf((stdout,"Warning: error closing tape device\n"));
    }
    info->tmRock = (char *)NULL;
    if(p)free (p);
    return 0;
}

static long rewindFile (info)
  struct butm_tapeInfo *info;
{   struct progress *p;
    long  code;

    dfprintf((stdout, "rewindFile: rewinding file\n"));
    p = (struct progress *)info->tmRock;
    code = Rewind(p->fid,1);
    if (code) {
	info->status |= BUTM_STATUS_SEEKERROR;
	return BUTM_IO;
    }

    info->position          = (isafile ? 0 : 1);
    dfprintf((stdout, "REWINDING : Position = %d\n", info->position));
    info->nBytes = info->nRecords = info->nFiles = info->kBytes = 0;
    info->status &= ~BUTM_STATUS_EOT;
    return 0;
}

/* file_Create
 *	write the header on a tape
 * entry:
 *	info - handle on tape unit
 *	label - label information. This label is not copied onto the tape.
 *		If supplied, various fields are copied from this label to
 *		the actual tape label written on the tape.
 */

static long file_Create (info, label)
  struct butm_tapeInfo *info;
  struct butm_tapeLabel *label;
{   struct progress *p;
    long  code;
    struct butm_tapeLabel tl;
    char *pad;
    long size;
      
#ifndef TESTINGTM
	pthread_yield();
#endif
    dfprintf((stdout, "butm_Create: creating tape label\n"));
    if (code = check (info, WRITE_OP)) return code;
    p = (struct progress *)info->tmRock;
    if (p->reading || p->writing) return BUTM_BADOP;

    /* allocate an appropriately sized tape block for the label */
    size = BUTM_BLOCKSIZE;
    pad = (char *)malloc(size);
    bzero((char *)pad,size);

    if (code = rewindFile (info)) {
	free(pad);
	return code;
    }
    bzero ((char *)&tl, sizeof(struct butm_tapeLabel));
    tl.magic = htonl(TAPE_MAGIC);
    tl.structVersion = htonl(CUR_TAPE_VERSION);
    tl.creationTime = htonl(time(0));
    tl.expirationDate = htonl(label->expirationDate);
    tl.size = htonl(label->size);
    tl.useCount = htonl(label->useCount);
    tl.dumpid = htonl(label->dumpid);
    bcopy ((char *)info->name, (char *)tl.name, BU_MAXTAPELEN);
    bzero((char *)&(tl.dumpPath[0]), BU_MAX_DUMP_PATH);
    if (label) {
	if (label->structVersion != CUR_TAPE_VERSION)
	{
		dfprintf((stdout, "wanted %d got %d\n",
			CUR_TAPE_VERSION, label->structVersion));
		return BUTM_OLDINTERFACE;
	}
	bcopy ((char *)label->cell, (char *)tl.cell, BU_MAXCELLLEN);
	bcopy ((char *)label->comment, (char *)tl.comment, 
	       sizeof(tl.comment));
	strcpy((char *)tl.dumpPath, (char *)label->dumpPath);
    }
    bcopy((char *)&tl,(char *)pad,sizeof(struct butm_tapeLabel));

    /* write the tape label */
    code = write (p->fid, pad,size);
    if ( code != size )
    {
	/* encountered an error */
	info->status |= BUTM_STATUS_WRITEERROR;	
	free(pad);
	return ( (code == -1) ? BUTM_IO : BUTM_EOT);
    }

    /* write eof on tape */
    code = WriteEOF(info, p->fid,1);
    if ( code == -1)
    {
	/* encountered an error */
	info->status |= BUTM_STATUS_WRITEERROR;	
	free(pad);
	return (BUTM_IO); 
    }

    if (isafile)
      info->position = 1;
    else
      info->position = 2;
    dfprintf((stdout, "CREATING : Position = %d\n", info->position));
    info->status = 0;			/* reset error status */
    free(pad);
    return 0;
}

static long
file_ReadLabel (info, label)
  struct butm_tapeInfo *info;
  struct butm_tapeLabel *label;
{   struct progress *p;
    long  code;
    struct butm_tapeLabel tl;
    char *pad;
    long size;
   
#ifndef TESTINGTM
	pthread_yield();
#endif
    dfprintf((stdout, "butm_ReadLabel: reading tape label\n"));
    if (code = check (info, READ_OP))
    	return code;
    p = (struct progress *)info->tmRock;
    if (p->reading || p->writing)
    	return BUTM_BADOP;
    
    size = BUTM_BLOCKSIZE;
    pad = (char *)malloc(size);
    bzero((char *)pad,size);
    if (code = rewindFile (info)) {
	free(pad);
	return code;
    }
    code = read (p->fid, pad,size);
    bzero((char *)&tl, sizeof(struct butm_tapeLabel));
    bcopy((char *)pad,(char *)&tl,sizeof(struct butm_tapeLabel));

    /* various checks on the label read
     * size - verify size as that of a markStruct
     * magic - make sure it has right magic
     * version - don't check this. Need to be able to read older tapes
     */
    if ( (code != size)
    ||   (ntohl(tl.magic) != TAPE_MAGIC)
       )
    {
	info->status |= BUTM_STATUS_READERROR;
	free(pad);
	return BUTM_IO;
    }
    
    if (label) {
	*label = tl;
	label->structVersion = ntohl(label->structVersion);
	label->creationTime = ntohl(label->creationTime);
	label->expirationDate = ntohl(label->expirationDate);
	label->size = ntohl(label->size);
	label->dumpid = ntohl(label->dumpid);
	label->useCount = ntohl(label->useCount);
	info->tapeSize = label->size;	/* set the size of the tape to 
					   that read in the label */
    }
    /* forward space one file */
    code = ForwardSpace(p->fid,1);
    if(code) {
	info->status |= BUTM_STATUS_READERROR;
	free(pad);
	return BUTM_IO;
    }

    if (isafile)
      info->position = 1;
    else
      info->position = 2;
    dfprintf((stdout, "READLABEL : Position = %d\n", info->position));
    info->status = 0;			/* reset error status */
    free(pad);
    return 0;
}

static long file_Seek (info, position)
  struct butm_tapeInfo *info;
  long position;
{   struct progress *p;
    long  code;
    int w; 

    dfprintf((stdout, "butm_Seek: seeking to position %ld\n", position));
    if (code = check (info, READ_OP)) return code;
    p = (struct progress *)info->tmRock;
    if (p->reading || p->writing) return BUTM_BADOP;

    if (isafile) {
      int posit = (position * BUTM_BLOCKSIZE);

        w = lseek(p->fid, posit, SEEK_SET);
      if (w != posit || w == -1 ) {
	    info->status |= BUTM_STATUS_SEEKERROR;
	    return BUTM_IO;
	  };
	info->position = position;
    dfprintf((stdout, "SEEK: Position = %d\n", info->position));
    }
    else  {
      if (position < info->position ) { /* back space info->position - position files */
	code = BackwardSpace(p->fid,(info->position - position));
	if(code) {
	  info->status |= BUTM_STATUS_SEEKERROR;
	  return BUTM_IO;
	}
	info->position = position;
      }

      if(position > info->position) { /* forward space ( position - info->position) files */ 
	code = ForwardSpace(p->fid,(position - info->position));
	if(code) {
	  info->status |= BUTM_STATUS_SEEKERROR;
	  return BUTM_IO;
	}
	info->position = position;
      }
    }
    return 0;
}

static long file_ReadFileBegin (info)
  struct butm_tapeInfo *info;
{   struct progress *p;
    long  code;
    struct markStruct mark;

#ifndef TESTINGTM
	pthread_yield();
#endif
    dfprintf((stdout, "butm_ReadFileBegin: reading file\n"));
    if (code = check (info, READ_OP)) return code;
    p = (struct progress *)info->tmRock;
    if (p->reading || p->writing) return BUTM_BADOP;

    code = ReadFileMark(info, p->fid,&mark);
    if(code) {
	info->status |= BUTM_STATUS_READERROR;
	return BUTM_IO;
    }
    if(mark.count == TM_EOT && mark.magic == FILE_MAGIC){
	dfprintf((stderr, "butm_ReadFileBegin: End of Tape\n"));
	info->status |= BUTM_STATUS_EOT;
	p->reading++;
	info->status &= ~BUTM_STATUS_EOF;
	return BUTM_EOT;
    }
    if(mark.count != FILE_BEGIN || mark.magic != FILE_MAGIC){
	dfprintf((stdout, "butm_ReadFileBegin: Status read error\n"));
	info->status |= BUTM_STATUS_READERROR;
	return BUTM_IO;
    }
    p->reading++;
    info->status &= ~BUTM_STATUS_EOF;
    return 0;
}

/* file_ReadFileData
 *	Read a data block from tape.
 * entry:
 *	info - tape info structure, c.f. fid
 *	data - ptr to buffer for data
 * exit:
 *	nBytes - no. of data bytes read.
 */

static long
file_ReadFileData (info, data, len, nBytes)
  struct butm_tapeInfo *info;
  char *data;
  int  len;
  int  *nBytes;
{   
      struct progress *p;
      long blocktype;
      long  code, retval;

#ifndef TESTINGTM
	pthread_yield();
#endif
      dfprintf((stdout, "butm_ReadFileData: reading file data\n"));
      *nBytes = 0;
      retval = 0;

      if (code = check (info, READ_OP))		/* permissions etc. check */
      		return code;
      p = (struct progress *)info->tmRock;
      if (!p->reading || p->writing)
      		return BUTM_BADOP;

      if(info->status & BUTM_STATUS_EOF)
	  return BUTM_STATUS_EOF;
      
      code = read(p->fid,data,BUTM_BLOCKSIZE);
      if(code != BUTM_BLOCKSIZE)
      {
	if (code == 0) {
	  retval = BUTM_STATUS_EOF;
	  goto exit;
	}
	info->status |= BUTM_STATUS_READERROR;
	retval = BUTM_IO;
	goto exit;
      }
      dfprintf((stderr, "butm_ReadFileData: code = %d\n", code));

      dfprintf((stderr, "butm_ReadFileData: magic %d, count %d\n",
		ntohl(((struct markStruct *)data)->magic),
		ntohl(((struct markStruct *)data)->count)));
      blocktype = 0;
      /* try to detect what kind of block it is. */
      if ( ntohl(((struct markStruct *)data)->magic) == BLOCK_MAGIC )
      		blocktype = BLOCK_MAGIC;
      else
      if ( ntohl(((struct markStruct *)data)->magic) == FILE_MAGIC )
      		blocktype = FILE_MAGIC;
      else
      if ( ntohl(((struct markStruct *)data)->magic) == TAPE_MAGIC )
      		blocktype = TAPE_MAGIC;

      switch ( blocktype )
      {
	case BLOCK_MAGIC:
	  /* normal data block */
	  dfprintf((stderr, "butm_ReadFileData: This block is block_magic\n"));
	  *nBytes = ntohl(((struct markStruct *)data)->count);
	  dfprintf((stderr, "butm_ReadFileData: Bytes of Data = %d\n", *nBytes));
	  /* only update info->nBytes if not negative. */
	  incSize(info,len);
	  if (isafile)
	    info->position++;
	  break;

	case FILE_MAGIC:
	  dfprintf((stderr, "butm_ReadFileData: This block is file_magic\n"));
	  /* verify that it is a file end mark, and then skip over the
	   * h/w file mark
	   */
	  if ( ntohl(((struct markStruct *)data)->count) != FILE_END )
	  {
	      info->status |= BUTM_STATUS_READERROR;
	      retval = BUTM_IO;
	      goto exit;
	  }
	  code = ForwardSpace(p->fid,1);
	  if(code)
	  {
	      info->status |= BUTM_STATUS_READERROR;
	      return BUTM_IO;
	  }
	  info->status &= ~BUTM_STATUS_EOF;
          p->reading = 0;
	  info->position++;
	  retval = BUTM_ENDVOLUME;
	  break;

	default:
	case TAPE_MAGIC:
	  /* tape magic is used only for tape labels */
	  dfprintf((stderr, "butm_ReadFileData: This block is tape_magic\n"));
	  if (isafile)
	    info ->position++;
	  info->status |= BUTM_STATUS_READERROR;
	  retval = BUTM_IO;
	  break;
      }

exit:
      return(retval);
}

/* file_ReadFileDataCopy
 *	Read a data block from tape and copies the data from the
 *      tape block into the application buffer.
 * entry:
 *	info - tape info structure, c.f. fid
 *	data - ptr to buffer for data
 *      len  - size of buffer for data
 * exit:
 *	nBytes - no. of data bytes read.
 */

static long
file_ReadFileDataCopy (info, data, len, nBytes)
  struct butm_tapeInfo *info;
  char *data;
  int  len;
  int  *nBytes;
{   
      long code;
      char *databuf;

      databuf = (char *)osi_Alloc(BUTM_BLOCKSIZE);
      if ( !databuf ) return(BUTM_NOMEM);
      code = file_ReadFileData(info,databuf,len,nBytes);
      if ( !code ) bcopy(databuf+BUTM_HDRSIZE,data,*nBytes);
      osi_Free(databuf,BUTM_BLOCKSIZE);
      return(code);
}

/* read the end of a tape file. If the caller is
 * reading ahead, then the end of file block should
 * already be in prevBuf. Otherwise we need to read
 * the tape.
 */
static long file_ReadFileEnd (info)
struct butm_tapeInfo *info;
{
    struct progress *p;
    long  code;
    struct markStruct mark;

#ifndef TESTINGTM
	pthread_yield();
#endif
    dfprintf((stdout, "butm_ReadFileEnd: reading file end\n"));
    if (code = check (info, READ_OP)) return code;
    p = (struct progress *)info->tmRock;
    if (!p->reading || p->writing) return BUTM_BADOP;
    info->status &= ~BUTM_STATUS_EOF;
    p->reading = 0;

    bzero((char *)&mark,BUTM_HDRSIZE);
    code = ReadFileMark(info, p->fid,&mark);
    if(code) {
	info->status |= BUTM_STATUS_READERROR;
	return BUTM_IO;
    }
    if(mark.count == TM_EOT && mark.magic == FILE_MAGIC){
	dfprintf((stderr, "butm_ReadFileEnd: End of Tape\n"));
	info->status |= BUTM_STATUS_EOT;
	return BUTM_EOT;
    }
    if(mark.count != FILE_END || mark.magic != FILE_MAGIC){
	dfprintf((stderr, "butm_ReadFileEnd: Status Error\n"));
	info->status |= BUTM_STATUS_READERROR;
	return BUTM_IO;
    }

    code = ForwardSpace(p->fid,1);
    if(code) {
	info->status |= BUTM_STATUS_READERROR;
	return BUTM_IO;
    }
    if (!isafile)
      info->position++;
    return 0;
}

static long file_WriteFileBegin (info)
  struct butm_tapeInfo *info;
{   struct progress *p;
    long  code;
    struct markStruct mark;

#ifndef TESTINGTM
	pthread_yield();
#endif
    dfprintf((stdout, "butm_WriteFileBegin: writing file begin\n"));
    if (code = check (info, WRITE_OP)) return code;
    p = (struct progress *)info->tmRock;
    
    mark.count = FILE_BEGIN;
    mark.magic = FILE_MAGIC;
    code = WriteFileMark(info, p->fid,&mark);
    if(code) {
	info->status |= BUTM_STATUS_WRITEERROR;
	  return BUTM_IO;
    }
    info->nFiles++;
    p->writing++;
    return 0;
}

static long
file_WriteFileData (info, data, count)
struct butm_tapeInfo *info;
char *data;
int count;
{  
      struct progress *p;
      long  code;

#ifndef TESTINGTM
	pthread_yield();
#endif
      dfprintf((stdout, "butm_WriteFileData: writing file data\n"));
      bzero(data,BUTM_HDRSIZE);
      ((struct markStruct *)data)->count = htonl(count);
      ((struct markStruct *)data)->magic = htonl(BLOCK_MAGIC);
      if(count > BUTM_DATASIZE)
      	return BUTM_IO;
      if (code = check (info, WRITE_OP))
      	return code;

      p = (struct progress *)info->tmRock;
      if (p->reading || !p->writing)
      	return BUTM_BADOP;
      if (count == 0)
      	return 0;		/* it seems harmless */

      dfprintf((stderr,"butm_WriteFileData: writing data, count = %d, block = %x\n",
		count, data));
      code = write (p->fid, data, BUTM_BLOCKSIZE);
      if ( code != BUTM_BLOCKSIZE)
      {
	/* encountered an error */
	  info->status |= BUTM_STATUS_WRITEERROR;
	  return ( (code == -1) ? BUTM_IO : BUTM_EOT);
      }

      incSize(info,BUTM_BLOCKSIZE);
      if (isafile) 
	info->position++;
      return 0;
}

static long file_WriteFileEnd (info)
  struct butm_tapeInfo *info;
{   
    struct progress *p;
    long  code;
    struct markStruct mark;

#ifndef TESTINGTM
	pthread_yield();
#endif
    dfprintf((stdout, "butm_WriteFileEnd: writing file end\n"));
    if (code = check (info, WRITE_OP)) return code;
    p = (struct progress *)info->tmRock;
    if (p->reading || !p->writing) return BUTM_BADOP;

    bzero((char *)&mark,BUTM_HDRSIZE);
    mark.count = FILE_END;
    mark.magic = FILE_MAGIC;
    code = WriteFileMark(info, p->fid,&mark);
    if(code) {
	info->status |= BUTM_STATUS_WRITEERROR;
	return BUTM_IO;
    }
    code = WriteEOF(info, p->fid,1);
    if(code) { 
	info->status |= BUTM_STATUS_WRITEERROR;
	return BUTM_IO;
    }
    if (!isafile)
      info->position++;

    p->writing = 0;
    return 0;
}

/* file_WriteFileDataCopy
 *	Write a data block from tape and copies the data from the
 *      application buffer into the tape block.
 * entry:
 *	info - tape info structure, c.f. fid
 *	data - ptr to buffer for data
 *      len  - size of buffer for data
 */

static long
file_WriteFileDataCopy (info, data, len)
  struct butm_tapeInfo *info;
  char *data;
  int  len;
{   
      long code;
      char *databuf;

      databuf = (char *)osi_Alloc(BUTM_BLOCKSIZE);
      if ( !databuf ) return(BUTM_NOMEM);
      bcopy(data,databuf+BUTM_HDRSIZE,len);
      code = file_WriteFileData(info,databuf,len);
      osi_Free(databuf,BUTM_BLOCKSIZE);
      return(code);
}

static long file_WriteEOT (info)
  struct butm_tapeInfo *info;
{   struct progress *p;
    long  code;
    struct markStruct mark;
    int i;

#ifndef TESTINGTM
	pthread_yield();
#endif
    dfprintf((stdout, "butm_WriteEOT: writing end of tape]\n"));
    if (code = check (info, WRITE_OP)) return code;
    p = (struct progress *)info->tmRock;
    if (p->reading || p->writing) return BUTM_BADOP;
    mark.count = TM_EOT;
    mark.magic = FILE_MAGIC;
    for(i = 0; i < 4; i++){
	code = WriteFileMark(info, p->fid,&mark);/* beginning of file mark */
	if(code) {
	    info->status |= BUTM_STATUS_WRITEERROR;
	    return BUTM_IO;
	}
	code = WriteFileMark(info, p->fid,&mark);/* end of file mark */
	if(code) {
	    info->status |= BUTM_STATUS_WRITEERROR;
	    return BUTM_IO;
	}
	code = WriteEOF(info, p->fid,1); /* write 4 eof markers to indicate eot */
	if(code) {
	    info->status |= BUTM_STATUS_WRITEERROR;
	    return BUTM_IO;
	}
    }
    if (!isafile)
      info->position += 4;

    info->status |= BUTM_STATUS_EOT;
    return 0;
}

static long file_SetSize (info,size)
struct butm_tapeInfo *info;
long size;
{
    if(size <= 0 ){
	info->tapeSize = config.capacity;
     }
    info->tapeSize = size;
    dfprintf((stderr, "butm_SetSize: tape size = %lu\n", info->tapeSize));
    return 0;

}

static long file_GetSize (info,size)
struct butm_tapeInfo *info;
long *size;
{
    *size = info->tapeSize;
    dfprintf((stderr, "butm_GetSize: tape size = %lu\n", info->tapeSize));
    return 0;

}

static long file_Configure (file)
  struct tapeConfig *file;
{   
    if(file == 0) {
	return BUTM_BADCONFIG;
    }
    dfprintf((stderr, "butm_Configure: configuring tape\n"));
    config = *file;
    if (config.capacity <
	    ((sizeof(struct butm_tapeLabel)+BUTM_HDRSIZE)/1024)) {
	return BUTM_BADCONFIG;
    }
    config.capacity -= ((sizeof (struct butm_tapeLabel))/1024);
    if (sizeof(config.device) == 0) {
	return BUTM_BADCONFIG;
    }
    return 0;
}

/* This procedure instantiates a tape module of type file_tm. */

long butm_file_Instantiate (info, file)
  struct butm_tapeInfo *info;
  struct tapeConfig *file;
{   long  code;

    dfprintf((stdout, "butm_File_Instatiate: instantiate a tape module\n"));
    if ( (info == (struct butm_tapeInfo *)NULL)
    ||   (info->structVersion != CUR_BUTM_VERSION)
       )
    	return BUTM_OLDINTERFACE;
    bzero ((char *)info, sizeof(struct butm_tapeInfo));
    info->structVersion = CUR_BUTM_VERSION;
    info->ops.mount = file_Mount;
    info->ops.dismount = file_Dismount;
    info->ops.create = file_Create;
    info->ops.readLabel = file_ReadLabel;
    info->ops.seek = file_Seek;
    info->ops.readFileBegin = file_ReadFileBegin;
    info->ops.readFileData = file_ReadFileData;
    info->ops.readFileDataCopy = file_ReadFileDataCopy;
    info->ops.readFileEnd = file_ReadFileEnd;
    info->ops.writeFileBegin = file_WriteFileBegin;
    info->ops.writeFileData = file_WriteFileData;
    info->ops.writeFileDataCopy = file_WriteFileDataCopy;
    info->ops.writeFileEnd = file_WriteFileEnd;
    info->ops.writeEOT = file_WriteEOT;
    info->ops.setSize = file_SetSize;
    info->ops.getSize = file_GetSize;

    code = file_Configure (file);
    return code;
}


/* incSize
 *	add the supplied no. of bytes to the byte count of information placed
 *	on the tape.
 * entry:
 *	dataSize - bytes used on the tape
 */

incSize(info, dataSize)
     struct butm_tapeInfo *info;
     long dataSize;
{
    info->nBytes += dataSize;
    info->kBytes += (info->nBytes/1024);
    info->nBytes %= 1024;   
}


