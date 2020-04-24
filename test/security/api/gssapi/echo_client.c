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
 * $Log: echo_client.c,v $
 * Revision 1.1.4.2  1996/02/17  23:24:48  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:01  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:19:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:51:35  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:45:18  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  17:16:44  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/09  15:51:04  wray
 * 	Initial submission (OT#10019)
 * 	[1994/03/09  15:40:12  wray]
 * 
 * $EndLog$
 */
/*
 * The Echo example program is derived from code found in
 * W. Richard Stevens UNIX Network Programming, copyright 1990, reprinted
 * by permission of Prentice-Hall, Inc., Englewood Cliffs, New Jersey.
 */
#include "echo_inet.h"
#include "echo_utils.h"
#include <dce/gssapi.h>
#include <stdlib.h>

#define value(x) (((x>='0') && (x<='9'))?x-'0':-1)
#define MAX_HOST_LENGTH 64

char * pname;
char * port_param = NULL;
char * host_param = NULL;
int port;
char port_name[512];
unsigned host_addr;
struct hostent * host_entry = NULL;

gss_ctx_id_t security_context;

int define_service(int socket, 
		   int security_level, 
		   gss_ctx_id_t * sec_ctx,
		   char * server_name) {
/* If security requested, perform initial token exchange and to set up       */
/* security context.                                                         */

/* The protocol is very simple.  The initial byte that the client sends the  */
/* server is simply the security level (either SERVICE_SECURE or             */
/* SERVICE_INSECURE).  For SERVICE_INSECURE, that's it - everything else is  */
/* user-data that the server will echo.  For SERVICE_SECURE, the next thing  */
/* that the client sends is a token, followed by a token from server to      */
/* client (actually, the process may continue for several tokens).  Each     */
/* such token is preceeded by a two-byte length field, in LSB-order.         */

  unsigned char service_byte = security_level;
  OM_uint32 maj_stat, min_stat;
  gss_name_t server;
  gss_buffer_desc name;
  gss_buffer_desc snd_token;
  gss_buffer_desc rcv_token;
  int done;
  unsigned char length_low, length_high;
  int length;

  if (write(socket, &service_byte, 1) != 1) {
    fprintf(stderr, "Error writing service byte\n");
    return -1;
  };

#ifdef DEBUG
  fprintf(stderr, "Written service byte...\n");
  fflush(stderr);
#endif
  
  if (security_level == SERVICE_SECURE) {
    name.length = strlen(server_name);
    name.value = server_name;
    maj_stat = gss_import_name(&min_stat,
			       &name,
			       GSS_C_NULL_OID,
			       &server);
    if (GSS_ERROR(maj_stat)) {
      display_error("gss_import_name", 
		    maj_stat, 
		    min_stat);
      return -1;
    };
    *sec_ctx = GSS_C_NO_CONTEXT;
    rcv_token.length = 0;
    rcv_token.value = NULL;
    snd_token.length = 0;
    snd_token.value = NULL;
    done = 0;
    
    while (!done) {
      maj_stat = gss_init_sec_context(&min_stat,
				      GSS_C_NO_CREDENTIAL, /* Default cred */
				      sec_ctx,
				      server,
				      GSS_C_NULL_OID,
				      GSS_C_MUTUAL_FLAG | GSS_C_INTEG_FLAG,
				      24*60*60,
				      GSS_C_NO_CHANNEL_BINDINGS,
				      &rcv_token,
				      NULL,
				      &snd_token,
				      NULL,
				      NULL);
      if (GSS_ERROR(maj_stat)) {
	display_error("gss_init_sec_context",
		      maj_stat,
		      min_stat);
	return -1;
      };
      
      if (snd_token.length != 0) {
	length_low = snd_token.length & 0xff;
	length_high = snd_token.length >> 8;
	if (write(socket, &length_low, 1) != 1) {
	  fprintf(stderr, "Error - EOF when sending token\n");
	  return -1;
	};

#ifdef DEBUG
	fprintf(stderr, "Written low-length...\n");
	fflush(stderr);
#endif

	if (write(socket, &length_high, 1) != 1) {
	  fprintf(stderr, "Error - EOF when sending token\n");
	  return -1;
	};

#ifdef DEBUG
	fprintf(stderr, "Written high-length...\n");
	fflush(stderr);
#endif

#ifdef DEBUG
     	fprintf(stderr, "Writing %d bytes of token\n", snd_token.length);
	fflush(stderr);
#endif

	if (write_token(socket, &snd_token) != snd_token.length) {
	  fprintf(stderr, "Error - EOF when sending token\n");
	  return -1;
	};
      };

#ifdef DEBUG
      fprintf(stderr, "Written token...\n");
      fflush(stderr);
#endif
      
      if (maj_stat & GSS_S_CONTINUE_NEEDED) {
	if (read(socket, &length_low, 1) != 1) {
	  fprintf(stderr, "Error - EOF when waiting for token\n");
	  return -1;
	};
	if (read(socket, &length_high, 1) != 1) {
	  fprintf(stderr, "Error - EOF when waiting for token\n");
	  return -1;
	};
	length = length_low + length_high * 256;
	rcv_token.value = realloc(rcv_token.value, length);
	rcv_token.length = length;
	if (read_token(socket, &rcv_token) != rcv_token.length) {
	  fprintf(stderr, "Error - EOF when waiting for token\n");
	  return -1;
	};
      } else {
	done = 1;
      }
    };
  };
  return 0;
}

int main(int argc, char * argv[]) {
    int sockfd;
    struct  sockaddr_in	serv_addr;
    struct servent * server;
    struct hostvent * host;
    int i, c;
    int errflg = 0;
    int service_level;
    char * server_name;

    extern int optind, opterr;
    extern char * optarg;
    
    pname = argv[0];
    service_level = SERVICE_INSECURE;

    while ((c = getopt(argc, argv, "s:p:h:")) != EOF) {
      switch (c) {
      case 's': service_level = SERVICE_SECURE; 
	server_name = optarg;
	break;
      case 'p': port_param = optarg; break;
      case 'h': host_param = optarg; break;
      default: errflg = 1; break;
      };
    };


    if (errflg || (optind < argc)) {
	fprintf(stderr, "Usage: %s [-s] [-p <port>] [-h <host>]\n", pname);
	return EXIT_FAILURE;
    };

    if (port_param == NULL) port_param = DEFAULT_ECHO_PORT;

    for (i=0; isdigit(port_param[i]); i++);

    if (port_param[i]) {
/* The port contains non-digits, therefore assume its a name and translate  */
/* it via getservbyname							    */

	if ((server = getservbyname(port_param, "tcp")) == NULL) {
	    fprintf(stderr, "%s: Couldn't locate %s\n", pname, port_param);
	    return EXIT_FAILURE;
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


    if (host_param == NULL) {
/* Echo server is on localhost.						    */
	host_param = (char *)malloc(MAX_HOST_LENGTH);
	memset((unsigned char *)host_param, 0, MAX_HOST_LENGTH);
	gethostname(host_param, MAX_HOST_LENGTH-1);
    };

/* Now to translate host_param to numeric form */


    for (i=0; host_param[i] != '\0'; i++) {
      if (!isdigit(host_param[i]) && (host_param[i] != '.')) break;
    };

    if (host_param[i] == '\0') {
/* We have a numeric address */
      host_addr = inet_addr(host_param);
      host_entry = gethostbyaddr((char *)&host_addr, 4, AF_INET);
    } else {
      host_entry = gethostbyname(host_param);
    };

    if (host_entry == NULL) {
      fprintf(stderr, "Couldn't find host %s\n", host_param);
      exit(EXIT_FAILURE);
    };


    memset((unsigned char *)&serv_addr, 0, sizeof(serv_addr));
    serv_addr.sin_family = AF_INET;
    memcpy((unsigned char *)&serv_addr.sin_addr.s_addr,
           (unsigned char *)host_entry->h_addr,
	   4);
    serv_addr.sin_port = htons(port);

    fprintf(stdout,
         "Echo client started for host %s, address %d.%d.%d.%d, port %d, %s\n",
	    host_param, 
	    ((unsigned char *)&serv_addr.sin_addr.s_addr)[0],
	    ((unsigned char *)&serv_addr.sin_addr.s_addr)[1],
	    ((unsigned char *)&serv_addr.sin_addr.s_addr)[2],
	    ((unsigned char *)&serv_addr.sin_addr.s_addr)[3],
	    port, 
	    port_name);

    if ((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
	err_dump("client: can't open stream socket");

    if (connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
	err_sys("client: Can't connect to server");    

    if (define_service(sockfd, 
		       service_level, 
		       &security_context, 
		       server_name)) {
      exit (EXIT_FAILURE);
    };

    fprintf(stdout, "Connection open...\n");
    fflush(stdout);

    str_cli(stdin, sockfd, service_level, security_context);

    close(sockfd);
    return EXIT_SUCCESS;
}

