/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: dce_error_krpc.c,v $
 * Revision 1.1.59.2  1996/02/18  00:00:40  marty
 * 	Update OSF copyright years
 * 	[1996/02/17  22:53:46  marty]
 *
 * Revision 1.1.59.1  1995/12/08  00:15:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/07  23:56:17  root]
 * 
 * Revision 1.1.57.1  1994/01/21  22:32:00  cbrooks
 * 	RPC Code Cleanup - Initial Submission
 * 	[1994/01/21  20:57:22  cbrooks]
 * 
 * Revision 1.1.4.3  1993/01/03  22:36:15  bbelch
 * 	Embedding copyright notice
 * 	[1993/01/03  19:52:27  bbelch]
 * 
 * Revision 1.1.4.2  1992/12/23  19:38:57  zeliff
 * 	Embedding copyright notice
 * 	[1992/12/23  14:53:10  zeliff]
 * 
 * Revision 1.1.2.2  1992/05/01  17:55:46  rsalz
 * 	"Changed as part of rpc6 drop."
 * 	[1992/05/01  17:50:38  rsalz]
 * 
 * Revision 1.1  1992/01/19  03:16:08  devrcs
 * 	Initial revision
 * 
 * $EndLog$
 */
/*
*/
/*
**  Copyright (c) 1991 by
**      Hewlett-Packard Company, Palo Alto, Ca. & 
**      Digital Equipment Corporation, Maynard, Mass.
**
**  NAME
**
**      dce_error_osc.c
**
**  FACILITY:
**
**      Distributed Computing Environment (DCE)
**
**  ABSTRACT:
**
**  Error status management routines.
**
*/

#include <dce/dce.h>
#include <dce/dce_error.h>

#define MSGA "no error text (0x"
#define MSGB ")"

/*
**++
**
**  ROUTINE NAME:       dce_error_inq_text
**
**  SCOPE:              PUBLIC - declared in dce_error.h
**
**  DESCRIPTION:
**      
**  Returns a text string in a user provided buffer associated with a given 
**  error status code. In the case of errors a text string will also be 
**  returned indicating the nature of the error.
**
**  INPUTS:
**
**      status_to_convert   A DCE error status code to be converted to 
**                          text form.
**
**  INPUTS/OUTPUTS:         None.
**
**  OUTPUTS:
**
**      error_text          A user provided buffer to hold the text
**                          equivalent of status_to_convert or
**                          a message indicating what error occurred.
**                          
**
**      status              The result of the operation. One of:
**                           0  -  success
**                          -1  -  failure
**
**  IMPLICIT INPUTS:    none
**
**  IMPLICIT OUTPUTS:   none
**
**  FUNCTION VALUE:     none
**
**  SIDE EFFECTS:       none
**
**--
**/

void dce_error_inq_text 
(
    unsigned long           status_to_convert,
    unsigned char           *error_text,
    int		        *status
)
{
    *status = 0;

    strcpy((char *)error_text, MSGA);
    error_text += sizeof(MSGA) - 1;
    sprintn(status_to_convert, 16, 1, 8, (char *)error_text);
    error_text += 8;
    strcat((char *)error_text, MSGB);
}
