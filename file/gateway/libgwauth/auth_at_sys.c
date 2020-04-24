/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/* Copyright (C) 1996, 1995 Transarc Corporation - All rights reserved */
/*
 * HISTORY
 * $Log: auth_at_sys.c,v $
 * Revision 1.1.6.1  1996/10/02  17:50:31  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:40  damon]
 *
 * Revision /main/HPDCE02/2  1995/08/12  22:56 UTC  kissel
 * 	Copy in Transarc code (all trivial merges).
 * 
 * Revision /main/kissel_tarc_merge01/1  1995/05/15  22:29 UTC  kissel
 * 	Fix Compiler warnings.
 * 
 * Revision /main/HPDCE02/1  1995/06/22  20:14 UTC  maunsell_c
 * 	mothra C&S fixes
 * 
 * Revision /main/brezak_dfsgw/3  1995/06/09  03:09 UTC  brezak
 * 	Fixes from Transarc
 * 
 * Revision /main/brezak_dfsgw/2  1995/05/26  17:26 UTC  brezak
 * 	Add icl trace for dfsgw.
 * 
 * Revision /main/brezak_mothra10/1  1995/05/18  16:23 UTC  brezak
 * 	Hide u. refs with appropriate osi functions.
 * 
 * Revision 1.1.2.1  1994/07/26  19:13:37  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:58:07  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:07:25  mckeen]
 * 
 * $EndLog$
 */
/*
 * auth_at_sys.c - System call interface to authentication table (AT).
 */

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/errno.h>
#include <dcedfs/osi.h>
#include <dcedfs/auth_at.h>
#include <dcedfs/syscall.h>
#include <dcedfs/lock.h>
#include <dcedfs/icl.h>

RCSID("$Header: /u0/rcs_trees/dce/rcs/file/gateway/libgwauth/auth_at_sys.c,v 1.1.6.1 1996/10/02 17:50:31 damon Exp $")

extern struct lock_data atlock;
struct icl_set *dfsgw_iclSetp;

int at_afscall_add(), at_afscall_delete(), at_afscall_query();
int at_afscall_getsize(), at_afscall_list(), at_afscall_obsolete();

/*
 * Local AT vector table.
 */
int (*atsystbl[AT_NSYSCALLS])() = {
        at_afscall_obsolete,
        at_afscall_delete,
        at_afscall_query,
        at_afscall_list,
        at_afscall_getsize,
        at_afscall_add
};

/*
 * afscall_at - Main entry point for AT syscalls.
 *
 * This function vectors individual syscalls off to particular routines
 * for processing.  Each servicing routine returns 0 on success, and an
 * error code otherwise.  This is reflected back to the main afscall entry
 * point for processing.
 */
int
afscall_at(long code, long arg0, long arg1, long arg2, long arg3, 
	   int *ignored_ret)
{
  if ((code < 0) || (code >= AT_NSYSCALLS)) {
    return(EINVAL);
  }
  return((*atsystbl[code])(arg0, arg1, arg2, arg3));
}

/*
 * at_configure - Config AT database.
 *
 * This routine is called when the first AT syscall is made.  It's
 * our chance to initialize the module.
 */
void
at_configure()
{
  struct icl_log *logp;
  int code;

  lock_Init(&atlock);  
  /*
   * Create ICL log for tracing dfsgw ops
   */
  code = icl_CreateLog("dfsgw", 60*1024, &logp);
  if (code == 0) {
    code = icl_CreateSet("dfsgw", logp, (struct icl_log *)0, &dfsgw_iclSetp);
    if (code) {
      printf("Failed to create ICL set for dfsgw\n");
      dfsgw_iclSetp = NULL;
    }
  }

  afs_set_syscall(AFSCALL_AT, afscall_at);
  osi_nfs_gateway_loaded = 1;
  osi_nfs_gateway_gc_p = at_gc;
}

int 
at_afscall_obsolete()
{
    return EINVAL;
}

/*
 * AFSCALL_AT_ADD syscall.
 */
int
at_afscall_add(at_pag_mapping_p mapping, int subcmd)
{
  at_pag_mapping_t m;             /* Local copy */
  int error;

  if (error = osi_copyin((char *)mapping, (char *)&m, 
			sizeof(at_pag_mapping_t))) {
    return error;
  }
  
  if (!osi_suser(osi_getucred()) && osi_GetUID(osi_getucred()) != m.uid) 
    return(EPERM);

  return(at_add(&m, subcmd));
}


/*
 * AFSCALL_AT_DELETE syscall.
 */
int
at_afscall_delete(at_pag_mapping_p mapping)
{
  at_pag_mapping_t m;             /* Local copy */
  int error;

  if (error = osi_copyin((char *)mapping, (char *)&m,
			 sizeof(at_pag_mapping_t))) 
    return(error);

  if (!osi_suser(osi_getucred()) && osi_GetUID(osi_getucred()) != m.uid) 
    return(EPERM);

  return(at_delete(&m));
}

/*
 * AFSCALL_AT_QUERY syscall.
 */
int
at_afscall_query(at_pag_mapping_p mapping)
{
  at_pag_mapping_t m;             /* Local copy */
  int error;

  if (error = osi_copyin((char *)mapping, (char *)&m, 
			 sizeof(at_pag_mapping_t))) 
    return(error);

  /*
   * It does not make sense for dfsgw_list to not require any privileges
   * and for dfsgw_query to do so. Hence disable any privilege checking
   * in dfsgw_query code 
   */

#if 0
        if (!osi_suser(osi_getucred()) && osi_GetUID(osi_getucred()) != m.uid) 
	    return(EPERM);
#endif
  if (!(error = at_query(&m))) 
    if (error = osi_copyout((char *)&m, (char *)mapping, 
			    sizeof(at_pag_mapping_t))) 
      return(error);
  return(error);
}

/*
 * AFSCALL_AT_GETSIZE syscall.
 */
int
at_afscall_getsize(int *nmappings)
{
  int x;
  int error;

  x = at_size();
  if (error = osi_copyout((char *)&x, (char *)nmappings, sizeof(int))) {
    return(error);
  }
  return(0);
}

/*
 * AFSCALL_AT_LIST syscall.
 *
 * This syscall is unlike all the others in such that we don't have
 * to check the addresses passed in from user space.  This is all done
 * inside at_list.  This breaks the general model but it beats copying
 * things around twice.
 */
int
at_afscall_list(int *nmappings, at_pag_mapping_p mappings)
{
  return(at_list(nmappings, mappings));
}
