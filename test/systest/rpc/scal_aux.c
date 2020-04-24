/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: scal_aux.c,v $
 * Revision 1.1.10.2  1996/03/09  20:51:31  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:29:17  marty]
 *
 * Revision 1.1.10.1  1995/12/13  21:55:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/13  21:45:21  root]
 * 
 * Revision 1.1.6.2  1993/07/09  13:15:50  root
 * 	Initial King Kong branch
 * 	[1993/07/09  13:15:17  root]
 * 
 * Revision 1.1.4.2  1993/03/31  12:35:36  mgm
 * 	Initial GAMERA branch
 * 
 * Revision 1.1.3.2  1992/06/11  19:57:57  mcfarland
 * 	changed files from test_latest
 * 	[1992/06/09  13:05:03  mcfarland]
 * 
 * 	Reorganize util routines
 * 	[1992/04/15  16:00:52  rps]
 * 
 * Revision 1.1.1.2  1992/05/28  15:14:34  root
 * 	    Loading drop Dce1_0_1b15
 * 
 * Revision 1.1  1992/01/19  14:36:19  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
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
/* @(#)scalc.c	1.1
 *  NIDL TEST 
 *	Extracted	1/31/91 16:32:59
 *	from delta on	1/31/91 16:30:57
 */
#ifndef LINT
static char sccs_id[] = "@(#)scalc.c	1.1";
#endif

#include <scal.h>
#include <scalt.h>
#include <stdio.h>
#include <math.h>
#include <dce/rpc.h> 
#include <scal_util.h>
#include <util.h>
extern char header[512], buf[128];

int scalars_test (h)
handle_t h;
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


        /**********************************************************************/
        /* test scalartest_op1: pass in a small int, add 1 to it and pass it  */
        /* back in a separate slot.                                           */
        /**********************************************************************/
        for (i = 0, sm1 = 0; i < 20; i++) 
        {
            sm2 = 0;
            scalartest_op1 (h, sm1, &sm2);
            sprintf (buf, "sm1 (%d) ? sm2 (%ld) - 1\n", sm1, sm2);
	    xx_log(buf);
            if (sm1 != sm2 - 1) 
            {
                sprintf(buf, "%sincorrect data returned", header);
		xx_error(buf);
                sprintf (buf, "sm1 (%d) != sm2 (%ld) - 1\n", sm1, sm2);
	 	xx_error(buf);
                return(1);
            }
            sm1 += 23;
        }
    
        /**********************************************************************/
        /* test scalartest_op2: pass in an unsigned small int, add 1 to it    */
        /* and pass it back in a separate slot.                               */
        /**********************************************************************/
        for (i = 0, usm1 = 0; i < 20; i++) 
        {
            usm2 = 0;
            scalartest_op2 (h, usm1, &usm2);
            sprintf (buf, "usm1 (%d) ? usm2 (%ld) - 1\n",usm1, usm2);
	    xx_log(buf);
            if (usm1 != usm2 - 1) 
            {
                sprintf(buf, "%sincorrect data returned", header);
		xx_error(buf);
                sprintf (buf, "usm1 (%d) != usm2 (%ld) - 1\n", usm1, usm2);
		xx_error(buf);
                return(1);
            }
            usm1 += 23;
        }
    
        /**********************************************************************/
        /* test scalartest_op3: pass in a short int, add 1 to it and pass it  */
        /* back in a separate slot.                                           */
        /**********************************************************************/
        for (i = 0, sh1 = 0; i < 20; i++) 
        {
            sh2 = 0;
            scalartest_op3 (h, sh1, &sh2);
            sprintf (buf, "sh1 (%d) ? sh2 (%ld) - 1\n", sh1, sh2);
	    xx_log(buf);
            if (sh1 != sh2 - 1) 
            {
                sprintf(buf, "%sincorrect data returned", header);
		xx_error(buf);
                sprintf (buf,  "sh1 (%d) != sh2 (%ld) - 1\n", sh1, sh2);
		xx_error(buf);
                return(1);
            }
            sh1 += 23*256+23;
        }
    
        /**********************************************************************/
        /* test scalartest_op4: pass in an unsigned short int, add 1 to it and*/
        /* pass it back in a separate slot.                                   */
        /**********************************************************************/
        for (i = 0, ush1 = 0; i < 20; i++) 
        {
            ush2 = 0;
            scalartest_op4 (h, ush1, &ush2);
            sprintf (buf, "ush1 (%d) ? ush2 (%ld) - 1\n", ush1, ush2);
	    xx_log(buf);
            if (ush1 != ush2 - 1) 
            {
                sprintf(buf, "%sincorrect data returned", header);
		xx_error(buf);
                sprintf (buf, "ush1 (%d) != ush2 (%ld) - 1\n", ush1, ush2);
		xx_error(buf);
                return(1);
            }
            ush1 += 23*256+23;
        }
    
        /**********************************************************************/
        /* test scalartest_op5: pass in a long int, add 1 to it and pass it   */
        /* back in a separate slot.                                           */
        /**********************************************************************/
        for (i = 0, l1 = 0; i < 20; i++) 
        {
            l2 = 0;
            scalartest_op5 (h, l1, &l2);
            sprintf (buf, "l1 (%ld) ? l2 (%ld) - 1\n",l1, l2);
	    xx_log(buf);
            if (l1 != l2 - 1) 
            {
                sprintf(buf, "%sincorrect data returned", header);
		xx_error(buf);
                sprintf (buf, "l1 (%ld) != l2 (%ld) - 1\n",l1, l2);
		xx_error(buf);
                return(1);
            }
            l1 += 123456789;
        }
    
        /**********************************************************************/
        /* test scalartest_op6: pass in an unsigned long int, add 1 to it and */
        /* pass it back in a separate slot.                                   */
        /**********************************************************************/
        for (i = 0, ul1 = 0; i < 20; i++) 
        {
            ul2 = 0;
            scalartest_op6 (h, ul1, &ul2);
            sprintf (buf, "ul1 (%ld) ? ul2 (%ld) - 1\n", ul1, ul2);
	    xx_log(buf);
            if (ul1 != ul2 - 1) 
            {
                sprintf(buf, "%sincorrect data returned", header);
		xx_error(buf);
                sprintf (buf, "ul1 (%ld) != ul2 (%ld) - 1\n", ul1, ul2);
		xx_error(buf);
                return(1);
            }
            ul1 += 123456789;
        }
    
#ifndef cray
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
            sprintf (buf, "h1 (%ld,%ld) ? h2 (%ld,%ld) - 1\n",
                                              h1.high, h1.low, h2.high, h2.low);
	    xx_log(buf);
            if (h1.low != h2.low - 1 || h2.high != 0) 
            {
                sprintf(buf, "%sincorrect data returned", header);
		xx_error(buf);
                sprintf (buf, "h1 (%ld,%ld) != h2 (%ld,%ld) - 1\n",
                                              h1.high, h1.low, h2.high, h2.low);
		xx_error(buf);
                return(1);
            }
            h1.low += 0xFFF00;
        }
    
        h1.high = 0;          h1.low = 0xFFFFFFFF;
        h2.high = 0x87654321; h2.low = 0;
        scalartest_op7 (h, h1, &h2);
        sprintf (buf, " h1 (%ld,%ld) ? h2 (%ld,%ld) - 1\n",
                                              h1.high, h1.low, h2.high, h2.low);
	    xx_log(buf);
        if (h2.low != 0 || h2.high != 1) 
        {
                sprintf(buf, "%sincorrect data returned", header);
		xx_error(buf);
            sprintf (buf, " h1 (%ld,%ld) != h2 (%ld,%ld) - 1\n",
                                              h1.high, h1.low, h2.high, h2.low);
	    xx_error(buf);
            return(1);
        }
    
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
            sprintf(buf,"uh1 (%ld,%ld) ? uh2 (%ld,%ld) - 1\n",
                                          uh1.high, uh1.low, uh2.high, uh2.low);
	    xx_log(buf);
            if (uh1.low != uh2.low - 1 || uh2.high != 0) 
            {
                sprintf(buf, "%sincorrect data returned", header);
		xx_error(buf);
                sprintf(buf,"uh1 (%ld,%ld) != uh2 (%ld,%ld) - 1\n",
                                          uh1.high, uh1.low, uh2.high, uh2.low);
		xx_error(buf);
                return(1);
            }
            uh1.low += 0xFFF00;
        }
    
        uh1.high = 0;          uh1.low = 0xFFFFFFFF;
        uh2.high = 0x87654321; uh2.low = 0;
        scalartest_op8 (h, uh1, &uh2);
        sprintf(buf," uh1 (%ld,%ld) ? uh2 (%ld,%ld) - 1\n",
                                          uh1.high, uh1.low, uh2.high, uh2.low);
	    xx_log(buf);
        if (uh2.low != 0 || uh2.high != 1) 
        {
                sprintf(buf, "%sincorrect data returned", header);
		xx_error(buf);
            sprintf(buf," uh1 (%ld,%ld) != uh2 (%ld,%ld) - 1\n",
                                          uh1.high, uh1.low, uh2.high, uh2.low);
		xx_error(buf);
            return(1);
        }

#else
        /**********************************************************************/
        /* test scalartest_op7: pass in a hyper int, add 1 to it and pass it  */
        /* back in a separate slot.                                           */
        /**********************************************************************/
        for (i = 0, h1 = 0; i < 20; i++) 
        {
            h2 = 0;
            scalartest_op7 (h, h1, &h2);
            sprintf(buf,"h1 (%ld) ? h2 (%ld) - 1\n", h1, h2);
	    xx_log(buf);
            if (h1 != h2 - 1) 
            {
                sprintf(buf, "%sincorrect data returned", header);
		xx_error(buf);
                sprintf(buf,"h1 (%ld) != h2 (%ld) - 1\n", h1, h2);
		xx_error(buf);
                return(1);
            }
            h1 += 0xFFFF0000;
        }
    
        /**********************************************************************/
        /* test scalartest_op8: pass in an unsigned hyper int, add 1 to it and*/
        /* pass it back in a separate slot.                                   */
        /**********************************************************************/
        for (i = 0, uh1 = 0; i < 20; i++) 
        {
            uh2 = 0;
            scalartest_op8 (h, uh1, &uh2);
            sprintf(buf," uh1 (%ld) ? uh2 (%ld) - 1\n", uh1, uh2);
	    xx_log(buf);
            if (uh1 != uh2 - 1) 
            {
                sprintf(buf, "%sincorrect data returned", header);
		xx_error(buf);
                sprintf(buf," uh1 (%ld) != uh2 (%ld) - 1\n", uh1, uh2);
		xx_error(buf);
                return(1);
            }
            uh1 += 0xFFFF0000;
        }
#endif
    
        /**********************************************************************/
        /* test scalartest_op9: pass in a boolean and pass it back negated.   */
        /**********************************************************************/
        bool1 = ndr_true;
        for (i = 0; i < 2; i++) 
        {
            bool2 = bool1;
            scalartest_op9 (h, bool1, &bool2);
            sprintf(buf,"bool1(%c) ? bool2(%c) : !bool2\n",
                         bool1 ? 't' : 'f', bool2 ? 't' : 'f');
	    xx_log(buf);
            if (bool1 ? bool2 : !bool2) 
            {
                sprintf(buf, "%sincorrect data returned", header);
		xx_error(buf);
                sprintf(buf,"bool1(%c) ? bool2(%c) : !bool2\n",
                         bool1 ? 't' : 'f', bool2 ? 't' : 'f');
		xx_error(buf);
                return(1);
            }
            bool1 = !bool1;
        }
    
        /**********************************************************************/
        /* test scalartest_op10: pass in a byte and pass it back inverted.    */
        /**********************************************************************/
        for (i = 0, byte1 = 0; i < 256; i++) 
        {
            byte2 = byte1;
            scalartest_op10 (h, byte1, &byte2);
            sprintf(buf," byte1 (%d) ^ byte2 (%d) ? 0xFF\n",
                                                                  byte1, byte2);
	    xx_log(buf);
            if ((byte1 ^ byte2) != 0xFF) 
            {
                sprintf(buf, "%sincorrect data returned", header);
		xx_error(buf);
                sprintf(buf," byte1 (%d) ^ byte2 (%d) != 0xFF\n", byte1, byte2);
		xx_error(buf);
                return(1);
            }
            byte1 +=1;
        }
    
        /**********************************************************************/
        /* test scalartest_op11: pass in a float and pass it back squared.    */
        /**********************************************************************/
        for (i = 0, sf1 = 0.1; i < 20; i++) 
        {
            sf2 = sf1;
            scalartest_op11 (h, sf1, &sf2);
            sprintf(buf," sf1(%f) ** 2 ? sf2 (%f)\n",sf1,sf2);
	    xx_log(buf);
            if (!close_sf (sf1 * sf1, sf2)) 
            {
                sprintf(buf, "%sincorrect data returned", header);
		xx_error(buf);
                sprintf(buf," sf1(%f) ** 2 != sf2 (%f)\n",sf1,sf2);
		xx_error(buf);
                return(1);
            }
            sf1 *= 2.3;
        }
    
        /**********************************************************************/
        /* test scalartest_op12: pass in a double and pass it back squared.   */
        /**********************************************************************/
        for (i = 0, lf1 = 0.1; i < 20; i++) 
        {
            lf2 = lf1;
            scalartest_op12 (h, lf1, &lf2);
            sprintf(buf," lf1(%f) ** 2 ? lf2 (%f)\n",lf1,lf2);
	    xx_log(buf);
            if (!close_lf (lf1 * lf1, lf2)) 
            {
                sprintf(buf, "%sincorrect data returned", header);
		xx_error(buf);
                sprintf(buf," lf1(%f) ** 2 != lf2 (%f)\n",lf1,lf2);
		xx_error(buf);
                return(1);
            }
            lf1 *= 2.3;
        }
    
        /**********************************************************************/
        /* test scalartest_op13: pass in a char and pass it back case reversed*/
        /* if alphabetic, unchanged otherwise.                                */
        /**********************************************************************/
        char1 = 'D';
        scalartest_op13 (h, char1, &char1);
        sprintf(buf," char1 (%c) ? 'd'\n", char1);
	    xx_log(buf);
        if (char1 != 'd') 
        {
                sprintf(buf, "%sincorrect data returned", header);
		xx_error(buf);
            sprintf(buf," char1 (%c) != 'd'\n", char1);
		xx_error(buf);
            return(1);
        }
    
        scalartest_op13 (h, '$', &char1);
        sprintf(buf,"char1 (%c) ? '$'\n", char1);
	    xx_log(buf);
        if (char1 != '$') 
        {
                sprintf(buf, "%sincorrect data returned", header);
		xx_error(buf);
            sprintf(buf,"char1 (%c) != '$'\n", char1);
		xx_error(buf);
            return(1);
        }
    
        scalartest_op13 (h, 'z', &char1);
        sprintf(buf,"char1 (%c) ? 'Z'\n", char1);
	    xx_log(buf);
        if (char1 != 'Z') 
        {
            sprintf(buf, "%sincorrect data returned", header);
	    xx_error(buf);
            sprintf(buf,"char1 (%c) != 'Z'\n", char1);
	    xx_error(buf);
            return(1);
        }

    return(0);
}
