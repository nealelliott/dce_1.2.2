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
 * $Log: pkc_cross_cert.h,v $
 * Revision 1.1.2.1  1996/11/13  20:37:35  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  19:55:00  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/10/18  00:39 UTC  arvind
 * 	CAPI drop from DEC
 * 	[1996/10/16  20:24 UTC  arvind  /main/arvind_capi/1]
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
 * ABSTRACT:    Cross-Cert module definitions.	
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







#ifndef PKC_CROSS_CERT_
#define PKC_CROSS_CERT_

extern "C" {
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <dce/dce.h>
}



/*
 * Structure to hold a linked-list of certificates 
 */

struct cert_entry {
  void *c;  /* asn-1 encoding of certificate */
  size_t size;  /* size of certificate */
  struct cert_entry *next;  /* next certificate */
} ;

/* 
 * Structure to hold a linked list of ca's and their
 * certificates found during looking up of CROSS_CERT_OID
 */

struct ca_db_entry {
  char *ca;
  struct cert_entry *hcerts;  /* head of linked list of certificates for this ca */
  struct cert_entry *tcerts;  /* tail of linked list of certificates for this ca */
  struct ca_db_entry *next;  /* next ca */
} ;

/* free memory for a ca_db_entry database */
unsigned32 free_ca_db (struct ca_db_entry **head);

/* if ca is present, returns the pointer to ca_db_entry for the ca */
unsigned32 is_ca_present (char *ca, struct ca_db_entry *head,
                          struct ca_db_entry **ret_ca,
                          struct ca_db_entry **prev_ca);

/* basically, add_ca_to_crdb & then add_certificate */
unsigned32 add_ca_and_cert_to_crdb
    (char *ca, void *cert, size_t length, struct ca_db_entry **cainfo);


 
#endif
