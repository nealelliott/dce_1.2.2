/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sv_preauth.c,v $
 * Revision 1.1.8.3  1996/02/18  19:16:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:02:47  marty]
 *
 * Revision 1.1.8.2  1995/12/08  20:54:07  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/09/29  22:39 UTC  tatsu_s
 * 	Submitted the fix for CHFts16512.
 * 
 * 	HP revision /main/HPDCE02/tatsu_s.dced_local.b0/1  1995/09/19  15:31 UTC  tatsu_s
 * 	Initial revision: dced splitting (libdcedpvt).
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/24  19:28 UTC  mullan_s
 * 	Merge
 * 
 * 	HP revision /main/mullan_mothra_bin_compat10/1  1995/05/23  19:01 UTC  mullan_s
 * 	Include sec_login_util.h instead of dce/sec_login_util.h
 * 	because it is no longer exported.
 * 	[1995/12/08  20:49:19  root]
 * 
 * Revision 1.1.4.1  1994/10/25  17:18:33  mob
 * 	Fix for OT 12545
 * 	[1994/10/24  20:36:21  mob]
 * 
 * Revision 1.1.2.8  1994/10/17  19:31:07  sommerfeld
 * 	OT11548: fix hole in local preauth protocol
 * 	[1994/10/17  19:29:27  sommerfeld]
 * 
 * Revision 1.1.2.7  1994/09/22  22:00:37  bowe
 * 	SVC work. Plus, removed unused #includes. [CR 11725]
 * 	[1994/09/22  21:43:10  bowe]
 * 
 * Revision 1.1.2.6  1994/08/15  21:18:27  bowe
 * 	Use localpath. [CR 11656]
 * 	[1994/08/15  21:18:10  bowe]
 * 
 * Revision 1.1.2.5  1994/07/18  18:53:21  rsalz
 * 	Fix third-party pre-authentication (OT CR 11327).
 * 	[1994/07/18  18:53:09  rsalz]
 * 
 * Revision 1.1.2.4  1994/06/13  20:05:39  rsalz
 * 	Code cleanup.
 * 	Server registration done in main now.
 * 	[1994/06/13  20:04:57  rsalz]
 * 
 * Revision 1.1.2.3  1994/06/08  15:19:06  pwang
 * 	Included dcedimpl.h to have debug variable.
 * 	[1994/06/08  15:17:57  pwang]
 * 
 * Revision 1.1.2.2  1994/06/08  11:59:22  bowe
 * 	Put fprintf() inside "if (debug)"
 * 	[1994/06/08  11:59:04  bowe]
 * 
 * Revision 1.1.2.1  1994/06/06  19:44:58  bowe
 * 	Code for rsec_login functionality, stolen from sec_clientd. [CR 9768]
 * 	[1994/06/06  19:44:35  bowe]
 * 
 * $EndLog$
 */

/*
**
** Copyright (c) Hewlett-Packard Company 1991
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */

/* NOTE:
 *	This is in a separate file from sv_clientd.c because if needs
 *	different compiler flags: -DOSF_DCE (for the krb parts)
 */

#if	!defined(_BSD_SOURCE)
#define _BSD_SOURCE
#endif	/* !defined(_BSD_SOURCE) */

#include <dce/dce.h>
#include <pthread.h>
#include <stdio.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <dce/dce.h>
#include <dce/dce_cf.h>
#include <dce/sec_authn.h>
#include <dce/sec_base.h>
#include <dce/sec_login.h>
#include <dce/sec_login_file_loc.h>
#include <rsec_login.h>
#include <sec_login_util.h>

#include <sec_krb_util.h>

#include <krb5/kdb.h>
#include <krb5/krb5.h>
#include <krb5/asn1.h>

#include <dcedimpl.h>
#include <svimpl.h>

/*
**  Temporary files used in pre-authenticated login are created by us,
**  and deleted by us so that we can use a protected directory and so
**  we needn't rely on the client.  The files live for SEC_LOGIN_TMP_LIFE
**  seconds (plus optional slop).  The list of files to be deleted is
**  maintained on a deletion queue which is a FIFO singly-linked list that
**  is mutex-protected.  A deleter thread pulls things off the front of
**  the list when it's time for them to go.
*/
#define PERMISSIONS(_M)		((_M) & (S_IRWXU | S_IRWXG | S_IRWXO))
#define WANTED_PERMISSIONS	(S_IRUSR|S_IWUSR|S_IXUSR | S_IXGRP | S_IXOTH)
#define SEC_LOGIN_TMP_LIFE	120
#define SEC_LOGIN_TMP_SLOP	10

typedef struct dq_node_s_t {
    struct dq_node_s_t	*next;
    char		*name;
    time_t		when;
} dq_node_t;

typedef struct dq_s_t {
    dq_node_t		*head;
    dq_node_t		*tail;
    pthread_mutex_t	m;
    pthread_cond_t	cv;
} dq_t;

static dq_t	dq;


static void
queue_deletion(
    char		*name
)
{
    dq_node_t		*new;
    int			cancelmode;

    /* Create a new node. */
    new = malloc(sizeof *new);
    new->name = dce_strdup(name);
    new->next = NULL;
    time(&new->when);
    new->when += SEC_LOGIN_TMP_LIFE;

    cancelmode = pthread_setcancel(CANCEL_OFF);
    pthread_mutex_lock(&dq.m);

    /* Add node to list; waking up deleter if list was empty. */
    if (dq.tail == NULL) {
	dce_assert(dhd_svc_handle, dq.head == NULL);
	dq.tail = dq.head = new;
	pthread_cond_signal(&dq.cv);
    }
    else {
	dq.tail->next = new;
	dq.tail = new;
    }

    pthread_mutex_unlock(&dq.m);
    if (cancelmode == CANCEL_ON)
	pthread_setcancel(CANCEL_ON);
}


/*
**  Thread to delete files from the deletion queue.
*/
static void *
file_deleter(
    void		*dummy
)
{
    static char		buff[512];
    time_t		now;
    struct stat		st;
    struct timespec	ts;
    dq_node_t		*t;
    int			fd;

    pthread_mutex_lock(&dq.m);
    pthread_setcancel(CANCEL_OFF);

    for ( ; ; ) {
	pthread_mutex_unlock(&dq.m);
	pthread_testcancel();
	pthread_mutex_lock(&dq.m);

	time(&now);
	while (dq.head) {
	    t = dq.head;
	    if (t->when > now)
		break;

	    /* Since the file contains an encryption key, make best effort
	     * to overwrite file with zeros before deleting in an attempt
	     * to improve our chances of "forward secrecy". */
	    if ((fd = open(t->name, O_WRONLY, 0)) >= 0) {
		fstat(fd, &st);
		if (st.st_size > sizeof buff)
		    st.st_size = sizeof buff;
		write(fd, buff, st.st_size);
		fsync(fd);
		close(fd);
	    }
	    unlink(t->name);

	    if (t == dq.tail)
		dq.head = dq.tail = NULL;
	    else
		dq.head = t->next;

	    free(t->name);
	    free(t);
	}
	if (dq.head) {
	    /* Allow some slop here, so we delete a batch of files
	     * at once instead of a single file. */
	    t = dq.head;
	    ts.tv_sec = t->when + SEC_LOGIN_TMP_SLOP;
	    ts.tv_nsec = 0;
	    pthread_cond_timedwait(&dq.cv, &dq.m, &ts);
	}
	else
	    /* Wait for something to do */
	    pthread_cond_wait(&dq.cv, &dq.m);
    }
    return NULL;
}

/* ______________________________________________________________________ */

/*
**  This routine gets the trusted portions of the preauthentication data
**  required for the type KRB5_PADATA_ENC_THIRD_PARTY.  It securely passes a
**  random key to the caller via a file.
*/
void
rsec_login_get_trusted_preauth(
  /* in */
    handle_t                    handle,
    idl_char                    *client,
    unsigned32			caller_uid,
  /* out */
    idl_char                    **tfilep,
    encoded_data_t              **machine_tgt,
    encoded_data_t              **ebuf,
    error_status_t              *st
)
{
    static char			CHR[65] =
    "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789-_";
    extern int			krb5_kdc_default_options;
    char			cache_name[SEC_LOGIN_PAG_NAME_LENGTH];
    krb5_ccache			kcache;
    krb5_creds			creds;
    krb5_keyblock		*rndkey;
    krb5_encrypt_block		eblock;
    krb5_data			*rkey_data;
    krb5_data			*edata_data;
    krb5_enc_data		edata;
    int				byte_size;
    char			*user_cell;
    char			*machine_cell;
    krb5_principal		client_princ;
    sec_des_block		fnr;
    FILE			*F;
    char			*tfile;
    int				tfd;
    int				c[3];

    DCE_SVC_DEBUG((dhd_svc_handle, dhd_s_secval, svc_c_debug1,
	"ENTER rsec_login_get_trusted_preauth"));
    *machine_tgt = NULL;
    *ebuf = NULL;
    *st = error_status_ok;
    *tfilep = NULL;

    /* Allocate a cache handle for cache name. */
    sprintf(cache_name, SEC_LOGIN_CCACHE_FROM_PAG_FMT_LOCAL,
	dcelocal_path, SEC_LOGIN_NO_PAG);
    *st = krb5_cc_resolve(cache_name, &kcache);
    if (*st != error_status_ok)
        return;

    /* Get machine principal name. */
    memset(&creds, 0, sizeof creds);
    *st = krb5_cc_get_principal(kcache, &creds.client);
    if (*st != error_status_ok) {
        krb5_cc_close(kcache);
        return;
    }

    /* Get user's cell name. */
    *st = krb5_parse_name((char *)client, &client_princ);
    if (*st != error_status_ok) {
        krb5_cc_close(kcache);
        krb5_free_cred_contents(&creds);
    }
    sec_dce_pair_from_krb_princ(client_princ, &user_cell, NULL, st);
    krb5_free_principal(client_princ);
    if (*st != error_status_ok) {
        krb5_cc_close(kcache);
        krb5_free_cred_contents(&creds);
        return;
    }

    /* Get machine principal's cell name. */
    dce_cf_get_cell_name(&machine_cell, st);
    if (*st != error_status_ok) {
        krb5_cc_close(kcache);
        krb5_free_cred_contents(&creds);
        if (user_cell)
            free(user_cell);
        return;
    }

    /* Now parse it into a krb5_principal. */
    sec_krb_tgs_parsed_name_from_dce_pair(machine_cell, user_cell, &creds.server, st);
    if (machine_cell)
        free(machine_cell);
    if (user_cell)
        free(user_cell);
    if (*st != error_status_ok) {
        krb5_cc_close(kcache);
        krb5_free_cred_contents(&creds);
        return;
    }

    /* Acquire machine TGT. */
    *st = krb5_get_credentials(krb5_kdc_default_options, kcache, &creds);
    krb5_cc_close(kcache);
    if (*st != error_status_ok) {
        krb5_free_cred_contents(&creds);
        return;
    }

    /* Generate random key. */
    *st = krb5_generate_subkey(&creds.keyblock, &rndkey);
    if (*st != error_status_ok) {
        krb5_free_cred_contents(&creds);
        return;
    }

    /* Encode random key. */
    *st = encode_krb5_keyblock(rndkey, &rkey_data);
    if (*st != error_status_ok) {
        krb5_free_cred_contents(&creds);
        krb5_free_keyblock(rndkey);
        return;
    }

    /* Create a temp file.  Don't detect collsions since the directory is
     * emptied at startup and DES is a bijection over [0..2**64-1] so the
     * only way to collide is to wrap-around.  This code will be rewritten long
     * before we have a system fast enough to do 2**64 DCE logins in
     * under three minutes. */
    if ((tfile =
	 dced_copy_for_stub(strlen(sv_preauth_dir) + 1 + 12 + 1)) == NULL) {
        krb5_free_cred_contents(&creds);
        krb5_free_keyblock(rndkey);
        *st = sec_s_no_memory;
        return;
    }
    sec_des_generate_random_block(&fnr);
#define B(n)	((int)fnr.bits[(n)])
    c[0] = ((B(0) & 0xC0) >> 2)
	 | ((B(1) & 0xC0) >> 4)
	 | ((B(2) & 0xC0) >> 6);
    c[1] = ((B(3) & 0xC0) >> 2)
	 | ((B(4) & 0xC0) >> 4)
	 | ((B(5) & 0xC0) >> 6);
    c[2] = ((B(6) & 0xC0) >> 2)
	 | ((B(7) & 0xC0) >> 4);
    sprintf(tfile, "%s/%c%c%c%c%c%c%c%c%c%c%c",
	sv_preauth_dir,
	CHR[B(0) & 0x3F], CHR[B(1) & 0x3F], CHR[B(2) & 0x3F], CHR[B(3) & 0x3F],
	CHR[B(4) & 0x3F], CHR[B(5) & 0x3F], CHR[B(6) & 0x3F], CHR[B(7) & 0x3F],
	CHR[c[0]], CHR[c[1]], CHR[c[2]]);

    /* Open the file, carefully setting modes. */
    tfd = open(tfile, O_WRONLY | O_CREAT | O_TRUNC | O_EXCL, S_IRUSR | S_IWUSR);
    chown(tfile, caller_uid, getgid());

    /* Write the random key to the file. */
    if (tfd < 0)
	*st = errno;
    else if ((F = fdopen(tfd, "w")) == NULL)
        *st = errno;
    else {
        if (fwrite(&rndkey->keytype, sizeof rndkey->keytype, 1, F) != 1
	 || fwrite(&rndkey->length, sizeof rndkey->length, 1, F) != 1
	 || fwrite(rndkey->contents, sizeof rndkey->contents[0], rndkey->length, F) != rndkey->length
	 || fflush(F) == EOF) {
            *st = errno;
            fclose(F);
        }
	else if (fclose(F) == EOF)
	    *st = errno;
    }
    krb5_free_keyblock(rndkey);
    if (*st != error_status_ok) {
	unlink(tfile);
        krb5_free_cred_contents(&creds);
        krb5_free_data(rkey_data);
        return;
    }
    queue_deletion(tfile);

    /* Set up encryption data. */
    edata.kvno = 0;
    krb5_use_keytype(&eblock, creds.keyblock.keytype);
    *st = krb5_process_key(&eblock, &creds.keyblock);
    if (*st != error_status_ok) {
        krb5_free_cred_contents(&creds);
        krb5_free_data(rkey_data);
        return;
    }

    /* Encrypt random key in machine session key. */
    sec_login_util_encrypt(&eblock, rkey_data, &edata.ciphertext, st);
    krb5_free_data(rkey_data);
    if (*st != error_status_ok) {
        krb5_finish_key(&eblock);
        krb5_free_cred_contents(&creds);
        return;
    }

    /* Encode edata. */
    edata.etype = eblock.crypto_entry->proto_enctype;
    *st = encode_krb5_enc_data(&edata, &edata_data);
    krb5_finish_key(&eblock);
    free(edata.ciphertext.data);
    if (*st != error_status_ok) {
        krb5_free_cred_contents(&creds);
        return;
    }

    byte_size = sizeof (encoded_data_t) + edata_data->length;
    if ((*ebuf = dced_copy_for_stub(byte_size)) == NULL) {
        krb5_free_cred_contents(&creds);
        krb5_free_data(edata_data);
        *st = sec_s_no_memory;
        return;
    }
    (*ebuf)->num_bytes = edata_data->length;
    memcpy((*ebuf)->bytes, edata_data->data, edata_data->length);
    krb5_free_data(edata_data);

    /* Ticket is already encoded, so we can simply copy it to machine_tgt. */
    byte_size = sizeof (encoded_data_t) + creds.ticket.length;
    if ((*machine_tgt = dced_copy_for_stub(byte_size)) == NULL) {
        krb5_free_cred_contents(&creds);
        *st = sec_s_no_memory;
        return;
    }
    (*machine_tgt)->num_bytes = creds.ticket.length;
    memcpy((*machine_tgt)->bytes, creds.ticket.data, creds.ticket.length);
    *tfilep = (idl_char *)tfile;

    /* Done. */
    krb5_free_cred_contents(&creds);
}


void
init_sv_preauth(
    error_status_t	*st
)
{
    pthread_t		tid;
    struct stat		sb;

    if (stat(sv_preauth_dir, &sb) < 0
     || !S_ISDIR(sb.st_mode)
     || sb.st_uid != 0
     || sb.st_gid != 0
     || PERMISSIONS(sb.st_mode) != WANTED_PERMISSIONS) {
	*st = dced_s_bad_keydir;
	return;
    }

    pthread_mutex_init(&dq.m, pthread_mutexattr_default);
    pthread_cond_init(&dq.cv, pthread_condattr_default);
    pthread_create(&tid, thread_attr, file_deleter, NULL);
    pthread_detach(&tid);

    *st = error_status_ok;
}
