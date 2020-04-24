/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: x500_addcell.c,v $
 * Revision 1.1.6.1  1996/10/03  14:34:21  arvind
 * 	OSF DCE 1.2.2 Drop 4
 * 	[1996/10/03  13:18:03  arvind]
 *
 * Revision 1.1.4.2  1996/02/18  19:31:31  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:12:32  marty]
 * 
 * Revision 1.1.4.1  1995/12/08  15:13:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:41:41  root]
 * 
 * Revision 1.1.2.6  1994/08/03  19:01:47  mccann
 * 	fix assert
 * 	[1994/07/21  19:53:15  mccann]
 * 
 * 	include file cleanup
 * 	[1994/07/19  17:52:29  mccann]
 * 
 * Revision 1.1.2.5  1994/06/23  18:30:12  mccann
 * 	sams cleanup drop1
 * 	[1994/06/22  19:28:25  mccann]
 * 
 * Revision 1.1.2.4  1994/06/09  18:38:15  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:10:31  devsrc]
 * 
 * Revision 1.1.2.3  1994/06/08  20:17:57  peckham
 * 	Initialize serviceability and message catalogs.
 * 	[1994/05/31  18:56:39  peckham]
 * 
 * Revision 1.1.2.2  1994/05/12  21:11:19  peckham
 * 	Eliminate dnstables reference.
 * 	[1994/05/12  19:15:18  peckham]
 * 
 * Revision 1.1.2.1  1994/03/12  22:01:54  peckham
 * 	DEC serviceability and i18n drop
 * 	[1994/03/12  14:10:24  peckham]
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
 * Module: addcell.c
 * Function: Registers a DCE cell name in X.500.
 * Inputs: -c <cell name> (e.g. -c /c=us/o=dec/ou=lkg)
 *         -o <obj class> object class (expressed as an index into the table 
 *                'allowableObjectClasses', below)
 * If the cell entry is already registered in X.500, the CDS_CELL and
 * CDS_REPLICAS attributes are replaced with new values for this cell.
 *
 * Returns: 0 on success, 1 on error and prints message to stderr.
 *             
*/


/*
 * xds and xom libraries
 */
#include <gda.h>
#include <dce/assert.h>
#include <dce/dcesvcmsg.h>
#include <dce/rpc.h>
#include <gda_xds.h>
#include <locale.h>
/*
 * Local Prototypes.
 */

int 
main (	
      int   ,
      char  **);

static void 
init (int ,
      char **,
      char **,
      int *);

static void
print_usage(
	    char **argv);



#define OM_DELETE_TEXT "om_delete"
#define OM_CREATE_TEXT "om_create"
#define OM_PUT_TEXT "om_put"
/* Put C-style (null-terminated) string in object */
#define FILL_OMD_STRING(desc, index, typ, syntx, val, len)   \
	desc[index].type = typ; \
	desc[index].syntax = syntx; \
	desc[index].value.string.length = (OM_string_length)len; \
	desc[index].value.string.elements = val;
/* Put other value in object */
#define FILL_OMD_STRUCT( desc, index, typ, syntx, val ) \
        desc[index].type = typ; \
        desc[index].syntax = syntx; \
        desc[index].value.object.padding = 0; \
        desc[index].value.object.object = val;
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
OM_EXPORT(DS_C_ATTRIBUTE_LIST)
OM_EXPORT(DS_A_OBJECT_CLASS)
OM_EXPORT(DS_O_TOP)
OM_EXPORT(DS_O_COUNTRY)
OM_EXPORT(DS_O_ORG)
OM_EXPORT(DS_O_ORG_UNIT)
OM_EXPORT(DS_O_LOCALITY)
OM_EXPORT(DS_O_APPLIC_ENTITY)
OM_EXPORT(DS_O_APPLIC_PROCESS)
OM_EXPORT(DS_O_PERSON)
OM_EXPORT(DS_O_ORG_ROLE)
OM_EXPORT(DS_O_GROUP_OF_NAMES)
OM_EXPORT(DS_O_DEVICE)
OM_EXPORT(DS_C_ENTRY_MOD_LIST)
OM_EXPORT(DS_C_ENTRY_MOD)

/*
 * Table of allowable object class for a DCE cell name
 * NOTE: these match the SNI schema (of DCE 1.0.2). They are
 * all sublasses of 'top'.
*/

static OM_descriptor allowedObjectClasses [] = {           /* index */
  OM_OID_DESC (DS_ATTRIBUTE_VALUES, DS_O_ORG_UNIT),        /*   0   */
  OM_OID_DESC (DS_ATTRIBUTE_VALUES, DS_O_ORG),             /*   1   */
  OM_OID_DESC (DS_ATTRIBUTE_VALUES, DS_O_ORG_ROLE),        /*   2   */
  OM_OID_DESC (DS_ATTRIBUTE_VALUES, DS_O_COUNTRY),         /*   3   */
  OM_OID_DESC (DS_ATTRIBUTE_VALUES, DS_O_LOCALITY),        /*   4   */
  OM_OID_DESC (DS_ATTRIBUTE_VALUES, DS_O_APPLIC_ENTITY),   /*   5   */
  OM_OID_DESC (DS_ATTRIBUTE_VALUES, DS_O_APPLIC_PROCESS),  /*   6   */
  OM_OID_DESC (DS_ATTRIBUTE_VALUES, DS_O_GROUP_OF_NAMES),  /*   7   */
  OM_OID_DESC (DS_ATTRIBUTE_VALUES, DS_O_DEVICE),           /*   8   */
  OM_OID_DESC (DS_ATTRIBUTE_VALUES, DS_O_PERSON)           /*   9   */
};
/*
 * To hold the attributes we want to attach to the name being added.
 */
static OM_descriptor xdsObjectClass[] = {

    /* This object is an attribute--an object class. */
    OM_OID_DESC( OM_CLASS,            DS_C_ATTRIBUTE ),
    OM_OID_DESC( DS_ATTRIBUTE_TYPE,   DS_A_OBJECT_CLASS ),

    /* Not only must the class be listed, but also all */
    /* its superclasses.                               */
    OM_OID_DESC( DS_ATTRIBUTE_VALUES, DS_O_TOP ),
    /* Place holder for object class */
    OM_NULL_DESCRIPTOR,
    /* Null terminator */
    OM_NULL_DESCRIPTOR
};
static OM_descriptor xdsCell[] = {

    /* This object is an attribute--a CDS CELL. */
    OM_OID_DESC( OM_CLASS,          DS_C_ATTRIBUTE ),
    OM_OID_DESC( DS_ATTRIBUTE_TYPE, DS_A_CDS_CELL ),

    /* No default--so we need a place holder for the actual data. */
    OM_NULL_DESCRIPTOR,

    /* Null terminator */
    OM_NULL_DESCRIPTOR
};
/* The replicas attribute value(s) - filled in at runtime */
static OM_descriptor xdsReplicas[X500_REPLICA_MAX+2];

static OM_descriptor xdsAttributesToAdd[] = {

    /* This object is an attribute list. */
    OM_OID_DESC( OM_CLASS, DS_C_ATTRIBUTE_LIST ),

    /* These are "pointers" to the attributes in the list. */
    { DS_ATTRIBUTES, OM_S_OBJECT, { 0, xdsObjectClass } },
    { DS_ATTRIBUTES, OM_S_OBJECT, { 0, xdsCell } },
    { DS_ATTRIBUTES, OM_S_OBJECT, { 0, xdsReplicas } },

    /* Null terminator */
    OM_NULL_DESCRIPTOR
};
static OM_descriptor xdsRemoveCell[] = {
    OM_OID_DESC (OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_REMOVE_ATTRIBUTE, NULL}},
    OM_OID_DESC( DS_ATTRIBUTE_TYPE, DS_A_CDS_CELL ),
    OM_NULL_DESCRIPTOR
  };

static OM_descriptor xdsRemoveReplicas[] = {
    OM_OID_DESC (OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_REMOVE_ATTRIBUTE, NULL}},
    OM_OID_DESC( DS_ATTRIBUTE_TYPE, DS_A_CDS_REPLICAS ),
    OM_NULL_DESCRIPTOR
  };

static OM_descriptor xdsAddCell[] = {
    OM_OID_DESC (OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_ATTRIBUTE, NULL}},
    OM_OID_DESC( DS_ATTRIBUTE_TYPE, DS_A_CDS_CELL ),
    OM_NULL_DESCRIPTOR, /* place holder for CDS_CELL attr. */
    OM_NULL_DESCRIPTOR, /* place holder for CDS_CELL value */
    OM_NULL_DESCRIPTOR
  };

/* This is the fixed data for the 'add replicas object', defined below
*/
static OM_descriptor xdsAddReplicasHeader[] = {
    OM_OID_DESC (OM_CLASS, DS_C_ENTRY_MOD),
    {DS_MOD_TYPE, OM_S_ENUMERATION, {DS_ADD_ATTRIBUTE, NULL}},
    OM_OID_DESC( DS_ATTRIBUTE_TYPE, DS_A_CDS_REPLICAS ),
  };
/*
 * This array holds the fixed and variable data for the 'add replica object'
 */
static OM_descriptor xdsAddReplicas[X500_REPLICA_MAX];


/* This describes the cell/replicas modify - CDS_CELL and CDS_REPLICAS
 * attributes are removed, then added with new values.
*/
static OM_descriptor xdsModList[] = {
    OM_OID_DESC (OM_CLASS, DS_C_ENTRY_MOD_LIST),
    {DS_CHANGES, OM_S_OBJECT, {0, xdsRemoveCell} },
    {DS_CHANGES, OM_S_OBJECT, {0, xdsRemoveReplicas}},
    {DS_CHANGES, OM_S_OBJECT, {0, xdsAddCell}},
    {DS_CHANGES, OM_S_OBJECT, {0, xdsAddReplicas}},
    OM_NULL_DESCRIPTOR
  };


#define MAX_UUID_STRING_LEN  50 /* max length of uuid string */
#define MAX_TOWER_STRING_LEN 50 /* max length of tower string */

int 
main (int   argc,
      char  *argv[])
{
    OM_private_object dn_object_ptr;
    OM_private_object bound_session;
    OM_workspace	workspace;
    OM_return_code return_code;
    OM_sint invokeID;
    OM_type		includeDSProblem[] = { DS_PROBLEM,
					       0 };
    OM_return_code      omStatus;
    OM_public_object	problem;
    OM_value_position	total;
    DS_status error;
    char *cellname=NULL, cell_uuid_string[50];
    ReplicaInfo_t replica_info[X500_REPLICA_MAX];
    char *myname_p, *p, **pp, *tower_p, **tower_pp, *replica_string;
    char svcname[128];
    char * replica_string_list[X500_REPLICA_MAX+1];
    error_status_t st;
    int  r_index, num_replicas, replicas_len, num_towers = 0, towers_len;
    int object_class_index=-1, exit_status;


    /*
     * Get our filename, stripped of directories
     */
    if (myname_p = strrchr(argv[0], '/'))
	myname_p++;
    else
	myname_p = argv[0];

    setlocale(LC_ALL, "");

    (void)sprintf(svcname, "%s(%ld)", myname_p, (long)getpid());
    dce_svc_set_progname(svcname, &st);
    dce_assert(cds__svc_handle, (st == svc_s_ok));

    dce_msg_define_msg_table(cds_msg_table,
                             sizeof(cds_msg_table) / sizeof(cds_msg_table[0]),
                             &st);
    dce_assert(cds__svc_handle, (st == msg_s_ok));

    init (argc, argv, &cellname, &object_class_index);

    if ((cellname == NULL) || (object_class_index < 0)) 
    {
	print_usage(argv); /* exit */
    }


    /* 
     * The object class value passed in is actually the index into the
     * table of allowable object classes for cell names, above.
     */
    xdsObjectClass[3] = allowedObjectClasses[object_class_index];

    /*
     * Get namespace uuid, clearinghouse type, uuid, name, and towers
     * for our cell.
     */
    ZERO_bytes (cell_uuid_string, sizeof(cell_uuid_string));
    exit_status = get_cell_info (cell_uuid_string, replica_info,&num_replicas);

    if (exit_status || (num_replicas == 0))
    {
	dce_fprintf(stderr, cds_s_addcell_cellinfo, (long)exit_status);
	exit(1);
    }
    
    /* 
     * Initialize XDS workspace 
     */
    if ((workspace=ds_initialize()) == NULL) 
    {
	dce_fprintf(stderr, cds_s_addcell_workspace);
	exit(1);
    }
    
    if (X500_name_to_object(cellname, &dn_object_ptr, workspace)) 
    {
	dce_fprintf(stderr, cds_s_addcell_nameobj);
	exit(1);
    }

    /* 	
     * Convert cell attribute strings to OM objects.
     * Append 3 nulls to cell uuid (SNI expects an optional root uuid and root
     * name, which are no longer used).
     */
    
    FILL_OMD_STRING (xdsCell,2,DS_ATTRIBUTE_VALUES,OM_S_OCTET_STRING,cell_uuid_string, strlen(cell_uuid_string)+3); /* include 3 null bytes at end */


    /* 
     * Convert each replica into an OM object :
     */

    OMX_CLASS_DESC (xdsReplicas[0], DS_C_ATTRIBUTE); /* attribute object*/
    OMX_ATTR_TYPE_DESC (xdsReplicas[1], DS_ATTRIBUTE_TYPE, DS_A_CDS_REPLICAS);

    /* 	
     * allocate space for the each replica attribute value
     * assume tower list is same length for each replica.
     */
    tower_pp = (char **)replica_info[0].towers;
    while (*tower_pp++ != NULL) ++num_towers;

    for (r_index=0;r_index<num_replicas;r_index++) 
    {
	/*
	 * for each replica 
	 */
	replica_string = p = malloc ((replicas_len = 
				      sizeof (int) + /* see ReplicaInfo_t */
				      MAX_UUID_STRING_LEN +
				      CDS_STR_SNAME_MAX+strlen("/.../") +
				      (num_towers*MAX_TOWER_STRING_LEN)));
	ZERO_bytes (replica_string, replicas_len);
	replica_string_list[r_index] = replica_string;/* free buffer later */
	*p++ = replica_info[r_index].clh_type;            /* replica type */

	/*
	 * clearinghouse uuid 
	 */
	strcpy (p, replica_info[r_index].clh_uuid);       
	p += strlen(p) + 1; /* leave null byte at end */

	/*
	 * clearinghouse name
	 */
	strcpy (p, replica_info[r_index].clh_name);       
	p += strlen(p) + 1; /* leave null byte at end */

	/*
	 * sum up towers lengths 
	 */
	for (tower_pp = (char **)replica_info[r_index].towers,
	     towers_len = 0;
	     (tower_p = *tower_pp) != NULL;
	     ++tower_pp) 
	{
	    towers_len += strlen(tower_p) + 1; /* add 1 for null byte */
	}
	sprintf (p, "%04d", towers_len);
	p += 4;

	/*
	 * now insert towers 
	 */
	for (tower_pp = (char **)replica_info[r_index].towers;
	     (tower_p = *tower_pp) != NULL;
	     ++tower_pp) 
	{
	    strcpy (p, tower_p);
	    p += strlen(tower_p) + 1;
	}

	/*
	 * free mem. from 'dnsCvtTowerToStr' 
	 */
	free ((char *)replica_info[r_index].towers);  
	replicas_len = p - replica_string;
	FILL_OMD_STRING (xdsReplicas,r_index+2,
			 DS_ATTRIBUTE_VALUES,OM_S_OCTET_STRING,
			 replica_string, replicas_len);
    }
    
    /*	
     * end of buffer list 
     */
    replica_string_list[r_index] = NULL; 

    /*
     * insert a NULL descriptor to end the attribute value list 
     */
    OMX_OM_NULL_DESC (xdsReplicas[r_index+2]);
    
    /*
     * bind to the DSA 
     */
    CHECK_DS_CALL ("ds_bind",
		   ds_bind(DS_DEFAULT_SESSION, workspace, &bound_session));
    
    error = ds_add_entry( bound_session, DS_DEFAULT_CONTEXT,
			 dn_object_ptr, xdsAttributesToAdd, &invokeID);
    if (error != DS_SUCCESS) 
    {
	/*
	 * get the return code object and check the error 
	 */
	omStatus = om_get( error,
			  OM_EXCLUDE_ALL_BUT_THESE_TYPES+OM_EXCLUDE_SUBOBJECTS,
			  includeDSProblem,
			  OM_FALSE,
			  OM_ALL_VALUES,
			  OM_ALL_VALUES,
			  &problem,
			  &total );

	/*		
	 * Make sure we successfully extracted the problem number.
	 */
	if( (omStatus == OM_SUCCESS) && (total > 0) ) 
	{
	    if (problem->value.enumeration == DS_E_ENTRY_EXISTS) 
	    {
		/*	
		 * Entry exists-modify the attributes 
		 */
		/* Copy CDS_CELL attribute to required object for 'ds_modify'*/
		xdsAddCell[2] = xdsCell[1]; 
		xdsAddCell[3] = xdsCell[2];

		/*
		 * Copy CDS_REPLICAS fixed data part to required object 
		 */
		memcpy (&xdsAddReplicas[0], &xdsAddReplicasHeader, 
			3*sizeof (OM_descriptor));

		/*
		 * Copy CDS_REPLICAS variable data part to required object 
		 */
		memcpy (&xdsAddReplicas[3], &xdsReplicas[2], 
			X500_REPLICA_MAX*sizeof (OM_descriptor));
		CHECK_DS_CALL ("ds_modify_entry",
			       ds_modify_entry( bound_session, 
					       DS_DEFAULT_CONTEXT,
					       dn_object_ptr, xdsModList, 
					       &invokeID));

		dce_fprintf(stdout, cds_s_addcell_entryexists, cellname);
		exit_status = 0;
	    }
	    else 
	    {
		report_ds_error ("ds_add_entry", error);
		exit_status = 1;
	    }
	}
	else 
	{
	    /*
	     * om_get failed!  programming bug? 
	     */
	  report_om_error("om_get", omStatus);
	  exit_status = 1;
	}
    }
    else 
    {
	/*
	 * add entry succeeded 
	 */
	dce_fprintf(stdout, cds_s_addcell_success);
	exit_status = 0;
    }

    /* 
     * Free replica value buffer(s)
     */
    for (pp = replica_string_list;*pp != NULL;++pp)
    {
	free (*pp);
    }
    
    /*
     * unbind
     */
    CHECK_DS_CALL ("ds_unbind",
		   ds_unbind(bound_session));

    /*
     * shutdown
     */
    CHECK_OM_CALL (OM_DELETE_TEXT, om_delete(bound_session));
    CHECK_DS_CALL ("ds_shutdown", ds_shutdown(workspace));

    exit (exit_status);
}

/********************************************************************/

static void 
init (int   argc,
      char  **argv,
      char  **cellname_p,
      int   *object_class_index_p)
{
  extern char 	*optarg;
  int 		c, inq_sts;


  /*
   * Message routing variables
   */
  error_status_t 	w_err_st;

  while ((c = getopt (argc, argv, "c:o")) != EOF) 
  {
    switch (c) {
    case 'c':
      *cellname_p = optarg;
      break;

    case 'o':
      *object_class_index_p = atoi(optarg)-1;
      break;

    default:
      print_usage (argv); /* exit */
    }
  }

  
}

static void 
print_usage (char **argv)
{

  /* Exits after printing message */
  dce_fprintf(stdout, cds_s_addcell_usage, argv[0]);
  exit(0);
}


/*
 * report_ds_error()
 *	Extracts the error number from a DS_status return code, prints it
 *	in an error message.
 */
void 
report_ds_error (char       *header,	/* In--Name of function whose return */
		                        /* code is being checked */

                 DS_status  returnCode)	 /* In--Return code to be checked */
{
    OM_type		includeDSProblem[] = { DS_PROBLEM,
					       0 };
    OM_return_code      omStatus;
    OM_public_object	problem;
    OM_value_position	total;


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
      dce_fprintf(stderr, cds_s_addcell_xds_error, header, 
		  (long)problem->value.enumeration);
    }
    else
    {
	dce_fprintf(stderr, cds_s_addcell_xds_unknown, header);
    }

}
void 
report_om_error (char            *header,
                 OM_return_code  returnCode) /* In--Return code to be checked*/
{

    dce_fprintf(stderr, cds_s_addcell_om, header, (long)returnCode);
}
