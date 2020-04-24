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
 * $Log: pkc_plcy_registry.cxx,v $
 * Revision 1.1.2.1  1996/11/13  20:33:42  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  20:28:10  arvind]
 *
 * Revision /main/DCE_1.2.2/2  1996/11/04  22:26 UTC  arvind
 * 	Cert-API drop 2 from DEC
 * 	[1996/11/03  23:23 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
 * 
 * Revision /main/DCE_1.2.2/1  1996/10/18  00:39 UTC  arvind
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
 * ABSTRACT:    Registry Policy Implementation.	
 * 
 *	
 * ENVIRONMENT:	portable
 * 
 * AUTHOR:
 *              Mahesh Girkar 
 *		Distributed Processing Engineering (DPE)
 * 
 * DATE:	March 1996
 * 
 */





#ifndef __cplusplus
#error This file requires a C++ compiler
#endif

extern "C" {
#include <string.h>
#include <stdio.h>
#include <pthread.h>
#include <xom.h>
#include <xds.h>
#include <xdsbdcp.h>
#include <xdscds.h>
#include <xdsgds.h>

#include <dce/sec_attr_base.h>
#include <dce/sec_attr_util.h>
#include <dce/binding.h>
#include <dce/uuid.h>
#include <dce/sec_login.h>
#include <dce/sec_rgy_attr.h>
#include <dce/secidmap.h>
#include <dce/sec_pk.h>


}
#include <dce/pkc_plcy_reg.h>
#include <dce/pkc_plcy_registry.h> 
#include <dce/pkc_plcy_oid.h>

#define policy_name "pkc_registry"


/* following char * uuid's are from src/security/sec_pk.h */
/* For now, we directly use the uuid's that are in variable
 * DCEPKAuthentication_id & and DCEPKKeyEncipherment_id in sec_pk.h
 */

#define DCEPK_AUTHENTICATION_UUID_STR "d44dc60e-6230-11cf-98ed-08000919ebb5" 
#define DCEPK_KEYINCIPHERMENT_UUID_STR "76251f8c-6230-11cf-af89-08000919ebb5"

/* two keys are stored in the security registry for a principal --
 * a authentication key and an encipherment key.
 * If we find only one of these, we still say we have two keys 
 * and return a count of 2 keys to the user.
 * In this case, the keydata for one of the keys will be null.
 * This way, the user can distinguish correctly whether the key returned
 * is the authentication key or the encipherment key, as we we fix 
 * the key_index of authentication key to be 0 and key index of 
 * enchiperment key to be 1.
 */

typedef struct registry_keyinfo {
  unsigned char *key[2];
  size_t key_length[2];
  unsigned is_auth_key[2];
  int is_auth;
  unsigned keycount;
} registry_keyinfo_t;



pkc_plcy_flags_t pkc_registry_flags = {0, 0};
// Specify 1 in the first component if the policy is threadsafe.
// Specify 1 in the second component if the policy is multisession.
    
OM_IMPORT(DSX_TYPELESS_RDN)

class {
    int t;
} registry_context_t;



/* mbg: forward prototypes to satisfy compiler.. */
error_status_t pkc_convert_x500name_to_dce_principal_name
                                           (
					   const x500name &subjectName, 
					   sec_rgy_name_t princ_p
					   );





//
// bind to a rgy site (query only required)
//

static error_status_t bind_rgy_site_and_parse_name 
                                    (sec_rgy_name_t princ,
                                     sec_rgy_handle_t *context, 
                                     sec_rgy_name_t &canon_rgy_principal,
                                     int *is_auth)
{
  error_status_t st;
  sec_login_handle_t lc;
  sec_rgy_bind_auth_info_t    auth_info;
  sec_rgy_name_t canon_rgy_cell;
  signed32 idexp;

  *context = 0;
  *is_auth = 0;
  auth_info.info_type = sec_rgy_bind_auth_none;

  sec_login_get_current_context(&lc, &st);

  if (st == error_status_ok) { 
    sec_login_get_expiration(lc, &idexp, &st);

    /* we accept any context that is valid, but not necessarily certified */
    if (st == sec_login_s_not_certified || st == error_status_ok) {
      auth_info.info_type = sec_rgy_bind_auth_dce;
      auth_info.tagged_union.dce_info.authn_level = rpc_c_protect_level_pkt_integ;
      auth_info.tagged_union.dce_info.authn_svc = rpc_c_authn_default;
      auth_info.tagged_union.dce_info.authz_svc = rpc_c_authz_dce;
      auth_info.tagged_union.dce_info.identity = lc;
      if (st != sec_login_s_not_certified)  /* only if login was certified */
        *is_auth = 1;
    }
  }
    
  /* now parse the name */
  sec_id_global_parse_name(&auth_info,
                           princ,
                           canon_rgy_cell,
                           NULL, 
                           canon_rgy_principal,
                           NULL,
                           &st);
                            
  if (st) 
    return st; 

  sec_rgy_site_bind_query(canon_rgy_cell,
                          &auth_info,
                          context,
                          &st);

  
  return st;
}
  
//
// get_key_data_from_era
//


static unsigned32 get_key_data_from_era (
                                  sec_rgy_name_t princ_p,
                                  uuid_t attr_uuid,
                                  unsigned char **key,
                                  size_t & len,
                                  int & is_auth
				  ) 
{

 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">get_key_data_from_era %s",princ_p));

#define MAX_ATTRS 1

  sec_attr_cursor_t       cursor;
  unsigned32              num_attr_keys = MAX_ATTRS;
  sec_attr_t              attr_keys[MAX_ATTRS];
  sec_attr_t              out_attrs[MAX_ATTRS];
  unsigned32              num_ret;
  unsigned32              num_left;
  error_status_t          st, st1;
  sec_rgy_name_t              canon_rgy_cell;
  sec_rgy_name_t              canon_rgy_principal;
  sec_rgy_handle_t   rgy_context;

  *key = 0;
  len = 0;

  sec_rgy_attr_cursor_alloc(&cursor, &st);
  if ( st != error_status_ok ) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
           "get_key_data_from_era sec_rgy_attr_cursor_alloc failed %ld",st));
      return pkc_s_nomem;
  }
  
  attr_keys[0].attr_id =  attr_uuid;
  attr_keys[0].attr_value.attr_encoding=sec_attr_enc_void;
  
  st = bind_rgy_site_and_parse_name (princ_p,
                                     &rgy_context,
                                     canon_rgy_principal,
                                     &is_auth);

  if (st) return st;

  sec_rgy_attr_lookup_by_id(rgy_context,
                            sec_rgy_domain_person,
                            canon_rgy_principal,
                            &cursor,
                            (unsigned32)1,
                            (unsigned32)1,
                            attr_keys, 
                            &num_ret,
                            out_attrs,
                            &num_left,
                            &st);
  
  sec_rgy_attr_cursor_release(&cursor, &st1);
  if (st1 != error_status_ok) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
           "get_key_data_from_era sec_rgy_attr_cursor_release failed %ld",st1));
  }

  sec_rgy_site_close(rgy_context, &st1);
  
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "get_key_data_from_era: sec_rgy_attr_lookup_by_id returned %ld",st));

  if (st != error_status_ok
      || num_ret != 1
      || (boolean32) uuid_compare(&attr_keys[0].attr_id, &out_attrs[0].attr_id, &st1 )) {

      if (num_ret > 1) sec_attr_util_inst_free_ptrs(&out_attrs[0]); 
      return pkc_s_key_not_found;
  };


  if ((out_attrs[0].attr_value.tagged_union.bytes)->length == 0) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
         "get_key_data_from_era: key is zero length !!!!"));

    sec_attr_util_inst_free_ptrs(&out_attrs[0]); 
    return pkc_s_key_not_found;
  } 

/*  
 *  The actual value of the BER encoding of the public key is 
 *  stored in the out_attrs[0].attr_value.tagged_union field, where
 *  the length is given by ... and the actual pointer to the BER 
 *  encoding is given by ...; So, basically, we just copy this BER
 *  encoding in our key structure in our public key world..
 *
 *  return(out_attrs[0].attr_value.tagged_union....);
 *
 */

 

   *key = (unsigned char *) pkc_malloc((out_attrs[0].attr_value.tagged_union.bytes)->length);
   if (!(*key)) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
           "get_key_data_from_era: Ran out of memory"));
     sec_attr_util_inst_free_ptrs(&out_attrs[0]); 
     return pkc_s_nomem;
   }

   len = (out_attrs[0].attr_value.tagged_union.bytes)->length;
   memcpy (*key, (out_attrs[0].attr_value.tagged_union.bytes)->data, len);

   sec_attr_util_inst_free_ptrs(&out_attrs[0]); 
   DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           "<get_key_data_from_era"));
   return 0;
}




//
// pkc_registry routines.
//


char * pkc_registry_name(void) 
{
    char * the_name;
    the_name = (char *)pkc_malloc(sizeof(policy_name) + 1);
    strcpy(the_name, policy_name);
    return the_name;
}

unsigned32 pkc_registry_open(void** context) 
{
    return 0;
}

unsigned32 pkc_registry_close(void** context) 
{
    return 0;
}


/*
 * pkc_registry_establish: For registry policy, it suffices to
 * copy the initial trust list into the out trust list.
 * There is nothing in this policy to extend the trustbase .
 */


unsigned32 pkc_registry_establish (
				  void ** context,
				  const pkc_trust_list_t & trust,
				  const utc_t * date,
				  char initial_explicit_policy_required,
                                  void **tbase 
                                 )
{
  return 0;
}

//
// pkc_registry_retrieve
//

unsigned32 pkc_registry_retrieve (
                        void ** context,
                        const void * trustbase,
                        const x500name & subjectName,
                        const utc_t * date,
                        const uuid_t & desired_domain,
                        const pkc_generic_key_usage_t & desired_usage,
                        char initial_explicit_policy_required,
                        void ** keyinfo
                       ) 
{



  error_status_t          st1, st2;
  sec_rgy_name_t          princ_p;
  struct registry_keyinfo *rkeyinfo = 0;
  error_status_t st;
  int is_auth;
  boolean32 domain_ok;

   DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">pkc_registry_retrieve"));

  if (!keyinfo) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
           "pkc_registry_retrieve: Bad parameters"));
    return pkc_s_asn_bad_param;
  }


  *keyinfo = 0;

  /* currently, we don't use the context field; if at some point we
   * get a "context" from the sec_rgy* calls that we use here, we can
   * save that in the context field.
   */

  /* the basic idea is to get the public key that is saved in the
   * registry for this particular user.
   */

  /*
   * There should only be one instance of CA_ERA_ID attached
   * to the principal, so we do not need to use sec_rgy_attr_cursor_init() (which
   * is a remote interface) to initialize the cursor.
   */

  domain_ok = uuid_equal((uuid_t* )&desired_domain,
                         &sec_pk_domain_dce_general,
                         &st);
  if (st) return pkc_s_asn_bad_param;

  if (!domain_ok) {
     domain_ok = uuid_equal((uuid_t* )&desired_domain,
                            &sec_pk_domain_unspecified,
                            &st);
     if (st) return pkc_s_asn_bad_param;
  };

  if (!domain_ok) return 0;
      // We don't support the requested domain, so just return.
      // When the user asks for a keycount, we'll say we didn't
      // find aything.

  st1 = pkc_convert_x500name_to_dce_principal_name(subjectName,princ_p);
  if (st1 != 0) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
           "pkc_registry_retrieve: pkc_convert_x500name_to_dce_principal_name %ld",
           st1));
      return(st1);
    }

  rkeyinfo = (struct registry_keyinfo *) pkc_malloc
                   (sizeof (struct registry_keyinfo));
  if (!(rkeyinfo)) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
           "pkc_registry_retrieve: Ran out of mem"));
    return pkc_s_nomem;
  }

  rkeyinfo->keycount = 0;
    

  if (desired_usage.permitted & PKC_KEY_USAGE_AUTHENTICATION) {

      st1 =  get_key_data_from_era (
                              princ_p,
                              DCEPKAuthentication_id,
                              &(rkeyinfo->key[rkeyinfo->keycount]),
                              (rkeyinfo->key_length[rkeyinfo->keycount]),
                              is_auth
                              );
    if (st1 == 0) {
	rkeyinfo->is_auth_key[rkeyinfo->keycount] = 1;
        rkeyinfo->keycount++;
    };
  } else {
    st1 = 1;
  };


  if (desired_usage.permitted & PKC_KEY_USAGE_KEY_ENCIPHERMENT) {

      st2 =  get_key_data_from_era (
                              princ_p,
                              DCEPKKeyEncipherment_id,
                              &(rkeyinfo->key[rkeyinfo->keycount]),
                              (rkeyinfo->key_length[rkeyinfo->keycount]),
                              is_auth
                              );
    if (st2 == 0) {
	rkeyinfo->is_auth_key[rkeyinfo->keycount] = 0;
        rkeyinfo->keycount++;
    };
  } else {
     st2 = 1;
  };

   // if both keys are not present, return success for now.. 
   // when the user tries to find how may keys there are (get_key_count...),
   //  we will return 0  
  if (st1 != 0 && st2 != 0) {
    pkc_free(rkeyinfo);
    return 0;
  };
  // atleast one is present.

 
  rkeyinfo->is_auth = is_auth;
  *keyinfo = (void *) rkeyinfo;

   DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           "<pkc_registry_retrieve"));

  return 0;
}

//
// pkc_registry_delete_keyinfo
//

unsigned32  pkc_registry_delete_key_info(void **context,
                                    void **keys_handle)
{
  struct registry_keyinfo *keyinfo;
  int i;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">pkc_registry_delete_key_info"));

  if (!keys_handle)
    return 0;

  keyinfo =   (struct registry_keyinfo *) *keys_handle;
  
  for (i=0; i<keyinfo->keycount; i++) {
      if (keyinfo->key[i] != NULL)
         pkc_free (keyinfo->key[i]);
  };
  pkc_free (keyinfo);
  *keys_handle = 0;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           "<pkc_registry_delete_key_info"));
  return 0;
}

//
// pkc_registry_get_key_count
//

unsigned32 pkc_registry_get_key_count 
  (void ** context, void * keys_handle, size_t * key_count)
{
  struct registry_keyinfo *keyinfo;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">pkc_registry_get_key_count"));

  // key_count should be presetn.
  if (!key_count)
    return pkc_s_asn_bad_param;

  keyinfo = (struct registry_keyinfo *) keys_handle;
  if (!keyinfo) {  // possible in pkc_registry_retrieve
    *key_count = 0;
  } else {
    *key_count = keyinfo->keycount;
  }

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           "<pkc_registry_get_key_count"));
  return 0;
}

//
// pkc_registry_get_key_data
//

unsigned32 pkc_registry_get_key_data
  (void ** context,
   void * keys_handle,
   unsigned key_index,
   unsigned char ** key_data,
   size_t * key_length)
{
  struct registry_keyinfo *keyinfo;
  unsigned char *k;
  size_t l;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">pkc_registry_get_key_data"));

  if (!keys_handle ||  !key_data  || !key_length)
    return pkc_s_asn_bad_param;


  *key_data = 0;
  *key_length = 0;

  keyinfo = (struct registry_keyinfo *) keys_handle;
  
  if (!keyinfo) {  // possible in pkc_registry_retrieve
                   // if neither  of the  two keys were present
    *key_data = 0;
    *key_length = 0;
    return pkc_s_index_out_of_range;
  } else {

    if (key_index + 1 > keyinfo->keycount)
      return pkc_s_index_out_of_range;

    k=keyinfo->key[key_index];
    l = keyinfo->key_length[key_index];

    if (k) {  /* there was a key, with length l */
      *key_data = (unsigned char *) pkc_malloc (l);
      if (!(*key_data)) {
         DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
           "pkc_registry_get_key_data Ran out of mem"));
        return pkc_s_nomem;
      }

      memcpy (*key_data, k, l);
      *key_length = l;
    }
  }

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           "<pkc_registry_get_key_data"));

  return 0;
}

//
// pkc_registry_get_key_trust,

unsigned32 pkc_registry_get_key_trust(void ** context,
                                      void * keys_handle,
                                      unsigned key_index,
                                      certification_flags_t * flags,
				      uuid_t * domain,
				      pkc_generic_key_usage_t * usages)
{
  struct registry_keyinfo *keyinfo;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">pkc_registry_get_key_trust"));


  keyinfo = (struct registry_keyinfo *) keys_handle;
  if (!keyinfo) {
    return pkc_s_index_out_of_range;
  }

  if (key_index + 1 > keyinfo->keycount)
    return pkc_s_index_out_of_range;

  if (flags) {

     flags->missing_crls = FALSE;
     flags->revoked = FALSE;
     if (keyinfo->is_auth)
       flags->trust_type = DIRECT_TRUST;
     else
       flags->trust_type = UNTRUSTED;
  };
  if (domain) {
    *domain = sec_pk_domain_dce_general;
  };

  if (usages) {
    if (keyinfo->is_auth_key[key_index]) {
       *usages = PKC_KEY_USAGE_AUTHENTICATION;
    } else {
       *usages = PKC_KEY_USAGE_KEY_ENCIPHERMENT;
    };
  };
 
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           "<pkc_registry_get_key_trust"));
  
  return 0;
}

unsigned32 pkc_registry_get_key_certifier_count( void ** context,
                                                 void * keys_handle,
                                                 unsigned key_index,
                                                 size_t * certifier_count)
{
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">pkc_registry_get_key_certifier_count"));

  if (!keys_handle ||  !certifier_count)
    return pkc_s_asn_bad_param;

  if (key_index != 0  && key_index != 1)
    return pkc_s_index_out_of_range;

  *certifier_count = 0;
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           "<pkc_registry_get_key_certifier_count"));
  return 0;
}

unsigned32 pkc_registry_get_key_certifier_info( void ** context,
                                         void * keys_handle,
                                          unsigned key_index,
                                          unsigned ca_index,
                                          char ** ca_name,
                                          utc_t * certification_start,
                                          utc_t * certification_expiration,
                                          char  * is_crl_valid,
                                          utc_t * last_crl_seen,
                                          utc_t * next_crl_expected)
{
  return pkc_s_unimplemented;
}


/* 
 * pkc_convert_x500name_to_dce_principal_name:
 * Input: subjectName,
 * Output: return value, & princ_p
 */



error_status_t
pkc_convert_x500name_to_dce_principal_name(
					   const x500name &subjectName, 
					   sec_rgy_name_t princ_p
					   )
{
  int i, j, found = 0;
  RelativeDistinguishedName *rdn;
  buffer_t buf,buf1;
  x500name *temp;
  gss_OID v;
  unsigned32 status;
  char *p;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">pkc_convert_x500name_to_dce_principal_name"));

  status = subjectName.display(buf,1);
  if (status != 0)
    return status;

    
  p = (char *) &princ_p[0]; 
  memcpy(p, buf.data, buf.data_length); 
  *(p + buf.data_length) = '\0';
  return 0;
 
#if 0
  /* 
   * mbg: call the appropriate public class in the x500name to
   * retreive the name in subjectName, and fill the sec_rgy_name_t
   * structure.
   */

  for (i = 0; i < subjectName.child_count(); i++) { 
      rdn = subjectName[i];
      ((*rdn)[0])->AttributeType.get_value (v);
      if ((memcmp (v->elements,
                  OMP_D_DSX_TYPELESS_RDN,
                  OMP_LENGTH(DSX_TYPELESS_RDN)) == 0)  && 
                  (v->length == OMP_LENGTH(DSX_TYPELESS_RDN))) {
           (*rdn)[0]->AttributeValue.display(buf1);
           buf.append(buf1);
           if (i != subjectName.child_count()-1)
             buf.append("/");
      } else {
         DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
           "pkc_convert_x500name_to_dce_principal_name: Returning invalid name"));
        return pkc_s_asn_invalid_value;
      }
   }
 

  memcpy(princ_p, buf.data, buf.data_length); 
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           "<pkc_convert_x500name_to_dce_principal_name"));
  return 0;

#endif 
}

unsigned32  pkc_registry_delete_trustbase (void **context, void **trustbase)
{
  return 0;
}


pkc_policy_t pkc_registry = {
    pkc_V1,
    {
      PKC_ENC_LENGTH(PKC_C_PLCY_DCE_REGISTRY_OID_ENC),
      PKC_C_PLCY_DCE_REGISTRY_OID_ENC
    },
    {
      pkc_registry_flags.threadsafe,
      pkc_registry_flags.multi_session
    },
    "",
    pkc_registry_name,
    pkc_registry_open,
    pkc_registry_close,
    pkc_registry_establish,
    pkc_registry_retrieve,
    pkc_registry_delete_trustbase,
    pkc_registry_delete_key_info,
    pkc_registry_get_key_count,
    pkc_registry_get_key_data,
    pkc_registry_get_key_trust,
    pkc_registry_get_key_certifier_count,
    pkc_registry_get_key_certifier_info,
};
