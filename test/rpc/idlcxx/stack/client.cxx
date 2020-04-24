// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: client.cxx,v $
// Revision 1.1.4.2  1996/09/10  18:21:13  bfc
// 	remove unecessary STDC definition
// 	[1996/09/10  18:16:33  bfc]
//
// Revision 1.1.4.1  1996/09/09  20:59:24  bfc
// 	finish TETizing and cleanup
// 	[1996/09/09  20:43:24  bfc]
// 
// Revision 1.1.2.2  1996/03/09  20:49:54  marty
// 	Add OSF copyright
// 	[1996/03/09  20:28:12  marty]
// 
// Revision 1.1.2.1  1995/12/11  19:35:49  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:18:48  root]
// 
// $EndLog$
// This test demonstrates how to pass native C++ objects as parameters
// in RPC calls. The test deals with a Stack object, where the Stack
// class might be defined in a C++ class library or be a user-defined
// C++ class. The test illustrates how a object reference can be passed
// without extending the class definition and also without passing all 
// the data back and forth everytime an RPC call is made. The stack is
// maintained in the client's address space and a reference is passed
// as an RPC argument. The server then simply makes callbacks to the stack 
// object.

#ifdef TET
#define TET_ENV
#define main loc_main
#endif

#include "test_com.h"

#ifdef _WIN32
#include <strstrea.h>
#else
#include <strstream.h>
#endif

#include <ctype.h>


#include "icalculate.h"		// IDL generated header file
#include "istack_mgr.h"		// IDL generated delegate class

#define MAXOP 100
#define NUMBER '0'

// Function declarations
static int getch();
static int getop(char *);
void ungetch(int);

// could use standard input but its easier to
// hard code the input for testing purposes
char *input =
	"1 2 - 4 5 + * \n \
	63 9 / \n \
	34 128 + 3 / 6 129 * * \n \
	14.8 3.6 / \n";
istrstream ist(input, strlen(input));

int 
main(int argc, char *argv[])
{
	Stack		s;		// local Stack object
	IStack 		*iStack;	// reference to the local Stack object
	ICalculate	*iCalc;		// calculator object on remote server
	int		type;		// type of element on stack
	char		str[MAXOP];	// buffer of input information

	//
	// Test passing a Stack object as an RPC argument
	//

	// Create an interface pointer to the local Stack object
	iStack = new IStackMgr(&s);

	// Create a reverse Polish notation calculator object on the server
	iCalc = ICalculate::newCalc(iStack);


	// process the input
	while ((type = getop(str)) != EOF) {
		switch (type) {
		case NUMBER:
			s.push(atof(str));	// local operation
			break;
		case '+':
			iCalc->plus();		// remote operation
			break;
		case '*':
			iCalc->multiply();	// remote operation
			break;
		case '-':
			iCalc->minus();		// remote operation
			break;
		case '/':
			iCalc->divide();	// remote operation
			break;
		case '\n':
			cout << "answer: " << s.pop() << endl;
			break;
		default:
			cout << "error: unknown command" << endl;
			break;
		}
	}

	cout << "All tests passed" << endl;
	return 0;
}

// get next operator from input buffer
int
getop(
	char *s				// input buffer
) {
	int i, c;

	// skip whitespace
	while ((s[0] =  c = getch()) == ' ' || c == '\t');

	s[1] = '\0';
	if (!isdigit(c) && c != '.') return c;
	i = 0;
	if (isdigit(c))
		while(isdigit(s[++i] = c = getch()));
	
	if (c == '.')
		while(isdigit(s[++i] = c = getch()));

	s[i] = '\0';

	if (c != EOF) ungetch(c);

	return NUMBER;
}

const int BUFSIZE=100;
char buf[BUFSIZE];
int bufp = 0;

// read next input character
int
getch()
{
	char c;

	if (bufp > 0)
		return buf[--bufp];
	ist.get(c);
	if (ist.eof()) return EOF;
	return c;
}

void
ungetch(int c)
{
	if (bufp >= BUFSIZE)
		cout << "ungetch: too many characters\n" << endl;
	else
		buf[bufp++] = c;
}
