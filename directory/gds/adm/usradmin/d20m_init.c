/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20m_init.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:25  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:23:12  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:21:51  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:53:02  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:07:06  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:14:11  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  18:04:15  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:38:53  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  11:06:23  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:42:24  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:11:13  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:07:59  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:41:14  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:47:15  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:02:19  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:46:12  melman]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20m_init.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:25 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d20m_init.c                                         */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                    */
/* DATE         : 24.07.89                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*  0.1     |24.07.89 |  original                      |    |         */
/*datoff **************************************************************/

/*****************************************************************************/
/*    P R E P R O C E S S O R   I N S T R U C T I O N S                      */
/*****************************************************************************/

#include <gds.h>

#include <stdio.h>
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2adm.h>
#include <d2msk.h>
#include <d20_adm.h>
#include <d20proto.h>

#include <nl_types.h>


/*****************************************************************************/
/*     D E F I N I T I O N S                                                 */
/*****************************************************************************/

#define D20_DO_NOTHING  '@'     /* indicates that no format string or      */
				/* and no output string has to be assigned */
				/* to the D20_omask-structure              */
#define D20_THROW_AWAY  '$'     /* necessary to read strings with leading  */
				/* blanks or blanks at the end (this       */
				/* character is thrown away)               */
#define D20_CONCATENATE '&'     /* indicates that the message string in the*/
				/* NLS-file has to be concatenated with the*/
				/* next message string (this character is  */
				/* thrown away)                            */


 d2_ret_val d20_mask_init(
   nl_catd     nl_fd,
   signed32    msg_set,
   char      **str,
   D20_omask  *m_ptr)
{
signed32 msg_no = 1;
char    *res;
char    *def_str = "DEFAULT";
signed32 x;
signed32 y;
char     message[200];
int      rc;

for ( ; m_ptr->d20_fkt_id != D20_EOMSK; m_ptr++)
{   if (m_ptr->d20_fkt_id == D20_FKTX)
    {   continue;
    }

				/* read message string                */
    if ((res = catgets(nl_fd, msg_set, msg_no, def_str)) == def_str)
    {   fprintf(stderr, "Error while reading NLS file; msg_set = %d msg_no = %d\n",
		msg_set, msg_no);
	return(D2_ERROR);
    }
    msg_no++;

				/* copy message string into user memory */
				/* area                                 */
    if ((rc = sscanf(res, "%d %d %[^#]%*c %[^#]%*c", &x, &y, message, *str))
		     != 4)
    {   fprintf(stderr, "Error while formatting NLS string; msg_set = %d msg_no = %d\n",
		msg_set, msg_no);
	return(D2_ERROR);
    }

    m_ptr->d20_line_no = x;
    m_ptr->d20_column_no = y;
    if (*(*str) != D20_DO_NOTHING)
				/* format string is defined           */
    {   m_ptr->d20_oformat = *str;
	*str += strlen(*str) + 1;
    }

    if (message[0] != D20_DO_NOTHING)
				/* IO-string is defined               */
    {   m_ptr->d20_ioinfo = *str;
	if (message[0] == D20_THROW_AWAY)
				/* skip first character               */
	{   strcpy(*str, &message[1]);
	    *str += strlen(&message[1]) + 1;
	}
	else
	{   strcpy(*str, message);
	    *str += strlen(message) + 1;
	    if (*(*str - 2) == D20_THROW_AWAY)
	    {                   /* ignore last char                   */
		*(*str - 2) = D2_EOS;
		(*str)--;
	    }
	}
    }
}

return(D2_NOERROR);
}



 d2_ret_val d20_read_str(
   nl_catd     nl_fd,
   signed32    msg_set,
   signed32    max_no,
   char      **str,
   char      **str_array)
{
signed32  msg_no = 1;
char    *res;
char    *def_str = "DEFAULT";

				/* read message string                */
for (msg_no = 1; msg_no <= max_no; msg_no++)
{   if ((res = catgets(nl_fd, (int)msg_set, (int)msg_no, def_str)) == def_str)
    {   fprintf(stderr, "Error while reading NLS file; msg_set = %d msg_no = %d\n",
		msg_set, msg_no);
	return(D2_ERROR);
    }

    *str_array = *str;
    str_array++;
    if                          /* first character is '$'             */
       (*res == D20_THROW_AWAY)
    {                           /* ignore first character             */
	strcpy(*str, res + 1);
    }
    else
    {   strcpy(*str, res);
    }
    *str += strlen(*str) + 1;

    if                          /* last character is '$'              */
       (*(*str - 2) == D20_THROW_AWAY)
    {                           /* ignore last char                   */
	*(*str - 2) = D2_EOS;
	(*str)--;
    }

    if                          /* last character is CONCATENATE character */
       (*(*str - 2) == D20_CONCATENATE)
    {                           /* ignore last char                   */
				/* don't terminate string with EOS    */
	(*str) -= 2;
    }
}
return(D2_NOERROR);
}

