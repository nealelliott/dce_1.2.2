/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_login_np.c,v $
 * Revision 1.1.10.2  1996/03/11  13:27:51  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:39  marty]
 *
 * Revision 1.1.10.1  1995/12/08  18:04:18  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:31:18  root]
 * 
 * Revision 1.1.7.1  1994/06/17  18:41:33  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:14  devsrc]
 * 
 * Revision 1.1.4.2  1993/07/26  20:57:31  burati
 * 	Always convert to OS pwent, not just if not null.
 * 	[1993/07/26  20:56:12  burati]
 * 
 * 	Commented out the line that is trying to set the
 * 	pw_quota field. It does not exist in SVR4.
 * 
 * Revision 1.1.2.2  1993/02/01  19:27:02  hinman
 * 	[hinman] - Check in SNI revision 1.1.2.3
 * 	[1993/01/08  13:38:03  hinman]
 * 
 * 	[meg] created SVR4/sec_login_np.c from osf rev 1.1.2.3
 * 
 * Revision 1.1.2.3  92/12/18  17:31:09  tikku
 * 	Commented out the line containing pw_quota. That does not
 * 	exist in SVR4.
 * 
 * Revision 1.1.2.2  92/12/18  10:09:32  meg
 * 	[meg] created SVR4/sec_login_np.c; based on osf rev 1.1.2.3
 * 
 * Revision 1.2.1.2  92/12/09  16:25:37  root
 * 	Auto checkin of OSF rev 1.1.2.3
 * 
 * Revision 1.1.2.3  1992/12/07  21:57:07  bolinger
 * 	Fix OT defect 6367:  use externally-visible names for all members
 * 	of passwd struct.
 * 	[1992/12/07  21:56:48  bolinger]
 * 
 * Revision 1.1.2.2  1992/12/04  21:30:30  burati
 * 	CR5978 Initial version, for PMAX, from original sec_login_np.c
 * 	[1992/12/04  16:41:46  burati]
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991,1992
** Unpublished work. All Rights Reserved.
**
*/
/* 
 *     Non-portable sec_login code
 */

#if !defined(LINT)
static char *VersionID =
    "$Id: sec_login_np.c,v 1.1.10.2 1996/03/11 13:27:51 marty Exp $";
#endif

#include <pthread.h>           /* use exception-based pthread package */
#include <un_maloc.h>
#include <stdlib.h>
#include <sys/param.h>
#include <pwd.h>
#include <stdio.h>
#include <un_strng.h>
#include <dce/rgynbase.h>
#include <dce/exc_handling.h> 
#include <dce/dce_cf.h>
#include <macros.h>
#include <sec_login_pvt.h>
#include <sec_login_np.h>

#ifdef USE_GETPW_R
/*
 * On OSF, we use the getpwnam_r routine, which is already thread
 * safe, so we can dispense with most of this.
 */

#define LOCK_PWD_DATA
#define UNLOCK_PWD_DATA
#else
/* primitive semaphore operations */
#define P(mutex) pthread_mutex_lock((&mutex))

#define V(mutex)  pthread_mutex_unlock((&mutex))


/* semaphore with exception handling - useful only if the semaphore is acquired
 * and freed within the same lexical scope (the VSEM macro takes no parameter to 
 * emphasize this contraint).  Can be nested within other exception
 * handling scopes, but may not "straddle" any other such scope.
 */
#define PSEM(mutex) { \
            pthread_mutex_t  this_mutex; \
            this_mutex = mutex; \
            P(this_mutex); \
            TRY {
                
#define VSEM \
            } CATCH_ALL { \
                V(this_mutex); \
                RERAISE; \
            } \
            ENDTRY \
            V(this_mutex); \
        } 


static pthread_mutex_t  mutex;
static boolean32        mutex_inited;
static pthread_once_t   mutex_once = pthread_once_init;

#define LOCK_PWD_DATA \
    if (!mutex_inited) { \
        pthread_once(&mutex_once, init_mutex); \
     } \
    PSEM(mutex); \
    setpwent(); \


#define UNLOCK_PWD_DATA  endpwent(); VSEM; 

PRIVATE void  init_mutex
#ifndef __STDC__
    ( )  
#else
    ( void )
#endif
{
    pthread_mutex_init(&mutex, pthread_mutexattr_default);
    mutex_inited = true;
}
#endif

/*
 * non-portable getpwent routine - may need to be modified to suit specific 
 * vendor platforms. 
 */

PUBLIC  void  sec_login_pvt_get_local_login_info
#ifndef __STDC__
    (name, pw_passwd, pw_uid, pw_gid, pw_gecos, pw_dir, pw_shell, num_groups, groups, stp)
    char            *name;      
    char            pw_passwd[];
    int             *pw_uid;
    int             *pw_gid;
    char            pw_gecos[];
    char            pw_dir[];
    char            pw_shell[];
    unsigned16      *num_groups;
    signed32        **groups;
    error_status_t  *stp;
#else
  (
    char            *name,      
    char            pw_passwd[],
    int             *pw_uid,
    int             *pw_gid,
    char            pw_gecos[],
    char            pw_dir[],
    char            pw_shell[],
    unsigned16      *num_groups,
    signed32        **groups,
    error_status_t  *stp
  )
#endif
{
    struct passwd  *pwd_data;
    char           *my_cell;
    error_status_t lst;
#ifdef USE_GETPW_R
    char buf[1024];
    struct passwd pw;
#   define GETPWNAM(name) ((getpwnam_r ((name), &pw, buf, sizeof(buf))>=0)?(&pw):(0));

#else
#   define GETPWNAM(name) getpwnam((name));
#endif

    CLEAR_STATUS(stp);
    *num_groups = 0;
    *groups = NULL; 

    LOCK_PWD_DATA {
        pwd_data =  GETPWNAM(name);
        if (pwd_data == NULL) {

            if (*name == '/') {
                char  *qname = NULL;

                /* 
                 * strip cell directory prefix or global host cell name,
                 *  and try again
                 */
                if (strncmp(CELL_DIR_ROOT, name, CELL_DIR_ROOT_LEN) == 0)
                    qname = name + CELL_DIR_ROOT_LEN;
                else {
                    dce_cf_get_cell_name(&my_cell, &lst);
                    if (GOOD_STATUS(&lst)) {
                        int len;

                        len = strlen(my_cell);
                        if (strncmp(name, my_cell, len) == 0
                        && len < strlen(name)
                        && name[len] != '\0') {
                            /*  add 1 to get past '/' separator */
                            qname = name + len + 1;
                        }
                    }
                }

                if (qname != NULL) {
                    pwd_data = GETPWNAM(qname);
                }
 
            } else {
                char qname[sizeof(sec_rgy_name_t)];
                /*
                 * try qualifying the name with the Cell directory root, 
                 * and the global host cell name
                 */  
                strcpy(qname, CELL_DIR_ROOT);
                strcat(qname, name);
                pwd_data = GETPWNAM(qname);
                if (pwd_data == NULL) {
                    dce_cf_get_cell_name(&my_cell, &lst);
                    if (GOOD_STATUS(&lst)) {
                        strcpy(qname, my_cell);
                        strcat(qname, name);
                        pwd_data = GETPWNAM(qname);
                        free(my_cell);
                    }
                }
            }
        }
             
        if (pwd_data != NULL) {
            strcpy(pw_passwd, pwd_data->pw_passwd);
            *pw_uid = pwd_data->pw_uid;
            *pw_gid = pwd_data->pw_gid;
            strcpy(pw_gecos, pwd_data->pw_gecos);
            strcpy(pw_dir, pwd_data->pw_dir);
            strcpy(pw_shell, pwd_data->pw_shell);
        } else {
            SET_STATUS(stp, sec_rgy_object_not_found);
        }

    } UNLOCK_PWD_DATA;

    /* 
     * Some implementations may supply groupset information.
     * The current implementation relies on /etc/groups, so we just return
     * NULL, and expect login programs to use the initgroups() system call to 
     * establish the local concurrent groupset for a locally authenticated
     * identity
     */
}


/*
 * sec_login_np_lc_to_pwent -
 *      Return an OS dependent struct passwd from a login context
 */
PUBLIC void sec_login_np_lc_to_pwent
#ifndef __STDC__
    (lcp, pwP, stp)
    sec_login_context_t	*lcp;
    sec_login_passwd_t  *pwP;
    error_status_t      *stp;
#else
  (
    sec_login_context_t	*lcp,
    sec_login_passwd_t  *pwP,
    error_status_t      *stp
  )
#endif
{
    struct sec_passwd	*sec_pwdP = UNIX_CREDS(lcp).pwent;
    struct passwd       *pwdP;

    CLEAR_STATUS(stp);
    if (UNIX_CREDS(lcp).pwent == NULL) {
        SET_STATUS(stp, sec_login_s_info_not_avail);
	*pwP = NULL;
    } else {
        if ((UNIX_CREDS(lcp).os_pwent == NULL)) {
            UNIX_CREDS(lcp).os_pwent =
                (struct passwd *) malloc(sizeof(struct passwd));
        }
	pwdP = (struct passwd *) UNIX_CREDS(lcp).os_pwent;
	pwdP->pw_name 	  = sec_pwdP->pw_name;
	pwdP->pw_passwd   = sec_pwdP->pw_passwd;
	pwdP->pw_uid      = (uid_t) sec_pwdP->pw_uid;
	pwdP->pw_gid      = (gid_t) sec_pwdP->pw_gid;
	/*	pwdP->pw_quota    = 0;	*/
	pwdP->pw_comment  = NULL;
	pwdP->pw_gecos    = sec_pwdP->pw_gecos;
	pwdP->pw_dir      = sec_pwdP->pw_dir;
	pwdP->pw_shell    = sec_pwdP->pw_shell;
    }
    if (GOOD_STATUS(stp)) {
	*pwP = UNIX_CREDS(lcp).os_pwent;
    }
}
