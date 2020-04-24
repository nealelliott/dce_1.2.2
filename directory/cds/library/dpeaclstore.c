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
 * $Log: dpeaclstore.c,v $
 * Revision 1.1.8.2  1996/02/18  19:34:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:15:04  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:27:01  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:47:20  root]
 * 
 * Revision 1.1.5.3  1994/08/25  19:30:48  proulx
 * 	Delegation support.
 * 	[1994/08/25  19:20:08  proulx]
 * 
 * Revision 1.1.5.2  1994/06/30  19:17:48  mccann
 * 	sams cleanup drop2
 * 	[1994/06/30  19:04:03  mccann]
 * 
 * Revision 1.1.5.1  1994/03/12  22:08:30  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:34:15  peckham]
 * 
 * Revision 1.1.3.3  1992/12/30  14:16:56  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  23:16:30  zeliff]
 * 
 * Revision 1.1.3.2  1992/09/29  18:52:50  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:15:57  weisman]
 * 
 * Revision 1.1  1992/01/19  15:18:58  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
#if !defined(lint) && !defined(SABER)
static char *SccsID = "@(#)dpeaclstore.c\t7\t(DECdns)\t11/17/1991";
#endif	/* neither lint nor saber-C */
/*
**  Copyright (c) 1991 by
**	Hewlett-Packard Company, Palo Alto, Ca. &
**      Digital Equipment Corporation, Maynard, Mass.
**
** @OSF_COPYRIGHT@
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
#include <dce/dcecdsmsg.h>
#include <dcecdsmac.h>
#include <dcecdssvc.h>
#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <dce/rpc.h>
#include <dce/dce_error.h>
#include <errno.h>

#include <dpeacl.h>


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
	dpe_acl_status = dpe_acl_s_no_access; \
	break; \
    case ENOENT: \
	dpe_acl_status = dpe_acl_s_no_file; \
	break; \
    default: \
	dpe_acl_status = dpe_acl_s_open_error; \
	break; \
    } \
} \


/*
 * Internal routines
 */
static void
read_acl_file(
     int		 ,
     char		*,
     int		 ,
     error_status_t	*);

static unsigned32 
acl_checksum(
     char	*flat_acl_p);


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
dpe_acl_storage_read (char            *file_name,
                      sec_acl_p_t     *acl_p,
                      error_status_t  *status)
{
    int		acl_fd;
    unsigned32	file_checksum;
    unsigned32	computed_checksum;
    unsigned32	flat_acl_size;
    char	*flat_acl_p;


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
    read_acl_file (acl_fd,
		   (char *)&flat_acl_size,
		   sizeof(flat_acl_size),
		   status);

    if (*status != dpe_acl_s_ok)
    {
	close (acl_fd);
	return;
    }


    /*
     * Now that we know the size, we can allocate a buffer for the flat acl
     */
    flat_acl_p = (char *)malloc (flat_acl_size);
    if (!flat_acl_p)
    {
	*status = dpe_acl_s_no_mem;
	close (acl_fd);
	return;
    }


    /*
     * Read in the flat acl
     */
    read_acl_file (acl_fd, flat_acl_p, (int)flat_acl_size, status);
    if (*status != dpe_acl_s_ok)
    {
	free (flat_acl_p);
	close (acl_fd);
	return;
    }


    /*
     * Read in the checksum, an unsigned32.
     */
    read_acl_file (acl_fd,
		   (char *)&file_checksum,
		   sizeof(file_checksum),
		   status);
    if (*status != dpe_acl_s_ok)
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
	*status = dpe_acl_s_corrupt;
	close (acl_fd);
	return;
    }


    /*
     * Now convert the flat acl to the normal in-memory format and free the
     * flat acl.
     */
    if (dpe_acl_flat_to_wire (flat_acl_p, DPE_ACL_STDMEM, acl_p))
    {
	*status = dpe_acl_s_no_mem;
	return;
    }
    free (flat_acl_p);

    *status = dpe_acl_s_ok;

} /* end of routine dpe_acl_storage_read */


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
**	with any of the acl_xxx_to_wire routines.
**
**	Walk down the acl structure, freeing all the structures linked to it,
**	including any structures attached to any of the acl entries.
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
dpe_acl_free_acl (sec_acl_p_t acl_p)
{
    sec_acl_entry_t *entry_p;
    int             i;


    /*
     * Free the realm name.
     */
    free (acl_p->default_realm.name);


    /*
     * Go through the array of acl entries, freeing any structures attached to
     * them.
     */
    for (i = 0; i < acl_p->num_entries; i++)
    {
        entry_p = &(acl_p->sec_acl_entries[i]);

        switch (entry_p->entry_info.entry_type)
	{
	case sec_acl_e_type_user:
	case sec_acl_e_type_group:
	case sec_acl_e_type_foreign_other:
	    /*
	     * These entries have a single principle name linked to them
	     */
	    free (entry_p->entry_info.tagged_union.id.name);
	    break;

	case sec_acl_e_type_foreign_user:
	case sec_acl_e_type_foreign_group:
	    /*
	     * These entries have a foreign principle name and a foreign realm
	     * name linked to them.
	     */
	    free (entry_p->entry_info.tagged_union.foreign_id.id.name);
	    free (entry_p->entry_info.tagged_union.foreign_id.realm.name);
	    break;

	case sec_acl_e_type_extended:
	    /*
	     * This entry has a sec_acl_extend_info_t structure linked to it.
	     */
	    free (entry_p->entry_info.tagged_union.extended_info);

	default:
	    break;
        }
    }

    /*
     * Now free the acl structure itself
     */
    free (acl_p);

} /* end of routine dpe_acl_free_acl */


/*
**++
**
** ROUTINE NAME:	read_acl_file ()
**
**	Read bytes from the given acl file.
**
**--
**/
static void
read_acl_file (int             acl_fd,
               char            *buf,
               int             nbytes,
               error_status_t  *status)
{
    int	size_read;


    *status = dpe_acl_s_ok;

    size_read = read (acl_fd, buf, nbytes);
    if (size_read < 0)
    {
	*status = dpe_acl_s_read_error;
    }
    else
    {
	if (size_read < nbytes)
	{
	    *status = dpe_acl_s_corrupt;
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
static unsigned32 
acl_checksum (char *flat_acl_p)
{
#define DPE_ACL_FAKE_CHECKSUM 0x55555555

    return (DPE_ACL_FAKE_CHECKSUM);

} /* end of routine write_acl_file */

/*  DEC/CMS REPLACEMENT HISTORY, Element DPEACLSTORE.C */
/*  *12   11-NOV-1991 16:18:45 COMUZZI "Fixed more lint" */
/*  *11    5-NOV-1991 15:43:10 COMUZZI "Fixed lint problems" */
/*  *10    5-NOV-1991 09:35:53 COMUZZI "Add missing entry on errno" */
/*  *9    11-OCT-1991 11:45:27 B_BAILEY "move pthread.h to the top" */
/*  *8     7-OCT-1991 09:38:47 B_BAILEY "fix compilation errors in cma environment" */
/*  *7     4-OCT-1991 16:45:00 B_BAILEY "make it ansi" */
/*  *6    23-SEP-1991 14:24:47 B_BAILEY "mem_type is unnecessary in dpe_acl_free_acl" */
/*  *5    23-SEP-1991 13:32:13 B_BAILEY "Add switch to select malloc or rpc_ss_allocate" */
/*  *4    20-SEP-1991 15:31:43 B_BAILEY "fix include files" */
/*  *3    20-SEP-1991 10:07:11 B_BAILEY "fix ansi compilation errors" */
/*  *2    18-SEP-1991 14:51:03 B_BAILEY "fold in cds_acl and acl_convert routines" */
/*  *1    26-AUG-1991 16:54:30 B_BAILEY "dpe acl storage manager" */
/*  DEC/CMS REPLACEMENT HISTORY, Element DPEACLSTORE.C */
