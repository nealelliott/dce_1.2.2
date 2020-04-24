/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1990, 1991, 1992, 1993, 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE in the
 * src directory for the full copyright text.
 */
/*
 * HISTORY
 * $Log: d21time.c,v $
 * Revision 1.1.10.2  1996/02/18  19:42:08  marty
 * 	Update OSF copyright years
 * 	[1996/02/18  18:22:52  marty]
 *
 * Revision 1.1.10.1  1995/12/08  15:20:29  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/08  14:52:45  root]
 * 
 * Revision 1.1.8.2  1994/03/23  15:06:58  keutel
 * 	March 1994 code drop
 * 	[1994/03/21  16:13:41  keutel]
 * 
 * Revision 1.1.8.1  1994/02/22  17:35:43  marrek
 * 	Code drop for dce1.1.
 * 	[1994/02/08  09:37:28  marrek]
 * 
 * Revision 1.1.6.2  1993/08/10  10:55:22  marrek
 * 	July 1993 code drop.
 * 	[1993/07/30  08:40:17  marrek]
 * 
 * Revision 1.1.4.3  1992/12/31  18:10:46  bbelch
 * 	Embedding copyright notice
 * 	[1992/12/31  13:07:18  bbelch]
 * 
 * Revision 1.1.4.2  1992/11/27  15:01:21  marrek
 * 	November 1992 code drop
 * 	[1992/11/16  16:13:58  marrek]
 * 
 * Revision 1.1.2.2  1992/06/01  20:01:04  melman
 * 	New GDS merged drop
 * 	[1992/05/28  20:45:18  melman]
 * 
 * $EndLog$
 */

#if !defined(lint) && defined(GDS_RCS_ID)
static char rcsid[] = "@(#)$RCSfile: d21time.c,v $ $Revision: 1.1.10.2 $ $Date: 1996/02/18 19:42:08 $";
#endif

/****************************************************************************
*                                                                           *
*         COPYRIGHT  (C)  SIEMENS NIXDORF INFORMATIONSSYSTEME AG 1991       *
*                                ALL RIGHTS RESERVED                        *
*                                                                           *
****************************************************************************/

/*daton ***************************************************************/
/*                                                                    */
/* NAME         : d21time.o                                           */
/*								      */
/* AUTHOR       : Schmid ,D AP 11                                     */
/* DATE         : 16.01.89                                            */
/*								      */
/*datoff **************************************************************/

#include <gds.h>
#include <stdio.h>

#include <dce/d2dir.h>
#include <dce/d27util.h>
#include <gdsext.h>
#include <d2delupd.h>
#include <d20proto.h>

/**********************************************************************/
/*								      */
/*			  D E F	I N E S				      */
/*								      */
/**********************************************************************/

				/* positions of update times in       */
				/* integer record                     */
#define D21_PMIN    0
#define D21_PHOUR   (D21_PMIN + D21_NMIN)
#define D21_PWDAY   (D21_PHOUR + D21_NHOUR)

#define D21TMIN            0    /* minimal value for hours (used if   */
				/* update frequency is UPD_LOW_1 or   */
				/* UPD_LOW_2)                         */
#define D21TMAX           23    /* maximal value for hours (used if   */
				/* update frequency is UPD_LOW_1 or   */
				/* UPD_LOW_2)                         */

#define D21WDMIN           0    /* minimal day count (in week)        */
#define D21WDMAX           6    /* maximal day count (in week)        */

#define D21_INCR_MIN       5    /* incremental value for minutes      */

/**********************************************************************/
/*								      */
/*		   D E C L A R A T I O N S       DATA		      */
/*								      */
/**********************************************************************/

				/* indicates the occurrency of update */
				/* times in the shadowing job file    */
extern signed32 d21time_upd[];

#ifndef CACHE_ADMIN
				/* indicates the occurrency of update */
				/* times in the shadowing job file    */
				/* before the update of the shadowing */
				/* job file                           */
static signed32 d21old_t[D21_T_OCC];
static signed32 d21_min_diff[] = { 1, 2, 3, 6 };
				/* indicates which elments in         */
				/* "d21time_upd" have to be set for   */
				/* minutes                            */
static signed32 d21_hour_diff[] = { 1, 2, 4, 6, 12 };
				/* indicates which elments in         */
				/* "d21time_upd" have to be set for   */
				/* hours                              */
#endif

static signed32 d21_min[] = { 5, 10, 15, 30 };
static signed32 d21_hours[] = { 1, 2, 4, 6, 12 };


/* LOCAL FUNCTIONS */

#ifndef CACHE_ADMIN
static void d21d014_add ( signed32 , signed32 , signed32 );
static void d21d015_del ( signed32 , signed32 , signed32 );
static void d21d018_compare_time ( signed32 , signed32 , char ** , Bool * );
#endif

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21d001_check_time()                                   */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will check whether the given update time is     */
/*      correct.                                                      */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      upd_time    update times                                      */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                    None                                            */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR    = update time correctly defined                 */
/*      D2_ERROR       = illegal update time                          */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21d001_check_time(
  Upd_time * upd_time)
{
signed32 i, i_max;

switch ((int) upd_time->upd_t_mode)
{   case UPD_HIGH:
	    for (i = 0, i_max = sizeof(d21_min)/sizeof(signed32); i < i_max; i++)
	    {   if (upd_time->upd_min == d21_min[i])
		{   break;
		}
	    }
	    if (i == i_max)
	    {   return(D2_ERROR);
	    }
	    upd_time->upd_hour = upd_time->upd_1d_of_week =
	    upd_time->upd_1hour = upd_time->upd_2d_of_week =
	    upd_time->upd_2hour = 0;
	    break;

    case UPD_MEDIUM:
	    for (i = 0, i_max = sizeof(d21_hours)/sizeof(signed32); i<i_max; i++)
	    {   if (upd_time->upd_hour == d21_hours[i])
		{   break;
		}
	    }
	    if (i == i_max)
	    {   return(D2_ERROR);
	    }
	    upd_time->upd_min = upd_time->upd_1d_of_week =
	    upd_time->upd_1hour = upd_time->upd_2d_of_week =
	    upd_time->upd_2hour = 0;
	    break;

    case UPD_LOW_1:
	    if ((upd_time->upd_1d_of_week < D21WDMIN ||
		 upd_time->upd_1d_of_week > D21WDMAX)
		 && upd_time->upd_1d_of_week != ALL_POSSIBLE_TIMES)
	    {   return(D2_ERROR);
	    }
	    if (upd_time->upd_1hour < D21TMIN ||
		upd_time->upd_1hour > D21TMAX)
	    {   return(D2_ERROR);
	    }
	    upd_time->upd_min = upd_time->upd_hour =
	    upd_time->upd_2d_of_week = upd_time->upd_2hour = 0;
	    break;

    case UPD_LOW_2:
	    if (((upd_time->upd_1d_of_week < D21WDMIN ||
		  upd_time->upd_1d_of_week > D21WDMAX)
		  && upd_time->upd_1d_of_week != ALL_POSSIBLE_TIMES) ||
		upd_time->upd_2d_of_week < D21WDMIN ||
		upd_time->upd_2d_of_week > D21WDMAX ||
		(upd_time->upd_1d_of_week == upd_time->upd_2d_of_week &&
		 upd_time->upd_1hour == upd_time->upd_2hour))
	    {   return(D2_ERROR);
	    }
	    if (upd_time->upd_1hour < D21TMIN ||
		upd_time->upd_1hour > D21TMAX ||
		upd_time->upd_2hour < D21TMIN ||
		upd_time->upd_2hour > D21TMAX)
	    {   return(D2_ERROR);
	    }
	    upd_time->upd_min = upd_time->upd_hour = 0;
	    break;
}

return(D2_NOERROR);
}

#ifndef CACHE_ADMIN

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21d012_add_upd_time()                                 */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will update the integer array containing        */
/*      the occurreny for the distinguished update time values.       */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      upd_time    = update times                                    */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR     = The function has been performed successfully.*/
/*      D2_ERROR       = function fails                               */
/*                                                                    */
/*exoff ***************************************************************/

void d21d012_add_upd_time(
  Upd_time * upd_time)
{
signed32 pos;
signed32 max_pos;
signed32 diff;   /* indicating the incremental value used for filling the  */
	         /* "d21time_upd" structure                                */

switch((int) upd_time->upd_t_mode)
{   case UPD_HIGH:
	    for (pos = 0; pos < sizeof(d21_min) / sizeof(signed32); pos++)
	    {   if (d21_min[pos] == upd_time->upd_min)
		{   break;
		}
	    }
	    diff = d21_min_diff[pos];
	    for (pos = D21_PMIN, max_pos = pos + D21_NMIN; pos < max_pos;
		 pos += diff)
	    {   d21time_upd[pos]++;
	    }
	    d21d014_add(ALL_POSSIBLE_TIMES, D21_PHOUR, D21_NHOUR);
	    d21d014_add(ALL_POSSIBLE_TIMES, D21_PWDAY, D21_NWDAY);
	    break;

    case UPD_MEDIUM:
	    for (pos = 0; pos < sizeof(d21_hours) / sizeof(signed32); pos++)
	    {   if (d21_hours[pos] == upd_time->upd_hour)
		{   break;
		}
	    }
	    diff = d21_hour_diff[pos];
	    for (pos = D21_PHOUR, max_pos = pos + D21_NHOUR; pos < max_pos;
		 pos += diff)
	    {   d21time_upd[pos]++;
	    }
	    d21time_upd[D21_PMIN]++;
	    d21d014_add(ALL_POSSIBLE_TIMES, D21_PWDAY, D21_NWDAY);
	    break;

    case UPD_LOW_2:
	    d21time_upd[D21_PMIN]++;
	    d21time_upd[D21_PWDAY + upd_time->upd_2d_of_week]++;
	    d21time_upd[D21_PHOUR + upd_time->upd_2hour]++;
	    d21d014_add(upd_time->upd_1d_of_week, D21_PWDAY, D21_NWDAY);
	    d21time_upd[D21_PHOUR + upd_time->upd_1hour]++;
	    break;

    case UPD_LOW_1:
	    d21time_upd[D21_PMIN]++;
	    d21d014_add(upd_time->upd_1d_of_week, D21_PWDAY, D21_NWDAY);
	    d21time_upd[D21_PHOUR + upd_time->upd_1hour]++;
	    break;
}
} /* end of d21d012_add_upd_time */

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21d013_del_upd_time()                                 */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will update the integer array containing        */
/*      the occurreny for the distinguished update time values.       */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      upd_time    = update times                                    */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR     = The function has been performed successfully.*/
/*      D2_ERROR       = function fails                               */
/*                                                                    */
/*exoff ***************************************************************/

void d21d013_del_upd_time(
  Upd_time * upd_time)
{
signed32 pos;
signed32 max_pos;
signed32 diff;   /* indicating the incremental value used for filling the  */
	         /* "d21time_upd" structure                                */

switch((int) upd_time->upd_t_mode)
{   case UPD_HIGH:
	    for (pos = 0; pos < sizeof(d21_min) / sizeof(signed32); pos++)
	    {   if (d21_min[pos] == upd_time->upd_min)
		{   break;
		}
	    }
	    diff = d21_min_diff[pos];
	    for (pos = D21_PMIN, max_pos = pos + D21_NMIN; pos < max_pos;
		 pos += diff)
	    {   d21time_upd[pos]--;
	    }
	    d21d015_del(ALL_POSSIBLE_TIMES, D21_PHOUR, D21_NHOUR);
	    d21d015_del(ALL_POSSIBLE_TIMES, D21_PWDAY, D21_NWDAY);
	    break;

    case UPD_MEDIUM:
	    for (pos = 0; pos < sizeof(d21_hours) / sizeof(signed32); pos++)
	    {   if (d21_hours[pos] == upd_time->upd_hour)
		{   break;
		}
	    }
	    diff = d21_hour_diff[pos];
	    for (pos = D21_PHOUR, max_pos = pos + D21_NHOUR; pos < max_pos;
		 pos += diff)
	    {   d21time_upd[pos]--;
	    }
	    d21time_upd[D21_PMIN]--;
	    d21d015_del(ALL_POSSIBLE_TIMES, D21_PWDAY, D21_NWDAY);
	    break;

    case UPD_LOW_2:
	    d21time_upd[D21_PMIN]--;
	    d21time_upd[D21_PWDAY + upd_time->upd_2d_of_week]--;
	    d21time_upd[D21_PHOUR + upd_time->upd_2hour]--;
	    d21d015_del(upd_time->upd_1d_of_week, D21_PWDAY, D21_NWDAY);
	    d21time_upd[D21_PHOUR + upd_time->upd_1hour]--;
	    break;

    case UPD_LOW_1:
	    d21time_upd[D21_PMIN]--;
	    d21d015_del(upd_time->upd_1d_of_week, D21_PWDAY, D21_NWDAY);
	    d21time_upd[D21_PHOUR + upd_time->upd_1hour]--;
	    break;
}
} /* end of d21d013_del_upd_time */

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21d014_add()                                          */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will update the integer array containing        */
/*      the occurreny for the distinguished update time values.       */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      dist_val    = distinguished value of update time              */
/*      pos         = beginning position in integer array             */
/*      no          = number of relvant values                        */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR     = The function has been performed successfully.*/
/*      D2_ERROR       = function fails                               */
/*                                                                    */
/*exoff ***************************************************************/

static void d21d014_add(
  signed32 dist_val,
  signed32 pos,
  signed32 no)
{
signed32 i, i_max;

if (dist_val == ALL_POSSIBLE_TIMES)
  { for (i = pos, i_max = i + no; i < i_max; i++)
    {   d21time_upd[i]++;
    }
  }
else
  {
    d21time_upd[pos + dist_val]++;
  }
} /* end of d21d014_add */


/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21d015_del()                                          */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will update the integer array containing        */
/*      the occurreny for the distinguished update time values.       */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      dist_val    = distinguished value of update time              */
/*      pos         = beginning position in integer array             */
/*      no          = number of relvant values                        */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                                                                    */
/*exoff ***************************************************************/

static void d21d015_del(
  signed32 dist_val,
  signed32 pos,
  signed32 no)
{
signed32 i, i_max;

if (dist_val == ALL_POSSIBLE_TIMES)
  { for (i = pos, i_max = i + no; i < i_max; i++)
    {   d21time_upd[i]--;
    }
  }
else
  {
    d21time_upd[pos + dist_val]--;
  }
} /* end of d21d015_del */

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21d017_store_old_times()                              */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will save the occurrencies of the update times  */
/*      before updating the shadowing job file.                       */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                                                                    */
/*exoff ***************************************************************/

void d21d017_store_old_times(void)
{
signed32 i;

for (i = 0; i < D21_T_OCC; i++)
  {   d21old_t[i] = d21time_upd[i];
  }
}


/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:  d21d016_gen_cron_entry()                               */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will create a new entry in "crontab" if         */
/*      necessary. It compares the old update times with the new ones.*/
/*      If more than D21_NDIST_VAL values have to be entered a "*"    */
/*      will be set as update time stamp in "crontab".                */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      path        = path name of directory installation             */
/*      dir_id      = directory ID                                    */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR     = The function has been performed successfully.*/
/*      D2_ERROR       = function fails                               */
/*                                                                    */
/*exoff ***************************************************************/

d2_ret_val d21d016_gen_cron_entry(
  char * path,
  signed32 dir_id)
{
signed32   i, i_max;
char  cron_entry[5 * D21_NDIST_VAL * 16 + 5 * D27_LFILE_NAME + 40];
char *p_str;
Bool  change = FALSE;

sprintf(cron_entry, ". %s%s/.profile;%s%s%s %s%s%s %s%s%s %d %s ",
	dcelocal_path, D27_CLIENT_DIR,
	dcelocal_path, D27_BIN_DIR, D20_EXEC, dcelocal_path, D27_BIN_DIR,
	D20_CUP_SCR, dcelocal_path, D27_BIN_DIR, D20_CUP_PROC, dir_id,
	dcelocal_path);
p_str = cron_entry + strlen(cron_entry);

				/* check whether all jobs are inactive */
for (i = 0, i_max = D21_T_OCC - 1; i < i_max; i++)
{   if (d21time_upd[i] != 0)
    {   break;
    }
}
if (i == i_max)
{                               /* alls jobs are inactive;            */
				/* ---> don't generate update times   */
    change = TRUE;
}
else
{   d21d018_compare_time(D21_PMIN, D21_NMIN, &p_str, &change);
    d21d018_compare_time(D21_PHOUR, D21_NHOUR, &p_str, &change);
    strcpy(p_str, " \\\'\''*'\'\\\' \\\'\''*'\'\\\' ");
    p_str += strlen(p_str);
    d21d018_compare_time(D21_PWDAY, D21_NWDAY, &p_str, &change);
}

if (change == TRUE)
  {                               /* cron entry is to be updated        */
    strcat(cron_entry, " 2>/dev/null");
    if (system(cron_entry) != 0)
    {   return(D2_ERROR);
    }
  }

return(D2_NOERROR);
} /* of d21d016_gen_cron_entry */

/*exon ****************************************************************/
/*                                                                    */
/*  FUNCTION:   d21d018_compare_time()                                */
/*                                                                    */
/*  SHORT DESCRIPTION:                                                */
/*      This function will compares the old update times the new ones */
/*      and creates a string for "crontab".                           */
/*      If more than D21_NDIST_VAL values have to be entered a "*"    */
/*      will be set as update time stamp in "crontab".                */
/*                                                                    */
/*  INPUT-PARAMETER:                                                  */
/*      pos         = position in global array containing update time */
/*                    occurrencies                                    */
/*      no          = number of values to be checked                  */
/*      p_cron      = pointer to "crontab" string                     */
/*                                                                    */
/*  OUTPUT-PARAMETER:                                                 */
/*      p_cron      = new pointer to "crontab" string                 */
/*      change      = a flag inidicating whether the crontab rentry   */
/*                    has to be changed                               */
/*                                                                    */
/*  RETURN-VALUE:                                                     */
/*      D2_NOERROR     = The function has been performed successfully.*/
/*      D2_ERROR       = function fails                               */
/*                                                                    */
/*exoff ***************************************************************/

static void d21d018_compare_time(
  signed32 pos,
  signed32 no,
  char ** p_cron,
  Bool * change)
{
signed32    i;
signed32    beg_pos = pos;      /* beginning position                 */
char        new_entry[D21_NDIST_VAL * 16 + 1];
char       *str = new_entry;
signed32    no_changes = 0;
signed32    no_dist_val = 0;    /* number of distinguished values in  */
				/* new "crontab" string               */

new_entry[0] = D2_EOS;
for (i = 0; i < no; i++, pos++)
{   if (d21time_upd[pos] > 0)
    {   if (no_dist_val < D21_NDIST_VAL)
	{   switch ((int) beg_pos)
	    {   case D21_PMIN:
			    sprintf(str, "%d,", (pos - beg_pos) *
				    D21_INCR_MIN);
			    break;

		default:    sprintf(str, "%d,", pos - beg_pos);
			    break;
	    }
	    str += strlen(str);
	}
	no_dist_val++;
    }
    if (d21time_upd[pos] != d21old_t[pos] &&
	((d21time_upd[pos] != 0 && d21old_t[pos] == 0) ||
	  (d21time_upd[pos] == 0 && d21old_t[pos] != 0)))
    {                           /* occurrency changed                 */
	no_changes++;
    }
}

if (no_changes != 0)
{                               /* "crontab" string has to be updated */
    *change = TRUE;
}

				/* generate "crontab" string          */
*(*p_cron) = ' ';
(*p_cron)++;
if (no_dist_val <= D21_NDIST_VAL && no_dist_val != 0)
{                               /* new string contains a sequence of  */
				/* distinguished values               */
    *(str-1) = D2_EOS;
    strcpy(*p_cron, new_entry);
}
else
  { if (no_dist_val > D21_NDIST_VAL)
    {                           /* new string = "*"                   */
	strcpy(*p_cron, "\\\'\''*'\'\\\' ");
    }
    else
    {                           /* number of values is 0;             */
				/* in case of minutes: generate "0",  */
				/* otherwise "*"                      */
	if (beg_pos == D21_PMIN)
	{   strcpy(*p_cron, "0 ");
	}
	else
	{   strcpy(*p_cron, "\\\'\''*'\'\\\' ");
	}
    }
  }
*p_cron += strlen(*p_cron);
} /* end of d21d018_compare_time */

#endif /* of not defined CACHE_ADMIN */
