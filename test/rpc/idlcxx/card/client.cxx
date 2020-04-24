// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: client.cxx,v $
// Revision 1.1.4.2  1996/09/10  18:21:00  bfc
// 	remove unecessary STDC definition
// 	[1996/09/10  18:15:18  bfc]
//
// Revision 1.1.4.1  1996/09/09  20:58:54  bfc
// 	finish TETizing and cleanup
// 	[1996/09/09  20:41:03  bfc]
// 
// Revision 1.1.2.2  1996/03/09  20:48:29  marty
// 	Add OSF copyright
// 	[1996/03/09  20:27:08  marty]
// 
// Revision 1.1.2.1  1995/12/11  19:33:47  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:17:49  root]
// 
// $EndLog$
//
// This test demonstrates how to pass native C++ objects as parameters
// in RPC calls. The test deals with Player objects, where the Player
// class might be defined in a C++ class library or be a user-defined
// C++ class. A Player class is a base class for a more specific definition
// such as a BaseballPlayer class.
//
// A server fuction is passed a Player delegate interface in order to create
// sports cards on the server. The delegate interface, IPlayer, demonstrates
// how a Player object can be created locally in the client and its 
// information accessed by the server without the use of interface
// inheritance or or knowledge of the Player class hierarchy.
//

#ifdef TET
#define TET_ENV
#define main loc_main
#endif

#ifdef _WIN32
#include "afx.h"                        // NT CString class
#else
#include <String.h>
#endif /* _WIN32 */

#include "test_com.h"


#include "card.h"			// IDL generated header file
#include "iplayer_mgr.h"		// IDL generated delegate class

int 
main(int argc, char *argv[])
{
	ICard	*t;
	Player	*players[10];
	IPlayer *iPlayer;
	String	s;
	int	num = 0;
	int	i;

	//
	// Test passing a Player object as an RPC argument
	//

	// Create some Player objects. Note that the actual object created
	// is specific to the sport, but referenced with the common base
	// class, Player
	players[num++] = new BaseballPlayer("Carl", "Yastremzki", .284);
	players[num++] = new BaseballPlayer("Ted", "Williams", .344);
	players[num++] = new BasketballPlayer("Larry", "Bird", .876);

	// Print out the local Player objects
	cout << "\nLocal Player Information:\n";
	for (i = 0; i < num; i++)
		cout << *players[i] << endl;

	// create delegates for the Player objects
	// and pass the delegate interface to the remote procedure
	// to be used to build a sports card.
	for (i = 0; i < num; i++) {
		IPlayer *iPlayer = new IPlayerMgr(players[i]);
		ICard::newCard(iPlayer, &s);
		delete iPlayer;
		cout << s << endl;
	}

	cout << "All tests passed" << endl;

	return 0;
}
