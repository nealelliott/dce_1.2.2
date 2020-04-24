// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: client.cxx,v $
// Revision 1.1.4.2  1996/09/10  18:21:12  bfc
// 	remove unecessary STDC definition
// 	[1996/09/10  18:16:28  bfc]
//
// Revision 1.1.4.1  1996/09/09  20:59:21  bfc
// 	finish TETizing and cleanup
// 	[1996/09/09  20:43:14  bfc]
// 
// Revision 1.1.2.2  1996/03/09  20:49:46  marty
// 	Add OSF copyright
// 	[1996/03/09  20:28:05  marty]
// 
// Revision 1.1.2.1  1995/12/11  19:35:37  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:18:43  root]
// 
// $EndLog$
#ifdef TET
#define TET_ENV
#define main loc_main
#endif

#include "test_com.h"
#include <assert.h>
#include <iostream.h>
#include <stdio.h>
#include <stdlib.h>
#ifndef _MSDOS
#ifdef __VMS
extern "C" {
    extern char *optarg;
    extern int getopt(int, char **, const char *);
}
#else
#ifndef AIX32
#include <unistd.h>
#endif
#endif
#endif
#include <dce/dce_error.h>
#include <dce/trace.hxx>
#include "objectid.h"		// IDL generated header file
#include "number.h"		// IDL generated header file
#ifndef _MSDOS
#if defined(VMS) || defined(__VMS)
#include <in.h>
#include <inet.h>
#else
#include <netinet/in.h>
#include <arpa/inet.h>
#endif
#endif

#ifdef _MSDOS
#define sleep(n) Sleep((n)*1000) 
#endif

// following line needed by older versions of OSF/1 (pre 3.0)
#ifdef __VMS
extern "C" {
extern int     gethostname ( char *, int );
extern unsigned int  sleep ( unsigned int );
}
#endif


int
main(int argc, char *argv[])
{
	Number         *n;
	char            hostname[100];
	char            name[100];
        unsigned_char_t *entry_name;
	double		val1;
	double		val2;

	cout << "trying bind by name" << endl;
	gethostname(hostname, sizeof(hostname));

        entry_name = (unsigned_char_t *)getenv("RPC_DEFAULT_ENTRY");
        if (entry_name == NULL)
          sprintf(name, "/.:/idltest/retry_%s", strtok(hostname, "."));
        else
          sprintf(name, "%s", entry_name);

	n = Number::bind((unsigned_char_t *) name);

	// get the number from the named object
	val1 = n->get();

	// kill this server I'm bound to - give it a chance to exit
	n->die();
	cout << "sleeping ..." << endl;
	//sleep(15);
	struct timespec interval;
	interval.tv_sec = 15;
	interval.tv_nsec = 0;
	pthread_delay_np(&interval);
	cout << "woke up" << endl;

	// get the number from the named object - since the bound server
	// has exited, I should automatically bind to the other server
	val2 = n->get();
	if (val1 == 1) assert(val2 == 2);
	if (val1 == 2) assert(val2 == 1);

	// kill this server I'm bound to - give it a chance to exit
	n->die();
	cout << "sleeping ..." << endl;
	//sleep(15);
	interval.tv_sec = 15;
	interval.tv_nsec = 0;
	pthread_delay_np(&interval);

	cout << "woke up" << endl;

	// now both servers should have exited - expect no more bindings
	TRY

		n->get();
	CATCH (rpc_x_no_more_bindings)
	ENDTRY
	cout << "test passed" << endl;

	return 0;
}
