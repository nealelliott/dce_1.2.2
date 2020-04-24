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
 * $Log: pkc_plcy_oid.h,v $
 * Revision 1.1.2.1  1996/11/13  20:39:17  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  19:56:23  arvind]
 *
 * Revision /main/DCE_1.2.2/2  1996/11/04  22:26 UTC  arvind
 * 	Cert-API drop 2 from DEC
 * 	[1996/11/03  23:23 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
 * 
 * Revision /main/DCE_1.2.2/1  1996/10/18  00:40 UTC  arvind
 * 	CAPI drop from DEC
 * 	[1996/10/16  20:31 UTC  arvind  /main/arvind_capi/1]
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
 * ABSTRACT:    OID's used in 	CAPI
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





#ifndef PKC_PLCY_OID_
#define PKC_PLCY_OID_

extern "C" {
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <xom.h>
}


/*
 * OID's used by CAPI library.
 *
 * Some of these are fixed; i.e. set by the ISO standard
 * on which the CAPI is based; others are assigned by
 * us, and may need to change.
 * 
 * I have put the OID's that may change at the top
 * followed by the OID's that don't change at the
 * bottom.
 *
 * Also, note that the CAPI library does not make
 * use of the string version of these OID's at all.
 * Those are here for readability of the OID itself!
 * I have not put them in a comment, because we may end
 * up using these char * versoins at some point.
 */

#define PKC_ENC_LENGTH(x) (sizeof(x)-1)
#define PKC_DECLARE_OID(x,y) \
const OM_object_identifier x = { PKC_ENC_LENGTH(y), y }


/*
 * PKC_C_PLCY_DCE_REGISTRY_OID
 */


#define PKC_C_PLCY_DCE_REGISTRY_OID "1.3.24.9.13"
#define PKC_C_PLCY_DCE_REGISTRY_OID_ENC "\x2b\x18\x09\x0d"
PKC_DECLARE_OID(pkc_c_policy_dce_registry, PKC_C_PLCY_DCE_REGISTRY_OID_ENC);


/* 
 * PKC_C_PLCY_DCE_HIERARCHY_OID 
 */

#define PKC_C_PLCY_DCE_HIERARCHY_OID "1.3.24.9.12"
#define PKC_C_PLCY_DCE_HIERARCHY_OID_ENC "\x2b\x18\x09\x0c"
PKC_DECLARE_OID(pkc_c_policy_dce_hierarchy, PKC_C_PLCY_DCE_HIERARCHY_OID_ENC);


/* mbg: The principal directory attribute OID 
 * equivalent. This is the attribute we lookup for
 * finding the relative position in CDS to lookup
 * for certificates in a CDS cell.
 *
 */

#define PRINCIPAL_DIRECTORY_OID "1.3.24.9.15"
#define PRINCIPAL_DIRECTORY_OID_ENC "\x2b\x18\x09\x0f"
PKC_DECLARE_OID(principal_directory_attribute, PRINCIPAL_DIRECTORY_OID_ENC);

/* 
 *  OID's that don't change 
 */



/* USER_CERT_OID is the oid of the attribute that will
 * store user certificates. We lookup certificates stored
 * in this attribute only for the final target in 
 * the trust chain.
 */

#define USER_CERT_OID "2.5.4.36"
#define USER_CERT_OID_ENC "\x55\x04\x24"
PKC_DECLARE_OID(user_cert_attribute, USER_CERT_OID_ENC);

/*
 * CROSS_CERT_OID is the oid of the attribute that will
 * store cross cert certificates. We lookup certificates
 * stored in CROSS_CERT_OID as we establish the initial
 * part of the trustchain (or trustbase). Any certificates
 * stored in this attribute that don't belong to the 
 * principal being looked up are stored under the
 * other principal's list. This "other" certs will be
 * retrieved during the downward chain, when we complete
 * trust along with certificates stored in USER_CERT_OID
 * and CA_CERT_OID.
 */

#define CROSS_CERT_OID "2.5.4.40"
#define CROSS_CERT_OID_ENC "\x55\x04\x28"
PKC_DECLARE_OID(cross_cert_attribute, CROSS_CERT_OID_ENC);

/*
 * CA_CERT_OID. SImilar to USER_CERT_OID, except it
 * is always looked up when establishing trustbase
 * and completing trust. 
 */

#define CA_CERT_OID "2.5.4.37"
#define CA_CERT_OID_ENC "\x55\x04\x25"
PKC_DECLARE_OID(ca_cert_attribute, CA_CERT_OID_ENC);

/*
 * If there are any revoked certificates, their serial number
 * will be stored under this attribute for a principal.
 */

#define CA_REVOCATION_LIST_OID "2.5.4.38"
#define CA_REVOCATION_LIST_OID_ENC "\x55\x04\x26"
PKC_DECLARE_OID(ca_revocation_list_attribute, CA_REVOCATION_LIST_OID_ENC);

/*
 * USER_REVOCATION_LIST_OID: Similar to
 * CA_REVOCATION_LIST_OID, except for a particular user.
 */

#define USER_REVOCATION_LIST_OID "2.5.4.39"
#define USER_REVOCATION_LIST_OID_ENC "\x55\x04\x27"
PKC_DECLARE_OID(user_revocation_list_attribute, USER_REVOCATION_LIST_OID_ENC);

#endif
