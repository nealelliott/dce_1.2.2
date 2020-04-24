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
 * $Log: pkc_plcy_hierarchy.cxx,v $
 * Revision 1.1.2.1  1996/11/13  20:33:01  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  20:26:10  arvind]
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
 * ABSTRACT:    Hierarchical Policy Module implementation.	
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







#ifndef __cplusplus
#error This file requires a C++ compiler
#endif

extern "C" {
/* #include <string.h> */
/* #include <stdio.h> */
}

#define UTC_OPERATORS
#include <dce/pkc_certs.h>

#include <dce/pkc_plcy_reg.h>
#include <dce/pkc_plcy_hierarchy.h>
#include <dce/xdsxx.h>
#include <xds.h>
#include <xdscds.h>
#include <pkc_plcy_oid.h>
#include <pkc_cross_cert.h>

#define policy_name "pkc_hierarchy"

OM_IMPORT(DSX_TYPELESS_RDN)


pkc_plcy_flags_t pkc_hierarchy_flags = {0, 0};
// Specify 0 if the policy is not threadsafe, 1 if it is.

     
// trust depth should be really set by user who is retrieving key based on hierarchical
// policy; see comment new the function extend_trust_list to understand what
// depth does. For now, we set it to 1;

int tolerate_depth_doing_up = 1;
int tolerate_depth_going_down = 1;

// various times, we need to keep track of whether we are going up or down.
// hence, the new type direction. 
enum direction {up, down};



static void add_certs_in_entry (pkc_trust_list_t *partial, 
			 x500name  & ca, 
                         int is_ca,
			 x500name  & ca_cert_dir, 
			 direction flag,
			 int depth,
                         struct ca_db_entry **cainfo_p,
			 unsigned32 *status);

unsigned32 process_ca_user_certs(pkc_trust_list_t *partial, 
			 x500name  & ca, 
                         int is_ca,
			 x500name &ca_cert_dir,
			 direction flag,
			 int depth,
                         struct ca_db_entry **cainfo_p);
unsigned32 process_cross_certs(pkc_trust_list_t *partial, 
			 x500name  & ca, 
                         int is_ca,
			 x500name &ca_cert_dir,
			 direction flag,
			 int depth,
                         struct ca_db_entry **cainfo_p);
unsigned32 process_cross_certs_in_db(pkc_trust_list_t *partial, 
			 x500name  & ca, 
                         int is_ca,
			 direction flag,
			 int depth,
                         struct ca_db_entry **cainfo_p);

static unsigned32 revoke_certs_if_any (
   const x500name & ca_cert_dir, 
  int is_ca, 
  pkc_trust_list_t *partial);


static unsigned32 ca_present_in_required_depth(x500name  & ca,
					       x500name  & other_ca,
					       direction flag,
					       int depth,
					       int *n1_ret,
					       int *n2_ret);

static unsigned char *convert_x500name_to_printable_form(const x500name &ca);

static unsigned32 convert_x500name_to_string(const x500name & ca, unsigned char *&namebuf);


// extend_trust_list: called when establishing trustbase. It extends the partial trustbase
// (partial)
// and fills up the cross-cert database (cadb_p)

static void extend_trust_list (pkc_trust_list_t *partial, 
			x500name & ca, 
                        direction flag,  int depth, 
                        struct ca_db_entry **cadb_p,
                        unsigned32 *status);

// form_trusted_ca_list_from_trust_list
// given a trust list, it returns a x500name list of ca's that have cut certificates
// in the trust list.

static x500name_list_t *form_trusted_ca_list_from_trust_list
   (const pkc_trust_list_t & initial_trust,  /* initial trust */
    unsigned32 *status);

 static xds_read_result_t *read_xds_cross_certificate_attribute(
    const x500name & ca_cert_dir, 
   int is_ca, 
  unsigned32 *status);

// read_xds_certificate_attribute
// reads certificates from XDS. 
static xds_read_result_t *read_xds_certificate_attribute
             (const x500name & ca, int is_ca, unsigned32 *status);

static unsigned32 determine_relation 
    (const x500name &ca1, const x500name &c2,
     int &ca1_ancestor_ca2,
     int &ca1_descendant_ca2,
     int &ca1_equal_ca2,
     int &ca1_sibling_ca2,
     int &depth_difference) ;


static unsigned32 construct_certificate(unsigned char *asn1_cert_p,
				   unsigned32 length,
                                  Certificate  & cert);

static unsigned32 construct_cross_certificate(
  unsigned char  *asn1_cert_p,
  size_t length, int &cert1_present, 
   unsigned char **asn1_cert1_p, size_t &asn1_length,
  Certificate & cert1, int &cert2_present,
  unsigned char ** asn1_cert2_p, size_t &asn2_length,Certificate & cert2);


unsigned32 pkc_hierarchy_complete_trust(
				void **context,
				const void * trustbase,
				const x500name & subjectName,
				const utc_t * date,
                                const uuid_t & desired_domain,
				const pkc_generic_key_usage_t & desired_usage,
				char initial_explicit_policy_required,
                                void **keyinfo
				);

unsigned32 pkc_hierarchy_establish(void **context,
				   const pkc_trust_list_t &in_trust,
				   const utc_t * date,
				   char initial_explicit_policy_required,
                                   void **tbase
				   );

unsigned32   pkc_hierarchy_delete_trustbase (void **context, void **trustbase);



unsigned32 find_nearest_ancestral_cell_pointer(const x500name & ca, 
                                               x500name * & ca_closest_cell_ptr,
                                               unsigned char * & princ_dir) ;

// compose_x500name concats ca_cell_pointer/princ_dir/ca and returns in ca_cert_dir
unsigned32 compose_x500name(x500name & ca_cell_pointer, 
                            unsigned char *& princ_dir, 
                            x500name & ca, x500name & ca_cert_dir);

unsigned32 rdn_oid_is_typeless (RelativeDistinguishedName *rdn);

unsigned32 return_princ_dir(const x500name & ca,  unsigned char * & princ_dir) ;

unsigned32 is_ca_a_cell_pointer(const x500name & ca, unsigned char * & princ_dir, 
                               unsigned32 & flag) ;

class {
    int t;
} hierarchy_context_t;

char * pkc_hierarchy_name(void) 
{
    char * the_name = 0;

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           ">pkc_hierarchy_name"));

    the_name = (char *)pkc_malloc(sizeof(policy_name) + 1);
    if (!the_name) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
           "pkc_hierarchy_name: Ran out of memory"));
      return 0;
    }
    strcpy(the_name, policy_name);

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
           "<pkc_hierarchy_name"));
    return the_name;
}

unsigned32 pkc_hierarchy_open(void** context) 
{

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
         ">pkc_hierarchy_open"));
    
    if (!context) {

      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
         "pkc_hierarchy_open: bad parameters"));


      return pkc_s_asn_bad_param;
    }

    *context = 0;  /* for now, we dont use it */

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
         "<pkc_hierarchy_open"));

    return 0;
}

unsigned32 pkc_hierarchy_close(void** context) 
{

   DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
         ">pkc_hierarchy_close"));


    *context = NULL;  /* for now, we don't use it */
    
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
         "<pkc_hierarchy_close"));

    return 0;
}



/* mbg: psuedo code..
 *
 */

/*
 *  6/19/96:
 *  mbg: Expanation for those who want to follow this code for trust
 *       policies :-)
 *
 *  Some basics....
 *  ===============
 *  For the purposes of describing  Trust Policies (TP)
 *  we define a User to be any "object" in the distributed computing 
 *  environment who is assigned (possibly more than 1) a public-private 
 *  key pair
 *  for the purposes of secure communication with other users. Examples
 *  of users are real persons, processes, organizations, departments, etc.
 *  Storing/maintaining/managing
 *  private keys is the responsibility of individual User, and can be 
 *  accomplished
 *  using smart cards, Private Key Storage Server (PKSS), etc. Public
 *  keys, however, are generally stored in a directory service (such as
 *  X.500), within
 *  an entity called the "Certificate".
 *  In our DCE implementation, we allow public keys to be stored in
 *  one of two places 
 *        (i) as part of the ERA of a user in the security registry,
 *       (ii) in the X.500 directory as part of certificates.
 *
 *  Why do we need to have trust in someone's public key?
 *  ===================================================== 
 *  Before using a User's public key (regardless of whether it is obtained
 *  from the
 *  security registry, or from X.500) it is important to have   "trust"
 *  in the public key ---
 *  in other words, ensure that the public key is indeed, the public
 *  of the recipient user that the sender  wants to communicate with; 
 *  otherwise, the sender can be 
 *  duped  into sending a secret message to an attacker, instead of
 *  the intended recepient. For example, an attacker could create a new
 *  private-public key pair Priv[k],Public[k], and provide the 
 *  sender with information
 *  to indicate that the receipient's public key is Public[k]. If the
 *  sender decides to use this public key without checking for trust, he or she
 *  can
 *  send sensitive information to the attacker.
 * 
 *  What is a TP?
 *  =============
 *  The purpose of a "trust policy" is to 
 *  describe ways in which public keys can be obtained by  users in
 *  a secure manner. As we have said earlier, 
 *  our current implementation allows for two trust policies.
 *
 *  The first one, is farily simplistic, and says that we trust
 *  the "Security Server" in DCE; In this policy, also called the
 *  "Registry Policy (RP)", public keys are stored in a special
 *  ERA attached to a registry user account. Anyone wishing
 *  to find the public key of a  user can make secure RPC calls
 *  (implicitly done via the sec_rgy_* interface) to the 
 *  security server to obtain the relevant public keys.  
 *
 *  The second trust policy, Heirachical Trust Policy (HTP), 
 *  describes one secure method of obtaining public keys stored in Certificates
 *  in a directory service, such as X.500.
 *  
 *  What is in a Certificate? 
 *  ========================
 *  One cannot discuss fields within a certificate without first 
 *  describing distinguished users called "Certification Authorities(CA)".
 *  A CA is any user who "signs" (or vouches) for the authenticity
 *  of data within a certificate. A CA has a private-public key pair
 *  similar to the key pairs used by other users. A CA "signs" data
 *  within a certificate using its private key. The act of signing
 *  (computing a cryptographic hash of data using a key)
 *  creates a "signature" which is then attached as a field
 *  within the  certificate.
 *
 *  For the sake of describing HTP, we can limit a certificate to 
 *  the following 4 fields:
 *
 *   x500UserName (X500 entry of user, whose public key is in PublickeyofUser),
 *   PublickeyofUser, (actual public key of user)
 *   x500nameofCAthatsignedthiscertificate,
 *                (X500 entry name of CA that signed this certificate)
 *   signature (formed by encrypting a 128 bit md5 hash of the plain text
 *              portion of the certificate using the 
 *              Privatekey of CA on the plaintext
 *              part of the certificate, such as the x500UserName,
 *              PublickeyofUser,
 *              x500nameofCAthatsignedthiscertificate)
 *  
 *   The first 3 fields are in plain text (i.e they are not encrypted).
 *   The last field is a "hash" obtained by using the Privatekey of the CA
 *   issuing this certificate on the plain text part of the certificate.
 *   There are several other "plaintext" fields within a certificate
 *   which are not mentioned here, such as timestamps that indicate the
 *   validity of the certificates, the serial number of certificates, etc.
 *   More fields have been added to certificates in the Version 3 of X.509
 *   standard, such as how to use the public keys, policy ids, alternate
 *   subject keys, etc.
 *    
 *  How does one trust data (or a public key) within a Certificate?
 *  ===============================================================
 *  If a user trusts the authenticity of the public key of the CA
 *  who issued the certificate, then, the user can trust 
 *  the public key of the user contained within the certificate by
 *  adopting the following simple trust procedure (STP):
 *           (i) COmpute the MD5 checksum of the plaintext portion of 
 *               the certificate; Decrypt the signature using the
 *               CA's public key.
 *           (ii) Verify that the result of decryption is identical to
 *                the MD5 checksum.
 *         (iii) If (ii) is succesful, ensure that the certificate is 
 *               trustworty by doing a validity check on the
 *               expiration/start date of the certificate, 
 *               by ensuring that the serial number of the certificate
 *               is not on the list of revoked certificates, etc. 
 *               The details of step (iii) are defined in X.509
 *               standard, and are not critical for  explaining
 *               trust policies; hence we will not go over this step in
 *               great detail.
 *
 *  Since the user trusts the CA's public key, he or she can now trust
 *  the public key of the user contained within the certificate unless
 *  indicated otherwise by the STP procedure.
 *
 *
 *  Trust Chains
 *  ============
 *  Consider the following 3 certificates:
 *
 *  1. <U1, P1, CA1, X1>      [certificate with U1's public key P1 that is
 *                             signed by CA1 (signature X1)]
 *  2. <CA1, PCA1, CA2, X2>   [certificate with CA1's public key PCA1 that is
 *                             signed by CA2 (signature X2)]
 *  3. <CA2, PCA2, CA3, X3>   [certificate with CA2's public key PCA2 that is
 *                             signed by CA3 (signature X3)].
 *
 *  If a user U2 had (I) access to 
 *  the above 3 certificates, and (II)  trust in CA3's public key, then
 *  U2 can obtain trust in U1's public key by repeating STP 3 times as 
 *  follows: The
 *  first run of STP will be on Certificate 3 using CA3's public key
 *  to obtain trust in CA2's public key. The second run of STP will
 *  be on Certificate 2 using CA2's public key to obtain trust in CA1's
 *  public key; and finally, the third run of STP will be  on
 *  Certificate 1 to obtain trust in U1's public key using CA1's
 *  public key.
 *
 *  Is this a chicken & egg problem?
 *  ===============================
 *  Clearly, we have to trust someone's public key, else we will
 *  never trust anyone else's public key within a certificate.
 *  In other words, all users must always  trust
 *  public keys of some initial set of CA's. Also, a user will always 
 *  trust his or her
 *  own public keys (therby trusting certificates signed by the user himself).
 *
 * What is HTP?
 * ============
 * HTP, or heirachical trust policy, describes one secure method of
 * obtaining public keys stored in a directory service, such as X.500,
 * given  an initial list of CA's whose public
 * keys can be trusted.
 *  
 * This policy assumes the following structure on the directory:
 *   1. Entries in the directory are organized in a "tree-like" structure,
 *      such that each node (or entry) other than the root has 
 *      exactly one "parent" node, and zero 
 *      or more children nodes.
 *  
 *   2. Each user has a corresponding (unique) entry in the 
 *      directory. Each such entry (for a particular user) 
 *      contains one or more certificates, all of
 *      which contain the the public key(s) 
 *      assigned to the user, and which are signed by some CA.
 *
 *
 * The first part of HTP describes how a trust list 
 * (a list of "trusted" certificates) can be 
 * obtained for a
 * particular user. This list is thus a list of "other users" (or CA's)
 * whose public keys the user can trust.  
 * Trust lists are obtained using three Rules (described below). 
 *
 * The second part of HTP describes how to "complete" a trust
 * chain from one User (Usource) to another (Utarget), so that 
 * Usource has "trust" in the public key(s)  of Utarget that are
 * contained within the certificates in the X.500 entry for Utarget.
 *
 * ======================================
 * PART I:
 * Three HTP rules for forming trust lists:
 * =======================================
 * HTP describes three rules, which, when applied initially with a
 * list of (CA,public key of CA) that  can be trusted by a user, results in
 * a list of certificates  that also can be trusted by the user.
 * We will refer to the list of (CA, public key of CA) as the
 * trusted public key list (TPKL), and the list of trusted certificates
 * formed using the rules below by TCL. Initially TCL is empty.
 * Note that we don't restrict CA's to have a single public-private
 * key pair. Thus, a TPKL can contain multiple entries for a CA.  
 * 
 *   The three HTP rules for forming a list of trusted certificates
 *   are the following:
 *  
 *   Rule 1: All the CA's, whose public key the user initially trusts, 
 *           have to be
 *           ancestors of the user in the X.500 directory structure.
 *           If any CA from the initial list is not an ancestor, then
 *           the user does not trust the public keys of that CA contained
 *           within the initial list, and prunes TPKL.
 *
 *   Rule 2: A user can trust any certificate signed by a
 *           a CA provided:
 *                (a) The user currently trusts (atleast one of the)
 *                    CA's public key; AND
 *                (b) The CA is an "ancestor" of the user in the X.500
 *                    tree structure; AND
 *                (c) Using one of the trusted 
 *                    public keys of the CA, the user is able to 
 *                    verify the validity of the certificate using the
 *                    STP procedure.
 * 
 *   Rule 3: If Rule 2 holds on a certificate stored in
 *           some ancestor entry,  a user can 
 *                (a) add the certificate to TCL
 *                (b) trust the public key (say P)
 *           of that ancestor user AU (that is stored within the certificate)
 *           and add entry (AU, P)  to TPKL.
 *
 *   Rules 2&3 establish, thru  a "recursive" procedure,
 *   a  list of certificates that
 *   a user can trust by traversing to all the ancestors (and executing
 *   the STP procedure on the certificates contained therein) of the initial
 *   CA's that the user trusts.
 *
 *
 *  There is one variable of HTP that is worth mentioning; we modify
 *  Rule 2(b) to restrict the CA to be a "parent" of some CA that is already
 *  trusted, and which is present in TPKL. We call this the
 *  Restricted HTP policy (RHTP).
 *
 *  All certificates signed for a user are stored in the user's
 *  corresponding x500 entry; Thus, this model assumes that every user
 *  has a unique x500 entry. If the user has an alias, then 
 *  In an x500 entry (pointed by an x500name)
 *  there are possibly zero or more certificates for the x500 user
 *
 * ======================================================
 * PART II:
 * Froming Trust Chains from User Usource, to User Utarget
 * =======================================================
 * 
 * To form a trust chain from Usource to Utarget, HTP 
 * dictates the following process:
 *
 * First, construct the TCL and TPKL for Usouruce [TCL(Usource) and
 *                                                 TPKL(Usource)]
 * Second, construct the TCL and TPKL for Utarget as well
 *         [TCL(Utarget) and TPKL(Utarget)], changing Rule 2
 *         in part I as follows:
 *   Rule 2: A user can trust any certificate signed by a
 *           a CA provided:
 *                (a) The user currently trusts (atleast one of the)
 *                    CA's public key; AND
 *                (b) The CA is an "ancestor" of the user in the X.500
 *                    tree structure; AND
 * That is, we don't perform  the STP procedure outlined in Rule 2(c). 
 * [If we are using the RHTP policy, we change Rule 2(b) as described 
 *  earlier.]
 * 
 * Third, we can stop the construction of TCL(Utarget) and TPKL(Utarget)
 * at any stage, and check whether we can complete a "trust chain"
 * as follows:
 *
 * Check whether there is a certificate in TCL(Utarget) such that
 * it is signed by a CA (say CA1) in TPKL(Usource). If so, apply the 
 * STP procedure
 * using this CA1's public key to determine whether we
 * can trust the public key (say PA2) of CA contained
 * in the certificate (say CA2) in TCL(Utarget). Assuming a trust
 * in PA2, we then check whether we can do the reverse of Rule 2, namely
 * starting from an "ancestor" of Utarget whose public key can be trusted, find
 * a chain of descendant certificates that can be trusted using the
 * certificates in TCL(Utarget), until you trust some certificate 
 * contained in the Utarget entry. If so, Usource can trust the public
 * key of Utarget contanied in the final certificate in this chain.
 *
 * Note that the procedure as outlined in HTP (part I and II) 
 * above is just a description,
 * and should not be construed as an implementation the formation
 * of trust lists and trust chains.
 *
 */




/* 
 * form_trusted_ca_list_from_trust_list forms a list of x500names of CA's that can be 
 * trusted initially; duplicates are not allowed in this list, and 
 * are removed. Thus, if a CA had two public keys that were trusted,
 * this CA is added only once.
 */


static x500name_list_t *form_trusted_ca_list_from_trust_list
   (const pkc_trust_list_t & initial_trust,  /* initial trust */
    unsigned32 *status)
{

  x500name_list_t *ca_list = 0;
  x500name new_x500name;
  pkc_trust_list_element_t *one_trusted_element;

   DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
         ">form_trusted_ca_list_from_trust_list"));


  ca_list = new x500name_list_t();
  if (!ca_list)
    {

     DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
         "form_trusted_ca_list_from_trust_list: Ran out of memory"));

      *status = pkc_s_nomem; 
      return 0;
    }

  /* if initial_trust list is empty, return immediately */
  if (initial_trust.first == NULL)
    {

     DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
         "form_trusted_ca_list_from_trust_list: Trust List is EMpty !!!!"));

      *status = 0;  /* Empty list*/	 
      return ca_list;
    }


  /* so now we have a list of x500names of the CA's */  
  one_trusted_element = initial_trust.first;
  while (one_trusted_element != NULL)  /* while there are CA's to trust,
                                          collect all of them in a list
                                        */
    {


      new_x500name = ((pkc_trusted_key_t *) one_trusted_element)->owner;
      ca_list->add_name(new_x500name);
      one_trusted_element = one_trusted_element->next;
    }

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
         "<form_trusted_ca_list_from_trust_list"));

  *status = 0;
  return (ca_list);
}


// extends trust list present in the first argument by looking up all certificates
// of ca, and ca's anscestors all the way upto the root.
// When you are extending trust list by going up to the parent, there is a choice
// of whether you want to trust all certificates in the parent, or you want to
// consider only those certificates that  are signed by
// just the child (where you are coming from), or from some child that is several
// level below; essentially, given a level (or depth), you are willing to consider
// adding to the  trust chain those certificates of the parent which are signed
// children that are not too deep down (given by depth). Thus, depth is a policy
// variable of the heirarchical policy.
 
static void extend_trust_list (pkc_trust_list_t *partial, 
			x500name & ca, direction flag,  int depth, 
                        struct ca_db_entry **cadb_p,
                        unsigned32 *status)
{
  x500name *temp = 0, temp1, *ca_cell_pointer = 0, ca_cert_dir;
  unsigned char *princ_dir = 0;
  unsigned32 do_call_again = 0;
  int is_ca = 1;
  *status = 0;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
         ">extend_trust_list"));

  if (ca.child_count() == 0) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
         "extend_trust_list: Bad parameters -- ca.child_count is 0"));
    *status = pkc_s_asn_bad_param;
    return;
  }


  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
         "extend_trust_list: ca = %s", convert_x500name_to_printable_form(ca)));
  

  // find_nearest_ancestral_cell_pointer does the following:
  // if there is a cell pointer in the ancestors of ca (i.e. another hierachical namespace)
  // then the x500name of the cell pointer is returned, as well as the principal directory
  // attribute value in the cell pointer xds entry (which may be null).  
  // If there isn't a cell pointer in the ancestors
  // of ca, then ca_cell_pointer is null. In this case, princ_dir is also null.
  
  
  *status = find_nearest_ancestral_cell_pointer(ca, ca_cell_pointer, princ_dir);
  if  (*status != 0) {
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
         "extend_trust_list: f_n_a_c_p failed %ld", *status));
    return;
  }

#ifdef DEBUG
  if (ca_cell_pointer) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
         "extend_trust_list: f_n_a_c_p succeded ca_cell_pointer = %s", 
         convert_x500name_to_printable_form(*ca_cell_pointer)));

    if (princ_dir) 
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
         "extend_trust_list: --- Princ Dir = %s", 
          princ_dir));
    else
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
         "extend_trust_list: --- Princ Dir = NULL"));
    fflush(stdout);   
  } else {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
         "extend_trust_list: Ca cell pointer = NULL"));
  }
#endif

  /* check if we would cross cell boundary when we go to the next parent */
  if (ca_cell_pointer && (*ca_cell_pointer == ca)) {
    do_call_again = 1;
  } else {
    do_call_again = 0;
  }


    if (ca_cell_pointer)  /* if we had a cell pointer at this point, use it to find the real cert dir */
      {
        /* compose_x500name creates a ca_cert_dir x500name from ca_cell_pointer,  princ_dir, temp */
        /* ca_cert_dir will be roughly equal to ca_cell_pointer/princ_dir/some rdn suffix of temp, */

        *status = compose_x500name(*ca_cell_pointer,  princ_dir, ca, ca_cert_dir);
        if (*status != 0) {
           DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
              "extend_trust_list: compose_x500name Failed %ld ", *status));
          return;
        }
      } /* if !ca_cell_pointer  */
    else {
      ca_cert_dir = ca;
    }


  // add_certs for the ca itself. 
    add_certs_in_entry(partial, ca, is_ca, ca_cert_dir, flag, depth, cadb_p,  status);
    if (*status != 0) {

      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
         "extend_trust_list: add_certs_in_entry Failed %ld ", *status));
      return ;
    }

    // if we had ca == ca_cell_pointer, then going one level up
    // assures us that we have crossed a cell boundary; in this
    // case, we need to find the new cell boundary.

    // find first parent of ca
  *status = ca.make_parent (&temp);
  if (*status != 0) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "extend_trust_list: make_parent Failed %ld ", *status));
    return;
  }


  while (temp->child_count() > 0) {  /* while we haven't reached root.. */


    if ((ca_cell_pointer) && do_call_again) { /* if we have crossed the cell boundary, 
                                                          * we need to find the next cell pointer 
                                                          */
        delete ca_cell_pointer;
        if (princ_dir) 
	   pkc_free (princ_dir);
        *status = find_nearest_ancestral_cell_pointer(*temp, ca_cell_pointer, princ_dir);
        if  (*status != 0) {
          DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
             "extend_trust_list: f_n_a_c_p Failed %ld ", *status));
          return;
        }
        
#ifdef DEBUG
        if (ca_cell_pointer) {
          DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
             "extend_trust_list: f_n_a_c_p succeded ca_cell_pointer=%s", 
              convert_x500name_to_printable_form(*ca_cell_pointer)));
          if (princ_dir) {
             DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
                "extend_trust_list: princ_dir=%s", princ_dir));
	  }
	  else {
             DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
                "extend_trust_list: princ_dir=NULL"));
	  }

        } else {

             DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
                "extend_trust_list: f_n_a_c_p did not find cellptr"));
        }
      
#endif

    } /* ca_cell_pointer && do_call_again */


    /* check if we will cross the cell boundary, at the next attempt
     */

    if (ca_cell_pointer && *ca_cell_pointer == *temp) 
      do_call_again = 1;
    else
     do_call_again = 0;



 
    if (ca_cell_pointer)  /* if we had a cell pointer at this point, use it to find the real cert dir */
      {
        /* compose_x500name creates a ca_cert_dir x500name from ca_cell_pointer,  princ_dir, temp */
        /* ca_cert_dir will be roughly equal to ca_cell_pointer/princ_dir/some rdn suffix of temp, */

        *status = compose_x500name(*ca_cell_pointer,  princ_dir, *temp, ca_cert_dir);
        if (*status != 0) {
          DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
                "extend_trust_list: compose_x500name failed %ld", *status));
          return;
        }
      } /* if !ca_cell_pointer  */
    else {
      ca_cert_dir = *temp;
    }
    
    add_certs_in_entry(partial, *temp, is_ca, ca_cert_dir, flag, depth, cadb_p,  status);
    if (*status != 0) {
       DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
             "extend_trust_list: add_certs_in_entry failed %ld", *status));
      return ;
    }





    temp1 = *temp;
    delete temp;   /* deallocate storage allocated in make_parent */
    *status = temp1.make_parent(&temp);
    if (*status != 0) {
       DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
             "extend_trust_list: make_parent failed %ld", *status));
      return ;
    }
  
  }  /* while we haven't reached root (temp->child_count() > 0) */
} /* end of extend_trust_list */


// ca_present_in_required_depth whether ca is "depth" up/down [if flag == up/down]
// of ca_other. If so, it returns 1; otherwise, returns 0; Note that if
// ca & other_ca don't share the ancestral relationship (i.e neither ca is
// ancestor of other_ca, nor other_ca the ancestor of ca) then this routine
// returns a 0;

static unsigned32 ca_present_in_required_depth(x500name  & ca,
					       x500name  & other_ca,
					       direction flag,
					       int depth,
					       int *n1_ret,
					       int *n2_ret)
{
  x500name temp;
  unsigned n1, n2, diff;
  char equal;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
        ">ca_present_in_required_depth"));
 
  if (compare_names (ca, other_ca, &equal, &n1, &n2) != 0) {

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
        "ca_present_in_required_depth: compare names failed"));

    return 0 ;  // ACTUALLY, this is an error of a different kind.. but for now, we return failure.
  }

  if (n1_ret)
    *n1_ret = n1;
  if (n2_ret)
    *n2_ret = n2;

  switch (flag) {
  case up:   
    // is ca "above" other_ca, and within depth ?? If so, n1 will be 0 && n2 >= 0
    // pictorially,  ca-->x-->y-->z...-->ca_other and #of links <=depth.
    if ((n1 == 0) && (n2 >= 0) && (n2 <= depth))
      return 1;
    return 0;
  case down:
    // is ca "below" other_ca, and within depth?? If so, n2 will be 0 && n1 >= 0
    // pictorially, ca_other-->x-->y-->z...-->ca and #of links <=depth.
    if ((n2 == 0) && (n1 >= 0) && (n1 <= depth))
      return 1;
    return 0;
  default:
    // error.
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
        "ca_present_in_required_depth: flag is neither up or down"));
    return 0;
  }

  // should not reach here...
  return -1;
}



// add_certs_in_entry
// lookup the ca entry for its certificates; if the certificate is signed by
// another ca (certifier)   who is within "depth" of ca being looked up depending
// upon direction (up/down), then 
// accept this certificate as a potential candidate for adding to the trust
// list;


static void add_certs_in_entry (pkc_trust_list_t *partial, 
			 x500name  & ca, 
                         int is_ca,   /* 1 if ca, 0 if user */
			 x500name  & ca_cert_dir, 
			 direction flag,
			 int depth,
                         struct ca_db_entry **cainfo_p,
			 unsigned32 *status)
{


  unsigned32 status1;



  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
        ">add_certs_in_entry"));

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
        ">add_certs_in_entry: ca = %s",convert_x500name_to_printable_form(ca)));
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
        ">add_certs_in_entry: ca_cert_dir = %s",convert_x500name_to_printable_form(ca_cert_dir)));
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
        ">add_certs_in_entry: is_ca = %ld, direction = %ld, & depth = %ld",
         is_ca, flag, depth));



  switch (flag) {

   // ************************************************
   // **************  UP ****************************
   // ************************************************
  case up:
    // first, read the ca_revoke list, and for each of the certificates listed,
    // call the appropriate pkc_certs api to remove any certificates that have
    // been revoked.

    *status = revoke_certs_if_any (ca_cert_dir, is_ca, partial);
    if (*status != 0) {

      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
            "add_certs_in_entry: revoke_certs_if_any failed %ld",*status));

      return;
    }

    // mbg: for now, we process both user revocation as well as 
    // ca revocation list regardless of whether we were called
    // for the target or an intermediate ca.
    *status = revoke_certs_if_any (ca_cert_dir, !is_ca, partial);
    if (*status != 0) {

      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
            "add_certs_in_entry: revoke_certs_if_any failed %ld",*status));

      return;
    }

    // process the certs in CA_CERT_OID. While establishing
    // trustbase, we only process certs in CA_CERT_OID &
    // don't process certs in USER_CERT_OID
    *status = process_ca_user_certs(partial, 
			 ca, 
                         is_ca,   /* 1 if ca, 0 if user */
			  ca_cert_dir, 
			 flag,
			 depth,
                         cainfo_p);
    if (*status != 0) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
            "add_certs_in_entry: process_ca_user_certs failed %ld",*status));
      return;
    }


  // second, read the certificates in the cross_cert_oid attr.
    *status = process_cross_certs(partial, 
			 ca, 
                         is_ca,   /* 1 if ca, 0 if user */
			  ca_cert_dir, 
			 flag,
			 depth,
                         cainfo_p);

    if (*status != 0) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
            "add_certs_in_entry: process_cross_certs failed %ld",*status));
      return;
    }

   break;

   // ************************************************
   // **************  DOWN ****************************
   // ************************************************
   case down: 

    // process the certs in CA_CERT_OID. While establishing
    // trustbase, we only process certs in CA_CERT_OID &
    // don't process certs in USER_CERT_OID
    // while going down, we need to process both ca_cert_oid
   // and user_cert_oid if is_ca == user.
    *status = process_ca_user_certs(partial, 
			 ca, 
                         is_ca,   /* 1 if ca, 0 if user */
			  ca_cert_dir, 
			 flag,
			 depth,
                         cainfo_p);
    if (*status != 0) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
            "add_certs_in_entry: process_ca_user_certs failed %ld",*status));
      return;
    }

    if (!is_ca) {  /* if ca was a user, process ca_cert_oid as well */
                   /* since for users, we need to do both */
     *status = process_ca_user_certs(partial, 
			 ca, 
                         !is_ca,
			  ca_cert_dir, 
			 flag,
			 depth,
                         cainfo_p);
      if (*status != 0) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
              "add_certs_in_entry: process_ca_user_certs failed %ld",*status));
        return;
      }
    }

  // second, read the certificates in the cross_cert_oid attr.
    *status = process_cross_certs(partial, 
			 ca, 
                         is_ca,   /* 1 if ca, 0 if user */
			  ca_cert_dir, 
			 flag,
			 depth,
                         cainfo_p);

    if (*status != 0) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
              "add_certs_in_entry: process_cross_certs failed %ld",*status));
      return;
    }

  // second, read the certificates in the cross_cert_oid attr.
    *status = process_cross_certs_in_db(partial, 
			 ca, 
                         is_ca,   /* 1 if ca, 0 if user */
			 flag,
			 depth,
                         cainfo_p);

    if (*status != 0) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
              "add_certs_in_entry: process_cross_certs_in_db failed %ld",*status));
      return;
    }

    *status = revoke_certs_if_any (ca_cert_dir, is_ca, partial);
    if (*status != 0) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
              "add_certs_in_entry: revoke_certs_if_any failed %ld",*status));
      return;
    }



    // mbg: for now, we process both user revocation as well as 
    // ca revocation list regardless of whether we were called
    // for the target or an intermediate ca.
    *status = revoke_certs_if_any (ca_cert_dir, !is_ca, partial);
    if (*status != 0) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
            "add_certs_in_entry: revoke_certs_if_any failed %ld",*status));
      return;
    }


    break;


    default:
     DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
           "add_certs_in_entry: direction is neither up or down, serious error"));
     return;
    }       /* end of switch */

  
 *status = 0;
 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
       ">add_certs_in_entry"));
  return ;


}  /* add_certs_in_entry */



// find_next_child_along_path
// start is guaranteed to be a ancestor of final; otherwise this procedure will
// not work. this procedure finds the child of start who is along the path
// between start and final.

void find_next_child_along_path(x500name & start,
                                const x500name & final,
                                unsigned32 *status) 
{
    unsigned start_rdn_count;
    unsigned final_rdn_count;
    long new_rdn_count;
    x500name * new_name = NULL;

    *status = pkc_s_internal_error;

    start_rdn_count = start.child_count();
    final_rdn_count = final.child_count();
    if (start_rdn_count > final_rdn_count) return;

    new_rdn_count = start_rdn_count+1;
    if (new_rdn_count <= 0) return;

    *status = final.make_ancestor(final_rdn_count - new_rdn_count,
                                  &new_name);
    start = *new_name;
    delete new_name;

}

// get_common_ancestor
// recusrively, get to the common ancestor of all nodes in the ca list,
// and the final target node.

void get_common_ancestor(x500name_list_t *&ca_list, 
			 const x500name &target, 
			 x500name &final_common_ancestor,
			 unsigned32 *status) 
{
  x500name temp1, temp2, *ancestor, *temp;
  int first_iteration;
  char equal;
  unsigned n1, n2;

 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
     ">get_common_ancestor "));

  ca_list->init_enumeration();
  ca_list->get_next(temp1);
  first_iteration = 1;
  while (ca_list->get_next(temp2)) {
    *status = compare_names(temp1,temp2,&equal, &n1,&n2);
    if (*status != 0) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
          "get_common_ancestor:compare_names failed!! "));
      return;
    }

    // common ancestor is n1 RDN's way from temp
    temp1.make_ancestor(n1, &ancestor);
    temp1 = *ancestor;
    delete ancestor;
  }

  *status = compare_names(temp1, target, &equal, &n1,&n2);
  if (*status != 0) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
        "get_common_ancestor:compare_names failed!! "));
    return;
  }
  temp1.make_ancestor(n1, &temp);
  final_common_ancestor = *temp;
  delete ancestor;
  delete temp;
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
     "<get_common_ancestor "));
  return;
}

// pkc_hierarchy_retrieve: retreives public key(s) of subjectName (target)
// given initial set of keys of ca's which can be trusted (trust)

unsigned32 pkc_hierarchy_retrieve(void ** context,
                                const void * trustbase,  
                                const x500name & subjectName,
                                const utc_t * date,
                                const uuid_t & desired_domain,
                                const pkc_generic_key_usage_t & desired_usage,
                                char initial_explicit_policy_required,
                                void ** keyinfo)
{
  unsigned32 status;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
     ">pkc_hierarchy_retrieve "));

  if (!trustbase || !context || !keyinfo) {
     DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
        "pkc_hierarchy_retrieve: Bad parameters "));
    return pkc_s_asn_bad_param;
  }

 

  status = pkc_hierarchy_complete_trust(context,
					trustbase,
					subjectName,
					date,
					desired_domain,
					desired_usage,
					initial_explicit_policy_required,
					keyinfo);

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
     "<pkc_hierarchy_retrieve "));
  return status;
}  /* end of retreive key */


// given an initial set of trusted public keys of ca's that someone trusts
// (in field in_trust in parameters), this function extends the trust
// list by using the heirarchical policy rules. This is part I of the HTP
// policy description. The complete out_trust trust list is the list of trusted
// ca's and their public keys as per the HTP policy. At a minimum, it
// will be equal to the in_trust.


// tbase will point to hierarchy_trustbase_t structure if everything works out 
// right in this routine.

unsigned32 pkc_hierarchy_establish(void **context,
				   const pkc_trust_list_t &in_trust,
				   const utc_t * date,
				   char initial_explicit_policy_required,
                                   void **tbase
				   )
{
  unsigned32 status;
  x500name_list_t *ca_initial_list = 0, *ca_temp_list = 0;
  x500name ca;
  x500name temp;
  pkc_trust_list_t *temp_trust_list = 0;
  struct ca_db_entry *cadb = 0;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
     ">pkc_hierarchy_establish "));

  if (!context || !tbase) {
     DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
        "pkc_hierarchy_establish: Bad parameters "));
    return pkc_s_asn_bad_param;
  }

  *context = 0;
  *tbase = 0;

  // copy in trust list.

  temp_trust_list = new pkc_trust_list_t;
  if (!temp_trust_list) { 
     DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
        "pkc_hierarchy_establish: Ran out of Memory"));
    return pkc_s_nomem;
  }

  status = pkc_copy_trustlist((const pkc_trust_list_t *) &in_trust, temp_trust_list);
  if (status != 0) {
     DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
        "pkc_hierarchy_establish: pkc_copy_trustlist failed %ld",status));
    return status;
  }


    
  // get a list of CA's (x500names only) who are to be trusted from the initial trust list
  // we cannot just go thru each entry in the trust list, because, a CA might be 
  // listed twice, as we may trust two or more public keys of a single CA. The
  // x500name_list_element_t class ptr that form_trusted_ca_list_from_trust_list
  // returns is ensured so that duplicate CA names are removed.
  ca_initial_list = form_trusted_ca_list_from_trust_list(in_trust, &status);
  if (status != 0) {
     DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
        "pkc_hierarchy_establish: f_t_c_l_f_t_l failed %ld",status));
    delete temp_trust_list;
    return status; 
  }

        
  // start enumeration of ca's
  ca_initial_list->init_enumeration();

   
  // while there are CA's to traverse..
  while (ca_initial_list->get_next(ca)) {
    // Add the certificates of this CA, and the certificates present
    // in CA's ancestors to the trust list

    // Note: tolerate_depth_doing_up is currently always 1. 
    extend_trust_list (temp_trust_list, ca, up, tolerate_depth_doing_up, &cadb, &status); 

    if (status != 0) {
       DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
          "pkc_hierarchy_establish: extend_trust_list failed %ld",status));
      delete ca_initial_list;
      delete temp_trust_list;
      free_ca_db(&cadb);
      return status;  
    }
  }

  *tbase = (struct hierarchy_trustbase *) 
     pkc_malloc(sizeof(struct hierarchy_trustbase));
  if (!(*tbase)) {
     DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_hierarchy_establish: Ran out of memory"));
    return pkc_s_nomem;
  }
  ((struct hierarchy_trustbase *) (*tbase))->tr =  temp_trust_list;  // copy extended trust list
  ((struct hierarchy_trustbase *) (*tbase))->crdb =  cadb;          // copy cross cert database lookedup
  delete ca_initial_list;  // don't need this anymore 

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
     "<pkc_hierarchy_establish"));

  return 0;
}  

// complete trust chain to subjectName.
// the trust list is assumed to have  been expaneded from a call to pkc_hierarchy_establish
// i.e., the sequence is pkc_hierarchy_establish, followed by pkc_hierarchy_complete_trust,
// where, the out_trust in pkc_hierarchy_establish is passed as input to 
// pkc_hierarchy_complete_trust (trust field).

unsigned32 pkc_hierarchy_complete_trust(
			void **context,
			const void *tbase,
			const x500name & subjectName,
			const utc_t * date,
                        const uuid_t & desired_domain,
			const pkc_generic_key_usage_t & desired_usage,
			char initial_explicit_policy_required,
                        void  **keyinfo
			)
  

{
  unsigned32 status, status1;
  x500name_list_t *ca_initial_list = 0;
  x500name ca, ca_cell_pointer, ca_cert_dir;
  x500name temp;
  char equal;
  unsigned32 flag, ca_cell_flag = 0;
  unsigned n1, n2;
  pkc_trust_list_t *partial_trust = 0, *trust = 0;  
  unsigned char *princ_dir = 0, *princ_dir1 = 0;
  int is_ca = 1;
  struct ca_db_entry *cainfo_p = 0;
  pkc_trusted_key_list_t *key_list = 0;
  size_t no_of_keys;
  struct hierarchy_keyinfo *rkeyinfo = 0;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
     ">pkc_hierarchy_complete_trust"));

  if (!context || !tbase) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
     "pkc_hierarchy_complete_trust Bad parameters"));
    return pkc_s_asn_bad_param;
  }

  *keyinfo = 0;
  partial_trust = ((struct hierarchy_trustbase *) tbase)->tr;
  cainfo_p = ((struct hierarchy_trustbase *) tbase)->crdb;
    
  trust = new pkc_trust_list_t;
  if (!trust) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
     "pkc_hierarchy_complete_trust Ran out of memory"));
    return pkc_s_nomem;
  }


  status = pkc_copy_trustlist(partial_trust, trust);
  if (status != 0) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
     "pkc_hierarchy_complete_trust: pkc_copy_trustlist failed %ld", status));
    goto end_func;
    // return status;
  }


  // get a list of CA's (x500names only) who are to be trusted from the initial trust list
  // we cannot just go thru each entry in the trust list, because, a CA might be 
  // listed twice, as we may trust two or more public keys of a single CA. The
  // x500name_list_element_t class ptr that form_trusted_ca_list_from_trust_list
  // returns is ensured so that duplicate CA names are removed.
  ca_initial_list = form_trusted_ca_list_from_trust_list(*trust, &status);
  if (status != 0) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
     "pkc_hierarchy_complete_trust: f_t_c_l_f_t_l failed %ld", status));
    goto end_func;
    // return status;
  }


   // temp is global root initially; so find the first child
   // along the path to subjectName
   find_next_child_along_path(temp, subjectName, &status);
   if (status != 0)
    {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_hierarchy_complete_trust: find_next_child_along_path failed %ld", status));
      goto end_func;
      // return status;
    }




  while (((status1 = compare_names(temp, subjectName, &equal, &n1, &n2)) == 0) && (equal == 0)) {


    // collect all certificates in temp;
    // try adding these provided they are signed by CA who is an ancestor of
    // of temp, and is within depth.
    

    status = is_ca_a_cell_pointer(temp, princ_dir1, flag);
    if (status != 0) {

      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_hierarchy_complete_trust: is_ca_a_cell_pointer failed %ld", status));

       goto end_func;
       // return status;
     }
     if (flag == 1) { /* yes, temp is a cell pointer */
         ca_cell_pointer = temp;
         if (princ_dir) 
            pkc_free (princ_dir);
         princ_dir = princ_dir1;
         ca_cell_flag = 1;
     }



   /* Note: once ca_cell_flag is set, it remains set */
     
    if (ca_cell_flag) {
        status = compose_x500name(ca_cell_pointer,  princ_dir, temp, ca_cert_dir);
        if (status != 0) {
          DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
           "pkc_hierarchy_complete_trust: compose_x500name failed %ld", status));

           goto end_func;
           // return status;
        }
      }
    else {
      ca_cert_dir = temp;
    }


    add_certs_in_entry (trust, temp, is_ca, ca_cert_dir, down, tolerate_depth_going_down,    &cainfo_p, &status);
    if (status != 0) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_hierarchy_complete_trust: add_certs_in_entry failed %ld", status));

      goto end_func;
      // return status;
    }

   find_next_child_along_path(temp, subjectName, &status);
   if (status != 0)
    {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_hierarchy_complete_trust: find_next_child_along_path failed %ld", status));
      goto end_func;
      // return status;
    }

  }  /* while we haven't reached subject */




  if ((status1 != 0) || (equal == 0)) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_hierarchy_complete_trust: possibly compare_names failed %ld", status1));
    status = status1;
    goto end_func;
    // return status1;
  }


  // now we do processing for temp == subjectName
  // first check if temp is a cell pointer; if so, get
  // the new princ_dir
  status = is_ca_a_cell_pointer(temp, princ_dir1, flag);
  if (status != 0) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_hierarchy_complete_trust: is_ca_call_pointer  failed %ld", status));

     goto end_func;
     // return status;
   }
     if (flag == 1) { /* yes, temp is a cell pointer */
         ca_cell_pointer = temp;
         if (princ_dir) 
            pkc_free (princ_dir);
         princ_dir = princ_dir1;
         ca_cell_flag = 1;
     }

  // so now, temp == subjectName.
  if (ca_cell_flag) {
    status = compose_x500name(ca_cell_pointer,  princ_dir, temp, ca_cert_dir);
    if (status != 0) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_hierarchy_complete_trust: compose_x500name  failed %ld", status));
      goto end_func; 
      // return status;
    }
  } else {
    ca_cert_dir = temp;
  }

   is_ca = 0;  /* add for user target */
   add_certs_in_entry (trust, temp, is_ca, ca_cert_dir, down, tolerate_depth_going_down,  &cainfo_p, &status);
   if (status != 0) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_hierarchy_complete_trust: add_certs_in_entry  failed %ld", status));
     goto end_func;
     // return status;  
   }

   {
     utc_t temp_date;
     temp_date = *date;
     const pkc_generic_key_usage_t temp_usages; 
     int i;

     status = pkc_lookup_keys_in_trustlist
       (trust, key_list, &no_of_keys, subjectName, &temp_date, &desired_usage);

     

     if (status != 0) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "pkc_hierarchy_complete_trust: pkc_lookup_keys_in_trustlist  failed %ld", status));
      goto end_func;
      // return status;
     }

     rkeyinfo = (struct hierarchy_keyinfo *) pkc_malloc
                      (sizeof (struct hierarchy_keyinfo));
     if (!(rkeyinfo)) {
        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
         "pkc_hierarchy_complete_trust: Ran out of mem"));
       status = pkc_s_nomem;
     }   
     
     status = 0;
     (rkeyinfo)->tr = trust;
     (rkeyinfo)->keylist = key_list;
     (rkeyinfo)->no_of_keys = no_of_keys;
     *keyinfo = (void *) rkeyinfo; 
     

   }

end_func:

   if (ca_initial_list)
     delete ca_initial_list;
   if (princ_dir)
     pkc_free (princ_dir);
   if (status) {
     if (trust)
       delete trust;
     if (key_list)
       delete key_list;
   }

   DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
    "<pkc_hierarchy_complete_trust"));
    return status;


  
}

//
// pkc_hierarchy_get_key_count
//

unsigned32 pkc_hierarchy_get_key_count
           (void **context, void *keyinfo, size_t *keycount) 
{

   DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
    ">pkc_hierarchy_get_key_count"));


  if (!keycount) 
    return pkc_s_asn_bad_param;

  if (!keyinfo)
    *keycount = 0;
  else
    *keycount = ((struct hierarchy_keyinfo *) keyinfo)->no_of_keys;

   DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
    "<pkc_hierarchy_get_key_count"));

  return 0;
}
  
//
// pkc_hierarchy_get_key_data
// if we don't find the key, we return 0 in key_data and 0 in key_length
//
unsigned32 pkc_hierarchy_get_key_data
    (void ** context,
     void * keyinfo,
     unsigned key_index,unsigned char ** key_data,
     size_t * key_length) 
{
  size_t keycount;
  pkc_trusted_key_list_element_t *temp = 0;
  int i = 0;
  buffer_t buf;
  unsigned32 status;

   DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
    ">pkc_hierarchy_get_key_data"));

  
  if (!key_data || !key_length) 
    return pkc_s_asn_bad_param;
  
  *key_data = 0;
  *key_length = 0;
  
  if (!keyinfo)
    return 0;

  if (!(((struct hierarchy_keyinfo *) keyinfo)->keylist))
    return pkc_s_internal_error;

  keycount = ((struct hierarchy_keyinfo *) keyinfo)->no_of_keys;
  if (key_index > keycount-1) 
    return pkc_s_index_out_of_range;
    

  temp = (((struct hierarchy_keyinfo *) keyinfo)->keylist)->first;

  if (!temp)
    return 0;

  for (i = 0; i < keycount; i++) {
    if (i == key_index) {

    // temp better be NON-NULL, otherwise keylist is corrupt.
      if (!temp) {
         DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
          "pkc_hierarchy_get_key_data: ** ERROR ** keylist is corrupt"));
         return pkc_s_internal_error;
      } else {
        status  = temp->key->value.write(buf);
        if (status != 0) {
           DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
            "pkc_hierarchy_get_key_data: write of key failed %ld", status));
          return (status);
        }

       *key_data = (unsigned char *) pkc_malloc (buf.data_length);
       if (!(*key_data)) {
           DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
            "pkc_hierarchy_get_key_data: Ran out of memory"));
         return pkc_s_nomem;
       }

       memcpy(*key_data, buf.data , buf.data_length);
       *key_length = buf.data_length;
       return 0;
     }  /* if !temp */
   }  /* if i == key_index */


   if (temp) {
     temp = temp->next;
   } else {

         DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
          "pkc_hierarchy_get_key_data: ** ERROR ** keylist is corrupt"));

      return pkc_s_internal_error;
   }
  } /* for loop */

 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
  "<pkc_hierarchy_get_key_data"));

  return 0;

} /* end of  pkc_hierarchy_get_key_data */

//
// pkc_hierarchy_delete_key_info
//

unsigned32 pkc_hierarchy_delete_key_info
       (void **context, void **key_handle)
{

  struct hierarchy_keyinfo *keyinfo = 0;

 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
  ">pkc_hierarchy_delete_key_info"));

  if (!key_handle)
    return 0;

  keyinfo = (struct hierarchy_keyinfo *) *key_handle;
  if (!keyinfo)
    return 0;
   
  if (keyinfo->tr)
    delete keyinfo->tr;

  if (keyinfo->keylist)
    delete keyinfo->keylist;

  pkc_free (keyinfo);
  *key_handle = 0;

 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
  "<pkc_hierarchy_delete_key_info"));

  return 0;
}
  

/* 
 * mbg: functions to retrieve certificates stored in xds. Note that we get these
 * as sequence of bytes from x500; we need to run these thru some other 
 * classes to make any sense out of these --- such as classes Certificate, etc
 */




// construct_certificate: construct a certificate class object given the
// asn1 encoded certificate; pretty straightforward.
static unsigned32 construct_certificate(unsigned char *asn1_cert_p,
				   unsigned32 length,
                                   Certificate & cert)
{
  unsigned32 status = 0;

 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
  ">construct_certificate"));

  /* returns a pointer to bytes that represent a ASN1 encoded certificate. Allocation
   * is done by this routine
   */

  r_buffer_t from_asn;  // instance of buffer class
  int buf_len = length;          // length of buffer pointed to by buf.


  from_asn.data = asn1_cert_p;
  from_asn.data_length = buf_len;



  status = cert.read (from_asn);

 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
  "<construct_certificate: cert read status = %ld", status));

  return status;
}

static unsigned32 construct_cross_certificate(
  unsigned char *asn1_cert_p,
  size_t length, int &cert1_present, 
  unsigned char ** asn1_cert1_p, size_t &asn1_length,
  Certificate & cert1, int &cert2_present,
  unsigned char ** asn1_cert2_p, size_t &asn2_length,Certificate & cert2)
{
  CrossCertificatePair crpair;
  unsigned32 status = 0;
  buffer_t temp_buffer;
  r_buffer_t from_asn;  // instance of buffer class
  int buf_len = length;          // length of buffer pointed to by buf.

  cert1_present = 0;
  cert2_present = 0;
  *asn1_cert1_p = 0;
  *asn1_cert2_p = 0;
  asn1_length = 0;
  asn2_length = 0;
 /* returns a pointer to bytes that represent a ASN1 encoded cross certificate.  * is done by this routine
   */


 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
  ">construct_cross_certificate"));

  from_asn.data = (unsigned char *) asn1_cert_p;
  from_asn.data_length = buf_len;



  status = crpair.read (from_asn);

  if (crpair.forward.is_present()) {
    cert1.copy(crpair.forward.cert);
    cert1_present = 1;
    temp_buffer.clear();
    status = cert1.write(temp_buffer);
    if (status) return status;
    *asn1_cert1_p = temp_buffer.data;
    asn1_length = temp_buffer.data_length;
    temp_buffer.detach_buffer();            
  }
    
  if (crpair.reverse.is_present()) {
    cert2_present = 1;
    cert2.copy(crpair.reverse.cert);
    temp_buffer.clear();
    status = cert2.write(temp_buffer);
    if (status) return status;
    *asn1_cert2_p = temp_buffer.data;
    asn2_length = temp_buffer.data_length;
    temp_buffer.detach_buffer();            

  }

 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
  "<construct_cross_certificate"));

  return status;



}






// read_xds_certificate_attribute will read the certificates stored in 
// the x500name entry, ca, and return a xds_read_result_t pointer;
// we then need to go thru this result ptr, to extract individual certificates.
static xds_read_result_t *read_xds_certificate_attribute
   (const x500name & ca, 
    int is_ca, 
    unsigned32 *status)
{
  xds_status_t *internal_status = 0;
  xds_selection_t criteria;
  xds_t xds_object;
  xds_read_result_t *result = 0;


 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
  ">read_xds_certificate_attribute"));


  /* however, by default, criteria will have DS_ALL_ATTRIBUTES set to true. since we want only
   * types and values for pkc_cert_attribute, we say so.. 
   */
  criteria.select_specified_attributes();

  /* set the selection criteria to the object id for certificate string attribute */
  if (is_ca)
    criteria.select_attribute(ca_cert_attribute);
  else
    criteria.select_attribute(user_cert_attribute);

  criteria.return_types_and_values();

  internal_status = xds_object.read(ca, criteria, result);


  if (internal_status == 0) {  /* no status... really a problem */
    *status = 1; /* really, should not happen, but even if it does,
                  * caller just assumes attribute does not exist.
                  */
    return 0;
  }

  if (internal_status->isError()) {
    if (internal_status->error_code) {
      *status = (unsigned32) internal_status->error_code;
    } else {
      if (internal_status->error_class) {
        *status = (unsigned32) internal_status->error_class;
      } else  {
      *status = (unsigned32) 1; /* really, should not happen, but even if it does,
                                 * caller just assumes attribute does not exist.
                                 */ 
      }
    }
   
    delete internal_status;  /* we should be doing this, right */
    result = 0;
  } else {
    *status = 0;
  }


 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
  "<read_xds_certificate_attribute: status = %ld", *status));
  return (result);
}

// read_xds_cross_certificate_attribute will read the certificates stored in 
// the x500name entry, ca, and return a xds_read_result_t pointer;
// we then need to go thru this result ptr, to extract individual certificates.
static xds_read_result_t *read_xds_cross_certificate_attribute
   (const x500name & ca, 
    int is_ca, 
    unsigned32 *status)
{
  xds_status_t *internal_status;
  xds_selection_t criteria;
  xds_t xds_object;
  xds_read_result_t *result;


   DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
    ">read_xds_cross_certificate_attribute"));

  /* however, by default, criteria will have DS_ALL_ATTRIBUTES set to true. since we want only
   * types and values for pkc_cert_attribute, we say so.. 
   */

  criteria.select_specified_attributes();

  /* set the selection criteria to the object id for certificate string attribute */
  criteria.select_attribute(cross_cert_attribute);

  criteria.return_types_and_values();

  internal_status = xds_object.read(ca, criteria, result);


  if (internal_status->isError()) {
    if (internal_status->error_code) {
      *status = (unsigned32) internal_status->error_code;
    } else {
      if (internal_status->error_class) {
        *status = (unsigned32) internal_status->error_class;
      } else  {
      *status = (unsigned32) 1; /* really, should not happen, but even if it does,
                                 * caller just assumes attribute does not exist.
                                 */
      }
    }

    delete internal_status; 
    result = 0;
  } else {
    *status = 0;
  }

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
   "<read_xds_cross_certificate_attribute: status = %ld", *status));

  return (result);
}      
    

// find_closest_ancestral_cell_pointer: return value 0 ==> call went thru ok.
//                                                   1 ==> call failed
// args: ca ==> starting x500name , input
//       ca_closest_cell_ptr, out param, points to closest cell pointer
//       princ_dir ==> principal_directory_attribute value, output. This is the value of
//                     attribute "principal_directory_attribute" in the closet cell pointer
//                     above ca.

unsigned32 find_nearest_ancestral_cell_pointer(const x500name & ca, 
                                               x500name * & ca_closest_cell_ptr,
                                               unsigned char * & princ_dir) 
{
  unsigned32 status;
  x500name *temp, temp1;
  x500name root;
  unsigned32 flag;
  RelativeDistinguishedName *rdn;
  OM_object_identifier v;
  int found;
  int i;

 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
  ">find_nearest_ancestral_cell_pointer"));

  ca_closest_cell_ptr = 0;
  princ_dir = 0;

  if (ca.child_count() == 0) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
     "find_nearest_ancestral_cell_pointer: was given xds root"));
    return 0;
  }

  status = rdn_oid_is_typeless(ca[ca.child_count()-1]);
  if (status == 0) { /* rdn is not typeless */
    ca_closest_cell_ptr = new x500name();
    *ca_closest_cell_ptr = ca;
    return 0;
  }

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
   "find_nearest_ancestral_cell_pointer: ca = %s",
    convert_x500name_to_printable_form(ca)));

  status = is_ca_a_cell_pointer( ca, princ_dir, flag);
  if (status != 0) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
     "find_nearest_ancestral_cell_pointer: is_ca_a_cell_pointer failed %ld",
     status));

   return status;  /* problem.. */
  }

  if (flag == 1)  /* yes, ca is a cell pointer */
    {
      ca_closest_cell_ptr = new x500name();
      *ca_closest_cell_ptr = ca;

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
     "find_nearest_ancestral_cell_pointer succeded"));

      return 0;
    }


 
  status = ca.make_parent (&temp);
  if (status != 0) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
     "find_nearest_ancestral_cell_pointer: make_parent failed %ld",
     status));
    return status;
  }


  while (temp->child_count() > 0) {  /* while we haven't reached root.. */

    status = is_ca_a_cell_pointer( *temp, princ_dir, flag);
    if (status != 0) {
     DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
      "find_nearest_ancestral_cell_pointer: is_ca_a_cell_pointer failed %ld",
      status));

      return status;  /* problem.. */
    }

    if (flag == 1)  /* yes, ca is a cell pointer */
      {
        ca_closest_cell_ptr = temp;

        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
         "find_nearest_ancestral_cell_pointer succeded"));


        return 0;
      }
        
    temp1 = *temp;
    delete temp;   /* deallocate storage allocated in make_parent */
    status = temp1.make_parent(&temp);
    if (status != 0) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
       "find_nearest_ancestral_cell_pointer: make_parent failed %ld",
       status));
      return status;
    }

  }
  delete temp;

 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
  "<find_nearest_ancestral_cell_pointer"));

  return 0;

}

// returns 1, if ca has a cell pointer
// returns 0, in case of an error, or in case ca is a not cell pointer.
// we don't bother returning all the xds error codes here, since it isn't a
// a critical failure at this point.
// ret value == flag for now; later on, if we want to change the
// ret value to be an xds error, we can do so without changing signature.
 
unsigned32 does_ca_have_a_cell_pointer(const x500name & ca, unsigned32 & flag)
{

  xds_status_t *internal_status = 0;
  xds_selection_t criteria;
  xds_t xds_object;
  xds_read_result_t *result = 0;
  const xds_attribute_t *attr_result = 0;
  unsigned32 status = 0, status1;
 
 DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
  ">does_ca_have_a_cell_pointer"));

  flag = 0;  // init flag to not found

  status = rdn_oid_is_typeless(ca[ca.child_count()-1]);
  if (status == 0) { /* oid is not typeless */
    return 0;
  }

  /* however, by default, criteria will have DS_ALL_ATTRIBUTES set to true. since we want only
   * types and values for pkc_cert_attribute, we say so.. 
   */

  criteria.select_specified_attributes();

  /* set the selection criteria to the object id for certificate string attribute */
  criteria.select_attribute(principal_directory_attribute);

  /* by default, when we init criteria, the constructor allows us to read both attribute types
   * and values; since we are interested only in both, types and values, we don't do anything special
   * default, when we init criteria, the constructor allows us to read both attribute types
   * and values; since we are interested only in both, types and values, we don't do anything special
   * for that.
   */

  criteria.select_specified_attributes();
  criteria.return_types_and_values();

  internal_status = xds_object.read(ca, criteria, result);
  if (!internal_status)  /* problem reading object.. should not happen. e
                          * errors here are not critical, and hence we return 0
                          */
    {

       DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
        "does_ca_have_a_cell_pointer xds object read failed"));

      return 0;  /* error */
    }

  if (internal_status->isError() || result == 0)
    {
      status = 0;
      goto end_func1;      
    }


  status = result->get_first_attribute(attr_result);
  if (status != 0)  /* cell_pointer_attribute does not exist in the ca entry */
    {
       DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
        "does_ca_have_a_cell_pointer get_first_attribute failed %ld", internal_status->error_code));
      status = 0;  /* failure, since no attribute of interest */
      goto end_func1;      
    }
  else {
       DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
        "does_ca_have_a_cell_pointer succeded"));
    flag = 1;  /* we did see the cell_pointer_attribute; so now, lets find whether it has princ_dir */
    status = 1;  /* success */
  }

end_func1:

  if (internal_status) delete internal_status;
  if (result) delete result;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
    "<does_ca_have_a_cell_pointer succeded"));

  return status;  /* we are done, return status  */
}

// return_princ_dir: if princ_dir attr is set, returns princ_dir in ca;
// otherwise returns 0 in princ_dir, and a possible error status
// for now, it always returns 0 even if there is an error. in this case
// princdir is set to 0.

unsigned32 return_princ_dir(const x500name & ca,  unsigned char * & princ_dir) 
{

  xds_status_t *internal_status = 0;
  xds_selection_t criteria;
  xds_t xds_object;
  xds_read_result_t *result = 0;
  const xds_attribute_t *attr_result = 0;
  unsigned32 status = 0, status1;
  const xds_value_t *value_result = 0;
  size_t length = 0;
  unsigned char *temp = 0;
 
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
    ">return_princ_dir"));


  attr_result = 0;
  internal_status = 0;
  princ_dir = 0;

  /* now we look for principal directory attribute; 
   */ 

  /* set the selection criteria to the object id for certificate string attribute */
  criteria.select_specified_attributes();
  criteria.select_attribute(principal_directory_attribute);
  criteria.return_types_and_values();

  internal_status = xds_object.read(ca, criteria, result);
  if (!internal_status)
    {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
        "return_princ_dir: ca = %s error doing read for princdir, returning NULL",
        convert_x500name_to_printable_form(ca)));
      return 0;  /* error, but returning success as if princ_dir does not exist */ 
    }

  if (internal_status->isError() || result == 0)  /* principal_directory_attribute does not exist? */
    {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
        "return_princ_dir: ca = %s error doing read for princdir, returning NULL",
        convert_x500name_to_printable_form(ca)));
      status = 0;
      goto end_func2;      
    }

  /* from now on, status returned will always be 0 */
  status = 0;

  status1 = result->get_first_attribute(attr_result);
  if (status1!= 0 || attr_result == 0)  /* principal_directory_attribute does not exist in the ca entry */
    {
      goto end_func2;
    }

  status1 = attr_result->get_first_value(value_result); //get the first value
  if (status1 != 0 || value_result == 0) 
    {
      goto end_func2;
    }

  status1 = value_result->get_value(temp, length);  

end_func2:
  if (internal_status) delete internal_status;
  if (result) delete result;

  if (temp && length > 0) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
      "return_princ_dir: ca = %s  princdir = %s",
      convert_x500name_to_printable_form(ca), temp));

    princ_dir = (unsigned char *) pkc_malloc (length +1);
    if (!princ_dir) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
        "return_princ_dir: Ran out of memory"));
      status = pkc_s_nomem;
    }
    memcpy(princ_dir, temp, length);
    *(princ_dir+length) = '\0';
    free (temp);
  }

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
    "<return_princ_dir"));
   
  return status;
}


// is_ca_a_cell_pointer: given ca, finds whether or not this ca is really
// a pointer to a hierarchical cell; this is done by checkking for attribute
// CDS_ParentCellPointers (OID 1.3.22.1.3.54) attribute (see /opt/dcelocal/etc/cds_attributes file)
// If CDS_ParentCellPointers exists, then the ca entry is a cell pointer, otherwise, it is
// a normal entry withing a cell. 
// 
// If ca is a cell pointer, then the function
// returns 1; otherwise, it returns 0. 
//
// If ca is not a cell pointer, princ_dir does not have any meaningul value.
//
// If ca is a cell pointer, we return the 
// the "Principal_Directory" attribute value in princ_dir; If
// this attribute does not exist, princ_dir is set to null.
 
unsigned32 is_ca_a_cell_pointer(const x500name & ca, unsigned char * & princ_dir, unsigned32 & flag) 
{
 unsigned32 status;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
    ">is_ca_a_cell_pointer"));

 princ_dir = 0;
 status = does_ca_have_a_cell_pointer(ca, flag);
 if (status == 0) {  /* ca is not a cell pointer, so no point trying to lookup princ_dir */

   return status;
 }


 if (flag == 1) {
   status = return_princ_dir(ca,  princ_dir) ; // status returned by return_princ_dir is always 0 for now.
 }

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
    "<is_ca_a_cell_pointer"));

 return status;
}





/* mbg: compose_x500name: Note: ca_cell_pointer is guaranteed to be an ancestor
 * of ca; princ_dir may be null.
 */

unsigned32 compose_x500name(x500name & ca_cell_pointer, 
                            unsigned char *& princ_dir, 
                            x500name & ca, x500name & ca_cert_dir)
{
  int i;
  unsigned32 status;
  RelativeDistinguishedName *rdn = 0;
  x500name newx500;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
    ">compose_x500name"));

  if (!princ_dir)  {  /* princ_dir is null, return ca */
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
      "compose_x500name: No princ dir, so returning ca_cert_dir as ca"));
    ca_cert_dir = ca;
    return 0;
  }

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
    "compose_x500name: ca_cell_pointer = %s",
    convert_x500name_to_printable_form(ca_cell_pointer)));
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
    "compose_x500name: princ_dir = %s",
    princ_dir));
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
    "compose_x500name: ca = %s",
    convert_x500name_to_printable_form(ca)));




  newx500 = ca_cell_pointer;

  rdn = newx500.new_child();
  if (!rdn) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
      "compose_x500name: new_child failed"));
    return pkc_s_nomem;
  }

  status  = rdn->set_value((const char *) princ_dir);
  if (status != 0) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
      "compose_x500name: set_value failed %ld", status));
    return status;
  }


  /* append the suffix */
  for (i = ca_cell_pointer.child_count(); i < ca.child_count(); i++)
    {
          rdn =  newx500.new_child();
          if (!rdn) {
             DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
               "compose_x500name: new_child failed"));
            return status;
          }


          status  = rdn->copy(*(ca[i]));
          if (status != 0) {
             DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
               "compose_x500name: rdn copy  failed %ld", status));
            return status;
          }
    }



  ca_cert_dir = newx500;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
    ">compose_x500name: ca_cert_dir = %s",
    convert_x500name_to_printable_form(ca_cert_dir)));

  return 0;
      
}   

// rdn_oid_is_typeless: returns 1 is rdn oid is typeeless.
// otherwise, returns 0.

unsigned32 rdn_oid_is_typeless (RelativeDistinguishedName *rdn)       
{
  gss_OID v = 0;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
    ">rdn_oid_is_typeless"));

  ((*rdn)[0])->AttributeType.get_value (v);
  if (v) {
    if ((memcmp (v->elements,
                OMP_D_DSX_TYPELESS_RDN,
                OMP_LENGTH(DSX_TYPELESS_RDN)) == 0)  &&
                (v->length == OMP_LENGTH(DSX_TYPELESS_RDN)))
      {
         return 1; 
      }
   } else {
     return 0;
  }


  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
    "<rdn_oid_is_typeless"));

  return 0; 
}


// note: this is not a thread safe debug function 
// this namebuf is static


#define MAX_X500_NAME 5000

static unsigned char *convert_x500name_to_printable_form(const x500name &ca)
{
  static unsigned char  namebuf[MAX_X500_NAME];
  unsigned32 status;

  buffer_t buf;

  strcpy((char *) &namebuf[0], " *** ERROR in convert_x500name_to_printable_form **");
  status  = ca.display(buf); 
  if (status == 0) {
    memcpy (&namebuf[0], buf.data, buf.data_length);
    namebuf[buf.data_length] = '\0'; 
  }

  return ((unsigned char *)  &namebuf[0]);
}






static unsigned32 convert_x500name_to_string(x500name & ca, unsigned char *&namebuf)
{
  buffer_t buf;
  unsigned32 status;
  namebuf = 0;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
    ">convert_x500name_to_string"));

  status = ca.display(buf);
  if (status != 0) {
    return status;
  }

  namebuf = (unsigned char *) pkc_malloc(buf.data_length+1);
  if (!namebuf) 
    return pkc_s_nomem;

  memcpy(namebuf,buf.data, buf.data_length);
  *(namebuf+buf.data_length) = '\0';

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
    "<convert_x500name_to_string"));
  return 0;
 
}


static unsigned32 determine_relation 
    (const x500name &ca1, const x500name &ca2,
     int &ca1_ancestor_ca2,
     int &ca1_descendant_ca2,
     int &ca1_equal_ca2,
     int &ca1_sibling_ca2,
     int &depth_difference) 
{
  unsigned32 status;
  unsigned n1, n2;
  char equal;


 ca1_equal_ca2 = 0;
 ca1_ancestor_ca2 = 0;
 ca1_descendant_ca2 = 0;
 ca1_sibling_ca2 = 0;
 depth_difference = 0;

 status = compare_names(ca1, ca2, &equal, &n1, &n2);
 if (status != 0) {
   return status;   /* should not happen, since compare_names alwys returns
                     * 0 
                     */
 }
 if (equal) {   /* equal */
   ca1_equal_ca2 = 1;
   ca1_ancestor_ca2 = 0;
   ca1_descendant_ca2 = 0;
   ca1_sibling_ca2 = 0;
   depth_difference = 0;
   return 0;
 }

 /* first check cross-link or sibling */
 if (n1 > 0 && n2 > 0) {
   ca1_equal_ca2 = 0;
   ca1_ancestor_ca2 = 0;
   ca1_descendant_ca2 = 0;
   ca1_sibling_ca2 = 1;
   depth_difference = 0;
   return 0;
 }

  if (n1 == 0  && n2 > 0) {  /* ca1 is ancestor of ca2 */
   ca1_equal_ca2 = 0;
   ca1_ancestor_ca2 = 1;
   ca1_descendant_ca2 = 0;
   ca1_sibling_ca2 = 0;
   depth_difference = n2;
   return 0;
 }
  if (n1 > 0  && n2 == 0) {  /* ca1 is descendant of ca2 */
   ca1_equal_ca2 = 0;
   ca1_ancestor_ca2 = 0;
   ca1_descendant_ca2 = 1;
   ca1_sibling_ca2 = 0;
   depth_difference = n1;
   return 0;
 }

  /* We should never come here */

 return 0; // To keepo the compiler happy
}   


// process the certs in CA_CERT_OID or USer_CERT_OID. 
// if is_ca == 1, process CA_CERT_OID, otherwise, process
// USER_CERT_OID
// While establishing
// trustbase, we only process certs in CA_CERT_OID &
// don't process certs in USER_CERT_OID
// When completing trustbase, we process both ca_cert_oid
// and user_cert_oid for target, and only ca_cert_oid
// for intermediate ca's. This routine thus can be called
// for establishing/compelting trustchains. 
unsigned32 process_ca_user_certs(pkc_trust_list_t *partial, 
			 x500name  & ca, 
                         int is_ca,
			 x500name &ca_cert_dir,
			 direction flag,
			 int depth,
                         struct ca_db_entry **cainfo_p)
{
  unsigned32 status, status1;
  x500name cert_issuer, cert_subject;
  int  is_ancestor,  is_descendant, is_equal, 
     is_sibling, depth_difference;
  Certificate cert;
  const xds_attribute_t *attr_result = 0;
  xds_read_result_t *result = 0;
  unsigned char *asn1_cert_p = 0; size_t length = 0;
  const xds_value_t *value_result = 0;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
    ">process_ca_user_certs"));

  // first, read the x500 certificate attribute values 
  // there should only be one attribute, with possibly multiple values returned
  // where the attribute identifies this to be a certificate attribute, and the
  // individual values correspond to the one or more certificates.
  result = read_xds_certificate_attribute(ca_cert_dir, is_ca, &status);
  if (status != 0) {
    return 0; 
  }

  status = result->get_first_attribute(attr_result); // we only have one attribute.
  if (status != 0 || attr_result == 0) {
    delete result;
    return 0;
  }

  status = attr_result->get_first_value(value_result); //get the first value
  if (status != 0 || value_result == 0) {
    delete result;
    return 0; 
  }


  status = value_result->get_value(asn1_cert_p, length);  // get the asn1 encoding in value
  if (status != 0 || length == 0 || asn1_cert_p == 0) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
      "process_ca_user_certs get value failed %ld", status));
    delete result;
    return 0;  // actually a problem, in that, there is value, but zero bytes in value.
  }
  

  while (1) {

    status = construct_certificate(asn1_cert_p,length, cert);
    if (status != 0)  { // problem creating certificate
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
        "process_ca_user_certs construct_certificate failed %ld", status));
      status = 0;
      goto next_cert;
    }
    cert_issuer = cert.certificateInfo.issuer;
    cert_subject = cert.certificateInfo.subject;

    status = determine_relation(cert_issuer, cert_subject, is_ancestor,
                          is_descendant, is_equal,
                          is_sibling, depth_difference);
    if (status != 0) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
        "process_ca_user_certs determine_relation failed %ld", status));
     if (asn1_cert_p)  
       pkc_free (asn1_cert_p);
     delete result;
     return status;
    }

    // check if ca is present in the required depth in the list of ca's so far..
    // if not, skip adding this certificate.

    // this is slightly more complicated.. If direction is up, then
    // we are willing to tolerate certs that are issued by cert_issuer who
    // is a child of ca, and within depth.
    //
    // if direction is down, we are willing to tolerate certs issued by
    // cert_issuer who is an ancestor of ca, and within depth. however,
    // in this case, we are also willing to tolerate certs that are
    // issued by cert_issuer who is a cross-link of ca. This is because,
    // to complete trust chain while coming down, we are allowed to take
    // one cross link.

    switch (flag) {

    case up:

      // check if this certificate belongs here. 
      // for this to work, we tolerate the certificate only if
      // issuer == ca, since it better be either an up certificate
     // or a cross certificate signed by the ca.

     if (ca == cert_issuer) {
       // process this certificate
       // we only process  UP certificates and cross certificates. 
      // first check for up certificate
      if (is_descendant && depth_difference <= depth) {

	status1  = pkc_check_cert_against_trustlist (partial, 
                  &cert, 1);

        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
          "process_ca_user_certs pkc_check_cert_against_trustlist returned %ld", status1));


      }

      if (is_sibling)  {  /* cross certificate */
	unsigned char *princ = 0;        
	status = convert_x500name_to_string(cert_subject, princ);
        if (status != 0) {
          if (asn1_cert_p)  
            pkc_free (asn1_cert_p);
          delete result;
          return status;
       }
       status = add_ca_and_cert_to_crdb((char *)princ, asn1_cert_p, length, cainfo_p);
       if (status != 0) {
          if (asn1_cert_p)  
            pkc_free (asn1_cert_p);
          delete result;
          return status;
       }
       pkc_free (princ);
    }
   }  /* if ca == cert_issuer */

   break;

    case down:

      // check if this certificate belongs here. 
      // for this to work, we tolerate the certificate only if
      // subject == ca, since it better be either an DOWN certificate
     // or a cross certificate signed for the ca by some sibling

     if (ca == cert_subject) {
       // process this certificate
       // we only process  DOWN certificates and cross certificates. 
      // in both cases, we try to add the certificate
      if ((is_ancestor && depth_difference <= depth) ||
           (is_sibling)) {

	status1  = pkc_check_cert_against_trustlist (partial,
                       &cert, 1);

        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
          "process_ca_user_certs pkc_check_cert_against_trustlist returned %ld", status1));

      }

   }  /* if ca == cert_subject */
   break;

   default:  /* should never come here */
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
        "process_ca_user_certs pkc_check_cert_against_trustlist ** ERROR **"));
   delete result;
   return (-1);
  } /* end of switch statement */




next_cert:
    if (asn1_cert_p)  
       pkc_free (asn1_cert_p);


    asn1_cert_p = 0;
    length = 0;

    status = attr_result->get_next_value(value_result); //get the next value value
    if (status != 0 || value_result == 0) {   // end of certificates..
      // should check for value and make sure it indicates last value.. but for now, this is ok.
      // this is really a normal return from this procedure.

      delete result;
      return 0;
    }

    status = value_result->get_value(asn1_cert_p, length);  // get the asn1 encoding in value
    if (status != 0 || length == 0 || asn1_cert_p == 0) {
      delete result;
      return 0;
    }

  } /* while (1) */

  /* should never come here */
   DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
     "process_ca_user_certs pkc_check_cert_against_trustlist ** ERROR **"));
  return (-1);
}  /* end of process_ca_user_certs */




















// for establishing/compelting trustchains. 
unsigned32 process_cross_certs(pkc_trust_list_t *partial, 
			 x500name  & ca, 
                         int is_ca,
			 x500name &ca_cert_dir,
			 direction flag,
			 int depth,
                         struct ca_db_entry **cainfo_p)
{
  unsigned32 status, status1, status2;
  Certificate cert1, cert2;
  const xds_attribute_t *attr_result = 0;
  xds_read_result_t *result = 0;
  const xds_value_t *value_result = 0;

  unsigned char *asn1_cert1_p = 0, *asn1_cert2_p = 0;
  unsigned char  *asn1_cross_cert_p = 0;
  size_t length = 0, asn1_length = 0, asn2_length = 0;
  int cert1_present, cert2_present;
  x500name cert1_issuer, cert1_subject, cert2_issuer, cert2_subject;
  int cert1_is_ancestor,  cert1_is_descendant, cert1_is_equal,
      cert1_is_sibling, cert1_depth_difference,
     cert2_is_ancestor,  cert2_is_descendant, cert2_is_equal,
      cert2_is_sibling, cert2_depth_difference;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
    ">process_cross_certs"));

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
    ">process_cross_certs ca = %s", convert_x500name_to_printable_form(ca)));
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
    ">process_cross_certs ca_cert_dir = %s", convert_x500name_to_printable_form(ca_cert_dir)));

  // first, read the x500 certificate attribute values 
  // there should only be one attribute, with possibly multiple values returned
  // where the attribute identifies this to be a certificate attribute, and the
  // individual values correspond to the one or more cross-certificates.

  // even though, strictly speaking, both for ca and user, cross cert oid
  // is same, we have made provision for read_xds_cross_cert.. to 
  // to take in is_ca, just in case we find that the standard specifes
  // otherwise.
  result = read_xds_cross_certificate_attribute(ca_cert_dir, is_ca, &status);
  if (status != 0 || result == 0) {
    return 0; 
  }

  status = result->get_first_attribute(attr_result); // we only have one attribute.
  if (status != 0 || attr_result == 0) {
    delete result;
    return 0;
  }

  status = attr_result->get_first_value(value_result); //get the first value
  if (status != 0 || value_result == 0) {
    delete result;
    return 0; 
  }


  status = value_result->get_value(asn1_cross_cert_p, length);  // get the asn1 encoding in value
  if (status != 0 || length == 0 || asn1_cross_cert_p == 0) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
      "process_cross_certs get_value failed %ld", status));
    delete result;
    return 0;  // actually a problem, in that, there is value, but zero bytes in value.
  }
  

  while (1) {



    status = construct_cross_certificate(asn1_cross_cert_p,length,
				  cert1_present, 
                                 &asn1_cert1_p, asn1_length, 
                                cert1, 
                                cert2_present,
                                 &asn1_cert2_p, asn2_length,
                               cert2);

    if (status != 0)  { // problem creating certificate

      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
        "process_cross_certs construct_cross_certificate failed %ld", status));

      goto next_cross_cert;
    }

    if (cert1_present) {    
        cert1_subject = cert1.certificateInfo.subject;
        cert1_issuer = cert1.certificateInfo.issuer;
        status = determine_relation(cert1_issuer, cert1_subject, cert1_is_ancestor,
                                cert1_is_descendant, cert1_is_equal,
                               cert1_is_sibling, cert1_depth_difference);
        if (status != 0) {
          if (asn1_cross_cert_p)  
             pkc_free (asn1_cross_cert_p);
          if (asn1_cert1_p)  
             pkc_free (asn1_cert1_p);
          if (asn1_cert2_p)  
             pkc_free (asn1_cert2_p);
         delete result;
         return status;
        }
    }

    if (cert2_present) {    
        cert2_subject = cert2.certificateInfo.subject;
        cert2_issuer = cert2.certificateInfo.issuer;

        status = determine_relation(cert2_issuer, cert2_subject, cert2_is_ancestor,
                                cert2_is_descendant, cert2_is_equal,
                               cert2_is_sibling, cert2_depth_difference);
        if (status != 0) {
          if (asn1_cross_cert_p)  
             pkc_free (asn1_cross_cert_p);
          if (asn1_cert1_p)  
             pkc_free (asn1_cert1_p);
          if (asn1_cert2_p)  
             pkc_free (asn1_cert2_p);
          delete result;
          return status;
        }
    }



    switch (flag) {

    case up:

      // check if this certificate belongs here. 
      // for this to work, we tolerate a cross certificate only if
      // issuer == ca, & issuer is a sibling of subject
      // any other type of cross cert is ignored. If the check 
      // passes, we add this cross cert in the subject's cross-cert
      // database entry.

     if ((cert1_present) &&
         (ca == cert1_issuer) &&
         (cert1_is_sibling)) {
        	unsigned char *princ = 0;        
	        status = convert_x500name_to_string(cert1_subject, princ);
               if (status != 0) {
                 if (asn1_cross_cert_p)  
                    pkc_free (asn1_cross_cert_p);
                 if (asn1_cert1_p)  
                    pkc_free (asn1_cert1_p);
                 if (asn1_cert2_p)  
                    pkc_free (asn1_cert2_p);
                  delete result;
                  return status;
               }
               status = add_ca_and_cert_to_crdb((char *)princ, asn1_cert1_p, asn1_length, cainfo_p);
              if (status != 0) {
                if (asn1_cross_cert_p)  
                  pkc_free (asn1_cross_cert_p);
                if (asn1_cert1_p)  
                  pkc_free (asn1_cert1_p);
                if (asn1_cert2_p)  
                  pkc_free (asn1_cert2_p);
                delete result;
                return status;
              }
              pkc_free (princ);
     }  /* cross cert whose subject is a sibling and issuer==ca */

     if ((cert2_present) &&
         (ca == cert2_issuer) &&
         (cert2_is_sibling)) {

        	unsigned char *princ = 0;        
	        status = convert_x500name_to_string(cert2_subject, princ);
               if (status != 0) {
                  if (asn1_cross_cert_p)  
                    pkc_free (asn1_cross_cert_p);
                  if (asn1_cert1_p)  
                    pkc_free (asn1_cert1_p);
                  if (asn1_cert2_p)  
                    pkc_free (asn1_cert2_p);
                  delete result;
                  return status;
               }
               status = add_ca_and_cert_to_crdb((char *)princ, asn1_cert2_p, asn2_length, cainfo_p);
              if (status != 0) {
                if (asn1_cross_cert_p)  
                  pkc_free (asn1_cross_cert_p);
                if (asn1_cert1_p)  
                  pkc_free (asn1_cert1_p);
                if (asn1_cert2_p)  
                  pkc_free (asn1_cert2_p);
                delete result;
                return status;
              }
              pkc_free (princ);
     }  /* cross cert whose subject is a sibling and issuer==ca */


   break;

    case down:

      // check validity of cross cert and location before
     // trying to add the certificate

    if (cert1_present && cert1_is_sibling &&
        cert1_subject == ca) {


	status1  = pkc_check_cert_against_trustlist (partial, &cert1, 1);

        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
          "process_cross_certs pkc_check_cert_against_trustlist failed %ld", status1));

      }

    if (cert2_present && cert2_is_sibling &&
        cert2_subject == ca) {


	status2  = pkc_check_cert_against_trustlist (partial,  &cert2, 1);

        DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
          "process_cross_certs pkc_check_cert_against_trustlist failed %ld", status2));


      }

   break;

   default:  /* should never come here */
     DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
       "process_cross_certs  ** ERROR **"));
   return (-1);
  } /* end of switch statement */





next_cross_cert:
    if (asn1_cross_cert_p)  
       pkc_free (asn1_cross_cert_p);
    if (asn1_cert1_p)  
       pkc_free (asn1_cert1_p);
    if (asn1_cert2_p)  
       pkc_free (asn1_cert2_p);

    asn1_cross_cert_p = 0;
    asn1_cert1_p = 0;
    asn1_cert2_p = 0;


    status = attr_result->get_next_value(value_result); //get the next value value
    if (status != 0 || value_result == 0) {   // end of certificates..
      // should check for value and make sure it indicates last value.. but for now, this is ok.
      // this is really a normal return from this procedure.

      delete result;
      return 0;
   }

    status = value_result->get_value(asn1_cross_cert_p, length);  // get the asn1 encoding in value
    if (status != 0 || length == 0 || asn1_cross_cert_p == 0) {
     return status;
    }

  } /* while (1) */

  /* should never come here */
   DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
     "process_cross_certs  ** ERROR **"));
  return  (-1);
}  /* end of process_ca_user_certs */



unsigned32 process_cross_certs_in_db(pkc_trust_list_t *partial, 
			 x500name  & ca, 
                         int is_ca,
			 direction flag,
			 int depth,
                         struct ca_db_entry **cainfo_p)
{
  unsigned char *princ = 0, *asn1_cert_p = 0;
  unsigned32 status, status1; size_t length;
  struct ca_db_entry *ret_ca = 0, *prev_ca = 0;
 struct cert_entry *certs = 0;
  Certificate cert;

   DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
     ">process_cross_certs_in_db"));

  status = convert_x500name_to_string(ca, princ);
  if (status != 0) {

   DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
     "process_cross_certs_in_db: convert_x500name_to_string failed %ld", status));

    return status;
 }

 status = is_ca_present((char *) princ, *cainfo_p, &ret_ca, &prev_ca);
 if (status == 0) { /* not found */
   status = 0;
   return status;
 }

 certs = ret_ca->hcerts;
 while (certs != NULL) {
   asn1_cert_p = (unsigned char *) certs->c;
   length = certs->size;

   status = construct_certificate((unsigned char *) certs->c, certs->size, cert);
   if (status != 0) { /* maybe a bad certificate, so ignore it */
     certs = certs->next;
     continue;
  }

   // for this routine, we will always be going down.. so no need to
   // check flag etc.. 
   // also , all the validity checks have already been done
  // otherwise, we would not have added this cert
  // in the cross cert database.

  status1  = pkc_check_cert_against_trustlist (partial,  &cert, 1);

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
    "process_cross_certs_in_db: pkc_check_cert_against_trustlist returned %ld", status1));

	// need good debugging statement
   certs = certs->next;
 }  

   DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
     "<process_cross_certs_in_db"));

 return 0;
}


static unsigned32 revoke_certs_if_any (
   const x500name & ca_cert_dir, 
  int is_ca, 
  pkc_trust_list_t *partial)
{
  SignedCertificateList crl;
  xds_status_t *internal_status = 0;
  xds_selection_t criteria;
  xds_t xds_object;
  unsigned32 status;
  const xds_attribute_t *attr_result = 0;
  xds_read_result_t *result = 0;
  const xds_value_t *value_result = 0;
  unsigned char *asn1_cross_cert_p = 0;
  size_t length = 0;
  r_buffer_t from_asn;  // instance of buffer class

   DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
     ">revoke_certs_if_any"));

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
      "revoke_certs_if_any: ca = %s", convert_x500name_to_printable_form(ca_cert_dir)));


  /* however, by default, criteria will have DS_ALL_ATTRIBUTES set to true. since we want only
   * types and values for pkc_cert_attribute, we say so.. 
   */

  criteria.select_specified_attributes();


  /* set the selection criteria to the object id for revoke list */

    if (is_ca)
      criteria.select_attribute(ca_revocation_list_attribute);
    else
      criteria.select_attribute(user_revocation_list_attribute);


  criteria.return_types_and_values();


  internal_status = xds_object.read(ca_cert_dir, criteria, result);

  if (!internal_status) {
    return 0;
  }

  if (internal_status->isError() || result == 0) {
    delete internal_status;
    return 0;
  }

  status = result->get_first_attribute(attr_result); // we only have one attribute.
  if (status != 0 || attr_result == 0) {
    delete internal_status;
    delete result;
    return 0;
  }

  status = attr_result->get_first_value(value_result); //get the first value
  if (status != 0 || value_result == 0) {
    delete internal_status;
    delete result;
    return status; 
  }


  status = value_result->get_value(asn1_cross_cert_p, length);  // get the asn1 encoding in value
  if (status != 0 || asn1_cross_cert_p == 0 || length == 0) {
    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
        "revoke_certs_if_any: get_value failed %ld",status));
    delete internal_status;
    delete result;
    return 0;  // actually a problem, in that, there is value, but zero bytes in value.
  }

  from_asn.data = asn1_cross_cert_p;
  from_asn.data_length = length;

  status =  crl.read(from_asn);
  if (status != 0) {
      DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
          "revoke_certs_if_any: crlread failed %ld",status));
    pkc_free (asn1_cross_cert_p);
    delete internal_status;
    delete result;
    return status;
  }

  status = pkc_revoke_certificates(partial, &crl);

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug4,
       "revoke_certs_if_any: pkc_revoke_certificates returned %ld",status));


  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
       "<revoke_certs_if_any"));

  delete internal_status;
  delete result;
  pkc_free (asn1_cross_cert_p);
  return status;
}

unsigned32   pkc_hierarchy_delete_trustbase (void **context, void **trustbase)
{
  pkc_trust_list_t *tr;
  struct hierarchy_trustbase *h;
  struct ca_db_entry *cainfo;
  unsigned32 status = 0;
  
  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
       ">pkc_hierarchy_delete_trustbase"));

  if (!trustbase)
    return 0;
  if (!(*trustbase))
    return 0;
 
  h = (struct hierarchy_trustbase *) *trustbase;
  tr = h->tr;
  cainfo = h->crdb;
  if (tr) delete tr;
  if (cainfo){
    status =  free_ca_db(&cainfo);
  }

  *trustbase = 0;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
       "<pkc_hierarchy_delete_trustbase"));
  return status;

}


static unsigned32 find_key(void * keyinfo,         /* IN */
                           unsigned key_index,     /* IN */
                           pkc_trust_list_t ** trustlist, /* OUT */
                           pkc_trusted_key_list_element_t ** key /* OUT */
			   ) {
    size_t keycount;
    pkc_trusted_key_list_t *key_list = NULL;
    int i;

  DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
       ">find_key"));

    if (keyinfo == NULL) return pkc_s_bad_param;

    keycount = ((struct hierarchy_keyinfo *) keyinfo)->no_of_keys;
    if (key_index > keycount-1) return pkc_s_index_out_of_range;

    key_list = ((struct hierarchy_keyinfo *) keyinfo)->keylist;

    if (key_list == NULL) return pkc_s_internal_error;

    *trustlist = ((struct hierarchy_keyinfo *) keyinfo)->tr;
    *key = key_list->first;

    if (*key == NULL) return pkc_s_internal_error;

    for (i = 0; i < key_index; i++) {
        *key = (*key)->next;
        if (*key == NULL) {

          DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug1,
               "find_key ** ERROR ** keylist is corrupt"));

            return pkc_s_internal_error;
        };
    };

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
       "<find_key"));

    return 0;
}

unsigned32 pkc_hierarchy_get_key_trust(void **context,
                                       void * keyinfo,
                                       unsigned key_index,
                                       certification_flags_t * flags,
				       uuid_t * domain,
				       pkc_generic_key_usage_t * usage) {
    pkc_trust_list_t * trustlist = NULL;
    pkc_trusted_key_list_element_t * original_key = NULL;
    const pkc_trusted_key_t * key_ptr = NULL;
    unsigned32 status = 0;
    unsigned long key_id;
    x500name issuer;

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
       ">pkc_hierarchy_get_key_trust"));

 
    if (flags == NULL) return pkc_s_asn_bad_param;

    flags->trust_type = CERTIFIED_TRUST;
    flags->missing_crls = 0;
    flags->revoked = 0;

    status = find_key(keyinfo,
                      key_index,
                      &trustlist,
                      &original_key);

    if (status) return status;

// We've found the key the user's asking about.  Search its certification
// path and find the CRL and revocation info.

    status = pkc_lookup_key_in_trustlist(trustlist,
                                         &key_ptr,
                                         original_key->key->get_key_id());
// Now key_ptr points to the trust_list_element that holds this key.
// Work backwards until we find an element with direct_trust set.
    while (status == 0) {

// If any key in the trust-path has its revoked flag set, then set the
// revoked flag in the return parameter.
        if (key_ptr->is_revoked()) flags->revoked = 0;

// If any key in the trust-path has its revoked flag set, then set the
// revoked flag in the return parameter.
        if (key_ptr->is_certified()) {
            status = key_ptr->get_certifier (issuer);
            if ((status != 0)   && (status != pkc_s_certificate_revoked)) 
              return pkc_s_internal_error;

            status = trustlist->revalidation_list.get_last_crl_seen_date(
                                            issuer,
                                            NULL);
            if (status != 0) flags->missing_crls = 1;
        };

        key_id = key_ptr->get_ca_key_id();
        if (key_id == 0) break;

        status = pkc_lookup_key_in_trustlist(trustlist,
                                             &key_ptr,
                                             key_id);

    };

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
       "<pkc_hierarchy_get_key_trust"));


    if (domain) *domain = sec_pk_domain_unspecified;
    if (usage) *usage = 0xfffffffful;
    
    return 0;
}
				       
unsigned32 pkc_hierarchy_get_key_certifier_count(void ** context,
                                                 void * keyinfo,
                                                 unsigned key_index,
                                                 size_t * ca_count) {
    pkc_trust_list_t * trustlist = NULL;
    pkc_trusted_key_list_t *key_list = NULL;
    pkc_trusted_key_list_element_t * original_key = NULL;
    const pkc_trusted_key_t * key_ptr = NULL;
    unsigned long key_id;
    unsigned32 status = 0;

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
       ">pkc_hierarchy_get_key_certifier_count"));

    if (ca_count == NULL) return 0;
    *ca_count = 0;

    status = find_key(keyinfo,
                      key_index,
                      &trustlist,
                      &original_key);

    if (status) return status;

// We've found the key the user's asking about.  Search its certification
// path and count the certifiers

    status = pkc_lookup_key_in_trustlist(trustlist,
                                         &key_ptr,
                                         original_key->key->get_key_id());

// Now key_ptr points to the trust_list_element that holds this key.
// Work backwards until we find an element with direct_trust set.
    while (status == 0) {

	if (!(key_ptr->is_certified())) break;
        key_id = key_ptr->get_ca_key_id();
        if (key_id == 0) break;

	(*ca_count)++;

        status = pkc_lookup_key_in_trustlist(trustlist,
                                             &key_ptr,
                                             key_id);
    };

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
       "<pkc_hierarchy_get_key_certifier_count"));

    if (status == pkc_s_key_not_found)
      return 0;


    return status;

}

unsigned32 pkc_hierarchy_get_key_certifier_info(
                                     void ** context,
                                     void * keyinfo,
                                     unsigned key_index,
                                     unsigned ca_index,
                                     char ** ca_name,
                                     utc_t * certification_start,
                                     utc_t * certification_expiration,
                                     char * crl_valid,
                                     utc_t * last_crl_seen,
                                     utc_t * next_crl_expected) {

    pkc_trust_list_t * trustlist = NULL;
    pkc_trusted_key_list_t *key_list = NULL;
    pkc_trusted_key_list_element_t * original_key = NULL;
    const pkc_trusted_key_t * key_ptr = NULL;
    unsigned long key_id;
    int i = 0;
    unsigned32 status = 0;
    char loc_crl_valid = 0;
    buffer_t temp_buffer;
    x500name ca_x500name;
    utc_t loc_next_crl_date;
    utc_t loc_last_crl_date;

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
       ">pkc_hierarchy_get_key_certifier_info"));


    if (last_crl_seen == NULL) last_crl_seen = &loc_last_crl_date;
    if (next_crl_expected == NULL) next_crl_expected = &loc_next_crl_date;
 
    if (ca_name) *ca_name = NULL;
    *crl_valid = 0;
    
    status = find_key(keyinfo,
                      key_index,
                      &trustlist,
                      &original_key);

    if (status) return status;

// We've found the key the user's asking about.  Search its certification
// path for the specified certifier


    status = pkc_lookup_key_in_trustlist(trustlist,
                                         &key_ptr,
                                         original_key->key->get_key_id());

// Now key_ptr points to the trust_list_element that holds this key.
// Work backwards along the CA chain until we get to the key the user's 
// asking about.
    for (i=0; (status == 0) && (i < ca_index); i++) {

        status = pkc_s_index_out_of_range;
	if (!(key_ptr->is_certified())) break;
        key_id = key_ptr->get_ca_key_id();
        if (key_id == 0) break;

        status = pkc_lookup_key_in_trustlist(trustlist,
                                             &key_ptr,
                                             key_id);
    };

    if (status) return pkc_s_index_out_of_range;

// Now we've found the key entry for the requested CA.  Return the stuff our
// caller asked for...

    status = key_ptr->get_certifier(ca_x500name);
    if ((status != 0)   && (status != pkc_s_certificate_revoked)) 
      return status;

    if (ca_name) {
        status = ca_x500name.display(temp_buffer, 1);
        if (status) return status;
	*ca_name = (char *)pkc_malloc(temp_buffer.data_length + 1);
        if (*ca_name == NULL) return pkc_s_nomem;
	memcpy(*ca_name,
               temp_buffer.data,
               temp_buffer.data_length);
	(*ca_name)[temp_buffer.data_length] = '\0';
    };

    if (certification_start) {
        status = key_ptr->get_key_start_date(certification_start);
	if ((status != 0) && (status != pkc_s_certificate_revoked)) return status;
    };

    if (certification_expiration) {
        status = key_ptr->get_key_end_date(certification_expiration);
	if ((status != 0) && (status != pkc_s_certificate_revoked)) return status;
    };

    status = trustlist->revalidation_list.get_next_crl_expected_date(
                                                            ca_x500name,
                                                            next_crl_expected);

    status = trustlist->revalidation_list.get_last_crl_seen_date(
                                                            ca_x500name,
                                                            last_crl_seen);
    if (status == 0) loc_crl_valid = 1;

    if (crl_valid) {
	*crl_valid = loc_crl_valid;
    };

    DCE_SVC_DEBUG((pkc__svc_handle, pkc_s_plcy, svc_c_debug9,
       "<pkc_hierarchy_get_key_certifier_info"));

    return 0;
}

pkc_policy_t pkc_hierarchy = {
    pkc_V1,
    {
      PKC_ENC_LENGTH(PKC_C_PLCY_DCE_HIERARCHY_OID_ENC),
      PKC_C_PLCY_DCE_HIERARCHY_OID_ENC
    },
    {
      pkc_hierarchy_flags.threadsafe,
      pkc_hierarchy_flags.multi_session
    },
    "",
    pkc_hierarchy_name,
    pkc_hierarchy_open,
    pkc_hierarchy_close,
    pkc_hierarchy_establish,
    pkc_hierarchy_retrieve,
    pkc_hierarchy_delete_trustbase,
    pkc_hierarchy_delete_key_info,
    pkc_hierarchy_get_key_count,
    pkc_hierarchy_get_key_data,
    pkc_hierarchy_get_key_trust,
    pkc_hierarchy_get_key_certifier_count,
    pkc_hierarchy_get_key_certifier_info,
};
