//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: sec_pkss_lits.h,v $
// Revision 1.1.2.1  1996/10/04  14:24:26  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/04  13:46:40  arvind]
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:30 UTC  arvind
// 	PKSS drop (2) from DEC (DCE1.2.2)
// 	[1996/09/04  14:32 UTC  arvind  /main/arvind_pkss/2]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
// PKSS utility functions prototypes and selected #defines required
// by the PKSS user client, management client, and server.
//

#ifndef SEC_PKSS_LITS_H
#define SEC_PKSS_LITS_H

extern "C"
{
#include <dce/nbase.h>
#include <sec_bsafe.h>
}

//
// PKSS server name - used to initialize PKSS configuration facility
//

#define PKSS_SERVER_NAME	"pkssd"
#define PKSS_CLIENT_NAME	"pkss-client"
#define PKSS_SVC_PROG_NAME "pks"


//
// List of supported symmetric encryption algorithms.  This should be changed
// to a configurable list of ASN.1 type AlgorithmIdentifier in a future
// revision.
//

#define PKSS_NUM_CRYPTO_ALGS	2
const unsigned32 pkss_master_alg_list[PKSS_NUM_CRYPTO_ALGS] = 
{
  sec_bsafe_enc_des,
  sec_bsafe_enc_des_ede3
};

#endif // SEC_PKSS_LITS_H
