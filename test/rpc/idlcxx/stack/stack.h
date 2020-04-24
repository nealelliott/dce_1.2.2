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
 * $Log: stack.h,v $
 * Revision 1.1.2.2  1996/03/09  20:50:01  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:28:16  marty]
 *
 * Revision 1.1.2.1  1995/12/11  19:35:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:18:52  root]
 * 
 * $EndLog$
 */
#ifndef stack_included
#define stack_included

// This file implements a simple Stack class. A real Stack class from
// a class library would make a better example but this one does the
// job. Its purpose is to illustrate how a C++ object reference can be passed
// as an RPC argument without extending the class definition itself.

#include <iostream.h>

////////////////////////////////////////////////////////////////////////////////
//
// A Stack class definition
//

class Stack {

private:
	double	theStack[100];	// this stack is just an array
	int	top;		// top element index

public:
	// default constructor
	Stack() : top(0) {}

	// destructor
	~Stack() {}

	// pop the top element off the stack
	double pop() { 
		if (top > 0)
			return theStack[--top];
		else {
			cout << "error: stack empty" << endl;
			return 0;
		}
	}

	// push an element onto the stack
	void push(
		double	e	// the element
	) {
		theStack[top++] = e;
	}
};

#endif
