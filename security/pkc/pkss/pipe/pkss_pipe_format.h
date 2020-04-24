//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved

// HISTORY
// $Log: pkss_pipe_format.h,v $
// Revision 1.1.2.1  1996/10/04  14:20:18  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/04  13:33:56  arvind]
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:28 UTC  arvind
// 	PKSS drop from DEC (DCE1.2.2)
// 	[1996/08/30  16:03 UTC  arvind  /main/arvind_pkss/1]
// 
// $EndLog$

// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
//

#ifndef PKSS_PIPE_FORMAT_H_
#define PKSS_PIPE_FORMAT_H_

#include <stdlib.h>

//
// A routine to fix-up the broken behavior of read() when it comes
// to record-oriented data transfer.

// Read nbytes of data from a file-descriptor.  Arguments are the same as
// the read() system call, but it only returns when <nbytes> have been read
// or an error occurs.
size_t read_item(int fd, void * buf, size_t nbytes);

// A similar wrapper for write().  While write() doesn't misbehave liek read(),
// a wrapper makes for a pleasant symmetry, and also lets us use C++ 
// type-checking.
size_t write_item(int fd, const void * buf, size_t nbytes);


#endif
