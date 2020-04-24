/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dnsuser.c,v $
 * Revision 1.1.6.2  1996/02/18  19:34:50  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:14:55  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:26:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:04  root]
 * 
 * Revision 1.1.4.3  1994/06/30  19:17:42  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:03:21  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:42:21  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:31  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:08:05  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:33:25  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:16:29  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:15:48  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:18:56  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module: dnsuser.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 * Module to map uid/gid to user name.
 *
 */

#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <uc_clerk.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <dns_malloc.h>
#include <dce/dns_record.h>
#include <threads_lib.h>

typedef struct uname {
    struct uname *un_link;		/* link to next user name */
    int		un_uid;			/* user's uid */
    SimpleName_t un_name;		/* the mapped SimpleName, */
    char	un_spare;		/*  NULL terminated */
} uname_t;

static dthread_once_t uname_once = dthread_once_init;
static dthread_mutex_t uname_mutex;
static uname_t *list_p;	/* linked list of cached names */

/*
 * Get the user name string.
 *
 * Note that this is a "special" SimpleName, in that
 * the name must be terminated by a null for a routine
 * in clerk_send_recv.c to work.
 *
 * In order to avoid the getpwuid() overhead,
 * we cache the resultant names by uid for later reuse.
 */

static void 
uname_init (void)
{
    int th_status;
    

    if ((th_status = dthread_create_mutex(&uname_mutex)) < 0) 
    {
	dce_svc_printf(CDS_S_PTHREADMUTEXINIT_A_MSG, (long)th_status);
    }
    
    list_p = NULL;
}

SimpleName_t 
*dns_user_name (int  uid,
                int  gid)
{
    register uname_t *un_p = NULL;

    dthread_once(&uname_once, uname_init);
    if (0 <= dthread_lock(&uname_mutex))
	{
	un_p = list_p;
	while (1==1) {
	    if (!un_p) {
		struct passwd *pwd_p = getpwuid(uid);
		if (!pwd_p) break;
		if (un_p = (uname_t *)dns_malloc_perm(sizeof(*un_p))) {
		    un_p->un_uid = uid;
		    INS8(un_p->un_name.sn_flag, SN_cds);
		    INS8(un_p->un_name.sn_length, strlen(pwd_p->pw_name));
		    strcpy((char *)un_p->un_name.sn_name, pwd_p->pw_name);
		    un_p->un_link = list_p;
		    list_p = un_p;
		}
		break;
	    }
	    if (uid == un_p->un_uid) break;
	    un_p = un_p->un_link;
	}
	dthread_unlock(&uname_mutex);
    }

    if (un_p)
	return(&un_p->un_name);
    else
	return(NULL);
}











