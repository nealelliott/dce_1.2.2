/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: util_macros.h,v $
 * Revision 1.1.4.2  1996/02/18  23:06:01  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  22:26:06  marty]
 *
 * Revision 1.1.4.1  1995/12/11  19:31:40  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:16:43  root]
 * 
 * Revision 1.1.2.2  1993/01/07  23:40:30  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:46:39  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:30:32  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
 * Allocate memory using malloc, issue error on failure and exit.
 *      var     = variable to hold returned address
 *      size    = size of memory to allocate
 */
#define MALLOC(var, size) \
{ \
    var = (idl_void_p_t)malloc(size); \
    if (var == NULL) \
    { \
        fprintf(stderr, "Out of malloc heap memory!\n"); \
        exit(0); \
    } \
}

/*
 * Free memory using free, issue error on failure and exit.
 *      var     = address of memory to deallocate
 */
#define FREE(var) \
{ \
    if (free(var) == -1) \
    { \
        fprintf(stderr, "Attempted to free invalid memory address!\n"); \
        exit(0); \
    } \
}

/*
 * Allocate memory using rpc_ss_allocate, issue error on failure and exit.
 *      var     = variable to hold returned address
 *      size    = size of memory to allocate
 */
#define RPC_SS_ALLOCATE(var, size) \
{ \
    var = rpc_ss_allocate(size); \
    if (var == NULL) \
    { \
        fprintf(stderr, "Out of rpc_ss_allocate heap memory!\n"); \
        exit(0); \
    } \
}

/*
 * Allocate memory for a conformant type - Assumes that the conformant array has
 * length 1 in the type definition which makes the type appear to be fixed size
 * but (array_size-1) more array elements need to be allocated.  Assume integer
 * array elements and automatic type conversion between integer types.
 * Allocates an extra 2 guard areas which are filled in with the guard pattern.
 *      routine_name    = name of RPC routine for error messages
 *      cfmt_mem        = variable for returned pointer to allocated memory
 *      bmk_cfmt_mem    = variable for a copy of cfmt_mem for later comparison
 *      cfmt_type       = type to allocate storage for
 *      array_size      = size of the contained conformant array
 *      array_elt_type  = array element type
 *      cfmt_array      = lvalue for conformant array, e.g. struct->array_field
 *      bmk_cfmt_array  = lvalue for copy of conformant array
 *      guard           = guard pattern
 */
#define ALLOC_CFMT_MEM(routine_name, cfmt_mem, bmk_cfmt_mem, cfmt_type, \
            array_size, array_elt_type, cfmt_array, bmk_cfmt_array, guard) \
{ \
    long i, total_size, cfmt_size; \
    char *guard1, *guard2; \
    cfmt_size = sizeof(cfmt_type) + (array_size-1) * sizeof(array_elt_type); \
    total_size = cfmt_size + 2 * sizeof(guard); \
    MALLOC(guard1, total_size); \
    MALLOC(bmk_cfmt_mem, cfmt_size); \
    cfmt_mem = &guard1[sizeof(guard)]; \
    guard2 = &guard1[total_size-sizeof(guard)]; \
    for (i = 0 ; i < sizeof(guard) ; i++) \
        guard1[i] = guard2[i] = guard[i]; \
    for (i = 0 ; i < array_size ; i++) \
        cfmt_array[i] = bmk_cfmt_array[i] = random(); \
}

/*
 * Modify the elements of a conformant array.
 *      cfmt_mem        = pointer to allocated memory
 *      array_size      = size of the contained conformant array
 *      cfmt_array      = lvalue for conformant array, e.g. struct->array_field
 *      expression      = expression that relates cfmt_mem elements to
 *                        bmk_cfmt_mem elements, e.g. *2
 */
#define MODIFY_CFMT_MEM(cfmt_mem, array_size, cfmt_array, expression) \
{ \
    int i; \
    for (i = 0; i < array_size ; i++) \
        cfmt_array[i] = cfmt_array[i] expression; \
}

/*
 * Check that elements of a conformant array have the expected values.
 *      routine_name    = name of RPC routine for error messages
 *      cfmt_mem        = pointer to allocated memory
 *      bmk_cfmt_mem    = pointer to copy of cfmt_mem for comparison
 *      cfmt_type       = datatype of allocated memory
 *      array_size      = size of the contained conformant array
 *      array_elt_type  = array element type
 *      cfmt_array      = lvalue for conformant array, e.g. struct->array_field
 *      bmk_cfmt_array  = lvalue for copy of conformant array
 *      guard           = guard pattern
 *      expression      = expression that relates cfmt_mem elements to
 *                        bmk_cfmt_mem elements, e.g. *2
 *      failures        = variable for count of local failures
 */
#define CHECK_CFMT_MEM(routine_name, cfmt_mem, bmk_cfmt_mem, cfmt_type, \
            array_size, array_elt_type, cfmt_array, bmk_cfmt_array, guard, \
            expression, failures) \
{ \
    long i, cfmt_size; \
    char *guard1, *guard2; \
    cfmt_size = sizeof(cfmt_type) + (array_size-1) * sizeof(array_elt_type); \
    guard1 = ((char *)cfmt_mem) - sizeof(guard); \
    guard2 = &(((char *)cfmt_mem)[cfmt_size]); \
    for (i = 0; i < sizeof(guard) ; i++) \
        if (guard1[i] != guard[i] || guard2[i] != guard[i]) \
        { \
            fprintf(stderr, \
                "Guard values incorrect after call to routine_name\n"); \
            failures++; \
            break; \
        } \
    for (i = 0; i < array_size ; i++) \
        if (cfmt_array[i] != bmk_cfmt_array[i] expression) \
        { \
            fprintf(stderr, \
                "Array values incorrect after call to routine_name\n"); \
            failures++; \
            break; \
        } \
}

/*
 * Call an RPC routine.
 *      routine_name    = name of RPC routine to call
 *      arguments       = routine argument list (enclose in parentheses)
 *      mgr_failures    = variable for count of manager failures
 *      failures        = variable for count of local failures
 */
#define CALL(routine_name, arguments, mgr_failures, failures) \
{ \
    mgr_failures = routine_name arguments; \
    if (mgr_failures != 0) \
    { \
        fprintf(stderr, "Failure in routine_name manager routine\n"); \
        failures += mgr_failures; \
    } \
}

/*
 * Report a failure and bump error count.
 *      error_string    = error text for fprintf
 *      failures        = variable for count of local failures
 */
#define FAIL(error_string, failures) \
{ \
    fprintf(stderr, error_string); \
    failures++; \
}

/*
 * Report a failure, bump error count, and break out of enclosing loop.
 *      error_string    = error text for fprintf
 *      failures        = variable for count of local failures
 */
#define FAIL_BREAK(error_string, failures) \
{ \
    fprintf(stderr, error_string); \
    failures++; \
    break; \
}
