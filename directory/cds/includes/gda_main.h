/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gda_main.h,v $
 * Revision 1.1.6.2  1996/02/18  23:34:04  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:52  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:16:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:20  root]
 * 
 * Revision 1.1.4.3  1994/07/25  15:14:07  proulx
 * 	Use common ACL manager for subcomponent mgmt ACLs
 * 	[1994/07/25  13:59:47  proulx]
 * 
 * Revision 1.1.4.2  1994/06/09  18:39:15  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:54  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:03:35  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:17:06  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:32:15  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:51:30  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:15:46  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _GDA_MAIN_H
#define _GDA_MAIN_H
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
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
 */

/* gda_main.h: contains of the external definitions of the gda_main module */

extern long cds_createsoftlink_th_num;
extern long cds_createobject_th_num;
extern long cds_deleteobject_th_num;
extern long cds_deletesoftlink_th_num;
extern long cds_deletedirectory_th_num;
extern long cds_enumerateattributes_th_num;
extern long cds_enumeratechildren_th_num;
extern long cds_enumerateobjects_th_num;
extern long cds_enumeratesoftlinks_th_num;
extern long cds_modifyattribute_th_num;
extern long cds_readattribute_th_num;
extern long cds_resolvename_th_num;
extern long cds_skulk_th_num;
extern long cds_testattribute_th_num;
extern long cds_testgroup_th_num;
extern long cds_allowclearinghouses_th_num;
extern long cds_disallowclearinghouses_th_num;
extern long recv_bind_th_num;
extern long rpc_listener_th_num;
extern long memstat_th_num;
extern long cds_doupdate_th_num;
extern long cds_combine_th_num;
extern long cds_linkreplica_th_num;
extern long cds_modifyreplica_th_num;
extern long cds_addreplica_th_num;
extern long cds_removereplica_th_num;
extern long cds_newepoch_th_num;
extern long cds_deletechild_th_num;
extern long cds_createchild_th_num;
extern long cds_createdirectory_th_num;

extern pthread_key_t	thread_num;
extern list_t		question_list;
extern long		process_th_num;
extern conduit_el_t	bind_conduit;
extern int		debug;
extern char		*named_servers;
extern rpc_binding_vector_p_t gdad_bind_vector;              

/**
 ** FUNCTION PROTOTYPES.... 
 **/
int		
choose_conduit (
    question_el_t      *);

void	
error_exit (
	    int);

void	
error_exit_th (
	       int);

extern uuid_t		ch_id;
#endif  /* #ifndef _GDA_MAIN_H */
