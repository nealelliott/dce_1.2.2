/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: fms.c,v $
 * Revision 1.1.74.1  1996/10/02  17:03:07  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:04:34  damon]
 *
 * $EndLog$
*/

#include <dcedfs/param.h>				/* for machine/host info */
#include <dcedfs/osi.h>
#include <curses.h>
#include <sys/file.h>
#include <sys/ioctl.h>
#include <dcedfs/cmd.h>

#if !defined(KERNEL) 
#include <pthread.h>
#endif /* !KERNEL */

/* for tape handling */

#ifdef AFS_AIX_ENV				/* rs_aix31 */
#include <sys/tape.h>

/* now a set of compatibility defines */
#define	mtop		stop			/* operations structure */
#define	mt_op		st_op
#define	mt_count	st_count

#define	MTIOCTOP	STIOCTOP		/* operations command */

#define	MTBSF		STRSF
#define	MTBSR		STRSR
#define	MTFSF		STFSF
#define	MTFSR		STFSR
#define	MTREW		STREW
#define	MTWEOF		STWEOF
#else /* AFS_AIX_ENV */
#include <sys/mtio.h>
#endif /* AFS_AIX_ENV */

/* structure for writing data to tape */

typedef struct tapeDataBuffer
{
    struct tapeDataBuffer	*tdb_next;
    char 			*tdb_buffer;
} tapeDataBufferT;
typedef tapeDataBufferT *tapeDataBufferP;

/* globals */
char *tapeDevice = 0;				/* device pathname */
long tapeFd = -1;				/* file descriptor of above */
long eotEnabled = 1;

/* forward refs */
extern tt_fileMarkSize();

#define ERROR(evalue)                                           \
        {                                                       \
            code = evalue;                                      \
            goto error_exit;                                    \
        }

#define MAXV	100

#ifdef sigaction
#undef sigaction
#endif

main(argc, argv)
     int    argc;
     char **argv;
{
    struct sigaction intvec, oldvec;

    struct cmd_syndesc *cptr;

    void quitFms();

    osi_setlocale(LC_ALL, "");
    bzero((char *)&intvec, sizeof(intvec));
    intvec.sa_handler = quitFms;

    sigaction(SIGINT, &intvec, &oldvec);

    cptr = cmd_CreateSyntax((char *) 0, tt_fileMarkSize, 0, 
			    "write a tape full of file marks");
    cmd_AddParm(cptr, "-tape", CMD_SINGLE, CMD_REQUIRED, "tape special file");

    cmd_Dispatch(argc, argv);
}

tt_fileMarkSize(as, arock)
     struct cmd_syndesc *as;
     char *arock;
{
    char *tapeDevice;

    tapeDevice = as->parms[0].items->data;

    fileMarkSize(tapeDevice);
}


fileMarkSize(tapeDevice)
     char *tapeDevice;
{
    unsigned long nFileMarks, nBlocks, nbfTape;
    unsigned long fmSize;
    unsigned long bufferSize = 16384;
    int tapeFd;
    FILE *logFile;
    int count = 0;
    int xpos, ypos;
    long code = 0;

    long rewindTape();

    initscr();
    clear();

    tapeFd = open(tapeDevice, O_RDWR);
    if ( tapeFd < 0 )
    {
	printf("Can't open tape device %s\n", tapeDevice);
	fflush(stdout);
	exit(1);
    }

    logFile = fopen("FMSLog", "w+");
    if ( logFile == NULL )
    {
	printf("Can't open log file\n");
	fflush(stdout);
	exit(1);
    }
    fprintf(logFile, "fms test started\n");
    fflush(logFile);

    code = rewindTape(tapeFd);
    if ( code )
    {
	fprintf(logFile, "Can't rewind tape\n");
	fflush(logFile);
	ERROR(code);
    }

    /* measure capacity of tape */
    nbfTape = 0;
    while ( 1 )
    {
	code = dataBlock(tapeFd, bufferSize);
	nbfTape++;
	count++;
	if ( code )
	    break;
	if ( count >= 5 ) 
	{
	    count = 0;
	    getyx(stdscr, ypos, xpos);
	    printw("wrote block: %d\n", nbfTape);
	    wmove(stdscr, ypos, xpos);
	    refresh();
	}
    }

    fprintf(logFile, "wrote %d blocks\n", nbfTape);
    fflush(logFile);
    wmove(stdscr, ypos+1, xpos);
    printw("Finished data capacity test - rewinding\n");
    refresh();
    /* reset the tape device */
    close(tapeFd);
    tapeFd = open(tapeDevice, O_RDWR);
    if ( tapeFd < 0 )
    {
	fprintf(logFile, "Can't open tape device for pass 2\n");
	fflush(logFile);
	printw("Can't open tape device %s\n", tapeDevice);
	goto error_exit;
    }

    code = rewindTape(tapeFd);
    if ( code )
    {
	fprintf(logFile, "Can't rewind tape\n");
	fflush(logFile);
	ERROR(code);
    }

    /* now measure file mark size */
    nFileMarks = 0;
    nBlocks = 0;
    count = 0;
    while ( 1 )
    {
	code = dataBlock(tapeFd, bufferSize);
	nBlocks++;
	if ( code )
	    break;
	code = fileMark(tapeFd);
	nFileMarks++;
	if ( code )
	    break;
	count++;

	if ( count >= 2 )
	{
	    count = 0;
	    getyx(stdscr, ypos, xpos);
	    printw("wrote %d blocks, %d filemarks\n", 
		   nBlocks, nFileMarks);
	    wmove(stdscr, ypos, xpos);
	    refresh();
	}
    }
    wmove(stdscr, ypos+1, xpos);
    printw("Finished filemark test\n");
    refresh();
    fmSize = ((nbfTape - nBlocks)*bufferSize)/nFileMarks;
    printw("Tape capacity is %.0f bytes\n",
	   (double)nbfTape*(double)bufferSize);
    printw("File marks are %d bytes\n", fmSize);
    fprintf(logFile, "Tape capacity is %.0f bytes\n",
	   (double)nbfTape*(double)bufferSize);
    fprintf(logFile, "File marks are %d bytes\n", fmSize);
    fflush(logFile);
    fclose(logFile);
    refresh();
error_exit:
    endwin();
    return(code);
}

void
quitFms(int x)
{
    endwin();
    exit(0);
}


/* --------------------------
 * device handling routines
 * --------------------------
 */

long
rewindTape(tapeFd)
     int tapeFd;
{
    struct mtop mtop;

    mtop.mt_op = MTREW;
    mtop.mt_count = 1;
    return(ioctl(tapeFd, MTIOCTOP, &mtop));
}


fileMark(tapeFd)
     int tapeFd;
{
    struct mtop mtop;

    mtop.mt_op = MTWEOF;
    mtop.mt_count = 1;
    return(ioctl(tapeFd, MTIOCTOP, &mtop));
}

/* dataBlock
 *	write a block of data on tape
 * entry:
 * 	blocksize - size of block in bytes
 */

dataBlock(tapeFd, reqSize)
     int tapeFd;
     long reqSize;
{
    static char *dB_buffer = 0;
    static long dB_buffersize = 0;
    static int dB_count = 0;
    int *ptr;
    long code = 0;

    /* dbBuffersize is only valid when dB_buffer is non-zero */

    if ( (dB_buffer != 0)
    &&   (dB_buffersize != reqSize )
       )
    {
	free(dB_buffer);
	dB_buffer = 0;
    }

    if (dB_buffer == 0 )
    {
	dB_buffer = (char *) malloc(reqSize);
	if ( dB_buffer == 0 )
	    ERROR(-1);
	dB_buffersize = reqSize;
	bzero(dB_buffer, dB_buffersize);
    }

    ptr = (int *) dB_buffer;
    *ptr = dB_count++;

    if ( write(tapeFd, dB_buffer, dB_buffersize) != dB_buffersize )
	ERROR(-1);

error_exit:
    return(code);
}







