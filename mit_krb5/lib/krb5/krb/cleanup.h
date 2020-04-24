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
 * $Log: cleanup.h,v $
 * Revision 1.1.2.1  1996/06/05  20:57:37  arvind
 * 	DCE1.2.2 Drop 2 to OSF
 * 	[1996/06/05  19:55:23  arvind]
 *
 * $EndLog$
 */
/*
** Copyright (c) Hewlett-Packard Company 1996
** Unpublished work. All Rights Reserved.
**
*/

#ifndef KRB5_CLEANUP
#define KRB5_CLEANUP

struct cleanup {
    void 		* arg;
    void		(*func)();
};

#define CLEANUP_INIT(x)							\
    struct cleanup cleanup_data[x];					\
    int cleanup_count = 0;		

#define CLEANUP_PUSH(x, y)						\
    cleanup_data[cleanup_count].arg = x;				\
    cleanup_data[cleanup_count].func = y;				\
    cleanup_count++;

#define CLEANUP_POP(x)							\
    if ((--cleanup_count) && x && (cleanup_data[cleanup_count].func)) 	\
	cleanup_data[cleanup_count].func(cleanup_data[cleanup_count].arg); 
	
#define CLEANUP_DONE()							\
    while(cleanup_count--) 						\
	if (cleanup_data[cleanup_count].func)  				\
	    cleanup_data[cleanup_count].func(cleanup_data[cleanup_count].arg); 
    

#endif
