/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: format_tower.c,v $
 * Revision 1.1.9.2  1996/02/18  19:29:32  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:11:17  marty]
 *
 * Revision 1.1.9.1  1995/12/08  15:09:59  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:40:41  root]
 * 
 * Revision 1.1.7.3  1994/08/03  19:01:04  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:46:42  mccann]
 * 
 * Revision 1.1.7.2  1994/06/09  18:37:06  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:08:54  devsrc]
 * 
 * Revision 1.1.7.1  1994/03/12  21:59:35  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:02:29  peckham]
 * 
 * Revision 1.1.5.4  1992/12/30  13:06:58  zeliff
 * 	Embedding copyright notices
 * 	[1992/12/29  22:38:20  zeliff]
 * 
 * Revision 1.1.5.3  1992/12/07  15:32:56  mfox
 * 	Define an in_addr variable and use it for calls to inet_ntoa().
 * 	Also include arpa/inet.h instead of declaring inet_ntoa().
 * 	[1992/12/07  15:30:33  mfox]
 * 
 * Revision 1.1.5.2  1992/09/29  19:10:44  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:11:10  weisman]
 * 
 * Revision 1.1.2.2  1992/03/22  21:49:22  weisman
 * 		Big merge with post-1.0 development at Digital.
 * 	[1992/03/22  20:22:01  weisman]
 * 
 * Revision 1.1  1992/01/19  15:23:04  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1991-1994. ALL RIGHTS RESERVED.
 *
 * THIS SOFTWARE IS FURNISHED UNDER A LICENSE AND MAY BE USED AND  COPIED
 * ONLY  IN  ACCORDANCE  WITH  THE  TERMS  OF  SUCH  LICENSE AND WITH THE
 * INCLUSION OF THE ABOVE COPYRIGHT NOTICE.  THIS SOFTWARE OR  ANY  OTHER
 * COPIES  THEREOF MAY NOT BE PROVIDED OR OTHERWISE MADE AVAILABLE TO ANY
 * OTHER PERSON.  NO TITLE TO AND OWNERSHIP OF  THE  SOFTWARE  IS  HEREBY
 * TRANSFERRED.
 *
 * THE INFORMATION IN THIS SOFTWARE IS SUBJECT TO CHANGE  WITHOUT  NOTICE
 * AND  SHOULD  NOT  BE  CONSTRUED  AS  A COMMITMENT BY DIGITAL EQUIPMENT
 * CORPORATION.
 *
 * DIGITAL ASSUMES NO RESPONSIBILITY FOR THE USE OR  RELIABILITY  OF  ITS
 * SOFTWARE ON EQUIPMENT THAT IS NOT SUPPLIED BY DIGITAL.
 *
 */

#include <cdscp.h>
#include <fcntl.h>
#include <tower.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <uc_clerk.h>     

#include <dce/cprpc.h>

#define DECnet_session 0x02	
#define OSI_ID  0x03
#define TP_ID 0x04
#define OSI_TP 0x05
#define DECnet_routing 0x06
#define TCP_ID 0x07
#define UDP_ID 0x08
#define IP_ID  0x09
#define MAX_STRING 1000	/* max of bind string or tower data	*/

static int get_floor_string(byte_t *, byte_t **, char *);

int
format_tower (
char		**towerstr_p,
MemberValue_t	*tower_p)
{
 	char	**array, *pointer;
	byte_t	*data_p, protocol = 0;
	int	i = 1;
	int	status = DNS_ERROR;
	int	count, asize;
        error_status_t rpc_status;
	char string[MAX_STRING];
	unsigned char *string_bind;
	rpc_binding_handle_t h_p;

	data_p = (byte_t *)tower_p;
	count = GETL16(data_p);	/* get number of tower floors */
	asize = (count + 1) * sizeof(char *) + count * MAX_STRING;
	*towerstr_p = (char *)malloc(asize);
	if ( !*towerstr_p )
		return(DNS_NOROOM);

	array = (char **)*towerstr_p;
	ZERO_bytes(*towerstr_p, asize);
	pointer = (char *)array + (count + 1) * sizeof(char *);
	array[0] = pointer;

	/* first try to use RPC to convert the complete tower for us.	*/

	rpc_tower_to_binding ((unsigned char *)tower_p, &h_p, &rpc_status);

	if ( rpc_status == rpc_s_ok )
	    rpc_binding_to_string_binding ( h_p, &string_bind, &rpc_status);

	if ( rpc_status == rpc_s_ok ) {
	    strcpy ( pointer, (char *)string_bind );
	    rpc_string_free ( &string_bind, &rpc_status );
	    status = DNS_SUCCESS;
	}

	/* If RPC fails because the tower was not an RPC tower, see if
	 * it might be our own two-floor protocol tower. If so, show the
	 * floors as binding strings, else revert to the "ugly" way of
	 * showing the floors individually.
	*/

	if ( rpc_status == rpc_s_not_rpc_tower ) {
	    if ((count == 2 ) && 
		( get_cds_binding_string ( (byte_t *)tower_p, string ))) {
		strcpy ( pointer, string );
		status = DNS_SUCCESS;
	    }
	}

        /* here to show the floors as ugly hex strings			*/

	if ( status != DNS_SUCCESS ) {
	    for (i = 0; i < count; i++) {   /* format each tower floor  */
		array[i] = pointer;
		pointer += get_floor_string ( &protocol, &data_p, array[i]);
	    }
	}
	array[i] = 0;
	return ( DNS_SUCCESS );
}

static int
get_floor_string (
	byte_t	*protocol,	/* IN/OUT */
	byte_t	**d_p,		/* IN */
	char	*str_p)		/* OUT */
{
	byte_t	id, *save_ptr;
	char	*Saddress_p;
	int	i, length;
	struct in_addr addr;

	/* Note: the data content of the protocol related floors is inter-
	 * dependent, i.e. the interpretation of floors n+1, n+2, etc.
	 * may depend on the protocol identifier in floor n. Thus the first
	 * floor containing what appears to be a reasonable protocol id
	 * sets the value of the protocol variable to be passed back and
	 * forth between this routine and the calling routine so that
	 * subsequent floors will be properly formatted.
	 * Crock of ages, cleft for me.....	   			*/

	str_p[0] = '\0';		/* in case nothing is found	*/
	length = GETL16 ( *d_p );	/* format the left side 	*/
	if ( length ) {
	    byte_t *in_p = *d_p;
	    char *out_p = strchr(str_p, '\0');

	    if ( length == 1 ){		/* protocol id's are one byte	*/
		id = EXT8 ( *d_p );	/* determine protocol id	*/
		if ( !*protocol ){
		    if ( id == TCP_ID || id == UDP_ID ||
			 id == OSI_ID || id == DECnet_session )
		    *protocol = id;
		}
	    }
	    for ( i = 0; i < length; i++ ) {
		if (0 < i)
		    *out_p++ = ' ';
		sprintf ( out_p, "%02x", *in_p++ );
		out_p = strchr(out_p, '\0');
	    }
	    *d_p = in_p;
	}
	else
	    cds_strcat ( str_p, CDSCP_NULL, "(null)" );

	strcat ( str_p, "\t" );

	length = GETL16 ( *d_p );	/* format the right side 	*/
	save_ptr = *d_p;		/* save ptr to related data 	*/
        if ( 0 < length ) {
	    byte_t *in_p = *d_p;
	    char *out_p = strchr(str_p, '\0');

	    for ( i = 0; i < length; i++ ) {
		if (0 < i)
		    *out_p++ = ' ';
		sprintf ( out_p, "%02x", *in_p++ );
		out_p = strchr(out_p, '\0');
	    }
	    *d_p = in_p;
	}
	else
	    cds_strcat ( str_p, CDSCP_NULL, "(null)" );

	if ( length > 2 )
	    strcat( str_p, "\t" );
	else
	    strcat( str_p, "\t\t" );

	/* generate hopefully useful ASCII string data */

	if ( !*protocol ){
	    if ( id == 0x0a )
		cds_strcat ( str_p, CDSCP_DATAGRAM, "datagram" );
	    else if ( id == 0x0b )
		cds_strcat ( str_p, CDSCP_CONNECTION, "connection" );
	}
	else {			/* protocol has been defined	*/
	    memset(&addr, 0, sizeof(addr));
	    switch ( id )
	      {
	      case DECnet_session:
		cds_strcat ( str_p, CDSCP_DNET_PROTOCOL, "ncacn_dnet_nsp" );
		break;

	      case OSI_ID:
		break;

	      case TP_ID:
					/*EMPTY*/
		if ( *protocol == OSI_ID ) {
		}
					/*EMPTY*/
		else if ( *protocol == DECnet_session ) {
		}
		break;

	      case OSI_TP:
		break;

	      case DECnet_routing:
		break;

	      case TCP_ID:
		if ( length ) {
		  COPY_bytes( save_ptr, &addr, length );
		  cds_sprintf ( strchr(str_p, '\0'),
			CDSCP_PORT, "port: %u, ", addr );
		}
		cds_strcat ( str_p, CDSCP_TCP_PROTOCOL, "ncacn_ip_tcp" );
		break;

	      case UDP_ID:
		if ( length ) {
		  COPY_bytes( save_ptr, &addr, length );
		  cds_sprintf ( strchr(str_p, '\0'),
			CDSCP_PORT, "port: %u, ", addr );
		}
		cds_strcat ( str_p, CDSCP_UDP_PROTOCOL, "ncadg_ip_udp" );
		break;

	      case IP_ID:
	/*	    
	** Call to inet_ntoa is Ultrix specific and IP_ID is a future for 
	** DECnet, so remove from VMS code.
	*/
#ifndef VMS	      
		if ( *protocol == TCP_ID || *protocol == UDP_ID ) {
		  COPY_bytes( save_ptr, &addr, length );
		  /* Convert network address to string */
		  Saddress_p = (char *)inet_ntoa ( addr );
		  cds_sprintf ( strchr(str_p, '\0'),
		    CDSCP_HOST, "host: %s", Saddress_p );
		}
#endif
		break;

	      default:
		break;
	      }
	}
	return ( strlen(str_p) + 1 );
}
