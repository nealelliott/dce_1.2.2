/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: key_mgmt.c,v $
 * Revision 1.2.4.2  1996/02/17  23:25:25  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:35  marty]
 *
 * Revision 1.2.4.1  1995/12/11  21:48:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:18  root]
 * 
 * Revision 1.2.2.4  1993/01/29  21:11:30  cuti
 * 	Add delete_key_type(), initialize_cursor(), release_cusor(), manage_key() test
 * 	[1993/01/28  16:03:31  cuti]
 * 
 * Revision 1.2.2.3  1992/12/31  17:28:55  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:54:27  htf]
 * 
 * Revision 1.2.2.2  1992/10/14  13:39:49  cuti
 * 	ot5562: Add key_pepper and key.version to key structure to avoid access violatio.
 * 	[1992/10/12  16:33:25  cuti]
 * 
 * Revision 1.2  1992/01/19  22:14:33  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
*/
/*
 *  OSF DCE Version 1.0 
*/
/* 
 * Copyright 1991 by Apollo Computer, Inc.
 * 
 * Permission to use, copy, modify, and distribute this
 * software and its documentation for any purpose and without
 * fee is hereby granted, provided that the above copyright
 * notice appear in all copies and that both that copyright
 * notice and this permission notice appear in supporting
 * documentation, and that the name of Apollo Computer, Inc not
 * be used in advertising or publicity pertaining to distribution
 * of the software without specific, written prior permission.
 * Apollo Computer, Inc. makes no representations about the
 * suitability of this software for any purpose.  It is provided
 * "as is" without express or implied warranty.
 * 
 *
 *   
 *	Extracted	12/19/91 12:57:38
 *	from delta on	12/10/91 16:53:35
 *
 *  $Log: key_mgmt.c,v $
 * Revision 1.2.4.2  1996/02/17  23:25:25  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:35  marty]
 *
 * Revision 1.2.5.2  1996/02/17  22:43:35  marty
 * 	Update OSF copyright year
 *
 * Revision 1.2.4.1  1995/12/11  21:48:30  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:18  root]
 *
 * Revision 1.2.3.2  1995/12/11  20:52:18  root
 * 	Submit OSF/DCE 1.2.1
 *
 * Revision 1.2.2.4  1993/01/29  21:11:30  cuti
 * 	Add delete_key_type(), initialize_cursor(), release_cusor(), manage_key() test
 * 	[1993/01/28  16:03:31  cuti]
 *
 * Revision 1.2.3.2  1993/01/28  16:03:31  cuti
 * 	Add delete_key_type(), initialize_cursor(), release_cusor(), manage_key() test
 *
 * Revision 1.2.2.3  1992/12/31  17:28:55  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:54:27  htf]
 *
 * Revision 1.2.3.2  1992/12/31  16:54:27  htf
 * 	Embedding copyright notice
 *
 * Revision 1.2.2.2  1992/10/14  13:39:49  cuti
 * 	ot5562: Add key_pepper and key.version to key structure to avoid access violatio.
 * 	[1992/10/12  16:33:25  cuti]
 *
 * Revision 1.2.1.2  1992/10/12  16:33:25  cuti
 * 	ot5562: Add key_pepper and key.version to key structure to avoid access violatio.
 *
 * Revision 1.2  1992/01/19  22:14:33  devrcs
 * Dropping DCE1.0 OSF1_misc port archive
 *
 * Revision 1.3.2.2  92/01/06  11:42:35  zeliff
 * 	moving up to dce1.0/dce.75
 * 	[92/01/06  11:04:56  zeliff]
 * 
 * Revision 1.3.1.2  92/01/06  11:04:56  zeliff
 * 	moving up to dce1.0/dce.75
 * 
 * Revision 1.2.2.2  92/01/02  12:45:25  eperkins
 * 	Removed the PLAIN password #defines
 * 	[92/01/02  12:44:25  eperkins]
 * 
 * Revision 1.2.1.2  92/01/02  12:44:25  eperkins
 * 	Removed the PLAIN password #defines
 * 
 * Revision 1.2  91/12/20  17:05:55  devrcs
 * Adding dce.74d to the rcs tree
 * 
 *
 *
*/

#ifndef LINT
static char sccs_id[] = "src/test/security/api/moretests/key_mgmt.c, , dce_osf, dce.74c";
#endif

/* Include files */
#include <dce/sec_login.h>
#include <tsh_misc.h>
#include <dce/keymgmt.h>
#include <key_mgmt.h>
#include <dce/pgo.h>
#include <dce/dce_error.h> 

#define PLAIN_PASSWD(k)   k.key.tagged_union.plain

CtxBlock          *lCtx;
dce_error_string_t	error_buf;

/* Args required by the test program. */

#define MIN_ARGS 2

ndr_long_int TestFunction (ctx, argc, argv)

/* Formal parameters */

   CtxBlock          *ctx;
   ndr_long_int      argc;
   unsigned_char_p_t argv[];

   {

/* Local variables */

   unsigned         	args_required;

   ndr_long_int			subcase;	/* argv[0], subcase to run */
   error_status_t		estatus;	/* argv[1], expected return from test */

   unsigned char    	*principal_name;
   unsigned char    	*n_principal_name;
   unsigned32         	vno, n_vno, old_vno, next_vno;          /* key version number */
   ndr_long_int     	cpflg;      /* create principle flag */
   void            		*arg;
   sec_passwd_type_t   	keytype = sec_passwd_des;
   sec_passwd_rec_t 	key;
   sec_passwd_rec_t 	*key_p=&key;

   void            		*cursor;
   sec_passwd_rec_t 	*keyptr;
   sec_passwd_rec_t 	*n_keyptr;
   signed32              index;
   sec_key_mgmt_authn_service auth_service;
   sec_timeval_period_t 	garbage_collect_time;

   sec_login_handle_t	auth_identity;	/* sec_login_setup_identity */
   sec_login_auth_src_t	auth_src;	/* sec_login_validate_identity */
   boolean32			reset_passwd;	/* sec_login_validate_identity */


   error_status_t		funcStatus;	/* test status */
   error_status_t		expctdStatus;
   error_status_t		status;
   error_status_t		clnupStatus;	/* cleanup status */
   ndr_long_int         testStatus;

 


   lCtx=ctx;  

   if (argc < MIN_ARGS)
     {
     TSHMessage(ctx, MOFAILURE, "Expecting at least %d arguments, got %d\n",
                MIN_ARGS,argc);
     return (TSH_BAD_ARG_COUNT);
     }


/* Obtain the basic  arguments from the .tsh command line. */

   subcase = atol(argv[0]);
   estatus = atol(argv[1]);

    switch (subcase)
        {
        case KEY_MGMT_SET_KEY: 
        case KEY_MGMT_CHANGE_KEY:
		case KEY_MGMT_GEN_RAND_KEY:
		case KEY_MGMT_GEN_RAND_NO_SEED_KEY:
                    args_required=MIN_ARGS+5;        /* princ, arg, vno, authn, key */
            break;

        case KEY_MGMT_DELETE_KEY:
        case KEY_MGMT_DELETE_KEY_TYPE: 
        case KEY_MGMT_GET_KEY:
        case KEY_MGMT_GARBAGE_COLLECT:
        case KEY_MGMT_GET_NTH_KEY:
        case KEY_MGMT_FREE_KEY:
            args_required=MIN_ARGS+4;        /* princ, arg, vno, authn */
            break;

        case KEY_MGMT_INITIALIZE_CURSOR:
        case KEY_MGMT_GET_NEXT_KEY:
        case KEY_MGMT_RELEASE_CURSOR:
        case KEY_MGMT_GET_NEXT_KVNO:
        case KEY_MGMT_MANAGE_KEY:
            {
            args_required=MIN_ARGS+3;        /* princ, arg, authn */
            break;
            }

        default:
            {
            TSHMessage(ctx, MOFAILURE, "Invalid subcase #: %d\n",
                subcase);
            return (TSH_INVALID_ARGUMENT);
            }
        }


    if (argc !=args_required)
        {
         TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n",
                args_required, argc);
        return (TSH_BAD_ARG_COUNT);
        }

    expctdStatus = atol(argv[1]); 
    testStatus = TSH_TEST_FAILED;         /* initial */

    principal_name= argv[2];
    arg=argv[3];
    

    switch (subcase)
        {
        case KEY_MGMT_SET_KEY:
        case KEY_MGMT_CHANGE_KEY:
		case KEY_MGMT_GEN_RAND_KEY:
		case KEY_MGMT_GEN_RAND_NO_SEED_KEY:
            {
            if ( strcmp(argv[6], "GEN") == 0 ) 
                {
                key.pepper = NULL;
                key.version_number = sec_passwd_c_version_none;
                key.key.key_type = sec_passwd_des;
                memcpy(key.key.tagged_union.des_key, argv[6], sec_passwd_c_des_key_size);
                } 
            else 
                {
                key.pepper = NULL;
                key.version_number = sec_passwd_c_version_none;
                key.key.key_type = sec_passwd_plain;
                PLAIN_PASSWD(key) = (unsigned char *) argv[6];
                }
            }                                   /* fall through */

        case KEY_MGMT_DELETE_KEY:
        case KEY_MGMT_DELETE_KEY_TYPE: 
        case KEY_MGMT_GET_KEY:
        case KEY_MGMT_GARBAGE_COLLECT:
        case KEY_MGMT_GET_NTH_KEY: 
		case KEY_MGMT_FREE_KEY:
            {
            if ( sscanf((char *) argv[5],"%ld",&auth_service) != 1 )
                {
                TSHMessage(ctx, MOFAILURE, "invalid argument: auth service\n");
                return (TSH_INVALID_ARGUMENT);
                }
            if ( sscanf((char *) argv[4],"%ld",&vno) != 1 )
                {
                TSHMessage(ctx, MOFAILURE, "invalid argument: version number\n");
                return (TSH_INVALID_ARGUMENT);
                }

            break;
            }

        case KEY_MGMT_INITIALIZE_CURSOR:
        case KEY_MGMT_GET_NEXT_KEY:
        case KEY_MGMT_RELEASE_CURSOR:
        case KEY_MGMT_GET_NEXT_KVNO:
        case KEY_MGMT_MANAGE_KEY:
            if ( sscanf((char *) argv[4],"%ld",&auth_service) != 1 )
                {
                TSHMessage(ctx, MOFAILURE, "invalid argument: auth service\n");
                return (TSH_INVALID_ARGUMENT);
                }

            break;
        } 

	testStatus = TSH_SUCCESS;

    switch (subcase)
        {
        case KEY_MGMT_SET_KEY:
            { 
            sec_key_mgmt_set_key(auth_service, arg, 
                            principal_name , vno,
                            &key, &funcStatus);
     		dce_error_inq_text(funcStatus, error_buf, &status);
            TSHMessage (ctx, MOTRACE,
                        "sec_key_mgmt_set_key status: %s for '%s', vno: %d\n", 
                         error_buf, principal_name, vno);

            if (TSHStatus (ctx, TEST,
                           "sec_key_mgmt_set_key failed\n", estatus, funcStatus) )
                break;  

            sec_key_mgmt_get_key(auth_service, arg, 
                            principal_name , vno,
                            &keyptr, &funcStatus);
     		dce_error_inq_text(funcStatus, error_buf, &status);
            TSHMessage (ctx, MOTRACE,
                   "sec_key_mgmt_get_key status: %s, for '%s', vno: %d\n", 
                   error_buf, principal_name, vno);

            testStatus=TSH_SUCCESS;
            break;
            }

        case KEY_MGMT_GET_KEY:
            {
            sec_key_mgmt_get_key(auth_service, arg, 
                            principal_name , vno,
                            &keyptr, &funcStatus);
     		dce_error_inq_text(funcStatus, error_buf, &status);
             if ( funcStatus == error_status_ok )
                TSHMessage (ctx, MOTRACE,
                   "sec_key_mgmt_get_key key type is %s. pepper= %u status: %s\n", 
                   (keyptr->key.key_type == sec_passwd_des) ? "des":"plain", 
                   keyptr->pepper, error_buf);
            else
                TSHMessage (ctx, MOTRACE,
                   "sec_key_mgmt_get_key status: %s\n", error_buf);


            if ( TSHStatus (ctx, TEST,
                          "sec_key_mgmt_get_key failed\n", estatus, funcStatus) )
                break;


            testStatus=TSH_SUCCESS;
            break;
            }

        case KEY_MGMT_GET_NTH_KEY:
            {
            index=vno;  /* index 0 means the first key, which is version 1 */
            sec_key_mgmt_get_nth_key(auth_service, arg, 
                            principal_name, index, &vno,
                            &keyptr, &funcStatus);
     		dce_error_inq_text(funcStatus, error_buf, &status);
             if ( funcStatus == error_status_ok )
                TSHMessage (ctx, MOTRACE,
                   "sec_key_mgmt_get_nth_key n=%d, vno=%d, key type is %s. pepper= %u status: %s\n",
                   index,vno,
                   (keyptr->key.key_type == sec_passwd_des) ? "des":"plain", 
                   keyptr->pepper, error_buf);
            else
                TSHMessage (ctx, 
                            MOTRACE,"sec_key_mgmt_get_nth_key vno=%d. status: %s\n", 
                            vno,error_buf);


            if ( TSHStatus (ctx, TEST, "sec_key_mgmt_get_nth_key failed\n", estatus, funcStatus) )
               break;

            testStatus=TSH_SUCCESS;
            break;
            }

        case KEY_MGMT_DELETE_KEY:
            {
            sec_key_mgmt_delete_key(auth_service, arg, 
                            principal_name , vno, &funcStatus);
     		dce_error_inq_text(funcStatus, error_buf, &status);
            TSHMessage (ctx, MOTRACE,
               "sec_key_mgmt_delete_key key status: %s\n", error_buf);

            if ( TSHStatus (ctx, TEST,
                            "sec_key_mgmt_delete_key failed\n", estatus, funcStatus) )
                break;

            testStatus=TSH_SUCCESS;
            break;
            }

        case KEY_MGMT_DELETE_KEY_TYPE:
            {    
			if (estatus == sec_key_mgmt_e_key_unsupported)
				keytype = sec_passwd_plain;

            sec_key_mgmt_delete_key_type(auth_service, arg, 
                            principal_name, &keytype, vno, &funcStatus);
     		dce_error_inq_text(funcStatus, error_buf, &status);
            TSHMessage (ctx, MOTRACE,
               "sec_key_mgmt_delete_key_type key status: %d, %s\n", funcStatus, error_buf);

            if ( TSHStatus (ctx, TEST,
                            "sec_key_mgmt_delete_key_type failed\n", estatus, funcStatus) )
                break;
                           
            testStatus=TSH_SUCCESS;
            break;
            }

        case KEY_MGMT_FREE_KEY:
            {

/*
** First allocate local storage by doing a get_key
*/
            sec_key_mgmt_get_key(auth_service, arg, 
                            principal_name , vno,
                            &keyptr, &funcStatus);
     		dce_error_inq_text(funcStatus, error_buf, &status);
             if ( funcStatus == error_status_ok )
                TSHMessage (ctx, MOTRACE,
                   "sec_key_mgmt_get_key key type is %s. pepper= %u status: %s\n", 
                   (keyptr->key.key_type == sec_passwd_des) ? "des":"plain", 
                   keyptr->pepper, error_buf);
            else
                TSHMessage (ctx, MOTRACE,
                   "sec_key_mgmt_get_key status: %s\n",error_buf);


/*
** Now free it
*/

            sec_key_mgmt_free_key(keyptr,&funcStatus);
     		dce_error_inq_text(funcStatus, error_buf, &status);

            TSHMessage (ctx, MOTRACE,
                        "sec_key_mgmt_free_key status: %s\n", error_buf);

            if ( TSHStatus (ctx, TEST,
                            "sec_key_mgmt_free_key failed\n", estatus, funcStatus) )
                break;

            testStatus=TSH_SUCCESS;
            break;
            }

        case KEY_MGMT_CHANGE_KEY:
            { 
			sec_key_mgmt_get_next_kvno(auth_service, arg, principal_name, &keytype,
				&old_vno, &next_vno, &funcStatus); /* get from secd */
     		dce_error_inq_text(funcStatus, error_buf, &status);
           	TSHMessage (ctx, MOTRACE,
                       "sec_key_mgmt_get_next_kvno status: %s, cur_vno: %d, next_vno: %d\n",  
                        error_buf, old_vno, next_vno );
  
			if (estatus == sec_key_mgmt_e_key_unavailable) { 
				/* remove the current key in key table which is the same as in rgy database */ 

            	if ( TSHStatus (ctx, SETUP,
                            "sec_key_mgmt_get_next_kvno  failed\n", error_status_ok, funcStatus) )
          			return;
 
               sec_key_mgmt_delete_key(auth_service, arg, principal_name,
                                   old_vno, &funcStatus);  /* delete from local data */
     			dce_error_inq_text(funcStatus, error_buf, &status);
            	TSHMessage (ctx, MOTRACE,
                        "sec_key_mgmt_delete_key status: %s, for '%s', vno: %d\n",  
                        error_buf, principal_name, old_vno );

            	if ( TSHStatus (ctx, SETUP,
                            "sec_key_mgmt_delete_key   failed\n", error_status_ok, funcStatus) )
          			return; 
			}

            sec_key_mgmt_change_key(auth_service,
                                    arg,
                                    principal_name,
                                    vno,
                                    &key,
                                    &garbage_collect_time,
                                    &funcStatus);
     		dce_error_inq_text(funcStatus, error_buf, &status);
            TSHMessage (ctx, MOTRACE,
                        "sec_key_mgmt_change_key garbage collect_time= %d,  status: %s , for '%s', vno: %d\n",
                        garbage_collect_time,error_buf, principal_name, vno);

            if ( TSHStatus (ctx, TEST,
                            "sec_key_mgmt_change_key failed\n", estatus, funcStatus) )
                break; 

			sec_key_mgmt_get_next_kvno(auth_service, arg, principal_name, &keytype,
				&old_vno, &next_vno, &funcStatus); /* get from secd */
     		dce_error_inq_text(funcStatus, error_buf, &status);
           	TSHMessage (ctx, MOTRACE,
                       "sec_key_mgmt_get_next_kvno status: %s, cur_vno: %d, next_vno: %d\n",  
                        error_buf, old_vno, next_vno );

                         
			if (estatus == sec_key_mgmt_e_key_unavailable) {
            	sec_key_mgmt_set_key(auth_service, arg, principal_name,
                                    old_vno, &key, &funcStatus);
     			dce_error_inq_text(funcStatus, error_buf, &status);  
            	TSHMessage (ctx, MOTRACE,
                        "sec_key_mgmt_set_key status: %s\n", error_buf);

            	if (TSHStatus (ctx, CLNUP,
                           "sec_key_mgmt_set_key failed\n", error_status_ok, funcStatus) )
                   return;  
			}

            testStatus=TSH_SUCCESS;
            break;
            }

		case KEY_MGMT_GEN_RAND_KEY:
            { 
            /* Log this client in. */
            
               sec_login_setup_identity (principal_name,
                                         (sec_login_flags_t)0,
                                         &auth_identity,
                                         &funcStatus);
      			dce_error_inq_text(funcStatus, error_buf, &status);           
               TSHMessage (ctx, MOTRACE,
            	       "sec_login_setup_identity status: %s\n", error_buf);
            
               if (TSHStatus (ctx, SETUP,
                              "sec_login_setup_identity failed\n", rpc_s_ok, funcStatus))
                 return (funcStatus);
                        
            /*    key.key.key_type = sec_passwd_plain;  
                key.key.tagged_union.plain = key;  */
                key.pepper = NULL;
                key.version_number = sec_passwd_c_version_none;
            
               (void) sec_login_validate_identity (auth_identity,
                                                   &key,
                                                   &reset_passwd,
                                                   &auth_src,
                                                   &funcStatus);
     			dce_error_inq_text(funcStatus, error_buf, &status);
               TSHMessage (ctx, MOTRACE,
            	       "sec_login_validate_identity status: %s\n", error_buf);
            
               TSHStatus (ctx, SETUP,
                              "sec_login_validate_identity failed\n", rpc_s_ok, funcStatus);
            
               sec_login_set_context (auth_identity, &funcStatus);
     			dce_error_inq_text(funcStatus, error_buf, &status);            
               TSHMessage (ctx, MOTRACE,
            	       "sec_login_set_context status: %s\n", error_buf);
            
               TSHStatus (ctx, SETUP,
                  "sec_login_set_context failed\n", rpc_s_ok, funcStatus);
 		/* This is the setup for no_seed_key.  Should fall through to do 
			the real test */
            }
			
		case KEY_MGMT_GEN_RAND_NO_SEED_KEY:
            {
            sec_key_mgmt_gen_rand_key(auth_service,
                                    arg,
                                    principal_name,
									&keytype,
                                    vno,
                                    &key_p,
                                    &funcStatus);
     		dce_error_inq_text(funcStatus, error_buf, &status);
            TSHMessage (ctx, MOTRACE,
                        "sec_key_mgmt_gen_rand_key status: %s\n", error_buf);

            if ( TSHStatus (ctx, TEST,
                            "sec_key_mgmt_gen_rand_key failed\n", estatus, funcStatus) )
                break;

            testStatus=TSH_SUCCESS;
            break;
            }

        case KEY_MGMT_MANAGE_KEY:
            {
            sec_key_mgmt_manage_key(auth_service,
                                    arg,
                                    principal_name,
                                    &funcStatus);
     		dce_error_inq_text(funcStatus, error_buf, &status);
            TSHMessage (ctx, MOTRACE,
                        "sec_key_mgmt_manage_key status: %d, %s\n", funcStatus, error_buf);

            if ( TSHStatus (ctx, TEST,
                            "sec_key_mgmt_manage_key failed\n", estatus, funcStatus) )
            break;

            testStatus=TSH_SUCCESS;
            break;
            }

        case KEY_MGMT_GARBAGE_COLLECT:
            {
            sec_key_mgmt_garbage_collect(auth_service,
                                    arg,
                                    principal_name,
                                    &funcStatus);
     		dce_error_inq_text(funcStatus, error_buf, &status);
            TSHMessage (ctx, MOTRACE,
                        "sec_key_mgmt_garbage_collect status: %s\n", error_buf);

            if ( TSHStatus (ctx, TEST,
                            "sec_key_mgmt_garbage_collect failed\n", estatus, funcStatus) )
                break;

            testStatus=TSH_SUCCESS;
            break;
            }

        case KEY_MGMT_INITIALIZE_CURSOR:
            {
            sec_key_mgmt_initialize_cursor(auth_service,
                                    arg,
                                    principal_name,
                                    &keytype,
                                    &cursor,
                                    &funcStatus);
     		dce_error_inq_text(funcStatus, error_buf, &status);
            TSHMessage (ctx, MOTRACE,
                        "sec_key_mgmt_initialize_cursor status: %d, %s\n", funcStatus, error_buf);

            if (TSHStatus (ctx, TEST,
                "sec_key_mgmt_initialize_cursor failed\n", estatus, funcStatus))  
				testStatus = TSH_TEST_FAILED; 
                
			if (funcStatus == error_status_ok) {
                sec_key_mgmt_release_cursor(&cursor, &clnupStatus);
         		dce_error_inq_text(clnupStatus, error_buf, &status);
                TSHMessage (ctx, MOTRACE,
                            "sec_key_mgmt_release_cursor status: %d, %s\n", funcStatus, error_buf);
    
                if (! TSHStatus (ctx, CLNUP,
                              "sec_key_mgmt_release_cursor failed", error_status_ok, clnupStatus) ) 
    				clnupStatus = error_status_ok;
     			else
    				testStatus = TSH_TEST_FAILED;  
			} 

            break;
            }


        case KEY_MGMT_GET_NEXT_KEY:
            {
            sec_key_mgmt_initialize_cursor(auth_service,
                                    arg,
                                    principal_name,
                                    &keytype,
                                    &cursor,
                                    &funcStatus);
     		dce_error_inq_text(funcStatus, error_buf, &status);
            TSHMessage (ctx, MOTRACE,
                        "sec_key_mgmt_initialize_cursor status: %s\n", error_buf);

            if (! TSHStatus (ctx, SETUP,
                            "sec_key_mgmt_initialize_cursor failed\n", error_status_ok, funcStatus) )	
   				testStatus = TSH_SUCCESS; 
			else
				testStatus = TSH_TEST_FAILED;                               

            sec_key_mgmt_get_next_key(cursor, &principal_name, &vno,
                                &keyptr, &funcStatus);

     		dce_error_inq_text(funcStatus, error_buf, &status); 
			if (funcStatus == error_status_ok)
            	TSHMessage (ctx, MOTRACE,
                       "sec_key_mgmt_get_next_key principle is %s, key vn is %d, key type is %s. pepper= %u status: %s\n", 
                        principal_name,
                        vno,
                       (keyptr->key.key_type == sec_passwd_des) ? "des":"plain", 
                       keyptr->pepper, error_buf);  
			else
            	TSHMessage (ctx, MOTRACE,
                       "sec_key_mgmt_get_next_key status: %s\n", error_buf);
                     
            if (! TSHStatus (ctx, SETUP,
                            "sec_key_mgmt_get_next_key failed\n", estatus, funcStatus) )	
   				testStatus = TSH_SUCCESS; 
			else
				testStatus = TSH_TEST_FAILED;                               

			/* repeat the operation, get_next shoudl get the same */

            sec_key_mgmt_initialize_cursor(auth_service,
                                    arg,
                                    principal_name,
                                    &keytype,
                                    &cursor,
                                    &funcStatus);
     		dce_error_inq_text(funcStatus, error_buf, &status);
            TSHMessage (ctx, MOTRACE,
                        "sec_key_mgmt_initialize_cursor status: %s\n", error_buf);

            if (! TSHStatus (ctx, SETUP,
                            "sec_key_mgmt_initialize_cursor failed\n", error_status_ok, funcStatus) )
   				testStatus = TSH_SUCCESS; 
 			else
				testStatus = TSH_TEST_FAILED;                               

             sec_key_mgmt_get_next_key(cursor, &n_principal_name, &n_vno,
                                &n_keyptr, &funcStatus);

     		 dce_error_inq_text(funcStatus, error_buf, &status);
			 if (funcStatus == error_status_ok)
             	TSHMessage (ctx, MOTRACE,
                       "sec_key_mgmt_get_next_key principle is %s, key vn is %d, key type is %s. pepper= %u status: %s\n", 
                        principal_name,
                        vno,
                       (keyptr->key.key_type == sec_passwd_des) ? "des":"plain", 
                       keyptr->pepper, error_buf);  
			else
            	TSHMessage (ctx, MOTRACE,
                       "sec_key_mgmt_get_next_key status: %s\n", error_buf);

            if (! TSHStatus (ctx, SETUP,
                            "sec_key_mgmt_get_next_key failed\n", estatus, funcStatus) )	
				testStatus = TSH_SUCCESS;
			else {
				testStatus = TSH_TEST_FAILED; 
			}                             
                        
			if (funcStatus != error_status_ok) {
                sec_key_mgmt_release_cursor(&cursor, &clnupStatus);
         		dce_error_inq_text(clnupStatus, error_buf, &status);
                TSHMessage (ctx, MOTRACE,
                            "sec_key_mgmt_release_cursor status: %s\n", error_buf);
    
                if (!TSHStatus (ctx, CLNUP,
                                "sec_key_mgmt_release_cursor failed\n", error_status_ok, clnupStatus) )
     				clnupStatus = error_status_ok; 
    			else
    				testStatus = TSH_TEST_FAILED; 
			}			                              

            break;
		}

	}

   return testStatus;
   
}

