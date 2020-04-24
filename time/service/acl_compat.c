/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 *(c) Copyright 1991,1992 Siemens-Nixdorf Information Systems,Burlington,MA,USA.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: acl_compat.c,v $
 * Revision 1.1.4.2  1996/02/17  23:35:16  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:51:30  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:13:12  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:25:25  root]
 * 
 * Revision 1.1.2.2  1994/08/23  20:32:19  cbrooks
 * 	Code Cleanup
 * 	[1994/08/19  17:52:57  cbrooks]
 * 
 * 	Embedding copyright notice
 * 	[1992/12/28  22:10:35  htf]
 * 
 * 	[OT 5373]  SNI/SVR4 Merge.
 * 	[1992/09/14  20:02:22  sekhar]
 * 
 * Revision 1.1.2.1  1994/05/31  21:29:18  cbrooks
 * 	CR 10611 - DTS needs new ACL manager
 * 	[1994/05/31  21:03:58  cbrooks]
 * 
 * Revision 1.1.6.1  1994/04/21  22:04:56  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  20:05:08  cbrooks]
 * 
 * Revision 1.1  1992/01/19  15:32:21  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1991 by
**	Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME:
**
**      dpeaclstore.c
**
**  FACILITY:
**
**      DPE acl library (libdpeacl.a)
**
**  ABSTRACT:
**
**  	This module contains routines that implement an acl storage manager for
**	DPE applications that are DCE RPC servers.
**
*/
#include <dce/dce.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/file.h>

#include <fcntl.h>

#include <dce/rpc.h>
#include <dce/dce_error.h>
#include <errno.h>

#include <sys/types.h>	
#include <stdlib.h>		/* for malloc() */
#include <dce/aclbase.h>

#include <dcedtssvc.h>

#ifndef NULL
#define NULL 0
#endif

#define PRIVATE
#define PUBLIC
#define SET_STATUS(stp, val)      (*stp) = val
#define CLEAR_STATUS(stp)         (*stp) = error_status_ok

/*
 * Return status codes
 */
#define DTS_ACL_SUCCESS	0
#define DTS_ACL_FAILURE	1

/* todo: remove these to complete code cleanup */
#define DPE_ACL_SUCCESS       0
#define DPE_ACL_FAILURE       1

/*
 * Fake status codes for the dpe_acl routines - we should figure out how to
 * assign real ones
 */
#define DTS_ACL_S_OK		0
#define DTS_ACL_S_CORRUPT	1
#define DTS_ACL_S_NO_FILE	2
#define DTS_ACL_S_NO_ACCESS	3
#define DTS_ACL_S_OPEN_ERROR	4
#define DTS_ACL_S_READ_ERROR	5
#define DTS_ACL_S_NO_MEM	6
#define DTS_ACL_S_WRITE_ERROR	7

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


/* Alignment macros */

# define	ALIGN_SIZE sizeof(int)

#define ALIGN_MASK (ALIGN_SIZE - 1)

#define ALIGN_POINTER(p) \
    ( ((unsigned long )((char *)p + ALIGN_MASK)) & ~ALIGN_MASK)

extern dce_svc_handle_t 	dts_svc_handle ;

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
**	DTS_ACL_OK		the acl was read successfully.  acl_p points to
**				a valid acl.
**
**	DTS_ACL_CORRUPT	the acl in the file is corrupt.
**
**  	DTS_ACL_NO_FILE	file or directory specified by file_name does
**				not exist.
**
**  	DTS_ACL_NO_ACCESS	caller does not have permission to access file
**				specified by file_name.
**
**	DTS_ACL_OPEN_ERROR	system-dependent error opening file file_name.
**
**  	DTS_ACL_READ_ERROR	system-dependent error reading file file_name.
**
**	DTS_ACL_NO_MEM	can't allocate memory to hold the acl.
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

void dpe_acl_storage_read (
    char		* /*file_name*/,
    sec_acl_p_t	* /*acl_p*/,
    error_status_t	* /* status*/
);


/*
**++
**
**  ROUTINE NAME:	dpe_acl_storage_write
**
**  SCOPE:		PUBLIC - declared in dpeacl.h
**
**  DESCRIPTION:
**
**	Write an acl to the specified file.  Creates the file if it doesn't
**	exist and overwrites any existing file.
**
**  INPUTS:
**
**	file_name	is the name of the acl file to write.
**
**  INPUTS/OUTPUTS:	none
**
**  OUTPUTS:
**
**	acl_p		is a pointer to the acl to write to the file.
**
**	status		is one of the following values:
**
**	DTS_ACL_OK		the acl was read successfully.  acl_p points to
**				a valid acl.
**
**  	DTS_ACL_NO_ACCESS	caller does not have permission to access file
**				specified by file_name.
**
**	DTS_ACL_OPEN_ERROR	system-dependent error opening file file_name.
**
**  	DTS_ACL_WRITE_ERROR	system-dependent error writing file file_name.
**
**	DTS_ACL_NO_MEM	can't allocate memory to hold the acl.
**
**
**  IMPLICIT INPUTS:	none
**
**  IMPLICIT OUTPUTS:
**
**	the file specified by file_name.
**
**  FUNCTION VALUE:	none
**
**  SIDE EFFECTS:
**
**	the file is created if it doesn't exist.  If it does exist, it is
**	overwritten.
**
**--
**/


void dpe_acl_storage_write (
    char		* /*file_name*/,
    sec_acl_p_t	 /*acl_p*/,
    error_status_t	*
);


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
void dpe_acl_free_acl (sec_acl_p_t);

boolean32 dpe_acl_mgr_is_authorized (
	 sec_acl_t		* /*sec_acl_p*/,
	 sec_acl_permset_t	/*desired_access*/, 
	 sec_id_pac_t		* /*accessor_info*/,
	 error_status_t		* /*st_p*/
);

#define NO_PERMISSIONS	((sec_acl_permset_t) 0)		/* NO permissions. */

static unsigned int dpe_acl_flat_to_wire (
    sec_acl_p_t	   	/*wire_acl*/, 
    int		   	/*mem_type*/,
    sec_acl_p_t * /* new_wire_acl */					  
);

/*
 * Unix file permission mask (in octal) for the acl file.  It allows owner read
 * and write access and group and other read access (-rw--r--r).
 */
#define DPE_ACL_FILE_PERMS	0644

/*
 * Macros
 */
#define XLATE_OPEN_ERRNO(dpe_acl_status) \
{ \
    switch (errno)\
    { \
    case EACCES: \
	dpe_acl_status = DTS_ACL_S_NO_ACCESS; \
	break; \
    case ENOENT: \
	dpe_acl_status = DTS_ACL_S_NO_FILE; \
	break; \
    default: \
	dpe_acl_status = DTS_ACL_S_OPEN_ERROR; \
	break; \
    } \
} \


/*
 * Internal routines
 */

static int read_acl_file ( int , char *, int , error_status_t	*);
static unsigned32 acl_checksum ( char	*) ;

/*
**++
**
**  ROUTINE NAME:	dpe_acl_storage_read
**
**  SCOPE:		PUBLIC - declared in dpeacl.h
**
**  DESCRIPTION:
**
**	Read an acl from the specified file.  Return the acl in wire format.
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
**			storage with dpe_acl_free_acl.
**
**	status		is one of the following values:
**
**	DTS_ACL_S_OK		the acl was read successfully.  acl_p points to
**				a valid acl.
**
**	DTS_ACL_S_CORRUPT	the acl in the file is corrupt.
**
**  	DTS_ACL_S_NO_FILE	file or directory specified by file_name does
**				not exist.
**
**  	DTS_ACL_S_NO_ACCESS	caller does not have permission to access file
**				specified by file_name.
**
**	DTS_ACL_S_OPEN_ERROR	system-dependent error opening file file_name.
**
**  	DTS_ACL_S_READ_ERROR	system-dependent error reading file file_name.
**
**	DTS_ACL_S_NO_MEM	can't allocate memory to hold the acl.
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
void dpe_acl_storage_read (
    char		*file_name,
    sec_acl_p_t		*acl_p,
    error_status_t	*status
)
{
    int		acl_fd;
    unsigned32	file_checksum;
    unsigned32	computed_checksum;
    unsigned32	flat_acl_size;
    void *      flat_acl_p;


    /*
     * Open the acl file for read.  If this fails, translate the error to a
     * dpe_acl error and return.
     */

    acl_fd = open (file_name, O_RDONLY, 0);
    if (acl_fd < 0)
    {
	XLATE_OPEN_ERRNO(*status);
	return;
    }
    

    /*
     * Read in the size of the flat acl, an unsigned32.
     */
    (void) read_acl_file (acl_fd,
			  (char *)&flat_acl_size,
			  sizeof(flat_acl_size),
			  status);
    if (*status != DTS_ACL_S_OK)
    {
	close (acl_fd);
	return;
    }


    /*
     * Now that we know the size, we can allocate a buffer for the flat acl
     */
    flat_acl_p = (void *)malloc (flat_acl_size);
    if (!flat_acl_p)
    {
	*status = DTS_ACL_S_NO_MEM;
	close (acl_fd);
	return;
    }


    /*
     * Read in the flat acl
     */
    (void) read_acl_file (acl_fd, flat_acl_p, (int)flat_acl_size, status);
    if (*status != DTS_ACL_S_OK)
    {
	free (flat_acl_p);
	close (acl_fd);
	return;
    }


    /*
     * Read in the checksum, an unsigned32.
     */
    (void) read_acl_file (acl_fd,
			  (char *)&file_checksum,
			  sizeof(file_checksum),
			  status);
    if (*status != DTS_ACL_S_OK)
    {
	free (flat_acl_p);
	close (acl_fd);
	return;
    }

    /*
     * We're done with the file now
     */
    close (acl_fd);


    /*
     * Now calculate a checksum on the flat acl and compare it with the one
     * read from the file.  If they don't match, the acl file is corrupt.
     */
    computed_checksum = acl_checksum (flat_acl_p);
    if (computed_checksum != file_checksum)
    {
	*status = DTS_ACL_S_CORRUPT;
	close (acl_fd);
	return;
    }


    /*
     * Now convert the flat acl to the normal in-memory format and free the
     * flat acl.
     */
    if (dpe_acl_flat_to_wire ((sec_acl_p_t)flat_acl_p, DPE_ACL_STDMEM, acl_p))
    {
	*status = DTS_ACL_S_NO_MEM;
	return;
    }
    free (flat_acl_p);

    *status = DTS_ACL_S_OK;

} /* end of routine dts_acl_storage_read */

/*
**++
**
** ROUTINE NAME:	read_acl_file ()
**
**	Read bytes from the given acl file.
**
**--
**/

static int read_acl_file ( 
    int		acl_fd,
    char		*buf,
    int		nbytes,
    error_status_t	*status)
{
    int	size_read;


    *status = DTS_ACL_S_OK;

    size_read = read (acl_fd, buf, nbytes);
    if (size_read < 0)
    {
	*status = DTS_ACL_S_READ_ERROR;
    }
    else
    {
	if (size_read < nbytes)
	{
	    *status = DTS_ACL_S_CORRUPT;
	}
    }

} /* end of routine read_acl_file */

/*
**++
**
** ROUTINE NAME:	acl_checksum ()
**
**	compute a checksum over an acl in flat format.
**
**--
**/
static unsigned32 acl_checksum (
    char	*flat_acl_p
)
{
#define DPE_ACL_FAKE_CHECKSUM 0x55555555

    return (DPE_ACL_FAKE_CHECKSUM);

} /* end of routine write_acl_file */


/* d p e _ a c l _ f l a t _ t o _ w i r e
**
** This function receives as input (flat_acl) a pointer to a contiguous acl 
** structure and returns as an output (wire_acl) a pointer to an initialized 
** acl data structure.
** 
** At this time it will be the callers responsibility to both free the input 
** and output data structures. 
** 
** ERROR CODES: 
**      DTS_ACL_SUCCESS 
**      DTS_ACL_FAILURE 
**
*/

static unsigned int dpe_acl_flat_to_wire
    (
    sec_acl_p_t   flat_acl, 
    int		  mem_type,
    sec_acl_p_t * new_wire_acl
    )

{  
   int                   num;
   int                   size;
   sec_acl_extend_info_t *extend_p;
   sec_acl_t             *wire_acl_p, *flat_acl_p;
   sec_acl_entry_t       *wire_entry_p, *flat_entry_p;
   char                  *tmp_p;

   /*
   ** Allocate a sec_acl_t block
   */
   *new_wire_acl = 
     (sec_acl_p_t)DPE_ACL_MALLOC (sizeof(sec_acl_t), sec_acl_p_t, mem_type);
   if (*new_wire_acl == NULL)
     return(DTS_ACL_FAILURE);
 
   /*
   ** Assign wire_acl to address on new_wire acl pointer
   */
   wire_acl_p = *new_wire_acl;
   flat_acl_p = (sec_acl_t *)flat_acl;

   *wire_acl_p = *flat_acl_p;

   /* 
   ** Allocate space for sec_acl_entry_t array 
   */
   wire_acl_p->sec_acl_entries =
         DPE_ACL_MALLOC ((sizeof(sec_acl_entry_t) * flat_acl_p->num_entries),
		         sec_acl_entry_t *,
	                 mem_type);
   if (wire_acl_p->sec_acl_entries == NULL)
     return(DTS_ACL_FAILURE);

   /*
   ** Copy acl entries from flat to wire, fix up ptrs later
   */
   flat_entry_p = (sec_acl_entry_t *)(flat_acl_p + 1);
   flat_entry_p = (sec_acl_entry_t *)ALIGN_POINTER(flat_entry_p);
   wire_entry_p = wire_acl_p->sec_acl_entries;

   memcpy((void *)wire_entry_p,
          (void *)flat_entry_p, 
	  (size_t)(sizeof(sec_acl_entry_t) * flat_acl_p->num_entries));

   tmp_p = (char *)(flat_entry_p + flat_acl_p->num_entries);

   /* 
   ** Allocate a realm_name block 
   */ 
   if (flat_acl_p->default_realm.name) {
       size = strlen(tmp_p) + sizeof(idl_char);
       wire_acl_p->default_realm.name = 
	 DPE_ACL_MALLOC(size, idl_char *, mem_type); 
       if (wire_acl_p->default_realm.name == NULL)
	 return(DTS_ACL_FAILURE);
       strcpy ((char *)(wire_acl_p->default_realm.name), tmp_p);
       tmp_p += size;
   } else
     wire_acl_p->default_realm.name = NULL;

   /*
   ** 
   ** Index down flat acl retrieving variable length data
   **
   */

   for (num=0;
        num <  flat_acl_p->num_entries; ++num, ++flat_entry_p, ++wire_entry_p)
   {
    
       switch (flat_entry_p->entry_info.entry_type) 
       {
       
         case sec_acl_e_type_extended :
#ifdef DCE_DEBUG
	   DCE_SVC_DEBUG((dts_svc_handle, dts_s_msgs, svc_c_debug1, 
			  "Extension Block found\n"));
#endif
	   /*
	    ** Get extension block
	    */ 
	   tmp_p = (char *)ALIGN_POINTER(tmp_p);
	   extend_p = (sec_acl_extend_info_t *)((void *)tmp_p);
	   size = sizeof(sec_acl_extend_info_t) + 
	     (extend_p->num_bytes * sizeof(idl_byte));

            /*
            ** Allocate space for sec_acl_extend_info_t block
            */
            wire_entry_p->entry_info.tagged_union.extended_info = 
                   DPE_ACL_MALLOC (size,
				   sec_acl_extend_info_t *,
				   mem_type); 
            if (wire_entry_p->entry_info.tagged_union.extended_info == NULL)
	      return(DTS_ACL_FAILURE);

            /*
            ** Copy flat acl extension block into wire acl
            */
	   memcpy((void *)wire_entry_p->entry_info.tagged_union.extended_info,
		  (void *)tmp_p,
                  (size_t)size);
	   tmp_p += size;
	   break;
        
        case sec_acl_e_type_user:
        case sec_acl_e_type_group:
        case sec_acl_e_type_foreign_other:
	   /*
            ** Allocate space for id name
            */
	   if (flat_entry_p->entry_info.tagged_union.id.name) {
	       size = strlen(tmp_p) + sizeof(idl_char );
	       wire_entry_p->entry_info.tagged_union.id.name =
			    DPE_ACL_MALLOC(size, idl_char *, mem_type);
	       if (wire_entry_p->entry_info.tagged_union.id.name == NULL)
		 return(DTS_ACL_FAILURE);
	       strcpy((char *)(wire_entry_p->entry_info.tagged_union.id.name),
                      tmp_p);
	       tmp_p += size;
	   } else
	     wire_entry_p->entry_info.tagged_union.id.name = NULL;
            break;
        case sec_acl_e_type_foreign_user:
        case sec_acl_e_type_foreign_group :
#ifdef DCE_DEBUG
	  DCE_SVC_DEBUG((dts_svc_handle, dts_s_msgs, 
			 svc_c_debug1, "Foreign Block found\n"));
#endif      
	   
	   /*
            ** Allocate space for foreign id name
            */
	   if (flat_entry_p->entry_info.tagged_union.foreign_id.id.name) {
	       size = strlen(tmp_p) + sizeof(idl_char);
	       wire_entry_p->entry_info.tagged_union.foreign_id.id.name =
			    DPE_ACL_MALLOC (size, idl_char *, mem_type);
	       if (wire_entry_p->
                        entry_info.tagged_union.foreign_id.id.name == NULL)
		   return(DTS_ACL_FAILURE);
	       strcpy((char *)(wire_entry_p->
	                          entry_info.tagged_union.foreign_id.id.name),
		      tmp_p);
	       tmp_p += size;
	   } else
	     wire_entry_p->entry_info.tagged_union.foreign_id.id.name = NULL;

            /*
            ** Allocate space for foreign realm name
            */
	   if (flat_entry_p->entry_info.tagged_union.foreign_id.realm.name) {
	       size = strlen(tmp_p) + sizeof(idl_char);
	       wire_entry_p->entry_info.tagged_union.foreign_id.realm.name =
                    DPE_ACL_MALLOC (size, idl_char *, mem_type);
	       if (wire_entry_p->
                      entry_info.tagged_union.foreign_id.realm.name == NULL)
	           return(DTS_ACL_FAILURE);
	       strcpy((char *)(wire_entry_p->
                               entry_info.tagged_union.foreign_id.realm.name),
	  	      tmp_p);
	       tmp_p += size;
	   } else
	     wire_entry_p->entry_info.tagged_union.foreign_id.realm.name =NULL;

            break;                       
	 default:
	   break;
        } /* end case */

   } /* end for */

   return(DTS_ACL_SUCCESS);
}
