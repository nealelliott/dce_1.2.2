/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: mgtacl.c,v $
 * Revision 1.1.64.2  1996/02/17  23:35:46  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:52:01  marty]
 *
 * Revision 1.1.64.1  1995/12/08  18:14:09  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/15  14:17 UTC  pare
 * 	Create HPDCE02 branch and merge changes
 * 
 * 	HP revision /main/pare_MOTHRA/1  1995/09/15  14:13 UTC  pare
 * 	Fix memory leak, free new_acl_filename_p -- CHFts16224
 * 	[1995/12/08  17:26:07  root]
 * 
 * Revision 1.1.62.13  1994/08/23  20:32:36  cbrooks
 * 	Code Cleanup - ANSI switches
 * 	[1994/08/23  15:26:30  cbrooks]
 * 
 * 	Code Cleanup Checkpoint
 * 	[1994/08/23  13:57:10  cbrooks]
 * 
 * Revision 1.1.62.12  1994/08/05  20:47:46  cbrooks
 * 	CR11240 - add dced config functionality
 * 	[1994/08/05  20:23:46  cbrooks]
 * 
 * Revision 1.1.62.11  1994/07/18  19:02:01  cbrooks
 * 	Change strdup to dce_strdup
 * 	[1994/07/18  17:13:04  cbrooks]
 * 
 * 	CR 11304 - modify all ACL manager print strings
 * 	[1994/07/14  19:54:40  cbrooks]
 * 
 * Revision 1.1.62.10  1994/06/30  19:45:30  cbrooks
 * 	CR11112 - new SAMS messages
 * 	[1994/06/28  20:13:45  cbrooks]
 * 
 * Revision 1.1.62.9  1994/06/10  22:26:20  cbrooks
 * 	CR10897 - use rpc_sm_enable_alloc()
 * 	[1994/06/10  22:24:17  cbrooks]
 * 
 * 	CR 10897
 * 	[1994/06/10  21:11:46  cbrooks]
 * 
 * Revision 1.1.62.8  1994/06/07  14:44:07  bowe
 * 	Added rpc_sm_enable_allocate(). [CR 10757]
 * 	[1994/06/07  14:41:40  bowe]
 * 
 * Revision 1.1.62.7  1994/06/02  17:59:02  tom
 * 	Bug 10836 - must always execute db_open for acl.
 * 	[1994/06/02  17:58:54  tom]
 * 
 * Revision 1.1.62.6  1994/05/31  21:29:28  cbrooks
 * 	Merged with changes from 1.1.62.5
 * 	[1994/05/31  21:28:37  cbrooks]
 * 
 * 	Merge in DTS code cleanup changes
 * 	[1994/05/31  21:16:00  cbrooks]
 * 
 * 	CR 10611 - DTS needs new ACL manager
 * 	[1994/05/31  21:04:01  cbrooks]
 * 
 * Revision 1.1.62.5  1994/05/16  18:51:58  rhw
 * 	fix for CR 10642
 * 	[1994/05/16  03:34:34  rhw]
 * 
 * Revision 1.1.62.4  1994/04/28  21:08:55  cbrooks
 * 	DTS Code cleanup
 * 	[1994/04/27  19:41:57  cbrooks]
 * 
 * Revision 1.1.62.3  1994/04/21  22:06:14  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  20:00:04  cbrooks]
 * 
 * 	Checkpoint:4.15.94
 * 	[1994/04/15  21:15:33  cbrooks]
 * 
 * 	Checkpoint
 * 	[1994/03/28  20:28:51  cbrooks]
 * 
 * Revision 1.1.62.2  1994/01/21  21:49:58  rsalz
 * 	Null checkin
 * 	[1994/01/21  21:49:37  rsalz]
 * 
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/21  21:07:33  rsalz]
 * 
 * Revision 1.1.62.1  1994/01/21  21:08:43  rsalz
 * 	OT CR 9747:  Add SVC and AUD to dtsd
 * 	[1994/01/21  21:07:33  rsalz]
 * 
 * Revision 1.1.5.3  1993/01/22  21:47:32  tom
 * 	Bug 6968 - rdacl_get_mgr_types_semantics arguments were mixed up in
 * 	the non-STDC case.
 * 	[1993/01/22  19:55:36  tom]
 * 
 * Revision 1.1.5.2  1992/12/30  16:39:41  htf
 * 	Embedding copyright notice
 * 	[1992/12/28  22:23:16  htf]
 * 
 * Revision 1.1.2.4  1992/05/12  17:54:39  ko
 * 	Fix ot 2150, replace DCE_* with LogDCEevent
 * 	[1992/05/12  17:50:34  ko]
 * 
 * Revision 1.1.2.3  1992/05/08  15:55:38  burati
 * 	Added rdacl_get_mgr_types_semantics.
 * 	[1992/05/07  18:38:09  burati]
 * 
 * Revision 1.1.2.2  1992/04/23  14:29:20  comuzzi
 * 	<<<Eliminated refereces to obsolete rdacliftmp>>>
 * 	[1992/04/21  20:48:15  comuzzi]
 * 
 * Revision 1.1  1992/01/19  15:34:34  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * 	Module MGTACL.C
 */
/*
 * Copyright (c) 1991 by
 * Digital Equipment Corporation, Maynard, Mass.
 *
 * This software is furnished under a license and may be used and copied
 * only  in  accordance  with  the  terms  of such  license and with the
 * inclusion of the above copyright notice.  This software or any  other
 * copies thereof may not be provided or otherwise made available to any
 * other person.  No title to and ownership of the  software  is  hereby
 * transferred.
 *
 * The information in this software  is subject to change without notice
 * and  should  not be  construed  as a commitment by Digitial Equipment
 * Corporation.
 *
 * Digital assumes no responsibility for the use or  reliablity  of its
 * software on equipment which is not supplied by Digital.
 *
 * FACILITY:	Distributed Time Service (DTS)
 *
 * ABSTRACT:	This module contains the routines and data structures used
 *		to protect the DTS management interface with DCE acl's (dacl's).
 *
 * ENVIRONMENT:	OSF DCE
 *
 * AUTHORS:
 *		Brian Bailey
 *		Distributed Processing Engineering (DPE)
 *
 * DATE:	June 1991
 *
 */

#include <dts.h>
#include <dce/dce_utils.h>
#include <dtss_includes.h>
#include <unistd.h>

#if CONFIG_SECURITY

#include <dce/aclif.h>
#include <dce/rdaclif.h>	/* new ACL interface */
#include <dce/rdaclifv0.h>	/* old ACL interface */
#include <dce/dceacl.h>
#include <dce/binding.h>
#include <dce/pgo.h>

#include <dcedtssvc.h>
#include <dcedtsmac.h>


#define K_V1_MGT_ACL_FILE "dtsd.acl"


extern ShmBuf	*shmPtr;	/* DTS global state buffer */
extern RpcBuf	*rpcBufPtr;	/* DTS global rpc state buffer */

/*
 * Macro to copy a sec_id_t
 */
#define COPY_SEC_ID(dst, src, sts) \
{ \
      dst.uuid = src.uuid; \
      dst.name = (idl_char *)DTSSmalloc((VOID *)strlen(src.name)+1); \
      sts = error_status_ok; \
      (void) strcpy ((char *)dst.name, (char *)src.name); \
}

/*
 * Table to map dpe_acl errors to dts/dce errors
 */

#define XLATE_ACL_FILE_ERROR(status) acl_file_error[(status)]


/*
 * DTS dacl usage
 *
 * One goal of DTS is to prevent malicious manipulation of the time in a
 * DCE environment. Since the commands in the DTS management interface can have
 * a significant effect on the time in a DCE environment, we must allow only
 * authorized people to use this interface.  To achieve the desired protection,
 * we apply a DCE acl (dacl) to the time service object and check the
 * authorization of each client that calls the DTS management interface against
 * this dacl.
 *
 * For authorization purposes, we distinguish two types of calls in
 * the DTS management interface:  read and write.  Read commands are those
 * such as SHOW TIME and SHOW ALL CHARACTERISTICS that only provide
 * information and have no potential to disrupt the time in a DCE environment.
 *
 * Write commands are those such as SET and CHANGE EPOCH that manipulate DTS
 * parameters and therefore could be used to disrupt time in the DCE
 * environment.
 *
 * DTS must provide an RPC interface to its dacl management routines to allow
 * access by the dacl management tools (e.g acl_edit).  This interface must
 * be protected so that only authorized users are allowed to change the DTS
 * dacl.  This routine is the rdacl interface and is described in the DCE
 * security documentation.
 *
 * To implement this protection policy, we map the three DTS functions above to
 * the standard dacl permissions as follows:
 *
 * DTS function		dacl permission	(bit name)
 * ------------		--------------------------
 *
 * read-only mgmt	read (r)
 * calls
 *
 * write mgmt calls	write (w)
 *
 * change DTS dacl	control (c)
 *
 */


/*
 * mgt_acl - DTS management interface acl
 *
 * This is a pointer to the acl protecting the DTS management interface.
 * When a management rpc call is made, the management server routines check the
 * caller's access against this acl by calling dpe_acl_client_is_authorized.
 * We keep it in memory for the life of the DTS daemon so this check is fast.
 * We use it basically as a write-through cache.  We read the acl from
 * permanent storage when DTS starts and store it here.  When someone calls
 * rdacl_replace to replace it, it is replaced here, then replaced in
 * permanent storage.
 *
 * The mutex protects the acl, since one thread may be in the midst of
 * reading it when an acl editor makes an rdacl_replace call to replace it.
 */

static struct
{
    volatile 	sec_acl_p_t 	acl_p;
    pthread_mutex_t    		mutex;
} mgt_acl;

/*
 * Acl manager object uuid
 */

static uuid_t	mgt_acl_mgr_type_uuid;

/*
 * interface to backing storage for this ACL 
 */

static dce_db_handle_t acl_db ;

/*
 * ACL print string and help string id table
 */

sec_acl_printstring_t dtsd_acl_help =  
{
    "dts", "Distributed Time Service server or clerk", (sec_acl_permset_t)0
};



sec_acl_printstring_t dtsd_printstrings[] = 
{
    { "r", "read", sec_acl_perm_read },
    { "w", "write", sec_acl_perm_write},
    { "c", "control", sec_acl_perm_control }
};
	
extern dce_svc_handle_t 	dts_svc_handle ;


/*
 * Internal routines
 */

static void create_default_acl (sec_acl_p_t *, error_status_t *);

error_status_t mgt_acl_init (void);

void mgt_unregister_acl_mgr_if ( rpc_binding_vector_t * , uuid_vector_t * ,
	error_status_t * );

void mgt_unexport_acl_mgr_if ( unsigned_char_t *, uuid_vector_t *,
			error_status_t *);

void mgt_register_acl_mgr_if ( rpc_binding_vector_t *, uuid_vector_t *,
     error_status_t *);


void mgt_export_acl_mgr_if ( unsigned_char_t *, rpc_binding_vector_t *,
			    uuid_vector_t *, error_status_t *);

static void dts_acl_resolve( handle_t, 
      sec_acl_component_name_t, sec_acl_type_t, uuid_t *, boolean32,
      void * , uuid_t *, error_status_t *) ; 
					      
boolean32 mgt_test_read_access ( rpc_binding_handle_t, StatusBlk_t *);

boolean32 mgt_test_write_access ( rpc_binding_handle_t, StatusBlk_t *);

void dpe_acl_storage_read( const char *, sec_acl_p_t *, error_status_t *) ;

/*
**++
**
**  ROUTINE NAME:	mgt_acl_init ()
**
**  SCOPE:		PRIVATE - declared in dtss_service.h
**
**  DESCRIPTION:
**
**	Initializes the DTS management acl.  This is called when DTS starts by
**	InitCPinterface.
**
**	Initialize the mutex protecting the acl.  Attempt to read the acl
**	from permanent storage.  If no acl is present, initialize the acl to
**	the default acl and write it to permanent storage.
**
**  INPUTS:		none
**
**  INPUTS/OUTPUTS:	none
**
**  OUTPUTS:		none
**
**  IMPLICIT INPUTS:
**
**	The permanent storage for the acl.
**
**  IMPLICIT OUTPUTS:
**
**	mgt_acl.acl_p	gets initialized, either with default values or from
**			the value in permanent storage.
**
**	mgt_acl.mutex 	gets initialized.
**
**  FUNCTION VALUE:
**
**	status 		is error_status_ok if we succeed.  Otherwise it contains
**			an error from the operation that failed.
**  SIDE EFFECTS:
**
**	Permanent storage is established for the initialized acl.  A file is
**	created to hold it if none existed.
**
**--
**/
error_status_t mgt_acl_init (void)
{
    char		mgt_acl_filename[ K_MAX_FILENAME + 1 ];
    char 		new_mgt_acl_filename[ K_MAX_FILENAME + 1 ];
    error_status_t	status, file_sts;
    unsigned32 	        flags = 0 ;
    static sec_acl_t    theAcl ;
    sec_acl_p_t		temp_acl_p = &theAcl ;    
    boolean32           must_create_default_acl = false ;
    boolean32		must_store_acl = false ;
    boolean32 		have_old_acl_file = false ;
    char * 		new_acl_filename_p = 0 ;
    char * 		lastchp = 0 ;

    /* begin */

    status = error_status_ok;

    /* assign the old 1.0.x ACL file name */
    (void) strncpy(mgt_acl_filename, dcelocal_path, K_MAX_FILENAME);
    (void) strncat(mgt_acl_filename, K_MGT_ACL_FILE, K_MAX_FILENAME);
    (void) strcpy( new_mgt_acl_filename, mgt_acl_filename) ;
    (void) strncat( new_mgt_acl_filename, ".bak", K_MAX_FILENAME) ;
#ifndef HPDCE_FIX_CHFts16224    
    if ((new_acl_filename_p = dce_strdup( new_mgt_acl_filename )) == NULL)
        return(dts_s_alloc_virt); /* Memory allocation failure -- dts.sams */
#else
    new_acl_filename_p = dce_strdup( new_mgt_acl_filename ) ;
#endif
    if((lastchp = strrchr( new_acl_filename_p,  '/')) != 0)
	lastchp++;
    strcpy( lastchp, K_V1_MGT_ACL_FILE) ;

    /*
     * Initialize the acl mutex
     */
    dtss_mutex_init (&mgt_acl.mutex);

    /*
     * Get the acl manager uuid - this is just the time service object uuid
     */

    mgt_acl_mgr_type_uuid = *rpcBufPtr->objUUIDvector.uuid[0];

    flags = db_c_index_by_uuid ;
    

    /* enable stub management memory allocation (for backing store code) */

    rpc_sm_enable_allocate(&status);
    if (status != error_status_ok)
    {
	dce_error_string_t tempstring;
	dce_svc_printf( DTS_S_RPC_SM_ENABLE_MSG,
		       dce_dts_error_text( status, tempstring )) ;
#ifndef HPDCE_FIX_CHFts16224
	    free(new_acl_filename_p);
#endif
	return status ;
    }

    /* see if new file exists */

    if ( access( new_acl_filename_p, R_OK) == 0) /* File Exists */
    {

	/*
	 * open the dtsd acl database - assume that it exists.
	 * we already know that it exists, so failure must be fatal
	 */
     
#ifdef DCE_DEBUG 
	DCE_SVC_DEBUG(( dts_svc_handle, dts_s_msgs, svc_c_debug1, 
		       "Using V1.1 ACL file - %s\n", new_acl_filename_p));
#endif 

	dce_db_open( new_acl_filename_p, (const char *)0, flags, 
		    (dce_db_convert_func_t)dce_rdacl_convert,  
		    &acl_db, &status) ;
	if (status != error_status_ok) 
	{
	    dce_error_string_t tempstring;
	    dce_svc_printf( DTS_S_ACL_DBOPEN_READ_MSG, 
			   dce_dts_error_text( status, tempstring));
#ifndef HPDCE_FIX_CHFts16224
	    free(new_acl_filename_p);
#endif
	    return status ;
	}

	temp_acl_p = &theAcl ;

	/* try and fetch from database */
       
	dce_db_fetch_by_uuid( acl_db, &mgt_acl_mgr_type_uuid,
			     temp_acl_p, &status) ;

	if (status != error_status_ok) /* can't get it  */
	{
	    dce_error_string_t tempstring;
	    dce_svc_printf( DTS_S_FETCH_UUID_MSG, 
			   dce_dts_error_text( status, tempstring));
	    must_create_default_acl = true ;
	}
    }
    else			/* new file doesn't exist */
    {
        must_store_acl = true ;
        must_create_default_acl = true ;

        /* See if if the v1.0.x ACL exists */
        if (have_old_acl_file = (access ( mgt_acl_filename, R_OK) == 0)) 
        {
            /* try and read it */
#ifdef DCE_DEBUG 
	    DCE_SVC_DEBUG(( dts_svc_handle, dts_s_msgs, svc_c_debug1, 
	        "Converting old V1.0.x ACL file - %s\n", mgt_acl_filename));
#endif 

	    dpe_acl_storage_read( mgt_acl_filename, &temp_acl_p,
                                  &file_sts);
#define DTS_ACL_OK 0 

	    if (file_sts == DTS_ACL_OK)	/* able to read old acl */
	    {
	        must_create_default_acl = false ;
	    }
        }

	/* regardless, always create new database */
	
	flags |= db_c_create ; /* create it  */
	
	dce_db_open( new_acl_filename_p, (const char *)0, flags, 
		    (dce_db_convert_func_t)dce_rdacl_convert,  
		    &acl_db, &status) ;

	if (status != error_status_ok)
	{
	    dce_error_string_t tempstring;
	    dce_svc_printf( DTS_S_ACL_DBOPEN_CREATE_MSG, 
			   dce_dts_error_text( status, tempstring)) ;
	    (void)remove( new_acl_filename_p) ;
#ifndef HPDCE_FIX_CHFts16224
	    free(new_acl_filename_p);
#endif
	    return status ;
	}
    }

    {
	/*
	 * Always register object types for this database 
	 * N.B. we also register a private resolver function 
	 * to map input parms to a uuid 
	 */

	unsigned32 print_str_size = sizeof dtsd_printstrings / 
	    sizeof dtsd_printstrings[0] ;
	
	dce_acl_register_object_type( acl_db, 
				     &mgt_acl_mgr_type_uuid, 
				     print_str_size, 
				     dtsd_printstrings,
				     &dtsd_acl_help, 
				     sec_acl_perm_control, 0, 
				     dts_acl_resolve, (void *)NULL, 0, 
				     &status);
	if (status != error_status_ok)
	{
	    dce_error_string_t tempstring;
	    dce_svc_printf( DTS_S_ACL_REGISTER_OBJ_TYPES_MSG, 
			   dce_dts_error_text(status, tempstring));
#ifndef HPDCE_FIX_CHFts16224
	    free(new_acl_filename_p);
#endif
	    return status ;
	}
    }

    if ( must_create_default_acl) /* either corrupt or non-existent */
    {
	create_default_acl( &temp_acl_p, &status) ;
	if( status != error_status_ok)
	{
	    dce_error_string_t tempstring;	
	    dce_svc_printf( DTS_S_ACL_CREATE_DEFAULT_MSG, 
			   dce_dts_error_text(status, tempstring));
	    
#ifndef HPDCE_FIX_CHFts16224
	    free(new_acl_filename_p);
#endif
	    return status ;
	}
	must_store_acl = true ;
    }



    /*
     * N.B. temp_acl_p will either be the default ACLs as created 
     * by the call to create_default_acl() above, OR it will be the 
     * values retrieved from the old V1.0.x ACL 
     */
    if (must_store_acl) 
    {
	dce_db_store_by_uuid( acl_db, &mgt_acl_mgr_type_uuid,
			     temp_acl_p, &status);
	if (status != error_status_ok)
	{
	    dce_error_string_t tempstring;
	    dce_svc_printf( DTS_S_DB_STORE_UUID_MSG, 
			   dce_dts_error_text(status, tempstring));
#ifndef HPDCE_FIX_CHFts16224
	    free(new_acl_filename_p);
#endif
	    return status ;
	}
    }

    mgt_acl.acl_p = (volatile sec_acl_p_t)temp_acl_p;

    if (have_old_acl_file) 
	(void)rename( mgt_acl_filename, new_mgt_acl_filename) ;
#ifndef HPDCE_FIX_CHFts16224    
    free(new_acl_filename_p);
#endif
    return (status);

} /* end of routine mgt_acl_init */

/*
**++
**
**  ROUTINE NAME:	mgt_register_acl_mgr_if ()
**
**  SCOPE:		PRIVATE - declared in dtss_service.h
**
**  DESCRIPTION:
**
**	Register the rdacl interface to the DTS management acl manager.  This
**	is called when DTS starts by CreateRPClisten.  This routine assumes
**	that mgt_acl_init has already been executed.
**
**	In order to distinguish the DTS rdacl interface from other applications
**	that may register this interface, we must use an object uuid.  This
**	object uuid will be the Time Service interface uuid.  After calling
**	this routine, all that's left to use the rdacl interface is to export
**	bindings to the name service, which will be done when DTS is enabled.
**
**  INPUTS:
**
**	bindings	is a binding vector containing all the bindings to use
**			with the interface - one per protocol sequence.
**
**	object_uuids	is a vector containing the time service object uuid.
**			This should be the same as the time service interface
**			uuid.
**
**  INPUTS/OUTPUTS:	none
**
**  OUTPUTS:
**
**	status		is error_status_ok if we succeed.  Otherwise it will be
**			the status of the rpc library call that failed.
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
void mgt_unregister_acl_mgr_if (
   rpc_binding_vector_t * bindings,
   uuid_vector_t * object_uuids,
   error_status_t * status
)
{
    rpc_ep_unregister (rdaclif_v1_0_s_ifspec,
		       bindings,
		       object_uuids,
		       status);
    if (*status != rpc_s_ok)
    {
	dce_error_string_t tempstring;
	dce_svc_printf( DTS_S_ACLV1_UNREGISTER_MSG, 
		       dce_dts_error_text(*status, tempstring));

	return;
    }

    rpc_ep_unregister (rdaclif_v0_0_s_ifspec,
		       bindings,
		       object_uuids,
		       status);
    if (*status != rpc_s_ok)
    {
	dce_error_string_t tempstring;
	dce_svc_printf( DTS_S_ACLV0_UNREGISTER_MSG, 
			   dce_dts_error_text(*status, tempstring));

	return;
    }

    return ;
}

extern rdaclif_v1_0_epv_t dce_acl_v1_0_epv ;
extern rdaclif_v0_0_epv_t  dce_acl_v0_0_epv ;

void mgt_register_acl_mgr_if (
    rpc_binding_vector_t	*bindings,
    uuid_vector_t		*object_uuids,
    error_status_t		*status
)
{

    /*
     * Register the rdacl interface
     */
    rpc_server_register_if (rdaclif_v1_0_s_ifspec,
			    (uuid_t *)NULL,
			    (rpc_mgr_epv_t)&dce_acl_v1_0_epv,
			    status);
    if (*status != rpc_s_ok)
    {
	dce_error_string_t tempstring;
	dce_svc_printf( DTS_S_ACLV1_RUNTIME_REGISTER_MSG, 
		       dce_dts_error_text(*status, tempstring));
	
	return;
    }

    /*
     * Register the rdacl endpoint
     */

    rpc_ep_register (rdaclif_v1_0_s_ifspec,
		     bindings,
		     object_uuids,
		     (unsigned_char_p_t)"Time Service ACL Mgr",
		     status);
    if (*status != rpc_s_ok)
    {
	dce_error_string_t tempstring;
	dce_svc_printf( DTS_S_ACLV1_REGISTER_MSG, 
			   dce_dts_error_text(*status, tempstring));

	return;
    }

    /*
     * Register the rdacl interface
     */
    rpc_server_register_if (rdaclif_v0_0_s_ifspec,
			    (uuid_t *)NULL,
			    (rpc_mgr_epv_t)&dce_acl_v0_0_epv,
			    status);
    if (*status != rpc_s_ok)
    {
	dce_error_string_t tempstring;
	    dce_svc_printf( DTS_S_ACLV0_RUNTIME_REGISTER_MSG,
			   dce_dts_error_text(*status, tempstring));
	
	return;
    }

    /*
     * Register the rdacl endpoint
     */

    rpc_ep_register (rdaclif_v0_0_s_ifspec,
		     bindings,
		     object_uuids,
		     (unsigned_char_p_t)"Time Service ACL Mgr",
		     status);
    if (*status != rpc_s_ok)
    {
	dce_error_string_t tempstring;
	dce_svc_printf( DTS_S_ACLV0_REGISTER_MSG,
			   dce_dts_error_text(*status, tempstring));

	return;
    }
    
    return ;
    
} /* end of routine mgt_register_acl_mgr_if */
/*
**++
**
**  ROUTINE NAME:	mgt_export_acl_mgr_if ()
**
**  SCOPE:		PRIVATE - declared in dtss_service.h
**
**  DESCRIPTION:
**
**	Export the DTS mangement acl manager (rdacl) interface to the.
**	service.  This is called when DTS is enabled.  This routine assumes
**	that mgt_register_acl_mgr_if has already been executed.
**
**  INPUTS:
**
**	entry_name	is the name service name of the entry to which we're
**			exporting bindings for this interface.  This should be
**			the time service entry name.
**
**	bindings	is a binding vector containing all the bindings to use
**			with the interface - one per protocol sequence.
**
**	object_uuids	is a vector containing the time service object uuid.
**			This should be the same as the time service interface
**			uuid.
**
**
**  INPUTS/OUTPUTS:	none
**
**  OUTPUTS:
**
**	status		is error_status_ok if we succeed.  Otherwise it will be
**			the status of the rpc library call that failed.
**
**  IMPLICIT INPUTS:
**
**  IMPLICIT OUTPUTS:
**
**  FUNCTION VALUE:	none
**
**  SIDE EFFECTS:
**
**--
**/

void mgt_export_acl_mgr_if (
    unsigned_char_t		*entry_name,
     rpc_binding_vector_t	*bindings,
     uuid_vector_t		*object_uuids,
     error_status_t		*status
)
{

    /*
     * Export the rdacl bindings to the name service
     */

    *status = rpc_s_ok ;

    return ;

} /* end of routine mgt_export_acl_mgr_if */

void mgt_unexport_acl_mgr_if (
    unsigned_char_t		*entry_name,
    uuid_vector_t		*object_uuids,
    error_status_t		*status
)
{

    /*
     * Export the rdacl bindings to the name service
     */
    *status = rpc_s_ok ;

    return ;
} /* end of routine mgt_export_acl_mgr_if */
/*
**++
**
**  ROUTINE NAME:	mgt_test_read_access ()
**
**  SCOPE:		PRIVATE - declared in dtss_service.h
**
**  DESCRIPTION:
**
**	Checks for read access to the DTS management interface.  This is called
**	by control program RPC server routines that read DTS management
**	parameters.
**
**  INPUTS:
**
**	client_handle	is the RPC binding handle for the caller of the
**			management interface.
**
**  INPUTS/OUTPUTS:	none
**
**  OUTPUTS:
**
*	status_blk      is the DTS status block representing any error that
**			occurs.
**
**  IMPLICIT INPUTS:
**
**	mgt_acl.acl_p	is the DTS management interface acl.
**
**  IMPLICIT OUTPUTS:	none
**
**  FUNCTION VALUE:
**
**	Returns TRUE if read access is granted and FALSE otherwise.
**
**  SIDE EFFECTS:	none
**
**--
**/

boolean32 mgt_test_read_access (
    rpc_binding_handle_t	client_handle,
    StatusBlk_t		*status_blk
)
{
    boolean32 result = FALSE ;
    error_status_t st = error_status_ok ;
    boolean32 authorized = FALSE ;

    dce_acl_is_client_authorized( (handle_t)client_handle, 
				 &mgt_acl_mgr_type_uuid,
				 &mgt_acl_mgr_type_uuid, 
				 (uuid_t *)0,
				 (uuid_t *)0,
				 (sec_acl_permset_t)sec_acl_perm_read,
				 &authorized, 
				 &st) ;
    if (st == error_status_ok && authorized) 
    {
	status_blk->status = DTSS_SUCCESS;
	result = TRUE ;
    }
    else
    {
	result = FALSE ;
	status_blk->status = K_DCE_ERROR;
 	status_blk -> sysStatus = (st == error_status_ok) ? 
	    dts_s_mgt_no_read : st ;
    }

    return(result);
} /* end of routine mgt_test_read_access */

/*
**++
**
**  ROUTINE NAME:	mgt_test_write_access ()
**
**  SCOPE:		PRIVATE - declared in dtss_service.h
**
**  DESCRIPTION:
**
**	Checks for write access to the DTS management interface.  This is called
**	by control program RPC server routines that perform any DTS management
**	function other than simply reading management parameters
**
**  INPUTS:
**
**	client_handle	is the RPC binding handle for the caller of the
**			management interface.
**
**  INPUTS/OUTPUTS:	none
**
**  OUTPUTS:
**
**	status_blk      is the DTS status block representing any error that
**			occurs.
**
**  IMPLICIT INPUTS:
**
**	mgt_acl.acl_p	is the DTS management interface acl.
**
**  IMPLICIT OUTPUTS:	none
**
**  FUNCTION VALUE:
**
**	Returns TRUE if write access is granted and FALSE otherwise.
**
**  SIDE EFFECTS:	none
**
**--
**/
boolean32 mgt_test_write_access (
    rpc_binding_handle_t	client_handle,
     StatusBlk_t		*status_blk
)
{
    boolean32 result = FALSE ;
    boolean32 authorized = FALSE ;
    error_status_t st = error_status_ok ;

    dce_acl_is_client_authorized((handle_t) client_handle, 
				 &mgt_acl_mgr_type_uuid, 
				 &mgt_acl_mgr_type_uuid, 
				 (uuid_t *)0,
				 (uuid_t *)0,
				 (sec_acl_permset_t) sec_acl_perm_write,
				 &authorized,
				 &st) ;
    if (st == error_status_ok && authorized) 
    {
	status_blk->status = DTSS_SUCCESS;
	result = TRUE ;
    }
    else
    {
	result = FALSE ;
	status_blk->status = K_DCE_ERROR;
	status_blk->sysStatus = 
	    ( st == error_status_ok) ? dts_s_mgt_no_write : st ;
    }

    return(result);
} /* end of routine mgt_test_write_access */


/*
 * N.B for DCE 1.1, we remove all local definitions of the rdacl_XXX
 * routines, and depend on those defined in the ACL manager library 
 */

/*
**++
**
** ROUTINE NAME:	create_default_acl
**
**	Create a default acl for the dts management interface that has the
**	following entries in it:
**
**		(<local-machine-principal>	cwr)
**		(TSadmin			cwr)
**		(ANY_OTHER			  r)
**		(UNAUTHENTICATED		  r)
**
**--
*/

static void create_default_acl (
    sec_acl_t		**def_acl_p,
     error_status_t	*statusp
)
{
    sec_acl_t		*acl_p = 0;
    sec_rgy_handle_t 	rgy_context;
    char		*dts_admin_group_name = DEF_TSERV_ADMIN_GROUP_NAME;
    uuid_t		dts_admin_group_uuid;
    error_status_t	sts = error_status_ok ;
    sec_acl_permset_t   permset = (sec_acl_permset_t) 0;

    /* begin */
    /*
     * create the ACL and initialize based on this manager 
     */
    
    acl_p = *def_acl_p = DTSSmallocStruct( sec_acl_t ) ;
    
    if ( dce_acl_obj_init( & mgt_acl_mgr_type_uuid, acl_p, statusp),
           *statusp != error_status_ok) 
	return;

    /*
     * allow "cwr" access to the host machine's principal 
     */

    permset  = sec_acl_perm_control | sec_acl_perm_read | sec_acl_perm_write;


    if ( dce_acl_obj_add_user_entry ( acl_p, permset, 
				&rpcBufPtr->host_principal.uuid , statusp),
				*statusp != error_status_ok ) 
	return ;

    /*
     * Allow "cwr" access to the default time service adminstrator's group.  If
     * this group doesn't exist, just omit this acl entry.
     */

    if (sec_rgy_site_open (rpcBufPtr->my_cell.name, &rgy_context, statusp),
	*statusp == error_status_ok) 
    {
	if ( sec_rgy_pgo_name_to_id (rgy_context,
				sec_rgy_domain_group,
				(unsigned char *)dts_admin_group_name,
				&dts_admin_group_uuid,
				statusp), *statusp == error_status_ok) 
	{
	    permset = sec_acl_perm_control | 
			sec_acl_perm_read | 
			    sec_acl_perm_write ;
	    
	    if ( dce_acl_obj_add_group_entry( acl_p, permset, 
					&dts_admin_group_uuid, statusp),
		*statusp != error_status_ok)
		return ;
	    
	}
	else 
	    return ;		/* sec_rgy_pgo_name_to_id failed... */
	
	sec_rgy_site_close (rgy_context, &sts);
    }
    else 
	return ;		/* sec_rgy_site_open failed... */

    /*
     * "r" access to any other principals 
     */

    permset = sec_acl_perm_read ;
    if ( dce_acl_obj_add_obj_entry( acl_p, sec_acl_e_type_any_other,
			      permset, statusp), *statusp != error_status_ok)
	return ;

    /*
     * "r" access to unauthenticated users 
     */

    permset = sec_acl_perm_read ;

    if ( dce_acl_obj_add_unauth_entry ( acl_p, permset, statusp), 
	*statusp != error_status_ok)
	return ;

    return ;
}

/*
 * dts_acl_resolve - 
 * resolver function for the new DCE 1.1 ACL library. 
 * see documenation for the dce_acl_resolve_by_{name,uuid}
 * for the interface to this function 
 * 
 * in our (simple, degenerate?) case, we have no database of objects 
 * to protect via our acls. Out acl file has one ACL in it, which 
 * the dtsd uses to protect access to its own data.
 * We store out ACL under the uuid of the time service manager (ourselvs).
 * Hence, our manager_type uuid and our "key" uuid are one and the same.
 */

static void dts_acl_resolve ( 
    handle_t handle,		/* [in] parameters  */
    sec_acl_component_name_t component_name,
    sec_acl_type_t sec_acl_type,
    uuid_t * mgr_type_p,
    boolean32 writing,
    void * resolver_arg_p,
    uuid_t * acl_uuid_p,	/* [out] parameters */
    error_status_t * stp
)
{
    *acl_uuid_p = mgt_acl_mgr_type_uuid ;
    *stp = error_status_ok ;
    return ;
}

boolean32 dts_dtsd_auth_function( 
	rpc_binding_handle_t client_handle,
	unsigned32 mgmt_opt,
	error_status_t * stp)
{
    boolean32 authorized = TRUE ;
    sec_acl_permset_t desired_perms = sec_acl_perm_write ;
    uuid_t mgt_acl_mgr_type_uuid ;
    uuid_t acl_uuid ;

    /* begin */

    *stp = rpc_s_ok ;
    mgt_acl_mgr_type_uuid = *rpcBufPtr->objUUIDvector.uuid[0];    
    acl_uuid = mgt_acl_mgr_type_uuid ;

    switch( (int)mgmt_opt) 
    {
	default:
	    return authorized ;

	case rpc_c_mgmt_stop_server_listen:

	    dce_acl_is_client_authorized ( 
		 client_handle,
		 &mgt_acl_mgr_type_uuid, 
		 &acl_uuid, /* the acl uuid - same for us */
		 (uuid_t *)0, /* owner id */
		 (uuid_t *)0, /* our group uuid */
		 desired_perms, /* permission */
		 /* Output Parameters */
		 &authorized, 
		 stp
		 );	
	    break;
    }
    return authorized;
}

#endif /* CONFIG_SECURITY */

