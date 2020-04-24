/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: login_test.c,v $
 * Revision 1.2.8.2  1996/02/17  23:25:27  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:43:37  marty]
 *
 * Revision 1.2.8.1  1995/12/11  21:48:47  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  20:52:25  root]
 * 
 * Revision 1.2.6.1  1993/10/19  21:07:18  desai
 * 	CR 9145 fix. Defined invalid password as an array to fix 486 failure problem.
 * 	[1993/10/19  21:06:42  desai]
 * 
 * Revision 1.2.4.6  1993/02/25  22:06:40  cuti
 * 	Change sec_rgy_site_open() to sec_rgy_site_open_update() for using site update operation.
 * 	[1993/02/25  16:31:22  cuti]
 * 
 * Revision 1.2.4.5  1993/01/29  21:11:48  cuti
 * 	Add sec_login_valid_and_cert_ident() test
 * 	[1993/01/28  16:04:18  cuti]
 * 
 * Revision 1.2.4.4  1992/12/31  17:29:26  htf
 * 	Embedding copyright notice
 * 	[1992/12/31  16:55:01  htf]
 * 
 * Revision 1.2.4.3  1992/11/23  19:04:56  cuti
 * 	Modify one more TSHMessage output.
 * 	[1992/11/23  19:03:48  cuti]
 * 
 * Revision 1.2.4.2  1992/11/21  23:00:37  cuti
 * 	Modify some test stages in TSHStatus.
 * 	[1992/11/21  20:55:15  cuti]
 * 
 * 	Modify sec_login_newgroups test.  Also modify TSHMessage's output from number to text.
 * 	[1992/11/20  19:48:42  cuti]
 * 
 * Revision 1.2.2.2  1992/03/18  19:12:23  burati
 * 	2 tests for specific errors added to CERTIFY_IDENTITY test
 * 	[1992/03/18  18:41:01  burati]
 * 
 * Revision 1.2  1992/01/19  22:14:34  devrcs
 * 	Dropping DCE1.0 OSF1_misc port archive
 * 
 * $EndLog$
 */
/*
*/
/*
 *  OSF DCE Version 1.0 
*/
/****************************************************************
 *
 * sec_login setup Tests
 *
****************************************************************/
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

#include <dce/dce_error.h> 
#include <login_test.h>
#include <dce/pgo.h>
#include <tsh_common.h>

#define NEW_GROUP "non_exist"   

boolean32 contextValid;
dce_error_string_t	error_buf;
unsigned32			status; 


/****************************************************************
** 
** SetupIdentity( [out]sec_login_handle_t    *context,
**                 [in] unsigned_char_p_t     acctName,
**                 [in] ndr_long_int          flags,
**                 [in] sec_passwd_rec_t      *passwd,
**                 [in] ndr_long_int          errorCase,
**                 [out]boolean32             *resetLogin,
**                 [out]sec_login_auth_src_t  *authSrc )
**
** Does a setup_identity and a validate_identity. errorCase specs what
** errors are expected Returns non-zer if unexpected errors occur
**
**/
boolean32 SetupIdentity
#ifdef __STDC__
(
CtxBlock              *ctx,
sec_login_handle_t    *context,
unsigned_char_p_t     acctName,
ndr_long_int          flags,
sec_passwd_rec_t      *passwd,
ndr_long_int          errorCase,
boolean32             *resetLogin,
sec_login_auth_src_t  *authSrc
)
#else
( ctx,context,acctName,flags,passwd,errorCase,resetLogin,authSrc ) 
CtxBlock              *ctx;
sec_login_handle_t    *context;
unsigned_char_p_t     acctName;
ndr_long_int          flags;
sec_passwd_rec_t      *passwd;
ndr_long_int          errorCase;
boolean32             *resetLogin;
sec_login_auth_src_t  *authSrc;
#endif
    
    {
    error_status_t   funcStatus;

    sec_login_setup_identity(acctName, flags, context, &funcStatus);
    dce_error_inq_text(funcStatus, error_buf, &status);
    TSHMessage (ctx, MOTRACE, "sec_login_setup_identity (%s %d.. ), status; %s\n", 
                acctName, flags, error_buf);
    contextValid=(funcStatus == sec_login_s_ok) ? true:false;

    if (errorCase & BAD_OBJECT_FLAG)
          return 0;
                     
    if ( TSHStatus (ctx, SETUP, "sec_login_setup_identity", 
                      funcStatus, sec_login_s_ok ) )
      return TSH_TEST_FAILED;

    sec_login_validate_identity(*context, 
                                passwd, 
                                resetLogin, 
                                authSrc, 
                                &funcStatus );
    dce_error_inq_text(funcStatus, error_buf, &status);
    TSHMessage(ctx, MOTRACE, "sec_login_validate_identity (%s .. ), status; %s\n", 
              passwd, error_buf);

    if (!(errorCase & BAD_PASSWD_FLAG) &&
          TSHStatus (ctx, SETUP, "sec_login_validate_identity", funcStatus, 
          sec_login_s_ok) )
        return TSH_TEST_FAILED;

    return 0;

    }


/****************************************************************
** 
** SetupContext  ( [out]sec_login_handle_t    *context,
**                 [in] unsigned_char_p_t     acctName,
**                 [in] ndr_long_int          flags,
**                 [in] sec_passwd_rec_t      *passwd,
**                 [in] ndr_long_int          errorCase )
**
** Does a setup_identity,validate_identity, and set context if network is authsrc.
**  errorCase specs what errors are expected Returns non-zer0 if unexpected 
** errors occur
**
**/
boolean32 SetupContext
#ifdef __STDC__
(
CtxBlock              *ctx,
sec_login_handle_t    *context,
unsigned_char_p_t     acctName,
ndr_long_int          flags,
sec_passwd_rec_t      *passwd,
ndr_long_int          errorCase
)
#else
( ctx,context,acctName,flags,passwd,errorCase ) 
CtxBlock              *ctx;
sec_login_handle_t    *context;
unsigned_char_p_t     acctName;
ndr_long_int          flags;
sec_passwd_rec_t      *passwd;
ndr_long_int          errorCase;
#endif

    {
    boolean32             resetLogin;
    sec_login_auth_src_t  authSrc;
    error_status_t        funcStatus;

    if (!SetupIdentity(ctx, context, acctName, flags, passwd, errorCase, 
                  &resetLogin, &authSrc) &&
        authSrc == sec_login_auth_src_network  )
       {
       sec_login_set_context(*context, &funcStatus);
	   dce_error_inq_text(funcStatus, error_buf, &status);
       TSHMessage (ctx, MOTRACE, "sec_login_set_context ( .. ), status; %s\n",error_buf);
       return TSHStatus (ctx, SETUP, "sec_login_set_context",funcStatus, sec_login_s_ok ) ; 
       }
    }


/****************************************************************
** 
** CertifyContext  ( [out]sec_login_handle_t    *context,
**                 [in] unsigned_char_p_t     acctName,
**                 [in] ndr_long_int          flags,
**                 [in] sec_passwd_rec_t      *passwd,
**                 [in] ndr_long_int          errorCase )
**
** Does a setup_identity,validate_identity, and set context if network is authsrc.
**  errorCase specs what errors are expected Returns non-zer0 if unexpected 
** errors occur
**
**/
boolean32 CertifyContext
#ifdef __STDC__
(
CtxBlock              *ctx,
sec_login_handle_t    *context,
unsigned_char_p_t     acctName,
ndr_long_int          flags,
sec_passwd_rec_t      *passwd,
ndr_long_int          errorCase
)
#else
( ctx,context,acctName,flags,passwd,errorCase ) 
CtxBlock              *ctx;
sec_login_handle_t    *context;
unsigned_char_p_t     acctName;
ndr_long_int          flags;
sec_passwd_rec_t      *passwd;
ndr_long_int          errorCase;
#endif

    {
    boolean32             resetLogin;
    sec_login_auth_src_t  authSrc;
    error_status_t        funcStatus;

    if (!SetupIdentity(ctx, context, acctName, flags, passwd, errorCase, 
                  &resetLogin, &authSrc) &&
        authSrc == sec_login_auth_src_network  )
       {
			sec_login_certify_identity(*context,&funcStatus); 
	   		dce_error_inq_text(funcStatus, error_buf, &status);
           TSHMessage (ctx, MOTRACE, "sec_login_certify_identity ( .. ), status; %s\n",error_buf);
           if ( TSHStatus (ctx, SETUP, "sec_login_certify_identity", funcStatus, error_status_ok) )
                return false;

       sec_login_set_context(*context, &funcStatus);
	   dce_error_inq_text(funcStatus, error_buf, &status);
       TSHMessage (ctx, MOTRACE, "sec_login_set_context ( .. ), status; %s\n",error_buf);
       return TSHStatus (ctx, SETUP, "sec_login_set_context",funcStatus, sec_login_s_ok ) ; 
       }
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
   ndr_long_int          argsRequired;
   ndr_long_int          testCase;             /* argv[0]/100, testCase to run */
   ndr_long_int          errorCase;            /* argv[0]%100, errorCase, if any */
   idl_char              passwdString[257];    /* arbitrary length */

   error_status_t        expctdStatus;         /* argv[1], expected return from test */
   error_status_t        funcStatus;       /* status value returned from rpc */
   ndr_long_int          testStatus;       /* status value returned from TestFunction */

   unsigned_char_p_t     acctName;
   sec_passwd_rec_t      passwd,
                         altPasswd;
   ndr_long_int	         flags;

   boolean32		 passwdExpired;
   sec_login_auth_src_t  authSrc;
   sec_login_handle_t    context,
                         newContext;

   signed32              expirationTime;
   unsigned_char_p_t     pwent;
   idl_byte              *xbuffer;

   unsigned32            xbufferLen,
                         bufUsed,
                         bufNeeded;

   unsigned32            numGroups;
   signed32              *groupList;
   sec_id_t              *newGroups;
   unsigned32            newGroupCount=2;

   unsigned32            i;
   char			 bogus_passwd[6] = "BOGUS";

/*
** Bug in CMA requires initialization
*/
   cma_init();

/* Obtain the minimum input arguments from the .tsh command line. */

   testCase = atol(argv[0]) / 100 * 100 ;
   errorCase = atol(argv[0]) % 100 ;

   switch (testCase)
   {
        case SETUP_IDENTITY:
        case PURGE_CONTEXT:
        case RELEASE_CONTEXT:
        case GET_EXPIRATION:
        case GET_PWENT:
        case GET_GROUPS:
        case CERTIFY_IDENTITY:
		case VALID_CERT_IDENT:
            argsRequired=4;
            break;
        case VALIDATE_IDENTITY:
        case SET_CONTEXT:
        case GET_CURRENT_CONTEXT:
        case REFRESH_IDENTITY:
        case CHECK_PASSWORD:
        case EXPORT_IMPORT_CONTEXT:
        case NEWGROUPS:
            argsRequired=5;
            break;

        default:
            TSHMessage (ctx, MOFAILURE, "Unimplemented testCase %d\n",testCase);
            return TSH_UNIMPLEMENTED;
    }
    
     if ( argc != argsRequired)
         {
         TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
                    argsRequired, argc);
         return TSH_BAD_ARG_COUNT ;
         }


   expctdStatus = atol(argv[1]); 
   acctName=argv[2];

   testStatus = TSH_TEST_FAILED;         /* initial */
   flags=sec_login_no_flags;             /* by default */

   switch (testCase)
   {
        case PURGE_CONTEXT:
        case SETUP_IDENTITY:
        case RELEASE_CONTEXT:
            flags=atol(argv[3]);
            break;

        case VALIDATE_IDENTITY:
        case SET_CONTEXT:
        case GET_CURRENT_CONTEXT:
        case REFRESH_IDENTITY:
        case NEWGROUPS:
            flags=atol(argv[3]);
            passwd.key.tagged_union.plain =argv[4];
            break;

       case EXPORT_IMPORT_CONTEXT:
            passwd.key.tagged_union.plain =argv[3];
            xbufferLen=atol(argv[4]);
            if ( (xbuffer= (idl_byte *) malloc(xbufferLen)) == NULL )
                {
                TSHMessage (ctx, MOFAILURE, "Malloc failed\n");
                return testStatus;
                }
            break;

        case GET_EXPIRATION:
        case GET_PWENT:
        case GET_GROUPS:
        case CERTIFY_IDENTITY:
		case VALID_CERT_IDENT:
            passwd.key.tagged_union.plain =argv[3];
            break;

        case CHECK_PASSWORD:
            passwd.key.tagged_union.plain =argv[3];
            altPasswd.key.tagged_union.plain =argv[4];
            break;

    }
    passwd.key.key_type = sec_passwd_plain;  
    passwd.pepper = NULL;
    passwd.version_number = sec_passwd_c_version_none;
    altPasswd.key.key_type = sec_passwd_plain;  
    altPasswd.pepper = NULL;
    altPasswd.version_number = sec_passwd_c_version_none;



   switch (testCase)
       {
       case SETUP_IDENTITY:
           sec_login_setup_identity(acctName, flags, &context, &funcStatus);
           contextValid=(funcStatus == sec_login_s_ok) ? true:false;
           dce_error_inq_text(funcStatus, error_buf, &status);
           TSHMessage (ctx, MOTRACE, "sec_login_setup_identity (%s %d.. ), status; %s\n", 
                acctName, flags, error_buf);
                     
           if (TSHStatus (ctx, TEST, "sec_login_setup_identity", funcStatus, expctdStatus) )
                break;

           if ( contextValid  )
              {
              sec_login_purge_context(&context, &funcStatus);
           	  dce_error_inq_text(funcStatus, error_buf, &status);
              TSHMessage (ctx, MOTRACE, "sec_login_purge_context ( .. ), status; %s\n",error_buf);
              contextValid=false;
              }
           testStatus=TSH_SUCCESS;
           break;


       case PURGE_CONTEXT:				/* uses the same code */
/*
** Do normal Setup and release
*/

           sec_login_setup_identity(acctName, flags, &context, &funcStatus);
           dce_error_inq_text(funcStatus, error_buf, &status);
           TSHMessage (ctx, MOTRACE, "sec_login_setup_identity (%s %d.. ), status; %s\n", 
                acctName, flags, error_buf);
                     
           if (TSHStatus (ctx, SETUP, "sec_login_setup_identity", 
                          funcStatus, sec_login_s_ok ) )
               break;
               

           sec_login_purge_context(&context, &funcStatus);
           dce_error_inq_text(funcStatus, error_buf, &status);
           TSHMessage (ctx, MOTRACE, "sec_login_purge_context ( .. ), status; %s\n",error_buf);
           TSHStatus (ctx, TEST, "sec_login_purge_context", funcStatus, sec_login_s_ok);
           contextValid=false;

/*
** Now release again: Handle should be default (NULL) 
*/
        
          sec_login_purge_context(&context, &funcStatus);
          dce_error_inq_text(funcStatus, error_buf, &status);
          TSHMessage (ctx, MOTRACE, "sec_login_purge_context ( .. ), status; %s\n",error_buf);
          TSHStatus (ctx, TEST, "sec_login_purge_context", funcStatus, expctdStatus);

          if ( funcStatus != sec_login_s_default_use ) 
                break;

          testStatus=TSH_SUCCESS;
          break;

       case RELEASE_CONTEXT:
/*
**  Do normal Setup and release 
*/

           sec_login_setup_identity(acctName, flags, &context, &funcStatus);
           contextValid=(funcStatus == sec_login_s_ok) ? true:false;
           dce_error_inq_text(funcStatus, error_buf, &status);
           TSHMessage (ctx, MOTRACE, "sec_login_setup_identity (%s %d.. ), status; %s\n", 
                acctName, flags, error_buf);
                     
           if (TSHStatus (ctx, SETUP, "sec_login_setup_identity", 
                          funcStatus, sec_login_s_ok ) )
               break;

           sec_login_release_context(&context, &funcStatus);
           dce_error_inq_text(funcStatus, error_buf, &status);
           TSHMessage (ctx, MOTRACE, "sec_login_release_context ( .. ), status; %s\n",error_buf);
           TSHStatus (ctx, TEST, "sec_login_release_context", funcStatus, sec_login_s_ok);
           contextValid=false;

/*
** Now release again: Handle should be default (NULL) 
*/
        
          sec_login_release_context(&context, &funcStatus);
          dce_error_inq_text(funcStatus, error_buf, &status);
          TSHMessage (ctx, MOTRACE, "sec_login_release_context ( .. ), status; %s\n",error_buf);
          TSHStatus (ctx, TEST, "sec_login_release_context", funcStatus, expctdStatus);

          if ( funcStatus != sec_login_s_default_use )
                break;

          testStatus=TSH_SUCCESS;
          break;


       case VALIDATE_IDENTITY:

           sec_login_setup_identity(acctName, flags, &context, &funcStatus);
           contextValid=(funcStatus == sec_login_s_ok) ? true:false;
           dce_error_inq_text(funcStatus, error_buf, &status);
           TSHMessage (ctx, MOTRACE, "sec_login_setup_identity (%s %d.. ), status; %s\n", 
                acctName, flags, error_buf);
                     
           if ( !(errorCase & BAD_OBJECT_FLAG) && 
                TSHStatus (ctx, SETUP, "sec_login_setup_identity", 
                           funcStatus, sec_login_s_ok ) )
               break;

           sec_login_validate_identity(context, 
                                       &passwd, 
                                       &passwdExpired, 
                                       &authSrc, 
                                       &funcStatus );
           dce_error_inq_text(funcStatus, error_buf, &status);
           TSHMessage(ctx, MOTRACE, "sec_login_validate_identity (%s .. ), status; %s\n", 
              passwd.key.tagged_union.plain, error_buf);

           TSHMessage (ctx, MOTRACE, "Reset Login flag is %s, AuthSrc is %s \n", 
                       (passwdExpired) ? "true":"false",
                       (authSrc == sec_login_auth_src_local) ? "Local":"Network" );

           if ( TSHStatus (ctx, TEST, "sec_login_validate_identity", funcStatus, expctdStatus) ) 
                break;

           testStatus=TSH_SUCCESS;
           break;

        case SET_CONTEXT:
          if (SetupIdentity(ctx,&context,acctName,flags,&passwd,errorCase,&passwdExpired,&authSrc) )
              break;

 	  	  sec_login_set_context(context, &funcStatus);
          dce_error_inq_text(funcStatus, error_buf, &status);
          TSHMessage (ctx, MOTRACE, "sec_login_set_context ( .. ), status; %s\n",error_buf);
          if ( TSHStatus (ctx, TEST, "sec_login_set_context",funcStatus, expctdStatus ) ) 
                break;

          testStatus=TSH_SUCCESS;
          break;

      case GET_CURRENT_CONTEXT:
          if (SetupContext(ctx,&context,acctName,flags,&passwd,errorCase) )
              break;

          sec_login_get_current_context(&newContext, &funcStatus);
          dce_error_inq_text(funcStatus, error_buf, &status);
          TSHMessage (ctx, MOTRACE, "sec_login_get_current_context ( .. ), status; %s\n",error_buf);

          if (TSHStatus (ctx, TEST, "sec_login_get_current_context", funcStatus,expctdStatus) )
                break;

          if ( newContext != context )
              {
              TSHStatus (ctx, TEST, "sec_login_get_current_context: new context handle different from old",
                         TSH_TEST_FAILED,0);
                break;
              }

          testStatus=TSH_SUCCESS;
          break;

      case REFRESH_IDENTITY:
/*
** sec_login_validate_identity (in SetupContext) kindly trashes the password string 
** for security reasons. We have to copy for use later.
*/            
          strncpy(passwdString,argv[4],sizeof(passwdString)-1 );
          passwdString[sizeof(passwdString)-1]='\0';

          if (SetupContext(ctx,&context,acctName,flags,&passwd,errorCase) )
              break;

 	  	  sec_login_refresh_identity(context, &funcStatus);
          dce_error_inq_text(funcStatus, error_buf, &status);
          TSHMessage (ctx, MOTRACE, "sec_login_refresh_identity ( .. ), status; %s\n",error_buf);

          if ( TSHStatus (ctx, TEST, "sec_login_refresh_identity", funcStatus, expctdStatus) )
              break;

          passwd.key.tagged_union.plain =passwdString;
          passwd.key.key_type = sec_passwd_plain;  
          passwd.pepper = NULL;
          passwd.version_number = sec_passwd_c_version_none;

          sec_login_validate_identity(context, 
                                       &passwd, 
                                       &passwdExpired, 
                                       &authSrc, 
                                       &funcStatus );
           dce_error_inq_text(funcStatus, error_buf, &status);
           TSHMessage(ctx, MOTRACE, "sec_login_validate_identity (%s .. ), status; %s\n", 
              passwd, error_buf);

           TSHMessage (ctx, MOTRACE, "Reset Login flag is %s, AuthSrc is %s \n", 
                       (passwdExpired) ? "true":"false",
                       (authSrc == sec_login_auth_src_local) ? "Local":"Network" );

           if ( TSHStatus (ctx, TEST, "sec_login_validate_identity", funcStatus, sec_login_s_ok) ) 
                break;

           testStatus=TSH_SUCCESS;
           break;

       case CHECK_PASSWORD:
           if (SetupContext(ctx,&context,acctName,flags,&passwd,errorCase) )
              break;

           sec_login_check_passwd(acctName, &altPasswd, &authSrc, &funcStatus);
           dce_error_inq_text(funcStatus, error_buf, &status);
           TSHMessage (ctx, MOTRACE, "sec_login_check_password (%s,... ) status; %s\n", 
                    acctName, error_buf);
           TSHMessage (ctx, MOTRACE, "AuthSrc is %s \n", 
                  (authSrc == sec_login_auth_src_local) ? "Local":"Network" );

           if (TSHStatus (ctx, TEST, "sec_login_check_password", funcStatus, expctdStatus) )
                break;

           testStatus=TSH_SUCCESS;
           break;

       case GET_EXPIRATION:
           if (SetupContext(ctx,&context,acctName,flags,&passwd,errorCase) )
              break;

           sec_login_get_expiration(context, &expirationTime, &funcStatus);
           dce_error_inq_text(funcStatus, error_buf, &status);
           TSHMessage (ctx, MOTRACE, "sec_login_get_expiration ( ..%s ), status; %s\n",
                              ctime(&expirationTime),error_buf);
           if (TSHStatus (ctx, TEST, "sec_login_get_expiration", funcStatus, expctdStatus) )
                break;

           testStatus=TSH_SUCCESS;
           break;

       case GET_PWENT:
           if (SetupContext(ctx,&context,acctName,flags,&passwd,errorCase) )
              break;

           sec_login_get_pwent(context, &pwent, &funcStatus);
           dce_error_inq_text(funcStatus, error_buf, &status);
           TSHMessage (ctx, MOTRACE, "sec_login_get_pwent status; %s\n",error_buf);
           if ( TSHStatus (ctx, TEST, "sec_login_get_pwent", funcStatus, expctdStatus) )
             break;
            
           if ( funcStatus == sec_login_s_ok && strlen(pwent) == 0 )
               {
               TSHMessage (ctx, MOFAILURE, "Empty passwd string\n");
               break;
               }

           TSHMessage (ctx, MOTRACE, "sec_login_get_pwent pwent: :%s:\n",pwent);

           testStatus=TSH_SUCCESS;
           break;

       case EXPORT_IMPORT_CONTEXT:
           if (SetupContext(ctx,&context,acctName,flags,&passwd,errorCase) )
              break;

           sec_login_export_context(context,xbufferLen, xbuffer, 
                                     &bufUsed, &bufNeeded, &funcStatus);
           dce_error_inq_text(funcStatus, error_buf, &status);
           TSHMessage (ctx, MOTRACE, "sec_login_export_context (buffer len:%d,..used:%d, needed:%d), status; %s\n",
                              xbufferLen,bufUsed,bufNeeded,error_buf);
           if ( TSHStatus (ctx, TEST, "sec_login_export_context", funcStatus, expctdStatus) )
                break;


           sec_login_import_context(bufUsed, xbuffer, &newContext, &funcStatus);
           dce_error_inq_text(funcStatus, error_buf, &status);
           TSHMessage (ctx, MOTRACE, "sec_login_import_context (buffer len:%d,..), status; %s\n",
                                  bufUsed,error_buf);
           if ( TSHStatus (ctx, TEST, "sec_login_import_context", funcStatus, sec_login_s_ok) )
                break;

           sec_login_set_context(context, &funcStatus);
           dce_error_inq_text(funcStatus, error_buf, &status);
           TSHMessage (ctx, MOTRACE, "sec_login_set_context ( .. ), status; %s\n",error_buf);
           if (TSHStatus (ctx, TEST, "sec_login_set_context", funcStatus, sec_login_s_ok) )
             break;

           testStatus=TSH_SUCCESS;
           break; 

        case GET_GROUPS:
			if (expctdStatus == sec_login_s_not_certified) {
           		if (SetupContext(ctx,&context,acctName,flags,&passwd,errorCase) )
              		break; 
				} 
			else if (expctdStatus == error_status_ok) {
           		if (CertifyContext(ctx,&context,acctName,flags,&passwd,errorCase) )
              		break; 
				} 
			else if (expctdStatus == sec_login_s_default_use)
				context = NULL;

           numGroups=0;
           groupList=NULL;

           sec_login_get_groups(context, &numGroups, &groupList, &funcStatus);
           dce_error_inq_text(funcStatus, error_buf, &status);
           TSHMessage (ctx, MOTRACE, "sec_login_get_groups ( # groups: %d, ), status; %s\n",
                        numGroups,error_buf);


           if ( TSHStatus (ctx, TEST, "sec_login_get_groups", funcStatus, expctdStatus) )
                break;

           TSHMessage (ctx, MOTRACE, "Group List Element          Group ID\n");
           for (i=0; i < numGroups; i++)
               TSHMessage (ctx, MOTRACE, "%d                          %d\n", i,groupList[i]);


           testStatus=TSH_SUCCESS;
           break;

        case NEWGROUPS:  
		{
	   sec_rgy_handle_t pgo_context;

	   testStatus=TSH_SUCCESS;
             
	   sec_login_get_current_context(&context, &funcStatus);
           dce_error_inq_text(funcStatus, error_buf, &status);
           TSHMessage (ctx, MOTRACE, "sec_login_get_current_context status: %s\n",
                        error_buf);
			/* certify context */
			sec_login_certify_identity(context,&funcStatus); 
           dce_error_inq_text(funcStatus, error_buf, &status);
           TSHMessage (ctx, MOTRACE, "sec_login_certify_identity ( .. ), status; %s\n",error_buf);
           if ( TSHStatus (ctx, SETUP, "sec_login_certify_identity", funcStatus, error_status_ok) )
				break;
                                    

			/* get original group */
           sec_login_get_groups(context, &numGroups, &groupList, &funcStatus);
           dce_error_inq_text(funcStatus, error_buf, &status);
           TSHMessage (ctx, MOTRACE, "sec_login_get_groups ( # groups: %d, ), status; %s\n",
                        numGroups,error_buf);

           if ( TSHStatus (ctx, TEST, "sec_login_get_groups", funcStatus, error_status_ok) )
                break;

           TSHMessage (ctx, MOTRACE, "Group List Element          Group ID\n");
           for (i=0; i < numGroups; i++)
               TSHMessage (ctx, MOTRACE, "%d                          %d\n", i,groupList[i]);
                         
			/* malloc the space */
            if ( (newGroups= (sec_id_t *) malloc( newGroupCount*sizeof(sec_id_t) )) == NULL )
                {
                TSHMessage (ctx, MOFAILURE, "Malloc failed\n");
                return testStatus;
                }

			/* setup the new group */
    			    
    			sec_rgy_site_open_update("", &pgo_context, &funcStatus);
           		dce_error_inq_text(funcStatus, error_buf, &status);
           		TSHMessage (ctx, MOTRACE, "sec_rgy_site_open_update status;:%s\n", error_buf);
           		if ( TSHStatus (ctx, SETUP, "sec_rgy_site_open_update failed ", funcStatus, error_status_ok) )
                	break;
     	                 
				for (i = 0; i <=1; i++) {   
    				sec_rgy_pgo_unix_num_to_id(pgo_context, sec_rgy_domain_group, groupList[i], &newGroups[i].uuid, &funcStatus);
           			dce_error_inq_text(funcStatus, error_buf, &status);
           			TSHMessage (ctx, MOTRACE, "sec_rgy_pgo_unix_num_to_id status;:%s\n", error_buf);
           			if ( TSHStatus (ctx, SETUP, "sec_rgy_pgo_unix_num_to_id failed ", funcStatus, error_status_ok) )
                		break;
				}
    
    			if	(expctdStatus != error_status_ok) {
    				/*	add a non_existent group  the the second element */ 
					uuid_create(&newGroups[1].uuid, &funcStatus);
    	          	dce_error_inq_text(funcStatus, error_buf, &status);
    	          	TSHMessage (ctx, MOTRACE, "uuid_create status;:%s\n", error_buf);
    	          	if ( TSHStatus (ctx, SETUP, "uuid_create failed ", funcStatus, error_status_ok) )
    	               	break;  
    			}
			
	              
			/* do the test */
	          sec_login_newgroups(context,flags, newGroupCount, newGroups, &newContext, &funcStatus);
	          dce_error_inq_text(funcStatus, error_buf, &status);
	          TSHMessage (ctx, MOTRACE, "sec_login_newgroups (%d, %d, ... ), status; %s\n",
	                       flags,newGroupCount,error_buf);
	          if (! TSHStatus (ctx, TEST, "sec_login_newgroups", funcStatus, expctdStatus) ){
					funcStatus = error_status_ok;
	           } 
				else 
					testStatus = funcStatus;
                       
 				/* cleanup */
		   if (funcStatus == error_status_ok && expctdStatus == error_status_ok) {

    			/* check the status */ 
               sec_login_get_groups(newContext, &numGroups, &groupList, &funcStatus);
               dce_error_inq_text(funcStatus, error_buf, &status);
               TSHMessage (ctx, MOTRACE, "sec_login_get_groups ( # groups: %d, ), status; %s\n",
                            numGroups,error_buf); 
			   if (TSHStatus (ctx, TEST, "sec_login_get_groups", numGroups, newGroupCount) )
					testStatus = TSH_DATA_MISSMATCH;
    
               if (! TSHStatus (ctx, TEST, "sec_login_get_groups", funcStatus, error_status_ok) ) {
    
               	   TSHMessage (ctx, MOTRACE, "Group List Element          Group ID\n");
                   for (i=0; i < numGroups; i++)
                       TSHMessage (ctx, MOTRACE, "%d                          %d\n", i,groupList[i]);
                                 
                  }
    
                sec_login_purge_context(&newContext, &funcStatus);
     	        dce_error_inq_text(funcStatus, error_buf, &status);
    	        TSHMessage (ctx, MOTRACE, "sec_login_purge_context status; %s\n",
    	                       error_buf);
    	        if (TSHStatus (ctx, CLNUP, "sec_login_purge_context", funcStatus, error_status_ok) )
    				testStatus = funcStatus;
            } 

           break;  
        }

        case CERTIFY_IDENTITY:
            switch(expctdStatus)
                {
                case sec_login_s_default_use: 
                   context = NULL;
                break;
                case sec_login_s_context_invalid: /* Do setup identity, no validate identity */
                        sec_login_setup_identity(acctName, flags, &context, &funcStatus);
           				dce_error_inq_text(funcStatus, error_buf, &status);
                        TSHMessage (ctx, MOTRACE, "sec_login_setup_identity (%s %d.. ), status; %s\n", 
                                    acctName, flags, error_buf);
                        contextValid=(funcStatus == sec_login_s_ok) ? true:false;

                        if ( TSHStatus (ctx, SETUP, "sec_login_setup_identity", 
                                funcStatus, sec_login_s_ok ) )
                            return TSH_TEST_FAILED;
                break;
                default:
                       if (SetupContext(ctx,&context,acctName,flags,&passwd,errorCase) )
                break;
                }  

           sec_login_certify_identity(context,&funcStatus); 
           dce_error_inq_text(funcStatus, error_buf, &status);
           TSHMessage (ctx, MOTRACE, "sec_login_certify_identity ( .. ), status; %s\n",error_buf);
           if ( TSHStatus (ctx, TEST, "sec_login_certify_identity", funcStatus, expctdStatus) )
                break;

           testStatus=TSH_SUCCESS;
           break;  

		case VALID_CERT_IDENT:
            switch(expctdStatus)
                {
                case sec_login_s_default_use: 
                   context = NULL;
                break;  
				default:
                    sec_login_setup_identity(acctName, flags, &context, &funcStatus);
       				dce_error_inq_text(funcStatus, error_buf, &status);
                    TSHMessage (ctx, MOTRACE, "sec_login_setup_identity (%s %d.. ), status; %s\n", 
                                acctName, flags, error_buf);

                    if ( TSHStatus (ctx, SETUP, "sec_login_setup_identity", 
                            funcStatus, sec_login_s_ok ) )
                            return TSH_TEST_FAILED;
				}  
		   if (expctdStatus == sec_rgy_passwd_invalid)
					passwd.key.tagged_union.plain = bogus_passwd;  

			sec_login_valid_and_cert_ident(context, &passwd, &passwdExpired, 
                                       &authSrc, 
                                       &funcStatus );
           dce_error_inq_text(funcStatus, error_buf, &status);
           TSHMessage(ctx, MOTRACE, "sec_login_valid_and_cert_ident (%s .. ), status; %s\n", 
              passwd, error_buf);

           TSHMessage (ctx, MOTRACE, "Reset Login flag is %s, AuthSrc is %s \n", 
                       (passwdExpired) ? "true":"false",
                       (authSrc == sec_login_auth_src_local) ? "Local":"Network" );

           if ( TSHStatus (ctx, TEST, "sec_login_valid_and_cert_ident", funcStatus, expctdStatus) ) 
                break;

           testStatus=TSH_SUCCESS;
           break;
				
  
      }

   if ( contextValid )
       {                       
       sec_login_purge_context(&context, &funcStatus);
       dce_error_inq_text(funcStatus, error_buf, &status);
       TSHMessage (ctx, MOTRACE, "sec_login_purge_context ( .. ), status; %s\n",error_buf);
       if ( TSHStatus (ctx, CLNUP, "sec_login_purge_context", funcStatus, sec_login_s_ok) )
           testStatus=TSH_TEST_FAILED;
       }
   return testStatus;

}

