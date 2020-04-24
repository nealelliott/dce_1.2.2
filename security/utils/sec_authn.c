/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_authn.c,v $
 * Revision 1.2.28.4  1996/10/15  20:57:20  arvind
 * 	fix trivial memory leak seen during secd startup.
 * 	[1996/10/11  00:06 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_2/1]
 *
 * 	Merge u2u changes: Add  tgt_len and tgt_data args to sec_krb_get_cred()
 * 	Use correct keyproc for ktab vs u2u in sec_krb_decode_msg_common_frags()
 * 	[1996/04/29  22:06 UTC  burati  /main/DCE_1.2/mb_u2u/1]
 *
 * 	merge u2u work
 * 	[1996/04/29  21:34 UTC  burati  /main/HPDCE02/mb_mothra8/1]
 *
 * 	u2u work off Mothra, before 1.2 branch is available
 * 	[1996/01/03  19:20 UTC  psn  /main/DCE_1.2/1]
 *
 * Revision 1.2.28.3  1996/10/03  15:14:26  arvind
 * 	Deal with compiler warnings.
 * 	[1996/09/16  23:29 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 * 
 * 	Fix memory leak for CHFts19783.
 * 	[1996/09/16  17:33 UTC  jrr  /main/HPDCE02/PATCH_1.4X/jrr_perf_3/1]
 * 
 * 	Merge u2u changes: Add  tgt_len and tgt_data args to sec_krb_get_cred()
 * 	Use correct keyproc for ktab vs u2u in sec_krb_decode_msg_common_frags()
 * 	[1996/04/29  22:06 UTC  burati  /main/DCE_1.2/mb_u2u/1]
 * 
 * 	merge u2u work
 * 	[1996/04/29  21:34 UTC  burati  /main/HPDCE02/mb_mothra8/1]
 * 
 * 	u2u work off Mothra, before 1.2 branch is available
 * 	[1996/01/03  19:20 UTC  psn  /main/DCE_1.2/1]
 * 
 * Revision 1.2.28.2  1996/06/04  22:04:40  arvind
 * 	last endgame sec fix fron hp -mcg
 * 	[1996/04/16  21:00:34  root]
 * 
 * 	sec_krb_get_cred(): if necessary, construct ptgt authdata with
 * 	ptgt_uuid so that krb5_get_credentials_dce will find the correct
 * 	PTGT in the rare case when there are duplicate PTGTS in the cache.
 * 	[1996/04/14  20:26 UTC  mullan_s  /main/HPDCE02/bartf_chfts16961/1]
 * 
 * 	sec_krb_get_cred(): if necessary, construct ptgt authdata with
 * 	ptgt_uuid so that krb5_get_credentials_dce will find the correct
 * 	PTGT in the rare case when there are duplicate PTGTS in the cache.
 * 	[1996/04/14  20:26 UTC  mullan_s  /main/HPDCE02/bartf_chfts16961/1]
 * 
 * 	Merge u2u changes: Add  tgt_len and tgt_data args to sec_krb_get_cred()
 * 	Use correct keyproc for ktab vs u2u in sec_krb_decode_msg_common_frags()
 * 	[1996/04/29  22:06 UTC  burati  /main/DCE_1.2/mb_u2u/1]
 * 
 * 	merge u2u work
 * 	[1996/04/29  21:34 UTC  burati  /main/HPDCE02/mb_mothra8/1]
 * 
 * 	u2u work off Mothra, before 1.2 branch is available
 * 	[1996/01/03  19:20 UTC  psn  /main/DCE_1.2/1]
 * 
 * Revision 1.2.28.1  1996/05/17  15:15:00  arvind
 * 	Drop 1 of DCE 1.2.2 to OSF
 * 
 * 	HP revision /main/DCE_1.2/mullan_dce121_secrefresh/1  1996/04/16  17:13 UTC  mullan_s
 * 	CHFts16434: passwd_export sometimes gets rpc_s_auth_tkt_expired error;
 * 	loop again to get tickets in sec_krb_get_cred() if get KRB5KRB_AP_ERR_TKT_EXPIRED
 * 	error while retrieving STKT when previous retrieval of EPAC succeeded.
 * 	[1995/11/14  14:26 UTC  aha  /main/HPDCE02/aha_mothra12/4]
 * 
 * 	merge
 * 	[1995/10/12  23:01 UTC  rps  /main/HPDCE02/13]
 * 
 * 	handle misaligned buffers
 * 	[1995/10/10  23:41 UTC  rps  /main/HPDCE02/rps_moth4/1]
 * 
 * 	Merge fix for CHFts16546
 * 	[1995/10/04  19:18 UTC  mullan_s  /main/HPDCE02/12]
 * 
 * 	Call pac_to_epac_chain_bytes instead of pac_to_epac
 * 	[1995/10/04  18:28 UTC  mullan_s  /main/HPDCE02/mullan_chfts16496/1]
 * 
 * 	Rename functions.
 * 	[1995/08/31  05:58 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/2]
 * 
 * 	Performance: let secidmap do the rca_site_bind iff its needed.
 * 	[1995/08/14  15:23 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/1]
 * 
 * 	Merge fix for CHFts15637
 * 	[1995/08/22  21:08 UTC  mullan_s  /main/HPDCE02/10]
 * 
 * 	Call priv_krb_util_construct_v1_stkt_authdata
 * 	[1995/08/09  14:18 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/2]
 * 
 * 	CHFts15637: Fix "mismatched seal" race conditions in sec_krb_get_cred.
 * 	[1995/07/26  19:32 UTC  mullan_s  /main/HPDCE02/mullan_chfts15637/1]
 * 
 * 	Merge
 * 	[1995/05/22  17:05 UTC  mullan_s  /main/HPDCE02/9]
 * 
 * 	sec_krb_get_cred: pass login handle to priv_krb_util_fetch_v1_1_ccache_authdata.
 * 	[1995/05/22  15:51 UTC  mullan_s  /main/HPDCE02/mullan_mothra_bin_compat9/1]
 * 
 * 	Merge memory leak fix from mb_mothra3
 * 	[1995/05/11  19:28 UTC  burati  /main/HPDCE02/8]
 * 
 * 	Fix mem leak of authentP in sec_krb_decode_msg_common_frags()
 * 	[1995/05/11  15:02 UTC  burati  /main/HPDCE02/mb_mothra3/1]
 * 
 * 	sec_krb_get_cred: set kcred->authdata=NULL after freeing it.
 * 	[1995/04/24  18:16 UTC  mullan_s  /main/HPDCE02/7]
 * 
 * 	sec_krb_get_cred: free kcred->authdata after we retrieve service ticket and
 * 	before we (optionally) put epac chain in.
 * 	[1995/04/21  20:15 UTC  mullan_s  /main/HPDCE02/6]
 * 
 * 	merge up to nightly
 * 	[1995/04/10  17:46 UTC  rps  /main/HPDCE02/MOTHRA_panic/2]
 * 
 * 	Binary Compatibility Merge
 * 	[1995/04/03  22:15 UTC  mullan_s  /main/HPDCE02/4]
 * 
 * 	DTS 14492 (OT 12808) and DTS 14418 (OT 12787) bug fix.
 * 	[1995/03/03  15:33 UTC  cuti  /main/HPDCE02/3]
 * 
 * 	Fix ot12808 (CHFts14492).
 * 	[1995/03/01  19:37 UTC  cuti  /main/HPDCE02/cuti_mothra_bug/1]
 * 
 * 	CHF13575: Avoid some needless lock contention
 * 	[1995/02/08  20:40 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHF13575/1]
 * 
 * 	[1994/12/20  18:50 UTC  mullan_s  /main/HPDCE02/1]
 * 
 * 	[1994/12/20  18:34 UTC  mullan_s  /main/mullan_dfsbind_mem_leaks/2]
 * 
 * 	Free server_cell in sec_krb_get_cred().
 * 	[1994/12/16  22:05 UTC  mullan_s  /main/mullan_dfsbind_mem_leaks/1]
 * 
 * 	HP revision /main/HPDCE02/bartf_chfts16961/1  1996/04/14  20:26 UTC  mullan_s
 * 	sec_krb_get_cred(): if necessary, construct ptgt authdata with
 * 	ptgt_uuid so that krb5_get_credentials_dce will find the correct
 * 	PTGT in the rare case when there are duplicate PTGTS in the cache.
 * 
 * 	HP revision /main/HPDCE02/14  1995/11/15  21:12 UTC  aha
 * 	CHFts16434: passwd_export sometimes gets rpc_s_auth_tkt_expired error;
 * 	loop again to get tickets in sec_krb_get_cred() if get KRB5KRB_AP_ERR_TKT_EXPIRED
 * 	error while retrieving STKT when previous retrieval of EPAC succeeded.
 * 	[1995/11/14  14:26 UTC  aha  /main/HPDCE02/aha_mothra12/4]
 * 
 * 	HP revision /main/HPDCE02/13  1995/10/12  23:01 UTC  rps
 * 	merge
 * 
 * 	HP revision /main/HPDCE02/rps_moth4/1  1995/10/10  23:41 UTC  rps
 * 	handle misaligned buffers
 * 
 * 	HP revision /main/HPDCE02/12  1995/10/04  19:18 UTC  mullan_s
 * 	Merge fix for CHFts16546
 * 
 * 	HP revision /main/HPDCE02/mullan_chfts16496/1  1995/10/04  18:28 UTC  mullan_s
 * 	Call pac_to_epac_chain_bytes instead of pac_to_epac
 * 
 * 	HP revision /main/HPDCE02/11  1995/09/08  17:26 UTC  sommerfeld
 * 	Rename functions.
 * 	[1995/08/31  05:58 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/2]
 * 
 * 	Performance: let secidmap do the rca_site_bind iff its needed.
 * 	[1995/08/14  15:23 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/1]
 * 
 * 	HP revision /main/HPDCE02/10
 * 
 * Revision 1.2.20.12  1994/10/07  18:30:47  rps
 * 	[OT12150] sec_krb_unparse_name:  canonicalize potential local cellname alias
 * 	[1994/10/07  15:21:17  rps]
 * 
 * Revision 1.2.20.11  1994/10/05  18:41:28  burati
 * 	CR12518 Was free()ing ap_req.data twice in sec_krb_build_message_common
 * 	[1994/10/05  18:40:15  burati]
 * 
 * Revision 1.2.20.10  1994/09/28  21:34:55  burati
 * 	CR12179 (slowness) Call new rtn krb5_get_credentials_dce()
 * 	[1994/09/28  21:13:12  burati]
 * 
 * 	Fix core dump induced by previous version.
 * 
 * 	Incorporate changes needed for DFS delegation work.
 * 	[1994/08/16  22:10:28  greg]
 * 
 * Revision 1.2.20.9  1994/08/24  21:34:38  burati
 * 	CR11861 Generate simulated auth info in compatibility case even if
 * 	called from dfsbind which only wants raw epac set back.
 * 	[1994/08/24  21:28:10  burati]
 * 
 * Revision 1.2.20.8  1994/08/18  20:25:45  greg
 * 	Accidental checkout: since I haven't figured out
 * 	how to undo a bco, I'll just have to check it back in.
 * 	[1994/08/17  14:10:15  greg]
 * 
 * 	Incorporate changes for DFS delegation (dfsbind).
 * 	[1994/08/17  13:02:24  greg]
 * 
 * Revision 1.2.20.6  1994/08/04  16:15:00  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/08/02  17:17:02  mdf]
 * 
 * Revision 1.2.20.5  1994/07/06  20:27:15  sommerfeld
 * 	[OT10607] pick up greg's change to the seal creation functions.
 * 	[1994/07/06  19:38:52  sommerfeld]
 * 
 * 	HP revision /main/ODESSA_2/cuti_dlg/1  1994/07/26  21:09 UTC  cuti
 * 	Add sec__cred_set_session_info to cred_handle
 * 
 * 	HP revision /main/ODESSA_2/7  1994/06/07  13:26 UTC  mdf
 * 	 Merge files from ODE sandbox back to Clearcase view
 * 
 * 	HP revision /main/ODESSA_2/mdf_odessa_era_bl37/1  1994/06/07  11:22 UTC  mdf
 * 	"merge back from ODE to ClearCase"
 * 
 * Revision 1.2.21.4  94/05/24  11:17:21  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 
 * Revision 1.2.21.3  94/05/20  7:23:09  mdf
 * 	hp_sec_to_osf_3 drop, removing cruft.
 * 
 * Revision 1.2.21.2  1994/05/18  12:17:30  mdf
 * 	hp_sec_to_osf_3 drop
 * 	[1994/05/18  12:17:30  mdf]
 * 
 * Revision 1.2.20.3  1994/05/11  19:34:52  ahop
 * 	hp_sec_to_osf_2 drop
 * 	pass real delegation token and seal sets to create_authz_handle.
 * 	add memory management function parameters to sec_encode interfaces
 * 	add compatibility mode to pac_to_epac() call
 * 	[1994/04/29  21:50:44  ahop]
 * 
 * Revision 1.2.20.2  1994/03/16  15:25:12  burati
 * 	CR10080 Don't continue on error in sec_krb_decode_message_common
 * 	[1994/03/16  00:08:19  burati]
 * 
 * Revision 1.2.20.1  1994/01/28  23:11:30  burati
 * 	sec_authn restructuring (dlg_bl1)
 * 	[1994/01/20  03:29:49  burati]
 * 
 * 	sec_authn restructuring (dlg_bl1)
 * 
 * Revision 1.2.18.1  1993/10/05  22:31:15  mccann
 * 	CR8651 64 bit porting changes
 * 	[1993/10/04  19:14:51  mccann]
 * 
 * Revision 1.2.16.2  1993/08/19  15:07:37  rps
 * 	[CHFts08414] sec_krb_register_server: unwind incomplete keytab alloc
 * 	[1993/08/17  15:41:45  rps]
 * 
 * Revision 1.2.14.7  1992/12/29  16:40:51  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  21:32:07  zeliff]
 * 
 * Revision 1.2.14.6  1992/12/11  20:52:01  sommerfeld
 * 	Retry on all errors rather than just a couple.
 * 	[1992/12/11  02:17:53  sommerfeld]
 * 
 * 	Bugfix to previous change: move variable defn.
 * 	[1992/12/10  16:18:20  sommerfeld]
 * 
 * 	[OT5794] don't check cache for ptgt et al if the priv tkt is
 * 	already there; a minor optimization.
 * 	[1992/12/09  22:43:43  sommerfeld]
 * 
 * Revision 1.2.14.5  1992/12/09  22:13:31  sommerfeld
 * 	[OT4707] use proper allocator in sec_id_unparse_name().
 * 	[1992/12/08  20:44:37  sommerfeld]
 * 
 * Revision 1.2.14.4  1992/10/20  14:36:43  burati
 * 	CR4785 mis-placed UNLOCK_IDENTITY in sec_authn.c
 * 	[1992/10/19  18:38:13  burati]
 * 
 * Revision 1.2.14.3  1992/10/13  20:57:11  weisman
 * 	Submit Wei Hu CN deadlock fixes/OT 5143
 * 	[1992/10/13  20:48:53  weisman]
 * 
 * Revision 1.2.14.2  1992/09/29  21:20:24  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:41:45  sekhar]
 * 
 * Revision 1.2.2.8  1992/07/13  21:26:47  sommerfeld
 * 	Clean up slightly to avoid being whopped by rsalz's momma.
 * 	[1992/07/13  19:40:06  sommerfeld]
 * 
 * 	[CR4513] check return code from krb5_copy_principal; fail if it fails.
 * 	[CR4513] Disallow parse of null name.
 * 	[1992/07/13  18:43:41  sommerfeld]
 * 
 * Revision 1.2.2.7  1992/05/26  21:42:23  pato
 * 	Fix typo in module ID string
 * 	[1992/05/26  15:30:01  pato]
 * 
 * 	OT#3620 default RPC key retrieval should not need keytab type prefix
 * 	[1992/05/22  21:52:39  pato]
 * 
 * Revision 1.2.2.6  1992/05/20  21:16:46  sommerfeld
 * 	Avoid alignment faults when unpickling.
 * 	Silence compiler warnings from GCC.
 * 	[1992/05/20  18:30:11  sommerfeld]
 * 
 * Revision 1.2.2.5  1992/05/20  19:53:58  rsalz
 * 	1992/05/06            pinkoski
 * 	     Check for no repl before calling
 * 	     krb5_free_ap_rep_enc_part.
 * 
 * 	1992/05/01            pinkoski
 * 	     Special version for RPC6 (AUTH CN) until HP can
 * 	     merge these changes at OSF
 * 	[1992/05/20  19:53:24  rsalz]
 * 
 * Revision 1.2.2.4  1992/02/26  18:31:29  sommerfeld
 * 	Fix LHS-casts.
 * 	[1992/02/26  16:12:51  sommerfeld]
 * 
 * Revision 1.2.2.3  1992/02/20  19:42:19  sommerfeld
 * 	Patch embedded revision.
 * 	Include fix for 2094 (along for the ride).
 * 	[1992/02/20  19:02:00  sommerfeld]
 * 
 * 	     Synchronize with sources at HP.
 * 	     Merge in CN-RPC authentication support from DEC.
 * 	[1992/02/20  18:57:31  sommerfeld]
 * 
 * Revision 1.2.2.2  1992/01/22  21:55:14  weisman
 * 	                  Merge in IBM defect fixes archive.
 * 	          [1992/01/22  21:54:52  weisman]
 * 
 * Revision 1.2  1992/01/19  22:13:53  devrcs
 * 	          Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */

/*
** Copyright (c) Hewlett-Packard Company 1991, 1992, 1994, 1995, 1996
** Unpublished work. All Rights Reserved.
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */

/* 
 *      Security Client Agent - Interface for RPC.
 */

#ifdef MODULE_VERSION_ID
static char *VersionID =
"$Id: sec_authn.c,v 1.2.28.4 1996/10/15 20:57:20 arvind Exp $";
#endif

#include <sec_authnp.h>
#include "u_str.h"

#include <pwd.h>
#include <sec_encode.h>
#include <authz_util.h>
#include <priv_krb_util.h>
#include <sec_login_db.h>

#include <restrictions.h>
#include <sec_cred_internal.h>

/*
 * Prototypes for the raw DES calls.
 */

extern void mit_des_cbc_cksum
    PROTOTYPE((
        krb5_octet *,
        krb5_octet *,
        krb5_int32,
        sec_des_key_schedule key, 
        krb5_octet *
    ));

extern krb5_error_code mit_des_cbc_encrypt
    PROTOTYPE((
        krb5_octet *,
        krb5_octet *,
        krb5_int32,
        sec_des_key_schedule,
        krb5_octet *,
        int
    ));

extern int mit_des_key_sched
    PROTOTYPE((
        mit_des_cblock ,
        sec_des_key_schedule 
    ));

extern krb5_error_code mit_des_new_random_key
    PROTOTYPE((
        mit_des_cblock key,
        mit_des_random_key_seed *p_seed
    ));

extern void mit_des_generate_random_block
    PROTOTYPE((
        mit_des_cblock key,
        mit_des_random_key_seed *p_seed
    ));

extern void mit_des_init_random_number_generator
    PROTOTYPE((
        mit_des_cblock key,
        mit_des_random_key_seed *p_seed
    ));

static void sec_id_pac_print
    PROTOTYPE((
        sec_id_pac_t *pac
    ));

static int sec_asn1len
    PROTOTYPE((
	unsigned char *base, 
	unsigned char *limit
    ));

/*
 * Global Variables.
 */

int sec__n_idents = 0;
sec_krb_server_t *sec__serv_id_table = NULL;
pthread_mutex_t sec__id_lock;
pthread_mutex_t sec__random_lock;
krb5_keytab sec__default_keytab = NULL;
krb5_rcache sec__replay_cache = NULL;
#define LOCK_RANDOM() (pthread_mutex_lock(&sec__random_lock))
#define UNLOCK_RANDOM() (pthread_mutex_unlock(&sec__random_lock))
#define IS_SET(FIELD, MASK) ((FIELD) & (MASK)) == (MASK)

char *sec__local_cell_name = NULL;
int sec__local_cell_len;      /* strlen of sec__local_cell_name */


void  sec_authn_pvt_get_krb_ccache
  (
     sec_login_handle_t  auth_identity,  /* [in] */
     unsigned32          authz_proto,    /* [in] */
     krb5_ccache         *krb_ccache_p,  /* [out] */
     error_status_t      *stp
  )
{
    CLEAR_STATUS(stp);

    /* 
     * For now, at least, we don't support authz_dce interoperability
     * between DCE and vanilla kerberos clients.  
     */
    *krb_ccache_p = (krb5_ccache) sec_login_util_get_krb_cache(auth_identity);
    if (*krb_ccache_p == NULL) {
        if (authz_proto == rpc_c_authz_name) {
            *stp = krb5_cc_default (krb_ccache_p);
        } else {
            /* !!! better status??? */
            SET_STATUS(stp, sec_login_s_context_invalid);
        }
    }
}
     
void sec_krb_parsed_name_free (pname)
    sec_krb_parsed_name *pname;
{
    krb5_principal real_name = (krb5_principal)*pname;

    if (real_name)
        krb5_free_principal(real_name);
    *pname = NULL;
}

void sec_krb_cred_free (cred)
    sec_krb_cred *cred;
{
    krb5_creds *val = (krb5_creds *) *cred;

    if (val)
        krb5_free_creds(val);
    *cred = NULL;
}

void sec_krb_ccache_free (ccache)
    sec_krb_ccache *ccache;
{
    sec_krb_ccache cc;
    
    /*
     * Drop refcount on ccache; free it if it goes to zero
     */
    cc = *ccache;

    if (cc) {
        *ccache = NULL;    
        sec_login_util_release(cc);
    }
}

void sec_krb_message_free (message)
    sec_krb_message *message;
{
    if (message->data)
        free(message->data);
    message->data = NULL;
    message->length = 0;
}


error_status_t sec_krb_get_cc (auth_ident, ccache)
    rpc_auth_identity_handle_t auth_ident;
    sec_krb_ccache *ccache;
{
    error_status_t st;
    
    sec_login_util_reference(auth_ident, ccache, &st);

    return st;
}

   
/*
 * s e c _ k r b _ s e c _ p a r s e _ n a m e
 *
 * This is a secure call.
 */

error_status_t sec_krb_sec_parse_name (context, level, name, parsed_name)
    rpc_auth_identity_handle_t context;
    unsigned32 level;
    unsigned_char_p_t     name;
    sec_krb_parsed_name   *parsed_name;
{
    sec_rgy_bind_auth_info_t  binding_auth_info; 
    sec_rgy_handle_t          rgy_context;  
    sec_rgy_name_t            cell_name;
    sec_rgy_name_t            princ_name;
    error_status_t            st, xst;   


    /* 
     * Don't allow zero-length names past here; they confuse too much
     * further down the line.
     */
    if (name[0] == '\0')
	return sec_s_null_param_illegal;
    
    /* 
     * check for names of the form "principal@realm" or cell relative
     * names and parse them immediately into krb5_principal format,
     * doing an end run around the rest of this hideous mechanism.
     */
    if (SEC_LOCAL_NAME(name)) {
        st = krb5_parse_name( (char *) name, (krb5_principal *)parsed_name);
        return st;
    }

    /*
    * Contact the local security service to decompose the global name.
    */
    if (level == rpc_c_authn_level_none) {
        binding_auth_info.info_type = sec_rgy_bind_auth_none;
    } else {
        binding_auth_info.info_type = sec_rgy_bind_auth_dce;
        binding_auth_info.tagged_union.dce_info.authn_level
            = rpc_c_authn_level_pkt_integrity;
        binding_auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_dce_private;
        binding_auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
        binding_auth_info.tagged_union.dce_info.identity = (void *)context;
    }

    sec_id_global_parse_name(&binding_auth_info, name,
		       cell_name, NULL,
		       princ_name, NULL, &st);

    if (BAD_STATUS(&st)) {
        return st;
    }
    sec_krb_princ_from_dce_pair((char *) cell_name, (char *) princ_name,  
                               (krb5_principal *) parsed_name, &st); 
    
    return st;
}


/*
 * Convert parsed name into DCE global name (character string).
 */

PUBLIC  error_status_t  sec_krb_unparse_name
  (
    sec_krb_parsed_name  parsed_name,
    unsigned_char_p_t    *name
  )
{
    char             *cell, *principal, *tempname;
    unsigned_char_p_t outname;
    error_status_t   st;
    char		**aliases;

    *name = NULL;
    sec_dce_pair_from_krb_princ( (krb5_principal) parsed_name, &cell, &principal, &st);
    if (BAD_STATUS(&st)) {
        return st;
    }


    /* canonicalize potential local cellname alias */

    dce_cf_get_cell_aliases(&aliases, &st);
    if (BAD_STATUS(&st))
	return st;

    if (aliases) {
	char		**alias;
	error_status_t	xst;

	for (alias = aliases; *alias; alias++) {
	    if (! strcmp(*alias, cell)) {
		free(cell);
        	dce_cf_get_cell_name(&cell, &st);
		if (GOOD_STATUS(&st))
		    break;
		dce_cf_free_cell_aliases(aliases, &xst);
		return st;
	    }
	}
	dce_cf_free_cell_aliases(aliases, &xst);
    }


    tempname = malloc(strlen(cell) + strlen("/") + strlen(principal) + 1);
    if (tempname != NULL) {  
       strcpy( tempname, cell);
       strcat( tempname, "/");
       strcat( tempname, principal);
    } else {
        SET_STATUS(&st, sec_rgy_cant_allocate_memory);
    }

    /*
     * !!! double allocate because we return an "RPC string", not a
     * malloc block..
     */
    free(cell);
    free(principal);
    outname = rpc_stralloc ((unsigned char *)tempname);
    if (outname != NULL)
    {
	*name = outname;
    } else {
        SET_STATUS(&st, sec_rgy_cant_allocate_memory);
    }
    free (tempname);
    return st;   
}

error_status_t sec_krb_get_cred (ccache, server_name, authn_level, authz_proto,
    tgt_length, tgt_data, cred, expiration)
    sec_krb_ccache ccache;    /* really a sec_login_handle_t */
    sec_krb_parsed_name server_name;
    unsigned32 authn_level;
    unsigned32 authz_proto;
    unsigned32 tgt_length;
    unsigned_char_p_t tgt_data;
    sec_krb_cred *cred;
    unsigned32 *expiration;
{
    krb5_creds *kcred;
    sec_krb_parsed_name client_name;
    krb5_ccache         krb_ccache;
    sec_login_handle_t  identity = (sec_login_handle_t) ccache;
    error_status_t st;
    char  *server_cell = NULL;
    krb5_authdata       **tgt_authdata = NULL;
    krb5_flags options = krb5_kdc_default_options;
    
    client_name = NULL;
    *cred = NULL;
    *expiration = 0x100;        /* long in the past, but positive */

    sec_authn_pvt_get_krb_ccache(identity, authz_proto, &krb_ccache, &st);

    if (BAD_STATUS(&st)) {
        return st;
    }

    if (authz_proto == rpc_c_authz_name) {

        st = sec_krb_get_name (krb_ccache, &client_name);
        if (st != rpc_s_ok) {
            return st;
        }

    } else if (authz_proto == rpc_c_authz_dce) {

        /* 
         * The "client" for authz_dce access is the priv server in the
         * the target server's realm
         */

        sec_dce_pair_from_krb_princ( (krb5_principal) server_name,
            &server_cell, NULL, &st);
        if (GOOD_STATUS(&st))
        {
            sec_krb_ptgs_parsed_name(server_cell,
                (krb5_principal *) &client_name, &st);
        }

        if (BAD_STATUS(&st)) {
            sec_krb_parsed_name_free(&client_name);
            if (st == KRB5KRB_AP_ERR_TKT_EXPIRED) {
                st = rpc_s_auth_tkt_expired;
            }
            if (server_cell)
                free(server_cell);
            return st;
        }
    }

    kcred = (krb5_creds *)malloc (sizeof(krb5_creds));
    if (kcred == NULL) {
        sec_krb_parsed_name_free(&client_name);
        if (server_cell)
            free(server_cell); 
        return sec_s_no_memory;
    }
    memset(kcred, 0, sizeof(*kcred));

    st = krb5_copy_principal((krb5_principal)server_name, &kcred->server);
    if (st == 0) 
    {
	kcred->client = (krb5_principal)client_name;

        /*
         * Construct the authorization data so that DCE 1.1 service
         * tickets and PTGTs are fetched from the creds file
         */
        if (authz_proto == rpc_c_authz_dce) {
	    priv_krb_util_construct_v1_stkt_authdata(NULL, NULL, &kcred->authdata, &st);
        }
    }
    
    /* user-to-user check; if 2nd tkt supplied, use it */
    if (tgt_length != 0) {
        options |= KRB5_GC_USER_USER;
        kcred->second_ticket.length = tgt_length;
	/* Will be deallocated when kcred itself is cleanup later... */
        if ((kcred->second_ticket.data = (char *)malloc(tgt_length)) == NULL) {
            st = sec_s_no_memory;
        } else {
            memcpy(kcred->second_ticket.data, tgt_data, tgt_length);
        }
    }

    if (st == 0) {
        if (authz_proto == rpc_c_authz_dce) {
	    sec_bytes_t		epac_set = {0, NULL};
	    char		*dfn = NULL;
	    sec_cred_state_t 	state;
	    uuid_t		ptgt_uuid;
	    error_status_t	lst;
    	    DB_DATAFILE_NAME(dfn, krb_ccache);

	    /*
             * Loop until we get a matching PTGT and service ticket which
             * we are sure will not cause mismatched seals. The loop will
             * be executed twice if we don't have a PTGT or STKT. The loop
             * will also be executed more than once if this process/thread
             * is swapped out and another process/thread refreshes the same 
             * credentials file, causing the process to reread the krb5
             * credentials file and the .db file for a matching STKT/PTGT pair.
             */
	    do {

                /* Always free the epac_set prior to calling _fetch_ */
                if (epac_set.bytes) {
                    free(epac_set.bytes);
                    epac_set.bytes = NULL;
                    epac_set.num_bytes = 0;
                }

            	/*
             	 * Hope that the ptgt and the service tkt are already there.
            	 * These must be retrieved as a pair and within a lock to prevent
            	 * other processes/threads from refreshing the same credential
            	 * file and causing mismatched seal errors.
            	 */
	    	state = sec_login_util_fetch_epac_and_stktcreds(identity, dfn,
			(unsigned_char_p_t) server_cell, krb_ccache, kcred,
			&epac_set, &ptgt_uuid, &st);

            	if ((st != 0) || !(IS_SET(state, creds_found))) {
		    if (state & epac_not_found) {
     	                /*
    	                 * refresh/acquire any necessary PTGTs before going after the
	                 * service ticket
	                 */
	                sec_login_util_get_krb_ptgt(identity, 
	   		            (unsigned_char_p_t) server_cell,
			            rpc_c_authn_dce_private,
			            rpc_c_authz_dce,
                                    NULL,
				    &st);
			if (st != 0)	/* bail out on errors */
			    break;
		    }
                    if (IS_SET(state, stkt_not_found)) {
		        if (tgt_authdata)
			    krb5_free_authdata(tgt_authdata);
			if (!uuid_is_nil(&ptgt_uuid, &lst))
			    priv_krb_util_construct_v1_ptgt_authdata(NULL, 0, &ptgt_uuid, NULL,
                		&tgt_authdata, NULL, &st);
			else
	    		    priv_krb_util_construct_v1_ptgt_authdata(NULL, 0, NULL, NULL,
				&tgt_authdata, NULL, &st);
			if (st != 0)
			    break;

                        /* 
                         * Kerberos does not expect a fully populated kcred
                         * structure, so free what we don't need.
                         */
                        if (kcred->ticket.data) {
                            xfree(kcred->ticket.data);
                            kcred->ticket.data = NULL;
                        }
                        if (kcred->second_ticket.data) {
                            xfree(kcred->second_ticket.data);
                            kcred->second_ticket.data = NULL;
                        }


		        st = krb5_get_credentials_dce(options,
			                  krb_ccache, kcred, tgt_authdata);
                        /* CHFts16434: if we get an error here, it means the
                         * PTGT just expired.  Release any epac data, then loop
                         * again, which will get a fresh PTGT this time
                         */
                        if (st == KRB5KRB_AP_ERR_TKT_EXPIRED) {
                            /* 
                             * Auth ticket has just expired. Loop again 
                             * to pick up fresh ticket.
                             */
                        } else
                        /* CHFts16434: end of change */
			if (st != 0)    /* bail out on errors */
			    break;
               	    }
		}
            } while (!(IS_SET(state, creds_found)));

	    if (dfn)
		free(dfn);
            /*
             * Get rid of authdata in stkt credentials. The format used
             * by the RPC runtime is a bit different (see below).
             */
            if (kcred->authdata) {
		krb5_free_authdata(kcred->authdata);
                kcred->authdata = NULL;
	    }

            /*
             * Now, put the epac chain in kcred->authdata.
             * We do this so sec_krb_build_message_common() can parse
             * the authdata properly and find the epac chain. It also
             * saves sec_krb_build_message_common() from doing a disk
             * read to get the information.
             *
             * NOTE! The format of the authdata constructed below
             * is different than what is stored on disk in the krb5
             * creds file. The authdata constructed below is only used
             * in-memory by the RPC runtime. The format of the DCE
             * authorization data consists of an encoded epac chain or NULL
             * if we are connected to a 1.0.x server and only PACS are
             * available. sec_krb_build_message_common will call
             * priv_krb_util_parse_v1_1_ccache_authdata which expects
             * the authdata to be in the format just described.
             */
            if ((st == 0) && epac_set.num_bytes && epac_set.bytes) {
                krb5_authdata **tmp_authdata = NULL;

                priv_krb_util_bytes_to_authdata(&epac_set, NULL,
                                                &tmp_authdata, &st);

                if ((st == 0) && tmp_authdata) {
                    kcred->authdata = tmp_authdata;
                }
            }
            if (epac_set.bytes)
                free(epac_set.bytes);
        } else {
            st = krb5_get_credentials_dce(options, 
                 krb_ccache, kcred, tgt_authdata);
	}
    }
    if (tgt_authdata)
        krb5_free_authdata(tgt_authdata);
    if (server_cell)
        free(server_cell);

    if (st != 0) {
        if (kcred)
            krb5_free_creds(kcred);
        if (st == KRB5KRB_AP_ERR_TKT_EXPIRED) {
            st = rpc_s_auth_tkt_expired;
        }
        return st;
    }
	
    *cred = (sec_krb_cred) kcred;
    *expiration = kcred->times.endtime;

    /*
     * Save a lock/unlock on most calls after the first.
     */
    
    if (sec__krb_key_seed == NULL) {
        LOCK_IDENTITY();
	LOCK_RANDOM();
        if (sec__krb_key_seed == NULL)
        {
            sec__krb_key_seed = (mit_des_random_key_seed *)
                malloc (sizeof (mit_des_random_key_seed));

            /*
             * !!! we should really initialize this with the TGT key;
             * however that's not amazingly easy to do.
             */

            mit_des_init_random_number_generator (kcred->keyblock.contents,
                sec__krb_key_seed);
        }
	UNLOCK_RANDOM();
        UNLOCK_IDENTITY();
    }

    return rpc_s_ok;
}

error_status_t sec_krb_get_name (ccache, client_name)
    sec_krb_ccache ccache;
    sec_krb_parsed_name *client_name;
{
    krb5_ccache kcc = (krb5_ccache) ccache;
    krb5_principal client;
    error_status_t st;
    
    sec_krb_parsed_name_free(client_name);
    
    st = krb5_cc_get_principal (kcc, &client);
    if (st == 0) {
        *client_name = (sec_krb_parsed_name) client;
    }
    return st;
}

/*
 * !!! We need to deal with key types in default retrieval (keytab) path 
 */

krb5_error_code sec_krb_keyproc 
(
    krb5_pointer ptr,
    krb5_principal server,
    krb5_keytype keytype,
    krb5_kvno kvno,
    krb5_keyblock **keyp
)
{
    error_status_t st, xst;
    krb5_keyblock *key;
    int i;
    unsigned_char_p_t server_global;

#ifdef DEBUG
    /* We don't support svr reg both via keytable and u2u, so check 1st one */
    assert(sec__krb_server_type == sec_krb_svr_type_ktab);
#endif

    /*
     * Convert server to global name.  This can be done quickly..
     */
    st = sec_krb_unparse_name (server, &server_global);
    if (st != rpc_s_ok)
        return st;

    LOCK_IDENTITY();

    for (i=0; i<sec__n_idents; i++) {
        sec_krb_server_t *id = &sec__serv_id_table[i];

        if (strcmp((char *)server_global, (char *)id->server_global) == 0) {
            /* !!! this may not be right allocator here.. */
            rpc_string_free (&server_global, &xst);
            
            if (id->get_key_func) {
                rpc_auth_key_retrieval_fn_t tmp_get_key_func = id->get_key_func;
                unsigned_char_p_t   tmp_server_string;
                krb5_pointer        tmp_arg = id->arg;
                tmp_server_string = rpc_stralloc(id->server_string);

                UNLOCK_IDENTITY();
                tmp_get_key_func (tmp_arg, tmp_server_string, 
                                  (unsigned32) keytype, (unsigned32) kvno,
                                  (void **)keyp, &st);
                rpc_string_free(&tmp_server_string, &xst);
                return st;
            }
            else {
                krb5_keytab_entry ktentry;

                if (!st)
                {
                    st = krb5_kt_get_entry (id->keytab, server,
                        kvno, &ktentry);
                    if (st == 0) {
                        krb5_copy_keyblock (&ktentry.key, keyp);
                        krb5_kt_free_entry(&ktentry);
                    }
                }
                UNLOCK_IDENTITY();
                return st;
            }
        }
    }
    UNLOCK_IDENTITY();
    rpc_string_free (&server_global, &xst);    
    return KRB5KRB_AP_WRONG_PRINC;
}

/* 
 * gen_fake_v1_1_cred_handle
 *
 * Construct v1_1 cred handle from v1_0 information.
 */

void  gen_fake_v1_1_cred_handle (
    sec_id_pac_t            *v1_pac,
    sec__cred_auth_info_t   *authP,
    unsigned32              *expiration,	
    rpc_authz_cred_handle_t *cred_handle,
    sec_bytes_t             *raw_epac_setP,
    error_status_t          *stp
)
{
    sec_bytes_t           deleg_info;
    sec_v1_1_authz_data_t v1_1_ad;
    
    CLEAR_STATUS(stp);
    memset(&deleg_info, 0, sizeof(deleg_info));
    memset(&v1_1_ad, 0, sizeof(v1_1_ad));
    uuid_create_nil(&v1_1_ad.session_id, stp); 
    if (BAD_STATUS(stp)) {
	return;
    }

    /*
     * There is a pac_to_epac conversion routine buried in the 
     * sec_login code, so we'll just make use of it here.
     */
    if ((authP->authz_svc == rpc_c_authz_dce) && (v1_pac != NULL)) {
	
	pac_to_epac_chain_bytes(v1_pac, &deleg_info, 0, NULL, 
	    sec_id_deleg_type_none, sec_id_compat_mode_initiator, stp);

        if (GOOD_STATUS(stp) && raw_epac_setP) {
	    *raw_epac_setP = deleg_info;
	}
    }

    if (GOOD_STATUS(stp) && (cred_handle != NULL)) {
	*stp = sec__cred_create_authz_handle(authP, &deleg_info, 
					     v1_1_ad.deleg_tokens,
					     v1_1_ad.seals, cred_handle);
	if (GOOD_STATUS(stp) && (authP->authz_svc == rpc_c_authz_dce)) {
	    sec__cred_set_session_info(cred_handle, &v1_1_ad.session_id, 
				       (sec_timeval_sec_t *)expiration, stp);
	}
    }

    if (deleg_info.bytes && raw_epac_setP == NULL) {
	sec_encode_buffer_free(free, &deleg_info.bytes);
    }

    sec_encode_v1_1_authz_data_free(free, &v1_1_ad);
}
	    
 
/*
 * Given key retrieval function and argument, try to get the key table
 * we want to use for this combination.
 */

static error_status_t get_keytab(get_key_func, arg, keytab)
    rpc_auth_key_retrieval_fn_t get_key_func;
    void *arg;
    krb5_keytab *keytab;
{
    error_status_t st;
    
    if (get_key_func != NULL)
    {
        *keytab = NULL;
        return error_status_ok;
    }

    /*
     * If we don't know what the default table is, find out.
     */
    if (sec__default_keytab == NULL)
        st = krb5_kt_default(&sec__default_keytab);
    else
        st = rpc_s_ok;

    if (st == 0 && arg != NULL) {
        char *name;
        int  prefix_len;

        /*
         * Filename given already.
         * !!! could scan existing table for an equal arg and just return
         * a copy, saving us some work if using multiple principals in
         * same non-standard keytab.
         */
        if ((st = krb5_kt_resolve(arg, keytab)) == KRB5_KT_BADNAME) {
            prefix_len = strlen(sec__default_keytab->ops->prefix);
            name = malloc(prefix_len + strlen((char *) arg) + 2);
            if (name != NULL) {
                strcpy(name, sec__default_keytab->ops->prefix);
                name[prefix_len] = ':';
                strcpy(&name[prefix_len+1], (char *) arg);

                st = krb5_kt_resolve (name, keytab);
                free(name);
            } else {
                st = sec_s_no_memory;
            }
        }
        return st;
    }

    /*
     * Use default key table.
     */
    *keytab = sec__default_keytab;
    return st;
}

/*
 * Convert name to "global" form.
 */

error_status_t sec_id_globalize_name (name, global_name)
    unsigned_char_p_t name;     /* input: string form */
    unsigned_char_p_t *global_name; /* output: DCE global name */
{
    error_status_t st;
    int name_len;
    unsigned_char_p_t global_ret;
    
    *global_name = NULL;

    /*
     * Allow kerberos-style names through; convert to global name form
     */
    if (strchr((char *)name, '@') != 0)
    {
        krb5_principal princ;
        st = krb5_parse_name((char *)name, &princ);
        if (st == rpc_s_ok)
        {
            st = sec_krb_unparse_name(princ, global_name);
            krb5_free_principal(princ);
        }
        return st;
    }
    /*
     * If name is already global, just return a copy of it
     */
    if (name[0] == '/')
    { 
        if (strncmp((char *)name, CELL_DIR_ROOT, CELL_DIR_ROOT_LEN) != 0)
        {
            unsigned char *tname;

            rpc_ns_entry_expand_name (rpc_c_ns_syntax_dce, name, &tname, &st);
            if (st != error_status_ok) {
                *global_name = NULL;
                return st;
            }
            *global_name = tname;
            return error_status_ok;
        }
        else
        {
            name += CELL_DIR_ROOT_LEN;
        }
    }
    
    if (sec__local_cell_name == NULL)
    {
        dce_cf_get_cell_name (&sec__local_cell_name, &st);
        if (st != rpc_s_ok)
            return st;
        sec__local_cell_len = strlen(sec__local_cell_name);
    }

    /*
     * Allocate space for strlen(name) + strlen(sec__local_cell_name) + 2.
     */

    name_len = strlen((char *)name);
    global_ret = malloc (name_len +  sec__local_cell_len + 2);
    if (global_ret == NULL)
    {
        return sec_s_no_memory;
    }
        
    memcpy (global_ret, sec__local_cell_name, sec__local_cell_len);
    global_ret[sec__local_cell_len] = '/';
    memcpy (global_ret + sec__local_cell_len + 1, name, name_len+1);

    *global_name = global_ret;

    return error_status_ok;
}


error_status_t sec_krb_inq_server (
    unsigned_char_p_t server_princ_name, 
    rpc_auth_key_retrieval_fn_t *get_key_func, 
    void **arg
)
{
    int i;
    
    LOCK_IDENTITY();
    for (i=0; i< sec__n_idents; i++) 
    {
        if (strcmp ((char *)server_princ_name,
                    (char *)sec__serv_id_table[i].server_string) == 0) 
	{
	    *get_key_func = sec__serv_id_table[i].get_key_func;
	    *arg = sec__serv_id_table[i].arg;
	    UNLOCK_IDENTITY();
	    return error_status_ok;
	}
    }
    UNLOCK_IDENTITY();
    return KRB5KRB_AP_WRONG_PRINC;
}	

/*
 * Register a principal and information on where to get its keys.
 * If we re-register an existing principal, just replace the existing
 * registration information.
 */

error_status_t sec_krb_register_server (server_princ_name, get_key_func, arg)
    unsigned_char_p_t                  server_princ_name;
    rpc_auth_key_retrieval_fn_t        get_key_func;
    void                               *arg;
{
    unsigned_char_p_t server_global;
    error_status_t st, xst;
    krb5_keytab keytab;
    int i;
    sec_krb_server_t *nid;

    LOCK_IDENTITY();

    /* We don't support both keytab registration and u2u, so check first */
    if (sec__krb_server_type == sec_krb_svr_type_u2u) {
        UNLOCK_IDENTITY();
        return sec_s_svr_type_conflict;
    }

    for (i=0; i<sec__n_idents; i++) {
        if (strcmp ((char *)server_princ_name,
                    (char *)sec__serv_id_table[i].server_string) == 0) {
            st = get_keytab(get_key_func, arg, &keytab);
            if (st == error_status_ok) {
                /*
                 * Already registered; replace entry.
                 */
                sec__serv_id_table[i].get_key_func = get_key_func;
                sec__serv_id_table[i].arg = arg;
                sec__serv_id_table[i].keytab = keytab;
            }
            UNLOCK_IDENTITY();
            return st;
        }
    }

    UNLOCK_IDENTITY();

    st = get_keytab(get_key_func, arg, &keytab);

    LOCK_IDENTITY();

    if (st == rpc_s_ok)
        st = sec_id_globalize_name(server_princ_name, &server_global);

    if (st == rpc_s_ok)
    {
        /*
         * !!! we don't detect the case where two threads call
         * sec_krb_register_server in parallel; same name gets
         * listed twice in table.
         */
        nid = (sec_krb_server_t *)malloc (sizeof(sec_krb_server_t) * (sec__n_idents + 1));

        memcpy (nid, sec__serv_id_table, sizeof(sec_krb_server_t) * sec__n_idents);
        free(sec__serv_id_table);

        /*
         * Resolve the keytab here if at all possible to save
         * ourselves significant effort later on.
         */

        sec__serv_id_table = nid;
        
        sec__serv_id_table[sec__n_idents].server_string =
            rpc_stralloc(server_princ_name);
        sec__serv_id_table[sec__n_idents].server_global = server_global;
        sec__serv_id_table[sec__n_idents].get_key_func = get_key_func;
        sec__serv_id_table[sec__n_idents].arg = arg;
        sec__serv_id_table[sec__n_idents].keytab = keytab;

        keytab = NULL;
        sec__n_idents++;

        if (get_key_func != NULL) {
            krb5_keyblock *k;
            UNLOCK_IDENTITY();
            (*get_key_func)(arg, server_princ_name, KEYTYPE_DES,
                0, (void **)&k, &st);
            LOCK_IDENTITY();
            if (st == rpc_s_ok) {
		if (sec__krb_key_seed == NULL) {
		    sec__krb_key_seed = (mit_des_random_key_seed *)
			malloc (sizeof (mit_des_random_key_seed));
		    if (sec__krb_key_seed == NULL)
		    {
			st = sec_s_no_memory;
		    } else {
			mit_des_init_random_number_generator
			    (k->contents, sec__krb_key_seed);
		    }
		}
		memset(k->contents, '\0', k->length);
		free(k->contents);
		free(k);
            }
        } else {
            sec_passwd_rec_t *keydata;
            sec_passwd_type_t keytype;
            int i;
            /*!!!
             * for now, we know that a single key type (des) is all that's
             * supported, and it's okay to pass the manifest constant for that
             * keytype to the key retrieval function, but when and if multiple
             * key types exist, we'll need to do something other than just fail
             * when we can't use a server key as a random number seed.  We assume
             * the key retrieval function understands that version number 0 means 
             * "current"
             */
            UNLOCK_IDENTITY();
            sec_key_mgmt_get_key (rpc_c_authn_dce_private, arg, server_global, 0, (void **)&keydata, &st);
            LOCK_IDENTITY();

            if (st == rpc_s_ok) {
                        
                for (i = 0;
                     (keytype = keydata[i].key.key_type) != sec_passwd_none;
                     i++)
                {
                    if (keytype == sec_passwd_des)
                    {
                        if (sec__krb_key_seed == NULL) { 
                            sec__krb_key_seed = (mit_des_random_key_seed *)
                                malloc (sizeof (mit_des_random_key_seed));
                            if (sec__krb_key_seed == NULL)
                            {
                                st = sec_s_no_memory;
                            } else {
                                mit_des_init_random_number_generator
                                    (keydata[i].key.tagged_union.des_key,
                                        sec__krb_key_seed);
                            }
                        }
                        break;
                    }
                }
                sec_key_mgmt_free_key(keydata, &xst);
                if ((st == rpc_s_ok) &&
                    (sec__krb_key_seed == NULL))
                {
                    /* !!! this status is not quite right */
                    st = sec_key_mgmt_e_key_unavailable; 
                }
            }
        }
	if (BAD_STATUS(&st)) {
	    sec__n_idents--;
	    rpc_string_free(&sec__serv_id_table[sec__n_idents].server_string, &xst);
	    free(sec__serv_id_table[sec__n_idents].server_global);
	}
    }
    UNLOCK_IDENTITY();

    return st;
}

static pthread_once_t sec_krb_inited = pthread_once_init;

static void sec__krb_init ()
{
    pthread_mutex_init (&sec__id_lock, pthread_mutexattr_default);
    pthread_mutex_init (&sec__random_lock, pthread_mutexattr_default);
    sec__serv_id_table = (sec_krb_server_t *)malloc (sizeof *sec__serv_id_table);
}

void sec_krb_init ()
{
    pthread_once(&sec_krb_inited, sec__krb_init);
}

void sec_des_cbc_cksum (plaintext, cksum, length, key, ivec)
    sec_des_block *plaintext;
    sec_des_block *cksum;
    signed32 length;
    sec_des_key_schedule key;
    sec_des_block *ivec;
{
    mit_des_cbc_cksum ((krb5_octet *)plaintext,
        (krb5_octet *)cksum,
        length,
        key,
        (krb5_octet *)ivec);
}

void sec_des_cbc_encrypt (inblocks, outblocks, length, key, ivec, direction)
    sec_des_block                      *inblocks;
    sec_des_block                     *outblocks;
    signed32                           length;
    sec_des_key_schedule               key;
    sec_des_block                      *ivec;
    signed32                           direction;
{
    mit_des_cbc_encrypt ((krb5_octet *)inblocks,
        (krb5_octet *)outblocks, length, key,
        (krb5_octet *)ivec, direction);
}

error_status_t sec_des_key_sched (key, sched)
    sec_des_key *                              key;
    sec_des_key_schedule                      sched;
{
    int code = mit_des_key_sched ((unsigned char *)key, sched);
    switch (code) {
    case 0:
        return error_status_ok;
    case -1:
        return KRB5DES_BAD_KEYPAR;
    case -2:
        return KRB5DES_WEAK_KEY;
    default:
        return code;
    }
}

error_status_t sec_des_new_random_key (key)
    sec_des_key *key;
{
    if (!sec__krb_key_seed)
        return sec_s_no_key_seed;
    LOCK_RANDOM();
    mit_des_new_random_key (key->bits, sec__krb_key_seed);
    UNLOCK_RANDOM();
    return error_status_ok;
}

error_status_t sec_des_generate_random_block (key)
    sec_des_block *key;
{
    if (!sec__krb_key_seed)
        return sec_s_no_key_seed;
    LOCK_RANDOM();
    mit_des_generate_random_block (key->bits, sec__krb_key_seed);
    UNLOCK_RANDOM();
    return error_status_ok;
}

#ifdef DEBUG_PICKLE
static void sec_id_pac_print (pac)
    sec_id_pac_t *pac;
{
    int i;
    
    printf("uid %d, gid %d, ngroups %d:",
        pac->principal.uuid.time_low,
        pac->group.uuid.time_low,
        pac->num_groups);
    for (i=0; i<pac->num_groups; i++) {
        printf (" %d", pac->groups[i].uuid.time_low);
    }
    printf("\n");
}
#endif

error_status_t sec_krb_get_server (name)
    unsigned char **name;
{
    error_status_t error;
    
    LOCK_IDENTITY();
    if (sec__n_idents <= 0)
    {
        error = sec_s_none_registered;
    } else {
        *name = sec__serv_id_table[0].server_global;
        error = rpc_s_ok;
    }
    UNLOCK_IDENTITY();
    return error;
}


error_status_t sec_krb_get_server_name_tgt (
    unsigned_char_p_t   *name,
    unsigned32          *tgt_len,
    unsigned_char_p_t   *tgt_data
)
{
    error_status_t error;
    
    LOCK_IDENTITY();
    if ((sec__n_idents <= 0) ||
	(sec__krb_server_type != sec_krb_svr_type_u2u)) {
        error = sec_s_none_registered;
    } else {
        *name = sec__serv_id_table[0].server_global;

	/* Update our idea of the current TGT */
        error = sec_krb_u2u_check_creds();
	*tgt_len = sec__serv_id_table[0].curr_creds->ticket.length;
	*tgt_data = (unsigned_char_p_t)
            sec__serv_id_table[0].curr_creds->ticket.data;
    }
    UNLOCK_IDENTITY();
    return error;
}


/*
 * Glue to MD[45] routines.
 *
 * Start a MD5 checksum.
 */

void sec_md_begin(state)
    sec_md_ptr state;
{
    MD5Init((MD5_CTX *)state);
}

/*
 * Add bits to MD5 checksum.
 */
void sec_md_update (state, data, len)
    sec_md_ptr state;
    byte_p_t data;
    unsigned32 len;
{
    MD5Update((MD5_CTX *)state, data, len);
}

/*
 * Terminate an MD5 checksum.
 */

void sec_md_final (state)
    sec_md_ptr state;
{
    MD5Final((MD5_CTX *)state);
}


/*
 * S E C _ K R B _ S T A T U S _ W I R E _ T O _ R P C
 */

/*
 * This routine will take an error code from a decoded KRB_ERROR
 * message or authentication level "none" error message and convert
 * it to an RPC status code. 
 */
error_status_t sec_krb_status_wire_to_rpc(code)

unsigned32      code;

{
    switch (code) 
    {
        case KRB_AP_ERR_BAD_INTEGRITY: return (rpc_s_auth_bad_integrity);
        case KRB_AP_ERR_TKT_EXPIRED: return (rpc_s_auth_tkt_expired);
        case KRB_AP_ERR_TKT_NYV: return (rpc_s_auth_tkt_nyv);
        case KRB_AP_ERR_REPEAT: return (rpc_s_auth_repeat);
        case KRB_AP_ERR_NOT_US: return (rpc_s_auth_not_us);
        case KRB_AP_ERR_BADMATCH: return (rpc_s_auth_badmatch);
        case KRB_AP_ERR_SKEW: return (rpc_s_auth_skew);
        case KRB_AP_ERR_BADADDR: return (rpc_s_auth_badaddr);
        case KRB_AP_ERR_BADVERSION: return (rpc_s_auth_badversion);
        case KRB_AP_ERR_MSG_TYPE: return (rpc_s_auth_msg_type);
        case KRB_AP_ERR_MODIFIED: return (rpc_s_auth_modified);
        case KRB_AP_ERR_BADORDER: return (rpc_s_auth_badorder);
        case KRB_AP_ERR_BADKEYVER: return (rpc_s_auth_badkeyver);
        case KRB_AP_ERR_NOKEY: return (rpc_s_auth_nokey);
        case KRB_AP_ERR_MUT_FAIL: return (rpc_s_auth_mut_fail);
        case KRB_AP_ERR_BADDIRECTION: return (rpc_s_auth_baddirection);
        case KRB_AP_ERR_METHOD: return (rpc_s_auth_method);
        case KRB_AP_ERR_BADSEQ: return (rpc_s_auth_badseq);
        case KRB_AP_ERR_INAPP_CKSUM: return (rpc_s_auth_inapp_cksum);
        case KRB_ERR_FIELD_TOOLONG: return (rpc_s_auth_field_toolong);
        case nca_s_out_args_too_big: return (rpc_s_credentials_too_large);
        case nca_s_unsupported_authn_level: return (rpc_s_unsupported_authn_level);
        case nca_s_invalid_checksum: return (rpc_s_invalid_checksum);
        case nca_s_invalid_crc: return (rpc_s_invalid_crc);
        default:
        {
            return (rpc_s_unknown_reject);
        }
    }
}


/*
 * S E C _ K R B _ S T A T U S _ T O _ W I R E
 */

/*
 * This routine will convert a KRB5 status code to a KRB_ERROR
 * status code or rpc status code to an "architected" status code.
 * Also indicate whether the provided status code was a kerberos or
 * RPC status code.
 */
unsigned32 sec_krb_status_to_wire(code, krb_error)

unsigned32      code;
boolean32       *krb_error;

{
    *krb_error = false;
    switch (code)
    {
        case rpc_s_credentials_too_large:   return (nca_s_out_args_too_big);
        case rpc_s_unsupported_authn_level: return (nca_s_unsupported_authn_level);
        case rpc_s_invalid_checksum:        return (nca_s_invalid_checksum);
        case rpc_s_invalid_crc:             return (nca_s_invalid_crc);
        case KRB5KRB_AP_ERR_BAD_INTEGRITY:
        case KRB5KRB_AP_ERR_TKT_EXPIRED:
        case KRB5KRB_AP_ERR_TKT_NYV:
        case KRB5KRB_AP_ERR_REPEAT:
        case KRB5KRB_AP_ERR_NOT_US:
        case KRB5KRB_AP_ERR_BADMATCH:
        case KRB5KRB_AP_ERR_SKEW:
        case KRB5KRB_AP_ERR_BADADDR:
        case KRB5KRB_AP_ERR_BADVERSION:
        case KRB5KRB_AP_ERR_MSG_TYPE:
        case KRB5KRB_AP_ERR_MODIFIED:
        case KRB5KRB_AP_ERR_BADORDER:
        case KRB5KRB_AP_ERR_BADKEYVER:
        case KRB5KRB_AP_ERR_NOKEY:
        case KRB5KRB_AP_ERR_MUT_FAIL:
        case KRB5KRB_AP_ERR_BADDIRECTION:
        case KRB5KRB_AP_ERR_METHOD:
        case KRB5KRB_AP_ERR_BADSEQ:
        case KRB5KRB_AP_ERR_INAPP_CKSUM:
        case KRB5KRB_ERR_GENERIC:
        case KRB5KRB_ERR_FIELD_TOOLONG:     
        {
            *krb_error = true;
            return (code-ERROR_TABLE_BASE_krb5);
        }
        default:                            return (nca_s_unspec_reject);
        
    }
}

/*
 * S E C _ K R B _ B U I L D _ R E P _ C O M M O N
 * 
 * This routine is used by GSSAPI and CN RPC only.
 * It is not used by DG.
 */

error_status_t sec_krb_build_rep_common  (
	unsigned32 authn_level,	/* in */
	krb5_keyblock *key,	/* in */
	sec_krb_msgstamp *msgstamp, /* in */
	sec_krb_message * rep_message /* out */
    )
{
    krb5_ap_rep                 ap_rep;
    krb5_error_code             code;
    krb5_ap_rep_enc_part        repl;
    krb5_data                   outbuf;

    /*
     * Build a KRB_AP_REP message if the authentication level is
     * anything other than "none".
     */
    if (authn_level != rpc_c_authn_level_none)
    {
        /*
         * Set up the output argument which will contain the formatted
         * message.
         */
        outbuf.data = NULL;
        outbuf.length = 0;

        /*
         * Set up information needed to format the KRB_AP_REP.
         */
        repl.ctime = (krb5_timestamp) msgstamp->ctime;
        repl.cusec = (krb5_int32) msgstamp->cusec;
        repl.subkey = 0;
        repl.seq_number = msgstamp->seq_number;
        
        code = krb5_mk_rep (&repl, key, &outbuf);
        rep_message->data = (unsigned char *)outbuf.data;
        rep_message->length = outbuf.length;
    }
    else
    {
        code = rpc_s_ok;
        rep_message->data = (unsigned char *)NULL;
        rep_message->length = 0;
    }
    return (code);
    
}


error_status_t sec_krb_decode_rep_common (
    unsigned32 authn_level,	/* in */
    sec_krb_message *message, /* in */
    krb5_keyblock *key,	/* in */
    sec_krb_msgstamp *msgstamp /* in/out */
) 
{
    krb5_data                   krb_message;
    krb5_ap_rep_enc_part        *repl;
    krb5_error_code             code;
    krb5_error                  *dec_error;
    unsigned32                  code_from_server;
    rpc_mp_t                    ump = NULL;

    krb_message.data = (char *) message->data;
    krb_message.length = message->length;
    if (msgstamp != NULL) 
    {
	msgstamp->ctime = 0;
	msgstamp->cusec = 0;
	msgstamp->seq_number = 0;
    }

    /*
     */
    if (authn_level != rpc_c_authn_level_none)
    {

        repl = (krb5_ap_rep_enc_part *)NULL;
        code = krb5_rd_rep (&krb_message, key, &repl);
        if (repl != (krb5_ap_rep_enc_part *)NULL)
        {
	    if (msgstamp != NULL) 
	    {
		msgstamp->ctime = repl->ctime;
		msgstamp->cusec = repl->cusec;
		msgstamp->seq_number = repl->seq_number;
	    }
            krb5_free_ap_rep_enc_part(repl);
        }
        if (code == KRB5KRB_AP_ERR_MSG_TYPE)
        {
            /*
             * A KRB_ERROR message could be in the reply if an error
             * occured at the server.
             */
            code = krb5_rd_error (&krb_message, &dec_error);
            if (code == 0)
            {
                code_from_server = (unsigned32) dec_error->error;
                krb5_free_error (dec_error);
                return (sec_krb_status_wire_to_rpc (code_from_server));
            }
        }
    }
    else
    {
        if (message->length)
        {
            /*
             * If the authentication level is none the status code was used
             * as the message in big endian format.
             *
             * Unmarshall the status code and convert it to a local
             * value.
             */
            ump = (rpc_mp_t) message->data;
            rpc_convert_be_long_int_unalign(ump, code_from_server);
            return (sec_krb_status_wire_to_rpc (code_from_server));
        }
        else
        {
            /*
             * If the authentication level is none and there was no
             * error no message was encoded.
             */
            return (rpc_s_ok);
        }
    }
    return (code);
    
}

error_status_t sec_krb_build_message_common (
    sec_krb_ccache ccache,	/* in */
    sec_krb_cred cred,	/* in */
    krb5_checksum *send_cksum, /* in */
    unsigned32 authz_proto,	/* in */
    unsigned32 authz_fmt,	/* in */
    sec_krb_msgstamp *msgstamp,	/* in/out */
    sec_krb_message *message /* out */
)
{
    krb5_data ap_req;
    krb5_ccache kccache;
    krb5_creds *kcred = (krb5_creds *) cred;
    char out_data[1024];
    rpc_mp_t mp;
    error_status_t   st;
    krb5_authenticator authent;
    sec_bytes_t   epac_set = {0, NULL};
    
    CLEAR_STATUS(&st);

    sec_authn_pvt_get_krb_ccache(ccache, authz_proto, &kccache, &st);
    if (BAD_STATUS(&st)) {
        return st;
    }
    /* krb5_mk_req_extended calls krb5_get_credentials(), not
     * krb_get_credentials_dce(), because we're passing in the credentials
     * needed for the request, so it will never need to acquire them.
     */
    st = krb5_mk_req_extended (
	0,
	send_cksum,
	krb5_kdc_default_options,
	msgstamp->seq_number,
	(krb5_keyblock *)NULL,
	kccache,
	kcred,
	&authent,
	&ap_req);
    
    if (BAD_STATUS(&st)) 
    {
	return st;
    }

    msgstamp->ctime = authent.ctime;
    msgstamp->cusec = authent.cusec;
    
    if ((authz_proto == rpc_c_authz_dce) && 
	(authz_fmt >= SEC_AUTHZ_FMT_V1_1)) 
    {
	boolean32  found;
	
	/* 
	 * Our caller has requested a format >= SEC_AUTHZ_FMT_V1_1
	 * but that doesn't mean we'll be able to give it to
	 * them.  We are running 1.1 libraries and so is the server
	 * we're connected to, but the server's cell may be running 
	 * pre-1.1 security servers.  In that case, we won't be able
	 * to obtain 1.1 security information from that cell's
	 * security service. The caller will have to settle for a
	 * SEC_AUTHZ_FMT_V1_0 security message.
	 */
	
	/*
	 * The additional info we need (the encoded delegation chain),
	 * if available at all, will be in DCE authdata in the creds, where
	 * the sec_krb_get_cred() routine put it when it generated
	 * the credentials.
	 */
	found = priv_krb_util_parse_v1_1_ccache_authdata(malloc_shim, free,
							 kcred->authdata,
							 &epac_set, &st);

	if (BAD_STATUS(&st)) 
        {
	    free(ap_req.data);
	    return st;
	}

	if (!found || (epac_set.num_bytes == 0) || (epac_set.bytes == NULL)) {
	    /* 
	     * SEC_AUTHZ_FMT_V1_0 is the best we can do, no matter
	     * what our caller asked for. 
	     */
	    authz_fmt = SEC_AUTHZ_FMT_V1_0;
	}
    }
	
    if ((authz_proto == rpc_c_authz_dce) && 
	(authz_fmt >= SEC_AUTHZ_FMT_V1_1)) 
    {
	size_t aplen, fulllen;
	/* calculate total length */
	
	aplen = ap_req.length;
	aplen = (aplen + 7) & ~7;
	fulllen = aplen + 8 + epac_set.num_bytes;
	
	/* allocate buffer */
	message->length = fulllen;
	message->data = malloc (fulllen);
	if (message->data != NULL) 
	{	
	    /* concatenate into message */
	    memset(message->data, 0, fulllen);
	    memcpy(message->data, (void *)ap_req.data, ap_req.length);
	    mp = (rpc_mp_t) message->data+aplen;
	    rpc_marshall_be_long_int (mp, SEC_AUTHZ_FMT_V1_1);
	    rpc_advance_mp(mp, 4);
	    rpc_marshall_be_long_int (mp, epac_set.num_bytes);
	    rpc_advance_mp(mp, 4);
	    memcpy(mp, epac_set.bytes, epac_set.num_bytes);
	} else {
	    SET_STATUS(&st, sec_s_no_memory);
	}

	free(ap_req.data);
	sec_encode_buffer_free(free, &epac_set.bytes);
	return st;

    } else {
	message->data = (unsigned char *)ap_req.data;
	message->length = ap_req.length;
    }

    return st;
}


/* sec_krb_decode_message_common
 *
 * Notes on the compatibility situation
 *
 * "We" are the server security library
 *
 * If our client is linked with a pre-1.1 libdce, there can 
 * be only a v1 PAC in the authdata because that is all
 * such a client could ask for.
 *
 * If the client is linked with a 1.1 libdce then many things
 * are possible.
 * 
 * I. The client is a client in our local cell
 *
 *    A. Our cell is running pre-1.1 security servers.
 *
 *       1. The authdata will always contain a v1 PAC, because 
 *          that is all that is available from the security 
 *          server.
 *
 *       2. The authdata will never contain v1.1 authorization
 *          data because no such data are available from a pre-1.1
 *          security service.
 *       
 *       3. There will be no epac chain appended to the kerberos data
 *          in message, since it could not possibly be of any use
 *          without a seal to verify its integrity.
 * 
 *    B. Our cell is running 1.1 security servers.
 *
 *       1. Whether or not the authdata contains a v1_pac
 *          depends on the compatibility mode in effect.
 *   
 *          a. compat mode = none: authdata does not contain a V1 PAC.
 *
 *          b. compat mode != none: authdata contains a V1 PAC.
 *
 *       2. The authdata will always contain v1_1 authorization
 *          data.
 *
 *       3. There will always be an epac chain appended to the
 *          kerberos data in the message.
 *
 * II. The client is from a foreign cell running pre-1.1 security
 *     servers.
 *
 *     Repeat (IA).
 *
 * III. The client is from a foreign cell running 1.1 security servers.
 *
 *    A. Our local cell is running pre-1.1 security servers
 *
 *       The client has been able to successfully cross into
 *       our cell because the client enabled compatibility.
 *       If the client had turned compatibility off, they would
 *       never have gotten past our pre-1.1 priv server.
 *
 *       Repeat (IA).
 *
 *    B. Our local cell is running 1.1 security servers
 *
 *       Repeat (IB).
 *
 * The lesson in all this is that it is not possible to
 * predict, based on knowledge of the RPC runtime protocol used
 * to transmit the authentication message from the client
 * process to the server process, the format of the security
 * information in the authentication message.  That can 
 * only be ascertained by looking into the message itself.
 */
error_status_t sec_krb_decode_msg_common_frags (
    sec_krb_message *message,			/* in */
    unsigned32 authz_fmt,			/* in */
    sec_krb_parsed_name *server_name,		/* out */
    unsigned char **client_name,		/* out */
    sec_id_pac_t *client_pac,			/* out */
    sec_bytes_t *raw_epac_setP,			/* out */
    rpc_authz_cred_handle_t *client_creds,	/* out */
    unsigned32 *expiration,			/* out */
    sec_krb_msgstamp *timestamp,		/* out */
    krb5_tkt_authent **authentP,		/* out */
    unsigned32 *authz_proto			/* out */
) 
{
    krb5_data              ap_req;
    krb5_ap_req            *request;
    error_status_t         st;
    krb5_tkt_authent       *authent;
    sec__cred_auth_info_t  auth_info, *authP;
    idl_pkl_t              *pickled_pac = NULL;
    sec_v1_1_authz_data_t  v1_1_ad;
    unsigned32             ad_parts = 0;
    unsigned char          *tmpptr;
    unsigned char          *eptr;
    rpc_mp_t               ump = NULL;
    int                    tmplen;
    error_status_t         ignore_st;
    sec_bytes_t            tmp_epac_set;
    sec_id_pac_t           tmp_pac;
 
    *authentP = NULL;
    authP = &auth_info;
    memset(authP, 0, sizeof(sec__cred_auth_info_t));
    memset(&v1_1_ad, 0, sizeof(v1_1_ad)); 
    if (client_creds) {
	memset(client_creds, 0, sizeof(*client_creds));
    }
    if (client_pac) {
	memset(client_pac, 0, sizeof(*client_pac));
    } else {
	memset(&tmp_pac, 0, sizeof(tmp_pac));
    }

    authP->authn_svc = rpc_c_authn_dce_secret; 
    authP->handle_type = sec__cred_handle_type_server;

    if (authz_fmt >= SEC_AUTHZ_FMT_V1_1) 
    {
	/* 
	 * Compute the length of the asn.1 prefix
	 * (the KRB5_AP_REQ protocol message)
	 */
	tmpptr = message->data; 
	eptr = tmpptr + message->length;
	ap_req.length = sec_asn1len(tmpptr, eptr);
    } else {
	ap_req.length = message->length;
    }

    if (ap_req.length == -1)
    {
	/*
	 * Treat failure to compute a legitimate asn.1 message length
	 * as a KRB5KRB_AP_ERR_MSG_TYPE error.  This will cause the request
	 * to be treated as unauthenticated.
	 */
	st = KRB5KRB_AP_ERR_MSG_TYPE;
    } else {
        ap_req.data = (char *)message->data;

	/* Normal keytab based request? */
	if (sec__krb_server_type != sec_krb_svr_type_u2u) {
            st = krb5_rd_req (&ap_req, (krb5_const_principal) NULL,
                    (krb5_address *) NULL, (krb5_const_pointer)NULL,
                    sec_krb_keyproc, (krb5_pointer)NULL,
                    (krb5_rcache) sec__replay_cache, authentP);
        } else {
            /* or user to user based request */

	    /* Check/refresh our copy of the TGT for decoding purposes */
            LOCK_IDENTITY();
            st = sec_krb_u2u_check_creds();

	    if (st == error_status_ok) {
                /* Attempt to decode the message with current creds */
                st = krb5_rd_req (&ap_req, (krb5_const_principal) NULL,
                        (krb5_address *) NULL, (krb5_const_pointer)NULL,
                        sec_krb_u2u_keyproc,
                        (krb5_pointer) sec__serv_id_table[0].curr_creds,
                        (krb5_rcache) sec__replay_cache, authentP);
                if ((st == KRB5KRB_AP_ERR_BAD_INTEGRITY) &&
                    (sec__serv_id_table[0].prev_creds)) {

                    /* Attempt to decode the message with previous creds */
                    st = krb5_rd_req (&ap_req, (krb5_const_principal) NULL,
                            (krb5_address *) NULL, (krb5_const_pointer)NULL,
                            sec_krb_u2u_keyproc,
                            (krb5_pointer) sec__serv_id_table[0].prev_creds,
                            (krb5_rcache) sec__replay_cache, authentP);
                }
            }
	    UNLOCK_IDENTITY();
        }
    }

    authent = *authentP;

    if (st == KRB5KRB_AP_ERR_MSG_TYPE)
    {
        authP->authenticated = false;
        authP->server_princ_name = NULL;

        /* !!! fill in server name? */
        /* this looks like it could be a level_none request instead.. */
        switch (message->data[0]) {
        case rpc_c_authz_name:
            message->data[message->length-1] = '\0';

            *client_name = rpc_stralloc (&message->data[1]);

            *authz_proto = rpc_c_authz_name;
	    *authentP = NULL;

            authP->authz_svc = *authz_proto;
            authP->authz_info.client_princ_name = *client_name;

            gen_fake_v1_1_cred_handle(NULL, authP, NULL,
				      client_creds, NULL, &st );

            return st;
        case rpc_c_authz_dce:
        {
            idl_pkl_t *pickled_pac = 
                (idl_pkl_t *)&message->data[1];
	    idl_pkl_t *aligned_pickled_pac =
		(idl_pkl_t *)malloc (message->length);
	    memcpy((void *)aligned_pickled_pac, (void *)pickled_pac,
		message->length-1);

            *client_name = NULL;
            sec_id_pac_t_unpickle(aligned_pickled_pac, (void *(*)(unsigned32)) malloc,
				  client_pac ? client_pac : &tmp_pac, &st);
	    
#ifdef DEBUG_PICKLE
            if (GOOD_STATUS(&st)) {
                printf("Unpickled pac:\n");
                sec_id_pac_print (client_pac ? client_pac : &tmp_pac);
            } else {
                printf("Unable to unpickle pac: %x\n", st);
            }
#endif
            /* !!! what if it blew out? */
            *authz_proto = rpc_c_authz_dce;
            authP->authz_svc = *authz_proto;
	    
	    *authentP = NULL;	    
	    
	    if (GOOD_STATUS(&st)) {
		gen_fake_v1_1_cred_handle(client_pac ? client_pac : &tmp_pac,
					  authP,  NULL, client_creds, 
					  raw_epac_setP, &st);
	    }
	    
	    free(aligned_pickled_pac);
	    
	    if (GOOD_STATUS(&st) && !client_pac) {
		sec_id_pac_util_free(&tmp_pac);
	    }

            return st;
        }
        default:
            return sec_s_authz_unsupp;
        }
    }
    if (BAD_STATUS(&st)) {
        return st;
    }


    /*
     * Verify that all the fields match what we're expecting.
     * I moved this chunk here, so we can do gen_fake_v1_1_cred_handle()
     * easier.
     */

    if (server_name) {
        krb5_copy_principal (authent->ticket->server,
            (krb5_principal *)server_name);
    }
    if (client_name) {
        sec_krb_unparse_name (authent->ticket->enc_part2->client,
            client_name);
    }
    *expiration = authent->ticket->enc_part2->times.endtime;
    if (timestamp) 
    {
	timestamp->ctime = (sec_krb_timestamp) authent->authenticator->ctime;
	timestamp->cusec = (unsigned32) authent->authenticator->cusec;
	timestamp->seq_number = (unsigned32) authent->authenticator->seq_number;
    }

    /*
     * By passing an authz_fmt parameter of SEC_AUTHZ_FMT_V1_0
     * the caller is telling us that the auth message was formatted
     * by a v1.0 client so we won't waste any time looking for 
     * any v1_1 info.
     *
     * If the caller passed an authz_fmt parameter of
     * SEC_AUTHZ_FMT_V1_1, then the auth message was
     * formatted by a new client, but that new client may
     * have produced a SEC_AUTHZ_FMT_V1_0 message if that's all
     * the security service in the target cell was able to provide.
     * We check for this condition by looking for additional
     * data beyond the asn.1 kerberos info.  If there is
     * no additional data, then the message is an old one.
     *
     * Finally, the client may be running some future version of
     * DCE security. If the client wants to maintain V1_1
     * compatability, they insert the epac chain at the appropriate 
     * point in the message stream. If they've flushed  V1_1 compatibility 
     * altogether they've put something else here;  something we can't possibly
     * understand.
     */
    if (ap_req.length == message->length)
    { 
	/* It's a V_0 message, no matter what our caller claims. */
	authz_fmt = SEC_AUTHZ_FMT_V1_0;
    } else {
	int ap_req_len = ap_req.length;

	/* read the actual authz format from the message buffer */
	ap_req_len = (ap_req_len+7)&~7;
	ump = (rpc_mp_t)(message->data + ap_req_len);
	rpc_convert_be_long_int_unalign(ump, authz_fmt);
	rpc_advance_mp(ump, 4);
    }

    /*
     * Find out what's actually in the DCE authorization data
     * in the service ticket. If the authz_fmt is SEC_AUTHZ_FMT_V1_1,
     * ask for both a v1 pickled PAC, and v1_1-style authorization data.
     * If the authz_fmt is SEC_AUTHZ_FMT_V1_0, ask only for 
     * a pickled v1 PAC because v1_1 data is useless.
     */
    ad_parts = priv_krb_util_parse_v0_authdata(malloc_shim, free,
			   authent->ticket->enc_part2->authorization_data,
			   (authz_fmt == SEC_AUTHZ_FMT_V1_0) 
			   ? NULL : &v1_1_ad,
			   &pickled_pac, &st);

    if (BAD_STATUS(&st)) 
    {
	goto out;
    }

    /* 
     * If there was DCE authorization data of an unknown type
     * in the ticket, bail out.  This would be the case if we've
     * received a message containing a service ticket which in turn
     * contains DCE authorization data generated by a post 1.1 version
     * of security that has flushed 1.0 and 1.1 compatibility entirely.  
     * It would also be  the case if there were more than one DCE 
     * authorization data entry in the service ticket.
     */
    if (FLAG_SET(ad_parts, priv_ad_part_unknown)) 
    {
	SET_STATUS(&st, sec_s_no_pac); /* !!! better status??? */
	goto out;
    }
    
    /*
     * There are 3 more ad_part flags that might be set.  If the 
     * priv_ad_part_none flag is set, we treat the request as an 
     * authz_name request (the else clause in the following lengthy if 
     * statement).  Otherwise, we treat it as an authz_dce request (the
     * then clause in the following if).
     */
    if (FLAG_SET(ad_parts, priv_ad_part_pac)
    || FLAG_SET(ad_parts, priv_ad_part_v1_1)) 
    { /* authz_dce path */
	krb5_principal  ps_princ;
	boolean32       is_ps;
	
	/*
	 * make certain that the client is the privilege server in the
	 * the local cell, and that the transited path is zero
	 */
	
	if (sec__local_cell_name == NULL)
        {
	    dce_cf_get_cell_name(&sec__local_cell_name, &st);
	    if (BAD_STATUS(&st))
		goto out;
	    sec__local_cell_len = strlen(sec__local_cell_name);
	}
	
	sec_krb_ptgs_parsed_name(sec__local_cell_name, &ps_princ, &st);
	if (GOOD_STATUS(&st))
        {
	    is_ps = krb5_principal_equiv(ps_princ, 
			   authent->ticket->enc_part2->client);
	    krb5_free_principal(ps_princ);   
	}
	
	if (GOOD_STATUS(&st)) 
        {
	    if (!is_ps 
            || authent->ticket->enc_part2->transited.tr_contents.length != 0) 
	    {
		SET_STATUS(&st, sec_s_not_trustworthy);
	    }
	}
	
	if (BAD_STATUS(&st))  
	    goto out;
	
	*authz_proto = rpc_c_authz_dce;
	
	authP->authenticated = true;
	if ((authP->server_princ_name == NULL)
	    && (authent->ticket->server != NULL))
	{
	    error_status_t st;
	    st = sec_krb_unparse_name (authent->ticket->server, 
			       (unsigned char **)&authP->server_princ_name);
	    
	}
	authP->authz_svc = *authz_proto;
	
	if (authz_fmt >= SEC_AUTHZ_FMT_V1_1) 
	{
	    sec_id_seal_t *seals;
	    int i;
 
	    /* 
	     * pull EPAC from end of authenticator.
	     */
	    rpc_convert_be_long_int_unalign(ump, tmplen);
	    rpc_advance_mp(ump, 4);		
	    /* at this point, epac is at *ump */
	    if ((ump + tmplen) > (rpc_mp_t)(message->data + message->length)) 
	    {
		SET_STATUS(&st, sec_authn_s_missing_epac);
		goto out;
	    }
	    tmp_epac_set.bytes = (unsigned_char_p_t) ump;
	    tmp_epac_set.num_bytes = tmplen;

	    /*
	     * Check that we really have an encoded v1_1 epac_set.
	     * There's no sense in proceeding if we don't.
	     */
	    {
		sec_encode_type_t  et;
		
		et = sec_encode_get_encoding_type(tmp_epac_set.num_bytes,
						  tmp_epac_set.bytes);
		if (et != sec_encode_type_epac_set) 		
		{
		    SET_STATUS(&st, sec_authn_s_missing_epac);
		    goto out;
		}
	    }
		
	    /* verify seal before proceeding any further */
	    /* extract seals from v1_1 auth data into seals */

	    seals = v1_1_ad.seals->seals;
	    
	    SET_STATUS(&st, sec_authn_s_no_seal);
	    for (i=0; i<v1_1_ad.seals->num_seals; i++) 
	    {
		if (seals[i].seal_type == sec_id_seal_type_md5) 
		{
		    sec_id_seal_t seal;
		    sec_authz_util_gen_seal(malloc_shim, free,
					    &tmp_epac_set, &seal);
		    if (!sec_authz_util_eq_seal(&seal, &seals[i])) 
		    {
			printf("mismatched seal %d\n", i);
			SET_STATUS(&st, sec_authn_s_bad_seal);
		    } else {
			SET_STATUS(&st, 0);
		    }
		    sec_id_seal_free(free, &seal);
		    break;
		}
	    }
	    if (BAD_STATUS(&st)) 
		goto out;
	    
	    /* 
	     * What to do about the client_pac?  
	     *
	     * If the client provided compatibility info (a pickled PAC)
	     * in the authorization data, we're golden.
	     *
	     * If the client did not provide compatibility info,
	     * we zero out the the client epac (effectively filling
	     * it with uuid_nil entries).  This is for consistency
	     * with what a client process sees after a call to
	     * sec_login_inquire_net_info() on V1_1 credentials
	     * in which compatibility has been turned off (it's
	     * a long story, having to do with a bug in pre-1.1
	     * libraries that requires us to always place a
	     * pickled PAC of some sort in the authdata in the
	     * client's credential cache--otherwise statically
	     * linked pre-1.1 applications that call
	     * sec_login_inquire_net_info on a credential cache
	     * generated by a 1.1 library might crash).
	     */
	    if (client_pac) 
	    {
		if (FLAG_SET(ad_parts, priv_ad_part_pac)) 
		{
		    sec_id_pac_t_unpickle(pickled_pac, 
				      (void *(*)(unsigned32)) malloc,
				      client_pac, &st);
		    free( (char *) pickled_pac); pickled_pac = NULL;
		    if (BAD_STATUS(&st)) 
		    {
			goto out;
		    }
		}
	    }

	    if (client_creds != NULL) 
	    {
		st = sec__cred_create_authz_handle(authP,
						   &tmp_epac_set,
						   v1_1_ad.deleg_tokens,
						   v1_1_ad.seals,
						   client_creds);
		if (GOOD_STATUS(&st)) 
		{
		    sec__cred_set_session_info(client_creds, 
					       &v1_1_ad.session_id,
					       (sec_timeval_sec_t *) expiration,
					       &st);
		}
	    }

	    /* Note for future: this (or the above function) 
	     * is where we would encache the epacs if desired 
	     * (for sharing purposes).
	     */
	    if (GOOD_STATUS(&st) && raw_epac_setP) {
		/*
		 * caller wants the raw epac set
		 */
		raw_epac_setP->bytes = malloc(tmp_epac_set.num_bytes);
		if (raw_epac_setP->bytes == NULL) 
		{
		    SET_STATUS(&st, sec_s_no_memory);
		} else {
		    raw_epac_setP->num_bytes = tmp_epac_set.num_bytes;
		    memcpy(raw_epac_setP->bytes, tmp_epac_set.bytes,
			   raw_epac_setP->num_bytes);
		}
	    }
	} else { /* SEC_AUTHZ_FMT_V1_0 path */

	    /*
	     * We are safely nested inside the  
	     *    if (FLAG_SET(ad_parts, priv_ad_part_pac)
             *    || FLAG_SET(ad_parts, priv_ad_part_v1_1)) 
	     * block, so a SEC_AUTHZ_FMT_V1_0 *must* contain a
	     * pickled PAC, otherwise, it's completely bogus.
	     */
	    if (FLAG_SET(ad_parts, priv_ad_part_pac)) 
	    {
		sec_id_pac_t_unpickle(pickled_pac, 
				      (void *(*)(unsigned32)) malloc,
				      client_pac ? client_pac : &tmp_pac,
				      &st);
		free( (char *) pickled_pac); pickled_pac = NULL;
		if (STATUS_OK(&st)) 
		{
		    gen_fake_v1_1_cred_handle( (client_pac ? client_pac :
			&tmp_pac), authP, expiration, client_creds, 
			raw_epac_setP, &st);
		    if (!client_pac) {
			sec_id_pac_util_free(&tmp_pac);
		    }
		}
	    } else {
		SET_STATUS(&st, sec_s_no_pac); 
		goto out;
	    }
	}	
    } else {
	*authz_proto = rpc_c_authz_name;
	
	authP->authenticated = true;  
	if ((authP->server_princ_name == NULL)
	&& (authent->ticket->server != NULL))
	{
	    error_status_t st;
	    
	    st = sec_krb_unparse_name(authent->ticket->server, 
			       (unsigned char **)&authP->server_princ_name);
	    
	}
	authP->authz_svc = *authz_proto;
	authP->authz_info.client_princ_name = (unsigned_char_p_t)*client_name;
	
	if (client_creds) 
	{
	    gen_fake_v1_1_cred_handle(NULL, authP, expiration, 
				      client_creds, NULL, &st);
	}
    
    }

out:
    if (pickled_pac != NULL)
	free(pickled_pac);
    sec_encode_v1_1_authz_data_free(free, &v1_1_ad);

    if (authP->server_princ_name) {
	rpc_string_free(&authP->server_princ_name, &ignore_st);
    }

    if (BAD_STATUS(&st))
    {
	if (client_name && *client_name) 
	    rpc_string_free(client_name, &ignore_st);   
	
	if (server_name && *server_name)
	    sec_krb_parsed_name_free(server_name);

	if (*authentP != NULL) {
            krb5_free_tkt_authent(*authentP);
            *authentP = NULL;
	}
    }
    return st;

}

error_status_t sec_krb_decode_message_common (
    sec_krb_message *message,			/* in */
    unsigned32 authz_fmt,			/* in */
    sec_krb_parsed_name *server_name,		/* out */
    unsigned char **client_name,		/* out */
    sec_id_pac_t *client_pac,			/* out */
    rpc_authz_cred_handle_t *client_creds,	/* out */
    unsigned32 *expiration,			/* out */
    sec_krb_msgstamp *timestamp,		/* out */
    krb5_tkt_authent **authentP,		/* out */
    unsigned32 *authz_proto			/* out */
)
{
    return( sec_krb_decode_msg_common_frags (message, authz_fmt, server_name,
	client_name, client_pac, NULL, client_creds, expiration, timestamp,
	authentP, authz_proto) );
}

error_status_t sec_krb_build_message_none (
    sec_krb_ccache ccache,	/* in */
    sec_krb_cred cred,	/* in */
    unsigned32 authz_proto,	/* in */
    unsigned32 authz_fmt,	/* in */
    sec_krb_message *message /* out */
	)
{	
    error_status_t st;
    
    switch (authz_proto) {
    case rpc_c_authz_name:
    {
	char *name;
#ifdef USE_GETPW_R
	struct passwd pw;
	char buf[1024];
	struct passwd *pwp = &pw;
                
	if (getpwuid_r(getuid(), &pw, buf, sizeof(buf)) < 0)
	    pwp = NULL;
#else
	struct passwd *pwp = getpwuid(getuid());
#endif
	name = (pwp? pwp->pw_name : "unknown_user");
                
	message->length = strlen(name) + 2;
	message->data = (byte_p_t) malloc (message->length);
	message->data[0] = rpc_c_authz_name;
	memcpy (&message->data[1], name, message->length-1);
	return rpc_s_ok;
    }
    case rpc_c_authz_dce:
    {
	/* Peter Packer Picked a Peck of Pickled PAC's */
	idl_pkl_t   *pickled_pac;
	unsigned32  pickle_len;
	sec_id_pac_t *client_pac;
	/* !!! should look at authz_fmt */
	client_pac = sec_login_util_get_pac(ccache);
#ifdef DEBUG_PICKLE
	printf("Pickling pac:\n");
	sec_id_pac_print (client_pac);
#endif
	sec_id_pac_t_pickle(client_pac, NULL, (void *(*)(unsigned32)) malloc,
	    (unsigned32) 8, &pickled_pac, &pickle_len, &st);
	if (GOOD_STATUS(&st)) {
	    message->length = pickle_len+1;
	    message->data = (byte_p_t) malloc (message->length);
	    message->data[0] = rpc_c_authz_dce;
	    memcpy (&message->data[1], pickled_pac, pickle_len);
	    free(pickled_pac);
	}
	return st;
    }
    default:
	return sec_s_authz_unsupp;
    }
}

#include <dce/asn1.h>

static int
sec_asn1len(base, limit)
unsigned char *base;
unsigned char *limit;
{
    int len;
    
    unsigned char *p = asn1_skip (base, limit);
    if (p == NULL) return -1;
    else return p-base;
}

