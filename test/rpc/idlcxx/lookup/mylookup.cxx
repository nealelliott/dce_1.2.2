// @OSF_COPYRIGHT@
// COPYRIGHT NOTICE
// Copyright (c) 1996 Open Software Foundation, Inc.
// ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
// the full copyright text.
// 
// 
// HISTORY
// $Log: mylookup.cxx,v $
// Revision 1.1.4.2  1996/09/10  18:21:05  bfc
// 	finish TETizing and cleanup
// 	[1996/09/10  18:15:45  bfc]
//
// Revision 1.1.4.1  1996/09/09  20:59:05  bfc
// 	finish TETizing and cleanup
// 	[1996/09/09  20:42:01  bfc]
// 
// Revision 1.1.2.2  1996/03/09  20:49:00  marty
// 	Add OSF copyright
// 	[1996/03/09  20:27:31  marty]
// 
// Revision 1.1.2.1  1995/12/11  19:34:32  root
// 	Submit OSF/DCE 1.2.1
// 	[1995/12/11  19:18:10  root]
// 
// $EndLog$
#include "test_com.h"
#include <iostream.h>
#include <dce/uuid.h>
#include "matriximpl.h"

extern Matrix * identityMatrix;
extern char	*entryName;

Matrix *
myLookup(uuid_t *u)
{
	unsigned_char_t *string_uuid;
	unsigned32	status;
	static int	i = 0;


	i++;
	if (i != 1) {
		cout << "now creating and registering " << entryName;
		cout << " with the Object Table" << endl;
		identityMatrix = new matrixImpl(1, 0, 0, 1);
		identityMatrix->register_named_object(
				(unsigned_char_t *)entryName
		);
		cout << "registerd identity matrix: " << endl;
		return identityMatrix;
	} else {
		return 0;
	}
}
