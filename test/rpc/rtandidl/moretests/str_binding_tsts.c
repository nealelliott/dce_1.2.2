/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: str_binding_tsts.c,v $
 * Revision 1.1.4.2  1996/02/17  23:22:20  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:14  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:10:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:45:15  root]
 * 
 * Revision 1.1.2.2  1993/01/11  23:40:55  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:14:24  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:52  devrcs
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
**      str_binding_tsts.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**  Definitions of types/constants internal to RPC facility and common
**  to all RPC components.
**
**  %a%private_begin
**
**  MODIFICATION HISTORY:
**
**   2-aug-91 martin    put <> around rpc.h.
**  13-nov-90 woodbury  added standard routine/function headers 
**  30-oct-90 woodbury  put in more checks for argc
**  25-oct-90 woodbury  made 'lint'-free
**  24-oct-90 woodbury  added all the rest of the tests
**  24-oct-90 woodbury  added comtest_bind_from_str_bind
**  15-out-90 burton    original
**
**  %a%private_end  
**
*/

#include    <string.h>
#include    <dce/rpc.h>
#include    <tstdriver.h>
#include    <comtest.h>

#define     FROM_STR_BIND_ARGS      1   /* one string binding */
#define     TO_STR_BIND_ARGS        1   /* one string binding */
#define     STR_BINDING_COMP_ARGS   5   /* 5 actual args are in string form */
#define     STR_BINDING_PARSE_ARGS  5   /* 5 args need to be converted */

extern int verbose;
extern unsigned32 in_status;

/*
 * These forward function declarations could be replaced with prototypes 
 * at some point.
 */

int comtest_bind_from_str_bind ();
int comtest_bind_to_str_bind ();
int comtest_bind_str_binding_comp ();
int comtest_bind_str_binding_parse ();

/*
 * internal forward declarations
 */

static unsigned_char_t *compose_string ();


/*
**++
**
**  ROUTINE NAME:       comtest_bind_from_str_bind
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**      
**      This routine is called with a string binding as input. The string
**  binding consists of an object UUID, RPC protocol sequence, network address,
**  endpoint, and option strings, concatenated together. The ability to call
**  rpc_binding_from_string_binding is tested here. Although we expect that a 
**  valid binding handle is returned, we don't test its validity here.
**
**  INPUTS:             
**
**      argc            argument count
**      argv            array of pointers to string input arguments,
**                      containing a string binding
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
**  SIDE EFFECTS:       none 
**
**--
**/

int comtest_bind_from_str_bind (argc, argv, status)
int                 argc;
char                *argv[];
unsigned32          *status;

{
    rpc_binding_handle_t    binding_h;

    CHECK_ARGS(FROM_STR_BIND_ARGS);

    PRINT_ARGS;

    rpc_binding_from_string_binding ( 
        (unsigned_char_t *)argv[1],
        &binding_h, 
        status);

    CHECK_STATUS;

    rpc_binding_free (&binding_h, status);

    CHECK_STATUS;

    return(COMTEST_OK);
}


/*
**++
**
**  ROUTINE NAME:       comtest_bind_to_str_bind
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**
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

int comtest_bind_to_str_bind (argc, argv, status)
int                 argc;
char                *argv[];
unsigned32          *status;
{
    rpc_binding_handle_t    binding_h;
    unsigned_char_t         *string_binding;
    int                     retval;

    CHECK_ARGS(TO_STR_BIND_ARGS);

    PRINT_ARGS;

    rpc_binding_from_string_binding ( 
        (unsigned_char_t *)argv[1],
        &binding_h, 
        status);

    CHECK_STATUS;

    rpc_binding_to_string_binding (
        binding_h,
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

    return(retval);
}


/*
**++
**
**  ROUTINE NAME:       comtest_bind_str_binding_comp 
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**
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

int comtest_bind_str_binding_comp (argc, argv, status)
int                 argc;
char                *argv[];
unsigned32          *status;
{
    unsigned_char_t *obj_uuid = NULL;
    unsigned_char_t *protseq = NULL;
    unsigned_char_t *network_addr = NULL;
    unsigned_char_t *endpoint = NULL;
    unsigned_char_t *options = NULL;
    unsigned_char_t *string_binding = NULL;

    CHECK_ARGS(STR_BINDING_COMP_ARGS);

    PRINT_ARGS;

    obj_uuid        = (unsigned_char_t *)argv[1];
    protseq         = (unsigned_char_t *)argv[2];
    network_addr    = (unsigned_char_t *)argv[3];
    endpoint        = (unsigned_char_t *)argv[4];
    if (argc > STR_BINDING_COMP_ARGS)
    {
        options     = (unsigned_char_t *)argv[5];
        if ((strcmp ((char *)options, "\"\"")) == 0)
        {
            *options = (unsigned_char_t) '\0';
        }
    }

    string_binding = compose_string 
        (obj_uuid, protseq, network_addr, endpoint, options, status);

    FREE_STRING(&string_binding);

    CHECK_STATUS;

    return(COMTEST_OK);
}


/*
**++
**
**  ROUTINE NAME:       comtest_bind_str_binding_parse 
**
**  SCOPE:              global to test program 
**
**  DESCRIPTION:
**
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

int comtest_bind_str_binding_parse (argc, argv, status)
int                 argc;
char                *argv[];
unsigned32          *status;
{
    /*
     * these pointers are here to enhance readability
     */
    unsigned_char_t *obj_uuid = NULL;
    unsigned_char_t *protseq = NULL;
    unsigned_char_t *network_addr = NULL;
    unsigned_char_t *endpoint = NULL;
    unsigned_char_t *options = NULL;
    unsigned_char_t *string_binding = NULL;
    /*
     * pointers for strings returned from string_binding_parse:
     */
    unsigned_char_t *ret_obj_uuid = NULL;
    unsigned_char_t *ret_protseq = NULL;
    unsigned_char_t *ret_network_addr = NULL;
    unsigned_char_t *ret_endpoint = NULL;
    unsigned_char_t *ret_options = NULL;
    int             ret_val;
    int             flag_special_case;
    char            *cmpstr;

    CHECK_ARGS(STR_BINDING_PARSE_ARGS);

    PRINT_ARGS;

    flag_special_case = 0;
    obj_uuid        = (unsigned_char_t *)argv[1];
    protseq         = (unsigned_char_t *)argv[2];
    network_addr    = (unsigned_char_t *)argv[3];
    endpoint        = (unsigned_char_t *)argv[4];
    if ((strcmp (obj_uuid,"NIL")) == 0)
        *obj_uuid = '\0';
    if ((strcmp (protseq,"NIL")) == 0)
        *protseq = '\0';
    if ((strcmp (network_addr,"NIL")) == 0)
        *network_addr = '\0';
    if ((strcmp (endpoint,"NIL")) == 0)
        *endpoint = '\0';

    if (argc > STR_BINDING_PARSE_ARGS)
    {
        options     = (unsigned_char_t *)argv[5];
        if ((strcmp ((char *)options, "\"\"")) == 0)
        {
            *options = (unsigned_char_t) '\0';
        }
    }

    /*
     * If the protseq & network_addr args are "", then this is a special case; 
     * obj_uuid arg is really a full string binding (kind of kludgey, I know, 
     * but this isn't the product, just a test program).
     */
    if (((strlen ((char *)protseq)) == 0) &&
        ((strlen ((char *)network_addr)) == 0))
    {
        flag_special_case = 1;
        string_binding = obj_uuid;
    }
    else
    {
        string_binding = compose_string 
            (obj_uuid, protseq, network_addr, endpoint, options, status);

        if (*status != rpc_s_ok)
        {
            FREE_STRING(&string_binding);
            return(RPC_CALL_FAILED);
        }
    }

    if (flag_special_case != 1)
    {
        ret_val = test_str_bind_parse (obj_uuid, protseq,
                network_addr, endpoint, options, 
                &string_binding, status );
        if (ret_val != COMTEST_OK) return (ret_val);
    }

    rpc_string_binding_parse (string_binding, &ret_obj_uuid, &ret_protseq,
        &ret_network_addr, &ret_endpoint, &ret_options, status);

    if (verbose)
    {
        fprintf (stdout, "tested string binding \"%s\", returning\n",
            string_binding);
        fprintf (stdout, "object uuid \"%s\", protocol sequence \"%s\",\n",
            ret_obj_uuid, ret_protseq);
        fprintf (stdout, "network address \"%s\", endpoint \"%s\",",
            ret_network_addr, ret_endpoint);
        fprintf (stdout, " and network options \"%s\".\n", ret_options);
    }

    if (flag_special_case == 1)
    {
        ret_val = COMTEST_OK;
        if (*status != in_status)
            CHECK_STATUS;
        return (ret_val);
    }

    /*
     * Check to see that the strings returned are lexically equal to the input
     * strings.
     */
    ret_val = COMTEST_OK;
    if ((strcmp ((char *)obj_uuid, (char *)ret_obj_uuid)) != 0) 
    {
        if (verbose) fprintf (stdout, "%s does not match original, \"%s\".\n",
            "object uuid", obj_uuid);
        ret_val |= COMTEST_FAILED;
    }
    if ((strcmp ((char *)protseq, (char *)ret_protseq)) != 0) 
    {
        if (verbose) fprintf (stdout, "%s does not match original, \"%s\".\n",
            "protocol sequence", protseq);
        ret_val |= COMTEST_FAILED;
    }
    if ((strcmp ((char *)network_addr, (char *)ret_network_addr)) != 0) 
    {
        if (verbose) fprintf (stdout, "%s does not match original, \"%s\".\n",
            "network address", network_addr);
        ret_val |= COMTEST_FAILED;
    }
    cmpstr = (char *) strchr ((char*)endpoint, '=');
    if (cmpstr == NULL) 
        cmpstr = (char *)endpoint;
    else
        cmpstr++;
    if ((strcmp (cmpstr, (char *)ret_endpoint)) != 0) 
    {
        if (verbose) fprintf (stdout, 
            "%s \"%s\" does not match original, \"%s\".\n",
            "endpoint", ret_endpoint, cmpstr);
        ret_val |= COMTEST_FAILED;
    }
#if 0
    cmpstr = (char *) strchr ((char *)options, '=');
    if (cmpstr == NULL) 
        cmpstr = (char *)options;
    else
        cmpstr++;
#endif
    if (options != NULL)
    {
        cmpstr = (char *)options;   /* forget all that, this is correct! */
        if ((strcmp (cmpstr, (char *)ret_options)) != 0) 
        {                                             
            if (verbose) fprintf (stdout, 
                "%s \"%s\" does not match original, \"%s\".\n",
                "option(s)", ret_options, options);
            ret_val |= COMTEST_FAILED;
        }
    }

    FREE_STRING(&ret_obj_uuid);
    FREE_STRING(&ret_protseq);
    FREE_STRING(&ret_network_addr);
    FREE_STRING(&ret_endpoint);
    FREE_STRING(&ret_options);
    FREE_STRING(&string_binding);

    CHECK_STATUS;

    return(ret_val);
}

/*
 * C O M P O S E _ S T R I N G 
 *
 * Jacket for calling rpc_string_binding_compose.
 * 
 * RETURNS: a pointer to the string_binding
 */

static unsigned_char_t *compose_string (obj_uuid, protseq, network_addr, 
                            endpoint, options, status)

unsigned_char_t *obj_uuid;
unsigned_char_t *protseq;
unsigned_char_t *network_addr;
unsigned_char_t *endpoint;
unsigned_char_t *options;
unsigned32      *status;
{
unsigned_char_t *string_binding;

    string_binding = NULL;

    rpc_string_binding_compose (obj_uuid, protseq, network_addr, 
        endpoint, options, &string_binding, status);

    return (string_binding);
}

/*
 * T E S T _ S T R _ B I N D _ P A R S E
 *
 *
 */

static int test_str_bind_parse (
    obj_uuid,
    protseq,
    network_addr,
    endpoint,
    options,
    string_binding,
    status
    )

unsigned_char_t *obj_uuid;
unsigned_char_t *protseq;
unsigned_char_t *network_addr;
unsigned_char_t *endpoint;
unsigned_char_t *options;
unsigned_char_t **string_binding;
unsigned32      *status;

{
    unsigned_char_t *ret_obj_uuid = NULL;
    unsigned_char_t *ret_protseq = NULL;
    unsigned_char_t *ret_network_addr = NULL;
    unsigned_char_t *ret_endpoint = NULL;
    unsigned_char_t *ret_options = NULL;
    int     ret_val = COMTEST_OK;

    /*
     * There are 2^5 possible combinations of substrings and NULLs.
     * Right now, we'll only test what we quess are the most likely.
     */
    ret_val |= parse_test ("all strings", *string_binding, obj_uuid,
        protseq, network_addr, endpoint, options, &ret_obj_uuid, 
        &ret_protseq,
        &ret_network_addr, &ret_endpoint, &ret_options, status);
    ret_val |= parse_test ("object uuid only", *string_binding, obj_uuid,
        protseq, network_addr, endpoint, options, 
        &ret_obj_uuid, NULL, NULL, NULL, NULL, status);
    ret_val |= parse_test ("protocol sequence only", *string_binding, obj_uuid,
        protseq, network_addr, endpoint, options, 
        NULL, &ret_protseq, NULL, NULL, NULL, status);
    ret_val |= parse_test ("test", *string_binding, obj_uuid,
        protseq, network_addr, endpoint, options, 
        NULL, NULL, &ret_network_addr, NULL, NULL, status);
    ret_val |= parse_test ("test", *string_binding, obj_uuid,
        protseq, network_addr, endpoint, options, 
        NULL, NULL, NULL, &ret_endpoint, NULL, status);
    ret_val |= parse_test ("test", *string_binding, obj_uuid,
        protseq, network_addr, endpoint, options, 
        NULL, NULL, NULL, NULL, &ret_options, status);
    /*
     * Also, we'll test a more or less obvious pathological case.
     */
    ret_val |= parse_test ("test", *string_binding, obj_uuid,
        protseq, network_addr, endpoint, options, 
        NULL, NULL, NULL, NULL, NULL, status);

#if 0
    FREE_STRING(*string_binding);
#endif

    return (ret_val);
}

/*
 * P A R S E _ T E S T
 */
static int parse_test (
    verbose_text,
    string_binding,
    obj_uuid,
    protseq,
    network_addr,
    endpoint,
    options,
    ret_obj_uuid,
    ret_protseq,
    ret_network_addr,
    ret_endpoint,
    ret_options,
    status
    )

char            *verbose_text;
unsigned_char_t *string_binding;
unsigned_char_t *obj_uuid;
unsigned_char_t *protseq;
unsigned_char_t *network_addr;
unsigned_char_t *endpoint;
unsigned_char_t *options;
unsigned_char_t **ret_obj_uuid;
unsigned_char_t **ret_protseq;
unsigned_char_t **ret_network_addr;
unsigned_char_t **ret_endpoint;
unsigned_char_t **ret_options;
unsigned32      *status;

{
    char    *cmpstr;
    int     ret_val = COMTEST_OK;

    rpc_string_binding_parse (string_binding, ret_obj_uuid, ret_protseq,
        ret_network_addr, ret_endpoint, ret_options, status);

    CHECK_STATUS;

    /*
     * Check to see that the ret_* strings are lexically equal to 
     * the originals.
     */
    if (ret_obj_uuid != NULL)
    {
        if ((strcmp ((char *)obj_uuid, (char *)*ret_obj_uuid)) != 0)
        {
            if (verbose) fprintf (stdout, 
              "%s: %s \"%s\" does not match original, \"%s\".\n",
              verbose_text, "object uuid", *ret_obj_uuid, obj_uuid);
            ret_val |= COMTEST_FAILED;
        }
        FREE_STRING(ret_obj_uuid);
    }
    if (ret_protseq != NULL)
    {
        if ((strcmp ((char *)protseq, (char *)*ret_protseq)) != 0)
        {
            if (verbose) fprintf (stdout, 
              "%s: %s \"%s\" does not match original, \"%s\".\n",
              verbose_text, "protocol sequence", *ret_protseq, protseq);
            ret_val |= COMTEST_FAILED;
        }
        FREE_STRING(ret_protseq);
    }
    if (ret_network_addr != NULL)
    {
        if ((strcmp ((char *)network_addr, (char *)*ret_network_addr)) != 0)
        {
            if (verbose) fprintf (stdout, 
              "%s: %s \"%s\" does not match original, \"%s\".\n",
              verbose_text, "network address", *ret_network_addr, network_addr);
            ret_val |= COMTEST_FAILED;
        }
        FREE_STRING(ret_network_addr);
    }
    if (ret_endpoint != NULL)
    {
        cmpstr = (char *) strchr ((char*)endpoint, '=');
        if (cmpstr == NULL) 
            cmpstr = (char *)endpoint;
        else
            cmpstr++;
        if ((strcmp (cmpstr, (char *)*ret_endpoint)) != 0) 
        {
            if (verbose) fprintf (stdout, 
              "%s: %s \"%s\" does not match original, \"%s\".\n",
               verbose_text, "endpoint", *ret_endpoint, cmpstr);
            ret_val |= COMTEST_FAILED;
        }
        FREE_STRING(ret_endpoint);
    }
    if ((options != NULL) && (ret_options != NULL))
    {
        cmpstr = (char *)options;
        if ((strcmp (cmpstr, (char *)*ret_options)) != 0) 
        {
            if (verbose) fprintf (stdout, 
              "%s: %s \"%s\" does not match original, \"%s\".\n",
                verbose_text, "option(s)", *ret_options, options);
            ret_val |= COMTEST_FAILED;
        }
        FREE_STRING(ret_options);
    }

    return (ret_val);
}

