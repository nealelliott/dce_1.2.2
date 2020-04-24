/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: example_gds2.c,v $
 * Revision 1.1.6.2  1996/02/18  00:24:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  23:11:47  marty]
 *
 * Revision 1.1.6.1  1995/12/08  22:12:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  21:45:27  root]
 * 
 * Revision 1.1.4.3  1994/10/05  16:44:24  keutel
 * 	OT 12431
 * 	[1994/10/05  16:36:56  keutel]
 * 
 * Revision 1.1.4.2  1994/07/07  07:02:49  marrek
 * 	Add version string in July 1994 code drop.
 * 	[1994/07/07  06:59:17  marrek]
 * 
 * Revision 1.1.4.1  1994/02/22  16:40:23  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/07  13:35:07  marrek]
 * 
 * Revision 1.1.2.2  1993/01/05  20:00:59  htf
 * 	Embedding copyright notice
 * 	[1993/01/05  18:18:13  htf]
 * 
 * Revision 1.1  1992/01/19  14:30:28  devrcs
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
/* A simple XDS application using GDS.                                  */
/*                                                                      */
/* The workspace is initialized.                                        */ 
/* The Basic Directory Contents Package is negotiated because Directory */
/* object classes and attribute types are used.                         */
/* The Global Directory Service Package is negotiated because an        */
/* authenticated bind to a GDS DSA is performed.                        */
/* [The user's credentials are added to the Directory.]                 */
/* The application binds to GDS (with credentials).                     */
/* A search operation is performed on the subtree :  C=DE               */
/* An unbind is then is performed.                                      */
/* [The user's credentials are removed from the Directory.]             */
/* The results of the list operation may then be inspected.             */
/* Finally, the workspace is shutdown.                                  */
/*                                                                      */
/*                                                                      */
/* In order to perform an authenticated bind the following tree must    */
/* exist (see add_credentials() and remove_credentials() ) :            */
/*                                                                      */
/*               O  CountryName="DE"                                    */
/*               |  (objectClass=Country)                               */
/*               |                                                      */
/*               O  OrganizationName="SNI"                              */
/*               |  (objectClass=Organization)                          */
/*               |                                                      */
/*               O  OrganizationalUnitName="AP11"                       */
/*               |  (objectClass=OrganizationalUnit)                    */
/*               |                                                      */
/*               O  CommonName="Guenther"                               */
/*                  (objectClass=OrganizationalPerson,                  */
/*                   surname="Schmit",                                  */
/*                   telephone="+49 89 636 0",                          */
/*                   userPassword="secret")                             */
/*                                                                      */
/*                                                                      */
/************************************************************************/

#include <xom.h>
#include <xds.h>
#include <xdsbdcp.h>
#include <xdsgds.h>
#include "example_gds.h"            /* Include file for initialization. */

void
main(
    int  argc,
    char *argv[]
)
{
    OM_private_object session;        /* Session object.                */
    OM_private_object bound_session;  /* Holds the Session object which */
                                      /* is returned by ds_bind()       */
    OM_private_object result;         /* Holds the search result object.*/
    OM_private_object pri_name;       /* Dist. name in private format.*/
    OM_sint           invoke_id;      /* Integer for the invoke id      */
                                      /* returned by ds_search(). This  */
                                      /* parameter has to be present    */
                                      /* eventhough it is ignored.      */
    /* char              iapl_name[] = "85.4.6DE";                    */

/* Initialize a directory workspace for use by XOM.                     */

    if ((workspace = ds_initialize()) == (OM_workspace)0)
        printf("ds_initialize() error\n");


/* Negotiate the use of the BDCP and GDS packages.                      */

    if (ds_version(features, workspace) != DS_SUCCESS)
        printf("ds_version() error\n");


/* Add the user's credentials into the Directory in order to permit an  */
/* authenticated bind :                                                 */
/*     requestor:  C=DE/O=SNI/OU=AP11/CN=Guenther                       */
/*     password:   "secret"                                             */

    if (! add_credentials())
        printf("add_credentials() error\n");


/* Create a default session object.                                     */

    if (om_create(DSX_C_GDS_SESSION,OM_TRUE,workspace,&session) != OM_SUCCESS)
        printf("om_create() error\n");


/* Alter the default session object to include the following            */
/* credentials:                                                         */
/*     requestor:  C=DE/O=SNI/OU=AP11/CN=Guenther                       */
/*     password:   "secret"                                             */

    if (om_put(session, OM_REPLACE_ALL, credentials, 0 ,0, 0) != OM_SUCCESS)
        printf("om_put() error\n");


/* Bind with credentials to the default GDS server.                     */
/* The returned session object is stored in the private object variable */
/* bsr and is used for all further directory calls.                     */

    if (ds_bind(session, workspace, &bound_session) != DS_SUCCESS)
        printf("ds_bind() error\n");


/* Create an XOM handle for the private format name.                    */

    omP_create_handle (DS_C_DS_DN, workspace, name1, &pri_name);


/* Search the whole subtree :   C=DE                                    */
/* The filter selects entries with an objectClass attribute.            */
/* The results are returned in the private object 'result'.             */

    if (ds_search(bound_session, DS_DEFAULT_CONTEXT, pri_name, DS_WHOLE_SUBTREE,
               filter1, OM_FALSE, selection, &result, &invoke_id) != DS_SUCCESS)
        printf("ds_search() error\n");


/* Close the connection to the GDS server.                              */

    if (ds_unbind(bound_session) != DS_SUCCESS)
        printf("ds_unbind() error\n");


/* Remove the user's credentials from the Directory                     */

    if (! remove_credentials())
        printf("remove_credentials() error\n");


/* Extract components from the search result by means of om_get()       */

/* if (om_get(result, ... ) != OM_SUCCESS)                              */
/*      printf("om_get() error\n");                                     */
/*                                                                      */
/* etc ...                                                              */
/*                                                                      */


/* Close the directory workspace.                                       */

    if (ds_shutdown(workspace) != DS_SUCCESS)
	printf("ds_shutdown() error\n");
}


int
add_credentials(
    void
)
{
    OM_private_object bound_session;  /* Holds the Session object which */
                                      /* is returned by ds_bind()       */
    OM_sint           invoke_id;      /* Integer for the invoke id      */
    int               error = 0;


/* Bind (without credentials) to the default GDS server.                */

    if (ds_bind(DS_DEFAULT_SESSION, workspace, &bound_session) != DS_SUCCESS)
        error++;


/* Add entries to the GDS server.                                       */

    if (ds_add_entry(bound_session, DS_DEFAULT_CONTEXT, name1, attr_list1,
						      &invoke_id) != DS_SUCCESS)
        error++;

    if (ds_add_entry(bound_session, DS_DEFAULT_CONTEXT, name2, attr_list2,
                                                      &invoke_id) != DS_SUCCESS)
        error++;

    if (ds_add_entry(bound_session, DS_DEFAULT_CONTEXT, name3, attr_list3,
                                                      &invoke_id) != DS_SUCCESS)
        error++;

    if (ds_add_entry(bound_session, DS_DEFAULT_CONTEXT, name4, attr_list4,
                                                      &invoke_id) != DS_SUCCESS)
        error++;


/* Close the connection to the GDS server.                              */

    if (ds_unbind(bound_session) != DS_SUCCESS)
        error++;


    return (error?0:1);
}

int
remove_credentials(
    void
)
{
    OM_private_object bound_session;  /* Holds the Session object which */
                                      /* is returned by ds_bind()       */
    OM_sint           invoke_id;      /* Integer for the invoke id      */
    int               error = 0;


/* Bind (without credentials) to the default GDS server.                */

    if (ds_bind(DS_DEFAULT_SESSION, workspace, &bound_session) != DS_SUCCESS)
        error++;


/* Remove entries from the GDS server.                                       */

    if (ds_remove_entry(bound_session, DS_DEFAULT_CONTEXT, name4, &invoke_id)
                                                                  != DS_SUCCESS)
        error++;

    if (ds_remove_entry(bound_session, DS_DEFAULT_CONTEXT, name3, &invoke_id)
                                                                  != DS_SUCCESS)
        error++;

    if (ds_remove_entry(bound_session, DS_DEFAULT_CONTEXT, name2, &invoke_id)
                                                                  != DS_SUCCESS)
        error++;

    if (ds_remove_entry(bound_session, DS_DEFAULT_CONTEXT, name1, &invoke_id)
								  != DS_SUCCESS)
        error++;

/* Close the connection to the GDS server.                              */

    if (ds_unbind(bound_session) != DS_SUCCESS)
        error++;

    return (error?0:1);
}
