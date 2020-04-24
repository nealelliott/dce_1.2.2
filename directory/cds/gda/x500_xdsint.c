/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: x500_xdsint.c,v $
 * Revision 1.1.4.2  1996/02/18  19:31:35  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:12:35  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:13:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:44  root]
 * 
 * Revision 1.1.2.4  1994/08/16  18:27:47  jd
 * 	drop from janet
 * 	[1994/08/13  16:05:27  jd]
 * 
 * Revision 1.1.2.3  1994/08/03  19:01:49  mccann
 * 	include file cleanup
 * 	[1994/07/19  17:53:01  mccann]
 * 
 * Revision 1.1.2.2  1994/06/09  18:38:17  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:33  devsrc]
 * 
 * Revision 1.1.2.1  1994/03/12  22:01:56  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:10:38  peckham]
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
 * x500_xdsint.c
 *
 * Contains interface to XDS/XOM for the X.500 GDA module.
 * x500_xds_lookup () binds to the default DSA and reads the CDS_CELL and
 * CDS_REPLICAS attributes of of the specified entry.
 * NOTE: during initial implementation (5/93), XDS was not thread safe,
 * so this module is single-threaded.  
 *
 * Format of CDS_CELL attribute value:
 * <cell uuid, in ASCII>
 * 
 * Format of CDS_REPLICAS attribute values:
 * <replica type, 1 octet><clearinghouse uuid, in ASCII><0><clearinghouse name><0>
 * <towers length, 2 octets><tower #1, in ASCII><tower #2, in ASCII>...
 *
 */

#include <gda.h>
#include <malloc.h>
#include <gda_xds.h>

#define IN
#define OUT
#define INOUT
#define X500TRACE 1

#define OM_GET_TEXT "om_get"
#define OM_DELETE_TEXT "om_delete"
#define OM_CREATE_TEXT "om_create"
#define OM_PUT_TEXT "om_put"

/*
 * Local prototypes..
 */

static void 
xds_initialize(
               void);

static int is_xds_initialized = 0;


/*
 * allocate memory for class constants
 */
OM_EXPORT(DS_A_COUNTRY_NAME)
OM_EXPORT(DS_A_LOCALITY_NAME)
OM_EXPORT(DS_A_ORG_NAME)
OM_EXPORT(DS_A_ORG_UNIT_NAME)
OM_EXPORT(DS_A_COMMON_NAME)
OM_EXPORT(DS_A_CDS_CELL)
OM_EXPORT(DS_A_CDS_REPLICAS)
OM_EXPORT(DS_C_ENTRY_INFO_SELECTION)
OM_EXPORT(DS_C_ATTRIBUTE)


/*
 * Attribute selection descriptor:
 * Select CDS_CELL and CDS_REPLICAS, only and
 * return types and values
 */
static OM_descriptor attr_selection[] = {
  OM_OID_DESC (OM_CLASS, DS_C_ENTRY_INFO_SELECTION),
  {DS_ALL_ATTRIBUTES, OM_S_BOOLEAN, OM_FALSE},
  OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_CDS_CELL),
  OM_OID_DESC(DS_ATTRIBUTES_SELECTED, DS_A_CDS_REPLICAS),
  {DS_INFO_TYPE, OM_S_ENUMERATION, {DS_TYPES_AND_VALUES, NULL}},
  OM_NULL_DESCRIPTOR
};
static    OM_workspace workspace;
static    OM_private_object bound_session;
static    pthread_once_t xds_initialize_once = pthread_once_init;

int 
x500_xds_lookup (char             *entry_name,	/* X500 entry name */ 
                 x500_dns_cell_t  *cell_p,	/* cell attribute info (uuid) */
                 x500_replica_t   replicas[],	/* replicas attribute info (uuid, name, towers) */
                 char             **cell_buffer_pp,	/* buffer to be freed by caller */
                 char             *replica_buffers[])	/* list of buffers to be  freed by caller (null-terminated) */
{
    int i,j,len,state = 0, exit_status = X500_ERROR;
    char *attrValue_p;
    OM_private_object dn_object_ptr;
    OM_type included_types[] = 	{0, OM_NO_MORE_TYPES};
    DS_status error;
    OM_return_code return_code;
    OM_private_object read_result;
    OM_public_object entry, attrList, attrType, attrValue;
    OM_sint invoke_id_return;
    char *replica_buf;
    OM_value_position desc_count, attrCount;

    *cell_buffer_pp = NULL;
    *replica_buffers = NULL;
    cell_p->num_replicas = 0;

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
	" >x500_xds_lookup(entry_name = %s)", 
	entry_name));

    pthread_once (&xds_initialize_once, xds_initialize);
    if (!is_xds_initialized) return (X500_ERROR);

    /*
     * convert supplied name to a private DN object
     */
    if (X500_name_to_object(entry_name, &dn_object_ptr, workspace)) 
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug2,
	    " <ERROR: X500_name_to_object(entry_name = %s) failed", 
	    entry_name));

        return (X500_ERROR);
    }
	
    /*
     * okay, look up object
     */
    CHECK_DS_CALL ("ds_read",
		   ds_read (bound_session,
			    DS_DEFAULT_CONTEXT,
			    dn_object_ptr,
			    attr_selection,
			    &read_result,
			    &invoke_id_return));

    included_types[0] = DS_ENTRY;
    CHECK_OM_CALL (OM_GET_TEXT,
		   om_get (read_result,
			   OM_EXCLUDE_ALL_BUT_THESE_TYPES +
			   OM_EXCLUDE_SUBOBJECTS,
			   included_types,
			   OM_FALSE,
			   OM_ALL_VALUES,
			   OM_ALL_VALUES,
			   &entry,
			   &desc_count));
    if (desc_count <= 0) 
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug1,
	    "ERROR: no OM descriptors found"));
	goto cleanup;
    }

    included_types[0] = DS_ATTRIBUTES;
    CHECK_OM_CALL (OM_GET_TEXT,
		   om_get (entry->value.object.object,
			   OM_EXCLUDE_ALL_BUT_THESE_TYPES +
			   OM_EXCLUDE_SUBOBJECTS,
			   included_types,
			   OM_FALSE,
			   OM_ALL_VALUES,
			   OM_ALL_VALUES,
			   &attrList,
			   &attrCount));
    if (attrCount <= 0) 
    {
        DCE_SVC_DEBUG((
            cds__svc_handle,
            cds_svc_gda,
            svc_c_debug1,
	    "ERROR: no cell attributes found"));
	goto cleanup;
    }

    for (i=0;i<attrCount;i++) 
    {
	included_types[0] = DS_ATTRIBUTE_TYPE;
	CHECK_OM_CALL (OM_GET_TEXT,
		       om_get ((attrList+i)->value.object.object,
			       OM_EXCLUDE_ALL_BUT_THESE_TYPES +
			       OM_EXCLUDE_SUBOBJECTS,
			       included_types,
			       OM_FALSE,
			       OM_ALL_VALUES,
			       OM_ALL_VALUES,
			       &attrType,
			       &desc_count));
	if (desc_count <= 0) 
	{
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug1,
		"ERROR: no cell attributes found"));
	    goto cleanup;
	}

	if (memcmp (attrType->value.string.elements, 
		    OMP_O_DS_A_CDS_CELL, 
		    OMP_LENGTH(DS_A_CDS_CELL)) == 0)
	{
	    /*
	     * Found CDS_CELL attribute 
	     */

	    /*
	     * state = reading CDS_CELL 
	     */
	    state = 1;  
	}
	else 
	    if (memcmp (attrType->value.string.elements,
			OMP_O_DS_A_CDS_REPLICAS, 
			OMP_LENGTH(DS_A_CDS_REPLICAS)) == 0)
	    {
		/*
		 * state = reading CDS_REPLICAS 
		 */
		state = 2;   
	    }
	    else
	    {
		/*
		 * state = ignore attribute 
		 */
		state = 0;  
	    }
	
	/*
	 * Get attribute value(s) 
	 */
	included_types[0] = DS_ATTRIBUTE_VALUES;
	CHECK_OM_CALL (OM_GET_TEXT,
		       om_get ((attrList+i)->value.object.object,
			       OM_EXCLUDE_ALL_BUT_THESE_TYPES+
			       OM_EXCLUDE_SUBOBJECTS,
			       included_types,
			       OM_FALSE,
			       OM_ALL_VALUES,
			       OM_ALL_VALUES,
			       &attrValue,
			       &desc_count));
	if (desc_count <= 0) 
	{
            DCE_SVC_DEBUG((
                cds__svc_handle,
                cds_svc_gda,
                svc_c_debug1,
		"ERROR: no cell attribute values found"));      
	    goto cleanup;
	}

	/*
	 * Get each attribute value 
	 */
	for (j=0,attrValue_p = attrValue->value.string.elements, 
	     len = attrValue->value.string.length;
	     j<desc_count; 
	     attrValue_p += len, j++) 
	{
	    if (state == 1) 
	    {  
		/*
		 * get cell uuid 
		 */

		/*
		 * save cell attribute in buffer 
		 */
		*cell_buffer_pp = (char *)malloc (len+1);

		/*
		 * cell attr should be a null-terminated uuid string. 
		 */
		memcpy (*cell_buffer_pp, (char *)attrValue_p, len);

		/*
		 * make sure of null-terminator 
		 */
		*(*cell_buffer_pp + len) = '\0'; 
		if (!x500_str_to_cell((unsigned char *)*cell_buffer_pp, 
				      len, cell_p)) 
		{
                    DCE_SVC_DEBUG((
			cds__svc_handle,
                        cds_svc_gda,
                        svc_c_debug1,
			"ERROR: x500_str_to_cell (%s) failed",
			*cell_buffer_pp));
		    goto cleanup;
		}
	    }
	    else
		if (state == 2) 
	        { 
		    /*
		     * get replica information 
		     */
		
		    /*
		     * save each replica attribute in a buffer 
		     */
		    replica_buf = (char *)malloc(len+1);
		    memcpy (replica_buf, (char *)attrValue_p, len);
		    *(replica_buf+len) = '\0';
		    *replica_buffers++ = replica_buf;
		    cell_p->num_replicas++;
		    if (!x500_str_to_replica((unsigned char *)replica_buf, 
					     len, &replicas[j])) 
		    {
                        DCE_SVC_DEBUG((
                            cds__svc_handle,
                            cds_svc_gda,
                            svc_c_debug1,
			    "ERROR: x500_str_to_replica (%s) failed", 
			    replica_buf));
			goto cleanup;
		    }
		}
	}
    }
    exit_status = 0;

/*
 * Release OM objects
 */
cleanup:
    CHECK_OM_CALL (OM_DELETE_TEXT, om_delete(read_result));
    CHECK_OM_CALL (OM_DELETE_TEXT, om_delete(entry));
    CHECK_OM_CALL (OM_DELETE_TEXT, om_delete(attrList));
    CHECK_OM_CALL (OM_DELETE_TEXT, om_delete(attrType));
    CHECK_OM_CALL (OM_DELETE_TEXT, om_delete(attrValue));
    CHECK_OM_CALL (OM_DELETE_TEXT, om_delete(dn_object_ptr));

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
	" <x500_xds_lookup (%s)", 
        entry_name));

    return (exit_status);
}




/*
 * xds_initialize: initialize a workspace and bind to the
 * default DSA. This is done only once.
 * Sets 'is_xds_initialized' to '1' on success.
 */
static void 
xds_initialize (void)
{
  DS_status  error;

  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_gda,
      svc_c_debug2,
      " >xds_initialize()"));
  /* 
   * Initialize XDS workspace 
   */
  if ((workspace=ds_initialize()) == NULL) 
  {
      DCE_SVC_DEBUG((
          cds__svc_handle,
          cds_svc_gda,
          svc_c_debug2,
	  " <ERROR: xds_initialize : Workspace initialization failed."));

      return;
  }

  /*
   * bind to the DSA defined in 'dua.defaults' file.
   */

  error = ds_bind(DS_DEFAULT_SESSION, workspace, &bound_session);
  if (error != DS_SUCCESS) 
  {
      report_ds_error("ds_bind", error);
  }
  else
  {
      /*
       * success 
       */
      is_xds_initialized = 1; 
  }
  DCE_SVC_DEBUG((
      cds__svc_handle,
      cds_svc_gda,
      svc_c_debug2,
      " <xds_initialize()"));
}


/*
 * report_ds_error()
 *	Extracts the error number from a DS_status return code, prints it
 *	in an error message.
 */
void 
report_ds_error (char       *header, 	/* In--Name of function whose return */
		                        /* code is being checked 	     */

                 DS_status  returnCode)	/* In--Return code to be checked */
{
    OM_type		includeDSProblem[] = { DS_PROBLEM,
					       0 };
    OM_return_code      omStatus;
    OM_public_object	problem;
    OM_value_position	total;
    char buf[100];

    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " >report_ds_error(header=%s, returnCode=%d)",
        header,
        (int)returnCode));
    /*
     * A DS_status return code is an object.  It will be one of the
     * subclasses of the class DS_C_ERROR.  What we want from it is
     * the value of the attribute DS_PROBLEM.
     */
    omStatus = om_get( returnCode,
		       OM_EXCLUDE_ALL_BUT_THESE_TYPES+OM_EXCLUDE_SUBOBJECTS,
		       includeDSProblem,
		       OM_FALSE,
		       OM_ALL_VALUES,
		       OM_ALL_VALUES,
		       &problem,
		       &total );

    /*
     * Make sure we successfully extracted the problem number and print
     * the error message before quitting.
     */
    if( (omStatus == OM_SUCCESS) && (total > 0) )
    {
	sprintf(buf, "%s returned error %d", 
		header, 
		problem->value.enumeration );
    }
    else
    {
	sprintf(buf, "%s failed for unknown reason", header );
    }
    
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug1,
	"ERROR: %s", 
	buf));
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug2,
        " <report_ds_error()"));
}

void 
report_om_error (char            *header,      /* In--Name of function whose */
		                               /*     is being checked */
                 OM_return_code  returnCode) /*In--Return code to be checked */
{
    DCE_SVC_DEBUG((
        cds__svc_handle,
        cds_svc_gda,
        svc_c_debug1,
	"ERROR: %s returned error: %d", 
	header, 
	(int)returnCode));
}


