/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d20ldsh.c,v $
 * Revision 1.1.10.2  1996/02/18  19:39:26  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:20:27  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:13:41  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:50:43  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:04:26  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:04:23  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  15:57:13  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:19:53  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  09:12:22  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:16:25  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  17:47:10  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:00:56  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  13:54:58  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  15:02:32  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  21:42:10  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  19:10:44  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d20ldsh.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:39:26 $";
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
/* NAME         : d20ldsh.o [Load locally stored DSA scheme file]     */
/*								      */
/* AUTHOR       : SANJAY JAIN (Siemens India) 			      */
/* DATE         : 26.11.90 					      */
/*								      */
/* COMPONENT    : DS                                                  */
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
/* HISTORY      :                                                     */
/*								      */
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
#include <stdlib.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2shm.h>
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

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*								      */
/**********************************************************************/

static signed32 d20_at_load(FILE *fp, Errcode *return_code, signed32 *no_at,
			At **at);
static signed32 d20_oct_load(FILE *fp, Errcode *return_code, signed32 *no_oct,
			Oct **oct);
static signed32 d20_srt_load(FILE *fp, Errcode *return_code, signed32 *no_srt,
			Srt **srt);

/*exon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : gds_ldsh()                                   	      */
/*								      */
/* AUTHOR:      SANJAY JAIN (Siemens India)			      */
/* DATE:        05.11.90					      */
/*								      */
/* SYNTAX:      signed32 gds_ldsh(return_code, version, file_name,    */
/*			     no_srt, srt,			      */
/*			     no_oct, oct,			      */
/*			     no_at, at)       		              */
/*		Errcode		*return_code;			      */
/*		signed16	 version;			      */
/*		char		*file_name;			      */
/*		signed32	*no_srt;			      */
/*		Srt		**srt;			      	      */
/*		signed32	*no_oct;			      */
/*		Oct		**oct;			      	      */
/*		signed32	*no_act;			      */
/*		At		**at;			      	      */
/*								      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function will load a locally created DSA scheme  */
/*		file AT,OCT and SRT.				      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*		signed16	 version;			      */
/*		char		*file_name;			      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		Errcode		*return_code;			      */
/*		signed32	*no_srt;			      */
/*		Srt		**srt;			      	      */
/*		signed32	*no_oct;			      */
/*		Oct		**oct;			      	      */
/*		signed32	*no_act;			      */
/*		At		**at;			      	      */
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

 signed32 gds_ldsh(
   Errcode   *return_code,
   signed16   version,
   char      *file_name,
   signed32  *no_srt,
   Srt      **srt,
   signed32  *no_oct,
   Oct      **oct,
   signed32  *no_at,		
   At       **at)

{                               /*  gds_ldsh()               */

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

   d2_ret_val   ret_value = D2_NOERROR;
   FILE    *fp;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

d20_errcode(return_code, D2_NOERROR, D2_NOERROR, D2_NOERROR);

if                              /*  version is wrong                  */
    (version != DS_V03)
{                               /*  set error values                  */
  d20_errcode(return_code, D2_CALL_ERR, D2_ILL_VERS, DS_NOT_SPEC);
  ret_value = D2_ERROR;
}
else if                         /*  open of local scheme file fails   */
   ((fp = fopen(file_name, "r")) == (FILE *) NULL)
{                               /*  set error values                  */
  d20_errcode(return_code, D2_TEMP_ERR, D2_SYS_ERR, DS_NO_FILE);
  ret_value = D2_ERROR;
}
else
{                               /*  load scheme from local file       */
  if                            /*  AT loading ok                     */
     ((ret_value = d20_at_load(fp, return_code,no_at,at)) == D2_NOERROR)
  {                             /*  load AT                           */
    if                          /*  OCT loading ok                    */
       ((ret_value = d20_oct_load(fp, return_code,no_oct,oct)) == D2_NOERROR)
    {   
				/* Load OCT */
      	/*  load SRT                */
        ret_value = d20_srt_load(fp, return_code,no_srt,srt);
    }                           
  }                             
				/*  close local scheme file           */
  fclose(fp);
}                               

				/*  return(ret_value)                 */
return(ret_value);

}                               /*  gds_ldsh */

/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_at_load()                                         */
/*								      */
/* AUTHOR:      SANJAY JAIN (Siemens India)                           */
/* DATE:        05.11.90                                              */
/*								      */
/* SYNTAX:      signed32 d20_at_load(fp, return_code, no_at, at)      */
/*              File    *fp;                                          */
/*		Errcode *return_code;				      */
/*		signed32 *no_at;			      	      */
/*		Oct	**at;					      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function reads AT from DSA scheme file.          */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              File   *fp;         file pointer to internal file     */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		*return_code      - return code			      */
/*              *no_at            - number of entries in AT           */
/*              *at               - pointer to first element in AT    */
/*								      */
/* RETURNVALUE:							      */
/*              D2_ERROR:   error on reading AT or allocating memory  */
/*              D2_NOERROR: no errors occurred                        */
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
/*inoff ***************************************************************/

 static signed32 d20_at_load(
   FILE      *fp,
   Errcode   *return_code,
   signed32  *no_at,
   At       **at)

{                               /*  d20_at_load()                    */

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

register At   *at_ent;
     char     *mem;
     signed16  i;
     signed32  anz;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

*at = (At *)NULL;
*no_at = 0;

				/*  read number of AT entries         */
if                              /*  error occurred                    */
   (fscanf (fp, DSAAT_HEAD, &anz) != 1)
{                               /*  set error values                  */
				/*  return(D2_ERROR)                  */
  d20_errcode(return_code, D2_TEMP_ERR, D2_SYS_ERR, DS_FILE_ERR);
  return(D2_ERROR);
}
else
{                               
				/*  allocate memory                   */
  mem = malloc(((anz + D2_A_MORE) * sizeof(At)) + sizeof(signed32)); 
  if                            /*  no memory for AT         	      */
     (mem == (char *) NULL)
  {                             /*  set error values                  */
				/*  return(D2_ERROR)                  */
    d20_errcode(return_code, D2_TEMP_ERR, D2_SYS_ERR, DS_ALLOC_ERROR);
    return(D2_ERROR);
  }
else
{                             
    for                         /*  all AT entries                   */
	(i = 0, at_ent = (At *) mem; i < anz; i++, at_ent++)
    {                           /*  read AT entry from file            */
      if                        /*  no errors occurred                */
	 (fscanf(fp, DSAAT_FORMAT, at_ent->att_abbrv, at_ent->att_obj_id,
	  	               at_ent->att_name, &at_ent->att_lower, 
			       &at_ent->att_upper, &at_ent->att_no_recur, 
			       &at_ent->att_syntax, &at_ent->att_acl,
			       &at_ent->att_ind_lev, &at_ent->att_phon)
		 != D2_DSA_NUMAT)
      {                         /*  set error values                  */
				/*  return(D2_ERROR)                  */
	d20_errcode(return_code, D2_TEMP_ERR, D2_SYS_ERR, DS_FILE_ERR);
	free(mem);
	return(D2_ERROR);
      }                         
    }                           

    *(signed32 *)((At *)mem + anz) = anz + D2_A_MORE; 
    *at = (At *) mem;
    *no_at = anz;
    return(D2_NOERROR);
  }                             
}                               

}                               /*  d20_at_load()                    */

/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_oct_load()                                        */
/*								      */
/* AUTHOR:      SANJAY JAIN (Siemens India)                           */
/* DATE:        05.11.90                                              */
/*								      */
/* SYNTAX:      signed32 d20_oct_load(fp, return_code, no_oct, oct)   */
/*              File    *fp;                                          */
/*		Errcode *return_code;				      */
/*		signed32 *no_oct;				      */
/*		Oct	**oct;					      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function reads OCT from DSA scheme file.         */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              File   *fp;         file pointer to internal file     */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		*return_code      - return code			      */
/*              *no_oct           - number of entries in OCT          */
/*              *oct              - pointer to first element in OCT   */
/*								      */
/* RETURNVALUE:							      */
/*              D2_ERROR:   error on reading OCT or allocating memory */
/*              D2_NOERROR: no errors occurred                        */
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
/*inoff ***************************************************************/

 static signed32 d20_oct_load(
   FILE      *fp,
   Errcode   *return_code,
   signed32  *no_oct,
   Oct      **oct)

{                               /*  d20_oct_load()                    */

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

register Oct  *oct_ent;
     char     *mem;
     signed16  i,j;
     signed32  anz;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

*oct = (Oct *)NULL;
*no_oct = 0;
				/*  read number of ODT entries        */
if                              /*  error occurred                    */
   (fscanf (fp, DSAOCT_HEAD, &anz) != 1)
{                               /*  set error values                  */
				/*  return(D2_ERROR)                  */
  d20_errcode(return_code, D2_TEMP_ERR, D2_SYS_ERR, DS_FILE_ERR);
  return(D2_ERROR);
}
else
{                               
				/*  allocate memory                   */
  mem = malloc(((anz + D2_O_MORE) * sizeof(Oct)) + sizeof(signed32)); 
  if                            /*  no memory for OCT         	      */
     (mem == (char *) NULL)
  {                             /*  set error values                  */
				/*  return(D2_ERROR)                  */
    d20_errcode(return_code, D2_TEMP_ERR, D2_SYS_ERR, DS_ALLOC_ERROR);
    return(D2_ERROR);
  }
else
{                             
    for                         /*  all OCT entries                   */
	(i = 0, oct_ent = (Oct *) mem; i < anz; 
					i++, oct_ent++, fscanf(fp, "\n"))
    {                           /*  read OCT entry from file            */
      if                        /*  no errors occurred                */
	 (fscanf(fp, DSAOCT_FORMAT, oct_ent->obj_abbrv, oct_ent->obj_id, 
				oct_ent->obj_name, &oct_ent->obj_class_kind, 
			       &oct_ent->obj_file_nr, &oct_ent->no_sup_class, 
			       &oct_ent->no_aux_class, &oct_ent->no_must_attr, 
			       &oct_ent->no_may_attr)
		 != D2_DSA_NUMOCT)
      {                         /*  set error values                  */
				/*  return(D2_ERROR)                  */
	d20_errcode(return_code, D2_TEMP_ERR, D2_SYS_ERR, DS_FILE_ERR);
	free(mem);
	return(D2_ERROR);
      }
else
{
        for                         /*  all superior classes 	      */
            (j = 0,fscanf(fp,"%*s"); j < oct_ent->no_sup_class; j++)
	{ 
          if                        /*  no errors occurred            */
	      (fscanf(fp, "%s", oct_ent->sup_class[j]) != 1)
          {                         /*  set error values              */
				    /*  return(D2_ERROR)              */
	    d20_errcode(return_code, D2_TEMP_ERR, D2_SYS_ERR, DS_FILE_ERR);
	    free(mem);
	    return(D2_ERROR);
          }                         
        }                           

        for                         /*  all auxiliary classes 	*/
            (j = 0,fscanf(fp,"%*s"); j < oct_ent->no_aux_class; j++)
	{ 
          if                        /*  no errors occurred                */
	      (fscanf(fp, "%s", oct_ent->aux_class[j]) != 1)
          {                         /*  set error values                  */
				/*  return(D2_ERROR)                  */
	    d20_errcode(return_code, D2_TEMP_ERR, D2_SYS_ERR, DS_FILE_ERR);
	    free(mem);
	    return(D2_ERROR);
          }                         
        }                           

        for                         /*  all mandatory attributes */
            (j = 0,fscanf(fp,"%*s"); j < oct_ent->no_must_attr; j++)
	{ 
          if                        /*  no errors occurred                */
	      (fscanf(fp, "%s", oct_ent->must_attr[j]) != 1)
          {                         /*  set error values                  */
				/*  return(D2_ERROR)                  */
	    d20_errcode(return_code, D2_TEMP_ERR, D2_SYS_ERR, DS_FILE_ERR);
	    free(mem);
	    return(D2_ERROR);
          }                         
        }                           

        for                         /*  all optional attributes */
            (j = 0,fscanf(fp,"%*s"); j < oct_ent->no_may_attr ; j++)
	{ 
          if                        /*  no errors occurred                */
	      (fscanf(fp, "%s", oct_ent->may_attr[j]) != 1)
          {                         /*  set error values                  */
				/*  return(D2_ERROR)                  */
	    d20_errcode(return_code, D2_TEMP_ERR, D2_SYS_ERR, DS_FILE_ERR);
	    free(mem);
	    return(D2_ERROR);
          }                         
        }                           

      }                         
    }                           

    *(signed32 *)((Oct *)mem + anz) = anz + D2_O_MORE; 
    *oct = (Oct *) mem;
    *no_oct = anz;
    return(D2_NOERROR);
  }                             
}                               

}                               /*  d20_oct_load()                    */


/*inon ****************************************************************/
/*								      */
/* TYPE:        C-FUNCTION                                            */
/*								      */
/* NAME       : d20_srt_load()                                        */
/*								      */
/* AUTHOR:      SANJAY JAIN (Siemens India)                           */
/* DATE:        05.11.90                                              */
/*								      */
/* SYNTAX:      signed32 d20_srt_load(fp,return_code,no_srt,srt)      */
/*              File     *fp;                                         */
/*		Errcode  *return_code;				      */
/*		signed32 *no_srt;				      */
/*		Srt	**srt;					      */
/*                                                                    */
/* DESCRIPTION:                                                       */
/*              This function reads SRT from DSA scheme file. 	      */
/*								      */
/* INPUT-PARAMETERS:                                                  */
/*              File   *fp;         file pointer to internal file     */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*		*return_code      - return_code			      */
/* 		*no_srt           - number of entries in SRT	      */
/*              *srt		  - pointer to chained SRT            */
/*								      */
/* RETURNVALUE:							      */
/*              D2_ERROR:   error on reading SRT or allocating memory */
/*              D2_NOERROR: no errors occurred                        */
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
/*inoff ***************************************************************/

 static signed32 d20_srt_load(
   FILE      *fp,
   Errcode   *return_code,
   signed32  *no_srt,
   Srt      **srt)

{                               /*  d20_srt_load()                    */

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
     Srt      *srt_ent;
     char     *mem;
     signed16  i,j;
     signed32  anz;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

*srt = (Srt *)NULL;
*no_srt = 0;

				/*  read number of SRT entries        */
if                              /*  error occurred                    */
   (fscanf (fp, DSASRT_HEAD, &anz) != 1)
{                               /*  set error values                  */
				/*  return(D2_ERROR)                  */
  d20_errcode(return_code, D2_TEMP_ERR, D2_SYS_ERR, DS_FILE_ERR);
  return(D2_ERROR);
}
else
{                               
				/*  allocate memory                   */
  mem = malloc(((anz + D2_S_MORE) * sizeof(Srt)) + sizeof(signed32));
  if                            /*  no memory for SRT         */
     (mem == (char *) NULL)
  {                             /*  set error values                  */
				/*  return(D2_ERROR)                  */
    d20_errcode(return_code, D2_TEMP_ERR, D2_SYS_ERR, DS_ALLOC_ERROR);
    return(D2_ERROR);
  }
else
{                             
    for                         /*  all SRT entries                   */
	(i = 0, srt_ent = (Srt *) mem; i < anz; 
					i++, srt_ent++, fscanf(fp, "\n"))
    {                           /*  read SRT entry from file          */
      if                        /*  no errors occurred                */
	 (fscanf(fp, DSASRT_FORMAT, &srt_ent->rule_nr, &srt_ent->par_rule_nr,
			       	    &srt_ent->no_nam_attr)
		 != D2_DSA_NUMSRT)
      {                         /*  set error values                  */
				/*  return(D2_ERROR)                  */
	d20_errcode(return_code, D2_TEMP_ERR, D2_SYS_ERR, DS_FILE_ERR);
	free(mem);
	return(D2_ERROR);
      }
else
{
				/* load naming attributes.	*/
	for (j=0,fscanf(fp,"%*s"); j<srt_ent->no_nam_attr; j++)
	{ 
          if                    /*  no errors occurred          */
	     (fscanf(fp, "%s", srt_ent->nam_attr[j]) != 1)
          {                     /*  set error values            */
				/*  return(D2_ERROR)            */
	    d20_errcode(return_code, D2_TEMP_ERR, D2_SYS_ERR, DS_FILE_ERR);
	    free(mem);
	    return(D2_ERROR);
	   } 
	} 
				/* load object class.	*/
	fscanf(fp, "%*s");
        if                      /*  no errors occurred  */
	     (fscanf(fp, "%s", srt_ent->obj_class) != 1)
        {                       /*  set error values    */
				/*  return(D2_ERROR)    */
	    d20_errcode(return_code, D2_TEMP_ERR, D2_SYS_ERR, DS_FILE_ERR);
	    free(mem);
	    return(D2_ERROR);
	} 

      }                         
    }                           

    *(signed32 *)((Srt *)mem + anz) = anz + D2_S_MORE; 
    *srt = (Srt *) mem;
    *no_srt = anz;
    return(D2_NOERROR);
  }                             
}                               

}                               /*  d20_srt_load()                    */
