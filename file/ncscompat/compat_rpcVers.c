/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: compat_rpcVers.c,v $
 * Revision 1.1.14.1  1996/10/02  17:54:34  damon
 * 	Newest DFS from Transarc
 * 	[1996/10/01  18:42:18  damon]
 *
 * Revision 1.1.9.3  1994/08/17  15:16:55  ruby
 * 	Implement partial S12Y (Internationalization) for DFS commands and daemons
 * 	[1994/08/04  14:50:34  ruby]
 * 
 * Revision 1.1.7.2  1994/06/29  11:41:35  devsrc
 * 	merged with bl-10
 * 
 * Revision 1.1.7.1  1994/04/28  17:37:03  mbs
 * 	Changed #include with double quotes to #include with angle brackets.
 * 	[1994/04/28  15:59:56  mbs]
 * 
 * Revision 1.1.9.1  1994/06/09  14:13:36  annie
 * 	fixed copyright in src/file
 * 	[1994/06/09  13:26:38  annie]
 * 
 * Revision 1.1.5.6  1993/03/09  19:16:17  jaffe
 * 	Transarc delta: comer-ot7203-upserver-dumps-core 1.1
 * 	  Selected comments:
 * 	    The ncscompat code for saving exported DFS interfaces was not zeroing
 * 	    the installedIf structure.  This caused the upserver to think that it
 * 	    really had 537514177 interfaces exported.
 * 	    memset the malloc'd structure to 0.
 * 	[1993/03/09  17:31:14  jaffe]
 * 
 * Revision 1.1.5.5  1993/01/19  16:08:58  cjd
 * 	embedded copyright notice
 * 	[1993/01/19  15:12:27  cjd]
 * 
 * Revision 1.1.5.4  1992/11/24  17:55:31  bolinger
 * 	Change include file install directory from .../afs to .../dcedfs.
 * 	[1992/11/22  19:10:29  bolinger]
 * 
 * Revision 1.1.5.3  1992/09/25  18:23:48  jaffe
 * 	Transarc delta: cfe-ot5296-rpc-bounds 1.1
 * 	  Selected comments:
 * 	    Many RPC interfaces that used varying arrays were erring in one of two
 * 	    ways:
 * 	    (a) using last_is() attributes for the array control variable
 * 	    in the .idl file, but treating the boundary as if it were
 * 	    specified with length_is()
 * 	    (b) not always initializing the array control variable, particularly
 * 	    when an RPC returned a varying array but took an error exit.
 * 	    Either of these situations could produce a fault_invalid_bound exception
 * 	    from the RPC runtime, when the control variable was given a value outside
 * 	    the allowed range.  In case (a), this could happen when an RPC value was
 * 	    packed full (e.g. an entire [0..24] range was used in a 25-element-max array,
 * 	    and 25 was stored in the control value).  This would be a correct length_is()
 * 	    value, but one too large for a last_is() value.  In case (b), uninitialized
 * 	    storage could take any value, including values outside the [0..24] range.
 * 	    Case (b) might have occurred in Rx-era code, since Rx didn't send bulk
 * 	    return values when any non-zero result was returned from the RPC (unlike
 * 	    DCE RPC, which ships these varying-array OUT parameters regardless of
 * 	    procedure result).
 * 	    Case-(b) error in getting dfs_interfaceList entries.
 * 	[1992/09/23  19:20:47  jaffe]
 * 
 * Revision 1.1.5.2  1992/08/31  20:07:38  jaffe
 * 	Transarc delta: jess-ot2869-com_err-usage-cleanup 1.5
 * 	  Selected comments:
 * 	    The use of com_err in dfs is inconsistent and in some cases
 * 	    incorrect.
 * 	    1) there are many sources which include <com_err.h> but don't
 * 	    use any definitions from it.
 * 	    2) there are sources which use com_err, but don't include the
 * 	    <com_err.h>
 * 	    3) there are subdirectories which do not need libcom_err.a for
 * 	    any reason that link with it.
 * 	    This delta is created to clean up the usage of com_err in dfs.
 * 	    Delete line "#include <com_err.h>
 * 	[1992/08/30  03:02:10  jaffe]
 * 
 * Revision 1.1.3.4  1992/01/25  20:48:20  zeliff
 * 	dfs6.3 merge, part2
 * 	[1992/01/25  20:06:36  zeliff]
 * 
 * $EndLog$
 */
/*
 * Copyright (C) 1991, 1995 Transarc Corporation
 * All Rights Reserved
 */
/*
 * compat_rpcVers.c -- interface installation and recovery routines for 
 *		       the DFS versioning mechanism
 */

#include <dcedfs/stds.h>  /* include appropriate standards file */

#include <dcedfs/param.h>
#include <sys/types.h>
#include <string.h>
#include <stdio.h>
#include <dce/rpc.h>
#include <pthread.h>

#include <compat.h>

#include <dce/dce.h>
#include <dce/dce_msg.h>
#include <dce/dcesvcmsg.h>
#include <dfsncsmac.h>
#include <dfsncssvc.h>
#include <dfsncsmsg.h>

extern int  compat_svc_must_init;
extern void compat_svc_init();

#define INIT_SVC if (compat_svc_must_init) compat_svc_init()

#define IS_COMM_ERR(s)  ((s) == rpc_s_ok ? 0 : 1)
#define ERROR(n)        { code = n; goto error_exit; }
#define ENOINTERFACE    -1
#define MAXINTERFACESPERSERVER 10

static dfs_interfaceArray *interfaceListChain[MAXINTERFACESPERSERVER];
static unsigned long interfaceNextSlot;

/* error recovery macros */
#define CHK_RPC_ERR(st)                 \
    if ( st != rpc_s_ok )               \
    {                                   \
        unsigned32 inqStatus;           \
        ndr_char errorText[256];        \
                                        \
        dce_error_inq_text(st, errorText, &inqStatus);                  \
        goto error_exit;                      \
    }

/*
 * dfs_installInterfaceDescription() -- install the interface that is to be
 * registered by the server to the versioning runtime. Servers keep track
 * of the different versions of each of the interfaces they export.
 * 
 * PARAMETERS: 	if_spec is the interface to be registered
 *		orig_if_spec is the original interface that is being extended
 *		vers_provider is the provider_version number of this interface
 *		code is the returned error indicator
 * LOCKS USED: none
 *
 * PERFORMANCE: nothing complex, see code
 * 
 * ASSUMPTIONS: none
 *
 * CAUTIONS: none
 *
 * SIDE EFFECTS: none
 *
 * RETURN CODES: 0 if everything is ok
 *		 non-zero if some error occurs. Use dce_error_inq_text() to
 *		 interpret the error
 */

void
dfs_installInterfaceDescription ( if_spec, orig_if_spec, 
				 vers_provider, if_text, code)
  rpc_if_handle_t if_spec, orig_if_spec;
  register unsigned32 vers_provider;
  register unsigned_char_t *if_text;
  register error_status_t *code;

{
    register unsigned int i, j;
    register dfs_interfaceDescription *serverInterface;
    register dfs_interfaceArray *installedIf;
    register unsigned int sameInterfaces;
    rpc_if_id_t origId, installedId, if_id;

    INIT_SVC;

    rpc_if_inq_id (orig_if_spec,
                   &origId,
                   code);
    CHK_RPC_ERR (*code);
    rpc_if_inq_id (if_spec,
                   &if_id,
                   code);
    CHK_RPC_ERR (*code);

    if ( memcmp(&if_id.uuid, &origId.uuid, sizeof(uuid_t)) == 0 &&
        if_id.vers_major == origId.vers_major &&
        if_id.vers_minor == origId.vers_minor )
        sameInterfaces = 1;
    else
        sameInterfaces = 0;

    /* Search to see if orig_if_spec has already been installed */
    for (i=0; interfaceListChain[i]; i++)  {
        /* compare the interface ids */
        memcpy (&installedId,
                &interfaceListChain[i]->if_id,
                sizeof(rpc_if_id_t));

        if ( memcmp(&installedId.uuid, &origId.uuid, sizeof(uuid_t)) == 0 &&
             installedId.vers_major == origId.vers_major &&
             installedId.vers_minor == origId.vers_minor )  {

            if (sameInterfaces) {
                dce_svc_printf(NCS_S_CV_ALREADY_INSTALLED_MSG);
                *code = -1;
                goto error_exit;
            } /*if*/

	    installedIf = interfaceListChain[i];

	    /* Search to see if if_spec has already been installed */
	    for (j=0; j<installedIf->count; j++) {
		memcpy (&installedId,
			&installedIf->interfaces[j],
			sizeof(rpc_if_id_t));

		if ( memcmp(&installedId.uuid, &if_id.uuid, sizeof(uuid_t)) \
		    == 0 && installedId.vers_major == if_id.vers_major &&
		    installedId.vers_minor == if_id.vers_minor ) {
		    dce_svc_printf(NCS_S_CV_ALREADY_INSTALLED_MSG);
		    *code = -1;
		    goto error_exit;
		}
	    }

	    /* Allocate for the new interface version */
	    installedIf->interfaces[installedIf->count] = \
		(dfs_interfaceDescription *)malloc 		\
		    (sizeof(dfs_interfaceDescription));
	    if (installedIf->interfaces[installedIf->count] == NULL) {
		dce_svc_printf(NCS_S_CV_NO_MEMORY_FOR_INTERFACE_MSG);
		exit(1);
	    }

	    installedIf->interfaces[installedIf->count]->interface_uuid = \
		if_id.uuid;
	    installedIf->interfaces[installedIf->count]->vers_major = \
		if_id.vers_major;
	    installedIf->interfaces[installedIf->count]->vers_minor = \
		if_id.vers_minor;
	    installedIf->interfaces[installedIf->count]->
		vers_provider = vers_provider;
	    if (if_text) {
		memcpy (installedIf->interfaces[installedIf->count]->spareText,
			if_text,
			MAXSPARETEXT-1);

                    installedIf->interfaces[installedIf->count]->
                        spareText[MAXSPARETEXT-1] = '\0';
                    installedIf->count++;
                } /*if*/
                goto exit; /* we're done */
        } /*if*/
    }/*for*/

    /* interface not registered previously, see if its a new one */
    if (sameInterfaces) { /* first time this interface family is installed */
	interfaceListChain[interfaceNextSlot] = (dfs_interfaceArray *) \
	    malloc (sizeof(dfs_interfaceArray));
	if (interfaceListChain[interfaceNextSlot] == NULL) {
	    dce_svc_printf(NCS_S_CV_NO_MEMORY_FOR_INTERFACE_MSG);
	    exit(1);
	}
	
        installedIf = interfaceListChain[interfaceNextSlot++];
	memset (installedIf, 0, sizeof(dfs_interfaceArray));

        installedIf->if_id.uuid = if_id.uuid;
        installedIf->if_id.vers_major = if_id.vers_major;
        installedIf->if_id.vers_minor = if_id.vers_minor;

	installedIf->interfaces[0] = \
	  (dfs_interfaceDescription *)malloc(sizeof(dfs_interfaceDescription));
	if (installedIf->interfaces[0] == NULL) {
	    dce_svc_printf(NCS_S_CV_NO_MEMORY_FOR_INTERFACE_MSG);
	    exit(1);
	}

        installedIf->interfaces[0]->interface_uuid=if_id.uuid;
        installedIf->interfaces[0]->vers_major = if_id.vers_major;
        installedIf->interfaces[0]->vers_minor = if_id.vers_minor;
        installedIf->interfaces[0]->vers_provider = vers_provider;
        if (if_text) {
            memcpy (installedIf->interfaces[0]->spareText,
		    if_text,MAXSPARETEXT-1);
            installedIf->interfaces[0]->spareText[MAXSPARETEXT-1] = '\0';
            installedIf->count++;
	}
    }

    else { /* error, orig_if_spec not installed */
        dce_svc_printf(NCS_S_CV_ORIGINAL_NOT_INSTALLED_MSG);
        *code = -1;
        goto error_exit;
    }

error_exit:
    /* an error occurred */
exit:
    /* cleanup */
    return;
}

/*
 * dfs_printInterfaceDescription() -- print an interface description. Used as
 * a debugging tool
 * 
 * PARAMETERS: 	interfaces is the bulk parameter of a set of interfaces
 *		code is the error indicator
 *
 * LOCKS USED: none
 *
 * PERFORMANCE: nothing complex, see code
 * 
 * ASSUMPTIONS: none
 *
 * CAUTIONS: none
 *
 * SIDE EFFECTS: none
 *
 * RETURN CODES: none
 */

void
dfs_printInterfaceDescription ( interfaces, code )
  register dfs_interfaceList *interfaces;
  register error_status_t *code;

{
    register int i,j;
    register dfs_interfaceArray *if_val;
    register dfs_interfaceDescription *if_desc;
    unsigned_char_t *uuidStr;

    INIT_SVC;
    *code = 0;
    
    dce_svc_printf(NCS_S_CV_INTERFACE_NUMBER_MSG);
    if_desc = interfaces->dfs_interfaceList_val;
    for (i=0; i<interfaces->dfs_interfaceList_len; i++) {
	    uuid_to_string (&(if_desc[i].interface_uuid), 
			    &uuidStr, code);
	    if (IS_COMM_ERR(*code)) 
		continue;
	    else
		dce_svc_printf(NCS_S_CV_INTERFACE_DATA_MSG, i+1, uuidStr,
			if_desc[i].vers_major, if_desc[i].vers_minor, 
			if_desc[i].vers_provider, if_desc[i].spareText);
	    if (uuidStr) rpc_string_free (&uuidStr, code);
	}
}

/*
 * dfs_GetServerInterfaces() -- called by client applications to get a list
 * of different versions of a server exported interface
 * 
 * PARAMETERS: if_spec is the interface for which a list of version is
 *		requested
 *		interfaces is the bulk parameter that is returned to the 
 *		client. It contains the list of versions of the interface
 *		code is the error indicator
 *
 * LOCKS USED: none
 *
 * PERFORMANCE: nothing complex, see code
 * 
 * ASSUMPTIONS: none
 *
 * CAUTIONS: none
 *
 * SIDE EFFECTS: none
 *
 * RETURN CODES: none
 */

void
dfs_GetServerInterfaces (if_spec, interfaces, code)
  rpc_if_handle_t if_spec;     
  register dfs_interfaceList *interfaces;
  register error_status_t *code;

{
    int i, j;
    dfs_interfaceArray *installedIf;
    dfs_interfaceDescription *ptr;
    rpc_if_id_t if_id;

    *code = 0;
    interfaces->dfs_interfaceList_len = 0;	/* prevent faults in callers */
    rpc_if_inq_id (if_spec,
                   &if_id,
                   code);
    CHK_RPC_ERR (*code);

    /* Get the correct set of interfaces requested */
    for (i=0; i<interfaceNextSlot; i++) 
	installedIf = interfaceListChain[i];
	if ( memcmp (&if_id, &installedIf->if_id, 
		     sizeof(rpc_if_id_t)) == 0 )  {
	    interfaces->dfs_interfaceList_len = installedIf->count;
	    memset (interfaces->dfs_interfaceList_val, 0,
		    MAXINTERFACESPERSERVER * sizeof(dfs_interfaceArray));
	    ptr = interfaces->dfs_interfaceList_val;
	    for (j=0; j<installedIf->count; j++) {
		memcpy (ptr, installedIf->interfaces[j],
			sizeof(dfs_interfaceDescription));
		ptr += sizeof(dfs_interfaceDescription);
	    }
	    return;
	}
    *code = -1;

error_exit:
    return;
}    

/*
 * dfs_sameInterface() -- return a match status after matching an interface
 * in a list of server supplied versions of an interface
 * 
 * PARAMETERS: if1 and if1_provider are the interfaces that the client desires
 *		serverInterfaces is the list of versions of a particular 
 *		interface that the server returned to the client.
 *
 * LOCKS USED: none
 *
 * PERFORMANCE: nothing complex, see code
 * 
 * ASSUMPTIONS: none
 *
 * CAUTIONS: none
 *
 * SIDE EFFECTS: none
 *
 * RETURN CODES: MATCH_GOOD, MATCH_MEDIUM or MATCH_BAD
 */

long
dfs_sameInterface (if1, if1_provider, serverInterfaces)
  rpc_if_handle_t if1;
  unsigned int if1_provider;
  dfs_interfaceList *serverInterfaces;

{
    error_status_t code;
    int match_status=MATCH_BAD;
    long provider_diff;
    long min_provider_diff = -1;
    rpc_if_id_t if_id1, if_id2;
    register int i;

    rpc_if_inq_id (if1, &if_id1, &code);
    if (IS_COMM_ERR(code))
	goto exit;

    for (i=0; i<serverInterfaces->dfs_interfaceList_len;i++) {

	memcpy ( &if_id2, &serverInterfaces->dfs_interfaceList_val[i],
		sizeof(rpc_if_id_t) );
	if (memcmp( &if_id1.uuid, &if_id2.uuid, sizeof(uuid_t) ) == 0 &&
	    if_id1.vers_major == if_id2.vers_major &&
	    if_id1.vers_minor <= if_id2.vers_minor ) {

	    if ( if1_provider <= serverInterfaces->dfs_interfaceList_val[i].\
		vers_provider) {
		match_status = MATCH_GOOD;
		break;
	    }
	    else  {
		if (match_status == MATCH_BAD)
		    match_status = MATCH_MEDIUM;
	    }/* else */

	}/* if */

    }/* for */

  exit:
    return match_status;
}
