//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: sec_pkss_dbkey_class.h,v $
// Revision 1.1.2.1  1996/10/04  14:24:02  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/04  13:46:21  arvind]
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:30 UTC  arvind
// 	PKSS drop (2) from DEC (DCE1.2.2)
// 	[1996/09/04  14:31 UTC  arvind  /main/arvind_pkss/2]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
// Module: sec_pkss_dbkey_class.h
//
// Purpose: class defs for pkss database key class
//

#ifndef SEC_PKSS_DBKEY_CLASS_H
#define SEC_PKSS_DBKEY_CLASS_H

extern "C"
{
#include <dce/sec_pk_base.h>
}
#include "sec_pkss_db.h"

class PkssDbKeyC
{
#define DOMAIN_STRING_SIZE (36)
public:
  PkssDbKeyC (const char *,
	     const char *,
	     sec_pk_usage_flags_t,
	     unsigned32);
  PkssDbKeyC (const unsigned char *, unsigned32);
  PkssDbKeyC (PkssDbKeyC *);
  ~PkssDbKeyC ();
  unsigned16 getNameLen(void);
  unsigned32 getKeyVno (void);
  sec_pk_usage_flags_t getUsages (void);
  const sec_pkss_dbkey_t *p(void);
  const char *getName(void);
  void setKeyVno (unsigned32 newVno);
  void setUsages (sec_pk_usage_flags_t newUsages);
private:
  sec_pkss_dbkey_t dbKey;
  char *nameP;
  unsigned16 nameLen;
  unsigned32 keyVno;
  sec_pk_usage_flags_t keyUsages;
  unsigned32 getKeyLen(void);
  unsigned char *getKeyVnoPos(void);
  unsigned char * getUsagesPos(void);
  unsigned32 getKeyVnoFromKeyData(void);
  sec_pk_usage_flags_t getUsagesFromKeyData (void);
};

#endif // SEC_PKSS_DBKEY_CLASS_H
