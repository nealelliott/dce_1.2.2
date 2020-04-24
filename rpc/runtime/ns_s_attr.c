/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: ns_s_attr.c,v $
 * Revision 1.1.5.2  1996/02/18  00:04:54  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:56:59  marty]
 *
 * Revision 1.1.5.1  1995/12/08  00:21:16  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/16  21:34 UTC  jrr
 * 	Remove Mothra specific code
 * 
 * 	HP revision /main/HPDCE02/2  1995/05/25  21:41 UTC  lmm
 * 	Merge allocation failure detection cleanup work
 * 
 * 	HP revision /main/HPDCE02/lmm_alloc_fail_clnup/1  1995/05/25  21:02 UTC  lmm
 * 	allocation failure detection cleanup
 * 
 * 	HP revision /main/HPDCE02/1  1995/04/02  01:16 UTC  lmm
 * 	Merge allocation failure detection functionality into Mothra
 * 
 * 	HP revision /main/lmm_rpc_alloc_fail_detect/1  1995/04/02  00:20 UTC  lmm
 * 	add memory allocation failure detection
 * 	[1995/12/07  23:59:56  root]
 * 
 * Revision 1.1.3.5  1994/07/29  16:19:14  tom
 * 	Add entry type to rpc__nsentry_create arguments. (OT 7945)
 * 	[1994/07/28  22:36:16  tom]
 * 
 * Revision 1.1.3.4  1994/07/28  18:30:29  mori_m
 * 	CR 11132  Changed long to idl_long_int
 * 	CR 11294  Changed attr_type to a uuid_t* for codesets attribute ops.
 * 	[1994/07/28  18:24:22  mori_m]
 * 
 * Revision 1.1.3.3  1994/06/10  20:54:48  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  15:00:14  devsrc]
 * 
 * Revision 1.1.3.2  1994/04/12  16:52:04  mori_m
 * 	CR 9701:  Second submission for RPC runtime I18N support.
 * 	[1994/04/12  16:46:09  mori_m]
 * 
 * Revision 1.1.3.1  1994/02/08  21:37:44  mori_m
 * 	CR 9701:  Initial submission for RPC runtime I18N support
 * 	[1994/02/08  21:37:34  mori_m]
 * 
 * $EndLog$
 */
/*
**  NAME
**
**     ns_mgmt_set_attribute.c
**
**  FACILITY:
**
**     Remote Procedure Call (RPC)
**
**  ABSTRACT:
**
**
*/
#include <dce/rpcbase.h>
#include <dce/rpcsts.h>
#include <dce/nbase.h>
#include <dce/uuid.h>

#include <commonp.h>
#include <ns.h>         	/* Externals for Name Services component   */
#include <com.h>         	/* Externals for Common Services component */
#include <rpcmem.h>

#include <nsp.h>
#include <nsattr.h>		/* Private routines for NS attributes      */
#include <nsentry.h>		/* Externals for NSL Entry sub-component   */

#include <codesets.h>		/* Generated from codesets.idl */
#include <cs_s.h>		/* Private defs for code set interoperability */

/*
 * Well-known UUID for code set attribute.
 * This must exactly match the corresponding #define's in cs_s.h
 */
uuid_t	rpc_c_attr_real_codesets  = { /* a1794860-a955-11cd-8443-08000925d3fe */
	0xa1794860, 0xa955, 0x11cd, 0x84, 0x43,
	{0x08, 0x00, 0x09, 0x25, 0xd3, 0xfe}
};

uuid_t	*rpc_c_attr_codesets = &rpc_c_attr_real_codesets;


/*
**++
**  ROUTINE NAME:       rpc_ns_mgmt_set_attribute
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**
**  This routine receives a value, and encodes it with IDL encoding services.
**  Then the encoded value is stored in CDS as an attribute.
**
**  INPUTS:
**
**      entry_name_syntax   Name syntax of the entry name.
**
**      entry_name          String representation of the CDS entry name.
**
**      attr_type           CDS attribute type.  It is defined in cs_s.h
**
**      attr_val            user supplied value.
**
**  OUTPUTS:
**
**      status          The return status value in *status indicates
**                      whether the routine completed successfully and, if
**                      not, why. Possible return values:
**                          rpc_s_ok
**                          rpc_s_mgmt_bad_type
**                          rpc_s_no_memory
**                      or status from a subsequent called routine.
**
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       Allocates memory for the encoded value from IDL encoding
**			services.
**
**--
*/

PUBLIC void rpc_ns_mgmt_set_attribute
#ifdef _DCE_PROTO_
(
	unsigned32		entry_name_syntax,
	unsigned_char_p_t	entry_name,
	uuid_t			*attr_type,
	void			*attr_val,
	error_status_t		*status
)
#else
(entry_name_syntax, entry_name, attr_type, attr_val, status)
	unsigned32		entry_name_syntax;
	unsigned_char_p_t	entry_name;
	uuid_t			*attr_type;
	void			*attr_val;
	error_status_t		*status;
#endif
{
	unsigned32		name_syntax;
	rpc_nsentry_p_t		nsentry;
	unsigned32		temp_status;
	unsigned32		encoding_size;
	rpc_codeset_mgmt_t	*codeset_val;
	idl_es_handle_t		es_h;
	idl_byte		*nsi_buf_start; 
	idl_ulong_int		nsi_encoding_size;
	unsigned32		int_attr_type;	/* internal attr_type form */


	CODING_ERROR (status);
	RPC_NS_VERIFY_INIT();

	/*
	 * Cast attribute to the appropriate type
	 */
	if (uuid_equal(attr_type, &rpc_c_attr_real_codesets, status))
	{
		codeset_val = (rpc_codeset_mgmt_t *)attr_val;
		int_attr_type = RPC_C_ATTR_CODESETS;
	}
	else
	{
		*status = rpc_s_mgmt_bad_type;
		return;
	}


	/* 
	 * Encode attribute with IDL Dynamic buffer encoding
	 */

	switch ((int)int_attr_type) 
	{

	case RPC_C_ATTR_CODESETS:

		/*
	 	 * Set IDL encoding services to dynamic encoding.
	 	 */
		idl_es_encode_dyn_buffer(&nsi_buf_start, 
					&nsi_encoding_size, 
					&es_h, status);

		if (*status != error_status_ok)
		{
			return;
		}

		/*
	 	 *  Encode code sets.
		 *
		 *  Size = (number of array elements + version + size)
		 *	Both version & size of array elements will be encoded 
		 *	along with the array elements (code sets)
	 	 */
		encoding_size = sizeof(codeset_val->version) +
				sizeof(codeset_val->count)   +
				sizeof(codeset_val->codesets[0]) * codeset_val->count;
		rpc__codesets_to_nscodesets(es_h, encoding_size, (idl_byte *)codeset_val);

		idl_es_handle_free(&es_h, status);

		if (*status != error_status_ok) 
		{
			return;
		}

		break;

	default:

		*status = rpc_s_mgmt_bad_type;
		return;

	}

	name_syntax = entry_name_syntax;
	RPC_RESOLVE_SYNTAX(name_syntax, status);
	if (*status != rpc_s_ok)
	{
		return;
	}

	nsentry = rpc__nsentry_from_entry (name_syntax, entry_name, status);
	if (*status != rpc_s_ok)
	{ 
		return; 
	}

	rpc__nsentry_create(nsentry, RPC_C_NS_CODESETS, status);
	if ((*status != rpc_s_ok) &&
		(*status != rpc_s_entry_already_exists))
	{
		rpc__nsentry_free(&nsentry, &temp_status);
		return;
	}

	/* Check the class version of the name service entry */
	rpc__ns_check_rpc_class_version (nsentry, RPC_C_NS_BYPASS_CACHE, status);
	if ((*status == rpc_s_ok) || (*status == rpc_s_not_rpc_entry))
	{
		switch ((int)int_attr_type) 
		{

		case RPC_C_ATTR_CODESETS:

			rpc__nsattr_add_value(
				nsentry,
				RPC_C_ATTR_CODESETS,
				(unsigned_char_p_t)nsi_buf_start,
				nsi_encoding_size,
				&temp_status );

			if (temp_status != rpc_s_ok)
			{
				*status = temp_status;

				rpc__nsentry_free(&nsentry, &temp_status);
				return;
			}

			break;

		default:	/* Not reachable */

			*status = rpc_s_mgmt_bad_type;
			rpc__nsentry_free(&nsentry, &temp_status);
			return;

		}

	}
	else
	{
		temp_status = rpc_s_ok;
	}

	if ((temp_status == rpc_s_ok) && (*status == rpc_s_not_rpc_entry))
	{
		rpc__nsentry_make_it_rpc(nsentry, &temp_status);
		if (temp_status != rpc_s_ok) 
		{
			*status = temp_status;
			rpc__nsentry_free(&nsentry, &temp_status);
			return;
		}

	}

	rpc__nsentry_free(&nsentry, status);
	return;

}

/*
**++
**  ROUTINE NAME:       rpc_ns_mgmt_read_attr_begin 
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:        
**
**  This routine creates an inquiry context for an object in
**  a name service entry. This routine is called prior to calling the
**  rpc_ns_mgmt_read_attr_next routine.
**
**  INPUTS:             
**
**      entry_name_syntax   Name syntax of the entry name.
**
**      entry_name          String representation of the entry name.
**
**      attr_type           CDS attribute type.  It is defined in cs_s.h.
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:            
**
**      inquiry_context An inquiry context specifying a code set attribute.
**
**      status          The return status value in *status indicates
**                      whether the routine completed successfully and, if
**                      not, why. Possible return values:
**                          rpc_s_ok
**                          rpc_s_mgmt_bad_type;
**                      or status from a subsequent called routine.
**                          
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       Allocates memory for the inquiry context structure.
**
**--
*/

PUBLIC 
void rpc_ns_mgmt_read_attr_begin
#ifdef _DCE_PROTO_
(
	unsigned32		entry_name_syntax, 
	unsigned_char_p_t	entry_name, 
	uuid_t			*attr_type, 
	rpc_ns_handle_t		*inquiry_context, 
	error_status_t		*status
)
#else
(entry_name_syntax, entry_name, attr_type, inquiry_context, status)
	unsigned32		entry_name_syntax;
	unsigned_char_p_t	entry_name;
	uuid_t			*attr_type;
	rpc_ns_handle_t		*inquiry_context;
	error_status_t		*status;
#endif
{

    unsigned32              new_entry_name_syntax;
    rpc_nsentry_p_t         nsentry;
    rpc_ns_inq_rep_p_t      inq = NULL;


    CODING_ERROR (status);
    RPC_NS_VERIFY_INIT();

    /*
     * Copy the given entry name and syntax into local variables, because
     * we might have to modify them.  Resolve the given name and syntax
     * using defaults if required and convert to internal form.
     */
    new_entry_name_syntax = entry_name_syntax;
    RPC_RESOLVE_SYNTAX (new_entry_name_syntax, status);
    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     *  Convert entry_name to name service representation.
     */
    nsentry = rpc__nsentry_from_entry (
                new_entry_name_syntax, entry_name, status);

    if (*status != rpc_s_ok)
    {
        return;
    }

    /*
     *  Create an inquiry context.
     */
    if (uuid_equal(attr_type, &rpc_c_attr_real_codesets, status))
    {
    	inq = (rpc_ns_inq_rep_p_t)rpc__inqcntx_create (nsentry, rpc_e_codesets_member, status);
    }
    else
    {
	*status = rpc_s_mgmt_bad_type;
	return;
    }

    *inquiry_context = (rpc_ns_handle_t) inq;
    return;
}

/*
**++
**  ROUTINE NAME:       rpc_ns_mgmt_read_attr_next 
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:        
**
**  This routine returns value from an attribute in a name service entry.
**
**  Currently, 'codesets' attribute does not have a recursive attribute
**  chain to follow, so only one 'rpc__nsattr_read_value()' call is sufficient.
**  However, this function name contains '_next' for the future expansion
**  possibility.  Current code does not have recursive attribute search
**  logic, so it needs to be added when the logic becomes necessary.
**
**  INPUTS:             
**
**      inquiry_context An opaque pointer to an inquiry context data rep.
**
**      attr_type       CDS attribute type.  It is defined in cs_s.h
**
**  INPUT/OUTPUTS:      none
**
**  OUTPUTS:            
**
**      value           The value for the attribute in the
**                      namespace entry designated by the inquiry_context.
**
**	length		The length of the attribute retrieved from the
**			namespace.
**
**      status          The return status value in *status indicates
**                      whether the routine completed successfully and, if
**                      not, why. Possible return values:
**                          rpc_s_ok
**                          rpc_s_invalid_ns_handle
**                          rpc_s_mgmt_bad_type
**                      or status from a subsequent called routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       Output 'value' will be allocated within the module.
**			This memory will be freed by rpc_ns_mgmt_free_codesets()
**			call.
**
**--
*/

PUBLIC void rpc_ns_mgmt_read_attr_next
#ifdef _DCE_PROTO_
(
	rpc_ns_handle_t	inquiry_context, 
	uuid_t		*attr_type, 	
	idl_byte	**value,
	unsigned32	*length, 
	error_status_t	*status
)
#else
(inquiry_context, attr_type, value, length, status)
	rpc_ns_handle_t	inquiry_context;
	uuid_t		*attr_type;
	idl_byte	**value;
	unsigned32	*length;
	error_status_t	*status;
#endif
{
	rpc_ns_inq_rep_p_t      inq;
	idl_es_handle_t		es_h;
	idl_byte		*nsi_out_buf;
	idl_ulong_int		nsi_out_length;
	unsigned32		int_attr_type;	/* internal attr_type form */


	CODING_ERROR (status);
	RPC_NS_VERIFY_INIT();

	/*
	 * Cast attribute to the appropriate type
	 */
	if (uuid_equal(attr_type, &rpc_c_attr_real_codesets, status))
	{
		int_attr_type = RPC_C_ATTR_CODESETS;
	}
	else
	{
		*status = rpc_s_mgmt_bad_type;
		return;
	}

	*length = 0;	/* Output initialization */

	/* 
	 * check against an invalid inquiry context
	 */
	inq = (rpc_ns_inq_rep_p_t) inquiry_context;
	if (inq->usage != rpc_e_codesets_member)
	{
		*status = rpc_s_invalid_ns_handle;
		return;
	}

	/*
	 * Read the value of NS attribute
	 */
	rpc__nsattr_read_value (
			inq, 
			(unsigned_char_p_t *)&nsi_out_buf, 
			&nsi_out_length, 
			status );

	if (*status == rpc_s_ok)
	{
		switch ((int)int_attr_type) 
		{

		case RPC_C_ATTR_CODESETS:
			/*
			 *  Decode code sets.
			 */
	 		idl_es_decode_buffer(nsi_out_buf, 
					nsi_out_length, &es_h, status);
	
			if (*status != error_status_ok)
			{
				return;
			}

			/* Allocate an output buffer for decoding */

			RPC_MEM_ALLOC (
				*value,
				unsigned_char_p_t,
				nsi_out_length,
				RPC_C_MEM_CDS_ATTR,
				RPC_C_MEM_WAITOK);
			if (*value == NULL){
			    error_status_t     lst;
			    
			    idl_es_handle_free(&es_h, &lst);
			    *status = rpc_s_no_memory;
			    return;
			}

			rpc__nscodesets_to_codesets(es_h, (idl_long_int *)length, (idl_byte *)(*value));

			idl_es_handle_free(&es_h, status);
	
			if (*status != error_status_ok)
			{
				RPC_MEM_FREE(value, RPC_C_MEM_CDS_ATTR);
				return;
			}

			break;

		default:
			*status = rpc_s_mgmt_bad_type;
			break;
		}
	}

	return;
}

/*
**++
**  ROUTINE NAME:       rpc_ns_mgmt_read_attr_done 
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:        
**
**  This routine frees an inquiry context which was created by calling the 
**  rpc_ns_mgmt_read_attr_begin routine.
**
**  INPUTS:             none
**
**  INPUT/OUTPUTS:          
**
**      inquiry_context On call, it is an opaque pointer to an inquiry
**                      context specifying code sets.
**                      Returns a NULL pointer on successful return.
**
**  OUTPUTS:            
**
**      status          The return status value in *status indicates
**                      whether the routine completed successfully and, if
**                      not, why. Possible return values:
**                          rpc_s_ok
**                          rpc_s_invalid_ns_handle
**                      return value from rpc__inqcntx_free 
**                          
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

PUBLIC void rpc_ns_mgmt_read_attr_done
#ifdef _DCE_PROTO_
(
	rpc_ns_handle_p_t	inquiry_context, 
	error_status_t		*status
)
#else
(inquiry_context, status)
	rpc_ns_handle_p_t	inquiry_context;
	error_status_t		*status;
#endif
{
	rpc_ns_inq_rep_p_t      inq;


	CODING_ERROR (status);
	RPC_NS_VERIFY_INIT();

	/*
	 * Check against an invalid inquiry context
	 */
	inq = (rpc_ns_inq_rep_p_t) *inquiry_context;
	if ((inq == (rpc_ns_inq_rep_p_t) NULL) ||
		(inq->usage != rpc_e_codesets_member))
	{
		*status = rpc_s_invalid_ns_handle;
		return;
	}

	rpc__inqcntx_free (&inq, status);
	*inquiry_context = (rpc_ns_handle_t) inq;

	*status = rpc_s_ok;

	return;
}

/*
**++
**  ROUTINE NAME:       rpc_ns_mgmt_remove_attribute
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:        
**
**  Delete a specified attribute.
**
**  INPUTS:             
**
**      name_syntax	The name syntax of the attribute name
**
**      entry_name	String representation of the CDS entry name.
**
**      attr_type       CDS attribute type.  It is defined in cs_s.h
**
**  INPUT/OUPUTS:       none
**
**  OUTPUTS:            
**
**      status          The status of the group delete operation.
**                      Can be one of:
**                          rpc_s_ok
**                      or a status returned from a called routine.
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

PUBLIC void rpc_ns_mgmt_remove_attribute
#ifdef _DCE_PROTO_
(
	unsigned32		name_syntax, 
	unsigned_char_p_t	entry_name, 
	uuid_t			*attr_type, 
	error_status_t		*status
)
#else
(name_syntax, entry_name, attr_type, status)
	unsigned32		name_syntax;
	unsigned_char_p_t	entry_name;
	uuid_t			*attr_type;
	error_status_t		*status;
#endif
{
	rpc_nsentry_p_t         nsattr_name;
	unsigned32              temp_status;
	unsigned32              new_name_syntax;
	unsigned32              int_attr_type;


	CODING_ERROR (status);
	RPC_NS_VERIFY_INIT();

	/*
	 * Cast attribute to the appropriate type
	 */
	if (uuid_equal(attr_type, &rpc_c_attr_real_codesets, status))
	{
		int_attr_type = RPC_C_ATTR_CODESETS;
	}
	else
	{
		*status = rpc_s_mgmt_bad_type;
		return;
	}

	/*
	 * Copy the given attribute syntax into a local variable, because
	 * we might have to modify it.  Resolve the given syntax
	 * using defaults if required and convert to internal form.
	 */
	new_name_syntax = name_syntax;
	RPC_RESOLVE_SYNTAX (new_name_syntax, status);
	if (*status != rpc_s_ok)
	{
        	return;
	}
				   
	/*
	 * Create a name-service representation of attr_name.
	 */
	nsattr_name = rpc__nsentry_from_entry(new_name_syntax, entry_name, status);

	if (*status != rpc_s_ok)
	{
		return;
	}

	/*
	 * Remove the given attribute if it exists.
	 */
	rpc__nsattr_delete (nsattr_name, int_attr_type, status);

	/*
	 * Free the name-service representation of the attribute name.
	 */
	rpc__nsentry_free (&nsattr_name, &temp_status);

	if (*status == rpc_s_ok)
	{
		*status = temp_status;
	}

	return;
}

/*
**++
**  ROUTINE NAME:       rpc_ns_mgmt_read_codesets
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**
**  This routine reads the code sets value from CDS.
**
**  INPUTS:
**
**      entry_name_syntax   Name syntax of the entry name.
**
**      entry_name          String representation of the CDS entry name.
**
**  OUTPUTS:
**
**      codeset_val         Code sets value
**
**      status          The return status value in *status indicates
**                      whether the routine completed successfully and, if
**                      not, why. Possible return values:
**                          rpc_s_ok
**                      or status from a subsequent called routine.
**
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

PUBLIC void rpc_ns_mgmt_read_codesets
#ifdef _DCE_PROTO_
(
	unsigned32		entry_name_syntax,
	unsigned_char_p_t	entry_name,
	rpc_codeset_mgmt_p_t	*codesets_val,
	error_status_t		*status
)
#else
(entry_name_syntax, entry_name, codesets_val, status)
	unsigned32		entry_name_syntax;
	unsigned_char_p_t	entry_name;
	rpc_codeset_mgmt_p_t	*codesets_val;
	error_status_t		*status;
#endif
{
	rpc_ns_handle_t inq_contxt;
	unsigned32	temp_size;

	CODING_ERROR (status);
	RPC_NS_VERIFY_INIT();

	rpc_ns_mgmt_read_attr_begin (
		entry_name_syntax,
		entry_name,
		rpc_c_attr_codesets,
		&inq_contxt,
		status );

	if (*status != rpc_s_ok)
	{
		return;
	}

        rpc_ns_mgmt_read_attr_next (
                inq_contxt,
                rpc_c_attr_codesets,
                (idl_byte **)codesets_val,
                &temp_size,
                status );

	if (*status != rpc_s_ok)
	{
		return;
	}

	rpc_ns_mgmt_read_attr_done (
		&inq_contxt,
		status );

	return;
}

/*
**++
**  ROUTINE NAME:       rpc_ns_mgmt_free_attr_data
**
**  SCOPE:              PUBLIC - declared in rpc.idl
**
**  DESCRIPTION:
**
**  This routine frees an attributes data allocated by the runtime.
**
**  INPUTS/OUTPUT:
**
**      idl_byte   	Address of the pointer to the data to free.
**			
**			The value NULL is returned.
**
**  OUTPUTS:
**
**      status          The return status value in *status indicates
**                      whether the routine completed successfully and, if
**                      not, why. Possible return values:
**                          rpc_s_ok
**
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     void
**
**  SIDE EFFECTS:       none
**
**--
*/

PUBLIC void rpc_ns_mgmt_free_attr_data
#ifdef _DCE_PROTO_
(
	idl_byte		**value,
	error_status_t		*status
)
#else
(value, status)
	idl_byte		**value;
	error_status_t		*status;
#endif
{
	CODING_ERROR (status);

	RPC_MEM_FREE (*value, RPC_C_MEM_CDS_ATTR);
	*value = NULL;

	*status = rpc_s_ok;
	return;
}

PUBLIC void rpc_ns_mgmt_free_codesets
#ifdef _DCE_PROTO_
(
	rpc_codeset_mgmt_t	**codesets,
	error_status_t		*status
)
#else
(codesets, status)
	rpc_codeset_mgmt_t	**codesets;
	error_status_t		*status;
#endif
{
	CODING_ERROR (status);

	RPC_MEM_FREE (*codesets, RPC_C_MEM_CDS_ATTR);
	*codesets = NULL;

	*status = rpc_s_ok;
	return;
}

