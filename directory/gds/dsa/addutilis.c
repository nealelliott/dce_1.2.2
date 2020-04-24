/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: addutilis.c,v $
 * Revision 1.1.10.2  1996/02/18  19:44:37  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:25:41  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:41:54  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:05:58  root]
 * 
 * Revision 1.1.8.3  1994/05/10  15:51:38  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:18:41  marrek]
 * 
 * Revision 1.1.8.2  1994/03/23  15:08:59  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:19:11  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  16:04:53  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:06:56  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  13:24:52  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  12:58:14  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:58:19  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:17:32  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  19:31:08  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:01:12  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:43:51  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:52:12  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char addutilis_rcsid[] = "@(#)$RCSfile: addutilis.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:44:37 $";
#endif

/****************************************************************************
*                                                                           *
*        COPYRIGHT (C) SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991          *
*                             ALL RIGHTS RESERVED                           *
*                                                                           *
****************************************************************************/

/*daton	***************************************************************/
/*								      */
/* TYPE         : MODUL                                               */
/*								      */
/* NAME         : d26isadd.c    <DS_ADD_OBJECT>                       */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 23.12.87                                            */
/*								      */
/* COMPONENT    : DS                                                  */
/*								      */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*			.					      */
/*			.					      */
/*								      */
/* PRD#/VERS.   :                                                     */
/*								      */
/* DESCRIPTION  :                                                     */
/*								      */
/* SYSTEM DEPENDENCIES: SINIX (--> C-ISAM)                            */
/*								      */
/* HISTORY      :                                                     */
/*								      */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    |15.01.88 |  Original                      | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*								      */
/*                     Module Identification                          */
/*								      */
/**********************************************************************/


/**********************************************************************/
/*								      */
/*			  I N C	L U D E	S			      */
/*								      */
/**********************************************************************/

#include <d26dsa.h>

/*****  external Includes  *****/

#include <dce/d27util.h>
#include <malgcfe.h>

/*****  internal Includes  *****/

#include <d26schema.h>
#include <d26isam.h>
#include <d26svcis.h>

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

extern int      iserrno;            /* C-ISAM error code              */

/**********************************************************************/
/*								      */
/*                D E C L A R A T I O N S         FUNCTIONS           */
/*								      */
/**********************************************************************/


/******** EXPORT ********/
signed16 d26_750_create_obj(signed32 *, D26_dninfo *, D23_at_type *,
			D23_at_type *, D26_dns *[], D23_at_type **,
			D26_attr_info *, signed16 *, signed32 *);
signed16 d26_751_create_rec_attr(signed16, signed16, signed32, 
			D26_attr_info *, D23_at_type **, signed16 *);
signed16 d26_754_create_alias(signed16, D26_dninfo *, D26_attr_info *,
			signed16, signed32, signed32);

/******** LOCAL  ********/
static  signed16 d26_755_insert_dn(signed16 oct_idx, D26_dninfo *dn_info);

/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : d26_750_create_obj(...)                             */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 15.01.88                                            */
/*								      */
/* SYNTAX       : Short d26_750_create_obj(oct_idx, dn_info, acl_attr,*/
/*                                         mk_attr, dnl_ptr, attr,    */
/*                                         at_info, anz_rec, key)     */
/*                Long           *oct_idx;                            */
/*                D26_dninfo     *dn_info;                            */
/*                D23_at_type    *acl_attr;                           */
/*                D23_at_type    *mk_attr;                            */
/*                D26_dns        *dnl_ptr[];                          */
/*                D23_at_type   **attr;                               */
/*                D26_attr_info  *at_info;                            */
/*                Short          *anz_rec;                            */
/*                Long           *key;                                */
/*								      */
/* DESCRIPTION  : This function will build the C-ISAM record using    */
/*                the Distinguished Name Information and the given    */
/*                Attribute Information.                              */
/*								      */
/* INPUT PARAMETERS:                                                  */
/*                Long           *oct_idx;  OCT index of new object   */
/*                D26_dninfo     *dn_info;  Distinguished Name Info   */
/*                D23_at_type    *acl_attr; pointer to  ACL-attribute */
/*                D23_at_type    *mk_attr;  pointer to MASTER-        */
/*                                          KNOWLEDGE attribute       */
/*                D26_dns        *dnl_ptr[];pointer list pointing to  */
/*                                          ACL-Shorts and MASTER-DSA */
/*                D23_at_type   **attr;     pointer list  pointing to */
/*                                          attribute in ADD-OBJECT-  */
/*                                          Invoke-Message            */
/*                D26_attr_info  *at_info;  attribute info (index of  */
/*                                          attribute in AT)          */
/*                Short          *anz_rec;  number of reccurring      */
/*                                          values for each attribute */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*                Long           *key;      C-ISAM key of created     */
/*                                          object                    */
/*								      */
/* RETURN VALUE :   D2_NO_ERR:      no error occured                  */
/*                  D26_ERROR:      creation of new object failed     */
/*								      */
/* STANDARD HEADER :                                                  */
/*								      */
/* GLOBAL DATA (read only):                                           */
/*                  D26_oct     *d26_oct;   pointer to OCT            */
/*                  D26_at      *d26_at;    pointer to AT             */
/*                  D26_fildes  *d26_ofildes; Description for object  */
/*                                            files                   */
/*                  Int          iserrno;   C-ISAM error code         */
/*                  Int          d26_oreclen; length of CISAM record  */
/*                                            for objects             */
/*                                                                    */
/* GLOBAL DATA (updated):                                             */
/*                  String       d26_ob_record; C_ISAM record to be   */
/*                                              filled                */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                          d26_i15_get_record_pos()                  */
/*                          d26_i16_insert_val()                      */
/*                          d26_755_insert_dn()                       */
/*                          stint()                                   */
/*                          stlong()                                  */
/*                          iswrite()                                 */
/*                          isuniqueid()                              */
/*								      */
/* REMARKS :                                                          */
/*								      */
/*exoff	***************************************************************/

signed16 d26_750_create_obj
(
  signed32       *oct_idx,
  D26_dninfo     *dn_info,
  D23_at_type    *acl_attr,
  D23_at_type    *mk_attr,
  D26_dns        *dnl_ptr[],
  D23_at_type   **attr,
  D26_attr_info  *at_info,
  signed16       *anz_rec,
  signed32       *key
)

{                                   /*  d26_750_create_obj()          */

/**********************************************************************/
/*								      */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

 static char function[] = "d26_750_create_obj";
 String           max_record;   /* pointer to end of ISAM record  */
 String           pos;          /* actual position in record      */

 signed16         i;
 signed16         idx;          /* AT-index                       */

 int              isfd;         /* C-ISAM file descriptor         */
 D26_at          *at;           /* pointer to AT entry            */
 D2_obj_id        oid_master_kn;/* define of type, length         */
				/* D2_MASTER_KNOWLEDGE            */
 D2_obj_id        oid_acl;      /* define of type, length D2_ACL  */

 boolean          is_ocl;       /* indicates whether object class */
				/* is inserted                    */
 unsigned16       rep;          /* value representation           */
 signed16         filenr = (d26_oct + *oct_idx)->d26c_filenr;

register signed16 length;       /* attribute length               */
register signed16 anz;          /* number of attribute occurrencies */
register char    *val;          /* attribute value                */
register D23_av_value *av;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

				    /*  initialize C-ISAM record      */
for (pos = d26_ob_record, max_record = d26_ob_record + d26_oreclen;
     pos < max_record; )
{ 
  *pos++ = D2_EOS;
} 
				    /*  insert DN in record           */
if                                  /*  operation is not successful   */
   (d26_755_insert_dn((signed16)*oct_idx, dn_info))
{                                   /*  return(D26_ERROR)             */
  return(D26_ERROR);
}                                   /*                                */

for                                 /*  all attributes                */
    ( ; at_info->d26_at_idx != D26_EOL; at_info++, attr++, anz_rec++)
{                                   /*                                */
  at = d26_at + at_info->d26_at_idx;
				    /*  get position of counter for   */
				    /*  number of attribute values    */
  pos = d26_i15_get_record_pos(d26_ob_record, 1, at, d26_oct + *oct_idx,
			       (D26_nam_at *) NULL, TRUE);
				    /*  get old number of attribute values */
  anz = ldint(pos);
				    /*  store new number of attr. values */
  stint(anz + *anz_rec, pos);
  if                                /*  attr. is no (real) recurring one */
     (at->d26a_filenr < 0)
  {                                 /*  get position of first value   */
    pos = d26_i15_get_record_pos(d26_ob_record, anz + 1, at,
				 d26_oct + *oct_idx, (D26_nam_at *) NULL,
				 FALSE);
    av = (D23_av_value *) ((char *) *attr + D23_S_ATT + (*attr)->d23_atoff +
			   D23_S_AV);

    is_ocl = (D2_ATTCMP(&(at->d26a_type),D2_OBJ_CLASS) == 0);
    val = is_ocl ? (char *) oct_idx : (char *) av + D23_S_ATV;
    for                             /*  all attribute occurrencies    */
       (anz = 1, length = av->d23_avlen; ; )
    {                               /*  insert attribute in record    */
      rep = av->d23_avrep;
      if                            /*  operation not successful      */
	 (d26_i16_insert_val(at, rep, val, length, dnl_ptr,
			     dnl_ptr + D2_ACLIDX_MAX, &pos))
      {                             /*  return(D26_ERROR)             */
	return(D26_ERROR);
      } else if                     /*  all occurrencies inserted     */
	    (anz >= *anz_rec)
      {                             /*  break                         */
	break;
      } else {                      /*  take next occurrency          */
	av = (D23_av_value *) (val + av->d23_avoff);
	anz++;
	val = is_ocl ? val + sizeof(signed32) : (char *) av + D23_S_ATV;
	length = av->d23_avlen;
      }                             /*                                */
    }                               /*                                */
  }                                 /*                                */
}                                   /*                                */

if                                  /*  master Knowledge is absent    */
   (mk_attr == (D23_at_type *) NULL)
{   
  oid_master_kn.d2_type = (Ob_id_string) D2_MASTER_KNOWLEDGE ;
  oid_master_kn.d2_typ_len = sizeof(D2_MASTER_KNOWLEDGE) - 1;
				    /*  look for position of master attr. */
  if                                /*  Master Knowledge not found    */
     ((idx = d26_u03_attr_search(&oid_master_kn)) < 0)
  {                                 /*  return(D26_ERROR)             */
    return(D26_ERROR);
  } else {                          /*  get position of master DSA    */
    pos = d26_i15_get_record_pos(d26_ob_record, 1, d26_at + idx,
				 d26_oct + *oct_idx, (D26_nam_at *) NULL,
				 TRUE);
				    /*  store number of values (=1)   */
    stint(1, pos);
    pos += INTSIZE;
				    /*  put OWN DSA in record         */
    stlong( *(dnl_ptr[D2_ACLIDX_MAX]->d26_didx), pos);
  }                                 /*                                */
}                                   /*                                */

if                                  /*  ACL-attribute is absent       */
   (acl_attr == (D23_at_type *) NULL)
{   
  oid_acl.d2_type = (Ob_id_string) D2_ACL;
  oid_acl.d2_typ_len = sizeof(D2_ACL) - 1;
				    /*  look for position of master attr. */
  if                                /*  ACL not found    */
    ((idx = d26_u03_attr_search(&oid_acl)) < 0)
  {                                 /*  return(D26_ERROR)             */
    return(D26_ERROR);
  } else {                          /*  get position of ACL attribute */
    pos = d26_i15_get_record_pos(d26_ob_record, 1, d26_at + idx,
				 d26_oct + *oct_idx, (D26_nam_at *) NULL,
				 TRUE);
				    /*  store number of values (=1)   */
    stint(1, pos);
    pos += INTSIZE;
				    /*  insert ACL-Indices in record  */
    for (i = 0; i < D2_ACLIDX_MAX; i++, pos += LONGSIZE)
    { 
      stlong( *(dnl_ptr[i]->d26_didx), pos);
    } 
  }                                 /*                                */
}                                   /*                                */


				    /*  into record                   */
stint(dn_info->d26_rdn[dn_info->d26_rd_anz - 1].d26_r_idx,
      d26_ob_record + LONGSIZE);
if                                  /*  open of object file fails     */
   (d26_i23_open_file(filenr, D26_OBJ) < (int   ) 0)
{                                   /*  return(D26_ERROR)             */
  return(D26_ERROR);
} else {
				    /*  get C-ISAM key                */
   if
    (D26_ISUNIQUEID(svc_c_debug3, isfd = (d26_ofildes + filenr)->d26_isfd, 
	key))
   {
     return(D26_ERROR);
   }
				    /*  put key in record             */
   stlong(*key, d26_ob_record);
   if                               /*  ISAM-write not successful     */
     (D26_ISWRITE(svc_c_sev_warning,isfd, d26_ob_record))
   {                           	    /*  break                         */
				    /*  return(D26_ERROR)             */
     return(D26_ERROR);
   } else {                         /*  return(D2_NO_ERR);            */
     D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_WRITE,function,1,0);
     return(D2_NO_ERR);
   }
}
}                                   /*  d26_750_create_obj()          */


/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : d26_751_create_rec_attr(...)                        */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 15.01.88                                            */
/*								      */
/* SYNTAX       : Short d26_751_create_rec_attr(oct_idx, obj_file_nr, */
/*                                              key, dnl_ptr, attr,   */
/*                                              at_info, anz_rec)     */
/*                Short           oct_idx;                            */
/*                Short           obj_file_nr;                        */
/*                Long            key;                                */
/*                D26_dns        *dnl_ptr[];                          */
/*                D23_at_type   **attr;                               */
/*                D26_attr_info  *at_info;                            */
/*                Short          *anz_rec;                            */
/*								      */
/* DESCRIPTION  : This function will insert all recurring values in   */
/*                the appropriate C-ISAM file.                        */
/*								      */
/* INPUT PARAMETERS:                                                  */
/*                Short           oct_idx;  index of object of new    */
/*                                          object in OCT             */
/*                Short           obj_file_nr; file number of object  */
/*                                             (= main entry)         */
/*                Long            key;      C-ISAM key of created     */
/*                                          object the attributes     */
/*                                          belong to                 */
/*                D26_dns        *dnl_ptr[];pointer list pointing to  */
/*                                          ACL-Shorts                */
/*                D23_at_type   **attr;     pointer list  pointing to */
/*                                          attribute in ADD-OBJECT-  */
/*                                          Invoke-Message            */
/*                D26_attr_info  *at_info;  attribute info (index of  */
/*                                          attribute in AT)          */
/*                Short          *anz_rec;  number of reccurring      */
/*                                          values for each attribute */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*								      */
/* RETURN VALUE :   D2_NO_ERR:      no error occured                  */
/*                  D26_ERROR:      any error occurred                */
/*								      */
/* STANDARD HEADER :                                                  */
/*								      */
/* GLOBAL DATA (read only):                                           */
/*                  D26_at     *d26_at;     pointer to AT             */
/*                  D26_fildes *d26_afildes;Description for attribute */
/*                                          files                     */
/*                  Int          iserrno;   C-ISAM error code         */
/*                  Int          d26_areclen; length of CISAM record  */
/*                                            for recurring attributes*/
/*                                                                    */
/* GLOBAL DATA (updated):                                             */
/*                  String       d26_a_record;  C_ISAM record to be   */
/*                                              filled                */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                          d26_i16_insert_val()                      */
/*                          d26_i23_open_file()                       */
/*                          sprintf()                                 */
/*                          stint()                                   */
/*                          stlong()                                  */
/*                          iswrite()                                 */
/*                          isrelease()                               */
/*								      */
/* REMARKS :                                                          */
/*								      */
/*exoff	***************************************************************/

signed16 d26_751_create_rec_attr
(
  signed16        oct_idx,
  signed16        obj_file_nr,
  signed32        key,
  D26_attr_info  *at_info,
  D23_at_type   **attr,
  signed16       *anz_rec
)

{                                   /*  d26_751_create_rec_attr()     */

/**********************************************************************/
/*								      */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

 static char function[] = "d26_751_create_rec_attr";
 String           pos;          /* actual position in record      */
 String           fix_pos;      /* fixed position of data record  */
 int              isfd;         /* C-ISAM File Descriptor         */
				/* C-ISAM File-Name               */
 D26_at          *at;           /* pointer to AT entry            */
 unsigned16       rep;          /* attribute value representation */

register signed16     length;       /* attribute length               */
register signed16     anz;          /* number of attribute occurrencies */
register char        *val;          /* attribute value                */
register D23_av_value *av;          /* pointer to attribute value in  */
				    /* message                        */
register short     iw_count = 0;    /* number of iswrite-calls        */

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

for                                 /*  all attributes                */
    ( ; at_info->d26_at_idx != D26_EOL; at_info++, attr++, anz_rec++)
{                                   /*                                */
  at = d26_at + at_info->d26_at_idx;

  if                                /*  attribute is a (real) recurring one*/
     (at->d26a_filenr >= 0)
  {                                 /*  get C-ISAM file descriptor    */
    if                              /*  operation fails               */
       ((isfd = d26_i23_open_file(at->d26a_filenr, D26_ATT)) < 0)
    {                               /*  continue with next attribute type */
      continue;
    } else {                        /*  put object file number in record */
      stint(obj_file_nr, d26_a_record);
				    /*  put object key in record      */
      stlong(key, d26_a_record + INTSIZE);

      fix_pos = d26_i15_get_record_pos(d26_a_record, 1, at,
				 d26_oct + oct_idx, (D26_nam_at *) NULL,
				 FALSE);

      av = (D23_av_value *) ((char *) *attr + D23_S_ATT +
			     (*attr)->d23_atoff + D23_S_AV);
      for                           /*  all attribute occurrencies    */
	 (anz = 1, val = (char *) av + D23_S_ATV, length = av->d23_avlen; ; )
      {                             /*  insert attribute in record    */
	rep = av->d23_avrep;        /* rep given by user              */
	pos = fix_pos;
	if                          /*  operation is not successful   */
	   (d26_i16_insert_val(at, rep, val, length, (D26_dns **) NULL,
			       (D26_dns **) NULL, &pos))
	{                           /*  return(D26_ERROR)             */
	  return(D26_ERROR);
	}                           /*                                */

				    /*  write C-ISAM record           */
	if                          /*  operation fails               */
	   (D26_ISWRITE(svc_c_sev_warning,isfd, d26_a_record))
	{                           /*  break                         */
				    /*  continue with next attribute type */
	  continue;
	}                           /*                                */
	iw_count++;

	if                          /*  all occurrencies inserted     */
	   (anz >= *anz_rec)
	{                           /*  break                         */
	  break;
	} else {                    /*  take next occurrency          */
	  anz++;
	  av = (D23_av_value *) (val + av->d23_avoff);
	  val = (char *) av + D23_S_ATV;
	  length = av->d23_avlen;
	}                           /*                                */
      }                             /*                                */
    }                               /*                                */
  }                                 /*                                */
}                                   /*                                */
D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_WRITE,function,iw_count,0);

return(D2_NO_ERR);

}                                   /*  d26_751_create_rec_attr()     */


/*exon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : d26_754_create_alias(...)                           */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 15.01.88                                            */
/*								      */
/* SYNTAX       : Short d26_754_create_alias(srt_idx, dn_info, attr,  */
/*                                           aliased_idx,             */
/*                                           alias_class, ald_idx)    */
/*                Short           srt_idx;                            */
/*                D26_dninfo     *dn_info;                            */
/*                D26_attr_info  *attr;                               */
/*                Short           aliased_idx;                        */
/*                Long            alias_class;                        */
/*                Long            ald_idx;                            */
/*								      */
/* DESCRIPTION  : This function will build the C-ISAM record using    */
/*                the Distinguished Name Information and the given    */
/*                Attribute Information.                              */
/*								      */
/* INPUT PARAMETERS:                                                  */
/*                Short           srt_idx;  index of new DN in SRT    */
/*                                          (alias object)            */
/*                D26_dninfo     *dn_info;  Distinguished Name Info   */
/*                D26_attr_info  *attr;     attributes of alias       */
/*                Short           aliased_idx;  index of aliased      */
/*                                          object attribute in AT    */
/*                Long            alias_class; index of object class  */
/*                                          ALIAS in OCT              */
/*                Long            ald_idx;  index of aliased object   */
/*                                          in DNLIST                 */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*								      */
/* RETURN VALUE :   D2_NO_ERR:      no error occured                  */
/*                  D26_ERROR:      creation of new object failed     */
/*								      */
/* STANDARD HEADER :                                                  */
/*								      */
/* GLOBAL DATA (read only):                                           */
/*                  D26_oct     *d26_oct;   pointer to OCT            */
/*                  D26_at      *d26_at;    pointer to AT             */
/*                  D26_fildes  *d26_ofildes; Description for object  */
/*                                            files                   */
/*                  Int          iserrno;   C-ISAM error code         */
/*                  Int          d26_oreclen; length of CISAM record  */
/*                                            for objects             */
/*                                                                    */
/* GLOBAL DATA (updated):                                             */
/*                  String       d26_ob_record; C_ISAM record to be   */
/*                                              filled                */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                          d26_755_insert_dn()                       */
/*                          stint()                                   */
/*                          stlong()                                  */
/*                          iswrite()                                 */
/*                          isuniqueid()                              */
/*								      */
/* REMARKS :                                                          */
/*								      */
/*exoff	***************************************************************/

signed16 d26_754_create_alias
(
  signed16        srt_idx,
  D26_dninfo     *dn_info,
  D26_attr_info  *attr,
  signed16        aliased_idx,
  signed32        alias_class,
  signed32        ald_idx
)

{                                   /*  d26_754_create_alias()        */

/**********************************************************************/
/*								      */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

 static char function[] = "d26_754_create_alias";
 String           max_record;   /* pointer to end of ISAM record  */
 String           pos;          /* actual position in record      */

 int              isfd;         /* C-ISAm file descriptor         */
 signed32         key;          /* C-ISAM key                     */

 D26_at          *at;           /* pointer to AT entry            */
 signed16         filenr;
 signed16         oct_idx;
 D26_ocl_arc     *ocl_arc;

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/

				    /*  determine OCT-index with      */
				    /*  smallest filenr for Alias     */
				    /*  object                        */
for (ocl_arc = (d26_srt + srt_idx)->d26s_ocl_arc,
     oct_idx = ocl_arc->d26s_oclass; ocl_arc->d26s_oclass >= 0;
     ocl_arc++)
{ 
  if (d26_oct[ocl_arc->d26s_oclass].d26c_filenr <
      d26_oct[oct_idx].d26c_filenr)
  {   
    oct_idx = ocl_arc->d26s_oclass;
  } 
} 
filenr = d26_oct[oct_idx].d26c_filenr;

				    /*  initialize C-ISAM record      */
for (pos = d26_ob_record, max_record = d26_ob_record + d26_oreclen;
     pos < max_record; )
{ 
  *pos++ = D2_EOS;
} 
				    /*  insert DN in record           */
if                                  /*  operation is not successful   */
   (d26_755_insert_dn(oct_idx, dn_info))
{                                   /*  return(D26_ERROR)             */
  return(D26_ERROR);
}                                   /*                                */

for                                 /*  all attributes of ALIAS       */
    ( ; attr->d26_at_idx >= 0; attr++)
{ 
  at = d26_at + attr->d26_at_idx;
				    /*  get position of counter for   */
				    /*  number of attribute values    */
  pos = d26_i15_get_record_pos(d26_ob_record, 1, at, d26_oct + oct_idx,
			       (D26_nam_at *) NULL, TRUE);
				    /*  store number of values (=1)   */
  stint(1, pos);
  pos += INTSIZE;
  if                                /*  attribute is ALIASED_OBJECT   */
     (attr->d26_at_idx == aliased_idx)
  {   
				    /*  insert index of aliased object*/
				    /*  in record                     */
    stlong(ald_idx, pos);
  } else {                          /*  attribute is OBJECT-CLASS     */
				    /*  insert attribute in record    */
    if                              /*  operation not successful      */
       (d26_i16_insert_val(at, at->d26a_rep, (char *) &alias_class, 0,
			   NULL, NULL, &pos))
    {                               /*  return(D26_ERROR)             */
      return(D26_ERROR);
    }                               /*                                */
  }                                 /*                                */
}                                   /*                                */

				    /*  put SRT index of new object   */
				    /*  into record                   */
stint(dn_info->d26_rdn[dn_info->d26_rd_anz - 1].d26_r_idx,
      d26_ob_record + LONGSIZE);
				    /*  get C-ISAM key                */
if
  (D26_ISUNIQUEID(svc_c_debug3, isfd = (d26_ofildes + filenr)->d26_isfd, &key))
{
  return(D26_ERROR);
}
				    /*  put key in record             */
stlong(key, d26_ob_record);

if                                  /*  ISAM-write not successful     */
   (D26_ISWRITE(svc_c_sev_warning,isfd, d26_ob_record))
{
				    /*  return(D26_ERROR)             */
  return(D26_ERROR);
} else {                            /*  return(D2_NO_ERR)             */
  D27_022_ENTER_TIME_TRACE(GDS_S_FCT_IS_WRITE,function,1,0);
  return(D2_NO_ERR);
}                                   /*                                */

}                                   /*  d26_754_create_alias()        */


/*inon ****************************************************************/
/*								      */
/* TYPE         : C-FUNCTION                                          */
/*								      */
/* NAME         : d26_755_insert_dn(...)                              */
/*								      */
/* AUTHOR       : Schmid, D AP 11                                     */
/* DATE         : 03.02.88                                            */
/*								      */
/* SYNTAX       : Short d26_755_insert_dn(oct_idx, dn_info)           */
/*                Short           oct_idx;                            */
/*                D26_dninfo     *dn_info;                            */
/*								      */
/* DESCRIPTION  : This function will insert the Distinguished Name    */
/*                in the C-ISAM record.                               */
/*								      */
/* INPUT PARAMETERS:                                                  */
/*                Long            oct_idx;  index of new object in    */
/*                                          OCT                       */
/*                D26_dninfo     *dn_info;  Distinguished Name Info   */
/*                                                                    */
/* OUTPUT PARAMETERS:                                                 */
/*								      */
/* RETURN VALUE : D2_NO_ERR:    no error                              */
/*                D26_ERROR:    insertion of DN fails                 */
/*								      */
/* STANDARD HEADER :                                                  */
/*								      */
/* GLOBAL DATA (read only):                                           */
/*                  D26_srt     *d26_srt;   pointer to SRT            */
/*                  D26_oct     *d26_oct;   pointer to OCT            */
/*                  D26_at      *d26_at;    pointer to AT             */
/* GLOBAL DATA (updated):                                             */
/*								      */
/* CALLED FUNCTIONS/PROGRAMS/MACROS:                                  */
/*                          d26_i16_insert_val()                      */
/*								      */
/* REMARKS :                                                          */
/*								      */
/*inoff ***************************************************************/

static  signed16 d26_755_insert_dn
(
  signed16        oct_idx,
  D26_dninfo     *dn_info
)

{                                   /*  d26_755_insert_dn()           */

/**********************************************************************/
/*								      */
/*                 T Y P E -  D E F I N I T I O N S                   */
/*								      */
/**********************************************************************/

/**********************************************************************/
/*								      */
/*                  D E C L A R A T I O N S                           */
/*								      */
/**********************************************************************/

 D26_at          *at;           /* pointer to AT entry            */
 D26_srt         *srt;          /* pointer to SRT entry           */

 String           pos;          /* actual position in C-ISAM record */
 unsigned16       rep;          /* value representation           */

register signed16       i_ava;      /* index of ava in d26_r_ava[]    */
register D26_ava       *p_ava;      /* pointer to ava in rdn          */
register D26_rdninfo *rdn;          /* pointer to rdn                 */
register D26_rdninfo *max_rdn;      /* pointer to last rdn + 1        */

/**********************************************************************/
/*								      */
/*			      C	O D E				      */
/*								      */
/**********************************************************************/


for                                 /*  all RDNs of new object        */
    (rdn = &dn_info->d26_rdn[0], max_rdn = rdn + dn_info->d26_rd_anz;
     rdn < max_rdn; rdn++)
{                                   /*  get appropriate AT-entry of RDN */
  srt = d26_srt + rdn->d26_r_idx;
  for (i_ava = 0, p_ava = rdn->d26_r_ava; i_ava < rdn->d26_r_nava; 
                        i_ava++, p_ava++)
  { 
    at = d26_at + p_ava->d26_a_idx;
    rep = at->d26a_rep;
				      /*  get position of counter for   */
				      /*  number of (naming) attribute value */
    pos = d26_i15_get_record_pos(d26_ob_record, 1, at, d26_oct + oct_idx,
			       &srt->d26s_nam_at[i_ava], TRUE);
    stint(1, pos);
    pos += INTSIZE;

    /* Get representation of name part */
    switch(*(p_ava->d26_a_val-1))
    {
      case D2_T61_REP:    rep = D2_T61;       break;
      case D2_PRTBL_REP:  rep = D2_PRINTABLE; break;
      case D2_NUM_REP:    rep = D2_NUMERIC;   break;
      case D2_IA5_REP:    rep = D2_IA5;       break;
      default:
	   return(D26_ERROR);
    }
				      /* put RDN in record              */
    if                                /*  operation is not successful   */
       (d26_i16_insert_val(at, rep, (String) p_ava->d26_a_val,
	p_ava->d26_a_len, (D26_dns **) NULL, (D26_dns **) NULL, &pos))
    {                                 /*  return(D26_ERROR)             */
       return(D26_ERROR);
    }                                 /*                                */
  } 
}                                   /*                                */

				    /*  return(D2_NO_ERR)             */
return(D2_NO_ERR);

}                                   /*  d26_755_insert_dn()           */
