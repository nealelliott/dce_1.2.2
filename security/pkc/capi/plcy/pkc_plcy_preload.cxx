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
 * $Log: pkc_plcy_preload.cxx,v $
 * Revision 1.1.2.1  1996/11/13  20:33:15  arvind
 * 	OSF DCE 1.2.2 Drop 5
 * 	[1996/11/13  20:27:00  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/10/18  00:38 UTC  arvind
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
 * FACILITY:    Certification API	
 * 
 * ABSTRACT:    Module for loading pre-defined CAPI policies.	
 * 
 *	
 * ENVIRONMENT:	portable
 * 
 * AUTHOR:
 *              Mahesh B. Girkar, John Wray.
 *		Distributed Processing Engineering (DPE)
 * 
 * DATE:	March 1996
 * 
 */








#include <dce/pkc_plcy_reg.h>
#include "pkc_plcy_int.h"
#include <dce/pkc_plcy_registry.h>
#include <dce/pkc_plcy_hierarchy.h>

pkc_policy_t * pkc_preloaded_policies[] = {
// Pre-defined policy implementations go here
    &pkc_hierarchy,
    &pkc_registry,
    NULL
};
