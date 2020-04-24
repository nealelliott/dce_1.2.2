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
// $Log: ndrui6.cxx,v $
// Revision 1.1.2.2  1996/03/09  20:45:09  marty
// 	Add OSF copyright
// 	[1996/03/09  20:24:46  marty]
//
// Revision 1.1.2.1  1995/12/08  00:13:14  root
// 	Submit OSF/DCE 1.2.1
// 
// 	HP revision /main/dat_xidl2/1  1995/11/17  17:09 UTC  dat
// 	New file for second XIDL drop for DCE 1.2.1
// 	[1995/12/08  00:06:27  root]
// 
// Revision 1.1.2.2  1995/11/02  15:24:11  bfc
// 	viv: initialize ifhandle in oref before creating proxy
// 	[1995/11/02  15:22:08  bfc]
// 
// Revision 1.1.2.1  1995/10/23  03:21:31  bfc
// 	oct 95 idl drop
// 	[1995/10/23  02:16:05  bfc]
// 
// 	may 95 idl drop
// 	[1995/10/23  02:15:11  bfc]
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
//      ndrui6.cxx
//
//  FACILITY:
//
//      Interface Definition Language (IDL) Compiler
//
//  ABSTRACT:
//
//      NDR unmarshalling routines for - Interface Pointers
//
//

#include <dce/idlddefs.h>
#include <dce/objref.hxx>
#include <dce/trace.hxx>
#include <dce/proxymgr.hxx>
#include <dce/ifacemgr.hxx>
#include <orefdef.h>
#include <ndrui.h>
#include <lsysdep.h>


#ifdef DEBUG
#include <iostream.h>
#define DEBUG_ON getenv("DEBUG")
#define DEBUG_COMMEM getenv("DEBUGCOMMEM")
#define ENABLE_COMMEM getenv("ENABLECOMMEM")
#endif

// Function Declarations


void rpc_ss_ndr_oref_to_object(
	ObjectRef *,
	rpc_object_reference **,
	uuid_t &
);


static void rpc_ss_tower_stream_to_binding_vector(
	TWRREP *,
	rpc_binding_vector_p_t *,
	rpc_tower_vector_p_t *
);

static OBJREF *allocate_wire_data_structure(
	void *,
	IDL_msp_t
);

static void get_wire_data_structure(
	OBJREF *,
	IDL_msp_t
);

static void free_wire_data_structure(
	OBJREF *,
	void *,
	IDL_msp_t
);



#ifdef DEBUG

static void print_vector(
	rpc_binding_vector_p_t
);

#endif

// C to C++ entry point vector

// undefine the epv macros for these routines since
// we use the real names in this module

#ifdef rpc_ss_ndr_unmar_interface
#undef rpc_ss_ndr_unmar_interface
#endif

#ifdef rpc_ss_ndr_unmar_dyn_interface
#undef rpc_ss_ndr_unmar_dyn_interface
#endif

class IDL_unmarEPV {
public:
	IDL_unmarEPV();
};

IDL_unmarEPV::IDL_unmarEPV()
{
	// add all functions to the entry point vector here
	rpc_ss_idl_mi_epv[RPC_SS_NDR_UNMAR_INTERFACE] =
		(rpc_ss_ndr_func_p_t) rpc_ss_ndr_unmar_interface;

	rpc_ss_idl_mi_epv[RPC_SS_NDR_UNMAR_DYN_INTERFACE] = 
		(rpc_ss_ndr_func_p_t) rpc_ss_ndr_unmar_dyn_interface;
}

// the next line causes the epv to be built via a constructor
IDL_unmarEPV rpc_ss_idl_unmarEPV;


////////////////////////////////////////////////////////////////////////////////
//
// Function to drive the unmarshalling of an interface pointer.
// This routine is called from the marshalling interpreter C modules 
// which necessitates the extern around this function definition.
//
////////////////////////////////////////////////////////////////////////////////

extern "C" {

void
rpc_ss_ndr_unmar_interface(
	idl_ulong_int defn_index,		// index into type vector
	void         *param_addr,		// address of parameter
	void         *xmit_data_buff,		// address to unmarshall into
	IDL_msp_t     IDL_msp			// marshalling state
) {
	Trace("rpc_ss_ndr_unmar_interface()");

	idl_ulong_int           routine_index;  // index in routine vector
	idl_byte                *defn_vec_ptr;  // definition vector pointer
	uuid_t                  iid;            // interface id

	// get pointer to entry in definition vector for this parameter
	defn_vec_ptr = IDL_msp->IDL_type_vec + defn_index;

	// get Interface UUID
	IDL_GET_UUID_FROM_VECTOR(iid, defn_vec_ptr);

	// Get a pointer to the routines for this type
	IDL_GET_LONG_FROM_VECTOR( routine_index, defn_vec_ptr );

	rpc_ss_ndr_unmar_dyn_interface(
    		routine_index,			// index into routine vector
    		param_addr,			// parameter being unmarshalled
		&iid,				// interface id
    		xmit_data_buff,			// buffer to unmarshall into
    		IDL_msp				// idl state parameter
	);

	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// This function does the work of unmarshalling an interface pointer.
// This routine is called from the marshalling interpreter C modules 
// which necessitates the extern around this function definition.
//
////////////////////////////////////////////////////////////////////////////////

void
rpc_ss_ndr_unmar_dyn_interface(
	idl_ulong_int	routine_index,		// index into type vector
	void         	*param_addr,		// address of parameter
	uuid_t       	*piid,			// pointer to interface id
	void        	*xmit_data_buff,	// address to unmarshall into
	IDL_msp_t     	IDL_msp			// marshalling state
) {
	Trace("rpc_ss_ndr_unmar_dyn_interface()");

	ObjectRef       oref;			// object reference information
	void 		(**routine_ptr)(...); 	// pointer to routine group
	OBJREF		*wire;			// wire data structure

	// Get a pointer to the routines for this interface
	routine_ptr = (void (**)(...)) (IDL_msp->IDL_rtn_vec + routine_index);

	// Get space to unmarshall the transmitted data into
	wire = allocate_wire_data_structure(xmit_data_buff, IDL_msp);

	// Fill in the wire data structure
	get_wire_data_structure(wire, IDL_msp);

	// if NULL was passed as an interface pointer, 
	// free the allocated buffer and return 0 as the interface pointer
	if (wire->u_objref.twr.count == 0) {
		* ((void **) param_addr) = 0;
		free_wire_data_structure(wire, xmit_data_buff, IDL_msp);
		return;
	}

	// convert the wire data to an ObjectRef structure
	(*(routine_ptr+IDL_RTN_WIRE_TO_OREF)) (wire, &oref);
	oref.ifHandle = CIfaceMgr::getCIFSpec(*piid);

	// create the proxy from the ObjectRef structure
		rpc_ss_ndr_oref_to_object(
			&oref,
			(rpc_object_reference **) param_addr,
			*piid
		);

	if (oref.overFlow->pingInfo)
        	delete oref.overFlow->pingInfo;
	delete oref.overFlow;
	// free the allocated wire data structure
	free_wire_data_structure(wire, xmit_data_buff, IDL_msp);
	return;
}

}	// end of extern "C" statement

////////////////////////////////////////////////////////////////////////////////
//
// Convert a wire representation to an ObjectRef structure
//
// Note that void * parameters are used so that stubs can be
// insulated from internal type definitions.
//
////////////////////////////////////////////////////////////////////////////////

void
rpc_ss_ndr_wire_rep_to_oref(
	void        	*pWire,			// ptr to wire data
	void		*pOref			// ptr to an object reference
) {
	Trace("rpc_ss_ndr_wire_rep_to_oref()");

	// get pointers to the real data types
	ObjectRef       *oref = (ObjectRef *) pOref;
	OBJREF          *wire = (OBJREF *)    pWire;

	// fill in ping period
	oref->overFlow		 = new OrefOverFlow;

        oref->overFlow->pingInfo = 0;

	// fill in principal id
	oref->principal = 0;

	// fill in the object id
	oref->objectId = wire->u_objref.twr.oid;

	// initialize the rest of the data structure
	oref->ifHandle    = 0;
	oref->objectName  = 0;
	oref->vectorIndex = 0;

	// build the binding vector and tower vector from the tower stream
	rpc_ss_tower_stream_to_binding_vector(
		&wire->u_objref.twr,
		&oref->objectBindings,
		&oref->overFlow->twr_vector
	);

	
	return;
}


////////////////////////////////////////////////////////////////////////////////
//
// Convert an ObjectRef structure to an interface pointer
//
////////////////////////////////////////////////////////////////////////////////

void
rpc_ss_ndr_oref_to_object(
	ObjectRef		*oref,		// ptr to an object reference
	rpc_object_reference	**object,	// addr to store interface ptr
	uuid_t			&iid		// interface id reference
) {
	Trace("rpc_ss_ndr_oref_to_object");

	ProxyCreator            proxyCreator;	// proxy creator function
	rpc_object_reference 	*basePtr;	// base class pointer

	// get the base class pointer for this object
	//
	// Note that if the object is local then we will find it in
	// the Object Table since all DCE objects are automatically
	// registered there. If we do not find it, then we can assume
	// that we are looking for a Proxy.

	*object = (rpc_object_reference *) 
		rpc_object_reference::get_object(oref->objectId, iid);

	// if not found in proxy table, create a proxy
	if (*object == 0) {
		// get the proxy creator function for this interface
		// an exception will be raised if this call fails
		proxyCreator = ProxyMgr::getProxyCreator(iid);

		// create the proxy and store the pointer
	    	* ((void **) object) = (*proxyCreator)(*oref);

		// add the proxy to the proxy table
		// the funny cast is required because we really have an
		// interface pointer returned by the proxy creator and need
		// to get down to the real base class pointer when calling
		// the registerProxyInTable() api.
		basePtr = (rpc_object_reference *)(basecast *)(void *)(*object);
		basePtr->registerProxyInTable(
			oref->objectId,
			iid,
			*object
		);
	}

	return;
}


////////////////////////////////////////////////////////////////////////////////
//
// Convert a stream of towers to a binding vector
//
////////////////////////////////////////////////////////////////////////////////

static void
rpc_ss_tower_stream_to_binding_vector(
	TWRREP			*twrStream,	// ptr to tower vector stream
	rpc_binding_vector_p_t	*bvec,		// place to build a binding vec
	rpc_tower_vector_p_t	*tvec		// place to store tower vector
) {
	Trace("rpc_ss_tower_stream_to_binding_vector");

	unsigned32	   st;			// status code
	int		   i;			// loop counter
	rpc_tower_vector_t *twr_vector;		// tower vector

	// get space for a tower vector
	twr_vector = (rpc_tower_vector_t *) malloc(
		sizeof(rpc_tower_vector_t) + 
		(twrStream->count - 1) * sizeof(twr_p_t)
	);
	if (twr_vector == 0) RAISE(rpc_x_no_memory);

	// copy the count to our local data structure
	twr_vector->count = twrStream->count;

	// copy the tower pointers and data to our local data structure
	// since the wire data structure is freed by the runtime
	for (i = 0; i < twr_vector->count; i++) {
		twr_vector->tower[i] = (twr_p_t) malloc(
			sizeof(twr_t) + twrStream->tower[i]->tower_length - 1
		);
		twr_vector->tower[i]->tower_length = 
			twrStream->tower[i]->tower_length;
		memcpy(
			twr_vector->tower[i]->tower_octet_string,
			twrStream->tower[i]->tower_octet_string,
			twrStream->tower[i]->tower_length
		);
	}

	// convert to tower vector to a binding vector using the RPC SPI
	rpc_binding_vec_from_twr_vec(
		twr_vector,
		bvec,
		&st
	);
	if (st != error_status_ok) exc_raise_status(st);

	// insert the object id into each binding handle
	for (i = 0; i < (*bvec)->count; i++) {
		rpc_binding_set_object(
			(*bvec)->binding_h[i],
			&twrStream->oid,
			&st
		);
		if (st != error_status_ok) exc_raise_status(st);
	}

#ifdef DEBUG
	if (DEBUG_ON) print_vector(*bvec);
#endif
	// save away the tower vector
	*tvec = twr_vector;

	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// Allocate space for and initialize a wire data structure
//
////////////////////////////////////////////////////////////////////////////////

static OBJREF *
allocate_wire_data_structure(
	void		*buff,			// buffer from high level rtns
	IDL_msp_t     	IDL_msp			// marshalling state
) {
	Trace("allocate_wire_data_structure()");

	OBJREF		*wire;			// ptr to wire data structure
	unsigned32	st;			// status code

	// if a buffer was passed in from the high level marshalling
	// interpreter routine, use it. Otherwise, create a new one.
	if (buff)
    		return (OBJREF *) buff;
	else {
    		wire = (OBJREF *) rpc_ss_mem_alloc(
                           		&IDL_msp->IDL_mem_handle,
					sizeof(OBJREF)
		);

			// init all fields in the wire data structure
			wire->u_objref.twr.count   = 0;
			wire->u_objref.twr.tower   = 0;
			wire->u_objref.twr.period  = 0;
			wire->u_objref.twr.princid = 0;
	
			uuid_create_nil(&wire->u_objref.twr.oid, &st);
			if (st != error_status_ok) exc_raise_status(st);
	}

	return wire;
}

////////////////////////////////////////////////////////////////////////////////
//
// Fill in a wire data structure from the io vector
//
////////////////////////////////////////////////////////////////////////////////

static void
get_wire_data_structure(
	OBJREF		*wire,			// address of space to free
	IDL_msp_t     	IDL_msp			// marshalling state
) {
	Trace("get_wire_data_structure()");

	idl_ulong_int	xmit_defn_index;  	// index into definition vector
	idl_ulong_int	*sav_off_vec;		// saves offset vector pointer
	idl_byte	*sav_typ_vec;		// saves type vector pointer
	IDL_side_k_t	sav_side_k;		// saves side kind flag

	// save the type/offset vector pointers and the client/server side flag
	sav_off_vec = IDL_msp->IDL_offset_vec;
	sav_typ_vec = IDL_msp->IDL_type_vec;
	sav_side_k  = IDL_msp->IDL_side;


	// swap in the type/offset vectors for an interface pointer type
	IDL_msp->IDL_offset_vec = interface_offset_vec;
	IDL_msp->IDL_type_vec   = interface_type_vec;


	// unmarshal the structure
	xmit_defn_index = INTERFACE_XMIT_INDEX;
        rpc_ss_ndr_unmar_struct(
		IDL_DT_FIXED_STRUCT,
                IDL_msp->IDL_type_vec + xmit_defn_index,
                (rpc_void_p_t)wire,
		NULL,
		NULL,
		IDL_msp
	);

	// unmarshall any embedded pointers in the structure

	// set server_side flag so that memory gets allocated 
	// for the unique ptr in the structure. 
	// see ndrui.c(rpc_ss_ndr_alloc_storage()) which calls
	// rpc_ss_mem_alloc().

	IDL_msp->IDL_side = IDL_server_side_k;

	rpc_ss_ndr_u_struct_pointees(
		IDL_DT_FIXED_STRUCT,
		xmit_defn_index,
                (rpc_void_p_t)wire,
		NULL,
		IDL_msp
	);

	// restore saved fields
	IDL_msp->IDL_offset_vec = sav_off_vec;
	IDL_msp->IDL_type_vec   = sav_typ_vec;
	IDL_msp->IDL_side       = sav_side_k;


	return;
}

////////////////////////////////////////////////////////////////////////////////
//
// Free a wire data structure
//
////////////////////////////////////////////////////////////////////////////////

static void
free_wire_data_structure(
	OBJREF		*wire,			// address of space to free
	void		*buff,			// buffer from high level rtns
	IDL_msp_t     	IDL_msp			// marshalling state
) {
	Trace("free_wire_data_structure()");

	int		i;			// loop counter


	// Release storage for transmitted type only if
	// it was allocated at this level in the marshalling process

	if (!buff) {
		// free each tower pointer in the array
		for (i = 0; i < wire->u_objref.twr.count; i++) {
			if (wire->u_objref.twr.tower[i]) {
				rpc_ss_mem_item_free(
					&IDL_msp->IDL_mem_handle,
					(byte_p_t) wire->u_objref.twr.tower[i]
				);
			}
		}

		// free the tower pointer array
		if (wire->u_objref.twr.tower) {
			rpc_ss_mem_item_free(
				&IDL_msp->IDL_mem_handle,
				(byte_p_t) wire->u_objref.twr.tower
			);
		}

		// free the wire data structure
		rpc_ss_mem_item_free(
			&IDL_msp->IDL_mem_handle,
			(byte_p_t) wire
		);
	}
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
	Trace("print_vector()");

	unsigned_char_t *s;			// string for of handle
	unsigned32	st;			// status code

	cout << "unmarshalled binding vector: " << endl;

	for (int i = 0; i < bvec->count; i++) {
		rpc_binding_to_string_binding(bvec->binding_h[i], &s, &st);
		if (st != error_status_ok) exc_raise_status(st);

		cout << "bvec[" << i << "]: " << (char *) s << endl;

		rpc_string_free(&s, &st);
		if (st != error_status_ok) exc_raise_status(st);
	}

}

#endif

