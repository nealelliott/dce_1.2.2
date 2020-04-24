#include "dfam_cli.h"
// EnglishComment
/*****************************************************************************/
/*  ID   = ps_input()                                                        */
/*  name = Inputting password                                                */
/*  func = Gets password for DSETPASS.EXE                                    */
/*          - Suppresses password display.                                   */
/*  i/o  = pass:char *:OUT: entered password                                 */
/*         len :int   :INP: the inputted field length                        */
/*                                                                           */
/*  return = >= 0: normal end                                                */
/*           -1  : shortage of memory                                        */
/*  common = none                                                            */
/*  note = This function gets a mapping area for Kanji checking when a       */
/*         password in entered.                                              */
/*  date = October 27, 1994 by Kentarou Chikura                              */
/*                                                                           */
/* Trade Secret of Hitachi, Ltd.                                             */
/* Do not disclose without written concession from Hitachi, Ltd.             */
/* Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados.  */
/*****************************************************************************/
// EnglishComment
int ps_input( char *pass, int len )
{
    int     i;
    BYTE    rc;                                                 
    char    *map;

    if ( (map = (char *)malloc( len+1 ) ) == NULL ) {
        return(-1);
    }
    memset ( map, 0x00, len+1 );
    rc = 0x00;
    i = 0;
    while( rc != 0x0d ) {
        rc = (BYTE)_getch();
        if( i < len ) {
            if( rc == 0x00 ) {
                rc = (BYTE)_getch();
            } else if ( rc == 0x08 ) {
                switch ( i ) {
                    case 0:
                        break;
                    case 1:
                        i = 0;
                        pass[i] = map[i] = 0x20;
                        printf( "\x08\x20\x08" );
                        break;
                    default:
                        if ( map[i-2] == '1' && map[i-1] == '2' ) {
                            i = i - 2;
                            pass[i] = pass[i+1] =
                             map[i] = map[i+1]  = 0x20;
                            printf( "\x08\x20\x08\x08\x20\x08" );
                        } else {
                            i --;
                            pass[i] = map[i] = 0x20;
                            printf( "\x08\x20\x08" );
                        }
                        break;
                }
            } else if ( rc == 0x0d ) {
                pass[i] = 0x00;
            } else if( rc >= 0x01 && rc <= 0x1f ) {
                ;
            } else {
                pass[i] = rc;                                   
                map[i] = 'c';                                   
                i++;                                            
                printf( "*" );                                  
            }
        } else {
            if ( rc == 0x08 ) {
                if ( map[i-2] == '1' && map[i-1] == '2' ) {     
                    i = i - 2;                          
                    pass[i] = pass[i+1] =                       
                     map[i] = map[i+1]  = 0x20;                 
                    printf( "\x08\x20\x08\x08\x20\x08" );       
                } else {                                
                    i --;                                       
                    pass[i] = map[i] = 0x20;                    
                    printf( "\x08\x20\x08" );                   
                }                                               
            }
        }
    }
    free( map );
    return(i);
}

