/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: gstasn1.c,v $
 * Revision 1.1.4.2  1996/03/11  13:23:51  marty
 * 	Update OSF copyright years
 * 	[1996/03/11  13:12:23  marty]
 *
 * Revision 1.1.4.1  1995/12/08  17:46:09  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  16:24:37  root]
 * 
 * Revision 1.1.2.2  1994/07/13  19:40:13  keutel
 * 	OT 11276: core dump in asn1_dump, ...
 * 	[1994/07/13  19:04:02  keutel]
 * 
 * Revision 1.1.2.1  1994/06/21  14:39:43  marrek
 * 	Created in June 94 code submission.
 * 	[1994/06/10  16:04:34  marrek]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: gstasn1.c,v $ $Revision: 1.1.4.2 $ $Date: 1996/03/11 13:23:51 $";
#endif

/**********************************************************************/
/*                                                                    */
/* Component    : GDS                                                 */
/*                                                                    */
/* File name    : gstasn1.c                                           */
/*                                                                    */
/* Description  : This file contains the functions of the logfile     */
/*                evaluation tool gdsstep of the GDS component, which */
/*                make the ASN.1 encoded messages readable.           */
/*                                                                    */
/* Date         : 05/27/94                                            */
/*                                                                    */
/* Author : Jochen Keutel, SNI BA NM 123                              */
/*                                                                    */
/**********************************************************************/

#ifdef THREADSAFE
#include <pthread.h>
#endif

#include <stdio.h>
#include <string.h>

#include <dce/asn1.h>
#include <gstep.h>

/**********************************************************************/
/*                                                                    */
/* Function name: gds_gst_print_asn1_encoded_msg                      */
/*                                                                    */
/* Description  : The function makes an ASN.1 encoded message         */
/*                readable. The first call prints and stores the      */
/*                operation ID, the  second call prepares the APDU.   */
/*                                                                    */
/* Return value :                                                     */
/*      None                                                          */
/*                                                                    */
/* Author : Jochen Keutel, SNI BA NM 123                              */
/*                                                                    */
/**********************************************************************/

void gds_gst_print_asn1_encoded_msg(
    signed32   size,    /* IN    - size of data element               */
    pointer_t  data)    /* IN    - data to be printed                 */

{
asn1       asn1_message;  /* ASN.1 encoded message                      */

asn1_message = (asn1) data;
printf("Message size: %ld, ASN.1 encoded message:\n",size);

if ((asn1_message != NULL) && (size != 0))
  {
  (void)asn1_dump(stdout,asn1_message,asn1_message + size);
  }
} /* end of gds_gst_print_asn1_encoded_msg */
