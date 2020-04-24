//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: sec_pkss_svr_util.cxx,v $
// Revision 1.1.2.2  1996/11/13  20:41:43  arvind
// 	PKSS bug fix from DEC
// 	[1996/11/03  23:39 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
//
// Revision 1.1.2.1  1996/10/03  20:32:20  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/03  19:31:59  arvind]
// 
// Revision /main/DCE_1.2.2/1  1996/09/09  21:30 UTC  arvind
// 	PKSS drop from DEC (DCE1.2.2)
// 	[1996/08/30  15:55 UTC  arvind  /main/arvind_pkss/1]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
// Module:  sec_pkss_svr_util.cxx
//
// Purpose:  This module contains utility functions of particular interest
//	     to the PKSS server.
//

#include <pthread.h>
#include <string.h>
#include <netinet/in.h>
#include <dce/nbase.h>
#include "sec_bsafe.h"
#include <dce/sec_pk_base.h>
#include "sec_pkss_dbkey_class.h"
#include "sec_pkss_db.h"
#include "sec_pkss_util.h"
#include "sec_pkss_svr_util.h"
#include "sec_pkss_types_if.h"
#include "sec_pkss_mgmt_if.h"
#include <dce/dcepksmsg.h>
#include <dce/dcepkssvc.h>  
#include <dce/dcepksmac.h> 
#include <dce/dcecdsmsg.h> 

#ifdef MALLOCTRACE
extern "C"
{
#include "malloc_trace.h"
}
#endif

//
// Rundown the context handle on broken connection.
//

void sec_pkss_ctx_hdl_t_rundown (rpc_ss_context_t ctx_handle)
{
#ifdef MALLOCTRACE
  malloc_mstatus ("Entering sec_pkss_ctx_hdl_t_rundown");
#endif

  sec_pkss_context_t *p = (sec_pkss_context_t *)ctx_handle;

  if (p)
  {
    (void) pthread_mutex_lock (&(p->mutex));

    p->deleted = 1;

    (void) pthread_mutex_unlock (&(p->mutex));
    (void) pthread_mutex_destroy (&(p->mutex));

    if (p->clt_nonce_p)
    {
        sec_pkss_zero (p->clt_nonce_p);
        free (p->clt_nonce_p);
        p->clt_nonce_p = 0;
    }
    if (p->svr_nonce_p)
      sec_pkss_zero_and_free (&(p->svr_nonce_p));
    if (p->crypto_key_p)
      sec_pkss_zero_and_free (&(p->crypto_key_p));
    if (p->dbkey_p)
    {
      delete (p->dbkey_p);
      p->dbkey_p = 0;
    }
    p->proto = 0;
    p->sym_alg = 0;
    p->state = 0;
    free (p);
    p = 0;
  }

#ifdef MALLOCTRACE
  malloc_mstatus ("Exiting sec_pkss_ctx_hdl_t_rundown");
#endif

} // End sec_pkss_ctx_hdl_t_rundown


//
// sec_pkss_db_lookup
//
// Search the PKSS database for userKey.  If not found, discontinue
// processing and return an error.  Note that key version number == 0
// means to find the record with the highest version number.
//

error_status_t sec_pkss_db_lookup (PkssDbKeyC *dbKeyP,
				   sec_pkss_dbrec_t *dbrec_p)
{
  error_status_t estatus = 0;
  sec_pkss_dbrec_t prev_ret_rec = {0, 0};
  unsigned32 last_vno = 0;
  PkssDbKeyC *prevKeyP = 0;
  PkssDbKeyC *curKeyP = 0;
  sec_pkss_dbkey_t ret_key = {0, 0};
  sec_pkss_dbrec_t ret_rec = {0, 0};

#ifdef MALLOCTRACE
  malloc_mstatus ("Entering sec_pkss_db_lookup");
#endif

  //
  // If the key version number in the key is not zero, do a specific
  // lookup on that key.  Recall that a key version number == 0 means
  // to find the record with the latest (largest) key version number.
  // Note that in this case, this function will set the key version
  // number of the PKSS database key pointed to by dbKeyP to that value.
  //

  if ((last_vno = dbKeyP->getKeyVno()) != 0)
  {

    estatus = pkss_db_p->read ((char **)&(prev_ret_rec.asn_rec_p),
                               &(prev_ret_rec.asn_rec_len),
			       (char *)dbKeyP->p()->data,
                               dbKeyP->p()->len);

      DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from pkss_db_p->read : 0x%x", estatus));

    if (estatus != 0)
    {
      goto RUNDOWN;
    }

  } // End if (dbKeyP->getKeyVno() != 0)

  else
  {
    //
    // Initialize the PKSS database lookup loop.
    //

    estatus = pkss_db_p->read ((char *)dbKeyP->p()->data,
				dbKeyP->p()->len,
				(char **)&(ret_rec.asn_rec_p),
				&(ret_rec.asn_rec_len),
				(char **)&(ret_key.data),
				&(ret_key.len));

      DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                   "status from pkss_db_p->read : 0x%x", estatus));

    if (estatus != 0)
    {
      goto RUNDOWN;
    }

    else
    {
      //
      // Instantiate a PKSS database key object to make manipulating
      // the returned key's contents easier.  Zero and free the returned
      // key which we no longer need.
      //

      prevKeyP = new PkssDbKeyC (ret_key.data, ret_key.len);
      if (!(prevKeyP))
      {
        dce_svc_printf(PKS_S_NO_MEMORY_MSG);
	estatus = pks_s_no_memory;
        goto RUNDOWN;
      }

      (void) memset (ret_key.data, 0, ret_key.len);
      free (ret_key.data);
      ret_key.data = 0;
      ret_key.len = 0;
      last_vno = prevKeyP->getKeyVno ();

      //
      // Initialize a new PKSS DB record structure saving the previously
      // returned record.  Don't forget to zero and free this record when it
      // is no longer needed.
      //

      prev_ret_rec.asn_rec_p = ret_rec.asn_rec_p;
      prev_ret_rec.asn_rec_len = ret_rec.asn_rec_len;
      ret_rec.asn_rec_p = 0;
      ret_rec.asn_rec_len = 0;

      while (estatus == 0)
      {
        //
        // Read the next record in line.
        //

        estatus = pkss_db_p->read ((char **)&(ret_rec.asn_rec_p),
				    &(ret_rec.asn_rec_len),
				    (char **)&(ret_key.data),
				    &(ret_key.len));

          DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                         "status from pkss_db_p->read : 0x%x", estatus));

        if (estatus == CDS_GDB_NOT_FOUND)
        {
	  estatus = 0;
          break;
        }

        //
        // Instantiate another PKSS database key object to make manipulating
        // the returned key's contents easier.  Zero and free the returned
	// key which we no longer need.
	//

	curKeyP = new PkssDbKeyC (ret_key.data, ret_key.len);
        if (!(curKeyP))
        {
          dce_svc_printf(PKS_S_NO_MEMORY_MSG);
	  estatus = pks_s_no_memory;
          goto RUNDOWN;
        }

        (void) memset (ret_key.data, 0, ret_key.len);
        free (ret_key.data);
        ret_key.data = 0;
        ret_key.len = 0;

	//
	// Loop through the records and keep the record whose returned key
	// contains the largest key version number.  Stop looking after
	// the username string portion of the key changes to a new user.
	//

	if ((dbKeyP->getNameLen() != curKeyP->getNameLen()) ||
	    (memcmp (dbKeyP->p()->data,
		    curKeyP->p()->data,
		    curKeyP->p()->len - sizeof (sec_pk_usage_flags_t))) != 0)
        {
	  break;
        }
	
	else
	{
	  last_vno = MAX (last_vno, curKeyP->getKeyVno ());
	  (void) memset (prev_ret_rec.asn_rec_p, 0, prev_ret_rec.asn_rec_len);
	  free (prev_ret_rec.asn_rec_p);
          prev_ret_rec.asn_rec_p = ret_rec.asn_rec_p;
          prev_ret_rec.asn_rec_len = ret_rec.asn_rec_len;
          ret_rec.asn_rec_p = 0;
          ret_rec.asn_rec_len = 0;
	  delete prevKeyP;
          prevKeyP = new PkssDbKeyC (curKeyP);

          if (!(prevKeyP))
          {
            dce_svc_printf(PKS_S_NO_MEMORY_MSG);
	    estatus = pks_s_no_memory;
            goto RUNDOWN;
          }

	  delete curKeyP;
	  curKeyP = 0;
	}

      } // End while (estatus == 0)

    } // End else path of if (estatus != 0) after first "lookup after" read

  } // End else path of if (dbKeyP->getKeyVno() != 0)

  dbKeyP->setKeyVno (last_vno);
  dbrec_p->asn_rec_p = prev_ret_rec.asn_rec_p;
  dbrec_p->asn_rec_len = prev_ret_rec.asn_rec_len;
  prev_ret_rec.asn_rec_p = 0;
  prev_ret_rec.asn_rec_len = 0;

RUNDOWN:

  //
  // Zero and delete storage no longer required.
  //

  if (prevKeyP)
  {
    delete prevKeyP;
    prevKeyP = 0;
  }

  if (curKeyP)
  {
    delete curKeyP;
    curKeyP = 0;
  }

  if (ret_key.data)
  {
    (void) memset (ret_key.data, 0, ret_key.len);
    free (ret_key.data);
    ret_key.data = 0;
    ret_key.len = 0;
  }

  if (ret_rec.asn_rec_p)
  {
    (void) memset (ret_rec.asn_rec_p, 0, ret_rec.asn_rec_len);
    free (ret_rec.asn_rec_p);
    ret_rec.asn_rec_p = 0;
    ret_rec.asn_rec_len = 0;
  }

  if (prev_ret_rec.asn_rec_p)
  {
    (void) memset (prev_ret_rec.asn_rec_p, 0, prev_ret_rec.asn_rec_len);
    free (prev_ret_rec.asn_rec_p);
    prev_ret_rec.asn_rec_p = 0;
    prev_ret_rec.asn_rec_len = 0;
  }

  //
  // Exit this function.
  //

#ifdef MALLOCTRACE
  malloc_mstatus ("Exiting sec_pkss_db_lookup");
#endif

  return (estatus);

} // End sec_pkss_db_lookup


//
// Expand multi-bit usage flags into multiple records with single-bit
// usage flags.
//

error_status_t sec_pkss_do_usage_flags
(
  PkssDbKeyC *dbKeyP,
  sec_pkss_dbrec_t *dbrec_p
)

{
  //
  // Local variables
  //

  error_status_t estatus = 0;
  sec_pk_usage_flags_t flag_mask = 1;
  sec_pk_usage_flags_t usages = dbKeyP->getUsages();

  //
  // Regard the mapping of multi-bit usage flag keys to individual
  // records with single-bit usage flags as atomic.
  //

  pkss_db_p->txnBegin();

  //
  // For every bit set in the usage flags, create an individual record
  // with only that bit set in the usage flags.
  //

  while (flag_mask)
  {
    if (usages & flag_mask)
    {
      PkssDbKeyC newKey (dbKeyP);
      newKey.setUsages (flag_mask);
      estatus = pkss_db_p->insert ((char *)dbrec_p->asn_rec_p,
                                   dbrec_p->asn_rec_len,
                                   (char *)newKey.p()->data,
                                   newKey.p()->len);

        DCE_SVC_DEBUG((pks_svc_handle, pks_s_general, svc_c_debug2,
                       "status from pkss_db_p->write : 0x%x", estatus));

      if (estatus != 0)
      {
        break;
      }

    } // End if (dbKeyP->keyUsages & flag_mask)

    flag_mask <<= 1;

  } // End while (flag_mask)

  //
  // If any one write failed, rollback this entire transaction; otherwise,
  // commit this transaction.
  //

  if (estatus)
    pkss_db_p->txnCancel();
  else
    pkss_db_p->txnEnd();

  //
  // Exit this function.
  //

  return (estatus);

} // End sec_pkss_do_usage_flags

