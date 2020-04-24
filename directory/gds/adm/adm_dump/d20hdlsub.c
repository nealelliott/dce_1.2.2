/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20hdlsub.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:33  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:11:03  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:38  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:02:48  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:01:41  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:56:04  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:12:58  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:20:08  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:04:07  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:44:25  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:58:05  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:44:55  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:36:43  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20hdlsub.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:36 $";
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
/* NAME         : d20hdlsub.c                                         */
/*								      */
/* AUTHOR       : Volpers,D AP 11                                     */
/* DATE         : 27.04.88                                            */
/*								      */
/* KOMPONENTE	: DS						      */
/*								      */
/* DOK.-NR.	: DS-Design-Spez.				      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.	: <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM-DEPENDENCIES:                                               */
/*                      COMMON                                        */
/*								      */
/* HISTORY 	:						      */
/*								      */
/* Vers.Nr. |  Date   |	 Updates    		       | KZ | CR# FM# */
/*      0.1 | 15.04.88| Erstellung                     | hv |         */
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
#include <ctype.h>
#include <fcntl.h>
#include <string.h>
#include <time.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <d2dump.h>
#include <gdsext.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		   T Y P E  -  D E F I N I T I O N S		      */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

extern D2_pbhead  *d20_head;          /* import of pbhead             */

 char  	d20_m_dsa[D2_DNL_MAX + 1];   
				    /* master DSA of objects that     */
				    /* shall be propagated later on   */
				    /* via "delta_updates"            */

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_B_handle_subtrees                                 */
/*								      */
/* AUTHOR:      Volpers   (D AP 11)                                   */
/* DATE:        27.04.88                                              */
/*								      */
/* SYNTAX:      signed16 d20_B_handle_subtrees(parblk, dst_dsa_name)  */
/*                                                                    */
/*              D20_pB         *parblk;                               */
/*		D2_name_string	dst_dsa_name;			      */
/*								      */
/* DESCRIPTION: This function handles the dump and restore of         */
/*              subtrees                                              */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*								      */
/* RETURNVALUE:							      */
/*								      */
/*       0      :       no error                                      */
/*      -1      :       error during dump/restore                     */
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

 signed16 d20_B_handle_subtrees(
   D20_pB         *parblk,
   D2_name_string  dst_dsa_name)

{                               /*  d20_B_handle_subtrees             */

/**********************************************************************/
/*								      */
/*                 T Y P E  -  D E F I N I T I O N S                  */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*		    D E	K L A R	A T I O	N E N			      */
/*								      */
/**********************************************************************/

   signed16 ret_value = D2_NOERROR;
   char  file1[D20_FILE_MAX];           /* file name for search   */
   char  file2[D20_FILE_MAX];           /* file name for search   */
   Bool  finish = FALSE;             /* a sign whether the     */
					    /* restore function needed*/
 D20_offset *dn1;           /* offsets of DN (of DSA1) relative   */
				/* to beginning of file or memory     */
				/* area                               */
 D20_offset *pdn1;

 signed32     dn_no1;       /* number of DNs found in DSA1        */
 char        *name1;        /* pointer to memory area for DNs     */
 int          fd1 = -1;     /* file descriptor of file1           */
 signed32     l_name1;      /* length of dnlist                   */
 signed32     attr1_len;    /* length of attributes               */

 char        *p_name;
 Bool         single_obj;

 D20_delupd_info del_info;      /* information to be used for "delta- */
				/* updates"                           */

 Bool         int_file = FALSE;
				/* internal file used                 */

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

d20_init();

d20_head = parblk->d20_Bpbhead;
d20_head->d2_retcode = d20_head->d2_errclass = d20_head->d2_errvalue =
		       D2_NO_ERR;

single_obj = parblk->d20_Bsingle_ob;
p_name = (char *)parblk->d20_Bsubtree;

strcpy(d20_m_dsa, (char *)parblk->d20_Bdsa);
d20_norm(d20_m_dsa, strlen(d20_m_dsa));

				/*  check if a file contains the      */
				/*  subtree                           */
if (parblk->d20_Bo_file == NULL)
{                               /*  dump subtree from dsa with given  */
				/*  bind_id                           */
  if                            /*  file name is given                */
    (parblk->d20_Bn_file != NULL)
  {   
    strcpy (file1,parblk->d20_Bn_file);
    finish = TRUE;
  }
  else
  {
    sprintf(file1, "%s%d", D20_SE1, getpid());
    finish = FALSE;
  } 

				/*  set start time of dumping         */
  parblk->d20_Bstime = time((time_t *) 0L);
  if ((ret_value = d20_165_call_search(parblk->d20_B1bind_id, single_obj,
				parblk->d20_B1c_a, (D2_name_string) p_name,
				file1, FALSE, &fd1, &name1, &l_name1, &pdn1,
				&dn1, &dn_no1, &attr1_len)) != D2_NOERROR)

  {   
    return(ret_value);
  } 

  /* set install flag for alias objects.	*/
  if ((ret_value = d20_099_set_install_alias(parblk->d20_B1bind_id, 
		      (D20_offset **) pdn1, dn_no1, name1, file1,
		      attr1_len)) == D2_ERROR)
    return(ret_value);
 
				/*  set termination time of dumping  */
  parblk->d20_Betime = time((time_t *) 0L);
}
else
{                               /*  dump file is given               */
  strcpy(file1, parblk->d20_Bo_file);
  finish = FALSE;
				/*  read dump file                    */
  if                            /*  operation fails                   */
     (d20_116_read_dump_file(file1, &name1, &pdn1, &dn1, &dn_no1,
			     &attr1_len, &del_info))
  {                             /*  return(D2_ERROR)                  */
    return(D2_ERROR);
  }
  else
  {                             /*  open SEARCH-file of DSA1          */
    single_obj = del_info.d20_sobj;
    p_name     = del_info.d20_obj;
    if                          /*  open fails                        */
       ((fd1 = open(file1, O_RDONLY)) == -1)
    {                           /*  return(D2_ERROR)                  */
      d20_head->d2_errvalue = D20_FILE_ERR;
      return(D2_ERROR);
    }                           
				/*  set time stamp read from dump file*/
				/*  set directory id                  */
    parblk->d20_Betime = del_info.d20_time_stamp;
    parblk->d20_Bdir_id = del_info.d20_dir_id;
  }                             
} 

if (parblk->d20_Bn_file == NULL)
{                               /*  restore subtree in dsa with given */
				/*  bind_id                           */
  int_file = TRUE;
  sprintf(file2, "%s%d", D20_SE2, getpid());
  if ((ret_value =
      d20_165_call_search(parblk->d20_B2bind_id, single_obj, parblk->d20_B2c_a,
			  (D2_name_string) p_name, file2, TRUE, &fd1, &name1,
			  &l_name1, &pdn1, &dn1, &dn_no1,
			  &attr1_len)) != D2_NOERROR)
  {   
    unlink(file2);
    return(ret_value);
  } 

  finish = FALSE;
} 

if (finish == FALSE)
{                               /* call restore function to handle   */
				/* the two files                     */
  ret_value = d20_100_restore(file1, fd1, name1, (D20_offset **)pdn1,
				(D20_offset **)dn1, dn_no1, attr1_len, file2,
				dst_dsa_name) ;
}
else
{                               /* write dump file                  */
				/* store dn list in a file          */
  del_info.d20_sobj = single_obj;
  strcpy(del_info.d20_obj, p_name);
  strcpy(del_info.d20_dsa, d20_m_dsa);
  del_info.d20_dir_id     = parblk->d20_Bdir_id;
  del_info.d20_time_stamp = parblk->d20_Betime;
  ret_value = d20_118_write_dump_file(file1, (D20_offset **)pdn1,
				      (signed32)dn_no1, name1,(signed32)l_name1,
				      attr1_len, &del_info);
  if (ret_value)
  {   
    d20_head->d2_errvalue = D20_FILE_ERR;
  } 
  d20_111_free(name1, (char *)pdn1, (char *)dn1, (char *)NULL, (char *)NULL,
	       (char *)NULL, (char *)NULL,
	       (char *)NULL, (char *)NULL, (char *)NULL);
close(fd1);
} 

if (int_file == TRUE)
{   
  unlink(file2);
} 

return(ret_value);

}                               /*  d20_B_handle_subtrees             */
