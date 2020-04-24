/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: pkc_plcy_api.cxx,v $
 * Revision 1.1.2.1  1996/11/13  20:32:48  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  20:24:59  arvind]
 *
 * Revision /main/DCE_1.2.2/2  1996/11/04  22:26 UTC  arvind
 * 	Cert-API drop 2 from DEC
 * 	[1996/11/03  23:23 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
 * 
 * Revision /main/DCE_1.2.2/1  1996/10/18  00:38 UTC  arvind
 * 	CAPI drop from DEC
 * 	[1996/10/16  20:32 UTC  arvind  /main/arvind_capi/1]
 * 
 * $EndLog$
 */
/*
 * @DEC_COPYRIGHT@
 * Copyright (c) 1996
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 * 
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 * 
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 * 
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 * 
 * FACILITY:    Certification API	
 * 
 * ABSTRACT:    User-level API routines that call into policy
 *              specific routines.  
 * 
 *	
 * ENVIRONMENT:	portable
 * 
 * AUTHOR:
 *              Mahesh B. Girkar , John Wray
 *		Distributed Processing Engineering (DPE)
 * 
 * DATE:	March 1996
 * 
 */



#include <stdlib.h>
#include <string.h>
#include <pthread.h>
#include <dce/dce.h>
#include <dce/x500name.h>
#include <dce/pkc_plcy_reg.h>
#include <pkc_plcy_int.h>
#include <dce/pkc_api.h>



uuid_t sec_pk_domain_unspecified = { /* D9FD7D3E-2F5C-11D0-93CD-08002B187D1A */
        0xd9fd7d3e,
        0x2f5c,
        0x11d0,
        0x93,
        0xcd,
        {0x08, 0x00, 0x2b, 0x18, 0x7d, 0x1a}
      };


/*
 * append_to_trustlist:
 * given a trust item, returns a new element of trust in pkc_trus_list_t 
 */


static unsigned32 append_to_trustlist
  (trustitem_t *tr_item, pkc_trust_list_t  & pktr);

static unsigned32 get_initial_policy
             (selection_t *sel, 
              char *polrequired);



static unsigned32 retrieve_keys_given_policy_oid
        (gss_OID policy_oid,
         const void *trustbase,
         selection_t *sel,
         utc_t *udate,
         const x500name & xname,
	 void ** keyinfo_handle);

static unsigned32 nm_oid_copy(gss_OID src,
		       gss_OID * dst);
/* 
 * get_initial_policy is currently a noop. In future versions,
 * it will extract the polrequired field, and pass it along
 * to our internal routines.
 */

static unsigned32 get_initial_policy
             (selection_t *sel, 
              char *polrequired) 
{
  return 0;
}


static unsigned32 retrieve_keys_given_policy_oid
        (gss_OID policy_oid,
         const void *trustbase,
         selection_t *sel,
         utc_t *udate,
         const x500name & xname, 
	 const uuid_t & domain,
	 const pkc_generic_key_usage_t & key_usage,
         void ** keyinfo_handle)
{ 
  unsigned32 status;
  void *context;
  char polrequired;
  pkc_trusted_key_t *keys;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">retrieve_keys_given_policy_oid "));

  /* currently, the function below is a no-op */
  status = get_initial_policy
             (sel, &polrequired);
  if (status != 0)
    return status;


 status = pkc_plcy_retrieve_keyinfo(
                   policy_oid,
                   trustbase,
                   xname,
                   udate,
                   domain,
                   key_usage,
		   polrequired,
		   keyinfo_handle);

           

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           "<retrieve_keys_given_policy_oid "));

 return status;  
}

/* copy gss_OID.. copied from gssapi.c */

static unsigned32 nm_oid_copy(gss_OID src,
		       gss_OID * dst) 
{
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">nm_oid_copy "));

    if (dst == NULL) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
           "nm_oid_copy: Bad Parameters "));
      return pkc_s_asn_bad_param;
    }
    if ((*dst = (gss_OID_desc *)pkc_malloc(sizeof(gss_OID_desc))) == NULL) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
           "nm_oid_copy: Ran out of memory "));
	return pkc_s_nomem;
    };
    memset(*dst, 0, sizeof(gss_OID_desc));
    (*dst)->length = src->length;
    if (((*dst)->elements = (void *)pkc_malloc(src->length)) == NULL) {
	pkc_free(*dst);
	*dst = NULL;

        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
           "nm_oid_copy: Ran out of memory "));
	return pkc_s_nomem;
    };
    memcpy((*dst)->elements, src->elements, src->length);

   DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           "<nm_oid_copy "));

    return 0;
}


/*
 *********************
 * Start of PUBLIC API
 *********************
 *
 */




  
/*
 * Function pkc_init_trustlist
 *
 * Creates an empty trustlist
 * If tr_list is empty, return pkc_s_asn_bad_param;
 * If we cannot pkc_malloc, returns pkc_s_nomem;
 * Otherwise, returns 0
 */

unsigned32 pkc_init_trustlist
 (trustlist_t **tr_list)
{
  pkc_plcy_init();

   DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">pkc_init_trustlist "));


  if (!tr_list) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_init_trustlist: Bad parameters"));
    return pkc_s_asn_bad_param;
  }

  *tr_list = (trustlist_t *) pkc_malloc(sizeof(trustlist_t));
  if (!(*tr_list)) { 
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_init_trustlist: Out of mem"));
    return pkc_s_nomem;
  }
  (*tr_list)->handle = (void *) new pkc_trust_list_t;
  if (!((*tr_list)->handle)) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_init_trustlist: Out of mem"));
    return pkc_s_nomem;
  }

 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           "<pkc_init_trustlist "));
  return 0;
}


/*
 * 
 * pkc_init_trustbase:
 *
 */

unsigned32 pkc_init_trustbase (trustlist_t **tr_list,
                               gss_OID policy_oid,
                               utc_t *time,
                               selection_t *sel,
                               trustbase_t **base)
{
  pkc_policy_t *details;
  void *context;
  pkc_trust_list_t *temp_tr;
  char polrequired;
  unsigned32 status;
  utc_t now;

  pkc_plcy_init();

 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">pkc_init_trustbase "));


  if ((!tr_list) || (!((*tr_list)->handle)) || (!base) || (sel != 0) ) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_init_trustbase: Bad parameters"));
    return pkc_s_asn_bad_param;
  }

  if (!time) {
    status = utc_getusertime(&now);
    if (status != 0) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
         "pkc_init_trustbase: utc_getusertime failed %ld", status));
      return status;
    }
  }

  *base = (trustbase_t *) pkc_malloc (sizeof (trustbase_t));
  if (!(*base)) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_init_trustbase: Out of mem"));
    return pkc_s_nomem;
  }

  status = nm_oid_copy(policy_oid, &((*base)->policy_oid));
  if (status != 0) {
    pkc_free (*base);
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_init_trustbase: nm_oid_copy: Failed %ld", status));
    return status;
  }

   
  if (time)
    (*base)->time = *time;
  else
    (*base)->time = now;
  (*base)->sel = sel;
  (*base)->trustbase_handle = 0; 


  (*base)->trustbase_handle = 0;


  temp_tr = (pkc_trust_list_t *) (*tr_list)->handle;



  status =  get_initial_policy(sel, &polrequired);
  if (status != 0) {
    pkc_free ((*base)->policy_oid);
    pkc_free (*base);
    return status;
  }


  status = pkc_plcy_establish_trustbase 
             ((*base)->policy_oid, 
              *temp_tr,
              (const utc_t *) &((*base)->time),
             polrequired,
              (void **) &((*base)->trustbase_handle)
             );

  if (status != 0) {

    pkc_free ((*base)->policy_oid);
    pkc_free (*base);
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_init_trustbase: pkc_plcy_establish_trustbase: Failed %ld", status));
    return status;
  }
              
 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           "<pkc_init_trustbase "));
  return 0;
}            
               
   

/*
 * pkc_retrieve_keylist
 *
 * Given a initial trust list, name (x500 char * name), time which specifies
 * when key is invalid(?) retreieves all public keys for the for
 * principal name.
 * 
 * currently, sel parameter is ignored. kept for future versions.
 * also, we only return one key, not all. The signature (**out_keys, *no_of_key)
 * is also kept for future versions.
 *
 *
 * we return as soon as any of the preloaded policies return success.
 * 
 * policy_tried is set to OID of policy that was used to succesfully
 * return key.
 * 
 */


unsigned32 pkc_retrieve_keyinfo(trustbase_t *base,
                                char *name, 
                                utc_t *key_date,
                                uuid_t * key_domain,
                                unsigned long * key_usages,
                                selection_t *in_sel,
                                keyinfo_t ** keyinfobase) 
{
  x500name xname;
  unsigned32 status;
  int i = 0;
  utc_t time;
  selection_t *sel = 0;
  uuid_t loc_key_domain = sec_pk_domain_unspecified;
  pkc_generic_key_usage_t loc_key_usages;

  
  pkc_plcy_init();

 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">pkc_retrieve_keyinfo "));


    if (key_domain == NULL) key_domain = &loc_key_domain;
    if (key_usages != NULL) {
	loc_key_usages = *key_usages;
    };

  if (!base || !name) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_retrieve_keyinfo: bad parameters"));
    return pkc_s_asn_bad_param;
  }
  if (!keyinfobase) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_retrieve_keyinfo: Bad parameters"));
    return pkc_s_asn_bad_param;
  }

  if (!key_date)
    time = base->time; 
   else
    time = *key_date;

  if (!in_sel)
    sel = base->sel;
  else
    sel = in_sel;


  if ((*keyinfobase = (keyinfo_t *)pkc_malloc(sizeof(keyinfo_t))) == NULL) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_retrieve_keyinfo: Out of mem"));
    return pkc_s_nomem;
  };

  (*keyinfobase)->base = base;  // Create link from keyinfo_t to trustbase_t
  (*keyinfobase)->handle = NULL;    // Initialize policy-specific part to NULL

  if ((status = xname.set_value(name)) != 0) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_retrieve_keyinfo: Unsupported syntax for name %s", name));
    return pkc_s_x500_unsupported_syntax;
  }
 

  status = retrieve_keys_given_policy_oid
          (base->policy_oid,
           base->trustbase_handle,
           sel, 
           &time,
           xname, 
           *key_domain,
           loc_key_usages,
	   &(*keyinfobase)->handle);
                 

 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           "<pkc_retrieve_keyinfo "));
  return status;
}
      
/*
 * pkc_get_registered_policies: same as 
 * pkc_plcy_get_registered_policies
 */

      
unsigned32 pkc_get_registered_policies(
   gss_OID_set * oid_set) 
{
  pkc_plcy_init();

 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">pkc_get_registered_policies "));


  return (pkc_plcy_get_registered_policies(oid_set));
}
    
     
  
  



/*
 * Function pkc_append_to_trustlist:
 * Appends to pkc_trust_list_t (pointed by (*tr_list)->handle)
 *    1. If tr_list is (nil), an error is returned.
 *    2  If (*tr_list)->handle is nil, a new trust list is created
 *       from the parameters in tr_item & no_of_tr_items.
 *       and a pointer to it is stored in (*tr_list)->handle.
 *    3. If (*tr_list)->handle is non-nil, it points to an existing
 *       pkc_trust_list_t; in this case, we return in
 *       (*tr_list)->handle a new trust list with the old trust list
 *       items, plust the new ones given by tr_item & no_of_tr_items.
 */


unsigned32 pkc_append_to_trustlist 
  (trustlist_t **tr_list, trustitem_t *tr_item, size_t no_of_tr_items)
{
  unsigned32 status;
  pkc_trust_list_t *pktr;
  pkc_trust_list_t *temp;
  pkc_trust_list_element_t *elm, *newelm, *newly_added, *temp1;
  int i;
  utc_t startdate, enddate;
  pkc_key_policies_t keypolicies;
  pkc_constraints_t constraints; 
 
  pkc_plcy_init();

 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">pkc_append_to_trustlist "));


  if (!tr_list) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_append_to_trustlist: Bad parameters"));
    return pkc_s_asn_bad_param;
  }
  if (!((*tr_list)->handle)) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_append_to_trustlist: Bad parameters"));
    return pkc_s_asn_bad_param;
  }

  pktr = (pkc_trust_list_t *) (*tr_list)->handle;
  
  temp = (pkc_trust_list_t *) new pkc_trust_list_t;
  if (!temp) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_append_to_trustlist: Out of mem"));
    return pkc_s_nomem;
  }

  *temp = *pktr;  /* make a copy of the in trust list */

  /* first, we try to see if there are any errors processing any of
   * the trust items; if so, we return immediately, and don't touch
   * the original pkc_trust_list_t in *tr_list->handle
   * The destructor for temp below ensures that we pkc_free up any memory
   * we accumulated in this loop.
   */


  for (i = 0; i < no_of_tr_items; i++) {
    status =  append_to_trustlist(&tr_item[i], *temp);
    if (status != 0) {
      delete temp;
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
         "pkc_append_to_trustlist: append_to_trustlist failed %ld", status));
      return status;
    }
  }

  delete pktr;   /* delete original trust list */
  (*tr_list)->handle = (void *) temp;  /* give back the new one */
  
  return 0;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           "<pkc_append_to_trustlist "));
}   

/*
 * append_to_trustlist:
 * given a trust item, returns a new element of trust in pkc_trus_list_t 
 */

static unsigned32 append_to_trustlist(trustitem_t *tr_item, pkc_trust_list_t  & pktr)
{
  pkc_trust_list_element_t *tel;
  pkc_ca_key_usage_t cakeyusage;
  pkc_key_usage_t keyusage;
  pkc_key_policies_t keypolicies;
  pkc_constraints_t constraints; 
  r_buffer_t rbuf;
  unsigned32 status;
  Certificate cert;
  utc_t startdate;
  utc_t enddate;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">append_to_trustlist "));

  if (!tr_item) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "append_to_trustlist: Bad parameters"));
    return pkc_s_asn_bad_param;
  }
    
  switch (tr_item->type) {
    case IS_KEY:
      if  (tr_item->u.k.version != CURRENT_KEY_VERSION) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
           "append_to_trustlist: Bad parameter key version is not CURRENT_KEY_VERSION"));
        return pkc_s_asn_bad_param;
      }

      {
        pkc_trusted_key_t trusted_key(
                    tr_item->u.k.startDate,
                    tr_item->u.k.endDate,
                    cakeyusage,         /* not being used right now. */
                    keyusage,          /* not being used right now. */
                    keypolicies,       /* not being used right now. */
                    constraints        /* not being used right now. */
                   );
      
        /* copy the key */
        rbuf.data = tr_item->u.k.key;
        rbuf.data_length = tr_item->u.k.size;
        status = trusted_key.value.read(rbuf);
        if (status != 0) {
           DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
              "append_to_trustlist: key read failed %ld", status));
          return status;
        }

        /* copy the subject whose key this is */
        status = trusted_key.owner.set_value((const char *) tr_item->u.k.ca);
        if (status != 0) {
           DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
              "append_to_trustlist: key owner setvalue failed %ld", status));
          return status;
        }


        status = pkc_add_trusted_key(&pktr, trusted_key);
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
           "append_to_trustlist: pkc_add_trusted_key returned %ld", status));
        return status;
      }

      
    case IS_CERT:
      if (tr_item->u.c.version != CURRENT_CERT_VERSION)
        return  pkc_s_asn_bad_param;

      rbuf.data = tr_item->u.c.cert;
      rbuf.data_length = tr_item->u.c.size;

      status =  cert.read(rbuf);
      if (status != 0) {
        return status;
      }
      
      status = cert.certificateInfo.validity.notBefore.get_value(startdate);  
      if (status != 0) {
        return status;
      }
      
      status = cert.certificateInfo.validity.notAfter.get_value(enddate);
      if (status != 0) {
        return status;
      }

      tel = new pkc_trust_list_element_t
               (
                pktr, 
                startdate,
                enddate,
                cakeyusage,         /* not being used right now. */
                keyusage,          /* not being used right now. */
                keypolicies,       /* not being used right now. */
                constraints        /* not being used right now. */
                );

      if (!tel) 
        return pkc_s_nomem;

      tel->value.copy(cert.certificateInfo.subjectPublicKeyInfo);  /* copy the key */
      tel->owner = cert.certificateInfo.subject;                   /* copy the subject whose
                                                                    * key this is
                                                                    */
      return 0;

    default:
      return pkc_s_asn_bad_param;
  } /* end of switch */
  return -1;  /**** SHOULD NEVER COME HERE ****/
}

/*
 * unsigned32 pkc_free_trustlist(trustlist_t **tr_list)
 */

unsigned32 pkc_free_trustlist(trustlist_t **tr_list) 
{
  pkc_trust_list_t *tr;

  pkc_plcy_init();

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">pkc_free_trustlist "));

  if (!tr_list) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
           "pkc_free_trustlist: bad parameters "));
    return pkc_s_asn_bad_param;
  }

  if (!((*tr_list)->handle)) {
    pkc_free (*tr_list);
    *tr_list = 0;
    return 0;
  }
 
  tr = (pkc_trust_list_t *) (*tr_list)->handle;
  delete tr;

  pkc_free (*tr_list);
  *tr_list = 0;
  return 0;
}
 


/*
 * pkc_free_trustbase
 *
 */

unsigned32 pkc_free_trustbase (trustbase_t **base)
{
  trustbase_t *b;
  pkc_trust_list_t *tr;
  pkc_policy_t *details;
  unsigned32 status;
  gss_OID policy_oid;

  pkc_plcy_init();

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">pkc_free_trustbase "));

  if (!base)
    return pkc_s_asn_bad_param;

  b = *base;
  policy_oid = b->policy_oid;
  status = pkc_plcy_delete_trustbase(policy_oid, &b->trustbase_handle);


 
  if (b->policy_oid) 
    pkc_free (b->policy_oid);

  pkc_free (b);
  *base = 0;
  return (status);

} 
  





/*
 * pkc_free_keyinfo
 *
 */

unsigned32 pkc_free_keyinfo (keyinfo_t **keyinfo)
{
  keyinfo_t *k;
  pkc_trust_list_t *tr;
  pkc_policy_t *details;
  unsigned32 status;
  trustbase_t *b;
  gss_OID policy_oid;

  pkc_plcy_init();

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">pkc_free_keyinfo "));

  if (!keyinfo)
    return pkc_s_asn_bad_param;

  k = *keyinfo;

  b = k->base;
  policy_oid = b->policy_oid;
  status = pkc_plcy_delete_keyinfo(policy_oid,
                                   &k->handle);
 
  *keyinfo = 0;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           "<pkc_free_keyinfo: status =%ld ", status));
  return (status);

} 
  

// Interrogation routines.
//
// These are all formed from the same basic skeleton:
// Get the policy OID, then call the policy-specific version of the fucntion
// 

unsigned32 pkc_get_key_count(keyinfo_t * keyinfo,
                             size_t * key_count) {

  unsigned32 status;
  trustbase_t *base;
  gss_OID policy_oid;

  pkc_plcy_init();

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">pkc_get_key_count "));

  if (!keyinfo) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
           "pkc_get_key_count: Bad parameters "));
    return pkc_s_asn_bad_param;
  }


  base = keyinfo->base;
  policy_oid = base->policy_oid;
  status = pkc_plcy_get_key_count(policy_oid,
                                  (keyinfo->handle),
				  key_count);

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           "<pkc_get_key_count: status =%ld ", status));
  return (status);
}			     


unsigned32 pkc_get_key_data(keyinfo_t * keyinfo,
                            unsigned index,
			    unsigned char ** key_buffer,
                            size_t * key_data) {

  unsigned32 status;
  trustbase_t *base;
  gss_OID policy_oid;

  pkc_plcy_init();
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">pkc_get_key_data "));

  if (!keyinfo) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
           "pkc_get_key_data: Bad parameters "));
    return pkc_s_asn_bad_param;
  }


  base = keyinfo->base;
  policy_oid = base->policy_oid;
  status = pkc_plcy_get_key_data(policy_oid,
                                 (keyinfo->handle),
                                 index,
                                 key_buffer,
                                 key_data);

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           "<pkc_get_key_data: status =%ld ", status));
  return (status);
}			     


unsigned32 pkc_get_key_trust_info (keyinfo_t * keyinfo,
                                   unsigned index,
                                   certification_flags_t * flags,
                                   uuid_t * key_domain,
                                   unsigned long * key_usages) {

  unsigned32 status;
  trustbase_t *base;
  gss_OID policy_oid;
  pkc_generic_key_usage_t loc_key_usages;
  uuid_t loc_key_domain;

  pkc_plcy_init();
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">pkc_get_key_trust_info "));

  if (!keyinfo) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
           "pkc_get_key_trust_info: Bad parameters "));
    return pkc_s_asn_bad_param;
  }


  base = keyinfo->base;
  policy_oid = base->policy_oid;
  status = pkc_plcy_get_key_trust(policy_oid,
                                  (keyinfo->handle),
                                  index,
                                  flags,
                                  &loc_key_domain,
                                  &loc_key_usages);

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           "<pkc_get_key_trust_info: status =%ld ", status));

  if (status == 0) {
     if (key_domain) {
        *key_domain = loc_key_domain;
     };
     if (key_usages) {
        *key_usages = loc_key_usages.permitted;
     };
  };

  return (status);
}			     

unsigned32 pkc_get_key_certifier_count (keyinfo_t * keyinfo,
                                        unsigned index,
                                        size_t * certifier_count) {
        
  unsigned32 status;
  trustbase_t *base;
  gss_OID policy_oid;

  pkc_plcy_init();
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">pkc_get_key_certifier_count "));

  if (!keyinfo) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
           "pkc_get_key_certifier_count: Bad parameters "));
    return pkc_s_asn_bad_param;
  }


  base = keyinfo->base;
  policy_oid = base->policy_oid;
  status = pkc_plcy_get_key_certifier_count(policy_oid,
                                            (keyinfo->handle),
                                            index,
                                            certifier_count);

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           "<pkc_get_key_certifier_count: status =%ld ", status));
  return (status);
}			     


unsigned32 pkc_get_key_certifier_info (keyinfo_t * keyinfo,
                                       unsigned key_index,
                                       unsigned certifier_index,
                                       char ** certifier_name,
                                       utc_t * certification_start,
                                       utc_t * certification_expiration,
                                       char * crl_valid,
                                       utc_t * crl_last_seen,
                                       utc_t * next_crl_expected) {
        
  unsigned32 status;
  trustbase_t *base;
  gss_OID policy_oid;

  pkc_plcy_init();
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">pkc_get_key_certifier_info "));

  if (!keyinfo) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
           "pkc_get_key_certifier_info: Bad parameters "));
    return pkc_s_asn_bad_param;
  }


  base = keyinfo->base;
  policy_oid = base->policy_oid;
  status = pkc_plcy_get_key_certifier_info (policy_oid,
                                            (keyinfo->handle),
                                            key_index,
                                            certifier_index,
                                            certifier_name,
                                            certification_start,
                                            certification_expiration,
                                            crl_valid,
                                            crl_last_seen,
                                            next_crl_expected);

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           "<pkc_plcy_get_key_certifier_info: status =%ld ", status));
  return (status);
}			     

