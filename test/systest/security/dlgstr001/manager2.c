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
 * $Log: manager2.c,v $
 * Revision 1.1.4.2  1996/02/17  23:28:31  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:45:30  marty]
 *
 * Revision 1.1.4.1  1995/12/11  22:45:37  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:21:40  root]
 * 
 * Revision 1.1.2.2  1994/10/06  20:43:28  agd
 * 	expand copyright
 * 	[1994/10/06  14:31:56  agd]
 * 
 * Revision 1.1.2.1  1994/09/19  18:17:50  bhaim
 * 	multidelegate test
 * 	[1994/09/19  17:50:59  bhaim]
 * 
 * $EndLog$
 */
/*
 ***************************************************************************
 * This is the server-side RPC manager function that implements the remote
 * procedure defined in the .idl file.  The server stub calls this function
 * when an RPC request comes in for this interface.
 ***************************************************************************
 */
/*
 * (c) Copyright 1992, 1993 Hewlett-Packard Co.
 */

#include <stdlib.h>
#include <stdio.h>
#include <ctype.h>
#include <dce/sec_login.h>
#include "common2.h"
#include "identity.h"
#include "string_conv2.h"
#include "string_conv3.h"
#include <dce/binding.h>
#include <dce/secidmap.h>
#include <dce/sec_cred.h>

#define IFSPEC_IN         string_conv2_v1_0_s_ifspec
#define IFSPEC_OUT        string_conv3_v1_0_s_ifspec
#define INTERFACE_NUMBER  conv_2_interface_number

/*
 * This manager takes an explicit binding handle and a string to convert.
 * The string is passed as an [in,out] reference pointer: it cannot be NULL
 * and must point to a string of at most max_conv_string_len characters.
 * The manager will convert it to all uppercase in place and return it.  The
 * server stub will then transmit the new string back to the client.
 * 
 * Note: the code in this manager function must be reentrant as it may
 * be running simultaneously in multiple server threads.
 */

void string_convert_uppercase2
(
/*[in]*/     handle_t        h,			/* Use explicit binding */
/*[in,out]*/ string_ref_t    conv_string,	/* The string to convert */
/*[in]*/     string_ref_t    num_serv,          /* The server number to play on */
/*[out]*/ error_status_t     *auth_status       
)
{
   int                   i, j, num_requested;
   unsigned32            status,  ignore; 
   char                  cds_entry_name[256];
   rpc_binding_handle_t  newbh;
   rpc_ns_handle_t       import_context;

/******/
   sec_login_handle_t           deleg_context;
   rpc_authz_cred_handle_t	privs, *privs_p = &privs;
   unsigned_char_p_t		server_princ_name;
   unsigned_char_t 		*uuid_string;

   unsigned32			protect_level, authn_svc, authz_svc;
   error_status_t              	st, *st_p = &st;

   sec_cred_attr_cursor_t	attr_cursor;
   sec_attr_t			attr, *attr_p = &attr;
   sec_cred_cursor_t		delegate_cursor;
   sec_id_pa_t			*pa;
   sec_id_delegation_type_t	delegate_type;
   sec_cred_pa_handle_t		delegate;
   int				init_flag;
/******/

   /* Trying to get epac */
	
    printf("Delegation Chain:\n");

    rpc_binding_inq_auth_caller(h, &privs, &server_princ_name, &protect_level,
	&authn_svc, &authz_svc, st_p);


    sec_cred_is_authenticated(privs, st_p);
    delegate = (sec_cred_pa_handle_t *) sec_cred_get_initiator(privs, st_p);
    init_flag = TRUE;
    sec_cred_initialize_cursor(&delegate_cursor, st_p);
    while (GOOD_STATUS(st_p)) {

       pa = (sec_id_pa_t *) sec_cred_get_pa_data(delegate, st_p);
       if (GOOD_STATUS(st_p))  {
            error_status_t xst;
            sec_rgy_name_t principal_name;
            unsigned_char_p_t  princ_namep = principal_name;

            principal_name[0] = '\0';
            /* try to convert the principal uuid to a name */
            sec_id_gen_name(sec_rgy_default_handle, &pa->realm.uuid,
                            &pa->principal.uuid, NULL, NULL,
                            principal_name, &xst);

            if (!STATUS_OK(&xst)) {
                printf ("status = %x\n", xst);
                /*
                 * If the uuid->name conversion failed, then
                 * print the uuid since its the only thing we
                 */
                uuid_to_string(&pa->principal.uuid, &princ_namep, &xst);
                if (!STATUS_OK(&xst)) {
                    /*
                     * If the uuid_to_string conversion failed
                     * (highly unlikely) then give up and print
                     * "UNKNOWN"
                     */
                    princ_namep = (unsigned_char_p_t) "<UNKNOWN>";
                }
            }
            printf ("Principal = %s\n", princ_namep);
        }
        else
            printf ("Unable to get delegate Principal data\n");

        /* if this is the initiator, print attribute info */

        if (init_flag) {
            sec_cred_initialize_attr_cursor(&attr_cursor, st_p);
            if (GOOD_STATUS(st_p)) {
                printf ("\tAttributes:\n");
                while (GOOD_STATUS(st_p)) {
                    sec_cred_get_extended_attrs(delegate, &attr_cursor,
                                            &attr, st_p);
                    if (GOOD_STATUS(st_p))
                        print_attrs(&attr, st_p);
                }
                printf("\tEnd of Attribute List.\n");
            }
            else
                printf("Unable to initialize cursor - status %d\n", *st_p);
        }

 	delegate = (sec_cred_pa_handle_t *) sec_cred_get_delegate(privs, 
		&delegate_cursor, st_p);
        init_flag = FALSE;
    }


/******/
   /*
    * Well, we have the authorization to do the upshift.  Now lets
    * see if we are the right server.  Defined in the .idl file is
    * the variable interface_number, which indicates the server
    * number currently in action.  If the server number requested by
    * the user is less than the interface number, something went wrong
    * with the chain, so we have to exit with an error.  If the
    * number requested is greater than the current interface number,
    * then we should call the next server in the chain.  If the two
    * numbers are equal, then we do the upshift work and finish.
    */
   num_requested = atoi((char *)num_serv);
   if (num_requested > INTERFACE_NUMBER)
   {
      printf("call the next server in the chain...\n");
      sprintf(cds_entry_name, "/.:/subsys/HP/sample-apps/string-convert3");

      rpc_ns_binding_import_begin(rpc_c_ns_syntax_default,
				 (unsigned_char_t *)cds_entry_name,
				 IFSPEC_OUT,
				 NULL,
				 &import_context,
				 &status);

      rpc_ns_mgmt_handle_set_exp_age(import_context,
				 0,
				 &status);

      rpc_ns_binding_import_next(import_context,
				 &newbh,
				 &status);

      rpc_ns_binding_import_done(&import_context, &ignore);

      deleg_context = sec_login_become_delegate (
                        privs,
                        iden_info->login_context,
                        sec_id_deleg_type_traced,
                        NULL,  /* no delegate restrictions */
                        NULL,  /* no target restrictions */
                        NULL,  /* no optional restrictions */
                        NULL,  /* no required restrictions */
                        sec_id_compat_mode_caller,
                        &status
                    );

    if (status != error_status_ok) {
        print_error (status, "sec_login_become_delegate() failed with error:" );
        exit(1);
    }

    rpc_binding_set_auth_info (
          newbh,
          (unsigned char *)string_conv3_SERVER_PRINCIPAL_NAME,  /* from real-common.h */
          rpc_c_protect_level_pkt,
          rpc_c_authn_dce_secret,
          deleg_context,
          rpc_c_authz_dce,
          &status
      );
      
      PRINT_FUNC(PRINT_HANDLE, "Calling string_convert_uppercase3(%s), servers(%s)\n",
		 conv_string, num_serv);

      string_convert_uppercase3(newbh, conv_string, num_serv, auth_status);

      PRINT_FUNC(PRINT_HANDLE, "string_convert_uppercase3 returned \"%s\"\n",
		 conv_string);
      
      sec_login_purge_context(&deleg_context, &status);

      rpc_binding_free(&newbh, &ignore);
      
   }
   else if (num_requested < INTERFACE_NUMBER)
   {
      printf("ERROR: interface number (%s) is greater than number requested (%s)\n",
              INTERFACE_NUMBER, num_requested);
      exit(1);
   }
   else
   {
      /*
       * If we get here, the number requested and the interface number
       * are the same, so we can just do the work.
       */
      PRINT_FUNC(PRINT_HANDLE, ">>> string_convert_uppercase2(%s), servers(%s)\n",
                 conv_string, num_serv);

      /* 
       * Capitalize and copy the string into the new fixed-length array.
       * Since the IDL file specified conv_string as a [string] argument it
       * was passed as a NULL-terminated string.  The maximum length of this
       * string from the IDL file is max_conv_string_len: don't overstep it.
       * (This may not be necessary -- the client stub may not pass more than
       * max_conv_string_len bytes, but it never hurts to be safe :-).
       */
      for (i=0; (conv_string[i] != '\0') && (i < max_conv_string_len); i++)
         conv_string[i] = toupper(conv_string[i]);

      PRINT_FUNC(PRINT_HANDLE, "<<< string_convert_uppercase2(%s), servers(%s)\n",
                 conv_string, num_serv);
      return;
   }
}
