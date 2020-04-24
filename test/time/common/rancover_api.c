/*
 * @OSF_COPYRIGHT@
 * COPYRIGHT NOTICE
 * Copyright (c) 1996 Open Software Foundation, Inc.
 * ALL RIGHTS RESERVED (DCE).  See the file named COPYRIGHT.DCE for
 * the full copyright text.
 * 
 */
/*
 * HISTORY
 * $Log: rancover_api.c,v $
 * Revision 1.1.2.2  1996/03/09  20:52:48  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:30:18  marty]
 *
 * Revision 1.1.2.1  1995/12/11  23:05:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:37:07  root]
 * 
 * $EndLog$
 */
/*
 * @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: rancover_api.c,v $
 * Revision 1.1.2.2  1996/03/09  20:52:48  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:30:18  marty]
 *
 * Revision 1.1.3.2  1996/03/09  20:30:18  marty
 * 	Add OSF copyright
 *
 * Revision 1.1.2.1  1995/12/11  23:05:32  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:37:07  root]
 *
 * Revision 1.1.1.2  1995/12/11  22:37:07  root
 * 	Submit OSF/DCE 1.2.1
 *
 *	HP revision /main/HPDCE02/4  1995/06/15  20:04 UTC  pare
 * 	Merge changes
 * 
 *	HP revision /main/HPDCE02/pare_mothra/3  1995/06/15  20:00 UTC  pare
 * 	Fix utc_getTCPtime test
 * 
 *	HP revision /main/HPDCE02/3  1995/06/15  13:17 UTC  pare
 * 	Merge changes
 * 
 *	HP revision /main/HPDCE02/pare_mothra/2  1995/06/15  13:14 UTC  pare
 * 	Change test loop from 250 to 50
 * 
 *	HP revision /main/HPDCE02/2  1994/11/15  22:03 UTC  pare
 * 	Add utc_getTCPtime() test.
 * 
 *	HP revision /main/HPDCE02/pare_mothra/1  1994/11/15  21:58 UTC  pare
 * 	Add utc_getTCPtime() test.
 * 
 *	HP revision /main/HPDCE02/1  1994/07/25  17:37 UTC  pare
 * 	createst initial version
 * 
 * Revision 1.1.2.3  1993/10/21  19:34:18  pare
 * 	Added PASS/FAIL message for tet and printf for user
 * 	[1993/10/21  19:33:42  pare]
 * 
 * Revision 1.1.2.2  1993/07/14  17:30:16  truitt
 * 	New test for King Kong.
 * 	[1993/07/14  17:26:42  truitt]
 * 
 * $EndLog$
 */

#include <dce/utc.h>
#include <stdlib.h>
#include <math.h>
#include <tet_api.h>

void (*tet_startup)() = NULL;
void (*tet_cleanup)() = NULL;
void rancover();

struct tet_testlist tet_testlist[] =
   {{rancover, 1},
    {NULL, NULL}};


/*
 * This routine returns a 32-bit random number
 */
unsigned long int random32()
{
   return(((rand() & 0x7fff) << 17) +
          ((rand() & 0x7fff) << 2) + (rand() & 0x3) );
}


void rancover()
{
   timespec_t       ts, ts2, inac, inac2;
   reltimespec_t    rts, rts2;
   struct tm        tim, inac_tim;
   utc_t            jimutc, utc, utc2, utcm, utc_tcp;
   utc_t            utcrel1, utcrel2, utcbin1, utcbin2;
   long             tdf, tdf2, tns, ins;
   enum utc_cmptype ctype;
   char             tzname[100], stuff[100], tmstr[100];
   int              i, isdst, ok;

   char timebuffer[ UTC_MAX_STR_LEN ];
   char * hostname = "col.hp.com";

   ok = 1;

   for (i = 0; i < 50; i++)
   {
      sprintf(stuff,"Loop number: %d\n",i);
      tet_infoline(stuff);

      /* 1. utc_gettime */

      if (utc_gettime(&jimutc) >= 0)
         tet_infoline("utc_gettime: ok\n");
      else {
         tet_infoline("utc_gettime: fail\n");
	 ok = 0;
      }

      /* 2. utc_getusertime */

      setenv("DTS_TZ","",1);
      if (utc_getusertime(&jimutc) >= 0)
         tet_infoline("utc_getusertime (DTS_TZ not set): ok\n");
      else {
         tet_infoline("utc_getusertime (DTS_TZ not set): fail\n");
	 ok = 0;
      }

      setenv("DTS_TZ","BITEME",1);
      if (utc_getusertime(&jimutc) >= 0)
         tet_infoline("utc_getusertime (DTS_TZ set BITEME): ok\n");
      else {
         tet_infoline("utc_getusertime (DTS_TZ set BITEME): fail\n");
	 ok = 0;
      }

      setenv("DTS_TZ","EST5EDT",1);
      if (utc_getusertime(&jimutc) >= 0)
         tet_infoline("utc_getusertime (DTS_TZ set EST5EDT): ok\n");
      else {
         tet_infoline("utc_getusertime (DTS_TZ set EST5EDT): fail\n");
	 ok = 0;
      }

      /* 3. utc_mkbintime */

      ts.tv_sec = random32() / 2;
      ts.tv_nsec = 100 * (random32() / 500);
      inac.tv_sec = random32() / 2;
      inac.tv_nsec = 100 * (random32() / 500);
      tdf = (random32() / (65536*60)) * 60;

      if (utc_mkbintime(&utc, &ts, &inac, tdf) >= 0)
         tet_infoline("utc_mkbintime (random): ok\n");
      else {
         tet_infoline("utc_mkbintime (random): fail\n");
	 ok = 0;
      }

      if (utc_mkbintime(&utc, &ts, NULL, tdf) >= 0)
         tet_infoline("utc_mkbintime (NULL inac): ok\n");
      else
         tet_infoline("utc_mkbintime (NULL inac): fail\n");

      if (utc_mkbintime(&utc, &ts, &inac, NULL) >= 0)
         tet_infoline("utc_mkbintime (NULL tdf): ok\n");
      else
         tet_infoline("utc_mkbintime (NULL tdf): fail\n");

      /* 4. utc_bintime */

      if (utc_bintime(&ts2, &inac2, &tdf2, &utc) >= 0)
         tet_infoline("utc_bintime (random utc): ok\n");
      else
         tet_infoline("utc_bintime (random utc): fail\n");

      if (utc_bintime(&ts2, &inac2, &tdf2, NULL) >= 0)
         tet_infoline("utc_bintime (NULL utc): ok\n");
      else
         tet_infoline("utc_bintime (NULL utc): fail\n");

      /* 5. utc_mkbinreltime */

      rts.tv_sec = random32() / 2;
      rts.tv_nsec = 100 * (random32() / 500);
      inac.tv_sec = random32() / 2;
      inac.tv_nsec = 100 * (random32() / 500);

      if (utc_mkbinreltime(&utc, &rts, &inac) >= 0)
         tet_infoline("utc_mkbinreltime (random): ok\n");
      else
         tet_infoline("utc_mkbinreltime (random): fail\n");

      if (utc_mkbinreltime(&utc, &rts, NULL) >= 0)
         tet_infoline("utc_mkbinreltime (NULL inac): ok\n");
      else
         tet_infoline("utc_mkbinreltime (NULL inac): fail\n");

      /* 6. utc_binreltime */

      if (utc_binreltime(&rts2, &inac2, &utc) >= 0)
         tet_infoline("utc_binreltime (random utc): ok\n");
      else
         tet_infoline("utc_binreltime (random utc): fail\n");

      if (utc_binreltime(&rts2, &inac2, NULL) >= 0)
         tet_infoline("utc_binreltime (NULL utc): ok\n");
      else
         tet_infoline("utc_binreltime (NULL utc): fail\n");

      /* 7. utc_localtime */

      setenv("DTS_TZ","",1);
      if (utc_localtime(&tim, &tns, &inac_tim, &ins, NULL) >= 0)
         tet_infoline("utc_localtime (NULL utc, NULL DTS_TZ): ok\n");
      else
         tet_infoline("utc_localtime (NULL utc, NULL DTS_STZ): fail\n");

      setenv("DTS_TZ","BITEME",1);
      if (utc_localtime(&tim, &tns, &inac_tim, &ins, &utc) >= 0)
         tet_infoline("utc_localtime (random utc, bad DTS_TZ): ok\n");
      else
         tet_infoline("utc_localtime (random utc, bad DTS_TZ): fail\n");

      setenv("DTS_TZ","EST5EDT",1);
      if (utc_localtime(&tim, &tns, &inac_tim, &ins, NULL) >= 0)
         tet_infoline("utc_localtime (NULL utc, good DTS_TZ): ok\n");
      else
         tet_infoline("utc_localtime (NULL utc, good DTS_TZ): fail\n");

      if (utc_localtime(&tim, &tns, &inac_tim, &ins, &utc) >= 0)
         tet_infoline("utc_localtime (random utc, good DTS_TZ): ok\n");
      else
         tet_infoline("utc_localtime (random utc, good DTS_TZ): fail\n");

      /* 8. utc_mklocaltime */

      setenv("DTS_TZ","",1);
      if (utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins) >= 0)
         tet_infoline("utc_mklocaltime (random tim, random inac_tim, good tns, NULL TZ): ok\n");
      else
         tet_infoline("utc_mklocaltime (random tim, random inac_tim, good tns, NULL TZ): fail\n");

      if (utc_mklocaltime(&utc2, &tim, tns, NULL, ins) >= 0)
         tet_infoline("utc_mklocaltime (random tim, NULL inac_tim, good tns, NULL TZ): ok\n");
      else
         tet_infoline("utc_mklocaltime (random tim, NULL inac_tim, good tns, NULL TZ): fail\n");

      if (utc_mklocaltime(&utc2, &tim, NULL, &inac_tim, NULL) >= 0)
         tet_infoline("utc_mklocaltime (random tim, random inac_tim, NULL tns, NULL TZ): ok\n");
      else
         tet_infoline("utc_mklocaltime (random tim, random inac_tim, NULL tns, NULL TZ): fail\n");

      if (utc_mklocaltime(&utc2, NULL, tns, &inac_tim, ins) >= 0)
         tet_infoline("utc_mklocaltime (NULL tim, random inac_tim, good tns, NULL TZ): ok\n");
      else
         tet_infoline("utc_mklocaltime (NULL tim, random inac_tim, good tns, NULL TZ): fail\n");

      setenv("DTS_TZ","EST5EDT",1);
      if (utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins) >= 0)
         tet_infoline("utc_mklocaltime (random tim, random inac_tim, good tns, good TZ): ok\n");
      else
         tet_infoline("utc_mklocaltime (random tim, random inac_tim, good tns, good TZ): fail\n");

      if (utc_mklocaltime(&utc2, &tim, tns, NULL, ins) >= 0)
         tet_infoline("utc_mklocaltime (random tim, NULL inac_tim, good tns, good TZ): ok\n");
      else
         tet_infoline("utc_mklocaltime (random tim, NULL inac_tim, good tns, good TZ): fail\n");

      if (utc_mklocaltime(&utc2, &tim, NULL, &inac_tim, NULL) >= 0)
         tet_infoline("utc_mklocaltime (random tim, random inac_tim, NULL tns, good TZ): ok\n");
      else
         tet_infoline("utc_mklocaltime (random tim, random inac_tim, NULL tns, good TZ): fail\n");

      if (utc_mklocaltime(&utc2, NULL, tns, &inac_tim, ins) >= 0)
         tet_infoline("utc_mklocaltime (NULL tim, random inac_tim, good tns, good TZ): ok\n");
      else
         tet_infoline("utc_mklocaltime (NULL tim, random inac_tim, good tns, good TZ): fail\n");

      /* 9. utc_gmtime */

      if (utc_gmtime(&tim, &tns, &inac_tim, &ins, &utc) >= 0)
         tet_infoline("utc_gmtime (random utc): ok\n");
      else
         tet_infoline("utc_gmtime (random utc): fail\n");

      if (utc_gmtime(&tim, &tns, &inac_tim, &ins, NULL) >= 0)
         tet_infoline("utc_gmtime (NULL utc): ok\n");
      else
         tet_infoline("utc_gmtime (NULL utc): fail\n");

      /* 10. utc_mkgmtime */

      if (utc_mkgmtime(&utc2, &tim, tns, &inac_tim, ins) >= 0)
         tet_infoline("utc_mkgmtime (random tim, random inac_tim, good tns): ok\n");
      else
         tet_infoline("utc_mkgmtime (random tim, random inac_tim, good tns): fail\n");

      if (utc_mkgmtime(&utc2, &tim, tns, NULL, ins) >= 0)
         tet_infoline("utc_mkgmtime (random tim, NULL inac_tim, good tns): ok\n");
      else
         tet_infoline("utc_mkgmtime (random tim, NULL inac_tim, good tns): fail\n");

      if (utc_mkgmtime(&utc2, &tim, NULL, &inac_tim, NULL) >= 0)
         tet_infoline("utc_mkgmtime (random tim, random inac_tim, NULL tns): ok\n");
      else
         tet_infoline("utc_mkgmtime (random tim, random inac_tim, NULL tns): fail\n");

      if (utc_mkgmtime(&utc2, NULL, tns, &inac_tim, ins) >= 0)
         tet_infoline("utc_mkgmtime (NULL tim, random inac_tim, good tns): ok\n");
      else
         tet_infoline("utc_mkgmtime (NULL tim, random inac_tim, good tns): fail\n");

      /* 11. utc_anytime */

      if (utc_anytime(&tim, &tns, &inac_tim, &ins, &tdf, &utc) >= 0)
         tet_infoline("utc_anytime (random utc): ok\n");
      else
         tet_infoline("utc_anytime (random utc): fail\n");

      if (utc_anytime(&tim, &tns, &inac_tim, &ins, &tdf, NULL) >= 0)
         tet_infoline("utc_anytime (NULL utc): ok\n");
      else
         tet_infoline("utc_anytime (NULL utc): fail\n");

      /* 12. utc_mkanytime */

      if (utc_mkanytime(&utc2, &tim, tns, &inac_tim, ins, tdf) >= 0)
         tet_infoline("utc_mkanytime (random tim, random inac_tim, good tns, good tdf): ok\n");
      else
         tet_infoline("utc_mkanytime (random tim, random inac_tim, good tns, good tdf): fail\n");

      if (utc_mkanytime(&utc2, &tim, tns, NULL, ins, tdf) >= 0)
         tet_infoline("utc_mkanytime (random tim, NULL inac_tim, good tns, good tdf): ok\n");
      else
         tet_infoline("utc_mkanytime (random tim, NULL inac_tim, good tns, good tdf): fail\n");

      if (utc_mkanytime(&utc2, &tim, NULL, &inac_tim, NULL, tdf) >= 0)
         tet_infoline("utc_mkanytime (random tim, random inac_tim, NULL tns, good tdf): ok\n");
      else
         tet_infoline("utc_mkanytime (random tim, random inac_tim, NULL tns, good tdf): fail\n");

      if (utc_mkanytime(&utc2, &tim, tns, &inac_tim, ins, NULL) >= 0)
         tet_infoline("utc_mkanytime (random tim, random inac_tim, good tns, NULL tdf): ok\n");
      else
         tet_infoline("utc_mkanytime (random tim, random inac_tim, good tns, NULL tdf): fail\n");

      if (utc_mkanytime(&utc2, NULL, tns, &inac_tim, ins, tdf) >= 0)
         tet_infoline("utc_mkanytime (NULL tim, random inac_tim, good tns, good tdf): ok\n");
      else
         tet_infoline("utc_mkanytime (NULL tim, random inac_tim, good tns, good tdf): fail\n");

      /* 13. utc_reltime */

      if (utc_reltime(&tim, &tns, &inac_tim, &ins, &utc) >= 0)
         tet_infoline("utc_reltime (random utc): ok\n");
      else
         tet_infoline("utc_reltime (random utc): fail\n");

      if (utc_reltime(&tim, &tns, &inac_tim, &ins, NULL) >= 0)
         tet_infoline("utc_reltime (NULL utc): ok\n");
      else
         tet_infoline("utc_reltime (NULL utc): fail\n");

      /* 14. utc_mkreltime */

      if (utc_mkreltime(&utc2, &tim, tns, &inac_tim, ins) >= 0)
         tet_infoline("utc_mkreltime (random tim, random inac_tim, good tns): ok\n");
      else
         tet_infoline("utc_mkreltime (random tim, random inac_tim, good tns): fail\n");

      if (utc_mkreltime(&utc2, &tim, tns, NULL, ins) >= 0)
         tet_infoline("utc_mkreltime (random tim, NULL inac_tim, good tns): ok\n");
      else
         tet_infoline("utc_mkreltime (random tim, NULL inac_tim, good tns): fail\n");

      if (utc_mkreltime(&utc2, &tim, NULL, &inac_tim, NULL) >= 0)
         tet_infoline("utc_mkreltime (random tim, random inac_tim, NULL tns): ok\n");
      else
         tet_infoline("utc_mkreltime (random tim, random inac_tim, NULL tns): fail\n");

      if (utc_mkreltime(&utc2, NULL, tns, &inac_tim, ins) >= 0)
         tet_infoline("utc_mkreltime (NULL tim, random inac_tim, good tns): ok\n");
      else
         tet_infoline("utc_mkreltime (NULL tim, random inac_tim, good tns): fail\n");

      /* 15. utc_abstime */

      rts.tv_sec = random32() / 2;
      rts.tv_nsec = 100 * (random32() / 500);
      inac.tv_sec = random32() / 2;
      inac.tv_nsec = 100 * (random32() / 500);
      utc_mkbinreltime(&utcm, &rts, &inac);

      if (utc_abstime(&jimutc, &utcm) >= 0)
         tet_infoline("utc_abstime (random utc): ok\n");
      else
         tet_infoline("utc_abstime (random utc): fail\n");
 
      if (utc_abstime(&jimutc, NULL) >= 0)
         tet_infoline("utc_abstime (NULL utc): ok\n");
      else
         tet_infoline("utc_abstime (NULL utc): fail\n");
 
      /* 16. utc_addtime */

      rts.tv_sec = random32() / 2;
      rts.tv_nsec = 100 * (random32() / 500);
      inac.tv_sec = random32() / 2;
      inac.tv_nsec = 100 * (random32() / 500);
      utc_mkbinreltime(&utcrel1, &rts, &inac);

      rts.tv_sec = random32() / 2;
      rts.tv_nsec = 100 * (random32() / 500);
      inac.tv_sec = random32() / 2;
      inac.tv_nsec = 100 * (random32() / 500);
      utc_mkbinreltime(&utcrel2, &rts, &inac);

      ts.tv_sec = random32() / 2;
      ts.tv_nsec = 100 * (random32() / 500);
      inac.tv_sec = random32() / 2;
      inac.tv_nsec = 100 * (random32() / 500);
      tdf = (random32() / (65536*60)) * 60;
      utc_mkbintime(&utcbin1, &ts, &inac, tdf);

      ts.tv_sec = random32() / 2;
      ts.tv_nsec = 100 * (random32() / 500);
      inac.tv_sec = random32() / 2;
      inac.tv_nsec = 100 * (random32() / 500);
      tdf = (random32() / (65536*60)) * 60;
      utc_mkbintime(&utcbin2, &ts, &inac, tdf);

      if (utc_addtime(&jimutc, &utcrel1, &utcrel2) >= 0)
         tet_infoline("utc_addtime (relative + relative): ok\n");
      else
         tet_infoline("utc_addtime (relative + relative): fail\n");

      if (utc_addtime(&jimutc, &utcrel1, NULL) >= 0)
         tet_infoline("utc_addtime (relative + NULL): ok\n");
      else
         tet_infoline("utc_addtime (relative + NULL): fail\n");

      if (utc_addtime(&jimutc, NULL, &utcrel2) >= 0)
         tet_infoline("utc_addtime (NULL + relative): ok\n");
      else
         tet_infoline("utc_addtime (NULL + relative): fail\n");

      if (utc_addtime(&jimutc, NULL, NULL) >= 0)
         tet_infoline("utc_addtime (NULL + NULL): ok\n");
      else
         tet_infoline("utc_addtime (NULL + NULL): fail\n");

      if (utc_addtime(&jimutc, &utcbin1, &utcbin2) >= 0)
         tet_infoline("utc_addtime (absolute + absolute): ok\n");
      else
         tet_infoline("utc_addtime (absolute + absolute): fail\n");

      if (utc_addtime(&jimutc, &utcbin1, NULL) >= 0)
         tet_infoline("utc_addtime (absolute + NULL): ok\n");
      else
         tet_infoline("utc_addtime (absolute + NULL): fail\n");

      if (utc_addtime(&jimutc, NULL, &utcbin2) >= 0)
         tet_infoline("utc_addtime (NULL + absolute): ok\n");
      else
         tet_infoline("utc_addtime (NULL + absolute): fail\n");

      if (utc_addtime(&jimutc, &utcrel1, &utcbin1) >= 0)
         tet_infoline("utc_addtime (relative + absolute): ok\n");
      else
         tet_infoline("utc_addtime (relative + absolute): fail\n");

      if (utc_addtime(&jimutc, &utcbin2, &utcrel2) >= 0)
         tet_infoline("utc_addtime (absolute + relative): ok\n");
      else
         tet_infoline("utc_addtime (absolute + relative): fail\n");

   /* 17. utc_subtime */

   rts.tv_sec = random32() / 2;
   rts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   utc_mkbinreltime(&utcrel1, &rts, &inac);

   rts.tv_sec = random32() / 2;
   rts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   utc_mkbinreltime(&utcrel2, &rts, &inac);

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;
   utc_mkbintime(&utcbin1, &ts, &inac, tdf);

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;
   utc_mkbintime(&utcbin2, &ts, &inac, tdf);

   if (utc_subtime(&jimutc, &utcrel1, &utcrel2) >= 0)
      tet_infoline("utc_subtime (relative + relative): ok\n");
   else
      tet_infoline("utc_subtime (relative + relative): fail\n");

   if (utc_subtime(&jimutc, &utcrel1, NULL) >= 0)
      tet_infoline("utc_subtime (relative + NULL): ok\n");
   else
      tet_infoline("utc_subtime (relative + NULL): fail\n");

   if (utc_subtime(&jimutc, NULL, &utcrel2) >= 0)
      tet_infoline("utc_subtime (NULL + relative): ok\n");
   else
      tet_infoline("utc_subtime (NULL + relative): fail\n");

   if (utc_subtime(&jimutc, NULL, NULL) >= 0)
      tet_infoline("utc_subtime (NULL + NULL): ok\n");
   else
      tet_infoline("utc_subtime (NULL + NULL): fail\n");

   if (utc_subtime(&jimutc, &utcbin1, &utcbin2) >= 0)
      tet_infoline("utc_subtime (absolute + absolute): ok\n");
   else
      tet_infoline("utc_subtime (absolute + absolute): fail\n");

   if (utc_subtime(&jimutc, &utcbin1, NULL) >= 0)
      tet_infoline("utc_subtime (absolute + NULL): ok\n");
   else
      tet_infoline("utc_subtime (absolute + NULL): fail\n");

   if (utc_subtime(&jimutc, NULL, &utcbin2) >= 0)
      tet_infoline("utc_subtime (NULL + absolute): ok\n");
   else
      tet_infoline("utc_subtime (NULL + absolute): fail\n");

   if (utc_subtime(&jimutc, &utcrel1, &utcbin1) >= 0)
      tet_infoline("utc_subtime (relative + absolute): ok\n");
   else
      tet_infoline("utc_subtime (relative + absolute): fail\n");

   if (utc_subtime(&jimutc, &utcbin2, &utcrel2) >= 0)
      tet_infoline("utc_subtime (absolute + relative): ok\n");
   else
      tet_infoline("utc_subtime (absolute + relative): fail\n");

   /* 18. utc_multime */

   rts.tv_sec = random32() / 2;
   rts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   utc_mkbinreltime(&utcrel1, &rts, &inac);

   if (utc_multime(&jimutc, &utcrel1, 15L) >= 0)
      tet_infoline("utc_multime (relative * 15L): ok\n");
   else
      tet_infoline("utc_multime (relative * 15L): fail\n");

   if (utc_multime(&jimutc, &utcrel1, 0L) >= 0)
      tet_infoline("utc_multime (relative * 0L): ok\n");
   else
      tet_infoline("utc_multime (relative * 0L): fail\n");

   if (utc_multime(&jimutc, &utcrel1, -15L) >= 0)
      tet_infoline("utc_multime (relative * -15L): ok\n");
   else
      tet_infoline("utc_multime (relative * -15L): fail\n");

   if (utc_multime(&jimutc, &utcrel1, -45.6) >= 0)
      tet_infoline("utc_multime (relative * -45.6): ok\n");
   else
      tet_infoline("utc_multime (relative * -45.6): fail\n");

   if (utc_multime(&jimutc, NULL, 15L) >= 0)
      tet_infoline("utc_multime (NULL * 15L): ok\n");
   else
      tet_infoline("utc_multime (NULL * 15L): fail\n");

   if (utc_multime(&jimutc, NULL, 0L) >= 0)
      tet_infoline("utc_multime (NULL * 0L): ok\n");
   else
      tet_infoline("utc_multime (NULL * 0L): fail\n");

   if (utc_multime(&jimutc, NULL, -15L) >= 0)
      tet_infoline("utc_multime (NULL * -15L): ok\n");
   else
      tet_infoline("utc_multime (NULL * -15L): fail\n");

   /* 19. utc_mulftime */

   rts.tv_sec = random32() / 2;
   rts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   utc_mkbinreltime(&utcrel1, &rts, &inac);

   if (utc_mulftime(&jimutc, &utcrel1, 15.45) >= 0)
      tet_infoline("utc_mulftime (relative * 15.45): ok\n");
   else
      tet_infoline("utc_mulftime (relative * 15.45): fail\n");

   if (utc_mulftime(&jimutc, &utcrel1, 0.0) >= 0)
      tet_infoline("utc_mulftime (relative * 0.0): ok\n");
   else
      tet_infoline("utc_mulftime (relative * 0.0): fail\n");

   if (utc_mulftime(&jimutc, &utcrel1, -15.78) >= 0)
      tet_infoline("utc_mulftime (relative * -15.78): ok\n");
   else
      tet_infoline("utc_mulftime (relative * -15.78): fail\n");

   if (utc_mulftime(&jimutc, &utcrel1, 17) >= 0)
      tet_infoline("utc_mulftime (relative * 17): ok\n");
   else
      tet_infoline("utc_mulftime (relative * 17): fail\n");

   if (utc_mulftime(&jimutc, NULL, 15.45) >= 0)
      tet_infoline("utc_mulftime (NULL * 15.45): ok\n");
   else
      tet_infoline("utc_mulftime (NULL * 15.45): fail\n");

   if (utc_mulftime(&jimutc, NULL, 0.0) >= 0)
      tet_infoline("utc_mulftime (NULL * 0.0): ok\n");
   else
      tet_infoline("utc_mulftime (NULL * 0.0): fail\n");

   if (utc_mulftime(&jimutc, NULL, -15.78) >= 0)
      tet_infoline("utc_mulftime (NULL * -15.78): ok\n");
   else
      tet_infoline("utc_mulftime (NULL * -15.78): fail\n");

   /* 20. utc_cmpmidtime */

   rts.tv_sec = random32() / 2;
   rts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   utc_mkbinreltime(&utcrel1, &rts, &inac);

   rts.tv_sec = random32() / 2;
   rts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   utc_mkbinreltime(&utcrel2, &rts, &inac);

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;
   utc_mkbintime(&utcbin1, &ts, &inac, tdf);

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;
   utc_mkbintime(&utcbin2, &ts, &inac, tdf);

   if (utc_cmpmidtime(&ctype, &utcrel1, &utcrel2) >= 0)
      tet_infoline("utc_cmpmidtime (relative to relative): ok\n");
   else
      tet_infoline("utc_cmpmidtime (relative to relative): fail\n");

   if (utc_cmpmidtime(&ctype, &utcrel1, NULL) >= 0)
      tet_infoline("utc_cmpmidtime (relative to NULL): ok\n");
   else
      tet_infoline("utc_cmpmidtime (relative to NULL): fail\n");

   if (utc_cmpmidtime(&ctype, NULL, &utcrel2) >= 0)
      tet_infoline("utc_cmpmidtime (NULL to relative): ok\n");
   else
      tet_infoline("utc_cmpmidtime (NULL to relative): fail\n");

   if (utc_cmpmidtime(&ctype, NULL, NULL) >= 0)
      tet_infoline("utc_cmpmidtime (NULL to NULL): ok\n");
   else
      tet_infoline("utc_cmpmidtime (NULL to NULL): fail\n");

   if (utc_cmpmidtime(&ctype, &utcbin1, &utcbin2) >= 0)
      tet_infoline("utc_cmpmidtime (absolute to absolute): ok\n");
   else
      tet_infoline("utc_cmpmidtime (absolute to absolute): fail\n");

   if (utc_cmpmidtime(&ctype, &utcbin1, NULL) >= 0)
      tet_infoline("utc_cmpmidtime (absolute to NULL): ok\n");
   else
      tet_infoline("utc_cmpmidtime (absolute to NULL): fail\n");

   if (utc_cmpmidtime(&ctype, NULL, &utcbin2) >= 0)
      tet_infoline("utc_cmpmidtime (NULL to absolute): ok\n");
   else
      tet_infoline("utc_cmpmidtime (NULL to absolute): fail\n");

   if (utc_cmpmidtime(&ctype, &utcbin1, &utcrel1) >= 0)
      tet_infoline("utc_cmpmidtime (absolute to relative): ok\n");
   else
      tet_infoline("utc_cmpmidtime (absolute to relative): fail\n");

   if (utc_cmpmidtime(&ctype, &utcrel2, &utcbin2) >= 0)
      tet_infoline("utc_cmpmidtime (relative to absolute): ok\n");
   else
      tet_infoline("utc_cmpmidtime (relative to absolute): fail\n");

   /* 21. utc_cmpintervaltime */

   rts.tv_sec = random32() / 2;
   rts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   utc_mkbinreltime(&utcrel1, &rts, &inac);

   rts.tv_sec = random32() / 2;
   rts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   utc_mkbinreltime(&utcrel2, &rts, &inac);

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;
   utc_mkbintime(&utcbin1, &ts, &inac, tdf);

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;
   utc_mkbintime(&utcbin2, &ts, &inac, tdf);

   if (utc_cmpintervaltime(&ctype, &utcrel1, &utcrel2) >= 0)
      tet_infoline("utc_cmpintervaltime (relative to relative): ok\n");
   else
      tet_infoline("utc_cmpintervaltime (relative to relative): fail\n");

   if (utc_cmpintervaltime(&ctype, &utcrel1, NULL) >= 0)
      tet_infoline("utc_cmpintervaltime (relative to NULL): ok\n");
   else
      tet_infoline("utc_cmpintervaltime (relative to NULL): fail\n");

   if (utc_cmpintervaltime(&ctype, NULL, &utcrel2) >= 0)
      tet_infoline("utc_cmpintervaltime (NULL to relative): ok\n");
   else
      tet_infoline("utc_cmpintervaltime (NULL to relative): fail\n");

   if (utc_cmpintervaltime(&ctype, NULL, NULL) >= 0)
      tet_infoline("utc_cmpintervaltime (NULL to NULL): ok\n");
   else
      tet_infoline("utc_cmpintervaltime (NULL to NULL): fail\n");

   if (utc_cmpintervaltime(&ctype, &utcbin1, &utcbin2) >= 0)
      tet_infoline("utc_cmpintervaltime (absolute to absolute): ok\n");
   else
      tet_infoline("utc_cmpintervaltime (absolute to absolute): fail\n");

   if (utc_cmpintervaltime(&ctype, &utcbin1, NULL) >= 0)
      tet_infoline("utc_cmpintervaltime (absolute to NULL): ok\n");
   else
      tet_infoline("utc_cmpintervaltime (absolute to NULL): fail\n");

   if (utc_cmpintervaltime(&ctype, NULL, &utcbin2) >= 0)
      tet_infoline("utc_cmpintervaltime (NULL to absolute): ok\n");
   else
      tet_infoline("utc_cmpintervaltime (NULL to absolute): fail\n");

   if (utc_cmpintervaltime(&ctype, &utcbin1, &utcrel1) >= 0)
      tet_infoline("utc_cmpintervaltime (absolute to relative): ok\n");
   else
      tet_infoline("utc_cmpintervaltime (absolute to relative): fail\n");

   if (utc_cmpintervaltime(&ctype, &utcrel2, &utcbin2) >= 0)
      tet_infoline("utc_cmpintervaltime (relative to absolute): ok\n");
   else
      tet_infoline("utc_cmpintervaltime (relative to absolute): fail\n");

   /* 22. utc_boundtime */

   rts.tv_sec = random32() / 2;
   rts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   utc_mkbinreltime(&utcrel1, &rts, &inac);

   rts.tv_sec = random32() / 2;
   rts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   utc_mkbinreltime(&utcrel2, &rts, &inac);

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;
   utc_mkbintime(&utcbin1, &ts, &inac, tdf);

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;
   utc_mkbintime(&utcbin2, &ts, &inac, tdf);

   if (utc_boundtime(&jimutc, &utcrel1, &utcrel2) >= 0)
      tet_infoline("utc_boundtime (relative to relative): ok\n");
   else
      tet_infoline("utc_boundtime (relative to relative): fail\n");

   if (utc_boundtime(&jimutc, &utcrel1, NULL) >= 0)
      tet_infoline("utc_boundtime (relative to NULL): ok\n");
   else
      tet_infoline("utc_boundtime (relative to NULL): fail\n");

   if (utc_boundtime(&jimutc, NULL, &utcrel2) >= 0)
      tet_infoline("utc_boundtime (NULL to relative): ok\n");
   else
      tet_infoline("utc_boundtime (NULL to relative): fail\n");

   if (utc_boundtime(&jimutc, NULL, NULL) >= 0)
      tet_infoline("utc_boundtime (NULL to NULL): ok\n");
   else
      tet_infoline("utc_boundtime (NULL to NULL): fail\n");

   if (utc_boundtime(&jimutc, &utcbin1, &utcbin2) >= 0)
      tet_infoline("utc_boundtime (absolute to absolute): ok\n");
   else
      tet_infoline("utc_boundtime (absolute to absolute): fail\n");

   if (utc_boundtime(&jimutc, &utcbin1, NULL) >= 0)
      tet_infoline("utc_boundtime (absolute to NULL): ok\n");
   else
      tet_infoline("utc_boundtime (absolute to NULL): fail\n");

   if (utc_boundtime(&jimutc, NULL, &utcbin2) >= 0)
      tet_infoline("utc_boundtime (NULL to absolute): ok\n");
   else
      tet_infoline("utc_boundtime (NULL to absolute): fail\n");

   if (utc_boundtime(&jimutc, &utcbin1, &utcrel1) >= 0)
      tet_infoline("utc_boundtime (absolute to relative): ok\n");
   else
      tet_infoline("utc_boundtime (absolute to relative): fail\n");

   if (utc_boundtime(&jimutc, &utcrel2, &utcbin2) >= 0)
      tet_infoline("utc_boundtime (relative to absolute): ok\n");
   else
      tet_infoline("utc_boundtime (relative to absolute): fail\n");

   /* 23. utc_spantime */

   rts.tv_sec = random32() / 2;
   rts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   utc_mkbinreltime(&utcrel1, &rts, &inac);

   rts.tv_sec = random32() / 2;
   rts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   utc_mkbinreltime(&utcrel2, &rts, &inac);

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;
   utc_mkbintime(&utcbin1, &ts, &inac, tdf);

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;
   utc_mkbintime(&utcbin2, &ts, &inac, tdf);

   if (utc_spantime(&jimutc, &utcrel1, &utcrel2) >= 0)
      tet_infoline("utc_spantime (relative to relative): ok\n");
   else
      tet_infoline("utc_spantime (relative to relative): fail\n");

   if (utc_spantime(&jimutc, &utcrel1, NULL) >= 0)
      tet_infoline("utc_spantime (relative to NULL): ok\n");
   else
      tet_infoline("utc_spantime (relative to NULL): fail\n");

   if (utc_spantime(&jimutc, NULL, &utcrel2) >= 0)
      tet_infoline("utc_spantime (NULL to relative): ok\n");
   else
      tet_infoline("utc_spantime (NULL to relative): fail\n");

   if (utc_spantime(&jimutc, NULL, NULL) >= 0)
      tet_infoline("utc_spantime (NULL to NULL): ok\n");
   else
      tet_infoline("utc_spantime (NULL to NULL): fail\n");

   if (utc_spantime(&jimutc, &utcbin1, &utcbin2) >= 0)
      tet_infoline("utc_spantime (absolute to absolute): ok\n");
   else
      tet_infoline("utc_spantime (absolute to absolute): fail\n");

   if (utc_spantime(&jimutc, &utcbin1, NULL) >= 0)
      tet_infoline("utc_spantime (absolute to NULL): ok\n");
   else
      tet_infoline("utc_spantime (absolute to NULL): fail\n");

   if (utc_spantime(&jimutc, NULL, &utcbin2) >= 0)
      tet_infoline("utc_spantime (NULL to absolute): ok\n");
   else
      tet_infoline("utc_spantime (NULL to absolute): fail\n");

   if (utc_spantime(&jimutc, &utcbin1, &utcrel1) >= 0)
      tet_infoline("utc_spantime (absolute to relative): ok\n");
   else
      tet_infoline("utc_spantime (absolute to relative): fail\n");

   if (utc_spantime(&jimutc, &utcrel2, &utcbin2) >= 0)
      tet_infoline("utc_spantime (relative to absolute): ok\n");
   else
      tet_infoline("utc_spantime (relative to absolute): fail\n");

   /* 24. utc_pointtime */

   rts.tv_sec = random32() / 2;
   rts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   utc_mkbinreltime(&utcrel1, &rts, &inac);

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;
   utc_mkbintime(&utcbin1, &ts, &inac, tdf);

   if (utc_pointtime(&utc, &utcm, &jimutc, &utcrel1) >= 0)
      tet_infoline("utc_pointtime (relative): ok\n");
   else
      tet_infoline("utc_pointtime (relative): fail\n");

   if (utc_pointtime(&utc, &utcm, &jimutc, &utcbin1) >= 0)
      tet_infoline("utc_pointtime (absolute): ok\n");
   else
      tet_infoline("utc_pointtime (absolute): fail\n");

   if (utc_pointtime(&utc, &utcm, &jimutc, NULL) >= 0)
      tet_infoline("utc_pointtime (NULL): ok\n");
   else
      tet_infoline("utc_pointtime (NULL): fail\n");

   /* 25. utc_localzone */

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;
   utc_mkbintime(&utcbin1, &ts, &inac, tdf);

   setenv("DTS_TZ","",1);
   if (utc_localzone(tzname, (size_t)100, &tdf, &isdst, NULL) >= 0)
      sprintf(stuff,"utc_localzone (NULL DTS_TZ, NULL utc) (%s, %d, %d): ok\n",tzname,isdst,tdf);
   else
      sprintf(stuff,"utc_localzone (NULL DTS_TZ, NULL utc) (%s, %d, %d): fail\n",tzname,isdst,tdf);
   tet_infoline(stuff);

   setenv("DTS_TZ","BITEME",1);
   if (utc_localzone(tzname, (size_t)100, &tdf, &isdst, NULL) >= 0)
      sprintf(stuff,"utc_localzone (bad DTS_TZ, NULL utc) (%s, %d, %d): ok\n",tzname,isdst,tdf);
   else
      sprintf(stuff,"utc_localzone (bad DTS_TZ, NULL utc) (%s, %d, %d): fail\n",tzname,isdst,tdf);
   tet_infoline(stuff);

   setenv("DTS_TZ","PST8PDT",1);
   if (utc_localzone(tzname, (size_t)100, &tdf, &isdst, NULL) >= 0)
      sprintf(stuff,"utc_localzone (good DTS_TZ, NULL utc) (%s, %d, %d): ok\n",tzname,isdst,tdf);
   else
      sprintf(stuff,"utc_localzone (good DTS_TZ, NULL utc) (%s, %d, %d): fail\n",tzname,isdst,tdf);
   tet_infoline(stuff);

   setenv("DTS_TZ","EST5EDT",1);
   if (utc_localzone(tzname, (size_t)100, &tdf, &isdst, NULL) >= 0)
      sprintf(stuff,"utc_localzone (local DTS_TZ, NULL utc) (%s, %d, %d): ok\n",tzname,isdst,tdf);
   else
      sprintf(stuff,"utc_localzone (local DTS_TZ, NULL utc) (%s, %d, %d): fail\n",tzname,isdst,tdf);
   tet_infoline(stuff);

   setenv("DTS_TZ","GMT+1",1);
   if (utc_localzone(tzname, (size_t)100, &tdf, &isdst, &utcbin1) >= 0)
      sprintf(stuff,"utc_localzone (good DTS_TZ, random utc) (%s, %d, %d): ok\n",tzname,isdst,tdf);
   else
      sprintf(stuff,"utc_localzone (good DTS_TZ, random utc) (%s, %d, %d): fail\n",tzname,isdst,tdf);
   tet_infoline(stuff);

   setenv("DTS_TZ","EST5EDT",1);
   if (utc_localzone(tzname, (size_t)100, &tdf, &isdst, &utcbin1) >= 0)
      sprintf(stuff,"utc_localzone (local DTS_TZ, random utc) (%s, %d, %d): ok\n",tzname,isdst,tdf);
   else
      sprintf(stuff,"utc_localzone (local DTS_TZ, random utc) (%s, %d, %d): fail\n",tzname,isdst,tdf);
   tet_infoline(stuff);

   if (utc_localzone(NULL, (size_t)NULL, NULL, NULL, NULL) >= 0)
      tet_infoline("utc_localzone (NULL everything): ok\n");
   else
      tet_infoline("utc_localzone (NULL everything): fail\n");

   /* 26. utc_anyzone */

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;
   utc_mkbintime(&utcbin1, &ts, &inac, tdf);

   setenv("DTS_TZ","EST5EDT",1);
   if (utc_anyzone(tzname, (size_t)100, &tdf, &isdst, NULL) >= 0)
      sprintf(stuff,"utc_anyzone (NULL utc, EST) (%s, %d, %d): ok\n",tzname,tdf,isdst);
   else
      sprintf(stuff,"utc_anyzone (NULL utc, EST) (%s, %d, %d): fail\n",tzname,tdf,isdst);
   tet_infoline(stuff);

   setenv("DTS_TZ","PST8PDT",1);
   if (utc_anyzone(tzname, (size_t)100, &tdf, &isdst, NULL) >= 0)
      sprintf(stuff,"utc_anyzone (NULL utc, PST) (%s, %d, %d): ok\n",tzname,tdf,isdst);
   else
      sprintf(stuff,"utc_anyzone (NULL utc, PST) (%s, %d, %d): fail\n",tzname,tdf,isdst);
   tet_infoline(stuff);

   setenv("DTS_TZ","",1);
   if (utc_anyzone(tzname, (size_t)100, &tdf, &isdst, &utcbin1) >= 0)
      sprintf(stuff,"utc_anyzone (random utc) (%s, %d, %d): ok\n",tzname,tdf,isdst);
   else
      sprintf(stuff,"utc_anyzone (random utc) (%s, %d, %d): fail\n",tzname,tdf,isdst);
   tet_infoline(stuff);

   if (utc_anyzone(NULL, (size_t)NULL, NULL, NULL, NULL) >= 0)
      tet_infoline("utc_anyzone (NULL everything): ok\n");
   else
      tet_infoline("utc_anyzone (NULL everything): fail\n");

   /* 27. utc_gmtzone */

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;
   utc_mkbintime(&utcbin1, &ts, &inac, tdf);

   setenv("DTS_TZ","EST5EDT",1);
   if (utc_gmtzone(tzname, (size_t)100, &tdf, &isdst, NULL) >= 0)
      sprintf(stuff,"utc_gmtzone (NULL utc, EST) (%s, %d, %d): ok\n",tzname,tdf,isdst);
   else
      sprintf(stuff,"utc_gmtzone (NULL utc, EST) (%s, %d, %d): fail\n",tzname,tdf,isdst);
   tet_infoline(stuff);

   setenv("DTS_TZ","PST8PDT",1);
   if (utc_gmtzone(tzname, (size_t)100, &tdf, &isdst, NULL) >= 0)
      sprintf(stuff,"utc_gmtzone (NULL utc, PST) (%s, %d, %d): ok\n",tzname,tdf,isdst);
   else
      sprintf(stuff,"utc_gmtzone (NULL utc, PST) (%s, %d, %d): fail\n",tzname,tdf,isdst);
   tet_infoline(stuff);

   setenv("DTS_TZ","",1);
   if (utc_gmtzone(tzname, (size_t)100, &tdf, &isdst, &utcbin1) >= 0)
      sprintf(stuff,"utc_gmtzone (random utc) (%s, %d, %d): ok\n",tzname,tdf,isdst);
   else
      sprintf(stuff,"utc_gmtzone (random utc) (%s, %d, %d): fail\n",tzname,tdf,isdst);
   tet_infoline(stuff);

   if (utc_gmtzone(NULL, (size_t)NULL, NULL, NULL, NULL) >= 0)
      tet_infoline("utc_gmtzone (NULL everything): ok\n");
   else
      tet_infoline("utc_gmtzone (NULL everything): fail\n");

   /* 28. utc_ascgmtime */

   rts.tv_sec = random32() / 2;
   rts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   utc_mkbinreltime(&utcrel1, &rts, &inac);

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;
   utc_mkbintime(&utcbin1, &ts, &inac, tdf);

   if (utc_ascgmtime(tmstr, (size_t)100, &utcrel1) >= 0)
      sprintf(stuff,"utc_ascgmtime (relative utc) (%s): ok\n",tmstr);
   else
      sprintf(stuff,"utc_ascgmtime (relative utc) (%s): fail\n",tmstr);
   tet_infoline(stuff);

   if (utc_ascgmtime(tmstr, (size_t)100, &utcbin1) >= 0)
      sprintf(stuff,"utc_ascgmtime (absolute utc) (%s): ok\n",tmstr);
   else
      sprintf(stuff,"utc_ascgmtime (absolute utc) (%s): fail\n",tmstr);
   tet_infoline(stuff);

   if (utc_ascgmtime(tmstr, (size_t)100, NULL) >= 0)
      sprintf(stuff,"utc_ascgmtime (NULL utc) (%s): ok\n",tmstr);
   else
      sprintf(stuff,"utc_ascgmtime (NULL utc) (%s): fail\n",tmstr);
   tet_infoline(stuff);

   if (utc_ascgmtime(NULL, (size_t)NULL, NULL) >= 0)
      tet_infoline("utc_ascgmtime (NULL everything): ok\n");
   else
      tet_infoline("utc_ascgmtime (NULL everything): fail\n");

   /* 29. utc_asclocaltime */

   rts.tv_sec = random32() / 2;
   rts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   utc_mkbinreltime(&utcrel1, &rts, &inac);

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;
   utc_mkbintime(&utcbin1, &ts, &inac, tdf);

   setenv("DTS_TZ","",1);
   if (utc_asclocaltime(tmstr, (size_t)100, &utcrel1) >= 0)
      sprintf(stuff,"utc_asclocaltime (relative utc, NULL DTS_TZ) (%s): ok\n",tmstr);
   else
      sprintf(stuff,"utc_asclocaltime (relative utc, NULL DTS_TZ) (%s): fail\n",tmstr);
   tet_infoline(stuff);

   if (utc_asclocaltime(tmstr, (size_t)100, &utcbin1) >= 0)
      sprintf(stuff,"utc_asclocaltime (absolute utc, NULL DTS_TZ) (%s): ok\n",tmstr);
   else
      sprintf(stuff,"utc_asclocaltime (absolute utc, NULL DTS_TZ) (%s): fail\n",tmstr);
   tet_infoline(stuff);

   if (utc_asclocaltime(tmstr, (size_t)100, NULL) >= 0)
      sprintf(stuff,"utc_asclocaltime (NULL utc, NULL DTS_TZ) (%s): ok\n",tmstr);
   else
      sprintf(stuff,"utc_asclocaltime (NULL utc, NULL DTS_TZ) (%s): fail\n",tmstr);
   tet_infoline(stuff);

   setenv("DTS_TZ","PST8PDT",1);
   if (utc_asclocaltime(tmstr, (size_t)100, &utcrel1) >= 0)
      sprintf(stuff,"utc_asclocaltime (relative utc, PST DTS_TZ) (%s): ok\n",tmstr);
   else
      sprintf(stuff,"utc_asclocaltime (relative utc, PST DTS_TZ) (%s): fail\n",tmstr);
   tet_infoline(stuff);

   if (utc_asclocaltime(tmstr, (size_t)100, &utcbin1) >= 0)
      sprintf(stuff,"utc_asclocaltime (absolute utc, PST DTS_TZ) (%s): ok\n",tmstr);
   else
      sprintf(stuff,"utc_asclocaltime (absolute utc, PST DTS_TZ) (%s): fail\n",tmstr);
   tet_infoline(stuff);

   if (utc_asclocaltime(tmstr, (size_t)100, NULL) >= 0)
      sprintf(stuff,"utc_asclocaltime (NULL utc, PST DTS_TZ) (%s): ok\n",tmstr);
   else
      sprintf(stuff,"utc_asclocaltime (NULL utc, PST DTS_TZ) (%s): fail\n",tmstr);
   tet_infoline(stuff);

   setenv("DTS_TZ","EST5EDT",1);
   if (utc_asclocaltime(tmstr, (size_t)100, &utcrel1) >= 0)
      sprintf(stuff,"utc_asclocaltime (relative utc, EST DTS_TZ) (%s): ok\n",tmstr);
   else
      sprintf(stuff,"utc_asclocaltime (relative utc, EST DTS_TZ) (%s): fail\n",tmstr);
   tet_infoline(stuff);

   if (utc_asclocaltime(tmstr, (size_t)100, &utcbin1) >= 0)
      sprintf(stuff,"utc_asclocaltime (absolute utc, EST DTS_TZ) (%s): ok\n",tmstr);
   else
      sprintf(stuff,"utc_asclocaltime (absolute utc, EST DTS_TZ) (%s): fail\n",tmstr);
   tet_infoline(stuff);

   if (utc_asclocaltime(tmstr, (size_t)100, NULL) >= 0)
      sprintf(stuff,"utc_asclocaltime (NULL utc, EST DTS_TZ) (%s): ok\n",tmstr);
   else
      sprintf(stuff,"utc_asclocaltime (NULL utc, EST DTS_TZ) (%s): fail\n",tmstr);
   tet_infoline(stuff);

   if (utc_asclocaltime(NULL, (size_t)NULL, NULL) >= 0)
      tet_infoline("utc_asclocaltime (NULL everything): ok\n");
   else
      tet_infoline("utc_asclocaltime (NULL everything): fail\n");

   /* 30. utc_ascanytime */

   rts.tv_sec = random32() / 2;
   rts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   utc_mkbinreltime(&utcrel1, &rts, &inac);

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;
   utc_mkbintime(&utcbin1, &ts, &inac, tdf);

   if (utc_ascanytime(tmstr, (size_t)100, &utcrel1) >= 0)
      sprintf(stuff,"utc_ascanytime (relative utc) (%s): ok\n",tmstr);
   else
      sprintf(stuff,"utc_ascanytime (relative utc) (%s): fail\n",tmstr);
   tet_infoline(stuff);

   if (utc_ascanytime(tmstr, (size_t)100, &utcbin1) >= 0)
      sprintf(stuff,"utc_ascanytime (absolute utc) (%s): ok\n",tmstr);
   else
      sprintf(stuff,"utc_ascanytime (absolute utc) (%s): fail\n",tmstr);
   tet_infoline(stuff);

   if (utc_ascanytime(tmstr, (size_t)100, NULL) >= 0)
      sprintf(stuff,"utc_ascanytime (NULL utc) (%s): ok\n",tmstr);
   else
      sprintf(stuff,"utc_ascanytime (NULL utc) (%s): fail\n",tmstr);
   tet_infoline(stuff);

   if (utc_ascanytime(NULL, (size_t)NULL, NULL) >= 0)
      tet_infoline("utc_ascanytime (NULL everything): ok\n");
   else
      tet_infoline("utc_ascanytime (NULL everything): fail\n");

   /* 31. utc_mkasctime */

   if (utc_mkasctime(&jimutc, tmstr) >=0)
      tet_infoline("utc_mkasctime (good tmstr): ok\n");
   else
      tet_infoline("utc_mkasctime (good tmstr): fail\n");

   strcpy(tmstr,"biteme");
   if (utc_mkasctime(&jimutc, tmstr) >=0)
      tet_infoline("utc_mkasctime (bad tmstr): ok\n");
   else
      tet_infoline("utc_mkasctime (bad tmstr): fail\n");

   if (utc_mkasctime(&jimutc, NULL) >=0)
      tet_infoline("utc_mkasctime (NULL tmstr): ok\n");
   else
      tet_infoline("utc_mkasctime (NULL tmstr): fail\n");

   /* 32. utc_ascreltime */

   rts.tv_sec = random32() / 2;
   rts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   utc_mkbinreltime(&utcrel1, &rts, &inac);

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;
   utc_mkbintime(&utcbin1, &ts, &inac, tdf);

   if (utc_ascreltime(tmstr, (size_t)100, &utcrel1) >= 0)
      sprintf(stuff,"utc_ascreltime (relative utc) (%s): ok\n",tmstr);
   else
      sprintf(stuff,"utc_ascreltime (relative utc) (%s): fail\n",tmstr);
   tet_infoline(stuff);

   if (utc_ascreltime(tmstr, (size_t)100, &utcbin1) >= 0)
      sprintf(stuff,"utc_ascreltime (absolute utc) (%s): ok\n",tmstr);
   else
      sprintf(stuff,"utc_ascreltime (absolute utc) (%s): fail\n",tmstr);
   tet_infoline(stuff);

   if (utc_ascreltime(tmstr, (size_t)100, NULL) >= 0)
      sprintf(stuff,"utc_ascreltime (NULL utc) (%s): ok\n",tmstr);
   else
      sprintf(stuff,"utc_ascreltime (NULL utc) (%s): fail\n",tmstr);
   tet_infoline(stuff);

   if (utc_ascreltime(NULL, (size_t)NULL, NULL) >= 0)
      tet_infoline("utc_ascreltime (NULL everything): ok\n");
   else
      tet_infoline("utc_ascreltime (NULL everything): fail\n");

      /* 33. utc_mkascreltime */

      if (utc_mkascreltime(&jimutc, tmstr) >=0)
         tet_infoline("utc_mkascreltime (good tmstr): ok\n");
      else
         tet_infoline("utc_mkascreltime (good tmstr): fail\n");

      strcpy(tmstr,"biteme");
      if (utc_mkascreltime(&jimutc, tmstr) >=0)
         tet_infoline("utc_mkascreltime (bad tmstr): ok\n");
      else
         tet_infoline("utc_mkascreltime (bad tmstr): fail\n");

      if (utc_mkascreltime(&jimutc, NULL) >=0)
         tet_infoline("utc_mkascreltime (NULL tmstr): ok\n");
      else
         tet_infoline("utc_mkascreltime (NULL tmstr): fail\n");

   /* 34. utc_getTCPtime */

   hostname = "reef";

   if ((utc_getTCPtime(hostname, &utc_tcp)) < 0 ) {
       tet_infoline("utc_getTCPtime (reef): fail\n");
       ok = 0;
   }
   else 
       tet_infoline("utc_getTCPtime (reef): ok\n");


   hostname = "col.hp.com";

   if ((utc_getTCPtime(hostname, &utc_tcp)) < 0 ) {
       tet_infoline("utc_getTCPtime (col.hp.com): fail\n");
       ok = 0;
   }
   else { 
       tet_infoline("utc_getTCPtime (col.hp.com): ok\n");
   } 
 }

    if (ok) {
        printf("PASSED, Test 1 'RANCOVER_API' : test ran successfully\n");
	tet_result(TET_PASS);
    }
    else {
        printf("FAILED, Test 1 'RANCOVER_API' : test ran unsuccessfully\n");
	tet_result(TET_FAIL);
    }
}
