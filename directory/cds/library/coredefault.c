/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: coredefault.c,v $
 * Revision 1.1.8.2  1996/02/18  19:32:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:10  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:20:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:44:47  root]
 * 
 * Revision 1.1.6.5  1994/08/03  19:03:03  mccann
 * 	includes file cleanup
 * 	[1994/07/19  18:09:34  mccann]
 * 
 * Revision 1.1.6.4  1994/06/30  19:16:27  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:58:42  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:40:42  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:08  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/29  15:51:53  peckham
 * 	Remove UNIX conditionals.
 * 	Use dcecdsmac.h macros for dce_svc_printf().
 * 	Use dcelocal_path.
 * 	[1994/04/29  13:59:36  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:05:45  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:24:50  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:53:37  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:06:44  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  22:15:08  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:30:07  weisman]
 * 
 * Revision 1.1  1992/01/19  15:19:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
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
 */
#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>
#include <dns_cfg.h>
#include <dns_strings.h>

#ifdef DCE_CDS_DEBUG

/*
 * for use in dbx ONLY !!! with initialisations
 *  used to help establish core file info.
 */
int   ctlinen			 = -1;
char  ctfilen[_DNS_MAXPATHLEN+1] = "initialise-value";
char  ctpfn			 = 'u';	/* u => unset */


#endif /* DCE_CDS_DEBUG */


void 
dns_coredefault (char  *myname_p,
                 int   isserver)
{
    char *const dptr = (isserver) ? CDS_SVR_WDIR : CDS_CLI_WDIR;
    char		dirname[_DNS_MAXPATHLEN+1];
    char		corebuf[128];
    struct stat		statbuf;
    int			count;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug8,
        " >dns_coredefault(%s,%d)",
	myname_p, isserver));

    /*
     * create the component dir if necessary 
     */
    sprintf(dirname, dptr, dcelocal_path);
    if (chdir(dirname) < 0) 
    {
	dce_svc_printf(CDS_S_LIBRARY_CD_EG_MSG, dirname, (long)errno);
    }

    mkdir(myname_p, 0751);

    /*
     * cd to it 
     */
    if (chdir(myname_p) < 0) 
    {
      char    newname[_DNS_MAXPATHLEN+1];
      
      sprintf(newname, "%s/%s", dirname, myname_p);
      dce_svc_printf(CDS_S_LIBRARY_CD_EG_MSG, newname, (long)errno);
    }

    /*
     * rename any existing core file 
     */
    count = 0;
    strcpy(corebuf, "core");
    while (0 <= stat(corebuf, &statbuf))
    {
	sprintf(corebuf, DNS_CORE_TMPL, ++count);
    }

    if (0 < count)
    {
	/*
	 * there was a core file 
	 */
	rename("core", corebuf);
    }
    

   DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug8,
        " <dns_coredefault(%s,%d)",
	myname_p, isserver));
}

