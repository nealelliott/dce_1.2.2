/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: bind_handle_tsts.c,v $
 * Revision 1.1.4.2  1996/02/17  23:20:02  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:38:39  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:02:57  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:42:46  root]
 * 
 * Revision 1.1.2.2  1993/01/11  22:11:21  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:03:43  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:29  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1989 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**  %a%copyright(,**  )
**
**  NAME
**
**      bind_handle_tsts.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**
**  %a%private_begin
**
**  MODIFICATION HISTORY:
**
**   2-aug-91 martin    put <> around rpc.h.
**  19-mar-91 woodbury  add pathological case handling
**  14-mar-91 woodbury  fix bug in comtest_bind_inq_client
**  13-nov-90 woodbury  add routine/function headers
**  09-Nov-90 burton    add comtest_bind_inq_client
**  25-oct-90 woodbury  made it 'lint'-free
**  18-oct-90 woodbury  include "comtest.h"
**                      Put in calls to PRINT_ARGS to give debug info & 
**                      to silence lint.
**  12-oct-90 burton    original
**
**  %a%private_end  
**
*/


#include    <dce/rpc.h>
#include    <tstdriver.h>
#include    <comtest.h>


extern int verbose;
extern unsigned32 in_status;

int comtest_bind_free ();
int comtest_bind_copy ();
int comtest_bind_vector_free ();
int comtest_bind_reset ();
int comtest_bind_inq_object ();
int comtest_bind_set_object ();
int comtest_bind_inq_client ();


#define     BIND_FREE_ARGS (1)
#define     BIND_COPY_ARGS (1)
#define     BIND_VECTOR_FREE_ARGS (0)
#define     BIND_RESET_ARGS (1)
#define     BIND_INQ_OBJECT_ARGS (1)
#define     BIND_SET_OBJECT_ARGS (2)


/*
**++
**
**  ROUTINE NAME:       comtest_bind_free 
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**
**      Tests rpc_binding_free() by first creating a binding handle from the
**  string binding input, and then freeing the binding. Note: this is the 
**  same as comtest_bind_from_str_bind.
**
**  INPUTS:             
**
**      argc            argument count
**      argv            array of pointers to string input arguments
**
**  INPUTS/OUTPUTS:     none
**
**      
**
**  OUTPUTS:            
**
**      status          an rpc status code
**
**  IMPLICIT INPUTS:    none 
**
**  IMPLICIT OUTPUTS:   none 
**
**  FUNCTION VALUE:     int
**
**      return          an integer specifying success or some type of failure
**                      
**
**  SIDE EFFECTS:       none 
**
**--
**/

comtest_bind_free (argc, argv, status)
int             argc;
char            *argv[];
unsigned32      *status;
{
    rpc_binding_handle_t    binding_h;

    CHECK_ARGS(BIND_FREE_ARGS);

    PRINT_ARGS;

    rpc_binding_from_string_binding ( 
        (unsigned_char_t *)argv[1],
        &binding_h, 
        status);

    CHECK_STATUS;

    rpc_binding_free (&binding_h, status);

    CHECK_STATUS;

    if (verbose != 0)
    {
        fprintf (stdout, 
            "This test is identical to comtest_bind_from_str_bind!\n");
    }
    return(COMTEST_OK);
}


/*
**++
**
**  ROUTINE NAME:       comtest_bind_copy 
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**
**      Test rpc_binding_copy by creating a binding handle using the string
**  binding supplied by the caller. Next call rpc_binding_copy to return a
**  copy of the binding, which is used as input to rpc_binding_to_str_binding.
**  The returned string is compared with the original string to make sure
**  rpc_binding_copy worked correctly.
**
**  INPUTS:             
**
**      argc            argument count
**      argv            array of pointers to string input arguments
**
**  INPUTS/OUTPUTS:     none
**
**
**  OUTPUTS:            
**
**      status          an rpc status code
**
**  IMPLICIT INPUTS:    none 
**
**  IMPLICIT OUTPUTS:   none 
**
**  FUNCTION VALUE:     int
**
**      return          an integer specifying success or some type of failure
**                      
**
**  SIDE EFFECTS:       none 
**
**--
**/

comtest_bind_copy (argc, argv, status)
int             argc;
char            *argv[];
unsigned32      *status;
{
    rpc_binding_handle_t    binding_h, dest_bind_h;
    int                     retval;
    unsigned_char_t         *string_binding;

    CHECK_ARGS(BIND_COPY_ARGS);

    PRINT_ARGS;

    rpc_binding_from_string_binding ( 
        (unsigned_char_t *)argv[1],
        &binding_h, 
        status);

    CHECK_STATUS;

    dest_bind_h = (rpc_binding_handle_t) NULL;

    rpc_binding_copy (binding_h, &dest_bind_h, status);

    CHECK_STATUS;

    rpc_binding_to_string_binding (
        dest_bind_h,
        &string_binding,
        status);

    CHECK_STATUS;

    if (strcmp ( argv[1], (char *)string_binding ) == 0 )
    {
         retval = COMTEST_OK;
    }
    else
    {
        retval = COMTEST_FAILED;
    }

    FREE_STRING (&string_binding);

    rpc_binding_free (&binding_h, status);

    CHECK_STATUS;

    rpc_binding_free (&dest_bind_h, status);

    CHECK_STATUS;

    return(retval);
}


/*
**++
**
**  ROUTINE NAME:       comtest_bind_vector_free 
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**
**      We test rpc_binding_vector_free here by calling
**  rpc_server_use_all_protseqs and then rpc_server_inq_bindings to return a
**  valid binding vector. Then we call rpc_binding_vector_free to free the 
**  memorry allocated for the vector.
**
**  INPUTS:             
**
**      argc            argument count
**      argv            array of pointers to string input arguments
**
**  INPUTS/OUTPUTS:     none
**
**
**  OUTPUTS:            
**
**      status          an rpc status code
**
**  IMPLICIT INPUTS:    none 
**
**  IMPLICIT OUTPUTS:   none 
**
**  FUNCTION VALUE:     int
**
**      return          an integer specifying success or some type of failure
**                      
**
**  SIDE EFFECTS:       none 
**
**--
**/

comtest_bind_vector_free (argc, argv, status)
int             argc;
char            *argv[];
unsigned32      *status;
{
    rpc_binding_vector_t    *binding_vec_ptr;

    CHECK_ARGS(BIND_VECTOR_FREE_ARGS);

    PRINT_ARGS;

    rpc_server_use_all_protseqs (rpc_c_protseq_max_calls_default, status);

    CHECK_STATUS;

    rpc_server_inq_bindings (&binding_vec_ptr, status);

    CHECK_STATUS;

    rpc_binding_vector_free (&binding_vec_ptr, status);

    CHECK_STATUS;

    return(COMTEST_OK);
}


/*
**++
**
**  ROUTINE NAME:       comtest_bind_reset 
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**
**      We obtain a binding handle using the caller-supplied string binding as
**  input. Next we call rpc_binding_reset to modify the returned binding. We 
**  use rpc_binding_to_string_binding and rpc_string_binding_parse to make
**  sure the endpoint has been removed from the binding. The required cleanup 
**  routines are called at the end.
**
**  INPUTS:             
**
**      argc            argument count
**      argv            array of pointers to string input arguments
**
**  INPUTS/OUTPUTS:     none
**
**
**  OUTPUTS:            
**
**      status          an rpc status code
**
**  IMPLICIT INPUTS:    none 
**
**  IMPLICIT OUTPUTS:   none 
**
**  FUNCTION VALUE:     int
**
**      return          an integer specifying success or some type of failure
**                      
**
**  SIDE EFFECTS:       none 
**
**--
**/

comtest_bind_reset (argc, argv, status)
int             argc;
char            *argv[];
unsigned32      *status;
{
    rpc_binding_handle_t    binding_h;
    int                     retval;
    unsigned_char_t         *string_binding;
    /*
     * pointers for strings returned from string_binding_parse:
     */
    unsigned_char_t *ret_obj_uuid = NULL;
    unsigned_char_t *ret_protseq = NULL;
    unsigned_char_t *ret_network_addr = NULL;
    unsigned_char_t *ret_endpoint = NULL;
    unsigned_char_t *ret_options = NULL;

    CHECK_ARGS(BIND_RESET_ARGS);

    PRINT_ARGS;

    rpc_binding_from_string_binding ( 
        (unsigned_char_t *)argv[1],
        &binding_h, 
        status);

    CHECK_STATUS;

    rpc_binding_reset (binding_h, status);

    CHECK_STATUS;

    rpc_binding_to_string_binding (
        binding_h,
        &string_binding,
        status);

    CHECK_STATUS;

    /*
     * We need to call string_binding_parse to see if the endpoint has been
     * properly reset, not just a simple strcmp.
     */
    if (strcmp ( argv[1], (char *)string_binding ) != 0 )
    {
        if (verbose != 0)
        {
            fprintf (stdout, "original binding was %s\n",
                argv[1] );
            fprintf (stdout, "   reset binding is  %s\n",
                string_binding);
        }
        retval = COMTEST_OK;
    }
    else
    {
        retval = COMTEST_FAILED;
    }

    rpc_string_binding_parse (string_binding, &ret_obj_uuid, &ret_protseq,
        &ret_network_addr, &ret_endpoint, &ret_options, status);

    if ((strcmp ("", (char *)ret_endpoint)) != 0) 
    {
        if (verbose != 0)
        {
            fprintf (stdout, "%s \"%s\"\n", 
                "endpoint =", (char *)ret_endpoint); 
        }
        retval = COMTEST_FAILED;
    }
    if ((strcmp ("", (char *)ret_options)) != 0) 
    {
        if (verbose != 0)
        {
            fprintf (stdout, "%s \"%s\"\n", 
                "network options =", (char *)ret_options);
        }
        retval = COMTEST_FAILED;
    }

    FREE_STRING(&ret_obj_uuid);
    FREE_STRING(&ret_protseq);
    FREE_STRING(&ret_network_addr);
    FREE_STRING(&ret_endpoint);
    FREE_STRING(&ret_options);
    FREE_STRING(&string_binding);

    rpc_binding_free (&binding_h, status);

    CHECK_STATUS;

    return(retval);
}


/*
**++
**
**  ROUTINE NAME:       comtest_bind_inq_object 
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**
**      The caller specifies a string binding which contains an object uuid 
**  for this test. The string binding is used in calling
**  rpc_binding_from_string_binding, which returns a binding handle. Then
**  rpc_binding_inq_object is called to return the object uuid in binary form.
**  This is converted to string form by uuid_to_string, and that string is
**  compared to the uuid string returned by a call to
**  rpc_string_binding_parse. The strings should be equal for the test to
**  pass.
**
**  INPUTS:             
**
**      argc            argument count
**      argv            array of pointers to string input arguments
**
**  INPUTS/OUTPUTS:     none
**
**      
**
**  OUTPUTS:            
**
**      status          an rpc status code
**
**  IMPLICIT INPUTS:    none 
**
**  IMPLICIT OUTPUTS:   none 
**
**  FUNCTION VALUE:     int
**
**      return          an integer specifying success or some type of failure
**                      
**
**  SIDE EFFECTS:       none 
**
**--
**/

comtest_bind_inq_object (argc, argv, status)
int             argc;
char            *argv[];
unsigned32      *status;
{
    int                     retval;
    rpc_binding_handle_t    binding_h;
    uuid_t                  object;
    unsigned_char_t         *object_string;
    /*
     * pointers for strings returned from string_binding_parse:
     */
    unsigned_char_t *ret_obj_uuid = NULL;
    unsigned_char_t *ret_protseq = NULL;
    unsigned_char_t *ret_network_addr = NULL;
    unsigned_char_t *ret_endpoint = NULL;
    unsigned_char_t *ret_options = NULL;

    CHECK_ARGS(BIND_INQ_OBJECT_ARGS);

    PRINT_ARGS;

    rpc_binding_from_string_binding ( 
        (unsigned_char_t *)argv[1],
        &binding_h, 
        status);

    CHECK_STATUS;

    rpc_binding_inq_object (binding_h, &object, status);

    CHECK_STATUS;

    rpc_string_binding_parse ((unsigned_char_t *)argv[1], &ret_obj_uuid, 
        &ret_protseq, &ret_network_addr, &ret_endpoint, &ret_options, status);

    CHECK_STATUS;

    uuid_to_string (&object, &object_string, status);

    CHECK_STATUS;

    if ((strcmp ((char *)ret_obj_uuid, (char *)object_string)) == 0)
    {
        retval = COMTEST_OK;
    }
    else
    {
        retval = COMTEST_FAILED;
    }

    FREE_STRING(&ret_obj_uuid);
    FREE_STRING(&ret_protseq);
    FREE_STRING(&ret_network_addr);
    FREE_STRING(&ret_endpoint);
    FREE_STRING(&ret_options);
    FREE_STRING(&object_string);

    return (retval);
}


/*
**++
**
**  ROUTINE NAME:       comtest_bind_set_object 
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**
**      The caller inputs a string binding, which is used to obtain a binding
**  handle. We call uuid_create to obtain a UUID which we will use as the
**  object UUID input for rpc_binding_set_object. To check that the call 
**  worked, we call rpc_binding_inq_object and compare the returned UUID by
**  calling uuid_equal.
**
**  INPUTS:             
**
**      argc            argument count
**      argv            array of pointers to string input arguments
**
**  INPUTS/OUTPUTS:     none
**
**
**  OUTPUTS:            
**
**      status          an rpc status code
**
**  IMPLICIT INPUTS:    none 
**
**  IMPLICIT OUTPUTS:   none 
**
**  FUNCTION VALUE:     int
**
**      return          an integer specifying success or some type of failure
**                      
**
**  SIDE EFFECTS:       none 
**
**--
**/

comtest_bind_set_object (argc, argv, status)
int             argc;
char            *argv[];
unsigned32      *status;
{
    int                     retval;
    rpc_binding_handle_t    binding_h;
    uuid_t                  object, ret_object;
    unsigned_char_t         *object_string;
    /*
     * pointers for strings returned from string_binding_parse:
     */
    unsigned_char_t *ret_obj_uuid = NULL;
    unsigned_char_t *ret_protseq = NULL;
    unsigned_char_t *ret_network_addr = NULL;
    unsigned_char_t *ret_endpoint = NULL;
    unsigned_char_t *ret_options = NULL;

    CHECK_ARGS(BIND_SET_OBJECT_ARGS);

    PRINT_ARGS;

    retval = RPC_CALL_FAILED;

    rpc_binding_from_string_binding ( 
        (unsigned_char_t *)argv[1],
        &binding_h, 
        status);

    CHECK_STATUS;

    uuid_create (&object, status);

    CHECK_STATUS;

#ifdef  DEBUG
    rpc_string_binding_parse ((unsigned_char_t *)argv[1], &ret_obj_uuid, 
        &ret_protseq, &ret_network_addr, &ret_endpoint, &ret_options, status);

    if (*status != rpc_s_ok) goto CLEANUP;

    uuid_to_string (&object, &object_string, status);

    if (*status != rpc_s_ok) goto CLEANUP;

    if (verbose != 0)
    {
        if ((strcmp ((char *)ret_obj_uuid, "")) != 0)
        {
            fprintf (stdout, "replacing object uuid %s with %s.\n", 
                ret_obj_uuid, object_string);
        }
        else
        {
            fprintf (stdout, "replacing NIL object uuid with %s.\n",
                object_string);
        }
    }
#endif

    rpc_binding_set_object (binding_h, &object, status);

    if (*status != rpc_s_ok) goto CLEANUP;

    uuid_create_nil (&ret_object, status);

    if (*status != rpc_s_ok) goto CLEANUP;

    rpc_binding_inq_object (binding_h, &ret_object, status);

    if (*status != rpc_s_ok) goto CLEANUP;

    if (!uuid_equal (&object, &ret_object, status))
    {
        retval = COMTEST_FAILED;
    }
    else if (*status != rpc_s_ok)
    {
        retval = RPC_CALL_FAILED;
    }
    else
    {
        retval = COMTEST_OK;
    }

CLEANUP:

#ifdef  DEBUG
    FREE_STRING(&ret_obj_uuid);
    FREE_STRING(&ret_protseq);
    FREE_STRING(&ret_network_addr);
    FREE_STRING(&ret_endpoint);
    FREE_STRING(&ret_options);
    FREE_STRING(&object_string);
#endif

    rpc_binding_free (&binding_h, status);

    CHECK_STATUS;

    return (retval);
}



/*
**++
**
**  ROUTINE NAME:       comtest_bind_inq_client
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**
**  Protocol sequence vector test.  RPC routines exercised:
**
**        rpc_binding_inq_client
**
**   Test the ability of rpc_binding_inq_client to 
**
**  INPUTS:             
**
**
**      argc            count of arg pointers passwd by argv
**
**      argv            standard 'argv' array of pointers to null
**                      terminated ascii argument strings
**                      see comment header on routine 
**                      comtest_if_assemb_args for order in which
**                      to specify input args
**
**  INPUTS/OUTPUTS:     none
**
**      
**
**  OUTPUTS:            an rpc status code
**
**      status
**
**  IMPLICIT INPUTS:    none 
**
**  IMPLICIT OUTPUTS:   none 
**
**  FUNCTION VALUE:     int
**
**      return          an integer specifying success or some type of failure
**                      
**                      
**
**  SIDE EFFECTS:       none 
**
**--
**/


comtest_bind_inq_client (argc, argv, status)
int argc;
char *argv[];
unsigned32      *status;
{
    rpc_binding_handle_t    binding_h;
    rpc_client_handle_t client_handle;


    PRINT_ARGS;


    rpc_binding_from_string_binding ( 
        (unsigned_char_t *)argv[1],
        &binding_h, 
        status);

    CHECK_STATUS;
    rpc_binding_inq_client((rpc_binding_handle_t) binding_h,
                                   &client_handle, status);
    if (*status != in_status)
        return COMTEST_FAILED;
    else
        return COMTEST_OK;
}

