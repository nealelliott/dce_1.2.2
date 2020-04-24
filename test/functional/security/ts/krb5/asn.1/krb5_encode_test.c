/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 *
 * Copyright (c) Hewlett-Packard Company 1996
 * Unpublished work. All Rights Reserved.
 */
/*
 * HISTORY
 * $Log: krb5_encode_test.c,v $
 * Revision 1.1.2.2  1996/10/03  21:16:51  arvind
 * 	fix gcc -Wall lint.
 * 	[1996/09/26  20:14 UTC  sommerfeld  /main/DCE_1.2.2/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.2.1  1996/08/28  20:58:49  arvind
 * 	DCE 1.2.2 drop 3.5 - (test) files left out
 * 	[1996/08/28  20:48:59  arvind]
 * 
 * Revision /main/DCE_1.2.2/1  1996/06/20  18:18 UTC  mullan_s
 * 	Merge to DCE_1.2.2
 * 
 * Revision /main/mullan_dce_krb5_der_work/1  1996/06/10  19:46 UTC  mullan_s
 * 	Port to DCE.
 * 
 * $EndLog$
 */
#include <stdio.h>
#include "krb5.h"
#include "asn1.h"
#include "com_err.h"
#include "utility.h"

#include "ktest.h"
#include <string.h>

#include "debug.h"

#ifndef OSF_DCE
krb5_context test_context;
#endif
int error_count = 0;
int do_trval = 0;

void encoder_print_results(code, typestring, description)
	krb5_data *code;
	char	*typestring;
	char	*description;
{
	char	*code_string = NULL;
	krb5_error_code	retval;
	int r, rlen;

	if (do_trval) {
		printf("encode_krb5_%s%s:\n", typestring, description);
		r = trval2(stdout, code->data, code->length, 0, &rlen);
		printf("\n\n");
		if (rlen != code->length) {
			printf("Error: length mismatch: was %d, parsed %d\n",
			       code->length, rlen);
			exit(1);
		}
		if (r != 0)
			exit(1);
	} else {
		retval = asn1_krb5_data_unparse(code,&(code_string));
		if(retval) {
			com_err("krb5_encode_test", retval ,
				"while unparsing %s", typestring);
			exit(1);
		}
		printf("encode_krb5_%s%s: %s\n", typestring, description,
		       code_string);
		free(code_string);
	}
	ktest_destroy_data(&code);
}	

void PRS(argc, argv)
	int	argc;
	char	**argv;
{
	extern char *optarg;	
	int optchar;
	extern int print_types, print_krb5_types, print_id_and_len,
		print_constructed_length, print_skip_context,
		print_skip_tagnum, print_context_shortcut;

	while ((optchar = getopt(argc, argv, "t")) != EOF) {
		switch(optchar) {
		case 't':
			do_trval = 1;
			break;
		case '?':
		default:
			fprintf(stderr, "Usage: %s [-t]\n", argv[0]);
			exit(1);
		}
	}
	print_types = 1;
	print_krb5_types = 1;
	print_id_and_len = 0;
	print_constructed_length = 0;
	print_skip_context = 1;
	print_skip_tagnum = 1;
	print_context_shortcut = 1;
}

void main(argc, argv)
	int	argc;
	char	**argv;
{
  krb5_data *code;
  krb5_error_code retval;

  PRS(argc, argv);
  
#ifndef OSF_DCE
  krb5_init_context(&test_context);
  krb5_init_ets(test_context);
#else
  krb5_init_ets();
#endif
  
#define setup(value,type,typestring,constructor)\
  retval = constructor(&(value));\
  if(retval){\
    com_err("krb5_encode_test", retval, "while making sample %s", typestring);\
    exit(1);\
  }
    
#define encode_run(value,type,typestring,description,encoder)\
  retval = encoder(&(value),&(code));\
  if(retval){\
    com_err("krb5_encode_test", retval,"while encoding %s", typestring);\
    exit(1);\
  }\
  encoder_print_results(code, typestring, description);
      
#ifndef OSF_DCE
  /* Not tested for DCE because MIT changed the krb5_authenticator datatype,
   * so encodings will be different. 
   */
  /****************************************************************/
  /* encode_krb5_authenticator */
  {
    krb5_authenticator authent;
    setup(authent,authenticator,"authenticator",ktest_make_sample_authenticator);

    encode_run(authent,authenticator,"authenticator","",encode_krb5_authenticator);

    ktest_destroy_checksum(&(authent.checksum));
    ktest_destroy_keyblock(&(authent.subkey));
    authent.seq_number = 0;
    ktest_empty_authorization_data(authent.authorization_data);
    encode_run(authent,authenticator,"authenticator","(optionals empty)",encode_krb5_authenticator);

    ktest_destroy_authorization_data(&(authent.authorization_data));
    encode_run(authent,authenticator,"authenticator","(optionals NULL)",encode_krb5_authenticator);
  }
#endif
  
  /****************************************************************/
  /* encode_krb5_ticket */
  {
    krb5_ticket tkt;
    setup(tkt,ticket,"ticket",ktest_make_sample_ticket);
    encode_run(tkt,ticket,"ticket","",encode_krb5_ticket);
  }

  /****************************************************************/
  /* encode_krb5_encryption_key */
  {
    krb5_keyblock keyblk;
    setup(keyblk,keyblock,"keyblock",ktest_make_sample_keyblock);
#ifdef OSF_DCE
    encode_run(keyblk,keyblock,"keyblock","",encode_krb5_keyblock);
#else
    encode_run(keyblk,keyblock,"keyblock","",encode_krb5_encryption_key);
#endif
  }  
  
  /****************************************************************/
  /* encode_krb5_enc_tkt_part */
  {
    krb5_ticket tkt;
    tkt.enc_part2 = (krb5_enc_tkt_part*)calloc(1,sizeof(krb5_enc_tkt_part));
    if(tkt.enc_part2 == NULL) com_err("allocating enc_tkt_part",errno,"");
    setup(*(tkt.enc_part2),enc_tkt_part,"enc_tkt_part",ktest_make_sample_enc_tkt_part);
  
    encode_run(*(tkt.enc_part2),enc_tkt_part,"enc_tkt_part","",encode_krb5_enc_tkt_part);
  
    tkt.enc_part2->times.starttime = 0;
    tkt.enc_part2->times.renew_till = 0;
    ktest_destroy_address(&(tkt.enc_part2->caddrs[1]));
    ktest_destroy_address(&(tkt.enc_part2->caddrs[0]));
    ktest_destroy_authdata(&(tkt.enc_part2->authorization_data[1]));
    ktest_destroy_authdata(&(tkt.enc_part2->authorization_data[0]));
  
    /* ISODE version fails on the empty caddrs field */
    ktest_destroy_addresses(&(tkt.enc_part2->caddrs));
    ktest_destroy_authorization_data(&(tkt.enc_part2->authorization_data));
  
#ifndef OSF_DCE
    /* Not tested for DCE because we always encode the caddr field, even if NULL */
    encode_run(*(tkt.enc_part2),enc_tkt_part,"enc_tkt_part","(optionals NULL)",encode_krb5_enc_tkt_part);
#endif
  }  
  
  /****************************************************************/
  /* encode_krb5_enc_kdc_rep_part */
  {
    krb5_kdc_rep kdcr;
  
    kdcr.enc_part2 = (krb5_enc_kdc_rep_part*)
      calloc(1,sizeof(krb5_enc_kdc_rep_part));
    if(kdcr.enc_part2 == NULL) com_err("allocating enc_kdc_rep_part",errno,"");
    setup(*(kdcr.enc_part2),enc_kdc_rep_part,"enc_kdc_rep_part",ktest_make_sample_enc_kdc_rep_part);
  
    encode_run(*(kdcr.enc_part2),enc_kdc_rep_part,"enc_kdc_rep_part","",encode_krb5_enc_kdc_rep_part);
  
    kdcr.enc_part2->key_exp = 0;
    kdcr.enc_part2->times.starttime = 0;
    kdcr.enc_part2->flags &= ~TKT_FLG_RENEWABLE;
    ktest_destroy_addresses(&(kdcr.enc_part2->caddrs));
  
    encode_run(*(kdcr.enc_part2),enc_kdc_rep_part,"enc_kdc_rep_part","(optionals NULL)",encode_krb5_enc_kdc_rep_part);
  }  

  /****************************************************************/
  /* encode_krb5_as_rep */
  {
    krb5_kdc_rep kdcr;
    setup(kdcr,kdc_rep,"kdc_rep",ktest_make_sample_kdc_rep);
  
/*    kdcr.msg_type = KRB5_TGS_REP;
    assert(encode_krb5_as_rep(&kdcr,&code) == KRB5_BADMSGTYPE,
	   "encode_krb5_as_rep type check\n");
    ktest_destroy_data(&code);*/
  
    kdcr.msg_type = KRB5_AS_REP;
    encode_run(kdcr,as_rep,"as_rep","",encode_krb5_as_rep);
  
    ktest_destroy_pa_data_array(&(kdcr.padata));
    encode_run(kdcr,as_rep,"as_rep","(optionals NULL)",encode_krb5_as_rep);
  }  
  
  /****************************************************************/
  /* encode_krb5_tgs_rep */
  {
    krb5_kdc_rep kdcr;
    setup(kdcr,kdc_rep,"kdc_rep",ktest_make_sample_kdc_rep);

/*    kdcr.msg_type = KRB5_AS_REP;
    assert(encode_krb5_tgs_rep(&kdcr,&code) == KRB5_BADMSGTYPE,
	   "encode_krb5_tgs_rep type check\n");*/
  
    kdcr.msg_type = KRB5_TGS_REP;
    encode_run(kdcr,tgs_rep,"tgs_rep","",encode_krb5_tgs_rep);

    ktest_destroy_pa_data_array(&(kdcr.padata));
    encode_run(kdcr,tgs_rep,"tgs_rep","(optionals NULL)",encode_krb5_tgs_rep);
  }  
  
  /****************************************************************/
  /* encode_krb5_ap_req */
  {
    krb5_ap_req apreq;
    setup(apreq,ap_req,"ap_req",ktest_make_sample_ap_req);
    encode_run(apreq,ap_req,"ap_req","",encode_krb5_ap_req);
  }  

  /****************************************************************/
  /* encode_krb5_ap_rep */
  {
    krb5_ap_rep aprep;
    setup(aprep,ap_rep,"ap_rep",ktest_make_sample_ap_rep);
    encode_run(aprep,ap_rep,"ap_rep","",encode_krb5_ap_rep);
  }  

  /****************************************************************/
  /* encode_krb5_ap_rep_enc_part */
  {
    krb5_ap_rep_enc_part apenc;
    setup(apenc,ap_rep_enc_part,"ap_rep_enc_part",ktest_make_sample_ap_rep_enc_part);
    encode_run(apenc,ap_rep_enc_part,"ap_rep_enc_part","",encode_krb5_ap_rep_enc_part);
  
    ktest_destroy_keyblock(&(apenc.subkey));
    apenc.seq_number = 0;
    encode_run(apenc,ap_rep_enc_part,"ap_rep_enc_part","(optionals NULL)",encode_krb5_ap_rep_enc_part);
  }
  
  /****************************************************************/
  /* encode_krb5_as_req */
  {
    krb5_kdc_req asreq;
    setup(asreq,kdc_req,"kdc_req",ktest_make_sample_kdc_req);
    asreq.msg_type = KRB5_AS_REQ;
    asreq.kdc_options &= ~KDC_OPT_ENC_TKT_IN_SKEY;
    encode_run(asreq,as_req,"as_req","",encode_krb5_as_req);

    ktest_destroy_pa_data_array(&(asreq.padata));
    ktest_destroy_principal(&(asreq.client));
#ifndef ISODE_SUCKS
    ktest_destroy_principal(&(asreq.server));
#endif
    asreq.kdc_options |= KDC_OPT_ENC_TKT_IN_SKEY;
    asreq.from = 0;
    asreq.rtime = 0;
    ktest_destroy_addresses(&(asreq.addresses));
    ktest_destroy_enc_data(&(asreq.authorization_data));
#ifndef OSF_DCE
    /* Not tested for DCE because the sname field is mandatory -
     * currently under discussion to be changed from OPTIONAL to mandatory
     * in RFC 1510. 
     */
    encode_run(asreq,as_req,"as_req","(optionals NULL except second_ticket)",encode_krb5_as_req);
#endif
    ktest_destroy_sequence_of_ticket(&(asreq.second_ticket));
#ifndef ISODE_SUCKS
    ktest_make_sample_principal(&(asreq.server));
#endif
    asreq.kdc_options &= ~KDC_OPT_ENC_TKT_IN_SKEY;
    encode_run(asreq,as_req,"as_req","(optionals NULL except server)",encode_krb5_as_req);
  }
  
  /****************************************************************/
  /* encode_krb5_tgs_req */
  {
    krb5_kdc_req tgsreq;
    setup(tgsreq,kdc_req,"kdc_req",ktest_make_sample_kdc_req);
    tgsreq.msg_type = KRB5_TGS_REQ;
    tgsreq.kdc_options &= ~KDC_OPT_ENC_TKT_IN_SKEY;
    encode_run(tgsreq,tgs_req,"tgs_req","",encode_krb5_tgs_req);

    ktest_destroy_pa_data_array(&(tgsreq.padata));
    ktest_destroy_principal(&(tgsreq.client));
#ifndef ISODE_SUCKS
    ktest_destroy_principal(&(tgsreq.server));
#endif
    tgsreq.kdc_options |= KDC_OPT_ENC_TKT_IN_SKEY;
    tgsreq.from = 0;
    tgsreq.rtime = 0;
    ktest_destroy_addresses(&(tgsreq.addresses));
    ktest_destroy_enc_data(&(tgsreq.authorization_data));
#ifndef OSF_DCE
    /* Not tested for DCE because the sname field is mandatory -
     * currently under discussion to be changed from OPTIONAL to mandatory
     * in RFC 1510.
     */
    encode_run(tgsreq,tgs_req,"tgs_req","(optionals NULL except second_ticket)",encode_krb5_tgs_req);
#endif

    ktest_destroy_sequence_of_ticket(&(tgsreq.second_ticket));
#ifndef ISODE_SUCKS
    ktest_make_sample_principal(&(tgsreq.server));
#endif
    tgsreq.kdc_options &= ~KDC_OPT_ENC_TKT_IN_SKEY;
    encode_run(tgsreq,tgs_req,"tgs_req","(optionals NULL except server)",encode_krb5_tgs_req);
  }
  
  /****************************************************************/
  /* encode_krb5_kdc_req_body */
  {
    krb5_kdc_req kdcrb;
    setup(kdcrb,kdc_req_body,"kdc_req_body",ktest_make_sample_kdc_req_body);
    kdcrb.kdc_options &= ~KDC_OPT_ENC_TKT_IN_SKEY;
    encode_run(kdcrb,kdc_req_body,"kdc_req_body","",encode_krb5_kdc_req_body);

    ktest_destroy_principal(&(kdcrb.client));
#ifndef ISODE_SUCKS
    ktest_destroy_principal(&(kdcrb.server));
#endif
    kdcrb.kdc_options |= KDC_OPT_ENC_TKT_IN_SKEY;
    kdcrb.from = 0;
    kdcrb.rtime = 0;
    ktest_destroy_addresses(&(kdcrb.addresses));
    ktest_destroy_enc_data(&(kdcrb.authorization_data));
#ifndef OSF_DCE
    /* Not tested for DCE because the sname field is mandatory -
     * currently under discussion to be changed from OPTIONAL to mandatory
     * in RFC 1510.
     */
    encode_run(kdcrb,kdc_req_body,"kdc_req_body","(optionals NULL except second_ticket)",encode_krb5_kdc_req_body);
#endif

    ktest_destroy_sequence_of_ticket(&(kdcrb.second_ticket));
#ifndef ISODE_SUCKS
    ktest_make_sample_principal(&(kdcrb.server));
#endif
    kdcrb.kdc_options &= ~KDC_OPT_ENC_TKT_IN_SKEY;
    encode_run(kdcrb,kdc_req_body,"kdc_req_body","(optionals NULL except server)",encode_krb5_kdc_req_body);
  }
  
  /****************************************************************/
  /* encode_krb5_safe */
  {
    krb5_safe s;
    setup(s,safe,"safe",ktest_make_sample_safe);
    encode_run(s,safe,"safe","",encode_krb5_safe);

    s.timestamp = 0;
    /* s.usec should be opted out by the timestamp */
    s.seq_number = 0;
    ktest_destroy_address(&(s.r_address));
    encode_run(s,safe,"safe","(optionals NULL)",encode_krb5_safe);
  }
  
  /****************************************************************/
  /* encode_krb5_priv */
  {
    krb5_priv p;
    setup(p,priv,"priv",ktest_make_sample_priv);
    encode_run(p,priv,"priv","",encode_krb5_priv);
  }
  
  /****************************************************************/
  /* encode_krb5_enc_priv_part */
  {
    krb5_priv_enc_part ep;
    setup(ep,priv_enc_part,"priv_enc_part",ktest_make_sample_priv_enc_part);
    encode_run(ep,enc_priv_part,"enc_priv_part","",encode_krb5_enc_priv_part);

    ep.timestamp = 0;
    /* ep.usec should be opted out along with timestamp */
    ep.seq_number = 0;
    ktest_destroy_address(&(ep.r_address));
    encode_run(ep,enc_priv_part,"enc_priv_part","(optionals NULL)",encode_krb5_enc_priv_part);
  }
  
#ifndef OSF_DCE
  /* Not tested for DCE because there is no krb5_cred datatype */
  /****************************************************************/
  /* encode_krb5_cred */
  {
    krb5_cred c;
    setup(c,cred,"cred",ktest_make_sample_cred);
    encode_run(c,cred,"cred","",encode_krb5_cred);
  }
  
  /****************************************************************/
  /* encode_krb5_enc_cred_part */
  {
    krb5_cred_enc_part cep;
    setup(cep,cred_enc_part,"cred_enc_part",ktest_make_sample_cred_enc_part);
    encode_run(cep,enc_cred_part,"enc_cred_part","",encode_krb5_enc_cred_part);

    ktest_destroy_principal(&(cep.ticket_info[0]->client));
    ktest_destroy_principal(&(cep.ticket_info[0]->server));
    cep.ticket_info[0]->flags = 0;
    cep.ticket_info[0]->times.authtime = 0;
    cep.ticket_info[0]->times.starttime = 0;
    cep.ticket_info[0]->times.endtime = 0;
    cep.ticket_info[0]->times.renew_till = 0;
    ktest_destroy_addresses(&(cep.ticket_info[0]->caddrs));
    cep.nonce = 0;
    cep.timestamp = 0;
    ktest_destroy_address(&(cep.s_address));
    ktest_destroy_address(&(cep.r_address));
    encode_run(cep,enc_cred_part,"enc_cred_part","(optionals NULL)",encode_krb5_enc_cred_part);
  }
#endif
  
  /****************************************************************/
  /* encode_krb5_error */
  {
    krb5_error kerr;
    setup(kerr,error,"error",ktest_make_sample_error);
    encode_run(kerr,error,"error","",encode_krb5_error);

    kerr.ctime = 0;
    ktest_destroy_principal(&(kerr.client));
    ktest_empty_data(&(kerr.text));
    ktest_empty_data(&(kerr.e_data));
    encode_run(kerr,error,"error","(optionals NULL)",encode_krb5_error);
  }
  
  /****************************************************************/
  /* encode_krb5_authdata */
  {
    krb5_authdata **ad;
    setup(ad,authorization_data,"authorization_data",ktest_make_sample_authorization_data);

    retval = encode_krb5_authdata(ad,&(code));
    if(retval) com_err("encoding authorization_data",retval,"");
    encoder_print_results(code, "authorization_data", "");
  }
  
#ifndef OSF_DCE
  /* Not tested for DCE because there is no passwd_phrase_element datatype */
  /****************************************************************/
  /* encode_pwd_sequence */
  {
    passwd_phrase_element ppe;
    setup(ppe,passwd_phrase_element,"PasswdSequence",ktest_make_sample_passwd_phrase_element);
    encode_run(ppe,passwd_phrase_element,"pwd_sequence","",encode_krb5_pwd_sequence);
  }

  /****************************************************************/
  /* encode_passwd_data */
  {
    krb5_pwd_data pd;
    setup(pd,krb5_pwd_data,"PasswdData",ktest_make_sample_krb5_pwd_data);
    encode_run(pd,krb5_pwd_data,"pwd_data","",encode_krb5_pwd_data);
  }
#endif

  exit(error_count);
}
