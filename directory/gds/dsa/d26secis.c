/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1994, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26secis.c,v $
 * Revision 1.1.127.2  1996/02/18  19:45:57  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:27:21  marty]
 *
 * Revision 1.1.127.1  1995/12/08  15:47:36  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:07:47  root]
 * 
 * Revision 1.1.125.3  1994/06/09  18:44:57  devsrc
 * 	cr10871 - expand copyright
 * 	[1994/06/09  18:17:44  devsrc]
 * 
 * Revision 1.1.125.2  1994/05/10  15:52:42  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:24:10  marrek]
 * 
 * Revision 1.1.125.1  1994/02/22  17:52:40  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:17:56  marrek]
 * 
 * Revision 1.1.123.2  1993/10/14  17:16:38  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:11:28  keutel]
 * 
 * Revision 1.1.123.1  1993/10/13  17:31:02  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:46:17  keutel]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26secis.c,v $ $Revision: 1.1.127.2 $ $Date: 1996/02/18 19:45:57 $";
#endif

/*daton ***************************************************************/
/*                                                                    */
/* TYPE         : MODUL                                               */
/*                                                                    */
/* NAME         : d26secis.c        [d26secis]                        */
/*                                                                    */
/* AUTHOR       : E. Kraemer, BUBA KP1                                */
/* DATE         : 03.08.93                                            */
/*                                                                    */
/* COMPONENT    : DSA, retrieval functions                            */
/*                                                                    */
/* DOC.-NR.     : <relevante Dokumente>                               */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   : <Produktnr. aus Produktstruktur> - <Versionsnr.>    */
/*                                                                    */
/* DESCRIPTION  :   The module contains the ISAM utilities used by    */
/*                  the DSA, that handle SAP attributes               */
/*                                                                    */
/* SYSTEM-DEPENDENCY  :                                               */
/*                      SINIX                                         */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Datum  |  Aenderungen                   | KZ | CR# FM# */
/*          | 03.08.93| birth                          | ek |         */
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

#define D26_IS44_LD_SUBJECTPUBLICKEYINFO d26_is44_ld_signature
#define D26_IS64_ST_SUBJECTPUBLICKEYINFO d26_is64_st_signature

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

/******** IMPORT ********/

static void d26_is41_ld_algid(char **alg_id, char **record);
static void d26_is42_ld_distname(char **ds_name, char **record);
static void d26_is43_ld_utctime(char **tm_info, char **record);
static void d26_is44_ld_signature(char **signature, char **record);
static void d26_is45_ld_revcert(char **rev_cert, char **record);

static void d26_is61_st_algid(char **alg_id, char **record);
static void d26_is62_st_distname(char **ds_name, char **record);
static void d26_is63_st_utctime(char **tm_info, char **record);
static void d26_is64_st_signature(char **signature, char **record);
static void d26_is65_st_revcert(char **rev_cert, char **record);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is31_ld_certificate                               */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        02.08.93                                              */
/*                                                                    */
/* DESCRIPTION: This function builds the certificate attribute value  */
/*              in APDU format from C-ISAM format.                    */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d26_is31_ld_certificate(
			/* OUT   -  where the certificate value has   */
    char **certificate, /*          to be written to                  */
    char **record)      /* IN    -  Record to read data from.         */

{                               /* d26_is31_ld_certificate            */

				/* load Version from record           */
  *(signed32 *)*certificate = ldlong(*record);
  *certificate += sizeof(signed32);
  *record      += LONGSIZE;

				/* load serial number from record     */
  *(signed32 *)*certificate = ldlong(*record);
  *certificate += sizeof(signed32);
  *record      += LONGSIZE;

				/* load algorithm identifier          */
  d26_is41_ld_algid(certificate,record);

				/* write issuer                       */
  d26_is42_ld_distname(certificate,record);

				/* write "not before"                 */
  d26_is43_ld_utctime(certificate,record);

				/* write "not after"                  */
  d26_is43_ld_utctime(certificate,record);

				/* write subject                      */
  d26_is42_ld_distname(certificate,record);

				/* write "subject public key info"    */
  D26_IS44_LD_SUBJECTPUBLICKEYINFO(certificate,record);

				/* write "signature"                  */
  d26_is44_ld_signature(certificate,record);

}                               /* d26_is31_ld_certificate            */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is32_ld_cert_pair                                 */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        02.08.93                                              */
/*                                                                    */
/* DESCRIPTION: This function builds the certificate pair attribute   */
/*              value in APDU format from C-ISAM format.              */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d26_is32_ld_cert_pair(
			/* OUT   -  where the certificate pair has    */
    char **cert_pair,   /*          to be written to                  */
    char **record)      /* IN    -  Record to read data from.         */

{                               /* d26_is32_ld_cert_pair              */

  unsigned16 tag;               /* tag of certificate                 */

				/* load certificate from record       */
  ((D23_certificate *)*cert_pair)->d23_cert_tag = ldint(*record);
  *record    += INTSIZE;
  *cert_pair += sizeof(D23_certificate);
  *record    += INTSIZE;

				/* load certificate from record       */
  d26_is31_ld_certificate(cert_pair,record);

  if                            /* another certificate present        */
    ((tag = ldint(*record)) == D23_REVTAG)
  {                             /* load second certificate from record*/
    ((D23_certificate *)*cert_pair)->d23_cert_tag = tag;
    *record    += INTSIZE;
    *cert_pair += sizeof(D23_certificate);
    *record    += INTSIZE;

				/* load certificate from record       */
    d26_is31_ld_certificate(cert_pair,record);

  }

}                               /* d26_is32_ld_cert_pair              */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is33_ld_cert_list                                 */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        04.08.93                                              */
/*                                                                    */
/* DESCRIPTION: This function builds the certificate list attribute   */
/*              value in APDU format from C-ISAM format. It returns   */
/*              its length.                                           */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d26_is33_ld_cert_list(
			/* OUT   -  where the certificate list has to */
    char **cert_list,   /*          be written to                     */
    char **record)      /* IN    -  Record to read data from.         */

{                               /* d26_is33_ld_cert_list              */

  signed32 nr_rev_cert;         /* number of revoked certificates     */

  register signed16 i;          /* loop variable                      */

				/* load algorithm identifier          */
  d26_is41_ld_algid(cert_list,record);

				/* write issuer                       */
  d26_is42_ld_distname(cert_list,record);

				/* write last update                  */
  d26_is43_ld_utctime(cert_list,record);

				/* load number of revoked             */
				/* certificates from record           */
  nr_rev_cert = *(signed32 *)*cert_list = ldlong(*record);
  *cert_list += sizeof(signed32);
  *record    += LONGSIZE;

  if                            /* revoked certificates present       */
    (nr_rev_cert != D2_NOT_DEFINED)
  {                             /* load them                          */
    for                         /* all revoked certificates           */
       (i = 0; i < nr_rev_cert; i++)
    {                           /* load revoked certificate           */
      d26_is45_ld_revcert(cert_list,record);
    }
				/* write signature for revoked cert.  */
    d26_is44_ld_signature(cert_list,record);
  }

				/* write signature for cert. list     */
  d26_is44_ld_signature(cert_list,record);

}                               /* d26_is33_ld_cert_list              */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is41_ld_algid                                     */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        04.08.93                                              */
/*                                                                    */
/* DESCRIPTION: This function loads an algorithm identifier in APDU   */
/*              format from C-ISAM format.                            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None        Short           length of algorithm in APDU format*/
/*                                                                    */
/*inoff ***************************************************************/

static void d26_is41_ld_algid(
			/* OUT   -  where the algorithm identifier has*/
    char **alg_id,      /*          to be written to                  */
    char **record)      /* IN    -  Record to read data from.         */

{                               /* d26_is41_ld_algid                  */

  signed16 par;             /* indicates kind of parameters           */
  signed16 off;             /* offset of object identifier            */

				/* write Algorithm identifier         */
				/* structure                          */
  ((D23_alg_id *)*alg_id)->d23_algtag = ldint(*record);
  *record += INTSIZE;
  ((D23_alg_id *)*alg_id)->d23_algpar = par = ldint(*record);
  *record += INTSIZE;
  ((D23_alg_id *)*alg_id)->d23_alglen = ldint(*record);
  *record += INTSIZE;
  ((D23_alg_id *)*alg_id)->d23_algoff = off = ldint(*record);
  *record += INTSIZE;
  *alg_id += sizeof(D23_alg_id);

				/* copy object identifier             */
  memcpy(*alg_id,*record,off);
  *alg_id += off;
  *record += off;

  if                            /* parameters are present             */
    (par == D23_INT_PARAMETERS)
  {                             /* load parameters                    */
    *(signed32 *)*alg_id = ldlong(*record);
    *alg_id += sizeof(signed32);
    *record += LONGSIZE;
  }

}                               /* d26_is41_ld_algid                  */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is42_ld_distname                                  */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        04.08.93                                              */
/*                                                                    */
/* DESCRIPTION: This function loads a distinguished name in APDU      */
/*              format from C-ISAM format.                            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_is42_ld_distname(
			/* OUT   -  where the distinguished name has  */
    char **ds_name,     /*          to be written to                  */
    char **record)      /* IN    -  Record to read data from.         */

{                               /* d26_is42_ld_distname               */

  signed16 off;             /* offset of distinguished name           */

				/* write distinguished name           */
  ((D23_ds_name *)*ds_name)->d23_dstag = ldint(*record);
  *record += INTSIZE;
  ((D23_ds_name *)*ds_name)->d23_dscomplete = ldint(*record);
  *record += INTSIZE;
  ((D23_ds_name *)*ds_name)->d23_dslen = ldint(*record);
  *record += INTSIZE;
  ((D23_ds_name *)*ds_name)->d23_dsoff = off = ldint(*record);
  *record += INTSIZE;
  *ds_name += sizeof(D23_ds_name);
  strcpy(*ds_name,*record);
  *ds_name += off;
  *record  += off;

}                               /* d26_is42_ld_distname               */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is43_ld_utctime                                   */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        04.08.93                                              */
/*                                                                    */
/* DESCRIPTION: This function loads a UTC-time in APDU format from    */
/*              C-ISAM format.                                        */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_is43_ld_utctime(
			/* OUT   -  where the time information        */
    char **tm_info,     /*          structure has to be written to    */
    char **record)      /* IN    -  Record to read data from.         */

{                               /* d26_is43_ld_utctime                */

  signed16 off;                 /* offset of time information         */

				/* write time information structure   */
  ((D23_tm_info *)*tm_info)->d23_tmtag = ldint(*record);
  *record  += INTSIZE;
  ((D23_tm_info *)*tm_info)->d23_tmfil = ldint(*record);
  *record  += INTSIZE;
  ((D23_tm_info *)*tm_info)->d23_tmlen = ldint(*record);
  *record  += INTSIZE;
  ((D23_tm_info *)*tm_info)->d23_tmoff = off = ldint(*record);
  *record  += INTSIZE;
  *tm_info += sizeof(D23_tm_info);

  memcpy(*tm_info,*record,off);
  *record  += off;
  *tm_info += off;

}                               /* d26_is43_ld_utctime                */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is44_ld_signature                                 */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        04.08.93                                              */
/*                                                                    */
/* DESCRIPTION: This function loads a signature in APDU format from   */
/*              C-ISAM format.                                        */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_is44_ld_signature(
			/* OUT   -  where the signature has to be     */
    char **signature,   /*          written to                        */
    char **record)      /* IN    -  Record to read data from.         */

{                               /* d26_is44_ld_signature              */

  signed16 off;                 /* offset of bit string               */

				/* write algorithm identifier         */
  d26_is41_ld_algid(signature,record);

  ((D23_bit_info *)*signature)->d23_btag = ldint(*record);
  *record   += INTSIZE;
  *record   += INTSIZE;
  ((D23_bit_info *)*signature)->d23_blen = ldint(*record);
  *record   += INTSIZE;
  ((D23_bit_info *)*signature)->d23_boff = off = ldint(*record);
  *record   += INTSIZE;
  *signature += sizeof(D23_bit_info);

  memcpy(*signature,*record,off);
  *record   += off;
  *signature += off;

}                               /* d26_is44_ld_signature               */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is45_ld_revcert                                   */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        05.08.93                                              */
/*                                                                    */
/* DESCRIPTION: This function loads a revoked certificate in APDU     */
/*              format from C-ISAM format.                            */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_is45_ld_revcert(
			/* OUT   -  where the revoked certificate has */
    char **rev_cert,    /*          to be written to                  */
    char **record)      /* IN    -  Record to read data from.         */

{                               /* d26_is45_ld_revcert                */

  ((D23_rev_cert *)*rev_cert)->d23_rc_tag   = ldint(*record);
  *record   += INTSIZE;
  *record   += INTSIZE;
  ((D23_rev_cert *)*rev_cert)->d23_rc_ucert = ldlong(*record);
  *record   += LONGSIZE;
  *rev_cert += sizeof(D23_rev_cert);

				/* write algorithm identifier         */
  d26_is41_ld_algid(rev_cert,record);

				/* write issuer                       */
  d26_is42_ld_distname(rev_cert,record);

				/* write revocation date              */
  d26_is43_ld_utctime(rev_cert,record);

}                               /* d26_is45_ld_revcert                */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is51_st_certificate                               */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        05.08.93                                              */
/*                                                                    */
/* DESCRIPTION: This function stores the certificate attribute value  */
/*              from APDU format to C-ISAM format.                    */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d26_is51_st_certificate(
    char **certificate, /* IN    -  certificate to be written         */
    char **record)      /* OUT   -  Record to write data to           */

{                               /* d26_is51_st_certificate            */

				/* store Version to record            */
  stlong(*(signed32 *)*certificate,*record);
  *certificate += sizeof(signed32);
  *record      += LONGSIZE;

				/* store serial number to record      */
  stlong(*(signed32 *)*certificate,*record);
  *certificate += sizeof(signed32);
  *record      += LONGSIZE;

				/* store algorithm identifier         */
  d26_is61_st_algid(certificate,record);

				/* store issuer                       */
  d26_is62_st_distname(certificate,record);

				/* store "not before"                 */
  d26_is63_st_utctime(certificate,record);

				/* store "not after"                  */
  d26_is63_st_utctime(certificate,record);

				/* store subject                      */
  d26_is62_st_distname(certificate,record);

				/* store "subject public key info"    */
  D26_IS64_ST_SUBJECTPUBLICKEYINFO(certificate,record);

				/* store "signature"                  */
  d26_is64_st_signature(certificate,record);

}                               /* d26_is51_st_certificate            */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is52_st_cert_pair                                 */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        06.08.93                                              */
/*                                                                    */
/* DESCRIPTION: This function stores the certificate pair attribute   */
/*              value in APDU format to C-ISAM format.                */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d26_is52_st_cert_pair(
    char **cert_pair,   /* IN    -  certificate pair to be written    */
    char **record)      /* OUT   -  Record to write data to           */

{                               /* d26_is52_st_cert_pair              */

  unsigned16 tag;               /* tag of certificate                 */

				/* store certificate tag to record    */
  stint(((D23_certificate *)*cert_pair)->d23_cert_tag,*record);
  *record    += INTSIZE;
  *record    += INTSIZE;
  *cert_pair += sizeof(D23_certificate);

				/* store certificate to record        */
  d26_is51_st_certificate(cert_pair,record);

  if                            /* another certificate present        */
    ((tag = ((D23_certificate *)*cert_pair)->d23_cert_tag) == D23_REVTAG)
  {                             /* store second certificate tag       */
    stint(tag,*record);
    *record    += INTSIZE;
    *record    += INTSIZE;
    *cert_pair += sizeof(D23_certificate);

				/* store second certificate to record */
    d26_is51_st_certificate(cert_pair,record);
  }

}                               /* d26_is52_st_cert_pair              */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is53_st_cert_list                                 */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        04.08.93                                              */
/*                                                                    */
/* DESCRIPTION: This function stores the certificate list attribute   */
/*              value in APDU format to C-ISAM format.                */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*exoff ***************************************************************/

void d26_is53_st_cert_list(
    char **cert_list,   /* IN    -  certificate list to be written    */
    char **record)      /* OUT   -  Record to write data to           */

{                               /* d26_is53_st_cert_list              */

  signed32 nr_rev_cert;         /* number of revoked certificates     */

  register signed16 i;          /* loop variable                      */

				/* store algorithm identifier         */
  d26_is61_st_algid(cert_list,record);

				/* store issuer                       */
  d26_is62_st_distname(cert_list,record);

				/* store last update                  */
  d26_is63_st_utctime(cert_list,record);

				/* store number of revoked            */
				/* certificates to record             */
  nr_rev_cert = *(signed32 *)*cert_list;
  *cert_list += sizeof(signed32);

				/* store number of revoced certific.  */
  stlong(nr_rev_cert,*record);
  *record += LONGSIZE;

  if                            /* revoked certificates present       */
    (nr_rev_cert != D2_NOT_DEFINED)
  {                             /* store them                         */
    for                         /* all revoked certificates           */
       (i = 0; i < nr_rev_cert; i++)
    {                           /* store revoked certificate          */
      d26_is65_st_revcert(cert_list,record);
    }

				/* store signature for revoked cert.  */
    d26_is64_st_signature(cert_list,record);
  }

				/* store signature for cert. list     */
  d26_is64_st_signature(cert_list,record);

}                               /* d26_is53_st_cert_list              */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is61_st_algid                                     */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        05.08.93                                              */
/*                                                                    */
/* DESCRIPTION: This function stores an algorithm identifier in APDU  */
/*              format to C-ISAM format.                              */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_is61_st_algid(
    char **alg_id,      /* IN    -  algorithm identifier to be written*/
    char **record)      /* OUT   -  Record to write data to           */

{                               /* d26_is61_st_algid                  */

  signed16 par;                 /* indicates kind of parameters       */
  signed16 off;                 /* offset of object identifier        */

				/* store algorithm tag                */
  stint(((D23_alg_id *)*alg_id)->d23_algtag,*record);
  *record += INTSIZE;

  par = ((D23_alg_id *)*alg_id)->d23_algpar;
  stint(par,*record);             /* store parameter flag             */
  *record += INTSIZE;

				/* store object identifier length     */
  stint(((D23_alg_id *)*alg_id)->d23_alglen,*record);
  *record += INTSIZE;

  off = ((D23_alg_id *)*alg_id)->d23_algoff;
  stint(off,*record);           /* store object identifier offset     */
  *record += INTSIZE;

  *alg_id += sizeof(D23_alg_id);
				/* copy object identifier             */
  memcpy(*record,*alg_id,off);
  *alg_id += off;
  *record += off;

  if                            /* parameters are present             */
    (par == D23_INT_PARAMETERS)
  {                             /* store parameters                   */
    stlong(*(signed32 *)*alg_id,*record);
    *alg_id += sizeof(signed32);
    *record += LONGSIZE;
  }

}                               /* d26_is61_st_algid                  */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is62_st_distname                                  */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        06.08.93                                              */
/*                                                                    */
/* DESCRIPTION: This function stores a distinguished name in APDU     */
/*              format to C-ISAM format.                              */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_is62_st_distname(
    char **ds_name,     /* IN    -  distinguished name to be written  */
    char **record)      /* OUT   -  Record to write data to           */

{                               /* d26_is62_st_distname               */

  signed16 off;                 /* offset of distinguished name       */

				/* write distinguished name           */
  stint(((D23_ds_name *)*ds_name)->d23_dstag,*record);
  *record += INTSIZE;
  stint(((D23_ds_name *)*ds_name)->d23_dscomplete,*record);
  *record += INTSIZE;
  stint(((D23_ds_name *)*ds_name)->d23_dslen,*record);
  *record += INTSIZE;
  off = ((D23_ds_name *)*ds_name)->d23_dsoff;
  stint(off,*record);
  *record += INTSIZE;
  *ds_name += sizeof(D23_ds_name);

  strcpy(*record,*ds_name);
  *record += off;
  *ds_name += off;

}                               /* d26_is62_st_distname               */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is63_st_utctime                                   */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        04.08.93                                              */
/*                                                                    */
/* DESCRIPTION: This function stores a UTC-time in APDU format to     */
/*              C-ISAM format.                                        */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_is63_st_utctime(
    char **tm_info,     /* IN    -  UTC time to be written            */
    char **record)      /* OUT   -  Record to write data to           */

{                               /* d26_is63_st_utctime                */

  signed16 off;                 /* length of distinguished name       */

				/* write time information structure   */
  stint(((D23_tm_info *)*tm_info)->d23_tmtag,*record);
  *record  += INTSIZE;
  *record  += INTSIZE;
  stint(((D23_tm_info *)*tm_info)->d23_tmlen,*record);
  *record  += INTSIZE;
  off = ((D23_tm_info *)*tm_info)->d23_tmoff;
  stint(off,*record);
  *record  += INTSIZE;
  *tm_info += sizeof(D23_tm_info);
  memcpy(*record,*tm_info,off);
  *record  += off;
  *tm_info += off;

}                               /* d26_is63_st_utctime                */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is64_st_signature                                 */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        04.08.93                                              */
/*                                                                    */
/* DESCRIPTION: This function stores a signature in APDU format from  */
/*              C-ISAM format.                                        */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_is64_st_signature(
    char **signature,   /* IN    -  signature to be written           */
    char **record)      /* OUT   -  Record to write data to           */

{                               /* d26_is64_st_signature              */

  signed16 off;                 /* offset of bit string               */

				/* store algorithm identifier         */
  d26_is61_st_algid(signature,record);

				/* store bit string structure         */
  stint(((D23_bit_info *)*signature)->d23_btag,*record);
  *record    += INTSIZE;
  *record    += INTSIZE;
  stint(((D23_bit_info *)*signature)->d23_blen,*record);
  *record    += INTSIZE;
  off = ((D23_bit_info *)*signature)->d23_boff;
  stint(off,*record);
  *record    += INTSIZE;
  *signature += sizeof(D23_bit_info);

				/* copy bit string to record          */
  memcpy(*record,*signature,off);
  *record    += off;
  *signature += off;

}                               /* d26_is64_st_signature              */

/*inon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_is65_st_revcert                                   */
/*                                                                    */
/* AUTHOR:      Kraemer, BUBA KP1                                     */
/* DATE:        05.08.93                                              */
/*                                                                    */
/* DESCRIPTION: This function stores a revoked certificate in APDU    */
/*              format to C-ISAM format.                              */
/*                                                                    */
/* RETURNVALUE:                                                       */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

static void d26_is65_st_revcert(
    char **rev_cert,    /* IN    -  revoked certificate to be written */
    char **record)      /* OUT   -  Record to write data to           */

{                               /* d26_is65_st_revcert                */

  stint(((D23_rev_cert *)*rev_cert)->d23_rc_tag,*record);
  *record   += INTSIZE;
  *record   += INTSIZE;
  stlong(((D23_rev_cert *)*rev_cert)->d23_rc_ucert,*record);
  *record   += LONGSIZE;
  *rev_cert += sizeof(D23_rev_cert);

				/* store algorithm identifier         */
  d26_is61_st_algid(rev_cert,record);

				/* store issuer                       */
  d26_is62_st_distname(rev_cert,record);

				/* store revocation date              */
  d26_is63_st_utctime(rev_cert,record);

}                               /* d26_is65_st_revcert                */
