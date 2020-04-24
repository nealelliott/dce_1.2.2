/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: hel_svc.h,v $
 * Revision 1.1.4.2  1996/02/18  23:38:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:32:21  marty]
 *
 * Revision 1.1.4.1  1995/12/08  20:54:58  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  20:50:05  root]
 * 
 * Revision 1.1.2.1  1994/06/14  19:58:39  weir
 * 	DCE 1.1 Beta check-in
 * 	[1994/06/14  16:15:52  weir]
 * 
 * $EndLog$
 */
/******
 *
 * hel_svc.h
 *
 *
 *****/


/* Prototypes for server. */
int main(int _1, char *_2[]);
void install_filters();
void print_error(char *_1, error_status_t _2);
