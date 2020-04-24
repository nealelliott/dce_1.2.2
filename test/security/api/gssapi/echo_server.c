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
 * $Log: echo_server.c,v $
 * Revision 1.1.4.2  1996/02/17  23:24:50  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:03  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:19:50  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:51:38  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:45:20  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:16:46  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/09  15:51:08  wray
 * 	Initial submission (OT#10019)
 * 	[1994/03/09  15:40:20  wray]
 * 
 * $EndLog$
 */
#include <stdlib.h>
#include <dce/gssapi.h>
#include "echo_inet.h"
#include "echo_utils.h"
/* Invoked with one parameter containing the port number (or string name)   */

#define value(x) (((x>='0')&&(x<='9'))?x-'0':-1)


char * pname;
char * port_param;
char * file_param;
char * server_name;
int port;
char port_name[512];

int accept_service(int socket, 
		   gss_ctx_id_t * sec_ctx,
		   gss_cred_id_t cred_handle) {
  unsigned char service_byte;
  unsigned char length_low, length_high;
  unsigned length;
  gss_buffer_desc snd_token;
  gss_buffer_desc rcv_token;
  gss_name_t client;
  gss_buffer_desc client_name;
  int done;
  OM_uint32 maj_stat, min_stat;
  
  if (read(socket, &service_byte, 1) != 1) {
    fprintf(stderr, "Error reading service byte\n");
    return -1;
  };

#ifdef DEBUG
  fprintf(stderr, "Read service byte...\n");
  fflush(stderr);
#endif

  if (service_byte == SERVICE_SECURE) {
    
    *sec_ctx = GSS_C_NO_CONTEXT;
    rcv_token.length = 0;
    rcv_token.value = NULL;
    snd_token.length = 0;
    snd_token.value = NULL;
    done = 0;
    
    while (!done) {
      if (read(socket, &length_low, 1) != 1) {
	fprintf(stderr, "Error reading token length\n");
	return -1;
      };

#ifdef DEBUG
      fprintf(stderr, "Read low-length...\n");
      fflush(stderr);
#endif
      
      if (read(socket, &length_high, 1) != 1) {
	fprintf(stderr, "Error reading token length\n");
	return -1;
      };

#ifdef DEBUG
      fprintf(stderr, "Read high-length...\n");
      fflush(stderr);
#endif
      
      length = length_low + length_high * 256;

#ifdef DEBUG
      fprintf(stderr, "Reading %d bytes of token\n", length);
      fflush(stderr);
#endif
      
      rcv_token.value = realloc(rcv_token.value, length);
      rcv_token.length = length;
      if (read_token(socket, &rcv_token) != rcv_token.length) {
	fprintf(stderr, "Error - EOF when waiting for token\n");
	return -1;
      };

#ifdef DEBUG
      fprintf(stderr, "Read token...\n");
      fflush(stderr);
#endif      

      maj_stat = gss_accept_sec_context(&min_stat,
					sec_ctx,
					cred_handle,
					&rcv_token,
					GSS_C_NO_CHANNEL_BINDINGS,
					&client,
					NULL,
					&snd_token,
					NULL,
					NULL,
					NULL);
      if (GSS_ERROR(maj_stat)) {
	display_error("gss_accept_sec_context", maj_stat, min_stat);
	return -1;
      };
      
      if (!(maj_stat & GSS_S_CONTINUE_NEEDED)) done = 1;

#ifdef DEBUG
      fprintf(stderr, "Accepted connection\n");
      fflush(stderr);
#endif

      if (snd_token.length != 0) {

#ifdef DEBUG
	fprintf(stderr, "Returning token...\n");
	fflush(stderr);
#endif

	length_low = snd_token.length & 0xff;
	length_high = snd_token.length >> 8;
	if (write(socket, &length_low, 1) != 1) {
	  fprintf(stderr, "Error - EOF when writing token length\n");
	  return -1;
	};
	if (write(socket, &length_high, 1) != 1) {
	  fprintf(stderr, "Error - EOF when writing token length\n");
	  return -1;
	};
	if (write_token(socket, &snd_token) != snd_token.length) {
	  fprintf(stderr, "Error - EOF when sending token\n");
	  return -1;
	};
      };
    };
    maj_stat = gss_display_name(&min_stat,
				client,
				&client_name,
				NULL);
    if (GSS_ERROR(maj_stat)) {
      display_error("gss_display_name", maj_stat, min_stat);
      return -1;
    };

    fprintf(stdout, 
	    "Accepted authenticated connection from %.*s\n", 
	    client_name.length,
	    client_name.value);

  } else {
    fprintf(stdout, "Connection is unauthenticated...\n");
  };


  return service_byte;
}

int main(int argc, char * argv[]) {
  int sockfd;
  int newsockfd;
  int clilen;
  int	childpid;
  struct  sockaddr_in	cli_addr, serv_addr;
  struct servent * server;
  int i;
  int service_level;
  unsigned security_context;
  int c;
  int errflg = 0;
  OM_uint32 min_stat, maj_stat;
  gss_cred_id_t cred_handle = GSS_C_NO_CREDENTIAL;
  gss_name_t server_name_t;
  gss_buffer_desc name_buffer;
  
  extern int optind, opterr;
  extern char * optarg;
  
  pname = argv[0];
  port_param = DEFAULT_ECHO_PORT;
  file_param = NULL;
  server_name = NULL;
  
  while ((c = getopt(argc, argv, "p:f:s:")) != EOF) {
    switch (c) {
    case 'p': port_param = optarg; break;
    case 'f': file_param = optarg; break;
    case 's': server_name = optarg; break;
    default: errflg=1;
    };
  };
  if (errflg || (optind < argc)) {
    fprintf(stderr, 
	    "%Usage: %s [-p <port>] [-f <keytable>] [-s <server-name>]\n", 
	    pname);
    exit (EXIT_FAILURE);
  };
  
  for (i=0; isdigit(port_param[i]); i++);
  
  if (port_param[i]) {
    /* The port contains non-digits, therefore assume its a name and translate  */
    /* it via getservbyname							    */
    
    if ((server = getservbyname(port_param, "tcp")) == NULL) {
      fprintf(stderr, "%s: Couldn't locate %s\n", pname, port_param);
      exit (EXIT_FAILURE);
    };
    port = server->s_port;
    strncpy(port_name, server->s_name, sizeof(port_name));
  } else {
    port = 0;
    for (i=0; port_param[i]; i++) {
      port = port * 10 + value(port_param[i]);
    };
    if ((server = getservbyport(port, "tcp")) == NULL) {
      strncpy(port_name, "<unnamed service>", sizeof(port_name));
    } else {
      strncpy(port_name, server->s_name, sizeof(port_name));
    };
  };
  if (server_name != NULL) {
    
    name_buffer.length = strlen(server_name);
    name_buffer.value = server_name;
    maj_stat = gss_import_name(&min_stat,
			       &name_buffer,
			       GSS_C_NULL_OID,
			       &server_name_t);
    if (GSS_ERROR(maj_stat)) {
      display_error("gss_import_name", maj_stat, min_stat);
      exit(EXIT_FAILURE);
    };

#ifdef DEBUG
    fprintf(stderr, "Imported name...\n");
    fflush(stderr);
#endif

    if (file_param != NULL) {
      maj_stat = 
	gssdce_register_acceptor_identity(&min_stat,
					  server_name_t,
					  NULL,
					  file_param);
      if (GSS_ERROR(maj_stat)) {
	display_error("gssdce_register_identity", maj_stat, min_stat);
	exit (EXIT_FAILURE);
      };
      
#ifdef DEBUG
      fprintf(stderr, "Registered identity...\n");
      fflush(stderr);
#endif

    };

    maj_stat = gss_acquire_cred(&min_stat,
				server_name_t,
				24*60*60,
				GSS_C_NULL_OID_SET,
				GSS_C_ACCEPT,
				&cred_handle,
				NULL,
				NULL);
    if (GSS_ERROR(maj_stat)) {
      display_error("gss_acquire_cred", maj_stat, min_stat);
      exit (EXIT_FAILURE);
    };

#ifdef DEBUG
    fprintf(stderr, "Acquired cred...\n");
    fflush(stderr);
#endif
      
  };

  fprintf(stdout,
	  "Echo server started on port %d, %s\n",
	  port, port_name);
  
  if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    err_dump("server: can't open stream socket");
  
  memset((unsigned char *)&serv_addr, 0, sizeof(serv_addr));
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
  serv_addr.sin_port = htons(port);
  
  if (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    err_dump("server: Can't bind local address");
  
  listen(sockfd, 5);
  
  for (;;) {
    clilen = sizeof(cli_addr);
    newsockfd = accept(sockfd, (struct sockaddr *)&cli_addr, &clilen);
    if (newsockfd < 0) err_dump("server: accept error");
    if ((childpid = fork()) < 0) {
      err_dump("server: fork error");
    } else if (childpid == 0) {
      close(sockfd);
      if ((service_level = accept_service(newsockfd, 
					  &security_context, 
					  cred_handle)
	   ) < 0) {
	fprintf(stderr, "Error accepting conection\n");
	exit(0);
      };
      if (service_level == SERVICE_SECURE)
	fprintf(stdout, "Accepted secure connection ");
      else fprintf(stdout, "Accepted insecure connection ");
      fprintf(stdout, "from host %d.%d.%d.%d\n",
	      ((unsigned char *)&cli_addr.sin_addr.s_addr)[0],
	      ((unsigned char *)&cli_addr.sin_addr.s_addr)[1],
	      ((unsigned char *)&cli_addr.sin_addr.s_addr)[2],
	      ((unsigned char *)&cli_addr.sin_addr.s_addr)[3]);
      
      str_echo(newsockfd, service_level, security_context);
      exit(0);
    }	
    close(newsockfd);
  }
}










