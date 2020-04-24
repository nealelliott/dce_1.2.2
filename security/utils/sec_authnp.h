/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_authnp.h,v $
 * Revision 1.1.6.1  1996/06/04  22:04:53  arvind
 * 	Merge u2u changes:
 * 	Define sec__krb_server_type (ktab or u2u) for specifying what'sin
 * 	sec__serv_id_table. Add identity and prev/curr creds (TGT data) to
 * 	sec_krb_server_t for u2u usage. Add proto for sec_krb_u2u_keyproc()
 * 	[1996/05/06  20:52 UTC  burati  /main/DCE_1.2/2]
 *
 * 	merge u2u work
 * 	[1996/04/29  21:59 UTC  burati  /main/DCE_1.2/mb_u2u/1]
 *
 * 	u2u work off Mothra, before 1.2 branch is available
 * 	[1996/04/29  21:33 UTC  burati  /main/HPDCE02/mb_mothra8/1]
 *
 * 	merge
 * 	[1995/10/12  23:01 UTC  rps  /main/HPDCE02/2]
 *
 * 	handle misaligned buffers
 * 	[1995/10/10  23:29 UTC  rps  /main/HPDCE02/rps_moth4/1]
 *
 * 	Merge
 * 	[1995/05/24  19:27 UTC  mullan_s  /main/HPDCE02/1]
 *
 * 	Include sec_login_util.h instead of dce/sec_login_util.h
 * 	because it is no longer exported.
 * 	[1995/05/23  19:01 UTC  mullan_s  /main/mullan_mothra_bin_compat10/1]
 *
 * Revision 1.1.4.2  1996/02/18  23:03:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:24  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  18:02:45  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/2  1995/10/12  23:01 UTC  rps
 * 	merge
 * 
 * 	HP revision /main/HPDCE02/rps_moth4/1  1995/10/10  23:29 UTC  rps
 * 	handle misaligned buffers
 * 
 * 	HP revision /main/HPDCE02/1  1995/05/24  19:27 UTC  mullan_s
 * 	Merge
 * 
 * 	HP revision /main/mullan_mothra_bin_compat10/1  1995/05/23  19:01 UTC  mullan_s
 * 	Include sec_login_util.h instead of dce/sec_login_util.h
 * 	because it is no longer exported.
 * 	[1995/12/08  17:23:04  root]
 * 
 * Revision 1.1.2.4  1994/08/09  17:32:49  burati
 * 	DFS/EPAC/KRPC/dfsbind changes
 * 	[1994/08/08  22:00:58  burati]
 * 
 * Revision 1.1.2.3  1994/06/17  18:43:04  devsrc
 * 	cr10872 - fix copyright
 * 	[1994/06/17  18:20:20  devsrc]
 * 
 * Revision 1.1.2.2  1994/06/02  21:17:58  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  15:20:59  mdf]
 * 
 * 	hp_sec_to_osf_3 drop
 * 
 * Revision 1.1.2.1  1994/01/28  23:11:33  burati
 * 	Initial version, sec_authn restructuring (dlg_bl1)
 * 	[1994/01/20  03:41:57  burati]
 * 
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1993
** Unpublished work. All Rights Reserved.
*/

#include <pthread.h>
#include <stdio.h>
#include <krb5/krb5.h>
#include <krb5/mit-des.h>
#include <krb5/ext-proto.h>     /* includes pthread.h */
#include <krb5/rsa-md5.h>
#include <dce/sec_authn.h>
#include <dce/secidmap.h>
#include <dce/sec_login.h>
#include <dce/binding.h>
#include <dce/keymgmt.h>
#include <dce/dce_cf.h>

#include <sec_login_util.h>
#include <sec_pkl.h>  
#include <sec_id_pac_util.h>
#include <sec_krb_util.h>  
#include <priv_krb_util.h>  
#include <rgymacro.h>
#include <assert.h>

#define PTHREAD_EXC        /* make one version of pthread_exc.h harmless */
#define PTHREAD_EXC_H      /* make another version of pthread_exc.h harmless */
#include <dce/stubbase.h>  /* includes pthread_exc.h */

#include <sys/types.h>
#include <netinet/in.h>    /* for ntohl/htonl */

extern unsigned char *rpc_stralloc PROTOTYPE ((unsigned char *));
extern int krb5_kdc_default_options;

extern char *sec__local_cell_name;
extern int sec__local_cell_len;      /* strlen of sec__local_cell_name */

mit_des_random_key_seed *sec__krb_key_seed;

/* Type of server registration */
typedef unsigned32 sec_krb_svr_type_t;
#define sec_krb_svr_type_ktab	0x1     /* Kerberos KTAB registration*/
#define sec_krb_svr_type_u2u	0x2     /* Kerberos U2U  registration*/

/* determines whether keytab or u2u registration*/
sec_krb_svr_type_t sec__krb_server_type;

/* Storage of a server's auth registration (used in sec__serv_id_table).
 * Only one arm of the information below the names is used for each
 * server type.  It is worth wasting the extra little bit of space to have both
 * types of into in the structure, vs the complexity of using a union here.
 */
typedef struct {
    unsigned_char_p_t server_string;
    unsigned_char_p_t server_global;

    /* keytab information */
    rpc_auth_key_retrieval_fn_t get_key_func;
    krb5_pointer arg;
    krb5_keytab keytab;

    /* login context handle for TGT retrieval, for u2u registration */
    sec_login_handle_t lh;
    krb5_creds         *curr_creds;
    krb5_creds         *prev_creds;
} sec_krb_server_t;

extern int sec__n_idents;
extern sec_krb_server_t *sec__serv_id_table;
extern pthread_mutex_t sec__id_lock;
extern krb5_keytab sec__default_keytab;
extern krb5_rcache sec__replay_cache;

typedef struct 
{
    krb5_timestamp ctime;
    krb5_int32 cusec;
    krb5_int32 seq_number;
} sec_krb_msgstamp;

/*
 * Assorted macros.
 */

#define LOCK_IDENTITY() (pthread_mutex_lock(&sec__id_lock))
#define UNLOCK_IDENTITY() (pthread_mutex_unlock(&sec__id_lock))

/*
 * Macro to determine if a name is purely local and can just be passed
 * to krb5_parse_name as-is without having to resort to an external authority.
 */

#define SEC_LOCAL_NAME(name) (((name)[0] != '/') || (u_strchr((name), '@') != NULL))

/*
 * For various reasons, it's painful to get at the NDR tag of the
 * underlying data, so we cheat and just encode it in big-endian order.
 */

#define rpc_marshall_be_long_int(mp, bei) \
{       long temp = htonl(bei);            \
        rpc_marshall_long_int (mp, temp);  \
}
      
#define rpc_convert_be_long_int(mp, bei) \
{                                       \
    rpc_unmarshall_long_int(mp, bei);   \
    bei = ntohl(bei);                   \
}

/*
 * Use memcpy in this version to relax the requirement
 * that the input pointer be naturally aligned.
 */
#define rpc_convert_be_long_int_unalign(mp, bei) \
{					\
    memcpy(&bei, mp, sizeof(bei));	\
    bei = ntohl(bei);			\
}


/* 
 * Callback routine for key fetch, defined in sec_authn.c.
 */

krb5_error_code sec_krb_keyproc 
    PROTOTYPE((
	krb5_pointer ptr,
	krb5_principal server,
	krb5_keytype keytype,
	krb5_kvno kvno,
	krb5_keyblock **keyp
    ));

/*
 * Callback routine for user to user based key fetch, defined in sec_krb_u2u.c
 */
krb5_error_code sec_krb_u2u_keyproc 
    PROTOTYPE((
	krb5_pointer ptr,
	krb5_principal server,
	krb5_keytype keytype,
	krb5_kvno kvno,
	krb5_keyblock **keyp
    ));

error_status_t sec_krb_status_wire_to_rpc
    PROTOTYPE ((
        unsigned32      code
    ));

unsigned32 sec_krb_status_to_wire
    PROTOTYPE ((
        unsigned32      code,
        boolean32       *krb_error
    ));

void  sec_authn_pvt_get_krb_ccache
    PROTOTYPE((
	sec_login_handle_t  auth_identity, /* [in] */
	unsigned32          authz_proto, /* [in] */
	krb5_ccache         *krb_ccache_p, /* [out] */
	error_status_t      *stp
    ));


error_status_t sec_krb_build_message_common
    PROTOTYPE((
	sec_krb_ccache ccache,	/* in */
	sec_krb_cred cred,	/* in */
	krb5_checksum *send_cksum, /* in */
	unsigned32 authz_proto,	/* in */
	unsigned32 authz_fmt, /* in */
	sec_krb_msgstamp *msgstamp, /* in/out */
	sec_krb_message *message /* out */
    ));

error_status_t sec_krb_build_message_none
    PROTOTYPE((
	sec_krb_ccache ccache,	/* in */
	sec_krb_cred cred,	/* in */
	unsigned32 authz_proto,	/* in */
	unsigned32 authz_fmt,	/* in */
	sec_krb_message *message /* out */
    ));

error_status_t sec_krb_decode_message_common
    PROTOTYPE((
	sec_krb_message *message,		/* in */
	unsigned32 authz_fmt,			/* in */
	sec_krb_parsed_name *server_name,	/* out */
	unsigned char **client_name,		/* out */
	sec_id_pac_t *client_pac,		/* out */
	rpc_authz_cred_handle_t *client_creds,	/* out */
	unsigned32 *expiration,			/* out */
	sec_krb_msgstamp *timestamp,		/* out */
	krb5_tkt_authent **authent,		/* out, freed by caller */
	unsigned32 *authz_proto			/* out */
    ));

error_status_t sec_krb_decode_msg_common_frags
    PROTOTYPE((
	sec_krb_message *message,		/* in */
	unsigned32 authz_fmt,			/* in */
	sec_krb_parsed_name *server_name,	/* out */
	unsigned char **client_name,		/* out */
	sec_id_pac_t *client_pac,		/* out */
	sec_bytes_t  *raw_epac_setP,		/* out */
	rpc_authz_cred_handle_t *client_creds,	/* out */
	unsigned32 *expiration,			/* out */
	sec_krb_msgstamp *timestamp,		/* out */
	krb5_tkt_authent **authent,		/* out, freed by caller */
	unsigned32 *authz_proto			/* out */
    ));

error_status_t sec_krb_build_rep_common
    PROTOTYPE((
	unsigned32 authn_level,	/* in */
	krb5_keyblock *key,	/* in */
	sec_krb_msgstamp *msgstamp, /* in */
	sec_krb_message * message_out /* out */
    ));

error_status_t sec_krb_decode_rep_common
    PROTOTYPE((
	unsigned32 authn_level,	/* in */
	sec_krb_message *message, /* in */
	krb5_keyblock *key,	/* in */
	sec_krb_msgstamp *msgstamp /* out */
    ));


/*
 * find keytab, get_key_func, if any, of server.
 * needed for GSSAPI.
 */

error_status_t sec_krb_inq_server
    PROTOTYPE((
	unsigned_char_p_t server_princ_name, 
	rpc_auth_key_retrieval_fn_t *get_key_func, 
	void **arg
    ));


/* sec_krb_u2u_get_tgt
 * 
 * Lookup TGT data from the login handle registered in the sec__serv_id_table.
 */
error_status_t sec_krb_u2u_get_tgt(
    sec_login_handle_t  lh,
    unsigned32          *tgt_len,
    unsigned_char_p_t   *tgt_data_p,
    mit_des_cblock      tgt_skey,
    krb5_creds          *ret_creds_p
);

