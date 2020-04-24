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
// $Log: ndrmi6.cxx,v $
// Revision 1.1.2.2  1996/03/09  20:45:07  marty
// 	Add OSF copyright
// 	[1996/03/09  20:24:45  marty]
//
// Revision 1.1.2.1  1995/12/08  00:13:13  root
// 	Submit OSF/DCE 1.2.1
// 
// 	HP revision /main/dat_xidl2/1  1995/11/17  17:09 UTC  dat
// 	New file for second XIDL drop for DCE 1.2.1
// 	[1995/12/08  00:03:19  root]
// 
// Revision 1.1.2.1  1995/10/23  03:21:29  bfc
// 	oct 95 idl drop
// 	[1995/10/23  02:12:47  bfc]
// 
// 	may 95 idl drop
// 	[1995/10/23  02:11:33  bfc]
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
//  NAME:
//
//      ndrmi6.cxx
//
//  FACILITY:
//
//      Interface Definition Language (IDL) Compiler
//
//  ABSTRACT:
//
//      NDR marshalling routines for - Interface Pointers
//
//

#ifdef _MSDOS
#define GUID_DEFINED 
#define GUID uuid_t
#endif
#include <dce/idlddefs.h>
#include <dce/objref.hxx>
#include <dce/ifacemgr.hxx>
#include <dce/trace.hxx>

#include <orefdef.h>
#include <refcnt.h>
#include <ndrmi.h>
#include <lsysdep.h>


#ifdef DEBUG
#include <iostream.h>
#define DEBUG_ON getenv("DEBUG")
#endif


// Function Declarations

void rpc_ss_ndr_object_to_oref(
	void *,
	ObjectRef *,
	uuid_t &
);


static OBJREF *allocate_wire_data_structure(
	IDL_msp_t
);

void bump_reference_count(
	void *,
	ObjectRef &,
	IDL_msp_t
);

static void put_wire_data_structure(
	OBJREF *,
	IDL_msp_t
);

static void rpc_ss_tower_vector_to_tower_stream(
	TWRREP *,
	rpc_tower_vector_p_t,
	IDL_msp_t
);


#ifdef DEBUG

static void print_vector(
	rpc_binding_vector_p_t
);

#endif


// undefine the epv macros for these routines since
// we use the real names in this module

#ifdef rpc_ss_ndr_marsh_interface
#undef rpc_ss_ndr_marsh_interface
#endif

#ifdef rpc_ss_ndr_marsh_dyn_interface
#undef rpc_ss_ndr_marsh_dyn_interface
#endif

class IDL_marshEPV {
public:
	IDL_marshEPV();
};

IDL_marshEPV::IDL_marshEPV()
{
	// add all functions to the entry point vector here
	rpc_ss_idl_mi_epv[RPC_SS_NDR_MARSH_INTERFACE] =
		(rpc_ss_ndr_func_p_t) rpc_ss_ndr_marsh_interface;

	rpc_ss_idl_mi_epv[RPC_SS_NDR_MARSH_DYN_INTERFACE] = 
		(rpc_ss_ndr_func_p_t) rpc_ss_ndr_marsh_dyn_interface;
}

// the next line causes the epv to be built via a constructor
IDL_marshEPV rpc_ss_idl_marshEPV;


////////////////////////////////////////////////////////////////////////////////
//
// Function to drive the marshalling of an interface pointer
// This routine is called from the marshalling interpreter C modules
// which necessitates the extern around this definition.
//
////////////////////////////////////////////////////////////////////////////////

extern "C" {

void
rpc_ss_ndr_marsh_interface(
	idl_ulong_int	defn_index,		// index into type vector 
	void		*param_addr,		// address of parameter
	IDL_msp_t     	IDL_msp			// marshalling state
) {
	Trace("rpc_ss_ndr_marsh_interface()");

	idl_byte	*defn_vec_ptr;		// definition vector ptr
	idl_ulong_int	routine_index;		// index of routine group 
	uuid_t		iid;			// interface uuid
	
	// point to entry in the definition vector for this parameter
	defn_vec_ptr = IDL_msp->IDL_type_vec + defn_index;

	// get Interface UUID
	IDL_GET_UUID_FROM_VECTOR(iid, defn_vec_ptr);

	// Get a pointer to the routines for this type
	IDL_GET_LONG_FROM_VECTOR(routine_index, defn_vec_ptr);

	rpc_ss_ndr_marsh_dyn_interface(
		routine_index,			// index into routine vector 
		param_addr,			// address of parameter
		&iid,				// interface id
		IDL_msp				// marshalling state
	);

	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function does the work of marshalling an interface pointer.
// This routine is called from the marshalling interpreter C modules
// which necessitates the extern around this definition.
//
////////////////////////////////////////////////////////////////////////////////

void
rpc_ss_ndr_marsh_dyn_interface(
	idl_ulong_int	routine_index,		// index into type vector 
	void		*param_addr,		// address of parameter
	uuid_t		*piid,			// pointer to interface id
	IDL_msp_t     	IDL_msp			// marshalling state
) {
	Trace("rpc_ss_ndr_marsh_dyn_interface()");

	ObjectRef	oref;			// object reference information
	OBJREF		*wire;   		// sent over wire
	void		(**routine_ptr)(...);	// converts to wire rep
	error_status_t	st;			// error status
	

	// convert the object to an ObjectRef structure
		rpc_ss_ndr_object_to_oref(param_addr, &oref, *piid);

	// allocate and initialize space for the wire data structure
	wire = allocate_wire_data_structure(IDL_msp);

	// Get a pointer to the routines for this type
	routine_ptr = ( void (**)(...)) IDL_msp->IDL_rtn_vec + routine_index;

	// convert the ObjectRef structure to a wire format
	(*(routine_ptr + IDL_RTN_OREF_TO_WIRE))(&oref, wire, IDL_msp);

	// since we are marshalling out a new reference,
	// bump the reference count for this object
	bump_reference_count(param_addr, oref, IDL_msp);

	// marshall the wire data onto the wire
	put_wire_data_structure(wire, IDL_msp);

	// Do NOT free the wire format data structure
	//
	// Since the data structures were allocated with rpc_ss_mem_alloc()
	// they will automatically be freed by the stub after the RPC call
	// has returned to the user. The reason for deferring freeing the
	// data structures is because they could potentially be 'marshalled
	// by pointing' rather than 'marshalled by copying' depending on how
	// big they are, and freeing a buffer that is pointed to by the
	// i/o vector would cause errors.

	return;
}

}	// end of extern "C"

////////////////////////////////////////////////////////////////////////////////
//
// Convert an interface pointer to an ObjectRef structure
//
////////////////////////////////////////////////////////////////////////////////

void
rpc_ss_ndr_object_to_oref(
	void            *param_addr,		// pointer to object address
	ObjectRef       *oref,        	 	// object reference information
	uuid_t          &iid			// interface id
) {
	Trace("rpc_ss_ndr_object_to_oref()");

	rpc_object_reference	*object;	// ptr to object's base class
	error_status_t	st;			// error status

	// get object's base class pointer
	object = (rpc_object_reference *) (* (basecast **) param_addr);

	// if object is null, return an empty object reference
	if (object == 0) {
		uuid_create_nil(&oref->objectId, &st);
	    	if (st != error_status_ok) exc_raise_status(st);
		oref->objectBindings = 0;
	    	oref->ifHandle       = 0;
		oref->objectName     = 0;
		oref->overFlow	     = 0;
		oref->principal	     = 0;
		return;
	}

	// if its a local object, prepare it for marshalling
	if (object->is_local()) {
		// object is local - prepare it for marshalling if necessary
		if (!object->has_been_marshalled()) {
			object->prepare_to_marshall();
		}
	}

	// store this object's reference information into the ObjectRef struct
	*oref = *object->get_reference();

	return;
}


////////////////////////////////////////////////////////////////////////////////
//
// Convert an object reference to a wire representation
//
////////////////////////////////////////////////////////////////////////////////

void
rpc_ss_ndr_oref_to_wire_rep(
	void		*pOref,			// ptr to an object reference
	void		*pWire,			// ptr to wire data structure
	IDL_msp_t	IDL_msp			// marshalling state
) {
	Trace("rpc_ss_ndr_oref_to_wire_rep()");

	// get pointers to the real data types
	OBJREF		*wire = (OBJREF *)    pWire;
	ObjectRef     	*oref = (ObjectRef *) pOref;

	// initialize the common fields of the structure
	wire->flags	= OBJREF_TWR;
	wire->mbz	= 0;

	// if the object reference is null, return null wire data
	// wire data was initialized by the allocation routine
	if (!oref->objectBindings)
		return;

	// fill in the object id
	wire->u_objref.twr.oid	   = oref->objectId;

	// fill in the object tower vector
	rpc_ss_tower_vector_to_tower_stream(
		&wire->u_objref.twr,
		oref->overFlow->twr_vector,
		IDL_msp
	);

	return;
}


////////////////////////////////////////////////////////////////////////////////
//
// Bump the reference count for the object if appropriate
//
////////////////////////////////////////////////////////////////////////////////

void
bump_reference_count(
	void 			*param_addr,	// address of object pointer
	ObjectRef		&oref,		// object reference information
	IDL_msp_t		IDL_msp		// marshalling state
) {
	Trace("bump_reference_count()");

	rpc_object_reference	*object;	// pointer to object base class
	error_status_t	st;			// error status


	// get hold of base class part of the object
	object = (rpc_object_reference *) (* (basecast **) param_addr);

	if (object != 0) {
		// register the reference counting interface
		// if we are marshalling a dynamic object
		if (oref.objectName == 0) {
        		rpc_server_register_if(
                    		Refcnt_v0_0_s_ifspec, 
                    		NULL,
		    		NULL,
		    		&st);
       	        	if (st != error_status_ok && 
		        	    st != rpc_s_type_already_registered)
				exc_raise_status(st);
		}
		// if marshalling an object as an [out] parameter,
		// increment the refernece count for the object
		if (IDL_msp->IDL_side == IDL_server_side_k)
				object->incRef(TRUE);
	}
	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// Convert a tower vector into a stream of towers for the wire
//
////////////////////////////////////////////////////////////////////////////////

static void
rpc_ss_tower_vector_to_tower_stream(
	TWRREP			*twrStream,	// tower stream
	rpc_tower_vector_p_t	twr_vector,	// tower vector to convert
	IDL_msp_t		IDL_msp		// marshalling state
) {
	Trace("rpc_ss_tower_vector_to_tower_stream()");

	twr_t			*tower;		// tower pointer
	int			i;		// loop counters
	unsigned		size;		// memory size to hold towers 
	unsigned32		st;		// status variable

	// calculate space needed for array of pointers
	size = twr_vector->count * sizeof(twr_p_t *);

	// allocate space to the wire data structure for an array of pointers
	twrStream->tower = (twr_p_t *) rpc_ss_mem_alloc(
				&IDL_msp->IDL_mem_handle,
				size
	);

	// assign count to wire data structure
	twrStream->count = twr_vector->count;

	// copy tower data to local storage and assign to wire data structure
	// 
	// Memory allocated by rpc_ss_mem_alloc() will automatically be
	// freed by the stub after the rpc call parameters have been
	// transmitted. But memory allocated by the tower SPI must be freed
	// here so as to avoid a memory leak. So each tower is copied
	// to memory allocated by rpc_ss_mem_alloc() in this loop.

	for (i = 0; i < twr_vector->count; i++) {
		// get size of a tower
		size = sizeof(twr_t) +
		       (twr_vector->tower[i]->tower_length - 1) *
		       sizeof(idl_byte);

		// allocate local memory to hold a tower
		tower = (twr_t *) rpc_ss_mem_alloc(
				&IDL_msp->IDL_mem_handle,
				size
		);

		// copy tower length
		tower->tower_length = twr_vector->tower[i]->tower_length;

		// copy tower data
		memcpy(
			tower->tower_octet_string, 
			twr_vector->tower[i]->tower_octet_string,
			tower->tower_length
		);

		// assign local tower address to wire data structure
		twrStream->tower[i] = tower;
	}

	return;

}

////////////////////////////////////////////////////////////////////////////////
//
// Allocate space for and initialize a wire data structure
//
////////////////////////////////////////////////////////////////////////////////

static OBJREF *
allocate_wire_data_structure(
	IDL_msp_t       	IDL_msp		// marshalling state
) {
	Trace("allocate_wire_data_structure()");

	OBJREF          	*wire;		// ptr to wire data structure
	unsigned32		st;		// status variable

	// use rpc_ss_mem_alloc() so that allocated memory can be
	// freed by the stub automatically after it is transmitted
	// to the client.
	wire = (OBJREF *) rpc_ss_mem_alloc(
				&IDL_msp->IDL_mem_handle,
				sizeof(OBJREF)
	);

	// init all fields in the union part of the wire data structure
	wire->u_objref.twr.period  = 0;
	wire->u_objref.twr.princid = 0;
	wire->u_objref.twr.count   = 0;
	wire->u_objref.twr.tower   = 0; 

	uuid_create_nil(&wire->u_objref.twr.oid, &st);
	if (st != error_status_ok) exc_raise_status(st);

	return wire;
}

////////////////////////////////////////////////////////////////////////////////
//
// Marshall the wire data structure onto the wire
//
////////////////////////////////////////////////////////////////////////////////

static void
put_wire_data_structure(
	OBJREF			*wire,		// wire data structure
	IDL_msp_t		IDL_msp		// marshalling state
) {
	Trace("put_wire_data_structure()");

	idl_ulong_int		xmit_defn_index;// index into definition vector
	idl_ulong_int		*sav_off_vec;	// save variable
	idl_byte		*sav_typ_vec;	// save variable

	// Marshall transmitted type

	// save the type/offset vector pointers
	sav_off_vec = IDL_msp->IDL_offset_vec;
	sav_typ_vec = IDL_msp->IDL_type_vec;

	// swap in the type/offset vector for an interface pointer
	IDL_msp->IDL_offset_vec = interface_offset_vec;
	IDL_msp->IDL_type_vec   = interface_type_vec;

	// marshall the wire data structure onto the wire
	xmit_defn_index = INTERFACE_XMIT_INDEX;
        rpc_ss_ndr_marsh_struct(
		IDL_DT_FIXED_STRUCT,
		xmit_defn_index,
                (rpc_void_p_t) wire,
		IDL_msp
	);

	// marshall any embedded pointers onto the wire
	rpc_ss_ndr_m_struct_pointees(
		IDL_DT_FIXED_STRUCT,
		xmit_defn_index,
		(rpc_void_p_t) wire,
		IDL_msp
	);

	// restore saved values
	IDL_msp->IDL_offset_vec = sav_off_vec;
	IDL_msp->IDL_type_vec   = sav_typ_vec;

	return;
}




#ifdef DEBUG

////////////////////////////////////////////////////////////////////////////////
//
// Print out a binding vector for debugging
//
////////////////////////////////////////////////////////////////////////////////

static void
print_vector(
	rpc_binding_vector_p_t bvec		// binding vector
) {
	Trace("print_vector");

	unsigned_char_t *s;                     // string for of handle
	unsigned32      st;                     // status code

	cout << "marshalling binding vector:" << endl;

	for (int i = 0; i < bvec->count; i++) {
		rpc_binding_to_string_binding(bvec->binding_h[i], &s, &st);
        	if (st != error_status_ok) exc_raise_status(st);

		cout << "bvec[" << i << "]: " << (char *) s << endl;

		rpc_string_free(&s, &st);
        	if (st != error_status_ok) exc_raise_status(st);
	}
}

#endif

