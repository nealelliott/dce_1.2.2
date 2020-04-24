/* ______________________________________________________________________ */
/*
 * @OSF_COPYRIGHT@
 * Copyright (c) 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dce_server_sec.c,v $
 * Revision 1.1.8.3  1996/02/18  19:15:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:01:35  marty]
 *
 * Revision 1.1.8.2  1995/12/08  21:25:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:58:41  root]
 * 
 * Revision 1.1.4.1  1994/10/25  21:49:55  max
 * 	CR 12568 general fixes
 * 	[1994/10/25  21:49:32  max]
 * 
 * Revision 1.1.2.5  1994/10/06  15:43:06  bowe
 * 	In dce_server_sec_begin() call sec_login_set_context() [CR 12532]
 * 	and call rpc_server_register_auth_info() [CR 12533]
 * 	[1994/10/06  15:42:44  bowe]
 * 
 * Revision 1.1.2.4  1994/10/05  20:46:49  bowe
 * 	In dce_server_sec_begin(), a bad handle was passed to
 * 	sec_login_validate_identity(). [CR 12527]
 * 	[1994/10/05  20:46:28  bowe]
 * 
 * Revision 1.1.2.3  1994/10/05  19:21:19  bowe
 * 	In dce_server_sec_begin(), bad logic - extraneous '!'. [CR 12520]
 * 	[1994/10/05  19:20:55  bowe]
 * 
 * Revision 1.1.2.2  1994/08/22  19:32:56  bowe
 * 	Change signature of dce_server_sec_xxx() [CR 11799]
 * 	[1994/08/22  19:32:29  bowe]
 * 
 * Revision 1.1.2.1  1994/08/20  18:05:02  bowe
 * 	Initial implemention of conv functions [CR 11791]
 * 	[1994/08/20  17:57:56  bowe]
 * 
 * $EndLog$
 */
/* ______________________________________________________________________ */

#include <dce/dce.h>
#include <dce/dce_utils.h>
#include <dce/dced.h>
#include <dce/dced_util.h>
#include <dce/keymgmt.h>
#include <dce/idl_es.h>
#include <dced_convert.h>
#include <dce/sec_login.h>
#include <pthread.h>

/* State for dce_server_sec_begin/end */
typedef struct dce_server_sec_state_s_t {
    unsigned32			flags;
    sec_login_handle_t		context;
    pthread_t			km_thread;
} dce_server_sec_state_t;
static dce_server_sec_state_t	dce_srvr_state;

/* local prototypes */
static void key_manager(void *);

/* ______________________________________________________________________ */

typedef struct key_mgr_data_s_t {
    char	*keyfile;
    idl_char	*prin;
} key_mgr_data_t;

static void
key_manager(void *arg)
{
    error_status_t	st;
    key_mgr_data_t	*kmd;
    char		*keyfile;
    idl_char		*prin;

    kmd = (key_mgr_data_t *) arg;
    keyfile = dce_strdup(kmd->keyfile);
    prin    = (idl_char *)dce_strdup((char *)kmd->prin);
    sec_key_mgmt_manage_key(rpc_c_authn_dce_secret, keyfile, prin, &st);
    /* sec_key_mgmt_manage_key() should never return.
     * But how to report an error from this thread?  (We can't.)
     */
}

/* ______________________________________________________________________ */

#define SRVR_DEC_BUF_SIZE	128

typedef struct server_decode_state_s_t {
    idl_byte	*buf;
    FILE	*fp;
} server_decode_state_t;

static void
srvr_file_read(
    idl_void_p_t	param,
    idl_byte		**buf,
    idl_ulong_int	*size
)
{
    server_decode_state_t	*state;
    state = (server_decode_state_t *)param;
    *size = fread(state->buf, 1, SRVR_DEC_BUF_SIZE, state->fp);
    *buf = state->buf;
}

/* ______________________________________________________________________ */
/* Retrieve the server_t structure from the pickled file.
 * The file name is passed with an env variable.
 */

void
dce_server_inq_server(
    server_t		**s,
    error_status_t	*st
)
{
    char			*name;
    server_decode_state_t	state;
    idl_es_handle_t		enc_h;
    error_status_t		st2;
    idl_byte			enc_buffer[SRVR_DEC_BUF_SIZE+8];
    static server_t		this_server;
    static boolean		got_server=FALSE;

    *st = error_status_ok;
    if (got_server) {
	*s = &this_server;
	return;
    }

    /* get info about this server */
    if ((name=getenv(dced_conf_env_name)) == NULL) {
	*st = dced_s_not_started_by_dced;
	return;
    }

    /* decode that file */
    if ((state.fp = fopen(name, "r")) == NULL) {
	*st = dced_s_data_unavailable;
	return;
    }
    state.buf = (idl_byte *)(((enc_buffer - (idl_byte *)0) + 7) & ~7);
    idl_es_decode_incremental((idl_void_p_t)&state, srvr_file_read, &enc_h, st);
    if (*st != error_status_ok) {
	idl_es_handle_free(&enc_h, &st2);
	return;
    }
    dced_server_env_decode(enc_h, &this_server, st);
    if (*st != error_status_ok) {
	idl_es_handle_free(&enc_h, &st2);
	return;
    }

    *s = &this_server;
    got_server=TRUE;

    fclose(state.fp);
    idl_es_handle_free(&enc_h, st);
}

/* ______________________________________________________________________ */

/* Setup server's security params. */

void
dce_server_sec_begin(
  /* in */
    unsigned32		flags,
  /* out */
    error_status_t	*st
)
{
    char			*keyfile;
    unsigned_char_p_t		prin;
    server_t			*this_server;
    sec_passwd_rec_t		*pwd;
    boolean32			reset;
    sec_login_auth_src_t	auth_src;
    key_mgr_data_t		kmd;

    *st = error_status_ok;

    /* get info about this server */
    dce_server_inq_server(&this_server, st);
    if (*st != error_status_ok)
	return;

    /* register auth info for this server's prin(s) */
    if (this_server->prin_names.count != 0) {
	prin = this_server->prin_names.list[0];
    }
    else {
	/* Error if login or manage-key are specified and print is null. */
	*st = dced_s_need_one_server_prin;
	return;
    }

    /* get password from keyfile */
    if ((keyfile=getenv(dced_keytab_env_name)) == NULL) {
	*st = dced_s_not_started_by_dced;
	return;
    }
    if (! *keyfile) {
	*st = dced_s_no_server_keyfile;
	return;
    }

    dce_srvr_state.flags = flags;
    if (flags & dce_server_c_login) {
	/* Get this server prin's key from keytab. */
	sec_key_mgmt_get_key(rpc_c_authn_dce_secret, keyfile, prin, 0,
	    (void **)&pwd, st);
	if (*st != error_status_ok) return;

	/* set server's identity */
	/* XXX - what flags to pass to setup? */
	/* XXX - returns bool - different than the returned status? */
	sec_login_setup_identity(prin, sec_login_no_flags,
		&dce_srvr_state.context, st);
	if (*st != error_status_ok) return;
	/* XXX - want to check if this account expired? */

	/* XXX - returns bool - different than the returned status? */
	sec_login_validate_identity(dce_srvr_state.context, pwd, &reset,
		&auth_src, st);
	if (*st != error_status_ok) return;
	sec_key_mgmt_free_key(pwd, st);
	sec_login_set_context(dce_srvr_state.context, st);
	if (*st != error_status_ok) return;
    }
    rpc_server_register_auth_info((idl_char *)prin, rpc_c_authn_dce_secret,
	NULL, keyfile, st);
    if (*st != error_status_ok) return;

    /* creat thread to manage key */
    if (flags & dce_server_c_manage_key) {
	/* kmd struct used for key mgmt thread */
	kmd.keyfile = keyfile;
	kmd.prin    = prin;
	if (pthread_create(&dce_srvr_state.km_thread, pthread_attr_default,
		(pthread_startroutine_t)key_manager, &kmd) != 0) {
	    *st = dced_s_cannot_create_key_mgmt_thr;
	    return;
	}
	if (pthread_detach(&dce_srvr_state.km_thread) != 0) {
	    *st = dced_s_cannot_detach_key_mgmt_thr;
	    return;
	}
    }
}

/* ______________________________________________________________________ */

/* Cleanup server's security params. */

void
dce_server_sec_done(
  /* out */
    error_status_t	*st
)
{
    *st = error_status_ok;

    /* get rid of this login context */
    if (dce_srvr_state.flags & dce_server_c_login) {
	sec_login_release_context(&dce_srvr_state.context, st);
	if (*st != error_status_ok) return;
    }

    /* kill key management thread */
    if (dce_srvr_state.flags & dce_server_c_manage_key) {
	pthread_cancel(dce_srvr_state.km_thread);
    }

    /* garbage collect the key */

    return;
}

/* ______________________________________________________________________ */
