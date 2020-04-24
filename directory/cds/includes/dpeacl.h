#ifndef _DPEACL_H
#define _DPEACL_H
/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 *(c) Copyright 1991, 1992 Siemens-Nixdorf Information Systems, Burlington, MA, USA
 * All Rights Reserved
*/
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
/*
 * HISTORY
 * $Log: dpeacl.h,v $
 * Revision 1.1.8.2  1996/02/18  23:33:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:22:33  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:16:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:43:06  root]
 * 
 * Revision 1.1.5.4  1994/08/25  19:30:42  proulx
 * 	Delegation support & cleanup.
 * 	[1994/08/25  19:13:53  proulx]
 * 
 * Revision 1.1.5.2  1994/05/20  22:03:18  sommerfeld
 * 	Include <dce/rdaclif.h>, not <rdaclif.h>
 * 	[1994/05/17  19:22:22  sommerfeld]
 * 
 * Revision 1.1.5.1  1994/03/12  22:03:07  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:15:11  peckham]
 * 
 * Revision 1.1.3.3  1992/12/30  13:30:50  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:49:25  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  19:14:48  devsrc
 * 	[OT 5373] SNI/SVR4 merge UPDATE:  removed the conditional SNI_DCOSX from the
 * 	          changes to the prototype.
 * 	[1992/09/23  20:58:31  sekhar]
 * 
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:13:20  weisman]
 * 
 * Revision 1.1  1992/01/19  15:17:00  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1991 by
**	Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
** @OSF_COPYRIGHT@
**
**  NAME:
**
**      dpeacl.h
**
**  FACILITY:
**
**      DPE acl library (libdpeacl.a)
**
**  ABSTRACT:
**
**  	This module contains the public interface to a set of routines that
**	implement an acl manager for DPE applications that are DCE RPC servers.
**
*/

#include <dce/aclbase.h>
#include <dce/dns_record.h>
#include <dce/rdaclif.h>

#define PRIVATE
#define PUBLIC
#define SET_STATUS(stp, val)      (*stp) = val
#define CLEAR_STATUS(stp)         (*stp) = error_status_ok

/*
 * Return status codes
 */
#define dpe_acl_success	0
#define dpe_acl_failure	1

/*
 * Fake status codes for the dpe_acl routines - we should figure out how to
 * assign real ones
 */
#define dpe_acl_s_ok		0
#define dpe_acl_s_corrupt	1
#define dpe_acl_s_no_file	2
#define dpe_acl_s_no_access	3
#define dpe_acl_s_open_error	4
#define dpe_acl_s_read_error	5
#define dpe_acl_s_no_mem	6
#define dpe_acl_s_write_error	7


/*
 * Memory allocation and freeing macros
 *
 * Sometimes acls need to be allocated by the rpc memory routines, so they can
 * be passed in rpc calls.  These macros will select the normal or the rpc
 * stub memory allocation routine.
 */
#define DPE_ACL_RPCMEM	0
#define DPE_ACL_STDMEM 	1

#define DPE_ACL_MALLOC(size, data_type, mem_type) \
    (mem_type ? (data_type)malloc(size) : (data_type)rpc_ss_allocate(size))

/*
 * dpe_acl_client_get_pac
 *
 * Get the authentication information (PAC) for a given RPC client.  Use
 * the RPC binding handle to obtain the PAC by calling
 * rpc_binding_inq_auth_client.  If there is no PAC present, a NULL pointer is
 * returned.  If the authorization service selected by the client is not
 * rpc_c_authz_dce, the an error is returned.
 */
void 
dpe_acl_client_get_pac (
   handle_t	       ,
   sec_id_pac_t	     **,
   error_status_t     *);


/*
 * dpe_acl_client_is_authorized
 *
 * This routine is called by an RPC server to see if a client is authorized
 * to perform a certain function.  This routine takes the client binding
 * handle, the acl for the server interface being called, and the
 * permissions needed to perform that server function.  It returns TRUE if
 * the acl allows the requested access by the client and FALSE otherwise.
 *
 * This routine uses the client's binding handle to obtain the PAC for the
 * client.  If no PAC is available, the client is assumed to be unauthenticated.
 * and is treated as such for access checking.
 */
boolean32 
dpe_acl_client_is_authorized (
   sec_acl_p_t		,
   handle_t		,
   sec_acl_permset_t 	,
   error_status_t	*);

/*
**++
**
**  ROUTINE NAME:	dpe_acl_storage_read
**
**  SCOPE:		PUBLIC - declared in dpeacl.h
**
**  DESCRIPTION:
**
**	read an acl from the specified file.
**
**  INPUTS:
**
**	file_name	is the name of the acl file to read.
**
**  INPUTS/OUTPUTS:	none
**
**  OUTPUTS:
**
**	acl_p		is a pointer to the acl read from the file.  this is
**			a pointer to memory allocated by rpc_ss_allocate.  it
**			is the caller's responsibility to deallocate this
**			storage with rpc_ss_free.
**
**	status		is one of the following values:
**
**	dpe_acl_s_ok		the acl was read successfully.  acl_p points to
**				a valid acl.
**
**	dpe_acl_s_corrupt	the acl in the file is corrupt.
**
**  	dpe_acl_s_no_file	file or directory specified by file_name does
**				not exist.
**
**  	dpe_acl_s_no_access	caller does not have permission to access file
**				specified by file_name.
**
**	dpe_acl_s_open_error	system-dependent error opening file file_name.
**
**  	dpe_acl_s_read_error	system-dependent error reading file file_name.
**
**	dpe_acl_s_no_mem	can't allocate memory to hold the acl.
**
**  IMPLICIT INPUTS:
**
**	the file specified by file_name.
**
**  IMPLICIT OUTPUTS:	none
**
**  FUNCTION VALUE:	none
**
**  SIDE EFFECTS:	none
**
**--
**/
void 
dpe_acl_storage_read (
   char		    *,
   sec_acl_p_t	    *,
   error_status_t   *);

/*
**++
**
**  ROUTINE NAME:	dpe_acl_free_acl
**
**  SCOPE:		PUBLIC - declared in dpeacl.h
**
**  DESCRIPTION:
**
**	Deallocate an acl buffer allocated with any of the dpe_acl routines or
**	with any of the acl_xxx_to_wire routines.  These routines all allocate
**	acls with rpc_ss_allocate, so this routine uses rpc_ss_free to
**	deallocate it.
**
**  INPUTS:
**
**	acl_p		the acl to be deallocated.
**
**  INPUTS/OUTPUTS:	none
**
**  OUTPUTS:		none
**
**  IMPLICIT INPUTS:	none
**
**  IMPLICIT OUTPUTS:	none
**
**  FUNCTION VALUE:	none
**
**  SIDE EFFECTS:	none
**
**--
**/
void 
dpe_acl_free_acl (
  sec_acl_p_t	  );




boolean32 
dpe_acl_mgr_is_authorized (
   sec_acl_t		*,
   sec_acl_permset_t	,
   sec_id_pac_t		*,
   error_status_t       *);
#define NO_PERMISSIONS	((sec_acl_permset_t) 0)		/* NO permissions. */
boolean32 
dpe_acl_mgr_get_access (
   sec_acl_t		*,
   rpc_authz_cred_handle_t *,
   sec_acl_permset_t	*,
   error_status_t	*);


unsigned int 
dpe_acl_wire_to_flat (
   sec_acl_p_t	  , 
   int		  ,
   char 	 **,
   int		  *);

unsigned int 
dpe_acl_flat_to_wire (
   char        	*, 
   int		,
   sec_acl_p_t 	*);

unsigned int 
dpe_acl_wire_to_wire (
   sec_acl_p_t	  , 
   int		  ,
   sec_acl_p_t 	  *);

#endif  /* #ifndef _DPEACL_H */
