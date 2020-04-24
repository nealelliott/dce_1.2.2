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
 * $Log: echo_utils.c,v $
 * Revision 1.1.4.2  1996/02/17  23:24:51  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:04  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:19:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:51:39  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:45:21  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:16:47  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/09  15:51:09  wray
 * 	Initial submission (OT#10019)
 * 	[1994/03/09  15:40:36  wray]
 * 
 * $EndLog$
 */
/*
 * The Echo example program is derived from code found in
 * W. Richard Stevens UNIX Network Programming, copyright 1990, reprinted
 * by permission of Prentice-Hall, Inc., Englewood Cliffs, New Jersey.
 */
#include <stdio.h>
#include <dce/gssapi.h>
#include "echo_utils.h"
#include "echo_inet.h"

#define min(a,b) (((a)<(b))?(a):(b))

/* READN - Read nbytes from file descriptor FD				    */
int readn(register int fd,
	  register char * ptr,
	  register int nbytes) {
    int nleft, nread;

    nleft = nbytes;
    while (nleft > 0) {
	nread = read(fd, ptr, nleft);
	if (nread < 0) return nread;
	else if (nread == 0) break; /* EOF */

	nleft -= nread;
	ptr += nread;
    }
    return (nbytes - nleft);

}

/* WRITEN - Write nbytes to file descriptor FD				    */
int writen(register int fd,
	   register char * ptr,
	   register int nbytes) {
    int nleft, nwritten;

    nleft = nbytes;

    while (nleft > 0) {
	nwritten = write(fd, ptr, nleft);
	if (nwritten <= 0) return nwritten;

	nleft -= nwritten;
	ptr += nwritten;
    }
    return (nbytes - nleft);

}

int readline(register int fd,
	     register char * ptr,
	     register int maxlen) {
    int	n, rc;
    char    c;

    for (n=1; n<maxlen; n++) {
	if ((rc=read(fd, &c, 1)) == 1) {
	    *ptr++ = c;
	    if (c == '\n') break;
	} else if (rc == 0) {
	    if (n == 1) return 0;   /* EOF, no data read		    */
	    else break;		    /* EOF, some data was read		    */
	} else return -1;	    /* error				    */
    }
    *ptr = 0;
    return n;
}

#define MAXLINE 512

void str_echo(int sockfd, int service_level, gss_ctx_id_t security_context) {
    int	n;
    char line[MAXLINE];
    OM_uint32 maj_stat, min_stat;
    gss_buffer_desc token, message;
    unsigned char length_low, length_high;

    for (;;) {
        if (service_level == SERVICE_SECURE) {
	  n = read(sockfd, &length_low,1);
	  if (n == 0) return; /* Connection closed */
	  if (n != 1) {
	    fprintf(stderr, "Error reading token length\n");
	    exit(EXIT_FAILURE);
	  };
	  if (read(sockfd, &length_high,1) != 1) {
	    fprintf(stderr, "Error reading token length\n");
	    exit(EXIT_FAILURE);
	  };
	  token.length = length_high * 256 + length_low;
	  token.value = malloc(token.length);
	  if (read_token(sockfd, &token) != token.length) {
	    fprintf(stderr, "Error reading token\n");
	    exit(EXIT_FAILURE);
	  };
	  maj_stat = gss_unseal(&min_stat,
				security_context,
				&token,
				&message,
				NULL,
				NULL);
	  if (GSS_ERROR(maj_stat)) {
	    display_error("gss_unseal", maj_stat, min_stat);
	    exit (EXIT_FAILURE);
	  };
	  n = min(MAXLINE, message.length);
	  strncpy(line, message.value, MAXLINE);
	} else {
	  n = readline(sockfd, line, MAXLINE);
	};
	if (n == 0) return; /* Connection terminated			    */
	else if (n < 0)
	    err_dump("str_echo: readline error");
	if (writen(sockfd, line, n) != n)
	    err_dump("str_echo: writen error");
    }
}

void str_cli(register FILE *fp,
	     register int sockfd,
	     int service_level,
	     gss_ctx_id_t security_context) {
    int n;
    char sendline[MAXLINE];
    char recvline[MAXLINE+1];
    gss_buffer_desc token;
    gss_buffer_desc message;
    OM_uint32 maj_stat, min_stat;
    unsigned char length_low;
    unsigned char length_high;

    while (fgets(sendline, MAXLINE, fp) != NULL) {
	n = strlen(sendline);
	
	if (service_level == SERVICE_SECURE) {
	  message.length = n;
	  message.value = sendline;
	  maj_stat = gss_seal(&min_stat,
			      security_context,
			      0,
			      GSS_C_QOP_DEFAULT,
			      &message,
			      NULL,
			      &token);
	  if (GSS_ERROR(maj_stat)) {
	    display_error("gss_seal", maj_stat, min_stat);
	    exit (EXIT_FAILURE);
	  };
	  length_low = token.length & 0xff;
	  length_high = token.length >> 8;
	  if (write(sockfd, &length_low, 1) != 1) {
	    fprintf(stderr, "Error writing length\n");
	    exit(EXIT_FAILURE);
	  };
	  if (write(sockfd, &length_high, 1) != 1) {
	    fprintf(stderr, "Error writing length\n");
	    exit(EXIT_FAILURE);
	  };
	  write_token(sockfd, &token);
	  gss_release_buffer(&min_stat, &token);

	} else {
	  if (writen(sockfd, sendline, n) != n)
	    err_sys("str_cli: writen error on socket");
	}
	n = readline(sockfd, recvline, MAXLINE);
	if (n < 0) err_dump("str_cli: readline error");
	recvline[n] = 0;
	fputs(recvline, stdout);
	if (strcmp(recvline, sendline) != 0)
	    err_sys("str_cli: Lines don't match!");
    }
    if (ferror(fp)) err_sys("str_cli: error reading file");
}


int read_token(int socket, gss_buffer_t token) {
  return readn(socket, (char *)token->value, token->length);
}

int write_token(int socket, gss_buffer_t token) {
  return writen(socket, (char *)token->value, token->length);
}

void display_error(char * where, OM_uint32 maj, OM_uint32 min) {
  OM_uint32 maj_stat, min_stat;
  gss_buffer_desc status_buffer;
  int ctx = 0;
  
  fprintf(stderr, "Errors detected in %s\n", where);
  do {
    
    if (GSS_ERROR(gss_display_status(&min_stat,
				     maj,
				     GSS_C_GSS_CODE,
				     GSS_C_NULL_OID,
				     &ctx,
				     &status_buffer))) {
      fprintf(stderr, "Error translating major status code (%X)\n", maj);
      ctx = 0;
    } else {
      fprintf(stderr, "%.*s\n", status_buffer.length, status_buffer.value);
      gss_release_buffer(&min_stat, &status_buffer);
    };
  } while (ctx != 0);
  
  if (GSS_ERROR(gss_display_status(&min_stat,
				   min,
				   GSS_C_MECH_CODE,
				   GSS_C_NULL_OID,
				   &ctx,
				   &status_buffer))) {
    fprintf(stderr, "Error translating minor status code (%X)\n", min);
  } else {
    fprintf(stderr, "%.*s\n", status_buffer.length, status_buffer.value);
    gss_release_buffer(&min_stat, &status_buffer);
  };

}
  



