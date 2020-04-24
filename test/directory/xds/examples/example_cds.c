/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: example_cds.c,v $
 * Revision 1.1.6.2  1996/02/18  00:24:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:44  marty]
 *
 * Revision 1.1.6.1  1995/12/08  22:12:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:22  root]
 * 
 * Revision 1.1.4.2  1994/07/07  07:02:46  marrek
 * 	Add version string in July 1994 code drop.
 * 	[1994/07/07  06:59:06  marrek]
 * 
 * Revision 1.1.4.1  1994/02/22  16:40:13  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:34:29  marrek]
 * 
 * Revision 1.1.2.2  1993/01/05  20:00:37  htf
 * 	Embedding copyright notice
 * 	[1993/01/05  18:17:57  htf]
 * 
 * Revision 1.1  1992/01/19  14:30:25  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/***************************************************************
*                                                              *
*  COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991   *
*                ALL RIGHTS RESERVED                           *
*                                                              *
***************************************************************/

/************************************************************************/
/*                                                                      */
/* A simple XDS application using CDS.                                  */
/*                                                                      */
/* The workspace is initialized.                                        */ 
/* The Basic Directory Contents Package is negotiated because Directory */
/* object classes and attribute types are used.                         */
/* The application binds to GDS (without credentials).                  */
/* Two entries are added to CDS and a list operation is performed on    */
/* their superior entry.                                                */
/* The two entries are then removed from the Directory and an unbind    */
/* is performed.                                                        */
/* The results of the list operation may then be inspected.             */
/* Finally, the workspace is shutdown.                                  */
/*                                                                      */
/* The CDSROOT of the CDS cell is :   C=IE/O=SNI                        */
/* The following Directory tree is used :                               */
/*                                                                      */
/*                                                                      */
/*                                 O  CountryName="IE"                  */
/*                                 |                                    */
/*                                 O  OrganizationName="SNI"            */
/*                                / \                                   */
/*        TypelessRDN="Brendan"  O   O  TypelessRDN="Sinead"            */
/*       (objectClass=Org.-Person,     (objectClass=Org.-Person,        */
/*            surname="Moloney",            surname="Murphy")           */
/*       userPassword="secret")     telephoneNumber="+49 89 636 0")     */
/*                                                                      */
/*                                                                      */
/************************************************************************/

#include <xom.h>
#include <xds.h>
#include <xdsbdcp.h>
#include <dce/xdscds.h>
#include <example_cds.h>           /* Include file for initialization. */


void
main(
    int  argc,
    char *argv[]
)
{
    OM_private_object bound_session;  /* Holds the Session object which */
                                      /* is returned by ds_bind()       */
    OM_private_object result;         /* Holds the list result object.  */
    OM_sint           invoke_id;      /* Integer for the invoke id      */
                                      /* returned by ds_list(). This    */
                                      /* parameter has to be present    */
                                      /* even though it is ignored.     */

/* Initialize the CDSROOT environment variable:   C=IE/O=SNI            */

    putenv ("CDSROOT=85.4.6=IE/85.4.10=SNI");


/* Initialise a directory workspace for use by XOM.                     */

    if ((workspace = ds_initialize()) == (OM_workspace)0)
        printf("ds_initialize() error\n");


/* Negotiate the use of the BDCP package.                               */

    if (ds_version(features) != DS_SUCCESS)
        printf("ds_version() error\n");


/* Bind (without credentials) to the default GDS server.                */
/* The returned session object is stored in the private object variable */
/* 'bound_session' and is used for all further directory calls.         */

    if (ds_bind(DS_DEFAULT_SESSION, &bound_session) != DS_SUCCESS)
        printf("ds_bind() error\n");


/* Add two entries to the CDS server.                                   */

    if (ds_add_entry(bound_session, DS_DEFAULT_CONTEXT, name2, attr_list1,
                                                      &invoke_id) != DS_SUCCESS)
        printf("ds_add_entry() error\n");

    if (ds_add_entry(bound_session, DS_DEFAULT_CONTEXT, name3, attr_list2,
                                                      &invoke_id) != DS_SUCCESS)
        printf("ds_add_entry() error\n");


/* List all the subordinates of the object referenced by the            */
/* distinguished name:  C=IE/O=SNI                                      */
/* Results are returned in the private object 'result'.                 */

    if (ds_list(bound_session, DS_DEFAULT_CONTEXT, name1, &result, &invoke_id)
                                                                  != DS_SUCCESS)
        printf("ds_list() error\n");


/* Remove the two entries from the CDS Server.                          */

    if (ds_remove_entry(bound_session, DS_DEFAULT_CONTEXT, name2, &invoke_id)
                                                                  != DS_SUCCESS)
        printf("ds_remove_entry() error\n");

    if (ds_remove_entry(bound_session, DS_DEFAULT_CONTEXT, name3, &invoke_id)
                                                                  != DS_SUCCESS)
        printf("ds_remove_entry() error\n");


/* Close the connection to the GDS server.                              */

    if (ds_unbind(bound_session) != DS_SUCCESS)
        printf("ds_unbind() error\n");


/* Extract components from the list result by means of om_get()         */

/* if (om_get(result, ... ) != OM_SUCCESS)                              */
/*      printf("om_get() error\n");                                     */
/*                                                                      */
/* etc ...                                                              */
/*                                                                      */


/* Close the directory workspace.                                       */

    ds_shutdown();
}

