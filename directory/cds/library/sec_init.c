/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: sec_init.c,v $
 * Revision 1.1.11.2  1996/02/18  19:35:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:12  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:27:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:31  root]
 * 
 * Revision 1.1.7.6  1994/08/03  19:03:14  mccann
 * 	more includes
 * 	[1994/07/21  16:39:28  mccann]
 * 
 * 	includes file cleanup
 * 	[1994/07/19  18:10:26  mccann]
 * 
 * Revision 1.1.7.5  1994/06/30  19:17:54  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:04:35  mccann]
 * 
 * Revision 1.1.7.4  1994/06/23  19:07:32  jd
 * 	Remove evtblk extern. It no longer exists.
 * 	[1994/06/23  15:40:37  jd]
 * 
 * Revision 1.1.7.3  1994/06/09  18:42:29  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:43  devsrc]
 * 
 * Revision 1.1.7.2  1994/04/29  15:55:35  peckham
 * 	Use dcecdsmac.h macros for dce_svc_printf().
 * 	Eliminate use of gettimeofday().
 * 	[1994/04/29  14:35:05  peckham]
 * 
 * Revision 1.1.7.1  1994/03/12  22:08:48  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:34:58  peckham]
 * 
 * Revision 1.1.5.3  1992/12/30  14:17:15  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:17:08  zeliff]
 * 
 * Revision 1.1.5.2  1992/09/29  18:53:39  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:16:19  weisman]
 * 
 * Revision 1.1.2.2  1992/03/22  23:06:25  weisman
 * 		New file from Digital (post 1.0)
 * 	[1992/03/22  23:02:15  weisman]
 * 
 * $EndLog$
 */
/*
 * Module sec_init.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 * 
 * Function: contains server security functions for DCE.
 */

#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dce/dce_error.h>
#include <dce/assert.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <stdio.h>
#include <sys/time.h>
#include <threads_lib.h>
#include <dce/rpc.h>
#include <dce/sec_login.h>
#include <dce/rgybase.h>
#include <dce/secidmap.h>
#include <dce/binding.h>
#include <dce/keymgmt.h>
#include <dce/dce_cf.h>
#include <dce/utc.h>

#define EMPTY_STRING ((unsigned char *)"\0")
typedef struct {
    sec_login_handle_t login_context;
    char passwd[100];
    char *name;
    int authn_svc;
} sec_info_t;

static void
get_key_and_validate_id (
   sec_login_handle_t ,
   char               *,
   char               *,
   error_status_t     *);

static dthread_address_t
refresh_server_identity (
   sec_info_t         *);

static dthread_address_t
manage_server_key (
   sec_info_t         *);


/* thread attributes for server refresh and key mgmt threads */
static dthread_attr_t refresh_attr = {"refresh_server_identity", 0};
static dthread_attr_t manage_attr = {"manage_server_key", 32000};

/*
 * server_sec_init: initialize a server to be a secure DCE server.
 * Does a secure login,
 * registers authentication info with RPC, and starts up threads to
 * refresh server's identity and manage the server's key, periodically.
 * NOTE: if the retry argument is true,
 * this routine should be called from a dedicated thread because
 * if any of the secure calls fail, it will sleep for 1 minute and
 * retry.
 * This function is common to both  'cdsd' and 'gdad'.
 * Inputs: server's principal name, password, retry flag, 
 * Outputs: server's cell name/uuid, principal name/uuid
 * Returns: TRUE or FALSE
*/
#define IN
#define OUT
int 
server_sec_init (unsigned char  *server_princ_name,
                 char           *passwd,
                 int            retry,
                 unsigned char  *cell_name,
                 uuid_p_t       cell_uuid_p,
                 unsigned char  *server_simp_name,
                 uuid_p_t       server_uuid_p)
{
  static sec_info_t sec_info;
  static sec_login_handle_t login_context;
  error_status_t st;
  int temp_status;
  long authn_svc = rpc_c_authn_dce_private;
  sec_rgy_handle_t  context;
  dthread_t sec_th;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug6,
      " >server_sec_init")); 

  while (1) 
  {
      /*
       * Identify ourselves 	
       */

      sec_login_setup_identity (server_princ_name,
				sec_login_no_flags,
				&login_context,
				&st);

      if (st != error_status_ok) 
      {
	  DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_library,
              svc_c_debug6,
              "server_sec_init: sec_login_setup_identity failed: %ld", 
	      st));

          goto waitabit;
      }
  
      /*
       * Validate our identity based on either a clear text password from the
       * config file, or the key(s) in the local key store.
       */
      get_key_and_validate_id (login_context,
			       passwd,
			       (char *)server_princ_name,
			       &st);
      
      if (st != error_status_ok) 
      {
	  DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_library,
              svc_c_debug6,
              "server_sec_init: get_key_and_validate_id failed: %ld",
	      st));

	  goto waitabit;
      }
    
      /*
       * Set our secure context 
       */
    
      sec_login_set_context (login_context, &st);
      if (st != error_status_ok) 
      {
	  DCE_SVC_DEBUG((
             cds__svc_handle,
             cds_svc_library,
             svc_c_debug6,
             "server_sec_init: sec_login_set_context failure : %d", 
	     st));

	  goto waitabit;
      }
      
      /*
       * Get server info and save in global nameserver structure  
       */
      sec_rgy_site_open (EMPTY_STRING, &context, &st);
      if (st != error_status_ok) 
      {
	  DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_library,
              svc_c_debug6,
              "server_sec_init: sec_rgy_site_open failed: %ld", 
	      st));
	  
	  goto waitabit;
      }
    
      sec_id_parse_name(context, server_princ_name,
			cell_name, cell_uuid_p,
			server_simp_name, server_uuid_p,
			&st);
    
      if (st != error_status_ok) 
      {
	  DCE_SVC_DEBUG((
              cds__svc_handle,
              cds_svc_library,
              svc_c_debug6,
              "server_sec_init: sec_rgy_parse_name failed: %ld", 
              st));
	  
	  goto waitabit;
      }
      sec_rgy_site_close(context, &st);

      /*
       * Fire off a thread to refresh our identity (as a client) periodically.
       */
      ZERO_bytes(&sec_info, sizeof(sec_info));
      sec_info.login_context = login_context;
      strcpy (sec_info.passwd, passwd);
      sec_info.name = (char *)server_princ_name;
      if (dthread_create (&sec_th, &refresh_attr,
			  (dthread_start_routine)refresh_server_identity,
			  (dthread_address_t)&sec_info) != 0) 
      {
	  dce_svc_printf(CDS_S_PTHREADCREATE_MSG);
	  return (0);  /* error return */
      }

      /*
       * Register authentication info with RPC 
       */
      rpc_server_register_auth_info (server_princ_name,
				     authn_svc,
				     NULL,
				     NULL,
				     &st);
    
      if (st != error_status_ok) 
      {
	  dce_svc_printf(CDS_S_RPCREGISTERAUTHINFO_MSG, (long)st);

	  goto waitabit;
      }

      /*	
       * Fire off a thread to manage our (server) key.
       */
      sec_info.authn_svc = (int)authn_svc;
      if (dthread_create (&sec_th, &manage_attr,
			  (dthread_start_routine)manage_server_key,
			  (dthread_address_t)&sec_info) != 0) 
      {
	  dce_svc_printf(CDS_S_PTHREADCREATE_MSG);
	  return (0);  /* error return */
      }
      
      break;   /* security initialization done */

waitabit:

      if (retry)
      {
	  dthread_delay ((dthread_interval_t)60);  /* pause 60 seconds */
      }
      else 
      {

	  DCE_SVC_DEBUG((
	      cds__svc_handle,
	      cds_svc_library,
	      svc_c_debug6,
	      " <server_sec_init: retry = 0 -> return(0)")); 

	  return (0);
      }
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug6,
      " <server_sec_init: @end -> return(1)")); 

  return (1);
}
/****************************************************************************/

/* refresh_server_identity: periodically refreshes the server's (client)
 * credentials.  Runs as a thread which never returns (except on error)
 * This code is essentially stolen from 'sec_clientd.c'
*/
#define GOOD_STATUS(stp)          ((stp)==NULL || (*stp) == error_status_ok)
#define BAD_STATUS(stp)           ((*stp) != error_status_ok)
#define CLEAR_STATUS(stp)         (*stp) = error_status_ok
#define STATUS_EQUAL(stp, st_val) ((*stp) == (st_val))
#define TEN_MINUTES         600
#define ONE_DAY             24*60*60

static dthread_address_t 
refresh_server_identity (sec_info_t *sec_info_p)
{
  error_status_t          st;
  dthread_interval_t      interval = 3600;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug2,
      " >refresh_server_identity"));

  CLEAR_STATUS(&st);

  while(GOOD_STATUS(&st)) 
  {
      utc_t now_utc;
      signed32 expiration;

      /*
       * What time is it? 
       */
      st = utc_gettime(&now_utc);
      dce_assert(cds__svc_handle, (0 <= st));
      /*
       * sleep until the server's context needs renewing 
       */
      sec_login_get_expiration(sec_info_p->login_context, &expiration, &st);
      if (GOOD_STATUS(&st) || (st == sec_login_s_not_certified)) 
      {
	  timespec_t now_ts, inacc;
	  long tdf;
	  signed32 valid_time;
	  boolean32 do_change = TRUE;

	  st = utc_bintime(&now_ts, &inacc, &tdf, &now_utc);
	  dce_assert(cds__svc_handle, (0 <= st));

	  /* Pretend it expires before it really does, so we'll have time
	   * to recover from network problems or clock skew.  Subtract the
	   * smaller of (valid_time/2) and TEN_MINUTES, from the valid_time
	   * and sleep for that number of seconds.
	   */
	  valid_time = expiration - now_ts.tv_sec;
	  interval = (dthread_interval_t)(valid_time -
                      MIN( TEN_MINUTES, (signed32)(valid_time/2) ));

	  if (interval > 0) 
	  {
	      /*
	       * Alpha pthread_delay_np doesn't work for BIG values 	
	       */
	      if (interval > ONE_DAY) 
	      {
		  interval = ONE_DAY;
		  do_change = FALSE;
	      }
	      dthread_delay (interval);
	  }
	  if (do_change) 
	  {
	      /*
	       * Refresh the context 
	       */
	      sec_login_refresh_identity(sec_info_p->login_context, &st);

	      /*
	       * revalidate our identity 
	       */
	      if (GOOD_STATUS(&st)) 
	      {	    
		  get_key_and_validate_id (sec_info_p->login_context,
					   sec_info_p->passwd,
					   sec_info_p->name,
					   &st);
	      }

	      /*
	       * On failure, give it a chance to try again 
	       */
	      if (st != error_status_ok) 
	      {

		dce_svc_printf(CDS_S_LIBRARY_REFRESH_MSG, (long)st);
		interval = TEN_MINUTES;
		dthread_delay (interval);
	      }
	  }
	  CLEAR_STATUS(&st);
      }
      else
      {
	  dce_svc_printf(CDS_S_LIBRARY_REFRESH_EB_MSG, (long)st);
      }
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug2,
      " <refresh_server_identity"));

  return((dthread_address_t)0);
}


/*****************************************************************************/

/* get_key_and_validate_id:
 * For each key in the local key file with our name on it, try to validate
 * our identity with it.  Return when the validation is successful or
 * there are no more keys to try.
*/

static void 
get_key_and_validate_id (sec_login_handle_t  login_context,
                         char                *passwd,
                         char                *name,
                         error_status_t      *st_p)
{
  unsigned32       vno;
  sec_passwd_rec_t    key;
  boolean32 reset_passwd = 0;
  sec_login_auth_src_t auth_src = 0;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug7,
      " >get_key_and_validate_id"));
  
  if (*passwd) 
  {
      /*
       * clear text password supplied 
       */
      key.version_number = sec_passwd_c_version_none;
      key.pepper = NULL;
      key.key.key_type = sec_passwd_plain;
      key.key.tagged_union.plain = (unsigned_char_p_t)passwd;

      sec_login_validate_identity (login_context,
				   &key,
				   &reset_passwd,
				   &auth_src,
				   st_p);
      if (*st_p != error_status_ok) 
      {
	  DCE_SVC_DEBUG((
	      cds__svc_handle,
              cds_svc_library,
              svc_c_debug7,
	      " <get_key_and_validate_id: return due to sec_login_validate_identity failed. status = %ld",
	      (long)*st_p));
	  
	  return;
      }
  }
  else 
  {   
      /*
       * get key from local store 
       */
      sec_login_valid_from_keytable(login_context, rpc_c_authn_dce_private,
				    NULL, 0, &vno, &reset_passwd, &auth_src,
				    st_p);
  }
 
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug7,
      " <get_key_and_validate_id"));
  
}
/****************************************************************************/
/*
 * manage_server_key : Manage our key. This thread runs indefinitely
 * and changes the server's key perodically, based on the cell's policy.
*/

static dthread_address_t 
manage_server_key (sec_info_t *sec_info_p)
{
  error_status_t st;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug2,
      " >manage_server_key"));

  while (1) 
  {
      sec_key_mgmt_manage_key ((
			      sec_key_mgmt_authn_service)sec_info_p->authn_svc,
			       NULL,
			       (unsigned char *)sec_info_p->name,
			       &st);
      if (BAD_STATUS(&st)) 
      {
	  DCE_SVC_DEBUG((
	      cds__svc_handle,
              cds_svc_library,
              svc_c_debug7,
              "manage_server_key: sec_key_mgmt_manage_key failed: %ld",
              st));
      }
      dthread_delay ((dthread_interval_t)TEN_MINUTES);
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug2,
      " <manage_server_key"));
}


