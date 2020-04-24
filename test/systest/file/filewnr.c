/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: filewnr.c,v $
 * Revision 1.1.19.2  1996/02/17  23:27:01  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:36  marty]
 *
 * Revision 1.1.19.1  1995/12/11  22:00:10  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/jrr_1.2_mothra/1  1995/11/14  21:01 UTC  dat
 * 	Remove malloc prototype
 * 	[1995/12/11  20:59:18  root]
 * 
 * Revision 1.1.17.2  1994/02/23  21:43:44  pellis
 * 	Changed std.h to dce.h
 * 	[1994/02/23  21:38:12  pellis]
 * 
 * Revision 1.1.17.1  1994/02/04  20:48:50  devsrc
 * 	Merged from 1.0.3a to 1.1
 * 	[1994/02/04  15:26:15  devsrc]
 * 
 * Revision 1.1.14.3  1994/02/02  17:42:22  gmd
 * 	Per 9870, if locking, open the file for read-write.
 * 	Added OPEN_FAILURE for exiting when open_file fails.
 * 	[1994/02/02  17:42:00  gmd]
 * 
 * Revision 1.1.14.2  1994/01/25  22:28:08  rsarbo
 * 	CR9763, re-open stdout and stderr connected to logfile on local host;
 * 	avoids passing back through remsh which can jumble the stdout
 * 	and stderr.  also open with O_RDONLY for the read case.  clean up
 * 	stuff that was included to deal with the jumbling of the stdout
 * 	and stderr.  beef up some error handling.
 * 	[1994/01/25  22:25:24  rsarbo]
 * 
 * Revision 1.1.14.1  1994/01/13  22:31:49  rsarbo
 * 	insert sleep() in tight read loop in wait_on_message() that
 * 	was causing severe performance problem.  Attempt to read
 * 	the synch file once every 15 seconds up to 8 minutes, then
 * 	give up and fail the test.
 * 	[1994/01/13  22:30:09  rsarbo]
 * 
 * Revision 1.1.11.4  1993/11/03  16:02:40  gmd
 * 	Replaced poor read_file algorithm for
 * 	randomly reading "blocks" of file so
 * 	that the algorithm now makes use of
 * 	every RAND call. Previously, blocks
 * 	were not read until their number was
 * 	generated. Now, compacting array with
 * 	every call and reading block pointed
 * 	to by array entry chosen randomly.
 * 	[1993/11/03  16:02:16  gmd]
 * 
 * Revision 1.1.11.3  1993/10/14  17:37:03  cmckeen
 * 	For CR 7671, included <dce/std.h> for definition of TRUE and FALSE
 * 	[1993/10/14  17:35:17  cmckeen]
 * 
 * Revision 1.1.11.2  1993/10/01  17:31:48  mort
 * 	Only use O_NONBLOCK flag for open().
 * 	Add "do_exit (BAD_OPTIONS);" to end of load_options().
 * 	[1993/10/01  17:29:49  mort]
 * 
 * Revision 1.1.11.1  1993/09/13  20:40:43  damon
 * 	Synched with ODE 2.1 based build
 * 	[1993/09/13  20:40:24  damon]
 * 
 * 	OT 8578: Port to HP-UX by including the following:
 * 	   Revision 1.1.9.3  1993/09/07  16:37:29  mhickey
 * 	Ported to HP-UX
 * 	   Revision 1.1.9.2  1993/08/13  16:04:13  mhickey
 * 	Changed the references to random for a sleep
 * 	value to the correct variable 'r'.  Using random (actually
 * 	a function call) was causing a 11974653 second
 * 	sleep.
 * 	defect 8447.
 * 	   Revision 1.1.5.5  1993/07/30  15:43:54  gmd
 * 	Corrected variable types from "int" to "long"
 * 	to better handle large calculations (ie. to
 * 	support maxfile testing).
 * 	[1993/07/30  15:03:42  gmd]
 * 
 * Revision 1.1.5.4  1993/06/21  20:36:18  mhickey
 * 	Fixed the -d switch to once again mean delete the file, removed the
 * 	extra debug switch, and added info on the -h, -C and -T options
 * 	to the usage strings output.  I also ordered the options
 * 	alphabetically.
 * 
 * 	Defect 8186.
 * 	[1993/06/21  20:02:51  mhickey]
 * 
 * Revision 1.1.5.3  1993/06/10  20:06:04  mhickey
 * 	Added wait_on_mess to wait for synchronization messages
 * 	in the waitfile, if -C <mess> is specified, better logging,
 * 	client id flag (-c), teminate message flag (-T).  Cleaned up
 * 	lock_file.  Added macro for perror to print system error
 * 	messages to stdout.
 * 
 * 	Defect 8138.
 * 	[1993/06/10  19:59:24  mhickey]
 * 
 * Revision 1.1.5.2  1993/05/07  20:35:37  gmd
 * 	Handle empty and incomplete test files by checking
 * 	the file size with right_size_file(). This prevents
 * 	dropping into an infinite loop in read_pattern() when
 * 	the file is zero length OR the pattern was not written
 * 	completely. NOTE: read() does NOT return -1 when asked
 * 	to read beyond EOF!
 * 	[1993/05/07  20:34:47  gmd]
 * 
 * Revision 1.1.2.5  1993/02/05  15:20:39  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:40:56  cjd]
 * 
 * Revision 1.1.3.2  1993/02/04  22:07:48  cjd
 * 	Embedded copyright notice
 * 
 * Revision 1.1.2.4  1993/01/08  19:32:17  gmd
 * 	Updated to sleep random # of seconds (within given range) between
 * 	attempts to open "waitfor" file.
 * 	Cleaned up reporting by moving it to routines report_params and
 * 	report_successes.
 * 	[1993/01/08  19:30:33  gmd]
 * 
 * Revision 1.1.2.3  1993/01/07  19:03:06  gmd
 * 	Added lock_file() and supporting logic.
 * 	Also added -w option and logic to force the program to
 * 	wait for a certain file to exist before starting.
 * 	Added check for verbose mode so if the program fails AND
 * 	the details have already been printf'd, it doesn't printf
 * 	them again.
 * 	[1993/01/07  18:53:16  gmd]
 * 
 * Revision 1.1.2.2  1993/01/03  23:31:34  gmd
 * 	Original version.
 * 	[1993/01/03  23:29:50  gmd]
 * 
 * $EndLog$
 */
/***************************************************************************
 * TITLE: filewnr.c
 *
 * ASSOCIATION:
 *      This script is part of the DCE DFS system test suite.  The DCE
 *      system test plan describes fully the goals of this test.
 *
 * OBJECTIVE:
 *	To exercise file open, lock, write and read operations.
 *
 * DESCRIPTION:
 *	file write 'n read program:
 *	Opens file and writes and/or reads pattern at offset in "blocks"
 *	of bytes for all blocks. If a lock type is specified (see usage 
 *      strings for details), then the program attempts to get the specified
 *      lock after opening the file.  If the -w <waitfile> option is 
 *      specified, then the program polls the filesystem for the existence of
 *      waitfile before attempting to open the file.  If the 
 *      -C <continue_message> option is specified, then the program polls the
 *      waitfile (which must also be specified) for the <continue_message> 
 *      after opening the file before writing/reading the file.
 *      Note that the default values create a 8kbyte+ file containing "holes";
 *      ie. unwritten bytes.
 *
 * PROCESSING:	print STARTing message
 *              if (wait_flag)
 *		  loop until waitfilename exists
 *                sleep (random%SLEEPRANGE)
 *              print READY message
 *		if not read_only
 *                open file
 *                if lock type is specified
 *                   try the lock
 *                   if success
 *                      print TRIED LOCK / GOT LOCK message
 *                   else
 *                      print TRIED LOCK / NO LOCK message
 *                      print EXITTED / FAILED message
 *                      exit
 *                if continue message is specified
 *                   loop waiting for the message to appear in the wait file
 *                   if we found the message
 *                      print FOUND message
 *                   else
 *                      if we can't open the wait file,
 *                      print EXITTED / FAILED message
 *                   exit
 *                for each block
 *                  write pattern at offset
 *              close file
 *		open file
 *                if lock type is specified
 *                   try the lock
 *                   if success
 *                      print TRIED LOCK / GOT LOCK message
 *                   else
 *                      print TRIED LOCK / NO LOCK message
 *                      print EXITTED / FAILED message
 *                      exit
 *                if continue message is specified
 *                   loop waiting for the message to appear in the wait file
 *                   if we found the message
 *                      print FOUND message
 *                   else
 *                      if we can't open the wait file,
 *                      print EXITTED / FAILED message
 *                   exit
 *		if file size correct
 *		  for each block (randomly chosen)
 *                  read patternlen # of bytes at offset
 *                  compare what was read to pattern
 *                  if no match
 *                     print EXITTED / FAILED message
 *              else
 *                  print EXITTED / FAILED message
 *              close file
 *              print EXITTED / PASSED message
 *
 * SYNTAX:
 *	see usage_strings.
 *
 * OUTPUT: 1 of exit values defined below
 *         messages about READY, TRIED LOCK, "started write", "finished write",
 *                        "Started read", "Finished read", etc.
 *         if -v
 *           logging of parameters used and successes/failures
 *           along the way 
 *           if -C <continue_message> 
 *              status of polling waitfile for message
 *           (stdout for info and success, stderr for failures)
 *
 * CALLED FROM:
 *      Command line, dfs.lock, dfs.glue
 *
 * CALLS INTO:
 *      libc.{a|so}
 **************************************************************************/

#include <errno.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>

#if !defined(__hpux)
#   include <sys/mode.h>
#endif /*---* if !defined(__hpux) *---*/

#include <sys/param.h>
#include <sys/mount.h>
#include <sys/utsname.h>
#include <dce/sec_login.h>
#include <dce/dce.h>

#if defined(_AIX)
#   include <sys/statfs.h>
#   include <net/nh.h>
#endif /*---* if defined(_AIX) *---*/

/*---* 
 *---* On HP-UX, we get the statfs struct definition from someplace else, 
 *---* and have to do something about filesystem types it doesn't know
 *---* about to avoid compilation errors.  We also pull in the macros for 
 *---* htonl et al.
 *---* HP-UX does not have the random/srandom calls either.  To make this
 *---* portable, we will define RAND and SRAND to be rand and srand for 
 *---* HPUX, and random and srandom for all others.
 *---*/
#if defined(__hpux)
#   include <sys/vfs.h>   /*---* for the statfs struct *---*/
#   include <netinet/in.h>

/*---* define a junk value for the unknown filesystem types. *---*/
#   if !defined(MOUNT_NONE)
#      define MOUNT_NONE  1000
#   endif  /*---* !defined(MOUNT_NONE) *---*/
#   if !defined(MOUNT_MFS)
#      define MOUNT_MFS 1001
#   endif  /*---* !defined(MOUNT_MFS) *---*/
#   if !defined(MOUNT_S5FS)
#      define MOUNT_S5FS  1002
#   endif  /*---* !defined(MOUNT_M5FS) *---*/

/*---* defines for random function *---*/
#   define RAND rand
#   define SRAND srand
#endif  /*---* if defined(__hpux) *---*/

#if !defined(RAND)
#   define RAND random
#endif  /*---* if defined(RAND) *---*/
#if !defined(SRAND)
#   define SRAND srandom
#endif  /*---* if defined(SRAND) *---*/

/* exit values */
#define COMPLETE_SUCCESS 0
#define COMPLETE_FAILURE 1
#define USAGE 2
#define BAD_OPTIONS 3
#define BAD_WAIT 4
#define BAD_LOCK 5
#define SYNCH_ERROR 6
#define SYNCH_MESS 7
#define OPEN_FAILURE 8

/*---* 
 *---* These may not be defined elsewhere
 *---*/

/*---*
 *---* return values for functions
 *---* 
 *---* lock_file()
 *---*/
#define lock_file_no_lock 0
#define lock_file_got_lock 1
/*---* wait_on_message() *---*/
#define wait_on_mess_open 0
#define wait_on_mess_continue 1
#define wait_on_mess_exit 2
#define wait_on_mess_space 3

/* default values */
/* combined numblocks X blocksize should be > 64k
 * to outgrow default dfs "chunk" size - all i/o in chunks
 * (note that numblocks => number of reads/writes)
 */
#define NUMBLOCKS 10
#define BLOCKSIZE 8193
#define OFFSET 22
#define MAXPIDLEN 20
#define MAXIDENTLEN MAXHOSTNAMELEN+MAXPIDLEN+4+1
#define MAXPATTERNLEN MAXIDENTLEN
#define LONGBYTES sizeof(long)
#define SLEEP_RANGE 10
#define WAIT_ON_MSG_ATTEMPTS 32
#define WAIT_ON_MSG_SLEEP 20

/* local typedefs */
typedef  struct input {
  char *filename; /* file to write and/or read */
  char *waitfilename; /* file to wait for */
  char *pattern;  /* pattern to write and/or read */
  char *locktype;  /* EX for exclusive write, SR for shared read */
  long bsize; /* size of "blocks" to write and/or read */
  long bnum; /* number of "blocks" to write and/or read */
  char *cont_mess;
  long offset; /* offset within blocks to write and/or read at */
  int cli_numb; /*---* client identification number *---*/
  int read_only_flag; /* flag to indicate whether just reading */
  int delete_flag; /* flag to indicate remove file when done (default is to leave file around) */
  int verbose_flag; /* flag to turn on erbose mode */
  int lock_flag; /* flag to indicate lock mode requested */
  char *quit_mess;
  int wait_flag; /* flag to indicate wait mode requested */
  int sleep_range; /* sleep a random number of seconds between 0 and this */
  int patternlen; /* number of bytes to write and/or read */
} input_struct;

typedef  struct full_pattern {
  char *pattern; /* pattern to write and/or read */
  long bnumon; /* block number we're on */
  unsigned long checksum; /* checksum */
  char *ident;
} pattern_struct;

/* External */
extern pid_t getpid();
extern void perror();
extern char *optarg;
extern int errno;
extern long random();
extern void srandom();

#if !defined(__hpux)
extern long random();
extern void srandom();
#endif

/* Internal */
unsigned long additup();
void close_file();
void do_exit(int);
int lock_file();
int open_file();
void initialize_variables();
void load_options();
int wait_on_message(char *, char *, char *, char *);
void write_file();
void write_pattern();
void read_file();
void read_pattern();
void report_fs();
void report_params();
void report_successes();
void report_whoiam();
void usage();

/* Global Variables */
long errorcount; /* running total of errors */
long successfulwrites;
long successfulreads;
long successfulcompares;
pid_t pid;
char ident[MAXIDENTLEN]; /* default pattern to write and/or read */
struct utsname uname_vals;  /*---* info on local system *---*/
input_struct input;	/* structure of input values */

char* usage_strings[] = {
  "\n",
  "usage: filewnr [-b <bsize>] [-C <okay_message>] [-c <cli_num] [-d] \n",
  "               [-f <filename>] [-h] [-l <locktype>] [-n <nblocks>]\n",
  "               [-o <offset>] [-p <pattern>] [-r] [-s <sleep_range>]\n",
  "               [-T <quit_message>] [-w <waitfile>]\n",
  " Where:\n"
    "    -b bsize         size of blocks to write and/or read.\n",
    "    -C okay_message  string (to be written into the wait file) that \n",
    "                     indicates it is okay to do write or read.\n",
    "    -c client_num    client identification number for driver.\n",
    "    -d               delete_flag - delete file when done.\n",
    "    -f filename      to write and/or read.\n",
    "    -h               print this usage message.\n",
    "    -l locktype      \"EW\" for exclusive write, \"SR\" for shared read.\n",
    "    -n nblocks       number of blocks to write and/or read.\n",
    "    -o offset        to write and/or read at.\n",
    "    -p pattern       to write and/or read.\n",
    "    -r               read_only_flag.\n",
    "    -s sleep_range   sleep [0 - this value] random seconds when waiting.\n",
    "    -T quit_message  string (to be written into the wait file) that \n",
    "                     indicates that the test should exit.\n",
    "    -v               verbose flag - more prints about status (can get\n",
    "                                    big when looking for a continue\n",
    "                                    message).\n",
    "    -w waitfilename  don't start 'til this file exists.\n",
    (char *)0
    };

/*---* Default quit message for wait_on_mess *---*/
#define QUIT_MESS "TERMINATE TEST"

/*---* lower bound on sleep interval *---*/
#define SLEEPRANGE_LOWBOUND 50

#define LOG_ERROR(s, count)	{ perror(s); count++; }

/*
 * Main procedure
 * No looping here - load defaults, load input and go.
 */

main (int argc, char **argv)
{
  int fd;
  long r;
  int ret_val;
  time_t tbuf;
  
  /*---* 
   *---* Now load the input struct with defaults
   *---*/
  initialize_variables (&input, ident);
  
  /*---* 
   *---* Over-ride the default values with any values specified on the 
   *---* command line.
   *---*/
  load_options (argc, argv, &input);
  
  /*---* now we report our start time as a first indication that we live *---*/
  pid=getpid();
  (void)time(&tbuf);
  fprintf(stdout, "Client(%d): Starting at %s...\n", pid, ctime(&tbuf));
  
  /*---* now get the system information from uname and report it *---*/
  uname(&uname_vals);
  fprintf(stdout, "Client(%d): PID: %d System: %s; O/S: %s...\n", 
	  input.cli_numb, pid, uname_vals.nodename, uname_vals.sysname);
  
  
  
  if (input.verbose_flag)
    {
      report_params (&input);
      report_whoiam ();
    }
  
  /*---* 
   *---* Print message indicating that we are ready.
   *---*/
  (void)time(&tbuf);
  fprintf (stdout, 
	   "Client( %d ): PID: %d System: %s ; O/S: %s lock type: %s - READY at %s\n",
	   input.cli_numb, pid, uname_vals.nodename, uname_vals.sysname, 
	   input.locktype, ctime(&tbuf));
  
  /*---*
   *---* if asked to wait awhile, do so.  The driver always calls one client 
   *---* with a sleep range of 0 (the particular client changes with each 
   *---* iteration of the test), so that each machine gets a chance to be the
   *---* one to get the lock.
   *---*/
  if (input.wait_flag)
    {
      /* seed and create random # for seconds to sleep */
      SRAND (pid);
      /*---* 
       *---* explicitly set the sleep value to 0 if sleep_range is 0 to
       *---* avoid the divide by zero.  If we get a sleep value, we
       *---* add SLEEPRANGE_LOWBOUND to the randomized value to prevent
       *---* the client from getting a random value of 0.
       *---*/
      r = input.sleep_range ? 
	((RAND () % input.sleep_range) + SLEEPRANGE_LOWBOUND): 0;
      
      (void)time(&tbuf);
      fprintf(stdout, "%s: Looking for %s at %s (%d sec sleep)\n", ident, 
	      input.waitfilename, ctime(&tbuf), r);
      
      while (((fd = open (input.waitfilename, O_RDONLY, 0)) == -1) &&
	     (errno == ENOENT))
	{
          
	  sleep (r);
	}
      if (fd != -1)
	{
	  close_file (fd);
	  sleep (r);
	}
      else
	{
	  LOG_ERROR("opening sync file", errorcount)
	    do_exit (BAD_WAIT);
	}
    }
  
  (void)time(&tbuf);
  fprintf(stdout, "%s: Found %s at %s\n", ident, input.waitfilename, 
	  ctime(&tbuf));
  
  if (!input.read_only_flag)
    {
      if (input.verbose_flag)
	{
	  (void)time(&tbuf);
	  fprintf(stdout, "%s: Trying to open %s at %s\n", ident, input.filename,
		  ctime(&tbuf));
	}
      fd = open_file (input.filename, O_RDWR | O_CREAT | O_SYNC, 
		      S_IRWXU);
      if (fd == -1)
	do_exit (OPEN_FAILURE);
      
      if (input.lock_flag)
	{
	  /*---* try for the requested lock *---*/
	  if (lock_file (fd, input.locktype) == lock_file_no_lock)
	    { 
	      (void)time(&tbuf);
	      /*---* didn't get it, report and quit *---*/
	      fprintf(stdout,
		      "Client( %d ): PID: %d System: %s ; O/S: %s lock type: %s - TRIED LOCK : NO LOCK at %s\n",
		      input.cli_numb, pid, uname_vals.nodename, uname_vals.sysname, 
		      input.locktype, ctime(&tbuf));
	      close(fd);
	      do_exit (BAD_LOCK);
	    }
	  else
	    {
	      /*---* Got it!! Report and continue *---*/
	      (void)time(&tbuf);
	      fprintf(stdout,
		      "Client( %d ): PID: %d System: %s ; O/S: %s lock type: %s - TRIED LOCK : GOT LOCK at %s\n",
		      input.cli_numb, pid, uname_vals.nodename, uname_vals.sysname, 
		      input.locktype, ctime(&tbuf));
	    }
	}
      
      if (input.verbose_flag)
	report_fs (fd);
      
      /*---*
       *---* if we got a continue message on the command line, then 
       *---* we poll the synch file for the message that says every
       *---* one tried the lock and we should go on, or else tells us to
       *---* quit.
       *---*/
      if (input.cont_mess != (char *)NULL)
	{
	  if ((ret_val=wait_on_message(input.waitfilename, input.locktype, 
				       input.cont_mess, input.quit_mess)) != wait_on_mess_continue)
	    {
	      /*---*
	       *---* we should quit either because we can't open the synch
	       *---* file, or because the driver says so.
	       *---*/
	      close_file(fd);
	      do_exit(ret_val == wait_on_mess_open ? SYNCH_ERROR:SYNCH_MESS);
	    }
	}
      
      /*---* we can proceed to write the file *---*/
      write_file (&input, fd);
      close_file (fd);
    }
  
  /*---* if we are to try a lock *---*/
  if (input.lock_flag)
    fd = open_file (input.filename, O_RDWR, 0);
  else
    fd = open_file (input.filename, O_RDONLY, 0);
  
  if (fd == -1)
    do_exit (OPEN_FAILURE);
  
  if (input.lock_flag)
    {
      /*---* try for the requested lock *---*/
      if (lock_file (fd, input.locktype) == lock_file_no_lock)
	{ 
	  (void)time(&tbuf);
	  /*---* didn't get it, report and quit *---*/
	  fprintf(stdout,
		  "Client( %d ): PID: %d System: %s ; O/S: %s lock type: %s - TRIED LOCK : NO LOCK at %s\n",
		  input.cli_numb, pid, uname_vals.nodename, uname_vals.sysname, 
		  input.locktype, ctime(&tbuf));
	  close(fd);
	  do_exit (BAD_LOCK);
	}
      else
	{
	  /*---* Got it!! Report and continue *---*/
	  (void)time(&tbuf);
	  fprintf(stdout,
		  "Client( %d ): PID: %d System: %s ; O/S: %s lock type: %s - TRIED LOCK : GOT LOCK at %s\n",
		  input.cli_numb, pid, uname_vals.nodename, uname_vals.sysname, 
		  input.locktype, ctime(&tbuf));
	}
    }
  
  if (input.verbose_flag)
    report_fs (fd);
  
  /*---*
   *---* if we got a continue message on the command line, then 
   *---* we poll the synch file for the message that says every
   *---* one tried the lock and we should go on, or else tells us to
   *---* quit.
   *---*/
  if (input.cont_mess != (char *)NULL)
    {
      if ((ret_val=wait_on_message(input.waitfilename, input.locktype, 
				   input.cont_mess, input.quit_mess)) != wait_on_mess_continue)
        {
	  /*---*
	   *---* we should quit either because we can't open the synch
	   *---* file, or because the driver says so.
	   *---*/
	  close_file(fd);
	  do_exit(ret_val == wait_on_mess_open ? SYNCH_ERROR : SYNCH_MESS);
        }
    }  
  if (right_size_file (&input, fd))
    read_file (&input, fd);
  
  close_file (fd);
  
  if (input.delete_flag)
    {
      if (unlink (input.filename))
	LOG_ERROR("unlink in main()", errorcount)
	}
  
  if (input.verbose_flag)
    report_successes ();
  
  if (errorcount)
    {
      fprintf (stdout, "%s : FAILED\n", ident );
      
      /* if you haven't reported them already, give the details */
      if (!input.verbose_flag)
	{
	  report_params (&input);
	  report_whoiam ();
	  report_successes ();
	}
      
      do_exit (COMPLETE_FAILURE);
    }
  else
    {
      fprintf (stdout, "%s : PASSED\n", ident );
      do_exit (COMPLETE_SUCCESS);
    }
}

/*---------------------------------------------------------------------------*
 *---* Lock_file(fd, locktype) - Attempt to lock the file as requested   *---*
 *---*                           If the requested lock can't be obtained *---*
 *---*                           then return 0, else return 1.           *---*
 *---------------------------------------------------------------------------*/
int lock_file (int fd, char *locktype)
{
  struct flock lock_desc;
  
  /* lock the whole file */
  lock_desc.l_whence = SEEK_SET;
  lock_desc.l_start = 0;
  lock_desc.l_len = 0;
  
  /*---* set the lock type *---*/
  lock_desc.l_type = ( strcmp(locktype, "EW") ? F_RDLCK : F_WRLCK );
  
  if (input.verbose_flag)
    {
      if (lock_desc.l_type == F_RDLCK)
        fprintf(stdout,"%s: lock type is F_RDLCK\n", ident);
      else if (lock_desc.l_type == F_WRLCK)
        fprintf(stdout,"%s: lock type is F_WRLCK\n", ident);
      else 
        fprintf(stdout,"%s: lock type is unknown (%d)\n", ident, lock_desc.l_type);
    }
  /*---* 
   *---* ask for it...
   *---* if we can't get the type, report and return not_locked
   *---* status.
   *---* 
   *---* XXX - should we be checking for a particular POSIX error code
   *---* when someone else holds the lock?
   *---*/
  if ((fcntl (fd, F_SETLK, &lock_desc)) == -1)
    return lock_file_no_lock;
  
  /*---* if we are here the lock is ours *---*/
  return lock_file_got_lock;
}


/*
 * Open_file
 * Simple wrapper - Potential for
 * adding number of times to try and how long to wait.
 */
int open_file (char *filename, int flags, int modebits)
{
  int fd;
  
  fd = open (filename, flags, modebits);
  if (fd == -1)
    LOG_ERROR("open_file", errorcount)
      return fd;
}

/* Close_file
 * Simple wrapper.
 */
void close_file (int fd)
{
  if (close (fd) == -1)
    LOG_ERROR("close_file", errorcount)
    }

/* Write_file
 * Loops writing pattern at offset for each block.
 */
void write_file (input_struct *input, int fd)
{
  long i;
  time_t tbuf;
  
  
  /*---* print info on start time *---*/
  (void)time(&tbuf);
  fprintf(stdout, "%s: Starting file write at %s\n", ident, ctime(&tbuf));
  
  /*
   * loop: until all the blocks have been written.
   *	 seek to the update position in the block, write the data 
   */
  for (i = 0; i < input->bnum; i++ )
    {
      if (lseek (fd, (i * input->bsize) + input->offset, SEEK_SET) == -1)
	LOG_ERROR("first lseek in write_file", errorcount)
      else
	write_pattern (input, fd, i);
    } /* end for */
  
  if (input->offset + input->patternlen < input->bsize)
    {
      /* seek out to the end of the last block so file size is bsize * bnum */
      /* lseek does not (by itself) extend filesize - must write a byte */
      if (lseek (fd, (i * input->bsize) - 1, SEEK_SET) == -1)
	LOG_ERROR("second lseek in write_file()", errorcount);
      
      if ((write (fd, "e", 1)) != 1)
	LOG_ERROR("write in write_file()", errorcount)
      else
	successfulwrites++;
    }
  
  /*---* print info on start time *---*/
  (void)time(&tbuf);
  fprintf(stdout, "%s: Finished file write at %s\n", ident, ctime(&tbuf));
  
}

/* Write_pattern
 * Contains while loop for completing interrupted writes.
 */
void write_pattern (input_struct *input, int fd, long i)
{
  pattern_struct full_pattern;
  int byteswritten, byteswrittensofar;
  unsigned long tempulong;
  
  byteswritten = byteswrittensofar = 0;
  
  full_pattern.pattern = input->pattern;
  full_pattern.bnumon = i;
  full_pattern.checksum = additup (&full_pattern, input);
  full_pattern.ident = ident;
  
  while (byteswrittensofar < input->patternlen)
    {
      byteswritten = write (fd, 
			    &input->pattern[byteswrittensofar], 
			    input->patternlen - byteswrittensofar);
      if (byteswritten == -1)
	{
	  LOG_ERROR("write in write_pattern()", errorcount)
	    return;
	}
      byteswrittensofar += byteswritten;
    }
  
  byteswrittensofar =  0;
  tempulong = htonl (full_pattern.bnumon);
  while (byteswrittensofar < LONGBYTES)
    {
      byteswritten = write (fd, &tempulong, LONGBYTES);
      if (byteswritten == -1)
	{
	  LOG_ERROR("write in write_pattern()", errorcount)
	    return;
	}
      byteswrittensofar += byteswritten;
    }
  
  byteswrittensofar =  0;
  tempulong = htonl (full_pattern.checksum);
  while (byteswrittensofar < LONGBYTES)
    {
      byteswritten = write (fd, &tempulong, LONGBYTES);
      if (byteswritten == -1)
	{
	  LOG_ERROR("write in write_pattern()", errorcount)
	    return;
	}
      byteswrittensofar += byteswritten;
    }
  successfulwrites++;
}

/* Read_file
 * Loops reading pattern at offset for each block.
 */
void read_file (input_struct *input, int fd)
{
  long i;
  char ch;
  long *array;
  long r;
  long bleft;
  time_t tbuf;
  
  /*
   * loop: until all the blocks have been read.
   *     To add some excitement, read randomly,
   *	 seek to the updated position in the block, read the data.
   *	 If successful read/comparison, update the 
   *	 read success count, otherwise print an error message
   */
  
  /* set up for random access - create array of block numbers */
  array = (long *)malloc (input->bnum * sizeof(long));
  for (i = 0; i < input->bnum; i++)
    array[i] = i;
  
  /* seed random # generator */
  SRAND (pid);
  
  /*---* print info on start time *---*/
  (void)time(&tbuf);
  fprintf(stdout, "%s: Starting file read at %s\n", ident, ctime(&tbuf));
  
  /* bleft is highest array subscript + 1 */
  bleft = input->bnum;
  for (i = 0; i < input->bnum; i++)
    {
      /* Choose an array entry number randomly */
      r = RAND () % bleft;
      
      /* Read block number contained in the array entry */
      if (lseek (fd, (array[r] * input->bsize) + input->offset, 
		 SEEK_SET) == -1)
	LOG_ERROR("first lseek in read_file()", errorcount)
      else
	read_pattern (input, fd, array[r]);
      
      /* Decrement number of blocks left to read */
      bleft--;
      
      /* Reload array entry with the last array entry's block number */
      array[r] = array[bleft];
      
    } /* end for */
  
  /* unmalloc */
  free (array);
  
  if (input->offset + input->patternlen < input->bsize)
    {
      /* seek out to the end of the last block to read last byte written */
      if (lseek (fd, (i * input->bsize) - 1, SEEK_SET) == -1)
	LOG_ERROR("second lseek in read_file()", errorcount)
	  
	  if ((read (fd, &ch, 1)) != 1)
	    LOG_ERROR("read() in read_file", errorcount)
	  else
	    successfulreads++;
      
      if (ch != 'e')
	{
	  fprintf(stderr, "failure in character compare in read_file()\n");
	}
      else
	successfulcompares++;
    }
  
  /*---* print info on start time *---*/
  (void)time(&tbuf);
  fprintf(stdout, "%s: Finished file read at %s\n", ident, ctime(&tbuf));
  
}

/*
 * Read_pattern
 */
void read_pattern (input_struct *input, int fd, long i)
{
  int bytesread, bytesreadsofar;
  char patternread[MAXPATTERNLEN];
  pattern_struct full_patternread;
  unsigned long tempulong;
  
  bytesread = bytesreadsofar = 0;
  while (bytesreadsofar < input->patternlen)
    {
      bytesread = read (fd, &patternread[bytesreadsofar], 
			input->patternlen - bytesreadsofar);
      if (bytesread == -1) 
	LOG_ERROR("read in read_pattern()", errorcount)
	  bytesreadsofar += bytesread;
    }
  
  full_patternread.pattern = patternread;
  
  bytesread = bytesreadsofar = 0;
  while (bytesreadsofar < LONGBYTES)
    {
      bytesread = read (fd, &tempulong, LONGBYTES - bytesreadsofar);
      if (bytesread == -1)
	{
	  LOG_ERROR("read in read_pattern()", errorcount)
	    return;
	}
      bytesreadsofar += bytesread;
    }
  
  full_patternread.bnumon = ntohl (tempulong);
  
  bytesread = bytesreadsofar = 0;
  while (bytesreadsofar < LONGBYTES)
    {
      bytesread = read (fd, &tempulong, LONGBYTES - bytesreadsofar);
      if (bytesread == -1)
	{
	  LOG_ERROR("read in read_pattern()", errorcount)
	    return;
	}
      bytesreadsofar += bytesread;
    }
  
  full_patternread.checksum = ntohl (tempulong);
  
  successfulreads++;
  
  if (strncmp (input->pattern, patternread, input->patternlen))
    {
      fprintf (stderr, "%s: INVALID DATA READ\n", ident);
      fprintf (stderr, "%s: Pattern read = %s, %d bytes\n", ident, 
	       patternread, strlen (patternread));
      fprintf (stderr, "%s: Pattern expected = %s, %d bytes \n", ident,
	       input->pattern, input->patternlen);
      errorcount++;
    }
  
  else
    if (i != full_patternread.bnumon)
      {
	fprintf (stderr, "%s: INVALID BLOCK NUMBER\n", ident);
	fprintf (stderr, "%s: Block number read = %d\n", ident, 
		 full_patternread.bnumon);
	fprintf (stderr, "%s: Block number expected = %d\n", ident, i);
	errorcount++;
      }
    else
      if (full_patternread.checksum != additup (&full_patternread, input))
	{
	  fprintf (stderr, "%s: INVALID CHECKSUM\n", ident);
	  fprintf (stderr, "%s: Checksum read = %d\n", ident, 
		   full_patternread.checksum);
	  errorcount++;
	}
      else
	successfulcompares++;
}

/* Initialize_variables
 * Load defaults, initialize errorcount and file descriptor.
 */
void initialize_variables (input_struct *input, char *ident)
{
  
  char pidstr[20];
  
  successfulwrites = successfulreads = successfulcompares = errorcount = 0;
  
  pid = getpid ();
  sprintf (pidstr, "%d", pid);
  gethostname (ident, MAXHOSTNAMELEN+1);
  strcat (ident, "_");
  strcat (ident, pidstr);
  strcat (ident, "_WNR");
  
  input->bnum = NUMBLOCKS;
  input->bsize = BLOCKSIZE;
  input->cont_mess=(char *)NULL;
  input->offset = OFFSET;
  input->filename = "integ.file";
  input->sleep_range = SLEEP_RANGE;
  input->read_only_flag = FALSE;
  input->verbose_flag = FALSE;
  input->delete_flag = FALSE;
  input->lock_flag = FALSE;
  input->quit_mess=QUIT_MESS;
  input->wait_flag = FALSE;
  input->pattern = ident;
  input->patternlen = strlen (ident);
}

/* Load_options
 * Translate command line switches (if any) into variables (overriding 
 * defaults). Report what paramters are being used.
 */
void load_options (int argc, char **argv, input_struct *input)
{
  int c, logfd;
  
  /* load options */
  while ((c = getopt (argc, argv, "b:c:C:df:hl:L:n:o:p:rs:T:vw:--")) != EOF)
    {
      switch (c)
	{
	case 'b':
	  input->bsize = atol (optarg);
	  break;
        case 'C': /*---* continue message for wait_for_message *---*/
          input->cont_mess=optarg;
          break;
        case 'c':
          input->cli_numb = atoi(optarg);
          break;
	case 'd':
	  input->delete_flag = TRUE;
	  break;
	case 'f': 
	  input->filename = optarg; 
	  break;
	case 'h':
	  usage ();
	  do_exit (USAGE);
	case 'l': 
          /*---* only allow EW or SR for locktype  *---*/
          if (strcmp(optarg, "EW") && strcmp(optarg, "SR"))
	    {
	      fprintf(stderr,
		      "Bad lock type %s, only \"EW\" and \"SR\" accepted.\n",
		      optarg);
	      usage();
	      do_exit(BAD_OPTIONS);
	    }
	  input->locktype = optarg; 
	  input->lock_flag = TRUE;
	  break;
	case 'L': 
	  /*
	   * Redirect stdout, stderr to a logfile.
	   * Turn off buffering so the records 
	   * from the two streams are in chronological
	   * order.
	   */
	  if (freopen(optarg, "a", stdout) == (FILE *)-1) {
	    LOG_ERROR("1: freopen", errorcount)
	      fflush(stderr);
	    do_exit(BAD_OPTIONS);
          }
	  if (freopen(optarg, "a", stderr) == (FILE *)-1) {
	    LOG_ERROR("2: freopen", errorcount)
	      fflush(stderr);
	    do_exit(BAD_OPTIONS);
          }
	  
	  setbuf(stdout, NULL);
	  setbuf(stderr, NULL);
	  break;
	case 'n':
	  input->bnum = atol (optarg);
	  break;
	case 'o':
	  input->offset = atol (optarg);
	  break;
	case 'p': 
	  input->pattern = optarg; 
	  input->patternlen = strlen (input->pattern);
	  if (input->patternlen > MAXPATTERNLEN)
	    {
	      input->patternlen = MAXPATTERNLEN;
	      input->pattern[MAXPATTERNLEN] = '0';
	      fprintf (stdout, "%s: truncating long pattern.\n");
	    }
	  break;
	case 'r':
	  input->read_only_flag = TRUE;
	  break;
	case 's':
	  input->sleep_range = atoi (optarg);
	  break;
        case 'T': /*---* Terminate message for wait_for_message *---*/
          input->quit_mess=optarg;
          break;
        case 'v': 
	  input->verbose_flag = TRUE;
	  break;
	case 'w': 
	  input->waitfilename = optarg; 
	  input->wait_flag = TRUE;
	  break;
	default: 
	  printf ("unimplemented option\n"); 
	  break;
	}
    }
  if ((input->bsize < (input->patternlen + input->offset + 2*LONGBYTES +
		       strlen (ident))) || (input->offset < 0))
    {
      fprintf (stderr, "%s: bsize not > offset + full pattern or offset not > 0.\n", 
	       ident);
      do_exit (BAD_OPTIONS);
    }
  if (input->cont_mess != (char *)NULL && !input->wait_flag)
    {
      fprintf (stderr, "%s: must specify -w waitfile with -C <message>\n",
	       ident);
      usage();
      do_exit (BAD_OPTIONS);
    }
}

/* Usage 
 * Simply cycle through usage_struct.
 */
void usage ()
{
  char **ptr;
  
  for (ptr = usage_strings; *ptr != 0; ptr++)
    fprintf (stdout, "%s", *ptr);
}

/* Report_whoiam
 * DCE-wise.
 */
void report_whoiam()
{
  sec_login_handle_t   context;
  error_status_t       st;
  int                  i;
  unsigned32           num_groups;
  signed32             *groups;
  sec_login_net_info_t   net_info;
  
  
  sec_login_get_current_context(&context, &st);
  if (st == sec_login_s_no_current_context)
    {
      fprintf (stderr, "%s: No principal logged in \n", ident);
      return;
    }
  
  sec_login_inquire_net_info (context, &net_info, &st);
  if (!st || st == sec_login_s_not_certified)
    {
      fprintf (stdout, "%s: DCE principal = %s\n", ident, 
	       net_info.pac.principal.name);
    }
  /*    
   *  Not all that interested in group info!
   */
  /*
   *  sec_login_get_groups (context, &num_groups, &groups, &st);
   *  fprintf(stdout, "%s: Num of groups = %d\n", ident, num_groups);
   *  for (i = 0; i < num_groups; i++)
   *    fprintf (stdout, "%s: Group %d = %d \n", ident, i, groups[i]);
   */
}
/* Additup
 * Calculate a checksum from the pattern and the block number
 */
unsigned long additup (pattern_struct *full_pattern, input_struct *input)
{
  unsigned long sum;
  unsigned char *ptr;
  int i;
  
  sum = 0;
  ptr = (unsigned char *)input->pattern;
  for (i = 0; i < input->patternlen; i++)
    sum += *ptr++;
  sum += full_pattern->bnumon;
  return sum;
}  

/* Report_fs
 * Report which filesystem file, pointed to by file descriptor fd, is in.
 */
void report_fs (int fd)
{
  struct statfs stats;
  
  if (fstatfs (fd, &stats))
    {
      LOG_ERROR("fstatfs in report_fs()", errorcount)
	return;
    }
  /* Currently, AIX does not have file system types defined. Comments in
   * the sys/statfs.h file indicate that f_type will always be 0.
   */
#ifndef _AIX
  else
    switch (stats.f_type)
      {
      case MOUNT_NONE: fprintf (stdout, "%s using NO filesystem?!\n", ident); break;
      case MOUNT_UFS: fprintf (stdout, "%s using UFS filesystem.\n", ident); break;
      case MOUNT_NFS: fprintf (stdout, "%s using NFS filesystem.\n", ident); break;
      case MOUNT_PC: fprintf (stdout, "%s using PC filesystem?!\n", ident); break;
      case MOUNT_MFS: fprintf (stdout, "%s using MFS filesystem?!\n", ident); break;
      case MOUNT_S5FS:fprintf (stdout, "%s using S5 filesystem.\n", ident); break;
      }
#endif
  return;
}
/* Report_params
 * Report what you asked for.
 */
void report_params (input_struct *input)
{
  fprintf (stdout, "%s: client number is %d.\n", ident, input->cli_numb);
  fprintf (stdout, "%s: using blocksize %d.\n", ident, input->bsize);
  fprintf (stdout, "%s: using file %s.\n", ident, input->filename);
  fprintf (stdout, "%s: using %d blocks.\n", ident, input->bnum);
  fprintf (stdout, "%s: Using pattern %s.\n", ident, input->pattern);
  if (input->read_only_flag)
    fprintf (stdout, "%s: Using read_only mode.\n", ident);
  else
    fprintf (stdout, "%s: Using default mode (write then read).\n", 
	     ident);
  if (input->lock_flag)
    fprintf (stdout, "%s: Requesting %s lock.\n", ident, input->locktype);
  if (input->cont_mess != (char *)NULL)
    fprintf (stdout, "%s: Continue message is %s.\n", ident, input->cont_mess);
}

/* Report_successes
 * Report running totals of successful operations
 */
void report_successes ()
{
  fprintf (stdout, "%s : %d successful writes\n", ident, 
	   successfulwrites);
  fprintf (stdout, "%s : %d successful reads\n", ident, 
	   successfulreads);
  fprintf (stdout, "%s : %d successful compares\n", ident, 
	   successfulcompares);
  fprintf (stdout, "%s : %d total error count\n", ident,
	   errorcount);
}
/*
 * Right_size_file
 */
int right_size_file (input_struct *input, int fd)
{
  struct stat fstats;
  
  if (fstat (fd, &fstats) == -1)
    {
      LOG_ERROR("fstat in right_size_file()", errorcount)
	return FALSE;
    }
  
  if (fstats.st_size != (off_t) (input->bnum * input->bsize))
    {
      fprintf(stderr, "size error in right_size_file(), %d, %d\n",
	      fstats.st_size, (input->bnum * input->bsize));
      return FALSE;
    }
  else
    return TRUE;
}

/*-----------------------------------------------------------------------*
 *---* do_exit(exit_val) - print a standard exit message to the log and  *
 *---*                     exit with exit_val.  If the exit_val is not   *
 *---*                     COMPLETE_SUCCESS, then print FAILED in the    *
 *---*                     message, else print PASSED.                   *
 *-----------------------------------------------------------------------*/
void
  do_exit(int exit_val)
{
  time_t tbuf;
  char *stat_str = (exit_val == COMPLETE_SUCCESS) ? "PASSED" : "FAILED";
  
  (void)time(&tbuf);
  
  if (exit_val != USAGE)
    {
      fprintf(stdout,
	      "Client( %d ): PID: %d System: %s ; O/S: %s lock type: %s - EXITTED / %s at %s\n",
	      input.cli_numb, pid, uname_vals.nodename, uname_vals.sysname, 
	      input.locktype, stat_str,  ctime(&tbuf));
    }
  exit(exit_val);
}


/*-------------------------------------------------------------------------*
 *---* wait_on_message(char *fn, char *lockype, char *cont_str, char *quit_str)
 *---*     
 *---*       Open the specified file, rewind it, and read it, looking for the
 *---*       specified strings.  Return when one of the strings is found with
 *---*       the appropriate status.
 *--------------------------------------------------------------------------*/
int
  wait_on_message(char *fn, char *locktype, char *cont_str, char *quit_str)
{
  int done=0,
  ret_stat;
  time_t tbuf;
  int fd, nread, ntoread;
  char  buf[256], buf2[256];
  
  /*---* 
   *---* first try to open the file.  We specify O_NONBLOCK and O_NDELAY to
   *---* avoid waiting to read and empty file while driver finds out that 
   *---* the clients are ready.
   *---*/
  if ( (fd = open_file(fn, O_RDONLY | O_NONBLOCK, 0)) == -1 )
    ret_stat = wait_on_mess_open;
  else
    {
      int clen, qlen, lookcount=0;
      
      /*---* set max number of chars to read *---*/
      ntoread= ( (clen=strlen(cont_str)) > (qlen=strlen(quit_str)) ? clen : 
		qlen);
      
      /*---* print a message *---*/
      (void)time(&tbuf);
      fprintf(stdout, "%s Looking for %s or %s at %s\n", ident, cont_str, 
	      quit_str, ctime(&tbuf));
      
      /*---* now poll the file *---*/
      while(!done)
	{  
	  /*---* if we got the continue message ... *---*/
	  if ((nread = read(fd, buf, ntoread)) == clen && !strncmp(buf,cont_str,clen))
	    {
	      *(buf + nread) = '\0';
	      /*---* print a message *---*/
	      (void)time(&tbuf);
	      
	      fprintf(stdout, "%s Found %s at %s\n", ident, buf,ctime(&tbuf));
	      
	      /*---* set the return status and exit the loop *---*/
	      ret_stat = wait_on_mess_continue;
	      done = 1;
	    }
	  
	  /*---* else if we got the quit message *---*/
	  else if (nread == qlen && !strncmp(buf, quit_str, qlen)) 
	    {
	      *(buf + nread) = '\0';
	      /*---* set the return status and exit the loop *---*/
	      (void)time(&tbuf);
	      fprintf(stdout, "%s Found %s at %s\n", ident, buf,ctime(&tbuf));
	      ret_stat = wait_on_mess_exit;
	      done = 1;
	    }
	  /*---* else seek to the top of the file and try again *---*/
	  else 
	    {
	      if (lookcount >= WAIT_ON_MSG_ATTEMPTS)
		{
		  (void)time(&tbuf);
		  fprintf(stdout, 
                          "%s: made %d attempts in wait_for_message at %s",
			  ident, lookcount, ctime(&tbuf));
		  fprintf(stdout, "%s: read returned %d\n", ident, nread);
		  if (nread > 0 )
		    {
		      *(buf + nread) = '\0';
		      fprintf(stdout, "%s: read %s\n",ident, buf);
		    }
		  /*---*
		   *---* tell the system to try printing the file to
		   *---* see if we are missing something
		   *---*/
		  fprintf(stdout, "%s: Printing %s\n", ident,fn);
		  sprintf(buf2, "cat %s", fn);
		  system(buf2);
		  fprintf(stdout, "%s: Done printing %s\n", ident,fn);
		  ret_stat = wait_on_mess_exit;
		  break;
		}
	      
	      /*---* if we got a weird number of bytes... *---*/
	      if (nread < 0)
		{
		  /*---* print a message, and continue *---*/
		  LOG_ERROR("read in wait_on_message()", errorcount)
		  }
	      
	      /*---* did not find a recognizable message, so try again *---*/
	      if (lseek(fd, (off_t)0, SEEK_SET) == (off_t) -1 )
		{
		  LOG_ERROR("lseek in wait_on_message()", errorcount)
		    do_exit(SYNCH_ERROR);
		}
	      /* 
	       * Sleep before trying again to prevent token 
	       * starvation for the dfs.lock script trying to 
	       * write to this file.
	       */
	      sleep(WAIT_ON_MSG_SLEEP);
	      lookcount++;
	    } /*---* end of else if (not cont_mess or quit_mess)... *---*/
	} /*---* end of while *---*/
      /*---* close the file *---*/
      close_file(fd);
      
    }  /*---* end of open file *---*/
  
  /*---* report the status *---*/
  return(ret_stat);
}                  


