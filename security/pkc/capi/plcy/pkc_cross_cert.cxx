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
 * $Log: pkc_cross_cert.cxx,v $
 * Revision 1.1.2.1  1996/11/13  20:32:34  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  20:24:07  arvind]
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
 * ABSTRACT:    This module is responsible for storage/retrieveal of cross-certs
 *              found during establishment of trustbase. These certificates
 *              are processed during the trust completion phase.
 * 
 *	
 * ENVIRONMENT:	portable
 * 
 * AUTHOR:
 *              Mahesh B. Girkar 
 *		Distributed Processing Engineering (DPE)
 * 
 * DATE:	March 1996
 * 
 */







extern "C" {
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
}

#include "pkc_cross_cert.h"
#include "pkc_common.h"
#include <dce/dcepkcmsg.h>


/* given cert, length, and ca_db_entry, adds cert to existing list of certs for ca */
/* does not bother checking whether cert already exists or not.. worst case, we */
/* will process the cert twice later on.. this is not a problem */
static unsigned32 add_certificate
    (void *cert, size_t length, struct ca_db_entry *cainfo);

/* create new ca_db_entry; if head is null, set head to the new entry */
/* will not create a duplicate entry if one already exists */
static unsigned32 add_ca_to_crdb (char *ca, struct ca_db_entry **head);





/* externally seen functions in this module */




#if 0
/* basically, add_ca_to_crdb & then add_certificate */
unsigned32 add_ca_and_cert_to_crdb
    (char *ca, void *cert, size_t length, struct ca_db_entry **cainfo);



/* delete ca_db_entry; adjust head if necessary */
unsigned32 delete_ca_from_crdb (char *ca, struct ca_db_entry **head);

/* if ca is present, returns the pointer to ca_db_entry for the ca */
unsigned32 is_ca_present (char *ca, struct ca_db_entry *head,
                          struct ca_db_entry **ret_ca,
                          struct ca_db_entry **prev_ca);

/* free up memory contained taken up by certificate */ 
/** unsigned32 free_certificate (struct cert_info **c);  **/

/* free memory for a ca_db_entry database */
unsigned32 free_ca_db (struct ca_db_entry **head);

#endif



/*
 * add_certificate
 * given a ca_db_entry ptr, this routine adds a certificate
 * to the cert list.
 */

static unsigned32 add_certificate
    (void *cert, size_t length, struct ca_db_entry *cainfo)
{
  struct cert_entry *t = 0;
  
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
            ">add_certificate"));  

  if ((!cert) || (!cainfo)) {
    return pkc_s_asn_bad_param;
  }
  
  t = (struct cert_entry *) pkc_malloc (sizeof(struct cert_entry));
  if (!t)  {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1, 
            ">add_certificate.. No memory"));  
    return pkc_s_nomem;
  }

  /* copy the cert */
  t->c = (void *) pkc_malloc(length);
  if (!t->c)  {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1, 
            ">add_certificate.. No memory"));  
    pkc_free (t);
    return pkc_s_nomem;
  }
  memcpy(t->c, cert, length);
  t->size = length;

  t->next = 0;
  
  if (cainfo->tcerts) {  /* there was atleast one cert for this ca already */
    cainfo->tcerts->next = t;
    cainfo->tcerts = t;    
  }
  else {  /* there were no certs for this ca */
    cainfo->hcerts = t;
    cainfo->tcerts = t;
  }

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
            "<add_certificate"));  
  return 0;
} 

unsigned32 add_ca_and_cert_to_crdb
    (char *ca, void *cert, size_t length, struct ca_db_entry **head)
{
  unsigned32 status;
  struct ca_db_entry *temp = 0, *tempprev = 0;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
            ">add_ca_and_cert_to_crdb"));  

  if (!ca || !cert || !head) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1, 
            ">add_ca_and_cert_to_crdb returning bad parameters"));  
    return pkc_s_asn_bad_param;
  }
  
  status = add_ca_to_crdb(ca, head);
  if (status != 0) { 
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1, 
            ">add_ca_and_cert_to_crdb returning %ld",status));  
    return status;
  }

  status = is_ca_present(ca, *head, &temp, &tempprev);
  if (status != 1) { 
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
            ">add_ca_and_cert_to_crdb returning %ld",status));  
    return status;
  }

  status = add_certificate(cert, length, temp);
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
            "<add_ca_and_cert_to_crdb"));  
  return status;
  
}

/*
 * delete_certificate
 * deletes certificate as well as the memory pointed
 * by the in pointer passed
 */

unsigned32 delete_certificate (struct cert_entry *c)
{
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
            ">delete_certificate"));  
  if (!c) return 0;

  if (c->c)
    pkc_free (c->c);

  pkc_free (c);
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
            "<delete_certificate"));  
  return 0;
}

/*
 * add_ca: given a prev_ca, adds a new one after it.
 */

static unsigned32 add_ca_to_crdb (char *ca, struct ca_db_entry **head)
{
  unsigned32 status = 0; 
  ca_db_entry *tca = 0;
  ca_db_entry *temp = 0, *tempprev = 0;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
            ">add_ca_to_crdb"));  

  if (!ca) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1, 
            "add_ca_to_crdb Got bad parameters"));  
    return pkc_s_asn_bad_param;
  }

  status = is_ca_present(ca, *head, &temp, &tempprev);
  if (status == 0) {  /* not found */
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
            "add_ca_to_crdb ca is present"));  

    tca = (struct ca_db_entry *) pkc_malloc (sizeof(struct ca_db_entry));  
    if (!tca) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1, 
            "add_ca_to_crdb Out of memory"));  
      return pkc_s_nomem;
    }

    tca->ca = (char *) pkc_malloc (strlen(ca)+1);
    if (!tca->ca) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1, 
            "add_ca_to_crdb Out of memory"));  
      pkc_free (tca);
      return pkc_s_nomem;
    }

    strcpy(tca->ca, ca);
    tca->hcerts = 0;
    tca->tcerts = 0;
    tca->next = 0;
    tca->next = *head;
    *head = tca;

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
            "<add_ca_to_crdb"));  
    
    return 0;
  }

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
          "add_ca_to_crdb ca is not present"));  
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
          "<add_ca_to_crdb"));  
  return 0;


}  

/*
 * delete_ca_from_crdb: deletes ca and memory for its certificate list
 */

unsigned32 delete_ca_from_crdb (char *ca, struct ca_db_entry **head)
{
  struct ca_db_entry *temp = 0, *tempprev = 0;
  unsigned32 status;
  struct cert_entry *c, *p;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
          ">delete_ca_from_crdb"));  

  status = is_ca_present(ca, *head, &temp, &tempprev);
  if (status == 0) {  /* not found, so return success */
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
            "delete_ca_from_crdb: ca is not present"));  
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
            "<delete_ca_from_crdb"));  
    return 0;
  }

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
          "delete_ca_from_crdb: ca is  present"));  

  c = temp->hcerts;

  while (c != 0) {
    p = c;
    c = c->next;
    delete_certificate (p);
  }

  temp->hcerts = 0;
  temp->tcerts = 0;
  if (temp == *head) { /* adjust head */
    *head = temp->next;
  } else {
    tempprev->next = temp->next;
  }    
  pkc_free (temp);

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
         "<delete_ca_from_crdb"));  


  return 0;
}

/* 
 * is_ca_present: returns 1 if ca is present in the list
 * starting with cainfo; if found, ret_ca holds a pointer
 * it.
 */

unsigned32 is_ca_present (char *ca, struct ca_db_entry *cainfo,
                          struct ca_db_entry **ret_ca, 
                          struct ca_db_entry **prev_ca)
{

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
         ">is_ca_present"));  


  if (!ret_ca || !prev_ca) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1, 
         "is_ca_present: ret_ca or prev_ca is null"));  
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
         "<is_ca_present"));  
    return 0;
  }


  *ret_ca = 0;
  *prev_ca = 0;
  if (!cainfo) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
         "is_ca_present: cainfo is null"));  
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
         "<is_ca_present"));  
    return 0;
  }
  if (!ca) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1, 
         "is_ca_present: ca is null"));  
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
         "<is_ca_present"));  
    return 0;
  }

  
  while (cainfo != 0) {
    if (!strcmp (cainfo->ca, ca)) {  /* found it */
      *ret_ca = cainfo;
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
         "is_ca_present: ca was found"));  
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
         "<is_ca_present"));  
      return 1;
    }
    *prev_ca = cainfo;
    cainfo = cainfo->next;
  }

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
     "<is_ca_present"));  

  return 0;
}

unsigned32 free_ca_db (struct ca_db_entry **head)
{
 struct ca_db_entry *temp = 0, *temp1 = 0;
 struct cert_entry *c = 0, *p = 0;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
     ">free_ca_db"));  

 if (!head)
   return 0;

 temp = *head;
 while (temp) {
   c = temp->hcerts;
   while (c != 0) {
    p = c->next;
    pkc_free (c->c);   
    pkc_free(c);
    c = p;
   }
   temp1 = temp->next;
   pkc_free (temp);
   temp = temp1;
 }

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9, 
     "<free_ca_db"));  
 return 0;
}

