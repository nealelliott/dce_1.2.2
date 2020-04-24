//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: sec_pkss_db.h,v $
// Revision 1.1.2.1  1996/10/04  14:23:49  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/04  13:46:10  arvind]
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:30 UTC  arvind
// 	PKSS drop from DEC (DCE1.2.2)
// 	[1996/08/30  15:52 UTC  arvind  /main/arvind_pkss/1]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//

#ifndef SEC_PKSS_DB_H
#define SEC_PKSS_DB_H

#include <dce/nbase.h>
#include "gdb44_methods.h"

//
// Pointer to PKSS database object
//

#ifndef PKSSD_MAIN
extern
#endif
 Gdb44C *pkss_db_p;


//
// PKSS database record key - contains Username:domain_id:usage_flags:version
//			      in a buffer consisting in a length field and a
//			      data field.

typedef struct
{
  unsigned32 len;
  unsigned char *data;
} sec_pkss_dbkey_t;

//
// PKSS database record - contains ASN.1 encoding of [Kxu~]H1~:H2~:Kou~
//			  in a buffer consisting in a length and a data field.

typedef struct
{
  unsigned32 asn_rec_len;
  unsigned char *asn_rec_p;
} sec_pkss_dbrec_t;

#endif // SEC_PKSS_DB_H
