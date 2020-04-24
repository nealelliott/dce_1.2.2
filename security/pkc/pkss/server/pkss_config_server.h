/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: pkss_config_server.h,v $
 * Revision 1.1.2.1  1996/10/04  14:22:11  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  13:41:44  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:28 UTC  arvind
 * 	PKSS drop (2) from DEC (DCE1.2.2)
 * 	[1996/09/04  14:28 UTC  arvind  /main/arvind_pkss/2]
 * 
 * $EndLog$
 */
/* (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 */

#ifndef PKSS_CONFIG_SERVER_H_
#define PKSS_CONFIG_SERVER_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <dce/dce.h>
#ifdef __cplusplus
}
#endif

#include <pkss_config_common.h>

/* Server-specific PKSS initialization routines				    */


/*									    */
/* pkss_config__server_init						    */
/*									    */
/*									    */
/* Initialize the server-side PKSS configuration module.  Read the	    */
/* configuration file and set globals accordingly.  This routine will also  */
/* all pkss_config__init(), so invoking pkss_config__server_init is the     */
/* only config initialization that the server has to do.  This routine      */
/* protects itself with a once-block and may be called multiple times       */
/* without complaint.                                                       */
/*									    */
/* The imagename parameter should contain the name of the image (i.e. the   */
/* string contained within argv[0]).  If omitted, the string "pkssd" will   */
/* be assumed.								    */

unsigned32 pkss_config__server_init(const char * imagename = NULL);




/*									    */
/* server-side pkss_config__XXXX inquiry routines.			    */
/*									    */
/* Each routine will call pkss_config__server_init if necessary, and return */
/* a pointer to static storage containing the desired string.		    */

/*									    */
/* pkss_config__cds__server_bindings_name				    */
/*									    */
/* Returns the name of the CDS entry into which the server should export    */
/* its bindings.							    */

const char * pkss_config__cds__server_bindings_name(void);



/*									    */
/* pkss_config__cds__service_group_name					    */
/*									    */
/* Returns the name of the CDS group to which the server should add its	    */
/* bindings name.							    */

const char * pkss_config__cds__service_group_name(void);


/*									    */
/* pkss_config__security__server_princ_name                                 */
/*									    */
/* Returns the name of the security principal under which the server should */
/* run.									    */

const char * pkss_config__security__server_princ_name(void);



/*									    */
/* pkss_config__security__admin_group_name					    */
/*									    */
/* Returns the name of the security group that contains all PKSS	    */
/* administrators.  This is used when initializing the ACL of a database.   */

const char * pkss_config__security__admin_group_name(void);



/*									    */
/* pkss_config__local__database_name					    */
/*									    */
/* Returns the name of the local database file.				    */
/*									    */

const char * pkss_config__local__database_name(void);


#endif
