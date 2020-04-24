/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: replica_lib.c,v $
 * Revision 1.1.6.2  1996/02/18  19:36:23  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:16:57  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:31:24  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:30  root]
 * 
 * Revision 1.1.4.4  1994/08/03  19:04:24  mccann
 * 	final sams cleanup drop
 * 	[1994/07/19  18:31:24  mccann]
 * 
 * Revision 1.1.4.3  1994/06/09  18:43:37  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:16:10  devsrc]
 * 
 * Revision 1.1.4.2  1994/04/14  14:51:24  peckham
 * 	Remove CMA, VMS and UNIX conditionals.
 * 	Remove dnstables dependencies.
 * 	Use dcecdsmac.h definitions for dce_svc_printf().
 * 	[1994/04/11  20:21:11  peckham]
 * 
 * Revision 1.1.4.1  1994/03/12  22:12:03  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:44:05  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  14:22:36  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:23:07  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:25:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * Module replica_lib.c
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
 */

#include <server.h>
#include <replica.h>


/*
 * replica_parse
 *    This routine parses a replica_pointer value and returns pointers
 * and lengths.  All input parameters are optional, except the replica_pointer
 *
 * Input:
 *     replica_p = replica pointer (required )
 *     totallen_p = address to return value lengh
 *     addr_pp = address to return pointer to rp_address
 *     addr_len_p = address to return rp_addr length
 *     type_p = address to return replica type byte
 *     fname_pp = address to return pointer to full name
 */
void 
replica_parse (ReplicaPointer_t  *replica_p,
               int               *totallen_p,
               Set_t             **addr_pp,
               int               *addr_len_p,
               byte_t            *type_p,
               FullName_t        **fname_pp)
{
    DEB_ASCII_BUF_ReplicaPointer(replicaBuf)

    /*
     * point at the tower set which is after the clearinghouse UID
     */
    byte_t *ptr_p = replica_p->rp_addressHint;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug8,
        " >replica_parse(%s,%p,%p,%p,%p,%p)",
        deb_ascii_ReplicaPointer(replicaBuf,replica_p),
        totallen_p,
        addr_pp,
        addr_len_p,
        type_p,
        fname_pp));

    if (addr_pp)
	*addr_pp = (Set_t *)ptr_p;

    /*
     * if the user wanted a pointer to the addresses, return it
     */
    if (addr_len_p)
	*addr_len_p = LEN_Set(ptr_p);
    
    /*
     * skip over the set and UID to get at the type
     */
    ptr_p = SKIP_Set(ptr_p);

    /* 
     * if the user wanted the type, extract it and return it
     */
    if (type_p)
	*type_p = EXT8(ptr_p);

    /*
     * skip over the type field
     */
    ptr_p = SKIP_ReplicaType(ptr_p);

    /*
     * if the user wanted the full name of the clearinghouse which is next
     * return it
     */
    if (fname_pp)
	*fname_pp = (FullName_t *)ptr_p;

    /*
     * if the user wanted the total length of the replica pointer
     * calculate it and return it
     */
    if (totallen_p)
        *totallen_p = SKIP_FullName(ptr_p) - (byte_t *)replica_p;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug8,
        " <replica_parse(%s,%p,%p,%p,%p,%p)",
        deb_ascii_ReplicaPointer(replicaBuf,replica_p),
        totallen_p,
        addr_pp,
        addr_len_p,
        type_p,
        fname_pp));
}

/*
 * replica_build
 *    This routine builds a replica_pointer 
 *
 * Input:
 *     replica_p = replica pointer 
 *     chid_p = address to clearinghouse uid
 *     addr_p = address to of clearinghouse address (0 if need to get local)
 *     type = replica type
 *     chname_p = address of clearinghouse name
 * 
 * Output:
 *     replica pointer build
 *
 * Value:
 *     length of replica pointer
 */
int 
replica_build (ReplicaPointer_t  *replica_p,
               ObjUID_t          *chid_p,
               Set_t             *addr_p,
               int               type,
               FullName_t        *chname_p)
{
    DEB_ASCII_BUF_ReplicaPointer(replicaBuf)
    DEB_ASCII_BUF_ObjUID(chidBuf)
    DEB_ASCII_BUF_FullName(chnameBuf)
    byte_t         *ptr_p = replica_p->rp_addressHint;
    int             len;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug8,
        " >replica_build(%p,%s,%p,%d,%s)",
        replica_p,
        deb_ascii_ObjUID(chidBuf,chid_p),
        type,
        addr_p,
        deb_ascii_FullName(chnameBuf,chname_p)));

    /* first copy the clearinghouse UID */
    COPY_ObjUID(chid_p, replica_p->rp_CHID);  /* first field is UID */

    /* if we got a tower copy it in, else grab the local address */
    if (!addr_p)
        (void)towerset_build(ptr_p);
    else
	COPY_Set(addr_p, ptr_p);

    if (NETWORKADDRESSMAX < LEN_Set(ptr_p))
	DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_server,
            svc_c_debug8,
	    "replica_build(%p,%s,%p,%d,%s) LEN_Tower(%d)", replica_p,
	    deb_ascii_ObjUID(chidBuf,chid_p), type, addr_p,
	    deb_ascii_FullName(chnameBuf,chname_p),
	    LEN_Set(ptr_p)));

    ptr_p = SKIP_Set(ptr_p);
    /*
     * build a pointer to the type which is after the towerset and insert it
     */
    INS8(ptr_p, type);
    ptr_p = SKIP_ReplicaType(ptr_p);

    /*
     * put in the name of the clearinghouse next
     */
    COPY_FullName(chname_p, ptr_p);

    /*
     * calculate and return the length of the replica pointer
     */
    len = DIFF_bytes(SKIP_FullName(ptr_p), replica_p);

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_server,
        svc_c_debug8,
        " <replica_build(%s,%s,%p,%d,%s) return(%d)",
        deb_ascii_ReplicaPointer(replicaBuf,replica_p),
        deb_ascii_ObjUID(chidBuf,chid_p),
        type,
        addr_p,
        deb_ascii_FullName(chnameBuf,chname_p),
        len));
    return(len);
}
