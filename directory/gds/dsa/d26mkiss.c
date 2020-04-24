/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26mkiss.c,v $
 * Revision 1.1.10.2  1996/02/18  19:45:36  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:26:53  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:46:08  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:07:18  root]
 * 
 * Revision 1.1.8.5  1994/10/05  15:39:13  keutel
 * 	OT 12481: multiple routing
 * 	[1994/10/05  14:36:45  keutel]
 * 
 * Revision 1.1.8.4  1994/06/21  14:45:37  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  11:53:33  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:52:25  marrek
 * 	Bug fixes for April 1994 submission.
 * 	[1994/05/04  14:25:12  marrek]
 * 
 * 	Bug fixes in April 1994 submission.
 * 	[1994/05/03  16:40:20  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:22:37  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:09:28  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:21:20  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  16:06:09  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:15:06  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  14:20:23  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:12:51  marrek]
 * 
 * Revision 1.1.4.6  1993/02/02  14:50:38  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.5  1993/02/02  11:23:54  marrek
 * 	replace <malloc.h> by <stdlib.h>
 * 	logging introduced
 * 	[1993/02/01  14:58:37  marrek]
 * 
 * Revision 1.1.4.4  1992/12/31  19:41:07  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:20:08  bbelch]
 * 
 * Revision 1.1.4.3  1992/12/17  23:13:08  tom
 * 	Bug 6411 - Add setlocale call for i18n.
 * 	[1992/12/17  23:08:25  tom]
 * 
 * Revision 1.1.4.2  1992/11/27  19:46:57  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:13:31  marrek]
 * 
 * Revision 1.1.2.3  1992/06/30  20:56:58  melman
 * 	GDS drop of 6/16/92
 * 	[1992/06/30  20:35:56  melman]
 * 
 * Revision 1.1.2.2  1992/06/01  23:50:23  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:56:40  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26mkiss.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:45:36 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

#include <d26dsa.h>
#include <dce/dcesvcmsg.h>

/*****  Operating-System-Includes  *****/

#include <errno.h>
#include <stdlib.h>
#include <locale.h>

/*****  external Includes    *****/

#include <d2shm.h>
#include <dce/d27util.h>

/*****  internal Includes    *****/

#include <d26isam.h>
#include <d26svcis.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

				/* positions in schema record of ...  */
				/* ... common name attribute          */
#define D26_CNPOS (LONGSIZE + INTSIZE)
				/* ... aliased object attribute       */
#define D26_AOPOS (D26_CNPOS + INTSIZE + D26_LPHON + INTSIZE + \
		   2 * (D26_CNL + 1))
				/* ... time stamp attribute           */
#define D26_TSPOS (D26_AOPOS + INTSIZE + LONGSIZE)
				/* ... SRT attribute                  */
#define D26_SRTPOS (D26_TSPOS + INTSIZE + D26_TLEN)
				/* ... OCT attribute                  */
#define D26_OCTPOS (D26_SRTPOS + INTSIZE)
				/* ... AT attribute                   */
#define D26_ATPOS (D26_OCTPOS + INTSIZE)
				/* ... master knowledge attribute     */
#define D26_MKPOS (D26_ATPOS + INTSIZE)
				/* ... ACL attribute                  */
#define D26_ACLPOS (D26_MKPOS + INTSIZE + LONGSIZE)
				/* ... object class attribute         */
#define D26_OCLPOS (D26_ACLPOS + INTSIZE + (D2_ACLIDX_MAX * LONGSIZE))
				/* length of schema record            */
#define D26_LSHOBJ (D26_OCLPOS + INTSIZE + (D26_OCL_VAL_COUNT * LONGSIZE))

				/* No of elements in table entries    */
#define D26_NR_SRT_ELEM 4
#define D26_NR_OCT_ELEM 7
#define D26_NR_ADT_ELEM 10

#define D26_REC_ATTR_COUNT  3   /* count of recurring attributes in   */
				/* schema object                      */

				/* maximum size of attribute values   */
				/* to be stored at the object         */
#define D26_MSIZE_OBJ_ATTR  200
#define D26_EOH '%'             /* end of header character            */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        DATA                 */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/******** IMPORT ********/

extern FILE  *d26_ms1_init(char *);
extern signed16 d26_ms2_terminate(FILE *,char *);

/******** LOCAL  ********/

static signed16 d26_msi2_fill_record(FILE *fp, signed32 count_rec_at,
	char *record, signed32 *count_of_values, int isfd, long *recnum);
static signed16 d26_msi3_fill_srt_record(FILE *fp, char *record,
	long recnum);
static signed16 d26_msi4_fill_oct_record(FILE *fp, char *record,
	long recnum);
static signed16 d26_msi5_fill_at_record(FILE *fp, char *record, long recnum);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :       main(argc,argv)        [main]                        */
/*                                                                    */
/* AUTHOR:      E. Kraemer, D AP 11                                   */
/*                                                                    */
/* DATE:        22.12.92                                              */
/*                                                                    */
/* DESCRIPTION: The function calls the initializeing function. It     */
/*              reads from the input file all the data, which the     */
/*              schema object consists of. It generates the C-ISAM    */
/*              files to contain the schema object in the appropriate */
/*              size and writes the data into C-ISAM records of these */
/*              files.                                                */
/*                                                                    */
/* EXITVALUE:                                                         */
/*      D2_NO_ERR       Short           No error occurred             */
/*          1                           Error occurred                */
/*                                                                    */
/*exoff ***************************************************************/

int main(
    int argc,           /* IN    -  count of command line parameters  */
    char *argv[])       /* IN    -  command line parameters           */

{                       /* main                                       */

  static int reclen[] = /* record lengths for ...                     */
  {
			/* ... structure rule table attribute         */
    INTSIZE + LONGSIZE + D2_L_SRT_ATT + 1,
			/* ... object class table attribute           */
    INTSIZE + LONGSIZE + D2_L_OCT_ATT + 1,
			/* ... attribute table attribute              */
    INTSIZE + LONGSIZE + D2_L_AT_ATT + 1
  };

			/* primary key for schema object file         */
  static struct keydesc pk = {ISNODUPS,1,{{0,LONGSIZE,LONGTYPE}}};

			/* key for DN of schema object                */
  static struct keydesc name_key =
  {
    ISDUPS + COMPRESS,5,
    {
			/* object number                              */
      {LONGSIZE,INTSIZE,INTTYPE},
			/* namepart                                   */
      {D26_CNPOS + INTSIZE + D26_LPHON + INTSIZE,D26_CNL + 1,CHARTYPE},
			/* aliased object                             */
      {D26_AOPOS,INTSIZE,INTTYPE},
			/* master knowledge                           */
      {D26_MKPOS + INTSIZE,LONGSIZE,LONGTYPE},
			/* record number                              */
      {0,LONGSIZE,LONGTYPE}
    }
  };

			/* key for aliased object attribute of schema */
			/* object                                     */
  static struct keydesc alob_key =
  {
    ISDUPS + COMPRESS,4,
    {
			/* object number                              */
      {LONGSIZE,INTSIZE,INTTYPE},
			/* aliased object                             */
      {D26_AOPOS,INTSIZE,INTTYPE},
			/* master knowledge                           */
      {D26_MKPOS + INTSIZE,LONGSIZE,LONGTYPE},
			/* record number                              */
      {0,LONGSIZE,LONGTYPE}
    }
  };

			/* key for object class attribute of schema   */
			/* object                                     */
  static struct keydesc obcl_key =
  {
    ISDUPS + COMPRESS,3,
    {
			/* object number                              */
      {LONGSIZE,INTSIZE,INTTYPE},
			/* object class                               */
      {D26_OCLPOS + INTSIZE,LONGSIZE,LONGTYPE},
			/* record number                              */
      {0,LONGSIZE,LONGTYPE}
    }
  };

			/* primary key for attribute files of schema  */
			/* object                                     */
  static struct keydesc attr_key =
  {
    ISDUPS + COMPRESS,3,
    {
			/* object file number                         */
      {0,INTSIZE,INTTYPE},
			/* object record number                       */
      {INTSIZE,LONGSIZE,LONGTYPE},
			/* attribute value                            */
      {INTSIZE + LONGSIZE,0,CHARTYPE}
    }
  };

  FILE  *fp;            /* file pointer for input file                */
			/* name of C-ISAM file                        */
  char  name[D27_LFILE_NAME];
  char  record[2048];   /* memory for C-ISAM record                   */
  int   isfd;           /* C-ISAM file descriptor                     */
			/* count of values of recurring attributes    */
			/* of schema object                           */
  signed32 count_of_values[D26_REC_ATTR_COUNT];
  long  recnum;         /* record number of schema object             */
  signed16 i;           /* loop variable                              */
  int   exit_value = 0; /* exit value                                 */

  error_status_t st;    /* error status for serviceability functions  */

  setlocale(LC_ALL, "");

  if                    /* command line argument is missing           */
    (argc < 2)
  {                     /* write usage message                        */
    fprintf(stderr,"usage: %s <filename>\n",argv[0]);
    exit(1);
  }
  else
  {                     /* prepare logging                            */
    d27_001_svc_init(argc,argv,&st);
    if (st != svc_s_ok)
    {
      fprintf(stderr,"Unable to initialize serviceability, status: 0x%lx\n",
	      st);
    }
    dce_svc_printf(GDS_S_ENTRY_MKISS_MSG);

    if                  /* program cannot be initialized              */
      ((fp = d26_ms1_init(argv[1])) == NULL)
    {                   /* exit error                                 */
      exit_value = 1;
    }
    else                /* generate file name                         */
    {
      sprintf(name,".%s%d",D26_OBJECTS,0);
      if                /* schema object file cannot be created       */
	((isfd = D26_ISBUILD(svc_c_sev_fatal,name,D26_LSHOBJ,&pk,
	 ISINOUT + ISEXCLLOCK)) < 0)
      {                 /* set exit value                             */
	exit_value = 1;
      }
      else
      {
	if              /* index on name of schema cannot be created  */
	  (D26_ISADDINDEX(svc_c_sev_fatal,isfd,&name_key) < 0 ||
	   D26_ISADDINDEX(svc_c_sev_fatal,isfd,&alob_key) < 0)
	{               /* set exit value                             */
	  exit_value = 1;
	}
	else
	{               /* create object class indices                */
	  for           /* all object class values                    */
	     (i = 0; i < D26_OCL_VAL_COUNT; i++)
	  {             /* reset position in OCL index                */
	    if          /* index on object class value cannot be      */
			/* created                                    */
	      (D26_ISADDINDEX(svc_c_sev_fatal,isfd,&obcl_key) < 0)
	    {           /* set exit value                             */
	      exit_value = 1;
	    }
	    obcl_key.k_part[1].kp_start += LONGSIZE;
	  }
	  if            /* record cannot be filled                    */
	    (d26_msi2_fill_record(fp,D26_REC_ATTR_COUNT,record,count_of_values,
	     isfd,&recnum))
	  {             /* set exit value                             */
	    exit_value = 1;
	  }
	  else if       /* record cannot be written                   */
		 (D26_ISWRITE(svc_c_sev_fatal,isfd,record))
	  {             /* set exit value                             */
	    exit_value = 1;
	  }
	}
	if              /* schema object file cannot be closed        */
	  (D26_ISCLOSE(svc_c_sev_fatal,isfd))
	{               /* set exit value                             */
	  exit_value = 1;
	}

	for             /* all recurring attributes of schema object  */
	   (i = 0; i < D26_REC_ATTR_COUNT; i++)
	{               /* write attribute file name                  */
	  sprintf(name,".%s%d",D26_ATTR,i);
	  attr_key.k_part[2].kp_leng = (reclen[i] > MAXKEYSIZE ?
		   MAXKEYSIZE : reclen[i]) - INTSIZE - LONGSIZE;
	  if            /* attribute file cannot be created           */
	    (D26_ISBUILD(svc_c_sev_fatal,name,reclen[i],&attr_key,
	     ISINOUT + ISEXCLLOCK) < 0)
	  {             /* set exit value                             */
	    exit_value = 1;
	    break;
	  }
	  else
	  {             /* create attribute values                    */
	    while       /* still values to write                      */
		 (count_of_values[i]-- > 0 && exit_value == 0)
	    {             /* initialize record                          */
	      d26_u02_fill_field(D2_EOS,0,"",(short)reclen[i],record);
	      switch(i)
	      {
		case 0:
		  if    /* SRT value cannot be created                */
		    (d26_msi3_fill_srt_record(fp,record,recnum))
		  {     /* set exit value                             */
		    exit_value = 1;
		  }
		  break;
		case 1:
		  if    /* OCT value cannot be created                */
		    (d26_msi4_fill_oct_record(fp,record,recnum))
		  {     /* set exit value                             */
		    exit_value = 1;
		  }
		  break;
		case 2:
		  if    /* AT value cannot be created                 */
		    (d26_msi5_fill_at_record(fp,record,recnum))
		  {     /* set exit value                             */
		    exit_value = 1;
		  }
		  break;
	      }
	      if        /* record cannot be written                   */
		(D26_ISWRITE(svc_c_sev_fatal,isfd,record))
	      {         /* set exit value                             */
		exit_value = 1;
	      }
	    }
	    if          /* attribute file cannot be closed            */
	      (D26_ISCLOSE(svc_c_sev_fatal,isfd))
	    {           /* set exit value                             */
	      exit_value = 1;
	    }
	  }
	}
      }

      if                /* data version and configuration file        */
			/* cannot be written                          */
	(d26_ms2_terminate(fp,argv[1]) != D2_NOERROR)
      {                 /* set exit value                             */
	exit_value = 1;
      }
    }
    dce_svc_printf(DCE_SVC(gds_svc_handle,""),GDS_S_GENERAL,
      exit_value == 0 ? svc_c_sev_notice : svc_c_sev_error,GDS_S_EXIT_MKISS);
    exit(exit_value);
  }

}                       /* main                                       */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_msi1_write_configuration_data                     */
/*                                                                    */
/* AUTHOR:      E. Kraemer, D AP 11                                   */
/*                                                                    */
/* DATE:        22.12.92                                              */
/*                                                                    */
/* DESCRIPTION: The function writes the C-ISAM specific data into the */
/*              configuration file.                                   */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NOERROR      signed16    No error occurred                 */
/*      D2_ERROR                    Error occurred                    */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_msi1_write_configuration_data(

    FILE *fp)           /* IN    -  file pointer of configuration file*/

{                       /* d26_msi1_write_configuration_data          */

  if                    /* configuration data cannot be written       */
    (d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s: %d\n",
     "MAX_LEN_FOR_ATT_VAL_STORED_AT_OBJECT",D26_MSIZE_OBJ_ATTR) < 0)
  {                     /* return error                               */
    return(D2_ERROR);
  }
  else
  {                     /* return no error                            */
    return(D2_NOERROR);
  }

}

/**********************************************************************/
/*                                                                    */
/* NAME  :      d26_msi2_fill_record                                  */
/*                                                                    */
/* AUTHOR:      E. Kraemer, D AP 11                                   */
/*                                                                    */
/* DATE:        22.12.92                                              */
/*                                                                    */
/* DESCRIPTION: The function fills the object record of the schema    */
/*              object with data.                                     */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      0               signed16    No error occurred                 */
/*      -1                          Error occurred                    */
/*                                                                    */
/**********************************************************************/

static signed16 d26_msi2_fill_record(
    FILE     *fp,
    signed32 count_rec_at,
    char     *record,
    signed32 *count_of_values,
    int      isfd,
    long     *recnum)

{                       /* d26_msi2_fill_record                       */

  char     n_at_norm[D2_NP_LEN],n_at_ex[D2_NP_LEN],time_stamp[D26_TLEN];
  signed16 i;
  int      var_count;   /* number of actually read variables          */

  if                    /* unique ID cannot be created                */
    (D26_ISUNIQUEID(svc_c_sev_fatal,isfd,recnum) < 0)
  {                     /* return error                               */
    return(-1);
  }
  else
  {                     /* store record number                        */
    stlong(*recnum,record);
    record += LONGSIZE;
    stint(1,record);
    record += INTSIZE;
  }

  if                    /* input file cannot be read                  */
    ((var_count = fscanf(fp,"n_at_norm: %s n_at_ex: %s\n",n_at_norm,
     n_at_ex)) < 2)
  {                     /* log error and return                       */
    D27_FPCHECK(svc_c_sev_fatal,fp);
    dce_svc_printf(GDS_S_INSUFF_INFO_MSG,2,var_count);
    return(-1);
  }
  else
  {                     /* store normed and exact name                */
    stint(1,record);                /* store value counter            */
    record += INTSIZE;

    strncpy(record,"    ",D26_LPHON);
    record += D26_LPHON;
    stint(D2_PRINTABLE,record);
    record += INTSIZE;
    d26_u02_fill_field(D2_EOS,strlen(n_at_norm),n_at_norm,D26_CNL + 1,
      record);
    record += D26_CNL + 1;
    d26_u02_fill_field(D2_EOS,strlen(n_at_ex),n_at_ex,D26_CNL + 1,
      record);
    record += D26_CNL + 1;
  }

  stint(0,record);              /* store aliased object counter       */
  record += INTSIZE;
  stlong(0L,record);            /* store aliased object (invalid)     */
  record += LONGSIZE;

  stint(1,record);              /* store UTC-Time                     */
  record += INTSIZE;
  d27_013_get_utc_time(time_stamp);
  d26_u02_fill_field(D2_EOS,strlen(time_stamp),time_stamp,D26_TLEN,record);
  record += D26_TLEN;

  for                   /* all recurring attributes                   */
    (i = 0; i < count_rec_at; i++)
  {                     /* read count of attribute values             */
    if                  /* count of values cannot be read             */
      ((var_count = fscanf(fp,"count_of_values: %d\n",count_of_values + i))
       < 1)
    {                   /* log error and return                       */
      D27_FPCHECK(svc_c_sev_fatal,fp);
      dce_svc_printf(GDS_S_INSUFF_INFO_MSG,1,var_count);
      return(-1);
    }
    else
    {                   /* store count of values                      */
      stint(count_of_values[i],record);
      record += INTSIZE;
    }
  }

				/* store master knowledge             */
  stint(1,record);
  record += INTSIZE;
  stlong(-1L,record);
  record += LONGSIZE;

				/* store ACL                          */
  stint(1,record);
  record += INTSIZE;
  for                           /* All ACL components                 */
     (i = 0; i < 20; i++)
  {                             /* access for whole tree              */
    stlong((long)(i % D2_NOIDX_ACL == 0 ? D26_O_SUBTREE - 1 : D26_EOL),
      record);
    record += LONGSIZE;
  }

  stint(0,record);              /* store object class counter         */
  record += INTSIZE;
  for                           /* all object class values            */
     (i = 0; i < D26_OCL_VAL_COUNT; i++)
  {                             /* store object class (not known)     */
    stlong(0L,record);
    record += LONGSIZE;
  }

  return(0);
}                       /* d26_msi2_fill_record                       */

/**********************************************************************/
/*                                                                    */
/* NAME  :      d26_msi3_fill_srt_record                              */
/*                                                                    */
/* AUTHOR:      E. Kraemer, D AP 11                                   */
/*                                                                    */
/* DATE:        22.12.92                                              */
/*                                                                    */
/* DESCRIPTION: The function fills an attribute record of the SRT     */
/*              attribute with data.                                  */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      0               signed16    No error occurred                 */
/*      -1                          Error occurred                    */
/*                                                                    */
/**********************************************************************/

static signed16 d26_msi3_fill_srt_record(
    FILE *fp,
    char *record,
    long recnum)

{                       /* d26_msi3_fill_srt_record                   */

  int ob_nr,po_nr;
  char acr_na[D2_NO_AVA * (D2_L_ACRONYM + 1)];
  char acr_oc[D2_L_ACRONYM + 1];
  int  var_count;       /* number of actually read variables          */
			    /* store object-filenumber 0              */
  stint(0,record);
  record += INTSIZE;
			    /* store object-record-number             */
  stlong(recnum,record);
  record += LONGSIZE;

  if                    /* input line cannot be read                  */
    ((var_count = fscanf(fp,
      "ob_nr: %d po_nr: %d acr_na: %[^@]@ acr_oc: %s com: %*s\n",
      &ob_nr,&po_nr,acr_na,acr_oc)) < D26_NR_SRT_ELEM)
  {                     /* log error and return                       */
    D27_FPCHECK(svc_c_sev_fatal,fp);
    dce_svc_printf(GDS_S_INSUFF_INFO_MSG,D26_NR_SRT_ELEM,var_count);
    return(-1);
  }
  else
  {                     /* write to record                            */
    sprintf(record,"%d %d %s : %s",ob_nr,po_nr,acr_na,acr_oc);
    return(0);
  }
}                       /* d26_msi3_fill_srt_record                   */

/**********************************************************************/
/*                                                                    */
/* NAME  :      d26_msi4_fill_oct_record                              */
/*                                                                    */
/* AUTHOR:      E. Kraemer, D AP 11                                   */
/*                                                                    */
/* DATE:        22.12.92                                              */
/*                                                                    */
/* DESCRIPTION: The function fills an attribute record of the OCT     */
/*              attribute with data.                                  */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      0               signed16    No error occurred                 */
/*      -1                          Error occurred                    */
/*                                                                    */
/**********************************************************************/

static signed16 d26_msi4_fill_oct_record(
    FILE *fp,
    char *record,
    long recnum)

{                       /* d26_msi4_fill_oct_record                   */

  static char *empty_acronym = "---";
  char acr_oc[D2_L_ACRONYM + 1];
  char oid_oc[D2_OBJ_IDL_MAX + 1];
  char nam_oc[D2_L_AT_NAME + 1];
  int  kind;
  int  fil_nr;
  char sup_cl[D2_NO_SUPCL * (D2_L_ACRONYM + 1)];
  char aux_cl[D2_NO_AUXCL * (D2_L_ACRONYM + 1)];
  char mand[D2_NO_MAND * (D2_L_ACRONYM + 1)];
  char opt[D2_NO_OPT * (D2_L_ACRONYM + 1)];

  int  var_count;       /* number of actually read variables          */

			    /* store object-filenumber 0              */
  stint(0,record);
  record += INTSIZE;
			    /* store object-record-number             */
  stlong(recnum,record);
  record += LONGSIZE;

  if                    /* input line cannot be read                  */
    ((var_count = fscanf(fp,
      "acr_oc: %s oid_oc: %s nam_oc: %s kind: %d fil_nr: %d ac_sc: %[^@]@ ac_ac: %[^@]@ mand: %[^@]@ opt: %[^@]@\n",
      acr_oc,oid_oc,nam_oc,&kind,&fil_nr,sup_cl,aux_cl,mand,opt))
      < D26_NR_OCT_ELEM)
  {                     /* log error and return                       */
    D27_FPCHECK(svc_c_sev_fatal,fp);
    dce_svc_printf(GDS_S_INSUFF_INFO_MSG,D26_NR_OCT_ELEM,var_count);
    return(-1);
  }
  else
  {                     /* write to record                            */
    sprintf(record,"%s %s %s %d %d",acr_oc,oid_oc,nam_oc,kind,fil_nr);
    record += strlen(record);

    if                  /* superclass is present                      */
      (strcmp(sup_cl,empty_acronym))
    {                   /* write superclass to record                 */
      sprintf(record," %s",sup_cl);
      record += strlen(record);
    }
    sprintf(record," :");
    record += strlen(record);

    if                  /* auxiliary classes present                  */
      (strcmp(aux_cl,empty_acronym))
    {                   /* write auxiliary classes                    */
      sprintf(record," %s",aux_cl);
      record += strlen(record);
    }
    sprintf(record," :");
    record += strlen(record);

    if                  /* mandatory attributes present               */
      (strcmp(mand,empty_acronym))
    {                   /* write mandatory attributes                 */
      sprintf(record," %s",mand);
      record += strlen(record);
    }
    sprintf(record," :");
    record += strlen(record);

    if                  /* optional attributes present                */
      (strcmp(opt,empty_acronym))
    {                   /* write optional attributes                  */
      sprintf(record," %s",opt);
    }
    return(0);
  }
}                       /* d26_msi4_fill_oct_record                   */

/**********************************************************************/
/*                                                                    */
/* NAME  :      d26_msi5_fill_at_record                               */
/*                                                                    */
/* AUTHOR:      E. Kraemer, D AP 11                                   */
/*                                                                    */
/* DATE:        22.12.92                                              */
/*                                                                    */
/* DESCRIPTION: The function fills an attribute record of the AT      */
/*              attribute with data.                                  */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      0               signed16    No error occurred                 */
/*      -1                          Error occurred                    */
/*                                                                    */
/**********************************************************************/

static signed16 d26_msi5_fill_at_record(
    FILE *fp,
    char *record,
    long recnum)

{                       /* d26_msi5_fill_at_record                    */

  char acr_at[D2_L_ACRONYM + 1];
  char oid_at[D2_OBJ_IDL_MAX + 1];
  char nam_at[D2_L_AT_NAME + 1];
  int  low_b,up_b,nr_rec,at_syn,phon,acl,ind_lev;
  int  var_count;       /* number of actually read variables          */

			    /* store object-filenumber 0              */
  stint(0,record);
  record += INTSIZE;
			    /* store object-record-number             */
  stlong(recnum,record);
  record += LONGSIZE;

  if                    /* input line cannot be read                  */
    ((var_count = fscanf(fp,
     "acr_at: %s oid_at: %s nam_at: %s low_b: %d up_b: %d nr_rec: %d at_syn: %d phon: %d acl: %d ind_lev: %d\n",
     acr_at,oid_at,nam_at,&low_b,&up_b,&nr_rec,&at_syn,&phon,&acl,
     &ind_lev)) < D26_NR_ADT_ELEM)
  {                     /* log error and return                       */
    D27_FPCHECK(svc_c_sev_fatal,fp);
    dce_svc_printf(GDS_S_INSUFF_INFO_MSG,D26_NR_ADT_ELEM,var_count);
    return(-1);
  }
  else
  {                     /* write AT attribute to record               */
    sprintf(record,"%s %s %s %d %d %d %d %d %d %d",acr_at,oid_at,nam_at,
      low_b,up_b,nr_rec,at_syn,phon,acl,ind_lev);
    return(0);
  }
}                       /* d26_msi5_fill_at_record                    */
