/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sa_util.c,v $
 * Revision 1.1.8.2  1996/03/11  13:27:32  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:15:23  marty]
 *
 * Revision 1.1.8.1  1995/12/08  18:02:51  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/04/28  20:04 UTC  mdf
 * 	Resolve CHFts15068
 * 	[1995/04/27  13:03 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_3/1]
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  21:22 UTC  mullan_s
 * 	Merge.
 * 	[1995/12/08  16:30:55  root]
 * 
 * Revision 1.1.6.5  1994/09/09  20:41:03  kline_s
 * 	OT 12028: I18N code cleanup for Security.
 * 	[1994/09/09  20:24:02  kline_s]
 * 	HP revision /main/HPDCE01/2  1994/05/03  16:40 UTC  hanfei
 * 	merge.
 * 
 * 	HP revision /main/KK/hanfei_ch_master/1  1994/04/15  16:43 UTC  hanfei
 * 	merge change master work: more infomation printed if master unknown.
 * 
 * 	HP revision /main/HPDCE01/emartin_ch_master/1  1994/04/14  17:58 UTC  emartin
 * 	print message if master is not known.
 * 
 * 	HP revision /main/HPDCE01/1  1994/01/26  19:50  mdf
 * 	merge
 * 
 * Revision 1.1.4.3  1993/12/28  21:20:11  hanfei
 * 	add duplicat master in rgy_state
 * 	[1993/12/21  21:01:34  hanfei]
 * 
 * Revision 1.1.6.4  1994/06/17  18:41:29  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:08:12  devsrc]
 * 
 * Revision 1.1.6.3  1994/06/02  21:57:21  mdf
 * 	Merged with changes from 1.1.6.2
 * 	[1994/06/02  21:53:54  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:13:31  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.4.3  1993/12/28  21:20:11  hanfei
 * 	add duplicat master in rgy_state
 * 	[1993/12/21  21:01:34  hanfei]
 * 
 * Revision 1.1.4.2  1993/12/16  18:22:40  hanfei
 * 	add sa_util_seqno_cmp
 * 	[1993/12/15  21:49:55  hanfei]
 * 
 * Revision 1.1.2.3  1993/09/15  15:36:38  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.2.6  1993/03/09  18:32:59  burati
 * 	CR7258 Fixed some output column spacing to make it easier to read.
 * 	[1993/03/09  18:21:07  burati]
 * 
 * Revision 1.1.2.5  1993/03/08  17:49:42  burati
 * 	CR7299 More rebust checking on info/last update information,
 * 	and make the info look like the docs say it should look.
 * 	[1993/03/08  16:53:41  burati]
 * 
 * Revision 1.1.2.4  1993/03/02  22:31:07  burati
 * 	Display sw_rev instead of confusing people with compat_sw_rev number
 * 	[1993/03/02  22:13:37  burati]
 * 
 * Revision 1.1.2.3  1993/02/20  23:21:20  burati
 * 	Changed "renaming" state to "saving database"
 * 	Added sa_status_only() util rtn, to display status w/out err msg
 * 	[1993/02/19  23:55:57  burati]
 * 
 * Revision 1.1.2.2  1993/01/29  18:52:53  burati
 * 	Initial version
 * 	[1993/01/14  23:00:39  burati]
 * 
 * Revision 1.1.6.2  1994/06/02  20:20:38  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:13:31  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.4.3  1993/12/28  21:20:11  hanfei
 * 	add duplicat master in rgy_state
 * 	[1993/12/21  21:01:34  hanfei]
 * 
 * Revision 1.1.4.2  1993/12/16  18:22:40  hanfei
 * 	add sa_util_seqno_cmp
 * 	[1993/12/15  21:49:55  hanfei]
 * 
 * Revision 1.1.2.3  1993/09/15  15:36:38  root
 * 	    Loading drop DCE1_0_3b03
 * 
 * Revision 1.1.2.6  1993/03/09  18:32:59  burati
 * 	CR7258 Fixed some output column spacing to make it easier to read.
 * 	[1993/03/09  18:21:07  burati]
 * 
 * Revision 1.1.2.5  1993/03/08  17:49:42  burati
 * 	CR7299 More rebust checking on info/last update information,
 * 	and make the info look like the docs say it should look.
 * 	[1993/03/08  16:53:41  burati]
 * 
 * Revision 1.1.2.4  1993/03/02  22:31:07  burati
 * 	Display sw_rev instead of confusing people with compat_sw_rev number
 * 	[1993/03/02  22:13:37  burati]
 * 
 * Revision 1.1.2.3  1993/02/20  23:21:20  burati
 * 	Changed "renaming" state to "saving database"
 * 	Added sa_status_only() util rtn, to display status w/out err msg
 * 	[1993/02/19  23:55:57  burati]
 * 
 * Revision 1.1.2.2  1993/01/29  18:52:53  burati
 * 	Initial version
 * 	[1993/01/14  23:00:39  burati]
 * 
 * Revision 1.1.4.1  1993/10/05  22:29:50  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:03:40  mccann]
 * 
 * Revision 1.1.2.6  1993/03/09  18:32:59  burati
 * 	CR7258 Fixed some output column spacing to make it easier to read.
 * 	[1993/03/09  18:21:07  burati]
 * 
 * Revision 1.1.2.5  1993/03/08  17:49:42  burati
 * 	CR7299 More rebust checking on info/last update information,
 * 	and make the info look like the docs say it should look.
 * 	[1993/03/08  16:53:41  burati]
 * 
 * Revision 1.1.2.4  1993/03/02  22:31:07  burati
 * 	Display sw_rev instead of confusing people with compat_sw_rev number
 * 	[1993/03/02  22:13:37  burati]
 * 
 * Revision 1.1.2.3  1993/02/20  23:21:20  burati
 * 	Changed "renaming" state to "saving database"
 * 	Added sa_status_only() util rtn, to display status w/out err msg
 * 	[1993/02/19  23:55:57  burati]
 * 
 * Revision 1.1.2.2  1993/01/29  18:52:53  burati
 * 	Initial version
 * 	[1993/01/14  23:00:39  burati]
 * 
 * $EndLog$
 */
/*
 * Copyright (c) Hewlett-Packard Company 1992, 1993
 * Unpublished work. All Rights Reserved.
 */
#include <stdio.h>
#include <sa_util.h>
#include <stdlib.h>
#include <wchar.h>
#include <un_maloc.h>

/* Global state var from sec_admin.c */
extern boolean32 sa_query;

/* Private table of printable replica states */
static struct {
   char *state;
} rgy_state[] = {
   { "bad state" },
   { "unknown to master" },
   { "uninitialized" },
   { "initializing" },
   { "in service" },
   { "saving database" },
   { "copying database" },
   { "in maintenance" },
   { "changing master key" },
   { "becoming master" },
   { "closed" },
   { "deleted" },
   { "becoming slave" },
   { "duplicate master" }
};

PRIVATE char *cmd_use[] = {
   "become -master | -slave ",
   "change_master -to other_replica",
   "delrep other_replica [-f[orce]]",
   "destroy replica",
   "exit",
   "help [command]",
   "info [-f[ull]]",
   "initrep other_replica",
   "lrep [-state] [-uuid] [-addr] [-prop] [-all]", 
   "master_key",
   "monitor [-r interval_in_minutes]",
   "quit",
   "replrep other_replica -addr addr_string ...",
   "site [name [-u[pdate]]]",
   "state -maintenance | -service",
   "stop",
   "? [command]",
   NULL,
   NULL
};


PRIVATE void make_lower_case
#ifndef __STDC__
    (buf)
   char *buf;
#else
  (
   char *buf
  )
#endif
{
    wchar_t     *wcs_buf, *wcs_ptr;
    int         wcs_size;

    wcs_buf = NULL;

    /* Determine the size of wchar_t buffer needed to be allocated,
     * then malloc storage and convert to wchar_t for processing.
     */

    wcs_size = mbstowcs((wchar_t *) 0, buf, 0);
    wcs_buf = malloc((wcs_size + 1) * sizeof(wchar_t));
    mbstowcs(wcs_buf, buf, (wcs_size+1));

    /* Convert to lower case */
    wcs_ptr = wcs_buf;
    while (*wcs_ptr != (wchar_t) 0) {
         if (iswupper(*wcs_ptr)) 
             *wcs_ptr = towlower(*wcs_ptr);
         wcs_ptr++;
    }

    /* Convert back to multibyte string format & cleanup */
    wcstombs(buf, wcs_buf, (wcs_size+1));
    free(wcs_buf);
}


PUBLIC void cmd_usage
#ifndef __STDC__
    (cmd)
    char *cmd;
#else
  (
    char *cmd
  )
#endif
{
    char ignore_case[256];
    int i;
    extern command_table commands[];

    strncpy(ignore_case, cmd, sizeof(ignore_case));
    make_lower_case(ignore_case);

    for (i=0; commands[i].name != NULL; i++) {
        if (match_command(commands[i].name, ignore_case, commands[i].min)) {
            fprintf(stderr, "Usage: %s\n", cmd_use[i]);
            break;
        }
    }
}


PUBLIC error_status_t tty_help_stop
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    cmd_usage("stop");
    return error_status_ok;
}


PUBLIC error_status_t tty_help_mon
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    cmd_usage("monitor");
    return error_status_ok;
}


PUBLIC error_status_t tty_help_chm
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    cmd_usage("change_master");
    return error_status_ok;
}


PUBLIC error_status_t tty_help_info
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    cmd_usage("info");
    return error_status_ok;
}


PUBLIC error_status_t tty_help_initr
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    cmd_usage("initrep");
    return error_status_ok;
}


PUBLIC error_status_t tty_help_delr
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    cmd_usage("delrep");
    return error_status_ok;
}

PUBLIC error_status_t tty_help_replr ( command_buf *c)
{
    cmd_usage("replrep");
    return error_status_ok;
}

PUBLIC error_status_t tty_help_destroy
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    cmd_usage("destroy");
    return error_status_ok;
}


PUBLIC error_status_t tty_help_lrep
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    cmd_usage("lrep");
    return error_status_ok;
}


PUBLIC error_status_t tty_help_site
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    cmd_usage("site");
    return error_status_ok;
}


PUBLIC error_status_t tty_help_become
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    cmd_usage("become");
    return error_status_ok;
}


PUBLIC error_status_t tty_help_state
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    printf("sta[te] -m[aintenance] | -s[ervice]\n");
    printf("            -- Set the state of the registry.\n\n");
    printf("            -maintenance mode causes a checkpoint of the\n");
    printf("             database and prevents further updates.\n\n");
    printf("            -service mode allows updates\n");
    return error_status_ok;
}


PUBLIC error_status_t tty_help_mkey
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    cmd_usage("master_key");
    printf("master_key  -- Set the registry's master key.\n");
    return error_status_ok;
}


PUBLIC error_status_t tty_help_quit
#ifndef __STDC__
    ( c ) 
    command_buf *c;
#else
  (
    command_buf *c
  )
#endif
{
    cmd_usage("quit");
    return error_status_ok;
}


/*
 * Print status
 */
PRIVATE void status_print
#ifndef __STDC__
    (error, fp, status)
    boolean32      error;
    FILE           *fp;
    error_status_t status;
#else
  (
    boolean32      error,
    FILE           *fp,
    error_status_t status
  )
#endif
{
    dce_error_string_t  tmp_buff;
    int                 tmp_stat;
    char                *err;

    if (error) {
        err = "ERROR - ";
    } else {
        err = "";
    }
    
    dce_error_inq_text(status, tmp_buff, &tmp_stat);
    if (GOOD_STATUS(&tmp_stat)) {
        fprintf(fp, "%s%s\n", err, (char *)tmp_buff);
    } else {
        fprintf(fp, "%s%x\n", err, status);
    }
}


/*
 * Print status to stdout
 */
PUBLIC void sa_status_print
#ifndef __STDC__
    (status)
    error_status_t status;
#else
  (
    error_status_t status
  )
#endif
{
    status_print(true, stdout, status);
}


/*
 * Print status only to stdout
 */
PUBLIC void sa_status_only
#ifndef __STDC__
    (status)
    error_status_t status;
#else
  (
    error_status_t status
  )
#endif
{
    status_print(false, stdout, status);
}


/*
 * Print status to stderr
 */
PUBLIC void sa_error_print
#ifndef __STDC__
    (status)
    error_status_t status;
#else
  (
    error_status_t status
  )
#endif
{
    status_print(true, stderr, status);
}


/*
 * Print status
 */
PUBLIC void msg_status_print
#ifndef __STDC__
    (msg, status)
    char           *msg;
    error_status_t status;
#else
  (
    char           *msg,
    error_status_t status
  )
#endif
{
    dce_error_string_t  tmp_buff;
    int                 tmp_stat;

    dce_error_inq_text(status, tmp_buff, &tmp_stat);
    if (GOOD_STATUS(&tmp_stat)) {
        fprintf(stderr, "%s - %s\n", msg, (char *)tmp_buff);
    } else {
        fprintf(stderr, "%s - %x\n", msg, status);
    }
}


/*
 * Print default replica info
 */
PUBLIC void sa_print_def_rep_info
#ifndef __STDC__
    (ctxt, stp)
    sec_rgy_handle_t	ctxt;
    error_status_t      *stp;
#else
  (
    sec_rgy_handle_t    ctxt,
    error_status_t      *stp
  )
#endif
{ 
    unsigned_char_p_t   cname, sname;
    sec_rgy_site_binding_get_info(ctxt,&cname,&sname,NULL,NULL,stp);
    if (GOOD_STATUS(stp)) {
	printf("Default replica:  %s\n", sname);
	printf("Default cell:     %s\n", cname);
	free(cname);
	free(sname);
    } else {
	fprintf(stderr, "Cannot obtain site information - ");
	sa_error_print(*stp);
    }
}


/*
 * Print towers as string bindings
 */
PRIVATE void sa_print_towers
#ifndef __STDC__
    (title, twrs)
    char                   *title;
    rs_replica_twr_vec_p_t twrs;
#else
  (
    char                   *title,
    rs_replica_twr_vec_p_t twrs
  )
#endif
{
    int 		 i;
    error_status_t	 st;
    rpc_binding_handle_t binding;
    unsigned_char_p_t	 bstr;
    
    if (twrs) {
        for (i = 0; i < twrs->num_towers; i++) {
            rpc_tower_to_binding(twrs->towers[i]->tower_octet_string,
                &binding, &st);
            if (GOOD_STATUS(&st)) {
                rpc_binding_to_string_binding(binding, &bstr, &st);
                if (GOOD_STATUS(&st)) {
                    if (i == 0)
                        printf("%s%s%s\n", saINDENT, title, bstr);
                    else
                        printf("%s%s%s     %s\n", saINDENT, saINDENT,
                            saINDENT,bstr);
                    rpc_string_free(&bstr, &st);
                }
                rpc_binding_free(&binding, &st);
            }
        }
    }
}


/*
 * Check to see if rep is in a state where info is meaningful
 */
PRIVATE boolean32 sa_util_state_ok
#ifndef __STDC__
    (state)
    unsigned32 state;
#else
  (
    unsigned32 state
  )
#endif
{
    switch (state) {
        case rs_c_state_in_service:
        case rs_c_state_renaming:
        case rs_c_state_copying_dbase:
        case rs_c_state_in_maintenance:
        case rs_c_state_mkey_changing:
        case rs_c_state_becoming_master:
        case rs_c_state_becoming_slave:
            return true;
        default:
            return false;
    }
}


/*
 * Print site info
 */
PUBLIC void sa_print_site_info
#ifndef __STDC__
    (infoP, display_update_time, full, rtwrs, mtwrs, stp)
    rs_replica_info_t   	*infoP;
    boolean32         		display_update_time;
    boolean32          		full;
    rs_replica_twr_vec_p_t      rtwrs;
    rs_replica_twr_vec_p_t      mtwrs;
    error_status_t      	*stp;
#else
  (
    rs_replica_info_t   	*infoP,
    boolean32	      		display_update_time,
    boolean32	        	full,
    rs_replica_twr_vec_p_t      rtwrs,
    rs_replica_twr_vec_p_t      mtwrs,
    error_status_t     		*stp
  )
#endif
{ 
    char		*update;
    unsigned_char_p_t	id_str;
    error_status_t	lst;
    
    if (infoP->master) {
	printf("%sState:         %s%s - master\n", saINDENT, saINDENT,
	    rgy_state[infoP->rep_state]);
    } else {
	printf("%sState:         %s%s - slave\n", saINDENT, saINDENT,
	    rgy_state[infoP->rep_state]);
    }
    if (display_update_time && sa_util_state_ok(infoP->rep_state)) {
	update = ctime(&infoP->last_upd_ts.sec);
	printf("%sLast update received at: %s", saINDENT, update);
	printf("%sLast update's seqno:     %d.%d\n", saINDENT,
	    infoP->last_upd_seqno.high, infoP->last_upd_seqno.low);
    }
    if (full) {
	/* Print information about the replica itself */    
	uuid_to_string(&infoP->rep_id, &id_str, &lst);
	if (GOOD_STATUS(&lst)) {
	    printf("%sInstance id:   %s%s\n", saINDENT, saINDENT, id_str);
	    free(id_str);
	} else {
	    fflush(stdout);
	    fprintf(stderr, "%sUnable to display Instance id: ", saINDENT);
	    sa_error_print(lst);
	}
        sa_print_towers("Addresses:               ", rtwrs);

	if ( infoP->master_known ) {
	    /* Print information about the master replica */
            uuid_to_string(&infoP->master_id, &id_str, &lst);
	    if (GOOD_STATUS(&lst)) {
	        printf("%sMaster id:     %s%s\n", saINDENT, saINDENT, id_str);
	        free(id_str);
	    } else {
	        fflush(stdout);
	        fprintf(stderr, "%sUnable to display Master id: ", saINDENT);
	        sa_error_print(lst);
	    }
            sa_print_towers("Master address:          ", mtwrs);
            printf("%sMaster seqno:  %s%d.%d\n", saINDENT, saINDENT,
            infoP->master_seqno.high, infoP->master_seqno.low);
        }
	else {
	    printf("%sMaster:        %sunknown\n", saINDENT, saINDENT);
	}

	printf("%sSoftware version:        %s\n", saINDENT,
	    infoP->sw_rev);
        if (infoP->master) {
            printf("%sPropagation queue seqnos: %d.%d - %d.%d\n", saINDENT,
                infoP->base_propq_seqno.high, infoP->base_propq_seqno.low,
                infoP->last_upd_seqno.high, infoP->last_upd_seqno.low);
        }
    }
}


/*
 * sa_ask_y_n
 */
boolean32 sa_ask_y_n(
#ifdef __STDC__
    void
#endif
)
{
    char      ch, response[4];
    int	      len;
    boolean32 gotone = false, done = false, ask = false;
    if (! sa_query)
	return true;
    do {
	fprintf(stdout,
          "Do you wish to continue (y[es]) or abort this operation (n[o])? ");
	fflush(stdout);
	len = 0; memset(response, 0, 4);
	do {
	    ch = getchar();
	    gotone = (ch == EOF || ch == '\n');
	    if (!gotone)
		if (len < 4) 
                    response[len] = ch;
	    len++;
	} while (! gotone);
	if (len < 5) {
	    make_lower_case(response);
	    if ((strcmp(response,"yes") == 0) || (strcmp(response,"y")==0)) {
		done = true;
		ask = true;
	    }
	    else if ((strcmp(response,"no")==0)||(strcmp(response,"n")==0)) {
		done = true;
	    }
	}
	if (! done) {
	    printf("Invalid response\n");
	}
    } while (!done);
    return ask;
}


/*
 *  Compare 2 sequence numbers.
 *  Return 0 if they are equal.
 *  Return 1 if seqno1 > seqno2
 *  Return -1 if seqno1 < seqno2 
 */
PUBLIC signed32 sa_util_seqno_cmp
#ifndef __STDC__
    (seqno1, seqno2)
    rs_update_seqno_t   *seqno1;
    rs_update_seqno_t   *seqno2;
#else
  (
    rs_update_seqno_t   *seqno1,
    rs_update_seqno_t   *seqno2
  )
#endif
{
    signed32    retval;

    if (seqno1->high == seqno2->high) {
        if (seqno1->low == seqno2->low) {
            retval = 0;
        }
        else {
            retval = (seqno1->low > seqno2->low) ? 1:-1;
        }
    }
    else {
        retval = (seqno1->high > seqno2->high) ? 1:-1;
    }

    return retval;
}
