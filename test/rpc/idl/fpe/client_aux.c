/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * (c) Copyright 1991, 1992 
 *     Siemens-Nixdorf Information Systems, Burlington, MA, USA
 *     All Rights Reserved
 */
/*
 * HISTORY
 * $Log: client_aux.c,v $
 * Revision 1.1.5.2  1996/02/17  23:14:13  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:34:44  marty]
 *
 * Revision 1.1.5.1  1995/12/11  20:05:49  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:31:31  root]
 * 
 * Revision 1.1.3.3  1993/01/07  19:56:59  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  13:33:48  bbelch]
 * 
 * Revision 1.1.3.2  1992/09/29  21:21:23  devsrc
 * 	SNI/SVR4 merge.  OT 5373
 * 	[1992/09/11  15:46:39  weisman]
 * 
 * Revision 1.1  1992/01/19  03:24:50  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
#include <stdio.h>
#include <dce/cma.h>
#include <fpe.h>

int client_aux(h, passno)
    handle_t h;
    int passno;
{
    int failures = 0;
    ndr_short_float fa, fb;
    ndr_long_float  da, db;

    setup(h);

    /************************/
    /**** float_overflow ****/
    /************************/

    fa = fb = 1.0e+25;
    TRY
        float_overflow(h, &fa, &fb);
        fprintf(stderr, "client_aux: float_overflow() did not cause exception\n");
        failures++;
    CATCH(exc_e_fltovf)
#ifdef SNI_SVR4
    CATCH(exc_e_aritherr)
#endif    
    CATCH_ALL
        fprintf(stderr, "client_aux: float_overflow() caused unexpected exception:\n");
        print_exception(THIS_CATCH);
        failures++;
    ENDTRY

    /*************************/
    /**** float_underflow ****/
    /*************************/

    fa = fb = 1.0e-25;
    TRY
        float_underflow(h, &fa, &fb);
        fprintf(stderr, "client_aux: float_underflow() did not cause exception\n");
        failures++;
    CATCH(exc_e_fltund)
#ifdef SNI_SVR4
    CATCH(exc_e_aritherr)
#endif    
    CATCH_ALL
        fprintf(stderr, "client_aux: float_underflow() caused unexpected exception:\n");
        print_exception(THIS_CATCH);
        failures++;
    ENDTRY

    /*************************/
    /**** float_divbyzero ****/
    /*************************/

    fa = fb = 0.0;
    TRY
        float_divbyzero(h, &fa, &fb);
        fprintf(stderr, "client_aux: float_divbyzero() did not cause exception\n");
        failures++;
#ifdef SNI_SVR4
    CATCH(exc_e_aritherr)
#endif    
    CATCH(exc_e_fltdiv)
    CATCH_ALL
        fprintf(stderr, "client_aux: float_divbyzero() caused unexpected exception:\n");
        print_exception(THIS_CATCH);
        failures++;
    ENDTRY


    /*************************/
    /**** double_overflow ****/
    /*************************/

    da = db = 1.0e+25;
    TRY
        double_overflow(h, &da, &db);
        fprintf(stderr, "client_aux: double_overflow() did not cause exception\n");
        failures++;
    CATCH(exc_e_fltovf)
#ifdef SNI_SVR4
    CATCH(exc_e_aritherr)
#endif    
    CATCH_ALL
        fprintf(stderr, "client_aux: double_overflow() caused unexpected exception:\n");
        print_exception(THIS_CATCH);
        failures++;
    ENDTRY

    /**************************/
    /**** double_underflow ****/
    /**************************/

    da = db = 1.0e-25;
    TRY
        double_underflow(h, &da, &db);
        fprintf(stderr, "client_aux: double_underflow() did not cause exception\n");
        failures++;
    CATCH(exc_e_fltund)
#ifdef SNI_SVR4
    CATCH(exc_e_aritherr)
#endif    
    CATCH_ALL
        fprintf(stderr, "client_aux: double_underflow() caused unexpected exception:\n");
        print_exception(THIS_CATCH);
        failures++;
    ENDTRY

    /**************************/
    /**** double_divbyzero ****/
    /**************************/

    da = db = 0.0;
    TRY
        double_divbyzero(h, &da, &db);
        fprintf(stderr, "client_aux: double_divbyzero() did not cause exception\n");
        failures++;
    CATCH(exc_e_fltdiv)
#ifdef SNI_SVR4
    CATCH(exc_e_aritherr)
#endif    
    CATCH_ALL
        fprintf(stderr, "client_aux: double_divbyzero() caused unexpected exception:\n");
        print_exception(THIS_CATCH);
        failures++;
    ENDTRY

    return(failures);
}
