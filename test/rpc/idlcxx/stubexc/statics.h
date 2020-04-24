/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: statics.h,v $
 * Revision 1.1.2.2  1996/03/09  20:50:22  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:28:30  marty]
 *
 * Revision 1.1.2.1  1995/12/11  19:36:20  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:19:05  root]
 * 
 * $EndLog$
 */
#ifndef statics_i_h
#define statics_i_h
#include <iostream.h>
#include <dce/trace.hxx>
#include "savings.h"
#include "checking.h"

Savings *
openNOW(handle_t h, double amt);

Savings *
openOld(handle_t h, double amt);

idl_long_int
getNumOTentries(handle_t h);

void
remote_local_test(handle_t h, Account *a);

double
balanceFromServer(handle_t h, Account *a);

double
checkBalFromServer(handle_t h, Checking *c);

#endif
