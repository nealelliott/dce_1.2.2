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
 * $Log: test_registry.cxx,v $
 * Revision 1.1.2.1  1996/11/14  17:07:02  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/14  15:51:28  arvind]
 *
 * Revision /main/DCE_1.2.2/2  1996/11/04  22:33 UTC  arvind
 * 	Cert-API drop 2 from DEC
 * 	[1996/11/03  22:53 UTC  arvind  /main/DCE_1.2.2/arvind_capi2/1]
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
 * ABSTRACT:	Test program for Registry Policy 
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
#include <strings.h>
#include <pthread.h>
#include <dce/dce_cf.h>
#include <dce/binding.h>
#include <dce/sec_rgy_attr.h>
#include <dce/sec_pk.h>

#ifdef ENVIRONMENT_TET
#include <tet_api.h>
#endif

}

#include <pkc_plcy_oid.h>
#include <pkc_api.h>
#include "common_defines_a.h"




#ifdef ENVIRONMENT_TET


static void startup(void) 
{
return;
}
static void cleanup(void) 
{
return;
}

static void startup(void);
static void cleanup(void);
static void tp(void);

void (*tet_startup)() = startup;
void (*tet_cleanup)() = cleanup;

struct tet_testlist tet_testlist[] = {
{tp, 1},
{NULL, 0}
};


#define REGISTRY_TEST_ARGUMENTS "REGISTRY_TEST_ARGUMENTS"

#endif




#define TEST_PRINCIPAL "test_capi"
/* foo_key_data[16] holds 00010203...15 */
/* this key data is entered into test_capi's ERA for authen/encipher key */
/* by using dcecp; that way we don't have to worry about writing */
/* code to create schema's, etc */

unsigned char foo_key_data[16];
size_t foo_key_length = 16;





/*
 * usage: test_registry <direct/untrusted>
 */

#ifndef ENVIRONMENT_TET
int main (int argc, char **argv)
#else
int loc_main (int argc, char **argv)
#endif

{
  unsigned32 status;
  trustlist_t *tr_list = 0;
  trustbase_t *tbase;
  keyinfo_t *keyinfohandle = 0;
  size_t no_of_keys;
  unsigned char *keydata = 0;
  size_t keylength;
  char *cell_name, princ_name[1000];
  error_status_t st;
  int i = 0;
  int check_trust_flag;
  uuid_t domain;
  unsigned long usages;
  
  /* if you run this program with argv[1] as direct, it is assumed
   * that you have done a dce_login -c (or certified your credentials)
   * This way , you should get back DIRECT_TRUST in certification
   * flag; otherwise, you will get UNTRUSTED
   */
   
  if (argc != 2) {
    print_general_1("Usage: test_registry <direct/untrusted>\n");
    return(-1);
  }
 
  if (strcmp (argv[1], "direct") == 0)
    check_trust_flag = DIRECT_TRUST;
  else
    check_trust_flag = UNTRUSTED;


  print_general_1("*********************************************************\n");
  print_general_1("***************  Testing Registry Policy ****************\n");

  /* initialize foo's key */
  for (i=0;i<16;i++) foo_key_data[i] = i; 
 
  print_general_1("=========================================================\n");
  print_general_1("===============  Doing pkc_init_trustlist ===============\n");

  status = pkc_init_trustlist(&tr_list);
  if (status != 0) {
    print_general_1("test_registry: problem doing pkc_init_trustlist\n");
    return(-1);
  } else
    print_general_1(" .... succeded\n");
  
  print_general_1("=========================================================\n");
  print_general_1("===============  Doing pkc_init_trustbase ===============\n");
  status = pkc_init_trustbase(&tr_list,
               (gss_OID) &pkc_c_policy_dce_registry,
               0,
               0,
              &tbase);
  if (status != 0) {
    print_general_1("test_registry: problem doing pkc_init_trustbase\n");
    return(-1);
  } else
    print_general_1(" .... succeded\n");

  dce_cf_get_cell_name (&cell_name, &st);
  if (st != dce_cf_st_ok) {
    print_general_2("test_registry: Cannot get cellname %ld\n",st);
    return ((int) st);
  }

  strcpy ((char *) princ_name,cell_name);
  free (cell_name);
  strcat ((char *) princ_name, "/");
  strcat ((char *) princ_name, TEST_PRINCIPAL);

  print_general_1("=========================================================\n");
  print_general_1("===============  Doing pkc_pkc_retrieve_keyinfo ===============\n");

  status = pkc_retrieve_keyinfo (tbase, (char *) princ_name,
                                0, 0, NULL, NULL, &keyinfohandle);
  if (status != 0) {
    print_general_2("test_registry: pkc_retrieve_keyinfo failed %ld\n",status);
    return(-1);
  } else 
    print_general_1(" .... succeded\n");

  print_general_1("=========================================================\n");
  print_general_1("===============  Doing pkc_pkc_retrieve_keyinfo ===============\n");
  status = pkc_get_key_count (keyinfohandle, &no_of_keys);
  if (status != 0) {
    print_general_2("test_registry: pkc_key_key_count failed %ld\n",status);
    return(-1);
  } 
 
  if (no_of_keys != 2) {
    print_general_1("test_registry: pkc_key_key_count failed to return 1 key\n");
    return(-1);
  }
 
  print_general_1(" .... succeded\n");
 
  print_general_1("=========================================================\n");
  print_general_1("===============  Doing pkc_get_key_data for authnkey ====\n");
  // test first key  (key index 0)
  status = pkc_get_key_data(keyinfohandle, 0, &keydata, &keylength);
  if (status != 0) {
    print_general_2("test_registry: pkc_get_key_data failed %ld\n",status);
    return(-1);
  }

  if (foo_key_length != keylength) {
    print_general_1("test_registry: pkc_get_key_data failed to return right keylength\n");
    print_general_2("test_registry:  -- expected keylength = %ld", foo_key_length);
    print_general_2("                --  returned keylength = %ld\n",  keylength);
    return(-1);
  }

  if (memcmp (keydata, foo_key_data, foo_key_length) != 0) { 
    print_general_1("test_registry: pkc_get_key_data failed to return right keydata\n");
    return(-1);
  }

  print_general_1(" .... succeded\n");

  print_general_1("=========================================================\n");
  print_general_1("===============  Doing pkc_get_key_data for cipherkey====\n");
  // test second key  (key index 1)
  status = pkc_get_key_data(keyinfohandle, 1, &keydata, &keylength);
  if (status != 0) {
    print_general_2("test_registry: pkc_get_key_data failed %ld\n",status);
    return(-1);
  }

  if (foo_key_length != keylength) {
    print_general_1("test_registry: pkc_get_key_data failed to return right keylength\n");
    print_general_2("test_registry:  -- expected keylength = %ld", foo_key_length);
    print_general_2("                -- returned keylength = %ld\n",  keylength);
    return(-1);
  }

  if (memcmp (keydata, foo_key_data, foo_key_length) != 0) { 
    print_general_1("test_registry: pkc_get_key_data failed to return right keydata\n");
    return(-1);
  }

  print_general_1(" .... succeded\n");

  {
    certification_flags_t certflags;
    size_t ca_count;

    print_general_1("=========================================================\n");
    print_general_1("===============  Doing pkc_get_key_trust_info for authnkey====\n");
    status = pkc_get_key_trust_info(keyinfohandle,
                                    0,
				    &certflags,
				    &domain,
                                    &usages);
  
    if (status != 0) {
      print_general_2("test_registry: pkc_get_key_trust_info failed %ld\n",status);
      return(-1);
    }

    if (certflags.trust_type != check_trust_flag)    {
      print_general_2("test_registry: pkc_get_key_trust_info failed to return %ld for trusttype\n",
                  check_trust_flag);
      return(-1);
    }

    if (certflags.missing_crls != FALSE  || certflags.revoked != FALSE) { 
      print_general_1("test_registry: pkc_get_key_trust_info failed to return FALSE for missing_crls and revoked fields\n");
      return(-1);
    }

    print_general_1(" .... succeded\n");
    print_general_1("=========================================================\n");
    print_general_1("===============  Doing pkc_get_key_trust_info for cipher====\n");
    status = pkc_get_key_trust_info(keyinfohandle,
                                    1,
				    &certflags,
				    &domain,
                                    &usages);
  
    if (status != 0) {
      print_general_2("test_registry: pkc_get_key_trust_info failed %ld\n",status);
      return(-1);
    }

    if (certflags.trust_type != check_trust_flag) {
      print_general_2("test_registry: pkc_get_key_trust_info failed to return %ld for trusttype\n",                     check_trust_flag);
      return(-1);
    }

    if (certflags.missing_crls != FALSE  || certflags.revoked != FALSE) { 
      print_general_1("test_registry: pkc_get_key_trust_info failed to return FALSE for missing_crls and revoked fields\n");
      return(-1);
    }

    print_general_1(" .... succeded\n");
    status = pkc_get_key_certifier_count(keyinfohandle,0,&ca_count);
    print_general_1("=========================================================\n");
    print_general_1("===============  Doing pkc_get_key_certifier_count ====\n");
    if (status != 0) {
      print_general_2("test_registry: pkc_get_key_certifier_count failed %ld\n",status);
      return(-1);
    }

    if (ca_count != 0) {
      print_general_1("test_registry: pkc_get_key_trust_info failed to return 0 for ca_count\n");
      return(-1);
    }

    status = pkc_get_key_certifier_count(keyinfohandle,1,&ca_count);
    if (status != 0) {
      print_general_2("test_registry: pkc_get_key_certifier_count failed %ld\n",status);
      return(-1);
    }

    if (ca_count != 0) {
      print_general_1("test_registry: pkc_get_key_trust_info failed to return 0 for ca_count\n");
      return(-1);
    }
    print_general_1(" .... succeded\n");

  }


  // now free up the memory 

  status = pkc_free_trustlist (&tr_list);
  if (status != 0) {
    print_general_2("test_registry: pkc_free_trustlist failed %ld\n",status);
    return(-1);
  }
  
  status = pkc_free_keyinfo (&keyinfohandle);
  if (status != 0) {
    print_general_2("test_registry: pkc_free_trustlist failed %ld\n",status);
    return(-1);
  }
  
  status = pkc_free_trustbase(&tbase);
  if (status != 0) {
    print_general_2("test_registry: pkc_free_trustlist failed %ld\n",status);
    return(-1);
  }

 print_general_1("test_registry: Succeded\n");
 return(0); 
}



#ifdef ENVIRONMENT_TET

static void tp(void) {

    char *argv[128];
    int argc;
    char * progname = "test_registry";
    char arg_buffer[256];
    char * arglist = NULL;
    char * cp1;
    char * cp2;
    int result;
    
    tet_infoline("Retrieving test-driver argument-list");
    arglist = getenv(REGISTRY_TEST_ARGUMENTS);

    argc = 1;
    argv[0] = progname;

    if (arglist != NULL) {
	tet_infoline("Parsing argument-list");
	strncpy(arg_buffer, arglist, sizeof(arg_buffer));
	arg_buffer[sizeof(arg_buffer)-1] = 0;    
	cp1 = arg_buffer;
		
	while (cp1[0] != 0) {
/* Advance cp1 over leading spaces...					    */
	    while (cp1[0] == ' ') cp1++;
	    if (cp1[0] == 0) continue;
/* Now cp1 points to the start of the next argument...			    */
 	    cp2 = cp1;
/* Advance cp2 over the argument...					    */
	    while (cp2[0] != ' ' && cp2[0] != 0) cp2++;
/* Now cp1 points to the first character of the argument, cp2 points to one */
/* past the end.							    */
	    argv[argc++] = cp1;
	    if (cp2[0] == 0) {
		cp1 = cp2;
	    } else {
		cp1 = cp2+1;
		cp2[0] = 0;
	    };
	};
    } else {
	tet_infoline("No arguments found");
    };

    tet_infoline("Invoking main test program");

    result = loc_main(argc, argv);    

    tet_infoline("Returned from main test program");

    if (result) {
	tet_infoline("Test failed");
	tet_result(TET_FAIL);
    } else {
	tet_infoline("Tests passed");
	tet_result(TET_PASS);
    };
    return;    
}
#endif

