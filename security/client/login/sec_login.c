/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_login.c,v $
 * Revision 1.1.24.1  1996/11/13  17:59:10  arvind
 * 	Merge to DCE_1.2.2
 * 	[1996/10/24  16:56 UTC  mullan_s  /main/mullan_dce12_tp_preauth/1]
 *
 * 	validate_identity: fallback to local rgy if get_login_info fails.
 * 	[1996/02/18  00:06:49  marty  1.1.22.3]
 *
 * Revision 1.1.22.3  1996/02/18  00:06:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:58  marty]
 * 
 * Revision 1.1.22.2  1995/12/08  18:04:25  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/9  1995/10/23  17:20 UTC  burati
 * 	Merge fix for CHFts16467 from mb_mothra7
 * 
 * 	HP revision /main/HPDCE02/8  1995/09/08  17:25 UTC  sommerfeld
 * 	In sec_login_export_context, bypass sec_login_pvt_check_exp() if
 * 	caller is only trying to get a guesstimate of the output size...
 * 	[1995/08/31  03:35 UTC  sommerfeld  /main/HPDCE02/sommerfeld_dfsperf/1]
 * 
 * 	HP revision /main/HPDCE02/7  1995/07/11  21:13 UTC  sommerfeld
 * 	add sec_login_unset_context().
 * 	[1995/05/02  19:42 UTC  sommerfeld  /main/HPDCE02/sommerfeld_CHFts13948/1]
 * 
 * 	HP revision /main/HPDCE02/6  1995/06/16  20:05 UTC  mdf
 * 	Close memory leaks.
 * 	[1995/06/16  19:57 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_6/2]
 * 
 * 	Close cache when finished, CHFts15050
 * 	[1995/06/14  19:42 UTC  mdf  /main/HPDCE02/mdf_mothra_defects_6/1]
 * 
 * 	HP revision /main/HPDCE02/5  1995/04/04  21:01 UTC  cuti
 * 	Add one more signature to sec_login_pvt_release_context() for contitional cache remove.
 * 
 * 	HP revision /main/HPDCE02/5  1995/04/04  21:01 UTC  cuti
 * 	Add one more signature to sec_login_pvt_release_context() for contitional
 * 	   cache remove.
 * 
 * 	HP revision /main/HPDCE02/cuti_machine_context/1  1995/04/01  21:56 UTC  cuti
 * 	Reflect sec_login_pvt_release_context signature change
 * 
 * 	HP revision /main/HPDCE02/cuti_machine_context/1  1995/04/01  21:55 UTC  cuti
 * 	Reflect sec_login_pvt_release_context signature change
 * 
 * 	HP revision /main/HPDCE02/4  1995/02/09  11:54 UTC  mdf
 * 	Merge
 * 
 * 	HP revision /main/HPDCE02/mdf_MOTHRA_5/1  1995/02/08  18:41 UTC  mdf
 * 	Changes for CHFts14258, which prevents kdestroy from being able to
 * 	remove the machine's credential files.
 * 
 * 	HP revision /main/HPDCE02/3  1995/02/02  19:40 UTC  mdf
 * 	resolve defect.
 * 
 * 	HP revision /main/HPDCE02/mdf_MOTHRA_4/2  1995/01/31  19:39 UTC  mdf
 * 	Changes for CHFts13794, (basicly Bill Sommerfeld's change for GTE).
 * 
 * 	HP revision /main/HPDCE02/2  1994/11/17  18:18 UTC  mullan_s
 * 	Merge unintegrated
 * 	[1995/12/08  16:31:20  root]
 * 
 * Revision 1.1.18.1  1994/10/24  18:32:43  greg
 * 	CR12159: Add a generation uuid to the login context and incorporate it
 * 	into the login context export token.  This allows the  CDS clerk to
 * 	recognize changes to the *fffffff* host machine credentials immediately.
 * 	[1994/10/24  18:04:57  greg]
 * 
 * 	HP revision /main/HPDCE02/1  1994/11/15  21:22 UTC  mullan_s
 * 	Merge.
 * 
 * Revision 1.1.13.10  1994/09/16  21:51:52  sommerfeld
 * 	[OT11915] store pag in login context.
 * 	[1994/09/16  21:46:01  sommerfeld]
 * 
 * Revision 1.1.13.9  1994/09/02  00:46:27  max
 * 	CR 11942.  Have sec_login_setup_first unlink the creds file if it fails.
 * 	[1994/09/02  00:42:06  max]
 * 
 * Revision 1.1.13.8  1994/08/22  17:03:30  max
 * 	Added crypt prototype for RIOS.  Eliminated unnecessary cast of crypt
 * 	return value
 * 	[1994/08/22  16:04:27  max]
 * 
 * Revision 1.1.13.7  1994/07/15  14:58:40  mdf
 * 	Hewlett Packard Security Code Drop
 * 	[1994/07/14  17:14:43  mdf]
 * 
 * Revision 1.1.13.6  1994/06/10  15:06:59  greg
 * 	intercell fixes for DCE1.1 beta
 * 	[1994/06/03  20:47:31  greg]
 * 
 * Revision 1.1.13.5  1994/06/02  21:57:25  mdf
 * 	Merged with changes from 1.1.13.4
 * 	[1994/06/02  21:55:13  mdf]
 * 
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  19:36:35  mdf]
 * 
 * Revision 1.1.13.4  1994/06/02  20:20:50  mdf
 * 	hp_sec_to_osf_3 drop, merge up with latest.
 * 	[1994/05/24  19:36:35  mdf]
 * 
 * Revision 1.1.13.2  1994/05/11  16:49:17  ahop
 * 	Get rid on non-ansi prototype
 * 	[1994/05/05  22:09:37  ahop]
 * 
 * 	hp_sec_to_osf_2 drop
 * 	add sec_login_cred_get_expiration
 * 	Do sec_login_set_extended_attrs work.
 * 	Prevent destroy_password from causing a core dump if a
 * 	NULL passwd is passed in.
 * 	sec_login_import_context:  plug ticket-expired memory leak
 * 	sec_login_purge_context_exp:  added to handle expired creds
 * 	[1994/04/29  20:45:11  ahop]
 * 
 * 	hp_sec_to_osf_2 drop
 * 	add sec_login_cred_get_expiration
 * 	Do sec_login_set_extended_attrs work.
 * 	Prevent destroy_password from causing a core dump if a
 * 	NULL passwd is passed in.
 * 	sec_login_import_context:  plug ticket-expired memory leak
 * 	sec_login_purge_context_exp:  added to handle expired creds
 * 
 * 	hp_sec_to_osf_2 drop
 * 	add sec_login_cred_get_expiration
 * 	Do sec_login_set_extended_attrs work.
 * 	Prevent destroy_password from causing a core dump if a
 * 	NULL passwd is passed in.
 * 	sec_login_import_context:  plug ticket-expired memory leak
 * 	sec_login_purge_context_exp:  added to handle expired creds
 * 
 * Revision 1.1.13.1  1994/01/28  23:10:18  burati
 * 	Delegation changes (dlg_bl1)
 * 	[1994/01/19  21:37:55  burati]
 * 
 * 	HP revision /main/HPDCE01/3  1994/01/31  19:27  mdf
 * 	Fixed bogus merge...
 * 
 * 	HP revision /main/HPDCE01/2  1994/01/28  13:47  mdf
 * 	Merge in KK_FIX for rps.
 * 
 * 	HP revision /main/GAMERA/KK/KK_FIX/1  1994/01/24  19:11  rps
 * 	create KK_FIX
 * 
 * 	HP revision /main/GAMERA/KK/rps_fix_exp_cred_purge/1  1994/01/24  16:09  rps
 * 	[CHFts10356] _purge_context_exp: Handle ticketless creds files
 * 
 * Revision 1.1.11.6  1993/12/17  20:35:13  mdf
 * 	Prevent destroy_password from causing a core dump if a
 * 	NULL passwd is passed in.
 * 	[1993/12/17  16:36:48  mdf]
 * 
 * Revision 1.1.11.5  1993/12/06  23:22:21  rps
 * 	[CHFts09511] sec_login_import_context:  plug ticket-expired memory leak
 * 	[CHFts09498] sec_login_purge_context_exp:  added to handle expired creds
 * 	[1993/12/04  01:17:44  rps]
 * 
 * Revision 1.1.9.5  1993/10/14  20:02:09  sommerfeld
 * 	Latent support for authentication forwarding
 * 	[1993/10/14  15:56:13  sommerfeld]
 * 
 * Revision 1.1.9.4  1993/10/08  13:39:40  mullan_s
 * 	[OT8642] All passwords, good or bad, are destroyed before returning from
 * 	sec_login_validate_identity() & sec_login_valid_and_cert_ident().
 * 	[1993/10/07  19:45:55  mullan_s]
 * 
 * Revision 1.1.9.3  1993/10/06  20:27:28  mccann
 * 	     manual merge of last 2 changes, didn't merge during bsubmit
 * 	     [1993/10/06  20:27:04  mccann]
 * 
 * Revision 1.1.9.2  1993/10/06  20:17:31  mccann
 * 	     CR8651 64 bit porting changes
 * 	     [1993/10/04  19:06:03  mccann]
 * 
 * Revision 1.1.9.1  1993/09/29  14:25:49  burati
 * 	CR6140 Remove unreachable error checking for overridden contexts.
 * 	[1993/09/28  23:17:52  burati]
 * 
 * Revision 1.1.6.9  1993/02/16  22:40:45  sommerfeld
 * 	[OT6556] blow up in sec_login_{import,export} context if context
 * 	has expired.
 * 	[1993/02/09  02:34:52  sommerfeld]
 * 
 * Revision 1.1.6.8  1993/01/26  20:35:25  burati
 * 	Temporarily remove fix for 4851 because it caused defects 6938,6917
 * 	[1993/01/26  20:32:40  burati]
 * 
 * Revision 1.1.6.7  1992/12/29  12:43:36  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:39:28  zeliff]
 * 
 * Revision 1.1.6.6  1992/12/17  22:41:59  burati
 * 	CR6295 Add sec_login_register_ccache()
 * 	CR5872 set *identity_expiration to 0, not NULL
 * 	[1992/12/17  22:39:16  burati]
 * 
 * Revision 1.1.6.5  1992/12/15  18:17:23  burati
 * 	CR4851 Make sure login context is finalized in export routine
 * 	[1992/12/15  17:13:56  burati]
 * 
 * Revision 1.1.6.4  1992/12/04  21:27:41  burati
 * 	CR5978 Make call to OS dependent routine to retrieve struct passwd
 * 	for returning to client from sec_login_get_pwent()
 * 	[1992/12/04  16:42:39  burati]
 * 
 * Revision 1.1.6.3  1992/11/16  23:18:42  burati
 * 	CR6108 Check for BAD_STATUS before dereferencing return value
 * 	[1992/11/16  22:42:36  burati]
 * 
 * Revision 1.1.6.2  1992/09/29  21:15:21  devsrc
 * 	[OT 5373]    SNI/SVR4 merge.
 * 	[1992/09/11  21:19:16  sekhar]
 * 
 * Revision 1.1.2.8  1992/07/16  15:03:17  burati
 * 	4720 Make sure refresh_identity and setup_first return boolean32
 * 	[1992/07/15  17:41:23  burati]
 * 
 * Revision 1.1.2.7  1992/06/03  18:59:29  ahop
 * 	 Fix 2143: handle new credential cache file location
 * 	[1992/05/22  19:39:05  ahop]
 * 
 * Revision 1.1.2.6  1992/05/22  19:41:48  burati
 * 	Make sure omit type passwd overrides prevent login
 * 	[1992/05/22  19:14:06  burati]
 * 
 * Revision 1.1.2.5  1992/05/14  18:54:44  burati
 * 	Mods to support overrides
 * 	[1992/05/14  01:47:35  burati]
 * 
 * Revision 1.1.2.4  1992/05/08  20:51:33  sommerfeld
 * 	Merge from mainline.
 * 	[1992/04/17  22:16:51  sommerfeld]
 * 
 * Revision 1.1.2.3  1992/03/31  21:18:37  burati
 * 	CR2419 Merged in local registry code
 * 	[1992/03/31  01:27:09  burati]
 * 	Revision 1.1.3.2  1992/04/15  22:06:56  sommerfeld
 * 	Context cache: On import, check login context cache in an attempt to
 * 	avoid allocating a new login context.
 * 
 * Revision 1.1.2.2  1992/03/19  22:53:09  burati
 * 	CR759 Set lcp->identity->state to info_certified in certify_identity
 * 	[1992/03/19  19:45:06  burati]
 * 
 * Revision 1.1  1992/01/19  14:46:58  devrcs
 * 	     Initial revision
 * 
 * $EndLog$
 */

/*  sec_login.c
**
** Copyright (c) Hewlett-Packard Company 1991,1992
** Unpublished work. All Rights Reserved.
**
*/
/*
 * (c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
 */
/*
 *     Registry Client Agent Interface - authenticated/authorized network login
 */

#ifdef MODULE_VERSION_ID
static char *VersionID =
    "$Id: sec_login.c,v 1.1.24.1 1996/11/13 17:59:10 arvind Exp $";
#endif

#include <stdlib.h>
#include <sys/types.h>
#include <sys/file.h>
#include <un_strng.h>
#include <bstring.h>
#include <un_io.h>
#include <un_stat.h>
#include <un_time.h>
#include <un_pass.h>
#include <sec_cred_internal.h>
#include <rgymacro.h>
#include <u_str.h>
#include <initstring.h>

#include <dce/sec_cert.h>
#include <dce/rpcbase.h>
#include <dce/keymgmt.h>
#include <dce/binding.h>
#include <dce/secidmap.h>
#include <dce/override.h>
#include <dce/passwd.h>
#include <krb5/base-defs.h>
#include <krb5/krb5.h>
#include <krb5/func-proto.h>

#include <sec_svc.h>

/* define sec_login ALLOCATE_GLOBALS to export the file location
 * globals in sec_login_file_loc.h (included by sec_login_pvt.h)
 */
#define _sec_login_ALLOCATE_GLOBALS_
#include <sec_login_pvt.h>
#include <dce/sec_login_encode.h>

#include <sec_krb_util.h>
#include <sec_id_pac_util.h>

#ifndef NULL
#    define NULL 0L
#endif


#ifdef _AIX
/* RIOS without crypt proto in unistd.h */
char    *crypt (const char*,const char*);
#endif

/*
 * wipe the memory containing password data clean
 */
PRIVATE  void destroy_passwd
#ifndef __STDC__
    ( passwd )
    sec_passwd_rec_t  *passwd;
#else
  (
    sec_passwd_rec_t  *passwd
  )
#endif
{
    unsigned_char_p_t  ucp;

    if (!passwd)
	return;

    switch (PASSWD_TYPE(passwd)) {
    case sec_passwd_plain:
        for (ucp = PLAIN_PASSWD(passwd); *ucp != '\0'; *ucp = '\0', ucp++);
        break;
    case sec_passwd_des:
        memset(DES_PASSWD(passwd), 0, sizeof(sec_passwd_des_key_t));
        break;
    }
}

/*
 * PUBLIC functions
 */


/* s e c _ l o g i n _ s e t u p _ i d e n t i t y
 *
 * Setup the user's network identity.  This operation will create any
 * local context necessary to perform authenticated network operations.
 * It will not establish any local OS context - that is the responsibility
 * of the caller.  The network identity set up by this operation may be
 * used to retrieve information before it is validated via
 * sec_login_validate_identity.
 *
 * The setup_identity operation and the validate_identity operation are
 * two halves of a single logical operation.  Together they collect the
 * identity data needed to establish an authenticated identity.  The
 * operations are independent to protect the confidentiality of the
 * user's password.  If setup_identity were to perform remote
 * operations to acquire the necessary network credentials, the operation
 * could take an arbitrary amount of time.  If a password were submitted to
 * this operation, an attacker could force long network delays and then
 * attempt to extract the user's password from the process address space.
 * This form of attack is only feasible on some machines - but
 * nevertheless, we have opted to separate the identity acquisition
 * operation into two phases and prevent this form of attack.
 *
 * If the sec_login_external_tgt flag is passed into this routine,
 * then we assume we are setting up an identity that will be "externally
 * validated"; that is, the TGT is obtained by some other means.  This
 * allows a simple proxy mechanism.  When this flag is set, we do not
 * attempt to gain a TGT, and we note this in the context for other
 * routines.
 *
 * NOTE: Neither setup_identity nor validate_identity check for
 * account or identity expiration.  The user of this interface is
 * responsible for such checks.
 *
 * Sealed Network credentials (or certificates) are retrieved via
 * sec_login_setup_identity.  In a Kerberos environment this operation
 * will acquire an appropriate ticket granting ticket.
 *
 * ERRORS:
 *    rgy_object_not_found     - principal does not exist
 *    rgy_registry_unavailable - network registry not available
 *    sec_login_s_no_memory    - memory allocation failure
 */
PUBLIC boolean32 sec_login_setup_identity
#ifndef __STDC__
    ( principal, flags, login_context, st )
    unsigned_char_p_t   principal;      /* [in] */
    sec_login_flags_t   flags;          /* [in] */
    sec_login_handle_t  *login_context; /* [out] */
    error_status_t      *st;            /* [out] */
#else
  (
    unsigned_char_p_t   principal,      /* [in] */
    sec_login_flags_t   flags,          /* [in] */
    sec_login_handle_t  *login_context, /* [out] */
    error_status_t      *st             /* [out] */
  )
#endif
{
    sec_login_handle_t  save = *login_context;  /* just in case */
    sec_login_context_t **lcap = (sec_login_context_t **) login_context;
    error_status_t      lst;

    *login_context = NULL;
    CLEAR_STATUS(st);

    /* Allocate and init login context */
    sec_login_pvt_allocate_login_context(false, principal,
                                         (char *) NULL,
                                         lcap,
                                         st);
    if (BAD_STATUS(st)) {
        return false;
    }

    if (GOOD_STATUS(st)) {
        (*lcap)->state = setup;
        (*lcap)->flags = flags;
    } else {
        sec_login_pvt_purge_context(*lcap, &lst);
        *login_context = save;  /* restore whatever the caller passed in */
    }

    return GOOD_STATUS(st);
}


/* s e c _ l o g i n _ r e f r e s h _ i d e n t i t y
*
* Refresh a previously established identity.  This operation
* operates on an existing valid context, and cannot be used to
* change credentials associated with that identity.  If any administrative
* changes to the principal's identity have occurred since the last
* setup/refresh  (e.g. the principal's membership in one or more groups
* has been revoked), the refreshed identity reflects these changes.
* It is an error to refresh a locally authenticated context.
*
* ERRORS:
*    rgy_object_not_found      - principal no longer exists
*    rgy_registry_unavailable  - network registry not available
*    sec_login_s_context_invalid - context has not been validated
*/
PUBLIC boolean32 sec_login_refresh_identity
#ifndef __STDC__
    ( login_context, st )
    sec_login_handle_t  login_context;
    error_status_t      *st;
#else
  (
    sec_login_handle_t  login_context,
    error_status_t      *st
  )
#endif
{
    sec_login_context_t *lcp;

    if (DEFAULT_HANDLE(login_context)) {
        SET_STATUS(st, sec_login_s_default_use);
        return false;
    }

    lcp = sec_login_pvt_check_handle(login_context, valid, st);

    /*
     * people can't refresh external tgts, they should recall
     * sec_login_krb5_add_cred.
     */

    if (FLAG_SET(lcp->flags, sec_login_external_tgt)) {
        SET_STATUS(st, sec_login_s_not_implemented);
        return false;
    }

    if (GOOD_STATUS(st)) {
        lcp->refreshed = true;
    }

    return GOOD_STATUS(st);
}



/* s e c _ l o g i n _ v a l i d a t e _ i d e n t i t y
 *
 * Validate the login context established via sec_login_setup_identity.
 * The caller must know the user's password for this operation to succeed.
 * This operation MUST be invoked before the network credentials will be
 * used.
 *
 * This operation can be used to validate network credentials, but as it does
 * not apply override data, it cannot be used to acquire local OS credentials.
 * Only sec_login_valid_and_cert_ident applies overrides (and checks the local
 * registry in the case of comm failures.
 *
 *
 * ERRORS:
 *    sec_rgy_registry_unavailable
 *    sec_rgy_passwd_invalid
 *    sec_login_s_null_password
 *
 *
 *
 * The TGT is not renewable, forwardable, proxiable, or
 * postdatable.  Lifetimes are defaulted to KDC-assigned values.
 *
 * The TGT is restricted to use from host that generates the TGT
 * ticket request.  Is this correct?
 *
 * It's possible that this login context may have been externally validated
 * (this is the case of externally orignated TGTs).  In this case,
 * we assume that someone has already entered the TGT into the ticket
 * cache (we ignore the password info).
 */
PUBLIC boolean32 sec_login_validate_identity
#ifndef __STDC__
    ( login_context,  passwd, reset_passwd, auth_src, st )
    sec_login_handle_t      login_context;  /* [in]  */
    sec_passwd_rec_t        *passwd;        /* [in]  */
    boolean32               *reset_passwd;  /* [out] */
    sec_login_auth_src_t    *auth_src;      /* [out] */
    error_status_t          *st;            /* [out] */
#else
  (
    sec_login_handle_t      login_context,  /* [in]  */
    sec_passwd_rec_t        *passwd,        /* [in]  */
    boolean32               *reset_passwd,  /* [out] */
    sec_login_auth_src_t    *auth_src,      /* [out] */
    error_status_t          *st             /* [out] */
  )
#endif
{
    sec_login_context_t     *lcp, *rcp = NULL, *tlcp;
    boolean32               refresh;
    error_status_t          lst;
    sec_override_fields_t   overridden;

    CLEAR_STATUS(st);

    lcp = sec_login_pvt_check_handle(login_context, setup, st);
    if (BAD_STATUS(st)) {
        destroy_passwd(passwd);
        return false;
    }

    /*
     * if the context is marked that it will be using an external tgt, make
     * sure the password parameter is null.
     */
    if (FLAG_SET(lcp->flags, sec_login_external_tgt) && passwd) {
        SET_STATUS(st, sec_s_invalid_auth_params);
        destroy_passwd(passwd);
        return false;
    }

    refresh = lcp->refreshed;

    /*
     * if the context has already been through a validation routine, and has not
     * been refreshed since, then fail
     */
    if (lcp->state >= valid && !refresh) {
        SET_STATUS(st, sec_login_s_already_valid);
        destroy_passwd(passwd);
        return false;
    }

    /* can't perform a network login with a zero-length password. */
    if (!FLAG_SET(lcp->flags, sec_login_external_tgt) &&
           PASSWD_TYPE(passwd) == sec_passwd_plain &&
           u_strlen(PLAIN_PASSWD(passwd)) == 0) {
        SET_STATUS(st, sec_login_s_null_password);
        return false;
    }

    if (refresh) {
        rcp = sec_login_pvt_setup_refresh_context(lcp, st);
        if (GOOD_STATUS(st)) {
            tlcp = rcp;
        }
    } else {
        tlcp = lcp;
    }

    if (BAD_STATUS(st)) {
        destroy_passwd(passwd);
        return false;
    }

    if (!FLAG_SET(lcp->flags, sec_login_external_tgt)) {
        /* try to obtain and decrypt a TGT */
        if (refresh) {
            tlcp->auth_src = sec_login_auth_src_network;
            sec_login_pvt_get_and_process_krb_tgt(tlcp, passwd, true, st);
        } else {
            sec_login_pvt_get_and_process_krb_tgt(tlcp, passwd, false, st);
        }
        if (BAD_STATUS(st)) {
            /* destroy scratch context */
            if (rcp != NULL) {
                sec_login_pvt_purge_refresh_context(rcp, &lst);
            }
            destroy_passwd(passwd);
            return false;
        }
    }

    destroy_passwd(passwd);

    /* MUST mark context valid here to get name-based authentication to work
     * in get_login_info.
     */
    tlcp->state = valid;

    sec_login_pvt_get_login_info(false, tlcp, &overridden, st);
    if (STATUS_EQUAL(st, sec_rgy_server_unavailable)) {
	sec_login_pvt_get_lrgy_login_info(false, tlcp, &lst);
	if (GOOD_STATUS(&lst)) {
	    tlcp->state = valid;
	    CLEAR_STATUS(st);
	}
    }

    if (GOOD_STATUS(st))
        sec_login_pvt_check_policy(tlcp, reset_passwd, st);

    if (GOOD_STATUS(st) && refresh) {
        sec_login_pvt_synch_refresh_context(lcp, rcp, st);
    }

    /* done with the refresh scratch context, in any event */
    if (rcp != NULL) {
        sec_login_pvt_purge_refresh_context(rcp, &lst);
    }

    if (BAD_STATUS(st)) {
        if (!refresh) {
            lcp->state = setup;
        }
        return false;
    } else {
        if (refresh) {
            /* reset "refreshed" indicator */
            lcp->refreshed = false;
        }
        *auth_src = lcp->auth_src;
    }

    return true;
}

/* s e c _ l o g i n _ c e r t i f y _ i d e n t i t y
 *
 * Certify that the server that granted the TGT for this context is
 * legitimate i.e., that there is a valid trust path from the user's
 * home cell to the host machines cell.  We do this by making an
 * authenticated call to the host machine's sec_clientd process.
 */
PUBLIC boolean32 sec_login_certify_identity ( 
    sec_login_handle_t      login_context,  /* [in]  */
    error_status_t          *st             /* [out] */
)
{
    sec_login_context_t     *lcp;

    CLEAR_STATUS(st);
    lcp = sec_login_pvt_check_handle(login_context, valid, st);
    if (BAD_STATUS(st)) {
        return false;
    }

    /* don't bother re-certifying */
    if (lcp->state == certified) {
        return true;
    }

    sec_login_validate_cert_auth(login_context, st);
    if (GOOD_STATUS(st)) {
        lcp->state = certified;
        lcp->identity->state = info_certified;
        return true;
    }
    return false;
}

/* s e c _ l o g i n _ v a l i d _  a n d _ c e r t _ i d e n t
 *
 */
PUBLIC boolean32 sec_login_valid_and_cert_ident
#ifndef __STDC__
    ( login_context,  passwd, reset_passwd, auth_src, st )
    sec_login_handle_t      login_context;  /* [in]  */
    sec_passwd_rec_t        *passwd;        /* [in]  */
    boolean32               *reset_passwd;  /* [out] */
    sec_login_auth_src_t    *auth_src;      /* [out] */
    error_status_t          *st;            /* [out] */
#else
  (
    sec_login_handle_t      login_context,  /* [in]  */
    sec_passwd_rec_t        *passwd,        /* [in]  */
    boolean32               *reset_passwd,  /* [out] */
    sec_login_auth_src_t    *auth_src,      /* [out] */
    error_status_t          *st             /* [out] */
  )
#endif
{
    char                        *cfile, *unix_key;
    sec_login_context_t         *lcp, *rcp = NULL, *tlcp;
    boolean32                   passwd_valid = false, refresh;
    boolean32                   passwd_ovrd, lrgy_add;
    signed32                    pw_uid, pw_gid;
    error_status_t              lst;
    sec_rgy_unix_passwd_buf_t   pw_salt;
    sec_override_fields_t       overridden;

    if (geteuid() != 0) {
        SET_STATUS(st, sec_login_s_privileged);
        destroy_passwd(passwd);
        return false;
    }

    lcp = sec_login_pvt_check_handle(login_context, setup, st);
    if (BAD_STATUS(st)) {
        destroy_passwd(passwd);
        return false;
    }

    if (FLAG_SET(lcp->flags, sec_login_external_tgt)) {
        SET_STATUS(st, sec_login_s_not_implemented);
        destroy_passwd(passwd);
        return false;
    }

    refresh = lcp->refreshed;

    /*
     * if the context has already been through a validation routine,
     * and has not been refreshed since, then fail
     */
    if (lcp->state >= valid && !refresh) {
        SET_STATUS(st, sec_login_s_already_valid);
        destroy_passwd(passwd);
        return false;
    }

    CLEAR_STATUS(st);
    tlcp = lcp;

    /* Check to see if password is overridden */
    passwd_ovrd = override_is_passwd_overridden(lcp->principal, &pw_uid,
                                                &pw_gid, pw_salt, st);

    if (BAD_STATUS(st)) {
        if (STATUS_EQUAL(st,sec_login_s_ovrd_ent_not_found)||
            STATUS_EQUAL(st,sec_login_s_no_override_info) ) {
            CLEAR_STATUS(st);
        } else {
            destroy_passwd(passwd);
            return false;
        }

        /* If password is overridden, network must be ignored and
         * all information must be obtained from overrides.
         */
    } else if (passwd_ovrd) {
        if (passwd->key.key_type != sec_passwd_plain) {
            SET_STATUS(st, sec_login_s_invalid_password);
            destroy_passwd(passwd);
            return false;
        } else {
            unix_key = crypt((char*)(passwd->key.tagged_union.plain), (char*)pw_salt);
            passwd_ovrd = override_check_passwd(lcp->principal,&pw_uid,&pw_gid,
                                                (unsigned_char_p_t) unix_key,
                                                st);
            if ((!passwd_ovrd) || BAD_STATUS(st)) {
                destroy_passwd(passwd);
                return false;
            } else {
                sec_login_pvt_override_all(lcp, st);
                *auth_src = lcp->auth_src;
                destroy_passwd(passwd);
                return GOOD_STATUS(st);
            }
        }
    }

    /* can't perform a network login with a zero-length password. */
    if (PASSWD_TYPE(passwd) == sec_passwd_plain &&
        u_strlen(PLAIN_PASSWD(passwd)) == 0) {
        SET_STATUS(st, sec_login_s_null_password);
        return false;
    }

    if (refresh) {
        rcp = sec_login_pvt_setup_refresh_context(lcp, st);
        if (GOOD_STATUS(st)) {
            tlcp = rcp;
        }
    }


    if (GOOD_STATUS(st)) {
        /* try to obtain and decrypt a TGT */
        if (refresh) {
            tlcp->auth_src = sec_login_auth_src_network;
            sec_login_pvt_get_and_process_krb_tgt(tlcp, passwd, true, st);
        } else {
            sec_login_pvt_get_and_process_krb_tgt(tlcp, passwd, false, st);
        }
    }

    /* wipe out the password */
    destroy_passwd(passwd);

    if (GOOD_STATUS(st)) {

        /* MUST mark context valid here to get name-based authentication
         * to work in get_login_info.
         */
        tlcp->state = valid;
        sec_login_pvt_get_login_info(true, tlcp, &overridden, st);
        /* If we're here and the password is overridden, then
         * it must be an invalid password since password overrides
         * must match by name, which would have happened above
         */
        if (FLAG_SET(overridden, sec_override_pw_passwd)) {
            SET_STATUS(st, sec_login_s_invalid_password);
        } else if (BAD_STATUS(st)) {
            sec_login_pvt_get_lrgy_login_info(true, tlcp, &lst);
            if (GOOD_STATUS(&lst)) {
                CLEAR_STATUS(st);
            }
        }
    }

    if (GOOD_STATUS(st)) {
        sec_login_pvt_certify_context(tlcp, st);
    }

    if (GOOD_STATUS(st)) {
        sec_login_pvt_check_policy(tlcp, reset_passwd, st);
    }

    if (GOOD_STATUS(st)) {
        if (refresh) {
            sec_login_pvt_synch_refresh_context(lcp, rcp, st);
            if (GOOD_STATUS(st)) {
                lcp->refreshed = false;
            }
        } else {
            /*
             * chown the credential cache with the uid from
             * context unix info.
             * Also, must change the login context owner uid whenever
             * we chown the krb5 cache file.
             */
            cfile = krb5_cc_get_name(tlcp->krb_info.cache);
            if (cfile != NULL && UNIX_CREDS(tlcp).pwent != NULL) {
                chown(cfile, UNIX_CREDS(tlcp).pwent->pw_uid, -1);
		tlcp->owner_uid = UNIX_CREDS(tlcp).pwent->pw_uid;
            }
        }

        *auth_src = lcp->auth_src;

    }  else if (!refresh) {
        lcp->state = setup;
    }

    if ((lcp->auth_src == sec_login_auth_src_network) &&
        (lcp->state == certified) && (GOOD_STATUS(st)) &&
        (UNIX_CREDS(lcp).pwent != NULL)) {
            /* Store the login context in the local registry */
        sec_login_pvt_lrgy_store(lcp, &lst);
    }

    if (rcp != NULL)
        sec_login_pvt_purge_refresh_context(rcp, &lst);

    return GOOD_STATUS(st);
}


/* s e c _ l o g i n _ n e w g r o u p s
 *
 * Setup a new login context with the specified group list.  The new groupset
 * must be a proper subset of the current groupset.  This function cannot
 * be used to expand a user's concurrent groupset.
 *
 * This operation does not need to be validated as the user identity does
 * not change - and knowledge of the password is not needed, but the
 * login_context should be purged when no longer needed.
 *
 * ERRORS:
 *    sec_login_s_groupset_invalid
 *    sec_login_s_default_use
 *    sec_login_s_invalid_context
 *
 * NOTE:
 *  Currently you can only have groups from the local realm.
 *  Perhaps someday this restriction will be
 *  relaxed.
 */
PUBLIC boolean32 sec_login_newgroups
#ifndef __STDC__
    (login_context, flags, num_local_groups, local_groups, restricted_context, st )
    sec_login_handle_t  login_context;          /* [in]  */
    sec_login_flags_t   flags;                  /* [in]  */
    unsigned32          num_local_groups;       /* [in]  */
    sec_id_t            local_groups[];         /* [in]  */
    sec_login_handle_t  *restricted_context;    /* [out] */
    error_status_t      *st;                    /* [out] */
#else
  (
    sec_login_handle_t  login_context,          /* [in]  */
    sec_login_flags_t   flags,                  /* [in]  */
    unsigned32          num_local_groups,       /* [in]  */
    sec_id_t            local_groups[],         /* [in]  */
    sec_login_handle_t  *restricted_context,    /* [out] */
    error_status_t      *st                     /* [out] */
  )
#endif
{
    sec_login_context_t *lcp;

    if (DEFAULT_HANDLE(login_context)) {
        SET_STATUS(st, sec_login_s_default_use);
        return false;
    }

    CLEAR_STATUS(st);

    lcp = sec_login_pvt_check_handle(login_context, valid, st);
    if (BAD_STATUS(st)) {
        return false;
    }

    /*
     * Can't use newgroups to expand a user's groupset
     */
    if (num_local_groups > NET_CREDS(lcp).num_groups) {
        SET_STATUS(st, sec_login_s_groupset_invalid);
        return false;
    }

    sec_login_pvt_create_restricted_context(lcp, num_local_groups, local_groups,
                                (sec_login_context_t **)restricted_context, st);

    if (GOOD_STATUS(st)) {
        lcp->flags = flags;
    }

    return GOOD_STATUS(st);
}



/* s e c _ l o g i n _ i n i t _ f i r s t
 *
 */
PUBLIC  void  sec_login_init_first
#ifndef __STDC__
    ( st )
    error_status_t       *st;            /* [out] */
#else
  (
    error_status_t       *st             /* [out] */
  )
#endif
{
    krb5_ccache      ccache;
    char             *env_context;
    char             env[SEC_LOGIN_PAG_NAME_LENGTH+SEC_LOGIN_ENV_NAME_LENGTH];

    CLEAR_STATUS(st);

    /* at DCE 1.0, init first is a no-op, so just return */
    return;

#ifdef NOT_IMPLEMENTED
    /* insist that the caller be root */
    if (geteuid() != 0) {
        SET_STATUS(st, sec_login_s_privileged);
        return;
    }

    /* check for an existing default context, and fail if we find one */
    env_context = getenv(SEC_LOGIN_CCACHE_ENV);
    if (env_context != NULL) {
        SET_STATUS(st, sec_login_s_default_use);
        return;
    }

    /* reserve a kerberos cache file name */
    sec_login_pvt_ccache_gennew (st, &ccache);
    if (BAD_STATUS(st)) {
        return;
    }

    /* establish the inheritance mechanism */
    sprintf(env, "%s=", SEC_LOGIN_CCACHE_ENV);
    setenv(env, krb5_cc_get_name(ccache), 1);

    /* close, but don't destroy, the cache */
    krb5_cc_close(ccache);

    return;
#endif
}


/* s e c _ l o g i n _ s e t u p  _ f i r s t
 *
 */
PUBLIC boolean32 sec_login_setup_first
#ifndef __STDC__
    ( init_context, st )
    sec_login_handle_t   *init_context;  /* [out] */
    error_status_t       *st;            /* [out] */
#else
  (
    sec_login_handle_t   *init_context,  /* [out] */
    error_status_t       *st             /* [out] */
  )
#endif
{
    sec_login_context_t  **lcap = (sec_login_context_t **) init_context;
    char                 *env_context;
    int                  dfd;
    char                 pagfile[SEC_LOGIN_PAG_NAME_LENGTH];
    char                 pagenv[SEC_LOGIN_PAG_NAME_LENGTH];

    *init_context = NULL;

    /* insist that the caller be root */
    if (geteuid() != 0) {
        SET_STATUS(st, sec_login_s_privileged);
        return false;
    }

    /*
     * by virtue of calling sec_login_setup_first, this process has indicated
     * that it wants to manage the machine context, so make sure that's all it will
     * ever get by disabling the default context inheritance mechanism
     */
    unsetenv(SEC_LOGIN_CCACHE_ENV);

    /*
     * try to create the file with the name derived from the
     * the "distinguished" pag value.  If it's already there,
     * then fail.
     */

     sprintf(pagfile, SEC_LOGIN_CCACHE_FILE_FROM_PAG_FMT, SEC_LOGIN_NO_PAG);
     dfd = open(pagfile, O_CREAT | O_EXCL, S_IREAD | S_IWRITE);
     if (dfd < 0) {
         SET_STATUS(st, sec_login_s_default_use);
         return false;
     }

    /*
     * construct a login_context for the host machine principal, using the
     * cache name inherited from the process environment.
     */
    sprintf(pagenv, SEC_LOGIN_CCACHE_FROM_PAG_FMT, SEC_LOGIN_NO_PAG);
    sec_login_pvt_allocate_first(pagenv, lcap, st);

    if (GOOD_STATUS(st)) {
        (*lcap)->state = setup;
        (*lcap)->flags = sec_login_machine_princ;
    }
    else
	unlink(pagfile);

    close(dfd);
    return  GOOD_STATUS(st);
}


PUBLIC void sec_login_valid_from_keytable
#ifndef __STDC__
    (login_context, authn_service, arg, try_kvno, used_kvno, reset_passwd, auth_src, st )
    sec_login_handle_t          login_context;  /* [in] */
    sec_key_mgmt_authn_service  authn_service;  /* [in] */
    void                        *arg;           /* [in] */
    unsigned32                  try_kvno;       /* [in] */
    unsigned32                  *used_kvno;     /* [out] */
    boolean32                   *reset_passwd;  /* [out] */
    sec_login_auth_src_t        *auth_src;      /* [out] */
    error_status_t              *st;            /* [out] */
#else
  (
    sec_login_handle_t          login_context,  /* [in] */
    sec_key_mgmt_authn_service  authn_service,  /* [in] */
    void                        *arg,           /* [in] */
    unsigned32                  try_kvno,       /* [in] */
    unsigned32                  *used_kvno,     /* [out] */
    boolean32                   *reset_passwd,  /* [out] */
    sec_login_auth_src_t        *auth_src,      /* [out] */
    error_status_t              *st             /* [out] */
  )
#endif
{
    sec_login_context_t   *lcp;
    sec_rgy_name_t        princ;
    sec_passwd_rec_t      *key, *kp;
    error_status_t        lst;
    int                   cell_len;
    boolean32             done;

    lcp = sec_login_pvt_check_handle(login_context, setup, st);
    if (BAD_STATUS(st)) {
        return;
    }

    /* construct a global principal name to pass to the key management layer */
    cell_len = u_strlen(lcp->cell);
    u_strcpy(princ, lcp->cell);
    princ[cell_len] = PATH_COMPONENT_SEPARATOR;
    u_strcpy(&princ[cell_len + 1], lcp->principal);

    /*
     * try the requested key version first, assuming that it's usually
     * the correct one
     */
    sec_key_mgmt_get_key(authn_service, arg, princ, try_kvno,
			(void **) &key, st);
    if (GOOD_STATUS(st)) {
        done = false;
        for (kp = key;
             PASSWD_TYPE(kp) != sec_passwd_none && !done;
             kp++) {
            done = sec_login_validate_identity(login_context, kp, reset_passwd,
                                               auth_src, st);
            if (done) {
                *used_kvno = kp->version_number;
            }
        }
        sec_key_mgmt_free_key(key, &lst);
    }

    if (BAD_STATUS(st)) {
        /*
         * the suggested key version number didn't work, so loop through the key
         * table, just in case there's more than one key entry for
         * the principal.
         */
        void               *cursor;
        unsigned32         kvno;
        sec_passwd_type_t  keytype = sec_passwd_none;

        sec_key_mgmt_initialize_cursor(authn_service, arg, princ, &keytype,
                                       &cursor, &lst);
        if (BAD_STATUS(&lst)) {
            /* return with prior failure status */
            return;
        }

        sec_key_mgmt_get_next_key(cursor, NULL, &kvno, (void **) &key, &lst);
        while (GOOD_STATUS(&lst) && BAD_STATUS(st)) {
            for (kp = key;
                 PASSWD_TYPE(kp) != sec_passwd_none && BAD_STATUS(st);
                 kp++) {
                sec_login_validate_identity(login_context, kp, reset_passwd,
                                            auth_src, st);
                if (GOOD_STATUS(st)) {
                    *used_kvno = kp->version_number;
                }
            }
            /* done with host key, in any case */
            sec_key_mgmt_free_key(key, &lst);
            if (BAD_STATUS(st)) {
                sec_key_mgmt_get_next_key(cursor, NULL, &kvno,
			(void **) &key, &lst);
            }
        }

        sec_key_mgmt_release_cursor(&cursor, &lst);
    }
}

/* s e c _ l o g i n _ v a l i d a t e  _ f i r s t
 *
 */
PUBLIC boolean32 sec_login_validate_first
#ifndef __STDC__
    ( init_context, reset_passwd, auth_src, st )
    sec_login_handle_t      init_context;   /* [in]  */
    boolean32               *reset_passwd;  /* [out] */
    sec_login_auth_src_t    *auth_src;      /* [out] */
    error_status_t          *st;            /* [out] */
#else
  (
    sec_login_handle_t      init_context,   /* [in]  */
    boolean32               *reset_passwd,  /* [out] */
    sec_login_auth_src_t    *auth_src,      /* [out] */
    error_status_t          *st             /* [out] */
  )
#endif
{
    sec_login_context_t   *lcp = (sec_login_context_t *) init_context;
    unsigned32            used_kvno;

    /* insist that the caller be root */
    if (geteuid() != 0) {
        SET_STATUS(st, sec_login_s_privileged);
        return false;
    }

    sec_login_valid_from_keytable(init_context, rpc_c_authn_dce_secret, NULL,
                                  sec_passwd_c_version_none, &used_kvno,
                                  reset_passwd, auth_src, st);


    if (GOOD_STATUS(st)) {
        /* Because this is the initial context, we can mark it certifed
         * right off because by sucessfully decrypting the tgt, we've
         * proved there's a trust path to the principal's KDC
         * (because in this case the principal is the host machine itself,
         * and the trust is based on the ability to get to a KDC that knows
         * the host machine's key)
         */
        lcp->state = certified;
        lcp->identity->state = info_certified;
        sec_login_pvt_synch_first(lcp, st);
    }

    return  GOOD_STATUS(st);
}


/* s e c _ l o g i n _ s e t _ c o n t e x t
 *
 * Set the network credentials to those specified by the login context.
 * This context must have been previously validated (contexts acquired
 * through sec_login_get_current_context or sec_login_newgroups don't need
 * to be validated since those functions return  previously validated
 * contexts).  If the login context is authenticated locally,
 * this operation establishes enough context to allow access to
 * distributed file system volumes stored on the local machine's
 * disk, but no network identity is established (i.e., if the
 * Passwd Etc server is unavailable, or the user's password is
 * customized for the local machine, the user can see file system
 * volumes that are local, but has no access to authenticated
 * network services)
 *
 */
PUBLIC void sec_login_set_context
#ifndef __STDC__
    ( login_context, st )
    sec_login_handle_t   login_context;  /* [in]  */
    error_status_t       *st;            /* [out] */
#else
  (
    sec_login_handle_t   login_context,  /* [in]  */
    error_status_t       *st             /* [out] */
  )
#endif
{
    sec_login_context_t  *lcp;
    error_status_t       lst;

    lcp = sec_login_pvt_check_handle(login_context, valid, st);
    if (BAD_STATUS(st)) {
        return;
    }

    /*
     * DCE 1.0 restriction - can't do a set context if the context was
     * marked sec_login_credentials_private.  Note that this means
     * such a context can only be used as an explicit context in user
     * space applications.  It cannot be used to make authenticated
     * kernel calls, because these rely on the default context.
     */
    if (FLAG_SET(lcp->flags, sec_login_credentials_private)) {
        SET_STATUS(st, sec_login_s_context_invalid);
        return;
    }

    sec_login_pvt_set_default_context(login_context, st);
}


PUBLIC void sec_login_unset_context (void)
{
    error_status_t xst;
    
    sec_login_pvt_unset_default_context();
}

/* s e c _ l o g i n _ g e t _ c u r r e n t _ c o n t e x t
 *
 * Retrieve a handle to the login context for the currently established
 * network identity.  The context is created from locally cached data
 * so subsequent data extraction operations will return some NULL values.
 *
 * ERRORS:
 *    sec_login_s_no_current_context
 */
PUBLIC void sec_login_get_current_context
#ifndef __STDC__
    ( login_context, st )
    sec_login_handle_t  *login_context;     /* [out] */
    error_status_t      *st;                /* [out] */
#else
  (
    sec_login_handle_t  *login_context,     /* [out] */
    error_status_t      *st                 /* [out] */
  )
#endif
{

    *login_context = sec_login_pvt_get_default_context(st);
}


/* s e c _ l o g i n _ p u r g e _ c o n t e x t
 *
 * free any storage allocated for a login context and destroy the associated
 * network credentials, if any.  Applications should be cautious when purging
 * the current context as this destroys network credentials for all
 * processes that share the credentials.
 *
 * ERRORS:
 *    sec_login_s_handle_invalid
 *    sec_login_s_default_use
 *
 * The handle parameter is in,out so that it can be reset to NULL.
 * If the context being purged is the same as the currently established
 * default context, then the internal default context state is purged as
 * well.
 */
PUBLIC void sec_login_purge_context
#ifndef __STDC__
    ( login_context, st )
    sec_login_handle_t   *login_context;  /* [in, out] */
    error_status_t       *st;             /* [out]     */
#else
  (
    sec_login_handle_t   *login_context,  /* [in, out] */
    error_status_t       *st              /* [out] */
  )
#endif
{
    sec_login_context_t  *lcp;
    char                 *cname;
    char		mcname[SEC_LOGIN_PAG_NAME_LENGTH];

    if (DEFAULT_HANDLE(login_context)) {
        SET_STATUS(st, sec_login_s_default_use);
        return;
    }

    CLEAR_STATUS(st);

    lcp = sec_login_pvt_check_handle(*login_context, setup, st);
    if (BAD_STATUS(st)) {
        return;
    }
    cname = lcp->context_name;
    if (cname != NULL) {
        sprintf(mcname, SEC_LOGIN_CCACHE_FROM_PAG_FMT, SEC_LOGIN_NO_PAG);
        if (u_strcmp(mcname, cname) == 0) {
            SET_STATUS(st, sec_login_s_default_use);
            return;
        }
    }
    sec_login_pvt_purge_context(lcp, st);
    *login_context = NULL;
}


/* s e c _ l o g i n _ p u r g e _ c o n t e x t _ e x p
 *
 * Applications can't import expired contexts in order to purge them,
 * so do it for them.
 *
 */
PUBLIC void sec_login_purge_context_exp
  (
    unsigned32		buf_len,	/* [in]  */
    ndr_byte		buf[],		/* [in]  */
    signed32		purge_time,	/* [in]  */
    error_status_t	*st		/* [out] */
  )
{
    sec_login_context_t *lcp;
    signed32 exp_time;

    krb5_ccache cache;
    krb5_cc_cursor cur;
    krb5_creds cred;
    char *cname;
    int cname_len;
    uuid_t  generation_id;

    CLEAR_STATUS(st);

    sec_login_pvt_parse_export_token(buf, buf_len, &cname, &cname_len, 
				     &generation_id, st);
    if (BAD_STATUS(st)) {
	return;
    }

    lcp = sec_login_cache_lookup (cname_len, cname, &generation_id);
    if (! lcp)
	sec_login_pvt_allocate_login_context(false,
		(unsigned_char_p_t) NULL, cname, &lcp, st);

    if (GOOD_STATUS(st)) {
	sec_login_pvt_inq_identity_expiration(lcp,
		(unsigned32 *) &exp_time, st);

	if (GOOD_STATUS(st) && exp_time < purge_time)
	    sec_login_pvt_purge_context(lcp, st);
	else
	    sec_login_pvt_release_context(lcp, false); /* don't force cache remove */
	return;
    }

    /*
     * Try as a krb singleton
     */
    if (krb5_cc_resolve(cname, &cache) == 0) {
	if (krb5_cc_start_seq_get(cache, &cur) == 0) {
	    if (krb5_cc_next_cred(cache, &cur, &cred) == 0) {

		CLEAR_STATUS(st);

		if (cred.times.endtime < purge_time)
		    *st = krb5_cc_destroy(cache);
		else {
		    krb5_cc_end_seq_get(cache, &cur);
		    krb5_cc_close(cache);
		}
		return;
	    }
	}
    }

    /* Probably leftover from unsuccessful auth; just rm */
    if (!u_strncmp(cname, SEC_LOGIN_CCACHE_PREFIX,
		    sizeof(SEC_LOGIN_CCACHE_PREFIX)-1)) {
	struct stat sb;

	cname += sizeof(SEC_LOGIN_CCACHE_PREFIX)-1;
	if (stat(cname, &sb) == 0 && sb.st_mtime < purge_time)
	    unlink(cname);
	CLEAR_STATUS(st);
    }
}


/* s e c _ l o g i n _ r e l e a s e _ c o n t e x t
 *
 * free any storage allocated for a login context.  Unlike
 * sec_login_purge_context this function does not destroy the associated
 * network credentials.  If the network credentials are in use by other
 * processes or are referred to by other sec_login_handles, they will continue to
 * exist.
 *
 * ERRORS:
 *    sec_login_s_handle_invalid
 *    sec_login_s_context_invalid
 *    sec_login_s_default_use
*/
PUBLIC void sec_login_release_context
#ifndef __STDC__
    ( login_context, st )
    sec_login_handle_t  *login_context;
    error_status_t      *st;
#else
  (
    sec_login_handle_t  *login_context,
    error_status_t      *st
  )
#endif
{
    sec_login_context_t  *lcp;

    if (DEFAULT_HANDLE(login_context)) {
        SET_STATUS(st, sec_login_s_default_use);
        return;
    }

    CLEAR_STATUS(st);

    lcp = sec_login_pvt_check_handle(*login_context, setup, st);
    if (BAD_STATUS(st)) {
        return;
    }

    /*
     * if we're releasing a handle on the current context, then cleanup
     * the internal state info as well
     */
    if (lcp->auth_src == sec_login_auth_src_network) {
        (void) sec_login_pvt_purge_default_context(*login_context);
    }

    sec_login_pvt_release_context(lcp, false); /* don't force cache remove */

    *login_context = NULL;
}


/* The following routines extract data from a login context.  These routines
** do not require a previously validated context, but return a
** sec_login_s_not_certified warning status when the input context
** has not been previously certified.
*/

/* s e c _ l o g i n _ g e t _ p w e n t
 *
 * extract a passwd file entry for the current network login state.  This
 * is generally useful for establishing the local OS context.  If the
 * struct passwd style info has not already been constructed (which it may
 * not have been if the login context was constructed by
 * sec_login_get_current_context), then go to the registry to get it.
 *
 * NOTE:
 *  the passwd structure will point to data stored in the login_context and
 *  should be treated as read-only data.  Writing to these data objects may
 *  cause unexpected failures.
 *
 * ERRORS:
 *    sec_login_s_handle_invalid
 *    sec_login_s_context_invalid
 *    sec_login_s_info_not_avail
 *    sec_login_s_not_certified
 *    sec_rgy_unavailable
 */
PUBLIC void sec_login_get_pwent
#ifndef __STDC__
    ( login_context, pw, st )
    sec_login_handle_t  login_context;  /* [in]  */
    sec_login_passwd_t  *pw;            /* [out] */
    error_status_t      *st;            /* [out  */
#else
  (
    sec_login_handle_t  login_context,  /* [in]  */
    sec_login_passwd_t  *pw,            /* [out] */
    error_status_t      *st             /* [out] */
  )
#endif
{
    sec_login_context_t     *lcp;
    context_state_t         min_state;
    sec_override_fields_t   overridden;

    if (DEFAULT_HANDLE(login_context)) {
        SET_STATUS(st, sec_login_s_default_use);
        return;
    }

    CLEAR_STATUS(st);
    *pw = NULL;
    min_state = semisetup;

    lcp = sec_login_pvt_check_handle(login_context, min_state, st);
    if (BAD_STATUS(st)) {
        return;
    }

    /*
     * if the context has not been certified, then try to get local passwd
     * data first, because it is protected by local security, and therefore
     * certified by default.  We'll throw it away and replace it with network
     * data if and when the context gets certified.  If there is no
     * local passwd data, we continue on to try the network, but flag
     * the data as not certified.
     */
    if ((lcp->identity->state != info_certified) &&
        (lcp->auth_src != sec_login_auth_src_overridden)) {

        sec_login_pvt_get_local_pwent(lcp, st);
        if (GOOD_STATUS(st)) {
            sec_login_np_lc_to_pwent(lcp, pw, st);
            return;
        } else {
            CLEAR_STATUS(st);   /* continue on to the network */
        }
    }

    sec_login_pvt_get_login_info(true, lcp, &overridden, st);
    if (GOOD_STATUS(st)) {
        /* hand back a pointer to the context passwd data */
        sec_login_np_lc_to_pwent(lcp, pw, st);
        if (*pw == NULL) {
            SET_STATUS(st, sec_login_s_info_not_avail);
        } else if (lcp->identity->state != info_certified) {
            SET_STATUS(st, sec_login_s_not_certified);
        }
    }
}


/* s e c _ l o g i n _ g e t _ g r o u p s
 *
 * extract the concurrent groupset for which the user has established
 * network privileges.
 *
 * ERRORS:
 *    sec_login_s_handle_invalid
 *    sec_login_s_context_invalid
 */
PUBLIC void sec_login_get_groups
#ifndef __STDC__
    ( login_context, num_groups, group_set, st)
    sec_login_handle_t  login_context;     /* [in]  */
    unsigned32          *num_groups;       /* [out] */
    signed32            **group_set;       /* [out] */
    error_status_t      *st;               /* [out] */
#else
  (
    sec_login_handle_t  login_context,     /* [in]  */
    unsigned32          *num_groups,       /* [out] */
    signed32            **group_set,       /* [out] */
    error_status_t      *st                /* [out] */
  )
#endif
{
    sec_login_context_t     *lcp;
    sec_override_fields_t   overridden;

    CLEAR_STATUS(st);
    *group_set = NULL;
    *num_groups = 0;

    lcp = sec_login_pvt_check_handle(login_context, setup, st);
    if (BAD_STATUS(st)) {
        return;
    }

    sec_login_pvt_get_login_info(false, lcp, &overridden, st);
    if (GOOD_STATUS(st)) {
        if (UNIX_CREDS(lcp).pwent == NULL) {
            /* we never got any unix info */
            *num_groups = 0;
            *group_set =  NULL;
            SET_STATUS(st, sec_login_s_info_not_avail);
        } else {
            /* hand back a pointer to the context unix groupset data */
            *num_groups = UNIX_CREDS(lcp).num_groups;
            *group_set =  UNIX_CREDS(lcp).groups;

           if (lcp->identity->state != info_certified) {
                SET_STATUS(st, sec_login_s_not_certified);
            }
        }
    }
}


/* s e c _ l o g i n _ g e t _ e x p i r a t i o n
 *
 * Extract the expiration date for the authentication identity from
 * the login context.  The expiration time is filled in if it is available,
 * otherwise set to 0.  This allows the application to provide feedback
 * as to when the user's identity will need to be refreshed.
 *
 * ERRORS:
 *    sec_login_s_not_certified
 */
PUBLIC void sec_login_get_expiration
#ifndef __STDC__
    ( login_context, identity_expiration, st )
    sec_login_handle_t  login_context;          /* [in]  */
    signed32            *identity_expiration;   /* [out] */
    error_status_t      *st;                    /* [out] */
#else
  (
    sec_login_handle_t  login_context,          /* [in]  */
    signed32            *identity_expiration,   /* [out] */
    error_status_t      *st                     /* [out] */
  )
#endif
{
    sec_login_context_t *lcp;

    CLEAR_STATUS(st);
    *identity_expiration = 0;

    if (DEFAULT_HANDLE(login_context)) {
        lcp = sec_login_pvt_get_default_context(st);
    } else {
        lcp = sec_login_pvt_check_handle(login_context, valid, st);
    }

    if (BAD_STATUS(st)) {
        return;
    }

    sec_login_pvt_inq_identity_expiration(lcp,
                                          (unsigned32 *) identity_expiration,
                                           st);

    if (GOOD_STATUS(st) && lcp->identity->state != info_certified) {
        SET_STATUS(st, sec_login_s_not_certified);
    }
}

/*
**  Miscellaneous Routines
*/

/* s e c _ l o g i n _ c h e c k _ p a s s w d
 *
 * Verify the user's knowledge of their password.  This function will
 * return true if the password matches the one belonging to the named
 * user, false otherwise.  This function should be used in place of the
 * conventional comparison against the encrypted password in the passwd
 * file structure.  This function is only useful for checking the user's
 * knowledge of the password - it cannot be used to set network credentials
 * (since it does not establish a login context).
 *
 * This function will apply any appropriate local overrides and will
 * attempt to access the local user registry if the network registry is
 * unavailable.
 */
PUBLIC boolean32 sec_login_check_passwd
#ifndef __STDC__
    ( principal, passwd, auth_src, st )
    unsigned_char_p_t    principal;
    sec_passwd_rec_t     *passwd;
    sec_login_auth_src_t *auth_src;
    error_status_t       *st;
#else
  (
    unsigned_char_p_t    principal,
    sec_passwd_rec_t     *passwd,
    sec_login_auth_src_t *auth_src,
    error_status_t       *st
  )
#endif
{
    SET_STATUS(st, sec_rgy_not_implemented);
    return GOOD_STATUS(st);

}


/* s e c _ l o g i n _ e x p o r t _ c o n t e x t
 *
 * Obtain an exportable version of the the login context information.
 * This information may be passed to another process running on the
 * same machine.
 *
 * Errors:
 *  sec_login_s_no_mem            - buffer too short, len_needed parm
 */
PUBLIC void sec_login_export_context
#ifndef __STDC__
    ( login_context, buf_len, buf, len_used, len_needed, st )
    sec_login_handle_t  login_context;  /* [in]  */
    unsigned32          buf_len;        /* [in]  */
    ndr_byte            buf[];          /* [out] */
    unsigned32          *len_used;      /* [out] */
    unsigned32          *len_needed;    /* [out] */
    error_status_t      *st;            /* [out] */
#else
  (
    sec_login_handle_t  login_context,  /* [in]  */
    unsigned32          buf_len,        /* [in]  */
    ndr_byte            buf[],          /* [out] */
    unsigned32          *len_used,      /* [out] */
    unsigned32          *len_needed,    /* [out] */
    error_status_t      *st             /* [out] */
  )
#endif
{
    sec_login_context_t *lcp;
    char                *cfn;   /* kerberos credential cache filename */
    time_t		now;

    CLEAR_STATUS(st);
    *len_used = 0;
    *len_needed = 0;

    /* can only export a valid context */
    if (DEFAULT_HANDLE(login_context)) {
        lcp = sec_login_pvt_get_default_context(st);
    } else {
        lcp = sec_login_pvt_check_handle(login_context, valid, st);
    }

    if (GOOD_STATUS(st)) {
	if (buf_len != 0)
	    sec_login_pvt_check_exp(lcp, st);
	if (GOOD_STATUS(st)) {
	
#ifdef FIXED4851
	    /* Make sure the context has been finalized */
	    if (KRB_INFO(lcp).num_caches == 0) {
		sec_login_pvt_set_context(login_context, false, st);
	    }
#endif
	    
	    sec_login_pvt_gen_export_token(lcp, buf_len, buf, len_needed, st);
	    if (GOOD_STATUS(st) && *len_needed <= buf_len) {
		*len_used = *len_needed;
	    }
	}
    }

    DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
		   "sec_login_export_context: status = (%x)", *st));
}


/* s e c _ _ l o g i n _ i m p o r t _ c o n t e x t
 *
 * Import a context obtained via a call to sec_login_export_context
 * performed on the same machine.  This function is generally reserved to
 * the privileged user.
 * 
 * Internal base class rtn with extra boolean arg for cc access checking.
 */
PRIVATE void sec__login_import_context
  (
    unsigned32          buf_len,         /* [in]  */
    ndr_byte            buf[],           /* [in]  */
    signed32            uid,             /* [in]  */
    sec_login_handle_t  *login_context,  /* [out] */
    error_status_t      *st              /* [out] */
  )
{
    sec_login_context_t **lcap = (sec_login_context_t **) login_context;
    sec_login_context_t *lcp;
    char                *cname;
    int                 cname_len;
    uuid_t              generation_id;

    CLEAR_STATUS(st);

    sec_login_pvt_parse_export_token(buf, buf_len, &cname, &cname_len, 
				     &generation_id, st);
    if (BAD_STATUS(st)) {
	*lcap = NULL;
	return;
    }

    /* Existing login context in cache with that cname? */
    lcp = sec_login_cache_lookup(cname_len, cname, &generation_id);
    if (lcp)
    {
	DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
		       "sec_login_import_context: cache hit! (%s)",
		       cname));
	sec_login_pvt_check_exp(lcp, st);
	if (BAD_STATUS(st)) {
	    DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
			   "sec_login_import_context: context (%s) has expired",
			   cname));
	    sec_login_pvt_release_context(lcp, false);
	    return;
	}
        *lcap = lcp;

        /* Make sure creds are owned by the specified UID before proceeding */
        if ((uid != -1) && lcp->owner_uid != uid) {
            SET_STATUS(st, sec_login_s_no_current_context);
	    *lcap = NULL;
	    sec_login_pvt_release_context(lcp, false);
	    return;
        }

        return;
    }

    DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
		   "sec_login_import_context: cache miss (%s)",
		   cname));

    /* get user from cache */
    sec_login_pvt_allocate_login_context(false, (unsigned_char_p_t) NULL,
        cname, lcap, st);
    if (GOOD_STATUS(st))
    {
        /* Make sure creds are owned by the specified UID before proceeding */
        if ((uid != -1) && (*lcap)->owner_uid != uid) {
            SET_STATUS(st, sec_login_s_no_current_context);
	    sec_login_pvt_release_context(*lcap, true);
	    *lcap = NULL;
	    return;
        }

	sec_login_pvt_check_exp(*lcap, st);
	if (BAD_STATUS(st)) {
	    DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
			   "sec_login_import_context: context (%s) has expired",
			   cname));
	    /* don't force cache remove */
	    sec_login_pvt_release_context(*lcap, false);
	}
    }

    if (BAD_STATUS(st))
    {
	DCE_SVC_DEBUG((sec_svc_handle, sec_s_login, svc_c_debug1,
		       "sec_login_import_context: failure status = (%x)",
		       *st));
        *lcap = NULL;
    }
}


/* s e c _ l o g i n _ i m p o r t _ c o n t e x t _ u i d
 *
 * Import a context obtained via a call to sec_login_export_context
 * performed on the same machine.  This function is generally reserved to
 * the privileged user.
 *
 */
PUBLIC void sec_login_import_context_uid
  (
    unsigned32          buf_len,         /* [in]  */
    ndr_byte            buf[],           /* [in]  */
    signed32            uid,             /* [in]  */
    sec_login_handle_t  *login_context,  /* [out] */
    error_status_t      *st              /* [out] */
  )
{
    sec__login_import_context(buf_len, buf, uid, login_context, st);
}


/* s e c _ l o g i n _ i m p o r t _ c o n t e x t
 *
 * Import a context obtained via a call to sec_login_export_context
 * performed on the same machine.  This function is generally reserved to
 * the privileged user.
 *
 */
PUBLIC void sec_login_import_context
  (
    unsigned32          buf_len,         /* [in]  */
    ndr_byte            buf[],           /* [in]  */
    sec_login_handle_t  *login_context,  /* [out] */
    error_status_t      *st              /* [out] */
  )
{
    sec__login_import_context(buf_len, buf, -1, login_context, st);
}


PUBLIC  void  sec_login_inquire_net_info
#ifndef __STDC__
    ( login_context, net_info, st )
    sec_login_handle_t    login_context;
    sec_login_net_info_t  *net_info;
    error_status_t        *st;
#else
  (
    sec_login_handle_t    login_context,
    sec_login_net_info_t  *net_info,
    error_status_t        *st
  )
#endif
{
    sec_login_context_t *lcp;

    CLEAR_STATUS(st);
    memset(net_info, 0, sizeof(*net_info));

    if (DEFAULT_HANDLE(login_context)) {
        lcp = sec_login_pvt_get_default_context(st);
    } else {
        lcp = sec_login_pvt_check_handle(login_context, valid, st);
    }
    if (BAD_STATUS(st)) {
        return;
    }

    net_info->acct_expiration_date = POLICY_INFO(lcp).acct_exp;
    net_info->passwd_expiration_date = POLICY_INFO(lcp).passwd_exp;

    sec_login_pvt_inq_pac(lcp, &net_info->pac, st);
    if (GOOD_STATUS(st)) {
        sec_login_pvt_inq_identity_expiration(lcp,
                                              &net_info->identity_expiration_date,
                                              st);
    }

    if (GOOD_STATUS(st) && lcp->identity->state != info_certified) {
        SET_STATUS(st, sec_login_s_not_certified);
    }
}


PUBLIC  void  sec_login_free_net_info
#ifndef __STDC__
    ( net_info )
    sec_login_net_info_t  *net_info;
#else
  (
    sec_login_net_info_t  *net_info
  )
#endif
{
    if (net_info != NULL) {
        sec_id_pac_util_free(&net_info->pac);
    }
}


/*
 * sec_login_register_ccache - Necessary to provide an entrypoint to the
 * kerberos routine that allows you to register an alternate ccache manager
 */
PUBLIC void sec_login_register_ccache
#ifndef __STDC__
    (ops, override, stp)
    void                *ops;
    boolean32 	        override;
    error_status_t      *stp;
#else
  (
    void                *ops,
    boolean32           override,
    error_status_t      *stp
  )
#endif
{
   *stp = (error_status_t)
       krb5_cc_register((krb5_cc_ops *) ops, (krb5_boolean) override);
}


/*
 * s e c _ l o g i n _ k r b 5 _ a d d _ c r e d
 *
 * Add an externally-originated TGT to the credential cache.
 */
PUBLIC void sec_login_krb5_add_cred
#ifndef __STDC__
    ( login_context, ticket, st )
    sec_login_handle_t  *login_context; /* [in] */
    krb5_creds 		*ticket;        /* [in] */
    error_status_t      *st;            /* [out] */
#else
  (
    sec_login_handle_t  *login_context, /* [in] */
    krb5_creds          *ticket,        /* [in] */
    error_status_t      *st             /* [out] */
  )
#endif
{
    sec_login_context_t		*lcp;

    lcp = sec_login_pvt_check_handle(login_context, setup, st);
    if (BAD_STATUS(st)) {
	return;
    }
    *st = krb5_cc_store_cred(lcp->krb_info.cache, ticket);
}


/*
 * s e c _ l o g i n _ i n q  _ p a g
 *
 * Obtain pag from login context.
 */
PUBLIC unsigned32 sec_login_inq_pag
#ifndef __STDC__
    ( login_context, st )
    sec_login_handle_t  *login_context; /* [in] */
    error_status_t      *st;            /* [out] */
#else
  (
    sec_login_handle_t  *login_context, /* [in] */
    error_status_t      *st             /* [out] */
  )
#endif
{
    sec_login_context_t		*lcp;

    lcp = sec_login_pvt_check_handle(login_context, setup, st);
    if (BAD_STATUS(st)) {
	return(0);
    }
    return(lcp->v1_1_info.pag);
}


/*
 * s e c _ l o g i n _ c o n t e x t _ f r o m _ p a g
 *
 * Obtain login_context from pag.
 */
PUBLIC void sec_login_context_from_pag
#ifndef __STDC__
    ( pag, login_context, st )
    unsigned32 		pag;                    /* [in] */
    sec_login_handle_t  *login_context;         /* [out] */
    error_status_t      *st;                    /* [out] */
#else
  (
    unsigned32 		pag,                    /* [in] */
    sec_login_handle_t  *login_context,         /* [out] */
    error_status_t      *st                     /* [out] */
  )
#endif
{
    char 		pagfile[SEC_LOGIN_PAG_NAME_LENGTH];
    /*
     * Construct cred cache name from pag, look in context cache.
     */
    sprintf(pagfile, SEC_LOGIN_CCACHE_FROM_PAG_FMT, pag);
    sec_login_import_context(strlen(pagfile), (ndr_byte *)pagfile,
        login_context, st);
}


/* sec_login_become_initiator
 *
 * This functions constructs a new login context that enables the selected
 * delegation type.
 * The parameter my_login_context specifies the identity of the caller.
 * Semantics of arguments
 *   my_login_context -          specifies the identity of the caller.
 *   delegation_type_permitted - determines the delegation type to be per-
 *                               mitted using the generated login context.
 *   delegate_restrictions -     the list of delegates that are permitted.
 *   target_restrictions -       the list of targets to whom this identity
 *                               may be presented.
 *   optional_restrictions -     the list of application defined optional
 *                               restrictions that apply to this delegation
 *                               context.
 *   required_restrictions -     the list of application defined required
 *                               restrictions that apply to this delegation
 *                               context.
 *   compatibility_mode -        specifies the compatibility mode desired
 *                               when operating on DCE 1.0 servers.
 *   error_status -              error status.
 */
PUBLIC sec_login_handle_t sec_login_become_initiator (
    sec_login_handle_t	        my_login_context,
    sec_id_delegation_type_t    delegation_type_permitted,
    sec_id_restriction_set_t    *delegate_restrictions,
    sec_id_restriction_set_t    *target_restrictions,
    sec_id_opt_req_t            *optional_restrictions,
    sec_id_opt_req_t            *required_restrictions,
    sec_id_compatibility_mode_t compatibility_mode,
    error_status_t              *stp
)
{
    sec_login_context_t *lcp;
    static sec_id_restriction_set_t dlg = {0, 0}, tgt = {0, 0};
    static sec_id_opt_req_t         opt = {0, 0}, req = {0, 0};
    CLEAR_STATUS(stp);

    lcp = sec_login_pvt_check_handle(my_login_context, valid, stp);
    if (BAD_STATUS(stp)) {
	return(NULL);
    }

    /* Allow NULL passed in for restrictions to mean none */
    if (delegate_restrictions == NULL)
        delegate_restrictions = &dlg;
    if (target_restrictions == NULL)
        target_restrictions = &tgt;
    if (optional_restrictions == NULL)
        optional_restrictions = &opt;
    if (required_restrictions == NULL)
        required_restrictions = &req;

    return(sec_login_pvt_become_initiator(lcp, delegation_type_permitted,
	delegate_restrictions, target_restrictions, optional_restrictions,
	required_restrictions, compatibility_mode, stp));
}


/* sec_login_become_delegate
 *
 * This function is used by intermediate servers to become a delegate for
 * their caller.
 * Semantics of arguments
 *   callers_identity - specifies the identity of the server's RPC client
 *                      as obtained from the RPC runtime.
 *   my_login_context - specifies the identity of the intermediate server.
 *                      This context must be a simple context (i.e., this
 *                      cannot be a context that represents a compound
 *                      identity created by the become_delegate function).
 *   delegation_type_permitted - determines the delegation type to be
 *                      permitted using the generated login context.
 *   delegate_restrictions - the list of delegates that are permitted
 *   target_restrictions -	 the list of targets to whom this identity may
 *                      be presented.
 *   optional_restrictions - the list of application defined optional
 *                      restrictions that apply to this delegation context.
 *   required_restrictions - the list of application defined required
 *                      restrictions that apply to this delegation context.
 *   compatibility_mode - specifies the compatibility mode desired when
 *                      operating on DCE 1.0 servers.
 *   error_status -	    error status.
 */
PUBLIC sec_login_handle_t sec_login_become_delegate (
    rpc_authz_cred_handle_t     callers_identity,
    sec_login_handle_t          my_login_context,
    sec_id_delegation_type_t    delegation_type_permitted,
    sec_id_restriction_set_t    *delegate_restrictions,
    sec_id_restriction_set_t    *target_restrictions,
    sec_id_opt_req_t            *optional_restrictions,
    sec_id_opt_req_t            *required_restrictions,
    sec_id_compatibility_mode_t compatibility_mode,
    error_status_t              *stp
)
{
    sec_login_context_t *lcp;
    static sec_id_restriction_set_t dlg = {0, 0}, tgt = {0, 0};
    static sec_id_opt_req_t         opt = {0, 0}, req = {0, 0};
    CLEAR_STATUS(stp);

    lcp = sec_login_pvt_check_handle(my_login_context, valid, stp);
    if (BAD_STATUS(stp)) {
	return(NULL);
    }

    /* Allow NULL passed in for restrictions to mean none */
    if (delegate_restrictions == NULL)
        delegate_restrictions = &dlg;
    if (target_restrictions == NULL)
        target_restrictions = &tgt;
    if (optional_restrictions == NULL)
        optional_restrictions = &opt;
    if (required_restrictions == NULL)
        required_restrictions = &req;

    return(sec_login_pvt_do_delegation_request(callers_identity, lcp,
	sec__login_c_dlg_req_traced, delegation_type_permitted, 
        delegate_restrictions, target_restrictions,
	optional_restrictions, required_restrictions, 
        compatibility_mode,stp));
}


/* sec_login_become_impersonator
 *
 * This function is used by intermediate servers to become an impersonator
 * for their caller.
 * Semantics of arguments
 *   callers_identity - specifies the identity of the server's RPC client
 *                      as obtained from the RPC runtime.
 *   delegation_type_permitted - determines the delegation type to be
 *                      permitted using the generated login context.
 *   delegate_restrictions - the list of delegates that are permitted
 *   target_restrictions - the list of targets to whom this identity may be
 *                      presented.
 *   optional_restrictions - the list of application defined optional
 *                      restrictions that apply to this delegation context.
 *   required_restrictions - the list of application defined required
 *                      restrictions that apply to this delegation context.
 *   error_status -     error status.
 */
PUBLIC sec_login_handle_t sec_login_become_impersonator (
    rpc_authz_cred_handle_t     callers_identity,
    sec_login_handle_t          my_login_context,
    sec_id_delegation_type_t    delegation_type_permitted,
    sec_id_restriction_set_t    *delegate_restrictions,
    sec_id_restriction_set_t    *target_restrictions,
    sec_id_opt_req_t            *optional_restrictions,
    sec_id_opt_req_t            *required_restrictions,
    error_status_t              *stp
)
{
    sec_login_context_t *lcp;
    static sec_id_restriction_set_t dlg = {0, 0}, tgt = {0, 0};
    static sec_id_opt_req_t         opt = {0, 0}, req = {0, 0};
    CLEAR_STATUS(stp);

    lcp = sec_login_pvt_check_handle(my_login_context, valid, stp);
    if (BAD_STATUS(stp)) {
	return(NULL);
    }

    /* Allow NULL passed in for restrictions to mean none */
    if (delegate_restrictions == NULL)
        delegate_restrictions = &dlg;
    if (target_restrictions == NULL)
        target_restrictions = &tgt;
    if (optional_restrictions == NULL)
        optional_restrictions = &opt;
    if (required_restrictions == NULL)
        required_restrictions = &req;

    return(sec_login_pvt_do_delegation_request(callers_identity, lcp,
	sec__login_c_dlg_req_imp, delegation_type_permitted, 
        delegate_restrictions, target_restrictions,
	optional_restrictions, required_restrictions, 
        sec_id_compat_mode_none /* placeholder */, stp));
}


/* sec_login_set_extended_attrs
 *
 * This function constructs a new login context that contains the requested
 * extended attributes.
 * Attributes cannot be added to a delegation chain in this manner, thus if
 * a login context referring to a delegation chain is passed to this call,
 * an invalid context error will be returned.
 * Semantics of arguments
 *   my_login_context - specifies the identity of the caller.
 *   num_attributes - the number of attributes passed in this call.
 *   attributes - the list of attributes to be set in the new login context
 *       If any of the requested attributes are not authorized for the
 *       caller, none are set and a vector of indices listing the invalid
 *       attributes is returned.
 *   error_status - error status.
 */
PUBLIC sec_login_handle_t sec_login_set_extended_attrs (
    sec_login_handle_t          my_login_context,
    unsigned32                  num_attributes,
    sec_attr_t                  attributes[],
    error_status_t              *stp
)
{
    sec_login_context_t *lcp;
    CLEAR_STATUS(stp);

    lcp = sec_login_pvt_check_handle(my_login_context, valid, stp);
    if (BAD_STATUS(stp)) {
        return(NULL);
    }
    return(sec_login_pvt_set_ext_attrs(lcp, num_attributes, attributes, stp));
}


/* sec_login_disable_delegation
 *
 * This function returns a login context without delegation or
 * impersonation enabled, from one that has one of the two delegation_types
 * enabled.
 * Semantics of arguments
 *  login_context - specifies the identity of the caller
 *  error_status - error_status.
 */
PUBLIC sec_login_handle_t sec_login_disable_delegation(
    sec_login_handle_t          login_context,
    error_status_t              *stp
)
{
    sec_login_context_t *lcp;
    CLEAR_STATUS(stp);

    lcp = sec_login_pvt_check_handle(login_context, valid, stp);
    if (BAD_STATUS(stp)) {
        return(NULL);
    }
    return(sec_login_pvt_disable_dlg(lcp, stp));
}


/* sec_login_cred_get_initiator
 *
 * This function is used to extract the initiator's privilege attributes
 * from a specified login context.
 * Semantics of arguments
 * login_context - 		specifies the login context to extract the
 *                          initiator from.
 * error_status -		error status.
 */
PUBLIC sec_cred_pa_handle_t sec_login_cred_get_initiator(
    sec_login_handle_t          login_context,
    error_status_t              *stp
)
{
    sec_login_context_t *lcp;
    sec_login_handle_t  tlh = NULL;
    sec_bytes_t         cred_chain;
    sec__cred_auth_info_t auth_info, *authP;
    error_status_t      tst;

    authP = &auth_info;
    /* Make sure the caller passed in a valid context */
    lcp = sec_login_pvt_check_handle(login_context, valid, stp);
    if (BAD_STATUS(stp)) {
        return(NULL);
    }

    /* If we haven't generated the creds block that sec_cred_ expects, do it */
    if (! lcp->cred_h_valid) {
        sec_login_pvt_get_ecreds(lcp, &cred_chain, stp);
	if (GOOD_STATUS(stp)) {
            authP = &auth_info;
            authP->handle_type = sec__cred_handle_type_client;
            authP->authenticated = false;
            authP->authz_svc = rpc_c_authz_dce;
            authP->authn_svc = rpc_c_authn_dce_secret;
            *stp = sec__cred_create_authz_handle(authP,
                &cred_chain, NULL, NULL, &lcp->cred_h);
	  }
        lcp->cred_h_valid = GOOD_STATUS(stp);
    }


    /* Clean up temporary lc if necessary */
    if (tlh != NULL) {
        sec_login_release_context(&tlh, &tst);
    }

    /* Pass back the type of handle that the caller really wants to see */
    if (GOOD_STATUS(stp)) {
        return(sec_cred_get_initiator(lcp->cred_h, stp));
    } else {
        return NULL;
    }
}


/* sec_login_cred_get_delegate
 *
 * This function is used to iterate through and extract the privilege
 * attributes of the delegates listed in a specified login context.
 * Semantics of arguments
 * login_context -          specifies the login context to extract the
 *                          delegates from.
 * cursor -                 an input/output cursor used to iterate through
 *                          the set of delegates in the login context.
 * error_status -           error status.
 */
PUBLIC sec_cred_pa_handle_t sec_login_cred_get_delegate (
    sec_login_handle_t          login_context,
    sec_cred_cursor_t           *cursor,
    error_status_t              *stp
)
{
    sec_login_context_t *lcp;
    sec_login_handle_t  tlh = NULL;
    sec_bytes_t         cred_chain;
    error_status_t      tst;
    sec__cred_auth_info_t auth_info, *authP;


    /* Make sure the caller passed in a valid context */
    lcp = sec_login_pvt_check_handle(login_context, valid, stp);
    if (BAD_STATUS(stp)) {
        return(NULL);
    }

    /* If we haven't generated the creds block that sec_cred_ expects, do it */
    if (! lcp->cred_h_valid) {
        sec_login_pvt_get_ecreds(lcp, &cred_chain, stp);
	if (GOOD_STATUS(stp)){
            authP = &auth_info;
            authP->handle_type = sec__cred_handle_type_client;
            authP->authenticated = false;
            authP->authz_svc = rpc_c_authz_dce;
            authP->authn_svc = rpc_c_authn_dce_secret;
        
        *stp = sec__cred_create_authz_handle(authP,
            &cred_chain, NULL, NULL, &lcp->cred_h);
      }
        lcp->cred_h_valid = GOOD_STATUS(stp);
    }

    /* Clean up temporary lc if necessary */
    if (tlh != NULL) {
        sec_login_release_context(&tlh, &tst);
    }

 
    /* Pass back the type of handle that the caller really wants to see */
    if (GOOD_STATUS(stp)) {
        return(sec_cred_get_delegate(lcp->cred_h, cursor, stp));
    } else {
        return NULL;
    }
}


/* sec_login_cred_init_cursor
 *
 * This function is used to initialize a sec_cred_cursor_t for use in calls
 * to the iterative routine sec_login_cred_get_delegate.
 * Semantics of arguments
 * cursor -                 an input/output cursor used to iterate through
 *                          the list of delegates.
 * error_status -           error status.
 */
PUBLIC void sec_login_cred_init_cursor (
    sec_cred_cursor_t           *cursor,
    error_status_t              *stp
)
{
    sec_cred_initialize_cursor(cursor, stp);
}


PUBLIC void sec_login_cred_get_expiration (
    sec_login_handle_t  login_context,          /* [in]  */
    unsigned32          *cred_expiration,       /* [out] */
    error_status_t      *st                     /* [out] */
)
{
    sec_login_context_t *lcp;

    CLEAR_STATUS(st);
    *cred_expiration = 0;

    if (DEFAULT_HANDLE(login_context)) {
        lcp = sec_login_pvt_get_default_context(st);
    } else {
        lcp = sec_login_pvt_check_handle(login_context, valid, st);
    }

    if (BAD_STATUS(st)) {
        return;
    }

    sec_login_pvt_inq_identity_expiration(lcp,
                                          (unsigned32 *) cred_expiration,
                                           st);

    /*
     * If delegation is in use the lifetime of the credentials
     * is also bounded the expiration time of the delegation
     * token
     */
    if ((DLG_REQ_TYPE(lcp) == sec__login_c_dlg_req_traced
    || DLG_REQ_TYPE(lcp) == sec__login_c_dlg_req_imp)
    && DLG_REQ_INFO(lcp).dlg_token_set.num_tokens > 0) {
	int i;
	sec_dlg_token_set_t token_set = DLG_REQ_INFO(lcp).dlg_token_set;  
	unsigned32  token_exp = token_set.tokens[0].expiration;

	/* 
         * Sometime in the future, there may be more than one delegation
         * token but, unless the authentication service in use is anything
         * other than kerberos, it is unlikely that the expiration
         * times will differ.  If we ever do support alternative
         * authentication services, this code will probably have to
         * be rewritten anyway, so we just assume that any additional
         * tokens are for a newer version of the kerberos-based
         * DCE delgation token.  Since delegation tokens are always
         * generated by our local priv server, we assume that all
         * delegation tokens in the set are undertood by our local
         * priv server and thus the the longest-lived delegation
         * token determines the upper bound of our credential
         * lifetime.
         */
	for (i = 1; i < token_set.num_tokens; i++) {
	    if (token_set.tokens[i].expiration > token_exp) {
		token_exp = token_set.tokens[i].expiration;
	    }
	}

	if (token_exp > 0 && token_exp < *cred_expiration) {
	    *cred_expiration = token_exp;
	}
    }
    
    if (GOOD_STATUS(st) && lcp->identity->state != info_certified) {
        SET_STATUS(st, sec_login_s_not_certified);
    }
}

/* sec_login_tkt_request_options
 *
 * This function is used by a client to request specific AS ticket
 * options. This optional function should be called after
 * sec_login_setup_identity or sec_login_refresh_identity and
 * before sec_login_validate_identity or sec_login_valid_and_cert_ident.
 *
 * Input should consist of a login context handle in the
 * setup or refreshed state, and a structure which specifies
 * the types of ticket options requested. If the user requests
 * a renewable/postdated ticket, or a non-default ticket lifetime,
 * additional data must be provided in the renewable_lifetime, 
 * postdated_dormanttime, and lifetime fields of the 
 * sec_login_tkt_info_t structure, respectively.
 *
 */
PUBLIC void sec_login_tkt_request_options (
    sec_login_handle_t    login_context,
    sec_login_tkt_info_t  *tkt_info,
    error_status_t        *stp
)
{
    sec_login_context_t *lcp;
    int                 options = 0;

    CLEAR_STATUS(stp);

    /*
     * make sure the caller passed in a login context
     * in at least the setup state.
     */
    lcp = sec_login_pvt_check_handle(login_context, setup, stp);
    if (BAD_STATUS(stp)) {
        return;
    }

    /*
     * if the context has already been through a validation routine,
     * and has not been refreshed since, then fail
     */
    if (lcp->state >= valid && !lcp->refreshed) {
        SET_STATUS(stp, sec_login_s_already_valid);
        return;
    }

    lcp->tkt_request.options = tkt_info->options;
    lcp->tkt_request.postdated_dormanttime = tkt_info->postdated_dormanttime;
    lcp->tkt_request.lifetime = tkt_info->lifetime;
    lcp->tkt_request.renewable_lifetime = tkt_info->renewable_lifetime;
}

PUBLIC void sec_login_export_from_pag
(
    unsigned32          pag,            /* [in] */
    unsigned32          buf_len,        /* [in]  */
    ndr_byte            buf[],          /* [out] */
    unsigned32          *len_used,      /* [out] */
    unsigned32          *len_needed,    /* [out] */
    error_status_t      *st             /* [out] */
)
{
    sprintf((char *)buf, SEC_LOGIN_CCACHE_FROM_PAG_FMT, pag);
    if (len_used) *len_used = strlen((char *)buf);
    if (len_needed) *len_needed = strlen((char *)buf);
    *st = error_status_ok;
}

