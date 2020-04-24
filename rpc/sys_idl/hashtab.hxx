// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: hashtab.hxx,v $
// Revision 1.1.3.2  1996/03/09  20:45:25  marty
// 	Add OSF copyright
// 	[1996/03/09  20:24:55  marty]
//
// Revision 1.1.3.1  1995/12/08  00:23:08  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/08  00:01:00  root]
// 
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/07  23:46:15  root]
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

#ifndef _HASH_TABLE
#define _HASH_TABLE

#include <dce/rpc.h>


#include <stdlib.h>

// This file defines the Hash Table used to store void * pointers.
// It implements a hash table of linked lists which is indexed by
// a uuid. Linked list elements are implemented by an
// RpcHashTableElement class.
// 
// The Hash Table is designed to be indexed by 1 or 2 uuids.
//
// The Hash Table is designed to be used in two ways - 
// 1) To look up a void * given a uuid (and possibly a second one)
// 2) To assign a void * to the table.
// 
// Hash Table Assignment
//   The Hash Table stores generic pointers as a void *. A subscript
//   operator returns a RpcHashTableElement reference. The RpcHashTableElement
//   assignment operator is then invoked with a void * parameter 
//   to assign the pointer to the table.
//
//   For example, each of the following two Hash Table assignments is 
//   is a valid expression.
//     hashTable[u1]     = (void *) new int (1234);
//     hashTable[u1][u2] = (void *) new int (6789);
// 
// Hash Table Lookup
//   The Hash Table lookup operation is performed with the RpcHashTable
//   subscript operator and the RpcHashTableElement subscript operator if 
//   a second subscript is used. Each of these operators returns an 
//   RpcHashTableElement reference. 
//
//   The return value can not be assigned to any variable since
//   the RpcHashTableElement class has a private constructor and no
//   variables of that type can be created. So the return value must be
//   cast to a valid type with the RpcHashTableElement void * operator and
//   then cast again to a specific pointer. The address-of operator
//   is also private to prevent access to RpcHashTableElement addresses.
//   
//   The following examples describle usage of Hash Table lookup.
//     int *x = (int *) (void *) hashTable[u1];		// Ok
//     int *x = (int *) (void *) hashTable[u1][u2];	// Ok
//     RpcHashTableElement e   = hashTable[u1];		// Wrong - private op
//     RpcHashTableElement *ep = hashTable[u1];		// Wrong - private op
//     hashTable[u1] = hashTable[u2];			// Ok but illegal use 
//
//   A lock variable is used to control concurrent access to the table.
//   The table is locked by the RpcHashTable::operator[] method which 
//   returns a RpcHashTableElement & value. Since this value must be
//   cast or referenced again if on the rhs or assigned to if on the lhs,
//   the unlock operation is performed there. This alleviates the use of
//   of explicit lock calls around Hash Table operations.
// 
//   However, the 
//	hashTable[u1] = hashTable[u2]
//   statement will cause a deadlock, since the RpcHashTable::operator[]
//   is called a second time before the RpcHashTableElement assignment
//   operator is called. This seems acceptable because this is an illegal
//   use of the table. The IDL compiler genrates code that accesses the
//   table and, therefore, has control of the table usage model. To guard
//   against this usage, the table lock is created with a non-recursive type
//   of mutex when the -DDEBUG compiler variable is defined. This will cause
//   an error message to be generated when a second lock is attempted on the
//   table. If it later turns out that this type of table access is needed,
//   the lock should be made into a recursive type to allow nested locks.
// 
//////////////////////////////////////////////////////////////////////////////

class RpcHashTableElement;

//////////////////////////////////////////////////////////////////////////////

const unsigned32 DEFAULT_HASH_TABLE_SIZE = 100;
const unsigned32 MAX_HASH_TABLE_SIZE     = 10000;

class RpcHashTable {

friend class RpcHashTableElement;

private:
	unsigned32		hashTableSize;	// hash table size
	RpcHashTableElement 	**Table;	// the hash table
	pthread_mutex_t 	lockFlag; 	// the mutex lock

	// hashes an object uuid to a table index
	unsigned32	hash(
				const uuid_t &
			);	
	
	// locks the table for use by the thread
	void	lock();

	// unlocks the table 
	void	unlock();

public:
	// null constructor - with a default hash table size 
				RpcHashTable(
					unsigned32 = DEFAULT_HASH_TABLE_SIZE
				);

	// destructor
				~RpcHashTable();
	
	// subscript operator
	RpcHashTableElement	&operator[](const uuid_t &);

	// subscript operator
	uuid_t &		operator[](const void *);

#ifdef DEBUG
	// prints table for debugging
	void 		dump() const;

	// prints profile of table for debugging
	void 		profile() const;
#endif

	// returns number of entries in the table
	long 		numEntries() const;
};

//////////////////////////////////////////////////////////////////////////////

class RpcHashTableElement {

friend class RpcHashTable;

private:
	uuid_t			key;		// object uuid
	void 			*objectPtr;	// object pointer
	RpcHashTableElement 	**head;		// points to head of this list
	RpcHashTableElement 	*nextElement;	// points to next element
	RpcHashTableElement 	*nextSubElement;// points to next sublist
	RpcHashTableElement 	*parent;	// points to parent of sublist
	RpcHashTable		*table;		// points to containing table

	// constructor to create an element
			RpcHashTableElement(
				const uuid_t &, 
				void *,
				RpcHashTable *
			);
	
	// destructor to delete an element
			~RpcHashTableElement();

	// returns TRUE if uuid matches this one
	boolean32	operator==(
				const uuid_t &
			);

	// private address-of operator to prevent access to table pointers
	RpcHashTableElement	*operator&();

public:
	// subscript operator
	RpcHashTableElement	&operator[](
					const uuid_t &
				);

	// assignment operator
	void *		operator=(
					void *
			);

	// cast-to-void*  operator
			operator void*() ;

};

//////////////////////////////////////////////////////////////////////////////

#endif
