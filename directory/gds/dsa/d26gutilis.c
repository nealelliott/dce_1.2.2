/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26gutilis.c,v $
 * Revision 1.1.523.2  1996/02/18  19:45:22  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:26:37  marty]
 *
 * Revision 1.1.523.1  1995/12/08  15:45:12  root
 * 	Submit OSF/DCE 1.2.1
 * 
 * 	HP revision /main/mothra_work/1  1995/07/07  22:58 UTC  dtruong
 * 	Correct uninitialize variable
 * 	[1995/12/08  15:07:01  root]
 * 
 * Revision 1.1.521.4  1994/10/05  15:39:07  keutel
 * 	OT 12481: multiple routing
 * 	[1994/10/05  14:36:27  keutel]
 * 
 * Revision 1.1.521.3  1994/06/21  14:45:30  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:32:22  marrek]
 * 
 * Revision 1.1.521.2  1994/05/10  15:52:17  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:22:08  marrek]
 * 
 * Revision 1.1.521.1  1994/02/22  16:05:53  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:13:35  marrek]
 * 
 * Revision 1.1.519.2  1993/10/14  17:16:30  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:10:33  keutel]
 * 
 * Revision 1.1.519.1  1993/10/13  17:30:48  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:33:16  keutel]
 * 
 * Revision 1.1.4.2  1993/08/10  14:11:41  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:10:40  marrek]
 * 
 * Revision 1.1.2.4  1993/02/02  15:38:30  marrek
 * 	Resubmit due to previous bsubmit merge error.
 * 	[1993/02/02  15:34:09  marrek]
 * 
 * Revision 1.1.2.3  1993/02/02  14:49:21  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.2.2  1993/02/02  11:23:05  marrek
 * 	Creation.
 * 	[1993/02/01  17:05:34  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26gutilis.c,v $ $Revision: 1.1.523.2 $ $Date: 1996/02/18 19:45:22 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/**********************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : d26gutilis.c                                        */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 11.01.93                                            */
/*                                                                    */
/* DESCRIPTION  : The module contains the C-ISAM specific utilities,  */
/*                that are called by d26gendb.                        */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | ID | CR# EM# */
/*      1.0 | 11.01.93| Birth                          | ek |         */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                    I N C L U D E S                                 */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

#include <errno.h>
#include <string.h>

/*****  external Includes    *****/

#include <d2shm.h>              /* Scheme definitions                 */
#include <dce/d27util.h>

/*****  internal Includes    *****/

#include <d26schema.h>
#include <d26dnlis.h>
#include <d26gendb.h>
#include <d26isam.h>
#include <d26svcis.h>

/**********************************************************************/
/*                                                                    */
/*                    D E F I N E S                                   */
/*                                                                    */
/**********************************************************************/

				/* length of ISAM record header       */
#define ISAM_HDR      (INTSIZE + LONGSIZE)

				/* max key part size for ISAM indices */
#define MAX_PART_SIZE (MAXKEYSIZE - LONGSIZE - INTSIZE)

				/* position of time stamp: header + ..*/
#define TMPOS       (ISAM_HDR + \
				/* ... name of schema object, ...     */\
		     INTSIZE + D26_LPHON + INTSIZE + 2 * (D26_CNL + 1) + \
				/* ... aliased object                 */\
		     INTSIZE + LONGSIZE)

				/* position of SRT  attribute         */
#define NSPOS       (TMPOS + \
				/* ... Time-Stamp                     */\
		     INTSIZE + D26_TLEN)

				/* position of OCT       attribute    */
#define NOPOS       (NSPOS + \
				/* ... structure-rule-table           */\
		     INTSIZE)

				/* position of AT       attribute     */
#define NAPOS       (NOPOS + \
				/* ... object-class-table             */\
		     INTSIZE)
				/* position of object-class attribute */
#define OCPOS       (NAPOS + \
				/* ... attribute-table                */\
		     INTSIZE + \
				/* ... master-knowledge, ...          */\
		     INTSIZE + LONGSIZE + \
				/* ... ACL, ...                       */\
		     INTSIZE + (D2_ACLIDX_MAX * LONGSIZE))

				/* length of scheme record            */
#define SRECL       (OCPOS + \
				/* ... object class                   */\
		     INTSIZE + D26_OCL_VAL_COUNT * LONGSIZE)

/**********************************************************************/
/*                                                                    */
/*                    T Y P E D E F S                                 */
/*                                                                    */
/**********************************************************************/

typedef struct                  /* information about length and index */
	{                       /* of attributes                      */
	  signed16 len;          /* total value len incl. phonetic part*/
	  signed16 idx_len;      /* CISAM index length                 */
	  signed16 idx_type;     /* CISAM index type                   */
	} Attr_info;

/**********************************************************************/
/*                                                                    */
/*                    D E C L A R A T I O N S                         */
/*                                                                    */
/**********************************************************************/

/******* EXPORT  ********/

			/* File descriptor array for object<> files   */
D26_fildes *d26_ofildes;
			/* File descriptor array for attr<> files     */
D26_fildes *d26_afildes;
signed32 d26_oreclen;        /* maximum length of object record*/
signed32 d26_areclen;        /* maximum length of attr. record */

/******** LOCAL  ********/

			/* Array of attr value lengths and C-ISAM     */
			/* index types for each AT entry.             */
static Attr_info  *attr_lens;

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S         FUNCTIONS           */
/*                                                                    */
/**********************************************************************/

/******** LOCAL  ********/

static signed16 d26_gi11_ld_scheme(char *record, char *scheme_time);
static void     d26_gi12_setup_attr_lens(void);
static signed16 d26_gi13_calc_schema_positions(void);
static signed16 d26_gi14_update_scheme_object(void);
static int      d26_gi15_calc_positions(signed16 filenr);
static signed16 d26_gi16_make_object_file(signed16 filenr, int reclen);
static signed16 d26_gi17_make_attr_file(signed16 filenr, signed16 *reclen);
static signed16 d26_gi18_make_dnlist_file(void);
static signed16 d26_gi19_write_scheme(char *sch_time);
static int      d26_gi21_calc_srt_positions(D26_srt *srtp, signed16 filenr,
    int rec);
static int      d26_gi22_calc_oct_positions(D26_oct *octp, signed16 filenr,
    int rec);
static int      d26_gi23_calc_at_positions(signed16 filenr, signed16 at_idx,
    int rec);
static signed16 d26_gi24_lowest_at_level(signed16 filenr, signed16 par_idx,
    signed16 level, signed16 at_idx);
static int      d26_gi31_create_isam(char *fname, int reclen,
    struct keydesc *key, boolean *created);
static signed16 d26_gi32_make_key(int isfd, signed16 start_pos,
    Attr_info *attr_len, D26_at *atp, signed16 alob_pos, signed16 mk_pos);

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_gi1_read_configuration_data                     */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 11.01.93                                            */
/*                                                                    */
/* DESCRIPTION  :   The function reads the C-ISAM specific            */
/*      configuration data from "conf"-file.                          */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      fp          FILE *          file pointer of conf file         */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      max_attr_val_len short *    maximum length of attribute values*/
/*                                  to be stored at object            */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NOERROR  short           no error occurred                 */
/*      D2_ERROR                    error occurred                    */
/*                                                                    */
/**********************************************************************/

signed16 d26_gi1_read_configuration_data(
    FILE     *fp,
    signed16 *max_attr_val_len)

{                       /* d26_gi1_read_configuration_data            */
  int   var_count;      /* number of actually read variables          */

#ifndef __hpux_13317CHFts
if ((var_count = fscanf(fp,"%*s %hd",max_attr_val_len)) != 1)
#else
if (fscanf(fp,"%*s %hd",max_attr_val_len) != 1)
#endif
{                       /* log error and return                       */
  D27_FPCHECK(svc_c_sev_fatal,fp);
  dce_svc_printf(GDS_S_INSUFF_INFO_MSG,1,var_count);
  return(D2_ERROR);
}
else                    /* return no error                            */
{
  return(D2_NOERROR);
}

}                       /* d26_gi1_read_configuration_data            */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_gi2_read_schema_object                          */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 11.01.93                                            */
/*                                                                    */
/* DESCRIPTION  :   The function reads the schema object from C-ISAM  */
/*      file objects0.                                                */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      None                                                          */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      recnum      long *          number of C-ISAM record containing*/
/*                                  the schema object                 */
/*      schema_time char *          time stamp of schema object       */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NOERROR  short           no error occurred                 */
/*      D2_ERROR                    error occurred                    */
/*                                                                    */
/**********************************************************************/

signed16 d26_gi2_read_schema_object(
    long *recnum,
    char *schema_time)

{                       /* d26_gi2_read_schema_object                 */

			/* filename for C-ISAM file containing schema */
char    filename[D27_LFILE_NAME];

int  isfd;              /* C-ISAM filedescriptor                      */

char    record[SRECL + 1];  /* memory for data record of schema object*/

signed16 ret_value = D2_NOERROR;    /* return value                   */

			/* Open file "objects0"                       */
sprintf(filename,"%s%s%d",dir_fname,D26_OBJECTS,0);
if                      /* objects0 cannot be read                    */
  ((isfd = D26_ISOPEN(svc_c_sev_fatal,filename,ISINOUT + ISEXCLLOCK)) < 0)
{                       /* log error and set return value             */
  ret_value = D2_ERROR;
}
else
{                       /* log success                                */
  if                    /* schema record cannot be read               */
    (D26_ISREAD(svc_c_sev_fatal,isfd,record,ISCURR))
  {                     /* log error and set return value             */
    ret_value = D2_ERROR;
  }
  else
  {                     /* log success                                */
    *recnum = isrecnum;

			/* read information from record               */
    if                  /* schema information cannot be read          */
      (d26_gi11_ld_scheme(record,schema_time) != D2_NO_ERR)
    {                   /* set return value                           */
      ret_value = D2_ERROR;
    }

    if                  /* objects0 cannot be closed                  */
      (D26_ISCLOSE(svc_c_sev_fatal,isfd))
    {                   /* log error and return                       */
      ret_value = D2_ERROR;
    }
  }
}

return(ret_value);

}                       /* d26_gi2_read_schema_object                 */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_gi3_read_attribute_table                        */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 02.12.91                                            */
/*                                                                    */
/* DESCRIPTION  : Read the rows from an attribute file into the       */
/*                appropriate array of structures. Do some checking   */
/*                on the validity of the data, ie. return an error    */
/*                code if the records have too many or too few        */
/*                elements or if the elements are too long.           */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      recnum          long        number of record containing       */
/*                                  schema object                     */
/*      tab_nr          short       number of attribute table to read */
/*      nr_tab_entries  short       number of entries to be read      */
/*      read_tab_entry  (short *)() function to read one table entry  */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NOERROR  short           No error occurred                 */
/*      D2_ERROR                    Error occurred                    */
/*                                                                    */
/**********************************************************************/

signed16 d26_gi3_read_attribute_table(
    long recnum,
    signed16 tab_nr,
    signed16 nr_tab_entries,
    signed16 (*read_tab_entry)(char *, short))

{                       /* d26_gi3_read_attribute_table               */

			/* name of C-ISAM file containing table entr. */
char    fname[D27_LFILE_NAME];
int     isfd;           /* ISAM File descriptor                       */

			/* memory for C-ISAM record containing table  */
			/* entry                                      */
char    record[ISAM_HDR + D2_L_SRT_ATT + D2_L_OCT_ATT + D2_L_AT_ATT + 1];


signed16 ret_value = D2_NOERROR;

register signed16 i;    /* loop variable                              */

sprintf(fname,"%s%s%d",dir_fname,D26_ATTR,tab_nr);

if                      /* ISAM file cannot be opened                 */
  ((isfd = D26_ISOPEN(svc_c_sev_fatal,fname,ISEXCLLOCK + ISINPUT)) < 0)
{                       /* log error and set return value             */
  ret_value = D2_ERROR;
}
else
{                       /* log success                                */
  for                   /* all table entries                          */
     (i = 0; i < nr_tab_entries; i++)
  {                     /* read the record                            */
    if                  /* record cannot be read                      */
      (D26_ISREAD(svc_c_route_nolog,isfd,record,ISNEXT) != 0)
    {                   /* check ISAM error                           */
      if                /* hard C-ISAM error occurred                 */
	(iserrno != EENDFILE)
      {                 /* log error                                  */
	dce_svc_printf(DCE_SVC(gds_svc_handle,"%d%d"),GDS_S_GENERAL,
	  svc_c_sev_warning,GDS_S_ISREAD_ERR,ISNEXT,iserrno);
      }
      break;
    }
    else if           /* serial keys differ                         */
	   (recnum != ldlong(record + INTSIZE))
    {                 /* log error and break                        */
      dce_svc_printf(GDS_S_SCH_INCONSISTENT_MSG);
      break;
    }
    else
    {                 /* set pointer to attribute value             */
      if              /* table entry cannot be converted            */
	(read_tab_entry(record + ISAM_HDR,i) != D2_NO_ERR)
      {               /* break loop                                 */
	break;
      }
    }
  }
  if                  /* table still incomplete                     */
    (i < nr_tab_entries)
  {                   /* set return value                           */
    ret_value = D2_ERROR;
  }
}

return(ret_value);

}                       /* d26_gi3_read_attribute_table               */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_gi6_fildes_alloc                                */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 12.01.93                                            */
/*                                                                    */
/* DESCRIPTION  :   The function allocates memory for the attribute   */
/*      length structures, the object filedescriptor structures and   */
/*      the attribute filedescriptor structures.                      */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      None                                                          */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NOERROR  short           no error occurred                 */
/*      D2_ERROR                    error occurred                    */
/*                                                                    */
/**********************************************************************/

signed16 d26_gi6_fildes_alloc(void)

{                       /* d26_gi6_fildes_alloc                       */

return((attr_lens = (Attr_info *)D27_CALLOC(svc_c_sev_fatal,d26_noat,
	sizeof(Attr_info))) == NULL ||
       (d26_ofildes = (D26_fildes *)D27_CALLOC(svc_c_sev_fatal,d26_nofile,
	sizeof(D26_fildes))) == NULL ||
       (d26_afildes = (D26_fildes *)D27_CALLOC(svc_c_sev_fatal,d26_nafile,
	sizeof(D26_fildes))) == NULL ? D2_ERROR : D2_NOERROR);

}                       /* d26_gi6_fildes_alloc                       */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_gi7_fildes_free                                 */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 12.01.93                                            */
/*                                                                    */
/* DESCRIPTION  :   The function frees the memory for the attribute   */
/*      length structures, the object filedescriptor structures and   */
/*      the attribute filedescriptor structures.                      */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      None                                                          */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      None                                                          */
/*                                                                    */
/**********************************************************************/

void d26_gi7_fildes_free(void)

{                       /* d26_gi7_fildes_free                        */

if                      /* memory for attribute length structures is  */
			/* allocated                                  */
  (attr_lens != (Attr_info *)NULL)
{                       /* free it                                    */
  free(attr_lens);
}

if                      /* memory for object filedescriptor structures*/
			/* is allocated                               */
  (d26_ofildes != (D26_fildes *)NULL)
{                       /* free it                                    */
  free(d26_ofildes);
}

if                      /* memory for attribute filedescriptor        */
			/* structures is allocated                    */
  (d26_afildes != (D26_fildes *)NULL)
{                       /* free it                                    */
  free(d26_afildes);
}

}                       /* d26_gi7_fildes_free                        */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_gi8_create_database                             */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 12.01.93                                            */
/*                                                                    */
/* DESCRIPTION  :   The function creates the empty C-ISAM database    */
/*      according to the read schema.                                 */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      schema_time     char *      time stamp of schema object       */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NOERROR  short           no error occurred                 */
/*      D2_ERROR                    error occurred                    */
/*                                                                    */
/**********************************************************************/

signed16 d26_gi8_create_database(
    char *schema_time)

{                       /* d26_gi8_create_database                    */

  signed16    file_nr;  /* C-ISAM file number                         */
  int         rec_len;  /* record length                              */
  D26_fildes  *fd_p;    /* ptr to filedes arrays                      */

  d26_gi12_setup_attr_lens();

  for                   /* all object file descriptors                */
     (file_nr = 0, fd_p = d26_ofildes, d26_oreclen = 0; file_nr < d26_nofile;
      file_nr++, fd_p++)
  {                     /* initialize file descriptor structure       */
    fd_p->d26_isfd     = -1;
    fd_p->d26_rlen     = 0;
    fd_p->d26_in_use   = FALSE;
    fd_p->d26_last_use = 0L;

    if                  /* schema object file is described            */
      (file_nr == 0)
    {                   /* update objects0 with new SRT index for the */
			/* schema object                              */
      fd_p->d26_rlen = d26_gi13_calc_schema_positions();

      d26_oreclen = d26_oreclen > fd_p->d26_rlen ? d26_oreclen :
						   fd_p->d26_rlen;

      if                /* schema object cannot be updated            */
	(d26_gi14_update_scheme_object() != D2_NO_ERR)
      {                 /* return error                               */
	return(D2_ERROR);
      }
      else
      {                 /* log success                                */
	DCE_SVC_LOG((GDS_S_SCH_UPDATED_MSG));
      }
    }
    else
    {                   /* Calculate positions of naming and other    */
			/* attribute values in a record of a given    */
			/* object file                                */
      fd_p->d26_rlen = (signed16)
	(rec_len = d26_gi15_calc_positions(file_nr));
      DCE_SVC_LOG((GDS_S_CALC_REC_POS_MSG,file_nr,rec_len));
      d26_oreclen = d26_oreclen > fd_p->d26_rlen ? d26_oreclen :
						   fd_p->d26_rlen;

      if                /* record length is 0                         */
	(rec_len == 0)
      {                 /* Don't create object files with no attrs    */
	DCE_SVC_LOG((GDS_S_FILE_AVOIDED_MSG,file_nr));
	continue;
      }

      if                /* object file cannot be created              */
	(d26_gi16_make_object_file(file_nr,rec_len) != D2_NO_ERR)
      {                 /* error exit                                 */
	return(D2_ERROR);
      }
      else
      {                 /* log success                                */
	DCE_SVC_LOG((GDS_S_FILE_CREATED_MSG,D26_OBJECTS,file_nr));
      }
    }
  }
#ifdef DEBUG
  {
    D26_nam_at *nam_atp;
    D26_srt *srtp;
    D26_at  *atp;
    short    j,k;

    fprintf(stderr, "\fNaming positions .............\n\nob_nr na_nr");
    for (file_nr = 0; file_nr < d26_nofile; file_nr++)
	fprintf(stderr,"     [%d]", file_nr);
    fprintf(stderr, "\n");
    for (j = 0, srtp = d26_srt; j < d26_nosrt; j++, srtp++)
    {
	fprintf(stderr, "%3d:\n",j);
	for (k=0, nam_atp = srtp->d26s_nam_at; nam_atp->d26s_index != -1;
	     k++, nam_atp++)
	{
	    fprintf(stderr,"%9d: ",k);
	    for (file_nr=0; file_nr < d26_nofile; file_nr++)
	    {
		fprintf(stderr, "%7d ", nam_atp->d26s_position[file_nr]);
	    }
	    fprintf(stderr, "\n");
	}
    }
    fprintf(stderr, "\n");
    fprintf(stderr, "\fAttribute positions .............\n\n   ");
    for (file_nr = 0; file_nr < d26_nofile; file_nr++)
	fprintf(stderr, "   [%d]", file_nr);
    fprintf(stderr, "\n");
    for (j=0, atp = d26_at; j < d26_noat; j++, atp++)
    {
	fprintf(stderr, "%2d: ",j);
	for (file_nr = 0; file_nr < d26_nofile; file_nr++) {
	    fprintf(stderr, "%5d ", atp->d26a_field_pos[file_nr]);
	}
	fprintf(stderr, "\n");
    }
    fprintf(stderr, "\n");
  }
#endif

  for                   /* all attribute files                        */
     (file_nr = 0, fd_p = d26_afildes, d26_areclen = 0; file_nr < d26_nafile;
      file_nr++, fd_p++)
  {                     /* initialize file descriptor structures      */
    fd_p->d26_isfd     = -1;
    fd_p->d26_rlen     = 0;
    fd_p->d26_in_use   = FALSE;
    fd_p->d26_last_use = 0L;

    if                  /* file contains SRT                          */
      (file_nr == 0)
    {                   /* only calculate record length               */
      fd_p->d26_rlen = ISAM_HDR + D2_L_SRT_ATT + 1;
    }
    else if             /* file contains OCT                          */
	   (file_nr == 1)
    {                   /* only calculate record length               */
      fd_p->d26_rlen = ISAM_HDR + D2_L_OCT_ATT + 1;
    }
    else if             /* file contains AT                           */
	   (file_nr == 2)
    {                   /* only calculate record length               */
      fd_p->d26_rlen = ISAM_HDR + D2_L_AT_ATT + 1;
    }
    else if             /* attribute file cannot be created           */
	   (d26_gi17_make_attr_file(file_nr,&fd_p->d26_rlen) != D2_NO_ERR)
    {                   /* return error                               */
      return(D2_ERROR);
    }
    else
    {                   /* log success                                */
      DCE_SVC_LOG((GDS_S_FILE_CREATED_MSG,D26_ATTR,file_nr));
    }
    d26_areclen = d26_areclen > fd_p->d26_rlen ? d26_areclen :
						 fd_p->d26_rlen;
  }

  if                    /* DN-List file cannot be created             */
    (d26_gi18_make_dnlist_file() != D2_NO_ERR)
  {                     /* return error                               */
    return(D2_ERROR);
  }
  else
  {                     /* log success                                */
    DCE_SVC_LOG((GDS_S_FILE_CREATED_MSG,"/d26dnlis",file_nr));

    if                  /* scheme file cannot be written              */
      (d26_gi19_write_scheme(schema_time) != D2_NO_ERR)
    {                   /* return error                               */
      return(D2_ERROR);
    }
    else
    {                   /* log success                                */
      DCE_SVC_LOG((GDS_S_SCHEME_CREATED_MSG));
    }
  }

return(D2_NOERROR);

}                       /* d26_gi8_create_database                    */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_gi11_ld_scheme                                  */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 21.04.88                                            */
/* UPDATE       : 10.10.90  by Melissa                                */
/*                                                                    */
/* DESCRIPTION  : From known positions in the supplied record, read   */
/*                the scheme time-stamp, and the total number of SRT, */
/*                OCT and AT entries. Close isfd.                     */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*                   char   *record                                   */
/*                                                                    */
/* OUTPUT-PARAMETER: char   *scheme_time                              */
/*                                                                    */
/* RETURNVALUE     :  D2_NO_ERR                                       */
/*                    D26_ERROR                                       */
/*                                                                    */
/**********************************************************************/

static signed16 d26_gi11_ld_scheme(
    char *record,
    char *scheme_time)

{                       /* d26_gi11_ld_scheme                         */

if                      /* no timestamp in schema                    */
  (ldint(record + TMPOS) <= 0)
{
    DCE_SVC_LOG((GDS_S_SCH_INSUFFICIENT_MSG));
    return(D26_ERROR);
}
else
{
    (void)strcpy(scheme_time,record + TMPOS + INTSIZE);
}

if                      /* number of SRT entries isn't valid         */
   ((nr_adm_srt = ldint(record + NSPOS)) <= 0)
{
    DCE_SVC_LOG((GDS_S_SCH_INSUFFICIENT_MSG));
    return(D26_ERROR);
}

if                      /* number of OCT entries isn't valid         */
   ((d26_nooct = (int)ldint(record + NOPOS)) <= 0)
{
    DCE_SVC_LOG((GDS_S_SCH_INSUFFICIENT_MSG));
    return(D26_ERROR);
}

if                      /* number of AT entries isn't valid          */
   ((d26_noat = (int)ldint(record + NAPOS)) <= 0)
{
    DCE_SVC_LOG((GDS_S_SCH_INSUFFICIENT_MSG));
    return(D26_ERROR);
}

d26_noat++;             /* An extra attribute (D2_DNLIST) will be    */
			/* generated internally                      */

return(D2_NO_ERR);

}                       /* d26_gi11_ld_scheme                         */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_gi12_setup_attr_lens                            */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : For all SRT and AT entries, link 'position' fields  */
/*      to d26_name_pos and d26_at_pos arrays. For each attribute     */
/*      in d26_at, calculate the total length required to             */
/*      store the attribute values in object or attr C-ISAM           */
/*      files. If the attr values are held in the object              */
/*      file then the rules for calculating the length is             */
/*      given in the design document. If they are held in             */
/*      attr files, the length function does not include              */
/*      <count> or <nr_recur>.                                        */
/*                                                                    */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      None                                                          */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR   short           No error occurred                 */
/*      D26_ERROR                   Error occurred                    */
/*                                                                    */
/**********************************************************************/

static void d26_gi12_setup_attr_lens(void)

{                       /* d26_gi12_setup_attr_lens                   */

D26_nam_at *nam_at;     /* pointer to naming attributes               */
D26_srt    *srtp;
D26_at     *atp;
signed16   *posp;       /* pointer to a positions array               */
signed16   i,j;         /* loop variables                             */
signed16   val_len;     /* length of a particular attr value          */

			/* for all SRT and AT entrys, link position   */
			/* field to d26_name_pos and d26_at_pos arrays*/

for                     /* all SRT entries                            */
   (i = 0, srtp = d26_srt, posp = d26_name_pos; i < d26_nosrt; i++, srtp++)
{                       /* scan the naming attributes                 */
  for                   /* all naming attributes                      */
     (nam_at = srtp->d26s_nam_at; nam_at->d26s_index != -1; nam_at++)
  {                     /* set the SRT position pointer               */
    nam_at->d26s_position = posp;
    for                 /* all position fields                        */
       (j = 0; j < d26_nofile; j++, posp++)
    {                   /* initialize position                        */
      *posp = 0;
    }
  }
}

for                     /* all AT entries                             */
   (i = 0, atp = d26_at, posp = d26_at_pos; i < d26_noat; i++, atp++)
{                       /* set the AT position pointer                */
    atp->d26a_field_pos = posp;
    for                 /* all position fields                        */
       (j = 0; j < d26_nofile; j++, posp++)
    {                   /* initialize position                        */
      *posp = 0;
    }
}

for                     /* all AT entries                             */
   (i = 0, atp = d26_at; i < d26_noat; i++, atp++)
{
				/* type for C-ISAM index if required  */
    attr_lens[i].idx_type = CHARTYPE;

    switch (atp->d26a_syntax)
    {

	case D2_CEXACT:         /* <value>'\0'                        */
	case D2_PRINTABLE:
	case D2_TIME:
	    val_len = atp->d26a_ubound + 1;
	    attr_lens[i].idx_len = val_len;
	    break;

	case D2_CIGNORE:        /* <norm value>'\0'<exact value>'\0'  */
	case D2_IA5:
	case D2_NUMERIC:
	case D2_TELNR_STX:
	case D2_COUNTRY_STX:
	    val_len = 2 * (atp->d26a_ubound + 1);
	    attr_lens[i].idx_len = val_len / 2;
	    break;

	case D2_ANY_STX:
	case D2_OBJ_IDENT:
	case D2_OCTET:
	case D2_PSAP_STX:
	case D2_GUIDE:
	case D2_TTXID_STX:
	case D2_FAXNR_STX:
	case D2_PASSWD_STX:
	case D2_ASN1:
	case D2_CERTIFICATE:
	case D2_CERT_PAIR:
	case D2_CERT_LIST:
	    if (D2_ATTCMP(&(atp->d26a_type), D2_OBJ_CLASS) == 0)
	    {
				/* <OCT_index>                        */
		val_len = LONGSIZE;
		attr_lens[i].idx_len  = val_len;
		attr_lens[i].idx_type = LONGTYPE;
	    }
	    else
	    {              /* <value><length>                    */
		val_len = atp->d26a_ubound + INTSIZE;
		attr_lens[i].idx_len = val_len - INTSIZE;
	    }
	    break;

	case D2_MHS_DLSP_STX:
	case D2_MHS_ORADR_STX:
	case D2_MHS_ORNAME_STX:
			   /* <normed val><length><exact val><length> */
	    val_len = 2 * (atp->d26a_ubound + INTSIZE);
	    attr_lens[i].idx_len = val_len / 2 - INTSIZE;
	    break;
	case D2_TLXNR_STX:
	    val_len = atp->d26a_ubound + D26_MAXVALPARTS;
	    attr_lens[i].idx_len = val_len;
	    break;

	case D2_CIGN_LIST:      /* <r1><nel1>'\0'..<r10><nel10>'\0'*/
	case D2_POST_AD_STX:    /* <r1><eel1>'\0'..<eel10>'\0'*/
	    val_len = 2 * (atp->d26a_ubound + 2 * D26_MAXVALPARTS);
	    attr_lens[i].idx_len = val_len / 2;
	    break;

	case D2_BOOLEAN:        /* <value>                            */
	case D2_INTEGER:
	    val_len = LONGSIZE;
	    attr_lens[i].idx_len  = val_len;
	    attr_lens[i].idx_type = LONGTYPE;
	    break;

	case D2_PR_DM_STX:      /* <value 1>...<value 10>             */
	case D2_MHS_PR_DM_STX:
	    val_len = LONGSIZE * atp->d26a_ubound;
	    attr_lens[i].idx_len = val_len;
	    break;

	case D2_DISTNAME:       /* <DN index>                         */
	    val_len = LONGSIZE;
	    attr_lens[i].idx_len  = val_len;
	    attr_lens[i].idx_type = LONGTYPE;
	    break;

	case D2_ACCL:           /* <index 1>...<index 20>             */
	    val_len = LONGSIZE * 20;
	    attr_lens[i].idx_len = val_len;
	    break;
    }                           /* end switch (syntax)                */

    if (attr_lens[i].idx_len > MAX_PART_SIZE)
	attr_lens[i].idx_len = MAX_PART_SIZE;

    if                          /* attribute allows phonetical match  */
      (atp->d26a_phon == TRUE)
    {                           /* reset length of value              */
      val_len += D26_LPHON;
    }

    if                          /* representation is a choice         */
      (atp->d26a_rep == D2_T61_PR)
    {                           /* reset length of value              */
      val_len += INTSIZE;
    }

    if (atp->d26a_filenr >= 0)  /* for an attr<> file, record the     */
				/* length of an individual value.     */
	attr_lens[i].len = val_len;
    else                        /* for an objects<> file, record      */
				/* <count> + length of all recurring  */
				/* values.                            */
	attr_lens[i].len = INTSIZE + atp->d26a_nr_recur * val_len;
}

#ifdef DEBUG
{
    Admin_at  *adm_atp;
    short     j;

    fprintf(stderr, "\fAT ............\n\n");
    for (i=0, atp = d26_at; i < d26_noat; i++, atp++)
    {
	for (j=0, adm_atp = admin_at; j < d26_noat; j++, adm_atp++)
	{
	    if (i == adm_atp->link)
		break;
	}
	fprintf(stderr, "%2d: %3s object id %s\n", i, adm_atp->acronym,
	    adm_atp->obj_id);
	fprintf(stderr, "\tphon %2d recur %2d syn %5d ub %5d ",
	    atp->d26a_phon, atp->d26a_nr_recur,
	    atp->d26a_syntax, atp->d26a_ubound);
	fprintf(stderr, "idxtyp %d space in ofile %d\n",
	    attr_lens[i].idx_type,
	    atp->d26a_filenr >= 0 ? INTSIZE : attr_lens[i].len);
	fprintf(stderr, "\tfile %2d imp   %2d rep %5d lb %5d ",
	    atp->d26a_filenr, atp->d26a_importance,
	    atp->d26a_rep, atp->d26a_lbound);
	fprintf(stderr, "level %2d acl %2d\n",
	    atp->d26a_ind_level, atp->d26a_acl);
	if ((i%20) == 19)
	    fprintf(stderr, "\f\n\n");
    }
    fprintf(stderr, "\n");
}
#endif

}                       /* d26_gi12_setup_attr_lens                   */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_gi13_calc_schema_positions                      */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : For the C-ISAM file containing the schema object,   */
/*                work out the C-ISAM                                 */
/*                record positions for all the naming attrs, the alias*/
/*                attr and all the other possible attrs which can be  */
/*                held in this file. Return the total length of the   */
/*                record.                                             */
/*                                                                    */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*                                                                    */
/* OUTPUT-PARAMETER: -                                                */
/*                                                                    */
/* RETURNVALUE     : length of record                                 */
/*                                                                    */
/*                                                                    */
/**********************************************************************/

static signed16 d26_gi13_calc_schema_positions(void)

{                       /* d26_gi13_calc_schema_positions             */

D26_ocl_arc   *ocl_arcp;    /* pointer to object class arc            */
D26_srt       *srtp;
D26_at        *atp;

signed16      rec;          /* current record position in an object   */
			    /* file                                   */
signed16      j;

			/* work out positions of naming and other     */
			/* attributes and store results in            */
			/* d26_name_pos and d26_at_pos                */

rec = ISAM_HDR;         /* start current record position at offset 6  */

			/* find the SRT-entry of schema object        */

for                     /* all SRT entries                            */
   (j = 0, srtp = d26_srt; j < d26_nosrt; j++, srtp++)
{                       /* scan the object class arcs                 */
    for                 /* all object class arcs                      */
       (ocl_arcp = srtp->d26s_ocl_arc; ocl_arcp->d26s_oclass != -1;
	ocl_arcp++)
    {
	if              /* object class for schema object found       */
	  (d26_oct[ocl_arcp->d26s_oclass].d26c_filenr == 0)
	{               /* record current position in d26_name_pos    */
	  srtp->d26s_nam_at->d26s_position[0] =
	      d26_at[srtp->d26s_nam_at->d26s_index].d26a_field_pos[0] = rec;
	  break;
	}
    }

    if                  /* SRT-entry of schema was found              */
      (ocl_arcp->d26s_oclass != -1)
    {                   /* break loop                                 */
	break;
    }
}

rec += INTSIZE + D26_LPHON + INTSIZE + 2 * (D26_CNL + 1);

			/* Finished processing naming attributes, now */
			/* reserve space for the other attributes     */

for                     /* all AT entries                             */
   (j = 0, atp = d26_at; j < d26_noat; j++, atp++)
{                       /* check whether aliased object attribute     */
    if                  /* aliased object attribute found             */
      (D2_ATTCMP(&atp->d26a_type,D2_ALIASED_OBJECT) == 0)
    {                   /* set record position                        */
	atp->d26a_field_pos[0] = rec;
	rec += INTSIZE + LONGSIZE;
	break;
    }
}

for                     /* all AT entries                             */
   (j = 0, atp = d26_at; j < d26_noat; j++, atp++)
{                       /* check whether time stamp attribute         */
    if                  /* time stamp attribute found                 */
      (D2_ATTCMP(&atp->d26a_type,D2_TIME_STAMP) == 0)
    {                   /* set record position                        */
	atp->d26a_field_pos[0] = rec;
	rec += INTSIZE + D26_TLEN;
	break;
    }
}

for                     /* all AT entries                             */
   (j = 0, atp = d26_at; j < d26_noat; j++, atp++)
{                       /* check whether SRT attribute                */
    if                  /* SRT attribute found                        */
      (D2_ATTCMP(&atp->d26a_type,D2_SRT) == 0)
    {                   /* set record position                        */
	atp->d26a_field_pos[0] = rec;
	rec += INTSIZE;
	break;
    }
}

for                     /* all AT entries                             */
   (j = 0, atp = d26_at; j < d26_noat; j++, atp++)
{                       /* check whether OCT attribute                */
    if                  /* OCT attribute found                        */
      (D2_ATTCMP(&atp->d26a_type,D2_OCT) == 0)
    {                   /* set record position                        */
	atp->d26a_field_pos[0] = rec;
	rec += INTSIZE;
	break;
    }
}

for                     /* all AT entries                             */
   (j = 0, atp = d26_at; j < d26_noat; j++, atp++)
{                       /* check whether AT attribute                 */
    if                  /* AT attribute found                         */
      (D2_ATTCMP(&atp->d26a_type,D2_AT) == 0)
    {                   /* set record position                        */
	atp->d26a_field_pos[0] = rec;
	rec += INTSIZE;
	break;
    }
}

for                     /* all AT entries                             */
   (j = 0, atp = d26_at; j < d26_noat; j++, atp++)
{                       /* check whether master knowledge attribute   */
    if                  /* master knowledge attribute found           */
      (D2_ATTCMP(&atp->d26a_type,D2_MASTER_KNOWLEDGE) == 0)
    {                   /* set record position                        */
	atp->d26a_field_pos[0] = rec;
	rec += INTSIZE + LONGSIZE;
	break;
    }
}

for                     /* all AT entries                             */
   (j = 0, atp = d26_at; j < d26_noat; j++, atp++)
{                       /* check whether ACL attribute                */
    if                  /* ACL attribute found                        */
      (D2_ATTCMP(&atp->d26a_type,D2_ACL) == 0)
    {                   /* set record position                        */
	atp->d26a_field_pos[0] = rec;
	rec += INTSIZE + (D2_ACLIDX_MAX * LONGSIZE);
	break;
    }
}

for                     /* all AT entries                             */
   (j = 0, atp = d26_at; j < d26_noat; j++, atp++)
{                       /* check whether object class attribute       */
    if                  /* object class attribute found               */
      (D2_ATTCMP(&atp->d26a_type,D2_OBJ_CLASS) == 0)
    {                   /* set record position                        */
	atp->d26a_field_pos[0] = rec;
	rec += INTSIZE + (D26_OCL_VAL_COUNT * LONGSIZE);
	break;
    }
}

return(rec);

}                       /* d26_gi13_calc_schema_positions             */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_gi14_update_scheme_object                       */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : Search for the scheme object in OCT. The index to   */
/*                the OCT is the object class of the schema. Search   */
/*                for the schema object in SRT. The index to the SRT  */
/*                is the object number of the schema. Update objects0 */
/*                with the new object number and object class.        */
/*                                                                    */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      None                                                          */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR    Short          No error occurred                 */
/*      D26_ERROR                   Error occurred                    */
/*                                                                    */
/**********************************************************************/

static signed16 d26_gi14_update_scheme_object(void)

{                       /* d26_gi14_update_scheme_object              */

D26_srt *srtp;          /* loop pointer to srt                        */
signed16 oc_nr;         /* index to object class of schema object     */
			/* filename for C-ISAM file containing schema */
char    filename[D27_LFILE_NAME];
int     isfd;           /* C-ISAM filedescriptor                      */
char    record[SRECL+1];/* memory for data record of schema object    */

signed16 ret_value = D2_NOERROR;    /* return value                   */

register D26_oct *octp; /* loop pointer to OCT                        */
register D26_ocl_arc *ocl_arcp;     /* pointer to object class arc    */
register signed16 i;    /* loop variable                              */

			/* Open file "objects0"                       */
sprintf(filename,"%s%s%d",dir_fname,D26_OBJECTS,0);
if                      /* objects0 cannot be read                    */
  ((isfd = D26_ISOPEN(svc_c_sev_fatal,filename,ISINOUT + ISEXCLLOCK)) < 0)
{                       /* log error and set return value             */
  ret_value = D2_ERROR;
}
else
{                       /* read schema object record                  */
  if                    /* schema record cannot be read               */
    (D26_ISREAD(svc_c_sev_fatal,isfd,record,ISCURR))
  {                     /* log error and set return value             */
    ret_value = D2_ERROR;
  }
  else
  {                     /* Search for the entry in OCT with filenr = 0*/
    for                 /* all OCT entries                            */
       (i = 0, octp = d26_oct; i < d26_nooct; i++, octp++)
    {                   /* check assigned file number                 */
      if                /* file contains schema                       */
	(octp->d26c_filenr == 0)
      {                 /* store object class in record               */
	oc_nr = i;
	stint(1,record + OCPOS);
	stlong((long)oc_nr,record + OCPOS + INTSIZE);
	break;
      }
    }
			/* Search for the entry in SRT with object    */
			/* class of schema                            */
    for                 /* all SRT entries                            */
       (i = 0, srtp = d26_srt; i < d26_nosrt; i++, srtp++)
    {                   /* scan object class arcs                     */
      for               /* all object class arcs                      */
	 (ocl_arcp = srtp->d26s_ocl_arc; ocl_arcp->d26s_oclass != -1;
	  ocl_arcp++)
      {                 /* compare with OCT entry of schema object    */
	if              /* object classes match                       */
	  (ocl_arcp->d26s_oclass == oc_nr)
	{               /* store the object number                    */
	  stint(i,record + LONGSIZE);
	  d26_soundex((byte *)(record + ISAM_HDR + INTSIZE + D26_LPHON +
	    INTSIZE),D26_CNL + 1,(byte *)(record + ISAM_HDR + INTSIZE));
	  break;
	}
      }
      if                /* schema object class was found              */
	(ocl_arcp->d26s_oclass != -1)
      {                 /* break loop                                 */
	break;
      }
    }
			/* Update the new index in objects0 file.     */
    if                  /* record cannot be rewritten                 */
     (D26_ISREWREC(svc_c_sev_fatal,isfd,isrecnum,record) < 0)
    {                   /* log error and set return value             */
      ret_value = D2_ERROR;
    }
  }

  if                    /* objects file cannot be closed              */
    (D26_ISCLOSE(svc_c_sev_fatal,isfd) < 0)
  {                     /* log error and set return value             */
    ret_value = D2_ERROR;
  }
}

return(ret_value);

}                       /* d26_gi14_update_scheme_object              */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_gi15_calc_positions                             */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : For a given object file number, work out the C-ISAM */
/*                record positions for all the naming attrs, the alias*/
/*                attr and all the other possible attrs which can be  */
/*                held in this file. Return the total length of the   */
/*                record. Return 0 if there are no naming attrs for   */
/*                this file.                                          */
/*                                                                    */
/*                                                                    */
/* INPUT-PARAMETER:   short      filenr                               */
/*                    D26_srt   *srt                                  */
/*                    D26_oct   *d26_oct                              */
/*                    D26_at    *d26_at                               */
/*                    Attr_info *attr_lens                            */
/*                                                                    */
/* OUTPUT-PARAMETER: -                                                */
/*                                                                    */
/* RETURNVALUE     : length of record                                 */
/*                                                                    */
/*                                                                    */
/**********************************************************************/

static int d26_gi15_calc_positions(
    signed16 filenr)

{                       /* d26_gi15_calc_positions                    */

D26_ocl_arc *ocl_arcp;  /* pointer to object class arcs               */

D26_srt  *srtp;
D26_oct  *octp;
signed16 *posp;
signed16 *auxp;
D26_at   *atp;

int      rec;           /* current record position in an object file  */

register signed16 i,j;   /* loop variables                            */

			/* initialize level positions array           */
for                     /* all levels in name                         */
   (i = 0, posp = level_pos; i < D2_NP_MAX; i++)
{                       /* scan AT                                    */
  for                   /* all AT entries                             */
     (j = 0; j < d26_noat; j++)
  {                     /* initialize the level position              */
    *posp++ = 0;
  }
}
			/* work out positions of naming and other     */
			/* attributes and store results in            */
			/* d26_name_pos and d26_at_pos                */

rec = ISAM_HDR;         /* start current record position at offset 6  */

			/* work out positions of RDNs in naming attr. */

for                     /* all SRT entries                            */
   (i = 0, srtp = d26_srt; i < d26_nosrt; i++, srtp++)
{                       /* check whether objects might be stored into */
			/* given file                                 */
  for                   /* all object class arcs                      */
     (ocl_arcp = srtp->d26s_ocl_arc; ocl_arcp->d26s_oclass != -1; ocl_arcp++)
  {                     /* check file number of object class          */
    if                  /* file numbers match                         */
      (d26_oct[ocl_arcp->d26s_oclass].d26c_filenr == filenr)
    {                   /* break loop                                 */
      break;
    }
  }
  if                    /* objects might be stored into given file    */
    (ocl_arcp->d26s_oclass != -1)
  {                     /* set positions for naming attributes        */
    rec = d26_gi21_calc_srt_positions(srtp,filenr,rec);
  }
}                       /* for each srt entry                         */

			/* Finished processing naming attributes, now */
			/* reserve space for the Alias attribute      */

for                     /* All AT entries                             */
   (i = 0, atp = d26_at; i < d26_noat; i++, atp++)
{                       /* check attribute type                       */
  if                    /* aliased object attribute found             */
    (D2_ATTCMP(&atp->d26a_type,D2_ALIASED_OBJECT) == 0)
  {                     /* set position in AT entry                   */
      atp->d26a_field_pos[filenr] = rec;
      rec += attr_lens[i].len;
      break;
  }
}

for                     /* all object classes                         */
   (i = 0, octp = d26_oct; i < d26_nooct; i++, octp++)
{                       /* check the file number                      */
  if                    /* file numbers match                         */
    (octp->d26c_filenr == filenr)
  {                     /* scan mandatory atributes                   */
    rec = d26_gi22_calc_oct_positions(octp,filenr,rec);

    for                 /* all auxiliary object classes               */
       (auxp = octp->d26c_aux_cl; *auxp != -1; auxp++)
    {                   /* calculate positions of its attributes      */
      rec = d26_gi22_calc_oct_positions(d26_oct + *auxp,filenr,rec);
    }
  }
}

return(rec);

}                       /* d26_gi15_calc_positions                    */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_gi16_make_object_file()                         */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : For a given file number, create file objects<filenr>*/
/*                Make <serial key> the primary index. Add secondary  */
/*                indices for all the naming attrs and for the alias  */
/*                and master knowledge attrs. For all other attrs, if */
/*                the 'index level' > 0 add another secondary index.  */
/*                                                                    */
/*                                                                    */
/* INPUT-PARAMETER:   short      filenr                               */
/*                    short      reclen                               */
/*                                                                    */
/* OUTPUT-PARAMETER: -                                                */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR   short       No error occurrred                    */
/*      D26_ERROR               error occurred                        */
/*                                                                    */
/**********************************************************************/

static signed16 d26_gi16_make_object_file(
    signed16 filenr,
    int      reclen)

{                       /* d26_gi16_make_object_file                  */

    char           fname[D27_LFILE_NAME];
    int            isfd;
    struct keydesc key;         /* structure defining a C-ISAM index  */
    signed16       pos;         /* holds a value from a position array*/
    signed16       at_idx;      /* index of an attr in the AT         */
    signed16       i,j;
    boolean        dont_care;
    D26_srt        *srtp;
    D26_nam_at     *natp;
    D26_at         *atp;

sprintf(fname, "%s%s%d",dir_fname,D26_OBJECTS,filenr);

key.k_flags  = ISNODUPS;
key.k_nparts = 1;
			/* serial key as primary index                */
key.k_part[0].kp_start = 0;
key.k_part[0].kp_leng  = LONGSIZE;
key.k_part[0].kp_type  = LONGTYPE;

if                      /* object file cannot be created or opened    */
  ((isfd = d26_gi31_create_isam(fname,reclen,&key,&dont_care)) < 0)
{
  return(D26_ERROR);
}
			/* add an index for all structure rules       */
for                     /* all SRT entries                            */
   (i = 0, srtp = d26_srt; i < d26_nosrt; i++, srtp++)
{                       /* check whether name   may be stored in file */
  if                    /* name may be stored in file                 */
    ((pos = srtp->d26s_nam_at[0].d26s_position[filenr]) > 0)
  {                     /* set AT index                               */
    at_idx = srtp->d26s_nam_at[0].d26s_index;
    if                  /* index cannot be created                    */
      (d26_gi32_make_key(isfd,pos,attr_lens + at_idx, d26_at + at_idx,
       d26_at[d26_alob_idx].d26a_field_pos[filenr],
       d26_at[d26_mk_idx].d26a_field_pos[filenr]) != D2_NO_ERR)
    {                   /* close file and return error                */
       D26_ISCLOSE(svc_c_sev_fatal,isfd);
       return(D26_ERROR);
    }
    for                 /* all additional naming attributes           */
       (natp = srtp->d26s_nam_at + 1; natp->d26s_index != -1; natp++)
    {                   /* set AT index                               */
      at_idx = natp->d26s_index;
      pos    = natp->d26s_position[filenr];
      if                /* index level is set                         */
	(d26_at[at_idx].d26a_ind_level > 0)
      {                 /* generate another index                     */
	if              /* index cannot be created                    */
	  (d26_gi32_make_key(isfd,pos,attr_lens + at_idx, d26_at + at_idx,0,
	   0) != D2_NO_ERR)
	{               /* close file and return error                */
	   D26_ISCLOSE(svc_c_sev_fatal,isfd);
	   return(D26_ERROR);
	}
      }
    }
  }
}
			/* add index for aliased object and master    */
			/* knowledge attribute                        */
if                      /* index cannot be created                    */
  (d26_gi32_make_key(isfd,0,(Attr_info *)NULL,(D26_at *)NULL,
   d26_at[d26_alob_idx].d26a_field_pos[filenr],
   d26_at[d26_mk_idx].d26a_field_pos[filenr]) != D2_NO_ERR)
{                       /* close file and return an error             */
  D26_ISCLOSE(svc_c_sev_fatal,isfd);
  return(D26_ERROR);
}
			/* add an index for all attributes with       */
			/* index_level set.                           */
for                     /* all AT entries                             */
   (i = 0, atp = d26_at; i < d26_noat; i++, atp++)
{                       /* check whether attribute occurs in file     */
  if                    /* attribute occurs in file and index level   */
			/* is set                                     */
    ((pos = atp->d26a_field_pos[filenr]) > 0 && atp->d26a_ind_level > 0)
  {                     /* check whether the attribute carries an     */
			/* index as naming attribute                  */
    for                 /* all level positions                        */
       (j = 0; j < D2_NP_MAX; j++)
    {                   /* compare field positions                    */
      if                /* field positions are equal                  */
	(level_pos[j * d26_noat + i] == pos)
      {                 /* break loop                                 */
	break;
      }
    }

    if                  /* attribute is not naming                    */
      (j == D2_NP_MAX)
    {                   /* make an index for it                       */
      if                /* index cannot be created                    */
	(d26_gi32_make_key(isfd,pos,attr_lens + i,atp,0,0) != D2_NO_ERR)
      {                 /* close file and return error                */
	D26_ISCLOSE(svc_c_sev_fatal,isfd);
	return(D26_ERROR);
      }
    }
  }                     /* if AT entry has position > 0 for this file */
}                       /* for all AT entries                         */

if                      /* file cannot be closed                      */
  (D26_ISCLOSE(svc_c_sev_fatal,isfd) < 0)
{                       /* log error                                  */
  return(D26_ERROR);
}

return(D2_NO_ERR);

}                       /* d26_gi16_make_object_file                  */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_gi17_make_attr_file                             */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : For a given file number, create file attr<filenr>.  */
/*                Make <obj nr><obj key><value> the primary index.    */
/*                If the attr has phonetic matching, add a secondary  */
/*                index on the phonetic part, <obj nr><obj key><phon>.*/
/*                                                                    */
/*                                                                    */
/* INPUT-PARAMETER:   short      filenr                               */
/*                                                                    */
/* OUTPUT-PARAMETER:  short     *reclen                               */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       short       no error occurred                 */
/*      D26_ERROR                   error occurred                    */
/*                                                                    */
/**********************************************************************/

static signed16 d26_gi17_make_attr_file(
    signed16 filenr,
    signed16 *reclen)

{                       /* d26_gi17_make_attr_file                    */
static struct keydesc key =
    {ISDUPS + COMPRESS,3,
     {{0,INTSIZE,INTTYPE},{INTSIZE,LONGSIZE,LONGTYPE}}};

static struct keydesc search_key =
    {ISDUPS + COMPRESS,3,
     {{0,INTSIZE,INTTYPE},{0,0,0},{INTSIZE,LONGSIZE,LONGTYPE}}};

char      fname[D27_LFILE_NAME];
int       isfd;
signed16  i;
boolean   dont_care;
D26_at    *atp;
int       rlen;

sprintf(fname, "%s%s%d",dir_fname,D26_ATTR,filenr);

for                     /* all AT entries                             */
   (i = 0, atp = d26_at; i < d26_noat; i++, atp++)
{                       /* check whether the attribute is stored in   */
			/* this file                                  */
  if                    /* file numbers match                         */
    (atp->d26a_filenr == filenr)
  {                     /* enhance key structure                      */
    *reclen = (signed16)(rlen = ISAM_HDR + attr_lens[i].len);

    key.k_part[2].kp_type = attr_lens[i].idx_type;
    key.k_part[2].kp_leng = attr_lens[i].idx_len;

    key.k_part[2].kp_start = ISAM_HDR;
    if                  /* attribute allows phonetical comparison     */
      (atp->d26a_phon == TRUE)
    {                   /* reset start for keypart                    */
      key.k_part[2].kp_start += D26_LPHON;
    }
    if                  /* attribute syntax is Case ignore or case    */
      (atp->d26a_syntax == D2_CEXACT || atp->d26a_syntax == D2_CIGNORE)
    {                   /* reset start for keypart                    */
	key.k_part[2].kp_start += INTSIZE;
    }
    break;
  }
}

if                      /* C-ISAM file cannot be created              */
  ((isfd = d26_gi31_create_isam(fname,rlen,&key,&dont_care)) < 0)
{                       /* return error                               */
  return(D26_ERROR);
}

if                      /* user wants key for searches                */
  (atp->d26a_ind_level > 0)
{                       /* assign second keypart for search key       */
  search_key.k_part[1] = key.k_part[2];
  if                    /* index cannot be generated                  */
    (D26_ISADDINDEX(svc_c_route_nolog,isfd,&search_key) < 0)
  {                     /* check the error                            */
    if                  /* C-ISAM index did not exist before          */
      (iserrno != EKEXISTS)
    {
      dce_svc_printf(GDS_S_ISADDINDEX_ERR_MSG,iserrno);
      D26_ISCLOSE(svc_c_sev_fatal,isfd);
      return(D26_ERROR);
    }
  }
}
			/* add secondary indices for ordering matching*/
if                      /* descending index is needed                 */
  (atp->d26a_syntax == D2_OCTET || atp->d26a_syntax == D2_TIME ||
  atp->d26a_syntax == D2_INTEGER)
{                       /* reset index type                           */
  key.k_part[2].kp_type += ISDESC;

  if                    /* index cannot be generated                  */
    (D26_ISADDINDEX(svc_c_route_nolog,isfd,&key) < 0)
  {                     /* check the error                            */
    if                  /* C-ISAM index did not exist before          */
      (iserrno != EKEXISTS)
    {
      dce_svc_printf(GDS_S_ISADDINDEX_ERR_MSG,iserrno);
      D26_ISCLOSE(svc_c_sev_fatal,isfd);
      return(D26_ERROR);
    }
  }
  if                    /* user wants key for searches                */
    (atp->d26a_ind_level > 0)
  {                     /* assign second keypart for search key       */
    search_key.k_part[1] = key.k_part[2];
    if                  /* index cannot be generated                  */
      (D26_ISADDINDEX(svc_c_route_nolog,isfd,&search_key) < 0)
    {                   /* check the error                            */
      if                  /* C-ISAM index did not exist before        */
	(iserrno != EKEXISTS)
      {
	dce_svc_printf(GDS_S_ISADDINDEX_ERR_MSG,iserrno);
	D26_ISCLOSE(svc_c_sev_fatal,isfd);
	return(D26_ERROR);
      }
    }
  }
}

			/* add a secondary index for phonetic attrs   */
if                      /* attribute admits phonetical comparison     */
  (atp->d26a_phon == TRUE)
{                       /* reset keypart 2                            */
  key.k_part[2].kp_type  = CHARTYPE;
  key.k_part[2].kp_start = ISAM_HDR;
  key.k_part[2].kp_leng  = D26_LPHON;

  if                    /* index cannot be generated                  */
    (D26_ISADDINDEX(svc_c_route_nolog,isfd,&key) < 0)
  {                     /* check the error                            */
    if                  /* index did not exist before                 */
      (iserrno != EKEXISTS)
    {
      dce_svc_printf(GDS_S_ISADDINDEX_ERR_MSG,iserrno);
      D26_ISCLOSE(svc_c_sev_fatal,isfd);
      return(D26_ERROR);
    }
  }
  if                    /* user wants key for searches                */
    (atp->d26a_ind_level > 0)
  {                     /* assign second keypart for search key       */
    search_key.k_part[1] = key.k_part[2];
    if                  /* index cannot be generated                  */
      (D26_ISADDINDEX(svc_c_route_nolog,isfd,&search_key) < 0)
    {                   /* check the error                            */
      if                  /* C-ISAM index did not exist before        */
	(iserrno != EKEXISTS)
      {
	dce_svc_printf(GDS_S_ISADDINDEX_ERR_MSG,iserrno);
	D26_ISCLOSE(svc_c_sev_fatal,isfd);
	return(D26_ERROR);
      }
    }
  }
}

if                      /* file cannot be closed                      */
  (D26_ISCLOSE(svc_c_sev_fatal,isfd))
{                       /* log error                                  */
  return(D26_ERROR);
}

return(D2_NO_ERR);

}                       /* d26_gi17_make_attr_file                    */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_gi18_make_dnlist_file                           */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : Re-create C-ISAM file 'dnlist' with the first 120   */
/*                bytes as the primary index. Add a secondary index on*/
/*                the <occurency> field. Write out a time-stamp as the*/
/*                first record.                                       */
/*                                                                    */
/*                                                                    */
/* INPUT-PARAMETER:   -                                               */
/*                                                                    */
/*                                                                    */
/* OUTPUT-PARAMETER:  -                                               */
/*                                                                    */
/* RETURNVALUE     : ptr to global error code string                  */
/*                   D2_NULL                                          */
/*                                                                    */
/**********************************************************************/

static signed16 d26_gi18_make_dnlist_file(void)

{                       /* d26_gi18_make_dnlist_file                  */

    char           fname[D27_LFILE_NAME];
    int            isfd;
			/* boolean which is TRUE if file was created, */
			/* FALSE if file was opened                   */
    boolean        created;
    struct keydesc key;
			/* <Irprt-flg><normed-part><DN><occurency> + 1*/
    char           record[D26_I02_RECDNLISTL];

sprintf(fname,"%s%s",dir_fname,D26_DNLIST);

key.k_flags  = ISDUPS + COMPRESS;
key.k_nparts = 1;
			/* <first 120 bytes> is primary index         */
key.k_part[0].kp_start = 0;
key.k_part[0].kp_leng  = MAXKEYSIZE;
key.k_part[0].kp_type  = CHARTYPE;

if                      /* C-ISAM file cannot be generated            */
  ((isfd = d26_gi31_create_isam(fname,D26_I02_RECDNLISTL,&key,&created)) < 0)
{                       /* return error                               */
  return(D26_ERROR);
}

			/* secondary index on <ocurrency>             */
key.k_part[0].kp_start = D26_I12_OCCURRENCY;
key.k_part[0].kp_leng  = LONGSIZE;
key.k_part[0].kp_type  = LONGTYPE;

if                      /* index cannot be created                    */
  (D26_ISADDINDEX(svc_c_route_nolog,isfd,&key) < 0)
{                       /* check the error                            */
  if                    /* index did not exist before                 */
    (iserrno != EKEXISTS)
  {                     /* log the error                              */
    dce_svc_printf(GDS_S_ISADDINDEX_ERR_MSG,iserrno);
    D26_ISCLOSE(svc_c_sev_fatal,isfd);
    return(D26_ERROR);
  }
}

if                      /* If dnlist was created for the first time   */
  (created == TRUE)
{                       /* then put a 0-version number into the first */
			/* record                                     */
  (void)memset(record,0,D26_I02_RECDNLISTL);

  record[0] ='0';       /* <modification type>                        */
  record[1] ='\001';    /* <CTRL A>                                   */
			/* version number                             */
  stlong(0L,record + D26_I09_VERSION);
			/* <occurency>                                */
  stlong(-1, record + D26_I12_OCCURRENCY);

  if                    /* version number record cannot be written    */
    (D26_ISWRITE(svc_c_sev_fatal,isfd,record) < 0)
  {                     /* log the error                              */
    D26_ISCLOSE(svc_c_sev_fatal,isfd);
    return(D26_ERROR);
  }
}

if                      /* file cannot be closed                      */
  (D26_ISCLOSE(svc_c_sev_fatal,isfd) < 0)
{                       /* log the error                              */
  return(D26_ERROR);
}

return(D2_NO_ERR);

}                       /* d26_gi18_make_dnlist_file                  */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_gi19_write_scheme                               */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : Create an normal binary file called "scheme". Write */
/*                out SRT, AT, OCT arrays and all other information   */
/*                listed in the design document. This file is read by */
/*                the DSA at boot time.                               */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*                    char        *sch_time                           */
/*                                                                    */
/*                                                                    */
/* OUTPUT-PARAMETER:  -                                               */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       short           no error occurred             */
/*      D26_ERROR                       error occurred                */
/*                                                                    */
/**********************************************************************/

static signed16 d26_gi19_write_scheme(
    char *sch_time)

{                       /* d26_gi19_write_scheme                      */

  char           fname[D27_LFILE_NAME];
  FILE          *fp;
  D26_nam_at    *nam_atp;
  D26_srt       *srtp;
  D26_oct       *octp;
  D26_at        *atp;
  signed16       i, j;
  signed16       len_ob_id_string;
  signed16       *idx;
  D2_obj_id     *soundex_obid;


			/* Calculate length of octal object id string */
			/* for AT and OCT obj ids.                    */

  for                   /* all AT entries                             */
     (i = 0, atp = d26_at, len_ob_id_string = 0; i < d26_noat; i++, atp++)
  {                     /* increment length of OB_ID string           */
    len_ob_id_string += atp->d26a_type.d2_typ_len;
  }

  for                   /* all OCT entries                            */
     (i=0, octp = d26_oct; i < d26_nooct; i++, octp++)
  {                     /* increment length of OB_ID string           */
    len_ob_id_string += octp->d26c_ob_id.d2_typ_len;
  }

  soundex_obid = d26_soundex(NULL,0,NULL);
  len_ob_id_string += soundex_obid->d2_typ_len;

  sprintf(fname,"%s%s",dir_fname,D26_SCHEME);

  if                    /* schema file cannot be created              */
    ((fp = D27_FOPEN(svc_c_sev_fatal,fname, "w")) == NULL)
  {                     /* log the error and return                   */
    return(D26_ERROR);
  }

  if                    /* initializing data cannot be written        */
    (d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,
     "nr_srt: %d nr_oct: %d nr_at: %d nr_ofiles: %d nr_afiles: %d o_reclen: %d a_reclen: %d len_ob_id_string: %d nr_oct_indices: %d nr_cl_indices: %d nr_na_indices: %d sch_time: %s\n",
     d26_nosrt,d26_nooct,d26_noat,d26_nofile,d26_nafile,d26_oreclen,
     d26_areclen,len_ob_id_string,nr_oct_indices,nr_cl_indices,nr_na_indices,
     sch_time) < 0)
  {                     /* log the error and return                   */
    D27_FCLOSE(svc_c_sev_fatal,fp);
    return(D26_ERROR);
  }

			/* Write a separator to separate from HEX data*/
  if                    /* separator cannot be written                */
    (D27_FWRITE(svc_c_sev_fatal,"@",1,1,fp) < (size_t)1)
  {                     /* log the error                              */
    return(D26_ERROR);
  }

			/* Write country-bitlist                      */

  if                    /* country-bitlist cannot be written          */
    (D27_FWRITE(svc_c_sev_fatal,(void *)d27_country_bitlist,
     D27_CNTRY_BITLIST_LEN,1,fp) < (size_t)1)
  {                     /* log error and return                       */
    return(D26_ERROR);
  }

			/* Write arrays in binary form: SRT, OCT, AT  */
			/* ocl_arcs, d26_ofildes, d26_afildes         */

  if                    /* SRT cannot be written                      */
    (D27_FWRITE(svc_c_sev_fatal,(void *)d26_srt,sizeof(D26_srt),d26_nosrt,fp)
     < (size_t)d26_nosrt)
  {                     /* log the error                              */
    return(D26_ERROR);
  }

  if                    /* OCT cannot be written                      */
    (D27_FWRITE(svc_c_sev_fatal,(void *)d26_oct,sizeof(D26_oct),d26_nooct,fp)
     < (size_t)d26_nooct)
  {                     /* log the error                              */
    return(D26_ERROR);
  }

  if                    /* AT cannot be written                       */
    (D27_FWRITE(svc_c_sev_fatal,(void *)d26_at,sizeof(D26_at),d26_noat,fp)
     < (size_t)d26_noat)
  {                     /* log the error                              */
    return(D26_ERROR);
  }

  if                    /* OCL ARCS cannot be written                 */
    (D27_FWRITE(svc_c_sev_fatal,(void *)d26_ocl_arc,sizeof(D26_ocl_arc),
     nr_cl_indices,fp) < (size_t)nr_cl_indices)
  {                     /* log the error                              */
    return(D26_ERROR);
  }

  if                    /* OFILDES cannot be written                  */
    (D27_FWRITE(svc_c_sev_fatal,(void *)d26_ofildes,sizeof(D26_fildes),
     d26_nofile,fp) < (size_t)d26_nofile)
  {                     /* log the error                              */
    return(D26_ERROR);
  }

  if                    /* AFILDES cannot be written                  */
    (D27_FWRITE(svc_c_sev_fatal,(void *)d26_afildes,sizeof(D26_fildes),
     d26_nafile,fp) < (size_t)d26_nafile)
  {                     /* log the error                              */
    return(D26_ERROR);
  }

  if                    /* soundex identifier cannot be written       */
    (D27_FWRITE(svc_c_sev_fatal,(void *)soundex_obid,sizeof(D2_obj_id),1,fp)
     < (size_t)1)
  {                     /* log the error                              */
    return(D26_ERROR);
  }
			/* Write octal obj id strings from AT + OCT   */

  for                   /* all AT entries                             */
     (i = 0, atp = d26_at; i < d26_noat; i++, atp++)
  {                     /* check the length of the object identifier  */
    if                  /* length is not 0                            */
      ((j = atp->d26a_type.d2_typ_len) != 0)
    {                   /* write the object identifier                */
      if                /* error from writing                         */
	(D27_FWRITE(svc_c_sev_fatal,(void *)(atp->d26a_type.d2_type),1,j,fp)
	 < (size_t)j)
      {                 /* log the error                              */
	return(D26_ERROR);
      }
    }
  }

  for                   /* all OCT entries                            */
     (i = 0, octp = d26_oct; i < d26_nooct; i++, octp++)
  {                     /* check the length of the object identifier  */
    if                  /* length is not 0                            */
      ((j = octp->d26c_ob_id.d2_typ_len) != 0)
    {                   /* write the object identifier                */
      if                /* error from writing                         */
	(D27_FWRITE(svc_c_sev_fatal,(void *)(octp->d26c_ob_id.d2_type),1,j,
	 fp) < (size_t)j)
      {
	return(D26_ERROR);
      }
    }
  }

  if                    /* error from writing soundex identifier      */
    (D27_FWRITE(svc_c_sev_fatal,(void *)(soundex_obid->d2_type),1,
     soundex_obid->d2_typ_len,fp) < (size_t)soundex_obid->d2_typ_len)
  {
    return(D26_ERROR);
  }

  for                   /* all SRT entries                            */
     (i = 0, srtp = d26_srt; i < d26_nosrt; i++, srtp++)
  {                     /* write naming attribute indices             */
    for                 /* all naming attributes                      */
       (nam_atp = srtp->d26s_nam_at; ; nam_atp++)
    {                   /* write AT-index into file                   */
      d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%hd ",
	nam_atp->d26s_index);
      if                /* end of list is reached                     */
	(nam_atp->d26s_index == -1)
      {                 /* break loop                                 */
	  break;
      }
    }
  }
			/* Write OCT-indices including -1 terminators.*/

  for                   /* all OCT entries                            */
     (i=0, octp = d26_oct; i < d26_nooct; i++, octp++)
  {                     /* write superclass indices                   */
    for                 /* all direkt superclasses                    */
       (idx = octp->d26c_sup_cl; ; idx++)
    {                   /* write index into file                      */
      d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp, "%d ",*idx);
      if                /* End of list reached                        */
	(*idx == -1)
      {                 /* break loop                                 */
	  break;
      }
    }

    for                 /* all auxiliary classes                      */
       (idx = octp->d26c_aux_cl; ; idx++)
    {                   /* write index into file                      */
      d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp, "%d ",*idx);
      if                /* End of list reached                        */
	(*idx == -1)
      {                 /* break loop                                 */
	  break;
      }
    }

    for                 /* all mandatory object classes               */
       (idx = octp->d26c_mandatory; ; idx++)
    {
      d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp, "%d ", *idx);
      if                /* end of list reached                        */
	(*idx == -1)
      {
	break;
      }
    }

    for                 /* all optional attributes                    */
       (idx = octp->d26c_optional; ; idx++)
    {
      d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp, "%d ", *idx);
      if                /* end of list reached                        */
	(*idx == -1)
      {
	break;
      }
    }
  }
			/* Write SRT and AT position arrays           */

  for                   /* all SRT entries                            */
     (i = 0, srtp = d26_srt; i < d26_nosrt; i++, srtp++)
  {
    for                 /* all naming attributes                      */
       (nam_atp = srtp->d26s_nam_at; nam_atp->d26s_index != -1; nam_atp++)
    {                   /* scan the object files                      */
      for               /* all object files                           */
	 (j = 0; j < d26_nofile; j++)
      {                 /* write the field positions                  */
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%d ",
	  nam_atp->d26s_position[j]);
      }
    }
  }

  for                   /* all AT entries                             */
     (i = 0, atp = d26_at; i < d26_noat; i++, atp++)
  {                     /* scan the object files                      */
    for                 /* all object files                           */
       (j = 0; j < d26_nofile; j++)
    {                   /* write the field positions                  */
      d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%d ",
	atp->d26a_field_pos[j]);
    }
  }

  if                    /* schema file cannot be closed               */
    (D27_FCLOSE(svc_c_sev_fatal,fp) < 0)
  {                     /* log the error                              */
    return(D26_ERROR);
  }

  return(D2_NO_ERR);

}                       /* d26_gi19_write_scheme                      */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_gi21_calc_srt_positions                         */
/*                                                                    */
/* AUTHOR       : Ernst Kraemer, AP 11                                */
/* DATE         : 11.03.92                                            */
/*                                                                    */
/* DESCRIPTION  : For a given object file number and SRT entry, set   */
/*                the record positions for all the naming attributes  */
/*                of the SRT entry and of the superior SRT entries.   */
/*                Return the total length of the actual record length.*/
/*                                                                    */
/* INPUT-PARAMETER:   short      filenr                               */
/*                    D26_srt   *srtp                                 */
/*      rec         short           actual record position            */
/*                                                                    */
/* OUTPUT-PARAMETER: -                                                */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      > 0         short           actual record position            */
/*                                                                    */
/**********************************************************************/

static int d26_gi21_calc_srt_positions(
    D26_srt  *srtp,
    signed16 filenr,
    int      rec)

{                       /* d26_gi21_calc_srt_positions                */

D26_nam_at *nam_atp;    /* pointer to naming attributes               */
D26_srt    *srtp_loop;  /* loop pointer to SRT                        */

signed16   level;       /* level of SRT entry below Root              */

for                     /* all superior structure rules               */
   (level = 0, srtp_loop = srtp; srtp_loop->d26s_par_nr != D26_ROOT;
    level++, srtp_loop = d26_srt + srtp_loop->d26s_par_nr);

			/* scan the naming attributes                 */
for                     /* all naming attributes                      */
   (nam_atp = srtp->d26s_nam_at; nam_atp->d26s_index != -1; nam_atp++)
{                       /* check whether position is already calculated*/
  if                    /* position not yet calculated                */
    (nam_atp->d26s_position[filenr] == 0)
  {                     /* check level position                       */
    if                  /* position on this level ready               */
      (level_pos[level * d26_noat + nam_atp->d26s_index] != 0)
    {                   /* record it into SRT entry                   */
      nam_atp->d26s_position[filenr] =
	       level_pos[level * d26_noat + nam_atp->d26s_index];
    }
    else
    {                   /* record current position in d26_name_pos and*/
			/* level positions                            */
      nam_atp->d26s_position[filenr] =
	level_pos[level * d26_noat + nam_atp->d26s_index] = (signed16)rec;
			/* increment current record position by length*/
			/* of naming attribute value                  */
      rec += attr_lens[nam_atp->d26s_index].len;
    }
  }
}

if                      /* SRT root not yet reached                   */
  (srtp->d26s_par_nr != D26_ROOT)
{                       /* call function recursively for superior SRT */
			/* entry                                      */
  rec = d26_gi21_calc_srt_positions(d26_srt + srtp->d26s_par_nr,filenr,rec);
}

			/* return actual record position              */
return(rec);

}                       /* d26_gi21_calc_srt_positions                */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_gi22_calc_oct_positions                         */
/*                                                                    */
/* AUTHOR       : Ernst Kraemer, AP 11                                */
/* DATE         : 11.03.92                                            */
/*                                                                    */
/* DESCRIPTION  : For a given object class the positions of all the   */
/*                mandatory and optional attributes in the object     */
/*                class are set.                                      */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      octp        D26_oct *       pointer to OCT entry              */
/*      filenr      short           file number                       */
/*      rec         short           actual record position            */
/*                                                                    */
/* OUTPUT-PARAMETER: -                                                */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      > 0         short           actual record position            */
/*                                                                    */
/**********************************************************************/

static int d26_gi22_calc_oct_positions(
    D26_oct  *octp,
    signed16 filenr,
    int      rec)

{                       /* d26_gi22_calc_oct_positions                */

register short    j, at_idx;    /* loop variables                     */

for                     /* all mandatory attributes                   */
   (j = 0, at_idx = octp->d26c_mandatory[j]; at_idx != -1;
    j++, at_idx = octp->d26c_mandatory[j])
{                       /* calculate AT record positions              */
    rec = d26_gi23_calc_at_positions(filenr,at_idx,rec);
}

for                     /* all optional attributes                    */
   (j = 0, at_idx = octp->d26c_optional[j]; at_idx != -1;
    j++, at_idx = octp->d26c_optional[j])
{                       /* calculate AT record positions              */
    rec = d26_gi23_calc_at_positions(filenr,at_idx,rec);
}

			/* return actual record position              */
return(rec);

}                       /* d26_gi22_calc_oct_positions                */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_gi23_calc_at_positions                           */
/*                                                                    */
/* AUTHOR       : Ernst Kraemer, AP 11                                */
/* DATE         : 11.03.92                                            */
/*                                                                    */
/* DESCRIPTION  : For a given object file number the position in the  */
/*                AT entry is set to the highest level position of    */
/*                this attribute if present, to the actual record     */
/*                position otherwise.                                 */
/*                                                                    */
/* INPUT-PARAMETER:   short      filenr                               */
/*      at_idx      short           index of attribute in AT          */
/*      rec         short           actual record position            */
/*                                                                    */
/* OUTPUT-PARAMETER: -                                                */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      > 0         short           actual record position            */
/*                                                                    */
/**********************************************************************/

static int d26_gi23_calc_at_positions(
    signed16 filenr,
    signed16 at_idx,
    int      rec)

{                       /* d26_gi23_calc_at_positions                 */

signed16 pos;                   /* position in record                 */

signed16 level;                 /* lowest attribute level             */

if                      /* field position in AT not yet defined       */
  (d26_at[at_idx].d26a_field_pos[filenr] == 0)
{                       /* scan the level positions                   */
  level = d26_gi24_lowest_at_level(filenr,D26_ROOT,-1,at_idx);
  pos = level_pos[level * d26_noat + at_idx];

  if                    /* field position defined already             */
    (pos != 0)
  {                     /* store it into AT entry                     */
    d26_at[at_idx].d26a_field_pos[filenr] = pos;
  }
  else
  {                     /* store actual record position               */
    d26_at[at_idx].d26a_field_pos[filenr] = (signed16)rec;

			/* values held in attr files only have <count>*/
			/* (2 bytes) in the object file.              */
    rec += d26_at[at_idx].d26a_filenr >= 0 ? INTSIZE :
				       attr_lens[at_idx].len;
  }
}

			/* return actual record position              */
return(rec);

}                       /* d26_gi23_calc_at_positions                 */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_gi24_lowest_at_level                            */
/*                                                                    */
/* AUTHOR       : Ernst Kraemer, AP 11                                */
/* DATE         : 10.04.92                                            */
/*                                                                    */
/* DESCRIPTION  : For a given object file number and attribute the    */
/*                lowest level in the name tree is calculated for     */
/*                objects that may be stored into the file and have   */
/*                the specified attribute.                            */
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*      filenr      short           number of file to contain objects */
/*      par_idx     short           index of parent structure rule to */
/*                                  be processed                      */
/*      level       short           level of parent structure rule    */
/*      at_idx      short           index of attribute in AT          */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*      None                                                          */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      >= 0        short           lowest level of objects in name   */
/*                                  tree                              */
/*                                                                    */
/**********************************************************************/

static signed16 d26_gi24_lowest_at_level(
    signed16 filenr,
    signed16 par_idx,
    signed16 level,
    signed16 at_idx)

{                       /* d26_gi24_lowest_at_level                   */

signed16    ret_level;      /* level to return                        */
signed16    branch_level;   /* AT level of one branch                 */
			    /* array of auxiliary object classes      */
signed32    ob_cl[D26_OCL_VAL_COUNT + 1];

register    signed16 i ,j;  /* loop variables                         */
register    D26_srt *srt;   /* loop pointer to SRT                    */
register    D26_ocl_arc *ocl_arc;   /* loop pointer to object class   */
			    /* arcs                                   */
register    signed16 *aux_cl;/* loop pointer to auxiliary object class */

for                     /* all SRT entries                            */
   (i = 0, srt = d26_srt, ret_level = -1; i < d26_nosrt; i++, srt++)
{                       /* check whether child of parent              */
  if                    /* SRT entry child of parent                  */
    (srt->d26s_par_nr == par_idx)
  {                     /* scan the object class arcs                 */
    if                  /*  level for this branch is greater          */
      ((branch_level = d26_gi24_lowest_at_level(filenr,i,level + 1,at_idx))
       > ret_level)
    {                   /*  reset the return level                    */
      ret_level = branch_level;
    }
    for                 /* all object class arcs                      */
       (ocl_arc = srt->d26s_ocl_arc;
	ocl_arc->d26s_oclass != -1 && ret_level < level + 1; ocl_arc++)
    {                   /* check the file number                      */
      if                /* file numbers match                         */
	(d26_oct[ocl_arc->d26s_oclass].d26c_filenr == filenr)
      {                 /* build the array of auxiliary object classes*/
	ob_cl[0] = (signed32)ocl_arc->d26s_oclass;
	for             /* all auxiliary object classes               */
	   (j = 1, aux_cl = d26_oct[ocl_arc->d26s_oclass].d26c_aux_cl;
	    j < D26_OCL_VAL_COUNT && *aux_cl != -1; j++, aux_cl++)
	{               /* put the index into the array               */
	  ob_cl[j] = (signed32)*aux_cl;
	}
	ob_cl[j] = D26_EOL;
			/* check whether object class contains the    */
			/* attribute                                  */
	if              /* object class contains the attribute        */
	  (d26_u59_attr_in_obj_class(ob_cl,at_idx) == TRUE)
	{               /* reset the return level                     */
	  ret_level = level + 1;
	}
      }
    }
  }
}

			/* return the return level                    */
return(ret_level);

}                       /* d26_gi24_lowest_at_level                   */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_gi31_create_isam                                */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : Create a named C-ISAM file, if it already exists    */
/*                then open it. Output parameter "created" is true if */
/*                file was created, it is false if it was opened.     */
/*                                                                    */
/*                                                                    */
/* INPUT-PARAMETER:   char           *fname                           */
/*                    short           reclen                          */
/*                    struct keydesc *key                             */
/*                                                                    */
/* OUTPUT-PARAMETER:  short          *created                         */
/*                                                                    */
/* RETURNVALUE     : ISAM file descriptor                             */
/*                   -1                                               */
/*                                                                    */
/**********************************************************************/

static int d26_gi31_create_isam(
    char     *fname,
    int      reclen,
    struct keydesc *key,
    boolean *created)

{                       /* d26_gi31_create_isam                       */

    int         isfd;

*created = TRUE;

if                      /* C-ISAM file cannot be created              */
  ((isfd = D26_ISBUILD(svc_c_route_nolog,fname,reclen,key,
   ISINOUT + ISEXCLLOCK)) < 0)
{                       /* check whether it existed before            */
  if                    /* file existed before                        */
    (iserrno == EEXIST)
  {
    if                  /* file cannot be opened                      */
      ((isfd = D26_ISOPEN(svc_c_sev_fatal,fname,ISINOUT + ISEXCLLOCK)) >= 0)
    {                   /* reset created flag                         */
      *created = FALSE;
    }
  }
  else
  {                     /* log error                                  */
    dce_svc_printf(DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
      svc_c_sev_fatal,GDS_S_ISBUILD_ERR,fname,reclen,ISINOUT + ISEXCLLOCK,
      iserrno);
  }
}

return(isfd);           /* isfd is negative if isbuild or isopen fails*/

}                       /* d26_gi31_create_isam                       */

/********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_gi32_make_key                                   */
/*                                                                    */
/* AUTHOR       : Melissa, D AP 11                                    */
/* DATE         : 20.10.90                                            */
/*                                                                    */
/* DESCRIPTION  : For a given attr, add one or more indices to an     */
/*                object file. If the attr value is held in an attr   */
/*                file don't create any indices. Otherwise create idx */
/*                at the given position, <obj nr><value>[<alob><mk>]  */
/*                <serial key>. alob and mk are part of the index, if */
/*                mk_pos is                                           */
/*                > 0. alob is part of another index, if              */
/*                If the attr allows phonetic matching, add another   */
/*                index, <obj nr><phon><serial key>. If the attr is   */
/*                recurring and index level > 0 then add more indices */
/*                for each recurring value.                           */
/*                                                                    */
/*                                                                    */
/* INPUT-PARAMETER:   int      isfd                                */
/*                    short       start_pos                           */
/*                    Attr_info  *attr_len                            */
/*                    D26_at     *atp                                 */
/*                    short       mk_pos                              */
/*                                                                    */
/* OUTPUT-PARAMETER: -                                                */
/*                                                                    */
/* RETURNVALUE     : D2_NO_ERR                                        */
/*                   D26_ERROR                                        */
/*                                                                    */
/**********************************************************************/

static signed16 d26_gi32_make_key(
    int       isfd,
    signed16  start_pos,
    Attr_info *attr_len,
    D26_at *atp,
    signed16 alob_pos,
    signed16 mk_pos)

{                       /* d26_gi32_make_key                          */

			/* ordinary key on attribute                  */
static struct keydesc key =
	    {ISDUPS + COMPRESS,3,{{LONGSIZE,INTSIZE,INTTYPE},{0,0,0},
				 {0,LONGSIZE,LONGTYPE}}};

			/* key including aliased object and           */
			/* master-knowledge attribute                 */
static struct keydesc key_al_mk =
	    {ISDUPS + COMPRESS,5,
	     {
	      {LONGSIZE,INTSIZE,INTTYPE},   /* object number part     */
	      {0,0,0},                      /* attribute value part   */
	      {0,INTSIZE,INTTYPE},          /* aliased object part    */
	      {0,LONGSIZE,LONGTYPE},        /* master knowledge part  */
	      {0,LONGSIZE,LONGTYPE}         /* record number part     */
	     }
	    };

			/* key including only aliased object and      */
			/* master-knowledge attribute                 */
static struct keydesc key_al_mk_only =
	    {ISDUPS + COMPRESS,4,
	     {
	      {LONGSIZE,INTSIZE,INTTYPE},   /* object number part     */
	      {0,INTSIZE,INTTYPE},          /* aliased object part    */
	      {0,LONGSIZE,LONGTYPE},        /* master knowledge part  */
	      {0,LONGSIZE,LONGTYPE}         /* record number part     */
	     }
	    };

  signed16 rest_len;    /* remaining length of attribute value        */
  signed16 one_val_len; /* attr_len includes space for recurring valus*/
			/* one_val_len is the length of one value.    */
  signed16 reduce_len;  /* length to reduce an index part             */
  signed16 i;

if                      /* attribute given for index                  */
  (atp != NULL)
{
			/* Don't make index if attr held in attr file */
  if                    /* attribute is in attribute file             */
    (atp->d26a_filenr >= 0)
  {                     /* return immediately                         */
    return(D2_NO_ERR);
  }

			/* skip over <count> (2 bytes)                */
  key.k_part[1].kp_start = start_pos + INTSIZE;

			/* for each recuring value: create an index + */
			/* if phonetic matching set create a 2nd index*/

  for                   /* all attribute values                       */
     (i = 0, rest_len = one_val_len =
			(attr_len->len - INTSIZE) / atp->d26a_nr_recur;
      i < atp->d26a_nr_recur; i++, rest_len = one_val_len)
  {                     /* check whether phonetic match is allowed    */
    if                  /* phonetic match is allowed                  */
      (atp->d26a_phon == TRUE)
    {                   /* set keypart 1                              */
      key.k_part[1].kp_leng = D26_LPHON;
      key.k_part[1].kp_type = CHARTYPE;

			/* Add index for phonetic bytes               */
      if                /* index cannot be added                      */
	(D26_ISADDINDEX(svc_c_route_nolog,isfd,&key) < 0)
      {                 /* check reason                               */
	if              /* index did not exist before                 */
	  (iserrno != EKEXISTS)
	{               /* log error                                  */
	  dce_svc_printf(GDS_S_ISADDINDEX_ERR_MSG,iserrno);
	  return(D26_ERROR);
	}
      }

      key.k_part[1].kp_start += D26_LPHON;
      rest_len -= D26_LPHON;
    }

    if                  /* syntax is Case Ignore or Case exact string */
      (atp->d26a_rep == D2_T61_PR)
    {                   /* reset start of second index part           */
      key.k_part[1].kp_start += INTSIZE;
      rest_len -= INTSIZE;
    }

    if                  /* master-knowledge is part of index          */
      (alob_pos > 0 && mk_pos > 0 && i == 0)
    {                   /* set keypart 1 and 2                        */
      key_al_mk.k_part[1].kp_start = key.k_part[1].kp_start;
      key_al_mk.k_part[1].kp_type  = attr_len->idx_type;
      reduce_len = key_al_mk.k_part[2].kp_leng + key_al_mk.k_part[3].kp_leng;
      key_al_mk.k_part[1].kp_leng =
	   (attr_len->idx_len + reduce_len < MAX_PART_SIZE ?
	    attr_len->idx_len : MAX_PART_SIZE - reduce_len);
      key_al_mk.k_part[2].kp_start = alob_pos;
      key_al_mk.k_part[3].kp_start = mk_pos + INTSIZE;
			/* Add index for one attr value               */
      if                /* index cannot be added                      */
	(D26_ISADDINDEX(svc_c_route_nolog,isfd,&key_al_mk) < 0)
      {                 /* check error                                */
	if              /* index did not exist before                 */
	  (iserrno != EKEXISTS)
	{               /* log error                                  */
	  dce_svc_printf(GDS_S_ISADDINDEX_ERR_MSG,iserrno);
	  return(D26_ERROR);
	}
      }
    }
    else
    {
      key.k_part[1].kp_type = attr_len->idx_type;
      key.k_part[1].kp_leng = attr_len->idx_len;
			/* Add index for one attr value               */
      if                  /* index cannot be added                      */
	(D26_ISADDINDEX(svc_c_route_nolog,isfd,&key) < 0)
      {                 /* check the error                            */
	if              /* index did not exist before                 */
	  (iserrno != EKEXISTS)
	{               /* log the error                              */
	  dce_svc_printf(GDS_S_ISADDINDEX_ERR_MSG,iserrno);
	  return(D26_ERROR);
	}
      }
			/* add a secondary index for ordering matching*/
      if (atp->d26a_syntax == D2_OCTET || atp->d26a_syntax == D2_TIME ||
	  atp->d26a_syntax == D2_INTEGER)
      {                 /* reset keypart type                         */
	key.k_part[1].kp_type += ISDESC;

	if              /* error from adding index                    */
	  (D26_ISADDINDEX(svc_c_route_nolog,isfd, &key) < 0)
	{               /* check the error                            */
	  if            /* index did not exist before                 */
	    (iserrno != EKEXISTS)
	  {             /* log the error                              */
	    dce_svc_printf(GDS_S_ISADDINDEX_ERR_MSG,iserrno);
	    return(D26_ERROR);
	  }
	}
      }
    }

    if                  /* index not on user request                  */
      (atp->d26a_ind_level == 0)
    {                   /* only make index for first value            */
      break;
    }
			/* set start position to next recurring value */
    key.k_part[1].kp_start += rest_len;
  }
}
else                    /* set positions for aliased object and       */
			/* master knowledge only                      */
{
  key_al_mk_only.k_part[1].kp_start = alob_pos;
  key_al_mk_only.k_part[2].kp_start = mk_pos + INTSIZE;
			/* Add index for one attr value               */
  if                    /* index cannot be added                      */
    (D26_ISADDINDEX(svc_c_route_nolog,isfd,&key_al_mk_only) < 0)
  {                     /* check error                                */
    if                  /* index did not exist before                 */
      (iserrno != EKEXISTS)
    {                   /* log error                                  */
      dce_svc_printf(GDS_S_ISADDINDEX_ERR_MSG,iserrno);
      return(D26_ERROR);
    }
  }
}

return(D2_NO_ERR);

}                       /* d26_gi32_make_key                          */
