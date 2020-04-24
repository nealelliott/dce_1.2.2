/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21_c80.c,v $
 * Revision 1.1.10.2  1996/02/18  18:16:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  17:39:09  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:53:10  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:09:40  root]
 * 
 * Revision 1.1.8.1  1994/02/22  17:38:27  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:30:39  marrek]
 * 
 * Revision 1.1.6.2  1993/08/11  06:04:33  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  10:51:56  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  19:46:36  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:24:24  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/02  03:15:31  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  21:36:32  zeliff]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21_c80.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 18:16:38 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*
* daton **************************************************************
*                                                                    *
* TYPE         :  MODULE                                             *
*                                                                    *
* NAME         :  d21_c80.c                                          *
*                                                                    *
* AUTHOR       :  Albert Biber                                       *
* DATE         :  22.01.88                                           *
*                                                                    *
* datoff *************************************************************
*/

#include <gds.h>
#include <string.h>

#include <d21dua.h>

/******************************************************************/

Bool d21_c82_dsname_cmp(
  D2_name_string str1,
  D2_name_string str2)
{
register signed32 i,len;

if ((len = strlen((char *)str1)) != strlen((char *)str2))
	return (FALSE);

for (i = 0; i < len; i++, str1++, str2++)
	{
	if (    (*str1 == *str2)
	    ||
		(*str1 >= D2_RG1 && *str1 <= D2_RG2 &&
			*str1 - *str2 == D2_ULDIFF)
	    ||
		(*str2 >= D2_RG1 && *str2 <= D2_RG2 &&
			*str2 - *str1 == D2_ULDIFF) 
	    ||
		(*str1 == D2_T61_REP && *str2 == D2_PRTBL_REP)
	    ||
		(*str2 == D2_T61_REP && *str1 == D2_PRTBL_REP) )
		continue;
	else
		return(FALSE);
	}
return (TRUE);
}
