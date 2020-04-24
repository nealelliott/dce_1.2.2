/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: rpc_cvt.c,v $
 * Revision 1.1.7.2  1996/02/18  19:35:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:10  marty]
 *
 * Revision 1.1.7.1  1995/12/08  15:27:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:29  root]
 * 
 * Revision 1.1.5.6  1994/08/03  19:03:12  mccann
 * 	includes file cleanup
 * 	[1994/07/19  18:10:20  mccann]
 * 
 * Revision 1.1.5.5  1994/06/30  19:17:52  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:04:28  mccann]
 * 
 * Revision 1.1.5.4  1994/06/09  18:42:28  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:14:41  devsrc]
 * 
 * Revision 1.1.5.3  1994/04/29  15:55:31  peckham
 * 	Change dns* to cds*
 * 	[1994/04/29  14:34:58  peckham]
 * 
 * Revision 1.1.5.2  1994/03/22  20:00:11  griffin
 * 	dified instrumentation events to Level 9 so the serviceability
 * 	logs aren't as cluttered.
 * 	[1994/03/22  16:45:19  griffin]
 * 
 * Revision 1.1.5.1  1994/03/12  22:08:40  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:34:50  peckham]
 * 
 * Revision 1.1.3.3  1992/12/30  14:17:07  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:16:53  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  18:53:15  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:16:08  weisman]
 * 
 * Revision 1.1  1992/01/19  15:19:33  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module rpc_cvt.c
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
 * This module converts between the wire protocol and the structures used
 * internally.
 */

#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdssvc.h>
#include <dce/dns_record.h>
#include <dce/sys_time.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <dce/rpc_cvt.h>

#if defined(DCE_SEC)
# include <dpeacl.h>
#endif

/*
 * This module is shared between server and client.  On the server side
 * rpc_ss_allocate and rpc_ss_free are supplied by the RPC
 * runtime.  On the child side they are defined in the clerk.
 */

    
void
cds_visa_init (cds_visa_t  *visa_p,
               char        *cellname_p)
{
    DCE_SVC_DEBUG((
       cds__svc_handle,
       cds_svc_library,
       svc_c_debug2,
       " >cds_visa_init"));

    ZERO_bytes(visa_p, sizeof(*visa_p));

    if (cellname_p) {
	visa_p->visa_str_cellname.fn_length = strlen(cellname_p);
	strcpy((char *)visa_p->visa_str_cellname.fn_name, cellname_p);
    }

    DCE_SVC_DEBUG((
       cds__svc_handle,
       cds_svc_library,
       svc_c_debug2,
       " <cds_visa_init"));
}

int 
cds_visa_local_destination (cds_visa_t *visa_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " >cds_visa_local_destination"));

    return(visa_p->visa_progress == visa_here);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " <cds_visa_local_destination"));
}

void 
cds_visa_destroy (cds_visa_t *visa_p)
{   	
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " >cds_visa_destroy"));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " <cds_visa_destroy"));
}

static void 
set_status (cds_status_t  *status_p,
            int           status)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " >set_status"));

    status_p->er_status = status;
    status_p->er_name = (cds_FullName_t *)0;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug2,
        " <set_status"));
}
/*
 * Copy local timeout to Remote
 */
byte_t *
cdsExport_Timeout (cds_visa_t      *visa_p,
                   Timeout_t       *in_p,
                   cds_TimeoutP_t  *out_pp)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug8,
        " >cdsExport_Timeout"));

  *out_pp = (cds_Timeout_t *)NULL;
  /*
   * All zero's => value not supplied, do byte compare cause invalid utc
   */
  if (NE_bytes(in_p, NullTimeout, sizeof(NullTimeout))) {
    *out_pp = (cds_Timeout_t *)rpc_ss_allocate(sizeof(cds_Timeout_t));
    if (*out_pp != (cds_Timeout_t *)NULL)
      COPY_Timeout(in_p, *out_pp);
  }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug8,
        " <cdsExport_Timeout"));

  return(SKIP_Timeout(in_p));
}

/*
 * Copy Remote to local timeout
 */
byte_t *
cdsImport_Timeout (cds_visa_t     *visa_p,
                   cds_Timeout_t  *in_p,
                   Timeout_t      *out_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug8,
        " >cdsImport_Timeout"));

  if (in_p == (cds_Timeout_t *)NULL)
    /* Special internal value => no timeout */
    COPY_Timeout(NullTimeout, out_p);
  else
    COPY_Timeout(in_p, out_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug8,
        " <cdsImport_Timeout"));

  return(SKIP_Timeout(out_p));
}

/*
 * Copy local Timestamp_t to cds_Timestamp_t
 */
byte_t *
cdsExport_Timestamp (cds_visa_t       *visa_p,
                     Timestamp_t      *in_p,
                     cds_Timestamp_t  *out_p)
{
  time_quad_t time;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsExport_Timestamp"));

  COPY_Node(in_p->ts_node, out_p->ts_node);

  COPY_Time(in_p->ts_time, &time);
#if defined(__alpha)
  out_p->ts_time = time;
#else
  out_p->ts_time.low = time.tq_lo;
  out_p->ts_time.high = time.tq_hi;
#endif

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsExport_Timestamp"));

  return(SKIP_Timestamp(in_p));
}

/*
 * Copy remote cds_Timestamp_t to local Timestamp_t
 */
byte_t *
cdsImport_Timestamp (cds_visa_t       *visa_p,
                     cds_Timestamp_t  *in_p,
                     Timestamp_t      *out_p,
                     int              freemem)
{
  time_quad_t time;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsImport_Timestamp"));

  COPY_Node(in_p->ts_node, out_p->ts_node);
#if defined(__alpha)
  time = in_p->ts_time;
#else
  time.tq_lo = in_p->ts_time.low;
  time.tq_hi = in_p->ts_time.high;
#endif

  COPY_Time(&time, out_p->ts_time);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsImport_Timestamp"));

  return(SKIP_Timestamp(out_p));
}

/*
 * The progress record has a full name spanning two FullNames.
 * When appending a cellname, don't prepend to both,
 * and when stripping a cellname, dont strip from both.
 */
static void 
export_name (cds_visa_t      *visa_p,
             FullName_t      *unresolved_p,
             FullName_t      *resolved_p,
             cds_Progress_t  *out_p)
{
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >export_name"));

  if (EQ_bytes(resolved_p, NullFullName, sizeof(NullFullName))) {
    /* nothing resolved */
    /* FullName starts with unresolvedName */
    cdsExport_FullName(NULL, resolved_p, &out_p->pr_resolved);
    cdsExport_FullName(visa_p, unresolved_p, &out_p->pr_unresolved);
  } else {
    /* FullName starts with resolvedName */
    cdsExport_FullName(visa_p, resolved_p, &out_p->pr_resolved);
    cdsExport_FullName(NULL, unresolved_p, &out_p->pr_unresolved);
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <export_name"));
}

static byte_t *
import_name (cds_visa_t      *visa_p,
             cds_Progress_t  *in_p,
             byte_t          *out_p,
             int             freemem)
{
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >import_name"));

  if (visa_p) visa_p->visa_progress = visa_there;

  /* HACK way of detecting NullFullName in pr_resolved */
  if ((in_p->pr_resolved.fn_length < 1) ||
	((in_p->pr_resolved.fn_length == 1) &&
	 (in_p->pr_resolved.fn_name[0] == '/') &&
	 EQ_bytes(NullFullName, &in_p->pr_resolved.fn_root,
			sizeof(in_p->pr_resolved.fn_root)))) {
    /* nothing resolved */
    /* FullName starts with unresolvedName */
    out_p = cdsImport_FullName(visa_p, &in_p->pr_unresolved,
		(FullName_t *)out_p, freemem);
    if (visa_p && (visa_p->visa_fullname != visa_global))
      visa_p->visa_progress = visa_here;
    /* Can't use cdsImport_FullName() 'cause creates non-null fn_root */
    COPY_FullName(NullFullName, out_p);
    out_p = SKIP_FullName(out_p);
  } else {
    /* FullName starts with resolvedName */
    out_p = cdsImport_FullName(NULL, &in_p->pr_unresolved,
		(FullName_t *)out_p, freemem);
    out_p = cdsImport_FullName(visa_p, &in_p->pr_resolved,
		(FullName_t *)out_p, freemem);
    if (visa_p && (visa_p->visa_fullname != visa_global))
      visa_p->visa_progress = visa_here;
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <import_name: @ end"));

  return(out_p);
}

/*
 * Build_R_Progress - Build Remote progress
 */
void 
cdsExport_new_Progress (cds_visa_t      *visa_p,
                        byte_t          *flags_p,
                        Timeout_t       *timeout_p,
                        FullName_t      *unresolved_p,
                        FullName_t      *resolved_p,
                        cds_Progress_t  *pr_p)
{
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsExport_new_Progress"));

  pr_p->pr_flags = EXT8(flags_p);
  cdsExport_Timeout(visa_p, timeout_p, &pr_p->pr_timeout);
  export_name (visa_p, unresolved_p, resolved_p, pr_p);
  /* Empty set on input to server */
  pr_p->pr_replicas_p = (cds_Set_t *)NULL;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsExport_new_Progress"));
}

/*
 * Convert a local progress record to remote.  Assumes
 * pr_replicas_p = NULL.
 * Return pointer past local.
 */
byte_t *
cdsExport_Progress (cds_visa_t      *visa_p,
                    Progress_t      *in_p,
                    cds_Progress_t  *out_p,
                    cds_status_t    *status_p)
{
  int i,j,cnt;
  Set_t *set_p;
  cds_Set_t *outset_p;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsExport_Progress"));

  /* Get the clearinghouse list */
  set_p = (Set_t *)SKIP_FullName(SKIP_FullName(in_p->pr_unresolvedName));
  cnt = EXT16(set_p->st_offsetLength)/sizeof(short);

  /* If not set or allocation fails, continue with no replicas */
  if (cnt) {
    outset_p = (cds_Set_t *)rpc_ss_allocate(sizeof(cds_Set_t) +
					   cnt*sizeof(cds_SetMember_t));
    if (outset_p == (cds_Set_t *)NULL)
      cnt = 0;
    else {
      outset_p->set_type = AT_set;
      outset_p->set_length = cnt;
    }
  } else
    outset_p = (cds_Set_t *)NULL;

  /* Fill it in */
  out_p->pr_flags = EXT8(in_p->pr_flags);

  cdsExport_Timeout(visa_p, (Timeout_t *)in_p->pr_timeout, &out_p->pr_timeout);

  export_name (visa_p, (FullName_t *)in_p->pr_unresolvedName,
		(FullName_t *)SKIP_FullName(in_p->pr_unresolvedName), out_p);

  for (i=0; i<cnt; ++i) {
    if (cdsExport_SetMember(visa_p, (MemberValue_t *)INDEX_Set(set_p, i),
				&outset_p->set_members[i],
				status_p) == NULL) {
      /* Cleanup */
      for (j=0; j<i; ++j)
	R_AtomicValue_Free(&outset_p->set_members[i].sm_value);
      rpc_ss_free((char *)outset_p);
      outset_p = (cds_Set_t *)NULL;
      break;
    }
  }
  /* Set up pointer for return at last minute */
  out_p->pr_replicas_p = outset_p;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsExport_Progress: @end"));

  return(SKIP_Set(set_p));
}

/*
 * cdsImport_Progress - Build local progress record
 *   Free full pointers in replica set.
 */
byte_t *
cdsImport_Progress (cds_visa_t      *visa_p,
                    cds_Progress_t  *in_p,
                    Progress_t      *out_p,
                    int             freemem)
{
  register byte_t *ptr_p;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsImport_Progress "));

  INS8(out_p->pr_flags, in_p->pr_flags);
  cdsImport_Timeout(visa_p, in_p->pr_timeout, (Timeout_t *)out_p->pr_timeout);
  if (in_p->pr_timeout != (cds_Timeout_t *)NULL)
    rpc_ss_free((char *)in_p->pr_timeout);
  ptr_p = import_name (visa_p, in_p, out_p->pr_unresolvedName, freemem);
  if (in_p->pr_replicas_p != (cds_Set_t *)NULL) {
    ptr_p = cdsImport_Set(visa_p, in_p->pr_replicas_p, (Set_t *)ptr_p, freemem);
    rpc_ss_free((char *)in_p->pr_replicas_p);
  } else {
    ZERO_Set(ptr_p);
    ptr_p = SKIP_Set(ptr_p);
  }
  in_p->pr_replicas_p = (cds_Set_t *)NULL;
  in_p->pr_timeout = (cds_Timeout_t *)NULL;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsImport_Progress: @ end"));

  return(ptr_p);
}

/*
 * Local FullName type to cds_FullName_t
 */
byte_t *
cdsExport_FullName (cds_visa_t      *visa_p,
                    FullName_t      *in_p,
                    cds_FullName_t  *out_p)
{
    register char *cellname_p = 0;


    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsExport_FullName"));

    /* Save objectuid, incase translation not known on other side */
    COPY_ObjUID(in_p->fn_root, &out_p->fn_root);

    if (visa_p)
	if ((visa_p->visa_names != visa_all_local) &&
		visa_p->visa_str_cellname.fn_length)
	    cellname_p = (char *)visa_p->visa_str_cellname.fn_name;

    /* Send name over wire in external format */
    out_p->fn_length = CDS_STR_FNAME_MAX;
    if (dns_CvtCDSFullToStr((cds_full_name_t *)in_p, out_p->fn_name,
			 (int *)&out_p->fn_length, cellname_p) == CDS_SUCCESS)
	--out_p->fn_length;
    else
	out_p->fn_length = 0;

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsExport_FullName: @ end"));

    return(SKIP_FullName(in_p));
}
/*
 * Remote cds_FullName_t to local FullName_t
 */
byte_t *
cdsImport_FullName (cds_visa_t      *visa_p,
                    cds_FullName_t  *in_p,
                    FullName_t      *out_p,
                    int             freemem)
{
    byte_t name[CDS_STR_FNAME_MAX];
    char *name_p = 0;
    int outlen = sizeof(cds_full_name_t);


    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsImport_FullName"));

    /* Null terminate name */
    COPY_bytes(in_p->fn_name, name, in_p->fn_length);
    name[in_p->fn_length] = '\0';

    if (visa_p) name_p = (char *)visa_p->visa_str_cellname.fn_name;

    if (dns_CvtCDSFullToOpq (name, (cds_full_name_t  *)out_p, 
			     &outlen, name_p) == CDS_SUCCESS) {
	COPY_ObjUID(&in_p->fn_root, out_p->fn_root);
	if (visa_p) {
	    /*
	     * Special hack: outlen not used to return actual length.
	     *  left non-zero if global name detected
	     *  zeroed if not global name
	     */
	    if (outlen) {
		if (!visa_p->visa_names)
		    visa_p->visa_names = visa_all_global;
		else if (visa_p->visa_names == visa_all_local)
		    visa_p->visa_names = visa_all_mixed;

		if (outlen < 0)
		    visa_p->visa_fullname = visa_stripped;
		else
		    visa_p->visa_fullname = visa_global;
	    } else
		visa_p->visa_fullname = visa_local;
	}
    } else if (visa_p)
	visa_p->visa_fullname = 0;

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsImport_FullName: @ end"));

    return(SKIP_FullName(out_p));
}

/*
 * cdsExport_AttributeName - copy AttributeName_t to cds_AttributeName_t
 */
byte_t *
cdsExport_AttributeName (cds_visa_t       *visa_p,
                         AttributeName_t  *in_p,
                         cds_Name_t       *out_p)
{
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsExport_AttributeName"));

  out_p->nm_length = LEN_AttributeName(in_p);
  COPY_AttributeName(in_p, out_p->nm_name);

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsExport_AttributeName: @ end"));

  return(SKIP_AttributeName(in_p));
}
/*
 * cdsImport_AttributeName - copy cds_AttributeName_t to AttributeName_t
 */
byte_t *
cdsImport_AttributeName (cds_visa_t       *visa_p,
                         cds_Name_t       *in_p,
                         AttributeName_t  *out_p,
                         int              freemem)
{
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsImport_AttributeName"));

  COPY_AttributeName(in_p->nm_name, out_p);

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsImport_AttributeName: @ end"));

  return(SKIP_AttributeName(out_p));
}

/*
 * cdsExport_SimpleName - copy SimpleName_t to cds_SimpleName_t
 */
byte_t *
cdsExport_SimpleName (cds_visa_t    *visa_p,
                      SimpleName_t  *in_p,
                      cds_Name_t    *out_p)
{
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsExport_SimpleName"));

  out_p->nm_length = LEN_SimpleName(in_p);
  COPY_SimpleName(in_p, out_p->nm_name);

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsExport_SimpleName"));

  return(SKIP_SimpleName(in_p));
}
/*
 * cdsImport_SimpleName - copy cds_SimpleName_t to SimpleName_t
 */
byte_t *
cdsImport_SimpleName (cds_visa_t    *visa_p,
                      cds_Name_t    *in_p,
                      SimpleName_t  *out_p,
                      int           freemem)
{
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsImport_SimpleName"));

  COPY_SimpleName(in_p->nm_name, out_p);

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsImport_SimpleName"));

  return(SKIP_SimpleName(out_p));
}

/*
 * cdsExport_ClassName - copy ClassName_t to cds_ClassName_t
 */
byte_t *
cdsExport_ClassName (cds_visa_t   *visa_p,
                     ClassName_t  *in_p,
                     cds_Name_t   *out_p)
{
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsExport_ClassName"));

  out_p->nm_length = LEN_ClassName(in_p);
  COPY_ClassName(in_p, out_p->nm_name);

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsExport_ClassName"));

  return(SKIP_ClassName(in_p));
}

/*
 * cdsImport_ClassName - copy cds_ClassName_t to ClassName_t
 */
byte_t *
cdsImport_ClassName (cds_visa_t   *visa_p,
                     cds_Name_t   *in_p,
                     ClassName_t  *out_p,
                     int          freemem)
{
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsImport_ClassName"));

  COPY_ClassName(in_p->nm_name, out_p);

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsImport_ClassName"));

  return(SKIP_ClassName(out_p));
}

/*
 * Convert a local set to a remote one
 *
 * Return pointer past Local set.
 */
byte_t *
cdsExport_Set (cds_visa_t    *visa_p,
               Set_t         *in_p,
               cds_SetP_t    *out_p,
               cds_status_t  *status_p)
{
  cds_Set_t *Rset_p;
  int        cnt,i,j;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsExport_Set"));

  /* Determine member count */
  cnt = EXT16(in_p->st_offsetLength)/sizeof(short);

  *out_p = (cds_Set_t *)rpc_ss_allocate(sizeof(cds_Set_t) +
				      sizeof(cds_SetMember_t)*cnt);
  if (*out_p == (cds_Set_t *)NULL) {
    set_status(status_p, PDU_NoLocalResources);

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsExport_Set: (1) return(NULL)"));

    return(NULL);
  }

  Rset_p = *out_p;

  /* fill in the set */
  Rset_p->set_type = AT_set;
  Rset_p->set_length = cnt;
  for (i=0; i<cnt; ++i) {
    if (cdsExport_SetMember(visa_p, (MemberValue_t *)INDEX_Set(in_p, i),
		     &Rset_p->set_members[i], status_p) == NULL) {
      /* Cleanup */
      for (j=0; j<i; ++j)
	R_AtomicValue_Free(&Rset_p->set_members[i].sm_value);
      rpc_ss_free((char *)Rset_p);
      *out_p = (cds_Set_t *)NULL;

      DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug8,
        " <cdsExport_Set: (2) return(NULL)"));

      return(NULL);
    }
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsExport_Set: @ end"));

  return(SKIP_Set(in_p));
}

/*
 * cdsImport_Set = convert an cds_Set to a Set_t
 *              Don't free set itself cause may be part of something bigger
 */
byte_t *
cdsImport_Set (cds_visa_t  *visa_p,
               cds_Set_t   *in_p,
               Set_t       *out_p,
               int         freemem)
{
  int i, cnt;
  int offset;
  byte_t *data_p,*next_p;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsImport_Set"));

  /* Get set member count */
  cnt = in_p->set_length;

  /* Come over RPC as set_t structure, but may have to turn into 1 member */
  if (in_p->set_type == AT_set) {

    /* Build Set Header */
    INS8(out_p->st_flag, ST_present);
    INS16(out_p->st_offsetLength, cnt*sizeof(short));
    INS16(out_p->st_valueLength, 0);

    /* Initialize values */
    data_p = DATA_Set(out_p);

    offset = sizeof(Set_overhead) + EXT16(out_p->st_offsetLength);

    /* Convert each member */
    for (i=0; i<cnt; ++i) {
      next_p = cdsImport_MemberValue(visa_p,
		&in_p->set_members[i], (MemberValue_t *)data_p, freemem);
      INS16(OFFSET_Set(out_p, i), offset);
      offset += (next_p - data_p);
      data_p = next_p;
    }

    INS16(out_p->st_valueLength, data_p - DATA_Set(out_p));
  } else {
    if (cnt == 0) {
      ZERO_bytes(out_p, sizeof(MemberValue_overhead));
      data_p = SKIP_MemberValue(out_p);
    }else
      data_p = cdsImport_MemberValue(visa_p,
		&in_p->set_members[0], (MemberValue_t *)out_p, freemem);
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsImport_Set: @ end"));

  return(data_p);
}

/*
 * Convert a local MemberValue to a remote set
 *
 * Return pointer past Local MemberValue.
 */
byte_t *
cdsExport_MemberValuetoSet (cds_visa_t     *visa_p,
                            MemberValue_t  *in_p,
                            cds_SetP_t     *out_p,
                            cds_status_t   *status_p)
{
  cds_Set_t *Rset_p;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsExport_MemberValuetoSet"));

  *out_p = (cds_Set_t *)(byte_t *)rpc_ss_allocate(sizeof(cds_Set_t) +
				      sizeof(cds_SetMember_t));
  if (*out_p == (cds_Set_t *)NULL) {
    set_status(status_p, PDU_NoLocalResources);

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsExport_MemberValuetoSet: (1) return(NULL)"));

    return(NULL);
  }

  Rset_p = *out_p;

  /* fill in the set */
  Rset_p->set_type = AT_single;
  Rset_p->set_length = 1;

  if (cdsExport_SetMember(visa_p,
		in_p, &Rset_p->set_members[0], status_p) == NULL) {
    /* Cleanup */
    rpc_ss_free((char *)Rset_p);
    *out_p = (cds_Set_t *)NULL;
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsExport_MemberValuetoSet: (2) return(NULL)"));

    return(NULL);
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsExport_MemberValuetoSet: @ end"));

  return(SKIP_MemberValue(in_p));
}

/*
 * Convert from RPC set member to local
 */
byte_t *
cdsImport_MemberValue (cds_visa_t       *visa_p,
                       cds_SetMember_t  *in_p,
                       MemberValue_t    *out_p,
                       int              freemem)
{
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsImport_MemberValue"));

  INS8(out_p->mv_flag, in_p->sm_flag);
  cdsImport_Timestamp(visa_p,
		&in_p->sm_ts, (Timestamp_t *)out_p->mv_timestamp,  freemem);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsImport_MemberValue"));

  return(cdsImport_AtomicValue(visa_p,
		&in_p->sm_value, (AtomicValue_t *)out_p->mv_length, freemem));
}

/*
 * Convert local membervalue to remote set member
 * Return pointer past local membervalue
 */
byte_t *
cdsExport_SetMember (cds_visa_t       *visa_p,
                     MemberValue_t    *in_p,
                     cds_SetMember_t  *out_p,
                     cds_status_t     *status_p)
{  
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsExport_SetMember"));

  out_p->sm_flag = EXT8(in_p->mv_flag);
  cdsExport_Timestamp(visa_p,
		(Timestamp_t *)in_p->mv_timestamp, &out_p->sm_ts);

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsExport_SetMember"));

  return(cdsExport_AtomicValue(visa_p, (AtomicValue_t *)in_p->mv_length,
			     &out_p->sm_value, status_p));
}

/*
 * Convert exception message into cds_status_t
 */
void 
cdsExport_Exception (cds_visa_t    *visa_p,
                     byte_t        *msg_p,
                     cds_status_t  *cds_status_p)
{
  unsigned long status;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsExport_Exception"));

  status = GET32(msg_p);

  /* Copy error message */
  cds_status_p->er_status = status  - CDS_NAMESERVERERRORS;
  if (cds_status_p->er_status == PDU_UnknownEntry) {
    cds_status_p->er_name = (cds_FullName_t *)(byte_t *)rpc_ss_allocate(sizeof(cds_FullName_t));
    if (cds_status_p->er_name != (cds_FullName_t *)NULL)
      cdsExport_FullName(visa_p, (FullName_t *)msg_p, cds_status_p->er_name);
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsExport_Exception"));
}

byte_t *
cdsImport_Exception (cds_visa_t    *visa_p,
                     cds_status_t  *cds_status_p,
                     byte_t        *msg_p,
                     int           freemem)
{
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsImport_Exception"));

  /* Copy error message */
  PUT32(msg_p, cds_status_p->er_status);
  if (cds_status_p->er_status == PDU_UnknownEntry) {
    msg_p = cdsImport_FullName(visa_p,
		cds_status_p->er_name, (FullName_t *)msg_p, freemem);
    if (freemem == TRUE)
      rpc_ss_free((char *)cds_status_p->er_name);
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsImport_Exception: @ end"));

  return(msg_p);
}

/*
 * Convert remote atomic value to local
 */
byte_t *
cdsExport_AtomicValue (cds_visa_t         *visa_p,
                       AtomicValue_t      *in_p,
                       cds_AtomicValue_t  *out_p,
                       cds_status_t       *status_p)
{
  register byte_t *ptr_p;
  struct {
      union {
	  long            align;
	  bytes_u         buf[MAXATTRIBUTE];
      } flat;
  } flatbuf;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsExport_AtomicValue"));

  out_p->av_valuetype = EXT8(in_p->av_valuetype);
  switch (out_p->av_valuetype)
    {
    case VT_none:
      break;
    case VT_long:
      COPY_bytes(in_p->av_value, &out_p->av_val.av_long,
		 sizeof(ndr_long_int));
      break;
    case VT_short:
      COPY_bytes(in_p->av_value, &out_p->av_val.av_short,
		 sizeof(ndr_short_int));
      break;
    case VT_small:
      COPY_bytes(in_p->av_value, &out_p->av_val.av_small,
		 sizeof(ndr_small_int));
      break;
    case VT_uuid:
      COPY_bytes(in_p->av_value, &out_p->av_val.av_uuid,
		 sizeof(uuid_t));
      break;
    case VT_Timestamp:
      cdsExport_Timestamp(visa_p,
		(Timestamp_t *)in_p->av_value, &out_p->av_val.av_timestamp);
      break;
    case VT_Timeout:
      COPY_Timeout(in_p->av_value, &out_p->av_val.av_timeout);
      break;
    case VT_char:
      out_p->av_val.av_char_p =
	(cds_OpenChar_t *)rpc_ss_allocate(EXT16(in_p->av_length) +
				sizeof(cds_OpenChar_t));
      if (!out_p->av_val.av_char_p) {
	set_status(status_p, PDU_NoLocalResources);

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_library,
	    svc_c_debug8,
	    " <cdsExport_AtomicValue: (1) ret(NULL)"));

	return(NULL);
      }
      out_p->av_val.av_char_p->op_length = EXT16(in_p->av_length);
      COPY_bytes(in_p->av_value, out_p->av_val.av_char_p->op_array,
		 out_p->av_val.av_char_p->op_length);
      break;
    case VT_ASN1:
    case VT_byte:
      out_p->av_val.av_byte_p =
	(cds_OpenByte_t *)rpc_ss_allocate(EXT16(in_p->av_length) +
				sizeof(cds_OpenByte_t));
      if (!out_p->av_val.av_byte_p) {
	set_status(status_p, PDU_NoLocalResources);

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_library,
	    svc_c_debug8,
	    " <cdsExport_AtomicValue: (2) ret(NULL)"));

	return(NULL);
      }
      out_p->av_val.av_byte_p->op_length = EXT16(in_p->av_length);
      COPY_bytes(in_p->av_value, out_p->av_val.av_byte_p->op_array,
		 out_p->av_val.av_byte_p->op_length);
      break;
    case VT_ReplicaPointer:
      {ReplicaPointer_t *const rp_p = (ReplicaPointer_t *)in_p->av_value;

      out_p->av_val.av_rp_p =
	(cds_ReplicaPointer_t *)rpc_ss_allocate(sizeof(cds_ReplicaPointer_t) +
			    LEN_Set(rp_p->rp_addressHint));
      if (!out_p->av_val.av_rp_p) {
	set_status(status_p, PDU_NoLocalResources);

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_library,
	    svc_c_debug8,
	    " <cdsExport_AtomicValue: (3) ret(NULL)"));

	return(NULL);
      }
      /* Allocation size will be big enough */
      COPY_ObjUID(rp_p->rp_CHID, &out_p->av_val.av_rp_p->rp_chid);
      ptr_p = RPTowerPack((Set_t *)rp_p->rp_addressHint, out_p->av_val.av_rp_p);
      out_p->av_val.av_rp_p->rp_type = GET8(ptr_p);
      ptr_p = cdsExport_FullName(visa_p,
		(FullName_t *)ptr_p, &out_p->av_val.av_rp_p->rp_chname);
      break;
      }
    case VT_GroupMember:
      {GroupMember_t *const gm_p = (GroupMember_t *)in_p->av_value;
      out_p->av_val.av_gm_p =
	(cds_GroupMember_t *)rpc_ss_allocate(sizeof(cds_GroupMember_t));
      if (!out_p->av_val.av_rp_p) {
	set_status(status_p, PDU_NoLocalResources);

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_library,
	    svc_c_debug8,
	    " <cdsExport_AtomicValue: (4) ret(NULL)"));

	return(NULL);
      }
      out_p->av_val.av_gm_p->gm_isaGroup = EXT8(gm_p->gm_isaGroup);
      cdsExport_FullName(visa_p, (FullName_t *)gm_p->gm_member,
		&out_p->av_val.av_gm_p->gm_member);
      break;
      }
    case VT_ParentPointer:
      {ParentPointer_t *const pp_p = (ParentPointer_t *)in_p->av_value;
      out_p->av_val.av_pp_p =
	(cds_ParentPointer_t *)rpc_ss_allocate(sizeof(cds_ParentPointer_t));
      if (!out_p->av_val.av_pp_p) {
	set_status(status_p, PDU_NoLocalResources);

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_library,
	    svc_c_debug8,
	    " <cdsExport_AtomicValue: (5) ret(NULL)"));

	return(NULL);
      }
      COPY_ObjUID(pp_p->pp_parentID, &out_p->av_val.av_pp_p->pp_parentID);
      COPY_Timeout(pp_p->pp_timeout,
		       &out_p->av_val.av_pp_p->pp_timeout);
      cdsExport_FullName(visa_p, (FullName_t *)pp_p->pp_myName,
		&out_p->av_val.av_pp_p->pp_myName);
      break;
      }
    case VT_gdaPointer:
      {gdaPointer_t *const gp_p = (gdaPointer_t *)in_p->av_value;
       ReplicaPointer_t *rp_p = (ReplicaPointer_t *)gp_p->gp_replica;

      out_p->av_val.av_gda_p =
	(cds_gdaPointer_t *)rpc_ss_allocate(sizeof(cds_gdaPointer_t)
			+ LEN_Set(rp_p->rp_addressHint));
      if (!out_p->av_val.av_gda_p) {
	set_status(status_p, PDU_NoLocalResources);

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_library,
	    svc_c_debug8,
	    " <cdsExport_AtomicValue: (6) ret(NULL)"));

	return(NULL);
      }
      COPY_Timeout(gp_p->gp_timeout,
		       &out_p->av_val.av_gda_p->gp_timeout);
      /* Allocation size will be big enough */
      COPY_ObjUID(rp_p->rp_CHID, &out_p->av_val.av_gda_p->gp_replica.rp_chid);
      ptr_p = RPTowerPack((Set_t *)rp_p->rp_addressHint,
	&out_p->av_val.av_gda_p->gp_replica);
      out_p->av_val.av_gda_p->gp_replica.rp_type = GET8(ptr_p);
      ptr_p = cdsExport_FullName(visa_p, (FullName_t *)ptr_p,
	&out_p->av_val.av_gda_p->gp_replica.rp_chname);
      break;
      }
    case VT_FullName:
      out_p->av_val.av_fullname_p =
	(cds_FullName_t *)rpc_ss_allocate(sizeof(cds_FullName_t));
      if (!out_p->av_val.av_pp_p) {
	set_status(status_p, PDU_NoLocalResources);


	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_library,
	    svc_c_debug8,
	    " <cdsExport_AtomicValue: (7) ret(NULL)"));

	return(NULL);
      }
      cdsExport_FullName(visa_p,
		(FullName_t *)in_p->av_value, out_p->av_val.av_fullname_p);
      break;
    case VT_CHDirectory:
      {CHDirectoryPointer_t *const cp_p = (CHDirectoryPointer_t *)in_p->av_value;

      out_p->av_val.av_cp_p =
	(cds_CHDirectory_t *)rpc_ss_allocate(sizeof(cds_CHDirectory_t));
      if (!out_p->av_val.av_cp_p) {
	set_status(status_p, PDU_NoLocalResources);

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_library,
	    svc_c_debug8,
	    " <cdsExport_AtomicValue: (8) ret(NULL)"));

	return(NULL);
      }
      COPY_ObjUID(cp_p->cp_dirID, &out_p->av_val.av_cp_p->cp_dirID);
      cdsExport_FullName(visa_p, (FullName_t *)cp_p->cp_directory,
	&out_p->av_val.av_cp_p->cp_directory);
      break;
      }
    case VT_Version:
      ptr_p = in_p->av_value;
      out_p->av_val.av_version.ver_major = GET8(ptr_p);
      out_p->av_val.av_version.ver_minor = GET8(ptr_p);
      break;
#if defined(DCE_SEC)
    case VT_DACL:
      /* This has to be aligned */
      COPY_bytes(in_p->av_value, flatbuf.flat.buf,
		 EXT16(in_p->av_length));
      dpe_acl_flat_to_wire((char *)flatbuf.flat.buf, DPE_ACL_RPCMEM,
			   &out_p->av_val.av_acl_p);
      break;
#endif
    }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsExport_AtomicValue: @ end"));

  return(SKIP_AtomicValue(in_p));
}

/*
 * Convert remote atomic value to local
 */
byte_t *
cdsImport_AtomicValue (cds_visa_t         *visa_p,
                       cds_AtomicValue_t  *in_p,
                       AtomicValue_t      *out_p,
                       int                freemem)
{
  byte_t *msg_p;
  int    len;
  byte_t *flat_p;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsImport_AtomicValue"));

  INS8(out_p->av_valuetype, in_p->av_valuetype);
  switch (in_p->av_valuetype)
    {
    case VT_none:
      INS16(out_p->av_length, 0);
      break;
    case VT_long:
      INS16(out_p->av_length, sizeof(ndr_long_int));
      COPY_bytes(&in_p->av_val.av_long, out_p->av_value,
		 sizeof(ndr_long_int));
      break;
    case VT_short:
      INS16(out_p->av_length, sizeof(ndr_short_int));
      COPY_bytes(&in_p->av_val.av_short, out_p->av_value,
		 sizeof(ndr_short_int));
      break;
    case VT_small:
      INS16(out_p->av_length, sizeof(ndr_small_int));
      COPY_bytes(&in_p->av_val.av_small, out_p->av_value,
		 sizeof(ndr_small_int));
      break;
    case VT_uuid:
      INS16(out_p->av_length, sizeof(uuid_t));
      COPY_bytes(&in_p->av_val.av_uuid, out_p->av_value,
		 sizeof(uuid_t));
      break;
    case VT_Timestamp:
      INS16(out_p->av_length, sizeof(Timestamp_t));
      cdsImport_Timestamp(visa_p, &in_p->av_val.av_timestamp,
		(Timestamp_t *)out_p->av_value, freemem);
      break;
    case VT_Timeout:
      INS16(out_p->av_length, sizeof(cds_Timeout_t));
      COPY_Timeout(&in_p->av_val.av_timeout, out_p->av_value);
      break;
    case VT_char:
      INS16(out_p->av_length, in_p->av_val.av_char_p->op_length);
      COPY_bytes(in_p->av_val.av_char_p->op_array, out_p->av_value,
		 in_p->av_val.av_char_p->op_length);
      if (freemem)
	rpc_ss_free((char *)in_p->av_val.av_char_p);
      break;
    case VT_ASN1:
    case VT_byte:
      INS16(out_p->av_length, in_p->av_val.av_byte_p->op_length);
      COPY_bytes(in_p->av_val.av_byte_p->op_array, out_p->av_value,
		 in_p->av_val.av_byte_p->op_length);
      if (freemem)
	rpc_ss_free((char *)in_p->av_val.av_byte_p);
      break;
    case VT_ReplicaPointer:
      msg_p = out_p->av_value;
      COPY_ObjUID(&in_p->av_val.av_rp_p->rp_chid, msg_p);
      msg_p = SKIP_ObjUID(msg_p);
      msg_p = RPTowerUnpack(in_p->av_val.av_rp_p->rp_towers, (Set_t *)msg_p);
      PUT8(msg_p, in_p->av_val.av_rp_p->rp_type);
      msg_p = cdsImport_FullName(visa_p, &in_p->av_val.av_rp_p->rp_chname,
		(FullName_t *)msg_p, freemem);
      if (freemem)
	rpc_ss_free((char *)in_p->av_val.av_rp_p);
      INS16(out_p->av_length, msg_p - out_p->av_value);
      break;
    case VT_GroupMember:
      msg_p = out_p->av_value;
      PUT8(msg_p, in_p->av_val.av_gm_p->gm_isaGroup);
      msg_p = cdsImport_FullName(visa_p, &in_p->av_val.av_gm_p->gm_member,
		(FullName_t *)msg_p, freemem);
      if (freemem)
	rpc_ss_free((char *)in_p->av_val.av_gm_p);
      INS16(out_p->av_length, msg_p - out_p->av_value);
      break;
    case VT_ParentPointer:
      msg_p = out_p->av_value;
      COPY_ObjUID(&in_p->av_val.av_pp_p->pp_parentID, msg_p);
      msg_p = SKIP_ObjUID(msg_p);
      COPY_Timeout(&in_p->av_val.av_pp_p->pp_timeout, msg_p);
      msg_p = SKIP_Timeout(msg_p);
      msg_p = cdsImport_FullName(visa_p, &in_p->av_val.av_pp_p->pp_myName,
		(FullName_t *)msg_p, freemem);
      if (freemem)
	rpc_ss_free((char *)in_p->av_val.av_pp_p);
      INS16(out_p->av_length, msg_p - out_p->av_value);
      break;
    case VT_gdaPointer:
      msg_p = out_p->av_value;
      COPY_Timeout(&in_p->av_val.av_gda_p->gp_timeout, msg_p);
      msg_p = SKIP_Timeout(msg_p);
      COPY_ObjUID(&in_p->av_val.av_gda_p->gp_replica.rp_chid, msg_p);
      msg_p = SKIP_ObjUID(msg_p);
      msg_p = RPTowerUnpack(in_p->av_val.av_gda_p->gp_replica.rp_towers,
				(Set_t *)msg_p);
      PUT8(msg_p, in_p->av_val.av_gda_p->gp_replica.rp_type);
      msg_p = cdsImport_FullName(visa_p,
		&in_p->av_val.av_gda_p->gp_replica.rp_chname,
		(FullName_t *)msg_p, freemem);
      INS16(out_p->av_length, msg_p - out_p->av_value);
      if (freemem)
	rpc_ss_free((char *)in_p->av_val.av_gda_p);
      break;
    case VT_FullName:
      msg_p = out_p->av_value;
      msg_p = cdsImport_FullName(visa_p, in_p->av_val.av_fullname_p,
		(FullName_t *)msg_p, freemem);
      if (freemem)
	rpc_ss_free((char *)in_p->av_val.av_fullname_p);
      INS16(out_p->av_length, msg_p - out_p->av_value);
      break;
    case VT_CHDirectory:
      msg_p = out_p->av_value;
      COPY_ObjUID(&in_p->av_val.av_cp_p->cp_dirID, msg_p);
      msg_p = SKIP_ObjUID(msg_p);
      msg_p = cdsImport_FullName(visa_p, &in_p->av_val.av_cp_p->cp_directory,
		(FullName_t *)msg_p, freemem);
      if (freemem)
	rpc_ss_free((char *)in_p->av_val.av_cp_p);
      INS16(out_p->av_length, msg_p - out_p->av_value);
      break;
    case VT_Version:
      INS16(out_p->av_length, sizeof(VersionNumber_u));
      msg_p = out_p->av_value;
      PUT8(msg_p, in_p->av_val.av_version.ver_major);
      PUT8(msg_p, in_p->av_val.av_version.ver_minor);
      break;
#if defined(DCE_SEC)
    case VT_DACL:
      /* Convert to flat format */
      dpe_acl_wire_to_flat(in_p->av_val.av_acl_p,
				DPE_ACL_STDMEM, (char **)&flat_p, &len);
      /* Copy flat to atomic value */
      COPY_bytes(flat_p, out_p->av_value, len);
      INS16(out_p->av_length, len);

      /* Free it all */
      free((char *)flat_p);
      if (freemem)
	R_Acl_Free(in_p->av_val.av_acl_p, TRUE);
      break;
#endif
    }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsImport_AtomicValue: @ end"));

  return(SKIP_AtomicValue(out_p));
}

/*
 * Convert local to remote udpate
 */
byte_t *
cdsExport_Update (cds_visa_t    *visa_p,
                  Update_t      *in_p,
                  cds_Update_t  *out_p,
                  cds_status_t  *status_p)
{
  AttributeSpecifier_t *att_p;
  byte_t   *msg_p;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsExport_Update"));

  out_p->ud_operation = EXT8(in_p->ud_operation);
  att_p =
   (AttributeSpecifier_t *)cdsExport_Timestamp(visa_p,
		(Timestamp_t *)in_p->ud_timestamp, &out_p->ud_timestamp);
  out_p->ud_type = EXT8(att_p->as_type);
  if (out_p->ud_type == AT_null) 
  {
      out_p->ud_value.av_valuetype = VT_none;

      DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug8,
        " <cdsExport_Update: out_p->ud_type == AT_null"));

      return(SKIP_Update((byte_t *)in_p));
  }

  COPY_bytes(att_p->as_name, out_p->ud_attribute,
	     LEN_AttributeName(att_p->as_name));
  msg_p = SKIP_bytes(att_p->as_name, LEN_AttributeName(att_p->as_name));

  if (GET8(msg_p) == BL_true)
  {
      DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug8,
        " <cdsExport_Update: GET8(msg_p) == BL_true")); 

    return(cdsExport_AtomicValue(visa_p,
				 (AtomicValue_t *)msg_p, 
				 &out_p->ud_value, status_p));
}
  
  out_p->ud_value.av_valuetype = VT_none;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsExport_Update: @ end"));

  return(SKIP_Update((byte_t *)in_p));
}
/*
 * Convert remote update to local
 */
byte_t *
cdsImport_Update (cds_visa_t    *visa_p,
                  cds_Update_t  *in_p,
                  Update_t      *out_p,
                  int           freemem)
{
  AttributeSpecifier_t *att_p;
  byte_t *msg_p;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsImport_Update"));

  INS8(out_p->ud_operation, in_p->ud_operation);
  att_p = (AttributeSpecifier_t *)cdsImport_Timestamp(visa_p,
		&in_p->ud_timestamp, (Timestamp_t *)out_p->ud_timestamp,
		freemem);
  INS8(att_p->as_type, in_p->ud_type);
  if (in_p->ud_type == AT_null) {
    msg_p = SKIP_byte(att_p);
    PUT8(msg_p, BL_false);

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug2,
      " <cdsImport_Update: return(msg_p)"));

    return(msg_p);
  }

  COPY_bytes(in_p->ud_attribute, att_p->as_name,
		     LEN_AttributeName(in_p->ud_attribute));
  msg_p = SKIP_bytes(att_p->as_name, LEN_AttributeName(att_p->as_name));

  if (in_p->ud_value.av_valuetype == VT_none)
    PUT8(msg_p, BL_false);
  else {
    PUT8(msg_p, BL_true);
    msg_p = cdsImport_AtomicValue(visa_p,
		&in_p->ud_value,(AtomicValue_t *)msg_p,freemem);
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsImport_Update: @ end - return(msg_p)"));

  return(msg_p);
}

/*
 * Convert local to remote UpdatePacket
 */
byte_t *
cdsExport_UpdatePkt (cds_visa_t       *visa_p,
                     UpdatePacket_t   *in_p,
                     cds_UpdatePkt_t  **out_p,
                     cds_status_t     *status_p)
{
  byte_t   *msg_p;
  cds_UpdatePkt_t *update_p;
  int i,j,cnt;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsExport_UpdatePkt"));

  /* Determine how many updates we have */
  msg_p = SKIP_FullName(in_p->up_entryName);
  msg_p = SKIP_Timestamp(msg_p);
  msg_p = SKIP_EntryType(msg_p);

  cnt = EXT8(msg_p);
  *out_p = (cds_UpdatePkt_t *)rpc_ss_allocate(sizeof(cds_UpdatePkt_t) +
					    (cnt*sizeof(cds_Update_t)));
  if (*out_p == (cds_UpdatePkt_t *)0) {
    set_status(status_p, PDU_NoLocalResources);

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsExport_UpdatePkt: (1) return(NULL)"));

    return(NULL);
  }

  update_p = *out_p;
  msg_p = cdsExport_FullName(visa_p,
		(FullName_t *)in_p->up_entryName, &update_p->pkt_name);
  msg_p = cdsExport_Timestamp(visa_p,
		(Timestamp_t *)msg_p, &update_p->pkt_cts);
  update_p->pkt_type = GET8(msg_p);
  update_p->pkt_cnt = GET8(msg_p);

  /* Copy Each Update */
  for (i=0; i<cnt; ++i) {
    msg_p = cdsExport_Update(visa_p,
		(Update_t *)msg_p, &update_p->pkt_data[i], status_p);
    if (msg_p == NULL) {
      /* Free any memory that has been allocated */
      for (j=0; j<i; ++j)
	R_AtomicValue_Free(&update_p->pkt_data[j].ud_value);
      rpc_ss_free((char *)update_p);
      *out_p = (cds_UpdatePkt_t *)0;

      DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug8,
        " <cdsExport_UpdatePkt: (2) return(NULL)"));

      return(NULL);
    }
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsExport_UpdatePkt: @ end - return(msg_p)"));

  return(msg_p);
}
/*
 * Convert remote to local UpdatePacket
 */
byte_t *
cdsImport_UpdatePkt (cds_visa_t       *visa_p,
                     cds_UpdatePkt_t  *in_p,
                     UpdatePacket_t   *out_p,
                     cds_status_t     *status_p)
{
  byte_t   *msg_p;
  int i;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >cdsImport_UpdatePkt"));

  /* Determine how many updates we have */
  msg_p = cdsImport_FullName(visa_p,
		&in_p->pkt_name, (FullName_t *)out_p->up_entryName, FALSE);
  msg_p = cdsImport_Timestamp(visa_p,
		&in_p->pkt_cts, (Timestamp_t *)msg_p, FALSE);
  PUT8(msg_p, in_p->pkt_type);
  PUT8(msg_p, in_p->pkt_cnt);

  /* Copy Each Update */
  for (i=0; i<in_p->pkt_cnt; ++i) {
    msg_p = cdsImport_Update(visa_p,
		&in_p->pkt_data[i], (Update_t *)msg_p, FALSE);
  }

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <cdsImport_UpdatePkt: @ end"));

  return(msg_p);
}

/*
 * Free up nidl malloc space for a set
 */
void 
R_Set_Free (cds_Set_t *in_p)
{
  int i;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >R_Set_Free"));

  /* If null pointer, all done */
  if (in_p == (cds_Set_t *)NULL) 
  {
      DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_library,
        svc_c_debug8,
        " <R_Set_Free: null pointer"));

      return;
  }

  for (i=0; i<in_p->set_length; ++i)
    R_AtomicValue_Free(&in_p->set_members[i].sm_value);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <R_Set_Free"));

  rpc_ss_free((char *)in_p);
}
/*
 * Release timeout storage
 */
void 
R_Timeout_Free (cds_Timeout_t *in_p)
{
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >R_Timeout_Free"));

  if (in_p != (cds_Timeout_t *)NULL)
    rpc_ss_free((char *)in_p);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <R_Timeout_Free"));
}

/*
 * Free up nidl malloc space for a updatePacket
 */
void 
R_UpdatePkt_Free (cds_UpdatePkt_t *in_p)
{
  int i;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >R_UpdatePkt_Free"));

  for (i=0; i<in_p->pkt_cnt; ++i)
    R_AtomicValue_Free(&in_p->pkt_data[i].ud_value);

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <R_UpdatePkt_Free"));

  rpc_ss_free((char *)in_p);
}
/*
 * Free up nidl malloc space for a set
 */
void 
R_AtomicValue_Free (cds_AtomicValue_t *in_p)
{  
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >R_AtomicValue_Free"));

  switch (in_p->av_valuetype)
    {
    case VT_none:
    case VT_long:
    case VT_short:
    case VT_small:
    case VT_uuid:
    case VT_Timestamp:
    case VT_Timeout:
    case VT_Version:
      break;
    case VT_char:
      rpc_ss_free((char *)in_p->av_val.av_char_p);
      break;
    case VT_ASN1:
    case VT_byte:
      rpc_ss_free((char *)in_p->av_val.av_byte_p);
      break;
    case VT_ReplicaPointer:
      rpc_ss_free((char *)in_p->av_val.av_rp_p);
      break;
    case VT_GroupMember:
      rpc_ss_free((char *)in_p->av_val.av_gm_p);
      break;
    case VT_ParentPointer:
      rpc_ss_free((char *)in_p->av_val.av_pp_p);
      break;
    case VT_gdaPointer:
      rpc_ss_free((char *)in_p->av_val.av_gda_p);
      break;
    case VT_FullName:
      rpc_ss_free((char *)in_p->av_val.av_fullname_p);
      break;
    case VT_CHDirectory:
      rpc_ss_free((char *)in_p->av_val.av_cp_p);
      break;
#if defined(DCE_SEC)
    case VT_DACL:
      R_Acl_Free(in_p->av_val.av_acl_p, TRUE);
      break;

#endif
    }
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <R_AtomicValue_Free: @ end"));
}


#if defined(DCE_SEC)

/*
 * Free the wire format of an sec_acl_entry_t
 */
void 
R_AclEntry_Free (sec_acl_entry_t *entry_p)
{
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >R_AclEntry_Free"));

    switch (entry_p->entry_info.entry_type) {
      case sec_acl_e_type_user_obj:
      case sec_acl_e_type_group_obj:
      case sec_acl_e_type_unauthenticated:
      default:
	/* No dynamic storage here */
	break;
      case sec_acl_e_type_user:
      case sec_acl_e_type_group:
      case sec_acl_e_type_foreign_other:
	if (entry_p->entry_info.tagged_union.id.name)
	  rpc_ss_free(entry_p->entry_info.tagged_union.id.name);
	break;
      case sec_acl_e_type_foreign_user:
      case sec_acl_e_type_foreign_group:
	if (entry_p->entry_info.tagged_union.foreign_id.id.name)
	  rpc_ss_free(entry_p->entry_info.tagged_union.foreign_id.id.name);
	if (entry_p->entry_info.tagged_union.foreign_id.realm.name)
	  rpc_ss_free(entry_p->entry_info.tagged_union.foreign_id.realm.name);
	break;
      case sec_acl_e_type_extended:
	if (entry_p->entry_info.tagged_union.extended_info)
	  rpc_ss_free(entry_p->entry_info.tagged_union.extended_info);
	break;
    } /* end switch */
  
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <R_AclEntry_Free"));
}
/*
 * Free the wire format of an sec_acl_t
 */
void 
R_Acl_Free (sec_acl_t  *acl_p,
            int        all)
{
    int             i;


    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >R_Acl_Free"));

    /* For each entry, free its space */
    for (i=0; i<acl_p->num_entries; ++i)
	R_AclEntry_Free(&acl_p->sec_acl_entries[i]);

    /* Free realm name */
    if (all == TRUE) {
	if (acl_p->default_realm.name)
	  rpc_ss_free(acl_p->default_realm.name);
	rpc_ss_free(acl_p->sec_acl_entries);
	rpc_ss_free(acl_p);
    }

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <R_Acl_Free"));
}
#endif

/*
 * Unpack the tower set in a replica pointer form the nidl fromat
 * to the DNS Set structure.
 */
byte_t *
RPTowerUnpack (byte_t  *in_p,
               Set_t   *set_p)
{
  int cnt,offset,i,len;
  MemberValue_t *member_p;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >RPTowerUnpack"));

  /* Get count of set members */
  cnt = GETL16(in_p);
  if (cnt == 0) {
    ZERO_Set(set_p);

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <RPTowerUnpack: return(SKIP_Set(set_p))"));

    return(SKIP_Set(set_p));
  }

  /* Build Set header */
  INS8(set_p->st_flag, ST_present);
  INS16(set_p->st_offsetLength, cnt*sizeof(unsigned short));

  offset=sizeof(Set_overhead) + EXT16(set_p->st_offsetLength);

  for (i=0; i<cnt; ++i) {
    /* Set offset value for this member */
    INS16(OFFSET_Set(set_p, i), offset);

    member_p = (MemberValue_t *)INDEX_Set(set_p, i);

    /* Set up header */
    INS8(member_p->mv_flag, MV_present);
    ZERO_Timestamp(member_p->mv_timestamp);
    INS8(member_p->mv_valuetype, VT_byte);
    len = GETL16(in_p);
    INS16(member_p->mv_length, len);
    COPY_bytes(in_p, member_p->mv_value, EXT16(member_p->mv_length));
    in_p = SKIP_bytes(in_p, EXT16(member_p->mv_length));

    /* Update offset value for next member */
    offset += EXT16(member_p->mv_length) + sizeof(MemberValue_overhead);
  }
  /* Set length of all values */
  INS16(set_p->st_valueLength, offset - sizeof(Set_overhead) -
	EXT16(set_p->st_offsetLength));

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <RPTowerUnpack: @ end - return(SKIP_Set(set_p))"));

  return(SKIP_Set(set_p));
}
/*
 * Convert a replicapointers set of towers to a little endian counted
 * stream of bytes .  This is done because the timestamps on the
 * member values are not important and the overhead to NIDLize another
 * set is not worth the effort.
 */
byte_t *
RPTowerPack (Set_t                 *set_p,
             cds_ReplicaPointer_t  *rp_p)
{
  register byte_t *ptr_p = rp_p->rp_towers;
  MemberValue_t *member_p;
  int cnt,i;


  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " >RPTowerPack"));

  cnt = EXT16(set_p->st_offsetLength) / sizeof(unsigned short);

  PUTL16(ptr_p, cnt);
  for (i=0; i<cnt; ++i) {
    member_p = (MemberValue_t *)INDEX_Set(set_p, i);

    PUTL16(ptr_p, EXT16(member_p->mv_length));
    COPY_bytes(member_p->mv_value, ptr_p, EXT16(member_p->mv_length));
    ptr_p = SKIP_bytes(ptr_p, EXT16(member_p->mv_length));

  }
  rp_p->rp_length = ptr_p - rp_p->rp_towers;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug8,
      " <RPTowerPack"));

  return(SKIP_Set(set_p));
}


#if defined(DCE_CDS_DEBUG) || defined(INSTRUMENT)

/*
 * cds_types instrumemtation
 */

static deb_ascii_bits_t prog_flags[] = {
    {PR_done, PR_done, "done"},
    {PR_up, PR_up, "up"},
    {PR_linked, PR_linked, "linked"},
    {PR_hitLink, PR_hitLink, "hitLink"},
    {PR_ignoreState, PR_ignoreState, "ignoreState"},
#if defined(PR_directory)
    {PR_directory, PR_directory, "directory"},
#endif
    {0,0,0}
};

char *
deb_ascii_bits (char              *str,
                unsigned long     data,
                deb_ascii_bits_t  *b)
{
    int first = 1;

 
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " >deb_ascii_bits"));

    *str++ = '{';
    for (; b->name; b++)
	if ((data&b->mask) == b->val) {
	    data &= ~b->mask;
	    if (first)
		first = 0;
	    else
		*str++ = ',';
	    strcpy(str, b->name);
	    str = strchr(str, '\0');
	}
    if (data) {
	if (!first) *str++ = ',';
	sprintf(str, "0x%X", data);
	str = strchr(str, '\0');
    }
    *str++ = '}';

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " <deb_ascii_bits: @ end"));

    return(str);
}

char *
deb_ascii_PDU (char           *str,
               unsigned long  data)
{
    char *fmt = "%u";


    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " >deb_ascii_PDU"));

    switch (data) {
    case PDU_Success:		fmt = "Success";			break;
    case PDU_InvalidArgument:	fmt = "InvalidArgument";		break;
    case PDU_InvalidName:	fmt = "InvalidName";			break;
    case PDU_NoLocalResources:	fmt = "NoLocalResources";		break;
    case PDU_NoCommunication:	fmt = "NoCommunication";		break;
    case PDU_AccessViolation:	fmt = "AccessViolation";		break;
    case PDU_CannotAuthenticate: fmt = "CannotAuthenticate";		break;
    case PDU_ConflictingArguments: fmt = "ConflictingArguments";	break;
    case PDU_TimeoutNotDone:	fmt = "TimeoutNotDone";			break;
    case PDU_TimeoutMaybeDone:	fmt = "TimeoutMaybeDone";		break;
    case PDU_EntryExists:	fmt = "EntryExists";			break;
    case PDU_UnknownEntry:	fmt = "UnknownEntry";			break;
    case PDU_NotSupported:	fmt = "NotSupported";			break;
    case PDU_NotImplemented:	fmt = "NotImplemented";			break;
    case PDU_InvalidUpdate:	fmt = "InvalidUpdate";			break;
    case PDU_UnknownClearinghouse: fmt = "UnknownClearinghouse";	break;
    case PDU_NotAReplica:	fmt = "NotAReplica";			break;
    case PDU_AlreadyReplica:	fmt = "AlreadyReplica";			break;
    case PDU_CrucialReplica:	fmt = "CrucialReplica";			break;
    case PDU_NotEmpty:		fmt = "NotEmpty";			break;
    case PDU_NotLinked:		fmt = "NotLinked";			break;
    case PDU_PossibleCycle:	fmt = "PossibleCycle";			break;
    case PDU_DanglingLink:	fmt = "DanglingLink";			break;
    case PDU_NotAGroup:		fmt = "NotAGroup";			break;
    case PDU_ClearinghouseDown:	fmt = "ClearinghouseDown";		break;
    case PDU_badEpoch:		fmt = "badEpoch";			break;
    case PDU_BadClock:		fmt = "BadClock";			break;
    case PDU_DataCorruption:	fmt = "DataCorruption";			break;
    case PDU_WrongAttributeType: fmt = "WrongAttributeType";		break;
    case PDU_MoreThanOneReplica: fmt = "MoreThanOneReplica";		break;
    case PDU_CannotPutHere:	fmt = "CannotPutHere";			break;
    case PDU_OldSkulk:		fmt = "OldSkulk";			break;
    case PDU_UntrustedCH:	fmt = "UntrustedCH";			break;
    case PDU_VersionSkew:	fmt = "VersionSkew";			break;
    case PDU_NotNameserver:	fmt = "NotNameserver";			break;
    case PDU_WrongState:	fmt = "WrongState";			break;
    case PDU_BadNickname:	fmt = "BadNickname";			break;
    case PDU_LocalOnly:		fmt = "LocalOnly";			break;
    case PDU_NotRoot:		fmt = "NotRoot";			break;
    case PDU_NotCHDirectory:	fmt = "NotCHDirectory";			break;
    case PDU_RootLost:		fmt = "RootLost";			break;
    case PDU_CannotUpgrade:	fmt = "CannotUpgrade";			break;
    case PDU_Bug:		fmt = "Bug";				break;
    }
    sprintf(str, fmt, data);

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " <deb_ascii_PDU"));

    return(str);
}

char *
deb_ascii_cds_Name (char        *str,
                    cds_Name_t  *data)
{
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " >deb_ascii_cds_Name"));

    if (data)
	deb_ascii_AttributeName(str, data->nm_name);
    else
	strcpy(str, "NULL cds_Name_t");

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " <deb_ascii_cds_Name"));

    return(str);
}

char *
deb_ascii_cds_Update (char          *str,
                      cds_Update_t  *data)
{
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " >deb_ascii_cds_Update"));

    if (data) {
	char *op = (data->ud_operation & 1) ? "present" : "absent";
	char *p = str;
	sprintf(p, "{%s,", op);
	p = strchr(p, '\0');
	deb_ascii_AttributeName(p, data->ud_attribute);
	p = strchr(p, '\0');
	*p++ = '}';
	*p = '\0';
    } else
	strcpy(str, "NULL cds_Update_t");

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " <deb_ascii_cds_Update"));

    return(str);
}

char *
deb_ascii_cds_FullName (char            *str,
                        cds_FullName_t  *data)
{
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " >deb_ascii_cds_FullName"));

    if (data) {
	char *p = str;
	if (NE_ObjUID(&data->fn_root, cdsWildRoot)) {
	    if (NE_ObjUID(&data->fn_root, NullFullName)) {
		deb_ascii_ObjUID(p, &data->fn_root);
		p = strchr(p, '\0');
	    } else
		*p++ = '0';

	    *p++ = ':';
	}
	COPY_bytes(data->fn_name, p, data->fn_length);
	p[data->fn_length] = '\0';
    } else
	strcpy(str, "NULL cds_FullName_t");

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " <deb_ascii_cds_FullName"));

    return(str);
}

char *
deb_ascii_cds_status (char          *str,
                      cds_status_t  *data)
{
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " >deb_ascii_cds_status"));

    if (data) {
	deb_ascii_PDU(str, (unsigned long)data->er_status);
	if (data->er_name) {
	    char *p = strchr(str, '\0');
	    *p++ = ':';
	    deb_ascii_cds_FullName(p, data->er_name);
	}
    } else
	strcpy(str, "NULL cds_status_t");

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " <deb_ascii_cds_status"));

    return(str);
}

char *
deb_ascii_cds_ReplicaPointer (char                  *str,
                              cds_ReplicaPointer_t  *data)
{
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " >deb_ascii_cds_ReplicaPointer"));

    if (data) {
	char *p = str, *fmt;

	*p++ = '{';
	switch (data->rp_type) {
	case RT_master:		fmt = "master";		break;
	case RT_secondary:	fmt = "secondary";	break;
	case RT_readOnly:	fmt = "readOnly";	break;
#if defined(RT_gda)
	case RT_gda:		fmt = "GDA";		break;
#endif
	default:		fmt = "%u";		break;
	}
	sprintf(p, fmt, data->rp_type);
	p = strchr(p, '\0');
	deb_ascii_cds_FullName(p, &data->rp_chname);
	p = strchr(p, '\0');
	*p++ = '}';
	*p = '\0';
    } else
	strcpy(str, "NULL cds_ReplicaPointer_t");

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " <deb_ascii_cds_ReplicaPointer"));

    return(str);
}

char *
deb_ascii_cds_AtomicValue (char               *str,
                           cds_AtomicValue_t  *data)
{ 
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " >deb_ascii_cds_AtomicValue"));

    if (data) {
	switch (data->av_valuetype) {
	case VT_ReplicaPointer:
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_library,
		svc_c_debug9,
		" <deb_ascii_cds_AtomicValue: deb_ascii_cds_ReplicaPointer(...)"));
	    return deb_ascii_cds_ReplicaPointer(str, data->av_val.av_rp_p);
	}
	sprintf(str, "type(%d)", data->av_valuetype);
    } else
	strcpy(str, "NULL cds_AtomicValue_t");

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " <deb_ascii_cds_AtomicValue"));

    return(str);
}

char *
deb_ascii_cds_SetMember (char             *str,
                         cds_SetMember_t  *data)
{   
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " >deb_ascii_cds_SetMember"));

    if (data)
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_library,
          svc_c_debug9,
          " <deb_ascii_cds_SetMember: deb_ascii_cds_AtomicValue(...)"));

	return deb_ascii_cds_AtomicValue(str, &data->sm_value);
    }
    

    strcpy(str, "NULL cds_SetMember_t");

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " <deb_ascii_cds_SetMember"));

    return(str);
}

char *
deb_ascii_cds_Set (char       *str,
                   cds_Set_t  *data)
{   
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " >deb_ascii_cds_Set"));

    if (data) {
	char *p = str;
	int i;

	*p++ = '{';
	for (i = 0; i < data->set_length; i++) {
	    if (i) *p++ = ',';
	    if (DEB_ASCII_MAX_cds_Set < i) {
		*p++ = '.';
		*p++ = '.';
		*p++ = '.';
		break;
	    }
	    deb_ascii_cds_SetMember(p, &data->set_members[i]);
	    p = strchr(p, '\0');
	}
	*p++ = '}';
	*p = '\0';
    } else
	strcpy(str, "NULL cds_Set_t");

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " <deb_ascii_cds_Set"));

    return(str);
}

char *
deb_ascii_cds_Progress (char            *str,
                        cds_Progress_t  *data)
{ 
    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " >deb_ascii_cds_Progress"));

    if (data) {
	char *p = str;
	*p++ = '{';
	p = deb_ascii_bits (p, data->pr_flags, prog_flags);
	*p++ = ',';
	deb_ascii_cds_FullName(p, &data->pr_unresolved);
	p = strchr(p, '\0');
	*p++ = ',';
	deb_ascii_cds_FullName(p, &data->pr_resolved);
	p = strchr(p, '\0');
	if (data->pr_replicas_p) {
	    *p++ = ',';
	    deb_ascii_cds_Set(p, data->pr_replicas_p);
	    p = strchr(p, '\0');
	}
	*p++ = '}';
	*p = '\0';
    } else
	strcpy(str, "NULL cds_Progress_t");

    DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_library,
      svc_c_debug9,
      " <deb_ascii_cds_Progress"));

    return(str);
}

#endif /* defined(DCE_CDS_DEBUG) || defined(INSTRUMENT) */
