/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dfs_dce_acl.c,v $
 * Revision 1.1.14.4  1996/04/01  16:10:51  root
 * 	Fix OT 13400: Remove unnecessary #ifdef DACL_EPISODE so that it uses
 * 	the correct signature for api dacl_epi_CheckAccessAllowedPac().
 * 	[1996/04/01  16:10:19  root]
 *
 * Revision 1.1.14.3  1996/02/18  00:06:43  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:57:55  marty]
 * 
 * Revision 1.1.14.2  1996/02/07  17:48:16  parul
 * 	DCE 1.2.1 final drop from HP
 * 	[1996/02/07  16:14:02  parul]
 * 
 * 	DCE 1.2.1 final drop from HP
 * 
 * Revision 1.1.11.4  1994/09/22  20:49:45  rsarbo
 * 	Remove include of fshs_deleg.h
 * 	[1994/09/22  20:49:08  rsarbo]
 * 
 * Revision 1.1.11.3  1994/09/21  15:45:02  rsarbo
 * 	convert to use EPAC's
 * 	[1994/09/20  20:01:41  rsarbo]
 * 
 * Revision 1.1.11.2  1994/09/02  19:43:18  mckeen
 * 	Set DFS acl manager name to proper syntax
 * 	[1994/09/02  19:15:36  mckeen]
 * 
 * Revision 1.1.11.1  1994/02/04  20:38:13  devsrc
 * 	Moving submission over to dce1.0.3a
 * 	[1993/12/07  21:58:11  zeliff]
 * 
 * Revision 1.1.6.1  1993/12/07  21:39:58  mbs
 * 	Added a missing argument to afs_syscall.
 * 	[1993/12/07  21:38:51  mbs]
 * 
 * Revision 1.1.4.4  1993/01/18  18:33:38  burati
 * 	CR6548 Provide error translation routine dfs_dce_convert_errors
 * 	[1993/01/18  16:45:58  burati]
 * 
 * Revision 1.1.4.3  1992/12/29  12:36:27  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/28  20:32:27  zeliff]
 * 
 * Revision 1.1.4.2  1992/12/01  15:41:35  burati
 * 	Change includes from <afs/*> to <dcedfs/*>
 * 	[1992/11/30  19:50:15  burati]
 * 
 * Revision 1.1.2.4  1992/07/09  19:46:18  burati
 * 	CR4161 More DFS ACL fixes from Beth, to hide local vs remote paths.
 * 	[1992/07/08  19:46:58  burati]
 * 
 * 	CR4161 DFS ACL support work
 * 	[1992/06/18  21:53:32  burati]
 * 
 * Revision 1.1.2.3  1992/05/08  15:10:28  burati
 * 	Add dfs_dce_acl_get_mgr_types_semantics
 * 	[1992/04/29  03:08:27  burati]
 * 
 * Revision 1.1.2.2  1992/01/22  23:04:25  melman
 * 	Adding changes from defect_fixes archive.
 * 	[1992/01/22  22:20:57  melman]
 * 
 * $EndLog$
 */

/*  dfs_dce_acl.c
**
** Copyright (c) Hewlett-Packard Company 1991, 1992, 1993
** Unpublished work. All Rights Reserved.
**
*/
/*  DFS/DCE acl lookup/replace routines
**
**  The lookup and replace routines here provide the interface between
**  DFS acls and DCE sec_acls.  They have been implemented here to keep
**  the sec_acl client agent from having to have knowledge about non-DCE
**  ACL constructs.  The lookup routine is called by the sec_acl client
**  agent bind routine (sec_acl_bind), before any remote operations are
**  attempted, to determine if the specified object is governed by a DFS
**  ACL.  If that lookup returns sec_acl_object_not_found, then the client
**  agent will assume that the pathname refers to an object in another DCE
**  component, that exports a DCE ACL manager.  Otherwise, all subsequent
**  operations will use the dfs_{lookup,replace}_dce_acl calls.
**
** History:
**  10/17/91    burati  Fix contents of manager_info
**  10/06/91    burati  Applied diffs/fixes received from Transarc
**  09/10/91    ahop    Changed ifdef apollo to ifndef DCE_DFS_PRESENT
**  07/29/91    burati  Created.
*/

#include <stdio.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>

#include <sys/errno.h>
#include <dce/dce_cf.h>
#include <dce/id_base.h>
#include <dce/binding.h>
#include <dce/aclbase.h>
#include <dce/sec_login.h>

#ifdef DCE_DFS_PRESENT
# include <dcedfs/stds.h>
# include <dcedfs/syscall.h>
# include <dcedfs/ioctl.h>
# include <dcedfs/aclint.h>
# include <com_err.h>
# include <dcedfs/dacl.h>
#endif

#include <macros.h>
#include <dfs_dce_acl.h>

extern unsigned long dacl_debug_flag;

#ifdef DCE_DFS_PRESENT
extern long dacl_From_SecAcl
#ifndef __STDC__
();
#else /* __STDC__ */
(dacl_t * daclP, sec_acl_t * secAclP);
#endif /* __STDC__ */
#endif

#ifdef nullsyscall
int afs_syscall(sysswitch, opcode, filenameP, bufferP, buffersizeP, flags)
    int     sysswitch;
    int     opcode;
    char *  filenameP;
    char *  bufferP;
    int *   buffersizeP;
    long    flags;
{
  return 0;
}
#endif /* nullsyscall */
#if defined(use_syscall) && defined(AFS_HPUX_ENV)
extern int dfs_is_present();
#endif /* use_syscall && AFS_HPUX_ENV */

#define STARTING_BUFFER_SIZE	512

/*
 * Convert syscall error codes to DCE errors
 */
PRIVATE void dfs_dce_convert_errors(errcode, status)
    int errcode;
    error_status_t *status;
{
    switch(errcode) {
    case EINVAL:
        *status = sec_acl_invalid_dfs_acl;
        break;
    case EIO:
    case ERANGE:
    case EFAULT:
    case E2BIG:
    case ESPIPE:
	*status = sec_acl_no_acl_found;
        break;
    case ENOSYS:
        *status = sec_acl_object_not_found;
        break;
    case ENOMEM:
        *status = sec_acl_cant_allocate_memory;
        break;
    case EACCES:
        *status = sec_acl_not_authorized;
        break;
    default:
	*status = sec_acl_no_acl_found;
	break;
    }
}


/*
** Lookup DFS acl
*/
error_status_t dfs_dce_acl_lookup(filenameP, sec_acl_type, sec_aclP)
     char            *filenameP;
     sec_acl_type_t  sec_acl_type;
     sec_acl_t       *sec_aclP;
{
  int		errcode = 0;
  long		translationErrcode;
  int		buffersize = STARTING_BUFFER_SIZE;
  char *	bufferP = NULL;
  long		flags;
  boolean32	done = false;
  error_status_t status;
#if !defined(use_syscall)
  int           aclfd;
  struct stat   statBuf;
#endif /* !defined(use_syscall) */
  static char routineName[] = "dfs_lookup_dce_acl";

#ifndef DCE_DFS_PRESENT
return(sec_acl_not_implemented);  
#else
#if defined(use_syscall) && defined(AFS_HPUX_ENV)
  if (!dfs_is_present())
    return(sec_acl_not_implemented);
#endif /* use_syscall && AFS_HPUX_ENV */

  /* Translate sec_acl type to AFS acl type */
  switch (sec_acl_type) {
    case sec_acl_type_object:
      flags = VNX_ACL_REGULAR_ACL;
      break;
    case sec_acl_type_default_object:
      flags = VNX_ACL_INITIAL_ACL;
      break;
    case sec_acl_type_default_container:
      flags = VNX_ACL_DEFAULT_ACL;
      break;
    default:
      return(sec_acl_invalid_acl_type);
  }
  
  bufferP = (char *)malloc(buffersize);

  /* Lookup the AFS acl if possible */
  while (!done) {
#if defined(use_syscall)
    errcode = afs_syscall(AFSCALL_VNODE_OPS, VNX_OPCODE_GETACL,
			  filenameP, bufferP, &buffersize, flags);
#else /* defined(use_syscall) */
    /* this is inefficient, but it maintains the coding structure used with the syscall */
    aclfd = open(filenameP, O_RDONLY, 0);
    if (aclfd >= 0) {
      if (fstat(aclfd, &statBuf) == 0) {
        if (statBuf.st_size <= buffersize) {
          errcode = (read(aclfd, bufferP, statBuf.st_size) != statBuf.st_size);
          if (!errcode) {
            buffersize = statBuf.st_size;
            done = true;

            if (errcode = (close(aclfd) != 0)) {
              if (dacl_debug_flag) perror("close");
            }
          }
          else if (dacl_debug_flag) perror("read");
        }
      }
      else if (dacl_debug_flag) {
        perror("fstat");
        errcode = -1;
      }
    }
    else if (dacl_debug_flag) {
      perror("open");
      errcode = -1;
    }
#endif /* defined(use_syscall) */

    if (errcode) {
      if (errno == E2BIG) {
	free(bufferP);
	bufferP = (char *) malloc(2 * buffersize);
        buffersize = 2 * buffersize;
	/* sizeof(char) === 1 */
      } else {
        dfs_dce_convert_errors(errno, &status);
        free(bufferP);
        return(status);
      }
    } else {
      done = true;
    }
  }
  
  /* Convert the AFS acl into a DCE sec_acl, if requested */
  if (sec_aclP) {
    translationErrcode = sec_acl_ParseAcl(sec_aclP, bufferP, buffersize);
  }
  else {
    free(bufferP);
    return(error_status_ok);
  }
  
  if (bufferP) {
    free(bufferP);
  }
  
  if (translationErrcode == DACL_SUCCESS) {
    return(error_status_ok);
  }
  else {
    /* whatever processing is required to reflect the appropriate error
       code to caller */
    if (dacl_debug_flag) {
      fprintf(stderr, "%s: sec_acl_ParseAcl returned error: %s\n",
            routineName, error_message(translationErrcode));
    }
    return(1);
  }
#endif /* DCE_DFS_PRESENT */
}


/*
** Replace DFS acl
*/
error_status_t dfs_dce_acl_replace(filenameP, sec_acl_type, sec_aclP)
     char            *filenameP;
     sec_acl_type_t  sec_acl_type;
     sec_acl_t       *sec_aclP;
{
  long		translateErrcode = 0;
  int		errcode = 0;
  unsigned int	buffersize = 0;
  char *	bufferP = (char *)NULL;
  long		flags;
  error_status_t status;
#if !defined(use_syscall)
  int           aclfd;
#endif /* !defined(use_syscall) */
  static char routineName[] = "dfs_replace_dce_acl";
  
#ifndef DCE_DFS_PRESENT
return(sec_acl_not_implemented);  
#else
#if defined(use_syscall) && defined(AFS_HPUX_ENV)
  if (!dfs_is_present())
    return(sec_acl_not_implemented);
#endif /* use_syscall && AFS_HPUX_ENV */

  /* Translate sec_acl type to AFS acl type */
  switch (sec_acl_type) {
    case sec_acl_type_object:
      flags = VNX_ACL_REGULAR_ACL;
      break;
    case sec_acl_type_default_object:
      flags = VNX_ACL_INITIAL_ACL;
      break;
    case sec_acl_type_default_container:
      flags = VNX_ACL_DEFAULT_ACL;
      break;
    default:
      return(sec_acl_invalid_acl_type);
  }
  
  /* Convert the DCE sec_acl into an AFS acl */
  translateErrcode = sec_acl_FlattenAcl(sec_aclP, &bufferP, &buffersize);
  if (translateErrcode) {
    /* whatever processing is required to reflect the appropriate error
       code to caller */
    if (dacl_debug_flag) {
      fprintf(stderr, "%s: sec_acl_FlattenAcl returned error: %s\n",
            routineName, error_message(translateErrcode));
    }
    free(bufferP);
    return(1);
  }
  
#if defined(use_syscall)
  /* Replace the AFS acl if possible */
  errcode = afs_syscall(AFSCALL_VNODE_OPS, VNX_OPCODE_SETACL,
			filenameP, bufferP, buffersize, flags);
#else /* defined(use_syscall) */
    aclfd = open(filenameP, (O_WRONLY | O_CREAT | O_TRUNC), 0644);
    if (aclfd >= 0) {
      if (errcode = (write(aclfd, bufferP, buffersize) != buffersize)) {
        if (dacl_debug_flag) perror("write");
      }

      if (errcode = (close(aclfd) != 0)) {
        if (dacl_debug_flag) perror("close");
      }
    }
    else {
      if (dacl_debug_flag) perror("open");
      errcode = -1;
    }
#endif /* defined(use_syscall) */

  if (errcode && dacl_debug_flag) {
    perror("afs_syscall");
    fprintf(stderr, "%s: afs_syscall returned %d\n", routineName, errcode);
  }

  /* this is probably as good as anything else */
  if (bufferP) {
    free(bufferP);
  }
  
  /* Assume the only reason we couldn't replace the acl, is if
   * filenameP doesn't point to an AFS object.
   */
  if (errcode) {
    dfs_dce_convert_errors(errno, &status);
    return(status);
  }

  return(error_status_ok);
#endif /* DCE_DFS_PRESENT */
}


/*
 * Determine types of acls protecting afs objects.  This is here to provide
 * some degree of compatibility with the DCE acl interfaces, so that DCE ACL
 * clients may manipulate AFS acls in a manner similar to that of DCE ACLs.
 */
void dfs_dce_acl_get_manager_types
#ifndef __STDC__
    (sec_acl_type, size_avail, size_used, num_types, manager_types, status)
    sec_acl_type_t          sec_acl_type;
    unsigned32              size_avail;
    unsigned32              *size_used;
    unsigned32              *num_types;
    uuid_t                  manager_types[];
    error_status_t          *status;
#else
  (
    sec_acl_type_t          sec_acl_type,
    unsigned32              size_avail,
    unsigned32              *size_used,
    unsigned32              *num_types,
    uuid_t                  manager_types[],
    error_status_t          *status
  )
#endif
{
#ifndef DCE_DFS_PRESENT
    *status = sec_acl_not_implemented;
#else
    *status = error_status_ok;
    *num_types = 1;

#if defined(use_syscall) && defined(AFS_HPUX_ENV)
    if (!dfs_is_present()) {
        *status = sec_acl_not_implemented;
	return;
    }
#endif /* use_syscall && AFS_HPUX_ENV */
    if (size_avail < 1) {
        *size_used = 0;
    } else {
        *size_used = 1;
        /* couldn't find a DCE uuid_copy routine of any form */
        bcopy((char *)&episodeAclMgmtUuid, (char *)&(manager_types[0]), sizeof(uuid_t));
    }
#endif /* DCE_DFS_PRESENT */
}


/*
 * Determine types of acls protecting afs objects.  This is here to provide
 * some degree of compatibility with the DCE acl interfaces, so that DCE ACL
 * clients may manipulate AFS acls in a manner similar to that of DCE ACLs.
 * Let the caller know we want POSIX mask_obj semantics
 */
void dfs_dce_acl_get_mgr_types_semantics
#ifndef __STDC__
  (sec_acl_type, size_avail, size_used, num_types, manager_types,
                                               posix_semantics, status)
    sec_acl_type_t              sec_acl_type;
    unsigned32                  size_avail;
    unsigned32                  *size_used;
    unsigned32                  *num_types;
    uuid_t                      manager_types[];
    sec_acl_posix_semantics_t   posix_semantics[];
    error_status_t              *status;
#else
  (
    sec_acl_type_t              sec_acl_type,
    unsigned32                  size_avail,
    unsigned32                  *size_used,
    unsigned32                  *num_types,
    uuid_t                      manager_types[],
    sec_acl_posix_semantics_t   posix_semantics[],
    error_status_t              *status
  )
#endif
{
#ifndef DCE_DFS_PRESENT
    *status = sec_acl_not_implemented;
#else
    *status = error_status_ok;
    *num_types = 1;

#if defined(use_syscall) && defined(AFS_HPUX_ENV)
    if (!dfs_is_present()) {
        *status = sec_acl_not_implemented;
	return;
    }
#endif /* use_syscall && AFS_HPUX_ENV */
    if (size_avail < 1) {
        *size_used = 0;
    } else {
        *size_used = 1;
        /* couldn't find a DCE uuid_copy routine of any form */
        bcopy((char *)&episodeAclMgmtUuid, (char *)&(manager_types[0]),
                sizeof(uuid_t));

        /* Only have 1 mgr type and it always wants POSIX mask_obj semantics */
        posix_semantics[0] = sec_acl_posix_mask_obj;
    }
#endif /* DCE_DFS_PRESENT */
}


/*
 * Determine printstrings of acls protecting afs objects.  This is here to
 * provide some degree of compatibility with the DCE acl interfaces, so that DCE
 * ACL clients may manipulate AFS acls in a manner similar to that of DCE ACLs.
 */
#define NUM_PSTRS 6

static sec_acl_printstring_t hardcoded_printstrings[] = {
    { "r",  "read",     sec_acl_perm_read       },
    { "w",  "write",    sec_acl_perm_write      },
    { "x",  "execute",  sec_acl_perm_execute    },
    { "c",  "control",  sec_acl_perm_owner      },
    { "i",  "insert",   sec_acl_perm_insert     },
    { "d",  "delete",   sec_acl_perm_delete     }
};

static sec_acl_printstring_t hardcoded_manager_info = {
    "dfs", "Distributed File Service server",
    (sec_acl_perm_read | sec_acl_perm_write | sec_acl_perm_execute |
     sec_acl_perm_owner | sec_acl_perm_insert | sec_acl_perm_delete)
};

void dfs_dce_acl_get_printstring
#ifndef __STDC__
    (manager_type, size_avail, manager_type_chain, manager_info, tokenize,
                    total_num_printstrings, size_used, printstrings, status)
    uuid_t                  *manager_type;
    unsigned32              size_avail;
    uuid_t                  *manager_type_chain;
    sec_acl_printstring_t   *manager_info;
    boolean32               *tokenize;
    unsigned32              *total_num_printstrings;
    unsigned32              *size_used;
    sec_acl_printstring_t   printstrings[];
    error_status_t          *status;
#else
  (
    uuid_t                  *manager_type,   
    unsigned32              size_avail,
    uuid_t                  *manager_type_chain,
    sec_acl_printstring_t   *manager_info,
    boolean32               *tokenize,
    unsigned32              *total_num_printstrings,
    unsigned32              *size_used,
    sec_acl_printstring_t   printstrings[],
    error_status_t          *status
  )
#endif
{
#ifndef DCE_DFS_PRESENT
    *status = sec_acl_not_implemented;
#else
    /* COULD CHECK MANAGER TYPE HERE, ONCE THE DUMMY MGR TYPE CREATE ABOVE
     * IS REPLACED WITH A REAL MANAGER TYPE UUID. - done - bab
     */
    int             i;
    boolean32       uuidCompare;

#if defined(use_syscall) && defined(AFS_HPUX_ENV)
    if (!dfs_is_present()) {
        *status = sec_acl_not_implemented;
	return;
    }
#endif /* use_syscall && AFS_HPUX_ENV */
    uuidCompare = uuid_equal(manager_type, (uuid_t *)&episodeAclMgmtUuid, status);
    if ((uuidCompare == true) && (*status == error_status_ok)) {
        *status = error_status_ok;
        *total_num_printstrings = NUM_PSTRS;
        *size_used = (size_avail < NUM_PSTRS) ? size_avail : NUM_PSTRS;
        *manager_info = hardcoded_manager_info;
        *tokenize = false;

        /* Only 1 set of printstrings */
        uuid_create_nil(manager_type_chain, status);

        for (i = 0; i < *size_used; i++)
            printstrings[i] = hardcoded_printstrings[i];
    } else {
        if (*status == error_status_ok) {
            *status = sec_acl_invalid_manager_type;
        }
        /* if the uuid_equal call returned an error, pass that error back to the caller */
    }
#endif /* DCE_DFS_PRESENT */
}


#ifdef DCE_DFS_PRESENT

PRIVATE void dfs_dce_acl_MakeUpLocalPacList
#ifndef __STDC__
  (pacListPP, statusP)
pac_list_t **		pacListPP;
error_status_t *	statusP;
#else /* __STDC__ */
  (
   pac_list_t **	pacListPP,
   error_status_t *	statusP
  )
#endif /* __STDC__ */
{
  uid_t				localUid;
  gid_t				localGid;
  int				gidsetlen;
  gid_t				gidset[NGROUPS];
  int				i;
  uuid_t			realmUuid;
  sec_rgy_name_t		theGlobalName;
  sec_rgy_bind_auth_info_t	bindingAuthInfo;
  sec_rgy_handle_t		secdHandle;
  sec_id_pa_t			*pa_p;
  
  CLEAR_STATUS(statusP);

  localUid = getuid();
  localGid = getgid();

  
  *pacListPP = (pac_list_t *) malloc(sizeof(pac_list_t));
  memset(*pacListPP, 0, sizeof(pac_list_t));
  (*pacListPP)->pacp = (sec_id_pa_t *) malloc(sizeof(sec_id_pa_t));
  memset((*pacListPP)->pacp, 0, sizeof(sec_id_pa_t));

  (*pacListPP)->authenticated = true;	/* trust a pac generated this way */
  pa_p = (*pacListPP)->pacp;

  /*
   * We have to get the local default realm; assume that it's the local 
   * realm for the security server.
   */
  bindingAuthInfo.info_type = sec_rgy_bind_auth_none;
  (void)strcpy(theGlobalName, CELL_DIR_ROOT);
  sec_rgy_cell_bind(theGlobalName,
		    &bindingAuthInfo,
		    &secdHandle,
		    statusP);
  if (GOOD_STATUS(statusP)) {
    /* get the local realm uuid from the security server */
    (void)sprintf(theGlobalName, "%s%s", CELL_DIR_ROOT, "root");
    sec_id_parse_name(secdHandle,
		      theGlobalName,
		      NULL,
		      &realmUuid,
		      NULL,
		      (uuid_t *)NULL,
		      statusP);
    if (GOOD_STATUS(statusP)) {
      bcopy((char *)&realmUuid, (char *)&(pa_p->realm), sizeof(uuid_t));
      bcopy((char *)&localUid,(char *)&(pa_p->principal.uuid),sizeof(localUid));
      bcopy((char *)&localGid, (char *)&(pa_p->group.uuid), sizeof(localGid));
      
      pa_p->num_groups = getgroups(&gidsetlen, gidset);
      pa_p->groups = (sec_id_t *)malloc(pa_p->num_groups * sizeof(sec_id_t));
      memset((char *)pa_p->groups, 0, pa_p->num_groups * sizeof(sec_id_t));
      for (i = 0; i < pa_p->num_groups; i++) {
	bcopy((char *)&(gidset[i]), (char *)&(pa_p->groups[i].uuid), 
						sizeof(gid_t));
      }
    }
  }
  if (BAD_STATUS(statusP)) {
	dfs_dce_acl_FreePacList(pacListPP);
  }
}

void dfs_dce_acl_FreePacList(pac_list_t **pacListPP)
{
  pac_list_t *next, *t;

  next = *pacListPP;
  while (next) {
     t = next->next;
     free(next);
     next = t;
  }
}
#endif /* DCE_DFS_PRESENT */


boolean32 dfs_dce_acl_CheckAccessAllowedPac
#ifndef __STDC__
  (entryNameP, secAclP, desiredPermsetP, accessAllowedP, statusP)
char *			entryNameP;
sec_acl_t *		secAclP;
sec_acl_permset_t *	desiredPermsetP;
sec_acl_permset_t *	accessAllowedP;
error_status_t *	statusP;
#else
  (
   char *		entryNameP,
   sec_acl_t *		secAclP,
   sec_acl_permset_t *	desiredPermsetP,
   sec_acl_permset_t *	accessAllowedP,
   error_status_t *	statusP
  )
#endif
{
#ifndef DCE_DFS_PRESENT
  *statusP = sec_acl_not_implemented;
  return false;
#else
  boolean32		rtnVal = false;
  unsigned long		errcode;
  dacl_t		dacl;
  pac_list_t		*pacListP = NULL;
  sec_id_pac_t *	pacP = (sec_id_pac_t *)NULL;
  sec_login_handle_t	loginContext;
  sec_login_net_info_t	networkContext;
  sec_acl_permset_t	localDesiredPermset = 0;
  epi_principal_id_t	userObj;
  epi_principal_id_t	groupObj;
  struct stat		statBuf;
  u_int32		modeBits;
  int			localPath = 0;	/* assume remote */
  int			syserror;
  char			cellName[BUFSIZ];
  struct afs_ioctl	ignoredPioctlInfo;
  static char		routineName[] = "dfs_dce_acl_CheckAccessAllowedPac";
  
#if defined(use_syscall) && defined(AFS_HPUX_ENV)
  if (!dfs_is_present()) {
    *statusP = sec_acl_not_implemented;
    return false;
  }
#endif /* use_syscall && AFS_HPUX_ENV */
  CLEAR_STATUS(statusP);

  /* we now do the localPath computation here */
  /*
   *  This is not ideal, but it should work for the short term, until
   * we get a real access syscall implemented.
   */
  ignoredPioctlInfo.in = ignoredPioctlInfo.out = (caddr_t)NULL;
  ignoredPioctlInfo.out = cellName;
  ignoredPioctlInfo.in_size = 0;
  ignoredPioctlInfo.out_size = BUFSIZ;
  syserror = afs_syscall(AFSCALL_PIOCTL, entryNameP, VIOC_FILE_CELL_NAME,
			 &ignoredPioctlInfo, 0, 0);
  if ((syserror != 0) && (errno != 0)) {
    localPath = 1;
  }

  if (localPath) {
    dfs_dce_acl_MakeUpLocalPacList(&pacListP, statusP);
    if (BAD_STATUS(statusP)) {
      pacListP = NULL;
    }
  }
  else {
    sec_login_get_current_context(&loginContext, statusP);
    
    /* we'll accept an uncertified context for this; it's not critical here */
    if (STATUS_EQUAL(statusP, sec_login_s_not_certified)) {
      CLEAR_STATUS(statusP);
    }
    
    if (GOOD_STATUS(statusP)) {
      sec_cred_pa_handle_t pa_h;
      pac_list_t *next;
      sec_cred_cursor_t cursor;

      pacListP = malloc(sizeof(pac_list_t));
      memset(pacListP, 0, sizeof(pac_list_t));

      pa_h = sec_login_cred_get_initiator(loginContext, statusP);
      next = pacListP;

      sec_login_cred_init_cursor(&cursor, statusP);
      while (1) {
         next->pacp = sec_cred_get_pa_data(pa_h, statusP);
         if (BAD_STATUS(statusP)){
             break;
         }
         next->authenticated = !sec_id_is_anonymous_pa(next->pacp, statusP);
         if (BAD_STATUS(statusP)){
             break;
         }
         pa_h = sec_login_cred_get_delegate(loginContext, &cursor, statusP);
         if (STATUS_EQUAL(statusP, sec_cred_s_no_more_entries)) {
             CLEAR_STATUS(statusP);
             break;
         }
         if (BAD_STATUS(statusP)){
             break;
         }
	 next->next = malloc(sizeof(pac_list_t));
	 next = next->next;
	 next->next = (pac_list_t *)NULL;
      }
      sec_cred_free_cursor(&cursor, statusP);
    }
  
    if (GOOD_STATUS(statusP)) {
      /* translate the acl back into a dacl - yuck! */
      errcode = dacl_From_SecAcl(&dacl, secAclP);
      if (!errcode) {
        /* do the dacl test */
        /* we get stat the file to get the user obj & group obj values */
        if (!stat(entryNameP, &statBuf)) {
	  bzero((char *)&userObj, sizeof(epi_principal_id_t));
	  bcopy((char *)&(statBuf.st_uid), (char *)&userObj, sizeof(statBuf.st_uid));
	
	  bzero((char *)&groupObj, sizeof(epi_principal_id_t));
	  bcopy((char *)&(statBuf.st_gid), (char *)&groupObj, sizeof(statBuf.st_gid));
	
	  /*
	   * We get the mode bits out of the ACL, since the ACL will provide the mode bits
	   * for the file on write-back, anyway; and this method will also probably
	   * be less confusing for the user.
	   */
	  errcode = dacl_ExtractPermBits(&dacl, &modeBits);
  	
	  if (!errcode) {
	    /* the permsets are isomorphic, so, we can just do the cast */
	    errcode = dacl_epi_CheckAccessAllowedPac(&dacl,
					       &modeBits,
					       ((dacl_permset_t *)
						(desiredPermsetP ?
						 desiredPermsetP :
						 &localDesiredPermset)),
					       pacListP,
					       &userObj,
					       &groupObj,
					       0,
					       (dacl_permset_t *)accessAllowedP);

	    if (!errcode) {
	      rtnVal = true;
	    }
	    else if ((errcode != DACL_ERROR_ACCESS_DENIED) &&
		   (errcode != DACL_ERROR_ACCESS_EXPLICITLY_DENIED)) {
	      if (dacl_debug_flag) {
	        fprintf(stderr, "%s: dacl_epi_CheckAccessAllowedPac returned %ld\n",
		      routineName, errcode);
	      }
	    
	      /* we really need a better error code for things like this */
	      SET_STATUS(statusP, sec_acl_object_not_found);
	    }
	  }
	  else {
	    if (dacl_debug_flag) {
	      fprintf(stderr, "%s: dacl_ExtractPermBits returned %ld\n",
		      routineName, errcode);
	    }	
	    
	    /* we really need a better error code for things like this */
	    SET_STATUS(statusP, sec_acl_object_not_found);
	  }
        }
        else {
	  /* stat failed */
	  perror("Stat of DFS file");
	
	  /* this is probably appropriate in this case */
	  SET_STATUS(statusP, sec_acl_object_not_found);
        }
      
        errcode = dacl_FreeAclEntries(&dacl);
        if (errcode && dacl_debug_flag) {
	  fprintf(stderr, "%s: dacl_FreeAclEntries returned %ld\n", routineName, errcode);
	  /* don't do anything to the status in this case */
        }
      }
      else {
        if (dacl_debug_flag) {
	  fprintf(stderr, "%s: dacl_From_SecAcl returned %ld\n", routineName, errcode);
        }
        
        /* we really need a better error code for things like this */
        SET_STATUS(statusP, sec_acl_object_not_found);
      }
    }
  }
  if ( pacListP != NULL )
    dfs_dce_acl_FreePacList( &pacListP );

  return rtnVal;
#endif /* DCE_DFS_PRESENT */
}

