// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: server.hxx,v $
// Revision 1.1.2.2  1996/03/09  20:49:52  marty
// 	Add OSF copyright
// 	[1996/03/09  20:28:09  marty]
//
// Revision 1.1.2.1  1995/12/11  19:35:43  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:18:46  root]
// 
// $EndLog$
// DCE server class

#ifndef server_included
#define server_included

#include <dce/rpc.h>

// Global Declarations
extern class Server *theServer;			// DCE server instance

class Server {

private:
	static rpc_binding_vector_p_t	bvec;	// binding vector
	rpc_if_handle_t			ifspec;	// interface specification
	char				*name;	// namespace entry
	char				*protocol;// protocol name

public:

	Server(				// constructor
		rpc_if_handle_t ifspec,
		char * = 0		// potocol
	);

	~Server();			// destructor

	rpc_binding_vector_p_t getBindingVectorPtr() {
		return bvec;
	}

	// Exports a name into CDS
	void exportName(char *name);

	// Exports a uuid into a CDS entry
	void exportUuidVector(uuid_vector_t &u);

	// Exports a uuid into a CDS entry
	void exportUuid(uuid_t &u);

	// removes a uuid vector from a CDS entry
	void unexportUuidVector(
			uuid_vector_t &u,
			int		down = 0
	);

	// removes a uuid from a CDS entry
	void unexportUuid(
			uuid_t	&u
	);

	// Puts the server process in the listening state
	static void listen();

	// Builds a string to issue an rpccp command - exports name with uuid
	void build_system_cmd(char *, char *);

	// Builds a string to issue an rpccp command - exports name without uuid
	void build_system_cmd(char *);

	// Print a binding handle to a file
	void printBindingToFile();

	// Builds a CDS entry name conforming to the IDL testing convention
	static char * buildEntryName(char *);
};

#endif
