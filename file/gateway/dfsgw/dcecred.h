/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for 
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: dcecred.h,v $
 * Revision 1.1.4.1  1996/10/02  17:49:49  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:39:13  damon]
 *
 * Revision 1.1.2.1  1994/07/26  19:13:02  mckeen
 * 	Expanded copyrights
 * 	[1994/07/26  18:57:43  mckeen]
 * 
 * 	Initial Version
 * 	[1994/07/26  16:01:49  mckeen]
 * 
 * $EndLog$
 */
/* $Header: /u0/rcs_trees/dce/rcs/file/gateway/dfsgw/dcecred.h,v 1.1.4.1 1996/10/02 17:49:49 damon Exp $ */
/*
 * dcecred.h - DCE credential 
 */

#ifndef _DCECRED_H_
#define _DCECRED_H_

#include <dce/sec_login.h>


typedef struct dcecred_t {
      int (*validate)();             /* validate cred with optional password */
      long (*pag)();                 /* return PAG */
      time_t (*expiration_time)();   /* return expiration time */
} dcecred_t;

typedef dcecred_t *dcecred_p;
extern dcecred_p dcecred_init(char *, unsigned long, int *);

#endif /* _DCECRED_H_ */

