/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: uc_parse.h,v $
 * Revision 1.1.14.1  1996/10/03  14:35:09  arvind
 * 	Quiet purify.
 * 	[1996/09/16  19:58 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.12.2  1996/02/18  23:34:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:23:49  marty]
 * 
 * Revision 1.1.12.1  1995/12/08  15:18:45  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:44:04  root]
 * 
 * Revision 1.1.9.7  1994/09/19  19:21:56  mccann
 * 	return security errors OT7950
 * 	[1994/09/19  19:21:26  mccann]
 * 
 * Revision 1.1.9.6  1994/08/03  19:02:53  mccann
 * 	include file cleanup
 * 	[1994/07/19  18:01:36  mccann]
 * 
 * Revision 1.1.9.5  1994/06/30  19:15:47  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  18:50:24  mccann]
 * 
 * Revision 1.1.9.4  1994/06/09  18:39:45  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:12:27  devsrc]
 * 
 * Revision 1.1.9.3  1994/04/29  15:50:25  peckham
 * 	Fix macros to not use return statements. Instead exit loop.
 * 	This eliminates PRM_sendrecv() and allows single exit in callers.
 * 	Eliminate obsolete size argument to PRM_build().
 * 	PRM_send() places status result in passed argument instead
 * 	of hidden PRM_status as PRM_sendrecv() did.
 * 	[1994/04/29  13:57:37  peckham]
 * 
 * Revision 1.1.9.2  1994/04/14  14:39:53  peckham
 * 	Split decnet_types into machine dependent cds_basic_types.h
 * 	Removed VMS, UNIX and DOSorOS2 conditionals.
 * 	[1994/04/11  19:58:15  peckham]
 * 
 * Revision 1.1.9.1  1994/03/12  22:04:48  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:21:20  peckham]
 * 
 * Revision 1.1.7.1  1993/09/24  16:55:58  peckham
 * 	OT#8390: Purge unused status codes
 * 	    Removed PC-oriented code (all the MRP_*() macros) which accessed
 * 	    status codes which have never and will never been used.
 * 	[1993/09/24  16:53:11  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:51:41  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:55:51  zeliff]
 * 
 * Revision 1.1.2.3  1992/07/06  20:14:47  mfox
 * 	Limit classnames to 31 characters to prevent server crash
 * 	[1992/07/06  20:11:23  mfox]
 * 
 * Revision 1.1.2.2  1992/03/22  22:13:25  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:29:33  weisman]
 * 
 * Revision 1.1  1992/01/19  15:17:12  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
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
 */
#ifndef _UC_PARSE_H
#define _UC_PARSE_H

#include <string.h>
#include <dce/dns_config.h>
#include <dce/dnsclerk.h>
#include <dce/dns_record.h>

#ifdef DCE_SEC
# include <dce/sec_login.h>

/*
 * prm_sec.c
 */

int
prm_login_context (
    sec_login_handle_t	*,
    dnsFlagStat_t	*,
    byte_t		*,
    int			);

#endif

/*
 * ******  W A R N I N G  *****
 *
 * If you change the library-child protocol, increment the UCP_VERSION!
 * If you don't, the version check (in clerk_client.c) won't catch a
 * mismatched library and child, and the child will probably crash
 * in strange and wonderful ways.
 */
#define UCP_VERSION 9

/*
 * the request message has the following header on it
 *
 * Keep things aligned and speed things up.
 */

typedef struct {
    unsigned int	length;		/* must be first */
    unsigned int	sequence;
    unsigned int	wait;
    unsigned char	op;		/* dont't move this */
    unsigned char	version;	/* dont't move this */
    unsigned char	dontCache;
    unsigned char	conf;
    void		*handle;
    void		*partialResults;
    void		*resLen_p;
    void		*fromCache_p;
    void		*outLink_p;
    uid_t		user_uid;
    gid_t		user_gid;
    unsigned int	resLen;
    unsigned int	cacheTimeout;
    unsigned char	UnAuth;
    unsigned char	TrustAll;
} ucp_rqst_header;

/*
 * the response message has the following header on it
 */

typedef struct {
    unsigned int	length;		/* must be first */
    unsigned int	sequence;
    void		*handle;
    unsigned int	status;
} ucp_resp_header;

/*
 * optimized native protocol has an aligned header
 */
#if 0

typedef struct {
    void		*address;	/* user's data address */
    u_short		length;		/* length of following data */
    u_short		return_length;	/* length of buffer for return data */
} ucp_prm_header;

#else

typedef struct {
    address_u		address;	/* user's data address */
    word_u		length;		/* length of following data */
} ucp_prm_header;

#endif

/*
 * We pass addresses and paramter lengths in the packets
 * Here are the definitions used in ensuing code...
 */

#define UCP_address_LEN \
    ((sizeof(address_u) <= sizeof(field32)) ? sizeof(field32) : sizeof(field64))
#define INSaddress(ptr,val) { if (sizeof(address_u) <= sizeof(field32)) \
    { INS32(ptr,(field32)(val)); } else { INS64(ptr,(val)); } }
#define PUTaddress(ptr,val) { if (sizeof(address_u) <= sizeof(field32)) \
    { PUT32(ptr,(field32)(val)); } else { PUT64(ptr,(val)); } }
#define EXTaddress(ptr) (address_t *)((sizeof(address_u) <= sizeof(field32)) \
     ? EXT32(ptr) : EXT64(ptr))
#define GETaddress(ptr) (address_t *)((sizeof(address_u) <= sizeof(field32)) \
    ? (ptr += sizeof(field32), EXT32(ptr - sizeof(field32))) : GET64(ptr))
#define PUT_address(val) PUTaddress(ucp_ptr,(val))

#define PRM_buffer_LEN (UCP_address_LEN+sizeof(field16))

/*
 * Header access - in time of transition
 */

#define UCP_INSbyte(ptr,val) (ptr)=(val)
#define UCP_INSlongword(ptr,val) (ptr)=(val)
#define UCP_INSaddress(ptr,val) (ptr)=(val)

#define UCP_EXTbyte(ptr) (ptr)
#define UCP_EXTlongword(ptr) (ptr)
#define UCP_EXTaddress(ptr) (ptr)

/*
 * parameter building
 */

#define	PRM_begin() { field8 *ucp_sav; \
    if (ucp_sav = ucp_ptr) ucp_ptr += sizeof(field16); else ucp_max += sizeof(field16)

#define	PRM_byte(arg) if (ucp_ptr) {PUT8(ucp_ptr,arg);} else ++ucp_max


#define	PRM_vbyte(arg) 						\
    if (ucp_ptr) 						\
    { 								\
      int ucp_vbval = arg;					\
	                                                        \
      if (ucp_vbval < 0) 					\
      {								\
	ucp_status = DNS_INVALIDARGUMENT;			\
	break;							\
      }								\
      else							\
	PUT8(ucp_ptr,ucp_vbval); 				\
    }								\
    else							\
      ++ucp_max

#define	PRM_word(arg) if (ucp_ptr) {PUT16(ucp_ptr,(arg));} else ucp_max += 2

#define	PRM_longword(arg) if (ucp_ptr) {PUT32(ucp_ptr,arg);} else ucp_max += 4

#define	PRM_bytes(arg,len,max) if (ucp_ptr) \
    { COPY_bytes(arg,ucp_ptr,len); ucp_ptr += len; \
    } else ucp_max += len


#define	PRM_lbytes(arg,len,max) 				\
    if (ucp_ptr) 						\
    {								\
      unsigned char *ucp_lbadr = arg; 				\
      int ucp_lblen = (len);					\
                                                                \
      if (!ucp_lbadr) 						\
      {								\
	ucp_status = DNS_INVALIDARGUMENT;		 	\
	break;							\
      }								\
      if (ucp_lblen)	 					\
	PRM_bytes(ucp_lbadr,ucp_lblen,max);	 		\
    } 								\
    else 							\
      ucp_max += len


#define	PRM_mbytes(arg,len,max)					\
    if (ucp_ptr) 						\
    { 								\
      int ucp_mblen = (len); 					\
	                                                        \
      if (max < ucp_mblen) 		            		\
      {								\
	ucp_status = DNS_INVALIDARGUMENT;		 	\
	break;							\
      }								\
      if (ucp_mblen) 						\
	PRM_bytes(arg,ucp_mblen,max); 				\
    }                                                           \
    else 							\
      ucp_max += len


#define	PRM_end() if (ucp_ptr) INS16(ucp_sav,(ucp_ptr-&ucp_sav[2])); }

#define	PRM_length(len) if (ucp_ptr) {PUT16(ucp_ptr,(len));} else ucp_max += sizeof(field16)

#define	PRM_buffer(adr,len) if (ucp_ptr) { \
    PUT_address(adr); PUT16(ucp_ptr,(len)); } else ucp_max += PRM_buffer_LEN

/*
 * Set building
 */

#define	PRM_Set_begin(cnt) { field8 *ucp_setb, *ucp_seto, *ucp_setd; \
    if (ucp_setb = ucp_ptr) { \
    INS8(ucp_ptr,(cnt)?1:0); INS16(&ucp_ptr[1], cnt<<1); \
    ucp_seto = &ucp_ptr[5]; ucp_setd = ucp_ptr = &ucp_seto[cnt<<1]; } \
    else ucp_max += (5 + (cnt<<1))

#define	PRM_Set_member(flg,ts) if (ucp_ptr) \
    { PUT16(ucp_seto, ucp_ptr-ucp_setb); PRM_byte(flg); PRM_Timestamp(ts); } \
    else ucp_max += sizeof(SetMember_u)

#define	PRM_Set_end() \
    if (ucp_ptr) INS16(&ucp_setb[3], ucp_ptr - ucp_setd); }

/*
 * needed parameter checks
 */
#define	PRM_AtomicValue(type, adr,len) PRM_word(len); PRM_byte(type); PRM_mbytes(adr,len,MAXATTRIBUTE)

#define	PRM_AttributeName(arg) PRM_lbytes(dns_validate_sname(arg), \
    LEN_SimpleName(arg),2+ATTRIBNAMEMAX)

#define	PRM_AttributeSpecifier(typ,nam) \
    PRM_AttributeType(typ); PRM_AttributeName(nam)

#define	PRM_AttributeType(arg) PRM_vbyte(dns_validate_AttrType(arg))

#define	PRM_ACE_flags(arg) PRM_byte(arg)

#define	PRM_ACE_rights(arg) PRM_byte(arg)

#define	PRM_BOOLEAN(arg) PRM_byte(arg)

#define	PRM_DTSS_BinAbsTime(arg) \
    PRM_bytes(arg,sizeof(struct DTSS_BinAbsTime),sizeof(struct DTSS_BinAbsTime))

#define	PRM_DTSS_BinRelTime(arg) \
    PRM_bytes(arg,sizeof(struct DTSS_BinRelTime),sizeof(struct DTSS_BinRelTime))

#define	PRM_FullName_Null(arg) PRM_lbytes(dns_validate_fname(arg,1),\
      LEN_FullName(arg), FULLNAMEMAX) 

#define	PRM_FullName(arg) PRM_lbytes(dns_validate_fname(arg,0), \
      LEN_FullName(arg), FULLNAMEMAX) 
			  
#define	PRM_SimpleName(arg) PRM_lbytes(dns_validate_sname(arg), \
    LEN_SimpleName(arg),2+SIMPLENAMEMAX)

#define	PRM_isaGroup(arg) PRM_vbyte(dns_validate_GrpMbrType(arg))

#define	PRM_Timestamp(arg) PRM_bytes(arg,DNS_CTS_LEN_MAX,DNS_CTS_LEN_MAX)
#define	PRM_ObjUID(arg) PRM_bytes(arg,DNS_ID_LEN_MAX,DNS_ID_LEN_MAX)

#define	PRM_Tower(arg) PRM_mbytes(arg,dnsGetAddrLen(arg),MAXATTRIBUTE)

/*
 * build atomic input parameters
 */
#define	PRM_IN_byte(arg) { PRM_length(1); PRM_byte(arg); }

#define	PRM_IN_vbyte(arg) { PRM_length(1); PRM_vbyte(arg); }

#define	PRM_IN_word(arg) { PRM_length(2); PRM_word(arg); }

#define	PRM_IN_longword(arg) { PRM_length(4); PRM_longword(arg); }

#define	PRM_IN_bytes(arg,len,max) { PRM_length(len); PRM_bytes(arg,len,max); }

#define	PRM_IN_lbytes(arg,len,max) 				\
    { 								\
      unsigned char *ucpin_lbadr = arg; 		        \
      int ucpin_lblen = (len);					\
	                                                        \
      if (!ucpin_lbadr)	 					\
      {								\
        ucp_status = DNS_INVALIDARGUMENT;			\
	break;							\
      }								\
      PRM_length(ucpin_lblen); 					\
      if (ucpin_lblen) 						\
	  PRM_bytes(ucpin_lbadr,ucpin_lblen,max);		\
    }

#define	PRM_IN_mbytes(arg,len,max) 				\
    {		                                                \
      int ucpin_mblen = (len);					\
		                                                \
      if (max < ucpin_mblen) 					\
      {								\
        ucp_status = DNS_INVALIDARGUMENT;			\
	break;							\
      }								\
      PRM_length(ucpin_mblen);					\
      if (ucpin_mblen) 						\
        PRM_bytes(arg,ucpin_mblen,max); 			\
    }



/*
 * build DNS data types
 */
#define	PRM_IN_AtomicValue(type,adr,len) \
    { int ucpin_avlen = (len); PRM_length(3+ucpin_avlen); \
    PRM_word(ucpin_avlen); PRM_byte(type);\
    if (ucpin_avlen) PRM_mbytes(adr,ucpin_avlen,MAXATTRIBUTE); }

#define	PRM_IN_AttributeName(arg) PRM_IN_SimpleName(arg)

#define	PRM_IN_BOOLEAN(arg) PRM_IN_byte(arg)

#define	PRM_IN_ClassName(arg) \
    PRM_IN_lbytes(dns_validate_cname(arg), \
    LEN_SimpleName(arg),2+CLASSSIZE)

#define	PRM_IN_ClassVersion(arg) PRM_IN_bytes(arg,2,2)

#define	PRM_IN_EntryType(arg) PRM_IN_vbyte(dns_validate_Entries(arg))

#define	PRM_IN_FullName_Null(arg) 				\
    if (dns_validate_fname(arg,1))	 			\
      PRM_IN_bytes(arg, LEN_FullName(arg), FULLNAMEMAX)		\
    else							\
    {								\
      ucp_status = DNS_INVALIDNAME;			 	\
      break;							\
    }

#define	PRM_IN_FullName(arg) 					\
    if (dns_validate_fname(arg,0)) 				\
      PRM_IN_bytes(arg, LEN_FullName(arg), FULLNAMEMAX) 	\
    else 							\
    {								\
      ucp_status = DNS_INVALIDNAME;			 	\
      break;							\
    }		

#define	PRM_IN_ReplicaType(arg) PRM_IN_vbyte(dns_validate_ReplicaType(arg))

#define	PRM_IN_SimpleName(arg) \
    PRM_IN_lbytes(dns_validate_sname(arg), \
    LEN_SimpleName(arg),2+SIMPLENAMEMAX)


#define	PRM_IN_Timeout(arg) PRM_IN_bytes(arg,sizeof(Timeout_u),sizeof(Timeout_u))

#define PRM_IN_ObjUID(arg) PRM_IN_bytes(arg,DNS_ID_LEN_MAX,DNS_ID_LEN_MAX)
#define PRM_IN_Timestamp(arg) PRM_IN_bytes(arg,DNS_CTS_LEN_MAX,DNS_CTS_LEN_MAX)

#define	PRM_IN_fsMaybeMore(arg) PRM_IN_vbyte(dns_validate_fsMaybeMore(arg))

/*
 * build atomic output parameters
 */
#define	PRM_OUT_byte(adr) { PRM_length(PRM_buffer_LEN); PRM_buffer(adr,1); }

#define	PRM_OUT_word(adr) { PRM_length(PRM_buffer_LEN); PRM_buffer(adr,2); }

#define	PRM_OUT_longword(adr) { PRM_length(PRM_buffer_LEN); PRM_buffer(adr,4); }

#define	PRM_OUT_bytes(adr,len) { PRM_length(PRM_buffer_LEN); PRM_buffer(adr,len); }

#define	PRM_OUT_BOOLEAN(adr) PRM_OUT_byte(adr)

#define	PRM_OUT_DTSS_UTCValue(adr) \
    PRM_OUT_bytes(adr,sizeof(struct DTSS_UTCValue))

#define	PRM_OUT_Set(adr,len) PRM_OUT_bytes(adr,len)

#define	PRM_OUT_Timeout(adr) PRM_OUT_bytes(adr,sizeof(Timeout_u))

#define	PRM_OUT_ObjUID(adr) PRM_OUT_bytes(adr,DNS_ID_LEN_MAX)
#define	PRM_OUT_Timestamp(adr) PRM_OUT_bytes(adr,DNS_CTS_LEN_MAX)

#define	PRM_OUT_VersionNumber(adr) PRM_OUT_bytes(adr,sizeof(VersionNumber_u))

#define	PRM_OUT_fsOutLink(adr) PRM_OUT_longword(adr)

/*
 * build atomic input/output parameters
 */
#define	PRM_INOUT_BOOLEAN(arg,adr) \
    { PRM_length(1+PRM_buffer_LEN); PRM_byte(arg); PRM_buffer(adr,1); }

#define	PRM_INOUT_ObjUID(arg,adr) { PRM_length(DNS_ID_LEN_MAX+PRM_buffer_LEN); \
    PRM_ObjUID(arg); PRM_buffer(adr,DNS_ID_LEN_MAX); }
#define	PRM_INOUT_Timestamp(arg,adr) { PRM_length(DNS_CTS_LEN_MAX+PRM_buffer_LEN); \
    PRM_Timestamp(arg); PRM_buffer(adr,DNS_CTS_LEN_MAX); }

/*
 * Optional input for cds
 */
#define PRM_IN_OPT_ObjUID(arg) {if (arg) PRM_IN_ObjUID(arg) else \
    PRM_length(0);}

#define	PRM_IN_OPT_ClassName(arg) {if (arg) PRM_IN_ClassName(arg) else \
    PRM_length(0);}

#define	PRM_IN_OPT_ClassVersion(arg) {if (arg) PRM_IN_ClassVersion(arg) else \
    PRM_length(0);}
#define	PRM_IN_OPT_Timeout(arg) {if (arg) PRM_IN_Timeout(arg) else \
    PRM_length(0);}


/*
 * build atomic return parameters
 */
#define	PRM_RET_BOOLEAN(adr) PRM_OUT_byte(adr)

#define	PRM_RET_FullName(adr,len) PRM_OUT_bytes(adr,len)

#define	PRM_RET_ObjUID(adr) PRM_OUT_bytes(adr,DNS_ID_LEN_MAX)
#define	PRM_RET_Timestamp(adr) PRM_OUT_bytes(adr,DNS_CTS_LEN_MAX)

/*
 * build and send a request message
 *
 * The following fields in the ucp_rqst_header are set in cds_send():
 *	sequence
 *	version
 *	dontCache
 *	partialResults
 *	resLen
 *	resLen_p
 *	fromCache_p
 *	user_uid
 *	user_gid
 */
/*
 *  NOTE: It's supposed to COPY_address, COPY_longword ..macros but
 *	  since PC just can not swallow that many level macro expansion,
 *	  one less level macros will be used when we are in PC context.
 */
#if defined(DCE_SEC)

#define	PRM_build(opr,flgs)					\
{ 								\
  struct dnsFlagStat *const ucp_flgs = flgs;			\
  field8 *ucp_ptr = 0;					 	\
  sec_login_handle_t ucp_ctx_h;					\
  int ucp_status, ucp_ctx_len;					\
  while (1==1)							\
  {								\
    size_t ucp_max = sizeof(ucp_rqst_header);		 	\
    ucp_rqst_header *const ucp_buf = (ucp_rqst_header *)ucp_ptr;\
    if (!ucp_buf)				 		\
    {  /* 1st time thru */ 					\
      ucp_ctx_len = prm_login_context(&ucp_ctx_h,		\
					     flgs, NULL, 0); 	\
      if (ucp_ctx_len < 0) 					\
      {								\
        ucp_status = (flgs)->fsLocStat;                           \
	break;							\
      }								\
      ucp_max += sizeof(field16) + ucp_ctx_len; 		\
    } 								\
    else							\
    { /* 2nd time thru loop*/ 					\
      ucp_ptr += sizeof(ucp_rqst_header); 			\
      UCP_INSaddress(ucp_buf->handle, NULL); 			\
      UCP_INSbyte(ucp_buf->op, opr); 				\
      /* Copy in login context, if necessary */ 		\
      PUT16(ucp_ptr,ucp_ctx_len); 				\
      ucp_ctx_len = prm_login_context(&ucp_ctx_h,		\
				      flgs, ucp_ptr,	 	\
				      ucp_ctx_len);		\
      if (ucp_ctx_len < 0) 					\
      {								\
	ucp_status = (flgs)->fsLocStat;				\
	break;							\
      }								\
      ucp_ptr += ucp_ctx_len;	 				\
    }

#else /* !defined(DCE_SEC) */

#define	PRM_build(opr,flgs)					\
{								\
  struct dnsFlagStat *const ucp_flgs = flgs;			\
  field8 *ucp_ptr = 0;						\
  int ucp_status;						\
  while (1==1)							\
  {								\
    size_t ucp_max = sizeof(ucp_rqst_header) + sizeof(field16);	\
    ucp_rqst_header *const ucp_buf = (ucp_rqst_header *)ucp_ptr;\
    if (ucp_buf)						\
    {								\
      ucp_ptr += sizeof(ucp_rqst_header);			\
      PUT16(ucp_ptr,0);						\
      UCP_INSaddress(ucp_buf->handle, NULL);			\
      UCP_INSbyte(ucp_buf->op, opr);				\
    }

#endif /* defined(DCE_SEC) */

#define	PRM_handle(hndl) { if (ucp_buf) { \
    UCP_INSaddress(ucp_buf->handle, hndl);} }

#define	PRM_send(sts) 						\
    if (ucp_ptr) 						\
    { 								\
      UCP_INSlongword(ucp_buf->length, 				\
		      (ucp_ptr - (field8 *)ucp_buf - 		\
		       sizeof(ucp_buf->length))); 		\
                                                                \
      sts = cds_send(ucp_flgs,(byte_t *)ucp_buf); 		\
      goto ucp_done;						\
    } 								\
    else 							\
    { 								\
      ucp_ptr = (field8 *)malloc(ucp_max);			\
      if (!ucp_ptr)						\
      {								\
	ucp_status = DNS_RESOURCEERROR;			 	\
	break;							\
      }								\
      memset(ucp_ptr, 0, ucp_max);				\
    } 								\
  }								\
  sts = dns_status_clerk(ucp_flgs, ucp_status);	 		\
ucp_done: ;							\
}


/*
 * dnsvalidate.c
 */

int
dns_validate_fsConf (
    enum dnsConf );

int
dns_validate_fsWait (
    int		);

int
dns_validate_fsMaybeMore (
    int		);

int
dns_validate_AttrType (
    enum dnsAttrType );

int
dns_validate_NameType (
    dnsNameType_t );

int
dns_validate_Syntax (
    dns_syntax_t );

int
dns_validate_Entries (
    enum dnsEntries );

int
dns_validate_GrpMbrType (
    enum dnsGrpMbrType );

int
dns_validate_ReplicaType (
    enum dnsReplicaType );

unsigned char *
dns_validate_sname (
    void *);

unsigned char *
dns_validate_cname (
    void *);

unsigned char *
dns_validate_fname (
    void *,
    int      );

#endif  /* #ifndef _UC_PARSE_H */
