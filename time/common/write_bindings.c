/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: write_bindings.c,v $
 * Revision 1.1.5.2  1996/02/17  23:34:41  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:49  marty]
 *
 * Revision 1.1.5.1  1995/12/08  18:08:48  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/1  1995/09/15  14:21 UTC  pare
 * 	Create HPDCE02 branch and merge changes.
 * 
 * 	HP revision /main/pare_MOTHRA/1  1995/09/15  14:18 UTC  pare
 * 	Fix memory leak, free binding -- CHFts16224
 * 	[1995/12/08  17:24:11  root]
 * 
 * Revision 1.1.2.6  1994/08/23  20:32:07  cbrooks
 * 	Code Cleanup - ANSI switches
 * 	[1994/08/23  15:26:19  cbrooks]
 * 
 * Revision 1.1.2.5  1994/07/29  18:27:13  cbrooks
 * 	CR11466 - new functions for read/write bindings file
 * 	[1994/07/29  18:25:09  cbrooks]
 * 
 * Revision 1.1.2.4  1994/06/29  17:57:10  cbrooks
 * 	CR10802 - bad error message
 * 	[1994/06/29  14:08:56  cbrooks]
 * 
 * Revision 1.1.2.3  1994/06/10  20:51:53  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:48:44  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/31  21:29:12  cbrooks
 * 	CR 10611 - DTS needs new ACL manager
 * 	[1994/05/31  21:03:54  cbrooks]
 * 
 * 	Use K_DTS_HOME_DIR, K_DTS_BINDING_FILE macros
 * 	[1994/05/13  21:02:15  cbrooks]
 * 
 * Revision 1.1.2.1  1994/04/21  22:05:20  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  20:04:29  cbrooks]
 * 
 * $EndLog$
 */

#include <dce/dce.h>
#include <dce/dcedtsmsg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#include <errno.h>

#include <dtssdef_ultrix.h>

/*
 * Externals 
 */

extern void dce_pgm_fprintf( FILE *, error_status_t );

extern void dce_bindings_to_file( FILE *, rpc_binding_vector_p_t, error_status_t *);
extern void dce_bindings_from_file( FILE *, rpc_binding_vector_p_t *, error_status_t *);

char * get_binding_file_name( void ) ;

void dts_write_bindings( error_status_t *);
void dts_read_bindings( rpc_binding_vector_p_t *, error_status_t *);
void dts_remove_bindings_file( void ) ;

static const char EOS = '\0' ;
static const char * const adm_sub_dir = K_DTS_HOME_DIR ;
static const char * const dtsd_binding_file = K_DTS_BINDING_FILE ;

char * get_binding_file_name(void)
{
    static char binding_file_name[ _POSIX_PATH_MAX ] ;

    /* since name is static, it will be 0. Use this to check for init */

    if (binding_file_name[0] == EOS)
    {
	strcpy( binding_file_name, dcelocal_path);
	strcat( binding_file_name, adm_sub_dir) ;
	strcat( binding_file_name, dtsd_binding_file);
    }

    return binding_file_name ;
}


/*
 * Public interface 
 * 
 */

/*
 * WriteBindings() - write bindings to the server binding file 
 * return: 0 if success, non-0 if an error has occurred
 * diagnostic has already been written
 */

void dts_write_bindings(error_status_t * stp)
{
    FILE * fp  = 0 ;
    char * theBindingFile = get_binding_file_name() ;
    rpc_binding_vector_p_t bvp = NULL;
    
   /* begin */

    *stp = error_status_ok ;
    if ( rpc_server_inq_bindings( &bvp, stp), *stp != error_status_ok)
	return ;

   /* (Re)write the bindings file */

    if((fp = fopen( theBindingFile, "w+")) != NULL)
    {
	
        /*
         * Force proper file ownership and access modes
         */

        (void)chown(theBindingFile, 0, 0);
        (void)chmod(theBindingFile, 0644);
	
	dce_bindings_to_file( fp, bvp, stp) ;
        (void)fclose( fp ) ;			     
    }
    else 
    {
	*stp = dts_s_binding_file_write_error ;
    }
#ifndef HPDCE_FIX_CHFts16244
    if (bvp != NULL)
        rpc_binding_vector_free(&bvp, stp);
#endif
    return ;
}

/*
 * ReadBindings( rpc_binding_vector_t **) 
 * read bindings from a the servers local data file 
 * returns: 0 if success, binding vector is filled in.
 * if failure, returns non-zero, and the pointer will be null.
 */

void dts_read_bindings( rpc_binding_vector_p_t * bvpp, error_status_t * stp)
{
    char * theBindingFile = get_binding_file_name();
    FILE * fp = 0; 

    /* begin */

    *bvpp = 0 ;
    if ((fp = fopen(theBindingFile, "r")) != NULL)
    {
	dce_bindings_from_file( fp, bvpp, stp) ;
	(void)fclose(fp);
    }
    else
    {				
	*stp = dts_s_binding_file_read_error ;
    }	
    return ;
}


/*
 * removes the binding file 
 * meant to be called on exit 
 */

void dts_remove_bindings_file( void )
{
    char * theBindingFile = get_binding_file_name() ;
    (void) remove( theBindingFile) ;
    return  ;
}
