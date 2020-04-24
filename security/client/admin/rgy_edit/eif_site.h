/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: eif_site.h,v $
 * Revision 1.1.4.2  1996/03/11  13:26:45  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:43  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:59:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:29:54  root]
 * 
 * Revision 1.1.2.2  1992/12/29  12:39:28  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:12:04  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:46:14  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  eif_site.h V=7 11/22/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit generic interface pgo management
** 
*/

#ifndef eif_site_h__included 
#define eif_site_h__included 
  
#include <dce/nbase.h>

#include <ek_site.h>

/* eif_site_print_current
 *
 *  prints the current site and it's access mode to the tty interface
 */
void eif_site_print_current (
#ifdef __STDC__
    void
#endif
);


/*  eif_site_open
 *
 * attempts to open a specific site.  If successful, prints the site name and
 * access.  If unsuccessful, prints a message and returns the bad status 
 */

error_status_t eif_site_open (
#ifdef __STDC__ 
    char  *site
#endif
);
 


/* eif_site_open_by_access
 * 
 * Attempts to open a site with the specified access.  If a site is opened,
 * prints out info about the site.  If a site with the desired access is not 
 * available, prints a diagnostic, followed by info about the current site
 * binding.
 *
 * Returns:
 *   ek_misc_read_only      - if an update is desired, but we got a readonly
 *                            site instead.
 *   ek_misc_no_update_site - if an update site is requested, but no site
 *                            (either readonly or update) responds.
 *
 */ 
error_status_t eif_site_open_by_access ( 
#ifdef __STDC__
    ek_site_access_t  desired_access
#endif
);

/* eif_site_check
 *
 * Checks that the current site meets the access requirements specified in the 
 * desired_access parameter.  There are three possible reasons why the current
 * site is unsuitable.
 *
 *   1) there's never been a successful site binding in the life of this 
 *      process
 *   2) there has been a successful binding, and the current site is available
 *      but does not accept updates.
 *   3) there has been a successful binding, but the current site is not
 *      available.
 *
 * We handle the three cases as follows.
 *
 *   1) Try to find a site with the desired access.  If none available, return 
 *      ek_misc_server_unavailable  (NOTE:  if we wanted an update site but
 *      got a readonly, then we close the readonly site first).
 *  
 *   2) Try to open an update site.  If we can't, return ek_misc_read_only
 *      (remember that the previous readonly binding is still in effect).  No
 *      error output necessary (let the caller decide whether or not to report
 *      it)
 *
 *   3) Try to contact a site that is up and supports the desired access.  If
 *      we can't, return ek_misc_server_unavailable.
 *
 * returns:
 *   ek_misc_read_only  - if the caller specified update access, but a readonly
 *                        site was found instead, or the current site is
 *                        readonly and no update site is available.
 *
 *   ek_misc_server_unavailable - if no registry server is available
 *
 */
error_status_t eif_site_check (
#ifdef __STDC__ 
     ek_site_access_t  desired_access,   /* query or update */
     boolean32         rebind            /* rebind if necessary? */
#endif
);

#endif
