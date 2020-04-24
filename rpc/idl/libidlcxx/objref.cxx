//
// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
//
//
// HISTORY
// $Log: objref.cxx,v $
// Revision 1.1.2.2  1996/03/09  20:45:11  marty
// 	Add OSF copyright
// 	[1996/03/09  20:24:47  marty]
//
// Revision 1.1.2.1  1995/12/08  00:13:15  root
// 	Submit OSF/DCE 1.2.1
// 
// 	HP revision /main/dat_xidl2/1  1995/11/17  17:09 UTC  dat
// 	New file for second XIDL drop for DCE 1.2.1
// 	[1995/12/08  00:06:28  root]
// 
// Revision 1.1.2.1  1995/10/23  03:21:33  bfc
// 	oct 95 idl drop
// 	[1995/10/23  02:18:17  bfc]
// 
// 	may 95 idl drop
// 	[1995/10/23  02:17:40  bfc]
// 
// $EndLog$
//
//
//  Copyright (c) Digital Equipment Corporation, 1994
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

#include <pthread.h>
#include <dce/rpc.h>
#ifndef _MSDOS
#ifndef PTHREAD_EXC
#define PTHREAD_EXC
#endif
#endif
#include <dce/stubbase.h>
#include <dce/rpcexc.h>
extern "C" {
#if defined(__VMS) || defined(_MSDOS) 
#include <stdio.h>
#endif
#include <dce/dce_cf.h>
}
#include <dce/objref.hxx>
#include <dce/objtable.hxx>
#include <dce/proxymgr.hxx>
#include <dce/ifacemgr.hxx>
#include "refcnt.h"
#include <dce/trace.hxx>

#ifdef DEBUG
#include <iostream.h>
#define DEBUG_ON getenv("DEBUG")
#endif

#include <new.h>
#include <string.h>


// macro to free context handle when
#define FREE_CONTEXT rpc_ns_binding_lookup_done(&import_context, &ignore);

// Static and Global variables

// identifies named objects without a character name (i.e. uuid or handle)
static unsigned_char_t * const SpecialName = (unsigned_char_t *) -1;	

// key used to store/retrieve a binding handle in thread-specific storage
static pthread_key_t rpc_object_reference__key;

// once-block used to create a thead-specific storage key
static pthread_once_t rpc_object_reference__once_block = pthread_once_init;

//////////////////// Thread-specific Key Initilizer class //////////////////////

// This class assures that the thread key will be initialized 
// before it is referenced

class ThreadKey {
public:
	ThreadKey() {

		int		ts;		// pthread status

		// create the thread-specific key for storing a binding handle
		ts = pthread_once(
			&rpc_object_reference__once_block,
			(pthread_initroutine_t) 
				rpc_object_reference::get_thread_key
		);
		if (ts != 0) RAISE(rpc_x_coding_error);
	}
};

ThreadKey threadKey;

////////////////////////// Rebind Policy definition ////////////////////////////

typedef struct {
	DCERebindPolicy policy;                 // rebind policy
	unsigned32      number;                 // used for attempt_rebind_n
} RebindPolicy;


//////////////////////////// Overflow Information //////////////////////////////

typedef struct {
	pthread_mutex_t         lockFlag;       // the mutex lock per object
	RebindPolicy            *rebindPolicy;  // proxy rebind policy
	void                    *future;        // for more overflow information
} OverFlow;

// This file is the implementation for the rpc_object_reference class
// which is the base class for all RPC objects.


#define GET_INTERFACE_ID(a) \
		((rpc_if_rep_t *) (a)->ifHandle)->id

////////////////////////////////////////////////////////////////////////////////
//
// This is the null constructor, which is used by object implementation
// classes. The object is therefore locally implemented.
//
// This function also executes a pthread once-block to create a key
// for storing a binding handle in thread-spedific storage
//
// Input:
//	none
//
// Output:
//	none 
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

rpc_object_reference::rpc_object_reference () : 
				referenceCount(0),
				object_is_local(TRUE),
				interfaceList(0)
{
	Trace("rpc_object_reference::rpc_object_reference()");

	unsigned32	st;			// status code
	int		ts;			// pthread status
#ifdef DEBUG
	pthread_attr_t  thread_attr;
#endif

	// set the object reference to null 
	uuid_create_nil(&objectRef.objectId, &st);
	if (st != rpc_s_ok) exc_raise_status(st);

     	objectRef.objectBindings = 0;
	objectRef.objectName     = 0;
	objectRef.ifHandle       = 0;
	objectRef.principal	 = 0;
	objectRef.vectorIndex    = -1;
	objectRef.overFlow	       = new OrefOverFlow;
	objectRef.overFlow->pingInfo   = 0;

	objectRef.overFlow->twr_vector = 0;

	// create the lock variable
	oflow = new OverFlow;
	((OverFlow *) oflow)->rebindPolicy = 0;
	((OverFlow *) oflow)->future     = 0;
#ifdef DEBUG
	ts = pthread_attr_create(&thread_attr);
	if (ts != 0) RAISE(rpc_x_coding_error);

	ts = pthread_mutexattr_setkind_np( &thread_attr, MUTEX_NONRECURSIVE_NP);
	if (ts != 0) RAISE(rpc_x_coding_error);

	ts = pthread_mutex_init( & ((OverFlow *) oflow)->lockFlag, thread_attr);
	if (ts != 0) RAISE(rpc_x_coding_error);
#else
	ts = pthread_mutex_init(
		& ((OverFlow *) oflow)->lockFlag, 
		pthread_mutexattr_default
	);
	if (ts != 0) RAISE(rpc_x_coding_error);
#endif

	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// This is the other constructor, which is used by object proxy classes.
// The object is therefore remotely implemented, and we have a reference to it.
//
// Input:
//	oref	- the remote object's reference
//
// Output:
//	none 
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

rpc_object_reference::rpc_object_reference(ObjectRef &oref) :
				referenceCount(1),
				object_is_local(FALSE),
				interfaceList(0)
{
	Trace("rpc_object_reference::rpc_object_reference(ObjectRef)");

	int		ts;			// pthread status

#ifdef DEBUG
	pthread_attr_t  thread_attr;
#endif

	// set the object reference from the parameter
	objectRef.objectId	 = oref.objectId;
	objectRef.objectBindings = oref.objectBindings;
	objectRef.objectName     = oref.objectName;

	// add tower vector to overflow area for an ObjectRef
	objectRef.overFlow       = new OrefOverFlow;
	if (oref.overFlow) 
		objectRef.overFlow->twr_vector = oref.overFlow->twr_vector;
	else
		objectRef.overFlow->twr_vector = 0;

	// add ping information to overflow area for an ObjectRef
	objectRef.overFlow->pingInfo = new PingInfo;
	if (oref.overFlow->pingInfo) 
		objectRef.overFlow->pingInfo->period = 
			oref.overFlow->pingInfo->period;
	else
		objectRef.overFlow->pingInfo->period = 0;

	// add principal information to ObjectRef
	objectRef.principal = new PrincipleInfo;
	if (oref.principal)
		objectRef.principal->prinid = oref.principal->prinid;
	else
		objectRef.principal->prinid = 0;

      	objectRef.ifHandle       = oref.ifHandle;
	objectRef.vectorIndex	 = oref.vectorIndex;

	// create the lock variable
	oflow = new OverFlow;
	((OverFlow *) oflow)->rebindPolicy         = new RebindPolicy;
	((OverFlow *) oflow)->rebindPolicy->policy = attempt_rebind;
	((OverFlow *) oflow)->rebindPolicy->number = 0;

	((OverFlow *) oflow)->future     = 0;
#ifdef DEBUG
	ts = pthread_attr_create(&thread_attr);
	if (ts != 0) RAISE(rpc_x_coding_error);

	ts = pthread_mutexattr_setkind_np( &thread_attr, MUTEX_NONRECURSIVE_NP);
	if (ts != 0) RAISE(rpc_x_coding_error);

	ts = pthread_mutex_init( & ((OverFlow *) oflow)->lockFlag, thread_attr);
	if (ts != 0) RAISE(rpc_x_coding_error);
#else
	ts = pthread_mutex_init(
		& ((OverFlow *) oflow)->lockFlag,
		pthread_mutexattr_default
	);
	if (ts != 0) RAISE(rpc_x_coding_error);
#endif
	return;
}


////////////////////////////////////////////////////////////////////////////////
//
// This function returns TRUE if the encapsulated binding information
// is incomplete. The binding information is incomplete if there is no
// binding handle.
//
// Input:
//	none
//
// Output:
//	boolean32 - TRUE if binding is incomplete, otherwise FALSE.
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

inline
boolean32
rpc_object_reference::bindingIncomplete() const {
	Trace("rpc_object_reference::bindingIncomplete()");

	return objectRef.vectorIndex == -1;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function performs a CDS lookup on the encapsulated 
// object name looking for a binding vector. If a binding 
// handle is found, the encapsulated object reference is updated
// with the binding information. If the information is not found
// the encapsulated object reference is not changed and this function returns.
//
// Input:
//	resolve - if TRUE (default), call endpoint mapper to resolve binding
//
// Output:
//	boolean32 - TRUE if binding vector found and endpoint resolved 
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

boolean32
rpc_object_reference::bindByName(boolean32 needToResolve) {
	Trace("rpc_object_reference::bindByName()");

	rpc_ns_handle_t	import_context;
	boolean32	done;
	boolean32	resolved;
	unsigned32	ignore;
	unsigned32	st;

	// start the name space query at the named entry
	rpc_ns_binding_lookup_begin(
			rpc_c_ns_syntax_default,
			objectRef.objectName,
			objectRef.ifHandle,
			NULL,				// no object uuid
			rpc_c_binding_max_count_default,
			&import_context,
			&st
	);
	if (st != rpc_s_ok && st != rpc_s_entry_not_found) {
		exc_raise_status(st);
	}

	// if name not found get out
	if (st == rpc_s_entry_not_found) {
		return FALSE;
	}

	done = FALSE;
	while (!done) {
		// start out with no proper binding handle
		objectRef.vectorIndex = -1;

		// get the binding vector
		rpc_ns_binding_lookup_next(
			import_context,
			&objectRef.objectBindings,
			&st
		);
		if (st != rpc_s_ok		&&
		    st != rpc_s_entry_not_found	&&
		    st != rpc_s_no_more_bindings) {
			FREE_CONTEXT;
			exc_raise_status(st);
		}

		// if binding vector not found, get out
		if (st == rpc_s_entry_not_found ||
		    st == rpc_s_no_more_bindings) {
			FREE_CONTEXT;
			return FALSE;
		}

		// got a binding vector so lets assume we will use 1st handle
		objectRef.vectorIndex = 0; 

		for (resolved = FALSE;

		    (needToResolve	&&
		    !resolved		&&
		    objectRef.vectorIndex < objectRef.objectBindings->count);

		    objectRef.vectorIndex++) {

			rpc_ep_resolve_binding(
				objectRef.objectBindings->
				binding_h[objectRef.vectorIndex],
				objectRef.ifHandle,
				&st
			);
		    	if (st != rpc_s_ok && st != ept_s_not_registered) {
				FREE_CONTEXT;
				exc_raise_status(st);
		    	}

		    	// if endpoint is registered, get out of loop
		    	if (st == rpc_s_ok)
		    		resolved = TRUE;
		}
		if (needToResolve)
			if (resolved) {
				objectRef.vectorIndex--;

				// make the handle failure timeout short
				rpc_mgmt_set_com_timeout(
					objectRef.objectBindings->
					       binding_h[objectRef.vectorIndex],
					rpc_c_binding_min_timeout,
					&st
				);
		    	} else
				continue;

		// get the object uuid associated with the binding handle
		rpc_binding_inq_object(
				objectRef.objectBindings->
					binding_h[objectRef.vectorIndex],
			       	&objectRef.objectId,
			       	&st
		);
		if (st != rpc_s_ok) {
			FREE_CONTEXT;
			exc_raise_status(st);
		}
		done = TRUE;
	}

	// end the query of the name space
	rpc_ns_binding_lookup_done(
		&import_context,
		&st
	);
	if (st != rpc_s_ok) exc_raise_status(st);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function performs a CDS lookup on the encapsulated 
// object uuid looking for a binding handle under the default 
// name space entry. If the binding handle is found, the 
// encapsulated object reference is updated. Otherwise, the
// object reference is left alone and this function returns.
//
// Input:
//	none
//
// Output:
//	boolean32 - TRUE if binding completed 
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

boolean32
rpc_object_reference::bindByUuid() {
	Trace("rpc_object_reference::bindByUuid()");

	rpc_ns_handle_t	import_context;
	boolean32       resolved;
	boolean32	done;
	unsigned32      ignore;
	unsigned32	st;
	char		*p;
	char		profile_name[CELL_NAME_LEN];

	// start the name space query at the default entry
	rpc_ns_binding_lookup_begin(
			rpc_c_ns_syntax_default,
			NULL,			// use RPC_DEFAULT_ENTRY
			objectRef.ifHandle,
			&objectRef.objectId,
			rpc_c_binding_max_count_default,
			&import_context,
			&st
	);
	if (st == rpc_s_no_env_setup ) {
		dce_cf_profile_entry_from_host(NULL, &p, &st);
		if (st != rpc_s_ok) exc_raise_status(st);
		sprintf(profile_name, "%s%s", CELL_DIR_ROOT, p);
		rpc_ns_binding_lookup_begin(
			rpc_c_ns_syntax_default,
			(unsigned_char_t *) profile_name,
			objectRef.ifHandle,
			&objectRef.objectId,
			rpc_c_binding_max_count_default,
			&import_context,
			&st
		);
	}
	if (st != rpc_s_ok && st != rpc_s_entry_not_found) {
		exc_raise_status(st);
	}

	// if name not found, return now
	if (st == rpc_s_entry_not_found) {
		return FALSE;
	}


	// loop through all entries and look for one that can be resolved
	done = FALSE;
	while (!done) {
		// start out with no proper binding handle
		objectRef.vectorIndex = -1;

		// get a binding vector
		rpc_ns_binding_lookup_next(
			import_context,
			&objectRef.objectBindings,
			&st
		);
		if (st != error_status_ok && st != rpc_s_no_more_bindings) {
			FREE_CONTEXT;
			exc_raise_status(st);
		}

		// if binding vector not found, get out
		if (st == rpc_s_no_more_bindings) {
			FREE_CONTEXT;
			return FALSE;
		}

		// got a binding vector so lets assume we will use 1st handle
		objectRef.vectorIndex = 0;

		for (resolved = FALSE;
			(!resolved &&
			      objectRef.vectorIndex <
				objectRef.objectBindings->count);
			objectRef.vectorIndex++) {

			rpc_ep_resolve_binding(
				objectRef.objectBindings->
					binding_h[objectRef.vectorIndex],
				objectRef.ifHandle,
				&st
			);
			if (st != rpc_s_ok && st != ept_s_not_registered) {
				FREE_CONTEXT;
				exc_raise_status(st);
			}

			// if endpoint  resolved, get out of loop
			if (st == rpc_s_ok)
				resolved = TRUE;

		}
		if (resolved) {
			done = TRUE;
			objectRef.vectorIndex--;

			// make the handle failure timeout short
			rpc_mgmt_set_com_timeout(
				objectRef.objectBindings->
					binding_h[objectRef.vectorIndex],
				rpc_c_binding_min_timeout,
				&st
			);
		}

	}

	// termintate name space query
	rpc_ns_binding_lookup_done(
		&import_context,
		&st
	);
	if (st != error_status_ok) exc_raise_status(st);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function just extracts the object uuid from the encapsulated 
// binding handle and encapsulates it.
//
// Input:
//	none
//
// Output:
//	boolean32 - TRUE if binding completed 
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

boolean32
rpc_object_reference::bindByHandle() {
	Trace("rpc_object_reference::bindByHandle()");

	unsigned32	st;

	rpc_binding_inq_object(
		objectRef.objectBindings->binding_h[objectRef.vectorIndex],
		&objectRef.objectId,
		&st
	);
	if (st != error_status_ok) exc_raise_status(st);

	return TRUE;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function binds to a named object in the CDS name space using
// the encapsulated binding information.
//
// Input:
//	none
//
// Output:
//	boolean32 - TRUE if binding completed 
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

boolean32
rpc_object_reference::cdsBind() {
	Trace("rpc_object_reference::cdsBind()");

	boolean32	nilUuid;
	unsigned32	st;

	// either bind by name, binding handle, or uuid
	if (objectRef.objectName && 
	    objectRef.objectName != SpecialName) {
		return bindByName();
	} else if (objectRef.objectBindings) {
		return bindByHandle();
	} else {
		nilUuid = uuid_is_nil(&objectRef.objectId, &st);
		if (st != rpc_s_ok) exc_raise_status(st);
		if (!nilUuid)
			return bindByUuid();
	}

	return FALSE; 
}

////////////////////////////////////////////////////////////////////////////////
//
// This function prints out the encapsulated information
// for debugging purposes.
//
// Input:
//	none
//
// Output:
//	none
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

#ifdef DEBUG

void
rpc_object_reference::print() const {
    	char		*uuid_string;
    	char		*bh_string;
	int		i;
	unsigned32	st;


	cout << endl << "*** Object Reference Printout ***" << endl;

	cout << "name: ";
	if (objectRef.objectName == (unsigned_char_t *) -1)
		cout << "special named object" << endl;
	else if (objectRef.objectName == (unsigned_char_t *) 0)
		cout << "unnamed object" << endl;
	else
		cout << (char *) objectRef.objectName << endl;
	
	cout << "object is implemented ";
	if (object_is_local)
		cout << "locally" << endl;
	else {
		cout << "remotely" << endl;
		cout << "rebind policy is ";
		switch (((OverFlow *) oflow)->rebindPolicy->policy) {
		case never_rebind:
			cout << "never_rebind";
			break;
		case attempt_rebind:
			cout << "attempt_rebind";
			break;
		case wait_on_rebind:
			cout << "wait_on_rebind";
			break;
		case attempt_rebind_n:
			cout << "attempt_rebind_n ";
	 		cout << ((OverFlow *) oflow)->rebindPolicy->number;
			break;
		default:
			cout << "unknown";
			break;
		}
		cout << endl;
	}

	cout << "reference count: " << referenceCount << endl;

	cout << "object binding handles: " << endl;
	if (objectRef.objectBindings) {
	    for (i = 0; i < objectRef.objectBindings->count; i++) {
	        cout << "  binding handle[" << i << "]: ";
		rpc_binding_to_string_binding(
			objectRef.objectBindings->binding_h[i],
			(unsigned_char_t **) &bh_string,
			&st
		);
		if (st != error_status_ok) exc_raise_status(st);
		cout << bh_string << endl;
    		rpc_string_free((unsigned_char_t **) &bh_string, &st);
	    }
	} else {
		cout << "none" << endl;
	}
	cout << "binding vector index: " << objectRef.vectorIndex << endl;

	cout << "object uuid: ";
    	uuid_to_string((uuid_t *)  &objectRef.objectId,
		       (unsigned_char_t **) &uuid_string,
		       &st);
	if (st != error_status_ok) exc_raise_status(st);
	cout << uuid_string  << endl << endl;
    	rpc_string_free((unsigned_char_t **) &uuid_string, &st);

	cout << "supported interfaces: " << endl;
	for (RpcInterfaceList *q = interfaceList; q; q = q->next) {
		uuid_to_string(
			&GET_INTERFACE_ID(q),
			(unsigned_char_t **) &uuid_string,
			&st
		);
		if (st != error_status_ok) exc_raise_status(st);
		cout << "  " << uuid_string  << endl;
		rpc_string_free((unsigned_char_t **) &uuid_string, &st);
	}
	cout << "***********************************" << endl;

	return;
}

#endif

////////////////////////////////////////////////////////////////////////////////
//
// This function encapsulates information for binding to a named object
// by its name. The actual binding is deferred until needed.
// A proxy is created from the information.
//
// Input:
//	name	 - the object's name
//	ifHandle - the RPC interface handle
//
// Output:
//	void *	- pointer to an object proxy
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

void *
rpc_object_reference::setBindInfo(
	const unsigned_char_t *name,
	const uuid_t 	      & iid
) {
	Trace("rpc_object_reference::setBindInfo(name, iid)");

	ObjectRef		oref;
	void			*obj;
	void			*clone;
	rpc_object_reference	*basePtr;
	boolean32		isNil;
	unsigned32		st;
	ProxyCreator 		proxyCreator;
	uuid_t 			&oid = basePtr->objectRef.objectId;

	// get the interface spec
	oref.ifHandle = CIfaceMgr::getCIFSpec(iid);

	// set object id to nil
	uuid_create_nil(&oref.objectId, &st);
	if (st != rpc_s_ok) exc_raise_status(st);

	// copy the name parameter to the ObjectRef
	if (! (oref.objectName = new unsigned_char_t [strlen((char *) name)+1]))
		RAISE(rpc_x_no_memory);

	strcpy((char *) oref.objectName, (char *) name);

	// fill pinging information into OverFlow area
	oref.overFlow		  = new OrefOverFlow;
	oref.overFlow->pingInfo   = 0;
	oref.overFlow->twr_vector = 0;

	// fill in priniciple information
	oref.principal = 0;

	// set the binding handle to null
	oref.objectBindings = 0;
	oref.vectorIndex    = -1;

	// find the proxy creator function and create the proxy
	proxyCreator = ProxyMgr::getProxyCreator(iid);
	obj = (*proxyCreator)(oref);
	basePtr = (rpc_object_reference *) (basecast *) obj;

	// try and resolve the binding
	if (!basePtr->cdsBind()) {
		delete basePtr;
		return 0;
	}

	// add to proxy table or reuse proxy only if object reference
	// from name space points to a unique object
	// 
	// if the object id is nil, then the name was registered
	// without an object id and we do not reuse proxys pointing
	// to an object with a name but no identity.

	isNil = uuid_is_nil(&basePtr->objectRef.objectId, &st);
	if (st != rpc_s_ok) exc_raise_status(st);
	if (isNil) return obj;

	// see if a proxy already exists
	clone = 
	    rpc_object_reference::get_object(basePtr->objectRef.objectId, iid);
	if (clone) {
		// free proxy just created and reuse the existing proxy
	        delete basePtr;
	        obj = clone;
		basePtr = (rpc_object_reference *) (basecast *) obj;
		basePtr->incRef();
	}

	// add the proxy to the Proxy Table
	basePtr->registerProxyInTable(basePtr->objectRef.objectId, iid, obj);

	return obj;
}

///////////////////////////////////////////////////////////////////////////////
//
// This function encapsulates information for binding to a named object
// by its uuiud. The actual binding is deferred until needed.
// The oject name is set to -1 to indicate that this is a special
// named object with no real name.
// A proxy is created from the information.
//
// Input:
//	u	 - the object's uuid
//	ifHandle - the RPC interface handle
//
// Output:
//	void *	- pointer to an object proxy
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

void *
rpc_object_reference::setBindInfo(
	const uuid_t & u,
	const uuid_t & iid
) {
	Trace("rpc_object_reference::setBindInfo(uuid_t, iid)");

	void			*obj;
	rpc_object_reference	*basePtr;
	ObjectRef		oref;
	boolean32		isNil;
	unsigned32		st;
	ProxyCreator 		proxyCreator;
	uuid_t 			&oid = oref.objectId;

	// see if there already is a proxy for this interface/object pair
	obj = rpc_object_reference::get_object(u, iid);

	if (!obj) {
		// get the interface spec
		oref.ifHandle = CIfaceMgr::getCIFSpec(iid);

		// set object id to the parameter value
		oref.objectId    = u;

		// set the name to indicate a named object with no real name
		oref.objectName = SpecialName;

		// fill pinging information into OverFlow area
		oref.overFlow		  = new OrefOverFlow;
		oref.overFlow->pingInfo   = 0;
		oref.overFlow->twr_vector = 0;

		// fill in priniciple information
		oref.principal = 0;

		// set the binding handle to null
		oref.objectBindings = 0;
		oref.vectorIndex    = -1;

		// find the proxy creator function and create the proxy
		proxyCreator = ProxyMgr::getProxyCreator(iid);
		obj = (*proxyCreator)(oref);
		basePtr = (rpc_object_reference *) (basecast *) obj;

		// try and resolve the binding
		if (basePtr->cdsBind()) {
			// add to proxy table only if object reference
			// from name space points to a unique object
			isNil = uuid_is_nil(&oid, &st);
			if (st != rpc_s_ok) exc_raise_status(st);
			if (!isNil) {
			    // add the proxy to the Proxy Table
			    basePtr->registerProxyInTable(oid, iid, obj);
			}
		} else {
			delete basePtr;
			obj = 0;
		}
	} else {
		// reusing the existing proxy so bump the reference count
		basePtr = (rpc_object_reference *) (basecast *) obj;
		basePtr->incRef();
	}

	return obj;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function encapsulates information for binding to a named object
// by its binding handle. The actual binding is deferred until needed.
// The oject name is set to -1 to indicate that this is a special
// named object with no real name.
// A proxy is created from the information.
//
// Input:
//	bh	 - the binding handle
//	ifHandle - the RPC interface handle
//
// Output:
//	void *	- pointer to an object proxy
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

void *
rpc_object_reference::setBindInfo(
	const handle_t bh,
	const uuid_t & iid
) {
	Trace("rpc_object_reference::setBindInfo(handle_t, iid)");

	void			*obj;
	rpc_object_reference	*basePtr;
	ObjectRef		oref;
	boolean32		isNil;
	unsigned32		st;
	ProxyCreator 		proxyCreator;
	uuid_t 			&oid = oref.objectId;

	// use the uuid from the handle
	rpc_binding_inq_object(bh, &oref.objectId, &st);
	if (st != rpc_s_ok) exc_raise_status(st);

	// see if there already is a proxy for this interface/object pair
	obj = rpc_object_reference::get_object(oid, iid);

	if (!obj) {
		// get the interface spec
		oref.ifHandle = CIfaceMgr::getCIFSpec(iid);

		// set the name to indicate a named object with no real name
		oref.objectName = SpecialName;
	
		// Use malloc rather than new so that we can 
		// call rpc_binding_vector_free() safely.
		oref.objectBindings = (rpc_binding_vector_p_t)
			malloc
				(sizeof(rpc_binding_vector_t));
		// copy the binding handle parameter
		rpc_binding_copy(bh, &oref.objectBindings->binding_h[0], &st);
		if (st != rpc_s_ok) exc_raise_status(st);
	
		// fill pinging information into OverFlow area
		oref.overFlow		  = new OrefOverFlow;
		oref.overFlow->pingInfo   = 0;
		oref.overFlow->twr_vector = 0;

		// fill in priniciple information
		oref.principal = 0;

		oref.objectBindings->count = 1;
		oref.vectorIndex	   = 0;

		// find the proxy creator function and create the proxy
		proxyCreator = ProxyMgr::getProxyCreator(iid);
		obj = (*proxyCreator)(oref);
		basePtr = (rpc_object_reference *) (basecast *) obj;

		// try and resolve the binding
		if (basePtr->cdsBind()) {
			// add to proxy table only if object reference
			// from name space points to a unique object
			isNil = uuid_is_nil(&oid, &st);
			if (st != rpc_s_ok) exc_raise_status(st);
			if (!isNil) {
			    // add the proxy to the Proxy Table
			    basePtr->registerProxyInTable(oid, iid, obj);
			}
		} else {
			delete basePtr;
			obj = 0;
		}
	} else {
		// reusing the existing proxy so bump the referenece count
		basePtr = (rpc_object_reference *) (basecast *) obj;
		basePtr->incRef();
	}

	return obj;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function encapsulates information for binding to a named object
// by another DCE RPC object. The information in the argument object is
// copied to the new object.
// A proxy is created from the information.
//
// Input:
//	base	 - the other interface pointer
//	ifHandle - the interface handle
//
// Output:
//	void *	- pointer to an object proxy
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

void *
rpc_object_reference::setBindInfo(
	const rpc_object_reference * base,
	const uuid_t 		   & iid
) {
	Trace("rpc_object_reference::setBindInfo(rpc_object_reference *, iid)");

	int			i;
	void			*obj;
	rpc_object_reference	*basePtr;
	ObjectRef		oref;
	unsigned32		st;
	ProxyCreator 		proxyCreator;
	uuid_t 			&oid = oref.objectId;

	oref.objectId   = base->objectRef.objectId;

	// see if there already is a proxy for this interface/object pair
	obj = rpc_object_reference::get_object(oid, iid);

	if (!obj) {

		// get the interface spec
		oref.ifHandle = CIfaceMgr::getCIFSpec(iid);

		oref.objectName = base->objectRef.objectName;

		// fill pinging information into OverFlow area
		oref.overFlow		  = new OrefOverFlow;
		oref.overFlow->pingInfo   = 0;
		oref.overFlow->twr_vector = 0;

		// fill in priniciple information
		oref.principal = 0;

		if (base->object_is_local)
			oref.objectBindings = 0;
		else {
			oref.objectBindings = (rpc_binding_vector_p_t)
						malloc
				(sizeof(rpc_binding_vector_t) +
				((base->objectRef.objectBindings->count - 1) *
				sizeof(rpc_binding_handle_t))
			);

			// copy the binding handle vector
			for (i = 0;
			     i < base->objectRef.objectBindings->count;
			     i++) {
				rpc_binding_copy(
					base->objectRef.objectBindings->
						binding_h[i],
					&oref.objectBindings->binding_h[i],
					&st
				);
				if (st != rpc_s_ok) exc_raise_status(st);
			}
			oref.objectBindings->count = 
				base->objectRef.objectBindings->count;
		}
	
		oref.vectorIndex = base->objectRef.vectorIndex;

		// find the proxy creator function and create the proxy
		proxyCreator = ProxyMgr::getProxyCreator(iid);
		obj = (*proxyCreator)(oref);
		basePtr = (rpc_object_reference *) (basecast *) obj;

		// add the proxy to the Proxy Table
		// Don't have to resolve the binding since the argument
		// is an object reference which must have been resolved
		// when it was created. 
		basePtr->registerProxyInTable(oid, iid, obj);

		// Since we may have a proxy to a dynamic object and we
		// are creating a new interface pointer to it,
		// bump the reference count in the server's object.
		// if fail, return 0 to indicate that the object does
		// not support this interface
		if (! basePtr->incRef(1) ) {
			delete basePtr;
			obj = 0;
		}
	} else {
		// reusing the existing proxy so bump the referenece count
		// and set the ifspec to the passed in argument
		basePtr = (rpc_object_reference *) (basecast *) obj;
		basePtr->incRef();
	}

	return obj;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function registers the object with the name service
// It will reuse an existing namespace identity if one exists and
// eliminate the current object identity if possible.  
//
// Input:
//	objPtr	- the object to register
//	name	- the name to register the object with
//	iid	- the interface id
//	replaceEndpoint	- if true, the endpoint mapper entry is replaced
//			  rather then add to. Default is TRUE.
//
// Output:
//	none
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

void
rpc_object_reference::registerObject(const void *objPtr,
				     const unsigned_char_t *name,
				     const uuid_t & iid,
				     const boolean32 replaceEndpoint) {
	Trace("rpc_object_reference::registerObject()");

	rpc_binding_vector_p_t	bvec;
	uuid_vector_t		uvec;
	error_status_t		st;
	uuid_t			oid;
	boolean32		wasMarshalled;

	// this is a non-proxy api
	if (!object_is_local) return;

	// get the interface spec
	objectRef.ifHandle = IfaceMgr::getIFSpec(iid);

	// copy the name parameter and interface handle to the ObjectRef
	if (objectRef.objectName)
		delete [] objectRef.objectName;
	if (! (objectRef.objectName = new unsigned_char_t
			[strlen((char *) name) + 1]))
		RAISE(rpc_x_no_memory);
	strcpy((char *) objectRef.objectName, (char *) name);

	// see if name/identity is already registered with the interface
	// if yes, give the object the identity from the namespace,
	// else use the encapsulated object identity
	//
	// Note that the object will already have an identity since
	// all DCE objects are given one when they are first created.
	// The point of this check is to see if we should switch to
	// a new identity since the user is registering the object
	// as a named object. If we have to switch, then we want to
	// eliminate the old identity only if it has not been marshalled
	// to some client application. (Don't want to invalidate any
	// outstanding references). Eliminating the old identity means 
	// to remove it from the Object Table.

	// if object has not been marshalled, save the id so we can delete it 
	if (!(wasMarshalled = has_been_marshalled()))
		oid = objectRef.objectId;

	// the FALSE argument says don't try to resolve the endpoint,
	// just see if a name exists for the object.
	// If result is TRUE, the encapsulated object identity will
	// have been updated with a new id which is why the old one
	// was saved previously.

	if (bindByName(FALSE)) {
		// remove the old identity if never marshalled before
		if (!wasMarshalled)
			objectTable[oid] = 0;

		// add the object to the Object Table with the new identity
		registerObjectInTable(objectRef.objectId);
	}

	// associate the object uuid with all the binding handles
	rpc_server_inq_bindings(&bvec, &st);
	if (st != error_status_ok) exc_raise_status(st);

	uvec.count = 1;
	uvec.uuid[0] = &objectRef.objectId;

	// export each supported interface for the object
	// under the object's name and update the endpoint map
	for (RpcInterfaceList *q = interfaceList; q; q = q->next) {

		rpc_ns_binding_export(
			rpc_c_ns_syntax_default,
			(unsigned_char_t *) name,
			q->ifHandle,
			bvec,
			&uvec,
			&st
		);
		if (st != error_status_ok) exc_raise_status(st);

		if (replaceEndpoint) {
    			rpc_ep_register(
                    		q->ifHandle, 
                    		bvec,
				&uvec,
                    		(unsigned_char_t *)"Named Object",
                    		&st
			);
		} else {
    			rpc_ep_register_no_replace(
                    		q->ifHandle, 
                    		bvec,
				&uvec,
                    		(unsigned_char_t *)"Named Object",
                    		&st
			);
		}
		if (st != error_status_ok) exc_raise_status(st);
	}

	// store the binding handles in the object
	objectRef.objectBindings = bvec;
	objectRef.vectorIndex    = 0;

	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function is the virtual destructor. It allows an object to be
// destroyed using a base class object.
//
// Input:
//	none
//
// Output:
//	none
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

rpc_object_reference::~rpc_object_reference() {
	Trace("rpc_object_reference::~rpc_object_reference");

	error_status_t	st;			// status code
	int		ts;			// pthread status

	// tell server to decrement its reference count if not named
	if (!object_is_local && !objectRef.objectName) {
		(*Refcnt_v0_0_c_epv.decRef) (
		    objectRef.objectBindings->binding_h[objectRef.vectorIndex],
		    objectRef.objectId
		);
	}

	// remove object entirely from Object Table
	objectTable[objectRef.objectId] = 0;

        // Free the object binding handle
	if (objectRef.objectBindings) {
		rpc_binding_vector_free(&objectRef.objectBindings, &st);
		if (st != error_status_ok) exc_raise_status(st);
	}
	
        // Free the object name string
	if (objectRef.objectName &&
	    objectRef.objectName != (unsigned_char_t *) -1) {
		delete [] objectRef.objectName;
	}

	// Set all fields to zero to avoid freeing things already freed
	uuid_create_nil(&objectRef.objectId, &st);
	if (st != rpc_s_ok) exc_raise_status(st);

	// delete pinging information structure
	if (objectRef.overFlow && objectRef.overFlow->pingInfo) {
		delete objectRef.overFlow->pingInfo;
		objectRef.overFlow->pingInfo = 0;
	}

	// free tower vector
	if (objectRef.overFlow && objectRef.overFlow->twr_vector) {
		rpc_tower_vector_free(
			&objectRef.overFlow->twr_vector,
			&st
		);
		if (st != rpc_s_ok) exc_raise_status(st);
		objectRef.overFlow->twr_vector = 0;
	}

	// delete principal information structure
	if (objectRef.principal) {
		delete objectRef.principal;
		objectRef.principal = 0;
	}

	// set binding vector index to invalid value
	objectRef.vectorIndex    	       = -1;

	objectRef.objectName     	       = 0;
	objectRef.ifHandle       	       = 0;

	if (oflow) {
		ts = pthread_mutex_destroy( & ((OverFlow *) oflow)->lockFlag);
		if (ts != 0) RAISE(rpc_x_coding_error);
		if (((OverFlow *) oflow)->rebindPolicy)
			delete ((OverFlow *) oflow)->rebindPolicy;
		delete oflow;
		oflow = 0;
	}

	// free the interface linked list
	RpcInterfaceList *p, *q;
	q = interfaceList;
	while (q) {
		p = q->next;
		delete q;
		q = p;
	}

	delete objectRef.overFlow;

	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function converts a binding handle and interface id
// into a 'this' pointer by extracting the object id from the
// binding handle and retrieving the object address using the
// object id and interface id from the Object Table.
//
// This function also puts the binding handle parameter in thread-specific
// storage for access by get_binding_handle();
//
// Input:
//	bh	- the binding handle
//	iid	- the interface id 
//
// Output:
//	void *  - the object's address
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

void *
rpc_object_reference::_getThis(
	const handle_t bh,       // the binding handle
	const uuid_t & iid       // interface uuid
) {
	Trace("rpc_object_reference::_getThis()");

	uuid_t		oid;			// object id
	error_status_t	st;			// status code
	int		ts;			// pthread status

	// put the binding handle into thread-specific storage
	ts = pthread_setspecific(rpc_object_reference__key,(pthread_addr_t) bh);
	if (ts != 0) RAISE(rpc_x_coding_error);

	// fill in the object id
	rpc_binding_inq_object(bh, &oid, &st);
	if (st != error_status_ok) exc_raise_status(st);

	return (void *) objectTable[oid][iid];
}

////////////////////////////////////////////////////////////////////////////////
//
// This function increments the reference count for the object.
// If the argument is TRUE and the object is remote and the object
// is not named, an RPC call is sent to the object to increment 
// its reference count.
// 
//
// Input:
//	flag which indicates if the operation should be performed
//        locally or remotely. Default is TRUE.
//
// Output:
//	the current reference count
//
// Errors:
//      if the server does not support the reference counting interface,
//        no action is taken.
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

unsigned long
rpc_object_reference::incRef(
	const boolean32 tell_server
) {

	Trace("rpc_object_reference::incRef()");
	
	unsigned long ret;

#ifdef DEBUG
	if (DEBUG_ON)
		print();
#endif
	if (tell_server && !object_is_local) {
	    if (!objectRef.objectName) {
		    return (*Refcnt_v0_0_c_epv.incRef) (
		      		objectRef.objectBindings->
					binding_h[objectRef.vectorIndex],
				objectRef.objectId,
				GET_INTERFACE_ID(&objectRef)
		    );
	    }
	    return referenceCount;
	} else {
		lock();
		ret = ++referenceCount;
		unlock();
		return ret;
	}
}

////////////////////////////////////////////////////////////////////////////////
//
// This function decrements the reference count for the object.
// If the count reaches zero and we are in the client and the object
// is not named, the server is notified that the object is no longer 
// needed by this client via the Reference Counting RPC interface. 
// If the server does not support the Reference Counting interface, 
// no action is taken.
//
// If the count reaches zero and we are in the server, the object is
// deleted.
//
// Input:
//	none
//
// Output:
//	none
//
// Errors:
//      if the server does not support the reference counting interface,
//        no action is taken.
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

unsigned long
rpc_object_reference::decRef() {

	Trace("rpc_object_reference::decRef()");

	unsigned long ret;
	
#ifdef DEBUG
	if (DEBUG_ON)
		print();
#endif
	ret = referenceCount;
	if (referenceCount > 0) {
	    lock();
	    ret = --referenceCount;
	    unlock();
	    if (ret == 0) {
		if (object_is_local) {			// server side
			// don't delete named objects
			if (!objectRef.objectName)
				delete this;
		} else {				// client side
			delete this;
		}
	    }
	}
	return ret;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function returns an object pointer from the Object Table.
//
// Input:
//	u	- the object's uuid
//
// Output:
//	void *	- the object pointer in the Object Table
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

void *
rpc_object_reference::get_object(const uuid_t &u) {
	Trace("rpc_object_reference::get_object(uuid_t)");
	
	return (void *) objectTable[u];
}

////////////////////////////////////////////////////////////////////////////////
//
// This function returns an object pointer from the Object Table.
//
// Input:
//	oid	- the object's uuid
//	iid	- the interface's uuid
//
// Output:
//	void *	- the object pointer in the Object Table
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

void *
rpc_object_reference::get_object(const uuid_t &oid, const uuid_t &iid) {
	Trace("rpc_object_reference::get_object(uuid_t, uuid_t)");
	
	return (void *) objectTable[oid][iid];
}

////////////////////////////////////////////////////////////////////////////////
//
// This function returns an ObjectRef for the object. If the encapsulated
// ObjectRef information indicates that the binding is incomplete, this
// routine completes the binding.
//
// Input:
//	none
//
// Output:
//	ObjectRef - an object reference structure
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

ObjectRef *
rpc_object_reference::get_reference() {
	Trace("rpc_object_reference::get_reference()");

#ifdef DEBUG
	if (DEBUG_ON)
		print();
#endif
	if (object_is_local)
		return &objectRef;

	if (bindingIncomplete())
		cdsBind();

#ifdef DEBUG
	if (DEBUG_ON)
		print();
#endif

	return &objectRef;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function returns a binding handle for the object.
//
// Input:
//	none
//
// Output:
//	 - the object's binding handle
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

handle_t       
rpc_object_reference::getHandle()
{
	Trace("rpc_object_reference::getHandle()");

	return (handle_t) objectRef.objectBindings->
				binding_h[objectRef.vectorIndex];
}


////////////////////////////////////////////////////////////////////////////////
//
// This function returns a binding handle for the object.
//
// Input:
//	pMutex	   - mutex for multi-threaded client access
//	retryCount - number of retrys for this rpc call
//	st	   - pointer to stub's error status 
//
// Output:
//	 - the object's binding handle
//	 - error status updated in client stub
//
// Errors:
//	if any rpc* call fails, an exception is raised
// 	or an error status is returned to the caller
//
////////////////////////////////////////////////////////////////////////////////

handle_t       
rpc_object_reference::getHandle(
	RPC_SS_THREADS_MUTEX_T *pMutex,		// provides for thread safeness
	unsigned32	&retryCount,		// thread's retry count
	error_status_t 	*st			// thread's return status 
) {
	Trace("rpc_object_reference::getHandle()");

	error_status_t 	st2;			// local status variable

	RPC_SS_THREADS_MUTEX_LOCK(pMutex);
	*st = rpc_s_ok;

	// if we have tried all handles in vector, indicate error status
	if (++retryCount > objectRef.objectBindings->count) {
		*st = rpc_s_no_more_bindings;
		RPC_SS_THREADS_MUTEX_UNLOCK(pMutex);
		return 0;
	}

	if (retryCount > 0) {
		// we are trying another handle because a previous one failed
		// so bump vector index and remove the endpoint from the handle
		// so that we get a fresh one from the endpoint map
		objectRef.vectorIndex = retryCount - 1;
		rpc_binding_reset(
			objectRef.objectBindings->
				binding_h[objectRef.vectorIndex],
			&st2
		);

		// make the handle failure timeout short
		rpc_mgmt_set_com_timeout(
			objectRef.objectBindings->
				binding_h[objectRef.vectorIndex],
			rpc_c_binding_min_timeout,
			&st2
		);
	}

	RPC_SS_THREADS_MUTEX_UNLOCK(pMutex);

	return (handle_t) objectRef.objectBindings->
				binding_h[objectRef.vectorIndex];
}

////////////////////////////////////////////////////////////////////////////////
//
// This function returns a binding handle for the object.
//
// Input:
//	pMutex	   - mutex for multi-threaded client access
//	numAttempts- number of times all handles in the vector have been tried
//	retryCount - number of retrys for this rpc call
//	st	   - pointer to stub's error status 
//
// Output:
//	 - the object's binding handle
//	 - error status updated in client stub
//
// Errors:
//	if any rpc* call fails, an exception is raised
// 	or an error status is returned to the caller
//
////////////////////////////////////////////////////////////////////////////////

handle_t       
rpc_object_reference::getHandle(
	RPC_SS_THREADS_MUTEX_T *pMutex,		// provides for thread safeness
	signed32	&numAttempts,		// number of vector retries
	signed32	&retryCount,		// thread's retry count
	error_status_t 	*st			// thread's return status 
) {
	Trace("rpc_object_reference::getHandle()");

	error_status_t 	st2;			// local status variable

	RPC_SS_THREADS_MUTEX_LOCK(pMutex);
	*st = rpc_s_ok;

	// increment thread's retry count
	retryCount++;

	// switch on proxy's rebind policy
	switch (((OverFlow *) oflow)->rebindPolicy->policy) {

	case never_rebind:			// do not retry
		if (retryCount > 0)
			retryCount = objectRef.objectBindings->count + 1;
		break;

	case wait_on_rebind:			// retry forever
		if (retryCount > objectRef.objectBindings->count)
			retryCount = 1;
		break;

	case attempt_rebind_n:			// retry a number of times
		if (retryCount > objectRef.objectBindings->count) {
			if (++numAttempts > 
			     ((OverFlow *) oflow)->rebindPolicy->number)
				retryCount =objectRef.objectBindings->count + 1;
			else
				retryCount = 1;
		}
		break;

	case attempt_rebind:			// retry until all handles fail
	default:
		break;
	}

	// if we have tried all handles in vector, indicate error status
	if (retryCount > objectRef.objectBindings->count) {
		*st = rpc_s_no_more_bindings;
		RPC_SS_THREADS_MUTEX_UNLOCK(pMutex);
		return 0;
	}

	if (retryCount > 0) {
		// we are trying another handle because a previous one failed
		// so bump vector index and remove the endpoint from the handle
		// so that we get a fresh one from the endpoint map
		objectRef.vectorIndex = retryCount - 1;
		rpc_binding_reset(
			objectRef.objectBindings->
				binding_h[objectRef.vectorIndex],
			&st2
		);

		// make the handle failure timeout short
		rpc_mgmt_set_com_timeout(
			objectRef.objectBindings->
				binding_h[objectRef.vectorIndex],
			rpc_c_binding_min_timeout,
			&st2
		);
	}

	RPC_SS_THREADS_MUTEX_UNLOCK(pMutex);

	return (handle_t) objectRef.objectBindings->
				binding_h[objectRef.vectorIndex];
}


////////////////////////////////////////////////////////////////////////////////
//
// This function gives an object an identity by creating a uuid for it.
//
// Input:
//	none
//
// Output:
//	the object's newly created uuid
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

inline
uuid_t
rpc_object_reference::set_identity() {
	Trace("rpc_object_reference::set_identity()");

	error_status_t st;

	// create the object's uuid
	uuid_create(&objectRef.objectId, &st);
	if (st != error_status_ok) exc_raise_status(st);

	return objectRef.objectId;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function registers an interface supported by an object 
// with the object base class.
//
// Input:
//	ptr	- the object's address within a hierarchy 
//	iid	- the interface id
//
// Output:
//	none	
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

void
rpc_object_reference::rpc_register_interface(
			void *ptr,
			const uuid_t & iid
) {
	Trace("rpc_object_reference::rpc_register_interface()");

	uuid_t		u;
	unsigned32	st;
	boolean32       isNull;


	// create a new linked list element with the interface id and pointer
	RpcInterfaceList *n = new RpcInterfaceList(ptr, 0, interfaceList);

	// add a new element onto the front of the linked list
	interfaceList = n;

	// get the interface spec from the Interface Manager
	n->ifHandle = IfaceMgr::getIFSpec(iid);

	// if ifspec is empty, create a thread that will update the
	// object with the ifspec when the Interface Manager makes
	// the ifspec available. This case will occur when a constructor
	// for an RPC static object is called before the Interface
	// Manager constructor.
	if (!n->ifHandle) {
		pthread_t     	defer;		// thread to update object
		int		ts;		// pthread status

		Ifspec_update_information *argList = 
			new Ifspec_update_information(iid, &n->ifHandle);

		// create the thread to update object ifspec when 
		// the interface manager makes the information available
		ts = pthread_create(
			&defer,
			pthread_attr_default,
			(pthread_startroutine_t)
				rpc_object_reference::ifspec_update,
			(pthread_addr_t) argList
		);
		if (ts != 0) RAISE(rpc_x_coding_error);
	}

	// create an object id if the object doesn't have one
	isNull =  uuid_is_nil(&objectRef.objectId, &st);
	if (st != error_status_ok) exc_raise_status(st);

	if (isNull) {
		set_identity();
		objectTable[objectRef.objectId] = this;
	}

	// register the interface for the object in the object table.
	objectTable[objectRef.objectId][iid] = ptr;

	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function updates an object's interface list when the ifspec
// needed is made available by the Interface Manager.
//
// Input:
//	argList	- pthread argument pointer containing the interface id
//		  and the address of the object's ifspec.
//
// Output:
//	none	
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

void
rpc_object_reference::ifspec_update(
	Ifspec_update_information *argList
) {
	Trace("rpc_object_reference::ifspec_update()");

	while ((*argList->ifHandle = IfaceMgr::getIFSpec(argList->iid)) == 0) {
		// give up the processor since the while condition will not 
		// become false until the main thread executes the Interface
		// Manager constructor
		pthread_yield();
	}

	delete argList;

	return; 				// actually a pthread_exit
}

////////////////////////////////////////////////////////////////////////////////
//
// This function stores an object hierarchy in the Object Table
//
// Input:
//	oid	- the object's identity uuid
//
// Output:
//	none	
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////
void
rpc_object_reference::registerObjectInTable(const uuid_t & oid)
{
	Trace("rpc_object_reference::registerObjectInTable()");

	uuid_t	iid;

	// store base class as first level entry
	objectTable[oid] = this;

	// store each supported interface part as a second level entry
	for (RpcInterfaceList *q = interfaceList; q; q = q->next) {
		iid = GET_INTERFACE_ID(q);
		objectTable[oid][iid] = q->ptr;
	}

	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function stores a proxy in the Proxy Table, The Proxy Table
// is the same thing as the object table, but the name implies it is
// used on the client side of a binding.
//
// Input:
//	oid	- the object uuid
//	iid	- the interface uuid
//	obj	- the proxy pointer
//
// Output:
//	none	
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

void
rpc_object_reference::registerProxyInTable(
	const uuid_t & oid,
	const uuid_t & iid,
	const void * obj
) {
	Trace("rpc_object_reference::registerProxyInTable()");

	// store base class as first level entry
	objectTable[oid] = this;

	// store proxy pointer as second level entry
	objectTable[oid][iid] = (void *) obj;

	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function prepares an object for marshalling by making sure all the
// object's interfaces are registered with the DCE RPC runtime and that there
// is a thread listening for incoming RPC calls.
//
// Input:
//	none
//
// Output:
//	none	
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

void
rpc_object_reference::prepare_to_marshall(
) {
	Trace("rpc_object_reference::prepare_to_marshall()");

	pthread_t	listener;		// listener thread
	error_status_t	st;			// status code
	int		ts;			// pthread status

	// register all the object's interfaces with the DCE RPC rutime 
	// each interface spec is on the object's interface list
	for (RpcInterfaceList *q = interfaceList; q; q = q->next) {
		if (!q->ifHandle)
			RAISE(rpc_x_no_server_stub);
		// register the interface
		rpc_server_register_if(q->ifHandle, NULL, NULL, &st);

		// if its already registered, that's ok
		if (st != error_status_ok && 
		    st != rpc_s_type_already_registered)
			exc_raise_status(st);
	}

	// query the DCE RPC runtime for a binding vector
	// and store it in the object reference
	rpc_server_inq_bindings(&objectRef.objectBindings, &st);

	// if there are no bindings, create some
	if (st != error_status_ok &&
	    st != rpc_s_no_bindings)
		exc_raise_status(st);

	// if no binding vector found, create one
	// and store it in the object reference
	// and create a listener thread
	//
	// This is the case where a client is marshalling out 
	// a local object reference and turning into a server.

	if (st == rpc_s_no_bindings) {
		// create a binding vector
        	rpc_server_use_all_protseqs(rpc_c_protseq_max_reqs_default,&st);
        	if (st != error_status_ok) exc_raise_status(st);

		// now retrieve the binding vector into the object
		rpc_server_inq_bindings(&objectRef.objectBindings, &st);
        	if (st != error_status_ok) exc_raise_status(st);

        	// create the thread to listen for requests
        	ts = pthread_create(
			&listener,
			pthread_attr_default,
			(pthread_startroutine_t)
			rpc_object_reference::rpc_ss_listener_thread,
		       	NULL
		);
		if (ts != 0) RAISE(rpc_x_coding_error);

	}

	// we have a local object so create a tower vector for marshalling
	rpc_twr_vec_from_binding_vec (
		objectRef.objectBindings,
		&objectRef.overFlow->twr_vector,
		&st
	);
        if (st != error_status_ok) exc_raise_status(st);

	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function is used by a listener thread to listen for incoming
// RPC calls.
//
// Input:
//	none
//
// Output:
//	none	
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

void
rpc_object_reference::rpc_ss_listener_thread(
	void
) {
	Trace("rpc_object_reference::rpc_ss_listener_thread()");

	error_status_t	st;			// status code

	// listen for calls
	rpc_server_listen(rpc_c_listen_max_calls_default, &st);

	// if already listening, that's ok
        if (st != error_status_ok &&
	    st != rpc_s_already_listening)
		exc_raise_status(st);

    	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function set authentication and authorization informations for
// all the binding handles encapsulated in the object.
//
// Input:
//	server_princ_name - server principal name
//	protect_level	  - protection level
//	authn_svc	  - authentication protocol
//	auth_identity	  - authentication identity
//	authz_svc	  - authorization protocol
//
// Output:
//	none	
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

void
rpc_object_reference::secure(
	unsigned_char_t *	server_princ_name,
	unsigned32		protect_level,
	unsigned32	 	authn_svc,
	rpc_auth_identity_handle_t auth_identity,
	unsigned32		authz_svc
) {
	Trace("rpc_object_reference::secure()");

	// this is a proxy api
	if (object_is_local) return;

	unsigned_char_t	*princ_name = 0;	// server principal name
	error_status_t	st;			// status code

	for (int i = 0; i < objectRef.objectBindings->count; i++) {
		if (server_princ_name == 0) {
			rpc_mgmt_inq_server_princ_name(
				objectRef.objectBindings->binding_h[i],
				authn_svc,
				&princ_name,
				&st
			);
        		if (st != error_status_ok) exc_raise_status(st);
		}

		rpc_binding_set_auth_info(
			objectRef.objectBindings->binding_h[i],
			(server_princ_name ? server_princ_name : princ_name),
			protect_level,
			authn_svc,
			auth_identity,
			authz_svc,
			&st
		);
        	if (st != error_status_ok) exc_raise_status(st);

		if (princ_name) {
    			rpc_string_free(&princ_name, &st);
        		if (st != error_status_ok) exc_raise_status(st);
			princ_name = 0;
		}
	}

	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function creates the key used to store the binding handle
// in thread-specific storage.
//
// Input:
//	none
//
// Output:
//	none	
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

inline void
rpc_object_reference::get_thread_key() {

	Trace("rpc_object_reference::get_thread_key()");

	int		ts;			// pthread status

	ts = pthread_keycreate(&rpc_object_reference__key, 0);
	if (ts != 0) RAISE(rpc_x_coding_error);
}

////////////////////////////////////////////////////////////////////////////////
//
// This function returns the binding handle stored in the thread-specific
// storage area for the RPC call.
//
// Input:
//	none
//
// Output:
//	handle_t	
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

handle_t
rpc_object_reference::get_binding_handle() {
	Trace("rpc_object_reference::get_binding_handle()");

	handle_t	bh;			// binding handle
	int		ts;			// pthread status

	ts = pthread_getspecific(
		rpc_object_reference__key,
		(pthread_addr_t*) &bh
	);
	if (ts != 0) RAISE(rpc_x_coding_error);

	return bh;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function returns TRUE if this object reference has been marshalled
// previously.
//
// Input:
//	none
//
// Output:
//	boolean32 - TRUE if object has previously been marshalled	
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

boolean32
rpc_object_reference::has_been_marshalled() {
	Trace("rpc_object_reference::has_been_marshalled()");

	return objectRef.objectBindings != 0;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function returns TRUE if the object is implemented locally
//
// Input:
//	none
//
// Output:
//	boolean32 - TRUE if this is a real object as opposed to a proxy
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

boolean32
rpc_object_reference::is_local() {
	Trace("rpc_object_reference::is_local()");

	return object_is_local;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function locks an object
//
// Input:
//	none
//
// Output:
//	none
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

void
rpc_object_reference::lock() {
	Trace("rpc_object_reference::lock()");

	int		ts;			// pthread status

	ts = pthread_mutex_lock(& ((OverFlow *) oflow)->lockFlag);
	if (ts != 0) RAISE(rpc_x_coding_error);

	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function unlocks an object
//
// Input:
//	none
//
// Output:
//	none
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

void
rpc_object_reference::unlock() {
	Trace("rpc_object_reference::unlock()");

	int		ts;			// pthread status

	ts = pthread_mutex_unlock(& ((OverFlow *) oflow)->lockFlag);
	if (ts != 0) RAISE(rpc_x_coding_error);

	return;
}


////////////////////////////////////////////////////////////////////////////////
//
// This function sets the rebind policy for a proxy
//
// Input:
//	DCERebindPolicy	- rebind policy
//	unsigned32	- used with attempt_rebind_n, the value of n
//
// Output:
//	none
//
// Errors:
//	if any rpc* call fails, an exception is raised
//
////////////////////////////////////////////////////////////////////////////////

void
rpc_object_reference::SetRebind(
	DCERebindPolicy p,
	unsigned32	n
) {
	Trace("rpc_object_reference::SetRebind()");

	// this is a proxy api
	if (object_is_local) return;

	((OverFlow *) oflow)->rebindPolicy->policy = p;
	((OverFlow *) oflow)->rebindPolicy->number = n;

	return;
}



