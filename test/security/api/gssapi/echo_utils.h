/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: echo_utils.h,v $
 * Revision 1.1.4.2  1996/02/18  23:07:11  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:28:17  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:19:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:51:40  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:45:22  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:16:54  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/09  15:51:10  wray
 * 	Initial submission (OT#10019)
 * 	[1994/03/09  15:40:42  wray]
 * 
 * $EndLog$
 */
/*
 * The Echo example program is derived from code found in
 * W. Richard Stevens UNIX Network Programming, copyright 1990, reprinted
 * by permission of Prentice-Hall, Inc., Englewood Cliffs, New Jersey.
 */

#ifndef _ECHO_UTILS_
#define _ECHO_UTILS_
#include <dce/gssapi.h>

#define err_sys(x) {perror(x); exit (1);}
#define err_dump(x) {fprintf(stderr, x); exit (1);}

int readn(register int fd,
	  register char * ptr,
	  register int nbytes);
int writen(register int fd,
	   register char * ptr,
	   register int nbytes);
int readline(register int fd,
	     register char * ptr,
	     register int maxlen);
void str_echo(int sockfd, int service_level, gss_ctx_id_t security_context);
void str_cli(register FILE *fp,
	     register int sockfd,
	     int service_level,
	     gss_ctx_id_t security_context);
int read_token(int socket, gss_buffer_t tok);
int write_token(int socket, gss_buffer_t tok);
void display_error(char * where, OM_uint32 maj, OM_uint32 min);
#endif
