/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20updconfig.c,v $
 * Revision 1.1.4.2  1996/02/18  19:41:49  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:34  marty]
 *
 * Revision 1.1.4.1  1995/12/08  15:19:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:33  root]
 * 
 * Revision 1.1.2.1  1994/06/21  14:39:23  marrek
 * 	Created in June 94 code submission.
 * 	[1994/06/10  16:03:19  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20updconfig.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/02/18 19:41:49 $";
#endif


/*-----------------------------------------------------------------*/
/*                                                                 */
/* Copyright (c) 1994 Siemens Nixdorf Informationssysteme AG       */
/* All Rights Reserved                                             */
/*                                                                 */
/* Component   : DCE 1.1 GDS Administration                        */
/*               Shadow Administration                             */
/*                                                                 */
/* File Name   : d20updconfig.c                                    */
/* Description : This file contains modules that update the        */
/*               configuration when creating or removing           */
/*               shadowing jobs.                                   */
/* Date        : Mi 25 Mai 11:22:03 1994                           */
/* Author      : Jean-Marc Bonnaudet, SNI BU BA NM 123             */
/*                                                                 */
/*-----------------------------------------------------------------*/



/*-- Includes -----------------------------------------------------*/

#include <gds.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>

#include <d2dir.h>
#include <d27util.h>
#include <d2delupd.h>
#include <d23ipc.h>
#include <d20proto.h>


/*-- Defines ------------------------------------------------------*/

#define MAX_DIST_COMMAND_TIMEOUT	30	/* Maximal timeout	*/
						/* for a distributed	*/
						/* command		*/

/*-- Exported Global Variable(s) ----------------------------------*/
/*-- Imported Global Variable(s) ----------------------------------*/
/*-- Local Global Variable(s) -------------------------------------*/
/*-- Exported Function(s) -----------------------------------------*/

/* d2_ret_val d20_turn_shadowing_job( signed16, d2_upd_status ); */


/*-- Imported Function(s) -----------------------------------------*/
/*-- Local Function(s) --------------------------------------------*/

static d2_ret_val d20_turn_update( signed16, char * );
static d2_ret_val d20_send_mess( signed16, signed16 );



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d20_turn_shadowing_job                    */
/* Description         : Change the update field in gdsconfig      */
/*                       and send a message to the DSAs            */
/* Return Value        : D2_NOERROR/D2_ERROR if it succeeds/fails  */
/* Author              : Jean-Marc Bonnaudet, SNI BU BA NM 123     */
/*                                                                 */
/*-----------------------------------------------------------------*/
d2_ret_val d20_turn_shadowing_job(	signed16 current_dir_id,
					d2_upd_status new_status )
{
char current_update;			/* value read in gdsconfig     [*-]  */
char new_update_value;			/* value to write in gdsconfig [*-]  */
d2_ret_val return_value = D2_NOERROR;	/* return_value: D2_ERROR/D2_NOERROR */

    /* Search the current update */
    if( d20_check_update( current_dir_id, &current_update ) == D2_ERROR )
	return_value = D2_ERROR;
    else
    {
	/* Turn the update on/off */
	if( new_status == D2_ON && current_update == UPDATE_OFF )
	{
	    /* Update gdsconfig */
	    new_update_value = UPDATE_ON;
	    if( d20_turn_update( current_dir_id, &new_update_value )
		== D2_ERROR )
		    return_value = D2_ERROR;
	    else
		/* Turn DSA logging on */
		if( d20_send_mess( current_dir_id, D23_ENASHLOG ) == D2_ERROR )
		    return_value = D2_ERROR;
	}
	else if( new_status == D2_OFF && current_update == UPDATE_ON )
	{
	    /* Update gdsconfig */
	    new_update_value = UPDATE_OFF;
	    if( d20_turn_update( current_dir_id, &new_update_value )
		== D2_ERROR )
		    return_value = D2_ERROR;
	    else
		/* Turn DSA logging off */
		if( d20_send_mess( current_dir_id, D23_DISSHLOG ) == D2_ERROR )
		    return_value = D2_ERROR;
	}

    }

return( return_value );
}



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d20_check_update()                        */
/* Description         : Check in the config file                  */
/*                       (/opt/dcelocal/etc/gdsconfig)             */
/*                       if update information is on or off        */
/*                       for the given Directory-id                */
/* Return Value        : D2_NOERROR / D2_ERROR                     */
/* Author              : Jean-Marc Bonnaudet, SNI BU BA NM 123     */
/*                                                                 */
/*-----------------------------------------------------------------*/
d2_ret_val d20_check_update( signed16 current_dir_id, char *update )
{
char	gdsconfig_file_name[D27_LFILE_NAME];
char	line[GDSCONFIG_LINE_LEN];	/* One line of gdsconfig	*/
char	word[GDSCONFIG_LINE_LEN];	/* One word of gdsconfig	*/
char	*current_ptr;			/* A pointer on 'line'		*/
FILE	*fd;				/* File descriptor for gdsconfig*/
Bool	dir_id_found = FALSE;		/* Line for current dir-id found*/
Bool	update_found = FALSE;		/* Is value for update found ?	*/
int	return_value = D2_ERROR;	/* return value			*/


    /* Open gdsconfig */
    sprintf(gdsconfig_file_name, "%s%s%s",
	    dcelocal_path, D27_ETC_DIR, D27_GDSCONFIG);

    if( ( fd = fopen(gdsconfig_file_name, "r") ) != NULL )
    {
	/* Search the update in gdsconfig */
	while( !update_found &&
		(fgets(line, GDSCONFIG_LINE_LEN, fd) != NULL) )
	{
	    current_ptr = line;
	    while( !update_found && (current_ptr < line + strlen(line)) )
	    {
		sscanf( current_ptr, "%s", word );
		if( !strcmp( word, DIR_ID_KEY ))
		{
		    current_ptr += strlen(word) + 1;
		    sscanf( current_ptr, "%s", word );
		    if(atoi(word) == current_dir_id)
		    {
			dir_id_found = TRUE;
		    }
		}
		if( dir_id_found && !strcmp( word, UPDATE_KEY ))
		{
		    current_ptr += strlen(word) + 1;
		    sscanf( current_ptr, "%s", word );
		    *update = word[0];
		    update_found = TRUE;
		    return_value = D2_NOERROR;
		    break;
		}
		current_ptr += strlen(word) + 1;
	    }
	}

	fclose(fd);
    }

    return( return_value );
}



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d20_turn_update()                         */
/* Description         : Change the value for update in the        */
/*                       config file /opt/dcelocal/etc/gdsconfig   */
/* Return Value        : D2_NOERROR / D2_ERROR                     */
/* Author              : Jean-Marc Bonnaudet, SNI BU BA NM 123     */
/*                                                                 */
/*-----------------------------------------------------------------*/
static d2_ret_val d20_turn_update(
	signed16 current_dir_id,
	char *new_update_value )
{
char	gdsconfig_file_name[D27_LFILE_NAME],
	gdsconfig_new_file_name[D27_LFILE_NAME];
char	line[GDSCONFIG_LINE_LEN];	/* One line of gdsconfig	*/
char	word[GDSCONFIG_LINE_LEN];	/* One word of gdsconfig	*/
char	*current_ptr;			/* A pointer on 'line'		*/
FILE	*fd_in, *fd_out;		/* File descriptor for gdsconfig*/
Bool	dir_id_found = FALSE;		/* Line for current dir-id found*/
Bool	update_found = FALSE;		/* Is value for update found ?	*/
int return_value = D2_ERROR;

    /* Open gdsconfig and gdsconfig_new */
    sprintf(gdsconfig_file_name, "%s%s%s",
	    dcelocal_path, D27_ETC_DIR, D27_GDSCONFIG);
    sprintf(gdsconfig_new_file_name, "%s%s%s",
	    dcelocal_path, D27_ETC_DIR, D27_GDSCONFIG_NEW);

    if( ( fd_in = fopen(gdsconfig_file_name, "r") ) != NULL )
    {
	if( ( fd_out = fopen(gdsconfig_new_file_name, "w") ) != NULL)
	{
	    /* Search where is the update defined in gdsconfig */
	    while( (fgets(line, GDSCONFIG_LINE_LEN, fd_in) != NULL) )
	    {
		current_ptr = line;
		while( !update_found && current_ptr < line + strlen(line) )
		{
		    sscanf( current_ptr, "%s", word );
		    if( !strcmp( word, DIR_ID_KEY ) )
		    {
			current_ptr += strlen(word) + 1;
			sscanf( current_ptr, "%s", word );
			if(atoi(word) == current_dir_id)
			{
			    dir_id_found = TRUE;
			}
		    }
		    if( dir_id_found && !update_found &&
			!strcmp( word, UPDATE_KEY ))
		    {
			/* Address of the value */
			current_ptr += strlen(word) + 1;

			/* Copy ONE new character now	*/
			*current_ptr = new_update_value[0];

			/* It was found			*/
			update_found = TRUE;
			return_value = D2_NOERROR;
		    }
		    current_ptr += strlen(word) + 1;
		}
		/* Write the new line to the output file	*/
		fprintf( fd_out, "%s", line );
	    }
	    fclose( fd_out );
	}
	fclose( fd_in );

	/* Move gdsconfig.new -> gdsconfig now */
	if( rename( gdsconfig_new_file_name, gdsconfig_file_name ) == (-1) )
	    return_value = D2_ERROR;
    }

    return( return_value );
}



/*-----------------------------------------------------------------*/
/*                                                                 */
/* Function Name       : d20_send_mess()                           */
/* Description         : send a message to the DSAs to start or    */
/*                       stop the shadowing job logging            */
/* Return Value        : D2_NOERROR / D2_ERROR                     */
/* Author              : Jean-Marc Bonnaudet, SNI BU BA NM 123     */
/*                                                                 */
/*-----------------------------------------------------------------*/
static d2_ret_val d20_send_mess(
		signed16 current_dir_id,/* Current directory-id		*/
		signed16 dist_com	/* Distributed command to send	*/
		)
{
D23_dwritepb dist_com_pb;		/* Distant command parameter block */
d2_ret_val return_value = D2_NOERROR;	/* return value			   */

    /* Initialization */
    dist_com_pb.d23_Jhdinfo.d2_version = D23_V02;
    dist_com_pb.d23_Jcmd               = dist_com;
    dist_com_pb.d23_Jsv_prid           = D23_DS0SID + current_dir_id;
    dist_com_pb.d23_Jtmout             = MAX_DIST_COMMAND_TIMEOUT;
    dist_com_pb.d23_Jparsize           = 0;
    dist_com_pb.d23_Jparref           = NULL;

    /* Send message */
    if (d23_write_dist_cmd(&dist_com_pb) == D23_ERROR)
	return_value = D2_ERROR;

    return( return_value );
}



/* end of d20updconfig.c */
