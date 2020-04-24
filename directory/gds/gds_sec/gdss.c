/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: gdss.c,v $
 * Revision 1.1.4.2  1996/02/18  18:18:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:41:21  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:56:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:11:39  root]
 * 
 * Revision 1.1.2.2  1994/06/09  18:45:18  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:18:07  devsrc]
 * 
 * Revision 1.1.2.1  1994/02/22  19:21:07  marrek
 * 	Add rcsid string.
 * 	[1994/02/09  14:31:28  marrek]
 * 
 * 	Creation for dce1.1.
 * 	[1994/02/02  10:55:08  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gdss.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 18:18:29 $";
#endif

/*--------------------------------------------------------------------*/
/*                                                                    */
/* Copyright (c) 1993 Siemens Nixdorf Informationssysteme AG          */
/* All Rights Reserved                                                */
/*                                                                    */
/* Project      : GDS SEC                                             */
/*                                                                    */
/* File Name    : gdss.c                                              */
/*                                                                    */
/* Description  : The module contains thetestprogram for the security */
/*                functions that are called from the DSA              */
/*                                                                    */
/* Date         : December 28, 1993                                   */
/*                                                                    */
/* Author       : Helmut Baumgaertner SNI BA NM 123, Munich, Germany  */
/*                                                                    */
/*--------------------------------------------------------------------*/

#include <stdio.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/uio.h>
#include <unistd.h>
#include <fcntl.h>
#include <d2dir.h>
#include <gds_sec.h>
#include <gds_sec_util.h>

#ifdef GSS_SAMSON
#include <xom.h>
#include <xds.h>
#include <xdsbdcp.h>
#include <scdirgds.h>
#include <scdirgss.h>
#endif

#define S_TOKEN "./s_token"
#define R_TOKEN "./r_token"

/*#define IAPL_SERVER_NAME    "85.4.10samson85.4.11test85.4.10sag185.4.3dsa1"*/
#define IAPL_SERVER_NAME    "/O=samson/OU=test/O=sag/CN=dsa1"



/* prototypes for functions */

int main(int ac , char* av[]);
int sec_test_get_token_size(char *tokenfile);
int sec_test_read_token( char *tokenfile, D2_str *token);
int sec_test_write_token( char *tokenfile, D2_str *token);


main(int ac, char* av[])
{

int ret , trust_l;
D2_sec_status status;
signed32 trust_level, bind_id;
Bool result;
signed16 mechtype;
D2_name_string myself ,target_dsa;
D2_name_string client_name;
D2_credent my_cred;
D2_password my_pw;
D2_cred_handle_t cred_h;
D2_str s_token , r_token;
Bool masterb;

GDS_SEC_TRACE0(1,"ENTRY main");

/* check args */

switch (ac)
	{
	case 1:
		mechtype = D2_STRONG;
		printf(" server test process uses Sec Mech Strong \n");
		break;
	case 2:
		if (strcmp (av[1], "a") == 0 )
			{
			mechtype = D2_ANONYMOUS;
			printf(" server test process uses Sec Mech  ANONYMOUS \n");
			}
		else if (strcmp (av[1], "s") == 0 )
			{
			mechtype = D2_SIMPLE;
			printf(" server test process uses Sec Mech  SIMPLE \n");
			}
		else if (strcmp (av[1], "D") == 0 )
			{
			mechtype = D2_DCE_AUTH;
			printf(" server test process uses Sec Mech  DCE \n");
			}
		else if (strcmp (av[1], "S") == 0 )
			{
			mechtype = D2_STRONG;
			printf(" server test process uses Sec Mech  STRONG \n");
			}
		else
			{
			mechtype = (short) 8;
			printf(" server test process uses Sec Mech 8 (not valid)\n");
			}
		break;
	default:
		printf("usage : %s [a|s|D|S]\n",av[0]);
		exit(1);
	}

myself = (D2_name_string) malloc (100);  
strcpy((char*)myself, IAPL_SERVER_NAME );


my_pw.d2_p_len = 8 ;
my_pw.d2_p_pw = (Octet_string) malloc (8);
memcpy(my_pw.d2_p_pw,"geheim!!", 8);
my_cred.d2_c_name =  (Octet_string) myself;
my_cred.d2_c_passwd = &my_pw,


ret = dsa_acquire_cred (mechtype,  myself, &status); 
	GDS_SEC_CHECK_RETURN("dsa_acquire_cred",ret,&status);
gds_sec__print_auth_ctx (mechtype);


/* very simple synchronisation mechanism */

fprintf(stderr,"WAIT FOR SECURITY TOKEN (return)");
getchar();

/* read the sectoken from file ./s_token written by the       
   client test process gdsc */

s_token.d2_size = sec_test_get_token_size(S_TOKEN) ;
s_token.d2_value = (unsigned char*) malloc (s_token.d2_size);

ret = sec_test_read_token(S_TOKEN,&s_token);
if (ret != s_token.d2_size)
	GDS_SEC_TRACE2(1,"sec token : %d of %d bytes",ret,s_token.d2_size);

/*  allocate memory for return tokenfrom dsa_accept_sec_ctx */ 
r_token.d2_size = 2000;
r_token.d2_value = (unsigned char*) malloc (2000);

/*  allocate memory for client_name returned from dsa_accept_sec_ctx */ 
client_name = (unsigned char*) malloc(500);

ret = dsa_accept_sec_ctx (mechtype, &s_token, &r_token, &client_name,
						&masterb, &status); 
	GDS_SEC_CHECK_RETURN("dsa_accept_sec_cxt",ret,&status);

GDS_SEC_TRACE2(2,"returned client_name <%s> masterbind <%d>",
								client_name,masterb);
GDS_SEC_TRACE_TOKEN(3,"return_token",&r_token);

/* write the token to file ./r_token in order to make it 
   available for client test process gdsc */

sec_test_write_token(R_TOKEN, &r_token);

ret = dsa_del_cred (mechtype, &status); 
	GDS_SEC_CHECK_RETURN("dsa_del_cred",ret,&status);
gds_sec__print_auth_ctx (mechtype);

ret = dsa_get_tdt_entry (myself, &trust_l, &status); 
	GDS_SEC_CHECK_RETURN("dsa_get_tdt_entry",ret,&status);
GDS_SEC_TRACE2(2,"returned trust_level <%d> for <%s>", trust_l, myself);

GDS_SEC_TRACE0(1,"EXIT main");
exit (0);
}

int sec_test_get_token_size(char *tokenfile)
	{
	struct stat finfo;

	if (stat (tokenfile,&finfo) == -1 )
		{
		GDS_SEC_TRACE0(1,"error in stat system call");
		exit (1);
		}
	GDS_SEC_TRACE2(3,"tokenfile %s size %d",tokenfile,finfo.st_size);
	if (finfo.st_size < 1 )
		{
		GDS_SEC_TRACE0(1,"tokenfile size too small");
		exit (1);
		}
	return (finfo.st_size);
	}

int sec_test_read_token( char *tokenfile, D2_str *token)
	{
	int dk , bytes_read;

	dk = open (tokenfile,O_RDONLY);
	if ( dk == -1 )
		{
		GDS_SEC_TRACE0(1,"error in open system call");
		exit (1);
		}
    bytes_read = read(dk, (void*) token->d2_value, token->d2_size);
	if (bytes_read == -1)
		{
		GDS_SEC_TRACE0(1,"error in read system call");
		exit (1);
		}
	close(dk);
	return (bytes_read);
	}

int sec_test_write_token( char *tokenfile, D2_str *token)
	{
	int dk , bytes_written;

	dk = open (tokenfile,O_WRONLY);
	if ( dk == -1 )
		{
		GDS_SEC_TRACE0(1,"error in open system call");
		exit (1);
		}
    bytes_written = write(dk, (void*) token->d2_value, token->d2_size);
	if (bytes_written == -1 )
		{
		GDS_SEC_TRACE0(1,"error in write system call");
		exit (1);
		}
	if (bytes_written != token->d2_size)
		GDS_SEC_TRACE2(1,"too less bytes written: %d of %d", 
				bytes_written, token->d2_size);
	close(dk);
	return (bytes_written);
	}

