/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gda_debug.h,v $
 * Revision 1.1.8.2  1996/02/18  23:34:02  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:49  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:16:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:17  root]
 * 
 * Revision 1.1.5.3  1994/06/23  18:30:35  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:18:34  mccann]
 * 
 * Revision 1.1.5.2  1994/06/09  18:39:14  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:52  devsrc]
 * 
 * Revision 1.1.5.1  1994/03/12  22:03:31  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:16:55  peckham]
 * 
 * Revision 1.1.3.3  1992/12/30  13:32:06  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:51:19  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  19:15:02  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:13:34  weisman]
 * 
 * Revision 1.1  1992/01/19  15:16:40  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _GDA_DEBUG_H
#define _GDA_DEBUG_H
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
#ifndef DCE_CDS_DEBUG

#define ALLOC(X,Y)
#define FREE(X,Y)

#define ALLOC_COMMA(X,Y)
#define FREE_COMMA(X,Y)

#else /* DCE_CDS_DEBUG */

#define COUNTER_MAX 64

typedef struct counter {
	pthread_mutex_t	mutex;
	long allocd[COUNTER_MAX];
	long freed[COUNTER_MAX];
	long total;
} counter;

struct memory_stat {
	counter rr_el_t;
	counter list_t;
	counter sendbuf;
	counter replicas_q;
	counter realloc_replicas_q;
	counter ss_cds_ReplicaPointer_t;
	counter debug_el_t;
	counter element_t;
	counter ss_cds_Set_t;
	counter question_el_t;
	counter cds_FullName_t;
};

extern struct memory_stat	mem_stat;

#define	ALLOC(X,Y) { \
		if(pthread_mutex_lock(&mem_stat.X.mutex)) \
		{ \
		      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_MSG, (long)errno);	\
		      error_exit(-1);	\
		} \
		mem_stat.X.allocd[0]++; \
		mem_stat.X.allocd[Y]++; \
		mem_stat.X.total++; \
		if(pthread_mutex_unlock(&mem_stat.X.mutex)) \
		{ \
		      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_MSG, (long)errno);\
		      error_exit(-1);	\
		} \
	}

#define	FREE(X,Y) { \
		if(pthread_mutex_lock(&mem_stat.X.mutex)) \
		{ \
		      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_MSG, (long)errno); \
		      error_exit(-1);	\
		} \
		mem_stat.X.freed[0]++; \
		mem_stat.X.freed[Y]++; \
		mem_stat.X.total--; \
		if(pthread_mutex_unlock(&mem_stat.X.mutex)) \
		{ \
		      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_MSG, (long)errno); \
		      error_exit(-1);	\
		} \
	}

#define	ALLOC_COMMA(X,Y) \
		if(pthread_mutex_lock(&mem_stat.X.mutex)) \
		{ \
		      dce_svc_printf(CDS_S_PTHREADMUTEXLOCK_MSG, (long)errno); \
		      error_exit(-1);	\
		}, \
		mem_stat.X.allocd[0]++, \
		mem_stat.X.allocd[Y]++, \
		mem_stat.X.total++, \
		if(pthread_mutex_unlock(&mem_stat.X.mutex)) \
		{ \
		      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_MSG, (long)errno);\
		      error_exit(-1); 	\
		}


#define	FREE_COMMA(X,Y) \
	if(pthread_mutex_lock(&mem_stat.X.mutex)) \
	{ \
		      dce_svc_printf(CDS_S_PTHREADMUTEXLOCKL_MSG, (long)errno); \
	      error_exit(-1);	\
	}, \
	mem_stat.X.freed[0]++, \
	mem_stat.X.freed[Y]++, \
	mem_stat.X.total--, \
	if(pthread_mutex_unlock(&mem_stat.X.mutex)) \
	{ \
	      dce_svc_printf(CDS_S_PTHREADMUTEXUNLOCK_MSG, (long)errno);\
	      error_exit(-1);	\
	}

#endif /* DCE_CDS_DEBUG */

/**
 ** FUNCTION PROTOTYPES.... 
 **/

void
init_memstat (
	      void);

#endif  /* #ifndef _GDA_DEBUG_H */








