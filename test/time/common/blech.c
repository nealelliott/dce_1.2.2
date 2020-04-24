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
 * $Log: blech.c,v $
 * Revision 1.1.2.2  1996/03/09  20:52:44  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:30:16  marty]
 *
 * Revision 1.1.2.1  1995/12/11  23:05:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:37:04  root]
 * 
 * $EndLog$
 */
/*
 * @HP_COPYRIGHT@
 */
/*
 * HISTORY
 * $Log: blech.c,v $
 * Revision 1.1.2.2  1996/03/09  20:52:44  marty
 * 	Add OSF copyright
 * 	[1996/03/09  20:30:16  marty]
 *
 * Revision 1.1.3.2  1996/03/09  20:30:16  marty
 * 	Add OSF copyright
 *
 * Revision 1.1.2.1  1995/12/11  23:05:25  root
 * 	Submit OSF/DCE 1.2.1
 * 	[1995/12/11  22:37:04  root]
 *
 * Revision 1.1.1.2  1995/12/11  22:37:04  root
 * 	Submit OSF/DCE 1.2.1
 *
 *	HP revision /main/HPDCE02/jrr_1.2_mothra/1  1995/11/29  21:09 UTC  psn
 * 	Conditionalize dtstime call for __hpux only.
 * 
 *	HP revision /main/HPDCE02/3  1995/05/26  14:02 UTC  pare
 * 	Merge change to HPDCE02
 * 
 *	HP revision /main/HPDCE02/pare_mothra/1  1995/05/25  16:49 UTC  pare
 * 	Remove dtstime call for HP-UX 10.0
 * 
 *	HP revision /main/HPDCE02/2  1994/07/25  17:59 UTC  pare
 * 	remove obsolete call to utc_strtoutc and utc_strtorel
 * 
 *	HP revision /main/HPDCE02/1  1994/07/25  17:37 UTC  pare
 * 	createst initial version
 * 
 * Revision 1.1.2.2  1993/07/14  17:03:04  truitt
 * 	New test for King Kong.
 * 	[1993/07/14  17:02:16  truitt]
 * 
 * $EndLog$
 */
#include <sys/types.h>
#include <sys/time.h>
#include <time.h>
#include <utctime.h>
#include <tet_api.h>

#define _UTC_H	"Don't expand this include file!"
#define _DNA_UTIL_H "Don't expand this include file!"

#include <dtss_config.h>
#include <utc_shared.h>

extern SharedState *utc_sharedptr;

#define PLUSMINUS '\261'

void (*tet_startup)() = NULL;
void (*tet_cleanup)() = NULL;
void blech();

struct tet_testlist tet_testlist[] =
   {{blech, 1},
    {NULL, NULL}};

unsigned long int random32()
{
   return(((rand() & 0x7fff) << 17) +
          ((rand() & 0x7fff) << 2) + (rand() & 0x3) );
}
#if !defined(__hpux1000p) && defined(__hpux)
/* call dtstime with the second parameter not NULL */
void ran01()
{
   struct timeval  tv;

   gettimeofday(&tv, (struct timezone *)NULL);
   tv.tv_sec = 0;
   tv.tv_usec = 0;
   dtstime(&tv, &tv);
}
#endif

/* call utc_sharedunmap with utc_sharedptr set to NULL */
void ran02()
{
   utc_sharedptr = NULL;
   utc_sharedunmap();
}

/* call utc_sharedunmap with utc_sharedptr set to a bogus value */
void ran03()
{
   utc_sharedptr = (SharedState *)7777;
   utc_sharedunmap();
}

/* call utc_mklocaltime with isdst pointer negative */
void ran04()
{
   timespec_t  ts, inac;
   struct tm   tim, inac_tim;
   utc_t       utc, utc2;
   long        tdf, tns, ins;

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;

   utc_mkbintime(&utc, &ts, &inac, tdf);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);

   tim.tm_isdst= -1;
   setenv("DTS_TZ","EST5EDT",1);
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);
   setenv("DTS_TZ","",1);
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);

   tim.tm_isdst = 0;
   setenv("DTS_TZ","EST5EDT",1);
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);
   setenv("DTS_TZ","",1);
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);

   tim.tm_isdst = 1;
   setenv("DTS_TZ","EST5EDT",1);
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);
   setenv("DTS_TZ","",1);
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);
}

/* some random gmtime calls */
void ran05()
{
   timespec_t  ts, inac;
   struct tm   tim, inac_tim;
   long        tdf, tns, ins;
   utc_t       utc;

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;

   utc_mkbintime(&utc, &ts, &inac, tdf);

   utc_gmtime(NULL, NULL, NULL, NULL, &utc);
   utc_gmtime(NULL, NULL, &inac_tim, &ins, &utc);
   utc_gmtime(&tim, &tns, &inac_tim, &ins, &utc);
}

/* call mklocaltime with a number of different years to hit mergetime() code */
void ran07()
{
   timespec_t  ts, inac;
   struct tm   tim, inac_tim;
   long        tdf, tns, ins;
   utc_t       utc, utc2;

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;

   utc_mkbintime(&utc, &ts, &inac, tdf);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);

   utc_mklocaltime(&utc2, NULL, tns, &inac_tim, ins);

   tim.tm_year = -31040;
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);
   utc_mkgmtime(&utc2, &tim, tns, &inac_tim, ins);
   utc_mkanytime(&utc2, &tim, tns, &inac_tim, ins);

   tim.tm_year = 31743;
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);

   tim.tm_year = -318;
   tim.tm_mon = 3;
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);

   tim.tm_year = -318;
   tim.tm_mon = 10;
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);

   tim.tm_year = -767;
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);

   tim.tm_year = -318;
   tim.tm_mon = 9;
   tim.tm_mday = 1;
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);

   tim.tm_year = -318;
   tim.tm_mon = 9;
   tim.tm_mday = 15;
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);

   tim.tm_year = -318;
   tim.tm_mon = 9;
   tim.tm_mday = 7;
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);

   tim.tm_year = -19845;
   tim.tm_mon = 0;
   tim.tm_mday = 5;
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);

   tim.tm_year = -1901;
   tim.tm_mon = 0;
   tim.tm_mday = 1;
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);

   tim.tm_year = -1902;
   tim.tm_mon = 0;
   tim.tm_mday = 1;
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);

   tim.tm_year = -1903;
   tim.tm_mon = 0;
   tim.tm_mday = 1;
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);

   tim.tm_year = -1904;
   tim.tm_mon = 0;
   tim.tm_mday = 1;
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);

   tim.tm_year = 76;
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);

   tim.tm_year = 77;
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);

   tim.tm_year = 78;
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);

   tim.tm_year = 79;
   utc_mklocaltime(&utc2, &tim, tns, &inac_tim, ins);

}

/* call a bunch of utc functions with NULL parameters */
void ran06()
{
   timespec_t     ts, inac;
   reltimespec_t  rts;
   long           tdf;
   utc_t          utc, utc2;
   char           name[100];

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   rts.tv_sec = random32() / 2;
   rts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;
   utc_mkbintime(&utc, &ts, &inac, tdf);

   utc_bintime(NULL, NULL, NULL, NULL);
   utc_bintime(NULL, NULL, NULL, &utc);
   utc_binreltime(NULL, NULL, NULL, NULL);
   utc_binreltime(NULL, NULL, NULL, &utc);

   /*utc_mkbinreltime(NULL, NULL, NULL);*/
   inac.tv_sec = -123;
   utc_mkbinreltime(&utc2, &rts, &inac);

   utc_localtime(NULL, NULL, NULL, NULL, NULL);
   utc_localtime(NULL, NULL, NULL, NULL, &utc);

   utc_localzone(NULL, NULL, NULL, NULL, NULL);
   utc_localzone(name, 0, NULL, NULL, &utc);
   utc_anyzone(NULL, NULL, NULL, NULL, NULL);
   utc_anyzone(name, 0, NULL, NULL, &utc);
   utc_gmtzone(NULL, NULL, NULL, NULL, NULL);
   utc_gmtzone(name, 0, NULL, NULL, &utc);

   utc_anytime(NULL, NULL, NULL, NULL, NULL, &utc);
   utc_reltime(NULL, NULL, NULL, NULL, NULL);
   utc_reltime(NULL, NULL, NULL, NULL, &utc);
   utc_mkreltime(&utc, NULL, NULL, NULL, NULL);
   utc_cmpmidtime(NULL, NULL, NULL);
   utc_cmpintervaltime(NULL, NULL, NULL);

   utc_multime(&utc, &utc2, -3);
   utc_multime(&utc, &utc2, 0);
   utc_multime(&utc, &utc2, 3);
   utc_mkbintime(&utc, &ts, &inac, tdf);
   utc_multime(&utc2, &utc, -3);
   utc_multime(&utc2, &utc, 0);
   utc_multime(&utc2, &utc, 3);
}

/* call utc_mulftime with a bunch of different scale factors */
void ran08()
{
   timespec_t  ts, inac;
   long        tdf;
   utc_t       utc, utc2;
   double      k_2_64 = 65536.0 * 65536.0 * 65536.0 * 65536.0;
   double      k_2_32 = 65536.0 * 65536.0;

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;
   utc_mkbintime(&utc, &ts, &inac, tdf);

   utc_mulftime(&utc2, &utc, NULL);
   utc_mulftime(&utc2, &utc, k_2_64+1.0);

   utc_mulftime(&utc2, &utc, k_2_64);
   utc_mulftime(&utc2, &utc, k_2_32);
   utc_mulftime(&utc2, &utc, k_2_32 - 2);
   utc_mulftime(&utc2, &utc, 64.0);

   utc_mulftime(&utc2, &utc, 0.005);
   utc_mulftime(&utc2, &utc, 1 / k_2_32);
   utc_mulftime(&utc2, &utc, 1 / (k_2_32 + 2));
   utc_mulftime(&utc2, &utc, 1 / k_2_64);
   utc_mulftime(&utc2, &utc, 1 / (k_2_64 * 10));
   utc_mulftime(&utc2, &utc, 1 / (k_2_64 / 10));
}

/* call tzset with a bunch of different parameters */
void ran09()
{
   timespec_t  ts, inac;
   long        tdf, tns, ins;
   utc_t       utc;
   struct tm   tim, inac_tim;
   int         i;
   char        biteme[1025];

   ts.tv_sec = random32() / 2;
   ts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   tdf = (random32() / (65536*60)) * 60;
   utc_mkbintime(&utc, &ts, &inac, tdf);

   setenv("DTS_TZ","",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ",":US/Eastern",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ",":US/Eastern",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ",":US/Biteme",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5EDT,M4.1.0,M10.5.0",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5EDT,MOUSE,M10.MOUSE",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","/etc/zoneinfo/localtime",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","/etc/zoneinfo/localtime.wrong",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);

   for (i = 0; i < 1025; i++)
      biteme[i] = 'a';
   biteme[i] = '\0';
   setenv("DTS_TZ",biteme,1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);

   setenv("DTS_TZ",":Blech1",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ",":Blech2",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ",":Blech3",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ",":Blech4",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);

   setenv("DTS_TZ","ES",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST+5EDT-1,M4.1.0,M3.5.0",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST+5EDT-B,M4.1.0,M3.5.0",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST+5EDT-5+,M4.1.0,M3.5.0",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5EDT-72,M4.1.0,M3.5.0",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5EDT+72,M4.1.0,M3.5.0",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5EDT+5:2,M4.1.0,M3.5.0",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5EDT+5:2:2,M4.1.0,M3.5.0",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5EDT+5:2:72,M4.1.0,M3.5.0",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5EDT+5:72:72,M4.1.0,M3.5.0",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);

   setenv("DTS_TZ","EST5EDT+2;M4.1.0,M10.5.0",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5EDT+2;M1.1.0,M2.5.0",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5EDT+2;M4.1.0!M10.5.0",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5EDT+2;M4.1.0,M10.5.0!",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);

   setenv("DTS_TZ","EST5EDT+1,J,",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5EDT+1,J4,",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5EDT+1,J4,J3",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5EDT+1,J74,J83",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5EDT+1,J4/32,J3/23",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5EDT+1,4,3",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);

   setenv("DTS_TZ","EST5EDT+1,M,A",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5EDT+1,M4!1!0,M3!5!0",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5EDT+1,M4.,M3.",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5EDT+1,M4.1,M3.5",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5EDT+1,M4.1.,M3.5.",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
   setenv("DTS_TZ","EST5EDT+1,M4.1!0,M3.5!0",1);
   utc_localtime(&tim, &tns, &inac_tim, &ins, &utc);
}

/* call utc_cvt functions with a bunch of different parameters */
void ran10()
{
   reltimespec_t  rts;
   timespec_t     inac;
   utc_t          utc;
   char           timestr[100];

   utc_ascanytime(timestr, NULL, NULL);
   utc_ascgmtime(timestr, NULL, NULL);
   utc_ascreltime(timestr, NULL, NULL);
   utc_asclocaltime(timestr, NULL, NULL);
   utc_ascsmplreltime(timestr, NULL, NULL);

   rts.tv_sec = random32() / 2;
   rts.tv_nsec = 100 * (random32() / 500);
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   utc_mkbinreltime(&utc, &rts, &inac);
   utc_ascsmplreltime(timestr, (size_t)100, &utc);

   rts.tv_sec = - (random32() / 2);
   rts.tv_nsec = - (100 * (random32() / 500));
   inac.tv_sec = random32() / 2;
   inac.tv_nsec = 100 * (random32() / 500);
   utc_mkbinreltime(&utc, &rts, &inac);
   utc_ascsmplreltime(timestr, (size_t)100, &utc);
}

/* call utc_mkasctime with a huge amount of different strings */
void ran11()
{
   int    i;
   char   timestr[80], badstring[100];
   utc_t  utc;

   utc_mkasctime(&utc, NULL);
   utc_mkasctime(&utc, "");
   utc_mkasctime(&utc,"\0");
   strcpy(timestr,"          ");
   utc_mkasctime(&utc, timestr);
   for (i = 0; i < 99; i++)
      badstring[i] = 'I';
   badstring[i] = '\0';
   utc_mkasctime(&utc, badstring);
   utc_mkasctime(&utc, "'I 3'");
   utc_mkasctime(&utc, "T");

   /* inacc part */
   utc_mkasctime(&utc, "I");
   utc_mkasctime(&utc, "I .");
   utc_mkasctime(&utc, "I ,");
   utc_mkasctime(&utc, "I .3");
   utc_mkasctime(&utc, "I 3");
   utc_mkasctime(&utc, "I -3");
   utc_mkasctime(&utc, "I 3.");
   utc_mkasctime(&utc, "I 3.3");
   utc_mkasctime(&utc, "I 3.33");
   utc_mkasctime(&utc, "I 3:33");
   utc_mkasctime(&utc, "I 33");
   utc_mkasctime(&utc, "I 33.");
   utc_mkasctime(&utc, "I 33.3");
   utc_mkasctime(&utc, "I 33.33");
   utc_mkasctime(&utc, "I 33,33");
   utc_mkasctime(&utc, "I 33:33");
   utc_mkasctime(&utc, "I inf");
   utc_mkasctime(&utc, "I INF");
   utc_mkasctime(&utc, "I -----");
   utc_mkasctime(&utc, "I epsilon");
   utc_mkasctime(&utc, "I '33,33'");
   utc_mkasctime(&utc, "I 'epsilon'");
   utc_mkasctime(&utc, "I +/-");
   utc_mkasctime(&utc, "I +/- 4");
   utc_mkasctime(&utc, "I +/- 4.");
   utc_mkasctime(&utc, "I +/- 4.4");
   utc_mkasctime(&utc, "I +/- 4.44");
   utc_mkasctime(&utc, "I +/- 4:44");
   utc_mkasctime(&utc, "I +/- 44");
   utc_mkasctime(&utc, "I +/- 44.");
   utc_mkasctime(&utc, "I +/- 44.4");
   utc_mkasctime(&utc, "I +/- 44.44");
   utc_mkasctime(&utc, "I +/- 44:44");
   utc_mkasctime(&utc, "I +/- 44,44");
   utc_mkasctime(&utc, "I +/- inf");
   utc_mkasctime(&utc, "I +/- INF");
   utc_mkasctime(&utc, "I +/- -----");
   utc_mkasctime(&utc, "I +/- epsilon");
   utc_mkasctime(&utc, "I '+/- 44.44'");
   utc_mkasctime(&utc, "I '+/- epsilon'");
   timestr[0]='"';
   timestr[1]='I';
   timestr[2]='"';
   timestr[3]='\0';
   utc_mkasctime(&utc,timestr);
   timestr[0]='"';
   timestr[1]='"';
   timestr[2]='\0';
   utc_mkasctime(&utc,timestr);
   timestr[0] = PLUSMINUS;
   timestr[1] = '\0';
   utc_mkasctime(&utc,timestr);

   /* partial day part (full day) */
   utc_mkasctime(&utc,"11");
   utc_mkasctime(&utc,"11 Z");
   utc_mkasctime(&utc,"11 Z 2");
   utc_mkasctime(&utc,"11.");
   utc_mkasctime(&utc,"11.34");
   utc_mkasctime(&utc,"11.34 Z");
   utc_mkasctime(&utc,"11.34 +");
   utc_mkasctime(&utc,"11.34 +01");
   utc_mkasctime(&utc,"11.34 +01 Z");
   utc_mkasctime(&utc,"11.34 +01:");
   utc_mkasctime(&utc,"11.34 +01 I");
   timestr[0] = '1';
   timestr[1] = '1';
   timestr[2] = '.';
   timestr[3] = '3';
   timestr[4] = '4';
   timestr[5] = ' ';
   timestr[6] = '+';
   timestr[7] = '0';
   timestr[8] = '1';
   timestr[9] = PLUSMINUS;
   timestr[10] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"11.34 +01:02");
   timestr[0] = '1';
   timestr[1] = '1';
   timestr[2] = '.';
   timestr[3] = '3';
   timestr[4] = '4';
   timestr[5] = ' ';
   timestr[6] = '+';
   timestr[7] = '0';
   timestr[8] = '1';
   timestr[8] = ':';
   timestr[8] = '0';
   timestr[8] = '2';
   timestr[9] = PLUSMINUS;
   timestr[10] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"11,34 +01:02");
   utc_mkasctime(&utc,"11.34 -");
   utc_mkasctime(&utc,"11.34 -01");
   utc_mkasctime(&utc,"11.34 -01 Z");
   utc_mkasctime(&utc,"11.34 -01:02");
   utc_mkasctime(&utc,"11.34 epsilon");
   utc_mkasctime(&utc,"11 +");
   utc_mkasctime(&utc,"11 +02");
   utc_mkasctime(&utc,"11 +02:03");
   utc_mkasctime(&utc,"11 -");
   utc_mkasctime(&utc,"11 -02");
   utc_mkasctime(&utc,"11 -02:03");
   utc_mkasctime(&utc,"11 epsilon");
   utc_mkasctime(&utc,"11:");
   utc_mkasctime(&utc,"12:-3");
   utc_mkasctime(&utc,"-1:14");
   utc_mkasctime(&utc,"-1:14 I");
   timestr[0] = '-';
   timestr[1] = '1';
   timestr[2] = ':';
   timestr[3] = '1';
   timestr[4] = '4';
   timestr[5] = PLUSMINUS;
   timestr[6] = '\0';
   utc_mkasctime(&utc,timestr);
   timestr[0] = '-';
   timestr[1] = '1';
   timestr[2] = ':';
   timestr[3] = '1';
   timestr[4] = '4';
   timestr[5] = 'Z';
   timestr[6] = PLUSMINUS;
   timestr[7] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"11:12 Z");
   utc_mkasctime(&utc,"11:12 Z 2");
   utc_mkasctime(&utc,"11:12.");
   utc_mkasctime(&utc,"11:12.34");
   utc_mkasctime(&utc,"11:12:34");
   utc_mkasctime(&utc,"11:12.34 I");
   timestr[0] = '1';
   timestr[1] = '1';
   timestr[2] = ':';
   timestr[3] = '1';
   timestr[4] = '2';
   timestr[5] = '.';
   timestr[6] = '3';
   timestr[7] = '4';
   timestr[8] = PLUSMINUS;
   timestr[9] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"11:12.34 Z");
   utc_mkasctime(&utc,"11:12.34 Z I");
   timestr[0] = '1';
   timestr[1] = '1';
   timestr[2] = ':';
   timestr[3] = '1';
   timestr[4] = '2';
   timestr[5] = '.';
   timestr[6] = '3';
   timestr[7] = '4';
   timestr[8] = 'Z';
   timestr[9] = PLUSMINUS;
   timestr[10] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"11:12.34 Z Q");
   utc_mkasctime(&utc,"11:12.34 +");
   utc_mkasctime(&utc,"11:12.34 +01");
   utc_mkasctime(&utc,"11:12.34 +01 Z");
   utc_mkasctime(&utc,"11:12.34 +01:02");
   utc_mkasctime(&utc,"11:12,34 +01:02");
   utc_mkasctime(&utc,"11:12.34 -10");
   utc_mkasctime(&utc,"11:12.34 -10:");
   utc_mkasctime(&utc,"11:12.34 -10:11");
   utc_mkasctime(&utc,"11:12.34 -10:11 Z");
   utc_mkasctime(&utc,"11:12.34 epsilon");
   utc_mkasctime(&utc,"11:12 +");
   utc_mkasctime(&utc,"11:12 +13");
   utc_mkasctime(&utc,"11:12 +13:14");
   utc_mkasctime(&utc,"11:12 -13");
   utc_mkasctime(&utc,"11:12 -13:14");
   utc_mkasctime(&utc,"11:12 epsilon");
   utc_mkasctime(&utc,"12:13: Z");
   utc_mkasctime(&utc,"12:13:-3 Z");
   utc_mkasctime(&utc,"12:13:14 Z");
   utc_mkasctime(&utc,"26:75:73 Z");
   utc_mkasctime(&utc,"12:13:14 Z 2");
   utc_mkasctime(&utc,"12:13:14.15");
   utc_mkasctime(&utc,"12:13:14:15");
   utc_mkasctime(&utc,"12:13:14.15 I");
   timestr[0] = '1';
   timestr[1] = '2';
   timestr[2] = ':';
   timestr[3] = '1';
   timestr[4] = '3';
   timestr[5] = ':';
   timestr[6] = '1';
   timestr[7] = '4';
   timestr[8] = '.';
   timestr[9] = '1';
   timestr[10] = '5';
   timestr[11] = PLUSMINUS;
   timestr[12] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"12:13:14.15 Z");
   utc_mkasctime(&utc,"12:13:14.15 Z I");
   timestr[0] = '1';
   timestr[1] = '2';
   timestr[2] = ':';
   timestr[3] = '1';
   timestr[4] = '3';
   timestr[5] = ':';
   timestr[6] = '1';
   timestr[7] = '4';
   timestr[8] = '.';
   timestr[9] = '1';
   timestr[10] = '5';
   timestr[11] = 'Z';
   timestr[12] = PLUSMINUS;
   timestr[13] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"12:13:14.15 Z Q");
   utc_mkasctime(&utc,"12:13:14.15 +");
   utc_mkasctime(&utc,"12:13:14.15 +03");
   utc_mkasctime(&utc,"12:13:14.15 +03:04");
   utc_mkasctime(&utc,"12:13:14.15 +03.04");
   utc_mkasctime(&utc,"12:13:14.15 +03,04");
   utc_mkasctime(&utc,"12:13:14.15 -03");
   utc_mkasctime(&utc,"12:13:14,15 -03");
   utc_mkasctime(&utc,"12:13:14.15 -03:04");
   utc_mkasctime(&utc,"12:13:14.15 epsilon");
   utc_mkasctime(&utc,"12:13:14 +");
   utc_mkasctime(&utc,"12:13:14 +21");
   utc_mkasctime(&utc,"12:13:14 +21:34");
   utc_mkasctime(&utc,"12:13:14 -21");
   utc_mkasctime(&utc,"12:13:14 -21:34");
   utc_mkasctime(&utc,"12:13:14 epsilon");

   /* partial day part (default day) */
   utc_mkasctime(&utc,"-06 I");
   timestr[0] = '-';
   timestr[1] = '0';
   timestr[2] = '6';
   timestr[3] = PLUSMINUS;
   timestr[4] = '\0';
   utc_mkasctime(&utc,timestr);
   timestr[0] = '-';
   timestr[1] = '0';
   timestr[2] = '6';
   timestr[3] = 'Z';
   timestr[4] = PLUSMINUS;
   timestr[5] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"--34");
   utc_mkasctime(&utc,"--34 Z");
   utc_mkasctime(&utc,"--34 Z 1");
   utc_mkasctime(&utc,"--34.");
   utc_mkasctime(&utc,"--34.45");
   utc_mkasctime(&utc,"--34:45");
   utc_mkasctime(&utc,"--34.45 Z");
   utc_mkasctime(&utc,"--34.45 +");
   utc_mkasctime(&utc,"--34.45 +11");
   utc_mkasctime(&utc,"--34.45 +11:12");
   utc_mkasctime(&utc,"--34.45 -11");
   utc_mkasctime(&utc,"--34,45 -11");
   utc_mkasctime(&utc,"--34.45 -11:12");
   utc_mkasctime(&utc,"--34.45 epsilon");
   utc_mkasctime(&utc,"--34 +");
   utc_mkasctime(&utc,"--34 +12");
   utc_mkasctime(&utc,"--34 +12:13");
   utc_mkasctime(&utc,"--34 -12");
   utc_mkasctime(&utc,"--34 -12:13");
   utc_mkasctime(&utc,"--34 epsilon");
   utc_mkasctime(&utc,"---");
   utc_mkasctime(&utc,"--- Z");
   utc_mkasctime(&utc,"--- Z 1");
   utc_mkasctime(&utc,"---.");
   utc_mkasctime(&utc,"---.56");
   utc_mkasctime(&utc,"---.56 Z");
   utc_mkasctime(&utc,"---:56 Z");
   utc_mkasctime(&utc,"---.56.");
   utc_mkasctime(&utc,"---.56 +");
   utc_mkasctime(&utc,"---.56 +05");
   utc_mkasctime(&utc,"---.56 +05:06");
   utc_mkasctime(&utc,"---.56 -05");
   utc_mkasctime(&utc,"---.56 -05:06");
   utc_mkasctime(&utc,"---,56 -05:06");
   utc_mkasctime(&utc,"---.56 epsilon");
   utc_mkasctime(&utc,"--- +");
   utc_mkasctime(&utc,"--- +05");
   utc_mkasctime(&utc,"--- +05:06");
   utc_mkasctime(&utc,"--- -05");
   utc_mkasctime(&utc,"--- -05:06");
   utc_mkasctime(&utc,"--- epsilon");
   utc_mkasctime(&utc,"-");
   utc_mkasctime(&utc,"-04");
   utc_mkasctime(&utc,"-04 Z");
   utc_mkasctime(&utc,"-04 Z 3");
   utc_mkasctime(&utc,"-04.");
   utc_mkasctime(&utc,"-04.23");
   utc_mkasctime(&utc,"-04.23 Z");
   utc_mkasctime(&utc,"-04.23 +");
   utc_mkasctime(&utc,"-04.23 +02");
   utc_mkasctime(&utc,"-04,23 +02");
   utc_mkasctime(&utc,"-04.23 +02:02");
   utc_mkasctime(&utc,"-04.23 +02,02");
   utc_mkasctime(&utc,"-04.23 -02");
   utc_mkasctime(&utc,"-04.23 -02:02");
   utc_mkasctime(&utc,"-04.23 epsilon");
   utc_mkasctime(&utc,"-04 +");
   utc_mkasctime(&utc,"-04 +02");
   utc_mkasctime(&utc,"-04 +02:02");
   utc_mkasctime(&utc,"-04 -02");
   utc_mkasctime(&utc,"-04 -02:02");
   utc_mkasctime(&utc,"-04 epsilon");
   utc_mkasctime(&utc,"-04:");
   utc_mkasctime(&utc,"-04:05");
   utc_mkasctime(&utc,"-04:05 Z");
   utc_mkasctime(&utc,"-04:05 Z 3");
   utc_mkasctime(&utc,"-04:05.");
   utc_mkasctime(&utc,"-04:05.67");
   utc_mkasctime(&utc,"-04:05.67 Z");
   utc_mkasctime(&utc,"-04:05:67 Z");
   utc_mkasctime(&utc,"-04:05,67");
   utc_mkasctime(&utc,"-04:05.67 +");
   utc_mkasctime(&utc,"-04:05.67 +16");
   utc_mkasctime(&utc,"-04:05.67 +16:16");
   utc_mkasctime(&utc,"-04:05.67 -16");
   utc_mkasctime(&utc,"-04:05.67 -16:16");
   utc_mkasctime(&utc,"-04:05.67 epsilon");
   utc_mkasctime(&utc,"-04:05 +");
   utc_mkasctime(&utc,"-04:05 +16");
   utc_mkasctime(&utc,"-04:05 +16:16");
   utc_mkasctime(&utc,"-04:05 -16");
   utc_mkasctime(&utc,"-04:05 -16:16");
   utc_mkasctime(&utc,"-04:05 epsilon");

   /* year part */
   utc_mkasctime(&utc,"1993");
   timestr[0] = '1';
   timestr[1] = '9';
   timestr[2] = '9';
   timestr[3] = '3';
   timestr[4] = PLUSMINUS;
   timestr[5] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"1993 T");
   utc_mkasctime(&utc,"1993 T I");
   timestr[0] = '1';
   timestr[1] = '9';
   timestr[2] = '9';
   timestr[3] = '3';
   timestr[4] = 'T';
   timestr[5] = PLUSMINUS;
   timestr[6] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"1993 I");
   utc_mkasctime(&utc,"1993 Z");
   utc_mkasctime(&utc,"1993 Z I");
   timestr[0] = '1';
   timestr[1] = '9';
   timestr[2] = '9';
   timestr[3] = '3';
   timestr[4] = 'Z';
   timestr[5] = PLUSMINUS;
   timestr[6] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"1993 T 3");
   utc_mkasctime(&utc,"1993 .69");
   utc_mkasctime(&utc,"1993 .69 I");
   timestr[0] = '1';
   timestr[1] = '9';
   timestr[2] = '9';
   timestr[3] = '3';
   timestr[4] = ' ';
   timestr[5] = '.';
   timestr[6] = '6';
   timestr[7] = '9';
   timestr[8] = PLUSMINUS;
   timestr[9] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"1993 .69 Z");
   utc_mkasctime(&utc,"1993 .69 Z I");
   timestr[0] = '1';
   timestr[1] = '9';
   timestr[2] = '9';
   timestr[3] = '3';
   timestr[4] = ' ';
   timestr[5] = '.';
   timestr[6] = '6';
   timestr[7] = '9';
   timestr[8] = 'Z';
   timestr[9] = PLUSMINUS;
   timestr[10] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"1993 .69 Z Q");
   utc_mkasctime(&utc,"1993-");
   utc_mkasctime(&utc,"1993-04");
   utc_mkasctime(&utc,"1993-04 I");
   timestr[0] = '1';
   timestr[1] = '9';
   timestr[2] = '9';
   timestr[3] = '3';
   timestr[4] = '-';
   timestr[5] = '0';
   timestr[6] = '4';
   timestr[7] = PLUSMINUS;
   timestr[8] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"1993-04 Z");
   timestr[0] = '1';
   timestr[1] = '9';
   timestr[2] = '9';
   timestr[3] = '3';
   timestr[4] = '-';
   timestr[5] = '0';
   timestr[6] = '4';
   timestr[7] = 'Z';
   timestr[8] = PLUSMINUS;
   timestr[9] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"1993-04 Z Q");
   utc_mkasctime(&utc,"1993-04 Z I");
   utc_mkasctime(&utc,"1993-04 T");
   timestr[0] = '1';
   timestr[1] = '9';
   timestr[2] = '9';
   timestr[3] = '3';
   timestr[4] = '-';
   timestr[5] = '0';
   timestr[6] = '4';
   timestr[7] = 'T';
   timestr[8] = PLUSMINUS;
   timestr[9] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"1993-04 T I");
   utc_mkasctime(&utc,"1993-04 Q");
   utc_mkasctime(&utc,"1993 - 04 T");
   utc_mkasctime(&utc,"1993-04 T 3");
   utc_mkasctime(&utc,"1993-epsilon");
   utc_mkasctime(&utc,"1993-04-");
   utc_mkasctime(&utc,"1993-04- Z");
   utc_mkasctime(&utc,"1993-04- Z I");
   utc_mkasctime(&utc,"1993-04- I");
   utc_mkasctime(&utc,"1993-04- Q");
   utc_mkasctime(&utc,"1993-04-epsilon");
   utc_mkasctime(&utc,"1993-04-05");
   utc_mkasctime(&utc,"1993-04-05 Z");
   utc_mkasctime(&utc,"1993-04-05 Z Q");
   utc_mkasctime(&utc,"1993-04-05 Z I");
   timestr[0] = '1';
   timestr[1] = '9';
   timestr[2] = '9';
   timestr[3] = '3';
   timestr[4] = '-';
   timestr[5] = '0';
   timestr[6] = '4';
   timestr[7] = '-';
   timestr[8] = '0';
   timestr[9] = '5';
   timestr[10] = 'Z';
   timestr[11] = PLUSMINUS;
   timestr[12] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"1993-04-05 I");
   utc_mkasctime(&utc,"1993-04-05 T");
   utc_mkasctime(&utc,"1993-04-05 T I");
   timestr[0] = '1';
   timestr[1] = '9';
   timestr[2] = '9';
   timestr[3] = '3';
   timestr[4] = '-';
   timestr[5] = '0';
   timestr[6] = '4';
   timestr[7] = '-';
   timestr[8] = '0';
   timestr[9] = '5';
   timestr[10] = 'T';
   timestr[11] = PLUSMINUS;
   timestr[12] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"1993-04-05:");
   utc_mkasctime(&utc,"1993-04-05.");
   utc_mkasctime(&utc,"1993-04-05,");
   timestr[0] = '1';
   timestr[1] = '9';
   timestr[2] = '9';
   timestr[3] = '3';
   timestr[4] = '-';
   timestr[5] = '0';
   timestr[6] = '4';
   timestr[7] = '-';
   timestr[8] = '0';
   timestr[9] = '5';
   timestr[10] = PLUSMINUS;
   timestr[11] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"1993 - 04 - 05 T");
   utc_mkasctime(&utc,"1993-04-05 T 3");
   utc_mkasctime(&utc,"1993-04-05 T 3,");
   utc_mkasctime(&utc,"1993-04-05 T 3I");
   timestr[0] = '1';
   timestr[1] = '9';
   timestr[2] = '9';
   timestr[3] = '3';
   timestr[4] = '-';
   timestr[5] = '0';
   timestr[6] = '4';
   timestr[7] = '-';
   timestr[8] = '0';
   timestr[9] = '5';
   timestr[10] = 'T';
   timestr[11] = ' ';
   timestr[12] = '3';
   timestr[13] = PLUSMINUS;
   timestr[14] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"1993-04-05 T 3 Z Q");
   timestr[0] = '1';
   timestr[1] = '9';
   timestr[2] = '9';
   timestr[3] = '3';
   timestr[4] = '-';
   timestr[5] = '0';
   timestr[6] = '4';
   timestr[7] = '-';
   timestr[8] = '0';
   timestr[9] = '5';
   timestr[10] = 'T';
   timestr[11] = ' ';
   timestr[12] = '3';
   timestr[13] = ' ';
   timestr[14] = 'Z';
   timestr[15] = PLUSMINUS;
   timestr[16] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"1993-04-05-");
   utc_mkasctime(&utc,"1993-04-05- Z");
   utc_mkasctime(&utc,"1993-04-05- Z Q");
   utc_mkasctime(&utc,"1993-04-05- Z I");
   utc_mkasctime(&utc,"1993 - 04 - 05 -");
   utc_mkasctime(&utc,"1993-04-05-epsilon");
   utc_mkasctime(&utc,"1993-12:00");
   utc_mkasctime(&utc,"1993-12.00");
   utc_mkasctime(&utc,"1993-12,00");
   utc_mkasctime(&utc,"2987-13-45-");

   /* year part followed by inacc part */
   utc_mkasctime(&utc, "1993-04 T I 3.3");
   utc_mkasctime(&utc, "1993-04 T I -3.3");
   utc_mkasctime(&utc, "1993-04 T I inf");
   utc_mkasctime(&utc, "1993-04 T I INF");
   utc_mkasctime(&utc, "1993-04 T I -----");
   utc_mkasctime(&utc, "1993-04 T I epsilon");

   /* year part followed by partial day part */
   utc_mkasctime(&utc,"1993 T 11 Z");
   utc_mkasctime(&utc,"1993 T 11.34 +01");
   utc_mkasctime(&utc,"1993 T 11.34 +01:02");
   utc_mkasctime(&utc,"1993 T 11.34 -01");
   utc_mkasctime(&utc,"1993 T 11.34 -01:02");
   utc_mkasctime(&utc,"1993 T 11.34 epsilon");
   utc_mkasctime(&utc,"1993 T 11 +02");
   utc_mkasctime(&utc,"1993 T 11 -02:03");
   utc_mkasctime(&utc,"1993 T 11 -02");
   utc_mkasctime(&utc,"1993 T 11 -02:03");
   utc_mkasctime(&utc,"1993 T 11 epsilon");
   utc_mkasctime(&utc,"1993 T 11:12 Z");
   utc_mkasctime(&utc,"1993 T 11:12.34 +01");
   utc_mkasctime(&utc,"1993 T 11:12.34 +10:11");
   utc_mkasctime(&utc,"1993 T 11:12.34 -01");
   utc_mkasctime(&utc,"1993 T 11:12.34 -10:11");
   utc_mkasctime(&utc,"1993 T 11:12.34 epsilon");
   utc_mkasctime(&utc,"1993 T 11:12 +13");
   utc_mkasctime(&utc,"1993 T 11:12 +13:14");
   utc_mkasctime(&utc,"1993 T 11:12 -13");
   utc_mkasctime(&utc,"1993 T 11:12 -13:14");
   utc_mkasctime(&utc,"1993 T 11:12 epsilon");
   utc_mkasctime(&utc,"1993 T 12:13:14 Z");
   utc_mkasctime(&utc,"1993 T 12:13:14.15 +03");
   utc_mkasctime(&utc,"1993 T 12:13:14.15 +03:04");
   utc_mkasctime(&utc,"1993 T 12:13:14.15 -03");
   utc_mkasctime(&utc,"1993 T 12:13:14.15 -03:04");
   utc_mkasctime(&utc,"1993 T 12:13:14.15 epsilon");
   utc_mkasctime(&utc,"1993 T 12:13:14 +21");
   utc_mkasctime(&utc,"1993 T 12:13:14 +21:34");
   utc_mkasctime(&utc,"1993 T 12:13:14 -21");
   utc_mkasctime(&utc,"1993 T 12:13:14 -21:34");
   utc_mkasctime(&utc,"1993 T 12:13:14 epsilon");
   utc_mkasctime(&utc,"1993-04-05- --34 Z");
   utc_mkasctime(&utc,"1993-04-05- --34.45 +11");
   utc_mkasctime(&utc,"1993-04-05- --34.45 +11:12");
   utc_mkasctime(&utc,"1993-04-05- --34.45 -11");
   utc_mkasctime(&utc,"1993-04-05- --34.45 -11:12");
   utc_mkasctime(&utc,"1993-04-05- --34.45 epsilon");
   utc_mkasctime(&utc,"1993-04-05- --34 +12");
   utc_mkasctime(&utc,"1993-04-05- --34 +12:13");
   utc_mkasctime(&utc,"1993-04-05- --34 -12");
   utc_mkasctime(&utc,"1993-04-05- --34 -12:13");
   utc_mkasctime(&utc,"1993-04-05- --34 epsilon");
   utc_mkasctime(&utc,"1993-04-05- --- Z");
   utc_mkasctime(&utc,"1993-04-05- ---.56 +05");
   utc_mkasctime(&utc,"1993-04-05- ---.56 +05:06");
   utc_mkasctime(&utc,"1993-04-05- ---.56 -05");
   utc_mkasctime(&utc,"1993-04-05- ---.56 -05:06");
   utc_mkasctime(&utc,"1993-04-05- ---.56 epsilon");
   utc_mkasctime(&utc,"1993-04-05- --- +05");
   utc_mkasctime(&utc,"1993-04-05- --- +05:06");
   utc_mkasctime(&utc,"1993-04-05- --- -05");
   utc_mkasctime(&utc,"1993-04-05- --- -05:06");
   utc_mkasctime(&utc,"1993-04-05- --- epsilon");
   utc_mkasctime(&utc,"1993-04-05- -04 Z");
   utc_mkasctime(&utc,"1993-04-05- -04.23 +02");
   utc_mkasctime(&utc,"1993-04-05- -04.23 +02:02");
   utc_mkasctime(&utc,"1993-04-05- -04.23 -02");
   utc_mkasctime(&utc,"1993-04-05- -04.23 -02:02");
   utc_mkasctime(&utc,"1993-04-05- -04.23 epsilon");
   utc_mkasctime(&utc,"1993-04-05- -04 +02");
   utc_mkasctime(&utc,"1993-04-05- -04 +02:02");
   utc_mkasctime(&utc,"1993-04-05- -04 -02");
   utc_mkasctime(&utc,"1993-04-05- -04 -02:02");
   utc_mkasctime(&utc,"1993-04-05- -04 epsilon");
   utc_mkasctime(&utc,"1993-04-05- -04:05 Z");
   utc_mkasctime(&utc,"1993-04-05- -04:05.67 +16");
   utc_mkasctime(&utc,"1993-04-05- -04:05.67 +16:16");
   utc_mkasctime(&utc,"1993-04-05- -04:05.67 -16");
   utc_mkasctime(&utc,"1993-04-05- -04:05.67 -16:16");
   utc_mkasctime(&utc,"1993-04-05- -04:05.67 epsilon");
   utc_mkasctime(&utc,"1993-04-05- -04:05 +16");
   utc_mkasctime(&utc,"1993-04-05- -04:05 +16:16");
   utc_mkasctime(&utc,"1993-04-05- -04:05 -16");
   utc_mkasctime(&utc,"1993-04-05- -04:05 -16:16");
   utc_mkasctime(&utc,"1993-04-05- -04:05 epsilon");

   /* year part followed by partial day part followed by inacc part */
   utc_mkasctime(&utc,"1993 T 11 Z I 3.3");
   utc_mkasctime(&utc,"1993 T 11.34 +01 I 3.3");
   utc_mkasctime(&utc,"1993 T 11.34 +01:02 I 3.3");
   utc_mkasctime(&utc,"1993 T 11.34 -01 I 3.3");
   utc_mkasctime(&utc,"1993 T 11.34 -01:02 I 3.3");
   timestr[0] = '1';
   timestr[1] = '9';
   timestr[2] = '9';
   timestr[3] = '3';
   timestr[4] = ' ';
   timestr[5] = 'T';
   timestr[6] = ' ';
   timestr[7] = '1';
   timestr[8] = '1';
   timestr[9] = '.';
   timestr[10] = '3';
   timestr[11] = '4';
   timestr[12] = ' ';
   timestr[13] = '-';
   timestr[14] = '0';
   timestr[15] = '1';
   timestr[16] = ':';
   timestr[17] = '0';
   timestr[18] = '2';
   timestr[19] = ' ';
   timestr[20] = PLUSMINUS;
   timestr[21] = '3';
   timestr[22] = '\0';
   utc_mkasctime(&utc,timestr);
   utc_mkasctime(&utc,"1993 T 11.34 epsilon I 3.3");
   utc_mkasctime(&utc,"1993 T 11 +02 I 3.3");
   utc_mkasctime(&utc,"1993 T 11 -02:03 I 3.3");
   utc_mkasctime(&utc,"1993 T 11 -02 I 3.3");
   utc_mkasctime(&utc,"1993 T 11 -02:03 I 3.3");
   utc_mkasctime(&utc,"1993 T 11 epsilon I 3.3");
   utc_mkasctime(&utc,"1993 T 11:12 Z I 3.3");
   utc_mkasctime(&utc,"1993 T 11:12.34 +01 I3.3");
   utc_mkasctime(&utc,"1993 T 11:12.34 +10:11 I3.3");
   utc_mkasctime(&utc,"1993 T 11:12.34 -01 I3.3");
   utc_mkasctime(&utc,"1993 T 11:12.34 -10:11 I3.3");
   utc_mkasctime(&utc,"1993 T 11:12.34 epsilon I3.3");
   utc_mkasctime(&utc,"1993 T 11:12 +13 I3.3");
   utc_mkasctime(&utc,"1993 T 11:12 +13:14 I3.3");
   utc_mkasctime(&utc,"1993 T 11:12 -13 I-3.3");
   utc_mkasctime(&utc,"1993 T 11:12 -13:14 I-3.3");
   utc_mkasctime(&utc,"1993 T 11:12 epsilon I-3.3");
   utc_mkasctime(&utc,"1993 T 12:13:14 Z I-3.3");
   utc_mkasctime(&utc,"1993 T 12:13:14.15 +03 I-3.3");
   utc_mkasctime(&utc,"1993 T 12:13:14.15 +03:04 I-3.3");
   utc_mkasctime(&utc,"1993 T 12:13:14.15 -03 I-3.3");
   utc_mkasctime(&utc,"1993 T 12:13:14.15 -03:04 I-3.3");
   utc_mkasctime(&utc,"1993 T 12:13:14.15 epsilon I-3.3");
   utc_mkasctime(&utc,"1993 T 12:13:14 +21 I-3.3");
   utc_mkasctime(&utc,"1993 T 12:13:14 +21:34 I-3.3");
   utc_mkasctime(&utc,"1993 T 12:13:14 -21 I-3.3");
   utc_mkasctime(&utc,"1993 T 12:13:14 -21:34 I-3.3");
   utc_mkasctime(&utc,"1993 T 12:13:14 epsilon I-3.3");
   utc_mkasctime(&utc,"1993-04-05- --34 Z I-3.3");
   utc_mkasctime(&utc,"1993-04-05- --34.45 +11 I-3.3");
   utc_mkasctime(&utc,"1993-04-05- --34.45 +11:12 I3.3");
   utc_mkasctime(&utc,"1993-04-05- --34.45 -11 I3.3");
   utc_mkasctime(&utc,"1993-04-05- --34.45 -11:12 I3.3");
   utc_mkasctime(&utc,"1993-04-05- --34.45 epsilon I3.3");
   utc_mkasctime(&utc,"1993-04-05- --34 +12 I3.3");
   utc_mkasctime(&utc,"1993-04-05- --34 +12:13 I3.3");
   utc_mkasctime(&utc,"1993-04-05- --34 -12 I3.3");
   utc_mkasctime(&utc,"1993-04-05- --34 -12:13 I3.3");
   utc_mkasctime(&utc,"1993-04-05- --34 epsilon I3.3");
   utc_mkasctime(&utc,"1993-04-05- --- Z I3.3");
   utc_mkasctime(&utc,"1993-04-05- ---.56 +05 I3.3");
   utc_mkasctime(&utc,"1993-04-05- ---.56 +05:06 I3.3");
   utc_mkasctime(&utc,"1993-04-05- ---.56 -05 I3.3");
   utc_mkasctime(&utc,"1993-04-05- ---.56 -05:06 I3.3");
   utc_mkasctime(&utc,"1993-04-05- ---.56 epsilon I3.3");
   utc_mkasctime(&utc,"1993-04-05- --- +05 I3.3");
   utc_mkasctime(&utc,"1993-04-05- --- +05:06 I3.3");
   utc_mkasctime(&utc,"1993-04-05- --- -05 I3.3");
   utc_mkasctime(&utc,"1993-04-05- --- -05:06 I3.3");
   utc_mkasctime(&utc,"1993-04-05- --- epsilon I3.3");
   utc_mkasctime(&utc,"1993-04-05- -04 ZI3,3");
   utc_mkasctime(&utc,"1993-04-05- -04.23 +02 I3,3");
   utc_mkasctime(&utc,"1993-04-05- -04.23 +02:02I3,3");
   utc_mkasctime(&utc,"1993-04-05- -04.23 -02 I3,3");
   utc_mkasctime(&utc,"1993-04-05- -04.23 -02:02 I3,3");
   utc_mkasctime(&utc,"1993-04-05- -04.23 epsilon I3,3");
   utc_mkasctime(&utc,"1993-04-05- -04 +02 I3.3");
   utc_mkasctime(&utc,"1993-04-05- -04 +02:02 I 3");
   utc_mkasctime(&utc,"1993-04-05- -04 -02 I inf");
   utc_mkasctime(&utc,"1993-04-05- -04 -02:02 I inf");
   utc_mkasctime(&utc,"1993-04-05- -04 epsilon I epsilon");
   utc_mkasctime(&utc,"1993-04-05- -04:05 Z I epsilon");
   utc_mkasctime(&utc,"1993-04-05- -04:05.67 +16 I epsilon");
   utc_mkasctime(&utc,"1993-04-05- -04:05.67 +16:16 I epsilon");
   utc_mkasctime(&utc,"1993-04-05- -04:05.67 -16 I inf");
   utc_mkasctime(&utc,"1993-04-05- -04:05.67 -16:16 I inf");
   utc_mkasctime(&utc,"1993-04-05- -04:05.67 epsilon I inf");
   utc_mkasctime(&utc,"1993-04-05- -04:05 +16 I inf");
   utc_mkasctime(&utc,"1993-04-05- -04:05 +16:16 I 3");
   utc_mkasctime(&utc,"1993-04-05- -04:05 -16 I 3");
   utc_mkasctime(&utc,"1993-04-05- -04:05 -16:16 I 3");
   utc_mkasctime(&utc,"1993-04-05- -04:05 epsilon I 3");
}

/* call utc_mkascreltime with a large amount of different strings */
void ran12()
{
   int    i;
   char   timestr[80], badstring[100];
   utc_t  utc;

   utc_mkascsmplreltime(&utc, NULL);

   utc_mkascreltime(&utc, NULL);
   utc_mkascreltime(&utc, "");
   utc_mkascreltime(&utc,"\0");
   strcpy(timestr,"          ");
   utc_mkascreltime(&utc, timestr);
   for (i = 0; i < 99; i++)
      badstring[i] = 'I';
   badstring[i] = '\0';
   utc_mkascreltime(&utc, badstring);
   timestr[0]='"';
   timestr[1]='"';
   timestr[2]='\0';
   utc_mkascreltime(&utc, timestr);
   timestr[0]='"';
   timestr[1]='B';
   timestr[2]='"';
   timestr[3]='\0';
   utc_mkascreltime(&utc, timestr);
   utc_mkascreltime(&utc, "''");
   utc_mkascreltime(&utc, "'B'");

   utc_mkascreltime(&utc, "12");
   utc_mkascreltime(&utc, "12-");
   utc_mkascreltime(&utc, "12-3");
   utc_mkascreltime(&utc, "12-3:");
   utc_mkascreltime(&utc, "12-3-");
   utc_mkascreltime(&utc, "12-3-14");
   utc_mkascreltime(&utc, "12-3:14");
   utc_mkascreltime(&utc, "12-3:14:");
   utc_mkascreltime(&utc, "12-3:14:23");
   utc_mkascreltime(&utc, "12-3:14:23.");
   utc_mkascreltime(&utc, "12-3:14:23.4");
   utc_mkascreltime(&utc, "12-3:14:23,4");
   utc_mkascreltime(&utc, "12-3.14.23");
   utc_mkascreltime(&utc, "12-3,14,23");
   utc_mkascreltime(&utc, "3:14");
   utc_mkascreltime(&utc, "3:14:23");
   utc_mkascreltime(&utc, "12-3:14:23 I");
   utc_mkascsmplreltime(&utc, "12-3:14:23 I");
   timestr[0]='1';
   timestr[1]='2';
   timestr[2]='-';
   timestr[3]='3';
   timestr[4]=':';
   timestr[5]='1';
   timestr[6]='4';
   timestr[7]=':';
   timestr[8]='2';
   timestr[9]='3';
   timestr[10]=PLUSMINUS;
   timestr[11]='\0';
   utc_mkascreltime(&utc, timestr);
   utc_mkascsmplreltime(&utc, timestr);
   utc_mkascreltime(&utc, "12-3:14:23.0123456789");
   utc_mkascreltime(&utc, "12-3:14:23.123456789");
   utc_mkascreltime(&utc, "12-3:14:23.123A56789");
   timestr[0]='1';
   timestr[1]='2';
   timestr[2]=':';
   timestr[3]='1';
   timestr[4]='3';
   timestr[5]=':';
   timestr[6]='1';
   timestr[7]='4';
   timestr[8]='.';
   timestr[9]='3';
   timestr[10]='4';
   timestr[11]=' ';
   timestr[12]=PLUSMINUS;
   timestr[13]='5';
   timestr[14]='.';
   timestr[15]='5';
   timestr[16]='\0';
   utc_mkascreltime(&utc, timestr);
   utc_mkascsmplreltime(&utc, timestr);
   utc_mkascreltime(&utc, "12:13:14.34 I5.0");
   utc_mkascreltime(&utc, "12:13:14.34 I5.5");
   utc_mkascsmplreltime(&utc, "12:13:14.34 I5.5");

   utc_mkascreltime(&utc, "P");
   utc_mkascreltime(&utc, "P BLECH");
   utc_mkascreltime(&utc, "P T");
   utc_mkascreltime(&utc, "P I");
   utc_mkascreltime(&utc, "P I 3");
   utc_mkascreltime(&utc, "P I 0.0");
   utc_mkascreltime(&utc, "P I .");
   utc_mkascreltime(&utc, "P I .0");
   utc_mkascreltime(&utc, "P I 3.");
   utc_mkascreltime(&utc, "P I 3.2");
   utc_mkascsmplreltime(&utc, "P I 3.2");
   utc_mkascreltime(&utc, "P I 3.000");
   utc_mkascreltime(&utc, "P I 3.0123456789");
   utc_mkascreltime(&utc, "P 32 T");
   utc_mkascreltime(&utc, "P 32 I");
   utc_mkascreltime(&utc, "P 3.2 T");
   utc_mkascreltime(&utc, "P 3.2 I");
   utc_mkascreltime(&utc, "P 12 W 3 Y 4 D 5 M");
   utc_mkascreltime(&utc, "P 1961 Y 03 M 19 D 0 W");
   utc_mkascreltime(&utc, "P 196.31 Y 0.3 M 1.9 D 0 W");
   utc_mkascreltime(&utc, "P 19610 Y 03 M 19 D 10 W");
   utc_mkascreltime(&utc, "P 1961 Y 93 M 19 D 10 W");
   utc_mkascreltime(&utc, "P 1961 Y 03 M 49 D 10 W");
   utc_mkascreltime(&utc, "P 1961 Y 03 M 19 D 60 W");
   utc_mkascreltime(&utc, "P T BLECH");
   utc_mkascreltime(&utc, "P T I");
   utc_mkascreltime(&utc, "P T 5 I");
   utc_mkascreltime(&utc, "P T 3 H 7 M 8 S");
   utc_mkascreltime(&utc, "P T 3.3 H 5.6 M 2.5 S");
   utc_mkascreltime(&utc, "P T 345 H 7 M 8 S");
   utc_mkascreltime(&utc, "P T 3 H 72 M 8 S");
   utc_mkascreltime(&utc, "P T 3 H 7 M 88 S");
   utc_mkascreltime(&utc, "P T I BLECH");
   utc_mkascreltime(&utc, "P T I 3");
   utc_mkascreltime(&utc, "P T I .0");
   utc_mkascreltime(&utc, "P T I .3");
   utc_mkascreltime(&utc, "P T I ,3");
   utc_mkascreltime(&utc, "P T I 3.3");
   utc_mkascreltime(&utc, "P T I -3");
   utc_mkascreltime(&utc, "P T I -3.3");
   utc_mkascreltime(&utc, "P T I -3,3");
   utc_mkascreltime(&utc, "P T I -3.A");
   utc_mkascreltime(&utc, "P T I inf");
   utc_mkascreltime(&utc, "P T I INF");
   utc_mkascreltime(&utc, "P T I -----");
}

/* call obsolete library routines */
void ran13()
{
   utc_sharedptr = (SharedState *)7777;
   utc_sharedunmap();
}

/* call utc_sharedmap with utc_sharedptr set to NULL */
void bad01()
{
   utc_sharedptr = NULL;
   utc_sharedmap(0);
}

/* call utc_sharedmap with utc_sharedptr set to a bogus value */
void bad02()
{
   utc_sharedptr = (SharedState *)77;
   utc_sharedmap(0);
}

void blech()
{
#if !defined(__hpux1000p) && defined(__hpux)
   tet_infoline("test ran01: call dtstime incorrectly with second parameter not NULL\n");
   ran01();
#endif
   tet_infoline("test ran02: call utc_sharedunmap with utc_sharedptr NULL\n");
   ran02();
   tet_infoline("test ran03: call utc_sharedunmap with bogus utc_sharedptr value\n");
   ran03();
   tet_infoline("test ran04: call utc_mklocaltime with isdts pointer negative\n");
   ran04();
   tet_infoline("test ran05: call internal splittime with NULL parameters\n");
   ran05();
   tet_infoline("test ran06: call internal mergetime with various parameters\n");
   ran06();
   tet_infoline("test ran07: call some utc functions with NULL parameters\n");
   ran07();
   tet_infoline("test ran08: call utc_mulftime with a variety of parameters\n");
   ran08();
   tet_infoline("test ran09: call tzset with a bunch of different parameters\n");
   ran09();
   tet_infoline("test ran10: call utc_cvt functions with a bunch of different paramters\n");
   ran10();
   tet_infoline("test ran11: call utc_mkasctime with a whole bunch of different strings\n");
   ran11();
   tet_infoline("test ran12: call utc_mkascreltime with a bunch of different strings\n");
   ran12();
   tet_infoline("test ran13: call obsolete utc library routines for coverage only\n");
   ran13();
   tet_infoline("test bad01: call utc_sharedmap with bad shared memory file\n");
   bad01();
   tet_infoline("test bad02: call utc_sharedmap with bogus shared pointer value\n");
   bad02();

   tet_result(TET_PASS);
}
