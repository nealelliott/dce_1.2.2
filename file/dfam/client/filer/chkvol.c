#include <windows.h>
#include <stdio.h>
#include <string.h>
#include <nwcaldef.h>
#include <nwdpath.h>
#include <nwcalls.h>
#include "err.h"
#include "gfiler.h"
#include "prot.h"

#define COLON	":"

extern NWCONN_HANDLE connHandle;
extern unsigned short getncpid;		

struct chkvol_data send_data;		
struct Recieve_data{
	struct RetHeader		head;	
	struct RetChkvolData	body;	
} r_data;							

// EnglishComment
/*****************************************************************************/
/*  ID   = chkvol                                                            */
/*  name = Getting the volume type (DFAM/NetWare)                            */
/*  func = Checks whether the volume name designated in vol_name is DFAM or  */
/*         NetWare, and returns the result.  Makes an error return if the    */
/*         target does not exist in the designated volume.                   */
/*                                                                           */
/*  i/o  = vol_name:unsigned char * :IN : volume name                        */
/*         conn    :NWCONN_HANDLE   :IN : connection handle                  */
/*         type    :unsigned short *:OUT: DFAM   : DFAM Volume               */
/*                 :                :   : NETWARE: NetWare Volume            */
/*                 :                :   :                                    */
/*         err     :unsigned short *:OUT: detailed error information         */
/*                 :                :   :   NORMAL : normal end              */
/*                 :                :   :   CONERR : connection error        */
/*                 :                :   :   NOENT  : invalid name            */
/*                 :                :   :   CABNORM: internal error          */
/*  return = TRUE : normal end                                               */
/*           FALSE: abnormal end                                             */
/*                                                                           */
/*  common = none                                                            */
/*  note   = none                                                            */
/*  date   = December 16, 1994 Kousuke Takano                                */
/*                                                                           */
/*  Trade Secret of Hitachi, Ltd.                                            */
/*  Do not disclose without written concession from Hitachi, Ltd.            */
/*  Copyright 1996 Hitachi, Ltd.  All Rights Reserved.  Derechos Reservados. */
/*****************************************************************************/
// EnglishComment

unsigned short chkvol( unsigned char *vol_name , NWCONN_HANDLE conn , unsigned short *type , unsigned short *err ){
	unsigned short rtn;
										
	unsigned char *commname = "CHKV";	
	unsigned int rec_len;
	
	
	
	
	connHandle = conn;
	
	if ( getncpid == 0 ) {				
		*type = NETWARE;				
		*err = NORMAL;
		return TRUE;
	}
	
	
	if( strlen( vol_name ) > VOLLEN ){;
		*err = CABNORM;
		return FALSE;
	}
	
	
	rtn = isnwvol( conn , vol_name );
	if ( rtn == FALSE ) {
		*err = NOVOL;
		return FALSE;
	}
	
	
	memcpy( send_data.command_name , commname , strlen( commname ) );
	
	
	strcpy( send_data.vol_name , vol_name );
	
	
	rec_len = sizeof( struct Recieve_data );
	rtn = ncpaccess( &send_data , sizeof( struct chkvol_data ) , &r_data , &rec_len );
	if ( rtn == FALSE ) {
		*err = CONERR;
		return FALSE;
	}
	
	
	if ( r_data.head.ret_value == FALSE ){
		*err = r_data.head.DFAM_errno;
		return FALSE;
	}
	
	if( r_data.head.ret_value != FALSE ){
									
		switch( r_data.body.is_vol ){
									
			case DFAM:				
				*type = DFAM;
				break;
			case NETWARE:			
				*type = NETWARE;	
				break;
			default:
				*err = CABNORM;		
				return FALSE;
		}
	}
	else {							
		*err = r_data.head.DFAM_errno;
									
		return FALSE;
	}
	
	
	*err = NORMAL;
	return TRUE;					
	
}
