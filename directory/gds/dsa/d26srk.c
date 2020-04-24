/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26srk.c,v $
 * Revision 1.1.730.2  1996/02/18  19:46:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:27:31  marty]
 *
 * Revision 1.1.730.1  1995/12/08  15:48:07  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:07:57  root]
 * 
 * Revision 1.1.728.1  1994/02/22  17:52:53  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:18:54  marrek]
 * 
 * Revision 1.1.726.2  1993/10/14  17:16:44  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:12:15  keutel]
 * 
 * Revision 1.1.726.1  1993/10/13  17:31:13  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:29:00  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  15:25:39  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:18:09  marrek]
 * 
 * Revision 1.1.4.5  1993/02/02  14:52:22  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.4  1993/02/02  11:26:34  marrek
 * 	Introduce D2_IA5
 * 	[1993/02/01  15:03:40  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  19:42:20  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:21:08  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  20:21:17  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:16:40  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:52:13  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:58:03  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26srk.c,v $ $Revision: 1.1.730.2 $ $Date: 1996/02/18 19:46:08 $";
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
/* NAME         : d26srk.c        [d26srk]                            */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 23.08.88                                            */
/*                                                                    */
/* COMPONENT    : DSA, retrieval functions                            */
/*                                                                    */
/* DOC.-NR.     : <relevante Dokumente>                               */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* DESCRIPTION  :   The module contains the ISAM utily, that selects  */
/*                  a key for a recurring attribute file.             */
/*                                                                    */
/* SYSTEM-DEPENDENCY  :                                               */
/*                      SINIX                                         */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Datum  |  Aenderungen                   | KZ | CR# FM# */
/*          | 23.08.88| birth                          | ek |         */
/*          |         |                                |    |         */
/*          |         |                                |    |         */
/*                                                                    */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

/*****  external Includes    *****/

#include <dce/d27util.h>

/*****  internal Includes    *****/

#include <d26isam.h>

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

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

/**********************************************************************/
/*                                                                    */
/*                D E C L A R A T I O N S        FUNCTIONS            */
/*                                                                    */
/**********************************************************************/

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_i55_sel_rec_key                                   */
/*                                                                    */
/* AUTHOR:      Kraemer, D AP 11                                      */
/* DATE:        26.02.88                                              */
/*                                                                    */
/* DESCRIPTION: This function builds the ISAM key assigned to a speci-*/
/*              fied attribute.                                       */
/*                                                                    */
/* INPUT-PARAMETERS:                                                  */
/*      at          D26_at *        attribute description             */
/*      rule        Ushort          matching rule to use for index    */
/*      op_id       Ushort          indicates whether key is used for */
/*                                  read or search                    */
/*                                                                    */
/* OUTPUT-PARAMETERS:                                                 */
/*      key         D26_keydesc *   pointer to ISAM-key               */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      D2_CMP      Short           caller has to compare values,     */
/*                                  because index is not long enough  */
/*      D2_NCMP                     no comparison has to be done      */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_i55_sel_rec_key(D26_keydesc *key, D26_at *at, D2_a_match rule,
    signed16 op_id)

{                               /*  d26_i55_sel_rec_key               */

  signed16 len;             /* length of keypart                      */
  signed16 add_len;         /* length of internal added characters    */

  struct keypart *var_keypart;      /* pointer to variable keypart    */

key->k_flags  = ISDUPS + COMPRESS;

key->k_nparts = 3;              /*  set count of index parts 3        */
				/*  set keypart object number         */
key->k_part[0].kp_start = 0;
key->k_part[0].kp_leng  = INTSIZE;
key->k_part[0].kp_type  = INTTYPE;

if                              /*  index for read access required    */
  (op_id == D23_READ)
{                               /*  set keypart object key            */
  key->k_part[1].kp_start = INTSIZE;
  key->k_part[1].kp_leng  = LONGSIZE;
  key->k_part[1].kp_type  = LONGTYPE;

  var_keypart = key->k_part + 2;
}
else
{                               /*  set keypart object key            */
  key->k_part[2].kp_start = INTSIZE;
  key->k_part[2].kp_leng  = LONGSIZE;
  key->k_part[2].kp_type  = LONGTYPE;

  var_keypart = key->k_part + 1;
}                               

				/*  set keypart attribute value       */
var_keypart->kp_start = INTSIZE + LONGSIZE;

add_len  = at->d26a_rep == D2_T61_PR_LIST ? 2 * D26_MAXVALPARTS :
	    (at->d26a_rep == D2_TLXNR_STX ? D26_MAXVALPARTS : 1);

switch(at->d26a_syntax)
{
  case D2_CEXACT    :
  case D2_CIGNORE   :
  case D2_IA5       :
  case D2_PRINTABLE :
  case D2_NUMERIC   :
  case D2_CIGN_LIST :
  case D2_TIME      :
  case D2_TELNR_STX :
  case D2_POST_AD_STX:
  case D2_TLXNR_STX :
  case D2_COUNTRY_STX:
    if                  /* value itself goes into the index           */
      (rule != D2_APPROX_MATCH || at->d26a_phon == FALSE)
    {                   /* reset start of index part                  */
      var_keypart->kp_start += at->d26a_phon == TRUE ? D26_LPHON : 0;
      var_keypart->kp_start += at->d26a_rep == D2_T61_PR ? INTSIZE : 0;
      len = at->d26a_ubound + add_len;
    }
    else
    {                   /* set length of index part only              */
      len = D26_LPHON;
    }                   /*                                            */
    var_keypart->kp_type  = CHARTYPE + (rule == D2_LTEQ ? ISDESC : 0);
    break;
  case D2_ANY_STX   :
  case D2_OBJ_IDENT :
  case D2_OCTET     :
  case D2_GUIDE     :
  case D2_TTXID_STX :
  case D2_FAXNR_STX :
  case D2_PSAP_STX  :
  case D2_PASSWD_STX:
  case D2_CERTIFICATE:
  case D2_CERT_PAIR :
  case D2_CERT_LIST :
  case D2_MHS_DLSP_STX:
  case D2_MHS_ORADR_STX:
  case D2_MHS_ORNAME_STX:
  case D2_ASN1:
    len = at->d26a_ubound;
    var_keypart->kp_type  = CHARTYPE + (rule == D2_LTEQ ? ISDESC : 0);
    break;
  case D2_PR_DM_STX:
  case D2_MHS_PR_DM_STX:
    len = at->d26a_ubound * LONGSIZE;
    var_keypart->kp_type  = CHARTYPE;
    break;
  case D2_DISTNAME  :
  case D2_BOOLEAN   :
  case D2_INTEGER   :
    len = LONGSIZE;
    var_keypart->kp_type  = LONGTYPE + (rule == D2_LTEQ ? ISDESC : 0);
    break;
  default           :
    len = 0;
    var_keypart->kp_type = CHARTYPE;
    break;
}   

if                      /*  keypart too long                          */
  (INTSIZE + LONGSIZE + len > MAXKEYSIZE)
{                       /*  reset length                              */
  var_keypart->kp_leng = MAXKEYSIZE - INTSIZE - LONGSIZE;
  return(D26_CMP);
}
else
{                       /*  return "no compare"                       */
  var_keypart->kp_leng = len;
  return(D26_NCMP);
}                       

}                       /*  d26_i55_sel_rec_key                       */
