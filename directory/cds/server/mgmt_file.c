/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: mgmt_file.c,v $
 * Revision 1.1.8.2  1996/02/18  19:36:19  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:53  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:31:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:27  root]
 * 
 * Revision 1.1.5.4  1994/08/03  19:04:21  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:25:46  mccann]
 * 
 * Revision 1.1.5.3  1994/06/09  18:43:34  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:07  devsrc]
 * 
 * Revision 1.1.5.2  1994/04/14  14:51:21  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:20:56  peckham]
 * 
 * Revision 1.1.5.1  1994/03/12  22:11:52  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:43:43  peckham]
 * 
 * Revision 1.1.3.4  1992/12/30  14:22:17  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:22:48  zeliff]
 * 
 * Revision 1.1.3.3  1992/12/04  21:32:42  keegan
 * 	In 'sys_mgmt_create_new_file', re-init status to > 0 so that if
 * 	the ch list is empty and the 'for' loop is null, we will exit
 * 	this proc with DNS_SUCCESS.
 * 	[1992/12/04  21:31:57  keegan]
 * 
 * Revision 1.1.3.2  1992/09/29  19:16:02  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:17:13  weisman]
 * 
 * Revision 1.1  1992/01/19  15:25:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module mgmt_file.c
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
 *
 */

#include <server.h>
#include <fcntl.h>
#ifdef SNI_SVR4_POSIX
#include <sys/types.h>
#endif
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <dns_strings.h>


/* 
 * Read the dns_file.txt file and for each clearinghouse listed
 * allocate a nsgbl_ch structure and link it to dns_nameserver
 *
 */
void 
sys_mgmt_file_load (nsgbl_ns_t *ns_p)
{
    DEB_ASCII_BUF_nsgbl_ns(nsBuf)
    FILE    		*f_ptr;
    char 		ns_file[MAX_FILE_SIZE];
    
    (void)sprintf(ns_file, "%s/%s", ns_p->ns_mgmt_directory, DNS_CH_FILES);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >sys_mgmt_file_load(%s) file(%s)",
        deb_ascii_nsgbl_ns(nsBuf,ns_p),
        ns_file));

    /* Assume file doesn't exist yet, must be doing a create */
    if ((f_ptr = fopen(ns_file, "r")) != NULL)
	{
	struct nsgbl_ch	*ch_p;
	char ch_file[MAX_FILE_SIZE], *char_p, *eol_p;

	/* Read each record and build nsgbl_ch structure */

	while ((char_p = fgets(ch_file, sizeof(ch_file), f_ptr)) != NULL)
	{
	    /* search for and of line in record and replace with '\0' */
	    eol_p = strchr(ch_file, '\n');
	    if (eol_p)
		*eol_p = '\0';
	    else
		ch_file[MAX_FILE_SIZE - 1] = '\0';

	    if ((ch_p = nsgbl_ch_malloc(ns_p)) != NULL)
	    {
		COPY_bytes(ch_file, ch_p->ch_filename, strlen(ch_file));
         
		DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_server,
                    svc_c_debug5,
                    "sys_mgmt_file_load(%s) clearinghouse %s identified",
                   deb_ascii_nsgbl_ns(nsBuf,ns_p),
                   ch_file));
	    }
	    else
	    {
		DCE_SVC_DEBUG((
                    cds__svc_handle,
                    cds_svc_server,
                    svc_c_debug1,
		    "sys_mgmt_file_load(%s) could not load clearinghouse %s, aborting",
		    deb_ascii_nsgbl_ns(nsBuf,ns_p), 
                    ch_file));
		break;
	    }
	} /* end while */
	(void)fclose(f_ptr);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <sys_mgmt_file_load(%s)",
        deb_ascii_nsgbl_ns(nsBuf,ns_p)));
}

/* 
 * Open the dns_file.txt file and add a new entry.  Create the file
 * if it doesn't already exist. 
 *
 * Input:
 *     ch_file_p - file name of clearinghouse that is being added
 *                   (null terminated)
 */
dns_status_t 
sys_mgmt_file_add (nsgbl_ns_t  *ns_p,
                   char        *ch_file_p)
{
    DEB_ASCII_BUF_nsgbl_ns(nsBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    static char		eol = '\n';
    char		ns_file[MAX_FILE_SIZE];
    int			f_dsc, u_status = -1;
    dns_status_t	status;
    
    (void)sprintf(ns_file, "%s/%s", ns_p->ns_mgmt_directory, DNS_CH_FILES);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >sys_mgmt_file_add(%s,%s) file(%s)",
        deb_ascii_nsgbl_ns(nsBuf,ns_p),
        ch_file_p,
        ns_file));

    if (0 <= (f_dsc = open(ns_file, O_CREAT|O_APPEND|O_RDWR, 0600)))
	{
	/* Write new record to end of file */
	if (0 <= (u_status = write(f_dsc, ch_file_p, strlen(ch_file_p))))
	    {
	    /* Terminate the record with new line character */
	    u_status = write(f_dsc, &eol, sizeof(eol));
	    }

	(void)close(f_dsc);
	}

    status = (0 <= u_status) ? DNS_SUCCESS : DNS_DATACORRUPTION;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <sys_mgmt_file_add(%s,%s) status(%s)",
        deb_ascii_nsgbl_ns(nsBuf,ns_p),
        ch_file_p,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/* 
 * Create a new version of the text file containing a list of all the
 * clearinghouses except the one that got deleted. This routine is called
 * when a clearinghouse is deleted.
 *
 */
dns_status_t 
sys_mgmt_create_new_file (nsgbl_ns_t *ns_p)
{
    DEB_ASCII_BUF_nsgbl_ns(nsBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    static char		eol = '\n';
    char		ns_file[MAX_FILE_SIZE];
    struct nsgbl_ch	*ch_p;
    int			f_dsc, u_status = -1;
    dns_status_t	status;
    
    (void)sprintf(ns_file, "%s/%s", ns_p->ns_mgmt_directory, DNS_CH_FILES);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " >sys_mgmt_create_new_file(%s) file(%s)",
        deb_ascii_nsgbl_ns(nsBuf,ns_p),
        ns_file));

    /* Create new version of the file - for writing only */

    if (0 <= (f_dsc = open(ns_file, O_WRONLY|O_TRUNC, 0600)))
	{
	/*
	 * add list of clearinghouses to the new file - the new file will not
	 * contain the file spec of the clearinghouse that was just deleted.
	 * Now that the file is open OK, re-init u_status to > 0 so that if
	 * the ch list is empty and the 'for' loop below is null, we will exit
	 * this proc with DNS_SUCCESS.
	 */
	u_status = 1;
	for (ch_p = (nsgbl_ch_t *)ns_p->ns_clearinghouses.next_p;
	     ch_p != (nsgbl_ch_t *)&ns_p->ns_clearinghouses;
	     ch_p = (nsgbl_ch_t *)ch_p->ch_clearinghouses.next_p)
	    {
	    /* Write new record to end of file */
            u_status = write(f_dsc, ch_p->ch_filename, strlen(ch_p->ch_filename));

            /* Log error writing dns_files.txt */
            if (u_status < 0)
        	break;

            /* Terminate the record with new line character */
    	    u_status = write(f_dsc, &eol, sizeof(eol));

            /* Log error writing dns_files.txt */
            if (u_status < 0)
        	break;
	    }

	(void)close(f_dsc);
	}

    status = (0 <= u_status) ? DNS_SUCCESS : DNS_DATACORRUPTION;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug7,
        " <sys_mgmt_create_new_file(%s) return(%s)",
        deb_ascii_nsgbl_ns(nsBuf,ns_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}
