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
 * $Log: cds_help_a.c,v $
 * Revision 1.1.2.1  1996/11/14  16:18:17  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/14  14:32:13  arvind]
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
 * ABSTRACT:    Testing module. 
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


#include <stdio.h>
#include <strings.h>
#include <memory.h>
#include <dce/dce.h>
#include <dce/cdsclerk.h>

static int reset_flags (cdsFlagStat_t *flags, 
                  cds_full_name_t *partial);


int delete_dir (char *dir);

int reset_flags (cdsFlagStat_t *flags, 
                  cds_full_name_t *partial)
{
  memset (flags, 0, sizeof (struct cdsFlagStat));
  flags->fsMaybeMore = 1;
  flags->fsConf = cdsConfLow;
  flags->fsNameType = cdsStrDCE;
  flags->fsUnAuth = 0;
  flags->fsTrustAll = 0;
  flags->fsPartialRes = partial;
  return 0;
}
   

int delete_dir (char *dir)
{

  cds_full_name_t name;
  cds_full_name_t partial;
  cdsFlagStat_t flags;
  int ret_value;

  if (strlen(dir) > CDS_FNAME_MAX) {
    return -1;
  }

  strcpy((char *) name.uc, dir);
   
  if (reset_flags(&flags, &partial)) {
    return -1;
  }

  ret_value = cdsDelDir(&name, &flags);
  if (ret_value == CDS_SUCCESS) {
    return ret_value;
  }

  if (flags.fsLocStat != 0)
    return (flags.fsLocStat);

  if (flags.fsCDSStat  != 0)
    return (flags.fsCDSStat);

  return -1;
}
    
int create_dir (char *dir)
{
  cds_full_name_t name;
  cds_full_name_t partial;
  cdsFlagStat_t flags;
  int ret_value;

  if (strlen(dir) > CDS_FNAME_MAX) {
    return -1;
  }

  strcpy((char *) name.uc, dir);
   
  if (reset_flags(&flags, &partial)) {
    return -1;
  }

  ret_value = cdsCreateDir(&name, 0, 0, &flags);
  if (ret_value == CDS_SUCCESS) {
    return ret_value;
  }

  if (flags.fsLocStat != 0)
    return (flags.fsLocStat);

  if (flags.fsCDSStat  != 0)
    return (flags.fsCDSStat);

  return -1;
}

int create_attr_value (char *dir, char *attr, unsigned char *in_value, size_t  len)
{
  cds_full_name_t name;
  cds_full_name_t partial;
  cds_attr_name_t cattr;
  cds_attrvalue_t value;

  cdsFlagStat_t flags;
  int ret_value;

  if (strlen(dir) > CDS_FNAME_MAX) {
    return -1;
  }

  strcpy((char *) name.uc, dir);
   
  if (reset_flags(&flags, &partial)) {
    return -1;
  }

  strcpy((char *) cattr.uc, attr);
  value.val_syntax = cds_byte;
  value.val_u.val_byte.length = len;
  value.val_u.val_byte.byte_p = (void *) in_value;
  ret_value = cdsAddAttrValue(&name, &cattr,  cdsSet,  cdsDir, &value, 0, &flags);
  if (ret_value == CDS_SUCCESS) {
    return ret_value;
  }

  if (flags.fsLocStat != 0)
    return (flags.fsLocStat);

  if (flags.fsCDSStat  != 0)
    return (flags.fsCDSStat);

  return -1;

  
}

int create_attr_value_from_file (char *dir, char *attr, char *fname)
{
  char octetstring_value[5000];
  FILE *file;
  size_t os_length; int c;
  int status;

  if (!fname)
    return -1;

  file = fopen (fname, "rb");
  if (!file)
    return -1;

  os_length = 0;
  while  ((c = fgetc(file)) != EOF) {
    octetstring_value[os_length] = (char) c;
    os_length++;
    if (os_length > sizeof(octetstring_value)) {
      fclose (file);
      return -1;
    };
  }

  fclose (file);

  status = create_attr_value (dir, attr, octetstring_value, os_length);

  if (status == CDS_SUCCESS)
    return 0;

  return -1;
}
  
