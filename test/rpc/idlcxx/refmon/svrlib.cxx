// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: svrlib.cxx,v $
// Revision 1.1.2.2  1996/03/11  14:10:16  marty
// 	Update OSF copyright years.
// 	[1996/03/11  13:59:04  marty]
//
// Revision 1.1.2.1  1996/02/23  18:48:08  root
// 	Submit OSF/DCE 1.2.1
// 	[1996/02/23  18:36:44  root]
// 
// $EndLog$
// undefine MIN/MAX since the are defined in param.h unconditionally
#ifdef MAX
#undef MAX
#endif

#ifdef MIN
#undef MIN
#endif

#ifdef _MSDOS
#define GUID_DEFINED
#define GUID uuid_t
#endif
#include "test_com.h"

// Implementation file for a DCE Server class
#include <dce/rpc.h>
#include <dce/stubbase.h>

#if defined(__osf__)
#include <unistd.h>
#elif defined (__VMS)
#include <socket.h>
#elif defined (_MSDOS)
#include <winsock.h>
#endif


#include "server.hxx"

#ifdef _MSDOS
#undef rpc_if_rep_t
#define rpc_if_rep_t RPC_CLIENT_INTERFACE
#endif

// Constant Declarations
const char *PREFIX = "/.:/idltest/";            // CDS entry prefix
const char *PROTOCOL_NAME = "ncacn_ip_tcp";
const char *BINDING_FILE = "binding.dat";

// The static binding vector
rpc_binding_vector_p_t Server::bvec = 0;

// the server class constructor
Server::Server(rpc_if_handle_t ifspec, char *prot) : 
    name(0), ifspec(ifspec), protocol(prot) {

	error_status_t	st;

	// create the binding vector if not already done
	if (!bvec) {
		if (!protocol) {
			protocol = (char *) PROTOCOL_NAME;
			rpc_server_use_all_protseqs(
				rpc_c_protseq_max_reqs_default,
				&st
			);
			CHECK_STATUS(st, "Can't create bindings:", ABORT);
		} else {
			rpc_server_use_protseq(
				(unsigned_char_t *) protocol,
				rpc_c_protseq_max_reqs_default,
				&st
			);
			CHECK_STATUS(st, "Can't create bindings:", ABORT);
		}

		// save the binding vector into the static variable
		rpc_server_inq_bindings(&bvec, &st);
		CHECK_STATUS(st, "Can't inquire bindings:", ABORT);
	}

	// update the endpoint map
	rpc_ep_register(
		ifspec,
		bvec,
		NULL,
		(unsigned_char_t *) "IDL Test Server",
		&st
	);
	CHECK_STATUS(st, "Can't update endpoint:", ABORT);
}

// the server class destructor
Server::~Server() {

	error_status_t	st;

	if (name) 
		delete [] name;

	rpc_ep_unregister(ifspec, bvec, NULL, &st);
	CHECK_STATUS(st, "Can't unregister endpoint:", RESUME);

	rpc_binding_vector_free(&bvec, &st);
	CHECK_STATUS(st, "Can't free binding vector:", RESUME);

}

// Exports a name into CDS
void
Server::exportName(
	char *name
) {

	error_status_t	st;

	if (this->name)
		delete [] this->name;

	this->name = new char[strlen(name) + 1];
	strcpy(this->name, name);

	rpc_ns_binding_export(
		rpc_c_ns_syntax_default,
		(unsigned_char_t *) this->name,
		ifspec,
		bvec,
		NULL,
		&st
	);
	CHECK_STATUS(st, "Can't export to name service:", ABORT);
}

// add a uuid vector to the namespace entry
void
Server::exportUuidVector(
	uuid_vector_t &uvec
) {
	error_status_t	st;

	rpc_ns_binding_export(
		rpc_c_ns_syntax_default,
		(unsigned_char_t *) name,
		ifspec,
		bvec,
		(uuid_vector_p_t)&uvec,
		&st
	);
	CHECK_STATUS(st, "Can't add uuid to namespace:", ABORT);
}

// add a uuid vector to the namespace entry
void
Server::exportUuid(
	uuid_t &u
) {
//	error_status_t	st;
	uuid_vector_t	uvec;

#ifdef _WIN32
        uvec.Count   = 1;
        uvec.Uuid[0] = &u;
#else        
	uvec.count   = 1;
	uvec.uuid[0] = &u;
#endif
	exportUuidVector(uvec);
}

// unexport a uuid vector from the namespace entry
// if default parameter down is 1, remove the namespace entry also
void
Server::unexportUuidVector(
	uuid_vector_t &uvec,
	int down
) {
	error_status_t	st;

	rpc_ns_binding_unexport(
		rpc_c_ns_syntax_default,
		(unsigned_char_t *) name,
		down ? ifspec : NULL,
		(uuid_vector_p_t)&uvec,
		&st
	);
	CHECK_STATUS(st, "Can't unexport uuid", ABORT);
}


// unexport a uuid from the namespace entry
void
Server::unexportUuid(
	uuid_t &u
) {
	uuid_vector_t	uvec;

#ifdef _WIN32
        uvec.Count   = 1;
        uvec.Uuid[0] = &u;
#else
 	uvec.count   = 1;
	uvec.uuid[0] = &u;
#endif
	unexportUuidVector(uvec, 0);
}

// Puts the server process in the listening state
void
Server::listen() {
	error_status_t	st;

	rpc_server_listen(rpc_c_listen_max_calls_default, &st);
	CHECK_STATUS(st, "Can't listen:", ABORT);

}

// Build a namespace entry conforming to the IDL tests
char *
Server::buildEntryName(char *s) {
	char	hostname[100];
	char	*ret;
	char	*p;

	// get host name from system
	if (gethostname(hostname, sizeof(hostname))) {
		perror("Can't get hostname");
		return 0;
	}
	// break of just the host part from the fully qualified name
	p = strtok(hostname, ".");

	// allocate space and build a CDS entry name
	ret = new char [strlen(PREFIX) + strlen(p) + strlen(s) + 2];
	sprintf(ret, "%s%s_%s",PREFIX, s, p);

	return ret;

}

// Build an rpccp command to export an object id under a name
void
Server::build_system_cmd(char *objectId, char *s)
{
	char		*uuidStr;
	char		hostname[100];
	unsigned32	st;

	gethostname(hostname, sizeof(hostname));

#ifdef _MSDOS
	uuid_to_string (
		  &(((rpc_if_rep_t *) ifspec)->InterfaceId.SyntaxGUID),
		  (unsigned_char_t **) &uuidStr,
		  &st
	);
#else
	uuid_to_string (
		  &(((rpc_if_rep_t *)ifspec)->id),
		  (unsigned_char_t **) &uuidStr,
		  &st
	);
#endif
	CHECK_STATUS(st, "Can't convert uuid to string:", ABORT);

    	sprintf(s, "rpccp export %s -i %s -b %s:%s -o %s",
		name,
		uuidStr,
		protocol,
		strtok(hostname, "."),
		objectId
    	);
}


// Build an rpccp command to export an object id under a name
void
Server::build_system_cmd(char *s)
{
	char		*uuidStr;
	char		hostname[100];
	unsigned32	st;

	gethostname(hostname, sizeof(hostname));

#ifdef _MSDOS
	uuid_to_string (
		  &(((rpc_if_rep_t *)ifspec)->InterfaceId.SyntaxGUID),
		  (unsigned_char_t **) &uuidStr,
		  &st
	);
#else
	uuid_to_string (
		  &(((rpc_if_rep_t *)ifspec)->id),
		  (unsigned_char_t **) &uuidStr,
		  &st
	);
#endif
	CHECK_STATUS(st, "Can't convert uuid to string:", ABORT);

    	sprintf(s, "rpccp export %s -i %s -b %s:%s",
		name,
		uuidStr,
		PROTOCOL_NAME,
		strtok(hostname, ".")
    	);
}

// write the binding handle in string form to an output file
void 
Server::printBindingToFile() {
	unsigned_char_t	*string_binding;
	FILE		*fp;
	unsigned32	st;

    	fp = fopen(BINDING_FILE,"w");
    	for (int i = 0; i < bvec->count; i++) {
        	idl_char *uuid, *protseq, *netaddr, *endpoint,*netopt;

        	rpc_binding_to_string_binding(
			bvec->binding_h[i],
			&string_binding,
			&st
		);
        	CHECK_STATUS(st, "Can't get string binding:", ABORT);
       	 
        	rpc_string_binding_parse(
			string_binding,
			&uuid,
			&protseq,
			&netaddr,
			&endpoint,
			&netopt,
			&st
		);
        	CHECK_STATUS(st, "Can't parse string binding:", ABORT);
	
        	fprintf(fp," %s %s %s \n", protseq, netaddr, endpoint);
    	}
    	fclose(fp);
}
