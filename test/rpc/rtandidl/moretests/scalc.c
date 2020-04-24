/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: scalc.c,v $
 * Revision 1.1.4.2  1996/02/17  23:22:01  marty
 * 	Update OSF copyright year
 * 	[1996/02/17  22:41:02  marty]
 *
 * Revision 1.1.4.1  1995/12/11  20:09:43  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  19:44:49  root]
 * 
 * Revision 1.1.2.4  1993/01/25  19:27:48  bbelch
 * 	Fix duplicate log
 * 	[1993/01/25  17:44:05  bbelch]
 * 
 * Revision 1.1.2.3  1993/01/11  23:39:42  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/06  18:13:05  bbelch]
 * 
 * Revision 1.1.2.2  1992/10/12  17:30:55  cuti
 * 	Remove hardcoded protocol seq and endpoint instead using all_protoseq.
 * 	[1992/10/09  21:19:25  cuti]
 * 
 * Revision 1.1  1992/01/19  03:36:52  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
#
#
# Copyright (c) 1991 by
#    Hewlett-Packard Company, Palo Alto, Ca.
#
# Permission to use, copy, modify, and distribute this
# software and its documentation for any purpose and without
# fee is hereby granted, provided that the above copyright
# notice appear in all copies and that both that copyright
# notice and this permission notice appear in supporting
# documentation, and that the name of Hewlett-Packard Company not
# be used in advertising or publicity pertaining to distribution
# of the software without specific, written prior permission.
# Hewlett-Packard Company makes no representations about the
# suitability of this software for any purpose.  It is provided
# "as is" without express or implied warranty.
#
#
# DCE API Test
#
*/



#include <scal.h>
#include <scalt.h>
#include <tsh_common.h>
#include <stdio.h>
#include <math.h>
#include <dce/rpc.h> 


#define ARGS_REQUIRED 4

ndr_long_int TestFunction(ctx, argc, argv)
CtxBlock *ctx;
ndr_long_int argc;
unsigned_char_p_t argv[];
{
    ndr_long_int subcase;
    unsigned_char_p_t bound_string;
    error_status_t status;
    handle_t h;



    /**************************************************************************/
    /* Check the number of arguments passed in.  They should be exactly the   */
    /* required number when you come into this routine. Looking for a subcase,*/
    /* a bogus expected_status, and a hostname.                               */
    /**************************************************************************/
    if(argc != ARGS_REQUIRED)
    {
	TSHMessage(ctx, MOFAILURE, "Expecting %d arguments, got %d\n", 
                                                               ARGS_REQUIRED, argc);
        return(TSH_BAD_ARG_COUNT);
    }

    subcase = atol(argv[0]);         /* turn subcase string into an int.      */
                                     /* ignore argv[2] expected status value, */
                                     /* each test does its own error checking.*/

    /**************************************************************************/
    /*  Do the compose of the string, followed by the binding from string.    */
    /**************************************************************************/
    rpc_string_binding_compose(
       NULL, (unsigned_char_p_t)argv[3], 
       argv[2], NULL, NULL, &bound_string, &status);

    if (TSHStatus(ctx, SETUP, "rpc_string_binding_compose", status, rpc_s_ok))
       return(status);
    rpc_binding_from_string_binding(bound_string, &h, &status);
    if (TSHStatus(ctx, SETUP, "rpc_string_binding_compose", status, rpc_s_ok))
       return(status);
    return(scalars_test(h, ctx, subcase));
}

int scalars_test (h, ctx, subcase)
handle_t h;
CtxBlock *ctx;
ndr_long_int subcase;
{
    ndr_long_int i;

    ndr_small_int sm1, sm2;
    ndr_usmall_int usm1, usm2;

    ndr_short_int sh1, sh2;
    ndr_ushort_int ush1, ush2;

    ndr_long_int l1, l2;
    ndr_ulong_int ul1, ul2;

    ndr_hyper_int h1, h2;
    ndr_uhyper_int uh1, uh2;

    ndr_boolean bool1, bool2;

    ndr_byte byte1, byte2;

    ndr_short_float sf1, sf2;
    ndr_long_float lf1, lf2;

    ndr_char char1, char2;


    switch(subcase)
    {

        case SCALAR_SMALL_INT:
        /**********************************************************************/
        /* test scalartest_op1: pass in a small int, add 1 to it and pass it  */
        /* back in a separate slot.                                           */
        /**********************************************************************/
        for (i = 0, sm1 = 0; i < 20; i++) 
        {
            sm2 = 0;
            scalartest_op1 (h, sm1, &sm2);
            TSHMessage (ctx, MOTRACE, "sm1 (%d) ? sm2 (%ld) - 1\n", sm1, sm2);
            if (sm1 != sm2 - 1) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING, "sm1 (%d) != sm2 (%ld) - 1\n",
                                                                      sm1, sm2);
                return(TSH_INCORRECT_DATA);
            }
            sm1 += 23;
        }
        break;
    
        case SCALAR_USMALL_INT:
        /**********************************************************************/
        /* test scalartest_op2: pass in an unsigned small int, add 1 to it    */
        /* and pass it back in a separate slot.                               */
        /**********************************************************************/
        for (i = 0, usm1 = 0; i < 20; i++) 
        {
            usm2 = 0;
            scalartest_op2 (h, usm1, &usm2);
            TSHMessage (ctx, MOTRACE, "usm1 (%d) ? usm2 (%ld) - 1\n",usm1, usm2);
            if (usm1 != usm2 - 1) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING, "usm1 (%d) != usm2 (%ld) - 1\n", 
                                                                    usm1, usm2);
                return(TSH_INCORRECT_DATA);
            }
            usm1 += 23;
        }
        break;
    
        case SCALAR_SHORT_INT:
        /**********************************************************************/
        /* test scalartest_op3: pass in a short int, add 1 to it and pass it  */
        /* back in a separate slot.                                           */
        /**********************************************************************/
        for (i = 0, sh1 = 0; i < 20; i++) 
        {
            sh2 = 0;
            scalartest_op3 (h, sh1, &sh2);
            TSHMessage (ctx, MOTRACE, "sh1 (%d) ? sh2 (%ld) - 1\n", sh1, sh2);
            if (sh1 != sh2 - 1) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING, "sh1 (%d) != sh2 (%ld) - 1\n", 
                                                                      sh1, sh2);
                return(TSH_INCORRECT_DATA);
            }
            sh1 += 23*256+23;
        }
        break;
    
        case SCALAR_USHORT_INT:
        /**********************************************************************/
        /* test scalartest_op4: pass in an unsigned short int, add 1 to it and*/
        /* pass it back in a separate slot.                                   */
        /**********************************************************************/
        for (i = 0, ush1 = 0; i < 20; i++) 
        {
            ush2 = 0;
            scalartest_op4 (h, ush1, &ush2);
            TSHMessage (ctx, MOTRACE,"ush1 (%d) ? ush2 (%ld) - 1\n", ush1, ush2);
            if (ush1 != ush2 - 1) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING,"ush1 (%d) != ush2 (%ld) - 1\n", 
                                                                    ush1, ush2);
                return(TSH_INCORRECT_DATA);
            }
            ush1 += 23*256+23;
        }
        break;
    
        case SCALAR_LONG_INT:
        /**********************************************************************/
        /* test scalartest_op5: pass in a long int, add 1 to it and pass it   */
        /* back in a separate slot.                                           */
        /**********************************************************************/
        for (i = 0, l1 = 0; i < 20; i++) 
        {
            l2 = 0;
            scalartest_op5 (h, l1, &l2);
            TSHMessage (ctx, MOTRACE,"l1 (%ld) ? l2 (%ld) - 1\n",l1, l2);
            if (l1 != l2 - 1) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING,"l1 (%ld) != l2 (%ld) - 1\n",l1, l2);
                return(TSH_INCORRECT_DATA);
            }
            l1 += 123456789;
        }
        break;
    
        case SCALAR_ULONG_INT:
        /**********************************************************************/
        /* test scalartest_op6: pass in an unsigned long int, add 1 to it and */
        /* pass it back in a separate slot.                                   */
        /**********************************************************************/
        for (i = 0, ul1 = 0; i < 20; i++) 
        {
            ul2 = 0;
            scalartest_op6 (h, ul1, &ul2);
            TSHMessage (ctx, MOTRACE,"ul1 (%ld) ? ul2 (%ld) - 1\n", ul1, ul2);
            if (ul1 != ul2 - 1) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING,"ul1 (%ld) != ul2 (%ld) - 1\n",
                                                                      ul1, ul2);
                return(TSH_INCORRECT_DATA);
            }
            ul1 += 123456789;
        }
        break;
    
#ifndef cray
        case SCALAR_HYPER_INT:
        /**********************************************************************/
        /* test scalartest_op7: pass in a hyper int, add 1 to it and pass it  */
        /* back in a separate slot.                                           */
        /**********************************************************************/
        h1.low = 0;
        for (i = 0; i < 20; i++) 
        {
            h1.high = h2.low = 0;
            h2.high = 0x87654321;
            scalartest_op7 (h, h1, &h2);
            TSHMessage (ctx, MOTRACE,"h1 (%ld,%ld) ? h2 (%ld,%ld) - 1\n",
                                              h1.high, h1.low, h2.high, h2.low);
            if (h1.low != h2.low - 1 || h2.high != 0) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage (ctx, MOWARNING,"h1 (%ld,%ld) != h2 (%ld,%ld) - 1\n",
                                              h1.high, h1.low, h2.high, h2.low);
                return(TSH_INCORRECT_DATA);
            }
            h1.low += 0xFFF00;
        }
    
        h1.high = 0;          h1.low = 0xFFFFFFFF;
        h2.high = 0x87654321; h2.low = 0;
        scalartest_op7 (h, h1, &h2);
        TSHMessage (ctx, MOTRACE," h1 (%ld,%ld) ? h2 (%ld,%ld) - 1\n",
                                              h1.high, h1.low, h2.high, h2.low);
        if (h2.low != 0 || h2.high != 1) 
        {
            TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
            TSHMessage (ctx, MOWARNING," h1 (%ld,%ld) != h2 (%ld,%ld) - 1\n",
                                              h1.high, h1.low, h2.high, h2.low);
            return(TSH_INCORRECT_DATA);
        }
        break;
    
        case SCALAR_UHYPER_INT:
        /**********************************************************************/
        /* test scalartest_op8: pass in an unsigned hyper int, add 1 to it and*/
        /* pass it back in a separate slot.                                   */
        /**********************************************************************/
        uh1.low = 0;
        for (i = 0; i < 20; i++) 
        {
            uh1.high = uh2.low = 0;
            uh2.high = 0x87654321;
            scalartest_op8 (h, uh1, &uh2);
            TSHMessage(ctx,MOTRACE,"uh1 (%ld,%ld) ? uh2 (%ld,%ld) - 1\n",
                                          uh1.high, uh1.low, uh2.high, uh2.low);
            if (uh1.low != uh2.low - 1 || uh2.high != 0) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage(ctx,MOWARNING,"uh1 (%ld,%ld) != uh2 (%ld,%ld) - 1\n",
                                          uh1.high, uh1.low, uh2.high, uh2.low);
                return(TSH_INCORRECT_DATA);
            }
            uh1.low += 0xFFF00;
        }
    
        uh1.high = 0;          uh1.low = 0xFFFFFFFF;
        uh2.high = 0x87654321; uh2.low = 0;
        scalartest_op8 (h, uh1, &uh2);
        TSHMessage(ctx,MOTRACE," uh1 (%ld,%ld) ? uh2 (%ld,%ld) - 1\n",
                                          uh1.high, uh1.low, uh2.high, uh2.low);
        if (uh2.low != 0 || uh2.high != 1) 
        {
            TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
            TSHMessage(ctx,MOWARNING," uh1 (%ld,%ld) != uh2 (%ld,%ld) - 1\n",
                                          uh1.high, uh1.low, uh2.high, uh2.low);
            return(TSH_INCORRECT_DATA);
        }
        break;

#else
        case SCALAR_HYPER_INT:
        /**********************************************************************/
        /* test scalartest_op7: pass in a hyper int, add 1 to it and pass it  */
        /* back in a separate slot.                                           */
        /**********************************************************************/
        for (i = 0, h1 = 0; i < 20; i++) 
        {
            h2 = 0;
            scalartest_op7 (h, h1, &h2);
            TSHMessage(ctx,MOTRACE,"h1 (%ld) ? h2 (%ld) - 1\n", h1, h2);
            if (h1 != h2 - 1) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage(ctx,MOWARNING,"h1 (%ld) != h2 (%ld) - 1\n", h1, h2);
                return(TSH_INCORRECT_DATA);
            }
            h1 += 0xFFFF0000;
        }
        break;
    
        case SCALAR_UHYPER_INT:
        /**********************************************************************/
        /* test scalartest_op8: pass in an unsigned hyper int, add 1 to it and*/
        /* pass it back in a separate slot.                                   */
        /**********************************************************************/
        for (i = 0, uh1 = 0; i < 20; i++) 
        {
            uh2 = 0;
            scalartest_op8 (h, uh1, &uh2);
            TSHMessage(ctx,MOTRACE," uh1 (%ld) ? uh2 (%ld) - 1\n", uh1, uh2);
            if (uh1 != uh2 - 1) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage(ctx,MOWARNING," uh1 (%ld) != uh2 (%ld) - 1\n", 
                                                                      uh1, uh2);
                return(TSH_INCORRECT_DATA);
            }
            uh1 += 0xFFFF0000;
        }
        break;
#endif
    
        case SCALAR_BOOLEAN:
        /**********************************************************************/
        /* test scalartest_op9: pass in a boolean and pass it back negated.   */
        /**********************************************************************/
        bool1 = ndr_true;
        for (i = 0; i < 2; i++) 
        {
            bool2 = bool1;
            scalartest_op9 (h, bool1, &bool2);
            TSHMessage(ctx,MOTRACE,"bool1(%c) ? bool2(%c) : !bool2\n",
                         bool1 ? 't' : 'f', bool2 ? 't' : 'f');
            if (bool1 ? bool2 : !bool2) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage(ctx,MOWARNING,"bool1(%c) ? bool2(%c) : !bool2\n",
                         bool1 ? 't' : 'f', bool2 ? 't' : 'f');
                return(TSH_INCORRECT_DATA);
            }
            bool1 = !bool1;
        }
        break;
    
        case SCALAR_BYTE:
        /**********************************************************************/
        /* test scalartest_op10: pass in a byte and pass it back inverted.    */
        /**********************************************************************/
        for (i = 0, byte1 = 0; i < 256; i++) 
        {
            byte2 = byte1;
            scalartest_op10 (h, byte1, &byte2);
            TSHMessage(ctx,MOTRACE," byte1 (%d) ^ byte2 (%d) ? 0xFF\n",
                                                                  byte1, byte2);
            if ((byte1 ^ byte2) != 0xFF) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage(ctx,MOWARNING," byte1 (%d) ^ byte2 (%d) != 0xFF\n",
                                                                  byte1, byte2);
                return(TSH_INCORRECT_DATA);
            }
            byte1 +=1;
        }
        break;
    
        case SCALAR_FLOAT:
        /**********************************************************************/
        /* test scalartest_op11: pass in a float and pass it back squared.    */
        /**********************************************************************/
        for (i = 0, sf1 = 0.1; i < 20; i++) 
        {
            sf2 = sf1;
            scalartest_op11 (h, sf1, &sf2);
            TSHMessage(ctx,MOTRACE," sf1(%f) ** 2 ? sf2 (%f)\n",sf1,sf2);
            if (!close_sf (sf1 * sf1, sf2)) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage(ctx,MOWARNING," sf1(%f) ** 2 != sf2 (%f)\n",sf1,sf2);
                return(TSH_INCORRECT_DATA);
            }
            sf1 *= 2.3;
        }
        break;
    
        case SCALAR_DOUBLE:
        /**********************************************************************/
        /* test scalartest_op12: pass in a double and pass it back squared.   */
        /**********************************************************************/
        for (i = 0, lf1 = 0.1; i < 20; i++) 
        {
            lf2 = lf1;
            scalartest_op12 (h, lf1, &lf2);
            TSHMessage(ctx,MOTRACE," lf1(%f) ** 2 ? lf2 (%f)\n",lf1,lf2);
            if (!close_lf (lf1 * lf1, lf2)) 
            {
                TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
                TSHMessage(ctx,MOWARNING," lf1(%f) ** 2 != lf2 (%f)\n",lf1,lf2);
                return(TSH_INCORRECT_DATA);
            }
            lf1 *= 2.3;
        }
        break;
    
        case SCALAR_CHAR:
        /**********************************************************************/
        /* test scalartest_op13: pass in a char and pass it back case reversed*/
        /* if alphabetic, unchanged otherwise.                                */
        /**********************************************************************/
        char1 = 'D';
        scalartest_op13 (h, char1, &char1);
        TSHMessage(ctx,MOTRACE," char1 (%c) ? 'd'\n", char1);
        if (char1 != 'd') 
        {
            TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
            TSHMessage(ctx,MOWARNING," char1 (%c) != 'd'\n", char1);
            return(TSH_INCORRECT_DATA);
        }
    
        scalartest_op13 (h, '$', &char1);
        TSHMessage(ctx,MOTRACE,"char1 (%c) ? '$'\n", char1);
        if (char1 != '$') 
        {
            TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
            TSHMessage(ctx,MOWARNING,"char1 (%c) != '$'\n", char1);
            return(TSH_INCORRECT_DATA);
        }
    
        scalartest_op13 (h, 'z', &char1);
        TSHMessage(ctx,MOTRACE,"char1 (%c) ? 'Z'\n", char1);
        if (char1 != 'Z') 
        {
            TSHStatus(ctx, TEST,"incorrect data returned",FAILURE,SUCCESS);
            TSHMessage(ctx,MOWARNING,"char1 (%c) != 'Z'\n", char1);
            return(TSH_INCORRECT_DATA);
        }
        break;

        default:
        /**********************************************************************/
        /* If an unrecognized subcase was entered.                            */
        /**********************************************************************/
        TSHMessage(ctx,MOFAILURE,"Unrecognized subcase %d\n", subcase);
        return(TSH_UNKNOWN_SUBCASE);
    }
    return(SUCCESS);
}
