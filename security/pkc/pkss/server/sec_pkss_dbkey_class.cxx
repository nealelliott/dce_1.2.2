//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved
//
// HISTORY
// $Log: sec_pkss_dbkey_class.cxx,v $
// Revision 1.1.2.1  1996/10/03  20:31:09  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/03  19:29:36  arvind]
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:30 UTC  arvind
// 	PKSS drop from DEC (DCE1.2.2)
// 	[1996/08/30  15:53 UTC  arvind  /main/arvind_pkss/1]
// 
// $EndLog$
//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
// Module: sec_pkcc_dbkey_class.cxx
//
// Purpose: Implements pkss database key methods.
//

#include <sys/types.h>
#include <netinet/in.h>
#include <dce/nbase.h>
#include <string.h>
#include <dce/sec_pk_base.h>
#include "sec_pkss_dbkey_class.h"
#include "sec_pkss_util.h"
#include <dce/dcepksmsg.h>
#include <dce/dcepkssvc.h>  
#include <dce/dcepksmac.h> 

//
// Constructor - use raw data
//

PkssDbKeyC::PkssDbKeyC (const char *name_p,
			const char *domain_p,
			sec_pk_usage_flags_t usages,
			unsigned32 version)

{
  nameLen = (unsigned16)strlen (name_p);

  dbKey.len = sizeof (nameLen) +
	      nameLen +
	      DOMAIN_STRING_SIZE +
	      sizeof (sec_pk_usage_flags_t) +
	      sizeof (unsigned32);

  dbKey.data = new unsigned char [dbKey.len]; 

  if (!(dbKey.data))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
    return;
  }

  //
  // Insert the length of the name string into the database key.  Recall
  // that all integer data is in network byte order in PKSS database records
  // and keys.
  //

  unsigned16 htons_name_len = htons (nameLen);

  unsigned32 i = 0;
  (void) memcpy (dbKey.data, &htons_name_len, sizeof (htons_name_len));

  i += sizeof (htons_name_len);

  //
  // Insert the user's name.
  //

  (void) memcpy (&(dbKey.data[i]), name_p, nameLen);
  i += nameLen;

  //
  // Insert the key domain ID.  Its length is the length of the string
  // representation of a uuid, thus it is constant, and it is not necessary
  // to store its length.  But use strncpy rather than memcpy so as to
  // truncate or zero-extend any domain string we're passed that isn't the
  // right length.
  //

  (void) strncpy ((char *)&(dbKey.data[i]),
		  domain_p,
		  DOMAIN_STRING_SIZE);

  i += DOMAIN_STRING_SIZE;

  keyUsages = usages;
  usages = htonl (keyUsages);
  (void) memcpy (&(dbKey.data[i]), &usages, sizeof (usages));

  //
  // Insert the key version number if supplied by PKSS mgmt client.
  //

  keyVno = version;
  version = htonl (keyVno);
  i += sizeof (usages);
  (void) memcpy (&(dbKey.data[i]), &version, sizeof (version));

  //
  // Keep copy of name as a string.
  //

  nameP = new char [nameLen+1]; 
  (void) memcpy (nameP, (const char *)&(dbKey.data[2]), nameLen);
  nameP[nameLen] = 0;

} // End PkssDbKeyC::PkssDbKeyC


//
// Copy constructor - use a pointer to data and a length; most likely
//		      to be used in conjunction with gdb44 methods.
//

PkssDbKeyC::PkssDbKeyC (const unsigned char *key_data_p,
			unsigned32 key_data_len)
{
  dbKey.len = key_data_len;
  dbKey.data = new unsigned char [dbKey.len]; 

  if (!(dbKey.data))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
  }

  (void) memcpy (dbKey.data, key_data_p, dbKey.len);
  keyVno = getKeyVnoFromKeyData();
  keyUsages = getUsagesFromKeyData();
  nameLen = ntohs (*(unsigned16 *)(&dbKey.data[0]));
  nameP = new char [nameLen+1]; 
  (void) memcpy (nameP, (const char *)&(dbKey.data[2]), nameLen);
  nameP[nameLen] = 0;

} // End PkssDbKeyC::PkssDbKeyC (unsigned char *, unsigned32)


//
// Copy constructor - instantiate a new instance of PkssDbKeyC from an
//		      existing one.
//

PkssDbKeyC::PkssDbKeyC (PkssDbKeyC *oldKeyP)
{
  dbKey.len = oldKeyP->dbKey.len; 
  dbKey.data = new unsigned char [dbKey.len]; 

  if (!(dbKey.data))
  {
    dce_svc_printf(PKS_S_NO_MEMORY_MSG);
  }

  (void) memcpy (dbKey.data, oldKeyP->dbKey.data, dbKey.len);
  keyVno = getKeyVnoFromKeyData();
  keyUsages = getUsagesFromKeyData();
  nameLen = ntohs (*(unsigned16 *)(&dbKey.data[0]));
  nameP = new char [nameLen+1]; 
  (void) memcpy (nameP, (const char *)&(dbKey.data[2]), nameLen);
  nameP[nameLen] = 0;

} // End PkssDbKeyC::PkssDbKeyC (sec_pkss_dbkey_t *)


//
// PkssDbKeyC class destructor - note that it zeroes contents before
// deleting.
//

PkssDbKeyC::~PkssDbKeyC ()
{
  (void) memset (dbKey.data, 0, dbKey.len);
  delete [] dbKey.data;
  dbKey.len = 0;
  dbKey.data = 0;
  (void) memset (nameP, 0, nameLen);
  delete [] nameP;
  nameP = 0;
  nameLen = 0;
  keyVno = 0;
  keyUsages = 0;

} // End PkssDbKeyC::~PkssDbKeyC


unsigned16 PkssDbKeyC::getNameLen (void)
{
  return (nameLen);

} // End PkssDbKeyC::getNameLen (void)


unsigned32 PkssDbKeyC::getKeyVno (void)
{
  return (keyVno);

} // End PkssDbKeyC::getKeyVno (void)


sec_pk_usage_flags_t PkssDbKeyC::getUsages (void)
{
  return (keyUsages);

} // End PkssDbKeyC::getUsages (void)


const sec_pkss_dbkey_t * PkssDbKeyC::p (void)
{
  return ((const sec_pkss_dbkey_t *)&dbKey);

} // End PkssDbKeyC::p (void)


const char * PkssDbKeyC::getName (void)
{
  return ((const char *)nameP);

} // End PkssDbKeyC::getName (void)


void PkssDbKeyC::setKeyVno (unsigned32 newVno)
{
  keyVno = newVno;
  unsigned char *version_p = getKeyVnoPos();
  newVno = htonl (newVno);
  (void) memcpy (version_p, &newVno, sizeof (unsigned32));

} // End PkssDbKeyC::setKeyVno


void PkssDbKeyC::setUsages (sec_pk_usage_flags_t newUsages)
{

  keyUsages = newUsages;
  unsigned char *usages_p = getUsagesPos();
  newUsages = htonl (newUsages);
  (void) memcpy (usages_p, &newUsages, sizeof (sec_pk_usage_flags_t));

} // End PkssDbKeyC::setUsages


sec_pk_usage_flags_t PkssDbKeyC::getUsagesFromKeyData (void)
{
  unsigned char *usages_p = getKeyVnoPos();
  return (ntohl (*(sec_pk_usage_flags_t *)usages_p));

} // End PkssDbKeyC::getUsagesFromKeyData (void)


unsigned32 PkssDbKeyC::getKeyVnoFromKeyData (void)
{
  unsigned char *version_p = getKeyVnoPos();
  return (ntohl (*(unsigned32 *)version_p));

} // End PkssDbKeyC::getKeyVnoFromKeyData (void)


unsigned char *PkssDbKeyC::getKeyVnoPos(void)
{
  unsigned char *version_p;
  version_p = dbKey.data;
  version_p += dbKey.len - sizeof (unsigned32);
  return (version_p);

} // End PkssDbKeyC::getKeyVnoPos(void)


unsigned char *PkssDbKeyC::getUsagesPos(void)
{
  unsigned char *usages_p;
  usages_p = dbKey.data;
  usages_p += sizeof (nameLen) + nameLen + DOMAIN_STRING_SIZE;
  return (usages_p);

} // End PkssDbKeyC::getUsagesPos(void)

