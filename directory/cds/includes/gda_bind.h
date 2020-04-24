/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gda_bind.h,v $
 * Revision 1.1.8.2  1996/02/18  23:34:00  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:46  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:16:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:14  root]
 * 
 * Revision 1.1.5.2  1994/06/09  18:39:12  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:50  devsrc]
 * 
 * Revision 1.1.5.1  1994/03/12  22:03:27  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:16:41  peckham]
 * 
 * Revision 1.1.3.3  1992/12/30  13:32:00  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:51:07  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  19:14:52  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:13:24  weisman]
 * 
 * Revision 1.1  1992/01/19  15:15:43  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _GDA_BIND_H
#define _GDA_BIND_H
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

typedef struct question_t {
	int		type;
	int		does_not_exist;
} question_t;

typedef struct nameserver_t {
	cds_FullName_t	mach_name;
} nameserver_t;

typedef struct gda_addr_t {
	struct sockaddr_in	mach_addr;
} gda_addr_t;

typedef struct addr_nameserver_t {
	struct sockaddr_in	mach_addr;
	int			prefix;
	int			auth;
	int			used;
	int			path;
	int			msgsize;
} addr_nameserver_t;

#define MAX_NUM_ADDR_CI	10

typedef struct cds_cell_info_txt_t {
	uuid_t			nsp_uuid;
	int			replica_type;
	cds_FullName_t		ch_name;
	uuid_t			ch_uuid;
	cds_FullName_t		mach_name;
	int			num_addr;
	struct sockaddr_in	mach_addr[MAX_NUM_ADDR_CI];
} cds_cell_info_txt_t;

enum rr_tag	{ e_header, e_question, e_nameserver, e_addr,
			e_addr_nameserver, e_cds_cell_info_txt };

typedef struct rr_el_t {
	cds_FullName_t		dname_rr;
	int			class_rr;
	int 			auth_rr;	/* 1 if this rr is
							authoritative */
	struct rr_el_t 		*next_rr;	/* next element on hash list*/

	enum rr_tag		tag_rr;
	union {
		HEADER			head;
		question_t		ques;
		nameserver_t		ns;
		gda_addr_t		ad;
		addr_nameserver_t	adns;
		cds_cell_info_txt_t	ci;
/*		afsdb_t			afs;*/
	} rr;
} rr_el_t;

typedef struct ns_asked_t {
	unsigned short	query_num;
	rr_el_t		*ad_ns;
			/* This record is an element of a list.  It is
				a pointer to the NS record */
} ns_asked_t;

#define NUM_BUCKETS		25
#define	MAX_NUM_Q_PER_QUES	20

typedef struct bind_t {
	pthread_cond_t		cond;
	pthread_mutex_t		mutex;

	list_t			answers;
	list_t			ad_ns;
	rr_el_t			*buckets[NUM_BUCKETS];
				/* these are all members of a list */

	list_t			*answer;
	rr_el_t			*ad_ns_answ;
				/* a pointer to the NS record corresponding
					to the server that just answered
					a question.  This element is a
					member of a list. */

	int			num_who;
	ns_asked_t		who[MAX_NUM_Q_PER_QUES];
} bind_t;
#endif  /* #ifndef _GDA_BIND_H */
