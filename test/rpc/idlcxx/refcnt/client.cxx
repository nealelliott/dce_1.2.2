// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: client.cxx,v $
// Revision 1.1.4.2  1996/09/10  18:21:10  bfc
// 	remove unecessary STDC definition
// 	[1996/09/10  18:16:17  bfc]
//
// Revision 1.1.4.1  1996/09/09  20:59:17  bfc
// 	finish TETizing and cleanup
// 	[1996/09/09  20:42:53  bfc]
// 
// Revision 1.1.2.2  1996/03/09  20:49:31  marty
// 	Add OSF copyright
// 	[1996/03/09  20:27:53  marty]
// 
// Revision 1.1.2.1  1995/12/11  19:35:14  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:18:31  root]
// 
// $EndLog$

#ifdef TET
#define TET_ENV
#define main loc_main
#endif

#include "test_com.h"

#include <iostream.h>
#include <assert.h>

#include "matrix.h"		// IDL generated header file

inline void print(Matrix *m) {
	cout << "    [" << m->get(0,0) << "," << m->get(0,1) << "]" << endl;
	cout << "    [" << m->get(1,0) << "," << m->get(1,1) << "]" << endl;
}

int
main(int argc, char *argv[])
{
	Matrix         *m1, *m2;
	long		count;

	cout << "creating new remote matrix from client" << endl;
	cout << "calling new2x2" << endl;
	m1 = Matrix::new2x2(1, 2, 3, 4);
	assert(Matrix::getNumOTentries() == 1);
	print(m1);

	cout << " built m1, now printing" << endl;
	print(m1);
	cout << "success!! on dynamic creator" << endl;

	cout << "copying m1 and bumping reference count" << endl;
	m2 = m1;
	assert(Matrix::getNumOTentries() == 1);

	cout << "releasing m1" << endl;
	assert(Matrix::getNumOTentries() == 1);

	cout << "printing m2 after releasing m1" << endl;
	print(m2);

	cout << "releasing m2" << endl;
	delete m2;
	assert(Matrix::getNumOTentries() == 0);

	///////////////////////////////////////////////////////////////////

	cout << "creating another new remote matrix from client" << endl;
	cout << "calling new2x2" << endl;
	m1 = Matrix::new2x2(5, 6, 7, 8);
	assert(Matrix::getNumOTentries() == 1);

	cout << "copying m1 without bumping reference count" << endl;
	m2 = m1;
	assert(Matrix::getNumOTentries() == 1);

	cout << "releasing m1" << endl;
	delete m2;
	assert(Matrix::getNumOTentries() == 0);
	
	///////////////////////////////////////////////////////////////////

	cout << "creating another new remote matrix from client" << endl;
	cout << "calling new2x2" << endl;
	m1 = Matrix::new2x2(9, 10, 11, 12);
	assert(Matrix::getNumOTentries() == 1);

	cout << " built m1, now printing" << endl;
	print(m1);
	cout << "success!! on dynamic creator" << endl;

	delete m1;
	assert(Matrix::getNumOTentries() == 0);

	///////////////////////////////////////////////////////////////////

	cout << "creating another new remote matrix from client" << endl;
	cout << "calling new2x2" << endl;
	m1 = Matrix::new2x2(13, 14, 15, 16);
	assert(Matrix::getNumOTentries() == 1);

	cout << " built m1, now printing" << endl;
	print(m1);
	cout << "success!! on dynamic creator" << endl;

	cout << "inverting m1 to get m2, printing m2" << endl;
	m1->invert(&m2);
	print(m2);
	assert(Matrix::getNumOTentries() == 2);

	cout << "decRef m1" << endl;
	delete m1;
	assert(Matrix::getNumOTentries() == 1);

	cout << "decRef m2" << endl;
	delete m2;
	assert(Matrix::getNumOTentries() == 0);

	///////////////////////////////////////////////////////////////////

	cout << "creating remote matrix from client" << endl;
	cout << "calling new2x2" << endl;
	m1 = Matrix::new2x2(1, 2, 3, 4);
	assert(Matrix::getNumOTentries() == 1);
	print(m1);

	cout << "creating remote matrix from client for m2" << endl;
	cout << "calling new2x2" << endl;
	m2 = Matrix::new2x2(3, 9, 81, 99);
	assert(Matrix::getNumOTentries() == 2);
	print(m2);

	cout << "adding m2 to m1" << endl;
	m1->add(&m2);
	count = Matrix::getNumOTentries();
	cout << "count: " << count << endl;
	assert(Matrix::getNumOTentries() == 2);

	delete m1;
	assert(Matrix::getNumOTentries() == 1);
	delete m2;
	assert(Matrix::getNumOTentries() == 0);

	///////////////////////////////////////////////////////////////////

	const int size = 1000;
	cout << "creating " << size << " objects from client - wait ..." << endl;
	Matrix *array[size];
	for (int i = 0; i < size; i++) {
		array[i] = Matrix::new2x2(i, 1, 0, 1);
	}
	assert(Matrix::getNumOTentries() == size);
	cout << "deleting " << size << " objects from client now"  << endl;

	for (i = 0; i < size; i++) {
		delete array[i];
	}
	assert(Matrix::getNumOTentries() == 0);

	///////////////////////////////////////////////////////////////////

	cout << "end of test" << endl;
        return 0;
}
