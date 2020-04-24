/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dns_malloc.h,v $
 * Revision 1.1.6.2  1996/02/18  23:33:44  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:19  marty]
 *
 * Revision 1.1.6.1  1995/12/08  15:15:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:42:53  root]
 * 
 * Revision 1.1.4.4  1994/09/22  18:35:24  mccann
 * 	move defines to cds.mk
 * 	[1994/09/22  18:33:32  mccann]
 * 
 * Revision 1.1.4.3  1994/08/03  19:02:14  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:57:42  mccann]
 * 
 * Revision 1.1.4.2  1994/06/09  18:38:55  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:11:28  devsrc]
 * 
 * Revision 1.1.4.1  1994/03/12  22:02:46  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:13:49  peckham]
 * 
 * Revision 1.1.2.2  1992/12/30  13:29:36  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:47:37  zeliff]
 * 
 * Revision 1.1  1992/01/19  15:16:30  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#ifndef _DNS_MALLOC_H
#define _DNS_MALLOC_H
/*
 * Module dns_malloc.h
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
 * Module Description
 *
 * These routines allow installation of paranoid memory management
 *
 */
#include <dce/dns_config.h>
#include <dce/dns_record.h>
#include <pthread.h>

/*
 * Typed memory support
 *
 * These types are only used for instrumented versions, but we'll define
 * them all the time.
 */

typedef enum {
    mem_permanent,
    mem_temporary,
    mem_unknown,
    mem_ignored
} mem_type_t;



/*
 *  Here we tailor the memory allocation algorithms to compile-time constants.
 *
 *  DNS_MALLOC will cause compiler to include and use DNS internal
 *  jacket routines for malloc and free which have instrumentation
 *  for debugging and diagnosis.
 *
 *  If neither of these compilation flags are set, then we reduce all
 *  dns_malloc* and dns_free calls down to the lowest level malloc and free
 *  calls we can.
 */

#ifdef DNS_MALLOC  

   void *
   dns_malloc_instr (
      int		,
      mem_type_t 	,
      int		,
      char	        *);

   void
   dns_free (
       void *);

#  define dns_malloc(length) \
      dns_malloc_instr((length),mem_unknown,__LINE__,__FILE__)

#  define dns_malloc_perm(length) \
      dns_malloc_instr((length),mem_permanent,__LINE__,__FILE__)

#  define dns_malloc_temp(length) \
      dns_malloc_instr((length),mem_temporary,__LINE__,__FILE__)



#else 


  /* The VMS Kernel has its own dns_malloc and dns_free functions */

#  ifdef DNS_VMS_KERNEL

#    define dns_malloc_perm(length)  dns_malloc(length) 
#    define dns_malloc_temp(length)  dns_malloc(length)

#  else  /* DNS_VMS_KERNEL */

#    define dns_malloc(length)       malloc(length)
#    define dns_malloc_perm(length)  malloc(length) 
#    define dns_malloc_temp(length)  malloc(length)
#    define dns_free(adr_p)          free(adr_p)

#  endif   /* DNS_VMS_KERNEL */


#endif



/*
 * Support for buffer farm - pools of common buffers
 */

typedef enum {
    update_buffer_bt,
    dthread_mutex_bt,
    dthread_cv_bt,
    clerk_comm_bt,
    clerk_thread_bt,
    ta_random_bt
} dns_buffer_type_t;



/*
 * Memory diagnostic routines
 *
 * These must always be available because they are called by diagnostic
 * interfaces.   The dnsmalloc.c module compiles them with behavior
 * suitable to the lack of instrumentation available.
 */

void
dns_diag_vm_totprint (
    void);

void
dns_diag_vm_tot (
    int 	(*)(char *,void *),
    void 	*);

void
dns_diag_vm_findprint (
    int );

void
dns_diag_vm_find (
    int      ,
    int      (*)(char *,void *),
    void *);

void
dns_diag_vm_dump (
    mem_type_t );
    
void
dns_diag_vm_lastmallocs (
    void);

void
dns_diag_vm_dumpall (
    void);

int
dns_diag_set_memory_check (
    int );

int
dns_diag_set_abort_on_full_mem (
    int );

int
dns_diag_set_superchunk_mode (
    int );


void *
dns_get_buffer (
    dns_buffer_type_t ,
    int               );

void
dns_free_buffer (
    void *);

#endif  /* #ifndef _DNS_MALLOC_H */
