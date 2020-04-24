/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26tutil.c,v $
 * Revision 1.1.12.2  1996/02/18  19:46:18  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:27:44  marty]
 *
 * Revision 1.1.12.1  1995/12/08  15:48:56  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:08:10  root]
 * 
 * Revision 1.1.10.4  1994/07/06  15:07:12  marrek
 * 	July 1994 code submission.
 * 	[1994/07/04  13:14:58  marrek]
 * 
 * Revision 1.1.10.3  1994/06/21  14:46:02  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:35:32  marrek]
 * 
 * Revision 1.1.10.2  1994/05/10  15:52:53  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:25:52  marrek]
 * 
 * Revision 1.1.10.1  1994/02/22  18:39:42  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:20:55  marrek]
 * 
 * Revision 1.1.8.1  1993/10/29  15:11:16  keutel
 * 	CR 9261
 * 	[1993/10/29  14:08:25  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  15:34:21  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:20:32  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  14:54:25  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.4  1993/02/02  11:29:56  marrek
 * 	Introduce D2_IA5
 * 	[1993/02/01  15:16:48  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:42:38  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:21:25  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  20:24:38  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:18:35  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:52:59  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:58:26  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char d26tutil_rcsid[] = "@(#)$RCSfile: d26tutil.c,v $ $Revision: 1.1.12.2 $ $Date: 1996/02/18 19:46:18 $";
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
/* NAME         : d26tutil.c                                          */
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
/*                     Module-identification                          */
/*                                                                    */
/**********************************************************************/


/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

#include <stdio.h>
#include <malloc.h>
#include <isam.h>

/*****  external Includes    *****/

/*****  internal Includes    *****/

#include <dce/d27util.h>
#include <malgcfe.h>

#include <d26isam.h>
#include <d26svcis.h>
#include <d26dnlis.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

#define D26_I00_NO_MEM_AREAS     10   /* maximum number of areas for       */
				      /* distinguished name strings        */
#define D26_I01_MEM_AREA_SIZE  2*8196 /* size of one area for distinguished*/
				      /* name strings                      */

#define D26_NO_SCHEME       2           /* no. of schemas (old + new) */
#define D26_OLD             0           /* identifier of old scheme   */
#define D26_NEW             1           /* identifier of new scheme   */

				/* max key part size for ISAM indices */
#define MAX_PART_SIZE (MAXKEYSIZE - LONGSIZE - INTSIZE)
				/* length of ISAM record header       */
#define ISAM_HDR      (INTSIZE + LONGSIZE)

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
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
/*                 D E C L A R A T I O N S        DATA                */
/*                                                                    */
/**********************************************************************/

/******** IMPORT ********/
					/* Distinguished-Name-list in core*/
extern D26_dndesc *d26_dnlist[D26_NO_SCHEME];    
					/* name of directory of database  */
extern char       d26_install[][D27_LFILE_NAME];
extern D26_srt    *d26_srt[D26_NO_SCHEME];   /* Structure Rule Table      */
extern signed32   d26_nosrt[D26_NO_SCHEME];  /* number of entries in SRT  */
extern D26_at     *d26_at[D26_NO_SCHEME];    /* Attribute Description Table  */
extern signed32   d26_noat[D26_NO_SCHEME];   /* maximum number of attributes */
					     /* in AT                        */
/******** LOCAL  ********/
/* a part of this variables must be exported when this module will be */
/* splitted                                                           */

static  String   d26_name_dnlist = "/dnlist";
				    /* name of file for dnlist             */

				    /* Begin of memory areas for the       */
				    /* distinguished name strings          */
static  String   d26_mem_areas[D26_NO_SCHEME][D26_I00_NO_MEM_AREAS] =
		      { { NULL, NULL, NULL, NULL, NULL,
			  NULL, NULL, NULL, NULL, NULL },
			{ NULL, NULL, NULL, NULL, NULL,
			  NULL, NULL, NULL, NULL, NULL } };

static signed32  d26t_maxidx_dnlist[D26_NO_SCHEME] = {-1,-1};
				    /* maximum index of               */
				    /* DN list in core                */
static  D26_acc_point *d26t_accp_list[D26_NO_SCHEME] =
       {(D26_acc_point *) NULL, (D26_acc_point *) NULL};
				    /* list of access points          */
static  D26_update_info d26t_upd_info[D26_NO_SCHEME] =
       {{ 0L, '0' }, { 0L, '0' }};  /* date of last change of DN list */
static signed32  d26_file_dnlist[D26_NO_SCHEME] = 
	{ (signed32)-1, (signed32)-1 };
				    /* filedescriptor for dnlist in file   */
static  signed16 d26_act_isam_key[D26_NO_SCHEME] = 
	{(signed16)-1, (signed16)-1 };
				    /* last key set by the function        */
				    /* d26_t23_set_dnlistkey               */

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/******** IMPORT ********/
extern int isaddindex(int, struct keydesc *);
extern int isdelindex(int, struct keydesc *);

/******** EXPORT ********/
signed16 d26_t21_upd_core_dnlist(signed32 sch);
signed16 d26_t22_upd_file_dnlist (D2_attr_mod type, D2_name_string old_dn,
	D2_dnint dnint, D2_name_string new_dn, signed32 occurrency,
	boolean new_version, signed32 *dnlist_core_idx, signed32 sch);
signed16 d26_t23_set_dnlistkey (signed16 key, signed32 sch);
signed16 d26_t24_get_updinfo (D26_update_info *upd_info, signed32 sch);
signed16 d26_t25_open_dnlistfile(signed32 sch);
signed16 d26_t26_close_dnlistfile(signed32 sch);
signed16 d26_t34_attr_search (signed32 sch, D2_a_type *attr_typ);
void d26_t35_select_key (signed32 sch, D26_keydesc *key, signed16 pos,
  	signed16 at_idx, signed16 filenr);
void  d26_t37_get_dninfo (String record, D26_dninfo *dninfo,
	D2_name_string *dname, signed16 np_count);
signed16 d26_t38_get_name (signed32 sch, D2_name_string dn, 
	D26_dninfo *dninfo);
signed16 d26_t42_add_index(signed16 at_idx, signed16 fnr, int isfd, 
	boolean add_idx);

/******** LOCAL  ********/

signed16 d26_t27_read_dnlistrec (signed16 key, char *dnlistrec, boolean lock,
	signed32 sch);
signed16 d26_t28_fill_charfield (char *field, char *contents,
	signed16 ln_field, char fill_character);
signed16 d26_t29_fill_dnint(D2_dnint from_dnint, D2_dnint *to_dnint);
signed16 d26_t30_lock_isamfile (int isfd );
signed16 d26_t31_rep_dnlistrec(D2_name_string old_dn, D2_name_string new_dn,
	boolean *changed, signed32 sch);
signed16 d26_t32_add_dnlistrec (D2_dnint dnint, D2_name_string dn,
	signed32 occurrency, signed32 *dnlistcoreidx, signed32 sch);
signed16 d26_t33_del_dnlistrec (signed32 dnlistcoreidx, signed32 occurrency,
	signed32 sch);
static signed16 d26_t43_read_dnlist(signed32 sch);
static signed16 d26_t39_syntax_check (D26_at *at, char *val1, 
	signed16 length);
static signed16 d26_t40_rdn_search (signed32 sch, D26_rdninfo *rdn,
	signed16 par_idx);
static signed16 d26_t41_setup_attr_len(D26_at *, Attr_info *);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t43_read_dnlist                                   */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        07.01.88                                              */
/*                                                                    */
/* SYNTAX:      EXPORT Short d26_t43_read_dnlist (sch)                */
/*              Int         sch;                                      */
/*                                                                    */
/* DESCRIPTION: This function reads the distinguished name list       */
/*              entries from ISAM file and stores them in core        */
/*              (s. structure D26_dndesc).                            */
/*                                                                    */
/*              Memory for referral list will be allocated if         */
/*              necessary (d26_mklist).                               */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      Short   D2_NO_ERR      successfull                            */
/*                             when max. value for d26t_maxidx_dnlist[]*/
/*                             (== D26_MAX_DNLIDX) is exceeded        */
/*                             D2_NO_ERR is returned after            */
/*                             D26_MAX_DNLIDX values were read        */
/*                                                                    */
/*              D26_ERROR      any error occurred                     */
/*                             (C-ISAM error or not enough memory     */
/*                              available)                            */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*    String   d26_name_dnlist    name of isam file for DN list       */
/*                                (this variable is global only for   */
/*                                 module)                            */
/*                                                                    */
/*    Fildes   d26_file_dnlist[]  file descriptor of isam file of     */
/*                                of dnlist                           */
/*                                (global only for this module)       */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*    Long       d26t_maxidx_dnlist[]  maximum index of dnlist in core */
/*                                                                    */
/*    D26_acc_point *d26t_accp_list[]  memory for access points        */
/*    D26_dndesc *d26_dnlist[]        pointer to dnlist in core       */
/*                                                                    */
/*    String     d26_mem_areas[...]   pointer to the memory areas for */
/*                                    the dnnames in core             */
/*                                    (global only for this module)   */
/*                                                                    */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        isread                                                      */
/*        ldlong                                                      */
/*        strlen                                                      */
/*        free                                                        */
/*        malloc                                                      */
/*        calloc                                                      */
/*        realloc                                                     */
/*        sprintf                                                     */
/*                                                                    */
/*        d26_t23_set_dnlistkey                                       */
/*                                                                    */
/*        d27_003_error_trace                                         */
/*                                                                    */
/*        MA2_002_ERROR                                               */
/*                                                                    */
/*                                                                    */
/* NOTES: There is no alignement for the memory areas for the dnlist  */
/*        in core and the distinguished name strings in their memory  */
/*        areas.                                                      */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_t43_read_dnlist (signed32 sch)

{                               /*  d26_t43_read_dnlist               */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

/* STATIC    ...    */
static String    function = "d26_t43_read_dnlist";

 char dnlist_rec[D26_I02_RECDNLISTL];
				  /* record of dn list file           */

 signed16 return_value = D2_NO_ERR;

/* variables for memory management */
 signed16  next_idx_memareas,/* next index of d26_mem_areas            */
	    dn_length;       /* length of exact distinguished name     */
 String begin_memarea,       /* pointer to begin of memarea            */
	    end_memarea,     /* pointer to end of memarea              */
	    next_dn_memory;  /* pointer to begin of next dn            */
 boolean   new_memory;       /* D2_TRUE: a new memarea must be         */
			     /*          allocated                     */


 D26_dndesc *ptr_dnlist;     /* pointer to dnlist in core              */
 char       *dnint_ptr,      /* ptr to dninterpretation in record ...  */
		*ename_ptr,  /* ptr to exact name in record...         */
		*dnoccur_ptr;/* ptr to occurrency in record ...        */

 signed32       max_short;       /* variable for max. index check          */
 signed32       old_dnlidx;      /* variable for check whether value of    */
				 /* d26t_maxidx_dnlist[] was changed       */
				 /* (this information is used for          */
				 /* allocating memory for d26t_accp_list)  */
 D26_acc_point *old_psap_list;   /* pointer to old Access-point list       */
 unsigned32     no_mems;         /* number of shorts or D26_dndescs which  */
				 /* memory must be allocated for           */

/* FASTAUTO  ...    */
register signed32  i;            /* counter */
register boolean   alloc_mem;    /* indicator whether memory for dn list   */
				 /* should be allocated                    */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

/****************/
/* set no key   */
/****************/

if                      /*  d26_t23_set_dnlistkey (NO_KEY,sch) fails       */
   ((d26_t23_set_dnlistkey (D26_I04_NO_KEY, sch)) != D2_NO_ERR)
{                       /*  ---> MA-LOG and exit function                  */

     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, D26_ERROR));
     return (D26_ERROR); 
}                       /*  d26_t23_set_dnlistkey fails                    */


/****************/
/* get maxindex */
/****************/

if                      /*  isread (last record) fails                     */
   (D26_ISREAD (svc_c_sev_warning, d26_file_dnlist[sch], dnlist_rec, 
		(int) ISLAST) < 0)
{   
     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, D26_ERROR));
     return (D26_ERROR); 
}                       /*  isread (last record) fails                     */

/*******************/
/* save max. index */
/*******************/
old_dnlidx = d26t_maxidx_dnlist[sch]; /* save old value of d26t_maxidx_dnlist   */

if                      /*  maximum index exceeded                         */
   ((max_short = isrecnum - D26_I03_GET_CORE_INDEX) > D26_MAX_DNLIDX)
{                       /*  error log                                      */
     dce_svc_printf(GDS_S_MAX_DN_INDEX_MSG);
     d26t_maxidx_dnlist[sch] = (signed32)D26_MAX_DNLIDX;    
}
else
{                       /*  save maxidx                                    */
     d26t_maxidx_dnlist[sch] = max_short;    
}                       /*  max. index exceeded */

/*********************************/
/* get memory for d26t_accp_list[sch]  */
/*********************************/
			/* calculate no. of memory segments which must be  */
			/* allocated                                       */
no_mems = d26t_maxidx_dnlist[sch] + 1;

if                      /*  d26t_maxidx_dnlist changed or psap_list == 0    */
   (((d26t_maxidx_dnlist[sch] > old_dnlidx) ||
    (d26t_accp_list[sch] == (D26_acc_point *) NULL)) &&
    (d26t_maxidx_dnlist[sch] >= 0))
{                       /*  allocate memory for d26t_accp_list[sch]         */
     if (d26t_accp_list[sch] == (D26_acc_point *) NULL)
     {    /* calloc */
	  d26t_accp_list[sch] =
	      (D26_acc_point *) D27_CALLOC(svc_c_sev_fatal, 
					   no_mems, sizeof (D26_acc_point));
     }
    else
    { /* realloc */
	  old_psap_list = d26t_accp_list[sch];
	  d26t_accp_list[sch] =
	      (D26_acc_point *) D27_REALLOC (svc_c_debug1, 
		(char *) old_psap_list, (no_mems * sizeof (D26_acc_point)));
	  if                 /* realloc fails                           */
	     (d26t_accp_list[sch] == (D26_acc_point *) NULL)
	  {                  /* not enough memory for PSAP list            */
	       free (old_psap_list);
	  }                  /* realloc fails */
     }  /* d26t_accp_list[sch] == D2_NULL */

}
else
{                       /*  free old PSAP list memory when necessary      */
     if ((d26t_accp_list[sch] != (D26_acc_point *) NULL) &&
	 (d26t_maxidx_dnlist[sch] < 0))
     {   
	  free (d26t_accp_list[sch]);
	  d26t_accp_list[sch] = (D26_acc_point *) NULL;
     } 
}                       /*  d26t_maxidx_dnlist changed */

/******************************/
/* get memory for new DN list */
/******************************/
alloc_mem = TRUE;

if (d26_dnlist[sch] != (D26_dndesc *) NULL) /*  old DN list in memory        */
{                                         
     if                                   /*  d26t_maxidx_dnlist changed    */
       ((d26t_maxidx_dnlist[sch] > old_dnlidx) || (d26t_maxidx_dnlist[sch] < 0))
     {                                    /*  free old memory              */
	  free (d26_dnlist[sch]);
	  d26_dnlist[sch] = (D26_dndesc *) NULL;
     }
     else
     {                                    /*  enough memory already        */
					  /*  allocated                    */
	  alloc_mem = FALSE;
     }                                    /*  d26t_maxidx_dnlist changed */
}                                         /*  old DN list in memory        */

if                                 /*  new DN list not empty and memory    */
				   /*  must be allocated                   */
   ((d26t_maxidx_dnlist[sch] >= 0) && (alloc_mem == TRUE))
{                                  /*  allocate memory for the new DN list */
    d26_dnlist[sch] =
	(D26_dndesc *) D27_CALLOC (svc_c_debug1, no_mems, sizeof (D26_dndesc));

     if (d26_dnlist[sch] == (D26_dndesc *) NULL)
     {   
	/* ERROR: not enough memory for new DN list available */
	d26t_maxidx_dnlist[sch] = -1L;/* no valid index for DN list in core ! */
	return_value = D26_ERROR;
     } 
}                                  /*  new DN list not empty */

/**************************************************************/
/* initialize variables for memory management (first memarea) */
/**************************************************************/

next_idx_memareas = 0;          /* next index of d26_mem_areas       */
begin_memarea = next_dn_memory = d26_mem_areas[sch][next_idx_memareas];
end_memarea = begin_memarea + D26_I01_MEM_AREA_SIZE;

/********************/
/* read new DN list */
/********************/

if                      /* isread (record with version) fails         */
   (D26_ISREAD(svc_c_sev_warning, d26_file_dnlist[sch], dnlist_rec, 
	       (signed32) ISFIRST) < 0)
{       
     d26t_maxidx_dnlist[sch] = -1L;/* no valid index for DN list in core ! */
     return_value = D26_ERROR;
}                       /*  isread (record with version) fails            */


/* initialize pointers to record positions */
dnint_ptr   = &dnlist_rec[D26_I10_DNINT];      /* dn interpretation     */
ename_ptr   = &dnlist_rec[MAXKEYSIZE];         /* exact dist. name      */
dnoccur_ptr = &dnlist_rec[D26_I12_OCCURRENCY]; /* no of references to dn*/

for                     /*  number of records with a dn in dnlist file     */
   (i=0; i<=d26t_maxidx_dnlist[sch]; i++)
{                       /*  read record with dn                            */
   /***************/
   /* read record */
   /***************/
   if                   /* isread (record with dn) fails                   */
      (D26_ISREAD(svc_c_sev_warning, d26_file_dnlist[sch], dnlist_rec, 
		  (signed32) ISNEXT) < 0)
   { 
	d26t_maxidx_dnlist[sch] = i - 1L;
	    /* (i-1) valid indices for dn list in core given */
	return_value = D26_ERROR;
	break;        /* exit loop                                       */
   }                    /* isread (record with dn) fails                   */

   /*****************************************/
   /* set dnlist entry in core (d26_dnlist[sch]) */
   /*****************************************/
		/*  set dnlist entry in core (d26_dnlist[sch])          */
   ptr_dnlist = &d26_dnlist[sch][i];
		/*  dn interpretation (d26_dnlist[sch].d26_dnint)       */
   ptr_dnlist->d26_dnint = *dnint_ptr;
		/*  occurrency (d26_dnlist[sch].d26_occurrency)         */
   ptr_dnlist->d26_occurrency = ldlong(dnoccur_ptr);

                /*  dn (d26_dnlist[sch].d26_dnname):                    */
		/*  look for memory for distingushed name string   */

   /****************************/
   /* memory for dnname string */
   /****************************/
   /* initialize variables */
   dn_length = strlen(ename_ptr);
   new_memory = FALSE;            /* allocate no new memory    */

   if (begin_memarea == NULL) /* no memory allocated                    */
   {                          /* set indicator for allocate new memory  */
	new_memory = TRUE;
   }
else
{                        /* memory already allocated               */
			 /* check whether there is enough memory   */
			 /* for the new distingushed name string   */
	if ((next_dn_memory + 2 * ( dn_length + 1)) > end_memarea)
	{                /* there isn't enough memory              */
			 /* set indicator for allocate new memory  */
	     new_memory = TRUE;
	} 
   }                     /* no memory allocated                    */


   if (new_memory == TRUE)    /* new memory must be allocated           */
   {   

	if                    /* valid index of d26_mem_areas is given ?*/
	   ((next_idx_memareas >= D26_I00_NO_MEM_AREAS)
	    || (next_idx_memareas < 0))
	{                        /* ERROR: no valid index of d26_mem_areas */
	     dce_svc_printf(GDS_S_MAX_DN_MEMORY_MSG);
	     d26t_maxidx_dnlist[sch] = i - 1L;
			  /* (i-1) valid indices for dn list in core given */
	     return_value = D26_ERROR;
	     break;              /* exit loop                              */
	}
	else
	{                        /* valid index of d26_mem_areas is given  */
				 /* allocate new memory                    */
	     d26_mem_areas[sch][next_idx_memareas] =
		D27_MALLOC (svc_c_debug1, D26_I01_MEM_AREA_SIZE);
				 /* set new begin of memory area and next dn*/
	     begin_memarea = next_dn_memory =
			      d26_mem_areas[sch][next_idx_memareas];
				 /* set end of memarea                     */
	     end_memarea = begin_memarea + D26_I01_MEM_AREA_SIZE;

	     if                  /* memory couldn't be allocated or not    */
				 /* enough memory for dn string ?          */
		((begin_memarea == NULL)
	    || ((next_dn_memory + 2 * (dn_length + 1)) > end_memarea))
	     {                   /* ERROR: no memory for dn string         */
		  d26t_maxidx_dnlist[sch] = i - 1L;
			  /* (i-1) valid indices for dn list in core given */
		  return_value = D26_ERROR;
		  break;         /* exit loop                              */
	     }                   /* memory couldn't be allocated ... */

				 /* set next index of d26_mem_areas        */
	     next_idx_memareas++;
	}                        /* valid index of d26_mem_areas ? */
   }                             /* new memory must be allocated  */



   /******************/
   /* dn name string */
   /******************/
			/*  set d26_dnlist[sch].d26_dnname                 */
   ptr_dnlist->d26_dnname = (D2_name_string) next_dn_memory;
			/*  norm dnname from record in memory              */
   d27_007_norm_name(ename_ptr,dn_length + 1,next_dn_memory);
			/*  set pointer for next dnname string             */
   next_dn_memory = next_dn_memory + dn_length + 1;

   ptr_dnlist->d26_ename = (D2_name_string)next_dn_memory;
			/*  copy dnname from record in memory              */
   strcpy (next_dn_memory, ename_ptr);
			/*  set pointer for next dnname string             */
   next_dn_memory = next_dn_memory + dn_length + 1;

                        

}                       /*  records with dn in file */



DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, return_value));
return (return_value);          
}                               /*  d26_t43_read_dnlist               */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t21_upd_core_dnlist                               */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        15.01.88                                              */
/*                                                                    */
/* SYNTAX:      EXPORT Short d26_t21_upd_core_dnlist (sch)            */
/*              Int         sch;                                      */
/*                                                                    */
/* DESCRIPTION: This function checks whether the dnlist in file was   */
/*              updated since last read of dn list. When the file     */
/*              was updated the function d26_t43_read_dnlist is       */
/*              called to read the entries of the isam file and store */
/*              them in memory.                                       */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*    Short  D26_NO_CHANGE      no error occurred                     */
/*                              the dnlist in memory wasn't changed   */
/*                              because there was no update on dnlist */
/*                              in file                               */
/*                                                                    */
/*           D26_CHANGED        no error occurred                     */
/*                              there was an update on dnlist in file */
/*                              the new entries were successfully     */
/*                              read and stored in memory             */
/*                                                                    */
/*           D26_ERROR          any error occurred e.g. C-isam error  */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*    D26_update_info  d26t_upd_info[] last version of the DN-lisr in  */
/*                                    isam file                       */
/*                                    (version and reason (not yet    */
/*                                     used) stored in the first      */
/*                                     record of the isam file are    */
/*                                     stord in this global variable) */
/*                                                                    */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        d26_t43_read_dnlist                                         */
/*        d26_t24_get_updinfo                                         */
/*        d26_t25_open_dnlistfile                                     */
/*                                                                    */
/* NOTES: For the record format of isam file record of dnlist entry   */
/*        see commentary of defines of positions of datas in this     */
/*        module.                                                     */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_t21_upd_core_dnlist
(
  signed32 sch
)

{                           /*  d26_t21_upd_core_dnlist               */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

/* AUTO      ...    */
 signed16            return_value = D26_NO_CHANGE;

 D26_update_info  update_info;   /* parameter for d26_t24_get_updinfo */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/


/*********************/
/* open DN list file */
/*********************/

if                      /*  open of dnlist file not successfull            */
   (d26_t25_open_dnlistfile (sch) != D2_NO_ERR)
{                       /*  ERROR: cannot open dnlist file                 */
     return (D26_ERROR); 
}                       /*  open successfull                               */


/*******************/
/* get update_info */
/*******************/

if                      /*  d26_t24_get_updinfo fails                     */
   (d26_t24_get_updinfo (&update_info, sch) != D2_NO_ERR)
{                       /*  ERROR: cannot get update info from dnlist file*/
     return (D26_ERROR); 
}                       /*  get_updinfo fails                             */


/************************************************/
/* check whether dnlist in core must be updated */
/************************************************/

if                      /*  dnlist in file was updated since last read of */
			/*  dnlist (d26_t20_read_dnlist)                  */
   (update_info.d26_version != d26t_upd_info[sch].d26_version)
{                       
     if                 /*  d26_t43_read_dnlist fails                     */
	(d26_t43_read_dnlist (sch) != D2_NO_ERR)
     {                  /*  ERROR: cannot read new dnlist from file       */
	  return_value = D26_ERROR; 
     }
else
{                  /*  save new update info:                         */
			/*  time of last update                           */
	  d26t_upd_info[sch].d26_version   = update_info.d26_version;
			/*  reason of last update                         */
	  d26t_upd_info[sch].d26_reason = update_info.d26_reason;

			/*  set return value D26_CHANGED                  */
	  return_value = D26_CHANGED;
     }                  /*  d26_t43_read_dnlist fails */
}                       /*  dnlist in file was updated ....               */

return (return_value); 

}                               /*  d26_t21_upd_core_dnlist                */



/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t22_upd_file_dnlist                               */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        18.01.88                                              */
/*                                                                    */
/* SYNTAX:      EXPORT Short d26_t22_upd_file_dnlist (type, old_dn,   */
/*                             dnint, new_dn, occurrency, new_version,*/
/*                             dnlist_core_idx, sch)                  */
/*                                                                    */
/*              D2_attr_mod      type;             ->                 */
/*              D2_name_string   old_dn;           ->                 */
/*              D2_dnint         dnint;            ->                 */
/*              D2_name_string   new_dn;           ->                 */
/*              Long             occurrency;       ->                 */
/*              Bool             new_version;      ->                 */
/*              Long            *dnlist_core_idx;  <>                 */
/*              Int              sch;              ->                 */
/*                                                                    */
/*                                                                    */
/* DESCRIPTION: This function updates one record in the dnlist in     */
/*              the isam file.                                        */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*       D2_attr_mod     type               type of modification      */
/*                             D2_REP_AV    replace record            */
/*                                          this value is needed when */
/*                                          the DSA function          */
/*                                          DS_REPLACE_RDN was        */
/*                                          called                    */
/*                                          when the old dn exists in */
/*                                          the dnlist the records    */
/*                                          with the old dn must be   */
/*                                          replaced with record with */
/*                                          the new dn                */
/*                             D2_ADD_AV    "add record"              */
/*                                          increases the occurrency  */
/*                                          of an existing record or  */
/*                                          adds a new record to the  */
/*                                          file using a record with  */
/*                                          occurency 0 or writing a  */
/*                                          new record                */
/*                             D2_DEL_AV    "delete record"           */
/*                                          decreases the occurrency  */
/*                                          of an existing record     */
/*                             D26_NOTHING  no modification on a      */
/*                                          dnlist record in isam file*/
/*                                          all parameters are ignored*/
/*                                          except "new_version"      */
/*                                          this is usefull when the  */
/*                                          time record only must be  */
/*                                          updated                   */
/*                                                                    */
/*       D2_name_string  old_dn             the old distinguished name*/
/*                                          (normed value)            */
/*                                          which is to be replaced   */
/*                                          by the value of new dn    */
/*                                          must be given only when   */
/*                                          type == D2_REP_AV (in the */
/*                                          other cases this parameter*/
/*                                          is ignored)               */
/*                                                                    */
/*       D2_dnint        dnint              interpretation of dn      */
/*                                          must be given only when   */
/*                                          type == D2_ADD_AV and     */
/*                                          dnlist_core_idx < 0       */
/*                                          (in the other cases this  */
/*                                          parameter is ignored)     */
/*                                                                    */
/*       D2_name_string  new_dn             distinguished name        */
/*                                          (normed value)            */
/*                                          type == D2_ADD_AV and     */
/*                                          dnlist_core_idx < 0:      */
/*                                            dn which must be added  */
/*                                            dnint must be given     */
/*                                          type == D2_REP_AV:        */
/*                                            new value of the dn     */
/*                                            (all records with the   */
/*                                            old dn must be replaced */
/*                                            by records with the new */
/*                                            dn)                     */
/*                                          this parameter is ignored */
/*                                          when type == D2_DEL_AV    */
/*                                          and dnlist_core_idx >= 0  */
/*                                          when type == D2_ADD_AV    */
/*                                                                    */
/*       Long            occurrency         the value which the       */
/*                                          occurrency of a dn must   */
/*                                          be decreased (type ==     */
/*                                          D2_DEL_AV) or increased   */
/*                                          (type == D2_ADD_AV)       */
/*                                          this parameter is ignored */
/*                                          when type == D2_REP_AV    */
/*                                                                    */
/*       Bool            new_version        indicator for writing the */
/*                                          new version into the first*/
/*                                          record                    */
/*                               D2_TRUE    write new version in first*/
/*                                          record (when errors       */
/*                                          occurred before writing   */
/*                                          the new version           */
/*                                          nevertheless the new time */
/*                                          will be written)          */
/*                                          when type == D2_REP_AV    */
/*                                          the new version is written*/
/*                                          only when the isam file   */
/*                                          was changed               */
/*                               D2_FALSE   the new version isn't     */
/*                                                                    */
/*       Long            *dnlist_core_idx   INPUT OUTPUT parameter    */
/*                          type == D2_ADD_AV:                        */
/*                          D26_IDX_NOT_DEF it is unknown whether a   */
/*                                          record with new_dn and    */
/*                                          dnint exists              */
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
/*                          type == D2_DEL_AV:                        */
/*                             >= 0         the record exists in the  */
/*                             (must be!!)  isam file and is read via */
/*                                          isrecnum: = *dnlist_core_ */
/*                                          idx + D26_I03_GET_CORE_IDX*/
/*                                          the parameters old_dn,    */
/*                                          dnint and new_dn are      */
/*                                          ignored                   */
/*                          type == D2_REP_AV:                        */
/*                                          this parameter is ignored */
/*                                                                    */
/*                                                                    */
/*                                                                    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     Short    D2_NO_ERR   no error occured (update successfull)     */
/*                                                                    */
/*              D26_ERROR   any error occurred (e.g. C-isam error or  */
/*                          parameter error)                          */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*    LOCAL Fildes d26_file_dnlist[]      filedescriptor of dnlist    */
/*                                        in isam file                */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        isrewcurr                                                   */
/*        isrelease                                                   */
/*                                                                    */
/*        d26_t27_read_dnlistrec                                      */
/*        d26_t31_rep_dnlistrec                                       */
/*        d26_t32_add_dnlistrec                                       */
/*        d26_t33_del_dnlistrec                                       */
/*                                                                    */
/* NOTES: After this function was executed successfully the dnlist    */
/*        in core must be updated. This will be executed when the     */
/*        function d26_t21_upd_core_dnlist is called at another time. */
/*                                                                    */
/*exoff ***************************************************************/


signed16 d26_t22_upd_file_dnlist
(
  D2_attr_mod      type,
  D2_name_string   old_dn,
  D2_dnint         dnint,
  D2_name_string   new_dn,
  signed32         occurrency,
  boolean          new_version,
  signed32        *dnlist_core_idx,
  signed32         sch
)

{                               /*  d26_t22_upd_file_dnlist           */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

/* STATIC    ...    */
static String    function = "d26_t22_upd_file_dnlist";

/* AUTO      ...    */
			/* record of dn list file           */
 char  dnlistrec[D26_I02_RECDNLISTL];
 signed16 return_value = D2_NO_ERR;
 boolean  changed = FALSE;  /* indicator whether isam file was changed */
			/* when type is D2_REP_AV                  */
 char  *ptrversion;     /* ptr to version value in version record  */
 signed16 rcode;        /* return code of read record function     */

/* FASTAUTO  ...    */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

/***********************/
/* record modification */
/***********************/

switch ((int) type)          /*  type of record modification               */
{
case D2_REP_AV:              /*  replace record                            */
                             /*  caller is DS_REPLACE_RDN:                 */
			     /*  the given old dn should be replaced by the*/
			     /*  given new dn in all records which were    */
			     /*  found with the old dn                     */
                             
     return_value = d26_t31_rep_dnlistrec (old_dn, new_dn, &changed, sch);
     break;

case D2_ADD_AV:              /*  "add" record                              */
                             /*  a new dnlist entry should be added to isam*/
			     /*  file by increasing the occurrency of an   */
			     /*  existing record, by using a unused record */
			     /*  in isam file (occurrency = 0) or by       */
			     /*  writing a new isam file record            */
                             
     return_value = d26_t32_add_dnlistrec (dnint,
					   new_dn,
					   occurrency,
					   dnlist_core_idx, sch);
     break;

case D2_DEL_AV:              /*  "delete" record                           */
                             /*  an old dnlist entry should be "deleted"   */
			     /*  in isam file by decreasing the occurrency */
			     /*  of the existing record                    */
			     /*  when the occurrency gets the value 0 the  */
			     /*  record isn't used and can be used to "add"*/
			     /*  a new dnlist entry to isam file           */
                             
     return_value = d26_t33_del_dnlistrec (*dnlist_core_idx, occurrency,sch);
     break;

case D26_NOTHING:            /*  no modification on record                 */
                             /*  no action                                 */
			     /*  (only time update)                        */
                             
     break;

default:                     /* error: wrong parameter value for type      */
     return_value = D26_ERROR;
     break;
}                            /*  type of record modification */

/*******************************/
/* version record modification */
/*******************************/

if (new_version == TRUE)  /*  version record should be updated          */
{                            
     if                      /*  (isam file changed and type is replace)   */
			     /*  or other type                             */
       (((type == D2_REP_AV) && (changed == TRUE))
	|| (type != D2_REP_AV))
     {                       
			     /* set ptr to version value                   */
	  ptrversion = &dnlistrec[D26_I09_VERSION];
			     /*  read version record                       */
			     /* set record no of version record            */
	  isrecnum = D26_I16_VERSION_RECNUM;
					  /* read record with lock         */
	  rcode = d26_t27_read_dnlistrec (D26_I04_NO_KEY,
					  dnlistrec,
					  TRUE, sch);

	  switch ((int) rcode) /*  return code of read                     */
	  {
	  case D26_I13_FOUND: /*  record found                             */
	                      /*  get new version and update record value  */
	       stlong(ldlong(ptrversion) + 1L,ptrversion);

			      /*  rewrite record                           */
	       if ((D26_ISREWCURR(svc_c_sev_warning,d26_file_dnlist[sch], 
		    dnlistrec)) < 0)
	       {              /* isam error occurred                       */
		    return_value = D26_ERROR; /* set return value          */
	       }              /* isam error occured                        */

			      /*  unlock record                            */
	       if ((D26_ISRELEASE(svc_c_sev_warning,d26_file_dnlist[sch])) < 0)
	       {              /* isam error occurred                       */
		    return_value = D26_ERROR; /* set return value          */
	       }              /* isam error occured                        */
	                      
	       break;

	  case D26_I14_NOT_FOUND: /*  record not found                     */
	                      /*  error: data inconsistency in isam file   */
			      /*  set return_value                         */
	       return_value = D26_ERROR;
	       break;

	  default:            /* C-isam error occurred                     */
			      /* set return value                          */
	       return_value = D26_ERROR;
	       break;
	  }                   /*  rcode                                    */
     }                       /*  isam file changed ...                     */
}                            /*  version record should be updated          */


DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, return_value));
return (return_value);

}                               /*  d26_t22_upd_file_dnlist           */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t23_set_dnlistkey                                 */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        13.01.88                                              */
/*                                                                    */
/* SYNTAX:      EXPORT Short d26_t23_set_dnlistkey (key, sch)         */
/*                                                                    */
/*              Short   key;                                          */
/*              Int     sch;                                          */
/*                                                                    */
/* DESCRIPTION: This function sets the key of the distinguished name  */
/*              list in isam file when the actually set key is        */
/*              another than the wished key.                          */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      Short     key                       wished key                */
/*                   D26_I04_KEY_NO_KEY     no key (ordering sequence */
/*                                          is isrecnum)              */
/*                   D26_I05_KEY_DNINTANDDN dninterpretation and dn   */
/*                   D26_I06_KEY_OCCURRENCY occurrency                */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      Short   D2_NO_ERR               no error occured              */
/*                                                                    */
/*              D26_ERROR               isstart returns an error      */
/*                                      or wrong parameter value for  */
/*                                      key given (programmers error!)*/
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*    LOCAL Fildes d26_file_dnlist[]  filedescriptor for dn list in   */
/*                                    file                            */
/*                                                                    */
/*    LOCAL Short  d26_act_isam_key[] actual set isam key of dn list  */
/*                                    file                            */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*    LOCAL Short  d26_act_isam_key[] actual set isam key of dn list  */
/*                                    file                            */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*          isstart                                                   */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_t23_set_dnlistkey
(
  signed16 key,
  signed32 sch
)

{                               /*  d26_t23_set_dnlistkey             */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/
/* AUTO      ...    */
 char dnlist_rec[D26_I02_RECDNLISTL];

 struct  keydesc  isam_key;        /* isam description of key          */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

if (key != d26_act_isam_key[sch]) /*  wished key != actually isam key      */
{                            /*  set key for dn list in isam file using    */
			     /*  C-isam function isstart                   */

     /* initialize variables which are the same for nearly all keys */
     isam_key.k_flags  = ISDUPS + COMPRESS;
     isam_key.k_nparts = 1;

     /* set variable isam_key for call of isstart */
     switch ((int) key)                /* wished key                       */
     {
     case D26_I04_NO_KEY:              /* no key (ordering sequence        */
				       /* isrecnum)                        */
	  isam_key.k_flags  = 0;
	  isam_key.k_nparts = 0;
	  break;

     case D26_I05_KEY_DNINTANDDN:      /* DN interpretation and DN         */
	  isam_key.k_part[0].kp_start = D26_I10_DNINT;
	  isam_key.k_part[0].kp_leng  = MAXKEYSIZE;
	  isam_key.k_part[0].kp_type  = CHARTYPE;
	  break;

     case D26_I06_KEY_OCCURRENCY:      /* occurrency (no of references to  */
				       /* dn)                              */
	  isam_key.k_part[0].kp_start = D26_I12_OCCURRENCY;
	  isam_key.k_part[0].kp_leng  = LONGSIZE;
	  isam_key.k_part[0].kp_type  = LONGTYPE;
	  break;

     case D26_I07_KEY_DN:              /* distinguished name               */
     /* not used
      * (such an index is not added in dn list in file at this time)
	  isam_key.k_part[0].kp_start = D26_I11_DNNAME;
	  isam_key.k_part[0].kp_leng  = MAXKEYSIZE;
	  isam_key.k_part[0].kp_type  = CHARTYPE;
	  break;
     */

     default:
	  /* error (programmer!!!) */
	  return (D26_ERROR);
     }   

     /* call isstart */
     if (D26_ISSTART(svc_c_sev_warning,d26_file_dnlist[sch], &isam_key, 0, 
		     dnlist_rec, ISFIRST) < 0)
     {   
	  return (D26_ERROR);
     } 

     d26_act_isam_key[sch] = key;    /*  set act_isam_key                  */
}                                    /*  act_isam_key != key               */


return (D2_NO_ERR);
}                               /*  d26_t23_set_dnlistkey             */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t24_get_updinfo                                   */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        15.01.88                                              */
/*                                                                    */
/* SYNTAX:      EXPORT Short d26_t24_get_updinfo (upd_info, sch)      */
/*                                                                    */
/*              D26_update_info *upd_info     <>                      */
/*              Int             sch                                   */
/*                                                                    */
/* DESCRIPTION: This function reads the first record of dnlist in     */
/*              isam file where the time of last modification is      */
/*              stored.                                               */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      D26_update_info   *upd_info   INPUT-OUTPUT parameter          */
/*                                    pointer to memory where the     */
/*                                    function stores the datas       */
/*                                    (reason and version)            */
/*                                                                    */
/* OUTPUT-PARAMETERS: (s. INPUT parameter upd_info)                   */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     Short    D2_NO_ERR        no error ouucured                    */
/*                                                                    */
/*              D26_ERROR        any error occurred (e.g. C-isam      */
/*                               error)                               */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*    LOCAL Fildes d26_file_dnlist[]  filedescriptor for dn list in   */
/*                                    file                            */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        isread                                                      */
/*        d26_t23_set_dnlistkey                                       */
/*                                                                    */
/* NOTES: For the record format of the first isam file record of      */
/*        dnlist which contains the time of last modification of the  */
/*        file see commentary of defines of positions of datas in     */
/*        this module.                                                */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_t24_get_updinfo 
(
  D26_update_info *upd_info,
  signed32         sch
)

{                               /*  d26_t24_get_updinfo               */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

/* STATIC    ...    */
static String    function = "d26_t24_get_updinfo";

/* AUTO      ...    */
 char dnlist_rec[D26_I02_RECDNLISTL];
				  /* record of dn list file           */
/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

DCE_SVC_LOG((GDS_S_ENTRY_FUNCTION_MSG,function));

/****************/
/* set no key   */
/****************/

if                      /*  d26_t23_set_dnlistkey (NO_KEY,sch) fails       */
   ((d26_t23_set_dnlistkey (D26_I04_NO_KEY,sch)) != D2_NO_ERR)
{                       /*  ---> MA-LOG and exit function                  */

     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, D26_ERROR));
     return (D26_ERROR); 
}                       /*  d26_t23_set_dnlistkey fails                    */

/****************/
/* get upd_info */
/****************/

if                      /*  isread (record with version) fails             */
   (D26_ISREAD(svc_c_sev_warning, d26_file_dnlist[sch], dnlist_rec, 
		(signed32) ISFIRST) < 0)
{    
     DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, D26_ERROR));
     return (D26_ERROR); 
}                       /*  isread (record with version) fails             */

			/*  save upd_info                                  */
			/*  - reason                                       */
upd_info->d26_reason = dnlist_rec[D26_I08_REASON];
			/*  - version                                      */
upd_info->d26_version   = ldlong(&dnlist_rec[D26_I09_VERSION]);


DCE_SVC_LOG((GDS_S_EXIT_FUNCTION_MSG,function, D2_NO_ERR));
return (D2_NO_ERR);
}                               /*  d26_t24_get_updinfo                    */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t25_open_dnlistfile                               */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        15.01.88                                              */
/*                                                                    */
/* SYNTAX:      EXPORT Short d26_t25_open_dnlistfile (sch)            */
/*              Int         sch;                                      */
/*                                                                    */
/* DESCRIPTION: This function opens the isam file of dnlist when it   */
/*              is closed.                                            */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     Short   D2_NO_ERR        no error occurred                     */
/*                              the isam file is opened               */
/*                                                                    */
/*             D26_ERROR        C-isam error occurred                 */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*    char     d26_install[][] name of directory of database          */
/*                                                                    */
/*    String   d26_name_dnlist name of isam file for DN list          */
/*                             (this variable is global only for this */
/*                              module)                               */
/*                                                                    */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*    LOCAL Fildes d26_file_dnlist[]  filedescriptor for dn list in   */
/*                                    file                            */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        isopen                                                      */
/*        strcpy                                                      */
/*        strcat                                                      */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_t25_open_dnlistfile
(
signed32     sch
)

{                               /*  d26_t25_open_dnlistfile               */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

/* AUTO      ...    */
 signed16 return_value = D2_NO_ERR;

				 /* absolute filename of dn list file */
 char  filename_dnl[D27_LFILE_NAME];

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

/*********************/
/* open DN list file */
/*********************/

if                      /*  file of distinguished name list is closed      */
   (d26_file_dnlist[sch] < 0)
{                       /*  open DN list file                              */

     /* build name of DN list file */
     strcpy (filename_dnl, d26_install[sch]);
     strcat (filename_dnl, d26_name_dnlist);

     /* open file */
     d26_file_dnlist[sch] = D26_ISOPEN(svc_c_sev_warning, filename_dnl, 
					((int) (ISINOUT + ISMANULOCK)));
     if (d26_file_dnlist[sch] < 0)
     {   
	  return_value = D26_ERROR;
     }  /* d26_file_dnlist[sch] < 0 */
}                       /*  DN list file closed                            */

return (return_value); 

}                               /*  d26_t25_open_dnlistfile                */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t26_close_dnlistfile                              */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        15.01.88                                              */
/*                                                                    */
/* SYNTAX:      EXPORT signed16 d26_t26_close_dnlistfile (sch)        */
/*              Int     sch;                                          */
/*                                                                    */
/* DESCRIPTION: This function closes the isam file of dnlist when it  */
/*              is opened.                                            */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     signed16   D2_NO_ERR     no error occurred                     */
/*                              the isam file is closed               */
/*                                                                    */
/*             D26_ERROR        C-isam error occurred                 */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*    LOCAL Fildes d26_file_dnlist[]  filedescriptor for dn list in   */
/*                                    file                            */
/*                                    when the isam file was closed   */
/*                                    successfully d26_file_dnlist is */
/*                                    set to -1                       */
/*                                                                    */
/*    LOCAL signed16  d26_act_isam_key[] actual set isam key of dn list  */
/*                                    file                            */
/*                                    when the isam file was closed   */
/*                                    successfully d26_act_isam_key is*/
/*                                    set to -1                       */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        isclose                                                     */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_t26_close_dnlistfile
(
signed32     sch
)

{                               /*  d26_t26_close_dnlistfile               */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

/* AUTO      ...    */
 signed16 return_value = D2_NO_ERR;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/


/**********************/
/* close DN list file */
/**********************/

if                      /*  file of distinguished name list is opened      */
   (d26_file_dnlist[sch] >= 0)
{                       /*  close DN list file                             */

     if (D26_ISCLOSE(svc_c_sev_warning, d26_file_dnlist[sch]) < 0)
     {   
	  return_value = D26_ERROR;
     }
     else
     { /* file was closed successfully              */
	  /* set d26_file_dnlist[sch] to -1 as an indicator */
	  /* that the file is closed                   */
	  d26_file_dnlist[sch] = -1;
	  /* set d26_act_isam_key[sch] to -1 as an indicator   */
	  /* that no key is set actually that is the      */
	  /* function d26_t23_set_dnlistkey wasn't called */
	  d26_act_isam_key[sch] = -1;
     }  /* d26_file_dnlist[sch] < 0 */
}                       /*  DN list file closed                            */

return (return_value); 

}                               /*  d26_t26_close_dnlistfile               */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t27_read_dnlistrec                                */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        20.01.88                                              */
/*                                                                    */
/* SYNTAX:      EXPORT Short d26_t27_read_dnlistrec (key, dnlistrec,  */
/*                                                   lock, sch)       */
/*                                                                    */
/*              Short   key;             ->                           */
/*              char    *dnlistrec;      <>                           */
/*              Bool    lock;            ->                           */
/*              Int     sch;             ->                           */
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
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*    LOCAL Fildes d26_file_dnlist[]  filedescriptor for dn list in   */
/*                                    file                            */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*          strcpy                                                    */
/*          strlen                                                    */
/*          strcmp                                                    */
/*          strncmp                                                   */
/*                                                                    */
/*          isread                                                    */
/*          isrelease                                                 */
/*                                                                    */
/*          sleep                                                     */
/*                                                                    */
/*          d26_t23_set_dnlistkey                                     */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_t27_read_dnlistrec
(
  signed16  key,
  char    *dnlistrec,
  boolean  lock,
  signed32 sch
)

{                               /*  d26_t27_read_dnlistrec            */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

/* AUTO      ...    */
 signed16       return_value = D26_I13_FOUND;
 signed32       rcode,                    /* return code of c-isam function*/
		mode;                     /*  mode for c-isam isread fct   */

 char        save_key [D26_I02_RECDNLISTL]; /* key: dnint and dn        */
 signed16    ln_save_key;                   /* length of save_key       */
 signed32    i;

 char        *ptr_m_save_key,   /* ptr to rest of searched value        */
				/* (> MAXKEYSIZE)                       */
	     *ptr_dnlistrec,    /* ptr to begin of value in read record */
	     *ptr_m_dnlistrec;  /* ptr to rest of value in read record  */
				/* (> MAXKEYSIZE)                       */
 signed32    rec_no;            /* saved record number                  */

/* FASTAUTO  ...    */
register boolean read_rec;    /* indicator for reading record           */

extern int isread (int isfd, String record, register int mode);

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

/************************/
/* initialize variables */
/************************/
switch ((int) key)           /*  given key                               */
{
case D26_I04_NO_KEY:         /*  no key (key = isrecnum)                 */
                             /*  save isrecnum (because isrecnum is      */
			     /*  changed by isstart)                     */
                             
     rec_no = isrecnum;
     break;

case D26_I06_KEY_OCCURRENCY: /*  occurrency                              */
                             /*  no action                               */
                             
     break;

case D26_I05_KEY_DNINTANDDN: /*  dn interptretation and dn               */
                        /*  save key value to check whether the correct    */
			/*  record was read                                */
			/*  ptr to begin of key value in read record       */
     ptr_dnlistrec   = &dnlistrec[D26_I10_DNINT];
     strcpy (save_key, ptr_dnlistrec);
			/*  calculate length of save_key                   */
     ln_save_key = strlen (save_key);
			/*  ptr to rest of searched value (> MAXKEYSIZE)   */
     ptr_m_save_key  = &save_key[MAXKEYSIZE];
			/*  ptr to rest of value in read record            */
			/*  (> MAXKEYSIZE)                                 */
     ptr_m_dnlistrec = &dnlistrec[D26_I10_DNINT+MAXKEYSIZE];
                        
     break;

default:                     /* error which never occurrs               */
     return (D26_ERROR);
}               /*  given key                                       */

read_rec = TRUE;     

			/*  set mode for isread                            */
mode = ISEQUAL;
if (lock == TRUE)   /* record should be locked                          */
{   
     mode = mode + ISLOCK;
} 

/***********/
/* set key */
/***********/

if                      /*  d26_t23_set_dnlistkey (key,sch) fails          */
   ((d26_t23_set_dnlistkey (key,sch)) != D2_NO_ERR)
{  
     return (D26_ERROR); 
}                       /*  d26_t23_set_dnlistkey fails                    */


/**********************/
/* read wished record */
/**********************/

if (key == D26_I04_NO_KEY)   /*  no key (key = isrecnum)                 */
{                            /*  get searched isrecnum                   */
     isrecnum = rec_no;
}                            

while                   /*  correct record isn't found                     */
      (read_rec == TRUE)
{                       /*  read record using isread                       */
   /***************/
   /* read record */
   /***************/
   rcode = isread(d26_file_dnlist[sch], dnlistrec, mode);
   while                /*  record is locked                               */
	 ((rcode < 0) && (iserrno == ELOCKED))
   {                    /*  sleep 1 second                                 */
      sleep (D26_I15_SLEEP_SECS);
			/*  try to read record again                       */
      rcode = isread (d26_file_dnlist[sch], dnlistrec, mode);
   }                    /*  record is locked */


   if (rcode < 0)       /*  isread error occurred                          */
   {                    /*  MA_LOG isam error                              */
	i = ldlong(&dnlistrec[D26_I12_OCCURRENCY]);

	read_rec = FALSE;

			/*  set return_value                               */
	switch (iserrno) /* errorcode from isread                          */
	{
	case EENDFILE:   /* end of file                                    */
	case ENOREC:     /* no such record                                 */
	     return_value = D26_I14_NOT_FOUND;
	     break;
	default:         /* other C isam error occured                     */
	     DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
		      svc_c_debug1,GDS_S_ISREAD_ERR, mode, iserrno));
	     return_value = D26_ERROR;
	     break;
	}                /* errorcode from isread                          */

	break;          /*  leave loop !!!!!                               */

   }                    /*  isread error occurred */

   /******************************************/
   /* check whether correct record was found */
   /******************************************/
			/*  check whether the wished record was found      */
   switch ((int) key)           /* given key                               */
   {
   case D26_I04_NO_KEY:         /* no key (key = isrecnum)                 */
   case D26_I06_KEY_OCCURRENCY: /* occurrency                              */
				/* correct record was found                */
	read_rec = FALSE;    /* don't read next record                  */
	break;

   case D26_I05_KEY_DNINTANDDN: /* dn interptretation and dn               */
				/* check whether correct record was        */
				/* found                                   */
	if                        /* key_value is shorter than key length  */
	   (ln_save_key < MAXKEYSIZE)
	{                         /* correct record was read               */
	     read_rec = FALSE; /* don't read next record                */
	}
	else
	{                         /* compare save_key value with record    */
				  /* value                                 */
	     if                   /* value matches within key length       */
		((strncmp (save_key,
			   ptr_dnlistrec,
			   MAXKEYSIZE)) == 0)
	     {                    /* compare rest of save_key value with   */
				  /* rest of record value                  */
		  if              /* rest of value matches with record value*/
		     ((strcmp (ptr_m_save_key, ptr_m_dnlistrec)) == 0)
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
				 d26_file_dnlist[sch])) < 0)
			    {   
				 return (D26_ERROR);
			    } 
			}               /* read record is locked           */

		  }               /* rest of values matches                */
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
		       if ((D26_ISRELEASE(svc_c_sev_warning,
			    d26_file_dnlist[sch])) < 0)
		       {   
			    return (D26_ERROR);
		       } 
		  }               /* read record is locked                 */
	     }                    /* value matches within key length       */
	}                         /* key value is shorter than key length  */

	break;

   default:                     /* error which never occurrs               */
	return (D26_ERROR);
   }               /* given key                                       */


}                       /*  read record using isread                       */


return (return_value);

}                               /*  d26_t27_read_dnlistrec            */


/*exon ****************************************************************/
/*                                                                    */
/* TYPE:          C FUNCTION                                          */
/*                                                                    */
/* NAME:        : d26_t28_fill_charfield                              */
/*                                                                    */
/* AUTHOR:        Schreck, D AP 11                                    */
/* DATE:          21.01.88                                            */
/*                                                                    */
/* SYNTAX:    EXPORT  Short d26_t28_fill_charfield (field, contents,  */
/*                                    ln_field, fill_character)       */
/*                                                                    */
/*            String   field;             <-                          */
/*            String   contents;          ->                          */
/*            Short    ln_field;          ->                          */
/*            Char     fill_character;    ->                          */
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
/*     Short      D2_NO_ERR       no error occurred                   */
/*                                                                    */
/*                D26_CHANGED     the value of contents was shortened */
/*                                to ln_field                         */
/*                                                                    */
/* STANDARD-HEADER :                                                  */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                                                                    */
/* GLOBAL DATA (updated):                                             */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*           strncpy                                                  */
/*           strcpy                                                   */
/*           strlen                                                   */
/*                                                                    */
/* NOTES: This function is usefull to fill character fields of isam   */
/*        records up to their length with a defined value.            */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_t28_fill_charfield 
(
  char    *field,
  char    *contents,
  signed16 ln_field,
  char     fill_character
)

{                                  /*  d26_t28_fill_charfield              */

/**********************************************************************/
/*                                                                    */
/*               T Y P E  - D E F I N I T I O N S                     */
/*                                                                    */
/**********************************************************************/





/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

/*****  AUTOMATIC  *****/
 signed16   return_value;

/*****  FAST-AUTOMATIC  *****/
register  signed16    counter;
register  signed16    ln_contents;    /* calculated length of contents      */


/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

if ((ln_contents = strlen (contents)) > ln_field)
{    /* contents is longer than field ==> contents is shortened */
     strncpy (field, contents, ln_field);
     return_value = D26_CHANGED;  /* indicates that value was shortened */
}
else
{ /* contents is shorter than field            */
     /* ==> field is filled with fill_characters  */
     strcpy (field, contents);

     /* fill with fill_characters   */
     for (counter = ln_contents; counter < ln_field; counter++)
     { 
	field[counter] = fill_character;
     } 

     return_value = D2_NO_ERR;
} 

return (return_value);
}   

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t29_fill_dnint                                    */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        21.01.88                                              */
/*                                                                    */
/* SYNTAX:      EXPORT Short d26_t29_fill_dnint (from_dnint, to_dnint)*/
/*                                                                    */
/*              D2_dnint   from_dnint;    ->                          */
/*              D2_dnint   *to_dnint;     <-                          */
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
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_t29_fill_dnint
(
  D2_dnint   from_dnint,
  D2_dnint   *to_dnint
)

{                               /*  d26_t29_fill_dnint                   */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

/* AUTO      ...    */
 signed16   return_value;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/


switch ((int) from_dnint)
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
     /* assume single object */
     *to_dnint = D2_SOBJ;
     return_value = D26_CHANGED;
     break;
}   

return (return_value);

}                               /*  d26_t29_fill_dnint                */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t30_lock_isamfile                                 */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        22.01.88                                              */
/*                                                                    */
/* SYNTAX:      EXPORT Short d26_t30_lock_isamfile (isfd)             */
/*                                                                    */
/*              Fildes  isfd;    ->                                   */
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
/*       Short  D2_NO_ERR       no error occured                      */
/*                                                                    */
/*              D26_ERROR       C isam error occured  (except ELOCKED)*/
/*                              (s. NOTE)                             */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*           islock                                                   */
/*           sleep                                                    */
/*                                                                    */
/* NOTES: The function works in the following way:                    */
/*        The function try to lock the file. When the file is locked  */
/*        the function goes in a loop until the file is unlocked. So  */
/*        the isam error code ELOCKED never cancels the function.     */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_t30_lock_isamfile (int isfd)

{                               /*  d26_t30_lock_isamfile             */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

/* AUTO      ...    */
 signed16  return_value = D2_NO_ERR;

/* FASTAUTO  ...    */
register signed32 rcode;    /* return code of C isam function               */

extern int islock(int isfd);

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

rcode = islock (isfd);           /*  try to lock isam file              */
while                            /*  file is locked                     */
      ((rcode < 0) && (iserrno == ELOCKED))
{                                
   sleep (D26_I15_SLEEP_SECS);   /*  sleep 1 second                     */
   rcode = islock (isfd);        /*  try to lock isamfile again         */
}                                /*  file is locked   */


if (rcode < 0)                   /*  islock error occurred              */
{                                /*  MA_LOG isam error                  */
     DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
       svc_c_debug1,GDS_S_ISLOCK_ERR, iserrno));
     return_value = D26_ERROR;   
}                                /*  islock error occurred */

return (return_value);           
}                                /*  d26_t30_lock_isamfile             */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t31_rep_dnlistrec                                 */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        22.01.88                                              */
/*                                                                    */
/* SYNTAX:      EXPORT d26_t31_rep_dnlistrec (old_dn, new_dn, changed,*/
/*                                             sch)                   */
/*                                                                    */
/*              D2_name_string    old_dn;    ->                       */
/*              D2_name_string    new_dn;    ->                       */
/*              Bool              *changed;  <-                       */
/*              Int               sch;       ->                       */
/*                                                                    */
/* DESCRIPTION: This function searches the records with the old dn    */
/*              and all possible dn interpretation vaules. When       */
/*              record(s) exists the old dn is replaced by the new dn */
/*              and the record is rewrited into the file.             */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*    D2_name_string      old_dn          the value of the old dn     */
/*                                                                    */
/*    D2_name_string      new_dn          the value of the new dn     */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*    Bool                changed         indicator whether isam file */
/*                                        was changed                 */
/*                               D2_TRUE  isam file was changed       */
/*                               D2_FALSE isam file wasn't changed    */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*    Short   D2_NO_ERR          no error occurred                    */
/*                                                                    */
/*            D26_ERROR          C isam error occurred                */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*    LOCAL Fildes d26_file_dnlist[]  filedescriptor for dn list in   */
/*                                    file                            */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*           isrewcurr                                                */
/*           isunlock                                                 */
/*                                                                    */
/*           d26_t27_read_dnlistrec                                   */
/*           d26_t28_fill_charfield                                   */
/*           d26_t29_fill_dnint                                       */
/*           d26_t30_lock_isamfile                                    */
/*           d26_m21_logg_dnlist                                      */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_t31_rep_dnlistrec
(
  D2_name_string old_dn,
  D2_name_string new_dn,
  boolean       *changed,
  signed32       sch
)

{                               /*  d26_t31_rep_dnlistrec             */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

			   /* possible values of dn interpretation         */
static  D2_dnint    dnint_values[D26_I17_NO_DNINT_VAL] =
				 { D2_SOBJ,
				   D2_SUBTREE,
				   D2_GROUP };

/* AUTO      ...    */
			      /* record buffer of isam file            */
 char      dnlistrec[D26_I02_RECDNLISTL];
 char      normed_new_dn[D2_DNL_MAX];

 char      *dnintptr,         /* ptr to dn interpretation              */
	   *dnptr,            /* ptr to normed DN                      */
	   *deptr;            /* ptr to exact DN                       */
 signed16     return_value = D2_NO_ERR;

/* FASTAUTO  ...    */
register signed16 rcode,      /* return_value of read function         */
		  i;          /* counter                               */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

/*******************/
/* lock whole file */
/*******************/

if                                /*  lock of isam file fails              */
  (d26_t30_lock_isamfile (d26_file_dnlist[sch]) == D26_ERROR)
{    
     return (D26_ERROR);          
}                                 /*  lock of isam file fails              */

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
   d26_t29_fill_dnint (dnint_values[i], dnintptr); /*  dn interpretation   */
						   /*  119 bytes dn name   */
   d26_t28_fill_charfield (dnptr, (char *) old_dn, D2_DNL_MAX, D2_EOS);

			   /*  search record without lock                  */
   rcode = d26_t27_read_dnlistrec (D26_I05_KEY_DNINTANDDN,dnlistrec,
				   FALSE, sch);

   switch ((int) rcode)    /*  return value of read dnlistrec              */
   {
   case D26_I13_FOUND:     /*  record was found                            */
	                   /*  update record value: new dn name            */
	d27_007_norm_name((char *) new_dn,strlen((char *) new_dn)+1,
			  (char *) normed_new_dn);
	d26_t28_fill_charfield (dnptr, normed_new_dn, MAXKEYSIZE - CHARSIZE,
				D2_EOS);
			   /*  update record value: new exact dn name      */
	d26_t28_fill_charfield(deptr,(char *) new_dn,D2_DNL_MAX,D2_EOS);
			   /*  rewrite record                              */
	if ((D26_ISREWCURR(svc_c_sev_warning,d26_file_dnlist[sch], dnlistrec)) 
		< 0)
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
   }                       /*  rcode */
}                          /*  all possible dn int. values */

if ((D26_ISUNLOCK(svc_c_sev_warning,d26_file_dnlist[sch])) < 0)
{                  /* isam error occurred                          */
     return_value = D26_ERROR; /* set return value                 */
}                  /* isam error occured                           */

return (return_value);     
}                               /*  d26_t31_rep_dnlistrec             */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t32_add_dnlistrec                                 */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        25.01.88                                              */
/*                                                                    */
/* SYNTAX:      EXPORT Short d26_t32_add_dnlistrec (dnint, dn,        */
/*                            occurrency, dnlistcoreidx, sch)         */
/*                                                                    */
/*              D2_dnint           dnint;             ->              */
/*              D2_name_string     dn;                ->              */
/*              Long               occurency;         ->              */
/*              Long               *dnlistcoreidx;    <>              */
/*              Int                sch;               ->              */
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
/*      D2_name_string    dn               distinguished name         */
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
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*    LOCAL Fildes d26_file_dnlist[]  filedescriptor for dn list in   */
/*                                    file                            */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*        isrewcurr                                                   */
/*        iswrite                                                     */
/*        isunlock                                                    */
/*        stlong                                                      */
/*        ldlong                                                      */
/*        sprintf                                                     */
/*        d26_t27_read_dnlistrec                                      */
/*        d26_t28_fill_dnint                                          */
/*        d26_t29_fill_charfield                                      */
/*        d26_t30_lock_isamfile                                       */
/*        d26_m21_logg_dnlist                                         */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_t32_add_dnlistrec
(
  D2_dnint       dnint,
  D2_name_string dn,
  signed32       occurrency,
  signed32      *dnlistcoreidx,
  signed32       sch
)

{                               /*  d26_t32_add_dnlistrec             */
/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

			   /* record buffer of isam file                   */
 char      dnlistrec[D26_I02_RECDNLISTL];
 char      normed_new_dn[D2_DNL_MAX];

 char      *dnintptr,         /* ptr to dn interpretation              */
	   *dnptr,            /* ptr to normed DN                      */
	   *deptr,            /* ptr to exact DN                       */
	   *ptroccurrency;    /* ptr to occurrency                     */
 signed16  return_value = D2_NO_ERR;
 boolean   rewrite = FALSE,   /* indicator for rewrite record         */
	   found = FALSE;     /* indicator for record exists          */
 signed32  new_occurrency;
 signed32  max_short;         /* variable for max. index check        */

/* FASTAUTO  ...    */
register signed16     rcode;  /* return code of called functions      */
register signed32     i;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

/*******************/
/* lock whole file */
/*******************/

if                                /*  lock of isam file fails              */
  (d26_t30_lock_isamfile (d26_file_dnlist[sch]) == D26_ERROR)
{                                 /*  MA-LOG + MA_ERROR                    */
     return (D26_ERROR);          
}                                 /*  lock of isam file fails              */

/************************/
/* initialize variables */
/************************/

dnintptr = &dnlistrec[D26_I10_DNINT];
dnptr    = &dnlistrec[D26_I11_DNNAME];
deptr    = &dnlistrec[MAXKEYSIZE];
ptroccurrency = &dnlistrec[D26_I12_OCCURRENCY];
new_occurrency = (signed32) abs ((signed32) occurrency);
d27_007_norm_name((char *) dn, strlen((char *) dn)+1,normed_new_dn);

/***************/
/* read record */
/***************/

if (*dnlistcoreidx >= 0)          /*  record exists                        */
{                                 /*  read record via isrecnum             */
     isrecnum = *dnlistcoreidx + D26_I03_GET_CORE_INDEX; /*  set isrecnum  */
				  /*  read record                          */
     rcode = d26_t27_read_dnlistrec (D26_I04_NO_KEY,
				     dnlistrec,
				     FALSE, sch); /* no lock because file is */
						/* locked                  */
     switch ((int) rcode)         /*  return code from read                */
     {
     case D26_I13_FOUND:          /*  record was found                     */
	                          /*  set indicator for rewrite and found  */
	  found = rewrite = TRUE;
	  break;
     case D26_I14_NOT_FOUND:      /*  record wasn't found                  */
     default:                     /*  C isam error occurred                */
	  return_value = D26_ERROR;
	  break;
     }                            /*  return code from read */
}
else
{                                 /*  unknown whether record exists        */
				  /*  try to read record via given dnint   */
				  /*  and dn                               */
				  /*  fill key value dnint and dn          */
     d26_t29_fill_dnint (dnint, dnintptr); /*  dn interpretation           */
					   /*  dn name                     */
     d26_t28_fill_charfield (dnptr, normed_new_dn, D2_DNL_MAX,
			     D2_EOS);

				  /*  read record                          */
     rcode = d26_t27_read_dnlistrec (D26_I05_KEY_DNINTANDDN,
				     dnlistrec,
				     FALSE, sch); /* no lock because file is */
						/* locked                  */

     switch ((int) rcode)         /*  return code from read                */
     {
     case D26_I13_FOUND:          /*  record was found                     */
	                          /*  set indicator for rewrite and found  */
	  found = rewrite = TRUE;
	  break;
     case D26_I14_NOT_FOUND:      /*  record wasn't found                  */
	                          /*  set indicator for record doesn't     */
				  /*  exist (found)                        */
	  found = FALSE;
				  /*  try to read record with occurrency   */
				  /*  = 0                                  */
				  /*  set key value                        */
	  stlong(0, ptroccurrency);
				  /*  read record                          */
	  rcode = d26_t27_read_dnlistrec (D26_I06_KEY_OCCURRENCY,
					  dnlistrec,
					  FALSE, sch); /* no lock because    */
						     /* file is locked     */
	  switch ((int) rcode)    /*  return code from read                */
	  {
	  case D26_I13_FOUND:     /*  record was found                     */
	                          /*  set indicator for rewrite            */
	       rewrite = TRUE;
	       break;
	  case D26_I14_NOT_FOUND: /*  record wasn't found                  */
	       rewrite = FALSE;
	       break;
	  default:                /*  C isam error occurred                */
	       return_value = D26_ERROR;
	       break;
	  }                       /*  return code from read */
	                          
	  break;
     default:                     /*  C isam error occurred                */
	  return_value = D26_ERROR;
	  break;
     }                            /*  return code from read */
}                                 /*  record exists */

if (return_value == D2_NO_ERR)    /*  no error occurred                    */
{                                 /*  fill in new record values            */
     /*****************************/
     /* fill in new record values */
     /*****************************/

     if (found == TRUE)        /*  record with given dnint and dn exists*/
     {                            /*  set new occurrency                   */
	  i = ldlong(ptroccurrency);
	  new_occurrency = new_occurrency + i;
     }
     else
     {                            /*  record with given dnint and dn       */
				  /*  doesn't exist                        */
				  /*  fill in dnint                        */
	  d26_t29_fill_dnint (dnint, dnintptr);
				  /*  fill in dnname                       */
	  d26_t28_fill_charfield (dnptr, normed_new_dn,
				  MAXKEYSIZE - CHARSIZE, D2_EOS);
	  d26_t28_fill_charfield (deptr, (char *) dn, D2_DNL_MAX, D2_EOS);
     }                            /*  record exists                        */

				  /*  fill in new occurrency               */
     stlong(new_occurrency, ptroccurrency);

     /*****************************/
     /* write record in isam file */
     /*****************************/

     if (rewrite == TRUE)      /*  rewrite record                       */
     {                            /*  rewrite record                       */
	  if ((D26_ISREWCURR(svc_c_sev_warning,d26_file_dnlist[sch], 
		dnlistrec)) < 0)
	  {                       /* isam error occurred                   */
	       return_value = D26_ERROR; /* set return value               */
	  }                       /* isam error occured                    */
     }
else
{                            /*  write record                         */
	  if ((D26_ISWRITE(svc_c_sev_warning,d26_file_dnlist[sch], 
		dnlistrec)) < 0)
	  {                       /* isam error occurred                   */
	       return_value = D26_ERROR; /* set return value               */
	  }                       /* isam error occured                    */
     }                            /*  rewrite record */

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
	  }                       /*  max. index exceeded */
     }   /* return_value == D2_NO_ERR */
}                                 /*  no error occurred */

/********************/
/* unlock isam file */
/********************/
				   /*  unlock of isam file fails           */
if (D26_ISUNLOCK(svc_c_sev_warning,d26_file_dnlist[sch]) < 0) 
{    
     return_value = D26_ERROR;     /*  set return_value                    */
}                                  /*  unlock of isam file fails */

return (return_value);

}                               /*  d26_t32_add_dnlistrec             */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t33_del_dnlistrec                                 */
/*                                                                    */
/* AUTHOR:      Schreck, D AP 11                                      */
/* DATE:        25.01.88                                              */
/*                                                                    */
/* SYNTAX:      EXPORT Short d26_t33_del_dnlistrec (dnlistcoreidx,    */
/*                                              occurrency, sch)      */
/*                                                                    */
/*              Long    dnlistcoreidx;       ->                       */
/*              Long    occurrency;          ->                       */
/*              Int     sch;                 ->                       */
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
/*    Short   D2_NO_ERR          no error occurred                    */
/*                                                                    */
/*            D26_ERROR          C isam error occurred                */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*    LOCAL Fildes d26_file_dnlist[]  filedescriptor for dn list in   */
/*                                    file                            */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*           isrewcurr                                                */
/*           isrelease                                                */
/*           ldlong                                                   */
/*           stlong                                                   */
/*           sprintf                                                  */
/*                                                                    */
/*           d26_t27_read_dnlistrec                                   */
/*           d26_m21_logg_dnlist                                      */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_t33_del_dnlistrec 
(
signed32    dnlistcoreidx,
signed32    occurrency,
signed32    sch
)

{                               /*  d26_t33_del_dnlistrec             */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

			      /* record buffer of isam file            */
 char      dnlistrec[D26_I02_RECDNLISTL];

 char      *ptroccurrency;    /* ptr to dn occurrency                  */
 signed16  return_value = D2_NO_ERR;
 signed32  abs_occurrency,    /* absolute value of given occurrency    */
	   new_occurrency,    /* new value of occurrency               */
	   old_occurrency;    /* old occurrency value                  */

/* FASTAUTO  ...    */
register signed16 rcode;      /* return_value of read function         */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

/************************/
/* initialize variables */
/************************/

ptroccurrency = &dnlistrec[D26_I12_OCCURRENCY];
abs_occurrency = (signed16) abs ((signed32) occurrency);

/*******************/
/* read old record */
/*******************/

if (dnlistcoreidx >= 0) /*  correct index given                      */
{                            /*  read record with lock via isrecnum        */
     isrecnum = dnlistcoreidx + D26_I03_GET_CORE_INDEX;  /*  set isrecnum  */
			     /*  read record                               */
     rcode = d26_t27_read_dnlistrec(D26_I04_NO_KEY,dnlistrec,TRUE,sch);

     switch ((int) rcode)    /*  return value of read dnlistrec            */
     {
     case D26_I13_FOUND:     /*  record was found                          */
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
	  if ((D26_ISREWCURR(svc_c_sev_warning,d26_file_dnlist[sch], 
		dnlistrec)) < 0)
	  {                  /* isam error occurred                        */
	       return_value = D26_ERROR; /* set return value               */
	  }                  /* isam error occured                         */
			     /*  unlock record                             */


	  if ((D26_ISRELEASE(svc_c_sev_warning,d26_file_dnlist[sch])) < 0)
	  {                /* isam error occurred                        */
	     return_value = D26_ERROR; /* set return value               */
	  }                /* isam error occured                         */
	  break;

     case D26_I14_NOT_FOUND: /*  no record found                           */
	                     /*  error: data inconsistency of dnlis in isam*/
	  return_value = D26_ERROR;
	  break;

     default:                /* C isam error occured                       */
	  return_value = D26_ERROR; /* set return_value                    */
	  break;
       }                       /*  rcode */
}
else
{                          /*  parameter error                             */
     return_value = D26_ERROR; /*  set return_value                        */
}                          /*  dnlistcoreidx >= D2_NULL    */

return (return_value);     
}                               /*  d26_t33_del_dnlistrec             */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_t34_attr_search(...)                            */
/*                                                                    */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 17.12.87                                            */
/*                                                                    */
/* SYNTAX       : Short d26_t34_attr_search (sch, attr_typ)           */
/*                Int       sch;                                      */
/*                D2_a_type *attr_typ;                                */
/*                                                                    */
/* DESCRIPTION  : This function will look for the given attribute.    */
/*                The at is sorted in increasing order.               */
/*                (criterion: attribute-id)                           */
/*                A binary search will be used.                       */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*                  D2_a_type *attr_typ;  attribute-id                */
/*                                                                    */
/* OUTPUT PARAMETERS: -                                               */
/*                                                                    */
/* RETURN VALUE :   >= 0:           index of attribute in AT          */
/*                  D26_ERROR:      attribute not found               */
/*                                                                    */
/* STANDARD HEADER :                                                  */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*                                                                    */
/* GLOBAL DATA (updated):                                             */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*              d27_303_cmp_obj_id()                                  */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_t34_attr_search
(
  signed32       sch,
  D2_a_type *attr_typ
)

{                                   /*  d26_t34_attr_search()         */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

register signed32      cmp;         /* indicates which attribute type */
				    /* is smaller                     */
register signed32      high;        /* right branch                   */
register signed32      low;         /* left branch                    */
register signed32      mid;         /* middle of interval             */
register D26_at        *at;         /* pointer to attribute table     */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

                                    /*  determine high and low value  */
				    /*  of interval to be searched    */
low = 0;
high = d26_noat[sch] - 1;
while                               /*  interval not empty            */
      (low <= high)
{                                   /*  take middle of interval       */
  mid = (low + high) / 2;
  at = d26_at[sch] + mid;
  if                                /*  attribute type < type in AT   */
    ((cmp = d27_303_cmp_obj_id(attr_typ,&at->d26a_type)) < 0)
  {                                 /*  high value = middle - 1       */
    high = mid - 1;
  }
else if                         /*  attr-Id > actual value        */
	(cmp > 0)
  {                                 /*  low value = middle + 1        */
    low = mid + 1;
  }
else
{                                 /*  return(attr-index)            */
    return(mid);
  }                                 
}                                   

				    /*  return (D26_ERROR)            */
return (D26_ERROR);
}                                   /*  d26_t34_attr_search()         */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t35_select_key                                    */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        21.01.88                                              */
/*                                                                    */
/* DESCRIPTION: This function builds an  ISAM key assigned to a speci-*/
/*              fied attribute.                                       */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              pos         Short           position in record to use */
/*                                          for index                 */
/*              at          D26_at *        attribute description     */
/*              filenr      Short           number of C-Isam-file     */
/*                                          present only if master-   */
/*                                          knowledge attribute  must */
/*                                          be part of the index      */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              key         D26_keydesc *   pointer to ISAM-key       */
/*                                                                    */
/* RETURNVALUE: None                                                  */
/*                                                                    */
/* STANDARD-HEADER: None                                              */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*                  d26_at      D26_at[]    Attribute table           */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                  d26_u03_attr_search()                             */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

void d26_t35_select_key
(
  signed32     sch,
  D26_keydesc *key,
  signed16     pos,
  signed16     at_idx,
  signed16     filenr
)

{                               /*  d26_t35_select_key                */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

			    /* type of aliased object attribute       */
static D2_a_type alob_type = {sizeof(D2_ALIASED_OBJECT) - 1,
			     (Ob_id_string) D2_ALIASED_OBJECT};

			    /* type of master knowledge attribute     */
static D2_a_type mk_type =   {sizeof(D2_MASTER_KNOWLEDGE) - 1,
			     (Ob_id_string)D2_MASTER_KNOWLEDGE};

 signed16 phon_len;        /* length of phonetical part of attribute */
 signed16 rep_len;         /* length for representation              */
 signed16 idx;             /* index of master-knowledge in at        */
 signed16 len;             /* length of keypart                      */
 signed16 max_key_len;     /* length of all keyparts without name    */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

max_key_len = INTSIZE + INTSIZE + LONGSIZE + LONGSIZE;
key->k_flags  = ISDUPS + COMPRESS;
key->k_nparts = 5;

				/*  set keypart object number         */
key->k_part[0].kp_start = LONGSIZE;
key->k_part[0].kp_leng  = INTSIZE;
key->k_part[0].kp_type  = INTTYPE;

				/*  set keypart namepart  value       */
key->k_part[1].kp_start = pos + INTSIZE;
phon_len = d26_at[sch][at_idx].d26a_phon == TRUE ? D26_LPHON : 0;
rep_len  = d26_at[sch][at_idx].d26a_rep == D2_T61_PR ? INTSIZE : 0;
key->k_part[1].kp_start += phon_len + rep_len;

len = d26_at[sch][at_idx].d26a_ubound + 1;
key->k_part[1].kp_leng = len + max_key_len > MAXKEYSIZE ?
			 MAXKEYSIZE - max_key_len : len;
key->k_part[1].kp_type  = CHARTYPE;

			/*  add aliased object part                   */
idx = d26_t34_attr_search(sch, &alob_type);
key->k_part[2].kp_start =
     d26_at[sch][idx].d26a_field_pos[filenr];
key->k_part[2].kp_leng  = INTSIZE;
key->k_part[2].kp_type  = INTTYPE;

			/*  add master knowledge                      */
idx = d26_t34_attr_search(sch, &mk_type);
key->k_part[3].kp_start = d26_at[sch][idx].d26a_field_pos[filenr] + INTSIZE;
key->k_part[3].kp_leng  = LONGSIZE;
key->k_part[3].kp_type  = LONGTYPE;

			/*  add serial number part                    */
key->k_part[4].kp_start = 0;
key->k_part[4].kp_leng  = LONGSIZE;
key->k_part[4].kp_type  = LONGTYPE;

}                       /*  d26_t35_select_key                        */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t37_get_dninfo()                                  */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        02.02.88                                              */
/*                                                                    */
/* DESCRIPTION: This function fills the structure dninfo from record  */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*              record      String          ISAM record containing    */
/*                                          the object.               */
/*              np_count    Short           count of nameparts in root*/
/*                                          object.                   */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*              dninfo      D26_dninfo *    pointer to DN information */
/*              dname       D2_name_string *pointer where DN has to   */
/*                                          be written to             */
/*                                                                    */
/* RETURNVALUE: None                                                  */
/*                                                                    */
/* STANDARD-HEADER: None                                              */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*              d26_srt              structure rule table            */
/*              d26_at                attribute table                 */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                  d27_301_oid_str()                                 */
/*                                                                    */
/*exoff ***************************************************************/

void  d26_t37_get_dninfo
(
  String          record,
  D26_dninfo     *dninfo,
  D2_name_string *dname,
  signed16        np_count
)

{                               /*  d26_t37_get_dninfo                */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E - D E F I N I T I O N S                    */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

    signed16    root;   /* object number of root object           */
    signed16    ob_nr;  /* object number of read object           */
    signed16    par_nr; /* parent number of read object           */
    D26_srt     *srt;   /* pointer to SRT                         */
    D26_at      *at;    /* pointer to AT                          */
    D26_rdninfo *rdn;   /* pointer to RDN                         */
    signed16    pos;    /* position in record                     */
    char        separator;  /* separator character between type   */
			    /* and value                              */

register signed16    i;         /* loop variable                      */
register D26_nam_at *nam_at;    /* loop pointer to naming attributes  */
register D26_ava    *ava;       /* loop pointer to AVAs               */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

par_nr = ob_nr = ldint(record + LONGSIZE);

root = np_count > 0 ? dninfo->d26_rdn[np_count - 1].d26_r_idx : D26_ROOT;

for                     /*  all ancestors                             */
   (i = 0, par_nr = ob_nr;
    (par_nr = d26_srt[D26_OLD][par_nr].d26s_par_nr) != root; i++)
{                       /*  count                                     */
}                       

dninfo->d26_rd_anz = np_count + i + 1;

for                     /*  all ancestors                             */
   (par_nr = ob_nr, rdn = dninfo->d26_rdn + np_count + i,
    srt = d26_srt[D26_OLD] + ob_nr; i >= 0;
    i--, srt = d26_srt[D26_OLD] + par_nr, rdn--)
{                       /*  assign srt-indices                        */
  rdn->d26_r_idx  = par_nr;
  for                   /*  all naming attributes of SRT entry        */
     (rdn->d26_r_nava = 0, ava = rdn->d26_r_ava, nam_at = srt->d26s_nam_at;
      nam_at->d26s_index != -1; rdn->d26_r_nava++, ava++, nam_at++)
  {                     /*  assign attribute index and type           */
    ava->d26_a_idx  = nam_at->d26s_index;
    par_nr          = srt->d26s_par_nr;
  }                     
}                       

for                     /*  all RDN's                                 */
   (i = 0, rdn = dninfo->d26_rdn + np_count;
    np_count + i < dninfo->d26_rd_anz; i++, rdn++)
{                       /*  copy values from record                   */
  srt = d26_srt[D26_OLD] + rdn->d26_r_idx;
  for                   /*  all naming attributes of SRT entry        */
     (ava = rdn->d26_r_ava, nam_at = srt->d26s_nam_at;
      nam_at->d26s_index != -1; ava++, nam_at++)
  {                     /*  convert object identifier                 */
    at = d26_at[D26_OLD] + nam_at->d26s_index;
    d27_301_oid_str(&at->d26a_type,(char *) *dname);
    *dname += strlen((char *) *dname);
    pos = nam_at->d26s_position[dninfo->d26_filenr] + INTSIZE +
				(at->d26a_phon == TRUE ? D26_LPHON : 0);
    switch((int)at->d26a_rep)
    {
      case D2_T61_PR:
	pos += INTSIZE;
	/* FALLTHROUGH */
      case D2_T61:                 /* analog to d27_007_norm_name */
      case D2_PRINTABLE:           /* analog to d27_007_norm_name */
	separator = D2_T61_REP;
	break;
      case D2_NUMERIC:
	separator = D2_NUM_REP;
	break;
      case D2_IA5:
	separator = D2_IA5_REP;
	break;
    }   
    sprintf((char *) *dname,"%c%s%c",separator,record + pos,D2_AVA_SEP);
    ava->d26_a_val = ++*dname;
    ava->d26_a_len = strlen((char *) *dname) - 1;
    *dname += ava->d26_a_len + 1;
  }                     
  *(*dname - 1) = D2_RDN_SEP;
}                       
*(*dname - 1) = D2_EOS;

}                       /*  d26_t37_get_dninfo                        */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_t38_get_name(...)                               */
/*                                                                    */
/* AUTHOR       : Loose  AP 11                                        */
/* DATE         : 28.4.92                                             */
/*                                                                    */
/* SYNTAX       : Short d26_t38_get_name (sch, dn, dninfo)            */
/*                Int             sch;                                */
/*                D2_name_string  dn;                                 */
/*                D26_dninfo     *dninfo;                             */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function verifies a given distinguished name for validity */
/*      using the scheme given in sch.                                */
/*      This means, all distinguished name parts must be IA5-strings, */
/*      all attribute identifiers must by in the predefined range and */
/*      the distinguished name must meet the form:                    */
/*                                                                    */
/*      <rdn1>\1<rdn2>\1...<rdn<n>>\0                                 */
/*                                                                    */
/*      The RDNs must meet the form:                                  */
/*                                                                    */
/*      <ava1>\2<ava2>\2...<ava<m>>                                   */
/*                                                                    */
/*      The AVAs must meet the form:                                  */
/*                                                                    */
/*      <type>\3<value>                                               */
/*                                                                    */
/*      All attribute types are checked until the sequence of RDNs is */
/*      hurt. In this case D26_CONCATENATION will be returned (because*/
/*      the Distinguished Name may be an alias; the application has to*/
/*      manage the rest of the string).                               */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*      sch         Int                 identifier for schema         */
/*      dn          D2_name_string      distinguished name            */
/*      dninfo      D26_dninfo *        pointer to "dninfo"-structure */
/*                                      where the RDNs could be stored*/
/*                                      (dninfo->d26_rd_anz must be   */
/*                                      set)                          */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*      dninfo      D26_dninfo          RDNs in the form              */
/*                                        - index of RDN in srt       */
/*                                        - number of AVAs            */
/*                                        - array of AVAs             */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NO_ERR           Short       no error occurred             */
/*      D2_NMSYNTAX_ERROR               name syntax error             */
/*      D26_CONCATENATION               DN may be an alias name       */
/*                                      (name is partly checked)      */
/*                                                                    */
/* STANDARD HEADER :                                                  */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*      d26_srt             D26_srt     SRT                           */
/*      d26_nosrt           Short       number of entries in SRT      */
/*      d26_at              D26_at      AT                            */
/*                                                                    */
/* GLOBAL DATA (updated):                                             */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*      d26_t4_rdn_search()                                          */
/*      d26_t34_attr_search()                                         */
/*      d27_302_str_oid()                                             */
/*      strlen()                                                      */
/*      atol()                                                        */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_t38_get_name
(
  signed32        sch,
  D2_name_string  dn,
  D26_dninfo     *dninfo
)

{                                   /*  d26_t38_get_name()            */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

				    /* memory for converted object    */
				    /* identifiers                    */
static   byte           ob_ids[D2_NO_AVA][D2_OBJ_IDL_MAX];
				    /* memory for naming attr. types  */
static   D2_a_type      nam_type[] =
			    {
			      {0,(Ob_id_string)ob_ids[0]},
			      {0,(Ob_id_string)ob_ids[1]},
			      {0,(Ob_id_string)ob_ids[2]}
			    };

     D2_name_string comp;       /* pointer to components of DN    */
     D26_ava        *ava;       /* pointer to AVA in RDN          */
     char           separator;  /* separator between type and     */
				/* value                          */
     signed16          len;     /* length of a component          */

     D26_rdninfo    *rdninfo;   /* pointer to RDN                 */
     signed16       par_idx;    /* SRT index of parent of RDN     */
     D26_at         *at;        /* pointer to AT entry            */

register D2_name_string dnp;    /* loop pointer to DN             */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

par_idx = dninfo->d26_rd_anz == 0 ?
	  D26_ROOT : dninfo->d26_rdn[dninfo->d26_rd_anz - 1].d26_r_idx;

for                     /*  all characters of the name                */
   (dnp = comp = dn, rdninfo = dninfo->d26_rdn + dninfo->d26_rd_anz,
    rdninfo->d26_r_nava = 0, ava = rdninfo->d26_r_ava, ava->d26_a_idx = -1;
    ; dnp++)
{                       /*  check if end of attribute type is reached */
  if                    /*  end of attribute type is reached          */
    (*dnp == D2_T61_REP || *dnp == D2_PRTBL_REP || *dnp == D2_NUM_REP ||
     *dnp == D2_IA5_REP)
  {                     /*  check whether object identifier has       */
			/*  illegal length                            */
    len = dnp - comp;
    if                  /*  length is illegal                         */
      (len == 0 || len > D2_NP_ID_LEN)
    {                   /*  return syntax error                       */
      return(D2_NMSYNTAX_ERROR);
    }
else
{                   /*  replace delimiter by EOS                  */
      separator = *dnp;
      *dnp = D2_EOS;
      if                /*  object identifier cannot be converted     */
	(d27_302_str_oid((char *) comp,nam_type + rdninfo->d26_r_nava) < 0)
      {                 /*  return name syntax error                  */
	*dnp = separator;
	return(D2_NMSYNTAX_ERROR);
      }
else if         /*  attribute type is not in AT               */
	   ((ava->d26_a_idx = d26_t34_attr_search(sch,nam_type +
	    rdninfo->d26_r_nava)) < 0)
      {                 /*  return syntax error                       */
	*dnp = separator;
	return(D2_NMSYNTAX_ERROR);
      }                 
      *dnp = separator;
      comp = dnp + 1;
    }                   
  }                     /*                                            */
  if                    /* end of AVA is reached                      */
    (*dnp == D2_AVA_SEP || *dnp == D2_RDN_SEP || *dnp == D2_EOS)
  {                     /* set length and value in AVA                */
    if                  /*  still attribute type not complete         */
      (ava->d26_a_idx == -1)
    {                   /*  return error                              */
      return(D2_NMSYNTAX_ERROR);
    }                   
    ava->d26_a_len = dnp - comp;
    ava->d26_a_val = comp;
    at = d26_at[sch] + ava->d26_a_idx;
    if                  /*  attribute value has illegal length or     */
			/*  syntax                                    */
      (ava->d26_a_len < at->d26a_lbound ||
       ava->d26_a_len > at->d26a_ubound ||
       d26_t39_syntax_check(at,(char *) ava->d26_a_val,ava->d26_a_len) 
	!=  D2_NO_ERR)
    {                   /*  return error                              */
      return(D2_NMSYNTAX_ERROR);
    }                   
    rdninfo->d26_r_nava++;
    if                  /*  still AVA following                       */
      (*dnp == D2_AVA_SEP)
    {                   /*  check the number of AVAs                  */
      if                /*  number of AVAs exceeds limit              */
	(rdninfo->d26_r_nava >= D2_NO_AVA)
      {                 /*  return syntax error                       */
	return(D2_NMSYNTAX_ERROR);
      }                 /*  reset the component pointer               */
      ava++;
      ava->d26_a_idx = -1;
    }                   
    if                  /*  end of RDN is reached                     */
      (*dnp == D2_RDN_SEP || *dnp == D2_EOS)
    {                   /*  check the concatenation                   */
      if                /*  no ruling structure rule found            */
	((rdninfo->d26_r_idx = d26_t40_rdn_search(sch,rdninfo,par_idx)) < 0)
      {                 /*  return syntax error                       */
	return(D26_CONCATENATION);
      }
else
{                 /*  reset the parent index                    */
	par_idx = rdninfo->d26_r_idx;
      }                 
      dninfo->d26_rd_anz++;
      if                /*  still RDN following                       */
	(*dnp == D2_RDN_SEP)
      {                 /*  check the number of RDNs                  */
	if              /*  number of RDNs exceeds limit              */
	  (dninfo->d26_rd_anz >= D2_NP_MAX)
	{               /*  return syntax error                       */
	  return(D2_NMSYNTAX_ERROR);
	}               /*  reset the component pointer               */
	rdninfo++;
	ava = rdninfo->d26_r_ava;
	ava->d26_a_idx = -1;
	rdninfo->d26_r_nava = 0;
      }
else
{                 /*  end of name is reached, break the loop    */
	break;
      }                 
    }                   
    comp = dnp + 1;
  }                     
}                       

len = dnp - dn;

if                      /*  length of DN is illegal                   */
  (len == 0 || len > D2_DNL_MAX)
{                       /*  return error                              */
  return(D2_NMSYNTAX_ERROR);
}                       

return(D2_NO_ERR);
}                                   /*  d26_t38_get_name()            */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_t39_syntax_check(...)                           */
/*                                                                    */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 19.02.88                                            */
/*                                                                    */
/* SYNTAX       : Short d26_t39_syntax_check(at, val1, length)        */
/*                D26_at    *at;                                      */
/*                char     *val1;                                     */
/*                Ushort    length;                                   */
/*                                                                    */
/* DESCRIPTION  : This function will look whether the syntax of the   */
/*                given attribute is correct.                         */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*                D26_at   *at;         pointer to at                 */
/*                Char     *val1;       attribute value               */
/*                Ushort    length;     attribute length              */
/*                                                                    */
/* OUTPUT PARAMETERS: -                                               */
/*                                                                    */
/* RETURN VALUE :   D2_NO_ERR:      attribute syntax is correct       */
/*                  D26_ERROR:      attribute syntax is not correct   */
/*                                                                    */
/* STANDARD HEADER :                                                  */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/* GLOBAL DATA (updated):                                             */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*      D2_ATTCMP()                                                   */
/*      d27_601_is_T61String()                                        */
/*      d27_605_is_PrtString()                                        */
/*                                                                    */
/* REMARKS :                                                          */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_t39_syntax_check
(
  D26_at  *at,
  char    *val1,
  signed16   length
)

{                                   /*  d26_t39_syntax_check()        */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

register Octet_string    val = (Octet_string) val1;

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

switch((int) at->d26a_syntax)      /*  syntax is                     */
{
  case D2_CEXACT:                   /*  Case Exact String             */
  case D2_CIGNORE:                  /*  Case Ignore String            */
		if                  /*  If not T61String              */
		  (d27_601_is_T61String (val,length) == D2_ERROR)
		{                   /*  return(D26_ERROR)             */
		  return(D26_ERROR);
		}                   
		break;
  case D2_IA5:
		if                  /*  If not IA5String              */
		  (d27_606_is_IA5String (val,length) == D2_ERROR)
		{                   /*  return(D26_ERROR)             */
		  return(D26_ERROR);
		}                   
		break;
  case D2_PRINTABLE:                /*  Printable String              */
		if                  /*  If not Printable String       */
		  (d27_605_is_PrtString (val, length) == D2_ERROR)
		{                   /*  return(D26_ERROR)             */
		  return(D26_ERROR);
		}                   
		break;
  case D2_COUNTRY_STX:              /*  Country-Syntax                */
		if                  /*  length != 2                   */
		   (length != 2)
		{                   /*  return(D26_ERROR)             */
		  return(D26_ERROR);
		}
else
{                   
		  if                /*  If not country-syntax         */
		    (d27_608_is_country (val) == D2_ERROR)
		  {                 /*  return(D26_ERROR)             */
		    return(D26_ERROR);
		  }                 
		}                   
		break;
  default:                          /*  otherwise                     */
		return(D26_ERROR);
}                                   

return (D2_NO_ERR);
}                                   /*  d26_t39_syntax_check()        */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_t40_rdn_search(...)                             */
/*                                                                    */
/* AUTHOR       : Loose, D AP 11                                      */
/* DATE         : 30.4.92                                             */
/*                                                                    */
/* SYNTAX       : Short d26_t40_rdn_search(sch,rdn,par_idx)           */
/*                                                                    */
/*                Int         sch;                                    */
/*                D26_rdninfo *rdn;                                   */
/*                Short       par_idx;                                */
/*                                                                    */
/* DESCRIPTION  : This function will look whether the RDN is known in */
/*                the SRT with the given identifier.                  */
/*                The naming attributes in one SRT entry are sorted.  */
/*                (criterion: attribute id)                           */
/*                The srt is sorted in increasing order.              */
/*                (criterion: attribute ids of the RDNs               */
/*                 lexicographically)                                 */
/*                                                                    */
/* INPUT PARAMETERS:                                                  */
/*      sch         Int             scheme identifier                 */
/*      rdn         D26_rdinfo *    relative distinguished name       */
/*      par_idx     Short           index of superior RDN in the SRT  */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      >= 0        Short           index of ruling SRT entry         */
/*      D26_ERROR                   any error occurred                */
/*                                                                    */
/* STANDARD HEADER :                                                  */
/*                                                                    */
/* GLOBAL DATA (read only):                                           */
/*      D26_srt     d26_srt[sch]    pointer to SRT                    */
/*      Short       d26_nosrt[sch]  number of entries in SRT          */
/*                                                                    */
/* GLOBAL DATA (updated):                                             */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*              d27_303_cmp_obj_id()                                  */
/*                                                                    */
/* REMARKS :                                                          */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_t40_rdn_search
(
signed32     sch,
D26_rdninfo *rdn,
signed16     par_idx
)

{                                   /*  d26_t40_rdn_search()          */

/**********************************************************************/
/*                                                                    */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*                                                                    */
/**********************************************************************/

/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

register signed16    i;                 /* loop variable              */
register D26_srt     *srt, *max_srt;    /* loop pointers to SRT       */
register D26_nam_at  *nam_at;           /* loop pointer to naming     */
					/* attribute structure in SRT */
register D26_ava     *ava;              /* loop pointer to naming     */
					/* AVA in RDN                 */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

for                                 /*  all entries in srt            */
    (srt = d26_srt[sch], max_srt = srt + d26_nosrt[sch];
     srt < max_srt; srt++)
{                                   /*  scan the naming attributes    */
  for                               /*  all naming attributes of the  */
				    /*  SRT entry                     */
     (i = 0, ava = rdn->d26_r_ava, nam_at = srt->d26s_nam_at;
      i < rdn->d26_r_nava; i++, ava++, nam_at++)
  {                                 /*  check for end of list in SRT  */
				    /*  entry                         */
    if                              /*  end of list is reached        */
      (nam_at->d26s_index == -1)
    {                               /*  SRT "smaller" than RDN,       */
				    /*  continue with next SRT entry  */
      break;
    }
else if                       /*  RDN "smaller" than SRT        */
	 (ava->d26_a_idx < nam_at->d26s_index)
    {                               /*  return (D26_ERROR)            */
      return (D26_ERROR);
    }
else if                       /*  SRT "smaller" than RDN,       */
	 (ava->d26_a_idx > nam_at->d26s_index)
    {                               /*  continue with next SRT entry  */
      break;
    }                               
  }                                 

  if                                /*  SRT entry was found, which    */
				    /*  shares the naming attributes  */
    (i == rdn->d26_r_nava)
  {                                 /*  check whether it has still    */
				    /*  more types                    */
    if                              /*  still types following in SRT  */
      (nam_at->d26s_index != -1)
    {                               /*  RDN "smaller" than SRT entry  */
				    /*  return error                  */
      return (D26_ERROR);
    }
else
{                               /*  look for parent index         */
      if                            /*  parent index found            */
	 (par_idx == srt->d26s_par_nr)
      {                             /*  return (object index)         */
	return ((signed16) (srt - d26_srt[sch]));
      }                             
    }                               
  }                                 
}                                   

				    /*  return (D26_ERROR)            */
return(D26_ERROR);

}                                   /*  d26_t40_rdn_search()          */

/**********************************************************************/
/*                                                                    */
/* TYPE         : C-FUNKTION                                          */
/*                                                                    */
/* NAME         : d26_t41_setup_attr_len                              */
/*                                                                    */
/* AUTHOR       : Loose, SNI BU BA KP 13                              */
/* DATE         : 15.10.93                                            */
/*                                                                    */
/* SYNTAX       : */ static  signed16 d26_t41_setup_attr_len(      /* */
/*                */                        D26_at    *atp,        /* */
/*                */                        Attr_info *attr_len)   /* */
/*                                                                    */
/* DESCRIPTION  : This function fills the Attr_info structure for     */
/*                adding an index (derived from gi12 in d26gutilis.c).*/
/*                                                                    */
/* INPUT-PARAMETER:                                                   */
/*                D26_at    *atp                                      */
/*                                                                    */
/* OUTPUT-PARAMETER:                                                  */
/*                Attr_info *attr_len                                 */
/*                                                                    */
/* RETURNVALUE     :                                                  */
/*      D2_NO_ERR   short           No error occurred                 */
/*      D26_ERROR                   Error occurred                    */
/*                                                                    */
/**********************************************************************/

{                       /* d26_t41_setup_attr_len                   */
/**********************************************************************/
/*                                                                    */
/*                    D E C L A R A T I O N S                         */
/*                                                                    */
/**********************************************************************/

signed16      val_len;     /* length of a particular attr value          */

/**********************************************************************/
/*                                                                    */
/*                    C O D E                                         */
/*                                                                    */
/**********************************************************************/

				/* type for C-ISAM index if required  */
    attr_len->idx_type = CHARTYPE;

    switch (atp->d26a_syntax)
    {

	case D2_CEXACT:         /* <value>'\0'                        */
	case D2_PRINTABLE:
	case D2_TIME:
	    val_len = atp->d26a_ubound + 1;
	    attr_len->idx_len = val_len;
	    break;

	case D2_CIGNORE:        /* <norm value>'\0'<exact value>'\0'  */
	case D2_IA5:
	case D2_NUMERIC:
	case D2_TELNR_STX:
	case D2_COUNTRY_STX:
	    val_len = 2 * (atp->d26a_ubound + 1);
	    attr_len->idx_len = val_len / 2;
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
		attr_len->idx_len  = val_len;
		attr_len->idx_type = LONGTYPE;
	    }
	    else
	    {              /* <value><length>                    */
		val_len = atp->d26a_ubound + INTSIZE;
		attr_len->idx_len = val_len - INTSIZE;
	    }
	    break;

	case D2_MHS_DLSP_STX:
	case D2_MHS_ORADR_STX:
	case D2_MHS_ORNAME_STX:
			   /* <normed val><length><exact val><length> */
	    val_len = 2 * (atp->d26a_ubound + INTSIZE);
	    attr_len->idx_len = val_len / 2 - INTSIZE;
	    break;
	case D2_TLXNR_STX:
	    val_len = atp->d26a_ubound + D26_MAXVALPARTS;
	    attr_len->idx_len = val_len;
	    break;

	case D2_CIGN_LIST:      /* <r1><nel1>'\0'..<r10><nel10>'\0'*/
	case D2_POST_AD_STX:    /* <r1><eel1>'\0'..<eel10>'\0'*/
	    val_len = 2 * (atp->d26a_ubound + 2 * D26_MAXVALPARTS);
	    attr_len->idx_len = val_len / 2;
	    break;

	case D2_BOOLEAN:        /* <value>                            */
	case D2_INTEGER:
	    val_len = LONGSIZE;
	    attr_len->idx_len  = val_len;
	    attr_len->idx_type = LONGTYPE;
	    break;

	case D2_PR_DM_STX:      /* <value 1>...<value 10>             */
	case D2_MHS_PR_DM_STX:
	    val_len = LONGSIZE * atp->d26a_ubound;
	    attr_len->idx_len = val_len;
	    break;

	case D2_DISTNAME:       /* <DN index>                         */
	    val_len = LONGSIZE;
	    attr_len->idx_len  = val_len;
	    attr_len->idx_type = LONGTYPE;
	    break;

	case D2_ACCL:           /* <index 1>...<index 20>             */
	    if (atp->d26a_nr_recur != 1)
		return(D26_ERROR);
	    val_len = LONGSIZE * 20;
	    attr_len->idx_len = val_len;
	    break;
    }                           /* end switch (syntax)                */

    if (attr_len->idx_len > MAX_PART_SIZE)
	attr_len->idx_len = MAX_PART_SIZE;

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
	attr_len->len = val_len;
    else                        /* for an objects<> file, record      */
				/* <count> + length of all recurring  */
				/* values.                            */
	attr_len->len = INTSIZE + atp->d26a_nr_recur * val_len;

return(D2_NO_ERR);

}                       /* d26_t41_setup_attr_len                   */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_t42_add_index                                     */
/*                                                                    */
/* AUTHOR:      Loose, SNI BU BA KP 13                                */
/* DATE:        15.10.93                                              */
/*                                                                    */
/* DESCRIPTION: The function looks in the at_idx_table for new        */
/*              indizes and calls the add_index-function.             */
/*              The code is overtaken from d26gutilis.c, gi17 + gi32. */
/*                                                                    */
/* INPUT-PARAMETERS: None                                             */
/*                                                                    */
/* OUTPUT-PARAMETERS: None                                            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*     D2_ERROR                                                       */
/*     D2_NOERROR                                                     */
/*                                                                    */
/* STANDARD-HEADER:                                                   */
/*                                                                    */
/* GLOBAL  DATA  (read only):                                         */
/*              at_idx_table                                          */
/*                                                                    */
/* GLOBAL  DATA  (updated):                                           */
/*                                                                    */
/* CALLED FUNCTIONS/PROGRAMS/MAKROS:                                  */
/*                                                                    */
/* NOTES:                                                             */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_t42_add_index(
  signed16   at_idx,
  signed16   fnr, 
  int  isfd,
  boolean    add_idx) 

{                                               /*  d26_t42_add_index */
/**********************************************************************/
/*                                                                    */
/*                  D E C L A R A T I O N S                           */
/*                                                                    */
/**********************************************************************/

/* for index in attribute files */
static struct keydesc search_key =
    {ISDUPS + COMPRESS,3,
     {{0,INTSIZE,INTTYPE},{0,0,0},{INTSIZE,LONGSIZE,LONGTYPE}}};

			/* ordinary key on attribute                  */
static struct keydesc key =
	    {ISDUPS + COMPRESS,3,{{LONGSIZE,INTSIZE,INTTYPE},{0,0,0},
				 {0,LONGSIZE,LONGTYPE}}};

signed16        i;              /* loop variable                      */
signed16        start_pos;      /* start position of indexed value    */
Attr_info      *attr_len;       /* structure containing length etc.   */
Attr_info       attr_info;      /* structure containing length etc.   */
D26_at         *atp;            /* pointer to AT                      */
signed16  rest_len;        /* remaining length of attribute value        */
signed16  one_val_len;     /* attr_len includes space for recurring valus*/
			   /* one_val_len is the length of one value.    */
char   fname[D27_LFILE_NAME];
char   log_comment[100];
				/* pointer to appropr.  function         */
int    (*isadd_or_del_index)(int, struct keydesc *); 

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

atp = d26_at[D26_NEW] + at_idx;
attr_len = &attr_info;
d26_t41_setup_attr_len(atp, attr_len);

if (add_idx)
  isadd_or_del_index = isaddindex;
else
  isadd_or_del_index = isdelindex;

if                    /* attribute is in attribute file             */
  (atp->d26a_filenr >= 0)
{
  /* code like in gi17 in d26gutilis.c */
  sprintf(fname, "%s%s%d",d26_install[D26_OLD],D26_ATTR,fnr);
  search_key.k_part[1].kp_type = attr_len->idx_type;
  search_key.k_part[1].kp_leng = attr_len->idx_len;

  search_key.k_part[1].kp_start = ISAM_HDR;
  if                    /* attribute allows phonetical comparison     */
    (atp->d26a_phon == TRUE)
  {                     /* reset start for keypart                    */
    search_key.k_part[1].kp_start += D26_LPHON;
  }
  if                    /* attribute syntax is Case ignore or case    */
    (atp->d26a_syntax == D2_CEXACT || atp->d26a_syntax == D2_CIGNORE)
  {                     /* reset start for keypart                    */
      search_key.k_part[1].kp_start += INTSIZE;
  }
  if                    /* index cannot be generated                  */
    (isadd_or_del_index(isfd,&search_key) < 0)
  {                     /* check the error                            */
    if                  /* C-ISAM index did not exist before          */
      (add_idx && iserrno != EKEXISTS || !add_idx)
    {
      sprintf(log_comment,"isfd: %d: %s  idx\nkeypart[1]: %d %d %d",
	      isfd, add_idx ? "add" : "del",
	      search_key.k_part[1].kp_start,search_key.k_part[1].kp_leng,
	      search_key.k_part[1].kp_type);
      DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
      DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
	svc_c_debug1,GDS_S_ISADDINDEX_ERR, iserrno));
      return(D26_ERROR);
    }
  }

} else {
  start_pos = atp->d26a_field_pos[fnr];
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
	(isadd_or_del_index(isfd, &key) < 0)
      {                 /* check reason                               */
	if              /* index did not exist before                 */
	  (add_idx && iserrno != EKEXISTS || !add_idx)
	{               /* log error                                  */
          sprintf(log_comment,"isfd: %d: %s  idx\nphon keypart[1]: %d %d %d",
	      isfd, add_idx ? "add" : "del",
	      search_key.k_part[1].kp_start,search_key.k_part[1].kp_leng,
	      search_key.k_part[1].kp_type);
          DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
          DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
	    svc_c_debug1,GDS_S_ISADDINDEX_ERR, iserrno));
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

    key.k_part[1].kp_type = attr_len->idx_type;
    key.k_part[1].kp_leng = attr_len->idx_len;
		      /* Add index for one attr value               */
    if                /* index cannot be added                      */
      (isadd_or_del_index(isfd,&key) < 0)
    {                 /* check the error                            */
      if              /* index did not exist before                 */
	(add_idx && iserrno != EKEXISTS || !add_idx)
      {               /* log the error                              */
          sprintf(log_comment,"isfd: %d: %s  idx\nnormal keypart[1]: %d %d %d",
	      isfd, add_idx ? "add" : "del",
	      search_key.k_part[1].kp_start,search_key.k_part[1].kp_leng,
	      search_key.k_part[1].kp_type);
          DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
          DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
	    svc_c_debug1,GDS_S_ISADDINDEX_ERR, iserrno));
	return(D26_ERROR);
      }
    }
			/* add a secondary index for ordering matching*/
    if (atp->d26a_syntax == D2_OCTET || atp->d26a_syntax == D2_TIME ||
	atp->d26a_syntax == D2_INTEGER)
    {                 /* reset keypart type                         */
      key.k_part[1].kp_type += ISDESC;

      if              /* error from adding index                    */
	(isadd_or_del_index(isfd, &key) < 0)
      {               /* check the error                            */
	if            /* index did not exist before                 */
	  (add_idx && iserrno != EKEXISTS || !add_idx)
	{             /* log the error                              */
          sprintf(log_comment,"isfd: %d: %s  idx\ndescend keypart[1]: %d %d %d",
	      isfd, add_idx ? "add" : "del",
	      search_key.k_part[1].kp_start,search_key.k_part[1].kp_leng,
	      search_key.k_part[1].kp_type);
          DCE_SVC_LOG((GDS_S_TRF_DEBUG_INFO_MSG, log_comment));
          DCE_SVC_LOG((DCE_SVC(gds_svc_handle,"%d"),GDS_S_GENERAL,
	    svc_c_debug1,GDS_S_ISADDINDEX_ERR, iserrno));
	  return(D26_ERROR);
	}
      }
    }

			/* set start position to next recurring value */
    key.k_part[1].kp_start += rest_len;
  }
}

return(D2_NO_ERR);
}                                               /*  d26_t42_add_index */

