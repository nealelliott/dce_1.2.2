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
 * $Log: pkss_config_common.h,v $
 * Revision 1.1.2.1  1996/10/04  14:21:36  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  13:37:48  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:28 UTC  arvind
 * 	PKSS drop (2) from DEC (DCE1.2.2)
 * 	[1996/09/04  14:26 UTC  arvind  /main/arvind_pkss/2]
 * 
 * $EndLog$
 */
/* (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 */


#ifndef PKSS_CONFIG_CONFIG_H_
#define PKSS_CONFIG_CONFIG_H_

#ifdef __cplusplus
extern "C"
{
#endif
#include <dce/nbase.h>
#include <dce/dce.h>
#include <dce/dcepksmsg.h>
#ifdef __cplusplus
}
#endif

/* PKSS configuration routines, common to client and server.		    */


#define PKSS_CONF_FILE "pkss.conf"

/*                                                                          */
/* pkss_config__init							    */
/*									    */
/*                                                                          */
/* Initialize the common portion of the PKSS configuration module.  Read    */
/* the configuration file and set globals accordingly.  This routine	    */
/* protects itself with a once-block and may be called multiple times	    */
/* without complaint.							    */
/*									    */
/* The filename parameter should contain the name of the image (i.e. the    */
/* string contained within argv[0]).  If omitted, the string "pkss" will be */
/* assumed.								    */

unsigned32 pkss_config__init(const char * imagename = NULL);





/*									    */
/*									    */
/* Common pkss_config__XXXX inquiry routines.                               */
/*									    */
/*									    */
/* Each routine returns a pointer to static storage containing the desired  */
/* string.								    */
/*									    */


/*									    */
/* pkss_config__cds__service_profile_name				    */
/*									    */
/* Returns the name of the CDS entry from where the client should start its */
/* rpc_ns_import loop in order to find a PKSS server.			    */
/*									    */

char * pkss_config__cds__service_profile_name(void);


 
/*									    */
/* pkss_config__security__server_group_name				    */
/*									    */
/* Returns the name of the security group to which all PKSS servers should  */
/* belong.  This should be used by the client to verify that the server is  */
/* a validly authorized PKSSD server.					    */

char * pkss_config__security__server_group_name(void);


 
#endif
