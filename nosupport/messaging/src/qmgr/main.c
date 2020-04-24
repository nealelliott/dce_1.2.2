/* ______________________________________________________________________
 * Copyright (c) 1996 The Open Group, Inc., Cambridge, MA USA
 * All rights reserved.
 * ______________________________________________________________________
 *
 * Queue manager.  Main.
 *______________________________________________________________________
 *
 * $Log: main.c,v $
 * Revision 1.3  1996/10/17  15:14:16  bowe
 * Added settable qmgr_object for obj uuid that is exported.
 *
 * Revision 1.2  1996/10/16  16:01:51  bowe
 * Add variables start_time, create_count, enqueue_count, dequeue_count.
 *
 * Revision 1.1  1996/09/27  18:04:28  bowe
 * Initial revision
 *
 * $EndLog$
 * ______________________________________________________________________
 */

#include "mosdefs.h"

#include <dce/dce_msg.h>
#include <dce/dce_svc.h>

#include <stdarg.h>
#include <errno.h>

/* ______________________________________________________________________ */

dce_svc_handle_t	mos_svc_handle;

static void set_defaults_and_parse_args(int, char **, error_status_t *);

unsigned_char_t	*qmgr_keyfile = NULL;
char		*qmgr_cell;
uuid_t		 qmgr_cell_uuid;
idl_char	*qmgr_prin = NULL;
uuid_t		 qmgr_prin_uuid;
utc_t		 start_time;
unsigned32	 create_count=0, enqueue_count=0, dequeue_count=0;
char		*qmgr_object = NULL;
uuid_t		 qmgr_object_uuid;

unsigned_char_t	*ns_entry = NULL;

static boolean	initialize=FALSE;
boolean	skip_authz=FALSE;		/* XXX - DEBUG */

static pthread_mutex_t	qattr_lock;

static uuid_t def_qmgr_obj_uuid = { /* a855638c-f49c-11cf-bf95-0800092474cc */
 0xa855638c, 0xf49c, 0x11cf, 0xbf, 0x95, {0x08, 0x00, 0x09, 0x24, 0x74, 0xcc}};

/* ______________________________________________________________________ */

static void
set_defaults_and_parse_args(int ac, char **av, error_status_t *st)
{
    int				c;
    sec_rgy_bind_auth_info_t	auth;
    sec_rgy_handle_t		rh;
    sec_rgy_name_t		prin_in_rgy;
    error_status_t		lst;
    char			objbuf[200];

    /* -------- handle args -------- */

    while ((c = getopt(ac, av, "p:e:w:k:o:ihA")) != EOF) {
	switch (c) {
	default:						break;
	case 'p': qmgr_prin = (idl_char *)optarg;		break;
	case 'e': ns_entry = (idl_char *)optarg;		break;
	case 'k': qmgr_keyfile = (unsigned_char_t *)optarg;	break;
	case 'o': qmgr_object = optarg;				break;
	case 'i': initialize = TRUE;				break;
	case 'A': skip_authz = TRUE;				break;	/*XXX*/
	case 'h': dce_svc_printf(MOS_S_USAGE_MSG);		break;
	case 'w':
	    dce_svc_routing((idl_char *)optarg, st);
	    if (*st != error_status_ok) {
		dce_pgm_fprintf(stderr, *st);
		exit(1);
	    }
	    break;
	}
    }

    /* If not specified on cmd line, try env variables */
    if ( qmgr_keyfile == NULL )
	qmgr_keyfile = (idl_char *)getenv(ENV_KEYFILE);
    if ( qmgr_prin == NULL )
	qmgr_prin = (idl_char *)getenv(ENV_PRINCIPAL);
    if ( ns_entry == NULL )
	ns_entry = (idl_char *)getenv(ENV_NS_ENTRY);
    if ( qmgr_object == NULL )
	qmgr_object = getenv(ENV_OBJECT);

    /* -------- obtail valies from DCED -------- */

    /* -------- initialize global data -------- */

    auth.info_type = sec_rgy_bind_auth_none;
    sec_rgy_site_bind(NULL, &auth, &rh, st);
    if (*st != error_status_ok)
	return;

    /* this qmgr's principal UUID */
/*#if 0*/
    strcpy((char *)prin_in_rgy, (char *)qmgr_prin);
    sec_rgy_pgo_name_to_id(rh, sec_rgy_domain_person, prin_in_rgy,
			   &qmgr_prin_uuid, st);
    if (*st != error_status_ok) {
	sec_rgy_site_close(rh, &lst);
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		       "sec_rgy_pgo_name_to_id", (long)*st);
	return;
    }
/*#endif*/

    /* this cell UUID */
    dce_cf_get_cell_name(&qmgr_cell, st);
    if (*st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		       "dce_cf_get_cell_name", (long)*st);

    sec_id_parse_name(rh, (idl_char *)qmgr_cell, NULL,
		      &qmgr_cell_uuid, NULL, NULL, st);
    if (*st != error_status_ok) {
	sec_rgy_site_close(rh, &lst);
	return;
    }

    sec_rgy_site_close(rh, st);

    (void)utc_gettime(&start_time);

    if (!qmgr_object) {
	/* Get this from private data/config file */
	qmgr_object = value_from_config_file(PRIV_KEY_OBJECT, objbuf);
    }
    if (qmgr_object) {
	uuid_from_string((unsigned_char_t *)qmgr_object, &qmgr_object_uuid, st);
	if (*st != error_status_ok) {
	    dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		   "uuid_from_string", (long)*st);
	}
    }
    else
	qmgr_object_uuid = def_qmgr_obj_uuid;
}

/* ______________________________________________________________________ */

int
main(int ac, char **av)
{
    error_status_t	st;
    char		*path;
    pthread_t		tid;
    extern void		*maint_thread(void *);

    path = av[0];

    dce_svc_set_progname(path, &st);
    mos_svc_handle = dce_svc_register(mos_svc_table, (idl_char *)"mos", &st);
    dce_msg_define_msg_table(mos_svc_g_table,
	sizeof mos_svc_g_table / sizeof mos_svc_g_table[0], &st);

    set_defaults_and_parse_args(ac, av, &st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG,
		       "set_defaults_and_parse_args", (long)st);

    if (initialize) {
	open_databases(TRUE);
	dce_login(FALSE);
	initialize_acls();
	close_databases();
	return 0;
    }

/* XXX - check if we're started by dced somewhere around here... */

    if (qmgr_prin == NULL)
	dce_svc_printf(MOS_S_NO_SERVER_PRIN_MSG);
    if (qmgr_keyfile == NULL)
	dce_svc_printf(MOS_S_NO_SERVER_KEYTAB_MSG);
    if (ns_entry == NULL)
	dce_svc_printf(MOS_S_NO_EXPORT_LOCATION_MSG);

    open_databases(FALSE);
    read_queues(&st);
    if (st != error_status_ok)
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_MSG, "read_queues", (long)st);
    mos_register_acls();

    dce_login(TRUE);
    register_server();

    /* Thread to do maint, check timeouts, etc. */
    if (pthread_create(&tid, pthread_attr_default, maint_thread, NULL) < 0) {
	dce_svc_printf(MOS_S_INIT_FUNCTION_FAIL_ER_MSG,
			       "pthread_create", (long)errno);
	return errno;
    }
    pthread_detach(&tid);

fprintf(stderr,"+++ server %s ready.\n", ns_entry);
    rpc_server_listen(rpc_c_listen_max_calls_default, &st);
    unregister_server();

    sec_login_release_context(&login_ctx, &st);
    close_databases();

    return 0;
}

/* ______________________________________________________________________ */

