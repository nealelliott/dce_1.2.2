/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cma_core.c,v $
 * Revision 1.1.4.3  1996/02/17  23:33:03  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:49:21  marty]
 *
 * Revision 1.1.4.2  1995/12/07  21:52:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  21:20:35  root]
 * 
 * $EndLog$
 */

#include <sys/types.h>
#include <sys/adspace.h>
#include <sys/shm.h>
#include <sys/user.h>
#include <sys/pseg.h>
#include <sys/file.h>
#include <stdio.h>
#include <sys/ldr.h>
#include <sys/core.h>
#include <sys/id.h>
#include <sys/errids.h>
#include <sys/resource.h>
#include <sys/acct.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <procinfo.h>

	/* dce includes */
#include <cma.h>
#include <cma_defs.h>
#include <cma_tcb.h>
#include <cma_deb_core.h>

extern struct proc *curproc;
extern cma__t_known_object       cma__g_known_threads;

extern uid_t	getuidx(int);
extern gid_t	getgidx(int);
static int	write_core();
static int	get_u_block();
static void	write_tty();
static void	identify_signal();

struct errstruct {
	struct err_rec0	hdr;
	int signo;
	pid_t pid;
	int fs_sn;
	int inode;
} cderr = { ERRID_CORE_DUMP, "SYSPROC", 0, 0, -1, -1 };
static struct user u;

#define HUNG_LOOP 1024
#define DEFAULT_STACK_COPY 65536
#define SENSIBLE 1024000

/*
 * NAME: cma__core()
 *
 * FUNCTION: This function places the necessary information from a running
 *	process into the file "core" to allow a debugger to reconstruct the
 *	current state of that process.  This process is usually terminated
 *	upon completion of this call.  The minimum usable dump with header
 *	is written first, so it is usable even if the dump is not completed.
 *
 * DATA STRUCTURES: The user area block, user stack, loaded file information
 *	and optional user data are read and copied to the core file.
 *	No process data is modified by this procedure.
 *
 */
void
cma__core
#ifdef	_CMA_PROTO_
	(int signo,
	int code,
	struct sigcontext *sigctx)
#else	/* prototype */
	(signo, code, sigctx)
	int signo;
	int code;
	struct sigcontext *sigctx;
#endif	/* no prototype */
{
	struct core_dump *chdr;		/* core dump header pointer */
	struct ld_info	*ldorgp, *ldp;	/* pointer to loader info */
	char		*datap;		/* pointer to data area */
	int 		cfd;		/* core file read/write descriptor */
	off_t		offset = CHDRSIZE; /* offset of core tables for I/O */
	caddr_t		stack_top;      /* top of stack, where stack begins */
	int		count;		/* loader entry counter */
	int		length;		/* length of data to write */
	int		status = 0;	/* write file status, 0 is good */
	int		cflag;		/* estimated correct core flag value */
	int		maxfile;	/* maximum core file size */
	int		stacksiz;	/* stack size */
	int		wrcount = 0;	/* returned count from write() */
	struct stat	core_stat;	/* statistics of core file */
	int	 	rc;
	pid_t		my_pid;
	int	 	dsize, t0stacksiz;
	int 		smax;
	uint 		sreg;
	struct sigcontext	*tcontext0;
	cma__t_queue	*ptr;
	cma__t_int_tcb	*thread;
	caddr_t		stack_cur, *p;
	char		obuf[80];


#if 0
	/* block signals while we dump core */
	sigblock(-1);
#endif

	/* in userland we must get a copy of the u-block for this process
	 * rather than address it directly as in the kernel.  Hence we
	 * call a routine to get our u-block.
	 */
	my_pid = getpid();
	if (get_u_block(&u,my_pid) == -1 ) {
		sprintf(obuf,"Failed to get u-block can't write core\n");
		write_tty(obuf);
		exit(0x80 | signo);
	}

	smax = u.u_smax;
	dsize = u.u_dsize;

	maxfile = (u.u_sigflags[signo] & SA_NODUMP) ? 0 : CCORE_SIZE;

	identify_signal(signo);

	/* check permissions and minimum size */
	if ((getuidx(ID_SAVED) != getuidx(ID_REAL)) ||
	    (getgidx(ID_SAVED) != getgidx(ID_REAL)) ||
	    (CHDRSIZE > maxfile)){
		sprintf(obuf,"permissions don't match or maxfile is too small\n");
		write_tty(obuf);
		exit(0x80 | signo);
	}

	/* For CMA threads the thread stacks live in the process data
	 * area and the initial thread has it's main stack in the
	 * user stack area and it's signal stack is in the process
	 * data area.  We need to find the top of the initial
	 * thread stack that is in the per process stack area and tell
	 * ourself to dump that.
	 */

	/*
		HERE IS THE PICTURE FROM thread->static_ctx.sp
		when the initial thread was preempted.


			 ---------------------------------------
			|					|
			|  	sigcontext structure from	|
			|	kernel setup for signal		|
			|					|
			|		608 bytes long		|
			|					|
			|---------------------------------------|
			|					|
			|		STKMIN bytes		|
			| 	(defined in sys/pseg.h)		|
			|					|
			|------sp to cma__periodic--------------|
			|					|
			|					|
			|					|
			|---sp to cma__yield_processor----------|
			|					|
			|					|
			|					|
			|--------sp to cma__dispatch------------|
			|					|
			|  	sigcontext structure from	|
			|		cma__transfer_ctx	|
			|					|
			|		608 bytes long		|
			|					|
   			 -----thread->static_ctx.sp-------------
	*/

	/* find the initial thread */
	ptr = cma__queue_next(&cma__g_known_threads.queue);
	rc = 0;
	while ((ptr != &cma__g_known_threads.queue) && (rc < HUNG_LOOP)) {
		rc++;
		thread	= cma__base (ptr, threads, cma__t_int_tcb);
		if (thread->kind == cma__c_thkind_initial)
			break;
		ptr = cma__queue_next (ptr);
	}
	/* If the thread was preempted then the thread context
	 * is saved in a sigcontext structure on the top
	 * of the stack.  The current sp is stored in static_ctx.sp and
	 * hence is a pointer to a sigcontext structure.  The stack pointer
	 * value given must be adjusted by the size of the sigcontext
	 * structure for a RS6000
	 */

	if ((thread->async_ctx.valid) && (cma__g_current_thread != thread)) {
		stack_top = ( caddr_t ) thread->static_ctx.sp;
		stack_top += sizeof(struct sigcontext);

		/* Now trace back up the stack until it stops, at this point we
		 * should be in the cma__periodic routine which is where the
		 * VTALRM interrupt transferred us to.  This code will stop only
		 * if the top of the stack that the thread is running from
		 * was 0.  Since we are running off the signal stack, this
		 * can be ensured by zeroing it before it is used initially.
		 * Currently it is a static variable in cma_timer.c which
		 * ensures that it will be set to zero because it will be in
		 * the data section of libcma.a.
		 */
		stack_cur = stack_top;
		p = (caddr_t *) stack_top;
		rc = 0;


		while (rc < HUNG_LOOP) {
			/* We have to be careful when tracing back up
			 * the stack because we might have gotten here
			 * due to a trounced stack and thus you never
			 * know what you might find in the way of back
			 * chain values.
			 */
			rc++;
			sreg = (uint)p >> SEGSHIFT;
			if (sreg != (uint) STACKSEG)
				rc = HUNG_LOOP;
			if (((int) (*p) == 0))
				break;
			stack_cur = ( caddr_t ) p;
			p = (caddr_t *) *p;
		}
		if (rc != HUNG_LOOP) {
			stack_cur = ( caddr_t ) p;
			/* Now we are at the first stack frame,the kernel
			 * places the sigcontext structure STKMIN above
			 * this stack pointer so we get it and then
			 * set stack_top using it.
	 		 */
			stack_cur +=  STKMIN ;
			tcontext0 = (struct sigcontext *) stack_cur;
			stack_top = (caddr_t) tcontext0->sc_jmpbuf.jmp_context.gpr[1];
		}

	} else if (cma__g_current_thread != thread) {
		/* The thread was synchronously switched and so the
		 * signal stack isn't being used and the sigcontext
		 * data stored by cma__transfer_ctx is on the main
		 * thread stack.
		 */
		stack_cur = ( caddr_t ) thread->static_ctx.sp;
		tcontext0 = (struct sigcontext *) stack_cur;
		stack_top = (caddr_t) tcontext0->sc_jmpbuf.jmp_context.gpr[1];
	} else {
		/* If the initial thread is the current thread then the
		 * sigcontext structure we entered this routine with will
		 * contain the stack pointer to use.
		 */
		stack_top = (caddr_t) sigctx->sc_jmpbuf.jmp_context.gpr[1];
	}

	/* The stack might have been damaged in the main thread, in
	 * which case the best we can do is just copy some default
	 * amount of the main thread stack.
	 */
	stacksiz = (int) ((caddr_t) USRSTACK - stack_top);
	sreg = (uint)stack_top >> SEGSHIFT;
	if ( rc == HUNG_LOOP  || stacksiz > SENSIBLE || sreg != STACKSEG )
		stack_top = (caddr_t) ( USRSTACK - DEFAULT_STACK_COPY );
	stacksiz = (int) ((caddr_t) USRSTACK - stack_top);

	/* open core file */
	if ((cfd = open(COREFILE, O_TRUNC|O_CREAT|O_RDWR, 0666 )) == -1) {
        	sprintf (obuf, "Failed on opening core file errno=%d\n", errno);
        	write_tty (obuf);
        	exit (0x80 | signo);
	}

	/* set the core flag based on file size limits, default to truncate */
	cflag = CORE_TRUNC | UBLOCK_VALID;
	if ((CHDRSIZE + CTABSIZE) <= maxfile)
		cflag |= LE_VALID;
	if ( ((CHDRSIZE + CTABSIZE + stacksiz) <= maxfile) &&
	     (stacksiz > 0) && (stacksiz <= smax) )
		cflag |= USTACK_VALID;
	cflag |= (u.u_sigflags[signo] & SA_FULLDUMP) ? FULL_CORE : 0;

	/* set core header to the minimum dump possible, write */
	chdr = (struct core_dump *) calloc((uint) CHDRSIZE, (uint) 2);
	if(chdr == NULL)
	{
		sprintf (obuf, "couldn't calloc memory, exiting\n");
		write_tty (obuf);
		(void)close (cfd);
		exit (0x80 | signo);
	}
	chdr->c_signo = signo;
	chdr->c_flag = cflag;
	chdr->c_entries = (cflag & LE_VALID) ? 1 : 0;
	chdr->c_tab = (cflag & LE_VALID) ? (void *)CHDRSIZE : NULL;
	chdr->c_stack = NULL;
	chdr->c_size = (cflag & USTACK_VALID) ? stacksiz : 0;
	chdr->c_u = u;
	chdr->c_u.u_save = sigctx->sc_jmpbuf.jmp_context;	/* sig save */

	status |= write_core(cfd, (caddr_t)chdr, CHDRSIZE, 0);

	if (cflag & LE_VALID)	/* walk through loaded files, write info */
	{
		length = CTABSIZE * 10;	/* starting guess size */
		ldorgp = (struct ld_info *)calloc( (uint)length, (uint) 2);
		if (ldorgp == NULL) {
			(void)close (cfd);
			sprintf (obuf, "couldn't calloc memory, exiting\n");
			write_tty (obuf);
			exit (0x80 | signo);
		}
		while (loadquery (L_GETINFO, ldorgp, (uint) length) == -1) {
			if ( errno == ENOMEM) {
				/* out with the old */
				free((void *)ldorgp);
				length *= 2;
				ldorgp=(struct ld_info *)calloc((uint)length,(uint)2);
				if (ldorgp == NULL) {	/* abort */
					status = -1;
					break;
				}
			} else {
				sprintf (obuf, "loadquery failed, errno is %d\n",errno);
				write_tty (obuf);
				perror ("loadquery");
				exit (0x80 | signo);
			}
		}

		for (ldp = ldorgp, count = 1; ldp->ldinfo_next; count++)
			ldp = (struct ld_info *) ((char *)ldp+ldp->ldinfo_next);
		length = MIN((char *)ldp - (char *)ldorgp + CTABSIZE, length);
		/* starting address for loader tables */
		offset = (off_t) chdr->c_tab;
		if (((int)(offset) + length + chdr->c_size) > maxfile)	/* too big? */
			length = CTABSIZE;
		else
			chdr->c_entries = count;

		status |= write_core(cfd, (caddr_t)ldorgp, length, offset);

		offset += (off_t) length;
	}

	if (cflag & USTACK_VALID)
	{
		chdr->c_stack = (void *)offset;
		length = chdr->c_size;

		status |= write_core(cfd, (caddr_t)stack_top, length, offset);

		offset += ( off_t )length;
	}

	/* For CMA threads the stacks for the threads will be included in
	 * the data area because threads does an sbrk to extend the data
	 * area and make room for the thread stacks.
	 */
	if (cflag & FULL_CORE)
	{
		int len2;

		if (dsize > SEGSIZE) {	/* big data? */
			length = u.u_sdsize;
			len2 = dsize - SEGSIZE;
		}
		else {	/* no */
			length = dsize;
			len2 = 0;
		}

		/* check if data area fits */
		if ( ((int) (offset)) + dsize <= maxfile)
		{
			status |= write_core (cfd, PRIVORG, length, offset);
			if (len2 > 0) {
				len2 += PAGESIZE;
				status |= write_core (cfd, BDATAORG, len2, offset + (off_t)SEGSIZE);
			}
		}
		else
			cflag &= ~FULL_CORE;
	}

	if ( !status )
		cflag &= ~CORE_TRUNC;
	chdr->c_flag = cflag;

	status |= write_core (cfd, (caddr_t)chdr, CHDRSIZE, 0);
	free((void *)chdr);
	(void)close(cfd);

	if (status != 0)
		exit(1);
	exit(0x80 | signo);
}


/*
 *	get_u_block - get a copy of this process's u-block
 *
 *	u - a pointer to the user structure in which to store the result.
 *	pid - the pid of the process for which to fetch the u-block
 *
 *	Return: 0 on success, -1 on error
 */
static int
get_u_block
#ifdef	_CMA_PROTO_
	(struct user	*u,
	pid_t		pid)
#else	/* prototype */
	(u, pid)
	struct user *u;
	pid_t	pid;
#endif	/* no prototype */
{
	int		pn, numprocs, rc;
	struct procinfo	aproc, *proc;
	struct userinfo	uinfo;

	rc = getproc( &aproc, 1, sizeof(struct procinfo));
	numprocs = aproc.pi_pid ;
	proc = (struct procinfo *) malloc(sizeof(struct procinfo)*numprocs);

	if ((rc = getproc(proc,numprocs,sizeof(struct procinfo))) == -1) {
		free(proc);
		return(-1);
	}

	for (pn=0; pn<numprocs; pn++) {
		if (proc[pn].pi_pid == pid) {
			rc = getuser (&proc[pn], sizeof(aproc), u,
				sizeof(struct user));
			if ( rc != 0 ){
				free(proc);
				return (-1);
			}
			break;
		}
		if (pn == numprocs) {
			free (proc);
			return (-1);
		}
	}
	free(proc);
	return(0);
}


void identify_signal
#ifdef	_CMA_PROTO_
	(int signo)
#else	/* prototype */
	(signo)
	int	signo;
#endif	/* no prototype */
{
	char	*signame;
	char	obuf[256];
	switch (signo) {
		case SIGSEGV:
			signame = "SIGSEGV";
			break;
		case SIGFPE:
			signame = "SIGFPE";
			break;
		case SIGIOT:
			signame = "SIGIOT";
			break;
		case SIGBUS:
			signame = "SIGBUS";
			break;
		case SIGEMT:
			signame = "SIGBUS";
			break;
		case SIGILL:
			signame = "SIGILL";
			break;
		default:
			signame = NULL;
			break;
	}
	if (signame == NULL)
		sprintf(obuf,"Dumping core after receiving signal #%d\n",signo);
	else
		sprintf(obuf,"Dumping core after receiving signal %s\n",signame);
	write_tty(obuf);
}


/*
 *	write_tty - writes a null terminated string to the current tty device
 *
 *	input - string a null terminated string
 *
 *	This function is required because the abort() library call closes all of
 *	the open file descriptors before sending the SIGABRT signal.  Thus if we
 *	got here due to the abort() call then there will be no stdout to printf
 *	to.
 */
void write_tty
#ifdef	_CMA_PROTO_
	(char *string)
#else	/* prototype */
	(string)
	char	*string;
#endif	/* no prototype */
{
	int fd,cnt;
	char *p;

	fd = open("/dev/tty",O_RDWR);
	if ( fd == -1 )
		return;
	if ( u.u_ttysid == 0 ){
		close(fd);
		if ( (fd = open("/dev/console",O_RDWR)) == -1 )
			return;
	}
	for ( cnt=0 , p = string; *p ; cnt++, p++);
	write(fd,string,cnt);
	close(fd);
}

/*
* write_core - write to a core file
*
* Arguments:	fd - file descriptor for the core file
*		addr - address of the buffer
*		count - number of bytes to write
*		offset - offset into the file
*
* Return value:	0 implies success, !0 implies failure.
*/

static int
write_core
#ifdef	_CMA_PROTO_
	(int	fd,
	caddr_t		addr,
	int		count,
	off_t		offset)
#else	/* prototype */
	(fd, addr, count, offset)
	int	fd;
	caddr_t		addr;
	int		count;
	off_t		offset;
#endif	/* no prototype */
{
	int rc, wcount;

	(void) lseek(fd, offset, SEEK_SET);
	rc = write(fd, addr, count);
	return rc == -1 || rc != count ;
}
