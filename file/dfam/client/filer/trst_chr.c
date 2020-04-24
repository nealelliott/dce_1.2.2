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
 * $Log: trst_chr.c,v $
 * Revision 1.1.2.2  1996/03/11  14:06:46  marty
 * 	Update OSF copyright years.
 * 	[1996/03/11  13:56:22  marty]
 *
 * Revision 1.1.2.1  1996/02/21  05:26:07  takano
 * 	First Submit
 * 	[1996/02/21  05:25:59  takano]
 * 
 * $EndLog$
 */
#include "cli_filr.h"

#define RIGHT_CLEAR "         "

struct  {
    int     pot;
    char    moji1;
    char    moji2;
}   chk_tbl[] =

    {   1,  'r',    'R',
        2,  'w',    'W',
        3,  'c',    'C',
        4,  'e',    'E',
        5,  'm',    'M',
        6,  'f',    'F',
        7,  'a',    'A',
        8,  'q',    'Q',
        -1, ' ',    ' ' };

// EnglishComment
/*****************************************************************************/
/*  ID   = trst_chr()                                                        */
/*  name = Character string sorting                                          */
/*  func = Sorts the character strings of the trustee rights.                */
/*            Gets the character strings of the trustee rights from Gateway, */
/*            and sorts them in the order of [ rwcemfaq].                    */
/*                                                                           */
/*  i/o  = ot_right:char *: trustee rights character strings (after editing) */
/*         in_right:char *: trustee rights character strings (before editing)*/
/*                                                                           */
/*  return = none                                                            */
/*  common = none                                                            */
/*  note = none                                                              */
/*  date = May 30, 1995 by Kentarou Chikura                                  */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
void trst_chr( char *ot_right, char *in_right )
{
    int i;

    strcpy( ot_right, RIGHT_CLEAR );
    for( i=0 ;
         chk_tbl[i].pot != -1 ;
         i++ ) {
        if( strchr( in_right,
                chk_tbl[i].moji1 ) != NULL) {
            ot_right[chk_tbl[i].pot] =
                chk_tbl[i].moji2;
        }
        if( strchr( in_right,
                chk_tbl[i].moji2 ) != NULL ) {
            ot_right[chk_tbl[i].pot] =
                chk_tbl[i].moji2;
        }
    }
}
