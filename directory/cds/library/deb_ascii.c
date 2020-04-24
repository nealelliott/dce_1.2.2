/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: deb_ascii.c,v $
 * Revision 1.1.9.3  1996/02/18  19:32:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:13:12  marty]
 *
 * Revision 1.1.9.2  1995/12/08  15:21:05  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:44:50  root]
 * 
 * Revision 1.1.5.1  1994/10/17  20:06:25  jd
 * 	Aligne references to uuid_p in deb_ascii_uuid and deb_ascii_ObjUUID.
 * 	[1994/10/17  19:54:35  jd]
 * 
 * Revision 1.1.2.4  1994/08/03  19:03:03  mccann
 * 	includes file cleanup
 * 	[1994/07/19  18:09:38  mccann]
 * 
 * Revision 1.1.2.3  1994/06/23  18:30:45  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:08:48  mccann]
 * 
 * Revision 1.1.2.2  1994/06/09  18:40:44  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:13:09  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/29  15:51:55  peckham
 * 	Split off from deb_event_mgr.c
 * 	Remove VMS and _POSIX_THREADS conditionals.
 * 	Use dcecdsmac.h macros for dce_svc_printf().
 * 	Still have to remove SVR4 conditionals!!!!
 * 	[1994/04/29  13:59:42  peckham]
 * 
 * $EndLog$
 */
/*
 * Module: deb_ascii.c
 *
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1990-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 *
 * Module Description
 *
 * Debug interpreters for CDS
 *
 * This module contains routines used to support instrumented code
 * within CDS.  In particular the routines contained in this module
 * are aimed at providing a portable debugging event subsystem.
 *
 */

#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dce/assert.h>
#include <dce/rpc.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <dce/deb_ascii.h>
#include <dce/dnsclerk.h>
#include <dce/dns_record.h>
#include <dce/sys_time.h>
#include <string.h>
#include <stdio.h>
#include <string.h>

static const char to_hex[] = "0123456789ABCDEF";

/*
 *        Debugging Event Output Formatting Support Routines
 *
 * The following set of routines exist to support the creation of
 * debugging event traces.  Each routine handles a different DNS-internal
 * datatype and provides a string (ascii) interpretation of it.
 *
 * All routines accept as the first parameter a buffer into which the
 * ascii interpretation is written.  The buffer is assumed to be of
 * sufficient length to hold anything the routine can write -- there
 * are NO bounds checking.
 *
 * All routines return the address of the first parameter (work buffer).
 * This allows the routine to be called in-line with the debugging statement.
 *
 */

char *
deb_ascii_ptr_to_buf (char        *buf_p,
                      const long  *ptr)
{
    const long *const null = (void *)0;
    char *const ret_p = &buf_p[2+2*sizeof(long *)];
    int i = 2*sizeof(long *);

    buf_p[0] = '0';
    buf_p[1] = 'x';
    buf_p = ret_p;

    do {
	*--buf_p = to_hex[(ptr - null) & 0xF];
	ptr = null + ((ptr - null) >> 4);
    } while (--i != 0);
    ret_p[0] = '\0';

    return(ret_p);
}

/*
 * deb_ascii_errno - returns errno interpretation
 */

char *
deb_ascii_errno (char *const  buf,
                 const int    status,
                 const int    his_errno)
{
    char *msg_p;
    static const char format_unknown_errno[] = "unknown errno(%d)";
    static const char format_length[] = "wrong length(%d)";
    const int ast1 = (sizeof(format_unknown_errno) - 3 + DEB_ASCII_LEN_int);
    const int ast2 = DEB_ASCII_LEN_errno;
    const int ast3 = (sizeof(format_length) - 3 + DEB_ASCII_LEN_int);

    dce_assert(cds__svc_handle, ast1 <= ast2);

    dce_assert(cds__svc_handle, ast3 <= ast2);

    if (status == -1)
	if ((msg_p = (char *)strerror(his_errno)) != NULL)
	    return(msg_p);
	else
            (void)sprintf(buf, (char *)format_unknown_errno, his_errno);
    else
        (void)sprintf(buf, (char *)format_length, status);

    return(buf);
}

/*
 * deb_ascii_dns_status - returns an ascii dns_status interpretation
 */
#include <dns_status_map.c>

char *
deb_ascii_dns_status (char *const         buf,
                      const dns_status_t  status)
{
    register const dns_status_map_t *map_p;
    const int ast1 = DEB_ASCII_LEN_long;
    const int ast2 = DEB_ASCII_LEN_dns_status;

    for (map_p = dns_status_map; map_p->name_p; map_p++)
	if (map_p->value == status)
	{
	    return(map_p->name_p);
	}
    

    dce_assert(cds__svc_handle, ast1 <= ast2);

    (void)sprintf(buf, "%u", status);

    return(buf);
}

/*
 * deb_ascii_rec - catch-all for dns_record.h
 */

char *
deb_ascii_rec (char *const        buf,
               const void *const  ptr,
               const char *const  null_name)
{
    if (ptr) {
	(void)deb_ascii_ptr_to_buf(buf, ptr);
	return(buf);
    } 
    else
    {
	return((char *)null_name);
    }
    
}

/*
 * deb_ascii_bytes - returns an ascii hex interpretation
 */

char *
deb_ascii_bytes (char *const        buf,
                 const void *const  ptr,
                 int                len_in)
{
    const byte_t *ptr_in = (const byte_t *)ptr;

    if (ptr_in) {
	if (len_in) {
	    char *str_p = buf, *stop_p = &buf[DEB_ASCII_LEN_bytes-3];
	    const int asn = DEB_ASCII_LEN_bytes;

	    dce_assert(cds__svc_handle,5 <= asn);

	    do {
		if (stop_p < str_p) {
		    str_p[-2] = '.';
		    str_p[-1] = '.';
		    *str_p++ = '.';
		    break;
		}
		*str_p++ = to_hex[(*ptr_in>>4)&0x0F];
		*str_p++ = to_hex[(*ptr_in++)&0x0F];
	    } while (--len_in);

	    *str_p = 0;
	    return (buf);
	} 
	else
	{
	    return("*null string*");
	}
	
    } 
    else
    {
	return("NULL byte_t");
    }
}

/*---------------------------------------------------------------------------*/

/*
 * deb_ascii_BOOLEAN - returns an ascii BOOLEAN interpretation
 */

char *
deb_ascii_BOOLEAN (char *const        buf,
                   const void *const  ptr)
{
    const BOOLEAN_t *const bl_in = (const BOOLEAN_t *)ptr;
    static const char format_BOOLEAN[] = "*bad BOOLEAN(%d)*";
    const int a1 = (sizeof(format_BOOLEAN) - 3 + DEB_ASCII_LEN_byte);
    const int a2 = DEB_ASCII_LEN_BOOLEAN;

    dce_assert(cds__svc_handle, a1 <= a2);

    if (bl_in)
	switch (EXT8(bl_in)) {
	case BL_false:
	    return ("false");

	case BL_true:
	    return ("true");

	default:
	    (void)sprintf(buf, (char *)format_BOOLEAN, EXT8(bl_in));
	    return (buf);
	}
    else
    {
	return ("NULL BOOLEAN_t");
    }
    
}

/*---------------------------------------------------------------------------*/

/*
 * deb_ascii_time_quad - returns an ascii interpretation of a time buffer
 */

char *
deb_ascii_time_quad (char *const        buf,
                     const void *const  ptr)
{
    time_quad_t aligned_buffer;

    COPY_Time(ptr, &aligned_buffer);         /* Align for MIPS */
    sys_utc_to_string(&aligned_buffer, buf, DEB_ASCII_LEN_time_quad);

    /*
     * We're all done.
     * Return the address of the output buffer for convenience.
     */

    return (buf);
}

/*---------------------------------------------------------------------------*/

/*
 * deb_ascii_time_local - returns an ascii interpretation of a local time
 */

char *
deb_ascii_time_local (char *const        buf,
                      const void *const  ptr)
{
    time_local_t *const tl_p = (time_local_t *)ptr;

    sys_time_to_string(tl_p, buf, DEB_ASCII_LEN_time_local);

    /*
     * We're all done.
     * Return the address of the output buffer for convenience.
     */

    return (buf);
}

/*---------------------------------------------------------------------------*/

/*
 * deb_ascii_Timestamp - returns an ascii interpretation of a DNS timestamp
 */

char *
deb_ascii_Timestamp (char *const        buf,
                     const void *const  ptr)
{
    const Timestamp_t *const ts_in = (const Timestamp_t *)ptr;

    if (ts_in) {
	char ascii_node_address[20];
	time_quad_t temp_time;           /* Temp. storage for alignment */
	const int a1 = (DEB_ASCII_LEN_time_quad+18);
	const int a2 = DEB_ASCII_LEN_Timestamp;

	dce_assert(cds__svc_handle, a1 <= a2);

	(void)sprintf(ascii_node_address, "%02X-%02X-%02X-%02X-%02X-%02X",
            ts_in->ts_node[0],
            ts_in->ts_node[1],
            ts_in->ts_node[2],
            ts_in->ts_node[3],
            ts_in->ts_node[4],
            ts_in->ts_node[5]
           );
	COPY_Time(ts_in->ts_time, &temp_time);
	sys_utc_to_string(&temp_time, buf, DEB_ASCII_LEN_time_quad);
	(void)strcat(buf, "/");
	(void)strcat(buf, ascii_node_address);
	return (buf);
    } 
    else
    {
	return("NULL Timestamp_t");
    }
    
}

/*
 * deb_ascii_ObjUID - returns an ascii interpretation of an id.  For CDS this
 * is a uuid
 */

char *
deb_ascii_ObjUID (char *const        buf,
                  const void *const  ptr)
{
  const ObjUID_t *const id_in = (const ObjUID_t *)ptr;
  uuid_t id;

  if (id_in) {
    COPY_bytes(id_in,&id,sizeof(id));
    if (EQ_bytes(&id, cdsWildRoot, sizeof(cdsWildRoot)))
      {
	  return("wild_ObjUID");
      }
      
    else {
      unsigned32 status;
      unsigned_char_t *string;


      uuid_to_string(&id, &string, &status);
      if (status == uuid_s_ok) 
      {
	  const int a1 = strlen((char *)string);
	  const int a2 = DEB_ASCII_LEN_ObjUID;
	  
	  dce_assert(cds__svc_handle, a1 <= a2);

	  (void)strcpy(buf, (char *)string);
	  rpc_string_free(&string, &status);
	  return(buf);
      } 
      else
      {
	  return("*bad ObjUID*");
      }
      
    }
  }
  else
  {
      return("NULL ObjUID_t");
  }
    
}

/*---------------------------------------------------------------------------*/

/*
 * deb_ascii_EntryType - returns an ascii EntryType interpretation
 */

char *
deb_ascii_EntryType (char *const        buf,
                     const void *const  ptr)
{
    static const char format_EntryType[] = "*bad EntryType(%d)*";
    const EntryType_t *const et_in = (const EntryType_t *)ptr;
    const int a1 = (sizeof(format_EntryType) - 3 + DEB_ASCII_LEN_byte);
    const int a2 = DEB_ASCII_LEN_EntryType;

    dce_assert(cds__svc_handle, a1 <= a2);

    if (et_in)
	switch (EXT8(et_in)) {
	case ET_directory:
	    return ("directory");
	case ET_object:
	    return ("object");
	case ET_childPointer:
	    return ("childPointer");
	case ET_softlink:
	    return ("softlink");
	case ET_clearinghouse:
	    return ("clearinghouse");
	case ET_anyDirectName:
	    return ("anyDirectName");
	case ET_dirOrObj:
	    return ("dirOrObj");
	default:
	    (void)sprintf(buf, (char *)format_EntryType, EXT8(et_in));
	    return (buf);
	}
    else
    {
	return ("NULL EntryType_t");
    }
    
}

/*---------------------------------------------------------------------------*/

/*
 * deb_ascii_SimpleName - returns an ascii interpretation
 *  	                  of a DNS SimpleName
 */

char *
deb_ascii_SimpleName (char *const        buf,
                      const void *const  ptr)
{
    const SimpleName_t *const sn_in = (const SimpleName_t *)ptr;
    int strLen = DEB_ASCII_LEN_SimpleName;
    const int a1 = CDS_STR_SNAME_MAX;
    const int a2 = DEB_ASCII_LEN_SimpleName;

    dce_assert(cds__svc_handle,a1 <= a2);

    if (sn_in) {
	if (cdsCvtSimpleToStr((cds_simple_name_t *)sn_in,
				 (unsigned char *)buf, &strLen) != DNS_SUCCESS)
	{
	    return ("*bad SimpleName*");
	}
	return (buf);
    } 
    else
    {
	return ("NULL SimpleName_t");
    }
    
}

/*---------------------------------------------------------------------------*/

/*
 * deb_ascii_AttributeName - returns an ascii interpretation
 *    	                     of a DNS AttributeName
 */

char *
deb_ascii_AttributeName (char *const        buf,
                         const void *const  ptr)
{
    const AttributeName_t *const sn_in = (const AttributeName_t *)ptr;
    const int a1 = CDS_STR_SNAME_MAX;
    const int a2 = DEB_ASCII_LEN_AttributeName;

    dce_assert(cds__svc_handle,a1 <= a2);
    
    if (sn_in) 
    {
	int strLen = DEB_ASCII_LEN_AttributeName;

	if ((cdsCvtObjIDToStr((cds_attr_name_t *)sn_in, (byte_t *)0,
		(byte_t *)buf, &strLen,
			      (byte_t *)0, (int *)0) == DNS_SUCCESS) && strLen)
	{
	    return (buf);
	}
	

	strLen = DEB_ASCII_LEN_AttributeName;
	if ((cdsCvtObjIDToStr((cds_attr_name_t *)sn_in, (byte_t *)0,
		(byte_t *)0, (int *)0,
		(byte_t *)buf, &strLen) == DNS_SUCCESS) && strLen)
	{
	    return (buf);
	}

	strLen = DEB_ASCII_LEN_AttributeName;
	if (cdsCvtSimpleToStr((cds_simple_name_t *)sn_in,
		(byte_t *)buf, &strLen) == DNS_SUCCESS)
	{
	    return (buf);
	}
	return ("*bad AttributeName*");

    } 
    else
    {
	return ("NULL AttributeName_t");
    }
    
}

/*---------------------------------------------------------------------------*/

/*
 * deb_ascii_FullName - returns an ascii interpretation of a DNS FullName
 */

char *
deb_ascii_FullName (char *const        buf,
                    const void *const  ptr)
{
    const FullName_t *const fn_in = (const FullName_t *)ptr;
    const int a1 = CDS_STR_FNAME_MAX-1;
    const int a2 = DEB_ASCII_LEN_FullName;

    dce_assert(cds__svc_handle,a1 <= a2);

    if (fn_in) 	
    {
	char *str_p = buf;
	int strLen = DEB_ASCII_LEN_FullName;

	if (NE_ObjUID(fn_in->fn_root, cdsWildRoot)) {
	    (void)deb_ascii_ObjUID(str_p, (ObjUID_t *)fn_in->fn_root);
	    if (EXT16(fn_in->fn_length) <= sizeof(NullSimpleName))
	    {
		return(buf);
	    }
	    
	    str_p = strchr(str_p, '\0');
	    *str_p++ = ':';
	    strLen -= str_p - buf;
	}
	if (cdsCvtFullToStr((cds_full_name_t *) fn_in,
			       (unsigned char *)str_p, &strLen) == DNS_SUCCESS)
	{
	    return (buf);
	}
	return ("*bad FullName*");
    } 
    else
    {
	return ("NULL FullName_t");
    }
    
}

/*---------------------------------------------------------------------------*/

/*
 * deb_ascii_AttributeType -  Returns an ascii AttributeType interpretation
 */

char *
deb_ascii_AttributeType (char *const        buf,
                         const void *const  ptr)
{
    const AttributeType_t *const at_in = (const AttributeType_t *)ptr;
    static const char format_AttributeType[] = "*bad AttributeType(%d)*";
    const int a1 = (sizeof(format_AttributeType) - 3 + DEB_ASCII_LEN_byte);
    const int a2 = DEB_ASCII_LEN_AttributeType;

    dce_assert(cds__svc_handle,a1 <= a2);

    if (at_in)
	switch (EXT8(at_in)) {
	case AT_null:
	    return ("null");
	case AT_single:
	    return ("single");
	case AT_set:
	    return ("set");
	default:
	    (void)sprintf(buf, (char *)format_AttributeType, EXT8(at_in));
	    return (buf);
	}
    else
    {
	return ("NULL AttributeType_t");
    }
    
}
/*---------------------------------------------------------------------------*/

/*
 * deb_ascii_uuid - returns an ascii interpretation of a uuid.
 */

char *
deb_ascii_uuid (char *const        buf,
                const void *const  ptr)
{
  uuid_t *const id_in = (uuid_t *)ptr;
  uuid_t id;


  if (id_in) 
  {
    unsigned_char_t *string;
    unsigned32 status;

  
    COPY_bytes(id_in,&id,sizeof(id));
    uuid_to_string(&id, &string, &status);
    if (status == uuid_s_ok) 
    {
	int a1,a2; /* assertions */


	a1 = strlen((char *)string);
	a2 = DEB_ASCII_LEN_uuid;
	dce_assert(cds__svc_handle,a1 <= a2);

	(void)strcpy(buf, (char *)string);
	rpc_string_free(&string, &status);

	return(buf);
    } 
    else
    {
	return("*bad uuid*");
    }
  }
  else
  {
      return("NULL uuid_t");
  }

}

# if defined(DCE_SEC)
/*
 * deb_ascii_seclogin - Returns ascii representation of an opaque,
 * secure login context. The context is currently a printable string
 * (filename), preceded by a 2-byte length.
 */

char *
deb_ascii_seclogin (char *const        buf,
                    const void *const  ptr)
{
    const byte_t *const opq_login = (const byte_t *)ptr;

    if (opq_login) {
	const int nameLen = EXT16(opq_login);

	if (0 < nameLen) {
	    char *const name_p = (char *)opq_login+sizeof(field16);
	    int a1,a2; /* assertions */


	    a1 = strlen(name_p);
	    dce_assert(cds__svc_handle, a1 < nameLen);

	    a2 = DEB_ASCII_LEN_seclogin;
	    dce_assert(cds__svc_handle,nameLen <= a2);

	    (void)strncpy (buf, name_p, nameLen);

	    return (buf);
	} 
	else
	{
	    return ("NO_USER");
	}
	
    } 
    else
    {
	return ("*ANONYMOUS*");
    }
    

}

# endif /* DCE_SEC */
