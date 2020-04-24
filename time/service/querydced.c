/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: querydced.c,v $
 * Revision 1.1.4.2  1996/02/17  23:35:51  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:52:07  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:14:17  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:26:15  root]
 * 
 * Revision 1.1.2.5  1994/10/06  20:48:32  agd
 * 	expand copyright
 * 	[1994/10/06  14:33:31  agd]
 * 
 * Revision 1.1.2.4  1994/09/23  20:57:22  tom
 * 	Bug 12325 - Don't complain if dced attributes don't exist.
 * 	[1994/09/23  20:57:13  tom]
 * 
 * Revision 1.1.2.3  1994/08/25  21:47:50  cbrooks
 * 	CR11240 - retrieve config info from dced
 * 	[1994/08/25  20:17:16  cbrooks]
 * 
 * 	CR11240 - retrieve config info from dced
 * 	[1994/08/25  20:05:37  cbrooks]
 * 
 * Revision 1.1.2.2  1994/08/23  20:32:40  cbrooks
 * 	Code Cleanup
 * 	[1994/08/19  17:53:09  cbrooks]
 * 
 * Revision 1.1.2.1  1994/08/05  20:47:50  cbrooks
 * 	CR11240 - add dced config functionality
 * 	[1994/08/05  20:23:53  cbrooks]
 * 
 * $EndLog$
 */
/*  $Rev$
**
**  dts_dtsd_update_attributes 
**  
**  check for configuration attributes, modify if necessary
*/

#include <pthread.h>
#include <dce/dce.h>
#include <dce/dced.h>
#include <dce/dce_error.h>
#include <dce/dce_svc.h>
#include <dce/sec_attr_base.h>
#include <dce/assert.h>
#include <dtsd-hostdata.h>
#include <dcedtssvc.h>
#include <dcedtsmac.h>
#include <dcedtsmsg.h>
#include <strings.h>

extern dce_svc_handle_t dts_svc_handle;

#define MAX_DTS_ATTR_LEN 128

static char attr_name[ MAX_DTS_ATTR_LEN ];
static char attr_value[ MAX_DTS_ATTR_LEN ];


void dts_dtsd_update_attributes( uuid_t *, boolean32, error_status_t *);
static int parse_attr_string( const idl_char *, char [], char []);
void read_data_from_file( sec_attr_enc_str_array_t ** );


/*
 * dts_dtsd_update_attributes 
 * 
 * retrieve attribute entries (if any) stored via dced for the DTS 
 * daemon, and update the configuration information attributes.
 *

 * Where possible Invalid entries (unknown names, out of range values,
 * value erroneously specified for clerk) generate warnings, but
 * processing continues.
 *
 * Current definition of the dce_server_inq_attr() function is in RFC47.3,
 * section 9.2 (as of 8.2.94).
 * */

void dts_dtsd_update_attributes(
    uuid_t * exec_id_p, 
    boolean32 read_from_file, 
    error_status_t * stp 
)
{
    sec_attr_enc_str_array_t * str_arr_p = 0 ;
    VarValues v ;
    sec_attr_t dts_attr_values;
    int i = 0 ;

    /* begin */

    *stp = rpc_s_ok ;

    if ( read_from_file) 
    {
	read_data_from_file( &str_arr_p ) ;
    }
    else 
    {
	dce_server_inq_attr( &dced_g_uuid_dtsconfattr, 
			     &dts_attr_values, 
			     stp);
	if (*stp != rpc_s_ok)
	{
	    dce_error_string_t tempstring;
	    if (*stp != dced_s_server_attr_not_found)
	    {
		/* OK if nothing there */
	    	dce_svc_printf( DTS_S_INQ_ATTR_MSG, 
			        dce_dts_error_text( *stp, tempstring ));
	    }
	    return ;
	}

	if ( dts_attr_values.attr_value.attr_encoding != 
	    sec_attr_enc_printstring_array)
	{
	    dce_error_string_t tempstring;
	    dce_svc_printf( DTS_S_ATTR_ENCODING_MSG, 
			   dce_dts_error_text(*stp, tempstring));
	    return ;
	}
	str_arr_p = dts_attr_values.attr_value.tagged_union.string_array ;
    }
    
    dce_assert( dts_svc_handle, str_arr_p != NULL) ;
    for (i = 0 ; i < str_arr_p->num_strings; i++) 
    {
	if (parse_attr_string( str_arr_p->strings[i], attr_name, attr_value))
	    continue ;
	dts_dtsd_parse_entry( attr_name, attr_value, &v) ;
	if (v.valid_value)	/* it's valid */
	    dts_dtsd_set_entry( &v ) ;
    }
    return;

}

static const char * WS = " \t" ;
    

/*
 * parse_attr_string
 *
 * parse an attribute string passed back by dced
 * we assume a format of 
 * "attribute name of multiple words : a_single_value "
 *
 * returns 0 if successful, 1 if not (standard Unix code)
 */

static int 
parse_attr_string( const idl_char * attr_str_p, char name [], char value[])
{
    register const char * p = (const char *)attr_str_p ;
    register const char * q;
    int i, n ;

    /* begin */

    name[0] = 0 ;
    value[0] = 0 ;

    i = strspn( p, WS) ;	/* skip leading WS */
    p += i ;			/* increment p by that amount */
    
    q = strpbrk( p, " \t:");	/* find the intermediate ':' */
    if (q == NULL)		/* not there !? damn! */
	return 1;

    if ((n = q - p) >= MAX_DTS_ATTR_LEN) /* too long? */
	return 1 ;
    strncpy( name, p, n); /* copy string */
    name[n] = 0;		/* make sure trailing null */

    p = ++q ;			/* one past the colon */
    i = strspn( p, " \t:") ;	/* skip whitespace, if any */
    p += i ;

    q = strpbrk( p, " \t#");	/* until trailing ws */
    if (q == NULL)		/* just end of line */
    {
	n = strlen(p) ;
    }
    else 
    {
	if ((n = q - p) >= MAX_DTS_ATTR_LEN)
	    return 1 ;
    }
    strncpy( value, p, n);
    value[n] = 0 ;
    return 0 ;
}

#include <stdio.h>
#include <malloc.h>
/*
 * read_data_from_file - 
 * this is a testing and debugging routine. Hence, we 
 * allow the perror() and abort() calls. Otherwise, 
 * we would add serviceability calls if this were production code
 */

void read_data_from_file ( sec_attr_enc_str_array_t ** sapp )
{
    register sec_attr_enc_str_array_t * sap = 0 ;
    register char * bufp = 0 ;
    FILE * fp = 0 ;
    int i = 0 ;
    

    /* begin */

    *sapp = sap = malloc( sizeof(sec_attr_enc_str_array_t) + 
			 ( 99 * sizeof (idl_char *))) ;
    if (sap == 0)
    {
	perror("malloc failure - read_data_from_file");
	abort();
    }

    sap -> num_strings = 0 ;

    if ( ( fp = fopen("dtsvars", "r")) == 0)
    {
	return ;
    }

    if ((bufp = malloc( 1024 )) == 0)
    {
	perror("malloc failure - read_data_from_file");
	abort();
    }
    while(fgets(bufp, 128, fp) != NULL)
    {
	int n  = 0 ;

	sap -> strings[i++] = (idl_char *)bufp;
	n = strlen(bufp);
	bufp[n - 1] = 0;	/* trailing newline */
	bufp += n ;
	sap -> num_strings++;
    }
    fclose(fp);
    return ;
}

	
	
    

    
    
       
    
	
