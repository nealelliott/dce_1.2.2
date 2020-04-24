/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_admin.c,v $
 * Revision 1.1.11.1  1996/11/13  17:58:55  arvind
 * 	Set and return proper exit status.
 * 	[1996/10/24  21:40 UTC  psn  /main/psn_secrep/1]
 *
 * Revision 1.1.9.2  1996/03/11  13:27:34  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:25  marty]
 * 
 * Revision 1.1.9.1  1995/12/08  18:03:03  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/5  1995/06/09  10:52 UTC  sekhar
 * 	Merge fixes to mothra.
 * 
 * 	HP revision /main/HPDCE02/sekhar_mothra_fix/1  1995/06/01  12:38 UTC  sekhar
 * 	[ CHFts15354 ] Fix incorrect diagnostic
 * 
 * 	HP revision /main/HPDCE02/4  1995/05/15  20:59 UTC  greg
 * 	Use v1_2_c_ifspec  of re_repadm interface when
 * 	converting towers.
 * 	[1995/05/13  02:40 UTC  greg  /main/HPDCE02/SEC_migrate_merge/1]
 * 
 * 	HP revision /main/HPDCE02/3  1995/04/28  20:04 UTC  mdf
 * 	Finishing touches to get this working.
 * 	[1995/04/28  12:07 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_3/3]
 * 
 * 	Fix warning messages, and now export rs_repadm.h so it doesn't have
 * 	to be rebuilt all over the place.
 * 	[1995/04/27  15:09 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_3/2]
 * 
 * 	Resolve CHFts15068
 * 	[1995/04/27  13:03 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_3/1]
 * 
 * 	HP revision /main/HPDCE02/2  1995/01/30  22:42 UTC  sommerfeld
 * 	Remove reference to hpdce_version.h or hpdfs_version.h
 * 	[1995/01/27  04:34 UTC  sommerfeld  /main/HPDCE02/sommerfeld_mothmk_2/1]
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  21:22 UTC  mullan_s
 * 	Merge.
 * 	[1995/12/08  16:30:58  root]
 * 
 * Revision 1.1.6.5  1994/07/15  14:58:36  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:14:31  mdf]
 * 
 * Revision 1.1.6.4  1994/06/02  21:57:22  mdf
 * 	Merged with changes from 1.1.6.3
 * 	[1994/06/02  21:54:17  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:13:35  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.5.5  1993/12/16  18:22:43  hanfei
 * 	verify if new master has the highest seqno for become -master cmd
 * 	[1993/12/15  21:50:03  hanfei]
 * 
 * Revision 1.1.5.4  1993/10/29  15:22:44  hanfei
 * 	activate and add codes for change_master.
 * 	[1993/10/29  15:14:56  hanfei]
 * 
 * Revision 1.1.5.3  1993/09/29  15:30:56  burati
 * 	Fix arg checking usage stmt in tty_do_lrep
 * 	[1993/09/29  15:30:34  burati]
 * 
 * Revision 1.1.5.2  1993/07/08  10:56:50  root
 * 	Initial King Kong branch
 * 	[1993/07/08  10:56:18  root]
 * 
 * Revision 1.1.3.2  1993/05/25  16:31:55  rps
 * 	standardize version string
 * 	[1993/05/25  16:22:02  rps]
 * 
 * 	CR7299 fix fprintf that was missing site name parameter in rtn
 * 	set_default_to_master.  Make lrep -prop/-all fall back to everything
 * 	except prop information, if it's not talking to the master rgy.
 * 	[1993/03/08  16:54:11  burati]
 * 
 * 	CR7339 Change prefix to error message for "state -service"
 * 	[1993/02/25  20:43:57  burati]
 * 
 * 	CR7324 Make sure we display an error if the initial site bind fails
 * 	and add the "-s name" option to specify site at invokation time.
 * 	[1993/02/23  19:58:54  burati]
 * 
 * 	Improve arg checking in site command
 * 	[1993/02/19  23:56:54  burati]
 * 
 * 	New features/commands for 1.0.2
 * 	[1993/01/14  23:01:39  burati]
 * 
 * 	Bug 6413 - Call setlocale for correct i18n.
 * 	[1993/01/08  21:00:55  tom]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  20:36:59  zeliff]
 * 
 * 	Initial version from original server/bin/rgy_admin.c
 * 	[1992/11/04  21:00:50  burati]
 * 
 * Revision 1.1.1.5  1993/05/04  20:24:11  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.6.3  1994/06/02  20:20:42  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:13:35  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.5.5  1993/12/16  18:22:43  hanfei
 * 	verify if new master has the highest seqno for become -master cmd
 * 	[1993/12/15  21:50:03  hanfei]
 * 
 * Revision 1.1.5.4  1993/10/29  15:22:44  hanfei
 * 	activate and add codes for change_master.
 * 	[1993/10/29  15:14:56  hanfei]
 * 
 * Revision 1.1.5.3  1993/09/29  15:30:56  burati
 * 	Fix arg checking usage stmt in tty_do_lrep
 * 	[1993/09/29  15:30:34  burati]
 * 
 * Revision 1.1.5.2  1993/07/08  10:56:50  root
 * 	Initial King Kong branch
 * 	[1993/07/08  10:56:18  root]
 * 
 * Revision 1.1.3.2  1993/05/25  16:31:55  rps
 * 	standardize version string
 * 	[1993/05/25  16:22:02  rps]
 * 
 * 	CR7299 fix fprintf that was missing site name parameter in rtn
 * 	set_default_to_master.  Make lrep -prop/-all fall back to everything
 * 	except prop information, if it's not talking to the master rgy.
 * 	[1993/03/08  16:54:11  burati]
 * 
 * 	CR7339 Change prefix to error message for "state -service"
 * 	[1993/02/25  20:43:57  burati]
 * 
 * 	CR7324 Make sure we display an error if the initial site bind fails
 * 	and add the "-s name" option to specify site at invokation time.
 * 	[1993/02/23  19:58:54  burati]
 * 
 * 	Improve arg checking in site command
 * 	[1993/02/19  23:56:54  burati]
 * 
 * 	New features/commands for 1.0.2
 * 	[1993/01/14  23:01:39  burati]
 * 
 * 	Bug 6413 - Call setlocale for correct i18n.
 * 	[1993/01/08  21:00:55  tom]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  20:36:59  zeliff]
 * 
 * 	Initial version from original server/bin/rgy_admin.c
 * 	[1992/11/04  21:00:50  burati]
 * 
 * Revision 1.1.1.5  1993/05/04  20:24:11  mgm
 * 	    Loading drop DCE1_0_2FNL
 * 
 * Revision 1.1.6.1  1994/03/25  17:34:02  sekhar
 * 	[ OT 10164 ]  Security SVC code drop ( sec_svc_bl1 )
 * 	[1994/03/25  17:33:36  sekhar]
 * 
 * Revision 1.1.4.1  1993/09/29  14:31:23  burati
 * 	CR8404 Fix usage() arg index in tty_do_lrep
 * 	[1993/09/28  23:50:54  burati]
 * 
 * Revision 1.1.2.9  1993/03/08  17:49:55  burati
 * 	CR7299 fix fprintf that was missing site name parameter in rtn
 * 	set_default_to_master.  Make lrep -prop/-all fall back to everything
 * 	except prop information, if it's not talking to the master rgy.
 * 	[1993/03/08  16:54:11  burati]
 * 
 * Revision 1.1.2.8  1993/02/25  20:47:04  burati
 * 	CR7339 Change prefix to error message for "state -service"
 * 	[1993/02/25  20:43:57  burati]
 * 
 * Revision 1.1.2.7  1993/02/23  20:22:38  burati
 * 	CR7324 Make sure we display an error if the initial site bind fails
 * 	and add the "-s name" option to specify site at invokation time.
 * 	[1993/02/23  19:58:54  burati]
 * 
 * Revision 1.1.2.6  1993/02/20  23:21:40  burati
 * 	Improve arg checking in site command
 * 	[1993/02/19  23:56:54  burati]
 * 
 * Revision 1.1.2.5  1993/01/29  18:53:31  burati
 * 	New features/commands for 1.0.2
 * 	[1993/01/14  23:01:39  burati]
 * 
 * Revision 1.1.2.4  1993/01/08  22:17:14  tom
 * 	Bug 6413 - Call setlocale for correct i18n.
 * 	[1993/01/08  21:00:55  tom]
 * 
 * Revision 1.1.2.3  1992/12/29  12:42:48  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:36:59  zeliff]
 * 
 * Revision 1.1.2.2  1992/11/04  22:25:54  burati
 * 	Initial version from original server/bin/rgy_admin.c
 * 	[1992/11/04  21:00:50  burati]
 * 
 * $EndLog$
 */

/* sec_admin.c
 *
 * Copyright (c) Hewlett-Packard Company 1991,1992,1993
 * Unpublished work. All Rights Reserved.
 */
/* 
 *  Security Server Admin Tool
 */

#include <pthread.h>

#include <unistd.h>
#include <sys/types.h>
#include <stdio.h>
#include <ctype.h>      /* for upper/lower case stuff */
#include <locale.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesadmsg.h>
#include <dce/rs_repadm.h>
#include <dce/dns_record.h>

#include <sa_util.h>

PUBLIC error_status_t   tty_exit_status = error_status_ok;

PRIVATE void set_default_to_master(
#ifdef __STDC__
    error_status_t              *stp
#endif
);

PRIVATE char prompt_str[] = "sec_admin> ";
#define RETURN_STATUS_ON_ERROR(status) \
  if (BAD_STATUS(&status)) { \
      return(status);\
  }
#define EXIT_ON_ERROR(status) \
  if (BAD_STATUS(&status)) { \
    exit(status); \
  }

#define SET_EXIT_STATUS(status)  { \
  if ((status) != error_status_ok) \
    tty_exit_status = (status); \
  }

/*
** Commands:
*/

COMMAND(tty_do_stop);
COMMAND(tty_do_state);
COMMAND(tty_do_master_key);
COMMAND(tty_do_quit);
COMMAND(tty_do_help);
COMMAND(tty_do_nil);
COMMAND(tty_do_nogood);
COMMAND(tty_do_site);
COMMAND(tty_do_info);
COMMAND(tty_do_initr);
COMMAND(tty_do_delr);
COMMAND(tty_do_replr);
COMMAND(tty_do_lrep);
COMMAND(tty_do_monitor);
COMMAND(tty_do_destroy);
COMMAND(tty_do_chm);
COMMAND(tty_do_become);

command_table commands[] = {
   { "become",      tty_do_become,      tty_help_become,    6 },
   { "change_master", tty_do_chm,       tty_help_chm,       3 },
   { "delrep",      tty_do_delr,        tty_help_delr,      4 },
   { "destroy",     tty_do_destroy,     tty_help_destroy,   5 },
   { "exit",        tty_do_quit,    	tty_help_quit,      1 },
   { "help",        tty_do_help,        tty_do_nil,         1 },
   { "info",        tty_do_info,        tty_help_info,      3 },
   { "initrep",     tty_do_initr,       tty_help_initr,     5 },
   { "lrep",        tty_do_lrep,        tty_help_lrep,      2 },
   { "master_key",  tty_do_master_key,  tty_help_mkey,      3 },
   { "monitor",     tty_do_monitor,     tty_help_mon,	    3 },
   { "quit",        tty_do_quit, 	tty_help_quit,      1 },
   { "replrep",     tty_do_replr,       tty_help_replr,     4 },
   { "site",        tty_do_site,        tty_help_site,      2 },
   { "state",       tty_do_state,       tty_help_state,     3 },
   { "stop",        tty_do_stop,        tty_help_stop,	    4 },
   { "?",           tty_do_help,    	tty_do_nil,         1 },
   { "",            tty_do_nil,     	tty_do_nil,         0 },
   { "#",           tty_do_nil,     	tty_do_nil,         1 },
   { "*",           tty_do_nil,     	tty_help_all,       0 },
   { NULL,          tty_do_nogood,  	tty_do_nil,         0 }
};

/* Global state of sec_admin */
static boolean32            must_prompt = true;
static sec_rgy_handle_t     master_context;
static char rgy_admin_version_id[] = "sec_admin v1.0.2";    
sec_rgy_handle_t            default_context;
boolean32                   sa_query = true;

/*
 * Provide info on the current status of the default replica
 */
PUBLIC error_status_t tty_do_info
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{        
    rs_replica_info_t           info;
    rs_replica_twr_vec_p_t      rtwrs = NULL, mtwrs = NULL;
    boolean32       	        full = false;
    error_status_t              st;

    CLEAR_STATUS(&st);
    switch (c->num_args) {
    case 1:
	break;
    case 2:
	if (match_command("-full", c->args[1], 2)) {
            full = true;
	} else {
            fprintf(stderr, "Unknown option %s\n", c->args[1]);
            cmd_usage("info");
	    return st;
	}
	break;
    default:
	fprintf(stderr, "Incorrect number of arguments\n");
	cmd_usage("info");
	return st;
    }

    printf("\n");
    sa_print_def_rep_info(default_context, &st);
    if (full) {
	sec_rgy_rep_admin_info_full(default_context,&info,&rtwrs,&mtwrs,&st);
    } else {
	sec_rgy_rep_admin_info(default_context, &info, &st);
    }
    if(BAD_STATUS(&st)) {
	fprintf(stderr, "%sState: ", saINDENT);
	sa_error_print(st);
    } else {
	sa_print_site_info(&info, true, full, rtwrs, mtwrs, &st);
    }

    SET_EXIT_STATUS(st);
    return st;
}


/*
 * Periodically list the registry replicas as stored in the replica
 * list at the default replica, and show the current state and update time
 * at each replica.  The -r option causes the replicas to be listed
 * every m minutes.  If this option is omitted, the period is 15 minutes.
 */
PUBLIC error_status_t tty_do_monitor
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{        
    error_status_t      st, lst;
    struct timespec     period;
    long                minutes;

    period.tv_nsec = 0;
    period.tv_sec = 15*60; /* Default is 15 min */
    CLEAR_STATUS(&st);
    switch (c->num_args) {
    case 1:
	break;
    case 2:
	if (match_command("-r", c->args[1], 2)) {
	    fprintf(stderr, "The -r option requires an argument\n");
	    cmd_usage("monitor");
	} else {
            fprintf(stderr, "Unknown option %s\n", c->args[1]);
            cmd_usage("monitor");
	}
	return st;
    case 3:
        if (match_command("-r", c->args[1], 2)) {
            minutes = strtol(c->args[2], (char **)NULL, 10);
            if ((minutes==0) || (minutes==LONG_MAX) || (minutes==LONG_MIN)) {
                fprintf(stderr,
		    "%s is not a legal integer value or is out of range\n",
                    c->args[2]);
                cmd_usage("monitor");
                return st;
            }
            period.tv_sec = 60 * minutes;
        }
        break;
    default:
        fprintf(stderr, "Incorrect number of arguments\n");
        cmd_usage("monitor");
        return st;
    }

    sa_print_def_rep_info(default_context, &st);
    for (;;) {
        sa_refresh_replist(default_context, &st);
        sa_display_replist(false, false, true, &st);
        if (BAD_STATUS(&st)) {
            msg_status_print("Error while attempting to display replicas",st);
        }
	pthread_delay_np(&period);
    }

    SET_EXIT_STATUS(st);
    return st;
}


/*
 * Change the master replica of the registry from default_replica to
 * other_replica.  If default_replica is not currently set to the master,
 * then rebind to the master (issuing a warning) before executing the
 * command.
 */
PUBLIC error_status_t tty_do_chm
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{        
    error_status_t      st, lst;
    char		*new_master;
    uuid_t		new_master_id;
    uuid_t              master_id;
    rs_update_seqno_t   new_master_seqno;
    rs_replica_info_t   info;

    CLEAR_STATUS(&st);
    switch (c->num_args) {
    case 3:
	if (match_command("-to", c->args[1], 2)) {
	    new_master = c->args[2];
	} else {
	    fprintf(stderr, "Illegal option %s\n", c->args[1]);
	    cmd_usage("change_master");
	    return st;
	}
	break;
    default:
	fprintf(stderr, "Incorrect number of arguments\n");
	cmd_usage("change_master");
	return st;
    }

    /* set up current master's binding */
    set_default_to_master(&st);
    if (GOOD_STATUS(&st)) {
    	/* find out what is the new_master_id */
        if (sa_get_rep_id_from_name((unsigned_char_p_t)c->args[2], 
		&new_master_id) ) {
	    /* no need to check for status, called before */
    	    sec_rgy_rep_admin_info(default_context, &info, &lst);
	    if ( uuid_equal(&info.master_id, &new_master_id, &lst) ) {
                fprintf(stderr, 
			"%s is already a master.\n", c->args[2]);
                SET_EXIT_STATUS(st);;
 		return st;
	    }

            if (sa_ask_y_n()) {
    		sec_rgy_rep_admin_change_master(default_context, 
			&new_master_id, &st);
    		if(BAD_STATUS(&st)) {
        	    msg_status_print("\tCannot do change_master\n", st);
                    SET_EXIT_STATUS(st);;
        	    return st;
    		}
            }
        } else {
            fprintf(stderr, "%s not found in replica list.\n", c->args[2])
;
        }
    } else {
        msg_status_print("Unable to make sure default replica is master",
			st);
        SET_EXIT_STATUS(st);;
	return st;
    }

    SET_EXIT_STATUS(st);
    return st;
}


/*
 * Bind to an update site
 */
PRIVATE void bind_to_master
#ifndef __STDC__
    (context, stp)  
    sec_rgy_handle_t            *context;
    error_status_t              *stp;
#else
  (
    sec_rgy_handle_t            *context,
    error_status_t              *stp
  )
#endif
{
    sec_rgy_pname_t             site_name;
             
    site_name[0] = '\0';
    /* sec_rgy_site_open_update will find the one
     * and only update site (i.e., the master).
     */
    sec_rgy_site_open_update(site_name, context, stp); 
}


/*
 * set_default_to_master
 * 
 * For commands that must be bound to the master replica:
 *  Get the rep info from the default replica and see if we're the master.
 *  If we're not already set to the master replica:
 *   Print out a warning saying the current default replica is not the master,
 *   and that we're attempting to rebind to the master for them.
 *   Bind to the master registry in the cell of our current default replica.
 *   If we found it, then rebind with it's name to make sure our handle
 *   doesn't have rebinding information in it that would allow the client
 *   agent to rebind to another replica, without our knowing it.
 */
PRIVATE void set_default_to_master
#ifndef __STDC__
    (stp)
    error_status_t              *stp;
#else
  (
    error_status_t              *stp
  )
#endif
{
    rs_replica_info_t	info;
    sec_rgy_handle_t	context;
    unsigned_char_p_t   cname, sname;
    boolean32           freethem = true;
    error_status_t      lst;
    
    sec_rgy_rep_admin_info(default_context, &info, stp);
    if (GOOD_STATUS(stp) && (! info.master)) {
        sec_rgy_site_binding_get_info(default_context, &cname, &sname, NULL,
	    NULL, &lst);
	if (BAD_STATUS(&lst)) {
	    sname = (unsigned_char_p_t) "<REPLICA NOT FOUND>";
	    cname = (unsigned_char_p_t) "/.:";
            freethem = false;
	}
	fprintf(stderr,
	    "\nWARNING: Command requires default replica to be the master\n");
	fprintf(stderr, "%s is not a master replica\n", sname);
	fprintf(stderr,
	    "Attempting to reset the default replica to the master now\n");
	sec_rgy_site_open_update(cname, &context, stp);
	if (GOOD_STATUS(stp)) {
	    if (freethem) {
		free(cname);
		free(sname);
	    }
	    /* Need to make sure it can't rebind to another site */
            sec_rgy_site_binding_get_info(context, &cname, &sname,
		NULL, NULL, stp);
	    if (GOOD_STATUS(stp)) {
		sec_rgy_site_close(context, &lst);
		sec_rgy_site_open_update(sname, &context, stp);
		if (GOOD_STATUS(stp)) {
		    sec_rgy_site_close(default_context, &lst);
		    default_context = context;
		    printf("Default replica:  %s\n", sname);
		    printf("Default cell:     %s\n", cname);
		}
	    } else {
		sec_rgy_site_close(context, &lst);
		freethem = false;
	    }
	}
	if (freethem) {
	    free(cname);
	    free(sname);
	}
    }
}


/* 
 * do_set
 *
 * This routine binds to a user-specified  (or random) server, figures out
 * the name associated with the server address, and acquires the
 * replica list.   This last step is important because most other
 * routines need to do address-to-id mappings.  This mapping 
 * requires a replica list.
 */
PRIVATE void do_set
#ifndef __STDC__
    ( r_name, r_master, stp ) 
    sec_rgy_pname_t   *r_name;
    boolean32         r_master;
    error_status_t    *stp;
#else
  (
    sec_rgy_pname_t   *r_name,
    boolean32         r_master,
    error_status_t    *stp
  )
#endif
{  
#ifdef NOTDEF
    /* Acquire a handle to the master if one doesn't already exist. */
    if ( master_context == NULL ) {
        bind_to_master (&master_context, stp);             
        if(STATUS_OK(stp)) {
            sec_rgy_site_get(master_context, &master_name, stp);
        }
        if(BAD_STATUS(stp)) {
            printf("\tCannot bind to master replica\n");
            master_name = (unsigned_char_p_t) "";
            return;
        }
    }

    /* Get a handle to the desired replica site */
    if (r_master) {
        bind_to_master (&default_context, stp);             
        if(BAD_STATUS(stp)) {
            printf("%sCannot bind to master replica\n", saINDENT);
            return;
        }
    }
    else {             
        /* r_name may be NULL, so we use "" if it is */
        if (r_name) {
            sec_rgy_site_open(*r_name, &default_context, stp);
        } else {
            sec_rgy_site_open((unsigned_char_p_t)"", &default_context, stp); 
        }
            
        if(STATUS_OK(stp)) {
            sec_rgy_site_get(default_context, &rep_name, stp);
        }
        if(BAD_STATUS(stp)) {
            printf("%sCannot bind to replica\n", saINDENT);
            rep_name = (unsigned_char_p_t) "";
        }
    }
#endif
}


PUBLIC error_status_t tty_do_stop
#ifndef __STDC__
    (c)
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    error_status_t      st;
              
    CLEAR_STATUS(&st);
    if (c->num_args > 1) {
        fprintf(stderr, "Incorrect number of arguments\n");
        cmd_usage("stop");
        return(st);
    }

    sec_rgy_rep_admin_stop(default_context, &st);
    if (BAD_STATUS(&st)) {
        fprintf(stderr, "%sCannot stop replica - ", saINDENT);
        sa_error_print(st);
    }
    SET_EXIT_STATUS(st);
    return(st);
}


/*
 * Reinitialize the replica identified by the required argument
 */
PUBLIC error_status_t tty_do_initr
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    uuid_t	   rep_id;
    error_status_t st;
    CLEAR_STATUS(&st);
    switch(c->num_args) {
    case 1:
	fprintf(stderr, "Missing required argument\n");
	cmd_usage("initrep");
	return st;
    case 2:
	break;
    default:
	fprintf(stderr, "Incorrect number of arguments\n");
	cmd_usage("initrep");
	return st;
    }

    set_default_to_master(&st);
    if (GOOD_STATUS(&st)) {
        if ( sa_get_rep_id_from_name((unsigned_char_p_t)c->args[1], &rep_id)) {
            if (sa_ask_y_n()) {
                sec_rgy_rep_admin_init_replica(default_context, &rep_id, &st);
		if (BAD_STATUS(&st)) {
                   msg_status_print("Initialize replica operation failed",st);
		}
            }
        } else {
       	    fprintf(stderr, "%s not found in replica list.\n", c->args[1]);
	}
    } else {
        msg_status_print("Unable to make sure default replica is master", st);
    }
    SET_EXIT_STATUS(st);
    return st;
}

    
/*
 * Delete the replica identified by the required other_replica argument
 * The default replica must be set to the master registry site.
 */
PUBLIC error_status_t tty_do_delr
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    boolean32	    force = false;
    uuid_t	    rep_id;
    error_status_t  st;
    
    CLEAR_STATUS(&st);
    switch(c->num_args) {
    case 1:
	fprintf(stderr, "Missing required argument\n");
	cmd_usage("delrep");
	return st;
    case 2:
	break;
    case 3:
	if (match_command("-force", c->args[2], 2)) {
	    force = true;
	} else {
            fprintf(stderr, "Unknown option %s\n", c->args[2]);
            cmd_usage("delrep");
	    return st;
	}
	break;
    default:
	fprintf(stderr, "Incorrect number of arguments\n");
	cmd_usage("delrep");
	break;
    }

    set_default_to_master(&st);
    if (GOOD_STATUS(&st)) {
        if (sa_get_rep_id_from_name((unsigned_char_p_t)c->args[1], &rep_id)) {
            if (sa_ask_y_n()) {
                sec_rgy_replist_delete_replica(default_context, &rep_id,
                    force, &st);
                if (BAD_STATUS(&st)) {
                    msg_status_print("Delete replica operation failed",st);
                }
            }
        } else {
            fprintf(stderr, "%s not found in replica list.\n",c->args[1]);
        }
    } else {
        msg_status_print("Unable to make sure default replica is master", st);
    }
    SET_EXIT_STATUS(st);
    return st;
}

/*
 */
PUBLIC error_status_t tty_do_replr ( command_buf *c)
{
    char 			*rep_name=NULL;
    unsigned_char_p_t 		addr_string;
    uuid_t	    		rep_id;
    rpc_binding_handle_t	binding;
    rpc_tower_vector_p_t 	tower_vector_p;

    rs_replica_twr_vec_p_t	rep_twrs;
    error_status_t  		st;
    unsigned32			i, j, cnt, rpcstatus;

    CLEAR_STATUS(&st);

    if (c->num_args < 4 ) {
	fprintf(stderr, "Incorrect number of arguments\n");
	cmd_usage("replrep");
	return st;
    }

    if (!match_command("-addr", c->args[2], 2)) {
	fprintf(stderr, "Unknown option %s\n", c->args[2]);
	cmd_usage("replrep");
	return st;
    }

    rep_name = c->args[1];

    rep_twrs = malloc(sizeof(*rep_twrs) + (c->num_args - 3)*sizeof(twr_p_t));

    if (rep_twrs == NULL) {
	fprintf(stderr, "unable to allocate memory.\n");
	SET_STATUS(&st, sec_s_no_memory);
        SET_EXIT_STATUS(st);
	return st;
    }
    rep_twrs->num_towers = 0;
    cnt = 0; 

    set_default_to_master(&st);
    if (GOOD_STATUS(&st)) {
        if (sa_get_rep_id_from_name(rep_name, &rep_id)) {
            if (sa_ask_y_n()) {
		for ( j=3; j < c->num_args; j++) {
		    addr_string = (unsigned_char_p_t)c->args[j];
		    rpc_binding_from_string_binding(addr_string, &binding, &st);
		    if (BAD_STATUS(&st)) {
                        msg_status_print("rpc_binding_from_string_binding failed",st);
			continue;
		    }
		    rpc_tower_vector_from_binding(rs_repadm_v1_2_c_ifspec, 
				binding, &tower_vector_p, &rpcstatus);
		    if (rpcstatus != rpc_s_ok) {
                        msg_status_print("rs_server_bindings_to_twr_vector failed",st);
		        rpc_binding_free(&binding, &st);
			continue;
		    }
		    if (tower_vector_p->count >= 1) {
                            /* copy it to stub memory */
                            rep_twrs->towers[cnt] = malloc(
                                     tower_vector_p->tower[0]->tower_length +
                                                             sizeof(twr_t));
                            if (rep_twrs->towers[cnt]) {
                                COPY_bytes(tower_vector_p->tower[0],
                                           rep_twrs->towers[cnt],
                                       tower_vector_p->tower[0]->tower_length +
                                       sizeof(twr_t));
                                ++cnt;
                            }
		    }
		    rpc_tower_vector_free(&tower_vector_p, &rpcstatus);
		    rpc_binding_free(&binding, &st);
	        }
		rep_twrs->num_towers = cnt;

                sec_rgy_replist_replace_replica(default_context, &rep_id, 
		    rep_name, rep_twrs, &st);

                if (BAD_STATUS(&st)) {
                    msg_status_print("Replace replica operation failed",st);
                }

		for (i = 0 ; i < cnt; i++) 
		    free(rep_twrs->towers[i]);
            }
        } else {
            fprintf(stderr, "%s not found in replica list.\n",c->args[1]);
        }
    } else {
        msg_status_print("Unable to make sure default replica is master", st);
    }
    free(rep_twrs);
    SET_EXIT_STATUS(st);
    return st;
}


/*
 * Destroy the specified registry replica.  The default replica must
 * be set (ie: via site command) to the specified replica before this
 * command is executed, to make sure that this is really what you want
 * to do.
 */
PUBLIC error_status_t tty_do_destroy
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    error_status_t      st;
    rs_replica_info_t   info;
    uuid_t              id;
    CLEAR_STATUS(&st);
    switch(c->num_args) {
    case 1:
	fprintf(stderr, "Missing required argument.\n");
	cmd_usage("destroy");
	return st;
    case 2:
	break;
    default:
	fprintf(stderr, "Incorrect number of arguments.\n");
	cmd_usage("destroy");
	return st;
    }

    /* Get info from default replica and make sure it matches the id obtained
     * from the specified replica, as found on the replica list.
     */
    sec_rgy_rep_admin_info(default_context, &info, &st);
    if (BAD_STATUS(&st)) {
        msg_status_print("Unable to obtain necessary info from replica", st);
        SET_EXIT_STATUS(st);
        return st;
    }
    if (! sa_get_rep_id_from_name(c->args[1], &id)) {
        fprintf(stderr, "Specified replica not found in replica list.\n");
        SET_EXIT_STATUS(st);
        return st;
    }
    /* Make sure the id's match */
    if (! uuid_equal(&id, &info.rep_id, &st)) {
        fprintf(stderr,
            "Default replica site does not match specified replica.\n");
        fprintf(stderr,
        "The default replica must be set to %s before it may be destroyed.\n",
        c->args[1]);
        SET_EXIT_STATUS(st);
        return st;
    }
    if (sa_ask_y_n()) {
        sec_rgy_rep_admin_destroy(default_context, &st);
        if (BAD_STATUS(&st)) {
            msg_status_print("Destroy operation failed", st);
	}
    }

    SET_EXIT_STATUS(st);
    return st;
}

    
/*
 * List the registry replicas as stored in the replica list at the default
 * replica.  Page in the replica list each time this command is executed.
 */
PUBLIC error_status_t tty_do_lrep
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    boolean32	    lstate, luuid, laddr, lprop, lall;
    error_status_t  st;
    int		    i;
        
    CLEAR_STATUS(&st);
    lstate = luuid = laddr = lprop = lall = false;
    
    for (i = 1; i < c->num_args; i++) {
	if (match_command("-state", c->args[i], 2)) {
	    lstate = true;
	} else if (match_command("-uuid", c->args[i], 2)) {
	    luuid = true;
	} else if (match_command("-addr", c->args[i], 3)) {
	    laddr = true;
	} else if (match_command("-prop", c->args[i], 2)) {
	    lprop = true;
	} else if (match_command("-all", c->args[i], 3)) {
	    lstate = luuid = laddr = lprop = lall = true;
	} else {
            fprintf(stderr, "Unknown option %s\n", c->args[i]);
            cmd_usage("lrep");
	    return st;
	}
    }

    sa_print_def_rep_info(default_context, &st);
    /* If we can't even figure out what site we're bound to, bail out */
    if (BAD_STATUS(&st)) {
        SET_EXIT_STATUS(st);
	return st;
    }
    
    /* lprop requires a replist_full, so break it up into two branches */
    if (lprop) {
	sa_refresh_full_replist(default_context, &st);
	/* If we're not bound to a master, print a warning and
	 * continue without prop info.
	 */
	if (STATUS_EQUAL(&st, sec_rgy_rep_not_master)) {
            printf("WARNING: The default replica is not the master registry.\n");
	    printf("         Propagation information will not be displayed\n");
            CLEAR_STATUS(&st);
	} else if (BAD_STATUS(&st)) {
	    sa_error_print(st);
            SET_EXIT_STATUS(st);
	    return st;
	} else {
	    printf("\n");
	    sa_display_full_replist(luuid, laddr, lstate, lprop, &st);
	    if (BAD_STATUS(&st)) {
		sa_error_print(st);
	    }
            SET_EXIT_STATUS(st);
	    return st;
	}
    }
    
    /* Only reached this point if lprop is false and status is ok */
    sa_refresh_replist(default_context, &st);
    if (BAD_STATUS(&st)) {
        sa_error_print(st);
    } else {
        printf("\n");
        sa_display_replist(luuid, laddr, lstate, &st);
        if (BAD_STATUS(&st)) {
            sa_error_print(st);
        }
    }

    SET_EXIT_STATUS(st);
    return st;
}


PUBLIC error_status_t tty_do_site
#ifndef __STDC__
    (c)
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    sec_rgy_handle_t	new_context;
    error_status_t      st, lst;
              
    CLEAR_STATUS(&st);
    switch(c->num_args) {
    case 1:
	break;
    case 2:
	if (match_command("-update", c->args[1], 2)) {
            fprintf(stderr, "-update must be preceeded by a name argument\n");
            cmd_usage("site");
            return st;
        }
        sec_rgy_site_open((unsigned_char_p_t)c->args[1],&new_context,&st);
        /* if it didn't work, then see if it's a name in the replist */
        if (BAD_STATUS(&st)) {
            sa_bind_to_rep_name((unsigned_char_p_t)c->args[1], &new_context,
                true, &lst);
            if (GOOD_STATUS(&lst)) {
                CLEAR_STATUS(&st);
            }
        }
        if (BAD_STATUS(&st)) {
            fprintf(stderr, "Cannot bind to specified site - ");
            sa_error_print(st);
        } else {
            sec_rgy_site_close(default_context, &lst);
            default_context = new_context;
        }
        break;
    case 3:
	if (! match_command("-update", c->args[2], 2)) {
	    fprintf(stderr, "Unknown option %s\n", c->args[2]);
            cmd_usage("site");
            return st;
        }
	sec_rgy_site_open_update((unsigned_char_p_t)c->args[1],
	    &new_context,&st);
	if (BAD_STATUS(&st)) {
	    fprintf(stderr, "Unable to bind to master registry - ");
	    sa_error_print(st);
	} else {
	    sec_rgy_site_close(default_context, &lst);
	    default_context = new_context;
	}
	break;
    default:
	fprintf(stderr, "Incorrect number of arguments\n");
	cmd_usage("site");
	return st;
    }

    if (GOOD_STATUS(&st)) {
	sa_print_def_rep_info(default_context, &st);
    }

    SET_EXIT_STATUS(st);
    return st;
}


/*
 * Turn a master into a slave or a slave into a master.
 * VERY dangerous command.  Must answer query before executing it,
 * to make sure they really want to do this.
 */
PUBLIC error_status_t tty_do_become
#ifndef __STDC__
    (c)
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    error_status_t      st;
    boolean32		master;
    rs_replica_info_t	info;
    unsigned_char_p_t   cname, sname;
                  
    CLEAR_STATUS(&st);
    switch(c->num_args) {
    case 2:
	if (match_command("-master", c->args[1], 7)) {
	    master = true;
	} else if (match_command("-slave", c->args[1], 6)) {
	    master = false;
	} else {
	    fprintf(stderr, "Unknown option %s\n", c->args[1]);
	    cmd_usage("become");
	    return st;
	}
	break;
    default:
	fprintf(stderr, "Incorrect number of arguments\n");
	cmd_usage("become");
	return st;
    }

    if (sa_ask_y_n()) {
        if (master) {
	    sec_rgy_site_binding_get_info(default_context, 
					  &cname, &sname, NULL,
					  NULL, &st);
	    if (BAD_STATUS(&st)) {
	        fprintf(stderr, "cannot bind to default replica\n");
                SET_EXIT_STATUS(st);
		return st;
	    }
	    sa_refresh_replist(default_context, &st);
	    if (BAD_STATUS(&st)) {
	        sa_error_print(st);
                SET_EXIT_STATUS(st);
		return st;
	    }
	    if ( sa_check_replica_seqno_hightest(&st) ) {
                sec_rgy_rep_admin_become_master(default_context, &st);
	    }
        } else {
            sec_rgy_rep_admin_become_slave(default_context, &st);
        }
        if (BAD_STATUS(&st)) {
            msg_status_print("Become operation failed", st);
        }
    }
    SET_EXIT_STATUS(st);
    return st;
}


/*
 * Change registry state between maintenance and service modes.
 */
PRIVATE void do_state_change
#ifndef __STDC__
    (in_maintenance, stp)
    boolean32       in_maintenance;
    error_status_t  *stp;
#else
  (
    boolean32       in_maintenance,
    error_status_t  *stp
  )
#endif
{
    if ( master_context == NULL ) {
        bind_to_master (&master_context, stp);             
        if (BAD_STATUS(stp)) {
            printf("%sCannot bind to master replica  ", saINDENT);
            sa_error_print(*stp);
            return;
        }
    }
        
    sec_rgy_rep_admin_maint(master_context, in_maintenance, stp);
    if (BAD_STATUS(stp)) {   
        if (STATUS_EQUAL(stp, sec_rgy_rep_not_master) ) {
            bind_to_master (&master_context, stp);             
            if (BAD_STATUS(stp))  {
                printf("%sCannot bind to master replica  ", saINDENT);
                sa_error_print(*stp);
                return;
            }
            sec_rgy_rep_admin_maint(master_context, in_maintenance, stp);
            if ( GOOD_STATUS(stp) ) return;  /* Fall thru if status bad */
        }
        if (in_maintenance)
            fprintf(stderr, "Unable to go into maintenance state  ");
        else
            fprintf(stderr, "Unable to go into service state  ");
        sa_error_print(*stp);
        return;
    }
}            


/*
 * state command
 */
PUBLIC error_status_t tty_do_state
#ifndef __STDC__
    (c)
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    error_status_t  st;

    CLEAR_STATUS(&st);

    if (c->num_args != 2) {
       fprintf(stderr, "Incorrect number of arguments\n");
       cmd_usage("state");
       return st;
    }

    if (match_command("-maintenance", c->args[1], 2)) {
       do_state_change(true, &st);
    }
    else
    if (match_command("-service", c->args[1], 2)) {
       do_state_change(false, &st);
    }
    else {
       printf("Unknown option %s\n", c->args[1]);
       cmd_usage("state");
       return st;
    }        

    SET_EXIT_STATUS(st);
    return st;
}                    


/*
 * master key command
 */
PUBLIC error_status_t tty_do_master_key
#ifndef __STDC__
    (c)
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    error_status_t      st;

    CLEAR_STATUS(&st);
    if (c->num_args > 1) {
        fprintf(stderr, "Incorrect number of arguments\n");
        cmd_usage("master_key");
        return(st);
    }
    sec_rgy_rep_admin_mkey(default_context, &st);
    if (BAD_STATUS(&st)) {
        fprintf(stderr, "Error changing master key \n ");
	sa_error_print(st);
    }
    SET_EXIT_STATUS(st);
    return(st);
}


/*
 * quit
 */
PUBLIC error_status_t tty_do_quit
#ifndef __STDC__
    ( c ) 
   command_buf *c;
#else
  (
   command_buf *c
  )
#endif
{
    printf("bye.\n");
    exit(tty_exit_status);   /* return the last failure status */
}


/*
 * Initialize pthreads
 */
void rgy_admin_pthread_init (
#ifdef __STDC__
    void
#endif
)
{
#ifdef CMA_INCLUDE
    sigset_t    sigset;
    pthread_t   dt;
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGINT);
    dt = pthread_self();
    pthread_signal_to_cancel_np(&sigset, &dt);
#endif
}


/*
 * init_global_state
 */
PRIVATE void init_global_state
#ifndef __STDC__
    (cell, nqflag, stp)
    char           *cell;
    boolean32      nqflag;
    error_status_t *stp;
#else
  (
    char           *cell,
    boolean32      nqflag,
    error_status_t *stp
  )
#endif
{
    /* Initialize the query flag based on whether -nq was specified */
    sa_query = (nqflag == false);

    /* Bind to an arbitrary site in the specified cell */
    sec_rgy_site_open((unsigned_char_p_t)cell, &default_context, stp);
    if (GOOD_STATUS(stp)) {
	sa_print_def_rep_info(default_context, stp);
    } else {
	fprintf(stderr, "Unable to bind to registry at %s\n", cell);
        sa_error_print(*stp);
    }
}


PRIVATE void sa_usage(
#ifdef __STDC__
    void
#endif
)
{
    printf("sec_admin [-u[sage] | -v[ersion] | [-s name] [-nq]]\n");
}


/*
 * MAIN
 */
int main
#ifndef __STDC__
    (argc, argv)
    int argc;
    char *argv[];
#else
  (
    int argc,
    char *argv[]
  )
#endif
{
    error_status_t      st;
    boolean32           nqflag = false;
    int                 i;
    char                *site = "/.:";
    
    setlocale(LC_ALL, "");

    dce_svc_set_progname("sec_admin", &st);
    dce_msg_define_msg_table(sad__table,
        sizeof sad__table / sizeof sad__table[0], &st);

    /*
     * Setup pthread state - make sure interrupts get mapped into cancels.
     */
    rgy_admin_pthread_init();

    for (i = 1; i < argc; i++) {
        if (match_command("-version", argv[i], 2)) {
            printf("sec_admin, version %s\n", rgy_admin_version_id);
            exit(0);
	} else if (match_command("-nq", argv[i], 3)) {
	    nqflag = true;
	} else if (match_command("-usage", argv[i], 2)) {
	    sa_usage();
            exit(0);
	} else if (match_command("-s", argv[i], 2)) {
            if ((i+1) < argc) {
                i++;
                site = argv[i];
	    } else {
                printf("-s requires name argument\n");
                sa_usage();
                exit(1);
            }
        } else {
            printf("Unknown argument %s\n", argv[i]);
            sa_usage();
            exit(1);
        }
    }

    init_global_state(site, nqflag, &st);
    EXIT_ON_ERROR(st);

    if (!isatty(0))
        must_prompt = false;

    set_commands(commands);
    procs_cmds(must_prompt, prompt_str);

    exit(tty_exit_status);      /* return the last failure status */
}
