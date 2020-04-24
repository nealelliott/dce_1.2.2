/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: clerk.h,v $
 * Revision 1.1.10.1  1996/08/09  11:58:08  arvind
 * 	Merge CDS changes from DEC into DCE_1.2.2
 * 	[1996/07/30  15:31 UTC  arvind  /main/DCE_1.2.2/arvind_cds122/1]
 *
 * 	CDS bug fixes from DEC
 * 	[1996/02/18  23:33:35  marty  1.1.8.2]
 *
 * Revision 1.1.8.2  1996/02/18  23:33:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:05  marty]
 * 
 * Revision 1.1.8.1  1995/12/08  15:15:06  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:42:36  root]
 * 
 * Revision 1.1.6.5  1994/08/16  18:27:52  jd
 * 	drop from janet
 * 	[1994/08/13  16:06:14  jd]
 * 
 * Revision 1.1.6.4  1994/08/03  19:02:06  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:55:42  mccann]
 * 
 * Revision 1.1.6.3  1994/06/09  18:38:44  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:14  devsrc]
 * 
 * Revision 1.1.6.2  1994/04/14  14:39:44  peckham
 * 	Split decnet_types into machine dependent cds_basic_types.h
 * 	Removed VMS, UNIX and DOSorOS2 conditionals.
 * 	[1994/04/11  19:57:24  peckham]
 * 
 * Revision 1.1.6.1  1994/03/12  22:02:28  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:12:36  peckham]
 * 
 * Revision 1.1.4.2  1992/12/30  13:28:46  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:46:24  zeliff]
 * 
 * Revision 1.1.2.2  1992/03/22  22:01:47  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:25:41  weisman]
 * 
 * Revision 1.1  1992/01/19  15:16:28  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _CLERK_H
#define _CLERK_H
/*
 * Module clerk.h
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
#include <sys_mbx.h>
#include <dce/dnsclerk.h>

/*
 * clerk_lib.c
 */

void
clerk_init (
    void);

dns_status_t
clerk_cvt_simple_to_string (
    char		*,
    int			,
    char		*,
    int			*);

dns_status_t
clerk_cvt_simple_to_opaque (
    char		*,
    int			,
    char		*,
    int			*,
    byte_t		*);


dns_status_t
clerk_cvt_full_to_opaque (
    char		*,
    int			,
    char		*,
    int			*,
    byte_t		*);

dns_status_t
clerk_resolve_name (
    FullName_t		*,
    FullName_t		*);

dns_status_t
clerk_create_child (
    FullName_t		*,
    byte_t		*,
    ObjUID_t		*,
    Set_t		*,
    ObjUID_t		*);


dns_status_t
clerk_create_object (
    FullName_t		*,
    ClassName_t  	*,
    ClassVersion_t	*,
    ObjUID_t            *,
    Timestamp_t		*);

dns_status_t
clerk_delete_child (
    FullName_t		*);

dns_status_t
clerk_delete_object (
    FullName_t		*);

dns_status_t
clerk_create_slink (
    FullName_t          *,
    FullName_t          *,
    Timeout_t           *,
    Timestamp_t         *);

dns_status_t
clerk_get_preferred_alias (
    FullName_t          *,
    FullName_t          *);

dns_status_t
clerk_skulk (
    FullName_t		*);

dns_status_t
clerk_read_attribute (
    FullName_t		*,
    int			,
    AttributeName_t	*,
    byte_t		*,
    Timestamp_t		*,
    byte_t		*);

dns_status_t
clerk_read_attribute_conf (
    FullName_t          *,
    int                 ,
    AttributeName_t	*,
    byte_t              *,
    Timestamp_t         *,
    byte_t              *,
    dnsConf_t           );

dns_status_t
clerk_test_attribute (
    FullName_t		*,
    int			,
    AttributeName_t	*,
    int			,
    byte_t		*,
    int	                );

dns_status_t
clerk_test_attribute_conf (
    FullName_t		*,
    int			,
    AttributeName_t	*,
    int			,
    byte_t		*,
    int	                ,
    dnsConf_t		);

dns_status_t
clerk_testgroup (
    FullName_t		*,
    FullName_t		*,
    char		*,
    ObjUID_t		*,
    Timeout_t		*);

dns_status_t
clerk_link_replica (
    unsigned char	*,
    FullName_t		*,
    ObjUID_t		*,
    Update_t		*,
    int			,
    ObjUID_t		*,
    VersionNumber_t     *);

dns_status_t
clerk_modify_attribute (
    FullName_t		*,
    int			,
    Update_t		*,
    int			);

dns_status_t
clerk_modify_replica (
    ObjUID_t		*,
    Update_t		*,
    int			,
    ReplicaPointer_t	*,
    int			);

dns_status_t
clerk_read_replica (
    ObjUID_t		*,
    FullName_t		*,
    AttributeName_t	*,
    byte_t		*,
    ReplicaPointer_t	*,
    int			,
    ReplicaPointer_t	*);

dns_status_t
clerk_do_updates (
    ObjUID_t		*,
    ReplicaPointer_t	*,
    int			,
    int			,
    byte_t		*);

dns_status_t
clerk_combine (
    ObjUID_t		*,
    Timestamp_t		*,
    ObjUID_t		*,
    Timestamp_t		*,
    FullName_t		*,
    ReplicaPointer_t	*,
    int			,
    ObjUID_t		*,
    ReplicaPointer_t	*);

dns_status_t
dns_adv_dump_ns (
    int			,
    nsgbl_ns_t		*);

int
sys_diag_ast_speed_selector (
    int			);

int
sys_diag_async_op (
    int			  , 
    int 		  ,
    char		 *, 
    int(*)(char 	 *, 
	   sys_mailbox 	 ),
    sys_mailbox		  );

#endif  /* #ifndef _CLERK_H */
