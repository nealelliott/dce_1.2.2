/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: x500process.c,v $
 * Revision 1.1.6.1  1996/10/03  14:34:36  arvind
 * 	better NO_GDS support.
 * 	[1996/09/17  06:10 UTC  sommerfeld  /main/sommerfeld_pk_kdc_1/1]
 *
 * Revision 1.1.4.2  1996/02/18  19:31:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:12:38  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  15:13:38  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:46  root]
 * 
 * Revision 1.1.2.3  1994/08/03  19:01:51  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:53:43  mccann]
 * 
 * Revision 1.1.2.2  1994/06/09  18:38:20  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:37  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/12  22:02:00  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:10:51  peckham]
 * 
 * $EndLog$
 */
/*
 * COPYRIGHT (c) DIGITAL EQUIPMENT CORPORATION 1993-1994. ALL RIGHTS RESERVED.
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
 */

/*
 * x500process.c
 *
 * This file contains functions for the X.500 GDA module.
 * It replaces the version from Siemens/Nixdorf ('x500parent.c)
 * and makes calls to XDS/XOM interfaces to read the CDS cell information 
 * directly from the DSA.
 *
 * It exports one major function:
 *
 * X500_process()
 *
 */

#include <gda.h>

/*****************************************************************************/

/* is_X500 is called by the gda in order to determine if the cell name
 *	pointed to by fname_p is really an X500 name.  It returns TRUE
 *	if the name is an X500 name and FALSE otherwise. 
 */

int 
is_X500 (cds_FullName_t *fname_p) /* IN/OUT:  Where to put the results */
{
	register char	*cp = (char *)fname_p->fn_name + 1;
	int looper;


	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_gda,
	    svc_c_debug2,
	    " >is_X500"));

	/*
	 * if there is an "=" sign within the first portion
	 * of the name, claim that the name is an X.500 name. 
	 */

	for(looper = 0;
		looper != fname_p->fn_length && *cp != '/';
		cp++, looper++)
	{
	    if(*cp == '=')
	    {
		DCE_SVC_DEBUG((
		    cds__svc_handle,
		    cds_svc_gda,
		    svc_c_debug2,
		    " <is_X500: return(TRUE)"));

		return(TRUE);
	    }
	    
	}

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_gda,
	    svc_c_debug2,
	    " <is_X500: return(FALSE)"));

	return(FALSE);
}

#ifdef DO_X500
#include <gda_xds.h>

/*
 * X500_process() - Performs two types of operations:
 * 
 *                  1. Takes as input a DECdns cell name whose junction
 *                     is stored in the X.500 directory system. It
 *                     splits the name into a global and DECdns part
 *                     and puts the results in the 'in_out' structure.
 *                     It then makes calls to the XDS/XOM API's
 *                     to read the cell attibutes for the X.500 object
 *                     whose name matches the global name part. It converts
 *                     and puts the attribute values in the 'in_out'
 *                     structure. The function return code is either
 *                     DNS_NAMESERVERERRORS, DNS_SUCCESS,
 *		       DNS_DNS_UNDERSPECIFIEDNAME or DNS_UNKNOWNENTRY.
 *
 */

int 
X500_process (question_el_t *in_out) /* IN/OUT:  Where to put the results */
{
	char		obj_name[DNS_STR_FNAME_MAX + 1];
	x500_dns_cell_t		cell;
	x500_replica_t		replicas [X500_REPLICA_MAX];
	char * cell_buffer;
	char * replica_buffers[X500_REPLICA_MAX];
	int	        i, dns_result, name_len;


      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
          " >x500_process"));

	name_len = sizeof(obj_name);

	/*
	 * Split off the X.500 part of the name 
	 */
	if (x500_rn_split_name(in_out, name_len, obj_name) == X500_ERROR) 
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug1,
		"x500_process: x500 recoverable error: Bad unresolved_in_q name "));

	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <x500_process: x500_rn_split_name failed - return(DNS_NAMESERVERERRORS)"));

	    return(DNS_NAMESERVERERRORS);
	}
	/*
	 * Look up the cell uuid and replica information via XDS interface 
	 */
	ZERO_bytes (&replicas, sizeof(replicas));
	if (x500_xds_lookup (obj_name, &cell, 
			     (x500_replica_t *)&replicas, 
			     &cell_buffer, 
			     (char **)&replica_buffers) == X500_ERROR) 
	{
	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug1,
		"x500_process: x500 recoverable error: Bad unresolved_in_q name"));

	    DCE_SVC_DEBUG((
		cds__svc_handle,
		cds_svc_gda,
		svc_c_debug2,
		" <x500_process: x500_xds_lookup failed - return(DNS_NAMESERVERERRORS)"));

	    return(DNS_NAMESERVERERRORS);
	}
	/*
	 * Convert the cell/replica info (ascii) into internal format 
	 */
	dns_result = x500_rn_convert (in_out, &cell, replicas);

	/*
	 * release attribute buffers 
	 */

	free (cell_buffer);
	for (i=0; i< cell.num_replicas; i++)
	    free (replica_buffers[i]);

	DCE_SVC_DEBUG((
	    cds__svc_handle,
	    cds_svc_gda,
	    svc_c_debug2,
	    " <x500_process"));

	return(dns_result);

}				/* X500_process()                            */

/*****************************************************************************/


/*
 * init_X500 is called during the initialization phase of the gda in order
 * to perform those initialization duties neccessary for the X500 code 
 */
int 
init_X500 ()
{
    return (DNS_SUCCESS);
} 


/*
 * init_X500_process is called before X500_process
 * in order to initialize any X500 data that must be initialized before
 * the call to X500_process. 
 */
int 
init_X500_process (question_el_t *element)
{
    return (DNS_SUCCESS);
}



/*
 * destroy_X500_process is called after X500_process
 * in order to destroy any X500 data that may be initialized during
 * the call to X500_process. 
 */
int 
destroy_X500_process (question_el_t *element)
{
    return (DNS_SUCCESS);
}



/* X500_conduit stores pointers to the functions defined within X500.c which
	should be used to answer rpc queries.  Each element of the func_c
	array of the conduit structure contains a pointer to an initialization
	routine and a processing routine designed to answer a question
	asked of one particular type of rpc call.  X500_conduit also indicates
	the initialization routine for the entire conduit, init_500, and the
	fact that the conduit is single-threaded, 1. */
conduit_el_t	X500_conduit = {
init_X500,
{	{	init_X500_process,	X500_process,	destroy_X500_process },
	{	init_X500_process,	X500_process,	destroy_X500_process },
	{	init_X500_process,	X500_process,	destroy_X500_process },
	{	init_X500_process,	X500_process,	destroy_X500_process },
	{	init_X500_process,	X500_process,	destroy_X500_process },
	{	init_X500_process,	X500_process,	destroy_X500_process },
	{	init_X500_process,	X500_process,	destroy_X500_process },
	{	init_X500_process,	X500_process,	destroy_X500_process },
	{	init_X500_process,	X500_process,	destroy_X500_process },
	{	init_X500_process,	X500_process,	destroy_X500_process },
	{	init_X500_process,	X500_process,	destroy_X500_process },
	{	init_X500_process,	X500_process,	destroy_X500_process },
	{	init_X500_process,	X500_process,	destroy_X500_process },
	{	init_X500_process,	X500_process,	destroy_X500_process },
	{	init_X500_process,	X500_process,	destroy_X500_process },
	{	init_X500_process,	X500_process,	destroy_X500_process },
	{	init_X500_process,	X500_process,	destroy_X500_process }
},1

};
#endif
