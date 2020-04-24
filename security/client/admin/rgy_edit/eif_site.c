/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: eif_site.c,v $
 * Revision 1.1.4.2  1996/03/11  13:26:44  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:14:42  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:58:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:29:51  root]
 * 
 * Revision 1.1.2.2  1992/12/29  12:39:21  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:12:01  zeliff]
 * 
 * Revision 1.1  1992/01/19  14:45:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/

/*  eif_site.c V=14 11/05/91 //littl/prgy/src/edrgy
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
**      rgy_edit generic pgo interface routines
** 
*/

#ifdef MODULE_VERSION_ID
static char *VersionID = "@(#)eif_site.c	14 - 11/05/91";
#endif

#include <stdio.h>
#include <un_strng.h>
#include <macros.h>

#include <dce/rgybase.h>

#include <ek_types.h>
#include <ek_error.h>
#include <tty_io.h>
#include <eif.h>
#include <eif_site.h>
 
/*
** PRIVATE routines
*/

PRIVATE  void  eif_print_site
#ifndef __STDC__
    ( preamble, site_name, access ) 
    char              *preamble;
    ek_site_name_t    site_name;
    ek_site_access_t  access;
#else
  (
    char              *preamble,
    ek_site_name_t    site_name,
    ek_site_access_t  access
  )
#endif
{
    char              site_str[sizeof(ek_site_name_t) + 1];

    strcpy(site_str, (char *)site_name);
    printf("%s %s %s ", preamble, "registry server at", site_str);
    if (access == ek_site_query)
	printf("%s\n", "  (read-only)");
    else
	printf("\n");
}
	       
 
/* 
** Public routines
*/

/* eif_site_print_current
 *
 *  prints the current site and it's access mode
 */
PUBLIC void eif_site_print_current (
#ifdef __STDC__
    void
#endif
)
{
    ek_site_name_t    current_site;
    ek_site_access_t  cur_access;
    error_status_t    st;

    st = ek_site_get_current(current_site, &cur_access);
    if (BAD_STATUS(&st)) {
	strcpy((char *)current_site, "<none>");
    }

    eif_print_site(eif_current_site, current_site, cur_access);
}

 

/*  eif_site_open
 *
 * attempts to open a specific site.  If successful, prints the site name and 
 * access.  If unsuccessful, prints a message and returns false.
 */

PUBLIC error_status_t eif_site_open
#ifndef __STDC__
    ( site ) 
    char               *site;     /* [in]  */
#else
  (
    char               *site
  )
#endif
{
    error_status_t     st,
                       local_status;
    ek_site_name_t     new_site,
                       prev_site;
    ek_site_access_t   new_access,
                       prev_access;
    boolean32          bound;
    
    local_status = ek_site_get_current(prev_site, &prev_access);
    bound =  (local_status != ek_misc_no_site);

    strncpy((char *)new_site, site, sizeof(new_site));

    st = ek_site_open_by_name(new_site, &new_access);
    if (BAD_STATUS(&st) && (!STATUS_EQUAL(&st, ek_misc_unauth_binding))) {
        tty_status_post_echo(site, eif_cant_open_site, st);
            /* if there was a previous binding, it's still in effect
             * so remind the user about it.
             */
        if (bound) 
            eif_site_print_current();
    } else {
        if (STATUS_EQUAL(&st, ek_misc_unauth_binding)) {
            tty_print_status("Warning - binding is unauthenticated", st);
            CLEAR_STATUS(&st);
        }
        if (bound)
            eif_print_site(eif_site_changed, new_site, new_access);
        else
            eif_print_site(eif_current_site, new_site, new_access);
    }
    return st;
}


/* eif_site_open_by_access
*/    
PUBLIC error_status_t eif_site_open_by_access
#ifndef __STDC__
    ( desired_access ) 
    ek_site_access_t    desired_access;  
#else
  (
    ek_site_access_t    desired_access
  )
#endif
{
    error_status_t      st,
                        local_st;
    ek_site_access_t    new_access,
                        prev_access;
    ek_site_name_t      new_site,
                        prev_site;
    boolean32           bound;

    local_st = ek_site_get_current(prev_site, &prev_access);
    bound =  (local_st != ek_misc_no_site);

    new_access = desired_access;
    st = ek_site_open_by_access(new_site, &new_access);
    if (BAD_STATUS(&st)) {
	if (STATUS_EQUAL(&st, ek_misc_read_only)) {
	    SET_STATUS(&local_st, ek_misc_no_update_site);
	    tty_print_status(eif_no_update_site, local_st);
	} else if (STATUS_EQUAL(&st, ek_misc_no_update_site)) {
	    tty_print_status(eif_no_update_site, st);
	} else if (STATUS_EQUAL(&st, ek_misc_unauth_binding)) {
            tty_print_status("Warning - binding is not authenticated", st);
            CLEAR_STATUS(&st);
        } else {
	    tty_print_status(eif_no_site_in_service, st);
	}

	if (bound) 
	    eif_site_print_current();
    } else {
	if (bound)
	    eif_print_site(eif_site_changed, new_site, new_access);
	else
	    eif_print_site(eif_current_site, new_site, new_access);
    }
    
    return st;
}

   
/* eif_site_check
 *
 * Checks that for a site that meets the access requirements specified in the 
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
 *   1) Try to find a site with the desired access.  If we can't, return 
 *      ek_misc_server_unavailable  (NOTE:  if we wanted an update site but
 *      got a readonly, then we close the readonly site before returning).
 *  
 *   2) Try to open an update site.  If we can't, return ek_misc_read_only
 *      (remember that the previous readonly binding is still in effect).  No
 *      error output necessary (let the caller decide whether or not to report
 *      it)
 *
 *   3) Try to contact a site that is up and supports the desired access.  If
 *      we can't, return ek_misc_server_unavailable.
 *
 *   If the site binding is changed for any reason, the info for the new site is
 *   printed.  This keeps the user informed of changes in the site binding.
 *
 * returns:
 *   ek_misc_read_only  - if the caller specified update access, but a readonly
 *                         site was found instead, or the current site is readonly
 *                         and no update site is available.
 *
 *   ek_misc_server_unavailable - if no registry server is available
 *
 */
PUBLIC error_status_t eif_site_check
#ifndef __STDC__
    ( desired_access, rebind ) 
    ek_site_access_t  desired_access;  
    boolean32         rebind;
#else
  (
    ek_site_access_t  desired_access,  
    boolean32         rebind
  )
#endif
{
    boolean32         bound,             /* are we currently bound to a site? */
                      print_site = false;  /* do we need to print site info */
    error_status_t    local_status,
                      st;
    ek_site_access_t  prev_access,
                      new_access;
    ek_site_name_t    prev_site,
                      new_site;

    local_status = ek_site_get_current(prev_site, &prev_access);
    bound =  (local_status != ek_misc_no_site);

    if (!bound) {   /* we're not currently bound to anything */

	if (rebind) {
	    new_access = desired_access;
	    st = ek_site_open_by_access(new_site, &new_access);
	    if (STATUS_EQUAL(&st, ek_misc_read_only)) {
	        /* we wanted an update site but got a read_only site */
		ek_site_close();
		SET_STATUS(&st, ek_misc_server_unavailable);
	    }
	    if (GOOD_STATUS(&st)) {
		/* remember to print the site before we leave */
		print_site = true;   
            } else if (STATUS_EQUAL(&st, ek_misc_unauth_binding)) {
                tty_print_status("Warning - binding is not authenticated", st);
                CLEAR_STATUS(&st);
            }
	}  else
	    SET_STATUS(&st, ek_misc_server_unavailable);

    } else {     /* we're already bound to something */

	    /* make sure the current site is reachable */
	st = ek_site_ping(prev_site, &prev_access);
	
	if (GOOD_STATUS(&st)) {    /* we can talk to the current site */

	    if (prev_access == ek_site_query && 
		desired_access == ek_site_update) {
		if (rebind) {
	            /* the site is there but doesn't give us update access, 
		     * so try to find one that does.
		     */
		    new_access = desired_access;
		    st = ek_site_open_by_access(new_site, &new_access);
		    if (GOOD_STATUS(&st)) {    /* we got what we wanted */
			print_site = true;
		    } else if (STATUS_EQUAL(&st, ek_misc_read_only)) {
			/* we wanted an update site but got a read_only site.  
			 * If we've changed the current site, then we need to 
			 * display the new site info.  
			 */
			print_site = true;
		    } else if (BAD_STATUS(&st)) {
			if (desired_access == ek_site_update) 
		            /* no update site was available and the previous
			     * binding is still in effect.  return a read_only
			     * status but don't print the site because it
			     * hasn't changed.  
			     */
			    SET_STATUS(&st, ek_misc_read_only);
		    }
		} else
		    SET_STATUS(&st, ek_misc_read_only);
	    }
	} else {      /* the current site has gone away */
		
	        /* look for a site that will speak to us */
	    if (rebind) {
		new_access = desired_access;
		st = ek_site_open_by_access(new_site, &new_access);
		if (GOOD_STATUS(&st)) {    /* we got what we wanted */
		    print_site = true;
		} else if (STATUS_EQUAL(&st, ek_misc_read_only)) {
	            /* we wanted an update site but got a read_only site.  If
		     * we've changed the current site, then we need to display
		     * the new site info.  
		     */
		    print_site = true;
		} else if (BAD_STATUS(&st)) {
		    /* the current site was not available and we weren't able to
                     * to bind to a new one that met our access needs.  Just
                     * return a server_unavailable status.
		     */
		    SET_STATUS(&st, ek_misc_server_unavailable);
		}
	    } else 
		SET_STATUS(&st, ek_misc_server_unavailable);
	}
    }
  
        /* if we were already bound, don't print new site info unless it's
	   really different than the one we were previously bound to */
    if ( (bound)  && (strcmp((char*)prev_site, (char *)new_site)) )
	print_site = false;
    
    if (print_site) {
	if (bound)
	    eif_print_site(eif_site_changed, new_site, new_access);
	else
	    eif_print_site(eif_current_site, new_site, new_access);
    }    

    return st;
}
