/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: parm_pru.c,v $
 * Revision 1.1.2.2  1996/03/11  14:07:01  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:33  marty]
 *
 * Revision 1.1.2.1  1996/02/21  05:51:27  takano
 * 	First Submit
 * 	[1996/02/21  05:51:21  takano]
 * 
 * $EndLog$
 */
#include "dfam_cli.h"
// EnglishComment
/*****************************************************************************/
/*  ID   = parm_pru()                                                        */
/*                                                                           */
/*  name = Parameter pruning                                                 */
/*  func = Takes off "" (double-quotation) and " " (quotation plus space)    */
/*         from the command parameters.                                      */
/*                                                                           */
/*  i/o  = in_c :int      :INP: argc (the number of parameters)              */
/*         io_v :char **  :INP: argv (the parameters)                        */
/*         out_v:char *** :OUT: pruned argv (parameters)                     */
/*                                                                           */
/*  return = >=0: normal (the number of pruned parameters)                   */
/*            -1: shortage of memory                                         */
/*  common = none                                                            */
/*                                                                           */
/*  note = The character string uses "argv" as it is.  However, out_v, which */
/*         uses an area allocated with malloc(), will be released after the  */
/*         process is completed.                                             */
/*                                                                           */
/*  date = October 18, 1995  by Kentarou Chikura (an added function P10-260) */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
int parm_pru( int in_c, char **in_v, char ***out_v )
{
    int     i, j, k;
    char    **v_tbl;
    char    flg;

    if( (v_tbl = (char **)malloc( sizeof(char *)*in_c )) == NULL ) {
        return( -1 );
    }
    for( i=1, j=1 ; i < in_c ; i++ )
        v_tbl[i] = (char *)NULL;
    v_tbl[0] = in_v[0];
    for( i=1, j=1 ; i < in_c ; i++ ) {
        if( in_v[i][0] != (char)0x00 ) {
            for( k=0, flg=F_OFF ; k < (int)strlen(in_v[i]) &&
                                  flg == F_OFF
                                ; k++ ) {
                if( in_v[i][k] != ' ' )
                    flg = F_ON;
            }
            if( flg == F_ON ) {
                v_tbl[j] = in_v[i];
                j++;
            }
        }
    }
    *out_v = v_tbl;
    return( j );
}
