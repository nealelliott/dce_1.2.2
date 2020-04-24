/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cds.c,v $
 * Revision 1.1.11.2  1996/02/18  00:24:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:41  marty]
 *
 * Revision 1.1.11.1  1995/12/08  22:12:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:16  root]
 * 
 * Revision 1.1.9.2  1994/10/05  16:44:18  keutel
 * 	OT 12431
 * 	[1994/10/05  16:36:41  keutel]
 * 
 * Revision 1.1.9.1  1994/02/22  16:40:01  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:33:52  marrek]
 * 
 * Revision 1.1.7.2  1993/08/12  12:47:56  marrek
 * 	July 1993 code drop.
 * 	[1993/08/04  10:59:55  marrek]
 * 
 * Revision 1.1.5.2  1993/01/05  20:00:16  htf
 * 	Embedding copyright notice
 * 	[1993/01/05  18:17:42  htf]
 * 
 * Revision 1.1.2.3  1992/07/28  20:12:45  melman
 * 	GDS drop of 7/17/92
 * 	[1992/07/28  19:46:56  melman]
 * 
 * Revision 1.1.2.2  1992/06/11  17:42:50  melman
 * 	New GDS test code drop
 * 	[1992/06/11  17:39:25  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: cds.c,v $ $Revision: 1.1.11.2 $ $Date: 1996/02/18 00:24:18 $";
#endif

/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/*
 *
 * A simple XDS application using CDS.
 *
 * The workspace is initialized.
 * The Basic Directory Contents Package is negotiated because Directory
 * object classes and attribute types are used.
 * The application binds to GDS (without credentials).
 * Two entries are added to CDS and a list operation is performed on
 * their superior entry.
 * The two entries are then removed from the Directory and an unbind
 * is performed.
 * The results of the list operation may then be inspected.
 * Finally, the workspace is shutdown.
 *
 * The CDSROOT of the CDS cell is :   C=ie/O=sni/OU=ap
 * The following Directory tree is used :
 *
 *
 *                               O  CountryName="ie"
 *                               |
 *                               O  OrganizationName="sni"
 *                               |
 *                               O  OrganizationUnitName="ap"
 *                              / \
 *              CN="brendan"  O     O  CN="sinead"
 *    (ObjectClass=Org.-Person,       (ObjectClass=Org.-Person,
 *         Surname="Moloney",              Surname="Murphy")
 *           PhoneNbr="+49 89 636 0)   UserPassword="secret")
 *
 *
 */

#ifdef THREADSAFE
#	include <pthread.h>
#endif
#include <xom.h>
#include <xds.h>
#include <xdsbdcp.h>
#include <xdscds.h>
#include "cds.h"                    /* Include file for initialization. */


void
main(
    int  argc,
    char *argv[]
)
{
    OM_workspace      workspace;      /* workspace for objects          */
    OM_private_object bound_session;  /* Holds the Session object which */
                                      /* is returned by ds_bind()       */
    OM_private_object result;         /* Holds the list result object.  */
    OM_sint           invoke_id;      /* Integer for the invoke id      */
                                      /* returned by ds_list(). This    */
                                      /* parameter has to be present    */
                                      /* even though it is ignored.     */

    /*
     * Initialize the CDSROOT environment variable:   C=ie/O=sni/OU=ap
     */
    putenv ("CDSROOT=85.4.6=ie/85.4.10=sni/85.4.11=ap");


    /*
     * Initialize a directory workspace for use by XOM.
     */
    if ((workspace = ds_initialize()) == (OM_workspace)0)
        printf("ds_initialize() error\n");


    /*
     * Negotiate the use of the BDCP package.
     */
    if (ds_version(features, workspace) != DS_SUCCESS)
        printf("ds_version() error\n");


    /*
     * Bind (without credentials) to the default GDS server.
     * The returned session object is stored in the private object variable
     * 'bound_session' and is used for all further directory calls.
     */
    if (ds_bind(DS_DEFAULT_SESSION, workspace, &bound_session) != DS_SUCCESS)
        printf("ds_bind() error\n");


    /*
     * Add two entries to the CDS server.
     */
    if (ds_add_entry(bound_session, DS_DEFAULT_CONTEXT, dn_brendan,
                      attr_list1, &invoke_id) != DS_SUCCESS)
        printf("ds_add_entry() error\n");

    if (ds_add_entry(bound_session, DS_DEFAULT_CONTEXT, dn_sinead,
                      attr_list2, &invoke_id) != DS_SUCCESS)
        printf("ds_add_entry() error\n");


    /*
     * List all the subordinates of the object referenced by the
     * distinguished name:  C=ie/O=sni/OU=ap
     * Results are returned in the private object 'result'.
     */
    if (ds_list(bound_session, DS_DEFAULT_CONTEXT, dn_ap,
	 &result, &invoke_id) != DS_SUCCESS)
        printf("ds_list() error\n");


    /*
     * Remove the two entries from the CDS Server.
     */
    if (ds_remove_entry(bound_session, DS_DEFAULT_CONTEXT, dn_brendan,
                         &invoke_id) != DS_SUCCESS)
        printf("ds_remove_entry() error\n");

    if (ds_remove_entry(bound_session, DS_DEFAULT_CONTEXT, dn_sinead,
                         &invoke_id) != DS_SUCCESS)
        printf("ds_remove_entry() error\n");


    /*
     * Close the connection to the GDS server.
     */
    if (ds_unbind(bound_session) != DS_SUCCESS)
        printf("ds_unbind() error\n");


    /*
     * Extract components from the list result by means of om_get()
     */
    /* if (om_get(result, ... ) != OM_SUCCESS)
     *     printf("om_get() error\n");
     *
     * etc ...
     */


    /*
     * Close the directory workspace.
     */
    if (ds_shutdown(workspace) != DS_SUCCESS)
        printf("ds_shutdown() error\n");
}

