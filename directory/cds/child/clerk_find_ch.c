/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerk_find_ch.c,v $
 * Revision 1.1.8.2  1996/02/18  19:25:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:08:59  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:03:55  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:37:50  root]
 * 
 * Revision 1.1.6.4  1994/08/03  18:59:22  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:10:49  mccann]
 * 
 * Revision 1.1.6.3  1994/06/23  18:28:44  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  18:53:41  mccann]
 * 
 * Revision 1.1.6.2  1994/06/09  16:07:33  devsrc
 * 	cr10892 - fix copyright
 * 	[1994/06/09  15:52:14  devsrc]
 * 
 * Revision 1.1.6.1  1994/03/12  21:56:15  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  13:50:01  peckham]
 * 
 * Revision 1.1.4.1  1993/10/14  16:05:58  peckham
 * 	CR#7759: Cannot use more than 2 clearinghouses on one host.
 * 	Add confidence parameter to clerk_find_ch().
 * 	Also change multiple returns to single exit.
 * 	[1993/10/11  03:38:55  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  12:40:35  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:26:05  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:26:47  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Program Clerk, Module clerk_find_ch
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
 *	Find the location of a clearinghouse.  This routine is called when
 *	either the clearinghouse was not in the cache, or the user requested
 *	a higher confidence.
 *
 * Networks & Communications Software Engineering
 * 
 */
#include <dce/dce.h>
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <uc_child.h>
#include <uc_clerk.h>
#include <dce/attributes.h>

/*
 * Function prototypes
 */
int 
clerk_find_ch (thread_t *,
               FullName_t *,
               int,
               CHlist_t **);


/*
 *		c l e r k _ f i n d _ c h
 *
 * This routine reads the address and uid of the specified clearinghouse
 * object and adds the information to the cache.  The information is obtained
 * by calling a known clearinghouse and asking for the DNA_Towers and
 * DNS_CTS/DNS_OBJECTUID attributes of the given clearinghouse.  
 * If towers is not there we get DNS_Address assuming it is a phaseIV server.
 *
 */
int 
clerk_find_ch (thread_t    *th_p,
               FullName_t  *name_p,
               int         conf,
               CHlist_t    **ch_pp)
{
    DEB_ASCII_BUF_thread(thBuf)
    DEB_ASCII_BUF_FullName(nameBuf)
    DEB_ASCII_BUF_CHlist(chBuf)
    DEB_ASCII_BUF_dns_status(statusBuf)
    AttributeContents_t *address_p;

    /*
     * Space for conversion of one address to a set with one member 
     */
    struct Rbuff {
      bytes_u clerkbuf[MAX_MSG_SIZE];
      Set_u   st_hdr;
      SetMember_u st_member;
    };

    struct Rbuff *readbuf_p;
    MemberValue_t *member_p;
    Set_t *set_p;
    ObjUID_t saveid;
    CHlist_t *ch_p = NULL;
    int status;


    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " >clerk_find_ch(%s,%s,%d,%p) level(%d)",
        deb_ascii_thread(thBuf,th_p),
        deb_ascii_FullName(nameBuf,name_p),
        conf,
        ch_pp,
        th_p->level));

    /* Only allow 3 nested calls.  Otherwise we could looping forever */
    if (th_p->level++ >= 3) 
    {
	status = DNS_NOCOMMUNICATION;
	goto leave_clerk_find_ch;
    }

    /* 
     * Get space for read attribute, keep off stack cause of recursion
     * and VAX threads limit of 65K/thread and pthreads is smaller
     */
    readbuf_p = (struct Rbuff *)dns_malloc(sizeof(struct Rbuff));
    if (readbuf_p == (struct Rbuff *)0) {
	status = DNS_NONSRESOURCES;
	goto leave_clerk_find_ch;
    }

    address_p = (AttributeContents_t *)readbuf_p->clerkbuf;
    /*
     * Read the DNS_OBJECTUID/DNS_CTS and DNA_Towers of clearinghosue object.  
     * Use maybeMore flag of 1 so that
     * next read doesn't need to contact clearinghouse.
     */
    /* Do dns_objectuid first since it will always return a result */
    status = read_att(th_p, name_p, ET_object,
		      dns_AttributeName(DNS_OBJECTUID),
		      (Timestamp_t *)NullTimestamp, BL_true, MAX_MSG_SIZE, 
		      address_p, conf, th_p->level);
    if (status == DNS_SUCCESS) { 
	/* Extract id from buffer for next read */
	member_p = (MemberValue_t *)address_p->ac_single;
	COPY_ObjUID(member_p->mv_value, &saveid);

	/* Now try towers => assumes phaseV node for clearinghouse */
	status = read_att(th_p, name_p, ET_object, 
			dns_AttributeName(DNA_TOWERS),
			(Timestamp_t *)NullTimestamp, BL_false, MAX_MSG_SIZE, 
			address_p, conf, th_p->level);
	set_p = (Set_t *)address_p->ac_set;
      
    }

    if (status == DNS_SUCCESS) {
	ch_p = (CHlist_t *)dns_malloc(sizeof(*ch_p)
			- sizeof(ch_p->ch_name) + LEN_FullName(name_p)
			- sizeof(ch_p->ch_set) + LEN_Set(set_p));
	if (ch_p) {
	    /* Start filling it in */
	    ch_p->ch_flags.init= 0;
	    ch_p->ch_count = 1;
	    COPY_ObjUID(&saveid, ch_p->ch_id);
	    COPY_FullName(name_p, ch_p->ch_name);
	    COPY_Set(set_p, SKIP_FullName(ch_p->ch_name));
	} else
	    status = DNS_NONSRESOURCES;
    } else if (status == DNS_UNKNOWNENTRY)
	/* Since looking for a clearinghouse, be specific */
	status = DNS_UNKNOWNCLEARINGHOUSE;

    /* All done */
    dns_free((char *)readbuf_p);

leave_clerk_find_ch:

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_child,
        svc_c_debug6,
        " <clerk_find_ch(%s,%s,%d,%p->%s) return(%s)",
        deb_ascii_thread(thBuf,th_p),
        deb_ascii_FullName(nameBuf,name_p),
        conf,
        ch_pp,
        deb_ascii_CHlist(chBuf,ch_p),
        deb_ascii_dns_status(statusBuf,status)));
    
    *ch_pp = ch_p;
    return(status);
}








