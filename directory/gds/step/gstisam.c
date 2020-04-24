/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: gstisam.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:57  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:28  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:46:33  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:47  root]
 * 
 * Revision 1.1.2.4  1994/07/06  15:08:16  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:20:49  marrek]
 * 
 * Revision 1.1.2.3  1994/06/10  21:16:07  devsrc
 * 	cr10871 - fix copyright
 * 	[1994/06/10  14:54:15  devsrc]
 * 
 * Revision 1.1.2.2  1994/05/10  16:03:12  marrek
 * 	Use correct rcs id string.
 * 	[1994/05/02  14:47:55  marrek]
 * 
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/02  13:23:05  marrek]
 * 
 * Revision 1.1.2.1  1994/03/23  15:47:13  keutel
 * 	creation
 * 	[1994/03/21  15:55:12  keutel]
 * 
 * $EndLog$
 */
/**********************************************************************/
/*                                                                    */
/* Component    : GDS                                                 */
/*                                                                    */
/* File name    : gstisam.c                                           */
/*                                                                    */
/* Description  : This file contains utility functions of the logfile */
/*                evaluation tool gdsstep of the GDS component, which */
/*                make C-ISAM structures readable.                    */
/*                                                                    */
/* Date         :  2/08/94                                            */
/*                                                                    */
/* Author       : E. Kraemer, BUBA NM12                               */
/*                                                                    */
/**********************************************************************/

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gstisam.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:57 $";
#endif

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif
#include <string.h>

#include <isam.h>

#include <gstep.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_keydesc                               */
/*                                                                    */
/* Description  : The function makes the C-ISAM key description       */
/*                readable.                                           */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_keydesc(
    const char *format, /* IN    - format string for printf           */
    signed16   call_nr, /* IN    - call number of function call       */
    unsigned32 size,    /* IN    - size of data element               */
    pointer_t  data)    /* IN    - data to be printed                 */

{                       /* gds_gst_print_keydesc                      */

  static struct keydesc keydesc;    /* key descriptor structure       */

  char *form_elm;       /* pointer to format element                  */

  register struct keypart *kpart;   /* loop pointer to key part       */
  register byte     *logdata;   /* data to be actually logged         */
  register signed32 loglen; /* length of data to be actually logged   */
  register signed32 len;/* length of logged data                      */
  register signed16 i;  /* loop variable                              */

  form_elm  = strchr(format,'%');
  *form_elm = '\0';
  printf(format);
  *form_elm = '%';
  printf("\n");

  switch(call_nr)
  {                     /* print adm prefix                           */
    case 0:
      keydesc = *(struct keydesc *)data;
      printf("Flags: %d number of key parts: %d\n",keydesc.k_flags,
	keydesc.k_nparts);
      break;
    case 1:
      for               /* all key parts                              */
	 (i = 0, kpart = keydesc.k_part, len = 0, logdata = (byte *)data;
	  i < keydesc.k_nparts; i++, kpart++)
      {                 /* print key part description                 */
	printf("kp_start: %d  kp_leng: %d  kp_type: ",
	  kpart->kp_start,kpart->kp_leng,kpart->kp_type);
	switch(kpart->kp_type)
	{
	  case CHARTYPE:
	    printf("CHARTYPE ");
	    if        /* still data to print                        */
	      (len < size)
	    {         /* print the data                             */
	      loglen = MIN(size - len,kpart->kp_leng);
	      printf("kp_value: \n");
	      gds_gst_print_binary_data(loglen,logdata);
	      logdata += loglen;
	      len     += loglen;
	    }
	    break;
	  case INTTYPE:
	    printf("INTTYPE ");
	    if        /* still data to print                        */
	      (len < size)
	    {         /* print the data                             */
	      printf("kp_value: %d",ldint(logdata));
	      logdata += INTSIZE;
	      len     += INTSIZE;
	    }
	    break;
	  case LONGTYPE:
	    printf("LONGTYPE ");
	    if        /* still data to print                        */
	      (len < size)
	    {         /* print the data                             */
	      printf("kp_value: %ld",ldlong((char *)logdata));
	      logdata += LONGSIZE;
	      len     += LONGSIZE;
	    }
	    break;
	  default:
	    printf("Unknown C-ISAM data type %d ",kpart->kp_type);
	    if        /* still data to print                        */
	      (len < size)
	    {         /* print the data                             */
	      loglen = MIN(size - len,kpart->kp_leng);
	      printf("kp_value: \n");
	      gds_gst_print_binary_data(loglen,logdata);
	      logdata += loglen;
	      len     += loglen;
	    }
	    break;
	}
	printf("\n");
      }
      break;
  }


}                       /* gds_gst_print_keydesc                      */

