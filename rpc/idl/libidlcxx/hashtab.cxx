// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: hashtab.cxx,v $
// Revision 1.1.2.2  1996/03/09  20:45:03  marty
// 	Add OSF copyright
// 	[1996/03/09  20:24:43  marty]
//
// Revision 1.1.2.1  1995/12/08  00:13:10  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/07  23:55:10  root]
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

#ifndef WIN32
extern "C" {
#include <pthread.h>
}
#endif
#include <dce/hashtab.hxx>

#ifdef DEBUG
#include <iostream.h>
#define DEBUG_ON getenv("DEBUG")
#endif

#include <dce/rpcexc.h>

// The Hash Table mutex lock

////////////////////////////////////////////////////////////////////////////////
//
// This is the constructor which builds an element from 
// a uuid/object pointer pair. Only the RpcHashTable class
// is allowed to call this constructor since it is private.
//
// Input:
//	id	- the element key
//	p	- the object pointer
//      t       - pointer to table containing this element
//
// Output:
//	none 
//
// Errors:
//	none
//
////////////////////////////////////////////////////////////////////////////////

RpcHashTableElement::RpcHashTableElement(const uuid_t &id,
					 void *p, 
					 RpcHashTable *t) :
						key(id), 
						objectPtr(p),
						table(t),
						head(0),
				    		nextElement(0),
						nextSubElement(0),
						parent(0) {
}

////////////////////////////////////////////////////////////////////////////////
//
// This is the destructor which destroys an element and
// its sublist of elements.
//
// Input:
//	none
//
// Output:
//	none 
//
// Errors:
//	none
//
////////////////////////////////////////////////////////////////////////////////

RpcHashTableElement::~RpcHashTableElement() {

	RpcHashTableElement *e;
	RpcHashTableElement *q;
	RpcHashTableElement *r;

	e = *head;				// get list this element is on
	if (e == this)
		*head = this->nextElement;	// remove first element
	else {
		// remove mid-list element
		for (; e->nextElement != this; e = e->nextElement);
		e->nextElement = nextElement;
	}

	// if there is a sublist, delete all elements on it
	q = this->nextSubElement;
	while (q) {
		r = q->nextElement;
		delete q;
		q = r;
	}
	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function returns TRUE if the arguement uuid 
// matches the one in this element instance.
//
// Input:
//	k	- the uuid
//
// Output:
//	boolean32 - TRUE if k equals this->key, FALSE otherwise 
//
// Errors:
//      if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

boolean32
RpcHashTableElement::operator==(const uuid_t &k) {
	boolean32	result;
	unsigned32	st;

	result =  uuid_compare(&key, (uuid_t *) &k, &st);
	if (st != error_status_ok) exc_raise_status(st);

	return !result;
}

////////////////////////////////////////////////////////////////////////////////
//
// This is the subscript operator that returns an element reference
// given a uuid. This function is called when the Hash Table
// is referenced by more than 1 subscript. If the element is
// not found a new one is created, indexed by the uuid.
// 
// The Hash Table is designed to be indexed by either 1 or 2 subscripts,
// the first level returning the object, the second returning a sub-object
// when inheritance is involved. 
//
// The first subscript operation returns a RpcHashTableElement reference
// from searching the Hash Table linked lists.  
// The second subscript operation returns a RpcHashTableElement reference
// from searching the sublist of the element.
//
// 
// Input:
//	id	- the Hash Table subscipt
//
// Output:
//	RpcHashTableElement& - a reference to the element keyed by the uuid 
//
// Errors:
//	none
//
////////////////////////////////////////////////////////////////////////////////

RpcHashTableElement &
RpcHashTableElement::operator[](const uuid_t & id) {

	// We are working on the second subscript of an Hash Table
	// reference when this operation is invoked.

	RpcHashTableElement	*p;
	RpcHashTableElement	*q;
	int			index;

	// search for match in this bucket and return it's reference if found 
	for (p = this->nextSubElement; p; p = p->nextElement)
		if (*p == id) {
			return *p;
		}
		
	// create a new sub-element, 
	// put it in the bucket and 
	// return it's reference
	p = new RpcHashTableElement(id, 0, table);
	p->head = (RpcHashTableElement **) &this->nextSubElement;
	p->parent = this;

	// add to end of sublist
	if (this->nextSubElement == 0) {
		this->nextSubElement = p;
	} else {
		for (q = this->nextSubElement;
		     q->nextElement;
		     q = q->nextElement);
		q->nextElement = p;
	}
	return *p;
}

////////////////////////////////////////////////////////////////////////////////
//
// This is the address-of operator that returns an element pointer.
// Its purpose is simply to make the address-of operation private
// and inaccessible from outside the class, thereby preventing illegal
// access to the table.
// 
// Input:
//	none
//
// Output:
//	RpcHashTableElement * - address of a table element.
//
// Errors:
//	none
//
////////////////////////////////////////////////////////////////////////////////

RpcHashTableElement*
RpcHashTableElement::operator&() {
	return this;
}

////////////////////////////////////////////////////////////////////////////////
//
// This is the assignemnt operator which assigns the object pointer p
// to this element. If p is NULL, the element is deleted from the Hash Table
// It may have been created when a reference was made to a non-existant
// element and the subscript operator created one.
//
// The table is locked by the time this function is invoked and
// it is unlocked here. Since RpcHashTableElements must be either
// assigned to or cast to a void *, this method is one of those that
// must be called after referenceing the RpcHashTable.
//
// Input:
//	p	- points to an object
//
// Output:
//	void *	- the same object pointer passed in 
//
// Errors:
//	none
//
////////////////////////////////////////////////////////////////////////////////

void *
RpcHashTableElement::operator=(void *p) {

	RpcHashTable            *theTable;	// points to containing table

	// assign pointer to element
	if (p) {	// assign object pointer
		objectPtr = p;
		table->unlock();
		return objectPtr;
	} else {	// null assignment means to delete this element
		theTable = table;		// save table ptr before freeing
		delete this;
		theTable->unlock();		// unlock thru save pointer
		return 0;
	}
}

////////////////////////////////////////////////////////////////////////////////
//
// This is the void* cast operator which translates this element
// into an object pointer.
//
// The table is locked by the time this function is invoked and
// it is unlocked here. Since RpcHashTableElements must be either
// assigned to or cast to a void *, this method is one of those that
// must be called after referenceing the RpcHashTable.
//
// If the pointer field in this element is 0, then this element was
// temporarily created by a subscript operator and can be safely removed
// here.
//
// Input:
//	none
//
// Output:
//	none 
//
// Errors:
//	none
//
////////////////////////////////////////////////////////////////////////////////

RpcHashTableElement::operator void*() { 

	RpcHashTable            *theTable;	// points to containing table

	// if the obejct pointer is null then delete the element
	if (objectPtr == 0) {
	    theTable = table;			// save ptr before freeing mem
	    if ( (*head)->parent && (*head)->parent->objectPtr == 0)
		delete (*head)->parent;
	    else
		delete this;
	    theTable->unlock();			// unlock with saved pointer
	    return 0;
	}

	table->unlock();
	return objectPtr;
}


////////////////////////////////////////////////////////////////////////////////
//
// This is the default constructor which builds the hash table and lock for it.
//
// If DEBUG is on, then create a non-recursive mutex for locking the table
// so we can find bugs easier. Otherwise use a fast (default) mutex.
//
// Input:
//	size	- default hash table size
//
// Output:
//	none 
//
// Errors:
//	none
//
////////////////////////////////////////////////////////////////////////////////

RpcHashTable::RpcHashTable(unsigned32 size) : hashTableSize(size),
                                                  Table(0) {

	int		st;
#ifdef DEBUG
	pthread_attr_t  thread_attr;
#endif

	// validate the table size
	if (size <= 0 || size > MAX_HASH_TABLE_SIZE)
		RAISE(rpc_x_coding_error);

	// allocate a hash table
	Table = new RpcHashTableElement * [hashTableSize];
	if (!Table)
		RAISE(rpc_x_no_memory);

	// initialize all buckets to empty
	for (int i = 0; i < hashTableSize; i++)
		Table[i] = 0;

	// create the lock variable
#ifdef DEBUG
	st = pthread_mutexattr_create(&thread_attr);
	if (st != 0)
		RAISE(rpc_x_coding_error);

	st = pthread_mutexattr_setkind_np(&thread_attr, MUTEX_NONRECURSIVE_NP);
	if (st != 0)
		RAISE(rpc_x_coding_error);

	st = pthread_mutex_init(&lockFlag, thread_attr);
	if (st != 0)
		RAISE(rpc_x_coding_error);
#else
	st = pthread_mutex_init(&lockFlag, pthread_mutexattr_default);
	if (st != 0)
		RAISE(rpc_x_coding_error);
#endif

	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// This is the destructor which deletes a hash table.
// The Hash Table is not actually deleted but rather made empty.
// This is so that if the compiler invokes the destructor function for any
// global objects derived from the rpc_object_reference base class during
// process exit, that destructor function can access the Hash Table intact.
//
// The Hash Table is a global variable and its destructor is automatically
// invoked only during process exit time, possibly before all global
// RPC objects have also been destroyed. There is no memory leak because
// the process is exiting when this operation is invoked and that is the
// only time this operation is used.
//
// Input:
//	none
//
// Output:
//	none 
//
// Errors:
//	none
//
////////////////////////////////////////////////////////////////////////////////

RpcHashTable::~RpcHashTable() {
	int			st;
	RpcHashTableElement	*p, *q;

#ifdef DEBUG
	if (DEBUG_ON)
		dump();
#endif

	// for each bucket, delete all elements
	for (int i = 0; i < hashTableSize; i++) {
		p = Table[i];
		while (p) {
			q = p->nextElement;
			delete p;
			p = q;
		}
	}

	// don't delete the table but if we did we would do ...
	//delete [] Table; 

	// don't destroy the lock flag but if we did we would do ...
	//st = pthread_mutex_destroy(&lockFlag);
	//if (st != 0)
		//RAISE(rpc_x_coding_error);

	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// This is the subscript operator which indexes into the table
// and returns an element. If the element does not exist, it
// is created.
//
// The table is locked here and left that way. A subsequent operation
// invoked on the returned element will unlock it. We can be sure that
// the subsequent operation is invoked since the RpcHashTableElement
// class can not be instantiated, and so must be cast to a void * or
// assigned to.
//
// Input:
//	id	- the table index
//
// Output:
//	RpcHashTableElement - the indexed element 
//
// Errors:
//	none
//
////////////////////////////////////////////////////////////////////////////////

RpcHashTableElement & 
RpcHashTable::operator[](const uuid_t & id) {

	int index;
	RpcHashTableElement *p;

	// get hash bucket 
	index = hash(id);

	// lock the table - it will be subsequently unlocked by
	// an element operation on the returned element
	lock();

	// search for match in this bucket and return it's reference if found 
	for (p = Table[index]; p; p = p->nextElement)
		if (*p == id)
			return *p;
		
	// create a new element,
	// put it at front of list and
	// return it's reference
	p = new RpcHashTableElement(id, 0, this);
	p->nextElement = Table[index];
	p->head = &Table[index];
	Table[index] = p;

	return *p;
}

////////////////////////////////////////////////////////////////////////////////
//
// This is the subscript operator which indexes into the table by void *
// and returns an element. If the element does not exist, it
// is created.
//
// The table is locked here and left that way. A subsequent operation
// invoked on the returned element will unlock it. We can be sure that
// the subsequent operation is invoked since the RpcHashTableElement
// class can not be instantiated, and so must be cast to a void * or
// assigned to.
//
// Note that this operation is obsolete but kept for possible future uses.
//
// Input:
//	id	- the table index
//
// Output:
//	RpcHashTableElement - the indexed element 
//
// Errors:
//	none
//
////////////////////////////////////////////////////////////////////////////////

uuid_t &
RpcHashTable::operator[](const void * addr) {

	int		    index;
	RpcHashTableElement *p;
	RpcHashTableElement *q;
	uuid_t		    nilUuid;
	unsigned32	    st;

	// lock the table - it will be subsequently unlocked by
	// an element operation on the returned element
	lock();

	// search for match in this bucket and return it's reference if found 

	for (index = 0; index < hashTableSize; index++) {
		for (p = Table[index]; p; p = p->nextElement) {
			if (p->objectPtr == addr) {
				unlock();
				return p->key;
			}
			for (q = p->nextSubElement; q; q = q->nextElement)
				if (q->objectPtr == addr) {
					unlock();
					return (*(q->head))->parent->key;
				}
		}
	}
	unlock();
		
	uuid_create_nil(&nilUuid, &st);
	if (st != error_status_ok) exc_raise_status(st);

	return nilUuid;
}

////////////////////////////////////////////////////////////////////////////////
//
// This is the hash function which converts an Hash Table key
// into an index. The right shift has proven to produce a reasonable
// distribution of uuids.
//
// Input:
//	key	- the key to hash on
//
// Output:
//	unsigned32 - the table index 
//
// Errors:
//	none
//
////////////////////////////////////////////////////////////////////////////////

unsigned32
RpcHashTable::hash(const uuid_t & key) {

	unsigned16	h;
	unsigned32	st;

	h = uuid_hash((uuid_t *) &key, &st);
	if (st != error_status_ok) exc_raise_status(st);

	return((h >> 3) % hashTableSize);
}

////////////////////////////////////////////////////////////////////////////////
//
// This function prints the table for deugging purposes
//
// Input:
//	none
//
// Output:
//	printout of table 
//
// Errors:
//	none
//
////////////////////////////////////////////////////////////////////////////////

#ifdef DEBUG

void
RpcHashTable::dump() const {
	RpcHashTableElement	*p;
	RpcHashTableElement	*q;
	int			i;
	int			count;

	cout << endl << "***** Hash Table Dump *****" << endl;
	count = 0;
	for (i = 0; i < hashTableSize; i++) {
		cout << "Bucket " << i << "\t";
		if (!Table[i])
			cout << "EMPTY";
		for (p = Table[i]; p; p = p->nextElement) {
			count++;
			cout << p->objectPtr;
			cout << " {";
			if (p->nextSubElement) {
				for (q = p->nextSubElement; q; q = q->nextElement) {
					count++;
					cout << q->objectPtr << "\t";
				}
			}
			cout << "}\t";
		}
		cout << endl;
	}
	cout << "Total number of objects left in table: " << count << endl;

	return;
}

#endif


////////////////////////////////////////////////////////////////////////////////
//
// This function profiles the table for deugging purposes
// by printing the percent capcities for each bucket.
//
// Input:
//	none
//
// Output:
//	printout of table profile.
//
// Errors:
//	none
//
////////////////////////////////////////////////////////////////////////////////

#ifdef DEBUG

void
RpcHashTable::profile() const {
	RpcHashTableElement	*p;	// used to traverse the table
	int			i;	// loop counter
	int			j;	// counts total entries on a hash list
	int			k;	// counts total entries in the table

	cout << endl << "***** Hash Table Profile *****" << endl;
	cout << "Hash Table Size: " << hashTableSize << endl;
	cout << "Table\tList\tPercent" << endl;
	cout << "Index\tLength\tof Table" << endl;

	for (i = 0, k = 0; i < hashTableSize; i++) {
		for (p = Table[i]; p; p = p->nextElement, k++);
	}

	if (k) {
		for (i = 0; i < hashTableSize; i++) {
			cout << i << "\t";
			for (p = Table[i], j = 0; p; p = p->nextElement, j++);
			cout << j << "\t" << ((float) j / (float) k) * 100 << endl; 
		}
	}

	cout << "Total Table Entries: " << k << endl;

	return;
}

#endif


////////////////////////////////////////////////////////////////////////////////
//
// This function returns the number of entries currently in the table
//
// Input:
//	none
//
// Output:
//	number of table entries
//
// Errors:
//	none
//
////////////////////////////////////////////////////////////////////////////////

long
RpcHashTable::numEntries() const {
	RpcHashTableElement	*p;	// used to traverse the table
	int			i;	// loop counter
	int			count;	// counts total entries in the table

	for (i = 0, count = 0; i < hashTableSize; i++) {
		for (p = Table[i]; p; p = p->nextElement, count++);
	}

	return count;
}


////////////////////////////////////////////////////////////////////////////////
//
// This function locks the table for this thread.
// It the table is already locked, this function blocks.
//
// Input:
//	none
//
// Output:
//	none 
//
// Errors:
//      if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

#ifndef AIX32
inline
#endif
void
RpcHashTable::lock() {
	int	st;

	st = pthread_mutex_lock(&lockFlag);
	if (st != 0)
		RAISE(rpc_x_coding_error);

	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function releases the lock on the table
//
// Input:
//	none
//
// Output:
//	none 
//
// Errors:
//      if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

#ifndef AIX32
inline
#endif
void
RpcHashTable::unlock() {
	int	st;

	st = pthread_mutex_unlock(&lockFlag);
	if (st != 0)
		RAISE(rpc_x_coding_error);

	return;
}

