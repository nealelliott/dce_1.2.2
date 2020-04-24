/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20rdwr.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:38  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:35  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:11:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:40  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:02:49  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:01:46  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:05  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:13:11  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:22:26  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:04:22  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:44:42  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:58:14  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:45:25  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:36:57  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20rdwr.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:38 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d20rdwr.o    [file handling: Read / Write]          */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 29.04.88                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOK.-NR.	: DS-Design-Spez.				      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM-DEPENDENCIES:                                               */
/*                      COMMON                                        */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*      0.1 | 29.04.88| Original                       | ws |         */
/*								      */
/*datoff **************************************************************/

/**********************************************************************/
/*								      */
/*		       Modulidentification			      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*			  I N C	L U D E	S			      */
/*								      */
/**********************************************************************/

#include <gds.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d2dump.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                 D E C L A R A T I O N S        DATA                */
/*								      */
/**********************************************************************/

extern  D2_pbhead  *d20_head;

  int     d20_n_bytes = 0;

static    char    buffer[D20_BUFSIZ];
static    char   *p_buf;

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_107_read()                                        */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        26.04.88                                              */
/*								      */
/* SYNTAX:      signed16 d20_107_read(fd, p_area, n)                  */
/*              int      fd;                                          */
/*              char    *p_area;                                      */
/*              signed32  n;                                          */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will read <n> Bytes from the given file.*/
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              int      fd;            file descriptor               */
/*              char    *p_area;        pointer to memory area        */
/*                                      == NULL if data need not be   */
/*                                                 copied             */
/*              signed32  n;            number of bytes to be read    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              char    *p_area;        memory area filled with data  */
/*                                      (if "copy" == D2_TRUE)        */
/*                                                                    */
/* RETURNVALUE:							      */
/*								      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       EOF reached                                   */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 signed16 d20_107_read(
   int       fd,
   char     *p_area,
   signed32  n)
 
{                               /*  d20_107_read()                    */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

    char   *p_max;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

while                           /*  data must be read                 */
      (n > d20_n_bytes)
{                               
  if                            /*  rest of data must be copied       */
     (p_area != NULL)
  {                             /*  copy rest of data                 */
    for (p_max = p_area + d20_n_bytes; p_area < p_max; )
    { 
      *p_area++ = *p_buf++;
    } 
  }                             
  n -= d20_n_bytes;
  if                            /*  error on reading new data         */
     ((d20_n_bytes = read(fd, buffer, D20_BUFSIZ)) <= 0)
  {                             /*  return(D2_ERROR)                  */
    d20_n_bytes = 0;
    return(D2_ERROR);
  }                             
  p_buf = buffer;
}                               

if                              /*  data must be copied               */
   (p_area != NULL)
{                               /*  copy data                         */
  for (p_max = p_area + n; p_area < p_max; )
  { 
    *p_area++ = *p_buf++;
  } 
}
else
{                               /*  actualize pointer to file buffer  */
  p_buf += n;
}                               
d20_n_bytes -= n;

				/*  return(D2_NOERROR)                */
return(D2_NOERROR);

}                               /*  d20_107_read()                    */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_117_write()                                       */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        26.04.88                                              */
/*								      */
/* SYNTAX:      signed16 d20_117_write(fd, p_area, n, out)            */
/*              int      fd;                                          */
/*              char    *p_area;                                      */
/*              signed32  n;                                          */
/*              Bool     out;                                         */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will write <n> Bytes into the given     */
/*              file.                                                 */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              int      fd;            file descriptor               */
/*              char    *p_area;        pointer to memory area        */
/*              signed32 n;             number of bytes to be written */
/*              Bool     out;           flag indicating whether the   */
/*                                      information should immediately*/
/*                                      be stored or not              */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:							      */
/*								      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       EOF reached                                   */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 signed16 d20_117_write(
   int       fd,
   char     *p_area,
   signed32  n,
   Bool      out)
 
{                               /*  d20_117_write()                   */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

    char   *p_max;
    char   *p_max_buf = buffer + D20_BUFSIZ;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

p_buf = buffer + d20_n_bytes;
for                             /*  all data                          */
    (p_max = p_area + n; p_area < p_max; )
{                               
  if                            /*  not end of buffer reached         */
     (p_buf < p_max_buf)
  {                             /*  copy into buffer                  */
    *p_buf++ = *p_area++;
    d20_n_bytes++;
  }
  else
  {                             /*  store buffer into file            */
    if                          /*  operation fails                   */
       (write(fd, buffer, d20_n_bytes) != d20_n_bytes)
    {                           /*  return(D2_ERROR)                  */
      d20_head->d2_errvalue = D20_FILE_ERR;
      return(D2_ERROR);
    }
    else
    {                           /*  reset buffer                      */
      p_buf = buffer;
      d20_n_bytes = 0;
    }                           
  }                             
}                               

if                              /*  buffer must immediately be stored */
   (out == TRUE)
{                               /*  store buffer into file            */
  if                            /*  operation fails                   */
     (write(fd, buffer, d20_n_bytes) != d20_n_bytes)
  {                             /*  return(D2_ERROR)                  */
    d20_head->d2_errvalue = D20_FILE_ERR;
    return(D2_ERROR);
  }
  else
  {                             /*  reset buffer                      */
    d20_n_bytes = 0;
  }                             
}                               

				/*  return(D2_NOERROR)                */
return(D2_NOERROR);

}                               /*  d20_117_write()                   */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_119_read_attr()                                   */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        04.05.88                                              */
/*								      */
/* SYNTAX:      signed16 d20_119_read_attr(fd, off, dn, attr_len,     */
/*					area, min, max, p_dn, p_attr) */
/*              int      fd;                                          */
/*              signed32 off;                                         */
/*              char    *dn;                                          */
/*              signed32 attr_len;                                    */
/*              char    *area;                                        */
/*              signed32 *min;                                        */
/*              signed32 *max;                                        */
/*              char   **p_dn;                                        */
/*              char   **p_attr;                                      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will read the attribute list belonging  */
/*              to the DN with a given offset.                        */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              int      fd;            file descriptor               */
/*              signed32 off;           offset of DN                  */
/*              char    *dn;            DN                            */
/*              signed32 attr_len;      number of bytes to be read    */
/*              char    *area;          pointer to memory area        */
/*              signed32 *min;          minimum position of read      */
/*                                      attributes                    */
/*              signed32 *max;          maximum position of read      */
/*                                      attributes                    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              char    *area;          memory area filled with data  */
/*              signed32 *min;          new minimum position of read  */
/*                                      attributes                    */
/*              signed32 *max;          new maximum position of read  */
/*                                      attributes                    */
/*              char   **p_dn;          pointer to exact DN           */
/*              char   **p_attr;        pointer to attributes         */
/*                                                                    */
/* RETURNVALUE:							      */
/*								      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       error occurred                                */
/*								      */
/* STANDARD-HEADER: None                                              */
/*								      */
/* GLOBAL  DATA  (read-only):                                         */
/*								      */
/* GLOBAL  DATA  (updated):                                           */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*								      */
/* NOTES:                                                             */
/*								      */
/*exoff ***************************************************************/

 signed16 d20_119_read_attr(
   int        fd,
   signed32   off,
   char      *dn,
   signed32   attr_len,
   char      *area,
   signed32  *min,
   signed32  *max,
   char     **p_dn,
   char     **p_attr)
 
{                               /*  d20_119_read_attr()               */

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

 signed32  dn_len;
 signed32  n_bytes;            /* bytes to be read                   */
 int       n_read;             /* number of bytes that were read     */
 signed32  max_off;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

dn_len = D2_ALIGN(strlen(dn) + 1);
n_bytes = dn_len + attr_len;
max_off = off + n_bytes;

if                              /*  entry information already read    */
   (off >= *min && off < *max && max_off >= *min && max_off < *max)
{                               /*  set pointer to attributes / DN    */
  *p_dn = area + off - *min;
}
else if                         /*  positioning fails                 */
      (lseek(fd, off, 0) == -1L)
{                               /*  return(D2_ERROR)                  */
  d20_head->d2_errvalue = D20_FILE_ERR;
  return(D2_ERROR);
}
else
{                               /*  read DN/attribute list            */
  n_read = (n_bytes < D20_BUFSIZ) ? D20_BUFSIZ : n_bytes;
  if                            /*  error on reading DN/attribute list*/
     ((n_read = read(fd, area, n_read)) < n_bytes)
  {                             /*  return(D2_ERROR)                  */
    d20_head->d2_errvalue = D20_FILE_ERR;
    return(D2_ERROR);
  }
  else
  {                             /*  set pointer to attributes / DN    */
    *p_dn = area;
    *min  = off;
    *max  = off + n_read;
  }                             
}                               
*p_attr = *p_dn + dn_len;

				/*  return(D2_NOERROR)                */
return(D2_NOERROR);

}                               /*  d20_119_read_attr()               */
