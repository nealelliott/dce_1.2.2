/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: cma_exec.c,v $
 * Revision 1.1.2.3  1996/10/03  15:16:54  arvind
 * 	Add OSF copyright
 * 	[1996/08/02  13:13 UTC  bissen  /main/DCE_1.2.2/2]
 *
 * Revision 1.1.2.2  1996/08/09  21:26:24  arvind
 * 	Remove 10.x'isms from exec wrappers
 * 	[1996/08/01  19:53 UTC  bissen  /main/DCE_1.2.2/bissen_fix_19701/1]
 * 
 * Revision 1.1.2.1  1996/06/05  14:19:53  arvind
 * 	Merge in exec wrapper changes for CHFts15229 and purify changes.
 * 	[1995/06/05  21:56 UTC  jss  /main/HPDCE02/5]
 * 
 * 	CHFts15229. exec wrapper changes to support adding a cma_execve entry to
 * 	tepv.
 * 	[1995/05/11  17:31 UTC  jss  /main/HPDCE02/jss_mothra_fix4/1]
 * 
 * 	Merge from branch jss_mothra_fix2.
 * 	[1995/02/16  15:43 UTC  jss  /main/HPDCE02/4]
 * 
 * 	Added routine _cma_atexec_hp() to improve DG RPC performance.
 * 	[1995/02/08  21:19 UTC  jss  /main/HPDCE02/jss_mothra_fix2/1]
 * 
 * 	Merge from branch jss_single_thread.
 * 	[1994/12/22  20:01 UTC  jss  /main/HPDCE02/3]
 * 
 * 	Changes for single threaded client.  Conditionalize wrapper usage based on whether
 * 	CMA is initialized.
 * 	[1994/12/22  19:24 UTC  jss  /main/HPDCE02/jss_single_thread/1]
 * 
 * 	Merge from branch jss_fork_mothra to HPDCE02.
 * 	[1994/12/09  16:19 UTC  jss  /main/HPDCE02/2]
 * 
 * 	Fix exec wrapper by removing unnecessary routine cma__restore_nonblocking().
 * 	[1994/11/28  16:25 UTC  jss  /main/HPDCE02/jss_fork_mothra/2]
 * 
 * 	Modify cma___post_exec_cleanup() to unset kernel because something
 * 	called in the exec routine inside of libc unlocks the kernel before
 * 	we have a chance to. No need to bugcheck here since it only happens
 * 	when exec returns with errors.
 * 	[1994/11/15  21:59 UTC  jss  /main/HPDCE02/jss_fork_mothra/1]
 * 
 * 	CHFts11248
 * 	[1994/04/12  22:19 UTC  jss  /main/HPDCE01/2]
 * 
 * 	Fix for CHFts11248. Modified exec wrapper so that it saves state before
 * 	an exec() and restores state properly if the exec() returns with an
 * 	error.
 * 	[1994/04/12  22:02 UTC  jss  /main/HPDCE01/jss_rodan_0323/1]
 * 
 * 	Support for 10.0 libc_r exec calls.  Modified the cma_execl,
 * 	cma_execlp, cma_execv, and cma_execve functions to match the
 * 	new behavior of exec calls in the 10.0 libc_r.  All changes
 * 	if'd for __hpux1000p.
 * 	[1993/10/22  15:08:19  truitt]
 * 
 * 	*
 * 
 * 	Initial King Kong branch
 * 	[1993/07/08  17:28:30  root]
 * 
 * 	Add Seiichi's fix for CN RPC:  don't
 * 	reset blocking mode on files during
 * 	exec() if close on exec flag set.
 * 	[1993/06/18  18:29:13  hopkins]
 * 
 * 	Initial GAMERA branch
 * 	[1993/03/26  13:02:05  mgm  1.1.3.2]
 * 
 * 	Original version.  Modified
 * 	from JB's wrappers.
 * 	[1992/12/21  23:28:10  hopkins]
 * 
 * $EndLog$
 */

#include <cma.h>

#if !_CMA_THREAD_SYNC_IO_
#include <cma_thdio_defs.h>
#include <fcntl.h>
#include <sys/stat.h>
#endif

#ifndef __STDC__
#include <varargs.h>
#else
#include <stdarg.h>
#endif

#include <stdlib.h>
#include <string.h>			/* for memcpy		*/
#include <errno.h>			/* for errno		*/
#include <sys/types.h>			/* for size_t		*/
#include <sys/signal.h>
#include <sys/time.h>

static char **cma__exec_argv();		   /* argv memory management */
static int cma__exec_args();

static int cma__cancel_timer();		   /* Pre-exec CMA cleanup */
static int cma__reset_signals();
static int cma__reset_blocking_io();

static int cma__restore_timer();	   /* Post-exec CMA cleanup */
static int cma__restore_signals();

static struct itimerval    unix_oldtime;   /* state of process before fork */
static struct sigaction    old_act[NSIG];

typedef struct CMA___T_EXEC_RTNS {
    void (*pre_exec)();                    /* Pre-exec routine */
    void (*post_exec)();                   /* Post-exec routine */
    } cma___t_exec_rtns;

/* exec routines from atexec() */
static cma___t_exec_rtns cma___g_exec_rtns = {cma_c_null_ptr, cma_c_null_ptr};


/*
 * CMA doesn't wrap exec - and the standard implementation of exec will
 * inherit the virtual timer interrupt settings from the parent.  We clone
 * the cma wrapper for fork here to turn off the nasty inherited behavior.
 */

#if !_CMA_THREAD_SYNC_IO_
#define cma___pre_exec_cleanup() {	\
        if (cma___g_exec_rtns.pre_exec) \
           (*cma___g_exec_rtns.pre_exec)();  \
        if (cma__is_multithreaded()) {  \
	   cma__enter_kernel();		\
	   cma__cancel_timer();		\
	   cma__reset_signals();	\
	   cma__reset_blocking_io();    \
           }                    	\
	}
#else
#define cma___pre_exec_cleanup() {	\
        if (cma___g_exec_rtns.pre_exec) \
           (*cma___g_exec_rtns.pre_exec)();  \
        if (cma__is_multithreaded()) {  \
	   cma__enter_kernel();		\
	   cma__cancel_timer();		\
	   cma__reset_signals();	\
           }                    	\
	}
#endif

/* This macro is used to clean up after a failed exec call.  It will restore 
 * the state of the process before the call to cma___pre_exec_cleanup()
 */

#define cma___post_exec_cleanup() {	\
        if (cma___g_exec_rtns.post_exec) \
           (*cma___g_exec_rtns.post_exec)();  \
        if (cma__is_multithreaded()) {  \
	   cma__restore_signals();	\
	   cma__restore_timer();	\
	   cma__unset_kernel();		\
           }                    	\
	}

int cma_execl
#ifndef __STDC__
    (argp, va_alist)
    char    *argp;
    va_dcl
#else
  (
   const char   *file,
   ...
  )
#endif
{
    char		**argv;		/* argv for `program'	*/
    va_list		args;		/* varargs args		*/
    int			error;		/* error status code	*/

    /* set up the arg list...  */
#ifdef __STDC__
    va_start(args, file);
#else
    va_start(args);
#endif

    /* get the rest of the args into argv...  */
    error = cma__exec_args(&argv, NULL, args);

    /* clean up the arg list...  */
    va_end(args);

    if (error == 0)
            /* do the exec...  */
            error = cma_execv(file, (const char **)argv);
    return(error);
}

int cma_execlp
#ifndef __STDC__
    (file, va_alist)
    char    *file;
    va_dcl
#else
  (
   const char    *file,
   ...
  )
#endif
{
    char		**argv;		/* argv for `program'	*/
    va_list		args;		/* varargs args		*/
    int			error;		/* error status code	*/

    /* set up the arg list...  */
#ifdef __STDC__
    va_start(args, file);
#else
    va_start(args);
#endif

    /* get the rest of the args into argv...  */
    error = cma__exec_args(&argv, NULL, args);

    /* clean up the arg list...  */
    va_end(args);

    if (error == 0)
            /* do the exec...  */
            error = cma_execvp(file, (const char **)argv);
    return(error);
}

int cma_execle
#ifndef __STDC__
    (file, va_alist)
    char    *file;
    va_dcl
#else
  (
   const char    *file,
   ...
   )
#endif
{
    char		**argv;		/* argv for `program'	*/
    char		**envp;		/* envp for `program'   */	
    va_list		args;		/* varargs args		*/
    int			error;		/* error status code	*/

    /* set up the arg list...  */
#ifdef __STDC__
    va_start(args, file);
#else
    va_start(args);
#endif

    /* get the rest of the args into argv...  */
    error = cma__exec_args(&argv, &envp, args);

    /* clean up the arg list...  */
    va_end(args);

    if (error == 0)
            /* do the exec...  */
            error = cma_execve(file, (const char **)argv, (const char **)envp);

    return(error);
}

int cma_execv
#ifndef __STDC__
    (argp, argv)
    char    *argp;
    char    *argv[];
#else
  (
   const char    *argp,
   const char    *argv[]
  )
#endif
{
   int status;

    cma___pre_exec_cleanup();
    status = execv(argp, argv);
    cma___post_exec_cleanup(); 
    return status;
}

int cma_execvp
#ifndef __STDC__
    (argp, argv)
    char    *argp;
    char    *argv[];
#else
  (
   const char    *argp,
   const char    *argv[]
  )
#endif
{
   int status;

    cma___pre_exec_cleanup();
    status = execvp(argp, argv);
    cma___post_exec_cleanup(); 
    return status;
}

int cma_execve
#ifndef __STDC__
    (argp, argv, envp)
    char    *argp;
    char    *argv[];
    char    *envp[];
#else
  (
   const char    *argp,
   const char    *argv[],
   const char    *envp[]
  )
#endif
{
   int status;

   cma___pre_exec_cleanup(); 
   status = execve(argp, argv, envp);
   cma___post_exec_cleanup(); 
   return status;
}

/*
 * Helper Routines
 */

/*
 * NAME:	_exec_args
 *
 * FUNCTION:	_exec_args - convert variable argument list and convert o
 *		argv[] format...
 *
 * NOTES:	exec_args takes a variable length argument list (like
 *		execl() is given) and converts it to a NULL terminated
 *		array of pointers.  If the second argument 'envp' is
 *		not NULL, it is assumed the environment comes immediately
 *		after the args.
 *		
 *
 * RETURN VALUE DESCRIPTION:	-1 on memory allocation error, else 0
 */

static int
cma__exec_args(argvp, envp, args)
char ***argvp;				/* pointer to destination argv	*/
char ***envp;				/* pointer to destination env	*/
va_list args;				/* variable list of arguments	*/
{
	int argc;			/* arguments processed so far	*/
	int alloced;			/* allocated pointers so far	*/
	char **argp;			/* use to 'walk thru' *argvp	*/

	argc = alloced = 0;

	do {
		if (argc >= alloced) {		/* used up our space?	*/
			if ((*argvp = cma__exec_argv(&alloced)) == NULL)
				return (-1);
			argp = *argvp + argc;
		}
		argc++;

	/* copy in the pointer */
	} while ((*argp++ = va_arg(args, char *)) != NULL);

	if (envp != NULL)		/* get enviroment...  */
		*envp = va_arg(args, char **);
	return (0);
}

/*
 * NAME:	_exec_argv
 *
 * FUNCTION:	_exec_argv - expand and return a pointer suitable for
 *		use as an argv.
 *
 * NOTES:	exec_argv expands and returns a pointer suitable for
 *		use as an array of character pointers (ala argv).  The
 *		argument 'argvct' points to a counter which indicates
 *		how much memory we've previously allocated.  The counter
 *		should be set to 0 for the first call to exec_argv.
 *		exec_argv uses the value to determine whether to use
 *		static memory, malloc some memory, or realloc some previously
 *		allocated memory.  exec_argv will update that variable to
 *		always be the total number of pointers allocated.  exec_argv
 *		should be called again when that number of pointers is
 *		exhausted.
 *
 *		Since there is a chance that exec's could fail, the normal
 *		loop of filling in argv's could start over.  In that case,
 *		the program should again set the variable 'argvct' points
 *		to to 0 when it starts the loop again.
 *
 * RETURN VALUE DESCRIPTION:	NULL if a memory allocation error
 *		occurred, or if there were too many arguments (assuming
 *		a maximum limit is set), else a pointer to the new
 *		memory.
 */

/*
 *	ARGV_SIZ is used 2 ways:
 *
 *	1)	our initial static array of pointers is this size
 *	2)	each time we expand, we expand by this size.
 *
 *	This should be as small as possible, but big enuf to handle most
 *	'normal' exec calls.  If this is too small, we spend lots of time
 *	malloc'ing and realloc'ing memory.  If this is too big, we end up
 *	wasting memory.
 */

#ifndef ARGV_SIZ
# define	ARGV_SIZ	64
#endif

static char **
cma__exec_argv(argvct)
int *argvct;
{
	size_t newsize;			/* new size to allocate		*/
	char **newargv;			/* new pointer			*/
	static int max_alloced = 0;	/* the max we've ever allocated	*/
	static char **oldargv = NULL;	/* last memory we allocated	*/
	static char *initial[ARGV_SIZ]; /* initial array for speed	*/

	/*
	 * if our allocated count is higher than our expansion target, just
	 * use the old memory and the allocated count... (this might occur
	 * if the loop is starting over and we've already got some space
	 * allocated...)
	 */
	if (max_alloced >= *argvct + ARGV_SIZ) {
		newargv = oldargv;
		*argvct = max_alloced;
		}

	else if (*argvct == 0) {	/* first time in this loop...	*/
		/* start with our static memory */
		newargv = initial;
		*argvct = ARGV_SIZ;
		}

	else {			/* second, etc.: need to malloc/realloc	*/
		if (max_alloced == 0) {
			/*
			 * first time in malloc loop - need to start with
			 * 2 * ARGV_SIZ (to take into account our initial
			 * static array plus expansion)
			 */
			max_alloced = 2 * ARGV_SIZ;
			newsize = (size_t) (max_alloced * sizeof(*newargv));
			if ((newargv = (char **) malloc(newsize)) != NULL)
				/*
				 * copy the static array into our new array
				 */
				(void) memcpy((void *) newargv,
				(void *) oldargv,
				(size_t) ((*argvct) * sizeof(*newargv)));
			}
		else {
			/*
			 * expand by ARGV_SIZ and realloc our old mem...
			 */
			max_alloced += ARGV_SIZ;
			newsize = (size_t) (max_alloced * sizeof(*newargv));
			newargv = (char **) realloc((void *) oldargv, newsize);
			}

		/* set errno to ENOMEM if our malloc/realloc failed */
		if (newargv == NULL)
			errno = ENOMEM;
		else
			*argvct = max_alloced;
		}

	return (oldargv = newargv);
}

/*
 * Pre-exec CMA cleanup routines
 */

static int
cma__cancel_timer(
#ifdef __STDC__
                  void
#endif
                  )        
{
    struct itimerval    unix_timer;

    timerclear(&unix_timer.it_interval);
    timerclear(&unix_timer.it_value);

    return (setitimer(ITIMER_VIRTUAL, &unix_timer, &unix_oldtime));
}

static int
cma__restore_timer(
#ifdef __STDC__
                  void
#endif
                  )        
{
    return (setitimer(ITIMER_VIRTUAL, &unix_oldtime, (struct itimerval *)0));
}

static int
cma__reset_signals(
#ifdef __STDC__
                  void
#endif
                   )
        
{
    struct sigaction    act;
    int                 i, error;

    /*
     * Set all signal actions to the default
     */
    if ((error = sigemptyset (&act.sa_mask)) == -1)
        return(error);

    act.sa_flags = 0;
    act.sa_handler = SIG_DFL;

    for (i = 1; i < NSIG; i++) {

        switch (i) {
            case SIGKILL :
            case SIGSTOP : {    /* Can't be caught */
                break;
            }
            default : {
                (void)sigaction (i, &act, &old_act[i]);
                break;
            }
        }
    }
    return(error);
}

static int
cma__restore_signals(
#ifdef __STDC__
                  void
#endif
                   )
        
{
    int                 i, error;

    /*
     * Set all signal actions to their original value
     */

    for (i = 1; i < NSIG; i++) {

        switch (i) {
            case SIGKILL :
            case SIGSTOP : {    /* Can't be caught */
                break;
            }
            default : {
                (void)sigaction (i, &old_act[i], (struct sigaction *)0);
                break;
            }
        }
    }
    return(error);
}

#if !_CMA_THREAD_SYNC_IO_
static int
cma__reset_blocking_io()
{
    register int i, flag;

    for(i = 0; i < cma__g_file_num; i++) {
	if(cma__is_open(i)) {
	    /*
	     * A close in progress could have closed
	     * closed the file but not yet deallocated
	     * the file object.
	     */
	     /*
	      * Don't reset blocking mode if close-on-exec
	      * flag is set ... fixes a problem with CN RPC.
	      * [CHFts07109].
	      *
	      * (But no matter what we do here, it will be
	      * wrong in some cases.)
	      */
	    if( (flag = fcntl( i, F_GETFD )) != -1 && !( flag & FD_CLOEXEC ) )
	        cma__set_blocking(i);
	    }
	}
    }
#endif	/* !_CMA_THREAD_SYNC_IO_ */

/*
 *  FUNCTIONAL DESCRIPTION:
 *
 *	Register caller's exec handler routines.
 *
 *  FORMAL PARAMETERS:
 *
 *	pre_exec - the routine to be called before performing the exec.
 *      post_exec - the routine to be called after performing the exec.
 *
 *  IMPLICIT INPUTS:
 *
 *      none
 *
 *  IMPLICIT OUTPUTS:
 *
 *	none
 *
 *  FUNCTION VALUE:
 *
 *	none
 *
 *  SIDE EFFECTS:
 *
 *	none
 */
extern void
_cma_atexec_hp (
    void (*pre_exec)(),
    void (*post_exec)() )
    {
	cma___g_exec_rtns.pre_exec = pre_exec;
	cma___g_exec_rtns.post_exec = post_exec;
    }

