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
 * $Log: pkc_common.cxx,v $
 * Revision 1.1.2.1  1996/11/13  20:34:34  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  20:16:46  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/10/18  00:39 UTC  arvind
 * 	CAPI drop from DEC
 * 	[1996/10/16  20:32 UTC  arvind  /main/arvind_capi/1]
 * 
 * $EndLog$
 */
/*
 * @DEC_COPYRIGHT@
 * Copyright (c) 1996
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
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
 *	
 * ENVIRONMENT:	portable
 * 
 * AUTHOR:
 *              John Wray 
 *		Distributed Processing Engineering (DPE)
 * 
 * DATE:	March 1996
 * 
 */



#include <dce/pkc_common.h>

extern "C" {
#include <dce/dcesvcmsg.h>
#include <dce/dcepkcsvc.h>
#include <dce/dcepkcmac.h>
#include <pthread.h>
}


dce_svc_handle_t pkc__svc_handle;

static pthread_once_t pkcOnceBlock = pthread_once_init;

static void pkc_once_routine(void) {
    error_status_t status;
    
    pkc__svc_handle = dce_svc_register(pkc__svc_table,
                                       (idl_char*)"pkc",
   				       &status);

    dce_msg_define_msg_table(pkc__table,
                             sizeof(pkc__table) / sizeof(pkc__table[0]),
			     &status);
}

error_status_t pkc_init_svc_handle(void) {
    pthread_once(&pkcOnceBlock,
                 pkc_once_routine);
    return 0;
}

