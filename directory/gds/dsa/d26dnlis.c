/* 
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/* 
 * HISTORY
 * $Log: d26dnlis.c,v $
 * Revision 1.1.719.2  1996/02/18  19:44:58  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:26:07  marty]
 *
 * Revision 1.1.719.1  1995/12/08  15:43:39  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:06:28  root]
 * 
 * Revision 1.1.717.4  1994/06/21  14:45:14  marrek
 * 	June 1994 code submission.
 * 	[1994/06/20  13:04:44  marrek]
 * 
 * Revision 1.1.717.3  1994/05/10  15:51:59  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:20:19  marrek]
 * 
 * Revision 1.1.717.2  1994/03/23  15:09:08  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:20:12  keutel]
 * 
 * Revision 1.1.717.1  1994/02/22  16:05:24  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:10:21  marrek]
 * 
 * Revision 1.1.715.2  1993/10/14  17:16:18  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:08:30  keutel]
 * 
 * Revision 1.1.715.1  1993/10/13  17:30:27  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:26:05  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  13:48:15  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:04:26  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:39:10  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:18:48  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:38:20  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:06:21  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:46:55  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:54:04  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26dnlis.c,v $ $Revision: 1.1.719.2 $ $Date: 1996/02/18 19:44:58 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODULE                                              */
/*                                                                    */
/* NAME         : d26dnlis.c                                          */
/*                                                                    */
/* AUTHOR       : Schreck, D AP 11                                    */
/* DATE         : 07.01.88                                            */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOC.-NR.     : DS-Design-Spec.                                     */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Productnr. from Productstructure - <Versionnr.>    */
/*                                                                    */
/* DESCRIPTION  : This module contains the functions to maintain      */
/*                the distinguished name list in file and core        */
/*                                                                    */
/* SYSTEM DEPENDENCIES: SINIX C-ISAM                                  */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | ID | CR# EM# */
/*   0.1    |07.01.88 | Original                       | as |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

#include <stdlib.h>
#include <isam.h>

/*****  external Includes    *****/

/*****  internal Includes    *****/

#include <dce/d27util.h>
#include <d26schema.h>
#include <d26dnlis.h>
#include <d26isam.h>
#include <d26svcis.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

#define D26_I00_NO_MEM_AREAS     10   /* maximum number of areas for       */
				      /* distinguished name strings        */
#define D26_I01_MEM_AREA_SIZE  2*8196 /* size of one area for distinguished*/
				      /* name strings                      */

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

/******** LOCAL  ********/

				    /* filedescriptor for distinguished    */
				    /* name list in file                   */
static int      d26_file_dnlist = -1;
				    /* last key set by the function        */
				    /* d26_i03_set_dnlistkey               */
static signed16 d26_act_isam_key = -1;
				    /* name of file for distinguished name */
				    /* list                                */
static char     *d26_name_dnlist = "/dnlist";

				    /* Begin of memory areas for the       */
				    /* distinguished name strings          */
static char     *d26_mem_areas[D26_I00_NO_MEM_AREAS] =
			{ NULL, NULL, NULL, NULL, NULL,
			  NULL, NULL, NULL, NULL, NULL };

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/******** LOCAL  ********/

static signed16 d26_i00_read_dnlist(void);
static signed16 d26_i03_set_dnlistkey(signed16 key1127);
static signed16 d26_i04_get_updinfo(D26_update_info *upd_info);
static signed16 d26_i10_lock_isamfile(int isfd);
static signed16 d26_i11_rep_dnlistrec(D2_name_string old_dn,
    D2_name_string new_dn, boolean *changed);
static signed16 d26_i12_add_dnlistrec(D2_dnint dnint, D2_name_string dn,
    signed32 occurrency, signed32 *dnlistcoreidx);
static signed16 d26_i13_del_dnlistrec(signed32 dnlistcoreidx,
    signed32 occurrency);

#ifndef MKUPD
static signed16 d26_i28_dn_otherint_exists(D2_dnint dnint,
    D2_name_string dn);
static signed16 d26_i29_otherval_exists(D2_name_string dn);
#endif

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i00_read_dnlist                                   */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        07.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function reads the distinguished name list       */
/*              entries from ISAM file and stores them in core        */
/*              (s. structure D26_dndesc).                            */
/*                                                                    */
/*              Memory for referral list will be allocated if         */
/*              necessary (d26_mklist).                               */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      signed16 D2_NO_ERR     successfull                            */
/*                             when max. value for d26_maxidx_dnlist  */
/*                             (== D26_MAX_DNLIDX) is exceeded        */
/*                             D2_NO_ERR is returned after            */
/*                             D26_MAX_DNLIDX values were read        */
/*                                                                    */
/*              D26_ERROR      any error occurred                     */
/*                             (C-ISAM error or not enough memory     */
/*                              available)                            */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_i00_read_dnlist(void)

{                               /*  d26_i00_read_dnlist               */

  static char *function = "i00";

				    /* record of dn list file         */
  char     dnlist_rec[D26_I02_RECDNLISTL];

  signed16 return_value = D2_NO_ERR;

				   /* variables for memory management */
  signed16 next_idx_memareas,       /* next index of d26_mem_areas    */
	   dn_length;               /* length of distinguished name   */
  char     *begin_memarea,          /* pointer to begin of memarea    */
	   *end_memarea,            /* pointer to end of memarea      */
	   *next_dn_memory;         /* pointer to begin of next dn    */

  D26_dndesc *ptr_dnlist;           /* pointer to dnlist in core      */
  char       *dnint_ptr,            /* DN interpretation in record    */
	     *ename_ptr,            /* exact name in record           */
	     *dnoccur_ptr;          /* occurrency in record           */

  signed32 max_short;       /* variable for max. index check          */
  signed32 old_dnlidx;      /* variable for check whether value of    */
			    /* d26_maxidx_dnlist was changed          */
			    /* (this information is used for          */
			    /* allocating memory for d26_dnlist)      */
			    /* pointer to old Access-point list       */
  D26_acc_point *old_psap_list;
  signed32      no_mems;    /* number of shorts or D26_dndescs which  */
			    /* memory must be allocated for           */
			    /* counter for isread calls               */
  signed32 ir_count = 0;

  register signed32 i;              /* counter                        */
  register boolean  alloc_mem;      /* indicator whether memory for   */
				    /* DN list should be allocated    */

/****************/
/* set no key   */
/****************/

  if                    /*  d26_i03_set_dnlistkey (NO_KEY) fails      */
    ((d26_i03_set_dnlistkey (D26_I04_NO_KEY)) != D2_NO_ERR)
  {                     /*  exit function                             */
    return(D26_ERROR);
  }


/****************/
/* get maxindex */
/****************/

  ir_count++;
  d26_dbacc_count++;
  if                    /*  isread (last record) fails              */
    (D26_ISREAD(svc_c_sev_warning,d26_file_dnlist,dnlist_rec,ISLAST) < 0)
  {                     /*  return error                            */
    D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,0,ir_count);
    return(D26_ERROR);
  }

/*******************/
/* save max. index */
/*******************/

  old_dnlidx = d26_maxidx_dnlist;

  if                    /*  maximum index exceeded                    */
    ((max_short = isrecnum - D26_I03_GET_CORE_INDEX) > D26_MAX_DNLIDX)
  {                     /*  error log                                 */
    dce_svc_printf(GDS_S_MAX_DN_INDEX_MSG);
    d26_maxidx_dnlist = (signed32)D26_MAX_DNLIDX;
  }
  else
  {                     /*  save maxidx                               */
    d26_maxidx_dnlist = max_short;
  }

/*********************************/
/* get memory for d26_accp_list  */
/*********************************/
			/* calculate no. of memory segments which must*/
			/* be allocated                               */
  no_mems = d26_maxidx_dnlist + 1;

  if                    /* memory for access point list insufficient  */
    ((no_mems > d26_accp_max || d26_accp_list == (D26_acc_point *)NULL) &&
     no_mems > 0)
  {                     /* check access point list allocation         */
    if                  /* access point list not allocated            */
      (d26_accp_list == (D26_acc_point *) NULL)
    {                   /* allocate memory for d26_accp_list          */
      if                /* access point list can be allocated         */
	((d26_accp_list = (D26_acc_point *)D27_CALLOC(svc_c_sev_warning,
	  no_mems,sizeof(D26_acc_point))) != NULL)
      {                 /* set access point maximum                   */
	d26_accp_max = no_mems;
      }
    }
    else
    {                   /* reallocate access point list               */
      old_psap_list = d26_accp_list;
      if                /* realloc fails                              */
	((d26_accp_list = (D26_acc_point *)D27_REALLOC(svc_c_sev_warning,
	  (void *)old_psap_list,(no_mems * sizeof(D26_acc_point)))) == NULL)
      {                 /* not enough memory for PSAP list            */
	free(old_psap_list);
      }
      else
      {                 /* initialize memory                          */
	memset((void *)d26_accp_list,(int)D2_EOS,
	  no_mems * sizeof(D26_acc_point));
	d26_accp_max = no_mems;
      }
    }
  }
  else
  {                     /* free old PSAP list memory when necessary   */
    if (d26_accp_list != (D26_acc_point *)NULL && no_mems <= 0)
    {
      free(d26_accp_list);
      d26_accp_list = NULL;
      d26_accp_max = 0;
    }
  }

/******************************/
/* get memory for new DN list */
/******************************/
  alloc_mem = TRUE;

  if                    /* old DN list in memory already              */
    (d26_dnlist != NULL)
  {
    if                  /* d26_maxidx_dnlist changed                  */
      ((d26_maxidx_dnlist > old_dnlidx) || d26_maxidx_dnlist < 0)
    {                   /* free old memory                            */
      free (d26_dnlist);
      d26_dnlist = NULL;
    }
    else
    {                   /* enough memory already allocated            */
      alloc_mem = FALSE;
    }
  }

  if                    /* new DN list not empty and memory must be   */
			/* allocated                                  */
    (d26_maxidx_dnlist >= 0 && alloc_mem == TRUE)
  {                     /* allocate memory for the new DN list        */
    if                  /* no more memory available                   */
      ((d26_dnlist = (D26_dndesc *)D27_CALLOC(svc_c_sev_warning,no_mems,
	sizeof(D26_dndesc))) == NULL)
    {                   /* set return value error                     */
      d26_maxidx_dnlist = -1L;/* no valid index for DN list in core ! */
      return_value = D26_ERROR;
    }
  }

/**************************************************************/
/* initialize variables for memory management (first memarea) */
/**************************************************************/

  next_idx_memareas = 0;/* next index of d26_mem_areas               */
  if                    /* first memory area is allocated            */
    ((begin_memarea = d26_mem_areas[next_idx_memareas]) != NULL)
  {                     /* increment next memory area                */
    next_dn_memory = begin_memarea;
    end_memarea = begin_memarea + D26_I01_MEM_AREA_SIZE;
    next_idx_memareas++;
  }

/********************/
/* read new DN list */
/********************/

  ir_count++;
  d26_dbacc_count++;
  if                    /* isread (record with version) fails         */
   (D26_ISREAD(svc_c_sev_warning,d26_file_dnlist, dnlist_rec,ISFIRST) < 0)
  {                     /* set return value error                     */
    d26_maxidx_dnlist = -1L; /* no valid index for DN list in core !  */
    return_value = D26_ERROR;
  }


			/* initialize pointers to record positions    */
  dnint_ptr   = &dnlist_rec[D26_I10_DNINT];
  ename_ptr   = &dnlist_rec[MAXKEYSIZE];
  dnoccur_ptr = &dnlist_rec[D26_I12_OCCURRENCY];

  for                   /* number of records with a dn in dnlist file */
     (i=0; i<=d26_maxidx_dnlist; i++)
  {                     /* read record with dn                        */
    ir_count++;
    d26_dbacc_count++;
    if                  /* isread (record with dn) fails              */
      (D26_ISREAD (svc_c_sev_warning,d26_file_dnlist,dnlist_rec,ISNEXT) < 0)
    {                   /* set return value error                     */
      d26_maxidx_dnlist = i - 1L;
			/* (i-1) valid indices for dn list in core    */
      return_value = D26_ERROR;
      break;
    }

   /*****************************************/
   /* set dnlist entry in core (d26_dnlist) */
   /*****************************************/
			/* set dnlist entry in core (d26_dnlist)      */
    ptr_dnlist = &d26_dnlist[i];
			/* dn interpretation (d26_dnlist.d26_dnint)   */
    ptr_dnlist->d26_dnint = *dnint_ptr;
			/* occurrency (d26_dnlist.d26_occurrency)     */
    ptr_dnlist->d26_occurrency = ldlong(dnoccur_ptr);

			/* dn (d26_dnlist.d26_dnname): look for       */
			/* memory for distingushed name string        */

   /****************************/
   /* memory for dnname string */
   /****************************/
			/* initialize variables                       */
    dn_length = strlen(ename_ptr);

    if                  /* new DN does not fit into memory            */
      (begin_memarea != NULL &&
       next_dn_memory + 2 * (dn_length + 1) > end_memarea)
    {                   /* check whether new memory area available    */
      if                /* still memory area available                */
	(next_idx_memareas < D26_I00_NO_MEM_AREAS)
      {                 /* assign next memory area                    */
	if              /* next memory area allocated                 */
	  ((begin_memarea = d26_mem_areas[next_idx_memareas]) != NULL)
	{               /* increment next memory area index           */
	  end_memarea    = begin_memarea + D26_I01_MEM_AREA_SIZE;
	  next_dn_memory = begin_memarea;
	  next_idx_memareas++;
	}
      }
      else
      {                 /* log error and break                        */
	dce_svc_printf(GDS_S_MAX_DN_MEMORY_MSG);
	d26_maxidx_dnlist = i - 1;
			/* (i-1) valid indices for dn list in core    */
	return_value = D26_ERROR;
	break;
      }
    }

    if                  /* no memory allocated                        */
      (begin_memarea == NULL)
    {                   /* allocate new memory                        */
      if                /* no more memory available                   */
	((d26_mem_areas[next_idx_memareas] = D27_MALLOC(svc_c_sev_warning,
	 D26_I01_MEM_AREA_SIZE)) == NULL)
      {                 /* set return value error                     */
	d26_maxidx_dnlist = i - 1;
			/* (i-1) valid indices for dn list in core    */
	return_value = D26_ERROR;
	break;
      }
      else
      {                 /* reset memory area                          */
	begin_memarea  = d26_mem_areas[next_idx_memareas];
	end_memarea    = begin_memarea + D26_I01_MEM_AREA_SIZE;
	next_dn_memory = begin_memarea;
	next_idx_memareas++;
      }
    }

   /******************/
   /* dn name string */
   /******************/
			/* set d26_dnlist.d26_dnname                  */
    ptr_dnlist->d26_dnname = (D2_name_string)next_dn_memory;
			/* norm dnname from record in memory          */
    d27_007_norm_name(ename_ptr,dn_length + 1,next_dn_memory);
			/* set pointer for next dnname string         */
    next_dn_memory = next_dn_memory + dn_length + 1;

    ptr_dnlist->d26_ename = (D2_name_string)next_dn_memory;
			/* copy dnname from record in memory          */
    strcpy (next_dn_memory, ename_ptr);
			/* set pointer for next dnname string         */
    next_dn_memory = next_dn_memory + dn_length + 1;
  }

  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,0,ir_count);

  return(return_value);

}                               /*  d26_i00_read_dnlist               */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i01_upd_core_dnlist                               */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        15.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function checks whether the dnlist in file was   */
/*              updated since last read of dn list. When the file     */
/*              was updated the function d26_i00_read_dnlist is       */
/*              called to read the entries of the isam file and store */
/*              them in memory.                                       */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*    signed16 D26_NO_CHANGE    no error occurred                     */
/*                              the dnlist in memory wasn't changed   */
/*                              because there was no update on dnlist */
/*                              in file                               */
/*                                                                    */
/*             D26_CHANGED      no error occurred                     */
/*                              there was an update on dnlist in file */
/*                              the new entries were successfully     */
/*                              read and stored in memory             */
/*                                                                    */
/*             D26_ERROR        any error occurred e.g. C-isam error  */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i01_upd_core_dnlist(void)

{                                   /* d26_i01_upd_core_dnlist        */

  signed16        return_value = D26_NO_CHANGE;

  D26_update_info update_info;      /* parameter for                  */
				    /* d26_i04_get_updinfo            */

/*********************/
/* open DN list file */
/*********************/

  if                    /* open of dnlist file not successfull        */
    (d26_i05_open_dnlistfile () != D2_NO_ERR)
  {                     /* ERROR: cannot open dnlist file             */
    return(D26_ERROR);
  }


/*******************/
/* get update_info */
/*******************/

  if                    /*  d26_i04_get_updinfo fails                 */
    (d26_i04_get_updinfo (&update_info) != D2_NO_ERR)
  {                     /*  ERROR: cannot get update info from dnlist file*/
    return (D26_ERROR); /* */
  }                     /* get_updinfo fails                           */


/************************************************/
/* check whether dnlist in core must be updated */
/************************************************/

  if                    /* dnlist in file was updated since last read */
			/* of dnlist (d26_i00_read_dnlist)            */
    (update_info.d26_version != d26_upd_info.d26_version)
  {
    if                  /* d26_i00_read_dnlist fails                  */
       (d26_i00_read_dnlist () != D2_NO_ERR)
    {                   /* ERROR: cannot read new dnlist from file     */
      return_value = D26_ERROR; /* */
    }
    else
    {                   /* save new update info:                      */
			/* version of last update                     */
      d26_upd_info.d26_version   = update_info.d26_version;
			/* reason of last update                      */
      d26_upd_info.d26_reason = update_info.d26_reason;

			/* set return value D26_CHANGED               */
      return_value = D26_CHANGED;
    }
  }

  return (return_value);

}                                   /* d26_i01_upd_core_dnlist        */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i02_upd_file_dnlist                               */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        18.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function updates one record in the dnlist in     */
/*              the isam file.                                        */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     signed16 D2_NO_ERR   no error occured (update successfull)     */
/*                                                                    */
/*              D26_ERROR   any error occurred (e.g. C-isam error or  */
/*                          parameter error)                          */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i02_upd_file_dnlist(
    D2_attr_mod    type,/* IN    -  type of modification              */
			/* IN    -  the old distinguished name        */
    D2_name_string old_dn,
			/* IN    -  interpretation of dn              */
    D2_dnint       dnint,
			/* IN    -  the new distinguished name        */
    D2_name_string new_dn,
			/* IN    -  delta of occurrency               */
    signed32       occurrency,
			/* IN    -  indicates a new version of DN-list*/
    boolean        new_version,
			/* INOUT -  index of DN-list entry            */
    signed32       *dnlist_core_idx)

{                               /*  d26_i02_upd_file_dnlist           */

  static char *function = "d26_i02_upd_file_dnlist";

				/* record of dn list file             */
  char     dnlistrec[D26_I02_RECDNLISTL];
  signed16 return_value = D2_NO_ERR;
				/* indicates whether C-ISAM file was  */
  boolean  changed = FALSE;     /* changed when type is D2_REP_AV     */
  char     *ptrversion;         /* ptr to version value in version    */
				/* record                             */
  signed16 rcode;               /* return code of read record function*/

/***********************/
/* record modification */
/***********************/

  switch (type)         /*  type of record modification               */
  {
    case D2_REP_AV:     /*  replace record                            */
			/*  caller is DS_REPLACE_RDN:                 */
			/*  the given old dn should be replaced by the*/
			/*  given new dn in all records which were    */
			/*  found with the old dn                     */
			/* */
      return_value = d26_i11_rep_dnlistrec (old_dn, new_dn, &changed);
      break;

    case D2_ADD_AV:     /*  "add" record                              */
			/*  a new dnlist entry should be added to isam*/
			/*  file by increasing the occurrency of an   */
			/*  existing record, by using a unused record */
			/*  in isam file (occurrency = 0) or by       */
			/*  writing a new isam file record            */
			/* */
      return_value = d26_i12_add_dnlistrec(dnint,new_dn,occurrency,
	dnlist_core_idx);
      break;
    case D2_DEL_AV:     /*  "delete" record                           */
			/*  an old dnlist entry should be "deleted"   */
			/*  in isam file by decreasing the occurrency */
			/*  of the existing record                    */
			/*  when the occurrency gets the value 0 the  */
			/*  record isn't used and can be used to "add"*/
			/*  a new dnlist entry to isam file           */
			/* */
      return_value = d26_i13_del_dnlistrec (*dnlist_core_idx, occurrency);
      break;

    case D26_NOTHING:        /*  no modification on record            */
			     /*  no action                            */
			     /*  (only version update)                */
      break;

    default:                 /* error: wrong parameter value for type      */
			     /* set return_value                           */
      return_value = D26_ERROR;
      break;
  }

/*******************************/
/* version record modification */
/*******************************/

  if (new_version == TRUE)   /*  version record should be updated     */
  {
    if                       /*  (isam file changed and type is replace)   */
			     /*  or other type                        */
      (((type == D2_REP_AV) && (changed == TRUE))
       || (type != D2_REP_AV))
    {                        /* set ptr to version value              */
      ptrversion = &dnlistrec[D26_I09_VERSION];
			     /*  read version record                  */
			     /* set record no of version record       */
      isrecnum = D26_I16_VERSION_RECNUM;
			     /* read record with lock                 */
      rcode = d26_i07_read_dnlistrec(D26_I04_NO_KEY,dnlistrec,TRUE);

      switch (rcode)      /*  return code of read                     */
      {
	case D26_I13_FOUND:
			  /*  get new version and update record value  */
	  stlong(ldlong(ptrversion) + 1L,ptrversion);

			  /*  rewrite record                           */
	  if ((D26_ISREWCURR(svc_c_sev_warning,d26_file_dnlist,dnlistrec))
	      < 0)
	  {              /* isam error occurred                       */
	    return_value = D26_ERROR;    /* set return value          */
	  }

			 /*  unlock record                            */
	  if ((D26_ISRELEASE(svc_c_sev_warning,d26_file_dnlist)) < 0)
	  {              /* isam error occurred                       */
	    return_value = D26_ERROR;    /* set return value          */
	  }
	  break;

	case D26_I14_NOT_FOUND:
			 /*  error: data inconsistency in isam file   */
	  dce_svc_printf(GDS_S_DB_INCONSISTENT_MSG);
			      /*  set return_value                         */
	  return_value = D26_ERROR;
	  break;

	default:            /* C-isam error occurred                     */
	  return_value = D26_ERROR;
	  break;
      }
    }
  }

  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_WRITE,function,1,0);
  return (return_value);

}                               /*  d26_i02_upd_file_dnlist           */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i03_set_dnlistkey                                 */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        13.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function sets the key of the distinguished name  */
/*              list in isam file when the actually set key is        */
/*              another than the wished key.                          */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*  signed16    D2_NO_ERR               no error occured              */
/*                                                                    */
/*              D26_ERROR               isstart returns an error      */
/*                                      or wrong parameter value for  */
/*                                      key given (programmers error!)*/
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_i03_set_dnlistkey(
    signed16 key)       /* IN    -  wished key                        */

{                            /*  d26_i03_set_dnlistkey             */
                             /* record of dn list file                 */
  char dnlist_rec[D26_I02_RECDNLISTL];

  struct keydesc isam_key;  /* isam description of key              */

  static char *function = "i03: ";

  if                        /* wished key != actually isam key      */
    (key != d26_act_isam_key)
  {                         /* reset key for dn list in isam file   */
    /* initialize variables which are the same for nearly all keys  */
    isam_key.k_flags  = ISDUPS + COMPRESS;
    isam_key.k_nparts = 1;

			    /* set variable isam_key                */
    switch (key)            /* wished key                           */
    {
      case D26_I04_NO_KEY:  /* no key (ordering sequence isrecnum   */
	isam_key.k_flags  = 0;
	isam_key.k_nparts = 0;
	break;
      case D26_I05_KEY_DNINTANDDN:  /* DN interpretation and DN     */
	isam_key.k_part[0].kp_start = D26_I10_DNINT;
	isam_key.k_part[0].kp_leng  = MAXKEYSIZE;
	isam_key.k_part[0].kp_type  = CHARTYPE;
	break;
      case D26_I06_KEY_OCCURRENCY:  /* occurrency (no of references */
				    /* to dn)                       */
	isam_key.k_part[0].kp_start = D26_I12_OCCURRENCY;
	isam_key.k_part[0].kp_leng  = LONGSIZE;
	isam_key.k_part[0].kp_type  = LONGTYPE;
	break;

      default:
	return (D26_ERROR);
    }


				    /* call isstart                 */
    d26_dbacc_count++;
    if                              /* error from isstart           */
      (D26_ISSTART(svc_c_sev_warning,d26_file_dnlist,&isam_key,0,
       dnlist_rec,ISFIRST) < 0)
    {                               /* return error                   */
      D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,1,0);
      return(D26_ERROR);
    }
    D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,1,0);

    d26_act_isam_key = key;         /* set act_isam_key               */
  }

  return(D2_NO_ERR);

}                               /* d26_i03_set_dnlistkey              */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i04_get_updinfo                                   */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        15.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function reads the first record of dnlist in     */
/*              isam file where the version of last modification is   */
/*              stored.                                               */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_NO_ERR   signed16    no error ouucured                     */
/*      D26_ERROR               any error occurred (e.g. C-isam error)*/
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_i04_get_updinfo(
			/* INOUT -  pointer to reason and version     */
    D26_update_info *upd_info)

{                               /*  d26_i04_get_updinfo               */
  static char *function = "i04: ";

			    /* record of dn list file                 */
  char dnlist_rec[D26_I02_RECDNLISTL];

/****************/
/* set no key   */
/****************/

  if                    /*  d26_i03_set_dnlistkey (NO_KEY) fails      */
    ((d26_i03_set_dnlistkey (D26_I04_NO_KEY)) != D2_NO_ERR)
  {                     /*  return error                              */
    return (D26_ERROR);
  }

/****************/
/* get upd_info */
/****************/

  d26_dbacc_count++;
  if                    /*  isread (record with version) fails        */
    (D26_ISREAD(svc_c_sev_warning,d26_file_dnlist,dnlist_rec,ISFIRST) < 0)
  {                     /*  return error                              */
    D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,0,1);
    return(D26_ERROR);
  }

			/*  save upd_info                             */
			/*  - reason                                  */
  upd_info->d26_reason  = dnlist_rec[D26_I08_REASON];
			/*  - version                                 */
  upd_info->d26_version = ldlong(&dnlist_rec[D26_I09_VERSION]);

  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,0,1);

  return (D2_NO_ERR);

}                           /* d26_i04_get_updinfo                    */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i05_open_dnlistfile                               */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        15.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function opens the isam file of dnlist when it   */
/*              is closed.                                            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     Short   D2_NO_ERR        no error occurred                     */
/*                              the isam file is opened               */
/*                                                                    */
/*             D26_ERROR        C-isam error occurred                 */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i05_open_dnlistfile(void)

{                           /*  d26_i05_open_dnlistfile               */

signed16 return_value = D2_NO_ERR;

				 /* absolute filename of dn list file */
char  filename_dnl[D27_LFILE_NAME];

/*********************/
/* open DN list file */
/*********************/

if                      /*  file of distinguished name list is closed      */
   (d26_file_dnlist < 0)
{                       /*  open DN list file                              */

     /* build name of DN list file */
     strcpy (filename_dnl, d26_install);
     strcat (filename_dnl, d26_name_dnlist);

     /* open file */
     d26_file_dnlist = D26_ISOPEN(svc_c_sev_warning,filename_dnl,
       ISINOUT + ISMANULOCK);
     if (d26_file_dnlist < 0)
     {
	return_value = D26_ERROR;
     }
}

return (return_value);

}                               /*  d26_i05_open_dnlistfile                */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i06_close_dnlistfile                              */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        15.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function closes the isam file of dnlist when it  */
/*              is opened.                                            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     D2_NO_ERR    signed16    no error occurred                     */
/*                              the isam file is closed               */
/*     D26_ERROR                C-isam error occurred                 */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i06_close_dnlistfile(void)

{                       /* d26_i06_close_dnlistfile                   */
  signed16 return_value = D2_NO_ERR;

  if                    /* file of distinguished name list is opened  */
    (d26_file_dnlist >= 0)
  {                     /* close DN list file                         */

    if(D26_ISCLOSE(svc_c_sev_warning,d26_file_dnlist) < 0)
    {
      return_value = D26_ERROR;
    }
    else
    {                   /* file was closed successfully               */
	  /* set d26_file_dnlist to -1 as an indicator */
	  /* that the file is closed                   */
      d26_file_dnlist = -1;
	  /* set d26_act_isam_key to -1 as an indicator   */
	  /* that no key is set actually that is the      */
	  /* function d26_i03_set_dnlistkey wasn't called */
      d26_act_isam_key = -1;
    }
  }

  return (return_value);

}                       /* d26_i06_close_dnlistfile                   */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i07_read_dnlistrec                                */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        20.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This funcion reads one record from dn list in isam    */
/*              file using given key. The caller must set the value   */
/*              of the key in the parameter dnlistrec. If record was  */
/*              found "isrecnum" contains the record number (which    */
/*              is used to calculate the index to distingushed name   */
/*              list in core, s. define D26_I03_GET_core_INDEX).      */
/*                                                                    */
/*              When a searched record is locked the function goes    */
/*              in a loop until the record is unlocked. (So isam      */
/*              error code ELOCKED never cancels the function.)       */
/*                                                                    */
/*              When a value is longer than a key value (MAXKEYSIZE)  */
/*              the function checks whether the correct record was    */
/*              read. The function goes in a loop until the correct   */
/*              record is found or she knows that the searched record */
/*              doesn't exist in the file. (This is the case for the  */
/*              the value of dn interpretation and dn.)               */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      Short     key                   key which should be used to   */
/*                                      read the record               */
/*                  D26_I04_NO_KEY      read record with record number*/
/*                                      (isrecnum)                    */
/*                                      the caller must store the     */
/*                                      value in isrecnum             */
/*                                      for reading "version record"  */
/*                                      isrecnum = 1 (define:         */
/*                                      D26_I16_VERSION_RECNUM)       */
/*                  D26_I05_KEY_DNINTANDDN                            */
/*                                      dn interpretation and dn      */
/*                                      the caller must store the     */
/*                                      value in                      */
/*                                      &dnlistrec[D26_I10_DNINT]     */
/*                  D26_I06_KEY_OCCURRENCY                            */
/*                                      occurency (no of references   */
/*                                      to DN)                        */
/*                                      the caller must store the     */
/*                                      value in                      */
/*                                      &dnlistrec[D26_I12_OCCURRENCY]*/
/*                                                                    */
/*      Char      *dnlistrec            INPUT OUTPUT parameter        */
/*                                      space for record allocated    */
/*                                      by the caller                 */
/*                                      -> contains searched key value*/
/*                                         (s. parameter key)         */
/*                                      <- contains record values     */
/*                                         if record was found else   */
/*                                         contents undefined         */
/*                                                                    */
/*      Bool      lock                  indicator for record lock     */
/*                          D2_TRUE     the record which is returned  */
/*                                      to the caller is locked       */
/*                          D2_FALSE    the record which is returned  */
/*                                      to the caller isn't locked    */
/*                                      that is the record is read    */
/*                                      only or the caller has locked */
/*                                      the whole file which is       */
/*                                      necessary for "adding" new    */
/*                                      dnlist records                */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*    Short   D26_I13_FOUND       the searched record was found       */
/*                                dnlistrec contains the record       */
/*                                values from isam file               */
/*                                isrecnum contains the record number */
/*                                                                    */
/*            D26_I14_NOT_FOUND   the searched record doesn't exist   */
/*                                in the isam file                    */
/*                                dnlistrec could have been changed   */
/*                                isrecnum is undefined               */
/*                                                                    */
/*            D26_ERROR           any error occurred                  */
/*                                (e.g. C isam error (except ELOCKED) */
/*                                 or parameter error)                */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i07_read_dnlistrec(signed16 key, char *dnlistrec, boolean lock)

{                               /*  d26_i07_read_dnlistrec            */

static char *function = "i07: ";

signed16    return_value = D26_I13_FOUND;
int         rcode,                    /* return code of c-isam function*/
	    mode;                     /*  mode for c-isam isread fct   */

char        *ptr_dnlistrec;    /* ptr to begin of value in read record */
char        save_key [D26_I02_RECDNLISTL]; /* key: dnint and dn        */
signed16    ln_save_key;                   /* length of save_key       */

byte        normed_name[D2_DNL_MAX];  /* meomory for normed DN   */
long        rec_no;    /* saved record number                    */
			    /* counter for isread calls               */
signed32    ir_count = 0;

register boolean read_rec;    /* indicator for reading record         */

/************************/
/* initialize variables */
/************************/
switch (key)                 /*  given key                               */
{
  case D26_I04_NO_KEY:       /*  no key (key = isrecnum)                 */
			     /*  save isrecnum (because isrecnum is      */
			     /*  changed by isstart)                     */
			     /* */
     rec_no = isrecnum;
     break;

  case D26_I06_KEY_OCCURRENCY: /*  occurrency                            */
			     /*  no action                               */
			     /* */
     break;

  case D26_I05_KEY_DNINTANDDN: /*  dn interptretation and dn             */
			/*  save key value to check whether the correct    */
			/*  record was read                                */
			/*  ptr to begin of key value in read record       */
     ptr_dnlistrec   = &dnlistrec[D26_I10_DNINT];
     strcpy(save_key,ptr_dnlistrec);
			/*  calculate length of save_key                   */
     ln_save_key = strlen(save_key);
     break;

  default:                     /* error which never occurrs               */
     return (D26_ERROR);
}               /*  given key                                       */

read_rec = TRUE;     /* */

			/*  set mode for isread                            */
mode = ISEQUAL;
if (lock == TRUE)   /* record should be locked                          */
{
     mode = mode + ISLOCK;
}

/***********/
/* set key */
/***********/

if                      /*  d26_i03_set_dnlistkey (key) fails         */
   ((d26_i03_set_dnlistkey (key)) != D2_NO_ERR)
{                       /*  return error                              */
     return (D26_ERROR);
}


/**********************/
/* read wished record */
/**********************/

if (key == D26_I04_NO_KEY)   /*  no key (key = isrecnum)                 */
{                            /*  get searched isrecnum                   */
     isrecnum = rec_no;
}                            /* */

while                   /*  correct record isn't found                     */
      (read_rec == TRUE)
{                       /*  read record using isread                       */
   /***************/
   /* read record */
   /***************/
   ir_count++;
   d26_dbacc_count++;
   rcode = D26_ISREAD (svc_c_route_nolog,d26_file_dnlist, dnlistrec,
     mode);
   while                /*  record is locked                               */
	 ((rcode < 0) && (iserrno == ELOCKED))
   {                    /*  sleep 1 second                                 */
      sleep (D26_I15_SLEEP_SECS);
			/*  try to read record again                       */
      ir_count++;
      d26_dbacc_count++;
      rcode = D26_ISREAD(svc_c_route_nolog,d26_file_dnlist,dnlistrec,
	mode);
   }


   if (rcode < 0)       /*  isread error occurred                          */
   {                    /*  set return_value                          */
	read_rec = FALSE;
	switch (iserrno) /* errorcode from isread                     */
	{
	  case EENDFILE:
	  case ENOREC:     /* no such record                          */
	    return_value = D26_I14_NOT_FOUND;
	    break;
	  default:         /* other C isam error occured              */
	    dce_svc_printf(GDS_S_ISREAD_ERR_MSG,mode,iserrno);
	    return_value = D26_ERROR;
	    break;
	}

	break;          /*  leave loop !!!!!                          */

   }

   /******************************************/
   /* check whether correct record was found */
   /******************************************/
			/*  check whether the wished record was found */
   switch (key)                 /* given key                          */
   {
    case D26_I04_NO_KEY:         /* no key (key = isrecnum)           */
    case D26_I06_KEY_OCCURRENCY: /* occurrency                        */
				 /* correct record was found          */
	read_rec = FALSE;        /* don't read next record            */
	break;

    case D26_I05_KEY_DNINTANDDN: /* dn interptretation and dn         */
				/* check whether correct record was   */
				/* found                              */
	if                      /* key_value is shorter than key length  */
	   (ln_save_key < MAXKEYSIZE)
	{                       /* correct record was read            */
	     read_rec = FALSE; /* don't read next record              */
	}
	else
	{                    /* compare save_key value with record    */
			     /* value                                 */
	     if              /* value matches within key length       */
		((strncmp (save_key,ptr_dnlistrec,MAXKEYSIZE)) == 0)
	     {                    /* norm name of record              */
		d27_007_norm_name(dnlistrec + MAXKEYSIZE,
		  ln_save_key - CHARSIZE + 1,(char *)normed_name);
		  if              /* rest of value matches with record value*/
		     (strcmp(save_key + CHARSIZE,(char *)normed_name) == 0)
		  {               /* correct record is found               */
				  /* don't read next record                */
		       read_rec = FALSE;
		  }
		  else
		  {               /* read record isn't correct             */
				  /* set indicator for read next record    */
		       read_rec = TRUE;
				  /* set mode for isread                   */
		       mode = ISNEXT;

		       if              /* read record is locked            */
			  (lock == TRUE)
		       {
				       /* set mode for isread              */
			    mode = mode + ISLOCK;
				       /* unlock record                    */
			    if ((D26_ISRELEASE(svc_c_sev_warning,
				 d26_file_dnlist)) < 0)
			    {
			      D27_022_ENTER_TIME_TRACE(
				GDS_S_FCT_IS_START_READ,function,0,
				ir_count);
			      return (D26_ERROR);
			    }
			}

		  }
	     }
	     else
	     {                    /* no record with save_key value exists  */
				  /* in the isam file                      */

				  /* don't read next record                */
		  read_rec = FALSE;
				  /* set return_value record not found     */
		  return_value = D26_I14_NOT_FOUND;

		  if              /* read record is locked                 */
		     (lock == TRUE)
		  {               /* unlock record                         */
		       if ((D26_ISRELEASE(svc_c_sev_warning,d26_file_dnlist))
			   < 0)
		       {
			D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,
			  function,0,ir_count);
			return (D26_ERROR);
		       }
		  }
	     }
	}

	break;

   default:                     /* error which never occurrs               */
	D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,0,
	  ir_count);
	return (D26_ERROR);
   }


}

D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,0,ir_count);

return (return_value);

}                               /*  d26_i07_read_dnlistrec            */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE:          C FUNCTION                                          */
/*                                                                    */
/* NAME:        : d26_i08_fill_charfield                              */
/*                                                                    */
/* AUTHOR:        Schreck, D AP 11                                    */
/* DATE:          21.01.88                                            */
/*                                                                    */
/* DESCRIPTION:  This function fills "field" with the value given in  */
/*               the parameter "contents". Is "contens" shorter than  */
/*               the value given in "ln_field" the rest of "field"    */
/*               is filled with the character given in                */
/*               "fill_character". Is "contents" longer than          */
/*               "ln_field" the value is shortened to "ln_field"s     */
/*               characters.                                          */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*     String   contents                value which should be copied  */
/*                                      to field                      */
/*                                                                    */
/*     Short    ln_field                maximum number of characters  */
/*                                      which could be copied to      */
/*                                      field (= length of field)     */
/*                                                                    */
/*     Char     fill_character          character with which field is */
/*                                      filled upto ln_field when     */
/*                                      contents is shorter than      */
/*                                      ln_field                      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*     String   field                   the field which is filled with*/
/*                                      contents                      */
/*                                      the memory must be allocated  */
/*                                      by the caller and pointer     */
/*                                      to field is input for this    */
/*                                      function                      */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*     signed16   D2_NO_ERR       no error occurred                   */
/*                                                                    */
/*                D26_CHANGED     the value of contents was shortened */
/*                                to ln_field                         */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i08_fill_charfield(char *field, char *contents,
	signed16 ln_field, char fill_character)

{                                   /*  d26_i08_fill_charfield        */

signed16 return_value;

register signed16  ln_contents;     /* calculated length of contents  */

if ((ln_contents = strlen (contents)) > ln_field)
{    /* contents is longer than field ==> contents is shortened */
     strncpy (field, contents, ln_field);

     return_value = D26_CHANGED;  /* indicates that value was shortened */

}
else
{ /* contents is shorter than field            */
     /* ==> field is filled with fill_characters  */
     strcpy (field, contents);
     memset(field + ln_contents,fill_character,ln_field - ln_contents);

     return_value = D2_NO_ERR;
}

return (return_value);
}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i09_fill_dnint                                    */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        21.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function copies the given value of dn            */
/*              interpretation from_dnint to to_dnint. When the       */
/*              given value is unknown D2_SOBJ is assumed and         */
/*              copied to to_dnint.                                   */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*       D2_dnint     from_dnint         value which should be copied */
/*                                       to to_dnint                  */
/*                          D2_SOBJ      '0' = single object          */
/*                          D2_SUBTREE   '1' = root of subtree        */
/*                          D2_GROUP     '2' = group of objects       */
/*                                           (handled as subtree now, */
/*                                            for further enhancement)*/
/*                          D2_COPY      '4' = copy of other DSA      */
/*                                           DN-List-Attribute        */
/*                          otherwise    D2_SOBJ is assumed           */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*        D2_dnint     *to_dnint         copied value                 */
/*                          D2_SOBJ      '0' = single object          */
/*                          D2_SUBTREE   '1' = root of subtree        */
/*                          D2_GROUP     '2' = group of objects       */
/*                                           (handled as subtree now, */
/*                                            for further enhancement)*/
/*                          D2_COPY      '4' = copy of other DSA      */
/*                                           DN-List-Attribute        */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     Short   D2_NO_ERR     no error occured                         */
/*                                                                    */
/*             D26_CHANGED   a unkown dn interpretation was given in  */
/*                           from_dnint                               */
/*                           the wrong interpretation is logged       */
/*                           D2_SOBJ for to_dnint was assumed         */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i09_fill_dnint(D2_dnint from_dnint, D2_dnint *to_dnint)

{                               /*  d26_i09_fill_dnint                   */

signed16 return_value;

switch (from_dnint)
{
  case D2_SOBJ:
  case D2_SUBTREE:
  case D2_GROUP:
  case D2_COPY :
     /* take given value */
     *to_dnint = from_dnint;
     return_value = D2_NO_ERR;
     break;

  default:
     /* error: unknown dn interpretation arrived            */
     /* assume single object */
     *to_dnint = D2_SOBJ;
     return_value = D26_CHANGED;
     break;
}

return (return_value);

}                               /*  d26_i09_fill_dnint                */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i10_lock_isamfile                                 */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        22.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function locks an isamfile. (lock with wait, s.  */
/*              NOTE)                                                 */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*        Fildes       isfd          filedescriptor of isam file which*/
/*                                   should be locked                 */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      signed16 D2_NO_ERR      no error occured                      */
/*                                                                    */
/*              D26_ERROR       C isam error occured  (except ELOCKED)*/
/*                              (s. NOTE)                             */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_i10_lock_isamfile(int isfd)

{                               /*  d26_i10_lock_isamfile             */

signed16 return_value = D2_NO_ERR;

register int rcode;    /* return code of C isam function              */

				    /*  try to lock isam file         */
rcode = D26_ISLOCK(svc_c_route_nolog,isfd);
while                            /*  file is locked                   */
      ((rcode < 0) && (iserrno == ELOCKED))
{                                /*                                   */
   sleep (D26_I15_SLEEP_SECS);   /*  sleep 1 second                   */
				    /*  try to lock isamfile again    */
   rcode = D26_ISLOCK(svc_c_route_nolog,isfd);
}

if (rcode < 0)                   /*  islock error occurred            */
{                                /*  return error                     */
    return_value = D26_ERROR;
}

return (return_value);

}                                /*  d26_i10_lock_isamfile            */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i11_rep_dnlistrec                                 */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        22.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function searches the records with the old dn    */
/*              and all possible dn interpretation vaules. When       */
/*              record(s) exists the old dn is replaced by the new dn */
/*              and the record is rewrited into the file.             */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*    D2_name_string      old_dn       the normed value of the old dn */
/*                                                                    */
/*    D2_name_string      new_dn       the exact value of the new dn  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*    Bool                changed         indicator whether isam file */
/*                                        was changed                 */
/*                               D2_TRUE  isam file was changed       */
/*                               D2_FALSE isam file wasn't changed    */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*    signed16 D2_NO_ERR         no error occurred                    */
/*                                                                    */
/*            D26_ERROR          C isam error occurred                */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_i11_rep_dnlistrec(D2_name_string old_dn,
    D2_name_string new_dn, boolean *changed)

{                               /*  d26_i11_rep_dnlistrec             */

static char *function = "d26_i11_rep_dnlistrec";
			   /* possible values of dn interpretation         */
static D2_dnint    dnint_values[D26_I17_NO_DNINT_VAL] =
				 { D2_SOBJ,
				   D2_SUBTREE,
				   D2_GROUP };

char      dnlistrec[D26_I02_RECDNLISTL]; /* record buffer of isam file*/
byte      normed_new_dn[D2_DNL_MAX];

char      *dnintptr,         /* ptr to dn interpretation              */
	       *dnptr,            /* ptr to normed DN                      */
	       *deptr;            /* ptr to exact DN                       */
signed16  return_value = D2_NO_ERR;

register signed16 rcode,          /* return_value of read function         */
		  i;              /* counter                               */

/*******************/
/* lock whole file */
/*******************/

if                                /*  lock of isam file fails         */
  (d26_i10_lock_isamfile(d26_file_dnlist) == D26_ERROR)
{                                 /*  return error                    */
     return (D26_ERROR);
}

/************************/
/* initialize variables */
/************************/

dnintptr = &dnlistrec[D26_I10_DNINT];
dnptr    = &dnlistrec[D26_I11_DNNAME];
deptr    = &dnlistrec[MAXKEYSIZE];
*changed = FALSE;

/*********************************/
/* search old records and update */
/*********************************/

for                        /*  all possible dn int. values                 */
    (i=0; ((i<D26_I17_NO_DNINT_VAL) && (return_value != D26_ERROR)); i++)
{                          /*  search record with old dn                   */
			   /*  fill key value dnint and dn                 */
   d26_i09_fill_dnint (dnint_values[i], dnintptr); /*  dn interpretation   */
						   /*  119 bytes dn name   */
   d26_i08_fill_charfield (dnptr,(char *)old_dn,D2_DNL_MAX,D2_EOS);

			   /*  search record without lock                  */
   rcode = d26_i07_read_dnlistrec (D26_I05_KEY_DNINTANDDN,dnlistrec,
				   FALSE);


   switch (rcode)          /*  return value of read dnlistrec              */
   {
    case D26_I13_FOUND:    /*  record was found                            */
			   /*  update record value: new normed dn name     */
	d27_007_norm_name((char *)new_dn,strlen((char *)new_dn) + 1,
	  (char *)normed_new_dn);
	d26_i08_fill_charfield(dnptr,(char *)normed_new_dn,
	  MAXKEYSIZE - CHARSIZE,D2_EOS);
	d26_i08_fill_charfield(deptr,(char *)new_dn,D2_DNL_MAX,D2_EOS);
			   /*  rewrite record                              */
	if(D26_ISREWCURR(svc_c_sev_warning,d26_file_dnlist,dnlistrec) < 0)
	{                  /* isam error occurred                          */
	     return_value = D26_ERROR; /* set return value                 */
	}
	else
	{                  /* set indicator for isam file changed          */
	     *changed = TRUE;
	}                  /* isam error occured                           */
			   /*  unlock record                               */
	break;

   case D26_I14_NOT_FOUND: /*  no record found                             */
			   /*  no action                                   */
	break;

   default:                /* C isam error occured                         */
	return_value = D26_ERROR; /* set return_value                      */
	break;
   }


}

#ifndef MKUPD
if                      /*  New value of DNList-Attribute        */
  (d26_ulogging == TRUE && return_value == D2_NO_ERR &&
   *changed == TRUE)
{                       /*  set write-logging-flag               */
  d26_m21_logg_dnlist(D2_REP_AV,old_dn,new_dn);
}                       /*                                       */
#endif

if ((D26_ISUNLOCK(svc_c_sev_warning,d26_file_dnlist)) < 0)
{                  /* isam error occurred                          */
     return_value = D26_ERROR; /* set return value                 */

}                  /* isam error occured                           */

D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_WRITE,function,1,0);

return (return_value);     /* */

}                               /*  d26_i11_rep_dnlistrec             */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i12_add_dnlistrec                                 */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        25.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function adds a new record in isam file of dnlist*/
/*              by increasing the occurrency of the existing record   */
/*              or by using a record with occurrency 0 or by          */
/*              writing a new record.                                 */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      D2_dnint          dnint            interpretation of dn       */
/*                                         ignored when dnlistcoreidx */
/*                                         >= 0                       */
/*                                                                    */
/*      D2_name_string    dn               exact distinguished name   */
/*                                         ignored when dnlistcoreidx */
/*                                         >= 0                       */
/*                                                                    */
/*      Long              occurrency       no of references to dn     */
/*                                         increases old occurency    */
/*                                         from file                  */
/*                                                                    */
/*      Long              *dnlistcoreidx   INPUT OUTPUT parameter:    */
/*                          D26_IDX_NOT_DEF it is unknown whether a   */
/*                                          record with dn and dnint  */
/*                                          exists                    */
/*                                          the record is read via    */
/*                                          dnint and dn              */
/*                                          the index to dnlist in    */
/*                                          core is returned (--->    */
/*                                          OUTPUT parameter)         */
/*                             >= 0         the record exists in the  */
/*                                          isam file and is read via */
/*                                          isrecnum: = *dnlist_core_ */
/*                                          idx + D26_I03_GET_CORE_IDX*/
/*                                          new_dn and dnint are      */
/*                                          ignored                   */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*    Short   D2_NO_ERR          no error occurred                    */
/*                                                                    */
/*            D26_ERROR          C isam error occurred                */
/*                               or max. value for dnl. index exceeded*/
/*                               (== D26_MAX_DNLIDX)                  */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_i12_add_dnlistrec(D2_dnint dnint, D2_name_string dn,
		  signed32 occurrency, signed32 *dnlistcoreidx)

{                               /*  d26_i12_add_dnlistrec             */

static char *function = "d26_i12_add_dnlistrec";

			   /* record buffer of isam file                   */
char      dnlistrec[D26_I02_RECDNLISTL];
byte      normed_new_dn[D2_DNL_MAX];

char      *dnintptr,         /* ptr to dn interpretation              */
	       *dnptr,            /* ptr to normed DN                      */
	       *deptr,            /* ptr to exact DN                       */
	       *ptroccurrency;    /* ptr to occurrency                     */
signed16  return_value = D2_NO_ERR;
boolean   rewrite = FALSE, /* indicator for rewrite record         */
	  found = FALSE;   /* indicator for record exists          */
signed32  new_occurrency;
signed32  max_short;          /* variable for max. index check        */
boolean   wr_mess = FALSE; /* indicates, whether delta-update-     */
				   /* message has to be written            */

register signed16  rcode;        /* return code of called functions        */
register long      i;

/*******************/
/* lock whole file */
/*******************/

if                                /*  lock of isam file fails         */
  (d26_i10_lock_isamfile (d26_file_dnlist) == D26_ERROR)
{                                 /*  return error                    */
     return (D26_ERROR);
}

/************************/
/* initialize variables */
/************************/

dnintptr = &dnlistrec[D26_I10_DNINT];
dnptr    = &dnlistrec[D26_I11_DNNAME];
deptr    = &dnlistrec[MAXKEYSIZE];
ptroccurrency = &dnlistrec[D26_I12_OCCURRENCY];
new_occurrency = (signed32) abs ((int)occurrency);
d27_007_norm_name((char *)dn,strlen((char *)dn) + 1,(char *)normed_new_dn);

/***************/
/* read record */
/***************/

if (*dnlistcoreidx >= 0)    /*  record exists                         */
{                                 /*  read record via isrecnum             */
     isrecnum = *dnlistcoreidx + D26_I03_GET_CORE_INDEX; /*  set isrecnum  */
				  /*  read record                          */
     rcode = d26_i07_read_dnlistrec (D26_I04_NO_KEY,
				     dnlistrec,
				     FALSE); /* no lock because file is */
						/* locked                  */

     switch (rcode)               /*  return code from read                */
     {
      case D26_I13_FOUND:         /*  record was found                     */
				  /*  set indicator for rewrite and found  */
	  found = rewrite = TRUE;
	  break;
      case D26_I14_NOT_FOUND:     /*  record wasn't found                  */
      default:                    /*  C isam error occurred                */
				  /*  set return_value                     */
	  return_value = D26_ERROR;
	  break;
     }                            /*  return code from read */
}
else
{                                 /*  unknown whether record exists        */
				  /*  try to read record via given dnint   */
				  /*  and dn                               */
				  /*  fill key value dnint and dn          */
     d26_i09_fill_dnint (dnint, dnintptr); /*  dn interpretation           */
					   /*  dn name                     */
     d26_i08_fill_charfield (dnptr, (char *)normed_new_dn, D2_DNL_MAX,
			     D2_EOS);
				  /*  read record                          */
     rcode = d26_i07_read_dnlistrec (D26_I05_KEY_DNINTANDDN,dnlistrec,
				     FALSE); /* no lock because file is */
						/* locked                  */

     switch (rcode)               /*  return code from read                */
     {
      case D26_I13_FOUND:         /*  record was found                     */
				  /*  set indicator for rewrite and found  */
	  found = rewrite = TRUE;
	  break;
      case D26_I14_NOT_FOUND:     /*  record wasn't found                  */
				  /*  set indicator for record doesn't     */
				  /*  exist (found)                        */
	  found = FALSE;
				  /*  try to read record with occurrency   */
				  /*  = 0                                  */
				  /*  set key value                        */
	  stlong(0L, ptroccurrency);
				  /*  read record                          */
	  rcode = d26_i07_read_dnlistrec (D26_I06_KEY_OCCURRENCY,
					  dnlistrec,
					  FALSE); /* no lock because    */
						     /* file is locked     */

	  switch (rcode)          /*  return code from read                */
	  {
	    case D26_I13_FOUND:   /*  record was found                     */
				  /*  set indicator for rewrite            */
	       rewrite = TRUE;
				  /* */
	       break;
	    case D26_I14_NOT_FOUND: /*  record wasn't found           */
	       rewrite = FALSE;
	       break;
	    default:              /*  C isam error occurred                */
	       return_value = D26_ERROR;
	       break;
	  }
	  break;

      default:                    /*  C isam error occurred                */
	  return_value = D26_ERROR;
	  break;
     }

}

if (return_value == D2_NO_ERR)    /*  no error occurred                    */
{                                 /*  fill in new record values            */

     /*****************************/
     /* fill in new record values */
     /*****************************/

     if (found == TRUE)        /*  record with given dnint and dn exists*/
     {                            /*  set new occurrency                   */
	  i = ldlong(ptroccurrency);
	  new_occurrency = new_occurrency + i;
	  if                      /*  New value of DNList-Attribute        */
	    (d26_ulogging == TRUE && dnint != D2_COPY && i == 0)
	  {                       /*  set write-logging-flag               */
	    wr_mess = TRUE;
	  }                       /*                                       */
     }
     else
     {                            /*  record with given dnint and dn       */
				  /*  doesn't exist                        */
				  /*  fill in dnint                        */
	  d26_i09_fill_dnint (dnint, dnintptr);
				  /*  fill in normed dnname                */
	  d26_i08_fill_charfield(dnptr,(char *)normed_new_dn,
	    MAXKEYSIZE - CHARSIZE,D2_EOS);
	  d26_i08_fill_charfield(deptr,(char *)dn,D2_DNL_MAX,D2_EOS);
	  if                      /*  New value of DNList-Attribute        */
	    (d26_ulogging == TRUE && dnint != D2_COPY)
	  {                       /*  set write-logging-flag               */
	    wr_mess = TRUE;
	  }
     }

				  /*  fill in new occurrency               */
     stlong(new_occurrency, ptroccurrency);


     /*****************************/
     /* write record in isam file */
     /*****************************/

     if (rewrite == TRUE)      /*  rewrite record                       */
     {                            /*  rewrite record                       */
	  if ((D26_ISREWCURR(svc_c_sev_warning,d26_file_dnlist, dnlistrec))
	      < 0)
	  {                       /* isam error occurred                   */
	       return_value = D26_ERROR; /* set return value               */

	  }                       /* isam error occured                    */
     }
     else
     {                            /*  write record                         */
	  if ((D26_ISWRITE(svc_c_sev_warning,d26_file_dnlist,dnlistrec)) < 0)
	  {                       /* isam error occurred                   */
	       return_value = D26_ERROR; /* set return value               */

	  }                       /* isam error occured                    */
     }                            /*  rewrite record */
     D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_WRITE,function,1,0);

     /*******************************/
     /* set index to dnlist in core */
     /*******************************/

     if (return_value == D2_NO_ERR)
     {
	  if                      /*  maximum index exceeded               */
	     ((max_short = isrecnum - D26_I03_GET_CORE_INDEX)
		  > D26_MAX_DNLIDX)
	  {                       /*  error log                            */
	    dce_svc_printf(GDS_S_MAX_DN_INDEX_MSG);
	    return_value = D26_ERROR; /*  set return_value              */
	  }
	  else
	  {                       /*  set index to dnlist in core          */
				  /*  (output parameter)                   */
	       *dnlistcoreidx = max_short;
	  }
     }
}

#ifndef MKUPD
if                                 /*  logging message must be written*/
  (return_value == D2_NO_ERR && wr_mess == TRUE)
{                                  /*  write logging message          */
  switch(d26_i28_dn_otherint_exists(dnint,normed_new_dn))
  {
    case FALSE:
      switch(d26_i29_otherval_exists(normed_new_dn))
      {
	case FALSE:
	  d26_m21_logg_dnlist(D2_ADDATT,NULL,dn);
	  break;
	case TRUE:
	  d26_m21_logg_dnlist(D2_ADD_AV,NULL,dn);
	  break;
	default:
	  return_value = D26_ERROR;
	  break;
      }
      break;
    case TRUE:
      break;
    default:
      return_value = D26_ERROR;
      break;
  }
}
#endif

/********************/
/* unlock isam file */
/********************/
				    /* unlock of isam file fails      */
if (D26_ISUNLOCK(svc_c_sev_warning,d26_file_dnlist) < 0)
{                                  /*  return error                   */
     return_value = D26_ERROR;     /*  set return_value               */
}

return (return_value);

}                               /*  d26_i12_add_dnlistrec             */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i13_del_dnlistrec                                 */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        25.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function "deletes" a record in isam file of      */
/*              dnlist by decreasing the occurrency value in record   */
/*              by the given occurrency.                              */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*    Long          dnlistcoreidx         index to dnlist in core     */
/*                           >= 0         the recordnumber "isrecnum" */
/*                                        is calculated by            */
/*                                        dnlistcoreidx +             */
/*                                        D26_I03_GET_CORE_IDX        */
/*                                                                    */
/*    Long          occurrency            the value by which the      */
/*                                        record value must be        */
/*                                        decreased                   */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*    signed16 D2_NO_ERR         no error occurred                    */
/*                                                                    */
/*            D26_ERROR          C isam error occurred                */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_i13_del_dnlistrec(signed32 dnlistcoreidx,
    signed32 occurrency)

{                               /*  d26_i13_del_dnlistrec             */

static char *function = "i13";

			   /* record buffer of isam file                   */
char      dnlistrec[D26_I02_RECDNLISTL];

D2_dnint           dnint;
				  /* normed DN                        */
byte      normed_dn[D2_DNL_MAX];
char      *ptroccurrency;    /* ptr to dn occurrency                  */
signed16  return_value = D2_NO_ERR;
signed32  abs_occurrency,    /* absolute value of given occurrency    */
	  new_occurrency,    /* new value of occurrency               */
	  old_occurrency;    /* old occurrency value                  */

register signed16 rcode;     /* return_value of read function         */

/*******************/
/* lock whole file */
/*******************/

if                                /*  Delta update is running              */
  (d26_ulogging == TRUE)
{                                 /*  lock whole file                      */
  if                              /*  lock of isam file fails              */
    (d26_i10_lock_isamfile (d26_file_dnlist) == D26_ERROR)
  {                               /*  return error                         */
     return (D26_ERROR);
  }
}

/************************/
/* initialize variables */
/************************/

ptroccurrency = &dnlistrec[D26_I12_OCCURRENCY];
abs_occurrency = (signed32)abs((int)occurrency);

/*******************/
/* read old record */
/*******************/

if (dnlistcoreidx >= 0)      /*  correct index given                      */
{                            /*  read record with lock via isrecnum        */
     isrecnum = dnlistcoreidx + D26_I03_GET_CORE_INDEX;  /*  set isrecnum  */
			     /*  read record                               */
     rcode = d26_i07_read_dnlistrec(D26_I04_NO_KEY,dnlistrec,!d26_ulogging);

     switch (rcode)          /*  return value of read dnlistrec            */
     {
      case D26_I13_FOUND:    /*  record was found                          */
			     /*  update record value: new occurrency       */
			     /*  get old occurrency value                  */
	  old_occurrency = ldlong(ptroccurrency);
			     /*  set new occurrency value                  */
	  new_occurrency = old_occurrency - abs_occurrency;
	  if (new_occurrency < 0)
	  {
	       new_occurrency = 0;
	  }

			     /*  update record value                       */
	  stlong(new_occurrency, ptroccurrency);

			     /*  rewrite record                            */
	  if ((D26_ISREWCURR(svc_c_sev_warning,d26_file_dnlist, dnlistrec))
	      < 0)
	  {                  /* isam error occurred                        */
	       return_value = D26_ERROR; /* set return value               */
	  }                  /* isam error occured                         */
			     /*  unlock record                             */
	  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_WRITE,function,1,0);


	  if                 /*  Delta update is not running               */
	    (d26_ulogging == FALSE)
	  {                  /*  release record                            */
	    if ((D26_ISRELEASE(svc_c_sev_warning,d26_file_dnlist)) < 0)
	    {                /* isam error occurred                        */
	       return_value = D26_ERROR; /* set return value               */
	    }                /* isam error occured                         */
	  }
	  else if            /* no error occurred and record is unvalid now*/
		 (return_value == D2_NO_ERR && new_occurrency == 0)
	  {                 /* check whether value was deleted             */
	    dnint = (D2_dnint)dnlistrec[D26_I10_DNINT];
	    d27_007_norm_name(dnlistrec + MAXKEYSIZE,
	      strlen(dnlistrec + MAXKEYSIZE),(char *)normed_dn);
	    if              /* value belongs to own DSA                    */
	      (dnint != D2_COPY)
	    {               /* check whether value still exists            */
#ifndef MKUPD
	      switch(d26_i28_dn_otherint_exists(dnint,normed_dn))
	      {
		case FALSE:
		  switch(d26_i29_otherval_exists(NULL))
		  {
		    case FALSE:
		      d26_m21_logg_dnlist(D2_DELATT,NULL,NULL);
		      break;
		    case TRUE:
		      d26_m21_logg_dnlist(D2_DEL_AV,normed_dn,NULL);
		      break;
		    default:
		      return_value = D26_ERROR;
		      break;
		  }
		  break;
		case TRUE:
		  break;
		default:
		  return_value = D26_ERROR;
		  break;
	      }
#endif
	    }                     /*                                       */
	  }                       /*                                       */
			     /*  */
	  break;

      case D26_I14_NOT_FOUND:/*  no record found                           */
			     /*  error: data inconsistency of dnlis in isam*/
			     /*  set return_value                          */
	  return_value = D26_ERROR;
			     /*  */
	  break;

      default:               /* C isam error occured                       */
	  return_value = D26_ERROR; /* set return_value                    */
	  break;
     }
}
else
{                          /*  parameter error                             */
     return_value = D26_ERROR; /*  set return_value                        */
}                          /*  dnlistcoreidx >= D2_NULL    */


/********************/
/* unlock isam file */
/********************/
if                                /*  Delta update is running              */
  (d26_ulogging == TRUE)
{                                 /*  unlock whole file                    */
				   /*  unlock of isam file fails           */
  if(D26_ISUNLOCK(svc_c_sev_warning,d26_file_dnlist) < 0)
  {                                /*  set return_value                    */
     return_value = D26_ERROR;
  }
}

return (return_value);

}                               /*  d26_i13_del_dnlistrec             */

#ifndef MKUPD
/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i28_dn_otherint_exists                            */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        20.11.89                                              */
/*                                                                    */
/* DESCRIPTION: This function checks, whether there is a record in the*/
/*              distinguished namelist file containing the DN dn and  */
/*              an interpretation, that is different from dnint and   */
/*              from D2_COPY with occurrency > 0.                     */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      D2_dnint          dnint            interpretation of dn       */
/*                                                                    */
/*      D2_name_string    dn               distinguished name         */
/*                                                                    */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*    Short   D2_TRUE            record with other interpretation     */
/*                               exists.                              */
/*            D2_FALSE           record doesn't exist.                */
/*            D26_ERROR          C-Isam-Error occurred                */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_i28_dn_otherint_exists(D2_dnint dnint, D2_name_string dn)

{                               /*  d26_i28_dn_otherint_exists        */

			   /* record buffer of isam file                   */
char      dnlistrec[D26_I02_RECDNLISTL];

register signed16  tmpint;      /*  different interpretation          */

for                              /*  all other interpretations      */
   (tmpint = D2_SOBJ; tmpint < D2_IGNORE; tmpint++)
{                                /*  check interpretation           */
  if                             /*  interpretation is different    */
    ((D2_dnint)tmpint != dnint)
  {                              /*  read DNlist-record             */
				 /*  dn interpretation into record  */
    d26_i09_fill_dnint(tmpint,dnlistrec + D26_I10_DNINT);
				/*  dn name into record            */
    d26_i08_fill_charfield(dnlistrec + D26_I11_DNNAME,(char *)dn,D2_DNL_MAX,
      D2_EOS);
				 /* read record                          */
    switch(d26_i07_read_dnlistrec(D26_I05_KEY_DNINTANDDN,dnlistrec,
				   FALSE)) /* no lock because no      */
					       /* update intended        */
    {
      case D26_I13_FOUND:
	if(ldlong(dnlistrec + D26_I12_OCCURRENCY) > 0)
	{
	  return(TRUE);
	}
	/* flow into not found case otherwise */
      case D26_I14_NOT_FOUND:
	break;
      default:
	return(D26_ERROR);
    }
  }
}

return(FALSE);

}                               /*  d26_i28_dn_otherint_exists        */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i29_otherval_exists()                             */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        20.11.89                                              */
/*                                                                    */
/* DESCRIPTION: This function checks, whether there is a record in the*/
/*              distinguished namelist file containing a DN-List-value*/
/*              different from dn. If DN is NULL, it checks, whether  */
/*              there is a value at all.                              */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      D2_name_string    dn               distinguished name         */
/*                                                                    */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*    signed16 D2_TRUE           other value exists.                  */
/*            D2_FALSE           other value doesn't exist.           */
/*            D26_ERROR          C-Isam-Error occurred                */
/*                                                                    */
/*inoff ***************************************************************/

static signed16 d26_i29_otherval_exists(D2_name_string dn)

{                               /*  d26_i29_otherval_exists           */

static char *function = "i29";

			    /* record buffer of isam file             */
char      dnlistrec[D26_I02_RECDNLISTL];
			    /* memory for normed name                 */
byte      normed_name[D2_DNL_MAX];
int       mode;             /* ISAM-Lesemodus                         */
			    /* counter for isread calls               */
signed32  ir_count = 0;

if                      /*  d26_i03_set_dnlistkey  fails              */
   ((d26_i03_set_dnlistkey (D26_I06_KEY_OCCURRENCY)) != D2_NO_ERR)
{                       /*  return error                              */
  return (D26_ERROR);
}
else
{                       /*  write 1 into occurrency                   */
  stlong(1L,dnlistrec + D26_I12_OCCURRENCY);
  for                   /*  all valid records                         */
     (mode = ISGTEQ; ; mode = ISNEXT)
  {                     /*  read record                               */
    ir_count++;
    d26_dbacc_count++;
    if                  /*  error from reading record                 */
      (D26_ISREAD(svc_c_route_nolog,d26_file_dnlist,dnlistrec,mode) < 0)
    {                   /*  check error                               */
      switch(iserrno)
      {
	case ELOCKED:
	  sleep(D26_I15_SLEEP_SECS);
	  continue;
	case ENOREC:
	case EENDFILE:
	  return(FALSE);
	default:
	  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,0,
	    ir_count);
	  return(D26_ERROR);
      }
    }
    else
    {                   /*  check interpretation                      */
      if                /*  DN isn't COPY                             */
	(dnlistrec[D26_I10_DNINT] != D2_COPY)
      {                 /*  check DN                                  */
	if              /*  DN is given (from add)                    */
	  (dn != NULL)
	{               /*  compare DNs                               */
	  d27_007_norm_name(dnlistrec + MAXKEYSIZE,
	    strlen(dnlistrec + MAXKEYSIZE) + 1,(char *)normed_name);
	  if            /*  DNs are different                         */
	    (strcmp((char *)dn,(char *)normed_name))
	  {             /*  different value is found                  */
	    D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,0,
	      ir_count);
	    return(TRUE);
	  }             /*                                            */
	}
	else
	{               /*  value is found                            */
	  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_START_READ,function,0,
	    ir_count);
	  return(TRUE);
	}
      }
    }
  }
}

}                               /*  d26_i29_otherval_exists           */
#endif
