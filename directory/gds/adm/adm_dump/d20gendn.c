/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20gendn.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:33  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:28  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:10:44  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:33  root]
 * 
 * Revision 1.1.8.3  1994/06/21  14:43:10  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  12:08:47  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:02:45  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:01:26  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:55:59  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:12:23  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:16:48  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:03:24  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:44:04  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:57:52  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:44:11  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:36:25  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20gendn.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:33 $";
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
/* NAME         : d20gendn.o   [generation of DN-list]                */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 27.04.88                                            */
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
/*      0.1 | 27.04.88| Original                       | ws |         */
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
#include <sys/types.h>
#include <stdio.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d23apdu.h>
#include <d2dump.h>
#include <d2delupd.h>
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

extern char      *d20_beg1;             /* start address of DN-strings*/
extern char      *d20_beg2;             /* start address of DN-strings*/

extern D2_pbhead *d20_head;

extern int        d20_n_bytes;          /* number of bytes already    */
					/* read                       */

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_105_gen_dn()                                      */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        20.04.88                                              */
/*								      */
/* SYNTAX:      d2_ret_val d20_105_gen_dn(ignore, file, fd, names,    */
/*				     act_anz, pdn_off, dn_off, dn_no, */
/*				     attr_len)                        */
/*              Bool         ignore;                                  */
/*              String       file;                                    */
/*              int         *fd;                                      */
/*              char       **names;                                   */
/*              signed32    *act_anz;                                 */
/*              D20_offset **pdn_off;                                 */
/*              D20_offset **dn_off;                                  */
/*              signed32    *dn_no;                                   */
/*              signed32    *attr_len;                                */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function generates the list of all DNs to be     */
/*              dumped or restored.                                   */
/*              It takes the result of the "search" operations and    */
/*              determines all DNs that must be dumped or restored    */
/*              additionally.                                         */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              Bool         ignore;        a flag indicating whether */
/*                                          error on reading first tag*/
/*                                          (--> search file contains */
/*                                          nothing) can be ignored   */
/*              String       file;      file name containing search   */
/*                                      result message                */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              int         *fd;        file descriptor of search     */
/*                                      result file                   */
/*              char       **names;     memory address for DNs        */
/*              signed32    *act_anz;   length of DN-list             */
/*              D20_offset **pdn_off;   pointer array for DN-offsets  */
/*              D20_offset **dn_off;    DN-offsets                    */
/*              signed32    *dn_no;     number of DNs                 */
/*              signed32    *attr_len;  max. length of longest        */
/*                                      attribute list                */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       Error occurred                                */
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


 d2_ret_val d20_105_gen_dn(
   Bool         ignore,
   String       file,
   int         *fd,
   char       **names,
   signed32    *act_anz,
   D20_offset **pdn_off,
   D20_offset **dn_off,
   signed32    *dn_no,
   signed32    *attr_len)

{                               /*  d20_105_gen_dn()                  */

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

 signed32     no_bytes = 0;             /* actual number of bytes */
					    /* read in search file    */

 Bool         mk_exist = FALSE;      /* a flag indicating whether */
					    /* Master-DSAs should be read*/

 D20_dn_off   dn;                       /* DN-info                */
 D20_dn_off   mk;                       /* master knowledge info  */

 D20_dns      ext_dn;                   /* external DN-list       */
 D20_dns      int_dn;                   /* internal DN-List       */

#ifdef TRACE
 signed16 i;
#endif

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

dn.poff = mk.poff = NULL;
dn.off = mk.off = NULL;
int_dn.names = ext_dn.names = NULL;

int_dn.max_anz = D20_MORE;
ext_dn.max_anz = D20_DN_SIZE;
int_dn.anz = ext_dn.anz = 0;

dn.no = mk.no = 0;
dn.max_no = D20_DN_NO;
mk.max_no = D20_INT_DN_NO;

if                              /*  open of file fails                */
   ((*fd = open(file, O_RDONLY)) == -1)
{                               /*  return(D2_ERROR)                  */
  d20_head->d2_errvalue = D20_FILE_ERR;
  /*
  d21d009_errlog("open of SEARCH file fails", (Errcode *)NULL);
  */
  return(D2_ERROR);
}
else if                         /*  no memory for offsets             */
      ((dn.poff = (D20_offset **) malloc((size_t) dn.max_no *
					 sizeof(String))) == NULL ||
       (dn.off = (D20_offset *) malloc((size_t) dn.max_no *
				       sizeof(D20_offset))) == NULL ||
       (mk.poff = (D20_offset **) malloc((size_t) mk.max_no *
					 sizeof(String))) == NULL ||
       (mk.off = (D20_offset *) malloc((size_t) mk.max_no *
				       sizeof(D20_offset))) == NULL ||
       (ext_dn.names = malloc((size_t) ext_dn.max_anz)) == NULL ||
       (int_dn.names = malloc((size_t) int_dn.max_anz)) == NULL)
{                               /*  return(D2_ERROR)                  */
  d20_head->d2_errvalue = D20_ALLOC_ERR;
  d20_111_free((char *)dn.poff, (char *)dn.off, NULL, NULL, NULL, NULL,
	       (char *)mk.poff, (char *)mk.off, ext_dn.names, int_dn.names);
  close(*fd);
  *fd = -1;
  return(D2_ERROR);
}                               

#ifdef TRACE
fprintf(stderr,"105_gen_dn: FILE: %s\n", file);
fflush(stderr);
#endif

*attr_len = 0L;
do
  {                             /*  determine offsets of DNs (in file */
				/*  and memory area)                  */
    mk_exist = FALSE;
    if                          /*  operation fails                   */
       (d20_106_get_dn_offsets(ignore, *fd, &ext_dn, &int_dn, &dn, &mk,
			       &no_bytes, attr_len))
    {                           /*  return(D2_ERROR)                  */
      if (d20_head->d2_errvalue == D2_NO_ERR)
      {   
	/*
	d21d009_errlog("SEARCH inconsistent", (Errcode *)NULL);
	*/
	d20_head->d2_errvalue = D20_FILE_ERR;
      } 
      d20_111_free((char *)dn.poff, (char *)dn.off, NULL, NULL, NULL, NULL,
		   (char *)mk.poff, (char *)mk.off, ext_dn.names, int_dn.names);
      close(*fd);
      *fd = -1;
      return(D2_ERROR);
    }                           

    d20_beg1 = d20_beg2 = ext_dn.names;

#ifdef TRACE
    fprintf(stderr,"d20gendn: no_bytes: %d\n", no_bytes);
    fprintf(stderr,"gen-dn: anz(dn): %d\n",dn.no);
    fflush(stderr);
    *names = ext_dn.names;
    for (i = 0; i < dn.no; i++)
    { fprintf(stderr,"gen-dn: names: %s---length: %d\n",*names,
		      strlen(*names) + 1);
      fflush(stderr);
      *names += strlen(*names) + 1;
    }
    fprintf(stderr,"gen-dn: anz(mk): %d\n",mk.no);
    fflush(stderr);
    *names = int_dn.names;
    for (i = 0; i < mk.no; i++)
    { fprintf(stderr,"gen-dn: names: %s---length: %d\n",*names,
		      strlen(*names) + 1);
      fflush(stderr);
      *names += strlen(*names) + 1;
    }
    for (i = 0; i < dn.no; i++)
    { fprintf(stderr, "gen-dn: mem_off: %d\n", (*(dn.poff + i))->d20_mem_off);
      fflush(stderr);
    }
#endif
				/*  sort DNs                          */
    qsort(dn.poff, (size_t) dn.no, (size_t) sizeof(String), 
	(int (*)(const void *, const void *)) d20_101_compare_dn);

    d20_beg1 = d20_beg2 = int_dn.names;
    if                          /*  MASTER-DSAs found                 */
       (mk.no > 0)
    {                           /*  sort MasterDSAs                   */
      qsort(mk.poff, (size_t) mk.no, (size_t) sizeof(String),
		(int (*)(const void *, const void *)) d20_101_compare_dn);
				/*  get Master-DSAs                   */
      d20_beg1 = ext_dn.names;
      if                        /*  operation fails                   */
	 (d20_112_get_more_dns(dn.poff, dn.no, mk.poff, mk.no, &mk_exist))
      {                         /*  return(D2_ERROR)                  */
	d20_111_free((char *)dn.poff, (char *)dn.off, NULL, NULL, NULL, NULL,
		     (char *)mk.poff, (char *)mk.off, ext_dn.names,
		     int_dn.names);
	close(*fd);
	*fd = -1;
	return(D2_ERROR);
      }                         
    }                           
#ifdef TRACE
fprintf(stderr,"105: mk_exist: %d\n", mk_exist);
fflush(stderr);
#endif
  }                             
while                           /*  DSAs still exist       */
      (mk_exist == TRUE);

#ifdef TRACE
fprintf(stderr,"105: ALL OBJECTS FOUND");
fflush(stderr);
#endif

d20_beg1 = d20_beg2 = ext_dn.names;

*names    = ext_dn.names;
*act_anz  = ext_dn.anz;

*pdn_off   = (D20_offset *) dn.poff;
*dn_off    = (D20_offset *) dn.off;
*dn_no     = dn.no;

d20_111_free( (char *)mk.poff, (char *)mk.off, int_dn.names, (char *)NULL, 
	      (char *)NULL,(char *)NULL, (char *)NULL, (char *)NULL, 
	      (char *)NULL, (char *)NULL);

				/*  return(D2_NOERROR)                */
return(D2_NOERROR);

}                               /*  d20_105_gen_dn()                  */
