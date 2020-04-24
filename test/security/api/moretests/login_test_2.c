/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: login_test_2.c,v $
 * Revision 1.1.4.2  1996/02/17  23:25:29  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:39  marty]
 *
 * Revision 1.1.4.1  1995/12/11  21:48:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:27  root]
 * 
 * Revision 1.1.2.2  1992/12/31  17:29:37  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:55:11  htf]
 * 
 * Revision 1.1  1992/01/19  04:07:19  devrcs
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
                     
    TSHStatus (lctx, TEST, "sec_login_setup_identity", *st, 
                     (est == DONT_CARE) ? *st:est);
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


    return TSHStatus (lctx, TEST, "sec_login_validate_identity", *st, 
                            (est == DONT_CARE) ? *st:est);
    }

/****************************************************************
**
*/
boolean32 DoLogin
#ifdef __STDC__
(
sec_login_handle_t *context,
unsigned_char_p_t  acctName,
unsigned_char_p_t  passwd,
ndr_long_int       flags,
boolean32          *resetLogin,
sec_login_auth_src_t *authSrc,
error_status_t     est,
error_status_t     *st
)
#else
(context,  acctName, passwd, flags, resetLogin, authSrc, est, st )
sec_login_handle_t *context;
unsigned_char_p_t  acctName;
unsigned_char_p_t  passwd;
ndr_long_int       flags;
boolean32          *resetLogin;
sec_login_auth_src_t *authSrc;
error_status_t     est;
error_status_t     *st;
#endif

    {

    boolean32 lst;
    sec_passwd_rec_t prec;

    prec.key.key_type = sec_passwd_plain;  
    prec.key.tagged_union.plain =passwd;
    prec.prefix = NULL;
    prec.version_number = sec_passwd_c_version_none;

    CALL_LOGIN(sec_login_setup_identity)(acctName, flags, context, st);

    TSHMessage (lctx, MOTRACE, "sec_login_setup_identity (%s %d.. ), status; %d\n", 
                acctName, flags, *st);
                     
    if ( lst=TSHStatus (lctx, TEST, "sec_login_setup_identity", *st, sec_login_s_ok) )
        {
 	    CALL_LOGIN(sec_login_purge_context)(context, st);
        TSHMessage (lctx, MOTRACE, "sec_login_purge_context ( .. ), status; %d\n",*st);
        lst=TSHStatus (lctx, TEST, "sec_login_purge_context", *st, 
                         (est == DONT_CARE) ? *st:est);
        }

    if ( passwd != NULL && !lst )
        {
 	    CALL_LOGIN(sec_login_validate_identity)(*context, &prec, resetLogin, authSrc, st);
        TSHMessage (lctx, MOTRACE, "sec_login_validate_identity (%s .. ), status; %d\n", 
                  passwd, *st);
        TSHMessage (lctx, MOTRACE, "Reset Login flag is %s, AuthSrc is %s \n", 
                  (*resetLogin) ? "true":"false",
                  (*authSrc == sec_login_auth_src_local) ? "Local":"Network" );
        lst=TSHStatus (lctx, TEST, "sec_login_validate_identity", *st, 
                             (est == DONT_CARE) ? *st:est);

        if (st == sec_login_s_ok)
            {
            CALL_LOGIN(sec_login_set_context)(*context, st);
            TSHMessage (lctx, MOTRACE, "sec_login_set_context ( .. ), status; %d\n",*st);

            return TSHStatus (lctx, TEST, "sec_login_set_context", *st, 
                               (est == DONT_CARE) ? *st:est);
            }


        }
         
    return lst;
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

    return TSHStatus (lctx, TEST, "sec_login_purge_context", *st, 
                     (est == DONT_CARE) ? *st:est);

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
   error_status_t    lstatus;	    /* local stati */
   unsigned_char_p_t acctName;
   unsigned_char_p_t  passwd;
   ndr_long_int	     flags;
   boolean32		 passwdExpired;
   signed32          expirationTime;
   sec_login_auth_src_t authSrc;
   sec_login_handle_t login_context;
   sec_login_handle_t extra_context;
   unsigned_char_p_t  pwent;
   idl_byte *xbuffer;
   unsigned32 xbufferLen,bufUsed,bufNeeded;
   unsigned32 numGroups;
   signed32 *groupList;
   sec_id_t *newGroups;
   unsigned32 newGroupCount;

   unsigned32  i;


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
        case LOGIN_CHECK_PASSWORD:
            {
			if ( (tstatus=CheckArgs(ctx, argc, 4)) )
				break;
			acctName=argv[2];
            passwd =argv[3];

            CALL_LOGIN(sec_login_check_passwd)( acctName, passwd, &authSrc, &tstatus);
            TSHMessage (lctx, MOTRACE, "sec_login_check_password (%s, %s .. ) status; %d\n", 
                    acctName, passwd, tstatus);
            TSHMessage (lctx, MOTRACE, "AuthSrc is %s \n", 
                  (authSrc == sec_login_auth_src_local) ? "Local":"Network" );
            TSHStatus (lctx, TEST, "sec_login_check_password", tstatus, estatus);
            break;
            }
           

		case LOGIN_GET_EXPIRATION:
			{
			if ( (tstatus=CheckArgs(ctx, argc, 4)) )
				break;

			acctName=argv[2];
            passwd =argv[3];

            DoLogin(&login_context, acctName, passwd, sec_login_no_flags, &passwdExpired, &authSrc, 
                                    DONT_CARE, &tstatus);

            CALL_LOGIN(sec_login_get_expiration)(login_context, &expirationTime, &tstatus);

            TSHMessage (lctx, MOTRACE, "sec_login_get_expiration ( ..%s ), status; %d\n",
                              ctime(&expirationTime),tstatus);
            TSHStatus (lctx, TEST, "sec_login_get_expiration", tstatus, estatus);

            DoPurgeContext(&login_context, DONT_CARE, &lstatus);
			break;
			}


		case LOGIN_GET_PWENT:
			{
			if ( (tstatus=CheckArgs(ctx, argc, 4)) )
				break;

			acctName=argv[2];
            passwd =argv[3];


            DoLogin(&login_context, acctName, passwd, sec_login_no_flags, &passwdExpired, &authSrc, 
                                    DONT_CARE, &tstatus);

            CALL_LOGIN(sec_login_get_pwent)(login_context, &pwent, &tstatus);

            if ( strlen(pwent) == 0 )
                {
                TSHMessage (ctx, MOFAILURE, "Empty passwd string\n");
				tstatus=TSH_TEST_FAILED;
                break;
                }


            TSHMessage (lctx, MOTRACE, "sec_login_get_pwent ( ..%s ), status; %d\n",pwent,tstatus);
            TSHStatus (lctx, TEST, "sec_login_get_pwent", tstatus, estatus);

            DoPurgeContext(&login_context, DONT_CARE, &lstatus);
			break;
			}


		case LOGIN_EXPORT_IMPORT_CONTEXT:
			{
			if ( (tstatus=CheckArgs(ctx, argc, 5)) )
				break;

			acctName=argv[2];
            passwd =argv[3];
			xbufferLen=atol(argv[4]);

            if ( (xbuffer= (idl_byte *) malloc(xbufferLen)) == NULL )
                {
                TSHMessage (ctx, MOFAILURE, "Malloc failed\n");
				tstatus=TSH_TEST_FAILED;
                break;
                }



            DoLogin(&login_context, acctName, passwd, sec_login_no_flags, &passwdExpired, &authSrc, 
                        DONT_CARE, &tstatus);

            CALL_LOGIN(sec_login_export_context)(login_context,xbufferLen, xbuffer, 
                       &bufUsed, &bufNeeded, &tstatus);

            TSHMessage (lctx, MOTRACE, "sec_login_export_context (buffer len:%d,..used:%d, needed:%d), status; %d\n",
                              xbufferLen,bufUsed,bufNeeded,tstatus);
            TSHStatus (lctx, TEST, "sec_login_export_context", tstatus, estatus);


            if ( tstatus == estatus )
                {
                CALL_LOGIN(sec_login_import_context)(bufUsed, xbuffer, &extra_context, &tstatus);
     
                TSHMessage (lctx, MOTRACE, "sec_login_import_context (buffer len:%d,..), status; %d\n",
                                  bufUsed,tstatus);
                if ( !TSHStatus (lctx, TEST, "sec_login_import_context", tstatus, sec_login_s_ok) )
                    {
   	                CALL_LOGIN(sec_login_set_context)(login_context, &tstatus);
                    TSHMessage (lctx, MOTRACE, "sec_login_set_context ( .. ), status; %d\n",tstatus);

                    TSHStatus (lctx, TEST, "sec_login_set_context", tstatus, sec_login_s_ok);
                    }
                }

            DoPurgeContext(&login_context, DONT_CARE, &lstatus);

            free(xbuffer);
			break;
			}


		case LOGIN_GET_GROUPS:				/* uses the same code */
            {
			if ( (tstatus=CheckArgs(ctx, argc, 4)) )
				break;

			acctName=argv[2];
			passwd=argv[3];
/*            maxGroups=atol(argv[4]);

            if ( (groupList= (signed32 *) malloc(maxGroups*sizeof(signed32))) == NULL )
                {
                TSHMessage (ctx, MOFAILURE, "Malloc failed\n");
				tstatus=TSH_TEST_FAILED;
                break;
                }

            numGroups=MaxGroups;

*/
            numGroups=0;
            groupList=NULL;

            DoLogin(&login_context, acctName, passwd, sec_login_no_flags, &passwdExpired, &authSrc, 
                                    DONT_CARE, &tstatus);

            CALL_LOGIN(sec_login_get_groups)(login_context, &numGroups, &groupList, &tstatus);

            TSHMessage (lctx, MOTRACE, "sec_login_get_groups ( # groups: %d, ), status; %d\n",
                        numGroups,tstatus);


            TSHStatus (lctx, TEST, "sec_login_get_groups", tstatus, estatus);

            TSHMessage (lctx, MOTRACE, "Group List Element          Group ID\n");
            for (i=0; i < numGroups; i++)
               TSHMessage (lctx, MOTRACE, "%d                          %d\n", i,groupList[i]);


            DoPurgeContext(&login_context, DONT_CARE, &lstatus);
			break;
			}


		case LOGIN_NEWGROUPS:
			{
        	if ( argc < 6 )
 		        {
		        TSHMessage(ctx, MOFAILURE, "Expecting at least 6 arguments, got %d\n", argc);
			    return(TSH_BAD_ARG_COUNT);
                }

			acctName=argv[2];
			flags=atol(argv[3]);
			passwd=(strcmp(argv[4],"NO_PASSWORD")) ? argv[4]:NULL;
            newGroupCount=argc-5;
            login_context=extra_context=NULL;
 
            if ( (newGroups= (sec_id_t *) malloc( newGroupCount*sizeof(sec_id_t) )) == NULL )
                {
                TSHMessage (ctx, MOFAILURE, "Malloc failed\n");
				tstatus=TSH_TEST_FAILED;
                break;
                }

            for (i=0; i < newGroupCount; i++)
                newGroups[i].name=argv[i+5];



            DoLogin(&login_context, acctName, passwd, sec_login_no_flags, &passwdExpired, &authSrc, 
                                    DONT_CARE, &tstatus);

            CALL_LOGIN(sec_login_newgroups)(login_context,flags, newGroupCount, newGroups, &extra_context, &tstatus);

            TSHMessage (lctx, MOTRACE, "sec_login_newgroups (%d, %d, ... ), status; %d\n",
                        flags,newGroupCount,tstatus);

            TSHStatus (lctx, TEST, "sec_login_newgroups", tstatus, estatus);

            if (tstatus == sec_login_s_ok)
                {
                CALL_LOGIN(sec_login_set_context)(extra_context, &tstatus);
                TSHMessage (lctx, MOTRACE, "sec_login_set_context ( .. ), status; %d\n",tstatus);
                TSHStatus (lctx, TEST, "sec_login_set_context", tstatus, sec_login_s_ok);
            }

            if ( login_context != NULL )
                DoPurgeContext(&login_context, DONT_CARE, &lstatus);
            if ( extra_context != NULL )
                DoPurgeContext(&extra_context, DONT_CARE, &lstatus);
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


			



