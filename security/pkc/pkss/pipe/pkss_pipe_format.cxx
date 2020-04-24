//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// All Rights Reserved

//
// HISTORY
// $Log: pkss_pipe_format.cxx,v $
// Revision 1.1.2.1  1996/10/04  14:20:05  arvind
// 	OSF DCE 1.2.2 Drop 4
// 	[1996/10/04  13:30:48  arvind]
//
// Revision /main/DCE_1.2.2/1  1996/09/09  21:28 UTC  arvind
// 	PKSS drop from DEC (DCE1.2.2)
// 	[1996/08/30  16:02 UTC  arvind  /main/arvind_pkss/1]
// 
// $EndLog$

//
// (c) Copyright 1996,
// Digital Equipment Corporation, Maynard, Massachusetts, USA
// All Rights Reserved
//
//

#include "pkss_pipe_format.h"
#include <unistd.h>


size_t read_item(int fd, void * buf, size_t nbytes) {
    int total = 0;
    int done = 0;
    int res;
    unsigned char * lbuf = (unsigned char *)buf;
    while (nbytes > 0) {
        res = read(fd, lbuf, nbytes);
        if (res < 0) return res;
        total += res;
        lbuf += res;
        nbytes -= res;
    };
    return total;
}

size_t write_item(int fd, const void * buf, size_t nbytes) {
    return write(fd, buf, nbytes);
}
