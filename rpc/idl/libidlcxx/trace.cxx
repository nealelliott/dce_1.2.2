// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: trace.cxx,v $
// Revision 1.1.2.2  1996/03/09  20:45:22  marty
// 	Add OSF copyright
// 	[1996/03/09  20:24:53  marty]
//
// Revision 1.1.2.1  1995/12/08  00:13:23  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/07  23:55:17  root]
// 
// $EndLog$
//
//  Copyright (c) Digital Equipment Corporation, 1995
//  All Rights Reserved.  Unpublished rights reserved
//  under the copyright laws of the United States.
//  
//  The software contained on this media is proprietary
//  to and embodies the confidential technology of 
//  Digital Equipment Corporation.  Possession, use,
//  duplication or dissemination of the software and
//  media is authorized only pursuant to a valid written
//  license from Digital Equipment Corporation.
//
//  RESTRICTED RIGHTS LEGEND   Use, duplication, or 
//  disclosure by the U.S. Government is subject to
//  restrictions as set forth in Subparagraph (c)(1)(ii)
//  of DFARS 252.227-7013, or in FAR 52.227-19, as
//  applicable.
//

#include <stdlib.h>
#include <dce/trace.hxx>

// This file simply defines the 2 static variables used
// in the Trace class for debugging purposes

#ifdef TRACE
int Trace::On = (int) (getenv("TRACE") != (char *) 0);
int Trace::Level = 0;
#endif

