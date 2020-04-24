// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: static.cxx,v $
// Revision 1.1.2.2  1996/03/09  20:49:19  marty
// 	Add OSF copyright
// 	[1996/03/09  20:27:44  marty]
//
// Revision 1.1.2.1  1995/12/11  19:34:52  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:18:22  root]
// 
// $EndLog$
#include "test_com.h"
#include "matrix.h"

void
Matrix::staticFunction(
	idl_long_int x,
	idl_long_int y,
	idl_long_int *z
) {
	*z = x + y;
	return;
}

