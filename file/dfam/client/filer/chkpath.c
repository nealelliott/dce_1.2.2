#include < windows.h >
#include < stdio.h >
#include < string.h >
#include < stdlib.h >
#include < nwcaldef.h >
#include < nwdpath.h >
#include < nwcalls.h >
#include < nwncpext.h >
#include < direct.h >

#include "gfiler.h"
#include "err.h"
#include "prot.h"

char Name[33];
DWORD ncpexId;
NWCONN_HANDLE connHandle;
unsigned short getncpid = 0;

// EnglishComment
/*****************************************************************************/
/*  ID   = initialize                                                        */
/*  name = Communication initializing function                               */
/*  func = Initializes communication for NCPExtension.                       */
/*  i/o    = none                                                            */
/*  return = TRUE : normal end                                               */
/*           FALSE: abnormal end                                             */
/*  common = none                                                            */
/*  note   = none                                                            */
/*                                                                           */
/*  date   = December 16, 1994 Kousuke Takano                                */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
unsigned short initialize(){
    NWCCODE ccode;


    ccode = NWCallsInit( NULL , NULL );
    if( ccode != 0 ){
        return FALSE;
    }
    return TRUE;
}

// EnglishComment
/*****************************************************************************/
/*  ID   = selectNCPext                                                      */
/*  name = Obtaining NCPExtensionID function                                 */
/*  func = NGets CPExtensionID.                                              */
/*  i/o  = conn      :NWCONN_HANDLE:IN : connection handle                   */
/*         ncpexName : char *      :IN : NCPExtension name                   */
/*  return = TRUE : normal end                                               */
/*           FALSE: abnormal end                                             */
/*  common = getncpid: :OUT:  0: failed to get                               */
/*                   : :   :  1: successfully got                            */
/*  note   = none                                                            */
/*                                                                           */
/*  date   = December 16, 1994 Kousuke Takano                                */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
unsigned short selectNCPext( NWCONN_HANDLE conn , char *ncpexName ){
    NWCCODE ccode;

    strcpy( Name , ( LPCSTR )ncpexName );
    ccode = NWGetNCPExtensionInfoByName ( conn , Name , & ncpexId , NULL , NULL , NULL , NULL );
    if ( ccode != 0 ){
        getncpid = 0;
        return FALSE;
    }
    getncpid = 1;
    return TRUE;
}

// EnglishComment
/*****************************************************************************/
/*  ID   = chkpath                                                           */
/*  name = Converting the mapped path into NetWare path                      */
/*  func = Converts the inputted path (ent_path) into the NetWare full       */
/*         path (ext_path).                                                  */
/*  i/o  = ent_path:char *:IN : path file name                               */
/*             :      :   : (e.g.) DFAM-1\DEV1:TAKANO\TEST.TXT               */
/*         ext_path:char *:OUT: file path name converted into NetWare style  */
/*             :      :   :                                                  */
/*             :      :   :                                                  */
/*                                                                           */
/*  return = NORMAL    : normal end                                          */
/*           INVALENT  : syntax error in the path name                       */
/*           LOCALDRIVE: a local drive is designated                         */
/*           NOSERVER  : no server                                           */
/*           MEMERR    : memory shortage                                     */
/*  common = none                                                            */
/*  note   = (1) The wildcard in a password is not supported.                */
/*           (2) The format of the input path is not strictly specified.     */
/*               If a drive name or volume name is omitted, the current      */
/*               directory is assumed.                                       */
/*                                                                           */
/*  date   = February 3, 1995 Kousuke Takano                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
short chkpath( char *ent_path , char *ext_path ){

    unsigned char *tmp_path;
    unsigned char *pos_slush;
    unsigned char *pos_yen;
    unsigned char *pos_path;
    unsigned char *pos_dot;
    unsigned char *pos_aster;

    NWCCODE ccode;
    char server[SERVLEN];
    char volName[VOLLEN];
    char path[NW_PATHSIZE];
    char buff[NW_PATHSIZE];


    if ( ent_path[0] == ':' ){
        return( INVALENT );
    }
    if ( strstr( ent_path , "..." ) != NULL ){
        return( INVALENT );
    }
    if ( strstr( ent_path , "\\\\" ) != NULL || strstr( ent_path , "//" ) != NULL ){
        return( INVALENT );
    }

    if ( strcmp( ent_path , CURRENT ) == 0 ){

        _getcwd( buff , NW_PATHSIZE );
    }
    else {
        strcpy( buff , ent_path );
    }
    strupr( buff );
    ccode = NWParsePath( buff , server , &connHandle , volName , path );

    if( connHandle == 0 && strcmp( server , "" ) == 0 ){

        return( LOCALDRIVE );
    }

    if( ccode != SUCCESSFUL ){

        return( NOSERVER );
    }

    tmp_path = ( char * )calloc( 1 , NW_PATHSIZE );

    if( tmp_path == NULL ) {
        return( MEMERR );
    }

    strcat( tmp_path , server );
    strcat( tmp_path , "\\" );
    strcat( tmp_path , volName );
    strcat( tmp_path , ":" );



    pos_slush = strrchr( path , '/' );
    pos_yen = strrchr( path , '\\' );
    pos_dot = strrchr( path , '.' );
    pos_aster = strrchr( path , '*' );

    pos_path = ( pos_slush > pos_yen )  ? pos_slush : pos_yen ;


    if( ( pos_path > pos_dot ) && ( pos_aster > pos_path ) ){

            strcat( path , ".*" );
    }
    if( ( pos_path == NULL ) && ( pos_aster != NULL ) && ( pos_dot == NULL ) ){

            strcat( path , ".*" );
    }

    if( path[ strlen( path ) - 1 ] == '.' && path[ strlen( path ) - 2 ] != '\\' ){
        path[ strlen( path ) - 1 ] = '\0';
    }

    strcat( tmp_path , path );


    strcpy( ext_path , tmp_path );

    free( tmp_path );

    return( NORMAL );
}

// EnglishComment
/*****************************************************************************/
/*  ID   = ncpaccess                                                         */
/*  name = Access processing function                                        */
/*  func = Establishes connection to Gateway with using an NCPExtension      */
/*         handler.                                                          */
/*  i/o  = sendData          :VOID *  :IN : structure of sending data        */
/*         sendDataLength    :WORD    :IN : length of sending data           */
/*         recieveData       :VOID *  :OUT: structure of received data       */
/*         recieveDataLength :WORD *  :OUT: length of received data          */
/*  return = TRUE : normal end                                               */
/*           FALSE: abnormal end                                             */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = February 3, 1995 Kousuke Takano                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
unsigned short ncpaccess( void *sendData , WORD sendDataLength , void *recieveData , WORD *recieveDataLength ){

    NWCCODE ccode;

    ccode = NWNCPExtensionRequest( connHandle , ncpexId , sendData , sendDataLength , recieveData , recieveDataLength );

    if ( ccode != 0 ){
        return FALSE;
    }
    return TRUE;
}

// EnglishComment
/*****************************************************************************/
/*  ID   = isnwvol                                                           */
/*  name = Testing NetWare function                                          */
/*  func = Checks if the designated volume is a NetWare volume.              */
/*            an internal function of chkvol.                                */
/*  i/o  = conn    :NWCONN_HANDLE:IN: connection handle                      */
/*         vol_name:char *       :IN: volume name                            */
/*  return = TRUE : NetWare Volume (including DFAM Volume)                   */
/*           FALSE: other volumes                                            */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = February 3, 1995 Kousuke Takano                                 */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment
unsigned short isnwvol( NWCONN_HANDLE conn , char *vol_name ){


    NWCCODE ccode;
    NWVOL_NUM vol_num;

     ccode = NWGetVolumeNumber( conn , vol_name , &vol_num );


    switch( ccode ){
        case SUCCESSFUL:
            break;
        case INVALID_CONNECTION:
        case SERVER_OUT_OF_MEMORY:
        default:
            return FALSE;
    }
    return TRUE;
}
