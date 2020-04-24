/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/* 
 * HISTORY 
 * $Log: choosebinding.c,v $
 * Revision 1.1.4.2  1996/02/17  23:34:44  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:50:54  marty]
 *
 * Revision 1.1.4.1  1995/12/08  18:09:15  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  17:24:17  root]
 * 
 * Revision 1.1.2.2  1994/06/10  20:51:58  devsrc
 * 	cr10872 - fixed copyright
 * 	[1994/06/10  16:48:46  devsrc]
 * 
 * Revision 1.1.2.1  1994/04/21  22:05:28  cbrooks
 * 	CR 3231, 8353
 * 	[1994/04/21  19:49:39  cbrooks]
 * 
 * $EndLog$
 */

/*
 * dts_dtscp_choose_dts_binding - given a binding handle, choose one
 * based on which protocol sequence we want
 * 
 * Convert to string, then compare against a list of protocol sequences.
 * We prefer Connection based, then Datagram
 */

#include <dtscp.h>
#include <stddef.h>



extern char ** protseq_select_vector ;


boolean dts_dtscp_choose_first_binding( rpc_binding_handle_t bh) 
{
    return TRUE ;
}


boolean dts_dtscp_choose_dts_binding(rpc_binding_handle_t binding_h)
{

    int                     p_indx;
    unsigned32              status = rpc_s_ok, junk = 0 ;
    boolean binding_chosen  = FALSE ;

    unsigned_char_t *  string_binding;
    unsigned_char_t *  protseq;

    /* begin */

    rpc_binding_to_string_binding( binding_h, &string_binding, &status);
    dts_dtscp_check_status( status, "Can't get string binding:", 
			   dts_e_dtscp_resume) ;
    if (status != rpc_s_ok) 
	return FALSE ;
    
    rpc_string_binding_parse( string_binding, 
			     NULL, /* no object UUID */
			     &protseq, /* protocol sequence  */
				/* not network addr, endpoint, options */
			     NULL, NULL, NULL, 
			     &status);

    dts_dtscp_check_status( status, "Can't parse string binding:", 
			   dts_e_dtscp_resume) ;

    if (status == rpc_s_ok)
    {
	char * ps  = 0 ;
	for (p_indx = 0; (ps = protseq_select_vector[p_indx]) != 0; p_indx++)
	{
	    if (strcmp( ps, (char *)protseq) == 0 )
	    {
		binding_chosen = TRUE ;
		break;
	    }
	} /* for */
    } /* if */

    rpc_string_free(&string_binding, &junk) ;
    rpc_string_free(&protseq, &junk) ;

    return binding_chosen ;
}




