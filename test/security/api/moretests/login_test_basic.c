/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: login_test_basic.c,v $
 * Revision 1.1.4.2  1996/02/17  23:25:31  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:40  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:48:53  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:28  root]
 * 
 * Revision 1.1.2.2  1992/12/31  17:29:42  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:55:16  htf]
 * 
 * Revision 1.1  1992/01/19  04:07:21  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/****************************************************************
 *
 * sec_login setup Tests
 *
****************************************************************/
/*
*/
/*
 * 
 * Copyright 1989 by Apollo Computer, Inc.
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
 */


#include "login_test.h"

CtxBlock          *lctx;

 /****************************************************************
**
** Function int CheckArgs(int argc, int args_required)
**
** Compares argc to args_required and issues a TSH error if not equal.
**
** Returns 0 if ok, TSH_BAD_ARG_COUNT otherwise
**
*/
int CheckArgs
#ifdef __STDC__
(
CtxBlock          *ctx,
ndr_long_int      argc,
ndr_long_int      args_required
)
#else
(ctx, argc, args_required) 
CtxBlock          *ctx;
ndr_long_int      argc;
ndr_long_int      args_required;
#endif
	{
	if ( argc != args_required)
		{
		TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
						args_required, argc);
			return(TSH_BAD_ARG_COUNT);
		}
	return 0;
	}



/****************************************************************
**
*/

void DoSetupIdentity
#ifdef __STDC__
(
unsigned_char_p_t  acctName,
ndr_long_int       flags,
error_status_t     est,
sec_login_handle_t *context,
error_status_t     *st
)
#else
(acctName, flags, est, context, st)
unsigned_char_p_t  acctName;
ndr_long_int       flags;
error_status_t     est;
sec_login_handle_t *context;
error_status_t     *st;
#endif
    {
    CALL_LOGIN(sec_login_setup_identity)(acctName, flags, context, st);

    TSHMessage (lctx, MOTRACE, "sec_login_setup_identity (%s %d.. ), status; %d\n", 
                acctName, flags, *st);
                     
    TSHStatus (lctx, TEST, "sec_login_setup_identity", *st, est);
    }

/****************************************************************
**
*/
boolean32 DoValidateIdentity
#ifdef __STDC__
(
sec_login_handle_t context,
sec_passwd_rec_t  *passwd,
error_status_t     est,
boolean32          *resetLogin,
sec_login_auth_src_t *authSrc,
error_status_t     *st
)
#else
(context, passwd, est, resetLogin, authSrc, st )
sec_login_handle_t context;
sec_passwd_rec_t  *passwd;
error_status_t     est;
boolean32          *resetLogin;
sec_login_auth_src_t *authSrc;
error_status_t     *st;
#endif
    {
 	CALL_LOGIN(sec_login_validate_identity)(context, passwd, resetLogin, authSrc, st);
    TSHMessage (lctx, MOTRACE, "sec_login_validate_identity (%s .. ), status; %d\n", 
              passwd, *st);
    TSHMessage (lctx, MOTRACE, "Reset Login flag is %s, AuthSrc is %s \n", 
              (*resetLogin) ? "true":"false",
              (*authSrc == sec_login_auth_src_local) ? "Local":"Network" );


    return TSHStatus (lctx, TEST, "sec_login_validate_identity", *st, est);
    }

/****************************************************************
**
*/
boolean32 DoPurgeContext
#ifdef __STDC__
(
sec_login_handle_t *context,
error_status_t     est,
error_status_t     *st
)
#else
(context,  est, st )
sec_login_handle_t *context;
error_status_t     est;
error_status_t     *st;
#endif
    {
 	CALL_LOGIN(sec_login_purge_context)(context, st);
    TSHMessage (lctx, MOTRACE, "sec_login_purge_context ( .. ), status; %d\n",*st);

    return TSHStatus (lctx, TEST, "sec_login_purge_context", *st, est);

    }

/****************************************************************
**
*/
boolean32 DoReleaseContext
#ifdef __STDC__
(
sec_login_handle_t *context,
error_status_t     est,
error_status_t     *st
)
#else
(context,  est, st )
sec_login_handle_t *context;
error_status_t     est;
error_status_t     *st;
#endif
    {
 	CALL_LOGIN(sec_login_release_context)(context, st);
    TSHMessage (lctx, MOTRACE, "sec_login_release_context ( .. ), status; %d\n",*st);

    return TSHStatus (lctx, TEST, "sec_login_release_context", *st, est);
    }

/****************************************************************
**
*/
boolean32 DoSetContext
#ifdef __STDC__
(
sec_login_handle_t context,
error_status_t     est,
error_status_t     *st
)
#else
(context,  est, st )
sec_login_handle_t context;
error_status_t     est;
error_status_t     *st;
#endif
    {
 	CALL_LOGIN(sec_login_set_context)(context, st);
    TSHMessage (lctx, MOTRACE, "sec_login_set_context ( .. ), status; %d\n",*st);

    return TSHStatus (lctx, TEST, "sec_login_set_context", *st, est);
    }


/****************************************************************
**
*/
boolean32 DoRefreshIdentity
#ifdef __STDC__
(
sec_login_handle_t context,
error_status_t     est,
error_status_t     *st
)
#else
(context,  est, st )
sec_login_handle_t context;
error_status_t     est;
error_status_t     *st;
#endif
    {
 	CALL_LOGIN(sec_login_refresh_identity)(context, st);
    TSHMessage (lctx, MOTRACE, "sec_login_refresh_identity ( .. ), status; %d\n",*st);

    return TSHStatus (lctx, TEST, "sec_login_refresh_identity", *st, est);
    }

/****************************************************************
**
*/
boolean32 DoGetCurrentContext
#ifdef __STDC__
(
sec_login_handle_t *context,
error_status_t     est,
error_status_t     *st
)
#else
(context,  est, st )
sec_login_handle_t *context;
error_status_t     est;
error_status_t     *st;
#endif
    {
 	CALL_LOGIN(sec_login_get_current_context)(context, st);
    TSHMessage (lctx, MOTRACE, "sec_login_get_current_context ( .. ), status; %d\n",*st);

    return TSHStatus (lctx, TEST, "sec_login_get_current_context", *st, est);
    }
 

/****************************************************************
** 
**	Function: ndr_long_int TestFunction(ctx, argc, argv)
** 
** This is the Test entry point. Subcase drives what is to be done.
** 
** 
** 
*/
ndr_long_int TestFunction
/* Formal parameters */
#ifdef __STDC__
(
CtxBlock          *ctx,
ndr_long_int      argc,
unsigned_char_p_t argv[]
)
#else
(ctx, argc, argv) 
CtxBlock          *ctx;
ndr_long_int      argc;
char              *argv[];
#endif
{

/* Local variables */
   ndr_long_int      subcase;		/* argv[0], subcase to run */
   error_status_t    estatus;		/* argv[1], expected return from test */
   error_status_t    tstatus;	    /* actual value returned from rpc */
   unsigned_char_p_t  acctName;
   sec_passwd_rec_t  prec;
   ndr_long_int	      flags;
   boolean32		  passwdExpired;
   sec_login_auth_src_t authSrc;
   sec_login_handle_t login_context;
   sec_login_handle_t extra_context;

   lctx=ctx;

/*
** Bug in CMA requires initialization
*/
   cma_init();

/* Obtain the minimum input arguments from the .tsh command line. */

   subcase = atol(argv[0]);
   estatus = atol(argv[1]); 

   tstatus = sec_login_s_ok;         /* initial */


	switch (subcase)
		{
		case LOGIN_SETUP_IDENTITY:
			{
			if ( (tstatus=CheckArgs(ctx, argc, 4)) )
				break;

			acctName=argv[2];
			flags=atol(argv[3]);

            DoSetupIdentity(acctName, flags, estatus, &login_context, &tstatus);
            if ( tstatus != sec_login_s_ok )
                break;
            DoPurgeContext(&login_context, sec_login_s_ok, &tstatus);
			break;
			}


		case LOGIN_PURGE_CONTEXT:				/* uses the same code */
            {
			if ( (tstatus=CheckArgs(ctx, argc, 4)) )
				break;

			acctName=argv[2];
			flags=atol(argv[3]);

    /* Do normal Setup and release */

            DoSetupIdentity(acctName, flags, estatus, &login_context, &tstatus);
            if ( tstatus != sec_login_s_ok )
                break;
            DoPurgeContext(&login_context, sec_login_s_ok, &tstatus);

    /* Now release again: Handle should be default (NULL) */
        
            DoPurgeContext(&login_context, sec_login_s_default_use, &tstatus);
            if ( tstatus != sec_login_s_default_use )
                break;

            tstatus=sec_login_s_ok;
            break;
            }

		case LOGIN_RELEASE_CONTEXT:
			{

			if ( (tstatus=CheckArgs(ctx, argc, 4)) )
				break;

			acctName=argv[2];
			flags=atol(argv[3]);

    /* Do normal Setup and release */

            DoSetupIdentity(acctName, flags, estatus, &login_context, &tstatus);
            if ( tstatus != sec_login_s_ok )
                break;
            DoReleaseContext(&login_context, sec_login_s_ok, &tstatus);

    /* Now release again: Handle should be default (NULL) */
        
            DoReleaseContext(&login_context, sec_login_s_default_use, &tstatus);
            if ( tstatus != sec_login_s_default_use )
                break;

            tstatus=sec_login_s_ok;
			break;
			}


		case LOGIN_VALIDATE_IDENTITY:
			{
			if ( (tstatus=CheckArgs(ctx, argc, 5)) )
				break;
			acctName=argv[2];
			flags=atol(argv[3]);
            prec.key.key_type = sec_passwd_plain;  
            prec.key.tagged_union.plain =argv[4];
            prec.prefix = NULL;
            prec.version_number = sec_passwd_c_version_none;

            DoSetupIdentity(acctName, flags, sec_login_s_ok, &login_context, &tstatus);
            if ( tstatus != sec_login_s_ok )
                break;

            if ( DoValidateIdentity(login_context, &prec, estatus, &passwdExpired, &authSrc, &tstatus ) )
/*            if (tstatus != estatus) */
                break;
            
            if ( login_context ) 
                DoPurgeContext(&login_context, sec_login_s_ok, &tstatus);

    /* The context is no longer valid. Make sure we gen an invalid handle error */

            if (DoValidateIdentity(login_context, &prec, sec_login_s_default_use, &passwdExpired, &authSrc, &tstatus ) )
                break;

            tstatus=sec_login_s_ok;
            break;
			}

		case LOGIN_SET_CONTEXT:
			{
			if ( (tstatus=CheckArgs(ctx, argc, 5)) )
				break;
			acctName=argv[2];
			flags=atol(argv[3]);
            prec.key.key_type = sec_passwd_plain;  
            prec.key.tagged_union.plain =argv[4];
            prec.prefix = NULL;
            prec.version_number = sec_passwd_c_version_none;


            DoSetContext(login_context, estatus, &tstatus);
            DoSetupIdentity(acctName, flags, sec_login_s_ok, &login_context, &tstatus);
            if ( tstatus != sec_login_s_ok )
                break;

            if ( DoValidateIdentity(login_context, &prec, sec_login_s_ok, 
                                     &passwdExpired, &authSrc, &tstatus ) )
                break;

            DoSetContext(login_context, sec_login_s_ok, &tstatus);

            if ( login_context ) 
                DoPurgeContext(&login_context, sec_login_s_ok, &tstatus);

			break;
			}

		case LOGIN_GET_CURRENT_CONTEXT:
			{
			if ( (tstatus=CheckArgs(ctx, argc, 5)) )
				break;
			acctName=argv[2];
			flags=atol(argv[3]);
            prec.key.key_type = sec_passwd_plain;  
            prec.key.tagged_union.plain =argv[4];
            prec.prefix = NULL;
            prec.version_number = sec_passwd_c_version_none;

/*          if ( DoGetCurrentContext(&login_context, estatus, &tstatus) )
              break;
*/

    /*  Now establish a context */

            DoSetupIdentity(acctName, flags, sec_login_s_ok, &login_context, &tstatus);
            if ( tstatus != sec_login_s_ok )
                break;

            if ( DoValidateIdentity(login_context, &prec, sec_login_s_ok, 
                                     &passwdExpired, &authSrc, &tstatus ) )
                break;
            
            if (authSrc == sec_login_auth_src_network)
                {
                if (DoSetContext(login_context, sec_login_s_ok, &tstatus) )
                    {
                    if ( login_context ) 
                        DoPurgeContext(&login_context, sec_login_s_ok, &tstatus);
                    break;
                    }
                }
                    
            DoGetCurrentContext(&login_context, sec_login_s_ok, &tstatus);

            if ( login_context ) 
                DoPurgeContext(&login_context, sec_login_s_ok, &tstatus);

			break;
			}
		case LOGIN_REFRESH_IDENTITY:
			{

			if ( (tstatus=CheckArgs(ctx, argc, 5)) )
				break;
			acctName=argv[2];
			flags=atol(argv[3]);
            prec.key.key_type = sec_passwd_plain;  
            prec.key.tagged_union.plain =argv[4];
            prec.prefix = NULL;
            prec.version_number = sec_passwd_c_version_none;


            DoSetupIdentity(acctName, flags, sec_login_s_ok, &login_context, &tstatus);
            if ( tstatus != sec_login_s_ok )
                break;

            DoValidateIdentity(login_context, &prec, sec_login_s_ok, 
                                     &passwdExpired, &authSrc, &tstatus );
            
            if (authSrc == sec_login_auth_src_network)
                {
                if (DoSetContext(login_context, sec_login_s_ok, &tstatus) )
                    {
                    if ( login_context ) 
                        DoPurgeContext(&login_context, sec_login_s_ok, &tstatus);
                    break;
                    }
                }
                    
            if (!DoRefreshIdentity(&login_context, estatus, &tstatus) )
                DoValidateIdentity(login_context, &prec, sec_login_s_ok, 
                                     &passwdExpired, &authSrc, &tstatus );


            if ( login_context ) 
                DoPurgeContext(&login_context, sec_login_s_ok, &tstatus);
			break;
			}
#ifdef IMPLEMENTED

#endif
		default:
			{
			tstatus=TSH_UNIMPLEMENTED;
         TSHMessage (ctx, MOFAILURE, "subcase %d, Status: %d\n",
							tstatus, subcase);

			break;
			}
		}
	return( tstatus == estatus ) ? sec_login_s_ok:tstatus;
	}


			



