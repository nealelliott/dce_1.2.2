/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: common.h,v $
 * Revision 1.1.4.2  1996/02/18  23:07:52  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:29:57  marty]
 *
 * Revision 1.1.4.1  1995/12/11  22:45:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:21:33  root]
 * 
 * Revision 1.1.2.2  1994/10/06  20:40:08  agd
 * 	expand copyright
 * 	[1994/10/06  14:31:18  agd]
 * 
 * Revision 1.1.2.1  1994/09/19  17:53:40  bhaim
 * 	multidelegate test
 * 	[1994/09/19  17:49:03  bhaim]
 * 
 * $EndLog$
 */
/*
 ***************************************************************************
 * This file contains definitions common between the client and server.
 ***************************************************************************
 */
/*
 * (c) Copyright 1992, 1993 Hewlett-Packard Co.
 */
#include "real-common.h"

/* 
 * The cds entry name is the relative name in the CDS where the server will
 * store its binding information.  The description is stored as an annotated
 * field in the endpoint mapper and the CDS.
 */
#define str_conv_cds_entry	"string-convert"
#define str_conv_description	"string_conv"

/* 
 * These print functions use stdio instead of the trace/log facility.  They
 * turn off the tracing macros by replacing them with standard IO routines.
 */
#define PRINT_FUNC	fprintf
#define PRINT_HANDLE	stdout

/* 
 * Priority value used in the call to rpc_ns_profile_elt_add().  This is a
 * middle-of-the-road value allowing higher or lower priority elements to be
 * added.
 */
#define my_profile_element_priority	2
