/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20dpfile.c,v $
 * Revision 1.1.10.2  1996/02/18  19:38:29  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:19:25  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:10:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:49:29  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:49:08  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:08:46  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:02:42  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:01:10  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:51:21  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:11:52  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  08:07:56  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:02:37  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  17:43:42  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  12:57:41  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  20:43:39  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  18:36:09  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20dpfile.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:38:29 $";
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
/* NAME         : d20dpfile.o  [dump file handling]                   */
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
#include <sys/types.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>
#include <fcntl.h>
#include <errno.h>
#include <stdlib.h>

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

extern  D2_pbhead   *d20_head;
extern  int          d20_n_bytes;

  int          d20_dp_fd = -1;      /* file descriptor for reading    */
				    /* internal dump file             */

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_116_read_dump_file()                              */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        26.04.88                                              */
/*								      */
/* SYNTAX:      signed16 d20_116_read_dump_file(file, names, pdn, dn, */
/*                                           dn_no,                   */
/*                                           attr_len, del_info)      */
/*              String        file;                                   */
/*              char        **names;                                  */
/*              D20_offset  **pdn;                                    */
/*              D20_offset  **dn;                                     */
/*              signed32     *dn_no;                                  */
/*              signed32     *attr_len;                               */
/*              D20_delupd_info *del_info;                            */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function reads the internally created dump file  */
/*              (of DSA1).                                            */
/*              This internal dump file contains the offsets of DNs   */
/*              and alias objects in the "search dump" file and in    */
/*              memory. There are also all DN strings.                */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              String        file;     file name containing internal */
/*                                      dump information              */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              char        **names;    DN-strings                    */
/*              D20_offset  **pdn;      pointer array with DN-offsets */
/*              D20_offset  **dn;       DN-offset information         */
/*              signed32     *dn_no;    number of DNs in file         */
/*              signed32     *attr_len; max. length of longest        */
/*                                      attribute list                */
/*              D20_delupd_info *del_info;  information to be used    */
/*                                          for "delta_updates"       */
/*                                                                    */
/* RETURNVALUE:							      */
/*								      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       operation fails                               */
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

 signed16 d20_116_read_dump_file(
   String           file,
   char           **names,
   D20_offset     **pdn,
   D20_offset     **dn,
   signed32        *dn_no,
   signed32        *attr_len,
   D20_delupd_info *del_info)

{                               /*  d20_116_read_dump_file()          */

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

    signed16     ret_value = D2_ERROR;
    signed32     val[D20_NOVAL];
    signed32     length;

    D20_offset **poff;
    D20_offset **pmax;
    D20_offset  *off;
    D20_offset  *moff;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

				/*  rewind internal dump file         */
if                              /*  lseek fails                       */
   (lseek(d20_dp_fd, 0L, SEEK_SET) == -1)
{                               /*  return(D2_ERROR)                  */
  close(d20_dp_fd);
  d20_head->d2_errvalue = D20_FILE_ERR;
  d20_dp_fd = -1;
  return(D2_ERROR);
}                               

				/*  read no. of DNs, aliases, length of DNs*/
if                              /*  reading is successful             */
   (d20_120_read_delupd_info(file, val, del_info) == D2_NOERROR)
{                               
  *dn_no    = D2_LDLONG(val[0]);
  length    = D2_LDLONG(val[1]);
  *attr_len = D2_LDLONG(val[2]);

				/*  allocate memory for DNs, aliases, */
				/*  offset structures                 */
  *pdn = *dn = (D20_offset *) 0;
  *names = (char *) 0;

  d20_head->d2_errvalue = D2_NOERROR;
  if (*dn_no != 0)
  {   
    if                          /*  operation fails                   */
       ((*pdn = (D20_offset *) malloc((size_t) *dn_no * sizeof(String)))
	      == NULL ||
	(*dn = (D20_offset *) malloc((size_t) *dn_no * sizeof(D20_offset)))
	      == NULL)
    {                           /*  set error code                    */
      d20_head->d2_errvalue = D20_ALLOC_ERR;
    } 
  } 
  if (d20_head->d2_errvalue != D20_ALLOC_ERR)
  {   
    if
       ((*names = malloc((size_t) length)) == NULL)
    {                           /*  set error code                    */
      d20_head->d2_errvalue = D20_ALLOC_ERR;
    } 
  } 

  if (d20_head->d2_errvalue != D20_ALLOC_ERR)
  {                             /*  build (DN-offset) - pointer array */
    for (poff = (D20_offset **) *pdn, pmax = poff + *dn_no, off = *dn;
	 poff < pmax; poff++, off++)
    { 
      *poff = off;
    } 
				/*  read DN-offsets                   */
    if                          /*  operation is successful           */
       (!d20_107_read(d20_dp_fd,(char *) *dn, *dn_no * sizeof(D20_offset)))
    {                           /*  read DN-strings                   */
	ret_value = d20_107_read(d20_dp_fd, *names, length);
    }                           
  }                             
}                               

if (ret_value)
{   
  if (d20_head->d2_errvalue == D2_NO_ERR)
  {   
    d20_head->d2_errvalue = D20_FILE_ERR;
  } 
  d20_111_free((char *)*pdn, (char *)*dn, *names, (char *)NULL, (char *)NULL,
	       (char *)NULL, (char *)NULL,
	       (char *)NULL, (char *)NULL, (char *)NULL);
}
else
{
  for (off = *dn, moff = off + *dn_no; off < moff; off++)
  { 
    off->d20_file_off = D2_LDLONG(off->d20_file_off);
    off->d20_mem_off  = D2_LDLONG(off->d20_mem_off);
    off->d20_attr_len = D2_LDLONG(off->d20_attr_len);
    off->d20_state    = D2_LDLONG(off->d20_state);
  } 
} 
close(d20_dp_fd);
d20_dp_fd = -1;

				/*  return(ret_value)                 */
return(ret_value);

}                               /*  d20_116_read_dump_file()          */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_118_write_dump_file()                             */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        26.04.88                                              */
/*								      */
/* SYNTAX:      signed16 d20_118_write_dump_file(file, pdn, dn_no,    */
/*                                            names, l_names,         */
/*                                            attr_len, del_info)     */
/*              String       file;                                    */
/*              D20_offset **pdn;                                     */
/*              signed32     dn_no;                                   */
/*              char        *names;                                   */
/*              signed32     l_names;                                 */
/*              signed32     attr_len;                                */
/*              D20_delupd_info *del_info;                            */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will store the internally generated     */
/*              DN-list (with offset in file and memory area) in a    */
/*              special file.                                         */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              String       file;      filename of file to be created*/
/*              D20_offset **pdn;       pointer list (of DNs)         */
/*              signed32     dn_no;     number of DNs                 */
/*              char        *names;     memory area of DN-strings     */
/*              signed32     l_names;   length of all DNs             */
/*              signed32     attr_len;  max. length of longest        */
/*                                      attribute list                */
/*              D20_delupd_info *del_info;  information to be used    */
/*                                          for "delta_updates"       */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:							      */
/*								      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       operation fails                               */
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

 signed16 d20_118_write_dump_file(
   String            file,
   D20_offset      **pdn,
   signed32          dn_no,
   char             *names,
   signed32          l_names,
   signed32          attr_len,
   D20_delupd_info  *del_info)

{                               /*  d20_118_write_dump_file()         */

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

 D20_offset **max_off;
 signed16     ret_value;
 int          fd;
 signed32     val[D20_NOVAL];
 char         d_file[D20_FILE_MAX];

 signed32     obj_len;
 signed32     dsa_len;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

d20_n_bytes = 0;
sprintf(d_file, "%s_1",file);

if                              /*  open of file fails                */
   ((fd = open(d_file, O_WRONLY | O_TRUNC | O_CREAT, 0666)) == -1)
{                               /*  return(D2_ERROR)                  */
  return(D2_ERROR);
}
else
{                               /*  store number of DNs, aliases, DN-length*/
  D2_STLONG(val[0], (signed32) dn_no);
  D2_STLONG(val[1], l_names);
  D2_STLONG(val[2], attr_len);
  D2_STLONG(val[3], (signed32) del_info->d20_sobj);
  obj_len = D2_ALIGN(strlen(del_info->d20_obj) + 1);
  D2_STLONG(val[4], obj_len);
  dsa_len = D2_ALIGN(strlen(del_info->d20_dsa) + 1);
  D2_STLONG(val[5], dsa_len);
  D2_STLONG(val[6], (signed32) del_info->d20_dir_id);
  D2_STLONG(val[7], del_info->d20_time_stamp);

  if                            /*  operation fails                   */
     (d20_117_write(fd, (char *) val, (signed32) sizeof(val), FALSE))
  {                             /*  return(D2_ERROR)                  */
    close(fd);
    return(D2_ERROR);
  }                             

				/*  store object/subtree name         */
  if                            /*  operation fails                   */
     (d20_117_write(fd, del_info->d20_obj, obj_len, FALSE))
  {                             /*  return(D2_ERROR)                  */
    close(fd);
    return(D2_ERROR);
  }                             

				/*  store DSA name                    */
  if                            /*  operation fails                   */
     (d20_117_write(fd, del_info->d20_dsa, dsa_len, FALSE))
  {                             /*  return(D2_ERROR)                  */
    close(fd);
    return(D2_ERROR);
  }                             
}                               

for                             /*  all DNs                           */
    (max_off = pdn + (signed32) dn_no; pdn < max_off; pdn++)
{                               /* convert DN info                    */
  D2_STLONG( (*pdn)->d20_file_off, (*pdn)->d20_file_off );
  D2_STLONG( (*pdn)->d20_mem_off, (*pdn)->d20_mem_off );
  D2_STLONG( (*pdn)->d20_attr_len, (*pdn)->d20_attr_len );
  D2_STLONG( (*pdn)->d20_state, (*pdn)->d20_state );
				/*  store file and memory offsets (DN)*/
  if                            /*  operation fails                   */
     (d20_117_write(fd, (char *) *pdn, (signed32) sizeof(D20_offset), FALSE))
  {                             /*  return(D2_ERROR)                  */
    close(fd);
    return(D2_ERROR);
  }                             
}                               

				/*  store DN strings                  */
ret_value = d20_117_write(fd, names, l_names, TRUE);

				/*  close file                        */
close(fd);

				/*  return(ret_value);                */
return(ret_value);

}                               /*  d20_118_write_dump_file()         */


/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_120_read_delupd_info()                            */
/*								      */
/* AUTHOR:      Schmid (D AP 11)                                      */
/* DATE:        07.03.89                                              */
/*								      */
/* SYNTAX:      signed16 d20_120_read_delupd_info(file, val, del_info)*/
/*              String           file;                                */
/*              signed32        *val;                                 */
/*              D20_delupd_info *del_info;                            */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function reads the internally created dump file  */
/*              (of DSA1).                                            */
/*              This internal dump file contains the offsets of DNs   */
/*              and alias objects in the "search dump" file and in    */
/*              memory. There are also all DN strings.                */
/*              This function reads all the information needed for    */
/*              "delta_updates". (e. g., object interpretation,       */
/*              object/subtree name, master DSA, directory ID, time   */
/*              stamp of dumping)                                     */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              String        file;     file name containing internal */
/*                                      dump information              */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              signed32        *val;       "longs" read from internal*/
/*                                          file                      */
/*              D20_delupd_info *del_info;  information to be used    */
/*                                          for "delta_updates"       */
/*                                                                    */
/* RETURNVALUE:							      */
/*								      */
/*      D2_NOERROR:     No error occurred                             */
/*      D2_ERROR:       operation fails                               */
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

 signed16 d20_120_read_delupd_info(
   String           file,
   signed32        *val,
   D20_delupd_info *del_info)

{                               /*  d20_120_read_delupd_info()        */

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

    char         d_file[D20_FILE_MAX];
    signed32     obj_len;       /* incl. EOS                      */
    signed32     dsa_len;       /* incl. EOS                      */

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

				/*  ret_value = D2_ERROR              */
if                              /*  dump file not yet opened          */
   (d20_dp_fd == -1)
{                               /*  open internal dump file           */
  sprintf(d_file, "%s_1",file);
  if                            /*  open of internal dump file fails  */
     ((d20_dp_fd = open(d_file, O_RDONLY)) == -1)
  {                             /*  return(D2_ERROR)                  */
    if (errno == EACCES || errno == ENOENT)
    {   
      d20_head->d2_retcode = D2_CALL_ERR;
      d20_head->d2_errclass = D2_PAR_ERR;
      d20_head->d2_errvalue = D20_NOFILE;
    }
else
{
      d20_head->d2_retcode = D2_PERM_ERR;
      d20_head->d2_errclass = D2_SYS_ERR;
      d20_head->d2_errvalue = D20_FILE_ERR;
    } 
    return(D2_ERROR);
  }                             
}                               

				/*  read no. of DNs, aliases, length of DNs*/
d20_n_bytes = 0;
if                              /*  operation is ok                   */
   (!d20_107_read(d20_dp_fd, (char *) val, D20_NOVAL * sizeof(signed32)))
{   
  del_info->d20_sobj    = (Bool) D2_LDLONG(val[3]);
  obj_len   = D2_LDLONG(val[4]);
  dsa_len   = D2_LDLONG(val[5]);
  del_info->d20_dir_id     = (signed16) D2_LDLONG(val[6]);
  del_info->d20_time_stamp = D2_LDLONG(val[7]);

				/*  read object / subtree name        */
  if (obj_len > (signed32)D2_ALIGN(D2_DNL_MAX))
  {   
    d20_head->d2_retcode = D2_PERM_ERR;
    d20_head->d2_errclass = D2_SYS_ERR;
    d20_head->d2_errvalue = D20_ALLOC_ERR;
    return(D2_ERROR);
  } 
  if                            /*  operation fails                   */
     (d20_107_read(d20_dp_fd, del_info->d20_obj, obj_len))
  {                             /*  return(D2_ERROR)                  */
    d20_head->d2_retcode = D2_PERM_ERR;
    d20_head->d2_errclass = D2_SYS_ERR;
    d20_head->d2_errvalue = D20_FILE_ERR;
    return(D2_ERROR);
  }                             

				/*  read master DSA name              */
  if (dsa_len > (signed32)D2_ALIGN(D2_DNL_MAX))
  {   
    d20_head->d2_retcode = D2_PERM_ERR;
    d20_head->d2_errclass = D2_SYS_ERR;
    d20_head->d2_errvalue = D20_ALLOC_ERR;
    return(D2_ERROR);
  } 
  if                            /*  operation fails                   */
     (d20_107_read(d20_dp_fd, del_info->d20_dsa, dsa_len))
  {                             /*  return(D2_ERROR)                  */
    d20_head->d2_retcode = D2_PERM_ERR;
    d20_head->d2_errclass = D2_SYS_ERR;
    d20_head->d2_errvalue = D20_FILE_ERR;
    return(D2_ERROR);
  }                             
}                               

return(D2_NOERROR);

}                               /*  d20_120_read_delupd_info()        */
