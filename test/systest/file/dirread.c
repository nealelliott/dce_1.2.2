/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dirread.c,v $
 * Revision 1.1.11.2  1996/02/17  23:26:59  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:44:35  marty]
 *
 * Revision 1.1.11.1  1995/12/11  22:00:04  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/jrr_1.2_mothra/1  1995/11/14  21:01 UTC  dat
 * 	Remove malloc prototype
 * 	define EXIT_FAILURE only if not previously defined
 * 	[1995/12/11  20:59:14  root]
 * 
 * Revision 1.1.9.1  1994/02/23  21:43:41  pellis
 * 	Changed std.h to dce.h
 * 	[1994/02/23  21:38:08  pellis]
 * 
 * Revision 1.1.7.1  1993/09/16  20:18:27  damon
 * 	Synched with ODE 2.1 based build
 * 	[1993/09/16  20:18:09  damon]
 * 
 * 	OT 8592: Include <dce/std.h> for its TRUE, FALSE definitions - needed
 * 	for HP-UX.
 * 	[1993/09/15  19:10:46  mort]
 * 
 * Revision 1.1.2.6  1993/03/09  21:29:03  gmd
 * 	Fix CR 6660 - remove possibility of unlinking a directory by
 * 	using lstat() (non-standard but necessary to avoid the opposite problem,
 * 	rmdir'ing a symbolic link!) and only unlinking non-directory files.
 * 	[1993/03/09  21:21:26  gmd]
 * 
 * 	Fix CR 6660 - Be more careful when "unlink"ing ie. stat first to
 * 	avoid the possibility of unlinking a directory and leaving incorrect
 * 	link counts around.
 * 	[1993/03/03  20:43:05  gmd]
 * 
 * Revision 1.1.2.5  1993/02/12  21:38:51  gmd
 * 	Corrected comments.
 * 	[1993/02/11  16:52:38  gmd]
 * 
 * Revision 1.1.2.4  1993/02/05  15:20:31  cjd
 * 	Embedded copyright notice
 * 	[1993/02/05  14:40:46  cjd]
 * 
 * Revision 1.1.4.2  1993/02/04  22:07:34  cjd
 * 	Embedded copyright notice
 * 
 * Revision 1.1.2.3  1993/01/07  19:03:01  gmd
 * 	Exit if fopen of inputfile fails.
 * 	[1993/01/05  22:16:56  gmd]
 * 
 * Revision 1.1.2.2  1993/01/03  23:31:42  gmd
 * 	Original version.
 * 	[1993/01/03  23:29:13  gmd]
 * 
 * $EndLog$
 */
/***************************************************************************
 * TITLE: dirread.c
 * SYNTAX:
 *	see usage_strings.
 * OBJECTIVE:
 *	To verify the contents of a directory.
 * DESCRIPTION:
 * Accepts an input file of directory entries (1 per line) and the
 * name of a test directory in which to find these entries. Processing
 * is simply to verify, using the readdir() call, that all the entries
 * exist in the test directory and that no other entries exist in the
 * test directory.
 *
 * PROCESSING: 
 *             read inputfile into an array
 *             readdir test directory, marking array entries as found
 *               if there's an entry not in the array -> error
 *               if there's an array entry not in the directory -> error
 *               if an entry is found in the directory more than once -> error
 *             if deleting test directory
 *               readdir test directory, marking array entries as deleted
 *		   stat entry
 *                 rmdir dir entry
 *                 unlink non-dir entry
 *
 * OUTPUT: 1 of exit values defined below
 *         if -v
 *           logging of parameters used and successes/failures
 *           along the way 
 *           (stdout for info and success, stderr for failures)
 **************************************************************************/

#include <stdio.h>
#include <errno.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <dce/sec_login.h>
#include <dce/dce.h>

/* exit values */
#define EXIT_SUCCESS 0
#ifndef EXIT_FAILURE
#define EXIT_FAILURE 1
#endif
#define EXIT_USAGE 2
#define EXIT_CHDIRP 3
#define EXIT_CHDIRT 4
#define EXIT_INPUTFILE 5
#define EXIT_CLOSE 6
#define EXIT_MALLOC 7
#define EXIT_OPENDIR 8
#define EXIT_CLOSET 9
#define EXIT_RMDIRT 10
#define EXIT_STAT 11

#define CHDIR_ERROR	fprintf (stderr, "%s: chdir failed\n", ident); \
  			perror (ident); errorcount++;

#define OPENDIR_ERROR	fprintf (stderr, "%s: opendir failed\n", ident); \
  			perror (ident); errorcount++;

#define READDIR_ERROR	fprintf (stderr, "%s: readdir failed\n", ident); \
  			perror (ident); errorcount++;

#define RMDIR_ERROR	fprintf (stderr, "%s: rmdir failed\n", ident); \
  			perror (ident); errorcount++;

#define DUP_ERROR	fprintf (stderr, "%s: found a file twice\n", ident); \
  			perror (ident); errorcount++;

#define MISSING_ERROR	fprintf (stderr, "%s: did not find a file\n", ident); \
  			perror (ident); errorcount++;

#define MISMATCH_ERROR	fprintf (stderr, "%s: file in dir, not in listing\n", ident); \
  			perror (ident); errorcount++;

#define OVERFLOW_ERROR	fprintf (stderr, "%s: input file contents > -n arg\n", ident); \
  			perror (ident); errorcount++;

#define UNLINK_ERROR	fprintf (stderr, "%s: unlink failed\n", ident); \
  			perror (ident); errorcount++;

#define STAT_ERROR	fprintf (stderr, "%s: stat failed\n", ident); \
  			perror (ident); errorcount++;

#define INDESTRUCTABLE_ERROR	fprintf (stderr, "%s: deleted file still there\n", ident); \
  			perror (ident); errorcount++;

#define FOPEN_ERROR	fprintf (stderr, "%s: fopen failed\n", ident); \
  			perror (ident); errorcount++;

#define CLOSE_ERROR	fprintf (stderr, "%s: close failed\n", ident); \
  			perror (ident); errorcount++;

#define MALLOC_ERROR	fprintf (stderr, "%s: malloc failed\n", ident); \
  			perror (ident); errorcount++;


/* some constants and defaults */
#define MAXPIDLEN 20
#define NUMENTRIES 5000
#define MAXENTRYNAMELEN 1024
#define MAXHOSTLEN 64
#define MAXIDENTLEN MAXHOSTLEN+1+MAXPIDLEN+8+1

/* entry status */
#define LISTED 1
#define FOUND 2
#define DELETED 3

/* local typedefs */
typedef  struct input {
  char *parentdir;
  char *testdir;
  char *inputfile;
  int numentries;
  int verbose_flag; /* flag to turn on debug/verbose mode */
  int delete_flag; /* flag to turn on delete/unlink code */
} input_struct;

typedef struct entry {
  char *entry_name;
  int status_flag;
} entry_struct;

/* External */
extern pid_t getpid();
extern void perror();
extern char *optarg;
extern int errno;
extern FILE *fopen();

/* Internal */
void initialize_variables();
void load_options();
void report_whoiam();
void usage();
void dir_loop();
int read_inputfile();

/* Global Variables */
int errorcount; /* running total of errors */
char ident[MAXIDENTLEN]; /* default pattern to write and/or read */

char* usage_strings[] = {
  "\n",
  "    -i inputfile (to read in numentries filenames from)\n",
  "    -p parentdir (to find testdir in)\n",
  "    -n numentries (number of dir entries should find)\n",
  "    -t testdir (contains numentries)\n",
  "    -d (delete_flag = > delete entries and test directory)\n",
  "    -v (verbose_flag = > some printfs to stdout)\n",
  (char *)0
};

/* directory integrity test */
main (argc, argv)
  int argc;
  char **argv;
{
  input_struct input;
  entry_struct *array;
  long arraysize;

  initialize_variables (&input, ident);
  load_options (argc, argv, &input);

  /* create and initialize array for tracking */
  arraysize = input.numentries * sizeof (entry_struct);
  array = (entry_struct *)malloc (arraysize);
  if (!array)
    {
      MALLOC_ERROR;
      fprintf (stdout, "%s: FAILED\n", ident);
      exit (EXIT_MALLOC);
    }
  bzero (array, arraysize);	
  
  /* change to parentdir */
  if (chdir (input.parentdir))
    {
      CHDIR_ERROR;
    }
  
  /* change to testdir */
  if (chdir (input.testdir))
    {
      CHDIR_ERROR;
    }

  if (read_inputfile (&input, array))
    {
      fprintf (stdout, "%s: FAILED\n", ident);    
      exit (EXIT_INPUTFILE);
    }

  /* enumerate dir and ensure they're all there */
  if (input.verbose_flag)
      fprintf (stdout, "%s: Verifying %d files\n", ident, input.numentries);

  dir_loop (array, input.numentries, LISTED, FOUND);

  if (input.delete_flag)
    {
      /* delete all from dir */
      if (input.verbose_flag)
	fprintf (stdout,"%s: Deleting %d files.\n", ident, input.numentries);
      
      dir_loop (array, input.numentries, FOUND, DELETED);
      
      /* recheck that dir is now empty */
      if (input.verbose_flag)
	fprintf (stdout, "%s: Verifying %d files are gone.\n", ident, input.numentries);
      
      dir_loop (array, input.numentries, DELETED, 0);
      
      /* cd back up and remove the dir */
      if (chdir (".."))
	{
	  CHDIR_ERROR;
	}
      
      if (rmdir (input.testdir))
	{
	  RMDIR_ERROR;
	}
    }
  if (errorcount)
    {
      fprintf (stdout, "%s: FAILED\n", ident);
      exit (EXIT_FAILURE);
    }
  else
    {
      fprintf (stdout, "%s: PASSED\n", ident);
      exit (EXIT_SUCCESS);
    }
}

/* Load_options
 * Translate command line switches (if any) into variables (overriding 
 * defaults). Report what parameters are being used.
 */
void load_options (int argc, char **argv, input_struct *input)
{
  int c;
  
  /* load options */
  while ((c = getopt (argc, argv, "dhi:n:p:t:v--")) != EOF)
    {
      switch (c)
	{
	case 'n':
	  input->numentries = atoi (optarg);
	  break;
	case 'd':
	  input->delete_flag = TRUE;
	  break;
	case 'v':
	  input->verbose_flag = TRUE;
	  break;
	case 'h':
	  usage ();
	  exit (EXIT_USAGE);
	case 'i': 
	  input->inputfile = optarg; 
	  break;
	case 'p': 
	  input->parentdir = optarg; 
	  break;
	case 't':
	  input->testdir = optarg;
	  break;
	default: 
	  fprintf (stderr, "unimplemented option\n"); 
	  break;
	}
    }

  if (input->verbose_flag)
    {
      fprintf (stdout, "%s: using parentdir %s\n", ident, input->parentdir);
      fprintf (stdout, "%s: using testdir %s\n", ident, input->testdir);
      fprintf (stdout, "%s: using inputfile %s\n", ident, input->inputfile);
      fprintf (stdout, "%s: looking for %d entries\n", ident, input->numentries);
      report_whoiam ();
    }
}

/* Usage 
 * Simply cycle through usage_struct.
 */
void usage()
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
 *  fprintf (stdout, "%s: Num of groups = %d\n", ident, num_groups);
 *  for (i = 0; i < num_groups; i++)
 *    fprintf (stdout, "%s: Group %d = %d \n", ident, i, groups[i]);
 */
}
/* Initialize_variables
 * Load defaults, initialize errorcount and file descriptor.
 */
void initialize_variables (input_struct *input, char *ident)
{
  pid_t pid;
  char pidstr[MAXPIDLEN];
  
  errorcount = 0;

  pid = getpid ();
  sprintf (pidstr, "%d", pid);
  gethostname (ident, MAXHOSTLEN+1);
  strcat (ident, "_");
  strcat (ident, pidstr);
  strcat (ident, "_dirread");

  input->parentdir = ".";
  input->testdir = "testdir";
  input->numentries = NUMENTRIES;
  input->inputfile = "CONTENTS";
  input->verbose_flag = FALSE;
  input->delete_flag = FALSE;

}

int read_inputfile (input_struct *input, entry_struct *array)
{
  char temp[MAXENTRYNAMELEN];
  int templen;
  FILE *input_filep;
  int i;

  /* open input file and read in a filename per line */
  input_filep = fopen (input->inputfile, "r");
  if (!input_filep)
    {
      FOPEN_ERROR;
      exit (EXIT_INPUTFILE);
    }

  i = 0;

  while ((fgets (temp, MAXENTRYNAMELEN, input_filep)) && 
	  (i < input->numentries))
    {
      templen = strlen (temp);
      temp [templen - 1] = '\0'; /* get rid of annoying newline char */
      array[i].entry_name = malloc (templen);
      if (!array[i].entry_name)
	{
	  MALLOC_ERROR;
	  fclose (input_filep);
	  exit (EXIT_MALLOC);
	}
      strcpy (array[i].entry_name, temp);
      array[i].status_flag = LISTED;
      i++;
    }

  if (i > input->numentries)
    {
      OVERFLOW_ERROR;
      fclose (input_filep);
      return -1;
    }

  fclose (input_filep);
  return 0;
}

void dir_loop (entry_struct *array, int numentries, int status_in, int status_out)
{
  DIR *tdirp;
  struct dirent *tdep;
  struct stat stat_buf;
  int found;
  int found_count;
  int i;

  tdirp = opendir (".");
  if (!tdirp)
    {
      OPENDIR_ERROR;
      exit (EXIT_OPENDIR);
    }

  found_count = 0;
  while (tdep = readdir (tdirp))
    {
      /* if either of the "special" entries, just move on */
      if (!strcmp(tdep->d_name, ".") || !strcmp(tdep->d_name, ".."))
	continue;
      
      /* do a linear search through the array for this dir entry */
      i = 0; found = FALSE;
      while (i < numentries && !found)
	{
	  if (strncmp (tdep->d_name, array[i].entry_name, tdep->d_namlen))
	    i++;
	  else
	    /* found it */
	    {
	      found = TRUE;
	      found_count++;
	      if (status_in == DELETED)
		/* we shouldn't have! */
		{
		  INDESTRUCTABLE_ERROR;
		  fprintf (stderr, "%s: Entry name %s\n", 
			   ident, array[i].entry_name);
		}
	      /* if asked to delete - do so */
	      if (status_out == DELETED)
		{
		  if (lstat (tdep->d_name, &stat_buf))
		      {
			STAT_ERROR;
			fprintf (stderr, "%s: Entry name %s\n", 
				 ident, array[i].entry_name);
			exit (EXIT_STAT); 
		      }

		  /* if it's really a dir, not a symlink to a dir, rmdir it. */
		  if (!(S_ISLNK(stat_buf.st_mode)) &&
		      (S_ISDIR(stat_buf.st_mode)))
		    {
		      if (rmdir (tdep->d_name))
			  {
			    RMDIR_ERROR;
			    fprintf (stderr, "%s: Entry name %s\n", 
				     ident, array[i].entry_name);
			  } /* end rmdir failed */
		    } /* end directory case */
		  else
		    /* whatever it is, unlink it */
		    if (unlink (tdep->d_name))
			  {
			    UNLINK_ERROR;
			    fprintf (stderr, "%s: Entry name %s\n", 
				     ident, array[i].entry_name);
			  }
		} /* end delete case */
	      
	      /* mark your action in the array */
	      if (array[i].status_flag == status_in)
		array[i].status_flag = status_out;
	      else
		{
		  DUP_ERROR;
		  fprintf (stderr, "%s: Entry name %s\n", array[i].entry_name);
		}
	    } /* end of finding it */
	} /* end while going through array */

      if (!found)
	{
	  MISMATCH_ERROR;
	}
    } /* end while going through directory */

  /* we're done looping through directory so close up shop */
  if (closedir (tdirp))
    {
      CLOSE_ERROR;
      exit (EXIT_CLOSET);
    }

  /* if we'd never deleted - did we find everything? */
  if ((status_in != DELETED) && (found_count != numentries))
    {
      fprintf (stderr, "%s :  found %d, was looking for %d\n", 
	       ident, found_count, numentries);
      errorcount++;
    }
  /* if we had deleted - did we find stuff we shouldn't have?! */
  if ((status_in == DELETED) && found_count)
    {
      fprintf (stderr, "%s : failed to delete %d entries\n", 
	       ident, found_count);
      errorcount++;
    }

  /* are there entries in the array unaccounted for this time around? */
/* never debugged this
 *  for (i=0; i < numentries; i++)
 *  {
 *    if (array[i].status_flag != status_out)n
 *	{
 *	  MISSING_ERROR;
 *        fprintf (stderr, "%s: Entry name %s\n", 
 *	           ident, array[i].entry_name);
 *      }
 *  }
*/
}







