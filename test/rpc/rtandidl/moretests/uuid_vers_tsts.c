/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: uuid_vers_tsts.c,v $
 * Revision 1.1.4.2  1996/02/17  23:22:58  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:37  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:13:00  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:46:19  root]
 * 
 * Revision 1.1.2.2  1993/01/13  19:50:53  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:17:53  bbelch]
 * 
 * Revision 1.1  1992/01/19  03:38:58  devrcs
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
**      uuid_vers_tsts.c
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
**  16-may-91   woodbury    original
**
**  %a%private_end  
**
*/

#include    <dce/rpc.h>
#include    <tstdriver.h>
#include    <comtest.h>

#define     UUID_VERS2_ARGS 1
extern int verbose;

/*
**++
**
**  ROUTINE NAME:       comtest_uuid_vers2
**
**  SCOPE:              global to test program
**
**  DESCRIPTION:
**      
**
**  INPUTS:             none [or...]
**
**      arg1            A one line description of the argument.
**
**  INPUTS/OUTPUTS:     none [or...]
**
**      arg2            A one line description of the argument.
**
**  OUTPUTS:            none [or...]
**
**      arg3            A one line description of the argument.
**
**  IMPLICIT INPUTS:    none [or...]
**
**  IMPLICIT OUTPUTS:   none [or...]
**
**  FUNCTION VALUE:     none [or...]
**
**      return          A one line description of the return value
**                      (including a listing of status codes, if any)
**
**  SIDE EFFECTS:       none [or...]
**
**--
**/

int comtest_uuid_vers2 ( argc, argv, status )

int argc;
char *argv[];
unsigned32  *status;
{
#define UUID_VERS_2 (2 << 12)
#define UUID_VERSION_MASK 0xf000    /* this should be the same in uuid.c */
    uuid_t      uuid1;              /* this will be a vers. 1 uuid */
    uuid_t      uuid2;              /* this will be a vers. 2 uuid */
    uuid_t      uuid_nil2;          /* this will be a nil vers. 2 uuid */
    uuid_t      user_uuid;
    unsigned_char_p_t   uuid_str1, uuid_str2;
    unsigned32  failed;
    unsigned32  i;
    unsigned16  hash_val;
    unsigned32  temp_status;

    CHECK_ARGS(UUID_VERS2_ARGS);

    PRINT_ARGS;

    failed = 0;
    uuid_str1 = NULL;
    uuid_str2 = NULL;

    /*
     * create a vers 1 uuid
     */
    uuid_create (&uuid1, status);
    if (*status != rpc_s_ok)
    {
        printf ("%s failed: st = %d.\n",
            "uuid_create()",
            *status);
        return (COMTEST_FAILED);
    }

    /*
     * copy all fields except the version field and set the version 
     */
    uuid2.time_low                  = uuid1.time_low;
    uuid2.time_mid                  = uuid1.time_mid;
    uuid2.time_hi_and_version       = ((uuid1.time_hi_and_version & 
                                        (~ UUID_VERSION_MASK)) | UUID_VERS_2);
    uuid2.clock_seq_hi_and_reserved = uuid1.clock_seq_hi_and_reserved;
    uuid2.clock_seq_low             = uuid1.clock_seq_low;
    for (i = 0; i < 6; i++)
        uuid2.node[i] = uuid1.node[i];

    uuid_to_string (&uuid1, &uuid_str1, status);
    if (*status != rpc_s_ok)
    {
        printf ("%s failed converting a version 1 uuid: st = %d.\n",
            "uuid_to_string()",
            *status);
        failed++;
    }

    uuid_to_string (&uuid2, &uuid_str2, status);
    if (*status != rpc_s_ok)
    {
        printf ("%s failed converting a version 2 uuid: st = %d.\n",
            "uuid_to_string()",
            *status);
        failed++;
    }

    /*
     * print out the uuids to show the user but not for the regression tests
     */
    if (verbose)
    {
        printf ("vers. 1 uuid: %s\n", 
            uuid_str1);
        printf ("vers. 2 uuid: %s\n", 
            uuid_str2);
    }

    /*
     * compare a vers. 1 uuid to a vers. 2 uuid (identical except vers) using
     * uuid_equal and uuid_compare.
     */
    if ((uuid_equal (&uuid1, &uuid2, status)) != false) 
    {
        printf ("%s did not return %s, as expected.\n",
            "uuid_equal()",
            "false");
        failed++;
    }

    if (*status != rpc_s_ok)
    {
        printf ("%s failed: st = %d.\n",
            "uuid_equal()",
            *status);
        failed++;
    }

    if ((uuid_compare (&uuid1, &uuid2, status)) != (-1)) 
    {
        printf ("%s did not return %s, as expected.\n",
            "uuid_compare()",
            "'-1'");
        failed++;
    }

    if ((uuid_compare (&uuid2, &uuid1, status)) != 1) 
    {
        printf ("%s did not return %s, as expected.\n",
            "uuid_compare()",
            "'1'");
        failed++;
    }

    if (*status != rpc_s_ok)
    {
        printf ("%s failed: st = %d.\n",
            "uuid_compare()",
            *status);
        failed++;
    }

    /*
     * Make sure uuid_hash works
     */
    hash_val = 0;
    hash_val = uuid_hash (&uuid2, status);
    if (*status != rpc_s_ok)
    {
        printf ("%s failed: st = %d.\n",
            "uuid_hash()",
            *status);
    } 
    else if (verbose) 
    {
        printf ("uuid_hash() produced %d\n", hash_val);
    }

    /* 
     * create uuid from string uuid on command line (just testing
     * uuid_from_string())
     */
    if (argv[1] != NULL)
    {
        uuid_from_string (argv[1], &user_uuid, status);
    }
    else
    {
        *status = rpc_s_ok;
    }

    if (*status != rpc_s_ok)
    {
        printf ("%s failed: st = %d.\n",
            "uuid_from_string()",
            *status);
    }

    /*
     * create a nil uuid (overwrite the old one), set the vers field to 2, 
     * and see if it's nil.
     */
    uuid_create_nil (&uuid2, status);
    if (*status != rpc_s_ok)
    {
        printf ("%s failed: st = %d.\n",
            "uuid_create_nil()",
            *status);
    }
    else if ((uuid_is_nil (&uuid2, status)) == false)
    {
        printf ("nil uuid not created or uuid_is_nil test broken!\n");
        failed++;
    }

    uuid2.time_hi_and_version       = UUID_VERS_2;
    if ((uuid_is_nil (&uuid2, status)) != false)
    {
        printf ("vers 2 uuid is nil and st = %d.\n",
            *status);
        failed++;
    }

    /*
     * cleanup
     */
    rpc_string_free (&uuid_str1, &temp_status);
    rpc_string_free (&uuid_str2, &temp_status);

    if (! failed)
    {
        if (verbose) printf ("All vers. 2 uuid tests suceeded!\n");
        return (COMTEST_OK);
    }
    else
    {
        if (verbose) printf ("%d tests failed.\n", failed);
        return (COMTEST_FAILED);
    }
}

