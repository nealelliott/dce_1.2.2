/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerkreadattribute.c,v $
 * Revision 1.1.11.2  1996/02/18  19:26:42  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:09:32  marty]
 *
 * Revision 1.1.11.1  1995/12/08  15:05:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:38:42  root]
 * 
 * Revision 1.1.9.5  1994/08/03  18:59:48  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:17:50  mccann]
 * 
 * Revision 1.1.9.4  1994/06/23  18:29:13  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:55:56  mccann]
 * 
 * Revision 1.1.9.3  1994/06/09  16:08:01  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:46  devsrc]
 * 
 * Revision 1.1.9.2  1994/04/14  14:41:16  peckham
 * 	New module clerk_socket.c to isolate SVR4 named pipes.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	Remove msgblk.h, cds_event_init() and evp references.
 * 	Remove v1v2 code (v1 never used for cds).
 * 	Remove VMS and UNIX conditionals, VMS code.
 * 	Remove last vestige of clerk_trace().
 * 	[1994/04/11  19:36:38  peckham]
 * 
 * Revision 1.1.9.1  1994/03/12  21:56:56  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:53:35  peckham]
 * 
 * Revision 1.1.7.1  1993/10/12  14:57:53  peckham
 * 	CR5596: null partial results.
 * 	CreateDirectory needs the partialResults from a failed
 * 	read_att(<parent directory>).
 * 	[1993/10/10  22:15:42  peckham]
 * 
 * 	CR5596: null partial results.
 * 	CreateDirectory needs the partialResults from a failed
 * 	read_att(<parent directory>).
 * 
 * Revision 1.1.4.4  1993/03/09  19:44:46  peckham
 * 	OT#7411: Request given to gda when other clearinghouse
 * 	available. Requests using ET_dirOrObj were not setting
 * 	cache_the_dir, causing WalkTree not to match the cellname.
 * 	[1993/03/09  19:40:22  peckham]
 * 
 * Revision 1.1.4.3  1992/12/30  12:42:42  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:28:45  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/11  14:42:03  keegan
 * 	Fix memory leak in 'read_att' and 'resolvename'.  Clearinghouse list
 * 	added to 'thread_t' structure on 'cloned' thread is never release.
 * 	[1992/12/11  14:39:51  keegan]
 * 
 * Revision 1.1.2.2  1992/03/22  21:29:20  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:10:10  weisman]
 * 
 * Revision 1.1  1992/01/19  15:27:20  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module clerkreadattribute.c
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
 *	Clerk.ReadAttribute command processor.
 *
 * Networks & Communications Software Engineering
 *
 */
#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <uc_clerk.h>
#include <uc_child.h>
#include <cache.h>
#include <dce/attributes.h>

#define ONEATTRIBUTE		1

static AttributeContents_t *find_att (
  WholeEntry_t	*in_p,
  AttributeName_t *att_p);

static byte_t	wholeSet = {BL_true};

/*
 * ReadAttribute arguments (passed in the thread block)
 */
#define	ENTRY_P		clerkArgs[0]
#define	LOOKINGFOR_P	clerkArgs[1]
#define	ATTRIBUTE_P	clerkArgs[2]
#define	MAYBEMORE_P	clerkArgs[3]
#define	CONTEXTVARIABLE_P clerkArgs[4]
#define	CLEARINGHOUSE_P	clerkArgs[5]
#define	OUTVALUES_P	clerkArgs[6]
#define	RET_P		clerkArgs[7]
#define CTS_P           clerkArgs[8]
#define ADDRESS_P       clerkArgs[9]
#define ADDRESSLEN_P    clerkArgs[10]


/*
 * Local Prototype declarations.
 */

byte_t 
*index_att (AttributeContents_t  *,
            int );

static AttributeContents_t 
*find_att (WholeEntry_t     *,
           AttributeName_t  *);

int 
read_att (thread_t *,
          FullName_t *,
          int, 
          AttributeName_t *,
          Timestamp_t *,
          int,
          int,
          AttributeContents_t *,
          int,
          int);

static int 
ReadAttributeRPC (thread_t  *,
                  Set_t     *,
                  ObjUID_t  *);

int
buildReadAttributeRequest (thread_t *);

static void 
parseReadAttributeResponse (thread_t *);

int 
buildReadReplicaRequest (thread_t *);

static void 
parseReadReplicaResponse (thread_t *);

int 
clerkReadAttributeCache (thread_t  *,
                         int       *);

void 
clerkReadAttribute (thread_t *);

void 
clerkReadReplica (thread_t *);

byte_t 
*clerkgetch (thread_t *);


/*
 *	i n d e x _ a t t
 *
 * Get address of next data item in AttributeContents Set
 *
 * Arguments:
 *	atc_p = pointer to AttributeContents
 *	index = which entry in set to get (starting at 0)
 */

byte_t 
*index_att (AttributeContents_t  *atc_p,
            int                  index)
{
    byte_t	*ptr = atc_p->ac_single;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >index_att"));

    switch (EXT8(atc_p->ac_type)) {
    case AT_set:
	if (index < NUM_Set(ptr)) {
	    ptr = INDEX_Set(ptr, index);
	    break;
	}
	DCE_SVC_DEBUG((
           cds__svc_handle,
           cds_svc_child,
           svc_c_debug9,
           " <index_att: (1) return(NULL)"));
	return(NULL);
    case AT_single:
	if (index == 0)
	    break;
    case AT_null:
    default:
	DCE_SVC_DEBUG((
           cds__svc_handle,
           cds_svc_child,
           svc_c_debug9,
           " <index_att: (2) return(NULL)"));
	return(NULL);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <index_att: @end"));

    return(((MemberValue_t *)ptr)->mv_value);
}

/*
 *	f i n d _ a t t
 *
 * Locate specified AttributeName in WholeEntry
 * and return pointer to associated AttributeContents
 *
 * Arguments:
 *	in_p =  pointer to WholeEntry
 *	att  =  pointer to required AttributeName
 */

static AttributeContents_t 
*find_att (WholeEntry_t     *in_p,
           AttributeName_t  *att_p)
{
    byte_t	*ptr_p = in_p->wh_entry;
    int		count = EXT8(in_p->wh_numberOfAttribtues);


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >find_att: in_p=%x  att_p=%x  count=%d",
	in_p,att_p,count));

    /*
     * First byte is the number of sets in this buffer.
     */
    for (; 0 < count; --count) {
	dns_opq_sname *const an_p = (dns_opq_sname *)ptr_p;

	ptr_p = SKIP_AttributeName(ptr_p);

	if (dnsCmpSimple((dns_opq_sname *)att_p, an_p) == 0)
	{
	    DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_child,
                svc_c_debug9,
                " <find_att: dnsCmpSimple() returned 0"));

	    return((AttributeContents_t *)ptr_p);
	}
	
	ptr_p = SKIP_AttributeContents(ptr_p);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <find_att: @ end: return(NULL)"));

    return(NULL);
}

/*
 *	r e a d _ a t t
 *
 * Routine used by clerk_find_ch.c and clerk_tree_walk.c
 *
 * Arguments:
 *	name_p =  name of entry which contains attribute
 *	type =    entry type (i.e., directory, object, etc.)
 *	att_p =   name of the attribute (i.e. DNS$Address)
 *	context = beginning position of value read
 *	outlen =  length of user requested output buffer
 *	buffer =  for clerk internal use
 *	flag =    for clerk internal use
 *
 * Returns:
 *	errors from lower level routines
 */
int 
read_att (thread_t             *th_p,
          FullName_t           *name_p,
          int                  type_in,
          AttributeName_t      *att_p,
          Timestamp_t          *context,
          int                  flag_in,
          int                  outlen,
          AttributeContents_t  *buffer,
          int                  confidence,
          int                  level)
{
    static int	wait = 0;
    struct {
	word_u		length;
	address_u	address;
	word_u		return_length;
    } outValues;
    thread_t		*readth_p;
    byte_t		*values_p = (byte_t *)&outValues;
    byte_t		*saveClerkRspMsg = NULL;
    EntryType_u		type;
    BOOLEAN_u		flag;
    byte_t whole;
    int status;
    int stale;
    byte_t linked;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug5,
        " >read_att"));

    INS8(type, type_in);
    INS8(flag, flag_in);

    /* check cache first */
    if (confidence == CO_low)
      if (CAReadAttribute(th_p->user, 0, name_p, (EntryType_t *)type, att_p,
	context, outlen, buffer, &whole, &stale, &linked) == DNS_SUCCESS) {
	++CAfixed_p->ca_hits;
	++CAfixed_p->ca_readoperations;

	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug5,
            " <read_att: return(DNS_SUCCESS)"));

	return(DNS_SUCCESS);
      }
      /* Change confidence to skip cache, output not the same */
      else confidence = CO_medium;

    readth_p = alloc_child(th_p->link_p, th_p);
    
    if (readth_p == NULL) 
    {
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug5,
            " <read_att: readth_p == NULL -> return(DNS_NONSRESOURCES)"));

	return(DNS_NONSRESOURCES);
    }
    

    readth_p->taOperation = OP_ReadAttribute;
    readth_p->taObj = 44;
    readth_p->level = level;
    readth_p->OPtype = CA_CNT_READ;

    /* Set up response length for deterimining max_size */
    readth_p->clerkRspBufLen = MAX_MSG_SIZE;

    readth_p->ENTRY_P = (prm_in_t *)name_p;
    readth_p->LOOKINGFOR_P = (prm_in_t *)type;
    readth_p->ATTRIBUTE_P = (prm_in_t *)att_p;
    readth_p->MAYBEMORE_P = (prm_in_t *)flag;
    readth_p->CONTEXTVARIABLE_P = (prm_in_t *)context;
    readth_p->clerkConf = confidence;
    readth_p->clerkWait = wait;
    readth_p->clerkStatus = DNS_SUCCESS;
    readth_p->CLEARINGHOUSE_P = (prm_in_t *)NullFullName;

    /* Set up dummy output for parsereadattributeresponse */
    PUT16(values_p, UCP_address_LEN+sizeof(field16));
    readth_p->OUTVALUES_P = values_p;
    PUTaddress(values_p, NULL);
    INS16(values_p, outlen);

    /*
     * If this request is for CreateDirectory, then we need to collect
     * the partialResults in case of failure.
     */
    if (th_p->taOperation == OP_CreateDirectory) {
      readth_p->clerkPartialRes =
	  UCP_EXTaddress(th_p->clerkReq.partialResults);
      readth_p->clerkResLength = UCP_EXTlongword(th_p->clerkReq.resLen);
      readth_p->clerkResLength_p = UCP_EXTaddress(th_p->clerkReq.resLen_p);
      readth_p->clerkOutLink_p =
	  UCP_EXTaddress(th_p->clerkReq.outLink_p);
      saveClerkRspMsg = readth_p->clerkRspMsg_p;
    }

    clerkReadAttribute(readth_p);

    if (readth_p->clerkStatus == DNS_SUCCESS) {
      byte_t	*ptr = SKIP_Progress(readth_p->taRspMsg_p);
      AttributeContents_t *val_p;


      /* The parse routine has already converted attribute names
       * to their v2 format.  Since the length doesn't change
       * the conversion is done in place and will be correct here.
       */
      if (*ptr++ != ONEATTRIBUTE)
	val_p = find_att((WholeEntry_t *)ptr, att_p);
      else
	val_p = (AttributeContents_t *)ptr;

	COPY_AttributeContents((byte_t *)val_p, buffer);
    } else if ((readth_p->clerkStatus == DNS_UNKNOWNENTRY)
		&& saveClerkRspMsg) {
	const int len = readth_p->clerkRspMsgEnd_p - saveClerkRspMsg;
	/*
	 * During directory creation, a ReadAttribute is done on the parent
	 * to get the Replicas. If this fails, the call immediately must fail.
	 * But, we must return partialResults - and this must come from here.
	 * The data is there - in readth_p->clerkRspBuf_p - so let's copy
	 * the data back for CreateDirectory.
	 * Not a pretty solution, but what is clean in this stuff anyway?
	 */
	if (len) {
	    COPY_bytes(saveClerkRspMsg, th_p->clerkRspMsg_p, len);
	    th_p->clerkRspMsgEnd_p = SKIP_bytes(th_p->clerkRspMsg_p, len);
	}
    }
    status = readth_p->clerkStatus;
    if (readth_p->cle_p != NULL)
        CAClearinghousesUsed(readth_p->cle_p);
    free_child(readth_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug5,
        " <read_att: return(status=%d)",status));

    return(status);
}

/*
 *	R e a d A t t r i b u t e R P C
 *
 * Routine to call RPC stub for ReadAttribute request
 *
 */
static int 
ReadAttributeRPC (thread_t  *th_p,
                  Set_t     *tower_p,
                  ObjUID_t  *uuid_p)
{
    rpc_binding_handle_t  h;
    unsigned long max_size = prm_out_length(th_p->OUTVALUES_P);
    unsigned long max_len;
    int i;
    cds_Timestamp_t context;
    cds_Name_t attname;
    cds_Progress_t Progress;
    error_status_t rpcStatus,commStatus = rpc_s_no_bindings;
    cds_status_t  dnsStatus;
    cds_RA_value_t value;
    byte_t wholeset;
    int fromcache, getout=0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >ReadAttributeRPC"));

    /* Set up from readreplica */
    th_p->taOperation = OP_ReadAttribute;

    /* Set up structures for calling RPC */
    Build_R_Progress((byte_t *)th_p->taProgress.flags,
		     (Timeout_t *)th_p->taProgress.Timeout,
		     th_p->taProgress.Unresolved,
		     th_p->taProgress.Resolved,
		     &Progress);
    (void)L_to_R_AttributeName((AttributeName_t *)th_p->ATTRIBUTE_P, &attname);
    (void)L_to_R_Timestamp((Timestamp_t *)th_p->CONTEXTVARIABLE_P, &context);
    value.returningToClerk = RA_none;

    /*
     * Determine maximum buffer size for clerk.
     * Adjust size by out parameter overhead of 6 bytes each +
     * size of output buffers
     */
    max_len = th_p->clerkRspBufLen - ENUM_READ_OVERHEAD;
    if (max_size > max_len)
      max_size = max_len;

    /*
     * First attempt to find a cached handle.  This
     * assumes their is one handle/clearinghouse.  There
     * is a use count associated with handles, so make
     * sure either free or release cached handle is called
     */
    if ((fromcache = get_cached_handle(&h, uuid_p, th_p)) != 0)
      commStatus = cds_ReadAttribute(h, &Progress,
				     EXT8(th_p->LOOKINGFOR_P), &attname,
				     &context, max_size,
				     EXT8(th_p->MAYBEMORE_P), &value,
				     &wholeset, &dnsStatus);

    if (commStatus != rpc_s_ok) {
	if (fromcache) {
	    free_cached_handle(th_p, h);
	    fromcache = 0;
	}

	/* Now process no cached handle */
	for (i=0; i<NUM_Set(tower_p); ++i) {
	    if (!get_rpc_handle(&h, (MemberValue_t *)INDEX_Set(tower_p, i),
				uuid_p, th_p, &getout))
		if (getout) break;
		else continue;
	    commStatus = cds_ReadAttribute(h, &Progress,
					   EXT8(th_p->LOOKINGFOR_P), &attname,
					   &context, max_size,
					   EXT8(th_p->MAYBEMORE_P), &value,
					   &wholeset, &dnsStatus);

	    if (commStatus == rpc_s_ok) break;
	    maximize_rpc_error(th_p, commStatus, &getout);
	    /* Free handle and try again */
	    rpc_binding_free(&h, &rpcStatus);
	    if (getout) break;
	} /* End for loop */
    }

    /* Process success case */
    if (commStatus == rpc_s_ok) {
	th_p->taRspMsgPtr_p = th_p->taRspMsg_p;
	if (dnsStatus.er_status == PDU_Success) {
	    th_p->taRspType = MT_Response;
	    th_p->taRspMsgPtr_p = R_to_L_Progress(&Progress,
				  (Progress_t *)th_p->taRspMsgPtr_p, TRUE);
	    if (FLAG_IS_SET(&Progress.pr_flags, PR_done)) {
	     switch (value.returningToClerk) {
	       case RA_none:
		 break;
	       case RA_single:
		 PUT8(th_p->taRspMsgPtr_p, ONEATTRIBUTE);
		 PUT8(th_p->taRspMsgPtr_p, AT_single);
		 th_p->taRspMsgPtr_p =
		   R_to_L_MemberValue(value.tagged_union.value_single_p,
				  (MemberValue_t *)th_p->taRspMsgPtr_p, FALSE);
		R_AtomicValue_Free(&value.tagged_union.value_single_p->sm_value);
		rpc_ss_free((char *)value.tagged_union.value_single_p);
		PUT8(th_p->taRspMsgPtr_p, wholeset);
		break;
	      case RA_set:
		PUT8(th_p->taRspMsgPtr_p, ONEATTRIBUTE);
		PUT8(th_p->taRspMsgPtr_p, AT_set);
		th_p->taRspMsgPtr_p =
		    R_to_L_Set(value.tagged_union.value_set_p,
			       (Set_t *)th_p->taRspMsgPtr_p, FALSE);
		R_Set_Free(value.tagged_union.value_set_p);
		PUT8(th_p->taRspMsgPtr_p, wholeset);
		break;
	      case RA_wholeSet:
		 DCE_SVC_DEBUG((
		     cds__svc_handle, 
		     cds_svc_library, 
		     svc_c_debug4, 
		     "  ReadAttributeRPC: Line %d - th_p = 0x%x  value = 0x%x",
		     __LINE__,
		     th_p,
		     value));
 
		PUT8(th_p->taRspMsgPtr_p, 2);
		PUT8(th_p->taRspMsgPtr_p,
			value.tagged_union.wholeEntry_p->numberOfAttributes);

		for (i = 0; i < value.tagged_union.wholeEntry_p->numberOfAttributes; i++) 
		{
		    cds_WE_entry_t *we_p = &value.tagged_union.wholeEntry_p->entry[i];

		  th_p->taRspMsgPtr_p = R_to_L_AttributeName(&we_p->name,
				(AttributeName_t *)th_p->taRspMsgPtr_p, FALSE);
		  PUT8(th_p->taRspMsgPtr_p, we_p->value.type);
		  switch (we_p->value.type) {
		  case RA_none:
		    break;
		  case RA_single:
		    th_p->taRspMsgPtr_p =
			R_to_L_MemberValue(we_p->value.tagged_union.single_p,
			       (MemberValue_t *)th_p->taRspMsgPtr_p, FALSE);
		    R_AtomicValue_Free(&we_p->value.tagged_union.single_p->sm_value);
		    rpc_ss_free((char *)we_p->value.tagged_union.single_p);
		    break;
		  case RA_set:
		    th_p->taRspMsgPtr_p =
			    R_to_L_Set(we_p->value.tagged_union.set_p,
				   (Set_t *)th_p->taRspMsgPtr_p, FALSE);
		    R_Set_Free(we_p->value.tagged_union.set_p);
		    break;
		  }
		}
		rpc_ss_free((char *)value.tagged_union.wholeEntry_p);
		break;
	      }
	    }
	  } else {
	    th_p->taRspType = MT_Exception;
	    th_p->taRspMsgPtr_p = R_to_L_Exception(&dnsStatus,
				       th_p->taRspMsgPtr_p, TRUE);
	    }
	R_Set_Free(Progress.pr_replicas_p);
	/* Save handle for later */
	CACHE_HANDLE(fromcache, h, th_p);

	DCE_SVC_DEBUG((
           cds__svc_handle,
           cds_svc_child,
           svc_c_debug4,
           " <ReadAttributeRPC: return(DNS_SUCCESS)"));

    	return(DNS_SUCCESS);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <ReadAttributeRPC: return (DNS_NOCOMMUNICATION)"));

    return (DNS_NOCOMMUNICATION);
}

/*
 *	b u i l d R e a d A t t r i b u t e R e q u e s t
 *
 * Routine to build an ReadAttribute request message and
 * return length of data
 *
 * Inputs:
 *	th_p->taReqMsgEnd_p	buffer to receive message arguments
 *				if zero, only figure length.
 *
 * Returns:
 *	total length of message arguments.
 */
int
buildReadAttributeRequest (thread_t *th_p)
{
    int length = 0;
    int max_size = prm_out_length(th_p->OUTVALUES_P);
    int max_len;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >buildReadAttributeRequest"));
  /*
   * Determine maximum buffer size for clerk.
   * Adjust size by out parmeter overhead of 6 bytes each +
   * size of output buffers
   */
  max_len = th_p->clerkRspBufLen - ENUM_READ_OVERHEAD;
  if (max_size > max_len)
    max_size = max_len;


    /*
     * ReadAttributeRequest message:
     *	progress: Progress
     *	entryType: EntryType
     *	context: Timestamp
     *	maybeMore: BOOLEAN
     *	attribute: SimpleName
     *	maxsize: INTEGER
     */
    length += buildProgress(th_p);
    length += LEN_EntryType(th_p->LOOKINGFOR_P);
    length += LEN_Timestamp(th_p->CONTEXTVARIABLE_P);
    length += LEN_BOOLEAN(th_p->MAYBEMORE_P);
    length += LEN_SimpleName(th_p->ATTRIBUTE_P);
    length += LEN_INTEGER(&max_size);

    if (th_p->taReqMsgEnd_p) {
	COPY_EntryType(th_p->LOOKINGFOR_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_EntryType(th_p->taReqMsgEnd_p);

	COPY_Timestamp(th_p->CONTEXTVARIABLE_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_Timestamp(th_p->taReqMsgEnd_p);

	COPY_BOOLEAN(th_p->MAYBEMORE_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_BOOLEAN(th_p->taReqMsgEnd_p);

	COPY_SimpleName(th_p->ATTRIBUTE_P, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_SimpleName(th_p->taReqMsgEnd_p);

	COPY_INTEGER(&max_size, th_p->taReqMsgEnd_p);
	th_p->taReqMsgEnd_p = SKIP_INTEGER(th_p->taReqMsgEnd_p);
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <buildReadAttributeRequest: return(length=%d)",length));

    return(length);
}

/*
 *	p a r s e R e a d A t t r i b u t e R e s p o n s e
 *
 * This local routine parses the response message.
 */
static void 
parseReadAttributeResponse (thread_t *th_p)
{
    FullName_t *name_p;
    byte_t *whole_p;
    byte_t returntype;
    byte_t *out_p = NULL;
    int i,cnt;
    AttributeContents_t *results_p;
    bytes_u emptyattribute[sizeof(char) + sizeof(NULLSET_u)];


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >parseReadAttributeResponse"));

    /*
     * ReadAttributeResponse message:
     *	progress: Progress
     *	returningToClerk:
     *	    oneAttribute
     *		result: AttributeContents
     *		wholeSet: BOOLEAN
     *	    wholeEntry
     *		wholeEntry: WholeEntry
     */
    ta_begin(th_p);
    (void)ta_Progress(th_p);

    returntype = ta_ReadAttrResponse_retToClerk(th_p);
    if (returntype != ONEATTRIBUTE)  {
      byte_t *newout_p = ta_WholeEntry(th_p, NULL);

      out_p = newout_p;

      results_p = find_att((WholeEntry_t *)out_p,
			     (AttributeName_t *)th_p->ATTRIBUTE_P);
      whole_p = &wholeSet;
    }
    else {
      results_p = (AttributeContents_t *)ta_AttributeContents(th_p, NULL);
      /* Get pointer to wholset results */
      whole_p = ta_BOOLEAN(th_p, NULL);
    }

    /*
     * ensure result_p is valid. If not dump core !
     */
    if (results_p == NULL)
    {
      results_p = (AttributeContents_t *)emptyattribute;
      INS8(results_p->ac_type, AT_set);
      ZERO_Set(results_p->acx_union);

      whole_p = &wholeSet;

      DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_child,
            svc_c_debug3,
            " <parseReadAttributeResponse: result_p == NULL"));
    }
    
    prm_out_AttributeContents(th_p, th_p->OUTVALUES_P, (byte_t *)results_p);
    prm_out_BOOLEAN(th_p, th_p->RET_P, whole_p);
    ta_end(th_p);

    if (th_p->taRspMsg_p ) {
      /* Get name with soft links resolved */
      name_p = (FullName_t *)
	    SKIP_FullName(((Progress_t *)th_p->taRspMsg_p)->pr_unresolvedName);

      /* Convert and cache other attributes returned */
      if (!th_p->clerkDontCache) {
	EntryType_u lookingFor;

#if (defined(ET_dirOrObj) && defined(PR_directory))
	if (EXT8(th_p->LOOKINGFOR_P) == ET_dirOrObj) {
	  if (FLAG_IS_SET(th_p->taProgress.flags, PR_directory)) {
	    INS8(lookingFor, ET_directory);
	  } else {
	    INS8(lookingFor, ET_object);
	  }
	} else
#endif
	  COPY_EntryType(th_p->LOOKINGFOR_P, lookingFor);

	if (returntype != ONEATTRIBUTE) {
	  AttributeContents_t *att_p;

	  /* Move pointer back to beginning of whole entry */
	  results_p = (AttributeContents_t *)out_p;

	  /* Cache each attribute */
	  for (cnt = EXT8(out_p++), i=0; i < cnt; ++i) {

	    /* Convert network address to tower set */
	    att_p = (AttributeContents_t *)SKIP_AttributeName(out_p);

	    (void)CAInsertAttribute(th_p->user, name_p,
	      (EntryType_t *)lookingFor, (AttributeName_t *)out_p,
	      (Timestamp_t *)NullTimestamp, att_p, wholeSet);

	    /* Adjust whole entry pointer */
	    out_p = SKIP_AttributeContents((byte_t *)att_p);
	  }
	}
	else
	  (void)CAInsertAttribute(th_p->user, name_p,
			    (EntryType_t *)lookingFor,
			    (AttributeName_t *)th_p->ATTRIBUTE_P,
			    (Timestamp_t *)th_p->CONTEXTVARIABLE_P,
			    results_p, EXT8(whole_p));
      } /* End don't cache */
    } /* End msg pointer present */

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <parseReadAttributeResponse"));
}

/*
 *	b u i l d R e a d R e p l i c a R e q u e s t
 *
 * Routine to build an ReadReplica request message and
 * return length of data
 *
 * Inputs:
 *	th_p->taReqMsgEnd_p	buffer to receive message arguments
 *				if zero, only figure length.
 *
 * Returns:
 *	total length of message arguments.
 */
int 
buildReadReplicaRequest (thread_t *th_p)
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >buildReadReplicaRequest"));

    th_p->taOperation = OP_ReadAttribute;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <buildReadReplicaRequest"));

    return(buildReadAttributeRequest(th_p));

}

/*
 *	p a r s e R e a d R e p l i c a R e s p o n s e
 *
 * This local routine parses the response message.
 */
static void 
parseReadReplicaResponse (thread_t *th_p)
{
    CleEntry_t *cle_p;
    int len=0;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " >parseReadReplicaResponse"));

    parseReadAttributeResponse(th_p);

    cle_p = CALookupClearinghouse((FullName_t *)th_p->CLEARINGHOUSE_P);
    if (cle_p) {
      len = LEN_Set(SKIP_FullName(cle_p->ch_name));
      if (len >  prm_out_length(th_p->OUTVALUES_P))
	prm_out_Integer(th_p, th_p->ADDRESSLEN_P, (byte_t *)&len);
      else {
	prm_copy(th_p, th_p->ADDRESS_P, SKIP_FullName(cle_p->ch_name), len);
	prm_out_Integer(th_p, th_p->ADDRESSLEN_P, (byte_t *)&len);
      }
      dns_free((char *)cle_p);
    }
    else
      /* Return 0 length */
      prm_out_Integer(th_p, th_p->ADDRESSLEN_P, (byte_t *)&len);

    ta_end(th_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug9,
        " <parseReadReplicaResponse"));
}

/*
 *	c l e r k R e a d A t t r i b u t e C a c h e
 *
 * Check the cache for an attribute.
 *
 * Inputs:
 *	ENTRY_P		FullName  of entry which contains attribute
 *	LOOKINGFOR_P	EntryType (i.e., directory, object, etc.)
 *	ATTRIBUTE_P	SimpleName of the attribute
 *	MAYBEMORE_P	BOOLEAN
 *	CONTEXTVARIABLE_P Timestamp of beginning position of value read
 *	CLEARINGHOUSE_P	FullName of related clearinghouse (optional)
 *
 * Outputs:
 *	OUTVALUES_P	AttributeContents
 *
 * Returns:
 *	RET_P		BOOLEAN
 */
int 
clerkReadAttributeCache (thread_t  *th_p,
                         int       *stale_p)
{
    int max_size,max_len;
    byte_t whole;
    byte_u acbuf[MAX_MSG_SIZE];


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug5,
        " >clerkReadAttributeCache"));

  /* Only check cache if confidence is low */
  if ((th_p->clerkConf != CO_low) ||
      NE_bytes(NullFullName, th_p->CLEARINGHOUSE_P, sizeof(NullFullName))) {
    ++CAfixed_p->ca_bypasses;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug5,
        " <clerkReadAttributeCache: (1) return(FALSE)"));

    return(FALSE);
  }


  /* Get user buffer size */
  max_size = prm_out_length(th_p->OUTVALUES_P);

  /*
   * Determine maximum buffer size for clerk.
   * Adjust size by out parmeter overhead of 6 bytes each +
   * size of output buffers for outlinked and wholeentry.
   */
  max_len = th_p->clerkRspBufLen - ENUM_READ_OVERHEAD;
  if (max_len > sizeof(acbuf))
    max_len = sizeof(acbuf);

  if (max_size > max_len)
    max_size = max_len;

  if (CAReadAttribute(th_p->user, th_p->clerkCacheTimeout,
		      (FullName_t *)th_p->ENTRY_P,
		      (EntryType_t *)th_p->LOOKINGFOR_P,
		      (AttributeName_t *)th_p->ATTRIBUTE_P,
		      (Timestamp_t *)th_p->CONTEXTVARIABLE_P, max_size,
		      (AttributeContents_t *)acbuf,
		      &whole, stale_p, &th_p->outLinked) != DNS_SUCCESS)
  {
      DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug5,
        " <clerkReadAttributeCache: (2) return(FALSE)"));

      return(FALSE);
  }
    


  th_p->clerkFromCache++;
  ++CAfixed_p->ca_hits;
  ++CAfixed_p->ca_readoperations;

  /* Build output buffer */
  prm_begin(th_p);
  prm_out_AttributeContents(th_p, th_p->OUTVALUES_P, (byte_t *)acbuf);
  prm_out_BOOLEAN(th_p, th_p->RET_P, &whole);
  prm_end(th_p);

  th_p->clerkStatus = DNS_SUCCESS;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug5,
      " <clerkReadAttributeCache: return(TRUE)"));

  return(TRUE);
}


/*
 *	c l e r k R e a d A t t r i b u t e
 *
 * Read attribute -- reads the values of an entry
 *
 * Inputs:
 *	ENTRY_P		FullName  of entry which contains attribute
 *	LOOKINGFOR_P	EntryType (i.e., directory, object, etc.)
 *	ATTRIBUTE_P	SimpleName of the attribute
 *	MAYBEMORE_P	BOOLEAN
 *	CONTEXTVARIABLE_P Timestamp of beginning position of value read
 *	CLEARINGHOUSE_P	FullName of related clearinghouse (optional)
 *
 * Outputs:
 *	OUTVALUES_P	AttributeContents
 *
 * Returns:
 *	RET_P		BOOLEAN
 */
void 
clerkReadAttribute (thread_t *th_p)
{

#if defined(DCE_CDS_DEBUG)

    DEB_ASCII_BUF_FullName(entryBuf)
    DEB_ASCII_BUF_EntryType(lookingForBuf)
    DEB_ASCII_BUF_AttributeName(attributeBuf)
    DEB_ASCII_BUF_BOOLEAN(maybeMoreBuf)
    DEB_ASCII_BUF_Timestamp(contextVariableBuf)
    DEB_ASCII_BUF_FullName(clearinghouseBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)

#endif

    int stale=0;
    int status;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >clerkReadAttribute(%s,%s,%s,%s,%s,%s,%p,%p)",
        deb_ascii_FullName(entryBuf,th_p->ENTRY_P),
        deb_ascii_EntryType(lookingForBuf,th_p->LOOKINGFOR_P),
        deb_ascii_AttributeName(attributeBuf,th_p->ATTRIBUTE_P),
        deb_ascii_BOOLEAN(maybeMoreBuf,th_p->MAYBEMORE_P),
        deb_ascii_Timestamp(contextVariableBuf,th_p->CONTEXTVARIABLE_P),
        deb_ascii_FullName(clearinghouseBuf,th_p->CLEARINGHOUSE_P),
        th_p->OUTVALUES_P,
        th_p->RET_P));
    /*
     * If clearinghouse not supplied, use walktree, otherwise
     * only query one clearinghouse.
     * Check cache.  If data is stale, continue with walktree and
     * use results only if cannot get to a clearinghouse
     */


    /* look to see if this is LOCAL                                         */
    /* I'm too stupid to parse the fancy parens ENTRY_P->fn_root would need */
    if (clerkReadAttributeCache(th_p, &stale) == FALSE) {

      /*
       * If this is a directory, cache the final progress info.
       */
      if ((EXT8(th_p->LOOKINGFOR_P) == ET_directory)

#ifdef ET_dirOrObj

	  || (EXT8(th_p->LOOKINGFOR_P) == ET_dirOrObj)

#endif

	  || (EXT8(th_p->LOOKINGFOR_P) == ET_clearinghouse))
	th_p->cache_the_dir = 1;

    /*
     * If this is a clearinghouse, and confidence is high,
     * don't need to lookup replica set cause there is only one.
     */
      if (EXT8(th_p->LOOKINGFOR_P) == ET_clearinghouse)
	++th_p->NoReplicas;
      else if (EXT8(th_p->LOOKINGFOR_P) == ET_softlink)
	++th_p->skipResolveNameCache;

      th_p->taBuildReq = buildReadAttributeRequest;
      th_p->taParseRsp = parseReadAttributeResponse;
      th_p->taRPC = ReadAttributeRPC;

      sendToWalkTree(th_p, th_p->ENTRY_P);

      if (th_p->clerkStatus == DNS_TIMEOUTMAYBEDONE)
	th_p->clerkStatus = DNS_TIMEOUTNOTDONE;

      /*
       * If previously found data, but it was stale use it
       * now if there was a communication error
       */
      status = cds_convert_rpc_status(th_p->clerkStatus);
      if ((stale) && ((status == DNS_NOCOMMUNICATION) ||
		      (status == DNS_TIMEOUTNOTDONE) ||
		      (status == DNS_CLEARINGHOUSEDOWN) ||
		      (status == DNS_DATACORRUPTION) ||
		      (status == DNS_UNKNOWNCLEARINGHOUSE))) {
	/* check cache again an take whatever is there */
	th_p->clerkCacheTimeout = 0;
	if (clerkReadAttributeCache(th_p, &stale) == TRUE) {
	  th_p->clerkStatus = DNS_SUCCESS_STALE;
	}
      }
    }

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <clerkReadAttribute(%s,%s,%s,%s,%s,%s,%p,%p) status(%s)",
        deb_ascii_FullName(entryBuf,th_p->ENTRY_P),
        deb_ascii_EntryType(lookingForBuf,th_p->LOOKINGFOR_P),
        deb_ascii_AttributeName(attributeBuf,th_p->ATTRIBUTE_P),
        deb_ascii_BOOLEAN(maybeMoreBuf,th_p->MAYBEMORE_P),
        deb_ascii_Timestamp(contextVariableBuf,th_p->CONTEXTVARIABLE_P),
        deb_ascii_FullName(clearinghouseBuf,th_p->CLEARINGHOUSE_P),
        th_p->OUTVALUES_P,
        th_p->RET_P,
        deb_ascii_dns_status(statusBuf,th_p->clerkStatus)));
}

/*
 *	c l e r k R e a d R e p l i c a
 *
 * Read replica -- reads the values of an entry
 *
 * Inputs:
 *	ENTRY_P		FullName  of entry which contains attribute
 *	LOOKINGFOR_P	EntryType (i.e., directory, object, etc.)
 *	ATTRIBUTE_P	SimpleName of the attribute
 *	MAYBEMORE_P	BOOLEAN
 *	CONTEXTVARIABLE_P Timestamp of beginning position of value read
 *	CLEARINGHOUSE_P	FullName of related clearinghouse (optional)
 *
 * Outputs:
 *	OUTVALUES_P	AttributeContents
 *
 * Returns:
 *	RET_P		BOOLEAN
 */
void 
clerkReadReplica (thread_t *th_p)
{

#if defined(DCE_CDS_DEBUG)

    DEB_ASCII_BUF_FullName(entryBuf)
    DEB_ASCII_BUF_EntryType(lookingForBuf)
    DEB_ASCII_BUF_AttributeName(attributeBuf)
    DEB_ASCII_BUF_BOOLEAN(maybeMoreBuf)
    DEB_ASCII_BUF_Timestamp(contextVariableBuf)
    DEB_ASCII_BUF_FullName(clearinghouseBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)

#endif


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " >clerkReadReplica(%s,%s,%s,%s,%s,%s,%p,%p)",
        deb_ascii_FullName(entryBuf,th_p->ENTRY_P),
        deb_ascii_EntryType(lookingForBuf,th_p->LOOKINGFOR_P),
        deb_ascii_AttributeName(attributeBuf,th_p->ATTRIBUTE_P),
        deb_ascii_BOOLEAN(maybeMoreBuf,th_p->MAYBEMORE_P),
        deb_ascii_Timestamp(contextVariableBuf,th_p->CONTEXTVARIABLE_P),
        deb_ascii_FullName(clearinghouseBuf,th_p->CLEARINGHOUSE_P),
        th_p->OUTVALUES_P,
        th_p->RET_P));
    /*
     * This is a directory, cache the final progress info.
     */
    th_p->cache_the_dir = 1;

    th_p->taBuildReq = buildReadReplicaRequest;
    th_p->taParseRsp = parseReadReplicaResponse;
    th_p->taRPC = ReadAttributeRPC;

    sendToWalkTree(th_p, th_p->ENTRY_P);

    if (th_p->clerkStatus == DNS_TIMEOUTMAYBEDONE)
      th_p->clerkStatus = DNS_TIMEOUTNOTDONE;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug4,
        " <clerkReadReplica(%s,%s,%s,%s,%s,%s,%p,%p) status(%s)",
        deb_ascii_FullName(entryBuf,th_p->ENTRY_P),
        deb_ascii_EntryType(lookingForBuf,th_p->LOOKINGFOR_P),
        deb_ascii_AttributeName(attributeBuf,th_p->ATTRIBUTE_P),
        deb_ascii_BOOLEAN(maybeMoreBuf,th_p->MAYBEMORE_P),
        deb_ascii_Timestamp(contextVariableBuf,th_p->CONTEXTVARIABLE_P),
        deb_ascii_FullName(clearinghouseBuf,th_p->CLEARINGHOUSE_P),
        th_p->OUTVALUES_P,
        th_p->RET_P,
        deb_ascii_dns_status(statusBuf,th_p->clerkStatus)));
}

/*
 * Get the clearinghouse name from the argument list
 */
byte_t 
*clerkgetch (thread_t *th_p)
{
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug7,
      " >clerkgetch"));

 if ((th_p->taOperation == OP_ReadAttribute) ||
     (th_p->taOperation == OP_ReadReplica))
     if NE_bytes(NullFullName, th_p->CLEARINGHOUSE_P, sizeof(NullFullName))
     {
	 DCE_SVC_DEBUG((
	     cds__svc_handle,
	     cds_svc_child,
	     svc_c_debug7,
	     " <clerkgetch: return(th_p->CLEARINGHOUSE_P)"));

	 return(th_p->CLEARINGHOUSE_P);
     }
  

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_child,
      svc_c_debug7,
      " <clerkgetch: return(NULL)"));

  return(NULL);
}
