/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * All Rights Reserved
 */
/*
 * HISTORY
 * $Log: sec_pvtkey_era.c,v $
 * Revision 1.1.2.1  1996/10/04  14:25:58  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/04  13:58:31  arvind]
 *
 * Revision /main/DCE_1.2.2/1  1996/09/09  21:32 UTC  arvind
 * 	PKSS drop from DEC (DCE1.2.2)
 * 	[1996/08/30  15:40 UTC  arvind  /main/arvind_pkss/1]
 * 
 * $EndLog$
 */
/* (c) Copyright 1996,
 * Digital Equipment Corporation, Maynard, Massachusetts, USA
 * All Rights Reserved
 *
 * Module:
 *
 * Purpose:
 *
 */

#include "sec_pvtkey_era.h"

const uuid_t sec_pvtkey_private_key_storage_era = {
        0x1f79c78e,
        0xb026,
        0x11cf,
        0xb7,
        0xc8,
        {0x08, 0x00, 0x09, 0x0a, 0x52, 0x54}
      };
