/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: cp_help.c,v $
 * Revision 1.1.34.2  1996/02/18  00:00:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:58  marty]
 *
 * Revision 1.1.34.1  1995/12/08  00:15:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:56:38  root]
 * 
 * Revision 1.1.32.1  1994/01/21  22:32:37  cbrooks
 * 	RPC Code Cleanup - Initial Submission
 * 	[1994/01/21  20:22:16  cbrooks]
 * 
 * Revision 1.1.2.3  1993/01/03  22:37:11  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:54:01  bbelch]
 * 
 * Revision 1.1.2.2  1992/12/23  19:40:56  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:54:50  zeliff]
 * 
 * Revision 1.1  1992/01/19  03:14:15  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
**  Copyright (c) 1991 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**
**  NAME
**
**      cp_help.c
**
**  FACILITY:
**
**      Remote Procedure Call (RPC) 
**
**  ABSTRACT:
**
**      help module interface
**
**
*/

#include    <stdio.h>
#include    <dce/rpc.h>

extern boolean32 usage _DCE_PROTOTYPE_((unsigned_char_p_t *));
extern boolean32 rpccp_help _DCE_PROTOTYPE_((unsigned_char_p_t *));


/*
**++
**
**  ROUTINE NAME:       rpccp_help
**
**  SCOPE:              global to rpccp
**
**  DESCRIPTION:
**      
**  This routine displays the usage() menu.
**
**  INPUTS:             
**
**      args            an array of pointers to unsigned char strings
**                      containing the command substrings (tokens)
**                      parsed from the command line - NOT accessed.
**
**  INPUTS/OUTPUTS:     none
**
**
**  OUTPUTS:            none 
**
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   displays the usage menu on the user's screen
**
**  FUNCTION VALUE:     boolean32 - true or false
**
**      return          true for success; false for failure
**
**  SIDE EFFECTS:       none
**
**--
**/

boolean32 rpccp_help (
    unsigned_char_p_t       *args
)
{
    return (usage (args));
}
