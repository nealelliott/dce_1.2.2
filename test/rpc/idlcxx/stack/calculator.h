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
 * $Log: calculator.h,v $
 * Revision 1.1.2.2  1996/03/09  20:49:54  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:28:11  marty]
 *
 * Revision 1.1.2.1  1995/12/11  19:35:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:18:47  root]
 * 
 * $EndLog$
 */
#ifndef calculator_included
#define calculator_included

// This file implements the arithmetic operations of a reverse Polish 
// notation calculator. The operations occur on the server but reference
// a Stack object in the client's address space.

#include <iostream.h>
#include "icalculate_mgr.h"

class ICalculator : public virtual ICalculateMgr {

private:
	IStack *stackPtr;

public:
	//
	// Constructor to encapsulate the clients Stack interface pointer.
	//
	ICalculator (IStack *p) : stackPtr(p) {};
  
	//
	// Add the two top members on the stack and push back the result.
	//
	virtual void IDL_STD_STDCALL plus() {
		stackPtr->push(stackPtr->pop() + stackPtr->pop());
	};

	//
	// Subtract the two top members on the stack and push back the result.
	//
	virtual void IDL_STD_STDCALL minus() {
		double op2 = stackPtr->pop();
		stackPtr->push(stackPtr->pop() - op2);
	};

	//
	// Multiply the two top members on the stack and push back the result.
	//
	virtual void IDL_STD_STDCALL multiply() {
		stackPtr->push(stackPtr->pop() * stackPtr->pop());
	};

	//
	// Divide the two top members on the stack and push back the result.
	//
	virtual void IDL_STD_STDCALL divide() {
		double op2 = stackPtr->pop();
		if (op2 != 0)
			stackPtr->push(stackPtr->pop() / op2);
		else
			cout << "error: zero divisor" << endl;
	};
};

#endif
