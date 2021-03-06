//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: test_pkss_5.cxx,v $
// Revision 1.1.2.1  1996/10/03  20:34:54  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/03  19:35:06  arvind]
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:31 UTC  arvind
// 	PKSS drop from DEC (DCE1.2.2)
// 	[1996/08/30  16:01 UTC  arvind  /main/arvind_pkss/1]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
// Module:  test_pkss_5.cxx
//
// Purpose: Continuous client test (requires no login credentials)
//
// Calls:  sec_pvtkey_pkss_open
//         sec_pvtkey_pkss_get
//	   sec_pvtkey_pkss_update (client)
//	   sec_pvtkey_pkss_update_generate (client)
//	   sec_pvtkey_pkss_close
//

extern "C"
{
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dce/nbase.h>
#include <dce/sec_pk.h>
#include "sec_bsafe.h"
#include <dce/sec_psm_base.h>
}

#include "sec_pkss_util.h"
#include "sec_pvtkey_pkss.h"

  void interpret_character_vector (int, char *, int *, char *, char *);
  void cds_ieu_emit (const char *, const char *, const char *);


#define USER5_USERNAME	((unsigned char *)"user5")
#define USER5_PASSWORD	((unsigned char *)"user5pwd")
#define USER5_NEWPWD	((unsigned char *)"user5new")


sec_pk_pvtkey_capability SEC_PVTKEY_CAP_KEYGEN
= { /* 6E78B87A-D4DE-11CF-987D-08002B187D1A */
        0x6e78b87a,
        0xd4de,
        0x11cf,
        0x98,
        0x7d,
        {0x08, 0x00, 0x2b, 0x18, 0x7d, 0x1a}
      };

sec_pk_pvtkey_capability SEC_PVTKEY_CAP_ANYALG
    = { /* 8BE2EEDA-D4DE-11CF-9F9C-08002B187D1A */
        0x8be2eeda,
        0xd4de,
        0x11cf,
        0x9f,
        0x9c,
        {0x08, 0x00, 0x2b, 0x18, 0x7d, 0x1a}
      };

sec_pk_pvtkey_capability SEC_PVTKEY_CAP_KEYALG
    = { /* 7E2D695A-D4DE-11CF-8696-08002B187D1A */
        0x7e2d695a,
        0xd4de,
        0x11cf,
        0x86,
        0x96,
        {0x08, 0x00, 0x2b, 0x18, 0x7d, 0x1a}
      };

sec_pk_pvtkey_capability SEC_PVTKEY_CAP_KEYDEL
    = { /* 9B0523BA-D4DE-11CF-8E7E-08002B187D1A */
        0x9b0523ba,
        0xd4de,
        0x11cf,
        0x8e,
        0x7e,
        {0x08, 0x00, 0x2b, 0x18, 0x7d, 0x1a}
      };

#ifndef CDS_GDB_EXISTS
#define CDS_GDB_EXISTS                  0x10d0afd7
#endif

int main (int argc, char **argv)
{
  //
  // Local variables
  //

  error_status_t estatus = 0;
  error_status_t pstatus = 0;
  unsigned32 pk_key_len = 512;
  unsigned32 key_vno = 0;
  unsigned32 ret_key_vno = 0;
  sec_pk_gen_data_t seed;
  sec_pk_gen_data_t rand;
  sec_pk_data_t kou;
  sec_pk_data_t kxu;
  sec_pk_data_t gen_kou;
  sec_pk_data_t ret_kou;
  sec_pk_data_t ret_kxu;
  sec_pk_usage_flags_t key_usages = 1;
  sec_pk_mechanism_handle_t user5_psm_context = 0;
  char i_val[512];
  char h_val[2048];
  int size;

  //
  // Open a session with the PKSS for USER5.
  //

#ifdef TRACE
  (void) printf ("Entering sec_pvtkey_pkss_open for user %s\n", USER5_USERNAME);
  (void) fflush (stdout);
#endif

  estatus = sec_pvtkey_pkss_open (USER5_USERNAME,
				  &sec_pk_domain_dce_pk_login,
				  &user5_psm_context);

#ifdef TRACE
  (void) printf ("Return status from sec_pvtkey_pkss_open = 0x%x\n", estatus);
  (void) fflush (stdout);
#endif

  if (estatus != 0)
  {
    (void) printf ("Return status from sec_pvtkey_pkss_open = 0x%x;",
		   estatus);
    (void) printf (" expected %d\n", 0);
    (void) fflush (stdout);
    goto EXIT;
  }

  //
  // Generate new key pair for sec_pvtkey_pkss_update.
  //

  rand.len = 64;
  rand.data = (unsigned char *) malloc (rand.len);

  seed.len = rand.len;
  seed.data = (unsigned char *) malloc (seed.len);
  (void) memcpy (seed.data, &(seed.data), sizeof (seed.data));

#ifdef TRACE
  (void) printf ("Entering sec_bsafe_random_bytes\n");
  (void) fflush (stdout);
#endif

  estatus = sec_bsafe_random_bytes (&seed, &rand);

#ifdef TRACE
  (void) printf ("Return status from sec_bsafe_random_bytes = 0x%x\n", estatus);
  (void) fflush (stdout);
#endif

  if (estatus != 0)
  {
    (void) printf ("Return status from sec_bsafe_random_bytes = 0x%x;",
		   estatus);
    (void) printf (" expected %d\n", 0);
    (void) fflush (stdout);
    goto PVTKEY_CLOSE;
  }

#ifdef TRACE
  (void) printf ("Entering sec_bsafe_gen_pub_key\n");
  (void) printf ("  Key length (bits):  %d\n", pk_key_len);
  (void) fflush (stdout);
#endif

  estatus = sec_bsafe_gen_pub_key (pk_key_len, &rand, &kou, &kxu);

#ifdef TRACE
  (void) printf ("Return status from sec_bsafe_gen_pub_key = 0x%x\n", estatus);
  (void) fflush (stdout);
#endif

  if (estatus != 0)
  {
    (void) printf ("Return status from sec_bsafe_gen_pub_key = 0x%x;", estatus);
    (void) printf (" expected %d\n", 0);
    (void) fflush (stdout);
    goto PVTKEY_CLOSE;
  }

  //
  // Client change key pair; client supplies key pair.
  //

#ifdef TRACE
  (void) printf ("Entering sec_pvtkey_pkss_update.\n");
  (void) fflush (stdout);
#endif

  estatus = sec_pvtkey_pkss_update (user5_psm_context,
				    (unsigned char *)USER5_PASSWORD,
			            (unsigned char *)USER5_NEWPWD,
				    key_usages,
				    &ret_key_vno,
				    &kxu,
				    &kou);

#ifdef TRACE
  (void) printf ("Returned key version number = %d\n", ret_key_vno);
  (void) printf ("Return status from sec_pvtkey_pkss_update = 0x%x\n", estatus);
  (void) fflush (stdout);
#endif

  if (estatus == 0)
  {
    key_vno = ret_key_vno;
    ret_key_vno = 0;
  }

  else
  {
    (void) printf ("Return status from sec_pvtkey_pkss_update = 0x%x;",
		   estatus);
    (void) printf (" expected %d\n", 0);
    (void) fflush (stdout);
    goto PVTKEY_CLOSE;
  }

  //
  // Obtain asymmetric key pair for client chg, client generates key.
  //

#ifdef TRACE
  (void) printf ("Entering sec_pvtkey_pkss_get; ask for latest key version\n");
  (void) fflush (stdout);
#endif

  estatus = sec_pvtkey_pkss_get (user5_psm_context,
			    (unsigned char *)USER5_NEWPWD,
			    key_usages,
			    &ret_key_vno, // key version number
                            &ret_kxu,
			    &ret_kou);

#ifdef TRACE
  (void) printf ("Return status from sec_pvtkey_pkss_get = 0x%x\n", estatus);
  (void) printf ("Returned key version number = %d\n", ret_key_vno);
  (void) fflush (stdout);
#endif

  if (estatus != 0)
  {
    (void) printf ("Return status from sec_pvtkey_pkss_get = 0x%x;", estatus);
    (void) printf (" expected %d\n", 0);
    (void) fflush (stdout);
    goto PVTKEY_CLOSE;
  }

  if (ret_key_vno != key_vno)
  {
    (void) printf ("Return key version number = %d;", ret_key_vno);
    (void) printf (" expected %d\n", key_vno);
    (void) fflush (stdout);
    goto PVTKEY_CLOSE;
  }

  //
  // Compare the stored and retrieved public key components.
  //

#ifdef TRACE
  (void) printf ("Entering sec_pkss_pk_gen_data_cmp for kou/ret_kou\n");
  (void) fflush (stdout);
#endif

  estatus = sec_pkss_pk_gen_data_cmp ((sec_pk_gen_data_t *)&kou,
				      (sec_pk_gen_data_t *)&ret_kou);

#ifdef TRACE
  (void) printf ("Return status from sec_pkss_pk_gen_data_cmp = 0x%x\n",
		 estatus);
  (void) fflush (stdout);
#endif

  if (estatus)
  {
    (void) printf ("Return status from sec_pkss_pk_gen_data_cmp = 0x%x;",
		   estatus);
    (void) printf (" expected %d\n", 0);
    (void) fflush (stdout);

   if (gen_kou.len != 0)
   {
     (void) printf ("kou.len:  %d\n", kou.len);
     interpret_character_vector (kou.len,
                                 (char *)kou.data,
                                 &size,
                                 i_val,
                                 h_val);
     cds_ieu_emit ("kou",
                   (const char *)i_val,
                   (const char *)h_val);
   }
   (void) memset (i_val, 0, sizeof (i_val));
   (void) memset (h_val, 0, sizeof (h_val));
   (void) printf ("ret_kou.len:  %d\n", ret_kou.len);
   if (ret_kou.len != 0)
   {
     interpret_character_vector (ret_kou.len,
                                 (char *)ret_kou.data,
                                 &size,
                                 i_val,
                                 h_val);
     cds_ieu_emit ("ret_kou",
                   (const char *)i_val,
                   (const char *)h_val);
   }
   (void) memset (i_val, 0, sizeof (i_val));
   (void) memset (h_val, 0, sizeof (h_val));
   goto PVTKEY_CLOSE;
  }

  //
  // Client change key pair; PKSS supplies key pair.
  //

#ifdef TRACE
  (void) printf ("Entering sec_pvtkey_pkss_update_generate.\n");
  (void) fflush (stdout);
#endif

  estatus = sec_pvtkey_pkss_update_generate (user5_psm_context,
			            (unsigned char *)USER5_NEWPWD,
				    (unsigned char *)USER5_PASSWORD,
			            (sec_pk_algorithm_id_t *)0,
				    pk_key_len,
				    key_usages,
				    &ret_key_vno,
				    &ret_kou);

#ifdef TRACE
  (void) printf ("Return status from sec_pvtkey_pkss_update_generate = 0x%x\n",
		 estatus);
  (void) printf ("Returned key version number = %d\n", ret_key_vno);
  (void) fflush (stdout);
#endif

  if (estatus)
  {
    (void) printf ("Return status from sec_pvtkey_pkss_update_generate = 0x%x;",
		   estatus);
    (void) printf (" expected %d\n", 0);
    (void) fflush (stdout);
  }

  if (ret_key_vno != key_vno+1)
  {
    (void) printf ("Return key version number = %d;", ret_key_vno);
    (void) printf (" expected %d\n", key_vno+1);
    (void) fflush (stdout);
  }

FREE_RET_KXU_KOU:

  sec_pkss_zero_and_free (&ret_kxu);
  sec_pkss_zero_and_free (&ret_kou);

PVTKEY_CLOSE:

  (void) memset (rand.data, 0, rand.len);
  free (rand.data);
  rand.len = 0;

  (void) memset (seed.data, 0, seed.len);
  free (seed.data);
  seed.len = 0;

#ifdef TRACE
  (void) printf ("Entering sec_pvtkey_pkss_close\n");
  (void) fflush (stdout);
#endif

  pstatus = sec_pvtkey_pkss_close (user5_psm_context);

#ifdef TRACE
  (void) printf ("Return status from sec_pvtkey_pkss_close = 0x%x\n", pstatus);
  (void) fflush (stdout);
#endif

  if (pstatus)
  {
    (void) printf ("Return status from sec_pvtkey_pkss_close = 0x%x;", pstatus);
    (void) printf (" expected %d\n", 0);
    (void) fflush (stdout);
    estatus = pstatus;
  }

EXIT:

  (void) printf ("Exiting test_pkss_5; estatus = 0x%x\n\n", estatus);
  (void) fflush (stdout);

  return (estatus);

} // End main

