/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26rschis.c,v $
 * Revision 1.1.10.2  1996/02/18  19:45:51  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:27:14  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:47:13  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:07:40  root]
 * 
 * Revision 1.1.8.4  1994/06/21  14:45:47  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:33:43  marrek]
 * 
 * Revision 1.1.8.3  1994/05/10  15:52:38  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:23:46  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:13:27  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:22:24  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:52:32  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:17:14  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  14:33:59  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:16:15  marrek]
 * 
 * Revision 1.1.4.2  1992/12/31  19:41:58  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:20:46  bbelch]
 * 
 * Revision 1.1.2.2  1992/06/01  19:39:20  zeliff
 * 	Commenting out non-ANSI #ident
 * 	[1992/05/28  15:48:55  zeliff]
 * 
 * 	New files from GDS drop
 * 	[1992/05/27  19:34:59  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26rschis.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:45:51 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*                                                                    */
/* NAME         : d26rschis.c        [rschis]                         */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 18.03.92                                            */
/*                                                                    */
/* COMPONENT    : DSA, main utilities                                 */
/*                                                                    */
/* DOC.-NR.     : <relevante Dokumente>                               */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* DESCRIPTION  :   The module contains the CISAM function, that reads*/
/*                  in the internal DSA schema.                       */
/*                                                                    */
/* SYSTEM-DEPENDENCY  :                                               */
/*                      SINIX                                         */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Datum  |  Aenderungen                   | KZ | CR# FM# */
/*          | 18.03.92| birth                          | ek |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

#include <sys/types.h>
#include <sys/stat.h>
#include <errno.h>

/*****  external Includes    *****/

#include <dce/d27util.h>

/*****  internal Includes    *****/

#include <d26isam.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

#define D26_SCHEME    "/scheme"

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

/******** EXPORT ********/
				    /* pathname of chdb for call      */
char       d26_chdb_name[D27_LFILE_NAME];

				    /* name of chdb program           */
char       *d26_chdb_prog = "/gdschdb";

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_mi9_read_scheme                                 */
/*                                                                    */
/* AUTHOR       : Kraemer, D AP 11                                    */
/* DATE         : 18.03.92                                            */
/*                                                                    */
/* DESCRIPTION  : The Function reads the scheme from a system_file    */
/*                into memory. Locations to write to are passed in    */
/*                the parameters.                                     */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR       Short           No error occurred             */
/*         -1                           System error occurred         */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_mi9_read_scheme(
			/* IN    -  name of directory containing the  */
    char       *sch_dir,/*          schema                            */
    signed32   *nosrt,  /* OUT   -  Number of SRT entries             */
    D26_srt    **srt,   /* OUT   -  Structure rule table (SRT)        */
			/* OUT   -  Array of naming attribue          */
    D26_nam_at **nam_at,/*          structures                        */
			/* OUT   -  Positions of naming attributes    */
    signed16   **nam_pos,
			/* OUT   -  object class arc structures       */
    D26_ocl_arc **ocl_arc,
			/* OUT   -  Number of OCT entries             */
    signed32   *nooct,
    D26_oct    **oct,   /* OUT   -  Object class table (OCT)          */
			/* OUT   -  Array of OCT indices              */
    signed16   **oct_idx,
    signed32   *noat,   /* OUT   -  Number af AT entries              */
    D26_at     **at,    /* OUT   -  Attribute table (AT)              */
    signed16   **at_pos,/* OUT   -  Positions of other attributes     */
			/* OUT   -  Object identifier string          */
    byte       **obid_str,
    signed32   *nofile, /* OUT   -  Number of object files            */
			/* OUT   -  File descriptor structures for    */
    D26_fildes **ofildes,           /* objects                        */
    signed32   *oreclen,/* OUT   -  length of object record           */
    char       **orec,  /* OUT   -  object record                     */
    signed32   *nafile, /* OUT   -  Number of attribute files         */
			/* OUT   -  File descriptor structures for    */
    D26_fildes **afildes,           /* attributes                     */
    signed32   *areclen,/* OUT   -  length of attribute record        */
    char       **arec,  /* OUT   -  attribute record                  */
    byte       *cntr_bl,/* OUT   -  country bitlist                   */
			/* OUT   -  object identifier for soundex     */
    D2_obj_id  *soundex_obid)       /* algorithm                      */

{                /*  d26_mi9_read_scheme                              */
			/* name of scheme file                        */
  char  filename[D27_LFILE_NAME];
  FILE  *fp;            /* File pointer for scheme file               */
  int   var_count;      /* number of actually read variables          */
  int   len_obids;      /* length in chars of d26_obid_str            */
  int   no_oct_indices; /* count of numbers required for all indices  */
			/* in OCT entries                             */
  int   no_na_indices;  /* count of name structures in SRT entries    */
  int   no_ocl_arcs;    /* count of object class arcs in SRT entries  */
  char  separator;      /* memory to read a single separator character*/

  D26_srt *srtp;        /* local pointer to SRT array                 */
  D26_nam_at *nam_atp;  /* local pointer to naming attributes array   */
  D26_ocl_arc *ocl_arcp;/* local pointer to object class arcs array   */
  D26_oct *octp;        /* local pointer to OCT array                 */
  D26_at *atp;          /* local pointer to AT array                  */
  signed16 *oct_idxp;   /* pointer to OCT-index array                 */
  byte  *ob_idp;        /* pointer to object identifiers              */
  signed16 *posp;       /* position pointer                           */

  register signed16 i,j;/* loop variables                             */
			/* return value                               */
  register signed16 ret_value = D2_NO_ERR;

  sprintf(filename,"%s%s",sch_dir,D26_SCHEME);

  if            /* error from opening scheme file                     */
    ((fp = D27_FOPEN(svc_c_sev_fatal,filename,"r")) == NULL)
  {             /* return error                                       */
    return(-1);
  }
  else
  {             /* read in scheme file                                */
    if          /* error from reading initalizing numbers             */
      ((var_count = fscanf(fp,
       "%*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %d %*s %*s\n",
       nosrt,nooct,noat,nofile,nafile,oreclen,areclen,&len_obids,
       &no_oct_indices,&no_ocl_arcs,&no_na_indices)) < 11)
    {           /* set return code                                    */
      D27_FPCHECK(svc_c_sev_fatal,fp);
      dce_svc_printf(GDS_S_INSUFF_INFO_MSG,11,var_count);
      ret_value = -1;
    }
    else if     /* error from allocating memory                       */
	   ((*srt = (D26_srt *)D27_CALLOC(svc_c_sev_fatal,*nosrt,
	      sizeof(D26_srt))) == NULL ||
	   (*oct = (D26_oct *)D27_CALLOC(svc_c_sev_fatal,*nooct,
	      sizeof(D26_oct))) == NULL ||
	   (*at  = (D26_at *)D27_CALLOC(svc_c_sev_fatal,*noat,
	      sizeof(D26_at))) == NULL ||
	   (*nam_pos = (signed16 *)D27_CALLOC(svc_c_sev_fatal,no_na_indices,
	      *nofile * sizeof(signed16))) == NULL ||
	   (*at_pos = (signed16 *)D27_CALLOC(svc_c_sev_fatal,*noat,
	      *nofile * sizeof(signed16))) == NULL ||
	   (*ofildes = (D26_fildes *)D27_CALLOC(svc_c_sev_fatal,*nofile,
	      sizeof(D26_fildes))) == NULL ||
	   (*afildes = (D26_fildes *)D27_CALLOC(svc_c_sev_fatal,*nafile,
	      sizeof(D26_fildes))) == NULL ||
	   (*orec = (char *)D27_MALLOC(svc_c_sev_fatal,*oreclen)) == NULL ||
	   (*arec = (char *)D27_MALLOC(svc_c_sev_fatal,*areclen)) == NULL ||
	   (*obid_str = (byte *)D27_CALLOC(svc_c_sev_fatal,len_obids,
	      sizeof(byte))) == NULL ||
	   (*oct_idx = (signed16 *)D27_CALLOC(svc_c_sev_fatal,no_oct_indices,
	      sizeof(signed16))) == NULL ||
	   (*ocl_arc = (D26_ocl_arc *)D27_CALLOC(svc_c_sev_fatal,
	      no_ocl_arcs,sizeof(D26_ocl_arc))) == NULL ||
	   (*nam_at  = (D26_nam_at *)D27_CALLOC(svc_c_sev_fatal,
	      no_na_indices,sizeof(D26_nam_at))) == NULL)
    {           /* set return code                                    */
      ret_value = -1;
    }
    else if     /* error during reading separator character           */
	 (D27_FREAD(svc_c_sev_fatal,(void *)&separator,1,1,fp) == 0)
    {           /* set return code                                    */
      ret_value = -1;
    }
    else if     /* error during reading country-bitlist               */
	   (D27_FREAD(svc_c_sev_fatal,(void *)cntr_bl,D27_CNTRY_BITLIST_LEN,
	    1,fp) == 0)
    {           /* set return code                                    */
      ret_value = -1;
    }
    else if     /* error during reading structure rule table          */
	   (D27_FREAD(svc_c_sev_fatal,(void *)*srt,sizeof(D26_srt),*nosrt,fp)
	    == 0)
    {           /* set return code                                    */
      ret_value = -1;
    }
    else if     /* error during reading object class table            */
	   (D27_FREAD(svc_c_sev_fatal,(void *)*oct,sizeof(D26_oct),*nooct,fp)
	    == 0)
    {           /* set return code                                    */
      ret_value = -1;
    }
    else if     /* error during reading attribute table               */
	   (D27_FREAD(svc_c_sev_fatal,(void *)*at,sizeof(D26_at),*noat,fp)
	    == 0)
    {           /* set return code                                    */
      ret_value = -1;
    }
    else if     /* error during reading object class arcs table       */
	   (D27_FREAD(svc_c_sev_fatal,(void *)*ocl_arc,sizeof(D26_ocl_arc),
	    no_ocl_arcs,fp) == 0)
    {           /* set return code                                    */
      ret_value = -1;
    }
    else if     /* error during reading object file descriptor table  */
	   (D27_FREAD(svc_c_sev_fatal,(void *)*ofildes,sizeof(D26_fildes),
	    *nofile,fp) == 0)
    {           /* set return code                                    */
      ret_value = -1;
    }
    else if     /* error during reading attr. file descriptor table   */
	   (D27_FREAD(svc_c_sev_fatal,(void *)*afildes,sizeof(D26_fildes),
	    *nafile,fp) == 0)
    {           /* set return code                                    */
      ret_value = -1;
    }
    else if     /* error during reading soundex identifier structure  */
	   (D27_FREAD(svc_c_sev_fatal,(void *)soundex_obid,
	    sizeof(D2_obj_id),1,fp) == 0)
    {           /* set return code                                    */
      ret_value = -1;
    }
    else if     /* error object id octal string                       */
	   (D27_FREAD(svc_c_sev_fatal,(void *)*obid_str,sizeof(char),
	    len_obids,fp) == 0)
    {           /* set return code                                    */
      ret_value = -1;
    }

    for         /* All AT entries                                     */
       (i = 0, atp = *at, ob_idp = *obid_str; i < *noat; i++, atp++ )
    {           /* set the type pointer                               */
      atp->d26a_type.d2_type = ob_idp;
      ob_idp += atp->d26a_type.d2_typ_len;
    }

    for         /* All OCT entries                                    */
       (i = 0, octp = *oct; i < *nooct; i++, octp++ )
    {           /* set the type pointer                               */
      octp->d26c_ob_id.d2_type = ob_idp;
      ob_idp += octp->d26c_ob_id.d2_typ_len;
    }

    soundex_obid->d2_type = ob_idp;
    ob_idp += soundex_obid->d2_typ_len;

    for         /* All SRT entries                                    */
       (i = 0, srtp = *srt, nam_atp = *nam_at, ocl_arcp = *ocl_arc;
	i < *nosrt && ret_value == D2_NO_ERR;
	i++, srtp++, nam_atp++, ocl_arcp++)
    {           /* set the naming attribute pointer                   */
      srtp->d26s_nam_at = nam_atp;
      for       /* all naming attributes of SRT entry                 */
	 (; ret_value == D2_NO_ERR; nam_atp++)
      {         /* read in the AT-index of the naming attribute       */
	if      /* AT-index of naming attribute cannot be read        */
	  ((var_count = fscanf(fp,"%hd ",&nam_atp->d26s_index)) < 1)
	{       /* log the error and break                            */
	  D27_FPCHECK(svc_c_sev_fatal,fp);
	  dce_svc_printf(GDS_S_INSUFF_INFO_MSG,1,var_count);
	  ret_value = -1;
	}
	else if /* separator was read in                              */
	       (nam_atp->d26s_index == -1)
	{       /* break loop                                         */
	  break;
	}
      }

      if        /* no error occurred                                  */
	(ret_value == D2_NO_ERR)
      {         /* set the object class arcs pointer                  */
	srtp->d26s_ocl_arc = ocl_arcp;
	while   /* separating ocl arc not reached                     */
	     (ocl_arcp->d26s_oclass != -1)
	{       /* increment the pointer                              */
	  ocl_arcp++;
	}
      }
    }

    for         /* All OCT indices                                    */
       (i = 0, oct_idxp = *oct_idx;
	ret_value == D2_NO_ERR && i < no_oct_indices; i++, oct_idxp++ )
    {           /* read the OCT index in                              */
      if        /* error from reading                                 */
	((var_count = fscanf(fp,"%hd ",oct_idxp)) < 1)
      {         /* log the error and break                            */
	D27_FPCHECK(svc_c_sev_fatal,fp);
	dce_svc_printf(GDS_S_INSUFF_INFO_MSG,1,var_count);
	ret_value = -1;
      }
    }

    if          /* no error occurred                                  */
      (ret_value == D2_NO_ERR)
    {           /* Extract all the OCT indices                        */
      for       /* all object classes                                 */
	 (i = 0, octp = *oct, oct_idxp = *oct_idx; i < *nooct; i++, octp++)
      {         /* set the superclass pointer                         */
	octp->d26c_sup_cl = oct_idxp;
	while   /* separator not reached                              */
	     (*oct_idxp++ != -1)
	{       /* nothing                                            */
	}

		/* set the auxiliary class pointer                    */
	octp->d26c_aux_cl = oct_idxp;
	while   /* separator not reached                              */
	     (*oct_idxp++ != -1)
	{       /* nothing                                            */
	}

		/* set the mandatory attribute pointer                */
	octp->d26c_mandatory = oct_idxp;
	while   /* separator not reached                              */
	     (*oct_idxp++ != -1)
	{       /* nothing                                            */
	}       /*                                                    */

		/* set the optional attribute pointer                 */
	octp->d26c_optional = oct_idxp;
	while   /* separator not reached                              */
	     (*oct_idxp++ != -1)
	{       /* nothing                                            */
	}
      }
    }

    for         /* All SRT entries                                    */
       (i = 0, srtp = *srt, posp = *nam_pos;
	i < *nosrt && ret_value == D2_NO_ERR; i++, srtp++)
    {           /* set the naming attribute positions                 */
      for       /* all naming attributes of SRT entry                 */
	 (nam_atp = srtp->d26s_nam_at;
	  ret_value == D2_NO_ERR && nam_atp->d26s_index != -1; nam_atp++)
      {         /* set the position pointer of the naming attribute   */
	for     /* all the object files                               */
	   (j = 0, nam_atp->d26s_position = posp;
	    ret_value == D2_NO_ERR && j < *nofile; j++, posp++)
	{       /* read the single file position                      */
	  if    /* position cannot be read                            */
	    ((var_count = fscanf(fp,"%hd ",posp)) < 1)
	  {     /* log the error and break                            */
	    D27_FPCHECK(svc_c_sev_fatal,fp);
	    dce_svc_printf(GDS_S_INSUFF_INFO_MSG,1,var_count);
	    ret_value = -1;
	  }
	}
      }
    }

    for         /*  All AT entries                                    */
       (i = 0, atp = *at, posp = *at_pos;
	i < *noat && ret_value == D2_NO_ERR; i++, atp++)
    {           /* set the attribute positions                        */
      for       /* all the object files                               */
	 (j = 0, atp->d26a_field_pos = posp;
	  ret_value == D2_NO_ERR && j < *nofile; j++, posp++)
      {         /* read the single file position                      */
	if      /* position cannot be read                            */
	  ((var_count = fscanf(fp,"%hd ",posp)) < 1)
	{       /* log the error and break                            */
	  D27_FPCHECK(svc_c_sev_fatal,fp);
	  dce_svc_printf(GDS_S_INSUFF_INFO_MSG,1,var_count);
	  ret_value = -1;
	}
      }
    }

    if          /* error from closing scheme file                     */
      (D27_FCLOSE(svc_c_sev_fatal,fp))
    {           /* set return code                                    */
      ret_value = -1;
    }
  }

  if            /* no error occurred                                  */
    (ret_value == D2_NO_ERR)
  {             /* log success                                        */
    DCE_SVC_LOG((GDS_S_SCHEME_READ_MSG));
  }
		/* return ret_value                                   */
  return(ret_value);

}               /* d26_mi9_read_scheme                                */
