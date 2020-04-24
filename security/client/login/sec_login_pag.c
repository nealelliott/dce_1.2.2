/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 *
 * @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: sec_login_pag.c,v $
 * Revision 1.1.23.1  1996/10/03  14:45:11  arvind
 * 	Variables live across setjmp must be volatile.
 * 	[1996/09/16  20:06 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.21.2  1996/03/11  13:27:55  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:43  marty]
 * 
 * Revision 1.1.21.1  1995/12/08  18:05:06  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/3  1995/10/13  17:01 UTC  sommerfeld
 * 	fix sec_login_pag warning to ignore *expected* errors from newtgt()
 * 	[1995/10/10  19:59 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/1]
 * 
 * 	HP revision /main/HPDCE02/2  1995/07/11  21:16 UTC  sommerfeld
 * 	add stubbed resetpag() if we're not building against DFS..
 * 	[1995/05/02  19:43 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts13948/1]
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  21:22 UTC  mullan_s
 * 	Merge.
 * 
 * 	HP revision /main/HPDCE01/2  1994/05/16  19:57 UTC  rose_l
 * 	merge sec_login PAG inheritance changes into Rodan(mainline)
 * 
 * 	HP revision /main/HPDCE01/rodan_initial/1  1994/05/16  19:22 UTC  rose_l
 * 	move inherit PAG changes into sec_login_pvt_new_pag().
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/26  19:51  mdf
 * 	merge
 * 	[1995/12/08  16:31:32  root]
 * 
 * Revision 1.1.12.3  1993/09/21  20:02:37  robinson
 * 	DCE 1.0.3 merge up.
 * 	[1993/09/21  20:01:35  robinson]
 * 	Revision 1.1.17.1  1994/07/15  14:58:42  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:14:51  mdf]
 * 
 * Revision 1.1.12.3  1993/09/21  20:02:37  robinson
 * 	DCE 1.0.3 merge up.
 * 	[1993/09/21  20:01:35  robinson]
 * 
 * Revision 1.1.13.3  1993/09/15  15:37:06  root
 * 	    Loading drop DCE1_0_3b03
 * 	Revision 1.1.12.2  1993/07/08  11:04:04  root
 * 	Initial King Kong branch
 * 	[1993/07/08  11:03:20  root]
 * 
 * Revision 1.1.15.2  1993/07/19  19:38:25  zeliff
 * 	HP port of DFS
 * 	[1993/07/19  18:29:44  zeliff]
 * 
 * Revision 1.1.13.3  1993/07/16  21:15:51  kissel
 * 	Change sense of NO_LATENT_DFS_SUPPORT to LATENT_DFS_SUPPORT
 * 	[1993/06/21  14:47:02  kissel]
 * 
 * Revision 1.1.11.2  1993/06/04  20:16:23  robinson
 * 	For latent support of DFS, must *never* get the stubs included
 * 	for setpag() et. al. since they will come out of file/sys/lclcalls.c
 * 	instead.
 * 	[1993/06/04  20:05:50  robinson]
 * 
 * Revision 1.1.8.7  1993/03/16  22:45:12  sommerfeld
 * 	[7144] Inform DFS when login context ready.
 * 	[1993/03/16  22:32:53  sommerfeld]
 * 
 * Revision 1.1.8.6  1992/12/29  13:01:48  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:39:49  zeliff]
 * 
 * Revision 1.1.8.5  1992/12/08  15:49:32  burati
 * 	CR6111 Detect errno better on link() failure to avoid infinite loop
 * 	[1992/12/08  00:20:25  burati]
 * 
 * Revision 1.1.8.4  1992/11/19  21:46:57  rsarbo
 * 	Move OSF1 setpag to file/sys/lclcalls.c where it belongs
 * 	[1992/11/19  21:46:02  rsarbo]
 * 
 * Revision 1.1.8.3  1992/10/19  14:22:55  rsarbo
 * 	correct ifdefs around setpag()
 * 	[1992/10/19  14:22:09  rsarbo]
 * 
 * Revision 1.1.7.2  1992/10/09  18:36:45  rsarbo
 * 	define stubbed setpag() for builds with NO_DFS.
 * 
 * Revision 1.1.3.6  1992/07/13  21:18:13  sekhar
 * 	[CR4682] Code drop Mike's fix.
 * 	[1992/07/13  21:14:53  sekhar]
 * 
 * Revision 1.1.5.3  1992/07/10  20:33:54  burati
 * 	CR4682 Fix name construction from real pag (more).
 * 
 * Revision 1.1.5.2  1992/07/10  14:54:38  burati
 * 	CR4682 Fix name construction from real pag.
 * 
 * Revision 1.1.3.5  1992/07/08  01:16:18  burati
 * 	CR4535 Fix simulated pag generation code.
 * 	[1992/07/06  22:41:48  burati]
 * 
 * Revision 1.1.3.4  1992/04/13  21:04:09  treff
 * 	4/13/92 DCT @ OSF
 * 	OOPS.  Don't forget to initialize return code . . .
 * 	[1992/04/13  21:03:34  treff]
 * 
 * Revision 1.1.3.3  1992/04/13  20:27:53  treff
 * 	4/13/92 DCT @ OSF
 * 	Replace signal()  with TRY and CATCH.
 * 	This officially turns off the HACK ALERT.
 * 	[1992/04/13  20:27:20  treff]
 * 
 * Revision 1.1.3.2  1992/04/09  22:14:04  treff
 * 	*** empty log message ***
 * 	[1992/04/09  22:05:46  treff]
 * 
 * 	4/9/92 DCT @ OSF
 * 	OSF/1 version of libdce gets it's own setpag,
 * 	imported from file/sys/lclcalls.c.  Decides at
 * 	runtime using syscall if DFS is loaded into the
 * 	kernel. *****HACK ALERT**** due to an ugly cma
 * 	bug, we have to call signal (unwrapped) instead
 * 	of sig_action(wrapped) to trap SIGSYS, which
 * 	of course happens when we call for a system call
 * 	which doesn't exist.  The correct code is
 * 	commented out, and should be included when the
 * 	bug is fixed . . .
 * 	[1992/04/09  21:47:31  treff]
 * 
 * Revision 1.1  1992/01/19  14:46:57  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */

/*  sec_login_pag.c V=8 12/05/91 //littl/prgy/src/client/sec_login
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*   
 *   User space PAG generation code
 */

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)sec_login_pag.c	8 - 12/05/91";
#endif

#include <sys/types.h>
#include <time.h>
#include <sec_login_pvt.h>
#include <stdlib.h>
#include <rgymacro.h>
#include <pthread.h>
#include <dce/exc_handling.h>

static  char *          kernel_debugging = NULL;
static  unsigned32      pag_val = 0;
static  boolean32       sec_login_pag__inited = false;
static  pthread_mutex_t sec_login_pag__mutex;

/* primitive semaphore operations */
#define LOCK(mutex) pthread_mutex_lock((&mutex))

#define UNLOCK(mutex)  pthread_mutex_unlock((&mutex))


/* semaphore with exception handling - useful only if the semaphore is acquired
 * and freed within the same lexical scope (the VSEM macro takes no parameter to 
 * emphasize this contraint).  Can be nested within other exception
 * handling scopes, but may not "straddle" any other such scope.
 */
#define LOCKSEM(mutex) { \
            pthread_mutex_t  this_mutex; \
            this_mutex = mutex; \
            LOCK(this_mutex); \
            TRY {
		
#define UNLOCKSEM \
	    } CATCH_ALL { \
		UNLOCK(this_mutex); \
                RERAISE; \
	    } \
            ENDTRY \
            UNLOCK(this_mutex); \
	} 


#if defined(hpux) || defined(__hpux)

#if !defined(DCE_DFS_PRESENT) && !defined(LATENT_DFS_SUPPORT)
/*
 * setpag() stub for builds with NO_DFS
 * The real setpag() is defined in file/sys/lclcalls.c
 */
int setpag PROTOTYPE((void))
{
	return -1;
}
 
/*
 * newtgt() stub for builds with NO_DFS.
 * 
 * Let kernel know about new tgt in ccache.
 * The real newtgt() is defined in file/sys/???.c
 */

int newtgt ( pag, exp )
    unsigned32 pag;
    unsigned32 exp;
{
    return 0; /* Success: nobody to tell. */
}

int resetpag PROTOTYPE((void))
{
    return 0;			/* ditto. */
}

#endif /* !DCE_DFS_PRESENT && !LATENT_DFS_SUPPORT */ 

#else /* hpux || __hpux */

#if !defined(DCE_DFS_PRESENT) && !defined(LATENT_DFS_SUPPORT)
/*
 * setpag() stub for builds with NO_DFS
 * The real setpag() is defined in file/sys/lclcalls.c
 */
int setpag PROTOTYPE((void))
{
	return -1;
}
 
/*
 * newtgt() stub for builds with NO_DFS.
 * 
 * Let kernel know about new tgt in ccache.
 * The real newtgt() is defined in file/sys/???.c
 */

int newtgt ( pag, exp )
    unsigned32 pag;
    unsigned32 exp;
{
    return 0; /* Success: nobody to tell. */
}
#endif /* !DCE_DFS_PRESENT && !LATENT_DFS_SUPPORT */ 


#endif /* hpux || __hpux */

PRIVATE void sec_login__init_pag_info
#ifndef __STDC__
    ( )  
#else
    ( void )
#endif
{
    kernel_debugging = getenv("DCE_SEC_KERNEL_DEBUGGING");
    if (kernel_debugging == NULL) {
        pthread_mutex_init(&sec_login_pag__mutex, pthread_mutexattr_default);
        pag_val = getpid();
        pag_val = (pag_val << 16) + ((time(0) & 0xFF) <<8);
    }
    sec_login_pag__inited = true;
}

/*
 * Put this process in a "new" process authentication group, and
 * return the numeric value of that PAG.  If the pag is obtainable
 * via syscall, then just pass it back with a status of 0.  Otherwise,
 * simulate a PAG with the pid and a nonce, and pass back 1 so that
 * the caller will know that this pag is simulated and may not be
 * unique.  If no PAG is obtainable, pass back -1.
 */

int sec_login_pvt_new_pag(lcp, pag)
    sec_login_context_t  *lcp;
    unsigned32 *pag;
{
    unsigned32 tmppag;
    char       *envstr;
    int        retval;
 
    if (FLAG_SET(lcp->flags, sec_login_inherit_pag)) {

        /*
         * get the PAG from the environment. Return the PAG
         * value if it's set, otherwise error.
         */
        if ((envstr = getenv(SEC_LOGIN_PAG_ENV)) == NULL) {
            retval = -1;
        } else {
            /*
             * unset the PAG Environment variable
             */
            unsetenv(SEC_LOGIN_PAG_ENV);

            if (!(tmppag = strtoul(envstr, (char **)NULL, 16))) {
                retval = -1;
            } else {
                *pag = tmppag;
                retval = 0;
            }
        }
        return retval;
    } else {

        /*
         * Issue the proper afs_syscall...
         */

        tmppag = setpag();
        if(tmppag == -1) {
            retval = -1;
        } else {
            *pag = tmppag;
            retval = 0;
        }
        return retval;
    }
}


PUBLIC void sec_login_pvt_link_ccache 
#ifndef __STDC__
    (pag, gen_pag, pagenv, prev_cfn, stp)
    volatile unsigned32      pag;
    boolean32       gen_pag;
    char            *pagenv;
    char            *prev_cfn;
    error_status_t  *stp;
#else
  (
    volatile unsigned32      pag,
    boolean32       gen_pag,
    char            *pagenv,
    char            *prev_cfn,
    error_status_t  *stp
  )
#endif
{
    volatile boolean32   done = false;
    char        new_cfn[SEC_LOGIN_PAG_NAME_LENGTH];
    static pthread_once_t   pag_pthread_init_block = pthread_once_init;

    if (gen_pag && !sec_login_pag__inited) {
        pthread_once(&pag_pthread_init_block, sec_login__init_pag_info);
    }

    if (kernel_debugging) {
        uid_t uid = getuid();
        if (uid < 0) {
            *stp = errno;
            return;
        } else {
            pag = uid;
            gen_pag = false;
        }
    }

    if (! gen_pag) {
        sprintf(new_cfn, SEC_LOGIN_CCACHE_FILE_FROM_PAG_FMT, pag);
        if (link(prev_cfn, new_cfn) == (-1)) {
            *stp = errno;
        }
    } else {
        while (! done) {
            LOCKSEM(sec_login_pag__mutex) {
                pag = pag_val++;
            } UNLOCKSEM;

            sprintf(new_cfn, SEC_LOGIN_CCACHE_FILE_FROM_PAG_FMT, pag);
            if (link(prev_cfn, new_cfn) != (-1)) {
                done = true;
            } else {
                switch(errno) {
                case EEXIST:
                    break;
                case ENOENT:
                    /* If name to link from is gone then context is invalid */
                    SET_STATUS(stp, sec_login_s_context_invalid);
                    done = true;
                    break;
                default:
                    SET_STATUS(stp, KRB5_CC_IO);
                    done = true;
                    break;
                }
            }
        }
        sprintf(pagenv, SEC_LOGIN_CCACHE_FROM_PAG_FMT, pag);
    }
}
 
/*
 * inform kernel that current tgt will expire at time `expiration'.
  */
void sec_login_pag_new_tgt (pag, expiration)
    unsigned32 pag;
    unsigned32 expiration;
{
    int code = newtgt (pag, expiration);
     
    if (code == ENOSYS)
        code = 0;
 
#define NEWTGT_DEBUG
#ifdef NEWTGT_DEBUG
    if ((code != 0) && (code != EACCES) && (code != EINVAL))
    {
	fprintf(stderr, "(sec_login_pvt_new_tgt) warning: newtgt(%d, %d) returned %d\n",
 	    pag, expiration, code);
	fprintf(stderr, "(sec_login_pvt_new_tgt) warning: may indicate AFS/DFS system call conflict\n");
    }
#endif
}
