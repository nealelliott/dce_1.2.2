/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: unix_lib.c,v $
 * Revision 1.1.8.2  1996/02/18  19:37:17  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:04  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:34:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:39  root]
 * 
 * Revision 1.1.6.5  1994/08/03  19:05:10  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:33:13  mccann]
 * 
 * Revision 1.1.6.4  1994/06/09  18:44:20  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:17:00  devsrc]
 * 
 * Revision 1.1.6.3  1994/04/29  15:56:19  peckham
 * 	Use dcelocal_path in file names.
 * 	[1994/04/29  14:37:48  peckham]
 * 
 * Revision 1.1.6.2  1994/04/14  14:51:39  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:22:58  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:13:57  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:48:52  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  14:25:26  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:26:17  zeliff]
 * 
 * Revision 1.1.2.2  1992/06/30  19:27:42  peckham
 * 	OT#3745: Replace sys_errno() with deb_ascii_errno()
 * 	[1992/06/30  19:27:16  peckham]
 * 
 * Revision 1.1  1992/01/19  15:25:42  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * unix_lib.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
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
 * MODULE DESCRIPTION:
 *
 * Networks & Communications Software Engineering
 *
 */

#include <server.h>

#include <unistd.h>
#include <dce/assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <time.h>

#include <dns_strings.h>
#include <events.h>
#include <names.h>


/*
 * sys_bld_filename
 *	Create a file name from the clearinghouse fullname 
 *
 * Input:
 *	chname_p = address of external clearinghouse full name
 *	filename_p = address to store file name
 */
dns_status_t 
sys_bld_filename (FullName_t  *chname_p,
                  char        *filename_p)
{
    DEB_ASCII_BUF_FullName(chnameBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    char prefix[_DNS_MAXPATHLEN+1];
    register char *ptr;
    int length;
    const dns_status_t status = DNS_SUCCESS;
    int prefixlen;		/* used by dce_assert */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug9,
        " >sys_bld_filename(%s,%p)",
        deb_ascii_FullName(chnameBuf,chname_p),
        filename_p));

    sprintf(prefix, CDS_SVR_DBDIR, dcelocal_path);
    prefixlen = strlen(prefix);
    dce_assert(cds__svc_handle, (4 <= prefixlen));

    ptr = &filename_p[prefixlen-4];
    length = MAX_FILE_SIZE - prefixlen + 5;
    (void)dnsCvtCDSFullToStr((dns_opq_fname *)chname_p,
	(unsigned char *)ptr, &length);

# ifdef DCE_CDS_DEBUG

    { static char globalroot[] = "/.../";
      const int is_global = EQ_bytes(ptr, globalroot, 5); 
      dce_assert(cds__svc_handle, is_global); }
# endif

    ptr = &filename_p[prefixlen+1]; /* skip "/.../" */

    do {
	if (*ptr == '/')
	    *ptr = '#';
    } while (*ptr++);

    (void)strncpy(filename_p, prefix, prefixlen);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug9,
        " <sys_bld_filename(%s,%p->%s) return(%s)",
        deb_ascii_FullName(chnameBuf,chname_p),
        filename_p,
        filename_p,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/*
 *        sys_set_data_limit
 *
 * Sets the maximum size, in bytes of the data segment.
 *   
 * Inputs:
 *   data_size: size in bytes
 *   
 * Outputs:
 *   none
 *
 * Returns:
 *   status     1 on success, 0 on failure
 *   
 */
int 
sys_set_data_limit (int data_size)
{
#ifdef RLIMIT_DATA
# include <sys/time.h>
# include <sys/resource.h>
  extern int setrlimit (int resource, struct rlimit *rip);
  struct rlimit rlim;

  rlim.rlim_cur = data_size; /* (this seems to be ignored) */
  rlim.rlim_max = data_size;
  return ((setrlimit(RLIMIT_DATA, &rlim) == -1) ? 0 : 1);
#else
  return(1);
#endif
}





