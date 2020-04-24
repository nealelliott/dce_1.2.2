/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d26sntx.c,v $
 * Revision 1.1.728.2  1996/02/18  19:45:59  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:27:25  marty]
 *
 * Revision 1.1.728.1  1995/12/08  15:47:46  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  15:07:50  root]
 * 
 * Revision 1.1.726.6  1994/08/10  08:25:27  marrek
 * 	Fix for SAP errors, OT11467.
 * 	[1994/08/08  14:14:02  marrek]
 * 
 * Revision 1.1.726.5  1994/07/19  14:48:54  marrek
 * 	SVR4 warnings removed.
 * 	[1994/07/19  11:57:57  marrek]
 * 
 * Revision 1.1.726.4  1994/06/21  14:45:52  marrek
 * 	June 1994 code submission.
 * 	[1994/06/16  11:54:09  marrek]
 * 
 * Revision 1.1.726.3  1994/05/10  15:52:44  marrek
 * 	Code submission on April 1994.
 * 	[1994/05/03  14:53:22  marrek]
 * 
 * 	Bug fixes.
 * 	[1994/04/29  09:29:35  marrek]
 * 
 * 	April 1994 code submission.
 * 	[1994/04/27  12:24:30  marrek]
 * 
 * Revision 1.1.726.2  1994/03/23  15:13:29  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:22:42  keutel]
 * 
 * Revision 1.1.726.1  1994/02/22  17:52:44  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  10:18:25  marrek]
 * 
 * Revision 1.1.724.2  1993/10/14  17:16:40  keutel
 * 	October 1993 code drop
 * 	[1993/10/14  17:11:55  keutel]
 * 
 * Revision 1.1.724.1  1993/10/13  17:31:06  keutel
 * 	October 1993 code drop
 * 	[1993/10/11  08:28:53  keutel]
 * 
 * Revision 1.1.6.2  1993/08/10  15:21:31  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  13:17:38  marrek]
 * 
 * Revision 1.1.4.7  1993/02/02  14:51:58  marrek
 * 	*** empty log message ***
 * 
 * Revision 1.1.4.6  1993/02/02  11:25:46  marrek
 * 	Upper bound of tel. number is checked in FAX-number
 * 	[1993/02/01  15:02:33  marrek]
 * 
 * Revision 1.1.4.5  1992/12/31  19:42:12  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:21:01  bbelch]
 * 
 * Revision 1.1.4.4  1992/12/07  13:23:29  marrek
 * 	November 1992 code drop
 * 	Non Basic Parameters in FAX-Number are checked only if present
 * 	[1992/12/07  13:20:21  marrek]
 * 
 * Revision 1.1.4.3  1992/12/02  10:26:33  marrek
 * 	November 1992 code drop
 * 	Check TTXID for printable cahracters
 * 	[1992/12/02  10:21:41  marrek]
 * 
 * Revision 1.1.4.2  1992/11/27  20:20:48  marrek
 * 	November 1992 code drop
 * 	[1992/11/20  11:16:10  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  23:51:59  zeliff
 * 	New version of file from GDS drop
 * 	[1992/05/28  20:57:54  zeliff]
 * 
 * $EndLog$
 */
#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d26sntx.c,v $ $Revision: 1.1.728.2 $ $Date: 1996/02/18 19:45:59 $";
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
/* NAME         : d26sntx.c <Utility-Functions for DSA-Syntax-check>  */
/*                                                                    */
/* AUTHOR       : Rustemeyer, D AP 11                                 */
/* DATE         : 08.04.91                                            */
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
/*   0.1    |08.04.91 |  Original                      | GR |         */
/*          |         |                                |    |         */
/*datoff **************************************************************/

/*--------------------------------------------------------------------*/
/*                     Module Identification                          */
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/*                        I N C L U D E S                             */
/*--------------------------------------------------------------------*/

/*****  internal Includes  *****/

#include <d26dsa.h>

/*****  Operating System - Includes  *****/

#include <string.h>
#include <time.h>

/*****  external Includes  *****/

#include <d21spat.h>
#include <dce/d27util.h>

/*--------------------------------------------------------------------*/
/*                        D E F I N E S                               */
/*--------------------------------------------------------------------*/

/*------------------------------------------------------------*/
/* constants for norming numbers and times                    */
/*------------------------------------------------------------*/
#define D26_DIGIT    0  /* indicates digit-segment in string          */
#define D26_NONDIGIT 1  /* indicates nondigit-segment in string       */
#define D26_UTC_PARTS 5 /* length of string containing Time up to     */
			/* minute precision                           */
#define D26_TM_SEGM_LEN 2   /* count of characters in on segment of   */
			/* UTC-time                                   */
#define D26_S_PER_MINUTE 60L    /* seconds per minute                 */
				/* seconds per hour                   */
#define D26_S_PER_HOUR  (60L * D26_S_PER_MINUTE)
				/* seconds per day                    */
#define D26_S_PER_DAY   (24L * D26_S_PER_HOUR)
				/* seconds per year                   */
#define D26_S_PER_YEAR  (365L * D26_S_PER_DAY)
#define D26_0_YEAR      70      /* year where count of time begins    */

#define D26_TZ_ABS      0x5a    /* indicates absolute UTC-time        */
#define D26_TZ_PLUS     0x2b    /* plus  character in UTC-time        */
#define D26_TZ_MINUS    0x2d    /* minus character in UTC-time        */

/*------------------------------------------------------------*/
/* constants for valid stringlists                            */
/*------------------------------------------------------------*/

#define D26_ADMD_STRLIST     0  /* D2_PRINTABLE or D2_NUMERIC-string, */
				/* Nullstring allowed                 */
#define D26_PRT_OR_NUM       1  /* D2_PRINTABLE or D2_NUMERIC-string, */
				/* Nullstring not allowed             */
#define D26_PRT_ANDOR_T61    2  /* D2_PRINTABLE and/or D2_T61-string  */

/*------------------------------------------------------------*/
/* constants for certificate                                  */
/*------------------------------------------------------------*/

#define D26_CERT_VER1      0    /* Version for certificates           */
				/* ... algorithm                      */

				/* macro to check subject public key  */
				/* info                               */
#define D26_USM_IS_SUBJPUBLICKEYINFO d26_usn_is_signature
/*------------------------------------------------------------*/
/* constants for upper bounds                                 */
/*------------------------------------------------------------*/

				/* Upper bound for ...                */
#define D26_UB_TELEX_NUMBER 14  /* ... telex numbers                  */
#define D26_UB_COUNTRY_CODE  4  /* ... country codes                  */
#define D26_UB_ANSWERBACK    8  /* ... answer back                    */

#define D26_UB_TTX_TERMID 1024  /* ... teletex terminal identifier    */

#define D26_UB_PHONE_NUMBER  32 /* ... telephone number component     */

/*--------------------------------------------------------------------*/
/*                 T Y P E -  D E F I N I T I O N S                   */
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/*                 D E C L A R A T I O N S        DATA                */
/*--------------------------------------------------------------------*/
struct strlist_limit
	  {
	     signed16 max_nb_str;
	     signed16 max_nb_prt;
	     signed16 max_nb_t61;
	     signed16 max_nb_num;
	  };

/*--------------------------------------------------------------------*/
/*                D E C L A R A T I O N S         FUNCTIONS           */
/*--------------------------------------------------------------------*/

static signed32 d26_usk_is_algorithm(byte **val);
static signed32 d26_usl_is_dist_name(byte **val);
static signed32 d26_usn_is_signature(byte **val);
static signed32 d26_usx_is_str_list(byte *val, signed16 len, signed32 type,
	signed32 *nbstr);
static signed16 d26_usy_str_list_part(byte **val, signed16 len);

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_us4_is_PreferDel                                */
/*                                                                    */
/* AUTHOR       : Rustemeyer, D AP 11                                 */
/* DATE         : 09.04.91                                            */
/*                                                                    */
/* RETURN VALUE :   D2_NOERROR      syntax-check successful           */
/*                  D2_ERROR:       syntax-check fails                */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function checks a string of given length.                 */
/*      The length must be 10, The 10 characters of the string must   */
/*      be a permutation of the digits {0,...,9}.                     */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_us4_is_PreferDel(
     byte *val,         /* IN    -  attribute value to check          */
     signed16 len)      /* IN    -  length of attribute value         */

{
  signed32        *curr_int = (signed32 *)val;
  signed32        i_count, nb_ints, i;
  static signed32 int_list[10];

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/
  if            /* attribute value too long                           */
    (len > 10 * sizeof(signed32))
  {             /* return error                                       */
    return (D2_ERROR);
  }

  for           /* all components of integer list                     */
     (i = 0; i < 10; i++)
  {             /* initialize it                                      */
    int_list[i] = -1;
  }

  nb_ints = len / sizeof(signed32);

  for           /* all components of integer list                     */
     (i_count=0 ; i_count < nb_ints; i_count++)
  {             /* check range                                        */
    if          /* component is out of range                          */
      (*curr_int < 0 || *curr_int > 9)
    {           /* return error                                       */
      return (D2_ERROR);
    }
    else if     /* component encountered first time                   */
	   (int_list [*curr_int ] == -1)
    {           /* mark it                                            */
      int_list[*curr_int ] = *curr_int;
    }
    else
    {           /* return error                                       */
      return(D2_ERROR);
    }
    curr_int++;
  }

  return (D2_NOERROR);

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_us6_is_NumString                                */
/*                                                                    */
/* AUTHOR       : Rustemeyer, D AP 11                                 */
/* DATE         : 09.04.91                                            */
/*                                                                    */
/* RETURN VALUE :   D2_NOERROR      syntax-check successful           */
/*                  D2_ERROR:       syntax-check fails                */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function checks a string of given length. The string must */
/*      consist of numeric characters only, that is digits or space.  */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_us6_is_NumString(
     byte *val,         /* IN    -  attribute value to check          */
     signed16 len)      /* IN    -  length of attribute value         */

{

  byte           *curr_char, *max_addr;

#define D26_DIGIT_0 0x30
#define D26_DIGIT_9 0x39
#define D26_SPACE   0x20

  curr_char = val;
  max_addr  = val + len;
  while                 /* end of string not reached                  */
       (curr_char < max_addr)
  {                     /* check for valid character                  */
    if                  /* character is valid                         */
      ((signed16)*curr_char >= D26_DIGIT_0 &&
       (signed16)*curr_char <= D26_DIGIT_9 ||
       *curr_char == D26_SPACE)
    {                   /* proceed                                    */
      curr_char ++ ;
    }
    else
    {                   /* return error                               */
      return (D2_ERROR);
    }
  }                                                    /* end while  */

  return (D2_NOERROR);

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* AUTHOR       : Rustemeyer, D AP 11                                 */
/* DATE         : 22.04.91                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function checks a string of given length. The string must */
/*      have the format "Universal  time".                            */
/*                                                                    */
/* RETURN VALUE :   D2_NOERROR      syntax-check successful           */
/*                  D2_ERROR:       syntax-check fails                */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_us7_is_UTCtime(
    byte *inp)          /* IN    -  attribute value to check          */

{

/*--------------------------------------------------------------------*/
/*                 T Y P E -  D E F I N I T I O N S                   */
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/*                  D E C L A R A T I O N S                           */
/*--------------------------------------------------------------------*/
int  nb_scan;                      /* scanf -  return-value      */
signed32 i;

static int days_of_month [][12] = {{31,28,31,30,31,30,31,31,30,31,30,31},
				   {31,29,31,30,31,30,31,31,30,31,30,31}};

int  year;                         /* year from UTC-Time         */
int  month;                        /* month from UTC-Time        */
int  day;                          /* day from UTC-Time          */
int  hour;                         /* hour from UTC-Time         */
int  minute;                       /* minute from UTC-Time       */
int  second;                       /* second from UTC-Time       */
char operation;                    /* indicates whether local    */
					/* time must be added or not  */
int  l_hour;                       /* hour from local time       */
int  l_minute;                     /* minute from local time     */

boolean leap;                      /* indicates leap             */

/*---------------------------------------------------------------------*/
/*                            C O D E                                  */
/*---------------------------------------------------------------------*/
nb_scan = sscanf((char *)inp,"%2d%2d%2d%2d%2d%c",&year,&month,&day,&hour,
		 &minute,&operation);

for (i=0; i<D26_UTC_PARTS*D26_TM_SEGM_LEN; i++)
{
  if (*(inp+i) == D26_TZ_MINUS  ||  *(inp+i) == D26_TZ_PLUS)
  {
    return(D2_ERROR);
  }
}

inp += D26_UTC_PARTS * D26_TM_SEGM_LEN;

if (nb_scan != 6)
{
   return (D2_ERROR);
}

/*------------------------------------------------------------------*/
/* test for valid month, day, hour and minute                       */
/*------------------------------------------------------------------*/
if (year < 0   ||
    day < 1   ||
    month < 1  ||  month > 12   ||
    hour < 0  ||  hour > 23   ||
    minute < 0  ||  minute > 59       )
{
   return (D2_ERROR);
}
else
{
  year = year >= D26_0_YEAR ? year - D26_0_YEAR : year + (100 - D26_0_YEAR);
  leap = (year + 2) % 4 > 0 ? FALSE : TRUE;
  if (day > days_of_month[leap][month-1])
  {
     return (D2_ERROR);
  }
}

/*------------------------------------------------------------------*/
/* if input-string contains seconds, scan next part                 */
/*------------------------------------------------------------------*/
if (operation >= D2_RG9 && operation <= D2_RG10)
{
  nb_scan = sscanf((char *)inp,"%2d%c",&second,&operation);
  for (i=0; i<D26_TM_SEGM_LEN; i++)
  {
    if (*(inp+i) == D26_TZ_MINUS  ||  *(inp+i) == D26_TZ_PLUS)
    {
      return(D2_ERROR);
    }
  }
  inp += D26_TM_SEGM_LEN;
  if (nb_scan != 2)
  {
    return(D2_ERROR);
  }
}
if (minute < 0  ||  minute > 59)
{
  return(D2_ERROR);
}

/*------------------------------------------------------------------*/
/* test operation, scan  rest of input and test l_hour and l_minute */
/*------------------------------------------------------------------*/
if  (operation != D26_TZ_ABS  &&
     operation != D26_TZ_PLUS  &&
     operation != D26_TZ_MINUS     )
{
  return(D2_ERROR);
}
else if(operation != D26_TZ_ABS)
{
  inp++;
  nb_scan = sscanf((char *)inp,"%2d%2d",&l_hour,&l_minute);
  if (nb_scan != 2)
  {
     return (D2_ERROR);
  }
  if (l_hour < 0  ||  l_hour > 23  ||
      l_minute < 0  ||  l_minute > 59)
  {
     return (D2_ERROR);
  }
  for (i=0; i<2*D26_TM_SEGM_LEN; i++)
  {
    if (*(inp+i) == D26_TZ_MINUS  ||  *(inp+i) == D26_TZ_PLUS)
    {
      return(D2_ERROR);
    }
  }
}

return (D2_NOERROR);

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_us9_norm_utc_time                                 */
/*                                                                    */
/* AUTHOR:      E. Kraemer, SNI AP 11                                 */
/* DATE:        07.01.91                                              */
/*                                                                    */
/* DESCRIPTION: This function writes the UTC-Time in a normed format. */
/*              That means, that it is converted to the coordinated   */
/*              format with trailing 'Z', precision extended to       */
/*              seconds.                                              */
/*                                                                    */
/*exoff ***************************************************************/

void d26_us9_norm_utc_time(
     char *inp,         /* IN    -  attribute value to norm           */
     char *out)         /* IN    -  normed attribute value            */

{                               /*  d26_us9_norm_utc_time             */

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

static int days_of_year_gone[][12] = {{0,31,59,90,120,151,181,212,243,
				     273,304,334},
				    {0,31,60,91,121,152,182,213,244,
				     274,305,335}};

int  year;                         /* year from UTC-Time         */
int  month;                        /* month from UTC-Time        */
int  day;                          /* day from UTC-Time          */
int  hour;                         /* hour from UTC-Time         */
int  minute;                       /* minute from UTC-Time       */
int  second;                       /* second from UTC-Time       */
char operation;                    /* indicates whether local    */
					/* time must be added or not  */
int  l_hour;                       /* hour from local time       */
int  l_minute;                     /* minute from local time     */

time_t calc_time;                  /* time to calculate          */
time_t time_zone;                  /* time difference to UTC-time*/
Bool leap;                         /* indicates leap             */
int  leaps_gone;                   /* count of leaps since 70    */

struct tm *abs_time;               /* absolute time, structured  */

/**********************************************************************/
/*                                                                    */
/*                            C O D E                                 */
/*                                                                    */
/**********************************************************************/

sscanf(inp,"%2d%2d%2d%2d%2d%c",&year,&month,&day,&hour,&minute,&operation);
inp += D26_UTC_PARTS * D26_TM_SEGM_LEN;
if                              /*  still digit following             */
  (operation >= D2_RG9 && operation <= D2_RG10)
{                               /*  read seconds from string          */
  sscanf(inp,"%2d%c",&second,&operation);
  inp += D26_TM_SEGM_LEN;
}
else
{                               /*  set seconds 0                     */
  second = 0;
}                               /*                                    */
				/*  calculate absolute time           */
year = year >= D26_0_YEAR ? year - D26_0_YEAR : year + (100 - D26_0_YEAR);
leap = (year + 2) % 4 > 0 ? FALSE : TRUE;
leaps_gone = (year + 1) / 4;
calc_time = (time_t)(year * D26_S_PER_YEAR +
	    (leaps_gone + days_of_year_gone[leap][month - 1] + day - 1L) *
		D26_S_PER_DAY +
	    hour * D26_S_PER_HOUR + minute * D26_S_PER_MINUTE + second);

if                              /*  UTC-time contains time difference */
  (operation != D26_TZ_ABS)
{                               /*  read time differential            */
  inp++;
  sscanf(inp,"%2d%2d",&l_hour,&l_minute);
  time_zone = (time_t)(l_hour * D26_S_PER_HOUR +
    l_minute * D26_S_PER_MINUTE);
  if                            /*  Time zone must pe added           */
    (operation == D26_TZ_MINUS)
  {                             /*  add time zone to local time       */
    calc_time += time_zone;
  }
  else
  {                             /*  subtract it                       */
    calc_time -= time_zone;
  }                             /*                                    */
}                               /*                                    */

abs_time = gmtime(&calc_time);

sprintf(out,"%2.2d%2.2d%2.2d%2.2d%2.2d%2.2d%c",abs_time->tm_year,
	abs_time->tm_mon + 1,abs_time->tm_mday,abs_time->tm_hour,
	abs_time->tm_min,abs_time->tm_sec,D26_TZ_ABS);

}                               /*  d26_us9_norm_utc_time             */

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_usa_is_tlxnr                                    */
/*                                                                    */
/* AUTHOR       : Rustemeyer, D AP 11                                 */
/* DATE         : 26.04.91                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function checks a string of given length. The string must */
/*      consist of three printable strings, separated by '\0'.        */
/*                                                                    */
/* RETURN VALUE :   D2_NOERROR      syntax-check successful           */
/*                  D2_ERROR:       syntax-check fails                */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_usa_is_tlxnr(
     byte *val,         /* IN    -  attribute value to check          */
     signed16 len)      /* IN    -  length of attribute value         */

{
/*--------------------------------------------------------------------*/
/*                  D E C L A R A T I O N S                           */
/*--------------------------------------------------------------------*/
byte *     telex_number, *country_code, *answerback;
signed16   len_telex_number, len_country_code, len_answerback;
signed16   rest_len;

/*---------------------------------------------------------------------*/
/*                            C O D E                                  */
/*---------------------------------------------------------------------*/

rest_len     = len;
telex_number = val;
if                          /* no second component                    */
  ((rest_len = d26_usy_str_list_part(&val,rest_len)) <= 0)
{                           /* return error                           */
  return(D2_ERROR);
}
country_code = val;
if                          /* no third component                     */
  ((rest_len = d26_usy_str_list_part(&val,rest_len)) <= 0)
{                           /* return error                           */
  return(D2_ERROR);
}
answerback = val;

len_telex_number = country_code - telex_number - 1;
len_country_code = answerback - country_code - 1;
len_answerback   = rest_len;

if                          /* any component is too long              */
  (len_telex_number > D26_UB_TELEX_NUMBER ||
   len_country_code > D26_UB_COUNTRY_CODE ||
   len_answerback   > D26_UB_ANSWERBACK)
{                           /* return error                           */
  return(D2_ERROR);
}
else if                     /* any component contains illegal         */
			    /* characters                             */
       (d27_605_is_PrtString(telex_number,len_telex_number) == D2_ERROR ||
	d27_605_is_PrtString(country_code,len_country_code) == D2_ERROR ||
	d27_605_is_PrtString(answerback,len_answerback) == D2_ERROR)
{                           /* return error                           */
  return(D2_ERROR);
}

			    /* return no error                        */
return (D2_NOERROR);

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_usb_is_ttxid                                    */
/*                                                                    */
/* AUTHOR       : Rustemeyer, D AP 11                                 */
/* DATE         : 22.04.91                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function checks a string of given length. The string must */
/*      consist of up to six d21_ttxid-structures, followed by        */
/*      Printable Strings, T61-Strings or Octet-Strings, depending    */
/*      on the string-type in d21_ttxid.                              */
/*      The first string-type must be "Terminal".                     */
/*                                                                    */
/* RETURN VALUE :   D2_NOERROR      syntax-check successful           */
/*                  D2_ERROR:       syntax-check fails                */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_usb_is_ttxid(
     byte *val,         /* IN    -  attribute value to check          */
     signed16 len)      /* IN    -  length of attribute value         */

{
/*--------------------------------------------------------------------*/
/*                  D E C L A R A T I O N S                           */
/*--------------------------------------------------------------------*/
signed32     type_control[5];
signed32     i;
byte         *max_len = val + len;
d21_ttxid    *d21_ttxid_p = (d21_ttxid *) val;

/*---------------------------------------------------------------------*/
/*                            C O D E                                  */
/*---------------------------------------------------------------------*/
for (i=0; i<5; i++)
    type_control[i]=0;

/*------------------------------------------------------------*/
/* test first component (must be Terminal-struct)             */
/*------------------------------------------------------------*/
if (d21_ttxid_p->d2_str_type != D2_TERM)
{
  return(D2_ERROR);
}

val += sizeof(d21_ttxid);

if                          /* TTX-ID too long                        */
  (d21_ttxid_p->d2_str_len > D26_UB_TTX_TERMID)
{
  return(D2_ERROR);
}
else if                     /* illegal character in Teletex terminal  */
       (d27_605_is_PrtString (val,d21_ttxid_p->d2_str_len) != D2_NOERROR)
{
  return (D2_ERROR);
}

val += d21_ttxid_p->d2_offset;

/*------------------------------------------------------------*/
/* test following components up to max_len                    */
/*------------------------------------------------------------*/
while (val < max_len)
   {
      d21_ttxid_p = (d21_ttxid *) val;
      val += sizeof(d21_ttxid);
      switch (d21_ttxid_p->d2_str_type)
	{
	   case D2_MISC:
		if (type_control[0]==1)
		{
		    return (D2_ERROR);
		}
		else
		{
		    type_control[0]=1;
		}
		if (d27_601_is_T61String (val,d21_ttxid_p->d2_str_len)
		    != D2_NOERROR)
		{
		   return (D2_ERROR);
		}
		break;
	   case D2_CTRL:
		if (type_control[1]==1)
		{
		    return (D2_ERROR);
		}
		else
		{
		    type_control[1]=1;
		}
		if (d27_601_is_T61String (val,d21_ttxid_p->d2_str_len)
		    != D2_NOERROR)
		{
		   return (D2_ERROR);
		}
		break;
	   case D2_GRPH:
		if (type_control[2]==1)
		{
		    return (D2_ERROR);
		}
		else
		{
		    type_control[2]=1;
		}
		if (d27_601_is_T61String (val,d21_ttxid_p->d2_str_len)
		    != D2_NOERROR)
		{
		   return (D2_ERROR);
		}
		break;
	   case D2_PAGE:
		if (type_control[3]==1)
		{
		    return (D2_ERROR);
		}
		else
		{
		    type_control[3]=1;
		}
		break;
	   case D2_PRIV:
		if (type_control[4]==1)
		{
		    return (D2_ERROR);
		}
		else
		{
		    type_control[4]=1;
		}
		break;
	   default:
		return (D2_ERROR);
	}

     val += d21_ttxid_p->d2_offset;
   }

return (D2_NOERROR);

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_usc_is_faxnr                                    */
/*                                                                    */
/* AUTHOR       : Rustemeyer, D AP 11                                 */
/* DATE         : 22.04.91                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function checks a string of given length. The string must */
/*      consist of a d21_faxnr-structure, followed by a Printable     */
/*      string of length d21_faxnr.d2_str_len.                        */
/*                                                                    */
/* RETURN VALUE :   D2_NOERROR      syntax-check successful           */
/*                  D2_ERROR:       syntax-check fails                */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_usc_is_faxnr(
     byte *val)         /* IN    -  attribute value to check          */

{
/*--------------------------------------------------------------------*/
/*                  D E C L A R A T I O N S                           */
/*--------------------------------------------------------------------*/
d21_faxnr     *d21_faxnr_p = (d21_faxnr *) val;
/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/
val += sizeof (d21_faxnr);
if                          /* FAX number component is too long       */
  (d21_faxnr_p->d2_str_len > D26_UB_PHONE_NUMBER)
{                           /* return error                           */
  return(D2_ERROR);
}
else if                 /* Telephone number component is not printable*/
       (d27_605_is_PrtString(val,d21_faxnr_p->d2_str_len)
	!= D2_NOERROR )
{                       /* return error                               */
  return(D2_ERROR);
}
else if                 /* G3FaxNonBasicParameters are present        */
       (d21_faxnr_p->d2_offset > 0)
{                       /* reset value to G3FaxNonBasicParameters     */
  val += d21_faxnr_p->d2_offset;
  if                    /* illegal bits contained in Bit string       */
    (*(signed32 *)val &
     ~(D2_TWO_DIMENSIONAL | D2_FINE_RESOLUTION | D2_UNLIMITED_LENGTH |
       D2_B4_LENGTH | D2_A3_WIDTH | D2_B4_WIDTH | D2_UNCOMPRESSED) != 0L)
  {                     /* return error                               */
    return (D2_ERROR);
  }
}

return(D2_NOERROR);

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_usd_is_OR_addr                                  */
/*                                                                    */
/* AUTHOR       : G. Rustemeyer, D AP 11                              */
/* DATE         : 22.11.91                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function checks a string of given length. The string must */
/*      consist of one or more structures D21_OR_address_part, each   */
/*      followed by a valid string-list or by a string of appropriate */
/*      type (Printable, T61, Numeric or Psap) or by an integer.      */
/*      The fields D21_OR_address_part.d21_ora_tag must contain       */
/*      valid address-part-tags.                                      */
/*      Only certain combinations of address-parts are allowed: these */
/*      are the types Mnemonic, Numeric, Structured Postal, Unstr.    */
/*      Postal or Terminal.                                           */
/*                                                                    */
/* RETURN VALUE :   D2_NOERROR      syntax-check successful           */
/*                  D2_ERROR:       syntax-check fails                */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_usd_is_OR_addr(
     byte *val,         /* IN    -  attribute value to check          */
     signed16 len)      /* IN    -  length of attribute value         */

{
/*---------------------------------------------------------------------*/
/*                   D E C L A R A T I O N S                           */
/*---------------------------------------------------------------------*/
byte *val_save   = val;
byte *end_of_val = val + len;
byte *string_p, *end_of_strings;

D21_OR_address_part  *addr_part_p;

signed32   t61_count = 0, nbstr, dom_type_nbstr=0;
size_t     str_len;
unsigned16 addrpart_len;

/*---------------------------------------------------------------------*/
/* Declare bitmasks for given / not given types of address-parts       */
/*---------------------------------------------------------------------*/

unsigned32 patt1_input = 0; /* marks NULL-values in addr-part 1 to 32  */
unsigned32 patt2_input = 0; /* marks NULL-values in addr-part 33 to 43 */

/*---------------------------------------------------------------*/
/* indices for bit-masks                                         */
/*---------------------------------------------------------------*/

/*--------------------------------------------------------*/
/* first 4 bytes (patt1_input, mask1_x_and, mask1_x_or)   */
/*--------------------------------------------------------*/
#define BIT_ORA_ADMD_N      (1 <<  0)
#define BIT_ORA_COM_N       (1 <<  1)
#define BIT_ORA_CTRY_N      (1 <<  2)
#define BIT_ORA_DOM_T1      (1 <<  3)
#define BIT_ORA_DOM_T2      (1 <<  4)
#define BIT_ORA_DOM_T3      (1 <<  5)
#define BIT_ORA_DOM_T4      (1 <<  6)
#define BIT_ORA_DOM_V1      (1 <<  7)
#define BIT_ORA_DOM_V2      (1 <<  8)
#define BIT_ORA_DOM_V3      (1 <<  9)
#define BIT_ORA_DOM_V4      (1 << 10)
#define BIT_ORA_GEN         (1 << 11)
#define BIT_ORA_GIV_N       (1 << 12)
#define BIT_ORA_INIT        (1 << 13)
#define BIT_ORA_ISDN_N      (1 << 14)
#define BIT_ORA_ISDN_S      (1 << 15)
#define BIT_ORA_NUM_ID      (1 << 16)
#define BIT_ORA_ORG_N       (1 << 17)
#define BIT_ORA_ORGU_N1     (1 << 18)
#define BIT_ORA_ORGU_N2     (1 << 19)
#define BIT_ORA_ORGU_N3     (1 << 20)
#define BIT_ORA_ORGU_N4     (1 << 21)
#define BIT_ORA_P_A_D       (1 << 22)
#define BIT_ORA_P_A_F       (1 << 23)
#define BIT_ORA_P_CODE      (1 << 24)
#define BIT_ORA_P_CTRY      (1 << 25)
#define BIT_ORA_P_D_P_N     (1 << 26)
#define BIT_ORA_P_D_S_N     (1 << 27)
#define BIT_ORA_P_G_D_A     (1 << 28)
#define BIT_ORA_P_LOC       (1 << 29)
#define BIT_ORA_P_O_B_N     (1 << 30)
#define BIT_ORA_P_O_N       ((unsigned)1 << 31)

/*--------------------------------------------------------*/
/* second 4 bytes (patt2_input, mask2_x_and, mask2_x_or)  */
/*--------------------------------------------------------*/
#define BIT_ORA_P_O_NUM     (1 <<  0)
#define BIT_ORA_P_ORG_N     (1 <<  1)
#define BIT_ORA_P_P_D       (1 <<  2)
#define BIT_ORA_P_P_N       (1 <<  3)
#define BIT_ORA_P_S_A       (1 <<  4)
#define BIT_ORA_PRES_A      (1 <<  5)
#define BIT_ORA_PRMD_N      (1 <<  6)
#define BIT_ORA_SURNAME     (1 <<  7)
#define BIT_ORA_T_ID        (1 <<  8)
#define BIT_ORA_T_T         (1 <<  9)
#define BIT_ORA_X121_A      (1 << 10)
#define BIT_ORA_P_A_L       (1 << 11)

#define ALL_0    0
#define ALL_1    ~0

/*---------------------------------------------------------------------*/
/* Declare bitmasks for checking rules                                 */
/*                                                                     */
/* maskn_x_and: contains "1" for each addr-part, that must be NULL     */
/*      (mask1_x_and: part 1 to 32, mask2_x_and: part 33 to 43)        */
/* maskn_x_or: contains "0" for each addr-part, that must be NOT NULL  */
/*      (mask1_x_or: part 1 to 32, mask2_x_or: part 33 to 43)          */
/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------*/
/* masks for checking NULL-pointers of Mnemonic OR-address       */
/*---------------------------------------------------------------*/
unsigned32 mask1_mn_and    =   BIT_ORA_ISDN_N
			     | BIT_ORA_ISDN_S
			     | BIT_ORA_NUM_ID
			     | BIT_ORA_P_A_D
			     | BIT_ORA_P_A_F
			     | BIT_ORA_P_CODE
			     | BIT_ORA_P_CTRY
			     | BIT_ORA_P_D_P_N
			     | BIT_ORA_P_D_S_N
			     | BIT_ORA_P_G_D_A
			     | BIT_ORA_P_LOC
			     | BIT_ORA_P_O_B_N
			     | BIT_ORA_P_O_N;

unsigned32 mask2_mn_and    =   BIT_ORA_P_O_NUM
			     | BIT_ORA_P_ORG_N
			     | BIT_ORA_P_P_D
			     | BIT_ORA_P_P_N
			     | BIT_ORA_P_S_A
			     | BIT_ORA_PRES_A
			     | BIT_ORA_T_ID
			     | BIT_ORA_T_T
			     | BIT_ORA_X121_A
			     | BIT_ORA_P_A_L;

/*---------------------------------------------------------------*/
/* masks for checking NOT-NULL-pointers of Mnemonic OR-address   */
/*---------------------------------------------------------------*/
unsigned32 mask1_mn_or     = (unsigned32)(ALL_1
			    & ~BIT_ORA_ADMD_N & ~BIT_ORA_CTRY_N);

unsigned32 mask2_mn_or     = (unsigned32)(ALL_1);

/*---------------------------------------------------------------*/
/* masks for checking NULL-pointers of Numeric OR-address        */
/*---------------------------------------------------------------*/
unsigned32 mask1_n_and     =   BIT_ORA_COM_N
			     | BIT_ORA_GEN
			     | BIT_ORA_GIV_N
			     | BIT_ORA_INIT
			     | BIT_ORA_ISDN_N
			     | BIT_ORA_ISDN_S
			     | BIT_ORA_ORG_N
			     | BIT_ORA_ORGU_N1
			     | BIT_ORA_ORGU_N2
			     | BIT_ORA_ORGU_N3
			     | BIT_ORA_ORGU_N4
			     | BIT_ORA_P_A_D
			     | BIT_ORA_P_A_F
			     | BIT_ORA_P_CODE
			     | BIT_ORA_P_CTRY
			     | BIT_ORA_P_D_P_N
			     | BIT_ORA_P_D_S_N
			     | BIT_ORA_P_G_D_A
			     | BIT_ORA_P_LOC
			     | BIT_ORA_P_O_B_N
			     | BIT_ORA_P_O_N;

unsigned32 mask2_n_and     =   BIT_ORA_P_O_NUM
			     | BIT_ORA_P_ORG_N
			     | BIT_ORA_P_P_D
			     | BIT_ORA_P_P_N
			     | BIT_ORA_P_S_A
			     | BIT_ORA_PRES_A
			     | BIT_ORA_SURNAME
			     | BIT_ORA_T_ID
			     | BIT_ORA_T_T
			     | BIT_ORA_X121_A
			     | BIT_ORA_P_A_L;

/*---------------------------------------------------------------*/
/* masks for checking NOT-NULL-pointers of Numeric OR-address    */
/*---------------------------------------------------------------*/
unsigned32 mask1_n_or      = (unsigned32)(ALL_1
			    & ~BIT_ORA_ADMD_N
			    & ~BIT_ORA_CTRY_N
			    & ~BIT_ORA_NUM_ID);

unsigned32 mask2_n_or      = (unsigned32)(ALL_1);

/*---------------------------------------------------------------*/
/* masks for checking NULL-pointers of S. Postal OR-address      */
/*---------------------------------------------------------------*/
unsigned32 mask1_sp_and    =   BIT_ORA_COM_N
			     | BIT_ORA_DOM_T1
			     | BIT_ORA_DOM_T2
			     | BIT_ORA_DOM_T3
			     | BIT_ORA_DOM_T4
			     | BIT_ORA_DOM_V1
			     | BIT_ORA_DOM_V2
			     | BIT_ORA_DOM_V3
			     | BIT_ORA_DOM_V4
			     | BIT_ORA_GEN
			     | BIT_ORA_GIV_N
			     | BIT_ORA_INIT
			     | BIT_ORA_ISDN_N
			     | BIT_ORA_ISDN_S
			     | BIT_ORA_NUM_ID
			     | BIT_ORA_ORG_N
			     | BIT_ORA_ORGU_N1
			     | BIT_ORA_ORGU_N2
			     | BIT_ORA_ORGU_N3
			     | BIT_ORA_ORGU_N4
			     | BIT_ORA_P_A_F;

unsigned32 mask2_sp_and    =   BIT_ORA_PRES_A
			     | BIT_ORA_SURNAME
			     | BIT_ORA_T_ID
			     | BIT_ORA_T_T
			     | BIT_ORA_X121_A
			     | BIT_ORA_P_A_L;

/*---------------------------------------------------------------*/
/* masks for checking NOT-NULL-pointers of s. Postal OR-address  */
/*---------------------------------------------------------------*/
unsigned32 mask1_sp_or     = (unsigned32)(ALL_1
			    & ~BIT_ORA_ADMD_N
			    & ~BIT_ORA_CTRY_N
			    & ~BIT_ORA_P_CODE
			    & ~BIT_ORA_P_CTRY);

unsigned32 mask2_sp_or     = (unsigned32)(ALL_1);

/*---------------------------------------------------------------*/
/* masks for checking NULL-pointers of U. Postal OR-address      */
/*---------------------------------------------------------------*/
unsigned32 mask1_up_and    =   BIT_ORA_COM_N
			     | BIT_ORA_DOM_T1
			     | BIT_ORA_DOM_T2
			     | BIT_ORA_DOM_T3
			     | BIT_ORA_DOM_T4
			     | BIT_ORA_DOM_V1
			     | BIT_ORA_DOM_V2
			     | BIT_ORA_DOM_V3
			     | BIT_ORA_DOM_V4
			     | BIT_ORA_GEN
			     | BIT_ORA_GIV_N
			     | BIT_ORA_INIT
			     | BIT_ORA_ISDN_N
			     | BIT_ORA_ISDN_S
			     | BIT_ORA_NUM_ID
			     | BIT_ORA_ORG_N
			     | BIT_ORA_ORGU_N1
			     | BIT_ORA_ORGU_N2
			     | BIT_ORA_ORGU_N3
			     | BIT_ORA_ORGU_N4
			     | BIT_ORA_P_A_D
			     | BIT_ORA_P_D_P_N
			     | BIT_ORA_P_G_D_A
			     | BIT_ORA_P_LOC
			     | BIT_ORA_P_O_B_N
			     | BIT_ORA_P_O_N;

unsigned32 mask2_up_and    =   BIT_ORA_P_O_NUM
			     | BIT_ORA_P_ORG_N
			     | BIT_ORA_P_P_D
			     | BIT_ORA_P_P_N
			     | BIT_ORA_P_S_A
			     | BIT_ORA_PRES_A
			     | BIT_ORA_SURNAME
			     | BIT_ORA_T_ID
			     | BIT_ORA_T_T
			     | BIT_ORA_X121_A;

/*---------------------------------------------------------------*/
/* masks for checking NOT-NULL-pointers of U. Postal OR-address  */
/*---------------------------------------------------------------*/
unsigned32 mask1_up_or     = (unsigned32)(ALL_1
			    & ~BIT_ORA_ADMD_N
			    & ~BIT_ORA_CTRY_N
			    & ~BIT_ORA_P_CODE
			    & ~BIT_ORA_P_CTRY);

unsigned32 mask2_up_or     = (unsigned32)(ALL_1);

/*---------------------------------------------------------------*/
/* masks for checking NULL-pointers of Terminal OR-address       */
/*---------------------------------------------------------------*/
unsigned32 mask1_t_and    =    BIT_ORA_COM_N
			     | BIT_ORA_GEN
			     | BIT_ORA_GIV_N
			     | BIT_ORA_INIT
			     | BIT_ORA_NUM_ID
			     | BIT_ORA_ORG_N
			     | BIT_ORA_ORGU_N1
			     | BIT_ORA_ORGU_N2
			     | BIT_ORA_ORGU_N3
			     | BIT_ORA_ORGU_N4
			     | BIT_ORA_P_A_D
			     | BIT_ORA_P_A_F
			     | BIT_ORA_P_CODE
			     | BIT_ORA_P_CTRY
			     | BIT_ORA_P_D_P_N
			     | BIT_ORA_P_D_S_N
			     | BIT_ORA_P_G_D_A
			     | BIT_ORA_P_LOC
			     | BIT_ORA_P_O_B_N
			     | BIT_ORA_P_O_N;

unsigned32 mask2_t_and     =   BIT_ORA_P_O_NUM
			     | BIT_ORA_P_ORG_N
			     | BIT_ORA_P_P_D
			     | BIT_ORA_P_P_N
			     | BIT_ORA_P_S_A
			     | BIT_ORA_SURNAME
			     | BIT_ORA_P_A_L;

/*---------------------------------------------------------------*/
/* masks for checking NOT-NULL-pointers of Terminal OR-address   */
/*---------------------------------------------------------------*/

unsigned32 mask1_t_or     = (unsigned32)(ALL_1);
unsigned32 mask2_t_or     = (unsigned32)(ALL_1);

/*---------------------------------------------------------------------*/
/*                            C O D E                                  */
/*---------------------------------------------------------------------*/
while (val < end_of_val)
   /*------------------------------------------------------------------*/
   /* check strings for valid character-sets                           */
   /*------------------------------------------------------------------*/
   {
      addr_part_p = (D21_OR_address_part *) val;
      if                /* address part length invalid               */
	((signed32)(addrpart_len=addr_part_p->d21_ora_len) <= 1)
      {                 /* return error                              */
	 return (D2_ERROR);
      }
      val += sizeof (D21_OR_address_part);
      switch (addr_part_p->d21_ora_tag)
	 {
	    case D21_ORA_ADMD_N:
	       /*------------------------------------------------------*/
	       /* type D2_str_list (type D26_ADMD_STRLIST)             */
	       /*------------------------------------------------------*/
	       if(d26_usx_is_str_list(val,addrpart_len,D26_ADMD_STRLIST,
		  &nbstr) == D2_ERROR)
	       {
		 return(D2_ERROR);
	       }
	       break;
	    case D21_ORA_CTRY_N:
	    case D21_ORA_P_CODE:
	    case D21_ORA_P_CTRY:
	    case D21_ORA_PRMD_N:
	       /*------------------------------------------------------*/
	       /* type D2_str_list (type D26_PRT_OR_NUM)               */
	       /*------------------------------------------------------*/
	       if (d26_usx_is_str_list(val,addrpart_len,D26_PRT_OR_NUM,
		   &nbstr) == D2_ERROR)
	       {
		  return (D2_ERROR);
	       }
	       break;
	    case D21_ORA_COM_N:
	    case D21_ORA_GEN:
	    case D21_ORA_GIV_N:
	    case D21_ORA_INIT:
	    case D21_ORA_ORG_N:
	    case D21_ORA_ORGU_N1:
	    case D21_ORA_ORGU_N2:
	    case D21_ORA_ORGU_N3:
	    case D21_ORA_ORGU_N4:
	    case D21_ORA_P_A_D:
	    case D21_ORA_P_D_P_N:
	    case D21_ORA_P_G_D_A:
	    case D21_ORA_P_LOC:
	    case D21_ORA_P_O_B_N:
	    case D21_ORA_P_O_N:
	    case D21_ORA_P_O_NUM:
	    case D21_ORA_P_ORG_N:
	    case D21_ORA_P_P_D:
	    case D21_ORA_P_P_N:
	    case D21_ORA_P_S_A:
	    case D21_ORA_SURNAME:
	       /*------------------------------------------------------*/
	       /* type D2_str_list (type D26_PRT_ANDOR_T61)            */
	       /*------------------------------------------------------*/
	       if (d26_usx_is_str_list(val,addrpart_len,D26_PRT_ANDOR_T61,
		   &nbstr) == D2_ERROR)
	       {
		 return(D2_ERROR);
	       }
	       break;
	    case D21_ORA_DOM_T1:
	    case D21_ORA_DOM_T2:
	    case D21_ORA_DOM_T3:
	    case D21_ORA_DOM_T4:
	       /*------------------------------------------------------*/
	       /* type D2_str_list (Domain Defined Type)               */
	       /*------------------------------------------------------*/
	       if (d26_usx_is_str_list(val,addrpart_len,D26_PRT_ANDOR_T61,
		   &nbstr) == D2_ERROR)
	       {
		  return (D2_ERROR);
	       }
	       dom_type_nbstr = nbstr;
	       break;
	    case D21_ORA_DOM_V1:
	    case D21_ORA_DOM_V2:
	    case D21_ORA_DOM_V3:
	    case D21_ORA_DOM_V4:
	       /*------------------------------------------------------*/
	       /* type D2_str_list (Domain Defined Value)              */
	       /*------------------------------------------------------*/
	       if (d26_usx_is_str_list(val,addrpart_len,D26_PRT_ANDOR_T61,
		   &nbstr) == D2_ERROR)
	       {
		  return (D2_ERROR);
	       }
	       if (nbstr != dom_type_nbstr)
	       {
		 return(D2_ERROR);
	       }
	       dom_type_nbstr=0;
	       break;
	    case D21_ORA_P_A_L:
	       /*------------------------------------------------------*/
	       /* type postal-address-in-lines (structure D21_str_list,*/
	       /*      followed by up to 6 t61-strings)                */
	       /*------------------------------------------------------*/
	       string_p = val;
	       end_of_strings = string_p + addr_part_p->d21_ora_len;
	       while (string_p < end_of_strings)
	       {
		  t61_count ++;
		  str_len = strlen((char *)string_p);
		  if (d27_601_is_T61String(string_p,(signed16)str_len)
		      == D2_ERROR)
		  {
		    return(D2_ERROR);
		  }
		  string_p += str_len + 1;
	       }
	       if (t61_count > 6)
	       {
		 return(D2_ERROR);
	       }
	       break;
	    case D21_ORA_P_D_S_N:
	    case D21_ORA_T_ID:
	       /*------------------------------------------------------*/
	       /* type Print_string                                    */
	       /*------------------------------------------------------*/
	       if (d27_605_is_PrtString (val,addrpart_len-1) == D2_ERROR)
	       {
		  return (D2_ERROR);
	       }
	       break;
	    case D21_ORA_ISDN_N:
	    case D21_ORA_ISDN_S:
	    case D21_ORA_NUM_ID:
	    case D21_ORA_X121_A:
	       /*------------------------------------------------------*/
	       /* type Numeric_string                                  */
	       /*------------------------------------------------------*/
	       if (d26_us6_is_NumString (val,addrpart_len-1) == D2_ERROR)
	       {
		 return(D2_ERROR);
	       }
	       break;
	    case D21_ORA_P_A_F:
	       /*------------------------------------------------------*/
	       /* type T61_string                                      */
	       /*------------------------------------------------------*/
	       if (d27_601_is_T61String(val,addrpart_len - 1)
		   == D2_ERROR)
	       {
		 return(D2_ERROR);
	       }
	       break;
	    case D21_ORA_PRES_A:
	       /*------------------------------------------------------*/
	       /* type Psap_string                                     */
	       /*------------------------------------------------------*/
	       break;
	    case D21_ORA_T_T:
	       /*------------------------------------------------------*/
	       /* type Int                                             */
	       /*------------------------------------------------------*/
	       if ( *((signed32 *) val) < D2_MHS_T_T_LBOUND  ||
		    *((signed32 *) val) > D2_MHS_T_T_UBOUND    )
	       {
		 return(D2_ERROR);
	       }
	       break;
	    default:
	       return (D2_ERROR);
	 }

      val += addr_part_p->d21_ora_off;

   }

/*---------------------------------------------------------------------*/
/* check for valid form of OR-address                                  */
/*---------------------------------------------------------------------*/

/*---------------------------------------------------------------*/
/* set bits in patterns of input-NULL/NOT-NULL-values            */
/*---------------------------------------------------------------*/
while (val_save < end_of_val)
   {
      addr_part_p = (D21_OR_address_part *) val_save;
      val_save += sizeof (D21_OR_address_part) +
		  addr_part_p->d21_ora_off;
      switch (addr_part_p->d21_ora_tag)
	 {
	    case D21_ORA_ADMD_N:
	       patt1_input = patt1_input | BIT_ORA_ADMD_N;
	       break;
	    case D21_ORA_CTRY_N:
	       patt1_input = patt1_input | BIT_ORA_CTRY_N;
	       break;
	    case D21_ORA_P_CODE:
	       patt1_input = patt1_input | BIT_ORA_P_CODE;
	       break;
	    case D21_ORA_P_CTRY:
	       patt1_input = patt1_input | BIT_ORA_P_CTRY;
	       break;
	    case D21_ORA_PRMD_N:
	       patt2_input = patt2_input | BIT_ORA_PRMD_N;
	       break;
	    case D21_ORA_COM_N:
	       patt1_input = patt1_input | BIT_ORA_COM_N;
	       break;
	    case D21_ORA_DOM_T1:
	       patt1_input = patt1_input | BIT_ORA_DOM_T1;
	       break;
	    case D21_ORA_DOM_T2:
	       patt1_input = patt1_input | BIT_ORA_DOM_T2;
	       break;
	    case D21_ORA_DOM_T3:
	       patt1_input = patt1_input | BIT_ORA_DOM_T3;
	       break;
	    case D21_ORA_DOM_T4:
	       patt1_input = patt1_input | BIT_ORA_DOM_T4;
	       break;
	    case D21_ORA_DOM_V1:
	       patt1_input = patt1_input | BIT_ORA_DOM_V1;
	       break;
	    case D21_ORA_DOM_V2:
	       patt1_input = patt1_input | BIT_ORA_DOM_V2;
	       break;
	    case D21_ORA_DOM_V3:
	       patt1_input = patt1_input | BIT_ORA_DOM_V3;
	       break;
	    case D21_ORA_DOM_V4:
	       patt1_input = patt1_input | BIT_ORA_DOM_V4;
	       break;
	    case D21_ORA_GEN:
	       patt1_input = patt1_input | BIT_ORA_GEN;
	       break;
	    case D21_ORA_GIV_N:
	       patt1_input = patt1_input | BIT_ORA_GIV_N;
	       break;
	    case D21_ORA_INIT:
	       patt1_input = patt1_input | BIT_ORA_INIT;
	       break;
	    case D21_ORA_ORG_N:
	       patt1_input = patt1_input | BIT_ORA_ORG_N;
	       break;
	    case D21_ORA_ORGU_N1:
	       patt1_input = patt1_input | BIT_ORA_ORGU_N1;
	       break;
	    case D21_ORA_ORGU_N2:
	       patt1_input = patt1_input | BIT_ORA_ORGU_N2;
	       break;
	    case D21_ORA_ORGU_N3:
	       patt1_input = patt1_input | BIT_ORA_ORGU_N3;
	       break;
	    case D21_ORA_ORGU_N4:
	       patt1_input = patt1_input | BIT_ORA_ORGU_N4;
	       break;
	    case D21_ORA_P_A_D:
	       patt1_input = patt1_input | BIT_ORA_P_A_D;
	       break;
	    case D21_ORA_P_D_P_N:
	       patt1_input = patt1_input | BIT_ORA_P_D_P_N;
	       break;
	    case D21_ORA_P_G_D_A:
	       patt1_input = patt1_input | BIT_ORA_P_G_D_A;
	       break;
	    case D21_ORA_P_LOC:
	       patt1_input = patt1_input | BIT_ORA_P_LOC;
	       break;
	    case D21_ORA_P_O_B_N:
	       patt1_input = patt1_input | BIT_ORA_P_O_B_N;
	       break;
	    case D21_ORA_P_O_N:
	       patt1_input = patt1_input | BIT_ORA_P_O_N;
	       break;
	    case D21_ORA_P_O_NUM:
	       patt2_input = patt2_input | BIT_ORA_P_O_NUM;
	       break;
	    case D21_ORA_P_ORG_N:
	       patt2_input = patt2_input | BIT_ORA_P_ORG_N;
	       break;
	    case D21_ORA_P_P_D:
	       patt2_input = patt2_input | BIT_ORA_P_P_D;
	       break;
	    case D21_ORA_P_P_N:
	       patt2_input = patt2_input | BIT_ORA_P_P_N;
	       break;
	    case D21_ORA_P_S_A:
	       patt2_input = patt2_input | BIT_ORA_P_S_A;
	       break;
	    case D21_ORA_SURNAME:
	       patt2_input = patt2_input | BIT_ORA_SURNAME;
	       break;
	    case D21_ORA_P_A_L:
	       patt2_input = patt2_input | BIT_ORA_P_A_L;
	       break;
	    case D21_ORA_P_D_S_N:
	       patt1_input = patt1_input | BIT_ORA_P_D_S_N;
	       break;
	    case D21_ORA_T_ID:
	       patt2_input = patt2_input | BIT_ORA_T_ID;
	       break;
	    case D21_ORA_ISDN_N:
	       patt1_input = patt1_input | BIT_ORA_ISDN_N;
	       break;
	    case D21_ORA_ISDN_S:
	       patt1_input = patt1_input | BIT_ORA_ISDN_S;
	       break;
	    case D21_ORA_NUM_ID:
	       patt1_input = patt1_input | BIT_ORA_NUM_ID;
	       break;
	    case D21_ORA_X121_A:
	       patt2_input = patt2_input | BIT_ORA_X121_A;
	       break;
	    case D21_ORA_P_A_F:
	       patt1_input = patt1_input | BIT_ORA_P_A_F;
	       break;
	    case D21_ORA_PRES_A:
	       patt2_input = patt2_input | BIT_ORA_PRES_A;
	       break;
	    case D21_ORA_T_T:
	       patt2_input = patt2_input | BIT_ORA_T_T;
	       break;
	    default:
	       return (D2_ERROR);
	 }

   }

/*---------------------------------------------------------------*/
/*  valid Mnemonic                                               */
/*                                                               */
/*  (0): check all values that must be NULL / NOT NULL           */
/*  (1): PERS.NAME ATTRS: if Given Name or Initials or Generation*/
/*       are present, then Surname must be present too           */
/*  (2): if any Domain-type is present then the associated       */
/*       Domain-value must also be present                       */
/*  (3): if any Domain-value is present then the associated      */
/*       Domain-type must also be present                        */
/*---------------------------------------------------------------*/
if  ( (patt1_input & mask1_mn_and) == ALL_0         &&   /* (0) */
      (patt2_input & mask2_mn_and) == ALL_0         &&   /* (") */
      (patt1_input | mask1_mn_or)  == ALL_1         &&   /* (") */
      (patt2_input | mask2_mn_or)  == ALL_1         &&   /* (") */
     ((patt1_input | ~BIT_ORA_GIV_N) != ALL_1 &&         /* (1) */
      (patt1_input | ~BIT_ORA_INIT)  != ALL_1 &&         /* (") */
      (patt1_input | ~BIT_ORA_GEN)   != ALL_1   ||       /* (") */
      (patt2_input | ~BIT_ORA_SURNAME) == ALL_1   ) &&   /* (") */
     ((patt1_input | ~BIT_ORA_DOM_T1) != ALL_1 ||        /* (2) */
      (patt1_input | ~BIT_ORA_DOM_V1) == ALL_1    ) &&   /* (") */
     ((patt1_input | ~BIT_ORA_DOM_T2) != ALL_1 ||        /* (") */
      (patt1_input | ~BIT_ORA_DOM_V2) == ALL_1    ) &&   /* (") */
     ((patt1_input | ~BIT_ORA_DOM_T3) != ALL_1 ||        /* (") */
      (patt1_input | ~BIT_ORA_DOM_V3) == ALL_1    ) &&   /* (") */
     ((patt1_input | ~BIT_ORA_DOM_T4) != ALL_1 ||        /* (") */
      (patt1_input | ~BIT_ORA_DOM_V4) == ALL_1    ) &&   /* (") */
     ((patt1_input | ~BIT_ORA_DOM_V1) != ALL_1 ||        /* (3) */
      (patt1_input | ~BIT_ORA_DOM_T1) == ALL_1    ) &&   /* (") */
     ((patt1_input | ~BIT_ORA_DOM_V2) != ALL_1 ||        /* (") */
      (patt1_input | ~BIT_ORA_DOM_T2) == ALL_1    ) &&   /* (") */
     ((patt1_input | ~BIT_ORA_DOM_V3) != ALL_1 ||        /* (") */
      (patt1_input | ~BIT_ORA_DOM_T3) == ALL_1    ) &&   /* (") */
     ((patt1_input | ~BIT_ORA_DOM_V4) != ALL_1 ||        /* (") */
      (patt1_input | ~BIT_ORA_DOM_T4) == ALL_1    )   )  /* (") */
{
  return(D2_NOERROR);
}

/*---------------------------------------------------------------*/
/*  valid Numeric                                                */
/*                                                               */
/*  (0): check all values that must be NULL / NOT NULL           */
/*  (1): if any Domain-type is present then the associated       */
/*       Domain-value must also be present                       */
/*  (2): if any Domain-value is present then the associated      */
/*       Domain-type must also be present                        */
/*---------------------------------------------------------------*/
if  ( (patt1_input & mask1_n_and) == ALL_0          &&   /* (0) */
      (patt2_input & mask2_n_and) == ALL_0          &&   /* (") */
      (patt1_input | mask1_n_or)  == ALL_1          &&   /* (") */
      (patt2_input | mask2_n_or)  == ALL_1          &&   /* (") */
     ((patt1_input | ~BIT_ORA_DOM_T1) != ALL_1 ||        /* (1) */
      (patt1_input | ~BIT_ORA_DOM_V1) == ALL_1    ) &&   /* (") */
     ((patt1_input | ~BIT_ORA_DOM_T2) != ALL_1 ||        /* (") */
      (patt1_input | ~BIT_ORA_DOM_V2) == ALL_1    ) &&   /* (") */
     ((patt1_input | ~BIT_ORA_DOM_T3) != ALL_1 ||        /* (") */
      (patt1_input | ~BIT_ORA_DOM_V3) == ALL_1    ) &&   /* (") */
     ((patt1_input | ~BIT_ORA_DOM_T4) != ALL_1 ||        /* (") */
      (patt1_input | ~BIT_ORA_DOM_V4) == ALL_1    ) &&   /* (") */
     ((patt1_input | ~BIT_ORA_DOM_V1) != ALL_1 ||        /* (2) */
      (patt1_input | ~BIT_ORA_DOM_T1) == ALL_1    ) &&   /* (") */
     ((patt1_input | ~BIT_ORA_DOM_V2) != ALL_1 ||        /* (") */
      (patt1_input | ~BIT_ORA_DOM_T2) == ALL_1    ) &&   /* (") */
     ((patt1_input | ~BIT_ORA_DOM_V3) != ALL_1 ||        /* (") */
      (patt1_input | ~BIT_ORA_DOM_T3) == ALL_1    ) &&   /* (") */
     ((patt1_input | ~BIT_ORA_DOM_V4) != ALL_1 ||        /* (") */
      (patt1_input | ~BIT_ORA_DOM_T4) == ALL_1    )   )  /* (") */
{
  return(D2_NOERROR);
}

/*---------------------------------------------------------------*/
/*  valid Structured Postal                                      */
/*                                                               */
/*  (0): check all values that must be NULL / NOT NULL           */
/*---------------------------------------------------------------*/
if   ( (patt1_input & mask1_sp_and) == ALL_0   &&        /* (0) */
       (patt2_input & mask2_sp_and) == ALL_0   &&        /* (") */
       (patt1_input | mask1_sp_or)  == ALL_1   &&        /* (") */
       (patt2_input | mask2_sp_or)  == ALL_1     )       /* (") */
{
  return(D2_NOERROR);
}

/*---------------------------------------------------------------*/
/*  valid Unstructured Postal                                    */
/*                                                               */
/*  (0): check all values that must be NULL / NOT NULL           */
/*  (1): Mandatory group POST.ADDR.ATTRS: at least one of the    */
/*       fields must be present                                  */
/*---------------------------------------------------------------*/
if ( (patt1_input & mask1_up_and) == ALL_0       &&      /* (0) */
     (patt2_input & mask2_up_and) == ALL_0       &&      /* (") */
     (patt1_input | mask1_up_or)  == ALL_1       &&      /* (") */
     (patt2_input | mask2_up_or)  == ALL_1       &&      /* (") */
    ((patt1_input | ~BIT_ORA_P_A_F) == ALL_1 ||          /* (1) */
     (patt2_input | ~BIT_ORA_P_A_L) == ALL_1   )     )   /* (") */
{
   return (D2_NOERROR);
}

/*---------------------------------------------------------------*/
/*  valid Terminal                                               */
/*                                                               */
/*  (0): check all values that must be NULL / NOT NULL           */
/*  (1): Mandatory group NETW.ADDR.ATTRS: at least one of the    */
/*       fields must be present                                  */
/*  (2): if presentation address is present, then isdn-number    */
/*       must not be present and                                 */
/*       if isdn-number is present then presentation address     */
/*       must not be present                                     */
/*  (3): if isdn-subaddress is present then isdn-number must     */
/*       be present                                              */
/*  (4): if any Domain-type is present then the associated       */
/*       Domain-value must also be present                       */
/*  (5): if any Domain-value is present then the associated      */
/*       Domain-type must also be present                        */
/*---------------------------------------------------------------*/
if   ( (patt1_input & mask1_t_and) == ALL_0          &&  /* (0) */
       (patt2_input & mask2_t_and) == ALL_0          &&  /* (") */
       (patt1_input | mask1_t_or)  == ALL_1          &&  /* (") */
       (patt2_input | mask2_t_or)  == ALL_1          &&  /* (") */
      ((patt1_input | ~BIT_ORA_ISDN_N) == ALL_1 ||       /* (1) */
       (patt1_input | ~BIT_ORA_ISDN_S) == ALL_1 ||       /* (") */
       (patt2_input | ~BIT_ORA_PRES_A) == ALL_1 ||       /* (") */
       (patt2_input | ~BIT_ORA_X121_A) == ALL_1   )  &&  /* (") */
      ((patt2_input | ~BIT_ORA_PRES_A) != ALL_1 ||       /* (2) */
       (patt1_input | ~BIT_ORA_ISDN_N) != ALL_1   )  &&  /* (") */
      ((patt2_input | ~BIT_ORA_ISDN_S) != ALL_1 ||       /* (3) */
       (patt2_input | ~BIT_ORA_ISDN_N) == ALL_1   )  &&  /* (") */
      ((patt1_input | ~BIT_ORA_DOM_T1) != ALL_1 ||       /* (4) */
       (patt1_input | ~BIT_ORA_DOM_V1) == ALL_1    ) &&  /* (") */
      ((patt1_input | ~BIT_ORA_DOM_T2) != ALL_1 ||       /* (") */
       (patt1_input | ~BIT_ORA_DOM_V2) == ALL_1    ) &&  /* (") */
      ((patt1_input | ~BIT_ORA_DOM_T3) != ALL_1 ||       /* (") */
       (patt1_input | ~BIT_ORA_DOM_V3) == ALL_1    ) &&  /* (") */
      ((patt1_input | ~BIT_ORA_DOM_T4) != ALL_1 ||       /* (") */
       (patt1_input | ~BIT_ORA_DOM_V4) == ALL_1    ) &&  /* (") */
      ((patt1_input | ~BIT_ORA_DOM_V1) != ALL_1 ||       /* (5) */
       (patt1_input | ~BIT_ORA_DOM_T1) == ALL_1    ) &&  /* (") */
      ((patt1_input | ~BIT_ORA_DOM_V2) != ALL_1 ||       /* (") */
       (patt1_input | ~BIT_ORA_DOM_T2) == ALL_1    ) &&  /* (") */
      ((patt1_input | ~BIT_ORA_DOM_V3) != ALL_1 ||       /* (") */
       (patt1_input | ~BIT_ORA_DOM_T3) == ALL_1    ) &&  /* (") */
      ((patt1_input | ~BIT_ORA_DOM_V4) != ALL_1 ||       /* (") */
       (patt1_input | ~BIT_ORA_DOM_T4) == ALL_1    )   ) /* (") */
{
  return (D2_NOERROR);
}

return (D2_ERROR);

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_use_is_OR_name                                  */
/*                                                                    */
/* AUTHOR       : G. Rustemeyer, D AP 11                              */
/* DATE         : 22.11.91                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function checks a string of given length. The string must */
/*      consist of one or two structures D21_OR_name, each followed   */
/*      by a name-string or by an OR-address.                         */
/*      the fields D21_OR_name.d21_orn_tag must contain valid         */
/*      OR-name-tags, each tag may appear only once.                  */
/*                                                                    */
/* RETURN VALUE :   D2_NOERROR      syntax-check successful           */
/*                  D2_ERROR:       syntax-check fails                */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_use_is_OR_name(
     byte *val,         /* IN    -  attribute value to check          */
     signed16 len)      /* IN    -  length of attribute value         */

{
/*---------------------------------------------------------------------*/
/*                   D E C L A R A T I O N S                           */
/*---------------------------------------------------------------------*/
byte         *end_of_val = val + len;
D21_OR_name  *OR_name_p;
signed32      name_count=0, addr_count=0;
unsigned16    or_name_len;
D26_dninfo    dn_info;

/*---------------------------------------------------------------------*/
/*                            C O D E                                  */
/*---------------------------------------------------------------------*/
while (val < end_of_val)
   {
      OR_name_p   = (D21_OR_name *) val;
      or_name_len = OR_name_p->d21_orn_len;
      val += sizeof (D21_OR_name);
      switch (OR_name_p->d21_orn_tag)
	 {
	    case D21_ORN_DN:
	       /*------------------------------------------------------*/
	       /* name-string                                          */
	       /*------------------------------------------------------*/
	       if (addr_count > 0)   /* name must occur before(!) addr */
	       {
		  return (D2_ERROR);
	       }
	       name_count ++;
	       if ((signed32)or_name_len <= 1)
	       {
		  return (D2_ERROR);
	       }
	       dn_info.d26_rd_anz = 0;
	       if                /*  Distinguished Name is not correct */
		 (d26_u06_get_name(val, &dn_info))
	       {
		 return(D2_ERROR);
	       }
	       break;
	    case D21_ORN_ORA:
	       /*------------------------------------------------------*/
	       /* OR-address                                           */
	       /*------------------------------------------------------*/
	       addr_count ++;
	       if ((signed32)or_name_len > 0 &&
		   d26_usd_is_OR_addr(val, or_name_len) == D2_ERROR)
	       {
		  return (D2_ERROR);
	       }
	       break;
	    default:
	       return (D2_ERROR);
	 }
	 val += OR_name_p->d21_orn_off;
   }
   if (name_count > 1  ||  addr_count != 1)
   {
      return (D2_ERROR);
   }

return (D2_NOERROR);

}


/*exon ****************************************************************/
/*                                                                    */
/* TYPE:        C-FUNCTION                                            */
/*                                                                    */
/* NAME  :      d26_usf_is_sub_perm                                   */
/*                                                                    */
/* AUTHOR:      Rustemeyer D AP 11                                    */
/* DATE:        06.12.91                                              */
/*                                                                    */
/* DESCRIPTION: The function checks a string of given length. The     */
/*              string must consist of a structure D21_DL_sub_perm,   */
/*              followed by a D21_OR_name structure or by a name-     */
/*              string.                                               */
/*                                                                    */
/* RETURN VALUE :   D2_NOERROR      syntax-check successful           */
/*                  D2_ERROR:       syntax-check fails                */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_usf_is_sub_perm(
     byte *val)         /* IN    -  attribute value to check          */
{
/*--------------------------------------------------------------------*/
/*                  D E C L A R A T I O N S                           */
/*--------------------------------------------------------------------*/
D21_DL_sub_perm   *sub_perm_p = (D21_DL_sub_perm *) val;

unsigned16        subperm_len;

D26_dninfo        dn_info;

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/
val += sizeof (D21_DL_sub_perm);

if ((signed32)(subperm_len = sub_perm_p->d21_dlsp_len) <= 1)
{
  return(D2_ERROR);
}

switch (sub_perm_p->d21_dlsp_tag)
   {
       case D21_DLSP_INDIV:
       case D21_DLSP_MEM_DL:
       case D21_DLSP_PATT:
	  if (d26_use_is_OR_name (val, subperm_len) == D2_ERROR)
	  {
	     return (D2_ERROR);
	  }
	  break;
       case D21_DLSP_MEM_G:
	  dn_info.d26_rd_anz = 0;
	  if                  /*  Distinguished Name is not correct */
	     (d26_u06_get_name(val, &dn_info))
	  {
	    return(D2_ERROR);
	  }
	  break;
       default:
	  return (D2_ERROR);
   }


return (D2_NOERROR);

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_usg_is_seq_t61_prtbl                            */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 16.04.92                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function checks a string of given length. The string must */
/*      consist of c_count strings, separated by '\0' of at most      */
/*      c_maxlen characters each.                                     */
/*                                                                    */
/* RETURN VALUE :   D2_NOERROR      syntax-check successful           */
/*                  D2_ERROR:       syntax-check fails                */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_usg_is_seq_t61_prtbl(
    byte *val,          /* IN    -  attribute value to check          */
    signed16 len,       /* IN    -  length of attribute value         */
    signed16 c_count,   /* IN    -  count of components               */
    signed16 c_maxlen)  /* IN    -  maximum length of one component   */

{
/*--------------------------------------------------------------------*/
/*                  D E C L A R A T I O N S                           */
/*--------------------------------------------------------------------*/

byte       *component;
signed16   len_component;
signed32   rest_len;
char       rep;        /* representation of one component        */

register signed16 i;   /* loop variable                          */

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/

for                         /* all components of the sequence         */
   (i = 0, rest_len = len; (c_count <= 0 || i < c_count) && rest_len > 0;
    i++)
{                           /* set the component pointer              */
  rep = *val;
  component = val + 1;
  rest_len = d26_usy_str_list_part(&val,rest_len);
  len_component = val - component;
  if                        /* component is NULL terminated           */
    (component[len_component - 1] == D2_EOS)
  {                         /* reduce length of component             */
    len_component--;
  }
  if                        /* component too long                     */
    (c_maxlen > 0 && len_component > c_maxlen)
  {                         /* return error                           */
    return(D2_ERROR);
  }
  else
  {                         /* check representation                   */
    switch(rep)
    {
      case D2_T61_REP:
	if                  /* component contains illegal characters  */
	  (d27_601_is_T61String(component,len_component)
	  == D2_ERROR)
	{                   /* return error                           */
	  return(D2_ERROR);
	}
	break;
      case D2_PRTBL_REP:
	if                  /* component contains illegal characters  */
	  (d27_605_is_PrtString(component,len_component) == D2_ERROR)
	{                   /* return error                           */
	  return(D2_ERROR);
	}
	break;
      default:              /* illegal representation                 */
	return(D2_ERROR);
    }
  }
}

if                          /* too much components present            */
  (rest_len > 0)
{                           /* return error                           */
  return(D2_ERROR);
}
else
{                           /* return no error                        */
  return (D2_NOERROR);
}

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_ush_is_certificate                              */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 09.08.93                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function checks a certificate. It must consist of the     */
/*      mandatory components of a certificate, each of which is       */
/*      checked according to the syntax of its own structure.         */
/*                                                                    */
/* RETURN VALUE :   D2_NOERROR      syntax-check successful           */
/*                  D2_ERROR:       syntax-check fails                */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_ush_is_certificate(
    byte **val)         /* INOUT  - attribute value to check          */

{

/*--------------------------------------------------------------------*/
/* T Y P E -  D E F I N I T I O N S   a n d   D E F I N E S           */
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/*                  D E C L A R A T I O N S                           */
/*--------------------------------------------------------------------*/

unsigned16 off;        /* offset of component                    */

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/

/*------------------------------------------------------------*/
/* check version number                                       */
/*------------------------------------------------------------*/

if                          /* version not 1988                       */
  (*(signed32 *)*val != D26_CERT_VER1)
{                           /* return error                           */
  return(D2_ERROR);
}

*val += 2 * sizeof(signed32);
if                          /* signature is an invalid algorithm      */
  (d26_usk_is_algorithm(val) != D2_NOERROR)
{                           /* return error                           */
  return(D2_ERROR);
}

if                          /* issuer is incorrect distinguished name */
  (d26_usl_is_dist_name(val) != D2_NOERROR)
{                           /* return error                           */
  return(D2_ERROR);
}

off   = ((D23_tm_info *)*val)->d23_tmoff;
*val += sizeof(D23_tm_info);
if                          /* format of 'not before' is not correct  */
  (d26_us7_is_UTCtime(*val) != D2_NOERROR)
{                           /* return error                           */
  return(D2_ERROR);
}
*val += off;

off   = ((D23_tm_info *)*val)->d23_tmoff;
*val += sizeof(D23_tm_info);
if                          /* format of 'not after' is not correct   */
  (d26_us7_is_UTCtime(*val) != D2_NOERROR)
{                           /* return error                           */
  return(D2_ERROR);
}
*val += off;

if                          /* subject is incorrect distinguished name*/
  (d26_usl_is_dist_name(val) != D2_NOERROR)
{                           /* return error                           */
  return(D2_ERROR);
}

if                          /* subject public key info is incorrect   */
  (D26_USM_IS_SUBJPUBLICKEYINFO(val) != D2_NOERROR)
{                           /* return error                           */
  return(D2_ERROR);
}

if                        /* signature is incorrect                 */
  (d26_usn_is_signature(val) != D2_NOERROR)
{                         /* return error                           */
  return(D2_ERROR);
}

return (D2_NOERROR);

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_usi_is_cert_pair                                */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 09.08.93                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function checks a certificate pair. It must must consist  */
/*      of at most two certificates, the forward and the reverse      */
/*      certificates. Both of them are checked for correct certificate*/
/*      syntax.                                                       */
/*                                                                    */
/* RETURN VALUE :   D2_NOERROR      syntax-check successful           */
/*                  D2_ERROR:       syntax-check fails                */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_usi_is_cert_pair(
    byte **val)         /* INOUT  - attribute value to check          */

{

/*--------------------------------------------------------------------*/
/* T Y P E -  D E F I N I T I O N S   a n d   D E F I N E S           */
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/*                  D E C L A R A T I O N S                           */
/*--------------------------------------------------------------------*/

signed16 count;             /* counter of certificates                */

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/

for                         /* all certificates in pair               */
   (count = 0; ((D23_certificate *)*val)->d23_cert_tag == D23_FWDTAG ||
    ((D23_certificate *)*val)->d23_cert_tag == D23_REVTAG; count++)
{                           /* check certificate                      */
  *val += sizeof(D23_certificate);
  if                        /* certificate syntax incorrect           */
    (d26_ush_is_certificate(val) != D2_NOERROR)
  {                         /* return error                           */
    return(D2_ERROR);
  }
}

if                          /* no or too much certificates found      */
  (count == 0 || count > 2)
{                           /* return error                           */
  return(D2_ERROR);
}

return (D2_NOERROR);

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_usj_is_cert_list                                */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 09.08.93                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function checks a certificate list. It must must consist  */
/*      of some mandatory components and an optional sequence of      */
/*      revoked certificates. All components are checked for correct  */
/*      syntax according to their structure.                          */
/*                                                                    */
/* RETURN VALUE :   D2_NOERROR      syntax-check successful           */
/*                  D2_ERROR:       syntax-check fails                */
/*                                                                    */
/*exoff ***************************************************************/

signed32 d26_usj_is_cert_list(
    byte **val)         /* INOUT  - attribute value to check          */

{

/*--------------------------------------------------------------------*/
/* T Y P E -  D E F I N I T I O N S   a n d   D E F I N E S           */
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/*                  D E C L A R A T I O N S                           */
/*--------------------------------------------------------------------*/

  signed32 nr_rev_cert;         /* number of revoked certificates     */
  signed16 off;                 /* offset of component                */
  signed16 count;               /* counter of certificates            */

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/

if                          /* signature component is not correct     */
  (d26_usk_is_algorithm(val) != D2_NOERROR)
{                           /* return error                           */
  return(D2_ERROR);
}

if                          /* issuer component is not correct        */
  (d26_usl_is_dist_name(val) != D2_NOERROR)
{                           /* return error                           */
  return(D2_ERROR);
}

off   = ((D23_tm_info *)*val)->d23_tmoff;
*val += sizeof(D23_tm_info);
if                          /* format of 'last update' is not correct */
  (d26_us7_is_UTCtime(*val) != D2_NOERROR)
{                           /* return error                           */
  return(D2_ERROR);
}
*val += off;

nr_rev_cert = *(signed32 *)*val;
*val       += sizeof(signed32);

if                          /* revoked certificates present           */
  (nr_rev_cert != D2_NOT_DEFINED)
{                           /* check them                             */
  for                       /* all revoked certificates               */
     (count = 0; count < nr_rev_cert; count++)
  {                         /* check revoked certificate              */

			    /* skip user certificate                  */
    *val += sizeof(D23_rev_cert);

    if                      /* syntax of signature component incorrect*/
      (d26_usk_is_algorithm(val) != D2_NOERROR)
    {                       /* return error                           */
      return(D2_ERROR);
    }

    if                      /* issuer component is not correct        */
      (d26_usl_is_dist_name(val) != D2_NOERROR)
    {                       /* return error                           */
      return(D2_ERROR);
    }

    off   = ((D23_tm_info *)*val)->d23_tmoff;
    *val += sizeof(D23_tm_info);
    if                      /* format of 'revokation date' is not     */
			    /* correct                                */
      (d26_us7_is_UTCtime(*val) != D2_NOERROR)
    {                       /* return error                           */
      return(D2_ERROR);
    }
    *val += off;
  }
}

if                          /* revoked certificates present           */
  (nr_rev_cert != D2_NOT_DEFINED)
{                           /* check signature for revoked cert.      */
  if                        /* signature not correct                  */
    (d26_usn_is_signature(val) != D2_NOERROR)
  {                         /* return error                           */
    return(D2_ERROR);
  }
}

if                          /* signature for certificate list not ok  */
  (d26_usn_is_signature(val) != D2_NOERROR)
{                           /* return error                           */
  return(D2_ERROR);
}

return (D2_NOERROR);

}

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_usk_is_algorithm                                */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 09.08.93                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function checks an algorithm. It must be one of the three */
/*      defined in X.509, Annex H.                                    */
/*                                                                    */
/* RETURN VALUE :   D2_NOERROR      syntax-check successful           */
/*                  D2_ERROR:       syntax-check fails                */
/*                                                                    */
/*inoff ***************************************************************/

static signed32 d26_usk_is_algorithm(
    byte **val)         /* INOUT  - algorithm value to check          */

{

/*--------------------------------------------------------------------*/
/* T Y P E -  D E F I N I T I O N S   a n d   D E F I N E S           */
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/*                  D E C L A R A T I O N S                           */
/*--------------------------------------------------------------------*/

D2_obj_id  ob_id;          /* structure for algorithm            */
unsigned16 par;            /* indicator for parameter type       */

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/

/*------------------------------------------------------------*/
/* assign object identifier                                   */
/*------------------------------------------------------------*/

  ob_id.d2_typ_len = ((D23_alg_id *)*val)->d23_alglen;
  ob_id.d2_type    = *val + sizeof(D23_alg_id);
  par              = ((D23_alg_id *)*val)->d23_algpar;

  if                        /* object identifier of algorithm is not  */
			    /* defined in X.509                       */
    (D2_ATTCMP(&ob_id,CERT_RSA) && D2_ATTCMP(&ob_id,CERT_SQMOD_N) &&
     D2_ATTCMP(&ob_id,CERT_SQMOD_NWITHRSA))
  {                         /* return error                           */
    return(D2_ERROR);
  }
  else
  {                         /* reset value pointer                    */
    *val += sizeof(D23_alg_id) + ((D23_alg_id *)*val)->d23_algoff;
    switch(par)
    {
      case  D23_NO_PARAMETERS:
	break;
      case  D23_INT_PARAMETERS:
	*val += sizeof(signed32);
	break;
      default:
	return(D2_ERROR);
    }
  }

  return(D2_NOERROR);

}

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_usl_is_dist_name                                */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 09.08.93                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function checks a distinguished name in APDU format       */
/*                                                                    */
/* RETURN VALUE :   D2_NOERROR      syntax-check successful           */
/*                  D2_ERROR:       syntax-check fails                */
/*                                                                    */
/*inoff ***************************************************************/

static signed32 d26_usl_is_dist_name(
    byte **val)         /* INOUT  - distinguished name to check       */

{

/*--------------------------------------------------------------------*/
/* T Y P E -  D E F I N I T I O N S   a n d   D E F I N E S           */
/*--------------------------------------------------------------------*/

/*--------------------------------------------------------------------*/
/*                  D E C L A R A T I O N S                           */
/*--------------------------------------------------------------------*/

Bool       complete;   /* indicates whether DN is complete       */
unsigned16 len;        /* length of component                    */
unsigned16 off;        /* offset of component                    */
D26_dninfo dninfo;     /* information about distinguished names  */

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/

complete  = ((D23_ds_name *)*val)->d23_dscomplete;
len       = ((D23_ds_name *)*val)->d23_dslen;
off       = ((D23_ds_name *)*val)->d23_dsoff;
*val     += sizeof(D23_ds_name);

if                          /* length is wrong                        */
  (len != strlen((String)*val) + 1)
{                           /* return error                           */
  return(D26_ERROR);
}                           /*                                        */

dninfo.d26_rd_anz = 0;      /* initialize dninfo structure            */
if                          /* issuer name has incorrect syntax       */
  (d26_u06_get_name((D2_name_string)*val,&dninfo) != D2_NO_ERR)
{                           /* return error                           */
  return(D2_ERROR);
}

*val += off;
if                          /* issuer was incompletely decoded        */
  (complete != TRUE)
{                           /* return error                           */
  *val += sizeof(D23_av_value) + ((D23_av_value *)*val)->d23_avoff;
}

return(D2_NOERROR);

}

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_usn_is_signature                                */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 09.08.93                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function checks a signature in APDU format.               */
/*                                                                    */
/* RETURN VALUE :   D2_NOERROR      syntax-check successful           */
/*                  D2_ERROR:       syntax-check fails                */
/*                                                                    */
/*inoff ***************************************************************/

static signed32 d26_usn_is_signature(
    byte **val)         /* INOUT  - signature to check                */

{
/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/

if                          /* algorithm is not ok                    */
  (d26_usk_is_algorithm(val) != D2_NOERROR)
{                           /* return error                           */
  return(D2_ERROR);
}

*val += sizeof(D23_bit_info) + ((D23_bit_info *)*val)->d23_boff;

return(D2_NOERROR);

}

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_uso_norm_acl                                    */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 19.05.94                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function norms an ACL value.                              */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

void d26_uso_norm_acl(
    byte *in,           /* IN    -  string to norm                    */
    byte *out)          /* OUT   -  normed string                     */

{

  signed16 len;                     /* length of distinguished names  */
  register signed16 i;              /* loop variable                  */

  for                   /* all DNs in ACL                             */
     (i = 0; i < D2_ACLIDX_MAX; i++)
  {                     /* copy interpretation                        */
    *out++ = *in++;
    len = strlen((char *)in) + 1;
    d27_007_norm_name((char *)in,len,(char *)out);
    d27_018_sort_name((D2_name_string)out);
    in  += len;
    out += len;
  }

}

/*inon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_usp_norm_ci_list                                */
/*                                                                    */
/* AUTHOR       : E. Kraemer, D AP 11                                 */
/* DATE         : 19.05.94                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function norms a Case Ignore List                         */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*      None                                                          */
/*                                                                    */
/*inoff ***************************************************************/

void d26_usp_norm_ci_list(
    byte *in,           /* IN    -  string to norm                    */
    byte *out,          /* OUT   -  normed string                     */
    signed16 len)       /* IN    -  length of string to norm          */

{

  byte     *next;                   /* next component                 */
  signed16 comp_len;                /* length of distinguished names  */
  byte     rep;                     /* representation of value        */

  while                 /* end of value not reached                   */
       (len > 0)
  {                     /* check representation                       */
    rep = *in++;
    *out++ = D2_T61_REP;
    len--;
    if                  /* component following                        */
      ((next = (byte *)memchr(in,D2_EOS,(size_t)len)) != NULL)
    {                   /* set length of component                    */
      comp_len = next - in;
    }
    else
    {                   /* set length of component                    */
      comp_len = len;
    }
    switch(rep)
    {
      case D2_T61_REP:
	d27_651_norm_T61String(in,out,comp_len);
	break;
      case D2_PRTBL_REP:
	d27_031_norm_pr_string(in,out,comp_len);
	break;
    }
    in  += comp_len;
    out += comp_len;
    len -= comp_len;
    if                  /* end of string not reached                  */
      (len > 0)
    {                   /* insert separator                           */
      in++;
      *out++ = D2_EOS;
      len--;
    }
  }

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_usx_is_str_list  (...)                          */
/*                                                                    */
/* AUTHOR       : G. Rustemeyer, D AP 11                              */
/* DATE         : 05.12.91                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function checks a string of given length. The string must */
/*      consist of one or more structures D21_str_list, each followed */
/*      by a string of appropriate type (Printable, T61 or Numeric)   */
/*      The fields D21_str_list.d2_rep must contain valid repr. tags. */
/*      Only certain combinations of tags are allowed. The valid      */
/*      combinations are indicated by the parameter "type".           */
/*                                                                    */
/* RETURN VALUE :   D2_NOERROR      syntax-check successful           */
/*                  D2_ERROR:       syntax-check fails                */
/*                                                                    */
/*exoff ***************************************************************/

static signed32 d26_usx_is_str_list(
    byte *val,          /* IN    -  string list to check              */
    signed16 len,       /* IN    -  length of string list             */
    signed32 type,      /* IN    -  type of stringlist                */
    signed32 *nbstr)    /* OUT   -  number of strings                 */
{
/*---------------------------------------------------------------------*/
/*                   D E C L A R A T I O N S                           */
/*---------------------------------------------------------------------*/
signed16 strl_len;
signed16 str_count=0, prt_count=0, num_count=0, t61_count=0;

D21_str_list  *strlist_p;
byte          *end_of_stringlist = val + len;

static struct strlist_limit limit_tab[3] =
			    { {1,1,0,1},      /* D26_ADMD_STRLIST      */
			      {1,1,0,1},      /* D26_PRT_OR_NUM        */
			      {2,1,1,0} };    /* D26_PRT_ANDOR_T61     */

/*---------------------------------------------------------------------*/
/*                            C O D E                                  */
/*---------------------------------------------------------------------*/

/*----------------------------------------------------------------*/
/* test every string for valid character-set                      */
/*----------------------------------------------------------------*/
while (val < end_of_stringlist)
   {
      strlist_p = (D21_str_list *) val;
      val += sizeof (D21_str_list);

      /*----------------------------------------------------------*/
      /* size 1 (i.e. strlen 0 + '\0') is allowed for admd-n. only*/
      /*----------------------------------------------------------*/
      strl_len = strlist_p -> d21_strl_len;
      if ( type != D26_ADMD_STRLIST      &&   strl_len <= 1)
      {
	 return (D2_ERROR);
      }
      /*----------------------------------------------------------*/
      /* test correct value of length-field (must include '\0')   */
      /*----------------------------------------------------------*/
      if ( strl_len != (signed16)(strlen ((char *)val) + 1) )
      {
	 return (D2_ERROR);
      }
      /*----------------------------------------------------------*/
      /* check string (except '\0') for character set and         */
      /* assign variables for number of strings                   */
      /*----------------------------------------------------------*/
      str_count ++;
      switch (strlist_p -> d21_strl_rep)
	 {
	    case  D2_PRINTABLE:
	      if (d27_605_is_PrtString (val,strl_len-1) == D2_ERROR)
	      {
		 return (D2_ERROR);
	      }
	      prt_count ++;
	      break;
	    case D2_T61:
	      if (type == D26_PRT_ANDOR_T61  &&    /* t61-string only  */
		  prt_count == 0                )  /* allowed after    */
	      {
		 return (D2_ERROR);                /* printable string */
	      }
	      if (d27_601_is_T61String(val,strl_len-1) == D2_ERROR)
	      {
		 return (D2_ERROR);
	      }
	      t61_count ++;
	      break;
	    case D2_NUMERIC:
	      if (d26_us6_is_NumString  (val,strl_len-1) == D2_ERROR)
	      {
		 return (D2_ERROR);
	      }
	      num_count ++;
	      break;
	    default:
	      return (D2_ERROR);
	 }

      val += strlist_p -> d21_strl_off;
   }

/*----------------------------------------------------------------*/
/* test number of strings                                         */
/*----------------------------------------------------------------*/
if (str_count > limit_tab[type].max_nb_str     ||
    prt_count > limit_tab[type].max_nb_prt     ||
    num_count > limit_tab[type].max_nb_num     ||
    t61_count > limit_tab[type].max_nb_t61        )
{
  return (D2_ERROR);
}

*nbstr = str_count;

return (D2_NOERROR);

}

/*exon ****************************************************************/
/*                                                                    */
/* TYPE         : C-FUNCTION                                          */
/*                                                                    */
/* NAME         : d26_usy_str_list_part                               */
/*                                                                    */
/* AUTHOR       : Rustemeyer, D AP 11                                 */
/* DATE         : 26.04.91                                            */
/*                                                                    */
/* DESCRIPTION  :                                                     */
/*      The function sets the value pointer to the next component     */
/*      of a list of T61- or printable strings, separated by D2_EOS.  */
/*                                                                    */
/* RETURN VALUE :                                                     */
/*                  D2_ERROR:       no next part found                */
/*                  > 0             length of skipped string          */
/*                                                                    */
/*exoff ***************************************************************/

static signed16 d26_usy_str_list_part(
    byte **val,         /* INOUT -  string list part to check         */
    signed16 len)       /* IN    -  length of string list             */

{
/*--------------------------------------------------------------------*/
/*                  D E C L A R A T I O N S                           */
/*--------------------------------------------------------------------*/

byte *skipped;              /* part of value to be skipped            */

/*--------------------------------------------------------------------*/
/*                            C O D E                                 */
/*--------------------------------------------------------------------*/

skipped = *val;
if                          /* separator missing                      */
  ((*val = (byte *)memchr((void *)*val,D2_EOS,(size_t)len)) == NULL)
{                           /* reset value pointer to the end         */
  *val = skipped + len;
}
else
{                           /* reset value to next component          */
  (*val)++;
}

return(len - (*val - skipped));

}
