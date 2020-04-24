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
 * $Log: common_defines_a.h,v $
 * Revision 1.1.2.1  1996/11/14  16:59:42  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/14  15:03:08  arvind]
 *
 * Revision /main/DCE_1.2.2/2  1996/11/04  22:33 UTC  arvind
 * 	Cert-API drop 2 from DEC
 * 	[1996/11/03  22:53 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
 * 
 * 	Initial revision
 * 	[1996/10/15  19:28:36  pkdev  1.1]
 * 
 * Revision /main/DCE_1.2.2/1  1996/10/18  00:45 UTC  arvind
 * 	CAPI drop from DEC
 * 	[1996/10/16  20:46 UTC  arvind  /main/arvind_capi/1]
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
 * ABSTRACT:    Common definitions for test code for 	hierarhichal/registry modules.
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




#ifndef _COMMON_DEFINES_A
#define _COMMON_DEFINES_A


#include <pkc_plcy_oid.h>





/* key modulus for public/private key pair generation */
#define KEYMOD 512

/* some arbitrary date in the past */
#define START_TIME "1995-12-25-13:01:02.1234567"

/* some arbitrary date in the future */
#define END_TIME "1999-12-25-13:01:02.1234567"




#define MAX_FILE_LEN  1024
#define TEST_DIR "testcases"
#define CERTS_DIR "certs"
#define CRCERTS_DIR "certs"
#define CRL_DIR "certs"
#define KEYS_DIR "keys"
#define LOG_FILE "capi.log"  /* file to store log of testcases as they are run */
#define PRINCIPAL_FILE "principals"  /* file to hold principals for testcases under testcases */
#define NO_OF_KEYS_FILE "no_of_keys"  /* file to hold number of key pairs to generate 
                                       * for each principal under testcases drectory */
#define CRL_FILE "revokelist"






/* replace line below with actual cert oid str */
/* #define CERT_OID_STR "2.5.4.37" */

#define X500_STR "X500"
#define CDS_STR  "CDS"
#define X500_FLAG 1
#define CDS_FLAG 0

#define USER_CERT_OID_STR "user"
#define CA_CERT_OID_STR "ca"
#define CROSS_CERT_OID_STR "cross"
#define USER_REVOKE_OID_STR "urevoke"
#define CA_REVOKE_OID_STR "carevoke"


#define USER_CERT_OID_FLAG 1
#define CA_CERT_OID_FLAG 2
#define CROSS_CERT_OID_FLAG 3
#define USER_REVOKE_OID_FLAG 4
#define CA_REVOKE_OID_FLAG 5

#define TEST_KEY_COUNT_STR "keycount"
#define TEST_KEY_DATA_STR "keydata"
#define TEST_KEY_TRUST_INFO_STR "trustinfo"
#define TEST_KEY_CERT_COUNT_STR "certcount"
#define TEST_KEY_CERT_INFO_STR "certinfo"
#define TEST_KEY_CERT_ALL_COUNT_STR "allcertcount"

#define TEST_KEY_COUNT_FLAG 1
#define TEST_KEY_DATA_FLAG 2
#define TEST_KEY_TRUST_INFO_FLAG 3
#define TEST_KEY_CERT_COUNT_FLAG 4
#define TEST_KEY_CERT_INFO_FLAG 5
#define TEST_KEY_CERT_ALL_COUNT_FLAG 6



#define BUFSIZE 4096

/* the unique part in this structure is <principal,key_pair> */

struct principal_and_key_info {
char *principal;
char *cell;
int key_pair;  /* which key of this principal. */
void *public_key;
int public_key_length;
void *private_key;
int private_key_length;
} *princ_key_arr;



struct certificate_info {
void *certificate;
int certificate_length; 
char *subject;
char *subject_cell;
int subject_key_pair;
char *issuer;
char *issuer_cell;
int issuer_key_pair;
} *cert_arr;

int total_no_of_principals;
int keys_per_principal;

/* #define MAX_LINELENGTH 1024  replaced by BUFSIZE */

char *find_next_non_blank (char *start);
char *find_next_blank (char *start);
char *find_next_blank_or_char (char *start, char c);
int get_loc_and_princ_attr_value (char *tok,
                         char *loc,
                         char *value);
int get_cds_init_fnames (char *tok,
                         char *config_fname,
                         char *cr_fname,
                         char *delete_fname,
                         char *princ_fname);
void strip_end_of_line (char *p);
 
#if 0
char *return_right_most_rdn (char *princ);
#endif

extern "C" {
int create_attr_value_from_file (char *dir, char *attr, char *fname);
int create_attr_value (char *dir, char *attr, unsigned char *value, size_t  len);
}

int parse_token (char *name, int &x500_flag, char * act_name);
int process_x500_cds_flag(char *buf, int &x500_flag);
int parse_cert_loc_token (char *tok, int &attr_flag, char * certfname,
                int &xds_flag, char *loc);


int parse_revoked_cert_token(char *tok, char *cert_fname,
                             char *date);
int parse_ca_crl_token (char *tok, char *ca_crl, char *thisupdate,
                        char *nextupdate);




int process_attr_flag(char *t1, int &attr_flag);
int parse_test_case_token(char *tok, int & test_case);
int parse_test_case_token_key_count(char *tok,
                            int & expected_status,
                            int &expected_key_count);
int parse_test_case_token_key_data(char *tok,
                            int & expected_status,
                            int &key_index);
int parse_test_case_token_key_trust_info(char *tok,
                            int & expected_status,
                            int &key_index, int &m_crl,
                            int & m_revoked, int &trust_type);

int parse_test_case_token_key_cert_count(char *tok,
                            int & expected_status,
                            int &key_index, int &ca_count);

int parse_test_case_token_key_cert_info(char *tok,
                            int & expected_status,
                            int &key_index, int &cert_index,
                            utc_t *certstart, utc_t *certexp,
                            utc_t *crllastseen, utc_t *nextcrlexpected,
                            char *certname);


int line_is_empty (char *line);

int read_principals ();
int read_principals_and_keys ();
int   find_princ_index(char *princ);
int find_princ_given_index (int index, char *princ);
int read_bytes_from_file (char *filename,  char *octetstring_value, 
                       size_t & len);
int compare_keydata(unsigned char *keydata,
                    size_t keylength,
                    int princ_index,
                    int key_index,
                    unsigned char *princkey,
                    size_t & len);
int parse_princ_key_index(char *q, char *x500_user);

void print_general_1(char *s);
void print_general_2(char *s, unsigned32 st);
void print_general_3(char *s, const char *s1);
void print_general_4(char *s, unsigned32 st1, unsigned32 st2);


#endif
