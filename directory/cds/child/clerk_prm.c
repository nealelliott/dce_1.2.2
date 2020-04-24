/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerk_prm.c,v $
 * Revision 1.1.6.2  1996/02/18  19:25:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:07  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:04:14  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:03  root]
 * 
 * Revision 1.1.4.3  1994/06/23  18:28:51  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:54:15  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  16:07:39  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:22  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  21:56:24  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:50:58  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  12:41:01  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:26:43  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:27:18  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * clerk_prm.c
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
 * MODULE DESCRIPTION:
 *
 *	Handle library/child protocol parameter parsing
 *
 * Networks & Communications Software Engineering
 */

#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <stdio.h>

#include <uc_child.h>
#include <uc_clerk.h>

/*
 * Maneuver around the parameter
 * Pointer is at possible input value.
 * Before pointer is length of entire parameter
 * Right before end of parameter is length of buffer for return value
 * Before return length is address for return value
 */
#define PRM_beg_p(ptr) SKIP_bytes(ptr,-sizeof(field16))
#define PRM_end_p(ptr) SKIP_bytes(ptr,EXT16(PRM_beg_p(ptr)))
#define PRM_len_p(ptr) SKIP_bytes(PRM_end_p(ptr), -sizeof(field16))
#define PRM_adr_p(ptr) SKIP_bytes(PRM_len_p(ptr), -UCP_address_LEN)



/*
 * instrumentation display aids
 */
byte_t 
*prm_out_value (const thread_t *const  th_p,
                const void *const      ptr)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >prm_out_value"));

    if (ptr) {
	const address_t *const match_p = EXTaddress(PRM_adr_p(ptr));
	byte_t *buf_p = SKIP_bytes(th_p->clerkRspBuf_p, sizeof(ucp_resp_header));

	while (buf_p < th_p->clerkRspMsgEnd_p) {
	    const address_t *const adr_p = GETaddress(buf_p);
	    const int length = GET16(buf_p);

	   if (adr_p == match_p)
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_child,
		    svc_c_debug9,
		    " <prm_out_value: return(buf_p)"));

		return(buf_p);
	    }
	    
	    buf_p = SKIP_bytes(buf_p, length);
	}
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <prm_out_value: at end: return(NULL)"));

    return(NULL);
}

char 
*deb_ascii_prm_buffer (char *const        buf_p,
                       const void *const  ptr)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >deb_ascii_prm_buffer"));

    if (ptr) 
    {
	char *next_p = buf_p;


	*next_p++ = '{';
	next_p = deb_ascii_ptr_to_buf(next_p, 
                                      (void *)EXTaddress(PRM_adr_p(ptr)));
	(void)sprintf(next_p, ",%d}", EXT16(PRM_len_p(ptr)));
    } 
    else
    {
	strcpy(buf_p, "{0,0}");
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <deb_ascii_prm_buffer: return(buf_p)"));

    return(buf_p);
}

/*
 *	p r m _ i n _ l e n g t h
 *
 * Get length of IN parameter
 */
int 
prm_in_length (prm_in_t *prm_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >prm_in_length"));

    if (prm_p)
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_child,
          svc_c_debug9,
          " <prm_in_length: EXT16"));

	return(EXT16(PRM_beg_p(prm_p)));
    }
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <prm_in_length: return(0)"));

    return(0);
}

/*
 *	p r m _ o u t _ l e n g t h
 *
 * Get length of OUT parameter
 */
int 
prm_out_length (prm_out_t *prm_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >prm_out_length"));

    if (prm_p)
    {
	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_child,
          svc_c_debug9,
          " <prm_out_length: EXT16"));

	return(EXT16(PRM_len_p(prm_p)));
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <prm_out_length: return(0)"));

    return(0);
}


/*
 *	p r m _ b e g i n
 *
 * Initialize for construction of response packet
 */
void 
prm_begin (thread_t *th_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >prm_begin"));

    if (th_p->clerkRspMsgEnd_p =
		SKIP_bytes(th_p->clerkRspBuf_p, sizeof(ucp_resp_header))) {
	if (th_p->clerkFromCache_p) {
	    PUTaddress(th_p->clerkRspMsgEnd_p, th_p->clerkFromCache_p);
	    PUT16(th_p->clerkRspMsgEnd_p, UCP_address_LEN);
	    PUTaddress(th_p->clerkRspMsgEnd_p, th_p->clerkFromCache);
	}
	if (th_p->clerkOutLink_p) {
	    PUTaddress(th_p->clerkRspMsgEnd_p, th_p->clerkOutLink_p);
	    PUT16(th_p->clerkRspMsgEnd_p, UCP_address_LEN);
	    PUTaddress(th_p->clerkRspMsgEnd_p, th_p->outLinked);
	}
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <prm_begin"));
}

/*
 *	p r m _ e n d
 *
 * Record length of response packet
 */
void 
prm_end (thread_t *th_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >prm_end"));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <prm_end"));
}

/*
 *      p r m _ c o p y
 *
 * Set up parameter copy if result is requested
 */
void 
prm_copy (thread_t   *th_p,
          prm_out_t  *prm_p,
          byte_t     *arg_p,
          int        len)
{

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >prm_copy"));
    
    if (th_p->clerkRspMsgEnd_p && prm_p) {
	address_t *const adr = EXTaddress(PRM_adr_p(prm_p));

	if (adr && (0 < len) && (len <= EXT16(PRM_len_p(prm_p)))) {
	    PUTaddress(th_p->clerkRspMsgEnd_p, adr);
	    PUT16(th_p->clerkRspMsgEnd_p, len);
	    COPY_bytes(arg_p, th_p->clerkRspMsgEnd_p, len);
	    th_p->clerkRspMsgEnd_p = SKIP_bytes(th_p->clerkRspMsgEnd_p, len);
	}
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >prm_copy"));
}

/*
 *      p r m _ a d r
 *
 * Set up parameter copy if result is requested
 */
static const byte_t 
*prm_adr (const thread_t *const   th_p,
          const prm_out_t *const  prm_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >prm_adr"));

    if (th_p->clerkRspMsgEnd_p && prm_p) 
    {
	const byte_t *const adr_p = PRM_adr_p(prm_p);

	if (EXTaddress(adr_p)) 
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_child,
		svc_c_debug9,
		" <prm_adr: return(adr_p)"));

	    return(adr_p);
	}
	
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <prm_adr: return(NULL)"));

    return(NULL);
}

/*
 *	p r m _ o u t _ A t t r i b u t e C o n t e n t s
 *
 * Store a AttributeContents value
 */
void 
prm_out_AttributeContents (thread_t   *th_p,
                           prm_out_t  *prm_p,
                           byte_t     *arg_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >prm_out_AttributeContents"));

    prm_copy(th_p, prm_p, arg_p, LEN_AttributeContents(arg_p));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <prm_out_AttributeContents"));
}

/*
 *	p r m _ o u t _ B O O L E A N
 *
 * Store a BOOLEAN parameter
 */
void 
prm_out_BOOLEAN (thread_t   *th_p,
                 prm_out_t  *prm_p,
                 byte_t     *arg_p)
{
    const byte_t *const prm_adr_p = prm_adr(th_p, prm_p);


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >prm_out_BOOLEAN:  prm_p=Ox%x  arg_p=Ox%x",
        prm_p,
        arg_p));

    if (prm_adr_p)
    {
	switch (EXT16(PRM_len_p(prm_p))) 
	{
	case 1:
	    COPY_bytes(prm_adr_p, th_p->clerkRspMsgEnd_p, PRM_buffer_LEN);
	    th_p->clerkRspMsgEnd_p = SKIP_bytes(th_p->clerkRspMsgEnd_p, PRM_buffer_LEN);
	    PUT8(th_p->clerkRspMsgEnd_p, EXT8(arg_p));
	    break;

	case 2:
	    COPY_bytes(prm_adr_p, th_p->clerkRspMsgEnd_p, PRM_buffer_LEN);
	    th_p->clerkRspMsgEnd_p = SKIP_bytes(th_p->clerkRspMsgEnd_p, PRM_buffer_LEN);
	    PUT16(th_p->clerkRspMsgEnd_p, EXT8(arg_p));
	    break;

	case 4:
	    COPY_bytes(prm_adr_p, th_p->clerkRspMsgEnd_p, PRM_buffer_LEN);
	    th_p->clerkRspMsgEnd_p = SKIP_bytes(th_p->clerkRspMsgEnd_p, PRM_buffer_LEN);
	    PUT32(th_p->clerkRspMsgEnd_p, EXT8(arg_p));
	    break;
	}
    }
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <prm_out_BOOLEAN"));
}

/*
 *	p r m _ o u t _ T i m e o u t
 *
 *  Parse and store a Timeout value
 */
void 
prm_out_Timeout (thread_t   *th_p,
                 prm_out_t  *prm_p,
                 byte_t     *arg_p)
{
    const byte_t *const prm_adr_p = prm_adr(th_p, prm_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >prm_out_Timeout"));

    if (prm_adr_p)
	switch (EXT16(PRM_len_p(prm_p))) {
	case sizeof(Timeout_u):
	    COPY_bytes(prm_adr_p, th_p->clerkRspMsgEnd_p, PRM_buffer_LEN);
	    th_p->clerkRspMsgEnd_p = SKIP_bytes(th_p->clerkRspMsgEnd_p,
						PRM_buffer_LEN);
	    COPY_Timeout(arg_p, th_p->clerkRspMsgEnd_p);
	    th_p->clerkRspMsgEnd_p = SKIP_Timeout(th_p->clerkRspMsgEnd_p);
	    break;
	}

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <prm_out_Timeout"));
}

/*
 *	p r m _ o u t _ V e r s i o n N u m b e r
 *
 *  Parse and store a VersionNumber value
 */
void 
prm_out_VersionNumber (thread_t   *th_p,
                       prm_out_t  *prm_p,
                       byte_t     *arg_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >prm_out_VersionNumber"));

    prm_copy(th_p, prm_p, arg_p, LEN_VersionNumber(arg_p));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <prm_out_VersionNumber"));
}

/*
 *	p r m _ o u t _ I n t e g e r
 *
 *  Parse and store an integer length value
 */
void 
prm_out_Integer (thread_t   *th_p,
                 prm_out_t  *prm_p,
                 byte_t     *arg_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >prm_out_Integer"));

    prm_copy(th_p, prm_p, arg_p, sizeof(int));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <prm_out_Integer"));
}

/*
 *	t a _ b e g i n
 *
 * Initialize for TA response parsing
 */
void 
ta_begin (thread_t *th_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >ta_begin"));

    prm_begin(th_p);
    th_p->taRspMsgPtr_p = th_p->taRspMsg_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <ta_begin"));
}

/*
 *	t a _ e n d
 *
 * Check for success of parsing
 */
void 
ta_end (thread_t *th_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >ta_end"));

    prm_end(th_p);
 
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <ta_end"));
}

/*
 *	t a _ A t t r i b u t e C o n t e n t s
 *
 * Parse and store a AttributeContents value
 */
byte_t 
*ta_AttributeContents (thread_t   *th_p,
                       prm_out_t  *prm_p)
{
    byte_t *const arg_p = th_p->taRspMsgPtr_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >ta_AttributeContents"));

    if (arg_p)
	th_p->taRspMsgPtr_p = SKIP_AttributeContents(arg_p);

    prm_copy(th_p, prm_p, arg_p, LEN_AttributeContents(arg_p));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <ta_AttributeContents: return(arg_p)"));

    return(arg_p);
}

/*
 *	t a _ B O O L E A N
 *
 * Parse and store a BOOLEAN parameter
 */
byte_t 
*ta_BOOLEAN (thread_t   *th_p,
             prm_out_t  *prm_p)
{
    byte_t *const arg_p = th_p->taRspMsgPtr_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >ta_BOOLEAN "));

    if (arg_p)
	th_p->taRspMsgPtr_p = SKIP_BOOLEAN(arg_p);

    prm_out_BOOLEAN(th_p, prm_p, arg_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <ta_BOOLEAN: return(arg_p)"));

    return(arg_p);
}

/*
 *	t a _ P r o g r e s s
 *
 * Save the address of the progress record and skip it
 */
byte_t 
*ta_Progress (thread_t *th_p)
{
    byte_t *const arg_p = th_p->taRspMsgPtr_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >ta_Progress"));

    if (arg_p)
	th_p->taRspMsgPtr_p = SKIP_Progress(arg_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <ta_Progress: return(arg_p)"));

    return(arg_p);
}

/*
 *	t a _ P r o g r e s s _ r e s o l v e d N a m e
 *
 * Parse and store the resolvedName from the Progress record
 */
byte_t 
*ta_Progress_resolvedName (thread_t   *th_p,
                           prm_out_t  *prm_p)
{
    byte_t	*arg_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >ta_Progress_resolvedName"));

    if (th_p->taRspMsgPtr_p == th_p->taRspMsg_p)
	ta_Progress(th_p);

    arg_p = SKIP_FullName(((Progress_t *)th_p->taRspMsg_p)->pr_unresolvedName);
    prm_copy(th_p, prm_p, arg_p, LEN_FullName(arg_p));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <ta_Progress_resolvedName: return(arg_p)"));

    return(arg_p);
}

/*
 *	ta_ReadAttrResponse_retToClerk
 *
 * Return and skip the returningToClerk field
 *  in the ReadAttributeResponse record
 */
byte_t 
ta_ReadAttrResponse_retToClerk (thread_t *th_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >ta_ReadAttrResponse_retToClerk"));

    if (th_p->taRspMsgPtr_p)
    {
 	DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_child,
          svc_c_debug9,
          " <ta_ReadAttrResponse_retToClerk: return(th_p...)"));

	return(*th_p->taRspMsgPtr_p++);
    }
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <ta_ReadAttrResponse_retToClerk: return(0)"));

    return(0);
}

/*
 *	t a _ S e t
 *
 * Parse and store a Set value
 */
byte_t 
*ta_Set (thread_t   *th_p,
         prm_out_t  *prm_p)
{
    byte_t *const arg_p = th_p->taRspMsgPtr_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >ta_Set"));

    if (arg_p)
	th_p->taRspMsgPtr_p = SKIP_Set(arg_p);

    prm_copy(th_p, prm_p, arg_p, LEN_Set(arg_p));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <ta_Set: return(arg_p)"));

    return(arg_p);
}

/*
 *	t a _ T i m e o u t
 *
 *  Parse and store a Timeout value
 */
byte_t 
*ta_Timeout (thread_t   *th_p,
             prm_out_t  *prm_p)
{
    byte_t *const arg_p = th_p->taRspMsgPtr_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >ta_Timeout"));

    if (arg_p)
	th_p->taRspMsgPtr_p = SKIP_Timeout(arg_p);

    prm_out_Timeout(th_p, prm_p, arg_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <ta_Timeout: return(arg_p)"));

    return(arg_p);
}

/*
 *	t a _ O b j U I D
 *
 * Parse and store a uid value.  For cds this is a uuid_t otherwise
 * a timestamp
 */
byte_t 
*ta_ObjUID (thread_t   *th_p,
            prm_out_t  *prm_p)
{
    byte_t *const arg_p = th_p->taRspMsgPtr_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >ta_ObjUID"));

    if (arg_p)
	th_p->taRspMsgPtr_p = SKIP_ObjUID(arg_p);

    prm_copy(th_p, prm_p, arg_p, LEN_ObjUID(arg_p));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <ta_ObjUID: return(arg_p)"));

    return(arg_p);
}
/*
 *	t a _ T i m e s t a m p
 *
 * Parse and store a Timestamp value
 */
byte_t 
*ta_Timestamp (thread_t   *th_p,
               prm_out_t  *prm_p)
{
    byte_t *const arg_p = th_p->taRspMsgPtr_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >ta_Timestamp"));

    if (arg_p)
	th_p->taRspMsgPtr_p = SKIP_Timestamp(arg_p);

    prm_copy(th_p, prm_p, arg_p, LEN_Timestamp(arg_p));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <ta_Timestamp: return(arg_p)"));

    return(arg_p);
}

/*
 *	t a _ V e r s i o n N u m b e r
 *
 * Parse and store a VersionNumber value
 */
byte_t 
*ta_VersionNumber (thread_t   *th_p,
                   prm_out_t  *prm_p)
{
    byte_t *const arg_p = th_p->taRspMsgPtr_p;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >ta_VersionNumber"));

    if (arg_p)
	th_p->taRspMsgPtr_p = SKIP_bytes(arg_p, LEN_VersionNumber(arg_p));

    prm_copy(th_p, prm_p, arg_p, LEN_VersionNumber(arg_p));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <ta_VersionNumber: return(arg_p)"));

    return(arg_p);
}

/*
 *	t a _ W h o l e E n t r y
 *
 * Parse and store a WholeEntry value
 */
byte_t 
*ta_WholeEntry (thread_t   *th_p,
                prm_out_t  *prm_p)
{
    byte_t *const arg_p = th_p->taRspMsgPtr_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >ta_WholeEntry"));

    if (arg_p)
	th_p->taRspMsgPtr_p = SKIP_WholeEntry(arg_p);

    prm_copy(th_p, prm_p, arg_p, LEN_WholeEntry(arg_p));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <ta_WholeEntry: return(arg_p)"));

    return(arg_p);
}
