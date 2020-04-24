// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
//
//
// HISTORY
// $Log: objref.hxx,v $
// Revision 1.1.3.2  1996/03/09  20:45:29  marty
// 	Add OSF copyright
// 	[1996/03/09  20:24:58  marty]
//
// Revision 1.1.3.1  1995/12/08  00:23:29  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/08  00:07:15  root]
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

#ifndef rpc_object_reference_v0_0_included
#define rpc_object_reference_v0_0_included

#include <dce/idlbase.h>
#include <dce/idlddefs.h>
#include <dce/rpc.h>


#ifdef DEBUG
#ifdef _MSDOS
#include <iostream.h>
#else
#include <stream.h>
#endif
#endif

#ifndef TRUE
#define TRUE   1
#endif

#ifndef FALSE
#define FALSE   0
#endif

// Rebind policies used by clients when a binding handle fails
typedef enum {
	never_rebind, attempt_rebind, wait_on_rebind, attempt_rebind_n
} DCERebindPolicy;


// This file defines the class for an rpc_object_reference,
// which is the base class for all RPC objects. An abstract
// base class derives from this class, which in turn is used
// to derive a proxy or implementation class. 

//////////////////////////// PingInfo definition ///////////////////////////////

typedef struct {
	unsigned short	period;			// pinging period
} PingInfo;

//////////////////////////// Principle definition //////////////////////////////

typedef struct {
	unsigned short	prinid;			// principal id
} PrincipleInfo;

///////////////////// Overflow for an ObjectRef Definition /////////////////////

typedef struct {
	PingInfo		*pingInfo;	// ping information
	rpc_tower_vector_p_t    twr_vector;     // tower vector for protocols
} OrefOverFlow;


//////////////////////////// ObjectRef definition //////////////////////////////

typedef struct {
	uuid_t		objectId;		// object's uuid
	rpc_if_handle_t	ifHandle;		// interface handle
	unsigned_char_t	*objectName;		// optional object's name
	OrefOverFlow	*overFlow;		// more information
	PrincipleInfo	*principal;		// principal information
	int		vectorIndex;		// index into binding vector
	rpc_binding_vector_p_t objectBindings;	// object's binding handles
} ObjectRef;

//////////////////////////// Extern Declarations //////////////////////////////

// my friend functions that allows the marshalling routines and
// reference counting apis to access to my private member functions.

extern "C" {

extern idl_ulong_int incRef(handle_t, uuid_t, uuid_t);
extern void decRef(handle_t, uuid_t);

}

//////////////////////////// Forward Declarations //////////////////////////////

class rpc_object_reference;

//////////////////////// interface hierarchy list //////////////////////////////

class RpcInterfaceList {

friend rpc_object_reference;

private:
	rpc_if_handle_t		ifHandle; // interface handle
	void			*ptr;	  // pointer to interface part of object
	RpcInterfaceList	*next;	  // pointer to next item on list

public:
	RpcInterfaceList(		  // constructor
		void *p,		  // pointer to object part
		rpc_if_handle_t ifspec,	  // interface handle
		RpcInterfaceList *head	  // list to attach element to
	) : ptr(p), ifHandle(ifspec), next(head) {}
};

// This class definition is used to pass arguments to a thread that will
// cause the RpcInterfaceList for an object to be updated with the correct
// information once the information is available. This becomes necessary
// when static RPC objects are declared in an application and their
// constructors are called before the interface manager constructor is called.

class Ifspec_update_information {

friend rpc_object_reference;

private:
	uuid_t		iid;			// interface id
	rpc_if_handle_t	*ifHandle;		// address of ifspec in object

public:
	Ifspec_update_information(
		const uuid_t & iid,		// interface id
		rpc_if_handle_t *ifHandle	// address of ifspec in object
	) : iid(iid), ifHandle(ifHandle)
	{};
};

class ThreadKey;

/////////////////////////// common base class //////////////////////////////////

class rpc_object_reference {

friend idl_ulong_int incRef(handle_t, uuid_t, uuid_t);
friend void decRef(handle_t, uuid_t);
friend void bump_reference_count(void *, ObjectRef &, IDL_msp_t);
friend void rpc_ss_ndr_object_to_oref(void *, ObjectRef *, uuid_t &);
friend void rpc_ss_ndr_oref_to_object(ObjectRef *, rpc_object_reference **, uuid_t &);
friend ThreadKey;

private:  

	ObjectRef	objectRef;		// object reference info
	boolean32	object_is_local;	// indicates local instance
	unsigned long	referenceCount;		// for reference counting
	RpcInterfaceList *interfaceList;	// interface linked list
	void            *oflow;			// object lock and reserved

	// returns TRUE if the encapsulated binding is incomplete
	boolean32	bindingIncomplete() const;	

	// does a CDS lookup by name
	boolean32	bindByName(
				boolean32 = TRUE // resolve endpoint
			);

	// does a CDS lookup by uuid
	boolean32	bindByUuid();

	// uses explicit binding handle
	boolean32	bindByHandle();

	// binds to a named object using encapsulated information
	boolean32	cdsBind() ;

	// returns the object pointer from the Object Table by oid
	static void *	get_object(
				const uuid_t &	// object uuid	
			);

	// returns the object pointer from the Object Table by oid/iid
	static void *	get_object(
				const uuid_t &,	// object uuid	
				const uuid_t &	// interface uuid	
			);

	// returns the object's reference
	ObjectRef	*get_reference();

	// once-block function for creating the key for thread-specific storage
	static void get_thread_key();

	// returns TRUE if object has been marshalled before
	boolean32	has_been_marshalled(); 

	// returns TRUE if object is implemented locally
	boolean32	is_local();

	// prepares the object to be accessed from a client
	void		prepare_to_marshall();

	// gives object an identity - create a uuid 
	uuid_t		set_identity();

	// store an object hierarchy in the Object Table
	void		registerObjectInTable(
				const uuid_t &  // object id
			);

	// store a proxy in the Proxy (Object) Table
	void		registerProxyInTable(
				const uuid_t &,  // object id
				const uuid_t &,	 // interface id
				const void *	 // proxy pointer
			);

	// function used by listener thread to receive RPC calls
	static void	rpc_ss_listener_thread(
				void
			);

	// locks the object
	void		lock();	

	// unlocks the object
	void		unlock();	

#ifdef DEBUG
	// prints the encapsulated information for debugging purposes
	void		print() const;
#endif
	static void	ifspec_update(Ifspec_update_information *);
	void		destructor();


	// increments the reference count
	unsigned long	incRef(
				const boolean32 = FALSE
			);

	// decrements the reference count and releases object if last reference
	unsigned long	decRef();

protected:

	// null constructor - used by object implementations
			rpc_object_reference ();

	// object reference constructor - used by object proxies
			rpc_object_reference (
				ObjectRef &	// object reference from server
			);

	// returns the object's binding handle
	handle_t	getHandle();

	// returns the object's binding handle
	handle_t	getHandle(
				RPC_SS_THREADS_MUTEX_T *,
				unsigned32 &,
				error_status_t *
	);

	// returns the object's binding handle
	handle_t	getHandle(
				RPC_SS_THREADS_MUTEX_T *,
				signed32 &,
				signed32 &,
				error_status_t *
	);

	// registers the object in the name space
	void		registerObject(
				const void *, 		// object ptr
				const unsigned char *,	// object name
				const uuid_t &,		// interface id
				const boolean32 = TRUE	// replace endpoints
			);

	// encapsulates binding information using a name
	static void * IDL_ENTRY setBindInfo(
				const unsigned char *,	// name to bind
				const uuid_t &		// interface id
			);

	// encapsulates binding information using a uuid
	static void * IDL_ENTRY setBindInfo(
				const uuid_t &,		// uuid to match
				const uuid_t &		// interface id
			);

	// encapsulates binding information using a binding handle
	static void * IDL_ENTRY setBindInfo(
				const handle_t,		// explicit handle
				const uuid_t &		// interface id
			);

	// encapsulates binding information using another RPC object
	static void * IDL_ENTRY setBindInfo(
				const rpc_object_reference *,// explicit handle
				const uuid_t &		// interface id
			);

	// registers an interface in the linked list of supported interfaces
	void rpc_register_interface(
				void *,			// object pointer
				const uuid_t & 		// interface id
			);
public:

	// destructor - so can delete derived classes from generic pointer
	virtual	~rpc_object_reference();

	// converts a binding handle and interface id into a 'this' pointer
	static void * IDL_ENTRY _getThis(
				const handle_t,		// the binding handle
				const uuid_t &		// interface uuid	
			);

	// sets authorization/authentication on binding vector
	void		secure(
				unsigned char * =	// server pricipal name
					0,
				unsigned32 =		// protection level 
					rpc_c_protect_level_default,
				unsigned32 =		// authorization 
					rpc_c_authn_default,
				rpc_auth_identity_handle_t = //auth identity
					NULL,
				unsigned32 authz_svc =	// authorization
					rpc_c_authz_name
			);

	// returns the binding handle from thread-specific storage
	handle_t	get_binding_handle();

	// sets the rebind policy for a proxy
	void SetRebind(
				DCERebindPolicy,	// rebind policy
				unsigned32 = 0		// for attempt_rebind_n
	);

};

////////////////////////////// offset class ////////////////////////////////////
//
// This class is used to get hold of base class operations from 
// an object that we know is derived from the base class but we
// don't know what type of object it is. It acts as a cast from
// the derived class to the base class. 

class basecast : public virtual rpc_object_reference {
	virtual void dummy() = 0;
};

#endif
