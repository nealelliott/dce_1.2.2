/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26sutil.c,v $
 * Revision 1.1.8.2  1996/02/18  19:46:10  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:27:34  marty]
 *
 * Revision 1.1.8.1  1995/12/08  15:48:21  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:08:00  root]
 * 
 * Revision 1.1.6.3  1994/06/21  14:45:55  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  09:34:16  marrek]
 * 
 * Revision 1.1.6.2  1994/05/10  15:52:48  marrek
 * 	April 1994 code submission.
 * 	[1994/04/27  12:24:41  marrek]
 * 
 * Revision 1.1.6.1  1994/02/22  17:52:56  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:19:14  marrek]
 * 
 * Revision 1.1.4.2  1993/08/10  15:27:55  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:18:35  marrek]
 * 
 * Revision 1.1.2.3  1993/02/02  14:52:38  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.2.2  1993/02/02  11:27:26  marrek
 * 	Creation.
 * 	[1993/02/01  17:06:36  marrek]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26sutil.c,v $ $Revision: 1.1.8.2 $ $Date: 1996/02/18 19:46:10 $";
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
/* NAME         : d26sutil.c <Utility-Functions for schema object     */
/*                            generation>                             */
/*                                                                    */
/* AUTHOR       : Kraemer, AP 11                                      */
/* DATE         : 22.12.92                                            */
/*                                                                    */
/* COMPONENT    : DS                                                  */
/*                                                                    */
/* DOC.-NR.     : Directory-Design-Specification                      */
/*                      .                                             */
/*                      .                                             */
/*                                                                    */
/* PRD#/VERS.   :                                                     */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*                                                                    */
/* SYSTEM DEPENDENCIES: COMMON                                        */
/*                                                                    */
/* HISTORY      :                                                     */
/*                                                                    */
/* Vers.Nr. |  Date   |  Updates                       | KZ | CR# FM# */
/*   0.1    |22.12.92 |  Original                      | WS |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/**********************************************************************/
/*                                                                    */
/*                        I N C L U D E S                             */
/*                                                                    */
/**********************************************************************/

#include <d26dsa.h>

/*****  Operating-System-Includes  *****/

#include <errno.h>

/*****  external Includes    *****/

#include <dce/d27util.h>

/*****  internal Includes    *****/

/**********************************************************************/
/*                                                                    */
/*                        D E F I N E S                               */
/*                                                                    */
/**********************************************************************/

#define D26_EOH '%'                 /* end of header character        */

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

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_ms1_init                                        */
/*                                                                    */
/* AUTHOR       : Kraemer, AP 11                                      */
/* DATE         : 22.12.92                                            */
/*                                                                    */
/* DESCRIPTION  : This function checks the command line parameters of */
/*                the program, that generates the schema object.      */
/*                It opens the input file and skips the header until  */
/*                useful information is found.                        */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      != NULL     FILE *          file pointer to input file        */
/*      NULL                        error occurred                    */
/*                                                                    */
/*exoff ***************************************************************/

FILE *d26_ms1_init(
    char *filename)     /* IN    -  name of input file                */

{                       /* d26_ms1_init                               */

  int c;                /* character to be read from input file       */
  FILE *fp;             /* file pointer to input file                 */

  if                    /* input file cannot be opened                */
    ((fp = D27_FOPEN(svc_c_sev_fatal,filename,"r")) == NULL)
  {                     /* return error                               */
    return((FILE *)NULL);
  }
  else                  /* scan input file for 'end of header'        */
  {
    while               /* end of file not reached                    */
	 ((c = fgetc(fp)) != EOF)
    {                   /* check the character                        */
      if                /* character terminates header                */
	(c == D26_EOH)
      {                 /* read newline character                     */
	fgetc(fp);
			/* return file pointer                        */
	return(fp);
      }
    }

    dce_svc_printf(GDS_S_INCONS_INSTREAM_MSG);
    D27_FCLOSE(svc_c_sev_fatal,fp);
    return((FILE *)NULL);
  }

}                       /* d26_ms1_init                               */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_ms2_terminate                                   */
/*                                                                    */
/* AUTHOR       : Kraemer, AP 11                                      */
/* DATE         : 22.12.92                                            */
/*                                                                    */
/* DESCRIPTION  : This function closes the input file. It generates   */
/*                the data version file and the default configuration */
/*                file.                                               */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      D2_NOERROR  signed16        no error occurred                 */
/*      D2_ERROR                    error occurred                    */
/*                                                                    */
/*exoff ***************************************************************/

signed16 d26_ms2_terminate(
    FILE *fp,           /* IN    -  file pointer to input file        */
    char *filename)     /* IN    -  name of input file                */

{                       /* d26_ms2_terminate                          */

			/* return value                               */
  signed16 ret_value = D2_NOERROR;

  if                    /* input file cannot be closed                */
    (D27_FCLOSE(svc_c_sev_fatal,fp) != 0)
  {                     /* return error                               */
    return(D2_ERROR);
  }
  else if               /* data version file cannot be opened         */
	 ((fp = D27_FOPEN(svc_c_sev_fatal,D27_DSA_D_VERS + 1,"w")) == NULL)
  {                     /* return error                               */
    return(D2_ERROR);
  }
  else
  {                     /* write data version                         */
    if                  /* data version cannot be written             */
      (d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%d",D26DV6) < 0)
    {                   /* set return value                           */
      ret_value = D2_ERROR;
    }
    if                  /* data version file cannot be closed         */
      (D27_FCLOSE(svc_c_sev_fatal,fp) != 0)
    {                   /* set return value                           */
      ret_value = D2_ERROR;
    }
    if                  /* error occurred                             */
      (ret_value == D2_ERROR)
    {                   /* return error                               */
      return(D2_ERROR);
    }
    else if             /* configuration file cannot be opened        */
	   ((fp = D27_FOPEN(svc_c_sev_fatal,D27_DSA_CONF + 1,"w")) == NULL)
    {                   /* return error                               */
      return(D2_ERROR);
    }
    else
    {                   /* write database specific configuration data */
      if                /* database specific configuration data       */
			/* can be written                             */
	(d26_msi1_write_configuration_data(fp) == D2_NOERROR)
      {                 /* write country codes                        */
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","countries: #");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","AD AE AF AG AI AL AN AO AQ AR AS AT AU AW");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","BB BD BE BF BG BH BI BJ BM BN BO BR BS BT BU BV BW BY BZ");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","CA CC CF CG CH CI CK CL CM CN CO CR CS CT CU CV CX CY");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","DD DE DJ DK DM DO DZ");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","EC EG EH ES ET");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","FI FJ FK FM FO FR");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","GA GB GD GF GH GI GL GM GN GP GQ GR GT GU GW GY");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","HK HM HN HT HU HV");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","ID IE IL IN IO IQ IR IS IT");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","JM JO JP JT");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","KE KH KI KM KN KP KR KW KY");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","LA LB LC LI LK LR LS LU LY");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","MA MC MG MH MI ML MN MO MP MQ MR MS MT MU MV MW MX MY MZ");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","NA NC NE NF NG NI NL NO NP NQ NR NT NU NZ");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","OM");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","PA PC PE PF PG PH PK PL PM PN PR PT PU PW PY");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","QA");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","RE RO RW");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","SA SB SC SD SE SG SH SJ SL SM SN SO SR ST SU SV SY SZ");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","TC TD TF TG TH TK TN TO TP TR TT TV TW TZ");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","UA UG UM US UY");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","VA VC VE VG VI VN VU");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","WF WK WS");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","YD YE YU");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","ZA ZM ZR ZW");
	d27_fprintf(__FILE__,__LINE__,svc_c_sev_fatal,fp,"%s\n","#end of countries");
      }

      if                /* configuration file cannot be closed        */
	(D27_FCLOSE(svc_c_sev_fatal,fp) != 0)
      {                 /* return error                               */
	return(D2_ERROR);
      }
    }
    return(D2_NOERROR);
  }

}
