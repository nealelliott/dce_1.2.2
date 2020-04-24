/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: unix_stubs.c,v $
 * Revision 1.1.11.1  1996/08/09  12:01:40  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/07/30  16:00 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/1]
 *
 * 	Fix for OT 12819 - check for null from cdsGetHandle in clerk_read_replica.
 * 	Also, don't need to allocate buffers off stack.
 * 	[1996/02/18  19:37:18  marty  1.1.9.2]
 *
 * Revision 1.1.9.2  1996/02/18  19:37:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:18:06  marty]
 * 
 * Revision 1.1.9.1  1995/12/08  15:34:22  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:42  root]
 * 
 * Revision 1.1.6.8  1994/08/16  18:28:13  jd
 * 	drop from janet
 * 	[1994/08/13  16:07:44  jd]
 * 
 * Revision 1.1.6.7  1994/08/03  19:05:12  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:33:23  mccann]
 * 
 * Revision 1.1.6.6  1994/06/09  18:44:21  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:17:01  devsrc]
 * 
 * Revision 1.1.6.5  1994/06/03  20:31:26  zee
 * 	     Bugfix for OT 10841: incorrect setup in clerk_get_preferred_alias.
 * 	[1994/06/02  20:23:49  zee]
 * 
 * Revision 1.1.6.4  1994/05/06  16:04:34  zee
 * 	     Added clerk_create_slink and clerk_get_preferred_alias routines
 * 	     for HCell BL4 support.
 * 	[1994/05/06  14:53:25  zee]
 * 
 * Revision 1.1.6.3  1994/04/29  15:56:23  peckham
 * 	Remove sys/file.h reference.
 * 	Use dcelocal_path for file names.
 * 	Use dcecdsmac.h macros for serviceability.
 * 	[1994/04/29  14:38:15  peckham]
 * 
 * Revision 1.1.6.2  1994/04/14  14:51:40  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:23:09  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:14:04  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:49:04  peckham]
 * 
 * Revision 1.1.4.3  1992/12/30  14:25:36  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:26:38  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/07  15:09:12  mfox
 * 	Fix bogus call to inet_ntoa() and declare it by including arpa/inet.h
 * 	[1992/12/07  15:07:25  mfox]
 * 
 * Revision 1.1.2.3  1992/06/04  17:04:54  mfox
 * 	Fix bugs in instrumentation that potentially cause thread exceptions.
 * 	[1992/06/03  19:22:53  mfox]
 * 
 * Revision 1.1.2.2  1992/03/22  22:39:29  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:37:44  weisman]
 * 
 * Revision 1.1  1992/01/19  15:25:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Modulel unix_stubs.c
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
 * Abstract:	Unix stubs - to be replaced later.
 *
 */
#include <server.h>
#include <dce/dce_priv_proc.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/time.h>
#include <unistd.h>

#include <fcntl.h>
#include <netdb.h>
#if !defined(SOCK_STREAM)	/* socket.h not idempotent in BSD ULTRIX */
# include <sys/socket.h>
#endif
#include <netinet/in.h>
#include <arpa/inet.h>

#include <dce/rpc.h>

#include <assert.h>
#include <uc_clerk.h>
#include <clerk.h>
#include <uid.h>
#include <dns_strings.h>

extern nsgbl_ns_t dns_nameserver;

#define	CLERK_CALLOC(type, ptr) \
    if (ptr = (type *)dns_malloc(sizeof(type))) { \
	dthread_cleanup_push(clerk_free, ptr); \
	ZERO_bytes(ptr, sizeof(type));

#define	CLERK_FREE(ptr) \
	dthread_cleanup_pop(1); \
    }

#if defined(DCE_CDS_DEBUG)

# define GET_DNS_STATUS(flags_p) ((flags_p)->fsLocStat)
#else 
# define GET_DNS_STATUS(flags_p) ((flags_p)->fsDNSStat + DNS_NAMESERVERERRORS) 

#endif


/*
 * Local prototypes
 */



void 
clerk_init (void)
{
}

/*
 * clerk_free
 *	Free the block
 */
static void 
clerk_free (char *buf_p)
{
    dns_free(buf_p);
}

/******************************************************************************/


/*
 *                       Clerk Jacket Routines
 */


/******************************************************************************/

/*
 *     Convert an internal simple name to external format.
 */
dns_status_t 
clerk_cvt_simple_to_string (char  *in_p,
                            int   inlen,
                            char  *out_p,
                            int   *outlen_p)
{
    DEB_ASCII_BUF_SimpleName(inBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    int length = DNS_STR_SNAME_MAX;
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >clerk_cvt_simple_to_string(%s,%d,%p,%p)",
        deb_ascii_SimpleName(inBuf,in_p),
        inlen,
        out_p,
        outlen_p));

    status = dnsCvtCDSSimpleToStr((dns_opq_sname *)in_p,
	(unsigned char *)out_p, &length);

    if (status == CDS_SUCCESS)
	*outlen_p = strlen(out_p);

    else
	out_p = "";

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <clerk_cvt_simple_to_string(%s,%d,%p->%s,%p->%d) return(%d)",
        deb_ascii_SimpleName(inBuf,in_p),
        inlen,
        out_p,
        out_p,
        outlen_p,
        *outlen_p,
        deb_ascii_dns_status(statusBuf,status)));

    return(status);
}

/******************************************************************************/

/*
 *     Convert an external simple name to internal format.
 */
dns_status_t 
clerk_cvt_simple_to_opaque (char    *in_p,
                            int     in_len,
                            char    *out_p,
                            int     *outlen_p,
                            byte_t  *next_p)
{
    DEB_ASCII_BUF_SimpleName(outBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >clerk_cvt_simple_to_opaque(%s,%d,%p,%p,%p)",
        in_p,
        in_len,
        out_p,
        outlen_p,
        next_p));

    status = dnsCvtCDSSimpleToOpq((unsigned char *)in_p, 0,
	(dns_simple_name_t *)out_p, outlen_p);

    if (status != CDS_SUCCESS)
	out_p = (char *)NullSimpleName;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <clerk_cvt_simple_to_opaque(%s,%d,%p->%s,%p->%d,) return(%s)",
        in_p,
        in_len,
        out_p,
        deb_ascii_SimpleName(outBuf,out_p),
        outlen_p,
        *outlen_p,
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/******************************************************************************/

dns_status_t 
clerk_cvt_full_to_opaque (char    *in_p,
                          int     in_len,
                          char    *out_p,
                          int     *outlen_p,
                          byte_t  *next_p)
{
    DEB_ASCII_BUF_FullName(outBuf)
    dns_status_t status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >clerk_cvt_full_to_opaque(%s,%d,%p,%p,%p)",
        in_p,
        in_len,
        out_p,
        outlen_p,
        next_p));

    status = dnsCvtCDSFullToOpq((unsigned char *)in_p,
	(dns_full_name_t *)out_p, outlen_p);

    if (status != CDS_SUCCESS)
	out_p = (char *)NullFullName;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <clerk_cvt_full_to_opaque(%s,%d,%p->%s,%p->%d,)",
        in_p,
        in_len,
        out_p,
        deb_ascii_FullName(outBuf,out_p),
        outlen_p,
        *outlen_p));
    return(status);
}


/******************************************************************************/

dns_status_t 
clerk_resolve_name (FullName_t  *directory_p,
                    FullName_t  *result_p)
{
    DEB_ASCII_BUF_FullName(directoryBuf)
    DEB_ASCII_BUF_FullName(resultBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    cdsFlagStat_t	flags;
    int		        resultLen;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >clerk_resolve_name(%s,%p)",
        deb_ascii_FullName(directoryBuf,directory_p),
        result_p));

    resultLen = sizeof(FullName_u);

    memset(&flags, 0, sizeof(flags));
    flags.fsNameType = dnsOpqDNA;
    flags.fsUnAuth = flags.fsTrustAll = 
	!dns_nameserver.Authenticated;
    if ((status = dnsResolveName((dns_full_name_t *)directory_p,
				 (dns_full_name_t *)result_p, &resultLen,
				 &flags)) != CDS_SUCCESS) {
      status = GET_DNS_STATUS(&flags);
      result_p = (FullName_t *)NullFullName;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <clerk_resolve_name(%s,%p->%s) return(%s)",
        deb_ascii_FullName(directoryBuf,directory_p),
        result_p,
        deb_ascii_FullName(resultBuf,result_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}


/******************************************************************************/

dns_status_t 
clerk_create_child (FullName_t  *parent_p,
                    byte_t      *onbehalf_p,
                    ObjUID_t    *childuid_p,
                    Set_t       *rpset_p,
                    ObjUID_t    *outuid_p)
{
    DEB_ASCII_BUF_FullName(parentBuf)
    DEB_ASCII_BUF_ObjUID(childuidBuf)
    DEB_ASCII_BUF_ObjUID(outuidBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    cdsFlagStat_t	flags;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >clerk_create_child(%s,%p,%s,%p,%p)",
        deb_ascii_FullName(parentBuf,parent_p),
        onbehalf_p,
        deb_ascii_ObjUID(childuidBuf,childuid_p),
        rpset_p,
        outuid_p));

    memset(&flags, 0, sizeof(flags));
    flags.fsNameType = dnsOpqDNA;
    flags.fsUnAuth = flags.fsTrustAll = !dns_nameserver.Authenticated;
    if ((status = cdsCreateChild((dns_opq_fname *)parent_p,
				 (sec_id_foreign_t *)onbehalf_p,
				 (dns_id *)childuid_p,
				 (dns_opq_fname *)rpset_p, LEN_Set(rpset_p),
				 (dns_id *)outuid_p, &flags)) != CDS_SUCCESS) {
      status = GET_DNS_STATUS(&flags);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <clerk_create_child(%s,%p,%s,%p,%p->%s) return(%s)",
        deb_ascii_FullName(parentBuf,parent_p),
        onbehalf_p,
        deb_ascii_ObjUID(childuidBuf,childuid_p),
        rpset_p,
        outuid_p,
        deb_ascii_ObjUID(outuidBuf,outuid_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}


/******************************************************************************/

dns_status_t 
clerk_create_object (FullName_t      *obj_p,
                     ClassName_t     *class_p,
                     ClassVersion_t  *version_p,
                     ObjUID_t        *uid_p,
                     Timestamp_t     *outcts_p)
{
    DEB_ASCII_BUF_FullName(objBuf)
    DEB_ASCII_BUF_SimpleName(classBuf)
    DEB_ASCII_BUF_ObjUID(uidBuf)
    DEB_ASCII_BUF_Timestamp(outctsBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    cdsFlagStat_t	flags;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >clerk_create_object(%s,%s,%p,%s,%p)",
        deb_ascii_FullName(objBuf,obj_p),
        deb_ascii_SimpleName(classBuf,class_p),
        version_p,
        deb_ascii_ObjUID(uidBuf,uid_p),
        outcts_p));

    memset(&flags, 0, sizeof(flags));
    flags.fsNameType = dnsOpqDNA;
    flags.fsUnAuth = flags.fsTrustAll = !dns_nameserver.Authenticated;
    
    if ((status = dnsCreateObj((dns_full_name_t *)obj_p,
		(dns_simple_name_t *)class_p, (dnsVersion_t *)version_p,
		(dns_id_t *)uid_p,
		(dns_cts_t *)outcts_p, &flags)) != CDS_SUCCESS) {
      status = GET_DNS_STATUS(&flags);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <clerk_create_object(%s,%s,%p,%s,%p->%s) return(%s)",
        deb_ascii_FullName(objBuf,obj_p),
        deb_ascii_SimpleName(classBuf,class_p),
        version_p,
        deb_ascii_ObjUID(uidBuf,uid_p),
        outcts_p,
        deb_ascii_Timestamp(outctsBuf,outcts_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}


/******************************************************************************/

dns_status_t 
clerk_delete_child (FullName_t *child_p)
{
    DEB_ASCII_BUF_FullName(childBuf)
    cdsFlagStat_t	flags;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >clerk_delete_child(%s)",
        deb_ascii_FullName(childBuf,child_p)));

    memset(&flags, 0, sizeof(flags));
    flags.fsNameType = dnsOpqDNA;
    flags.fsUnAuth = flags.fsTrustAll = !dns_nameserver.Authenticated;

    if ((status = cdsDelChild((dns_opq_fname *)child_p, &flags))
	!= CDS_SUCCESS) {
      status = GET_DNS_STATUS(&flags);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <clerk_delete_child(%s) return(%d)",
        deb_ascii_FullName(childBuf,child_p),
        status));
    return(status);
}


/******************************************************************************/

dns_status_t 
clerk_delete_object (FullName_t *obj_p)
{
    DEB_ASCII_BUF_FullName(objBuf)
    cdsFlagStat_t	flags;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >clerk_delete_object(%s)",
        deb_ascii_FullName(objBuf,obj_p)));

    memset(&flags, 0, sizeof(flags));
    flags.fsNameType = dnsOpqDNA;
    flags.fsUnAuth = flags.fsTrustAll = !dns_nameserver.Authenticated;

    if ((status = dnsDelObj((dns_full_name_t *)obj_p, &flags))
	!= CDS_SUCCESS) {
      status = GET_DNS_STATUS(&flags);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <clerk_delete_object(%s) return(%d)",
        deb_ascii_FullName(objBuf,obj_p),
        status));
    return(status);
}


/******************************************************************************/

dns_status_t 
clerk_create_slink (FullName_t      *link_p,
		    FullName_t      *target_p,
		    Timeout_t       *timeout_p,
		    Timestamp_t     *outcts_p)
{
    DEB_ASCII_BUF_FullName(linkBuf)
    DEB_ASCII_BUF_FullName(targetBuf)
    DEB_ASCII_BUF_Timestamp(outctsBuf)
    DEB_ASCII_BUF_Timeout(timeoutBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    cdsFlagStat_t	flags;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >clerk_create_link(%s,%s,%p,%p)",
        deb_ascii_FullName(linkBuf,link_p),
        deb_ascii_FullName(targetBuf,target_p),
        timeout_p,
        outcts_p));

    memset(&flags, 0, sizeof(flags));
    flags.fsNameType = dnsOpqDNA;
    flags.fsUnAuth = flags.fsTrustAll = !dns_nameserver.Authenticated;

    if ((status = dnsCreateSLink((dns_full_name_t *)link_p,
		(dns_full_name_t *)target_p, (dns_timeout *)timeout_p,
		(dns_cts_t *)outcts_p, &flags)) != CDS_SUCCESS) {
      status = GET_DNS_STATUS(&flags);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <clerk_create_link(%s,%s,%p,%p->%s) return(%s)",
        deb_ascii_FullName(linkBuf,link_p),
        deb_ascii_FullName(targetBuf,target_p),
	timeout_p,
        outcts_p,
        deb_ascii_Timestamp(outctsBuf,outcts_p),
        deb_ascii_dns_status(statusBuf,status)));
    return(status);
}

/******************************************************************************/

dns_status_t 
clerk_get_preferred_alias (FullName_t      *cellname_p,
			   FullName_t      *pref_alias_p)
{
    int           num_mem, pos;
    byte_t        linked;		 /* Used in clerk read attr calls */
    byte_t        readbuf[READ_BUF_SIZE];
    Set_t        *attr_set_p;
    FullName_t   *preferred_p = NULL;
    dns_status_t  status = 0;

    status = clerk_read_attribute_conf(cellname_p,
				       ET_directory,
				       dns_AttributeName(DNS_CELLALIASES),
				       readbuf,
				       NULL, &linked, cdsConfHi);
    if (status != CDS_SUCCESS) {
	return status;
    }

    attr_set_p = (Set_t *)((AttributeContents_t *)readbuf)->ac_set;
    num_mem = NUM_Set((Set_t *)attr_set_p);
    if (num_mem == 0) {
	COPY_FullName(cellname_p, pref_alias_p);
	return CDS_SUCCESS;
    }

    /*
     *  Loop through the cell aliases set and find the
     *  preferred alias.
     */
    for (pos = 0; pos < num_mem; pos++) {
	MemberValue_t *member_p;
	byte_t        *nameptr_p;

	member_p = (MemberValue_t *) INDEX_Set(attr_set_p, pos);
	if (!EXT8(member_p->mv_flag)) continue;

	/* Check preferred flag */
	if (EXT8(member_p->mv_value) == BL_true) {
	    /* Skip over preferred flag and get preferred ptr */
	    preferred_p = (FullName_t *) SKIP_byte(member_p->mv_value);
	    break;
	}
    } /* end for */

    if (preferred_p) {
	/* We've got the preferred alias */
	COPY_FullName(preferred_p, pref_alias_p);
	return CDS_SUCCESS;
    }
    return CDS_NOTFND;
}

/******************************************************************************/

dns_status_t 
clerk_skulk (FullName_t *directory_p)
{
    DEB_ASCII_BUF_FullName(dirBuf)
    cdsFlagStat_t	flags;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >clerk_skulk(%s)",
        deb_ascii_FullName(dirBuf,directory_p)));

    memset(&flags, 0, sizeof(flags));
    flags.fsNameType = dnsOpqDNA;
    flags.fsUnAuth = flags.fsTrustAll = !dns_nameserver.Authenticated;
    flags.fsWait = 24*60*60;	/* give it enough time */

    if ((status = dnsSkulkDir((dns_full_name_t *)directory_p, &flags))
	!= CDS_SUCCESS) {
      status = GET_DNS_STATUS(&flags);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <clerk_skulk(%s) return(%d)",
        deb_ascii_FullName(dirBuf,directory_p),
        status));
    return(status);
}


/******************************************************************************/

/* wrap read with confidence call */
dns_status_t 
clerk_read_attribute (FullName_t       *name_p,
                      int              type,
                      AttributeName_t  *att_p,
                      byte_t           *buf_p,
                      Timestamp_t      *context_p,
                      byte_t           *link_p)
{
  return(clerk_read_attribute_conf(name_p, type, att_p, buf_p, context_p, 
				   link_p, cdsConfLow));
}

/******************************************************************************/

/* 
 * Read the attribute using a flag to assign confidence.
 */
dns_status_t 
clerk_read_attribute_conf (FullName_t       *name_p,
                           int              type,
                           AttributeName_t  *att_p,
                           byte_t           *buf_p,
                           Timestamp_t      *context_p,
                           byte_t           *link_p,
                           dnsConf_t        conf)
{
    DEB_ASCII_BUF_FullName(nameBuf)
    DEB_ASCII_BUF_AttributeName(attBuf)
    struct buf {
	cdsFlagStat_t	flags;
	AttributeType_u	lastAttCon;
	Set_u		st;
	MemberValue_u	mv;
        unsigned char   whole_set;
    } *ctx_p;
    dns_status_t	status = CDS_NONSRESOURCES;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >clerk_read_attribute_conf(%s,%d,%s)",
        deb_ascii_FullName(nameBuf,name_p),
        type,
        deb_ascii_AttributeName(attBuf,att_p)));

    CLERK_CALLOC(struct buf, ctx_p) {
	byte_t *lastAttCon_p = NULL;
	ctx_p->flags.fsNameType = dnsOpqDNA;
	ctx_p->flags.fsConf = conf;		 /* Set confidence  */
	ctx_p->flags.fsUnAuth = ctx_p->flags.fsTrustAll = 
	  !dns_nameserver.Authenticated;
	if (context_p) {
	    Set_t *st_p;
	    MemberValue_t *mv_p;

	    lastAttCon_p = ctx_p->lastAttCon;
	    INS8(ctx_p->lastAttCon, AT_set);
	    st_p = (Set_t *)ctx_p->st;
	    INS8(st_p->st_flag, ST_present);
	    INS16(st_p->st_offsetLength, sizeof(word_u));
	    mv_p = (MemberValue_t *)DATA_Set(st_p);
	    INS16(st_p->st_offsets, (byte_t *)mv_p - (byte_t *)st_p);
	    INS8(mv_p->mv_flag, MV_present);
	    COPY_Timestamp(context_p, mv_p->mv_timestamp);
	    INS16(mv_p->mv_length, 0);
	    INS16(st_p->st_valueLength, mv_p->mv_value - (byte_t *)mv_p);
            ctx_p->whole_set = 0;
	}

	if ((status = cdsReadAttrSet((dns_opq_fname *)name_p, lastAttCon_p,
		(dns_opq_sname *)att_p, (dnsEntries_t)type, NULL,
		buf_p, READ_BUF_SIZE,  &ctx_p->whole_set,
                &ctx_p->flags)) != CDS_SUCCESS)
	    status = GET_DNS_STATUS(&ctx_p->flags);

	if (link_p)
	    *link_p = ctx_p->flags.fsOutLink;

        if ((status == CDS_SUCCESS) && !ctx_p->whole_set) {
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_server,
                svc_c_debug6,
                "  clerk_read_attribute(%s,%d,%s) - more_data",
                deb_ascii_FullName(nameBuf,name_p),
                type,
                deb_ascii_AttributeName(attBuf,att_p)));
            status = CDS_MOREDATA;
        }

    } CLERK_FREE(ctx_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <clerk_read_attribute_conf(%s,%d,%s) return(%d)",
        deb_ascii_FullName(nameBuf,name_p),
        type,
        deb_ascii_AttributeName(attBuf,att_p),
        status));
    return(status);
}


/******************************************************************************/

dns_status_t 
clerk_test_attribute (FullName_t       *name_p,
                      int              type,
                      AttributeName_t  *att_p,
                      int              len,
                      byte_t           *data_p,
                      int              valuetype)
{
 return(clerk_test_attribute_conf(name_p, type, att_p, len, data_p, 
				  valuetype, cdsConfLow));
}

/******************************************************************************/

/*									    
 *  Clerk_test_attribute_conf currently only used by ta_cle.c.
 */
 
dns_status_t 
clerk_test_attribute_conf (FullName_t       *name_p,
                           int              type,
                           AttributeName_t  *att_p,
                           int              len,
                           byte_t           *data_p,
                           int              valuetype,
                           dnsConf_t        conf)
{
    DEB_ASCII_BUF_FullName(nameBuf)
    DEB_ASCII_BUF_AttributeName(attBuf)
    cdsFlagStat_t	flags;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >clerk_test_attribute_conf(%s,%d,%s)",
        deb_ascii_FullName(nameBuf,name_p),
        type,
        deb_ascii_AttributeName(attBuf,att_p)));

    memset(&flags, 0, sizeof(flags));
    flags.fsNameType = dnsOpqDNA;
    flags.fsConf = conf;			/* Set confidence */
    flags.fsUnAuth = flags.fsTrustAll = !dns_nameserver.Authenticated;

    status = dnsIntTestAttrValue((dns_full_name_t *)name_p,
		(dns_attr_name_t *)att_p, (dnsEntries_t)type, valuetype,
		data_p, len, &flags);
    if ((status != CDS_SUCCESS) && (status != CDS_NOTFND)) {
      status = GET_DNS_STATUS(&flags);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <clerk_test_attribute_conf(%s,%d,%s) return(%d)",
        deb_ascii_FullName(nameBuf,name_p),
        type,
        deb_ascii_AttributeName(attBuf,att_p),
        status));
    return(status);
}


/******************************************************************************/

dns_status_t 
clerk_testgroup (FullName_t  *group_p,
                 FullName_t  *member_p,
                 char        *direct_p,
                 ObjUID_t    *loop_p,
                 Timeout_t   *timeout_p)
{
    DEB_ASCII_BUF_FullName(groupBuf)
    DEB_ASCII_BUF_FullName(memberBuf)
    DEB_ASCII_BUF_ObjUID(loopBuf)
    cdsFlagStat_t	flags;
    int	        	myDirect;
    char		direct;
    dns_status_t	status;

    if (!direct_p) {
	direct_p = &direct;
	direct = FALSE;
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >clerk_testgroup(%s,%s,%d,%s)",
        deb_ascii_FullName(groupBuf,group_p),
        deb_ascii_FullName(memberBuf,member_p),
        *direct_p,
        deb_ascii_ObjUID(loopBuf,loop_p)));

    memset(&flags, 0, sizeof(flags));
    flags.fsNameType = dnsOpqDNA;
    flags.fsUnAuth = flags.fsTrustAll = !dns_nameserver.Authenticated;

    myDirect = *direct_p;
    status = dnsTestMbr((dns_opq_fname *)group_p,
			(dns_opq_fname *)member_p, &myDirect,
			(dns_timeout *)timeout_p,
			(dns_id *)loop_p, &flags);

    if ((status != CDS_SUCCESS) && (status != CDS_NOTFND)) {
      status = GET_DNS_STATUS(&flags);
    }

    *direct_p = myDirect;
      
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <clerk_testgroup(%s,%s,%d,%s) return(%d)",
        deb_ascii_FullName(groupBuf,group_p),
        deb_ascii_FullName(memberBuf,member_p),
        *direct_p,
        deb_ascii_ObjUID(loopBuf,loop_p),
        status));
    return(status);
}


/******************************************************************************/

dns_status_t 
clerk_link_replica (byte_t           *onbehalf_p,
                    FullName_t       *directory_p,
                    ObjUID_t         *diruid_p,
                    Update_t         *update_p,
                    int              update_len,
                    ObjUID_t         *epoch_p,
                    VersionNumber_t  *replicaversion_p)
{
    DEB_ASCII_BUF_FullName(onbehalfBuf)
    DEB_ASCII_BUF_FullName(directoryBuf)
    DEB_ASCII_BUF_ObjUID(diruidBuf)
    cdsFlagStat_t	flags;
    dns_id              ringpointer;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >clerk_link_replica(%s,%s,%s)",
        deb_ascii_FullName(onbehalfBuf,&onbehalf_p[1]),
        deb_ascii_FullName(directoryBuf,directory_p),
        deb_ascii_ObjUID(diruidBuf,diruid_p)));

    memset(&flags, 0, sizeof(flags));
    flags.fsNameType = dnsOpqDNA;
    flags.fsUnAuth = flags.fsTrustAll =  !dns_nameserver.Authenticated;

    if ((status = cdsLinkReplica((dns_opq_fname *)directory_p,
		(dns_opq_fname *)&onbehalf_p[1], (dns_id *)diruid_p,
		(byte_t *)update_p, update_len,
		(dns_id *)epoch_p,
		&ringpointer,
		(dnsVersion_t *)replicaversion_p,
		&flags)) != CDS_SUCCESS) {
      status = GET_DNS_STATUS(&flags);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <clerk_link_replica(%s,%s,%s) return(%d)",
        deb_ascii_FullName(onbehalfBuf,&onbehalf_p[1]),
        deb_ascii_FullName(directoryBuf,directory_p),
        deb_ascii_ObjUID(diruidBuf,diruid_p),
        status));

    return(status);
}


/******************************************************************************/

dns_status_t 
clerk_modify_attribute (FullName_t  *name_p,
                        int         type,
                        Update_t    *update_p,
                        int         update_len)
{
    DEB_ASCII_BUF_FullName(nameBuf)
    DEB_ASCII_BUF_AttributeName(attNameBuf)
    AttributeName_t	*const attName_p = (AttributeName_t *)
	FLD_AttributeSpecifier_AttributeName(update_p->ud_attribute);
    cdsFlagStat_t	flags;
    dns_status_t	status;
    const int operation = EXT8(update_p->ud_operation);
    dns_cts *const CTS_p = (dns_cts *)update_p->ud_timestamp;
    AttributeSpecifier_t *const as_p = (AttributeSpecifier_t *)update_p->ud_attribute;
    dns_attr_name_t *AttrName_p;
    BOOLEAN_t *valuePresent_p;
    AtomicValue_t *av_p;
    enum dnsAttrType AttrType;
    int valuePresent;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >clerk_modify_attribute(%s,%d,%s)",
        deb_ascii_FullName(nameBuf,name_p),
        type,
        deb_ascii_AttributeName(attNameBuf,attName_p)));

    memset(&flags, 0, sizeof(flags));
    flags.fsNameType = dnsOpqDNA;
    flags.fsUnAuth = flags.fsTrustAll = !dns_nameserver.Authenticated;

    /* parse the update packet */
    AttrName_p = (dns_attr_name_t *)as_p->as_name;
    valuePresent_p = (BOOLEAN_t *)SKIP_AttributeSpecifier(as_p);
    av_p = (AtomicValue_t *)SKIP_BOOLEAN(valuePresent_p);
    AttrType = (dnsAttrType_t)EXT8(as_p->as_type);
    valuePresent = EXT8(valuePresent_p);

    if (operation != UD_present) {
      if (!valuePresent) {
	status = dnsRmAttr((dns_full_name_t *)name_p,
			   AttrName_p, AttrType, (dnsEntries_t)type,
			   CTS_p, &flags);
      } else {
	status = dnsIntRmAttrValue((dns_full_name_t *)name_p,
				   AttrName_p, (enum dnsEntries)type,
				   (int)EXT8(av_p->av_valuetype),
				   av_p->av_value, EXT16(av_p->av_length),
				   CTS_p, &flags);
      }
    } else {
      status = dnsIntAddAttrValue((dns_full_name_t *)name_p,
				  AttrName_p, AttrType, (dnsEntries_t)type,
				  (int)EXT8(av_p->av_valuetype),
				  av_p->av_value, EXT16(av_p->av_length), 
				  CTS_p, &flags);
    }
    if (status != CDS_SUCCESS) {
      status = GET_DNS_STATUS(&flags);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <clerk_modify_attribute(%s,%d,%s) return(%d)",
        deb_ascii_FullName(nameBuf,name_p),
        type,
        deb_ascii_AttributeName(attNameBuf,attName_p),
        status));

    return(status);
}


/******************************************************************************/

dns_status_t 
clerk_modify_replica (ObjUID_t          *diruid_p,
                      Update_t          *update_p,
                      int               update_len,
                      ReplicaPointer_t  *rep_p,
                      int               replen)
{
    DEB_ASCII_BUF_ObjUID(diruidBuf)
    DEB_ASCII_BUF_AttributeName(attNameBuf)
    DEB_ASCII_BUF_FullName(chNameBuf)
    SimpleName_t	*attName_p = (SimpleName_t *)
	FLD_AttributeSpecifier_AttributeName(update_p->ud_attribute);
    FullName_t		*chName_p = (FullName_t *)
	FLD_ReplicaPointer_CHName(rep_p);
    cdsFlagStat_t	flags;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >clerk_modify_replica(%s,%s,,%s)",
        deb_ascii_ObjUID(diruidBuf,diruid_p),
        deb_ascii_AttributeName(attNameBuf,attName_p),
        deb_ascii_FullName(chNameBuf,chName_p)));

    memset(&flags, 0, sizeof(flags));
    flags.fsNameType = dnsOpqDNA;
    flags.fsUnAuth = flags.fsTrustAll = !dns_nameserver.Authenticated;

    if ((status = cdsModifyReplica((dns_id *)diruid_p,
				   (byte_t *)rep_p, replen,
				   (byte_t *)update_p, update_len, 
				   &flags)) != CDS_SUCCESS) {
      status = GET_DNS_STATUS(&flags);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <clerk_modify_replica(%s,%s,,%s) return(%d)",
        deb_ascii_ObjUID(diruidBuf,diruid_p),
        deb_ascii_AttributeName(attNameBuf,attName_p),
        deb_ascii_FullName(chNameBuf,chName_p),
        status));

    return(status);
}


/******************************************************************************/

dns_status_t 
clerk_read_replica (ObjUID_t          *uid_p,
                    FullName_t        *dir_p,
                    AttributeName_t   *att_p,
                    byte_t            *outbuf_p,
                    ReplicaPointer_t  *in_rp_p,
                    int               in_rplen,
                    ReplicaPointer_t  *out_rp_p)
{
    DEB_ASCII_BUF_ObjUID(uidBuf)
    DEB_ASCII_BUF_FullName(dirBuf)
    DEB_ASCII_BUF_AttributeName(attBuf)
    DEB_ASCII_BUF_FullName(chNameBuf)
    FullName_t		*chName_p =
	(FullName_t *)FLD_ReplicaPointer_CHName(in_rp_p);
    dns_handle		handle_p;
    cdsFlagStat_t       flags;
    dns_status_t	status;
    byte_t      	*value_p;
    int		        valueLen;
    char	        valueType;
    dns_cts	        *cts_p;
    int		        addrLen;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >clerk_read_replica(%s,%s,%s,,%s)",
        deb_ascii_ObjUID(uidBuf,uid_p),
        deb_ascii_FullName(dirBuf,dir_p),
        deb_ascii_AttributeName(attBuf,att_p),
        deb_ascii_FullName(chNameBuf,chName_p)));

    memset(&flags, 0, sizeof(flags));
    flags.fsNameType = dnsOpqDNA;
    flags.fsUnAuth = flags.fsTrustAll = !dns_nameserver.Authenticated;

    handle_p = cdsGetHandle();
    if (handle_p == NULL) {
	/* Couldn't allocate a buffer */
	status = CDS_NONSRESOURCES;
    }
    else {
	MemberValue_t *mv_p = (MemberValue_t *)SKIP_byte(outbuf_p);
	byte_t *rp_addressHint_p;

	addrLen = sizeof(out_rp_p->rp_addressHint);
	/*
	 * reconstruct ReadAttributeResponse
	 */
	INS8(outbuf_p, RC_ONE_ATTRIBUTE);
	INS8(mv_p->mv_flag, MVX_absent);
	COPY_Timestamp(NullTimestamp, mv_p->mv_timestamp);
	INS16(mv_p->mv_length, 0);
	/*
	 * Fake out a ReplicaPointer - upper layers will only be
	 * interested in the addressHint. Default that to the NullSet
	 * in case the clerk call doesn't return it.
	 */
	if (out_rp_p) {
	    ZERO_ObjUID(out_rp_p->rp_CHID);
	    COPY_Set(NullSet, out_rp_p->rp_addressHint);
	    rp_addressHint_p = out_rp_p->rp_addressHint;
	} else {
	    rp_addressHint_p = NULL;
	}
	
	/*
	 * Used only to read DNS$ReplicaState and DNS$CTS
	 * both are single-valued so a loop is not needed.
	 */
	status = cdsReadReplica((dns_opq_fname *)dir_p,
				(dns_id *)uid_p,
				handle_p,
				(dns_attr_name_t *)att_p,
				(dns_opq_fname *)FLD_ReplicaPointer_CHName(in_rp_p),
				(byte_t *)&valueType, &value_p,
				&valueLen, &cts_p, rp_addressHint_p,
				&addrLen, &flags);
	if (status == CDS_SUCCESS) {
	    if (cts_p)
	      COPY_Timestamp(cts_p, mv_p->mv_timestamp);

	    if (value_p) {
		INS8(mv_p->mv_flag, MV_present);
		INS16(mv_p->mv_length, valueLen);

		if (valueLen)
		  COPY_bytes(value_p, mv_p->mv_value, valueLen);
	    }

	} else {
	    status = GET_DNS_STATUS(&flags);
	}
	
	if (out_rp_p) {
	    INS8(FLD_ReplicaPointer_replicaType(out_rp_p), RT_master);
	    COPY_FullName(NullFullName, FLD_ReplicaPointer_CHName(out_rp_p));
	}
	
	dnsFreeHandle(handle_p);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <clerk_read_replica(%s,%s,%s,%s) return(%d)",
        deb_ascii_ObjUID(uidBuf,uid_p),
        deb_ascii_FullName(dirBuf,dir_p),
        deb_ascii_AttributeName(attBuf,att_p),
        deb_ascii_FullName(chNameBuf,chName_p),
        status));

    return(status);
}


/******************************************************************************/

dns_status_t 
clerk_do_updates (ObjUID_t          *epoch_p,
                  ReplicaPointer_t  *rp_p,
                  int               rp_len,
                  int               upd_len,
                  byte_t            *upd_p)
{
    DEB_ASCII_BUF_ObjUID(epochBuf)
    DEB_ASCII_BUF_FullName(chNameBuf)
    FullName_t		*chName_p =
	(FullName_t *)FLD_ReplicaPointer_CHName(rp_p);
    cdsFlagStat_t	flags;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >clerk_do_updates(%s,%s)",
        deb_ascii_ObjUID(epochBuf,epoch_p),
        deb_ascii_FullName(chNameBuf,chName_p)));

    memset(&flags, 0, sizeof(flags));
    flags.fsNameType = dnsOpqDNA;
    flags.fsUnAuth = flags.fsTrustAll = !dns_nameserver.Authenticated;
    flags.fsWait = 12*60*60;	/* give it enough time */

    if ((status = cdsDoUpdate((byte_t *)rp_p, rp_len,
			      (dns_id *)epoch_p, upd_p, upd_len,
			      NULL, &flags)) != CDS_SUCCESS) {
      status = GET_DNS_STATUS(&flags);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <clerk_do_updates(%s,%s) return(%d)",
        deb_ascii_ObjUID(epochBuf,epoch_p),
        deb_ascii_FullName(chNameBuf,chName_p),
        status));

    return(status);
}


/******************************************************************************/

dns_status_t 
clerk_combine (ObjUID_t          *uid_p,
               Timestamp_t       *all_p,
               ObjUID_t          *epoch_p,
               Timestamp_t       *sk_p,
               FullName_t        *chname_p,
               ReplicaPointer_t  *in_rp_p,
               int               in_rp_len,
               ObjUID_t          *retuid_p,
               ReplicaPointer_t  *out_rp_p)
{
    DEB_ASCII_BUF_ObjUID(uidBuf)
    DEB_ASCII_BUF_Timestamp(allBuf)
    DEB_ASCII_BUF_ObjUID(epochBuf)
    DEB_ASCII_BUF_Timestamp(skBuf)
    DEB_ASCII_BUF_FullName(chnameBuf)
    DEB_ASCII_BUF_FullName(rpnameBuf)
    cdsFlagStat_t	flags;
    dns_status_t	status;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " >clerk_combine(%s,%s,%s,%s,%s,%s)",
        deb_ascii_ObjUID(uidBuf,uid_p),
        deb_ascii_Timestamp(allBuf,all_p),
        deb_ascii_ObjUID(epochBuf,epoch_p),
        deb_ascii_Timestamp(skBuf,sk_p),
        deb_ascii_FullName(chnameBuf,chname_p),
        deb_ascii_FullName(rpnameBuf,FLD_ReplicaPointer_CHName(in_rp_p))));

    memset(&flags, 0, sizeof(flags));
    flags.fsNameType = dnsOpqDNA;
    flags.fsUnAuth = flags.fsTrustAll = !dns_nameserver.Authenticated;
    flags.fsWait = 12*60*60;	/* give it enough time */

    /*
     * Fake out a ReplicaPointer - upper layers will only be
     * interested in the addressHint. Default that to the NullSet
     * in case the clerk call doesn't return it.
     */
    ZERO_ObjUID(out_rp_p->rp_CHID);
    COPY_Set(NullSet, out_rp_p->rp_addressHint);

    if ((status = cdsCombine((dns_id *)uid_p,
		(byte_t *)in_rp_p, in_rp_len, (dns_opq_fname *)chname_p,
		(dns_cts *)sk_p, (dns_cts *)all_p, (dns_id *)epoch_p,
		(dns_id *)retuid_p, out_rp_p->rp_addressHint,
		sizeof(NetworkAddress_u), &flags)) != CDS_SUCCESS) {
      status = GET_DNS_STATUS(&flags);
    }

    INS8(FLD_ReplicaPointer_replicaType(out_rp_p), RT_master);
    COPY_FullName(NullFullName, FLD_ReplicaPointer_CHName(out_rp_p));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug6,
        " <clerk_combine(%s,%s,%s,%s,%s,%s) return(%d)",
        deb_ascii_ObjUID(uidBuf,uid_p),
        deb_ascii_Timestamp(allBuf,all_p),
        deb_ascii_ObjUID(epochBuf,epoch_p),
        deb_ascii_Timestamp(skBuf,sk_p),
        deb_ascii_FullName(chnameBuf,chname_p),
        deb_ascii_FullName(rpnameBuf,FLD_ReplicaPointer_CHName(in_rp_p)),
        status));

    return(status);
}

/******************************************************************************/


/*
 * From vms_select.c
 */

/*
 * ROUTINE - sys_diag_ast_speed_selector
 *
 * Sets the fast_asts switch on or off
 *
 * Arguments:
 *     switch_value (in)   - 0-off, 1-on
 *
 * Returns:
 *     prior value of the switch.
 */
int 
sys_diag_ast_speed_selector (int switch_value)
{
   return(switch_value);
}



/******************************************************************************/

/* 
 * ROUTINE - sys_diag_async_op
 *
 * Diagnostic data structure dumper for asynchronous operations
 *
 * This routine can provide diagnostic-level output of private async op
 * structures.  It creates formatted output, which must be handled by a
 * user-supplied routine (e.g., a routine that printf's the output string).
 *
 * Arguments:
 *    s_code (in)       - What structure to dump.
 *    detail_level (in) - What level of detail for the dump (s_code dependent)
 *    op_block (in)     - Pointer to a specific operation block
 *    emit_routine (in) - Routine to call to emit output.
 *    emit_arg (in)     - Argument for emit_routine
 *
 * Supported s_codes:
 *    1 - Dump a specific async operation block
 *    2 - Dump all async operation blocks currently being managed
 *    3 - Dump statistics
 *
 * Returns:
 *
 *    0 - Dump completed.
 *   -1 - Error somewhere.
 */
int 
sys_diag_async_op (int   	s_code, 
		   int          detail_level, 
		   char        *op_block, 
		   int   	(emit_routine)(char *, sys_mailbox),
		   sys_mailbox  emit_arg)
{
    return (-1);
}





